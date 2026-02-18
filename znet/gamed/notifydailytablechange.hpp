
#ifndef __GNET_NOTIFYDAILYTABLECHANGE_HPP
#define __GNET_NOTIFYDAILYTABLECHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gsp_if.h"

namespace GNET
{

class NotifyDailyTableChange : public GNET::Protocol
{
	#include "notifydailytablechange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GMSV::NotifyTableChange(tableid);	
	}
};

};

#endif
