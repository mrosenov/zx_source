
#ifndef __GNET_SECTRECRUIT_RE_HPP
#define __GNET_SECTRECRUIT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void sect_add_member(int master, int disciple);

namespace GNET
{

class SectRecruit_Re : public GNET::Protocol
{
	#include "sectrecruit_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		//师徒关系更新
		if(retcode == 0)
		{
			sect_add_member(sectid, disciple);
		}
	}
};

};

#endif
