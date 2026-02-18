#include "seek_raid_player.h"
#include "../clstab.h"
#include "../obj_interface.h"
#include "seek_raid_world_manager.h"
#include "../playertransform.h"

DEFINE_SUBSTANCE(seek_raid_player_imp,raid_player_imp,CLS_SEEK_RAID_PLAYER_IMP)

seek_raid_player_imp::seek_raid_player_imp()
{
}

seek_raid_player_imp::~seek_raid_player_imp()
{
}

int seek_raid_player_imp::MessageHandler(const MSG& msg)
{
	gplayer *pPlayer = GetParent();
	switch (msg.message)
	{
		case GM_MSG_SEEK_PREPARE:
		{
			if (msg.param) //躲藏者
			{
				A3DVECTOR pos(-85,405,-93);
				LongJump(pos);
				for (int i = seek_raid_world_manager::HIDE_SKILL0; i <= seek_raid_world_manager::HIDE_SKILL9; i++)
					InsertSysSkill(i,1);
				SendClientHiderBloodNum(0,2);
			}
			else
			{
				A3DVECTOR pos(-284,404,-292);
				LongJump(pos);
				_commander->DenyCmd(controller::CMD_MOVE);
				object_interface oif(this);
				oif.SendClientNotifyRoot(0);
				LazySendTo<0>(GM_MSG_SEEK_TRANSFORM, _parent->ID,0, 10); //解决时序问题
				//OI_StartTransform(923,1,1,600,player_transform::TRANSFORM_TASK);
				//SendClientSeekerSkillInfo(20,0);
			}
			return 0;
		}
		break;

		case GM_MSG_SEEK_TRANSFORM:
		{
			OI_StartTransform(923,1,1,600,player_transform::TRANSFORM_TASK);
			SendClientSeekerSkillInfo(20,0);
			return 0;
		}
		break;

		case GM_MSG_SEEK_START:
		{
			_commander->AllowCmd(controller::CMD_MOVE);
			object_interface oif(this);
			oif.SendClientDispelRoot(0);
			return 0;
		}
		break;

		case GM_MSG_SEEK_RAID_KILL:
		{
			char blood_num = 2 - msg.param;
			SendClientHiderBloodNum(msg.param2,blood_num);
			if (!blood_num)
			{
				pPlayer->SetExtraState2(gplayer::STATE_SEEKRAID_DIE);
				if (_transform_obj) StopTransform(_transform_obj->GetTransformID(),player_transform::TRANSFORM_SKILL);
			}
			return 0;
		}
		break;

		case GM_MSG_SEEK_STOP:
		{
			if (msg.param)
			{
				gplayer * pPlayer = GetParent();
				pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_VISITOR);
				if (_silent_seal_flag) SetSilentSealMode(false);
				for (int i = seek_raid_world_manager::HIDE_SKILL0; i <= seek_raid_world_manager::HIDE_SKILL9; i++)
					ClearSysSkill(i);
				if (_transform_obj) StopTransform(_transform_obj->GetTransformID(),player_transform::TRANSFORM_SKILL);
			}
			else
			{
				OI_StopTransform(923,player_transform::TRANSFORM_TASK);
				ClearSysSkill(5328);
			}
			if (pPlayer->CheckExtraState2(gplayer::STATE_SEEKRAID_DIE)) pPlayer->ClrExtraState2(gplayer::STATE_SEEKRAID_DIE);
			return 0;
		}
		break;

		case GM_MSG_SEEK_RAID_SKILL_LEFT:
		{
			SendClientSeekerSkillInfo(msg.param,msg.param2);
			return 0;
		}
		break;

		case GM_MSG_ENCHANT:
		{
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if( !ech_msg.helpful )
			{
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}
			else
			{
				if( !(ech_msg.attacker_faction & (GetFaction())))
				{
					return 0;
				}
			}
			ech_msg.is_invader = false;
			ech_msg.force_attack = 1;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;
	}
	return raid_player_imp::MessageHandler(msg);
}

void seek_raid_player_imp::PlayerEnter()
{
	packet_wrapper h1(64);
	using namespace S2C;
	raid_player_imp::PlayerEnter();
	CMD::Make<CMD::hide_and_seek_enter_raid>::From(h1);
	send_ls_msg(GetParent(),h1.data(),h1.size());
}

void seek_raid_player_imp::PlayerLeave()
{
	packet_wrapper h1(64);
	using namespace S2C;
	raid_player_imp::PlayerLeave();
	CMD::Make<CMD::hide_and_seek_leave_raid>::From(h1);
	send_ls_msg(GetParent(),h1.data(),h1.size());
}

bool seek_raid_player_imp::SetSeekRaidRandomSkillAttack(unsigned int & skill_id)
{
	seek_raid_world_manager * pManager = (seek_raid_world_manager *)GetWorldManager();
	SeekPlayerInfoMap & seek_player_info_map = pManager->GetSeekPlayerInfoMap();
	SeekPlayerInfoMap::iterator it = seek_player_info_map.find(_parent->ID.id);
	if (it == seek_player_info_map.end()) return false;
	seek_player_info &hsInfo = it->second;
	ScoreMap & scoremap = pManager->GetScoreMap();
	IntMap::iterator it2;
	ScoreMap::iterator it3 = scoremap.find(_parent->ID.id);
	if (it3 == scoremap.end()) return false;

	IntMap sa_map = pManager->GetSkill2AchieveMap();
	IntMap bs_map = pManager->GetBasic2CostMap();
	if (skill_id > 100000) 
	{
		if (hsInfo.used_random_skill || hsInfo.random_skill) return false;
		if (scoremap[_parent->ID.id].num < 40) return false;
		int skillid[10] = {0}; 
		int num = 0;
		
		for (it2 = bs_map.begin(); it2 != bs_map.end(); ++it2)
			skillid[num++] = it2->first;
		for (it2 = sa_map.begin(); it2 != sa_map.end(); ++it2)
		{
			if (IsAchievementFinish(it2->second))
			{
				skillid[num++] = it2->first;
			}
		}
		std::random_shuffle(skillid,skillid+num);
		if (skillid[0] >= seek_raid_world_manager::HIDE_SKILL0 && skillid[0]  <= seek_raid_world_manager::HIDE_SKILL9)
		{
			if (!CheckCoolDown(skillid[0]+SKILL_COOLINGID_BEGIN)) return false;
		}
		skill_id = skillid[0];
		hsInfo.random_skill = skill_id;
		return true;
	}
	else
	{
		for (it2 = sa_map.begin(); it2 != sa_map.end(); ++it2)
		{
			if (skill_id == (unsigned)it2->first)
			{
				if (!IsAchievementFinish(it2->second)) return false;
				return true;
			}
		}
	}
	if (skill_id >= seek_raid_world_manager::HIDE_SKILL0 && skill_id <= seek_raid_world_manager::HIDE_SKILL9)
	{
		if (!CheckCoolDown(skill_id+SKILL_COOLINGID_BEGIN)) return false;
	}
	return true;
}

void seek_raid_player_imp::BeHurtOnSeekAndHideRaid(const XID & attacker)
{
	seek_raid_world_manager * pManager = (seek_raid_world_manager *)GetWorldManager();
	SeekPlayerInfoMap & seek_player_info_map = pManager->GetSeekPlayerInfoMap();
	SeekPlayerInfoMap::iterator it = seek_player_info_map.find(_parent->ID.id);
	seek_player_info &hsInfo = it->second;
	gplayer* pPlayer = GetParent();
	if (hsInfo.ishide)
	{
		pManager->OnSeekRaidKill(pPlayer->ID.id,attacker.id);
	}
}

void seek_raid_player_imp::SendClientHiderTaunted(bool isStart)
{
	seek_raid_world_manager * pManager = (seek_raid_world_manager *)GetWorldManager();
	gplayer* pPlayer = GetParent();
	pManager->OnSeekRaidHiderTaunted(pPlayer->ID.id, pPlayer->pos,isStart);
}

void seek_raid_player_imp::GetSeekAndHiderTauntedScore()
{
	seek_raid_world_manager * pManager = (seek_raid_world_manager *)GetWorldManager();
	gplayer* pPlayer = GetParent();
	pManager->OnSeekRaidHiderTauntedGetScore(pPlayer->ID.id);
}

void seek_raid_player_imp::SendClientHiderBloodNum(int seeker_id, char blood_num)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gplayer * pPlayer = GetParent();
	if (!blood_num) //观察者
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_VISITOR);
		SetSilentSealMode(true);
	}
	CMD::Make<CMD::hide_and_seek_blood_num>::From(h1,seeker_id,pPlayer->ID.id,blood_num);
	AutoBroadcastCSMsg(_plane,pPlayer->pPiece,h1,-1);
}

void seek_raid_player_imp::SendClientSeekerSkillInfo(char skill_left, char skill_buy_num)
{
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<CMD::hide_and_seek_skill_info>::From(h1,skill_left,skill_buy_num);
	gplayer * pPlayer = GetParent();
	send_ls_msg(pPlayer,h1.data(),h1.size());
}

void seek_raid_player_imp::HandleRaidResult()
{
	seek_raid_world_manager* pManager = (seek_raid_world_manager*)GetWorldManager();

	int cur_time = g_timer.get_systime();
	bool same_day = IsSameDay(cur_time,_hide_and_seek_info.hide_and_seek_timestamp);
	ScoreMap & scoremap = pManager->GetScoreMap();
	int player_id = _parent->ID.id;
	short score = scoremap[player_id].num;
	if (!same_day)
	{
		_hide_and_seek_info.score = score;
	}
	else
	{
		if (score > _hide_and_seek_info.score) _hide_and_seek_info.score = score;
	}
	_hide_and_seek_info.hide_and_seek_timestamp = cur_time;
	SendHideAndSeekRaidInfo();
}

void seek_raid_player_imp::PlayerEnterSeekRaid()
{
	gplayer * pPlayer = GetParent();
	seek_raid_world_manager* pManager = (seek_raid_world_manager*)GetWorldManager();
	if (_reenter)
	{
		pManager->OnPlayerReenterSeekRaid(pPlayer,_reenter,_battle_faction);
	}
	else
	{
		pManager->SlowPlayerEnterSeekRaid(pPlayer);
	}
}

void seek_raid_player_imp::SendClientRaidRoundInfo(int step, int end_timestamp)
{
	packet_wrapper h1(64);
	using namespace S2C;
	char round = (step-1)/3;
	char round_status = (step-1)%3;
	CMD::Make<CMD::hide_and_seek_round_info>::From(h1,round,round_status, end_timestamp);
	send_ls_msg(GetParent(),h1.data(),h1.size());
}
