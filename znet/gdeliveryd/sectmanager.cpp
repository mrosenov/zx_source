#include "roleid"
#include "factionid"
#include "log.h"
#include "sectmanager.h"
#include "mapuser.h"
#include "dbsectget.hrp"
#include "dbsectquit.hrp"
#include "maplinkserver.h"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "sectrecruit_re.hpp"
#include "sectexpel_re.hpp"
#include "sectupdate.hpp"
#include "sectquit.hpp"
#include "dbsectupdate.hrp"
#include "gproviderserver.hpp"

namespace GNET
{

bool SectManager::GetSect(int master, SectList_Re& re, int roleid)
{
	Map::iterator it = sects.find(master);
	if(it==sects.end())
	{
		DBSectGet * rpc = (DBSectGet*) Rpc::Call(RPC_DBSECTGET, Integer(master));
		rpc->asker = roleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);	
		return false;
	}
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(master);
	if(pinfo)
		re.lineid = pinfo->gameid;
	else
		re.lineid = 0;
	GSectInfo* info = it->second;
	re.name = info->name;
	re.master = master;
	re.reputation = info->reputation;
	re.skills = info->skills;
	re.disciples = info->disciples;
	re.capacity = GetCapacity(info->reputation);
	info->uptime = Timer::GetTime();
	return true;
}
GSectInfo *SectManager::FindSect(int sectid)
{
	Map::iterator it = sects.find(sectid);
	if(it!=sects.end())
		return it->second;
	else
		return NULL;
}
bool SectManager::CheckSect(int sectid, int roleid)
{
	if (!open)
		return false;
	if(!sectid)
		return false;
	Map::iterator it = sects.find(sectid);
	if(it!=sects.end())
		return true;
	DBSectGet * rpc = (DBSectGet*) Rpc::Call(RPC_DBSECTGET, Integer(sectid));
	rpc->asker = roleid;
	GameDBClient::GetInstance()->SendProtocol(rpc);	
	return false;
}
bool SectManager::GetSkill(int sectid, std::vector<GSectSkill>& skills)
{
	if (!open)
		return false;
	Map::iterator it = sects.find(sectid);
	if(it==sects.end())
		return false;
	skills = it->second->skills;
	it->second->uptime = Timer::GetTime();
	return true;
}

void SectManager::OnLoad(int sectid, GSect& sect)
{
	Map::iterator is = sects.find(sectid);
	if(is!=sects.end())
		return;
	GSectInfo* info = new GSectInfo;
	info->name = sect.name;
	info->skills = sect.skills;
	info->reputation = sect.reputation;
	info->cooldown = sect.cooldown;
	info->dirty = false;
	for(std::vector<GDisciple>::iterator it=sect.disciples.begin();it!=sect.disciples.end();++it)
	{
		PlayerInfo* role = UserContainer::GetInstance().FindRole(it->roleid);
		if(role)
			info->disciples.push_back(GDiscipleInfo(it->roleid,it->name,role->level,role->occupation,role->gameid));
		else
			info->disciples.push_back(GDiscipleInfo(it->roleid,it->name,it->level,it->occupation,0));
	}
	sects[sectid] = info;
}
void SectManager::OnLogin(int sectid, int roleid, int gameid)
{
	if (!open)
		return;
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
		return;
	GSectInfo* info = is->second;
	for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
	{
		if((int)it->roleid==roleid)
		{
			it->lineid = gameid;
			break;
		}
	}
	info->uptime = Timer::GetTime();
}
void SectManager::OnLogout(int sectid, int roleid, char level, char occupation)
{
	if (!open)
		return;
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
		return;
	GSectInfo* info = is->second;
	for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
	{
		if((int)it->roleid==roleid)
		{
			it->lineid = 0;
			if(it->level!=level)
			{
				it->level = level;
				info->dirty = true;
			}
			if(it->occupation!=occupation)
			{
				it->occupation = occupation;
				info->dirty = true;
			}
			break;
		}
	}
	info->uptime = Timer::GetTime();

}
void SectManager::OnJoin(int sectid, GDisciple& d)
{
        SectRecruit_Re re(0, sectid, d.roleid, d.name, 0);
	Map::iterator is = sects.find(sectid);
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	if(is==sects.end())
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(sectid);
		if(pinfo)
		{
			if(pinfo->sectid==0)
			{
				// 新建师门通知师傅所在gs
				SectUpdate data;
				data.sectid = sectid;
				data.roleid = sectid;
				GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, data );
				DEBUG_PRINT("SectManager, send sectupdate to master id=%d line=%d", sectid, pinfo->gameid);
			}
			pinfo->sectid = sectid;
			re.localsid = pinfo->localsid;
			dsm->Send(pinfo->linksid,re);
		}
		pinfo = UserContainer::GetInstance().FindRoleOnline(d.roleid);
		if(pinfo)
		{
			pinfo->sectid = sectid;
			re.localsid = pinfo->localsid;
			dsm->Send(pinfo->linksid,re);
			SectUpdate data;
			data.sectid = sectid;
			data.roleid = d.roleid;
			GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, data );
		}
		DBSectGet * rpc = (DBSectGet*) Rpc::Call(RPC_DBSECTGET, Integer(sectid));
		rpc->asker = 0;
		GameDBClient::GetInstance()->SendProtocol(rpc);	
		return;
	}
	int lineid = 0;
	PlayerInfo * role = UserContainer::GetInstance().FindRoleOnline(sectid);
	if(role)
		role->sectid = sectid;
	role = UserContainer::GetInstance().FindRole(d.roleid);
	if(role)
	{
		SectUpdate data;
		data.sectid = sectid;
		data.roleid = d.roleid;
		data.reputation = is->second->reputation;
		data.skills = is->second->skills;
		lineid = role->gameid;
		GProviderServer::GetInstance()->DispatchProtocol( lineid, data );
		role->sectid = sectid;
	}
	is->second->cooldown = Timer::GetTime();
	is->second->disciples.push_back(GDiscipleInfo(d.roleid,d.name,d.level,d.occupation,lineid));
        Broadcast(sectid, is->second, re, re.localsid);
}
int SectManager::DoExpel(int sectid, int disciple)
{
	if (!open)
		return -1;
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
		return ERR_SECT_BUSY;
	GSectInfo* info = is->second;
	for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
	{
		if((int)it->roleid==disciple)
		{
			DBSectQuit* rpc = (DBSectQuit*) Rpc::Call( RPC_DBSECTQUIT, DBSectQuitArg(sectid, disciple));
			rpc->reason = -1;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return 0;
		}
	}
	return 1;
}
int SectManager::Quit(int sectid, int disciple, char reason)
{
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
		return ERR_SECT_BUSY;
	GSectInfo* info = is->second;
	for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
	{
		if((int)it->roleid==disciple)
		{
			DBSectQuit* rpc = (DBSectQuit*) Rpc::Call( RPC_DBSECTQUIT, DBSectQuitArg(sectid, disciple));
			rpc->reason = reason;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return 0;
		}
	}
	return 1;
}
void SectManager::Broadcast(int sectid, GSectInfo* info, Protocol& data, unsigned int &localsid)
{
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(sectid);
	if(pinfo)
	{
		LOG_TRACE("SectBroadcast send master roleid=%d", sectid);
		localsid = pinfo->localsid;
		dsm->Send(pinfo->linksid,data);
	}
	for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
	{
		if(it->lineid==0)
			continue;
		LOG_TRACE("SectBroadcast roleid=%d", it->roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(it->roleid);
		if(pinfo)
		{
			localsid = pinfo->localsid;
			dsm->Send(pinfo->linksid,data);
		}
	}

}
void SectManager::OnQuit(int sectid, int roleid, Octets& name, char reason)
{
	PlayerInfo* role = UserContainer::GetInstance().FindRoleOnline(roleid);
	SectQuit data;
	data.sectid = sectid;
	data.name = name;
	data.disciple = roleid;
	data.reason = reason?0:1;
	if(role)
	{
		GProviderServer::GetInstance()->DispatchProtocol( role->gameid, data );
		role->sectid = 0;
	}
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
		return;
	GSectInfo* info = is->second;
	if(reason==-1)
	{
		SectExpel_Re re(0, sectid, roleid, name, 0);
		Broadcast(sectid, info, re, re.localsid);
	}
	else
		Broadcast(sectid, info, data, data.localsid);


	for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
	{
		if((int)it->roleid==roleid)
		{
			info->disciples.erase(it);
			break;
		}
	}
}
int SectManager::PreRecruit(int sectid, int invitee)
{
	if (!open)
		return -1;
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
		return ERR_SECT_BUSY;
	if(is->second->disciples.size()>=GetCapacity(is->second->reputation))
		return ERR_SECT_FULL;
	if(Timer::GetTime()-is->second->cooldown<=86400)
		return ERR_SECT_COOLING;
	return 0;
}
bool SectManager::CheckCapacity(int sectid, unsigned int& capacity)
{
	Map::iterator is = sects.find(sectid);
	if(is==sects.end())
	{
		capacity = 1;
		return true;
	}
	capacity = GetCapacity(is->second->reputation);
	if(is->second->disciples.size()>=capacity)
		return false;
	return true;
}

unsigned int  SectManager::GetCapacity(int reputation)
{
	if (reputation<350)
		return 1;
	else if(reputation<1500)
		return 2;
	else if(reputation<4500)
		return 3;
	else if(reputation<12000)
		return 4;
	else if(reputation<30000)
		return 5;
	else if(reputation<72000)
		return 6;
	else
		return 7;
}

void SectManager::UpdateSect(int sectid, unsigned int reputation, std::vector<GSectSkill>& skills)
{
	if (!open)
		return;
	Map::iterator is = sects.find(sectid);
	if(is!=sects.end())
	{
		GSectInfo* info = is->second;
		if(info->reputation != (int)reputation)
		{
			info->reputation = reputation;
			info->dirty = true;
		}
		info->skills = skills;
		if(!info->dirty)
		{
			std::vector<GSectSkill>::iterator it1, it2;
			for(it1=info->skills.begin(),it2=skills.begin();it1!=info->skills.end()&&it2!=skills.end();++it1,++it2)
			{
				if(it1->id!=it2->id || it1->level!=it2->level)
					break;
			}
			if(it1!=info->skills.end()&&it2!=skills.end())
				info->dirty = true;
		}
		if(info->dirty)
			info->dirty = false;
		else 
			return;
	}

	DBSectUpdateArg arg;
	arg.sectid = sectid;
	arg.sect.reputation = reputation;
	arg.sect.skills = skills;
	LOG_TRACE("UpdateSect sectid=%d, reputation=%d, skills=%d", sectid, reputation, skills.size());

	DBSectUpdate * rpc = (DBSectUpdate*) Rpc::Call(RPC_DBSECTUPDATE, arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);	
}
void SectManager::Initialize()
{
	IntervalTimer::Attach(this,1000000/IntervalTimer::Resolution());
	open = true;
}
void SectManager::OnUpdate(int sectid, GSect& sect)
{
	LOG_TRACE("OnUpdate sectid=%d", sectid);
	Map::iterator is = sects.find(sectid);
	if(is!=sects.end())
	{
		GSectInfo* info = is->second;
		info->reputation = sect.reputation;
		info->skills = sect.skills;
		return;
	}
	GSectInfo* info = new GSectInfo;
	info->name = sect.name;
	info->skills = sect.skills;
	info->reputation = sect.reputation;
	info->cooldown = sect.cooldown;
	info->dirty = false;
	for(std::vector<GDisciple>::iterator it=sect.disciples.begin();it!=sect.disciples.end();++it)
	{
		PlayerInfo* role = UserContainer::GetInstance().FindRole(it->roleid);
		if(role)
			info->disciples.push_back(GDiscipleInfo(it->roleid,it->name,role->level,role->occupation,role->gameid));
		else
			info->disciples.push_back(GDiscipleInfo(it->roleid,it->name,it->level,it->occupation,0));
	}
	sects[sectid] = info;
}

void SectManager::OnRolenameChange(int sectid, int roleid, const Octets & oldname, const Octets & newname)
{
	GSectInfo * psect = FindSect(sectid);
	if (psect == NULL)
		return;
	if (sectid == roleid)
	{
		//if (oldname == psect->name)
			psect->name = newname;
	}
	else
	{
		std::vector<GDiscipleInfo>::iterator it, ite = psect->disciples.end();
		for (it = psect->disciples.begin(); it != ite; ++it)
		{
			if (it->roleid == roleid)
			{
				it->name = newname;
				break;
			}
		}
	}
	psect->dirty = true; //目前不必要 因为 DBSectUpdate 不会覆盖 name
}

bool SectManager::Update() 
{ 
	if(sects.size()<2)
		return true;
	time_t now = Timer::GetTime();
	Map::iterator it = sects.upper_bound(cursor);

	for(int i=0;i<8 && it!=sects.end(); ++i,++it)
	{
		GSectInfo* info = it->second;
		if(info->dirty)
		{
			DBSectUpdateArg arg;
			arg.sectid = it->first;
			arg.sect.reputation = info->reputation;
			arg.sect.skills = info->skills;
			for(std::vector<GDiscipleInfo>::iterator it=info->disciples.begin();it!=info->disciples.end();++it)
			{
				arg.sect.disciples.push_back(GDisciple(it->roleid,it->name,it->level,it->occupation));
			}
			DBSectUpdate * rpc = (DBSectUpdate*) Rpc::Call(RPC_DBSECTUPDATE, arg);
			GameDBClient::GetInstance()->SendProtocol(rpc);	
			info->dirty = false;
			break;
		}
		if(now-it->second->uptime>1200)
		{
			PlayerInfo * role = UserContainer::GetInstance().FindRole(it->first);
			if(role)
				info->uptime = now;
			else
			{
				std::vector<GDiscipleInfo>::iterator is=info->disciples.begin();
				for(;is!=info->disciples.end();++is)
					if(is->lineid)
						break;
				if(is==info->disciples.end())
				{
					DEBUG_PRINT("SectManager, remove obsolete sect id=%d", it->first);
					delete info;
					sects.erase(it);
				}
				else
					info->uptime = now;

			}
			break;
		}
	}
	if(it==sects.end())
		cursor = 0;
	else
		cursor = it->first;
	return true;
}


};

