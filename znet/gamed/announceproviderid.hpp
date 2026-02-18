
#ifndef __GNET_ANNOUNCEPROVIDERID_HPP
#define __GNET_ANNOUNCEPROVIDERID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "lineinfo"

void OnDeliveryConnected();
void OnGTConnected();
#include "gproviderclient.hpp"
#include "log.h"
#include "localmacro.h"
namespace GNET
{

class AnnounceProviderID : public GNET::Protocol
{
	#include "announceproviderid"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GProviderClient::Attach(id,(GProviderClient*)manager))
		{
			manager->Close(sid);
			Log::log(LOG_ERR,"gamed:: identical provider server id exists. connection failed.");
			return;
		} else if (id == 0)
		{
			// id 为0说明是由Delivyer发来的
			OnDeliveryConnected();
		} else if (id == 100) 
		{
			// id 为100说明是GT Provider
			if ((attr | GT_SERVER_CONNECTED)) 
			{
				OnGTConnected();
			}
		}
	}	
};

};

#endif
