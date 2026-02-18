#include <stdio.h>
#include "cs_flow_player.h"
#include "../actsession.h"
#include "../clstab.h"
#include "bg_world_manager.h"
#include <vector>
#include "../skill_filter.h"
#include "../cooldowncfg.h"
#include "../invincible_filter.h"
#include "../gmatrix.h"
#include "../task/taskman.h"
#include <factionlib.h>


DEFINE_SUBSTANCE(cs_flow_player_imp,  bg_player_imp,  CLS_CS_FLOW_PLAYER_IMP)

int cs_flow_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			if(msg.source == _parent->ID)
			{
				break;
			}
			if( 0 == msg.content_length )
			{
				if(_c_kill_cnt < 0)
				{
					_c_kill_cnt = 1;
				}
				else
				{
					_c_kill_cnt ++;
				}
				_kill_count ++;
				if(_max_c_kill_cnt < _c_kill_cnt)
				{
					_max_c_kill_cnt = _c_kill_cnt;
				}

				cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();
				int point = pManager->GetKillBaseScore() + msg.param2;
				if(point < 0) point = 0;
				if(point > 20) point = 20;

				IncScore(point);

				int curr = g_timer.get_systime();
				if(curr - _m_kill_cnt_timeatamp > 10)
				{
					_m_kill_cnt = 0;
				}
				_m_kill_cnt_timeatamp = curr;
				_m_kill_cnt ++;
				if(_max_m_kill_cnt < _m_kill_cnt)
				{
					_max_m_kill_cnt = _m_kill_cnt;
				}

				SendPlayerCSFlowInfo(true);				
				gplayer* pPlayer = GetParent();
				pPlayer->flow_battle_ckill = _c_kill_cnt;
				pPlayer->SetExtraState(gplayer::STATE_FLOW_BATTLE);
				SyncFlowPlayerInfo();

				//判断成就
				object_interface oif(this);
				GetAchievementManager().OnFlowKill(oif, 1, _c_kill_cnt);
				GetAchievementManager().OnFlowKill(oif, 2, _m_kill_cnt);
				GetAchievementManager().OnFlowKillPlayer(oif, msg.param2);
				
				return 0;
			}
		}
		break;

		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			if(0 == msg.content_length)
			{
				_monster_kill_cnt ++;
				cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();	
				int score = pManager->GetKillMonsterScore(msg.param);

				if(IsInTeam() && _team.GetMemberNum() > 0)
				{
					int effect_count = 0;
					int count = _team.GetMemberNum();
					for(int i=0;i<count;i++)
					{
						const player_team::member_entry &ent = _team.GetMember(i);
						if(ent.id.id == _parent->ID.id ||  _parent->pos.squared_distance(ent.pos)  <= 30 * 30) 
						{
							effect_count++;
						}
					}

					if(effect_count >=2 )
					{
						int rs = score / effect_count + score / 15;
						DispatchFlowScore(rs);
						IncScore(rs);
					}
					else if(effect_count == 1)
					{
						IncScore(score);
					}
				}
				else
				{
					IncScore(score);
				}
				SendPlayerCSFlowInfo();
				SyncFlowPlayerInfo();
				return 0;
			}
		}
		break;

		case GM_MSG_FLOW_TEAM_SCORE:
		{
			if(0 == msg.content_length)
			{
				IncScore(msg.param);
				SendPlayerCSFlowInfo();
				SyncFlowPlayerInfo();
			}
		}
		break;
	}

	return bg_player_imp::MessageHandler(msg);	
}

void cs_flow_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsZombie() ) return;
	__PRINTF( "战场处理死亡消息: 玩家 %d 被 %d 杀死了, is_pariah=%d, faction_battle=%d\n", _parent->ID.id, lastattack.id, is_pariah, faction_battle );

	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();	
	if( lastattack.IsPlayerClass() && pManager->IsBattleRunning())
	{
		++_death_count;
		SendTo2<0>( GM_MSG_KILL_PLAYER_IN_BATTLEGROUND, lastattack, pPlayer->ID.id, _c_kill_cnt);
		if(_c_kill_cnt > 0)
		{
			_c_kill_cnt = 0;
		}
		else
		{
			_c_kill_cnt --;
		}
		gplayer* pPlayer = GetParent();
		pPlayer->flow_battle_ckill = _c_kill_cnt;
		pPlayer->SetExtraState(gplayer::STATE_FLOW_BATTLE);
		SendPlayerCSFlowInfo();
		SyncFlowPlayerInfo();

		//发死亡牌子
		/*if(InventoryHasSlot(1))
		{
			DeliverItem(57887, 1, true, 0, ITEM_INIT_TYPE_TASK);
		}
		else
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		}*/
	}

	gplayer_imp::OnDeath( lastattack, is_pariah, true);
	_resurrect_hp_recover = 1.0f;
}

void cs_flow_player_imp::PostLogin(const userlogin_t & user, char trole, char loginflag)
{
	bg_player_imp::PostLogin(user, trole, loginflag);
	//loginflag=7 表示为流水席战场中断线再上线，取回玩家的数据
	if(loginflag == 7)
	{
		cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();
		_reenter = true;
		pManager->GetPlayerCSFlowInfo(_parent->ID.id, _score, _c_kill_cnt, _death_count, _kill_count, _monster_kill_cnt, _max_c_kill_cnt, _max_m_kill_cnt, _battle_result_sent);

		if(_battle_result_sent)
		{
			_timeout = 5;
		}
	}
}

void cs_flow_player_imp::PlayerEnter()
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

void cs_flow_player_imp::PostPlayerEnterWorld()
{
	if(_reenter)
	{
		GetParent()->SetExtraState(gplayer::STATE_FLOW_BATTLE);
		SendPlayerCSFlowInfo();
	}
}

void cs_flow_player_imp::OnHeartbeat( size_t tick )
{
	bg_player_imp::OnHeartbeat(tick);

	++_heartbeat_counter; 

	cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();
	if(!_battle_result_sent && _score >= pManager->GetTargetScore())
	{
		HandleBattleResult();
		_timeout = abase::Rand( pManager->GetKickoutTimeMin(), pManager->GetKickoutTimeMax() );
		__PRINTF( "玩家%d达到流水席目标分数%d, %d 秒后踢出玩家\n", _parent->ID.id, _score, _timeout );
		GLog::log(GLOG_INFO, "玩家达到流水席目标分数%d, 被踢出去了: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d", _score, _parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );				
	}
}

void cs_flow_player_imp::HandleBattleResult()
{
	if(_battle_result_sent)
	{
		return;
	}
	_battle_result_sent = true;
	object_interface oif(this);
	cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();
	if(_monster_kill_cnt <= 0 && _score >= pManager->GetTargetScore())
	{
		GetAchievementManager().OnFlowDone(oif, 1);
	}
	if(_kill_count <= 0 && _score >= pManager->GetTargetScore())
	{
		GetAchievementManager().OnFlowDone(oif, 2);
	}
	if(_death_count <= 0 &&  _score >= pManager->GetTargetScore())
	{
		GetAchievementManager().OnFlowDone(oif, 3);
	}

	DATA_TYPE dt;
	BATTLE_201304_CONFIG* config = (BATTLE_201304_CONFIG*)gmatrix::GetDataMan().get_data_ptr(pManager->GetBattleTemplateId(), ID_SPACE_CONFIG, dt);
	if(!config || dt != DT_BATTLE_201304_CONFIG)
	{
		__PRINTF("Failed to load BATTLE_201304_CONFIG tid=%d\n", pManager->GetBattleTemplateId());
		return; 
	}
	
	PlayerTaskInterface tf(this);
	if(_score >= pManager->GetTargetScore())
	{
		
		short cls = GetParent()->GetClass();
		if(((cls >= 0 && cls <= 29) || (cls >= 64 && cls <= 68)) && OnTaskCheckDeliver(&tf, config->taskid_success_human, 0))
		{
			GLog::log(GLOG_INFO, "流水席战场人族玩家领取完成奖励任务%d, score=%d", config->taskid_success_human, _score);
		}
		else if(((cls >= 33 && cls <= 60) || (cls >= 96 && cls <= 100)) && OnTaskCheckDeliver(&tf, config->taskid_success_wild, 0))
		{
			GLog::log(GLOG_INFO, "流水席战场神族玩家领取完成奖励任务%d, score=%d", config->taskid_success_wild, _score);
		}
		else if((cls >= 102 && cls <= 112) && OnTaskCheckDeliver(&tf, config->taskid_success_human, 0))
		{
			GLog::log(GLOG_INFO, "流水席战场天脉玩家领取完成奖励任务%d, score=%d", config->taskid_success_human, _score);
		}
	}
	SyncFlowPlayerInfo();

}

void cs_flow_player_imp::HandleDeath()
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

void cs_flow_player_imp::SyncBattleInfo()
{
	gplayer* pPlayer = GetParent();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	MSG msg_send;
	player_battle_info info(_battle_faction, GetBattleScore(), _kill_count, _death_count, 0, 0);
	BuildMessage( msg_send,GM_MSG_SYNC_BATTLE_INFO , XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), pPlayer->ID, pPlayer->pos, 0, &info, sizeof(info));
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );
}

void cs_flow_player_imp::GetBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	std::vector<S2C::CMD::flow_battle_info::player_info_in_flow_battle> info;

	if( pManager->GetCSFlowBattleInfo( _battle_info_seq, info ) && info.size() > 0)
	{
		_runner->flow_battle_info((const char*)&info[0], sizeof(S2C::CMD::flow_battle_info::player_info_in_flow_battle) * info.size() );
	}
}

void cs_flow_player_imp::SendPlayerCSFlowInfo(bool mKillChange)
{
	_runner->cs_flow_player_info(_score, _c_kill_cnt, mKillChange, _m_kill_cnt, _kill_count, _death_count, _max_c_kill_cnt, _max_m_kill_cnt);
}	

bool cs_flow_player_imp::UseItem(item_list & inv, int inv_index, int where, int item_type)
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

bool cs_flow_player_imp::UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size)
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

void cs_flow_player_imp::PlayerFlowBattleLeave()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return;
	}
	GNET::SendOpenBanquetLeave(_parent->ID.id, gmatrix::GetServerIndex(), pManager->GetWorldTag());
}

void cs_flow_player_imp::SetLogoutDisconnect()
{
	_disconnected = true;
}

void cs_flow_player_imp::OnTimeOutKickout()
{
	PlayerFlowBattleLeave();
}

void cs_flow_player_imp::DispatchFlowScore(int score)
{
	gplayer * pPlayer = (gplayer*)_parent;
	if(!IsInTeam()) return; 
	if(score <= 0) return;

	int count = _team.GetMemberNum();
	for(int i=0;i<count;i++)
	{
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.id.id != _parent->ID.id && _parent->pos.squared_distance(ent.pos)  <= 30 * 30) 
		{
			MSG msg;
			BuildMessage( msg, GM_MSG_FLOW_TEAM_SCORE, XID(GM_TYPE_PLAYER, ent.id.id), pPlayer->ID, pPlayer->pos, score);
			gmatrix::SendMessage(msg);
		}
	}
}

void cs_flow_player_imp::IncScore(int sc)
{
	cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();
	int target_score = pManager->GetTargetScore(); 
	int award_score = int(target_score * 0.05);

	//发放参与奖
	if(_score < award_score && _score + sc >= award_score)
	{
		int task_id = 28559;
		PlayerTaskInterface tf(this);
		OnTaskCheckDeliver(&tf, task_id, 0); 
	}	
	_score += sc;
}

void cs_flow_player_imp::SyncFlowPlayerInfo()
{
	cs_flow_world_manager* pManager = (cs_flow_world_manager*)_plane->GetWorldManager();
	gplayer * pPlayer = (gplayer*)_parent;
	//World Manager同步玩家信息
	MSG msg_send;
	msg_sync_flow_info c;
	c.score = _score;
	c.c_kill_cnt = _c_kill_cnt;
	c.kill_cnt = _kill_count;
	c.death_cnt = _death_count;
	c.monster_kill_cnt = _monster_kill_cnt;
	c.max_ckill_cnt = _max_c_kill_cnt;
	c.max_mkill_cnt = _max_m_kill_cnt;
	c.battle_result_sent = _battle_result_sent;
	BuildMessage( msg_send, GM_MSG_SYNC_CSFLOW_PLAYER_INFO, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), pPlayer->ID, pPlayer->pos, -1, &c, sizeof(c));
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );
}


