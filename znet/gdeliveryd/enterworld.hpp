
#ifndef __GNET_ENTERWORLD_HPP
#define __GNET_ENTERWORLD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"

#include "gamedbclient.hpp"
//#include "rolelogin.hpp"

#include "gproviderserver.hpp"
#include "playerstatusannounce.hpp"
#include "acstatusannounce.hpp"
#include "ganticheatclient.hpp"
#include "mapuser.h"
#include "familymanager.h"
#include "syncfamilydata.hpp"
#include "addictioncontrol.hpp"
#include "contestmanager.h"
#include "sectupdate.hpp"
#include "snsplayerinfomanager.h"
#include "referencemanager.h"
#include "fungamemanager.h"
#include "hometownmanager.h"
#include "netmarble.h"
#include "vipinfocache.hpp"
#include "battleflagmanager.hpp"
#include "discountannounce.hpp"

namespace GNET
{

class EnterWorld : public GNET::Protocol
{
	#include "enterworld"
	void AnnouncePlayerOnline(UserInfo& ui)
	{
		ACStatusAnnounce2 acsa;
		acsa.status=_STATUS_ONGAME;
		acsa.info_list.push_back( ACOnlineStatus2(ui.roleid, ui.userid, ui.ip) );
		GAntiCheatClient::GetInstance()->SendProtocol(acsa);
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (NULL==pinfo) 
			return;
		if (pinfo->linksid!=sid || pinfo->localsid!=localsid )
			return;
		UserInfo* user = pinfo->user;
		if(user->status!=_STATUS_READYGAME)
			return;
		user->status=_STATUS_ONGAME;
		pinfo->ingame = true;
		user->GetLocktime(locktime, timeout);
	
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,this);

		AnnouncePlayerOnline(*user);
		if(pinfo->familyid)
		{
			SyncFamilyData data;
			data.roleid = roleid;
			data.familyid = pinfo->familyid;
			if(FamilyManager::Instance()->SyncGameOnLogin(pinfo->familyid, data.skills, pinfo->linksid, pinfo->localsid))
			{
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);
			}
		}
		if(pinfo->sectid)
		{
			SectUpdate data;
			data.sectid = pinfo->sectid;
			data.roleid = roleid;
			if(SectManager::Instance()->GetSkill(pinfo->sectid,data.skills))
			{
				GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, data );
				LOG_TRACE("send SectSkill size=%d", data.skills.size());
			}
		}
		if(user->actime>0 && user->acstate)
		{
			for(std::vector<GPair>::iterator it=user->acstate->data.begin(),ie=user->acstate->data.end();it!=ie;++it)
			{
				if(it->key==1)
					it->value += Timer::GetTime() - user->actime;
			}
			user->acstate->userid = pinfo->roleid;
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,user->acstate);
			user->actime = Timer::GetTime();
		}
		
		ContestManager::GetInstance().OnPlayerLogin(pinfo->roleid, pinfo->linksid, pinfo->localsid);
		SNSPlayerInfoManager::GetInstance()->OnPlayerLogin(pinfo->roleid);
		ReferenceManager::GetInstance()->OnLogin(pinfo->roleid);
		FunGameManager::GetInstance().OnPlayerLogin(pinfo->roleid, pinfo->linksid, pinfo->localsid, pinfo->gameid);
		HometownManager::GetInstance()->OnPlayerLogin(pinfo->roleid);
		NetMarble::GetInstance()->OnLogin(user->userid, user->ip);
		VIPInfoCache::GetInstance()->SendVIPInfoToGS(user->userid, pinfo->roleid, pinfo->gameid);

		BattleFlagKey bfkey;
		BattleFlagManager::MakeFactionID(bfkey, pinfo->roleid, pinfo->factionid, pinfo->gameid, pinfo->world_tag);
		BattleFlagManager::GetInstance()->QueryBattleFlagBuff(bfkey);
		
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();
		if (dsm->fastpay_discounts.size())
			dsm->Send(pinfo->linksid, DiscountAnnounce(dsm->fastpay_discounts, pinfo->localsid));
		if (user->merchantlist.size())
			dsm->Send(pinfo->linksid, FastPayBindInfo(user->userid, user->merchantlist, pinfo->localsid));
	}
};

};

#endif
