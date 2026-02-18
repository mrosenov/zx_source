
#ifndef __GNET_SNSCANCELLEAVEMESSAGE_HPP
#define __GNET_SNSCANCELLEAVEMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "snscancelleavemessage_re.hpp"
#include "snsmessagemanager.h"
#include "gleavemessage"


namespace GNET
{

class SNSCancelLeaveMessage : public GNET::Protocol
{
	#include "snscancelleavemessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);

		if (pinfo!=NULL && pinfo->roleid==roleid && pinfo->linksid==sid)
		{
			SNSCancelLeaveMessage_Re cancel_re;
			cancel_re.roleid = roleid;
			cancel_re.message_id = message_id;
			cancel_re.message_type = message_type;
			cancel_re.leavemsg_index = leavemsg_index;
			cancel_re.localsid = localsid;

			SNSMessageManager *mm = (SNSMessageManager *)SNSMessageManager::GetInstance();
			SNSMessage msg;
			GLeaveMessage leavemsg;

			if (mm->GetMessage(message_type, message_id, msg)
				&& mm->GetLeaveMessage(message_type, message_id, leavemsg_index, leavemsg))
			{
				if (msg.roleid==roleid || pinfo->IsGM())
				{
					mm->CancelLeaveMessage(roleid, message_id, message_type, leavemsg_index);
					cancel_re.retcode = ERR_SUCCESS;
				}
				else
					cancel_re.retcode = SNS_ERR_PRIVILEGE;
			}
			else
				cancel_re.retcode = SNS_ERR_LEAVEMSGNOTFOUND;

			manager->Send(sid, cancel_re);
		}
	}
};

};

#endif
