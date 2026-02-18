
#ifndef __GNET_GOPENBANQUETREGADDINFO_HPP
#define __GNET_GOPENBANQUETREGADDINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetregadd"
#include "openbanquetmanager.h"

namespace GNET
{

class GOpenBanquetRegAddInfo : public GNET::Protocol
{
	#include "gopenbanquetregaddinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("GOpenBanquetRegAddInfo", "gs_id=%d:size=%d", gsid, tlist.size());
		OpenBanquetManager::GetInstance()->RegisterServer(sid, gsid, tlist);

	}
};

};

#endif
