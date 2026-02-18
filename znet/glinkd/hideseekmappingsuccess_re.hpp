
#ifndef __GNET_HIDESEEKMAPPINGSUCCESS_RE_HPP
#define __GNET_HIDESEEKMAPPINGSUCCESS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HideSeekMappingSuccess_Re : public GNET::Protocol
{
	#include "hideseekmappingsuccess_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
