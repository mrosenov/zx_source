#include <stdio.h>
#include "cs_player.h"
#include "../actsession.h"
#include "../clstab.h"
#include "bg_world_manager.h"
#include <vector>
#include "../skill_filter.h"
#include "../cooldowncfg.h"
#include "../invincible_filter.h"


DEFINE_SUBSTANCE(cs_player_imp,  bg_player_imp,  CLS_CS_PLAYER_IMP )

int cs_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length )
			{
				++_kill_count;
				gplayer* pPlayer = GetParent();
				bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
				MSG msg_send;
				BuildMessage( msg_send, GM_MSG_MODIFIY_BATTLE_KILL, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
					pPlayer->ID, pPlayer->pos, _kill_count );
				gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );
				return 0;
			}
		}
		break;

		case GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER:
		{
			_kill_count += msg.param;
			_death_count += msg.param2;
		}
	}

	return bg_player_imp::MessageHandler(msg);	
}

void cs_player_imp::PlayerEnter()
{
	if(CheckGMPrivilege() )
	{
		_runner->set_invisible(true);	
		gplayer * pPlayer = GetParent();
		pPlayer->invisible = true;
		_runner->toggle_invisible(0);
	}
	
	if(_battle_faction == BF_ATTACKER || _battle_faction == BF_DEFENDER)	
	{
		bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
		//加无敌filter
		int timeout = g_timer.get_systime() - pManager->GetBattleStartstamp();
		if(timeout > 0 && timeout < INVICIBLE_TIME)
		{
			_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, INVICIBLE_TIME - timeout));
		}
	}

	bg_player_imp::PlayerEnter();
}

void cs_player_imp::OnHeartbeat( size_t tick )
{
	bg_player_imp::OnHeartbeat(tick);

	++_heartbeat_counter; 
	GetCrossServerBattleInfo();
}

void cs_player_imp::HandleBattleResult()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// 判断是否需要发送结果,给奖励
	if( !_battle_result_sent && pManager->GetBattleResult() )
	{
		unsigned char attacker_building_left;
		unsigned char defender_building_left;
		std::vector<S2C::CMD::battle_info::player_info_in_battle> info;
		std::vector<player_off_line_in_cs_battle> vec_info;

		if( pManager->GetCommonBattleInfo( _battle_info_seq, attacker_building_left, defender_building_left, info ) )
		{
			if( info.size() )
			{
				_runner->battle_info( attacker_building_left, defender_building_left,
						(const char*)&info[0], sizeof(S2C::CMD::battle_info::player_info_in_battle) * info.size() );
				if (pManager->GetCrossServerOfflineInfo(vec_info) && vec_info.size())
				{
					_runner->crossserver_off_line_score_info(vec_info);	
				}
			}
		}
		_runner->battle_result(pManager->GetBattleResult(), pManager->GetAttackerScore(), pManager->GetDefenderScore());
		_battle_result_sent = true;
	}
}

const A3DVECTOR& cs_player_imp::GetLogoutPos( int &world_tag )
{
	if (BF_ATTACKER == _battle_faction)
	{
		world_tag = 317;
		A3DVECTOR pos(-75,36,243);
		_old_pos = pos;
	}
	else if (BF_DEFENDER == _battle_faction)
	{
		world_tag = 318;
		A3DVECTOR pos(103,36,352);
		_old_pos = pos;
	}
	return _old_pos;
}

void cs_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsZombie() ) return;
	__PRINTF( "战场处理死亡消息: 玩家 %d 被 %d 杀死了, is_pariah=%d, faction_battle=%d\n",
			_parent->ID.id, lastattack.id, is_pariah, faction_battle );
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	
	// 杀人被杀记录逻辑
	if( lastattack.IsPlayerClass() && pManager->IsBattleRunning())
	{
		// 如果是被人杀了，并且战场在进行中
		++_death_count;
		SendTo<0>( GM_MSG_KILL_PLAYER_IN_BATTLEGROUND, lastattack, 0);
		MSG msg;
		BuildMessage( msg, GM_MSG_MODIFIY_BATTLE_DEATH, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, _death_count );
		gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg );

	}

	gplayer_imp::OnDeath( lastattack, is_pariah, true);
	_resurrect_hp_recover = 1.0f;
}

void cs_player_imp::HandleDeath()
{
	if(IsDead())
	{
		++_revive_counter;
		if(_revive_counter >= REVIVE_FORCE_COUNTER)
		{
			//强制复活
			_revive_counter = 0;
			ClearSession();
			session_resurrect_in_town *pSession= new session_resurrect_in_town(this);
			pSession->SetExpReduce(0);
			AddStartSession(pSession);
		}

	}
}	

void cs_player_imp::SyncBattleInfo()
{
	gplayer* pPlayer = GetParent();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	MSG msg_send;
	player_battle_info info(_battle_faction, GetBattleScore(), _kill_count, _death_count, 0, 0);
	BuildMessage( msg_send,GM_MSG_SYNC_BATTLE_INFO , XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, 0, &info, sizeof(info) );
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );
}

void cs_player_imp::GetBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	unsigned char attacker_building_left;
	unsigned char defender_building_left;
	std::vector<S2C::CMD::battle_info::player_info_in_battle> info;
	std::vector<player_off_line_in_cs_battle> vec_info;

	__PRINTF("GetCommonBattleInfo _battle_info_seq = %d\n",_battle_info_seq);
	if( pManager->GetCommonBattleInfo( _battle_info_seq, attacker_building_left, defender_building_left, info ) )
	{
		__PRINTF("GetCommonBattleInfo Get _battle_info_seq = %d,info.size=%d\n",_battle_info_seq,info.size());
		if( info.size() )
		{
			_runner->battle_info( attacker_building_left, defender_building_left,
				(const char*)&info[0], sizeof(S2C::CMD::battle_info::player_info_in_battle) * info.size() );
			if (pManager->GetCrossServerOfflineInfo(vec_info) && vec_info.size())
			{
				_runner->crossserver_off_line_score_info(vec_info);	
			}
		}
		if (pManager->GetBattleResult())
		{
			_runner->battle_result(pManager->GetBattleResult(), pManager->GetAttackerScore(), pManager->GetDefenderScore());
		}
	}
}

void cs_player_imp::GetCrossServerBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	S2C::CMD::crossserver_battle_info info;

	int seq = 1;	
	if(pManager->GetCrossServerBattleInfo(seq,info))
	{
		info.kill_count = _kill_count;
		info.death_count = _death_count;
		_runner->crossserver_battle_info(info);
					
	}
}

void cs_player_imp::DumpBattleSelfInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	S2C::CMD::crossserver_battle_info info;
	
	int seq = 1;
	if(pManager->GetCrossServerBattleInfo(seq,info))
	{
		char buf[512];
		sprintf(buf, "attacker_score =%d, defender_score = %d, kill_count =%d, death_count =%d",
				info.attacker_score, info.defender_score, _kill_count, _death_count);	
		this->Say(buf);

	}
}
	

void cs_player_imp::DumpBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	unsigned char attacker_building_left;
	unsigned char defender_building_left;
	std::vector<S2C::CMD::battle_info::player_info_in_battle> info;
	std::vector<player_off_line_in_cs_battle> vec_info;
	if( pManager->GetCommonBattleInfo( _battle_info_seq, attacker_building_left, defender_building_left, info ) )
	{
		for(size_t i = 0; i < info.size(); ++i)
		{
			char buf[512];
			sprintf(buf, "id=%d, faction=%d, kill=%d, death=%d",
				info[i].id, info[i].battle_faction, info[i].kill, info[i].death);	
			this->Say(buf);
		}	
	}
	if (pManager->GetCrossServerOfflineInfo(vec_info))
	{
		for (size_t i = 0; i < vec_info.size(); ++i)
		{
			char buf[512];
			player_off_line_in_cs_battle tmp = vec_info[i];
			if (tmp.battle_faction == BF_ATTACKER)
			sprintf(buf,"attacker_list off_line _timestamp = %d,off_line_count = %d,off_line_score = %d"
					,tmp._timestamp,tmp.off_line_count,tmp.off_line_score);
			else
			sprintf(buf,"defender_list off_line _timestamp = %d,off_line_count = %d,off_line_score = %d"
					,tmp._timestamp,tmp.off_line_count,tmp.off_line_score);
			this->Say(buf);
		}
	}

}

bool cs_player_imp::UseItem(item_list & inv, int inv_index, int where, int item_type)
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return false;
	}

	if( pManager->IsForbiddenItem(item_type) )
	{
		return false;
	}

	item & it = inv[inv_index];
	bool bRst = it.IsBroadcastUseMsg();
	int count = 0;
	bool dolog = it.NeedUseLog();
	int guid1 = it.guid.guid1;
	int guid2 = it.guid.guid2;
	int rst = inv.UseItem(inv_index,this,count);
	if(rst >= 0)
	{
		ASSERT(rst == item_type);
		_runner->use_item(where,inv_index, item_type,count);
		if(bRst) _runner->use_item(item_type);
		if(count > 0 )
		{
			UseItemLog(item_type, guid1,guid2, count);
			if(dolog) GLog::log(GLOG_INFO,"用户%d使用%d个物品%d", _parent->ID.id, count, item_type);
		}

		
		return true;
	}
	return false;
}

bool cs_player_imp::UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size)
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return false;
	}

	if( pManager->IsForbiddenItem(item_type) )
	{
		return false;
	}

	item & it = inv[inv_index];
	if(it.type != item_type) return false;
	bool bBroadcast = it.IsBroadcastUseMsg();
	bool bBCArg = it.IsBroadcastArgUseMsg();
	int count = 0;
	bool dolog = it.NeedUseLog();
	int guid1 = it.guid.guid1;
	int guid2 = it.guid.guid2;
	int rst = inv.UseItemWithArg(inv_index,this,count,arg, arg_size);
	if(rst >= 0)
	{
		_runner->use_item(where,inv_index, item_type,count,arg,arg_size);
		if(bBroadcast) 
		{
			if(bBCArg)
			{
				_runner->use_item(item_type, arg, arg_size);
			}
			else
			{
				_runner->use_item(item_type);
			}
		}
		if(count > 0 )
		{
			UseItemLog(item_type, guid1,guid2, count);
			if(dolog) GLog::log(GLOG_INFO,"用户%d使用%d个物品%d", _parent->ID.id, count, item_type);
		}
		
		return true;
	}
	return false;
}




//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_SUBSTANCE(cs_melee_player_imp,  cs_player_imp,  CLS_CS_MELEE_PLAYER_IMP )

void cs_melee_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction = 0xFFFFFFFF;
}

void cs_melee_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	//可以给所有人加诅咒和祝福
	if(enchant.helpful)
	{
		enchant.target_faction = 0;

	}
	else
	{
		enchant.target_faction = 0xFFFFFFFF;
	}
}
	

int cs_melee_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		//禁止组队
		case GM_MSG_TEAM_INVITE:
		case GM_MSG_TEAM_AGREE_INVITE:
		case GM_MSG_TEAM_REJECT_INVITE:
		case GM_MSG_JOIN_TEAM:
		case GM_MSG_LEADER_UPDATE_MEMBER:
		
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
	}

	return cs_player_imp::MessageHandler(msg);	
}

