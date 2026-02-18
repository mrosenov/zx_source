
#ifndef __GNET_GCHANGEROLENAME_RE_HPP
#define __GNET_GCHANGEROLENAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


void change_name_response(int roleid, char * name, size_t name_len);

namespace GNET
{

class GChangeRolename_Re : public GNET::Protocol
{
	#include "gchangerolename_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(retcode == 0)
		{
			change_name_response(roleid, (char*)newname.begin(), newname.size());
		}
	}
};

};

#endif
