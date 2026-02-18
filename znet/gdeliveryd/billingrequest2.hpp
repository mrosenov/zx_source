
#ifndef __GNET_BILLINGREQUEST2_HPP
#define __GNET_BILLINGREQUEST2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "billingagent.h"

namespace GNET
{

class BillingRequest2 : public GNET::Protocol
{
	#include "billingrequest2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!BillingAgent::Instance().Itemid2Menuid(itemid, itemnum, timeout, menuid, agtxno)) 
			return;
		PlayerInfo * role = UserContainer::GetInstance().FindRoleOnline(userid);
		if(!role)
			return;
		userid = role->userid;
		BillingAgent::Instance().SendToAu(*this);
		DEBUG_PRINT("BillingRequest, userid=%d,request=%d,itemid=%d,amount=%d,menuid=%.*s,bxtxno=%.*s", 
				userid, request, itemid, amount, menuid.size(), (char*)menuid.begin(), bxtxno.size(), 
				(char*)bxtxno.begin());
	}
};

};

#endif
