#include "../player_imp.h"
#include "kingdom_bath_world_manager.h"
#include <conf.h>
#include <factionlib.h>

int kingdom_bath_world_manager::Init( const char* gmconf_file, const char* servername, int tag, int index)
{
	int rst = global_world_manager::Init( gmconf_file, servername, tag, index);
	if(rst != 0) return rst;

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

	OnInit();
	return 0;
}


bool kingdom_bath_world_manager::CreateKingdom(char fieldtype, int tag_id, int defender, std::vector<int> attacker_list)
{
	ASSERT(fieldtype == 3 && tag_id == GetWorldTag());

	spin_autolock keeper( _battle_lock );
	GLog::log(GLOG_INFO, "收到澡堂开启请求: type=%d, tag_id=%d", fieldtype, tag_id);

	if(_battle_status != BS_READY)
	{
		GLog::log( GLOG_INFO, "澡堂开启失败,原因：澡堂状态不为BS_READY, tag_id=%d, _status=%d", tag_id, _battle_status );
		return false;
	}

	_start_timestamp = g_timer.get_systime();
	_end_timestamp = _start_timestamp + 3610;
	_cur_timestamp = _end_timestamp;

	OnCreate();

	GLog::log( GLOG_INFO, "澡堂开启成功: type=%d, tag_id=%d", fieldtype, tag_id);
	return true;
	
}

void kingdom_bath_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{

}

void kingdom_bath_world_manager::PlayerLeave( gplayer* pPlayer, int faction)
{
}

void kingdom_bath_world_manager::Heartbeat()
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

void kingdom_bath_world_manager::OnInit()
{
	_battle_status = BS_READY;
}

void kingdom_bath_world_manager::OnReady()
{
}

void kingdom_bath_world_manager::OnCreate()
{
	_battle_status = BS_RUNNING;
}


void kingdom_bath_world_manager::OnRunning()
{
	// 战斗逻辑,如果时间到或者产生结果了
	if( _cur_timestamp < g_timer.get_systime())
	{
		Close();
		gmatrix::BathClosed();
	}
}	      

void kingdom_bath_world_manager::Close()
{
	_battle_status = BS_CLOSING;
	_cur_timestamp = g_timer.get_systime() + 180; //3分钟保证所有的玩家已经退出

	GLog::log( GLOG_INFO, "搓澡结束开始清理,gs_id=%d, world_tag=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _end_timestamp );
}

void kingdom_bath_world_manager::OnClosing()
{
	// 这个期间踢出玩家
	// 在玩家heartbeat逻辑里完成规定时间内的踢人
	if( _cur_timestamp < g_timer.get_systime() )
	{
		GLog::log( GLOG_INFO, "澡堂结束:gs_id=%d, world_tag=%d, end_time=%d,",
				gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _end_timestamp );

		// 在这之前需要确保非gm玩家已经被踢出
		_battle_status = BS_READY; 
		Reset();
	}
}

void kingdom_bath_world_manager::StopKingdom(char fieldtype, int tag_id)
{
	spin_autolock keeper( _battle_lock );
	if(_battle_status != BS_RUNNING) return;

	Close();
}


void kingdom_bath_world_manager::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_attacker_data.town_list.push_back(pos);
	}       
}

void kingdom_bath_world_manager::RecordRebornPos(const A3DVECTOR& pos,int faction,int ctrl_id)
{
	revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_attacker_data.revive_map[ctrl_id].push_back(temp);
	}
}


void kingdom_bath_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{       
	if(faction & FACTION_OFFENSE_FRIEND)
	{       
		_attacker_data.entry_list.push_back(pos);
	}       
}      


bool kingdom_bath_world_manager::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag)
{
	return false;
}       

void kingdom_bath_world_manager::GetRandomEntryPos(A3DVECTOR& pos,int battle_faction)
{
	pos = A3DVECTOR(0,0,0);
	if(0 == _attacker_data.entry_list.size()) return;
	pos = _attacker_data.entry_list[abase::Rand(0,_attacker_data.entry_list.size() - 1)];
} 

bool kingdom_bath_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
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

