
#ifndef __GNET_SENDSNSRESULT_HPP
#define __GNET_SENDSNSRESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void handle_message_result(int roleid, char message_type, char op_type, int charm); //gs should implement this	

namespace GNET
{

class SendSNSResult : public GNET::Protocol
{
	#include "sendsnsresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(retcode == 0){
			handle_message_result(roleid, message_type, op_type, charm);		
		}
	}
};

};

#endif
