
#ifndef __GNET_GETCOMMONDATA_HPP
#define __GNET_GETCOMMONDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCommonData : public GNET::Protocol
{
	#include "getcommondata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (key != 10052)
			return;
		CommonDataQuery *rpc = (CommonDataQuery *)Rpc::Call(RPC_COMMONDATAQUERY, CommonDataQueryArg(key));
		rpc->localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
