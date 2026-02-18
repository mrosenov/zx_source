#include "capture_raid_player.h"
#include "capture_raid_world_manager.h"
#include "../clstab.h"
#include "../obj_interface.h"
#include "../playertransform.h"
#include <factionlib.h>
#include "../actsession.h"
#include "../invincible_filter.h"

DEFINE_SUBSTANCE(capture_raid_player_imp, raid_player_imp, CLS_CAPTURE_RAID_PLAYER_IMP)

capture_raid_player_imp::capture_raid_player_imp()
{
	_revive_counter = 0;
	_transform_counter = 0;
	_is_flag_player = false;
}

capture_raid_player_imp::~capture_raid_player_imp()
{
}

int capture_raid_player_imp::MessageHandler(const MSG & msg)
{
	gplayer *pPlayer = GetParent();
	switch(msg.message)
	{
		//禁止多人骑乘	
		case GM_MSG_PLAYER_LINK_RIDE_INVITE: 
		case GM_MSG_PLAYER_LINK_RIDE_INV_REPLY:
		//禁止相依相偎
		case GM_MSG_PLAYER_BIND_REQUEST:
		case GM_MSG_PLAYER_BIND_INVITE:
		case GM_MSG_PLAYER_BIND_REQ_REPLY:
		case GM_MSG_PLAYER_BIND_INV_REPLY:		
		{
			return 0;
		}
		break;

		case GM_MSG_GET_CAPTURE_MONSTER_AWARD:
		{
			SKILL::Data data(msg.param);
			_skill.CastRune(data, this, 1,0);
			return 0;
		}
		break;

		case GM_MSG_CLEAN_FLAG_TRANSFORM:
		{
			_is_flag_player = false;
			pPlayer->ClrExtraState2(gplayer::STATE_CAPTURE_RAID_FLAG);
			if (_transform_obj) StopTransform(_transform_obj->GetTransformID(),player_transform::TRANSFORM_SKILL);
			ClrCoolDown(CAPTURE_FLAG_TRANSFORM_SKILL0+ SKILL_COOLINGID_BEGIN);
			ClrCoolDown(CAPTURE_FLAG_TRANSFORM_SKILL1+ SKILL_COOLINGID_BEGIN);
			return 0;
		}
		break;

		case GM_MSG_CAPTURE_FORBID_MOVE:
		{
			_commander->DenyCmd(controller::CMD_MOVE);
			object_interface oif(this);
			oif.SendClientNotifyRoot(0);
			using namespace S2C;
			packet_wrapper h1(64);
			CMD::Make<CMD::capture_broadcast_tip>::From(h1,2);
			send_ls_msg(pPlayer,h1);
			return 0;
		}
		break;

		case GM_MSG_CAPTURE_ALLOW_MOVE:
		{
			_commander->AllowCmd(controller::CMD_MOVE);
			object_interface oif(this);
			oif.SendClientDispelRoot(0);
			return 0;
		}
		break;
	}

	return raid_player_imp::MessageHandler(msg);
}

void capture_raid_player_imp::HandleDeath()
{
	if(IsDead())
	{   
		++_revive_counter;
		if(_revive_counter >= REVIVE_MAX_TIMEOUT)
		{   
			//强制复活
			_revive_counter = 0;
			ClearSession();
			session_resurrect_in_town *pSession= new session_resurrect_in_town(this);
			pSession->SetExpReduce(0);
			AddStartSession(pSession);
		}   

	}   
	else
	{
		_revive_counter = 0;
	}
}

void capture_raid_player_imp::PlayerEnter()
{
}

void capture_raid_player_imp::PlayerEnterWorld()
{
	__PRINTF( "玩家 %d 在副本世界 %d 上线啦!!!\n", _parent->ID.id, _plane->GetTag() );
	EnableFreePVP( true );
	capture_raid_world_manager* pManager = (capture_raid_world_manager*)GetWorldManager();
	gplayer* pPlayer = GetParent();

	bool rst = pManager->GetCapturePlayerInfo(_parent->ID.id, _battle_faction); 
	if(rst)
	{
		if( RF_ATTACKER == _battle_faction )
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
			_attack_faction = FACTION_BATTLEDEFENCE;
			_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
		}
		else if( RF_DEFENDER == _battle_faction ) 
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
			_attack_faction = FACTION_BATTLEOFFENSE;
			_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		}
		else if( RF_VISITOR == _battle_faction ) 
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_VISITOR);
			SetSilentSealMode(true);
			_attack_faction = 0;
			_defense_faction = 0;
		}

		gplayer* pPlayer = GetParent();
		
		pManager->PlayerEnter( pPlayer, _battle_faction, _reenter);
		pManager->GetRandomEntryPos(pPlayer->pos,_battle_faction);
		pPlayer->pos.y += 0.2;
		_raid_faction = _battle_faction;
	}

	gplayer_imp::PlayerEnterWorld();
		
	if(_success_entered)
	{
		GNET::SendRaidEnterSuccess(gmatrix::GetServerIndex(), _parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag());
	}

	GLog::log(GLOG_INFO, "玩家进入副本: roleid=%d, raid_id=%d, raid_world_tag=%d, difficulty=%d", 
			_parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());
	if(rst)
	{
		_runner->enter_raid( _battle_faction, (unsigned char)(pManager->GetRaidType()), pManager->GetRaidID(), pManager->GetRaidEndstamp() );
	
	}
	else
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "玩家在封神副本因为信息异常踢出副本: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( pManager->GetRaidID(), _timeout );
	}
	pManager->PlayerEnterWorld( pPlayer, _battle_faction, _reenter);
}

void capture_raid_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	raid_player_imp::OnDeath(lastattack, is_pariah, faction_battle, is_hostile_duel, time);
	bool kill_much = false;
	gplayer* pPlayer = GetParent();
	if (_is_flag_player)
	{
		_is_flag_player = false;
		pPlayer->ClrExtraState2(gplayer::STATE_CAPTURE_RAID_FLAG);
		ClrCoolDown(CAPTURE_FLAG_TRANSFORM_SKILL0+ SKILL_COOLINGID_BEGIN);
		ClrCoolDown(CAPTURE_FLAG_TRANSFORM_SKILL1+ SKILL_COOLINGID_BEGIN);

	}
	capture_raid_world_manager* pManager = (capture_raid_world_manager*)GetWorldManager();
	_resurrect_hp_recover = 1.0f;
	pManager->OnPlayerDeath(pPlayer, lastattack.id, pPlayer->ID.id, _battle_faction, kill_much);
}

void capture_raid_player_imp::OnResurrect()
{
	_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, 10)); 
}

void capture_raid_player_imp::PlayerSubmitCaptureRaidFlag()
{
	capture_raid_world_manager* pManager = (capture_raid_world_manager *)GetWorldManager();
	int item_id = _battle_faction == RF_ATTACKER ? DEFENDER_MATTER_ITEM_ID : ATTACKER_MATTER_ITEM_ID;
	if (_inventory.Find(0, item_id) < 0)
	{
		_runner->error_message(S2C::ERR_HAS_NOT_CAPTURE_RAID_FLAG);
		return;
	}
	pManager->SubmitCaptureRaidFlag(this,_battle_faction);
}

int capture_raid_player_imp::CheckCanMineInCaptureRaid(int matter_id)
{
	XID target(GM_TYPE_MATTER,matter_id);
	world::object_info info;

	bool rst = _plane->QueryObject(target,info);
	if (!rst) return 0;

	if (info.tid == DEFENDER_MATTER_ID)
	{
		if (_battle_faction == RF_DEFENDER) return 1;
	}
	if (info.tid == ATTACKER_MATTER_ID)
	{
		if (_battle_faction == RF_ATTACKER) return 1;
	}
	if (_inventory.IsFull()) return 2;
	return 0;
}

void capture_raid_player_imp::OnGatherMatter(int matter_tid ,int cnt)
{
	GetParent()->SetExtraState2(gplayer::STATE_CAPTURE_RAID_FLAG);
	capture_raid_world_manager *pManager = (capture_raid_world_manager *)GetWorldManager();
	if (_battle_faction == RF_ATTACKER)
	{
		pManager->SetAttackerFlagID(this);
	}
	else
	{
		pManager->SetDefenderFlagID(this);
	}
	SKILL::Data data(CAPTURE_CLEAR_SKILL_ID);
	_skill.CastRune(data, this, 1,0);
	SKILL::Data data2(CAPTURE_CLEAR_SKILL_ID2);
	_skill.CastRune(data2, this, 1,0);
	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF | filter::FILTER_MASK_BUFF);
	_summonman.ClearAllSummonNPC(this);
	
	_transform_counter = 1;
	_is_flag_player = true;
}

void capture_raid_player_imp::HandleRaidResult()
{
	gplayer *pPlayer = GetParent();
	capture_raid_world_manager* pManager = (capture_raid_world_manager*)GetWorldManager();
	int raid_result = pManager->GetRaidResult();

	int atk_score = pManager->GetAttackerScore();
	int def_score = pManager->GetDefenderScore();

	if (atk_score + def_score)
	{
		int atk_num = 0;
		int def_num = 0;
		int item_num = 0;
		if (atk_score == def_score)
		{
			atk_num = def_num = 50;
		}
		else if (atk_score > def_score)
		{
			atk_num = (100 * atk_score % (atk_score + def_score) == 0) ? (100 * atk_score)/(atk_score+def_score) : ((100*atk_score)/(atk_score+def_score) + 1);
			def_num = 100-atk_num;
		}
		else
		{
			def_num = (100 * def_score % (atk_score + def_score) == 0) ? (100 * def_score)/(atk_score+def_score) : ((100*def_score)/(atk_score+def_score) + 1);
			atk_num = 100-def_num;
		}

		if (pPlayer->IsBattleOffense())
		{
			item_num = atk_num;
			DeliverItem(AWARD_ITEM_ID,atk_num,false,0,ITEM_INIT_TYPE_TASK);
		}
		else if (pPlayer->IsBattleDefence() && def_num)
		{
			item_num = def_num;
			DeliverItem(AWARD_ITEM_ID,def_num,false,0,ITEM_INIT_TYPE_TASK);
		}
		GLog::log(GLOG_INFO,"player finish fengshen raid: rold_id=%d,item_id=%d,item_num=%d",
				_parent->ID.id, AWARD_ITEM_ID,item_num);
	}

	if((raid_result == RAID_ATTACKER_WIN && pPlayer->IsBattleOffense()) || (raid_result == RAID_DEFENDER_WIN && pPlayer->IsBattleDefence()))
	{
		DeliverItem(AWARD_ITEM_ID2,1,false,0,ITEM_INIT_TYPE_TASK);
	}
	GLog::log(GLOG_INFO, "玩家完成封神战场副本: roleid= %d, raid_id=%d, radi_world_tag =%d, difficulty=%d", 
			                  _parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());

}

void capture_raid_player_imp::PlayerEnterCaptureRaid()
{
	capture_raid_world_manager * pManager = (capture_raid_world_manager *)GetWorldManager();
	pManager->UpdatePlayerScore();
}

void capture_raid_player_imp::OnHeartbeat(size_t tick)
{
	raid_player_imp::OnHeartbeat(tick);
	if (_transform_counter > 0)
	{
		_transform_counter--;
		if (_transform_counter <= 0)
		{
			SKILL::Data data(CAPTURE_FLAG_TRANSFORM_ID);
			_skill.CastRune(data, this, 1,0);
		}
	}
}

bool capture_raid_player_imp::StepMove(const A3DVECTOR &offset)
{
	bool bRst = false;
	if ((bRst = gplayer_imp::StepMove(offset)))
	{
		if (_is_flag_player)
		{
			gplayer* pPlayer = GetParent();
			MSG msg;
			raid_world_manager* pManager = (raid_world_manager*)GetWorldManager();
			BuildMessage( msg,GM_MSG_CAPTURE_SYNC_FLAG_POS, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()),
					pPlayer->ID, pPlayer->pos, _battle_faction);
			gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg );
		}
	}
	return bRst;
}
