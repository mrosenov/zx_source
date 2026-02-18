#include "gterritoryitemget.hrp"
#include "dbterritorychallenge.hrp"
#include "dbterritorylistload.hrp"
#include "dbterritorylistsave.hrp"
#include "dbterritorysave.hrp"
#include "territorymapget_re.hpp"
#include "territorybrief"
#include "factionmanager.h"
#include "syncterritorylist.hpp"
#include "domaindataman.h"
#include "gterritorychallenge.hpp"
#include "gterritoryenter.hpp"
#include "gterritoryleave.hpp"
#include "sendterritorystart.hpp"
#include "dbfactionget.hrp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "territoryenterremind.hpp"
#include "territoryscoreupdate.hpp"
#include "familymanager.h"

namespace GNET
{
static void LoadFaction(int fid)
{
	FactionDetailInfo * faction = FactionManager::Instance()->Find(fid);
	if (!faction)
	{
		DBFactionGet* rpc = (DBFactionGet*) Rpc::Call( RPC_DBFACTIONGET,FactionId(fid));
		rpc->listener = new TerritoryListener();
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return;
	}
	faction->SetKeepAlive();
}

static void UnLoadFaction(int fid)
{
	FactionDetailInfo * faction = FactionManager::Instance()->Find(fid);
	if (faction)
		faction->ClrKeepAlive();
}

TerritoryInfo::TerritoryInfo(const GTerritoryInfo & g)
{
	_id = g.id;
	_owner = g.owner;
	if (_owner) LoadFaction(_owner);
	_occupy_time = g.occupy_time;
	_color = g.color;
	_candidate_challenges.clear();
	GTChallengeVector::const_iterator it, ite = g.challengelist.end();
	for (it = g.challengelist.begin(); it != ite; ++it)
	{
		_candidate_challenges.push_back(TChallenge(it->factionid, it->itemcount));
		LoadFaction(it->factionid);
	}
	_defender = g.defender;
	_success_challenge = TChallenge(g.success_challenge.factionid, g.success_challenge.itemcount);
	if (_success_challenge._factionid)
		LoadFaction(_success_challenge._factionid);
	_success_award = g.success_award;
	_start_time = g.start_time;
	_assis_drawn_num = g.assis_drawn_num;
	_rand_award_itemid = g.rand_award_itemid;
	_rand_award_itemcount = g.rand_award_itemcount;
	_rand_award_drawn = g.rand_award_drawn;

	DOMAIN_INFO_SERV * pterritory = domain_data_getbyid(g.id);
	if(!pterritory)
	{
		type = 0;
		item_min = 0;
		item_max = 0;
		Log::log(LOG_ERR, "Load Territory(%d) error, can not find domain config", g.id);
	}
	else
	{
		type = pterritory->type;
		item_min = pterritory->item_lowerlimit;
		item_max = pterritory->item_upperlimit;
		LOG_TRACE("Load Territory(%d) type %d owner %d lowerlimit %d upperlimit %d", g.id, type, _owner, item_min, item_max);
	}
	status = 0;
	map_tag = 0;
	timeout = 0;
}

void TerritoryInfo::EnterRemind()
{
	LOG_TRACE("Territory %d EnterRemind", _id);
	TerritoryEnterRemind remind(_id);
	FactionManager::Instance()->Broadcast(_defender, remind, remind.localsid);
	FactionManager::Instance()->Broadcast(_success_challenge._factionid, remind, remind.localsid);
}

void TerritoryInfo::EndAnnounce(int result, int time)
{
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_TERRI_END;
	Marshal::OctetsStream msg;
	int attacker = _success_challenge._factionid;
	Octets defender_name, attacker_name;
	if (_defender)
		FactionManager::Instance()->GetName(_defender, defender_name);
	FactionManager::Instance()->GetName(attacker, attacker_name);
	msg<<_id<<_defender<<defender_name<<attacker<<attacker_name<<result<<time;
	chat.msg = msg;
	LOG_TRACE("Territory %d End Announce defender %d namesize %d attacker %d namesize %d result %d time %d", _id, _defender, defender_name.size(), attacker, attacker_name.size(), result, time);
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

void TerritoryBidAnnounceTask::Run()
{               
	if (_type != ANNOUNCE_BID1 || TerritoryManager::GetInstance()->IsBidAnnounceState())
	{
		LOG_TRACE("Territory Announce bid %d", _type);
		ChatBroadCast chat;
		chat.channel = GP_CHAT_SYSTEM;
		chat.srcroleid = MSG_TERRI_BID;
		Marshal::OctetsStream msg;
		msg<<_type;
		chat.msg = msg;
		LinkServer::GetInstance().BroadcastProtocol(chat);

		if (_type == ANNOUNCE_BID1)
		{
			Thread::HouseKeeper::AddTimerTask(this, TerritoryManager::TERRITORY_BID_ANNOUNCE_INTERVAL);
			return;
		}
	}
	delete this;
}

void TerritoryAwardAnnounceTask::Run()
{               
	LOG_TRACE("Territory Award Announce territoryid %d itemid %d", _territoryid, _itemid);
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_TERRI_AWARD;
	Marshal::OctetsStream msg;
	msg<<_territoryid<<_itemid;
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);

	if(!TerritoryManager::GetInstance()->IsAwardAnnounceState())
	{       
		delete this;
		return;
	}
	Thread::HouseKeeper::AddTimerTask(this, TerritoryManager::TERRITORY_AWARD_ANNOUNCE_INTERVAL);
}

void TerritoryManager::StartAnnounce()
{
	LOG_TRACE("Territory Start Announce");
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_TERRI_START;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

bool TerritoryManager::SendTerritoryMap(int roleid, unsigned int sid, unsigned int localsid)
{
	if (!(status & ST_DATAREADY))
		return false;
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
	if (!pinfo)
		return false;
	TerritoryMapGet_Re re;
	re.retcode = ERR_SUCCESS;
	re.status = (status&(ST_BID|ST_ASSISGET|ST_FIGHT));
	re.localsid = localsid;
	//time_t now = GetTime();
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		TerritoryBrief brief(it->_id);
		brief.owner = it->_owner;
		brief.defender = it->_defender;
		brief.attacker = it->_success_challenge._factionid;
		if (it->status & TS_FIGHTING)
			brief.flag = 0;
	//	else if (it->_defender == 0) //无人宣战
		else if (it->_success_challenge._factionid == 0)
			brief.flag = -1;
	//	else if (now >= it->_start_time + 60 && (status&ST_FIGHT))
		else if (it->_start_time == 0 && (status&ST_FIGHT))
		{
			if (it->_owner == it->_defender)
				brief.flag = 1;
			else
				brief.flag = 2;
		}
		else
			brief.flag = -1;
		brief.color = it->_color;
		brief.battle_time = it->_start_time;
		brief.battle_end_time = brief.battle_time + type2config[it->type].battle_last_time;
		if (pinfo->factionid != 0)
		{
			if (FactionManager::Instance()->IsMaster(it->_owner, roleid))
				brief.item_count = it->_success_challenge._itemcount;//胜利方领取宣战金奖励用	
			else
			{//宣战失败方领取宣战金用
				std::vector<TChallenge>::const_iterator cit, cite = it->_candidate_challenges.end();
				for (cit = it->_candidate_challenges.begin(); cit != cite; ++cit)
				{
					if (pinfo->factionid == cit->_factionid)
					{
						brief.challenged_by_selffaction = 1;
						if (FactionManager::Instance()->IsMaster(cit->_factionid, roleid))
							brief.item_count = cit->_itemcount;
						break;
					}
				}
			}
			if (status & ST_ASSISGET)
			{
				if (FactionManager::Instance()->IsMaster(it->_owner, roleid))
				{
					brief.assis_num = type2config[it->type].assis_num;
					brief.assis_drawn_num = it->_assis_drawn_num;
				}
			}
		}
		brief.defender_num = it->team_defender.size();
		brief.attacker_num = it->team_attacker.size();
		TerritoryTag * ptag = FindMapTag(it->map_tag);
		if (ptag != NULL && ptag->tid == it->_id)
			brief.player_limit = ptag->player_limit;
		brief.rand_award_itemid = it->_rand_award_itemid;
		brief.rand_award_count = it->_rand_award_itemcount;
		brief.rand_award_drawn = it->_rand_award_drawn;
		re.tlist.push_back(brief);
	}
	GDeliveryServer::GetInstance()->Send(sid, re);
	return true;
}

bool TerritoryManager::SyncTerritoryGS()
{
	LOG_TRACE( "TerritoryManager: Sync TerritoryList to GS.");
	if (status & ST_DATAREADY)
	{
		SyncTerritoryList sync;
		for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
		{
			sync.tlist.push_back(TerritoryOwner(it->_id, it->type, it->_owner));
		}
		GProviderServer::GetInstance()->BroadcastProtocol(sync);
	}
	return true;
}

bool TerritoryManager::SyncTerritoryGS(unsigned int sid)
{
	LOG_TRACE( "TerritoryManager: Sync TerritoryList to GS sid %d.", sid);
	if (status & ST_DATAREADY)
	{
		SyncTerritoryList sync;
		for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
		{
			sync.tlist.push_back(TerritoryOwner(it->_id, it->type, it->_owner));
		}
		GProviderServer::GetInstance()->Send(sid, sync);
	}
	return true;
}

bool TerritoryManager::IsAdjacent(int territoryid, unsigned int fid)
{
	DOMAIN_INFO_SERV * pterritory = domain_data_getbyid(territoryid);
	if(!pterritory)
		return false;
	TVector::iterator ie=tlist.end();
	for(std::vector<int>::iterator i=pterritory->neighbours.begin();i!=pterritory->neighbours.end();++i)
	{
		for(TVector::iterator it=tlist.begin();it!=ie;++it)
		{
			if(it->_id==*i)
			{
				if(fid==it->_owner)
					return true;
				break;
			}
		}
	}
	return false;
}

int TerritoryManager::Challenge(const GTerritoryChallenge& proto)
{
	if (!((status&ST_OPEN)&&(status&ST_BID)))
		return ERR_TERRI_BID_TIME;
	if (!(status & ST_GSREADY))
		return ERR_TERRI_BID_BUSY;
	if (!FactionManager::Instance()->IsMaster(proto.factionid, proto.roleid))
		return ERR_TERRI_BID_NOTMASTER;
	FactionDetailInfo * pfac = FactionManager::Instance()->Find(proto.factionid);
	if (pfac == NULL || GetTime() < pfac->info.createtime + CONDITION_CREATE_TIME)
		return ERR_TERRI_BID_CREATETIME;
	if (IsInCooldown(proto.factionid))
		return ERR_TERRI_BID_COOLING;
	std::map<int, int> lev2num;
	TVector::iterator i, ite = tlist.end(), it = tlist.end();
	for (i = tlist.begin(); i != ite; ++i)
	{
		if (i->_owner == proto.factionid)
			lev2num[i->type]++;
		if (i->_id == proto.territoryid)
		{
			it = i;
			continue;
		}
		std::vector<TChallenge>::const_iterator cit, cite = i->_candidate_challenges.end();
		for (cit = i->_candidate_challenges.begin(); cit != cite; ++cit)
		{
			if (cit->_factionid == proto.factionid)
				return ERR_TERRI_BID_REPEAT;
		}
	}
	if (it == tlist.end())
		return ERR_TERRI_NOTFOUND;
	if (it->status & TS_BIDBUSY)
		return ERR_TERRI_BID_BUSY;
	int f_level;
	Octets f_name;
	if (!FactionManager::Instance()->GetBrief(proto.factionid, f_name, f_level))
		return ERR_TERRI_BID_FAC_NOTFOUND;
	if (it->_owner == proto.factionid)
		return ERR_TERRI_BID_SELF;
	if (proto.itemid != BID_ITEMID)
		return ERR_TERRI_BID_ITEMTYPE;
	if (proto.item_count < 1)
		return ERR_TERRI_BID_ITEM_MIN;
	int itemtotal = proto.item_count;
	std::vector<TChallenge>::const_iterator cit, cite = it->_candidate_challenges.end();
	for (cit = it->_candidate_challenges.begin(); cit != cite; ++cit)
	{
		if (cit->_factionid == proto.factionid)
		{
			itemtotal += cit->_itemcount;
			break;
		}
	}
	if (cit == cite)
	{
		if (it->_candidate_challenges.size() >= TERRITORY_BID_LIST_MAX)
			return ERR_TERRI_BID_LISTSIZE;
	}
	if (itemtotal < it->item_min)
		return ERR_TERRI_BID_ITEM_MIN;
	if (itemtotal > it->item_max)
		return ERR_TERRI_BID_ITEM_MAX;
	if (it->type == DOMAIN_TYPE_1ST_CLASS)
	{
		if (f_level < 3)
			return ERR_TERRI_BID_FAC_LEVEL;
		if ((lev2num[DOMAIN_TYPE_3RD_CLASS] != 0
			|| lev2num[DOMAIN_TYPE_2ND_CLASS] != 0
			|| lev2num[DOMAIN_TYPE_1ST_CLASS] != 0)
			&& !IsAdjacent(it->_id, proto.factionid))
			return ERR_TERRI_BID_ADJACENT;
	}
	else if (it->type == DOMAIN_TYPE_2ND_CLASS)
	{
		if (f_level < 4)
			return ERR_TERRI_BID_FAC_LEVEL;
		if (lev2num[DOMAIN_TYPE_1ST_CLASS] < 2)
			return ERR_TERRI_BID_OCCUPY_NUM;
		if (!IsAdjacent(it->_id, proto.factionid))
			return ERR_TERRI_BID_ADJACENT;
	}
	else if (it->type == DOMAIN_TYPE_3RD_CLASS)
	{
		if (f_level < 5)
			return ERR_TERRI_BID_FAC_LEVEL;
		if (lev2num[DOMAIN_TYPE_2ND_CLASS] < 2)
			return ERR_TERRI_BID_OCCUPY_NUM;
		if (!IsAdjacent(it->_id, proto.factionid))
			return ERR_TERRI_BID_ADJACENT;
	}
	DBTerritoryChallengeArg arg(proto.roleid, proto.territoryid, proto.factionid, proto.itemid, proto.item_count, proto.syncdata);
	LOG_TRACE("send DBTerritoryChallenge roleid %d tid %d fid %d itemid %d itemcount %d", arg.roleid, arg.territoryid, arg.factionid, arg.itemid, arg.item_count);
	DBTerritoryChallenge * rpc = (DBTerritoryChallenge *)Rpc::Call(RPC_DBTERRITORYCHALLENGE, arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
	it->status |= TS_BIDBUSY;
	SetCooldown(proto.factionid);
	return ERR_SUCCESS;
}
			
bool TerritoryManager::LoadConfig()
{
/*
#territory.conf
#########################################################
[TypeConf]
assis_num = 2,4,8
battle_time = 1800,3600,7200
[RandAwardConf]
item_list = (1001,1)(1002,3)(1003,5)
#########################################################
*/
	Conf *conf = Conf::GetInstance("territory.conf");
	int ret = 0;
	int assis_num[3];
	ret = sscanf( conf->find("TypeConf", "assis_num").c_str(), "%d,%d,%d", 
				&(assis_num[0]), &(assis_num[1]), &(assis_num[2]));
	if (ret != 3)
	{
		Log::log(LOG_ERR, "Territory load conf, invalid assis_num:%s", 
				conf->find("TypeConf", "assis_num").c_str());
		Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
		return false;
	}
	int battle_time[3];
	ret = sscanf( conf->find("TypeConf", "battle_time").c_str(), "%d,%d,%d", 
				&(battle_time[0]), &(battle_time[1]), &(battle_time[2]));
	if (ret != 3)
	{
		Log::log(LOG_ERR, "Territory load conf, invalid battle_time:%s", 
				conf->find("TypeConf", "battle_time").c_str());
		Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
		return false;
	}
	std::string rand_str = conf->find("RandAwardConf", "item_list");
	if( rand_str.length() > 1023 )
	{               
		Log::log(LOG_ERR, "Territory load conf, item_list is too long");
		Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
		return false;
	}
	type2config[DOMAIN_TYPE_1ST_CLASS] = TypeConfig(assis_num[0], battle_time[0], 10);//低级
	type2config[DOMAIN_TYPE_2ND_CLASS] = TypeConfig(assis_num[1], battle_time[1], 25);//中级
	type2config[DOMAIN_TYPE_3RD_CLASS] = TypeConfig(assis_num[2], battle_time[2], 50);//高级
	LOG_TRACE("Territory assis %d-%d-%d time %d-%d-%d", assis_num[0], assis_num[1], assis_num[2],
					battle_time[0], battle_time[1], battle_time[2]);
	char rand_buffer[1024];
	strncpy( rand_buffer, rand_str.c_str(), std::min(sizeof(rand_buffer)-1,rand_str.length()) );
	rand_buffer[sizeof(rand_buffer)-1] = 0;
	char * cur = rand_buffer;
	char * token = strchr( cur, '(' );
	while( NULL != token )
	{       
		cur = token+1;
		token = strchr( cur, ',' );
		if( NULL == token )     break;
		*token = 0;
		int itemid = atol(cur);
		if (itemid <= 0)
		{
			Log::log(LOG_ERR, "Territory load conf, invalid itemid %d", itemid);
			Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
			return false;
		}
		cur = token+1;
		token = strchr( cur, ')' );
		if( NULL == token )     break;
		*token = 0;
		int itemcount = atol(cur);
		if (itemcount <= 0)
		{
			Log::log(LOG_ERR, "Territory load conf, invalid itemcount %d", itemcount);
			Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
			return false;
		}
		randawardlist.push_back(RandAwardItem(itemid, itemcount));
		cur = token+1;
		token = strchr( cur, '(' );
	}       
	RandAwardList::const_iterator it, ite;
	for (it=randawardlist.begin(),ite=randawardlist.end(); it!=ite; ++it)
		LOG_TRACE("itemid[%d]->count[%d]", it->itemid, it->itemcount);
	//加载原始配置文件
	Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
	return true;
}

bool TerritoryManager::Initialize()
{
	if(domain_data_load())
	{
		LOG_TRACE( "TerritoryManager::Init() load domain data failed" );
		return false;
	}
	if (!LoadConfig())
	{
		LOG_TRACE( "TerritoryManager::Init() load territory conf failed" );
		return false;
	}
	srand(Timer::GetTime());
	UpdateTime();
	IntervalTimer::Attach( this,UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());
	status |= ST_OPEN;
	return true;
}

bool TerritoryManager::RegisterServer(int server, const TerritoryServerVector & fields)
{
	LOG_TRACE( "TerritoryServerRegister: server=%d field_num=%d", server, fields.size());
	TerritoryServerVector::const_iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		ServerMap::const_iterator sit = servers.find(it->map_tag);
		if (sit != servers.end())
			Log::log(LOG_ERR, "TerritoryServerRegister duplicate map_tag %d gsid %d", it->map_tag, server);
		LOG_TRACE( "TerritoryServerRegister: server=%d type=%d", server, it->map_type);
		servers.insert(std::make_pair(it->map_tag, TerritoryTag(server, it->map_type, it->player_limit)));
	}
	status |= ST_GSREADY;
	return true;
}

bool TerritoryManager::LoadTerritoryList(const GTerritoryStore & v)
{
	int count = domain_data_getcount();
	for (int i = 0; i < count; i++)
	{
		DOMAIN_INFO_SERV * p = domain_data_getbyindex(i);
		LOG_TRACE("tid %d", p->id);
	}
	if((int)v.tlist.size()!=count)
	{
		Log::log(LOG_ERR,"TerritoryList load error. load size%d, config size %d", v.tlist.size(), count);
		return false;
	}
	if((status&ST_DATAREADY)&&((int)tlist.size()==count))
	{
		Log::log(LOG_ERR,"TerritoryList already loaded. status=%d", status);
	}
	else
	{
		time_t now = GetTime();
		status |= ((v.status & ST_REAL) | ST_DATAREADY);
		LOG_TRACE("Territory Load status %x", status);
		tlist.clear();
		scoremap.clear();
		fight_announce_set.clear();
		GTerritoryInfoVector::const_iterator it, ite=v.tlist.end();
		for (it = v.tlist.begin(); it != ite; ++it)
		{
			TerritoryInfo territory(*it);
			if (territory._owner)
			{
				if (territory._color == 0 || territory._color > count)
					territory._color = SelectColor(territory._owner);
				scoremap[territory._owner] += type2config[territory.type].score;
			}
			else
				territory._color = 0;
			if (territory._start_time && territory._success_challenge._factionid != 0)
			{
				if (territory._start_time < now + 600)
				{
					territory.status |= TS_CANCEL;
					Log::log(LOG_ERR,"Territory time missed, cancel territoryid=%d defender=%d attacker=%d deposit=%d time=%d status=0x%x", territory._id, territory._owner, territory._success_challenge._factionid, territory._success_challenge._itemcount, territory._start_time, territory.status);
				}
				else
					fight_announce_set.insert(territory._start_time);
			}
			tlist.push_back(territory);
/*
战斗结束之后会重置_start_time 此时如果宕机重启 不应该重新settime
如果宣战阶段中宕机 宣战阶段之后才重启 则不进行settime本周所有领土战取消
			if (territory._attacker && territory._start_time == 0)
				status |= ST_SETTIME;
*/
		}
		LOG_TRACE("load Territory list success, size %d announce size %d", tlist.size(), fight_announce_set.size());
		SyncTerritoryGS();
	}
	return true;
}

void TerritoryManager::OnChallenge(int ret, DBTerritoryChallengeArg *arg, int & itemtotal)
{
	TVector::iterator it, ie = tlist.end();
	for(it=tlist.begin();it!=ie;++it)
	{
		if (it->_id == arg->territoryid)
			break;
	}
	if (it == ie)
	{
		Log::log(LOG_ERR, "territorymanager OnChallenge() can not find tid %d", arg->territoryid);
		return;
	}
	if (!(it->status & TS_BIDBUSY))
	{
		Log::log(LOG_ERR, "territorymanager OnChallenge() tid %d status %x invalid", arg->territoryid, it->status);
		return;
	}
	it->status &= (~TS_BIDBUSY);
	if (ret == ERR_SUCCESS)
	{
		LoadFaction(arg->factionid);
		std::vector<TChallenge>::iterator bit, bite;
		TChallenge challenge(arg->factionid, arg->item_count);
		for (bit=it->_candidate_challenges.begin(),bite=it->_candidate_challenges.end();bit!=bite;++bit)
		{
			if (bit->_factionid == arg->factionid)
			{
				challenge._itemcount += bit->_itemcount;
				it->_candidate_challenges.erase(bit);
				break;
			}
		}
		for (bit=it->_candidate_challenges.begin(),bite=it->_candidate_challenges.end();bit!=bite;++bit)
		{
			if (challenge._itemcount > bit->_itemcount)
				break;
		}
		itemtotal = challenge._itemcount;
		it->_candidate_challenges.insert(bit, challenge);
		Log::formatlog("territorychallenge", "tid %d fid %d itemcount %d itemtotal %d", it->_id, arg->factionid, arg->item_count, challenge._itemcount);
		LOG_TRACE("challengelist map_id %d size %d", it->_id, it->_candidate_challenges.size());
		for (bit=it->_candidate_challenges.begin(),bite=it->_candidate_challenges.end();bit!=bite;++bit)
			LOG_TRACE("factionid %d itemcount %d", bit->_factionid, bit->_itemcount);
	}
}

void TerritoryManager::ItemGet(GTerritoryItemGetArg * arg, GTerritoryItemGetRes * res)
{
	if (!FactionManager::Instance()->IsMaster(arg->factionid, arg->roleid))
	{
		res->retcode = ERR_TERRI_GETITEM_NOTMASTER;
		return;
	}
	if (arg->item_count < 1)
	{
		res->retcode = ERR_TERRI_GETITEM_NUM;
		return;
	}
	switch(arg->itemtype)
	{
	case GET_ASSIST:
	{
		//有效:itemid itemcount 无效:territoryid
		if (!(status & ST_ASSISGET))
		{
			res->retcode = ERR_TERRI_GETITEM_TIME;
			return;
		}
		if (arg->itemid != ASSIS_ITEMID)
		{
			res->retcode = ERR_TERRI_GETITEM_ITEMID;
			return;
		}
		int num_candraw = 0;
		TVector::iterator it, ite = tlist.end();
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (it->_defender==arg->factionid && it->_success_challenge._factionid!=0 && it->_assis_drawn_num<type2config[it->type].assis_num)
			{
				num_candraw += (type2config[it->type].assis_num-it->_assis_drawn_num);
				if (num_candraw >= arg->item_count)
					break;
			}
		}
		if (num_candraw < arg->item_count)
		{
			res->retcode = ERR_TERRI_GETITEM_NUM;
			return;
		}
		int num = arg->item_count;
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (it->_defender==arg->factionid && it->_success_challenge._factionid!=0 && it->_assis_drawn_num<type2config[it->type].assis_num)
			{
				if (num <= (type2config[it->type].assis_num-it->_assis_drawn_num))
				{
					it->_assis_drawn_num+=num;
					num = 0;
				}
				else
				{
					num -= (type2config[it->type].assis_num-it->_assis_drawn_num);
					it->_assis_drawn_num = type2config[it->type].assis_num;
				}
				if (num <= 0)
					break;
			}
		}
		res->retcode = ERR_SUCCESS;
		res->itemid = ASSIS_ITEMID;
		res->item_count = arg->item_count;
		SaveTerritoryList(SAVELIST_REASON_GETASSIST);
		return;
	}
	break;
	case GET_FIGHT_SUCCESS_REWARD:
	{
		//有效:itemid itemcount money
		if (status&(ST_BID|ST_ASSISGET|ST_FIGHT))
		{
			res->retcode = ERR_TERRI_GETITEM_TIME;
			return;
		}
		if (arg->itemid != BID_ITEMID)
		{
			res->retcode = ERR_TERRI_GETITEM_ITEMID;
			return;
		}
		if (arg->item_count <= 0 && arg->money <= 0)
		{
			res->retcode = ERR_TERRI_GETITEM_NUM;
			return;
		}
		int item_total = 0, money_total = 0;
		TVector::iterator it, ite = tlist.end();
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (arg->factionid == it->_owner && it->_success_award > FLOAT_ZERO)
			{
				int count = (int)(it->_success_award);
				int money = (int)((it->_success_award+FLOAT_ZERO - count)*ITEM_TO_MONEY);
				money = (money/1000000)*1000000; //取整 防止出现 money为200 00 07 情况
				item_total += count;
				money_total += money;
			}
		}
/* 攻城帮派可能解散
		if (it->_success_challenge._factionid == 0 || it->_success_challenge._itemcount <= 0)
		{
			res->retcode = ERR_TERRI_GETITEM_PEACE;
			return;
		}
*/
		if (arg->item_count != item_total || arg->money != money_total)
		{
			Log::log(LOG_ERR, "Territory Get success reward count %d:%d money %d:%d", arg->item_count,
								item_total, arg->money, money_total);
			res->retcode = ERR_TERRI_GETITEM_NUM;
			return;
		}
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (arg->factionid == it->_owner && it->_success_award > FLOAT_ZERO)
			{
				if (it->_success_challenge._factionid != it->_owner)
					UnLoadFaction(it->_success_challenge._factionid);
				it->_success_challenge.Reset();
				it->_success_award = 0.0f;
			}
		}
		res->retcode = ERR_SUCCESS;
		res->itemid = BID_ITEMID;
		res->item_count = arg->item_count;
		res->money = arg->money;
		SaveTerritoryList(SAVELIST_REASON_SUCCESSREWARD);
		return;
	}
	break;
	case GET_BID_FAIL_REWARD:
	{
		//有效:itemid itemcount money territoryid
		if (status&ST_BID)
		{
			res->retcode = ERR_TERRI_GETITEM_TIME;
			return;
		}
		if (arg->itemid != BID_ITEMID)
		{
			res->retcode = ERR_TERRI_GETITEM_ITEMID;
			return;
		}
		TVector::iterator it, ite = tlist.end();
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (it->_id == arg->territoryid)
				break;
		}
		if (it == tlist.end())
		{
			res->retcode = ERR_TERRI_NOTFOUND;
			return;
		}
		std::vector<TChallenge>::iterator cit, cite = it->_candidate_challenges.end();
		for (cit = it->_candidate_challenges.begin(); cit != cite; ++cit)
		{
			if (cit->_factionid == arg->factionid)
				break;
		}
		if (cit == cite)
		{
			res->retcode = ERR_TERRI_GETITEM_NOTBIDDER;
			return;
		}
		if (arg->item_count != cit->_itemcount)
		{
			res->retcode = ERR_TERRI_GETITEM_NUM;
			return;
		}
		UnLoadFaction(cit->_factionid);
		it->_candidate_challenges.erase(cit);
		res->retcode = ERR_SUCCESS;
		res->itemid = BID_ITEMID;
		res->item_count = arg->item_count;
		SaveTerritory(SAVE_REASON_BIDFAIL, it->SaveToDB());
		return;
	}
	break;
	case GET_RAND_AWARD:
	{
		//有效:itemid itemcount territoryid
		if (status&(ST_BID|ST_ASSISGET|ST_FIGHT))
		{
			res->retcode = ERR_TERRI_GETITEM_TIME;
			return;
		}
		TVector::iterator it, ite = tlist.end();
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (it->_id == arg->territoryid)
				break;
		}
		if (it == tlist.end())
		{
			res->retcode = ERR_TERRI_NOTFOUND;
			return;
		}
		if (arg->factionid != it->_owner)
		{
			res->retcode = ERR_TERRI_GETITEM_NOTWINNER;
			return;
		}
		if (it->_rand_award_itemid == 0 || it->_rand_award_itemcount <= 0
			|| it->_rand_award_drawn == RAND_AWARD_DRAWN)
		{
			res->retcode = ERR_TERRI_GETITEM_NORAND;
			return;
		}
		if (arg->itemid != it->_rand_award_itemid)
		{
			res->retcode = ERR_TERRI_GETITEM_ITEMID;
			return;
		}
		if (arg->item_count != it->_rand_award_itemcount)
		{
			res->retcode = ERR_TERRI_GETITEM_NUM;
			return;
		}
		it->_rand_award_drawn = RAND_AWARD_DRAWN;
		res->retcode = ERR_SUCCESS;
		res->itemid = arg->itemid;
		res->item_count = arg->item_count;
		SaveTerritory(SAVE_REASON_RANDAWARD, it->SaveToDB());
		return;
	}
	break;
	default:
	{
		res->retcode = ERR_TERRI_GETITEM_INVALIDTYPE;
		return;
	}
	break;
	}
}

time_t TerritoryManager::GetTime()
{
	time_t now = Timer::GetTime();
	return now + t_forged;
}

void TerritoryManager::SetForgedTime(time_t forge)
{
	t_forged = forge;
}

void TerritoryManager::BeginChallenge()
{
	if (!(status & ST_DATAREADY))
		return;
	LOG_TRACE("territory begin challenge status %x list size %d", status, tlist.size());
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		it->status = 0;
		it->_start_time = 0;
		it->timeout = 0;
		it->_defender = 0;
		if (it->_success_challenge._factionid)
		{
			if (it->_success_challenge._factionid != it->_owner)
				UnLoadFaction(it->_success_challenge._factionid);
			it->_success_challenge.Reset();
		}
		std::vector<TChallenge>::iterator cit, cite = it->_candidate_challenges.end();
		for (cit = it->_candidate_challenges.begin(); cit != cite; ++cit)
		{
			Log::formatlog("clearterritorychallenge","tid %d challenger %d itemcount %d", it->_id, cit->_factionid, cit->_itemcount);
			UnLoadFaction(cit->_factionid);
		}
		it->_candidate_challenges.clear();
		it->_success_award = 0.0f;
		it->_rand_award_itemid = 0;
		it->_rand_award_itemcount = 0;
		it->_rand_award_drawn = RAND_AWARD_UNDRAWN;
	}
	SaveTerritoryList(SAVELIST_REASON_BIDBEGIN);
}

void TerritoryManager::SaveTerritoryList(int reason)
{
	DBTerritoryListSaveArg arg;
	arg.reason = reason;
	arg.store.status = status;
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
		arg.store.tlist.push_back(it->SaveToDB());
	GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_DBTERRITORYLISTSAVE, arg));
}

void TerritoryManager::RecordFactionDynamic()
{
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		if (it->_success_challenge._factionid)
		{
			FAC_DYNAMIC::terri_ready log = {it->_id, it->_start_time};
			FactionManager::Instance()->RecordDynamic(it->_success_challenge._factionid, FAC_DYNAMIC::TERRI_READY, log);
		}
	}
}

void TerritoryManager::EndChallenge()
{
	LOG_TRACE("territory end challenge status %x listsize %d", status, tlist.size());
	bool challenged = false;
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		if (it->_candidate_challenges.size())
		{
			challenged = true;
			it->_defender = it->_owner;
			it->_success_challenge = it->_candidate_challenges.front();
			it->_candidate_challenges.erase(it->_candidate_challenges.begin());
			it->_assis_drawn_num = 0;
		}
	}
	if (!challenged) //无人宣战
		return;
	ArrangeTerritory();
	RecordFactionDynamic();
	SaveTerritoryList(SAVELIST_REASON_BIDEND);
}

void TerritoryManager::BeginFight()
{
	LOG_TRACE("Fight time begin ! status %x listsize %d", status, tlist.size());
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		it->_assis_drawn_num = type2config[it->type].assis_num;
	}
	SaveTerritoryList(SAVELIST_REASON_FIGHTBEGIN);
}

void TerritoryManager::EndFight()
{
	std::vector<int> index_list;
	int i = 0;
	for (TVector::iterator it=tlist.begin(), ie=tlist.end(); it!=ie; ++it,i++)
	{
		if (it->_owner != 0)
			index_list.push_back(i);
		if (it->status & TS_FIGHTING)
		{
			Log::log(LOG_ERR, "Territory EndFight cancel tid %d", it->_id);
			it->status &= (~TS_FIGHTING);
			it->status |= TS_CANCEL;
			it->timeout = 0;
			//下一轮UpdateTerritory会SaveDB
		}
	}
	if (index_list.size() == 0)
	{
		LOG_TRACE("Fight time end ! no territory is occupied");
		return;
	}
	int terri_index = index_list[rand()%index_list.size()];
	TerritoryInfo & award_terri = tlist[terri_index];
	int award_index = rand()%randawardlist.size();
	award_terri._rand_award_itemid = randawardlist[award_index].itemid;
	award_terri._rand_award_itemcount = randawardlist[award_index].itemcount;
	Thread::HouseKeeper::AddTimerTask(new TerritoryAwardAnnounceTask(award_terri._id, award_terri._rand_award_itemid), 0);
	SaveTerritory(SAVE_REASON_RANDAWARD, award_terri.SaveToDB());
	LOG_TRACE("Fight time end ! status %x listsize %d awardterritory %d itemid %d itemcount %d", status, tlist.size(), award_terri._id, award_terri._rand_award_itemid, award_terri._rand_award_itemcount);
}

time_t TerritoryManager::UpdateTime()
{
	time_t now = GetTime();
	LOG_TRACE("Territory status %x Timer update: (%d) %s", status, t_forged, ctime(&now));
#ifdef TERRI_DEBUG
	if (now-t_base>3600 || now<t_base)
	{
		struct tm dt;
		localtime_r (&now, &dt);
		dt.tm_sec = 0;
/*
		if (dt.tm_min < 30)
			dt.tm_min = 0;
		else
			dt.tm_min = 30;
*/
		dt.tm_min = 0;
		t_base = mktime(&dt);
	}
#else
	if(now-t_base>604800)
	{
		struct tm dt;
		localtime_r(&now, &dt);
		dt.tm_sec = 0;
		dt.tm_min = 0;
		dt.tm_hour = 0;
		//周一凌晨零点
		if (dt.tm_wday == 0)
			t_base = mktime(&dt)-86400*6;
		else
			t_base = mktime(&dt)-86400*(dt.tm_wday-1);
	}
#endif
	return now;
}

bool TerritoryManager::UpdateStatus(time_t now)
{
	bool status_changed = false;
	if (now >= BidBeginTime() && now < BidEndTime())
	{
		if (!(status & ST_BID))
		{
			BeginChallenge();
			status |= ST_BID;
			status_changed = true;
		}
	}
	else
	{
		if (status & ST_BID)
		{
			EndChallenge();
			status &= (~ST_BID);
			status_changed = true;
		}
	}
	if (now >= BidAnnounceBegin() && now < BidAnnounceEnd())
		status |= ST_BIDANNOUNCE;
	else    
	{
		status &= (~ST_BIDANNOUNCE);
		add_task = false;
	}
	if (now >= BidEndTime() && now < FightBeginTime())
		status |= ST_ASSISGET;
	else
		status &= (~ST_ASSISGET);
	if (now >= FightBeginTime() && now < FightEndTime())
	{
		if (!(status & ST_FIGHT))
		{
			BeginFight();
			status |= ST_FIGHT;
			status_changed = true;
		}
	}
	else
	{
		if (status & ST_FIGHT)
		{
//			TerritoryReset();
			EndFight();
			status &= (~ST_FIGHT);
			status_changed = true;
		}
	}
	if (now >= FightEndTime() && now < AwardAnnounceEnd())
		status |= ST_AWARDANNOUNCE;
	else    
		status &= (~ST_AWARDANNOUNCE);
	return status_changed;	
}

void TerritoryManager::SaveTerritory(int reason, const GTerritoryInfo & info)
{
	
	DBTerritorySaveArg arg(reason, info);
	GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_DBTERRITORYSAVE,arg));
}

void TerritoryManager::UpdateTerritory(time_t now)
{
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		if(it->status&TS_CANCEL)
		{
			Log::log(LOG_ERR,"Territory Cancel territoryid=%d:time=%d:result=4:defender=%d:attacker=%d:attacker_deposit=%d:map_tag=%d", 
					it->_id, now, it->_defender, it->_success_challenge._factionid, it->_success_challenge._itemcount, it->map_tag);
			it->status &= (~TS_CANCEL);
			it->_start_time = 0;
			if (it->_success_challenge._factionid != 0 && it->_success_challenge._itemcount > 0)
			{
				std::vector<TChallenge>::iterator bit, bite;
				for (bit=it->_candidate_challenges.begin(),bite=it->_candidate_challenges.end();bit!=bite;++bit)
				{
					if (it->_success_challenge._itemcount > bit->_itemcount)
						break;
				}
				it->_candidate_challenges.insert(bit, it->_success_challenge);
				it->_defender = 0;
				it->_success_challenge.Reset();
			}
			it->team_defender.clear();
			it->team_attacker.clear();
			FreeMapTag(it->map_tag);
			it->map_tag = 0;
			SaveTerritory(SAVE_REASON_CANCEL, it->SaveToDB());
		}
		else if(it->status&TS_SENDSTART)
		{
			LOG_TRACE("territory id %d send start!!!!", it->_id);
			if(it->timeout && it->timeout <= now)
			{
				Log::log(LOG_ERR,"Start Territory timeout, cancel territoryid=%d", it->_id);
				it->status &= (~TS_SENDSTART);
				it->status |= TS_CANCEL;
				it->timeout = 0;
			}
		}
		else if(it->status&TS_FIGHTING)
		{
			LOG_TRACE("territory id %d map %d fighting!!!!", it->_id, it->map_tag);
			if(it->timeout && it->timeout <= now)
			{
				Log::log(LOG_ERR,"Territory fighting timeout, cancel cityid=%d", it->_id);
				it->status &= (~TS_FIGHTING);
				it->status |= TS_CANCEL;
				it->timeout = 0;
			}
		}
		else if(it->_start_time && it->_success_challenge._factionid != 0 && it->_start_time < now)
		{
			int map_tag = 0;
			int gs = 0;
			if (!AllocMapTag(it->type, it->_id, map_tag, gs))
			{
				Log::log(LOG_ERR, "Territory alloc map for territoryid %d failed, type=%d", it->_id, it->type);
				it->status |= TS_CANCEL;
			}
			else
			{
				it->status |= TS_SENDSTART;
				it->timeout = now + 60;
				SendTerritoryStart start;
				start.territoryid = it->_id;
				start.map_tag = map_tag;
				start.defender = it->_defender;
				start.attacker = it->_success_challenge._factionid;
#ifdef TERRI_DEBUG
				start.end_time = now + 1980;
#else
				start.end_time = now + type2config[it->type].battle_last_time;
#endif
				LOG_TRACE("TerritoryManager: start Territory on server %d, Territory=%d defender=%d attacker=%d map_tag=%d end_time=%d\n",
					gs, start.territoryid, start.defender, start.attacker, start.map_tag, start.end_time);
				GProviderServer::GetInstance()->DispatchProtocol(gs, start);
				it->map_tag = map_tag;
			}
		}
		if (it->_start_time && (it->_start_time-now) < ENTER_REMIND_AHEAD && (it->_start_time-now) >= (ENTER_REMIND_AHEAD-UPDATE_INTERVAL))
			it->EnterRemind();
		if (it->_id == 23)
			LOG_TRACE("starttime %d now %d", it->_start_time, now);
	}
}

bool TerritoryManager::Update()
{
	if (!(status & ST_DATAREADY))
		return true;
	time_t now = UpdateTime();
	if (UpdateStatus(now))
		SaveTerritoryList(SAVELIST_REASON_STATUS);
	UpdateTerritory(now);
/*改为通过NPC喊话
	if ((status & ST_BIDANNOUNCE) && !add_task)
	{
		add_task = true;
		Thread::HouseKeeper::AddTimerTask(new TerritoryBidAnnounceTask(TerritoryBidAnnounceTask::ANNOUNCE_BID1), 0);
	}
	if (now > BidAnnounceEnd() && now <= BidAnnounceEnd() + UPDATE_INTERVAL)
		Thread::HouseKeeper::AddTimerTask(new TerritoryBidAnnounceTask(TerritoryBidAnnounceTask::ANNOUNCE_BID2), 0);
	if (now > BidEndNotice() && now <= BidEndNotice() + UPDATE_INTERVAL)
		Thread::HouseKeeper::AddTimerTask(new TerritoryBidAnnounceTask(TerritoryBidAnnounceTask::ANNOUNCE_BID3), 0);
	if (now > BidEndTime() && now <= BidEndTime() + UPDATE_INTERVAL)
		Thread::HouseKeeper::AddTimerTask(new TerritoryBidAnnounceTask(TerritoryBidAnnounceTask::ANNOUNCE_BID4), 0);
*/
#ifdef TERRI_DEBUG
	if ((now > FightBeginTime() && now <= FightBeginTime() + UPDATE_INTERVAL))
		StartAnnounce();
#else
	std::set<time_t>::iterator sit, site = fight_announce_set.end();
	for (sit = fight_announce_set.begin(); sit != site; ++sit)
	{
		if ((*sit - now) < START_ANNOUNCE_AHEAD && (*sit - now) >= (START_ANNOUNCE_AHEAD-UPDATE_INTERVAL))
		{
			StartAnnounce();
			fight_announce_set.erase(sit);
			break;
		}
	}
#endif
	CooldownMap::iterator it = cooldown.lower_bound(cooldown_cursor);
	CooldownMap::iterator ie = cooldown.end();
	for (int i = 0; i<10 && it!=ie; ++i)
	{
		if (it->second+CHALLENGE_COOLTIME < now)
			cooldown.erase(it++);
		else
			++it;
	}
	if (it == ie)
		cooldown_cursor = 0;
	else
		cooldown_cursor = it->first;
	return true;
}

int TerritoryManager::Enter(const GTerritoryEnter & proto, int & dst_gs, int & map_tag, PlayerInfo * pinfo)
{
	if (pinfo == NULL)
		return -1;
	if (proto.factionid == 0)
		return ERR_TERRI_LOGIC;
	TVector::iterator it, ite = tlist.end();
	for (it = tlist.begin(); it != ite; ++it)
	{
		if (it->_id == proto.territoryid)
			break;
	}
	if (it == ite)
		return ERR_TERRI_NOTFOUND;
	if (!(status & ST_FIGHT) || !(it->status & TS_FIGHTING))
		return ERR_TERRI_ENTER_NOTSTART;
	if (it->_defender != proto.factionid && it->_success_challenge._factionid != proto.factionid)
		return ERR_TERRI_ENTER_NOTBATTLESIDE;
	if (proto.factionid == pinfo->factionid)
	{
		int now = GetTime();
		if (now < pinfo->jointime + CONDITION_JOINTIME)
			return ERR_TERRI_ENTER_JOINTIME;
		FamilyInfo * family = FamilyManager::Instance()->Find(pinfo->familyid);
		if (now < family->info.jointime + CONDITION_JOINTIME)
			return ERR_TERRI_ENTER_JOINTIME;
	}
	if (it->team_defender.find(proto.roleid) != it->team_defender.end())
		return ERR_TERRI_ENTER_ALREADYIN;
	if (it->team_attacker.find(proto.roleid) != it->team_attacker.end())
		return ERR_TERRI_ENTER_ALREADYIN;
	std::set<int> * team; 
	if (it->_defender == proto.factionid)
		team = &(it->team_defender);
	else
		team = &(it->team_attacker);
	TerritoryTag * ptag = FindMapTag(it->map_tag);
	if (ptag == NULL || ptag->tid != it->_id)
		return ERR_TERRI_OUTOFSERVICE;
	map_tag = it->map_tag;
	dst_gs = ptag->gs_id;
	if ((int)team->size() >= ptag->player_limit)
		return ERR_TERRI_ENTER_NUMLIMIT;
	team->insert(proto.roleid);
	return ERR_SUCCESS;
}

int TerritoryManager::Leave(const GTerritoryLeave & proto)
{
	TVector::iterator it, ite = tlist.end();
	for (it = tlist.begin(); it != ite; ++it)
	{
		if (it->_id == proto.territoryid)
			break;
	}
	if (it == ite)
		return ERR_TERRI_NOTFOUND;
	if ((it->status&TS_FIGHTING) && it->team_defender.find(proto.roleid) == it->team_defender.end()
		&& it->team_attacker.find(proto.roleid) == it->team_attacker.end())
		return ERR_TERRI_LEAVE_NOTIN;
	return ERR_SUCCESS;
}

void TerritoryManager::OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map, char & role)
{
	//进城战
	if (IsTerritoryServer(new_gs, new_map))
	{
		TVector::iterator it, ite = tlist.end();
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (it->map_tag == new_map)
			{
				if (it->team_defender.find(roleid) != it->team_defender.end())
				{
					role = ROLE_DEFENDER;
					if (retcode != ERR_CHGS_SUCCESS)
					{
						LOG_TRACE("Territory %d defender erase role %d", it->_id, roleid);
						it->team_defender.erase(roleid);
					}
				}
				if (it->team_attacker.find(roleid) != it->team_attacker.end())
				{
					role = ROLE_ATTACKER;
					if (retcode != ERR_CHGS_SUCCESS)
					{
						LOG_TRACE("Territory %d attacker erase role %d", it->_id, roleid);
						it->team_attacker.erase(roleid);
					}
				}
				break;
			}
		}
		LOG_TRACE("Territory Enter ChangeGS old gs(%d):tag(%d) new gs(%d):tag(%d) role %d retcode %d role %d", old_gs, old_map, new_gs, new_map, roleid, retcode, role);
	}
	if (IsTerritoryServer(old_gs, old_map))
	{
		LOG_TRACE("Territory Leave ChangeGS old gs(%d):tag(%d) new gs(%d):tag(%d) role %d retcode %d", old_gs, old_map, new_gs, new_map, roleid, retcode);
		if (retcode == ERR_CHGS_SUCCESS)
		{
			TVector::iterator it, ite = tlist.end();
			for (it = tlist.begin(); it != ite; ++it)
			{
				if (it->map_tag == old_map)
				{
					LOG_TRACE("Territory %d erase role %d", it->_id, roleid);
					it->team_defender.erase(roleid);
					it->team_attacker.erase(roleid);
					break;
				}
			}

		}
	}
}

void TerritoryManager::OnLoginWorld(int roleid)
{
}

void TerritoryManager::OnLogout(int roleid, int gsid, int map)
{
	if (IsTerritoryServer(gsid, map))
	{
		LOG_TRACE("Territory role %d Logout gs %d map %d", roleid, gsid, map);
		TVector::iterator it, ite = tlist.end();
		for (it = tlist.begin(); it != ite; ++it)
		{
			if (it->map_tag == map)
			{
				LOG_TRACE("Territory %d erase role %d", it->_id, roleid);
				it->team_defender.erase(roleid);
				it->team_attacker.erase(roleid);
				break;
			}
		}
	}
}

void TerritoryManager::OnLoginTerritory(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map)
{
	//进城战
	if (IsTerritoryServer(new_gs, new_map))
	{
		LOG_TRACE("Territory Enter Login old gs(%d):tag(%d) new gs(%d):tag(%d) ret %d", old_gs, old_map, new_gs, new_map, retcode);
		if (retcode != ERR_SUCCESS)
		{
			TVector::iterator it, ite = tlist.end();
			for (it = tlist.begin(); it != ite; ++it)
			{
				if (it->map_tag == new_map)
				{
					LOG_TRACE("Territory %d erase role %d", it->_id, roleid);
					it->team_defender.erase(roleid);
					it->team_attacker.erase(roleid);
					break;
				}
			}
		}
	}
}

char TerritoryManager::SelectColor(unsigned int factionid)
{

	if (factionid == 0)
		return 0;
	int count = (char)domain_data_getcount();
	char * colors = (char *)malloc(count+1);
	memset(colors, 0, count+1);
	bool done = false;
	char color = 0;
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie&&!done;++it)
	{
		if(it->_owner == factionid && it->_color > 0)
		{
			color = it->_color;
			done = true;
		}
		if(it->_color < count+1 && it->_color > 0)
			colors[it->_color] = 1;
	}
	for(char i = 1; i < count+1 && !done; i++)
	{
		if(colors[i]==0)
		{
			color = i;
			done = true;
		}
	}
	free(colors);
	return color;
}

void TerritoryManager::SetOwner(TerritoryInfo & info, unsigned int new_owner, bool load)
{
	//必须先SelectColor再设置_owner
	info._color = SelectColor(new_owner);
	if (info._owner)
	{
		int & score_ref = scoremap[info._owner];
		score_ref -= type2config[info.type].score;
		TerritoryScoreUpdate update(info._owner, score_ref);
		FactionManager::Instance()->Broadcast(info._owner, update, update.localsid);
		LOG_TRACE("TerritoryScore broadcast fid=%d score=%d", info._owner, score_ref);
		UnLoadFaction(info._owner);
	}
	info._owner = new_owner;
	if (info._owner)
	{
		int & score_ref = scoremap[info._owner];
		score_ref += type2config[info.type].score;
		info._occupy_time = GetTime();
		TerritoryScoreUpdate update(info._owner, score_ref);
		FactionManager::Instance()->Broadcast(info._owner, update, update.localsid);
		LOG_TRACE("TerritoryScore broadcast fid=%d score=%d", info._owner, score_ref);
		if (load)
			LoadFaction(info._owner);
	}
	else
		info._occupy_time = 0;
}
		
void TerritoryManager::LogTerritoryList()
{
	TVector::const_iterator it, ite = tlist.end();
	for(it=tlist.begin(); it!=ite; ++it)
		Log::formatlog("TerritoryOwner", "tid=%d, owner=%d", it->_id, it->_owner);
}

void TerritoryManager::OnTerritoryEnd(int id, int result)
{
	time_t now = GetTime();
	TVector::iterator it, ite = tlist.end();
	for(it=tlist.begin(); it!=ite; ++it)
	{
		if(it->_id==id)
			break;
	}
	if (it == ite)
	{
		Log::log(LOG_ERR, "OnTerritoryEnd() can not find tid %d", id);
		return;
	}
	if (!(it->status & TS_FIGHTING))
	{
		Log::log(LOG_ERR, "OnTerritoryEnd status %x never start", it->status);
		return;
	}
	Log::formatlog("Territoryend","tid=%d:time=%d:result=%d:defender=%d:attacker=%d:map_tag=%d", 
			 id, now, result, it->_defender, it->_success_challenge._factionid, it->map_tag);
	int time = (now-it->_start_time < type2config[it->type].battle_last_time/2)?1:0;
	it->EndAnnounce(result, time);
	it->status &= (~TS_FIGHTING);
	it->_start_time = 0;
	it->timeout = 0;
	FreeMapTag(it->map_tag);
	it->map_tag = 0;
	if(result==ATTACKER_WIN)
	{
		SetOwner(*it, it->_success_challenge._factionid, false);
		it->_success_award = it->_success_challenge._itemcount * 0.8;
		SyncTerritoryGS();
	}
	else
		it->_success_award = it->_success_challenge._itemcount * 0.5;
	it->team_defender.clear();
	it->team_attacker.clear();
	SaveTerritory(SAVE_REASON_END, it->SaveToDB());
	LogTerritoryList();
}

void TerritoryManager::OnTerritoryStart(int id, int retcode)
{
	TVector::iterator it, ie = tlist.end();
	for(it=tlist.begin(); it!=ie; ++it)
	{
		if (id == it->_id)
			break;
	}
	if (it == ie)
	{
		Log::log(LOG_ERR, "OnTerritoryStart can not find tid %d", id);
		return;
	}
	if (!(it->status & TS_SENDSTART))
	{
		Log::log(LOG_ERR, "OnTerritoryStart tid %d status %x invalid", id, it->status);
		return;
	}

	it->status &= (~TS_SENDSTART);
	if(retcode != ERR_SUCCESS)
	{
		it->status |= TS_CANCEL;
		Log::log(LOG_ERR,"Territory start failed (%d), cancel tid=%d defender=%d attacker=%d", retcode, it->_id, it->_defender, it->_success_challenge._factionid);
		return;
	}
	it->status |= TS_FIGHTING;
	it->timeout = GetTime() + type2config[it->type].battle_last_time + 1800;
	Log::formatlog("Territorystart","tid=%d:time=%d:defender=%d:attacker=%d", it->_id, GetTime(), it->_defender, it->_success_challenge._factionid);
}
//领土战gs掉线
void TerritoryManager::OnDisconnect(int gsid)
{       
	LOG_TRACE("Territory gs %d disconnected", gsid);
	TerritoryTag * ptag = NULL;
	for (TVector::iterator it=tlist.begin(),ite=tlist.end();it!=ite;++it)
	{
		
		if ((it->status | (TS_FIGHTING|TS_SENDSTART))
			&& (ptag=FindMapTag(it->map_tag)) && ptag->tid == it->_id
			&& ptag->gs_id == gsid)
		{
			it->status |= TS_CANCEL;
			it->status &= ~TS_FIGHTING;
			it->status &= ~TS_SENDSTART;
			it->timeout = 0;
		}
	}       
	for (ServerMap::iterator it=servers.begin(),ite=servers.end(); it!=ite;)
	{
		if (it->second.gs_id == gsid)
			servers.erase(it++);
		else
			++it;
	}
	if (servers.size()==0)
		status &= (~ST_GSREADY);
}

int64_t CalcuPrior(unsigned int owner, unsigned int attacker, int tid)
{
	const int64_t OWNER_LEV_PRIOR 	     = 10000000000000LL;//假设帮派等级<=9
	const int64_t OWNER_POPULATION_PRIOR 	=  1000000000LL;//假设帮派人口数<=999
	const int64_t ATTACKER_LEV_PRIOR	=    10000000LL;
	const int64_t ATTACKER_POPULATION_PRIOR	= 	 1000LL;
	const int64_t TID_PRIOR			=	    1LL;//假设领土id<=99

	int64_t prior = 0;
	if (owner != 0)
	{
		FactionDetailInfo *powner = FactionManager::Instance()->Find(owner);
                if (powner == NULL) 
			Log::log(LOG_ERR, "Calculate priority owner faction %d does not load", owner);
		else
		{
			prior += ((powner->info.level+1)*OWNER_LEV_PRIOR
				+ powner->GetMemberCount()*OWNER_POPULATION_PRIOR);
		}
	}
	if (attacker == 0)
		Log::log(LOG_ERR, "Calculate priority attacker is 0");
	else
	{
		FactionDetailInfo *pattacker = FactionManager::Instance()->Find(attacker);
                if (pattacker == NULL) 
			Log::log(LOG_ERR, "Calculate priority attacker faction %d does not load", attacker);
		else
		{
			prior += ((pattacker->info.level+1)*ATTACKER_LEV_PRIOR
				+ pattacker->GetMemberCount()*ATTACKER_POPULATION_PRIOR);
		}
	}
	prior += tid * TID_PRIOR;
	return prior;
}

typedef std::vector<TerritoryManager::TVector::iterator> ItVector;
class TimingSet //每个时段的城池列表
{
	unsigned int max;
	std::map<int, int> battle_map; //每个帮派参与的场次数
public:
	ItVector list;
	int64_t priority;
	TimingSet(int m) : max(m),priority(0){}
	bool Insert(TerritoryManager::TVector::iterator& it, bool force=false)
	{
		if(max<=list.size())
			return false;
		if(!force)
		{
/*
			for(ItVector::iterator i=list.begin();i!=list.end();++i)
				if(it->_owner==(*i)->_success_challenge._factionid || it->_success_challenge._factionid ==(*i)->_owner)
					return false;
*/
			if (it->_owner && battle_map[it->_owner] >= 3)
				return false;
			if (battle_map[it->_success_challenge._factionid] >= 3)
				return false;
		}
		list.push_back(it);
		if (it->_owner)
			battle_map[it->_owner]++;
		battle_map[it->_success_challenge._factionid]++;
		priority += CalcuPrior(it->_owner, it->_success_challenge._factionid, it->_id);
		return true;
	}
	bool TryInsert(ItVector& v, int size)
	{
		if(max-list.size()<(unsigned int)size)
			return false;
		std::map<int, int> tmp_map(battle_map);
		for(ItVector::iterator it=v.begin();it!=v.end()&&size;++it)
		{
			if ((*it)->_owner && tmp_map[(*it)->_owner] >= 3)
				continue;
			if (tmp_map[(*it)->_success_challenge._factionid] >= 3)
				continue;
			if ((*it)->_owner)
				tmp_map[(*it)->_owner]++;
			tmp_map[(*it)->_success_challenge._factionid]++;
			size--;
		}
		return (size==0);
/*
		for(ItVector::iterator it=list.begin();it!=list.end();++it)
			if((*it)->_success_challenge._factionid==owner)
				return false;
		for(ItVector::iterator il=v.begin();il!=v.end()&&size;++il)
		{
			ItVector::iterator it;
			for(it=list.begin();it!=list.end();++it)
				if((*il)->_success_challenge._factionid==(*it)->_owner)
					break;
			if(it==list.end())
				size--;
		}
		return (size==0);
*/
	}
};
struct compare_Priority
{
	bool operator() (const TimingSet& c1,const TimingSet& c2) const { return c1.priority > c2.priority; }
};
class ItSet
{
public:
	ItVector list;
	unsigned int owner;
	int64_t priority;
	ItSet(int o,TerritoryManager::TVector::iterator& it) : owner(o)
	{
		priority = CalcuPrior(it->_owner, it->_success_challenge._factionid, it->_id);
		list.push_back(it);
	}
	bool Add(TerritoryManager::TVector::iterator& it)
	{
		priority += CalcuPrior(it->_owner, it->_success_challenge._factionid, it->_id);
		list.push_back(it);
		return true;
	}
	bool Remove(TerritoryManager::TVector::iterator _it)
	{
		ItVector::iterator it, ite = list.end();
		for (it = list.begin(); it != ite; ++it)
		{
			if (*it == _it)
			{
				LOG_TRACE("Remove duplicate it (tid = %d)", (*it)->_id);
				list.erase(it);
				priority -= CalcuPrior(_it->_owner, _it->_success_challenge._factionid, _it->_id);
				return true;
			}
		}
		return false;
	}
	std::vector<TimingSet>::iterator FindSlot(std::vector<TimingSet>& set,std::vector<TimingSet>::iterator it,int n)
	{
		for(;it!=set.end() && !it->TryInsert(list, n);++it);
		return it;
	}
	void Dump() const
	{
		LOG_TRACE("---TtSet dump: owner %d priority %lld", owner, priority);
		ItVector::const_iterator it, ite = list.end();
		for (it = list.begin(); it != ite; ++it)
			LOG_TRACE("------Tid %d owner %d attacker %d", (*it)->_id, (*it)->_owner, (*it)->_success_challenge._factionid);
	}
};
struct compare_Size
{
	bool operator() (const ItSet& i1,const ItSet& i2) const
	{
		return (i1.list.size()<i2.list.size()) || (i1.list.size()==i2.list.size() && i1.priority<i2.priority);
	}
};
bool TerritoryManager::ArrangeTerritory()
{
#ifdef TERRI_DEBUG
	TVector::iterator it, ite=tlist.end();
	for (it = tlist.begin(); it != ite; ++it)
	{
		if (it->_success_challenge._factionid != 0)
		{
			it->_start_time = FightBeginTime();
			LOG_TRACE("Settime territory %d, %d", it->_id, it->_start_time);
		}
	}
	return true;
#else
	std::vector<ItSet>     sorter;
	std::vector<TimingSet> scheduler;
	ItVector left;
	const std::vector<_BATTLETIME_SERV>& times = getbattletimelist();
	unsigned int i,tsize = times.size(),tmax = getbattletimemax();
	for(i=0;i<tsize;i++)
	{
		if (i == 0)
			scheduler.push_back(TimingSet(8));//第一个时段安排8场
		else
			scheduler.push_back(TimingSet(tmax));
	}
	for(TVector::iterator it=tlist.begin(),ie=tlist.end();it!=ie;++it)
	{
		if(it->_success_challenge._factionid)
		{
			bool attacker_insert = false;
			bool owner_insert = (it->_owner != 0 ? false:true);
			std::vector<ItSet>::iterator its = sorter.begin();
			for (; its != sorter.end(); ++its)
			{
				if (it->_owner && it->_owner == its->owner)
				{
					its->Add(it);
					owner_insert = true;
				}
				if (it->_success_challenge._factionid == its->owner)
				{
					its->Add(it);
					attacker_insert = true;
				}
			}
			if (!attacker_insert)
				sorter.push_back(ItSet(it->_success_challenge._factionid, it));
			if (!owner_insert)
				sorter.push_back(ItSet(it->_owner, it));
/*
			if(it->_owner==0)
				sorter.push_back(ItSet(0,it));
			else
			{
				std::vector<ItSet>::iterator its = sorter.begin();
				for(;its!=sorter.end();++its)
				{
					if(its->owner==it->_owner)
					{
						its->Add(it);
						break;
					}
				}
				if(its==sorter.end())
					sorter.push_back(ItSet(it->_owner,it));
			}
*/
		}
	}
	LOG_TRACE("ItSet before sort");
	std::vector<ItSet>::const_iterator sit, site = sorter.end();
	for (sit = sorter.begin(); sit != site; ++sit)
		sit->Dump();
	std::vector<ItSet>     final_sorter;
	while(sorter.size())
	{
		std::vector<ItSet>::iterator max_it = std::max_element(sorter.begin(), sorter.end(), compare_Size());
		ItSet max_item = *max_it;
		sorter.erase(max_it);
		if (max_item.list.size() == 0)
			continue;
		final_sorter.push_back(max_item);
		ItVector::const_iterator tit, tite = max_item.list.end();
		for (tit = max_item.list.begin(); tit != tite; ++tit)
		{
			std::vector<ItSet>::iterator it2, it2e = sorter.end();
			for (it2 = sorter.begin(); it2 != it2e; ++it2)
				it2->Remove(*tit);
		}
	}
	LOG_TRACE("ItSet after sort");
	site = final_sorter.end();
	for (sit = final_sorter.begin(); sit != site; ++sit)
		sit->Dump();
	std::vector<TimingSet>::iterator ic, ice=scheduler.end();
	for(std::vector<ItSet>::iterator its=final_sorter.begin(),ite=final_sorter.end();its!=ite;++its)
	{
		LOG_TRACE("ItSet owner %d", its->owner);
		unsigned int size = its->list.size();
		ic = scheduler.begin();
		while(size)
		{
			size = its->list.size()>3?3:its->list.size();
			ic = its->FindSlot(scheduler, ic, size);
			if(ic==ice)
				ic = its->FindSlot(scheduler, scheduler.begin(), size);
			if(ic==ice)
				break;
			i = 0;
			LOG_TRACE("FindSlot %d", std::distance(scheduler.begin(), ic));
			for(ItVector::iterator il=its->list.begin();il!=its->list.end()&&i<size;)
			{
				if(ic->Insert(*il))
				{
					LOG_TRACE("--insert success tid %d i %d", (*il)->_id, i);
					i++;
					il = its->list.erase(il);
				}
				else
					++il;
			}
			if(++ic==ice)
			{
				LOG_TRACE("return to begin");
				ic = scheduler.begin();
			}
		}
		left.insert(left.end(),its->list.begin(),its->list.end());
	}
	for(ItVector::iterator it=left.begin();it!=left.end();++it)
	{
		LOG_TRACE("left tid %d", (*it)->_id);
		std::vector<TimingSet>::iterator is=scheduler.begin(),ise=scheduler.end();
		for(;is!=ise&&!is->Insert(*it);++is);
		if(is==ise)
			for(is=scheduler.begin();is!=ise&&!is->Insert(*it,true);++is);
	}
	std::sort(scheduler.begin(),scheduler.end(),compare_Priority());
	i = 0;
	fight_announce_set.clear();
	for(std::vector<TimingSet>::iterator it=scheduler.begin(),ie=scheduler.end();it!=ie;++it,++i)
	{
		const _BATTLETIME_SERV& t = times[i];
		LOG_TRACE("Time size:%d Time[%d] day:%d hour:%d min:%d list.size %d priority=%lld", tsize, i, t.nDay, t.nHour, t.nMinute, it->list.size(), it->priority);
		time_t start_time = t_base + 86400*t.nDay + 3600*t.nHour + 60*t.nMinute;
		for(ItVector::iterator il=it->list.begin();il!=it->list.end();++il)
		{
			(*il)->_start_time = start_time;
			LOG_TRACE("Territory %d set time %s", (*il)->_id, ctime(&start_time));
		}
		if (it->list.size())
			fight_announce_set.insert(start_time);
	}
	LOG_TRACE("Fight Announce set size %d", fight_announce_set.size());
	return true;
#endif
}

bool TerritoryManager::OnDBConnect(Protocol::Manager *manager, int sid)
{
	if((status&ST_OPEN) && !(status&ST_DATAREADY))
	{
		DBTerritoryListLoadArg arg;
		for (int i = 0; i < domain_data_getcount(); i++)
		{
			DOMAIN_INFO_SERV * domain_info = domain_data_getbyindex(i);
			if (domain_info != NULL)
				arg.default_ids.push_back(domain_info->id);
		}
		manager->Send(sid, Rpc::Call(RPC_DBTERRITORYLISTLOAD, arg));
	}
	return true;
}
//与gamedbd逻辑保持一致
void TerritoryManager::OnDelFaction(unsigned int factionid)
{
	bool changed = false;
	for(TVector::iterator it=tlist.begin();it!=tlist.end();++it)
	{
		if(it->_owner==factionid)
		{
			LOG_TRACE("Territory %d owner %d deleted", it->_id, it->_owner);
			scoremap.erase(it->_owner);
			it->_owner = 0;
			it->_occupy_time = 0;
			it->_color = 0;
			changed = true;
		}
		else if (it->_success_challenge._factionid == factionid)
		{
			LOG_TRACE("Territory %d attacker %d deleted", it->_id, it->_success_challenge._factionid);
			it->_success_challenge.Reset();
			it->_start_time = 0;
			changed = true;
		}
		else
		{
			std::vector<TChallenge>::iterator cit, cite = it->_candidate_challenges.end();
			for (cit = it->_candidate_challenges.begin(); cit != cite; ++cit)
			{
				if (cit->_factionid == factionid)
				{
					LOG_TRACE("Territory %d challenger %d deleted", it->_id, cit->_factionid);
					it->_candidate_challenges.erase(cit);
					changed = true;
					break;
				}
			}
		}
	}
	if (changed)
		SyncTerritoryGS();
}

void TerritoryManager::DebugSetOwner(int id, int factionid)
{
	TVector::iterator it=tlist.begin(),ie=tlist.end();
	for(;it!=ie;++it)
	{
		if(it->_id==id)
		{
			SetOwner(*it, factionid, true);
			SyncTerritoryGS();
			SaveTerritory(SAVE_REASON_DEBUG, it->SaveToDB());
			break;
		}
	}
	LogTerritoryList();
}
};

