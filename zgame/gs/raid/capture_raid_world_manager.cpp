#include "capture_raid_player.h"
#include "capture_raid_world_manager.h"
#include "raid_world_message_handler.h"
#include <factionlib.h>

capture_raid_world_manager::capture_raid_world_manager()
{
	_max_attacker_killer_id = 0;
	_max_defender_killer_id = 0; 

	_max_attacker_kill_count = 0;
	_max_defender_kill_count = 0;

	_attacker_flag_count = 0;
	_defender_flag_count = 0;

	_attacker_flag_capture_id = 0;
	_defender_flag_capture_id = 0;

	_attacker_score_time = _defender_score_time = g_timer.get_systime();

	_reset_flag_timer = CAPTURE_FLAG_BORN_TIME;
	_forbid_move_timer = 30;
	_tip2_timer = 0;
}

capture_raid_world_manager::~capture_raid_world_manager()
{
	_capture_player_info_map.clear();
	_forbid_set.clear();
}

void capture_raid_world_manager::OnPlayerEnterWorld(gplayer* pPlayer, bool reenter, int faction)
{
	int player_id = pPlayer->ID.id;
	if (reenter)
	{
		CapturePlayerInfoMap::iterator it = _capture_player_info_map.find(player_id);
		if (it != _capture_player_info_map.end())
		{
			capture_player_info& cpInfo = it->second;
			pPlayer->pos = cpInfo.leavePos;
			__PRINTF("玩家封神副本断线重入位置: x=%3f, y=%3f, z=%3f\n", pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
		}
	}
	else
	{
		capture_player_info cpInfo;
		memset(&cpInfo,0,sizeof(cpInfo));
		cpInfo.roleid = player_id;
		cpInfo.faction = faction;
		_capture_player_info_map.insert(std::make_pair(cpInfo.roleid,cpInfo));
	}

	if (_forbid_move_timer > 0)
	{
		if (_forbid_set.find(player_id) == _forbid_set.end())
		{
			_forbid_set.insert(player_id);
		}
		MSG msg;
		BuildMessage(msg,GM_MSG_CAPTURE_FORBID_MOVE,XID(GM_TYPE_PLAYER,player_id),XID(-1,-1),A3DVECTOR(0,0,0));
		gmatrix::SendMessage(msg);
	}
}

void capture_raid_world_manager::OnPlayerLeave(gplayer* pPlayer, bool cond_kick, int faction)
{
	int player_id = pPlayer->ID.id;
	CapturePlayerInfoMap::iterator it = _capture_player_info_map.find(player_id);
	if (it != _capture_player_info_map.end())
	{
		capture_player_info& cpInfo = it->second;
		cpInfo.leavePos = pPlayer->pos;
		__PRINTF("玩家%d封神副本下线,退出位置(x=%f, y=%f, z=%f)\n", pPlayer->ID.id,pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	}
	capture_raid_player_imp * pImp = (capture_raid_player_imp *)pPlayer->imp;
	if (RemoveCaptureRaidFlag(pImp,faction,player_id))
	{
		BroadCastResetFlag(faction);
		GenMatter((RF_ATTACKER | RF_DEFENDER) - faction);
	}
}

void capture_raid_world_manager::OnRunning()
{
	// raid_world_manager::OnRunning();

	//处理多次进副本delievery冷却时间
	if( _cur_timestamp < g_timer.get_systime() || _raid_result > 0)
	{
		OnClose();
		_status = RS_CLOSING;
		GNET::SendRaidEnd( gmatrix::Instance()->GetServerIndex(), _raid_id, GetWorldTag(), _raid_result); 
		_cur_timestamp = g_timer.get_systime() + _close_raid_time;
		_kill_monster_timestamp = _cur_timestamp - 90;

		//KillAllMonsters();

		// 战斗结束时更新一次信息
		SendRaidInfo();

		// 计算战斗结果
		__PRINTF( "副本:%d 结束发布结果开始准备清理result=%d, close=%d\n", GetWorldTag(), _raid_result, _close_raid_time );
		GLog::log( GLOG_INFO, "副本结束,gs_id=%d, raid_world_tag=%d, raid_id=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _raid_id, _end_timestamp );
	}

	if (_reset_flag_timer > 0)
	{
		_reset_flag_timer--;
		if (_reset_flag_timer <= 0)
		{
			GenMatter(RF_ATTACKER | RF_DEFENDER);
			BroadCastResetFlag(RF_ATTACKER | RF_DEFENDER);
		}
	}
	if (_forbid_move_timer > 0)
	{
		_forbid_move_timer--;
		if (_forbid_move_timer <= 0)
		{
			std::set<int>::iterator it = _forbid_set.begin();
			for (; it != _forbid_set.end(); ++it)
			{
				MSG msg;
				BuildMessage(msg,GM_MSG_CAPTURE_ALLOW_MOVE,XID(GM_TYPE_PLAYER,*it),XID(-1,-1),A3DVECTOR(0,0,0));
				gmatrix::SendMessage(msg);
			}
		}
	}

	if (_tip2_timer < 120)
	{
		_tip2_timer++;
		if (_tip2_timer % 30 == 0)
		{
			using namespace S2C;
			packet_wrapper h1(64);
			CMD::Make<CMD::capture_broadcast_tip>::From(h1,1);
			if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
		}

	}

}


void capture_raid_world_manager::OnClosing()
{
}

void capture_raid_world_manager::Reset()
{
	raid_world_manager::Reset();
	_player_list.clear();
	_capture_player_info_map.clear();
	_forbid_set.clear();

	_max_attacker_killer_id = 0;
	_max_defender_killer_id = 0; 

	_max_attacker_kill_count = 0;
	_max_defender_kill_count = 0;

	_attacker_flag_count = 0;
	_defender_flag_count = 0;

	_attacker_flag_capture_id = 0;
	_defender_flag_capture_id = 0;

	_attacker_score_time = _defender_score_time = g_timer.get_systime();

	_reset_flag_timer = CAPTURE_FLAG_BORN_TIME;
	_forbid_move_timer = 30;
	_tip2_timer = 0;
}

bool capture_raid_world_manager::CanCastSkillInWorld(int skill_id, int who)
{

	if (_attacker_flag_capture_id == who || _defender_flag_capture_id == who)
	{
		if (skill_id >= CAPTURE_FLAG_TRANSFORM_SKILL0 && skill_id <= CAPTURE_FLAG_TRANSFORM_SKILL1) return true;
		return false;
	}
	return true;
}

void capture_raid_world_manager::AddAttackerScore(int score)
{
	IncAttackerScore(score);
	if (GetAttackerScore() >= CAPTURE_WIN_SCORE && _raid_result == 0)
	{
		_raid_result = RAID_ATTACKER_WIN;
	};
}

void capture_raid_world_manager::AddDefenderScore(int score)
{
	IncDefenderScore(score);
	if (GetDefenderScore() >= CAPTURE_WIN_SCORE && _raid_result == 0)
	{
		_raid_result = RAID_DEFENDER_WIN;
	};
}

int capture_raid_world_manager::OnMobDeath(world* pPlane, int faction, int tid, const A3DVECTOR& pos, int attacker_id)
{
	spin_autolock keeper(_raid_lock);
	if (_raid_result)
	{
		return raid_world_manager::OnMobDeath(pPlane, faction, tid,pos,attacker_id);
	}
	CapturePlayerInfoMap::iterator it = _capture_player_info_map.find(attacker_id);
	int score = CAPTURE_KILL_MONSTER_SCORE;
	if (it != _capture_player_info_map.end())
	{
		capture_player_info& cpInfo = it->second;
		cpInfo.score += score;
		if (cpInfo.faction == RF_ATTACKER)
		{
			AddAttackerScore(score);
		}
		else if (cpInfo.faction == RF_DEFENDER)
		{
			AddDefenderScore(score);
		}
		packet_wrapper h1(64);
		using namespace S2C;
		CMD::Make<CMD::capture_monster_flag>::From(h1,cpInfo.faction,tid);
		if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	

		//add buff
		vector<int> vec_factionid;
		GetSameFactionId(attacker_id,vec_factionid);

#define CAPTURE_ADD_ATTACKEER 5352
		int skillid = 0;
		if (tid == 63236 || tid == 63238)
		{
			skillid = CAPTURE_ADD_ATTACKEER;
			for (size_t i = 0; i < vec_factionid.size(); ++i)
			{
				MSG msg;
				BuildMessage(msg,GM_MSG_GET_CAPTURE_MONSTER_AWARD,XID(GM_TYPE_PLAYER,vec_factionid[i]),XID(-1,-1),A3DVECTOR(0,0,0),skillid);
				gmatrix::SendMessage(msg);
			}
		}
	}
	return raid_world_manager::OnMobDeath(pPlane, faction, tid,pos,attacker_id);
}

bool capture_raid_world_manager::GetCapturePlayerInfo(int roleid, int & battle_faction) 
{
	spin_autolock keeper( _raid_lock );

	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(roleid == _player_list[i].roleid)
		{
			battle_faction = _player_list[i].faction;
			return true;
		}
	}
	return false;
}

void capture_raid_world_manager::IncAttackerScore(int score)
{
	int scoreup = _attacker_score + score;
	if (_attacker_score < CAPTURE_BROAD_CAST_SCORE && scoreup >= CAPTURE_BROAD_CAST_SCORE)
	{
		BroadCastScoreUp(RF_ATTACKER,scoreup);
	}
	_attacker_score = scoreup;
	_attacker_score_time = g_timer.get_systime();
	UpdateScore();
}

void capture_raid_world_manager::IncDefenderScore(int score)
{
	int scoreup = _defender_score + score;
	if (_defender_score < CAPTURE_BROAD_CAST_SCORE && scoreup >= CAPTURE_BROAD_CAST_SCORE)
	{
		BroadCastScoreUp(RF_DEFENDER,scoreup);
	}
	_defender_score = scoreup;
	_defender_score_time = g_timer.get_systime();
	UpdateScore();
}

void capture_raid_world_manager::UpdateScore()
{
        packet_wrapper h1(256);
        using namespace S2C;

	typedef S2C::CMD::capture_raid_player_info::capture_score_info capture_score_info;
	std::vector<capture_score_info> vec_info;
	CapturePlayerInfoMap::iterator it_beg = _capture_player_info_map.begin();
	CapturePlayerInfoMap::iterator it_end = _capture_player_info_map.end();
	CapturePlayerInfoMap::iterator it;
	capture_score_info info;
	for (it = it_beg; it != it_end; ++it)
	{
		capture_player_info& cpInfo = it->second;
		info.roleid = cpInfo.roleid;
		info.kill_count = cpInfo.kill_count;
		info.death_count = cpInfo.death_count;
		info.faction = cpInfo.faction;
		info.flag_count = cpInfo.flag_count;
		vec_info.push_back(info);
	}
	CMD::Make<CMD::capture_raid_player_info>::From(h1,GetAttackerScore(),GetDefenderScore(),_attacker_flag_count,
	_defender_flag_count,_max_attacker_killer_id,_max_defender_killer_id,_max_attacker_kill_count,_max_defender_kill_count,&vec_info[0],vec_info.size());
        if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}

void capture_raid_world_manager::OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int dead_battle_faction, bool& kill_much )
{

	spin_autolock keeper(_raid_lock);

	CapturePlayerInfoMap::iterator it_deadman = _capture_player_info_map.find(deadman);

	if (it_deadman == _capture_player_info_map.end())
	{
		return;
	}

	capture_raid_player_imp *pImp = (capture_raid_player_imp *)pPlayer->imp;
	if (RemoveCaptureRaidFlag(pImp, dead_battle_faction, deadman))
	{
		BroadCastResetFlag(dead_battle_faction);
		GenMatter((RF_ATTACKER | RF_DEFENDER) - dead_battle_faction);
	}
	if (killer == deadman) return;

	capture_player_info& deadmanInfo = it_deadman->second;
	deadmanInfo.death_count += 1;
	CapturePlayerInfoMap::iterator it_killer = _capture_player_info_map.find(killer);
	if ( killer <= 0 || it_killer == _capture_player_info_map.end()) 
	{
		UpdateScore();
		return;
	}

	capture_player_info& killerInfo = it_killer->second;

	killerInfo.kill_count += 1;
	int score = 0;
	killerInfo.score += score;
	
	CapturePlayerInfoMap::iterator it_beg = _capture_player_info_map.begin();
	CapturePlayerInfoMap::iterator it_end = _capture_player_info_map.end();
	CapturePlayerInfoMap::iterator it;
	int max_kill_count = 0;
	int max_kill_player = 0;
	int faction = killerInfo.faction;
	for (it = it_beg; it != it_end; ++it)
	{
		capture_player_info& cpInfo = it->second;
		if (cpInfo.faction == faction && cpInfo.kill_count > max_kill_count)
		{
			max_kill_count = cpInfo.kill_count;
			max_kill_player = cpInfo.roleid;
		}
	}
	if (max_kill_count)
	{
		if (faction == RF_ATTACKER)
		{
			_max_attacker_killer_id = max_kill_player;
			_max_attacker_kill_count = max_kill_count;
		}
		else
		{
			_max_defender_killer_id = max_kill_player;
			_max_defender_kill_count = max_kill_count;
		}
	}

	if (faction == RF_ATTACKER)
	{
		AddAttackerScore(score);
	}
	else if (faction == RF_DEFENDER)
	{
		AddDefenderScore(score);
	}
}

void capture_raid_world_manager::OnClose()
{
	if (!_raid_result)
	{
		if (_attacker_score > _defender_score)
		{
			_raid_result = RAID_ATTACKER_WIN;
		}
		else if (_attacker_score < _defender_score)
		{
			_raid_result = RAID_DEFENDER_WIN;
		}
		else
		{
			_raid_result = RAID_DRAW;
		}
	}
	KillAllMatter();
	raid_world_manager::OnClose();
}

void capture_raid_world_manager::BroadCastScoreUp(char faction,char score)
{
	packet_wrapper h1(64);
	using namespace S2C;
	
	CMD::Make<CMD::capture_broadcast_score>::From(h1,faction,score);
	if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}

void capture_raid_world_manager::BroadCastSubmitFlag(char faction, gplayer_imp * pImp)
{
	packet_wrapper h1(64);
	using namespace S2C;
	
	int player_id = pImp->GetParent()->ID.id;

	unsigned len = 0;
	const void* name = pImp->GetPlayerName(len);
	CMD::Make<CMD::capture_submit_flag>::From(h1,faction,player_id,name, len);
	if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}

void capture_raid_world_manager::BroadCastResetFlag(char faction)
{
	packet_wrapper h1(64);
	using namespace S2C;

	CMD::Make<CMD::capture_reset_flag>::From(h1,faction);
	if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}

void capture_raid_world_manager::BroadCastGatherFlag(char faction, gplayer_imp * pImp)
{
	packet_wrapper h1(64);
	using namespace S2C;

	int player_id = pImp->GetParent()->ID.id;
	unsigned len = 0;
	const void* name = pImp->GetPlayerName(len);
	CMD::Make<CMD::capture_gather_flag>::From(h1,faction,player_id, name, len);
	if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}

void capture_raid_world_manager::SetAttackerFlagID(gplayer_imp * pImp)
{
	int player_id = pImp->GetParent()->ID.id;
	spin_autolock keeper(_raid_lock);
	_attacker_flag_capture_id = player_id;
	BroadCastGatherFlag(RF_ATTACKER, pImp);
}

void capture_raid_world_manager::SetDefenderFlagID(gplayer_imp * pImp)
{
	int player_id = pImp->GetParent()->ID.id;
	spin_autolock keeper(_raid_lock);
	_defender_flag_capture_id = player_id;
	BroadCastGatherFlag(RF_DEFENDER, pImp);
}

inline bool is_in_zone(A3DVECTOR min, A3DVECTOR max, A3DVECTOR pos)
{
	return (min.x <= pos.x &&  min.z <= pos.z) && (max.x >= pos.x && max.z >= pos.z);
}

void capture_raid_world_manager::SubmitCaptureRaidFlag(gplayer_imp * pImp, int faction)
{
	spin_autolock keeper(_raid_lock);
	bool can_submit_flag = true;
	gplayer* pPlayer = pImp->GetParent();
	if (faction == RF_ATTACKER)
	{
		A3DVECTOR posAMin(210,302,-392),posAMax(245,400,-370);
		if (_defender_flag_capture_id || !is_in_zone(posAMin,posAMax,pPlayer->pos))
		{
			can_submit_flag = false;
		}
	}
	else
	{
		A3DVECTOR posBMin(209,302,-86), posBMax(246,302,-62);
		if (_attacker_flag_capture_id || !is_in_zone(posBMin,posBMax,pPlayer->pos))
		{
			can_submit_flag = false;
		}
	}
	if (!can_submit_flag)
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_SUBMIT_CAPTURE_RAID_FLAG);
		return;
	}

	int player_id = pPlayer->ID.id;
	int score = CAPTURE_SUBMIT_FLAG_SCORE;
	CapturePlayerInfoMap::iterator it = _capture_player_info_map.find(player_id);
	if (it != _capture_player_info_map.end())
	{
		capture_player_info& cpInfo = it->second;
		cpInfo.score += score;
		cpInfo.flag_count++;
	}

	RemoveCaptureRaidFlag(pImp,faction,player_id);
	MSG msg;
	BuildMessage(msg,GM_MSG_CLEAN_FLAG_TRANSFORM,XID(GM_TYPE_PLAYER,player_id),XID(-1,-1),A3DVECTOR(0,0,0));
	gmatrix::SendMessage(msg);
	int other_playerid = _attacker_flag_capture_id > 0 ? _attacker_flag_capture_id : _defender_flag_capture_id;
	if (other_playerid > 0)
	{
		int index = gmatrix::FindPlayer(other_playerid);
		if (index >= 0) 
		{
			gplayer * pOtherPlayer = gmatrix::GetPlayerByIndex(other_playerid);
			capture_raid_player_imp* pOtherImp =(capture_raid_player_imp*)pOtherPlayer->imp;
			int otherfaction = pOtherImp->GetFaction();
			RemoveCaptureRaidFlag(pOtherImp,otherfaction,other_playerid);
			MSG msg2;
			BuildMessage(msg2,GM_MSG_CLEAN_FLAG_TRANSFORM,XID(GM_TYPE_PLAYER,other_playerid),XID(-1,-1),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg2);
		}
	}
	else
	{
		KillAllMatter();
	}
	BroadCastSubmitFlag(faction,pImp);
	_reset_flag_timer = CAPTURE_FLAG_REBORN_TIME;
	_attacker_flag_capture_id = _defender_flag_capture_id = 0;

	if (faction == RF_ATTACKER)
	{
		_attacker_flag_count++;
		if (_attacker_flag_count >= CAPTURE_FLAG_MAX) _raid_result = RAID_ATTACKER_WIN; 
		AddAttackerScore(score);
	}
	else
	{
		_defender_flag_count++;
		if (_defender_flag_count >= CAPTURE_FLAG_MAX) _raid_result = RAID_DEFENDER_WIN;
		AddDefenderScore(score);
	}
}

int capture_raid_world_manager::RemoveCaptureRaidFlag(gplayer_imp * pImp, int faction, int player_id)
{
	if (player_id == _attacker_flag_capture_id || player_id == _defender_flag_capture_id)
	{
		int item_id = faction == RF_ATTACKER ? DEFENDER_MATTER_ITEM_ID : ATTACKER_MATTER_ITEM_ID;
		pImp->TakeOutItem(item_id,1);
		if (player_id == _attacker_flag_capture_id)
		{
			_attacker_flag_capture_id = 0;
		}
		else
		{
			_defender_flag_capture_id = 0;
		}
		return faction;
	}
	return 0;
}

void capture_raid_world_manager::GenMatter(int faction)
{
	int tid[2] = {0};
	A3DVECTOR pos[2] ; 
	A3DVECTOR atkpos(228,300,-320);
	A3DVECTOR defpos(228,300,-137);

	pos[0] = atkpos;
	pos[1] = defpos;

	if (faction & RF_ATTACKER)
	{
		tid[0] = ATTACKER_MATTER_ID;
	}

	if (faction & RF_DEFENDER)
	{
		tid[1] = DEFENDER_MATTER_ID;
	}

	for (int i = 0; i < 2; ++i)
	{
		if (!tid[i]) continue;
		mine_template  *pTemplate = mine_stubs_manager::Get(tid[i]);
		if (!pTemplate)
		{
			__PRINTF("error mine id = %d\n",tid[i]);
			continue;
		}

		mine_spawner::entry_t ent;
		memset(&ent,0,sizeof(ent));
		ent.mid = tid[i];
		ent.mine_count = 1;
		ent.reborn_time = 10000;

		world *plane = GetWorldByIndex(0);
		if(!plane)
		{
			__PRINTF("error plane id = %d\n",tid[i]);
			continue;
		}

		gmatter * pMatter = mine_spawner::CreateMine(NULL,pos[i],plane,0,ent,0,1,0);
		if(pMatter)
		{
			plane->InsertMatter(pMatter);
			pMatter->imp->_runner->enter_world();
			pMatter->Unlock();
		}
	}
}

void capture_raid_world_manager::KillAllMatter()
{
	//需要残留怪物,物品等检测 全杀,策划保证战斗停止后所有控制器不会再触发	
	__PRINTF("副本(gs_id: %d,raid_id: %d,world_tag: %d) 杀死所有matter\n",		gmatrix::GetServerIndex(),_raid_id,GetWorldTag());
	MSG msg;	
	BuildMessage(msg,GM_MSG_MINE_DISAPPEAR,XID(GM_TYPE_MATTER,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
	std::vector<exclude_target> empty;
	_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF,empty);
}

int capture_raid_world_manager::TestCreateRaid(const raid_world_template& rwt,
	int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	if(_status != RS_IDLE)
	{
		return -1;
	}

	if((size % sizeof(player_apply_info)) != 0 )
	{
		return -2;
	}

	return 0;
}

int capture_raid_world_manager::CreateRaid(int raidroom_id, const raid_world_template& rwt,
		int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	ASSERT(size % sizeof(player_apply_info) == 0);

	_status = RS_READY;

	_raid_id = rwt.rwinfo.raid_id;
	int ret = global_world_manager::Init(0, rwt.servername.c_str(), tag, index);
	if( 0 != ret )
	{
		return ret;
	}
	delete _message_handler;
	_message_handler = new raid_world_message_handler( this, &_plane );

	if(!OnInit()) 
	{
		return -10015;
	}
	__PRINTF( "副本创建请求, raid_template_id=%d, raid_id=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
	
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt.rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		__PRINTF("无效的副本模板: %d\n", rwt.rwinfo.raid_template_id);
		GLog::log( GLOG_INFO, "副本开启失败,原因：副本模板不存在 raid_template_id=%d, raid_id=%d, _status=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status );
		return -10011;
	}	
	
	
	_close_raid_time = 300;
	_player_leave_time = 30;
	_raid_time = raid_config.total_exist_time + 30;	//30s准备时间

	_start_timestamp = g_timer.get_systime();
	_end_timestamp = _start_timestamp + _raid_time;
	_cur_timestamp = _end_timestamp;
	_raid_id = rwt.rwinfo.raid_id;
	_apply_count = roleid_list.size();
	
	OnCreate();
	_status = RS_RUNNING;
	
	player_apply_info * pInfo = (player_apply_info * ) buf;
	size_t count = size / sizeof(player_apply_info);
	player_apply_info info; 
	for(size_t i = 0; i < count; ++i, ++pInfo)
	{
		info.roleid = pInfo->roleid;
		info.faction = pInfo->faction;
		_player_list.push_back(info);
	}

	__PRINTF( "副本:%d:%d开启,持续时间:%d\n", _raid_id, GetWorldTag(), _raid_time); 
	GLog::log( GLOG_INFO, "副本开启成功 raid_template_id=%d, raid_id=%d, end_time=%d\n", rwt.rwinfo.raid_template_id,_raid_id, _end_timestamp);
	return 0;
	
}

void capture_raid_world_manager::GetSameFactionId(int attacker_id, vector<int>& vec_factionid)
{
	int faction = 0;
	for (size_t i = 0; i < _player_list.size(); ++i)
	{
		player_apply_info & info = _player_list[i];
		if (info.roleid == attacker_id)
		{
			faction = info.faction;
			break;
		}
	}

	for (size_t i = 0; i < _player_list.size(); i++)
	{
		player_apply_info & info = _player_list[i];
		if (info.faction == faction)
		{
			vec_factionid.push_back(info.roleid);
		}
	}
}

void capture_raid_world_manager::UpdatePlayerScore()
{
	spin_autolock keeper(_raid_lock);
	UpdateScore();
}

void capture_raid_world_manager::OnCreate()
{
	raid_world_manager::OnCreate();
	ActiveSpawn( this, CTRL_CONDISION_ID_RAID_BEGIN, true ); 
}

void capture_raid_world_manager::HandleCaptureRaidSyncPos(char faction, const A3DVECTOR& pos)
{
	using namespace S2C;
	packet_wrapper h1(64);
	CMD::Make<CMD::capture_broadcast_flag_move>::From(h1,faction,pos);
	if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}
