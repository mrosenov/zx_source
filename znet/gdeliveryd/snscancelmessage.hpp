
#ifndef __GNET_SNSCANCELMESSAGE_HPP
#define __GNET_SNSCANCELMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "snsmessagemanager.h"
#include "snscancelmessage_re.hpp"
#include "localmacro.h"
#include "mapuser.h"

namespace GNET
{

class SNSCancelMessage : public GNET::Protocol
{
	#include "snscancelmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);

		if (pinfo!=NULL && pinfo->roleid==roleid && pinfo->linksid==sid)
		{
			SNSCancelMessage_Re cancel_re;
			cancel_re.roleid = roleid;
			cancel_re.message_id = message_id;
			cancel_re.message_type = message_type;
			cancel_re.localsid = localsid;

			SNSMessageManager *mm = (SNSMessageManager *)SNSMessageManager::GetInstance();
			SNSMessage msg;

			if (mm->GetMessage(message_type, message_id, msg))
			{
				if (msg.roleid==roleid || pinfo->IsGM())
				{
					mm->CancelMessage(roleid, message_id, message_type);
					cancel_re.retcode = ERR_SUCCESS;
				}
				else
					cancel_re.retcode = SNS_ERR_PRIVILEGE;
			}
			else
				cancel_re.retcode = SNS_ERR_MSGNOTFOUND;

			manager->Send(sid, cancel_re);
		}
	}
};

};

#endif
