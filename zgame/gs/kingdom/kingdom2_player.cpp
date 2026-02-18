#include "kingdom2_player.h"
#include "kingdom2_world_manager.h"
#include "../clstab.h"
#include "../userlogin.h"
#include <factionlib.h>


DEFINE_SUBSTANCE( kingdom2_player_imp, gplayer_imp, CLS_KINGDOM2_PLAYER_IMP )

void kingdom2_player_imp::PlayerEnterWorld()
{
	kingdom2_world_manager * pManager = (kingdom2_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	pManager->PlayerEnter(pPlayer, _battle_faction);
	EnableFreePVP(true);
	gplayer_imp::PlayerEnterWorld();


	int end_time = pManager->GetBattleEndstamp();
	int defender_mafia_id = 0; 
	std::vector<int> attacker_mafia_list;

	_runner->enter_kingdom(_battle_faction, 2, 1, end_time, defender_mafia_id, attacker_mafia_list); 
	GLog::log(GLOG_INFO, "国王战玩家进入战场: roleid=%d, line_id=%d,  battle_world_tag=%d", _parent->ID.id, gmatrix::Instance()->GetServerIndex(), pManager->GetWorldTag()); 
}


void kingdom2_player_imp::PlayerLeaveWorld()
{
	EnableFreePVP( false );
	
	kingdom2_world_manager* pManager = (kingdom2_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	pManager->PlayerLeave(pPlayer,_battle_faction);
	gplayer_imp::PlayerLeaveWorld();
	
	_runner->leave_kingdom(2);
	GLog::log(GLOG_INFO, "国王战玩家离开战场: roleid=%d, line_id=%d,  battle_world_tag=%d", _parent->ID.id, gmatrix::Instance()->GetServerIndex(), pManager->GetWorldTag()); 
}


void kingdom2_player_imp::PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos)
{
	__PRINTF("国王战战场中不应该出现切换场景\n");
	ASSERT(false);
}

void kingdom2_player_imp::PlayerLeaveMap()
{
	__PRINTF("国王战战场中不应该出现切换场景\n");
	ASSERT(false);
}

int kingdom2_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			return 0;
		}
		break;
	}
	return gplayer_imp::MessageHandler( msg );
}

void kingdom2_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{                       
	//设置登出使用的原坐标
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;

}

void kingdom2_player_imp::GetLogoutPos(int& world_tag,A3DVECTOR& pos)
{
	world_tag = _logout_tag;
	pos = _logout_pos;
}

void kingdom2_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	kingdom2_world_manager* pManager = (kingdom2_world_manager*)_plane->GetWorldManager();
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
			LeaveKingdom();
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
			GLog::log(GLOG_INFO, "国王战玩家退出异常，被强制退出: userid= %d, status = %d, kick_timeout = %d,  battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());

		}
		else if(_kick_timeout == 45)
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			GLog::log(GLOG_INFO, "国王战玩家退出异常，被强制断线: userid= %d, status = %d, kick_timeout = %d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
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
		GLog::log(GLOG_INFO, "国王战战场非运行状态, 玩家从战场退出: userid= %d, status = %d, timeout = %d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return;
	}
	
	UpdateKeyNPCInfo();

}

void kingdom2_player_imp::UpdateKeyNPCInfo()
{
	kingdom2_world_manager* pManager = (kingdom2_world_manager*)_plane->GetWorldManager();
	if(pManager->GetBattleStatus() != kingdom2_world_manager::BS_RUNNING) return;

	int cur_hp = 0;
	int max_hp = 0;
	int hp_add = 0;
	int hp_dec = 0;
	pManager->OnGetKeyNPCInfo(cur_hp, max_hp, hp_add, hp_dec);
	_runner->kingdom_key_npc_info(cur_hp, max_hp, hp_add, hp_dec);
}

void kingdom2_player_imp::OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time)
{
	gplayer_imp::OnDeath(lastattack,is_pariah,true,false,10);
}

void kingdom2_player_imp::PlayerLeaveKingdom()
{
	if(!_pstate.CanLogout() || IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}

	LeaveKingdom();

}

void kingdom2_player_imp::LeaveKingdom()
{
	if(_is_leaving) return;
	_is_leaving = true;
	GNET::SendKingdomLeave(_parent->ID.id, 2, GetWorldTag(), _logout_tag, _logout_pos.x, _logout_pos.y, _logout_pos.z); 
	_kick_timeout = 0;
}

