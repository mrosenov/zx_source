
#ifndef __GNET_SECTUPDATE_HPP
#define __GNET_SECTUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gsectskill"

namespace GNET
{

class SectUpdate : public GNET::Protocol
{
	#include "sectupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SectUpdate sectid=%d, roleid=%d, reputation=%d, skills=%d", sectid, roleid, reputation, skills.size());

		SectManager::Instance()->UpdateSect(sectid, reputation, skills);
	}
};

};

#endif
