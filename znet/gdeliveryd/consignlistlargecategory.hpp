
#ifndef __GNET_CONSIGNLISTLARGECATEGORY_HPP
#define __GNET_CONSIGNLISTLARGECATEGORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignListLargeCategory : public GNET::Protocol
{
	#include "consignlistlargecategory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("ConsignListLargeCategory: receive. page=%d roleid=%d\n",page,roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL!=pinfo )
		{
			ConsignListLargeCategory_Re re;
			re.localsid = localsid;
			re.page = page;
			re.retcode = ConsignManager::GetInstance()->GetConsignListLargeCategory(re.nodes,re.page,re.timestamp);
			GDeliveryServer::GetInstance()->Send( pinfo->linksid, re);
		}
	}
};

};

#endif
