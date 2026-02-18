
#ifndef __GNET_POSTEXPIRE_HPP
#define __GNET_POSTEXPIRE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "consignmanager.h"
#include "gwebtradeclient.hpp"
#include "postexpire_re.hpp"
namespace GNET
{

class PostExpire : public GNET::Protocol
{
	#include "postexpire"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("Consign postexpire: roleid=%lld,sn=%lld\n",roleid,sn);
		int retcode=ConsignManager::GetInstance()->DoPostExpire(userid,roleid,sn,messageid,timestamp);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("Consign postexpire err:  roleid=%lld,sn=%lld,retcode=%d\n",roleid,sn,retcode);
			PostExpire_Re re;
			re.userid = userid;
			re.roleid = roleid;
			re.sn = sn;
			re.messageid = messageid;
			re.retcode = ConsignManager::GetWebRqstRetcode(retcode);
			if(retcode==ERR_WT_ENTRY_NOT_FOUND)
				re.retcode =ConsignManager::GetInstance()->SN_notfound(sn);
			DEBUG_PRINT("Consign postexpire_re :  roleid=%lld,sn=%lld,retcode=%d\n",re.roleid,re.sn,re.retcode);	
			GWebTradeClient::GetInstance()->SendProtocol(re);
		}
	}
};

};

#endif
