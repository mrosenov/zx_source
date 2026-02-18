
#ifndef __GNET_OPENBANQUETNOTIFY_RE_HPP
#define __GNET_OPENBANQUETNOTIFY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetNotify_Re : public GNET::Protocol
{
	#include "openbanquetnotify_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetNotify_Re::retcode=%d,roleid=%d,localsid=%d", retcode, roleid, localsid);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
