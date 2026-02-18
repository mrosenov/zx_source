
#ifndef __GNET_GAMEPOSTCANCEL_RE_HPP
#define __GNET_GAMEPOSTCANCEL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GamePostCancel_Re : public GNET::Protocol
{
	#include "gamepostcancel_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gamepostcancel_re:retcode=%d roleid=%lld,sn=%lld\n",retcode,roleid,sn);
		ConsignManager* cmanager = ConsignManager::GetInstance();
		switch(retcode)
		{
			case 0:
			case 1:
			case 2:
			case 5:
			case 7:
				cmanager->RecvCancelPostRe(true,userid,sn);
				break;
			case 6:
				cmanager->RecvCancelPostRe(false,userid,sn);
				break;
			case 3:
			case 4:
			case -1:
			default:
				//auto resend gamepostcancel
				break;
		}
	}
};

};

#endif
