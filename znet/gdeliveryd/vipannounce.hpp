#ifndef __GNET_VIPANNOUNCE_HPP
#define __GNET_VIPANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
#include "vipinfocache.hpp"
#include "gdeliveryserver.hpp"

namespace GNET
{

class VIPAnnounce : public GNET::Protocol
{
	#include "vipannounce"

	bool IsLegalMsg()
	{
		return ((viplevel >= 1) && (viplevel <= 6) && (starttime <= endtime)) ? true : false;
	}

	// VIPAnnounce可能在如下三种情况下收到
	// 1、玩家登录成功后（由AU推送）
	// 2、玩家VIP等级上升时（由AU推送，此时不一定在线，不在则不处理）
	// 3、在线VIP玩家到期时（由delivery主动跟AU请求）
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		VIPInfoCache *vipcache = VIPInfoCache::GetInstance();
		LOG_TRACE("VIPAnnounce:userid=%d,currenttime=%d,starttime=%d,endtime=%d,viplevel=%d:vipnum=%d:recenttotalcash=%d", 
			   userid, currenttime, starttime, endtime, viplevel, vipcache->GetOnlineVIPNum(), recenttotalcash);
		
		if(!IsLegalMsg())
		      return;

		UserInfo *puser = UserContainer::GetInstance().FindUser(userid);
		if(puser == NULL)
		      return;

		vipcache->UpdateVIPInfo(userid, currenttime, starttime, endtime, viplevel, status, totalcash, recenttotalcash, infolack);
		
		// 如果在本服并且已经在游戏中则直接转发，否则对于玩家在本服的等到EnterWorld时才转发
		if(puser->status == _STATUS_ONGAME)
		{
			vipcache->SendVIPInfoToGS(userid, puser->roleid, puser->gameid);
		}
		// 如果角色已经在跨服则需要转发给跨服delivery，如果正在跨服中则将在回复RemoteLoginQuery_Re时转发
		else if(puser->status == _STATUS_REMOTE_LOGIN && !GDeliveryServer::GetInstance()->IsCentralDS())
		{
			vipcache->SendVIPInfoToCentralDS(userid);
		}
	}
};

};

#endif
