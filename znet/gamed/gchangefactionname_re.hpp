
#ifndef __GNET_GCHANGEFACTIONNAME_RE_HPP
#define __GNET_GCHANGEFACTIONNAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void change_faction_name(int roleid, int fid, char type, char * name, size_t name_len);

namespace GNET
{

class GChangeFactionName_Re : public GNET::Protocol
{
	#include "gchangefactionname_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(retcode == 0)
		{
			change_faction_name(roleid, fid, scale, (char*)newname.begin(), newname.size());
		}
	}
};

};

#endif
