
#ifndef __GNET_FACTIONMEMBERUPDATE_HPP
#define __GNET_FACTIONMEMBERUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformclient.hpp"
#include "gtmanager.h"

#include "factionidbean"
#include "rolebean"

namespace GNET
{

class FactionMemberUpdate : public GNET::Protocol
{
	#include "factionmemberupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionMemberUpdate size=%d operation=%d fid=%lld ftype=%d",roles.size(),operation,factionid.factionid,factionid.ftype);
		if(operation == MEMBER_ADD)
		{
			std::vector<RoleBean>::iterator it,ite=roles.end();
			for(it=roles.begin();it!=ite;++it)
			{
				it->status = GTManager::Instance()->RoleStatusQuery(it->info.roleid);
			}
		}
		GTPlatformClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
