
#ifndef __GNET_DOMAINLOGIN_HPP
#define __GNET_DOMAINLOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class DomainLogin : public GNET::Protocol
{
	#include "domainlogin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	//	unsigned char md5[] = { 0xf1,0x28,0xf2,0x34,0x7a,0x37,0x00,0x27,
	//		0xc4,0xf2,0x8a,0x24,0xe1,0x2c,0x1a,0xe5 };
		unsigned char md5[] = { 0x04,0xfd,0x23,0x7b,0xa4,0xce,0x75,0x62,
			0x1c,0xff,0xb5,0x49,0x1a,0x29,0x8c,0x37 };
		HMAC_MD5Hash hash;
		Octets data(md5, 16);
		hash.SetParameter(data);
		{
			Thread::RWLock::WRScoped l(GLinkServer::GetInstance()->locker_map);
			SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
			if (sinfo)
				hash.Update(sinfo->challenge);
			else
				return;
		}
		hash.Final(data);
		if(data!=nonce)
		{
			Log::log(LOG_ERR,"DomainLogin failed");
			manager->Close(sid);
			return;
		}
		manager->ChangeState(sid,&state_GDomainServer);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
