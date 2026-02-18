
#ifndef __GNET_SENDSNSAPPLY_HPP
#define __GNET_SENDSNSAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "snsmessagemanager.h"
#include "snsplayerinfomanager.h"
#include "gapplyinfo"
#include "localmacro.h"
#include "snsapplynotify.hpp"


namespace GNET
{

class SendSNSApply : public GNET::Protocol
{
	#include "sendsnsapply"

	//每人同一天只能申请一次
	bool HavePlayerAppliedToday(int roleid, const SNSMessage &msg)
	{
		GApplyInfoVector::const_iterator it, ie=msg.apply_list.end();

		for (it = msg.apply_list.begin(); it != ie; ++it)
		{
			if (it->roleid == roleid)
			{
				int now = Timer::GetTime();
				if (now-it->last_time < SNS_LIMIT_APPLYTIME)
					return true;
				break;
			}
		}

		return false;
	}

	bool CheckOccupationCondition(unsigned cond_occup, unsigned char role_occup)
	{
		if (cond_occup == (unsigned char)-1)
			return true;
		else if (cond_occup == 0)
			return ((role_occup>=7 && role_occup<=9) || (role_occup>=19 && role_occup<=21));
		else if (cond_occup == 1)
			return ((role_occup>=4 && role_occup<=6) || (role_occup>=16 && role_occup<=18));
		else if (cond_occup == 2)
			return ((role_occup>=10 && role_occup<=12) || (role_occup>=22 && role_occup<=24));
		else if (cond_occup == 3)
			return (role_occup>=25 && role_occup<=29);
		else if (cond_occup == 4)
			return ((role_occup>=1 && role_occup<=3) || (role_occup>=13 && role_occup<=15));
		else if (cond_occup == 5)
			return (role_occup>=33 && role_occup<=38);
		else if (cond_occup == 6)
			return (role_occup>=39 && role_occup<=44);
		else if (cond_occup == 7)
			return (role_occup>=45 && role_occup<=50);
		else if (cond_occup == 8)
			return (role_occup>=51 && role_occup<=56);
		else if (cond_occup == 9)
			return (role_occup>=64 && role_occup<=69);
		else if (cond_occup == 10)
			return (role_occup>=96 && role_occup<=101);
		else
			return false;
	}

	int CheckApplyCondition(const SNSMessage &msg, const SNSPlayerInfoCache &info, const PlayerInfo *pinfo)
	{
		if (message.size() > SNS_LIMIT_MESSAGESIZE)
			return SNS_ERR_LENGTH;

		if (msg.leave_msgs[msg.leave_msgs.size()-1].index >= SHRT_MAX)
			return SNS_ERR_LEAVEMSGFULL;

		if (oper == 0) 
		{
			if (roleid == msg.roleid)
				return SNS_ERR_FORBIDSELF;

			//应征的冷却时间为15分钟
			int now = Timer::GetTime();
			if (now-info.playerinfo.apply_time < SNS_LIMIT_COOLTIME)
				return SNS_ERR_APPLYCOOLTIME;

			//每天只能对同条消息的申请1次
			if (HavePlayerAppliedToday(roleid, msg))
				return SNS_ERR_APPLYTIME24HOUR;

			const GApplyCondition &cond = msg.condition;

			if (cond.gender!=-1 && cond.gender!=info.roleinfo.gender)
				return SNS_ERR_GENDER;

			if (cond.level > pinfo->level)
				return SNS_ERR_LEVEL;

			if (cond.faction_level > info.roleinfo.faction_level)
				return SNS_ERR_FACTIONLEVEL;

			if (msg.message_type==SNS_MESSAGE_TYPE_WEDDING)
			{
				if (msg.accept_list.size() > 0)
					return SNS_ERR_MARRYALREADYACCEPT;

				if (info.roleinfo.spouse!=0)
					return SNS_ERR_MARRIED;
			}

			//90级（含）以上不能申请师徒信息
			if (msg.message_type == SNS_MESSAGE_TYPE_SECT)
			{
				if (pinfo->level >= SNS_LIMIT_SECTAPPLYLEVEL)
					return SNS_ERR_SECTAPPLYLEVEL;

				if (pinfo->sectid != 0)
					return SNS_ERR_ALREADYINSECT;
			}

			if (msg.message_type == SNS_MESSAGE_TYPE_FACTION)
			{
				if (pinfo->factionid != 0)
					return SNS_ERR_ALREADYINFACTION;
			}

			//检查门派是否符合要求
			if (cond.occupation!=(unsigned char)-1 && !CheckOccupationCondition(cond.occupation, info.roleinfo.occupation))
				return SNS_ERR_OCCUPATION;
		}
		else
		{
			//留言的冷却时间为15分钟
			int now = Timer::GetTime();
			if (now-info.playerinfo.leavemsg_time < SNS_LIMIT_LEAVEMSGCOOLTIME)
				return SNS_ERR_LEAVEMSGCOOLTIME;

			int leavemsg_level = SNSMessageManager::GetInstance()->GetLeaveMsgLevel(message_type, message_id, dst_index);
			if (leavemsg_level==-1 || leavemsg_level>=SNS_LIMIT_LEAVEMSGMAXLEVEL)
				return SNS_ERR_LEAVEMSGLEVEL;
		}

		return SNS_ERR_SUCCESS;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SNSApply_Re apply_re;

		SendSNSResult resp;
		resp.roleid = roleid;
		resp.message_type = message_type;
		if (oper == 0)
			resp.op_type = SNS_OP_APPLY;
		else
			resp.op_type = SNS_OP_APPLYMSG;

		PlayerInfo *pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo != NULL)
		{
			SNSMessageManager *mm = SNSMessageManager::GetInstance();
			SNSMessage msg;
			SNSPlayerInfoCache info;
			bool find_message = mm->GetMessage(message_type, message_id, msg); 
			bool find_player = SNSPlayerInfoManager::GetInstance()->GetSNSPlayerInfo(roleid, info);

			if (find_message && find_player)
			{
				resp.retcode = CheckApplyCondition(msg, info, pinfo);

				if (resp.retcode == SNS_ERR_SUCCESS)
				{
					char type;
					if (oper == 0)
					{
						type = SNS_LEAVEMESSAGE_TYPE_APPLY;
						dst_index = -1;
					}
					else
						type = SNS_LEAVEMESSAGE_TYPE_LEAVE;

					GLeaveMessage lmsg(-1, dst_index, type, roleid, info.playerinfo.rolename, Timer::GetTime(), message);
					mm->ApplyMessage(oper, info, message_type, message_id, lmsg);
					apply_re.index = lmsg.index;

					if (oper == 0)
					{
						//如果发布者在线，要通知发布者有人申请
						PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(msg.roleid);
						if (pinfo != NULL)
						{
							SNSApplyNotify notify(pinfo->roleid, roleid, info.playerinfo.rolename, message_type, message_id, pinfo->localsid);
							GDeliveryServer::GetInstance()->Send(pinfo->linksid, notify);
						}
					}
				}
			}
			else if (!find_message)
				resp.retcode = SNS_ERR_MSGNOTFOUND;
			else
				resp.retcode = SNS_ERR_ROLEOFFLINE;

			apply_re.retcode = resp.retcode;
			apply_re.roleid = roleid;
			apply_re.oper = oper;
			apply_re.message_type = message_type;
			apply_re.message_id = message_id;
			apply_re.dst_index = dst_index;
			apply_re.message = message;
			apply_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, apply_re);
		}
		else
			resp.retcode = SNS_ERR_ROLEOFFLINE;

		manager->Send(sid, resp);
	}
};

};

#endif
