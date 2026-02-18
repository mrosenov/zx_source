
#ifndef __GNET_CONSIGNLISTALL_HPP
#define __GNET_CONSIGNLISTALL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "consignmanager.h"

namespace GNET
{

class ConsignListAll : public GNET::Protocol
{
	#include "consignlistall"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("ConsignListAll: receive. index=%lld,page=%d category=%d direct=%d\n",index,page,category,direction);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL!=pinfo )
		{
			ConsignListAll_Re re;
			re.localsid = localsid;
			re.page = page;
			re.category = category;
			re.retcode = ConsignManager::GetInstance()->GetConsignListWrap(roleid,re.nodes,re.page,direction,category,index);
			GDeliveryServer::GetInstance()->Send( pinfo->linksid, re);
		}
	}
};

};

#endif
