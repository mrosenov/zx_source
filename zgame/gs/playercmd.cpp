#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include <common/protocol_imp.h>
#include "task/taskman.h"
#include "task/TaskAsyncData.h"
#include "invincible_filter.h"
#include "sfilterdef.h"
#include "cooldowncfg.h"
#include "instance/battleground_manager.h"
#include "serviceprovider.h"
#include "pmvsession.h"
#include <factionlib.h>
#include "commondata_if.h"
#include "global_drop.h"
#include "raid/raid_world_manager.h"
#include "item/item_talisman.h"
#include "skillwrapper.h"
#include "forbid_manager.h"
#include "battleground/bg_world_manager.h"

/*
 *	这个文件里面是专门处理player命令的和controller消息的
 *
 */
#include "pathfinding/pathfinding.h"

int g_use_old_move_policy = 0;


static bool DebugTimeSegmentToSecs(int start_time, int end_time, time_t &start, time_t &end);
static bool DebugDateTimeToSecs(int date, int exactlytime, time_t &setcurtime);

int
gplayer_controller::MessageHandler(const MSG & msg)
{
	return 0;
}
void
gplayer_controller::ResurrectByItem(float exp_reduce)
{
	gplayer_imp * pImp = ((gplayer_imp*)_imp);
	//判定是否能够进行复活操作
	if(!pImp->CanResurrect())
	{
		return ;
	}

	error_cmd(S2C::ERR_ITEM_NOT_IN_INVENTORY);
	return;
}

void 
gplayer_controller::ResurrectInTown(float exp_reduce)
{
	A3DVECTOR pos;
	gplayer_imp * pImp = ((gplayer_imp*)_imp);
	if(!pImp->CanResurrect())
	{
		return ;
	}
	
	int resurrect_timestamp = pImp->GetResurrectTimestamp();
	if(resurrect_timestamp > 0 && g_timer.get_systime() < resurrect_timestamp)
	{
		return;
	}
	
	if(resurrect_timestamp > 0) pImp->ClrResurrectTimestamp();
	int world_tag;
	if(!pImp->GetWorldManager()->GetTownPosition(pImp,pImp->_parent->pos,pos,world_tag))
	{
		//应该使用默认复活点
		pos = _imp->_parent->pos;
		world_tag = _imp->GetWorldTag();
	}
	//未来首先需要寻找城镇复生点的位置
	float exp_reduce_tmp = 0.1f;
	float hp_recover = 0.1f;
	pImp->GetResurrectState(exp_reduce_tmp,hp_recover);
	pImp->Resurrect(pos,false,exp_reduce,world_tag,hp_recover);

}

int 
gplayer_controller::UnLockInventoryHandler(int cmd_type,const void * buf, size_t size)
{
	switch(cmd_type)
	{
		case C2S::EXCHANGE_INVENTORY_ITEM:
		{
			C2S::CMD::exchange_inventory_item & eii = *(C2S::CMD::exchange_inventory_item*) buf;
			if(sizeof(eii) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eii.index1);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eii.index2);
			}
		}
		break;

		case C2S::MOVE_INVENTORY_ITEM:
		{
			C2S::CMD::move_inventory_item & mii = *(C2S::CMD::move_inventory_item *) buf;
			if(sizeof(mii) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mii.src);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mii.dest);
			}
		}
		break;

		case C2S::DROP_INVENTORY_ITEM:
		{
			C2S::CMD::drop_inventory_item& dii = *(C2S::CMD::drop_inventory_item*) buf;
			if(sizeof(dii) == size)
			{
				gplayer_imp *pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, dii.index);
			}
		}
		break;
		
		case C2S::DROP_EQUIPMENT_ITEM:
		{
			C2S::CMD::drop_equipment_item & dei = *(C2S::CMD::drop_equipment_item*) buf;
			if(sizeof(dei) == size)
			{
				gplayer_imp *pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, dei.index);
			}
		}
		break;

		case C2S::EXCHANGE_EQUIPMENT_ITEM:
		{
			C2S::CMD::exchange_equip_item & eei = *(C2S::CMD::exchange_equip_item*)buf;
			if(sizeof(eei) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eei.idx1);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eei.idx2);
			}
		}
		break;

		case C2S::EQUIP_ITEM:
		{
			C2S::CMD::equip_item & ei = *(C2S::CMD::equip_item*)buf;
			if(sizeof(ei) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, ei.idx_inv);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, ei.idx_eq);
			}
		}
		break;

		case C2S::MOVE_ITEM_TO_EQUIPMENT:
		{
			C2S::CMD::move_item_to_equipment & mite  = *(C2S::CMD::move_item_to_equipment*)buf;
			if(sizeof(mite) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mite.idx_inv);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, mite.idx_eq);
			}
		}
		break;

		case C2S::EXCHANGE_TRASHBOX_ITEM:
		{
			C2S::CMD::exchange_trashbox_item & eti = *(C2S::CMD::exchange_trashbox_item*) buf;
			if(sizeof(eti) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, eti.index1);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, eti.index2);
			}
		}
		break;

		case C2S::MOVE_TRASHBOX_ITEM:
		{
			C2S::CMD::move_trashbox_item & mti = *(C2S::CMD::move_trashbox_item *) buf;
			if(sizeof(mti) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, mti.src);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, mti.dest);
			}
		}
		break;
		
		case C2S::EXHCANGE_TRASHBOX_INVENTORY:
		{
			C2S::CMD::exchange_trashbox_inventory & eti = *(C2S::CMD::exchange_trashbox_inventory *) buf;
			if(sizeof(eti) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, eti.idx_tra);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eti.idx_inv);
			}
		}
		break;

		case C2S::MOVE_TRASHBOX_ITEM_TO_INVENTORY:
		{
			C2S::CMD::move_trashbox_item_to_inventory & mtiti = *(C2S::CMD::move_trashbox_item_to_inventory *) buf;
			if(sizeof(mtiti) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, mtiti.idx_tra);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mtiti.idx_inv);
			}
		}
		break;
		
		case C2S::MOVE_INVENTORY_ITEM_TO_TRASHBOX:
		{
			C2S::CMD::move_inventory_item_to_trashbox & miitt  = *(C2S::CMD::move_inventory_item_to_trashbox *) buf;
			if(sizeof(miitt) == size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_TRASH_BOX, miitt.idx_tra);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, miitt.idx_inv);
			}
		}
		break;
		
		case C2S::DESTROY_ITEM:
		{
			C2S::CMD::destroy_item  & di   = *(C2S::CMD::destroy_item *) buf;
			if(size == sizeof(di))
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(di.where, di.index);
			}
		}
		break;

		case C2S::EQUIP_PET_BEDGE:
		{       
			C2S::CMD::equip_pet_bedge& epb = *(C2S::CMD::equip_pet_bedge*)buf;
			if(sizeof(epb) == size)
			{       
				gplayer_imp* pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,epb.inv_index);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_PET_BEDGE,epb.pet_index);
			}
		}     
		break;

		case C2S::MOVE_PET_BEDGE:
		{
			C2S::CMD::move_pet_bedge& mpb = *(C2S::CMD::move_pet_bedge*)buf;
			if(sizeof(mpb) == size)
			{
				gplayer_imp* pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,mpb.src_index);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_PET_BEDGE,mpb.dst_index);
			}
		}
		break;

		case C2S::EQUIP_PET_EQUIP:
		{       
			C2S::CMD::equip_pet_equip& epe = *(C2S::CMD::equip_pet_equip*)buf;
			if(sizeof(epe) == size)
			{       
				gplayer_imp* pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,epe.inv_index);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_PET_EQUIP,epe.pet_index);
			}       
		}       
		break;

		case C2S::EXCHANGE_POCKET_ITEM:
		{
			C2S::CMD::exchange_pocket_item & epi = *(C2S::CMD::exchange_pocket_item*) buf;
			if(sizeof(epi) != size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, epi.index1);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, epi.index2);
			}
		}
		break;

		case C2S::MOVE_POCKET_ITEM:
		{
			C2S::CMD::move_pocket_item & mpi = *(C2S::CMD::move_pocket_item *) buf;
			if(sizeof(mpi) != size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, mpi.src);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, mpi.dest);
			}
		}
		break;

		case C2S::EXCHANGE_INVENTORY_POCKET_ITEM:
		{
			C2S::CMD::exchange_inventory_pocket_item &eip = *(C2S::CMD::exchange_inventory_pocket_item *) buf;
			if(sizeof(eip) != size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eip.idx_inv);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, eip.idx_poc);
			}
		}
		break;

		case C2S::MOVE_POCKET_ITEM_TO_INVENTORY:
		{
			C2S::CMD::move_pocket_item_to_inventory & mpiti = *(C2S::CMD::move_pocket_item_to_inventory *) buf;
			if(sizeof(mpiti) != size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, mpiti.idx_poc);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mpiti.idx_inv);
			}
		}
		break;
		
		case C2S::MOVE_INVENTORY_ITEM_TO_POCKET:
		{
			C2S::CMD::move_inventory_item_to_pocket& miitp  = *(C2S::CMD::move_inventory_item_to_pocket*) buf;
			if(sizeof(miitp) != size)
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, miitp.idx_inv);
				pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, miitp.idx_poc);
			}
		}
		break;

		case C2S::EXCHANGE_FASHION_ITEM:
		{
			C2S::CMD::exchange_fashion_item & efi = *(C2S::CMD::exchange_fashion_item*) buf;
			if(sizeof(efi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, efi.index1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, efi.index2);
		}
		break;

		case C2S::EXCHANGE_MOUNT_WING_ITEM:
		{
			C2S::CMD::exchange_mount_wing_item & efi = *(C2S::CMD::exchange_mount_wing_item*) buf;
			if(sizeof(efi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, efi.index1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, efi.index2);
		}
		break;

		case C2S::EXCHANGE_INVENTORY_FASHION_ITEM:
		{
			C2S::CMD::exchange_inventory_fashion_item & eifi = *(C2S::CMD::exchange_inventory_fashion_item*) buf;
			if(sizeof(eifi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eifi.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, eifi.idx_fas);
		}
		break;

		case C2S::EXCHANGE_INVENTORY_MOUNTWING_ITEM:
		{
			C2S::CMD::exchange_inventory_mountwing_item & eifi = *(C2S::CMD::exchange_inventory_mountwing_item*) buf;
			if(sizeof(eifi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eifi.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, eifi.idx_mw);
		}
		break;


		case C2S::EXCHANGE_EQUIPMENT_FASHION_ITEM:
		{
			C2S::CMD::exchange_equipment_fashion_item & eefi = *(C2S::CMD::exchange_equipment_fashion_item*) buf;
			if(sizeof(eefi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eefi.idx_equ);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, eefi.idx_fas);
		}
		break;

		case C2S::EXCHANGE_EQUIPMENT_MOUNTWING_ITEM:
		{
			C2S::CMD::exchange_equipment_mountwing_item & eefi = *(C2S::CMD::exchange_equipment_mountwing_item*) buf;
			if(sizeof(eefi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eefi.idx_equ);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, eefi.idx_mw);
		}
		break;

	}
	return 0;
}

int
gplayer_controller::InvalidCommandHandler(int cmd_type, const void * buf, size_t size)
{
	//do nothing
	return 0;
}

int 
gplayer_controller::CommandHandler(int cmd_type,const void * buf, size_t size)
{
	if(!CheckBanish()) return 0;
	switch(cmd_type)
	{
		case C2S::PLAYER_MOVE:
			if(CheckDeny(CMD_MOVE)) return 0;
			cmd_user_move(buf,size, false);
			return 0;
		case C2S::STOP_MOVE:
			if(CheckDeny(CMD_MOVE)) return 0;
			cmd_user_stop_move(buf,size,false);
			return 0;
			
		case C2S::START_MOVE:
			if(CheckDeny(CMD_MOVE)) return 0;
		{
#ifdef __USE_OLD_MOVE_POLICY__
			//do noting in old move policy
#else
			//do nothing yet
			if(!g_use_old_move_policy)
			{
				gplayer_imp * pImp= (gplayer_imp *) _imp;
				pImp->AddStartSession(new session_p_start_move(pImp));
			}
#endif
		}
		break;
			
		case C2S::UNSELECT:
			UnSelect();
			return 0;

		case C2S::RESURRECT_IN_TOWN:
			_load_stats ++;
			{
				gplayer_imp * pImp= (gplayer_imp *) _imp;
				if(!pImp->IsResurrectPosActive())
				{
					error_cmd(S2C::ERR_RESURRECT_POS_NOT_EXIST);
					break;
				}
				
				session_resurrect_in_town *pSession= new session_resurrect_in_town(pImp,39);
				float exp_reduce = player_template::GetResurrectExpReduce(pImp->_basic.sec_level);
				if(pImp->_basic.level < DEATH_PROTECT_LEVEL) exp_reduce = 0.f; 
				pSession->SetExpReduce(exp_reduce);
				pImp->AddStartSession(pSession);
			}
			break;

		case C2S::RESURRECT_BY_ITEM:
			_load_stats ++;
			{
				gplayer_imp * pImp= (gplayer_imp *) _imp;
				session_resurrect_by_item *pSession= new session_resurrect_by_item(pImp,99);
				float exp_reduce = player_template::GetResurrectExpReduce(pImp->_basic.sec_level);
				if(pImp->_basic.level < DEATH_PROTECT_LEVEL) exp_reduce = 0.f; 
				pSession->SetExpReduce(exp_reduce);
				pImp->AddStartSession(pSession);
			}
			break;

		case C2S::RESURRECT_AT_ONCE:
			{
				gplayer_imp * pImp= (gplayer_imp *) _imp;
				float exp_reduce;
				float hp_recover;
				if(!pImp->GetResurrectState(exp_reduce,hp_recover))
				{
					break;
				}
				
				//清楚可能的以前的复活操作
				pImp->ClearSession();

				float base_exp_reduce = player_template::GetResurrectExpReduce(pImp->_basic.sec_level);

				//现在原来的exp_reduce变成比率了，但是传入的数值仍然是0.0~0.05所以进行一下换算
				exp_reduce = (exp_reduce/PLAYER_DEATH_EXP_VANISH) * base_exp_reduce;
				if(pImp->_basic.level < DEATH_PROTECT_LEVEL) exp_reduce = 0.f; 

				if(hp_recover < 0.1f) hp_recover = 0.1f;
				if(hp_recover > 1.0f) hp_recover = 1.0f;


				//还需要检查一下世界的经验值比率
				pImp->Resurrect(pImp->_parent->pos,true,exp_reduce,1, hp_recover);
			}
			break;

		case C2S::SELECT_TARGET:
			{

				C2S::CMD::select_target &cmd = *(C2S::CMD::select_target *)buf;
				if(size != sizeof(cmd))
				{
					error_cmd(S2C::ERR_FATAL_ERR);
					return 0;
				}
				XID target;
				MAKE_ID(target,cmd.id);
				if(target.IsActive())
				{	
					SelectTarget(target.id,true);
				}
			}
			return 0;
		case C2S::NORMAL_ATTACK:
			if(CheckDeny(CMD_ATTACK)) return 0;
		{
			C2S::CMD::normal_attack & attack = *(C2S::CMD::normal_attack*)buf;
			if(sizeof(attack) != size) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(_cur_target.id == -1)
			{
				//当前没有选中对象，返回一个错误
				error_cmd(S2C::ERR_INVALID_TARGET);
				break;
			}

			//不检查选中对象的位置信息
			char force_attack = attack.force_attack;
			if(InSafeLock()) force_attack &= ~C2S::FORCE_ATTACK_WHITE;
			gactive_imp *pImp= (gactive_imp*)_imp;
			session_normal_attack *pAttack = new session_normal_attack(pImp);
			pAttack->SetTarget(_cur_target,force_attack);
			pImp->AddStartSession(pAttack);
		}
		return 0;

		case C2S::PICKUP:
		if(CheckDeny(CMD_PICKUP)) return 0;
		{
			C2S::CMD::pickup_matter & pm = *(C2S::CMD::pickup_matter*)buf;
			if(sizeof(pm) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->_cheat_punish) return 0;
			if(!pImp->CheckCoolDown(COOLDOWN_INDEX_PICKUP)) return 0;
			pImp->SetCoolDown(COOLDOWN_INDEX_PICKUP,400);
			if(!pImp->StandOnGround() && !pImp->InFlying()) return 0;
			
			//限制捡物品的速度
			int ts = g_timer.get_systime();
			_pickup_counter += (ts - _pickup_timestamp) * STD_PICKUP_PER_SECOND;
			if(_pickup_counter > MAX_PICKUP_PER_SECOND) _pickup_counter = MAX_PICKUP_PER_SECOND;
			else if(_pickup_counter < -1024) _pickup_counter = -5;
			_pickup_timestamp = ts;
			
			_pickup_counter --;
			if(_pickup_counter <= 0) return 0;;		//超过了最大捡取速度
			
			size_t type = pm.type;
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			XID obj(GM_TYPE_MATTER,pm.mid);
			if(type != (size_t)g_config.item_money_id)
			{
				//检查包裹是否已经满了 
				if(!pImp->_inventory.HasSlot(type))
				{
					error_cmd(S2C::ERR_INVENTORY_IS_FULL);
					break;
				}
			}
			else
			{	
				//是金钱物品对应的id
				//检查金钱是否满了
				if(pImp->GetMoney() >= pImp->_money_capacity)
				{
					error_cmd(S2C::ERR_INVENTORY_IS_FULL);
					break;
				}
			}
			

			//检查物品是否存在和位置是否合适
			//注意物品的类型包含了16位标志，所以要去掉
			world::object_info info;
			if(!pImp->_plane->QueryObject(obj,info) 
					|| (info.tid & 0xFFFF) != (int)type)
			{
				((gplayer_dispatcher*)pImp->_runner)->object_is_invalid(obj.id);
				break;
			}

			if(info.pos.squared_distance(pPlayer->pos)>=PICKUP_DISTANCE*PICKUP_DISTANCE)
			{
				error_cmd(S2C::ERR_ITEM_CANT_PICKUP);
				break;
			}

			
			//是钱或者没有组队直接发出消息
			{
				msg_pickup_t self = { pPlayer->ID,0,0};
				int roll_item = 0;
				if(pImp->IsInTeam()) 
				{
					self.team_seq = pImp->_team.GetTeamSeq();
					self.team_id = pImp->_team.GetTeamID();
					roll_item = pImp->_team.IsRandomPickup()?1:0;
				}
				pImp->SendTo<0>(GM_MSG_PICKUP,obj,roll_item,&self,sizeof(self));
			}
		}
		break;
		case C2S::GET_ITEM_INFO:
		{
			C2S::CMD::get_item_info & gii = *(C2S::CMD::get_item_info*)buf;
			if(sizeof(gii) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->PlayerGetItemInfo(gii.where,gii.index) < 0)
			{
				//error_cmd(S2C::ERR_FATAL_ERR);
			}
		}
		break;

		case C2S::GET_INVENTORY:
		{
			C2S::CMD::get_inventory & gi = *(C2S::CMD::get_inventory *)buf;
			if(sizeof(gi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int where = gi.where;
			if(where > 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetInventory(where);
		}
		break;

		case C2S::GET_INVENTORY_DETAIL:
		{
			C2S::CMD::get_inventory_detail & gid = *(C2S::CMD::get_inventory_detail *)buf;
			if(sizeof(gid) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			int where = gid.where;
			if(where > 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetInventoryDetail(where);
		}
		break;

		case C2S::EXCHANGE_INVENTORY_ITEM:
		{
			C2S::CMD::exchange_inventory_item & eii = *(C2S::CMD::exchange_inventory_item*) buf;
			if(sizeof(eii) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeInvItem(eii.index1,eii.index2);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eii.index1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eii.index2);
		}
		break;

		case C2S::MOVE_INVENTORY_ITEM:
		{
			C2S::CMD::move_inventory_item & mii = *(C2S::CMD::move_inventory_item *) buf;
			if(sizeof(mii) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerMoveInvItem(mii.src,mii.dest,mii.amount);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mii.src);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mii.dest);
		}
		break;

		case C2S::DROP_INVENTORY_ITEM:
		{
			C2S::CMD::drop_inventory_item& dii = *(C2S::CMD::drop_inventory_item*) buf;
			if(sizeof(dii) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			if(pImp->_cheat_punish) return 0;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, dii.index);
			if(!TestSafeLock()) return 0;
			if(CheckDeny(CMD_PICKUP)) 
			{
				error_cmd(S2C::ERR_CAN_NOT_DROP_ITEM);
				return 0;
			}
			if(!pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_DROP_ITEM))
			{
				break;
			}
			pImp->SetCoolDown(COOLDOWN_INDEX_DROP_ITEM,DROPITEM_COOLDOWN_TIME);
			pImp->PlayerDropInvItem(dii.index,dii.amount,false);
		}
		break;
		
		case C2S::DROP_EQUIPMENT_ITEM:
		{
			C2S::CMD::drop_equipment_item & dei = *(C2S::CMD::drop_equipment_item*) buf;
			if(sizeof(dei) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			if(pImp->_cheat_punish) return 0;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, dei.index);
			if(!TestSafeLock()) return 0;
			if(CheckDeny(CMD_PICKUP)) 
			{
				error_cmd(S2C::ERR_CAN_NOT_DROP_ITEM);
				return 0;
			}

			if(!pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_DROP_ITEM))
			{
				break;
			}
			pImp->SetCoolDown(COOLDOWN_INDEX_DROP_ITEM,DROPITEM_COOLDOWN_TIME);
			pImp->PlayerDropEquipItem(dei.index,false);
		}
		break;

		case C2S::EXCHANGE_EQUIPMENT_ITEM:
		{
			C2S::CMD::exchange_equip_item & eei = *(C2S::CMD::exchange_equip_item*)buf;
			if(sizeof(eei) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeEquipItem(eei.idx1,eei.idx2);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eei.idx1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eei.idx2);
		}
		break;

		case C2S::EQUIP_ITEM:
		{
			C2S::CMD::equip_item & ei = *(C2S::CMD::equip_item*)buf;
			if(sizeof(ei) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerEquipItem(ei.idx_inv,ei.idx_eq);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, ei.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, ei.idx_eq);
		}
		break;

		case C2S::MOVE_ITEM_TO_EQUIPMENT:
		{
			C2S::CMD::move_item_to_equipment & mite  = *(C2S::CMD::move_item_to_equipment*)buf;
			if(sizeof(mite) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerMoveEquipItem(mite.idx_inv,mite.idx_eq);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mite.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, mite.idx_eq);
		}
		break;


		case C2S::DROP_MONEY:
		{
			if(!TestSafeLock()) return 0;
			C2S::CMD::drop_money & dm = *(C2S::CMD::drop_money *)buf;
			if(sizeof(dm) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_DROP_MONEY))
			{
				break;
			}
			pImp->SetCoolDown(COOLDOWN_INDEX_DROP_MONEY,DROPMONEY_COOLDOWN_TIME);
			pImp->PlayerDropMoney(dm.amount,false);
		}
		break;

		case C2S::SELF_GET_PROPERTY:
		{
			C2S::CMD::self_get_property & gsp = *(C2S::CMD::self_get_property *)buf;
			if(sizeof(gsp) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetProperty();
		}
		break;

		case C2S::LEARN_SKILL:
		{
			C2S::CMD::learn_skill & ls = *(C2S::CMD::learn_skill*)buf;
			if(sizeof(ls) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerLearnSkill(ls.skill_id);
		}
		break;

		case C2S::GET_EXTPROP_BASE:
			_imp->_runner->get_extprop_base();
		break;
		case C2S::GET_EXTPROP_MOVE:
			_imp->_runner->get_extprop_move();
		break;
		case C2S::GET_EXTPROP_ATTACK:
			_imp->_runner->get_extprop_attack();
		break;
		case C2S::GET_EXTPROP_DEFENSE:
			_imp->_runner->get_extprop_defense();
		break;

		case C2S::TEAM_INVITE:
		{
			C2S::CMD::team_invite & ti = *(C2S::CMD::team_invite *)buf;
			if(sizeof(ti) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			XID member(GM_TYPE_PLAYER,ti.id);
			if(member == pImp->_parent->ID)
			{
				error_cmd(S2C::ERR_TEAM_CANNOT_INVITE);
				break;
			}
	
			//检测该玩家是否存在
			if(!gmatrix::IsPlayerExist(member.id))
			{
				pImp->_runner->error_message(S2C::ERR_PLAYER_NOT_EXIST);
				break;
			}

			if(!pImp->_team.CliInviteOther(member))
			{
				error_cmd(S2C::ERR_TEAM_CANNOT_INVITE);
			}
		}
		break;

		case C2S::TEAM_AGREE_INVITE:
		{
			C2S::CMD::team_agree_invite & tai = *(C2S::CMD::team_agree_invite *)buf;
			if(sizeof(tai) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			XID leader(GM_TYPE_PLAYER,tai.id);
			if(!pImp->_team.CliAgreeInvite(leader,tai.team_seq))
			{
				error_cmd(S2C::ERR_TEAM_JOIN_FAILED);
			}
		}
		break;
		
		case C2S::TEAM_REJECT_INVITE:
		{
			C2S::CMD::team_reject_invite & tri = *(C2S::CMD::team_reject_invite *)buf;
			if(sizeof(tri) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			XID leader(GM_TYPE_PLAYER,tri.id);
			pImp->_team.CliRejectInvite(leader);
		}
		break;

		case C2S::TEAM_LEAVE_PARTY:
		{
			C2S::CMD::team_leave_party & tlp = *(C2S::CMD::team_leave_party *)buf;
			if(sizeof(tlp) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_team.CliLeaveParty();
		}
		break;

		case C2S::TEAM_KICK_MEMBER:
		{
			C2S::CMD::team_kick_member & tkm = *(C2S::CMD::team_kick_member *)buf;
			if(sizeof(tkm) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			XID member(GM_TYPE_PLAYER,tkm.id);
			pImp->_team.CliKickMember(member);
		}
		break;
		case C2S::TEAM_GET_TEAMMATE_POS:
		{
			C2S::CMD::team_get_teammate_pos & tgtp= *(C2S::CMD::team_get_teammate_pos*)buf;
			if(tgtp.count >= TEAM_MEMBER_CAPACITY || !tgtp.count || sizeof(tgtp) + tgtp.count * sizeof(int) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->_team.IsInTeam()) break;

			XID member(GM_TYPE_PLAYER,-1);
			A3DVECTOR pos;
			int tag;
			for(size_t i = 0; i< tgtp.count; i++)
			{
				member.id = tgtp.id[i];
				if( pImp->_team.GetMemberPos(member,pos,tag))
				{
					pImp->_runner->teammate_get_pos(member,pos,tag); 
				}
			}
		}
		break;

		case C2S::GET_OTHERS_EQUIPMENT:
		{
			C2S::CMD::get_others_equipment & goe = *(C2S::CMD::get_others_equipment*)buf;
			if(sizeof(goe) + sizeof(int) * goe.size != size || goe.size > 256 )
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			_load_stats += goe.size>>1;
			//注：这个是一个非常耗费资源的操作 需要限制玩家的发送速度
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			MSG msg;
			int cs_index = pPlayer->cs_index;
			BuildMessage(msg,GM_MSG_QUERY_PLAYER_EQUIPMENT,XID(-1,-1),pPlayer->ID,
						pPlayer->pos,pPlayer->cs_sid,&cs_index, sizeof(cs_index));

			//针对player id 进行群体发送
			gmatrix::SendPlayerMessage(goe.idlist,goe.size,msg);
		}
		break;

		case C2S::QUERY_CIRCLE_OF_DOOM_INFO:
		{
			C2S::CMD::query_circleofdoom_info & qcd = *(C2S::CMD::query_circleofdoom_info *)buf;
			if(sizeof(qcd) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			MSG msg;
			BuildMessage2(msg, GM_MSG_CIRCLE_OF_DOOM_QUERY, XID(GM_TYPE_PLAYER, qcd.sponsor_id), pPlayer->ID, 
					pPlayer->pos, pPlayer->cs_sid, pPlayer->cs_index);
			gmatrix::SendMessage(msg);
		}
		break;

		case C2S::QUERY_BE_SPIRIT_DRAGGED:
		{
			C2S::CMD::query_be_spirit_dragged & qbsd = *(C2S::CMD::query_be_spirit_dragged*)buf;
			if(sizeof(qbsd) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			MSG msg;
			BuildMessage2(msg, GM_MSG_QUERY_BE_SPIRIT_DRAGGED, XID(GM_TYPE_PLAYER, qbsd.id_be_spirit_dragged), pPlayer->ID, pPlayer->pos, pPlayer->cs_sid, pPlayer->cs_index);
			gmatrix::SendMessage(msg);
		}
		break;

		case C2S::QUERY_BE_PULLED:
		{
			C2S::CMD::query_be_pulled & qbp = *(C2S::CMD::query_be_pulled*)buf;
			if(sizeof(qbp) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			MSG msg;
			BuildMessage2(msg, GM_MSG_QUERY_BE_PULLED, XID(GM_TYPE_PLAYER, qbp.id_be_pulled), pPlayer->ID, pPlayer->pos, pPlayer->cs_sid, pPlayer->cs_index);
			gmatrix::SendMessage(msg);
		}
		break;
		
		case C2S::CHANGE_PICKUP_FLAG:
		{
			C2S::CMD::set_pickup_flag & spf= *(C2S::CMD::set_pickup_flag *)buf;
			if(sizeof(spf) != size)	
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_team.SetPickupFlag(spf.pickup_flag);
		}
		break;

		case C2S::SERVICE_HELLO:
		{
			C2S::CMD::service_hello & cmd = *(C2S::CMD::service_hello*)buf;
			if(size != sizeof(cmd)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			XID target;
			MAKE_ID(target,cmd.id);
			if(target.IsActive())
			{
				session_say_hello *pSession = new session_say_hello(pImp);
				pSession->SetTarget(target);
				pImp->AddStartSession(pSession);
			}
		}
		break;

		case C2S::SERVICE_GET_CONTENT:
		{
			C2S::CMD::service_get_content & cmd = *(C2S::CMD::service_get_content*)buf;
			if(size != sizeof(cmd))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->QueryServiceContent(cmd.service_type);
		}
		break;
		
		case C2S::SERVICE_SERVE:
		{
			C2S::CMD::service_serve & cmd = *(C2S::CMD::service_serve*)buf;
			if(cmd.len > 4096 || size != sizeof(cmd) + cmd.len) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(forbid_manager::IsForbidService(cmd.service_type))
			{
				error_cmd(S2C::ERR_FORBID_SERVICE);
				break;
			}
			
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SendServiceRequest(cmd.service_type,cmd.content,cmd.len);
		}
		break;

		case C2S::LOGOUT:
		{
			C2S::CMD::logout & cmd = *(C2S::CMD::logout *)buf;
			if(size != sizeof(cmd)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int type = cmd.logout_type;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerLogout(type, cmd.offline_agent);
		}
		break;

		case C2S::GET_OWN_WEALTH:
		{
			C2S::CMD::get_own_wealth & gow = *(C2S::CMD::get_own_wealth *)buf;
			if(size != sizeof(C2S::CMD::get_own_wealth)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(gow.detail_inv)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_INVENTORY);

			if(gow.detail_equip)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_EQUIPMENT);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_EQUIPMENT);

			if(gow.detail_task)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_TASK_INVENTORY);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_TASK_INVENTORY);
			
			if(true)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_FASHION);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_FASHION);

			if(true)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_MOUNT_WING);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_MOUNT_WING);
			
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_POCKET);		

			pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);
			pImp->_runner->battle_score( pImp->GetBattleScore() );
		}
		break;

		case C2S::GET_ALL_DATA:
		{
			C2S::CMD::get_all_data & gad = *(C2S::CMD::get_all_data *)buf;
			if(size != sizeof(C2S::CMD::get_all_data)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetProperty();
			if(gad.detail_inv)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_INVENTORY);

			if(gad.detail_equip)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_EQUIPMENT);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_EQUIPMENT);

			if(gad.detail_task)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_TASK_INVENTORY);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_TASK_INVENTORY);

			if(true)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_FASHION);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_FASHION);

			if(true)
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_MOUNT_WING);
			else
				pImp->PlayerGetInventory(gplayer_imp::IL_MOUNT_WING);
			
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_PET_BEDGE);
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_PET_EQUIP);
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_POCKET);		
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_GIFT);		
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_FUWEN);		

			pImp->PlayerNotifyHotkey();	
			pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);
			pImp->_runner->battle_score( pImp->GetBattleScore() );
			pImp->_runner->get_skill_data();
			pImp->_runner->get_addon_skill_permanent_data();
			pImp->_runner->get_combine_skill_data();
			pImp->_runner->player_produce_skill_info(pImp->GetProduceLevel(), pImp->GetProduceExp());
			pImp->_runner->get_recipe_data(pImp->GetRecipeBuf(), pImp->GetRecipeCount());
			pImp->_runner->trashbox_passwd_state(pImp->_trashbox.HasPassword());
			pImp->_runner->player_waypoint_list(NULL,0);	//路点信息已经废弃
			if(!gmatrix::GetWorldParam().billing_shop)
			{
				pImp->_runner->player_cash(pImp->_mallinfo.GetCash(), pImp->_mallinfo.GetCashUsed(), pImp->_mallinfo.GetCashAdd());
				pImp->_runner->player_bonus(pImp->_bonusinfo.GetBonus(), pImp->_bonusinfo.GetBonusUsed());
			}
			else
			{
				pImp->_runner->player_cash(pImp->GetBillingCash(), -1, -1);

			}
			pImp->_runner->pk_value_notify(pImp->GetPKValue());
			pImp->_runner->player_pet_room_capacity(pImp->GetPetBedgeInventory().Size());
			pImp->_runner->player_pet_civilization(pImp->GetPetMan().GetPetCivilization());
			pImp->_runner->player_pet_construction(pImp->GetPetMan().GetPetConstruction());
			pImp->_runner->player_reborn_info(&*(pImp->_reborn_list.begin()), pImp->_reborn_list.size());
			pImp->_runner->send_cooldown_data();
			pImp->_runner->send_timestamp(); 
			pImp->_runner->talisman_combine_value(pImp->GetTalismanValue());
			pImp->_runner->player_mafia_contribution(pImp->GetMafiaContribution(),pImp->GetFamilyContribution());
			pImp->_runner->get_faction_coupon( pImp->GetFactionCoupon() );  // Youshuang add, 获得帮派金券值
			pImp->_runner->offline_agent_time(pImp->_offline_agent_time);
			pImp->SendMultiExp();
			size_t title_count;
			const short * title = pImp->GetPlayerTitleList(title_count);
			size_t extra_title_count;
			const short * extra_title = pImp->GetPlayerExtraTitle(extra_title_count);
			pImp->_runner->player_title_list(title,title_count,extra_title,extra_title_count);
			pImp->_runner->player_use_title( pImp->GetUsedTitle() );
			size_t rep_count;
			const int * rep = pImp->GetRegionReputationData(rep_count);
			pImp->_runner->player_region_reputation(rep,rep_count);
			pImp->_runner->player_talent_notify(pImp->_talent_point);
			pImp->_runner->player_achievement();
			pImp->_runner->player_change_vipstate(pImp->GetParent()->vip_type);
			pImp->_runner->notify_mall_prop(g_config.mall_prop);
			pImp->_runner->get_task_data(); //这个指令被客户端判定为所有数据发完的指示
			pImp->_runner->last_logout_time(pImp->GetLogoutTime());
			pImp->_runner->player_raid_counter();
			pImp->_runner->player_tower_data();
			pImp->_runner->player_tower_monster_list();
			if(pImp->IsTreasureActive()) pImp->_runner->treasure_info();
			if(pImp->IsRuneActive()) pImp->_runner->rune_info(pImp->IsRuneActive(), pImp->GetRuneScore(), pImp->GetRuneCustomizeInfo());
			pImp->_runner->littlepet_info(pImp->GetLittlePetLevel(), pImp->GetLittlePetAwardLevel(), pImp->GetLittlePetCurExp());
			pImp->_runner->fill_platform_mask(pImp->GetFillInfoMask());
			pImp->_runner->ui_transfer_opened_notify();
			pImp->_runner->liveness_notify();
			pImp->_runner->notify_astrology_energy();
			pImp->_runner->event_info();
			pImp->_runner->cash_gift_ids(gmatrix::GetCurCashGiftID1(), gmatrix::GetCurCashGiftID2());
			pImp->_runner->web_order_info();
			pImp->_runner->phase_info();
			pImp->SendCollisionRaidInfo();
			pImp->SendFuwenComposeInfo();
			
			pImp->GetPetMan().OnPlayerLogin(pImp);
			PlayerTaskInterface tf(pImp);
			tf.PlayerEnterWorld();
			//被推广人第一次登陆的时候颁发奖励
			pImp->DeliveryReferrAward();
			pImp->TryEnableAchievement();

			//在线倒计时奖励数据
			pImp->SendOnlineAwardDataDirectly(g_timer.get_systime());
			pImp->GetWorldManager()->GetAllDataWorldManager(pImp);
			pImp->TryObtainCashGift();
			//基地数据
			pImp->PlayerGetFacBase();
			pImp->SendHideAndSeekRaidInfo();
			pImp->PlayerEnterSeekRaid();
			pImp->PlayerEnterCaptureRaid();
		}
		break;

		case C2S::GET_SERVER_TIMESTAMP:
		{
			if(size != sizeof(C2S::CMD::get_server_timestamp)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->send_timestamp(); 
		}
		break;

		case C2S::USE_ITEM:
		if(CheckDeny(CMD_ITEM)) return 0;
		{
			C2S::CMD::use_item & ui = *(C2S::CMD::use_item *)buf;
			if(size != sizeof(C2S::CMD::use_item)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(forbid_manager::IsForbidItem(ui.item_id))
			{
				error_cmd(S2C::ERR_FORBID_ITEM);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUseItem(ui.where, ui.index, ui.item_id,ui.count))
			{	
				//改在物品里面发送这个错误信息了
				error_cmd(S2C::ERR_CANNOT_USE_ITEM);
			}
		}
		break;

		case C2S::USE_ITEM_WITH_ARG:
		if(CheckDeny(CMD_ITEM)) return 0;
		{
			C2S::CMD::use_item_with_arg & ui = *(C2S::CMD::use_item_with_arg *)buf;
			if(size < sizeof(C2S::CMD::use_item)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(forbid_manager::IsForbidItem(ui.item_id))
			{
				error_cmd(S2C::ERR_FORBID_ITEM);
				break;
			}

			size_t buf_size = size - sizeof(ui);
			if(buf_size > 256) break;

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUseItemWithArg(ui.where, ui.index, ui.item_id,ui.count,ui.arg, buf_size))
			{	
				//改在物品里面发送这个错误信息了
				error_cmd(S2C::ERR_CANNOT_USE_ITEM);
			}
		}
		break;

		case C2S::CAST_SKILL:
		if(CheckDeny(CMD_SKILL)) return 0;
		{
			C2S::CMD::cast_skill & cs = *(C2S::CMD::cast_skill *)buf;
			if(size != sizeof(C2S::CMD::cast_skill) + sizeof(int)*(size_t)cs.target_count) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(forbid_manager::IsForbidSkill(cs.skill_id))
			{
				error_cmd(S2C::ERR_FORBID_SKILL);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;

			//不检查选中对象的位置信息
			char force_attack = cs.force_attack;
			if(InSafeLock()) force_attack &= ~C2S::FORCE_ATTACK_WHITE;

			if(pImp->IsXuanYuan())
			{
				char dlType = GNET::SkillWrapper::GetDarkLightType(cs.skill_id);
				char skill_type = GNET::SkillWrapper::GetType(cs.skill_id);	
				if(dlType > 0 && (skill_type == 1 || skill_type == 3))
				{
					int idx = -1;
					for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
					{
						if((pImp->IsDarkLightForm(gplayer_imp::DLFORM_LIGHTUNIFORM) || pImp->IsDarkLightForm(gplayer_imp::DLFORM_DARKUNIFORM))
						&& pImp->GetDarkLightSpirit(i) == gplayer::SPIRIT_TYPE_DARKLIGHT && (dlType == 1 || dlType == 2))
						{
							idx = i;
							break;
						}
					}
					if(idx == -1)
					{
						for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
						{
							if( (pImp->IsDarkLightForm(gplayer_imp::DLFORM_SENIOR_DARK) && pImp->GetDarkLightSpirit(i) == gplayer::SPIRIT_TYPE_DARK && dlType == 1)
									|| (pImp->IsDarkLightForm(gplayer_imp::DLFORM_SENIOR_LIGHT) && pImp->GetDarkLightSpirit(i) == gplayer::SPIRIT_TYPE_LIGHT && dlType == 2))
							{
								idx = i;
								break;
							}
						}
					}
					if(idx >= 0 && idx < MAX_XUANYUAN_SPIRITS)
					{
						pImp->SetDarkLightSpirit(idx, gplayer::SPIRIT_TYPE_NONE);

						spirit_session_skill *pSkill= new spirit_session_skill(pImp);
						pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, cs.skillpos, idx);
						pImp->AddSpiritSession(pSkill);
						return 0;
					}
				}
			}

			int preCastSkillId = GNET::SkillWrapper::GetPreCastSkillId(cs.skill_id);
			if(preCastSkillId > 0)
			{
				act_session* cur_ses = pImp->GetCurSession();
				if(cur_ses && cur_ses->IsSkillPerforming(preCastSkillId))
				{
					session_skill *pSkill= new session_skill(pImp);
					pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, cs.skillpos);
					pImp->AddStartSession(pSkill);
					pImp->TryStopCurSession();
					break;
				}
				else if(GNET::SkillWrapper::IsCastInPreSkill(cs.skill_id))
				{
					pImp->_runner->error_message(S2C::ERR_SKILL_NOT_AVAILABLE);
					break;
				}
			}
			session_skill *pSkill= new session_skill(pImp);
			pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, cs.skillpos);
			pImp->AddStartSession(pSkill);
		}
		break;

		case C2S::CAST_INSTANT_SKILL:
		if(CheckDeny(CMD_SKILL)) return 0;
		{
			C2S::CMD::cast_instant_skill & cs = *(C2S::CMD::cast_instant_skill *)buf;
			if(size != sizeof(C2S::CMD::cast_instant_skill) + sizeof(int)*(size_t)cs.target_count) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(forbid_manager::IsForbidSkill(cs.skill_id))
			{
				error_cmd(S2C::ERR_FORBID_SKILL);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;

			//不检查选中对象的位置信息
			char force_attack = cs.force_attack;
			if(InSafeLock()) force_attack &= ~C2S::FORCE_ATTACK_WHITE;
			if(pImp->IsXuanYuan())
			{
				char dlType = GNET::SkillWrapper::GetDarkLightType(cs.skill_id);
				char skill_type = GNET::SkillWrapper::GetType(cs.skill_id);	
				if(dlType > 0 && (skill_type == 1 || skill_type == 3))
				{
					int idx = -1;
					for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
					{
						if((pImp->IsDarkLightForm(gplayer_imp::DLFORM_LIGHTUNIFORM) || pImp->IsDarkLightForm(gplayer_imp::DLFORM_DARKUNIFORM))
						&& pImp->GetDarkLightSpirit(i) == gplayer::SPIRIT_TYPE_DARKLIGHT && (dlType == 2 || dlType == 2))
						{
							idx = i;
							break;
						}
					}
					if(idx == -1)
					{
						for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
						{
							if( (pImp->IsDarkLightForm(gplayer_imp::DLFORM_SENIOR_DARK) && pImp->GetDarkLightSpirit(i) == gplayer::SPIRIT_TYPE_DARK && dlType == 1)
									|| (pImp->IsDarkLightForm(gplayer_imp::DLFORM_SENIOR_LIGHT) && pImp->GetDarkLightSpirit(i) == gplayer::SPIRIT_TYPE_LIGHT && dlType == 2))
							{
								idx = i;
								break;
							}
						}
					}
					if(idx >= 0 && idx < MAX_XUANYUAN_SPIRITS)
					{
						pImp->SetDarkLightSpirit(idx, gplayer::SPIRIT_TYPE_NONE);

						session_instant_skill *pSkill= new session_instant_skill(pImp);
						pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, cs.skillpos, idx);
						pImp->AddSpiritSession(pSkill);
						return 0;
					}
				}
			}

			int preCastSkillId = GNET::SkillWrapper::GetPreCastSkillId(cs.skill_id);
			if(preCastSkillId > 0)
			{
				act_session* cur_ses = pImp->GetCurSession();
				if(cur_ses && cur_ses->IsSkillPerforming(preCastSkillId))
				{
					session_skill *pSkill= new session_skill(pImp);
					pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, cs.skillpos);
					pImp->AddStartSession(pSkill);
					pImp->TryStopCurSession();
					break;
				}
				else if(GNET::SkillWrapper::IsCastInPreSkill(cs.skill_id))
				{
					pImp->_runner->error_message(S2C::ERR_SKILL_NOT_AVAILABLE);
					break;
				}
			}
			session_instant_skill *pSkill= new session_instant_skill(pImp);
			pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, cs.skillpos, -1);
			pImp->AddStartSession(pSkill);
		}
		break;
		
		case C2S::CAST_CHARGE_SKILL:
		if(CheckDeny(CMD_SKILL)) return 0;
		{
			C2S::CMD::cast_charge_skill & cs = *(C2S::CMD::cast_charge_skill *)buf;
			if(size != sizeof(C2S::CMD::cast_charge_skill) + sizeof(int)*(size_t)cs.target_count) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//不检查选中对象的位置信息
			char force_attack = cs.force_attack;
			if(InSafeLock()) force_attack &= ~C2S::FORCE_ATTACK_WHITE;
			session_instant_skill *pSkill= new session_instant_skill(pImp);
			pSkill->SetTarget(cs.skill_id,force_attack,cs.target_count,cs.targets, 0, -1, -1, false, A3DVECTOR(), -1);
			XID charge_target;
			MAKE_ID(charge_target,cs.charge_target);
			pSkill->SetCharge(cs.charge_pos, charge_target);
			pImp->AddStartSession(pSkill);
		}
		break;

		case C2S::CANCEL_ACTION:
		{
			if(size != sizeof(C2S::CMD::cancel_action))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;

			//加入一个阻止除了除了移动操作外的所有的命令的session
			session_cancel_action *pCancel= new session_cancel_action();
			pImp->AddStartSession(pCancel);

			//停止当前的session
			pImp->TryStopCurSession();
		}
		break;

		case C2S::USE_ITEM_WITH_TARGET:
		if(CheckDeny(CMD_ITEM)) return 0;
		{
			C2S::CMD::use_item_with_target & uiwt = *(C2S::CMD::use_item_with_target *)buf;
			if(size != sizeof(C2S::CMD::use_item_with_target)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(forbid_manager::IsForbidItem(uiwt.item_id))
			{
				error_cmd(S2C::ERR_FORBID_ITEM);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUseItemWithTarget(uiwt.where, uiwt.index, uiwt.item_id,uiwt.force_attack))
			{
				error_cmd(S2C::ERR_CANNOT_USE_ITEM);
			}
		}
		break;

		case C2S::SIT_DOWN:
		if(CheckDeny(CMD_MOVE)) return 0;
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerSitDown())
			{
				error_cmd(S2C::ERR_CANNOT_SIT_DOWN);
			}
			
		}
		break;

		case C2S::EMOTE_ACTION:
		{
			C2S::CMD::emote_action & tn = *(C2S::CMD::emote_action *)buf;
			if(size != sizeof(tn))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->CheckCoolDown(COOLDOWN_INDEX_EMOTE))
			{
				pImp->SetCoolDown(COOLDOWN_INDEX_EMOTE,EMOTE_COOLDOWN_TIME);
				if(pImp->HasEmoteState())
				{
					pImp->TryStopCurSession();
				}
				pImp->_runner->do_emote(tn.action);
			}
		}
		break;

		case C2S::TRICKS_ACTION:
		{
			C2S::CMD::tricks_action & ta = *(C2S::CMD::tricks_action *)buf;
			if(size != sizeof(ta))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			//现在先不考虑限制此操作的使用次数，如有需要， 以后可以考虑
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->_cur_session && pImp->_cur_session->GetGUID() == CLS_SESSION_MOVE) 
			{
				pImp->_runner->do_action(ta.action);
			}
		}
		break;

		case C2S::TASK_NOTIFY:
		{
			C2S::CMD::task_notify & tn = *(C2S::CMD::task_notify *)buf;
			if(size < sizeof(C2S::CMD::task_notify) || size != sizeof(C2S::CMD::task_notify) + tn.size) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			_load_stats += 10;
			PlayerTaskInterface  task_if((gplayer_imp*)_imp);
			OnClientNotify(&task_if,tn.buf,tn.size);
		}
		break;


		case C2S::ASSIST_SELECT:
		{
			C2S::CMD::assist_select & tn = *(C2S::CMD::assist_select *)buf;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(size != sizeof(C2S::CMD::assist_select))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(pImp->IsInTeam())
			{	
				XID target(GM_TYPE_PLAYER,tn.partner);
				pImp->PlayerAssistSelect(target);
			}
		}
		break;

		case C2S::CONTINUE_ACTION:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp; 
			pImp->PlayerRestartSession();
		}
		break;

		case C2S::STOP_FALL:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp; 
			if(pImp->_layer_ctrl.IsFalling())
			{
				pImp->_layer_ctrl.Ground();
			}
		}
		break;

		case C2S::GET_ITEM_INFO_LIST:
		{
			C2S::CMD::get_item_info_list & giil = *(C2S::CMD::get_item_info_list*)buf;
			if(size < sizeof(giil) || size != sizeof(giil) + giil.count)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetItemInfoList(giil.where,giil.count,giil.item_list);
		}
		break;

		case C2S::GATHER_MATERIAL:
		if(CheckDeny(CMD_PICKUP)) return 0;
		{
			C2S::CMD::gather_material & gm = *(C2S::CMD::gather_material*)buf;
			if(size != sizeof(gm))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			XID target;
			MAKE_ID(target,gm.mid);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(target.type == GM_TYPE_MATTER)
			{
				int task_id = gm.task_id;
				if(task_id > 0)
				{
					PlayerTaskInterface  task_if((gplayer_imp*)_imp);
					if(!task_if.CanDoMining(task_id))
					{
						break;
					}
				}

				int roleid = pImp->GetParent()->ID.id;
				int teamid = pImp->GetTeamID();
				int masterid = pImp->GetSectID();
				
				session_gather_prepare *pSession = new session_gather_prepare(pImp);
				pSession->SetTarget(target.id,gm.tool_where,gm.tool_index,gm.tool_type,task_id, roleid, teamid, masterid);
				pImp->AddStartSession(pSession);
			}
		}
		break;

		case C2S::GET_TRASHBOX_INFO:
		{
			C2S::CMD::get_trashbox_info & gti = *(C2S::CMD::get_trashbox_info*)buf;
			if(size != sizeof(gti))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerGetTrashBoxInfo(gti.detail);
		}
		break;
		
		case C2S::EXCHANGE_TRASHBOX_ITEM:
		{
			C2S::CMD::exchange_trashbox_item & eti = *(C2S::CMD::exchange_trashbox_item*) buf;
			if(sizeof(eti) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), eti.index1);
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), eti.index2);
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerExchangeTrashItem(eti.index1,eti.index2);
		}
		break;

		case C2S::MOVE_TRASHBOX_ITEM:
		{
			C2S::CMD::move_trashbox_item & mti = *(C2S::CMD::move_trashbox_item *) buf;
			if(sizeof(mti) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), mti.src);
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), mti.dest);
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerMoveTrashItem(mti.src,mti.dest,mti.amount);
		}
		break;
		
		case C2S::EXHCANGE_TRASHBOX_INVENTORY:
		{
			C2S::CMD::exchange_trashbox_inventory & eti = *(C2S::CMD::exchange_trashbox_inventory *) buf;
			if(sizeof(eti) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), eti.idx_tra);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eti.idx_inv);
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerExchangeTrashInv(eti.idx_tra,eti.idx_inv);
		}
		break;

		case C2S::MOVE_TRASHBOX_ITEM_TO_INVENTORY:
		{
			C2S::CMD::move_trashbox_item_to_inventory & mtiti = *(C2S::CMD::move_trashbox_item_to_inventory *) buf;
			if(sizeof(mtiti) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), mtiti.idx_tra);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mtiti.idx_inv);
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerTrashItemToInv(mtiti.idx_tra,mtiti.idx_inv,mtiti.amount);
		}
		break;
		
		case C2S::MOVE_INVENTORY_ITEM_TO_TRASHBOX:
		{
			C2S::CMD::move_inventory_item_to_trashbox & miitt  = *(C2S::CMD::move_inventory_item_to_trashbox *) buf;
			if(sizeof(miitt) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(pImp->GetTrashBoxOpenType(), miitt.idx_tra);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, miitt.idx_inv);
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerInvItemToTrash(miitt.idx_inv,miitt.idx_tra,miitt.amount);
		}
		break;
		
		case C2S::EXCHANGE_TRASHBOX_MONEY:
		{
			C2S::CMD::excnahge_trashbox_money & etm  = *(C2S::CMD::excnahge_trashbox_money *) buf;
			if(sizeof(etm) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->IsTrashBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			pImp->PlayerExchangeTrashMoney(etm.inv_money, etm.trashbox_money);
		}
		break;

		case C2S::SET_ADV_DATA:
		{
			C2S::CMD::set_adv_data & sad = *(C2S::CMD::set_adv_data*) buf;
			if(sizeof(sad) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer * pPlayer = (gplayer*)_imp->_parent;
			pPlayer->adv_data1 = sad.data1;
			pPlayer->adv_data2 = sad.data2;
			pPlayer->object_state |= gactive_object::STATE_ADV_MODE;
			_imp->_runner->set_adv_data(sad.data1,sad.data2);
		}
		break;

		case C2S::CLR_ADV_DATA:
		{
			gplayer * pPlayer = (gplayer*)_imp->_parent;
			if(pPlayer->object_state & gactive_object::STATE_ADV_MODE)
			{
				pPlayer->object_state &= ~gactive_object::STATE_ADV_MODE;
				_imp->_runner->clear_adv_data();
			}
		}
		break;

		case C2S::TEAM_LFG_REQUEST:
		{
			C2S::CMD::team_lfg_request & tlr  = *(C2S::CMD::team_lfg_request *) buf;
			if(sizeof(tlr) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->IsInTeam())
			{
				msg_apply_party msg;
				msg.faction = pImp->GetFaction(); 
				msg.family_id = pImp->GetParent()->id_family; 
				msg.mafia_id = pImp->GetParent()->id_mafia; 
				// Youshuang add
				msg.level = ( pImp->GetRebornCount() == 0 ) ? pImp->GetParent()->level : 0;
				msg.sectid = pImp->GetParent()->sect_id;
				msg.referid = pImp->GetReferID();
				// end
				
				//发送特定的消息
				pImp->SendTo<0>(GM_MSG_TEAM_APPLY_PARTY,XID(GM_TYPE_PLAYER,tlr.id),0, &msg, sizeof(msg));
			}
		}
		break;

		case C2S::TEAM_LFG_REPLY:
		{
			C2S::CMD::team_lfg_reply & tlr  = *(C2S::CMD::team_lfg_reply *) buf;
			if(sizeof(tlr) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_team.AgreeApply(tlr.id,tlr.result);
		}
		break;

		case  C2S::QUERY_PLAYER_INFO_1:
		{
			C2S::CMD::query_player_info_1 & qpi = *(C2S::CMD::query_player_info_1 *) buf;
			if(size < sizeof(C2S::CMD::query_player_info_1) || 
					qpi.count >= 256 ||
					size != sizeof(C2S::CMD::query_player_info_1)+qpi.count*sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			_load_stats += qpi.count;
			//对玩家 进行特殊处理
			((gplayer_imp*)_imp)->QueryOtherPlayerInfo1(qpi.count,qpi.id);
		}
		break;

		case  C2S::QUERY_NPC_INFO_1:
		{
			C2S::CMD::query_npc_info_1 & qni = *(C2S::CMD::query_npc_info_1 *) buf;
			if(size < sizeof(C2S::CMD::query_npc_info_1) || 
					qni.count >= 256 ||
					size != sizeof(C2S::CMD::query_npc_info_1)+qni.count*sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			_load_stats += qni.count;
			((gplayer_imp*)_imp)->QueryNPCInfo1(qni.count,qni.id);
		}
		break;

		case C2S::SESSION_EMOTE_ACTION:
		{
			C2S::CMD::session_emote_action & sea = *(C2S::CMD::session_emote_action *) buf;
			if(size != sizeof(sea))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			//加入动作的session
			if(sea.action <= 0) break;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->CheckCoolDown(COOLDOWN_INDEX_EMOTE))
			{
				pImp->SetCoolDown(COOLDOWN_INDEX_EMOTE,EMOTE_COOLDOWN_TIME);
				session_emote_action *pSession = new session_emote_action(pImp);
				pSession->SetAction(sea.action);
				pImp->AddStartSession(pSession);
			}
		}
		break;

		case C2S::TEAM_CHANGE_LEADER:
		{
			C2S::CMD::team_change_leader & tcl   = *(C2S::CMD::team_change_leader *) buf;
			if(size != sizeof(tcl))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//现在转换队长的操作还很粗糙
			pImp->TeamChangeLeader(tcl.new_leader);
		}
		break;

		case C2S::ENTER_SANCTUARY:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->TestSanctuary();
			//不用session去做， 因为上线的时候如果发这个会去掉无敌的session
	//		pImp->AddStartSession(new session_enter_sanctuary(pImp));
			_load_stats ++;

		}
		break;

		case C2S::OPEN_PERSONAL_MARKET:
		if(CheckDeny(CMD_MARKET)) return 0;
		if(!TestSafeLock()) return 0;
		{
			typedef C2S::CMD::open_personal_market cmd_t;
			cmd_t & pmo = *(cmd_t *) buf;
			if(size < sizeof(pmo) || !pmo.count || size != sizeof(pmo) + pmo.count * sizeof(cmd_t::entry_t))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//试图建立摊位
			if(!pImp->PlayerOpenPersonalMarket(pmo.index, pmo.item_id, pmo.count,pmo.name ,(int *)pmo.list))
			{
				error_cmd(S2C::ERR_CANNOT_OPEN_PLAYER_MARKET);
			}
		}
		break;

		case C2S::CANCEL_PERSONAL_MARKET:
		{
			gplayer_imp * pImp= (gplayer_imp *) _imp;
			pImp->CancelPersonalMarket();
		}
		break;

		case C2S::QUERY_PERSONAL_MARKET_NAME:
		{
			typedef C2S::CMD::query_personal_market_name cmd_t;
			cmd_t & qpmn = *(cmd_t *) buf;
			if(size < sizeof(qpmn) || !qpmn.count || size != sizeof(qpmn) + qpmn.count * sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(qpmn.count > 128) qpmn.count = 128;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			_load_stats += qpmn.count;

			//注：这个是一个非常耗费资源的操作 需要限制玩家的发送速度
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			MSG msg;
			int cs_index = pPlayer->cs_index;
			BuildMessage(msg,GM_MSG_QUERY_MARKET_NAME,XID(-1,-1),pPlayer->ID,
						pPlayer->pos,pPlayer->cs_sid,&cs_index, sizeof(cs_index));

			//针对player id 进行群体发送
			gmatrix::SendPlayerMessage( qpmn.list, qpmn.count, msg);
			
		}
		break;

		case C2S::DESTROY_ITEM:
		{
			if(!TestSafeLock()) return 0;
			C2S::CMD::destroy_item  & di   = *(C2S::CMD::destroy_item *) buf;
			if(size != sizeof(di))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//pImp->PlayerDestroyItem(di.where,di.index,di.type);
			pImp->_runner->unlock_inventory_slot(di.where, di.index);
		}
		break;

		case C2S::TEST_PERSONAL_MARKET:
		{
			C2S::CMD::test_personal_market data = *(C2S::CMD::test_personal_market *)buf;
			if(size != sizeof(data))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(!TestSafeLock()) return 0;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerTestPersonalMarket(data.index, data.item_id))
			{
				error_cmd(S2C::ERR_CANNOT_OPEN_PLAYER_MARKET);
			}
		}
		break;

		case C2S::SWITCH_FASHION_MODE:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_SWITCH_FASHION))
			{
				pImp->SetCoolDown(COOLDOWN_INDEX_SWITCH_FASHION,FASHION_COOLDOWN_TIME);
				pImp->SwitchFashionMode();
			}
		}
		break;

		case C2S::SET_FASHION_MASK:
		{
			C2S::CMD::set_fashion_mask  & rt = *(C2S::CMD::set_fashion_mask *) buf;
			if(size != sizeof(rt))
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_SET_FASHION_MASK))
			{
				pImp->SetCoolDown(COOLDOWN_INDEX_SET_FASHION_MASK,200);
				pImp->SetFashionMask(rt.fashion_mask);
			}
		}
		break;

		case C2S::REGION_TRANSPORT:
		if(CheckDeny(CMD_MOVE)) return 0;
		{
			C2S::CMD::region_transport  & rt   = *(C2S::CMD::region_transport *) buf;
			if(size != sizeof(rt))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerRegionTransport(rt.region_index, rt.target_tag);
		}
		break;

		case C2S::NOTIFY_POS_TO_MEMBER:
		{
			_load_stats += 10;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->IsInTeam()) break;
			
			packet_wrapper  h1(64);
			using namespace S2C;
			CMD::Make<CMD::teammate_pos>::From(h1,pImp->_parent->ID,pImp->_parent->pos,pImp->GetWorldTag());
			pImp->_team.SendGroupData(h1.data(),h1.size());
		}
		break;

		case C2S::CAST_POS_SKILL:
		if(CheckDeny(CMD_SKILL)) return 0;
		{
			C2S::CMD::cast_pos_skill & cs = *(C2S::CMD::cast_pos_skill *)buf;
			if(size != sizeof(C2S::CMD::cast_pos_skill))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(forbid_manager::IsForbidSkill(cs.skill_id))
			{
				error_cmd(S2C::ERR_FORBID_SKILL);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;

			//不检查选中对象的位置信息
			session_pos_skill *pSkill= new session_pos_skill(pImp);
			pSkill->SetTarget(cs.skill_id,cs.pos);
			pImp->AddStartSession(pSkill);
		}
		break;
		
		case C2S::QUERY_DOUBLE_EXP_INFO:
		{
		}
		break;

		case C2S::DUEL_REQUEST:
		{
			C2S::CMD::duel_request & dr= *(C2S::CMD::duel_request *)buf;
			if(size != sizeof(C2S::CMD::duel_request))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(_imp->_plane->GetWorldManager()->GetWorldLimit().noduel)
			{
				error_cmd(S2C::ERR_HERE_CAN_NOT_DUEL);
				break;
			}

			XID target;
			MAKE_ID(target,dr.target);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerDuelRequest(target);
			}

		}
		break;

		case C2S::DUEL_REPLY:
		{
			C2S::CMD::duel_reply & dr= *(C2S::CMD::duel_reply *)buf;
			if(size != sizeof(C2S::CMD::duel_reply))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(_imp->_plane->GetWorldManager()->GetWorldLimit().noduel)
			{
				error_cmd(S2C::ERR_HERE_CAN_NOT_DUEL);
				break;
			}
			XID target;
			MAKE_ID(target,dr.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerDuelReply(target,dr.param);
			}
		}
		break;

		case C2S::BIND_PLAYER_REQUEST:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bind_player_request & bpr= *(C2S::CMD::bind_player_request *)buf;
			if(size != sizeof(bpr))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,bpr.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerBindRequest(target);
			}

		}
		break;

		case C2S::BIND_PLAYER_INVITE:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bind_player_invite & bpi= *(C2S::CMD::bind_player_invite *)buf;
			if(size != sizeof(bpi))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,bpi.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerBindInvite(target);
			}
		}
		break;

		case C2S::BIND_PLAYER_REQUEST_REPLY:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bind_player_request_reply & dr= *(C2S::CMD::bind_player_request_reply *)buf;
			if(size != sizeof(dr))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,dr.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerBindRequestReply(target,dr.param);
			}
		}
		break;

		case C2S::BIND_PLAYER_INVITE_REPLY:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bind_player_invite_reply & dr= *(C2S::CMD::bind_player_invite_reply *)buf;
			if(size != sizeof(dr))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,dr.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerBindInviteReply(target,dr.param);
			}
		}
		break;

		case C2S::BIND_PLAYER_CANCEL:
		//要求取消绑定状态
		{
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerBindCancel();
		}
		return 0;

		case C2S::QUERY_OTHER_EQUIP_DETAIL:
		{
			C2S::CMD::query_other_equip_detail & goed = *(C2S::CMD::query_other_equip_detail*)buf;
			if(sizeof(goed) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			_load_stats ++;

			XID target;
			MAKE_ID(target,goed.target);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				if(!pImp->CheckCoolDown(COOLDOWN_INDEX_QUERY_OTHER_EQUIP))
				{
					error_cmd(S2C::ERR_SKILL_IS_COOLING);
					break;
				}

				struct 
				{
					int cs_index;
					int cs_sid;
				}data;

				gplayer * pPlayer = (gplayer*)(pImp->_parent);
				data.cs_index = pPlayer->cs_index;
				data.cs_sid = pPlayer->cs_sid;

				//发送查询消息
				pImp->SendTo<0>(GM_MSG_QUERY_EQUIP_DETAIL,target,
						pImp->GetFaction(), &data,sizeof(data));

				//设置冷却时间
				pImp->SetCoolDown(COOLDOWN_INDEX_QUERY_OTHER_EQUIP,QUERY_OTHER_EQUIP_COOLDOWN_TIME);
			}
		}
		break;

		case C2S::PRODUCE_ITEM:
		{
			C2S::CMD::produce_item & pi = *(C2S::CMD::produce_item*)buf;
			if(size != sizeof(pi))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerProduceItem(pi.recipe_id);
		}
		break;

		case C2S::MALL_SHOPPING:
		{
			C2S::CMD::mall_shopping & ms = *(C2S::CMD::mall_shopping *)buf;
			if(size < sizeof(ms) || ms.count == 0 ||  ms.count > 65535 ||
			size != sizeof(ms) + ms.count *sizeof(C2S::CMD::mall_shopping::__entry))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			//gplayer_imp * pImp = (gplayer_imp*)_imp;
			//pImp->PlayerDoShopping(ms.count, (const short *)&ms.list);
			//pImp->PlayerDoShopping(ms.list[0].goods_id, ms.list[0].goods_index, ms.list[0].goods_slot, 10);
		}
		break;

		case C2S::MALL_SHOPPING2:
		{
			C2S::CMD::mall_shopping_2 & ms2 = *(C2S::CMD::mall_shopping_2 *)buf;
			if(size != sizeof(ms2) || ms2.count == 0)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			// Youshuang add
			//DATA_TYPE dt1;
			//const COLORANT_ITEM_ESSENCE &ess1 = *(const COLORANT_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr( ms2.goods_id, ID_SPACE_ESSENCE, dt1 );
			//if( dt1 == DT_COLORANT_ITEM_ESSENCE && &ess1 != NULL )
			//{
			//	ms2.goods_id = player_template::GetInstance().GetRandomGoodsID();
			//}
			// end
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!gmatrix::GetWorldParam().billing_shop){
				if(pImp->PlayerDoShopping(ms2.goods_id, ms2.goods_index, ms2.goods_slot, ms2.count))
				{
					pImp->_runner->player_cash(pImp->_mallinfo.GetCash(), pImp->_mallinfo.GetCashUsed(), pImp->_mallinfo.GetCashAdd()); 
				}
			}
			else
			{
				if(pImp->PlayerDoBillingShoppingStep1(ms2.goods_id, ms2.goods_index, ms2.goods_slot, ms2.count))
				{
					
					pImp->_runner->player_cash(pImp->GetBillingCash(), -1, -1);
				}
			}
		}
		break;

		case C2S::SELECT_TITLE:
		{
			C2S::CMD::select_title & st = *(C2S::CMD::select_title *)buf;
			if(size != sizeof(st)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SelectPlayerTitle(st.title);
		}
		break;

		case C2S::LOTTERY_CASHING:
		{
			C2S::CMD::lottery_cashing & lc = *(C2S::CMD::lottery_cashing *)buf;
			if(size != sizeof(C2S::CMD::lottery_cashing)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerLotteryCashing(lc.item_index);
		}
		break;

		case C2S::CHECK_RECORDER:
		{
			C2S::CMD::check_recorder & cr = *(C2S::CMD::check_recorder *)buf;
			if(size != sizeof(C2S::CMD::check_recorder)) 
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->QueryRecorder(cr.item_index);
		}
		break;


		case C2S::AUTO_BOT_BEGIN:
		if(CheckDeny(CMD_BOT)) return 0;
		{
			C2S::CMD::auto_bot_begin & abb = *(C2S::CMD::auto_bot_begin *)buf;
			if(size != sizeof(C2S::CMD::auto_bot_begin)) 
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerTalismanBotBegin(abb.active, abb.need_question );
		}
		break;

		case C2S::AUTO_BOT_CONTINUE:
		if(CheckDeny(CMD_BOT)) return 0;
		{
			C2S::CMD::auto_bot_continue & abb = *(C2S::CMD::auto_bot_continue *)buf;
			if(size != sizeof(abb)) 
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerTalismanBotContinue();
		}
		break;

		case C2S::GET_BATTLE_SCORE:
		{
			C2S::CMD::get_battle_score & gbs = *(C2S::CMD::get_battle_score*)buf;
			if( sizeof(gbs) != size )
			{
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_runner->battle_score( pImp->GetBattleScore() );
		}
		break;

		case C2S::GET_BATTLE_INFO:
		{
			C2S::CMD::get_battle_info& gbs = *(C2S::CMD::get_battle_info*)buf;
			if( sizeof(gbs) != size )
			{
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetBattleInfo();
		}
		break;

		case C2S::GET_RAID_INFO:
		{
			
			C2S::CMD::get_raid_info& gbs = *(C2S::CMD::get_raid_info*)buf;
			if( sizeof(gbs) != size )
			{
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetRaidInfo();
		};
		break;

		case C2S::GET_RAID_COUNT:
		{
			
			C2S::CMD::get_raid_count& grc = *(C2S::CMD::get_raid_count*)buf;
			if( sizeof(grc) != size )
			{
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_runner->player_single_raid_counter(grc.map_id);
		};
		break;

		case C2S::EQUIP_PET_BEDGE:
		{
			C2S::CMD::equip_pet_bedge& epb = *(C2S::CMD::equip_pet_bedge*)buf;
			if(sizeof(C2S::CMD::equip_pet_bedge) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerEquipPetBedge(epb.inv_index,epb.pet_index);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,epb.inv_index);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_PET_BEDGE,epb.pet_index);
		}
		break;

		case C2S::MOVE_PET_BEDGE:
		{
			C2S::CMD::move_pet_bedge& mpb = *(C2S::CMD::move_pet_bedge*)buf;
			if(sizeof(C2S::CMD::move_pet_bedge) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerMovePetBedge(mpb.src_index,mpb.dst_index);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,mpb.src_index);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_PET_BEDGE,mpb.dst_index);
		}
		break;

		case C2S::EQUIP_PET_EQUIP:
		{
			C2S::CMD::equip_pet_equip& epe = *(C2S::CMD::equip_pet_equip*)buf;
			if(sizeof(C2S::CMD::equip_pet_equip) != size )
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerEquipPetEquip(epe.inv_index,epe.pet_index);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,epe.inv_index);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_PET_EQUIP,epe.pet_index);
		}
		break;

		case C2S::SUMMON_PET:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::summon_pet & sp = *(C2S::CMD::summon_pet *)buf;
			if(size != sizeof(C2S::CMD::summon_pet))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int rst = pImp->PlayerSummonPet(sp.pet_index);
			if(rst)
			{
				error_cmd(rst);
			}
		}
		break;

		case C2S::RECALL_PET:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::recall_pet& rp = *(C2S::CMD::recall_pet*)buf;
			if(size != sizeof(C2S::CMD::recall_pet))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int rst = pImp->PlayerRecallPet(rp.pet_index);
			if(rst)
			{
				error_cmd(rst);
			}
		}
		break;

		case C2S::COMBINE_PET:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::combine_pet& cp = *(C2S::CMD::combine_pet*)buf;
			if(size != sizeof(C2S::CMD::combine_pet))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int rst = pImp->PlayerCombinePet(cp.pet_index,cp.combine_type);
			if(rst)
			{
				error_cmd(rst);
			}
		}
		break;

		case C2S::UNCOMBINE_PET:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::uncombine_pet& ucp = *(C2S::CMD::uncombine_pet*)buf;
			if(size != sizeof(C2S::CMD::uncombine_pet))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int rst = pImp->PlayerUncombinePet(ucp.pet_index);
			if(rst)
			{
				error_cmd(rst);
			}
		}
		break;

		case C2S::SET_PET_RANK:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::set_pet_rank& spr = *(C2S::CMD::set_pet_rank*)buf;
			if(size != sizeof(C2S::CMD::set_pet_rank))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int rst = pImp->PlayerSetPetRank(spr.pet_index,spr.pet_tid,spr.rank);
			if(rst)
			{
				error_cmd(rst);
			}
		}
		break;

		case C2S::SET_PET_STATUS:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::set_pet_status& sps = *(C2S::CMD::set_pet_status*)buf;
			if(size != sizeof(C2S::CMD::set_pet_status))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int rst = pImp->PlayerSetPetStatus(sps.pet_index,sps.pet_tid,sps.main_status,sps.sub_status);
			if(rst)
			{
				error_cmd(rst);
			}
		}
		break;

		case C2S::PET_CTRL_CMD:
		{
			if(CheckDeny(CMD_PET)) return 0;
			C2S::CMD::pet_ctrl_cmd & pcc = *(C2S::CMD::pet_ctrl_cmd *)buf;
			if(size < sizeof(C2S::CMD::pet_ctrl_cmd))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerSendPetCommand(pcc.pet_index,pcc.target,pcc.pet_cmd,pcc.buf,size - sizeof(C2S::CMD::pet_ctrl_cmd));
		}
		break;

		case C2S::START_FLY:
		if(CheckDeny(CMD_FLY)) return 0;
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStartFly();
		}
		break;

		case C2S::STOP_FLY:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStopFly();
		}
		break;

		case C2S::START_ONLINE_AGENT:
		{
			gplayer_imp *pImp= (gplayer_imp*)_imp;
			if(pImp->HasSession()) break;
			if(!pImp->_filters.IsFilterExist(FILTER_INDEX_PVPLIMIT))	//要求在河阳城且在安全区才能继续操作
			{
				error_cmd(S2C::ERR_ONLINE_AGENT_POS);
				break;
			}
			session_online_agent *pSession = new session_online_agent(pImp);
			pImp->AddStartSession(pSession);
		}
		break;

		case C2S::JOIN_INSTANCE:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::join_instance & ji = *(C2S::CMD::join_instance*)buf;
			object_interface oi((gplayer_imp *)_imp);
			GNET::SendInstancingJoinToDelivery(pImp->_parent->ID.id,gmatrix::GetServerIndex(),ji.battle_id,oi);
		}
		break;

		case C2S::BIND_RIDE_INVITE:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bind_ride_invite& bri= *(C2S::CMD::bind_ride_invite*)buf;
			if(size != sizeof(bri))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,bri.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerRideInvite(target);
			}
		}
		break;

		case C2S::BIND_RIDE_INVITE_REPLY:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bind_ride_invite_reply & bir= *(C2S::CMD::bind_ride_invite_reply*)buf;
			if(size != sizeof(bir))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,bir.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerRideInviteReply(target,bir.param);
			}
		}
		break;

		case C2S::BIND_RIDE_CANCEL:
		{
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerRideCancel();
		}
		break;

		case C2S::BIND_RIDE_KICK:
		{
			C2S::CMD::bind_ride_kick & bir= *(C2S::CMD::bind_ride_kick*)buf;
			if(size != sizeof(bir))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerRideKick(bir.pos);
		}
		break;

		case C2S::POST_SNS_MESSAGE:
		{
			C2S::CMD::post_sns_message & psm= *(C2S::CMD::post_sns_message*)buf;
			if(size > 4096 || size != sizeof(psm) + sizeof(char)*(size_t)psm.message_len)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerPostMessage(psm.message_type, psm.occupation, psm.gender, psm.level,
				psm.faction_level, psm.message_len, psm.message);
		}
		break;

		case C2S::APPLY_SNS_MESSAGE:
		{
			C2S::CMD::apply_sns_message & psm= *(C2S::CMD::apply_sns_message*)buf;
			if(size > 4096 || size != sizeof(psm) + sizeof(char)*(size_t)psm.message_len)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerApplyMessage(psm.message_type, psm.message_id, psm.message_len, 
				psm.message);
		}
		break;

		case C2S::VOTE_SNS_MESSAGE:
		{
			C2S::CMD::vote_sns_message & vsm= *(C2S::CMD::vote_sns_message*)buf;
			if(size != sizeof(vsm))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerVoteMessage(vsm.vote_type, vsm.message_type, vsm.message_id); 
		}
		break;

		case C2S::RESPONSE_SNS_MESSAGE:
		{
			C2S::CMD::response_sns_message & rsm= *(C2S::CMD::response_sns_message*)buf;
			if(size > 4096 || size != sizeof(rsm) + sizeof(char)*(size_t)rsm.message_len)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerResponseMessage(rsm.message_type, rsm.message_id, rsm.dst_index,
				 rsm.message_len, rsm.message);
		}
		break;

		case C2S::EXCHANGE_POCKET_ITEM:
		{
			C2S::CMD::exchange_pocket_item & epi = *(C2S::CMD::exchange_pocket_item*) buf;
			if(sizeof(epi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangePocketItem(epi.index1,epi.index2);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, epi.index1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, epi.index2);
		}
		break;

		case C2S::MOVE_POCKET_ITEM:
		{
			C2S::CMD::move_pocket_item & mpi = *(C2S::CMD::move_pocket_item *) buf;
			if(sizeof(mpi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerMovePocketItem(mpi.src,mpi.dest,mpi.amount);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, mpi.src);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, mpi.dest);
		}
		break;

		case C2S::EXCHANGE_INVENTORY_POCKET_ITEM:
		{
			C2S::CMD::exchange_inventory_pocket_item &eip = *(C2S::CMD::exchange_inventory_pocket_item *) buf;
			if(sizeof(eip) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eip.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, eip.idx_poc);
			pImp->PlayerExchangeInvPocket(eip.idx_poc,eip.idx_inv);
		}
		break;

		case C2S::MOVE_POCKET_ITEM_TO_INVENTORY:
		{
			C2S::CMD::move_pocket_item_to_inventory & mpiti = *(C2S::CMD::move_pocket_item_to_inventory *) buf;
			if(sizeof(mpiti) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, mpiti.idx_poc);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, mpiti.idx_inv);
			pImp->PlayerMovePocketItemToInv(mpiti.idx_poc,mpiti.idx_inv,mpiti.amount);
		}
		break;
		
		case C2S::MOVE_INVENTORY_ITEM_TO_POCKET:
		{
			C2S::CMD::move_inventory_item_to_pocket& miitp  = *(C2S::CMD::move_inventory_item_to_pocket*) buf;
			if(sizeof(miitp) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, miitp.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_POCKET, miitp.idx_poc);
			pImp->PlayerMoveInvItemToPocket(miitp.idx_inv,miitp.idx_poc,miitp.amount);
		}
		break;
		
		case C2S::MOVE_ALL_POCKET_ITEM_TO_INVENTORY:
		{
			C2S::CMD::move_all_pocket_item_to_inventory& mapi= *(C2S::CMD::move_all_pocket_item_to_inventory *)buf;
			if( sizeof(mapi) != size )
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerMoveAllPocketItemToInv();
		}
		break;

		case C2S::MOVE_MAX_POCKET_ITEM_TO_INVENTORY:
		{
			C2S::CMD::move_max_pocket_item_to_inventory& mxpi= *(C2S::CMD::move_max_pocket_item_to_inventory *)buf;
			if( sizeof(mxpi) != size )
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerMoveMaxPocketItemToInv();
		}
		break;
		
		case C2S::CAST_ITEM_SKILL:
		if(CheckDeny(CMD_SKILL)) return 0;
		{
			C2S::CMD::cast_item_skill& cis= *(C2S::CMD::cast_item_skill*)buf;
			if(size != sizeof(C2S::CMD::cast_item_skill) + sizeof(int)*(size_t)cis.target_count) 
			{
				break;
			}

			if(forbid_manager::IsForbidSkill(cis.skill_id))
			{
				error_cmd(S2C::ERR_FORBID_SKILL);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//不检查选中对象的位置信息
			char force_attack = cis.force_attack;
			if(InSafeLock()) force_attack &= ~C2S::FORCE_ATTACK_WHITE;
			pImp->PlayerCastItemSkill(cis.skill_id, cis.skill_type, cis.item_id, cis.item_index, force_attack, cis.target_count, cis.targets);
		}
		break;

		case C2S::UPDATE_FASHION_HOTKEY:
		{
			C2S::CMD::update_fashion_hotkey & ufh = *(C2S::CMD::update_fashion_hotkey*) buf;
			if(size != sizeof(C2S::CMD::update_fashion_hotkey) + sizeof(C2S::CMD::update_fashion_hotkey::key_combine)*size_t(ufh.count))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerSaveFashionKey(ufh.count, ufh.key);
		}
		break;

		case C2S::EXCHANGE_FASHION_ITEM:
		{
			C2S::CMD::exchange_fashion_item & efi = *(C2S::CMD::exchange_fashion_item*) buf;
			if(sizeof(efi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeFashionItem(efi.index1,efi.index2);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, efi.index1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, efi.index2);
		}
		break;

		case C2S::EXCHANGE_MOUNT_WING_ITEM:
		{
			C2S::CMD::exchange_mount_wing_item & efi = *(C2S::CMD::exchange_mount_wing_item*) buf;
			if(sizeof(efi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeMountWingItem(efi.index1,efi.index2);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, efi.index1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, efi.index2);
		}
		break;
		
		case C2S::EXCHANGE_INVENTORY_FASHION_ITEM:
		{
			C2S::CMD::exchange_inventory_fashion_item & eifi = *(C2S::CMD::exchange_inventory_fashion_item*) buf;
			if(sizeof(eifi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeInvFashion(eifi.idx_inv,eifi.idx_fas);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eifi.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, eifi.idx_fas);
		}
		break;

		case C2S::EXCHANGE_INVENTORY_MOUNTWING_ITEM:
		{
			C2S::CMD::exchange_inventory_mountwing_item & eifi = *(C2S::CMD::exchange_inventory_mountwing_item*) buf;
			if(sizeof(eifi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeInvMountWing(eifi.idx_inv,eifi.idx_mw);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, eifi.idx_inv);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, eifi.idx_mw);
		}
		break;
		
		case C2S::EXCHANGE_EQUIPMENT_FASHION_ITEM:
		{
			C2S::CMD::exchange_equipment_fashion_item & eefi = *(C2S::CMD::exchange_equipment_fashion_item*) buf;
			if(sizeof(eefi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeEquipFashion(eefi.idx_equ,eefi.idx_fas);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eefi.idx_equ);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, eefi.idx_fas);
		}
		break;

		case C2S::EXCHANGE_EQUIPMENT_MOUNTWING_ITEM:
		{
			C2S::CMD::exchange_equipment_mountwing_item & eefi = *(C2S::CMD::exchange_equipment_mountwing_item*) buf;
			if(sizeof(eefi) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeEquipMountWing(eefi.idx_equ,eefi.idx_mw);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, eefi.idx_equ);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_MOUNT_WING, eefi.idx_mw);
		}
		break;

		case C2S::BONUSMALL_SHOPPING:
		{
			C2S::CMD::bonusmall_shopping & bms = *(C2S::CMD::bonusmall_shopping *)buf;
			if(size != sizeof(bms) || bms.count == 0)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!gmatrix::GetWorldParam().billing_shop){
				if(pImp->PlayerDoBonusShopping(bms.goods_id, bms.goods_index, bms.goods_slot, bms.count))
				{
					pImp->_runner->player_bonus(pImp->_bonusinfo.GetBonus(), pImp->_bonusinfo.GetBonusUsed());
				}
			}
			else
			{
				//韩国版本暂时不支持鸿利商城
				error_cmd(S2C::ERR_FATAL_ERR);
			}
		}
		break;

		case C2S::UPDATE_PVP_MASK:
		{
			C2S::CMD::update_pvp_mask &upm = *(C2S::CMD::update_pvp_mask *)buf;
			if(size != sizeof(upm))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->UpdatePVPMask(upm.pvp_mask);
		}
		break;
		
		case C2S::QUERY_OTHERS_ACHIEVEMENT:
		{
			C2S::CMD::query_others_achievement & goa = *(C2S::CMD::query_others_achievement *)buf;
			if(sizeof(goa) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			XID target;
			MAKE_ID(target,goa.target);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				if(!pImp->CheckCoolDown(COOLDOWN_INDEX_QUERY_OTHER_ACHIEVMENT))
				{
					error_cmd(S2C::ERR_SKILL_IS_COOLING);
					break;
				}

				struct 
				{
					int cs_index;
					int cs_sid;
				}data;

				gplayer * pPlayer = (gplayer*)(pImp->_parent);
				data.cs_index = pPlayer->cs_index;
				data.cs_sid = pPlayer->cs_sid;

				//发送查询消息
				pImp->SendTo<0>(GM_MSG_QUERY_ACHIEVEMENT,target,
						pImp->GetFaction(), &data,sizeof(data));

				//设置冷却时间
				pImp->SetCoolDown(COOLDOWN_INDEX_QUERY_OTHER_ACHIEVMENT, QUERY_OTHER_ACHIEVMENT_COOLDOWN_TIME);
			}
		}
		break;
		
		case C2S::START_TRANSFORM:
		if(CheckDeny(CMD_TRANSFORM)) return 0;
		{
			C2S::CMD::start_transform &sat = *(C2S::CMD::start_transform *)buf;
			if(size != sizeof(sat))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStartTransform();
		}
		break;
		
		case C2S::STOP_TRANSFORM:
		{
			C2S::CMD::stop_transform &sot = *(C2S::CMD::stop_transform *)buf;
			if(size != sizeof(sot))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStopTransform();
		}
		break;
		
		case C2S::CAST_TRANSFORM_SKILL:
		{
			C2S::CMD::cast_transform_skill& cts= *(C2S::CMD::cast_transform_skill*)buf;
			if(size != sizeof(C2S::CMD::cast_transform_skill) + sizeof(int)*(size_t)cts.target_count) 
			{
				break;
			}

			if(forbid_manager::IsForbidSkill(cts.skill_id))
			{
				error_cmd(S2C::ERR_FORBID_SKILL);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//不检查选中对象的位置信息
			char force_attack = cts.force_attack;
			if(InSafeLock()) force_attack &= ~C2S::FORCE_ATTACK_WHITE;
			pImp->PlayerCastTransformSkill(cts.skill_id, cts.skill_type, force_attack, cts.target_count, cts.targets, cts.pos);
		}
		break;

		case C2S::ENTER_CARRIER:
		{
			C2S::CMD::enter_carrier & ec = *(C2S::CMD::enter_carrier*)buf;
			if(size != sizeof(C2S::CMD::enter_carrier)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerEnterCarrier(ec.carrier_id, ec.rpos, ec.rdir))
			{
				pImp->_runner->player_enter_carrier(0, A3DVECTOR(0.0f, 0.0f, 0.0f), 0, false);

			}
		}
		break;

		case C2S::LEAVE_CARRIER:
		{
			C2S::CMD::leave_carrier & lc = *(C2S::CMD::leave_carrier*)buf;
			if(size != sizeof(C2S::CMD::leave_carrier)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerLeaveCarrier(lc.carrier_id, lc.pos, lc.dir))
			{
				pImp->_runner->player_leave_carrier(lc.carrier_id,pImp->GetParent()->pos,pImp->GetParent()->dir, false);
			}
		}
		break;

		case C2S::MOVE_ON_CARRIER:
		if(CheckDeny(CMD_MOVE)) return 0;
		{
			if(size != sizeof(C2S::CMD::move_on_carrier)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer* pPlayer = pImp->GetParent();
			if(!pPlayer->CheckExtraState(gplayer::STATE_CARRIER)) 
			{
				return 0;
			}
			cmd_user_move(buf,size, true);
		}
		break;

		case C2S::STOP_MOVE_ON_CARRIER:
		{
			if(size != sizeof(C2S::CMD::stop_move_on_carrier)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer* pPlayer = pImp->GetParent();
			if(!pPlayer->CheckExtraState(gplayer::STATE_CARRIER))
			{
				return 0;
			}

			cmd_user_stop_move(buf,size,true);
		}
		break;

		case C2S::EXCHANGE_HOMETOWN_MONEY:
		{
			C2S::CMD::exchange_hometown_money & ehm  = *(C2S::CMD::exchange_hometown_money*)buf;
			if(size != sizeof(C2S::CMD::exchange_hometown_money)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			object_interface oi((gplayer_imp *)_imp);
			GNET::SendExchangeHometownMoney(pImp->_parent->ID.id,ehm.amount,oi);
		}
		break;

		case C2S::TERRITORY_LEAVE:
		{
			if(size != sizeof(C2S::CMD::territory_leave)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerLeaveTerritory();
		}
		break;

		case C2S::RAID_LEAVE:
		{
			if(size != sizeof(C2S::CMD::raid_leave))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerLeaveRaid(0);
		}
		break;

		case C2S::UPDATE_COMBINE_SKILL:
		{
			C2S::CMD::update_combine_skill & ucs = *(C2S::CMD::update_combine_skill*)buf;
			if(size != sizeof(C2S::CMD::update_combine_skill) + sizeof(int)*(size_t)ucs.element_count) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerUpdateCombineSkill(ucs.skill_id, ucs.element_count, ucs.element_id);
		}
		break;
		
		case C2S::UNIQUE_BID_REQUEST:
		{
			C2S::CMD::unique_bid_request & ubr = *(C2S::CMD::unique_bid_request*)buf;
			if(size != sizeof(C2S::CMD::unique_bid_request)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUniqueBidRequest(ubr.bid_money_upperbound, ubr.bid_money_lowerbound))
			{
				error_cmd(S2C::ERR_CAN_NOT_BID);
			}
		}
		break;
		
		case C2S::UNIQUE_BID_GET:
		{
			if(size != sizeof(C2S::CMD::unique_bid_get)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUniqueBidGet())
			{
			}
		}
		break;

		case C2S::GET_CLONE_EQUIPMENT:
		{
			C2S::CMD::get_clone_equipment & gme = *(C2S::CMD::get_clone_equipment*)buf;
			if(size != sizeof(C2S::CMD::get_clone_equipment))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			if(gme.master_id <= 0) break;

			XID master;
			MAKE_ID(master,gme.master_id);

			if(master.IsPlayer())
			{
				msg_get_clone_equipment mce;
				mce.cs_sid = pPlayer->cs_sid;
				mce.cs_index = pPlayer->cs_index;
				
				MSG msg;
				BuildMessage(msg,GM_MSG_QUERY_CLONE_EQUIPMENT, master, pPlayer->ID, pPlayer->pos,gme.clone_id,&mce, sizeof(mce));
				gmatrix::SendMessage(msg);
			}
		}
		break;
		
		case C2S::TASK_FLY_POS:
		if(CheckDeny(controller::CMD_FLY_POS)) return 0;
		{
			C2S::CMD::task_fly_pos & tfp = *(C2S::CMD::task_fly_pos*)buf;
			if(size != sizeof(C2S::CMD::task_fly_pos)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerFlyTaskPos(tfp.task_id, tfp.npc_id))
			{
				pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
			}
		}
		break;
		
		case C2S::ZONEMALL_SHOPPING:
		{
			C2S::CMD::zonemall_shopping & zms = *(C2S::CMD::zonemall_shopping *)buf;
			if(size != sizeof(zms) || zms.count == 0)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->PlayerDoZoneShopping(zms.goods_id, zms.goods_index, zms.goods_slot, zms.count))
			{
			}
		}
		break;
		
		case C2S::ACTIVITY_FLY_POS:
		{
			C2S::CMD::activity_fly_pos & afp = *(C2S::CMD::activity_fly_pos*)buf;
			if(size != sizeof(C2S::CMD::activity_fly_pos)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerFlyActivityPos(afp.id))
			{
				pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
			}
		}
		break;

		case C2S::DEITY_LEVELUP:
		{
			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeityLevelUp();
		}
		break;

		case C2S::CANCEL_PULLING:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_filters.RemoveFilter(FILTER_PULLING);	
		}
		break;

		case C2S::GET_MALL_SALETIME_ITEM:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetMallSaleTimeItem();
		}
		break;

		case C2S::GET_VIP_AWARD_INFO:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->SendPlayerVipAwardInfo();
		}
		break;

		case C2S::GET_VIP_AWARD_BY_ID:
		{
			C2S::CMD::obtain_vip_award & data = *(C2S::CMD::obtain_vip_award *)buf;
			if(size != sizeof(C2S::CMD::obtain_vip_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int curtime = g_timer.get_systime();
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerObtainVipAward(curtime, data.award_id, data.award_item_id);
		}
		break;

		case C2S::GET_REPURCHASE_INV_DATA:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->SendRepurchaseInvData();
		}
		break;

		case C2S::TRY_GET_ONLINE_AWARD: 
		{
			C2S::CMD::obtain_online_award & data = *(C2S::CMD::obtain_online_award *)buf;
			if(size != sizeof(C2S::CMD::obtain_online_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int curtime		= g_timer.get_systime();
			int permitcode	= 0;

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerTryGetOnlineAward(data.award_id, curtime, data.award_index, data.small_giftbag_id, permitcode);
			pImp->_runner->online_award_permit(data.award_id, data.award_index, permitcode);
		}
		break;

		case C2S::GET_ONLINE_AWARD:
		{
			C2S::CMD::obtain_online_award & data = *(C2S::CMD::obtain_online_award *)buf;
			if(size != sizeof(C2S::CMD::obtain_online_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int curtime		= g_timer.get_systime();

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerObtainOnlineAward(data.award_id, curtime, data.award_index, data.small_giftbag_id);
		}
		break;
		
		case C2S::FIVE_ANNI_REQUEST:
		{
			if(size != sizeof(C2S::CMD::five_anni_request))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::five_anni_request & data = *(C2S::CMD::five_anni_request*)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(data.request == 0)
			{
				pImp->ResetAnniData();
			}
			else if(data.request == 1)
			{
				pImp->FiveAnniCongratuation();
			}
			else if(data.request == 2)
			{
				pImp->FiveAnniDeliveryAward();
			}
		}
		break;


		case C2S::UPGRADE_TREASURE_REGION:
		{
			C2S::CMD::upgrade_treasure_region & utr = *(C2S::CMD::upgrade_treasure_region*)buf;
			if(size != sizeof(C2S::CMD::upgrade_treasure_region)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUpgradeTreasureRegion(utr.item_id, utr.item_index, utr.region_id, utr.region_index))
			{
			}
		}
		break;
		
		case C2S::UNLOCK_TREASURE_REGION:
		{
			C2S::CMD::unlock_treasure_region & utr = *(C2S::CMD::unlock_treasure_region*)buf;
			if(size != sizeof(C2S::CMD::unlock_treasure_region)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerUnlockTreasureRegion(utr.item_id, utr.item_index, utr.region_id, utr.region_index))
			{
			}
		}
		break;
		
		case C2S::DIG_TREASURE_REGION:
		{
			C2S::CMD::dig_treasure_region & dtr = *(C2S::CMD::dig_treasure_region*)buf;
			if(size != sizeof(C2S::CMD::dig_treasure_region)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerDigTreasureRegion(dtr.region_id, dtr.region_index))
			{
			}
		}
		break;

		case C2S::ADOPT_LITTLEPET:
		{
			if(size != sizeof(C2S::CMD::adopt_littlepet)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerAdoptLittlePet();
		}
		break;

		case C2S::FEED_LITTLEPET:
		{
			C2S::CMD::feed_littlepet & flp = *(C2S::CMD::feed_littlepet*)buf;
			if(size != sizeof(C2S::CMD::feed_littlepet)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerFeedLittlePet(flp.item_id, flp.item_count);
		}
		break;

		case C2S::GET_LITTLEPET_AWARD:
		{
			if(size != sizeof(C2S::CMD::get_littlepet_award)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetLittlePetAward();
		}
		break;

		case C2S::RUNE_IDENTIFY:
		{
			C2S::CMD::rune_identify & ri = *(C2S::CMD::rune_identify*)buf;
			if(size != sizeof(C2S::CMD::rune_identify)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerIdentifyRune(ri.rune_index, ri.assist_index);
		}
		break;

		case C2S::RUNE_COMBINE:
		{
			C2S::CMD::rune_combine & rc = *(C2S::CMD::rune_combine*)buf;
			if(size != sizeof(C2S::CMD::rune_combine)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerCombineRune(rc.rune_index, rc.assist_index);
		}
		break;

		// Youshuang add
		case C2S::TALISMAN_ENCHANT_CONFIRM:
		{
			C2S::CMD::talisman_enchant_confirm & rc = *(C2S::CMD::talisman_enchant_confirm*)buf;
			if(size != sizeof(C2S::CMD::talisman_enchant_confirm)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			talisman_item::ConfirmEnchant( pImp, rc.index1, rc.confirm );
		}
		break;
		
		case C2S::FASHION_COLORANT_COMBINE:
		{
			C2S::CMD::fashion_colorant_combine & rc = *(C2S::CMD::fashion_colorant_combine*)buf;
			if(size != sizeof(C2S::CMD::fashion_colorant_combine)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int idx[3] = { rc.colorant_index1, rc.colorant_index2, rc.colorant_index3 };
			pImp->CombineFashionColorant( idx, 3 );
		}
		break;
		
		case C2S::FASHION_ADD_COLOR:
		{
			C2S::CMD::fashion_add_color & rc = *(C2S::CMD::fashion_add_color*)buf;
			if(size != sizeof(C2S::CMD::fashion_add_color)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->AddFashionColor( rc.colorant_index, rc.where, rc.fashion_item_index );
		}
		break;

		case C2S::GET_ACHIEVEMENT_AWARD:
		{
			C2S::CMD::get_achieve_award & rc = *(C2S::CMD::get_achieve_award*)buf;
			if(size != sizeof(C2S::CMD::get_achieve_award)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetAchievementAward( rc.achieve_id, rc.award_id );
		}
		break;

		case C2S::GET_FACBASE_MALL_INFO:
		{
			if(size != sizeof(C2S::CMD::get_facbase_mall_info)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetFacbaseMallInfo();
		}
		break;

		case C2S::SHOP_FROM_FACBASE_MALL:
		{
			C2S::CMD::shop_from_facbase_mall & rc = *(C2S::CMD::shop_from_facbase_mall*)buf;
			if(size != sizeof(C2S::CMD::shop_from_facbase_mall)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DoShopFromFacbaseMall( rc.item_index, rc.item_id, rc.cnt );
		}
		break;

		case C2S::CONTRIBUTE_FACBASE_CASH:
		{
			C2S::CMD::contribute_facbase_cash & rc = *(C2S::CMD::contribute_facbase_cash*)buf;
			if(size != sizeof(C2S::CMD::contribute_facbase_cash)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ContributeFacbaseCash( rc.cnt );
		}
		break;

		case C2S::GET_FACBASE_CASH_ITEMS_INFO:
		{
			if(size != sizeof(C2S::CMD::get_facbase_cash_items_info)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetFacbaseCashItemsInfo();
		}
		break;

		case C2S::BUY_FACBASE_CASH_ITEM:
		{
			C2S::CMD::buy_facbase_cash_item & rc = *(C2S::CMD::buy_facbase_cash_item*)buf;
			if(size != sizeof(C2S::CMD::buy_facbase_cash_item)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->BuyFacbaseCashItem( rc.item_index );
		}
		break;
		
		case C2S::BID_ON_FACBASE:
		{
			C2S::CMD::bid_on_facbase & rc = *(C2S::CMD::bid_on_facbase*)buf;
			if(size != sizeof(C2S::CMD::bid_on_facbase) + sizeof(char)*(size_t)rc.name_len) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			gplayer_imp::FBASE_CMD_RET ret = pImp->DoBidFromFacbaseMall( rc.item_index, rc.item_id, rc.coupon, rc.name_len, rc.player_name);
			if (ret == gplayer_imp::FBASE_CMD_DELIVER)
				pImp->DeliverFBaseCMD(buf, size);
		}
		break;
		
		case C2S::POST_FAC_BASE_MSG:
		{
			C2S::CMD::post_fac_base_msg & psm = *(C2S::CMD::post_fac_base_msg *)buf;
			if(size > 4096 || size != sizeof(psm) + sizeof(char)*(size_t)psm.msg_len)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			gplayer_imp::FBASE_CMD_RET ret = pImp->CheckPostFacBaseMsg(psm.fid, psm.msg, psm.msg_len);
			if (ret == gplayer_imp::FBASE_CMD_DELIVER)
				pImp->DeliverFBaseCMD(buf, size);
		}
		break;

		case C2S::TASK_FLY_TO_AREA:
		{
			C2S::CMD::task_fly_to_area & rc = *(C2S::CMD::task_fly_to_area*)buf;
			if(size != sizeof(C2S::CMD::task_fly_to_area)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerFlyTaskPos(rc.task_id, 0, false);
		}
		break;

		// case C2S::NEWYEAR_AWARD:
		// {
			// C2S::CMD::newyear_award & rc = *(C2S::CMD::newyear_award*)buf;
			// if(size != sizeof(C2S::CMD::newyear_award)) 
			// {
				// error_cmd(S2C::ERR_FATAL_ERR);
				// break;
			// }

			// gplayer_imp* pImp = (gplayer_imp*)_imp;
			// pImp->FetchNewYearAward(rc.award_type);
		// }
		// break;

		case C2S::USE_TITLE:
		{
			C2S::CMD::use_title & st = *(C2S::CMD::use_title *)buf;
			if(size != sizeof(st)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->UseTitle(st.title);
		}
		break;
		// end

		case C2S::RUNE_REFINE:
		{

			C2S::CMD::rune_refine & rr = *(C2S::CMD::rune_refine*)buf;
			if(size != sizeof(C2S::CMD::rune_refine)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerRefineRune(rr.rune_index, rr.assist_index);
		}
		break;
		
		case C2S::RUNE_RESET:
		{
			C2S::CMD::rune_reset & rr = *(C2S::CMD::rune_reset*)buf;
			if(size != sizeof(C2S::CMD::rune_reset)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerResetRune(rr.rune_index, rr.assist_index);
		}
		break;

		case C2S::RUNE_DECOMPOSE:
		{
			C2S::CMD::rune_decompose & rd = *(C2S::CMD::rune_decompose*)buf;
			if(size != sizeof(C2S::CMD::rune_decompose)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerDecomposeRune(rd.rune_index);
		}
		break;

		case C2S::RUNE_LEVELUP:
		{
			C2S::CMD::rune_levelup & rl = *(C2S::CMD::rune_levelup*)buf;
			if(size != sizeof(C2S::CMD::rune_levelup)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerLevelupRune(rl.rune_index))
			{
			}
		}
		break;
		
		case C2S::RUNE_OPEN_SLOT:
		{
			/*
			C2S::CMD::rune_open_slot & ro = *(C2S::CMD::rune_open_slot*)buf;
			if(size != sizeof(C2S::CMD::rune_open_slot)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerOpenRuneSlot(ro.rune_index);
			*/
		}
		break;

		case C2S::RUNE_CHANGE_SLOT:
		{
			C2S::CMD::rune_change_slot & rcs = *(C2S::CMD::rune_change_slot*)buf;
			if(size != sizeof(C2S::CMD::rune_change_slot)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerChangeRuneSlot(rcs.rune_index, rcs.assist_index);
		}
		break;

		case C2S::RUNE_ERASE_SLOT:
		{
			C2S::CMD::rune_erase_slot & res = *(C2S::CMD::rune_erase_slot*)buf;
			if(size != sizeof(C2S::CMD::rune_erase_slot)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerEraseRuneSlot(res.rune_index, res.stone_index);
		}
		break;

		case C2S::RUNE_INSTALL_SLOT:
		{
			C2S::CMD::rune_install_slot & ris = *(C2S::CMD::rune_install_slot*)buf;
			if(size != sizeof(C2S::CMD::rune_install_slot)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerInstallRuneSlot(ris.rune_index, ris.slot_index, ris.stone_index, ris.stone_id);
		}
		break;

		case C2S::RUNE_REFINE_ACTION:
		{
			C2S::CMD::rune_refine_action & rra = *(C2S::CMD::rune_refine_action*)buf;
			if(size != sizeof(C2S::CMD::rune_refine_action)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(!pImp->PlayerRefineRuneAction(rra.rune_index, rra.accept_result)) 
			{
			}
		}
		break;

		case C2S::START_RANDOM_TOWER_MONSTER:
		{
			if(size != sizeof(C2S::CMD::start_random_tower_monster)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::start_random_tower_monster& srtm = *(C2S::CMD::start_random_tower_monster*)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerStartRandomTowerMonster(srtm.client_idx);
		}
		break;

		case C2S::TOWER_REWARD:
		{
			C2S::CMD::tower_reward & tr = *(C2S::CMD::tower_reward*)buf;
			if(size != sizeof(C2S::CMD::tower_reward)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeliveryTowerReward(tr.raid_template_id, tr.level, tr.type);
		}
		break;

		case C2S::RESET_SKILL_PROP:
		{
			C2S::CMD::reset_skill_prop & data = *(C2S::CMD::reset_skill_prop *)buf;
			if(size != sizeof(C2S::CMD::reset_skill_prop))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerResetSkillProp(data.opcode);
		}
		break;

		case C2S::GET_TASK_AWARD:
		{
			C2S::CMD::get_task_award & gta = *(C2S::CMD::get_task_award*)buf;
			if(size != sizeof(C2S::CMD::get_task_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetTaskAward(gta.type);
		}
		break;

		case C2S::PK_1ST_GUESS:
		{
			if(size < sizeof(C2S::CMD::pk_1st_guess))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::pk_1st_guess & data = *(C2S::CMD::pk_1st_guess *)buf;
			int cnt = data.cnt;
			if(size != sizeof(C2S::CMD::pk_1st_guess) + cnt * 2 * sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerPk1stBet(data.area, data.cnt, data.items);	
		}
		break;

		case C2S::PK_TOP3_GUESS:
		{
			if(size < sizeof(C2S::CMD::pk_top3_guess))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::pk_top3_guess & data = *(C2S::CMD::pk_top3_guess *)buf;
			int cnt = data.cnt;
			if(size != sizeof(C2S::CMD::pk_top3_guess) + cnt * 2 * sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerPkTop3Bet(data.area, data.cnt, data.items, data.cancel);	
		}
		break;

		case C2S::PK_1ST_GUESS_REWARD:
		{
			if(size != sizeof(C2S::CMD::pk_1st_guess_reward))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeliveryPk1stBetReward();
		}
		break;

		case C2S::PK_1ST_GUESS_RESULT_REWARD:
		{
			if(size != sizeof(C2S::CMD::pk_1st_guess_result_reward))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeliveryPk1stBetResultReward();
		};
		break;

		case C2S::PK_TOP3_GUESS_REWARD:
		{
			if(size != sizeof(C2S::CMD::pk_top3_guess_reward))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeliveryPkTop3BetReward();
		}
		break;

		case C2S::PK_TOP3_GUESS_RESULT_REWARD:
		{
			if(size != sizeof(C2S::CMD::pk_top3_guess_result_reward))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::pk_top3_guess_result_reward* data = (C2S::CMD::pk_top3_guess_result_reward*)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeliveryPkTop3BetResultReward(data->type);
		}
		break;

		case C2S::START_SPECIAL_MOVE:
		{
			C2S::CMD::start_special_move & ssm = *(C2S::CMD::start_special_move*)buf;
			if(size != sizeof(C2S::CMD::start_special_move))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerStartSpecialMove(ssm.velocity, ssm.acceleration, ssm.cur_pos, ssm.collision_state, ssm.timestamp);
		}
		break;

		case C2S::SPECIAL_MOVE:
		{
			C2S::CMD::special_move & sm = *(C2S::CMD::special_move*)buf;
			if(size != sizeof(C2S::CMD::special_move))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerSpecialMove(sm.velocity, sm.acceleration, sm.cur_pos, sm.stamp, sm.collision_state, sm.timestamp);
		}
		break;

		case C2S::STOP_SPECIAL_MOVE:
		{
			C2S::CMD::stop_special_move & sm = *(C2S::CMD::stop_special_move*)buf;
			if(size != sizeof(C2S::CMD::stop_special_move))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerStopSpecialMove(sm.dir, sm.cur_pos, sm.stamp);
		}
		break;

		case C2S::GET_PLAYER_BET_DATA:
		{
			if(size != sizeof(C2S::CMD::get_player_bet_data))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_runner->get_player_pk_bet_data();
		}
		break;

		case C2S::COLLISION_RAID_APPLY:
		{
			if(size != sizeof(C2S::CMD::collision_raid_apply))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::collision_raid_apply & cra = *(C2S::CMD::collision_raid_apply*)buf;
			pImp->PlayerApplyCollisionRaid(cra.map_id, cra.raid_template_id, cra.is_team, cra.is_cross);
		}
		break;

		case C2S::HIDE_VIP_LEVEL:
		{
			if(size != sizeof(C2S::CMD::hide_vip_level))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::hide_vip_level & data = *(C2S::CMD::hide_vip_level *)buf;
			pImp->SetVipLevelHide(data.is_hide);
		}
		break;

		case C2S::CHANGE_WING_COLOR:
		{
			C2S::CMD::change_wing_color & cwc = *(C2S::CMD::change_wing_color*)buf;
			if(size != sizeof(C2S::CMD::change_wing_color))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerChangeWingColor(cwc.wing_color);
		}
		break;

		case C2S::ASTROLOGY_IDENTIFY:
		{
			C2S::CMD::astrology_identify & ai = *(C2S::CMD::astrology_identify*)buf;
			if(size != sizeof(C2S::CMD::astrology_identify))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerIdentifyAstrology(ai.item_index, ai.item_id);
		}
		break;

		case C2S::ASTROLOGY_UPGRADE:
		{
			C2S::CMD::astrology_upgrade & au = *(C2S::CMD::astrology_upgrade*)buf;
			if(size != sizeof(C2S::CMD::astrology_upgrade))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerUpgradeAstrology(au.item_index, au.item_id, au.stone_index, au.stone_id);
		}
		break;
		
		case C2S::ASTROLOGY_DESTROY:
		{
			C2S::CMD::astrology_destroy & ad = *(C2S::CMD::astrology_destroy*)buf;
			if(size != sizeof(C2S::CMD::astrology_destroy))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerDestroyAstrology(ad.item_index, ad.item_id);
		}
		break;

		case C2S::JOIN_RAID_ROOM:
		{
			if(size != sizeof(C2S::CMD::join_raid_room))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::join_raid_room & data = *(C2S::CMD::join_raid_room *)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerJoinRaidRoom(data.map_id, data.raid_template_id, data.room_id, data.raid_faction);
		}
		break;

		case C2S::TALISMAN_REFINESKILL_RESULT:
		{
			if(size != sizeof(C2S::CMD::talisman_refineskill_result))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::talisman_refineskill_result & data = *(C2S::CMD::talisman_refineskill_result *)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->TalismanRefineSkillResult(data.talisman1_index, data.talisman1_id, data.result);

		}
		break;

		case C2S::GET_COLLISION_AWARD:
		{
			if(size != sizeof(C2S::CMD::get_collision_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::get_collision_award & data = *(C2S::CMD::get_collision_award*)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetCollisionAward(data.daily_award, data.award_index);
		}
		break;

		case C2S::CANCEL_ICE_CRUST:
		{
			if(size != sizeof(C2S::CMD::cancel_ice_crust))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp= (gplayer_imp *) _imp;
			bool bRst = pImp->_filters.IsFilterExist(FILTER_ICESHIELD);
			if(bRst)
			{
				pImp->_filters.RemoveFilter(FILTER_ICESHIELD);
			}
		}
		break;

		case C2S::PLAYER_FIRST_EXIT_REASON:
		{
			if(sizeof(C2S::CMD::player_first_exit_reason) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::player_first_exit_reason & data = *(C2S::CMD::player_first_exit_reason *)buf;
			gplayer_imp * pImp= (gplayer_imp *) _imp;
			pImp->PlayerFirstExitReason(data.reason);
		}
		break;

		case C2S::REMEDY_METEMPSYCHOSIS_LEVEL:
		{
			if(size != sizeof(C2S::CMD::remedy_metempsychosis_level))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::remedy_metempsychosis_level & rml = *(C2S::CMD::remedy_metempsychosis_level*)buf;
			gplayer_imp * pImp= (gplayer_imp *) _imp;
			pImp->PlayerRemedyMetempsychosisLevel(rml.item_index);
		}
		break;

		case C2S::MERGE_POTION:
		{
			if(size != sizeof(C2S::CMD::merge_potion))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::merge_potion& mp = *(C2S::CMD::merge_potion* )buf;
			gplayer_imp * pImp= (gplayer_imp *) _imp;
			pImp->FillPotionBottle(mp.bottle_id, mp.bottle_index, mp.potion_id, mp.potion_index);
		};
		break;
		
		case C2S::KINGDOM_LEAVE:
		{
			if(size != sizeof(C2S::CMD::kingdom_leave)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerLeaveKingdom();
		}
		break;

		case C2S::GET_COLLISION_PLAYER_POS:
		{
			if(size != sizeof(C2S::CMD::get_collision_player_pos)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::get_collision_player_pos & gcpp = *(C2S::CMD::get_collision_player_pos* )buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetCollisionPlayerPos(gcpp.roleid);
		}
		break;

		case C2S::TAKE_LIVENESS_AWARD:
		{
			if(size != sizeof(C2S::CMD::take_liveness_award)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::take_liveness_award & pCmd = *(C2S::CMD::take_liveness_award* )buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->TakeLivenessAward(pCmd.grade);
		}
		break;

		case C2S::RAID_LEVEL_AWARD:
		{
			if(size != sizeof(C2S::CMD::raid_level_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::raid_level_award& cmd = *(C2S::CMD::raid_level_award*)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetRaidLevelReward(cmd.level);
			
		}
		break;

		case C2S::GET_RAID_LEVEL_RESULT:
		{
			if(size != sizeof(C2S::CMD::get_raid_level_result))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetRaidLevelResult();
		}
		break;

		case C2S::OPEN_TRASHBOX:
		{
			C2S::CMD::open_trashbox & ot = *(C2S::CMD::open_trashbox*)buf;
			if(size != sizeof(C2S::CMD::open_trashbox) + ot.passwd_size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerOpenTrashBox(ot.passwd_size, ot.passwd);
		}
		break;

		case C2S::DELIVERY_GIFT_BAG:
		{
			C2S::CMD::delivery_gift_bag& ot = *(C2S::CMD::delivery_gift_bag*)buf;
			if(size != sizeof(C2S::CMD::delivery_gift_bag))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DeliveryGiftBox(ot.index, ot.item_id);
		}
		break;

		case C2S::GET_CASH_GIFT_AWARD:
		{
			if(size != sizeof(C2S::CMD::get_cash_gift_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetCashGiftAward();
		}
		break;
		
		case C2S::GEN_PROP_ADD_ITEM:
		{
			if(size != sizeof(C2S::CMD::gen_prop_add_item))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::gen_prop_add_item& gp = *(C2S::CMD::gen_prop_add_item*)buf;
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GenPropAddItem(gp.material_id, gp.material_idx, gp.material_cnt);
		}
		break;

		case C2S::REBUILD_PROP_ADD_ITEM:
		{
			if(size != sizeof(C2S::CMD::rebuild_prop_add_item))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			C2S::CMD::rebuild_prop_add_item& gp = *(C2S::CMD::rebuild_prop_add_item*)buf;
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->RebuildPropAddItem(gp.itemId, gp.itemIdx, gp.forgeId, gp.forgeIdx);
		}
		break;

		case C2S::GET_PROPADD:
		{
			if(size != sizeof(C2S::CMD::get_propadd))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_runner->player_propadd();
		}
		break;

		case C2S::BUY_KINGDOM_ITEM:
		{
			if(size != sizeof(C2S::CMD::buy_kingdom_item))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::buy_kingdom_item & bki = *(C2S::CMD::buy_kingdom_item* )buf;
			pImp->PlayerBuyKingdomItem(bki.type, bki.index);
		}
		break;

		case C2S::KING_TRY_CALL_GUARD:
		{
			if(size != sizeof(C2S::CMD::king_try_call_guard))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->KingTryCallGuard();
		}
		break;

		case C2S::GET_TOUCH_AWARD:
		{
			if(size != sizeof(C2S::CMD::get_touch_award))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::get_touch_award & gta = *(C2S::CMD::get_touch_award* )buf;
			pImp->PlayerGetTouchAward(gta.type, gta.count);
		}
		break;

		case C2S::FLOW_BATTLE_LEAVE:
		{
			if(size != sizeof(C2S::CMD::flow_battle_leave))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerFlowBattleLeave();
		}
		break;

		case C2S::BATH_INVITE:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bath_invite & bi = *(C2S::CMD::bath_invite*)buf;
			if(size != sizeof(bi))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,bi.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerBathInvite(target);
			}
		}
		break;

		case C2S::BATH_INVITE_REPLY:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::bath_invite_reply & bir= *(C2S::CMD::bath_invite_reply*)buf;
			if(size != sizeof(bir))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,bir.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerBathInviteReply(target,bir.param);
			}
		}
		break;

		case C2S::DELIVER_KINGDOM_TASK:
		{
			if(size != sizeof(C2S::CMD::deliver_kingdom_task))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::deliver_kingdom_task & dkt = *(C2S::CMD::deliver_kingdom_task* )buf;
			pImp->PlayerDeliverKingdomTask(dkt.task_type);

		}
		break;

		case C2S::RECEIVE_KINGDOM_TASK:
		{
			if(size != sizeof(C2S::CMD::receive_kingdom_task))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::receive_kingdom_task & rkt = *(C2S::CMD::receive_kingdom_task* )buf;
			pImp->PlayerReceiveKingdomTask(rkt.task_type);
		}
		break;	

		case C2S::KINGDOM_FLY_BATH_POS:
		{
			if(size != sizeof(C2S::CMD::kingdom_fly_bath_pos))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerFlyBathPos();
		}
		break;


		case C2S::FUWEN_COMPOSE:
		{
			if(size != sizeof(C2S::CMD::fuwen_compose))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::fuwen_compose & fc = *(C2S::CMD::fuwen_compose* )buf;
			pImp->PlayerComposeFuwen(fc.consume_extra_item, fc.extra_item_index);
		}
		break;

		case C2S::FUWEN_UPGRADE:
		{
			if(size < sizeof(C2S::CMD::fuwen_upgrade))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::fuwen_upgrade & fu = *(C2S::CMD::fuwen_upgrade* )buf;
			int count = fu.assist_count;
			if(count <= 0 || count > 200) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			if(size != sizeof(C2S::CMD::fuwen_upgrade) + count * sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			pImp->PlayerUpgradeFuwen(fu.main_fuwen_index, fu.main_fuwen_where, fu.assist_count, fu.assist_fuwen_index);
		};
		break;

		case C2S::FUWEN_INSTALL:
		{
			if(size != sizeof(C2S::CMD::fuwen_install))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::fuwen_install & fi = *(C2S::CMD::fuwen_install* )buf;
			pImp->PlayerInstallFuwen(fi.src_index, fi.dst_index);
		};
		break;

		case C2S::FUWEN_UNINSTALL:
		{
			if(size != sizeof(C2S::CMD::fuwen_uninstall))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}	
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::fuwen_uninstall & fu = *(C2S::CMD::fuwen_uninstall* )buf;
				
			pImp->PlayerUninstallFuwen(fu.fuwen_index, fu.assist_index, fu.inv_index);
		};
		break;

		case C2S::RESIZE_INV:
		{
			if(size < sizeof(C2S::CMD::resize_inv))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			C2S::CMD::resize_inv& data = *(C2S::CMD::resize_inv*)buf;
			if(size != sizeof(C2S::CMD::resize_inv) + data.cnt * sizeof(int) * 3)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ResizeInv(data.new_size, data.type, data.cnt, data.items);
		}
		break;

		case C2S::REGISTER_FLOW_BATTLE:
		{
			if(size != sizeof(C2S::CMD::register_flow_battle))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->RegisterFlowBattle();
		}
		break;

		case C2S::ARRANGE_INVENTORY:
		{
			C2S::CMD::arrange_inventory& at = *(C2S::CMD::arrange_inventory*)buf;
			if (size != sizeof(at))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			if (!pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_ARRANGE_INVENTORY))
			{
				break;
			}
			std::vector<exchange_item_info> vec_exchange_info;
			pImp->SetCoolDown(COOLDOWN_INDEX_ARRANGE_INVENTORY,ARRANGE_INVENTORY_COOLDOWN_TIME);
			pImp->GetInventory().ArrangeItem(at.pageind,vec_exchange_info);
			if (!vec_exchange_info.size())
			{
				error_cmd(S2C::ERR_INVENTORY_HAS_SORT);
				break;
			}
			pImp->PlayerGetItemExchangeList(gplayer_imp::IL_INVENTORY,vec_exchange_info);
		}
		break;

		case C2S::ARRANGE_POCKET:
		{
			C2S::CMD::arrange_pocket& at = *(C2S::CMD::arrange_pocket*)buf;
			if (size != sizeof(at))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			if (!pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_ARRANGE_POCKET))
			{
				break;
			}
			std::vector<exchange_item_info> vec_exchange_info;
			pImp->SetCoolDown(COOLDOWN_INDEX_ARRANGE_POCKET,ARRANGE_POCKET_COOLDOWN_TIME);
			pImp->GetPocketInventory().ArrangeItem(0,vec_exchange_info);
			if (!vec_exchange_info.size())
			{
				error_cmd(S2C::ERR_POCKET_HAS_SORT);
				break;
			}
			pImp->PlayerGetItemExchangeList(gplayer_imp::IL_POCKET,vec_exchange_info);
		}
		break;

		case C2S::ARRANGE_TRASHBOX:
		{
			C2S::CMD::arrange_trashbox& at = *(C2S::CMD::arrange_trashbox*)buf;
			if (size != sizeof(at))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			if (!pImp->GetTrashBox().IsNormalBoxOpen())
			{
				error_cmd(S2C::ERR_TRASH_BOX_NOT_OPEN);
				break;
			}
			if (!pImp->_cooldown.TestCoolDown(COOLDOWN_INDEX_ARRANGE_TRASHBOX))
			{
				break;
			}
			std::vector<exchange_item_info> vec_exchange_info;
			pImp->SetCoolDown(COOLDOWN_INDEX_ARRANGE_TRASHBOX,ARRANGE_TRASHBOX_COOLDOWN_TIME);
			pImp->GetTrashBoxInventory().ArrangeItem(at.pageind,vec_exchange_info);
			if (!vec_exchange_info.size())
			{
				error_cmd(S2C::ERR_TRASHBOX_HAS_SORT);
				break;
			}
			pImp->PlayerGetItemExchangeList(gplayer_imp::IL_TRASH_BOX ,vec_exchange_info);
		}
		break;

		case C2S::GET_WEB_ORDER:
		{
			C2S::CMD::get_web_order & gwo = *(C2S::CMD::get_web_order*)buf;
			if (size != sizeof(gwo))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetWebOrder(gwo.order_id);
		}
		break;

		case C2S::CUSTOMIZE_RUNE:
		{
			C2S::CMD::customize_rune & cr = *(C2S::CMD::customize_rune*)buf;
			if (size != sizeof(cr) + cr.count*sizeof(int))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerCustomizeRune(cr.type, cr.count, cr.prop);
		}
		break;

		case C2S::GET_CROSS_SERVER_BATTLE_INFO:
		{
			C2S::CMD::get_cross_server_battle_info & gcsbi = *(C2S::CMD::get_cross_server_battle_info*)buf;
			const int tag = 158;
			if (gcsbi.map_id != tag- 100)
			{
				error_cmd(S2C::ERR_CROSS_SEVER_MAP_ID);
				break;
			}
			bg_world_manager* pManager = (bg_world_manager*)gmatrix::FindWorld(tag);
			if (size != sizeof(gcsbi)|| pManager == NULL || pManager->IsIdle())
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			unsigned char attacker_building_left;
			unsigned char defender_building_left;
			std::vector<S2C::CMD::battle_info::player_info_in_battle> info;
			std::vector<player_off_line_in_cs_battle> vec_info;
			int seq = 1;
			S2C::CMD::crossserver_battle_info info_cross;
			if (pManager->GetCommonBattleInfo(seq,attacker_building_left,defender_building_left,info,true))
			{
				if (info.size())	
				{
					pImp->_runner->battle_info( attacker_building_left, defender_building_left,
							(const char*)&info[0], sizeof(S2C::CMD::battle_info::player_info_in_battle) * info.size() );
				}
				else
				{
					error_cmd(S2C::ERR_BATTLE_READY);
					break;
				}
				if (pManager->GetCrossServerOfflineInfo(vec_info) && vec_info.size())
				{
					pImp->_runner->crossserver_off_line_score_info(vec_info);	
				}
				if (pManager->GetCrossServerBattleInfo( seq,info_cross,true))
				{
					info_cross.kill_count = 0;
					info_cross.death_count = 0;
					pImp->_runner->crossserver_battle_info(info_cross);
				}
				if (pManager->GetBattleResult())
				{
					pImp->_runner->battle_result(pManager->GetBattleResult(), pManager->GetAttackerScore(), pManager->GetDefenderScore());
				}
			}
		}
		break;

		case C2S::CONTROL_TRAP:
		{
			C2S::CMD::control_trap & ct = *(C2S::CMD::control_trap*)buf;
			if (size != sizeof(ct))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerControlTrap(ct.id, ct.tid);
		}
		break;

		case C2S::SUMMON_TELEPORT:
		{
			C2S::CMD::summon_teleport & st = *(C2S::CMD::summon_teleport*)buf;
			if (size != sizeof(st))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerSummonTeleport(st.npc_id);
		}
		break;

		case C2S::CREATE_CROSSVR_TEAM:
		{
			C2S::CMD::create_crossvr_team & cct = *(C2S::CMD::create_crossvr_team*)buf;
			if (size <= sizeof(cct) || size != sizeof(cct) + cct.name_len)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerCreateCrossVrTeam(cct.name_len, cct.team_name);
		}
		break;

		case C2S::GET_KING_REWARD:
		{
			C2S::CMD::get_king_reward & gkr = *(C2S::CMD::get_king_reward*)buf;
			if (size != sizeof(gkr))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetKingReward();
		}
		break;

		case C2S::ENTER_FACTION_BASE:
		{
			C2S::CMD::enter_faction_base & efb = *(C2S::CMD::enter_faction_base*)buf;
			if (size != sizeof(efb))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->CheckEnterBase();
		}
		break;

		case C2S::MOBACTIVE_START:
		{
			C2S::CMD::mobactive_start &ma = *(C2S::CMD::mobactive_start*)buf;
			if (size != sizeof(ma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			if (ma.task_id > 0)
			{
				PlayerTaskInterface task_if((gplayer_imp*)_imp);
				if (!task_if.HasTask(ma.task_id))
				{
					error_cmd(S2C::ERR_MOB_ACTIVE_HAS_INVALID_TASK);
					break;
				}
			}
			if (ma.tool_type > 0)
			{
				if (!pImp->CheckItemExist(ma.tool_type,1))
				{
					error_cmd(S2C::ERR_MOB_ACTIVE_HAS_INVALID_TOOL);
					break;
				}
			}

			pImp->PlayerMobActive(ma);
		}
		break;

		case C2S::MOBACTIVE_FINISH:
		{
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerMobActiveFinish();	
		}
		break;

		case C2S::QILIN_INVITE:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::qilin_invite & qi = *(C2S::CMD::qilin_invite*)buf;
			if(size != sizeof(qi))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			XID target;
			MAKE_ID(target,qi.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerQilinInvite(target);
			}
		}
		break;

		case C2S::QILIN_INVITE_REPLY:
		if(CheckDeny(CMD_BIND)) return 0;
		{
			C2S::CMD::qilin_invite_reply & qir = *(C2S::CMD::qilin_invite_reply*)buf;
			if(size != sizeof(qir))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,qir.who);
			if(target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerQilinInviteReply(target,qir.param);
			}
		}
		break;

		case C2S::QILIN_CANCEL:
		{
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			pImp->PlayerQilinCancel();
		}
		break;

		case C2S::QILIN_DISCONNECT:
		{
			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerQilinDisconnect();
		}
		break;

		case C2S::QILIN_RECONNECT:
		{
			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerQilinReconnect();
		}
		break;


		case C2S::ADD_FAC_BUILDING:
		{
			C2S::CMD::add_fac_building & afb = *(C2S::CMD::add_fac_building *)buf;
			if (size != sizeof(afb))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->AddFacBuilding(afb.fid, afb.field_index, afb.building_tid);
		}
		break;

		case C2S::UPGRADE_FAC_BUILDING:
		{
			C2S::CMD::upgrade_fac_building & ufb = *(C2S::CMD::upgrade_fac_building *)buf;
			if (size != sizeof(ufb))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->UpgradeFacBuilding(ufb.fid, ufb.field_index, ufb.building_tid, ufb.cur_level);
		}
		break;

		case C2S::REMOVE_FAC_BUILDING:
		{
			C2S::CMD::remove_fac_building & rfb = *(C2S::CMD::remove_fac_building *)buf;
			if (size != sizeof(rfb))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->RemoveFacBuilding(rfb.fid, rfb.field_index, rfb.building_tid);
		}
		break;

		case C2S::PET_CHANGE_SHAPE:
		{
			C2S::CMD::pet_change_shape & pcs = *(C2S::CMD::pet_change_shape*)buf;
			if (size != sizeof(pcs))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerChangePetShape(pcs.pet_index, pcs.shape_id);
		}
		break;

		case C2S::PET_REFINE_ATTR:
		{
			C2S::CMD::pet_refine_attr & pra = *(C2S::CMD::pet_refine_attr*)buf;
			if (size != sizeof(pra))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerRefinePetAttr(pra.pet_index, pra.attr_type, pra.assist_id, pra.assist_count);
		}
		break;

		case C2S::ACTIVE_EMOTE_ACTION_INVITE:
		if (CheckDeny(CMD_ACTIVE_EMOTE)) return 0;
		{
			C2S::CMD::active_emote_action_invite & aeai = *(C2S::CMD::active_emote_action_invite *)buf;
			if (size != sizeof(aeai))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,aeai.who);
			if (target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				if (pImp->CheckCoolDown(COOLDOWN_INDEX_EMOTE))
				{
					pImp->SetCoolDown(COOLDOWN_INDEX_EMOTE,5000);
					pImp->PlayerActiveEmoteInvite(target,aeai.type);
				}
			}
		}
		break;

		case C2S::ACTIVE_EMOTE_ACTION_INVITE_REPLY:
		if (CheckDeny(CMD_ACTIVE_EMOTE)) return 0;
		{
			C2S::CMD::active_emote_action_invite_reply & aeair = *(C2S::CMD::active_emote_action_invite_reply *)buf;
			if (size != sizeof(aeair))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			XID target;
			MAKE_ID(target,aeair.who);
			if (target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerActiveEmoteInviteReply(target, aeair.type, aeair.param);
			}
		}
		break;

		case C2S::ACTIVE_EMOTE_ACTION_CANCEL:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerActiveEmoteCancel();
		}
		break;
		
		case C2S::GET_RAID_TRANSFORM_TASK:
		{
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->GetRaidTransformTask();
		}
		break;

		case C2S::GET_FAC_BASE_INFO:
		{
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetFacBase();
		}
		break;

		case C2S::FAC_BASE_TRANSFER:
		{
			C2S::CMD::fac_base_transfer & fbt = *(C2S::CMD::fac_base_transfer *)buf;
			if (size != sizeof(fbt))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->FacBaseTransfer(fbt.region_index);
		}
		break;

		case C2S::GET_CS_6V6_AWARD:
		{
			C2S::CMD::get_cs_6v6_award & gca = *(C2S::CMD::get_cs_6v6_award*)buf;
			if (size != sizeof(gca))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerTryGetCS6V6Award(gca.award_type, gca.award_level);
		}
		break;

		case C2S::EXCHANGE_CS_6V6_AWARD:
		{
			C2S::CMD::exchange_cs_6v6_award & eca = *(C2S::CMD::exchange_cs_6v6_award*)buf;
			if (size != sizeof(eca))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeCS6V6Award(eca.index, eca.item_id, eca.currency_id);
		}
		break;

		case C2S::PUPPET_FORM_CHANGE:
		{
			C2S::CMD::puppet_form_change & pfc = *(C2S::CMD::puppet_form_change*)buf;
			if (size != sizeof(pfc))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerChangePuppetForm();
		}
		break;

		case C2S::EXCHANGE_CS_6V6_MONEY:
		{
			C2S::CMD::exchange_cs_6v6_money & ecm = *(C2S::CMD::exchange_cs_6v6_money*)buf;
			if (size != sizeof(ecm))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerExchangeCS6V6Money(ecm.type, ecm.count);
		}
		break;

		case C2S::WITHDRAW_FACBASE_AUCTION:
		{
			C2S::CMD::withdraw_facbase_auction & withdraw = *(C2S::CMD::withdraw_facbase_auction *)buf;
			if(size != sizeof(C2S::CMD::withdraw_facbase_auction)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->WithdrawFacBaseAuction(withdraw.item_index);
		}
		break;

		case C2S::WITHDRAW_FACBASE_COUPON:
		{
			C2S::CMD::withdraw_facbase_coupon & withdraw = *(C2S::CMD::withdraw_facbase_coupon *)buf;
			if(size != sizeof(withdraw)) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->WithdrawFacBaseCoupon();
		}
		break;

		case C2S::LEAVE_FACTION_BASE:
		{
			C2S::CMD::leave_faction_base & leave = *(C2S::CMD::leave_faction_base*)buf;
			if (size != sizeof(leave))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->LeaveFacBase();
		}
		break;

		case C2S::OBJECT_CHARGE_TO:
		{
			C2S::CMD::object_charge_to & oct = *(C2S::CMD::object_charge_to*)buf;
			if (size != sizeof(oct))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerChargeTo(oct.type, oct.target_id, oct.pos);
		}
		break;

		case C2S::STOP_TRAVEL_AROUND:
		{
			C2S::CMD::stop_travel_around & sta = *(C2S::CMD::stop_travel_around*)buf;
			if (size != sizeof(sta))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStopTravelAround();
		}
		break;

		case C2S::GET_SUMMON_PETPROP:
		{
			C2S::CMD::get_summon_petprop & gsp = *(C2S::CMD::get_summon_petprop*)buf;
			if (size != sizeof(gsp))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetSummonPetProp();
		}
		break;

		case C2S::GET_SEEK_AWARD:
		{
			C2S::CMD::get_seek_award & gca = *(C2S::CMD::get_seek_award*)buf;
			if (size != sizeof(gca))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetHideAndSeekAward();

		}
		break;

		case C2S::SEEKER_BUY_SKILL:
		{
			C2S::CMD::seeker_buy_skill & sbs = *(C2S::CMD::seeker_buy_skill*)buf;
			if (size != sizeof(sbs))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			bool flag = pImp->PlayerBuySeekerSkill();
			if (!flag)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
			}
		}
		break;

		case C2S::HIDE_AND_SEEK_RAID_APPLY:
		{
			if(size != sizeof(C2S::CMD::hide_and_seek_raid_apply))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::hide_and_seek_raid_apply & hasra = *(C2S::CMD::hide_and_seek_raid_apply*)buf;
			pImp->PlayerApplyHideAndSeekRaid(hasra.map_id, hasra.raid_template_id, hasra.is_team);
		}
		break;

		case C2S::CAPTURE_RAID_SUBMIT_FLAG:
		{
			if (size != sizeof(C2S::CMD::capture_raid_submit_flag))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerSubmitCaptureRaidFlag();
		}
		break;

		case C2S::CAPTURE_RAID_APPLY:
		{
			if (size != sizeof(C2S::CMD::capture_raid_apply))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			C2S::CMD::capture_raid_apply & cra = *(C2S::CMD::capture_raid_apply*)buf;
			pImp->PlayerApplyCaptureRaid(cra.map_id, cra.raid_template_id, cra.is_team);
		}
		break;

		default:
		{
			ASSERT(false);
		}
		break;
	}
	return 0;
}


// ---------------------分割线-----------------------------------------------

int 
gplayer_controller::GMCommandHandler(int cmd_type,const void * buf, size_t size)
{
#define DEFCMD(type) C2S::CMD::type & cmd = *(C2S::CMD::type*)buf; \
			if (size != sizeof(cmd))\
			{\
				error_cmd(S2C::ERR_FATAL_ERR);\
				break;\
			}
#define TESTGMPRIVILEGE(X) if(!_gm_auth->X()) \
			{\
				error_cmd(S2C::ERR_INVALID_PRIVILEGE); \
				break;\
			}
	if(cmd_type > C2S::GM_COMMAND_END && _debug_command_enable)
	{
		//开启了调试开关才能进行调试命令
		return DebugCommandHandler(cmd_type,buf,size);
	}

	if(!_gm_auth) 
	{
		_load_stats += 3;
		return 0;
	}


	switch(cmd_type)
	{
		case C2S::GMCMD_MOVE_TO_PLAYER:
		{
			TESTGMPRIVILEGE(Has_MoveTo_Role)

			DEFCMD(gmcmd_move_to_player)
			XID target(GM_TYPE_PLAYER,cmd.id);

			//现在直接发送查询消息
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//记录日志
			GLog::log(GLOG_INFO,"GM:%d试图移动到玩家%d处",pImp->_parent->ID.id,target.id);
			GLog::action("GM,gmid=%d:cmd=jump:rid=%d", pImp->_parent->ID.id,target.id);

			pImp->SendTo<0>(GM_MSG_GM_MQUERY_MOVE_POS,target,0);
		}
		break;

		case C2S::GMCMD_RECALL_PLAYER:
		{
			TESTGMPRIVILEGE(Has_Fetch_Role)

			DEFCMD(gmcmd_recall_player)
			XID target(GM_TYPE_PLAYER,cmd.id);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			//记录日志
			const A3DVECTOR &pos = pImp->_parent->pos;
			GLog::log(GLOG_INFO,"GM:%d将玩家%d移动过来(%f,%f,%f,)",pImp->_parent->ID.id,target.id,pos.x,pos.y,pos.z);
			//发送一个消息过去要求玩家跳转过来
			pImp->SendTo<0>(GM_MSG_GM_RECALL,target,pImp->GetWorldTag());
			
		}
		break;

		case C2S::GMCMD_OFFLINE:
		{
			//此命令已经被屏蔽，将在link上实现
			TESTGMPRIVILEGE(Has_Force_Offline)

			DEFCMD(gmcmd_offline)
			if(_cur_target.type  != GM_TYPE_PLAYER)
			{
				error_cmd(S2C::ERR_INVALID_TARGET);
				break;
			}
			//发送一个消息下线
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SendTo<0>(GM_MSG_GM_OFFLINE,_cur_target,0);
		}
		break;


		case C2S::GMCMD_TOGGLE_INVISIBLE:
		{
			TESTGMPRIVILEGE(Has_Hide_BeGod)

			bool bRst = _imp->_runner->is_invisible();
			if(!bRst)
			{
				//必须现在调用，后面调用就自动忽略次消息了
				_imp->_runner->leave_world();
			}
			_imp->_runner->set_invisible(!bRst);
			gplayer* pPlayer = (gplayer*)(_imp->_parent);
			pPlayer->invisible = !bRst;
			if(bRst)
			{
				//如果现在是非隐身，播放一下进入世界
				_imp->_runner->appear();
			}
			//通知客户端
			_imp->_runner->toggle_invisible(bRst?1:0);

			//记录日志
			GLog::log(GLOG_INFO,"GM:%d切换了隐身状态(%s)", _imp->_parent->ID.id,bRst?"隐身":"现形");
			GLog::action("GM,gmid=%d:cmd=disclose:stus=%s", _imp->_parent->ID.id,bRst?"on":"off");
		}
		break;

		case C2S::GMCMD_TOGGLE_INVINCIBLE:
		{
			TESTGMPRIVILEGE(Has_Hide_BeGod)

			gactive_imp *pImp = (gactive_imp *)_imp;
			bool bRst = pImp->_filters.IsFilterExist(FILTER_INVINCIBLE);
			if(bRst)
			{
				pImp->_filters.RemoveFilter(FILTER_INVINCIBLE);
			}
			else
			{
				pImp->_filters.AddFilter(new invincible_filter(pImp,FILTER_INVINCIBLE));
			}
			_imp->_runner->toggle_invincible(bRst?0:1);
			GLog::log(GLOG_INFO,"GM:%d切换了无敌状态(%s)", pImp->_parent->ID.id,bRst?"无敌":"正常");
			GLog::action("GM,gmid=%d:cmd=god:stus=%s", pImp->_parent->ID.id,bRst?"on":"off");
		}
		break;

		case C2S::GOTO:
		{
			TESTGMPRIVILEGE(Has_Move_AsWill)

			C2S::CMD::player_goto & pg = *(C2S::CMD::player_goto *)buf;
			if(sizeof(pg) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos(pg.pos);
			GLog::log(GLOG_INFO,"GM:%d进行了空间跳跃(%f,%f,%f)", pImp->_parent->ID.id,pos.x,pos.y,pos.z);
			pos.y = pImp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
			pImp->PlayerGoto(pos);
		}
		break;


		case C2S::GMCMD_DROP_GENERATOR:
		{
			C2S::CMD::gmcmd_drop_generator & gdg = *(C2S::CMD::gmcmd_drop_generator *)buf;
			if(sizeof(gdg) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			DATA_TYPE dt = gmatrix::GetDataMan().get_data_type(gdg.id,ID_SPACE_ESSENCE);
			if(dt != DT_GM_GENERATOR_ESSENCE)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			GLog::log(GLOG_INFO,"GM:%d丢出了怪物生成器%d", _imp->_parent->ID.id,gdg.id);
			element_data::item_tag_t tag = {element_data::IMT_NULL,0};
			item_data * data = gmatrix::GetDataMan().generate_item(gdg.id,&tag,sizeof(tag));
			if(data)
			{
				DropItemData(_imp->_plane,_imp->_parent->pos,data,_imp->_parent->ID,0,0);
			}
		}
		break;

		case C2S::GMCMD_ACTIVE_SPAWNER:
		{
			TESTGMPRIVILEGE(Has_ActivityManager)

			C2S::CMD::gmcmd_active_spawner & gas = *(C2S::CMD::gmcmd_active_spawner *)buf;
			if(sizeof(gas) != size)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(gas.is_active)
			{
				
				GLog::action("GM,gmid=%d:cmd=actmonster:mid=%d", _imp->_parent->ID.id,gas.sp_id);
				GLog::log(GLOG_INFO,"GM:%d激活了生成区域%d",_imp->_parent->ID.id,gas.sp_id);
				world_manager::ActiveSpawn(_imp->GetWorldManager(), gas.sp_id, true);
			}
			else
			{
				GLog::action("GM,gmid=%d:cmd=closemonster:mid=%d", _imp->_parent->ID.id,gas.sp_id);
				GLog::log(GLOG_INFO,"GM:%d取消了生成区域%d",_imp->_parent->ID.id,gas.sp_id);
				world_manager::ActiveSpawn(_imp->GetWorldManager(), gas.sp_id, false);
			}
		}
		break;

		case C2S::GMCMD_GENERATE_MOB:
		{
			TESTGMPRIVILEGE(Has_ActivityManager)
			C2S::CMD::gmcmd_generate_mob & ggm = *(C2S::CMD::gmcmd_generate_mob *)buf;
			if(size < sizeof(ggm) || size != sizeof(ggm) + ggm.name_len || ggm.name_len > 18)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			object_interface::minor_param prop;
			memset(&prop,0,sizeof(prop));
			
			prop.mob_id = ggm.mob_id;
			prop.remain_time = ggm.life;
			prop.exp_factor = 1.f;
			prop.drop_rate = 1.f;
			prop.money_scale = 1.f;
			prop.spec_leader_id = XID(0,0);
			prop.parent_is_leader = false;
			prop.use_parent_faction = false;
			prop.die_with_leader = false;
			prop.vis_id = ggm.vis_id;
			prop.mob_name_size = ggm.name_len;
			if(ggm.name_len)
			{
				memcpy(prop.mob_name,ggm.name,ggm.name_len);
			}
			for(int i =0; i < ggm.count; i ++)
			{
				oi.CreateMinors(prop);
			}
			GLog::log(GLOG_INFO,"GM:%d创建了%d个怪物%d(%d)",
					_imp->_parent->ID.id, ggm.count, ggm.vis_id,ggm.mob_id);
			GLog::action("GM,gmid=%d:cmd=cremonster:mid=%d:count=%d", _imp->_parent->ID.id,ggm.mob_id,ggm.count);
		}
		break;

		case C2S::GMCMD_MOVETOMAP:
		{
			TESTGMPRIVILEGE(Has_MoveTo_Map)
			DEFCMD(gmcmd_movetomap)

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos(cmd.x, cmd.y, cmd.z);
			GLog::log(GLOG_INFO,"GM:%d进行了地图(%d)跳跃(%f,%f,%f)", pImp->_parent->ID.id, cmd.world_tag, pos.x,pos.y,pos.z);
			pImp->LongJump(pos, cmd.world_tag);
		}
		break;
		
	default:
		__PRINTF("收到无法辨识的命令 %d\n",cmd_type);
		break;
	}
	return 0;
#undef DEFCMD
}

int 
gplayer_controller::DebugCommandHandler(int cmd_type,const void * buf, size_t size)
{
	if(!_debug_command_enable) return 0;
	GLog::log(LOG_INFO,"GM:用户%d执行了内部命令%d",_imp->_parent->ID.id,cmd_type);
	switch(cmd_type)
	{
		case C2S::DEBUG_DELIVERY_CMD:
		{
			C2S::CMD::debug_delivery_cmd & sp = *(C2S::CMD::debug_delivery_cmd *)buf;
			if(size < sizeof(int)) break;
			GMSV::SendDebugCommand(_imp->_parent->ID.id , sp.type, sp.buf, size - sizeof(int));
		}
		break;

		case 1988:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			size_t money = 1000000*pImp->_basic.level;
			pImp->GainMoneyWithDrop(money);
			pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);
		}
		break;

		case 1989:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;

			int money = *(int*)((char*)buf + 2);
			money *= 10000;
			if(money < 0 || money > 2000000000) break;
			pImp->GainMoneyWithDrop(money);
			pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);
		}
		break;

		case 1990:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int id = *(int*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_trashbox.SetTrashBoxSize(id);
		}
		break;

		case 1994:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int skill = *(int*)((char*)buf+2);
			//不检查选中对象的位置信息
			if(_cur_target.IsValid())
			{
				session_skill *pSkill= new session_skill(pImp);
				int t = _cur_target.id;
				pSkill->SetTarget(skill,1,1,&t);
				pImp->AddStartSession(pSkill);
			}
		}
		break;

		case 1995:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;

			int ratio = *(int*)((char*)buf+2);
			if(ratio > 100) ratio = 100;

			int level = pImp->_basic.level;
			size_t reborn_cnt = pImp->GetRebornCount();
			if(level >= player_template::GetMaxLevel(reborn_cnt)) 
			{
				level = player_template::GetMaxLevel(reborn_cnt);
				break;
			}
			
			int64_t exp = int64_t(player_template::GetLvlupExp(reborn_cnt,level) * ratio / 100);
			msg_exp_t mm={level,exp, true};
			pImp->SendTo<0>(GM_MSG_EXPERIENCE,pImp->_parent->ID,0,&mm,sizeof(mm));

		}
		break;

		case 1996:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int64_t exp = (*(int*)((char*)buf+2));
			pImp->IncExp(exp);
		}
		break;

		case 1997:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int level = pImp->_basic.level;
			size_t reborn_cnt = pImp->GetRebornCount();
			for(int i = 0; i < 150 && level < player_template::GetMaxLevel(reborn_cnt); ++i)
			{
				int64_t exp = player_template::GetLvlupExp(reborn_cnt,level);
				pImp->IncExp(exp);
				level = pImp->_basic.level;
			}
		}
		break;

		case 1998:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->GetRebornCount() != 0) break;
			if(pImp->_basic.level != 150) break;

			int new_prof = 7;
			if(size == 6)
			{
				new_prof = (*(int*)((char*)buf+2));
			}
			pImp->Metempsychosis(new_prof);

		}
		break;

		
		case 1999:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int level = pImp->_basic.level;
			size_t reborn_cnt = pImp->GetRebornCount();
			for(int i = 0; i < 10 && level < player_template::GetMaxLevel(reborn_cnt); ++i)
			{
				int64_t exp = player_template::GetLvlupExp(reborn_cnt,level);
				pImp->IncExp(exp);
				level = pImp->_basic.level;
			}
		}	
		break;

		case 2000:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int level = pImp->_basic.level;
			size_t reborn_cnt = pImp->GetRebornCount();
			if(level >= player_template::GetMaxLevel(reborn_cnt)) 
			{
				level = player_template::GetMaxLevel(reborn_cnt);
				break;
			}

			msg_exp_t mm={level,player_template::GetLvlupExp(reborn_cnt,level), true};
			pImp->SendTo<0>(GM_MSG_EXPERIENCE,pImp->_parent->ID,0,&mm,sizeof(mm));
		}
		break;

		case 2001:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int level = pImp->_basic.level;
			size_t reborn_cnt = pImp->GetRebornCount();
			if(reborn_cnt == 0)
			{
				for(int i = 0; i < 150 && level < player_template::GetMaxLevel(reborn_cnt); ++i)
				{
					int64_t exp = player_template::GetLvlupExp(reborn_cnt,level);
					pImp->IncExp(exp);
					level = pImp->_basic.level;
				}
				pImp->Metempsychosis(20);

				reborn_cnt = pImp->GetRebornCount();
				for(int i = 0; i < 150 && level < player_template::GetMaxLevel(reborn_cnt); ++i)
				{
					int64_t exp = player_template::GetLvlupExp(reborn_cnt,level);
					pImp->IncExp(exp);
					level = pImp->_basic.level;
				}
			}
			else
			{
				for(int i = 0; i < 150 && level < player_template::GetMaxLevel(reborn_cnt); ++i)
				{
					int64_t exp = player_template::GetLvlupExp(reborn_cnt,level);
					pImp->IncExp(exp);
					level = pImp->_basic.level;
				}
			}

			if(size == 6)
			{
				int cls = (*(int*)((char*)buf +2));
				pImp->ChangeClass(cls);

			}	
			else
			{
				int occupation[] = {14, 17, 20, 23, 29, 68, 37, 43, 49, 55, 100, 60, 106, 112};
				int index = abase::Rand(0, sizeof(occupation)/sizeof(int)-1);
				pImp->ChangeClass(occupation[index]);
			}
		}
		break;


		case 2002:
		{
			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int id = (*(int*)((char*)buf+2));
			size_t timeout = (*(size_t*)((char*)buf+6));
			if(timeout < 100) timeout = 120;
			if(timeout > 300) timeout = 300;
			if(id == 8510)
			{
				class try_restart : public abase::timer_task, public ONET::Thread::Runnable
				{
					int  _timeout;
					int  _id;
					world * _plane;
					int _counter;
				public:
					try_restart(int id, size_t timeout,world * pPlane):_timeout(timeout),_id(id),_plane(pPlane)
					{
						_counter = 20;
						SetTimer(g_timer,20,timeout);
					}

					void SendTimeout()
					{
						//发送数据
						char buf[512];
						short buf2[512];
						sprintf(buf,"Server shutdown:%dsec\n",_timeout);
						char * p = buf;
						short* w = buf2;
						do
						{
							*w++ = *p++;
						}while(*p);
						//broadcast_chat_msg(_plane,_id,buf2,(w - buf2)*sizeof(short),0);
						broadcast_chat_msg(_id,buf2,(w - buf2)*sizeof(short),0,0, 0,0);
					}

					
					virtual void Run()
					{
						_timeout --;
						if(_timeout > 150) 
						{
							_counter -= 1;
						}
						else if(_timeout > 80)
						{
							_counter -= 2;
						}
						else if(_timeout > 20)
						{
							_counter -= 4;
						}
						else 
						{
							_counter -= 20;
						}
						if(_counter <= 0)
						{
							_counter = 20;
							SendTimeout();
						}
					}

					void OnTimer(int index,int rtimes, bool & is_delete)      
					{
						//ONET::Thread::Pool::AddTask(this);
						Run();
						if(rtimes == 0)
						{
							gmatrix::RestartProcess();
						}
					}

				};
				(new try_restart(_imp->_parent->ID.id,timeout,_imp->_plane))->SendTimeout();
			}
		}
		break;


		case 2003:
		case 10808:
		{
			
			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"talisman.lua"))
				{
					printf("失败1\n");
					break;
				}
				if(luaL_dofile(L,"talisman_prepare.lua"))
				{
					printf("失败2\n");
					break;
				}
				__PRINTF("load tailsman.lua sucess!\n");
			}
			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"petbedge.lua"))
				{
					__PRINTF("load petbedge.lua failed!\n");
					break;
				}
				__PRINTF("load petbedge.lua sucess!\n");
			}
			
			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PLAYER);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"player.lua"))
				{
					__PRINTF("load player.lua failed!\n");
					break;
				}
				__PRINTF("load player.lua sucess!\n");
			}
			
			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_SUMMON);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"summon.lua"))
				{
					__PRINTF("load summon.lua failed!\n");
					break;
				}
				__PRINTF("load summon.lua sucess!\n");
			}

			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_MAGIC);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"magic.lua"))
				{
					__PRINTF("load magic.lua failed!\n");
					break;
				}
				__PRINTF("load magic.lua sucess!\n");
			}

			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_TRANSFORM);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"transform.lua"))
				{
					__PRINTF("load transform.lua failed!\n");
					break;
				}
				__PRINTF("load transform.lua sucess!\n");
			}
			
			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_RUNE);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"rune.lua"))
				{
					__PRINTF("load rune.lua failed!\n");
					break;
				}
				__PRINTF("load rune.lua sucess!\n");
			}
			
			{
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_ASTROLOGY);
				lua_State * L = keeper.GetState();
				if(L == NULL) break;
				if(luaL_dofile(L,"astrology.lua"))
				{
					__PRINTF("load astrology.lua failed!\n");
					break;
				}
				__PRINTF("load astrology.lua sucess!\n");
			}
		}
		break;

		case 2005:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			char buf[128];
			sprintf(buf, "pos.x = %f, pos.y = %f, pos.z = %f, dir = %d, tag=%d, raid_id=%d", 
				pImp->_parent->pos.x, pImp->_parent->pos.y, pImp->_parent->pos.z, pImp->_parent->dir, pImp->GetWorldManager()->GetWorldTag(), 
				 pImp->GetWorldManager()->GetRaidID());
			pImp->Say(buf);
		}
		break;
		
		case 2006:
		{
		}
		break;
			
		case 2007:
		{
			if(6 != size) break;
			int id = *(int*)((char*)buf+2);
			XID target(GM_TYPE_PLAYER,id);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			world::object_info info; 
			if(pImp->_plane->QueryObject(target,info,true))
			{       
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->LongJump(info.pos,info.tag);
			}       
		}                       
		break;

		
		case 2008:
		{
		#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int tag;
				int x;
				int z;
			};
		#pragma pack()
			mma & pg = *(mma*)buf;
			if(sizeof(pg) != size)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos(pg.x,0,pg.z);
			pImp->LongJump(pos,pg.tag);
		}
		break;

		case 2009:
		{
		#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int x;
				int z;
			};
		#pragma pack()
			mma & pg = *(mma*)buf;
			if(sizeof(pg) != size)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos(pg.x,0,pg.z);
			pImp->LongJump(pos);
		}


		case 2010:
		{
		#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int tag;
				int x;
				int y;
				int z;
			};
		#pragma pack()
			mma & pg = *(mma*)buf;
			if(sizeof(pg) != size)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos(pg.x,pg.y,pg.z);
			pImp->LongJump(pos,pg.tag);
		}
		break;

		case 2011:
		{
			struct mma
			{
				unsigned short cmd;
				size_t idx1;
				size_t idx2;
			};
			if(size != 10 )
			{
				break;
			}
			int cls = *(size_t*)((char*)buf + 6);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ChangeClass(cls);

		}
		break;

		case 2012:
		{
			if(size != 10 )
			{
				break;
			}
			int cls = *(size_t*)((char*)buf + 2);
			if(cls != 740404) break;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			cls = *(size_t*)((char*)buf + 6);
			pImp->StepMove(A3DVECTOR(0,(float)(cls)/100.f,0));
			pImp->_runner->trace_cur_pos(pImp->_commander->GetNextMoveSeq());
		}
		break;

		case 2013:
		{
			if(size != 10 )
			{
				break;
			}
			int cls = *(size_t*)((char*)buf + 2);
			if(cls != 740404) break;
			cls = *(size_t*)((char*)buf + 6);
			if(cls <= 0) cls = 5;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_cur_prop.run_speed = cls;
			pImp->_runner->get_extprop_move();

		}
		break;

		case 2222:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_basic.hp = pImp->GetMaxHP();
			pImp->_basic.mp = pImp->GetMaxMP();
			pImp->SetRefreshState();
		}
		break;

		case 2223:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_basic.hp = 1; 
			pImp->_basic.mp = 0; 
			pImp->SetRefreshState();
		}
		break;

		case 2224:
		{
			ASSERT(false);
		}
		break;

		case 2220:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->DebugJump();
		}
		break;

		case 10800:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			size_t count = 1;
			if(size != 6 && size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			if(size == 10)
			{
				count = *(size_t*)((char*)buf + 6);
				count = 10;
			}
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * data = gmatrix::GetDataMan().generate_item(*(int*)((char*)buf+2),&tag,sizeof(tag));
			if(data)
			{
				if(count > data->pile_limit) count = data->pile_limit;
				data->count = count;
				DropItemData(_imp->_plane,_imp->_parent->pos,data,_imp->_parent->ID,0,0);
			}
		}
		break;

		case 10801:              
		case 6050:
		{                       
			struct mma
			{               
				unsigned short cmd;
				size_t id;
			};      
			size_t count = 1;
			if(size != 6 && size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			if(size == 10)
			{
				count = *(size_t*)((char*)buf + 6);
				count = 10;
			}
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * data = gmatrix::GetDataMan().generate_item(*(int*)((char*)buf+2),&tag,sizeof(tag));
			if(data)
			{       
				count = data->pile_limit;
				data->count = count;
				DropItemData(_imp->_plane,_imp->_parent->pos,data,_imp->_parent->ID,0,0);
			}               
		}                       
		break;   


		case 10802:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
				size_t remain_time;
				size_t uuu;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma * ccc = (mma*)buf;
			int id = ccc->id;
			size_t count = ccc->count;
			object_interface oi((gplayer_imp *)_imp);
			object_interface::minor_param prop;
			memset(&prop,0,sizeof(prop));
			
			prop.remain_time = ccc->remain_time;
			prop.exp_factor = 1.f;
			prop.drop_rate = 1.f;
			prop.money_scale = 1.f;
			prop.spec_leader_id = _cur_target;
			prop.parent_is_leader = false;
			//prop.use_parent_faction = ccc->uuu;
			prop.use_parent_faction = false;
			//_cur_target == _imp->_parent->ID?true:false;
			prop.die_with_leader = true;
			for(size_t i =0; i < count; i ++)
			{
				prop.mob_id = id;
				oi.CreateMinors(prop);
			}

		}
		break;

		case 10803:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			object_interface::minor_param prop;
			memset(&prop,0,sizeof(prop));
			
			prop.remain_time = 0;
			prop.exp_factor = 0.f;
			prop.money_scale = 0.f;
			prop.spec_leader_id = _cur_target;
			prop.parent_is_leader = true;
			prop.use_parent_faction = true;
			prop.die_with_leader = true;
			for(size_t i =0; i< 10000;i ++)
			{
				prop.mob_id = i;
				oi.CreateMinors(prop);
			}
		
		}
		break;

		case 10804:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int id = *(int*)((char*)buf+2);
			object_interface oi((gplayer_imp*)_imp);
			object_interface::mine_param param = {id, 1000};
			oi.CreateMine(_imp->_parent->pos,param);
		}
		break;
		
		case 10805:
		case 11802:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
				size_t remain_time;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma * ccc = (mma*)buf;
			int id = ccc->id;
			size_t count = ccc->count;
			object_interface oi((gplayer_imp *)_imp);
			object_interface::minor_param prop;
			memset(&prop,0,sizeof(prop));
			
			prop.remain_time = ccc->remain_time;
			for(size_t i =0; i < count; i ++)
			{
				prop.mob_id = id;
				oi.CreateNPC(prop);
			}
		}
		break;

		case 10806:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
			};
			if(size != 10 )
			{
				break;
			}
			int id = *(int*)((char*)buf+2);
			int count = *(int*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp *) _imp;
			pImp->GainItem(id, count, 0, 0, 0);  
		}
		break;


	
		case 10807:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			if(size != 6 )
			{
				break;
			}
			int id = *(int*)((char*)buf+2);
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
			gplayer_imp * pImp = (gplayer_imp *) _imp;
			if(pItem)
			{
				int item_count = 1;
				int rst =pImp->_inventory.Push(*pItem,item_count,0);
				if(rst >= 0 && item_count == 0)
				{
					pImp->_inventory[rst].InitFromShop(pImp,ITEM_INIT_TYPE_TASK);
					int state = item::Proctype2State(pItem->proc_type);
					pImp->_runner->obtain_item(id,0,1,pImp->_inventory[rst].count, 0,rst,state);
					pImp->DeliverTopicSite( new TOPIC_SITE::got_item( pItem->type, 1 ) );  // Youshuang add
				}
			}
		}
		break;


		case 10809:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			if(size != 6 )
			{
				break;
			}
			int id = *(int*)((char*)buf+2);
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * pItem = gmatrix::GetDataMan().generate_item(id,&tag,sizeof(tag));
			if(pItem)
			{
				int t2 = g_timer.get_systime();
				pItem->expire_date = t2 + 60;
				gplayer_imp * pImp = (gplayer_imp *) _imp;
				int item_count = 1;
				int rst =pImp->_inventory.Push(*pItem,item_count,pItem->expire_date);
				if(rst >= 0 && item_count == 0)
				{
					pImp->_inventory[rst].InitFromShop(pImp,ITEM_INIT_TYPE_TASK);
					int state = item::Proctype2State(pItem->proc_type);
					pImp->_runner->obtain_item(id,0,1,pImp->_inventory[rst].count, 0,rst,state);
				}
			}
		}
		break;

		case 10810:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
				int level;

			};
			int id = *(int*)((char*)buf+2);
			int level = *(int*)((char*)buf+6);
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
			gplayer_imp * pImp = (gplayer_imp *) _imp;
			if(pItem)
			{
				int item_count = 1;
				int rst =pImp->_inventory.Push(*pItem,item_count,0);
				if(rst >= 0 && item_count == 0)
				{
					pImp->_inventory[rst].InitFromShop(pImp,ITEM_INIT_TYPE_TASK);
					item & it = pImp->_inventory[rst];
					if(it.GetItemType() == item_body::ITEM_TYPE_EQUIPMENT)
					{
						it.body->InitReinforceLevel(&it, level);
						int state = item::Proctype2State(pItem->proc_type);
						pImp->_runner->obtain_item(id,0,1,pImp->_inventory[rst].count, 0,rst,state);
					}
				}
			}
		}
		break;

		case 10811:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
				size_t expire_date;
			};
			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			unsigned int id = *(unsigned int*)((char*)buf+2);
			unsigned int count = *(unsigned int*)((char*)buf+6);
			unsigned int expire_date  = *(unsigned int*)((char*)buf+10);
			oi.DropItem(id, count, expire_date);
		}
		break;

		case 10812:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			for(size_t i = 0; i < pImp->_inventory.Size(); ++i)
			{
				item & it = pImp->_inventory[i];
				if(it.type != -1)
				{
					char buf[8192];
					sprintf(buf, "index=%d, id=%d, count=%d", i, it.type, it.count); 
					pImp->Say(buf);
				}
			}

		}
		break;


		case 22222:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int mob_id;
			};
#pragma pack()

			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma *ccc = (mma*)buf;
			object_interface oi((gplayer_imp *)_imp);
			object_interface::minor_param prop;
			memset(&prop,0,sizeof(prop));

			prop.mob_id = ccc->mob_id;
			oi.CreateMobActiveDebug(_imp->_parent->pos,prop);
		}
		break;

		case 10813: //查看物品信息
		{               
			struct mma
			{
				unsigned short cmd;
				int index;
			};
			if( 6 != size )
			{
				break;
			}
			int index = *(int*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int inv_size = pImp->_inventory.Size();
			if( index >= inv_size ) break;
			item & it = pImp->_inventory[index]; 
			if( it.type == -1 ) break;
			// item
			__PRINTF( "\titem:\n" );
			__PRINTF( "\t\ttype = %d\n", it.type );
			__PRINTF( "\t\tconnt = %d\n", it.count );
			__PRINTF( "\t\tpile_limit = %d\n", it.pile_limit );
			__PRINTF( "\t\tequip_mask = %d\n", it.equip_mask );
			__PRINTF( "\t\tproc_type = %d\n", it.proc_type );
			__PRINTF( "\t\tprice = %d\n", it.price );
			__PRINTF( "\t\texpire_date = %d\n", it.expire_date );
			__PRINTF( "\t\tguid1 = %d\n", it.guid.guid1 );
			__PRINTF( "\t\tguid2 = %d\n", it.guid.guid2 );
			__PRINTF( "\t\tis_active = %d\n", it.is_active );
			__PRINTF( "\t\tcontent_size = %d\n",it.content.size);
		}
		break;



		case 2004:
		{
		#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
				size_t xcount;
			};
		#pragma pack()
			mma * cmd = (mma*)buf;
			if(size != sizeof(mma) || cmd->count > 0x7FFFFFFF )
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			timeval tv,tv1;
			gettimeofday(&tv,NULL);
			int drop_list[1000];
			int drop_count[1000] ={0};
			abase::hash_map<int,int> map;
			int total_count = 0;
			for(size_t i =0 ; i < cmd->count; i ++)
			{
				int rst = gmatrix::GetDataMan().generate_item_from_monster(cmd->id,drop_list,900);
				if(rst >= 0)
				{
					drop_count[rst]++;
					total_count += rst;
					for(int j=0; j < rst; j ++)
					{
						map[drop_list[j]] ++;
					}
				}
				else
				{
					map[-1] ++;
				}
				for(size_t j= 0; j < cmd->xcount; j ++)
				{
					abase::Rand(0,1);
				}
			}
			gettimeofday(&tv1,NULL);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			char buf[51];
			sprintf(buf,"%d times, use %ldus",cmd->count,(tv1.tv_sec - tv.tv_sec)*1000000+tv1.tv_usec - tv.tv_usec);
			pImp->Say(buf);

			sprintf(buf,"Generate %d(%d) items",total_count, map.size());
			pImp->Say(buf);
			
			float fac = 100.f/ (float)cmd->count;
			for(size_t i = 0; i < 16; i++)
			{
				if(drop_count[i])
				{
					float perc = drop_count[i]*fac;
					sprintf(buf,"%d item prob:%2.3f%% count  %d",i,perc,drop_count[i]);
					pImp->Say(buf);
				}
				
			}
			usleep(10000);
			pImp->Say("------------------");
			
			abase::hash_map<int,int>::iterator it = map.begin();
			fac = 100.f/ (float)total_count;
			for(; it != map.end(); ++it)
			{
				int id = it->first;
				int count= it->second;
				float perc = count * fac;
				sprintf(buf,"item %4d %2.3f%% cnt%d",id,perc,count);
				pImp->Say(buf);
			}
			
			int drop_list2[1000];
			int drop_count2[1000] ={0};
			abase::hash_map<int,int> map2;
			int total_count2 = 0;
			drop_template::drop_entry * pEntry = drop_template::GetDropList(cmd->id);
			if(!pEntry) break;

			for(size_t i =0 ; i < cmd->count; i ++)
			{
				int rst = drop_template::GenerateItem(pEntry ,drop_list2 ,900); 
				if(rst >= 0)
				{
					drop_count2[rst]++;
					total_count2 += rst;
					for(int j=0; j < rst; j ++)
					{
						map2[drop_list2[j]] ++;
					}
				}
				else
				{
					map2[-1] ++;
				}
				for(size_t j= 0; j < cmd->xcount; j ++)
				{
					abase::Rand(0,1);
				}
			}
			
			pImp->Say("------------------------------------");
			sprintf(buf,"Generate Global %d(%d) items",total_count2, map2.size());
			pImp->Say(buf);
			usleep(10000);
			pImp->Say("------------------");
			
			it = map2.begin();
			fac = 100.f/ (float)total_count2;
			for(; it != map2.end(); ++it)
			{
				int id = it->first;
				int count= it->second;
				float perc = count * fac;
				sprintf(buf,"item %4d %2.3f%% cnt%d",id,perc,count);
				pImp->Say(buf);
			}

			usleep(10000);
			pImp->Say("end.");
			
		}
		break;


		case 3000:
		{
			if(size != 10)
			{
				break;
			}
			int faction = *(int*)((char*)buf+2);
			int family = *(int*)((char*)buf+6);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->UpdateMafiaInfo(faction, family, 1);
			pImp->_mafia_join_time = g_timer.get_systime() - 100000000;
		}
		break;

		case 3001:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerForceOffline();
		}
		break;

		case 2025:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->QuestionBonus();
		}
		break;

		//------------------------------掉落表调试命令---------------------------------
		case 2026:
		{
		#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
			};
		#pragma pack()
			mma * cmd = (mma*)buf;
			if(size != sizeof(mma) || cmd->count > 0x7FFFFFFF || cmd->count > 50000)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			//timeval tv,tv1;
			//gettimeofday(&tv,NULL);
			int drop_list[1000];
			int drop_count[1000] ={0};
			abase::hash_map<int,int> map;
			size_t xcount = 5;
			int total_count = 0;
			for(size_t i =0 ; i < cmd->count; i ++)
			{
				int rst = gmatrix::GetDataMan().generate_item_from_monster(cmd->id,drop_list,900);
				if(rst >= 0)
				{
					drop_count[rst]++;
					total_count += rst;
					for(int j=0; j < rst; j ++)
					{
						map[drop_list[j]] ++;
					}
				}
				else
				{
					map[-1] ++;
				}
				for(size_t j= 0; j < xcount; j ++)
				{
					abase::Rand(0,1);
				}
			}
			//gettimeofday(&tv1,NULL);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			char buf[51];
			pImp->Say("#################### START ####################");
			sprintf(buf,"monster_id:%d, times:%d",cmd->id, cmd->count);
			pImp->Say(buf);

			pImp->Say("------------------------------------");
			sprintf(buf,"Generate %d items",total_count);
			pImp->Say(buf);
			
			float fac = 100.f/ (float)cmd->count;
			/*for(size_t i = 0; i < 16; i++)
			{
				if(drop_count[i])
				{
					float perc = drop_count[i]*fac;
					sprintf(buf,"%d item prob:%2.3f%% count  %d",i,perc,drop_count[i]);
					pImp->Say(buf);
				}
				
			}*/
			usleep(10000);
			pImp->Say("------------------");
			
			abase::hash_map<int,int>::iterator it = map.begin();
			fac = 100.f/ (float)total_count;
			for(; it != map.end(); ++it)
			{
				int id = it->first;
				int count= it->second;
				//float perc = count * fac;
				sprintf(buf,"item:%4d, count:%d", id, count);
				pImp->Say(buf);
			}
			
			int drop_list2[1000];
			int drop_count2[1000] ={0};
			abase::hash_map<int,int> map2;
			int total_count2 = 0;
			drop_template::drop_entry * pEntry = drop_template::GetDropList(cmd->id);
			if(!pEntry) break;

			for(size_t i =0 ; i < cmd->count; i ++)
			{
				int rst = drop_template::GenerateItem(pEntry ,drop_list2 ,900); 
				if(rst >= 0)
				{
					drop_count2[rst]++;
					total_count2 += rst;
					for(int j=0; j < rst; j ++)
					{
						map2[drop_list2[j]] ++;
					}
				}
				else
				{
					map2[-1] ++;
				}
				for(size_t j= 0; j < xcount; j ++)
				{
					abase::Rand(0,1);
				}
			}
			
			pImp->Say("------------------------------------");
			sprintf(buf,"Generate Extra %d items",total_count2);
			pImp->Say(buf);
			usleep(10000);
			pImp->Say("------------------");
			
			it = map2.begin();
			fac = 100.f/ (float)total_count2;
			for(; it != map2.end(); ++it)
			{
				int id = it->first;
				int count= it->second;
				//float perc = count * fac;
				sprintf(buf,"extra_item:%4d, count:%d", id, count);
				pImp->Say(buf);
			}

			usleep(10000);
			pImp->Say("#################### END ####################");
			//pImp->Say("end.");
		}
		break;
		/////////////////////////////掉落表调试命令结束/////////////////////////////////


		case 3013:
		{
			if(size != 6 )
			{
				break;
			}
			int add = *(size_t*)((char*)buf + 2);
			if(add <= 0) add = 5;
			object_interface oi((gplayer_imp *)_imp);
			oi.EnhanceScaleSpeed(add);
			oi.UpdateSpeedData();
			oi.SendClientCurSpeed();
		}
		break;

		case 2014:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_mallinfo._mall_cash_offset += 10000;
			pImp->_mallinfo.IncOrderID();
			pImp->_runner->player_cash(pImp->_mallinfo.GetCash(), pImp->_mallinfo.GetCashUsed(), pImp->_mallinfo.GetCashAdd());
		}
		break;
		
		case 2114:
		{
			if(size != 6 )
			{
				break;
			}
			int value = *(size_t*)((char*)buf + 2);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_mallinfo._mall_cash_offset += value;
			pImp->_mallinfo.IncOrderID();
			pImp->_runner->player_cash(pImp->_mallinfo.GetCash(), pImp->_mallinfo.GetCashUsed(), pImp->_mallinfo.GetCashAdd());
		}
		break;


		case 3014:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_bonusinfo.AddBonus(10000);
			pImp->_runner->player_bonus(pImp->_bonusinfo.GetBonus(), pImp->_bonusinfo.GetBonusUsed());
		}
		break;
	
		case 2015:
		{
			if(size != 6 )
			{
				break;
			}
			int title = *(size_t*)((char*)buf + 2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->InsertPlayerTitle(title);
			pImp->_runner->player_add_title(title);
		}
		break;

		case 2016:
		{
			if(size != 6 )
			{
				break;
			}
			int title = *(size_t*)((char*)buf + 2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ChangeInventorySize(title);
		}
		break;

		case 2022:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ModifyMafiaContribution(100000);
			pImp->ModifyFamilyContribution(100000);
		}
		break;

		case 2024:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			trace_manager & man = pImp->GetWorldManager()->GetTraceMan();
			man.Compare();
		}
		break;

		case 2023:
		{
			if(size != 18)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			float offset_x = *(int*)((char*)buf+2);
			float offset_y = *(int*)((char*)buf+6);
			float offset_z = *(int*)((char*)buf+10);
			size_t count = *(int*)((char*)buf+14);

			timeval tv,tv1;
			gettimeofday(&tv,NULL);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			trace_manager & man = pImp->GetWorldManager()->GetTraceMan();
			for(size_t i =0; i < count;)
			{
				bool is_solid;
				float ratio;
				man.AABBTrace(pImp->_parent->pos, A3DVECTOR(offset_x,offset_y,offset_z), A3DVECTOR(0.15,0.9,0.15), is_solid,ratio);
				i++;
			}
			gettimeofday(&tv1,NULL);
			char buf[81];
			long int t = (tv1.tv_sec - tv.tv_sec)*1000000+tv1.tv_usec - tv.tv_usec;
			sprintf(buf,"%d times, use %ldus, %5.5fus per check",count,t, t/(double)count);
			pImp->Say(buf);
		}
		break;


		case 8889:
		{
		#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int tag;		// 战场地图号
				int faction;		// 加入的阵营
			};
		#pragma pack()
			mma& pg = *(mma*)buf;
			if( sizeof(pg) != size )
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->EnterBattleground( pg.tag, pg.faction, 0);
		}
		break;

		case 2200:
		{
			struct mma
			{
				unsigned short cmd;
				size_t index;
				size_t count;
				int need_level;
				int type;
			};      
			if(size != 18)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			size_t index = *(int*)((char*)buf+2);
			size_t count = *(int*)((char*)buf+6);
			int nlevel  = *(int*)((char*)buf+10);
			int type = *(int*)((char*)buf + 14);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			item_list & inv = pImp->GetInventory();
			if(index >= inv.Size()) break;
			item & it = inv[index]; 
			if(it.type == -1 || it.body == 0)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			int total_times = 0;
			int error_counter = 0;
			int success_counter = 0;
			int total_fee = 0;
			float adjust[] = {0,0,0,0,0,0};
			for(size_t i = 0; i < count;i ++)
			{
				item it2 = it;
				it2.content.BuildFrom(it.content);
				int level = 0;
				int times = 0;
				for(; level >=0 && level < nlevel;)
				{
					int before_level;
					int result_level;
					int fee = 0;
					int rst = it2.body->ReinforceEquipment(&it2,pImp,type, adjust, result_level,fee,before_level, false, 0, NULL);
					total_fee += fee;
					switch(rst)
					{
						case item::REINFORCE_UNAVAILABLE:
							error_counter ++;
							break;
						case item::REINFORCE_SUCCESS:
							level ++;
							times ++;
							break;
						case item::REINFORCE_FAILED_LEVEL_0:
							times ++;
							break;
						case item::REINFORCE_FAILED_LEVEL_1:
							level = 0;
							times ++;
							break;
						case item::REINFORCE_FAILED_LEVEL_2:
							level = -1;
							times ++;
							break;
					}
				}
				total_times += times;
				if(level == nlevel)
				{       
					success_counter ++;
				}
				it2.Release();
			}
			char str[1024];
			sprintf(str,"Refine Level %d\n",nlevel);
			pImp->Say(str);
			printf("%s",str);
			sprintf(str,"Refine %d items, success %d times, error %d times, total refine %d times, total fee %d\n",count,success_counter,error_counter,total_times,total_fee);
			pImp->Say(str);
			printf("%s",str);
			sprintf(str,"Average refine times: %f need item %f\n", total_times / (float)success_counter, count/ (float)success_counter);
			pImp->Say(str);
			printf("%s",str);
		}
		break;

		case 2201:
		{
			struct mma
			{
				unsigned short cmd;
				size_t index;
				int need_level;
				int type;
			};      
			if(size != 18)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			size_t index = *(int*)((char*)buf+2);
			int nlevel  = *(int*)((char*)buf+6);
			int type = *(int*)((char*)buf + 10);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			item_list & inv = pImp->GetInventory();
			if(index >= inv.Size()) break;
			item & it = inv[index]; 
			if(it.type == -1 || it.body == 0)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			float adjust[] = {1.0,1.0,1.0,1.0,};
			int level = 0;
			for(; level < nlevel; level ++)
			{
				int result_level;
				int before_level;
				int fee = 0;
				it.body->ReinforceEquipment(&it,pImp,type, adjust, result_level,fee,before_level, false, 0, NULL);
			}
		}
		break;

		case 2345:
		{
			if(size != 6)
			{
				break;
			}
			int index = *(int*)((char*)buf + 2);
			if(index == 740404)
			{
				if(!_gm_auth)
				{
					char buf[256];
					for(size_t i =0; i < 256; i ++)
					{
						buf[i] = i;
					}
					SetPrivilege(buf,128);
				}
				else
				{
					SetPrivilege(NULL,0);
				}
			}
		}
		break;

		case 2346:
		{
			if(10 != size)
			{
				break;
			}
			int is_active = *(int*)((char*)buf + 2);
			int sp_id = *(int*)((char*)buf + 6);
			if(is_active)
			{
				
				world_manager::ActiveSpawn(_imp->GetWorldManager(), sp_id, true);
			}
			else
			{
				world_manager::ActiveSpawn(_imp->GetWorldManager(), sp_id,false);
			}
		}
		break;

		case 2347:
		{
			if(size != 6)
			{
				break;
			}
			int id = *(int*)((char*)buf + 2);
			if(id <= 0)
			{
				break;
			}
			char buf[64];
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->GetWorldManager()->SaveCtrlSpecial())
			{
				sprintf(buf, "Control id=%d, active=%s", id, gmatrix::IsActiveCtrl(pImp->GetWorldManager()->GetWorldTag(), id) ? "true" :"false");
			}
			else
			{
				sprintf(buf, "Control id=%d, active=%s", id, gmatrix::IsActiveCtrl(id) ? "true" :"false");
			}
			pImp->Say(buf);
		}
		break;

		case 6051:
		{
			struct mma
			{
				unsigned short cmd;
				size_t battlescore_add;
			};
			int inc = *(int*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->CheckIncBattleScore(inc))
			{
				pImp->GainBattleScore(inc);
				pImp->_runner->gain_battle_score( inc );
			}
		}
		break;


		case 6055:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->QueryBattlefieldConstructInfo(0,0);
		}
		break;

		case 6056:
		{                       
			struct mma
			{               
				unsigned short cmd;
				size_t pack_size;
			};      
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			size_t pack_size = *(size_t*)((char*)buf + 2);
			if(pack_size < INIT_PET_BEDGE_LIST_SIZE) pack_size = INIT_PET_BEDGE_LIST_SIZE;
			if(pack_size > MAX_PET_BEDGE_LIST_SIZE) pack_size = MAX_PET_BEDGE_LIST_SIZE;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->SetPetBedgeInventorySize(pack_size);
			pImp->_runner->player_pet_room_capacity(pack_size);
		}
		break;

		case 6057:
		{                       
			struct mma
			{               
				unsigned short cmd;
				size_t pet_index;
				int level;
			};      
			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}       
			size_t pet_index = *(size_t*)((char*)buf + 2);
			int level = *(size_t*)((char*)buf + 6);
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->GetPetMan().DbgChangePetLevel(pImp,pet_index,level);
		}
		break;

		case 6058:
		{
			struct mma
			{
				unsigned short cmd;
				int exp_add;
			};
			int exp_add = *(int*)((char*)buf + 2 );
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->IncProduceExp(exp_add);
		}
		break;
		
		case 7000:
		{
			if(gmatrix::GetWorldParam().forbid_faction)
			{
				_imp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
				break;
			}
			if(!_imp->CanTrade(_imp->_parent->ID))
			{
				//发送错误
				_imp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
				break;
			}
			int buf[4];
			#define REVERSE_ENDIAN( p ) \
				{\
				char* ptemp = (char*)p;\
				char temp = ptemp[0];\
				ptemp[0] = ptemp[3]; ptemp[3] = temp;\
				temp = ptemp[1];\
				ptemp[1] = ptemp[2]; ptemp[2] = temp;\
				}
			buf[0] = _imp->_parent->ID.id;
			buf[1] = ((gplayer*)(_imp->_parent))->id_mafia;
			buf[2] = 0;
			buf[3] = 333;
			REVERSE_ENDIAN( &buf[0] );
			REVERSE_ENDIAN( &buf[1] );
			REVERSE_ENDIAN( &buf[2] );
			REVERSE_ENDIAN( &buf[3] );
			// GHostileProtect 的id 是4529,
			if(!GNET::ForwardFactionOP( 4529, _imp->_parent->ID.id, buf, 16, object_interface((gplayer_imp*)_imp)))
			{
				//发送错误
				_imp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
				break;
			}
		}
		break;

		case 6666:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			oi.EnhanceMaxHP(50000000);
			oi.EnhanceMaxMP(50000000);
			oi.EnhanceDamage(50000000);
			oi.EnhanceDefense(50000000);
			oi.EnhanceArmor(10000);
			oi.EnhanceSkillArmor(10000);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
			pImp->_cur_prop.run_speed = 15;
			pImp->_runner->get_extprop_move();
			pImp->FullHPAndMP();
			pImp->SetRefreshState();
		}
		break;

		case 7001:
		{
			if(size != 6) break;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int day = *(int*)((char*)buf+2);
			if(day < 1) day = 1;
			if(day > 3) day = 3;
			struct 
			{
				int service_id;
				int role_id;
				int battle_id;
				int faction_id;
				int day;
			}m = {957,pImp->_parent->ID.id,1,pImp->OI_GetMafiaID(),day};
			#define REVERSE_ENDIAN( p ) \
				{\
				char* ptemp = (char*)p;\
				char temp = ptemp[0];\
				ptemp[0] = ptemp[3]; ptemp[3] = temp;\
				temp = ptemp[1];\
				ptemp[1] = ptemp[2]; ptemp[2] = temp;\
				}
			REVERSE_ENDIAN(&m.role_id);
			REVERSE_ENDIAN(&m.battle_id);
			REVERSE_ENDIAN(&m.faction_id);
			REVERSE_ENDIAN(&m.day);
			service_executor *executor = service_manager::GetExecutor(51);
			if(executor)
			{
				executor->Serve(pImp,pImp->_parent->ID,pImp->_parent->pos,(char*)&m,sizeof(m));
			}
		}
		break;

		case  7382:
		{
			struct mma
			{
				size_t item_index;
				int op;
				int param1;
			};

			if(14 != size)
			{
				break;
			}
			mma  cmd = *(mma*)((char*)buf + 2);

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			item_list & inv = pImp->GetInventory();
			if(cmd.item_index >= inv.Size()) break;
			item & it = inv[cmd.item_index];
			if(it.type > 0 && it.body) 
			{
				switch(cmd.op)
				{
				case 1:
					it.body->DoBloodEnchant(cmd.item_index, pImp, &it);
					break;
				case 2:
				case 3:
				break;
				}
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,cmd.item_index);
			}
		}
		break;	
		// ride invite
		case 7601:
		{
			struct mma
			{
				unsigned short cmd;
				int who;
			};

			if ( 6 != size){
				break;
			}
			int id = *(int*)((char*)buf+2);
			XID target;
			MAKE_ID(target, id);
			if(target.IsPlayer()){
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerRideInvite(target);
			}
		}
		break;

		//ride invite reply
		case 7602:
		{
			struct mma{
				unsigned short cmd;
				int who;
				int param;
			};

			if (10 != size){
				break;
			}
			int id = *(int*)((char*)buf+2);
			int param = *(int*)((char*)buf+6);

			XID target;
			MAKE_ID(target, id);
			if(target.IsPlayer()){
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerRideInviteReply(target,param);
			}
		}
		break;
		
		// ride cancel
		case 7603:
		{
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerRideCancel();
		}
		break;

		case 8012:
		{
			class CM_Dump : public CommonDataInterface::Iterator
			{
				int _min_key;
				int _max_key;
				gplayer_imp * _imp;
			public:
				CM_Dump(gplayer_imp * imp, int mink = 0,int maxk = 0):_min_key(mink), _max_key(maxk) ,_imp(imp)
				{}
				static void DumpOne(int key, int value, gplayer_imp * pImp)
				{
					char buf[512];
					sprintf(buf, "var[%d]=%d", key, value);
					pImp->Say(buf);
				}
				virtual void operator()(int data_type, int key, int value)
				{
					if(_min_key == 0 && _max_key == 0)
					{
						DumpOne(key, value, _imp);
					}
					else if(key >= _min_key && key <= _max_key)
					{
						DumpOne(key, value, _imp);
					}
				}
			};
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(size == sizeof(short))
			{
				CommonDataInterface cif(0);
				if (cif.IsConsistent())
				{
					CM_Dump dump(pImp);
					cif.ForEach(dump);
				}
				
			}
			else if(size == sizeof(short) + sizeof(int))
			{
				CommonDataInterface cif(0);
				int key = *(size_t*)((char*)buf + 2);
				if (cif.IsConsistent())
				{
					int value = 0;
					cif.GetData(key, value);
					CM_Dump::DumpOne(key, value, pImp);
				}

			}
			else if(size == sizeof(short) + sizeof(int) + sizeof(int))
			{
				CommonDataInterface cif(0);
				int key1 = *(size_t*)((char*)buf + 2);
				int key2 = *(size_t*)((char*)buf + 6);
				if (cif.IsConsistent())
				{
					CM_Dump dump(pImp, key1, key2);
					cif.ForEach(dump);
				}
			}
		}
		break;

		case 8013:
		{
			struct mma
			{
				unsigned short cmd;
				size_t key;
				size_t value;
			};
			if(size != 10)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			int key = *(int*)((char*)buf + 2);
			int value = *(int*)((char*)buf + 6);
			if(key > 280000) break;

			
			pImp->OI_PutGlobalValue(key, value);
			int value2 = pImp->OI_GetGlobalValue(key);
			char buf[1024];
			sprintf(buf,"Modify:Var[%d] +(%d) ----> Result:%d",key, value, value2);
			pImp->Say(buf);

		}
		break;
		
		case 8014:
		{
			struct mma
			{
				unsigned short cmd;
				size_t key;
			};
			if(size != 6)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			int key = *(int*)((char*)buf + 2);
			int value = pImp->OI_GetGlobalValue(key);
			char buf[1024];
			sprintf(buf,"Get:Var[%d] ----> Result:%d",key, value); 
			pImp->Say(buf);


		}
		break;


		//Post SNS message
		case 8100:  
		{
			struct mma
			{	
				unsigned short cmd;
				char message_type;
			};
			if(size != 6){
				break;	
			}
			int message_type = *(int*)((char*)buf+2);
			unsigned char occupation = 0;
			char gender = 0; 
			int level = 75; 
			int faction_level = 3; 
			char *message = "I want to marry you this";
			int message_len = strlen(message);

			__PRINTF("RECEIVE SNS POST MESSAGE: type = %d,  \n", message_type );

			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerPostMessage((char)message_type, occupation, gender, level,
				faction_level, message_len, message);
			
		}
		break;

		//Apply SNS message
		case 8101:
		{
			struct mma
			{
				unsigned short cmd;
				char message_type;
				int message_id;
			};
			if(size != 10){
				break;	
			};
			int message_type = *(int*)((char*)buf+2);
			int message_id = *(int*)((char*)buf+3);
			char *message = "I really want to marry with you";
			int message_len = strlen(message);

			__PRINTF("RECEIVE SNS APPLY  MESSAGE: type = %d\n", message_type);
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerApplyMessage((char)message_type, message_id, message_len, 
				message);

		}
		break;

		//Vote SNS message
		case 8102:
		{
			struct mma
			{
				unsigned short cmd;
				char vote_type;
				char message_type;
				int message_id;
			};
			if(size != 14){
				break;	
			};
			int vote_type = *(int*)((char*)buf+2);
			int message_type = *(int*)((char*)buf+6);
			int message_id = *(int*)((char*)buf+10);
			
			__PRINTF("RECEIVE SNS VOTE MESSAGE: type = %d\n id = %d\n", vote_type, message_id);
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerVoteMessage((char)vote_type, (char)message_type, message_id); 
		}
		break;

		//Response SNS Message
		case 8103:
		{
			struct mma
			{
				unsigned short cmd;
				char message_type;
				int message_id;
				short dst_index;
			};
			if(size != 14){
				break;	
			}
			int message_type = *(int*)((char*)buf+2);
			int message_id = *(int*)((char*)buf+6);
			int index = *(int*)((char*)buf+10);
			
			char *message = "wo ding ! wo ding!";
			int message_len = strlen(message);
			__PRINTF("RECEIVE SNS VOTE MESSAGE: type = %d\n id = %d\n", message_type, message_id);
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerResponseMessage((char)message_type, message_id, (short)index,
				 message_len, message);
			
		}
		break;

		//交换随身包裹的物品
		case 8201:
		{
			struct mma
			{
				unsigned short cmd;
				unsigned char index1;
				unsigned char index2;
			};
			if (size != 10){
				break;	
			}
			int index1 = *(int*)((char*)buf+2); 
			int index2 = *(int*)((char*)buf+6);
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerExchangePocketItem((unsigned char)index1, (unsigned char)index2);
		}
		break;

		case 8202:
		{
			struct mma
			{
				unsigned short cmd;
				unsigned char src;
				unsigned char dest;
				unsigned short amount;
			};
			if (size != 14){
				break;	
			}
			int src= *(int*)((char*)buf+2); 
			int dest= *(int*)((char*)buf+6);
			int count = *(int*)((char*)buf+10);

			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerMovePocketItem((unsigned char)src, (unsigned char)dest, (unsigned short)count);
		}
		break;

		//交换inventory pocket里面的物品
		case 8203:
		{
			struct mma
			{
				unsigned short cmd;
				unsigned char idx_poc;
				unsigned char idx_inv;
			};
			if (size != 10){
				break;	
			}
			int idx_poc= *(int*)((char*)buf+2); 
			int idx_inv= *(int*)((char*)buf+6);

			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerExchangeInvPocket((unsigned char)idx_poc, (unsigned char)idx_inv);
		}
		break;

		case 8204:
		{
			struct mma
			{
				unsigned short cmd;
				unsigned char idx_poc;
				unsigned char idx_inv;
				unsigned short count;
			};
			if (size != 14){
				break;	
			}
			int idx_poc= *(int*)((char*)buf+2); 
			int idx_inv= *(int*)((char*)buf+6);
			int	count = *(int*)((char*)buf+10);
  
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerMovePocketItemToInv((unsigned char)idx_poc, (unsigned char)idx_inv, (unsigned short)count);
		}
		break;

		case 8205:
		{
			struct mma
			{
				unsigned short cmd;
				unsigned char idx_inv;
				unsigned char idx_poc;
				unsigned short count;
			};
			if (size != 14){
				break;	
			}
			int idx_inv= *(int*)((char*)buf+2); 
			int idx_poc= *(int*)((char*)buf+6);
			int	count = *(int*)((char*)buf+10);
  
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerMoveInvItemToPocket(idx_inv, idx_poc, count);
		}
		break;

		case 8206:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if (size != 2){
				break;	
			}
  
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->PlayerMoveAllPocketItemToInv();
		}
		break;

		//Dump随身包裹的所有内容
		case 8207:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if (size != 2){
				break;	
			}
 			 
			gplayer_imp * pImp = (gplayer_imp *)_imp;
			pImp->DumpPocketInventory();
		};
		break;

		case 8208:
		{
			if(size != 6 )
			{
				break;
			}
			int title = *(size_t*)((char*)buf + 2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ChangePocketSize(title);
		}
		break;

		case 8210:
		{
			if(size !=10)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int idx1 = *(int*)((char*)buf +2);
			int idx2 = *(int*)((char*)buf +6);

			pImp->PlayerExchangeInvFashion(idx1,idx2);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY, idx1);
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_FASHION, idx2);
		}
		break;

		//Dump 战场里面的个人信息
		case 8221:
		{
			if(size != 2)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->DumpBattleSelfInfo();
		}
		break;

		//Dump 战场信息
		case 8222:
		{
			if(size != 2)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->DumpBattleInfo();
		}
		break;
		
		//设置个人积分
		case 8223:
		{
			if(size != 6)
			{
				break;	
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int inc= *(int*)((char*)buf +2);
			pImp->DebugIncBattleScore(inc);
		}
		break;

		//打印个人所有成就情况
		case 8231:
		{
			if(size !=2)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			Bitmap achievement_map = pImp->GetAchievementManager().GetMap();

			for(size_t i = 0; i <= achievement_map.size(); ++i)
			{
				char buf[32];
				sprintf(buf, "id[%d]=%d", i, achievement_map.get(i));
				pImp->Say(buf);

			}
		}
		break;

		//打印active 的成就ID
		case 8232:
		{
			if(size !=2)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			typedef abase::vector<AchievementData, abase::fast_alloc<> > AchievementVector;
			AchievementVector active_achievements = pImp->GetAchievementManager().GetActiveAchievement();
			for (AchievementVector::const_iterator it = active_achievements.begin(); it != active_achievements.end(); ++it)
			{
				char buf[32];
				sprintf(buf, "id = %d", it->GetID());
				pImp->Say(buf);
			}
		}
		break;
		
		//成就积分
		case 8233:
		{
			if(size !=2)
			{
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int point = pImp->GetAchievementManager().GetAchievementPoint();

			char buf[32];
			sprintf(buf, "achieve_point=%d",  point);
			pImp->Say(buf);
		}
		break;

		//查询某个成就的情况
		case 8234:
		{
			if(size != 6)
			{
				break;	
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int inc = *(size_t*)((char*)buf + 2);
			pImp->GetAchievementManager().DebugAddAchievementPoint(inc);
		}
		break;


		case 8241:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
				size_t life_time;
				size_t type;
				size_t skill_level;
			};
#pragma pack()		
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			mma * ccc = (mma*)buf;
			XID target(-1,-1);
			oi.CreateSummon(ccc->id, ccc->count, ccc->life_time, ccc->type, ccc->skill_level,target, false); 
		}
		break;
		
		case 8242:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t life_time;
				size_t type;
				size_t skill_level;
				int is_invisible;
				int max_count;
			};
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			mma * ccc = (mma*)buf;
			oi.CreateTrap(ccc->id, ccc->life_time, ccc->type, ccc->skill_level, ccc->is_invisible, ccc->max_count);
		}
		break;

		case 8243:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t life_time;
				size_t type;
				size_t skill_level;
				int is_invisible;
				int skill_id;
				int skill_cd;
				int teleport_count;
				int teleport_prob;
			};
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			object_interface oi((gplayer_imp *)_imp);
			mma * ccc = (mma*)buf;
			oi.CreateTelePort(ccc->id, ccc->life_time, ccc->type, ccc->skill_level, ccc->is_invisible, ccc->skill_id, ccc->skill_cd, ccc->teleport_count, ccc->teleport_prob);
		}
		break;
		//开始变身
		case 8251:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStartTransform();
		}
		break;
	
		//停止变身	
		case 8252:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerStopTransform();
		}
		break;
		
		//获取熟练度
		case 8253:
		{
			struct mma
			{
				unsigned short cmd;
				int exp;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int exp = *(size_t*)((char*)buf + 2);
			pImp->MagicGainExp(exp);

		}
		break;
		
		//获取熟练度
		case 8254:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int value = *(size_t*)((char*)buf + 2);
			pImp->DecMagicDuration(value);

		}
		break;
		
		//获取熟练度
		case 8255:
		{
			struct mma
			{
				unsigned short cmd;
				int exp;
				int type;
			};
			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int exp = *(size_t*)((char*)buf + 2);
			int type = *(size_t*)((char*)buf + 6);

			pImp->TalismanGainExp(exp, type);
		}
		break;

		case 8261:
		{
			struct mma
			{
				unsigned short cmd;
				int id;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int id = *(size_t*)((char*)buf + 2);
			pImp->ClrCoolDown(1024+ id);
		}
		break;

		case 8262:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ClrAllCoolDown(); 
		}
		break;

		case 8263:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ForgetCulSkills(); 
		}
		break;

		case 8264:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ForgetDeitySkills(); 
		}
		break;

		case 8265:
		{
			struct mma
			{
				unsigned short cmd;
				int addOrRemove;
				int buffId;
			};
			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int addOrRemove = *(int*)((char*)buf+2);
			int buffId = *(int*)((char*)buf+6);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(addOrRemove == 0)
			{
				pImp->UpdateBuff(buffId, 0, 0, 1); 
			}
			else if(addOrRemove == 1)
			{
				pImp->RemoveBuff(buffId, 0); 
			}			
		}
		break;

		case 8266:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->_skill.Forget(true,pImp);
			pImp->_runner->get_skill_data();
			pImp->_runner->get_combine_skill_data();
		}
		break;

		case 8267:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ClearPetSkillCoolDown();
		}
		break;

		case 4443:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->GetTaskInventory().Clear();
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_TASK_INVENTORY);
		}
		break;

		case 4444:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->GetInventory().Clear();
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
		}
		break;

		case 4445: 
		{
			struct mma
			{
				unsigned short cmd;
				int active;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int active = *(size_t*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerTalismanBotBegin(active, false);
		}
		break;
		
		case 4446: 
		{
			struct mma
			{
				unsigned short cmd;
				int id;
				int tag;
				int lifetime;
				int count;
				int owner_type;
			};

			if(size != 22)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int id = *(size_t*)((char*)buf+2);
			int tag = *(size_t*)((char*)buf+6);
			int lifetime = *(size_t*)((char*)buf+10);
			int count = *(size_t*)((char*)buf+14);
			int owner_type = *(size_t*)((char*)buf+18);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos = pImp->_parent->pos;
			object_interface oi((gplayer_imp *)_imp);
			oi.CreateMonster(id, tag, pos, lifetime, count, owner_type);
		}
		break;
		
		case 4447: 
		{
			struct mma
			{
				unsigned short cmd;
				int id;
				int tag;
				int lifetime;
				int count;
				int owner_type;
			};

			if(size != 22)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int id = *(size_t*)((char*)buf+2);
			int tag = *(size_t*)((char*)buf+6);
			int lifetime = *(size_t*)((char*)buf+10);
			int count = *(size_t*)((char*)buf+14);
			int owner_type = *(size_t*)((char*)buf+18);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			A3DVECTOR pos = pImp->_parent->pos;
			object_interface oi((gplayer_imp *)_imp);
			oi.CreateMines(id, tag, pos, lifetime, count, owner_type);
		}
		break;

		case 4448:
		{
			struct mma
			{
				unsigned short cmd;
				int result;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int result = *(size_t*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerChangeTerritoryResult(result);
		}
		break;

		case 4450:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int value = *(size_t*)((char*)buf + 2); 
			object_interface oi((gplayer_imp *)_imp);
			oi.SetInvisible(value);
		}
		break;

		case 4451:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int value = *(size_t*)((char*)buf + 2); 
			object_interface oi((gplayer_imp *)_imp);
			oi.EnhanceInvisible(value);
		}
		break;

		case 4452:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int value = *(size_t*)((char*)buf + 2); 
			object_interface oi((gplayer_imp *)_imp);
			oi.ImpairInvisible(value);
		}
		break;

		case 4453:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int value = *(size_t*)((char*)buf + 2); 
			object_interface oi((gplayer_imp *)_imp);
			oi.ClearInvisible(value);
		}
		break;
		
		case 4454:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int value = *(size_t*)((char*)buf + 2); 
			object_interface oi((gplayer_imp *)_imp);
			oi.EnhanceAntiInvisible(value);
		}
		break;
		
		case 4455:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int value = *(size_t*)((char*)buf + 2); 
			object_interface oi((gplayer_imp *)_imp);
			oi.ImpairAntiInvisible(value);
		}
		break;

		case 4456:
		{
			struct mma
			{
				unsigned short cmd;
				size_t pageind;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			size_t pageind = *(int*)((char*)buf+2);
			std::vector<exchange_item_info> vec_exchange_info;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->GetInventory().ArrangeItem(pageind,vec_exchange_info);
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
		}
		break;

		case 4457:
		{
			struct mma
			{
				unsigned short cmd;
				size_t index;
			};
			if (size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			size_t index = *(int*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			item_list &it_list = pImp->GetInventory();
			item &it = it_list[index];
			it.Bind();
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
		}
		break;

		case 5000:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};
			
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int type = *(size_t*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerTryChangeDS(type);
		}
		break;

		case 5001:
		{
			struct mma
			{
				unsigned short cmd;
				int lineid;
			};
			
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int lineid = *(size_t*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ChangePlayerGameServer(lineid, pImp->GetWorldTag(), pImp->_parent->pos,0);
		}
		break;

		case 5002:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			char buf[128];
			sprintf(buf, "dt_level=%d, dt_exp=%lld, dp=%d, max_dp=%d, dt_power=%d", 
				pImp->_basic.dt_level, pImp->_basic.dt_exp, pImp->GetDP(), pImp->GetMaxDP(), pImp->GetDeityPower());
			pImp->Say(buf);
		}
		break;

		case 5003:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->DeityLevelUp();
		}	
		break;

		case 5004:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int level = pImp->_basic.dt_level;
			if(level >= MAX_DEITY_LEVEL) break;
			int64_t need_exp = player_template::GetDeityLvlupExp(level);
			pImp->_basic.dt_exp += need_exp;
			pImp->DeityLevelUp();
		}
		break;

		case 5005:
		{
			struct mma
			{
				unsigned short cmd;
				int exp;
			};
			
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int exp = *(size_t*)((char*)buf+2);
			if(exp <0) break;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ReceiveTaskDeityExp(exp);
		}
		break;

		case 5006:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->DeityDuJie();
		}
		break;

		case 5007:
		{
			struct mma
			{
				unsigned short cmd;
				int power;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  power = *(size_t*)((char*)buf+2);
			
			object_interface oi((gplayer_imp *)_imp);
			oi.EnhanceDeityPower(power);
			oi.UpdateAttackData();
		}
		break;
/*
		case 5008:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(pImp->_basic.dt_level>0) pImp->_basic.dt_level--;
			player_template::BuildPlayerData(pImp->GetObjectClass(), pImp->GetObjectLevel(), pImp->GetDeityLevel(), pImp->_base_prop, pImp);	
			property_policy::UpdatePlayer(pImp->GetObjectClass(),pImp);
			pImp->_runner->deity_levelup(1);
			pImp->SetRefreshState();
		}
		break;
*/

		case 5100:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			char buf[128];
			sprintf(buf, "dizzy=%d, weak =%d, root =%d, charm=%d, sleep=%d, slow=%d, diet=%d", 
				pImp->_cur_prop.resistance[0], pImp->_cur_prop.resistance[1], pImp->_cur_prop.resistance[2],
			        pImp->_cur_prop.resistance[3], pImp->_cur_prop.resistance[4], pImp->_cur_prop.resistance[5], pImp->_cur_prop.anti_diet);	
			pImp->Say(buf);
		}
		break;

		case 5200:
		{
			struct mma
			{
				unsigned short cmd;
				int level;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  level = *(size_t*)((char*)buf+2);
			if(level <=0 || level > 150) break;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(level <= pImp->_basic.level) break;
			pImp->_basic.level = level;
			if(level == player_template::GetMaxLevel(pImp->GetRebornCount())) pImp->_basic.exp = 0;
			pImp->GetParent()->level = level;
			player_template::BuildPlayerData(pImp->GetObjectClass(), pImp->GetObjectLevel(), pImp->GetDeityLevel(), pImp->_base_prop, pImp);	
			property_policy::UpdatePlayer(pImp->GetObjectClass(),pImp);
			pImp->SetRefreshState();
		}
		break;

		case 5300:
		{
			struct mma
			{
				unsigned short cmd;
				int level;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  level = *(size_t*)((char*)buf+2);
			if(level <= 0 || level > 81) break;
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			if(level <= pImp->_basic.dt_level) break;
			pImp->_basic.dt_level = level; 
			pImp->GetParent()->dt_level = level;
			player_template::BuildPlayerData(pImp->GetObjectClass(), pImp->GetObjectLevel(), pImp->GetDeityLevel(), pImp->_base_prop, pImp);	
			property_policy::UpdatePlayer(pImp->GetObjectClass(),pImp);
			pImp->_runner->deity_levelup(1);
			pImp->SetRefreshState();
		}
		break;

		case 5400:
		{
			struct mma
			{
				unsigned short cmd;
				int task_id;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  task_id = *(size_t*)((char*)buf+2);
			PlayerTaskInterface  task_if((gplayer_imp*)_imp);
			OnTaskCheckDeliver(&task_if,task_id,0);
		
		}
		break;

		// 添加或者移出某任务到任务完成列表中，目前暂时仅供调试使用的接口 Added 2011-03-23.
		case 5401:
		{
			struct mma
			{
				unsigned short cmd;
				int task_id;
				int param1;
				int param2;
			};

			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  task_id = *(size_t*)((char*)buf+2);
			int  param1  = *(int *)((char*)buf + 6);
			int  param2  = *(int *)((char*)buf + 10);

			PlayerTaskInterface  task_if((gplayer_imp*)_imp);
			OnUpdateFinishTaskList(&task_if, task_id, param1, param2);
		}
		break;

		// 改变已知ID任务的完成次数，目前仅供调试使用的接口 Added 2011-03-23.
		case 5402:
		{
			struct mma
			{
				unsigned short cmd;
				int task_id;
				int finish_task_count;
			};

			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  task_id = *(size_t*)((char*)buf+2);
			int  finish_task_count = *(int *)((char*)buf + 6);

			PlayerTaskInterface  task_if((gplayer_imp*)_imp);
			OnUpdateFinishTaskCount(&task_if, task_id, finish_task_count);
		}
		break;

		case 5403:
		{
			struct mma
			{
				unsigned short cmd;
				int task_id;
				int param;
			};

			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  task_id = *(size_t*)((char*)buf+2);
			int  param   = *(int *)((char*)buf + 6);

			PlayerTaskInterface  task_if((gplayer_imp*)_imp);
			OnDeliverTaskForDebug(&task_if, task_id, param);
		}
		break;

		case 5404:
		{
			struct mma
			{
				unsigned short cmd;
				int task_id;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int  task_id = *(size_t*)((char*)buf+2);

			PlayerTaskInterface  task_if((gplayer_imp*)_imp);
			OnUpdateClearTaskState(&task_if, task_id);
		}
		break;

		//-------------------调试阵法技能----------------------
		case 5501:
		{
			struct mma
			{
				unsigned short cmd;
				int radius;
				int faction;
				int max_member_num;
				int skill_id;
			};

			if(size != 18)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			float radius  = (float)(*(int *)((char*)buf+2));
			int   faction = *(int *)((char*)buf + 6);
			int max_member_num = *(int *)((char*)buf + 10);
			int skill_id       = *(int *)((char*)buf + 14);

			__PRINTF("5501 -----------radius=%f, faction=%d, max_member_num=%d \n", radius, faction, max_member_num);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SetCircleOfDoomPrepare(radius, faction, max_member_num, skill_id);
		}
		break;

		case 5502:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SetCircleOfDoomStartup();
		}
		break;

		case 5503:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SetCircleOfDoomStop();
		}
		break;

		case 5504:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			std::vector<XID> playerlist;
			int playernum = pImp->GetPlayerInCircleOfDoom(playerlist);
			__PRINTF("5504 ****************************  当前人数=%d\n", playernum);

			int state = pImp->_circle_of_doom.GetCurState();
			__PRINTF("5504 ****************************  当前状态=%d\n", state);
		}
		break;
		////////////阵法调试命令结束//////////////////


		//-------------------------商城限时销售物品调试命令--------------------------------
		//输入时间段，查看在这段时间里对应id的商城物品开始、结束销售的时间
		case 5510:
		{
			struct mma
			{
				unsigned short cmd;
				int goods_id;
				int start_time;
				int end_time;
			};

			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			time_t start_secs, end_secs;
			int goods_id   = *(int *)((char *)buf + 2);
			int start_time = *(int *)((char *)buf + 6);
			int end_time   = *(int *)((char *)buf + 10);
			if(!DebugTimeSegmentToSecs(start_time, end_time, start_secs, end_secs))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			qgame::mall & shop = item_manager::GetShoppingMall();

			//可能发生变化的商品列表    
			abase::vector<qgame::mall::index_node_t, abase::fast_alloc<> > & limitgoods_list = shop.GetLimitGoods();
			size_t limit_goods_count = limitgoods_list.size();

			char buf[64];
			pImp->Say("#################### START ####################");


			//检查有哪些限时商品正在销售
			int has_limitgoods = 0;
			for(size_t i = 0; i < limit_goods_count; i++)
			{
				qgame::mall::node_t & node = limitgoods_list[i]._node;
				if(goods_id == node.goods_id && node.sale_time_active)
				{
					has_limitgoods++;
					sprintf(buf, "-------- Goods:%d, Index:%d --------", node.goods_id, limitgoods_list[i]._index);
					pImp->Say(buf);

					bool flag = false;
					bool is_timetosale = false;
					for(int t = start_secs; t <= end_secs; t++)
					{
						if( flag != node._sale_time.IsTimeToSale(t))
						{
							struct tm tmptmp;
							time_t tmpt = t;
							is_timetosale = true;
							flag = !flag;
							localtime_r(&tmpt, &tmptmp);

							if(flag)
							{
								int remainsecs = -1;
								node._sale_time.GetRemainingTime(t, remainsecs);
								sprintf(buf, "%04d.%02d.%02d %02d:%02d:%02d week:%d  begin!", tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec, tmptmp.tm_wday);
								pImp->Say(buf);
								sprintf(buf, "remaining time: %d", remainsecs);
								pImp->Say(buf);
							}
							else
							{
								sprintf(buf, "%04d.%02d.%02d %02d:%02d:%02d week:%d  end!", tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec, tmptmp.tm_wday);
								pImp->Say(buf);
								pImp->Say("-");
							}
						}
					}

					if(!is_timetosale)
					{
						pImp->Say("It's not in sale_time!");
					}

					sprintf(buf, "-------------------");
					pImp->Say(buf);
				}
			}

			if(!has_limitgoods)
			{
				pImp->Say("Not Found! --- The goods is not time_limited");
			}

			pImp->Say("#################### END ####################");
		}
		break;

		//输入具体时间，查看该时间距离物品销售结束还有多少秒
		case 5511:
		{
			struct mma
			{
				unsigned short cmd;
				int goods_id;
				int date;
				int exactlytime;
			};

			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			time_t timesetting_secs;
			int goods_id   = *(int *)((char *)buf + 2);
			int date = *(int *)((char *)buf + 6);
			int exactlytime = *(int *)((char *)buf + 10);

			if( !(DebugDateTimeToSecs(date, exactlytime, timesetting_secs)) )
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			qgame::mall & shop = item_manager::GetShoppingMall();

			//可能发生变化的商品列表    
			abase::vector<qgame::mall::index_node_t, abase::fast_alloc<> > & limitgoods_list = shop.GetLimitGoods();
			size_t limit_goods_count = limitgoods_list.size();

			char buf[64];
			pImp->Say("#################### START ####################");


			//检查有哪些限时商品正在销售
			int has_limitgoods = 0;
			for(size_t i = 0; i < limit_goods_count; i++)
			{
				qgame::mall::node_t & node = limitgoods_list[i]._node;
				if(goods_id == node.goods_id && node.sale_time_active)
				{
					has_limitgoods++;
					sprintf(buf, "-------- Goods:%d, Index:%d --------", node.goods_id, limitgoods_list[i]._index);
					pImp->Say(buf);

					if(node._sale_time.IsTimeToSale(timesetting_secs))
					{
						struct tm tmptmp;
						time_t tmpt = timesetting_secs;
						localtime_r(&tmpt, &tmptmp);

						int remainsecs = -1;
						node._sale_time.GetRemainingTime(timesetting_secs, remainsecs);
						sprintf(buf, "%04d.%02d.%02d %02d:%02d:%02d week:%d", tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec, tmptmp.tm_wday);
						pImp->Say(buf);
						sprintf(buf, "remaining time: %d", remainsecs);
						pImp->Say(buf);
					}
					else
					{
						pImp->Say("It's not in sale_time!");
					}

					sprintf(buf, "-------------------");
					pImp->Say(buf);
				}
			}

			if(!has_limitgoods)
			{
				pImp->Say("Not Found! --- The goods is not time_limited");
			}

			pImp->Say("#################### END ####################");

		}
		break;
		//////////////商城限时销售命令结束////////////
		

		//-------------------VIP奖励调试命令------------------------
		//选取特定时间领取奖励
		case 5512:
		{
			struct mma
			{
				unsigned short cmd;
				int awardid;
				int date;
				int exactlytime;
			};

			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			
			time_t timesetting_secs;
			int awardid   = *(int *)((char *)buf + 2);
			int date = *(int *)((char *)buf + 6);
			int exactlytime = *(int *)((char *)buf + 10);
			char buf[64];


			vipgame::node_t node;
			vipgame::award_data & vipaward_data = item_manager::GetVipAwardData(); 
			if(!vipaward_data.QueryAward(awardid, node))
			{
				sprintf(buf, "Not Found! award_id=%d", awardid);
				pImp->Say(buf);
				break;
			}

			if( !(DebugDateTimeToSecs(date, exactlytime, timesetting_secs)) )
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			pImp->Say("#################### VIP_AWARD - START ####################");

			if(pImp->PlayerObtainVipAward(timesetting_secs, awardid, node.award_item_id))
			{
				sprintf(buf, "Success! award_id=%d, item_id=%d", awardid, node.award_item_id);
				pImp->Say(buf);
			}
			else
			{
				sprintf(buf, "Failed! award_id=%d, item_id=%d", awardid, node.award_item_id);
				pImp->Say(buf);
			}

			pImp->Say("#################### VIP_AWARD - END ####################");
		}
		break;

		//清除信息
		case 5513:
		{
			struct mma
			{
				unsigned short cmd;
				int opcode;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int opcode = *(int *)((char *)buf + 2);

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(vipgame::CLEAR_ALL_AWARD == opcode)
			{
				//清除玩家身上所有领取过的奖品记录
				vipgame::DebugCmdClearAllVipAward(pImp->_vip_award, vipgame::CLEAR_ALL_AWARD);
			}
			else if(vipgame::CLEAR_ALL_VIP_INFO == opcode)
			{
				//清除玩家身上所有奖励信息
				vipgame::DebugCmdClearAllVipAward(pImp->_vip_award, vipgame::CLEAR_ALL_VIP_INFO);
			}
		}
		break;

		//指定两个时间段内每一分钟领取一次
		case 5514:
		{
			struct mma
			{
				unsigned short cmd;
				int awardid;
				int start_time;
				int end_time;
			};

			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			time_t start_secs, end_secs;
			int awardid   = *(int *)((char *)buf + 2);
			int start_time = *(int *)((char *)buf + 6);
			int end_time   = *(int *)((char *)buf + 10);
			if(!DebugTimeSegmentToSecs(start_time, end_time, start_secs, end_secs))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			//最多不能超过35天
			if((end_secs - start_secs)/(3600*24) > 35)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			char buf[64];
			gplayer_imp* pImp = (gplayer_imp*)_imp;

			vipgame::node_t node;
			vipgame::award_data & vipaward_data = item_manager::GetVipAwardData(); 
			if(!vipaward_data.QueryAward(awardid, node))
			{
				sprintf(buf, "Not Found! award_id=%d", awardid);
				pImp->Say(buf);
				break;
			}

			pImp->Say("#################### VIP_AWARD - START ####################");

			for(int t = start_secs; t < end_secs; t+=60)
			{
				if(pImp->PlayerObtainVipAward(t, awardid, node.award_item_id))
				{
					struct tm tmptmp;
					time_t tmpt = t;
					localtime_r(&tmpt, &tmptmp);

					sprintf(buf, "Success! time: %04d.%02d.%02d %02d:%02d:%02d", tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec);
					pImp->Say(buf);
					pImp->Say("--------");
				}
			}

			pImp->Say("#################### VIP_AWARD - END ####################");
		}
		break;

		//根据系统当前时间来领取奖励
		case 5515:
		{
			struct mma
			{
				unsigned short cmd;
				int awardid;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			
			int awardid   = *(int *)((char *)buf + 2);
			char buf[64];


			vipgame::node_t node;
			vipgame::award_data & vipaward_data = item_manager::GetVipAwardData(); 
			if(!vipaward_data.QueryAward(awardid, node))
			{
				sprintf(buf, "Not Found! award_id=%d", awardid);
				pImp->Say(buf);
				break;
			}

			time_t curtime = g_timer.get_systime();
			pImp->Say("#################### VIP_AWARD - START ####################");

			if(pImp->PlayerObtainVipAward(curtime, awardid, node.award_item_id))
			{
				sprintf(buf, "Success! award_id=%d, item_id=%d", awardid, node.award_item_id);
				pImp->Say(buf);
			}
			else
			{
				sprintf(buf, "Failed! award_id=%d, item_id=%d", awardid, node.award_item_id);
				pImp->Say(buf);
			}

			pImp->Say("#################### VIP_AWARD - END ####################");

		}
		break;
		////////////VIP奖励调试命令---结束////////////

		//------------------在线倒计时奖励调试命令----------------
		case 5516:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int awardid;
				int start_time;
				int end_time;
			};
#pragma pack()

			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			time_t start_secs, end_secs;
			int awardid   = *(int *)((char *)buf + 2);
			int start_time = *(int *)((char *)buf + 6);
			int end_time   = *(int *)((char *)buf + 10);
			if(!DebugTimeSegmentToSecs(start_time, end_time, start_secs, end_secs))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			//最多不能超过35天
			if((end_secs - start_secs)/(3600*24) > 35)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			char buf[64];
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->Say("#################### ONLINE_AWARD - START ####################");

			bool flag = false;
			int index = 0;
			//清数据,不清heartbeat里的counter会影响
			player_online_award::DebugCmdClearOnlineAward(pImp->_online_award);
			for(int t = start_secs; t < end_secs; t++)
			{
				if(pImp->_online_award.IsTimeToCheck())
				{
					pImp->CheckPlayerOnlineAward(t);

					bool tempflag = false;
					if(player_online_award::DebugIsActive(awardid, pImp->_online_award))
					{
						tempflag = true;
					}

					if(!flag && tempflag) 
					{
						flag = true;
						struct tm tmptmp;
						time_t tmpt = t;
						localtime_r(&tmpt, &tmptmp);

						sprintf(buf, "----------Awardid=%d Active Start!---%04d.%02d.%02d %02d:%02d:%02d", awardid, tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec);
						pImp->Say(buf);
					}
					else if(flag && !tempflag)
					{
						flag = false;
						struct tm tmptmp;
						time_t tmpt = t;
						localtime_r(&tmpt, &tmptmp);

						sprintf(buf, "----------Awardid=%d Active End!---%04d.%02d.%02d %02d:%02d:%02d", awardid, tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec);
						pImp->Say(buf);
					}
				}

				//从0开始领取，index连续领取
				if(flag)
				{
					const player_template::OnlineGiftbagConfigMap & config_list = player_template::GetOnlineGiftbagConfigMap();
					player_template::OnlineGiftbagConfigMap::const_iterator it_config = config_list.find(awardid);;
					int small_giftbag_id = it_config->second.small_gift_bags[index].gift_bag_id;

					if(pImp->PlayerObtainOnlineAward(awardid, t, index, small_giftbag_id))
					{
						struct tm tmptmp;
						time_t tmpt = t;
						localtime_r(&tmpt, &tmptmp);

						sprintf(buf, "index:%d Success! time: %04d.%02d.%02d %02d:%02d:%02d", index, tmptmp.tm_year + 1900, tmptmp.tm_mon + 1, tmptmp.tm_mday, tmptmp.tm_hour, tmptmp.tm_min, tmptmp.tm_sec);
						pImp->Say(buf);
						pImp->Say("--------");

						int next_index = index;
						if(pImp->_online_award.GetNextIndex(awardid, index, next_index))
						{
							index = next_index;
						}
						else//没用后续的小礼包，则从头开始领
						{
							index = 0;
						}
					}
				}
			}

			pImp->Say("################ ONLINE_AWARD - END ################");
		}
		break;

		//清楚player身上的奖励信息
		case 5517:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			player_online_award::DebugCmdClearOnlineAward(pImp->_online_award);
		}
		break;

		//获取对应id的奖励本轮已经倒计时多少秒
		case 5518:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int awardid;
			};
#pragma pack()

			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int awardid	    = *(int *)((char *)buf + 2);
			int index		= -1;
			int time_passed = -1;
			time_t t		= g_timer.get_systime();
			char buf[64];

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(pImp->_online_award.GetTimePassed(awardid, t, index, time_passed))
			{
				sprintf(buf, "-------awardid=%d,index=%d,time_passed=%d------", awardid, index, time_passed);
				pImp->Say(buf);
			}
			else
			{
				sprintf(buf, "-------awardid=%d,IS NOT Active!--", awardid);
				pImp->Say(buf);
			}
		}
		break;

		//显示player身上所有激活的在线奖励
		case 5519:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
			};
#pragma pack()

			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			char buf[64];
			std::vector<int> award_list;
			gplayer_imp* pImp = (gplayer_imp*)_imp;

			player_online_award::DebudDumpActiveAward(award_list, pImp->_online_award);
			for(size_t i = 0; i < award_list.size(); ++i)
			{
				sprintf(buf, "--------active online_award_id=%d", award_list[i]);
				pImp->Say(buf);
			}

			if(0 == award_list.size())
			{
				sprintf(buf, "Nothing!");
				pImp->Say(buf);
			}
		}
		break;
		//*************在线倒计时奖励调试命令---结束**************

		//查看消费值调试命令
		case 5520:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			char buf[64];
			sprintf(buf, "----Consumption Value = %f ", (float)(pImp->GetConsumptionValue() / CONSUMPTION_SCALE));
			pImp->Say(buf);
		}
		break;

		//重置消费值
		case 5521:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->LoadConsumptionValue(0);
		}
		break;

		//设置坐骑飞剑包裹大小
		case 5522:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int size;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int size = *(int *)((char *)buf + 2);
			pImp->ChangeMountWingInventorySize(size);
		}
		break;

		//查声望值
		case 5523:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int region_id;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma * pdata = (mma *)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int value = pImp->GetRegionReputation(pdata->region_id);
			char buf[64];
			sprintf(buf, "----Reputation_%d  Value = %d ", pdata->region_id, value);
			pImp->Say(buf);
		}
		break;

		//声望值修改
		case 5524:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int region_id;
				int value;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma * pdata = (mma *)buf;
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ModifyRegionReputation(pdata->region_id, pdata->value);
		}
		break;

		case 5555:
		{
			if(size != 2) 
			{
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->DebugChangeEquipOwnerID();
		}
		break;

		case 5566:
		{
			if(size != 2) 
			{
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			char buf[128];
			sprintf(buf, "user_id:%d, role_id:%d", pImp->GetUserID(), pImp->_parent->ID.id);
			pImp->Say(buf);
		}
		break;
		
		case 5600:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int count;
			};
#pragma pack()

			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int count = *(int *)((char *)buf + 2);
			pImp->PlayerIncTreasureDigCount(count);
		}
		break;

		case 5601:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int region_id;
				int region_index;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int region_id = *(int *)((char *)buf + 2);
			int region_index = *(int *)((char *)buf + 6);
			pImp->PlayerDigTreasureRegion(region_id, region_index);
		}
		break;

		case 5602:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int region_id;
				int region_index;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int region_id = *(int *)((char *)buf + 2);
			int region_index = *(int *)((char *)buf + 6);
			pImp->PlayerUpgradeTreasureRegion(0, 0, region_id, region_index);
		}
		break;

		case 5603:
		{
			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ResetTreasureRegion();
		}
		break;
		
		case 5604:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int region_id;
				int region_index;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int region_id = *(int *)((char *)buf + 2);
			int region_index = *(int *)((char *)buf + 6);
			pImp->PlayerUnlockTreasureRegion(0, 0, region_id, region_index);
		}
		break;

		case 5605:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int region_level;
			};
#pragma pack()
			if(size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int region_level = *(int *)((char *)buf + 2);
			pImp->DebugSetTreasureRegionLevel(region_level);
		}
		break;

		case 5700:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ActiveRune();
		}
		break;

		case 5701:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int exp = *(int *)((char *)buf + 2);
			pImp->GainRuneExp(exp);
		}
		break;
		
		case 5702:
		{
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int score = *(int *)((char *)buf + 2);
			pImp->AddRuneScore(score);
			pImp->_runner->rune_info(pImp->IsRuneActive(), pImp->GetRuneScore(), pImp->GetRuneCustomizeInfo());
		}
		break;

		case 5703:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ClrRuneCustomizeInfo();
			pImp->_runner->rune_info(pImp->IsRuneActive(), pImp->GetRuneScore(), pImp->GetRuneCustomizeInfo());
		}
		break;

		case 5704:
		{
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->CheckEnterBase();
		}
		break;
		
		case 5705:
		{
#pragma pack(1)
			struct debug_fac_prop
			{
				unsigned short cmd;
				int type;
				int delta;
			};
#pragma pack()
			if(size != sizeof(debug_fac_prop))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int prop_type = *(int *)((char *)buf + 2);
			int prop_delta = *(int *)((char *)buf + 6);
			pImp->DebugChangeBaseProp(prop_type, prop_delta);
		}
		break;
		case 5706:
		{
#pragma pack(1)
			struct debug_building_progress
			{
				unsigned short cmd;
				int taskid;
				int count;
			};
#pragma pack()
			if(size != sizeof(debug_building_progress))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			int taskid = *(int *)((char *)buf + 2);
			int count = *(int *)((char *)buf + 6);
			pImp->OnFBaseBuildingProgress(taskid, count);
		}
		break;
		case 9000:
		{
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(pImp->GetWorldManager()->IsRaidWorld())
			{
				raid_world_manager* pManager = (raid_world_manager*)pImp->GetWorldManager();
				pManager->ForceCloseRaid();
			}
			else if(pImp->GetWorldManager()->IsBattleWorld())
			{
				bg_world_manager * pManager = (bg_world_manager*)pImp->GetWorldManager();
				pManager->ForceClose();
			}
		}
		break;

		case 9001:
		{
			struct mma
			{
				unsigned short cmd;
				int raid_tag;
			};

			if(size != sizeof(6))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma * ccc = (mma*)buf;
			gmatrix::ReleaseRaidWorldManager(ccc->raid_tag);

		};
		break;

		case 9002:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ClearRaidCounter();
		}
		break;

		case 9003:
		{
			struct mma
			{
				unsigned short cmd;
				int result;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int result = *(int *)((char*)buf + 2);
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			if(pImp->GetWorldManager()->IsRaidWorld())
			{
				raid_world_manager* pManager = (raid_world_manager*)pImp->GetWorldManager();
				pManager->SetRaidResult(result);
			}
			
		}
		break;

		
		case 9101:
		{
			struct mma
			{
				unsigned short cmd;
				int score_a;
				int score_b;
			};

			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int score_a = *(int *)((char *)buf + 2);
			int score_b = *(int *)((char *)buf + 6);

			pImp->AddCollisionScoreA(score_a);
			pImp->AddCollisionScoreB(score_b);
			pImp->SendCollisionRaidInfo();
		}
		break;
		
		case 9102:
		{
			struct mma
			{
				unsigned short cmd;
				int win;
				int lost;
				int draw;
			};

			if(size != 14)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int win = *(int *)((char *)buf + 2);
			int lost = *(int *)((char *)buf + 6);
			int draw = *(int *)((char *)buf + 10);

			pImp->AddCollisionWin(win);
			pImp->AddCollisionLost(lost);
			pImp->AddCollisionDraw(draw);
			pImp->SendCollisionRaidInfo();
		}
		break;

		case 9103:
		{
			struct mma
			{
				unsigned short cmd;
				int collision_score;
			};
			
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int score = *(int *)((char *)buf + 2);
			pImp->ChangeCollisionScore(score);
		}
		break;

		case 9104:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			char buf[128];
			sprintf(buf, "score:%d, score_a:%d, score_b:%d, win:%d, lost:%d, draw:%d\n", pImp->GetCollisionScore(), pImp->GetCollisionScoreA(), pImp->GetCollisionScoreB(),
					pImp->GetCollisionWin(), pImp->GetCollisionLost(), pImp->GetCollisionDraw());
			pImp->Say(buf);
		}
		break;

		case 9105:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 10)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int faction = *(int *)((char *)buf + 2);
			int score = *(int *)((char *)buf + 6);
			pImp->DebugChangeCollisionResult(faction, score);
		}
		break;

		case 9106:
		{
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->ClearCollisionInfo();
		}
		break;

		case 9107:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 22)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			int type = *(int *)((char *)buf + 2);
			int level = *(int *)((char *)buf + 6);
			int score = *(int *)((char *)buf + 10);
			int team_score = *(int *)((char *)buf + 14);
			int team_rank = *(int *)((char *)buf + 18);
			int timestamp = g_timer.get_systime();
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->PlayerGetCS6V6Award(type, level, score, team_score, team_rank, timestamp);
		}
		break;

		case 9200:
		{
			struct mma
			{
				unsigned short cmd;
				int value;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int value = *(int *)((char *)buf + 2);

			pImp->GainAstrologyEnergy(value);
		}
		break;


		case 9300:
		{
			struct mma
			{
				unsigned short cmd;
				int task_id;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int task_id = *(int *)((char *)buf + 2);
			int count = 0;

			count = pImp->GetTaskFinishCount(task_id);
		}
		break;

		case 9301:
		{
			struct mma
			{
				unsigned short cmd;
				int darklight;
			};

			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->SetDarkLight(*(int*)(char*)buf + 2);
			pImp->_runner->player_darklight();
		}
		break;

		case 9400:
		{
			struct mma
			{
				unsigned short cmd;
				int type;
			};
			if(size != 6)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			int type = *(int*)((char*)buf+2);
			GNET::SendKingdomEnter(pImp->_parent->ID.id, type);
		}
		break;

		// 清除活跃度
		case 9401:
		{
			int cur_time = g_timer.get_systime();
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_liveness_info.classifyid2ids.clear();
			pImp->_liveness_info.cur_point = 0;
			memset(pImp->_liveness_info.grade_timestamp, 0, sizeof(pImp->_liveness_info.grade_timestamp));
			pImp->_liveness_info.midnight_clear_timestamp = pImp->GetNextMidNightTime(cur_time);
			pImp->_runner->liveness_notify();
		}
		break;
		// 加满活跃度
		case 9402:
		{
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			liveness_cfg* pCfg = player_template::GetLivenessCfg();
			for (std::map<int, int>::iterator it = pCfg->taskindexs.begin(); it != pCfg->taskindexs.end(); ++it)
		   	{
				pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_TASK_DONE, it->first);	
			}
			for (std::map<int, int>::iterator it = pCfg->mapindexs.begin(); it != pCfg->mapindexs.end(); ++it)
		   	{
				pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_TRANSFER_MAP, it->first);	
			}
			for (std::map<int, int>::iterator it = pCfg->specialindexs.begin(); it != pCfg->specialindexs.end(); ++it)
		   	{
				pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, it->first);	
			}
			pImp->_runner->liveness_notify();
		}
		break;
		// 零点清除活跃度
		case 9403:
		{
			int cur_time = g_timer.get_systime();
			struct mma
			{
				unsigned short cmd;
			};
			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->OnHeartbeatLiveness(pImp->GetNextMidNightTime(cur_time));
			pImp->_runner->liveness_notify();
		}
		break;

		// 调试连续活跃度天数
		case 9404:
		{
			struct mma
			{
				unsigned short cmd;
				int needday;
			};
			if(size != (2 + sizeof(int)))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			pImp->_liveness_info.debug_needday = *(int*)((char*)buf+2);
		}
		break;

		case 9500:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2) 
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			gplayer_imp* pImp = (gplayer_imp*)_imp;
			for(size_t i = 0; i < pImp->_gift_inventory.Size(); ++i)
			{
				item & it = pImp->_gift_inventory[i];
				if(it.type != -1)
				{
					char buf[128];
					sprintf(buf, "item_index=%d, item_id=%d, item_count=%d", i, it.type, it.count); 
					pImp->Say(buf);
				}
			}

		}
		break;

		case 9501:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
				size_t count;
			};

			if(size != 10 )
			{
				break;
			}

			int id = *(int*)((char*)buf+2);
			int item_count = *(int*)((char*)buf+6);
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
			gplayer_imp * pImp = (gplayer_imp *) _imp;
			if(pItem)
			{
				int rst =pImp->_gift_inventory.Push(*pItem,item_count,0);
				if(rst >= 0 && item_count == 0)
				{
					pImp->PlayerGetInventoryDetail(gplayer_imp::IL_GIFT);
				}
			}
		}
		break;

		case 9502:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2 )
			{
				break;
			}

			gplayer_imp * pImp = (gplayer_imp *) _imp;
			pImp->GetCashGiftAward();
		}
		break;

		case 9503:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->GetGiftInventory().Clear();
			pImp->PlayerGetInventoryDetail(gplayer_imp::IL_GIFT);
		}
		break;

		case 9600:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gmatrix::OpenBath();
		}
		break;

		case 9700:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ResetFuwenComposeInfo();
		}
		break;

		case 9701:
		{
			struct mma
			{
				unsigned short cmd;
			};

			if(size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->ClearMultiExp();
		}
		break;

		case 9800:
		{
			if(size != 10 )
			{
				break;
			}

			int type = *(int*)((char*)buf+2);
			int id = *(int*)((char*)buf+6);
			forbid_manager::DebugForbidInfo(type, id);
		}
		break;

		case 9801:
		{
			if(size != 2 )
			{
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SetQilinForm(true);
			pImp->ChangeShape(7);
			pImp->_runner->change_shape(7);
		}
		break;

		case 9802:
		{
			if(size != 2 )
			{
				break;
			}

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->SetQilinForm(false);
			pImp->ChangeShape(0);
			pImp->_runner->change_shape(0);
		}
		break;

		case 9900:
		{
			if(size != 6 )
			{
				break;
			}

			int new_prof = *(int*)((char*)buf+2);
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->Metempsychosis(new_prof);
		}
		break;

		case 9901:
		{
			if(size != 10 )
			{
				break;
			}
			int pet_index = *(int*)((char*)buf+2);
			int type = *(int*)((char*)buf+6);

			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerCombinePet(pet_index, type);
		}
		break;

		case 10000:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t who;
				int type;
			};
#pragma pack()

			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			mma *ccc = (mma*)buf;
			XID target;
			MAKE_ID(target,ccc->who);
			if (target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerActiveEmoteInvite(target,ccc->type);
			}
		}
		break;

		case 10001:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				size_t who;
				size_t type;
				int param;
			};
#pragma pack()

			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma *ccc = (mma*)buf;
			XID target;
			MAKE_ID(target,ccc->who);
			if (target.IsPlayer())
			{
				gplayer_imp * pImp = (gplayer_imp*)_imp;
				pImp->PlayerActiveEmoteInviteReply(target, ccc->type, ccc->param);
			}
		}
		break;

		case 10002:
		{
			if (size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->PlayerActiveEmoteCancel();
		}
		break;

		case 10003:
		{
			if (size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			pImp->GetRaidTransformTask();
		}
		break;


		case 10004:
		{
			if (size != 2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_START_MOVE,XID(GM_TYPE_NPC,pImp->GetParent()->mobactive_id),0);
		}
		break;

		case 10005:
		{
			if (size !=2)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->SendTo<0>(GM_MSG_MOB_ACTIVE_STOP_MOVE,XID(GM_TYPE_NPC,pImp->GetParent()->mobactive_id),0);
		}
		break;

		case 10006:
		{
			if (size != 18)
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}

			int travel_vehicle_id = *(int*)((char*)buf+2);
			int travel_speed  = *(int*)((char*)buf+6);
			int travel_path_id = *(int*)((char*)buf+10);
			int travel_max_time = *(int*)((char*)buf+14);

			gplayer_imp *pImp = (gplayer_imp*)_imp;
			pImp->StartTravelAround(travel_vehicle_id, (float)travel_speed, travel_path_id, travel_max_time);
		}
		break;

		case 10007:
		{

			gplayer_imp * pImp= (gplayer_imp *) _imp;
			object_interface oi(pImp);
			pImp->ClearSession();
			oi.Die();
		}
		break;

		case 10008:
		{
			struct mma
			{
				unsigned short cmd;
				size_t id;
			};
			if (size != 6)
			{
				break;
			}
			int id = *(int*)((char*)buf+2);
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
			gplayer_imp* pImp = (gplayer_imp*) _imp;
			if (pItem)
			{
				int item_count = 1;
				int rst = pImp->_task_inventory.Push(*pItem,item_count,0);
				if (rst >= 0 && item_count == 0)
				{
					pImp->_task_inventory[rst].InitFromShop(pImp,ITEM_INIT_TYPE_TASK);
					item_data_client data;
					if (pImp->_task_inventory.GetItemDataForClient(rst,data) > 0)
					{
						pImp->_runner->self_item_info(gplayer_imp::IL_TASK_INVENTORY,rst,data,0);
					}
				}
			}

		}
		break;

		case 10009:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int value;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			mma * ccc = (mma*)buf;
			TaskPairData p(0,ccc->value,0);
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			PlayerTaskInterface task_if(pImp);
			int familyid = pImp->OI_GetFamilyID();
			TaskFamilyAsyncData d(familyid,enumTaskFamilyAddCommonValue,4740,enumTaskReasonAddCommonValue,0,&p,1);
			task_if.QueryFamilyData(d.GetBuffer(),d.GetSize());

		}
		break;

		case 10010:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int id;
				int is_active;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			mma * ccc = (mma*)buf;
			pImp->SetPhase(ccc->id,ccc->is_active);

		}
		break;

		case 10011:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int id;
				int add;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp* pImp = (gplayer_imp*)_imp;
			mma * ccc = (mma*)buf;
			pImp->OI_AddProficiency(ccc->id, ccc->add);
		}
		break;

		case 10012:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int template_id;
				int time_out;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			mma * ccc = (mma*)buf;

			pImp->OI_StartTransform(ccc->template_id,1,1,ccc->time_out,2);
		}
		break;

		case 10013:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int player_id;
				int ishide;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			mma *  ccc = (mma*)buf;
			world_manager* pManager = pImp->GetWorldManager();
			pManager->DebugSetHider(ccc->player_id, ccc->ishide);

		}
		break;

		case 10014:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int score;
			};
#pragma pack()

			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			// gplayer_imp * pImp = (gplayer_imp*)_imp;
			// mma *  ccc = (mma*)buf;
			// pImp->DebugAddNewYearScore( ccc->score );
		}
		break;

		case 10015:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int player_id;
				int score;
			};
#pragma pack()

			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			mma *  ccc = (mma*)buf;
			world_manager* pManager = pImp->GetWorldManager();
			pManager->DebugSetSeekRaidScore(ccc->player_id,ccc->score);
		}
		break;

		case 10016:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int score;
			};
#pragma pack()

			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			mma *  ccc = (mma*)buf;
			pImp->DebugAddSeekRaidScore(ccc->score);
			pImp->SendHideAndSeekRaidInfo();
		}
		break;

		case 10017:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			object_interface oif(pImp);
			int money = pImp->GetMoney();
			int moneyinc = 2000000000 > money ? 2000000000 - money : 0;
			//19
			pImp->GetAchievementManager().OnMoneyInc(oif,moneyinc);
			//36
			pImp->GetAchievementManager().OnPKValueChange(oif,999);
			//76
			pImp->GetAchievementManager().OnGainScore(oif,1,99);
			//107
			pImp->GetAchievementManager().OnRefine(oif,7,8,160);
			pImp->GetAchievementManager().OnRefine(oif,7,0,160);
			//315
			pImp->GetAchievementManager().OnFlowKill(oif,1,100);
			pImp->GetAchievementManager().OnFlowKill(oif,2,30);
			pImp->GetAchievementManager().OnFlowKillPlayer(oif,100);
			pImp->GetAchievementManager().OnFlowDone(oif,1);
			pImp->GetAchievementManager().OnFlowDone(oif,2);
			pImp->GetAchievementManager().OnFlowDone(oif,3);

		}
		break;

		case 10018:
		{
#pragma pack(1)
			struct mma
			{
				unsigned short cmd;
				int map_id;
				int raid_template_id;
				int is_team;
			};
#pragma pack()
			if (size != sizeof(mma))
			{
				error_cmd(S2C::ERR_FATAL_ERR);
				break;
			}
			gplayer_imp * pImp = (gplayer_imp*)_imp;
			mma *  ccc = (mma*)buf;
			pImp->PlayerApplyCaptureRaid(ccc->map_id,ccc->raid_template_id,ccc->is_team);
		}
		break;

		default:
		__PRINTF("收到无法辨识的命令 %d\n",cmd_type);
		break;
	}
	return 0;
}

int 
gplayer_controller::cmd_user_move(const void * buf, size_t size, bool on_carrier)
{
	if(size != sizeof(C2S::CMD::player_move)) 
	{
		return 0;
	}
	C2S::CMD::player_move & cmd = (*(C2S::CMD::player_move *)buf);
	if( ((((int)cmd.cmd_seq) - _move_cmd_seq) & 0xFFFF) > 2000) 
	{
		return 0;
	}

	//检查移动命令的正确性 这个检查被推后了
	//不过可能前期做一些检查也有好处,比如飞行的切换等等
	unsigned short use_time = cmd.info.use_time;
	__PRINTF("时间:%d\n",use_time);


	//通过了验证（其实还有碰撞监测的验证），
	gplayer_imp * pImp = (gplayer_imp*)_imp;
	A3DVECTOR curpos = cmd.info.cur_pos;
	A3DVECTOR nextpos = cmd.info.next_pos;

	//建立移动的session
#ifdef __USE_OLD_MOVE_POLICY__
	session_move *pMove = new session_move(pImp); 
	pMove->SetDestination(cmd.info.speed,cmd.info.move_mode,curpos , use_time);
	pMove->SetPredictPos(nextpos);
	pMove->SetCmdSeq(cmd.cmd_seq);
	pImp->AddStartSession(pMove);
#else
	if(!g_use_old_move_policy)
	{	
		session_p_move *pMove = new session_p_move(pImp);
		pMove->SetDestination(cmd.info.speed,cmd.info.move_mode,curpos , use_time);
		pMove->SetPredictPos(nextpos);
		pMove->SetCmdSeq(cmd.cmd_seq);
		pMove->SetCarrierMode(on_carrier);
		pImp->AddStartSession(pMove);
	}
	else
	{
		session_move *pMove = new session_move(pImp); 
		pMove->SetDestination(cmd.info.speed,cmd.info.move_mode,curpos , use_time);
		pMove->SetPredictPos(nextpos);
		pMove->SetCmdSeq(cmd.cmd_seq);
		pImp->AddStartSession(pMove);
	}
#endif
	if(on_carrier)
	{
		pImp->GetParent()->rpos = cmd.info.cur_pos;
	}
	pImp->CheckSpecialMove();
	return 0;
}

int
gplayer_controller::cmd_user_stop_move(const void * buf, size_t size, bool on_carrier)
{
	if(size != sizeof(C2S::CMD::player_stop_move)) 
	{
		return 0;
	}
 	C2S::CMD::player_stop_move &cmd = (*(C2S::CMD::player_stop_move *)buf);
	if( ((((int)cmd.cmd_seq) - _move_cmd_seq) & 0xFFFF) > 2000) 
	{
		return 0;
	}

	A3DVECTOR curpos = cmd.pos;
	gplayer_imp * pImp= (gplayer_imp *) _imp;


#ifdef __USE_OLD_MOVE_POLICY__
	session_stop_move *pMove = new session_stop_move(pImp);
	pMove->SetDestination(cmd.speed,cmd.move_mode,curpos,cmd.use_time);
	pMove->SetDir(cmd.dir);
	pMove->SetCmdSeq(cmd.cmd_seq);
	pImp->AddStartSession(pMove);
#else
	if(!g_use_old_move_policy)
	{
		session_p_stop_move *pMove = new session_p_stop_move(pImp);
		pMove->SetDestination(cmd.speed,cmd.move_mode,curpos,cmd.use_time);
		pMove->SetDir(cmd.dir);
		pMove->SetCmdSeq(cmd.cmd_seq);
		pMove->SetCarrierMode(on_carrier);
		pImp->AddStartSession(pMove);
	}
	else
	{
		session_stop_move *pMove = new session_stop_move(pImp);
		pMove->SetDestination(cmd.speed,cmd.move_mode,curpos,cmd.use_time);
		pMove->SetDir(cmd.dir);
		pMove->SetCmdSeq(cmd.cmd_seq);
		pImp->AddStartSession(pMove);
	}
#endif
	if(on_carrier)
	{
		pImp->GetParent()->rpos = cmd.pos;
		pImp->GetParent()->rdir = cmd.dir;
	}
	pImp->CheckSpecialMove();
	return 0;
}

/**
 * @brief DebugTimeSegmentToSecs: 商城限时销售物品调试命令使用
 *
 * @param start_time: 格式为20110305的int型
 * @param end_time
 * @param start: 计算结果的保存变量
 * @param end
 *
 * @return 
 */
static bool DebugTimeSegmentToSecs(int start_time, int end_time, time_t &start, time_t &end)
{
	if( start_time > end_time ) return false;
	if( (start_time/100000000) || (end_time/100000000) ) return false;

	struct tm tmpstart, tmpend;
	time_t timenow;
	time(&timenow);
	localtime_r(&timenow, &tmpstart);
	memcpy(&tmpend, &tmpstart, sizeof(tm));

	tmpstart.tm_year = start_time/10000 - 1900;
	tmpstart.tm_mon  = (start_time % 10000)/100 - 1;
	tmpstart.tm_mday = start_time % 100;
	tmpstart.tm_hour = 0;
	tmpstart.tm_min  = 0;
	tmpstart.tm_sec  = 0;

	tmpend.tm_year = end_time/10000 - 1900;
	tmpend.tm_mon  = (end_time % 10000)/100 - 1;
	tmpend.tm_mday = end_time % 100;
	tmpend.tm_hour = 0;
	tmpend.tm_min  = 0;
	tmpend.tm_sec  = 0;


	//start_time,end_time格式为：20110915，精确到日
	//年的取值范围是2000 ~ 2030，时间跨度不能超过2年
	if( (tmpstart.tm_year < 100) || (tmpstart.tm_year > 130) 
	 || (tmpend.tm_year < 100) || (tmpend.tm_year > 130) 
	 || (tmpend.tm_year < tmpstart.tm_year) 
	 || (tmpend.tm_year - tmpstart.tm_year > 2) )
	{
		return false;
	}

	if( (tmpstart.tm_mon < 0) || (tmpstart.tm_mon > 11) 
	 || (tmpend.tm_mon < 0) || (tmpend.tm_mon > 11))
	{
		return false;
	}

	if( (tmpstart.tm_mday < 1) || (tmpstart.tm_mday > 31) 
	 || (tmpend.tm_mday < 1) || (tmpend.tm_mday > 31))
	{
		return false;
	}

	start = mktime(&tmpstart);
	end   = mktime(&tmpend);
	return true;
}

/**
 * @brief DebugDateTimeToSecs: 商城限时销售物品调试命令使用 
 *
 * @param date: 格式为：20110915，精确到日
 * @param exactlytime: 格式是：142159，表示14点21分59秒
 * @param setcurtime: 返回的秒数
 *
 * @return 
 */
static bool DebugDateTimeToSecs(int date, int exactlytime, time_t &setcurtime)
{
	if( (date/100000000) || (exactlytime/1000000) ) return false;

	struct tm tmpdatetime;
	time_t timenow;
	time(&timenow);
	localtime_r(&timenow, &tmpdatetime);

	tmpdatetime.tm_year = date/10000 - 1900;
	tmpdatetime.tm_mon  = (date % 10000)/100 - 1;
	tmpdatetime.tm_mday = date % 100;
	tmpdatetime.tm_hour = exactlytime/10000;
	tmpdatetime.tm_min  = (exactlytime % 10000)/100;
	tmpdatetime.tm_sec  = exactlytime % 100;


	//date格式为：20110915，精确到日
	//年的取值范围是2000 ~ 2030
	//exactlytime的格式是：142159，表示14点21分59秒
	if( (tmpdatetime.tm_year < 100) || (tmpdatetime.tm_year > 130) 
	 || (tmpdatetime.tm_mon < 0) || (tmpdatetime.tm_mon > 11) 
	 || (tmpdatetime.tm_mday < 1) || (tmpdatetime.tm_mday > 31) )
	{
		return false;
	}

	if( (tmpdatetime.tm_hour < 0) || (tmpdatetime.tm_hour > 23) 
	 || (tmpdatetime.tm_min < 0) || (tmpdatetime.tm_min > 59) 
	 || (tmpdatetime.tm_sec < 0) || (tmpdatetime.tm_sec > 59) )
	{
		return false;
	}


	setcurtime = mktime(&tmpdatetime);
	return true;
}


