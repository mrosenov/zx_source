
#ifndef __GNET_BILLINGREQUEST_HPP
#define __GNET_BILLINGREQUEST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "billingagent.h"
#include "mapuser.h"

namespace GNET
{

class BillingRequest : public GNET::Protocol
{
	#include "billingrequest"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!BillingAgent::Instance().Menuid2Itemid(menuid, itemid, itemnum, timeout)) 
			return;
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if ( NULL!=pinfo && pinfo->status==_STATUS_ONGAME )
		{
			userid = pinfo->roleid;
			BillingAgent::Instance().SendToGs(pinfo->gameid, *this);
		}
		DEBUG_PRINT("BillingResult, userid=%d,request=%d,result=%d,amount=%d,menuid=%.*s,bxtxno=%.*s", 
				userid, request, result, amount, menuid.size(), (char*)menuid.begin(), bxtxno.size(), 
				(char*)bxtxno.begin());
	}
};

};

#endif
