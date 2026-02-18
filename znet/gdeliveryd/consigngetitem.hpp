
#ifndef __GNET_CONSIGNGETITEM_HPP
#define __GNET_CONSIGNGETITEM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "consigngetitem_re.hpp"
#include "mapuser.h"
#include "consignmanager.h"
#include "gdeliveryserver.hpp"

namespace GNET
{

class ConsignGetItem : public GNET::Protocol
{
	#include "consigngetitem"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("ConsignGetItem: receive. sn=%lld,roleid=%d \n", sn,roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL!=pinfo )
		{
			ConsignGetItem_Re re;
			re.localsid = localsid;
			re.sn = sn;
			re.retcode = ConsignManager::GetInstance()->GetItemObj(sn,re.obj);
			GDeliveryServer::GetInstance()->Send( pinfo->linksid, re);
		}
	}
};

};

#endif
