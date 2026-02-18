
#ifndef __GNET_COMMONDATASYNC_HPP
#define __GNET_COMMONDATASYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void CommonDataHandler (int session_key, int data_type, void *, size_t);

namespace GNET
{

class CommonDataSync : public GNET::Protocol
{
	#include "commondatasync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		CommonDataHandler (session_key, data_type, data.begin(), data.size());
	}
};

};

#endif
