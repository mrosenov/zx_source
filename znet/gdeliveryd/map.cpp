
#include "mapremaintime.h"
#include "mapforbid.h"
#include "maplinkserver.h"
#include "mapuser.h"
#include "mappasswd.h"
#include "mapusbkey.hpp"

#include "gdeliveryserver.hpp"
#include "onlineannounce.hpp"
#include "onlineannounce.hpp"
#include "kickoutuser.hpp"
#include "privatechat.hpp"
#include "addictioncontrol.hpp"
#include "userlogout.hrp"
#include "sectmanager.h"
#include "playerlogout.hpp"
#include "playeroffline.hpp"
#include "acstatusannounce2.hpp"
#include "ganticheatclient.hpp"
#include "contestmanager.h"
#include "instancingmanager.h"
#include "snsplayerinfomanager.h"
#include "referencemanager.h"
#include "fungamemanager.h"
#include "hometownmanager.h"
#include "territorymanager.h"
#include "kingdommanager.h"
#include "netmarble.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "disconnectplayer.hpp"
#include "openbanquetlogout.hpp"
#include "circlemanager.h"
#include "consignmanager.h"
#include "centraldeliveryserver.hpp"
#include "centraldeliveryclient.hpp"
#include "senddataandidentity.hpp"
#include "saveplayerdata.hrp"
#include "senddataandidentity_re.hpp"

#include "remoteloginquery.hpp"
#include "remoteloginquery_re.hpp"
#include "freezeplayerdata.hrp"
#include "remotelogout.hpp"
#include "kickoutremoteuser_re.hpp"
#include "kickoutremoteuser.hpp"
#include "roleid2uid.hrp"
#include "rolelist_re.hpp"
#include "getremoteroleinfo.hpp"
#include "getroleinfo.hrp"
#include "friendcallbackmanager.hpp"
#include "gtplatformagent.h"
#include "raidmanager.h"
#include "vipinfocache.hpp"
#include "mapphonetoken.h"
#include "getfriendlist.hrp"
#include "openbanquetmanager.h"
#include "dbgetserviceforbidcmd.hrp"
#include "playerreconnect.hpp"
#include "tryreconnect_re.hpp"
#include "iwebcmd.h"


namespace GNET
{

RemainTime	RemainTime::instance;
Passwd		Passwd::instance;
ForbidLogin	ForbidLogin::instance;
ForbidRoleLogin	ForbidRoleLogin::instance;
ForbidTrade	ForbidTrade::instance;
ForbidConsign 	ForbidConsign::instance;
ForbidUserTalk 	ForbidUserTalk::instance;
ForbiddenUsers	ForbiddenUsers::instance;
LinkServer	LinkServer::instance;
UserContainer	UserContainer::instance;
std::set<int> 	DelayRolelistTask::roleidset;
int UsbKeyCache::time_diff = 0;
int PhoneTokenCache::time_diff = 0;

void ForbiddenUsers::CheckTimeoutUser()
{
	int now = time(NULL);
	for(Set::const_iterator it = set.begin(), ie = set.end(); it != ie; )
	{
		if( now - it->second.addtime > MAX_REMOVETIME )
		{
			int userid = it->first, roleid = it->second.roleid, status = it->second.status;
			++it;
			set.erase(userid);
			DEBUG_PRINT("RemoveForbidden: timeout. userid=%d,roleid=%d,status=%d\n", userid , roleid, status);
			UserContainer::GetInstance().ContinueLogin(userid, true);
		}
		else
			++it;
	}
}

RemoteLoggingUsers::RemoteLoggingUsers()
{
	int timeout = atoi(Conf::GetInstance()->find(GDeliveryServer::GetInstance()->Identification(), "remote_logging_timeout").c_str());
	logging_timeout = timeout > DEFAULT_REMOTE_LOGGING_TIMEOUT ? timeout : DEFAULT_REMOTE_LOGGING_TIMEOUT;
	LOG_TRACE("RemoteLoggingUsers set timeout %d", logging_timeout);
}

void RemoteLoggingUsers::CheckTimeoutUser()
{
	int now = time(NULL);
	for (ForbiddenUsers::Set::iterator it = user_map.set.begin(), ite = user_map.set.end(); it != ite; )
	{
		if (now - it->second.addtime > logging_timeout)
		{
			int userid = it->first;
			LOG_TRACE("Remove remote logging timeout user %d roleid %d status %d", userid, it->second.roleid, it->second.status);
			UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
			if (pinfo)
			{
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, KickoutUser(userid, pinfo->localsid, ERR_TIMEOUT));
				UserContainer::GetInstance().UserLogout(pinfo);
			}
			STAT_MIN5("RemoteLoginTimeout", 1);
			user_map.set.erase(it++);
		}
		else
			++it;
	}
}

void RemoveForbidden::Run()
{
	ForbiddenUsers::GetInstance().CheckTimeoutUser();
	RemoteLoggingUsers::GetInstance().CheckTimeoutUser();
	Thread::HouseKeeper::AddTimerTask(this,MAX_REMOVETIME/2);
}

void ForbidStudioUser::Update()
{
	time_t now = Timer::GetTime();
	std::map<int, int>::const_iterator it, ite = usermap.end();
	MapEraser<std::map<int, int> > del_keys(usermap);
	for (it = usermap.begin(); it != ite; ++it)
	{
		if (now > it->second)
		{
			LOG_TRACE("Kickout studio user %d", it->first);
			UserContainer & container = UserContainer::GetInstance();
			UserInfo * pinfo = container.FindUser(it->first);
			if (pinfo)
				container.UserLogout(pinfo, KICKOUT_LOCAL, true); //pinfo失效
			del_keys.push(it->first);
		}
	}	
}

void CheckTimer::Run()
{
	static size_t _counter=0;
	if((_counter++)%3==0)
	{
		int total_online = UserContainer::GetInstance().Size();
		int remote_online = UserContainer::GetInstance().RemoteOnlineSize();
		size_t cache_size =  RoleInfoCache::Instance().Size();
		size_t faction_size =  FactionManager::Instance()->Size();
		STAT_MIN5("OnlineUsers", (total_online-remote_online>0 ? total_online-remote_online : 0));//本地在线
		STAT_MIN5("RemoteOnlineUsers", remote_online);//跨服在线
		STAT_MIN5("AllOnlineUsers", GDeliveryServer::GetInstance()->IsCentralDS() ? 0 : total_online);
		Log::formatlog("statistic","zoneid=%d:online_player=%d:cache_size=%d:faction_size=%d:remote_online_player=%d",
				GDeliveryServer::GetInstance()->zoneid, total_online, cache_size, faction_size, remote_online);
	}
	ForbidLogin::GetInstance().Update( update_time );
	ForbidRoleLogin::GetInstance().Update( update_time );
	ForbidTrade::GetInstance().Update( update_time );
	ForbidConsign::GetInstance().Update( update_time );
	ForbidUserTalk::GetInstance().Update( update_time );
	ForbidStudioUser::GetInstance()->Update();

	Thread::HouseKeeper::AddTimerTask(this,update_time);
}

void LinkServer::BroadcastProtocol(const Protocol* p)
{
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	unsigned int iwebsid = dsm->iweb_sid;
	Thread::Mutex::Scoped l(locker);
	for (Set::const_iterator it=set.begin(), ite=set.end(); it!=ite; ++it )
	{
		if(iwebsid!=*it)
			dsm->Send((*it),p);
	}
}

void LinkServer::BroadcastProtocol( Protocol* p)
{
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	unsigned int iwebsid = dsm->iweb_sid;
	Thread::Mutex::Scoped l(locker);
	for (Set::const_iterator it=set.begin(), ite=set.end(); it!=ite; ++it )
	{
		if(iwebsid!=*it)
			dsm->Send((*it),p);
	}
}

bool UserInfo::GenSSOInfo(const Octets & account_os) // bear1111 or 555@qq@sso
{
	ssoinfo.userid = userid;
	ssoinfo.account = account_os;
	std::string account_str((char*)account_os.begin(), account_os.size());
	static const std::string sso_str("@sso");
	size_t pos = account_str.rfind(sso_str);
	if (pos == std::string::npos || pos != account_str.size()-sso_str.size())//判断sso_str是否出现在串末尾
		return false;
	std::string sub_str(account_str.substr(0, pos));
	size_t pos2 = sub_str.rfind('@');
	if (pos2 == std::string::npos || pos2 == sub_str.size()-1)
		return false;
	ssoinfo.isagent = 1;
	ssoinfo.account = Octets(sub_str.c_str(), sub_str.size());
	ssoinfo.agentname = Octets(sub_str.c_str()+pos2+1, sub_str.size()-pos2-1);
	ssoinfo.agentaccount = Octets(sub_str.c_str(), pos2);
	return true;
}

UserInfo* UserContainer::FindUser(int userid,unsigned int link_sid,unsigned int localsid)
{
	UserMap::iterator it = usermap.find(userid);
	if (it==usermap.end())
		return NULL;
	if ((*it).second.linksid!=link_sid || (*it).second.localsid!=localsid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		dsm->Send(link_sid,KickoutUser(userid,localsid,ERR_GENERAL));
		return NULL;
	}
	return &(*it).second;
}

int UserContainer::GetOnlineRolelist(int startrole,int count,std::vector<int>& rolelist,char & finish)
{
	if(count<=0)
		return -1;
	RoleMap::iterator it = rolemap.upper_bound(startrole);
	RoleMap::iterator ite = rolemap.end();
	rolelist.reserve(count);
	for(int i=0;i<count && it!=ite;++it)
	{
		if(it->second && it->second->ingame)
		{
			rolelist.push_back(it->first);
			i++;
		}
	}
	if(it==ite)
		finish=1;
	else
		finish=0;

	return 0;
}

std::string UserContainer::GetUserIP( int userid )
{
	Thread::RWLock::RDScoped l(locker);
	UserMap::iterator it = usermap.find(userid);
	if(it!=usermap.end())
	{
		const char* ip=inet_ntoa( *(struct in_addr*)(&(*it).second.ip) );
		return std::string( ip,strlen(ip) );
	}
	else
		return std::string();
}	
UserInfo::~UserInfo()
{                                       
	if(acstate)                     
		delete acstate;         
}     

void UserContainer::UserLogin( int userid, int linksid, int localsid, bool isgm, int type, int data, int ip, const Octets & iseckey, const Octets & oseckey, const Octets & account, bool announce)
{
	UserInfo ui(userid,linksid,localsid,_STATUS_READYLOGIN);
	ui.rewardtype = type;
	ui.rewarddata = data;
	ui.ip = ip;
	ui.iseckey = iseckey;
	ui.oseckey = oseckey;
	ui.account = account;
	ui.GenSSOInfo(account);
	LOG_TRACE("ssoinfo isagent %d userid %lld account:%.*s agentname:%.*s agentaccount:%.*s",
		ui.ssoinfo.isagent, ui.ssoinfo.userid,
		ui.ssoinfo.account.size(), (char*)ui.ssoinfo.account.begin(),
		ui.ssoinfo.agentname.size(), (char*)ui.ssoinfo.agentname.begin(),
		ui.ssoinfo.agentaccount.size(), (char*)ui.ssoinfo.agentaccount.begin());
	GDeliveryServer* ds = GDeliveryServer::GetInstance();
	//缓存AU发过来的上线区id和角色id，可能不在本区或本服务器
	if (type>=411 && type<=430)		//区号只会在这个范围内，新手卡号肯定不会在这个范围内
	{
		ui.au_suggest_districtid = type;
		ui.au_suggest_referrer = data;
	}

	if (!ForbiddenUsers::GetInstance().IsExist(userid) && !RemoteLoggingUsers::GetInstance().IsExist(userid))	
	{
		ui.status = _STATUS_ONLINE;
		if (announce)
			ds->Send(linksid,OnlineAnnounce(userid,localsid,0,ds->zoneid,0,-1, ds->district_id));
	}
	if(isgm)
		ui.gmstatus |= GMSTATE_ACTIVE;
	else
		ui.gmstatus = 0;
	Thread::RWLock::WRScoped l(locker);
	usermap[userid] = ui;
	EraseRemoteOnline(userid);

	UserInfoCache::GetInstance().Remove(userid); //clear the cache which is used for reconnection
}

void UserContainer::UserReconnect(UserInfo * pinfo, int userid, int roleid, int localsid, int linksid, const Octets & checksum, int loginip)
{
	LOG_TRACE("[reconnect]: gdeliveryd receive the reconnect request from link, roleid=%d, loginip=%d\n", roleid, loginip); 

	GDeliveryServer* gdsm=GDeliveryServer::GetInstance();
	if(pinfo == NULL)
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id); 
		gdsm->Send(linksid, data);
		LOG_TRACE("[reconnect]: reconnect failed, user not existed, user_id=%d, roleid=%d", userid, roleid);
		return;
	}

	if(pinfo->roleid != 0 && pinfo->roleid != roleid)
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id); 
		gdsm->Send(linksid, data);
		LOG_TRACE("[reconnect]: reconnect failed, role not match, user_id=%d, roleid=%d, old_ip=%d, login_ip=%d", userid, roleid, pinfo->ip, loginip);
		return;
	}

	/* 重连后有可能ip地址会改变
	if(pinfo->ip != loginip)
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id); 
		gdsm->Send(linksid, data);
		LOG_TRACE("[reconnect]: reconnect failed, ip not match, user_id=%d, roleid=%d, old_ip=%d, login_ip=%d", userid, roleid, pinfo->ip, loginip);
		return;
	}
	*/

	//只有玩家在线或者玩家已经断线可以重连
	//目前只支持玩家在线的时候断线重连
	if(pinfo->status != _STATUS_ONGAME  && pinfo->status != _STATUS_DISCONNECT)
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
		gdsm->Send(pinfo->linksid, data);

		LOG_TRACE("[reconnect]: reconnect failed, user status not correct, , user_id=%d, roleid=%d, status=%d", userid, roleid, pinfo->status);
		return;
	}	

	Octets hash_checksum;
	MD5Hash md5;

        char buf[32];
	time_t now = time(NULL);
	strftime(buf, sizeof(buf)-1, "20%Y13%m47%d00", localtime(&now));
	Octets rand_time(buf, 16);
	
	md5.Update(pinfo->account);
	md5.Update(pinfo->checksum);
	md5.Update(rand_time);
	md5.Final(hash_checksum);
	if(hash_checksum != checksum)
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
		gdsm->Send(pinfo->linksid, data);

		LOG_TRACE("[reconnect]: reconnect failed, checksum is not correct, , user_id=%d, roleid=%d, status=%d", userid, roleid, pinfo->status);
		return;
	}
	
	if (GDeliveryServer::GetInstance()->IsCentralDS())
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
		gdsm->Send(pinfo->linksid, data);

		LOG_TRACE("[reconnect]: reconnect failed, center ds not allow reconnect, , user_id=%d, roleid=%d, status=%d", userid, roleid, pinfo->status);
		return;
	}

	if (ForbiddenUsers::GetInstance().IsExist(userid))
	{
		TryReconnect_Re data(-1, roleid, userid, localsid, gdsm->zoneid, gdsm->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
		gdsm->Send(pinfo->linksid, data);

		LOG_TRACE("[reconnect]: reconnect failed, forbidden user, , user_id=%d, roleid=%d, status=%d", userid, roleid, pinfo->status);
		return;
	}

	//服务器没有检测到玩家连接有问题，这个时候服务器内部玩家是正常的
	//需要踢掉玩家让玩家重新登录一下
	if(pinfo->status == _STATUS_ONGAME)
	{
		PlayerReconnect data(pinfo->roleid,pinfo->linkid,pinfo->localsid);	
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);

		gdsm->Send(pinfo->linksid, data);

		pinfo->status = _STATUS_TRY_RECONNECT;
		pinfo->linksid = linksid;	//替换成新的linksid
		pinfo->localsid = localsid;	//替换成新的localsid

		LOG_TRACE("[reconnect]: reconnect trying, user online and  send the request to gs: user_id=%d, roleid=%d, status=%d, ip=%d", userid, roleid, pinfo->status, loginip);
		return;

	}
	//服务器内部已经检查到该玩家是异常退出
	else if(pinfo->status == _STATUS_DISCONNECT)
	{
		pinfo->status = _STATUS_ONLINE;

		pinfo->linksid = linksid;
		pinfo->localsid = localsid;	//替换成新的localsid

		GDeliveryServer* gdsm=GDeliveryServer::GetInstance();
		TryReconnect_Re data(ERR_SUCCESS, roleid, userid, pinfo->localsid, gdsm->zoneid, gdsm->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
		gdsm->Send(pinfo->linksid, data);

		UserInfoCache::GetInstance().Remove(userid);
		LOG_TRACE("[reconnect]: reconnect trying, user offline and  send the request to link: user_id=%d, roleid=%d, status=%d", userid, roleid, pinfo->status);
		return;

	}

}

void UserContainer::ReconnectLogin(int userid, bool result)
{
	Thread::RWLock::WRScoped l(locker);
	UserInfo * pinfo = FindUser( userid );
	GDeliveryServer* ds = GDeliveryServer::GetInstance();

	if(pinfo && pinfo->status==_STATUS_TRY_RECONNECT) 
	{
		if (result)
		{
			LOG_TRACE("[reconnect]: reconnect login userid=%d localsid=%d", pinfo->userid, pinfo->localsid);

			TryReconnect_Re data(ERR_SUCCESS, pinfo->roleid, userid, pinfo->localsid, ds->zoneid, ds->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
			GDeliveryServer* gdsm=GDeliveryServer::GetInstance();
			gdsm->Send(pinfo->linksid, data);

			pinfo->status = _STATUS_ONLINE;
			if(pinfo->role)
			{
				RoleLogout(pinfo);
			}
		}
		else
		{
			TryReconnect_Re data(-1, pinfo->roleid, userid, pinfo->localsid, ds->zoneid, ds->district_id, pinfo->iseckey, pinfo->oseckey, pinfo->account);
			GDeliveryServer* gdsm=GDeliveryServer::GetInstance();
			gdsm->Send(pinfo->linksid, data);
			LOG_TRACE("[reconnect]: reconnect login failed userid=%d, localsid=%d", pinfo->userid, pinfo->localsid);

			UserContainer::GetInstance().UserLogout(pinfo);
		}
	}
}


void UserContainer::UserLogout(UserInfo * pinfo,char kicktype, bool force)
{
//	if (!force && (pinfo->status==_STATUS_REMOTE_HALFLOGIN||pinfo->status==_STATUS_REMOTE_LOGIN))
	if (!force && pinfo->status==_STATUS_REMOTE_LOGIN)
		return;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_USERLOGOUT,UserLogoutArg(pinfo->userid,pinfo->localsid)));

	if(pinfo->role && !ForbiddenUsers::GetInstance().IsExist(pinfo->userid) && pinfo->gameid!=_GAMESERVER_ID_INVALID)
	{
		if(kicktype != 0)
		{
			PlayerKickout data(pinfo->roleid,pinfo->linkid,pinfo->localsid);	
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);
		}
		else
		{
			PlayerOffline data(pinfo->roleid,pinfo->linkid,pinfo->localsid);
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);
		}
		ForbiddenUsers::GetInstance().Push(pinfo->userid,pinfo->roleid,pinfo->status);
	}

	if(pinfo->role)
	{
		RoleLogout(pinfo);
	}
	if (GDeliveryServer::GetInstance()->IsCentralDS())
	{
		if (kicktype != KICKOUT_REMOTE) // 0 or 1
		{
			if (pinfo->src_zoneid != 0)
			{
				LOG_TRACE("Send RemoteLogout to zoneid %d userid %d", pinfo->src_zoneid, pinfo->userid);
				CentralDeliveryServer::GetInstance()->DispatchProtocol(pinfo->src_zoneid, RemoteLogout(pinfo->userid));
			}
			//else
				//Log::log(LOG_ERR, "CentralDeliveryServer user %d logout, but src_zoneid is 0", pinfo->userid);
		}
		else if (!ForbiddenUsers::GetInstance().IsExist(pinfo->userid))
		{
			LOG_TRACE("Tell DS zoneid %d Kickout user %d success", pinfo->src_zoneid, pinfo->userid);
			CentralDeliveryServer::GetInstance()->DispatchProtocol(pinfo->src_zoneid, KickoutRemoteUser_Re(ERR_SUCCESS, pinfo->userid));
		}
	}
	else if (kicktype == KICKOUT_LOCAL && (pinfo->status == _STATUS_REMOTE_LOGIN || pinfo->status == _STATUS_REMOTE_HALFLOGIN) &&
			!RemoteLoggingUsers::GetInstance().IsExist(pinfo->userid))
	{//AU发起KickoutUser时玩家可能处于REMOTE_HALFLOGIN状态
	 //玩家或者本地登录 或者远程登录 不可能既发PlayerKickout又发KickoutRemoteUser
		KickoutRemoteUser data(pinfo->userid, GDeliveryServer::GetInstance()->zoneid);
		CentralDeliveryClient::GetInstance()->SendProtocol(data);
		RemoteLoggingUsers::GetInstance().Push(pinfo->userid, pinfo->roleid, pinfo->status);
		LOG_TRACE("KickoutRemoteUser roleid %d userid %d status %d kicktype %d", pinfo->roleid, pinfo->userid, pinfo->status, kicktype);
	}
	RoleInfoCache::Instance().OnLogout(pinfo->logicuid, pinfo->rolelist);
	StockExchange::Instance()->OnLogout(pinfo->userid);
	EraseRemoteOnline(pinfo->userid);
	EraseUser(pinfo->userid); //之后不能再访问pinfo
}

void UserContainer::UserDisconnect(UserInfo * pinfo)
{
	if(!pinfo) return;

	if(pinfo->role && !ForbiddenUsers::GetInstance().IsExist(pinfo->userid) && pinfo->gameid!=_GAMESERVER_ID_INVALID)
	{
		PlayerOffline data(pinfo->roleid,pinfo->linkid,pinfo->localsid);
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);
		ForbiddenUsers::GetInstance().Push(pinfo->userid,pinfo->roleid,pinfo->status);
	}

	if(pinfo->role)
	{
		RoleLogout(pinfo);
	}

	pinfo->status = _STATUS_DISCONNECT;
	time_t now = Timer::GetTime();
	UserInfoCache::GetInstance().Insert(pinfo->userid, now);

}

void UserContainer::RoleLogout(UserInfo* user, bool forward_to_cds)
{
	if(user->gmstatus & GMSTATE_ACTIVE)
		MasterContainer::Instance().Erase(user->roleid);
	PlayerInfo* role = user->role;
	GTPlatformAgent::OnPlayerLogout(role->roleid,role->occupation,role->level);
	RaidManager::GetInstance()->TryAbnormalOffline(role->roleid, role->gameid);

	ACStatusAnnounce2 acsa;
	acsa.status = _STATUS_OFFLINE;
	acsa.info_list.push_back( ACOnlineStatus2(role->roleid,0,0) );
	GAntiCheatClient::GetInstance()->SendProtocol(acsa);

	if(user->status==_STATUS_ONGAME||user->status==_STATUS_REMOTE_LOGIN)
	{
		GNET::Transaction::DiscardTransaction(role->roleid);
		FactionManager::Instance()->OnLogout(role->roleid, role->factionid, role->familyid, role->level, role->occupation);
		CircleManager::Instance()->OnLogout(role->roleid, role->GetCircleID(), role->level, role->occupation);
		ConsignManager::GetInstance()->OnRoleLogout(role->roleid);
//		RaidManager::GetInstance()->OnLogout(role->roleid,role->gameid);
		if(!GDeliveryServer::GetInstance()->IsCentralDS())
		{
			// 因为跨服战场允许玩家回到原服，所以跨服退出时将不清除战场信息
			BattleManager::GetInstance()->OnLogout(role->roleid, role->gameid);
			CrssvrTeamsManager::Instance()->OnLogout(role->roleid, role->level, role->occupation);
		}
		else
		{
			CrossCrssvrTeamsManager::Instance()->OnLogout(role->roleid, user->src_zoneid);
		}

		InstancingManager::GetInstance()->OnLogout(role->roleid, role->gameid);
		ContestManager::GetInstance().OnPlayerLogout(role->roleid);
		SNSPlayerInfoManager::GetInstance()->OnPlayerLogout(role->roleid);
		FunGameManager::GetInstance().OnPlayerLogout(role->roleid);
		HometownManager::GetInstance()->OnPlayerLogout(role->roleid);
		NetMarble::GetInstance()->OnLogout(user);
		FriendCallbackManager::GetInstance()->PlayerLogout(role->roleid);
		
		if (!forward_to_cds)
		{// 普通下线
			LOG_TRACE("Prepare to RoleLogout OnLogout");
			ReferenceManager::GetInstance()->OnLogout(role->roleid);
			LogoutRoleTask::Add(*role);
		}
		else
		{
			PlayerInfo * playerinfo = UserContainer::GetInstance().FindRole(role->roleid);
			if(!playerinfo)
			{
				LOG_TRACE("Prepare to RoleLogout palyerinfo is NULL");
			}
			else
			{
				FriendStatus stat(role->roleid, -2, 0);
				LOG_TRACE("Prepare to RoleLogout Send FriendStatus, friend_ver:%d", playerinfo->friend_ver);
				if(playerinfo && playerinfo->friend_ver>=0)
				{
					LOG_TRACE("RoleLogout Send FriendStatus");
					Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
					for(GFriendInfoVector::iterator it = playerinfo->friends.begin();it!=playerinfo->friends.end();it++)
					{
						PlayerInfo * _pinfo = UserContainer::GetInstance().FindRole(it->rid);
						if (NULL != _pinfo )
						{
							//LOG_TRACE("RoleLogout really Send FriendStatus, linksid=%d", _pinfo->linksid);
							stat.localsid = _pinfo->localsid;
							GDeliveryServer::GetInstance()->Send(_pinfo->linksid,stat);
						}
					}			
				}

				if(playerinfo->friend_ver>0)
				{
						LOG_TRACE("RoleLogout send rpc putFriendList friend_ver=%d,friendsize=%d", playerinfo->friend_ver, playerinfo->friends.size());
						FriendListPair pair;
						pair.key = role->roleid;
						pair.value.groups = playerinfo->groups; 
						pair.value.friends = playerinfo->friends; 
						pair.value.enemies = playerinfo->enemies; 
						LOG_TRACE("RoleLogout send rpc putFriendList friendsize=%d",playerinfo->friends.size() );
						PutFriendList* rpc = (PutFriendList*) Rpc::Call(RPC_PUTFRIENDLIST,pair);
						GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
		}
		{
		/*	FriendStatus stat(role->roleid, -1, 0);
			PlayerInfo * playerinfo = UserContainer::GetInstance().FindRole(role->roleid);
			if(!playerinfo)
			{
				LOG_TRACE("Prepare to RoleLogout palyerinfo is NULL");
				return;
			}
			LOG_TRACE("Prepare to RoleLogout Send FriendStatus, friend_ver:%d", playerinfo->friend_ver);
			if(playerinfo && playerinfo->friend_ver>=0)
			{
				LOG_TRACE("RoleLogout Send FriendStatus");
				Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
				for(GFriendInfoVector::iterator it = playerinfo->friends.begin();it!=playerinfo->friends.end();it++)
				{
					PlayerInfo * _pinfo = UserContainer::GetInstance().FindRoleOnline(it->rid);
					if (NULL != _pinfo )
					{
						LOG_TRACE("RoleLogout really Send FriendStatus, linksid=%d", _pinfo->linksid);
						stat.localsid = _pinfo->localsid;
						GDeliveryServer::GetInstance()->Send(_pinfo->linksid,stat);
					}
				}			
			}*/

		}
	}
	TerritoryManager::GetInstance()->OnLogout(role->roleid, role->gameid, role->world_tag);
	KingdomManager::GetInstance()->OnLogout(role->roleid, role->gameid, role->world_tag);
	if(GDeliveryServer::GetInstance()->IsCentralDS())
	{
		OpenBanquetManager::GetInstance()->TryAbnormalOffline(role->roleid, role->gameid, role->world_tag);
	}

	if(role->name.size())
		EraseName( role->name );

	if(role->sectid)
		SectManager::Instance()->OnLogout(role->sectid, user->roleid, role->level, role->occupation);
	user->rolelist.SeekToBegin();
	user->gameid = _GAMESERVER_ID_INVALID;
	if (!forward_to_cds)
	{
		rolemap.erase(user->roleid);
		user->roleid = 0;
		user->role = NULL;
		delete role;
	}
	else
	{
		role->ingame = false;
		role->gameid = _GAMESERVER_ID_INVALID;
		role->linksid = 0;
		role->localsid = 0;
		role->world_tag = 0;
	}
}

bool UserContainer::OnPlayerLogout(PlayerLogout& cmd)
{
	PlayerInfo* role = FindRole(cmd.roleid);
	if(!role)
		return false;
	UserInfo* user = role->user;
	if(user->linkid!=cmd.provider_link_id || user->localsid!=(unsigned int)cmd.localsid)
		return false;

	RaidManager::GetInstance()->HideSeekApplyingRoleQuit(role->roleid);
	RaidManager::GetInstance()->OnLogout(role->roleid,role->gameid);
	if(GDeliveryServer::GetInstance()->IsCentralDS())
	{
		LOG_TRACE("Prepare to RoleLogout OnLogout OpenBanquetManager,roleid=%d,gs_id=%d,map_tag=%d,map_id=%d",role->roleid, role->gameid, role->world_tag, role->chgs_mapid);
		OpenBanquetManager::GetInstance()->OnLogout(role->roleid, role->gameid, role->world_tag);
		//CrossCrssvrTeamsManager::Instance()->OnLogout(role->roleid, user->src_zoneid);
	}
	else
	{
		LOG_TRACE("Prepare to RoleLogout OnLogout OpenBanquetManager,roleid=%d, lvl=%d, occup=%d",role->roleid, role->level, role->occupation);
		OpenBanquetLogout msg(role->roleid, 0, 0);
		CentralDeliveryClient::GetInstance()->SendProtocol(msg);
		//CrssvrTeamsManager::Instance()->OnLogout(role->roleid, role->level, role->occupation);
	}

	RoleLogout(user);
	if(cmd.result==_PLAYER_LOGOUT_FULL)
		user->status = _STATUS_READYLOGOUT;
	else
		user->status = _STATUS_ONLINE;
	GDeliveryServer::GetInstance()->Send(user->linksid,cmd);
	return true;
}

void UserContainer::ContinueLogin(int userid, bool result)
{
	GDeliveryServer* ds = GDeliveryServer::GetInstance();
	Thread::RWLock::WRScoped l(locker);
	UserInfo * pinfo = FindUser( userid );
	if(pinfo && (pinfo->status==_STATUS_READYLOGIN || pinfo->status==_STATUS_REMOTE_LOGIN))
	{
		if (result)
		{
			DEBUG_PRINT("ContinueLogin: userid=%d is waiting, send OnlineAnnounce, localsid=%d", 
				pinfo->userid, pinfo->localsid);
			ds->Send( pinfo->linksid, OnlineAnnounce(pinfo->userid,pinfo->localsid,0,ds->zoneid,0,-1,ds->district_id));
			pinfo->status = _STATUS_ONLINE;
		}
		else
		{
			DEBUG_PRINT("ContinueLogin: failed, userid=%d,localsid=%d", userid, pinfo->localsid);
			ds->Send(pinfo->linksid,KickoutUser(pinfo->userid,pinfo->localsid,ERR_LOGINFAIL));
		}
	}
}
RoleInfo& RoleInfoCache::ConvertRoleInfo(const GRoleInfo& role, RoleInfo& info)
{
	info.roleid = role.id;
	info.gender = role.gender;
	info.faceid = role.faceid;
	info.hairid = role.hairid;
	info.earid = role.earid;
	info.tailid = role.tailid;
	info.occupation = role.occupation;
	info.level = role.level;
	info.name = role.name;
	info.equipment = role.equipment;
	info.status = role.delete_flag;
	info.delete_time = role.delete_time;
	info.create_time = role.create_time;
	info.lastlogin_time = role.lastlogin_time;
	info.posx = role.posx;
	info.posy = role.posy;
	info.posz = role.posz;
	info.worldtag = role.worldtag;
	info.custom_status = role.custom_status;
	info.charactermode = role.charactermode;
	info.src_zoneid = role.src_zoneid;
	info.fashionid = role.fashionid;
	info.cultivation = role.cultivation;
	return info;
}
GRoleInfo* RoleInfoCache::GetOnlist(int roleid)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
		return NULL;
	GRoleInfo* info = &(it->second);
	//LOG_TRACE("******GetOnlist cache, roleid=%d, status=%d", roleid, info->status);
	info->uptime = Timer::GetTime();
	info->status |= CACHESTATUS_LOCKED;
	return info;
}
GRoleInfo* RoleInfoCache::GetOnlogin(int roleid)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
	{
		Log::log(LOG_ERR,"Role cache miss when login, roleid=%d", roleid);
		return NULL;
	}

	// 设为LOCKED是为了保证玩家在线时cache不被清除，并且玩家下线再上后重新读取GRoleInfo，这是因为
	// 本次登录过程中玩家数据有可能改变
	//LOG_TRACE("******GetOnlogin cache, roleid=%d", roleid);
	GRoleInfo* info = &(it->second);
	info->equipment.clear();
	info->status &= ~CACHESTATUS_COMPLETE;
	info->forbid.clear();
	return info;
}
bool RoleInfoCache::Update()
{
	//LOG_TRACE("******RoleCache update, cursor=%d,size=%d", cursor,roles.size());
	RoleInfoMap::iterator it = roles.upper_bound(cursor);
	time_t now = Timer::GetTime();
	for(int i=0;i<2&&it!=roles.end();++i)
	{
		if((it->second.status&CACHESTATUS_LOCKED)==0 && now-it->second.uptime > 60)
		{
			//LOG_TRACE("******RoleCache Erase timeout role %d", it->first);
			roles.erase(it++);
		}
		else
			++it;
	}
	if(it==roles.end())
		cursor = 0;
	else
		cursor = it->first;
	return true;
}
void RoleInfoCache::SetDelete(int roleid, bool del, int delete_time)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
		return;
	if(del)
	{
		it->second.delete_time = delete_time;
		it->second.delete_flag =  _ROLE_STATUS_READYDEL;
	}
	else
		it->second.delete_flag =  _ROLE_STATUS_NORMAL;
}
void RoleInfoCache::SetSelling(int roleid, bool sell)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
		return;
	if(sell)
	{
		if(it->second.delete_flag == _ROLE_STATUS_NORMAL)
			it->second.delete_flag =  _ROLE_STATUS_SELLING;
		else
			DEBUG_PRINT("RoleInfoCache::SetSelling error,delete_flag=%d",it->second.delete_flag);
	}
	else
	{
		if(it->second.delete_flag == _ROLE_STATUS_SELLING)
			it->second.delete_flag =  _ROLE_STATUS_NORMAL;
		else
			DEBUG_PRINT("RoleInfoCache::SetSelling cancel error,delete_flag=%d",it->second.delete_flag);
	}
}

bool RoleInfoCache::IsRoleDeleted(int roleid)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
		return false;

	if (it->second.delete_flag == _ROLE_STATUS_READYDEL)
		return true;
	else
		return false;
}

bool RoleInfoCache::IsRoleFrozen(int roleid)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
		return false;
	else 
		return it->second.delete_flag == _ROLE_STATUS_FROZEN;
}

bool RoleInfoCache::IsRoleSelling(int roleid)
{
	RoleInfoMap::iterator it = roles.find(roleid);
	if(it==roles.end())
		return false;
	else 
		return it->second.delete_flag == _ROLE_STATUS_SELLING;
}

GRoleInfo RoleInfoCache::ChopRoleInfo(const GRoleInfo& role)
{
	/*避免向客户端传大块无用数据 */
	GRoleInfo info(role);
	info.status = info.delete_flag;
	info.config_data.clear();
	info.help_states.clear();
	for (GRoleInventoryVector::iterator it = info.equipment.begin(); it != info.equipment.end(); ++it)
	{
		GRoleInventory & item = *it;
		if (item.client_size) item.data.resize(item.client_size);
	}
	return info;
}
int UserContainer::DisconnectLinkUsers(unsigned int linksid)
{
	int n = 0;
	for( UserMap::iterator it = usermap.begin();it!=usermap.end();)
	{
		UserMap::iterator itmp = it++;
		if(itmp->second.linksid==linksid)
		{
			UserLogout(&(itmp->second));
			n++;
		}
	}
	return n;
}
int UserContainer::ClearRemoteUsers()
{
	int n = 0;
	for( UserMap::iterator it = usermap.begin();it!=usermap.end();)
	{
		UserMap::iterator itmp = it++;
		if(itmp->second.status==_STATUS_REMOTE_LOGIN ||
			itmp->second.status==_STATUS_REMOTE_HALFLOGIN)
		{
			LOG_TRACE("Clear Remote user %d status %d", itmp->first, itmp->second.status);
			//此时跨服中玩家数据 可能gs还未保存完毕 禁止再次跨服登录 超时后解禁
			RemoteLoggingUsers::GetInstance().Push(itmp->first, itmp->second.roleid, itmp->second.status);
			UserLogout(&(itmp->second), 0, true);
			n++;
		}
	}
	return n;
}
int UserContainer::DisconnectZoneUsers(int zoneid)
{
	int n = 0;
	for( UserMap::iterator it = usermap.begin();it!=usermap.end();)
	{
		UserMap::iterator itmp = it++;
		if(itmp->second.src_zoneid == zoneid)
		{
			/*if (itmp->second.roleid != 0)
				GDeliveryServer::GetInstance()->Send(itmp->second.linksid, DisconnectPlayer(itmp->second.roleid, -1, itmp->second.localsid, -1));*/
			GDeliveryServer::GetInstance()->Send(itmp->second.linksid, KickoutUser(itmp->first, itmp->second.localsid, 0));
			UserLogout(&(itmp->second));
			n++;
		}
	}
	return n;
}

UserIdentityCache::UserIdentityCache()
{
	int timeout = atoi(Conf::GetInstance()->find(GDeliveryServer::GetInstance()->Identification(), "user_iden_cache_time").c_str());
	cache_max_time = timeout > DEFAULT_CACHE_MAXTIME ? timeout : DEFAULT_CACHE_MAXTIME;
	LOG_TRACE("UserIdentityCache cache time %d", cache_max_time);
	Timer::Attach(this); 
}

void UserContainer::LegacyFetchUserid(Protocol::Manager* manager, Protocol::Manager::Session::ID sid, Protocol::Type type, Protocol * cmd, int roleid)
{
	Roleid2Uid* rpc = (Roleid2Uid*) Rpc::Call(RPC_ROLEID2UID,Roleid2UidArg(roleid));
	rpc->_manager = manager;
	rpc->_sid = sid;
	rpc->_pro_type = type;
	rpc->_cmd = cmd;
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void DelayRolelistTask::OnRecvInfo(int uid, int rid)
{
	UserInfo* user = UserContainer::GetInstance().FindUser(uid);
	if(user && PopID(rid))
	{
		LOG_TRACE("DelayRolelistTask, userid=%d, roleid=%d",uid, rid);
		GRoleInfo* info = RoleInfoCache::Instance().GetOnlist(rid);
		if(info && (info->status&CACHESTATUS_COMPLETE)!=0)
		{
			GRoleInfo roleinfo_chop = RoleInfoCache::ChopRoleInfo(*info);
			RoleList_Re re(ERR_SUCCESS, (rid%MAX_ROLE_COUNT), uid, user->localsid);
			RoleInfo roleinfo;
			re.rolelist.add(RoleInfoCache::ConvertRoleInfo(roleinfo_chop, roleinfo));
			if (user->rolelist.GetRoleCount() == 0)
				re.referrer = user->real_referrer>0?user->real_referrer:user->au_suggest_referrer;
			else
				re.referrer = user->real_referrer;
			GDeliveryServer::GetInstance()->Send(user->linksid, re);
			return;
		}
	}
}
//协议处理函数 begin
void SendDataAndIdentity::Process(Manager *manager, Manager::Session::ID sid)
{
	LOG_TRACE("Recv SendDataAndIdentity from zoneid %d roleid %d userid %d ip %d flag %d version %d logintime %d au_isgm %d au_func %d au_funcparm %d auth.size %d",
			src_zoneid, roleid, userid, ip, flag, data_timestamp, logintime, au_IsGM, au_func, au_funcparm, auth.size());
	if (IsTransToCentralDS(flag) || flag == DIRECT_TO_CENTRALDS)
	{
		if (!GDeliveryServer::GetInstance()->IsCentralDS())
			return;
	}
	else if (IsTransToNormalDS(flag))
	{
		if (GDeliveryServer::GetInstance()->IsCentralDS())
			return;
	}
	else
		return;
	SendDataAndIdentity_Re re(-1, roleid, userid, flag, 0);
	UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
	if(pinfo/*&& !arg.blkickuser*/&& pinfo->status!=_STATUS_REMOTE_LOGIN)
	{
		Log::log(LOG_ERR, "SendDataAndIdentity roleid %d, userid %d already online status %d",
				roleid, userid, pinfo->status);
		re.retcode = ERR_MULTILOGIN;
		manager->Send(sid, re);
		return;
	}
	if (pinfo)
		UserContainer::GetInstance().UserLogout(pinfo, 0, true); //pinfo 析构
	if (ForbiddenUsers::GetInstance().IsExist(userid))
	{
		Log::log(LOG_ERR, "SendDataAndIdentity roleid %d is handling by GS",
				roleid);
		re.retcode = ERR_ACCOUNTLOCKED;
		manager->Send(sid, re);
		return;
	}
	if (RemoteLoggingUsers::GetInstance().IsExist(userid))
	{
		Log::log(LOG_ERR, "SendDataAndIdentity user %d is in remote logging process", userid);
		re.retcode = ERR_ACCOUNTLOCKED;
		manager->Send(sid, re);
		return;
	}
	if (UserIdentityCache::GetInstance()->Exist(userid))
	{
		Log::log(LOG_ERR, "UserIdentityCache userid %d already exists", userid);
		manager->Send(sid, re);
		return;
	}
	if (!GameDBClient::GetInstance()->SendProtocol((SavePlayerData *)Rpc::Call(RPC_SAVEPLAYERDATA, SavePlayerDataArg(roleid, userid, src_zoneid, data, flag, data_timestamp))))
	{
		Log::log(LOG_ERR, "SendDataAndIdentity Send to SavePlayerData error roleid %d userid %d zoneid %d", roleid, userid, src_zoneid);
		re.retcode = ERR_GAMEDB_FAIL;
		manager->Send(sid, re);
		return;
	}
	UserIdentityCache::GetInstance()->Insert(userid, UserIdentityCache::Identity(roleid, src_zoneid, ip, iseckey, oseckey, account, random, logintime, au_IsGM, au_func, au_funcparm, auth, forbid_talk));
	LOG_TRACE("Send to SavePlayerData roleid %d userid %d", roleid, userid);
}

void RemoteLoginQuery::Process(Manager *manager, Manager::Session::ID sid)
{
	LOG_TRACE("Recv RemoteLoginQuery retcode %d roleid %d userid %d flag %d",
			retcode, roleid, userid, flag);
	if (GDeliveryServer::GetInstance()->IsCentralDS())
		return;
	RemoteLoginQuery_Re re(ERR_SUCCESS, roleid, userid, flag);
	UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
	if (pinfo == NULL || pinfo->status != _STATUS_REMOTE_HALFLOGIN)
	{
		Log::log(LOG_ERR, "RemoteLoginQuery timeout userid %d userstatus %d",
				userid, pinfo==NULL?0:pinfo->status);
		re.retcode = 101;
		manager->Send(sid, re);
		return;
	}
	if (retcode == ERR_SUCCESS)
	{
		if (flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLEMAP)
		{
			FreezePlayerData * rpc = (FreezePlayerData *)Rpc::Call(RPC_FREEZEPLAYERDATA, FreezePlayerDataArg(roleid, userid));
			if (!GameDBClient::GetInstance()->SendProtocol(rpc))
			{
				Log::log(LOG_ERR, "RemoteLoginQuery FreezePlayerData Failed userid %d roleid %d",
						userid, roleid);
				re.retcode = 102;
				manager->Send(sid, re);
				RemoteLoggingUsers::GetInstance().Pop(userid);
				UserContainer::GetInstance().UserLogout(pinfo);
			}
			else
				LOG_TRACE("RemoteLoginQuery try to FreezePlayerData roleid %d", roleid);
		}
		else
		{
			if (manager->Send(sid, re))
			{
				LOG_TRACE("Send RemoteLoginQuery_Re retcode %d roleid %d userid %d",
						re.retcode, roleid, userid);
				pinfo->status = _STATUS_REMOTE_LOGIN;
				UserContainer::GetInstance().InsertRemoteOnline(userid);
				//	GameDBClient::GetInstance()->SendProtocol(rpc);
			}
			else
				UserContainer::GetInstance().UserLogout(pinfo);
			RemoteLoggingUsers::GetInstance().Pop(userid);
		}
		if(pinfo->actime > 0 && pinfo->acstate)
		{
			CentralDeliveryClient::GetInstance()->SendProtocol(pinfo->acstate);
		}
		VIPInfoCache::GetInstance()->SendVIPInfoToCentralDS(pinfo->userid);
		
		PlayerInfo * playerinfo = UserContainer::GetInstance().FindRole(roleid);
		if(!playerinfo)
		{
			LOG_TRACE("Prepare to RemoteLoginQuery palyerinfo is NULL");
		}
		else
		{
			if(playerinfo->friend_ver<0)
			{
				LOG_TRACE("RemoteLoginQuery send  getfriendlist rpc");
				GetFriendList* rpc = (GetFriendList*) Rpc::Call(RPC_GETFRIENDLIST,RoleId(roleid));
				rpc->roleid = roleid;
				rpc->bUpdateFriend = false;
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
			else
			{
				FriendStatus stat(roleid, REMOTE_ONLINE_FLAG, 0);
				LOG_TRACE("Prepare to RemoteLoginQuery Send FriendStatus, friend_ver:%d", playerinfo->friend_ver);
				if(playerinfo && playerinfo->friend_ver>=0)
				{
					LOG_TRACE("RemoteLoginQuery Send FriendStatus");
					Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
					for(GFriendInfoVector::iterator it = playerinfo->friends.begin();it!=playerinfo->friends.end();it++)
					{
						PlayerInfo * _pinfo = UserContainer::GetInstance().FindRoleOnline(it->rid);
						if (NULL != _pinfo )
						{
							LOG_TRACE("RemoteLoginQuery really Send FriendStatus, linksid=%d", _pinfo->linksid);
							stat.localsid = _pinfo->localsid;
							GDeliveryServer::GetInstance()->Send(_pinfo->linksid,stat);
						}
					}			
				}
				if(playerinfo->friend_ver>0)
				{
					LOG_TRACE("RemoteLoginQuery send rpc putFriendList friend_ver=%d,friendsize=%d", playerinfo->friend_ver, playerinfo->friends.size());
					FriendListPair pair;
					pair.key = roleid;
					pair.value.groups = playerinfo->groups; 
					pair.value.friends = playerinfo->friends; 
					pair.value.enemies = playerinfo->enemies; 
					LOG_TRACE("RemoteLoginQuery send rpc putFriendList friendsize=%d",playerinfo->friends.size() );
					PutFriendList* rpc = (PutFriendList*) Rpc::Call(RPC_PUTFRIENDLIST,pair);
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}

			}
		}

	}
	else
	{
		RemoteLoggingUsers::GetInstance().Pop(userid);
		UserContainer::GetInstance().UserLogout(pinfo);
	}
}

void GetRemoteRoleInfo::Process(Manager *manager, Manager::Session::ID sid)
{
	LOG_TRACE("Recv GetRemoteRoleInfo roleid %d userid %d zoneid %d", roleid, userid, zoneid);
	GetRoleInfo* rpc = (GetRoleInfo*) Rpc::Call(RPC_GETROLEINFO,RoleId(roleid));
	rpc->userid = userid;
	rpc->source = GetRoleInfo::SOURCE_REMOTE; 
	rpc->save_zoneid = zoneid;
	if (!GameDBClient::GetInstance()->SendProtocol(rpc))
		CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, GetRemoteRoleInfo_Re(ERR_COMMUNICATION, roleid, userid, GRoleInfo()));
}

void DBGetServiceForbidCmd::Client(Rpc::Data *argument, Rpc::Data *result, Manager *manager, Manager::Session::ID sid)
{
	//DBGetServiceForbidCmdArg *arg = (DBGetServiceForbidCmdArg *)argument;
	DBGetServiceForbidCmdRes *res = (DBGetServiceForbidCmdRes *)result;
	if (res->retcode == ERR_SUCCESS)
		IwebCmd::GetInstance()->LoadCmd(res->cmdlist);
}

//协议处理函数 end

};

