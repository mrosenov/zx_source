
#ifndef __GNET_ACACCUSE_RE_HPP
#define __GNET_ACACCUSE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACAccuse_Re : public GNET::Protocol
{
	#include "acaccuse_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("acaccuse_re zoneid %d roleid %lld accid %lld accusation_roleid %lld accusation_accid %lld result %d",
				zoneid, roleid, accid, accusation_roleid, accusation_accid, result);
		if (roleid == 0 || result != 2)
			return;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(int(roleid));
		if (pinfo)
		{
			zoneid = pinfo->localsid;//¸´ÓÃ zoneid ×Ö¶Î
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, this);
		}
		// else
		// {
			// Message arg;
			// arg.channel = CHANNEL_SPECIAL_TYPE;
			// arg.srcroleid = OFFLINE_MSG_ACACCUSE_REP;
			// arg.dstroleid = (int)roleid;
			// PutMessage* rpc = (PutMessage*) Rpc::Call(RPC_PUTMESSAGE, arg);
			// GameDBClient::GetInstance()->SendProtocol(rpc);
		// }
	}
};

};

#endif
