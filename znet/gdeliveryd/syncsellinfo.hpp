
#ifndef __GNET_SYNCSELLINFO_HPP
#define __GNET_SYNCSELLINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "cardmarket.h"
#include "updateremaintime.hpp"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "announcesellresult.hpp"
//#include "syssendmail.hpp"
#include "putmessage.hrp"
#include "gamedbclient.hpp"
#include "conv_charset.h"

#include "worldchat.hpp"
#include "maplinkserver.h"
namespace GNET
{
class SyncSellInfo : public GNET::Protocol
{
	#include "syncsellinfo"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
