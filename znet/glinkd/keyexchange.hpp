
#ifndef __GNET_KEYEXCHANGE_HPP
#define __GNET_KEYEXCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "linelist.hpp"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "userlogin.hrp"

namespace GNET
{

class KeyExchange : public GNET::Protocol
{
	#include "keyexchange"

private:
	Octets& GenerateKey(Octets &identity, Octets &password, Octets &nonce, Octets &key)
	{
		HMAC_MD5Hash hash;
		hash.SetParameter(identity);
		hash.Update(password);
		hash.Update(nonce);
		return hash.Final(key);
	}

public:
	void Setup(Octets &identity, Octets &password, Manager *manager, Manager::Session::ID sid)
	{
		Security *random = Security::Create(RANDOM);
		random->Update(nonce.resize(16));
		random->Destroy();

		Octets key;
		GLinkServer::GetInstance()->SetSaveISecurity(sid, ARCFOURSECURITY, GenerateKey(identity, password, nonce, key));
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer *lsm = (GLinkServer *)manager;
		Octets key;
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (!sinfo)
			return;
		lsm->SetSaveOSecurity(sid,COMPRESSARCFOURSECURITY,GenerateKey(sinfo->identity,sinfo->response,nonce,key));
		int client_ip = ((struct sockaddr_in*)(sinfo->GetPeer()))->sin_addr.s_addr;
		
		Protocol *rpc = MakeUserLogin(lsm->GetAuthVersion(), sinfo->userid, sinfo->identity, blkickuser, client_ip, sinfo->elec_num, sid); 

		LineList linelist;
		if( !GProviderServer::GetInstance()->GetLineList(linelist) )
		{
			lsm->SessionError(sid, ERR_NO_LINE_AVALIABLE, "Server network error.");
			return;
		} 
		linelist.algorithm = sinfo->algorithm;
		lsm->Send(sid, linelist);
		if ( GDeliveryClient::GetInstance()->SendProtocol(rpc) )
		{
			lsm->ChangeState(sid,&state_GKeyExchgSucc);
		}
		else
			lsm->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
	}

	Protocol *MakeUserLogin(int version, int userid, const Octets &username, char blkickuser, int ip, const Octets & elec_num, Manager::Session::ID sid)
	{
		Protocol *rpc;
		GLinkServer *lsm = GLinkServer::GetInstance();
		if(1 == version)
		{
			/*
			Octets used_elec_number;
			if(SessionInfo::USE_ELECNUMBER == login_method)
			{
				lsm->GetUsedElecNumber(userid, used_elec_number);
			}
			*/
			UserLoginArg2 arg(userid, sid, blkickuser, 0, elec_num, 0, Octets(), ip, Octets(), Octets(), username);
			lsm->GetSecurityKeys(sid, arg.iseckey, arg.oseckey);
			rpc = (Protocol *)Rpc::Call(RPC_USERLOGIN2, arg);
			((UserLogin2 *)rpc)->localsid = sid;
		}
		else
		{
			UserLoginArg arg(userid, sid, blkickuser, 0, ip, Octets(), Octets(), username);
			lsm->GetSecurityKeys(sid, arg.iseckey, arg.oseckey);
			rpc = (Protocol *)Rpc::Call(RPC_USERLOGIN, arg);
			((UserLogin *)rpc)->localsid = sid;
		}
		return rpc;
	}
};

};

#endif
