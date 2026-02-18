
#ifndef __GNET_TRYCHANGEGS_HPP
#define __GNET_TRYCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "trychangegs_re.hpp"

namespace GNET
{

class TryChangeGS : public GNET::Protocol
{
	#include "trychangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("TryChangeGS roleid %d dst_lineid %d", roleid, dst_lineid);
		if (!GLinkServer::ValidRole(sid, roleid))
			return;
		this->localsid = sid;
		int check_limit_res = GProviderServer::GetInstance()->CheckLineLimit(dst_lineid);
		if( check_limit_res != 0 )	
		{
			TryChangeGS_Re re(-1, roleid, 0);
			if (check_limit_res == -1)
				re.retcode = ERR_NO_LINE_AVALIABLE;
			else if (check_limit_res == -2)
				re.retcode = ERR_INVALID_LINEID;
			else if (check_limit_res == 1)
				re.retcode = ERR_EXCEED_LINE_MAXNUM;
			manager->Send(sid, re);
			return;
		}
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
