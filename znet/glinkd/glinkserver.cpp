
#include "glinkserver.hpp"
#include "state.hxx"

#include "gdeliveryclient.hpp"
#include "getlineplayerlimit.hrp"
#include "gproviderserver.hpp"
#include "challenge.hpp"
#include "statusannounce.hpp"
#include "errorinfo.hpp"
#include "acprotostat.hpp"
#include "matrixfailure.hpp"

#include "macros.h"
#include "glog.h"
#include "maperaser.h"
#include <vector>
#include "callid.hxx"
namespace GNET
{

GLinkServer GLinkServer::instance;

const Protocol::Manager::Session::State* GLinkServer::GetInitState() const
{
	return &state_GLoginServer;
}
void GLinkServer::OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer)
{
	Thread::RWLock::WRScoped l(locker_map);
	Iterator it = sessions.find(sid);
	if (it != sessions.end())
	{
		it->second.SetLocal(local);
		it->second.SetPeer(peer);
	}
}
void GLinkServer::OnAddSession(Session::ID sid)
{
	Challenge challenge;
	challenge.Setup(16);
	challenge.version=version;
	challenge.edition = edition;
	challenge.algo = challenge_algo;
	challenge.server_attr = GetServerAttr();
	Send(sid, challenge);
	{
		Thread::RWLock::WRScoped l(locker_map);
		sessions[sid].challenge = challenge.nonce;
		sessions[sid].policy.Initialize();
	}
	SetVerbose(sid, LOG_DEBUG);
	TriggerListen(sid, true);
	Log::log(LOG_DEBUG, "GLinkServer::OnAddSession sid=%d", sid);
}

void GLinkServer::OnDelSession(Session::ID sid, int status)
{
	Log::log(LOG_DEBUG, "GLinkServer::OnDelSession sid=%d status=0x%x", sid, status);
	unsigned char not_ttl=1;
	not_ttl=alivetimemap.erase(sid);
	TriggerListen(sid, false);

	time_t now = time(NULL);
	Iterator it=sessions.find(sid);
	if (it==sessions.end())
		return;
	SessionInfo & info = it->second;
	int userid = info.userid;
	int roleid = info.roleid;
	char strpeer[256];
	if(info.userid>0)
	{
		strcpy( strpeer,inet_ntoa(((struct sockaddr_in*)info.GetPeer())->sin_addr) );
		Log::formatlog("logout","account=%.*s:userid=%d:sid=%d:peer=%s:time=%d:status=0x%x:mid=%.*s",
				info.identity.size(), (char*)info.identity.begin(), userid, sid, strpeer, now-info.login_time, status, info.mid.size(), (char *)info.mid.begin());
		GLog::action("userlogout, acc=%.*s:uid=%d:time=%d:ip=%s", info.identity.size(), (char*)info.identity.begin(), userid, (int)(now-info.login_time),strpeer);
	}

	if (info.checker)
	{
		MatrixFailure mf(info.checker->GetUid(), info.checker->GetIp(), 1);
		GDeliveryClient::GetInstance()->SendProtocol(mf);
		delete info.checker;
		info.checker = NULL;
	}
	sessions.erase(sid);
	if ( roleid>0 )
	{
		RoleInfoMap::iterator itui = roleinfomap.find(roleid);
		if ( itui!=roleinfomap.end() )
		{
			RoleData& ui = itui->second;
			//Log::rolelogout(roleid, now - ui.logintime);
			Log::formatlog("rolelogout","userid=%d:roleid=%d:localsid=%d:time=%d",userid, roleid, ui.sid, now - ui.logintime);
			GLog::action("rolelogout, uid=%d:rid=%d:time=%d:ip=%s",userid,roleid,(int)(now-ui.logintime), strpeer);
			if (ui.sid != sid)
				return;
			roleinfomap.erase(itui);
		}
	}


	if ( !not_ttl )
		DEBUG_PRINT("GLinkd::OnDelSession: Session Closed for TTL expired. userid=%d,sid=%d\n",userid,sid);
	if(userid)
		GDeliveryClient::GetInstance()->SendProtocol(StatusAnnounce(userid, sid, _STATUS_OFFLINE));

}

void GLinkServer::SendErrorInfo(Session::ID sid, int errcode, const char *info)
{
	Send(sid, ErrorInfo(errcode, Octets(info,strlen(info))));
}

void GLinkServer::SessionError(Session::ID sid, int errcode, const char *info)
{
	if(info)
		SendErrorInfo(sid, errcode, info);
	ChangeState(sid, &state_Null);
	Log::log(LOG_DEBUG, "glinkd::SessionError: change to Null state sid=%d, errno=%d reason=%s\n",sid,errcode,info);
}


///////////////////////////////////////////////////////////////////////////////////
//Check keepalive map and shutup user map periodically, and remove timeout items //
///////////////////////////////////////////////////////////////////////////////////

void CheckTimer::CheckConnection()
{
	GLinkServer* lsm=GLinkServer::GetInstance();
	{
		Thread::Mutex::Scoped l(lsm->locker_alive);
		MapEraser<GLinkServer::TimeoutMap> del_keys_alive(lsm->alivetimemap);
		for (GLinkServer::TimeoutMap::iterator it=lsm->alivetimemap.begin();it!=lsm->alivetimemap.end();it++)
		{
			(*it).second-=update_time;
			if ((*it).second<=0)
			{
				DEBUG_PRINT("glinkserver::AliveKeeper:: session %d's TTL is expired(%d). session closed.\n",
					(*it).first,(*it).second);
				lsm->Close((*it).first);
				del_keys_alive.push( it );
			}
		}
		MapEraser<GLinkServer::TimeoutMap> del_keys_close(lsm->readyclosemap);
		for (GLinkServer::TimeoutMap::iterator it=lsm->readyclosemap.begin();it!=lsm->readyclosemap.end();it++)
		{
			(*it).second-=update_time;
			if ((*it).second<=0)
			{
				DEBUG_PRINT("glinkserver::Ready Close:: session %d's is expired(%d). session closed.\n",
					(*it).first,(*it).second);
				lsm->Close((*it).first);
				del_keys_close.push( it );
			}
		}
	}
}
void CheckTimer::CheckForbid()
{
	GLinkServer* lsm=GLinkServer::GetInstance();
	{
		MapEraser<GLinkServer::ForbidMap> del_keys_user(lsm->muteusers);
		for (GLinkServer::ForbidMap::iterator it=lsm->muteusers.begin();it!=lsm->muteusers.end();it++)
		{
			(*it).second.time-=update_time;
			if ((*it).second.time<=0)
			{
				DEBUG_PRINT("glinkserver::ShutupKeeper:: user %d's ShutupTimer is expired.\n",(*it).first);
				del_keys_user.push( it );
			}
		}
	}
	{	
		MapEraser<GLinkServer::ForbidMap> del_keys_role(lsm->muteroles);
		for (GLinkServer::ForbidMap::iterator it=lsm->muteroles.begin();it!=lsm->muteroles.end();it++)
		{
			(*it).second.time-=update_time;
			if ((*it).second.time<=0)
			{
				DEBUG_PRINT("glinkserver::ShutupKeeper:: role %d's ShutupTimer is expired.\n",(*it).first);
				del_keys_role.push( it );
			}
		}
	}
}
void CheckTimer::CheckProtoStat()
{
	GLinkServer* lsm=GLinkServer::GetInstance();
	GLinkServer::SessionInfoMap::iterator it=lsm->sessions.begin(),ite=lsm->sessions.end();
	ACProtoStat acprotostat;
	for ( ;it!=ite;++it )
	{
		SessionInfo& info=(*it).second;
		if ( info.roleid && (info.protostat.remain_time-=update_time)<=0 )
		{
			acprotostat.roleid              =  info.roleid;
			acprotostat.keepalive           =  info.protostat.keepalive;
			acprotostat.gamedatasend        =  info.protostat.gamedatasend;
			acprotostat.publicchat          =  info.protostat.publicchat;
			info.protostat.remain_time      =  ACREPORT_TIMER;
			GDeliveryClient::GetInstance()->SendProtocol( acprotostat );
		}
	}
}	

void LineTimer::Run()
{
	GDeliveryClient::GetInstance()->SendProtocol(
		(GetLinePlayerLimit*)Rpc::Call(RPC_GETLINEPLAYERLIMIT, GetLinePlayerLimitArg(GLinkServer::GetInstance()->GetVersion())));
	Thread::HouseKeeper::AddTimerTask(this,update_time);
}

void CheckTimer::Run()
{
	CheckConnection();
	CheckForbid();
	CheckProtoStat();

	Thread::HouseKeeper::AddTimerTask(this,update_time);
}
void GLinkServer::TriggerListen(Session::ID sid, bool insert)
{
	if(insert)
	{
		halfloginset.insert(sid);
		if (passiveio && halflogin_limit+__HALFLOGIN_THRESHOLD<halfloginset.size())
		{
			Log::log(LOG_ERR,"Number of waiting users %d exceed threshold %d, stop listen", halfloginset.size(), halflogin_limit+__HALFLOGIN_THRESHOLD); 
			passiveio->Close(); 
			passiveio = NULL;
		}
	}
	else
	{
		halfloginset.erase(sid);
		if(!passiveio && halflogin_limit>=__HALFLOGIN_THRESHOLD+halfloginset.size())
		{
			Log::log(LOG_ERR,"Restart passive listen, waiting users=%d, threshold=%d", halfloginset.size(), halflogin_limit+__HALFLOGIN_THRESHOLD); 
			StartListen();
		}
	}
}
bool GLinkServer::ValidLocalsid(Session::ID localsid, int roleid)
{
	Iterator it = instance.sessions.find(localsid);
	if (it==instance.sessions.end() || it->second.roleid!=roleid)
	{
		GDeliveryClient::GetInstance()->SendProtocol(StatusAnnounce(it->second.userid, localsid,_STATUS_OFFLINE));
		return false;
	}
	return true;
}

bool GLinkServer::RoleLogout(Session::ID localsid, int roleid)
{
	SessionInfo * sinfo = GetSessionInfo(localsid);
	if (!sinfo || sinfo->roleid!=roleid)
		return false;
	sinfo->gsid = 0;
	sinfo->roleid = 0;
	sinfo->ingame = 0;

	RoleInfoMap::iterator it = roleinfomap.find(roleid);
	if (it == roleinfomap.end())
		return false;
	RoleData& ui = it->second;
	Log::formatlog("rolelogout","userid=%d:roleid=%d:localsid=%d:time=%d",ui.userid, ui.roleid, ui.sid, GNET::Timer::GetTime()-ui.logintime);
	GLog::action("rolelogout, uid=%d:rid=%d:time=%d:ip=%s",ui.userid,ui.roleid,(int)(GNET::Timer::GetTime()-ui.logintime),
			inet_ntoa(((struct sockaddr_in*)sinfo->GetPeer())->sin_addr));
	roleinfomap.erase(roleid);
	return true;
}
void GLinkServer::RoleLogin(Session::ID localsid, int roleid, int gsid, ByteVector& auth, int src_zoneid)
{
	SessionInfo * sinfo = GetSessionInfo(localsid);
	if(!sinfo)
		return;
	sinfo->roleid = roleid;
	sinfo->gsid   = gsid;
	RoleData uinfo(localsid, sinfo->userid, roleid, gsid, src_zoneid);
	uinfo.logintime = GNET::Timer::GetTime();
	roleinfomap[roleid] = uinfo;
	ChangeState(localsid,&state_GReadyGame);

	GetUserPrivilege(sinfo->userid, auth);
	Log::formatlog("rolelogin","userid=%d:roleid=%d:lineid=%d:localsid=%d:src_zoneid=%d",sinfo->userid, roleid, gsid, localsid, src_zoneid);
	GLog::action("rolelogin, uid=%d:rid=%d:ip=%s",sinfo->userid,roleid,inet_ntoa(((struct sockaddr_in*)sinfo->GetPeer())->sin_addr));
}

void LineList::Process(Manager *manager, Manager::Session::ID sid)
{
	GLinkServer *lsm = (GLinkServer *)manager;
	SessionInfo * sinfo = lsm->GetSessionInfo(sid);
	if (GProviderServer::GetInstance()->GetLineList(*this) && sinfo!=NULL)
	{
		this->algorithm = sinfo->algorithm;
		lsm->Send(sid, *this);
		LOG_TRACE("Send LineList to Client sid=%d", sid);
	}
}

};
