
#ifndef __GNET_TEAMRAIDUPDATECHAR_HPP
#define __GNET_TEAMRAIDUPDATECHAR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidroledata"

namespace GNET
{

class TeamRaidUpdateChar : public GNET::Protocol
{
	#include "teamraidupdatechar"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                if (manager==GDeliveryClient::GetInstance())
		{
			GLinkServer::GetInstance()->Send(localsid,this);
		}
		else if(manager==GLinkServer::GetInstance())
		{
			if ( !GLinkServer::ValidRole( sid,roledata.roleid ) )
				return;
			localsid = sid;
			GDeliveryClient::GetInstance()->SendProtocol( this );
		}
	}
};

};

#endif
