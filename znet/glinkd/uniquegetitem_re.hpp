
#ifndef __GNET_UNIQUEGETITEM_RE_HPP
#define __GNET_UNIQUEGETITEM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class UniqueGetItem_Re : public GNET::Protocol
{
	#include "uniquegetitem_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
