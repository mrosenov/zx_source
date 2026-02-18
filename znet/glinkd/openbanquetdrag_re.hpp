
#ifndef __GNET_OPENBANQUETDRAG_RE_HPP
#define __GNET_OPENBANQUETDRAG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetDrag_Re : public GNET::Protocol
{
	#include "openbanquetdrag_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetDrag_Re::gs_id=%d,map_id=%d,localsid=%d", gs_id, map_id, localsid);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
