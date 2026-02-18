#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threadpool.h>
#include <conf.h>
#include <io/pollio.h>
#include <io/passiveio.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <glog.h>
#include <algorithm>

#include "../template/itemdataman.h"
#include "../template/npcgendata.h"
#include "bg_player.h"
#include "bg_world_manager.h"
#include "bg_world_message_handler.h"
#include <factionlib.h>



int bg_world_manager::Init( const char* gmconf_file, const char* servername, int tag, int index)
{
	__PRINTF( "战场：%s 初始化\n", servername );
	int ret = global_world_manager::Init( gmconf_file, servername, tag, index);
	if( 0 != ret )
	{
		return ret;
	}
	delete _message_handler;
	_message_handler = new bg_world_message_handler( this, &_plane );
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	section += servername;
	_battle_id = atoi( conf->find( section, "battle_id" ).c_str() );
	if( _battle_id <= 0 )
	{
		__PRINTF( "_battle_id=%d,不正确\n", _battle_id );
		return -10000;
	}
	__PRINTF( "战场id为：%d\n", _battle_id );
	_battle_type = atoi( conf->find( section, "battle_type" ).c_str() );
	__PRINTF( "battle_type为：%d\n", _battle_type );
	_max_player_count = atoi( conf->find( section, "battle_max_player_count" ).c_str() );
	if( _max_player_count <= 0 )
	{
		_max_player_count = DEFAULT_MAX_PLAYER_COUNT;
	}
	__PRINTF( "战场每方允许人数为：%d\n", _max_player_count );
	_kickout_time_min = atoi( conf->find( section, "battle_kickout_time_min" ).c_str() );
	if( _kickout_time_min <= 0 )
	{
		_kickout_time_min = DEFAULT_KICKOUT_TIME_MIN;
	}
	__PRINTF( "踢出玩家时间min:%d\n", _kickout_time_min );
	_kickout_time_max = atoi( conf->find( section, "battle_kickout_time_max" ).c_str() );
	if( _kickout_time_max <= 0 )
	{
		_kickout_time_max = DEFAULT_KICKOUT_TIME_MAX;
	}
	__PRINTF( "踢出玩家时间max:%d\n", _kickout_time_max );
	_kickout_close_time = atoi( conf->find( section, "battle_kickout_close_time" ).c_str() );
	if( _kickout_close_time <= 0 )
	{
		_kickout_close_time = DEFAULT_KICKOUT_CLOSE_TIME;
	}
	__PRINTF( "关闭延迟时间max:%d\n", _kickout_close_time );
	_need_level_min = atoi( conf->find( section, "battle_need_level_min" ).c_str() );
	if( _need_level_min < 0 || _need_level_min > 200 )
	{
		__PRINTF( "级别需求不正确 battle_need_level_min=%d\n", _need_level_min );
		return -10001;
	}
	__PRINTF( "battle_need_level_min=%d\n", _need_level_min );
	
	_need_level_max = atoi( conf->find( section, "battle_need_level_max" ).c_str() );
	if( _need_level_max <= 0 || _need_level_max > 200 )
	{
		__PRINTF( "级别需求不正确 battle_need_level_max=%d\n", _need_level_max );
		return -10002;
	}
	__PRINTF( "battle_need_level_max=%d\n", _need_level_max );
	if( _need_level_max < _need_level_min )
	{
		__PRINTF( "_need_level_max < _need_level_min\n" );
		return -10003;
	}

	_reborn_count_need = atoi( conf->find(section, "reborn_count_need").c_str() );
	if(_reborn_count_need < 0)
	{
		__PRINTF("进入战场条件读入有误 reborn_count_need=%d\n", _reborn_count_need);
		return -10004;
	}
	__PRINTF("reborn_count_need=%d\n", _reborn_count_need);

	_fighting_time = atoi( conf->find(section, "battle_fighting_time").c_str() );
	if(_fighting_time < 0)
	{
		__PRINTF("报名冷却时间读入有误 battle_fighting_time=%d\n", _fighting_time);
		return -10005;
	}
	__PRINTF("battle_fighting_time=%d\n", _fighting_time);

	_max_enter_perday = atoi( conf->find(section, "battle_max_enter_perday").c_str() );
	if(_max_enter_perday < -1)
	{
		__PRINTF("玩家每天能参加的次数读入有误 battle_max_enter_perday=%d\n", _max_enter_perday);
		return -10006;
	}
	__PRINTF("battle_max_enter_perday=%d\n", _max_enter_perday);

	_cooldown_time = atoi( conf->find(section, "battle_cooldown_time").c_str() );
	if(_cooldown_time < 0)
	{
		__PRINTF("玩家报名冷却时间读入有误 battle_cooldown_time=%d\n", _cooldown_time);
		return -10007;
	}
	__PRINTF("battle_cooldown_time=%d\n", _cooldown_time);

	_bg_template_id = atoi( conf->find(section, "battle_template_id").c_str());
	__PRINTF("bg_template_id=%d\n", _bg_template_id);

	std::string min_player_deduct_score_str = conf->find(section,"min_player_deduct_score");
	if (!min_player_deduct_score_str.empty())
	{
		 _min_player_deduct_score = atoi(min_player_deduct_score_str.c_str());
		 __PRINTF("min_player_deduct_score=%d\n",_min_player_deduct_score);
	}

	std::string first_deduct_off_line_time_str = conf->find(section,"first_deduct_off_line_time");
	if (!first_deduct_off_line_time_str.empty())
	{
		_first_deduct_off_line_time = atoi(first_deduct_off_line_time_str.c_str());
		__PRINTF("first_deduct_off_line_time=%d\n",_first_deduct_off_line_time);
	}

	std::string delta_time_str = conf->find(section,"delta_time");
	if (!delta_time_str.empty())
	{
		_delta_time = atoi(delta_time_str.c_str());
		__PRINTF("delta_time=%d\n",_delta_time);
	}

	std::string deduct_score_str = conf->find(section,"deduct_score");
	if (!deduct_score_str.empty())
	{
		_deduct_score = atoi(deduct_score_str.c_str());
		__PRINTF("deduct_score=%d\n",_deduct_score);
	}

	if(!OnInit()) return -10010;

	Reset();
	return 0;
}

void bg_world_manager::KillAllMonsters()
{	
	//需要残留怪物,物品等检测 全杀,策划保证战斗停止后所有控制器不会再触发	
	__PRINTF("战场(gs_id: %d,battle_id: %d,world_tag: %d) 杀死所有npc\n",		gmatrix::GetServerIndex(),_battle_id,GetWorldTag());	
	MSG msg;	
	BuildMessage(msg,GM_MSG_BATTLE_NPC_DISAPPEAR,XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
	std::vector<exclude_target> empty;
	_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF,empty);
}

void bg_world_manager::Heartbeat()
{
	global_world_manager::Heartbeat();
	++_heartbeat_counter; 
	++_off_line_heartbeat_counter;
	if( (_heartbeat_counter) % 20 == 0 )
	{
		spin_autolock keeper( _battle_lock );
		switch ( _status )
		{
			case BS_READY:
			{
			}
			break;
			
			case BS_RUNNING:
			{
				OnRunning();
				// 战斗逻辑,如果时间到或者产生结果了
				if( _cur_timestamp < g_timer.get_systime() || 0 != _battle_result )
				{
					OnClose();
					_status = BS_CLOSING;
					_cur_timestamp = g_timer.get_systime() + _kickout_close_time;

					// 战斗结束时更新一次信息
					SendBattleInfo();

					// 计算战斗结果
					__PRINTF( "战场:%d 结束发布结果开始准备清理result=%d, min=%d, max=%d, close=%d\n", 
						GetWorldTag(), _battle_result, 
						_kickout_time_min, _kickout_time_max, _kickout_close_time );
					GLog::log( GLOG_INFO, "战场结束,gs_id=%d, battle_world_tag=%d, battle_id=%d, end_time=%d,",
						gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_id, _end_timestamp );
				}
			}
			break;

			case BS_CLOSING:
			{
				// 这个期间踢出玩家
				// 在玩家heartbeat逻辑里完成规定时间内的踢人
				if( _cur_timestamp < g_timer.get_systime() )
				{
					KillAllMonsters();
					// 在这之前需要确保非gm玩家已经被踢出
					Reset();
					OnReady();
					__PRINTF( "战场:%d，恢复到初始状态蝄n", GetWorldTag() );
					// 通知delivery
					GNET::SendBattleEnd( gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_id, _battle_type); 
					// 不需要残留怪物检测，策划保证所有怪物在45后消失
					GLog::log( GLOG_INFO, "战场清理结束,gs_id=%d, battle_world_tag=%d, battle_id=%d, end_time=%d,",
						gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_id, _end_timestamp );
				}
			}
			break;

			default:
			{
				__PRINTF( "ERROR! _status=%d\n", _status );
				ASSERT( false );
			}
		};
	}
	// 每30秒更新一次信息
	if( (_heartbeat_counter) % ( 30 * 20 ) == 0 )
	{
		SendBattleInfo();
		_heartbeat_counter = 0;
	}
}

void bg_world_manager::ForceClose()
{
	spin_autolock keeper( _battle_lock );
	if(_status == BS_CLOSING) return;
	OnClose();
	_status = BS_CLOSING;
	_cur_timestamp = g_timer.get_systime() + _kickout_close_time;

	// 战斗结束时更新一次信息
	SendBattleInfo();

	// 计算战斗结果
	__PRINTF( "强制结束战场:%d 结束发布结果开始准备清理result=%d, min=%d, max=%d, close=%d\n", GetWorldTag(), _battle_result, _kickout_time_min, _kickout_time_max, _kickout_close_time );
	GLog::log( GLOG_INFO, "战场强制结束,gs_id=%d, battle_world_tag=%d, battle_id=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_id, _end_timestamp );
}

void bg_world_manager::PostPlayerLogin( gplayer* pPlayer )
{
	// never come here
}

int bg_world_manager::OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm )
{
	//跨服可以直接登录战场
	if(gmatrix::IsZoneServer() && IsBattleRunning())
	{
		__PRINTF("玩家 %d 在跨服直接登录战场 %d", pInfo->id, GetWorldTag());
		GLog::log( GLOG_INFO, "玩家 %d 在跨服直接登录战场 %d", pInfo->id, GetWorldTag() );
		return 0;
	}

	GLog::log( GLOG_ERR, "玩家 %d 从下线状态直接进入战场地图 %d, 数据错误", pInfo->id, GetWorldTag() );
	return -1000;
}

void bg_world_manager::OnDeliveryConnected( std::vector<battle_field_info>& info )
{
	__PRINTF( "收集战场注册信息line_id=%d, battle_id=%d, battle_world_tag=%d, min_level=%d, max_level=%d\n",
		gmatrix::Instance()->GetServerIndex(), _battle_id, GetWorldTag(), _need_level_min, _need_level_max );
	GLog::log( GLOG_INFO, "收集战场注册信息line_id=%d, battle_id=%d, battle_world_tag=%d, min_level=%d, max_level=%d\n",
		gmatrix::Instance()->GetServerIndex(), _battle_id, GetWorldTag(), _need_level_min, _need_level_max );
	battle_field_info temp;
	temp.battle_id = _battle_id;
	temp.battle_world_tag = GetWorldTag();
	temp.level_min = _need_level_min;
	temp.level_max = _need_level_max;
	temp.battle_type = _battle_type;
	temp.reborn_count_need = _reborn_count_need; 
	temp.fighting_time     = _fighting_time;
	temp.max_enter_perday  = _max_enter_perday;
	temp.cooldown_time     = _cooldown_time;
	temp.max_player_count  = _max_player_count;
	// 这里只收集信息,在外面统一发一个协议吧
	info.push_back( temp );
}

bool bg_world_manager::CreateBattleGround( const bg_param& param )
{
	spin_autolock keeper( _battle_lock );
	__PRINTF( "战场创建请求, battle_world_tag=%d, batlte_id=%d, end_timestemp=%d\n",
			param.battle_world_tag, param.battle_id, param.end_timestemp );
	// 首先判断状态是否能创建
	if( BS_READY != _status )
	{
		__PRINTF( "创建战场失败，当前状态不为BS_READY, _status=%d\n", _status );
		GLog::log( GLOG_INFO, "战场开启失败,原因：战场状态不为BS_READY"
				"battle_world_tag=%d, battle_id=%d ,end_timestemp=%d,_status=%d\n",
				param.battle_world_tag, param.battle_id, param.end_timestemp, _status );
		return false;
	}
	// 判断时间戳
	int t = param.end_timestemp - g_timer.get_systime();
	if( t < 0 )
	{
		__PRINTF( "end_timestemp参数有误，持续时间为负 t=%d\n", t );
		GLog::log( GLOG_INFO, "战场开启失败,原因：持续时间为负数"
				"battle_world_tag=%d, battle_id=%d, end_time=%d,time=%d\n",
				param.battle_world_tag, param.battle_id, param.end_timestemp, t );
		return false;
	}
	_start_timestamp = g_timer.get_systime();
	_end_timestamp = param.end_timestemp;
	_cur_timestamp = _end_timestamp;
	_status = BS_RUNNING;
	_battle_id = param.battle_id;
	_apply_count = param.apply_count;
	_off_line_heartbeat_counter = _heartbeat_counter % 20;

	OnCreate();

	__PRINTF( "战场:%d:%d开启,持续时间:%d\n", _battle_id, GetWorldTag(), t ); 
	GLog::log( GLOG_INFO, "战场开启成功 battle_world_tag=%d, battle_id=%d, end_time=%d\n",
			param.battle_world_tag, param.battle_id, param.end_timestemp );
	return true;
}

void bg_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{
	__PRINTF( "int bg_world_manager::PlayerEnter, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	AddMapNode( _all_list, pPlayer );
	player_battle_info tempinfo;
	memset( &tempinfo, 0, sizeof(player_battle_info) );
	tempinfo.battle_faction = faction;	
	tempinfo.rank = 1;

	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	tempinfo.battle_score = pImp->GetBattleScore();
	if( faction == BF_ATTACKER )
	{
		_attacker_data.player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		_defender_data.player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode( _defender_list, pPlayer );
	}
	else if( faction == BF_MELEE ) 
	{
		tempinfo.battle_score = 0; 
		_melee_data.player_info_map[pPlayer->ID.id] = tempinfo;
	}
	SendBattleInfo();
}

void bg_world_manager::PlayerLeave( gplayer* pPlayer, int faction, bool cond_kick, bool disconnected)
{
	__PRINTF( "int bg_world_manager::PlayerLeave, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	DelMapNode( _all_list, pPlayer );
	PlayerInfoIt it;
	if( faction == BF_ATTACKER )
	{
		DelMapNode( _attacker_list, pPlayer );
		it = _attacker_data.player_info_map.find( pPlayer->ID.id );
		if( it != _attacker_data.player_info_map.end() )
		{
			_attacker_data.player_info_map.erase( it );
		}
	}
	else if( faction == BF_DEFENDER )
	{
		DelMapNode( _defender_list, pPlayer );
		it = _defender_data.player_info_map.find( pPlayer->ID.id );
		if( it != _defender_data.player_info_map.end() )
		{
			_defender_data.player_info_map.erase( it );
		}
	}
	else if( faction == BF_MELEE )
	{
		it = _melee_data.player_info_map.find(pPlayer->ID.id);
		if(it != _melee_data.player_info_map.end())
		{
			_melee_data.player_info_map.erase(it);
		}
	}
	OnPlayerLeave(pPlayer, faction, cond_kick);
	SendBattleInfo();
}

void bg_world_manager::OnPlayerLeave(gplayer* pPlayer, int faction, bool cond_kick)
{
	GNET::SendPlayerLeave(gmatrix::GetServerIndex(),_battle_id,GetWorldTag(),pPlayer->ID.id,cond_kick);
}

void bg_world_manager::DumpWorldMsg()
{
	__PRINTF( "-------------------- world message dump --------------------\n" );
	__PRINTF( "battle_id=%d, tag=%d, status=%d, result=%d\n", _battle_id, GetWorldTag(), _status, _battle_result );
	__PRINTF( "总人数:%d, 攻击方人数:%d, 防守方人数:%d\n",
		_attacker_data.player_info_map.size() + _defender_data.player_info_map.size(),
		_attacker_data.player_info_map.size(), _defender_data.player_info_map.size() ); 

	__PRINTF( "玩家信息:\n" );
	PlayerInfoIt it;
	for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
	{
		__PRINTF( "\tid=%d,battle_faction=%d,battle_score=%d,kill=%d,death=%d\n",
			it->first, it->second.battle_faction, it->second.battle_score, it->second.kill, it->second.death );
	}
	for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
	{
		__PRINTF( "\tid=%d,battle_faction=%d,battle_score=%d,kill=%d,death=%d\n",
			it->first, it->second.battle_faction, it->second.battle_score, it->second.kill, it->second.death );
	}
	__PRINTF( "\n" );
}

void bg_world_manager::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("注册回城点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//现在还无法区分攻方和攻方辅助方
		_attacker_data.town_list.push_back(pos);
	}       
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.town_list.push_back(pos);
	}       
	else
	{
		ASSERT(false && "注册回城点 faction错误 ");
	}
}

void bg_world_manager::RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id)
{
	__PRINTF("注册复活点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	competitor_data::revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//允许同一控制器控制多个
		_attacker_data.revive_map[cond_id].push_back(temp);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.revive_map[cond_id].push_back(temp);
	}
	else
	{
		ASSERT(false && "注册复活点 faction错误 ");
	}
}

void bg_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{       
	__PRINTF("注册进入点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	if(faction & FACTION_OFFENSE_FRIEND)
	{       
		//现在还无法区分攻方和攻方辅助方
		_attacker_data.entry_list.push_back(pos);
	}       
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.entry_list.push_back(pos);
	}
	else
	{
		ASSERT(false && "注册进入点 no faction");
	}       
}      

void bg_world_manager::RecordBattleBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building )
{
	__PRINTF( "RecordBattleBuilding pos(%f,%f,%f), faction=%d, tid=%d, key=%d\n",
		pos.x, pos.y, pos.z, faction, tid, is_key_building );
	ASSERT( faction & ( FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND ) );
	if( faction & FACTION_OFFENSE_FRIEND )
	{
		if( is_key_building )
		{
			++_attacker_data.buildings.key_building_init;
		}
		else
		{
			++_attacker_data.buildings.normal_building_init;
		}
	}
	else
	{
		if( is_key_building )
		{
			++_defender_data.buildings.key_building_init;
		}
		else
		{
			++_defender_data.buildings.normal_building_init;
		}
	}
}

bool bg_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	//获得随机复活点
	int battle_faction = pImp->GetBattleFaction();
	spin_autolock keeper(_battle_lock);
	std::vector<A3DVECTOR> revive_pos_list;
	competitor_data::CtrlReviveMapIt it;
	if(BF_ATTACKER == battle_faction)
	{
		for(it = _attacker_data.revive_map.begin();it != _attacker_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{
				//诛仙的战场里复活点控制器不用了,真晕-_-b
				revive_pos_list.push_back(it->second[i].pos);
			}
		}
	}
	else if(BF_DEFENDER == battle_faction)
	{
		for(it = _defender_data.revive_map.begin();it != _defender_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{       
				revive_pos_list.push_back(it->second[i].pos);
			}               
		}
	}
	else if(BF_MELEE == battle_faction) 
	{
		for(it = _melee_data.revive_map.begin();it != _melee_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{       
				revive_pos_list.push_back(it->second[i].pos);
			}               
		}
	}
	else
	{
		return false;
	}
	size_t size = revive_pos_list.size();
	if(0 == size) return false;
	pos = revive_pos_list[abase::Rand(0,revive_pos_list.size() - 1)];
	world_tag = GetWorldTag();
	return true;
}

bool bg_world_manager::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag)
{
	return false;
}       

void bg_world_manager::GetRandomEntryPos(A3DVECTOR& pos,int battle_faction)
{
	pos = A3DVECTOR(0,0,0);
	spin_autolock keeper(_battle_lock);
	if(BF_ATTACKER == battle_faction)
	{       
		if(0 == _attacker_data.entry_list.size()) return;
		pos = _attacker_data.entry_list[abase::Rand(0,_attacker_data.entry_list.size() - 1)];
	}
	else if(BF_DEFENDER == battle_faction)
	{       
		if(0 == _defender_data.entry_list.size()) return;
		pos = _defender_data.entry_list[abase::Rand(0,_defender_data.entry_list.size() - 1)];
	}
	else if(BF_MELEE == battle_faction) 
	{
		if(0 == _melee_data.entry_list.size()) return;
		pos = _melee_data.entry_list[abase::Rand(0,_melee_data.entry_list.size() - 1)];
	}
	//gm和旁观者用这个
	else
	{
		if(0 == _defender_data.entry_list.size()) return;
		pos = _defender_data.entry_list[abase::Rand(0,_defender_data.entry_list.size() - 1)];
	}
} 
	
int bg_world_manager::TranslateCtrlID( int which )
{
	if( which >= CTRL_CONDISION_ID_BATTLE_MIN && which <= CTRL_CONDISION_ID_BATTLE_MAX )
	{
		return CTRL_ID_BATTLE_BASE +
			( GetWorldTag() - BATTLE_WORLD_TAG_BEGIN ) * CTRL_CONDISION_ID_BATTLE_OFFSET +
			( which - CTRL_CONDISION_ID_BATTLE_MIN );
	}
	return which;
}

void bg_world_manager::SendBattleInfo()
{
	_battle_info_seq ++;
}

void bg_world_manager::HandleModifyBattleDeath( int player_id, int death_count, size_t content_length, const void *content)
{
	spin_autolock keeper( _battle_lock );
	if(BS_RUNNING != _status) return;
	PlayerInfoIt it;
	it = _attacker_data.player_info_map.find( player_id );
	if( it != _attacker_data.player_info_map.end() )
	{       
		++_defender_data.kill_count;
		it->second.death = death_count;
		UpdatePlayerInfo(it, content_length, content);
	}  
	else
	{
		it = _defender_data.player_info_map.find( player_id );
		if( it != _defender_data.player_info_map.end() )
		{       
			++_attacker_data.kill_count;
			it->second.death = death_count;
			UpdatePlayerInfo(it, content_length, content);
		}
		else
		{
			it = _melee_data.player_info_map.find( player_id );
			if( it != _melee_data.player_info_map.end() )
			{       
				++_melee_data.kill_count;
				it->second.death = death_count;
				UpdatePlayerInfo(it, content_length, content);
			}
		}
	}
	SendBattleInfo();
}


void bg_world_manager::HandleModifyBattleKill( int player_id, int kill_count, size_t content_length, const void *content)
{
	spin_autolock keeper( _battle_lock );
	if(BS_RUNNING != _status) return;
	PlayerInfoIt it;
	it = _attacker_data.player_info_map.find( player_id );
	if( it != _attacker_data.player_info_map.end() )
	{       
		it->second.kill = kill_count;
		UpdatePlayerInfo(it, content_length, content);
	}  
	else
	{
		it = _defender_data.player_info_map.find( player_id );
		if( it != _defender_data.player_info_map.end() )
		{       
			it->second.kill = kill_count;
			UpdatePlayerInfo(it, content_length, content);
		}
		else
		{
			it = _melee_data.player_info_map.find( player_id );
			if( it != _melee_data.player_info_map.end() )
			{       
				it->second.kill = kill_count;
				UpdatePlayerInfo(it, content_length, content);
			}
		}
	}
	SendBattleInfo();
}

void bg_world_manager::HandleSyncBattleInfo(int player_id, int reserver,  size_t content_length, const void *content)
{
	spin_autolock keeper( _battle_lock );
	if(BS_RUNNING != _status && BS_CLOSING != _status) return;
	
	PlayerInfoIt it;
	it = _attacker_data.player_info_map.find( player_id );
	if( it != _attacker_data.player_info_map.end() )
	{       
		UpdatePlayerInfo(it, content_length, content);
	}  
	else
	{
		it = _defender_data.player_info_map.find( player_id );
		if( it != _defender_data.player_info_map.end() )
		{       
			UpdatePlayerInfo(it, content_length, content);
		}
		else
		{
			it = _melee_data.player_info_map.find( player_id );
			if( it != _melee_data.player_info_map.end() )
			{       
				UpdatePlayerInfo(it, content_length, content);
			}
		}
	}
	SendBattleInfo();
}

void bg_world_manager::BattleFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int emote_id, int self_id)
{
	if( faction & (FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND) )
	{
		multi_send_chat_msg( _attacker_list, buf, size, channel, emote_id, aux_data, dsize, self_id);
	}       
	else if( faction & (FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND) )
	{
		multi_send_chat_msg( _defender_list, buf, size, channel, emote_id, aux_data, dsize, self_id);
	}
}


void bg_world_manager::UpdatePlayerInfo(PlayerInfoIt &it, size_t content_length, const void * content)
{
	if(content_length == sizeof(player_battle_info))
       	{
		it->second = *(player_battle_info*)content;
	}
}

void bg_world_manager::OnPlayerDeath( int killer, int deadman, int deadman_battle_faction, bool& kill_much )
{
	spin_autolock keeper( _battle_lock );
	// 检查第一次击杀情况,现在改成双方只有一个第一滴血了。。。。。
	if( deadman_battle_faction == BF_DEFENDER)
	{
		if( _attacker_data.first_kill )
		{
			_attacker_data.first_kill = false;
			_defender_data.first_kill = false;
			// 发送信息
			BroadcastBattleMsg( 0, 1, _battle_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
			__PRINTF( "在 %d 战场(%d线), %d 杀死了 %d, 获得第一滴血\n",
				_battle_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
		}
	}
	else if( deadman_battle_faction == BF_ATTACKER)
	{
		if( _defender_data.first_kill )
		{
			_defender_data.first_kill = false;
			_attacker_data.first_kill = false;
			// 发送信息
			BroadcastBattleMsg( 0, 2, _battle_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
			__PRINTF( "在 %d 战场(%d线), %d 杀死了 %d, 获得第一滴血\n",
				_battle_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
		}
	}
	// 检查阻止杀戮
	if( kill_much )
	{
		kill_much = false;
		// 发送信息
		BroadcastBattleMsg( 2, deadman_battle_faction == BF_DEFENDER?1:2,
			_battle_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
		__PRINTF( "在 %d 战场(%d线), %d 成功阻止了 %d 的杀戮\n",
			_battle_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
	}
}

int bg_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	npc_template * pTemplate = npc_stubs_manager::Get( tid );
	if( !pTemplate ) return 0;
	if( ROLE_IN_WAR_BATTLE_KEY_BUILDING == pTemplate->role_in_war )
	{
		OnDestroyKeyBuilding( faction );
	}
	else if( ROLE_IN_WAR_BATTLE_NORMAL_BUILDING == pTemplate->role_in_war) 
	{
		OnDestroyNormalBuilding( faction );
	}
	return 0; 
}

int bg_world_manager::OnDestroyKeyBuilding( int faction )
{
	spin_autolock keeper( _battle_lock );
	if( _battle_result || BS_RUNNING != _status ) return -1;
	if( faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
	{
		_battle_result = BATTLE_DEFENDER_WIN;
	}
	else
	{
		_battle_result = BATTLE_ATTACKER_WIN;
	}
	SendBattleInfo();
	return 0;
}

void bg_world_manager::BroadcastBattleMsg( unsigned char type, unsigned char battle_faction, 
				int battle_id, int gs_id, int killer, int deadman )
{
#pragma pack(1)
	struct
	{
		unsigned char type;		// 类型 0 第一次击杀 1 杀神 2 阻止杀戮
		unsigned char battle_faction;	// killer的阵营 1 攻方 2 守方
		int battle_id;			// 战场id
		int gs_id;			// 线号
		int killer;			// 杀人玩家id
		int deadman;			// 被杀玩家id
	} data;
#pragma pack()
	data.type = type;
	data.battle_faction = battle_faction;
	data.battle_id = battle_id;
	data.gs_id = gs_id;
	data.killer = killer;
	data.deadman = deadman;
	//broadcast_chat_msg( CHAT_BATTLE, &data, sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, 0 );
	multi_send_chat_msg( _all_list, &data,sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0,0, CHAT_BATTLE);
}

int bg_world_manager::GetPostion(int id, int score)
{
	spin_autolock keeper( _battle_lock );
	if(_attacker_data.player_info_map.find(id) == _attacker_data.player_info_map.end() && 
			_defender_data.player_info_map.find(id) == _defender_data.player_info_map.end())
	{
		return 0; // 找不到这个玩家
	}

	int pos = 1;
	pos += std::count_if(_attacker_data.player_info_map.begin(), _attacker_data.player_info_map.end(), scorePredicate(score));	
	pos += std::count_if(_defender_data.player_info_map.begin(), _defender_data.player_info_map.end(), scorePredicate(score));
	return pos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool cb_world_manager::OnInit()
{
	if( 0 == _attacker_data.buildings.normal_building_init || 0 == _attacker_data.buildings.key_building_init ||
		0 == _defender_data.buildings.normal_building_init || 0 == _defender_data.buildings.key_building_init ||
		_attacker_data.buildings.normal_building_init != _defender_data.buildings.normal_building_init ||
		_attacker_data.buildings.key_building_init != _defender_data.buildings.key_building_init )
	{
		return false;
	}
	return true;
}

int cb_world_manager::OnDestroyNormalBuilding( int faction )
{	
	spin_autolock keeper( _battle_lock );
	if( _battle_result || BS_RUNNING != _status ) return -1;
	if( faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
	{
		--_attacker_data.buildings.normal_building_left;
		if( _attacker_data.buildings.normal_building_left <= 0 )
		{
			ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_ATTACKER_KEY, true );
		}
	}
	else
	{
		--_defender_data.buildings.normal_building_left;
		if( _defender_data.buildings.normal_building_left <= 0 )
		{
			ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_DEFENDER_KEY, true );
		}
	}
	SendBattleInfo();
	return 0;
}

bool cb_world_manager::GetCommonBattleInfo( int& info_seq, unsigned char& attacker_building_left, 
		unsigned char& defender_building_left, std::vector<player_info_battle>& info, bool bForce )
{
	spin_autolock keeper( _battle_lock );
	if( _battle_info_seq != info_seq || bForce )
	{
		info.clear();
		info_seq = _battle_info_seq;
		
		//飞升战场只统计打掉的建筑数量
		//Note: 对于飞升战场，打掉的建筑数量有可能超过初始话的数量， 因为有的建筑会重新刷出
		if(_battle_type == BT_REBORN_COMMON)
		{
			attacker_building_left = _attacker_data.buildings.normal_building_init - _attacker_data.buildings.normal_building_left;
			defender_building_left = _defender_data.buildings.normal_building_init - _defender_data.buildings.normal_building_left;
		}
		else
		{	attacker_building_left = _attacker_data.buildings.normal_building_left;
			defender_building_left = _defender_data.buildings.normal_building_left;
		}

		S2C::CMD::battle_info::player_info_in_battle temp;
		PlayerInfoIt it;
		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.battle_score = it->second.battle_score;
			temp.kill = it->second.kill;
			temp.death= it->second.death;
			info.push_back( temp );
		}
		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.battle_score = it->second.battle_score;
			temp.kill = it->second.kill;
			temp.death= it->second.death;
			info.push_back( temp );
		}
		return true;
	}
	return false;
}


void cb_world_manager::FindMaxKillerID()
{
	int max_kill_count = 0;
	int count = 0;
	PlayerInfoMap::iterator it;
	// 先找出最大值
	for(it = _attacker_data.player_info_map.begin();it != _attacker_data.player_info_map.end(); ++it)
	{
		if(it->second.kill > max_kill_count) max_kill_count = it->second.kill;
	}
	for(it = _defender_data.player_info_map.begin();it != _defender_data.player_info_map.end(); ++it)
	{
		if(it->second.kill > max_kill_count) max_kill_count = it->second.kill;
	}
	// 比较谁是最大值,记录id 数量
	for(it = _attacker_data.player_info_map.begin();it != _attacker_data.player_info_map.end(); ++it)
	{
		if(it->second.kill == max_kill_count)
		{
			_max_killer_id = it->first;
			++count;
		}
	}
	for(it = _defender_data.player_info_map.begin();it != _defender_data.player_info_map.end(); ++it)
	{
		if(it->second.kill == max_kill_count)
		{
			_max_killer_id = it->first;
			++count;
		}
	}
	// 如果不只一个是最大的不要, 如果没杀人的也不要
	if(1 != count || 0 == max_kill_count) _max_killer_id = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool kf_world_manager::GetKillingFieldInfo(int& info_seq, int& attacker_score, int& defender_score, 
	int& attacker_count, int& defender_count, std::vector<player_info_killingfield>& info, bool bForce)
{
	if( _battle_info_seq != info_seq || bForce )
	{
		spin_autolock keeper( _battle_lock );
		info.clear();
		info_seq = _battle_info_seq;

		attacker_score = GetAttackerScore();
		defender_score = GetDefenderScore();
		attacker_count = _attacker_data.player_info_map.size();
		defender_count = _defender_data.player_info_map.size();
		info = _top_list;
		return true;
	}
	return false;
}

void kf_world_manager::UpdateTopList()
{
	player_info_killingfield temp;
	PlayerInfoIt it;
	_top_list.clear();
	for(it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it)
	{
		temp.id = it->first;
		temp.faction = it->second.battle_faction;
		temp.score = it->second.battle_score;
		temp.rank = it->second.rank;
		temp.conKill = it->second.conKill;
		_top_list.push_back( temp );
	}

	for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
	{
		temp.id = it->first;
		temp.faction = it->second.battle_faction;
		temp.score = it->second.battle_score;
		temp.rank = it->second.rank;
		temp.conKill = it->second.conKill;
		_top_list.push_back( temp );
	}

	std::sort(_top_list.begin(), _top_list.end(), CompareScore());
	if(_top_list.size() > TOP_LIST_MAX_NUM)
	{
		_top_list.resize(TOP_LIST_MAX_NUM);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//注册回城点
void ar_world_manager::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("注册回城点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND) || faction == 0);
	_melee_data.town_list.push_back(pos);
}

//注册复活点
void ar_world_manager::RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id)
{
	__PRINTF("注册复活点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND) || faction == 0);
	competitor_data::revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	_melee_data.revive_map[cond_id].push_back(temp);
}

//注册进入点
void ar_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("注册进入点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND) || faction == 0);
	_melee_data.entry_list.push_back(pos);
}


bool ar_world_manager::GetArenaInfo(int& info_seq, int& apply_count, int& remain_count, std::vector<player_info_arena> & info, bool bForce)
{
	if( _battle_info_seq != info_seq || bForce )
	{
		info_seq = _battle_info_seq;
		apply_count = _apply_count;
		remain_count = _melee_data.player_info_map.size();
		info = _top_list;
		return true;
	}
	return false;
}


void ar_world_manager::UpdateTopList()
{
	player_info_arena temp;
	PlayerInfoIt it;
	_top_list.clear();
	for(it = _melee_data.player_info_map.begin(); it != _melee_data.player_info_map.end(); ++it)
	{
		temp.score = it->second.battle_score;
		temp.kill_count = it->second.kill;
		_top_list.push_back(temp);
	}
	std::sort(_top_list.begin(), _top_list.end(), CompareScore());
	if(_top_list.size() > TOP_LIST_MAX_NUM)
	{
		_top_list.resize(TOP_LIST_MAX_NUM);
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////
void cs_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{
	__PRINTF( "int cs_world_manager::PlayerEnter, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	AddMapNode( _all_list, pPlayer );
	player_battle_info tempinfo;
	memset( &tempinfo, 0, sizeof(player_battle_info) );
	tempinfo.battle_faction = faction;	

	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	tempinfo.battle_score = pImp->GetBattleScore();
	if( faction == BF_ATTACKER )
	{
		PlayerInfoIt iter = _attacker_data.player_info_map.find(pPlayer->ID.id);
		//已经存在
		if(iter != _attacker_data.player_info_map.end())
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER, pPlayer->ID, XID(-1,-1), A3DVECTOR(0,0,0), iter->second.kill, iter->second.death);
			gmatrix::SendMessage(msg);
		}
		else
		{
			_attacker_data.player_info_map[pPlayer->ID.id] = tempinfo;
		}
		AddMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		PlayerInfoIt iter = _defender_data.player_info_map.find(pPlayer->ID.id);
		//已经存在
		if(iter != _defender_data.player_info_map.end())
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER, pPlayer->ID, XID(-1,-1), A3DVECTOR(0,0,0), iter->second.kill, iter->second.death);
			gmatrix::SendMessage(msg);
		}
		else
		{
			_defender_data.player_info_map[pPlayer->ID.id] = tempinfo;
		}
		AddMapNode( _defender_list, pPlayer );
	}
	SendBattleInfo();
}

void cs_world_manager::PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick, bool disconnected)
{
	__PRINTF( "int cs_world_manager::PlayerLeave, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	DelMapNode( _all_list, pPlayer );

	if( faction == BF_ATTACKER )
	{
		DelMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		DelMapNode( _defender_list, pPlayer );
	}
	OnPlayerLeave(pPlayer, faction, cond_kick);
	SendBattleInfo();
}


bool cs_world_manager::GetCrossServerBattleInfo(int& info_seq, S2C::CMD::crossserver_battle_info& info, bool bForce)
{
	if( _battle_info_seq != info_seq || bForce )
	{
		info_seq = _battle_info_seq;

		info.attacker_score = _attacker_data.kill_count;
		info.defender_score = _defender_data.kill_count;
		info.attacker_off_line_score = _off_line_score_list[BF_ATTACKER];
		info.defender_off_line_score = _off_line_score_list[BF_DEFENDER];
		info.attacker_count = get_cs_user_map_size(_attacker_list);
		info.defender_count = get_cs_user_map_size(_defender_list);
		info.min_player_deduct_score= _min_player_deduct_score;
		info.end_timestamp = _end_timestamp;

		return true;
	}
	return false;
}

bool cs_world_manager::GetCrossServerOfflineInfo(std::vector<player_off_line_in_cs_battle>& vec_info)
{
	size_t counter = _vec_off_line_list.size();
	for (size_t i = 0; i < counter; i++)
	{
		vec_info.push_back(_vec_off_line_list[i]);
	}
	return true;
}

bool cs_world_manager::GetCommonBattleInfo( int& info_seq, unsigned char& attacker_building_left, 
		unsigned char& defender_building_left, std::vector<player_info_battle>& info, bool bForce )
{
	spin_autolock keeper( _battle_lock );
	if( _battle_info_seq != info_seq || bForce )
	{
		info.clear();
		info_seq = _battle_info_seq;

		attacker_building_left = 0; 
		defender_building_left = 0; 
		
		S2C::CMD::battle_info::player_info_in_battle temp;
		PlayerInfoIt it;
		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.battle_score = it->second.battle_score;
			temp.kill = it->second.kill;
			temp.death= it->second.death;
			info.push_back( temp );
		}
		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.battle_score = it->second.battle_score;
			temp.kill = it->second.kill;
			temp.death= it->second.death;
			info.push_back( temp );
		}
		return true;
	}
	return false;
}

bool cs_world_manager::InitiForbiddenItemList()
{
	_forbidden_item_set.clear();

	ONET::Conf *conf = ONET::Conf::GetInstance();
	std::string root = conf->find("Template","Root");
	std::string path = root + conf->find("Template", "CSTeamPKForbiddenItemList");

	std::ifstream in( path.c_str() );
	if(!in) return false;
	std::string str;
	printf("初始化跨服战场禁用物品列表\t");
	while(in)
	{
		in >> str;
		_forbidden_item_set.insert(atoi(str.c_str()));
			
	}
	printf("物品数量:  %d\n", _forbidden_item_set.size());
	in.close();

	return true;
}

bool cs_world_manager::IsForbiddenItem(int item_type)
{
	return (_forbidden_item_set.find(item_type) != _forbidden_item_set.end() );
}

bool cs_world_manager::IsLimitSkillInWorld(int skill_id) 
{
	switch( skill_id )
	{
		case 2978:
		case 2979:
		case 2982:
		case 2983:
		case 2980:
		case 2981:
		case 2984:
		case 2985:
		case 2986:
			{
				return true;
			}
			break;
	}

	return false;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
void ib_world_manager::OnDeliveryConnected( std::vector<instance_field_info>& info )
{
	__PRINTF( "收集剧情战场注册信息line_id=%d, battle_id=%d, battle_world_tag=%d, min_level=%d, max_level=%d\n",
		gmatrix::Instance()->GetServerIndex(), _battle_id, GetWorldTag(), _need_level_min, _need_level_max );
	GLog::log( GLOG_INFO, "收集剧情战场注册信息line_id=%d, battle_id=%d, battle_world_tag=%d, min_level=%d, max_level=%d\n",
		gmatrix::Instance()->GetServerIndex(), _battle_id, GetWorldTag(), _need_level_min, _need_level_max );
	instance_field_info temp;

	memset(&temp,0,sizeof(temp));

	player_template::GetInstanceFieldInfo(GetWorldTag(), temp);

	temp.battle_id = _battle_id;
	temp.world_tag = GetWorldTag();
	
	// 这里只收集信息,在外面统一发一个协议吧
	info.push_back( temp );
}


void ib_world_manager::OnPlayerLeave(gplayer* pPlayer, int faction, bool cond_kick)
{
	GNET::SendPlayerLeaveInstence(gmatrix::GetServerIndex(),_battle_id,pPlayer->ID.id,cond_kick);
}

void ib_world_manager::OnActiveSpawn(int id,bool active)
{
	bool need_update = false;
	if(active)
	{
		if(id == _win_ctrl_id)
		{
			_battle_result = BATTLE_ATTACKER_WIN; 
		}
		else
		{
			int cur_time = g_timer.get_systime();
			for(size_t i = 0; i < _level_info.size(); ++i)
			{
				if(id == _level_info[i].open_ctrl_id && LEVEL_INACTIVE == _level_info[i].status )
				{
					_cur_level = i+1;
					_level_info[i].status= LEVEL_RUNNING;
					_level_info[i].start_timestamp = cur_time;
					need_update = true;

				}
				else if(id == _level_info[i].finish_ctrl_id && LEVEL_RUNNING ==  _level_info[i].status)
				{
					_cur_level = 0;
					_level_info[i].status= LEVEL_COMPLETE;
					_level_info[i].close_timestamp = cur_time;
					
					_level_score += _level_info[i].score;
					int spend_time =  _level_info[i].close_timestamp - _level_info[i].start_timestamp;
					if(spend_time < _level_info[i].time_limit)
					{
						int award_score = (_level_info[i].time_limit - spend_time) * _level_info[i].score_per_sec;
						_level_time_score += award_score; 
					}
					need_update = true;
				}

			}
		}
	}

	if(need_update)
	{
		SendBattleInfo();
		OnBattleInfoChange();
	}

	bg_world_manager::OnActiveSpawn(id,active);
}

void ib_world_manager::OnBattleInfoChange()
{
	PlayerInfoIt iter = _attacker_data.player_info_map.begin();
	for(; iter != _attacker_data.player_info_map.end(); ++iter)
	{
		MSG msg;
		BuildMessage(msg, GM_MSG_BATTLE_INFO_CHANGE, XID(GM_TYPE_PLAYER, iter->first),
				XID(-1,-1),A3DVECTOR(0,0,0), _battle_info_seq);
		gmatrix::SendMessage(msg);
	} 
}

bool ib_world_manager::GetInstanceBattleInfo(int& info_seq, int& cur_level, int& level_score, int& level_time_score,
			int& monster_score, int& death_penalty_score, std::vector<char> & level_info, bool bForce)
{
	if( _battle_info_seq != info_seq || bForce )
	{
		spin_autolock keeper( _battle_lock );
		cur_level = _cur_level;
		level_score = _level_score;
		level_time_score = _level_time_score;
		monster_score = _monster_score;
		death_penalty_score = _death_penalty_score;
			
		for(size_t i = 0; i < _level_info.size(); ++i)
		{
			level_info.push_back((char)(_level_info[i].status));
		}
		return true;
	}
	return false;
}

int ib_world_manager::GetBattleTotalScore()
{
	return _level_score + _level_time_score + _monster_score + _death_penalty_score;
}

void ib_world_manager::UpdateBattleStatus(int status)
{
	GNET::SendBattleStatus(gmatrix::GetServerIndex(),_battle_id,GetWorldTag(),status);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//挑战空间
bool ch_world_manager::OnInit()
{
	if(_defender_data.buildings.key_building_init <= 0)
	{
		return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//跨服小队PK
void 
cs_team_world_manager::OnCreate()
{
	_history_round_score.clear();

	set_cur_status(WAITING, g_timer.get_systime() + STATE_WAITING_TIME);

	_match_info.set_round_start_time(0);
	_match_info.set_is_round_end(false);
	_match_info.set_cur_round(1);
	_match_info.clear_game_result();
	_match_info.set_end_time( get_cur_endtime() + STATE_PREPARING_TIME );
	_match_info.clear_best_killer();

	ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
}


void 
cs_team_world_manager::OnClose()
{
	for(int i = 0; i <= ROUND_COUNT; i++)
	{
		ActiveSpawn( this, _cs_team_ctrl[i], false );
	}
}


void 
cs_team_world_manager::OnReady()
{
	ClearScore();
	_history_round_score.clear();

	set_cur_status(UNKNOW, 0);

	_match_info.set_round_start_time(0);
	_match_info.set_is_round_end(false);
	_match_info.set_end_time(0);
	_match_info.set_cur_round(0);
	_match_info.clear_best_killer();
}


cs_team_world_manager::cs_team_world_manager()
{
	_cs_team_lock = 0;
	set_cur_status(UNKNOW, 0);

	_match_info.set_round_start_time(0);
	_match_info.set_cur_round(0);
	_match_info.clear_game_result();

	_cs_team_ctrl[0] = CTRL_CONDISION_ID_BATTLE_BEGIN;
	for(int i = 1; i <= ROUND_COUNT; i++)
	{
		_cs_team_ctrl[i] = CTRL_CONDISION_ID_BATTLE_TEAM_CRSSVR_BEGIN + i - 1;
	}
	//ASSERT(CTRL_CONDISION_ID_BATTLE_TEAM_CRSSVR_END == _cs_team_ctrl[ROUND_COUNT]);

}


cs_team_world_manager::~cs_team_world_manager()
{
	set_cur_status(UNKNOW, 0);

	_match_info.set_round_start_time(0);
	_match_info.set_cur_round(0);
	_match_info.clear_game_result();
}


void 
cs_team_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{
	__PRINTF( "int cs_team_world_manager::PlayerEnter, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	AddMapNode( _all_list, pPlayer );
	player_battle_info tempinfo;
	memset( &tempinfo, 0, sizeof(player_battle_info) );
	tempinfo.battle_faction = faction;	

	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	tempinfo.battle_score = pImp->GetBattleScore();

	if( faction == BF_ATTACKER )
	{
		PlayerInfoIt iter = _attacker_data.player_info_map.find(pPlayer->ID.id);
		//已经存在
		if(iter != _attacker_data.player_info_map.end())
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER, pPlayer->ID, XID(-1,-1), A3DVECTOR(0,0,0), iter->second.kill, iter->second.death);
			gmatrix::SendMessage(msg);
		}
		else
		{
			_attacker_data.player_info_map[pPlayer->ID.id] = tempinfo;
		}
		AddMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		PlayerInfoIt iter = _defender_data.player_info_map.find(pPlayer->ID.id);
		//已经存在
		if(iter != _defender_data.player_info_map.end())
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER, pPlayer->ID, XID(-1,-1), A3DVECTOR(0,0,0), iter->second.kill, iter->second.death);
			gmatrix::SendMessage(msg);
		}
		else
		{
			_defender_data.player_info_map[pPlayer->ID.id] = tempinfo;
		}
		AddMapNode( _defender_list, pPlayer );
	}

	SendBattleInfo();
}

void 
cs_team_world_manager::PlayerLeave( gplayer* pPlayer, int faction, bool cond_kick, bool disconnected)
{
	__PRINTF( "int cs_team_world_manager::PlayerLeave, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	DelMapNode( _all_list, pPlayer );

	if( faction == BF_ATTACKER )
	{
		DelMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		DelMapNode( _defender_list, pPlayer );
	}

	OnPlayerLeave(pPlayer, faction, cond_kick);
	SendBattleInfo();
}


bool 
cs_team_world_manager::GetRebornPos(gplayer_imp* pImp, A3DVECTOR& pos, int& tag)
{
	//获得随机复活点
	int battle_faction = pImp->GetBattleFaction();
	spin_autolock keeper(_battle_lock);
	std::vector<A3DVECTOR> revive_pos_list;
	competitor_data::CtrlReviveMapIt it;

	if(BF_ATTACKER == battle_faction)
	{
		for(it = _attacker_data.revive_map.begin();it != _attacker_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{
				//诛仙的战场里复活点控制器不用了,真晕-_-b
				revive_pos_list.push_back(it->second[i].pos);
			}
		}
	}
	else if(BF_DEFENDER == battle_faction)
	{
		for(it = _defender_data.revive_map.begin();it != _defender_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{       
				revive_pos_list.push_back(it->second[i].pos);
			}               
		}
	}
	else if(BF_MELEE == battle_faction) 
	{
		for(it = _melee_data.revive_map.begin();it != _melee_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{       
				revive_pos_list.push_back(it->second[i].pos);
			}               
		}
	}
	else
	{
		return false;
	}

	size_t size = revive_pos_list.size();
	if(0 == size) return false;
	pos = revive_pos_list[abase::Rand(0,revive_pos_list.size() - 1)];
	tag = GetWorldTag();

	return true;
}


bool 
cs_team_world_manager::GetTownPosition(gplayer_imp* pImp, const A3DVECTOR& opos, A3DVECTOR &pos, int& world_tag)
{
	if( pImp->IsDead() )
	{
		if( GetRebornPos(pImp, pos, world_tag) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if( !pImp->IsInBattleCombatZone() || !IsRoundRunning() )
	{
		return false;
	}

	int battle_faction = pImp->GetBattleFaction();
	pos = A3DVECTOR(0,0,0);
	spin_autolock keeper(_battle_lock);

	if(BF_ATTACKER == battle_faction)
	{       
		if(0 == _attacker_data.town_list.size()) return false;
		pos = _attacker_data.town_list[abase::Rand(0,_attacker_data.town_list.size() - 1)];
	}
	else if(BF_DEFENDER == battle_faction)
	{       
		if(0 == _defender_data.town_list.size()) return false;
		pos = _defender_data.town_list[abase::Rand(0,_defender_data.town_list.size() - 1)];
	}
	else //gm和旁观者用这个
	{
		if(0 == _defender_data.town_list.size()) return false;
		pos = _defender_data.town_list[abase::Rand(0,_defender_data.town_list.size() - 1)];
	}

	world_tag = GetWorldTag();

	return true;
}


bool 
cs_team_world_manager::GetCommonBattleInfo( int& info_seq, unsigned char& attacker_building_left, 
		unsigned char& defender_building_left, std::vector<player_info_battle>& info, bool bForce )
{
	spin_autolock keeper( _battle_lock );
	if( _battle_info_seq != info_seq || bForce )
	{
		info.clear();
		info_seq = _battle_info_seq;

		attacker_building_left = 0; 
		defender_building_left = 0; 
		
		S2C::CMD::battle_info::player_info_in_battle temp;
		PlayerInfoIt it;
		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.battle_score = it->second.battle_score;
			temp.kill = it->second.kill;
			temp.death= it->second.death;
			info.push_back( temp );
		}
		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.battle_score = it->second.battle_score;
			temp.kill = it->second.kill;
			temp.death= it->second.death;
			info.push_back( temp );
		}
		return true;
	}
	return false;
}


void
cs_team_world_manager::UpdateRoundScore(int battle_faction, int change)
{
	spin_autolock keeper(_cs_team_lock);

	if( IsRoundRunning() )
	{
		if(BF_ATTACKER == battle_faction)
		{
			_attacker_score += change;
			if(_attacker_score <= 0)
			{
				_match_info.set_is_round_end(true);
			}
		}
		else if(BF_DEFENDER ==  battle_faction)
		{
			_defender_score += change;
			if(_defender_score <= 0)
			{
				_match_info.set_is_round_end(true);
			}
		}

		SendBattleInfo();
	}

}


bool 
cs_team_world_manager::GetCrossServerTeamBattleInfo(int& info_seq, S2C::CMD::crossserver_team_battle_info & info, std::vector<history_round_score>& history_info, bool bForce)
{
	if( _battle_info_seq != info_seq || bForce )
	{
		spin_autolock keeper(_cs_team_lock);
		info_seq = _battle_info_seq;
		history_info.clear();

		if(RUNNING == _cur_round_status)
		{
			info.battle_state = ROUND_IS_RUNNING;
		}
		else
		{
			info.battle_state = ROUND_IS_WAITING;
		}

		info.cur_state_end_time = _match_info.state_end_time;
		info.best_killer_id     = _match_info.best_killer_id;
		info.attacker_score     = _attacker_score;
		info.defender_score		= _defender_score;

		history_round_score temp;
		for(size_t i = 0; i < _history_round_score.size(); i++)
		{
			temp.attacker_score = _history_round_score[i].attacker_score;
			temp.defender_score = _history_round_score[i].defender_score;

			history_info.push_back(temp);
		}

		return true;
	}

	return false;
}


void 
cs_team_world_manager::ClearScore()
{
	_attacker_score = 0;
	_defender_score = 0;
}

int
cs_team_world_manager::CalculateRoundResult()
{
	history_round_score temp;
	temp.attacker_score = _attacker_score;
	temp.defender_score = _defender_score;
	_history_round_score.push_back(temp);

	int best_killer_id = -1;
	int tempkill  = 0;
	int tempdeath = -1;

	PlayerInfoIt it;
	if(_attacker_score > _defender_score)
	{
		_match_info.inc_att_res();

		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			if( tempkill < it->second.kill )
			{
				tempkill = it->second.kill;
				best_killer_id = it->first;
			}
		}
	}
	else if( _attacker_score < _defender_score)
	{
		_match_info.inc_def_res();

		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			if( tempkill < it->second.kill )
			{
				tempkill = it->second.kill;
				best_killer_id = it->first;
			}
		}
	}
	else if( _attacker_score == _defender_score)
	{
		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			if( tempkill < it->second.kill )
			{
				tempkill = it->second.kill;
				best_killer_id = it->first;
				tempdeath = it->second.death;
			}
		}

		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			if( tempkill < it->second.kill )
			{
				tempkill = it->second.kill;
				best_killer_id = it->first;
				tempdeath = it->second.death;
			}
			else if( tempkill == it->second.kill )
			{
				if( tempdeath > it->second.death )
				{
					tempkill = it->second.kill;
					best_killer_id = it->first;
					tempdeath = it->second.death;
				}
			}
		}
	}

	ClearScore();
	SendBattleInfo();

	return best_killer_id;
}


void 
cs_team_world_manager::SendRoundStartMessage()
{
	int counter = 0;
	for(cs_user_map::iterator it = _attacker_list.begin(); it != _attacker_list.end(); ++it)
	{
		const cs_user_list & list = it->second;
		for(size_t i = 0; i < list.size(); ++i)
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_ROUND_START_IN_BATTLE, XID(GM_TYPE_PLAYER, list[i].first),
					XID(-1,-1),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg);
			counter++;
		}
	}
	//ASSERT( counter <= _max_player_count );
	if( counter > _max_player_count )
	{
		GLog::log(GLOG_ERR, "跨服小队PK红方人数超过最大人数上限，counter1=%d", counter);
	}
	

	counter = 0;
	for(cs_user_map::iterator it = _defender_list.begin(); it != _defender_list.end(); ++it)
	{
		const cs_user_list & list = it->second;
		for(size_t i = 0; i < list.size(); ++i)
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_ROUND_START_IN_BATTLE, XID(GM_TYPE_PLAYER, list[i].first),
					XID(-1,-1),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg);
			counter++;
		}
	}
	//ASSERT( counter <= _max_player_count );
	if( counter > _max_player_count )
	{
		GLog::log(GLOG_ERR, "跨服小队PK蓝方人数超过最大人数上限，counter2=%d", counter);
	}
}


void 
cs_team_world_manager::SendRoundEndMessage()
{
	int counter = 0;
	for(cs_user_map::iterator it = _attacker_list.begin(); it != _attacker_list.end(); ++it)
	{
		const cs_user_list & list = it->second;
		for(size_t i = 0; i < list.size(); ++i)
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_ROUND_END_IN_BATTLE, XID(GM_TYPE_PLAYER, list[i].first),
					XID(-1,-1),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg);
			counter++;
		}
	}
	//ASSERT( counter <= _max_player_count );

	counter = 0;
	for(cs_user_map::iterator it = _defender_list.begin(); it != _defender_list.end(); ++it)
	{
		const cs_user_list & list = it->second;
		for(size_t i = 0; i < list.size(); ++i)
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_ROUND_END_IN_BATTLE, XID(GM_TYPE_PLAYER, list[i].first),
					XID(-1,-1),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg);
			counter++;
		}
	}
	//ASSERT( counter <= _max_player_count );
}

int 
cs_team_world_manager::GetAttackerScore()
{
	return _match_info.get_att_res();
}

int 
cs_team_world_manager::GetDefenderScore()
{
	return _match_info.get_def_res();
}


bool
cs_team_world_manager::CheckBattleResult()
{
	if(_match_info.cur_round > ROUND_COUNT)
	{
		if( _match_info.is_att_greater() )
		{
			_battle_result = BATTLE_ATTACKER_WIN;
		}
		else if( _match_info.is_def_greater() )
		{
			_battle_result = BATTLE_DEFENDER_WIN;
		}
		else if( _match_info.is_att_def_equal() )
		{
			_battle_result = BATTLE_DRAW;
		}
		
		SendRoundEndMessage();
		_match_info.set_end_time(0);

		return true;
	}

	if( (ROUND_COUNT - _match_info.cur_round + 1) < (abs(_match_info.result_attacker - _match_info.result_defender)) )
	{
		if( _match_info.is_att_greater() )
		{
			_battle_result = BATTLE_ATTACKER_WIN;
		}
		else if( _match_info.is_def_greater() )
		{
			_battle_result = BATTLE_DEFENDER_WIN;
		}
		else if( _match_info.is_att_def_equal() )
		{
			_battle_result = BATTLE_DRAW;
		}

		SendRoundEndMessage();
		_match_info.set_end_time(0);

		return true;
	}

	return false;
}


bool 
cs_team_world_manager::IsLimitSkillInWorld(int skill_id) 
{
	switch( skill_id )
	{
		case RESURRECT_SKILL_1:
		case RESURRECT_SKILL_2:
			{
				return true;
			}
			break;

		case 2978:
		case 2979:
		case 2982:
		case 2983:
		case 2980:
		case 2981:
		case 2984:
		case 2985:
		case 2986:
			{
				return true;
			}
			break;
	}

	return false;
}


bool
cs_team_world_manager::OnInit()
{
	if( (_attacker_data.town_list.size() == 0  || _defender_data.town_list.size() == 0) 
	 || (_attacker_data.revive_map.size() == 0 || _defender_data.revive_map.size() == 0) 
	 || (_attacker_data.entry_list.size() == 0 || _defender_data.entry_list.size() == 0) )
	{
		return false;
	}

	InitiForbiddenItemList();

	return true;
}


bool 
cs_team_world_manager::InitiForbiddenItemList()
{
	_forbidden_item_set.clear();

	ONET::Conf *conf = ONET::Conf::GetInstance();
	std::string root = conf->find("Template","Root");
	std::string path = root + conf->find("Template", "CSTeamPKForbiddenItemList");

	std::ifstream in( path.c_str() );
	if(!in) return false;
	std::string str;
	printf("初始化跨服小队6v6战场禁用物品列表\t");
	while(in)
	{
		in >> str;
		_forbidden_item_set.insert(atoi(str.c_str()));
			
	}
	printf("物品数量:  %d\n", _forbidden_item_set.size());
	in.close();

	return true;
}


bool
cs_team_world_manager::IsForbiddenItem(int item_type)
{
	return (_forbidden_item_set.find(item_type) != _forbidden_item_set.end() );
}


void
cs_team_world_manager::OnRunning()
{
	spin_autolock keeper(_cs_team_lock);
	if( CheckBattleResult() ) return;

	switch(_cur_round_status)
	{
		case WAITING:
			{
				if( get_cur_endtime() <= g_timer.get_systime() )
				{
					__PRINTF("-----------------------WAITING end!\n\n\n\n");
					set_cur_status(PREPARING, g_timer.get_systime() + STATE_PREPARING_TIME);
				}
			}
			break;

		case PREPARING:
			{
				if( get_cur_endtime() <= g_timer.get_systime() )
				{
					__PRINTF("-----------------------PREPARING end!\n\n\n\n");
					int curtime = g_timer.get_systime();
					set_cur_status(RUNNING, curtime + STATE_RUNNING_TIME);
					
					ActiveSpawn( this, _cs_team_ctrl[_match_info.cur_round], true );

					_match_info.set_round_start_time( curtime );
					_match_info.set_end_time( get_cur_endtime() );

					SendRoundStartMessage();
					SendBattleInfo();
				}
			}
			break;

		case RUNNING:
			{
				if( (get_cur_endtime() <= g_timer.get_systime() ) || _match_info.is_round_end)
				{
					__PRINTF("-----------------------RUNNING end!\n\n\n\n");
					int best_killer_id = CalculateRoundResult();
					set_cur_status(CLOSING, g_timer.get_systime() + STATE_CLOSING_TIME);

					_match_info.set_best_killer(best_killer_id);
					_match_info.set_is_round_end(false);
					_match_info.set_end_time( get_cur_endtime() + STATE_WAITING_TIME + STATE_PREPARING_TIME);
					_match_info.inc_cur_round();

					SendRoundEndMessage();
					SendBattleInfo();

					ActiveSpawn( this, _cs_team_ctrl[_match_info.cur_round], false );
				}
			}
			break;

		case CLOSING:
			{
				if( get_cur_endtime() <= g_timer.get_systime() )
				{
					__PRINTF("-----------------------CLOSING end!\n\n\n\n");
					set_cur_status(WAITING, g_timer.get_systime() + STATE_WAITING_TIME);

					_match_info.clear_best_killer();

					ClearScore();
					KillAllMonsters();
				}
			}
			break;

		default:
			{
				__PRINTF( "cs_team_world_manager ERROR! _status=%d\n", _cur_round_status );
				ASSERT( false );
			}
			break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//跨服流水席战场逻辑

bool cs_flow_world_manager::OnInit()
{
	DATA_TYPE dt;
	BATTLE_201304_CONFIG* config = (BATTLE_201304_CONFIG*)gmatrix::GetDataMan().get_data_ptr(_bg_template_id, ID_SPACE_CONFIG, dt);
	if(!config || dt != DT_BATTLE_201304_CONFIG)
	{
		__PRINTF("Failed to load BATTLE_201304_CONFIG tid=%d\n", _bg_template_id);
		return false;
	}
	if(_attacker_data.buildings.normal_building_init > 0 || _defender_data.buildings.normal_building_init > 0 
	|| _attacker_data.buildings.key_building_init > 0 ||  _defender_data.buildings.key_building_init  > 0)
	{
		return false;
	}
	
	for(size_t i = 0; i < sizeof(config->monster_info)/sizeof(config->monster_info[0]); i ++)
	{
		_monster_score_map[config->monster_info[i].id] = config->monster_info[i].score;
	}
	_kill_base_score = config->score;
	_target_score = config->score_target;
	_is_delay = config->add_time_if_full ? 1 : 0;
	_delay_time = config->minite_add * 60;
	
	InitiForbiddenItemList();
	return true;
}

void cs_flow_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{
	__PRINTF( "int cs_flow_world_manager::PlayerEnter, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _battle_lock );
	AddMapNode( _all_list, pPlayer );
	player_battle_info tempinfo;
	memset( &tempinfo, 0, sizeof(player_battle_info) );
	tempinfo.battle_faction = faction;	

	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	tempinfo.battle_score = pImp->GetBattleScore();
	if( faction == BF_ATTACKER )
	{
		PlayerInfoIt iter = _attacker_data.player_info_map.find(pPlayer->ID.id);
		//已经存在
		if(iter != _attacker_data.player_info_map.end())
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER, pPlayer->ID, XID(-1,-1), A3DVECTOR(0,0,0), iter->second.kill, iter->second.death);
			gmatrix::SendMessage(msg);
		}
		else
		{
			_attacker_data.player_info_map[pPlayer->ID.id] = tempinfo;
		}
		AddMapNode( _attacker_list, pPlayer );

		flow_player_map::iterator it = _atk_fp_map.find(pPlayer->ID.id);
		if(it == _atk_fp_map.end())
		{
			flow_player_info info;
			info.level = pImp->_basic.level; 
			info.cls = pPlayer->GetClass();
			info.zoneId = pPlayer->id_zone;
			_atk_fp_map[pPlayer->ID.id] = info;
		}
	}
	else if( faction == BF_DEFENDER )
	{
		PlayerInfoIt iter = _defender_data.player_info_map.find(pPlayer->ID.id);
		//已经存在
		if(iter != _defender_data.player_info_map.end())
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER, pPlayer->ID, XID(-1,-1), A3DVECTOR(0,0,0), iter->second.kill, iter->second.death);
			gmatrix::SendMessage(msg);
		}
		else
		{
			_defender_data.player_info_map[pPlayer->ID.id] = tempinfo;
		}
		AddMapNode( _defender_list, pPlayer );

		flow_player_map::iterator it = _def_fp_map.find(pPlayer->ID.id);
		if(it == _def_fp_map.end())
		{
			flow_player_info info;
			info.level = pImp->_basic.level; 
			info.cls = pPlayer->GetClass();
			info.zoneId = pPlayer->id_zone;
			_def_fp_map[pPlayer->ID.id] = info;
		}
	}
	SendBattleInfo();
}

void cs_flow_world_manager::PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick, bool disconnected)
{
	__PRINTF( "int cs_flow_world_manager::PlayerLeave, pPlayer=%p, faction=%d, disconnected=%s\n", pPlayer, faction, disconnected ? "true" : "false");
	DelMapNode( _all_list, pPlayer );
	if( faction == BF_ATTACKER )
	{
		DelMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		DelMapNode( _defender_list, pPlayer );
	}

	//不是断线情况的都进行清理
	if(!disconnected)
	{
		bg_world_manager::PlayerLeave(pPlayer, faction, cond_kick, disconnected);
	}
}


void cs_flow_world_manager::OnPlayerLeave( gplayer* pPlayer, int faction, bool cond_kick)
{
	bg_world_manager::OnPlayerLeave(pPlayer, faction, cond_kick);
	flow_player_map::iterator it;	
	if( faction == BF_ATTACKER )
	{
		it = _atk_fp_map.find( pPlayer->ID.id );
		if( it != _atk_fp_map.end() )
		{
			_atk_fp_map.erase( it );
		}
	}
	else if( faction == BF_DEFENDER )
	{
		it = _def_fp_map.find( pPlayer->ID.id );
		if( it != _def_fp_map.end() )
		{
			_def_fp_map.erase( it );
		}
	}
}

void cs_flow_world_manager::FlowBattlePlayerLeave(int role_id, char faction)
{
	spin_autolock keeper( _battle_lock );
	PlayerInfoIt it;
	if( faction == BF_ATTACKER )
	{
		it = _attacker_data.player_info_map.find( role_id);
		if( it != _attacker_data.player_info_map.end() )
		{
			_attacker_data.player_info_map.erase( it );
		}
	}
	else if( faction == BF_DEFENDER )
	{
		it = _defender_data.player_info_map.find( role_id );
		if( it != _defender_data.player_info_map.end() )
		{
			_defender_data.player_info_map.erase( it );
		}
	}
	else if( faction == BF_MELEE )
	{
		it = _melee_data.player_info_map.find(role_id);
		if(it != _melee_data.player_info_map.end())
		{
			_melee_data.player_info_map.erase(it);
		}
	}
	flow_player_map::iterator it1;	
	if( faction == BF_ATTACKER )
	{
		it1 = _atk_fp_map.find( role_id );
		if( it1 != _atk_fp_map.end() )
		{
			_atk_fp_map.erase( it1 );
		}
	}
	else if( faction == BF_DEFENDER )
	{
		it1 = _def_fp_map.find( role_id );
		if( it1 != _def_fp_map.end() )
		{
			_def_fp_map.erase( it1 );
		}
	}
	SendBattleInfo();
}

bool cs_flow_world_manager::InitiForbiddenItemList()
{
	_forbidden_item_set.clear();

	ONET::Conf *conf = ONET::Conf::GetInstance();
	std::string root = conf->find("Template","Root");
	std::string path = root + conf->find("Template", "CSTeamPKForbiddenItemList");

	std::ifstream in( path.c_str() );
	if(!in) return false;
	std::string str;
	printf("初始化跨服战场禁用物品列表\t");
	while(in)
	{
		in >> str;
		_forbidden_item_set.insert(atoi(str.c_str()));
			
	}
	printf("物品数量:  %d\n", _forbidden_item_set.size());
	in.close();

	return true;
}

bool cs_flow_world_manager::IsForbiddenItem(int item_type)
{
	return (_forbidden_item_set.find(item_type) != _forbidden_item_set.end() );
}

bool cs_flow_world_manager::IsLimitSkillInWorld(int skill_id) 
{
	switch( skill_id )
	{
		case 2978:
		case 2979:
		case 2982:
		case 2983:
		case 2980:
		case 2981:
		case 2984:
		case 2985:
		case 2986:
			{
				return true;
			}
			break;
	}

	return false;
}

void cs_flow_world_manager::SyncCSFlowPlayerInfo( int player_id, size_t content_length, const void* content)
{
	spin_autolock keeper( _battle_lock );
	if(content_length != sizeof(msg_sync_flow_info))
	{
		return;
	}
	msg_sync_flow_info& c = *(msg_sync_flow_info*)content;
	flow_player_map::iterator it = _atk_fp_map.find(player_id);
	if(it != _atk_fp_map.end())
	{
		flow_player_info& info = it->second;
		info.score = c.score;
		info.c_kill_cnt = c.c_kill_cnt;
		info.monster_kill_cnt = c.monster_kill_cnt;
		info.max_ckill_cnt = c.max_ckill_cnt;
		info.max_mkill_cnt = c.max_mkill_cnt;
		info.battle_result_sent = c.battle_result_sent;
	}
	else
	{
		it = _def_fp_map.find(player_id);
		if(it != _def_fp_map.end())
		{
			flow_player_info& info = it->second;
			info.score = c.score;
			info.c_kill_cnt = c.c_kill_cnt;
			info.monster_kill_cnt = c.monster_kill_cnt;
			info.max_ckill_cnt = c.max_ckill_cnt;
			info.max_mkill_cnt = c.max_mkill_cnt;
			info.battle_result_sent = c.battle_result_sent;
		}
	}
	PlayerInfoIt it1;
	it1 = _attacker_data.player_info_map.find( player_id );
	if( it1 != _attacker_data.player_info_map.end() )
	{       
		++ _defender_data.kill_count;
		it1->second.kill = c.kill_cnt;
		it1->second.death = c.death_cnt;
	}  
	else
	{
		it1 = _defender_data.player_info_map.find( player_id );
		if( it1 != _defender_data.player_info_map.end() )
		{       
			++_attacker_data.kill_count;
			it1->second.kill = c.kill_cnt;
			it1->second.death = c.death_cnt;
		}
		else
		{
			it1 = _melee_data.player_info_map.find( player_id );
			if( it1 != _melee_data.player_info_map.end() )
			{       
				++_melee_data.kill_count;
				it1->second.kill = c.kill_cnt;
				it1->second.death = c.death_cnt;
			}
		}
	}
	SendBattleInfo();
}

bool cs_flow_world_manager::GetPlayerCSFlowInfo(int player_id, int& score, int& c_kill_cnt, int &death_cnt, int& kill_cnt, int& monster_kill_cnt, 
		int& max_ckill_cnt, int& max_mkill_cnt, bool & battle_result_sent)
{
	spin_autolock keeper( _battle_lock );
	
	PlayerInfoIt it1;
	it1 = _attacker_data.player_info_map.find( player_id );
	if( it1 != _attacker_data.player_info_map.end() )
	{       
		kill_cnt = it1->second.kill;
		death_cnt = it1->second.death;
	}  
	else
	{
		it1 = _defender_data.player_info_map.find( player_id );
		if( it1 != _defender_data.player_info_map.end() )
		{       
			kill_cnt = it1->second.kill;
			death_cnt = it1->second.death;
		}
		else
		{
			it1 = _melee_data.player_info_map.find( player_id );
			if( it1 != _melee_data.player_info_map.end() )
			{       
				kill_cnt = it1->second.kill;
				death_cnt = it1->second.death;
			}
		}
	}
	
	flow_player_map::iterator it = _atk_fp_map.find(player_id);
	if(it != _atk_fp_map.end())
	{
		flow_player_info& info = it->second;
		score = info.score;
		c_kill_cnt = info.c_kill_cnt; 
		monster_kill_cnt = info.monster_kill_cnt;
		max_ckill_cnt = info.max_ckill_cnt;
		max_mkill_cnt = info.max_mkill_cnt;
		battle_result_sent = info.battle_result_sent;
		return true;
	}
	else
	{
		it = _def_fp_map.find(player_id);
		if(it != _def_fp_map.end())
		{
			flow_player_info& info = it->second;
			score = info.score;
			c_kill_cnt = info.c_kill_cnt;
			monster_kill_cnt = info.monster_kill_cnt;
			max_ckill_cnt = info.max_ckill_cnt;
			max_mkill_cnt = info.max_mkill_cnt;
			battle_result_sent = info.battle_result_sent;
			return true;
		}
	}
	return false;
}

bool cs_flow_world_manager::GetCSFlowBattleInfo(int& info_seq, std::vector<player_info_flow_battle>& info, bool bForce)
{
	spin_autolock keeper( _battle_lock );
	if( _battle_info_seq != info_seq || bForce )
	{
		info.clear();
		info_seq = _battle_info_seq;
	
		S2C::CMD::flow_battle_info::player_info_in_flow_battle temp;
		PlayerInfoIt it;
		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.kill = it->second.kill;
			temp.death= it->second.death;	

			flow_player_map::iterator fit = _atk_fp_map.find(temp.id);
			if(fit != _atk_fp_map.end())
			{
				temp.level = fit->second.level;
				temp.cls = fit->second.cls;
				temp.zone_id = fit->second.zoneId;
				temp.flow_score = fit->second.c_kill_cnt;
			}
			
			info.push_back( temp );
		}
		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.battle_faction = it->second.battle_faction;
			temp.kill = it->second.kill;
			temp.death= it->second.death;

			flow_player_map::iterator fit = _def_fp_map.find(temp.id);
			if(fit != _def_fp_map.end())
			{
				temp.level = fit->second.level;
				temp.cls = fit->second.cls;
				temp.zone_id = fit->second.zoneId;
				temp.flow_score = fit->second.c_kill_cnt;
			}
			info.push_back( temp );
		}
		return true;
	}
	return false;
}
