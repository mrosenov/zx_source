
#ifndef __GNET_SHELFCANCEL_HPP
#define __GNET_SHELFCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ShelfCancel : public GNET::Protocol
{
	#include "shelfcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("Consign shelfcancel: receive. roleid=%lld,sn=%lld\n",roleid,sn);

		int retcode = ConsignManager::GetInstance()->DoShelfCancel(userid,roleid,sn,messageid,timestamp);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("Consign shelfcancel err:  roleid=%lld,sn=%lld,retcode=%d\n",roleid,sn,retcode);
			ShelfCancel_Re re;
			re.userid = userid;
			re.roleid = roleid;
			re.sn = sn;
			re.messageid = messageid;
			re.retcode = ConsignManager::GetWebRqstRetcode(retcode);
			if(retcode==ERR_WT_ENTRY_NOT_FOUND)
				re.retcode =ConsignManager::GetInstance()->SN_notfound(sn);
			
			GWebTradeClient::GetInstance()->SendProtocol(re);
		}
	}
};

};

#endif
