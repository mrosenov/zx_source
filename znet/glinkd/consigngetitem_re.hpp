
#ifndef __GNET_CONSIGNGETITEM_RE_HPP
#define __GNET_CONSIGNGETITEM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "groleinventory"

namespace GNET
{

class ConsignGetItem_Re : public GNET::Protocol
{
	#include "consigngetitem_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
