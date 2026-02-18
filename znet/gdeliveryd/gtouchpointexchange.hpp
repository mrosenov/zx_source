
#ifndef __GNET_GTOUCHPOINTEXCHANGE_HPP
#define __GNET_GTOUCHPOINTEXCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTouchPointExchange : public GNET::Protocol
{
	#include "gtouchpointexchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gtouchpointexchange roleid %d flag %d count %d points_need %d",
				roleid, flag, count, points_need);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo || pinfo->user == NULL)
			return;
		TouchPointExchange_Re client_re(-1, roleid, flag, count, 0, pinfo->localsid);
		GTouchPointExchange_Re gs_re(-1, roleid, flag, count);
		if (!GAuthClient::GetInstance()->IsConnAvailable() || !GameDBClient::GetInstance()->IsConnect())
		{
			client_re.retcode = gs_re.retcode = ERR_TOUCH_COMMUNICATION;
			manager->Send(sid, gs_re);
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, client_re);
			return;
		}
		if (pinfo->user->touchpoint < points_need)
		{
			client_re.retcode = gs_re.retcode = ERR_TOUCH_POINT_NOTENOUGH;
			manager->Send(sid, gs_re);
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, client_re);
			return;
		}
		DBGetTouchOrderArg arg(roleid);
		DBGetTouchOrder * rpc = (DBGetTouchOrder *)Rpc::Call(RPC_DBGETTOUCHORDER, arg);
		rpc->flag = flag;
		rpc->count = count;
		rpc->points_need = points_need;
		rpc->linksid = pinfo->linksid;
		rpc->localsid = pinfo->localsid;
		rpc->gssid = sid;
		rpc->userid = pinfo->userid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
