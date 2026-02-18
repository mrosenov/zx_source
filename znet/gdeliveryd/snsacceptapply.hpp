
#ifndef __GNET_SNSACCEPTAPPLY_HPP
#define __GNET_SNSACCEPTAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "mapuser.h"
#include "localmacro.h"
#include "snsplayerinfomanager.h"
#include "snsmessagemanager.h"
#include "snsacceptapply_re.hpp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "sendsnsresult.hpp"
#include "gproviderserver.hpp"

namespace GNET
{

class SNSAcceptApply : public GNET::Protocol
{
	#include "snsacceptapply"

	void BroadCast(const Octets &rolename, const Octets &apply_rolename)
	{
		ChatBroadCast chat;
		chat.channel = GP_CHAT_SYSTEM;
		chat.srcroleid = MSG_SNSACCEPTAPPLY;
		Marshal::OctetsStream data;
		data << rolename << apply_rolename << message_type;
		chat.msg = data;
		LinkServer::GetInstance().BroadcastProtocol(chat);
	}

	int CheckAcceptCondition(const SNSMessage &msg, Octets &rolename)
	{
		if (msg.roleid == roleid)
		{
			if (msg.message_type == SNS_MESSAGE_TYPE_WEDDING
				&&  oper == 0 && msg.accept_list.size() > 0) //征婚只能接受一次
				return SNS_ERR_MARRYMULTIACCEPT;

			std::vector<int>::const_iterator int_it, int_ie = msg.accept_list.end();
			for (int_it = msg.accept_list.begin(); int_it != int_ie; ++int_it)
			{
				if (*int_it == apply_roleid)
					break;
			}
			if (int_it != int_ie)
				return SNS_ERR_APPLYMULTIACCEPT;
			
			GApplyInfoVector::const_iterator it, ie = msg.apply_list.end();
			for (it = msg.apply_list.begin(); it != ie; ++it)
			{
				if (it->roleid == apply_roleid)
				{
					rolename = it->rolename;
					break;
				}
			}
			if (it == ie)
				return SNS_ERR_APPLYROLEIDNOTFOUND;
		}
		else
			return SNS_ERR_PRIVILEGE;

		return SNS_ERR_SUCCESS;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);

		if (pinfo!=NULL && pinfo->roleid==roleid && pinfo->linksid==sid)
		{
			SNSAcceptApply_Re accept_re;
			accept_re.roleid = roleid;
			accept_re.apply_roleid = apply_roleid;
			accept_re.message_type = message_type;
			accept_re.message_id = message_id;
			accept_re.oper = oper;
			accept_re.localsid = localsid;

			SNSMessageManager *mm = (SNSMessageManager *)SNSMessageManager::GetInstance();
			SNSMessage msg;

			if (mm->GetMessage(message_type, message_id, msg))
			{
				Octets rolename;
				accept_re.retcode = CheckAcceptCondition(msg, rolename);

				if (accept_re.retcode == SNS_ERR_SUCCESS)
				{
					mm->AcceptApply(oper, roleid, apply_roleid, message_type, message_id);

					if (oper == 0)
						BroadCast(pinfo->name, rolename);
				}
			}
			else
				accept_re.retcode = SNS_ERR_MSGNOTFOUND;

			manager->Send(sid, accept_re);

			SendSNSResult send_re;
			send_re.retcode = accept_re.retcode;
			send_re.roleid = roleid;
			send_re.message_type = message_type;
			if (oper == 0)
				send_re.op_type = SNS_OP_ACCEPTAPPLY;
			else
				send_re.op_type = SNS_OP_REJECTAPPLY;
			send_re.charm = SNSPlayerInfoManager::GetInstance()->GetSNSPlayerCharm((SNS_CHARM_TYPE)message_type, roleid);

			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, send_re);
		}
	}
};

};

#endif
