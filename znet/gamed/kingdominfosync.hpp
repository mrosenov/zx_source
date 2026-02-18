
#ifndef __GNET_KINGDOMINFOSYNC_HPP
#define __GNET_KINGDOMINFOSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdominformation"

void kingdom_info_sync(int mafia_id, int point, std::map<int, int> & kingdom_title, int task_type);

namespace GNET
{

class KingdomInfoSync : public GNET::Protocol
{
	#include "kingdominfosync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		std::map<int,int> kingdom_title;

		for(size_t i = 0; i < info.functionaries.size(); ++i)
		{
			kingdom_title[info.functionaries[i].roleid] = info.functionaries[i].title;
		}

		kingdom_info_sync(info.kingfaction, info.points, kingdom_title, info.task_type);
	}
};

};

#endif
