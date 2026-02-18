
#ifndef __GNET_UPDATETOPTABLE_HPP
#define __GNET_UPDATETOPTABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "topmanager.h"

namespace GNET
{

class UpdateTopTable : public GNET::Protocol
{
	#include "updatetoptable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("top","updatetop:tableid=%d:size=%d:time=%d:weekly=%d",
			table.id,table.items.size(),table.time,weekly);
		TopManager::Instance()->UpdateTop(table, weekly);
	}
};

};

#endif
