
#ifndef __GNET_SENDSNSPRESSMESSAGE_HPP
#define __GNET_SENDSNSPRESSMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "snsmessagemanager.h"
#include "snsplayerinfomanager.h"
#include "sendsnsresult.hpp"
#include "mapuser.h"
#include "localmacro.h"
#include "gdeliveryserver.hpp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"

namespace GNET
{

class SendSNSPressMessage : public GNET::Protocol
{
	#include "sendsnspressmessage"

	//检查是否满足发布条件
	int CheckPressCondition(unsigned char level)
	{
		SNSMessageManager *mm = SNSMessageManager::GetInstance();
		if (!mm->IsFinishLoading())
			return SNS_ERR_NOTREADY;

		if (message.size() > SNS_LIMIT_MESSAGESIZE)
			return SNS_ERR_LENGTH;

		if (level < SNS_LIMIT_PRESSLEVEL)
			return SNS_ERR_LEVEL;

		SNSPlayerInfoManager *pm = SNSPlayerInfoManager::GetInstance();
		SNSPlayerInfoCache info;
		if (!pm->GetSNSPlayerInfo(roleid, info))
			return SNS_ERR_ROLEOFFLINE;

		int charm_star = pm->GetSNSPlayerCharmStar((SNS_CHARM_TYPE)message_type, roleid);
		int now = (int)Timer::GetTime();
		int last_press_time = pm->GetSNSPlayerPressTime(roleid);
		int diff_time = now-last_press_time;
		
		if ((charm_star>=4 && diff_time<SNS_LIMIT_PRESSTIME_CHARMSTARGE4) || diff_time<SNS_LIMIT_PRESSTIME_CHARMSTARLT4)
		{
			if (charm_star >= 4)
				return SNS_ERR_PRESSTIME12HOUR;
			else
				return SNS_ERR_PRESSTIME24HOUR;
		}

		int count = mm->GetPlayerPressedMsgTypeCount(roleid, message_type);
		if ((charm_star>=3 && count>=2) || (charm_star<3 && count>=1))
			return SNS_ERR_REPEAT;

		if (message_type==SNS_MESSAGE_TYPE_WEDDING && info.roleinfo.spouse!=0)
			return SNS_ERR_MARRIED;

		if (message_type==SNS_MESSAGE_TYPE_SECT && level<SNS_LIMIT_SECTPRESSLEVEL)
			return SNS_ERR_LEVEL;

		if (message_type==SNS_MESSAGE_TYPE_FACTION && info.roleinfo.factiontitle!=TITLE_MASTER)
			return SNS_ERR_TITLE;

		return SNS_ERR_SUCCESS;
	}

	void BroadCast(const Octets &name)
	{
		ChatBroadCast chat;
		chat.channel = GP_CHAT_SYSTEM;
		chat.srcroleid = MSG_SNSPRESSMSG;
		Marshal::OctetsStream data;
		data << name << message_type;
		chat.msg = data;
		LinkServer::GetInstance().BroadcastProtocol(chat);
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SNSPressMessage_Re press_re;
		SendSNSResult resp;

		resp.roleid = roleid;
		resp.message_type = message_type;
		resp.op_type = SNS_OP_PRESSMESSAGE;

		PlayerInfo *pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo != NULL)
		{
			resp.retcode = CheckPressCondition(pinfo->level);

			if (resp.retcode == SNS_ERR_SUCCESS)
			{
				SNSPlayerInfoCache info;

				if (SNSPlayerInfoManager::GetInstance()->GetSNSPlayerInfo(roleid, info))
				{
					SNSMessage msg(-1, message_type, roleid, info.playerinfo.rolename, info.roleinfo.gender, 0, 0, message, Timer::GetTime(), condition);
					msg.leave_msgs.push_back(GLeaveMessage(0, -1, SNS_LEAVEMESSAGE_TYPE_PRESS, roleid, info.playerinfo.rolename, Timer::GetTime(), message));
					SNSMessageManager::GetInstance()->PressMessage(msg);
					press_re.message_id = msg.message_id;

					//魅力星级大于等于2时，世界喊话
					if (SNSPlayerInfoManager::GetInstance()->GetSNSPlayerCharmStar((SNS_CHARM_TYPE)message_type, roleid) >= 2)
						BroadCast(info.playerinfo.rolename);
				}
				else
					resp.retcode = SNS_ERR_ROLEOFFLINE;
			}

			press_re.retcode = resp.retcode;
			press_re.roleid = roleid;
			press_re.message_type = message_type;
			press_re.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, press_re);
		}
		else
			resp.retcode = SNS_ERR_ROLEOFFLINE;

		manager->Send(sid, resp);
	}
};

};

#endif
