
#ifndef __GNET_ENTERWORLD_HPP
#define __GNET_ENTERWORLD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gproviderclient.hpp"
#include "s2cgamedatasend.hpp"	
#include "playerlogout.hpp"

#ifdef _TESTCODE
	#include "switchserverstart.hpp"
#endif
void user_enter_world(int cs_index,int sid,int uid, int locktime, int maxlocktime);
namespace GNET
{
#ifdef _TESTCODE
struct INFO
{
	int roleid;
	unsigned int localsid;
	INFO(int _r,unsigned int _l) : roleid(_r),localsid(_l) { }
};
void* _Sender(void* pInfo)
{
	INFO info=*((INFO*)pInfo);
	free(pInfo);
	printf("gamed::enterworld: enter sending-thread. roleid=%d,localsid=%d\n",info.roleid,info.localsid);
	S2CGamedataSend s2c(info.roleid,info.localsid,Octets("Hello",5));
	while(1)
	{
		GProviderClient::GetInstance()->DispatchProtocol(1,s2c);
		usleep(10000);
		PollIO::WakeUp();
	}
	return NULL;
}
class TestPlayerLogout : public Thread::Runnable
{
	int roleid;
	int provider_linkid;
	unsigned int localsid;
public:
	TestPlayerLogout(int _r,int _p,unsigned int _l,int prior = 1) : Runnable(prior),roleid(_r),provider_linkid(_p),localsid(_l) { }
	void Run()
	{
		DEBUG_PRINT("gamed::Send playlogout to gdelivery,retcode=_PLAYER_LOGOUT_HALF\n");
		GProviderClient::DispatchProtocol(0,PlayerLogout(_PLAYER_LOGOUT_HALF,roleid,provider_linkid,localsid));
		PollIO::WakeUp();
		delete this;
	}
};	

#endif
class EnterWorld : public GNET::Protocol
{

	#include "enterworld"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		user_enter_world(provider_link_id,localsid,roleid,timeout,locktime);	
	}
};

};

#endif
