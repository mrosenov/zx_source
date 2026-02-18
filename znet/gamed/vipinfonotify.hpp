
#ifndef __GNET_VIPINFONOTIFY_HPP
#define __GNET_VIPINFONOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void recv_vipaward_info_notify(int roleid, int cur_time, int start_time, int end_time, int vipaward_level, int recent_total_cash);

namespace GNET
{

class VIPInfoNotify : public GNET::Protocol
{
	#include "vipinfonotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		recv_vipaward_info_notify(roleid, currenttime, starttime, endtime, viplevel, recenttotalcash);
	}
};

};

#endif
