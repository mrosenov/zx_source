
#ifndef __GNET_POST_RE_HPP
#define __GNET_POST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "timeinfo"
#include "dbconsignskipsn.hpp"
namespace GNET
{

class Post_Re : public GNET::Protocol
{
	#include "post_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("post_re: receive. retcode=%d roleid=%lld,sn=%lld showperiod=%d postperiod=%d shelfperiod=%d,timestamp=%lld\n",retcode,roleid,sn,time.showperiod,time.postperiod,time.sellperiod,timestamp);
		ConsignManager& cmanager = *ConsignManager::GetInstance();		
		switch(retcode)
		{
			case 0:
			case 1:
				cmanager.RecvPostRe(true,userid, sn, time.postperiod, time.showperiod, time.sellperiod,commodity_id);
				break;
			case 6:
				GameDBClient::GetInstance()->SendProtocol(DBConsignSkipSN(100));//sn conflict, add 100 to sn
			case 2:
			case 5:
			case 7:
			case 8:
			case 9:
				cmanager.RecvPostRe(false,userid, sn,0,0,0,0);
				break;
			case 3:	
			case 4:
			case -1:
			default:
				//auto resend post
				break;
		}
	}
};

};

#endif
