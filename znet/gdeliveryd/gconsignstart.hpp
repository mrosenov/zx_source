
#ifndef __GNET_GCONSIGNSTART_HPP
#define __GNET_GCONSIGNSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "consignstarterr.hpp"
#include "gconsignend.hpp"
#include "consignmanager.h"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "gproviderserver.hpp"

namespace GNET
{

class GConsignStart : public GNET::Protocol
{
	#include "gconsignstart"
	
	void SendErr( int retcode,PlayerInfo& ui )
	{
		GDeliveryServer::GetInstance()->Send(
				ui.linksid,
				ConsignStartErr(retcode,ui.localsid )
			);
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gconsignstart: receive. roleid=%d,consign_type=%d,money=%d,item_id=%d,item_idx=%d,item_cnt=%d,\
				price=%d,shelf_period=%d,sell_to_rolename.size=%d\n",roleid,consign_type,money,item_id,item_idx,item_cnt,price,shelf_period,sell_to_rolename.size());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL==pinfo )
		{
			Log::log(LOG_ERR,"GConsignStart: role %d is not online", roleid);
			return;
		}
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(pinfo->userid);//to get ip
		if( NULL==userinfo)
		{
			Log::log(LOG_ERR,"GConsignStart: user %d role %d is not online", pinfo->userid, roleid);
			return;
		}
		//if pinfo or userinfo is null, let gs timeout and disconnect the player
		int retcode = ConsignManager::GetInstance()->TryPrePost(*this,userinfo->ip,*pinfo);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("gconsignstart: TryPrePost err. roleid=%d,consign_type=%d,money=%d,item_id=%d,item_idx=%d,item_cnt=%d,price=%d,shelf_period=%d\n",roleid,consign_type,money,item_id,item_idx,item_cnt,price,shelf_period);
			retcode = ConsignManager::GetClientRqstRetcode(retcode);
			SendErr( retcode,*pinfo );
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,GConsignEnd(roleid,0));    
		}
	}
};

};

#endif
