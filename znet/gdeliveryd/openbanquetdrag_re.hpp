
#ifndef __GNET_OPENBANQUETDRAG_RE_HPP
#define __GNET_OPENBANQUETDRAG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
#include "gdeliveryserver.hpp"


namespace GNET
{

class OpenBanquetDrag_Re : public GNET::Protocol
{
	#include "openbanquetdrag_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetDrag_Re roleid=%d, gs_id=%d, map_id=%d, iscross=%d", roleid, gs_id, map_id, iscross);
		PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
		if(info != NULL && info->ingame)
		{
			localsid = info->localsid;
			GDeliveryServer::GetInstance()->Send(info->linksid, this);
		}
	}
};

};

#endif
