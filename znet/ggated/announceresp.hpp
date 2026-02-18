
#ifndef __GNET_ANNOUNCERESP_HPP
#define __GNET_ANNOUNCERESP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformclient.hpp"

namespace GNET
{

class AnnounceResp : public GNET::Protocol
{
	#include "announceresp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("AnnounceResp boottime=%lld code=%d",boottime,code);
		GTPlatformClient::GetInstance()->OnInitMsgResp(boottime,code);
	}
};

};

#endif
