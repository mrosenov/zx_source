#include "netmarble.h"
#include "netmarblededuct.hrp"
#include "netmarblelogin.hrp"
#include "netmarblelogout.hpp"
#include "gauthclient.hpp"
#include "gdeliveryserver.hpp"
#include "timer.h"
#include "mapuser.h"

const static int UPDATE_INTERVAL = 10000000;
const static int UPDATE_INTERVAL2 = 30000000;
const static int UPDATE_TIMES_PER_MIN = 60000000/UPDATE_INTERVAL;

bool NetMarble::Initialize(int update_interval)
{
	_is_active = true;
//	IntervalTimer::Attach(this,UPDATE_INTERVAL/IntervalTimer::Resolution());
	IntervalTimer::Attach(this,UPDATE_INTERVAL2/IntervalTimer::Resolution());
	if (update_interval > DEFAULT_UPDATE_INTERVAL_PER_USER)
		_update_interval_per_user = update_interval;
	return true;
}

bool NetMarble::Update()
{
	if (!_is_active) return false;
	if (_usermap.empty()) return true;
	time_t now = Timer::GetTime();
	NetMarbleInfoMap::iterator it, ite = _usermap.end();
	for (it = _usermap.begin(); it != ite; ++it)
	{
		if (now - it->second.update_time >= _update_interval_per_user)
		{
			LOG_TRACE("NetMarble user %d now (%s)", it->first, ctime(&now));
			NetMarbleDeduct *rpc = (NetMarbleDeduct *)Rpc::Call(RPC_NETMARBLEDEDUCT, NetMarbleDeductArg(it->first,it->second.ip,(int)now-it->second.last_deduct_time));
			GAuthClient::GetInstance()->SendProtocol(rpc);
			it->second.update_time = now;
		}
	}

/*
	int count = (_usermap.size()+UPDATE_TIMES_PER_MIN-1)/UPDATE_TIMES_PER_MIN;
	if (count<=0) return true;

	NetMarbleInfoMap::iterator it = _usermap.lower_bound(_cursor);
	if (it == _usermap.end()) it = _usermap.begin();
	_cursor = it->first;

	int i = 0; 
	time_t now = Timer::GetTime();
	do
	{
		if (now - it->second.update_time < _update_interval_per_user)
			continue;
		NetMarbleDeduct *rpc = (NetMarbleDeduct *)Rpc::Call(RPC_NETMARBLEDEDUCT, NetMarbleDeductArg(it->first,it->second.ip,(int)now-it->second.last_deduct_time));
		GAuthClient::GetInstance()->SendProtocol(rpc);
		it->second.update_time = now;
		++i, ++it;
		if (it == _usermap.end()) it = _usermap.begin();
	} while(i<count && it->first!=_cursor);

	if (it == _usermap.end()) 
		_cursor = 0;
	else
		_cursor = it->first;
*/
	return true;
}

void NetMarble::OnLogin(int userid, int ip)
{
	if (!_is_active) return;
	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	NetMarbleLogin *rpc = (NetMarbleLogin *)Rpc::Call(RPC_NETMARBLELOGIN, NetMarbleLoginArg(userid, ip, dsm->aid, dsm->zoneid));
	GAuthClient::GetInstance()->SendProtocol(rpc);
}

void NetMarble::OnLogout(UserInfo * user)
{
	Conf *conf = Conf::GetInstance();
	std::string netmarble = conf->find(GDeliveryServer::GetInstance()->Identification(), "korea_netmarble");
	if (netmarble == "true")
	{
		//韩国网吧系统在enterworld的时候会发NetbarAnnounce 国内网吧系统只在UserLogin的时候才发
		user->netbar_level = user->award_type = -1;
		LOG_TRACE("Korea User %d ->Role %d logout clear NetBarReward", user->userid, user->roleid);
	}
	if (!_is_active) return;
	NetMarbleInfoMap::iterator it = _usermap.find(user->userid);
	if (it != _usermap.end())
	{
		GAuthClient::GetInstance()->SendProtocol(NetMarbleLogout(user->userid, it->second.ip, (int)Timer::GetTime()-it->second.last_deduct_time));
		_usermap.erase(it);
	}
}

void NetMarble::OnNetMarbleLogin(int retcode, int userid, int ip, int deducttype)
{
	if (!_is_active) return;
	if (retcode==ERR_SUCCESS && deducttype)
	{
		time_t now = Timer::GetTime();
		NetMarbleInfo &info = _usermap[userid];
		info.ip = ip;
		info.last_deduct_time = now;
		info.update_time = now;
	}
}

void NetMarble::OnNetMarbleDeduct(int retcode, int userid, int deduct_time)
{
	if (!_is_active) return;
	if (retcode == ERR_SUCCESS)
	{
		NetMarbleInfoMap::iterator it = _usermap.find(userid);
		if (it != _usermap.end())
		{
			it->second.last_deduct_time += deduct_time;
		}
	}
}
