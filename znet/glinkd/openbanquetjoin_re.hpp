
#ifndef __GNET_OPENBANQUETJOIN_RE_HPP
#define __GNET_OPENBANQUETJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetJoin_Re : public GNET::Protocol
{
	#include "openbanquetjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetJoin_Re::retcode=%d,gs_id=%d,map_id=%d,queue_number=%d,localsid=%d", retcode, gs_id, map_id, queue_number, localsid);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
