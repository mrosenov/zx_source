
#ifndef __GNET_OPENBANQUETGETFIELD_RE_HPP
#define __GNET_OPENBANQUETGETFIELD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gopenbanquetroleinfo"

namespace GNET
{

class OpenBanquetGetField_Re : public GNET::Protocol
{
	#include "openbanquetgetfield_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetGetField_Re localsid=%d, red_team.size=%d, blue_team.size=%d", localsid, red_team.size(), blue_team.size());
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
