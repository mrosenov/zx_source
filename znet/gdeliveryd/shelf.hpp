
#ifndef __GNET_SHELF_HPP
#define __GNET_SHELF_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "timeinfo"

namespace GNET
{

class Shelf : public GNET::Protocol
{
	#include "shelf"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("Consign shelf: receive. roleid=%lld,sn=%lld,price=%d,actiontime=%lld,showeriod=%d,sellperiod=%d,buyerroleid=%d",roleid,sn,price,time.actiontime,time.showperiod,time.sellperiod,buyerroleid);
		int retcode = ConsignManager::GetInstance()->DoShelf(userid,roleid,sn,price,time.actiontime,time.showperiod,\
				time.sellperiod,buyerroleid,messageid,timestamp);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("Consign shelf err:  roleid=%lld,sn=%lld,retcode=%d\n",roleid,sn,retcode);
			Shelf_Re re;
			re.userid = re.userid;
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
