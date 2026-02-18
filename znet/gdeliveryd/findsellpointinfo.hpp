
#ifndef __GNET_FINDSELLPOINTINFO_HPP
#define __GNET_FINDSELLPOINTINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "cardmarket.h"
#include "findsellpointinfo_re.hpp"
namespace GNET
{

class FindSellPointInfo : public GNET::Protocol
{
	#include "findsellpointinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		FindSellPointInfo_Re fspi_re;
		fspi_re.localsid=localsid;
		CardMarket::GetInstance().FindSell( startid,fspi_re.list,!!forward );
		manager->Send(sid,fspi_re);	
	}
};

};

#endif
