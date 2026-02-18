#include "snsmessagemanager.h"
#include "snsplayerinfomanager.h"
#include "dbsnsgetmessage.hrp"
#include "dbsnsupdatemessage.hrp"
#include "gamedbclient.hpp"
#include "factionmanager.h"
#include "mapuser.h"
#include "localmacro.h"
#include "timer.h"
#include "namemanager.h"

namespace GNET
{

void SNSMessageManager::OnDBConnect(Protocol::Manager *manager, int sid)
{
	/*
	if (!open)
		return;
	Clear();

	DBSNSGetMessage *rpc = (DBSNSGetMessage *)Rpc::Call(RPC_DBSNSGETMESSAGE, DBSNSGetMessageArg(0, -1));
	manager->Send(sid, rpc);
	*/
}

void SNSMessageManager::OnLoad(GSNSMessage &gmsg)
{
	if (!ValidMessageType(gmsg.message_type)) return;

	NameManager::GetInstance()->FindName(gmsg.roleid, gmsg.rolename); 
	GLeaveMessageVector::iterator lit, lite = gmsg.leave_msgs.end();
	for (lit = gmsg.leave_msgs.begin(); lit != lite; ++lit)
		NameManager::GetInstance()->FindName(lit->roleid, lit->rolename); 
	GApplyInfoVector::iterator ait, aite = gmsg.apply_list.end();
	for (ait = gmsg.apply_list.begin(); ait != aite; ++ait)
		NameManager::GetInstance()->FindName(ait->roleid, ait->rolename); 
	aite = gmsg.agree_list.end();
	for (ait = gmsg.agree_list.begin(); ait != aite; ++ait)
		NameManager::GetInstance()->FindName(ait->roleid, ait->rolename); 
	aite = gmsg.deny_list.end();
	for (ait = gmsg.deny_list.begin(); ait != aite; ++ait)
		NameManager::GetInstance()->FindName(ait->roleid, ait->rolename); 

	SNSMessage msg;
	GSNSToSNSMessage(gmsg, msg);

	m_msgmap[(int)msg.message_type][msg.message_id] = msg;

	if (m_msgindex[(int)msg.message_type] <= msg.message_id)
		m_msgindex[(int)msg.message_type] = msg.message_id+1;

	m_players.insert(PLAYERMAP::value_type(msg.roleid, pair<char, int>(msg.message_type, msg.message_id)));
}

void SNSMessageManager::PressMessage(SNSMessage &msg)
{
	if (!ValidMessageType(msg.message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)msg.message_type];
	if (msgmap.size() >= SNS_LIMIT_MSGNUM)
	{
		//删除最老的一条消息
		MSGMAP::iterator it, ie=msgmap.end(), itdst=msgmap.begin();
		int count = 0;
		for (it=msgmap.begin(); it!=ie && count<SNS_LIMIT_MSGNUM-10; ++it, ++count)
		{
			if (it->second.charm < itdst->second.charm)
				itdst = it;
		}
		CancelMessage(itdst->second.roleid, itdst->second.message_id, itdst->second.message_type);
	}

	msg.message_id = m_msgindex[(int)msg.message_type]++;
	//帮派招募信息需要记录招募时角色的帮派id
	if (msg.message_type == SNS_MESSAGE_TYPE_FACTION)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(msg.roleid);
		if (pinfo != NULL)
			msg.factionid = pinfo->factionid;
	}
	msgmap[msg.message_id] = msg; 

	m_players.insert(PLAYERMAP::value_type(msg.roleid, pair<char, int>(msg.message_type, msg.message_id)));

	SNSPlayerInfoManager *pm = SNSPlayerInfoManager::GetInstance();
	pm->UpdateSNSPlayerPressTime(msg.roleid, Timer::GetTime());
	pm->UpdateSNSRoleInfo(msg.roleid);

	SyncDB(0, msg);
}

void SNSMessageManager::CancelMessage(int roleid, int message_id, char message_type)
{
	if (!ValidMessageType(message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	MSGMAP::iterator mit = msgmap.find(message_id);
	if (mit != msgmap.end())
	{
		SNSMessage &msg = mit->second;
		if (message_type == SNS_MESSAGE_TYPE_FACTION)
		{
			FactionDetailInfo *pfaction = FactionManager::Instance()->Find(msg.factionid);
			if (pfaction != NULL)
			{
				pfaction->UpdateCharm(msg.charm);
				SNSPlayerInfoManager::GetInstance()->SetSNSPlayerCharm((SNS_CHARM_TYPE)message_type, pfaction->info.master, pfaction->GetCharm());
			}
		}
		else
			SNSPlayerInfoManager::GetInstance()->UpdateSNSPlayerCharm((SNS_CHARM_TYPE)message_type, roleid, msg.charm);

		SyncDB(1, msg);

		//删除该消息
		msgmap.erase(mit);
	}

	PLAYERMAP::iterator it = m_players.lower_bound(roleid);
	while (it!=m_players.end() && it->first==roleid)
	{
		if (it->second.first==message_type && it->second.second==message_id)
		{
			m_players.erase(it);
			break;
		}
		++it;
	}
}

class DescendantLeaveMsg
{
	std::set<short> parent_indices;
public:
	DescendantLeaveMsg(short root)
	{
		parent_indices.insert(root);
	}

	bool operator () (const GLeaveMessage &leave_msg)
	{
		if (parent_indices.find(leave_msg.dst_index) != parent_indices.end())
		{
			parent_indices.insert(leave_msg.index);
			return true;
		}
		else
			return false;
	}
};

void SNSMessageManager::CancelLeaveMessage(int roleid, int message_id, char message_type, short leavemsg_index)
{
	if (!ValidMessageType(message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	MSGMAP::iterator mit = msgmap.find(message_id);
	if (mit != msgmap.end())
	{
		SNSMessage &msg = mit->second;

		GLeaveMessageVector::iterator lit, lie = msg.leave_msgs.end();
		for (lit = msg.leave_msgs.begin(); lit != lie; ++lit)
		{
			if (lit->index == leavemsg_index)
				break;
		}

		if (lit != lie)
		{
			msg.leave_msgs.erase(lit);
			msg.leave_msgs.GetVector().erase(std::remove_if(msg.leave_msgs.begin(), msg.leave_msgs.end(), DescendantLeaveMsg(leavemsg_index)), msg.leave_msgs.end());
			SyncDB(1, msg);
		}
	}
}

void SNSMessageManager::ApplyMessage(char oper, const SNSPlayerInfoCache &info, char message_type, int message_id, GLeaveMessage &lmsg)
{
	if (!ValidMessageType(message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	MSGMAP::iterator it = msgmap.find(message_id);
	if (it != msgmap.end())
	{
		SNSMessage &msg = it->second;
		SNSPlayerInfoManager *pm = SNSPlayerInfoManager::GetInstance();
		
		lmsg.index = msg.leave_msgs[msg.leave_msgs.size()-1].index+1;
		if (msg.leave_msgs.size() >= SNS_LIMIT_LEAVEMSGNUM)
		{
			//删除除发布者留言以外深度最大的叶子留言
			GLeaveMessageVector::iterator it=msg.leave_msgs.begin(), ie=msg.leave_msgs.end();
			const static unsigned int FLAG_INNER_NODE = 0x80000000;
			map<short, unsigned int> id2level;
			id2level[it->index] = 0;

			for (++it; it != ie; ++it)
			{
				if (id2level.find(it->dst_index) != id2level.end())
				{
					unsigned int &msg_level = id2level[it->dst_index];
					id2level[it->index] = (msg_level&~FLAG_INNER_NODE)+1;
					msg_level |= FLAG_INNER_NODE;
				}
				else
					id2level[it->index] = 0;
			}

			it = msg.leave_msgs.begin();
			GLeaveMessageVector::iterator it_maxlevel = ie, it_maxlevel_second = ie;
			unsigned int maxlevel = 0, maxlevel_second = 0;
			for (++it; it != ie; ++it)
			{
				unsigned int msg_level = id2level[it->index];
				if (!(msg_level & FLAG_INNER_NODE))				//如果是叶节点
				{
					if (it_maxlevel == ie)
					{
						it_maxlevel = it;
						maxlevel = msg_level;
					}
					else if (maxlevel<msg_level || (maxlevel==msg_level && it->time<it_maxlevel->time))
					{
						it_maxlevel_second = it_maxlevel;
						maxlevel_second = maxlevel;
						it_maxlevel = it;
						maxlevel = msg_level;
					}
					else if (it_maxlevel_second==ie || maxlevel_second<msg_level || (maxlevel_second==msg_level && it->time<it_maxlevel_second->time))
					{
						it_maxlevel_second = it;
						maxlevel_second = msg_level;
					}
				}
			}

			if (it_maxlevel!=ie && it_maxlevel->index!=lmsg.dst_index)
				msg.leave_msgs.erase(it_maxlevel);
			else if (it_maxlevel_second!=ie && it_maxlevel_second->index!=lmsg.dst_index)
				msg.leave_msgs.erase(it_maxlevel_second);
			else
				msg.leave_msgs.erase(++msg.leave_msgs.begin());
		}

		msg.leave_msgs.push_back(lmsg);

		if (oper == 0) //申请
		{
			GApplyInfoVector::iterator it, ie = msg.apply_list.end();
			for (it = msg.apply_list.begin(); it != ie; ++it)
			{
				if (it->roleid == info.playerinfo.roleid)
				{
					it->count++;
					it->last_time = Timer::GetTime();
					break;
				}
			}

			if (it == ie)
			{
				GApplyInfo app(info.playerinfo.roleid, info.playerinfo.rolename, 1, Timer::GetTime(), info.roleinfo.level, info.roleinfo.gender);
				msg.apply_list.push_back(app);

				if (msg.apply_list.size() > SNS_LIMIT_APPLYVOTENUM)
					msg.apply_list.erase(msg.apply_list.begin());
			}

			msg.charm += SNS_LIMIT_APPLYINCCHARM;
			pm->UpdateSNSPlayerApplyTime(info.playerinfo.roleid, Timer::GetTime());
		}
		else //普通留言
		{
			msg.charm += SNS_LIMIT_LEAVEMSGINCCHARM;
			pm->UpdateSNSPlayerLeaveMsgTime(info.playerinfo.roleid, Timer::GetTime());
		}
		if (msg.charm > SNS_LIMIT_MSGCHARM)
			msg.charm = SNS_LIMIT_MSGCHARM;
	
		SyncDB(0, msg);
	}
}

int SNSMessageManager::GetLeaveMsgLevel(char message_type, int message_id, short leavemsg_index)
{
	if (!ValidMessageType(message_type)) return -1;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	MSGMAP::iterator it = msgmap.find(message_id);
	if (it != msgmap.end())
	{
		SNSMessage &msg = it->second;
		int level = 0;
		size_t high_last = msg.leave_msgs.size();

		while (leavemsg_index >= 0)
		{
			size_t low = 0;
			size_t high = high_last; 

			while (low < high)
			{
				size_t mid = (low+high)/2;
				short mid_index = msg.leave_msgs[mid].index;

				if (mid_index > leavemsg_index)
					high = mid;
				else if (mid_index < leavemsg_index)
					low = mid+1;
				else
				{
					++level;
					leavemsg_index = msg.leave_msgs[mid].dst_index;
					high_last = mid;
					break;
				}
			}

			if (low >= high)
				return -1;
		}

		return level;
	}
	
	return -1;
}

void SNSMessageManager::VoteMessage(char oper, const SNSPlayerInfoCache &info, char message_type, int message_id)
{
	if (!ValidMessageType(message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	MSGMAP::iterator it = msgmap.find(message_id);
	if (it != msgmap.end())
	{
		SNSMessage &msg = it->second;
		
		GApplyInfoVector *vote_list = NULL;
		if (oper == 1)	//支持
		{
			vote_list = &msg.agree_list;
			msg.charm += SNS_LIMIT_VOTEAGREEINCCHARM;
			if (msg.charm > SNS_LIMIT_MSGCHARM)
				msg.charm = SNS_LIMIT_MSGCHARM;
		}
		else		//反对
		{
			vote_list = &msg.deny_list;
			msg.charm -= SNS_LIMIT_VOTEDENYDECCHARM;
			if (msg.charm < 0)
				msg.charm = 0;
		}

		GApplyInfoVector::iterator it, ie = vote_list->end();
		for (it = vote_list->begin(); it != ie; ++it)
		{
			if (it->roleid == info.playerinfo.roleid)
			{
				it->count++;
				it->last_time = Timer::GetTime();
				break;
			}
		}
		if (it == ie)
		{
			GApplyInfo app(info.playerinfo.roleid, info.playerinfo.rolename, 1, Timer::GetTime(), info.roleinfo.level, info.roleinfo.gender);
			vote_list->push_back(app);

			if (vote_list->size() > SNS_LIMIT_APPLYVOTENUM)
				vote_list->erase(vote_list->begin());
		}

		SNSPlayerInfoManager *pm = SNSPlayerInfoManager::GetInstance();
		pm->UpdateSNSPlayerVoteTime(info.playerinfo.roleid, Timer::GetTime());

		SyncDB(0, msg);
	}
}

void SNSMessageManager::AcceptApply(char oper, int roleid, int apply_roleid, char message_type, int message_id)
{
	if (!ValidMessageType(message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	MSGMAP::iterator it = msgmap.find(message_id);
	if (it!=msgmap.end() && it->second.roleid==roleid)
	{
		SNSMessage &msg = it->second;

		//接受申请
		if (oper == 0)
		{
			msg.accept_list.push_back(apply_roleid);
		}
		else //拒绝申请
		{
			GApplyInfoVector::iterator it, ie = msg.apply_list.end();
			for (it = msg.apply_list.begin(); it != ie; ++it)
			{
				if (it->roleid == apply_roleid)
				{
					msg.apply_list.erase(it);
					break;
				}
			}
		}

		SyncDB(0, msg);
	}
}

void SNSMessageManager::ListMessage(char message_type, SNSMessageBriefVector &lists)
{
	if (!ValidMessageType(message_type)) return;

	MSGMAP &msgmap = m_msgmap[(int)message_type];
	size_t sz = msgmap.size();

	if (sz > 0)
	{
		lists.resize(sz);
		size_t i = 0;
		MSGMAP::iterator it, ie = msgmap.end();
		SNSPlayerInfoManager *pm = SNSPlayerInfoManager::GetInstance();

		for (it = msgmap.begin(); it != ie; ++it, i++)
		{
			SNSMessageToBrief(it->second, lists[i]); 
			lists[i].charmstar_player = pm->GetSNSPlayerCharm((SNS_CHARM_TYPE)message_type, it->second.roleid);
		}
	}
}

void SNSMessageManager::SyncDB(char oper, const SNSMessage &msg)
{
	GSNSMessage gmsg;
	GSNSToSNSMessage(msg, gmsg);
	DBSNSUpdateMessage *rpc = (DBSNSUpdateMessage *)Rpc::Call(RPC_DBSNSUPDATEMESSAGE, DBSNSUpdateMessageArg(oper, gmsg));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}


//清楚发布超过7天的信息
bool SNSMessageManager::Update()
{
	if (!IsFinishLoading())
	{
		if (b_startload == false && GameDBClient::GetInstance()->IsConnect()
				&& open == true && NameManager::GetInstance()->IsInit())
		{
			b_startload = true;
			DBSNSGetMessage *rpc = (DBSNSGetMessage *)Rpc::Call(RPC_DBSNSGETMESSAGE, DBSNSGetMessageArg(0, -1));
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		LOG_TRACE("snsmessagemanager update startload %d finish_load %d", b_startload, m_finishLoading);
		return true;
	}
	int now = Timer::GetTime();

	for (int i = 0; i < SNS_MESSAGE_TYPE_MAX; i++)
	{
		MSGMAP &msgmap = m_msgmap[i];
		MSGMAP::iterator it = msgmap.begin(), ie = msgmap.end();
		while (it != ie)
		{
			SNSMessage &msg = it->second;
			++it;

			if (now-msg.press_time > SNS_LIMIT_MSGLIFETIME)
				CancelMessage(msg.roleid, msg.message_id, msg.message_type);
		}
	}

	return true;
}

void SNSMessageManager::OnRolenameChange(int roleid, const Octets & rolename)
{
	PLAYERMAP::iterator it = m_players.lower_bound(roleid);
	while (it!=m_players.end() && it->first==roleid)
	{
		MSGMAP::iterator mit = m_msgmap[(int)(it->second.first)].find(it->second.second);
		if (mit != m_msgmap[(int)(it->second.first)].end())
		{
			if (mit->second.roleid == roleid)
				mit->second.rolename = rolename;
		}
		++it;
	}
}


};
