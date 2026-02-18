
#ifndef __GNET_FACDYNWINAUCTION_HPP
#define __GNET_FACDYNWINAUCTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynWinAuction : public GNET::Protocol
{
	#include "facdynwinauction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facdynwinauction fid %d itemid %d cost %d", fid, itemid, cost);

		FAC_DYNAMIC::win_auction_item log = {itemid, cost, 20, {}};
		FactionDynamic::GetName(rolename, log.rolename, log.rolenamesize);
		FactionManager::Instance()->RecordDynamic(fid, FAC_DYNAMIC::WIN_AUCTION_ITEM, log);
	}
};

};

#endif
