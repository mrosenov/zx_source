
#ifndef __GNET_FACTIONCHAT_HPP
#define __GNET_FACTIONCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionmanager.h"
#include "familymanager.h"
#include "base64.h"
#include "gamegateserver.hpp"
#include "gatefactionchat.hpp"

namespace GNET
{

class FactionChat : public GNET::Protocol
{
	#include "factionchat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int fid;
		src_route = FROM_GAME;

		GateFactionChat gtchat;
		gtchat.src =src;
		gtchat.channel =channel;
		gtchat.msg = msg;

	//	DEBUG_PRINT("FactionChat src=%d channel=%d",src,channel);
		switch(channel)
		{
		case GP_CHAT_FACTION: //faction
			{
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(src);
				if (NULL==pinfo || pinfo->factionid==0) 
					return;
				fid = pinfo->factionid;
				emotion = pinfo->emotion;
				gtchat.fid = fid;
			}
			GameGateServer::GetInstance()->GTSend(gtchat);

			if(FactionManager::Instance()->Broadcast(fid, *this, this->localsid))
			{
				Octets out;
				Base64Encoder::Convert(out, msg);
				Log::log(LOG_CHAT,"Guild: src=%d fid=%d msg=%.*s",src,fid,out.size(),(char*)out.begin());
			}
			break;
		default: // family
			{
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(src);
				if (NULL==pinfo || pinfo->familyid==0) 
					return;
				fid = pinfo->familyid;
				emotion = pinfo->emotion;
			}
			FamilyInfo * family = FamilyManager::Instance()->Find(fid);
			if (NULL == family)
				return;
			channel = GP_CHAT_FAMILY;

			gtchat.fid = fid;
			gtchat.channel =channel;
			GameGateServer::GetInstance()->GTSend(gtchat);
			family->Broadcast(*this, this->localsid);
			Octets out;
			Base64Encoder::Convert(out, msg);
			Log::log(LOG_CHAT,"Family: src=%d fid=%d msg=%.*s",src,fid,out.size(),(char*)out.begin());
			break;
		}
	}
};

};

#endif
