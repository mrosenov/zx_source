#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include <threadpool.h>
#include <conf.h>
#include <io/pollio.h>
#include <io/passiveio.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <glog.h>
#include <strtok.h>
#include <timer.h>

#include "config.h"
#include "world.h"
#include "object.h"
#include "msgqueue.h"
#include "item.h"
#include "player_imp.h"
#include "npcgenerator.h"
#include "global_manager.h"
#include "petdataman.h"
#include "mobactivedataman.h"
#include "global_drop.h"
#include "ai/policy_loader.h"
#include "template/globaldataman.h"
#include "task/taskman.h"
#include "gmatrix.h"
#include "battleground/bg_world_manager.h"
#include "battlefield/bf_world_manager.h"
#include "territory/tr_world_manager.h"
#include "kingdom/kingdom_world_manager.h"
#include "kingdom/kingdom2_world_manager.h"
#include "kingdom/kingdom_bath_world_manager.h"
#include "item_manager.h"
#include "userlogin.h"
#include "antiwallow.h"
#include "wallow/wallowobj.h"
#include "commondata_if.h"
#include "global_table.h"
#include "achievement.h"
#include "obj_script.h"
#include "raid/raid_world_manager.h"
#include "raid/tower_raid_world_manager.h"
#include "raid/collision_raid_world_manager.h"
#include "raid/mafia_battle_raid_world_manager.h"
#include "raid/level_raid_world_manager.h"
#include "raid/transform_raid_world_manager.h"
#include "raid/step_raid_world_manager.h"
#include "raid/seek_raid_world_manager.h"
#include "raid/capture_raid_world_manager.h"
#include "forbid_manager.h"
#include <common/protocol.h>
#include <common/protocol_imp.h>
#include "factionbase/facbase_world_manager.h"
#include "topic_site.h"

extern abase::timer g_timer;

world_manager* gmatrix::_worlds[gmatrix::MAX_WORLD_TAG] = {0};

size_t      gmatrix::_player_max_count = GL_MAX_PLAYER_COUNT;
size_t      gmatrix::_npc_max_count = GL_MAX_NPC_COUNT;
size_t      gmatrix::_matter_max_count = GL_MAX_MATTER_COUNT;
world_param gmatrix::_world_param;
int	    gmatrix::w_max_tag = 0;
gmatrix::TOPLIST gmatrix::_toptitle;
int  	    gmatrix::_toptitle_lock = 0;
int         gmatrix::_lua_data_version = 0;
int         gmatrix::_active_ctrl_lock = 0;
int         gmatrix::_territory_lock = 0;
int         gmatrix::_raid_lock = 0;
int         gmatrix::_gmtoff = 0;

ai_trigger::manager gmatrix::_trigger_man;
rest_time_ctrl 	    gmatrix::_rest_ctrl;
itemdataman 	    gmatrix::_dataman;
abase::vector<int>  gmatrix::_instance_tag_list;
gmatrix::query_map  gmatrix::w_player_map(10000);
std::set<int> 	    gmatrix::_spawner_ctrl_disable_list;
std::set<int>	    gmatrix::_pocket_item_list;
std::set<int>	    gmatrix::_active_ctrl_list;	
std::map<int, gmatrix::territory_info> gmatrix::_territory_owner_list;
std::map<int, gmatrix::activity_info> gmatrix::_activity_info_list;

int         	gmatrix::w_pmap_lock = 0;	
int        	gmatrix::_server_index = 0;
int		gmatrix::_is_battle_field_server = 0;
int		gmatrix::_is_zone_server = 0;
char * 		gmatrix::_server_name = "";
std::string	gmatrix::_restart_shell = "restart";
std::string 	gmatrix::_toplist_script = "null";
reborn_bonus_man gmatrix::_reborn_bonus;
world_config 	g_config(MONEY_DROP_RATE,MONEY_MATTER_ID,WORLD_SPEAKER_ID,WORLD_SPEAKER_ID2,ITEM_DESTROYING_ID);

obj_manager<gmatter,TICK_PER_SEC * MATTER_HEARTBEAT_SEC,gmatrix::Insertor> gmatrix::w_matter_man;
obj_manager<gnpc   ,TICK_PER_SEC      ,gmatrix::Insertor> gmatrix::w_npc_man;
obj_manager<gplayer,TICK_PER_SEC      ,gmatrix::Insertor> gmatrix::w_player_man;

std::map<int, int>	gmatrix::_consign_item_list;
std::map<int, int>	gmatrix::_order_item_list;
gmatrix::ConsumptionValueConfigMap gmatrix::_equip_consumption_map; 
gmatrix::ConsumptionValueConfigMap gmatrix::_useitem_consumption_map;
gmatrix::ConsumptionValueConfigMap gmatrix::_special_consumption_map;


int gmatrix::w_max_raid_or_fbase_tag = 0;
//int gmatrix::w_max_facbase_tag = 0;
raid_controller gmatrix::w_raid_controller;
int gmatrix::_is_raid_server = 0;

gmatrix::WORLD_ACTIVE_CTRL_LIST gmatrix::_world_active_ctrl_list;
int gmatrix::_world_active_ctrl_lock = 0;
std::set<int> gmatrix::_active_event_list;
int gmatrix::_event_lock = 0;

int gmatrix::lottery_give_item_prob = 0;
int gmatrix::lottery_ty_exp_probs[5] = {0};
gmatrix::cash_gift_info gmatrix::_cash_gift_info;
gmatrix::kingdom_info gmatrix::_kingdom_info;
abase::vector<gmatrix::fuwen_compose> gmatrix::_fuwen_compose_list;
gmatrix::gt_buff_id gmatrix::_gt_buff_id;

int gmatrix::_is_flow_battle_server = 0;
bool gmatrix::_disable_cash_gift = false;
gmatrix::facbase_field_info gmatrix::_facbase_info;
facbase_controller gmatrix::w_fbase_controller;
// Youshuang add
std::map<int, facbase_mall*> gmatrix::_facbase_mall_list;
int gmatrix::_facbase_mall_lock = 0;
facbase_mall* gmatrix::GetFacbaseMall( int fid )
{
	spin_autolock keeper(_facbase_mall_lock);
	std::map<int, facbase_mall*>::iterator iter = _facbase_mall_list.find( fid );
	return ( iter != _facbase_mall_list.end() ) ? iter->second : NULL;
}
void gmatrix::AddFacbaseMall( int fid, facbase_mall* new_mall )
{
	ASSERT( _facbase_mall_lock );
	if( !new_mall ){ return; }
	_facbase_mall_list[fid] = new_mall;
}
void gmatrix::ModFacbaseMall( int fid, const std::set<int>& actived_items )
{
	spin_autolock keeper(_facbase_mall_lock);

	std::map<int, facbase_mall*>::iterator iter = _facbase_mall_list.find( fid );
        if( iter != _facbase_mall_list.end() )
        {       
		facbase_mall* facmall = iter->second;
                facmall->SyncMallItems( actived_items );
        }       
        else    
        {       
                facbase_mall* new_facmall = new facbase_mall( fid, actived_items );
                AddFacbaseMall( fid, new_facmall );
        } 
}
// end

gmatrix gmatrix::_instance;

static void timer_thread()
{
	g_timer.timer_thread();
}

static void timer_tick(int index,void *object,int remain)
{
	class World_Tick_Task : public ONET::Thread::Runnable , public abase::ASmallObject
	{
		public:
			World_Tick_Task() {}
			virtual void Run()
			{
				gmatrix::Instance()->Heartbeat();
				delete this;
			}
	};
	
	ONET::Thread::Pool::AddTask(new World_Tick_Task());
}

gmatrix::gmatrix()
{
	_heartbeat_lock = 0;
}

static int  script_Random0(lua_State *L)
{
	lua_pushnumber(L,abase::RandUniform());
	return 1;
}
static int  script_Random1(lua_State *L,int m)
{
	lua_pushinteger(L,abase::Rand(1,m));
	return 1;
}
static int  script_Random2(lua_State *L,int m, int n)
{
	lua_pushinteger(L,abase::Rand(m, n));
	return 1;
}

static int  script_Random(lua_State *L)
{
	int n = lua_gettop(L);    /* number of arguments */
	switch(n)
	{
		case 0:
			return script_Random0(L);
		case 1:
			{
				int m = -1;
				if(lua_isnumber(L, -1)) 
				{
					m = lua_tointeger(L,-1);
				}
				if(m <= 0) m = 1;
				return script_Random1(L,m);
			}
		case 2:
		default:
			{
				int m = -1;
				int n = -1;
				if(lua_isnumber(L, -2)) m = lua_tointeger(L,-2);
				if(lua_isnumber(L, -1)) n = lua_tointeger(L,-1);
				if(n < m) n = m;
				return script_Random2(L,m,n);
			}
	}
}

bool 
gmatrix::InitScriptSystem(const char * filename, const int * index_list, size_t count)
{
	for(size_t j = 0;j < count; j ++)
	{
		ASSERT(index_list[j] >= 0 && index_list[j] < LUA_ENV_INDEX_MAX);
		lua_State *L = luaL_newstate ();
		luaL_openlibs(L);

		//注册一个随机函数
		lua_register(L, "__ABASE_Random", script_Random);

		//装载全局初始化代码，这个代码是初始化代码，不是由策划写的
		if(luaL_dofile(L,filename))
		{
			return false;
		}

		//执行初始化代码
		lua_getfield(L, LUA_GLOBALSINDEX, "Init");
		lua_pushinteger(L, index_list[j]);
		lua_call(L, 1, 10);
		for(int i = 1; i <= 10; i ++)
		{
			const char * str = lua_tostring(L,-i);
			if(str == NULL) continue;
			printf("装载并执行lua脚本'%s'...",str);  fflush(stdout);
			if(luaL_dofile(L,str))
			{
				printf("失败\n");
				return false;
			}
			printf("完成\n");
		}
		lua_pop(L,10);

		AddGlobalLua(index_list[j], L);
	}
	return true;
}

bool 
gmatrix::ClearScriptDoFile(const int * list, size_t count)
{
	for(size_t j = 0;j < count; j ++)
	{
		ASSERT(list[j] >= 0 && list[j] < LUA_ENV_INDEX_MAX);
		lua_State *L = AttachScript(list[j]);
		if(L == NULL) continue;

		//执行初始化代码
		lua_getfield(L, LUA_GLOBALSINDEX, "InitClear");
		lua_call(L, 0, 0);
	}
	return true;
}

bool 
gmatrix::InitDisableSpawnerCtrl(const char * str)
{
	abase::strtok tok(str, " \t\r\n");
	const char * token;
	while((token = tok.token()))
	{
		if(!*token) continue;
		int id = atoi(token);
		printf("控制器%s('%d')被取消\n",token, id);
		_spawner_ctrl_disable_list.insert(id);
	}
	return true;
}

bool 
gmatrix::InitPocketItemList(const char *filename)
{
	std::ifstream in(filename);
	if(!in) return false;
	std::string str;
	printf("初始化随身包裹允许物品列表\t");
	while(in)
	{
		in >> str;
		_pocket_item_list.insert(atoi(str.c_str()));
			
	}
	printf("物品数量:  %d\n", _pocket_item_list.size());
	in.close();
	return true;
	
}	

bool 
gmatrix::LoadLuaVersion(const char * filename)
{
	char buf[1024];
	FILE * file = fopen(filename, "rb");
	if(file == NULL) return false;
	bool bRst = false;
	if(fgets(buf,sizeof(buf), file))
	{
		int version = 0;
		sscanf(buf, "--%d", &version);
		if(version <= 0)
		{
			printf("LUA腳本錯誤%d '%s'\n", version, buf);
		}
		else
		{
			_lua_data_version = version;
			printf("LUA腳本版本%d\n", version);
			bRst = true;
		}
	}
	fclose(file);
	return bRst;
}

static bool GetWallowConf(const char * name, anti_wallow::punitive_param  &p)
{
	ONET::Conf *conf = ONET::Conf::GetInstance();
	p.exp           = atof(conf->find(name,"exp").c_str());
	p.item          = atof(conf->find(name,"item").c_str());
	p.money         = atof(conf->find(name,"money").c_str());
	p.task_exp      = atof(conf->find(name,"task_exp").c_str());
	p.task_sp       = atof(conf->find(name,"task_sp").c_str());
	p.task_money    = atof(conf->find(name,"task_money").c_str());
	return *conf->find(name,"exp").c_str();
}

bool gmatrix::InitWallowParam()
{
	anti_wallow::punitive_param p = {true, 1.0,1.0,1.0,1.0,1.0,1.0};
	anti_wallow::SetParam(0, p);
	anti_wallow::SetParam(1, p);

	if(!GetWallowConf("WallowLight", p)) return false;
	anti_wallow::SetParam(2, p);

	if(!GetWallowConf("WallowHeavy", p)) return false;
	anti_wallow::SetParam(3, p);

	ONET::Conf *conf = ONET::Conf::GetInstance();

	int light,heavy,clear;
	light = atoi(conf->find("WallowTime", "TimeLight").c_str());
	heavy = atoi(conf->find("WallowTime", "TimeHeavy").c_str());
	clear = atoi(conf->find("WallowTime", "TimeClear").c_str());
	bool mode = strcmp(conf->find("WallowTime", "ClearMode").c_str(), "TimeOfDay") == 0;
	printf("沉迷的三个时间参数为%d,%d,%d ,沉迷恢复方式为:'%s'\n",light,heavy, clear,mode?"每天零点":"休息时间到达");
	wallow_object::SetParam(light,heavy,clear,mode?1:0);

	std::string str = conf->find("General", "AntiWallow");
	if(strcmp(str.c_str() , "true") == 0)
	{
		_world_param.anti_wallow = 1;
		printf("防沉溺功能系统开启\n");
	}
	else if(strcmp(str.c_str(), "false") == 0)
	{
		_world_param.anti_wallow = 0;
		printf("防沉溺功能系统关闭\n");
	}
	else
	{
		printf("未找到防沉迷开关状态 默认关闭\n");
	}
	
	return true;
}

void
gmatrix::RegisterObjectScript(int env)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),env);
	lua_State* L = keeper.GetState();
	if(L == NULL) return;

	lua_register(L, "__ABASE_OBJECT_GetLevel", gactive_script::script_GetLevel);
	lua_register(L, "__ABASE_OBJECT_GetHP", gactive_script::script_GetHP);
	lua_register(L, "__ABASE_OBJECT_GetMP", gactive_script::script_GetMP);
	lua_register(L, "__ABASE_OBJECT_GetCurHP", gactive_script::script_GetCurHP);
	lua_register(L, "__ABASE_OBJECT_GetCurMP", gactive_script::script_GetCurMP);
	lua_register(L, "__ABASE_OBJECT_GetSpeed", gactive_script::script_GetSpeed);
	lua_register(L, "__ABASE_OBJECT_GetCrit", gactive_script::script_GetCrit);
	lua_register(L, "__ABASE_OBJECT_GetDamage", gactive_script::script_GetDamage);
	lua_register(L, "__ABASE_OBJECT_GetAttack", gactive_script::script_GetAttack);
	lua_register(L, "__ABASE_OBJECT_GetDefense", gactive_script::script_GetDefense);
	lua_register(L, "__ABASE_OBJECT_GetArmor", gactive_script::script_GetArmor);
	lua_register(L, "__ABASE_OBJECT_GetResistance", gactive_script::script_GetResistance);
	lua_register(L, "__ABASE_OBJECT_GetAntiCrit", gactive_script::script_GetAntiCrit);
	lua_register(L, "__ABASE_OBJECT_GetAttackRange", gactive_script::script_GetAttackRange);
	lua_register(L, "__ABASE_OBJECT_GetSkillRate", gactive_script::script_GetSkillRate);
	lua_register(L, "__ABASE_OBJECT_GetSkillLevel", gactive_script::script_GetSkillLevel);
	lua_register(L, "__ABASE_OBJECT_GetPosition", gactive_script::script_GetPosition);
	lua_register(L, "__ABASE_OBJECT_GetCultivation", gactive_script::script_GetCultivation);
	lua_register(L, "__ABASE_OBJECT_GetCircleMemberCnt", gactive_script::script_GetCircleMemberCnt);

	lua_register(L, "__ABASE_OBJECT_SetLevel", gactive_script::script_SetLevel);
	lua_register(L, "__ABASE_OBJECT_SetHP", gactive_script::script_SetHP);
	lua_register(L, "__ABASE_OBJECT_SetMP", gactive_script::script_SetMP);
	lua_register(L, "__ABASE_OBJECT_SetCurHP", gactive_script::script_SetCurHP);
	lua_register(L, "__ABASE_OBJECT_SetCurMP", gactive_script::script_SetCurMP);
	lua_register(L, "__ABASE_OBJECT_SetSpeed", gactive_script::script_SetSpeed);
	lua_register(L, "__ABASE_OBJECT_SetCrit", gactive_script::script_SetCrit);
	lua_register(L, "__ABASE_OBJECT_SetDamage", gactive_script::script_SetDamage);
	lua_register(L, "__ABASE_OBJECT_SetAttack", gactive_script::script_SetAttack);
	lua_register(L, "__ABASE_OBJECT_SetDefense", gactive_script::script_SetDefense);
	lua_register(L, "__ABASE_OBJECT_SetArmor", gactive_script::script_SetArmor);
	lua_register(L, "__ABASE_OBJECT_SetResistance", gactive_script::script_SetResistance);
	lua_register(L, "__ABASE_OBJECT_SetAttackRange", gactive_script::script_SetAttackRange);
	lua_register(L, "__ABASE_OBJECT_SetAntiCrit", gactive_script::script_SetAntiCrit);
	lua_register(L, "__ABASE_OBJECT_SetSkillRate", gactive_script::script_SetSkillRate);
	lua_register(L, "__ABASE_OBJECT_SetAntiInvisibleRate", gactive_script::script_SetAntiInvisibleRate);

	lua_register(L, "__ABASE_OBJECT_AddHP", gactive_script::script_AddHP);
	lua_register(L, "__ABASE_OBJECT_AddMP", gactive_script::script_AddMP);
	lua_register(L, "__ABASE_OBJECT_AddDefense", gactive_script::script_AddDefense);
	lua_register(L, "__ABASE_OBJECT_AddAttack", gactive_script::script_AddAttack);
	lua_register(L, "__ABASE_OBJECT_AddArmor", gactive_script::script_AddArmor);
	lua_register(L, "__ABASE_OBJECT_AddDamage", gactive_script::script_AddDamage);
	lua_register(L, "__ABASE_OBJECT_AddResistance", gactive_script::script_AddResistance);
	lua_register(L, "__ABASE_OBJECT_AddCrit", gactive_script::script_AddCrit);
	lua_register(L, "__ABASE_OBJECT_AddSpeed", gactive_script::script_AddSpeed);
	lua_register(L, "__ABASE_OBJECT_AddAntiCrit", gactive_script::script_AddAntiCrit);
	lua_register(L, "__ABASE_OBJECT_AddSkillRate", gactive_script::script_AddSkillRate);
	lua_register(L, "__ABASE_OBJECT_AddAttackRange", gactive_script::script_AddAttackRange);
	lua_register(L, "__ABASE_OBJECT_AddScaleHP", gactive_script::script_AddScaleHP);
	lua_register(L, "__ABASE_OBJECT_AddScaleMP", gactive_script::script_AddScaleMP);
	lua_register(L, "__ABASE_OBJECT_AddScaleDefense", gactive_script::script_AddScaleDefense);
	lua_register(L, "__ABASE_OBJECT_AddScaleAttack", gactive_script::script_AddScaleAttack);
	lua_register(L, "__ABASE_OBJECT_AddScaleArmor", gactive_script::script_AddScaleArmor);
	lua_register(L, "__ABASE_OBJECT_AddScaleDamage", gactive_script::script_AddScaleDamage);
	lua_register(L, "__ABASE_OBJECT_AddScaleSpeed", gactive_script::script_AddScaleSpeed);
	lua_register(L, "__ABASE_OBJECT_AddScaleResistance", gactive_script::script_AddScaleResistance);

	lua_register(L, "__ABASE_OBJECT_AddReputation", gactive_script::script_AddReputation);
	lua_register(L, "__ABASE_OBJECT_AddSkillProficiency", gactive_script::script_AddSkillProficiency);
	lua_register(L, "__ABASE_OBJECT_AddTransformSkill", gactive_script::script_AddTransformSkill);
}

void
gmatrix::RegisterMiscScript()
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_MISC);
	lua_State* L = keeper.GetState();
	if(L == NULL) return;
	
	lua_register(L, "__ABASE_MISC_AddActivityInfo", gmatrix::script_AddActivityInfo);
}

int 
gmatrix::Init(const char * gmconf_file, char ** world_name, char ** ins_name, char ** zone_name  )
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_BLOCK, &set, NULL);


	/*
		创建全局数据空间
	*/

	CommonDataSpaceCreate(0);	//通用空间


	CommonDataSpaceCreate(1003);	//测试空间一号
	
	CommonDataSpaceCreate(LINE_COMMON_DATA_TYPE_BEGIN, CMN_DATA_LOCAL); //本线专用数据, 不存盘



	/*
	 *       创建定时器线程 
	 */
	ONET::Thread::Pool::CreateThread(timer_thread);

	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "Matrix";

	//获取时区偏移值
        int cur_time = g_timer.get_systime();
        tm tm_cur; 
        localtime_r((time_t*)&cur_time, &tm_cur);
        _gmtoff = tm_cur.tm_gmtoff;


	
	//初始化全局lua脚本系统
	std::string global_lua  = conf->find("Script","GlobalScript");
	if(!global_lua.empty()) 
	{
		int script_list[] = {LUA_ENV_LOTTERY, LUA_ENV_TALISMAN,LUA_ENV_PET_BEDGE, LUA_ENV_PLAYER, LUA_ENV_SUMMON, 
			LUA_ENV_MAGIC, LUA_ENV_TRANSFORM, LUA_ENV_MISC, LUA_ENV_RUNE, LUA_ENV_ASTROLOGY};
		if(!InitScriptSystem(global_lua.c_str(), script_list, sizeof(script_list)/sizeof(int)))
		{	
			printf("初始化LUA脚本系统失败\n");
			return -11;
		}
	}
	
	RegisterObjectScript(LUA_ENV_PLAYER);
	RegisterObjectScript(LUA_ENV_SUMMON);
	RegisterObjectScript(LUA_ENV_MAGIC);
	RegisterObjectScript(LUA_ENV_TRANSFORM);
	RegisterObjectScript(LUA_ENV_RUNE);
	RegisterMiscScript();

	if(!LoadSpiritConfig("script/soulpowerdata.lua"))
	{	
		printf("初始化魂力配置LUA脚本系统失败\n");
		return -11;
	}
	
	if(!LoadMenologyConfig())
	{	
		printf("初始化活动配置LUA脚本系统失败\n");
		return -12;
	}

	//读取防沉迷数据
	if(!gmatrix::InitWallowParam())
	{
		printf("读取防沉迷数据失败\n");
		return -122;
	}

	//读取商场配置
	std::string korea_version= conf->find("General", "korea_version");
	if(strcmp(korea_version.c_str() , "true") == 0)
	{
		_world_param.billing_shop= true;
		printf("开启韩国billing元宝交易系统\n");
	}
	else if(strcmp(korea_version.c_str(), "false") == 0)
	{
		_world_param.billing_shop= false;
	}
	else
	{
		_world_param.billing_shop= false;
	}
	
	std::string pk_setting = conf->find("General", "pk_forbidden");
	if(strcmp(pk_setting.c_str() , "true") == 0)
	{
		_world_param.pk_forbidden = true;
		printf("开启PK保护\n");
	}
	else if(strcmp(pk_setting.c_str(), "false") == 0)
	{
		_world_param.pk_forbidden = false;
	}
	else
	{
		_world_param.pk_forbidden = false;
	}	
	std::string  zoneserver = conf->find("General", "ZoneServer");
	if(strcmp(zoneserver.c_str() , "true") == 0)
	{
		_is_zone_server = true;
		printf("跨服服务器启动");
	}
	else if(strcmp(zoneserver.c_str(), "false") == 0)
	{
		_is_zone_server = false;
	}
	else
	{
		_is_zone_server = false;
	}

	std::string  disable_cash_gift = conf->find("General", "DisableCashGift");
	if(strcmp(disable_cash_gift.c_str() , "true") == 0)
	{
		_disable_cash_gift = true;
	}
	else if(strcmp(disable_cash_gift.c_str(), "false") == 0)
	{
		_disable_cash_gift = false;
	}
	else
	{
		_disable_cash_gift = false;
	}

	g_config.long_offline_bonus = atoi(conf->find("General", "OfflineBonus").c_str());
	printf("老玩家回归类别一为：%s\n", (g_config.long_offline_bonus & 0x01)?"开启":"关闭");
	printf("老玩家回归类别二为：%s\n", (g_config.long_offline_bonus & 0x02)?"开启":"关闭");
	printf("老玩家回归类别三为：%s\n", (g_config.long_offline_bonus & 0x04)?"开启":"关闭");

	g_config.item_broken_active = atoi(conf->find("General", "BreakDropItem").c_str());
	printf("红名掉落物品破损：%s\n", g_config.item_broken_active?"开启":"关闭");
	g_config.gshop_bonus_ratio = atoi(conf->find("General", "GShopBonusRatio").c_str());

	//从世界中读取NPC,PLAYER和物品限制
	_player_max_count = atoi(conf->find(section,"player_capacity").c_str());
	_npc_max_count = atoi(conf->find(section,"npc_count").c_str());
	_matter_max_count = atoi(conf->find(section,"matter_count").c_str());
	if(_player_max_count > GL_MAX_PLAYER_COUNT || _player_max_count <= 0) 
		_player_max_count=GL_MAX_PLAYER_COUNT;
	if(_npc_max_count > GL_MAX_NPC_COUNT || _npc_max_count <= 0) 
		_npc_max_count = GL_MAX_NPC_COUNT;
	if(_matter_max_count > GL_MAX_MATTER_COUNT || _matter_max_count <=0) 
		_matter_max_count = GL_MAX_MATTER_COUNT;

	w_npc_man.Init(_npc_max_count);
	w_matter_man.Init(_matter_max_count);
	w_player_man.Init(_player_max_count);

	_server_index = atoi(conf->find("Identify","ServerID").c_str());
	if(_server_index <= 0) 
	{
		printf("未找到正确的Server ID\n");
		return -10;
	}

	_server_name = strdup(conf->find("Identify","Name").c_str());
	
	printf("服务器ID %d ， 服务器名称 '%s'\n",_server_index, _server_name);

	
	time_t ct = time(NULL);
	printf("Start time:%s\n",ctime(&ct));
	std::string root = conf->find("Template","Root");
	std::string path = root + conf->find("Template","itemDataFile");

	std::string consign_path = root + conf->find("Template", "ConsignItemListFile");
	if(!InitConsignItemList(consign_path.c_str()))
	{
		printf("无法读物寄售物品立标文件: '%s'\n", consign_path.c_str());
		return -21; 
	}

	std::string order_item_path = root + conf->find("Template","OrderItemListFile");
	if (!InitOrderItemList(order_item_path.c_str()))
	{


		printf("无法读取整理物品顺序文件:'%s'\n", order_item_path.c_str());
		return -23; 
	}

	std::string lottery_prob_path = root + conf->find("Template", "LotteryProbFile");
	if(!InitLotteryGiveItemProb(lottery_prob_path.c_str()))
	{
		printf("无法读取使用彩票给予奖励物品概率配置文件: '%s'\n", lottery_prob_path.c_str());
		return -22;
	}

	std::string consumption_path = conf->find("Template", "ConsumptionValueFile");
	if(!consumption_path.empty())
	{
		consumption_path = root + consumption_path;
		if(!InitConsumptionValueList(consumption_path.c_str()))
		{
			printf("读取消费值对照表的配置文件有误: '%s'\n", consumption_path.c_str());
			return -321; 
		}
	}
	
	if(strcmp(conf->find("General","old_move").c_str(),"true") == 0)
	{
		extern int g_use_old_move_policy;
		g_use_old_move_policy = 1;
		printf("使用旧式的移动策略\n");
	}

	//装载物品和数据模板
	if(_dataman.load_data(path.c_str()))
	{
		printf("无法读取物品和怪物的模板文件:'%s'\n", "elements.data");
		return -3;
	}

	std::string cash_gift_path = root + conf->find("Template", "CashGiftFile");
	if(!InitCashGiftInfo(cash_gift_path.c_str()))
	{
		printf("读取商城买赠功能配置的时候发生错误 '%s'\n", cash_gift_path.c_str());
		return -23;
	}

	std::string fuwen_compose_path = root + conf->find("Template", "FuwenComposeFile");
	if(!InitFuwenComposeInfo(fuwen_compose_path.c_str()))
	{
		printf("读取符文合成的功能配置的时候发生错误 '%s'\n", fuwen_compose_path.c_str());
		return -24;
	}

	
	std::string gt_buff_path = root + "gt_buff.txt";
	if(!InitGTBuffInfo(gt_buff_path.c_str()))
	{
		printf("读取GT buff配置的时候发生错误'%s'\n", gt_buff_path.c_str());
		return -25;
	}
       	

	//装载全局掉落表
	path = root + conf->find("Template","DropData");
	if(load_extra_drop_tables(path.c_str()))
	{
		printf("读取全局掉落表失败:'%s'\n",path.c_str());
		return -5;
	}

	if(!player_template::Load("ptemplate.conf",&GetDataMan()))
	{
		__PRINTF("无法装载 'ptemplate.conf' 或者elementdata的config有误\n");
		return -7;
	}

/*	 有错 暂时注释掉 
	if(!player_template::GetDebugMode())
	{
		int script_list[] = {LUA_ENV_LOTTERY, LUA_ENV_TALISMAN };
		ClearScriptDoFile(script_list, sizeof(script_list)/sizeof(int));
	}
	*/

	//这里进行策略装载
	path = root + conf->find("Template","PolicyData");
	if(!LoadAIPolicy(path.c_str()))
	{
		printf("装载策略文件失败:'%s'\n",path.c_str());
		return -300;
	}

	if(!drop_template::LoadDropList())
	{
		printf("初始化全局掉落表失败\n");
		return -6;
	}

	//装载控制器取消列表
	InitDisableSpawnerCtrl(conf->find("SpawnController","disable").c_str());


	//装载任务系统
	std::string path1 = root + conf->find("Template","QuestPackage");
	std::string path2 = root + conf->find("Template","QuestPackage2");
	std::string path3 = root + conf->find("Template","QuestForbid");
	std::string path4 = root + conf->find("Template","QuestNPCInfo");
	if(!InitQuestSystem(path1.c_str(),path2.c_str(),path3.c_str(),path4.c_str()))
	{
		printf("读取任务系统失败:'%s','%s','%s','%s'' \n",path1.c_str(), path2.c_str(), path3.c_str(), path4.c_str());
		return -4;
	}
	
	//初始化随身包裹允许的物品列表
	path1 = root + conf->find("Template", "PocketItemFile");
	if(!InitPocketItemList(path1.c_str()))
	{
		printf("读取随身包裹允许物品列表失败: %s\n", path1.c_str());
		return -20;
	}

	path1 = root + conf->find("Template","GlobalData");
	path2 = root + conf->find("Template","MallData");
	path3 = root + conf->find("Template","BonusMallData");
	path4 = root + conf->find("Template","ZoneMallData");
	std::string path5 = root + conf->find("Template", "VipAwardData");
	if(!globaldata_loadserver(path1.c_str(),path2.c_str(), path3.c_str(), path4.c_str(), path5.c_str()))
	{
		printf("初始化全局数据失败:'%s', '%s', '%s', '%s', '%s'\n",path1.c_str(),path2.c_str(),path3.c_str(), path4.c_str(), path5.c_str());
		return -18;
	}

	//装载NPC MATTER DYN_OBJECT的凸包数据
	path = root + conf->find("Template","CollisionElement");
	if(!trace_manager::LoadElement(path.c_str()))
	{
		printf("加载元素凸包数据据失败'%s'",path.c_str());
		return - 19;
	}

	
	//初始化物品管理器 这个应该在 初始化全局数据后面，因为需要装载百宝阁数据 
	if(!item_manager::InitFromDataMan(GetDataMan()))
	{
		__PRINTF("item_manager::初始化物品管理器失败\n");
		return -8;
	}

	if(!_reborn_bonus.InitFromDataMan(GetDataMan()))
	{
		__PRINTF("reborn_bonus_manager::初始化转生奖励失败\n");
		return -9;
	}

	if(!InitNPCTemplate())
	{
		printf("初始化配方和NPC失败\n");
		return -17;
	}

	if(!InitTagList())
	{
		printf("读取副本标识失败\n");
		return -123;
	}

	//读取打卡时间参数
	std::string str = conf->find("DoubleExp","clear_day");
	int min,hours,mday;
	sscanf(str.c_str(),"%d %d %d",&min,&hours,&mday);
	if(min < 0 || min >= 60 || hours <0 || hours >= 24 || mday < 0 || mday>=7) 
	{
		printf("双倍打卡时间起始时间不正确\n");
		return -18;
	}
	char *weeks[] ={"日","一","二","三","四","五","六"};
	printf("双倍经验时间每周起始时间为:星期%s %02d:%02d\n",weeks[mday],hours,min);

	int first_rest_time = atoi(conf->find("DoubleExp","first_rest_time").c_str());
	int rest_period = atoi(conf->find("DoubleExp","rest_period").c_str());
	int rest_time_unit = atoi(conf->find("DoubleExp","rest_time_unit").c_str());
	int max_rest_time  = atoi(conf->find("DoubleExp","max_rest_time").c_str());

	if(first_rest_time < 0 || rest_period <= 0 || rest_time_unit < 0 || max_rest_time <= 0 || max_rest_time > 3600*200)
	{
		printf("双倍时间参数不正确 \n");
		return -19;
	}
	rest_time_ctrl ctrl = {{min,hours,mday},first_rest_time,rest_period*3600,rest_time_unit,max_rest_time};
	_rest_ctrl = ctrl;

	//读取脚本文件版本
	path = root + conf->find("Template","LuaData");
	if(!LoadLuaVersion(path.c_str()))
	{
		printf("无法读取LUA脚本文件'%s'\n", path.c_str());
		return -18;
	}

	//得到重新启动的操作
	_restart_shell  = conf->find("Template","RestartShell");

	//得到生成称谓列表的shell
	_toplist_script  = conf->find("Template","TopListScript");
	char cur_path[1024];
	if(getcwd(cur_path, sizeof(cur_path) -1 ))
	{
		int len = strlen(cur_path);
		if(len && cur_path[len -1] != '/') 
		{
			cur_path[len] = '/';
			cur_path[len+1] = 0;
		}
		_toplist_script  = cur_path + _toplist_script;
	}

	if(access(_toplist_script.c_str(), R_OK|X_OK|F_OK) != 0)
	{
		printf("toplistsrcipt:'%s'不满足执行需求\n", _toplist_script.c_str());
		return -102;
	}
	
	path = root + conf->find("Template","AchievementFile");
	if (!Achievement::LoadConfig(path.c_str()))
	{
		printf("读取成就配置文件失败，!\n");
		return -103;
	}

	// Youshuang add, read topic site config file
	//path = root + conf->find("Template","TopicFile");
	if( !TOPIC_SITE::LoadConfig( "./config/affairs_config.txt" ) )
	{
		printf( "read zhuxian topic site file failed !\n" );
		return -104;
	}
	// end
	
	char ** map_name;
	if(!IsZoneServer()) 
	{
		map_name = world_name;
	}
	else
	{
		map_name = zone_name;
	}

	//城战GS
	if(atoi(conf->find("General","is_battlefield").c_str()))
	{
		for(;*map_name;map_name ++)
		{
			const char* servername = *map_name;
			global_world_manager* gwm = bf_world_manager_base::new_bf_world_manager(gmconf_file,servername);
			printf("-----------------初始化battlefield世界 %s -------------------\n",servername);
			if(int rst = gwm->Init(gmconf_file,servername, -1, -1))
			{
				printf("世界%s初始化失败%d\n",servername,rst);
				return -111;
			}
			int tag = gwm->GetWorldTag();
			if( tag >MAX_WORLD_TAG)
			{
				printf("非法的World_tag=%d 在 _world %s\n", gwm->GetWorldTag(), servername);
				return -1;
			}
			//if(_worlds[tag] != NULL)
			if(FindWorld(tag) != NULL)
			{
				printf("重名的World_tag=%d 在 world %s\n", gwm->GetWorldTag(), servername);
				return -2;
			}
			InsertWorldManager(tag, gwm);
		}
		_is_battle_field_server = 1;
	}
	//领土战GS
	//帮派基地地图放在领土战gs上 因基地地图与副本类似有多个实例，所以基地地图应该配置在最后面
	else if(atoi(conf->find("General","is_territory").c_str()))
	{
		for(;*map_name;map_name ++)
		{
			const char* servername = *map_name;
			ONET::Conf::section_type section = "World_";
			section += servername;
			int kingdom_type = atoi(conf->find(section, "kingdom_type").c_str());
			int is_facbase = atoi(conf->find(section, "is_facbase").c_str());
			global_world_manager* gwm = NULL;

			if (is_facbase)
			{
				if (_facbase_info.world_tag != 0 || _facbase_info.max_instance != 0)
				{
					printf("帮派基地地图配置重复\n");
					return -28;
				}
				int max_count = atoi(conf->find(section, "max_instance_num").c_str());
				if (max_count <= 0)
				{
					printf("帮派基地实例个数 %d 非法\n", max_count);
					return -27;
				}
				int tag = atoi(conf->find(section, "tag").c_str());
				_facbase_info.world_tag = tag;
				_facbase_info.max_instance = max_count;
				_facbase_info.servername = servername;

				if (!w_fbase_controller.Init(servername, tag))
				{
					printf("基地地图共享资源初始化错误");
					return -29;
				}

				if (w_max_raid_or_fbase_tag == 0)
					w_max_raid_or_fbase_tag = w_max_tag;
				printf("-----初始化帮派基地空对象 tag %d max_num %d\n", tag, max_count);
				for (int i = 0; i < max_count; i++)
				{
					printf("create facbase index %d\n", w_max_raid_or_fbase_tag + 1);
					gwm = new facbase_world_manager();
					InsertRaidOrFBaseManager(gwm);
				}
				break; //认为基地地图为最后一个配置 后面不再有其他地图配置
			}
			if(kingdom_type == 1)
			{
				printf("-----------------初始化国王战主战场地图 %s -------------------\n",servername);
				gwm = new kingdom_world_manager(); 
			}
			else if(kingdom_type == 2)
			{
				printf("-----------------初始化国王战辅战场地图%s -------------------\n",servername);
				gwm = new kingdom2_world_manager();
			}
			else
			{
				printf("-----------------初始化领土战地图 %s -------------------\n",servername);
				gwm = new tr_world_manager();
			}
			if(int rst = gwm->Init(gmconf_file,servername, -1, -1))
			{
				printf("世界%s初始化失败%d\n",servername,rst);
				return -111;
			}
			int tag = gwm->GetWorldTag();
			if( tag >MAX_WORLD_TAG)
			{
				printf("非法的World_tag=%d 在 _world %s\n", gwm->GetWorldTag(), servername);
				return -1;
			}
			//if(_worlds[tag] != NULL)
			if(FindWorld(tag) != NULL)
			{
				printf("重名的World_tag=%d 在 world %s\n", gwm->GetWorldTag(), servername);
				return -2;
			}
			InsertWorldManager(tag, gwm);
		}
		_is_battle_field_server = 1;
	}
	//Raid副本GS
	else if(atoi(conf->find("General", "is_raid").c_str()))
	{
		char ** ins_map_name = ins_name;

		for(; *ins_map_name; ins_map_name ++)
		{
			const char* servername = *ins_map_name;
			ONET::Conf::section_type section = "World_";
			section += servername;
			printf("-----------------初始化Raid副本世界模板 %s -------------------\n",servername);
			raid_world_template* t = new raid_world_template();
			if(!t->Init(servername, atoi(conf->find(section, "raid_id").c_str())))
			{
				delete t;
				t = NULL;
				printf("副本模板初始化错误\n");
				return -1113;
			}
			t->rwinfo.raid_template_id = atoi(conf->find(section, "raid_template_id").c_str());
			t->rwinfo.raid_type = atoi(conf->find(section, "raid_type").c_str());
			t->rwinfo.raid_max_instance = atoi(conf->find(section,"max_instance_num").c_str());

			DATA_TYPE dt;
			const void* config = _dataman.get_data_ptr(t->rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
			if((dt != DT_TRANSCRIPTION_CONFIG && dt != DT_TOWER_TRANSCRIPTION_CONFIG)|| !config)
			{
				printf("无效的副本模板: %d\n", t->rwinfo.raid_template_id);
				delete t;
				t = NULL;
				return -1112;
			}


			if(dt == DT_TRANSCRIPTION_CONFIG)
			{
				const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) _dataman.get_data_ptr(t->rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
				t->rwinfo.queuing_time = raid_config.room_active_time;
				t->rwinfo.group_limit.max_player_limit = raid_config.max_player_num;
				t->rwinfo.group_limit.min_start_player_num = raid_config.min_player_num;
			}
			else if(dt == DT_TOWER_TRANSCRIPTION_CONFIG)
			{
				const TOWER_TRANSCRIPTION_CONFIG& raid_config = *(const TOWER_TRANSCRIPTION_CONFIG*) _dataman.get_data_ptr(t->rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
				t->rwinfo.queuing_time = raid_config.room_active_time;
				t->rwinfo.group_limit.max_player_limit = raid_config.max_player_num;
				t->rwinfo.group_limit.min_start_player_num = raid_config.min_player_num;
			}

			if(!w_raid_controller.AddRaidWorldTemplate(t))
			{
				printf("重复的raid_id=%d 在 Raid %s\n", t->rwinfo.raid_id, servername);
				delete t;
				t = NULL;
				return  -1111;
			}
		}
		if (w_max_raid_or_fbase_tag == 0)
			w_max_raid_or_fbase_tag = w_max_tag; 

		raid_controller::RaidWorldInfoMap::const_iterator it = w_raid_controller.GetRaidWorldInfoMap().begin();

		for(; it != w_raid_controller.GetRaidWorldInfoMap().end(); ++ it)
		{
			const raid_world_template* t = it->second;
			printf("-----------------初始化普通副本空对象, raid_id=%d, raid_type=%d, max_num=%d-------------------\n", t->rwinfo.raid_id, t->rwinfo.raid_type, t->rwinfo.raid_max_instance);
			for(int i = 0; i < t->rwinfo.raid_max_instance; i ++)
			{
				printf("--raid_instance_world_tag=%d初始化--\n", w_max_raid_or_fbase_tag + 1);
				global_world_manager* gwm = NULL;
				if(t->rwinfo.raid_type == RT_COMMON)
				{
					gwm = new cr_world_manager();
				}
				else if(t->rwinfo.raid_type == RT_TOWER)
				{
					gwm = new tower_raid_world_manager();
				}
				else if(t->rwinfo.raid_type == RT_COLLISION)
				{
					gwm = new collision_raid_world_manager();
				}
				else if(t->rwinfo.raid_type == RT_COLLISION2) 
				{
					gwm = new collision2_raid_world_manager();
				}
				else if(t->rwinfo.raid_type == RT_MAFIA_BATTLE)
				{
					gwm = new mafia_battle_raid_world_manager();
				}
				else if(t->rwinfo.raid_type == RT_LEVEL)
				{
					gwm = new level_raid_world_manager();
				}
				else if (t->rwinfo.raid_type == RT_TRANSFORM)
				{
					gwm = new transform_raid_world_manager();
				}
				else if (t->rwinfo.raid_type == RT_STEP)
				{
					gwm = new step_raid_world_manager();
				}
				else if (t->rwinfo.raid_type == RT_SEEK)
				{
					gwm = new seek_raid_world_manager();
				}
				else if (t->rwinfo.raid_type == RT_CAPTURE)
				{
					gwm = new capture_raid_world_manager();
				}

				InsertRaidOrFBaseManager(gwm);
			}
		}
		_is_raid_server = 1;
	}
	//普通GS
	else
	{
		//开始创建每个世界
		for(;*map_name;map_name ++)
		{
			const char * servername = *map_name;
			ONET::Conf::section_type section = "World_";
			section += servername;
			global_world_manager* gwm = NULL;

			if( atoi( conf->find( section, "is_battle_world" ).c_str() ) )
			{
				int battle_type = atoi(conf->find(section, "battle_type").c_str());
				if(battle_type == BT_COMMON || battle_type == BT_REBORN_COMMON)
				{	
					gwm = new cb_world_manager();
				}
				else if(battle_type == BT_KILLFIELD || battle_type == BT_REBORN_KILLFIELD)
				{	
					gwm = new kf_world_manager();
				}
				else if(battle_type == BT_ARENA || battle_type == BT_REBORN_ARENA)
				{	
					gwm = new ar_world_manager();
				}
				else if(battle_type == BT_CROSSSERVER || battle_type == BT_MELEE_CROSSSERVER)
				{	
					gwm = new cs_world_manager();
				}
				else if(battle_type == BT_INSTANCE)
				{
					gwm = new ib_world_manager();
				}
				else if(battle_type == BT_CHALLENGE_SPACE)
				{
					gwm = new ch_world_manager();
				}
				else if(battle_type == BT_TEAM_CRSSVR)
				{
					gwm = new cs_team_world_manager();
				}
				else if(battle_type == BT_FLOW_CRSSVR)
				{
					gwm = new cs_flow_world_manager();
				}
			}
			else if(atoi( conf->find( section, "kingdom_type" ).c_str()) == 3)
			{
				gwm = new kingdom_bath_world_manager();
			}
			else
			{
				gwm = new global_world_manager();
			}
			printf("-----------------初始化世界 %s -------------------\n",servername);
			if(int rst = gwm->Init(gmconf_file,servername, -1, -1))
			{
				printf("世界%s初始化失败%d\n",servername,rst);
				return -111;
			}
			int tag = gwm->GetWorldTag();
			if( tag >MAX_WORLD_TAG)
			{
				printf("非法的World_tag=%d 在 world %s\n", gwm->GetWorldTag(), servername);
				return -1;
			}
			//if(_worlds[tag] != NULL)
			if(FindWorld(tag) != NULL)
			{
				printf("重名的World_tag=%d 在 world %s\n", gwm->GetWorldTag(), servername);
				return -2;
			}
			InsertWorldManager(tag, gwm);
		}
		if(atoi(conf->find("General", "is_flow_battle").c_str()))
		{
			_is_flow_battle_server = 1;
		}
	}

	/*
	for(;*ins_name; ins_name ++)
	{
		const char * servername = *world_name;
		instance_world_manager * gwm = new instance_world_manager();
		printf("-----------------初始化副本 %s -------------------\n",servername);
		gwm->Init(gmconf_file,servername);
		int tag = gwm->GetWorldTag();
		if(tag > MAX_WORLD_TAG)
		{
			printf("非法的World_tag=%d 在 instance %s\n", gwm->GetWorldTag(), servername);
			return -1;
		}
		if(_worlds[tag] != NULL)
		{
			printf("重名的World_tag=%d 在 instance %s\n", gwm->GetWorldTag(), servername);
			return -2;
		}
		InsertWorldManager(tag, gwm);
	}
	*/
	printf("-----------------所有世界管理器初始化完成-------------------\n");

	trace_manager::ReleaseElement();	//释放所有的小凸包数据

	// if(!__PRINT_FLAG)
	// {
		// //关掉标准输出和错误输出
		// //外服的gs所在机器没有硬盘, nfs写到manager上的硬盘，如果nfs出问题可能会阻塞gs进程
		// //这种情况下有可能导致gs宕机,这里关闭掉这些输出来减少对manager硬盘的依赖
		// int fd_null = open("/dev/null",O_RDWR);
		// dup2(fd_null,STDOUT_FILENO);
		// dup2(fd_null,STDERR_FILENO);
		// close(fd_null);
	// }

	InitNetClient(gmconf_file);
	//by2021 GLog::init();	//这个的初始化必须在io 库初始化之后
    GLog::init(_server_index);	//这个的初始化必须在io 库初始化之后
	g_timer.set_timer(1,0,0,timer_tick,this);	
	return 0;
}

world_manager*  
gmatrix::FindWorld(int tag)
{
	bool is_raid_or_fbase = false;
	if(tag >= RAID_OR_FBASE_TAG_BEGIN)
	{
		tag -= RAID_OR_FBASE_TAG_BEGIN;
		is_raid_or_fbase = true;
	}
	if(tag < 0 || tag >= MAX_WORLD_TAG)
	{
		printf("Failed to Find world %d\n", is_raid_or_fbase ? tag + RAID_OR_FBASE_TAG_BEGIN : tag);
		return NULL;
	}
	world_manager* manager = _worlds[(unsigned int)tag];
	if(manager && is_raid_or_fbase && ((!manager->IsRaidWorld()&&!manager->IsFacBase()) || manager->IsIdle()))
	{
		printf("Failed to Find raid world %d\n", tag + RAID_OR_FBASE_TAG_BEGIN);
		return NULL;
	}
	if(manager && !is_raid_or_fbase && (manager->IsRaidWorld()||manager->IsFacBase()))
	{
		printf("Failed to Find common world %d\n", tag);
		return NULL;
	}
	return manager;
}

void 
gmatrix::Heartbeat()
{
	abase::RandUniform();
	spin_autolock keeper(_heartbeat_lock);
	_battle_ctrl.Heartbeat();
	w_npc_man.OnHeartbeat();
	w_matter_man.OnHeartbeat();
	w_player_man.OnHeartbeat();

	bool is_delete = false;
	_msg_queue.OnTimer(0,100, is_delete);
	_msg_queue2.OnTimer(0,100, is_delete);

	for(int i = 0; i <= w_max_tag; i ++)
	{
		if(_worlds[i])
		{
			_worlds[i]->Heartbeat();
		}	
	}
	for(int i = w_max_tag + 1; i <= w_max_raid_or_fbase_tag; i ++)
	{
		if(_worlds[i]/* && !_worlds[i]->IsIdle()*/)
		{
			_worlds[i]->Heartbeat();
		}	
	}
}

bool 
gmatrix::InitQuestSystem(const char * path, const char * path2, const char * path3, const char* path4)
{
	return ::InitQuestSystem(path,path2,path3,path4, gmatrix::GetDataMan().GetElementDataMan());
}

enum GS_ATTRIBUTE_MASK
{
	GS_ATTR_HIDE	= 0x01,
	GS_ATTR_CENTRAL = 0x02,
};

bool 
gmatrix::InitNetClient(const char * gmconf)
{
	extern unsigned long _task_templ_cur_version;
	char version[1024];
	int ele_version = ELEMENTDATA_VERSION;
	int task_version = _task_templ_cur_version;
	int gshop_version = item_manager::GetMallTimestamp();
	sprintf(version, "%x%x%x%x%x", ele_version, g_config.element_data_version,  task_version, TaskGetExportVersion(), gshop_version);
	int pksetting = _world_param.pk_forbidden ? 1 : 0;
	int server_attr = 0;
        if(_is_battle_field_server) server_attr |= GS_ATTR_HIDE;
	if(_is_zone_server) server_attr |= GS_ATTR_CENTRAL;
	if(_is_raid_server) server_attr |= GS_ATTR_HIDE;
	if(_is_flow_battle_server) server_attr |= GS_ATTR_HIDE;
   
	GMSV::InitGSP(gmconf,_server_index,_server_name,server_attr,version, pksetting);
	GDB::init_gamedb();
        printf("---------------------InitNetClient初始化完成-------------------\n");
	return true;
}

bool 
gmatrix::InitNPCTemplate()
{
	if(!npc_stubs_manager::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}
	if(!mine_stubs_manager::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}


	if(!recipe_manager::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}

	if(!item_trade_manager::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}

	if(!reputation_shop_manager::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}


	if(!npc_produce_manager::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}

	if(!bf_world_manager_base::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}

	if(!pet_bedge_dataman::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}

	if(!pet_equip_dataman::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}

	if(!mob_active_mount_point_dataman::LoadTemplate(gmatrix::GetDataMan()))
	{
		return false;
	}
	return true;
}

bool 
gmatrix::InitTagList()
{
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	std::string servers = conf->find("General","instance_servers").c_str();
	abase::strtok tok(servers.c_str(),";,\r\n");
	const char * token;
	while((token = tok.token()))
	{       
		if(!*token) continue;
		std::string osvr = section + token;
		int tag = atoi(conf->find(osvr,"tag").c_str());
		if(tag <= 0) return false;
		for(size_t i = 0; i < _instance_tag_list.size(); i ++)
		{
			if(_instance_tag_list[i] == tag)
			{
				return false;
			}
		}
		_instance_tag_list.push_back(tag);
	}
	return true;
}

void 
gmatrix::SendPlaneMessage(world * plane, const MSG & msg)
{
	Instance()->_msg_queue2.AddMsg(plane, msg);
}

void 
gmatrix::SendPlaneMessage(world * plane, const MSG & msg, int latancy)
{
	Instance()->_msg_queue2.AddMsg(plane, msg, latancy);
}

void 
gmatrix::SendWorldMessage(int tag , const MSG & msg)
{
	Instance()->_msg_queue2.AddMsg(tag, msg);
}

void 
gmatrix::SendMessage(const MSG & msg)
{
	ASSERT(msg.target.IsObject());
	Instance()->_msg_queue.AddMsg(msg);
}

void 
gmatrix::SendMessage(const MSG & msg,int latancy)
{
	ASSERT(msg.target.IsObject());
	Instance()->_msg_queue.AddMsg(msg,latancy);
}

void 
gmatrix::SendMessage(const XID * first, const XID * last, const MSG & msg)
{
	Instance()->_msg_queue.AddMultiMsg(first,last,msg);
}

void 
gmatrix::SendPlayerMessage(int * player_list, size_t count, const MSG & msg)
{
	Instance()->_msg_queue.AddPlayerMultiMsg(count, player_list,msg);
}

void 
gmatrix::SendMultiMessage(abase::vector<gobject*,abase::fast_alloc<> > &list, const MSG & msg)
{
	Instance()->_msg_queue.AddMultiMsg(list, msg);
}

void 
gmatrix::SendMultiMessage(abase::vector<gobject*,abase::fast_alloc<> > &list, const MSG & msg,size_t limit)
{
	size_t t = list.size();
	if(t > limit)
	{
		//考虑是否进行打乱操作
		list.erase(list.begin() + limit, list.end());
	}
	Instance()->_msg_queue.AddMultiMsg(list, msg);
}

void 
gmatrix::DispatchWorldMessage(int tag, const MSG & msg)
{
	world_manager * manager = FindWorld(tag);
	if(!manager || manager->IsIdle()) return ;
	manager->HandleWorldMessage(msg);
}


static inline void call_message_handler(gobject * obj, const MSG &msg)
{
	//obj在外面lock
	int rst = 0;
#ifdef _DEBUG
	obj->cur_msg = msg.message;
#endif
	if(obj->imp) rst = obj->imp->DispatchMessage(msg);
	if(!rst)
	{
		ASSERT(obj->spinlock && "这里必须是上锁状态");
		obj->Unlock();
	}
	else
	{
		ASSERT(!obj->spinlock && "没有解开锁，可能是时序问题，但更可能是错误");
	}
}

gobject * 
gmatrix::locate_object_from_msg(const MSG & msg)
{
	int id = msg.target.id;
	switch (msg.target.type)
	{
		case GM_TYPE_PLAYER:
			{
				int index = FindPlayer(id);
				if(index == -1) return NULL;
				return GetPlayerByIndex(index);
			}
		break;
		case GM_TYPE_NPC:
			{
				size_t index = ID2IDX(id);
				if(index >= GetMaxNPCCount())
				{
					//这里是有可能的 因为有时 这个ID来源有时来自客户端
					//所以不再报告ASSERT了
					return NULL;
				}
				gnpc *pNPC = GetNPCByIndex(index);
				if(pNPC->ID.id != id) return NULL;
				return  pNPC;
			}
		break;
		case GM_TYPE_MATTER:
			{
				size_t index = ID2IDX(id);
				if(index >= GetMaxMatterCount())
				{
					//这里是有可能的 因为有时 这个ID来源有时来自客户端
					//所以不再报告ASSERT了
					return NULL;
				}
				gmatter *pMatter = GetMatterByIndex(index);
				if(pMatter->ID.id != id) return NULL;
				return  pMatter;
			}
		break;
		default:
			return NULL;
	}
}

void 
gmatrix::DispatchMessage(const MSG &msg)
{
	gobject * obj = locate_object_from_msg(msg);
	if(obj == NULL || !obj->IsActived()) {
		//找不到指定对象，所以无法发送
		return ;
	}
#ifdef DEBUG
	if(msg.message != 31)   __PRINTF("handle message %d to %p\n",msg.message,obj);
#endif
	//滤掉session repeat的函数

	obj->Lock();
	if(obj->ID.id == msg.target.id)
	{
		call_message_handler(obj,msg);
	}
	else
	{
		obj->Unlock();
	}
	return;
}

void 
gmatrix::DispatchMessage(gobject * obj, const MSG &msg)
{
	obj->Lock();
	if(obj->IsActived()) 
	{
		call_message_handler(obj,msg);
	}
	else 
	{
		obj->Unlock();
	}
	return;
}

void 
gmatrix::UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size, char flag)
{
	if(int rindex = FindPlayer(uid) >= 0)
	{
		GMSV::SendLoginRe(cs_index,uid,cs_sid,3,flag);       // login failed
		GLog::log(GLOG_WARNING,"用户%d已经登录(%d,%d)(%d)",uid,cs_index,cs_sid,GetPlayerByIndex(rindex)->login_state);
		return ;
	}
}

void 
gmatrix::RestartProcess()
{
	//考虑让所有人都断线 
	gplayer * pPool = GetPlayerPool();
	for(size_t i = 0; i<GetMaxPlayerCount(); i ++)
	{
		if(pPool[i].IsEmpty()) continue;
		if(!pPool[i].imp) continue;
		int cs_index = pPool[i].cs_index;
		if(cs_index <=0) continue;

		do {
			spin_autolock keeper(pPool[i].spinlock);
			if(!pPool[i].IsActived()) break;
			if(!pPool[i].imp) break;
			if(pPool[i].login_state != gplayer::LOGIN_OK) break;
			pPool[i].imp->ServerShutDown();
		} while(0);
		GMSV::SendDisconnect(cs_index,pPool[i].ID.id,pPool[i].cs_sid,0);
	}
	if(!fork())
	{
		for(int i =3;i < getdtablesize(); i ++)
		{
			close(i);
		}
		sleep(1);
		system(_restart_shell.c_str());
	}
}

void
gmatrix::ShutDown()
{
	gplayer * pPool = GetPlayerPool();
	for(size_t i = 0; i<GetMaxPlayerCount(); i ++)
	{
		if(pPool[i].IsEmpty()) continue;
		if(!pPool[i].imp) continue;
		spin_autolock keeper(pPool[i].spinlock);
		if(!pPool[i].IsActived()) continue;
		if(!pPool[i].imp) continue;
		if(pPool[i].login_state != gplayer::LOGIN_OK) continue;
		pPool[i].imp->ServerShutDown();
	}
	sleep(15);
	kill(getpid(),15);
}

void
gmatrix::WriteAllUserBack()
{
	gplayer * pPool = GetPlayerPool();
	for(size_t i = 0; i<GetMaxPlayerCount(); i ++)
	{
		if(pPool[i].IsEmpty()) continue;
		if(!pPool[i].imp) continue;
		spin_autolock keeper(pPool[i].spinlock);
		g_d_state.IncShutDownCounter();
		if(!pPool[i].IsActived()) continue;
		if(!pPool[i].imp) continue;
		if(pPool[i].login_state == gplayer::LOGIN_OK)
		{
			pPool[i].imp->ServerShutDown();
		}
		else if(pPool[i].login_state == gplayer::WAITING_ENTER)
		{
			pPool[i].imp->_commander->Release();
		}
	}
}

void gmatrix::InitTopTitle(std::map<int , std::vector<int> > & list)
{
	spin_autolock keeper(_toptitle_lock);
	std::map<int , std::vector<int> >::iterator it = list.begin();
	_toptitle.clear();
	for( ;it != list.end(); ++it)
	{
		std::vector<int> &l1 = it->second;
		if(l1.empty()) continue;
		int userid = it->first;
		for(size_t i = 0; i < l1.size(); i ++)
		{
			_toptitle[userid].push_back(l1[i]);
		}
	}
}

int gmatrix::GetTopTitle(int userid, gplayer_imp * pImp) 
{
	spin_autolock keeper(_toptitle_lock);
	TOPLIST::iterator it = _toptitle.find(userid);
	if(it == _toptitle.end()) return 0;
	abase::vector<short, abase::fast_alloc<> > & l = it->second;
	for(size_t i = 0; i < l.size(); i ++)
	{
		pImp->InsertPlayerExtraTitle(l[i]);
	}
	return l.size();
}

void 
gmatrix::TriggerSpawn(int ctrl_id, bool active)
{
	if(active && forbid_manager::IsForbidCtrlID(ctrl_id))
	{
		return;
	}
	
	if(active)
	{
		GLog::action("trigeron,triggerid=%d", ctrl_id);
	}
	else
	{
		GLog::action("trigeroff,triggerid=%d", ctrl_id);
	}

	CheckCtrlID(ctrl_id, active);

	for(size_t i = 0; i < MAX_WORLD_TAG; i ++)
	{
		world_manager * pManager = _worlds[i];
		if(pManager == NULL || pManager->IsIdle()) continue;
		if(active)
		{
			pManager->GetWorldByIndex(0)->TriggerSpawn(ctrl_id);
		}
		else
		{
			pManager->GetWorldByIndex(0)->ClearSpawn(ctrl_id);
		}
	}
}

void 
gmatrix::TriggerSpawn(int ctrl_id, bool active, int world_tag)
{
	if(active && forbid_manager::IsForbidCtrlID(ctrl_id))
	{
		return;
	}

	if(active)
	{
		GLog::action("local triger on,triggerid=%d, world_tag:%d", ctrl_id, world_tag);
	}
	else
	{
		GLog::action("local triger off,triggerid=%d, world_tag:%d", ctrl_id, world_tag);
	}

	CheckCtrlID(world_tag, ctrl_id, active);
	world_manager * pManager = FindWorld(world_tag);
	if(!pManager || pManager->IsIdle()) return;
	if(active)
	{
		pManager->GetWorldByIndex(0)->TriggerSpawn(ctrl_id);
	}
	else
	{
		pManager->GetWorldByIndex(0)->ClearSpawn(ctrl_id);
	}
}

void
gmatrix::BroadcastAllNPCMessage(MSG & msg)
{
	for(size_t i = 0; i < MAX_WORLD_TAG; i ++)
	{
		world_manager * pManager = _worlds[i];
		if(pManager == NULL || pManager->IsIdle()) continue;
		std::vector<exclude_target> empty;
		pManager->GetWorldByIndex(0)->BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);
	}
}

void
gmatrix::CheckCtrlID(int ctrl_id, bool active)
{
	spin_autolock keeper(_active_ctrl_lock);
	if(active)
	{
		_active_ctrl_list.insert(ctrl_id);
	}
	else
	{
		_active_ctrl_list.erase(ctrl_id);
	}
}

void gmatrix::CheckCtrlID(int world_tag, int ctrl_id, bool active)
{
	spin_autolock keeper(_world_active_ctrl_lock);
	if(active)
	{
		_world_active_ctrl_list[world_tag].insert(ctrl_id);
	}
	else
	{
		_world_active_ctrl_list[world_tag].erase(ctrl_id);
	}
}

void gmatrix::CleanWorldActiveCtrlList(int world_tag)
{
	spin_autolock keeper(_world_active_ctrl_lock);
	WORLD_ACTIVE_CTRL_LIST::iterator it = _world_active_ctrl_list.find(world_tag);
	if(it != _world_active_ctrl_list.end())
	{
		_world_active_ctrl_list.erase(it);
	}
}

void gmatrix::StartEvent(int event_id)
{
	spin_autolock keeper(_event_lock);

	if(_active_event_list.find(event_id) == _active_event_list.end())
	{
		_active_event_list.insert(event_id);

		packet_wrapper h1(64);
		using namespace S2C;
		CMD::Make<CMD::event_start>::From(h1, event_id);

		GMSV::BroadcastLineMessage(GetServerIndex(), h1.data(), h1.size());
	}
}

void gmatrix::StopEvent(int event_id)
{
	spin_autolock keeper(_event_lock);

	if(_active_event_list.find(event_id) != _active_event_list.end())
	{
		_active_event_list.erase(event_id);

		packet_wrapper h1(64);
		using namespace S2C;
		CMD::Make<CMD::event_stop>::From(h1, event_id);

		GMSV::BroadcastLineMessage(GetServerIndex(), h1.data(), h1.size());
	}
}


void gmatrix::GetEventList(std::set<int> & event_list)
{
	spin_autolock keeper(_event_lock);
	event_list = _active_event_list;
}

int
gmatrix::GetRankLevel(int index)
{
	return GMSV::GetRankLevel(index);
}

bool gmatrix::InitOrderItemList(const char* filepath)
{
	char buf[64];
	int line = 1;

	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;
	while(fgets(buf,sizeof(buf), file))
	{
		int item_type = 0;
		sscanf(buf, "%d", &item_type);
		std::map<int, int>::iterator it = _order_item_list.find(item_type);
		if(item_type < 0 || it != _order_item_list.end())
		{
			fclose(file);
			printf("整理物品顺序列表数据错误, [Line %d]: %s", line, buf);
			return false;
		}
		_order_item_list.insert(std::make_pair(item_type,line));
		line++;
	}
	fclose(file);
	return true;
}

bool gmatrix::InitConsignItemList(const char* filepath)
{
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;
	char buf[64];
	int cnt = 1;
	while(fgets(buf,sizeof(buf), file))
	{
		int id = 0;
		int type = 0;
		sscanf(buf, "%d,%d", &type, &id);
		std::map<int, int>::iterator it = _consign_item_list.find(id);
		if(id < 0 || type <=0 || it != _consign_item_list.end())
		{
			fclose(file);
			printf("寄售物品列表数据错误, [Line %d]: %s", cnt, buf);
			return false;
		}
		_consign_item_list[id] = type;
		cnt ++;
	}
	fclose(file);
	return true;
}

/**
 *@Brief: 	Add By Houjun 2010-08-11, 
 *@Return: 	物品存在, 返回>0的物品ID; 物品不存在, 返回-1;
 */
int gmatrix::GetConsignItemType(int item_id)
{
	std::map<int, int>::iterator it = _consign_item_list.find(item_id);
	if(it != _consign_item_list.end())
	{
		return it->second;
	}
	return -1;
}

bool gmatrix::InitLotteryGiveItemProb(const char* filepath)
{
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;
	char buf[128];
	if(!fgets(buf,sizeof(buf), file))
	{	
		fclose(file);
		return false;
	}
	sscanf(buf, "%d", &lottery_give_item_prob);
	if(!fgets(buf,sizeof(buf), file))
	{
		fclose(file);
		return false;
	}
	sscanf(buf, "%d,%d,%d,%d,%d", &lottery_ty_exp_probs[0], &lottery_ty_exp_probs[1], &lottery_ty_exp_probs[2], &lottery_ty_exp_probs[3], &lottery_ty_exp_probs[4]);
	int v = 0;
	for(int i = 0; i < 5; i ++)
	{
		v += lottery_ty_exp_probs[i];
	}
	if(v != 1000)
	{
		fclose(file);
		return false;
	}
	printf("Lottery GiveItemProb=%d, TY_EXP_PROB=[%d,%d,%d,%d,%d]\n", lottery_give_item_prob, lottery_ty_exp_probs[0], lottery_ty_exp_probs[1], lottery_ty_exp_probs[2], lottery_ty_exp_probs[3], lottery_ty_exp_probs[4]);
	fclose(file);
	return true;
}

std::string gmatrix::SimpleTrim(const std::string & str)
{
	if(str.empty()) return str;

	std::string::size_type pos = str.find_first_of(" \t\n\r\0\x0B");
	if(pos == std::string::npos)
	{
		return str;
	}

	std::string::size_type pos2 = str.find_last_not_of(" \t\n\r\0\x0B");
	return str.substr(pos, pos2 - pos + 1);
}


bool gmatrix::InitCashGiftInfo(const char * filepath)
{
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;

	char buf[128];
	int gift_id1;
	int gift_id2;
	int gift_award_id;
	char timestamp[32];
	bool cur_flag = false;			//是否配置了当期的奖励物品
	std::set<int> item_list;		//检查id不能重复
	
	while(fgets(buf,sizeof(buf), file))
	{
		if(buf[0] == '\n') continue;
		//赠品1ID 赠品2ID 消费礼包ID
		int ret = sscanf(buf, "%d,%d,%d,%s", &gift_id1, &gift_id2, &gift_award_id, timestamp);
		if(ret != 4 || gift_id1 <= 0 || gift_id2 <= 0 || gift_award_id < 0)
		{
			printf("读取商城买赠配置发生错误,请确保为逗号分隔格式\n");
			return false;
		}	

		//检查是否有重复的物品id
		if(!CheckGiftItem(item_list, gift_id1) || !CheckGiftItem(item_list, gift_id2) || !CheckGiftItem(item_list, gift_award_id)) 
		{
			printf("商城买赠发现重复的物品id\n");
			return false;
		}

		//检查是否是当期，当期没有奖励配置
		if(gift_award_id <= 0)	
		{
			if(cur_flag)
			{
				printf("当期商城买赠奖品已经配置 \n");
				return false;
			}

			_cash_gift_info.cur_gift_id1 = gift_id1;
			_cash_gift_info.cur_gift_id2 = gift_id2;
			cur_flag = true;
			continue;
		}

		//检查礼包id类型是否正确 
		DATA_TYPE dt;
		const GIFT_PACK_ITEM_ESSENCE &ess= *(const GIFT_PACK_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(gift_award_id, ID_SPACE_ESSENCE,dt); 
		if(dt != DT_GIFT_PACK_ITEM_ESSENCE|| &ess == NULL)
		{
			printf("商城买赠信息里面的礼包id类型不正确, id=%d\n", gift_award_id);
			return false;
		}
	

		int year;
		int month;
		int day;
		if(sscanf(timestamp, "%d-%d-%d", &year, &month, &day) != 3)
		{
			printf("读取商城买赠配置的奖品发放时间的时候发生错误\n");
			return false;
		}
		if(year < 2013 || year > 3000 || month <= 0 || month > 12 || day <= 0 || day > 31)
		{
			printf("读取商城买赠配置的奖品发放时间的时候发生错误,年月日不符合规范\n");
			return false;
		}
		
		struct tm tt;
		memset(&tt, 0, sizeof(tt));
		tt.tm_year = year-1900;
		tt.tm_mon = month-1;
		tt.tm_mday = day;

		time_t award_timestamp = mktime(&tt);
		if(award_timestamp == -1)
		{
			printf("读取商城买赠配置的奖品发放时间的时候发生错误,秌time错误\n");
			return false;
		}

		cash_gift_award gift_award;
		gift_award.gift_id1 = gift_id1;
		gift_award.gift_id2 = gift_id2;
		gift_award.award_id = gift_award_id; 
		gift_award.award_timestamp = award_timestamp;

		_cash_gift_info.gift_award_list.push_back(gift_award);
	}


	if(!cur_flag)
	{
		printf("没有配置当期商城买赠赠品id\n");
		return false;
	}
	
	fclose(file);
	return true;
}


//检查配置的商城返赠物品id不能重复
//赠品1ID和赠品2ID在每一期都不能重复
//消费礼包ID不能和赠品ID一样
//消费礼包每一期的ID也不一样
bool gmatrix::CheckGiftItem(std::set<int> & item_list, int item_id)
{
	if(item_list.find(item_id) != item_list.end()) 
	{
		return false;
	}
	item_list.insert(item_id);
	return true;
}

void gmatrix::ConsumeKingdomPoint(int point)
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);

	_kingdom_info.is_dirty = true;
	_kingdom_info.point -= point;
	GMSV::SendChangeKingdomPoint(-point);
}

void gmatrix::ObtainKingdomPoint(int point)
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);

	_kingdom_info.is_dirty = true;
	_kingdom_info.point += point;
	GMSV::SendChangeKingdomPoint(point);
}

void gmatrix::SyncKingdomPoint(int point)
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);

	_kingdom_info.point = point;
	_kingdom_info.is_dirty = false;
}

void gmatrix::SyncKingdomInfo(int mafia_id, int kingdom_point, std::map<int, int> & kingdom_title, int task_type)
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);

	_kingdom_info.mafia_id = mafia_id;
	_kingdom_info.point = kingdom_point;
	_kingdom_info.is_dirty = false;

	_kingdom_info.kingdom_title.clear();
	_kingdom_info.kingdom_title = kingdom_title;
	_kingdom_info.kingdom_task_type = task_type;

}

int gmatrix::GetKingdomTitle(int roleid)
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);

	std::map<int, int>::iterator iter;
	if( (iter = _kingdom_info.kingdom_title.find(roleid)) != _kingdom_info.kingdom_title.end())
	{
		return iter->second;
	}
	return 0; 
}

void gmatrix::OpenBath()
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);


	world_manager* manager = gmatrix::Instance()->FindWorld( KINGDOM_BATH_TAG_ID);
	if(manager && manager->IsBattleWorld())
	{
		int defender = 0;
		std::vector<int>  attacker_list;
		manager->CreateKingdom(3, KINGDOM_BATH_TAG_ID, defender, attacker_list);
		_kingdom_info.is_bath_open = true;
	}
}

void gmatrix::CloseBath()
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);

	world_manager* manager = gmatrix::Instance()->FindWorld( KINGDOM_BATH_TAG_ID);
	if(manager && manager->IsBattleWorld())
	{
		manager->StopKingdom(3, KINGDOM_BATH_TAG_ID);
	}

	_kingdom_info.is_bath_open = false;
	ClearBathCount();
}

void gmatrix::BathClosed()
{
	_kingdom_info.is_bath_open = false;
	ClearBathCount();
}

bool gmatrix::IsBathOpen()
{
	spin_autolock keeper(_kingdom_info.kingdom_lock);
	return _kingdom_info.is_bath_open;
}

void gmatrix::SyncBathCount(int id, int count)
{
	spin_autolock keeper(_kingdom_info.bath_count_lock);
	_kingdom_info.bath_count_map[id] = count;
	
}

void gmatrix::ClearBathCount()
{
	spin_autolock keeper(_kingdom_info.bath_count_lock);
	_kingdom_info.bath_count_map.clear();
}

int gmatrix::GetBathCount(int id)
{
	spin_autolock keeper(_kingdom_info.bath_count_lock);

	std::map<int,int>::iterator iter;
	iter = _kingdom_info.bath_count_map.find(id);

	if(iter != _kingdom_info.bath_count_map.end())
	{
		return iter->second;
	}
	return -1;
}

bool gmatrix::InitConsumptionValueList(const char * filename)
{
	if(!ReadConsumpConfigfile(filename, "EquipConsumption", _equip_consumption_map)) return false;
	if(!ReadConsumpConfigfile(filename, "UseItemConsumption", _useitem_consumption_map)) return false;
	if(!ReadConsumpConfigfile(filename, "SpecialConsumption", _special_consumption_map)) return false;

	return true;
}

bool gmatrix::ReadConsumpConfigfile(const char * filename, const std::string & keyword, ConsumptionValueConfigMap & options)
{
	std::ifstream ifs(filename);
	if(!ifs.is_open())
	{
		printf("无法打开配置文件consumptione_value_list.txt\n");
		return false;
	}

	std::string line = "";
	std::string::size_type pos	= std::string::npos;
	std::string::size_type pos2	= std::string::npos;


	while(!ifs.eof())
	{
		getline(ifs, line);

		if(line.empty() || line.at(0) == '#')
		{
			continue;
		}
		line = SimpleTrim(line);

		if((pos = line.find('[')) == std::string::npos || (pos2 = line.find(']')) == std::string::npos)
		{
			continue;
		}

		std::string tmpkeyword = line.substr(pos+1, pos2-1);
		if(tmpkeyword == keyword)
		{
			while(!ifs.eof())
			{
				getline(ifs, line);	
				if(line.empty() || line.at(0) == '#')
				{
					continue;
				}
				line = SimpleTrim(line);

				if((pos = line.find('[')) != std::string::npos)
				{
					break;
				}

				if((pos = line.find(',')) == std::string::npos)
				{
					printf("ERR:消费值对照表格式不对\n");
					return false;
				}
				
				int item_id		= 0;
				float ivalue	= 0.0f;
				sscanf(line.c_str(), "%d,%f", &item_id, &ivalue);
				if(item_id < 0 || ivalue < 1/CONSUMPTION_SCALE || ((ivalue >= -0.00001) && (ivalue <= 0.00001)))
				{
					printf("ERR:消费值或者item_type小于零，是非法值！\n");
					return false;
				}

				ConsumptionValueConfigMap::iterator it = options.find(item_id);
				if(it != options.end())
				{
					printf("ERR:消费值有重复的item_type:%d\n", item_id);
					return false;
				}

				//消费值配置表里的数据最小能支持到0.0001的精度
				//调整为整形
				options[item_id] = (int)(ivalue * CONSUMPTION_SCALE);
			}

			return true;
		}
	}

	return false;
}

int gmatrix::GetConsumptionValue(CONSUMPTION_TYPE type, int item_type)
{
	int ret = -1;

	switch(type)
	{
	case EQUIP_CONSUMPTION:
		{
			ConsumptionValueConfigMap::iterator iter = _equip_consumption_map.find(item_type);
			if(iter != _equip_consumption_map.end())
			{
				ret = iter->second;
			}
		}
		break;

	case USEITEM_CONSUMPTION:
		{
			ConsumptionValueConfigMap::iterator iter = _useitem_consumption_map.find(item_type);
			if(iter != _useitem_consumption_map.end())
			{
				ret = iter->second;
			}
		}
		break;

	case SPECIAL_CONSUMPTION:
		{
			ConsumptionValueConfigMap::iterator iter = _special_consumption_map.find(item_type);
			if(iter != _special_consumption_map.end())
			{
				ret = iter->second;
			}
		}
		break;

	default:
		return -1;
	}

	return ret;
}

bool gmatrix::InitFuwenComposeInfo(const char * filepath)
{
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;

	char buf[128];
	int fuwen_id;
	float prob;
	fuwen_compose fc;
	float total_prob = 0.f;
	while(fgets(buf,sizeof(buf), file))
	{
		if(buf[0] == '\n') continue;
		int ret = sscanf(buf, "%d %f", &fuwen_id, &prob);
		if(ret != 2 || fuwen_id <= 0 || prob < 0)
		{
			printf("读取符文碎片合成的配置的时候发生错误\n");
			return false;
		}	

		//检查礼包id类型是否正确 
		DATA_TYPE dt;
		const RUNE2013_ITEM_ESSENCE &ess= *(const RUNE2013_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(fuwen_id, ID_SPACE_ESSENCE,dt); 
		if(dt != DT_RUNE2013_ITEM_ESSENCE|| &ess == NULL)
		{
			printf("读取符文碎片合成的配置的时候发生错误, 符文id的类型不正确, id=%d\n", fuwen_id);
			return false;
		}

		fc.fuwen_id = fuwen_id;
		fc.prob = prob;

		_fuwen_compose_list.push_back(fc);
		total_prob += prob;
	}

	if(fabs(1.0f - total_prob) > 1e-6)
	{
		printf("读取符文碎片合成的配置的时候发生错误, 总概率不为1, total_prob=%f\n", total_prob); 
		return false;
	}	
	return true;
}

int gmatrix::GetFuwenComposeID()
{
	if(_fuwen_compose_list.size() == 0) return 0;
	
	float * prob = &(_fuwen_compose_list[0].prob); 
	int index = abase::RandSelect(prob, sizeof(fuwen_compose), _fuwen_compose_list.size());
	return _fuwen_compose_list[index].fuwen_id;
}

bool gmatrix::InitGTBuffInfo(const char * filepath)
{
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;

	char buf[128];
	int skill_id;
	int buff_id;
	if(fgets(buf,sizeof(buf), file))
	{
		int ret = sscanf(buf, "%d,%d", &skill_id, &buff_id);
		if(ret != 2 || buff_id <= 0 || skill_id < 0)
		{
			printf("读取GT buff配置的时候发生错误\n"); 
			return false;
		}	

		
		_gt_buff_id.skill_id = skill_id;
		_gt_buff_id.buff_id = buff_id;
		return true;
	}
	return false;
}

//创建副本世界管理器
int gmatrix::CreateRaidWorldManager(int raid_id, int raidroom_id, const std::vector<int>& roleid_list, int& world_tag, const void* buf, size_t size, char difficulty)
{
	spin_autolock alock(_raid_lock);

	world_tag = -1;
	const raid_world_template* t = w_raid_controller.GetRaidWorldTemplate(raid_id);
	if(!t)
	{
		__PRINTF("副本raid_id=%d实例创建失败，raid_id无效\n", raid_id);
		GLog::log( GLOG_INFO, "副本raid_id=%d实例创建失败，raid_id无效\n", raid_id);
		return -50001;
	}	
	if(t->cur_instance_num >= t->rwinfo.raid_max_instance)
	{
		__PRINTF("副本raid_id=%d实例创建失败，实例数已经到达上限%d\n", raid_id, t->rwinfo.raid_max_instance);
		GLog::log( GLOG_INFO, "副本raid_id=%d实例创建失败，实例数已经到达上限%d\n", raid_id, t->rwinfo.raid_max_instance);
		return -50002;
	}
	raid_world_manager* rwm = NULL;
	for(int i = w_max_tag + 1; i <= w_max_raid_or_fbase_tag; i ++)
	{
		if(_worlds[i] && _worlds[i]->IsRaidWorld() && _worlds[i]->IsIdle() && t->rwinfo.raid_type == _worlds[i]->GetRaidType())
		{
			rwm = dynamic_cast<raid_world_manager*>(_worlds[i]);
			if(rwm)
			{
				int tag = i + RAID_OR_FBASE_TAG_BEGIN;
				printf("-----------------创建Raid副本 %s 实例 world_tag=%d-------------------\n",t->servername.c_str(), tag);
				int rst = rwm->TestCreateRaid(*t, tag, tag, roleid_list, buf, size, difficulty);
				if(rst == -1)
				{
					continue;
				}
				else if(rst == -2)
				{
					__PRINTF("副本raid_id=%d实例创建失败，初始化失败rst=%d\n", raid_id, rst);
					GLog::log( GLOG_INFO, "副本raid_id=%d实例创建失败，初始化失败\n", raid_id);
					return -50006;
				}
				rst = rwm->CreateRaid(raidroom_id, *t, tag, tag, roleid_list, buf, size, difficulty);
				if(rst)
				{
					rwm->Release();
					__PRINTF("副本raid_id=%d实例创建失败，初始化失败rst=%d\n", raid_id, rst);
					GLog::log( GLOG_INFO, "副本raid_id=%d实例创建失败，初始化失败\n", raid_id);
					return -50005;
				}
				world_tag = tag;
				gmatrix::IncRaidWorldInstance(raid_id);
				return 0;
			}
		}
	}
	__PRINTF("没有空闲的副本世界管理器创建副本实例raid_id=%d, raid_type=%d\n", raid_id, t->rwinfo.raid_type);
	GLog::log( GLOG_INFO, "没有空闲的副本世界管理器创建副本实例raid_id=%d, raid_type=%d\n", raid_id, t->rwinfo.raid_type);
	return -50004;
}

//释放副本世界管理器
int gmatrix::ReleaseRaidWorldManager(int tag)
{
	world_manager* wm = FindWorld(tag);
	if(!wm || !wm->IsRaidWorld() || wm->IsIdle())
	{
		return -1;
	}

	raid_world_manager* rwm = dynamic_cast<raid_world_manager*>(wm);
	if(!rwm)
	{
		return -1;
	}
	printf("##############找到副本%d,卸载实例###############\n", tag);
	rwm->ForceCloseRaid();
	return 0;
}

//int gmatrix::ReleaseAllRaidWorldManager()
int gmatrix::WorldManagerOnDisconnect()
{
	for(int i = w_max_tag + 1; i <= w_max_raid_or_fbase_tag; ++i)
	{
//		ReleaseRaidWorldManager(i);
		world_manager * wm = _worlds[i]; //此处不能用FindWorld 会返回 NULL
		if (wm)
			wm->OnDisconnect();
	}
	return 0;
}

int gmatrix::CloseAllFlowBattleManager()
{
	for(int i = 0; i <= w_max_tag; ++i)
	{
		world_manager* wm = FindWorld(i);
		if(!wm || !wm->IsBattleWorld())
		{
			continue;
		}
		bg_world_manager* bwm = dynamic_cast<bg_world_manager*>(wm);
		if(!bwm || bwm->GetBattleType() != BT_FLOW_CRSSVR)
		{
			continue;
		}
		bwm->ForceClose();
	}
	return 0;
}

void gmatrix::GetRaidWorldInfos(std::vector<raid_world_info>& infos)
{
	w_raid_controller.GetRaidWorldInfos(infos);
}

raid_world_template* gmatrix::GetRaidWorldTemplate(int raid_id)
{
	return w_raid_controller.GetRaidWorldTemplate(raid_id);
}

void gmatrix::IncRaidWorldInstance(int raid_id)
{
	w_raid_controller.IncRaidWorldInstance(raid_id);
}

void gmatrix::DecRaidWorldInstance(int raid_id)
{
	w_raid_controller.DecRaidWorldInstance(raid_id);
}

bool mafia_free_battle::InsertBattle(int attacker, int defender, int end_time)
{
	spin_autolock keeper(_battle_lock);
	if(_table.nGet(attacker) ) return false;
	if(_table.nGet(defender) ) return false;		//判断两个帮都不在战斗中
	if(end_time <= g_timer.get_systime()) return false; 	//未开始就已经结?
	node_t  node = {attacker, defender, end_time};
	hash_node_t hn = {-1, end_time};

	UpdateEndTime(end_time);
	_battle_counter = SECOND_TO_TICK(5.0f);	
	//5秒内每个玩家都要进行帮派是否在战斗的检查，每个不符合帮派玩家会被检查五次
	//这个开销应当不算太大

	//加入至列表
	_list.push_back(node);
	bool bRst = _table.put(attacker, (hn.enemy=defender, hn)) && _table.put(defender, (hn.enemy=attacker, hn));
	ASSERT(bRst);
	return true;
}

void mafia_free_battle::Heartbeat()
{
	spin_autolock keeper(_battle_lock);
	if(_battle_counter > 0) _battle_counter --;
	//每次都判断是否过快了？实际上每1秒判断一次即可，不过先不管了
	if(_list.empty()) return;
	int t = g_timer.get_systime();
	if(!TestEndTime(t)) return;

	ClearEndTime();
	for(size_t i = 0; i < _list.size();)
	{
		if(_list[i].end_time <= t)
		{
			//删除战斗信息
			_table.erase(_list[i].attacker);
			_table.erase(_list[i].defender);
			_list.erase(_list.begin() + i);
		}
		else
		{
			UpdateEndTime(_list[i].end_time);
			i ++;
		}
	}
}


lua_State * gmatrix::AttachScript(size_t index )
{
	mutex_spinlock(&_global_lua_lock[index]);
	return _global_lua_state[index];
}


void gmatrix::DetachScript(size_t index)
{
	mutex_spinunlock(&_global_lua_lock[index]);
}

int gmatrix::script_AddActivityInfo(lua_State *L)
{
	int n = lua_gettop(L);    /* number of arguments */
	if(n != 5) return 0;	// self, x, y, z; errror???

	int id;
	int map_id;
	float x, y, z;
	id = lua_tointeger(L, 1);
	map_id = lua_tointeger(L, 2);
	x = lua_tonumber(L, 3);
	y = lua_tonumber(L, 4);
	z = lua_tonumber(L, 5);

	if(id > 0 && map_id > 0)
	{
		activity_info info = {map_id, x, y, z};
		_activity_info_list[id] = info; 
	}
	return 0;
}

namespace abase
{
	void _t_RecordTimer(int index, abase::timer_task* t, int tx1, int rst)
	{
		return ;	//$$$$$$现在不记录timer的调用情况了
		const char * name = typeid(*t).name();
		if(strstr(name,"npc")) return;
		if(tx1 == 0)
		{
			GLog::log(GLOG_INFO, "分配定时器%d到%p(%s),结果%d",index, t, name, rst);
		}
		else if(tx1 == 1)
		{
			GLog::log(GLOG_INFO, "释放定时器%d为%p(%s),结果%d",index, t, name, rst);
		}
		else if(tx1 == 2)
		{
			GLog::log(GLOG_INFO, "自放定时器%d为%p(%s),结果%d",index, t, name, rst);
		}
	}
}


bool faction_hostiles_manager::RefreshHostileInfo( unsigned int faction_id, int& refresh_seq,
		abase::static_set<unsigned int>& hostiles, bool bForce )
{
	spin_autolock alock( _map_lock );
	if( _refresh_seq == refresh_seq && !bForce )
	{
		// 列表没有更新，并且不要求强制更新
		return false;
	}
	refresh_seq = _refresh_seq;
	hostiles.clear();
	hostiles.reserve(5);
	for( abase::static_set<unsigned int>::iterator it = _fh_map[faction_id].begin(); it != _fh_map[faction_id].end(); ++it )
	{
		hostiles.insert( *it );
	}
	return true;
}

/*
bool faction_hostiles_manager::IsHostileFaction( unsigned int faction_id, unsigned int enemy_faction_id )
{
	spin_autolock alock( _map_lock );
	std::set<unsigned int>::iterator it = _fh_map[faction_id].find( enemy_faction_id );
	if( _fh_map[faction_id].end() == it ) return false;
	return true;
}
*/

void faction_hostiles_manager::SyncFactionHostile( unsigned int faction_id, unsigned int hostile )
{
	if( faction_id == hostile ) return; // 自己帮派的信息是不会加进来的
	spin_autolock alock( _map_lock );
	_fh_map[faction_id].insert( hostile );
	if( _fh_map[faction_id].size() > 5 )
	{
		GLog::log( GLOG_INFO, "同一帮派的敌对帮派超过5个，不该收到这样的delivery消息，查delivery日志,%d-%d",
				faction_id, hostile );
	}
	++_refresh_seq;
}

void faction_hostiles_manager::AddFactionHostile( unsigned int faction_id, unsigned int hostile )
{
	SyncFactionHostile( faction_id, hostile );
}

void faction_hostiles_manager::DelFactionHostile( unsigned int faction_id, unsigned int hostile )
{
	spin_autolock alock( _map_lock );
	_fh_map[faction_id].erase( hostile );
	++_refresh_seq;
}

void faction_hostiles_manager::Heartbeat()
{
	static unsigned int counter = 0;
	++counter;
	if( counter >= 20 * 5 )
	{
		FH_MAP::iterator it;
		abase::static_set<unsigned int>::iterator set_it;
		spin_autolock alock( _map_lock );
		__PRINTF( "帮派敌对信息:\n" );
		for( it = _fh_map.begin(); it != _fh_map.end(); ++it )
		{
			__PRINTF( "\t帮派 %d 的敌对帮派有:  ", it->first );
			for( set_it = it->second.begin(); set_it != it->second.end(); ++ set_it )
			{
				__PRINTF( "%d  ", *set_it );
			}
			__PRINTF( "\n" );
		}
		// 校验数据完整性
		for( it = _fh_map.begin(); it != _fh_map.end(); ++it )
		{
			for( set_it = it->second.begin(); set_it != it->second.end(); ++ set_it )
			{
				if( _fh_map[*set_it].find( it->first ) == _fh_map[*set_it].end() )
				{
					// 有问题了
					__PRINTF( "敌对帮派数据不一致 %d -- %d\n", it->first, *set_it );
				}
			}
		}
		counter = 0;
	}
}

int gmatrix::CreateFacBaseManager(int fid)
{
	if (_facbase_info.world_tag <= 0 || _facbase_info.max_instance <= 0)
	{
		GLog::log(GLOG_INFO, "帮派 %d 基地创建失败， 未配置基地", fid);
		return -2;
	}
	/*
	if (_facbase_info.cur_num >= _facbase_info.max_instance)
	{
		GLog::log(GLOG_INFO, "帮派 %d 基地创建失败, 实例数已达上限 %d:%d\n", fid, _facbase_info.cur_num, _facbase_info.max_instance);
		return -3;
	}
	*/
	facbase_world_manager* fwm = NULL;
	for (int i = w_max_tag + 1; i <= w_max_raid_or_fbase_tag; i++)
	{
		if(_worlds[i] && _worlds[i]->IsFacBase() && _worlds[i]->IsFree())
		{
			fwm = dynamic_cast<facbase_world_manager*>(_worlds[i]);
			if(fwm)
			{
//				int tag = i + FACBASE_TAG_BEGIN;
				printf("创建基地实例 fid %d tag %d\n", fid, i);
				int rst = fwm->TestAllocBase(i, fid);
				if (rst == -1)
					continue;
				return 0;
				/*
				if(rst == -1)
				{
					continue;
				}
				else if(rst == -2)
				{
					__PRINTF("副本raid_id=%d实例创建失败，初始化失败rst=%d\n", raid_id, rst);
					GLog::log( GLOG_INFO, "副本raid_id=%d实例创建失败，初始化失败\n", raid_id);
					return -50006;
				}
				rst = rwm->CreateRaid(raidroom_id, *t, tag, tag, roleid_list, buf, size, difficulty);
				if(rst)
				{
					rwm->Release();
					__PRINTF("副本raid_id=%d实例创建失败，初始化失败rst=%d\n", raid_id, rst);
					GLog::log( GLOG_INFO, "副本raid_id=%d实例创建失败，初始化失败\n", raid_id);
					return -50005;
				}
				world_tag = tag;
				gmatrix::IncRaidWorldInstance(raid_id);
				return 0;
				*/
			}
		}
	}
	GLog::log(GLOG_ERR, "没有空闲的基地可创建 fid %d", fid);
	return -4;
}

