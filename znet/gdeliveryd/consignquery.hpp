
#ifndef __GNET_CONSIGNQUERY_HPP
#define __GNET_CONSIGNQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
#include "consignmanager.h"
#include "gdeliveryserver.hpp"

namespace GNET
{

class ConsignQuery : public GNET::Protocol
{
	#include "consignquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("ConsignQuery: receive. roleid=%d,localsid=%d \n", roleid,localsid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL!=pinfo )
		{
			ConsignQuery_Re re;
			re.localsid = localsid;
			if(ConsignManager::GetInstance()->GetRoleConsignList(roleid,re.items,localsid))
				GDeliveryServer::GetInstance()->Send( pinfo->linksid, re);
		}
	}
};

};

#endif
