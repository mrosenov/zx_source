#ifndef _GNET_SNS_MESSAGE_MANAGER_H_
#define _GNET_SNS_MESSAGE_MANAGER_H_

#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include "localmacro.h"
#include "itimer.h"
#include "timer.h"
#include "snsmessage"
#include "gsnsmessage"
#include "snsmessagebrief"
#include "snsplayerinfocache"
#include "gleavemessage"
#include "gapplyinfo"

using std::vector;
using std::map;
using std::multimap;
using std::pair;
using std::unique;

namespace GNET
{
class SNSMessageManager : public IntervalTimer::Observer
{
public:
	static SNSMessageManager *GetInstance() { static SNSMessageManager instance; return &instance; }

	template <typename SNSMESSAGE1, typename SNSMESSAGE2>
	static void GSNSToSNSMessage(const SNSMESSAGE1 &from, SNSMESSAGE2 &to)
	{
		to.message_id = from.message_id;
		to.roleid = from.roleid; 
		to.rolename = from.rolename;
		to.message_type = from.message_type;
		to.gender = from.gender; 
		to.factionid = from.factionid;
		to.charm = from.charm;
		to.message = from.message;
		to.press_time = from.press_time;
		to.condition = from.condition;
		to.leave_msgs = from.leave_msgs;
		to.apply_list = from.apply_list;
		to.agree_list = from.agree_list;
		to.deny_list = from.deny_list;
		to.accept_list = from.accept_list;
	}

	static void SNSMessageToBrief(const SNSMessage &from, SNSMessageBrief &to)
	{
		to.message_id = from.message_id;
		to.roleid = from.roleid;
		to.gender = from.gender; 
		to.rolename = from.rolename;
		to.charm = from.charm;
		to.attention = from.agree_list.size()+from.deny_list.size();
		to.apply_count = from.apply_list.size();
		to.press_time = from.press_time;
		to.message = from.message;
		to.condition = from.condition;
		to.accept_list = from.accept_list;
	}

	bool Initialize()
	{
		SetFinishLoading(false);

		IntervalTimer::Attach(this,10000000/IntervalTimer::Resolution());
		open = true;
		return true;
	}

	bool IsFinishLoading() { return m_finishLoading; }

	void SetFinishLoading(bool finish = true) { m_finishLoading = finish; }

	bool Update();

	void OnDBConnect(Protocol::Manager *manager, int sid);

	void OnLoad(GSNSMessage &gmsg);
		
	void GetSNSPlayerList(vector<int> &roleid_list)
	{

		PLAYERMAP::iterator it, ie = m_players.end();
		for (it = m_players.begin(); it != ie; ++it)
		{
			roleid_list.push_back(it->first);
		}

		roleid_list.erase(unique(roleid_list.begin(), roleid_list.end()), roleid_list.end());
	}

	bool HavePlayerPressedMessage(int roleid) { return m_players.find(roleid)!=m_players.end(); }

	int GetPlayerPressedMsgTypeCount(int roleid, char message_type)
	{
		int count = 0;

		PLAYERMAP::iterator it = m_players.lower_bound(roleid);
		while (it!=m_players.end() && it->first==roleid)
		{
			if (it->second.first == message_type)
				count++;
			++it;
		}
		return count;
	}

	bool GetMessage(char message_type, int message_id, SNSMessage &msg)
	{
		if (!ValidMessageType(message_type)) return false;

		MSGMAP &msgmap = m_msgmap[(int)message_type];
		MSGMAP::iterator it = msgmap.find(message_id);

		if (it != msgmap.end())
		{
			msg = it->second;
			return true;
		}
		else
		{
			LOG_TRACE("SNSMessageManager:: fail to GetMessage, message_type=%d, message_id=%d\n", message_type, message_id);
			return false;
		}
	}

	bool GetLeaveMessage(char message_type, int message_id, short leavemsg_index, GLeaveMessage &leavemsg)
	{

		if (!ValidMessageType(message_type)) return false;

		MSGMAP &msgmap = m_msgmap[(int)message_type];
		MSGMAP::iterator it = msgmap.find(message_id);

		if (it != msgmap.end())
		{
			SNSMessage &msg = it->second;

			GLeaveMessageVector::const_iterator lit, lie = msg.leave_msgs.end();
			for (lit = msg.leave_msgs.begin(); lit != lie; ++lit)
			{
				if (lit->index == leavemsg_index)
				{
					leavemsg = *lit;
					return true;
				}
			}
		}
		return false;
	}
	
	void PressMessage(SNSMessage &msg);

	void CancelMessage(int roleid, int message_id, char message_type);

	void CancelLeaveMessage(int roleid, int message_id, char message_type, short leavemsg_index);

	void ApplyMessage(char oper, const SNSPlayerInfoCache &info, char message_type, int message_id, GLeaveMessage &lmsg);

	int GetLeaveMsgLevel(char message_tyep, int message_id, short leavemsg_index);

	void VoteMessage(char oper, const SNSPlayerInfoCache &info, char message_type, int message_id);

	void AcceptApply(char oper, int roleid, int apply_roleid, char message_type, int message_id);

	void ListMessage(char message_type, SNSMessageBriefVector &lists);

	void OnRolenameChange(int roleid, const Octets & rolename);
private:
	SNSMessageManager() : open(false), b_startload(false) {}
	void SyncDB(char oper, const SNSMessage &msg);

	bool ValidMessageType(char message_type) { return (message_type>=0 && message_type<SNS_MESSAGE_TYPE_MAX); }
	
	void Clear()
	{
		for (int i = 0; i < SNS_MESSAGE_TYPE_MAX; i++)
		{
			m_msgmap[i].clear();
			m_msgindex[i] = 0;
		}

		m_players.clear();
	}

public:
	typedef map<int, SNSMessage> MSGMAP;
	typedef multimap<int, pair<char, int> > PLAYERMAP;			//roleid->message_type, message_id

private:
	bool open;
	MSGMAP m_msgmap[SNS_MESSAGE_TYPE_MAX];
	int m_msgindex[SNS_MESSAGE_TYPE_MAX];	//每类消息的下一个未用index
	PLAYERMAP m_players;			//用户roleid到发布过消息列表
	bool m_finishLoading;			//是否从数据库中加载完毕
	bool b_startload;			//是否开始加载
};
};
#endif
