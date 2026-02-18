
#ifndef __GNET_GCONSIGNSTARTROLE_HPP
#define __GNET_GCONSIGNSTARTROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "consignplayer_re.hpp"
#include "gconsignendrole.hpp"
#include "consignmanager.h"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "gproviderserver.hpp"

namespace GNET
{

class GConsignStartRole : public GNET::Protocol
{
	#include "gconsignstartrole"

	void SendErr( int retcode,PlayerInfo& ui )
	{
		GDeliveryServer::GetInstance()->Send(
				ui.linksid,
				ConsignPlayer_Re(ui.roleid,ui.localsid,retcode)
			);
		DEBUG_PRINT("gconsignstartrole, ConsignPlayer_Re retcode=%d",retcode);
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gconsignstartrole: receive. roleid=%d,consign_type=%d,price=%d,shelf_period=%d,sell_to_rolename.size=%d\n",roleid,consign_type,price,shelf_period,sell_to_rolename.size());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL==pinfo )
		{
			Log::log(LOG_ERR,"GConsignStartRole: role %d is not online", roleid);
			return;
		}
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(pinfo->userid);//to get ip
		if( NULL==userinfo)
		{
			Log::log(LOG_ERR,"GConsignStartRole: user %d role %d is not online", pinfo->userid, roleid);
			return;
		}
		
		int retcode = ConsignManager::GetInstance()->TryPrePostRole(*this,userinfo->ip,*pinfo);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("gconsignstartrole: TryPrePost err. roleid=%d,consign_type=%d,price=%d,shelf_period=%d, retcode=%d\n",roleid,consign_type,price,shelf_period,retcode);
			retcode = ConsignManager::GetClientRqstRetcode(retcode);
			SendErr( retcode,*pinfo );
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,GConsignEndRole(roleid,retcode));    
		}
	}
};

};

#endif
