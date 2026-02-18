
#ifndef __GNET_CONSIGNCANCELPOST_HPP
#define __GNET_CONSIGNCANCELPOST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignCancelPost : public GNET::Protocol
{
	#include "consigncancelpost"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("consigncancelpost: roleid=%d,sn=%lld",roleid,sn);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL == pinfo )
		{
			Log::log(LOG_ERR,"gdelivery::consigncancelpost: role %d is not online", roleid);
			return;
		}

		int retcode = ConsignManager::GetInstance()->TryPreCancelPost(roleid,sn,*pinfo);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("consigncancelpost err: roleid=%d,sn=%lld,retcode=%d",roleid,sn,retcode);
 			retcode = ConsignManager::GetClientRqstRetcode(retcode);
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,ConsignCancelPost_Re(retcode,sn,pinfo->localsid)); 
		}
	}
};

};

#endif
