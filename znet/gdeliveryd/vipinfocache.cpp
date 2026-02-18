#include "vipinfocache.hpp"
#include "vipinfonotify.hpp"
#include "vipannounce.hpp"
#include "gproviderserver.hpp"
#include "centraldeliveryclient.hpp"
#include "game2au.hpp"
#include "gauthclient.hpp"
#include "protocolexecutor.h"

namespace GNET
{

bool VIPInfoCache::_isopen = false;

void VIPInfoCache::Initialize()
{
	_now = Timer::GetTime();
	SetTimeBase();

	IntervalTimer::Attach(this, UPDATE_INTERVAL*SECOND_TO_MICROSECOND / IntervalTimer::Resolution());

	_isopen = true;
}

void VIPInfoCache::UpdateVIPInfo(int userid, int currenttime, int starttime, int endtime, int viplevel, int status, int totalcash, int recenttotalcash, int infolack)
{
	if(!_isopen)
	      return;

	VIPInfo &vipinfo = _vipinfos[userid];
	//vipinfo.deltatime = Timer::GetTime() - currenttime;
	vipinfo.deltatime = 0;
	vipinfo.currenttime = currenttime;
	vipinfo.starttime = starttime;
	vipinfo.endtime = endtime;
	vipinfo.viplevel = viplevel;
	vipinfo.status = status;
	vipinfo.totalcash = totalcash;
	vipinfo.recenttotalcash = recenttotalcash;
	vipinfo.infolack = infolack;
}

void VIPInfoCache::SendVIPInfoToGS(int userid, int roleid, int gsid) const
{
	if(!_isopen)
	      return;

	if(gsid == 0 || gsid == _GAMESERVER_ID_INVALID || roleid == 0)
	      return;

	VIPInfoMap::const_iterator it = _vipinfos.find(userid);
	if(it != _vipinfos.end())
	{
		const VIPInfo &info = it->second;
		VIPInfoNotify notify(roleid, info.currenttime, info.starttime, info.endtime, info.viplevel, info.status, info.totalcash, info.recenttotalcash, info.infolack);
		bool is_succ = GProviderServer::GetInstance()->DispatchProtocol(gsid, notify);
		LOG_TRACE("VIPInfoNotify::userid=%d:roleid=%d:currenttime=%d:starttime=%d:endtime=%d:viplevel=%d:recenttotalcash=%d:gsid=%d:is_succ=%d",
			   userid, roleid, info.currenttime, info.starttime, info.endtime, info.viplevel, info.recenttotalcash, gsid, is_succ);
	}
}

void VIPInfoCache::SendVIPInfoToCentralDS(int userid) const
{
	if(!_isopen)
	      return;

	VIPInfoMap::const_iterator it = _vipinfos.find(userid);
	if(it != _vipinfos.end())
	{
		const VIPInfo &info = it->second;
		VIPAnnounce announce(userid, 0, 0, info.currenttime, info.starttime, info.endtime, info.status, info.viplevel, info.totalcash, info.recenttotalcash, info.infolack);
		bool is_succ = CentralDeliveryClient::GetInstance()->SendProtocol(announce);
		LOG_TRACE("VIPAnnounce::userid=%d:currenttime=%d:starttime=%d:endtime=%d:viplevel=%d:recenttotalcash=%d:send_to_centralds:is_succ=%d",
			   userid, info.currenttime, info.starttime, info.endtime, info.viplevel, is_succ);
	}
}

bool VIPInfoCache::Update()
{
	if(!_isopen)
	      return true;

	if(IsAnotherPeriod())
	{
		LOG_TRACE("VIPInfoCache::IsAnotherPeriod now=%s", ctime(&_now));
		SetTimeBase(); 
		EraseOfflineVIPInfo();
		GatherUserNeedToUpdate();
	}
	QueryVIPInfo();
	return true;
}

void VIPInfoCache::SetTimeBase()
{
	time_t now = _now;
	struct tm dt;
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 5;
	_timebase = mktime(&dt);
}

void VIPInfoCache::EraseOfflineVIPInfo()
{
	VIPInfoMap::iterator it = _vipinfos.begin(), ie = _vipinfos.end();
	for(; it!= ie; )
	{
		if(UserContainer::GetInstance().FindUser(it->first) == NULL) // 玩家已经下线，清除保存的VIP信息
		{
			LOG_TRACE("VIPInfo::EraseOfflineVIP:userid=%d", it->first);
			_vipinfos.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void VIPInfoCache::GatherUserNeedToUpdate()
{
	VIPInfoMap::iterator it = _vipinfos.begin(), ie = _vipinfos.end();
	for(; it != ie; )
	{
		const VIPInfo &vipinfo = it->second;
		if(vipinfo.endtime - _now + vipinfo.deltatime <= 0) 
		{
			LOG_TRACE("VIPInfoCache::vip_userid=%d has expired %d, endtime=%d, deltatime=%d", it->first, vipinfo.endtime-_now+vipinfo.deltatime, vipinfo.endtime, vipinfo.deltatime);
			// 缓存到期的VIP用户，重新向AU请求其新的VIP等级
			_updating_vipinfos.push_back(it->first);
			_vipinfos.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void VIPInfoCache::QueryVIPInfo()
{
	if(_updating_vipinfos.size() == 0)
	      return;

	Game2AU game2au;
	game2au.qtype = QUERY_VIPINFO;
	game2au.reserved = 0;
	game2au.info.clear();
	std::vector<int>::iterator it = _updating_vipinfos.begin(), ie = _updating_vipinfos.end();
	for(int i = 0; (it != ie && i < MAXNUM_PERUPDATE); ++i, ++it)
	{
		game2au.userid = *it;
		GAuthClient::GetInstance()->SendProtocol(game2au);
		LOG_TRACE("Game2AU::userid=%d:qtype=%d", *it, QUERY_VIPINFO);
	}
	_updating_vipinfos.erase(_updating_vipinfos.begin(), it);
	LOG_TRACE("VIPInfoCache::_updating_vipinfos.size=%d", _updating_vipinfos.size());
}

};
