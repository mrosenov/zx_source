
#ifndef __GNET_KINGDOMTITLESYNC_HPP
#define __GNET_KINGDOMTITLESYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomTitleSync : public GNET::Protocol
{
	#include "kingdomtitlesync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
