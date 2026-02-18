
#ifndef __GNET_FACTIONINFOREQ_HPP
#define __GNET_FACTIONINFOREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtmanager.h"
#include "snsplatformclient.hpp"
#include "factionidbean"

namespace GNET
{

class FactionInfoReq : public GNET::Protocol
{
	#include "factioninforeq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionInfoReq facitionid=%lld ftype=%d",factionid.factionid,factionid.ftype);
		if(manager == SNSPlatformClient::GetInstance())
		{
			GTManager::Instance()->OnFactionInfoReq(localuid,factionid.ftype,factionid.factionid,GGT_SNS_TYPE);
		}
		else
			GTManager::Instance()->OnFactionInfoReq(localuid,factionid.ftype,factionid.factionid,GGT_GT_TYPE);
	}
};

};

#endif
