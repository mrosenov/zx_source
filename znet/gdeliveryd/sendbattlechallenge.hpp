
#ifndef __GNET_SENDBATTLECHALLENGE_HPP
#define __GNET_SENDBATTLECHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battlechallenge_re.hpp"
#include "groleinventory"
#include "gmailendsync.hpp"
#include "mapuser.h"
#include "gdeliveryserver.hpp"
#include "postoffice.h"

namespace GNET
{

class SendBattleChallenge : public GNET::Protocol
{
	#include "sendbattlechallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int retcode = 0;
		if( PostOffice::GetInstance().GetMailBoxSize(roleid) >= 32 )
			retcode = ERR_AS_MAILBOXFULL;	
		BattleChallenge_Re res(0, roleid, id, 0, 0, 0, 0);
		if(!retcode)
			retcode = BattleManager::GetInstance()->Challenge(*this, res, sid);
		if(retcode!=ERR_SUCCESS)
		{
			unsigned int linkid;
			res.retcode = retcode;
			syncdata.inventory.items.clear();
			manager->Send(sid, GMailEndSync(0,retcode,roleid,syncdata));
			BattleManager::GetInstance()->SendPlayer(roleid, res, res.localsid, linkid);
		}
	}
};

};

#endif
