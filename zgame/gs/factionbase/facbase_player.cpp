#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "facbase_player.h"
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
#include "facbase_world_manager.h"
#include "../faction.h"
#include <factionlib.h>
#include "../general_indexer_cfg.h"

DEFINE_SUBSTANCE( facbase_player_imp, gplayer_imp, CLS_FACBASE_PLAYER_IMP )

int facbase_player_imp::MessageHandler( const MSG& msg )
{
	return gplayer_imp::MessageHandler( msg );
}

void facbase_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );

	facbase_world_manager* pManager = (facbase_world_manager*)GetWorldManager();
	if (pManager->IsClosing())
	{
		if (_leave_timeout == 0)
		{
			__PRINTF("基地关闭 传出玩家 %d\n", _parent->ID.id);
			LeaveFacBase();
		}
		else
		{
			if (_leave_timeout > LEAVE_TIMEOUT)
			{
				//仿照 ServerShutDown() 函数
				if (_parent->b_disconnect)
					return;
				ActiveCombatState(false);
				LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
				GLog::log(GLOG_ERR, "基地玩家强制退出: userid= %d", _parent->ID.id);
			}
		}
		_leave_timeout++;
	}
}

void facbase_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;

	//_success_entered = true;
}

void facbase_player_imp::PlayerEnterWorld()
{
	__PRINTF( "玩家 %d 在基地 %d 上线啦!!!\n", _parent->ID.id, _plane->GetTag() );
//	PlayerEnter();
	A3DVECTOR temp(312, 189, 33);
	_parent->pos = temp;
	__PRINTF("玩家基地登陆位置: x=%3f, y=%3f, z=%3f\n", _parent->pos.x, _parent->pos.y, _parent->pos.z);

	gplayer_imp::PlayerEnterWorld();

	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	gplayer* pPlayer = GetParent();
	mgr->PlayerEnterBase(pPlayer);
}
/*
void facbase_player_imp::PlayerEnter()
{
//	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
//	gplayer* pPlayer = GetParent();

	A3DVECTOR temp(0,100,0);
	_parent->pos = temp;
	__PRINTF("玩家基地登陆位置: x=%3f, y=%3f, z=%3f\n", _parent->pos.x, _parent->pos.y, _parent->pos.z);

//	pManager->PlayerEnter( pPlayer, _battle_faction, _reenter);
	// 通知客户端
//	_runner->enter_raid( _battle_faction, (unsigned char)(pManager->GetRaidType()), pManager->GetRaidID(), pManager->GetRaidEndstamp() );

//	GLog::log(GLOG_INFO, "玩家进入副本: roleid=%d, raid_id=%d, raid_world_tag=%d, difficulty=%d", 
//			_parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());
}
*/
void facbase_player_imp::PlayerLeaveWorld()
{
	__PRINTF( "玩家 %d 从基地 %d 下线啦!!!\n", _parent->ID.id, _plane->GetTag() );
//	PlayerLeave();
	gplayer_imp::PlayerLeaveWorld();

	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	gplayer* pPlayer = GetParent();
	mgr->PlayerLeaveBase(pPlayer);
}
/*
void facbase_player_imp::PlayerLeave()
{
//	pManager->PlayerLeave( pPlayer, _battle_faction,_cond_kick);
	// 通知客户端
//	_runner->leave_raid();
}
*/
void facbase_player_imp::PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos )
{
	__PRINTF("玩家在副本中不应该出现切换场景\n");
	ASSERT(false);
}

void facbase_player_imp::PlayerLeaveMap()
{
	__PRINTF("玩家在副本中不应该出现切换场景\n");
	ASSERT(false);
}
/*
const A3DVECTOR& facbase_player_imp::GetLogoutPos( int &world_tag )
{
	world_tag = _old_world_tag;
	return _old_pos;
}
*/
bool facbase_player_imp::InSelfBase() //玩家是否在自己的基地中
{               
	if (OI_GetMafiaID() == 0)
		return false;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	if (mgr->GetBaseId() != OI_GetMafiaID())
		return false;
	return true;
}       

void facbase_player_imp::AddFacBuilding(int fid, int index, int tid)
{
	if (fid == 0 || fid != OI_GetMafiaID())
		return;
	if (!InSelfBase())
		return;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER 
			&& GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER
			&& GetParent()->rank_mafia != gplayer::TITLE_CAPTAIN)
	{
		_runner->error_message(S2C::ERR_FBASE_PRIVILEGE);
		return;
	}
	/*
	MSG msg;
	add_fac_building_msg info(index, tid);
	BuildMessage(msg, GM_MSG_ADD_FAC_BUILDING, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()),
			pPlayer->ID, pPlayer->pos, 0, &info, sizeof(info) );
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send )
	*/
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
//	int grass = 0, mine = 0, food = 0, core = 0;
	int err = mgr->AddFacBuilding(index, tid, GetParent()->ID, _username_len, _username);
	if (err)
	{
		_runner->error_message(err);
		return;
	}
	//_runner->fac_building_add(index, tid); 改为全基地广播了
//	size_t name_len = 0;
//	const void * name = GetPlayerName(name_len);
//	GMSV::SendFacDynBuildingUpgrade(OI_GetMafiaID(), name, name_len, tid, 1, grass, mine, food, core);
}

void facbase_player_imp::UpgradeFacBuilding(int fid, int index, int tid, int cur_lev)
{
	if (fid == 0 || fid != OI_GetMafiaID())
		return;
	if (!InSelfBase())
		return;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER 
			&& GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER
			&& GetParent()->rank_mafia != gplayer::TITLE_CAPTAIN)
	{
		_runner->error_message(S2C::ERR_FBASE_PRIVILEGE);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
//	int grass = 0, mine = 0, food = 0, core = 0;
	int err = mgr->UpgradeFacBuilding(index, tid, GetParent()->ID, cur_lev, _username_len, _username);
	if (err)
	{
		_runner->error_message(err);
		return;
	}
	//_runner->fac_building_upgrade(index, tid, cur_lev); 改为全基地广播了
//	size_t name_len = 0;
//	const void * name = GetPlayerName(name_len);
//	GMSV::SendFacDynBuildingUpgrade(OI_GetMafiaID(), name, name_len, tid, cur_lev+1, grass, mine, food, core);
}

void facbase_player_imp::RemoveFacBuilding(int fid, int index, int tid)
{
	if (fid == 0 || fid != OI_GetMafiaID())
		return;
	if (!InSelfBase())
		return;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER 
			&& GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER
			&& GetParent()->rank_mafia != gplayer::TITLE_CAPTAIN)
	{
		_runner->error_message(S2C::ERR_FBASE_PRIVILEGE);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	int err = mgr->RemoveFacBuilding(index, tid);
	if (err)
	{
		_runner->error_message(err);
		return;
	}
	//_runner->fac_building_remove(index); 改为全基地广播了
}

void facbase_player_imp::PlayerGetFacBase()
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	S2C::CMD::player_fac_base_info base_info;
	std::vector<S2C::INFO::player_fac_field> field_info;
	abase::octets msg;
	mgr->GetBaseInfo(base_info, field_info, msg);
	_runner->player_fac_base_info(base_info, field_info, msg);
}

void facbase_player_imp::FacBaseTransfer(int index)
{
	if (!InSelfBase())
		return;
	A3DVECTOR dst_pos;
	if (!gmatrix::GetFBaseCtrl().GetTransferPos(index, dst_pos))
		return;
	LongJump(dst_pos);
}

int facbase_player_imp::GetFacBuildingLevel(int iBuildId)
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetBuildingLevel(iBuildId);
}

int facbase_player_imp::GetFacInBuildingLevel()
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetInBuildingLevel();
}

int facbase_player_imp::GetFBaseGrass()
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetGrass();
}

void facbase_player_imp::DeductFBaseGrass(int num)
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	num *= -1;
	return mgr->ChangeGrass(_parent->ID.id, num);
}

void facbase_player_imp::DeliverFBaseGrass(int num)
{
	if (!InSelfBase())
	{
		gplayer_imp::DeliverFBaseGrass(num);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->ChangeGrass(_parent->ID.id, num);
}

int facbase_player_imp::GetFBaseMine()
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetMine();
}

void facbase_player_imp::DeductFBaseMine(int num)
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	num *= -1;
	return mgr->ChangeMine(_parent->ID.id, num);
}

void facbase_player_imp::DeliverFBaseMine(int num)
{
	if (!InSelfBase())
	{
		gplayer_imp::DeliverFBaseMine(num);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->ChangeMine(_parent->ID.id, num);
}

int facbase_player_imp::GetFBaseMonsterFood()
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetMonsterFood();
}

void facbase_player_imp::DeductFBaseMonsterFood(int num)
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	num *= -1;
	return mgr->ChangeMonsterFood(_parent->ID.id, num);
}

void facbase_player_imp::DeliverFBaseMonsterFood(int num)
{
	if (!InSelfBase())
	{
		gplayer_imp::DeliverFBaseMonsterFood(num);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->ChangeMonsterFood(_parent->ID.id, num);
}

int facbase_player_imp::GetFBaseMonsterCore()
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetMonsterCore();
}

void facbase_player_imp::DeductFBaseMonsterCore(int num)
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	num *= -1;
	return mgr->ChangeMonsterCore(_parent->ID.id, num);
}

void facbase_player_imp::DeliverFBaseMonsterCore(int num)
{
	if (!InSelfBase())
	{
		gplayer_imp::DeliverFBaseMonsterCore(num);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->ChangeMonsterCore(_parent->ID.id, num);
}

int facbase_player_imp::GetFBaseCash()
{
	if (!InSelfBase())
		return 0;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->GetCash();
}

void facbase_player_imp::DeductFBaseCash(int num)
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	num *= -1;
	return mgr->ChangeCash(_parent->ID.id, num);
}

void facbase_player_imp::DeliverFBaseCash(int num)
{
	if (!InSelfBase())
	{
		gplayer_imp::DeliverFBaseCash(num);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	return mgr->ChangeCash(_parent->ID.id, num);
}

void facbase_player_imp::OnFBaseBuildingProgress(unsigned long task_id, int value)
{
	if (!InSelfBase())
	{
		gplayer_imp::OnFBaseBuildingProgress(task_id, value);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	mgr->OnFacBaseBuildingProgress(OI_GetMafiaID(), task_id, value);
}

void facbase_player_imp::DeliveryFBaseAuction(int itemid)
{
	if (!InSelfBase())
	{
		gplayer_imp::DeliveryFBaseAuction(itemid);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	mgr->FacBaseTaskPutAuction(_parent->ID.id, _username_len, _username, itemid);
}

void facbase_player_imp::DebugChangeBaseProp(int type, int delta)
{
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	mgr->OnFacBasePropChange(_parent->ID.id, OI_GetMafiaID(), type, delta);
}

bool facbase_player_imp::GetFacbaseCashItemsInfo()
{
	if (!InSelfBase())
		return false;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER && GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER)
	{
		_runner->error_message(S2C::ERR_FBASE_PRIVILEGE);
		return false;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	_runner->get_facbase_cash_items_info( mgr->GetCashItemsCooldownInfo() );
	return true;
}

void facbase_player_imp::BuyFacbaseCashItem( int item_idx )
{
	if (!InSelfBase())
		return;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER && GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER)
	{
		_runner->error_message(S2C::ERR_FBASE_PRIVILEGE);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	//int err = mgr->BuyCashItem(item_idx);
	int err = mgr->BuyCashItem(_parent->ID.id, item_idx, _username_len, _username);
	if (err)
		_runner->error_message(err);
	else
	{
		_runner->get_facbase_cash_items_info( mgr->GetCashItemsCooldownInfo() );
		_runner->get_facbase_cash_item_success( item_idx );
		GLog::formatlog("formatlog:玩家%d从帮派元宝商城购买第%d个物品", _parent->ID.id, item_idx);
	}
}

gplayer_imp::FBASE_CMD_RET facbase_player_imp::CheckPostFacBaseMsg(int fid, char message[], unsigned int len)
{
	if (!InSelfBase())
		return gplayer_imp::CheckPostFacBaseMsg(fid, message, len);
	if (OI_GetMafiaID() <= 0 || fid != OI_GetMafiaID())
		return FBASE_CMD_FAIL;
	if (len > MAX_FAC_BASE_MSG_LEN)
		return FBASE_CMD_FAIL;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER 
			&& GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER
			&& GetParent()->rank_mafia != gplayer::TITLE_CAPTAIN)
		return FBASE_CMD_FAIL;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	mgr->UpdateFacBaseMsg(message, len);
	return FBASE_CMD_SUCCESS;
}

gplayer_imp::FBASE_CMD_RET facbase_player_imp::DoBidFromFacbaseMall( int idx, int item_id, int coupon, int name_len, char * player_name)
{
	if (!InSelfBase())
		return gplayer_imp::DoBidFromFacbaseMall(idx, item_id, coupon, name_len, player_name);
	const facbase_auction_item_info* iteminfo = player_template::GetInstance().GetAuctionItemInfoByID( item_id );
	if( !iteminfo || ( iteminfo->needed_faction_coupon > coupon ) || ( coupon > _faction_coupon ) )
		return FBASE_CMD_FAIL;
	if (name_len != (int)_username_len || strncmp(player_name, _username, _username_len) != 0)
	{
		GLog::log(GLOG_ERR, "DoBidFromFacbaseMall name invalid, roleid %d", _parent->ID.id);
		return FBASE_CMD_FAIL;
	}
	FactionCouponChange(-1*coupon);

	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	mgr->FacBaseBid(_parent->ID.id, idx, item_id, coupon, name_len, player_name);
	return FBASE_CMD_SUCCESS;
}

void facbase_player_imp::WithdrawFacBaseAuction(int item_index)
{
	if (!InSelfBase())
		return;
	if (_inventory.GetEmptySlotCount() <= 0)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	int itemid = 0;
	int err = mgr->WithdrawAuction(_parent->ID.id, item_index, itemid);
	if (err)
	{
		_runner->error_message(err);
		return;
	}
	DeliverItem(itemid, 1, false, 0, ITEM_INIT_TYPE_TASK);
	GLog::log(GLOG_INFO, "player %d withdraw auction item %d", _parent->ID.id, itemid);
	GLog::formatlog("formatlog:withdrawfacbaseauc:roleid=%d:item_index=%d:itemid=%d", _parent->ID.id, item_index, itemid);
}

void facbase_player_imp::WithdrawFacBaseCoupon()
{
	if (!InSelfBase())
		return;
	facbase_world_manager * mgr = (facbase_world_manager *)GetWorldManager();
	int coupon = 0;
	int err = mgr->WithdrawCoupon(_parent->ID.id, coupon);
	if (err)
	{
		_runner->error_message(err);
		return;
	}
	FactionCouponChange(coupon);
	GLog::log(GLOG_INFO, "player %d withdraw return coupon %d", _parent->ID.id, coupon);
	GLog::formatlog("formatlog:withdrawfacbasecoupon:roleid=%d:coupon=%d", _parent->ID.id, coupon);
}

bool facbase_player_imp::LeaveFacBase()
{
	if (!_pstate.IsNormalState() || IsCombatState())
	{
		printf("玩家 %d 无法离开基地，玩家当前状态无法换线\n", GetParent()->ID.id);
		return false;
	}
	return GNET::SendFacBaseLeave(_parent->ID.id, _logout_tag, _logout_pos.x, _logout_pos.y, _logout_pos.z);
}

void facbase_player_imp::OnUpdateMafiaInfo()
{
	if (OI_GetMafiaID() == 0)
	{
		ActiveCombatState(false);
		LeaveFacBase();
	}
}


