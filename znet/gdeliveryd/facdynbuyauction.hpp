
#ifndef __GNET_FACDYNBUYAUCTION_HPP
#define __GNET_FACDYNBUYAUCTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynBuyAuction : public GNET::Protocol
{
	#include "facdynbuyauction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facdynbuyauction fid %d itemid %d time %d", fid, itemid, time);

		FAC_DYNAMIC::buy_auction_item log = {itemid, time, 20, {}};
		FactionDynamic::GetName(rolename, log.rolename, log.rolenamesize);
		FactionManager::Instance()->RecordDynamic(fid, FAC_DYNAMIC::BUY_AUCTION_ITEM, log);
	}
};

};

#endif
