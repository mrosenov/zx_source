
#ifndef __GNET_BILLINGREQUEST2_HPP
#define __GNET_BILLINGREQUEST2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "obj_interface.h"
#include "stocklib.h"
#include <glog.h>

bool player_billing_approved(int roleid, int itemid, int itemnum,int expire_time, int cost, int count);
namespace GNET
{

class BillingRequest2 : public GNET::Protocol
{
	#include "billingrequest2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(result)
			return;
		if(request==210)
			return;
		if(request==100)
		{
			SendBillingConfirm(userid, itemid, itemnum, timeout, amount, count,  (const char *)bxtxno.begin(), bxtxno.size());
		}
		else if(!player_billing_approved(userid, itemid, itemnum, timeout, amount, count))
		{
			SendBillingCancel(userid, itemid, itemnum, timeout, amount, count, (const char *)bxtxno.begin(), bxtxno.size());
		}
		else
		{
			GLog::formatlog("formatlog:billing:userid=%d:itemid=%d:itemnum=%d:timeout=%d:amount=%d:count=%d:bxtxno=%.*s:menuid=%.*s",
				userid, itemid, itemnum, timeout, amount, bxtxno.size(), count,(char*)bxtxno.begin(), 
				menuid.size(), (char*)menuid.begin());
		}
	}
};

};

#endif
