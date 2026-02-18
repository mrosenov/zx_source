
#ifndef __GNET_OPENBANQUETQUEUECOUNT_RE_HPP
#define __GNET_OPENBANQUETQUEUECOUNT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetQueueCount_Re : public GNET::Protocol
{
	#include "openbanquetqueuecount_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetQueueCount_Re::gs_id=%d,map_id=%d,queue_number=%d,localsid=%d", gs_id, map_id, queue_number, localsid);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
