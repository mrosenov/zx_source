#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "raid_player.h"
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
#include "raid_world_manager.h"
#include "../faction.h"
#include <factionlib.h>
#include "../general_indexer_cfg.h"

DEFINE_SUBSTANCE( raid_player_imp, gplayer_imp, CLS_RAID_PLAYER_IMP )
DEFINE_SUBSTANCE( cr_raid_player_imp, raid_player_imp, CLS_CR_RAID_PLAYER_IMP )  // Youshuang add

void raid_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void raid_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}
	
int raid_player_imp::MessageHandler( const MSG& msg )
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
			if( (ack_msg.attacker_mode & attack_msg::PVP_SANCTUARY) && ack_msg.ainfo.attacker.IsPlayerClass() ) return 0;
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
				if( (ech_msg.attacker_mode & attack_msg::PVP_SANCTUARY) && ech_msg.ainfo.attacker.IsPlayerClass() ) return 0;
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
			raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
			pManager->OnKillMonster(GetParent(), msg.param, msg.param2);
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

void raid_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// 已经被清除了,断线了等待处理中
		return;
	}

	if( _timeout >= 0 )
	{
		// 已经在踢出准备状态下，_timeout后一定会踢出，不会在做后面面判断
		--_timeout;
		if( _timeout <= 0 && !_is_leaving)
		{
			LeaveRaid(CHG_GS_LEAVE_RAID);
		}
		return;
	}

	if(_is_leaving)
	{
		++_kickout_time;
		if(_kickout_time == _leave_raid_time && !_send_raid_leave)
		{
			int worldTag = -1;
			const A3DVECTOR& logoutPos = GetLogoutPos(worldTag);
			GNET::SendRaidLeave(gmatrix::GetServerIndex(), _parent->ID.id, _old_world_tag, pManager->GetWorldTag(), logoutPos.x, logoutPos.y, logoutPos.z, CHG_GS_LEAVE_RAID);
			_send_raid_leave = true;
		}
		if(_kickout_time == _leave_raid_time + 15)
		{
			LeaveAbnormalState();
			PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			GLog::log(GLOG_INFO, "副本玩家退出异常，被强制退出: userid= %d, status = %d, kick_timeout = %d, raid_id=%d, radi_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetRaidStatus(), _kickout_time, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		}
		else if(_kickout_time == _leave_raid_time + 30)
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			GLog::log(GLOG_INFO, "副本玩家退出异常，被强制断线: userid= %d, status = %d, kick_timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetRaidStatus(), _kickout_time, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		}
		return;
	}

	
	// 以下进行判断是否需要踢出玩家
	//if( CheckGMPrivilege() )
	//{
		// gm不踢
		//return;
	//}
	if( !pManager->IsRaidRunning() )
	{
		// 判断是否需要发送结果,给奖励
		if(!_raid_result_sent) HandleRaidResult();

		// 非正常状态
		// xx秒后踢出该玩家，发送消息
		_timeout = abase::Rand(10, 20);
		_leave_raid_time = pManager->GetPlayerLeaveTime();
		_is_leaving = true;
		__PRINTF( "非正常副本状态%d, %d 秒后踢出玩家\n", pManager->GetRaidStatus(), _timeout + _leave_raid_time );
		GLog::log(GLOG_INFO, "非正常副本状态, 玩家在副本被踢出去了: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( pManager->GetRaidID(), _timeout + _leave_raid_time);
		return;
	}
	
	// 处理复活
	HandleDeath();
	SendRaidUseGlobalVars();
}

struct cost_item
{
	int idx;
	int count;
};

bool raid_player_imp::ValidPlayer()
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	const raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(pManager->GetRaidID());
	if(!rwt)
	{
		return false;
	}
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt->rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		printf("无效的副本模板: %d\n", rwt->rwinfo.raid_template_id);
		return false;
	}	

	//检查加入副本物品
	int joinItemId = raid_config.required_item_id;
	int joinItemCnt = raid_config.required_item_count;
	std::vector<cost_item> costItems;
	item_list& inv = GetInventory();
	
	if(joinItemId > 0 && joinItemCnt > 0)
	{
		bool flag = false;
		for(size_t i = 0; i < inv.Size(); i ++)
		{
			if(inv[i].type == joinItemId && joinItemCnt > 0)
			{
				int invCnt = inv[i].count;
				cost_item ci;
				ci.idx = i;
				ci.count = joinItemCnt <= invCnt ? joinItemCnt : invCnt;
				costItems.push_back(ci);
				joinItemCnt -= invCnt;
				if(joinItemCnt <= 0)
				{
					flag = true;
					break;
				}
			}
		}
		if(!flag)
		{
			_timeout = 3;
			__PRINTF( "玩家没有进入副本所需物品 %d 秒后踢出玩家 %d, 物品ID=%d, 数量%d\n", _timeout, _parent->ID.id, raid_config.required_item_id, raid_config.required_item_count);
			GLog::log(GLOG_INFO, "玩家没有进入副本所需物品被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
			_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
			return false;
		}
	}

	// 对于级别的判断，级别不符合踢出去
	if( _basic.level < raid_config.player_min_level || _basic.level > raid_config.player_max_level)
	{
		// 非正常级别玩家进入副本了
		// 3秒后踢出该玩家，发送消息
		_timeout = 3;
		__PRINTF( "玩家级别不在范围内 %d 秒后踢出玩家 %d, 级别:%d, 需求级别范围 %d - %d\n", _timeout, _parent->ID.id, _basic.level, pManager->GetLevelMin(), pManager->GetLevelMax() );
		GLog::log(GLOG_INFO, "玩家在副本因为等级不符合被踢出去了: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}

	short cls = GetParent()->GetClass();
	if(!object_base_info::CheckCls(cls, raid_config.character_combo_id, raid_config.character_combo_id2))
	{
		_timeout = 3;
		__PRINTF( "玩家职业不在范围内 %d 秒后踢出玩家 %d, 职业:%d, 职业掩码1:0x%llx, 职业掩码2:0x%lld\n", _timeout, _parent->ID.id, cls, raid_config.character_combo_id, raid_config.character_combo_id2);
		GLog::log(GLOG_INFO, "玩家职业不符合被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.required_race == 0 && !(cls <= 0x1F || (cls > 0x3F && cls <= 0x5F)))
	{
		_timeout = 3;
		__PRINTF( "玩家种族不符 %d 秒后踢出玩家 %d, 职业: %d\n", _timeout, _parent->ID.id, cls);
		GLog::log(GLOG_INFO, "玩家种族不符合被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.required_race == 1 && !((cls > 0x1F && cls <= 0x3F) || (cls > 0x5F && cls <= 0x7F)))
	{
		_timeout = 3;
		__PRINTF( "玩家种族不符 %d 秒后踢出玩家 %d, 职业: %d\n", _timeout, _parent->ID.id, cls);
		GLog::log(GLOG_INFO, "玩家种族不符合被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d\n", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(GetRebornCount() < raid_config.renascence_count)
	{
		_timeout = 3;
		__PRINTF( "玩家飞升等级不符 %d 秒后踢出玩家 %d, 飞升等级: %d, 所需飞升等级: %d", _timeout, _parent->ID.id, GetRebornCount(), raid_config.renascence_count);
		GLog::log(GLOG_INFO, "玩家飞升等级不符合被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.god_devil_mask && !(GetCultivation() & raid_config.god_devil_mask))
	{
		_timeout = 3;
		__PRINTF( "玩家造化不符 %d 秒后踢出玩家 %d, 玩家造化: %d, 所需造化: %d\n", _timeout, _parent->ID.id, GetCultivation(), raid_config.god_devil_mask);
		GLog::log(GLOG_INFO, "玩家造化不符合被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.required_money > 0 && GetMoney() < (size_t)raid_config.required_money)
	{
		__PRINTF( "玩家金钱不足 %d 秒后踢出玩家 %d, 玩家金钱: %d, 所需金钱: %d\n", _timeout, _parent->ID.id, GetMoney(), raid_config.required_money);
		GLog::log(GLOG_INFO, "玩家金钱不符合被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}

	for(int i = 0; i < 4; i ++)
	{
		int id = raid_config.required_reputation[i].reputation_type;
		int value = raid_config.required_reputation[i].reputation_value; 
		if(id >= 0 && value > 0 && GetRegionReputation(id) < value)
		{
			__PRINTF( "玩家声望不足 %d 秒后踢出玩家 %d, 玩家声望: %d, 所需声望ID: %d, 所需声望值%d\n", _timeout, _parent->ID.id, GetRegionReputation(id), id, value);
			GLog::log(GLOG_INFO, "玩家声望不足被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
			_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
			return false;
		}
	}	

	for(size_t i = 0; i < costItems.size(); i ++)
	{
		cost_item& ci = costItems[i];
		UseItemLog(inv, ci.idx, ci.count);
		inv.DecAmount(ci.idx, ci.count);
		_runner->player_drop_item(IL_INVENTORY, ci.idx, joinItemId, ci.count ,S2C::DROP_TYPE_USE);
	}

	if(raid_config.required_money > 0)
	{
		SpendMoney(raid_config.required_money);
		_runner->spend_money(raid_config.required_money);
	}

	return true;

}

void raid_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{
	_old_world_tag = user._origin_tag;
	_old_pos = user._origin_pos;
	_logout_pos_flag = 1;
	raid_world_manager* pManager = (raid_world_manager*)GetWorldManager();

	//loginflag=4 表示为副本中断线再上线，如果非这种情况都需要检测NPC进入副本条件，并增加当日进入副本的次数
	//loginflag=7 断线重入跨服战场或副本
	if(loginflag != 4 && loginflag != 7)
	{
		if(ValidPlayer())
		{
			IncRaidCounter(pManager->GetRaidID());
			_success_entered = true;
		}
	}
	else
	{
		_reenter = true;
		_success_entered = true;
	}
}

void raid_player_imp::PlayerEnterWorld()
{
	__PRINTF( "玩家 %d 在副本世界 %d 上线啦!!!\n", _parent->ID.id, _plane->GetTag() );
	PlayerEnter();
	gplayer_imp::PlayerEnterWorld();
	raid_world_manager* pManager = (raid_world_manager*)GetWorldManager();
	gplayer* pPlayer = GetParent();
	if(_success_entered)
	{
		GNET::SendRaidEnterSuccess(gmatrix::GetServerIndex(), _parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag());
	}
	
	if(!_reenter && pManager->GetRaidID() == 533)
	{	
		PlayerTaskInterface task_if(this);
		if(!OnTaskCheckDeliver(&task_if, 26213,0))
		{
			__PRINTF("梦回河阳城任务26213发放失败\n");
		}
		
		if(!OnTaskCheckDeliver(&task_if, 26368, 0))
		{
			__PRINTF("梦回河阳城任务26368发放失败\n");
		}
	}
	pManager->PlayerEnterWorld( pPlayer, _battle_faction, _reenter);
}

void raid_player_imp::PlayerEnter()
{
	EnableFreePVP( true );
	//回收副本专用物品
	RecycleBattleItem();
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	_battle_faction = RF_ATTACKER;

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
		SetMeleeSealMode(true);
		SetDietSealMode(true);
		SetSilentSealMode(true);
		_attack_faction = 0;
		_defense_faction = 0;
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}

	if(_battle_faction != 0)
	{
		A3DVECTOR temp(0,0,0);
		pManager->GetRandomEntryPos(temp,_raid_faction);
		if(temp.x != 0 || temp.y != 0 || temp.z != 0)
		{
			_parent->pos = temp;
		}
		__PRINTF("玩家副本登陆位置: x=%3f, y=%3f, z=%3f\n", _parent->pos.x, _parent->pos.y, _parent->pos.z);
	}
	else
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "玩家在副本因为无阵营被踢出: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( pManager->GetRaidID(), _timeout );
	}

	pManager->PlayerEnter( pPlayer, _battle_faction, _reenter);
	// 通知客户端
	_runner->enter_raid( _battle_faction, (unsigned char)(pManager->GetRaidType()), pManager->GetRaidID(), pManager->GetRaidEndstamp() );

	gobject_imp::StepMove(A3DVECTOR(0,0,0));

	GLog::log(GLOG_INFO, "玩家进入副本: roleid=%d, raid_id=%d, raid_world_tag=%d, difficulty=%d", 
			_parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());
}

void raid_player_imp::PlayerLeaveWorld()
{
	__PRINTF( "玩家 %d 从副本世界 %d 下线啦!!!\n", _parent->ID.id, _plane->GetTag() );
	PlayerLeave();
	gplayer_imp::PlayerLeaveWorld();
}

void raid_player_imp::PlayerLeave()
{
	EnableFreePVP( false );
	//回收副本专用物品
	RecycleBattleItem();
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
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
		SetMeleeSealMode(false);
		SetDietSealMode(false);
		SetSilentSealMode(false);
	}
	pManager->PlayerLeave( pPlayer, _battle_faction,_cond_kick);
	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF );
	// 通知客户端
	_runner->leave_raid();
}

void raid_player_imp::PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos )
{
	__PRINTF("玩家在副本中不应该出现切换场景\n");
	ASSERT(false);
}

void raid_player_imp::PlayerLeaveMap()
{
	__PRINTF("玩家在副本中不应该出现切换场景\n");
	ASSERT(false);
}

int raid_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int raid_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

const A3DVECTOR& raid_player_imp::GetLogoutPos( int &world_tag )
{
	world_tag = _old_world_tag;
	return _old_pos;
}

void raid_player_imp::HandleRaidResult()
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	int raid_result = pManager->GetRaidResult();
	if( !_raid_result_sent && raid_result)
	{
		if(raid_result == RAID_ATTACKER_WIN)
		{
			int win_task_id = pManager->GetWinTaskID(); 
			if(win_task_id > 0)
			{
				PlayerTaskInterface task_if(this);
				if(OnTaskCheckDeliver(&task_if,win_task_id,0))
				{
					__PRINTF("发放副本获胜奖励任务%d... \n", win_task_id);
				}
			}

			GLog::log(GLOG_INFO, "玩家完成副本: roleid= %d, raid_id=%d, radi_world_tag =%d, difficulty=%d", 
					_parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());
		}
	}
}

void raid_player_imp::LeaveRaid(int reason)
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	if(!pManager || pManager->GetRaidID() == 0) return;

	if(!_send_raid_leave)
	{
		int worldTag = -1;
		const A3DVECTOR& logoutPos = GetLogoutPos(worldTag);
		GNET::SendRaidLeave(gmatrix::GetServerIndex(), _parent->ID.id, _old_world_tag, pManager->GetWorldTag(), logoutPos.x, logoutPos.y, logoutPos.z, reason);
		_send_raid_leave = true;
	}
	_leave_raid_time = pManager->GetPlayerLeaveTime();
	_is_leaving = true;
	_timeout = 0;
}

void raid_player_imp::PlayerLeaveRaid(int reason)
{
	if(!_pstate.CanLogout())
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}
	if(reason != CHG_GS_LEAVE_BY_VOTE && IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;	
	}
	LeaveRaid(reason);
}

bool raid_player_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//按照通常的判定来进行
	amsg.force_attack = 0;
	return true;
}

bool raid_player_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void raid_player_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void raid_player_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

gactive_imp::attack_judge raid_player_imp::GetHookAttackJudge()
{
	return raid_player_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge raid_player_imp::GetHookEnchantJudge()
{
	return raid_player_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill raid_player_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill raid_player_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}	

void raid_player_imp::GetRaidInfo()
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	std::vector<S2C::CMD::raid_info::player_info_in_raid> info;
	if( pManager->GetRaidInfo( _raid_info_seq, info ) )
	{
		if( info.size() )
		{
			_runner->raid_info((const char*)&info[0], sizeof(S2C::CMD::raid_info::player_info_in_raid) * info.size() );
		}
	}
}

void raid_player_imp::SendRaidUseGlobalVars()
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	std::vector<int> var_list;
	pManager->GetRaidUseGlobalVars(var_list);
	if(var_list.size() > 0)
	{
		raw_wrapper ar(128);
		for(size_t i = 0; i < var_list.size(); i ++)
		{
			int key = var_list[i];
			int value = OI_GetGlobalValue(key);
			ar << key << value;
		}
		_runner->send_raid_global_vars(var_list.size(), ar.data(), ar.size());
	}
}

void raid_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsZombie() ) return;
	gplayer_imp::OnDeath( lastattack, is_pariah, true );
}

// Youshuang add
void cr_raid_player_imp::PlayerEnter()
{
	raid_player_imp::PlayerEnter();
	cr_world_manager* pManager = (cr_world_manager*)_plane->GetWorldManager();
	_runner->get_raid_boss_info( pManager->GetKilledBossList() );
}
// end
