
#ifndef __GNET_SECTEXPEL_HPP
#define __GNET_SECTEXPEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "sectmanager.h"
#include "sectexpel_re.hpp"


namespace GNET
{

class SectExpel : public GNET::Protocol
{
	#include "sectexpel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SectExpel master=%d, disciple=%d", master, disciple);
		int ret = SectManager::Instance()->DoExpel(master, disciple);
		if(ret)
		{
			SectExpel_Re re;
			re.retcode = ret;
			re.sectid = master;
			re.disciple = disciple;
			re.localsid = localsid;
			manager->Send(sid, re);
			return;
		}
	}
};

};

#endif
