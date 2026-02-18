
#ifndef __GNET_WEBPOSTCANCEL_HPP
#define __GNET_WEBPOSTCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "webpostcancel_re.hpp"

#include "consignmanager.h"
#include "gwebtradeclient.hpp"

namespace GNET
{

class WebPostCancel : public GNET::Protocol
{
	#include "webpostcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("Consign webpostcancel: roleid=%lld,sn=%lld,ctype=%d\n",roleid,sn,ctype);
		int retcode = ConsignManager::GetInstance()->DoWebPostCancel(userid,roleid,sn,ctype,messageid,timestamp);
		if(retcode != ERR_SUCCESS)
		{
			DEBUG_PRINT("Consign webpostcancel err:  roleid=%lld,sn=%lld,retcode=%d\n",roleid,sn,retcode);
			WebPostCancel_Re re;
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
