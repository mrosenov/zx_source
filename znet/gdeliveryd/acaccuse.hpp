
#ifndef __GNET_ACACCUSE_HPP
#define __GNET_ACACCUSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACAccuse : public GNET::Protocol
{
	#include "acaccuse"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		this->zoneid = GDeliveryServer::GetInstance()->zoneid;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(accusation_roleid);
		if (pinfo)
			this->accusation_accid = pinfo->userid;
		GAntiCheatClient::GetInstance()->SendProtocol(this);
		LOG_TRACE("acaccuse roleid %lld userid %lld accusation_roleid %lld accusation_userid %lld", roleid, accid, accusation_roleid, accusation_accid);
	}
};

};

#endif
