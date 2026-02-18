
#ifndef __GNET_CONSIGNLISTROLE_HPP
#define __GNET_CONSIGNLISTROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "consignmanager.h"

namespace GNET
{

class ConsignListRole : public GNET::Protocol
{
	#include "consignlistrole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("ConsignListRole: receive. roleid=%d,page=%d",roleid,page);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL!=pinfo )
		{
			ConsignListRole_Re re;
			re.localsid = localsid;
			re.page = page;
			int category = 90074;
			re.retcode = ConsignManager::GetInstance()->GetConsignListRoleWrap(roleid,re.nodes,re.page,0,category,0);
			GDeliveryServer::GetInstance()->Send( pinfo->linksid, re);
		}
	}
};

};

#endif
