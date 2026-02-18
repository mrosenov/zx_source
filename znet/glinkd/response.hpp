
#ifndef __GNET_RESPONSE_HPP
#define __GNET_RESPONSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "matrixpasswd.hrp"
#include "matrixtoken.hrp"
namespace GNET
{


class Response : public GNET::Protocol
{
	#include "response"

	void Decode(Octets& challenge,Octets& response)
	{
		char* ptr1 = (char*)response.begin();
		char* ptr2 = (char*)challenge.begin();
		size_t len = response.size();
		for(size_t i=0,j=0;i<len;i++,j++)
		{
			if(j>=challenge.size())
			{
				j = 0;  
				ptr2 = (char*)challenge.begin();
			}
			*ptr1 ^= *ptr2;
			ptr1++;
			ptr2++;
		}
	}

	Protocol *MakeMatrixPasswd(int version, Octets &identity, Octets &response, int ip, Manager::Session::ID sid)
	{
		Protocol *rpc;
		if(1 == version) 
		{
			rpc = (Protocol *)Rpc::Call(RPC_MATRIXPASSWD2, MatrixPasswdArg2(identity, response, ip));
			((MatrixPasswd2 *)rpc)->save_sid = sid;
		}
		else
		{
			rpc = (Protocol *)Rpc::Call(RPC_MATRIXPASSWD, MatrixPasswdArg(identity, response, ip));
			((MatrixPasswd *)rpc)->save_sid = sid;
		}
		return rpc;
	}

	/*bool MidValid(Octets& mid)
	{
		char* ptr = (char*)mid.begin();
		size_t len = mid.size();

		for(size_t i=0; i<len; ++i)
		{
			if(0 != isdigit(ptr[i]) || 0 != isalpha(ptr[i]))
			{
				continue;
			}
			else
			{
				return false;
			}
		}

		return true;
	}*/

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::trace("glinkd::receive response from client. identity=%.*s, use_token=%d, state=%.*s, mid=%.*s, mid.size=%d", identity.size(), (char *)identity.begin(), use_token, state.size(), (char *)state.begin(), mid.size(), (char *)mid.begin(), mid.size());

		GLinkServer *lsm = (GLinkServer *)manager;
		lsm->TriggerListen(sid, false);

		if (!lsm->ValidSid(sid) || !identity.size())
			return;
		if (lsm->ExceedUserLimit(lsm->roleinfomap.size()))
		{
			lsm->SessionError(sid, ERR_SERVEROVERLOAD, "Server error.");
			return;
		}

		SessionInfo *sinfo = lsm->GetSessionInfo(sid);
		if(!sinfo)
			return;
		int client_ip = ((struct sockaddr_in *)(sinfo->GetPeer()))->sin_addr.s_addr;
		sinfo->response = response;
		sinfo->identity = identity;
		
		if(mid.size() <= 0 || mid.size() > 10)
		{
			sinfo->mid = Octets("wanmei", 6);
		}
		else
			sinfo->mid = mid;

		Protocol *rpc;
		if(use_token != 0) 	//1:北美core平台登录 2:韩国网页cookies登录
		{
			//Decode(sinfo->challenge, response);
			Log::trace("glinkd::send matrixtoken, token=%.*s", response.size(), (char *)response.begin());
			rpc = (Protocol *)Rpc::Call(RPC_MATRIXTOKEN, MatrixTokenArg(identity, response, client_ip, sinfo->challenge));
			((MatrixToken *)rpc)->save_sid = sid;
		}
		else 	// use password for authentication
		{
			int algo = lsm->challenge_algo;
			int version = lsm->GetAuthVersion();
			Log::trace("glinkd::send matrixpasswd, algo=%d, version=%d", algo, version);
			if(1 == version) // 国内版本AU，采用MatrixPasswd2协议
			{
				if(ALGO_MD5 != algo)
				{
					lsm->SessionError(sid, ERR_GENERAL, "Server error.");
					return;
				}
				rpc = MakeMatrixPasswd(version, identity, sinfo->challenge, client_ip, sid);
			}
			else // 国外版本AU
			{
				if(ALGO_PLAINTEXT == algo)
				{
					Decode(sinfo->challenge, response);
					Log::trace("glinkd::ALGO_PLAINTEXT, passwd=%.*s", response.size(), (char *)response.begin());
					rpc = MakeMatrixPasswd(version, identity, response, client_ip, sid);
				}
				else
				{
					rpc = MakeMatrixPasswd(version, identity, sinfo->challenge, client_ip, sid);
				}
			}
		}

		if(!GDeliveryClient::GetInstance()->SendProtocol(rpc))
		{
			lsm->SessionError(sid, ERR_COMMUNICATION, "Server Network Error.");
		}
		else
		{
			lsm->ChangeState(sid, &state_GResponseReceive);
		}
	}
	
};

};

#endif
