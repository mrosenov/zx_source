
#ifndef __GNET_OPENBANQUETJOIN_RE_HPP
#define __GNET_OPENBANQUETJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetJoin_Re : public GNET::Protocol
{
	#include "openbanquetjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetJoin_Re count=%d, roleid=%d", queue_number, roleid);
		PlayerInfo *info = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(info != NULL)
		{
			localsid = info->localsid;
			GDeliveryServer::GetInstance()->Send(info->linksid, this);
		}
	}
};

};

#endif
