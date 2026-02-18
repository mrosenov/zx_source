#include "worldmanager.h"
#include <ASSERT.h>
#include <strtok.h>
#include <conf.h>
#include "world.h"
#include "matter.h"
#include "item.h"
#include "task/taskman.h"
#include "player_imp.h"
#include "npcgenerator.h"
#include "template/globaldataman.h"
#include "pathfinding/pathfinding.h"
#include "petdataman.h"
#include "ai/policy_loader.h"
#include "global_drop.h"
#include "gmatrix.h"
#include <gsp_if.h>
#include <meminfo.h>
#include "battleground/bg_world_manager.h"
#include "commondata_if.h"
#include "general_indexer_cfg.h"

bool 
world_manager::InitWorldLimit(const char * servername)
{
	memset(&_world_limit,0,sizeof(_world_limit));
	ONET::Conf *conf = ONET::Conf::GetInstance();
	std::string limit = conf->find(servername ,"limit").c_str();
	abase::strtok tok(limit.c_str(),";,\r\n");
	const char * token;
	while((token = tok.token()))
	{       
		if(!*token) continue;
		if(strcmp(token,"nothrow") == 0)
		{
			_world_limit.nothrow = true;
		}
		else if(strcmp(token,"allow-root") == 0)
		{
			_world_limit.allowroot = true;
		}
		else if(strcmp(token,"use-save-point") == 0)
		{
			_world_limit.savepoint = true;
		}
		else if(strcmp(token,"nomount") == 0)
		{
			_world_limit.nomount = true;
		}
		else if(strcmp(token,"gm-free") == 0)
		{
			_world_limit.gmfree = true;
		}
		else if(strcmp(token,"noduel") == 0)
		{
			_world_limit.noduel = true;
		}
		else if(strcmp(token,"no-bind") ==0)
		{
			_world_limit.nobind = true;
		}
		else if(strcmp(token,"no-fly") ==0)
		{
			_world_limit.nofly = true;
		}
		else if(strcmp(token,"no-couple-jump") == 0)
		{
			_world_limit.nocouplejump = true;
		}
		else if(strcmp(token,"no-trade") == 0)
		{
			_world_limit.notrade = true;
		}
		else if(strcmp(token,"no-market") == 0)
		{
			_world_limit.nomarket = true;
		}
		else if(strcmp(token,"no-shop") == 0)
		{
			_world_limit.noshop = true;
		}
		else if(strcmp(token,"can-use-battleground-potion") == 0)
		{
			_world_limit.can_use_battleground_potion = true;
		}
		else if(strcmp(token,"can-use-battleground-skill-mater") == 0)
		{
			_world_limit.can_use_battleground_skill_mater = true;
		}
		else if(strcmp(token,"faction-team") == 0)
		{
			_world_limit.faction_team = true;
		}
		else if(strcmp(token, "no-drug") == 0)
		{
			_world_limit.nodrug = true;
		}
		else if(strcmp(token, "no-pet-summon") == 0)
		{
			_world_limit.nopetsummon = true;
		}
		else if(strcmp(token, "no-pet-combine") == 0)
		{
			_world_limit.nopetcombine = true;	
		}
		else if(strcmp(token, "no-pet-food") == 0)
		{
			_world_limit.nopetfood = true;	
		}
		else if(strcmp(token, "no-longjump-in") == 0)
		{
			_world_limit.no_longjump_in = true;
		}
		else if (strcmp(token,"no-fly-task") == 0)
		{
			_world_limit.no_flytask = true;
		}
		else if (strcmp(token,"need-deity") == 0)
		{
			_world_limit.need_deity = true;
		}
	}
	
	//跨服服务器的特殊限制
	if(gmatrix::IsZoneServer())
	{
		_world_limit.nothrow = true;
		_world_limit.nomarket = true;
		_world_limit.noshop = true;
		_world_limit.notrade = true;
	}
	return true;
}
	
void world_manager::SetWorldIndex(int world_index) 
{
	ASSERT(world_index >= 0);
	_world_index =  world_index;
}

bool 
world_manager::InitTerrain(std::string base_path, std::string section,const rect & rt)
{
	ONET::Conf *conf = ONET::Conf::GetInstance();

	TERRAINCONFIG config;
	config.nAreaWidth = atoi(conf->find(section,"nAreaWidth").c_str());
	config.nAreaHeight = atoi(conf->find(section,"nAreaHeight").c_str());
	config.nNumAreas = atoi(conf->find(section,"nNumAreas").c_str());
	config.nNumCols = atoi(conf->find(section,"nNumCols").c_str());
	config.nNumRows =atoi(conf->find(section,"nNumRows").c_str());
	config.vGridSize = atof(conf->find(section,"vGridSize").c_str());
	config.vHeightMin = atof(conf->find(section,"vHeightMin").c_str());
	config.vHeightMax = atof(conf->find(section,"vHeightMax").c_str());
	memset(config.szMapPath,0,sizeof(config.szMapPath));
	std::string path = base_path + conf->find(section,"szMapPath");
	strncpy(config.szMapPath, path.c_str(),sizeof(config.szMapPath) - 1);

	ASSERT(rt.left <= rt.right && rt.top <= rt.bottom);
	if(!path_finding::InitTerrain(_movemap, config,rt.left,rt.top,rt.right,rt.bottom))
	{
		printf("无法初始化地形数据\n");
		return false;
	}
	printf("terrain data loaded\n");
	return true;
}

int 
world_manager::PlaneSwitch(gplayer_imp * pImp,const A3DVECTOR & pos,int tag,const instance_key & key, size_t fee)
{
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	ASSERT(pPlayer->spinlock);
	world_manager*  manager = gmatrix::FindWorld(tag);
	if(manager == NULL || manager->IsIdle()) return -1;

	//对于跳转是副本的检查，只允许在副本内部传送
	if(IsRaidWorld() || manager->IsRaidWorld())
	{
		if(manager->GetRaidID() == GetRaidID() && tag == _world_tag)
		{
			if(fee)
			{
				pImp->SpendMoney(fee);
				pImp->_runner->spend_money(fee);
			}
			pImp->LongJump(pos);
			return 0;
		}
		else
		{
			__PRINTF( "禁止从副本跳出或跳入\n" );
			return -1;
		}
	}
	//liuyue-facbase
	if (IsFacBase() || manager->IsFacBase())
	{
		if (manager->GetClientTag() == GetClientTag() && tag == _world_tag)
		{
			if (fee)
			{
				pImp->SpendMoney(fee);
				pImp->_runner->spend_money(fee);
			}
			pImp->LongJump(pos);
			return 0;
		}
		else
		{
			__PRINTF( "禁止从基地跳出或跳入\n" );
			return -1;
		}
	}
	if(tag == _world_tag && pImp->_plane->PosInWorld(pos))
	{	
		// 是自己
		if(fee)
		{
			pImp->SpendMoney(fee);
			pImp->_runner->spend_money(fee);
		}
		pImp->LongJump(pos);
		return 0;
	}

	// 如果自己是战场地图,目标也是战场地图,不让跳
	if( IsBattleWorld() && manager->IsBattleWorld() )
	{
		__PRINTF( "跳跃地图和目标地图都是战场,不让跳啦\n" );
		return -1;
	}

	//进行是否可以进行副本重置的检测
	int ins_timer = pImp->CheckInstanceTimer(tag);
	
	//减少金钱
	if(fee)
	{
		pImp->SpendMoney(fee);
		pImp->_runner->spend_money(fee);
	}
	instance_key key2 = key;

	MSG msg;
	BuildMessage(msg,GM_MSG_PLANE_SWITCH_REQUEST,XID(GM_TYPE_MANAGER,tag),pImp->_parent->ID,pos,
			ins_timer,&key2,sizeof(key2));
	gmatrix::SendWorldMessage(tag, msg);

	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("%d开始转移服务器:%u.%u\n",pPlayer->ID.id,(size_t)tv.tv_sec,(size_t)tv.tv_usec);
	return 0;
}

namespace 
{
	template <typename T>
	bool VeriyClass(int guid)
	{
		ClassInfo * pInfo;
		pInfo = ClassInfo::GetRunTimeClass(guid);
		return pInfo && pInfo->IsDerivedFrom(T::GetClass());
	}
}

bool 
world_manager::player_cid::Init(const char * str)
{
	abase::strtok tok(str, ";, \r\n");
	int index = 0;
	const char * ch;
	while( (ch = tok.token()))
	{
		cid[index] = atoi(ch);
		index ++;
		if(index == 3) break;
	}
	if(index != 3) return false;

	if(!VeriyClass<gplayer_imp>(cid[0])) return false;
	if(!VeriyClass<gplayer_controller>(cid[1])) return false;
	if(!VeriyClass<gplayer_dispatcher>(cid[2])) return false;
	return true;
}

void 
world_manager::Heartbeat()
{
}

int
world_manager::InitBase(const char * section)
{
	world_manager::InitWorldLimit(section);
	if(!_world_limit.allowroot && (getuid() == 0 || geteuid()==0))
	{
		printf("目前的设置不允许root启动程序\n");
		return -124;
	}

	ONET::Conf *conf = ONET::Conf::GetInstance();

	std::string root = conf->find("Template","Root");

	//取得可能存在的存盘点
	std::string str = conf->find(section,"save_point");
	{
		int tag = -1;
		A3DVECTOR pos;
		sscanf(str.c_str(),"%d,%f,%f,%f",&tag,&pos.x,&pos.y,&pos.z);
		if(tag <=0) tag = -1;
		_save_point.tag = tag;
		_save_point.pos = pos;
	}

	//设置世界索引和世界标记
	SetWorldIndex(atoi(conf->find(section,"index").c_str()));
	SetWorldTag(atoi(conf->find(section,"tag").c_str()));

	//读取最大视野距离
	float mmsr = atof(conf->find(section,"max_sight_range").c_str());
	if(mmsr < 1.0f) mmsr = DEFAULT_AGGRESSIVE_RANGE;
	_max_mob_sight_range = mmsr;

	float mvr = atof(conf->find(section, "max_visible_range").c_str());
	if(mvr < 10.f) mvr = DEFAULT_GRID_SIGHT_RANGE;
	_max_visible_range =  mvr;

	printf("可见范围:%f\n", _max_visible_range);
	
	std::string trace_file = conf->find("Template","CollisionFile");
	if(strlen(trace_file.c_str()) && ( GetMemTotal() >3*1024*1024 ||player_template::GetDebugMode()) )   //$$$$$$$ 这个是否应该用参数控制 
	{
		std::string base_path;
		base_path = root + conf->find(section,"base_path");
		std::string trace_path = base_path + trace_file;
		/*
		if(IsRaidWorld())
		{
			raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
			if(!rwt)
			{
				printf("副本加载共享凸包数据失败, 副本模板读取失败%d\n", GetRaidID());
				return -125;
			}
			_trace_man.Attach(&rwt->GetTraceMan());
		}
		*/
		if (!OnTraceManAttach())
		{
			printf("实例加载共享凸包数据失败\n");
			return -126;
		}
		if(!_trace_man.Load(trace_path.c_str()))
		{
			printf("加载凸包数据'%s'失败\n",trace_path.c_str());
			return -125;
		}
		else
		{

			printf("加载'%s'完成\n",trace_path.c_str());
		}
	}
	return 0;
}

bool
world_manager::InitMoveMap(const char* ground, const char* water, const char* air)
{
	_movemap = path_finding::InitMoveMap(ground, water, air);
	return _movemap;
}

bool
world_manager::InitRegionData(const char * precinct_path,const char * region_path)
{
	if(!_region)
	{
		_region = new city_region;
	}
	_region->SetWorldTag(_world_tag);
	return _region->InitRegionData(precinct_path,region_path);
}

bool world_manager::ReleasePathMan()
{
	if(_pathman)
	{
		if(!OnReleasePathMan())
		{
			delete _pathman;
		}
		_pathman = 0;
	}
	return true;
}

bool world_manager::ReleaseMoveMap()
{
	if(_movemap)
	{
		if(!OnReleaseMoveMap())
		{
			delete _movemap;
		}
		_movemap = NULL;
	}
	return true;
}

bool world_manager::ReleaseRegionData()
{
	if(_region)
	{
		if(!OnReleaseCityRegion())
		{
			delete _region;
		}
		_region = NULL;
	}
	return true;
}

bool world_manager::ReleaseTraceMan()
{
	_trace_man.Release();
	return true;
}

bool world_manager::ReleaseBuffArea()
{
	if(_buffarea)
	{
		if(!OnReleaseBuffArea())
		{
			delete _buffarea;
		}
		_buffarea = NULL;
	}
	return true;
}

bool 
world_manager::GetTownPosition(gplayer_imp *pImp, const A3DVECTOR &opos, A3DVECTOR &pos, int & world_tag)
{
	return _region && _region->GetCityPos(opos.x,opos.z,pos,world_tag);
}

void 
world_manager::RecordMob(int type, int tid, const A3DVECTOR &pos,int faction,int cnt)
{
	if(type == 1)
	{
		//只记录NPC
		w_service_npc_list[tid] = pos;
	}
	else
	{
		w_normal_mobs_list[tid] = pos;
	}
}

int 
world_manager::SwitchPlayerFrom(world_manager *from, gplayer * pPlayer, const A3DVECTOR & pos, const instance_key & ins_key)
{
	return -1;
}

void 
world_manager::SwitchPlayerTo(int uid, int desttag, const A3DVECTOR & pos, const instance_key & ins_key)
{
	int world_index = 0;
	gplayer * pPlayer = gmatrix::FindPlayer(uid, world_index);
	if(!pPlayer)
	{
		//无法找到玩家，因此也无法进行转移操作
		return ;
	}
	
	spin_autolock keeper(pPlayer->spinlock);

	//进行玩家的基础校验
	if(!pPlayer->IsActived() || pPlayer->tag != GetWorldTag()  ||
			pPlayer->imp == NULL || pPlayer->plane == NULL || 
			pPlayer->login_state != gplayer::LOGIN_OK)

	{
		return ;
	}

	//查找目标世界
	world_manager * target = gmatrix::FindWorld(desttag);
	if(target == NULL || target->IsIdle())
	{
		ASSERT(false);
		return;
	}
	gplayer_imp *pImp = (gplayer_imp*)pPlayer->imp;
	if( !pImp->CanSwitch() )
	{
		return;
	}
	pImp->PlayerLeaveMap();
	//发送离开消息
	pPlayer->imp->_runner->leave_world();
	
	//将Player从本世界内移出 
	world *pPlane = pPlayer->plane;
	ASSERT(pPlane == pPlayer->imp->_plane);
	pPlane->RemovePlayer(pPlayer);

		
	A3DVECTOR oldpos = pPlayer->pos;	
	if(target->SwitchPlayerFrom(this, pPlayer, pos, ins_key) != 0)
	{
		//转移失败，不知道什么原因......
		//正常情况下应该将 Player加回到世界之中
		pPlayer->pos = oldpos;
		pPlane->InsertPlayer(pPlayer);
		ASSERT(false);
	}
}

int
world_message_handler::HandleMessage(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_SWITCH_START:
		ASSERT(msg.content_length == sizeof(instance_key));
		_manager->HandleSwitchStart(msg.source.id,msg.param, msg.pos, *(instance_key*)msg.content);
		break;

		case GM_MSG_SWITCH_GET:
		//玩家要准备转移了
		ASSERT(msg.content_length == sizeof(instance_key));
		ASSERT(msg.source.IsManager());
		ASSERT(msg.target.IsManager());
		_manager->SwitchPlayerTo(msg.param, msg.source.id,msg.pos, *(instance_key*)msg.content);
		break;
	}
	return 0;
}

world * 
world_manager::GetWorldLogin(int id, const GDB::base_info * pInfo, const GDB::vecdata * data)
{
	ASSERT(false);
	return NULL;
}

int world_manager::TranslateCtrlID( int which )
{
	return which;
}

bool world_manager::GetCommonData(int key, int & value, int data_type)
{
	if (data_type == 0 && key >= GENERAL_INDEX_LINE_BEGIN && key <= GENERAL_INDEX_LINE_END)
	{
		data_type = LINE_COMMON_DATA_TYPE_BEGIN;
	}
	else if (data_type == 0 && key >= GENERAL_INDEX_MAP_BEGIN && key <= GENERAL_INDEX_MAP_END)
	{
		data_type = MAP_COMMON_DATA_TYPE_BEGIN + _world_index;
	}
	else
	{
		ASSERT(false);
	}

	CommonDataInterface cif(data_type);
	if (cif.IsConsistent())
	{
		return cif.GetData(key, value);
	}
	return false;
}

void world_manager::SetCommonData(int key, int value, int data_type)
{
	if (data_type == 0 && key >= GENERAL_INDEX_LINE_BEGIN && key <= GENERAL_INDEX_LINE_END)
	{
		__PRINTF("设置本线全局数据，key=%d,value=%d\n", key, value);
		data_type = LINE_COMMON_DATA_TYPE_BEGIN;
	}
	else if (data_type == 0 && key >= GENERAL_INDEX_MAP_BEGIN && key <= GENERAL_INDEX_MAP_END)
	{
		__PRINTF("设置本地图全局数据，key=%d,value=%d\n", key, value);
		data_type = MAP_COMMON_DATA_TYPE_BEGIN + _world_index;
	}
	else
	{
		ASSERT(false);
	}

	CommonDataInterface cif(data_type);
	if (cif.IsConsistent())
	{
		cif.SetData(key, value);
	}
}

bool world_manager::ModifyCommonData(int key, int offset, int data_type)
{
	if (data_type == 0 && key >= GENERAL_INDEX_LINE_BEGIN && key <= GENERAL_INDEX_LINE_END)
	{
		data_type = LINE_COMMON_DATA_TYPE_BEGIN;
		__PRINTF("修改本线全局数据，key=%d,offset=%d\n", key, offset);
	}
	else if (data_type == 0 && key >= GENERAL_INDEX_MAP_BEGIN && key <= GENERAL_INDEX_MAP_END)
	{
		data_type = MAP_COMMON_DATA_TYPE_BEGIN + _world_index;
		__PRINTF("修改本地图全局数据，key=%d,offset=%d\n", key, offset);
	}
	else
	{
		ASSERT(false);
	}

	CommonDataInterface cif(data_type);
	if (cif.IsConsistent())
	{
		cif.AddData(key, offset);
		return true;
	}
	return false;
}

void world_manager::Release()
{
	if(_message_handler) 
	{
		delete _message_handler;
		_message_handler = 0;
	}
	_max_mob_sight_range = 15.f;
	_max_visible_range = DEFAULT_GRID_SIGHT_RANGE;
	_save_point.tag = -1;
	_save_point.pos = A3DVECTOR(0.f,0.f,0.f);
	ReleaseTraceMan();
}

bool world_manager::IsBuffAreaActive(int areaTid, int& npcTid)
{
	spin_autolock alock(status_map_lock);
	npcTid = -1;
	BuffAreaStatusMap::iterator it = statusMap.find(areaTid);
	if(it != statusMap.end())
	{
		npcTid = it->second;
		return true;
	}
	return false;
}

void world_manager::ActiveBuffArea(int areaTid, int npcId)
{
	spin_autolock alock(status_map_lock);
	BuffAreaStatusMap::iterator it = statusMap.find(areaTid);
	if(it == statusMap.end())
	{
		statusMap[areaTid] = npcId;
		__PRINTF("Active buff area: areaTid=%d, npcId=%d\n", areaTid, npcId);
	}
}

void world_manager::DeactiveBuffArea(int areaTid)
{
	spin_autolock alock(status_map_lock);
	BuffAreaStatusMap::iterator it = statusMap.find(areaTid);
	if(it != statusMap.end())
	{
		statusMap.erase(it);
		__PRINTF("Deactive buff area: areaTid=%d\n", areaTid);
	}
}

