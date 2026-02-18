#ifndef __GNET_GDELIVERYSERVER_HPP
#define __GNET_GDELIVERYSERVER_HPP

#include "protocol.h"
#include "macros.h"

#include "groleforbid"
#include "grolebase"
#include "hashstring.h"
#include <map>
#include <set>
#include <vector>
#include "serverattr.h"
#include "gshopscheme"
#include "merchantdiscount"

namespace GNET
{
class GDeliveryServer : public Protocol::Manager
{
	static GDeliveryServer instance;
	size_t accumulate_limit;
	int  debugmode;
	bool is_central_ds;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	unsigned int link_version;
	Octets gs_edition;	
	bool support_token_login;
	int auth_version;
	bool activate_kill;
	Thread::RWLock locker_ignore;
	std::set<Protocol::Type> ignore_set;
public:
	static GDeliveryServer *GetInstance() { return &instance; }
	std::string Identification() const { return "GDeliveryServer"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GDeliveryServer() : accumulate_limit(0),activate_kill(false),challenge_algo(0)
	{
		link_version = 0;
		iweb_sid = _SID_INVALID;
		is_central_ds = false;
		support_token_login = false;
		zoneid = 0;
		aid = 0;
		district_id = 0;
		gshop_scheme.sale = 1;
		gshop_scheme.discount = 0;
		auth_version = 0;
		zone_off = 0;
		isdebug = false;
	}

	int	zoneid; //game zone id
	char	aid;	//accounting area id
	int district_id;		//服务器所在区的id
	unsigned int iweb_sid ;	//sid of the webservice server
	ServerAttr serverAttr;
	char challenge_algo;
	GShopScheme gshop_scheme;		//商城打折方案
	std::string conf_file;			//全局配置文件名 gamesys.conf
	MerchantDiscountVector fastpay_discounts;
	int64_t zone_off;
	bool isdebug;
	void BroadcastStatus();
	void OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer);
	void SetDebugmode(int mode) { debugmode = mode; }
	int  GetDebugmode() { return debugmode; }
	char ChallengeAlgo() { return challenge_algo; }
	bool IsCentralDS() { return is_central_ds; }
	void SetCentralDS(bool b) { is_central_ds = b; }
	bool IsSupportTokenLogin() { return support_token_login; }
	void SetSupportTokenLogin(bool b) { support_token_login = b; }
	void SetVersion(unsigned int ver) { link_version = ver; }
	unsigned int GetVersion() { return link_version; }
	void SetEdition(const Octets & edi) { gs_edition = edi; }
	Octets GetEdition() { return gs_edition; }
	void SetAuthVersion(int version) { auth_version = version; }
	int GetAuthVersion() { return auth_version; }
	void SetActivateKill() { activate_kill = true; }
	bool IsActivateKill() const { return activate_kill; }
	bool IsDebug() { return isdebug; }
	void SetDebug(bool debug) { isdebug = debug; LOG_TRACE("GDeliveryServer setdebug=%d", debug); }

	virtual bool CheckIgnore(Protocol::Type t)
	{
		//manager 的所有 session 共用此锁 加读锁保证效率
		Thread::RWLock::RDScoped l(locker_ignore);
		//return ignore_set.find(t) != ignore_set.end();
		bool res = ignore_set.find(t) != ignore_set.end();
		if (res)
			LOG_TRACE("GDeliveryServer ignore protocol %d", t);
		return res;
	}
	void InsertIgnore(const std::set<Protocol::Type> & s)
	{
		Thread::RWLock::WRScoped l(locker_ignore);
		ignore_set.insert(s.begin(), s.end());
	}
	bool IsSameDay(int time1, int time2)
	{
		return (time1 + zone_off)/86400 == (time2 + zone_off)/86400;
	}
};

};
#endif
