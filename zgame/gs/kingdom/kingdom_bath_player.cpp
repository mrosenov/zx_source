#include "kingdom_bath_player.h"
#include "kingdom_bath_world_manager.h"
#include "../clstab.h"
#include "kingdom_bath_filter.h"
#include "../userlogin.h"
#include "../cooldowncfg.h"
#include "../actsession.h"
#include <factionlib.h>
#include "gsp_if.h"

DEFINE_SUBSTANCE( kingdom_bath_player_imp, gplayer_imp, CLS_KINGDOM_BATH_PLAYER_IMP )

void kingdom_bath_player_imp::PlayerEnterWorld()
{
	ASSERT(false);
}

void kingdom_bath_player_imp::PlayerLeaveWorld()
{
	EnableFreePVP( false );
	
	kingdom_bath_world_manager* pManager = (kingdom_bath_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	pManager->PlayerLeave(pPlayer,_battle_faction);
	gplayer_imp::PlayerLeaveWorld();
	
	_runner->leave_kingdom(3);
	GLog::log(GLOG_INFO, "玩家离开澡堂: roleid=%d, line_id=%d,  world_tag=%d", _parent->ID.id, gmatrix::Instance()->GetServerIndex(), pManager->GetWorldTag()); 
}

void kingdom_bath_player_imp::PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos )
{
	_old_world_tag = old_tag;
	_old_pos = old_pos;

	// 这个一定要在gplayer_imp::PlayerEnterMap();之前完成
	// 要在PlayerEnterMap之前确定进入点
	gplayer_imp::PlayerEnterMap();
	gplayer* pPlayer = GetParent();

	EnableFreePVP( true );

	_battle_faction = 1; 
	pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
	_attack_faction = FACTION_BATTLEDEFENCE;
	_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;

	A3DVECTOR temp(-339.397, 298.855, 352.838);
//	kingdom_bath_world_manager* pManager = (kingdom_bath_world_manager*)_plane->GetWorldManager();
//	temp.y =  pManager->GetTerrain().GetHeightAt(temp.x,temp.z);
	_parent->pos = temp;

	_bath_count = gmatrix::GetBathCount(_parent->ID.id);
	if(_bath_count == -1)
	{
		_bath_count = 0;
		GMSV::SendSyncBathCount(_parent->ID.id, _bath_count);	//同步信息表明这个玩家已经进入过
	}
	
	int remain_bath_count = MAX_BATH_INVITE_COUNT - _bath_count;
	if(remain_bath_count < 0) remain_bath_count = 0;
	_runner->enter_bath_world(remain_bath_count);

	_commander->DenyCmd(controller::CMD_SKILL);
	_commander->DenyCmd(controller::CMD_ATTACK);

	if(IsRenMa() && GetShape() != 8)
	{
		ChangeShape(8);
		_runner->change_shape(8);
	}
	
}

void kingdom_bath_player_imp::PlayerLeaveMap()
{
	EnableFreePVP( false );
	gplayer* pPlayer = GetParent();

	pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_OFFENSE );
	_battle_faction = 0;
	
	gplayer_imp::PlayerLeaveMap();
	_runner->leave_bath_world();

	_commander->AllowCmd(controller::CMD_SKILL);
	_commander->AllowCmd(controller::CMD_ATTACK);

	if(_filters.IsFilterExist(FILTER_INDEX_KINGDOM_BATH))
	{
		_filters.RemoveFilter(FILTER_INDEX_KINGDOM_BATH);
	}

	if(IsRenMa() && GetShape() == 8)
	{
		ChangeShape(0);
		_runner->change_shape(0);
	}
}

int kingdom_bath_player_imp::GetFaction()
{
	return _defense_faction;
}

int kingdom_bath_player_imp::GetEnemyFaction()
{
	return _attack_faction;
}

int kingdom_bath_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_ATTACK:
		{
		}
		return 0;

		case GM_MSG_ENCHANT:
		{
		}
		return 0;

		case GM_MSG_ENCHANT_ZOMBIE:
		{
		}
		return 0;

		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
		}
		return 0;
	}
	return gplayer_imp::MessageHandler( msg );
}


void kingdom_bath_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	kingdom_bath_world_manager* pManager = (kingdom_bath_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// 已经被清除了,断线了等待处理中
		return;
	}
	if( _timeout > 0 )
	{
		// 已经在踢出准备状态下，_timeout后一定会踢出，不会在做后面面判断
		--_timeout;
		if( _timeout <= 0  && _is_leaving == false)
		{
			if(_pstate.IsBindState())
			{
				LeaveBindState();
			}
			LeaveBathWorld();
		}
		return;
	}

	if(_is_leaving)
	{
		++_kick_timeout;
		if(_kick_timeout == 30)
		{
			LeaveAbnormalState();
			PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			GLog::log(GLOG_INFO, "澡堂玩家退出异常，被强制退出: userid= %d, status = %d, kick_timeout = %d,  battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());

		}
		else if(_kick_timeout == 45)
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			GLog::log(GLOG_INFO, "澡堂玩家被强制断线: userid= %d, status = %d, kick_timeout = %d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		}
		return;
	}

	
	// 以下进行判断是否需要踢出玩家
	if( CheckGMPrivilege() )
	{
		// gm不踢
		return;
	}
	if( !pManager->IsBattleRunning() )
	{
		// 非正常状态
		// xx秒后踢出该玩家，发送消息
		_timeout = abase::Rand( pManager->GetKickoutTimeMin(), pManager->GetKickoutTimeMax() );
		GLog::log(GLOG_INFO, "澡堂非运行状态, 玩家退出: userid= %d, status = %d, timeout = %d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return;
	}
}

const A3DVECTOR& kingdom_bath_player_imp::GetLogoutPos( int &world_tag )
{
	world_tag = _old_world_tag;
	return _old_pos;
}

void kingdom_bath_player_imp::LeaveBathWorld()
{
	if(_is_leaving) return;
	_is_leaving = true;

	if(_filters.IsFilterExist(FILTER_INDEX_KINGDOM_BATH))
	{
		_filters.RemoveFilter(FILTER_INDEX_KINGDOM_BATH);
	}

	kingdom_bath_world_manager* pManager = (kingdom_bath_world_manager*)_plane->GetWorldManager();
	int world_tag = -1;
	A3DVECTOR pos(0,0,0);
	pManager->GetLogoutPos(this,world_tag,pos);
	LongJump(pos,world_tag);
}

void kingdom_bath_player_imp::PlayerLeaveKingdom()
{
	LeaveBathWorld();
}

void kingdom_bath_player_imp::PlayerBathInvite(const XID & target)
{
	if(_bath_count > MAX_BATH_INVITE_COUNT) 
	{
		_runner->error_message(S2C::ERR_BATH_MAX_LIMIT);
		return;
	}
	if(!CheckCoolDown(COOLDOWN_INDEX_BATH))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	kingdom_bath_world_manager* pManager = (kingdom_bath_world_manager*)_plane->GetWorldManager();
	if( !pManager->IsBattleRunning() ) return;

	
	_link_bath_player.PlayerLinkInvite(this,target);
}


void kingdom_bath_player_imp::PlayerBathInviteReply(const XID & target, int param)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_BATH))
	{
		_runner->error_message(S2C::ERR_BATH_IS_COOLDOWN);
		return;
	}

	_link_bath_player.PlayerLinkInvReply(this,target,param);
}


void kingdom_bath_player_imp::OnBathStart(bool is_leader)
{
	SetCoolDown(COOLDOWN_INDEX_BATH, 180000);
	_bath_count++;
	GMSV::SendSyncBathCount(_parent->ID.id, _bath_count);	//同步信息表明这个玩家已经进入过

	int award_period = player_template::GetBathAwardPeriod();
	_commander->DenyCmd(controller::CMD_MOVE);
	_filters.AddFilter(new kingdom_bath_filter(this,FILTER_INDEX_KINGDOM_BATH, 120, is_leader,award_period, _bath_count));
}

void kingdom_bath_player_imp::OnBathEnd(bool is_leader)
{
	_commander->AllowCmd(controller::CMD_MOVE);
	if(!_pstate.IsBindState()) return;

	if(is_leader)
	{
		_link_bath_player.PlayerLinkCancel(this);
	}	
}


