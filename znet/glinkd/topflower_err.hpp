
#ifndef __GNET_TOPFLOWER_ERR_HPP
#define __GNET_TOPFLOWER_ERR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlower_Err : public GNET::Protocol
{
	#include "topflower_err"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TopFlower_Err, localsid:%d", localsid);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp,this);	

	}
};

};

#endif
