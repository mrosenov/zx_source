
#ifndef __GNET_SNSGETMESSAGE_HPP
#define __GNET_SNSGETMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "snsgetmessage_re.hpp"
#include "snsmessagemanager.h"
#include "localmacro.h"

namespace GNET
{

class SNSGetMessage : public GNET::Protocol
{
	#include "snsgetmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		SNSGetMessage_Re get_re;
		get_re.message_type = message_type;
		get_re.message_id = message_id;
		get_re.roleid = roleid;
		get_re.localsid = localsid;

		SNSMessage msg;
		if (SNSMessageManager::GetInstance()->GetMessage(message_type, message_id, msg))
		{
			get_re.retcode = SNS_ERR_SUCCESS;
			get_re.leave_msgs = msg.leave_msgs;
			get_re.apply_list = msg.apply_list;
			get_re.agree_list = msg.agree_list;
			get_re.deny_list = msg.deny_list;
		}
		else
			get_re.retcode = SNS_ERR_MSGNOTFOUND;

		manager->Send(sid, get_re);
	}
};

};

#endif
