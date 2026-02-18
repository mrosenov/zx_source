
#ifndef __GNET_GETKDCTOKEN_HPP
#define __GNET_GETKDCTOKEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetKDCToken : public GNET::Protocol
{
	#include "getkdctoken"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(GET_KDC_TOKEN) || sinfo->roleid != roleid/*等同于 ValidRole 验证*/)
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
