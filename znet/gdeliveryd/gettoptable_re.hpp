
#ifndef __GNET_GETTOPTABLE_RE_HPP
#define __GNET_GETTOPTABLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "mapuser.h"

namespace GNET
{

class GetTopTable_Re : public GNET::Protocol
{
	#include "gettoptable_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// 应客户端要求，战队搜索返回以TopTable形式，只限于从跨服返回消息
		LOG_TRACE("GetTopTable_Re roleid=%d, tableid=%d, selfrank=%d", roleid, tableid, selfrank);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL != pinfo )
		{
			GDeliveryServer * dsm = GDeliveryServer::GetInstance();
			(*this).localsid = pinfo->localsid;
			dsm->Send(pinfo->linksid, this);
		}
	}
};

};

#endif
