
#ifndef __GNET_SENDASYNCDATA_HPP
#define __GNET_SENDASYNCDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GMSV
{
	void RecvTaskAsyncData(int roleid, int result, const void * buf, size_t size);
}

namespace GNET
{

class SendAsyncData : public GNET::Protocol
{
	#include "sendasyncdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		 GMSV::RecvTaskAsyncData(roleid, 0, data.begin(), data.size());
	}
};

};

#endif
