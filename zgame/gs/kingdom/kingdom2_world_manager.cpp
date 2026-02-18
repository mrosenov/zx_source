#include "../player_imp.h"
#include "kingdom2_world_manager.h"
#include "kingdom2_world_message_handler.h"
#include <conf.h>
#include <factionlib.h>

int kingdom2_world_manager::Init( const char* gmconf_file, const char* servername, int tag, int index)
{
	int rst = global_world_manager::Init( gmconf_file, servername, tag, index);
	if(rst != 0) return rst;

	if(_message_handler != NULL) delete _message_handler;
	_message_handler = new kingdom2_world_message_handler( this, &_plane );
	
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	section += servername;

	_kickout_time_min = atoi( conf->find( section, "battle_kickout_time_min" ).c_str() );
	if( _kickout_time_min <= 0 )
	{
		_kickout_time_min = DEFAULT_KICKOUT_TIME_MIN;
	}

	_kickout_time_max = atoi( conf->find( section, "battle_kickout_time_max" ).c_str() );
	if( _kickout_time_max <= 0 )
	{
		_kickout_time_max = DEFAULT_KICKOUT_TIME_MAX;
	}

	_kickout_close_time = atoi( conf->find( section, "battle_kickout_close_time" ).c_str() );
	if( _kickout_close_time <= 0 )
	{
		_kickout_close_time = DEFAULT_KICKOUT_CLOSE_TIME;
	}

	player_template::GetKingdomMonsterID(_attacker_npc_id, _defender_npc_id);
	
	OnInit();
	return 0;
}


void kingdom2_world_manager::OnDeliveryConnected( std::vector<kingdom_field_info>& info )
{
	kingdom_field_info kfi;
	kfi.type = 2;
	kfi.world_tag = GetWorldTag(); 
	info.push_back(kfi);
}


bool kingdom2_world_manager::CreateKingdom(char fieldtype, int tag_id, int defender, std::vector<int> attacker_list)
{
	ASSERT(fieldtype == 2 && tag_id == GetWorldTag());

	spin_autolock keeper( _battle_lock );
	GLog::log(GLOG_INFO, "收到国王战战场开启请求: type=%d, tag_id=%d, defender_id=%d",
			fieldtype, tag_id, defender);

	if(_battle_status != BS_READY)
	{
		GLog::log( GLOG_INFO, "国王战战场开启失败,原因：战场状态不为BS_READY, tag_id=%d, _status=%d", tag_id, _battle_status );
		return false;
	}

	_start_timestamp = g_timer.get_systime();
	_end_timestamp = _start_timestamp + 3800;
	_cur_timestamp = _end_timestamp;

	OnCreate();

	GLog::log( GLOG_INFO, "国王战战场开启成功: type=%d, tag_id=%d, defender_id=%d", fieldtype, tag_id, defender);
	return true;
	
}

int kingdom2_world_manager::OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm )
{
	if(_battle_status != BS_RUNNING && !is_gm)
	{
		GLog::log(GLOG_INFO, "国王战玩家无法进入战场,战场未开启:  roleid=%d, line_id=%d, world_tag=%d, battle_status=%d",
				pInfo->id, gmatrix::Instance()->GetServerIndex(),GetWorldTag(),_battle_status);
		return -1001;
	}

	return 0;
}

void kingdom2_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{

}

void kingdom2_world_manager::PlayerLeave( gplayer* pPlayer, int faction)
{
}

void kingdom2_world_manager::Heartbeat()
{
	global_world_manager::Heartbeat();
	++_heartbeat_counter; 
	if( (_heartbeat_counter) % 20 == 0 )
	{
		spin_autolock keeper( _battle_lock );
		switch ( _battle_status )
		{
			case BS_READY:
			{
				OnReady();
			}
			break;
			
			case BS_RUNNING:
			{
				OnRunning();
			}
			break;

			case BS_CLOSING:
			{
				OnClosing();
			}
			break;

			default:
			{
				ASSERT( false );
			}
		};
	}
}

void kingdom2_world_manager::OnInit()
{
	_battle_status = BS_READY;
}

void kingdom2_world_manager::OnReady()
{
}

void kingdom2_world_manager::OnCreate()
{
	_battle_status = BS_RUNNING;
}


void kingdom2_world_manager::OnRunning()
{
	// 战斗逻辑,如果时间到或者产生结果了
	if( _cur_timestamp < g_timer.get_systime())
	{
		Close();
		GNET::SendKingdomEnd(2, GetWorldTag(), 0);
	}
}	      

void kingdom2_world_manager::Close()
{
	_battle_status = BS_CLOSING;
	_cur_timestamp = g_timer.get_systime() + 120; //2分钟保证所有的玩家已经退出

	// 杀死所有怪物
	MSG msg;	
	BuildMessage(msg,GM_MSG_NPC_DISAPPEAR,XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
	std::vector<exclude_target> empty;
	_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);


	// 去掉所有矿物
	MSG msg2;	
	BuildMessage(msg2,GM_MSG_MINE_DISAPPEAR,XID(GM_TYPE_MATTER,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
	empty.clear();
	_plane.BroadcastSphereMessage(msg2,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);

	GLog::log( GLOG_INFO, "国王战结束开始清理,gs_id=%d, world_tag=%d, end_time=%d,",
			gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _end_timestamp );
}

void kingdom2_world_manager::OnClosing()
{
	// 这个期间踢出玩家
	// 在玩家heartbeat逻辑里完成规定时间内的踢人
	if( _cur_timestamp < g_timer.get_systime() )
	{
		GLog::log( GLOG_INFO, "国王战结束,gs_id=%d, world_tag=%d, end_time=%d,",
				gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _end_timestamp );

		// 在这之前需要确保非gm玩家已经被踢出
		_battle_status = BS_READY; 
		Reset();
	}
}

void kingdom2_world_manager::StopKingdom(char fieldtype, int tag_id)
{
	spin_autolock keeper( _battle_lock );
	if(_battle_status != BS_RUNNING) return;

	Close();
}


void kingdom2_world_manager::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_attacker_data.town_list.push_back(pos);
	}       
}

void kingdom2_world_manager::RecordRebornPos(const A3DVECTOR& pos,int faction,int ctrl_id)
{
	revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_attacker_data.revive_map[ctrl_id].push_back(temp);
	}
}


void kingdom2_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{       
	if(faction & FACTION_OFFENSE_FRIEND)
	{       
		_attacker_data.entry_list.push_back(pos);
	}       
}      


bool kingdom2_world_manager::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag)
{
	return false;
}       

void kingdom2_world_manager::GetRandomEntryPos(A3DVECTOR& pos,int battle_faction)
{
	pos = A3DVECTOR(0,0,0);
	if(0 == _attacker_data.entry_list.size()) return;
	pos = _attacker_data.entry_list[abase::Rand(0,_attacker_data.entry_list.size() - 1)];
} 

bool kingdom2_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	spin_autolock keeper(_battle_lock);

	std::vector<A3DVECTOR> revive_pos_list;
	CtrlReviveMapIt it;
	for(it = _attacker_data.revive_map.begin();it != _attacker_data.revive_map.end(); ++it)
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
	size_t size = revive_pos_list.size();
	if(0 == size) return false;
	pos = revive_pos_list[abase::Rand(0,revive_pos_list.size() - 1)];
	world_tag = GetWorldTag();
	return true;
}

int kingdom2_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	int type = 0;
	if(tid == _attacker_npc_id)
	{
		type = 1;
	}
	else if(tid == _defender_npc_id)
	{
		type = 2;
	}
	else
	{
		return 0;
	}

	MSG msg;	
	BuildMessage(msg,GM_MSG_KINGDOM_CHANGE_KEY_NPC,XID(GM_TYPE_MANAGER,KINGDOM_TAG_ID),XID(-1,-1),A3DVECTOR(0,0,0), type); 
	gmatrix::SendWorldMessage(KINGDOM_TAG_ID, msg);
	return 0; 
}


void kingdom2_world_manager::OnSyncKeyNPCInfo(int cur_hp, int max_hp, int hp_add, int hp_dec)
{
	_key_npc_info.cur_hp = cur_hp;
	_key_npc_info.max_hp = max_hp;
	_key_npc_info.hp_add = hp_add;
	_key_npc_info.hp_dec = hp_dec;
}

void kingdom2_world_manager::OnGetKeyNPCInfo(int & cur_hp, int & max_hp, int & hp_add, int & hp_dec)
{
	cur_hp = _key_npc_info.cur_hp;
	max_hp = _key_npc_info.max_hp;
	hp_add = _key_npc_info.hp_add;
	hp_dec = _key_npc_info.hp_dec;
}

