
#ifndef __GNET_KEYREESTABLISH_HPP
#define __GNET_KEYREESTABLISH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KeyReestablish : public GNET::Protocol
{
	#include "keyreestablish"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::log(LOG_DEBUG, "Recv KeyReestablish roleid %d userid %d flag %d src_zoneid %d random.size %d sid %d",
					roleid, userid, flag, src_zoneid, random.size(), sid);
		GLinkServer *lsm = (GLinkServer *)manager;
		lsm->TriggerListen(sid, false);
		if (!lsm->ValidSid(sid) || !random.size())
			return;
		if (lsm->ExceedUserLimit(lsm->roleinfomap.size()))
		{
			//客户端已经将ISecKey设置 所以此处无法给客户端成功发送协议
			//lsm->SessionError(sid, ERR_SERVEROVERLOAD, "Server error.");
			Log::log(LOG_ERR, "KeyReestablish link user count %d exceed limit roleid %d userid %d flag %d src_zoneid %d",
					lsm->roleinfomap.size(), roleid, userid, flag, src_zoneid);
			lsm->Close(sid);
			return;
		}
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if(!sinfo)
			return;
		int client_ip = ((struct sockaddr_in*)(sinfo->GetPeer()))->sin_addr.s_addr;
		PlayerIdentityMatch * rpc = (PlayerIdentityMatch *)Rpc::Call(RPC_PLAYERIDENTITYMATCH,
			PlayerIdentityMatchArg(roleid, userid, client_ip, src_zoneid, random, flag, sid));
		if (!GDeliveryClient::GetInstance()->SendProtocol(rpc))
		{
			//lsm->SessionError(sid, ERR_COMMUNICATION, "Server Network Error.");
			lsm->Close(sid);
		}
		else
		{
			//LOG_TRACE("roleid %d change to state_GResponseReceive", roleid);
			lsm->ChangeState(sid, &state_GResponseReceive);
		}
	}
};

};

#endif
