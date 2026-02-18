#include <stdio.h>
#include <conf.h>
#include "bf_player_imp.h"
#include "bf_world_manager.h"
#include "bf_world_message_handler.h"
#include <factionlib.h>
#include "../template/itemdataman.h"
#include "../npc.h"

int bf_world_manager_base::_city_detail_lock = 0;
gs_city_detail bf_world_manager_base::_city_detail;

struct battlefiled_msg_t
{
	int type;		//消息类型 0 捐献 1 建设
	int player_id;		//事件发起人
	char name[MAX_USERNAME_LENGTH];
	int data1;		//type 为 0 时 表示 捐献了什么 type 为 1 时表示建设了什么(key)
	int data2;		//type 为 0 时 表示 捐献了多少 type 为 1 时表示建筑的级别
	int data3;		//type 为 1 时 表示 建设类型(攻击,防御,什么的)
};

bool player_contribution::AddContribution(int player_id,size_t index,size_t add_num)
{
	if(0 == add_num) return false;
	PLAYER_CONTRIBUTION_VECTOR::iterator it = Find(player_id);
	PLAYER_CONTRIBUTION_VECTOR::iterator it2;
	if(it != player_contri_vec.end())
	{
		//已经存在
		it->AddContributionNum(index,add_num);
		uint64_t sum = it->GetContributionSum();
		it2 = it;
		while(it2 != player_contri_vec.begin())
		{
			--it2;
			if(it2->GetContributionSum() > sum) break;
		}
		player_contribution_node node = *it;
		if(it2->GetContributionSum() > sum)
		{
			++it2;
		}
		for(;it != it2;--it)
		{
			*it = *(it-1);
		}
		*it2 = node;
	}
	else
	{
		//加入新条目
		it2  = Find((uint64_t)add_num);
		player_contribution_node node(player_id);
		node.AddContributionNum(index,add_num);
		player_contri_vec.insert(it2,node);
	}
	return true;
}


bool bf_world_manager_base::LoadTemplate(itemdataman& dataman)
{
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(;id != 0;id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(DT_WAR_ROLE_CONFIG == dt)
		{
			const WAR_ROLE_CONFIG& war_role =
				*(const WAR_ROLE_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&war_role && DT_WAR_ROLE_CONFIG == dt);
			battlefield_consturct_temp bfct;
			memset(&bfct,0,sizeof(battlefield_consturct_temp));
			bfct.tid = id;
			bfct.attack_max_lev = war_role.attack_max_lev;
			memcpy(bfct.attack_level,war_role.attack_level,sizeof(int) * 2 * 8);
			bfct.attack_war_material = war_role.attack_war_material;
			memcpy(bfct.attack_co,war_role.attack_co,sizeof(float) * 3);
			bfct.defence_max_lev = war_role.defence_max_lev;
			memcpy(bfct.defence_level,war_role.defence_level,sizeof(int) * 2 * 8);
			bfct.defence_war_material = war_role.defence_war_material;
			memcpy(bfct.defence_co,war_role.defence_co,sizeof(float) * 3);
			bfct.range_max_lev = war_role.range_max_lev;
			memcpy(bfct.range_values,war_role.range_values,sizeof(int) * 5);
			bfct.range_war_material = war_role.range_war_material;
			memcpy(bfct.range_co,war_role.range_co,sizeof(float) * 3);
			bfct.strategy_max_lev = war_role.strategy_max_lev;
			memcpy(bfct.strategy_id,war_role.strategy_id,sizeof(int) * 5);
			bfct.strategy_war_material = war_role.strategy_war_material;
			memcpy(bfct.strategy_co,war_role.strategy_co,sizeof(float) * 3);
			bfct.init_hp = war_role.init_hp;
			bfct.hp_war_material = war_role.hp_war_material;
			bfct.lvlup_hp= war_role.lvlup_hp;
			bfct.hp_material_num = war_role.hp_material_num;
			Insert(bfct);
		}
	}
	__PRINTF("城战建筑模版数据整理完毕,模版共有 %d 种建筑\n",__GetInstance()._bfc_map.size());
	return true;
}

bf_world_manager_base::bf_world_manager_base():_bfc_map(1024)
{
	_battle_id = 0;
	_active_ctrl_id_prepare = 0;
	_active_ctrl_id_prepare2 = 0;
	_active_ctrl_id_start = 0;
	_active_ctrl_id_end = 0;
	_kickout_time_min = 0;
	_kickout_time_max = 0;
	_kickout_close_time = 0;
	_cur_timestamp = 0;
	_end_timestamp = 0;
	_heartbeat_counter = 0;
	_battle_info_seq = 0;
	_status = 0;
	_battle_result = BR_NONE;
	_battle_lock = 0;
	_get_counter = DEFAULT_RETRY_COUNT;
	_put_counter = DEFAULT_RETRY_COUNT;

	_key_tid_map[14280] = 1;
	_key_tid_map[14281] = 2;
	_key_tid_map[14282] = 3;
	_key_tid_map[14283] = 4;
	_key_tid_map[14285] = 5;
	_key_tid_map[14286] = 6;
	_key_tid_map[14284] = 7;
	_key_tid_map[15842] = 8;
	_key_tid_map[15841] = 9;
	_key_tid_map[15840] = 10;

	_key_map[1].tid = 14280;
	_key_map[1].npc_tid = 14254;
	_key_map[2].tid = 14281;
	_key_map[2].npc_tid = 14255;
	_key_map[3].tid = 14282;
	_key_map[3].npc_tid = 14256;
	_key_map[4].tid = 14283;
	_key_map[4].npc_tid = 14257;
	_key_map[5].tid = 14285;
	_key_map[5].npc_tid = 14250;
	_key_map[6].tid = 14286;
	_key_map[6].npc_tid = 14249;
	_key_map[7].tid = 14284;
	_key_map[7].npc_tid = 14248;
	_key_map[8].tid = 15842;
	_key_map[8].npc_tid = 14247;
	_key_map[9].tid = 15841;
	_key_map[9].npc_tid = 14246;
	_key_map[10].tid = 15840;
	_key_map[10].npc_tid = 14245;
	//用来显示界面
	_key_map[11].npc_tid = 14258;
	_key_map[12].npc_tid = 15945;
	_key_map[13].npc_tid = 14234;
	_key_map[14].npc_tid = 14262;

	for(int i = 1;i <= 14;++i)
	{
		_key_map[i].hp_factor = 1.0f;
		_key_map[i].pos = A3DVECTOR(0,0,0);
		_key_map[i].status = 0;
	}
}

bf_world_manager_base::~bf_world_manager_base()
{
}

void bf_world_manager_base::AdjustNPCAttrInBattlefield(gnpc_imp* pImp,ai_param& aip,int ai_cid)
{
	//统一的城战调整
	npc_template* pTemplate = npc_stubs_manager::Get(pImp->GetParent()->tid);
	if(!pTemplate) return;
	for(int i = 11;i < 14;++i)
	{
		if(_key_map[i].npc_tid == pImp->GetParent()->tid)
		{
			_key_map[i].pos = pImp->GetParent()->pos;
			_key_map[i].status = 1;
			_key_map[i].hp_factor = 1.0f;
		}
	}
	if(0 == pTemplate->war_role_config) return;
	const battlefield_consturct_temp* bfct = Get(pTemplate->war_role_config);
	if(!bfct) return;
	if(_key_tid_map.find(pTemplate->war_role_config) == _key_tid_map.end())
	{
		//没有这个key值
		return;
	}
	int key = _key_tid_map[pTemplate->war_role_config];
	if(key < 1 || key > 10)
	{
		//非法key
		return;
	}
	_key_map[key].pos = pImp->GetParent()->pos;
	_key_map[key].status = 1;
	_key_map[key].hp_factor = 1.0f;
	//这里进行调整 根据 bfct
	spin_autolock keeper(_city_detail_lock);
	abase::hash_map<int,city_structure_attr>::iterator it = _city_detail.csam.find(key);
	if(it == _city_detail.csam.end())
	{
		//没有相关的建设信息
		return;
	}
	int attack_level = it->second.am[ATTACK].level;
	int defence_level = it->second.am[DEFENCE].level;
	int range_level = it->second.am[RANGE].level;
	int ai_level = it->second.am[AI_POLICY].level;
	int maxhp = it->second.am[MAX_HP].level;
	pImp->_base_prop.damage_low = bfct->attack_level[attack_level].attack_value;
	pImp->_base_prop.damage_high = bfct->attack_level[attack_level].attack_value;
	pImp->_en_point.spec_damage = bfct->attack_level[attack_level].attack_extra;
	pImp->_base_prop.defense = bfct->defence_level[defence_level].defence_value;
	pImp->_en_point.dmg_reduce = bfct->defence_level[defence_level].defence_extra;
	pImp->_base_prop.attack_range = bfct->range_values[range_level] + pTemplate->body_size;
	aip.trigger_policy = bfct->strategy_id[ai_level];
	pImp->_base_prop.max_hp = maxhp;
	pImp->_basic.hp = maxhp;
}

void bf_world_manager_base::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("注册回城点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//现在还无法区分攻方和攻方辅助方
		_competitor[BI_ATTACKER].town_list.push_back(pos);
		_competitor[BI_ATTACKER_ASSISTANT].town_list.push_back(pos);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_competitor[BI_DEFENDER].town_list.push_back(pos);
	}
	else
	{
		ASSERT(false && "注册回城点 no faction");
	}
}

void bf_world_manager_base::RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id)
{
	__PRINTF("注册复活点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	if(cond_id < CTRL_ID_BATTLE_BASE)
	{
		__PRINTF("RecordRevivePos invalid: %d\n",cond_id);
	}
	competitor_data::revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//允许同一控制器控制多个
		_competitor[BI_ATTACKER].revive_map[cond_id].push_back(temp);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_competitor[BI_DEFENDER].revive_map[cond_id].push_back(temp);
	}
	else
	{
		__PRINTF("无阵营复活点!\n");
	}
}

void bf_world_manager_base::RecordEntryPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("注册进入点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//现在还无法区分攻方和攻方辅助方
		_competitor[BI_ATTACKER].entry_list.push_back(pos);
		_competitor[BI_ATTACKER_ASSISTANT].entry_list.push_back(pos);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_competitor[BI_DEFENDER].entry_list.push_back(pos);
	}
	else
	{
		ASSERT(false && "注册进入点 no faction");
	}
}

int bf_world_manager_base::Init(const char* gmconf_file,const char* servername, int tag, int index)
{
	_max_mob_sight_range = 70.0f;
	__PRINTF("bf_world_manager_base: %s 初始化\n", servername);
	int rst = global_world_manager::Init(gmconf_file,servername, tag, index);
	if(0 != rst)
	{
		return rst;
	}
	delete _message_handler;
	_message_handler = new bf_world_message_handler(this,&_plane);
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	section += servername;
	_battle_id = atoi(conf->find(section,"battle_id").c_str());
	if(_battle_id <= 0)
	{
		__PRINTF("_battle_id= %d,不正确\n",_battle_id);
		return -10000;
	}
	__PRINTF("城战id为：%d\n",_battle_id);
	_kickout_time_min = atoi(conf->find(section,"battle_kickout_time_min").c_str());
	if(_kickout_time_min <= 0)
	{
		_kickout_time_min = DEFAULT_KICKOUT_TIME_MIN;
	}
	__PRINTF("踢出玩家时间min:%d\n",_kickout_time_min);
	_kickout_time_max = atoi(conf->find(section,"battle_kickout_time_max").c_str());
	if(_kickout_time_max <= 0)
	{
		_kickout_time_max = DEFAULT_KICKOUT_TIME_MAX;
	}
	__PRINTF("踢出玩家时间max:%d\n",_kickout_time_max);
	_kickout_close_time = atoi(conf->find(section,"battle_kickout_close_time").c_str());
	if(_kickout_close_time <= 0)
	{
		_kickout_close_time = DEFAULT_KICKOUT_CLOSE_TIME;
	}
	__PRINTF("关闭延迟时间max:%d\n",_kickout_close_time);
	
	int max_player_limit = atoi(conf->find(section,"battle_max_player_count").c_str());
	if(max_player_limit<= 0)
	{
		__PRINTF("battle_max_player_count= %d 不正确\n",max_player_limit);
		return -10000;
	}
	__PRINTF("人数上限: %d\n",max_player_limit);
	for(int i = 0;i < BI_MAX;++i)
	{
		_competitor[i].max_player_limit = max_player_limit;
	}

	// 这里从配置文件或者地图数据中获得合法准备位置；
	std::string temp;
	temp = conf->find( section, "battle_prepare_attacker" );
	if( temp == "" )
	{       
		__PRINTF( "没有攻方准备区域，使用默认值\n" );
		temp = "(-512,-512,-512,512,512,512)";
	}
	sscanf( temp.c_str(), "(%f,%f,%f)-(%f,%f,%f)",
			&_competitor[BI_ATTACKER].prepare_valid_area_min.x, &_competitor[BI_ATTACKER].prepare_valid_area_min.y, &_competitor[BI_ATTACKER].prepare_valid_area_min.z,
			&_competitor[BI_ATTACKER].prepare_valid_area_max.x, &_competitor[BI_ATTACKER].prepare_valid_area_max.y, &_competitor[BI_ATTACKER].prepare_valid_area_max.z );

	temp = conf->find( section, "battle_prepare_defender" );
	if( temp == "" )
	{
		__PRINTF( "没有守方准备区域，使用默认值\n" );
		temp = "(-512,-512,-512,512,512,512)";
	}       
	sscanf( temp.c_str(), "(%f,%f,%f)-(%f,%f,%f)",
			&_competitor[BI_DEFENDER].prepare_valid_area_min.x, &_competitor[BI_DEFENDER].prepare_valid_area_min.y, &_competitor[BI_DEFENDER].prepare_valid_area_min.z,
			&_competitor[BI_DEFENDER].prepare_valid_area_max.x, &_competitor[BI_DEFENDER].prepare_valid_area_max.y, &_competitor[BI_DEFENDER].prepare_valid_area_max.z );

	//全局的非法区域
	temp = conf->find( section, "battle_invalid_first_area" );
	if( temp == "" )
	{       
		__PRINTF( "没有全局非法区域1，使用默认值\n" );
		temp = "(-512,-512,-512,512,512,512)";
	}
	sscanf( temp.c_str(), "(%f,%f,%f)-(%f,%f,%f)",
			&invalid_first_area_min.x,&invalid_first_area_min.y,&invalid_first_area_min.z,
			&invalid_first_area_max.x,&invalid_first_area_max.y,&invalid_first_area_max.z);
	temp = conf->find( section, "battle_invalid_second_area" );
	if( temp == "" )
	{       
		__PRINTF( "没有全局非法区域2，使用默认值\n" );
		temp = "(-512,-512,-512,512,512,512)";
	}
	sscanf( temp.c_str(), "(%f,%f,%f)-(%f,%f,%f)",
			&invalid_second_area_min.x,&invalid_second_area_min.y,&invalid_second_area_min.z,
			&invalid_second_area_max.x,&invalid_second_area_max.y,&invalid_second_area_max.z);
	Reset();
	return 0;
}

void bf_world_manager_base::OnDeliveryConnected(std::vector<battle_field_info>& info)
{
	GNET::SendBattleFieldRegister(gmatrix::GetServerIndex(),_battle_id,GetWorldTag(),(int)IsUseForBattle());
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 发送注册信息",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 发送注册信息\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
}

int bf_world_manager_base::CreateBattleField(const bf_param& param,bool force)
{
	spin_autolock keeper(_battle_lock);
	//首先判断状态是否能创建
	if(BS_READY != _status && !force)
	{
		return CBFR_INVALID_STATUS;
	}
	if(_battle_id != param.battle_id)
	{
		return CBFR_INVALID_BATTLE_ID;
	}
	_competitor[BI_ATTACKER].mafia_id = param.attacker_mafia_id;
	_competitor[BI_DEFENDER].mafia_id = param.defender_mafia_id;
	_competitor[BI_ATTACKER_ASSISTANT].mafia_id = param.attacker_assistant_mafia_id;
	_end_timestamp = param.end_timestamp;
	_battle_result = BR_NONE;
	if(0 == _competitor[BI_DEFENDER].mafia_id)
	{
		_active_ctrl_id_prepare = 70401;
	}
	else
	{
		_active_ctrl_id_prepare = 70400;
	}
	_get_counter = DEFAULT_RETRY_COUNT;
	GetCityDetail();
	return 0;
}

void bf_world_manager_base::Reset()
{
	int i;
	for(i = 0;i < BI_MAX;++i)
	{
		_competitor[i].player_info_map.clear();
		_competitor[i].mafia_id = 0;
		_competitor[i].cur_player_count = 0;
	}
	_cur_timestamp = 0;
	_end_timestamp = 0;
	_heartbeat_counter = 0;
	_battle_info_seq = 0;
	_status = BS_READY;
	_battle_result = BR_NONE;
	_get_counter = DEFAULT_RETRY_COUNT;
	_put_counter = DEFAULT_RETRY_COUNT;
}

bool bf_world_manager_base::IsBattleInfoRefresh(int& info_seq)
{       
	if(_battle_info_seq != info_seq)
	{
		info_seq = _battle_info_seq;
		return true;
	}       
	return false;
}   

void bf_world_manager_base::RecordFlag(const A3DVECTOR& pos,int faction,int tid,bool is_key)
{
	ASSERT(false);
}

void bf_world_manager_base::RecordMine(const A3DVECTOR& pos,int faction,int tid,bool is_key)
{
	ASSERT(false);
}

void bf_world_manager_base::BattleFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int char_emote, int self_id)
{
/*
	__PRINTF("BattleFactionSay\n");
	spin_autolock keeper(_battle_lock);
	if(faction & (FACTION_2_SIDE_BATTLE_OFFENSE | FACTION_2_SIDE_BATTLE_OFFENSE_FRIEND))
	{
		multi_send_chat_msg(_attacker_list,buf,size,GMSV::CHAT_CHANNEL_LOCAL,0,0,0,0);
	}       
	else if(faction & (FACTION_2_SIDE_BATTLE_DEFENCE | FACTION_2_SIDE_BATTLE_DEFENCE_FRIEND))
	{
		multi_send_chat_msg(_defender_list,buf,size,GMSV::CHAT_CHANNEL_LOCAL,0,0,0,0);
	}
	*/
}

void bf_world_manager_base::BattleSay(const void* buf,size_t size)
{
	__PRINTF("BattleSay\n");
	spin_autolock keeper(_battle_lock);
	multi_send_chat_msg(_all_list,buf,size,GMSV::CHAT_CHANNEL_LOCAL,0,0,0,0);
}

void bf_world_manager_base::BattleChat(int faction,const void* buf, size_t size,int channel,int self_id)
{
/*j
	__PRINTF("BattleChat\n");
	spin_autolock keeper(_battle_lock);
	if(faction & (FACTION_2_SIDE_BATTLE_OFFENSE | FACTION_2_SIDE_BATTLE_OFFENSE_FRIEND))
	{
		multi_send_chat_msg(_attacker_list,buf,size,channel,0,0,0,self_id);
	}       
	else if(faction & (FACTION_2_SIDE_BATTLE_DEFENCE | FACTION_2_SIDE_BATTLE_DEFENCE_FRIEND))
	{
		multi_send_chat_msg(_defender_list,buf,size,channel,0,0,0,self_id);
	}
	*/
}

static void BuildProtocolNode(S2C::CMD::battlefield_construction_info::node_t & tmpNode , int key, city_structure_attr & value)
{
	memset(&tmpNode, 0, sizeof(tmpNode));
	tmpNode.key 			= key;
	tmpNode.type 			= value.type;
	tmpNode.attr.attack_level 	= value.am[ATTACK].level;
	tmpNode.attr.defence_level 	= value.am[DEFENCE].level;
	tmpNode.attr.range_level 	= value.am[RANGE].level;
	tmpNode.attr.ai_level		= value.am[AI_POLICY].level;
	tmpNode.attr.maxhp_level 	= value.am[MAX_HP].level;
}

bool bf_world_manager_base::SendBattleFieldConstructionInfo(bf_player_imp* pImp)
{
	ASSERT(_city_detail_lock);
	//获取建设相关数据
	//确定协议大小
	typedef S2C::CMD::battlefield_construction_info::node_t NODE_T;
	abase::vector<NODE_T, abase::fast_alloc<> > buffer;
	buffer.reserve(_city_detail.csam.size());
	//组织协议
	int res_a = _city_detail.ccrm[CONSTRUCTION_RES_KEY_A].num;
	int res_b = _city_detail.ccrm[CONSTRUCTION_RES_KEY_B].num;
	for(abase::hash_map<int,city_structure_attr>::iterator it = _city_detail.csam.begin();it != _city_detail.csam.end();++it)
	{
		NODE_T tmpNode;
		BuildProtocolNode(tmpNode,it->first,it->second);
		buffer.push_back(tmpNode);
	}
	pImp->_runner->battlefield_construction_info(res_a,res_b,(char*)&*buffer.begin(),sizeof(NODE_T) * buffer.size());
	return true;
}

bool bf_world_manager_base::SendBattleFieldContributionInfo(bf_player_imp* pImp,int page)
{
	ASSERT(_city_detail_lock);
	if(page < 0) return false;
	//获取玩家贡献数据
	//确定协议大小
	size_t count = _city_detail.contribution.size();
	if(count == 0)
	{
		pImp->_runner->battlefield_contribution_info(page,0,NULL,0);
		return true;
	}
	int max_page = (count - 1) / PLAYER_CONTRIBUTION_ONE_PAGE_COUNT;
	if(page > max_page) page = max_page;
	size_t start_index = page * PLAYER_CONTRIBUTION_ONE_PAGE_COUNT;
	size_t end_index = page * PLAYER_CONTRIBUTION_ONE_PAGE_COUNT + PLAYER_CONTRIBUTION_ONE_PAGE_COUNT - 1;
	if(count - 1 < end_index) end_index = count - 1;
	count = end_index - start_index + 1;

	typedef S2C::CMD::battlefield_contribution_info::node_t NODE_T;
	abase::vector<NODE_T,abase::fast_alloc<> > buffer;
	buffer.reserve(count);
	//组织协议
	for(size_t i = start_index;i <= end_index;++i)
	{
		NODE_T tmpNode;
		memset(&tmpNode,0,sizeof(tmpNode));
		tmpNode.player_id = _city_detail.contribution.player_contri_vec[i].GetPlayerID();
		tmpNode.res_a = _city_detail.contribution.player_contri_vec[i].GetContributionNum(CONSTRUCTION_RES_KEY_A);
		tmpNode.res_b = _city_detail.contribution.player_contri_vec[i].GetContributionNum(CONSTRUCTION_RES_KEY_B);
		buffer.push_back(tmpNode);
	}
	pImp->_runner->battlefield_contribution_info(page,max_page,(char*)&*buffer.begin(),sizeof(NODE_T) * buffer.size());
	return true;
}

void bf_world_manager_base::SendBattlefieldInfo(bf_player_imp* pImp)
{
	spin_autolock keeper(_battle_lock);
	size_t count = _key_map.size();
	std::vector<S2C::CMD::battlefield_info::structure_info_t> temp_vector;
	for(StructureInfoKeyMap::iterator it = _key_map.begin();it != _key_map.end();++it)
	{
		S2C::CMD::battlefield_info::structure_info_t temp_info;
		temp_info.key = it->first;
		temp_info.status = it->second.status;
		temp_info.pos = it->second.pos;
		temp_info.hp_factor = it->second.hp_factor;
		temp_vector.push_back(temp_info);
	}
	pImp->_runner->battlefield_info(count,&temp_vector[0]);
}

bool bf_world_manager_base::QueryBattlefieldConstructInfo(bf_player_imp* pImp,int type,int page)
{
	spin_autolock keeper(_battle_lock);
	spin_autolock keeper_detail(_city_detail_lock);
	if(0 == type)
	{
		SendBattleFieldConstructionInfo(pImp);
	}
	else
	{
		SendBattleFieldContributionInfo(pImp,page);
	}
	return true;
}

bool bf_world_manager_base::BattlefieldContribute(bf_player_imp* pImp,int res_type)
{
	spin_autolock keeper(_battle_lock);
	//捐献
	int item_id;
	int key;
	if(g_config.id_war_material1 == res_type)
	{
		if(!pImp->CheckItemExist(g_config.id_war_material1,1))
		{
			pImp->_runner->error_message(S2C::ERR_NOT_ENOUGH_RES_TO_CONTRIBUTE);
			return true;
		}
		item_id = g_config.id_war_material1;
		key = CONSTRUCTION_RES_KEY_A;
	}
	else if(g_config.id_war_material2 == res_type)
	{
		if(!pImp->CheckItemExist(g_config.id_war_material2,1))
		{
			pImp->_runner->error_message(S2C::ERR_NOT_ENOUGH_RES_TO_CONTRIBUTE);
			return true;
		}
		item_id = g_config.id_war_material2;
		key = CONSTRUCTION_RES_KEY_B;
	}
	else
	{
		return false;
	}
	spin_autolock keeper_detail(_city_detail_lock);
	_city_detail.contribution.AddContribution(pImp->_parent->ID.id,key,CONTRIBUTION_NUM_PER_MATERIAL);
	int rst = 0;
	rst = pImp->_inventory.Find(rst,item_id);
	ASSERT(rst >= 0);
	pImp->UseItemLog(pImp->_inventory[rst],1);
	pImp->_inventory.DecAmount(rst,1);
	pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,item_id,1,S2C::DROP_TYPE_USE);
	_city_detail.ccrm[key].num += CONTRIBUTION_NUM_PER_MATERIAL;
	//通知信息
	battlefiled_msg_t msg;
	msg.type = 0;
	msg.player_id = pImp->_parent->ID.id;
	size_t len;
	const void* pname = pImp->GetPlayerName(len);
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memset(msg.name,0,MAX_USERNAME_LENGTH);
	memcpy(msg.name,pname,len);
	msg.data1 = res_type;
	msg.data2 = 1;
	GNET::SendBattleFieldBroadcast(pImp->OI_GetMafiaID(),BATTLEFIELD_BROADCAST_ID,&msg,sizeof(battlefiled_msg_t));
	SendBattleFieldConstructionInfo(pImp);
	//记录日志
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d),玩家 %d 执行了城战捐献操作 捐献了1个 %d",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),pImp->_parent->ID.id,res_type);
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d),玩家 %d 执行了城战捐献操作 捐献了1个 %d\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),pImp->_parent->ID.id,res_type);
	return true;
}

bool bf_world_manager_base::BattlefieldConstruct(bf_player_imp* pImp,int key,int type,int cur_level)
{
	spin_autolock keeper(_battle_lock);
	//判断玩家是否为帮主
	if(0 == pImp->GetParent()->id_mafia) return false; //没有帮
	if(pImp->GetParent()->id_mafia != _competitor[BI_DEFENDER].mafia_id) return false; //不是自己城,应该不会出现,还是判下吧
	if(2 != pImp->GetParent()->rank_mafia && 3 != pImp->GetParent()->rank_mafia) return false; //不是帮主或副帮主
	if(key < CITY_STRUCTURE_KEY_MIN || key > CITY_STRUCTURE_KEY_MAX) return false;
	if(_key_map.find(key) == _key_map.end()) return false;
	if(type < ATTACK || type > MAX_HP) return false;
	spin_autolock keeper_detail(_city_detail_lock);
	int real_level = _city_detail.csam[key].am[type].level;
	if(cur_level != real_level)
	{
		//级别变化了,数据错误或者升级时别人也再升级
		return false;
	}
	int tid = _key_map[key].tid;
	const battlefield_consturct_temp* bfct = Get(tid);
	if(!bfct) return false; //没有这个key值,怎么可能啊......
	int need_material_type = 0;
	int need_material_count = 0;
	int max_level = 0x7FFFFFFF;
	int lvl_added = 0;
	switch(type)
	{
		case ATTACK:
		{
			need_material_type = bfct->attack_war_material;
			max_level = bfct->attack_max_lev;
			need_material_count = (int)(bfct->attack_co[0] * cur_level * cur_level + bfct->attack_co[1] * cur_level + bfct->attack_co[2]);
			lvl_added = 1;
		}
		break;

		case DEFENCE:
		{
			need_material_type = bfct->defence_war_material;
			max_level = bfct->defence_max_lev;
			need_material_count = (int)(bfct->defence_co[0] * cur_level * cur_level + bfct->defence_co[1] * cur_level + bfct->defence_co[2]);
			lvl_added = 1;
		}
		break;

		case RANGE:
		{
			need_material_type = bfct->range_war_material;
			max_level = bfct->range_max_lev;
			need_material_count = (int)(bfct->range_co[0] * cur_level * cur_level + bfct->range_co[1] * cur_level + bfct->range_co[2]);
			lvl_added = 1;
		}
		break;

		case AI_POLICY:
		{
			need_material_type = bfct->strategy_war_material;
			max_level = bfct->strategy_max_lev;
			need_material_count = (int)(bfct->range_co[0] * cur_level * cur_level + bfct->range_co[1] * cur_level + bfct->range_co[2]);
			lvl_added = 1;
		}
		break;

		case MAX_HP:
		{
			if((int)_city_detail.csam[key].am[type].level >= CITY_STRUCTURE_MAX_HP)
			{
				pImp->_runner->error_message(S2C::ERR_CONSTRUCT_MAX_LEVEL);
				return true;
			}
			need_material_type = bfct->hp_war_material;
			need_material_count = bfct->hp_material_num;
			lvl_added = bfct->lvlup_hp;
			if((int)_city_detail.csam[key].am[type].level + bfct->lvlup_hp > CITY_STRUCTURE_MAX_HP)
			{
				lvl_added = CITY_STRUCTURE_MAX_HP - _city_detail.csam[key].am[type].level;
			}
		}
		break;

		default:
			ASSERT(false);
		break;
	}
	if(cur_level >= max_level)
	{
		pImp->_runner->error_message(S2C::ERR_CONSTRUCT_MAX_LEVEL);
		return true;
	}
	if(CONSTRUCTION_RES_KEY_A != need_material_type && CONSTRUCTION_RES_KEY_B != need_material_type)
	{
		return false;
	}
	//判断够不够物资
	if((size_t)need_material_count > _city_detail.ccrm[need_material_type].num)
	{
		pImp->_runner->error_message(S2C::ERR_NOT_ENOUGH_RES_TO_CONSTRUCT);
		return true;
	}
	//扣东西
	_city_detail.ccrm[need_material_type].num -= (size_t)need_material_count;
	//增加级别
	_city_detail.csam[key].am[type].level += lvl_added;
	//通知信息
	battlefiled_msg_t msg;
	msg.type = 1;
	msg.player_id = pImp->_parent->ID.id;
	size_t len;
	const void* pname = pImp->GetPlayerName(len);
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memset(msg.name,0,MAX_USERNAME_LENGTH);
	memcpy(msg.name,pname,len);
	msg.data1 = key;
	msg.data2 = cur_level + lvl_added;
	msg.data3 = type;
	GNET::SendBattleFieldBroadcast(pImp->OI_GetMafiaID(),BATTLEFIELD_BROADCAST_ID,&msg,sizeof(battlefiled_msg_t));
	SendBattleFieldConstructionInfo(pImp);
	//记录日志
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 玩家 %d 执行了城战建设操作"
		"(key: %d,type: %d,level: %d) 消耗了(res: %d,num: %d)",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),pImp->_parent->ID.id,
		key,type,cur_level,need_material_type,need_material_count);
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 玩家 %d 执行了城战建设操作"
		"(key: %d,type: %d,level: %d) 消耗了(res: %d,num: %d)\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),pImp->_parent->ID.id,
		key,type,cur_level,need_material_type,need_material_count);
	return true;
}

void bf_world_manager_base::OnActiveSpawn(int id,bool active)
{
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 调用控制器(id: %d,active: %d)\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),id,active);
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 调用控制器(id: %d,active: %d)",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),id,active);
	if(_competitor[BI_ATTACKER].revive_map.find(id) != _competitor[BI_ATTACKER].revive_map.end())
	{
		size_t size = _competitor[BI_ATTACKER].revive_map[id].size();
		for(size_t i = 0;i < size;++i)
		{
			_competitor[BI_ATTACKER].revive_map[id][i].active = active;
		}
	}
	if(_competitor[BI_DEFENDER].revive_map.find(id) != _competitor[BI_DEFENDER].revive_map.end())
	{
		size_t size = _competitor[BI_DEFENDER].revive_map[id].size();
		for(size_t i = 0;i < size;++i)
		{
			_competitor[BI_DEFENDER].revive_map[id][i].active = active;
		}
	}
	_service_ctrl_id_map[id] = active;
	global_world_manager::OnActiveSpawn(id,active);
}

bool bf_world_manager_base::CheckServiceCtrlID(int ctrl_id)
{
	int id2 = TranslateCtrlID(ctrl_id);
	std::map<int,bool>::iterator it;
	it = _service_ctrl_id_map.find(id2);
	if(_service_ctrl_id_map.end() == it)
	{
		return false;
	}
	return _service_ctrl_id_map[id2];
}

void bf_world_manager_base::Heartbeat()
{
	global_world_manager::Heartbeat();
	spin_autolock keeper(_battle_lock);
	++_heartbeat_counter; 
	if(0 == (_heartbeat_counter) % 20)
	{
		switch(_status)
		{
			case BS_READY:
			{
				//__PRINTF("BS_READY\n");
			}
			break;
			
			case BS_PREPARING:
			{
				__PRINTF("BS_PREPARING( %d )\n",(int)(_cur_timestamp - g_timer.get_systime()));
				if(_cur_timestamp <= g_timer.get_systime())
				{
					OnRun();
					ChangeBattleStatus(BS_RUNNING);
				}
			}
			break;
			
			case BS_RUNNING:
			{
				__PRINTF("BS_RUNNING( %d )\n",(int)(_cur_timestamp - g_timer.get_systime()));
				//战斗逻辑,如果时间到或者产生结果了
				if(_cur_timestamp <= g_timer.get_systime() || BR_NONE != _battle_result)
				{
					if(BR_NONE == _battle_result) _battle_result = BR_DEFENDER_WIN;//时间到防守方胜利
					OnClose();
					_put_counter = DEFAULT_RETRY_COUNT;
					PutCityDetail();
					ChangeBattleStatus(BS_CLOSING);
					//战斗结束时更新一次信息
					SendBattleInfo();
					SendBattleEnd(_battle_result);
					GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)结束"
						"发布结果开始准备清理(result: %d,end_timestamp= %d,min= %d,max= %d,close= %d)", 
						gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),
						_battle_result,(int)g_timer.get_systime(),_kickout_time_min,_kickout_time_max,_kickout_close_time);
					__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)结束"
						"发布结果开始准备清理(result: %d,end_timestamp= %d,min= %d,max= %d,close= %d)\n", 
						gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),
						_battle_result,(int)g_timer.get_systime(),_kickout_time_min,_kickout_time_max,_kickout_close_time);
					GNET::SendBattleFieldEnd(gmatrix::GetServerIndex(),_battle_id,GetWorldTag(),_battle_result);
				}
			}
			break;

			case BS_CLOSING:
			{
				__PRINTF("BS_CLOSING( %d )\n",(int)(_cur_timestamp - g_timer.get_systime()));
				//这个期间踢出玩家
				//在玩家heartbeat逻辑里完成规定时间内的踢人
				if(_cur_timestamp <= g_timer.get_systime())
				{
					KillAllMonsters();
					//在这之前需要确保非gm玩家已经被踢出
					OnReady();
					__PRINTF( "城战: %d 恢复到初始状态\n",GetWorldTag());
					Reset();
					ChangeBattleStatus(BS_READY);
				}
			}
			break;

			default:
			{
				__PRINTF("ERROR! _status=%d\n",_status);
				ASSERT(false);
			}
		};
	}
	//每30秒更新一次信息
	if(0 == (_heartbeat_counter) % (30 * 20))
	{
		SendBattleInfo();
	}
}

void bf_world_manager_base::PostPlayerLogin(gplayer* pPlayer)
{
	__PRINTF("PostPlayerLogin player_id= %d,mafia_id= %d\n",pPlayer->ID.id,pPlayer->id_mafia);
	if(pPlayer->id_mafia == _competitor[BI_ATTACKER].mafia_id)
	{
		pPlayer->SetObjectState(gactive_object::STATE_BATTLE_OFFENSE);
		GetRandomEntryPos(pPlayer->pos,BF_ATTACKER);
	}
	else if(pPlayer->id_mafia == _competitor[BI_DEFENDER].mafia_id)
	{
		pPlayer->SetObjectState(gactive_object::STATE_BATTLE_DEFENCE);
		GetRandomEntryPos(pPlayer->pos,BF_DEFENDER);
	}
	else if(pPlayer->id_mafia == _competitor[BI_ATTACKER_ASSISTANT].mafia_id)
	{
		pPlayer->SetObjectState(gactive_object::STATE_BATTLE_OFFENSE);
		GetRandomEntryPos(pPlayer->pos,BF_ATTACKER_ASSISTANT);
	}
	else
	{
		GetRandomEntryPos(pPlayer->pos,BF_NONE);
	}
}

int bf_world_manager_base::OnPlayerLogin(const GDB::base_info* pInfo,const GDB::vecdata * data,bool is_gm)
{
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 玩家 %d 请求登录(mafia_id: %d)",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),pInfo->id,pInfo->factionid);
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 玩家 %d 请求登录(mafia_id: %d)\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),pInfo->id,pInfo->factionid);
	if(BS_PREPARING != _status && BS_RUNNING != _status && !is_gm)
	{
		__PRINTF("城战(battle_id: %d,world_tag: %d)为不可进入状态状态并且进入玩家不是gm,_status=%d\n",
			_battle_id,GetWorldTag(),_status);
		return -1001;
	}
	if((int)pInfo->factionid == _competitor[BI_ATTACKER].mafia_id)
	{
		if(!AddPlayer(BI_ATTACKER))
		{
			__PRINTF("攻击方人数上限\n");
			return -1002;
		}
	}
	else if((int)pInfo->factionid == _competitor[BI_DEFENDER].mafia_id)
	{
		if(!AddPlayer(BI_DEFENDER))
		{
			__PRINTF("防御方人数上限\n");
			return -1003;
		}
	}
	else if((int)pInfo->factionid == _competitor[BI_ATTACKER_ASSISTANT].mafia_id)
	{
		if(!AddPlayer(BI_ATTACKER_ASSISTANT))
		{
			__PRINTF("攻击辅助方人数上限\n");
			return -1004;
		}
	}
	else if(GetWorldLimit().gmfree && is_gm)
	{
	}
	else
	{
		__PRINTF( "玩家帮派不符又不是gm不可以进入,mafia_id=%d\n", pInfo->factionid );
		return -1005;
	}
	return 0;
}

void bf_world_manager_base::PlayerEnter(gplayer* pPlayer,int battle_faction)
{
	__PRINTF("int bf_world_manager_base::PlayerEnter,pPlayer= %p,battle_faction= %d\n",pPlayer,battle_faction);
	spin_autolock keeper(_battle_lock);
	AddMapNode(_all_list,pPlayer);
	player_battle_info tempinfo;
	memset(&tempinfo,0,sizeof(player_battle_info));
	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	tempinfo.level = pImp->_basic.level;
	tempinfo.battle_faction = battle_faction;	
	if(BF_ATTACKER == battle_faction)
	{
		_competitor[BI_ATTACKER].player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode(_competitor[BI_ATTACKER].player_list,pPlayer);
	}
	else if(BF_DEFENDER == battle_faction)
	{
		_competitor[BI_DEFENDER].player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode(_competitor[BI_DEFENDER].player_list,pPlayer);
	}
	else if(BF_ATTACKER_ASSISTANT)
	{
		_competitor[BI_ATTACKER_ASSISTANT].player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode(_competitor[BI_ATTACKER_ASSISTANT].player_list,pPlayer);
	}
	SendBattleInfo();
}

void bf_world_manager_base::PlayerLeave(gplayer* pPlayer,int battle_faction)
{
	__PRINTF("int bf_world_manager_base::PlayerLeave,pPlayer= %p,battle_faction= %d\n",pPlayer,battle_faction);
	spin_autolock keeper(_battle_lock);
	DelMapNode(_all_list,pPlayer);
	PlayerInfoIt it;
	if(BF_ATTACKER == battle_faction)
	{
		DelMapNode(_competitor[BI_ATTACKER].player_list,pPlayer);
		it = _competitor[BI_ATTACKER].player_info_map.find(pPlayer->ID.id);
		if(it != _competitor[BI_ATTACKER].player_info_map.end())
		{
			_competitor[BI_ATTACKER].player_info_map.erase(it);
		}
		DelPlayer(BI_ATTACKER);
	}
	else if(BF_DEFENDER == battle_faction)
	{
		DelMapNode(_competitor[BI_DEFENDER].player_list,pPlayer);
		it = _competitor[BI_DEFENDER].player_info_map.find(pPlayer->ID.id);
		if(it != _competitor[BI_DEFENDER].player_info_map.end())
		{
			_competitor[BI_DEFENDER].player_info_map.erase(it);
		}
		DelPlayer(BI_DEFENDER);
	}
	else if(BF_ATTACKER_ASSISTANT == battle_faction)
	{
		DelMapNode(_competitor[BI_ATTACKER_ASSISTANT].player_list,pPlayer);
		it = _competitor[BI_ATTACKER_ASSISTANT].player_info_map.find(pPlayer->ID.id);
		if(it != _competitor[BI_ATTACKER_ASSISTANT].player_info_map.end())
		{
			_competitor[BI_ATTACKER_ASSISTANT].player_info_map.erase(it);
		}
		DelPlayer(BI_ATTACKER_ASSISTANT);
	}
	SendBattleInfo();
}

bool bf_world_manager_base::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
/*
	world_tag = GetWorldTag();
	if(pImp->OI_GetMafiaID() == _competitor[BI_ATTACKER].mafia_id)
	{
		if(0 == _competitor[BI_ATTACKER].town_list.size()) return false;
		pos = _competitor[BI_ATTACKER].town_list[abase::Rand(0,_competitor[BI_ATTACKER].town_list.size() - 1)];
	}
	else if(pImp->OI_GetMafiaID() == _competitor[BI_DEFENDER].mafia_id)
	{
		if(0 == _competitor[BI_DEFENDER].town_list.size()) return false;
		pos = _competitor[BI_DEFENDER].town_list[abase::Rand(0,_competitor[BI_DEFENDER].town_list.size() - 1)];
	}
	else if(pImp->OI_GetMafiaID() == _competitor[BI_ATTACKER_ASSISTANT].mafia_id)
	{
		if(0 == _competitor[BI_ATTACKER_ASSISTANT].town_list.size()) return false;
		pos = _competitor[BI_ATTACKER_ASSISTANT].town_list[abase::Rand(0,_competitor[BI_ATTACKER_ASSISTANT].town_list.size() - 1)];
	}
	else
	{
		return false;
	}
	return true;
	*/
	//获得最近的复活点
	int battle_faction = pImp->GetBattleFaction();
	spin_autolock keeper(_battle_lock);
	abase::vector<A3DVECTOR> revive_pos_list;
	competitor_data::CtrlReviveMapIt it;
	if(BF_ATTACKER == battle_faction || BF_ATTACKER_ASSISTANT == battle_faction)
	{
		for(it = _competitor[BI_ATTACKER].revive_map.begin();it != _competitor[BI_ATTACKER].revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{
				if(it->second[i].active)
				{
					revive_pos_list.push_back(it->second[i].pos);
				}
			}
		}
	}
	else if(BF_DEFENDER == battle_faction)
	{
		for(it = _competitor[BI_DEFENDER].revive_map.begin();it != _competitor[BI_DEFENDER].revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{
				if(it->second[i].active)
				{
					revive_pos_list.push_back(it->second[i].pos);
				}
			}
		}
	}
	else
	{
		return false;
	}
	size_t size = revive_pos_list.size();
	if(0 == size) return false;
	float min_dis = 1000000.f;
	A3DVECTOR temp_pos;
	for(size_t i = 0;i < size;++i)
	{
		float temp_dis = revive_pos_list[i].horizontal_distance(pImp->GetParent()->pos);
		if(temp_dis < min_dis)
		{
			pos = revive_pos_list[i];
			min_dis = temp_dis;
		}
	}
	world_tag = GetWorldTag();
	return true;
}

bool bf_world_manager_base::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag)
{
	return false;
}

void bf_world_manager_base::GetRandomEntryPos(A3DVECTOR& pos,int battle_faction)
{
	spin_autolock keeper(_battle_lock);
	if(BF_ATTACKER == battle_faction)
	{
		if(0 == _competitor[BI_ATTACKER].entry_list.size()) return;
		pos = _competitor[BI_ATTACKER].entry_list[abase::Rand(0,_competitor[BI_ATTACKER].entry_list.size() - 1)];
	}
	else if(BF_ATTACKER_ASSISTANT == battle_faction)
	{
		if(0 == _competitor[BI_ATTACKER_ASSISTANT].entry_list.size()) return;
		pos = _competitor[BI_ATTACKER_ASSISTANT].entry_list[abase::Rand(0,_competitor[BI_ATTACKER_ASSISTANT].entry_list.size() - 1)];
	}
	else
	{
		if(0 == _competitor[BI_DEFENDER].entry_list.size()) return;
		pos = _competitor[BI_DEFENDER].entry_list[abase::Rand(0,_competitor[BI_DEFENDER].entry_list.size() - 1)];
		//gm也用这个吧
	}
}

void bf_world_manager_base::ChangeBattleStatus(int status)
{
	ASSERT(_battle_lock);
	_status = status;
}

void bf_world_manager_base::OnPrepare()
{
	ASSERT(_battle_lock);
	_cur_timestamp = g_timer.get_systime() + DEFAULT_PREPARE_TIME;
	//打开城战准备的控制器
	if(_active_ctrl_id_prepare)ActiveSpawn(this,_active_ctrl_id_prepare,true);
	if(_active_ctrl_id_prepare2)ActiveSpawn(this,_active_ctrl_id_prepare2,true);
}

void bf_world_manager_base::OnRun()
{
	ASSERT(_battle_lock);
	_cur_timestamp = _end_timestamp;
	//打开城战开始控制器
	if(_active_ctrl_id_start) ActiveSpawn(this,_active_ctrl_id_start,true);
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<CMD::battlefield_start>::From(h1);
	if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
}

void bf_world_manager_base::OnClose()
{
	ASSERT(_battle_lock);
	if(BR_NONE == _battle_result) _battle_result = BR_DRAW;
	_cur_timestamp = g_timer.get_systime() + _kickout_close_time;
	//计算战斗结果
	//关闭相关控制器
	if(_active_ctrl_id_end) ActiveSpawn(this,_active_ctrl_id_end,true);

	if(_active_ctrl_id_prepare) ActiveSpawn(this,_active_ctrl_id_prepare,false);
	if(_active_ctrl_id_prepare2) ActiveSpawn(this,_active_ctrl_id_prepare2,false);
	if(_active_ctrl_id_start) ActiveSpawn(this,_active_ctrl_id_start,false);
}

void bf_world_manager_base::OnReady()
{
	ASSERT(_battle_lock);
}

void bf_world_manager_base::DumpWorldMsg()
{
	ASSERT(_battle_lock);
	__PRINTF("-------------------- world message dump --------------------\n" );
	__PRINTF("battle_id= %d,world_tag= %d,status= %d,result= %d\n",_battle_id,GetWorldTag(),_status,_battle_result);
	__PRINTF("总人数: %d,攻击方人数: %d,防守方人数: %d\n",
		_competitor[BI_ATTACKER].player_info_map.size() + _competitor[BI_DEFENDER].player_info_map.size(),
		_competitor[BI_ATTACKER].player_info_map.size(),_competitor[BI_DEFENDER].player_info_map.size());
	__PRINTF("玩家信息:\n");
	PlayerInfoIt it;
	for(it = _competitor[BI_ATTACKER].player_info_map.begin();it != _competitor[BI_ATTACKER].player_info_map.end();++it)
	{
		__PRINTF("\tid= %d,battle_faction= %d,kill= %d,death= %d\n",
			it->first,it->second.battle_faction,it->second.kill,it->second.death);
	}
	for(it = _competitor[BI_DEFENDER].player_info_map.begin();it != _competitor[BI_DEFENDER].player_info_map.end();++it)
	{
		__PRINTF("\tid= %d,battle_faction= %d,kill= %d,death= %d\n",
			it->first,it->second.battle_faction,it->second.kill,it->second.death);
	}
	__PRINTF("\n");
}

int bf_world_manager_base::TranslateCtrlID(int which)
{
	if(which >= CTRL_CONDISION_ID_BATTLE_MIN && which <= CTRL_CONDISION_ID_BATTLE_MAX)
	{
		__PRINTF("城战(battle_id: %d,world_tag: %d) 调用TranslateCtrlID(id: %d)\n",
			_battle_id,GetWorldTag(),which);
		return CTRL_ID_BATTLE_BASE +
			(GetWorldTag() - BATTLE_WORLD_TAG_BEGIN ) * CTRL_CONDISION_ID_BATTLE_OFFSET +
			(which - CTRL_CONDISION_ID_BATTLE_MIN);
	}
	return which;
}

void bf_world_manager_base::KillAllMonsters()
{
	//需要残留怪物,物品等检测 全杀,策划保证战斗停止后所有控制器不会再触发
	__PRINTF("城战(battle_id: %d,world_tag: %d) 杀死所有npc\n",_battle_id,GetWorldTag());
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 清理所有npc",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d) 清理所有npc\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
	MSG msg;
	BuildMessage(msg,GM_MSG_BATTLE_NPC_DISAPPEAR,XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0));
	std::vector<exclude_target> empty;
	_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF,empty);
}

namespace
{
	class GetCityDetailTask : public abase::ASmallObject, public GDB::City_Op_Result
	{
		bf_world_manager_base* _manager;
	public:
		GetCityDetailTask(bf_world_manager_base* manager):_manager(manager)
		{
		}
		~GetCityDetailTask()
		{
		}
		virtual void OnTimeOut()
		{
			if(0 == _manager->GetGetCounter())
			{
				_manager->OnGetCityDetail(bf_world_manager_base::CBFR_GET_DB_TIME_OUT,0,NULL,0);
				GLog::log(GLOG_ERR,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)读取数据超时",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)读取数据超时\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
			}
			else
			{
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)读取数据超时,重试(%d)!!!\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle(),_manager->GetGetCounter());
				_manager->GetCityDetail();
			}
		}
		virtual void OnGetCity(int battle_id,int stamp,int ret_code,const void* buf,size_t size)
		{
			if(battle_id != _manager->GetBattleID())
			{
				__PRINTF("怎么可能呢\n");
				return;
			}
			_manager->OnGetCityDetail(ret_code,stamp,buf,size);
		}
		virtual void OnPutCity(int battle_id,int ret_code)
		{
			ASSERT(false);
		}
	};

	class PutCityDetailTask : public abase::ASmallObject, public GDB::City_Op_Result
	{
		bf_world_manager_base* _manager;
	public:
		PutCityDetailTask(bf_world_manager_base* manager):_manager(manager)
		{
		}
		~PutCityDetailTask()
		{
		}
		virtual void OnTimeOut()
		{
			if(0 == _manager->GetPutCounter())
			{
				_manager->OnPutCityDetail(bf_world_manager_base::CBFR_GET_DB_TIME_OUT);
				GLog::log(GLOG_ERR,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)存储数据超时",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)存储数据超时\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
			}
			else
			{
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)存储数据超时,重试(%d)!!!\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle(),_manager->GetPutCounter());
				spin_autolock keeper(*_manager->GetBattleLock());
				_manager->PutCityDetail();
			}
		}
		virtual void OnGetCity(int battle_id,int stamp,const void* buf,size_t size)
		{
			ASSERT(false);
		}
		virtual void OnPutCity(int battle_id,int ret_code)
		{
			if(battle_id != _manager->GetBattleID())
			{
				__PRINTF("怎么可能呢\n");
				return;
			}
			_manager->OnPutCityDetail(ret_code);
		}
	};
}

void bf_world_manager_base::GetCityDetail()
{
	GDB::get_city_detail(_battle_id,new GetCityDetailTask(this));
	--_get_counter;
}

void bf_world_manager_base::PutCityDetail()
{
	spin_autolock keeper(_city_detail_lock);
	_city_detail.stamp += 1;
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)执行PutCityDetail (stamp: %d)\n",
		gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),_city_detail.stamp);
	raw_wrapper ar;
	for(int key = 1;key <=10;++key)
	{
		abase::hash_map<int,city_structure_attr>::iterator it = _city_detail.csam.find(key);
		if(it == _city_detail.csam.end())
		{
			//没有相关的建设信息
			_city_detail.csam[key].am[ATTACK].level = 0;
			_city_detail.csam[key].am[DEFENCE].level = 0;
			_city_detail.csam[key].am[RANGE].level = 0;
			_city_detail.csam[key].am[AI_POLICY].level = 0;
			_city_detail.csam[key].am[MAX_HP].level = CITY_STRUCTURE_DEATH_INIT_HP;
			int tid = _key_map[key].tid;
			const battlefield_consturct_temp* bfct = Get(tid);
			if(!bfct) continue;
			if(_key_tid_map.find(tid) == _key_tid_map.end())
			{
				//没有这个key值
				continue;
			}
			_city_detail.csam[key].am[MAX_HP].level = bfct->init_hp;
		}
		__PRINTF("%d - %d - %d - %d - %d\n",
			_city_detail.csam[key].am[ATTACK].level,
			_city_detail.csam[key].am[DEFENCE].level,
			_city_detail.csam[key].am[RANGE].level,
			_city_detail.csam[key].am[AI_POLICY].level,
			_city_detail.csam[key].am[MAX_HP].level);
	}
	Save(ar);
	GDB::put_city_detail(_battle_id,_city_detail.stamp,ar.data(),ar.size(),new PutCityDetailTask(this));
	--_put_counter;
}

void bf_world_manager_base::OnGetCityDetail(int ret_code,int stamp,const void* buf,size_t size)
{
	__PRINTF("OnGetCityDetail(ret_code= %d,stamp= %d,bufsize= %d)\n",ret_code,stamp,size);
	spin_autolock keeper(_battle_lock);
	if(0 == ret_code)
	{
		spin_autolock keeper(_city_detail_lock);
		raw_wrapper ar(buf,size);
		if(!Load(ar)) ret_code = CBFR_GET_DB_DATA_ERROR;
		//这里初始化数据
		for(int key = 1;key <=10;++key)
		{
			int tid = _key_map[key].tid;
			const battlefield_consturct_temp* bfct = Get(tid);
			if(!bfct) continue;
			if(_key_tid_map.find(tid) == _key_tid_map.end())
			{
				//没有这个key值
				continue;
			}
			abase::hash_map<int,city_structure_attr>::iterator it = _city_detail.csam.find(key);
			if(it == _city_detail.csam.end())
			{
				//没有相关的建设信息
				_city_detail.csam[key].am[ATTACK].level = 0;
				_city_detail.csam[key].am[DEFENCE].level = 0;
				_city_detail.csam[key].am[RANGE].level = 0;
				_city_detail.csam[key].am[AI_POLICY].level = 0;
				_city_detail.csam[key].am[MAX_HP].level = bfct->init_hp;
			}
			if(0 == _competitor[BI_DEFENDER].mafia_id)
			{
				//无主的城给初始值
				_city_detail.csam[key].am[MAX_HP].level = bfct->init_hp;
			}
			__PRINTF("%d - %d - %d - %d - %d\n",
				_city_detail.csam[key].am[ATTACK].level,
				_city_detail.csam[key].am[DEFENCE].level,
				_city_detail.csam[key].am[RANGE].level,
				_city_detail.csam[key].am[AI_POLICY].level,
				_city_detail.csam[key].am[MAX_HP].level);
		}
	}
	if(0 == ret_code)
	{
		OnPrepare();
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)开启成功",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)开启成功\n",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
		ChangeBattleStatus(BS_PREPARING);
	}
	else
	{
		//这里应该告诉del失败了,或者超时,或者数据有问题,不应该出现的
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)开启失败(ret_code: %d)",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),ret_code);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)开启失败(ret_code: %d)\n",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),ret_code);
	}
	GNET::SendBattleFieldStartRe(gmatrix::GetServerIndex(),_battle_id,GetWorldTag(),ret_code);
}

void bf_world_manager_base::OnPutCityDetail(int ret_code)
{
	if(0 == ret_code)
	{
		//成功不用log了
		//GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)保存数据成功",
		//	gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)保存数据成功\n",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle());
	}
	else
	{
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)保存数据失败(ret_code: %d)",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),ret_code);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle: %d)保存数据失败(ret_code: %d)\n",
			gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),ret_code);
	}
}

bool bf_world_manager_base::Save(raw_wrapper& ar) 
{
	ASSERT(_battle_lock);
	ASSERT(_city_detail_lock);
	ar << _city_detail.stamp;
	ar << _city_detail.ccrm.size();
	for(abase::hash_map<int,city_construction_res>::iterator it = _city_detail.ccrm.begin();it != _city_detail.ccrm.end();++it)
	{
		ar << it->first;
		ar << it->second.num;
	}
	ar << _city_detail.csam.size();
	for(abase::hash_map<int,city_structure_attr>::iterator it = _city_detail.csam.begin();it != _city_detail.csam.end();++it)
	{
		ar << it->first;
		ar << it->second.type;
		ar << it->second.am.size();
		for(abase::hash_map<int,city_structure_attr::attr_t>::iterator it2 = it->second.am.begin();
			it2 != it->second.am.end(); ++it2)
		{
			ar << it2->first;
			ar << it2->second.level;
		}
	}
	ar << _city_detail.contribution.size();
	size_t size = _city_detail.contribution.size();
	for(size_t i = 0;i < size;++i)
	{
		ar << _city_detail.contribution.player_contri_vec[i].player_id;
		ar << CONSTRUCTION_RES_COUNT;
		for(size_t j = 0;j < CONSTRUCTION_RES_COUNT;++j)
		{
			ar << j;
			ar << _city_detail.contribution.player_contri_vec[i].contribute[j];
		}
	}
	ar << _city_detail.service_npc_born_timestamp;
	return true;
}

bool bf_world_manager_base::Load(raw_wrapper& ar)
{
	ASSERT(_battle_lock);
	ASSERT(_city_detail_lock);
	if(0 == ar.size())
	{
		//以前数据库里没东西
		_city_detail.stamp = 1;
		return true;
	}
	size_t stamp = 0;
	ar >> stamp;
	if(stamp <= _city_detail.stamp)
	{
		//已经是新的了
		return true;
	}
	_city_detail.stamp = stamp;
	size_t size = 0;
	ar >> size;
	for(size_t i = 0;i < size;++i)
	{
		int key;
		ar >> key;
		ar >> _city_detail.ccrm[key].num;
	}
	ar >> size;
	for(size_t i = 0;i < size;++i)
	{
		int key;
		ar >> key;
		ar >> _city_detail.csam[key].type;
		size_t size2 = 0;
		ar >> size2;
		for(size_t j = 0;j < size2;++j)
		{
			int key2;
			ar >> key2;
			ar >> _city_detail.csam[key].am[key2].level;
		}
	}
	ar >> size;
	for(size_t i = 0;i < size;++i)
	{
		int player_id;
		ar >> player_id;
		size_t size2;
		ar >> size2;
		player_contribution::player_contribution_node temp_node(player_id);
		for(size_t j = 0;j < size2;++j)
		{
			size_t index;
			size_t sum;
			ar >> index;
			if(index != j)
			{
				__PRINTF("error !!!!!!!!!!!bool bf_world_manager_base::Load(raw_wrapper& ar)\n");
				return false;
			}
			ar >> sum;
			temp_node.SetContributionNum(index,sum);
		}
		_city_detail.contribution.player_contri_vec.push_back(temp_node);
		//从数据库里读出来的一定是排序好的
	}
	if(ar.offset() == ar.size())
	{
		_city_detail.service_npc_born_timestamp = 0;
	}
	else
	{
		ar >> _city_detail.service_npc_born_timestamp;
	}
	return true;
}

global_world_manager* bf_world_manager_base::new_bf_world_manager(const char* gmconf_file,const char* servername)
{
	//根据模版信息或配置信息,建立相应的城战类型
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	section += servername;
	int use_for_battle = atoi(conf->find(section,"use_for_battle").c_str());
	if(use_for_battle)
	{
		return new bf_world_manager_battle();
	}
	else
	{
		return new bf_world_manager_construction();
	}
}


bf_world_manager_battle::bf_world_manager_battle()
{
}

bf_world_manager_battle::~bf_world_manager_battle()
{
}

int bf_world_manager_battle::Init(const char* gmconf_file,const char* servername)
{
	int rst = bf_world_manager_base::Init(gmconf_file,servername, -1, -1);
	_active_ctrl_id_prepare2 = BATTLE_PREPARE2_CTRL_ID;
	if(0 != rst)
	{
		return rst;
	}
	//检测信息
	for(int i = 0;i < BI_MAX;++i)
	{
		if(0 == _competitor[i].entry_list.size())
		{
			__PRINTF("进入点数据为空\n");
			return -10103;
		}
	}
	for(int i = 0;i < BI_MAX;++i)
	{
		if(0 == _competitor[i].town_list.size())
		{
			__PRINTF("回城点数据为空\n");
			return -10104;
		}
	}
	if(0 == _competitor[BI_DEFENDER]._key_npc_list.size())
	{
		return -20000;
	}
	if(0 == _competitor[BI_ATTACKER]._key_npc_list.size())
	{
		return -20001;
	}
	return 0;
}

void bf_world_manager_battle::Reset()
{
	bf_world_manager_base::Reset();
}

void bf_world_manager_battle::RecordBattleBuilding(const A3DVECTOR& pos,int faction,int tid,bool is_key_building)
{
	__PRINTF("RecordBattleBuilding pos(%f,%f,%f),faction= %d,tid= %d,is_key_building= %d\n",
		pos.x,pos.y,pos.z,faction,tid,is_key_building);
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_competitor[BI_ATTACKER]._key_npc_list.insert(tid);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_competitor[BI_DEFENDER]._key_npc_list.insert(tid);
	}
	else
	{
		ASSERT(false && "RecordBattleBuilding no faction");
	}
}

int bf_world_manager_battle::OnMobDeath(world * pPlane,int faction,int tid,const A3DVECTOR& pos, int attacker_id)
{
	__PRINTF("城战 (battle_id= %d,world_tag= %d),key npc(tid= %d) 死亡\n",
		_battle_id,GetWorldTag(),tid);
	spin_autolock keeper(_battle_lock);
	npc_template* pTemplate = npc_stubs_manager::Get(tid);
	if(!pTemplate) return 0;
	if(BS_RUNNING != _status)
	{
		__PRINTF("OnMobDeath but not running!\n");
		return 0;
	}
	if(!IsUseForBattle())
	{
		__PRINTF("OnMobDeath but not use for battle\n");
		return 0;
	}
	for(int i = 11;i <= 14;++i)
	{
		if(tid == _key_map[i].npc_tid)
		{
			_key_map[i].status = 0;
			SendBattleInfo();
		}
	}
	if(0 != pTemplate->war_role_config)
	{
		const battlefield_consturct_temp* bfct = Get(pTemplate->war_role_config);
		if(bfct)
		{
			if(_key_tid_map.find(pTemplate->war_role_config) != _key_tid_map.end())
			{
				int key = _key_tid_map[pTemplate->war_role_config];
				if(key >= 1 || key <= 10)
				{
					//合法key
					//发送
					_key_map[key].status = 0;
					packet_wrapper h1(64);
					using namespace S2C;
					CMD::Make<CMD::battlefield_building_status_change>::From(h1,key,0,pos,_key_map[key].hp_factor);
					if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
					spin_autolock keeper2(_city_detail_lock);
					//处理死亡信息
					if(_city_detail.csam[key].am[ATTACK].level > 0)
						_city_detail.csam[key].am[ATTACK].level -= 1;
					if(_city_detail.csam[key].am[DEFENCE].level > 0)
						_city_detail.csam[key].am[DEFENCE].level -= 1;
					if(_city_detail.csam[key].am[RANGE].level > 0)
						_city_detail.csam[key].am[RANGE].level -= 1;
					if(_city_detail.csam[key].am[AI_POLICY].level)
						_city_detail.csam[key].am[AI_POLICY].level -= 1;
					_city_detail.csam[key].am[MAX_HP].level = CITY_STRUCTURE_DEATH_INIT_HP;
					_key_map[key].status = 0;
					SendBattleInfo();
					if(14248 == _key_map[key].npc_tid ||
						14249 == _key_map[key].npc_tid ||
						14250 == _key_map[key].npc_tid)
					{
						//如果这3个怪有一个被打下来了,计算下次npc刷新时间
						time_t now = g_timer.get_systime();
						time_t t_base;
						struct tm dt;
						localtime_r(&now, &dt);
						dt.tm_sec = 0;
						dt.tm_min = 0;
						dt.tm_hour = 0;
						t_base = mktime(&dt);
						int next_day_22_clock = t_base + 86400 + 79200;
						_city_detail.service_npc_born_timestamp = next_day_22_clock;
						GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)  %d be killed, set next_day_22_clock",
								gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),_key_map[key].npc_tid);
						__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)  %d be killed, set next_day_22_clock\n",
								gmatrix::GetServerIndex(),GetBattleID(),GetWorldTag(),IsUseForBattle(),_key_map[key].npc_tid);
					}
				}
			}
		}
	}
	if(ROLE_IN_WAR_KEY_BUILDING != pTemplate->role_in_war)
	{
		__PRINTF("OnMobDeath but role_in_war != ROLE_IN_WAR_KEY_BUILDING\n");
		return 0;
	}
	if(BR_NONE != _battle_result)
	{
		__PRINTF("OnMobDeath but result form already!\n");
		return 0;
	}
	if(!(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND)))
	{
		__PRINTF("OnMobDeath but invalid faction!\n");
		return 0;
	}
	else if(faction & FACTION_OFFENSE_FRIEND)
	{
		if(_competitor[BI_ATTACKER]._key_npc_list.find(tid) == _competitor[BI_ATTACKER]._key_npc_list.end())
		{
			ASSERT(false && "没有key npc");
			return 0;
		}
		_battle_result = BR_DEFENDER_WIN;
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		if(_competitor[BI_DEFENDER]._key_npc_list.find(tid) == _competitor[BI_DEFENDER]._key_npc_list.end())
		{
			ASSERT(false && "没有key npc");
			return 0;
		}
		_battle_result = BR_ATTACKER_WIN;
	}
	return 0;
}

void bf_world_manager_battle::OnClose()
{
	bf_world_manager_base::OnClose();
	//这里根据结果和死亡情况调整数据
	if(BR_DEFENDER_WIN == _battle_result)
	{
		//不用做什么了,死亡时已经处理过了
		//剩余血量同步过了
	}
	else
	{
		//不是守方胜利
		spin_autolock keeper(_city_detail_lock);
		_city_detail.clear();
	}
	
}

void bf_world_manager_battle::OnBuyArcher(gplayer_imp* pImp)
{
	battlefiled_msg_t msg;
	msg.type = 2;
	msg.player_id = pImp->_parent->ID.id;
	size_t len;
	const void* pname = pImp->GetPlayerName(len);
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memset(msg.name,0,MAX_USERNAME_LENGTH);
	memcpy(msg.name,pname,len);
	msg.data1 = 1;
	GNET::SendBattleFieldBroadcast(pImp->OI_GetMafiaID(),BATTLEFIELD_BROADCAST_ID,&msg,sizeof(battlefiled_msg_t));
}

void bf_world_manager_battle::OnBattleNPCNotifyHP(int faction,int tid,int cur_hp,int max_hp)
{
	spin_autolock keeper(_battle_lock);
	npc_template* pTemplate = npc_stubs_manager::Get(tid);
	if(!pTemplate) return;
	if(BS_RUNNING != _status)
	{
		__PRINTF("OnMobDeath but not running!\n");
		return;
	}
	if(!IsUseForBattle())
	{
		__PRINTF("OnMobDeath but not use for battle\n");
		return;
	}
	for(int i = 11;i <= 14;++i)
	{
		if(tid == _key_map[i].npc_tid)
		{
			if(0 != max_hp)
			{
				_key_map[i].hp_factor = float(cur_hp)/max_hp;
				if(_key_map[i].hp_factor > 1.0f) _key_map[i].hp_factor = 1.0f;
				if(_key_map[i].hp_factor < 0.0f) _key_map[i].hp_factor = 0.0f;
			}
		}
	}
	if(0 != pTemplate->war_role_config)
	{
		const battlefield_consturct_temp* bfct = Get(pTemplate->war_role_config);
		if(bfct)
		{
			if(_key_tid_map.find(pTemplate->war_role_config) != _key_tid_map.end())
			{
				int key = _key_tid_map[pTemplate->war_role_config];
				if(key >= 1 || key <= 10)
				{
					_key_map[key].hp_factor = float(cur_hp)/max_hp;
					spin_autolock keeper(_city_detail_lock);
					_city_detail.csam[key].am[MAX_HP].level = cur_hp;
					if(_city_detail.csam[key].am[MAX_HP].level < CITY_STRUCTURE_DEATH_INIT_HP)
						_city_detail.csam[key].am[MAX_HP].level = CITY_STRUCTURE_DEATH_INIT_HP;
					if(_city_detail.csam[key].am[MAX_HP].level > CITY_STRUCTURE_MAX_HP)
						_city_detail.csam[key].am[MAX_HP].level = CITY_STRUCTURE_MAX_HP;
				}
			}
		}
	}
}

bf_world_manager_construction::bf_world_manager_construction()
{
	_service_npc_borned = false;
}

bf_world_manager_construction::~bf_world_manager_construction()
{
}

int bf_world_manager_construction::Init(const char* gmconf_file,const char* servername)
{
	int rst = bf_world_manager_base::Init(gmconf_file,servername, -1, -1);
	_active_ctrl_id_prepare2 = CONSTURCT_PREPARE2_CTRL_ID;
	if(0 != rst)
	{
		return rst;
	}
	//检测信息
	if(0 == _competitor[BI_DEFENDER].entry_list.size())
	{
		__PRINTF("进入点数据为空\n");
		return -10101;
	}
	if(0 == _competitor[BI_DEFENDER].town_list.size())
	{
		__PRINTF("回城点数据为空\n");
		return -10102;
	}
	return 0;
}

void bf_world_manager_construction::OnDeliveryConnected(std::vector<battle_field_info>& info)
{
	bf_world_manager_base::OnDeliveryConnected(info);
}

void bf_world_manager_construction::Reset()
{
	bf_world_manager_base::Reset();
}

void bf_world_manager_construction::OnPrepare()
{       
	ASSERT(_battle_lock);
	_cur_timestamp = g_timer.get_systime() + 1;
	//打开城战准备的控制器
	if(_active_ctrl_id_prepare)ActiveSpawn(this,_active_ctrl_id_prepare,true);
	if(_active_ctrl_id_prepare2)ActiveSpawn(this,_active_ctrl_id_prepare2,true);
}   

void bf_world_manager_construction::OnClose()
{
	bf_world_manager_base::OnClose();
}

void bf_world_manager_construction::Heartbeat()
{
	bf_world_manager_base::Heartbeat();
	spin_autolock keeper(_battle_lock);
	if(BS_RUNNING == _status)
	{
		if(0 == (_heartbeat_counter) % 20)
		{
			if(!_service_npc_borned && g_timer.get_systime() > _city_detail.service_npc_born_timestamp)
			{
				_service_npc_borned = true;
				ActiveSpawn(this,SERVICE_NPC_CTRL_ID,true);
			}
		}
		if(0 == (_heartbeat_counter) % (20 * 600))
		{
			PutCityDetail();
		}
	}
}

int bf_world_manager_construction::CreateBattleField(const bf_param& param,bool force)
{
	//首先判断状态是否能创建
	if(BS_READY != _status && !force)
	{
		return CBFR_INVALID_STATUS;
	}
	if(_battle_id != param.battle_id)
	{
		return CBFR_INVALID_BATTLE_ID;
	}
	if(_competitor[BI_DEFENDER].mafia_id == param.defender_mafia_id && 0 != param.defender_mafia_id)
	{
		//没变化,延时,其它什么也不用干
		_end_timestamp = param.end_timestamp;
		if(BS_RUNNING == _status)
		{
			_cur_timestamp = _end_timestamp;
		}
		_battle_result = BR_NONE;
		return 0;
	}
	return bf_world_manager_base::CreateBattleField(param,force);
}

