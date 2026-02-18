
#ifndef __GNET_ADDCASH_RE_HPP
#define __GNET_ADDCASH_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_get_cash_total(int roleid);
namespace GNET
{

class AddCash_Re : public GNET::Protocol
{
	#include "addcash_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(retcode==ERR_SUCCESS)
		{
			int roleid = userid;
			player_get_cash_total(roleid);
		}
	}
};

};

#endif
