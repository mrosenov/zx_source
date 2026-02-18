
#ifndef __GNET_FACDYNPUTAUCTION_HPP
#define __GNET_FACDYNPUTAUCTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynPutAuction : public GNET::Protocol
{
	#include "facdynputauction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facdynputauction fid %d itemid %d time %d task_id %d", fid, itemid, time, task_id);

		FAC_DYNAMIC::put_auction_item log = {itemid, time, task_id, 20, {}};
		FactionDynamic::GetName(rolename, log.rolename, log.rolenamesize);
		FactionManager::Instance()->RecordDynamic(fid, FAC_DYNAMIC::PUT_AUCTION_ITEM, log);
	}
};

};

#endif
