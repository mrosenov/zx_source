
#ifndef __GNET_GFACTIONPKRAIDDEPOSIT_HPP
#define __GNET_GFACTIONPKRAIDDEPOSIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"


namespace GNET
{

class GFactionPkRaidDeposit : public GNET::Protocol
{
	#include "gfactionpkraiddeposit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gsid=GProviderServer::GetInstance()->GetGSID(sid);
		int ret=RaidManager::GetInstance()->FactionPkRaidDeposit( gsid,index, mapid, money);
		LOG_TRACE("GFactionPkRaidDeposit gsid=%d index=%d money=%d ret=%d",gsid,index,money,ret);
	}
};

};

#endif
