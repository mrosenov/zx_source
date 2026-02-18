#include "../player_imp.h"
#include "../npc.h"
#include <conf.h>
#include <factionlib.h>

#include "kingdom_world_manager.h"
#include "kingdom_world_message_handler.h"

int kingdom_world_manager::Init( const char* gmconf_file, const char* servername, int tag, int index)
{
	int rst = global_world_manager::Init( gmconf_file, servername, tag, index);
	if(rst != 0) return rst;
	
	if(_message_handler != NULL) delete _message_handler;
	_message_handler = new kingdom_world_message_handler( this, &_plane );
	
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
	
	InitRebornPos();
	OnInit();

	return 0;
}


//
// 初始化复活点坐标，这里采用硬编码, 第一个位置是防守方位置，后面四个为攻方位置
//
void kingdom_world_manager::InitRebornPos()
{
	int tid[] = {55835, 55836, 55845, 55846, 55847};
	A3DVECTOR pos[] = {A3DVECTOR(-134.972, 321.235, 138.034), A3DVECTOR(14.418, 301.2, -47.19), A3DVECTOR(-307.301, 302.036, -28.833), 
		A3DVECTOR(-290.061, 301.493, 349.857), A3DVECTOR(34.417, 301.862, 350.757)};
	bool is_attacker[] = {false, true, true, true, true};

	for(size_t i = 0; i < 5; ++i)
	{
		reborn_info info;
		memset(&info, 0, sizeof(info));
		
		//复活点的静态信息
		info.reborn_pos = pos[i];
		info.tid = tid[i];
		info.seq = i; 
		info.is_attacker = is_attacker[i];

		_reborn_info_list.push_back(info);
	}


}

void kingdom_world_manager::ResetRebornFaction()
{
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].is_attacker &&  _reborn_info_list[i].npc_id != 0)
		{
			int faction = 0;
			int enemy_faction = 0;
			GetFactionBySeq(_reborn_info_list[i].seq, faction, enemy_faction);

			MSG msg;	
			BuildMessage2(msg,GM_MSG_KINGDOM_CHANGE_NPC_FACTION,XID(GM_TYPE_NPC,_reborn_info_list[i].npc_id),XID(-1,-1),A3DVECTOR(0,0,0), faction, enemy_faction); 
			gmatrix::SendMessage(msg);
		}
	}
}

void kingdom_world_manager::OnDeliveryConnected( std::vector<kingdom_field_info>& info )
{
	kingdom_field_info kfi;
	kfi.type = 1;
	kfi.world_tag = GetWorldTag(); 
	info.push_back(kfi);
}


bool kingdom_world_manager::CreateKingdom(char fieldtype, int tag_id, int defender, std::vector<int> attacker_list)
{
	ASSERT(fieldtype == 1 && tag_id == GetWorldTag());
	GLog::log(GLOG_INFO, "收到国王战战场开启请求: type=%d, tag_id=%d, defender_id=%d",
			fieldtype, tag_id, defender);

	spin_autolock keeper( _battle_lock );
	if(_battle_status != BS_READY)
	{
		GLog::log( GLOG_INFO, "国王战战场开启失败,原因：战场状态不为BS_READY, tag_id=%d, _status=%d", tag_id, _battle_status );
		return false;
	}

	//随机攻击方复活点
	std::random_shuffle(_reborn_info_list.begin()+1, _reborn_info_list.end());

	kingdom_mafia_info info;
	if(defender > 0)
	{
		memset(&info, 0, sizeof(info));
		info.mafia_id = defender;
		info.is_attacker = false;
		if(!GetMafiaRebornInfo(info.mafia_id, info.is_attacker))
		{
		}

		_mafia_info_list.push_back(info);
	}

	for(size_t i = 0; i < attacker_list.size(); ++i)
	{
		int attacker_id = attacker_list[i];

		memset(&info, 0, sizeof(info));
		info.mafia_id = attacker_id;
		info.is_attacker = true;
		if(!GetMafiaRebornInfo(info.mafia_id, info.is_attacker))
		{
		}

		_mafia_info_list.push_back(info);
	}

	//首场对怪物
	if(defender == 0)
	{
		ActiveSpawn( this, OPEN_CTRL_ID1, true );
	}
	else
	{
		ActiveSpawn( this, OPEN_CTRL_ID2, true );
	}

	_start_timestamp = g_timer.get_systime();
	_end_timestamp = _start_timestamp + 3600;	
	_cur_timestamp = _end_timestamp;

	OnCreate();
	GLog::log( GLOG_INFO, "国王战战场开启成功: type=%d, tag_id=%d, defender_id=%d", fieldtype, tag_id, defender);
	__PRINTF("国王战战场开启成功: type=%d, tag_id=%d, defender_id=%d\n", fieldtype, tag_id, defender);
	return true;
	
}

void kingdom_world_manager::StopKingdom(char fieldtype, int tag_id)
{
	spin_autolock keeper( _battle_lock );
	if(_battle_status != BS_RUNNING && _battle_status != BS_RUNNING2) return;

	Close();
}

void kingdom_world_manager::PlayerEnter( gplayer* pPlayer, int mafia_id)
{
	spin_autolock keeper( _battle_lock );
	AddMapNode(_all_list, pPlayer);

	bool exist = false;
	for(size_t i = 0; i < _mafia_info_list.size(); ++i)
	{
		if(_mafia_info_list[i].mafia_id == mafia_id) 
		{
			for(size_t j = 0; j < _mafia_info_list[i].player_list.size(); ++j)
			{
				if(_mafia_info_list[i].player_list[j].roleid == pPlayer->ID.id)
				{
					_mafia_info_list[i].player_list[j].is_online = true;
					exist = true;
					break;
				}
			}

			if(!exist)
			{
				kingdom_player_info info;
				info.roleid = pPlayer->ID.id;
				info.cs_index = pPlayer->cs_index;
				info.cs_sid = pPlayer->cs_sid;
				info.mafia_id = mafia_id; 
				info.kill_count = 0;
				info.death_count = 0;
				info.is_online = true;
				_mafia_info_list[i].player_list.push_back(info);
			}
			break;
			
		}
	}

	SyncMafiaInfo();
	return;
}

void kingdom_world_manager::PlayerLeave( gplayer* pPlayer, int mafia_id)
{
	spin_autolock keeper( _battle_lock );
	DelMapNode(_all_list, pPlayer);
	
	int roleid = pPlayer->ID.id;
	for(size_t i = 0; i < _mafia_info_list.size(); ++i)
	{
		if(_mafia_info_list[i].mafia_id == mafia_id)
		{
			std::vector<kingdom_player_info>::iterator iter;
			for(iter = _mafia_info_list[i].player_list.begin(); iter != _mafia_info_list[i].player_list.end(); ++iter)
			{
				if(iter->roleid == roleid)
				{
					iter->is_online = false;
					 return;
				}
			}
		}
	}

	return;
}

int kingdom_world_manager::OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm )
{
	if((_battle_status != BS_RUNNING && _battle_status != BS_RUNNING2) && !is_gm)
	{
		GLog::log(GLOG_INFO, "国王战玩家无法进入战场,战场未开启:  roleid=%d, line_id=%d, world_tag=%d, battle_status=%d",
				pInfo->id, gmatrix::Instance()->GetServerIndex(),GetWorldTag(),_battle_status);
		return -1001;
	}

	if(!IsDefenderFaction((int)pInfo->factionid) && !IsAttackerFaction((int)pInfo->factionid))
	{
		GLog::log(GLOG_INFO, "国王战玩家无法进入战场,玩家不属于攻守方帮派:  roleid=%d, line_id=%d, world_tag=%d, battle_status=%d, mafia_id=%d",
				pInfo->id, gmatrix::Instance()->GetServerIndex(),GetWorldTag(),_battle_status, (int)pInfo->factionid);
		return -1002;
	}
	return 0;
}

void kingdom_world_manager::Heartbeat()
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

			case BS_RUNNING2:
			{
				OnRunning2();
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
			break;
		};
	}
}

void kingdom_world_manager::OnInit()
{
	_battle_status = BS_READY;
}

void kingdom_world_manager::OnReady()
{
}

void kingdom_world_manager::OnCreate()
{
	_battle_status = BS_RUNNING;
}


void kingdom_world_manager::OnRunning()
{
	// 战斗逻辑,如果时间到或者产生结果了
	if( _cur_timestamp < g_timer.get_systime() || 0 != _battle_result )
	{
		if(_battle_result == 0) 
		{
			_battle_result = BR_DEFENDER_WIN; 
			Close();
		}
		else if(_battle_result == BR_DEFENDER_WIN)
		{
			Close();
		}
		else if(_battle_result == BR_ATTACKER_WIN)
		{
			_battle_status = BS_RUNNING2;
			_cur_timestamp = g_timer.get_systime() + 3600;
			_end_timestamp = _cur_timestamp;
		}

		_half_timestamp = g_timer.get_systime();

		//通知所有玩家半场结果，在玩家的消息里面处理半场结束后的行为
		MSG msg;	
		BuildMessage2(msg,GM_MSG_KINGDOM_BATTLE_HALF,XID(GM_TYPE_PLAYER,-1),XID(-1,-1),A3DVECTOR(0,0,0), _battle_result, g_timer.get_systime());	
		std::vector<exclude_target> empty;
		_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);


		//重新设置复活怪的阵营
		ResetRebornFaction();


		//发生协议通知delivery, delivery将通知战场2关闭
		//这里需要获取攻方被摧毁的复活点，没有复活点的帮派不能再进入玩家参加第二场比赛
		std::vector<int> attacker_destroy_reborn;
		GetActiveRebornAttackerMafia(attacker_destroy_reborn);
		GNET::SendKingdomBattleHalf(1, GetWorldTag(), _battle_result, attacker_destroy_reborn);  
	
		__PRINTF("国王战战场半场结束: battle_result=%d", _battle_result); 
		GLog::log( GLOG_INFO, "国王战战场第一阶段结束: result=%d", _battle_result); 
	}
	else
	{
		if(_key_npc_info.is_alive && _key_npc_info.npc_id != 0)
		{
			MSG msg;
			BuildMessage(msg,GM_MSG_KINGDOM_QUERY_KEY_NPC,XID(GM_TYPE_NPC,_key_npc_info.npc_id), XID(-1,-1),A3DVECTOR(0,0,0), 0);
			gmatrix::SendMessage(msg);
		}
	}
}	      

void kingdom_world_manager::OnRunning2()
{
	if( _cur_timestamp < g_timer.get_systime() || 0 != _battle_result2 )
	{
		if(_battle_result2 == 0)
		{
			std::vector<int> remain_mafia_list;
			for(size_t i = 0; i < _mafia_info_list.size(); ++i)
			{
				if(_mafia_info_list[i].is_attacker && !_mafia_info_list[i].player_list.empty())
				{
					for(size_t j = 0; j < _mafia_info_list[i].player_list.size(); ++j)
					{
						if(_mafia_info_list[i].player_list[j].is_online == true)
						{
							remain_mafia_list.push_back(_mafia_info_list[i].mafia_id);
							break;
						}
					}
				}
			}

			if(!remain_mafia_list.empty())
			{
				std::random_shuffle(remain_mafia_list.begin(), remain_mafia_list.end());
				_battle_result2 = remain_mafia_list[0]; 
			}
		}

		GNET::SendKingdomEnd(1, GetWorldTag(), _battle_result2); 
		Close();
		
		MSG msg;	
		BuildMessage(msg,GM_MSG_KINGDOM_BATTLE_END,XID(GM_TYPE_PLAYER,-1),XID(-1,-1),A3DVECTOR(0,0,0), _battle_result);	
		std::vector<exclude_target> empty;
		_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);
		
		__PRINTF("国王战战场结束: battle_result=%d\n", _battle_result2); 
		GLog::log( GLOG_INFO, "国王战战场第二阶段结束: result=%d", _battle_result2); 
	}
}


void kingdom_world_manager::Close()
{
	_battle_status = BS_CLOSING;
	_cur_timestamp = g_timer.get_systime() + 120;

	ActiveSpawn( this, OPEN_CTRL_ID1, false);
	ActiveSpawn( this, OPEN_CTRL_ID2, false);

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

	__PRINTF("国王战战场开始清理\n");
	GLog::log( GLOG_INFO, "国王战结束开始清理,gs_id=%d, world_tag=%d, result=%d, end_time=%d,",
			gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_result, _end_timestamp );
}

void kingdom_world_manager::OnClosing()
{
	// 这个期间踢出玩家
	// 在玩家heartbeat逻辑里完成规定时间内的踢人
	if( _cur_timestamp < g_timer.get_systime() )
	{
		GLog::log( GLOG_INFO, "国王战结束,gs_id=%d, world_tag=%d, result1=%d, result2=%d, end_time=%d,",
				gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_result, _battle_result2, _end_timestamp );

		// 在这之前需要确保非gm玩家已经被踢出
		_battle_status = BS_READY; 
		Reset();
	}
}

void kingdom_world_manager::OnActiveSpawn(int id,bool active)
{
	__PRINTF("active ctrl: id=%d, active=%d\n", id, active);
	global_world_manager::OnActiveSpawn(id,active);
}

int kingdom_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	npc_template * pTemplate = npc_stubs_manager::Get( tid );
	if( !pTemplate ) return 0;
	spin_autolock keeper( _battle_lock );

	//守方中心生命柱体被摧毁
	if( pTemplate->role_in_war == ROLE_IN_WAR_BATTLE_KEY_BUILDING)
	{
		if(_battle_status == BS_RUNNING && (faction &  (FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND)))
		{
			_key_npc_info.is_alive = false;	
			_key_npc_info.cur_hp = 0;
			SyncKeyNPCInfo();
			_battle_result = BR_ATTACKER_WIN;  
			GLog::log(GLOG_INFO, "国王战第一阶段守方中心生命柱体被摧毁: tid=%d", tid);
		}
	}
	//复活点被摧毁
	else if(pTemplate->role_in_war == ROLE_IN_WAR_BATTLE_NORMAL_BUILDING)
	{
		for(size_t i = 0; i < _reborn_info_list.size(); ++i)
		{
			if(_reborn_info_list[i].tid == tid)
			{
				_reborn_info_list[i].is_active = false;
				_reborn_info_list[i].npc_id = 0;
				GNET::SendKingdomAttackerFail(1, GetWorldTag(), _reborn_info_list[i].mafia_id); 

				__PRINTF("国王战战场攻方复活点被摧毁: mafia_id=%d, tid=%d\n",  _reborn_info_list[i].mafia_id, tid); 
				GLog::log(GLOG_INFO, "国王战战场攻方复活点被摧毁: mafia_id=%d, tid=%d",  _reborn_info_list[i].mafia_id, tid); 
			}
		}
	}
	return 0;
}

void kingdom_world_manager::RecordBattleNPC(gnpc_imp * pImp)
{
	spin_autolock keeper( _battle_lock );

	npc_template* pTemplate = npc_stubs_manager::Get(pImp->GetParent()->tid);
	if(!pTemplate) return;

	if( pTemplate->role_in_war == ROLE_IN_WAR_BATTLE_KEY_BUILDING) 
	{
		if(pTemplate->faction &  (FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND))
		{
			_key_npc_info.npc_id = pImp->_parent->ID.id;
			_key_npc_info.cur_hp = pImp->_base_prop.max_hp;
			_key_npc_info.max_hp = pImp->_base_prop.max_hp;
			_key_npc_info.is_alive = true; 

			SyncKeyNPCInfo();
		}
	}
	else if(pTemplate->role_in_war == ROLE_IN_WAR_BATTLE_NORMAL_BUILDING)
	{
		for(size_t i = 0; i < _reborn_info_list.size(); ++i)
		{
			if(_reborn_info_list[i].tid == pImp->GetParent()->tid)
			{
				_reborn_info_list[i].npc_id = pImp->_parent->ID.id;
				break;
			}
		}
	}
}


bool kingdom_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	int mafia_id = pImp->OI_GetMafiaID();
	world_tag = GetWorldTag();
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].mafia_id == mafia_id && _reborn_info_list[i].is_active)
		{
			pos = _reborn_info_list[i].reborn_pos;
			return true;
		}
	}
	return false;
}


bool kingdom_world_manager::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag)
{
	int mafia_id = pImp->OI_GetMafiaID();
	world_tag = GetWorldTag();
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].mafia_id == mafia_id) 
		{
			pos = _reborn_info_list[i].reborn_pos;
			return true;
		}
	}
	return false;

}


bool kingdom_world_manager::IsRebornActive(gplayer_imp* pImp)
{
	int mafia_id = pImp->OI_GetMafiaID();
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].mafia_id == mafia_id) 
		{
			return _reborn_info_list[i].is_active;
		}
	}
	return false;

}

//mafia_id: 该参数本来应该是battle_faction, 这里使用mafia id来获得进入点位置
void kingdom_world_manager::GetRandomEntryPos(A3DVECTOR& pos,int mafia_id)
{
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].mafia_id == mafia_id)
		{
			pos = _reborn_info_list[i].reborn_pos;
			return;
		}
	}
	return;
}

void kingdom_world_manager::GetAttackerMafiaFaction(int mafia_id, int & faction, int & enemy_faction)
{
	int seq = 0;
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].mafia_id == mafia_id)
		{
			seq = _reborn_info_list[i].seq;
			break;
		}
	}

	GetFactionBySeq(seq, faction, enemy_faction);


}

enum
{
        KINGDOM_FACTION_1  = (1 << 20),
        KINGDOM_FACTION_2  = (1 << 21),
        KINGDOM_FACTION_3  = (1 << 22),
        KINGDOM_FACTION_4  = (1 << 23),
        KINGDOM_FACTION_5  = (1 << 24),
};

void kingdom_world_manager::GetFactionBySeq(int seq, int & faction, int & enemy_faction)
{
	switch(seq)
	{
		case 1:
			faction = KINGDOM_FACTION_1;
		       	enemy_faction = KINGDOM_FACTION_2 | KINGDOM_FACTION_3 | KINGDOM_FACTION_4 | KINGDOM_FACTION_5;	
		break;

		case 2:
			faction = KINGDOM_FACTION_2;
		       	enemy_faction = KINGDOM_FACTION_1 | KINGDOM_FACTION_3 | KINGDOM_FACTION_4 | KINGDOM_FACTION_5;	
		break;

		case 3:
			faction = KINGDOM_FACTION_3;
		       	enemy_faction = KINGDOM_FACTION_1 | KINGDOM_FACTION_2 | KINGDOM_FACTION_4 | KINGDOM_FACTION_5;	
		break;


		case 4:
			faction = KINGDOM_FACTION_4;
		       	enemy_faction = KINGDOM_FACTION_1 | KINGDOM_FACTION_2 | KINGDOM_FACTION_3 | KINGDOM_FACTION_5;	
		break;


		case 5:
			faction = KINGDOM_FACTION_5;
		       	enemy_faction = KINGDOM_FACTION_1 | KINGDOM_FACTION_2 | KINGDOM_FACTION_3 | KINGDOM_FACTION_4;	
		break;

		default:
		       	faction = KINGDOM_FACTION_1 | KINGDOM_FACTION_2 | KINGDOM_FACTION_3 | KINGDOM_FACTION_4 | KINGDOM_FACTION_5; 
			enemy_faction = 0;
		break;
	}
}

void kingdom_world_manager::OnChangeKeyNPCInfo(int type)
{
	if(_battle_status != BS_RUNNING) return;

	MSG msg;	
	BuildMessage(msg,GM_MSG_KINGDOM_CHANGE_KEY_NPC,XID(GM_TYPE_NPC,_key_npc_info.npc_id),XID(-1,-1),A3DVECTOR(0,0,0), type); 
	gmatrix::SendMessage(msg);
}

void kingdom_world_manager::OnUpdateKeyNPCInfo(int cur_hp, int max_hp, int op_type, int change_hp)
{
	if(_battle_status != BS_RUNNING) return;

	_key_npc_info.cur_hp = cur_hp;
	_key_npc_info.max_hp = max_hp;
	
	if(op_type == 1)
	{
		_key_npc_info.hp_add += change_hp;
	}
	else if(op_type == 2)
	{
		_key_npc_info.hp_dec += change_hp;
	}
	
	SyncKeyNPCInfo();
}

void kingdom_world_manager::SyncKeyNPCInfo()
{
	msg_kingdom_sync_key_npc key_npc;
	key_npc.cur_hp = _key_npc_info.cur_hp;
	key_npc.max_hp = _key_npc_info.max_hp;
	key_npc.hp_add = _key_npc_info.hp_add;
	key_npc.hp_dec = _key_npc_info.hp_dec;

	MSG msg;	
	BuildMessage(msg,GM_MSG_KINGDOM_SYNC_KEY_NPC,XID(GM_TYPE_MANAGER,KINGDOM2_TAG_ID),XID(-1,-1),A3DVECTOR(0,0,0), 0, &key_npc, sizeof(key_npc)); 
	gmatrix::SendWorldMessage(KINGDOM2_TAG_ID, msg);
}

void kingdom_world_manager::OnQueryKeyNPCInfo(int cur_hp, int max_hp)
{
	_key_npc_info.cur_hp = cur_hp;
	_key_npc_info.max_hp = max_hp;
	SyncKeyNPCInfo();
}

void kingdom_world_manager::OnGetKeyNPCInfo(int & cur_hp, int & max_hp, int & hp_add, int & hp_dec)
{
	cur_hp = _key_npc_info.cur_hp;
	max_hp = _key_npc_info.max_hp;
	hp_add = _key_npc_info.hp_add;
	hp_dec = _key_npc_info.hp_dec;
}


bool kingdom_world_manager::GetMafiaRebornInfo(int mafia_id, bool is_attacker)
{
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(!_reborn_info_list[i].is_used && _reborn_info_list[i].is_attacker == is_attacker)
		{
			_reborn_info_list[i].is_used = true;
			_reborn_info_list[i].is_active = true;
			_reborn_info_list[i].mafia_id = mafia_id;
			return true;
		}
	}
	return false;
}


void kingdom_world_manager::GetActiveRebornAttackerMafia(std::vector<int> & mafia_id_list)
{
	for(size_t i = 0; i < _reborn_info_list.size(); ++i)
	{
		if(_reborn_info_list[i].is_attacker && !_reborn_info_list[i].is_active)
		{
			mafia_id_list.push_back(_reborn_info_list[i].mafia_id);
		}
	}
}



void kingdom_world_manager::OnPlayerDeath(int killer, int deadman, int deadman_battle_faction, bool & kill_much)
{
	spin_autolock keeper( _battle_lock );
	if(_battle_status != BS_RUNNING && _battle_status != BS_RUNNING2) return;
	if(killer == deadman) return;

	for(size_t i = 0; i < _mafia_info_list.size(); ++i)
	{
		std::vector<kingdom_player_info> & player_info_list = _mafia_info_list[i].player_list;	
		for(size_t j = 0; j < player_info_list.size(); ++j)
		{
			if(killer == player_info_list[j].roleid)
			{
				_mafia_info_list[i].mafia_score += 1;
				_mafia_info_list[i].kill_count += 1;
				player_info_list[j].kill_count += 1;
			}
			else if(deadman == player_info_list[j].roleid)
			{
				_mafia_info_list[i].death_count += 1;
				player_info_list[j].death_count += 1;
			
			}
		}
	}
	SyncMafiaInfo();
}


void kingdom_world_manager::SyncMafiaInfo()
{
	S2C::CMD::kingdom_mafia_info::player_info_in_kingdom info; 
	std::vector<S2C::CMD::kingdom_mafia_info::player_info_in_kingdom> info_list;

	for(size_t i = 0; i < _mafia_info_list.size(); ++i)
	{
		if(_mafia_info_list[i].mafia_id > 0)
		{
			std::vector<kingdom_player_info> & player_info_list = _mafia_info_list[i].player_list;	
			for(size_t j = 0; j < player_info_list.size(); ++j)
			{
				info.roleid = player_info_list[j].roleid;
				info.mafia_id = player_info_list[j].mafia_id;
				info.kill_count = player_info_list[j].kill_count;
				info.death_count = player_info_list[j].death_count;

				info_list.push_back(info);
			}
		}
	}
	
	packet_wrapper h1(2048);
	using namespace S2C;
	if(!info_list.empty())
	{
		CMD::Make<CMD::kingdom_mafia_info>::From(h1, info_list.size(), (const char*)&info_list[0]);
		if(_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
	}
}

void kingdom_world_manager::OnMineStart(int roleid, int mafia_id, int mine_id)
{
	if(mine_id != KEY_MINE_ID) return;
	spin_autolock keeper( _battle_lock );

	if(_battle_status != BS_RUNNING2) return;

	packet_wrapper h1(32);
	using namespace S2C;
	CMD::Make<CMD::kingdom_gather_mine_start>::From(h1, roleid, mafia_id); 
	if(_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
}

void kingdom_world_manager::OnMineEnd(int roleid, int mafia_id, int item_id)
{
	if(item_id != KEY_MINE_ITEM_ID) return;
	if(roleid <= 0 || mafia_id <= 0) return;

	spin_autolock keeper( _battle_lock );
	if(_battle_status != BS_RUNNING2) return;

	bool found = false;
	for(size_t i = 0; i < _mafia_info_list.size(); ++i)
	{
		if(_mafia_info_list[i].mafia_id == mafia_id)
		{
			for(size_t j = 0; j < _mafia_info_list[i].player_list.size(); ++j)
			{
				if(roleid == _mafia_info_list[i].player_list[j].roleid)
				{
					found = true;
					break;
				}
			}
			break;
		}
	}

	if(!found)
	{
		GLog::log(GLOG_INFO, "国王战第二阶段采矿发生错误, roleid=%d, mafia_id=%d, item_id=%d", roleid, mafia_id, item_id);
		return;
	}
	else
	{
		_battle_result2 = mafia_id;
		__PRINTF("国王战战场第二阶段挖矿结束, mine_id=%d, mafia_id=%d, role_id=%d\n", item_id, mafia_id, roleid);
		GLog::log(GLOG_INFO, "国王战战场第二阶段挖矿结束, mine_id=%d, mafia_id=%d, role_id=%d", item_id, mafia_id, roleid);
	}
}

