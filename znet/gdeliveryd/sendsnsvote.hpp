
#ifndef __GNET_SENDSNSVOTE_HPP
#define __GNET_SENDSNSVOTE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "timer.h"

namespace GNET
{

class SendSNSVote : public GNET::Protocol
{
	#include "sendsnsvote"

	int GetLastVoteTime(const GApplyInfoVector &vote_list, int roleid)
	{
		int time = 0;

		GApplyInfoVector::const_iterator it, ie = vote_list.end();
		for (it = vote_list.begin(); it != ie; ++it)
		{
			if (it->roleid == roleid)
			{
				if (time < it->last_time)
					time = it->last_time;
				break;
			}
		}

		return time;
	}

	//两次投票要间隔15分钟
	int CheckVoteCondition(const SNSMessage &msg, const SNSPlayerInfoCache &info)
	{
		if (info.playerinfo.roleid == msg.roleid)
			return SNS_ERR_FORBIDSELF;

		int now = Timer::GetTime();
		if (now-info.playerinfo.vote_time < SNS_LIMIT_COOLTIME)
			return SNS_ERR_VOTECOOLTIME;

		int time = 0;
		int agree_time = GetLastVoteTime(msg.agree_list, info.playerinfo.roleid);
		int deny_time = GetLastVoteTime(msg.deny_list, info.playerinfo.roleid);

		if (time < agree_time) time = agree_time;
		if (time < deny_time) time = deny_time;

		if (now - time < SNS_LIMIT_VOTETIME)
			return SNS_ERR_VOTETIME15MIN;

		return SNS_ERR_SUCCESS;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SendSNSResult resp;
		resp.roleid = roleid;
		resp.message_type = message_type;
		if (oper == 1)
			resp.op_type = SNS_OP_AGREE;
		else
			resp.op_type = SNS_OP_DENY;

		SNSMessageManager *mm = SNSMessageManager::GetInstance();
		SNSMessage msg;
		SNSPlayerInfoCache info;
		bool find_message = mm->GetMessage(message_type, message_id, msg); 
		bool find_player = SNSPlayerInfoManager::GetInstance()->GetSNSPlayerInfo(roleid, info);

		if (find_message && find_player)
		{
			resp.retcode = CheckVoteCondition(msg, info);

			if (resp.retcode == SNS_ERR_SUCCESS)
				mm->VoteMessage(oper, info, message_type, message_id);
		}
		else if (!find_message)
			resp.retcode = SNS_ERR_MSGNOTFOUND;
		else
			resp.retcode = SNS_ERR_ROLEOFFLINE;

		PlayerInfo *pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo != NULL)
		{
			SNSVote_Re vote_re;
			vote_re.retcode = resp.retcode;
			vote_re.roleid = roleid;
			vote_re.oper = oper;
			vote_re.message_id = message_id;
			vote_re.message_type = message_type;
			vote_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, vote_re);
		}

		manager->Send(sid, resp);
	}
};

};

#endif
