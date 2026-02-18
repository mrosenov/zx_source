#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "bg_player.h"
#include "../usermsg.h"
#include "../actsession.h"
#include "../userlogin.h"
#include "../playertemplate.h"
#include "../serviceprovider.h"
#include <common/protocol_imp.h>
#include "../task/taskman.h"
#include "../playerstall.h"
#include "../pvplimit_filter.h"
#include <glog.h>
#include "../pathfinding/pathfinding.h"
#include "../player_mode.h"
#include "../cooldowncfg.h"
#include "../template/globaldataman.h"
#include "../petnpc.h"
#include "../item_manager.h"
#include "../netmsg.h"
#include "../mount_filter.h"
#include "bg_world_manager.h"
#include "../faction.h"
#include "../invincible_filter.h"

DEFINE_SUBSTANCE( bg_player_imp, gplayer_imp, CLS_BG_PLAYER_IMP )

void bg_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void bg_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}
	
int bg_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_ATTACK:
		{
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
			XID attacker = ack_msg.ainfo.attacker;
			//这里无需区分玩家和NPC
			//自己不允许攻击
			if(attacker.id == _parent->ID.id) return 0;
			if(!(ack_msg.target_faction  & (GetFaction())))
			{
				//派系不正确则返回
				return 0;
			}
			if( ack_msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return 0;
			ack_msg.is_invader =  false;
			//试着选择对象
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(msg,&ack_msg);
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
				if(!(ech_msg.target_faction  & (GetFaction())))
				{
					return 0;
				}
				if( ech_msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return 0;
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
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;
		case GM_MSG_ENCHANT_ZOMBIE:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if( !ech_msg.helpful )
			{
				return 0;
			}
			if( !(ech_msg.attacker_faction & (GetFaction())))
			{
				return 0;
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			return 0;
		}
		break;
		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			return 0;
		}
		break;
		case GM_MSG_BATTLE_INFO_CHANGE:
		{
			return 0;
		}
		break;
	}
	return gplayer_imp::MessageHandler( msg );
}

void bg_player_imp::OnTimeOutKickout()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	int world_tag = -1;
	A3DVECTOR pos(0,0,0);
	pManager->GetLogoutPos(this,world_tag,pos);
	LongJump(pos,world_tag);
}

void bg_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// 已经被清除了,断线了等待处理中
		return;
	}
	if( _timeout >= 0 )
	{
		// 已经在踢出准备状态下，_timeout后一定会踢出，不会在做后面面判断
		--_timeout;
		if( _timeout <= 0 )
		{
			if(_pstate.IsMarketState())
			{
				CancelPersonalMarket();
			}
			else if(_pstate.IsBindState())
			{
				LeaveBindState();
			}
			_old_pos.y += 0.5f;
			if(!_parent->b_disconnect)
			{
				if(_pstate.IsTradeState())
				{
					LostConnection();
				}
				else
				{
					OnTimeOutKickout();
				}
			}
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
		// 判断是否需要发送结果,给奖励
		if(!_battle_result_sent) HandleBattleResult();

		// 非正常状态
		// xx秒后踢出该玩家，发送消息
		_timeout = abase::Rand( pManager->GetKickoutTimeMin(), pManager->GetKickoutTimeMax() );
		__PRINTF( "非正常战场状态%d, %d 秒后踢出玩家\n", pManager->GetBattleStatus(), _timeout );
		GLog::log(GLOG_INFO, "非正常战场状态, 玩家在战场被踢出去了: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return;
	}

	// 判断是否是战场里面合法的玩家
	if(!ValidPlayer()) return;
	
	// 处理复活
	HandleDeath();
}


bool bg_player_imp::ValidPlayer()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// 对于级别的判断，级别不符合踢出去
	if( _basic.level < pManager->GetLevelMin() || _basic.level > pManager->GetLevelMax() )
	{
		// 非正常级别玩家进入战场了
		// 3秒后踢出该玩家，发送消息
		_timeout = 3;
		__PRINTF( "玩家级别不在范围内 %d 秒后踢出玩家 %d, 级别:%d, 需求级别范围 %d - %d\n", 
			_timeout, _parent->ID.id, _basic.level, pManager->GetLevelMin(), pManager->GetLevelMax() );
		GLog::log(GLOG_INFO, "玩家在战场因为等级不符合被踢出去了: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}

	int rebornCnt = GetRebornCount();
	//飞升玩家才能参加的战场
	if(pManager->IsRebornBattle() && rebornCnt != 1)
	{
		_timeout = 3;
		//普通玩家不能进入飞升战场
		GLog::log(GLOG_INFO, "普通玩家在飞升战场被踢出: userid=%d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id,	pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}
	//普通玩家才能参加的战场
	else if(pManager->IsNormalBattle()&& rebornCnt != 0)
	{
		//飞升玩家不能进入普通战场
		_timeout = 3;
		GLog::log(GLOG_INFO, "飞升玩家在普通战场被踢出去了: userid=%d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id,	pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}
	return true;

}

void bg_player_imp::PostLogin(const userlogin_t & user, char trole, char loginflag)
{
	//只有跨服上的战场，player才能直接login进来
	ASSERT(gmatrix::IsZoneServer());
	gplayer_imp::PostLogin(user, trole, loginflag);
	_battle_faction = trole;
}

void bg_player_imp::PlayerEnterWorld()
{
	if(gmatrix::IsZoneServer())
	{
		__PRINTF( "玩家 %d 跨服战场 %d 上线啦!!!\n", _parent->ID.id, _plane->GetTag() );
		gplayer* pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);

		PlayerEnter();
		gplayer_imp::PlayerEnterWorld();

		PostPlayerEnterWorld();

		_old_world_tag = 615;
		A3DVECTOR pos(-398, 175, 138);
		_old_pos = pos;

		bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
		_runner->enter_battleground( _battle_faction, (unsigned char)(pManager->GetBattleType()), pManager->GetBattleID(), pManager->GetBattleEndstamp() );

		return;
	}
	// 战场中不应该出现直接进入的情况
	ASSERT( false );
}

void bg_player_imp::PlayerLeaveWorld()
{
	__PRINTF( "玩家 %d 从战场地图 %d 下线啦!!!\n", _parent->ID.id, _plane->GetTag() );
	PlayerLeave();
	gplayer_imp::PlayerLeaveWorld();
}

void bg_player_imp::PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos )
{
	__PRINTF( "玩家 %d 进入战场地图 %d 啦!!!\n", _parent->ID.id, _plane->GetTag() );

	if(gmatrix::IsZoneServer())
	{
		_old_world_tag = 615;
		A3DVECTOR pos(-398, 175, 138);
		_old_pos = pos;

		gplayer* pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);
	}
	else
	{
		//玩家有可能从一些特殊地图进入战场，比如从庙会，庙会本身不能存盘
		world_manager * manager = gmatrix::Instance()->FindWorld(old_tag);
		if(manager->GetWorldLimit().savepoint && manager->GetSavePoint().tag > 0)
		{
			_old_world_tag = manager->GetSavePoint().tag;
			_old_pos = manager->GetSavePoint().pos;
		}
		else
		{
			_old_world_tag = old_tag;
			_old_pos = old_pos;
		}
	}

	// 这个一定要在gplayer_imp::PlayerEnterMap();之前完成
	// 要在PlayerEnterMap之前确定进入点
	gplayer_imp::PlayerEnterMap();
	//PlayerEnter()要在PlayerEnterMap之后调用，因为PlayerEnterMap里会把session里的imp换成新的
	PlayerEnter();
}

void bg_player_imp::PlayerLeaveMap()
{
	__PRINTF( "玩家 %d 离开战场地图 %d 啦!!!\n", _parent->ID.id, _plane->GetTag() );
	PlayerLeave();
	gplayer_imp::PlayerLeaveMap();
}

int bg_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int bg_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

void bg_player_imp::PlayerEnter()
{
	EnableFreePVP( true );
	//回收战场专用物品
	RecycleBattleItem();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	if( BF_ATTACKER == _battle_faction )
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		_attack_faction = FACTION_BATTLEDEFENCE;
		_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
	}
	else if( BF_DEFENDER == _battle_faction ) 
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
		_attack_faction = FACTION_BATTLEOFFENSE;
		_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
	}
	else if( BF_VISITOR == _battle_faction ) 
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_VISITOR);
		SetMeleeSealMode(true);
		SetDietSealMode(true);
		SetSilentSealMode(true);
		_attack_faction = 0;
		_defense_faction = 0;
		_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, 7200));
	}
	else if( BF_MELEE == _battle_faction ) 
	{
		_attack_faction = 0;
		_defense_faction = 0xFFFFFFFF;
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}

	A3DVECTOR temp(0,0,0);
	pManager->GetRandomEntryPos(temp,_battle_faction);
	temp.y += 3.0f;
	_parent->pos = temp;
	pManager->PlayerEnter( pPlayer, _battle_faction );
	__PRINTF("玩家登陆战场坐标： world_tag=%d, pos(%f, %f, %f)\n", pManager->GetWorldTag(), temp.x, temp.y, temp.z);
	// 通知客户端
	_runner->enter_battleground( _battle_faction, (unsigned char)(pManager->GetBattleType()), pManager->GetBattleID(), pManager->GetBattleEndstamp() );
}


void bg_player_imp::PlayerLeave()
{
	EnableFreePVP( false );
	//回收战场专用物品
	RecycleBattleItem();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsBattleOffense() )
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		
	}
	else if( pPlayer->IsBattleDefence() ) 
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_DEFENCE );
	}
	else if( pPlayer->IsBattleVisitor() ) 
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_VISITOR);
		_filters.RemoveFilter(FILTER_BATTLEGROUND_INVINCIBLE);
		SetMeleeSealMode(false);
		SetDietSealMode(false);
		SetSilentSealMode(false);
	}
	pManager->PlayerLeave( pPlayer, _battle_faction,_cond_kick, IsLogoutDisconnected());
	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF | filter::FILTER_MASK_BUFF);
	// 通知客户端
	_runner->leave_battleground();
}


const A3DVECTOR& bg_player_imp::GetLogoutPos( int &world_tag )
{
	world_tag = _old_world_tag;
	return _old_pos;
}

bool bg_player_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//按照通常的判定来进行
	amsg.force_attack = 0;
	return true;
}

bool bg_player_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void bg_player_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void bg_player_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

gactive_imp::attack_judge bg_player_imp::GetHookAttackJudge()
{
	return bg_player_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge bg_player_imp::GetHookEnchantJudge()
{
	return bg_player_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill bg_player_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill bg_player_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}


bool bg_player_imp::PlayerUseItem(int where, size_t inv_index, int item_type,size_t count)
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

	
	return gplayer_imp::PlayerUseItem(where,inv_index,item_type,count);
}

bool bg_player_imp::PlayerUseItemWithArg(int where,size_t inv_index,int item_type,size_t count, const char * buf, size_t buf_size)
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
	
	return gplayer_imp::PlayerUseItemWithArg(where,inv_index,item_type,count,buf,buf_size);
}
