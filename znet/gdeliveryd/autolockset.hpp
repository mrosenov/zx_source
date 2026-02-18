
#ifndef __GNET_AUTOLOCKSET_HPP
#define __GNET_AUTOLOCKSET_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbautolockset.hrp"
#include "autolockset_re.hpp"


namespace GNET
{

class AutolockSet : public GNET::Protocol
{
	#include "autolockset"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(NULL==pinfo) 
			return;
		UserInfo* user = pinfo->user;
		if(user->SetLocktime(timeout)==0)
		{
			DEBUG_PRINT("gdelivery::autolock: set locktime=%d,roleid=%d\n",timeout,roleid);
			DBAutolockSetArg arg;
			arg.userid = pinfo->userid;
			arg.autolock = user->autolock.GetList();
			DBAutolockSet * rpc = (DBAutolockSet*) Rpc::Call( RPC_DBAUTOLOCKSET,arg);
			rpc->timeout = timeout;
			rpc->save_sid = sid;
			rpc->save_localsid = localsid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
			manager->Send(sid,AutolockSet_Re(1,0,localsid));
	}
};

};

#endif
