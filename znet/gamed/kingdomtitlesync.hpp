
#ifndef __GNET_KINGDOMTITLESYNC_HPP
#define __GNET_KINGDOMTITLESYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void kingdom_title_sync(int roleid, char title);

namespace GNET
{

class KingdomTitleSync : public GNET::Protocol
{
	#include "kingdomtitlesync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		kingdom_title_sync(roleid, self_title);
	}
};

};

#endif
