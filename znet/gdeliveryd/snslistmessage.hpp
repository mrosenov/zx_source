
#ifndef __GNET_SNSLISTMESSAGE_HPP
#define __GNET_SNSLISTMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "snsmessagemanager.h"

namespace GNET
{

class SNSListMessage : public GNET::Protocol
{
	#include "snslistmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SNSListMessage_Re ls_re;
		ls_re.retcode = ERR_SUCCESS;
		ls_re.roleid = roleid;
		ls_re.message_type = message_type;
		ls_re.localsid = localsid;
		SNSMessageManager::GetInstance()->ListMessage(message_type, ls_re.lists);

		manager->Send(sid, ls_re);
	}
};

};

#endif
