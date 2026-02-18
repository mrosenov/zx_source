
#ifndef __GNET_ROLELIST_RE_HPP
#define __GNET_ROLELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "groleinventory"
#include "roleinfo"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "statusannounce.hpp"

namespace GNET
{

class RoleList_Re : public GNET::Protocol
{
	#include "rolelist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (!lsm->ValidUser(localsid,userid))
		{
			GDeliveryClient::GetInstance()->SendProtocol(StatusAnnounce(userid,localsid,_STATUS_OFFLINE));
			return;
		}
		//change state
		if (handle==_HANDLE_END)
		{
			lsm->ChangeState(localsid,&state_GSelectRoleServer);
			lsm->SetAliveTime(localsid, _CLIENT_TTL);
			//小退时更新各线状态信息
			SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
			LineList linelist;
			if (GProviderServer::GetInstance()->GetLineList(linelist) && sinfo!=NULL)
			{
				linelist.algorithm = sinfo->algorithm;
				lsm->Send(localsid, linelist);
				LOG_TRACE("Send LineList to Client sid=%d userid=%d", localsid, userid);
			}
		}
		else
		{
			lsm->ChangeState(localsid,&state_GRoleList);
		}
		//send to client
		unsigned int tmpsid=localsid;
		localsid=_SID_INVALID;
		lsm->Send(tmpsid,this);
	}
};

};

#endif
