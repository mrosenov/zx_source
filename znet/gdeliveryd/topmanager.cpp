#include "topmanager.h"
#include "dbgettoptable.hrp"
#include "gamedbclient.hpp"
#include "gettoptable_re.hpp"
#include "mapuser.h"
#include "gproviderserver.hpp"
#include "notifydailytablechange.hpp"
#include "factionmanager.h"
#include "facbasemanager.h"
#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"
#include "gdeliveryserver.hpp"

namespace GNET
{
int TopTableID[TOPTABLE_COUNT]={
	TOP_PERSONAL_LEVEL,
	TOP_PERSONAL_MONEY,
	TOP_PERSONAL_REPUTATION,
	TOP_PERSONAL_CREDIT_START,
	TOP_PERSONAL_CREDIT_START+1,
	TOP_PERSONAL_CREDIT_START+2,
	TOP_PERSONAL_CREDIT_START+3,
	TOP_PERSONAL_CREDIT_START+4,
	TOP_PERSONAL_CREDIT_START+5,
	TOP_PERSONAL_CREDIT_START+6,
	TOP_PERSONAL_CREDIT_START+7,
	TOP_PERSONAL_CREDIT_START+8,
	TOP_PERSONAL_CREDIT_START+9,
	TOP_PERSONAL_CREDIT_START+10,
	TOP_PERSONAL_CREDIT_START+11,
	TOP_PERSONAL_CREDIT_START+13,
	TOP_PERSONAL_CREDIT_START+14,
	TOP_PERSONAL_CREDIT_START+15,//保留给东夷
	TOP_PERSONAL_CREDIT_START+16,//太昊
	TOP_PERSONAL_CREDIT_START+17,
	TOP_PERSONAL_CREDIT_START+18,
	TOP_PERSONAL_CREDIT_START+19,//焚香
	//此处不再扩充 改为 TOP_PERSONAL_CREDIT_START2 处扩充
	TOP_FACTION_LEVEL,
	TOP_FACTION_MONEY,
	TOP_FACTION_POPULATION,
	TOP_FACTION_PROSPERITY,
	TOP_FACTION_NIMBUS,
	TOP_FACTION_ACTIVITY,      //帮派活跃度
	TOP_FACTION_CREDIT_START,
	TOP_FACTION_CREDIT_START+1,
	TOP_FACTION_CREDIT_START+2,
	TOP_FACTION_CREDIT_START+3,
	TOP_FACTION_CREDIT_START+4,
	TOP_FACTION_CREDIT_START+5,
	TOP_FACTION_CREDIT_START+6,
	TOP_FACTION_CREDIT_START+7,
	TOP_FACTION_CREDIT_START+8,
	TOP_FACTION_CREDIT_START+9,
	TOP_FACTION_CREDIT_START+10,
	TOP_FACTION_CREDIT_START+11,
	TOP_FACTION_CREDIT_START+13,
	TOP_FACTION_CREDIT_START+14,
	TOP_FACTION_CREDIT_START+15,//保留给东夷
	TOP_FACTION_CREDIT_START+16,//太昊
	TOP_FACTION_CREDIT_START+17,
	TOP_FACTION_CREDIT_START+18,
	TOP_FACTION_CREDIT_START+19,//焚香,注意最多到+19,TOP_FAMILY_TASK_START=60
	TOP_FAMILY_TASK_START,
	TOP_FAMILY_TASK_START+1,
	TOP_FAMILY_TASK_START+2,
	TOP_FAMILY_TASK_START+3,
	TOP_FAMILY_TASK_START+4,
	TOP_FAMILY_TASK_START+5,
	TOP_FAMILY_TASK_START+6,
	TOP_FAMILY_TASK_START+7,
	TOP_FAMILY_TASK_START+8,
	TOP_PERSONAL_CHARM,
	TOP_FRIEND_CHARM,
	TOP_SECT_CHARM,
	TOP_FACTION_CHARM,
	TOP_PERSONAL_DEITY_LEVEL_START,
	TOP_PERSONAL_DEITY_LEVEL_START+1,
	TOP_PERSONAL_DEITY_LEVEL_START+2,
	TOP_PERSONAL_DEITY_LEVEL_START+3,
	TOP_TOWERRAID_LEVEL,
	TOP_CRSSVRTEAMS_SCORE,
	//个人声望
	TOP_PERSONAL_CREDIT_START2+8, //国家贡献度排行 声望 28
	TOP_PERSONAL_CREDIT_START2+10, //千机
	TOP_PERSONAL_CREDIT_START2+11, //英招
	//帮派声望
	TOP_FACTION_CREDIT_START2+10, //千机 
	TOP_FACTION_CREDIT_START2+11, //英招
};


bool TopManager::Initialize()
{	
	return true;
}

void TopManager::OnDBConnect(Protocol::Manager *manager, int sid)
{
	if(id_load<TOPTABLE_COUNT)
		manager->Send(sid,Rpc::Call(RPC_DBGETTOPTABLE,Integer(TopTableID[id_load])));
}

void TopManager::GetDailyTable(int selfid, GetTopTable_Re& re)
{
	Map::iterator ic = tables.find(re.tableid);
	if(ic==tables.end())
		return;
	re.maxsize = ic->second.maxsize;
	if((int)re.pageid<0 || (int)re.pageid*TOP_ITEM_PER_PAGE<0 ||
			re.pageid*TOP_ITEM_PER_PAGE>=(int)ic->second.items.size())
		return;
	{
		std::vector<GTopItem>::iterator it=ic->second.items.begin(),ie=ic->second.items.end();
		std::advance(it,re.pageid*TOP_ITEM_PER_PAGE);
		for(int i=0;i<TOP_ITEM_PER_PAGE&&it!=ie;i++,++it)
			re.items.push_back(*it);
		re.selfrank = 0;
		if(selfid)
		{
			int i = 0;
			for(it=ic->second.items.begin();it!=ie;++it)
			{
				i++;
				if(it->id==(unsigned int)selfid)
				{
					re.selfrank = i;
					break;
				}
			}
		}

	}
	LOG_TRACE("TopManager::GetDailyTable re.tableid=%d", re.tableid);
	if(re.tableid == TOP_CRSSVRTEAMS_SCORE)
	{
		LOG_TRACE("TopManager::GetDailyTable re.tableid=%d, items_detail.size=%d", re.tableid, ic->second.items_detail.size());
		std::vector<GTopDetail>::iterator it=ic->second.items_detail.begin(),ie=ic->second.items_detail.end();
		std::advance(it,re.pageid*TOP_ITEM_PER_PAGE);
		for(int i=0;i<TOP_ITEM_PER_PAGE&&it!=ie;i++,++it)
		{
			std::vector<GCrssvrTeamsRole> rolelist;
			int zoneid;
			Marshal::OctetsStream os((*it).content);
			os >> rolelist >> zoneid;

			re.details.push_back(*it);
			
			//for(int j=0; j<rolelist.size(); ++j)
			//	LOG_TRACE("TopManager::GetDailyTable re.tableid=%d, roleid=%d, occup=%d", 
			//			re.tableid, rolelist[j].roleid, rolelist[j].occupation); 
			//LOG_TRACE("TopManager::GetDailyTable teamid=%d, value1=%d,value2=%d,value3=%d", (*it).id, (*it).value1, (*it).value2, (*it).value3);
		}
		
	}
	return;
}
int  TopManager::GetDailyTable(int tableid, GTopTable& ret)
{
	if(!status)
		return TOP_DATE_NOTREADY;
	Map::iterator ic = tables.find(tableid);
	if(ic==tables.end())
		return 0;
	ret = ic->second;
	ret.id = tableid;
	return 0;
}

int TopManager::GetWeeklyTable(int index, GTopTable& ret)
{
	LOG_TRACE("TopManager::GetWeeklyTable index=%d, TOPTABLE_COUNT=%d. TopTableID[index]=%d", index, TOPTABLE_COUNT, TopTableID[index]);
	if(!status)
		return TOP_DATE_NOTREADY;
	if(index<0||index>=TOPTABLE_COUNT)
		return TOP_INVALID_ID;
	ret.id = TopTableID[index];
	Map::iterator ic = tables.find(TopTableID[index]+WEEKLYTOP_BEGIN);
	if(ic==tables.end())
		return 0;
	ret = ic->second;
	ret.id = TopTableID[index];
	return 0;
}

int TopManager::GetWeeklyTableByTableID(int tableid, GTopTable& ret)
{
	if(!status)
		return TOP_DATE_NOTREADY;
	Map::iterator ic = tables.find(tableid+WEEKLYTOP_BEGIN);
	if(ic==tables.end())
		return 0;
	
	ret = ic->second;
	ret.id = tableid;
	return 0;
}

void TopManager::ClientGetWeeklyTable(int tableid, int pageid, int & maxsize, std::vector<GTopItem> & items)
{
	Map::iterator ic = tables.find(tableid+WEEKLYTOP_BEGIN);
	if(ic==tables.end())
		return;
	maxsize = ic->second.maxsize;
	if((int)pageid<0 || (int)pageid*TOP_ITEM_PER_PAGE<0 ||
			pageid*TOP_ITEM_PER_PAGE>=(int)ic->second.items.size())
		return;
	std::vector<GTopItem>::iterator it=ic->second.items.begin(),ie=ic->second.items.end();
	std::advance(it, pageid*TOP_ITEM_PER_PAGE);
	for(int i=0;i<TOP_ITEM_PER_PAGE&&it!=ie;i++,++it)
		items.push_back(*it);
	/*
	re.selfrank = 0;
	if(selfid)
	{
		int i = 0;
		for(it=ic->second.items.begin();it!=ie;++it)
		{
			i++;
			if(it->id==(unsigned int)selfid)
			{
				re.selfrank = i;
				break;
			}
		}
	}
	*/
	return;
}

void TopManager::UpdateTop(GTopTable& table, int weekly)
{
	LOG_TRACE("TopManager::UpdateTop id=%d", table.id);

	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	bool iscentral = dsm->IsCentralDS();
	if(!iscentral)// 原服不需更新日排行榜，等待跨服发过来的都放此处
	{
		if (table.id == TOP_CRSSVRTEAMS_SCORE)
		{
			return;
		}
	}
	else // 跨服需要把日排行榜同步到原服
	{
		// 更新原服日排行榜, 赛季
		if (table.id == TOP_CRSSVRTEAMS_SCORE)
		{
			LOG_TRACE("TopManager::UpdateTop update src server daily toptable table.id=%d", table.id);
			CrossCrssvrTeamsManager::Instance()->SendSyncTopUpdate(table, CRSSVRTEAMS_DAILY_TOP_TYPE);
			CrossCrssvrTeamsManager::Instance()->SendSyncTopUpdate(table, CRSSVRTEAMS_SEASON_TOP_TYPE);
		}
	}

	table.maxsize = table.items.size();
	tables[table.id] = table;

	GProviderServer::GetInstance()->BroadcastProtocol(NotifyDailyTableChange(table.id));

	if(weekly)
	{
		if (table.id == TOP_FACTION_ACTIVITY)
		{
			//fac_act_toptime = table.time;
			FactionManager::Instance()->OnActTopUpdate(table.time);
//			FacBaseManager::GetInstance()->OnActTopUpdate(table);
			Log::formatlog("toptable", "update fac_act_toptime %d", table.time);
			return; //帮派活跃度周榜更新后不显示 下次维护重启之后才显示
		}

		if (iscentral && table.id == TOP_CRSSVRTEAMS_SCORE)
		{
			// 更新原服周排行榜
			LOG_TRACE("TopManager::UpdateTop update src server weekly toptable, crssvrteams_toptime=%d, table.id=%d, weekly=%d", table.time, table.id, weekly);
			CrossCrssvrTeamsManager::Instance()->SendSyncTopUpdate(table, CRSSVRTEAMS_WEEKLY_TOP_TYPE);
		}

		table.id += WEEKLYTOP_BEGIN;
		tables[table.id] = table;

	}
}

void TopManager::UpdateTopFromCentral(int table_type, GTopTable& table)
{
	LOG_TRACE("TopManager::UpdateTopFromCentral id=%d, table_type=%d", table.id, table_type);

	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	bool iscentral = dsm->IsCentralDS();

	table.maxsize = table.items.size();
	tables[table.id] = table;

	GProviderServer::GetInstance()->BroadcastProtocol(NotifyDailyTableChange(table.id));

	if (!iscentral && table.id == TOP_CRSSVRTEAMS_SCORE)
	{
		LOG_TRACE("TopManager::UpdateTop, crssvrteams_toptime %d, table.id %d, table.maxsize=%d", table.time, table.id, table.maxsize);
		CrssvrTeamsManager::Instance()->OnTopUpdate(table, table_type);
		//Log::formatlog("toptable", "update crssvrteams_toptime %d", table.time);
	}

	if(table_type == CRSSVRTEAMS_WEEKLY_TOP_TYPE)
	{

		table.id += WEEKLYTOP_BEGIN;
		tables[table.id] = table;

	}
}

int TopManager::OnTableLoad(GTopTable& daily, GTopTable& weekly)
{
	LOG_TRACE("TopManager::OnTableLoad daily.id=%d,id_load=%d, LAST_TOPTABLE_ID=%d, TOPTABLE_COUNT=%d, weekly.id=%d", daily.id, id_load, LAST_TOPTABLE_ID, TOPTABLE_COUNT, weekly.id);

	if(daily.id<id_load|| daily.id>LAST_TOPTABLE_ID)
		return -1;
	daily.maxsize = daily.items.size();
	tables[daily.id] = daily;
	weekly.maxsize = weekly.items.size();
	tables[weekly.id+WEEKLYTOP_BEGIN] = weekly;
	if (weekly.id == TOP_FACTION_ACTIVITY)
	{
		//fac_act_toptime = weekly.time;
		FactionManager::Instance()->OnActTopUpdate(weekly.time);
		FacBaseManager::GetInstance()->OnActTopUpdate(weekly);
		Log::formatlog("toptable", "load fac_act_toptime %d", weekly.time);
	}
	++id_load;
	if(id_load<TOPTABLE_COUNT)
		return TopTableID[id_load];
	status = TOP_STATUS_READY;

	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	bool iscentral = dsm->IsCentralDS();
	if(iscentral)
	{
		CrossCrssvrTeamsManager::Instance()->OnCrossTopUpdate(CRSSVRTEAMS_DAILY_TOP_TYPE);
		CrossCrssvrTeamsManager::Instance()->OnCrossTopUpdate(CRSSVRTEAMS_WEEKLY_TOP_TYPE);

		// 原服load靠cross同步
	}

	return -1;
	
}

bool TopManager::GetCredit(int index, unsigned int fid, int64_t* credit) const
{
	if (!status) 
		return false;

	if (index < 0 || index >= TOPTABLE_COUNT - TOP_FACTION_CREDIT_START)	
		return false;

	Map::const_iterator it = tables.find(TopTableID[TOP_FACTION_CREDIT_START+ index - 1]);
	if (it == tables.end())
		return false;
	*credit = 0;
	std::vector<GTopItem>::const_iterator vit = it->second.items.begin();
	for (; vit != it->second.items.end(); ++ vit)
	{
		if (vit->id == fid)
		{
			*credit = vit->value;
			break;
		}
	}
	return true;
}

bool TopManager::IsRankIn(int index, unsigned int fid, unsigned int rank) const
{
	if (!status)
		return false;
	if (index < 0 || index >= TOPTABLE_COUNT - TOP_FACTION_CREDIT_START)
		return false;

	Map::const_iterator it = tables.find(TopTableID[TOP_FACTION_CREDIT_START+ index - 1]);
	if (it == tables.end())
		return false;
	
	if (it->second.items.size() < rank)
		return false;
	std::vector<GTopItem>::const_iterator vit = it->second.items.begin();
	for (unsigned int i = 0; i < rank; ++ vit, i++)
	{
		if (vit->id == fid)
			return true;
	}
	return false;
}

}

