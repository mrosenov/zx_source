#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>

#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include "userlogin.h"
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>
#include "trade.h"
#include "task/taskman.h"
#include "playerstall.h"
#include "pvplimit_filter.h"
#include <glog.h>
#include "pathfinding/pathfinding.h"
#include "player_mode.h"
#include "cooldowncfg.h"
#include "template/globaldataman.h"
#include "petnpc.h"
#include "item_manager.h"
#include "netmsg.h"
#include "mount_filter.h"
#include "fly_filter.h"
#include "skill_filter.h"
#include "antiwallow.h"
#include "item/item_talisman.h"
#include "item/item_magic.h"
#include "item/item_rune.h"
#include "item/item_equip.h"
#include "wallow/wallowobj.h"
#include "item/item_petbedge.h"
#include <common/chatdata.h>
#include <stocklib.h>
#include "./battleground/bg_world_manager.h"
#include "playertransform.h"
#include "invincible_filter.h"
#include "circleofdoom_filter.h"
#include "summonnpc.h"
#include <factionlib.h>
#include "consign.h"
#include "sfilterdef.h"
#include "raid/tower_raid_world_manager.h"
#include "battleflag_filter.h"
#include "kingdom_filter.h"
#include "kingdom/kingdom_bath_world_manager.h"
#include "./item/item_sale_promotion.h"
#include "./item/item_fuwen.h"
#include "./item/item_colorant.h"
#include "rage_filter.h"
#include "facbase_mall.h"
#include "azone_filter.h"
#include "travel_filter.h"
#include "./task/TaskTemplMan.h"

DEFINE_SUBSTANCE(gplayer_imp,gobject_imp,CLS_PLAYER_IMP)



void player_mallinfo::UseCash(int offset, gplayer_imp * pImp)
{
	_mall_cash_offset -= offset;
	pImp->OnUseCash(offset);
}

void player_mallinfo::AddWebOrder(const std::vector<GMSV::weborder> & weborders, gplayer_imp * pImp)
{
	for(size_t i = 0; i < weborders.size(); ++i)
	{
		web_order w_order;
		w_order._order_id = weborders[i].orderid;
		w_order._userid = weborders[i].userid;
		w_order._roleid = weborders[i].roleid;
		w_order._pay_type = weborders[i].paytype;
		w_order._status = weborders[i].status;
		w_order._timestamp = weborders[i].timestamp;

		for(size_t j = 0; j < weborders[i].funcs.size(); ++j)
		{
			const GMSV::webmallfunc & w_mall_func = weborders[i].funcs[j];
			web_order_package w_order_package;
			w_order_package._package_id = w_mall_func.id;
			w_order_package._name_len = w_mall_func.name.length();
			if(w_order_package._name_len > 32)  w_order_package._name_len = 32;
			memcpy(w_order_package._package_name,w_mall_func.name.c_str(),  w_order_package._name_len);
			w_order_package._count = w_mall_func.count;
			w_order_package._price = w_mall_func.price;

			for(size_t k = 0; k < w_mall_func.goods.size(); ++k)
			{
				const GMSV::webmallgoods & w_goods = w_mall_func.goods[k];
				web_order_goods goods;
				goods._goods_id = w_goods.id;
				goods._goods_count = w_goods.count;
				goods._goods_flag = w_goods.flagmask;
				goods._goods_time = w_goods.timelimit;

				w_order_package._order_goods.push_back(goods);
			}
			w_order._order_package.push_back(w_order_package);
		}
		_web_order_list.push_back(w_order);
		GLog::log(GLOG_INFO,"用户%d从Web商城购买物品, order_id=%lld, user_id=%d, role_id=%d, pay_type=%d, status=%d, timestamp=%d",
				pImp->_parent->ID.id, w_order._order_id, w_order._userid, w_order._roleid, w_order._pay_type, w_order._status, w_order._timestamp);
	}
	CalcWebCash();

}


void TrySwapPlayerData(world * pPlane,const int cid[3],gplayer * pPlayer)
{
	gplayer_imp * imp = (gplayer_imp*) pPlayer->imp;
	gplayer_controller * ctrl = (gplayer_controller *)imp->_commander;
	gplayer_dispatcher * dis = (gplayer_dispatcher*)imp->_runner;
	if(imp->GetGUID() == cid[0] &&
			ctrl->GetGUID() == cid[1] &&
			dis->GetGUID() == cid[2])
	{
		//和原来的类完全一致，无须处理
		return ;
	}

	__PRINTF("开始进行玩家的数据置换\n");
	//重新生成 前面应该已经验证过cid的正确性了
	gplayer_imp * new_imp = (gplayer_imp *)CF_Create(cid[0],cid[2],cid[1],pPlane,pPlayer);
	ASSERT(new_imp);
//	gplayer_controller * new_ctrl =(gplayer_controller*)new_imp->_commander; 
//	gplayer_dispatcher * new_dis = (gplayer_dispatcher*)new_imp->_runner;
	
	new_imp->Swap(imp);
//	new_ctrl->LoadFrom(ctrl); 在里面已经Swap过了
//	new_dis->LoadFrom(dis);

	pPlayer->imp = new_imp;
	delete  imp;
	delete  dis;
	delete  ctrl;
	return ;
}

gplayer_imp::gplayer_imp()
	:_inventory(item::INVENTORY,ITEM_LIST_BASE_SIZE),
	_equipment(item::BODY,item::EQUIP_INVENTORY_COUNT),
	_task_inventory(item::TASK_INVENTORY,TASKITEM_LIST_SIZE),
	_pet_bedge_inventory(item::PET_BEDGE_INVENTORY,INIT_PET_BEDGE_LIST_SIZE),
	_pet_equip_inventory(item::PET_EQUIP_INVENTORY,PET_EQUIP_LIST_SIZE),
	_pocket_inventory(item::POCKET_INVENTORY,POCKET_INVENTORY_BASE_SIZE),
	_fashion_inventory(item::FASHION_INVENTORY,FASHION_INVENTORY_SIZE),
	_mount_wing_inventory(item::MOUNT_WING_INVENTORY,MOUNT_WING_INVENTORY_BASE_SIZE),
	_gift_inventory(item::GIFT_INVENTORY,GIFT_INVENTORY_SIZE),
	_fuwen_inventory(item::FUWEN_INVENTORY,FUWEN_INVENTORY_SIZE),
	_player_money(0),_combat_timer(0)
{
	// Youshuang add
	_fashion_weapon_addon_id = -1;
	_faction_coupon = 0;
	_fac_coupon_add = 0;
	_fac_coupon_added = false;
	memset(&_newyear_info,0,sizeof(_newyear_info));
	_used_title = 0;
	// end
	memset(&_instance_switch_key,0,sizeof(_instance_switch_key));
	_battle_faction = 0;
	_battle_score = 0;
	_killingfield_score = 0;
	_arena_score = 0;
	_raid_faction = 0;
	_inventory.SetOwner(this);
	_equipment.SetOwner(this);
	_task_inventory.SetOwner(this);
	_trashbox.SetOwner(this);
	_pet_bedge_inventory.SetOwner(this);
	_pet_equip_inventory.SetOwner(this);
	_pocket_inventory.SetOwner(this);
	_fashion_inventory.SetOwner(this);
	_mount_wing_inventory.SetOwner(this);
	_gift_inventory.SetOwner(this);
	_fuwen_inventory.SetOwner(this);
	_provider.id = XID(-1,-1);

	_disconnect_timeout = 0;
	_offline_type = PLAYER_OFF_LOGOUT;

	_inv_level = 0;
	_money_capacity = MONEY_CAPACITY_BASE;
	_faction = 0;
	_enemy_faction = 0;
	_trade_obj = NULL;
	_stall_obj = NULL;
	_consign_obj = NULL;
	_transform_obj = NULL;
	_stall_trade_timer = 0;
	_stall_trade_id = g_timer.get_systime();
	_write_timer = 513;
	_general_timeout = 0;
	_task_mask = 0;
	_link_notify_timer  = LINK_NOTIFY_TIMER;
	_pvp_flag_enable = false;
	_eq_change_counter = 1;		//默认认为装备栏发生了变化 不需存盘
	_kill_by_player = false;
	_free_pvp_mode = false;
	_resurrect_state = false;
	_resurrect_exp_reduce = 0.f;
	_resurrect_hp_recover = 0.1f;
	_resurrect_timestamp = 0;
	_last_move_mode = 0;
	_logout_pos_flag = 0;
	_last_instance_tag = -1;
	_last_instance_timestamp = 0;
	_last_source_instance_tag = -1;
	_db_save_error = 0;
	memset(&move_checker,0,sizeof(move_checker));
	_username_len = 0;
	_pvp_combat_timer = 0;
	_login_timestamp = 0;
	_played_time = 0;
	_spec_task_reward = 0;
	_spec_task_reward_param = 0;
	_fill_info_mask = 0;
	_duel_target = 0;
	_idle_player_state = 0;
	_item_poppet_counter = 0;
	_exp_poppet_counter = 0;

	_enemy_list.reserve(MAX_PLAYER_ENEMY_COUNT);

	_active_task_list.insert(_active_task_list.begin(),TASK_ACTIVE_LIST_BUF_SIZE,0);
	_finished_task_list.insert(_finished_task_list.begin(),TASK_FINISHED_LIST_BUF_SIZE,0);
	_finished_time_task_list.insert(_finished_time_task_list.begin(),TASK_FINISH_TIME_LIST_BUF_SIZE,0);

	_speed_ctrl_factor = 16.0f;	//给个查不多的初值即可
	_produce_level = 0;
	_produce_exp = 0;
	_is_moved = false;
	_mafia_contribution = 0;
	_mafia_join_time = g_timer.get_systime();
	_family_contribution = 0;
	_recorder_timestamp = 0;
	_another_day_timestamp = 0;
	_logout_time = 0;
	_create_time = 0;
	_invader_counter = 0;
	_sanctuary_mode = 0;
	memset(_region_reputation, 0 , sizeof(_region_reputation));
	_wallow_level = 0;
	_cheat_punish = 0;
	_cheat_mode = 0;
	_cheat_report = 0;
	_chat_emote = 0;
	_talent_point = 0;
	_db_magic_number = 0;
	_ip_address = 0;
	_sect_id = 0;
	_sect_init_level = 0;
	_cultivation = 0;
	_offline_agent_time = 0;
	_offline_agent_active = 0;
	_last_db_save_time = 0;
	_referrer_id = 0;
	_pvp_mask = 0;
	_achieve_timer = 0;
	memset(_username, 0, sizeof(_username));
	memset(&_fly_info, 0, sizeof(_fly_info));
	memset(&_talisman_info,0,sizeof(_talisman_info));
	memset(&_vipstate, 0, sizeof(_vipstate));
	memset (&_achieve_cache, 0, sizeof(_achieve_cache));
	memset (&_combo_skill, 0, sizeof(_combo_skill));
	memset(&_treasure_info, 0, sizeof(_treasure_info));
	memset(&_little_pet, 0, sizeof(_little_pet));
	memset(&_fuwen_compose_info, 0, sizeof(_fuwen_compose_info));
	memset(&_item_multi_exp, 0, sizeof(_item_multi_exp));
	memset(&_fac_multi_exp, 0, sizeof(_fac_multi_exp));
	_time_raid_counters_reset = 0;

	id_spirit_drag_me = -1;
	id_pulling_me = -1;
	SetConsignRoleFlag(false);
	_annu_five_star = 0;
	_time_annu_five_award = 0;
	_annu_sign_state = 0;
	_active_rune = 0;
	_rune_score = 0;
	_active_rune_level = 0;
	_flag_mask = 0;

	memset(_pk_bets, 0, sizeof(_pk_bets));
	_1st_bet = false;
	_top3_bet = false;
	_1st_bet_reward_deliveried = false;
	_1st_bet_reward_result_deliveried = false;
	_top3_bet_reward_deliveried = false;
	memset(_top3_bet_reward_result_deliveried, 0, sizeof(_top3_bet_reward_result_deliveried));

	_consumption_value = 0;

	_dark_light = 0;
	_dark_light_form = 0;
	memset(&_astrology_info, 0, sizeof(_astrology_info));
	memset(&_collision_info, 0, sizeof(_collision_info));

	_check_buffarea_time = 0;
	memset(_prop_add, 0, sizeof(_prop_add));
	_kingdom_title = 0;
	_puppet_immune_prob = 0;
	_puppet_skill_id = 0;
	_no_check_move = false;
	_mafia_name_len = 0;
	memset(_mafia_name,0,sizeof(_mafia_name));
	memset(&_hide_and_seek_info,0,sizeof(_hide_and_seek_info));
}

gplayer_imp::~gplayer_imp()
{
	if(_trade_obj)
	{
		//ASSERT(false && "交易对象应该提前释放的");
		delete _trade_obj;
	}
	if(_stall_obj)
	{
		//ASSERT(false && "摆摊对象应该提前释放的");
		delete _stall_obj;
	}
	if(_transform_obj)
	{
		delete _transform_obj;
		_transform_obj = NULL;
	}
	if(_consign_obj)
	{
		delete _consign_obj;
		_consign_obj = NULL;
	}

	//删除临时的物品回购包裹
	std::list<item>::iterator it_inv = _repurchase_inventory.begin();
	while(it_inv != _repurchase_inventory.end())
	{
		(*it_inv).Release();
		++it_inv;
	}

	ClearSpiritSession();
}

void 
gplayer_imp::Init(world * pPlane,gobject*parent)
{
	gactive_imp::Init(pPlane,parent);
	_team.Init(this);
}

void 
gplayer_imp::PlayerEnterWorld()
{
        if(gmatrix::IsZoneServer())
        {
		gplayer* pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);
	}
	//做进入世界前的最后初始化

	//激活转生效果
	ActiveRebornBonus(true);


	//回收战场里面的物品，避免gs当机的时候这些物品带入普通世界
	RecycleBattleItem();
	
	//使得装备和符文生效
	RefreshPropInventory();
	//生成装备的数据（供外人看）
	CalcEquipmentInfo();

	//发送进入世界消息和取得世界的数据
	EnterWorld();
	
	if(_layer_ctrl.IsFalling())
	{
		//如果处于下落状态，考虑开始跌落
		//现在不再发送跌落数据，而是由客户端来完成此操作。
	}

	//测试是否死亡且可以复活
	if(_parent->IsZombie() && _resurrect_state)
	{
		_runner->enable_resurrect_state(_resurrect_exp_reduce);
	}

	//测试是否在安全区
	TestSanctuary();
	_runner->server_config_data();

	_ph_control.Initialize(this);

	//飞升开放160等级后, 原来的飞升148-150等级升级所需要的经验降低,这里检查玩家的经验是否满足升级条件，如果满足直接升级
	//原来飞升150的给与一定的补偿
	TryFixRebornLevel();
	
	//检查离线托管状态
	if(_offline_agent_active)
	{
		int offline_time = (g_timer.get_systime() - _last_db_save_time) / 60;
		if(offline_time > 0) 
		{
			//计算有效的离线托管时间
			int valid_offline_time = offline_time;
			if(valid_offline_time > _offline_agent_time) valid_offline_time = _offline_agent_time;
			_offline_agent_time  -= valid_offline_time;

			//计算合适的经验值
			int64_t exp = GetAgentExpBonus(_basic.level, GetRebornCount(), true);
			exp *= valid_offline_time;
			IncExp(exp);
			if(exp > 0)
			{
				GLog::log(GLOG_INFO,"用户%d得到离线托管经验%lld(level:%dtime:%dmin)，剩余可用托管时间%d",_parent->ID.id,exp, _basic.level, valid_offline_time, _offline_agent_time);
				GLog::formatlog("formatlog:offline_agent_bonus:userid=%d:exp=%lld:level=%d:time=%d:timeleft=%d",_parent->ID.id,exp, _basic.level, valid_offline_time, _offline_agent_time);
				_runner->offline_agent_bonus(valid_offline_time, _offline_agent_time, exp);
			}
			else
			{
				GLog::log(GLOG_INFO,"用户%d得到离线托管经验%lld(level:%dtime:%dmin)，剩余可用托管时间%d",_parent->ID.id,exp, _basic.level, valid_offline_time, _offline_agent_time);
			}

		}
		_offline_agent_active = 0;
	}

	//进行长时间离线奖励措施
	int offline_time = g_timer.get_systime() - _last_db_save_time;
	if( _basic.level > 15 && offline_time > 24*3600)
	{
		offline_time /= 24*3600;
		if(g_config.long_offline_bonus & 0x01) 	//一类
		{
			const int  LIST_COUNT = 12;
			int time_list[LIST_COUNT] = { -65535, 0,   10,   20,   30,   40,   50,   60,   70,   80,   90, 180};
			int task_list[LIST_COUNT] = { -65535, 0,   12951,12952,12953,12954,12955,12956,12957,12958,12959,14545};
			int spec_task = 12960;  //特殊奖励， 150级且30天以上未登录游戏
			int * p = std::lower_bound(time_list, time_list +LIST_COUNT, offline_time);
			if( p == time_list + LIST_COUNT) 
				p = &time_list[LIST_COUNT - 1]; 
			else if( *p != offline_time) 	//lower_bound返回下一个符合的位置，所以应该减一
				p --;
			int index = p - time_list;
			int task_id = task_list[index];
			if(task_id > 0)
			{
				PlayerTaskInterface  task_if(this);
				OnTaskCheckDeliver(&task_if,task_id,0);
				if(offline_time >= 30 && _basic.level >= 150)
				{
					OnTaskCheckDeliver(&task_if,spec_task,0);
				}
			}
			GLog::log(GLOG_INFO,"用户%d得到长时间离开奖励%d,%d", _parent->ID.id, task_id, (offline_time >= 30 && _basic.level >= 150)?spec_task:0);
		}

		if(g_config.long_offline_bonus & 0x02) 
		{
			PlayerTaskInterface  task_if(this);
			if(offline_time >= 30)
			{
				OnTaskCheckDeliver(&task_if,17606,0);
				GLog::log(GLOG_INFO,"用户%d得到二类长时间离开奖励%d", _parent->ID.id, 12961);
			}
		}
	}

	_logout_time = _last_db_save_time; 

	//查询网吧奖励
	GMSV::QueryNetbarReward(_parent->ID.id); 

	GMSV::SendQueryBattleFlagBuff(_parent->ID.id, gmatrix::GetServerIndex(), GetWorldManager()->GetWorldTag(), GetParent()->id_mafia);
	
	//加入上线保护
	session_logon_invincible *pSession = new session_logon_invincible(this, 30);
	AddStartSession(pSession);

	_skill.EventAfterEnterWorld(this);	

	// 上线检查更新活跃度VIP卡状态
	if(((gplayer *)_parent)->IsVIPState()) {
		_runner->liveness_notify();
		EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_VIP_CARD_OPEN);
	}

	_runner->sale_promotion_info(_sale_promotion_info.taskids);
	
	// 更新活跃度
	/*liuyue-facbase
	if (GetWorldManager()->IsRaidWorld()) {
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TRANSFER_MAP, GetWorldManager()->GetRaidID());
	} else {
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TRANSFER_MAP, GetWorldManager()->GetWorldTag());
	}
	*/	
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TRANSFER_MAP, GetClientTag());

        int gmtoff = gmatrix::Instance()->GetServerGMTOff();
	int cur_time = g_timer.get_systime();
	int diff_days = (cur_time + gmtoff)/86400 - (_last_db_save_time + gmtoff)/86400;
	//和上次登录不是同一天
	if(diff_days > 0)
	{
		_fuwen_compose_info.compose_count = 0;
		_fuwen_compose_info.extra_compose_count = 0;
	}

	// 获得凌晨12点的事件戳
	_another_day_timestamp = cur_time + 86400 - (cur_time + gmtoff)%86400;	

	_filters.AddFilter(new rage_filter(this,FILTER_INDEX_RAGE));

}



int64_t gplayer_imp::GetAgentExpBonus(int level, int reborn_count, bool is_offline)
{
	double k1,m1,k2,m2;
	if(level < 60)
	{
		k1=1;
		k2 =0;
	}
	else
	{
		k1 = 0;
		k2 = 1;
	}
	if(is_offline)
	{
		m1 = 0.3*(1 + 0.5*reborn_count);
		m2 = 0.3*(1 + 1.5*reborn_count);
	}
	else
	{
		m1 = 0.1f*(1 + 0.5*reborn_count);
		m2 = 0.1f*(1 + 1.5*reborn_count);
	}

	return (int64_t)(k1*m1*(1.8*level*level + 43.3*level + 246) + k2*m2*(43.4*level*level - 5333.3*level + 174167));
}

int 
gplayer_imp::DispatchMessage(const MSG & msg)
{
	gplayer * pPlayer = (gplayer *)_parent;
	int rst = 0;
	switch(pPlayer->login_state)
	{

		case gplayer::WAITING_ENTER:
			rst = WaitEnterHandler(msg);
			break;

		case gplayer::DISCONNECTED:
		case gplayer::LOGIN_OK:
			rst = _pstate.DispatchMessage(this,msg);
			break;
		default:
		//其它状态不处理任何消息
			rst = 0;
			break;
	}
	return rst;
}

int 
gplayer_imp::WaitEnterHandler(const MSG & msg)
{
	if(msg.message == GM_MSG_ENTER_WORLD)
	{
		_general_timeout = 0; 
		((gplayer *)_parent)->login_state = gplayer::LOGIN_OK;
		PlayerEnterWorld();

		int locktime = msg.param;
		int maxlocktime = msg.param2;;
		//记录锁定时间
		gplayer_controller * pCtrl = (gplayer_controller *) _commander;
		pCtrl->SetSafeLock(locktime, maxlocktime);
	}
	else if(msg.message == GM_MSG_HEARTBEAT)
	{
		_general_timeout ++;
		if(_general_timeout > 600)
		{
			//WAITING_ENTER 10 分钟超时
			int cs_index = ((gplayer*)_parent)->cs_index;
			int uid = ((gplayer*)_parent)->ID.id;
			int sid = ((gplayer*)_parent)->cs_sid;
			_commander->Release();
			GMSV::SendDisconnect(cs_index,uid,sid,0);
		}
	}
	return 0;
}

int 
gplayer_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
		{
			ASSERT(msg.content_length >= sizeof(attack_msg));

			attack_msg ack_msg = *(attack_msg*)msg.content;
			//处理一下到来的攻击消息
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
			
			if(!TestAttackMsg(msg,ack_msg)) return 0;
			if(!(ack_msg.attacker_mode & attack_msg::PVP_DURATION) &&
				(ack_msg.is_invader || (ack_msg.attacker_mode & attack_msg::PVP_MAFIA_DUEL)
				|| ( ack_msg.attacker_mode & attack_msg::PVP_HOSTILE_DUEL ) ))
			{
				SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,ack_msg.ainfo.attacker,0);
			}

			//试着选择对象
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(msg,&ack_msg);
		}
		return 0;

		case GM_MSG_ENCHANT:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);

			//空中不能给地面的玩家加祝福
			if(ech_msg.helpful)
			{
				if(ech_msg.ainfo.attacker.IsPlayerClass() && ech_msg.is_flying != GetParent()->IsFlyMode()) return false;
			}

			if(!ech_msg.helpful)
			{
				if(!TestHarmfulEnchant(msg,ech_msg)) return 0;
				if(!(ech_msg.attacker_mode & attack_msg::PVP_DURATION) &&
						(ech_msg.is_invader || (ech_msg.attacker_mode & attack_msg::PVP_MAFIA_DUEL)
						|| ( ech_msg.attacker_mode & attack_msg::PVP_HOSTILE_DUEL ) ))
				{
					SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,ech_msg.ainfo.attacker,0);
				}
			}
			else
			{
				if(!TestHelpfulEnchant(msg,ech_msg)) return 0;
				//如果对方并非PVP状态，并且自己处于PVP状态，则发一个消息让对方成为PVP状态
				if(!(ech_msg.attacker_mode & attack_msg::PVP_DURATION) && IsInPVPCombatState())
				{
					SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,ech_msg.ainfo.attacker,0);
				}
			}
			HandleEnchantMsg(msg,&ech_msg);
		}
		return 0;

		case GM_MSG_ENCHANT_ZOMBIE:
		{
			if(IsDead())
			{
				__PRINTF("recv zombie enchant\n");
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				if(!ech_msg.helpful)
				{
					return 0;
				}
				else
				{
					if(!TestHelpfulEnchant(msg,ech_msg)) return 0;
				}
				HandleEnchantMsg(msg,&ech_msg);
			}
		}
		return 0;

		case GM_MSG_ERROR_MESSAGE:
			_runner->error_message(msg.param);
			return 0;

		case GM_MSG_RECEIVE_MONEY:
			OnPickupRawMoney(msg.param,true);
		return 0;

		case GM_MSG_MONSTER_MONEY:
			OnPickupRawMoney(msg.param,false);
		return 0;

		case GM_MSG_MONSTER_GROUP_MONEY:
			if(msg.param > 0)
			{
				if(msg.param > MONEY_CAPACITY_BASE)
				{
					GLog::log(GLOG_ERR, "收到错误的怪物金钱%d", msg.param);
					return 0;
				}

				if(!_team.PickupTeamMoney(msg.param2, msg.pos,msg.param))
				{
					//队伍分发失败 所以算自己的
					SendTo<0>(GM_MSG_RECEIVE_MONEY, GetParent()->ID, msg.param);
				}
			}
			return 0;

		case GM_MSG_TEAM_EXPERIENCE:
			if(msg.content_length == sizeof(msg_grp_exp_t)) 
			{
				if(msg.pos.squared_distance(_parent->pos) <= (TEAM_EXP_DISTANCE * TEAM_EXP_DISTANCE))
				{
					msg_grp_exp_t * pExp = (msg_grp_exp_t*)msg.content;
					
					int64_t exp = pExp->exp;
					if(gmatrix::AntiWallow())
					{
						anti_wallow::AdjustNormalExp(_wallow_level, exp);
					}
					ReceiveExp(exp);
					if(pExp->level > 0)
					{
						//是组队杀得怪物,调用任务接口 
						if(_task_mask & TASK_MASK_KILL_MONSTER)
						{
							PlayerTaskInterface  task_if(this);
							OnTaskTeamKillMonster(&task_if,msg.param, pExp->level,pExp->rand);
						}
						object_interface oif(this);
						GetAchievementManager().OnKillMonster(oif, msg.param);
						DeliverTopicSite( new TOPIC_SITE::kill_monster( msg.param ) );  // Youshuang add
					}
				}
			}
			else
			{
				ASSERT(false && "经验值消息的内容大小不正确");
			}
			return 0;
		case GM_MSG_EXPERIENCE:
			if(msg.content_length == sizeof(msg_exp_t))
			{
				if(msg.pos.squared_distance(_parent->pos) <= (NORMAL_EXP_DISTANCE * NORMAL_EXP_DISTANCE))
				{
					ReceiveExp(*(msg_exp_t*)msg.content);
				}
			}
			else
			{
				ASSERT(false && "经验值消息的内容大小不正确");
			}
			return 0;
		case GM_MSG_GROUP_EXPERIENCE:
		{
			ASSERT(msg.content_length && ((msg.content_length-sizeof(msg_grpexp_info)) % sizeof(msg_grpexp_t)) == 0);

			int count = ((msg.content_length-sizeof(msg_grpexp_info)) / sizeof(msg_grpexp_t));
			msg_grpexp_info * pExp = (msg_grpexp_info*)msg.content;
			const msg_grpexp_t * list = (msg_grpexp_t*)((char*)msg.content + sizeof(msg_grpexp_info));

			int64_t exp = pExp->exp;
			int64_t team_damage = pExp->team_damage;
			int team_seq = pExp->team_seq;
			int npc_level = pExp->npc_level;
			int npc_tag = pExp->npc_tag;
			int npc_id = pExp->npc_id;
			float r = pExp->r;
			bool no_exp_punish = pExp->no_exp_punish;

			if(_team.IsLeader() && _team.GetTeamSeq() == team_seq)
			{
				ASSERT(count >= 1);
				ReceiveGroupExp(msg.pos,team_damage,exp,npc_level,count, npc_id,  npc_tag, r, list,  no_exp_punish);
			}
		}
		return 0;

		case GM_MSG_PICKUP_TEAM_ITEM:
			ASSERT(msg.content && msg.content_length);
			if(_team.IsLeader())
			{
				//进行物品分发
				XID who = _team.RollItem(msg.pos,msg.param);
				if(who.IsObject())
				{
					MSG newmsg;
					BuildMessage(newmsg,GM_MSG_PICKUP_ITEM,who,msg.source,msg.pos,
							0x80000000,msg.content,msg.content_length);
					gmatrix::SendMessage(newmsg);
				}
			}
			else
			{
				OnPickupItem(msg.pos,msg.content,msg.content_length,msg.param&0x80000000,msg.param&0x7FFFFFFF);
			}
			return 0;

		case GM_MSG_HATE_YOU:
			ActiveCombatState(true);
			if(_combat_timer < NORMAL_COMBAT_TIME)
			{
				_combat_timer = NORMAL_COMBAT_TIME;
			}
			((gplayer_controller*)_commander)->TrySelect(msg.source);

			if(_enemy_list.size() < MAX_PLAYER_ENEMY_COUNT)
			{
				_enemy_list[msg.source.id] ++;
			}
			else
			{
				ENEMY_LIST::iterator it = _enemy_list.find(msg.source.id);
				if(it != _enemy_list.end())
				{
					it->second ++;
				}
			}

			return 0;


		case GM_MSG_TEAM_INVITE:
		{
			if(GetWorldManager()->GetWorldLimit().faction_team && (msg.param2 & GetEnemyFaction())) return 0;

			gplayer* pPlayer = GetParent();
			if((pPlayer->IsBattleOffense() && !(msg.param2 & FACTION_BATTLEOFFENSE)) 
			  ||(!(pPlayer->IsBattleOffense()) && (msg.param2 & FACTION_BATTLEOFFENSE)) 
			  ||(pPlayer->IsBattleDefence() && !(msg.param2 & FACTION_BATTLEDEFENCE))
			  ||(!(pPlayer->IsBattleDefence()) && (msg.param2 & FACTION_BATTLEDEFENCE)))
			{
				return 0;
			}

			ASSERT(msg.content_length == sizeof(msg_team_invite));
			const msg_team_invite * pMsg = (const msg_team_invite*)msg.content;
			if(!_team.MsgInvite(msg.source,msg.param,pMsg->pickup_flag, pMsg->family_id, pMsg->mafia_id, pMsg->level, pMsg->sectid, pMsg->referid))
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_TEAM_CANNOT_INVITE);

			}
		}
		return 0;

		case GM_MSG_TEAM_AGREE_INVITE:
			ASSERT(msg.content_length == sizeof(player_team::agree_invite_entry));
			if(msg.content_length == sizeof(player_team::agree_invite_entry))
			{
				_team.MsgAgreeInvite(msg.source,msg.pos,*(player_team::agree_invite_entry*)msg.content,msg.param);
			}
			return 0;

		case GM_MSG_TEAM_REJECT_INVITE:
			_team.MsgRejectInvite(msg.source);
			return 0;

		case GM_MSG_JOIN_TEAM:
			{
				int count = msg.param & 0x7FFF;
				int pickup_flag = (msg.param >> 16 ) & 0x7FFF;
				size_t header_size = count * sizeof(player_team::member_entry);
				ASSERT(msg.content_length == header_size + _cur_tag_counter.size()*sizeof(int)*3);
				if(msg.content_length == header_size +  _cur_tag_counter.size()*sizeof(int)*3)
				{
					
					void * buf = ((char*)msg.content) + header_size;
					size_t size = msg.content_length - header_size;
					_team.MsgJoinTeam(msg.source,(player_team::member_entry*)msg.content,
							count,pickup_flag,buf,size,msg.param2);
				}
			}
			return 0;
		case GM_MSG_LEADER_UPDATE_MEMBER:
			ASSERT(msg.content_length == msg.param * sizeof(player_team::member_entry));
			if(msg.content_length == msg.param * sizeof(player_team::member_entry))
			{
				_team.MsgLeaderUpdateData(msg.source,
						(player_team::member_entry*)msg.content,msg.param);
			}
			return 0;

		case GM_MSG_JOIN_TEAM_FAILED:
			_team.MsgJoinTeamFailed(msg.source);
			return 0;

		case GM_MSG_MEMBER_NOTIFY_DATA:
			ASSERT(msg.content_length == sizeof(team_mutable_prop));
			if(msg.content_length == sizeof(team_mutable_prop))
			{
				_team.MsgMemberUpdateData(msg.source,msg.pos,*(team_mutable_prop*)msg.content);
			}
			return 0;
			
		case GM_MSG_NEW_MEMBER:
			ASSERT(msg.content_length == msg.param * sizeof(player_team::member_entry));
			if(msg.content_length == msg.param * sizeof(player_team::member_entry))
			{
				_team.MsgNewMember(msg.source,(player_team::member_entry*)msg.content,msg.param);
			}
			return 0;

		case GM_MSG_LEAVE_PARTY_REQUEST:
			_team.MsgMemberLeaveRequest(msg.source);
			return 0;

		case GM_MSG_LEADER_CANCEL_PARTY:
			_team.MsgLeaderCancelParty(msg.source,msg.param);
			return 0;

		case GM_MSG_MEMBER_NOT_IN_TEAM:
			_team.MsgNotifyMemberLeave(msg.source,_parent->ID,0);
			return 0;

		case GM_MSG_LEADER_KICK_MEMBER:
			_team.MsgLeaderKickMember(msg.source,XID(GM_TYPE_PLAYER,msg.param));
			return 0;

		case GM_MSG_MEMBER_LEAVE:
			_team.MsgNotifyMemberLeave(msg.source,XID(GM_TYPE_PLAYER,msg.param));
			return 0;
/*	现在已经没有使用了
		case GM_MSG_GET_MEMBER_POS:
			_runner->teammate_get_pos(msg.source,*(int*)(msg.content), msg.param);
			return 0;
 */

		case GM_MSG_QUERY_PLAYER_EQUIPMENT:
			{
				float ox = msg.pos.x - _parent->pos.x;
				float oz = msg.pos.z - _parent->pos.z;
				if(ox*ox + oz*oz <= GET_EQUIP_INFO_DIS*GET_EQUIP_INFO_DIS)
				{
					_runner->send_equipment_info(msg.source,*(int*)(msg.content), msg.param);
				}
			}
			return 0;
		
		case GM_MSG_QUERY_CLONE_EQUIPMENT:
			{
				msg_get_clone_equipment mce = *(msg_get_clone_equipment*)msg.content;
				int clone_id = msg.param;
				if(GetCloneID().id == clone_id) 
				{
					_runner->send_clone_equipment_info(msg.source, clone_id, mce.cs_index, mce.cs_sid);
				}
				
			}
			return 0;

		case  GM_MSG_TEAM_PICKUP_NOTIFY:
			{
				_runner->team_member_pickup(msg.source,msg.param,*(int*)msg.content);
			}
			return 0;
		case  GM_MSG_SERVICE_GREETING:
			_provider.id = msg.source;
			_provider.pos = msg.pos;
			_provider.id_mafia = msg.param;
			_runner->npc_greeting(msg.source);
			return 0;

		case  GM_MSG_SERVICE_DATA:
		{
			ASSERT(!_parent->IsZombie() && "不在死亡状态接受任何服务");
			int service_type = msg.param;
			service_executor *executor = service_manager::GetExecutor(service_type);
			if(executor)
			{
				executor->Serve(this,msg.source,msg.pos,msg.content,msg.content_length);
			}
		}
		return 0;
		
		case  GM_MSG_NPC_BE_KILLED:
		{
			//处理红名减少的问题 
			ASSERT(msg.content_length == sizeof(int));
			int level = *(int*)msg.content;

			//得到了杀死npc的消息
			if(_task_mask & TASK_MASK_KILL_MONSTER)
			{
				//调用任务系统的操作
				PlayerTaskInterface  task_if(this);
				OnTaskKillMonster(&task_if,msg.param, level,abase::RandUniform());
			}
			_petman.KillMob(this,level);
			if(!IsInTeam())
			{
				object_interface oif(this);
				GetAchievementManager().OnKillMonster(oif, msg.param);
				DeliverTopicSite( new TOPIC_SITE::kill_monster( msg.param ) );  // Youshuang add
			}
		}
		return 0;

		case  GM_MSG_TASK_SHARE_NPC_BE_KILLED:
		{
			//处理红名减少的问题 
			ASSERT(msg.content_length == sizeof(int));
			int level = *(int*)msg.content;

			//得到了杀死npc的消息
			if(_task_mask & TASK_MASK_KILL_MONSTER)
			{
				//调用任务系统的操作
				PlayerTaskInterface  task_if(this);
				OnTaskShareKillMonster(&task_if,msg.param, level,abase::RandUniform());
			}
			_petman.KillMob(this,level);
			if(!IsInTeam())
			{
				object_interface oif(this);
				GetAchievementManager().OnKillMonster(oif, msg.param);
				DeliverTopicSite( new TOPIC_SITE::kill_monster( msg.param ) );  // Youshuang add
			}
		}
		return 0;

		case GM_MSG_PLAYER_TASK_TRANSFER:
		{
			__PRINTF("手到其他玩家传来的任务信息\n");
			PlayerTaskInterface  task_if(this);
			OnPlayerNotify(&task_if,msg.source.id,msg.content,msg.content_length);
		}
		return 0;

		case GM_MSG_PLAYER_KILL_PLAYER:
		{
			if(msg.content_length != sizeof(msg_player_kill_player))
			{
				ASSERT(false);
				return 0;
			}
			const msg_player_kill_player * pMsg = (const msg_player_kill_player *) msg.content;
			if(pMsg->noDrop && _mduel.InBattle() && _mduel.IsEnemy(pMsg->mafia_id))
			{
				//帮战
				_mduel.KillEnemy();
				return 0;
			}
			
			if(pMsg->bpKilled)
			{	//需要进行PK值等等的计算
				int tpv = pMsg->pkvalue;
				if(tpv <= 0)
				{
					//杀死一个白名
					ModifyPKValue(1);
					_runner->pk_value_notify(GetPKValue());
				}
			}

		}
		return 0;

		case GM_MSG_PLAYER_KILL_PET:
		{
			if(_mduel.InBattle())
			{
				if(_mduel.IsEnemy(msg.param)) return 0;
			}
			//杀死一个白名人的宠物
			ModifyPKValue(1);
			_runner->pk_value_notify(GetPKValue());
		}
		return 0;
		

		case GM_MSG_PLAYER_BECOME_INVADER:
		if(_pk_level <= 0 )
		{
			_invader_counter = msg.param & 0x7F;
			gplayer * pPlayer = GetParent();
			if(!pPlayer->CheckObjectState(gactive_object::STATE_INVADER))
			{
				pPlayer->SetObjectState(gactive_object::STATE_INVADER);
				_runner->player_invader_state(true);
				NotifyMasterInfo();
			}
		}
		return 0;

		case GM_MSG_QUERY_SELECT_TARGET:
		{
			SendTo<0>(GM_MSG_NOTIFY_SELECT_TARGET,msg.source,
					((gplayer_controller*)_commander)->GetCurTarget().id);
		}
		return 0;
		
		case GM_MSG_NOTIFY_SELECT_TARGET:
		{
			((gplayer_controller*)_commander)->SelectTarget(msg.param);
		}
		return 0;

		case GM_MSG_SUBSCIBE_CONFIRM:
		{
			((gplayer_controller*)_commander)->SubscibeConfirm(msg.source);
		}
		return 0;

		case GM_MSG_GATHER_REPLY:
		{
			//受到可以开采的通知
			if(HasSession())
			{
				//取消采集
				SendTo<0>(GM_MSG_GATHER_CANCEL,msg.source,0);
			}
			else
			{
				//开始采集
				session_gather *pSession = new session_gather(this);
				ASSERT(msg.param>0);
				bool can_be_interrupted = true;
				int eliminate_tool = -1;
				if(msg.content_length == sizeof(gather_reply))
				{
					gather_reply * pr = (gather_reply*)msg.content;
					can_be_interrupted = pr->can_be_interrupted;
					eliminate_tool = pr->eliminate_tool;
				}
				if(eliminate_tool != -1)
				{
					//只有需要删除物品才锁定物品栏
					pSession->LockInventory();
				}
				pSession->SetTarget(msg.source.id,msg.param,can_be_interrupted); 
				AddStartSession(pSession);
			}
		}
		return 0;

		case GM_MSG_GATHER_RESULT:
		{
			//收到开采结果
			ASSERT(msg.content_length == sizeof(gather_result));
			gather_result * res = (gather_result*)msg.content;
			if(res->eliminate_tool > 0)
			{
				//检查物品是否存在，如果存在才能进行此操作，存在物品的话则删除此物品
				int rst = _inventory.Find(0,res->eliminate_tool);
				if(rst >= 0)
				{	
					UseItemLog(_inventory[rst], 1);
					_inventory.DecAmount(rst,1);
					_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,
							res->eliminate_tool,1,S2C::DROP_TYPE_USE);
				}
				else
				{
					//无法采集
					_runner->error_message(S2C::ERR_MINE_GATHER_FAILED);
					return 0;
				}
			}

			int item_id = msg.param;
			size_t count = res->amount;
			int task_id = res->task_id;
			if(item_id > 0 && count > 0)
			{
				
				element_data::item_tag_t tag = {element_data::IMT_NULL,0};
				item_data * data = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
				if(data)
				{
					if(count > data->pile_limit) count = data->pile_limit;
					GLog::log(GLOG_INFO,"用户%d采集得到%d个%d",_parent->ID.id, count,item_id);
					OnGatherMatter(item_id, count);
					
					data->count = count;
					int rst = _inventory.Push(*data);
					int state = item::Proctype2State(data->proc_type);
					if(rst >=0) _runner->obtain_item(item_id,data->expire_date,count - data->count,_inventory[rst].count, 0,rst,state);
					if(data->count)
					{
						_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
						//剩余了物品
						DropItemData(_plane,_parent->pos,data,_parent->ID,0,0);
						//这种情况不需要释放内存
						//报告制造中断
					}
					else
					{
						FreeItem(data);
					}
				}
				else
				{
					//物品生成失败
					_runner->error_message(S2C::ERR_MINE_GATHER_FAILED);
				}
			}

			if(task_id > 0)
			{
				//任务矿，传递给任务系统
				PlayerTaskInterface  task_if(this);
				OnTaskMining(&task_if,task_id);
			}

			OnMineEnd(item_id);
		}
		return 0;

		case GM_MSG_TEAM_APPLY_PARTY:
		{
			ASSERT(msg.content_length == sizeof(msg_apply_party));

			const msg_apply_party * pMsg = (const msg_apply_party*)msg.content;
			if(GetWorldManager()->GetWorldLimit().faction_team && (pMsg->faction & GetEnemyFaction())) return 0;
			gplayer* pPlayer = GetParent();
			if((pPlayer->IsBattleOffense() && !(pMsg->faction & FACTION_BATTLEOFFENSE)) 
			  ||(!(pPlayer->IsBattleOffense()) && (pMsg->faction & FACTION_BATTLEOFFENSE)) 
			  ||(pPlayer->IsBattleDefence() && !(pMsg->faction & FACTION_BATTLEDEFENCE))
			  ||(!(pPlayer->IsBattleDefence()) && (pMsg->faction & FACTION_BATTLEDEFENCE)))
			{
				return 0;
			}

			_team.ApplyParty(msg.source, pMsg->faction, pMsg->family_id, pMsg->mafia_id, pMsg->level, pMsg->sectid, pMsg->referid);
		}
		return 0;
		
		case GM_MSG_TEAM_CHANGE_TO_LEADER:
		{
			_team.ChangeToLeader(msg.source);
			PlayerTaskInterface  tif(this);
			OnTeamCaptainChange(&tif);
		}
		return 0;

		case GM_MSG_TEAM_LEADER_CHANGED:
		{
			_team.LeaderChanged(msg.source);
			PlayerTaskInterface  tif(this);
			OnTeamCaptainChange(&tif);
		}
		return 0;

		case GM_MSG_TEAM_APPLY_REPLY:
		{
			_team.ApplyPartyReply(msg.source,msg.param);
		}
		return 0;

		case GM_MSG_GM_RECALL:
		{
			LongJump(msg.pos,msg.param);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_MQUERY_MOVE_POS_RPY:
		{
			ASSERT(sizeof(instance_key) == msg.content_length);
			//这里简单的long jump不能满足需求了
			GLog::log(GLOG_INFO,"GM:%d移动到玩家%d处,tag:%d,(%f,%f,%f)",_parent->ID.id,msg.source.id,msg.param,msg.pos.x,msg.pos.y,msg.pos.z);
			if(msg.param == _plane->GetTag())
			{
				//同一个副本的跳转又如何？
				if(!GetWorldManager()->IsUniqueWorld())
				{
					_runner->error_message(S2C::ERR_CAN_NOT_JUMP_BETWEEN_INSTANCE);
				}
				else
				{
					LongJump(msg.pos,msg.param);
				}
				return 0;
			}

			//执行副本跳转逻辑
			instance_key key = *(instance_key*)msg.content;
			key.target = key.essence;

			//让Player进行副本传送 
			if(GetWorldManager()->PlaneSwitch(this ,msg.pos,msg.param,key,0) < 0)
			{
				_runner->error_message(S2C::ERR_CANNOT_ENTER_INSTANCE);
			}
		}
		return 0;

		case GM_MSG_GM_MQUERY_MOVE_POS:
		{	
			int world_tag = _plane->GetTag();
			instance_key key;
			GetInstanceKey(world_tag,key);
			SendTo<0>(GM_MSG_GM_MQUERY_MOVE_POS_RPY,msg.source,world_tag,&key,sizeof(key));
		}
		return 0;
		
		case GM_MSG_GM_CHANGE_EXP:
		{
			return 0;	//暂时禁止了
		}
		return 0;

		case GM_MSG_GM_ENDUE_ITEM:
		{
			return 0;	//暂时禁止了
			if(_inventory.GetEmptySlotCount() <=0)
			{
				//发送回馈消息....
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_INVENTORY_IS_FULL);
				return 0;
			}
			int item_id = msg.param;
			size_t count  = *(int*)msg.content;
			element_data::item_tag_t tag = {element_data::IMT_NULL,0};
			item_data * data = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
			if(!data) 
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_PRODUCE_FAILED);
				return 0;
			}
			if(count > data->pile_limit) count = data->pile_limit;
			data->count = count;
			if(ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_RESET_PP:
		{
			_basic.status_point += player_template::Rollback(GetPlayerClass(),_base_prop);
			//使得装备生效
			RefreshEquipment();
			//生成装备的数据（供外人看）
			CalcEquipmentInfo();

			//给自己信息
			PlayerGetProperty();
		}
		return 0;

		case GM_MSG_GM_ENDUE_SELL_ITEM:
		{
			return 0;	//暂时禁止了
			if(_inventory.GetEmptySlotCount() <=0)
			{
				//发送回馈消息....
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_INVENTORY_IS_FULL);
				return 0;
			}
			int item_id = msg.param;
			size_t count  = *(int*)msg.content;
			const void *pBuf = gmatrix::GetDataMan().get_item_for_sell(item_id);
			if(!pBuf) 
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_PRODUCE_FAILED);
				return 0;
			}
			item_data * data = DupeItem(*(const item_data*)pBuf);
			if(count > data->pile_limit) count = data->pile_limit;
			data->count = count;
			if(ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_REMOVE_ITEM:
		{
			int item_id = msg.param;
			size_t num = *(int*)msg.content;
			if(item_id < 0) 
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_ITEM_NOT_IN_INVENTORY);
				return 0;
			}
			int rst = 0;
			while(num && (rst = _inventory.Find(rst,item_id)) >= 0)
			{
				size_t count = num;
				if(_inventory[rst].count < count) count = _inventory[rst].count;
				UseItemLog(_inventory[rst], count);
				_inventory.DecAmount(rst,count);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,item_id,count,S2C::DROP_TYPE_GM);
				num -= count;
				rst ++;
			}
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_RESURRECT:
		{
			if(IsDead())
			{
				Resurrect(0.f);
			}
		}
		return 0;

		case GM_MSG_GM_ENDUE_MONEY:
		{
			return 0;	//暂时禁止了
		}
		return 0;

		case GM_MSG_GM_OFFLINE:
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_DEBUG_COMMAND:
		{
			gplayer_controller * pCtrl = ((gplayer_controller*)_commander);
			pCtrl->SetDebugMode(!pCtrl->GetDebugMode());
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_PLANE_SWITCH_REPLY:
		{
			//本消息只有NORMAL状态下可以进行处理
			if(!_pstate.CanSwitch()) return 0;
			if(msg.source.id == _plane->GetTag())
			{
				//转移的目标和源不应是一个
				return 0;
			}
			ASSERT(msg.content_length == sizeof(instance_key));
			instance_key * key = (instance_key*)msg.content;
			SwitchSvr(msg.source.id,_parent->pos,msg.pos,key);
		}
		return 0;

		case GM_MSG_DBSAVE_ERROR:
		{
			_db_save_error ++;
			if(_db_save_error >= 3)
			{
				GLog::log(GLOG_ERR,"用户%d由于存盘错误次数过多，被强行执行下线逻辑",_parent->ID.id);
				LostConnection(PLAYER_OFF_LPG_DISCONNECT);
			}
		}
		return 0;

		case GM_MSG_ENABLE_PVP_DURATION:
		{
			__PRINTF("%d进入PK状态\n",_parent->ID.id);
			SetPVPCombatState();
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_REQUEST:
		{
			//检查距离 
			if(_parent->pos.squared_distance(msg.pos) <= 400)
			{
				_duel.MsgDuelRequest(this,msg.source);
			}
			else
			{
				SendTo<0>(GM_MSG_PLAYER_DUEL_REPLY,msg.source,player_duel::DUEL_REPLY_OUT_OF_RANGE);
			}
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 400)
			{
				_duel.MsgDuelReply(this,msg.source,msg.param);
			}
			else
			{
				SendTo<0>(GM_MSG_PLAYER_DUEL_CANCEL,msg.source,0);
			}
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_PREPARE:
		{
			_duel.MsgDuelPrepare(this,msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_START:
		{
			if(msg.source != _parent->ID)
			{
				_duel.MsgDuelStart(this,msg.source);
			}
			else
			{
				ASSERT(msg.content_length == sizeof(XID));
				_duel.MsgDuelStart(this,*(XID *)msg.content);
			}
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_CANCEL:
		{
			_duel.MsgDuelCancel(this,msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_STOP:
		{
			if(msg.source != _parent->ID)
			{
				_duel.MsgDuelStop(this,msg.source,msg.param);
			}
			else
			{
				ASSERT(msg.content_length == sizeof(XID));
				_duel.MsgDuelStop(this,*(XID *)msg.content,msg.param);
			}
		}
		return 0;

		case GM_MSG_PLAYER_BIND_REQUEST:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgRequest(this,msg.source);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_INVITE:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgInvite(this,msg.source);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_REQ_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgRequestReply(this,msg.source,msg.param);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_INV_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgInviteReply(this,msg.source,msg.param);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_PREPARE:
		{
			_bind_player.MsgPrepare(this,msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_BIND_LINK:
		{
			_bind_player.MsgBeLinked(this,msg.source,msg.pos);
		}
		return 0;
	
 		case GM_MSG_PLAYER_BIND_STOP:
		{
			_bind_player.MsgStopLinked(this,msg.source);
 		}
		return 0;
	
		case GM_MSG_PLAYER_BIND_FOLLOW:
		{
			//50米内才能follow
			if(_parent->pos.squared_distance(msg.pos) <= 50.f*50.f)
			{
				_bind_player.MsgFollowOther(this,msg.source,msg.pos);
			}
		}
		return 0;

		case GM_MSG_PLAYER_LINK_RIDE_INVITE:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100){
				_link_ride_player.MsgInvite(this,msg.source);
			}
		}
		return 0;

		case GM_MSG_PLAYER_LINK_RIDE_INV_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_link_ride_player.MsgInviteReply(this,msg.source,msg.param);
			}
		}
		return 0;

     		case GM_MSG_PLAYER_LINK_RIDE_START:                                                                             
		{                                                                                                         
			_link_ride_player.MsgBeLinked(this,msg.source,msg.pos,msg.param);                                                    
		}                                                                                                         
		return 0;                                                                                                 
        
		case GM_MSG_PLAYER_LINK_RIDE_FOLLOW:
		{
			//50米内才能follow
			if(_parent->pos.squared_distance(msg.pos) <= 50.f*50.f)
			{
				_link_ride_player.MsgFollowOther(this,msg.source,msg.pos);
			}
		}
		return 0;
        
		case GM_MSG_PLAYER_LINK_RIDE_STOP:                                                                             
		{                                                                                                         
			_link_ride_player.MsgStopLinked(this,msg.source);                                                          
		}                                                                                                         
		return 0;                                                                                                 
        
		case GM_MSG_PLAYER_LINK_RIDE_LEAVE_REQUEST:                                                                    
		{
			_link_ride_player.MsgMemberLeaveRequest(this, msg.source);                                                 
		}
		return 0;            
                                                                                     
		case GM_MSG_PLAYER_LINK_RIDE_MEMBER_LEAVE:                                                                     
        	{
            		_link_ride_player.MsgMemberLeave(this, msg.source, XID(GM_TYPE_PLAYER,msg.param));                         
		}
		return 0;  

		case GM_MSG_PLAYER_LINK_RIDE_LEADER_LEAVE:
		{
			_link_ride_player.MsgLeaderLeave(this, msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_LINK_RIDE_KICK_MEMBER:
		{
			_link_ride_player.MsgKickMember(this, msg.source);
		}
		return 0;

		case GM_MSG_QUERY_EQUIP_DETAIL:
		{
			if(GetEnemyFaction() & msg.param)
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_CANNOT_QUERY_ENEMY_EQUIP);
				return 0;
			}

			float dis = msg.pos.squared_distance(_parent->pos);
			if(dis <= 30*30)
			{
				ASSERT(msg.content_length == sizeof(int)*2);
				int * tmp = (int*)(msg.content);
				int cs_index = tmp[0];
				int cs_sid = tmp[1];

				static const  int query_list[] = {
					item::EQUIP_INDEX_WEAPON,
					item::EQUIP_INDEX_HEAD,
					item::EQUIP_INDEX_BODY,
					item::EQUIP_INDEX_FOOT,
					item::EQUIP_INDEX_NECK,
					item::EQUIP_INDEX_FINGER1,
					item::EQUIP_INDEX_FINGER2,
					item::EQUIP_INDEX_FASHION_EYE,	
					item::EQUIP_INDEX_MAGIC,
					item::EQUIP_INDEX_FASHION_LIP,
					item::EQUIP_INDEX_FASHION_HEAD,
					item::EQUIP_INDEX_FASHION_BODY,
					item::EQUIP_INDEX_FASHION_LEG,
					item::EQUIP_INDEX_FASHION_FOOT,
					item::EQUIP_INDEX_FASHION_BACK,
					item::EQUIP_INDEX_WING,
					item::EQUIP_INDEX_TALISMAN1,
					item::EQUIP_INDEX_TALISMAN2,
					item::EQUIP_INDEX_TALISMAN3,
					item::EQUIP_INDEX_BUGLE,
					item::EQUIP_INDEX_XITEM1,
					item::EQUIP_INDEX_XITEM2,
					item::EQUIP_INDEX_XITEM3,
					item::EQUIP_INDEX_RUNE,
					item::EQUIP_INDEX_XITEM4,
					item::EQUIP_INDEX_XITEM5,
					item::EQUIP_INDEX_FASHION_WEAPON,  // Youshuang add
				};

				raw_wrapper rw(1024);
				size_t count = sizeof(query_list)/sizeof(int);
				_equipment.DetailSavePartial(rw,query_list,count);
				raw_wrapper rw_skill(256);
				_skill.StorePartial(rw_skill);
				raw_wrapper rw_pet(1024);
				int query_pet = _petman.GetActivePetIndex(this);
				if(-1 != query_pet)
				{
					_pet_bedge_inventory.DetailSavePartial(rw_pet,&query_pet,1);
				}
				raw_wrapper rw_pet_equip(1024);
				int query_list_pet_equip[PET_EQUIP_LIST_SIZE];
				for(int i = 0;i < PET_EQUIP_LIST_SIZE;++i)
				{
					query_list_pet_equip[i] = i;
				}
				_pet_equip_inventory.DetailSavePartial(rw_pet_equip,query_list_pet_equip,PET_EQUIP_LIST_SIZE);
				_runner->send_equip_detail(cs_index, cs_sid,msg.source.id,rw.data(),rw.size(),rw_skill.data(),rw_skill.size(),
						rw_pet.data(),rw_pet.size(),rw_pet_equip.data(),rw_pet_equip.size());
				_runner->peep_info(msg.source.id);
			}
		}
		return 0;

		case GM_MSG_PLAYER_RECALL_PET:
		{
			_petman.ClearAllPetBehavior(this);
		}
		return 0;
		
		case GM_MSG_REMOVE_ITEM:
		{
			int item_id = msg.param;
			if(item_id <= 0) 
			{
				return 0;
			}
			int rst = 0;
			rst = _inventory.Find(rst,item_id);
			if(rst >= 0)
			{
				UseItemLog(_inventory[rst], 1);
				_inventory.DecAmount(rst,1);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,item_id,1,S2C::DROP_TYPE_GM);
			}
		}
		return 0;

		case GM_MSG_PET_RELOCATE_POS:
		{
			int dis = *(int*)msg.content;
			if(!_petman.PetRelocatePos(this,msg.source,msg.param,msg.param2,dis))
			{
				SendTo<0>(GM_MSG_PET_DISAPPEAR,msg.source,msg.param);
			}
		}
		return 0;

		case GM_MSG_PET_NOTIFY_HP_VP:
		{
			if(sizeof(msg_pet_notify_hp_vp_t) == msg.content_length)
			{
				msg_pet_notify_hp_vp_t* pInfo = (msg_pet_notify_hp_vp_t *) msg.content;
				if(!_petman.PetNotifyPetHPVPState(this,msg.source,msg.param,msg.param2,*pInfo))
				{
					SendTo<0>(GM_MSG_PET_DISAPPEAR,msg.source,msg.param);
				}
				if(pInfo->combat_state)
				{
					ActiveCombatState(true);
					if(_combat_timer < NORMAL_COMBAT_TIME)
					{
						_combat_timer = NORMAL_COMBAT_TIME;
					}
				}
			}
		}
		return 0;

		case GM_MSG_PET_NOTIFY_DEATH:
		{
			if(sizeof(int) == msg.content_length)
			{
				int kill_by_npc = *(int*)msg.content;
				_petman.PetDeath(this,msg.source,msg.param,msg.param2,kill_by_npc);
			}
		}
		return 0;

		case GM_MSG_PET_SET_COOLDOWN:
		{
			if(sizeof(msg_pet_skill_cooldown_t) == msg.content_length )
			{
				msg_pet_skill_cooldown_t* pCooldown = (msg_pet_skill_cooldown_t*)msg.content;
				_petman.PetSetCoolDown(this,msg.source,msg.param,msg.param2,pCooldown->cooldown_index,pCooldown->cooldown_time);
			}
		}
		return 0;

		case GM_MSG_PET_SET_AUTO_SKILL:
		{
			if(sizeof(msg_pet_auto_skill_set_t) == msg.content_length)
			{
				msg_pet_auto_skill_set_t* pData = (msg_pet_auto_skill_set_t*)msg.content;
				_petman.PetSetAutoSkill(this,msg.source,msg.param,msg.param2,pData->skill_id,pData->set_flag);
			}
		}
		return 0;

		/*
		case GM_MSG_PET_CAST_SKILL:
		{
			if(0 == msg.content_length)
			{
				_petman.PetCastSkill(this,msg.source,msg.param,msg.param2);
			}
		}
		return 0;
		*/

		case GM_MSG_PLAYER_KILL_SUMMON:
		{
			if(_mduel.InBattle())
			{
				if(_mduel.IsEnemy(msg.param)) return 0;
			}
			//杀死一个白名人的宠物
			ModifyPKValue(1);
			_runner->pk_value_notify(GetPKValue());
		}
		return 0;
		
		case GM_MSG_SUMMON_NOTIFY_DEATH:
		{
			_summonman.SummonNPCDeath(this,msg.source,msg.param,msg.param2);
			if(msg.content_length == sizeof(int))
			{
				int summonTid = *(int*)msg.content;
				if((summonTid == 50639 || summonTid == 50638 || summonTid == 45509) && _circle_of_doom.GetSkillid() == 3454)
				{
					_filters.RemoveFilter(FILTER_CYCLE);
				}
			}
		}
		return 0;
		
		case GM_MSG_SUMMON_RELOCATE_POS:
		{
			if(!_summonman.SummonRelocatePos(this,msg.source,msg.param,msg.param2))
			{
				SendTo<0>(GM_MSG_NPC_DISAPPEAR,msg.source,msg.param);
			}
		}
		return 0;
		

		case GM_MSG_SUMMON_HEARTBEAT:
		{
			if(!_summonman.SummonHeartbeat(this,msg.source,msg.param))
			{
				SendTo<0>(GM_MSG_NPC_DISAPPEAR,msg.source,msg.param);
			}
		}
		return 0;

		case GM_MSG_REPU_CHG_STEP_1:
		{
			int rep1 = msg.param;
			int rep2 = GetReputation();
			int offset = player_template::CalcPKReputation(rep2,rep1);
			SendTo<0>(GM_MSG_REPU_CHG_STEP_2, msg.source, offset);
		}
		return 0;

		case GM_MSG_REPU_CHG_STEP_2:
		{
			int bonus = msg.param;
			int rep = (int)(bonus* 1.05f);
			if(rep > GetReputation()) 
			{
				rep = GetReputation();
				bonus = (int)(rep * (1.0f/1.05f));
				if(bonus <= 0) bonus = 1;
			}
			ModifyReputation(-rep);
			SendTo<0>(GM_MSG_REPU_CHG_STEP_3, msg.source, bonus);
		}
		return 0;

		case GM_MSG_REPU_CHG_STEP_3:
		{
			ModifyReputation(msg.param);
		}
		return 0;

		case GM_MSG_TEAM_MEMBER_LVLUP:
		{
			//目前所有特殊状态都不得
			size_t level = msg.param;
			if(level <  HELP_BONUS_LEVEL && _basic.level >= HELP_LEVEL_LIMIT && IsTeamLeader())
			{
				int bonus = level * ((level/10) + 1);
				ModifyReputation(bonus);
			}
		}
		return 0;

		case GM_MSG_TASK_AWARD_TRANSFOR:
		{
			if(msg.content_length == sizeof(msg_task_transfor_award))
			{
				msg_task_transfor_award award = *(msg_task_transfor_award*)msg.content;
				PlayerTaskInterface  tif(this);
				if(award.gold && tif.IsDeliverLegal())
				{
					tif.DeliverGold(award.gold);
				}
				if(award.exp ) tif.DeliverExperience(award.exp);
				if(award.reputation ) tif.DeliverReputation(award.reputation);
				if(award.region_rep_val ) tif.DeliverRegionReputation(award.region_rep_idx,award.region_rep_val);
				if(award.common_item_id && tif.IsDeliverLegal() && tif.CanDeliverCommonItem(1)) tif.DeliverCommonItem(award.common_item_id, award.common_item_count, award.common_item_bind, award.common_item_period); 
				if(award.task_item_id && tif.IsDeliverLegal() && tif.CanDeliverTaskItem(1)) tif.DeliverTaskItem(award.task_item_id, award.task_item_count); 
				if(award.title) tif.GiveOneTitle(award.title);
				
			}
		}
		return 0;
	
		case GM_MSG_PLAYER_CATCH_PET:
		{
			if(msg.content_length == sizeof(msg_catch_pet))
			{
				msg_catch_pet catch_msg = *(msg_catch_pet*)msg.content;
				int petbedge_id = 0;
				if(CatchPet(catch_msg.monster_id, catch_msg.monster_level, catch_msg.monster_raceinfo, catch_msg.monster_catchdifficulty, catch_msg.monster_hp_ratio, petbedge_id))
				{
					SendTo<0>(GM_MSG_NPC_BE_CATCHED, msg.source, petbedge_id);		
				}
			}

		}	
		return 0;

		case GM_MSG_NPC_BE_CATCHED_CONFIRM:
		{
			CatchPetSuccess(msg.param);
		}
		return 0;
		
		case GM_MSG_QUERY_ACHIEVEMENT:
		{
			ASSERT(msg.content_length == sizeof(int)*2);
			int * tmp = (int*)(msg.content);
			int cs_index = tmp[0];
			int cs_sid = tmp[1];

			size_t map_size;
			const void * map_buf  = GetAchievementManager().GetMap().data(map_size);
			int achieve_point = GetAchievementManager().GetAchievementPoint();
			_runner->player_achievement_map(cs_index, cs_sid, msg.source.id, achieve_point, map_buf, map_size);
		}
		return 0;

		case GM_MSG_ENTER_CARRIER:
	      	{
			if(sizeof(msg_enter_carrier) != msg.content_length) return 0;
			const msg_enter_carrier* pData = (const msg_enter_carrier*)msg.content;
			EnterCarrier(msg.source.id,pData->rpos,pData->rdir,pData->carrier_pos,pData->carrier_dir);
		}
		return 0;

		case GM_MSG_LEAVE_CARRIER:
		{
			LeaveCarrier(msg.source.id);
		}
		return 0;	

		case GM_MSG_CARRIER_SYNC_POS:
		{
			gplayer* pPlayer = GetParent();
			if(pPlayer->carrier_id == msg.source.id)
			{
				A3DVECTOR offset = msg.pos; 
				offset += pPlayer->rpos;
				offset -= pPlayer->pos;
				StepMove(offset);
				pPlayer->dir = msg.param + pPlayer->rdir;
			}
		}
		return 0;
		
		case GM_MSG_USE_COMBO_SKILL:
		{
			if(sizeof(msg_combo) != msg.content_length) return 0;
			const msg_combo * pData = (const msg_combo*)msg.content;
			UseComboSkill(pData->combo_type, pData->combo_color, pData->combo_color_num);
		}
		return 0;

		case GM_MSG_PROTECTED_NPC_NOTIFY:
		{
			PlayerTaskInterface  task_if(this);
			int task_id = msg.param;
			int reason = msg.param2;
			OnTaskProtectNPCNotify(&task_if, task_id, reason);
		}
		return 0;

		case GM_MSG_CIRCLE_OF_DOOM_PREPARE:
		{
			HandleMsgCircleOfDoomPrepare(msg);
		}
		return 0;

		case GM_MSG_CIRCLE_OF_DOOM_STARTUP:
		{
			HandleMsgCircleOfDoomStartup(msg);	
		}
		return 0;

		case GM_MSG_CIRCLE_OF_DOOM_STOP:
		{
			HandleMsgCircleOfDoomStop(msg);	
		}
		return 0;

		case GM_MSG_CIRCLE_OF_DOOM_ENTER:
		{
			HandleMsgCircleOfDoomMemberEnter(msg);
		}
		return 0;

		case GM_MSG_CIRCLE_OF_DOOM_LEAVE:
		{
			HandleMsgCircleOfDoomMemberLeave(msg);	
		}
		return 0;

		case GM_MSG_CIRCLE_OF_DOOM_QUERY:
		{
			HandleMsgCircleOfDoomQuery(msg);	
		}
		return 0;

		case GM_MSG_REMOVE_PERMIT_CYCLE_AREA:
		{
			int id = msg.source.id;
			_filters.ModifyFilter(4218, FMID_REMOVE_AREA, &id, sizeof(id));
		}	
		return 0;

		case GM_MSG_CANCEL_BE_PULLED:
		{
			_filters.RemoveFilter(FILTER_PULL);
		}
		return 0;

		case GM_MSG_CANCEL_BE_CYCLE:
		{
			if(-1 == msg.param)
			{
				_filters.RemoveFilter(FILTER_CYCLE);
			}
			else
			{
				if(GetCurSessionID() == msg.param)
				{
					session_cancel_action *pCancel= new session_cancel_action();
					AddStartSession(pCancel);

					//停止当前的session
					TryStopCurSession();
				}
			}
		}
		return 0;

		case GM_MSG_QUERY_BE_SPIRIT_DRAGGED:
		{
			if(!((gplayer*)_parent)->IsBeSpiritDraggedState())
			{
				__PRINTF("玩家未处于被灵力牵引状态\n");
			}
			else
			{
				int sid = msg.param;
				int cs_index = msg.param2;
				_runner->player_be_pulled(msg.source, cs_index, sid, id_spirit_drag_me, true, 1);
			}
		}	
		return 0;

		case GM_MSG_QUERY_BE_PULLED:
		{
			if(!((gplayer*)_parent)->IsBePulledState())
			{
				__PRINTF("玩家未处于被拉扯状态\n");
			}
			else
			{
				int sid = msg.param;
				int cs_index = msg.param2;
				_runner->player_be_pulled(msg.source, cs_index, sid, id_pulling_me, true, 0);
			}
		}
		return 0;

		case GM_MSG_TASK_CHECK_STATE:
		{
			PlayerTaskInterface tf(this);
			OnTaskCheckState(&tf);
		}
		return 0;

		case GM_MSG_SPIRIT_SESSION_END:
		{
			EndSpiritSession(msg.param);
		}
		return 0;

		case GM_MSG_SPIRIT_SESSION_REPEAT:
		{
			SpiritSessionMap::iterator it = _spirit_session_list.find(msg.param);
			if(it != _spirit_session_list.end())
			{
				act_session* ses = it->second;
				if(ses)
				{
					if(!ses->RepeatSession())
					{
						EndSpiritSession(msg.param);
					}
				}
			}
		}
		return 0;

		case GM_MSG_TALISMAN_SKILL:
		{
			if(msg.content_length != sizeof(msg_talisman_skill))
			{
				return 0;
			}
			const msg_talisman_skill* mts = (const msg_talisman_skill *) msg.content;
			ActiveTalismanSkillEffects(msg.source, mts);
		}
		return 0;

		case GM_MSG_KINGDOM_CALL_GUARD:
		{
			if(!IsKingdomGuard()) return 0;
			msg_kingdom_call_guard * pMsg = (msg_kingdom_call_guard*)msg.content;
			KingCallGuardInvite(pMsg->kingname, pMsg->kingname_len, pMsg->line_id, pMsg->map_id, pMsg->pos);
		}
		return 0;

		case GM_MSG_PLAYER_LINK_BATH_INVITE:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100){
				_link_bath_player.MsgInvite(this,msg.source);
			}
		}
		return 0;

		case GM_MSG_PLAYER_LINK_BATH_INV_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_link_bath_player.MsgInviteReply(this,msg.source,msg.param);
			}
		}
		return 0;

     		case GM_MSG_PLAYER_LINK_BATH_START:                                                                             
		{                                                                                                         
			_link_bath_player.MsgBeLinked(this,msg.source,msg.pos,msg.param);                                                    
		}                                                                                                         
		return 0;                                                                                                 
        
		case GM_MSG_PLAYER_LINK_BATH_STOP:                                                                             
		{                                                                                                         
			_link_bath_player.MsgStopLinked(this,msg.source);                                                          
		}                                                                                                         
		return 0;                                                                                                 

		case GM_MSG_PLAYER_LINK_BATH_LEAVE_REQUEST:                                                                    
		{
			_link_bath_player.MsgMemberLeaveRequest(this, msg.source);                                                 
		}
		return 0;            
        
		case GM_MSG_PLAYER_LINK_BATH_MEMBER_LEAVE:                                                                     
        	{
            		_link_bath_player.MsgMemberLeave(this, msg.source, XID(GM_TYPE_PLAYER,msg.param));                         
		}
		return 0;  

		case GM_MSG_PLAYER_LINK_BATH_LEADER_LEAVE:
		{
			_link_bath_player.MsgLeaderLeave(this, msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_LINK_QILIN_INVITE:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100){
				_link_qilin_player.MsgInvite(this,msg.source);
			}
		}
		return 0;

		case GM_MSG_PLAYER_LINK_QILIN_INV_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_link_qilin_player.MsgInviteReply(this,msg.source,msg.param);
			}
		}
		return 0;

     		case GM_MSG_PLAYER_LINK_QILIN_START:                                                                             
		{                                                                                                         
			_link_qilin_player.MsgBeLinked(this,msg.source,msg.pos,msg.param);                                                    
		}                                                                                                         
		return 0;                                                                                                 

     		case GM_MSG_PLAYER_LINK_QILIN_FOLLOW:                                                                             
		{                                                                                                         
			//50米内才能follow
			if(_parent->pos.squared_distance(msg.pos) <= 50.f*50.f)
			{
				_link_qilin_player.MsgFollowOther(this,msg.source,msg.pos);
			}
		}                                                                                                         
		return 0;                                                                                                 
        
		case GM_MSG_PLAYER_LINK_QILIN_STOP:                                                                             
		{                                                                                                         
			_link_qilin_player.MsgStopLinked(this,msg.source);                                                          
		}                                                                                                         
		return 0;                                                                                                 

		case GM_MSG_PLAYER_LINK_QILIN_LEAVE_REQUEST:                                                                    
		{
			_link_qilin_player.MsgMemberLeaveRequest(this, msg.source);                                                 
		}
		return 0;            
        
		case GM_MSG_PLAYER_LINK_QILIN_MEMBER_LEAVE:                                                                     
        	{
            		_link_qilin_player.MsgMemberLeave(this, msg.source, XID(GM_TYPE_PLAYER,msg.param));                         
		}
		return 0;  

		case GM_MSG_PLAYER_LINK_QILIN_LEADER_LEAVE:
		{
			_link_qilin_player.MsgLeaderLeave(this, msg.source);
		}
		return 0;


		case GM_MSG_SUMMON_TELEPORT_REPLY:
		{
			A3DVECTOR pos = *(A3DVECTOR*)msg.content;
			HandleSummonTeleport(pos, msg.param);
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_STATE_START:
		{
			//TODO
			int pos = *(((int*)msg.content) +0);
			int active_mount_point = *(((int*)msg.content) +1);
			int active_time[3];
			active_time[0] = *(((int*)msg.content)+2);
			active_time[1] = *(((int*)msg.content)+3);
			active_time[2] = *(((int*)msg.content)+4);
			int path_id = *(((int*)msg.content)+5);
			int active_mode = *(((int*)msg.content)+6);
			gplayer* pPlayer = GetParent();
			pPlayer->before_mobactive_pos = pPlayer->pos;
			_mobactiveman.SetBindTarget(this,msg.source); //设置互动对象
			pPlayer->mobactive_pos = pos; 
			_mobactiveman.SetMobActiveMountPoint(active_mount_point);
			_mobactiveman.SetMobActiveTime(active_time,sizeof(active_time)/sizeof(active_time[0]));
			_mobactiveman.SetMobActiveState(mobactive_manager::MOB_ACTIVE_STATE_START);
			pPlayer->mobactive_state = mobactive_manager::MOB_ACTIVE_STATE_START;
			_mobactiveman.SetMobActiveMode(active_mode);
			_mobactiveman.SetMobActivePos(pos);
			_mobactiveman.MobActiveSyncPos(this);
			_mobactiveman.SetMobActiveInterruptCondition();
			_mobactiveman.MobActiveStateStartCtrl(this);
			_mobactiveman.SetMobActivePathId(path_id);
			_mobactiveman.DenyCmd(this);
			_mobactiveman.MsgMobActiveStateStart(this);
			_mobactiveman.DeliverMobActiveTask(this,mobactive_manager::MOB_ACTIVE_STATE_START);
			//	AddStartSession(new session_mob_active_ready(this,pos,active_mount_point,active_ready_time));
		}
		return 0;


		case GM_MSG_MOB_ACTIVE_STATE_FINISH:
		{
			_mobactiveman.MsgMobActiveStateFinish(this);
			//完成与互动怪交互
			int mobactive_tid = GetMobActiveTid();
			if(_task_mask & TASK_MASK_INTER_REACH_SITE)
			{
				//调用任务系统的操作
				PlayerTaskInterface  task_if(this);
				OnTaskInterLeaveSite(&task_if,mobactive_tid);
			}
			else if (_task_mask & TASK_MASK_INTER_LEAVE_SITE)
			{
				PlayerTaskInterface  task_if(this);
				OnTaskInterReachSite(&task_if,mobactive_tid);
			}
			else if (_task_mask & TASK_MASK_COLLECT_INTEROBJ)
			{
				PlayerTaskInterface  task_if(this);
				OnTaskCollectInterObj(&task_if,mobactive_tid);
			}
			
			_mobactiveman.AllowCmd(this);
			_mobactiveman.Reset();
			GetParent()->mobactive_id = 0;
			int itemid[5] = {0};
			memcpy(itemid,msg.content,sizeof(itemid));
			for (int idx = 0; idx < 5; idx++)
			{
				int	item_id = itemid[idx];
				if (item_id > 0)
				{
					int count = 1;
					element_data::item_tag_t tag = {element_data::IMT_NULL,0};
					item_data * data = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
					if(data)
					{
						GLog::log(GLOG_INFO,"用户%d互动得到%d个%d",_parent->ID.id, count, item_id);

						data->count = 1;
						int rst = _inventory.Push(*data);
						int state = item::Proctype2State(data->proc_type);
						if(rst >=0) _runner->obtain_item(item_id,data->expire_date,count - data->count,_inventory[rst].count, 0,rst,state);
						if(data->count)
						{
							_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
							//剩余了物品
						}
						else
						{
							FreeItem(data);
						}
					}
					else
					{
						//物品生成失败
						_runner->error_message(S2C::ERR_MOB_ACTIVE_FINISH_GET_ITEM_FAILED);
					}
				}
			}
		}
		return 0;

		//收到互动怪发来的解除互动消息
		case GM_MSG_MOB_ACTIVE_STATE_CANCEL:
		{
			gplayer* pPlayer = GetParent();
			if(pPlayer->mobactive_id == msg.source.id)
			{
				pPlayer->mobactive_id = 0;
				_mobactiveman.Reset();
				_mobactiveman.AllowCmd(this);
			}
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_SYNC_POS:
		{
			gplayer* pPlayer = GetParent();
			if(pPlayer->mobactive_id == msg.source.id)
			{
				A3DVECTOR offset = msg.pos; 
				offset -= pPlayer->pos;
				StepMove(offset);
			}
		}
		return 0;

		case GM_MSG_FAC_BUILDING_COMPLETE:
		{
			/*
			if (msg.content_length != sizeof(msg_fac_building_complete))
				return 0;
			const msg_fac_building_complete * mfbc = (const msg_fac_building_complete *)msg.content;
			_runner->fac_building_complete(mfbc->index, mfbc->tid, mfbc->level);
			*/
		}
		return 0;

		case GM_MSG_PLAYER_ACTIVE_EMOTE_INVITE:
		{
			if (_parent->pos.squared_distance(msg.pos) <= 100)	
			{
				_active_emote_player.CliMsgInvite(this,msg.source,msg.param);
			}
		}
		return 0;

		case GM_MSG_PLAYER_ACTIVE_EMOTE_INV_REPLY:
		{
			if (_parent->pos.squared_distance(msg.pos) <= 100)	
			{
				int type = *(((int*)msg.content) + 0);
				_active_emote_player.CliMsgInviteReply(this,msg.source,type, msg.param);
			}
		}
		return 0;

		case GM_MSG_PLAYER_ACTIVE_EMOTE_LINK:
		{
			_active_emote_player.CliMsgStartActiveEmote(this,msg.source, msg.param, msg.pos);
		}
		return 0;

		case GM_MSG_PLAYER_ACTIVE_EMOTE_STOP:
		{
			_active_emote_player.CliMsgStopActiveEmoteStop(this,msg.source);
		}
		return 0;

		case GM_MSG_MOB_ACTIVE_PATH_END:
		{
			_mobactiveman.PlayerMobActiveFinish(this);
		}
		return 0;

	}//switch end

	return gactive_imp::MessageHandler(msg);
}

void 
gplayer_imp::LevelUp()
{
	int cls = GetPlayerClass();
	bool is_level_up = false;

	int64_t need_exp1  = 0; //升级所需要的经验
	int64_t need_exp2  = 0; //同级别未飞升玩家升级所需的经验
	
	do
	{
		int64_t	 exp = player_template::GetLvlupExp(GetRebornCount(), _basic.level);
		int64_t  exp2 = (GetRebornCount() == 0) ? exp : player_template::GetLvlupExp(0, _basic.level);
		if(exp > _basic.exp)
		{	
			break;
		}

		_basic.exp -= exp;
		if(!player_template::LevelUp(cls,_basic.level,_base_prop,this))
		{
			GLog::log(GLOG_ERR,"用户%d升级时发生错误,职业%d,级别%d",_parent->ID.id,cls,_basic.level);
			PlayerForceOffline();
			return ;
		}
		if(_basic.level < HELP_BONUS_LEVEL)
		{
			if(IsInTeam() && !IsTeamLeader())
			{
				SendTo<0>(GM_MSG_TEAM_MEMBER_LVLUP ,GetTeamLeader(),_basic.level);
			}
		}
		_basic.level ++;
		DeliverTopicSite( new TOPIC_SITE::level_up( ( GetRebornCount() != 0 ), _basic.level ) );  // Youshuang add
		is_level_up = true;
		_runner->level_up();

		need_exp1 += exp;
		need_exp2 += exp2;
		//ModifyReputation(12 * (_basic.level -1));
		size_t m = GetMoney() + _trashbox.GetMoney();
		int tu = GetPlayEd();
		GLog::upgrade(_parent->ID.id,_basic.level,m);
		GLog::log(GLOG_INFO,"用户%d升级到%d级金钱%d,游戏时间%d:%02d:%02d",
				_parent->ID.id, _basic.level,m,tu/3600,(tu/60)%60,tu%60);

		object_interface oif(this);	
		GetAchievementManager().OnLevelUp(oif, GetRebornCount(), _basic.level);
		if(_basic.level >= player_template::GetMaxLevel(GetRebornCount()))
		{
			_basic.exp = 0;
			break;
		}
	}while(1);
	if(is_level_up)
	{
		property_policy::UpdatePlayer(cls,this);
		if(!((gplayer*)_parent)->IsZombie())
		{
			//让玩家血和魔回满
			FullHPAndMP();
		}

		GetParent()->level = (unsigned char) _basic.level;
		if(_basic.level >= PVP_PROTECT_LEVEL) 
		{
			EnablePVPFlag();
			//试图加入威望称谓，如果已存在则不通知客户端
			if(_extra_title.insert(PLAYER_DEFAULT_TITLE_REPUTATION).second)
			{
				_runner->player_add_title(PLAYER_DEFAULT_TITLE_REPUTATION);
			}
		}
		
		_talent_point = player_template::GetTalentPoint(GetRebornCount(), _basic.level, _basic.dt_level);
		_talent_point -= _skill.GetTalentSum();
		_runner->player_talent_notify(_talent_point);

		_basic.status_point = player_template::GetSkillPoint(GetRebornCount(), _basic.level, _basic.dt_level);
		_basic.status_point -= _skill.GetSpSum();

		PlayerTaskInterface  task_if(this);
		OnTaskPlayerLevelUp(&task_if);
		SetRefreshState();

		SyncPlayerMafiaInfo();
		SendRefLevelUp(need_exp1, need_exp2);

		//在线倒计时奖励检查
		CheckPlayerOnlineAward(g_timer.get_systime());

		if(_treasure_info.update_timestamp == 0 && IsTreasureActive())
		{
			InitTreasureRegion();
		}
		UpdateRuneProp();

		//每次升级清除XP技能冷却
		_skill.ClearXPSkillCoolTime(this);
	}
}

float
gplayer_imp::GetExtraExpRatio()
{
	gplayer * parent = GetParent();
	if(parent->IsVIPState())
	{
		return _vipstate.exp_ratio;
	}
	return 0;
}

void 
gplayer_imp::ReceiveGroupExp(const A3DVECTOR &pos,int64_t total_damage,int64_t exp,int level,size_t count, int npcid,int npctag, float r, const msg_grpexp_t * list, bool no_exp_punish)
{
	//首先除去所有已经离开的队员
	int64_t member_damage = 0;
	ASSERT(total_damage >0);
	float factor = 1.f/(float)(total_damage);
	char flag_list[TEAM_MEMBER_CAPACITY];
	memset(flag_list,0,sizeof(flag_list));
	int mlist[TEAM_MEMBER_CAPACITY];
	int mlist_count = 0;
	int max_level = 0;
	int min_level = 0xFFFF;
	int total_level = 0;
	for(size_t i = 0; i < count ; i ++)
	{
		int64_t damage = list[i].damage;
		A3DVECTOR mpos;
		int level;
		int index;
		int tag;
		if((index = _team.GetMember(list[i].who,mpos,level,tag)) >= 0)
		{
			flag_list[index] = 1;
			if(tag == npctag && mpos.squared_distance(pos) <= TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE) 
			{
				member_damage += damage;
				mlist[mlist_count] = index;
				mlist_count ++;
				total_level += level;

				if(level > max_level) max_level = level;
				if(level < min_level) min_level = level;

			}

		}
		else
		{
			msg_exp_t data;
			float tmp = factor * damage;
			data.level = level;
			data.exp = (int64_t)(exp * tmp + 0.5f);
			data.no_exp_punish = no_exp_punish;
			if(data.exp > 0)
			{
				MSG msg;
				BuildMessage(msg,GM_MSG_EXPERIENCE,list[i].who,list[i].who,pos,0,&data,sizeof(data));
				gmatrix::SendMessage(msg);
			}
		}
	}

	if(!member_damage || !mlist_count || total_level <= 0) return ;
	if(member_damage < total_damage)
	{
		factor *= member_damage;
		exp = (int64_t)( factor * exp + 0.5f);
	}

	//考虑所有的玩家数据 中间过滤了上面已经处理了的
	for(int i = 0; i < _team.GetMemberNum(); i ++)
	{
		if(flag_list[i]) continue;
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.pos.squared_distance(pos) > TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE || npctag !=ent.data.world_tag || ent.data.is_zombie)
		{
			continue;
		}
		mlist[mlist_count] = i;
		mlist_count ++;
		int level = ent.data.level;
		total_level += level;
		if(level > max_level) max_level = level;
		if(level < min_level) min_level = level;
	}

	_team.DispatchExp(pos,mlist,mlist_count, exp,level,total_level,max_level, min_level,npcid,r, no_exp_punish);
}

void 
gplayer_imp::ReceiveShareExp(int64_t exp )
{
	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustNormalExp(_wallow_level, exp);
	}
	if(exp) 
	{
		AddExp(exp, 0);
		_runner->receive_exp(exp);
	}
}


void 
gplayer_imp::ReceiveExp(const msg_exp_t & entry)
{
	int64_t exp = entry.exp;
	float exp_adj = 0;
	bool bReborn = GetRebornCount() > 0 ? true: false;
	player_template::GetExpPunishment(_basic.level - entry.level,&exp_adj, bReborn, entry.no_exp_punish);
	exp = (int64_t)(exp * exp_adj + 0.5f);

	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustNormalExp(_wallow_level, exp);
	}
	if(exp) ReceiveExp(exp);
}

void 
gplayer_imp::AddExp(int64_t &exp ,int64_t ext_exp)
{
	if(_basic.level >= player_template::GetMaxLevel(GetRebornCount()))
	{	
		//达到最大级别
		exp = 0;
		return ;
	}
	if(exp <= 0) return;
	
	int64_t nexp = exp + ext_exp;
	
	//物品多倍经验 系统多倍经验 VIP多倍经验中取最大值
	float exp_ratio = 1.0f;
	float item_multiple_exp = GetMultiExp();			//物品
	float sys_multiple_exp = gmatrix::GetWorldParam().get_multiple_exp();	//系统颁发
	float vip_multiple_exp = GetExtraExpRatio();	//vip卡
	float fac_multiple_exp = GetFacMultiExp();
	if(item_multiple_exp > exp_ratio) exp_ratio = item_multiple_exp;
	if(sys_multiple_exp > exp_ratio) exp_ratio = sys_multiple_exp;
	if(vip_multiple_exp > exp_ratio) exp_ratio = vip_multiple_exp;
	if(fac_multiple_exp > exp_ratio) exp_ratio = fac_multiple_exp;

	//国王帮成员享受额外的0.5倍经验加成 
	if(IsKingdomPlayer()) exp_ratio += 0.5;

	//考虑浮点数误差
	if(exp_ratio > 1.0001)
	{
		nexp =(int64_t)(nexp * exp_ratio);
		GLog::bot("用户%d得到经验(%f倍) %lld(+%lld)",_parent->ID.id, exp_ratio, (int64_t)(exp*exp_ratio),(int64_t)(ext_exp*exp_ratio)); 
	}
	else
	{
		GLog::bot("用户%d得到经验%lld(+%lld)",_parent->ID.id,exp,ext_exp);
	}

	if(nexp <= 0) return;
	
	exp = nexp;

	SetRefreshState();
	_basic.exp += nexp;
	int64_t next_exp = player_template::GetLvlupExp(GetRebornCount(), _basic.level);
	if(next_exp > _basic.exp)
	{
		return ;
	}
	LevelUp();
}

void
gplayer_imp::IncExp(int64_t &exp)
{
	if(_basic.level >= player_template::GetMaxLevel(GetRebornCount()))
	{	
		//达到最大级别
		exp = 0;
		return ;
	}
	
	if(exp <= 0) return;
	
	SetRefreshState();
	_basic.exp += exp;
	GLog::log(GLOG_INFO,"用户%d得到经验%lld",_parent->ID.id,exp);
	int64_t next_exp = player_template::GetLvlupExp(GetRebornCount(), _basic.level);
	if(next_exp > _basic.exp)
	{
		return ;
	}

	LevelUp();
}

void 
gplayer_imp::IncDeityExp(int &exp)
{
	if(_basic.dt_level <=0 || _basic.dt_exp > MAX_DEITY_EXP)
	{
		exp = 0;
		return ;
	}
	if(exp <= 0) return;

	if(exp + _basic.dt_exp > MAX_DEITY_EXP)
	{
		exp = MAX_DEITY_EXP - _basic.dt_exp;
	}

	_basic.dt_exp += (int64_t)exp;
	SetRefreshState();
	GLog::log(GLOG_INFO,"用户%d得到封神经验%d",_parent->ID.id,exp);
}

bool
gplayer_imp::DecDeityExp(int exp)
{
	if(_basic.dt_level <=0 || _basic.dt_exp < exp)
	{
		return false;
	}
	if(exp <= 0) return false;
	
	_basic.dt_exp -= (int64_t)exp;
	SetRefreshState();
	GLog::log(GLOG_INFO,"用户%d消耗封神经验%d",_parent->ID.id,exp);
	return true;
}

void
gplayer_imp::DeityLevelUp()
{
	if(_basic.dt_level <= 0 || _basic.dt_level >= player_template::GetMaxDeityLevel() )
	{
		return;
	}

	int64_t	exp = player_template::GetDeityLvlupExp(_basic.dt_level);
	int64_t	fail_exp = player_template::GetDeityLvlupPunishExp(_basic.dt_level);
	float succ_prob = player_template::GetDeityLvlupProb(_basic.dt_level);

	if(_basic.dt_exp < exp) return;
	if(_basic.dt_exp < fail_exp) return;

	//每9级一次的转重通过任务实现
	if((_basic.dt_level % MAX_DEITY_CLEVEL) == 0) return; 

	//升级成功
	if(abase::RandUniform() <  succ_prob)
	{
		int cls = GetPlayerClass();
		if(!player_template::DeityLevelUp(cls,_basic.dt_level,this))
		{
			GLog::log(GLOG_ERR,"用户%d封神升级时发生错误,职业%d,封神级别%d",_parent->ID.id,cls,_basic.dt_level);
			PlayerForceOffline();
			return ;
		}
		
		_basic.dt_exp -= exp;
		_basic.dt_level++;
		
		property_policy::UpdatePlayer(cls,this);
		if(!((gplayer*)_parent)->IsZombie())
		{
			_basic.dp = GetMaxDP();
		}
		GetParent()->dt_level = (unsigned char) _basic.dt_level;

		_runner->deity_levelup(1);

		_talent_point = player_template::GetTalentPoint(GetRebornCount(), _basic.level, _basic.dt_level);
		_talent_point -= _skill.GetTalentSum();
		_runner->player_talent_notify(_talent_point);

		_basic.status_point = player_template::GetSkillPoint(GetRebornCount(), _basic.level, _basic.dt_level);
		_basic.status_point -= _skill.GetSpSum();

		PlayerTaskInterface  task_if(this);
		OnTaskPlayerLevelUp(&task_if);
		SetRefreshState();
		GLog::log(GLOG_INFO,"用户%d元神等级升级到%d级,消耗修为%lld",_parent->ID.id, _basic.dt_level, exp);
	}
	//升级失败
	else
	{
		_basic.dt_exp -= fail_exp;
		_runner->deity_levelup(0);
		SetRefreshState();
		GLog::log(GLOG_INFO,"用户%d升级元神等级失败,当前等级为%d, 消耗修为%lld",_parent->ID.id, _basic.dt_level, fail_exp);
	}	
	
}
	
void 
gplayer_imp::DeityDuJie()
{
	if(_basic.dt_level < 0 || _basic.dt_level >= player_template::GetMaxDeityLevel() )
	{
		GLog::log(GLOG_ERR,"用户%d封神升级时发生错误,dt_level=%d", _parent->ID.id, _basic.dt_level);
		return;
	}

	//每9级一次的转重通过任务实现
	if((_basic.dt_level % MAX_DEITY_CLEVEL) != 0) 
	{
		GLog::log(GLOG_ERR,"用户%d封神升级时发生错误,dt_level=%d", _parent->ID.id, _basic.dt_level);
		return; 
	}

	int cls = GetPlayerClass();
	if(!player_template::DeityLevelUp(cls,_basic.dt_level,this))
	{
		GLog::log(GLOG_ERR,"用户%d封神升级时发生错误,职业%d,封神级别%d",_parent->ID.id,cls,_basic.dt_level);
		PlayerForceOffline();
		return ;
	}

	//检查经验是否够
	int64_t	exp = player_template::GetDeityLvlupExp(_basic.dt_level);
	if(_basic.dt_exp < exp) 
	{
		GLog::log(GLOG_ERR,"用户%d封神升级时发生错误经验不够, exp=%lld, need_exp=%lld", _parent->ID.id,_basic.dt_exp, exp);
		return;
	}

	_basic.dt_exp -= exp;
	_basic.dt_level++;

	property_policy::UpdatePlayer(cls,this);
	if(!((gplayer*)_parent)->IsZombie())
	{
		_basic.dp = GetMaxDP();
	}
	GetParent()->dt_level = (unsigned char) _basic.dt_level;

	_runner->deity_levelup(1);

	_talent_point = player_template::GetTalentPoint(GetRebornCount(), _basic.level, _basic.dt_level);
	_talent_point -= _skill.GetTalentSum();
	_runner->player_talent_notify(_talent_point);

	_basic.status_point = player_template::GetSkillPoint(GetRebornCount(), _basic.level, _basic.dt_level);
	_basic.status_point -= _skill.GetSpSum();

	PlayerTaskInterface  task_if(this);
	OnTaskPlayerLevelUp(&task_if);
	SetRefreshState();
	GLog::log(GLOG_INFO,"用户%d元神等级升级到%d级,消耗修为%lld",_parent->ID.id, _basic.dt_level, exp);

}


void 
gplayer_imp::ReceiveExp(int64_t exp)
{
	ASSERT(exp >=0 );
	//计算经验，对经验进行修正
	//首先是装备经验加成
	float addon = _exp_addon * 0.001f;;
	if(addon >=1.0f) addon = 1.0f;
	int64_t ext_exp = (int)((exp * addon) + 0.1f);
	//进行双倍经验判定
	int fid = GetParent()->id_family;
	if(fid > 0)
	{
		GMSV::SendFamilyExpShare(_parent->ID.id,  fid, _basic.level, exp);
	}
	
	AddExp(exp, ext_exp);
	_runner->receive_exp(exp);
	return;
}


bool 
gplayer_imp::CanAttack(const XID & target)
{
	//现在任何地方都可以进行普通攻击
	if(!_layer_ctrl.CheckAttack()) return false;
	return _equipment[item::EQUIP_INDEX_WEAPON].CheckAttack(_equipment);
}

bool
gplayer_imp::StandOnGround()
{
	return _ph_control.OnGround();
}

int 
gplayer_imp::DoAttack(const XID & target, char force_attack)
{
	ActiveCombatState(true);
	_combat_timer = MAX_COMBAT_TIME;
	
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack,0);
	
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack, 0);
	gmatrix::SendMessage(msg);
	NotifyStartAttack(target,force_attack);
	OnAttack();
	return 0;
}

/*
void gplayer_imp::SendExtraAttackMsg( const XID & target, attack_msg & attack )
{
	if( _fashion_weapon_addon.arg[0] > 0 )
	{
		attack.skill_id = 223;
		//SendAttackMsg( target, attack );
	}
}
*/

void 
gplayer_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gactive_imp::FillAttackMsg(target,attack);
	attack.ainfo.sid = GetParent()->cs_sid;
	attack.ainfo.cs_index = GetParent()->cs_index;
	if(!_pvp_flag_enable) attack.force_attack = 0; 	//如果处于PK保护，则强行关闭强行攻击开关
	attack.ainfo.mafia_id = ((gplayer*)_parent)->id_mafia;
	attack.ainfo.family_id = ((gplayer*)_parent)->id_family;
	attack.ainfo.master_id = GetSectID() == attack.ainfo.attacker.id ? 0 : GetSectID();
	attack.ainfo.invisible_rate = ((gplayer*)_parent)->invisible_rate;

	attack.attacker_mode = _pvp_flag_enable?attack_msg::PVP_ENABLE:0; 
	if(IsInPVPCombatStateHigh()) attack.attacker_mode |= attack_msg::PVP_DURATION;

	_team.GetTeamID(attack.ainfo.team_id, attack.ainfo.team_seq);
	attack.ainfo.eff_level = _team.GetEffLevel();
	attack.ainfo.wallow_level = _team.GetWallowLevel();
	attack.cultivation = _cultivation & 0xFF;
	DoWeaponOperation<0>();

	gplayer * pPlayer = GetParent();
	if(pPlayer->IsFlyMode()) attack.is_flying = 1;
	if(IsRenMa()) attack.feedback_mask |= GNET::FEEDBACK_DAMAGE;
	attack.fashion_weapon_addon_id = _fashion_weapon_addon_id;
}

void 
gplayer_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gactive_imp::FillEnchantMsg(target,enchant);
	enchant.ainfo.sid = GetParent()->cs_sid;
	enchant.ainfo.cs_index = GetParent()->cs_index;
	if(!_pvp_flag_enable) enchant.force_attack = 0; 	//如果处于PK保护，则强行关闭强行攻击开关
	enchant.ainfo.mafia_id = ((gplayer*)_parent)->id_mafia;
	enchant.ainfo.family_id = ((gplayer*)_parent)->id_family;
	enchant.ainfo.master_id = GetSectID() == enchant.ainfo.attacker.id ? 0 : GetSectID();

	enchant.attacker_mode = _pvp_flag_enable?attack_msg::PVP_ENABLE:0; 
	if(IsInPVPCombatStateHigh()) enchant.attacker_mode |= attack_msg::PVP_DURATION;


	_team.GetTeamID(enchant.ainfo.team_id, enchant.ainfo.team_seq);
	enchant.ainfo.eff_level = _team.GetEffLevel();
	enchant.ainfo.wallow_level = _team.GetWallowLevel();

	gplayer * pPlayer = GetParent();
	if(pPlayer->IsFlyMode()) enchant.is_flying = 1;
	//DoWeaponOperation<0>();  这里不能调用，否则会出错
}

void 
gplayer_imp::SendAttackMsg(const XID & target, attack_msg & attack)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack,0);
	gmatrix::SendMessage(msg);
}

void 
gplayer_imp::SendEnchantMsg(int message,const XID & target, enchant_msg & attack)
{
	SendTo<0>(message,target,0,&attack,sizeof(attack));
}

//Tag by Houjun 2011-09-23, Instance Related Logic
void
gplayer_imp::SwitchSvr(int dest_tag, const A3DVECTOR & oldpos, const A3DVECTOR &newpos, const instance_key * switch_key)
{
	__PRINTF("switch\n");
	if(_pstate.CanSwitch() && _switch_dest == -1)
	{
		//设置自己的转移目标和转移地点
		_switch_dest = dest_tag;
		_switch_pos = newpos;
		if(switch_key) 
			_instance_switch_key = *switch_key;
		else
			memset(&_instance_switch_key,0,sizeof(_instance_switch_key));

		//开始发出转移的消息
		MSG msg;
		BuildMessage(msg,GM_MSG_SWITCH_START,XID(GM_TYPE_MANAGER,dest_tag),_parent->ID,newpos,
				_plane->GetTag(),&_instance_switch_key,sizeof(_instance_switch_key));
		gmatrix::SendWorldMessage(dest_tag, msg);
	}
}


void 
gplayer_imp::OnDeath(const XID & lastattack,bool is_pariah, bool faction_battle, bool is_hostile_duel, int time)
{
	__PRINTF("player %d dead \n",_parent->ID.id);
	if((gplayer*)_parent->IsZombie())
	{
		//已经死亡，不应该再次调用了
		return ;
	}

	if(IsSitDownMode())
	{
		LeaveStayInState();
	}

	if(_pstate.IsMarketState())
	{
		CancelPersonalMarket();
	}
	else if(_pstate.IsBindState())
	{
		gplayer *pPlayer = GetParent();
		//如果是leader死亡， 在Die函数中remove filter的时候会利用下马逻辑解除绑定
		//如果是 member死亡, 调用专门的接口保证先解绑， 然后处理死亡逻辑
		if(pPlayer->bind_type == 4){
			_link_ride_player.OnMemberDeath(this);
		}
		else if(pPlayer->bind_type == 1 || pPlayer->bind_type == 2){
			_bind_player.PlayerLinkCancel(this);
		}
		else if(pPlayer->bind_type == 7 || pPlayer->bind_type == 8){
			_link_qilin_player.PlayerLinkCancel(this);
		}
	}
	
	_mobactiveman.PlayerMobActiveCancel(this);

	bool bCanDrop = _pstate.CanDeathDrop();

	bool no_exp_drop = false;
	bool no_drop1 = faction_battle;
	bool poppet_actived= false;

	//自杀
	if(GetParent()->ID.id == lastattack.id)
	{
		no_exp_drop = true;
		no_drop1 = true;
	}

	if(!no_drop1)
	{
		bool has_poppet = (_item_poppet_counter > 0) || (_exp_poppet_counter > 0);
		if( !(_free_pvp_mode && lastattack.IsPlayerClass()) && has_poppet)
		{
			//试图删除item_poppet
			int index = _inventory.FindByType(0,item_body::ITEM_TYPE_POPPET);
			if(index >= 0)
			{
				//检查是否需要记录消费值
				CheckSpecialConsumption(_inventory[index].type, 1);

				DecInventoryItem(index, 1, S2C::DROP_TYPE_TAKEOUT);
				GLog::log(GLOG_INFO, "用户%d替身娃娃生效",GetParent()->ID.id);
				no_drop1 = true;
				no_exp_drop = true;
				poppet_actived= true;
			}
			else
			{
				//未找到替身娃娃 
				ASSERT(false);
			}
		}
	}
	else
	{
		if( !is_hostile_duel )
		{
			no_exp_drop = true;
		}
	}

	if(!_free_pvp_mode && lastattack.IsPlayerClass() && !faction_battle && GetParent()->ID.id != lastattack.id)
	{
		//发送仇人信息
		GMSV::SendUpdateEnemy(GetParent()->ID.id, lastattack.id);
	}

	if(lastattack.IsPlayerClass())
	{
		//被玩家杀死的 执行损失威望逻辑
		//威望损失逻辑目前被禁止了
		//SendTo<0>(GM_MSG_REPU_CHG_STEP_1, lastattack, GetReputation());
	}
	else
	{
		//是NPC杀死的检测是否可以掉落物品 和减少经验
		world::object_info info;
		if(_plane->QueryObject(lastattack,info))
		{
			//找到了对象，按照对象数据来完成
			if(npc_template * pTemplate = npc_stubs_manager::Get(info.tid))
			{
				if(!no_drop1)
				{
					//如果要掉下物品才会使用怪物的掉落逻辑（因为可能有替身娃娃)
					no_drop1 = !pTemplate->kill_drop;
				}

				if(!no_exp_drop)
				{
					//如果要损失经验才会使用怪物损失经验逻辑（因为可能有替身娃娃)
					no_exp_drop = !pTemplate->kill_exp;
				}
			}
			else
			{
				//找不到模板就肯定不掉落
				no_drop1 = true;
			}
		}
		else
		{
			//找不到怪物就肯定不掉落
			no_drop1 = true;
		}
	}

	if(!no_exp_drop)
	{
		if(IsNoExpDrop()) no_exp_drop = true;
	}
	
	if(bCanDrop)
	{
		//检测是否应该掉落物品
		if(!no_drop1)
		{
			//如果是玩家杀死， 则试图掉落物品 注意只有非PVP mode才会掉落物品
			if(!_free_pvp_mode)
			{

				int eq_drop;
				int inv_drop;
				// 这里应该对敌对帮派战斗和普通情况分开处理
				if( is_hostile_duel )
				{
					eq_drop = 0;
					player_template::GetHostileDuleDeathDropRate( inv_drop );
				}
				else
				{
					player_template::GetDeathDropRate(_pk_level,eq_drop,inv_drop);
				}
				if(eq_drop + inv_drop > 0)
				{
					int team_id = 0;
					int team_seq = 0;
					XID owner(0,0);
					
					/*
					TMD designer
					if(!_dmg_list.empty())
					{
						GetMaxHurtTeam(owner, team_id, team_seq);
					}
					*/
					DropItemOnDeath(inv_drop ,eq_drop,owner, team_id, team_seq);
				}
			}
		}
	}
	
	ClearSession();
	ClearSpiritSession();
	ActiveCombatState(false);		//这里会清除攻击列表，所以在掉落物品判定完成之后进行

	//宠物接口
	_petman.OnMasterDeath(this);

	if(!IsRenMa())_basic.mp = 0;
	_basic.dp = 0;
	//发送杀死玩家的消息
	if(lastattack.type == GM_TYPE_PLAYER && !_free_pvp_mode)
	{
		msg_player_kill_player msg_kill;
		msg_kill.bpKilled = (is_pariah && (_invader_counter == 0 || GetPKValue() > 0));
		msg_kill.noDrop = no_drop1;
		msg_kill.mafia_id = OI_GetMafiaID();
		msg_kill.pkvalue = GetPKValue();
		SendTo<0>(GM_MSG_PLAYER_KILL_PLAYER, lastattack, 0,&msg_kill, sizeof(msg_kill));
		
		if(msg_kill.mafia_id && faction_battle)
		{
			_mduel.OnDeath(msg_kill.mafia_id, GetParent()->ID, lastattack);
		}
	}
	else if(lastattack.type == GM_TYPE_NPC)
	{
		//被NPC杀死，回馈一个消息
		SendTo<0>(GM_MSG_PLAYER_KILLED_BY_NPC,lastattack,0);
	}

	//发送全服杀人广播消息
	if(lastattack.type == GM_TYPE_PLAYER && !_duel.IsDuelMode() && lastattack.id != _parent->ID.id)
	{
		SendPKMessage(lastattack.id, _parent->ID.id);
	}

	//死亡的操作，进入Zombie状态
	_parent->b_zombie = true;
	if(lastattack.IsPlayerClass()) _kill_by_player = true; else _kill_by_player = false;
	
	if(no_exp_drop)
	{
		//如果是帮战或者
		//如果有经验替身娃娃 则认为是玩家杀死的， 不损失经验
		_kill_by_player = true;
	}

	//注意在竞技场里下线复活可能会有不应该有的经验损失 但是由于是被玩家杀死的 所以还是不会有经验损失的
	if(_free_pvp_mode && (lastattack.IsPlayerClass() || !CanDropExpPVPMode()))
	{
		//只要是自由PVP模式，则都认为是被玩家杀死，这样不会有复活经验损失
		_kill_by_player = true;
	}

	_resurrect_state = false;
	_resurrect_exp_reduce = 1.0f;
	_resurrect_hp_recover = poppet_actived?1.0:0.1f;

	if(time > 0) _resurrect_timestamp = g_timer.get_systime() + time;

	//发送死亡消息
	_runner->on_death(lastattack, false, time);
	int death_type = lastattack.type&0xFF;
	if(is_pariah) death_type |= 0x100;
	GLog::die(_parent->ID.id,death_type,lastattack.id);
	GLog::log(GLOG_INFO,"用户%d被%d杀死，类别(%d)",_parent->ID.id,lastattack.id,death_type);

	//进行任务的处理
	PlayerTaskInterface task_if(this);
	OnTaskPlayerKilled(&task_if);
	
	GetAchievementManager().OnDeath(this);

	//设置死亡后自动定时复生的session
//	session_player_reborn * pSession = new session_player_reborn(this);
//	pSession->SetDelay(50*10);

}

int 
gplayer_imp::PlayerGetItemInfo(int where, int index)
{
	int rst;
	item_data_client data;
	switch(where)
	{       
		case IL_INVENTORY:
			rst = _inventory.GetItemDataForClient(index,data);
			break;
		case IL_EQUIPMENT:
			rst = _equipment.GetItemDataForClient(index,data);
			break;
		case IL_TASK_INVENTORY:
			rst = _task_inventory.GetItemDataForClient(index,data);
			break;
		case IL_PET_BEDGE:
			rst = _pet_bedge_inventory.GetItemDataForClient(index,data);
			break;
		case IL_PET_EQUIP:
			rst = _pet_equip_inventory.GetItemDataForClient(index,data);
			break;
		case IL_TRASH_BOX:
			if(IsTrashBoxOpen())
				rst = _trashbox.GetCurBackpack().GetItemDataForClient(index,data);
			else
				rst = -1;
			break;
		case IL_POCKET:
			rst = _pocket_inventory.GetItemDataForClient(index, data);
			break;
		case IL_FASHION:
			rst = _fashion_inventory.GetItemDataForClient(index, data);
			break;
		case IL_MOUNT_WING:
			rst = _mount_wing_inventory.GetItemDataForClient(index, data);
			break;
		case IL_GIFT:
			rst = _gift_inventory.GetItemDataForClient(index, data);
			break;
		case IL_FUWEN:
			rst = _fuwen_inventory.GetItemDataForClient(index, data);
			break;
		default:
			rst = -1;
	}
	if(rst == 0)
	{
		//不存在
		_runner->self_item_empty_info(where,index);
	}else if(rst > 0)
	{
		//找到了
		_runner->self_item_info(where,index,data,0);
	}

	return rst;
}

int 
gplayer_imp::PlayerGetItemInfoList(int where, size_t count , unsigned char *list)
{
	item_list &inv = GetInventory(where);

	size_t cap = inv.Size();
	for(size_t i = 0; i < count; i ++)
	{
		size_t index = list[i];
		if(index < cap)
		{

			item_data_client data;
			if(inv.GetItemDataForClient(index,data) > 0)
			{
				_runner->self_item_info(where,index,data,0);
			}
		}
	}
	

	return 0;
}

void 
gplayer_imp::TrashBoxOpen(int type)
{
	_trashbox.Open(type);
	_runner->trashbox_open(GetTrashBoxOpenType());
	GLog::log(GLOG_INFO,"用户%d开启了仓库",_parent->ID.id);
}

void 
gplayer_imp::TrashBoxClose()
{
	_trashbox.Close();
	_runner->trashbox_close();
	GLog::log(GLOG_INFO,"用户%d关闭了仓库",_parent->ID.id);
}

void
gplayer_imp::PlayerGetTrashBoxInfo(bool detail)
{
	item_list & list = _trashbox.GetCurBackpack();
	if(detail)
	{
		raw_wrapper rw(1024);
		list.DetailSave(rw);
		_runner->self_inventory_detail_data(GetTrashBoxOpenType(),list.Size(),rw.data(),rw.size());
	}
	else
	{
		abase::octets buf;
		list.SimpleSave(buf);
		_runner->self_inventory_data(GetTrashBoxOpenType(), list.Size() , buf.begin(), buf.size());
	}
	_runner->trashbox_wealth(_trashbox.GetMoney());
}

void 
gplayer_imp::PlayerGetInventoryDetail(int where)
{
	raw_wrapper rw;
	unsigned char size;
	switch(where)
	{
		case IL_INVENTORY:
			_inventory.DetailSave(rw);
			size = _inventory.Size();
			break;
		case IL_EQUIPMENT:
			_equipment.DetailSave(rw);
			size = _equipment.Size();
			break;
		case IL_TASK_INVENTORY:
			_task_inventory.DetailSave(rw);
			size = _task_inventory.Size();
			break;
		case IL_PET_BEDGE:
			_pet_bedge_inventory.DetailSave(rw);
			size = _pet_bedge_inventory.Size();
			break;
		case IL_PET_EQUIP:
			_pet_equip_inventory.DetailSave(rw);
			size = _pet_equip_inventory.Size();
			break;
		case IL_POCKET:
			_pocket_inventory.DetailSave(rw);
			size = _pocket_inventory.Size();
			break;
		case IL_FASHION:
			_fashion_inventory.DetailSave(rw);
			size = _fashion_inventory.Size();	
			break;
		case IL_MOUNT_WING:
			_mount_wing_inventory.DetailSave(rw);
			size = _mount_wing_inventory.Size();
			break;
		case IL_GIFT:
			_gift_inventory.DetailSave(rw);
			size = _gift_inventory.Size();
			break;
		case IL_FUWEN:
			_fuwen_inventory.DetailSave(rw);
			size = _fuwen_inventory.Size();
			break;
		default:
		ASSERT(false);
		return;
	}
	_runner->self_inventory_detail_data(where,size, rw.data(),rw.size());
}

void gplayer_imp::PlayerGetItemExchangeList(unsigned char where, const std::vector<exchange_item_info>& vec_exchange_info)
{
	raw_wrapper rw;
	for (size_t ind = 0; ind < vec_exchange_info.size(); ind++)
	{
		const struct exchange_item_info & eti = vec_exchange_info[ind];
		if (eti.isrc != eti.idst)
		{
			rw << eti.isrc;
			rw << eti.idst;
		}
	}
	_runner->exchange_item_list_info(where,rw.data(),rw.size());
}

void
gplayer_imp::PlayerExchangeInvItem(size_t idx1,size_t idx2)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(!_inventory.ExchangeItem(idx1,idx2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->exchange_inventory_item(idx1,idx2);
}

void 
gplayer_imp::PlayerMoveInvItem(size_t src,size_t dest,size_t count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(!_inventory.MoveItem(src,dest,&count))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->move_inventory_item(src,dest,count);
}

void
gplayer_imp::PlayerExchangeTrashItem(size_t idx1,size_t idx2)
{
	if(!_trashbox.GetCurBackpack().ExchangeItem(idx1,idx2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->exchange_trashbox_item(GetTrashBoxOpenType(),idx1,idx2);
}

void 
gplayer_imp::PlayerMoveTrashItem(size_t src,size_t dest,size_t count)
{
	if(!_trashbox.GetCurBackpack().MoveItem(src,dest,&count))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->move_trashbox_item(GetTrashBoxOpenType(),src,dest,count);
}


void 
gplayer_imp::PlayerExchangeTrashInv(size_t idx_tra, size_t idx_inv)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	item_list & box = _trashbox.GetCurBackpack();
	if(idx_tra >= box.Size() || idx_inv >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	//if(!_inventory[idx_inv].CanMove()|| !box[idx_tra].CanMove())
	if(!_inventory[idx_inv].CanMove()|| !_inventory[idx_inv].CanPutToTrash() || !box[idx_tra].CanMove())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	
	item it;
	box.Exchange(idx_tra,it);
	_inventory.Exchange(idx_inv,it);
	box.Exchange(idx_tra,it);
	_runner->exchange_trashbox_inventory(GetTrashBoxOpenType(),idx_tra,idx_inv);
}

void 
gplayer_imp::PlayerExchangeTrashMoney(size_t inv_money, size_t tra_money)
{
	if(!_trashbox.IsNormalBoxOpen())
	{
		//如果不是正常的仓库，无法操作金钱
		return;
	}

	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if( (inv_money && tra_money) || (!inv_money && !tra_money) || inv_money > GetMoney()  || tra_money > _trashbox.GetMoney())  
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if(inv_money)
	{
		//钱挪到钱箱
		size_t delta = inv_money;
		size_t tm = _trashbox.GetMoney();
		size_t rst = tm + delta;
		if(rst < tm || rst > TRASHBOX_MONEY_CAPACITY)
		{
			delta = TRASHBOX_MONEY_CAPACITY - tm;
		}
		_trashbox.GetMoney() += delta;
		SpendMoney(delta);
		GLog::log(GLOG_INFO,"用户%d仓库放入金钱%d",_parent->ID.id,delta);
		_runner->exchange_trash_money(-(int)delta,delta);
	}
	else
	{
		//从钱箱里拿出钱

		//首先检查包裹栏是否足够
		if(!CheckIncMoney(tra_money))
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return ;
		}

		//只有钱够的时候才会加入金钱
		size_t delta = _player_money;
		GainMoney(tra_money);
		delta = _player_money - delta;
		_trashbox.GetMoney() -= delta;
		GLog::log(GLOG_INFO,"用户%d仓库移出金钱%d",_parent->ID.id,delta);
		_runner->exchange_trash_money(delta,-(int)delta);
	}
}

void 
gplayer_imp::PlayerTrashItemToInv(size_t idx_tra, size_t idx_inv, size_t count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	int delta = MoveBetweenItemList(_trashbox.GetCurBackpack(),_inventory,idx_tra,idx_inv,count);
	if(delta >= 0)
	{
		_runner->trash_item_to_inventory(GetTrashBoxOpenType(),idx_tra,idx_inv,delta);
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
}

void 
gplayer_imp::PlayerInvItemToTrash(size_t idx_inv, size_t idx_tra, size_t count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	
	if( idx_inv >= _inventory.Size() || !_inventory[idx_inv].CanPutToTrash() )
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	int delta = MoveBetweenItemList(_inventory,_trashbox.GetCurBackpack(),idx_inv,idx_tra,count);
	if(delta >= 0)
	{
		_runner->inventory_item_to_trash(GetTrashBoxOpenType(),idx_inv,idx_tra,delta);
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
}

void 
gplayer_imp::PlayerDropInvItem(size_t index,size_t count,bool isProtected)
{
	if(index >= _inventory.Size() || !count)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(GetWorldManager()->GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(_inventory[index].type == -1)
	{
		_runner->player_drop_item(IL_INVENTORY,index,-1,0,S2C::DROP_TYPE_PLAYER);
		return ;
	}

	if(!_inventory[index].CanThrow())
	{
		//此物品无法丢出，是否报告错误？
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return;
	}

	//检查现在空闲的物品区域是否足够
	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	ThrowInvItem(index,count,isProtected,S2C::DROP_TYPE_PLAYER);
}

void 
gplayer_imp::ThrowInvItem(size_t index, size_t count, bool isProtected,unsigned char drop_type)
{
	item_data data;
	int rst = _inventory.GetItemData(index, data);
	if(rst <=0) 
	{
		ASSERT(false);
		return;
	}
	if(data.count > count)
	{
		data.count = count;
	}

	//设置随机的位置
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//如果超出了边界，那么就按照自己的位置来算
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	
	//必须现在丢出，不然当item释放后里面的content 可能就错乱了
	if(isProtected)
		DropItemFromData(_plane,pos,data,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id); 
	else
		DropItemFromData(_plane,pos,data,XID(0,0),0,0,_parent->ID.id); 

	GLog::log(LOG_INFO,"用户%d丢弃包裹%d个%d(type:%d)",_parent->ID.id,data.count,data.type,drop_type);

	_inventory.DecAmount(index,data.count);
	_runner->player_drop_item(IL_INVENTORY,index,data.type,data.count,drop_type);
}

void 
gplayer_imp::ThrowInvItem(size_t index, size_t count, unsigned char drop_type,XID owner, int team, int seq)
{
	item_data data;
	int rst = _inventory.GetItemData(index, data);
	if(rst <=0) 
	{
		ASSERT(false);
		return;
	}
	if(data.count > count)
	{
		data.count = count;
	}

	//设置随机的位置
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//如果超出了边界，那么就按照自己的位置来算
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	
	//必须现在丢出，不然当item释放后里面的content 可能就错乱了
	DropItemFromData(_plane,pos,data,owner,team, seq,_parent->ID.id); 
	GLog::log(LOG_INFO,"用户%d丢弃包裹%d个%d(type:%d)",_parent->ID.id,data.count,data.type,drop_type);

	_inventory.DecAmount(index,data.count);
	_runner->player_drop_item(IL_INVENTORY,index,data.type,data.count,drop_type);
}

void 
gplayer_imp::PlayerDropEquipItem(size_t index,bool isProtected)
{
	if(index >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
		
	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	if(GetWorldManager()->GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(_equipment[index].type == -1)
	{
		_runner->player_drop_item(IL_EQUIPMENT,index,-1,0,S2C::DROP_TYPE_PLAYER);
		return ;
	}
	if(!_equipment[index].CanThrow())
	{
		//此物品无法丢出，是否报告错误？
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return;
	}

	//检查现在空闲的物品区域是否足够
	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}
	ThrowEquipItem(index,isProtected,S2C::DROP_TYPE_PLAYER);
}

void 
gplayer_imp::ThrowEquipItem(size_t index, bool isProtected,unsigned char drop_type)
{
	item_data data;
	int rst = _equipment.GetItemData(index, data);
	if(rst <=0) 
	{
		ASSERT(false);
		return;
	}

	//设置装备改变标志
	IncEquipChangeFlag();

	//设置随机的位置
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//如果超出了边界，那么就按照自己的位置来算
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	//必须现在丢出，不然当item释放后里面的content 可能就错乱了
	if(isProtected)
		DropItemFromData(_plane,pos,data,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id); 
	else
		DropItemFromData(_plane,pos,data,XID(0,0),0,0,_parent->ID.id); 

	GLog::log(LOG_INFO,"用户%d丢弃装备%d(type:%d)",_parent->ID.id,data.type,drop_type);
	_equipment.Remove(index);
	_runner->player_drop_item(IL_EQUIPMENT,index,data.type,data.count,drop_type);

	//重新刷新装备
	RefreshEquipment();
	
	//发出更改信息
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1<<index,0,0); //此函数使用了CalcEquipmentInfo的结果
}

void 
gplayer_imp::ThrowEquipItem(size_t index, unsigned char drop_type,XID owner, int team, int seq)
{
	item_data data;
	int rst = _equipment.GetItemData(index, data);
	if(rst <=0) 
	{
		ASSERT(false);
		return;
	}

	//设置装备改变标志
	IncEquipChangeFlag();

	//设置随机的位置
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//如果超出了边界，那么就按照自己的位置来算
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	//必须现在丢出，不然当item释放后里面的content 可能就错乱了
	DropItemFromData(_plane,pos,data,owner,team,seq,_parent->ID.id); 

	GLog::log(LOG_INFO,"用户%d丢弃装备%d(type:%d)",_parent->ID.id,data.type,drop_type);
	_equipment.Remove(index);
	_runner->player_drop_item(IL_EQUIPMENT,index,data.type,data.count,drop_type);

	//重新刷新装备
	RefreshEquipment();
	
	//发出更改信息
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1<<index,0,0); //此函数使用了CalcEquipmentInfo的结果
}


void 
gplayer_imp::PlayerExchangeEquipItem(size_t index1,size_t index2)
{
	size_t size = _equipment.Size();
	if(index1 >= size || index2 >= size)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	size_t type = ((_equipment[index1].type == -1)?1:0) + ((_equipment[index2].type == -1)?2:0);
	unsigned int mask1 = 1 << index1;
	unsigned int mask2 = 1 << index2;
	unsigned int mask_add = 0;
	unsigned int mask_del = 0;
	int64_t id[2];
	int count=0;
	switch(type)
	{
		case 0:
			//index1 有 index2有
			if(!(_equipment[index2].equip_mask & mask1) || !(_equipment[index1].equip_mask & mask2))
			{
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}
			mask_add = mask1 | mask2;
			mask_del = 0;
			count = 2;
			if(index1 < index2)
			{
				id[0] = _equipment[index1].type | _equipment[index1].GetIdModify();
				id[1] = _equipment[index2].type | _equipment[index2].GetIdModify();
			}
			else
			{
				id[1] = _equipment[index1].type | _equipment[index1].GetIdModify();
				id[0] = _equipment[index2].type | _equipment[index2].GetIdModify();
			}
		break;
		case 1:
			//index1 空 index 2有
			if(!(_equipment[index2].equip_mask & mask1))
			{
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}
			mask_add = mask1;
			mask_del = mask2;
			count = 1;
			id[0] = _equipment[index1].type | _equipment[index1].GetIdModify();
		break;
		case 2:
			//index2 空 index 1有
			if(!(_equipment[index1].equip_mask & mask2))
			{
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}
			mask_add = mask2;
			mask_del = mask1;
			count = 1;
			id[0] = _equipment[index2].type | _equipment[index2].GetIdModify();
		break;
		case 3:
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
	}

	IncEquipChangeFlag();

	_equipment.ExchangeItem(index1,index2);
	_runner->exchange_equipment_item(index1,index2);
	
	CalcEquipmentInfo();
	_runner->equipment_info_changed(mask_add,mask_del,id, count * sizeof(int64_t)); //此函数使用了CalcEquipmentInfo的结果
}

void 
gplayer_imp::PlayerEquipItem(size_t index_inv, size_t index_equip)
{
	if(index_inv >= _inventory.Size() || index_equip >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	bool type1 = (_inventory[index_inv].type == -1);
	bool type2 = (_equipment[index_equip].type == -1);
	if(type1 && type2)
	{
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}
	
	if(!type1)
	{
		//包裹栏非空
		if(!(_inventory[index_inv].equip_mask & (1 << index_equip)))
		{	
			//装备位置不对
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}

		item& itemInv = _inventory[index_inv];
		if(itemInv.body->IsHolyLevelup(&itemInv) && GetRebornCount() <= 0)
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return ;
		}

		//交换之
		if(!EquipItem(index_inv,index_equip))
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		}
		else
		{
			//交换成功，检查一下是否需要进行绑定操作
			item & it = _equipment[index_equip];
			ASSERT(it.type != -1);
			bool notify_equip_item = false;
			if(it.IsBindOnEquip())
			{
				//满足绑定条件，进行绑定
				it.BindOnEquip();
				notify_equip_item = true;
				GLog::log(LOG_INFO,"用户%d装备绑定物品%d,GUID(%d,%d)",_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
				//检查是否需要记录消费值
				CheckEquipConsumption(it.type);
			}
		
			int count1 = _inventory[index_inv].count;
			int count2 = _equipment[index_equip].count;
			ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
			_runner->equip_item(index_inv,index_equip,count1,count2);
			int64_t id1 = _equipment[index_equip].type | _equipment[index_equip].GetIdModify();
			CalcEquipmentInfo();
			_runner->equipment_info_changed(1<<index_equip , 0, &id1,sizeof(id1));//此函数使用了CalcEquipmentInfo的结果
			if(notify_equip_item)
			{
				PlayerGetItemInfo(IL_EQUIPMENT,index_equip);
			}

		}
		IncEquipChangeFlag();
		return ;
	}
	//包裹栏为空，拿下原来的，并刷新装备
	item  it1;
	_equipment.Remove(index_equip,it1);
	bool bRst = _inventory.Put(index_inv,it1);
	ASSERT(bRst);
	if(bRst)
	{
		it1.Clear();
	}
	else
	{
		//记录错误日志
		GLog::log(GLOG_ERR,"装备物品时发生致命错误");
		it1.Release();
	}
	RefreshEquipment();
	int count1 = _inventory[index_inv].count;
	ASSERT(count1 > 0);
	_runner->equip_item(index_inv,index_equip,count1,0);
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1<<index_equip , 0, 0); //此函数使用了CalcEquipmentInfo的结果
	IncEquipChangeFlag();
	
}

void 
gplayer_imp::PlayerMoveEquipItem(size_t index_inv, size_t index_equip)
{
	if(index_inv >= _inventory.Size() || index_equip >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}
	
	const item & iteq = _equipment[index_equip];
	if(iteq.type == -1) 
	{
		//装备栏对应位置无内容，则等同于装备
		PlayerEquipItem(index_inv,index_equip);
		return ;
	}

	const item & itin = _inventory[index_inv];
	if(itin.type == -1 || !(itin.equip_mask & (1 << index_equip)) )
	{
		//物品栏对应位置无内容，则无法装备
		//物品栏对应物品无法装备到指定位置也无法装备
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}

	if(itin.type != iteq.type || iteq.count >= iteq.pile_limit)
	{
		//物品不匹配,或者数量已经满了，无法装备
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}
	
	int delta = _equipment.IncAmount(index_equip,itin.count);
	if(delta < 0)
	{
		ASSERT(false);
		return ;
	}

	IncEquipChangeFlag();
	_inventory.DecAmount(index_inv,delta);
	_runner->move_equipment_item(index_inv,index_equip,delta);
}

void 
gplayer_imp::CalcEquipmentInfo()
{
	_equip_info.data.clear();
	_equipment.GetEquipmentData(_equip_info.mask,_equip_info.data);
	_parent->crc = crc16(_equip_info.data.begin(),_equip_info.data.size()) ^ _equip_info.mask;
}

void 
gplayer_imp::RefreshPropInventory()
{
	//这里不再取消所有装备,只需进行一次扫描即可
	//但注意的是装备可能失效,所有也需要将装备失效或者生效
	size_t count = _equipment.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _equipment[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.CanActivate(_equipment,this))
		{
			it.Activate(i,_equipment,this);
		}
		else
		{
			it.Deactivate(i,this);
		}
	}

	CalcAstrologyConsume();

	count = _fuwen_inventory.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _fuwen_inventory[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.CanActivate(_fuwen_inventory,this))
		{
			it.Activate(i,_fuwen_inventory,this);
		}
		else
		{
			it.Deactivate(i,this);
		}
	}



	//重新计算player 所有数据
	property_policy::UpdatePlayer(GetPlayerClass(),this);

	if(_basic.hp > GetMaxHP()) _basic.hp = GetMaxHP();
	if(_basic.mp > GetMaxMP()) _basic.mp = GetMaxMP();
	if(_basic.rage > GetMaxHP()) _basic.rage = GetMaxHP();
}

void 
gplayer_imp::RefreshEquipment()
{
	//这里不再取消所有装备,只需进行一次扫描即可
	//但注意的是装备可能失效,所有也需要将装备失效或者生效
	size_t count = _equipment.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _equipment[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.CanActivate(_equipment,this))
		{
			it.Activate(i,_equipment,this);
		}
		else
		{
			it.Deactivate(i,this);
		}
	}

	CalcAstrologyConsume();

	//重新计算player 所有数据
	property_policy::UpdatePlayer(GetPlayerClass(),this);

	if(_basic.hp > GetMaxHP()) _basic.hp = GetMaxHP();
	if(_basic.mp > GetMaxMP()) _basic.mp = GetMaxMP();
	if(_basic.rage > GetMaxHP()) _basic.rage = GetMaxHP();
}

bool
gplayer_imp::EquipItem(size_t index_inv,size_t index_equip)
{	
	ASSERT(index_inv < _inventory.Size());
	ASSERT(_inventory[index_inv].type != -1);
	ASSERT(_inventory[index_inv].equip_mask & (1 <<index_equip));

	const item & it = _inventory[index_inv];
	if(it.body)
	{
		if(!it.CanActivate(_equipment,this))
		{
			return false;	
		}
	}
	
	int itemid = it.type;

	item  it1;
	item  it2;
	_inventory.Remove(index_inv, it1);
	_equipment.Remove(index_equip, it2);

	_equipment.Put(index_equip,it1);
	_inventory.Put(index_inv,it2);
	it1.Clear();
	it2.Clear();
	
	RefreshEquipment();

	object_interface oif(this);
	GetAchievementManager().OnEquip(oif, itemid);
	GetAchievementManager().OnEquip(oif, index_equip, _equipment[index_equip].GetSpiritLevel(),  _equipment[index_equip].GetReinforceLevel());
	
	return true;
}

void 
gplayer_imp::RefreshFuwen()
{
	size_t count = _fuwen_inventory.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _fuwen_inventory[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.CanActivate(_fuwen_inventory,this))
		{
			it.Activate(i,_fuwen_inventory,this);
		}
		else
		{
			it.Deactivate(i,this);
		}
	}


	//重新计算player 所有数据
	property_policy::UpdatePlayer(GetPlayerClass(),this);

	if(_basic.hp > GetMaxHP()) _basic.hp = GetMaxHP();
	if(_basic.mp > GetMaxMP()) _basic.mp = GetMaxMP();
	if(_basic.rage > GetMaxHP()) _basic.rage = GetMaxHP();
}

bool
gplayer_imp::ExchangeFashionItem(size_t index_fas,size_t index_equip)
{	
	ASSERT(index_fas < _fashion_inventory.Size());
	ASSERT(_fashion_inventory[index_fas].type != -1);
	ASSERT(_fashion_inventory[index_fas].equip_mask & (1 <<index_equip));

	const item & it = _fashion_inventory[index_fas];
	if(it.body)
	{
		if(!it.CanActivate(_equipment,this))
		{
			return false;	
		}
	}

	int itemid = it.type;

	item  it1;
	item  it2;
	_fashion_inventory.Remove(index_fas, it1);
	_equipment.Remove(index_equip, it2);

	_equipment.Put(index_equip,it1);
	_fashion_inventory.Put(index_fas,it2);
	it1.Clear();
	it2.Clear();
	
	RefreshEquipment();

	object_interface oif(this);
	GetAchievementManager().OnEquip(oif, itemid);

	return true;
}

bool
gplayer_imp::ExchangeEquipMountWingItem(size_t index_mw, size_t index_equip)
{	
	ASSERT(index_mw < _mount_wing_inventory.Size());
	ASSERT(_mount_wing_inventory[index_mw].type != -1);
	ASSERT(_mount_wing_inventory[index_mw].equip_mask & (1 <<index_equip));

	const item & it = _mount_wing_inventory[index_mw];
	if(it.body)
	{
		if(!it.CanActivate(_equipment,this))
		{
			return false;	
		}
	}

	int itemid = it.type;

	item  it1;
	item  it2;
	_mount_wing_inventory.Remove(index_mw, it1);
	_equipment.Remove(index_equip, it2);

	_equipment.Put(index_equip,it1);
	_mount_wing_inventory.Put(index_mw,it2);
	it1.Clear();
	it2.Clear();
	
	RefreshEquipment();

	object_interface oif(this);
	GetAchievementManager().OnEquip(oif, itemid);

	return true;
}

void
gplayer_imp::PlayerExchangePocketItem(size_t idx1, size_t idx2)
{
	if(!_pocket_inventory.ExchangeItem(idx1,idx2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->exchange_pocket_item(idx1,idx2);
}


void 
gplayer_imp::PlayerMovePocketItem(size_t src, size_t dest, size_t count)
{
	if(!_pocket_inventory.MoveItem(src,dest,&count))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->move_pocket_item(src,dest,count);
}


void
gplayer_imp::PlayerExchangeInvPocket(size_t idx_poc, size_t idx_inv)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(idx_poc >= _pocket_inventory.Size() || idx_inv >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if(!_inventory[idx_inv].CanMoveToPocket())
	{
		_runner->error_message(S2C::ERR_CANNOT_MOVE_TO_POCKET);
		return ;
	}
	item it;
	_pocket_inventory.Exchange(idx_poc, it);
	_inventory.Exchange(idx_inv, it);
	_pocket_inventory.Exchange(idx_poc, it);
	_runner->exchange_inventory_pocket_item(idx_poc, idx_inv);
}

void
gplayer_imp::PlayerMovePocketItemToInv(size_t idx_poc, size_t idx_inv, size_t count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	int delta = MoveBetweenItemList(_pocket_inventory,_inventory,idx_poc,idx_inv,count);
	if(delta >= 0)
	{
		_runner->move_pocket_item_to_inventory(idx_poc, idx_inv, delta);
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
}


void
gplayer_imp::PlayerMoveInvItemToPocket(size_t idx_inv, size_t idx_poc, size_t count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	
	if(idx_poc >= _pocket_inventory.Size() || idx_inv >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if(!_inventory[idx_inv].CanMoveToPocket())
	{
		_runner->error_message(S2C::ERR_CANNOT_MOVE_TO_POCKET);
		return ;
	}

	int delta = MoveBetweenItemList(_inventory,_pocket_inventory,idx_inv,idx_poc,count);
	if(delta >= 0)
	{
		_runner->move_inventory_item_to_pocket(idx_inv, idx_poc, delta);
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
}

void
gplayer_imp::PlayerMoveAllPocketItemToInv()
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(MoveAllItemBetweenItemList(_pocket_inventory, _inventory) != 0)
	{
		_runner->error_message(S2C::ERR_CANNOT_MOVE_ALL_TO_INVENTORY);
		return ;
	}
	_runner->move_all_pocket_item_to_inventory();
}

// Youshuang add
void gplayer_imp::PlayerMoveMaxPocketItemToInv()
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	MoveMaxItemBetweenItemList(_pocket_inventory, _inventory);
	_runner->move_max_pocket_item_to_inventory();
}
// end

void
gplayer_imp::PlayerExchangeFashionItem(size_t index1, size_t index2)
{
	if(!_fashion_inventory.ExchangeItem(index1,index2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->exchange_fashion_item(index1,index2);
}

void
gplayer_imp::PlayerExchangeInvFashion(size_t idx_inv, size_t idx_fas)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(idx_inv >= _inventory.Size() || idx_fas >= _fashion_inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if(_inventory[idx_inv].type != -1 && _inventory[idx_inv].GetEquipType() != 3)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	
	
	if(_inventory[idx_inv].type !=-1 && (_inventory[idx_inv].equip_mask & 
	   (item::EQUIP_MASK_FASHION_EYE | item::EQUIP_MASK_FASHION_HEAD | item::EQUIP_MASK_FASHION_BODY | item::EQUIP_MASK_FASHION_FOOT)) == 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	
	item it;
	_fashion_inventory.Exchange(idx_fas, it);
	_inventory.Exchange(idx_inv, it);
	_fashion_inventory.Exchange(idx_fas, it);
	_runner->exchange_inventory_fashion_item(idx_inv, idx_fas);
}


void 
gplayer_imp::PlayerExchangeEquipFashion(size_t index_equ, size_t index_fas)
{
	if(index_fas >= _fashion_inventory.Size() || index_equ >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	bool type1 = (_fashion_inventory[index_fas].type == -1);
	bool type2 = (_equipment[index_equ].type == -1);
	if(type1 && type2)
	{
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}

	if(!type2 && (_equipment[index_equ].equip_mask & 
	    (item::EQUIP_MASK_FASHION_EYE | item::EQUIP_MASK_FASHION_HEAD | item::EQUIP_MASK_FASHION_BODY | item::EQUIP_MASK_FASHION_FOOT))== 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	
	if(!type1)
	{
		if(!(_fashion_inventory[index_fas].equip_mask & (1 << index_equ)))
		{	
			//装备位置不对
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}

		//交换之
		if(!ExchangeFashionItem(index_fas,index_equ))
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		}
		else
		{
			//交换成功，检查一下是否需要进行绑定操作
			item & it = _equipment[index_equ];
			ASSERT(it.type != -1);
			bool notify_equip_item = false;
			if(it.IsBindOnEquip())
			{
				//满足绑定条件，进行绑定
				it.BindOnEquip();
				notify_equip_item = true;
				GLog::log(LOG_INFO,"用户%d装备绑定物品%d,GUID(%d,%d)",_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
				//检查是否需要记录消费值
				CheckEquipConsumption(it.type);
			}
		
			int count1 = _fashion_inventory[index_fas].count;
			int count2 = _equipment[index_equ].count;
			ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
			_runner->exchange_equipment_fashion_item(index_equ, index_fas);
			int64_t id1 = _equipment[index_equ].type | _equipment[index_equ].GetIdModify();
			CalcEquipmentInfo();
			_runner->equipment_info_changed(1<<index_equ , 0, &id1,sizeof(id1));//此函数使用了CalcEquipmentInfo的结果
			if(notify_equip_item)
			{
				PlayerGetItemInfo(IL_EQUIPMENT,index_equ);
			}

		}
		IncEquipChangeFlag();
		return ;
	}
	//拿下原来的，并刷新装备
	item  it1;
	_equipment.Remove(index_equ,it1);
	bool bRst = _fashion_inventory.Put(index_fas,it1);
	ASSERT(bRst);
	if(bRst)
	{
		it1.Clear();
	}
	else
	{
		//记录错误日志
		GLog::log(GLOG_ERR,"装备物品时发生致命错误");
		it1.Release();
	}
	RefreshEquipment();
	int count1 = _fashion_inventory[index_fas].count;
	ASSERT(count1 > 0);
	_runner->exchange_equipment_fashion_item(index_equ, index_fas);
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1<<index_equ , 0, 0); //此函数使用了CalcEquipmentInfo的结果
	IncEquipChangeFlag();
}

	
void 
gplayer_imp::PlayerGetInventory(int where)
{
	abase::octets buf;
	unsigned char size;
	switch(where)
	{
		case IL_INVENTORY:
			_inventory.SimpleSave(buf);
			size = _inventory.Size();
			break;
		case IL_EQUIPMENT:
			_equipment.SimpleSave(buf);
			size = _equipment.Size();
			break;
		case IL_TASK_INVENTORY:
			_task_inventory.SimpleSave(buf);
			size = _task_inventory.Size();
			break;
		case IL_PET_BEDGE:
			_pet_bedge_inventory.SimpleSave(buf);
			size = _pet_bedge_inventory.Size();
			break;
		case IL_PET_EQUIP:
			_pet_equip_inventory.SimpleSave(buf);
			size = _pet_equip_inventory.Size();
			break;
		case IL_POCKET:
			_pocket_inventory.SimpleSave(buf);
			size = _pocket_inventory.Size();
			break;	
		case IL_FASHION:
			_fashion_inventory.SimpleSave(buf);
			size = _fashion_inventory.Size();
			break;
		case IL_MOUNT_WING:
			_mount_wing_inventory.SimpleSave(buf);
			size = _mount_wing_inventory.Size();
			break;
		case IL_GIFT:
			_gift_inventory.SimpleSave(buf);
			size = _gift_inventory.Size();
			break;
		case IL_FUWEN:
			_fuwen_inventory.SimpleSave(buf);
			size = _fuwen_inventory.Size();
			break;
		default:
		ASSERT(false);
		return;
	}
	_runner->self_inventory_data(where, size, buf.begin(), buf.size());
}

void 
gplayer_imp::PlayerDropMoney(size_t amount,bool isProtected)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(GetWorldManager()->GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}
	ThrowMoney(amount,isProtected);
}

void 
gplayer_imp::ThrowMoney(size_t amount, bool isProtected)
{	
	if(amount >= _player_money) amount = _player_money;
	if(amount == 0) return ;
	GLog::log(LOG_INFO,"用户%d丢弃金钱%d",_parent->ID.id,amount);
	if (amount) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=6:hint=0",_parent->ID.id,_db_magic_number,amount);
	SpendMoney(amount,false);

	//设置随机的位置
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();

	//如果超出了边界，那么就按照自己的位置来算
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	if(isProtected)
		DropMoneyItem(_plane,pos,amount,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id);
	else
		DropMoneyItem(_plane,pos,amount,XID(0,0),0,0,_parent->ID.id);

	_runner->spend_money(amount);
}

void 
gplayer_imp::PlayerGetProperty()
{
	_runner->self_get_property(_basic.status_point,_cur_prop);
}

void
gplayer_imp::PlayerLearnSkill(int skill_id)
{
	//如果没有任何可用技能点， 不进行学习操作
	int level = _skill.Learn(skill_id, object_interface(this));
	if(level <= 0)
	{
		_runner->error_message(S2C::ERR_CANNOT_LEARN_SKILL);
	}
}

bool
gplayer_imp::LongJump(const A3DVECTOR &pos,int target_tag)
{
	if(!_pstate.CanLongJump())
	{
		return false;
	}

	A3DVECTOR new_pos = pos;
	if(target_tag == 1)
	{
		target_tag = 401;
		new_pos = A3DVECTOR(368, 481, 348);
	}

	//麒麟的member不可以jump
	gplayer *pParent = GetParent();
	if(_pstate.IsBindState() && (pParent->bind_type == 8 || pParent->bind_type == 7))
	{
		_link_qilin_player.PlayerLinkCancel(this);
	}

	world_manager * target_wm = gmatrix::FindWorld(target_tag);
	if(!target_wm || target_wm->IsIdle()) return false;

	//一些地图限制通过LongJump跳进地图，只允许本地图内LongJump。以后加了地图进入限制条件后，应该把这个限制删掉，走模板更合理
	if(GetWorldTag() != target_tag && target_wm->GetWorldLimit().no_longjump_in)
	{
		return false;
	}

	//目标地图需要封神
	if(GetWorldTag() != target_tag && target_wm->GetWorldLimit().need_deity && !IsDeity())
	{
		return false;
	}

	//增加副本世界内跳转的检查，需要tag一直，raid_id一致 
	if(GetWorldManager()->IsRaidWorld() || target_wm->IsRaidWorld())
	{
		if(target_wm->GetRaidID() == GetWorldManager()->GetRaidID() && target_tag == _plane->GetTag())
		{
			LongJump(new_pos);
			return true;
		}
		else
		{
			return false;
		}
	}//liuyue-facbase
	else if (GetWorldManager()->IsFacBase() || target_wm->IsFacBase())
	{
		if (target_wm->GetClientTag() == GetClientTag() && target_tag == _plane->GetTag())
		{
			LongJump(new_pos);
			return true;
		}
		else
			return false;
	}
	else if(target_tag == _plane->GetTag()) 
	{
		LongJump(new_pos);
		return true;
	}

	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	//让Player进行副本传送 
	{
		if(GetWorldManager()->PlaneSwitch(this ,new_pos,target_tag,key,0) < 0)
		{
			return false;
		}
		_filters.RemoveFilter(FILTER_SPIRITDRAG);
		_filters.RemoveFilter(FILTER_PULL);
	}	
	return true;
}


void 
gplayer_imp::LongJump(const A3DVECTOR &pos)
{
	if(_plane->PosInWorld(pos))
	{
		_filters.RemoveFilter(FILTER_SPIRITDRAG);
		_filters.RemoveFilter(FILTER_PULL);
		//目标在世界中，直接进行跳转操作
		__PRINTF("player goto %f %f\n",pos.x,pos.z);

		A3DVECTOR newpos = pos;
		float height = GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
		if(newpos.y < height) newpos.y = height;
		/*liuyue-facbase
		if(GetWorldManager()->IsRaidWorld())
		{
			_runner->notify_raid_pos(newpos);
		}
		else
		{
			_runner->notify_pos(newpos);
		}
		*/
		_runner->notify_pos(newpos);

		newpos -= _parent->pos;
		bool bRst = StepMove(newpos);
		_runner->stop_move(_parent->pos,0x1000,_parent->dir,0x01);
		ASSERT(bRst);
		//测试是否在安全区
		TestSanctuary();

		_ph_control.Initialize(this);
	}
}

void
gplayer_imp::DebugJump()
{
	int target_tag = 1;
	A3DVECTOR pos(165.2, 163.5, 140.3);

	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	//让Player进行副本传送 
	{
		if(GetWorldManager()->PlaneSwitch(this,pos,target_tag,key,0) < 0)
		{
			return ;
		}
		_filters.RemoveFilter(FILTER_SPIRITDRAG);
		_filters.RemoveFilter(FILTER_PULL);
	}	
	return;

}

void 
gplayer_imp::PlayerGoto(const A3DVECTOR & pos)
{
	LongJump(pos);
}

bool 
gplayer_imp::Resurrect(float exp_reduce)
{
	if(!_parent->IsZombie()) return false;
	if(!_pstate.CanResurrect())  return false;
	session_resurrect * pSession= new session_resurrect(this);
	pSession->SetExpReduce(exp_reduce);
	AddStartSession(pSession);
	return true;
}

bool 
gplayer_imp::CanResurrect()
{
	//判定是否能够进行复活操作
	//不是所有状态都能够复活的
	return _pstate.CanResurrect();
}

int 
gplayer_imp::Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int world_tag, float hp_recover)
{
	if(_kill_by_player) exp_reduce = 0.f;
	//复活流程 首先复活自己
	_parent->b_zombie = false;
	
	//将自己的hp和mp成为原始的10%
	_basic.hp = (int)(GetMaxHP()* hp_recover + 0.5f);
	if(!IsRenMa()) _basic.mp = (int)(GetMaxMP()* hp_recover + 0.5f);
	_basic.dp = (int)(GetMaxDP()* hp_recover + 0.5f);

	SetRefreshState();
	_enemy_list.clear();

	((gplayer_controller*)_commander)->OnResurrect();

	//清除后继所有的session
	ClearNextSession();

	
	if(!nomove)
	{
		_runner->resurrect(0);
		LongJump(pos,world_tag);
	}
	else
	{
		//现在暂时原地复活

		//暂时处于无敌和不可操作状态(加入新的session) 
		AddStartSession(new session_resurrect_protect(this));


	}
	int64_t exp = player_template::GetLvlupExp(GetRebornCount(), _basic.level);
	int64_t rexp = (int64_t)(exp * exp_reduce + 0.5f);
	if(rexp > 0)
	{
		if(rexp > MAX_DEATH_EXP_PUNISH) rexp = MAX_DEATH_EXP_PUNISH;
		int64_t new_exp = (_basic.exp - rexp);
	//	if(new_exp < -exp) new_exp = -exp;
		if(new_exp < 0) new_exp = 0;	//经验值不会变成负数...
		if(_basic.level >= player_template::GetMaxLevel(GetRebornCount())) new_exp = 0;
//		_runner->receive_exp(new_exp - _basic.exp,0);
		rexp = new_exp - _basic.exp;
		_basic.exp = new_exp;
	}

	OnResurrect();

	//记录复活日志
	GLog::log(GLOG_INFO,"用户%d复活类型(%d) 损失经验%lld(%2.2f)", _parent->ID.id, nomove?1:0,rexp,exp_reduce);
	return 0;
}

#include "cmdprepare.h"
static player_cmd_dispatcher ccd;
int 
gplayer_imp::DispatchCommand(int cmd_type, const void * buf,size_t size)
{
	return ccd.DispatchCommand(this, ((gplayer_controller*)_commander),cmd_type, buf, size);
}

void 
gplayer_imp::OnPickupMoney(size_t money,int drop_id)
{	
	if(drop_id)
	{
		GLog::log(GLOG_INFO,"用户%d拣起金钱%d[用户%d丢弃]",_parent->ID.id,money,drop_id);
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d拣起金钱%d",_parent->ID.id,money);
	}
	if (money) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=1:reason=4:hint=0",_parent->ID.id,_db_magic_number,money);
	size_t nmoney = _player_money;
	GainMoneyWithDrop(money,false);	//里面不再记录日志
	if(_player_money > nmoney)
	{
		_runner->pickup_money(_player_money - nmoney);
	}
	if((int)_player_money < 0) _player_money = 0x7FFFFFFF;
	__PRINTF("现在有钱%u\n",_player_money);
}

void
gplayer_imp::PickupMoneyInTrade(size_t money)
{
	((gplayer_dispatcher*)_runner)->pickup_money_in_trade(money);
	_trade_obj->PickupMoney(money);
}

void 
gplayer_imp::PickupItemInTrade(const A3DVECTOR & pos,const void * data, size_t size,bool isTeam)
{
	if(size < sizeof(item_data)) return ;
	item_data *pData = (item_data*)data;
	if(size != sizeof(item_data) + pData->content_length)
	{
		ASSERT(false && " invalid item data size");
		return;
	}
	pData->item_content = (char*)data + sizeof(item_data);
	int ocount = pData->count;

	if(isTeam && _team.IsInTeam() && _team.IsRandomPickup())
	{
		//是随机捡取通知队友 检到了物品
		_team.NotifyTeamPickup(pos,pData->type,pData->count);
	}

	int rst = _trade_obj->PickupItem(pData);
	if(rst >=0)
	{
		//发送捡起物品的消息
		//要注意可能还剩下物品
		__PRINTF("现在有物品%d to %d\n",pData->type,rst);
		((gplayer_dispatcher*)_runner)->pickup_item_in_trade(pData->type,ocount - pData->count);
	}

	if(pData->count)
	{
		//没有完全捡起来,应该将物品重新抛出
		//未必能够真正抛出(由于分配物品的原因)
		DropItemFromData(_plane,_parent->pos,*pData,_parent->ID,0,0);
		__PRINTF("捡起失败，未完全捡起%d %d/%d\n",pData->type,pData->count,ocount);
	}
}

void 
gplayer_imp::PurchaseItem(abase::pair<const item_data *, int>* pItem,size_t size,size_t money)
{
	ASSERT(_player_money >= money && size && "调用之前已经经过检查"  );
	int rst = 0;
	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,-1, money,size);

	if(size > 36) return;
	char logtxt[24*14+512];	//假设最多购买12件物品
	int index = 0;

	sprintf(logtxt, "NPCtrade, huid=%d:hrid=%d:type=1:moneytype=1:price=%d:lv=%d", _db_magic_number, _parent->ID.id,money, GetObjectLevel());
	std::string actlog= logtxt;

	for(size_t i = 0; i < size; i ++,pItem++)
	{
		//rst =_inventory.PushInEmpty(rst,*pItem->first,pItem->second);
		int count = pItem->second;
		rst =_inventory.Push(*pItem->first,count,0);
		ASSERT(rst >= 0 && count == 0);
		_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_NPC);

		int state = item::Proctype2State(pItem->first->proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,pItem->first, pItem->second,rst,state);
		index += snprintf(logtxt + index,sizeof(logtxt) - index,"%d个%d,",pItem->second,pItem->first->type);

		char tmpbuf[128];
		actlog +=":nitemid=";
		sprintf(tmpbuf, "%d", pItem->first->type);
		actlog += tmpbuf ;
		actlog +=",";
		sprintf(tmpbuf, "%d", pItem->second);
		actlog += tmpbuf ;
	}
	if (money) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=1:hint=0,0",_parent->ID.id,_db_magic_number,money);
	GLog::action("%s",actlog.c_str());
	ASSERT(index);
	index --;
	logtxt[index] = 0;
	GLog::log(GLOG_INFO,"用户%d从NPC购买了%s",_parent->ID.id, logtxt);
	SpendMoney(money);
	send_ls_msg(GetParent(), h1);
}

void 
gplayer_imp::OnPickupItem(const A3DVECTOR &pos,const void * data, size_t size,bool isTeam,int drop_id)
{
	if(size < sizeof(item_data)) return ;
	item_data *pData = (item_data*)data;
	if(size != sizeof(item_data) + pData->content_length)
	{
		ASSERT(false && " invalid item data size");
		return;
	}
	pData->item_content = (char*)data + sizeof(item_data);
	int ocount = pData->count;

	//可回收物品不能带出战场、副本，进包裹前再次检查是防止时序问题。
	if( CanRecycle(pData) )
	{
		if( !(_plane->GetWorldManager()->IsBattleWorld() || _plane->GetWorldManager()->IsRaidWorld() || _plane->GetWorldManager()->IsFacBase()) )  
		{
			return;
		}
	}

	if(drop_id)
	{
		GLog::log(GLOG_INFO,"用户%d拣起%d个%d[用户%d丢弃]%s",_parent->ID.id,ocount, pData->type,drop_id,_cheat_punish?"(作弊)":"");
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d拣起%d个%d%s",_parent->ID.id,ocount, pData->type,_cheat_punish?"(作弊)":"");
	}

	if(isTeam && _team.IsInTeam())// && _team.IsRandomPickup())
	{
		//是随机捡取通知队友 检到了物品
		_team.NotifyTeamPickup(pos,pData->type,pData->count);
	}

	if(_cheat_punish)
	{
		//作弊惩罚， 不能得到任何物品
		return ;
	}

	int rst = _inventory.Push(*pData);
	if(rst >=0)
	{
		//发送捡起物品的消息
		//要注意可能还剩下物品
		__PRINTF("现在有物品%d to %d\n",pData->type,rst);
		int state = item::Proctype2State(pData->proc_type);
		_runner->pickup_item(pData->type,pData->expire_date, ocount - pData->count,_inventory[rst].count, 0,rst,state);

		if( pData->count == 0 )
		{
			DeliverTopicSite( new TOPIC_SITE::got_item( pData->type, ocount ) );  // Youshuang add
		}
	}

	if(pData->count)
	{
		//没有完全捡起来,应该将物品重新抛出
		if(isTeam)
			DropItemFromData(_plane,_parent->pos,*pData,_parent->ID,0,0);
		else
			DropItemFromData(_plane,pos,*pData,_parent->ID,0,0);
		__PRINTF("捡起失败，未完全捡起%d %d/%d\n",pData->type,pData->count,ocount);
	}
}

bool
gplayer_imp::ObtainItem(int where, item_data * pData, bool isTask, int deliver_type)
{
	ASSERT(where != IL_EQUIPMENT);
	int ocount = pData->count;
	int pile_limit = pData->pile_limit;
	item_list & inv = GetInventory(where);
	int rst = inv.Push(*pData);
	if(rst >=0)
	{
		DeliverTopicSite( new TOPIC_SITE::got_item( pData->type, ocount - pData->count ) );  // Youshuang add
		
		int state = item::Proctype2State(pData->proc_type);
		if(isTask)
		{
			_runner->task_deliver_item(pData->type,pData->expire_date, state, ocount - pData->count,inv[rst].count, where,rst);
			if(pile_limit == 1)
			{
				//以任务方式发送，并且堆叠上限为1，则调用InitFromShop
				//这主要是为了法宝初始化
				//注意彩票也会调用DeliverItem并以任务方式调用ObtainItem
				inv[rst].InitFromShop(this,deliver_type);
			}
		}
		else
			_runner->obtain_item(pData->type,pData->expire_date, ocount - pData->count,inv[rst].count, where,rst,state);
	}

	if(pData->count && !isTask)
	{
		//应该不会剩下物品才对
		//不过为了保险，还是判断一下
		DropItemData(_plane,_parent->pos,pData,_parent->ID,0,0);
		return false;
	}
	return true;
}

bool gplayer_imp::ObtainItem(int where, item_data * pData, int& inv_idx, bool isTask,int deliver_type)
{
	ASSERT(where != IL_EQUIPMENT);
	inv_idx = -1;
	int ocount = pData->count;
	int pile_limit = pData->pile_limit;
	item_list & inv = GetInventory(where);
	int rst = inv.Push(*pData);
	if(rst >=0)
	{
		DeliverTopicSite( new TOPIC_SITE::got_item( pData->type, ocount - pData->count ) );  // Youshuang add
		
		inv_idx = rst;
		int state = item::Proctype2State(pData->proc_type);
		if(isTask)
		{
			_runner->task_deliver_item(pData->type,pData->expire_date, state, ocount - pData->count,inv[rst].count, where,rst);
			if(pile_limit == 1)
			{
				//以任务方式发送，并且堆叠上限为1，则调用InitFromShop
				//这主要是为了法宝初始化
				//注意彩票也会调用DeliverItem并以任务方式调用ObtainItem
				inv[rst].InitFromShop(this,deliver_type);
			}
		}
		else
			_runner->obtain_item(pData->type,pData->expire_date, ocount - pData->count,inv[rst].count, where,rst,state);
	}

	if(pData->count && !isTask)
	{
		//应该不会剩下物品才对
		//不过为了保险，还是判断一下
		DropItemData(_plane,_parent->pos,pData,_parent->ID,0,0);
		return false;
	}
	return true;
}

bool
gplayer_imp::StepMove(const A3DVECTOR &offset)
{
	_direction = offset;
	_is_moved = true;
	bool bRst = false;
	if((bRst = gobject_imp::StepMove(offset)))
	{
		if(_pstate.IsBindState())
		{
			gplayer *pParent = GetParent();
			//相依相偎的男方
			if(pParent->bind_type == 1 && _bind_player.IsPlayerLinked()){
				SendTo<0>(GM_MSG_PLAYER_BIND_FOLLOW,_bind_player.GetLinkedPlayer(),0);
			}
			//多人骑乘的leader
			else if(pParent->bind_type == 3 && _link_ride_player.IsPlayerLinked()){
				_link_ride_player.SendGroupMessage(this, GM_MSG_PLAYER_LINK_FOLLOW);
			}
			//多人骑乘的leader
			else if(pParent->bind_type == 7 && _link_qilin_player.IsPlayerLinked()){
				_link_qilin_player.SendGroupMessage(this, GM_MSG_PLAYER_LINK_FOLLOW);
			}
		}
		if (_mobactiveman.IsActive())
		{
			_mobactiveman.PlayerBeMoved(this);
			_mobactiveman.MobActiveSyncPos(this);
		}

		_filters.EF_OnMoved();
	}
	return bRst;
}

class PlayerForceOfflineTask :public ONET::Thread::Runnable
{
	gplayer * _player;
	int _uid;
	int _cs_sid;
	int _cs_index;
public:
	PlayerForceOfflineTask(gplayer *pPlayer):_player(pPlayer)
						 ,_uid(pPlayer->ID.id),_cs_sid(pPlayer->cs_sid)
						 ,_cs_index(pPlayer->cs_index)
	{
	}
	
	virtual void Run()
	{
		spin_autolock keeper(_player->spinlock);
		if(!_player->IsActived() || _player->ID.id != _uid  ||
				_player->cs_index != _cs_index || _player->cs_sid != _cs_sid || !_player->imp)
		{
			//这个用户可能正好已经消失了，所以直接返回，不进行处理
			GLog::log(GLOG_WARNING,"用户%d异常下线时此用户已经消失",_uid);
			return;
		}
		GLog::log(GLOG_WARNING,"用户%d因为数据错误进行异常下线操作",_uid);
		_player->imp->PlayerForceOffline();
		delete this;
	}
};

void 
gplayer_imp::OnHeartbeat(size_t tick)
{
	_filters.Heartbeat(tick);
	if(_cur_session)
	{
		//这里是为了解决一个timer的错误而设置的
		//在某种情况下，尤其是p_move系列session时，有时session无法正确执行timer
		//这种情况的概率不高，但尚未到可以控制重现的地步
		//为此加入了如下判断代码以供较快的将session清除
		//由于只观察到错误仅发生在session_p_stop_move处
		//目前 GetLimitTime函数仅在session_p_stop_move处实现
		int rst = _cur_session->GetLimitTime();
		if(rst > 0)
		{
			//进行进行超时时间的判定
			unsigned int t =  _cur_session->_start_tick;
			if((unsigned int)(g_timer.get_tick() - t) > (unsigned int)(MILLISEC_TO_TICK(t) + TICK_PER_SECOND * 2))
			{
				//超出时间两秒，清除当前session
				GLog::log(GLOG_ERR, "%d cur_session 等待超时了%d",_parent->ID.id, _cur_session->GetGUID());
				ClearSession();
			}
		}
	}

//	__PRINTF("player %d heart beat\n", _parent->ID.id);
	if(_combat_timer)
	{
		_combat_timer --;
		if(_combat_timer <=0)
		{
			ActiveCombatState(false);
			_combat_timer = 0;
		}
	}

	_plane->InspireNPC<0>(_parent->pPiece,0);

	//auto gen hp/mp
	if(!_parent->IsZombie())
	{
		bool sitdown = IsSitDownMode();
		if(IsCombatState())
		{
			int hpgen = netgame::GetCombatHPGen(_cur_prop, sitdown);
			int mpgen = netgame::GetCombatMPGen(_cur_prop, sitdown);
			int dpgen = netgame::GetCombatDPGen(_cur_prop, sitdown);
			GenHPandMP(hpgen,mpgen);
			GenDP(dpgen);
		}
		else
		{
			int hpgen = netgame::GetHPGen(_cur_prop,sitdown);
			int mpgen = netgame::GetMPGen(_cur_prop,sitdown);
			int dpgen = netgame::GetDPGen(_cur_prop,sitdown);
			GenHPandMP(hpgen,mpgen);
			GenDP(dpgen);
		}

		if(IsRenMa())
		{
			_basic.mp = GetMaxMP();
		}
	}

	
	int cur_time = g_timer.get_systime();
	int rand_id = cur_time + (_parent->ID.id >> 5);
	
	if(!_parent->IsZombie() && (rand_id & 0x03) == 0)
	{
		//每4秒钟清除一次仇恨
		ENEMY_LIST::iterator it = _enemy_list.end(); 
		for(;it > _enemy_list.begin(); )
		{
			--it;
			if(it->second <=0) 
			{
				__PRINTF("删除了超时的仇恨记录 来自%x\n",it->first);
				it = _enemy_list.erase(it);
			}
			else
			{
				__PRINTF("来自%x的仇恨记录 有%d个计数\n",it->first,it->second);
				it->second = 0;
			}
		}
	}

	//在线倒计时奖励
	if(_online_award.IsTimeToCheck())
	{
		CheckPlayerOnlineAward(cur_time);
	}
	//检查活跃度是否到零点
	OnHeartbeatLiveness(cur_time);

	//每8秒一次检查是否需要自动进入安全区
	//每8秒检查是否处在合法的区域
	if(!_sanctuary_mode && (rand_id & 0x07) == 0 )
	{
		TestSanctuaryNormal();
		//TestRegion();
	}
	if((rand_id&0x07) == 0)
	{
		TestRegion();
	}


	//放在这里的原因是在回血和减血之后，清空数据更新之前，这样有助于正确的发送数据
	_team.OnHeartbeat();

	bool bSaveMode = _pstate.CanSave();
	if(_expire_item_date  && bSaveMode)
	{
		if(cur_time >= _expire_item_date)
		{
			//应该进行删除操作了
			RemoveAllExpireItems();
		}
	}

	//判断是否存盘 只有普通模式进行这个判断
	if(--_write_timer <= 0)
	{
		if(bSaveMode)
		{
			AutoSaveData();
		}
		_write_timer = abase::Rand(500, 513);
	}


	//判断是否发送玩家在线信息
	if(--_link_notify_timer <= 0)
	{
		//每隔33秒调用一次任务的函数
		PlayerTaskInterface tf(this);
		OnTaskCheckState(&tf);

		//重置计数器
		_link_notify_timer = LINK_NOTIFY_TIMER;

		//这里要判断自己是否能够查询到自己（一个办法是在存盘的时候检查）
		//这里如果无法查询到自己的话，会有少许问题，即不能在这里删除自身
		//因为Heartbeat的时候需要做出一些事情
		gplayer *pPlayer = GetParent();
		int index1;
		gplayer * pPlayer2 = gmatrix::FindPlayer(pPlayer->ID.id,index1);
		if(pPlayer2 != pPlayer || pPlayer2 == NULL)
		{
			//这样必须将自己释放且不存盘
			//加入一个可能的任务
			GLog::log(GLOG_WARNING,"%d用户%d(%d,%d)数据发生错误，开始异常下线(%d)",_plane->GetTag(),pPlayer->ID.id,pPlayer->cs_index, pPlayer->cs_sid,pPlayer2?1:0);
			ONET::Thread::Pool::AddTask(new PlayerForceOfflineTask(pPlayer));
		}
		else if(_pstate.IsOnline())
		{
			GMSV::SendPlayerHeartbeat(pPlayer->cs_index,pPlayer->ID.id, pPlayer->cs_sid);
			__PRINTF("发送心跳到link服务器 %d\n",pPlayer->ID.id);
			//这里记录日志似乎会过多 ，考虑一下记录日志的办法

			//用随机数来减少日志数量
			if(abase::Rand(0,5) == 0)
			{
				GLog::log(GLOG_INFO,"%d发送用户%d(%d,%d)心跳消息,gs_id=%d",_plane->GetTag(),pPlayer->ID.id,pPlayer->cs_index,pPlayer->cs_sid, gmatrix::GetServerIndex());
			}

			bool pos_log = false;
			if(_pstate.IsNormalState())
			{
				//正常状态记录日志更加频繁一些
				if(abase::Rand(0,4) == 0)
				{
					pos_log = true;
				}
			}
			else
			{
				if(abase::Rand(0,8) == 0)
				{
					pos_log = true;
				}
			}
			if(pos_log)
			{
				GLog::log(GLOG_INFO,"用户%d处于位置(%f,%f,%f)[%d],状态%d",_parent->ID.id,_parent->pos.x,_parent->pos.y,_parent->pos.z,_plane->GetTag(), _layer_ctrl.layer);
			}
		}


	}

	//判断是否双倍时间
	if(_item_multi_exp.cur_multi_exp > 0)
	{
		if(cur_time >= _item_multi_exp.cur_multi_exp_timestamp)
		{
			UpdateMultiExp(true);
		}
	}

	PVPCombatHeartbeat();
	_duel.Heartbeat(this);
	_bind_player.Heartbeat(this);
	_link_ride_player.Heartbeat(this);
	_link_bath_player.Heartbeat(this);
	_link_qilin_player.Heartbeat(this);
	_petman.Heartbeat(this);
	_summonman.Heartbeat(this);
	_mobactiveman.Heartbeat(this);
	_active_emote_player.Heartbeat(this);

	int mafia_id = ((gplayer*)_parent)->id_mafia;
	if(mafia_id > 0)
	{
		_mduel.Heartbeat(this, mafia_id);
		_fh_duel.Heartbeat( this, mafia_id );
	}

	if(_talisman_info.talisman_state)
	{
		if(_talisman_info.bot_state)
		{
			//飞升前90级以下未飞行时不扣除法宝精力
			//VIP账号也可以享受
			if(IsFreeBot() && !_fly_info.active)
			{
			}
			else
			{
				float rst = _talisman_info.stamina - _talisman_info.energy_drop_speed;
				if(rst < 0) 
				{
					rst = 0;
					StopTalismanBot();
				}
				if(rst != _talisman_info.stamina)
				{
					_talisman_info.stamina = rst;
					//按照数值下降
					SetRefreshState();
				}
			}

		}
		else
		{
			if(!_fly_info.state || !_fly_info.active)		//只有不飞行的时候才涨精力
			{
				float offset = _talisman_info.energy_recover_speed;
				if(!IsCombatState() || _duel.IsDuelMode())
				{
					offset *= _talisman_info.energy_recover_factor;
				}
				offset *= _talisman_info.max_stamina;		//按照百分比增长
				float rst = _talisman_info.stamina + offset;
				if(rst > _talisman_info.max_stamina) rst = _talisman_info.max_stamina;
				if(rst != _talisman_info.stamina)
				{
					_talisman_info.stamina = rst;
					SetRefreshState();
				}
			}
		}
	}

/*
	//处理沉迷系统
	if(gmatrix::AntiWallow())
	{
		int new_wallow = _wallow_obj.Tick(g_timer.get_systime());
		if(new_wallow != _wallow_level)
		{
			//沉迷状态发生变化
			_wallow_level = new_wallow;

			//通知客户端
			time_t l_time;
			time_t h_time;
			int ptime;
			_wallow_obj.GetTimeLeft(&l_time, &h_time, &ptime);
			_runner->player_wallow_info(_wallow_level, ptime, l_time, h_time);
		}
	}
*/
	if(_cheat_mode > 0)
	{
		_cheat_mode --; 
		if(_cheat_mode <= 0)
		{
			_cheat_mode = -1;
			PunishCheater();
		}
	}       
	
	if (--_achieve_timer <=0)
	{
		_achieve_timer = 10;
		object_interface oif(this);
		if (GetMoney() > _achieve_cache.money)
		{
			_achieve_cache.money = GetMoney();
			GetAchievementManager().OnMoneyInc(oif, _achieve_cache.money);
		}
		if (GetPKValue() > _achieve_cache.pk_value)
		{
			_achieve_cache.pk_value = GetPKValue();
			GetAchievementManager().OnPKValueChange(oif, _achieve_cache.pk_value);
		}
	}

	if(--_astrology_info.counter <= 0)
	{
		UpdateAstrologyInfo();
		_astrology_info.counter = Rand(30, 90);
	}

	UpdateTreasureInfo(cur_time);
	if(!GetWorldManager()->NoBuffArea() && --_check_buffarea_time <= 0)
	{
		_check_buffarea_time = 2;
		CheckBuffArea();
	}

	//过了凌晨12点, 重置一些东西
	if(_another_day_timestamp > 0 && cur_time >= _another_day_timestamp) 
	{
		_another_day_timestamp = cur_time + 86400; 
		//重置符文碎片合成信息
		ResetFuwenComposeInfo();
		//reset cs exchange award info
		ResetExchange6V6AwardInfo();
	}
}



void gplayer_imp::TryClearTBChangeCounter()
{
	if(_trashbox.IsOpened())
	{
		_trashbox.IncChangeCounter();
	}
	else
	{
		_trashbox.ClrChangeCounter();
	}
}

void 
gplayer_imp::AutoSaveData()
{
	//write
	class AutoWrite: public GDB::Result, public abase::ASmallObject
	{	
		world * _plane;
		int _userid;
		unsigned int _counter;
		unsigned int _counter2; 
		int _cur_order_id;
	public:
		AutoWrite(gplayer_imp * imp)
		{
			_plane = imp->_plane;
			_userid = imp->_parent->ID.id;
			_counter = imp->GetTrashBoxCounter();
			_counter2 = imp->_eq_change_counter;
			_cur_order_id = imp->_mallinfo.GetOrderID();
		}

		//平时存盘不做处理
		virtual void OnTimeOut() { OnFailed();}
		//不会受到这个命令 
		virtual void OnFailed() {
			GLog::log(GLOG_WARNING,"在%d自动存盘保存用户%d数据失败",_plane->GetTag(),_userid);
			MSG msg;
			BuildMessage(msg,GM_MSG_DBSAVE_ERROR,XID(GM_TYPE_PLAYER,_userid),XID(GM_TYPE_PLAYER,_userid),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg);
			delete this;
		}

		virtual void OnPutRole(int retcode){
			//试图寻找一下玩家，将物品箱的写入标志置为false
			ASSERT(retcode == 0);

			int index = gmatrix::FindPlayer(_userid);
			if(index >= 0)
			{
				gplayer * pPlayer = gmatrix::GetPlayerByIndex(index);
				spin_autolock keeper(pPlayer->spinlock);
				if(pPlayer->IsActived() && pPlayer->imp && pPlayer->ID.id == _userid
						&& pPlayer->login_state >= gplayer::LOGIN_OK)
				{
					gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
					//只有没有再次打开过而没有正在打开箱子，才会清除箱子的更改计数 
					if(_counter && pImp->GetTrashBoxCounter() == _counter)
					{
						pImp->TryClearTBChangeCounter();
					}

					if(_counter2 == pImp->_eq_change_counter )
					{
						pImp->_eq_change_counter = 0;
					}
					pImp->_mallinfo.SaveDone(_cur_order_id);

					//将存盘错误清0
					pImp->_db_save_error = 0;
				}
				
			}
			GLog::log(GLOG_INFO,"用户%d数据自动存盘完成",_userid);
			delete this;
		}
	};
	_write_counter ++;
	_write_timer = abase::Rand(100, 120);		//这里也设一下写盘等待时间，这样别的地方也可以调用写盘
	user_save_data((gplayer*)_parent,new AutoWrite(this),1);
}

void
gplayer_imp::OnAttacked(const MSG & msg,attack_msg *attack,damage_entry &dmg,bool is_hit)
{
	//这里应该做很多判断，暂时先考虑战斗状态
	ActiveCombatState(true);
	if(_combat_timer < NORMAL_COMBAT_TIME)
	{
		_combat_timer = NORMAL_COMBAT_TIME;
	}
	_petman.PlayerBeAttacked(this, msg.source);
	_summonman.PlayerBeAttacked(this, msg.source);
	_mobactiveman.PlayerBeAttacked(this);

	if(IsXuanYuan())
	{
		CastDarkLightSpiritOnAttackSkill(attack->ainfo.attacker);
	}
}

void gplayer_imp::OnEnchant(const MSG & msg, enchant_msg * enchant)
{
	if(!enchant->helpful)
	{
		if(IsXuanYuan())
		{
			CastDarkLightSpiritOnAttackSkill(enchant->ainfo.attacker);
		}
	}
}

void 
gplayer_imp::OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader)
{
	//设置战斗状态的消息
	ActiveCombatState(true);
	if(_combat_timer < NORMAL_COMBAT_TIME)
	{
		_combat_timer = NORMAL_COMBAT_TIME;
	}

	if(info.attacker.IsPlayerClass())
	{
		AddHurtEntry(info.attacker,damage,info.team_id,info.team_seq);
	}

	_runner->be_hurt(attacker, info, damage,invader);
}

void 
gplayer_imp::OnDamage(const XID & attacker,int skill_id, const attacker_info_t&info,int damage, int dt_damage, char at_state,char stamp,bool orange)
{
	//设置战斗状态的消息
	ActiveCombatState(true);
	if(_combat_timer < NORMAL_COMBAT_TIME)
	{
		_combat_timer = NORMAL_COMBAT_TIME;
	}
	if(info.attacker.IsPlayerClass())
	{
		AddHurtEntry(info.attacker,damage,info.team_id,info.team_seq);
	}

	_runner->be_damaged(attacker,skill_id, info, damage, dt_damage, -1,at_state,stamp,orange);
}

void 
gplayer_imp::AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg)
{
	if(IS_HUMANSIDE(attack->ainfo.attacker))
	{
		dmg.adjust *= PVP_DAMAGE_REDUCE;
	}

	//随机挑选一件装备减少魂力
	if(abase::RandUniform() < SPIRIT_POWER_DEC_PROB)
	{
		int list[] = { item::EQUIP_INDEX_HEAD, item::EQUIP_INDEX_BODY, item::EQUIP_INDEX_FOOT};
		int count = sizeof(list) / sizeof(int);

		for(int i = 0; i < count; i ++)
		{
			int index = list[i];
			item & it = _equipment[index];
			if(it.type == -1 || it.body == NULL || it.body->GetSpiritPower(&it) <= 0) 
			{
				list[i] = list[count - 1];
				count --;
			}
		}
		
		if(count > 0)
		{
			int index = abase::Rand(0, count);
			DecSpiritPower(list[index]);
		}
	}
}

void
gplayer_imp::OnAttack()
{
	if(abase::RandUniform() < SPIRIT_POWER_DEC_PROB)
	{
		DecSpiritPower(item::EQUIP_INDEX_WEAPON);
	}
}

void 
gplayer_imp::SendServiceRequest(int service_type,const void * buf, size_t length)
{
	if( (!_pstate.IsNormalState() && !_pstate.IsBindState()) || IsSitDownMode()) return;
	service_executor * executor;
	if(CanUseService() && (executor = service_manager::GetExecutor(service_type)) )
	{
		if(!executor->ServeRequest(this,_provider.id, buf,length))
		{
			_runner->error_message(S2C::ERR_SERVICE_ERR_REQUEST);
		}
	}
	else
	{
		_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
	}
}

void 
gplayer_imp::QueryServiceContent(int service_type)
{
	if(CanUseService())
	{	
		struct 
		{
			int cs_index;
			int sid;
		} data;
		gplayer * pPlayer = (gplayer * )_parent;
		data.cs_index = pPlayer->cs_index;
		data.sid =  pPlayer->cs_sid;
		SendTo<0>(GM_MSG_SERVICE_QUIERY_CONTENT,_provider.id,service_type,&data,sizeof(data));
	}
	else
	{
		_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
	}
}

void gplayer_imp::GetGsRoleInfoForConsign(GMSV::role_basic_attribute & basic_attr, GMSV::role_deity_info & deity_info, raw_wrapper & title_info, raw_wrapper & skill_info, std::vector<GMSV::role_pet_prop_added> & pet_prop_added_list)
{
	//基本信息
	basic_attr.resistance1				= _cur_prop.resistance[0];
	basic_attr.resistance2				= _cur_prop.resistance[1];
	basic_attr.resistance3				= _cur_prop.resistance[2];
	basic_attr.resistance4				= _cur_prop.resistance[3];
	basic_attr.resistance5				= _cur_prop.resistance[4];
	basic_attr.level					= GetObjectLevel();
	basic_attr.cls						= GetParent()->GetClass();
	//现在转生次数最多只有一次，如果以后有多次转生这个地方代码有问题
	if(GetRebornCount() > 0)
	{
		basic_attr.level_before_reborn	= (short)((_reborn_list[0] >> 16) & 0xFFFF);
		basic_attr.cls_before_reborn	= (short)(_reborn_list[0] & 0xFFFF);
	}
	else
	{
		basic_attr.level_before_reborn	= 0;
		basic_attr.cls_before_reborn	= 0;
	}
	basic_attr.talisman_value			= GetTalismanValue();
	basic_attr.reputation				= GetParent()->reputation;
	basic_attr.pk_value					= GetPKValue();
	basic_attr.cultivation				= GetCultivation();
	basic_attr.exp						= _basic.exp;
	basic_attr.max_hp					= _cur_prop.max_hp;
	basic_attr.max_mp					= _cur_prop.max_mp;
	basic_attr.damage_low				= _cur_prop.damage_low;
	basic_attr.damage_high				= _cur_prop.damage_high;
	basic_attr.defense					= _cur_prop.defense;
	basic_attr.attack					= _cur_prop.attack;
	basic_attr.armor					= _cur_prop.armor;
	basic_attr.crit_damage				= _cur_prop.crit_damage;
	basic_attr.crit_rate				= _cur_prop.crit_rate;
	basic_attr.skill_armor_rate			= _cur_prop.skill_armor_rate;
	basic_attr.skill_attack_rate		= _cur_prop.skill_attack_rate;
	basic_attr.anti_crit				= _cur_prop.anti_crit;
	basic_attr.anti_crit_damage			= _cur_prop.anti_crit_damage;
	basic_attr.cult_defense_xian		= _cur_prop.cult_defense[0];
	basic_attr.cult_defense_mo			= _cur_prop.cult_defense[1];
	basic_attr.cult_defense_fo			= _cur_prop.cult_defense[2];
	basic_attr.battlescore				= _battle_score;
	basic_attr.reborn_count				= GetRebornCount();
	basic_attr.gender					= GetParent()->base_info.gender;

	//元神信息
	deity_info.atatck_xian				= _cur_prop.cult_attack[0];
	deity_info.atatck_mo				= _cur_prop.cult_attack[1];
	deity_info.atatck_fo				= _cur_prop.cult_attack[2];
	deity_info.deity_level				= GetParent()->dt_level;
	deity_info.deity_power				= _cur_prop.deity_power;
	deity_info.max_dp					= _cur_prop.max_dp;
	deity_info.deity_exp				= _basic.dt_exp;

	//尊号信息
	size_t title_count;
	const short * title					= GetPlayerTitleList(title_count);
	size_t extra_title_count;
	const short * extra_title			= GetPlayerExtraTitle(extra_title_count);
	title_info << (title_count + extra_title_count);
	for(size_t i = 0; i < title_count; i++)
	{
		title_info << (short)title[i];
	}
	for(size_t i = 0; i < extra_title_count; i++)
	{
		title_info << (short)extra_title[i];
	}

	//技能信息
	_skill.StorePartial(skill_info);

	//宠物pet_prop_added
	for(size_t index = 0; index <  _pet_bedge_inventory.Size(); index++)
	{
		if(_petman.CheckPetBedgeExist(this, index))
		{
			S2C::CMD::player_pet_prop_added data;
			_petman.GetPetPropAddedInfo(this, index, &data);

			GMSV::role_pet_prop_added petprop_added_data;
			petprop_added_data.pet_index			= data.pet_index;
			petprop_added_data.maxhp				= data.maxhp;
			petprop_added_data.maxmp				= data.maxmp;
			petprop_added_data.defence				= data.defence;
			petprop_added_data.attack				= data.attack;
			for(int i = 0; i < 6; i++)
			{
				petprop_added_data.resistance[i]	= data.resistance[i];
			}
			petprop_added_data.hit					= data.hit;
			petprop_added_data.jouk					= data.jouk;
			petprop_added_data.crit_rate			= data.crit_rate;
			petprop_added_data.crit_damage			= data.crit_damage;
			petprop_added_data.equip_mask			= data.equip_mask;

			pet_prop_added_list.push_back(petprop_added_data);
		}
	}
}

void gplayer_imp::GetGsRoleInfoForPlatform(GMSV::role_basic_attribute & basic_attr, GMSV::role_deity_info & deity_info, raw_wrapper & title_info, raw_wrapper & skill_info, std::vector<GMSV::role_pet_prop_added> & pet_prop_added_list, GMSV::role_basic_attribute_ext & ext)
{
	GetGsRoleInfoForConsign(basic_attr, deity_info, title_info, skill_info, pet_prop_added_list);
	ext.renxing1				= _cur_prop.resistance_tenaciy[0];
	ext.renxing2				= _cur_prop.resistance_tenaciy[1];
	ext.renxing3				= _cur_prop.resistance_tenaciy[2];
	ext.renxing4				= _cur_prop.resistance_tenaciy[3];
	ext.renxing5				= _cur_prop.resistance_tenaciy[4];

	ext.jingtong1				= _cur_prop.resistance_proficiency[0];
	ext.jingtong2				= _cur_prop.resistance_proficiency[1];
	ext.jingtong3				= _cur_prop.resistance_proficiency[2];
	ext.jingtong4				= _cur_prop.resistance_proficiency[3];
	ext.jingtong5				= _cur_prop.resistance_proficiency[4];
}

void 
gplayer_imp::PlayerLogout(int type, int offline_agent)
{
	if(_pstate.IsMarketState())
	{
		CancelPersonalMarket();
	}

	if(!_pstate.CanLogout() || IsCombatState() || type < 0)
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}

	GLog::log(GLOG_INFO,"用户%d执行登出逻辑%d",_parent->ID.id, type);
	ASSERT(_offline_type == 0);
	//这里调用玩家的登出操作  先用LostConnection 代替
	_team.PlayerLogout();
	CircleOfDoomPlayerLogout();

	offline_agent = (offline_agent && _offline_agent_time)?1:0;
	Logout(type, offline_agent);

	//向SNS平台同步角色信息 目前采用退出即同步的策略 后面可能会调整
	GMSV::role_basic_attribute basic_attr;
	GMSV::role_deity_info deity_info;
	raw_wrapper title_rp, skill_rp;
	std::vector<GMSV::role_pet_prop_added> pet_prop_added_list;
	GMSV::role_basic_attribute_ext ext;
	GetGsRoleInfoForPlatform(basic_attr, deity_info, title_rp, skill_rp, pet_prop_added_list, ext);
	GMSV::role_title_info title_info = {title_rp.size(), title_rp.data()};
	GMSV::role_skill_info skill_info = {skill_rp.size(), skill_rp.data()};
	GMSV::SendGsRoleInfo2Platform(GetParent()->ID.id, basic_attr, deity_info, title_info, skill_info, pet_prop_added_list, ext);

	GLog::money("money_hold:[roleid=%d,userid=%d]:moneyhold=%d:type=1:place=1:hint=0",_parent->ID.id,_db_magic_number,_player_money);
	GLog::money("money_hold:[roleid=%d,userid=%d]:moneyhold=%d:type=1:place=2:hint=0",_parent->ID.id,_db_magic_number,_trashbox.GetMoney());
}

void 
gplayer_imp::Logout(int type, int offline_agent)
{
	class UserInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		gplayer * _player;
		int _userid;
		int _type;
	public:
		UserInfoWriteBack(gplayer * pPlayer,int type):_player(pPlayer),_userid(pPlayer->ID.id),_type(type)
		{}

		virtual void OnTimeOut()
		{
			//目前并没有重新发送存盘请求
			GLog::log(GLOG_ERR,"下线时保存%d数据超时",_userid);
			OnPutRole(1);
		}

		virtual void OnFailed()
		{	
			//不会受到这个命令 
			GLog::log(GLOG_ERR,"下线时保存%d数据失败",_userid);
			OnPutRole(2);
		}
		
		virtual void OnPutRole(int retcode)
		{
			//ASSERT(retcode == 0);
			//写入磁盘成功
			_player->Lock();
			if(_player->ID.id !=  _userid || _player->login_state != gplayer::WAITING_LOGOUT) 
			{
				//角色寄售信息
				((gplayer_imp*)(_player->imp))->CheckToSendConsignRoleInfo(ERR_CONSIGN_ROLE_SAVEDATA_1);

				//忽略错误，直接返回
				_player->Unlock();
				GLog::log(GLOG_INFO,"写入用户%d发生状态不一致错误 id2:%d state:%d",_userid,_player->ID.id,_player->login_state);
				delete this;
				return ;
			}
			ASSERT(_player->imp);
			GLog::log(GLOG_INFO,"写入用户%d数据完成(%d)，发送登出数据%d",_userid,retcode,_type);

			//角色寄售信息
			if(0 == retcode)
			{
				((gplayer_imp*)(_player->imp))->CheckToSendConsignRoleInfo(CONSIGN_ROLE_SUCCESS);
			}
			else
			{
				((gplayer_imp*)(_player->imp))->CheckToSendConsignRoleInfo(ERR_CONSIGN_ROLE_SAVEDATA_2);
			}

			//将player对象删除
			((gplayer_imp*)(_player->imp))->SendLogoutRequest(_type);
			_player->imp->_commander->Release(); 
			_player->Unlock();
			record_leave_server(_userid);

			delete this;
		}

	};

	ASSERT(_pstate.CanLogout());

	/*liuyue-facbase
	int map_id = 0;
	if (GetWorldManager()->IsRaidWorld()) {
		map_id = GetWorldManager()->GetRaidID();
	}
	else
	{
		map_id = GetWorldTag();
	}
	*/
	int map_id = GetClientTag();
	int onlinetime = g_timer.get_systime() - _login_timestamp;
	GLog::formatlog("formatlog:rolelogout_gs:roleid=%d:userid=%d:occupation=%d:map_id=%d:x=%f:y=%f:z=%f:level=%d:reborn_cnt=%d:onlinetime=%d",_parent->ID.id, _db_magic_number, GetPlayerClass(),
		       	map_id, _parent->pos.x, _parent->pos.y, _parent->pos.z, GetObjectLevel(),GetRebornCount(), onlinetime); 

	//离开阵法
	CircleOfDoomPlayerLogout();

	_runner->leave_world();
	PlayerLeaveWorld();
	
	//还要考虑断线逻辑
	//用户进入断线逻辑，并开始存盘
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->login_state = gplayer::WAITING_LOGOUT;

	ClearSession();
	ClearSpiritSession();

	//先将用户从世界中删除 在commander->Release中会见用户从管理器中移出
	slice * pPiece = pPlayer->pPiece;
	if(pPiece) _plane->RemovePlayer(pPlayer);
	pPlayer->pPiece = NULL;

	_offline_agent_active = (_offline_agent_time > 0 &&  offline_agent)?1:0;

	try
	{
		if(type >= 0)
		{
			//写入磁盘	还没有考虑多写入写入失败的情况
			//这时强制保存仓库
			_trashbox.IncChangeCounter();
			user_save_data((gplayer*)_parent,new UserInfoWriteBack(pPlayer,type),2);
		}
		else
		{
			//角色寄售信息
			CheckToSendConsignRoleInfo(ERR_CONSIGN_ROLE_SAVEDATA_3);

			//出错情况，不进行写盘操作
			SendLogoutRequest(-1);
			_commander->Release(); 
		}
	}
	catch(...)
	{
		//角色寄售信息
		CheckToSendConsignRoleInfo(ERR_CONSIGN_ROLE_SAVEDATA_4);

		ASSERT(false);
	}
}

void 
gplayer_imp::DummyLogout(int type)
{
	class UserInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		gplayer * _player;
		int _userid;
		int _type;
	public:
		UserInfoWriteBack(gplayer * pPlayer,int type):_player(pPlayer),_userid(pPlayer->ID.id),_type(type)
		{}

		virtual void OnTimeOut()
		{
			//目前并没有重新发送存盘请求
			GLog::log(GLOG_ERR,"下线时保存%d数据超时",_userid);
			OnPutRole(1);
		}

		virtual void OnFailed()
		{	
			//不会受到这个命令 
			GLog::log(GLOG_ERR,"下线时保存%d数据失败",_userid);
			OnPutRole(2);
		}
		
		virtual void OnPutRole(int retcode)
		{
			//do nothing
			delete this;
		}

	};
	ASSERT(_pstate.CanLogout());

	gplayer * pPlayer = (gplayer*)_parent;
	try
	{
		//写入磁盘	还没有考虑多写入写入失败的情况
		//这时强制保存仓库
		_trashbox.IncChangeCounter();
		user_save_data((gplayer*)_parent,new UserInfoWriteBack(pPlayer,type),2);
	}
	catch(...)
	{
		ASSERT(false);
	}
}

void gplayer_imp::PlayerForceOffline()
{
	//这个时候由于正在等待写盘所以自然不能断线，到时候自然会断线
	//还要考虑必要的释放操作
	if(!_pstate.IsOnline()) return;
	if(_parent->pPiece) _runner->leave_world();

	gplayer * pPlayer = GetParent();
	if(pPlayer->login_state == gplayer::LOGIN_OK)
	{
		PlayerLeaveWorld();
	}
	
	//将用户从世界中删除
	_commander->Release(); 
}

void 
gplayer_imp::ServerShutDown()
{
	//关闭的原则是忽略战斗状态
	if(_parent->b_disconnect) return;
	ActiveCombatState(false);
	LostConnection(PLAYER_OFF_LPG_DISCONNECT);
}

void 
gplayer_imp::SendLogoutRequest(int type )
{
	gplayer * pPlayer  =(gplayer*)_parent;
	bool bRst;
	switch(_offline_type)
	{
		case PLAYER_OFF_LOGOUT:
		bRst = GMSV::SendLogout(pPlayer->cs_index, pPlayer->ID.id, pPlayer->cs_sid,type>=0?type:GMSV::PLAYER_LOGOUT_FULL);
		//ASSERT(bRst);
		//不再报告错误了，由于linkserver的连接随时可能会中断，因此这种情况的发生概率颇高
		break;

		case PLAYER_OFF_OFFLINE:
		bRst = GMSV::SendOfflineRe(pPlayer->cs_index, pPlayer->ID.id, pPlayer->cs_sid,0);
		//ASSERT(bRst);
		//不再报告错误了，由于linkserver的连接随时可能会中断，因此这种情况的发生概率颇高
		break;
		
		case PLAYER_OFF_KICKOUT:
		bRst = GMSV::SendKickoutRe(pPlayer->ID.id, pPlayer->cs_sid,0, pPlayer->id_zone);
		//ASSERT(bRst);
		//不再报告错误了，由于linkserver的连接随时可能会中断，因此这种情况的发生概率颇高
		break;

		case PLAYER_OFF_LPG_DISCONNECT:
		bRst = GMSV::SendDisconnect(pPlayer->cs_index,pPlayer->ID.id,pPlayer->cs_sid,0);
		break;

		case PLAYER_OFF_RECONNECT:
		bRst = GMSV::SendReconnectRe(pPlayer->ID.id, pPlayer->cs_sid,0); 
		GLog::log(GLOG_INFO,"[reconnect]: send reconnect login to delivery, roleid=%d", pPlayer->ID.id); 
		break;
	};
	
}

void
gplayer_imp::LostConnection(int offline_type)
{
	if(_parent->b_disconnect) return;
	_parent->b_disconnect = true;
	_offline_type = offline_type;
	//断线，根据各种状态进行操作
	if(_pstate.IsMarketState()) 
	{
		CancelPersonalMarket();
	}

	if(_pstate.IsBindState() || _pstate.IsNormalState())
	{
		if(!IsCombatState())
		{
			//非战斗状态，等待几秒钟退出，不再立刻退出了
			_pstate.Disconnect();
			_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
			//Logout(GMSV::PLAYER_LOGOUT_FULL);
		}
		else
		{
			//战斗状态
			//进入断线逻辑状态
			_pstate.Disconnect();
			_disconnect_timeout = LOGOUT_TIME_IN_COMBAT;
		}

		if(offline_type == PLAYER_OFF_RECONNECT)
		{
			_disconnect_timeout = LOGOUT_TIME_IN_RECONNECT;
		}	
	}
	else if(_pstate.IsWaitTradeState())
	{
		if(_trade_obj)
		{
			//直接忽略交易
			//进入logout 状态
			ReplyTradeRequest(false);
			FromTradeToNormal();
		}
		else if(_consign_obj)
		{
			FromConsignToNormal(false);
		}
	} 
	else if(_pstate.IsTradeState())
	{
		if(_trade_obj)
		{
			//交易状态，发送取消交易数据
			DiscardTrade();
			//进入断线等待状态
		}
		else if(_consign_obj)
		{
			_pstate.WaitTradeComplete();
			_consign_obj->SetTimeOut(10);
		}
	}
	else
	{
		//do nothing
	}

	//调用队伍的退出逻辑
	_team.PlayerLostConnection();

	//调用阵法的退出逻辑
	CircleOfDoomPlayerDisconnect();

	GLog::log(GLOG_INFO,"用户掉线: type=%d, usrid=%d, timeout=%d",offline_type,_parent->ID.id,_disconnect_timeout);
}

int 
gplayer_imp::TakeOutItem(int item_id, int count)
{
	if(item_id <= 0 || count <= 0) return -1;
	int rst = 0;
	while( count > 0 && (rst = _inventory.Find(rst,item_id)) >= 0)
	{
		item & it = _inventory[rst];
		int use_count = count; 
		if(use_count > (int)it.count) use_count = it.count;
		UseItemLog(_inventory[rst], use_count);
		_inventory.DecAmount(rst,use_count);
		_runner->player_drop_item(IL_INVENTORY,rst,item_id,use_count,S2C::DROP_TYPE_TAKEOUT);
		count -= use_count;
	}
	return rst;
}

int 
gplayer_imp::TakeOutNonExpireItem(int item_id, int count)
{
	if(item_id <= 0 || count <= 0) return -1;

	for(size_t i = 0; i < _inventory.Size(); ++i)
	{
		if(_inventory[i].type == item_id && _inventory[i].expire_date == 0)
		{
			item & it = _inventory[i];
			int use_count = count; 
			if(use_count > (int)it.count) use_count = it.count;

			UseItemLog(_inventory[i], use_count);
			_inventory.DecAmount(i,use_count);
			_runner->player_drop_item(IL_INVENTORY,i,item_id,use_count,S2C::DROP_TYPE_TAKEOUT);
			count -= use_count;
			if(count <= 0) return count;
		}
	}
	return count;
}


int
gplayer_imp::TakeOutItem(int inv_index, int item_id, int count)
{
	if(item_id <=0 || count <=0 || inv_index <0) return -1;
	if(_inventory.Size() < (size_t)(inv_index+1)) return -1;
	item &it = _inventory[inv_index];
	
	if(it.type != item_id) return -1;
	if(it.count < (size_t)count) count = it.count;
	UseItemLog(_inventory[inv_index], count);
	
	_inventory.DecAmount(inv_index, count);
	_runner->player_drop_item(IL_INVENTORY,inv_index,item_id,count,S2C::DROP_TYPE_TAKEOUT);

	return inv_index;
}

int
gplayer_imp::TakeOutEquipItem(int equip_index, int item_id, int count)
{
	if(item_id <=0 || count <=0 || equip_index<0) return -1;
	if(_equipment.Size() < (size_t)(equip_index+1)) return -1;
	item &it = _equipment[equip_index];
	
	if(it.type != item_id) return -1;
	if(it.count < (size_t)count) count = it.count;
	
	_equipment.DecAmount(equip_index, count);
	_runner->player_drop_item(IL_EQUIPMENT,equip_index,item_id,count,S2C::DROP_TYPE_TAKEOUT);

	RefreshEquipment();
	
	//发出更改信息
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1<<equip_index,0,0); //此函数使用了CalcEquipmentInfo的结果

	return equip_index;

}

int
gplayer_imp::TakeOutFashionItem(int fas_index, int item_id, int count)
{
	if(item_id <=0 || count <=0 || fas_index <0) return -1;
	if(_fashion_inventory.Size() < (size_t)(fas_index+1)) return -1;
	item &it = _fashion_inventory[fas_index];
	
	if(it.type != item_id) return -1;
	if(it.count < (size_t)count) count = it.count;
	UseItemLog(_fashion_inventory[fas_index], count);
	
	_fashion_inventory.DecAmount(fas_index, count);
	_runner->player_drop_item(IL_FASHION,fas_index,item_id,count,S2C::DROP_TYPE_TAKEOUT);

	return fas_index;
}

int
gplayer_imp::TakeOutTrashItem(int tra_index, int item_id, int count)
{
	item_list & box = _trashbox.Backpack();
	if(item_id <=0 || count <=0 || tra_index <0) return -1;
	if(box.Size() < (size_t)(tra_index+1)) return -1;
	item &it = box[tra_index];
	
	if(it.type != item_id) return -1;
	if(it.count < (size_t)count) count = it.count;
	UseItemLog(box[tra_index], count);
	
	box.DecAmount(tra_index, count);
	_runner->player_drop_item(IL_TRASH_BOX,tra_index,item_id,count,S2C::DROP_TYPE_TAKEOUT);

	return tra_index;
}

int gplayer_imp::TakeOutGiftItem(int item_index, int item_id, int count)
{
	if(item_id <=0 || count <=0 || item_index <0) return -1;
	if((size_t)item_index >= _gift_inventory.Size()) return -1;
	item &it = _gift_inventory[item_index];
	
	if(it.type != item_id) return -1;
	if(it.count < (size_t)count) count = it.count;
	
	_gift_inventory.DecAmount(item_index, count);
	return item_index;
}

int 
gplayer_imp::TakeOutGiftItem(int item_id, int count)
{
	if(item_id <= 0 || count <= 0) return -1;
	int rst = 0;
	while( count > 0 && (rst = _gift_inventory.Find(rst,item_id)) >= 0)
	{
		item & it = _gift_inventory[rst];
		int use_count = count; 
		if(use_count > (int)it.count) use_count = it.count;
		_gift_inventory.DecAmount(rst,use_count);
		count -= use_count;
	}
	return rst;
}

int gplayer_imp::UseItemByID(gplayer_imp * pImp, int where, int item_id,size_t num, int use_type)
{
	item_list & inv =  GetInventory(where);
	int idx = 0;
	while(num && (idx = inv.Find(idx, item_id)) >= 0)
	{
		size_t count = num;
		if(inv[idx].count < count) count = inv[idx].count;
		pImp->UseItemLog(inv[idx], count);
		inv.DecAmount(idx,count);
		_runner->player_drop_item(where, idx, item_id, count, use_type);
		num -= count;
		idx ++;
	}
	return idx;
}

bool 
gplayer_imp::CheckItemExist(int item_id, size_t count)
{
	int rst = 0;
	while((rst = _inventory.Find(rst,item_id)) >= 0)
	{
		if(_inventory[rst].count >= count) return true;
		count -= _inventory[rst].count;
		rst ++;
	}
	return false;
}

//没有限时物品
bool 
gplayer_imp::CheckNonExpireItemExist(int item_id, size_t count)
{
	for(size_t i = 0; i < _inventory.Size(); ++i)
	{
		if(_inventory[i].type == item_id && _inventory[i].expire_date == 0)
		{
			if( _inventory[i].count >= count) return true;
			count -= _inventory[i].count;
		}
	}
	return false;
}

bool 
gplayer_imp::CheckItemExist(int inv_index, int item_id, size_t count)
{
	return IsItemExist(inv_index,item_id,count);
}

void 
gplayer_imp::SendMsgToTeam(const MSG & msg, float range, bool exclude_self, bool norangelimit, int max_count)
{
	ASSERT(IsInTeam());
	if(exclude_self)
		_team.SendMessage(msg,range, norangelimit, max_count);
	else
		_team.SendAllMessage(msg,range, norangelimit, max_count);
}

bool 
gplayer_imp::EmbedChipToEquipment(size_t chip,size_t equip)
{
	_runner->error_message(S2C::ERR_CANNOT_EMBED);
	return false;
}

bool 
gplayer_imp::DrainMana(int mana)
{
	if(IsRenMa()) return false;

	_filters.EF_AdjustManaCost(mana);
	if(_basic.mp >= mana)
	{
		_basic.mp -= mana;
		SetRefreshState();
		return true;
	}
	else
	{
		if(_basic.mp) SetRefreshState();
		_basic.mp = 0;
	}
	return false;
}

bool 
gplayer_imp::DecDeity(int deity)
{
	if(_basic.dp >= deity)
	{
		_basic.dp -= deity;
		SetRefreshState();
		return true;
	}
	else
	{
		if(_basic.dp) SetRefreshState();
		_basic.dp = 0;
	}
	return false;
}

bool 
gplayer_imp::DecInk(int ink)
{
	if(_basic.ink >= ink)
	{
		_basic.ink -= ink;
		SetRefreshState();
		return true;
	}
	else
	{
		if(_basic.ink) SetRefreshState();
		_basic.ink = 0;
	}
	return false;
}

bool 
gplayer_imp::PlayerUseItem(int where, size_t inv_index, int item_type,size_t count)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT && where != IL_MOUNT_WING) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].CanUse(inv.GetLocation(),this)) return false;
	if(!inv[inv_index].CheckZoneUse(inv.GetLocation(),this)) return false;
	if(IsZoneServer() && inv[inv_index].proc_type);
	int rst = inv[inv_index].GetUseDuration();
	if(rst < 0)
	{
		UseItem(inv,inv_index,where,item_type);
		return true;
	}
	else
	{
		//战斗状态下增加下面这些传送符的传送时间
		if((item_type == 23865 || item_type == 23319) && IsCombatState())
		{
			rst = SECOND_TO_TICK(8);
		}
		else if((item_type == 55426 || item_type == 55427) && IsCombatState())
		{
			rst = SECOND_TO_TICK(10);
		}

		if(_filters.IsFilterExist(FILTER_SPIRITDRAG) || _filters.IsFilterExist(FILTER_PULL))
		{
			return false;
		}	
		if(rst > 0 && _session_state == STATE_SESSION_USE_ITEM) 
		{
			//如果已经在使用物品了，此项使用物品则不进行
			return true;
		}
		AddStartSession(new session_use_item(this,where,inv_index,item_type,count,rst));
		return true;
	}
}

bool 
gplayer_imp::PlayerUseItemWithArg(int where,size_t inv_index,int item_type,size_t count, const char * buf, size_t buf_size)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].CanUseWithArg(inv.GetLocation(),buf_size,this)) return false;
	if(!inv[inv_index].CheckZoneUse(inv.GetLocation(),this)) return false;
	int rst = inv[inv_index].GetUseDuration();
	if(rst < 0)
	{
		UseItemWithArg(inv,inv_index,where,item_type,buf , buf_size);
		return true;
	}
	else
	{
		if((item_type == 35330 || item_type == 23339) && IsCombatState())
		{
			rst = SECOND_TO_TICK(8);
		}
		if(rst > 0 && _session_state == STATE_SESSION_USE_ITEM) 
		{
			//如果已经在使用物品了，此项使用物品则不进行
			return true;
		}
		
		//AddStartSession(new session_use_item(this,where,inv_index,item_type,count,rst));
		//$$$$$$$$$$  现在暂时没有执行时间的参数使用物品
		return true;
	}
}


bool 
gplayer_imp::PlayerSitDownUseItem(int where, size_t inv_index, int item_type,size_t count)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].SitDownCanUse(inv.GetLocation(),this)) return false;
	if(!inv[inv_index].CheckZoneUse(inv.GetLocation(),this)) return false;
	int rst = inv[inv_index].GetUseDuration();
	if(rst >= 0) return false;
	return UseItem(inv,inv_index,where,item_type);
}

bool 
gplayer_imp::PlayerUseItemWithTarget(int where,size_t inv_index,int item_type,char force_attack)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	XID cur_target = ((gplayer_controller*)_commander)->GetCurTarget();
	if(!cur_target.IsActive()) return false;

	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].CanUseWithTarget(inv.GetLocation(),this)) return false;
	if(!inv[inv_index].CheckZoneUse(inv.GetLocation(),this)) return false;
	int rst = inv[inv_index].GetUseDuration();
	if(rst < 0)
	{
		return UseItemWithTarget(inv,inv_index,where, item_type, cur_target,force_attack);
	}
	else
	{
		if((item_type == 35330 || item_type == 23339) && IsCombatState())
		{
			rst = SECOND_TO_TICK(8);
		}
		session_use_item_with_target * pSession = new session_use_item_with_target(this,where,inv_index,item_type,1,rst);
		pSession->SetTarget(cur_target,force_attack);
		AddStartSession(pSession);
		return true;
	}
	return false;
}

bool
gplayer_imp::ReturnToTown()
{
	A3DVECTOR pos;
	int world_tag;
	if(!GetWorldManager()->GetTownPosition(this,_parent->pos,pos,world_tag)) return false;
	LongJump(pos,world_tag);
	return true;
}

bool 
gplayer_imp::PlayerSitDown()
{
	if(!CanSitDown()) return false;

	//坐下操作应该被作为session而被排入队列 
	AddStartSession(new session_sit_down(this));
	return true;
}

bool
gplayer_imp::PlayerStandUp()
{
	//可以立刻起立
	//坐下操作，内部调用
	if(!IsSitDownMode()) return false;
	StandUp();
	return true;
}

bool 
gplayer_imp::CanSitDown()
{
	//要判断是否能够坐下
	//if(IsCombatState()) return false;
	//if(!_layer_ctrl.CanSitDown()) return false;
	if(!_pstate.CanSitDown()) return false;
	if(_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER)) return false;
	return true;
}

bool
gplayer_imp::SitDown()
{
	//坐下操作，内部调用
	if(!CanSitDown()) return false;

	//要清空后面的所有session
	ClearNextSession();

	//加入一个自己的session来进入和保持打坐状态
	AddStartSession(new session_keeping_sitdown(this));
	return true;
}

void
gplayer_imp::StandUp()
{
	ClearSession();
	//坐下操作，内部调用
	LeaveStayInState();
}

void gplayer_imp::OnSetSealMode()
{
	_idle_player_state = CalcIdlePlayerState();
	if (_root_seal_flag)
	{
		_mobactiveman.PlayerMobActiveCancel(this);
	}
}

void 
gplayer_imp::SetCombatState()
{
	ActiveCombatState(true);
	_combat_timer = MAX_COMBAT_TIME;
}

void 
gplayer_imp::AddAggroToEnemy(const XID & who,int rage)
{
	size_t count = _enemy_list.size();
	if(!count || rage <= 0) return;
	XID list[MAX_PLAYER_ENEMY_COUNT];
	ENEMY_LIST::iterator it = _enemy_list.begin(); 
	for(size_t i = 0;it != _enemy_list.end();i ++, ++it )
	{
		MAKE_ID(list[i],it->first);
	}

	msg_aggro_info_t info;
	info.source = who;
	info.aggro = rage;
	info.aggro_type = 0;
	info.faction = 0xFFFFFFFF;
	info.level = 0;

	MSG msg;
	BuildMessage(msg,GM_MSG_GEN_AGGRO,XID(-1,-1),who,_parent->pos,0,&info,sizeof(info));

	gmatrix::SendMessage(list, list + count, msg);
}

void
gplayer_imp::BeTaunted2(const XID & who,int time)
{
	_runner->be_taunted2(who, time);
}


void 
gplayer_imp::PlayerRestartSession()
{
	if(_cur_session)
	{
		if(!_cur_session->RestartSession())
		{
			EndCurSession();
			StartSession();
		}
	}
}

void gplayer_imp::DisappearOnDrop(item_list & inv,int index, int where,int max_count)
{
	bool bFlag = false;
	//首先是破碎逻辑
	if(g_config.item_broken_id || inv[index].pile_limit == 1)
	{
		DestroyBindItem(index, inv[index].type,  inv, where , 1);
		bFlag =  true;
	}
	else
	{
		int count = max_count;
		if((size_t)count > inv[index].count) count = inv[index].count;
		_runner->player_drop_item(where,index,inv[index].type,count,S2C::DROP_TYPE_DEATH_DESTROY);
		UseItemLog(inv[index], count);
		inv.DecAmount(index, count);
		bFlag =  false;
	}

	if(where == IL_EQUIPMENT)
	{
		//重新刷新装备
		RefreshEquipment();

		//发出更改信息
		CalcEquipmentInfo();
		int64_t bid = g_config.item_broken_id;
		if(bFlag)
			_runner->equipment_info_changed(1 << index ,1<<index,&bid,sizeof(bid)); //此函数使用了CalcEquipmentInfo的结果
		else
			_runner->equipment_info_changed(0,1<<index,0,0); //此函数使用了CalcEquipmentInfo的结果

	}
}
static bool TestIsDropItem(const item & it)
{
	if(g_config.item_broken_active)
	{
		if(!it.CanDrop()) return false;
	}
	else
	{
		if(!it.CanDrop() || !it.CanTrade()) return false;
	}
	//破碎的和正在销毁的东西是不会掉落的
	if(g_config.item_broken_id && it.type == g_config.item_broken_id) return false;
	if(g_config.item_destroy_id && it.type == g_config.item_destroy_id) return false;
	return true;
}

void 
gplayer_imp::DropItemOnDeath(size_t inv_drop, size_t eq_drop, const XID & owner , int team_id, int team_seq)
{
	ASSERT(inv_drop + eq_drop);
	
	if(inv_drop)
	{
		abase::vector<int,abase::fast_alloc<> > inv_list;
		inv_list.reserve(_inventory.Size() + item::EQUIP_INVENTORY_COUNT);
		for(size_t i = 0; i < _inventory.Size(); i ++)
		{
			if(_inventory[i].type == -1) continue;
			if(!TestIsDropItem(_inventory[i])) continue;
			inv_list.push_back(i);
		}
		for(size_t i = 0; i < inv_drop;i ++)
		{
			if(inv_list.size())
			{
				int index = abase::Rand(0,inv_list.size()-1);
				int drop_id = inv_list[index];
				if(_inventory[drop_id].IsEraseOnDrop())
				{
					DisappearOnDrop(_inventory, drop_id, IL_INVENTORY, MAX_ITEM_DROP_COUNT);
				}
				else
				{
					ThrowInvItem(drop_id ,MAX_ITEM_DROP_COUNT,S2C::DROP_TYPE_DEATH,owner,team_id,team_seq);
				}
				inv_list.erase_noorder(inv_list.begin() + index);
				continue;
			}
			break;
		}
	}

	if(eq_drop)
	{
		abase::vector<int,abase::fast_alloc<> > equip_list;
		equip_list.reserve(item::EQUIP_INVENTORY_COUNT);
		for(size_t i = 0; i < item::EQUIP_INVENTORY_COUNT; i ++)
		{
			if(_equipment[i].type == -1) continue;
			if(!TestIsDropItem(_equipment[i])) continue;
			equip_list.push_back(i);
		}


		for(size_t i = 0; i < eq_drop;i ++)
		{
			if(equip_list.size())
			{
				int index = abase::Rand(0,equip_list.size()-1);
				int drop_id = equip_list[index];
				if(_equipment[drop_id].IsEraseOnDrop())
				{
					DisappearOnDrop(_equipment, drop_id, IL_EQUIPMENT, MAX_ITEM_DROP_COUNT);
				}
				else
				{
					ThrowEquipItem(drop_id,S2C::DROP_TYPE_DEATH ,owner, team_id, team_seq);
				}
				equip_list.erase_noorder(equip_list.begin() + index);
				continue;
			}
			break;
		}
	}
}
/*
void 
gplayer_imp::DropItemOnDeath(size_t drop_count_inv,size_t drop_count_equip)
{
	ASSERT(drop_count_inv + drop_count_equip);
	
	abase::vector<int,abase::fast_alloc<> > inv_list;
	abase::vector<int,abase::fast_alloc<> > equip_list;
	if(drop_count_inv)
	{
		inv_list.reserve(_inventory.Size() + item::EQUIP_INVENTORY_COUNT);
		for(size_t i = 0; i < _inventory.Size(); i ++)
		{
			if(_inventory[i].type == -1) continue;
			if(!TestIsDropItem(_inventory[i])) continue;
			inv_list.push_back(i);
		}
		for(size_t i = 0; i < drop_count_inv;i ++)
		{
			if(inv_list.size())
			{
				int index = abase::Rand(0,inv_list.size()-1);
				int drop_id = inv_list[index];
				if(_inventory[drop_id].IsEraseOnDrop())
				{
					DisappearOnDrop(_inventory, drop_id, IL_INVENTORY, MAX_ITEM_DROP_COUNT);
				}
				else
				{
					ThrowInvItem(drop_id ,MAX_ITEM_DROP_COUNT,false,S2C::DROP_TYPE_DEATH);
				}
				inv_list.erase_noorder(inv_list.begin() + index);
				continue;
			}
			break;
		}
	}

	if(drop_count_equip)
	{
		equip_list.reserve(item::EQUIP_INVENTORY_COUNT);
		for(size_t i = 0; i < item::EQUIP_INVENTORY_COUNT; i ++)
		{
			if(i == item::EQUIP_INDEX_WEAPON) continue;
			if(_equipment[i].type == -1) continue;
			if(!TestIsDropItem(_equipment[i])) continue;
			equip_list.push_back(i);
		}


		for(size_t i = 0; i < drop_count_equip;i ++)
		{
			if(equip_list.size())
			{
				int index = abase::Rand(0,equip_list.size()-1);
				int drop_id = equip_list[index];
				if(_equipment[drop_id].IsEraseOnDrop())
				{
					DisappearOnDrop(_equipment, drop_id, IL_EQUIPMENT, MAX_ITEM_DROP_COUNT);
				}
				else
				{
					ThrowEquipItem(drop_id ,false,S2C::DROP_TYPE_DEATH);
				}
				equip_list.erase_noorder(equip_list.begin() + index);
				continue;
			}
			break;
		}
	}
}
*/

void 
gplayer_imp::PlayerAssistSelect(const XID & cur_target)
{
	if(cur_target != _parent->ID && IsMember(cur_target))
	{
		SendTo<0>(GM_MSG_QUERY_SELECT_TARGET,cur_target,0);
	}
}

void 
gplayer_imp::SendInfo00(link_sid *begin, link_sid *end)
{
	ASSERT(begin != end);
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<CMD::player_info_00>::From(h1,_parent->ID,_basic.hp,_basic,_cur_prop,IsCombatState()?1:0);
	send_ls_msg(begin, end, h1.data(), h1.size());
}

void 
gplayer_imp::SendBuffData(link_sid *begin, link_sid *end)
{
	packet_wrapper h1(64);
	using namespace S2C;
	//_backup_hp[1]

	size_t count1;
	const buff_t * pBuff1 = _buff.GetBuff(count1);
	CMD::Make<CMD::self_buff_notify>::From(h1,_parent->ID,pBuff1,count1);
	//发给自己
	send_ls_msg(((gplayer_dispatcher*)_runner)->_header,h1);

	//只有有选中和组队才会发送下面的协议
	if(begin != end || IsInTeam());
	{
		h1.clear(); 
		size_t count;
		const buff_t_s * pBuff = _buff.GetBuffForElse(count);
		CMD::Make<CMD::object_buff_notify>::From(h1,_parent->ID,pBuff,count);
		
		//发给选中对象
		if(begin != end) send_ls_msg(begin, end, h1.data(),h1.size());
		
		//发给队友
		if(IsInTeam()) _team.SendGroupData(h1.data(),h1.size());
	}

}

void 
gplayer_imp::SendTeamDataToMembers()
{
	ASSERT(IsInTeam());

	if(!_buff.BuffExist()) return ;
	packet_wrapper h1(64);
	using namespace S2C;
	size_t count;
	const buff_t_s * pBuff = _buff.GetBuffForElse(count);
	CMD::Make<CMD::object_buff_notify>::From(h1,_parent->ID,pBuff,count);
	_team.SendGroupData(h1.data(),h1.size());
}

void 
gplayer_imp::Swap(gplayer_imp * rhs)
{
	((gplayer_controller *)_commander)->LoadFrom((gplayer_controller*)rhs->_commander);
	((gplayer_dispatcher*)_runner)->LoadFrom((gplayer_dispatcher*)rhs->_runner);

#define Set(var,cls) var = cls->var

	// Youshuang add
	Set(_fashion_weapon_addon_id,rhs);
	Set(_faction_coupon,rhs);
	Set(_fac_coupon_add,rhs);
	Set(_newyear_info,rhs);
	Set(_used_title,rhs);
	// end
	Set(_battle_faction,rhs);
	Set(_battle_score,rhs);
	Set(_killingfield_score,rhs);
	Set(_arena_score,rhs);
	Set(_raid_faction, rhs);
	Set(_player_money,rhs);
	Set(_combat_timer,rhs);
	Set(_provider,rhs);
	Set(_money_capacity,rhs);
	Set(_inv_level ,rhs);
	Set(_stall_trade_id,rhs);
	Set(_last_move_mode,rhs);
	Set(_pvp_flag_enable,rhs);
	Set(_kill_by_player,rhs);
	Set(_resurrect_exp_reduce,rhs);
	Set(_resurrect_timestamp,rhs);
	Set(_resurrect_state,rhs);
	Set(_db_save_error,rhs);
	Set(_pvp_combat_timer,rhs);
	Set(_username_len,rhs);
	memcpy(_username,rhs->_username,sizeof(_username));
	Set(_login_timestamp,rhs);
	Set(_played_time,rhs);
	Set(_db_timestamp,rhs);
	Set(_duel_target, rhs);
	Set(_idle_player_state, rhs);
	Set(_item_poppet_counter,rhs);
	Set(_exp_poppet_counter, rhs);
	Set(_produce_level, rhs);
	Set(_produce_exp, rhs);
	Set(_mafia_contribution, rhs);
	Set(_mafia_join_time, rhs);
	Set(_family_contribution, rhs);
	Set(_invader_counter, rhs);
	Set(_sanctuary_mode, rhs);
	Set(_mduel, rhs);
	Set(_recorder_timestamp, rhs);
	Set(_another_day_timestamp, rhs);
	Set(_logout_time, rhs);
	Set(_create_time, rhs);
	Set(_pstate, rhs);
	Set(_mcontrol, rhs);
	Set(_wallow_level, rhs);
	Set(_cheat_punish, rhs);
	Set(_cheat_mode, rhs);
	Set(_cheat_report, rhs);
	Set(_chat_emote,rhs);
	Set(_talisman_info, rhs);
	Set(_fly_info, rhs);
	Set(_db_magic_number, rhs);
	Set(_ip_address, rhs);
	Set(_sect_id, rhs);
	Set(_sect_init_level, rhs);
	Set(_cultivation, rhs);
	Set(_offline_agent_time, rhs);
	Set(_last_db_save_time, rhs);
	Set(_referrer_id, rhs);
	Set(_vipstate, rhs);
	Set(_achieve_timer, rhs);
	Set(_achieve_cache, rhs);
	Set(_pvp_mask, rhs);
	Set(_talent_point, rhs);
	Set(_combo_skill, rhs);	
	Set(_treasure_info, rhs);
	Set(_little_pet, rhs);
	Set(_fuwen_compose_info, rhs);
	Set(_item_multi_exp, rhs);
	Set(_fac_multi_exp, rhs);
	Set(_active_rune, rhs);
	Set(_rune_score, rhs);
	Set(_active_rune_level, rhs);
	Set(_flag_mask, rhs);
	Set(_puppet_immune_prob, rhs);
	Set(_puppet_skill_id, rhs);
	Set(_no_check_move, rhs);
	
	memcpy(_region_reputation, rhs->_region_reputation, sizeof(_region_reputation));
	_inventory.Swap(rhs->_inventory);
	_equipment.Swap(rhs->_equipment);
	_task_inventory.Swap(rhs->_task_inventory);
	_trashbox.Swap(rhs->_trashbox);
	_pet_bedge_inventory.Swap(rhs->_pet_bedge_inventory);
	_pet_equip_inventory.Swap(rhs->_pet_equip_inventory);
	_pocket_inventory.Swap(rhs->_pocket_inventory);
	_fashion_inventory.Swap(rhs->_fashion_inventory);
	_mount_wing_inventory.Swap(rhs->_mount_wing_inventory);
	_gift_inventory.Swap(rhs->_gift_inventory);
	_fuwen_inventory.Swap(rhs->_fuwen_inventory);
	_repurchase_inventory.swap(rhs->_repurchase_inventory);
	_team.Swap(rhs->_team);
	_duel.Swap(rhs->_duel);
	_fh_duel.Swap(rhs->_fh_duel);
	_bind_player.Swap(rhs->_bind_player);
	_active_emote_player.Swap(rhs->_active_emote_player);
	_link_ride_player.Swap(rhs->_link_ride_player);	
	_link_bath_player.Swap(rhs->_link_bath_player);	
	_link_qilin_player.Swap(rhs->_link_qilin_player);	
	_recipe_list.swap(rhs->_recipe_list);
	_dmg_list.swap(rhs->_dmg_list);
	_title_list.swap(rhs->_title_list);
	_extra_title.swap(rhs->_extra_title);
	_mallinfo.Swap(rhs->_mallinfo);
	_bonusinfo.Swap(rhs->_bonusinfo);
	//_wallow_obj.Swap(rhs->_wallow_obj);
	_reborn_list.swap(rhs->_reborn_list);
	
	_cooldown.Swap(rhs->_cooldown);
	_ph_control.Swap(rhs->_ph_control);


	_task_mask = rhs->_task_mask;
	_active_task_list.swap(rhs->_active_task_list);
	_finished_task_list.swap(rhs->_finished_task_list);
	_finished_time_task_list.swap(rhs->_finished_time_task_list);


	_cur_ins_key_list.swap(rhs->_cur_ins_key_list);
	_team_ins_key_list.swap(rhs->_team_ins_key_list);
	_cur_tag_counter.swap(rhs->_cur_tag_counter);
	_ins_key_timer.swap(rhs->_ins_key_timer);

	_petman.Swap(rhs->_petman);
	_fashion_hotkey.swap(rhs->_fashion_hotkey);
	_summonman.Swap(rhs->_summonman);
	_mobactiveman.Swap(rhs->_mobactiveman);
	_achieve_man.swap(rhs->_achieve_man);
	 abase::swap(_transform_obj, rhs->_transform_obj);
	 abase::swap(_title, rhs->_title);

	_circle_of_doom.Swap( rhs->_circle_of_doom );
	_vip_award.Swap( rhs->_vip_award );
	_online_award.Swap(rhs->_online_award);
	_permit_area.swap(rhs->_permit_area);
	_limit_area.swap(rhs->_limit_area);
	_raid_counters.swap(rhs->_raid_counters);
	_ui_transfer_info.Swap(rhs->_ui_transfer_info);
	_liveness_info.Swap(rhs->_liveness_info);
	_sale_promotion_info.Swap(rhs->_sale_promotion_info);

	Set(_time_raid_counters_reset, rhs);	
	Set(id_spirit_drag_me, rhs);
	Set(id_pulling_me, rhs);
	Set(_is_consign_role, rhs);

	Set(_annu_five_star, rhs);
	Set(_time_annu_five_award, rhs);
	Set(_annu_sign_state, rhs);

	_tower_raid_info.Swap(rhs->_tower_raid_info);
	memcpy(_pk_bets, rhs->_pk_bets, sizeof(_pk_bets));

	Set(_1st_bet, rhs);
	Set(_top3_bet, rhs);
	Set(_1st_bet_reward_deliveried, rhs);
	Set(_1st_bet_reward_result_deliveried, rhs);
	Set(_top3_bet_reward_deliveried, rhs);
	memcpy(_top3_bet_reward_result_deliveried, rhs->_top3_bet_reward_result_deliveried, sizeof(_top3_bet_reward_result_deliveried));

	Set(_consumption_value, rhs);

	Set(_dark_light, rhs);
	Set(_dark_light_form, rhs);
	Set(_astrology_info, rhs);
	Set(_collision_info, rhs);

	_spirit_session_list.swap(rhs->_spirit_session_list);
	Set(_check_buffarea_time, rhs);
	memcpy(_prop_add, rhs->_prop_add, sizeof(_prop_add));
	Set(_kingdom_title, rhs);
	_phase_info.swap(rhs->_phase_info);
	_exchange_cs6v6_award_info.swap(rhs->_exchange_cs6v6_award_info);
	
	Set(_mafia_name_len,rhs);
	memcpy(_mafia_name,rhs->_mafia_name,sizeof(_mafia_name));

	Set(_hide_and_seek_info,rhs);


#undef Set
	gactive_imp::Swap(rhs);
}

void 
gplayer_imp::QueryOtherPlayerInfo1(size_t count, int ids[])
{	

	_runner->begin_transfer();
	gplayer_dispatcher * pRunner = (gplayer_dispatcher * )_runner;
	using namespace S2C;
	for(size_t i = 0; i < count; i ++)
	{
		int id =  ids[i];
		gplayer * pPlayer = _plane->GetPlayerByID(id);
		if(!pPlayer) continue;
		if(pPlayer->IsActived() && pPlayer->pos.horizontal_distance(_parent->pos) < 150*150 &&
				(( (gplayer*)_parent)->anti_invisible_rate >= pPlayer->invisible_rate
				   || IsInTeam() && IsMember(XID(GM_TYPE_PLAYER,id) ))) 
		{
			if(CMD::Make<INFO::player_info_1>::From(pRunner->_pw,pPlayer))
			{
				pRunner->_pw._counter ++;
			}
		}
	}
	_runner->end_transfer();

	_runner->player_darklight();
}

void 
gplayer_imp::QueryNPCInfo1(size_t count, int ids[])
{	
	abase::vector<XID, abase::fast_alloc<> > extern_npc_list;
	gplayer_dispatcher * pRunner = (gplayer_dispatcher * )_runner;
	using namespace S2C;
	for(size_t i = 0; i < count; i ++)
	{
		//npc的处理先简单一些
		world::object_info info;
		XID  id(GM_TYPE_NPC,ids[i]);
		if(_plane->QueryObject(id,info))
		{
			if(info.pos.horizontal_distance(_parent->pos) < 150*150)
			{
				//收集该对象，供后面发出查询消息
				extern_npc_list.push_back(id);
			}
		}
	}

	//发出给玩家的消息 这些消息肯定都是外部消息
	if(extern_npc_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_QUERY_INFO_1,XID(-1,-1),_parent->ID,_parent->pos,pRunner->_header.cs_id,&(pRunner->_header.cs_sid),sizeof(int));
		gmatrix::SendMessage(extern_npc_list.begin(),extern_npc_list.end(),msg);
	}
}

bool gplayer_imp::IsMafiaHeader(char rank)
{
	return rank == 2;
}

bool gplayer_imp::IsFamilyHeader(char rank)
{
	return rank >=2 && rank <=5;
}

void 
gplayer_imp::UpdateMafiaInfo(int m_id,int f_id, char rank)
{
	gplayer * pPlayer = (gplayer*)_parent;
	//帮主传位
	if(IsMafiaHeader(pPlayer->rank_mafia)  && !IsMafiaHeader(rank)) 
	{
		PlayerTaskInterface tf(this);
		OnTaskPlayerLeaveFactionHeader(&tf);
	}

	//族长退位
	if(IsFamilyHeader(pPlayer->rank_mafia) && !IsFamilyHeader(rank))
	{
		PlayerTaskInterface tf(this);
		OnTaskPlayerLeaveFamilyHeader(&tf);
	}

	//如果帮派发生变化 先计算一下当前双倍时间
	if(pPlayer->id_mafia != m_id) 
	{
		_mafia_join_time = g_timer.get_systime();
		SetMafiaContribution(0);

		//检查当前是否出于战斗状态
		//更换帮派当然要去除现在的帮战状态
		if(_mduel.InBattle()) _mduel.ClearBattle();
	
		// 只要有帮派信息改变就强制更新敌对帮派信息
		_fh_duel.Refresh( this, m_id, true );
	}
	pPlayer->id_mafia = m_id;

	pPlayer->rank_mafia = rank;
	if( pPlayer->id_family != f_id)
	{
		pPlayer->id_family = f_id;
		PlayerTaskInterface tf(this);
		if(f_id == 0) 
		{
			SetFamilySkill(NULL, 0);
			OnTaskPlayerLeaveFamily(&tf);
		}
		else
		{
			OnTaskPlayerJoinFamily(&tf);
		}
	}

	if(m_id)
	{
		_mduel.CheckAndSetBattle(m_id);
		pPlayer->SetObjectState(gactive_object::STATE_MAFIA);
	}
	else
	{
		pPlayer->ClrObjectState(gactive_object::STATE_MAFIA);
	}

	if(f_id)
	{
		pPlayer->SetObjectState(gactive_object::STATE_FAMILY);
	}
	else
	{
		pPlayer->ClrObjectState(gactive_object::STATE_FAMILY);
	}

	if(pPlayer->pPiece)
	{
		_runner->mafia_info_notify();
	}

	OnUpdateMafiaInfo();
}

bool gplayer_imp::TestSanctuaryNormal()
{
	if(GetWorldManager()->GetCityRegion()->IsInSanctuary(_parent->pos.x, _parent->pos.z))
	{
		_filters.AddFilter(new pvp_limit_filter(this,FILTER_INDEX_PVPLIMIT));
		return true;
	}
	return false;
}


bool gplayer_imp::TestRegion()
{
	int reborn_cnt = 0;
	int max_level = 0;
	int min_level = 0;
	int kick_city = 0;
	A3DVECTOR  kick_pos;

	if(GetWorldManager()->GetCityRegion()->GetRegionLimit(_parent->pos.x, _parent->pos.z, reborn_cnt, max_level, min_level, kick_city, kick_pos))
	{
		if(max_level < 0 || min_level < 0 || kick_city <= 0 || reborn_cnt < 0) return false;
		if(max_level == 0 && min_level == 0) return false;

		int reborn_level =  reborn_cnt * 150; 
		//max_level += reborn_level;
		//min_level += reborn_level;
		int player_level = GetObjectLevel() + GetRebornCount() * 150; 

		if((max_level > 0 && player_level > max_level + reborn_level) || (min_level > 0 && player_level < min_level + reborn_level)) 
		{
			GLog::log(GLOG_INFO,"用户%d处在限制区域被转移(%f,%f), limit_reborn_cnt=%d, max_level=%d, min_level=%d, kick_city=%d, kick_pos_x=%f, kick_pos_y=%f, kick_pos_z=%f",
					_parent->ID.id, _parent->pos.x,_parent->pos.z, reborn_cnt, max_level, min_level, kick_city, kick_pos.x, kick_pos.y, kick_pos.z);
			LongJump(kick_pos, kick_city);
		}
	}
	return false;
}

bool 
gplayer_imp::TestSanctuary()
{
//      if(_filters.IsFilterExist(FILTER_INDEX_PVPLIMIT)) return true;
	if(!TestSanctuaryNormal())
	{
		GLog::log(GLOG_INFO,"用户%d未进入安全区(%f,%f)",_parent->ID.id, _parent->pos.x,_parent->pos.z);
	}
	return false;
}

void 
gplayer_imp::PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos )
{
	_switch_dest = -1;

	ReInit();

	SpiritSessionMap::iterator it = _spirit_session_list.begin();
	for(; it != _spirit_session_list.end(); ++ it)
	{
		act_session* se = it->second;
		se->Restore(this, -1);
	}
	
	((gplayer_controller*)_commander)->ReInit();
	CalcEquipmentInfo();

	if(GetWorldManager()->GetWorldLimit().nomount)
	{       
		//试图去除骑乘效果
		if(_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER))
		{
			_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
		}
	}

	if(GetWorldManager()->GetWorldLimit().nofly)
	{       
		//试图去除飞行效果
		_filters.RemoveFilter(FILTER_INDEX_AIRCRAFT);
	}

	_filters.RemoveFilter(FILTER_INDEX_BATTLEFLAG);
	GMSV::SendQueryBattleFlagBuff(GetParent()->ID.id, gmatrix::GetServerIndex(), GetWorldManager()->GetWorldTag(), GetParent()->id_mafia);
	
	TestSanctuary();
	_ph_control.Initialize(this);
	_petman.TrySummonLastSummonPet(this);

	if(GetWorldManager()->GetWorldLimit().faction_team) _team.CliLeaveParty();

	// 更新活跃度
	/*liuyue-facbase
	if (GetWorldManager()->IsRaidWorld()) {
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TRANSFER_MAP, GetWorldManager()->GetRaidID());
	} else {
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TRANSFER_MAP, GetWorldManager()->GetWorldTag());
	}
	*/
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TRANSFER_MAP, GetClientTag());
}

void 
gplayer_imp::PlayerLeaveMap()
{
	_petman.RecordCurSummonPet(this);
	_petman.ClearAllPetBehavior(this);
	_summonman.OnLeaveMap(this);
	_mobactiveman.PlayerMobActiveCancel(this);
}

void 
gplayer_imp::PlayerLeaveWorld()
{
	PlayerTaskInterface tif(this);
	tif.PlayerLeaveWorld();
	_petman.ClearAllPetBehavior(this);
	_summonman.OnLeaveWorld(this);
	_mobactiveman.PlayerMobActiveCancel(this);
}

int 
gplayer_imp::CheckPlayerMove(const A3DVECTOR & offset,int mode,int use_time)
{
	if(use_time  < MIN_MOVE_USE_TIME || use_time > MAX_MOVE_USE_TIME) 
	{
		return -1;
	}
//	if(!CheckMove(use_time,mode)) return -1;
	//考虑一下地形限制
	int lmode = _last_move_mode;
	_last_move_mode = mode;
	float t = use_time*0.001f;
	float speed_square = 1000.f;
	float cur_speed = (GetSpeedByMode(mode) + 0.2f)*t;	//加有少量的允许值
	float max_speed = ((MAX_FLIGHT_SPEED+0.2f)*(MAX_FLIGHT_SPEED+0.3f))*(t*t);

	if(!(mode & C2S::MOVE_MASK_DEAD))
	{
		FindCheater2();
	}
	
	mode = mode & 0x0F;
	if(mode == C2S::MOVE_MODE_FALL || mode ==  C2S::MOVE_MODE_FLY_FALL 
			|| mode == C2S::MOVE_MODE_JUMP || mode == C2S::MOVE_MODE_SLIDE)
	{
		//允许y方向上有较高速度
		if(offset.y > (MAX_JUMP_SPEED + 0.3f)*t) 
		{
			return -1;
		}
		else
			speed_square = offset.x *offset.x + offset.z*offset.z;
	}
	else
	{
		speed_square = offset.x *offset.x + offset.z*offset.z;
		float my = fabs(offset.y);
		int xmode = lmode & 0x0F;
		if(my <= cur_speed)
		{
			//什么也不做
		}
		else
		if(offset.y > (MAX_JUMP_SPEED + 0.3f)*t) 
		{
			return -1;
		}
		else if(xmode == C2S::MOVE_MODE_FALL 
				|| xmode ==  C2S::MOVE_MODE_FLY_FALL
				|| xmode ==  C2S::MOVE_MODE_JUMP
				|| mode == C2S::MOVE_MODE_DODGE
				|| xmode == C2S::MOVE_MODE_DODGE)
		{
			//什么也不做
		}
		else if(offset.y < -(MAX_JUMP_SPEED*2.0f + 0.3f)*t)
		{
			return -1;
		}
		else
		{	
			if(speed_square > max_speed) return -1;
			//肯定大于cur_speed
			return 3;
		}
	}
	if(isnan(speed_square) || isnan(offset.y)) return -1;

	if(speed_square > max_speed)
	{
		//超过了速度最大极限，不应该，直接打入冷宫
		return -1;
	}

	float cur_speed_square = cur_speed * cur_speed;
	float stmp = 1.f/(t*t);
	if(speed_square > cur_speed_square && speed_square > _en_point.over_speed_square )
	{
		__PRINTF("CBTL用户%d超过最大速度，speed_square:%f, cur_speed_square:%f, over_speed_square:%f\n", _parent->ID.id, speed_square, cur_speed_square, _en_point.over_speed_square);
		//如果当前速度超过了最大速度，那么和历史中的最大速度进行对比
		//首先将速度转换成为
		if(speed_square * stmp > _speed_ctrl_factor)
		{
			__PRINTF("CBTL用户%d超过了最大速度, speed_square:%f , _speed_ctrl_factor:%f , mode:%d time:%d cur_speed:%f sp%f\n",
					_parent->ID.id,speed_square,_speed_ctrl_factor,mode,use_time,
					cur_speed ,GetSpeedByMode(mode)
					);
			__PRINTF("CBTL用户%d位置(%f,%f,%f) 偏移(%f,%f,%f)\n",
					_parent->ID.id,_parent->pos.x,_parent->pos.y,_parent->pos.z,
					offset.x,offset.y,offset.z);

			//如果超过了最大速度，那么直接拉回来
			return -1;
		}
		return 	2;
	}
	else
	{
		//更新一下最大的可用速度
		_speed_ctrl_factor = cur_speed_square * stmp;
	}
	return 0;
}

bool 
gplayer_imp::PlayerDestroyItem(int where, size_t index, int item_type)
{
	item_list & list = GetInventory(where);
	if(index >= list.Size()) return false;
	if(list[index].type == -1 || list[index].type != item_type) return false;
	item it;
	list.Remove(index,it);
	_runner->player_drop_item(where,index,item_type,it.count,S2C::DROP_TYPE_DESTROY);
	UseItemLog(it);
	it.Release();
	return true;
}

const A3DVECTOR & 
gplayer_imp::GetLogoutPos(int &world_tag)
{
	if(_logout_pos_flag)
	{ 
		world_tag = _logout_tag;
		return _logout_pos;
	}
	else
	{ 
		world_tag = _plane->GetTag();
		return _parent->pos;
	}
}

void 
gplayer_imp::SetCoolDownData(const void * buf, size_t size)
{
	if(size == 0) return;
	raw_wrapper ar(buf,size);
	_cooldown.Load(ar);
}

void 
gplayer_imp::GetCoolDownData(archive & ar)
{
	_cooldown.Save(ar);
}

void 
gplayer_imp::GetCoolDownDataForClient(archive & ar)
{
	_cooldown.SaveForClient(ar);
}

void 
gplayer_imp::EnableFreePVP(bool bVal)
{
	//这里不给客户端发送数据
	_free_pvp_mode = bVal;
}

void 
gplayer_imp::SwitchFashionMode()
{
	gplayer * pPlayer = GetParent();
	bool is_fashion;
	if((is_fashion = (pPlayer->object_state & gactive_object::STATE_FASHION_MODE)))
	{
		pPlayer->object_state &= ~gactive_object::STATE_FASHION_MODE;
	}
	else
	{
		pPlayer->object_state |= gactive_object::STATE_FASHION_MODE;
	}
	_runner->player_enable_fashion_mode(!is_fashion);
}

void gplayer_imp::SetFashionMask(int fashion_mask)
{
	gplayer * pPlayer = GetParent();
	bool is_fashion= (pPlayer->object_state & gactive_object::STATE_FASHION_MODE);
	pPlayer->fashion_mask = fashion_mask;
	_runner->player_enable_fashion_mode(is_fashion);
}


void 
gplayer_imp::ObjReturnToTown()
{
	ReturnToTown();
}

void 
gplayer_imp::AddEffectData(short effect)
{
	gplayer * pPlayer = GetParent();
	size_t count = pPlayer->effect_count;
	if(count >= MAX_PLAYER_EFFECT_COUNT)  return;
	for(size_t i = 0; i< count; i ++)
	{
		if(pPlayer->effect_list[i] == effect) return ;
	}
	pPlayer->effect_list[count] =effect;
	pPlayer->effect_count ++;
	if(_parent->pPiece)
	{
		_runner->player_enable_effect(effect);
	}
}

void 
gplayer_imp::RemoveEffectData(short effect)
{
	gplayer * pPlayer = GetParent();
	size_t count = pPlayer->effect_count;
	for(size_t i = 0; i< count; i ++)
	{
		if(pPlayer->effect_list[i] == effect) 
		{
			if( i < count -1)
			{
				pPlayer->effect_list[i] = pPlayer->effect_list[count - 1];
			}
			pPlayer->effect_count --; 
			if(_parent->pPiece)
			{
				_runner->player_disable_effect(effect);
			}
			return ;
		}
	}
}

void 
gplayer_imp::GetPlayerCharMode(archive & ar)
{
	char data;
	gplayer * pPlayer = GetParent();
	data = (pPlayer->object_state & gactive_object::STATE_FASHION_MODE)?1:0;
	if(data)
	{
		ar << (int) PLAYER_CHAR_MODE_FASHION  << (int)1;
	}

	ar << (int)PLAYER_CHAR_MODE_FASHION_MASK << (int)GetParent()->fashion_mask;
	ar << (int)PLAYER_CHAR_MODE_OFFLINE_AGENT_ACTIVE << (int) _offline_agent_active;
	ar << (int)PLAYER_CHAR_MODE_OFFLINE_AGENT_TIME << (int) _offline_agent_time;

/*	 现在不保存骑乘状态了
	if(pPlayer->IsMountMode())
	{
		ar << (int) PLAYER_CHAR_MODE_MOUNT <<  (int)pPlayer->mount_id;
	}
	*/
	
}


void 
gplayer_imp::SetPlayerCharMode(const void * buf, size_t size)
{
	if(size < sizeof(int)*2) return ; //do nothing 
	int * data = (int *)buf;
	gplayer * pPlayer = GetParent();
	size_t count = size / sizeof(int);
	for(size_t i= 0; i < count - 1 ; i += 2)
	{
		switch (data[i])
		{
			case PLAYER_CHAR_MODE_FASHION:
				if(data[i+1]) pPlayer->object_state |=  gactive_object::STATE_FASHION_MODE;
			break;

			case PLAYER_CHAR_MODE_FASHION_MASK:
				pPlayer->fashion_mask = data[i+1];
			break;

			case PLAYER_CHAR_MODE_OFFLINE_AGENT_ACTIVE:
				_offline_agent_active = data[i+1];
			break;

			case PLAYER_CHAR_MODE_OFFLINE_AGENT_TIME:
				_offline_agent_time = data[i+1];
			break;

		}
	}
}

void 
gplayer_imp::PlayerRegionTransport(int rindex, int tag)
{
	//区域传送由于跟位置有关,所以必须是要排队的任务
	AddStartSession(new session_region_transport(this, rindex, tag));
}

bool
gplayer_imp::RegionTransport(int rindex, int tag)
{
	if(!_pstate.CanSwitch())
	{
		//非正常状态不允许切换
		return false;
	}

	//检查传送是否符合要求
	A3DVECTOR target_pos; 
	int target_tag;
	int target_line;
	if(!GetWorldManager()->GetCityRegion()->GetRegionTransport(_parent->pos,tag,rindex,target_pos,target_tag, target_line))
	{
		return false;
	}
	if(GetWorldManager()->GetCityRegion()->GetLevelLimit(rindex) > GetObjectLevel())
	{
		return false;
	}
	int ctrl_id = GetWorldManager()->GetCityRegion()->GetControllerID(rindex);
	ctrl_id = GetWorldManager()->TranslateCtrlID(ctrl_id);

	//副本是的控制器激活状态是分别保存，用另外一个接口查询
	if(GetWorldManager()->SaveCtrlSpecial())
	{
		if(ctrl_id > 0 && !gmatrix::IsActiveCtrl(GetWorldManager()->GetWorldTag(), ctrl_id))
		{
			return true;	//返回true为了不显示错误信息
		}
	}
	else if(ctrl_id > 0 && !gmatrix::IsActiveCtrl(ctrl_id))
	{
		return true;	//返回true为了不显示错误信息
	}

	if(target_tag < 0)
	{
		//目标tag小于0 ， 则代表应是小退下线
		if(_offline_type == 0)
		{
			GLog::log(GLOG_INFO,"用户%d通过副本盒子登出",_parent->ID.id);
			//这里调用玩家的登出操作  先用LostConnection 代替
			_team.PlayerLogout();
			Logout(1);
		}
		return true;
	}

	if(target_line > 0 && target_line != gmatrix::GetServerIndex())
	{
		class change_gs_task : public ONET::Thread::Runnable
		{
			int _sid;
			int _userid;
			int _target_line;
			int _tag;
			A3DVECTOR _pos;
			public:
			change_gs_task(int sid, int userid, int target_line, int tag, const A3DVECTOR & pos):_sid(sid),_userid(userid),_target_line(target_line),_tag(tag),_pos(pos) 
			{}
			virtual void Run()
			{
				player_change_gs(_sid,_userid,_target_line,_tag,_pos.x,_pos.y,_pos.z,0);
				delete this;
			}
		};
		//走换线逻辑
		ONET::Thread::Pool::AddTask( new change_gs_task( GetParent()->cs_sid, GetParent()->ID.id, target_line, target_tag, target_pos));
		return true;
	}

	//考虑加入region transport的冷却
	
	//手动拉高5公分以保证不会掉入建筑中
	target_pos.y += 0.05f;
	//ClearNextSession();

	//让Player进行副本传送 
	//liuyue-facbase 副本和基地不能跨地图?
	if(GetWorldManager()->IsRaidWorld() || GetWorldManager()->IsFacBase())
	{
//		if(GetWorldManager()->GetRaidID() == target_tag)
		if (GetClientTag() == target_tag)
		{
			return LongJump(target_pos,GetWorldTag());
		}
	}
	else
	{
		return LongJump(target_pos,target_tag);
	}
	return false;
}

void 
gplayer_imp::EnterResurrectReadyState(float exp_reduce,float hp_recover)
{
	//进入可以复活的状态 通知客户端那，设置自己的标志 和经验损失 这些数据还要存盘
	if(!_parent->IsZombie()) return;

	_resurrect_state = true;
	if(exp_reduce < 0.f) exp_reduce =0.f;
	if(exp_reduce >1.f)  exp_reduce = 1.f;

	if(hp_recover < 0.f) hp_recover =0.f;
	if(hp_recover >1.f)  hp_recover = 1.f;
	if(_resurrect_exp_reduce > exp_reduce) _resurrect_exp_reduce = exp_reduce;
	if(_resurrect_hp_recover < hp_recover) _resurrect_hp_recover = hp_recover;

	if(_parent->pPiece) _runner->enable_resurrect_state(_resurrect_exp_reduce);
}

bool 
gplayer_imp::CheckCoolDown(int idx) 
{ 
	return _cooldown.TestCoolDown(idx);
}

void 
gplayer_imp::SetCoolDown(int idx, int msec) 
{ 
	if(msec <= 0) return;
	_cooldown.SetCoolDown(idx,msec);
	_runner->set_cooldown(idx, msec);
}

bool 
gplayer_imp::CheckAndSetCoolDown(int idx, int msec)
{
	if(!_cooldown.TestCoolDown(idx)) return false;
	_cooldown.SetCoolDown(idx,msec);
	_runner->set_cooldown(idx, msec);
	return true;
}

void
gplayer_imp::ReduceSkillCooldown(int idx, int msec)
{
	raw_wrapper cd_data; 
	int rst = _cooldown.ReduceCoolDown(SKILL_COOLINGID_BEGIN+idx, msec, cd_data);

	if(rst == 0)
	{
		_runner->set_cooldown(SKILL_COOLINGID_BEGIN+idx, 0);
	}
	else if(rst > 0)
	{
		((gplayer_dispatcher*)_runner)->update_cooldown_data(cd_data);
	}
}

void 
gplayer_imp::ClrCoolDown(int idx)
{
	if(_cooldown.ClrCoolDown(idx))
	{
		_runner->set_cooldown(idx, 0);
	}
}

void
gplayer_imp::ClrAllCoolDown()
{
	abase::vector<int> list;
	_cooldown.GetCoolDownList(list);

	for(size_t i = 0; i < list.size(); ++i)
	{
		ClrCoolDown(list[i]);
	}
}

void 
gplayer_imp::RebuildAllInstanceData(int create_time)
{
	//空数据，要从头开始
	const abase::vector<int> & list = gmatrix::GetInstanceTags();
	_cur_ins_key_list.reserve(list.size());
	_cur_tag_counter.reserve(list.size());
	_ins_key_timer.reserve(list.size());
	for(size_t i = 0; i < list.size(); i ++)
	{
		_cur_tag_counter.push_back(abase::pair<int,int>(list[i],create_time));
		_ins_key_timer.push_back(abase::pair<int,int>(0,0));
	}
	//_ins_key_timer是 timer/标记对 它和_cur_tag_counter的数据一一对应
	//其中timer代表此tag的数据上次进入副本的时间 标记则代表是否进入过该副本
	//已经进入过副本的话，则可以随意进入，否则必须满足timer后时间加5分钟的条件
	RebuildInstanceKey();
	//复制出队伍的副本key，否则转移服务器会引发失败
	_team_ins_key_list = _cur_ins_key_list;
}

#define CUR_INSTANCE_KEY_DATA_VERSION  (int)0
void 
gplayer_imp::InitInstanceKey(int create_time, const void * buf , size_t size)
{
	if(size < sizeof(int)*2)
	{
		RebuildAllInstanceData(create_time);
		return ;
	}

	int tag_counter = gmatrix::GetInstanceTags().size();
	int * pData = (int*)buf;
	if(pData[0] != CUR_INSTANCE_KEY_DATA_VERSION || pData[1] != tag_counter)
	{
		RebuildAllInstanceData(create_time);
		return ;
	}
	if(size != sizeof(int)*2 + pData[1] *(sizeof(int) *2 + sizeof(int)*3 + sizeof(int)*2))
	{
		ASSERT(false && "数据大小不正确");
		RebuildAllInstanceData(create_time);
		return ;
	}

	//存盘数据格式如下:
	/*
	   struct
	   {
	   int version;
	   size_t count;

	   _cur_tag_counter 数据 
	   _ins_key_timer 数据
	   _cur_ins_key_list 数据
	   };
	 */
	size_t count = pData[1];
	pData += 2;
	_cur_tag_counter.reserve(count);
	_ins_key_timer.reserve(count);
	_cur_ins_key_list.reserve(count);

	_cur_tag_counter.clear();
	_ins_key_timer.clear();
	_cur_ins_key_list.clear();
	const abase::vector<int> & list = gmatrix::GetInstanceTags();
	for(size_t i = 0;i < count ; i ++)
	{
		int tag = *pData++;
		int counter = *pData++;
		_cur_tag_counter.push_back(abase::pair<int,int>(tag,counter));
		int j = 0;
		for(j = 0; j < tag_counter; j ++)
		{
			if(list[j] == tag) break;
		}
		if(j == tag_counter)
		{
			//副本Tag发生了变化，彻底重新生成副本ID
			__PRINTF("副本TAG表有变化，重新生成副本ID列表\n");
			_cur_tag_counter.clear();
			RebuildAllInstanceData(create_time);
			return ;
		}
	}
	for(size_t i = 0;i < count ; i ++)
	{
		int time = *pData++;
		int state = *pData++;
		_ins_key_timer.push_back(abase::pair<int,int>(time,state));
	}
	for(size_t i = 0;i < count ; i ++)
	{
		int tag = *pData++;
		int key1= *pData++;
		int key2= *pData++;
		_cur_ins_key_list[tag] = abase::pair<int,int>(key1,key2);
	}

	//复制出队伍的副本key，否则转移服务器会引发失败
	_team_ins_key_list = _cur_ins_key_list;
}

void 
gplayer_imp::SaveInstanceKey(archive & ar)
{
	size_t count = _cur_tag_counter.size();

	ar << CUR_INSTANCE_KEY_DATA_VERSION;
	ar << count;
	ar.push_back(_cur_tag_counter.begin(), count * (sizeof(int)*2) );
	ar.push_back(_ins_key_timer.begin(), count * (sizeof(int)*2) );

	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
	for(it = _cur_ins_key_list.begin(); it != _cur_ins_key_list.end(); ++it)
	{
		ar << it->first << it->second.first << it->second.second;
	}
}

void
gplayer_imp::RebuildInstanceKey()
{
	_cur_ins_key_list.clear();
	for(size_t i = 0; i < _cur_tag_counter.size(); i ++)
	{
		abase::pair<int,int> & p = _cur_tag_counter[i];
		abase::pair<int,int> value = abase::pair<int,int>(_parent->ID.id, p.second++);
		_cur_ins_key_list[p.first] = value;
		_ins_key_timer[i].second = 0;
	}
}

bool 
gplayer_imp::GetInstanceKeyBuf(char * buf, size_t size)
{
	if(size != _team_ins_key_list.size() * (sizeof(int)*3)) return false;
	int * pBuf = (int*)buf;
	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
	for(it = _team_ins_key_list.begin(); it != _team_ins_key_list.end(); ++it)
	{
		*pBuf++ = it->first;
		*pBuf++ = it->second.first;
		*pBuf++ = it->second.second;
	}
	return true;
}

bool 
gplayer_imp::SetInstanceKeyBuf(const void * buf, size_t size)
{
	if(size != _cur_tag_counter.size() * sizeof(int)*3)
	{
		return false;
	}

	_team_ins_key_list.clear();
	int * pBuf = (int*)buf;
	size_t offset = 0;
	for(size_t i = 0; i < size; i += sizeof(int)*3,offset ++)
	{
		int tag = *pBuf++;
		int key1= *pBuf++;
		int key2= *pBuf++;
		_team_ins_key_list[tag] = abase::pair<int,int>(key1,key2);

		//重置所有副本的进入标志
		_ins_key_timer[offset].second = 0;
	}
	return true;
}

int
gplayer_imp::CheckInstanceTimer(int tag)
{
	int t = g_timer.get_systime();
	for(size_t i = 0; i < _cur_tag_counter.size(); i ++)
	{
		if(_cur_tag_counter[i].first == tag)
		{
			if(_ins_key_timer[i].second == 1) return 1;
			if(t - _ins_key_timer[i].first > 3)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

void
gplayer_imp::SetInstanceTimer(int tag)
{
	int t = g_timer.get_systime();
	for(size_t i = 0; i < _cur_tag_counter.size(); i ++)
	{
		if(_cur_tag_counter[i].first == tag)
		{
			if(_ins_key_timer[i].second == 0) 
			{
				_ins_key_timer[i].first = t;
			}
			_ins_key_timer[i].second = 1;
			return ;
		}
	}
}

void 
gplayer_imp::OnHeal(const XID & healer, int life)
{
	//进行仇恨的增加
	AddNPCAggro(healer,(int)(life * (0.5f/PVP_DAMAGE_REDUCE)));
}

void
gplayer_imp::OnHeal(int life, bool is_potion)
{
	if(!is_potion) return;
	if(!OI_IsCloneExist()) return;
	XID clone_id = GetCloneID();
	if(clone_id.type == -1 || clone_id.id == -1) return;

	SendTo<0>(GM_MSG_HEAL_CLONE, clone_id, life);
}

void
gplayer_imp::OnInjectMana(int mana, bool is_potion)
{
	if(!is_potion) return;
	if(!OI_IsCloneExist()) return;
	XID clone_id = GetCloneID();
	if(clone_id.type == -1 || clone_id.id == -1) return;

	SendTo<0>(GM_MSG_MANA_CLONE, clone_id, mana);
}



int 
gplayer_imp::GetLinkIndex()
{
	return GetParent()->cs_index;
}

int 
gplayer_imp::GetLinkSID()
{
	return GetParent()->cs_sid;
}

size_t 
gplayer_imp::OI_GetInventorySize()
{
	return _inventory.Size();
}

int 
gplayer_imp::OI_GetInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _inventory.GetDBData(list,size);
}

size_t gplayer_imp::OI_GetPetBedgeInventorySize()
{
	return _pet_bedge_inventory.Size();
}

int gplayer_imp::OI_GetPetBedgeInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _pet_bedge_inventory.GetDBData(list,size);
}
size_t 
gplayer_imp::OI_GetPetEquipInventorySize()
{
	return _pet_equip_inventory.Size();
}

int 
gplayer_imp::OI_GetPetEquipInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _pet_equip_inventory.GetDBData(list,size);
}

int 
gplayer_imp::OI_GetEquipmentDetail(GDB::itemdata * list, size_t size) 
{ 
	return _equipment.GetDBData(list,size);
}

size_t 
gplayer_imp::OI_GetEquipmentSize() 
{ 
	return _equipment.Size();
}

size_t
gplayer_imp::OI_GetPocketInventorySize()
{
	return _pocket_inventory.Size();
}

	
int 
gplayer_imp::OI_GetPocketInventoryDetail(GDB::pocket_item * list, size_t size)
{
	return _pocket_inventory.GetDBData(list, size);
}

size_t
gplayer_imp::OI_GetFashionInventorySize()
{
	return _fashion_inventory.Size();
}
	
int 
gplayer_imp::OI_GetFashionInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _fashion_inventory.GetDBData(list, size);
}

size_t
gplayer_imp::OI_GetMountWingInventorySize()
{
	return _mount_wing_inventory.Size();
}

int 
gplayer_imp::OI_GetMountWingInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _mount_wing_inventory.GetDBData(list, size);
}

size_t
gplayer_imp::OI_GetGiftInventorySize()
{
	return _gift_inventory.Size();
}

int 
gplayer_imp::OI_GetGiftInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _gift_inventory.GetDBData(list, size);
}

size_t
gplayer_imp::OI_GetFuwenInventorySize()
{
	return _fuwen_inventory.Size();
}

int 
gplayer_imp::OI_GetFuwenInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _fuwen_inventory.GetDBData(list, size);
}

int 
gplayer_imp::TradeLockPlayer(int get_mask, int put_mask)
{
	//这类锁定默认tradeid为0 
	return StartFactionTrade(0,get_mask,put_mask,true)?0:-1;
}

int 
gplayer_imp::TradeUnLockPlayer()
{
	if(!_pstate.IsWaitFactionTradeState()) return -1;
	FromFactionTradeToNormal();
	return 0;
}

int     
gplayer_imp::TradeCashLockPlayer(int get_mask, int put_mask)
{       
	return StartTradeCash(0, get_mask, put_mask) ? 0 : -1;
}

int
gplayer_imp::TradeCashUnLockPlayer()
{
	if(!_pstate.IsWaitFactionTradeState()) return -1;
	FromFactionTradeToNormal();
	return 0;
}

void 
gplayer_imp::GainMoneyWithDrop(size_t inc,bool write_log)
{
	size_t newmoney = _player_money + inc;
	ASSERT((int)_player_money >= 0);
	if((int)newmoney < (int)_player_money || newmoney > _money_capacity)
	{
		//这里溢出了
		newmoney = (size_t)-1;
		size_t delta = _money_capacity - _player_money;
		size_t drop = inc - delta;
		_player_money = _money_capacity; 
		DropMoneyItem(_plane,_parent->pos,drop,_parent->ID,0,0,_parent->ID.id);
		//这里无论如何都记录日志， 因为有掉落出现
		GLog::log(GLOG_INFO,"用户%d得到金钱%d,其中掉落%d",_parent->ID.id,inc,drop);

	}
	else
	{
		//这里如果外面设置了不记录日志，那么就不记录了
		if(write_log) GLog::log(GLOG_INFO,"用户%d得到金钱%d",_parent->ID.id,inc);
		_player_money = newmoney;
	}
}
	
void 
gplayer_imp::GainMoney(size_t inc)
{
	size_t newmoney = _player_money + inc;
	ASSERT((int)_player_money >= 0);
	if((int)newmoney < (int)_player_money || newmoney > _money_capacity)
	{
		//这里溢出不加钱
		return ;
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d得到金钱%d",_parent->ID.id,inc);
		_player_money = newmoney;
	}
}

void 
gplayer_imp::SpendMoney(size_t delta,bool write_log)
{
	ASSERT(delta <= _player_money);
	_player_money -= delta;
	if(write_log) GLog::log(GLOG_INFO,"用户%d花掉金钱%d",_parent->ID.id,delta);
}

bool 
gplayer_imp::CheckIncMoney(size_t inc)
{
	size_t newmoney = _player_money + inc;
	if((int)newmoney < (int)_player_money || newmoney > _money_capacity)
	{
		//溢出
		return false ;
	}
	else
	{
		return true;
	}
}

bool 
gplayer_imp::CheckDecMoney(size_t inc)
{
	return _player_money >= inc;
}

void gplayer_imp::GainBattleScore( size_t inc )
{
	size_t newbattlescore = _battle_score + inc;
	ASSERT( (int)_battle_score >= 0 );
	if( (int)newbattlescore < (int)_battle_score )
	{
		// 这里溢出不加积分
		return;
	}
	else
	{
		GLog::log( GLOG_INFO, "用户%d得到战场积分%d", _parent->ID.id, inc );
		_battle_score = newbattlescore;
	}
}

void gplayer_imp::SpendBattleScore( size_t dec, bool write_log )
{       
	ASSERT( dec <= _battle_score );
	_battle_score -= dec;
	if( write_log ) GLog::log( GLOG_INFO, "用户%d花掉战场积分%d", _parent->ID.id, dec );
}

bool gplayer_imp::CheckIncBattleScore( size_t inc )
{
	size_t newbattlescore = _battle_score + inc;
	if( (int)newbattlescore < (int)_battle_score )
	{
		// 溢出
		return false;
	}
	else
	{
		return true;
	}
}

bool gplayer_imp::CheckDecBattleScore( size_t dec )
{
	return _battle_score >= dec;
}


void
gplayer_imp::LoadMultiExp(archive & ar)
{
	if(ar.size() == 0) return;
	ASSERT(ar.size() % (2 * sizeof(int)) == 0);
	int count = ar.size() / (2 * sizeof(int));

	int i;
	int timestamp;
	while(count--)
	{
		ar >> i;
		ar >> timestamp;
		_item_multi_exp.multi_exp_timestamp[i] = timestamp;
	}
	UpdateMultiExp(false);
}	      

void
gplayer_imp::SaveMultiExp(archive & ar)
{
	for(size_t i = 2; i < 10; ++i)
	{
		if(_item_multi_exp.multi_exp_timestamp[i] > 0)
		{
			ar << i;
			ar << _item_multi_exp.multi_exp_timestamp[i];
		}
	}
}

void
gplayer_imp::ActiveMultiExpTime(int multi_exp, int time)
{
	ASSERT(multi_exp > 0 && multi_exp < 10);

	int t = g_timer.get_systime();
	//更换当前的多倍经验系数
	if(multi_exp > _item_multi_exp.cur_multi_exp)
	{
		if(_item_multi_exp.multi_exp_timestamp[multi_exp] == 0)
		{
			_item_multi_exp.multi_exp_timestamp[multi_exp] = t + time;
		}
		else
		{
			_item_multi_exp.multi_exp_timestamp[multi_exp] += time;
		}

		_item_multi_exp.cur_multi_exp = multi_exp;
		_item_multi_exp.cur_multi_exp_timestamp = _item_multi_exp.multi_exp_timestamp[multi_exp]; 

		SendMultiExp();
	}
	//更新当前的多倍经验时间
	else if(multi_exp ==  _item_multi_exp.cur_multi_exp)
	{
		_item_multi_exp.cur_multi_exp_timestamp += time; 
		_item_multi_exp.multi_exp_timestamp[_item_multi_exp.cur_multi_exp] += time;

		SendMultiExp();
	}
	//暂时不启用
	else if(multi_exp < _item_multi_exp.cur_multi_exp)
	{
		if(_item_multi_exp.multi_exp_timestamp[multi_exp] == 0)
		{
			_item_multi_exp.multi_exp_timestamp[multi_exp] = t + time;
		}
		else
		{
			_item_multi_exp.multi_exp_timestamp[multi_exp] += time;
		}
	}
	GLog::log(GLOG_INFO,"用户%d增加多倍时间, multi_exp=%d, time=%d", _parent->ID.id, multi_exp, time);
}

void
gplayer_imp::UpdateMultiExp(bool need_notify)
{
	int t = g_timer.get_systime();
	int max_multi_exp = 0;

	for(int i = 2; i < 10; ++i)
	{
		//多倍时间到期了
		if(_item_multi_exp.multi_exp_timestamp[i] > 0 && _item_multi_exp.multi_exp_timestamp[i] <= t)
		{
			_item_multi_exp.multi_exp_timestamp[i] = 0;
			continue;
		}
		
		if(_item_multi_exp.multi_exp_timestamp[i] > 0 && i > max_multi_exp)
		{
			max_multi_exp = i;
		}
	}

	if(max_multi_exp == 0)
	{
		_item_multi_exp.cur_multi_exp = 0;
		_item_multi_exp.cur_multi_exp_timestamp = 0;
	}
	else
	{
		_item_multi_exp.cur_multi_exp = max_multi_exp;
		_item_multi_exp.cur_multi_exp_timestamp =  _item_multi_exp.multi_exp_timestamp[max_multi_exp];
	}

	if(need_notify) SendMultiExp();
}

void 
gplayer_imp::ClearMultiExp()
{
	_item_multi_exp.cur_multi_exp = 0;
	_item_multi_exp.cur_multi_exp_timestamp = 0;
	memset(_item_multi_exp.multi_exp_timestamp, 0, sizeof(_item_multi_exp.multi_exp_timestamp));
	SendMultiExp();
}

int 
gplayer_imp::GetMultiExp()
{
	return _item_multi_exp.cur_multi_exp;
}

void 
gplayer_imp::SendMultiExp()
{
	_runner->multi_exp_mode(_item_multi_exp.cur_multi_exp, _item_multi_exp.cur_multi_exp_timestamp); 
}

int gplayer_imp::GetMultiExpTime(int multi_exp)
{
	if(_item_multi_exp.multi_exp_timestamp[multi_exp] == 0) return 0;

	return _item_multi_exp.multi_exp_timestamp[multi_exp] - g_timer.get_systime();
}


bool 
gplayer_imp::ItemToMoney(size_t inv_index, int type, size_t count)
{
	if(inv_index >= _inventory.Size()) return false;
	const item & it = _inventory[inv_index];
	if(it.type == -1 || it.type != type || it.count < count) return false;
	if(!it.CanSell()) return false;

	if(GetMoney() >= _money_capacity) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	float incmoney = (float)it.price * count;
	if(incmoney < 0) incmoney = 0;
	if(incmoney >1e11) incmoney = 0;
	size_t imoney = (size_t)(incmoney+0.5f);

	if(!CheckIncMoney(imoney))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//放入回购包裹
	AddToRepurchaseInv(_inventory[inv_index], count);

	GLog::log(GLOG_INFO,"用户%d卖店%d个%d",_parent->ID.id,count,type);
	//减去物品和增加钱数
	UseItemLog(_inventory[inv_index], count);
	_inventory.DecAmount(inv_index,count);
	size_t nmoney = GetMoney();
	GainMoney(imoney);
	if(imoney) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=1:reason=1:hint=0",_parent->ID.id,_db_magic_number,(int)imoney);


	((gplayer_dispatcher*)_runner)->item_to_money(inv_index, type, count, GetMoney() - nmoney);
	return true;
}

float
gplayer_imp::CalcIncMoney(size_t inv_index, int type, size_t count)
{
	float incmoney = 0.0f;

	if(inv_index >= _inventory.Size()) return incmoney;
	const item & it = _inventory[inv_index];
	if(it.type == -1 || it.type != type || it.count < count) return incmoney;
	if(!it.CanSell()) return incmoney;

	incmoney = (float)it.price * count;
	return incmoney;
}

void 
gplayer_imp::PlayerDuelRequest(const XID & target)
{
	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return;
	}
	_duel.PlayerDuelRequest(this,target);
}

void 
gplayer_imp::PlayerDuelReply(const XID & target,int param)
{
	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return;
	}
	_duel.PlayerDuelReply(this,target,param);
}

void 
gplayer_imp::Die(const XID & attacker, bool is_pariah, char attacker_mode)
{
	//检测是否决斗带来的死亡
	if(attacker_mode & attack_msg::PVP_DUEL)
	{
		//决斗带来的死亡，进行特殊处理
		_basic.hp = 1;
		_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

		//调用可能的决斗函数
		_duel.OnDeath(this, true);
		return ;
	}

	//使用傀儡特技保命
	gplayer* pPlayer = GetParent();
	if(pPlayer->IsPuppetState())
	{
		if(_puppet_skill_id > 0 && _puppet_immune_prob > 0 && _filters.IsFilterExist(FILTER_PUPPETSKILL))
		{
			if(abase::Rand(0, 100) < _puppet_immune_prob)
			{
				_basic.hp = 1;

				enchant_msg enchant;
				memset(&enchant,0,sizeof(enchant));
				enchant.skill        = _puppet_skill_id;
				enchant.skill_level  = 1; 
				enchant.helpful = true;

				FillEnchantMsg(_parent->ID,enchant);
				TranslateEnchant(_parent->ID, enchant);
				SendEnchantMsg(GM_MSG_ENCHANT, _parent->ID, enchant);

				_filters.RemoveFilter(FILTER_PUPPETFORM);
				_filters.RemoveFilter(FILTER_PUPPETSKILL);
				return;
			}

			_filters.RemoveFilter(FILTER_PUPPETSKILL);
		}
		_filters.RemoveFilter(FILTER_PUPPETFORM);
	}
		
	_duel.OnDeath(this, false);
	//filter调用死亡前的处理
	_filters.EF_BeforeDeath(attacker_mode & attack_msg::PVP_DUEL);

	bool no_drop = false;
	if(attacker_mode & attack_msg::PVP_MAFIA_DUEL)
	{
		//帮派死亡，发送自己死亡的消息
		gplayer * pPlayer = GetParent();
		if(pPlayer->id_mafia > 0)
		{
			//...
			//GMSV::SendMafiaBattleResult(pPlayer->ID.id, pPlayer->id_mafia, attacker.id);
		}
		no_drop = true;
	}

	//去除死亡时应该去掉的filter
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH);

//	这些判定不一定能够立刻生效 这是由于上面的删除可能会延迟进行
//	ASSERT(!_silent_seal_flag);
//	ASSERT(!_diet_seal_flag);
//	ASSERT(!_melee_seal_flag);
//	ASSERT(!_root_seal_flag);
	//要求死亡后这些状态标志都应该被自动清除

	//死亡，调用死亡函数，进行处理
	OnDeath(attacker, is_pariah, no_drop, attacker_mode & attack_msg::PVP_HOSTILE_DUEL );

	_filters.EF_AfterDeath(attacker_mode & attack_msg::PVP_DUEL);
}

/*
        飞剑         变身          骑乘          绑定(相依相偎 搓澡 多人骑乘 麒麟)
人马     可以        可以          可以                       不行 
麒麟     不行        可以          可以                       麒麟
人马人   可以        可以          可以                       相依相偎 搓澡 多人骑乘   
*/

bool 
gplayer_imp::CheckPlayerBindRequest()
{
	//检查是否能够发起请求
	//条件是,只能是normal状态
	//没有当前操作
	if(!_pstate.IsNormalState()) return false;
	if(_cur_session || HasNextSession()) return false;
	if(GetShape() && GetShape() != 8) return false;
	if(IsTransformState()) return false;
	gplayer * pPlayer = GetParent();
	if(pPlayer->IsInvisible()) return false;
	//人马默认不能绑定
	if(IsRenMa() && GetShape() == 0) return false;

	return true;
}

bool 
gplayer_imp::CheckPlayerBindInvite()
{
	//检查是否能够发起邀请
	//只能是normal或者bind状态
	if(!_pstate.IsNormalState() && !_pstate.IsBindState()) return false;
	if(_cur_session || HasNextSession()) return false;
	if(GetShape() != 0 && GetShape() != 7 && GetShape() != 8) return false;
	if(IsTransformState()) return false;
	gplayer * pPlayer = GetParent();
	if(pPlayer->IsInvisible()) return false;
	//人马默认不能绑定
	if(IsRenMa() && GetShape() == 0) return false;

	return true;
	
}

bool
gplayer_imp::EnterBindMoveState(const XID & target, const char type)
{
	if(!_pstate.IsNormalState()) return false;

	_pstate.Bind();

	gplayer *pParent = GetParent();
	pParent->bind_type = type;
	pParent->bind_target = target.id;
	pParent->object_state |= gactive_object::STATE_IN_BIND;

	return true;
}

bool
gplayer_imp::EnterBindFollowState(const XID & leader, const char type, const char pos)
{
	if(!_pstate.IsNormalState()) return false;
	//进入载人移动状态
	//这个状态不改变player_state
	_commander->DenyCmd(controller::CMD_MOVE);
	_commander->DenyCmd(controller::CMD_PET);
	_pstate.Bind();

	//去除骑马filter和标志
	_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
	_filters.RemoveFilter(FILTER_INDEX_AIRCRAFT);

	//去除所有宠物状态
	_petman.ClearAllPetBehavior(this);
	
	//禁止飞行
	_bind_to_ground ++;

	//清除当前session
	ClearSession();

	gplayer *pParent = GetParent();
	pParent->bind_type = type;
	pParent->bind_target = leader.id;
	pParent->bind_pos = pos;
	pParent->object_state |= gactive_object::STATE_IN_BIND;
	return true;
}

void 
gplayer_imp::ReturnBindNormalState(char link_type)
{
	gplayer *pParent = GetParent();
 	if(!_pstate.IsBindState())
 	{
 		GLog::log(GLOG_ERR,"玩家已经处于正常状态, roleid%d, link_type=%d", pParent->ID.id, link_type);
 		return;
 	}	
 	if((pParent->bind_type == 1 || pParent->bind_type == 2) && link_type != 1) 
 	{
 		GLog::log(GLOG_ERR,"玩家在相依相偎中，不能从多人骑乘返回正常状态, roleid=%d, bind_type=%d, link_type=%d\n", pParent->ID.id, pParent->bind_type, link_type);
 		return;
 	}
 	if((pParent->bind_type == 3 || pParent->bind_type == 4) && link_type != 2) 
 	{
 		GLog::log(GLOG_ERR,"玩家在多人骑乘中，不能从相依相偎返回正常状态, roleid=%d, bind_type=%d, link_type=%d\n", pParent->ID.id, pParent->bind_type, link_type);
 		return;
 	}
 
	_pstate.Normalize();
	if(pParent->bind_type == 2 || pParent->bind_type == 4 || pParent->bind_type == 8) 
	{
		if(!pParent->bind_disconnect)
		{
			_commander->AllowCmd(controller::CMD_MOVE);
			_commander->AllowCmd(controller::CMD_PET);
			_bind_to_ground --;

			//如果是乘客
			//这种情况下需要同步一下坐标位置
			int seq = _commander->GetCurMoveSeq();
			seq = (seq + 100) & 0xFFFF;
			_commander->SetNextMoveSeq(seq);
			_runner->trace_cur_pos(seq);
		}
	}
	else if(pParent->bind_type == LINK_BATH_MEMBER_TYPE || pParent->bind_type == LINK_BATH_LEADER_TYPE)
	{
		_commander->AllowCmd(controller::CMD_MOVE);
		_commander->AllowCmd(controller::CMD_PET);
		_bind_to_ground --;
	}

	pParent->bind_type = 0;
	pParent->bind_target = 0;
	pParent->object_state &= ~gactive_object::STATE_IN_BIND;
}

void 
gplayer_imp::PlayerBindRequest(const XID & target)
{
	_bind_player.PlayerLinkRequest(this,target);
}

void 
gplayer_imp::PlayerBindInvite(const XID & target)
{
	_bind_player.PlayerLinkInvite(this,target);
}

void 
gplayer_imp::PlayerBindRequestReply(const XID & target, int param)
{
	_bind_player.PlayerLinkReqReply(this,target,param);
}

void 
gplayer_imp::PlayerBindInviteReply(const XID & target, int param)
{
	_bind_player.PlayerLinkInvReply(this,target,param);
}

void 
gplayer_imp::PlayerBindCancel()
{
	if(!_pstate.IsBindState())
	{
		//这应该是不可能的
		ASSERT(false);
		return;
	}
	_bind_player.PlayerLinkCancel(this);
}


void 
gplayer_imp::PlayerRideInvite(const XID & target)
{
	_link_ride_player.PlayerLinkInvite(this,target);
}


void 
gplayer_imp::PlayerRideInviteReply(const XID & target, int param)
{
	_link_ride_player.PlayerLinkInvReply(this,target,param);
}

void 
gplayer_imp::PlayerRideCancel()
{
	if(!_pstate.IsBindState())
	{
		//这应该是不可能的
		return;
	}
	_link_ride_player.PlayerLinkCancel(this);
}


void 
gplayer_imp::PlayerRideKick(char pos)
{
	if(!_pstate.IsBindState())
	{
		//这应该是不可能的
		return;
	}
	_link_ride_player.PlayerLinkKick(this, pos);
}

void 
gplayer_imp::ActiveMountState(int mount_id, int mount_lvl, bool no_notify, char mount_type)
{
	gplayer * pPlayer = GetParent();
	bool is_mount = pPlayer->IsMountMode();
	if(is_mount) return;
	//加入命令限制
	_commander->DenyCmd(controller::CMD_MARKET);
	
	//禁止飞行
	_bind_to_ground ++;

	//进行状态切换 
	pPlayer->mount_level = mount_lvl & 0xFF;
	pPlayer->mount_id  = mount_id;
	pPlayer->mount_type = mount_type; 
	pPlayer->object_state |= gactive_object::STATE_MOUNT;

	//骑上多人骑乘的时候解除相依相偎状态
	if(pPlayer->mount_type == 1 && _pstate.IsBindState()){
		if(pPlayer->bind_type == 1 || pPlayer->bind_type == 2){	
			_bind_player.PlayerLinkCancel(this);
		}
	}

	//发送命令
	if(!no_notify) _runner->player_mounting(mount_id, mount_lvl, mount_type);
	
	//去除隐身
	if(pPlayer->IsInvisible()) _filters.RemoveFilter(FILTER_INDEX_INVISIBLE);
}

void 
gplayer_imp::DeactiveMountState()
{
	gplayer * pPlayer = GetParent();
	bool is_mount = pPlayer->IsMountMode();
	if(!is_mount) return;

	//多人骑乘状态
	if(_pstate.IsBindState() && pPlayer->bind_type == 3){
		_link_ride_player.PlayerLinkCancel(this);	
	}
	_bind_to_ground --;
	_commander->AllowCmd(controller::CMD_MARKET);

	//进行状态切换 
	pPlayer->object_state &= ~gactive_object::STATE_MOUNT;
	
	//发送命令
	_runner->player_mounting(0,0);
}

void
gplayer_imp::OnChangeShape(int shape)
{
	gplayer * pPlayer = GetParent();
	//麒麟状态
	if(_pstate.IsBindState() && pPlayer->bind_type == 7){
		_link_qilin_player.PlayerLinkCancel(this);	
	}
}

bool 
gplayer_imp::OI_IsSpouse(const XID & target) 
{ 
	return GetParent()->spouse_id && (target.id == GetParent()->spouse_id); 
}

void 
gplayer_imp::OI_JumpToSpouse() 
{ 
	if(!IsMarried())
	{
		return;
	}

	if(!GetWorldManager() || GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
		return;
	}

	int sp = GetSpouse();
	world::object_info info;
	if(!_plane->QueryObject(XID(GM_TYPE_PLAYER,sp),info,true))
	{
		_runner->error_message(S2C::ERR_CAN_NOT_FIND_SPOUSE);
		return;
	}

	world_manager * pManager = gmatrix::FindWorld(info.tag);
	if(!pManager || pManager->IsIdle() || pManager->GetWorldLimit().nocouplejump)
	{
		_runner->error_message(S2C::ERR_CAN_TRANSMIT_IN);
		return;
	}

	LongJump(info.pos, info.tag);
}

bool 
gplayer_imp::OI_IsMafiaMember()
{
	return GetParent()->id_mafia;
}

int
gplayer_imp::OI_GetMafiaID()
{
	return GetParent()->id_mafia;
}

int
gplayer_imp::OI_GetFamilyID()
{
	return GetParent()->id_family;
}

int
gplayer_imp::OI_GetZoneID()
{
	return GetParent()->id_zone;
}


int
gplayer_imp::OI_GetMasterID()
{
	return (GetSectID() == this->_parent->ID.id) ? 0 : GetSectID(); 
}

bool 
gplayer_imp::CheckGMPrivilege()
{
	return ((gplayer_controller*)_commander)->HasGMPrivilege();
}

int 
gplayer_imp::CheckItemPrice(int inv_index, int item_id)
{
	if(IsItemExist(inv_index,item_id,1))
	{
		return _inventory[inv_index].price;
	}
	else
	{
		return 0;
	}
}

size_t 
gplayer_imp::OI_GetTrashBoxCapacity()
{
	return _trashbox.GetTrashBoxSize();
}

size_t 
gplayer_imp::OI_GetMafiaTrashBoxCapacity()
{
	return _trashbox.GetMafiaTrashBoxSize();
}


int 
gplayer_imp::OI_GetTrashBoxDetail(GDB::itemdata * list, size_t size)
{
	return _trashbox.Backpack().GetDBData(list,size);
}

int 
gplayer_imp::OI_GetMafiaTrashBoxDetail(GDB::itemdata * list, size_t size)
{
	return _trashbox.MafiaBackpack().GetDBData(list,size);
}

bool 
gplayer_imp::OI_IsTrashBoxModified()
{
	return IsTrashBoxChanged();
}

bool 
gplayer_imp::OI_IsEquipmentModified()
{
	return _eq_change_counter;
}

size_t
gplayer_imp::OI_GetTrashBoxMoney()
{
	return _trashbox.GetMoney();
}

bool gplayer_imp::EnterBattleground( int target_battle_tag, int battle_faction, int battle_type )
{
	if( _plane->GetWorldManager()->IsBattleWorld() || _plane->GetWorldManager()->IsRaidWorld() || _plane->GetWorldManager()->IsBattleFieldWorld() || _plane->GetWorldManager()->IsFacBase())
	{
		__PRINTF( "只准从普通map进入战场map\n" );
		return false;
	}
	if( !_pstate.CanSwitch() )
	{
		__PRINTF( "玩家:%d 处于!CanSwitch状态\n", GetParent()->ID.id );
		return false;
	}
	
	// 这里不需要其他检测,战场状态,人数等delivery会判断,直接让玩家进入
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey( target_battle_tag, key );
	key.target = key.essence;
	//key.target.key_level4 = battle_id;
	key.target.key_level4 = 0;
	A3DVECTOR pos(0,0,0);//这个pos不重要进入战场后会调整
	// 让Player进行战场传送
	// instance_key不使用新加了_battle_faction表示
	_battle_faction = battle_faction;
	if( GetWorldManager()->PlaneSwitch( this, pos, target_battle_tag, key, 0 ) < 0 )
	{
		return false;
	}
	return true;
}

int 
gplayer_imp::OI_GetDBTimeStamp()
{
	return _db_timestamp;
}

int 
gplayer_imp::OI_InceaseDBTimeStamp()
{
	_db_timestamp ++;
	return _db_timestamp;
}

void 
gplayer_imp::LeaveAbnormalState()
{
	if(_pstate.IsTradeState())
	{
		if(_trade_obj)
		{
			DiscardTrade();
		}
		else if(_consign_obj)
		{
			_pstate.WaitTradeComplete();
			_consign_obj->SetTimeOut(10);
		}
	}
	else if(_pstate.IsMarketState())
	{
		CancelPersonalMarket();
	}
	else if(_pstate.IsBindState())
	{
		LeaveBindState();
	}
}

bool
gplayer_imp::TaskLeaveAbnormalState()
{
	if(_pstate.IsNormalState()) return true;
	LeaveAbnormalState();
	if(_pstate.IsNormalState()) return true;
	return false;
}

void gplayer_imp::SendTeamChat(char channel, const void * buf, size_t len, const void * aux_data, size_t dsize, int use_id)
{
	if(IsInTeam())
	{
		_team.TeamChat(channel,_chat_emote,buf,len, use_id > 0 ?use_id: _parent->ID.id, aux_data , dsize);
	}
}

void gplayer_imp::SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data , size_t dsize )
{
	world *pPlane = _plane;
	if(pPlane == NULL) return ;

	//限制未飞升30级以下小号的发言长度和频率
	if(GetObjectLevel() < 30 && channel == GMSV::CHAT_CHANNEL_LOCAL && GetRebornCount() == 0)
	{
		//冷却6秒
		if(!CheckCoolDown(COOLDOWN_INDEX_NEWBIE_CHAT))
		{
			_runner->error_message(S2C::ERR_NEWBIE_CHAT_IS_COOLING);
			return;
		}
		SetCoolDown(COOLDOWN_INDEX_NEWBIE_CHAT, 6000);
	}
	
	gplayer * pPlayer = GetParent();
	slice * pPiece = pPlayer->pPiece;
	int level = GetRebornCount() > 0 ? 0 : GetObjectLevel();
	if(CheckGMPrivilege())
	{
		level = 150;
	}
	RegionChatMsg(pPlane,pPiece,buf,len,channel,_chat_emote,aux_data, dsize, pPlayer->ID.id, level, pPlayer->id_mafia, pPlayer->sect_id);
}

void
gplayer_imp::SendFarCryChat(char channel, const void * msg, size_t size, const void * data, size_t dsize)
{
	if(!_pstate.CanSave()) return;

	if(!CheckCoolDown(COOLDOWN_INDEX_FARCRY))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}

	int item_idx = _inventory.Find(0, g_config.item_speaker_id2);
	int item_id = g_config.item_speaker_id2;
	if(item_idx < 0 )
	{
		item_idx = _inventory.Find(0, g_config.item_speaker_id);
		item_id = g_config.item_speaker_id;
		if(item_idx < 0)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return ;
		}
	}

	//检查是否需要记录消费值
	CheckSpecialConsumption(_inventory[item_idx].type, 1);

	UseItemLog(_inventory[item_idx],1);
	_inventory.DecAmount(item_idx,1);
	_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_id,1);

	
	//1秒冷却
	SetCoolDown(COOLDOWN_INDEX_FARCRY, 200);

	broadcast_chat_msg(_parent->ID.id,msg,size,channel,_chat_emote,data, dsize);
}

void
gplayer_imp::SendGlobalChat(char channel, const void * msg, size_t size, const void * data, size_t dsize, int item_idx)
{
	if(!_pstate.CanSave()) return;
	if(!CheckCoolDown(COOLDOWN_INDEX_FARCRY))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	if(item_idx < 0 || (size_t)item_idx >= _inventory.Size()) return;

	int item_id = _inventory[item_idx].type;
	if(item_id == -1) return;
	bool found = false;

	for(size_t i = 0; i < 3; ++i)
	{
		if(g_config.item_speaker2_id[i] == item_id)
		{
			found = true;
			break;
		}
	}
	if(!found) return;

	//检查是否需要记录消费值
	CheckSpecialConsumption(_inventory[item_idx].type, 1);

	UseItemLog(_inventory[item_idx],1);
	_inventory.DecAmount(item_idx,1);
	_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_id,1);

	//1秒冷却
	SetCoolDown(COOLDOWN_INDEX_FARCRY, 200);

	broadcast_chat_msg(_parent->ID.id,msg,size,channel,_chat_emote,data, dsize);
}

void
gplayer_imp::SendRumorChat(char channel, const void * msg, size_t size, const void * data, size_t dsize, int item_idx)
{
	if(!_pstate.CanSave()) return;
	if(!CheckCoolDown(COOLDOWN_INDEX_FARCRY))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	if(item_idx < 0 || (size_t)item_idx >= _inventory.Size()) return;

	int item_id = _inventory[item_idx].type;
	if(item_id == -1) return;
	bool found = false;

	for(size_t i = 0; i < 3; ++i)
	{
		if(g_config.item_speaker3_id[i] == item_id)
		{
			found = true;
			break;
		}
	}
	if(!found) return;

	UseItemLog(_inventory[item_idx],1);
	_inventory.DecAmount(item_idx,1);
	_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_id,1);

	//1秒冷却
	SetCoolDown(COOLDOWN_INDEX_FARCRY, 200);

	broadcast_chat_msg(_parent->ID.id,msg,size,channel,_chat_emote,data, dsize);
}


void
gplayer_imp::SendZoneChat(char channel, const void * msg, size_t size, const void * data, size_t dsize)
{
	if(!_pstate.CanSave()) return;
	broadcast_chat_msg(_parent->ID.id,msg,size,channel,_chat_emote,data, dsize);
}

void
gplayer_imp::SetChatEmote(int emote_id)
{
	_chat_emote = emote_id & 0xFF;
	GMSV::SetChatEmote(GetParent()->ID.id, _chat_emote);
}

int gplayer_imp::CheckUseTurretScroll()
{
	//只能战场里使用
	return S2C::ERR_ITEM_NEED_BATTLEGROUND;
}

	
gactive_imp::attack_judge
gplayer_imp::GetHookAttackJudge()
{
	return gplayer_imp::__GetHookAttackJudge;
}

gactive_imp::enchant_judge 
gplayer_imp::GetHookEnchantJudge()
{
	return gplayer_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill 
gplayer_imp::GetHookAttackFill()
{
	return gplayer_imp::__GetHookAttackFill;
}

gactive_imp::enchant_fill 
gplayer_imp::GetHookEnchantFill()
{
	return gplayer_imp::__GetHookEnchantFill;
}

template <typename MESSAGE,typename EFFECT_MSG>
inline static  bool HookTestHarmfulEffect(gactive_imp * __this, const MESSAGE & msg, EFFECT_MSG & emsg)
{
	// 自己的宠物被打了
	// 宠物的攻击判定
	int enemy_mafia_id = __this->OI_GetMDuelMafiaID();
	if( emsg.ainfo.mafia_id && enemy_mafia_id)
	{
		int self_mafia_id = __this->OI_GetMafiaID();
		if(emsg.attacker_mode & attack_msg::PVP_SANCTUARY) return false;
		if(self_mafia_id == emsg.ainfo.mafia_id) return false;
		if(enemy_mafia_id == emsg.ainfo.mafia_id) emsg.attacker_mode |=  attack_msg::PVP_MAFIA_DUEL;
	}       

	bool IsInvader = false;
	if(emsg.attacker_mode & (attack_msg::PVP_DUEL|attack_msg::PVP_MAFIA_DUEL))
	{               
		//决斗状态一律有效
		emsg.target_faction = 0xFFFFFFFF;
		emsg.is_invader = false;
		return true;
	}               
	XID attacker = emsg.ainfo.attacker;
	if(attacker.IsPlayerClass())
	{       
		//非强制攻击不受伤害
		if(!emsg.force_attack) return false;
		//如果对方未开PK开关也不会被攻击
		if(!(emsg.attacker_mode & attack_msg::PVP_ENABLE))
		{       
			return false;
		}
		//如果处于PK保护状态则不被攻击
		if(!__this->OI_IsPVPEnable()) return false;
		//不受队友伤害
		if(__this->OI_IsInTeam() && __this->OI_IsMember(attacker)) return false;
		//如果开启了帮派保护，并且攻击者和被攻击者的帮派相同，则不会被攻击到
		if(!(emsg.force_attack & C2S::FORCE_ATTACK_MAFIA))
		{
			int idmafia = emsg.ainfo.mafia_id;
			if(idmafia && idmafia == __this->OI_GetMafiaID()) return false;
		}
		if(!(emsg.force_attack & C2S::FORCE_ATTACK_FAMILY))
		{
			int idfamily = emsg.ainfo.family_id;
			if(idfamily && idfamily == __this->OI_GetFamilyID()) return false;
		}
		if( __this->OI_GetPkLevel() > 0 || __this->OI_GetInvaderCounter() > 0 )
		{
			//如果自己是红名或者粉名且对方不攻击红名则返回
			if(!(emsg.force_attack & C2S::FORCE_ATTACK_RED)) return false;
		}
		else if( __this->OI_GetPkLevel() == 0 )
		{
			//如果自己是白名且对方不攻击白名则返回
			if(!(emsg.force_attack & C2S::FORCE_ATTACK_WHITE)) return false;
		}
		IsInvader = true;
	}
	emsg.is_invader = IsInvader;
	return true;
}

bool 
gplayer_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//进行宠物的攻击判定，和玩家基本一致
	//如果是决斗 则在外面判断 这里不进行处理 
	return HookTestHarmfulEffect(__this, msg, amsg);
}

bool 
gplayer_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	XID attacker = emsg.ainfo.attacker;
	if(emsg.helpful)
	{
		int enemy_mafia_id = __this->OI_GetMDuelMafiaID();
		if( emsg.ainfo.mafia_id && enemy_mafia_id)
		{
			if(enemy_mafia_id == emsg.ainfo.mafia_id) return false;
			if(!(emsg.attacker_mode & attack_msg::PVP_SANCTUARY))
			{
				if( __this->OI_GetMafiaID() == emsg.ainfo.mafia_id) return true;
			}
		}

		//友好技能
		XID attacker = emsg.ainfo.attacker;
		bool pvpcombat = __this->OI_IsInPVPCombatState();
		if(attacker.IsPlayerClass())
		{
			if(!(emsg.attacker_mode & attack_msg::PVP_ENABLE) && pvpcombat)
			{
				return false;
			}
		}
		emsg.is_invader = pvpcombat && ( ((gplayer_imp*)__this)->OI_GetPkLevel() > 0 )
			&& (emsg.attacker_mode & attack_msg::PVP_ENABLE);

		//如果对方并非PVP状态，并且自己处于PVP状态，则发一个消息让对方成为PVP状态
		if(!(emsg.attacker_mode & attack_msg::PVP_DURATION) && pvpcombat)
		{
			__this->SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,attacker,0);
		}
	}
	else
	{
		//有害技能 相同于普通攻击
		bool bRst = HookTestHarmfulEffect(__this, msg, emsg);
		if( !bRst ) return false;
		//如果对方并非PVP状态，则回传消息让玩家处于PVP
		if(!(emsg.attacker_mode & attack_msg::PVP_DURATION) &&
				(emsg.is_invader || (emsg.attacker_mode & attack_msg::PVP_MAFIA_DUEL)))
		{
			__this->SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,emsg.ainfo.attacker,0);
		}
	}
	return true;
}

template < typename EFFECT_MSG>
inline static void FillHookAttackMsg(gactive_imp * __this, EFFECT_MSG &attack)
{
	bool is_pvpenable = __this->OI_IsPVPEnable();
	if(!is_pvpenable) attack.force_attack = 0;
	//不管设置不设置不伤帮派成员都设置帮派ID
	attack.ainfo.mafia_id = __this->OI_GetMafiaID();
	attack.ainfo.family_id = __this->OI_GetFamilyID();
	attack.ainfo.master_id = __this->OI_GetMasterID();

	attack.attacker_mode = is_pvpenable?attack_msg::PVP_ENABLE:0;
}

void 
gplayer_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	FillHookAttackMsg(__this, attack);
}

void 
gplayer_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	FillHookAttackMsg(__this, enchant);
}

void 
gplayer_imp::SetLeaderData(leader_prop & data)
{
	data.pvp_flag_enable = OI_IsPVPEnable();
	data.pvp_combat_timer = GetPVPCombatTimer();
	data.is_flying = InFlying(); 
	data.mafia_id = OI_GetMafiaID();
	data.zone_id = OI_GetZoneID();
	data.family_id = OI_GetFamilyID();
	data.team_count = GetTeamMemberNum();
	data.level = _basic.level;
	data.team_efflevel = _team.GetEffLevel();
	data.wallow_level = _team.GetWallowLevel();
	if(data.team_efflevel <=0) data.team_efflevel = _basic.level;
	GetMemberList(data.teamlist);
	data.team_id = 0;
	data.team_seq = 0;
	_team.GetTeamID(data.team_id, data.team_seq);
	data.cs_index = GetCSIndex();
	data.cs_sid = GetCSSid();
	data.duel_target = _duel_target;
	data.pk_level = _pk_level;
	data.invader_counter = _invader_counter;
	data.free_pvp_mode = _free_pvp_mode;
	if( _mduel.InBattle() )
	{       
		data.mduel_mafia_id = _mduel.GetEnemy();
	}       
	else    
	{
		data.mduel_mafia_id = 0;
	}
	data.sanctuary_mode = _sanctuary_mode;
	data.safe_lock = OI_TestSafeLock();
	data.pet_attack_adjust = _en_point.pet_attack_adjust;
	data.cultivation = _cultivation;
	data.pvp_mask = _pvp_mask;
	data.master_id = GetSectID();
}

void gplayer_imp::NotifyStartAttack(const XID& target,char force_attack)
{
	_petman.NotifyStartAttack(this,target,force_attack);
	_summonman.NotifyStartAttack(this,target,force_attack);

	_at_feedback_state = 0;

}

void 
gplayer_imp::NotifyMasterInfo()
{
	leader_prop data;
	SetLeaderData(data);

	_petman.NotifyMasterInfo(this, data);
	_summonman.NotifyMasterInfo(this, data);
}

void 
gplayer_imp::OnDuelStart(const XID & target)
{
	gactive_imp::OnDuelStart(target);
	_duel_target = target.id;
	NotifyMasterInfo();
}

void 
gplayer_imp::OnDuelStop()
{
	gactive_imp::OnDuelStop();
	_duel_target = 0;

	_petman.NotifyDuelStop(this);
	_summonman.NotifyDuelStop(this);

	NotifyMasterInfo();
}


//处理遗留的双倍经验存盘数据
void 
gplayer_imp::SetRestParam(const void * buf, size_t size)
{
	enum {OFFSET = 5};
	if(size < sizeof(int) + sizeof(int)*OFFSET || !buf)
	{	
		//大小不正确，不处理
		return;
	}

	const int * content = (const int*)buf;
	int double_exp_timeout	= content[0];
	if(double_exp_timeout > 0) 
	{
		int t = g_timer.get_systime();
		if(double_exp_timeout > t)
		{
			ActiveMultiExpTime(2, double_exp_timeout-t);
		}
	}
}

void 
gplayer_imp::SayHelloToNPC(const XID & target)
{
	SendTo<0>(GM_MSG_SERVICE_HELLO,target,GetFaction());
}

bool
gplayer_imp::RefineItemAddon(size_t index, int item_type, size_t stone_index, int rt_index) 
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size || stone_index >= inv_size) return false;
	if(rt_index >= 0 &&  (size_t)rt_index  >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;
	if(it.IsLocked()) return false;

	//检查石头是否存在
	item & it2 = _inventory[stone_index];
	if(it2.type == -1) return false;

	float adjust[4] = {0,0,0,0};
	bool mode_2 = false;
	int require_level = 0;
	float adjust2[20] = {0};
	//检查保留符和幸运符是否存在
	int rt_id = -1;

	//精炼成功后是否需要绑定
	bool need_bind = false;

	bool is_wanbi = false;  // add by sunjunbo 2012-11-19 完璧符判断
		
	if(rt_index >= 0) 
	{
		rt_id = _inventory[rt_index].type;
		if(rt_id <= 0) return false;
		DATA_TYPE dt2;
		const REFINE_TICKET_ESSENCE &ess= *(const REFINE_TICKET_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(rt_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_REFINE_TICKET_ESSENCE || &ess == NULL)
		{
			return false;
		}
	
		need_bind = ess.bind_equip;	
		float adj1 = ess.ext_reserved_prob;
		float adj2 = ess.ext_succeed_prob;
		is_wanbi = ess.ext_reserved_prob >= 1.2;
		if(adj1 < 0) adj1 = 0;
		if(adj2 < 0) adj2 = 0;
		if(adj1 > 1.0) adj1 = 1.0;
		if(adj2 > 1.0) adj2 = 1.0;
		if(adj1 != ess.ext_reserved_prob || adj2 != ess.ext_succeed_prob)
		{
			__PRINTF("强化时发生状态调整\n");
		}

		adjust[0] = adj2;
		adjust[1] = adj1;

		if(ess.new_mode)
		{
			mode_2 = true;
			require_level = ess.refine_level;
			memcpy(adjust2, ess.new_succeed_prob, sizeof(adjust2));
			ASSERT(sizeof(adjust2) == sizeof(ess.new_succeed_prob));
		}
	}

	int level_before = 0;
	int level_result = 0;
	int fee = 0;
	int rst = it.body->ReinforceEquipment(&it, this,it2.type, adjust, level_result,fee,level_before, mode_2, require_level, adjust2);

	if(rst != item::REINFORCE_UNAVAILABLE)
	{
		//记录日志
		GLog::formatlog("formatlog:reinforce:userid=%d:item=%d:level=%d:stone=%d:result=%d:luckyroll=%d:level_before=%d",_parent->ID.id, item_type, level_result,it2.type,rst,rt_id,level_before);
		GLog::action("fgitem,uid=%d:rid=%d:itemid=%d:lv_before=%d:lv=%d:res=%d:payflag=%d:stone=%d", _db_magic_number,_parent->ID.id, item_type,level_before,level_result,rst,rt_id>0?1:0,rt_id);
	}

	if(rst != item::REINFORCE_UNAVAILABLE && fee > 0)
	{
		ASSERT((size_t)fee <= GetMoney());
		//内部已经判断了金钱
		DecMoneyAmount(fee);
		if (fee) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=9:hint=%d",_parent->ID.id,_db_magic_number,fee,it.type);
	}

	if(need_bind)
	{
		it.Bind();	
	}

	switch(rst)
	{
		case item::REINFORCE_UNAVAILABLE:
			//无法进行精炼，发送精炼失败  这种情况不作任何变化 
			_runner->error_message(S2C::ERR_REINFORCE_FAILED);
			return true;
			break;
		case item::REINFORCE_SUCCESS:
			//精炼成功
			_runner->refine_result(index,item_type,0);
			PlayerGetItemInfo(IL_INVENTORY,index);
			break;

		case item::REINFORCE_FAILED_LEVEL_00:
			//精炼00失败，装备无变化，消耗材料本身
			PlayerGetItemInfo(IL_INVENTORY,index);
			_runner->refine_result(index,item_type, 4);
			break;

		case item::REINFORCE_FAILED_LEVEL_0:
			//精炼一级失败，删除材料 级别降低一级
			PlayerGetItemInfo(IL_INVENTORY,index);
			_runner->refine_result(index,item_type, 1);
			break;

		default:
			GLog::log(GLOG_ERR,"精炼装备时返回了异常错误%d",rst);
		case item::REINFORCE_FAILED_LEVEL_1:
			//精炼二级失败，删除材料，属性已经被自动清除
			_runner->refine_result(index,item_type,2);
			PlayerGetItemInfo(IL_INVENTORY,index);
			break;
		case item::REINFORCE_FAILED_LEVEL_2:
			//精炼三级失败，删除材料，删除物品
			_runner->refine_result(index,item_type,3);
			{
				item it;
				_inventory.Remove(index,it);
				_runner->player_drop_item(IL_INVENTORY,index,item_type,it.count,S2C::DROP_TYPE_USE);
				UseItemLog(it);
				it.Release();
			}
			break;
	}

	int stone_id = it2.type;
	if(rt_index >= 0) 
	{
		UseItemLog(_inventory[rt_index],1);
		_inventory.DecAmount(rt_index, 1);
		_runner->player_drop_item(IL_INVENTORY,rt_index,rt_id, 1 ,S2C::DROP_TYPE_USE);
		//检查是否需要记录消费值
		CheckSpecialConsumption(rt_id, 1);
	}
	UseItemLog(_inventory[stone_index],1);
	_inventory.DecAmount(stone_index, 1);
	_runner->player_drop_item(IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);

	if (is_wanbi) {
		// 活跃度更新, 完璧符
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_WANBI_FU);
	}
	return true;
}


//装备升级
bool
gplayer_imp::UpgradeEquipment(size_t index, int item_type, size_t stone_index, int rt_index) 
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size || stone_index >= inv_size) return false;
	if(rt_index >= 0 &&  (size_t)rt_index  >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) return false;
	if(it.IsLocked()) return false;
	if(it.expire_date > 0) return false;
	//检查装备是否能够升级
	if(!player_template::CanEquipUpgrade(item_type)) return false;

	//检查模具是否符合
	item & it2 = _inventory[stone_index];
	if(it2.type == -1 || it2.type == 0) return false;
	int stone_id = 0;
	int stone_num = 0;
	player_template::GetEquipUpgradeStoneInfo(item_type, stone_id, stone_num);
	if(stone_id < 0 || stone_num < 0) return false;
	if(stone_id != it2.type) return false;
	if(it2.count < (size_t)stone_num) return false;

	//检查产出的ID是否对
	int output_id = player_template::GetEquipUpgradeOutputID(item_type);
	if(output_id == 0) return false;
	DATA_TYPE dt;
	const EQUIPMENT_ESSENCE &ess = *(const EQUIPMENT_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(output_id, ID_SPACE_ESSENCE,dt); 
	if(dt != DT_EQUIPMENT_ESSENCE || &ess == NULL)
	{
		return false;
	}

	//检查保留符是否存在
	int rt_id = -1;
	bool is_special_rt = false;
	int special_level = 0;


	//有保留符
	if(rt_index >= 0) 
	{
		rt_id = _inventory[rt_index].type;
		if(rt_id <= 0) return false;
		DATA_TYPE dt2;
		const UPGRADE_EQUIP_STONE_ESSENCE &ess2 = *(const UPGRADE_EQUIP_STONE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(rt_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_UPGRADE_EQUIP_STONE_ESSENCE || &ess2 == NULL)
		{
			return false;
		}

		is_special_rt = ess2.type;
		special_level = ess2.level;

		if(is_special_rt && (special_level < it.GetReinforceLevel() && special_level < 10) ) return false;
	}
	bool rst = it.body->UpgradeEquipment(&it, this, index, item_type, output_id, is_special_rt, special_level, stone_index, stone_id, stone_num, rt_index, rt_id);
		
	//检查是否需要记录消费值,不管成功还是失败都要消耗保留符
	if(rt_id > 0) CheckSpecialConsumption(rt_id, 1);
		
	GLog::formatlog("formatlog:upgrade_equipment, userid=%d:item=%d:output_id=%d:stone_id=%d:rt_id=%d:result=%d",_parent->ID.id, item_type, output_id, stone_id, rt_id, rst);
	GLog::log(GLOG_INFO,"用户%d升级封神装备%d到%d, 模具:%d, 保留符:%d, 结果:%d",_parent->ID.id,item_type, output_id, stone_id, rt_id, rst);
	
	return true;	
}

bool
gplayer_imp::ProduceItem(const recipe_template & rt, bool is_npc_produce)
{
	if(!is_npc_produce && !VerifyRecipe(rt.id))
	{
		//以后修改错误类型
		_runner->error_message(S2C::ERR_PRODUCE_FAILED);
		return false;
	}
	if(GetMoney() < rt.fee)
	{
		//报告一个中断消息
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
	
	int xindex = abase::RandSelect(&(rt.products[0].probability), sizeof(rt.products[0]),3);

	//判断生成何种物品
	int item_id = rt.products[xindex].target_id;
	int target_count_low = rt.products[xindex].target_count_low;
	int target_count_high = rt.products[xindex].target_count_high;

	//检查包裹是否足够
	if(!_inventory.GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[6];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));
	int num = rt.material_num;

	item_list & inv = _inventory;
	int nlist[inv.Size()];
	memset(nlist,0,sizeof(nlist));

	int total_count = rt.material_total_count;

	//检查物品列表是否满足条件
	for(size_t i = 0;i < inv.Size(); i++)
	{
		int type = inv[i].type;
		if(type == -1) continue;
		for(int j= 0; j < num; j++)
		{
			if(mlist[j].item_id == type)
			{
				size_t count = inv[i].count;
				if(count > mlist[j].count) count = mlist[j].count;
				nlist[i] = count;
				if(!(mlist[j].count -= count))
				{
					std::swap(mlist[j],mlist[num-1]);
					num --;
				}
				total_count -= count;
				break;
			}
		}
		if(total_count == 0) break;
	}	
	ASSERT(total_count >= 0);
	if(total_count > 0)
	{
		//原料不够
		//报告错误并中断
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false;
	}

	if(1 == rt.recipe_class)
	{
		if(!_petman.CheckProduceItem(this,rt)) return false;
	}

	//进行物品生成
	item_data * data = NULL;
	if(item_id)
	{
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		data = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
		if(!data)
		{
			//物品生成失败
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = abase::Rand(target_count_low, target_count_high);
		if(data->count <= 0) data->count = 1;
		if(data->count > data->pile_limit) data->count = data->pile_limit;
		GLog::log(GLOG_INFO,"用户%d制造了%d个%d 配方%d",_parent->ID.id,data->count, item_id,rt.id);
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d制造%d时未成功 配方%d",_parent->ID.id,item_id,rt.id);
	}

	if(data && !is_npc_produce)
	{
		if(1 == rt.recipe_class)
		{
			_petman.AddPetConstruction(this,rt.level * rt.level);
		}
		else
		{
			//增加一些经验
			int level = _produce_level;
			int exp = _produce_exp;
			player_template::ProduceItem(_produce_level,_produce_exp,rt.level);
			if( level != _produce_level || exp != _produce_exp)
			{
				GLog::log(GLOG_INFO,"用户%d的生产技能为%d级，当前经验为%d",_parent->ID.id,_produce_level, _produce_exp);
				_runner->player_produce_skill_info(_produce_level, _produce_exp);
			}
		}
	}

	//设置冷却，判断冷却的代码在service_provider里
	if(rt.cool_type > 0)
	{
		SetCoolDown(COOLDOWN_INDEX_PRODUCE_BEGIN + rt.cool_type, rt.cool_time);
	}

	//发送制造出物品的消息
	SpendMoney(rt.fee);
	_runner->spend_money(rt.fee);
	//从player身上取掉原料和手续费
	for(size_t i = 0; i < inv.Size(); i ++)
	{
		if(nlist[i]) 
		{
			_runner->player_drop_item(IL_INVENTORY,i,inv[i].type,nlist[i],S2C::DROP_TYPE_PRODUCE);
			UseItemLog(inv[i],nlist[i]);
			inv.DecAmount(i,nlist[i]);
		}
	}
	if(data)
	{
		int count = data->count;
		int rst = inv.Push(*data);
		if(rst >=0)
		{
			int state = item::Proctype2State(data->proc_type);
			//发出获得制造物品的消息
			_runner->produce_once(item_id,count - data->count,inv[rst].count, 0,rst,state);
		}

		if(data->count)
		{
			//剩余了物品
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0);
			//这种情况不需要释放内存
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			//报告制造中断
			return false;
		}
		FreeItem(data);
		_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_PRODUCE);
		object_interface oif(this);
		GetAchievementManager().OnProduce(oif, rt.id, item_id);
	}
	else
	{
		//发出制造未成功的消息
		_runner->produce_null(rt.id);
	}
	
	//生成额外获得
	script_ExtGainOfProduce(rt.id, _basic.level, IsPlayerFemale(), GetPlayerClass());
	return true;
}

bool
gplayer_imp::RemoveRefineAddon(size_t index, int item_type, bool remove_pstone)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;

	size_t money = GetMoney();
	int rst = it.body->RemoveReinforce(&it, this, money,remove_pstone);
	switch(rst)
	{
		case item::REMOVE_REINFORCE_SUCCESS:
		if (money) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=9:hint=%d",_parent->ID.id,_db_magic_number,money,it.type);
		SpendMoney(money);
		_runner->spend_money(money);
		PlayerGetItemInfo(IL_INVENTORY,index);
		_runner->refine_result(index,item_type,remove_pstone?5:6);

		//做日志
		GLog::formatlog("formatlog:remove_reinforce:userid=%d:item=%d:remove_skill_stone=%d:cost=%d",_parent->ID.id, item_type, remove_pstone?0:1, money);
		break;

		case item::REMOVE_REINFORCE_OUT_OF_FUND:
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		break;
		case item::REMOVE_REINFORCE_FAILD:
		return false;
	}
	return true;
}

bool
gplayer_imp::PlayerProduceItem(int recipe_id)
{
	const recipe_template *rt = recipe_manager::GetRecipe(recipe_id);
	if(!rt) return false;

	//校验配方技能和级别是否匹配
	if(rt->level > GetProduceLevel()) return false;

	if(GetMoney() < rt->fee) 
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;	//只报告金钱不够的错误
	}

	if(1 == rt->recipe_class)
	{
		if(!_petman.CheckProduceItem(this,*rt)) return false;
	}
	if(rt->cool_type > 0 && rt->cool_type < 200)	//多留200余量
	{
		if(!CheckCoolDown(COOLDOWN_INDEX_PRODUCE_BEGIN + rt->cool_type ))
		{
			_runner->error_message(S2C::ERR_PRODUCE_IN_COOLDOWN);
			return false;
		}
	}

	AddStartSession(new session_produce(this,rt,1));
	return true;
}



void 
gplayer_imp::ModifyItemPoppet(int offset)
{
	_item_poppet_counter += offset;
	__PRINTF("物品替身娃娃的格数为%d\n",_item_poppet_counter);
}

void 
gplayer_imp::ModifyExpPoppet(int offset) 
{
	_exp_poppet_counter += offset;
	__PRINTF("经验替身娃娃的格数为%d\n",_exp_poppet_counter);
}

bool 
gplayer_imp::VerifyWhileSave()
{
	ASSERT(_inventory.CountItemByType(item_body::ITEM_TYPE_POPPET) == _item_poppet_counter + _exp_poppet_counter);
	return true;
}

void 
gplayer_imp::DecInventoryItem(int index, int count, int drop_type)
{
	item & it = _inventory[index];
	int id = it.type;
	if((size_t)count > it.count) count = it.count;
	UseItemLog(it, count);
	_inventory.DecAmount(index, count);
	_runner->player_drop_item(IL_INVENTORY,index,id, count ,drop_type);
}

void 
gplayer_imp::ResetPKValue(int point, size_t need_money)
{
	ASSERT(need_money <= GetMoney());
	int pkvalue = GetPKValue();
	if(pkvalue == 0) return;
	if(point > abs(pkvalue)) point = abs(pkvalue);
	if(pkvalue > 0) point = -point;
	ModifyPKValue(point);
	if(need_money > 0)
	{
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}
	_runner->pk_value_notify(GetPKValue());
}

/**
 * @brief PlayerGetMallItemPrice: 天帝宝库打开时查询哪些限时商品正在销售
 *
 * @return: true if success
 */
bool 
gplayer_imp::PlayerGetMallSaleTimeItem()
{
	//设置冷却
	if(!CheckCoolDown(COOLDOWM_INDEX_GET_MALL_PRICE)) return false;
	SetCoolDown(COOLDOWM_INDEX_GET_MALL_PRICE, GET_MALL_PRICE_COOLDOWN_TIME);

	using namespace S2C;
	qgame::mall & shop = item_manager::GetShoppingMall();

	//可能发生变化的商品列表    
	abase::vector<qgame::mall::index_node_t, abase::fast_alloc<> > & limitgoods_list = shop.GetLimitGoods();
	size_t limit_goods_count = limitgoods_list.size();

	time_t t = (time_t)g_timer.get_systime();
	packet_wrapper tmpgoodslist;

	//检查有哪些限时商品正在销售
	for(size_t i = 0; i < limit_goods_count; i++)
	{
		qgame::mall::node_t & node = limitgoods_list[i]._node;

		if(node.sale_time_active)
		{
			int remainsecs = 0;
			if(qgame::mall::sale_time::RET_SUCCESS == node._sale_time.GetRemainingTime(t, remainsecs))
			{
				CMD::Make<CMD::mall_item_price>::AddGoods(tmpgoodslist, limitgoods_list[i]._index, node.goods_id, remainsecs);
			}
		}
	}

	_runner->mall_item_price(tmpgoodslist);
	return true;
}

bool
gplayer_imp::PlayerDoShopping(int id, size_t order_index, size_t slot, size_t order_count)
{
	if(!_pstate.CanShopping()) return false;
	if(order_count == 0) return false;
	if(id <= 0) return false;
	if(IsZoneServer()) return false;
	if(GetWorldManager()->GetWorldLimit().noshop) return false;

	qgame::mall & shop = item_manager::GetShoppingMall();
	if(slot >= qgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	qgame::mall::node_t node;
	if(!shop.QueryGoods(order_index,node) || node.goods_id != id || node.entry[slot].cash_need <= 0)
	{	
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	//检查限时销售物品
	if(node.sale_time_active)
	{
		time_t t = (time_t)g_timer.get_systime();
		if( !(node._sale_time.IsTimeToSale(t)) )
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
	}

	int goods_count = node.goods_count;
	size_t cash_need = node.entry[slot].cash_need;
	int expire_time = node.entry[slot].expire_time;

	//是否支持销售方案
	if(!((node.props & 0x00FF0000) & (g_config.mall_prop & 0x00FF0000)))
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
		
	//是否支持打折方案
	if(((node.props & 0xFF000000) & (g_config.mall_prop & 0xFF000000)))
	{
		cash_need = cash_need * node.discount / 100;
		if(cash_need == 0) cash_need = 1;
	}

	if(_mallinfo.GetCash() <= 0 || _mallinfo.GetCash()/order_count < cash_need)
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}

	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	int total_count = goods_count * order_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	int need_present_slot = 0;
	if(node.has_present)
	{
		const item_data * pPresentItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.present_id);
		int total_present_count = node.present_count * order_count;
		need_present_slot = total_present_count/pPresentItem->pile_limit + ((total_present_count % pPresentItem->pile_limit)? 1: 0); 
	}
	need_empty_slot += need_present_slot;
	if(total_count <=0  || (int)(total_count/order_count) != goods_count || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return true;
	}

	int expire_date = 0;
	if(expire_time) expire_date = g_timer.get_systime() + expire_time;

	int total_cash = _mallinfo.GetCash();
	int cash_used = 0;
	int self_id = GetParent()->ID.id;
	int bonus_reward = 0;
	int bonus_ratio = node.bonus_ratio + g_config.gshop_bonus_ratio; 
	
	float  cash_need_log = 0;

	//3994为完毕，如果一组完毕有赠品，log记录成0.8, 其他赠品各记录0.5
	//因为买赠会有积分赠送,所有的log里面原来的购买乘以0.77, 另外0.23记录成商城积分
	if(!gmatrix::IsDisableCashGift())
	{
		//打开元宝购买反赠功能，需要乘以0.77
		if(id == 3994 && goods_count == 11)
		{		
			cash_need_log = node.has_present ? cash_need * 0.8 * 0.77 : cash_need * 0.77;  
		}
		else
		{
			cash_need_log = node.has_present ? cash_need * 0.5 * 0.77 : cash_need * 0.77;  
		}
	}
	else
	{
		if(id == 3994 && goods_count == 11)
		{		
			cash_need_log = node.has_present ? cash_need * 0.8  : cash_need;  
		}
		else
		{
			cash_need_log = node.has_present ? cash_need * 0.5  : cash_need;  
		}
	}

	float  cash_left_log = total_cash;	
	int order_id = _mallinfo.GetOrderID(); 

	if(pItem->pile_limit == 1)
	{
		//需要GUID
		ASSERT(goods_count == 1);
		for(size_t i = 0; i < order_count; i ++)
		{
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * pItem2 = gmatrix::GetDataMan().generate_item(id,&tag,sizeof(tag));
			if(pItem2 == NULL) continue;
			//将物品放入包裹中
			int item_count = 1;

			int rst =_inventory.Push(*pItem2,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);

			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);

			int state = item::Proctype2State(pItem2->proc_type);
			_runner->obtain_item(id,expire_date,1,_inventory[rst].count, 0,rst,state);

			total_cash -= cash_need;
			cash_used += cash_need;
			cash_left_log -= cash_need_log; 

			_mallinfo.AddOrder(_mallinfo.GetOrderID(), id, goods_count, cash_need, expire_date,1,pItem2->guid.guid1, pItem2->guid.guid2);
			_mallinfo.IncOrderID();
			FreeItem(pItem2);
		}
	}
	else
	{
		//不需要GUID
		for(int i = total_count; i >0;)
		{
			//将物品放入包裹中
			int item_count = i;
			if((size_t)item_count > pItem->pile_limit) item_count = pItem->pile_limit;
			int ocount = item_count;
			int rst =_inventory.Push(*pItem,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(id,expire_date,ocount,_inventory[rst].count, 0,rst,state);

			i -= ocount;
		}
		for(size_t i = 0; i < order_count; i ++)
		{
			total_cash -= cash_need;
			cash_used += cash_need;
			cash_left_log -= cash_need_log; 
		}

		_mallinfo.AddOrder(_mallinfo.GetOrderID(), id, goods_count, cash_need, expire_date,order_count);
		_mallinfo.IncOrderID();
	}

	//跨服代币记录formatlog的时候做特殊处理, cash_need字段记为0(财务统计需求) 
	GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
			self_id, _db_magic_number, order_id, id, expire_date,goods_count*order_count, (id == g_config.item_zone_cash ) ? 0 : cash_need_log*order_count, cash_left_log);
	GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%.2f点交易序号%d",self_id,goods_count*order_count,id,expire_date,cash_need_log*order_count,order_id);
	GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, self_id, cash_need_log*order_count, GetObjectLevel(), 
					GetMoney(), cash_left_log, id, goods_count*order_count);

	//处理赠品
	if(node.has_present)
	{
		const item_data * pPresentItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.present_id);
		int present_expire_date = 0;
		int total_present_count = node.present_count * order_count;

		float cash_need_log_present = cash_need  - cash_need_log;
		if(!gmatrix::IsDisableCashGift()) cash_need_log_present = cash_need * 0.77 - cash_need_log;

		if(node.present_time) present_expire_date = g_timer.get_systime() + node.present_time;
		if(pPresentItem->pile_limit == 1)
		{
			for(size_t i = 0; i < order_count; i++)
			{
				ASSERT(node.present_count == 1);
				element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
				item_data * pPresentItem2 = gmatrix::GetDataMan().generate_item(node.present_id,&tag,sizeof(tag));
				if(pPresentItem2 == NULL) continue;
				//将物品放入包裹中
				int item_count = 1;
				int rst =_inventory.Push(*pPresentItem2,item_count,present_expire_date);
				ASSERT(rst >= 0 && item_count == 0);

				_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);
				if(node.present_bind) _inventory[rst].Bind();
				int state = item::Proctype2State(_inventory[rst].proc_type);
				_runner->obtain_item(node.present_id,present_expire_date,1,_inventory[rst].count, 0,rst,state);

				FreeItem(pPresentItem2);
			}
		}
		else
		{
			//不需要GUID
			for(int i = total_present_count; i >0;)
			{
				//将物品放入包裹中
				int item_count = i;
				if((size_t)item_count > pPresentItem->pile_limit) item_count = pPresentItem->pile_limit;
				int ocount = item_count;
				int rst =_inventory.Push(*pPresentItem,item_count,present_expire_date);
				ASSERT(rst >= 0 && item_count == 0);
				int state = item::Proctype2State(pPresentItem->proc_type);
				_runner->obtain_item(node.present_id,present_expire_date,ocount,_inventory[rst].count, 0,rst,state);
				i -= ocount;
			}
		}
		cash_left_log -= (cash_need_log_present * order_count); 
		//记录赠品日志  
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
				self_id, _db_magic_number,0,node.present_id, present_expire_date,total_present_count,cash_need_log_present*order_count,cash_left_log);
		GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%.2f点交易序号%d",self_id,total_present_count,node.present_id,expire_date,cash_need_log_present*order_count,0);
		GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, self_id, cash_need_log_present*order_count, GetObjectLevel(), 
				GetMoney(), cash_left_log, node.present_id, total_present_count);
	}

	if(!gmatrix::IsDisableCashGift())
	{
		//记录商城积分日志  
		float cash_gift_log = cash_used * 0.23;
		int cash_gift_num = cash_used / 10;
		cash_left_log -= cash_gift_log; 
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
				self_id, _db_magic_number,0, gmatrix::GetCurCashGiftID1(), 0, cash_gift_num, cash_gift_log, cash_left_log); 
		GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%.2f点交易序号%d",self_id,cash_gift_num,gmatrix::GetCurCashGiftID1(),0,cash_gift_log, 0);
		GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, self_id, cash_gift_log, GetObjectLevel(), 
				GetMoney(), cash_left_log, gmatrix::GetCurCashGiftID1(), cash_gift_num);
	}

	_mallinfo.UseCash(cash_used, this);
	_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	
	if(bonus_ratio > 0)
	{
		bonus_reward = cash_used * bonus_ratio / 100;
		_bonusinfo.AddBonus(bonus_reward);
		_runner->player_bonus(_bonusinfo.GetBonus());
	}

	SendRefCashUsed(cash_used);

	GLog::log(GLOG_INFO,"用户%d在百宝阁购买%d件物品，花费%d点剩余%d点",self_id,order_count,cash_used,_mallinfo.GetCash());
	//考虑加快存盘速度 
	return true;
}

//鸿利商城
bool
gplayer_imp::PlayerDoBonusShopping(int id, size_t order_index, size_t slot, size_t order_count)
{
	if(!_pstate.CanShopping()) return false;
	if(order_count == 0) return false;
	if(id <= 0) return false;
	if(IsZoneServer()) return false;
	if(GetWorldManager()->GetWorldLimit().noshop) return false;

	qgame::mall & shop = item_manager::GetBonusShoppingMall();
	if(slot >= qgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	qgame::mall::node_t node;
	if(!shop.QueryGoods(order_index,node) || node.goods_id != id || node.entry[slot].cash_need <= 0)
	{	
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	int goods_count = node.goods_count;
	size_t bonus_need = node.entry[slot].cash_need;
	int expire_time = node.entry[slot].expire_time;

	if(_bonusinfo.GetBonus() <= 0 || _bonusinfo.GetBonus()/order_count < bonus_need)
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}

	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	int total_count = goods_count * order_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	int need_present_slot = 0;
	if(node.has_present)
	{
		const item_data * pPresentItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.present_id);
		int total_present_count = node.present_count * order_count;
		need_present_slot = total_present_count/pPresentItem->pile_limit + ((total_present_count % pPresentItem->pile_limit)? 1: 0); 
	}
	need_empty_slot += need_present_slot;
	if(total_count <=0  || (int)(total_count/order_count) != goods_count || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return true;
	}

	int expire_date = 0;
	if(expire_time) expire_date = g_timer.get_systime() + expire_time;

	int total_bonus = _bonusinfo.GetBonus();
	int bonus_used = 0;
	int self_id = GetParent()->ID.id;

	if(pItem->pile_limit == 1)
	{
		//需要GUID
		ASSERT(goods_count == 1);
		for(size_t i = 0; i < order_count; i ++)
		{
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * pItem2 = gmatrix::GetDataMan().generate_item(id,&tag,sizeof(tag));
			if(pItem2 == NULL) continue;
			//将物品放入包裹中
			int item_count = 1;

			int rst =_inventory.Push(*pItem2,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);

			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);

			int state = item::Proctype2State(pItem2->proc_type);
			_runner->obtain_item(id,expire_date,1,_inventory[rst].count, 0,rst,state);

			total_bonus -= bonus_need;
			bonus_used += bonus_need;
			//专门记录日志  
			GLog::formatlog("formatlog:gbonusshop_trade:userid=%d:db_magic_number=%d:item_id=%d:"
					"expire=%d:item_count=%d:bonus_need=%d:bonus_left=%d:guid=%d,%d",
					self_id, _db_magic_number, id, expire_date,goods_count,
					bonus_need,total_bonus,pItem2->guid.guid1,pItem2->guid.guid2);
			
			GLog::log(GLOG_INFO, "用户%d鸿利商城购买了%d个%d有效期%d花费了%d点",self_id,goods_count,id,expire_date,bonus_need);

			GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%d:lv=%d:h_gold=%d:h_bonus=%d:itemid=%d,%d", _db_magic_number, self_id, bonus_need, GetObjectLevel(), GetMoney(), total_bonus,id, goods_count);
			FreeItem(pItem2);

		}
	}
	else
	{
		//不需要GUID
		for(int i = total_count; i >0;)
		{
			//将物品放入包裹中
			int item_count = i;
			if((size_t)item_count > pItem->pile_limit) item_count = pItem->pile_limit;
			int ocount = item_count;
			int rst =_inventory.Push(*pItem,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(id,expire_date,ocount,_inventory[rst].count, 0,rst,state);

			i -= ocount;
		}
		for(size_t i = 0; i < order_count; i ++)
		{
			total_bonus -= bonus_need;
			bonus_used += bonus_need;
			//专门记录日志  
			GLog::formatlog("formatlog:gbonusshop_trade:userid=%d:db_magic_number=%d:item_id=%d:expire=%d:item_count=%d:bonus_need=%d:bonus_left=%d:guid=0,0",
					self_id, _db_magic_number, id, expire_date,goods_count,bonus_need,total_bonus);
			GLog::log(GLOG_INFO, "用户%d鸿利商城购买了%d个%d有效期%d花费了%d点",self_id,goods_count,id,expire_date,bonus_need);
			GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%d:lv=%d:h_gold=%d:h_bonus=%d:itemid=%d,%d", _db_magic_number, self_id, bonus_need, GetObjectLevel(), GetMoney(), total_bonus,id, goods_count);

		}
	}

	//处理赠品
	if(node.has_present)
	{
		const item_data * pPresentItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.present_id);
		int present_expire_date = 0;
		if(node.present_time) present_expire_date = g_timer.get_systime() + node.present_time;
		if(pPresentItem->pile_limit == 1)
		{
			for(size_t i = 0; i < order_count; i++)
			{
				for(size_t j = 0; j < node.present_count; ++j)
				{
					element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
					item_data * pPresentItem2 = gmatrix::GetDataMan().generate_item(node.present_id,&tag,sizeof(tag));
					if(pPresentItem2 == NULL) continue;
					//将物品放入包裹中
					int item_count = 1;
					int rst =_inventory.Push(*pPresentItem2,item_count,present_expire_date);
					ASSERT(rst >= 0 && item_count == 0);

					_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);
					if(node.present_bind) _inventory[rst].Bind();

					int state = item::Proctype2State(pPresentItem2->proc_type);
					_runner->obtain_item(node.present_id,present_expire_date,1,_inventory[rst].count, 0,rst,state);
					FreeItem(pPresentItem2);
				}
			}

		}
		else
		{
			int total_present_count = node.present_count * order_count;
			//不需要GUID
			for(int i = total_present_count; i >0;)
			{
				//将物品放入包裹中
				int item_count = i;
				if((size_t)item_count > pPresentItem->pile_limit) item_count = pPresentItem->pile_limit;
				int ocount = item_count;
				int rst =_inventory.Push(*pPresentItem,item_count,present_expire_date);
				ASSERT(rst >= 0 && item_count == 0);
				int state = item::Proctype2State(pPresentItem->proc_type);
				_runner->obtain_item(node.present_id,present_expire_date,ocount,_inventory[rst].count, 0,rst,state);
				i -= ocount;
			}

		}
		GLog::formatlog("formatlog:gbonusshop_present:userid=%d:order_id=%d:item_id=%d:order_count=%d:present_id=%d:present_count=%d:present_time=%d",
				self_id, _mallinfo.GetOrderID(),id, order_count, node.present_id, node.present_count, node.present_time);
	}
	_bonusinfo.UseBonus(bonus_used);
	_runner->player_bonus(_bonusinfo.GetBonus());

	GLog::log(GLOG_INFO,"用户%d在鸿利商城购买%d件物品，花费%d点剩余%d点",self_id,order_count,bonus_used,_bonusinfo.GetBonus());
	return true;
}

//跨服商城
bool
gplayer_imp::PlayerDoZoneShopping(int id, size_t order_index, size_t slot, size_t order_count)
{
	if(!_pstate.CanShopping()) return false;
	if(order_count == 0) return false;
	if(id <= 0) return false;
	if(!IsZoneServer()) return false;

	qgame::mall & shop = item_manager::GetZoneShoppingMall();
	if(slot >= qgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	qgame::mall::node_t node;
	if(!shop.QueryGoods(order_index,node) || node.goods_id != id || node.entry[slot].cash_need <= 0)
	{	
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	int goods_count = node.goods_count;
	size_t zone_cash_need = node.entry[slot].cash_need;
	int expire_time = node.entry[slot].expire_time;

	int zone_cash_total = GetZoneCash();
	if(zone_cash_total <= 0 || (size_t)zone_cash_total < order_count * zone_cash_need)
	{
		//no engouh zone mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}

	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	int total_count = goods_count * order_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	int need_present_slot = 0;
	if(node.has_present)
	{
		const item_data * pPresentItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.present_id);
		int total_present_count = node.present_count * order_count;
		need_present_slot = total_present_count/pPresentItem->pile_limit + ((total_present_count % pPresentItem->pile_limit)? 1: 0); 
	}
	need_empty_slot += need_present_slot;
	if(total_count <=0  || (int)(total_count/order_count) != goods_count || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return true;
	}

	int expire_date = 0;
	if(expire_time) expire_date = g_timer.get_systime() + expire_time;

	int zone_cash_used = 0;
	int self_id = GetParent()->ID.id;
	
	float cash_need_log = 0;

	if(!gmatrix::IsDisableCashGift())
	{
		if(id == 3994 && goods_count == 11)
		{
			cash_need_log = node.has_present ? zone_cash_need *0.8 *0.77 : zone_cash_need*0.77;  
		}
		else
		{
			cash_need_log = node.has_present ? zone_cash_need *0.5 *0.77 : zone_cash_need*0.77;  
		}
	}
	else
	{
		if(id == 3994 && goods_count == 11)
		{
			cash_need_log = node.has_present ? zone_cash_need *0.8  : zone_cash_need;  
		}
		else
		{
			cash_need_log = node.has_present ? zone_cash_need *0.5  : zone_cash_need;  
		}
	}

	float  cash_left_log = zone_cash_total;	
	int order_id = _mallinfo.GetOrderID(); 

	if(pItem->pile_limit == 1)
	{
		//需要GUID
		ASSERT(goods_count == 1);
		for(size_t i = 0; i < order_count; i ++)
		{
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * pItem2 = gmatrix::GetDataMan().generate_item(id,&tag,sizeof(tag));
			if(pItem2 == NULL) continue;
			//将物品放入包裹中
			int item_count = 1;

			int rst =_inventory.Push(*pItem2,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);

			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);

			int state = item::Proctype2State(pItem2->proc_type);
			_runner->obtain_item(id,expire_date,1,_inventory[rst].count, 0,rst,state);

			zone_cash_total -= zone_cash_need; 
			zone_cash_used += zone_cash_need;
			cash_left_log -= node.has_present ? cash_need_log : zone_cash_need; 

			_mallinfo.AddOrder(_mallinfo.GetOrderID(), id, goods_count, zone_cash_need, expire_date,1,pItem2->guid.guid1, pItem2->guid.guid2);
			_mallinfo.IncOrderID();
			FreeItem(pItem2);

		}
	}
	else
	{
		//不需要GUID
		for(int i = total_count; i >0;)
		{
			//将物品放入包裹中
			int item_count = i;
			if((size_t)item_count > pItem->pile_limit) item_count = pItem->pile_limit;
			int ocount = item_count;
			int rst =_inventory.Push(*pItem,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(id,expire_date,ocount,_inventory[rst].count, 0,rst,state);

			i -= ocount;
		}
		for(size_t i = 0; i < order_count; i ++)
		{
			zone_cash_total -= zone_cash_need;
			zone_cash_used += zone_cash_need;
			cash_left_log -= node.has_present ? cash_need_log : zone_cash_need; 
		}
		_mallinfo.AddOrder(_mallinfo.GetOrderID(), id, goods_count, zone_cash_need, expire_date,order_count);
		_mallinfo.IncOrderID();
	}

	//专门记录日志  
	GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
			self_id, _db_magic_number, order_id, id, expire_date,goods_count*order_count,cash_need_log*order_count,cash_left_log);
	GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%.2f点交易序号%d",self_id,goods_count*order_count,id,expire_date,cash_need_log*order_count,order_id);
	GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, self_id, cash_need_log*order_count, GetObjectLevel(), 
					GetMoney(), cash_left_log, id, goods_count*order_count);

	//处理赠品
	if(node.has_present)
	{
		const item_data * pPresentItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.present_id);
		int present_expire_date = 0;
		int total_present_count = node.present_count * order_count;

		float cash_need_log_present = zone_cash_need - cash_need_log;
		if(!gmatrix::IsDisableCashGift()) cash_need_log_present = zone_cash_need*0.77 - cash_need_log;

		if(node.present_time) present_expire_date = g_timer.get_systime() + node.present_time;
		if(pPresentItem->pile_limit == 1)
		{
			for(size_t i = 0; i < order_count; i++)
			{
				for(size_t j = 0; j < node.present_count; ++j)
				{
					element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
					item_data * pPresentItem2 = gmatrix::GetDataMan().generate_item(node.present_id,&tag,sizeof(tag));
					if(pPresentItem2 == NULL) continue;
					//将物品放入包裹中
					int item_count = 1;
					int rst =_inventory.Push(*pPresentItem2,item_count,present_expire_date);
					ASSERT(rst >= 0 && item_count == 0);

					_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);
					if(node.present_bind) _inventory[rst].Bind();
					cash_left_log -= cash_need_log_present; 
					int state = item::Proctype2State(pPresentItem2->proc_type);
					_runner->obtain_item(node.present_id,present_expire_date,1,_inventory[rst].count, 0,rst,state);
					FreeItem(pPresentItem2);
				}
			}

		}
		else
		{
			//不需要GUID
			for(int i = total_present_count; i >0;)
			{
				//将物品放入包裹中
				int item_count = i;
				if((size_t)item_count > pPresentItem->pile_limit) item_count = pPresentItem->pile_limit;
				int ocount = item_count;
				int rst =_inventory.Push(*pPresentItem,item_count,present_expire_date);
				ASSERT(rst >= 0 && item_count == 0);
				int state = item::Proctype2State(pPresentItem->proc_type);
				_runner->obtain_item(node.present_id,present_expire_date,ocount,_inventory[rst].count, 0,rst,state);
				i -= ocount;
				cash_left_log -= cash_need_log_present; 
			}

		}

		//记录赠品日志  
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
				self_id, _db_magic_number,0,node.present_id, present_expire_date,total_present_count,cash_need_log_present*order_count,cash_left_log);
		GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%.2f点交易序号%d",self_id,total_present_count,node.present_id,expire_date,cash_need_log_present*order_count,0);
		GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, self_id, cash_need_log_present*order_count, GetObjectLevel(), 
				GetMoney(), cash_left_log, node.present_id, total_present_count);
	}
	UseZoneCash(zone_cash_used);

	GLog::log(GLOG_INFO,"用户%d在跨服商城购买%d件物品，花费%d点剩余%d点",self_id,order_count,zone_cash_used,GetZoneCash());
	return true;
}


/*
下面的函数已经作废
bool
gplayer_imp::PlayerDoShopping(size_t goods_count,const short * order_list)
{
	if(!_pstate.CanShopping()) return false;
	if(goods_count == 0) 
	{
		return false;
	}
	if(goods_count > _inventory.Size() || !InventoryHasSlot(goods_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	qgame::mall & shop = item_manager::GetShoppingMall();
	qgame::mall_order  order(_mallinfo.GetOrderID());
	size_t offset = 0;
	for(size_t i = 0; i < goods_count; i ++, offset += sizeof(C2S::CMD::mall_shopping::__entry) / sizeof(short))
	{
		int id = order_list[offset]; 
		size_t index = order_list[offset +1] & 0xFFFF;
		size_t slot = order_list[offset +2] & 0xFFFF;
		if(slot >= qgame::mall::MAX_ENTRY)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		qgame::mall::node_t node;
		if(!shop.QueryGoods(index,node) || node.goods_id != id)
		{	
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		
		if(node.entry[slot].cash_need <= 0) 
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		
		order.AddGoods(node.goods_id, node.goods_count,node.entry[slot].cash_need, node.entry[slot].expire_time);
	}
	if(_mallinfo.GetCash() < order.GetPointRequire())
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}

	int total_cash = _mallinfo.GetCash();
	int cash_used = 0;
	//金钱足够， 开始加入物品
	int cur_t = g_timer.get_systime();
	int self_id = GetParent()->ID.id;
	for(size_t i = 0; i < goods_count; i ++)
	{
		int id;
		int count;
		int point;
		int expire_time;
		bool bRst = order.GetGoods(i, id, count,point, expire_time);
		if(bRst)
		{
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
			if(pItem)
			{
				int expire_date = 0;
				if(expire_time) expire_date = cur_t + expire_time;

				int ocount = count;
				int rst =_inventory.Push(*pItem,count,expire_date);
				ASSERT(rst >= 0 && count == 0);
				_runner->obtain_item(id,expire_date,ocount,_inventory[rst].count, 0,rst);

				total_cash -= point;
				cash_used += point;
				//记录日志  
				GLog::formatlog("formatlog:gshop_trade:userid=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%d:cash_left=%d",self_id,_mallinfo.GetOrderID(),id, expire_date,ocount,point,total_cash);
				GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%d点交易序号%d",self_id,ocount,id,expire_date,point,_mallinfo.GetOrderID());
			}
			else
			{
				//记录错误日志
				GLog::log(GLOG_ERR,"用户%d在购买百宝阁物品%d时生成物品失败",self_id, id);
			}
		}
		else
		{
			ASSERT(false);
		}
	}

	_mallinfo.UseCash(cash_used);
	_runner->player_cash(_mallinfo.GetCash());

	GLog::log(GLOG_INFO,"用户%d在百宝阁购买%d样物品，花费%d点剩余%d点",self_id,goods_count,cash_used,_mallinfo.GetCash());

	_mallinfo.IncOrderID();
	//考虑加快存盘速度 
	return true;
}
*/

void gplayer_imp::IncProduceExp(int exp)
{
	int level = _produce_level;
	GLog::log(GLOG_INFO,"用户%d生产技能获得%d经验",_parent->ID.id,exp);
	player_template::IncProduceExp(_produce_level,_produce_exp,exp);
	if( level != _produce_level || exp != _produce_exp)
	{
		GLog::log(GLOG_INFO,"用户%d的生产技能为%d级，当前经验%d",_parent->ID.id,_produce_level, _produce_exp);
		_runner->player_produce_skill_info(_produce_level, _produce_exp);
	}

}

void 
gplayer_imp::OnPickupRawMoney(size_t money,bool isteam)
{
	if(_cheat_punish) return;
	int ext_money = CalcMoneyAddon(money);
	if(isteam)
	{
		GLog::bot("用户%d获得队伍%d金钱%d(+%d)",GetParent()->ID.id,GetTeamID(),money,ext_money);
	}
	else
	{
		GLog::bot("用户%d获得怪物金钱%d(+%d)",GetParent()->ID.id,money,ext_money);
	}
	if (money+ext_money) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=1:reason=3:hint=0",_parent->ID.id,_db_magic_number,money+ext_money);
	size_t nmoney = _player_money;
	money += ext_money;
	GainMoney(money);
	if(_player_money > nmoney)
	{
		_runner->pickup_money(_player_money - nmoney);
	}
	if((int)_player_money < 0) _player_money = 0x7FFFFFFF;
}


bool 
gplayer_imp::OI_GetMallInfo(int &cash_used, int & cash, int &cash_delta,  int &order_id)
{
	_mallinfo.GetInfo(cash, cash_used, cash_delta, order_id);
	return true;
}

bool 
gplayer_imp::OI_IsCashModified()
{
	return _mallinfo.NeedSave();
}

bool
gplayer_imp::OI_IsGathering()
{
	if(_cur_session && _session_state == gactive_imp::STATE_SESSION_GATHERING) return true;
	return false;
}

int
gplayer_imp::OI_GetBonus()
{
	return _bonusinfo.GetBonus();
}

void
gplayer_imp::OI_AddBonus(int inc)
{
	_bonusinfo.AddBonus(inc);
}

namespace
{
class  clear_expire_item
{
	gplayer_imp * _imp;
	bool _notify;
	char _where;
public:
	int _min_date;
	int _cur_t;
	int _remove_count;
	int _remove_mask;
public:
	clear_expire_item(gplayer_imp * __this, bool notify, char where):_imp(__this),_notify(notify),_where(where)
	{
		_cur_t = g_timer.get_systime();
		_min_date = 0;
		_remove_count = 0;
		_remove_mask = 0;
	}
	
	void operator()(item_list * list, size_t index, item & it)
	{
		if(_cur_t >= it.expire_date)
		{
			if(_notify) 
			{
				_imp->_runner->player_drop_item(_where,index,it.type,it.count,S2C::DROP_TYPE_EXPIRE);
			}
			GLog::log(GLOG_INFO,"用户%d位于%d的物品%d(%d个)过期消失",_imp->GetParent()->ID.id,_where,it.type, it.count);
			item it;
			list->Remove(index, it);
			_imp->UseItemLog(it);
			if(it.type == g_config.item_destroy_id)
			{
				
				const void * data;
				size_t data_len;
				it.GetItemData(&data, data_len);
				if(data_len >= sizeof(int)*9)
				{
					int id	  = ((const int *)data)[0]; //顺序看DestroyBindItem的代码 这个顺序应该是不变的 除非是非武林的项目
					int count = ((const int *)data)[2];
					int guid1 = ((const int *)data)[4];
					int guid2 = ((const int *)data)[5];
					//ar >> id >> tmp >> count >> tmp >> guid1 >> guid2;
					_imp->UseItemLog(id, guid1, guid2, count);
				}
			}
			else if(it.type == g_config.item_unlocking_id)
			{
				_imp->RestoreUnlock(*list, index, _where, it);
			}
			it.Release();
			_remove_count ++;
			_remove_mask |= 1 << index;
		}
		else
		{
			if(!_min_date)
			{
				_min_date = it.expire_date;
			}
			else if(_min_date > it.expire_date)
			{
				_min_date = it.expire_date;
			}
		}
		
	}
};

}

void 
gplayer_imp::RemoveAllExpireItems()
{
	clear_expire_item  cei1(this, true, IL_INVENTORY);
	_inventory.ForEachExpireItems(cei1);

	clear_expire_item  cei2(this, true, IL_EQUIPMENT);
	_equipment.ForEachExpireItems(cei2);
	if(cei2._remove_count)
	{
		RefreshEquipment();
		
		CalcEquipmentInfo();
		_runner->equipment_info_changed(0,cei2._remove_mask, 0, 0); //此函数使用了CalcEquipmentInfo的结果
		IncEquipChangeFlag();
	}
	
	clear_expire_item  cei3(this, true, IL_TRASH_BOX);
	_trashbox.Backpack().ForEachExpireItems(cei3);

	clear_expire_item  cei4(this, true, IL_TRASH_BOX2);
	_trashbox.MafiaBackpack().ForEachExpireItems(cei4);

	clear_expire_item  cei5(this, true, IL_PET_BEDGE);
	_pet_bedge_inventory.ForEachExpireItems(cei5);
	if(cei5._remove_count)
	{
		//有宠物牌消失了
		_petman.SomePetBedgeExpired(this);
	}

	clear_expire_item  cei6(this, true, IL_PET_EQUIP);
	_pet_equip_inventory.ForEachExpireItems(cei6);
	if(cei6._remove_count)
	{
		//有宠物装备消失了
		_petman.SomePetEquipExpired(this);
	}

	clear_expire_item  cei7(this, true, IL_FASHION);
	_fashion_inventory.ForEachExpireItems(cei7);

	clear_expire_item  cei8(this, true, IL_MOUNT_WING);
	_mount_wing_inventory.ForEachExpireItems(cei8);

	clear_expire_item  cei9(this, true, IL_GIFT);
	_gift_inventory.ForEachExpireItems(cei9);

	clear_expire_item  cei10(this, true, IL_FUWEN);
	_fuwen_inventory.ForEachExpireItems(cei10);
	if(cei10._remove_count)
	{
		RefreshFuwen();
	}

	_expire_item_date = 0;
	if(cei1._min_date) UpdateExpireItem(cei1._min_date);
	if(cei2._min_date) UpdateExpireItem(cei2._min_date);
	if(cei3._min_date) UpdateExpireItem(cei3._min_date);
	if(cei4._min_date) UpdateExpireItem(cei4._min_date);
	if(cei5._min_date) UpdateExpireItem(cei5._min_date);
	if(cei6._min_date) UpdateExpireItem(cei6._min_date);
	if(cei7._min_date) UpdateExpireItem(cei7._min_date);
	if(cei8._min_date) UpdateExpireItem(cei8._min_date);
	if(cei9._min_date) UpdateExpireItem(cei9._min_date);

	//单独检查物品回购包裹
	int cur_time = g_timer.get_systime();
	int repurchase_min_date = 0;
	std::list<item>::iterator it_inv = _repurchase_inventory.begin();
	while(it_inv != _repurchase_inventory.end())
	{
		if( (-1 != (*it_inv).type) && (0 != (*it_inv).expire_date) && (cur_time >= (*it_inv).expire_date) )
		{
			(*it_inv).type = -1;
		}
		else if(0 != (*it_inv).expire_date)
		{
			if(!repurchase_min_date)
			{
				repurchase_min_date = (*it_inv).expire_date;
			}
			else if(repurchase_min_date > (*it_inv).expire_date)
			{
				repurchase_min_date = (*it_inv).expire_date;
			}
		}

		++it_inv;
	}
	if(repurchase_min_date) UpdateExpireItem(repurchase_min_date);
	UpdateRepurchaseInv();
}

void 
gplayer_imp::ClearHurtEntry()
{
	_dmg_list.clear();
}

void  
gplayer_imp::AddHurtEntry(const XID & attacker, int damage, int team_id,int team_seq)
{
	DAMAGE_MAP::iterator it = _dmg_list.find(attacker);
	if(it != _dmg_list.end())
	{
		hurt_entry & ent = it->second;
		ent.team_id = team_id;
		ent.team_seq = team_seq;
		ent.damage += damage;
	}
	else
	{
		if(_dmg_list.size() >= MAX_HURT_ENTRY)
		{	
			//人数满了，不再加入新的数据了
			return ;
		}
		hurt_entry & ent = _dmg_list[attacker];
		ent.team_id = team_id;
		ent.team_seq= team_seq;
		ent.damage = damage;
	}
}


namespace
{
	struct  TempDmgEntry
	{
		int damage;
		TempDmgEntry():damage(0)
		{}
	};      
};      
void  
gplayer_imp::GetMaxHurtTeam(XID & owner, int team_id, int team_seq)
{
	typedef abase::hash_map<XID,TempDmgEntry,XID_HashFunc, abase::fast_alloc<> >  TempDmgMap;
	TempDmgMap dlist(_dmg_list.size());

	DAMAGE_MAP::iterator it = _dmg_list.begin();
	int max_damage = -1;                //单体最大伤害造成的伤害值
	int max_team_id = 0;                    //最大伤害的team_id
	int max_team_seq = -1;                  //最大伤害的team_seq
	XID max_id = XID(0,0);
	int total_damage = 0;
	for(;it != _dmg_list.end(); ++it)
	{
		int damage = it->second.damage;
		total_damage += damage;
		if(it->second.team_id > 0)
		{
			TempDmgEntry &ent = dlist[XID(-it->second.team_id,it->second.team_seq)];
			ent.damage += damage;

			if(max_damage < ent.damage)
			{
				max_damage = ent.damage;
				max_team_id = it->second.team_id;
				max_team_seq = it->second.team_seq;
				max_id = XID(-it->second.team_id,it->second.team_seq);
			}
		}
		else
		{
			dlist[it->first].damage += damage;
			if(max_damage < damage)
			{
				max_damage = damage;
				max_team_id = 0;
				max_team_seq = -1;
				max_id = it->first;
			}
		}
	}

	if(max_team_id > 0)
	{
		owner = XID(0,0);
		team_id = max_team_id;
		team_seq = max_team_seq;
	}
	else
	{
		owner = max_id;
		team_id = 0;
		team_seq = -1;
	}
}

void 
gplayer_imp::ChangeInventorySize(int size)
{
	if(size > ITEM_LIST_MAX_SIZE) size = ITEM_LIST_MAX_SIZE;
	if(size < ITEM_LIST_BASE_SIZE) size = ITEM_LIST_BASE_SIZE;
	_inventory.SetSize(size);
	_runner->player_change_inventory_size(_inventory.Size());
}

void 
gplayer_imp::ChangeMountWingInventorySize(int size)
{
	if(size > MOUNT_WING_INVENTORY_MAX_SIZE) size = MOUNT_WING_INVENTORY_MAX_SIZE;
	if(size < MOUNT_WING_INVENTORY_BASE_SIZE) size = MOUNT_WING_INVENTORY_BASE_SIZE;
	_mount_wing_inventory.SetSize(size);
	_runner->player_change_mountwing_inv_size(_mount_wing_inventory.Size());
}

bool    
gplayer_imp::CheckItemBindCondition(size_t index,  int id)
{       
	if(!IsItemExist(index, id,1)) return false;
	const item & it = _inventory[index];
	if(it.proc_type & item::ITEM_PROC_TYPE_BIND) return false;      //已经绑定过了
	if(it.proc_type & item::ITEM_PROC_TYPE_BIND2) return false;      //已经绑定过了
	if(!(it.equip_mask & item::EQUIP_MASK_CAN_BIND)) return false;  //不符合的装备位置，不能绑定
	if(it.pile_limit != 1)  return false;                           //可以堆叠的不能绑定
	return true;
}

bool 
gplayer_imp::CheckItemLockCondition(size_t index,  int id)
{
	if(!IsItemExist(index, id,1)) return false;
	const item & it = _inventory[index];
	if(it.pile_limit != 1)  return false;                           //可以堆叠的不能绑定
	if(!it.CanLock()) return false;				//已经是可以交易的就不能锁定了
	DATA_TYPE dt;
	const void * pRst = gmatrix::GetDataMan().get_data_ptr(it.type, ID_SPACE_ESSENCE,dt); 
	if(pRst == NULL || dt == DT_DESTROYING_ESSENCE) return false;	//任何类型的等待销毁的物品都不能加锁
	return true;
}

bool 
gplayer_imp::BindItem(size_t index, int id)
{
	ASSERT(IsItemExist(index, id,1));
	//前面已经确认检查过是否可以进行绑定了
	item & it = _inventory[index];

	//立刻绑定
	it.Bind();

	//装备绑定
//	it.proc_type |= item::ITEM_PROC_TYPE_BIND2;
	//再次发送一次物品数据
	PlayerGetItemInfo(IL_INVENTORY,index);

	_runner->player_bind_success(index, id, it.GetProctypeState());
	GLog::log(GLOG_INFO,"用户%d绑定了物品%d",_parent->ID.id,id);
	return true;
}

bool 
gplayer_imp::CheckBindItemDestroy(size_t index, int id)
{
	if(!IsItemExist(index, id,1)) return false;
	const item & it = _inventory[index];
	if(!(it.proc_type & item::ITEM_PROC_TYPE_BIND)) return false;   //未绑定的物品不能进行此种销毁
	if(it.IsLocked()) return false;   //已经锁定的物品不能摧毁
	return true;
}

bool 
gplayer_imp::CheckItemUnlock(size_t index, int id)
{
	if(!IsItemExist(index, id,1)) return false;
	const item & it = _inventory[index];
	if(!it.IsLocked()) return false;
	if(g_config.item_unlocking_id <= 0) return false;
	DATA_TYPE dt;
	const void * pRst = gmatrix::GetDataMan().get_data_ptr(it.type, ID_SPACE_ESSENCE,dt); 
	if(pRst == NULL || dt == DT_DESTROYING_ESSENCE) return false;	//任何类型的等待销毁的物品都不能解锁
	return true;
}

bool 
gplayer_imp::DestroyBindItem(size_t index, int id, item_list& inv, int where, int destroy_type)
{
	ASSERT(inv.IsItemExist(index, id,1));
	//已经处于销毁中的物品不会破碎,时间会有问题,其实不该进到这里来,收集的时候就排除了,不过还是判一下吧
	if(g_config.item_broken_id && g_config.item_broken_id == id) return false;
	//已经处于破碎状态的物品不会破碎,会嵌套,其实不该进到这里来,收集的时候就排除了,不过还是判一下吧
	if(g_config.item_destroy_id && g_config.item_destroy_id == id) return false;
	//前面已经确认检查过是否可以进行绑定了
	item  it;
	inv.Remove(index, it);
	GDB::itemdata data;
	ConvertDBItemEntry(data, it, -1);

	//组织本物品的数据包
	packet_wrapper h1(200);
	h1 << data.id << data.index << data.count 
		<< (int)data.max_count << data.guid1 << data.guid2
		<< data.proctype << data.expire_date;

	h1 << data.size;
	h1.push_back(data.data,data.size);

	//用这个数据包组织新的物品
	item_data nit;
	nit.count       = 1;
	nit.pile_limit  = 1;
	nit.proc_type = 0;
	if(destroy_type == 0)
	{
		nit.type        = g_config.item_destroy_id;
		item::CustomBind(nit.proc_type);
	}
	else
	{
		nit.type        = g_config.item_broken_id;
		item::CustomBind(nit.proc_type);
		item::CustomLock(nit.proc_type);
	}
	nit.guid.guid1  = 0;
	nit.guid.guid2  = 0;
	nit.price       = 0;
	nit.expire_date = g_timer.get_systime()  + 7*24*3600;
	nit.content_length = h1.size();
	nit.item_content = (char *)h1.data();

	//通知客户端删除物品
	//并真的删除物品
	_runner->player_drop_item(where,index, it.type, it.count,destroy_type?S2C::DROP_TYPE_DEATH_DAMAGE:S2C::DROP_TYPE_USE);
	it.Release();

	if(destroy_type == 0)
	{
		//将新的物品加入到玩家包裹之中
		int rst = inv.PushInEmpty(0, nit, 1);
		if(rst >=0)
		{       
			int state = item::Proctype2State(nit.proc_type);
			_runner->obtain_item(nit.type,nit.expire_date,1,inv[rst].count, where,rst,state);
		}
		else
		{       
			ASSERT(false);  //这是不应该出现的
		}
	}
	else
	{
		inv.PushInEmpty(index, nit, 1);
		PlayerGetItemInfo(where, index);
	}
	GLog::log(GLOG_INFO,"用户%d摧毁了绑定了物品%d到%d",_parent->ID.id,id, nit.type);
	return true;
}

bool 
gplayer_imp::CheckRestoreDestroyItem(size_t index, int id)
{
	if(!IsItemExist(index, id,1)) return false;
	if(id != g_config.item_destroy_id) return false;
	return true;
}

bool 
gplayer_imp::CheckRestoreBrokenItem(size_t index, int id)
{
	if(!IsItemExist(index, id,1)) return false;
	if(id != g_config.item_broken_id) return false;
	return true;
}

bool 
gplayer_imp::RestoreDestroyItem(size_t index, int id)
{
	ASSERT(IsItemExist(index, id,1));

	const item  &it = _inventory[index];

	const void * data;
	size_t data_len;
	it.GetItemData(&data, data_len);

	if(data_len < sizeof(int)*9) return false; //sizeof(int)*9 是物品的必要数据，考虑将此值用宏替代

	GDB::itemdata idata;
	raw_wrapper ar(data,data_len);

	ar >> idata.id >> idata.index >> idata.count
		>> (int&)idata.max_count >> idata.guid1 >> idata.guid2
		>> idata.proctype >> idata.expire_date;
	ar >> idata.size;

	idata.data = ar.cur_data();
	if(idata.size != ar.size() - ar.offset()) return false;

	item new_item;
	if(!MakeItemEntry(new_item,idata)) return false;

	//删除原来的物品
	item  old_item;
	_inventory.Remove(index, old_item);
	_runner->player_drop_item(IL_INVENTORY,index, old_item.type, old_item.count,S2C::DROP_TYPE_USE);
	old_item.Release();

	//将新的物品加入到包裹之中
	int type = new_item.type;
	int expire_date = new_item.expire_date;
	int count = new_item.count;
	int rst = _inventory.Push(new_item);
	if(rst >=0)
	{
		int state = item::Proctype2State(new_item.proc_type);
		_runner->obtain_item(type,expire_date,count,_inventory[rst].count, IL_INVENTORY,rst,state);
		item_data_client data;
		if (_inventory.GetItemDataForClient(rst,data) > 0)
		{
			_runner->self_item_info(IL_INVENTORY,rst,data,0);
		}
	}
	else
	{
		new_item.Release();
		ASSERT(false);
	}

	GLog::log(GLOG_INFO,"用户%d恢复了绑定摧毁%d从%d ",_parent->ID.id,type,id);
	new_item.Clear();
	return true;
}

bool
gplayer_imp::ForgetSkill(int type)
{
	int rst = 0;
	const char * str = NULL;
	switch(type)
	{
	case 0:
		rst = _skill.Forget(true, this);	//全洗
		str = "所有类型";
		TaskSendMessage(12901, GMSV::CHAT_CHANNEL_FARCRY, 0, 0);	//发送全洗符任务广播
		break;
	case 1:
		rst = _skill.Forget(false, this);	//半洗
		str = "当前职业";
		break;
	case 2:
		rst = _skill.ForgetTalent(this);
		if(rst > 0)
		{
			str = "天赋";
			ModifyTalentPoint(rst);
		}
		break;
	}
	if(rst <=0) return false;
	GLog::log(GLOG_INFO,"用户%d执行了洗点操作(%d:%s)，恢复了'%d'点数", 
			_parent->ID.id,type, str,rst);
	_runner->get_skill_data();
	_runner->get_combine_skill_data();
	return true;
}

void gplayer_imp::ForgetCulSkills()
{
	_skill.ForgetCulSkills(this);
	_runner->get_skill_data();
	_runner->get_combine_skill_data();
}

void gplayer_imp::ForgetDeitySkills()
{
	_skill.ForgetDeitySkills(this);
	_runner->get_skill_data();
	_runner->get_combine_skill_data();
}

void 
gplayer_imp::DoTeamRelationTask(int reason)
{
	//必须处于队伍且只有队长能发起此类调用
	if(!IsInTeam() || !IsTeamLeader()) return;
	if(GetTeamMemberNum() <= 1) return ;
	//对这个操作加入1秒钟冷却
	if(!CheckAndSetCoolDown(COOLDOWN_INDEX_TEAM_RELATION, 1000)) return ;
	
	class TeamRelationExec : public ONET::Thread::Runnable
	{
		int _leader;
		int _reason;
		abase::vector<int , abase::fast_alloc<> > _list;
	public:
		TeamRelationExec(gplayer_imp * pImp, int reason, const XID * list, size_t count)
		{
			_leader = pImp->GetParent()->ID.id;
			_reason = reason;
			_list.reserve(count);
			for(size_t i = 0; i < count ; i ++)
			{	
				_list.push_back(list[i].id);
			}
		}
		virtual void Run()
		{
			OnRun();
			delete this;
		}

		bool OnRun()
		{
			//首先根据所有人的ID得到所有的pPlayer
			abase::vector<gplayer *, abase::fast_alloc<> > plist;
			abase::vector<int *, abase::fast_alloc<> > llist;
			gplayer * pLeader = NULL;
			plist.reserve(_list.size());
			llist.reserve(_list.size());
			for(size_t i = 0; i < _list.size();i ++)
			{
				int foo;
				gplayer *pPlayer = gmatrix::FindPlayer(_list[i],foo);
				if(pPlayer == NULL) return false;	//找不到指定的队友
				plist.push_back(pPlayer);
				llist.push_back(&pPlayer->spinlock);
				if(_list[i] == _leader) pLeader = pPlayer;
			}
			if(pLeader == NULL) return false;	//找不到发起者（队长）

			std::sort(llist.begin(), llist.end());

			//锁定所有玩家
			for(size_t i = 0; i < llist.size();i ++)
			{
				mutex_spinlock(llist[i]);
			}

			try
			{
				//检查所有人的状态位置是否正确
				A3DVECTOR pos = pLeader->pos;
				int tag = pLeader->tag;
				for(size_t i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					if(pPlayer->ID.id != _list[i])  throw -3;
					if(!pPlayer->IsActived() || 
							pPlayer->IsZombie() || 
							pPlayer->login_state != gplayer::LOGIN_OK) throw -4;
					if(pPlayer->pos.squared_distance(pos) > 30.f*30.f||
							pPlayer->tag != tag) throw -5;
					if(!pPlayer->imp) throw -6;
					if(!pPlayer->imp->CanTeamRelation()) throw -7;
					if(!pPlayer->imp->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()))
						throw -8;

				}

				//检查队伍状态是否符合
				gplayer_imp * pLmp = (gplayer_imp *) pLeader->imp; 
				if(!pLmp->IsInTeam() || !pLmp->IsTeamLeader()) throw -9;
				for(size_t i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					if(pPlayer == pLeader) continue;
					if(!pLmp->IsMember(pPlayer->ID)) throw -10;
				}
				
				PlayerTaskTeamInterface TaskTeam;
				//使用任务的检查接口进行检查
				for(size_t i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					gplayer_imp * pImp = (gplayer_imp *) pPlayer->imp; 
					PlayerTaskInterface TaskIf(pImp);
					if(!TaskIf.OnCheckTeamRelationship(_reason,&TaskTeam))
					{
						throw -11;
					}
				}

				//所有检查都通过了，调用任务的执行接口
				for(size_t i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					gplayer_imp * pImp = (gplayer_imp *) pPlayer->imp; 
					PlayerTaskInterface TaskIf(pImp);
					TaskIf.OnCheckTeamRelationshipComplete(_reason,&TaskTeam);
				}

				//最终执行队伍的逻辑关系
				TaskTeam.Execute(plist.begin(), plist.size());
			}catch(int e)
			{
				__PRINTF("组队关系任务检查失败，错误号:%d\n", e);
			}

			//解锁所有玩家
			for(size_t i = 0; i < llist.size();i ++)
			{
				mutex_spinunlock(llist[i]);
			}
			return true;
		}
	};

	XID list[TEAM_MEMBER_CAPACITY];
	int count = GetMemberList(list);
	ONET::Thread::Pool::AddTask( new TeamRelationExec(this, reason, list, count));
}

bool 
gplayer_imp::CanTeamRelation()
{
	if(!_pstate.IsNormalState()) return false;
	return true;
}

void 
gplayer_imp::PVPCombatHeartbeat()
{
	if(_pk_value)
	{
		if(_pk_value > 0)
		{
			if((_pk_value % PKVALUE_PER_POINT) == 1)
			{
				_pk_value --;
				int nlevel = player_template::CalcPKLevel(GetPKValue()); 
				if(GetPKLevel() != nlevel)
				{
					_pk_level = nlevel;
					SetParentPKLevel();
					_runner->pk_level_notify(GetPKLevel());
				}
				_runner->pk_value_notify(GetPKValue());

			}
			else
			{
				_pk_value --;
			}
		}
		else
		{
			if(((-_pk_value) % PKVALUE_PER_POINT) == 1)
			{
				_pk_value ++;
				int nlevel = player_template::CalcPKLevel(GetPKValue()); 
				if(GetPKLevel() != nlevel)
				{
					_pk_level = nlevel;
					SetParentPKLevel();
					_runner->pk_level_notify(GetPKLevel());
				}
				_runner->pk_value_notify(GetPKValue());
			}
			else
			{
				_pk_value ++;
			}
		}
	}

	if(_invader_counter > 0)
	{
		_invader_counter --;
		if(_invader_counter == 0)
		{
			GetParent()->ClrObjectState(gactive_object::STATE_INVADER);
			_runner->player_invader_state(false);
		}
	}

	if(!_pvp_combat_timer) return;
	if((--_pvp_combat_timer) <= 0)
	{
		GetParent()->ClrObjectState(gactive_object::STATE_IN_PVP_COMBAT);
		_runner->active_pvp_combat_state(false);
		_pvp_combat_timer = 0;
	}

}

void gplayer_imp::OI_UpdateAllProp()
{
	property_policy::UpdatePlayer(GetObjectClass(), this);
}


void gplayer_imp::DeliverItem(int item_id,int count, bool bind, int lPeriod,int deliver_type, int& inv_idx)
{
	inv_idx = -1;
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * pData = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
	if(pData)
	{
		if(bind)
		{
			item::Bind(pData->proc_type);
		}
		if(count > (int)pData->pile_limit)
			pData->count = pData->pile_limit;
		else
			pData->count = count;

		if(pData->pile_limit == 1)
		{
			pData->expire_date = (lPeriod <=0?0:lPeriod + g_timer.get_systime());
		}
		//返回false表明没有全部放入
		if(ObtainItem(gplayer_imp::IL_INVENTORY,pData,inv_idx,true,deliver_type)) FreeItem(pData);
	}
	else
	{
		//出错，打印错误
	}
}

void gplayer_imp::DeliverItem(int item_id,int count, bool bind, int lPeriod,int deliver_type)
{
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * pData = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
	if(pData)
	{
		if(bind)
		{
			item::Bind(pData->proc_type);
		}
		if(count > (int)pData->pile_limit)
			pData->count = pData->pile_limit;
		else
			pData->count = count;

		if(pData->pile_limit == 1)
		{
			pData->expire_date = (lPeriod <=0?0:lPeriod + g_timer.get_systime());
		}
		//返回false表明没有全部放入
		if(ObtainItem(gplayer_imp::IL_INVENTORY,pData,true,deliver_type)) FreeItem(pData);
	}
	else
	{
		//出错，打印错误
	}
}

void gplayer_imp::GainItem(int item_id, int item_count, bool bind, int lPeriod, int deliver_type)
{
	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
	while(item_count > 0)
	{
		int count = item_count;
		if((size_t)item_count > pile_limit) 
		{
			count = pile_limit;
		}
		DeliverItem(item_id,count, bind, lPeriod, deliver_type);
		item_count -= count;
	}
}

void gplayer_imp::DeliverTaskItem(int item_id, int count)
{
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0}; 

	//这里是否可以用卖出方式完成?
	item_data * pData = gmatrix::GetDataMan().generate_item(item_id, &tag, sizeof(tag));
	if(pData)
	{
		if(count > (int)pData->pile_limit) 
			pData->count = pData->pile_limit;
		else
			pData->count = count;
		//返回false表明没有全部放入
		if(ObtainItem(gplayer_imp::IL_TASK_INVENTORY, pData, true)) FreeItem(pData);
	}
	else
	{
		//出错，打印错误
	}
}

void gplayer_imp::DeliverReinforceItem(int item_id,int reinforce_level, bool bind, int lPeriod,int deliver_type)
{
	if(reinforce_level <= 0) return;
	if(GetInventory().GetEmptySlotCount()  == 0) return;

	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * pItem= gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
	if(pItem)
	{
		if(bind)
		{
			item::Bind(pItem->proc_type);
		}
		pItem->count = 1;
		if(pItem->pile_limit == 1)
		{
			pItem->expire_date = (lPeriod <=0?0:lPeriod + g_timer.get_systime());
		}
		if(pItem)
		{
			int rst = _inventory.Push(*pItem);
			if(rst >= 0)
			{
				_inventory[rst].InitFromShop(this, deliver_type);
				item & it = _inventory[rst];
				it.InitReinforceLevel(reinforce_level);
				int state = item::Proctype2State(pItem->proc_type);
				_runner->obtain_item(item_id,pItem->expire_date,1,_inventory[rst].count, gplayer_imp::IL_INVENTORY, rst,state);
			}
		}
	}
}

bool
gplayer_imp::PlayerLotteryCashing(size_t inv_index)
{
	if(inv_index >= _inventory.Size() || _inventory[inv_index].type <= 0) return false;
	int bonus_level = 0, bonus_money = 0, prize_id = -1, p_count = 0, p_bind = 0, p_period = 0 ;
	if(!_inventory[inv_index].LotteryCashing(this, &bonus_level,&bonus_money,&prize_id,&p_count,&p_bind,&p_period))
	{
		return false;
	}
	int type = _inventory[inv_index].type;
	int count = _inventory[inv_index].count;
	_runner->lottery_bonus(_inventory[inv_index].type, bonus_level, prize_id, p_count, bonus_money);
	//发奖 首先删除旧的彩票 保证有地方
	_inventory.Remove(inv_index);
	_runner->player_drop_item(IL_INVENTORY,inv_index, type, count,S2C::DROP_TYPE_USE);
	GLog::log(GLOG_INFO,"用户%d兑换彩票%d等级%d奖品(%d,%d)奖金%d", _parent->ID.id,  type, bonus_level, prize_id, p_count, bonus_money);
	GLog::formatlog("formatlog:lottery:userid=%d:lottery_item=%d:bonus=%d:bonus_item=%d:bonus_count=%d:bonus_money=%d", _parent->ID.id,  type, bonus_level, prize_id, p_count, bonus_money);


	if(p_count > 0 && prize_id != -1)
	{
		//发放物品奖励 以任务方式发放， 会调用InitFromShop
		DeliverItem(prize_id,p_count,p_bind,p_period,ITEM_INIT_TYPE_LOTTERY);
	}

	if(bonus_money > 0)
	{
		//发放金钱奖励
		GainMoneyWithDrop(bonus_money);
		_runner->task_deliver_money(bonus_money,GetMoney());
	}

	return 0;
}

void gplayer_imp::PlayerMovePetBedge(size_t src_index,size_t dst_index)
{
	if(src_index >= _pet_bedge_inventory.Size() || dst_index >= _pet_bedge_inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	if(_lock_pet_bedge)
	{
		_runner->error_message(S2C::ERR_PET_BEDGE_IS_LOCKED);
		return;
	}
	bool type1 = (-1 == _pet_bedge_inventory[src_index].type);
	bool type2 = (-1 == _pet_bedge_inventory[dst_index].type);
	if(type1 && type2)
	{
		//两个位置都是空的
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}
	//src 非空,或者dst 非空,调换
	int rst = _petman.TestCanMovePetBedge(this,src_index,dst_index);
	if(0 != rst)
	{
		_runner->error_message(rst);
		return;
	}
	//交换之前要先把召唤出来的宠物收了
	_petman.ClearPetBehavior(this,src_index);
	_petman.ClearPetBehavior(this,dst_index);
	item it1;
	item it2;
	_pet_bedge_inventory.Remove(src_index,it1);
	_pet_bedge_inventory.Remove(dst_index,it2);
	_pet_bedge_inventory.Put(dst_index,it1);
	_pet_bedge_inventory.Put(src_index,it2);
	it1.Clear();
	it2.Clear();
	_runner->player_move_pet_bedge(src_index,dst_index);
	int count1 = _pet_bedge_inventory[src_index].count;
	int count2 = _pet_bedge_inventory[dst_index].count;
	ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
	PlayerGetItemInfo(IL_PET_BEDGE,src_index);
	PlayerGetItemInfo(IL_PET_BEDGE,dst_index);
	_petman.OnMovePetBedge(this,dst_index,src_index);
}

void gplayer_imp::PlayerEquipPetBedge(size_t inv_index,size_t pet_index)
{
	if(inv_index >= _inventory.Size() || pet_index >= _pet_bedge_inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	if(_lock_pet_bedge)
	{
		_runner->error_message(S2C::ERR_PET_BEDGE_IS_LOCKED);
		return;
	}
	bool type1 = (-1 == _inventory[inv_index].type);
	bool type2 = (-1 == _pet_bedge_inventory[pet_index].type);
	if(type1 && type2)
	{
		//两个位置都是空的
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}
	const item& it = _inventory[inv_index];
	if(!type1)
	{
		//包裹栏非空
		if(item_body::ITEM_TYPE_PET_BEDGE != it.GetItemType())
		{
			//类型不对,想装备的物品不是宠物牌
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}
		if(-1 == it.type || NULL == it.body)
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}
		int rst = _petman.TestCanEquipPetBedge(this,inv_index,pet_index);
		if(0 != rst)
		{
			_runner->error_message(rst);
			return;
		}
		//交换之前要脱离宠物绑定状态
		//交换之前要先把召唤出来的宠物收了
		_petman.ClearPetBehavior(this,pet_index);
		item it1;
		item it2;
		_inventory.Remove(inv_index,it1);
		_pet_bedge_inventory.Remove(pet_index,it2);
		_pet_bedge_inventory.Put(pet_index,it1);
		_inventory.Put(inv_index,it2);
		it1.Clear();
		it2.Clear();
		_runner->player_equip_pet_bedge(inv_index,pet_index);
		//这里需要检查绑定操作
		item& temp = _pet_bedge_inventory[pet_index];
		ASSERT(-1 != temp.type);
		if(temp.IsBindOnEquip())
		{
			//满足绑定条件,进行绑定
			temp.BindOnEquip();
			GLog::log(LOG_INFO,"用户%d装备绑定物品%d,GUID(%d,%d)",_parent->ID.id,temp.type,temp.guid.guid1,temp.guid.guid2);
			PlayerGetItemInfo(IL_PET_BEDGE,pet_index);
		}
		int count1 = _inventory[inv_index].count;
		int count2 = _pet_bedge_inventory[pet_index].count;
		ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
		PlayerGetItemInfo(IL_INVENTORY,inv_index);
		PlayerGetItemInfo(IL_PET_BEDGE,pet_index);
		_petman.OnEquipPetBedge(this,pet_index);
		return;
		
	}
	//包裹栏为空,拿下原来的,原来位置一定有东西
	//拿下之前要先把召唤出来的宠物收了
	_petman.ClearPetBehavior(this,pet_index);
	item it1;
	_pet_bedge_inventory.Remove(pet_index,it1);
	bool bRst = _inventory.Put(inv_index,it1);
	ASSERT(bRst);
	if(bRst)
	{
		it1.Clear();
	}
	else
	{
		//记录错误日志
		GLog::log(GLOG_ERR,"装备宠物牌时发生致命错误");
		it1.Release();
	}
	int count1 = _inventory[inv_index].count;
	ASSERT(count1 > 0);
	_runner->player_equip_pet_bedge(inv_index,pet_index);
	PlayerGetItemInfo(IL_INVENTORY,inv_index);
	PlayerGetItemInfo(IL_PET_BEDGE,pet_index);
}

void gplayer_imp::PlayerEquipPetEquip(size_t inv_index,size_t pet_equip_index)
{
	if(inv_index >= _inventory.Size() || pet_equip_index >= _pet_equip_inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	if(_lock_pet_equip)
	{
		_runner->error_message(S2C::ERR_PET_EQUIP_IS_LOCKED);
		return;
	}
	bool type1 = (-1 == _inventory[inv_index].type);
	bool type2 = (-1 == _pet_equip_inventory[pet_equip_index].type);
	if(type1 && type2)
	{
		//两个位置都是空的
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}
	const item& it = _inventory[inv_index];
	if(!type1)
	{
		//包裹栏非空
		if(item_body::ITEM_TYPE_PET_EQUIP != it.GetItemType())
		{
			//类型不对,想装备的物品不是宠物装备
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}
		if(-1 == it.type || NULL == it.body)
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}
		int rst = _petman.TestCanEquipPetEquip(this,inv_index,pet_equip_index);
		if(0 != rst)
		{
			_runner->error_message(rst);
			return;
		}
		item it1;
		item it2;
		_inventory.Remove(inv_index,it1);
		_pet_equip_inventory.Remove(pet_equip_index,it2);
		_pet_equip_inventory.Put(pet_equip_index,it1);
		_inventory.Put(inv_index,it2);
		it1.Clear();
		it2.Clear();
		_runner->player_equip_pet_equip(inv_index,pet_equip_index);
		//这里需要检查绑定操作
		item& temp = _pet_equip_inventory[pet_equip_index];
		ASSERT(-1 != temp.type);
		if(temp.IsBindOnEquip())
		{
			//满足绑定条件,进行绑定
			temp.BindOnEquip();
			GLog::log(LOG_INFO,"用户%d装备绑定物品%d,GUID(%d,%d)",_parent->ID.id,temp.type,temp.guid.guid1,temp.guid.guid2);
			PlayerGetItemInfo(IL_PET_EQUIP,pet_equip_index);
		}
		int count1 = _inventory[inv_index].count;
		int count2 = _pet_equip_inventory[pet_equip_index].count;
		ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
		PlayerGetItemInfo(IL_INVENTORY,inv_index);
		PlayerGetItemInfo(IL_PET_EQUIP,pet_equip_index);
		_petman.OnEquipPetEquip(this,pet_equip_index);
		return;
		
	}
	//包裹栏为空,拿下原来的,原来位置一定有东西
	item it1;
	_pet_equip_inventory.Remove(pet_equip_index,it1);
	bool bRst = _inventory.Put(inv_index,it1);
	ASSERT(bRst);
	if(bRst)
	{
		it1.Clear();
	}
	else
	{
		//记录错误日志
		GLog::log(GLOG_ERR,"装备宠物装备时发生致命错误");
		it1.Release();
	}
	int count1 = _inventory[inv_index].count;
	ASSERT(count1 > 0);
	_runner->player_equip_pet_equip(inv_index,pet_equip_index);
	PlayerGetItemInfo(IL_INVENTORY,inv_index);
	PlayerGetItemInfo(IL_PET_EQUIP,pet_equip_index);
	_petman.OnEquipPetEquip(this,pet_equip_index);
}

int gplayer_imp::PlayerSummonPet(size_t pet_index)
{
	if(GetWorldManager()->GetWorldLimit().nopetsummon)
	{
		return S2C::ERR_PET_CAN_NOT_BE_SUMMONED;
	}
	A3DVECTOR pos;
	int rst = _petman.TestCanSummonPet(this,pet_index,pos);
	if(0 != rst)
	{
		return rst;
	}
	if(_filters.IsFilterExist(FILTER_SPIRITDRAG) || _filters.IsFilterExist(FILTER_PULL))
	{
		return 9999;
	}	

	if(_petman.IsRebornPet(this, pet_index))
	{
		SummonPet(pet_index);
	}
	else
	{
	//基本校验通过，开启一个session 
	session_summon_pet* pSession = new session_summon_pet(this);
	pSession->SetTarget(pet_index,0);
	pSession->SetDelay(SUMMON_PET_TIME);
	AddStartSession(pSession);
	}
	return 0;
}

int gplayer_imp::PlayerRecallPet(size_t pet_index)
{
	if(GetWorldManager()->GetWorldLimit().nopetsummon)
	{
		return S2C::ERR_PET_CAN_NOT_BE_RECALLED;
	}
	int rst = _petman.TestCanRecallPet(this,pet_index);
	if(0 != rst)
	{
		return rst;
	}
	if(_filters.IsFilterExist(FILTER_SPIRITDRAG) || _filters.IsFilterExist(FILTER_PULL))
	{
		return 9999;
	}	

	if(_petman.IsRebornPet(this, pet_index))
	{
		RecallPet(pet_index);
	}
	else
	{
		//基本校验通过，开启一个session 
		session_recall_pet * pSession = new session_recall_pet(this);
		pSession->SetTarget(pet_index,0);
		pSession->SetDelay(RECALL_PET_TIME);
		AddStartSession(pSession);
	}
	return 0;
}

int gplayer_imp::PlayerCombinePet(size_t pet_index,int type)
{
	if(GetWorldManager()->GetWorldLimit().nopetcombine)
	{
		return S2C::ERR_PET_CAN_NOT_BE_COMBINED;
	}
	int rst = _petman.TestCanCombinePet(this,pet_index,type,true);
	if(0 != rst)
	{
		return rst;
	}

	if(_petman.IsRebornPet(this, pet_index))
	{
		CombinePet(pet_index,type);
	}
	else
	{
		//基本校验通过，开启一个session 
		session_combine_pet * pSession = new session_combine_pet(this,type);
		pSession->SetTarget(pet_index,0);
		pSession->SetDelay(COMBINE_PET_TIME);
		AddStartSession(pSession);
	}
	return 0;
}

int gplayer_imp::PlayerUncombinePet(size_t pet_index)
{
	if(GetWorldManager()->GetWorldLimit().nopetcombine)
	{
		return S2C::ERR_PET_CAN_NOT_BE_UNCOMBINED;
	}
	int rst = _petman.TestCanUncombinePet(this,pet_index);
	if(0 != rst)
	{
		return rst;
	}
	if(_filters.IsFilterExist(FILTER_SPIRITDRAG) || _filters.IsFilterExist(FILTER_PULL))
	{
		return 9999;
	}	
	//基本校验通过，开启一个session 
	session_uncombine_pet * pSession = new session_uncombine_pet(this,_petman.GetPetCombineType(this,pet_index));
	pSession->SetTarget(pet_index,0);
	pSession->SetDelay(UNCOMBINE_PET_TIME);
	AddStartSession(pSession);
	return 0;
}

int gplayer_imp::PlayerSetPetStatus(size_t pet_index,int pet_tid,int main_status,int sub_status)
{
	int rst = _petman.SetPetStatus(this,pet_index,pet_tid,main_status,sub_status);
	if(0 != rst)
	{
		return rst;
	}
	return 0;
}

int gplayer_imp::PlayerSetPetRank(size_t pet_index,int pet_tid,int rank)
{
	int rst = _petman.SetPetRank(this,pet_index,pet_tid,rank);
	if(0 != rst)
	{
		return rst;
	}
	return 0;
}

void gplayer_imp::PlayerSendPetCommand(size_t pet_index,int cur_target,int pet_cmd,const void* buf,size_t size)
{
	_petman.PlayerPetCtrl(this,pet_index,cur_target,pet_cmd,buf,size);
}

int gplayer_imp::PlayerFeedPet(size_t pet_index,item *parent,int pet_level_min,int pet_level_max,
                unsigned int pet_type_mask,int food_usage,unsigned int food_type,int food_value,int& cur_value)
{
	return _petman.FeedPet(this,pet_index,parent,pet_level_min,pet_level_max,pet_type_mask,food_usage,food_type,food_value,cur_value);
}

int gplayer_imp::PlayerAutoFeedPet(size_t pet_index,item *parent,float hp_gen,float mp_gen,float hunger_gen,int mask,int& cur_value)
{
	return _petman.AutoFeedPet(this,pet_index,parent,hp_gen,mp_gen,hunger_gen,mask,cur_value);
}

int gplayer_imp::SummonPet(size_t pet_index)
{
	return _petman.SummonPet(this,pet_index);
}

int gplayer_imp::RecallPet(size_t pet_index)
{
	return _petman.RecallPet(this,pet_index);
}

int gplayer_imp::CombinePet(size_t pet_index,int type)
{
	return _petman.CombinePet(this,pet_index,type, true);
}

int gplayer_imp::UncombinePet(size_t pet_index,int type)
{
	gplayer * pPlayer = GetParent();
	if(pPlayer->IsInvisible()) _filters.RemoveFilter(FILTER_INDEX_INVISIBLE);
	return _petman.UncombinePet(this,pet_index,type);
}

void gplayer_imp::ClearPetSkillCoolDown()
{
	_petman.ClearPetSkillCoolDown(this);
}

void gplayer_imp::PlayerGetSummonPetProp()
{
	int cs_index = ((gplayer*)_parent)->cs_index;
	int uid = ((gplayer*)_parent)->ID.id;
	int sid = ((gplayer*)_parent)->cs_sid;

	_petman.GetSummonPetProp(this, cs_index, uid, sid);
}

void 
gplayer_imp::StartRecorder()
{
	_recorder_timestamp = abase::Rand(1,100000000);
	_runner->player_recorder_start(_recorder_timestamp);
}

void 
gplayer_imp::QueryRecorder(size_t record_item)
{
	if(record_item >= _inventory.Size()) return;
	if(!CheckCoolDown(COOLDOWN_INDEX_RECORDER_CHECK)) return;
	SetCoolDown(COOLDOWN_INDEX_RECORDER_CHECK, RECORDER_CHECK_COOLDOWN_TIME);
	
	const item & it =_inventory[record_item];
	if(it.type == -1 || it.GetItemType() != item_body::ITEM_TYPE_RECORDER)
	{
		//物品位置不正确或者什么什么的 进行整个包裹的搜寻 
		if(_inventory.FindByType(0, item_body::ITEM_TYPE_RECORDER) < 0)
		{
			return ;
		}
	}

	MD5_CTX	context;
	MD5_Init(&context);
	MD5_Update(&context,&(_parent->ID.id), sizeof(int));
	MD5_Update(&context, &(_recorder_timestamp), sizeof(int));
	for(size_t i = 0 ;i < _inventory.Size(); i ++)
	{
		const item & it1 =_inventory[i];
		MD5_Update(&context, &(it1.type), sizeof(int));
	}
	unsigned char digest[16];
	MD5_Final(digest, &context);

	_recorder_timestamp ++;
	_runner->player_recorder_check((const char*) digest, 16);
}


void gplayer_imp::FactionTradeTerminateSave()
{
	faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
	if(pTrade)
	{
		user_save_data((gplayer*)_parent,NULL,2,pTrade->_put_mask);
	}
	else
	{
		ASSERT(false);
	}
}

bool gplayer_imp::TradeObjectHeartbeat() 
{ 
	return _trade_obj->Heartbeat(this); 
}
void gplayer_imp::ReplyTradeRequest(bool v) 
{
	GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),_parent->ID.id,((gplayer*)_parent)->cs_sid,v);
}

void gplayer_imp::DiscardTrade() 
{
	GMSV::DiscardTrade(_trade_obj->GetTradeID(),_parent->ID.id);
	//回到等待交易完成的状态
	_pstate.WaitTradeComplete();
	_trade_obj->SetTimeOut(10);
}

bool gplayer_imp::ConsignObjectHeartbeat()
{
	return _consign_obj->Heartbeat(this);
}


bool gplayer_imp::DisconnectHeartbeat()
{
	ASSERT(_disconnect_timeout >= 0 && _disconnect_timeout < 1000);
	if(--_disconnect_timeout <= 0)
	{
		SetLogoutDisconnect();
		GLog::log(GLOG_INFO,"用户%d连接断开，开始执行退出逻辑", _parent->ID.id);
		Logout(GMSV::PLAYER_LOGOUT_FULL);
		return false;
	}
	return true;
}

void
gplayer_imp::ChangePlayerGameServer(int target_gs, int world_tag, const A3DVECTOR & target_pos, int reason)
{
	if(!_pstate.CanLogout()) 
	{
		GMSV::SendChangeGsRe( GNET::ERR_CHGS_STATUSINVALID, GetParent()->ID.id, GetParent()->cs_sid, target_gs, world_tag, target_pos.x, target_pos.y, target_pos.z, reason);
		return ;
	}

	//reason(0: 普通切换gs  1: 副本投票踢出玩家 2: 进入碰撞副本)
	//1和2都不需要检查战斗状态
	if(reason == 0 && IsCombatState())
	{
		GMSV::SendChangeGsRe( GNET::ERR_CHGS_STATUSINVALID, GetParent()->ID.id, GetParent()->cs_sid, target_gs, world_tag, target_pos.x, target_pos.y, target_pos.z, reason);
		return ;
	}
	
	LeaveAbnormalState();

	GLog::log(GLOG_INFO,"用户%d执行换线登出逻辑,目标%d线 %d号地图(%f,%f,%f)"
			,_parent->ID.id, target_gs,world_tag, target_pos.x,target_pos.y, target_pos.z);
	if(_offline_type != 0)
	{
		GLog::log(GLOG_INFO,"用户%d执行换线登出逻辑时状态不对, offline_type=%d" ,_parent->ID.id, _offline_type); 
		return;
	}
	
	_team.PlayerLogout();
	CircleOfDoomPlayerLogout();
	class UserInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		gplayer * _player;
		int _userid;
		int _target_gs;
		int _tag;
		A3DVECTOR _pos;
		int reason;

		public:
		UserInfoWriteBack(gplayer * pPlayer, int target_gs, int tag, const A3DVECTOR & pos, int r)
			:_player(pPlayer),_userid(pPlayer->ID.id), _target_gs(target_gs), _tag(tag), _pos(pos), reason(r)
		{}
		virtual void OnTimeOut()
		{
			//目前并没有重新发送存盘请求
			GLog::log(GLOG_ERR, "换线时保存%d数据超时", _userid );
			OnPutRole(1);
		}
		virtual void OnFailed()
		{	
			//不会受到这个命令 
			GLog::log(GLOG_ERR, "换线时保存%d数据失败", _userid );
			OnPutRole(2);
		}
		virtual void OnPutRole(int retcode)
		{
			_player->Lock();
			if(_player->ID.id !=  _userid || _player->login_state != gplayer::WAITING_LOGOUT) 
			{
				//忽略错误，直接返回
				_player->Unlock();
				GLog::log(GLOG_INFO,"写入用户%d发生状态不一致错误 id2:%d state:%d",_userid,_player->ID.id,_player->login_state);
				delete this;
				return ;
			}
			ASSERT(_player->imp);
			GLog::log(GLOG_INFO,"写入用户%d数据完成(%d)，发出换线请求",_userid,retcode);
			//将player对象删除
			GMSV::SendChangeGsRe( retcode?GNET::ERR_CHGS_DB_ERROR:GNET::ERR_CHGS_SUCCESS, 
					_player->ID.id, _player->cs_sid,_target_gs,_tag, _pos.x, _pos.y,_pos.z, reason);
			_player->imp->_commander->Release(); 
			_player->Unlock();
			record_leave_server(_userid);
			delete this;
		}

	};

	PlayerLeaveWorld();
	_runner->leave_world();

	//还要考虑断线逻辑
	//用户进入断线逻辑，并开始存盘
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->login_state = gplayer::WAITING_LOGOUT;

	ClearSession();
	ClearSpiritSession();

	//先将用户从世界中删除 在commander->Release中会见用户从管理器中移出
	slice * pPiece = pPlayer->pPiece;
	if(pPiece) _plane->RemovePlayer(pPlayer);
	pPlayer->pPiece = NULL;
	try
	{
		//写入磁盘 这里并不保存将来的存盘坐标，这里保存这个坐标是为了能够返回给delivery
		_trashbox.IncChangeCounter();
		UserInfoWriteBack * pCallback = new UserInfoWriteBack(pPlayer, target_gs, world_tag,target_pos,reason);
		user_save_data((gplayer*)_parent,pCallback, 2, DBMASK_PUT_ALL);
	}
	catch(...)
	{
		ASSERT(false);
	}
}


bool gplayer_imp::UseItem(item_list & inv, int inv_index, int where, int item_type)
{
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
			//检查是否需要记录消费值
			CheckUseItemConsumption(item_type, count);
		}

		
		return true;
	}
	return false;
}

bool gplayer_imp::UseItemWithTarget(item_list & inv, int inv_index, int where, int item_type,const XID& target, char force_attack )
{
	item & it = inv[inv_index];
	bool bRst = it.IsBroadcastUseMsg();
	int count = 0;
	bool dolog = it.NeedUseLog();
	int guid1 = it.guid.guid1;
	int guid2 = it.guid.guid2;
	int rst = inv.UseItemWithTarget(inv_index,this,target, force_attack,count);
	if(rst >= 0)
	{
		ASSERT(rst == item_type);
		_runner->use_item(where,inv_index, item_type,count);
		if(bRst) _runner->use_item(item_type);
		if(count > 0)
		{
			UseItemLog(item_type, guid1,guid2, count);
			if(dolog) GLog::log(GLOG_INFO,"用户%d使用%d个物品%d", _parent->ID.id, count, item_type);
		}
		return true;
	}
	return false;
}

bool 
gplayer_imp::UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size)
{
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
			//检查是否需要记录消费值
			CheckUseItemConsumption(item_type, count);
		}
		
		return true;
	}
	return false;
}

size_t gplayer_imp::OI_GetMallOrdersCount()
{
	return _mallinfo.GetOrderCount();
}

int gplayer_imp::OI_GetMallOrders(GDB::shoplog * list, size_t size)
{
	return _mallinfo.GetOrder(list,size);
}


void
gplayer_imp::OI_ToggleSanctuary(bool active)
{
	_sanctuary_mode = active;
}

bool gplayer_imp::TalismanGainExp(int exp, bool is_aircraft)
{
	int index = is_aircraft?item::EQUIP_INDEX_WING:item::EQUIP_INDEX_TALISMAN1;
	item & it = _equipment[index];
	if(it.type == -1 || it.body == NULL) return false;
	bool level_up = false;
	int rst = it.body->GainExp(item::BODY, exp, &it,this, index, level_up);
	if(rst >= 0) 
	{
		//汇报新经验
		_runner->talisman_exp_notify(IL_EQUIPMENT,index, rst);
		if(level_up)
		{
			property_policy::UpdatePlayer(GetPlayerClass(),this);
			PlayerGetItemInfo(IL_EQUIPMENT, index);
			_runner->self_get_property(_basic.status_point,_cur_prop);
			if(!is_aircraft && _talisman_info.bot_state)
			{
				DeactiveTalismanAutobotAddon();
				if( _talisman_info.autobot_arg.addon_exist_flag )
				{
					ActiveTalismanAutobotAddon();
				}
			}

		}
		return true;
	}
	else
	{
		return false;
	}
}

bool
gplayer_imp::RefineTalisman(size_t index, int item_type, int material_id)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size || material_id <= 0) return false;
	
	//检查物品是否存在
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;

	//检查血炼材料是否存在
	int material_index = _inventory.Find(0,material_id);
	if(material_index < 0) return false;
	int exp_inc = 0;
	int need_money = 0;
	{
		//查询需要的金钱数量
		DATA_TYPE dt2;
		const TALISMAN_MAINPART_ESSENCE &ess1= *(const TALISMAN_MAINPART_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_type, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_TALISMAN_MAINPART_ESSENCE || &ess1 == NULL)
		{
			return false;
		}
		need_money = ess1.fee_expfood;

		//查询血炼增加经验值
		const TALISMAN_EXPFOOD_ESSENCE &ess2= *(const TALISMAN_EXPFOOD_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(material_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_TALISMAN_EXPFOOD_ESSENCE || &ess2 == NULL)
		{
			return false;
		}

		exp_inc = ess2.exp_added;

		int talisman_mask = ess1.is_aircraft?2:1;
		int food_mask = ess2.food_type + 1; 	//0 1 2 --> 1 2 3 成为 mask
		if (!(food_mask & talisman_mask)) return false;	//食物要求不匹配
	}

	if(exp_inc <= 0) return false;
	if(GetMoney() < (size_t)need_money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	
	bool  level_up = false; 
	int rst = it.body->GainExp(item::INVENTORY, exp_inc, &it,this, index, level_up);
	if(rst <0) return false;

	//这里可能会升级，所以索性发送更新数据
	//通知客户端 经验改变
	PlayerGetItemInfo(IL_INVENTORY,index);
//	_runner->talisman_exp_notify(IL_INVENTORY,index , rst);

	//检查是否需要记录消费值
	CheckSpecialConsumption(_inventory[material_index].type, 1);
	//删除材料
	UseItemLog(_inventory[material_index],1);
	_inventory.DecAmount(material_index, 1);
	_runner->player_drop_item(IL_INVENTORY,material_index,material_id, 1 ,S2C::DROP_TYPE_USE);
	
	//删除金钱
	if(need_money > 0)
	{	
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=11:hint=%d",GetParent()->ID.id,GetUserID(),need_money,it.type);
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}

	// 活跃度更新, 传承
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	GLog::log(LOG_INFO,"用户%d精炼聚气了法宝(%d,%d),使用物化天宝(%d)",_parent->ID.id,index, item_type,material_id);
	
	return true;
}

bool
gplayer_imp::TalismanLevelUp(size_t index, int item_type)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;

	int need_money = 0;
	{
		//查询需要的金钱数量
		DATA_TYPE dt2;
		const TALISMAN_MAINPART_ESSENCE &ess1= *(const TALISMAN_MAINPART_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_type, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_TALISMAN_MAINPART_ESSENCE || &ess1 == NULL)
		{
			return false;
		}
		need_money = ess1.fee_levelup;
	}

	if(GetMoney() < (size_t)need_money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	
	bool rst = it.body->LevelUp(&it,this);
	if(!rst) return false;
	//更新客户端数据
	PlayerGetItemInfo(IL_INVENTORY,index);

	//考虑是否做广播通知周围所有人 $$$$$$$$$$$

	//删除金钱
	if(need_money > 0)
	{	
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}

	return true;
}

bool
gplayer_imp::TalismanReset(size_t index, int item_type)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;
	if(it.IsLocked()) return false;

	int need_money = 0;
	{
		//查询需要的金钱数量
		DATA_TYPE dt2;
		const TALISMAN_MAINPART_ESSENCE &ess1= *(const TALISMAN_MAINPART_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_type, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_TALISMAN_MAINPART_ESSENCE || &ess1 == NULL)
		{
			return false;
		}
		need_money = ess1.fee_reset;
	}

	int material = g_config.talisman_reset_item;
	if(material <=0) return false;
	int material_index = _inventory.Find(0, material);
	if(material_index < 0) return false;

	if(GetMoney() < (size_t)need_money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	
	bool rst = it.body->ItemReset(&it,this);
	if(!rst) return false;
	//更新客户端数据
	PlayerGetItemInfo(IL_INVENTORY,index);

	//检查是否需要记录消费值
	CheckSpecialConsumption(_inventory[material_index].type, 1);

	UseItemLog(_inventory[material_index],1);
	_inventory.DecAmount(material_index, 1);
	_runner->player_drop_item(IL_INVENTORY,material_index,material, 1 ,S2C::DROP_TYPE_USE);
	

	//删除金钱
	if(need_money > 0)
	{	
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=11:hint=%d",GetParent()->ID.id,GetUserID(),need_money,item_type);
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}

	// 活跃度更新, 法宝归元
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	
	GLog::log(LOG_INFO,"用户%d精炼归元了法宝(%d,%d),使用归元石(%d)",_parent->ID.id,index, item_type,material);
	return true;
}

bool gplayer_imp::TalismanEnchant( size_t index1, int item_type1, size_t index2, int item_type2 )
{
	// 检查物品是否存在
	if( !IsItemExist( index1, item_type1, 1 ) || !IsItemExist( index2, item_type2, 1 ) ) return false;
	// 检查物品是否为法宝
	item & it1 = _inventory[index1];
	if( !it1.body ) return false;
	if( it1.GetItemType() != item_body::ITEM_TYPE_TALISMAN ) return false;
//	if(it1.IsLocked()) return false;

	// 检查物品是否为法宝熔炼剂
	DATA_TYPE dt;
	const TALISMAN_MERGEKATALYST_ESSENCE &tme = 
		*(const TALISMAN_MERGEKATALYST_ESSENCE*)gmatrix::GetDataMan().get_data_ptr( item_type2, ID_SPACE_ESSENCE, dt );
	if( !&tme ) return false;
	if( DT_TALISMAN_MERGEKATALYST_ESSENCE != dt ) return false;
	size_t need_money = g_config.talisman_enchant_fee;
	if( GetMoney() < need_money )
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	if( !talisman_item::Enchant( this, index1, index2 ) )
	{
		return false;
	}
	if( need_money )
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=11:hint=%d",GetParent()->ID.id,GetUserID(),need_money,it1.type);
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}

	// 活跃度更新, 法宝灌魔
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	return true;
}

bool 
gplayer_imp::TalismanCombine(size_t index1, int item_type1, size_t index2, size_t item_type2, int catalyst_id)
{
	//检查物品是否存在
	if(index1 == index2) return false;
	if(!IsItemExist(index1,item_type1,1) || !IsItemExist(index2,item_type2,1)) return false;
	int catalyst_index = -1;
	if(catalyst_id > 0)
	{
		catalyst_index = _inventory.Find(0,catalyst_id);
		if(catalyst_index < 0) return false;
	}
	if(catalyst_index < 0) catalyst_id = 0;

	//检查物品是否为法宝
	item & it1 = _inventory[index1];
	item & it2 = _inventory[index2];
	if(!it1.body || !it2.body) return false;
	if(it1.GetItemType() != item_body::ITEM_TYPE_TALISMAN || it2.GetItemType() != item_body::ITEM_TYPE_TALISMAN)
	{
		return false;
	}

	if(it1.expire_date > 0 || it2.expire_date > 0)
	{
		return false;
	}
	
//	if(it1.IsLocked() || it2.IsLocked()) return false;
	size_t need_money = g_config.talisman_combine_fee;
	if(GetMoney() < need_money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}

	int item1_id = it1.type;
	int item2_id = it2.type;
	if(!talisman_item::Combine(this, index1,index2, catalyst_id ))
	{
		return false;
	}
	if(catalyst_index >= 0)
	{
		UseItemLog(_inventory[catalyst_index],1);
		_inventory.DecAmount(catalyst_index, 1);
		_runner->player_drop_item(IL_INVENTORY,catalyst_index,catalyst_id, 1 ,S2C::DROP_TYPE_USE);
	}
	if(need_money)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=11:hint=%d,%d",GetParent()->ID.id,GetUserID(),need_money,item1_id,item2_id);
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}
	_runner->talisman_combine_value(GetTalismanValue());
	object_interface oif(this);
	GetAchievementManager().OnRefineTali(oif, GetTalismanValue());
	
	// 活跃度更新, 法宝融合
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	return true;
}

bool gplayer_imp::TalismanHolyLevelup(size_t talisman_index, int talisman_id, int levelup_id)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(talisman_index >= inv_size || levelup_id <= 0) return false;
	
	//检查物品是否存在
	item & it = _inventory[talisman_index];
	if(it.type ==-1 || it.body == NULL || talisman_id != it.type )  return false;
	if(it.IsLocked()) return false;

	//检查血炼材料是否存在
	int levelup_index = _inventory.Find(0, levelup_id);
	if(levelup_index < 0) return false;

	if(!talisman_item::HolyLevelup(this, talisman_index, levelup_id))
	{
		return false;
	}

	//这里可能会升级，所以索性发送更新数据
	//通知客户端 经验改变
	PlayerGetItemInfo(IL_INVENTORY,talisman_index);

	//检查是否需要记录消费值
	CheckSpecialConsumption(_inventory[levelup_index].type, 1);

	//删除材料
	UseItemLog(_inventory[levelup_index],1);
	_inventory.DecAmount(levelup_index, 1);
	_runner->player_drop_item(IL_INVENTORY,levelup_index, levelup_id,1,S2C::DROP_TYPE_USE);

	// 活跃度更新, 法宝飞升
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	
	return true;
}

bool gplayer_imp::TalismanEmbedSkill(size_t talisman1_index, int talisman1_id, size_t talisman2_index, int talisman2_id, int needitem1_id, int needitem1_index, int needitem2_id, int needitem2_index)
{
	//检查物品是否存在
	if(talisman1_index == talisman2_index) return false;
	if(!IsItemExist(talisman1_index, talisman1_id, 1) || !IsItemExist(talisman2_index,talisman2_id,1)) return false;

	if(needitem1_id < 0 || needitem1_index < 0 || !IsItemExist(needitem1_index, needitem1_id, 1))
	{
		return false;
	}

	if(needitem2_id < 0 || needitem2_index < 0 || !IsItemExist(needitem2_index, needitem2_id, 1))
	{
		return false;
	}
	if(needitem1_index == needitem2_index) return false;

	//检查物品是否为法宝
	item & it1 = _inventory[talisman1_index];
	item & it2 = _inventory[talisman2_index];
	item & it3 = _inventory[needitem1_index];
	item & it4 = _inventory[needitem2_index];
	if(!it1.body || !it2.body) return false;
	if(it1.GetItemType() != item_body::ITEM_TYPE_TALISMAN || it2.GetItemType() != item_body::ITEM_TYPE_TALISMAN)
	{
		return false;
	}

	if(it1.expire_date > 0 || it2.expire_date > 0)
	{
		return false;
	}
	if(it1.IsLocked() || it2.IsLocked() || it3.IsLocked() || it4.IsLocked()) return false;

	if(!talisman_item::EmbedSkill(this,talisman1_index, talisman2_index, needitem1_id, needitem2_id))
	{
		return false;
	}
	if(needitem1_index >= 0)
	{
		UseItemLog(_inventory[needitem1_index],1);
		_inventory.DecAmount(needitem1_index, 1);
		_runner->player_drop_item(IL_INVENTORY,needitem1_index,needitem1_id, 1 ,S2C::DROP_TYPE_USE);
	}
	if(needitem2_index >= 0)
	{
		UseItemLog(_inventory[needitem2_index],1);
		_inventory.DecAmount(needitem2_index, 1);
		_runner->player_drop_item(IL_INVENTORY,needitem2_index,needitem2_id, 1 ,S2C::DROP_TYPE_USE);
	}

	// 活跃度更新, 法宝 传承 or 融合
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	return true;
}

bool gplayer_imp::TalismanSkillRefine(size_t talisman1_index, int talisman1_id, size_t talisman2_index, int talisman2_id, int needitem_id)
{
	//检查物品是否存在
	if(talisman1_index == talisman2_index) return false;
	if(!IsItemExist(talisman1_index, talisman1_id, 1) || !IsItemExist(talisman2_index,talisman2_id,1)) return false;
	int needitem_index = -1;
	if(needitem_id > 0)
	{
		needitem_index = _inventory.Find(0, needitem_id);
		if(needitem_index < 0) return false;
	}
	if(needitem_index < 0) needitem_id = 0;

	//检查物品是否为法宝
	item & it1 = _inventory[talisman1_index];
	item & it2 = _inventory[talisman2_index];
	if(!it1.body || !it2.body) return false;
	if(it1.GetItemType() != item_body::ITEM_TYPE_TALISMAN || it2.GetItemType() != item_body::ITEM_TYPE_TALISMAN)
	{
		return false;
	}

	if(it1.expire_date > 0 || it2.expire_date > 0)
	{
		return false;
	}
	if(it1.IsLocked() || it2.IsLocked()) return false;


	if(!talisman_item::SkillRefine(this,talisman1_index, talisman2_index, needitem_id))
	{
		return false;
	}
	if(needitem_index >= 0)
	{
		UseItemLog(_inventory[needitem_index],1);
		_inventory.DecAmount(needitem_index, 1);
		_runner->player_drop_item(IL_INVENTORY,needitem_index,needitem_id, 1 ,S2C::DROP_TYPE_USE);
	}

	// 活跃度更新, 法宝 传承 or 融合
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FABAO);
	return true;
}

bool gplayer_imp::TalismanSkillRefineResult(size_t talisman_index, int talisman_id, char result)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(talisman_index >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[talisman_index];
	if(it.type ==-1 || it.body == NULL || talisman_id != it.type )  return false;

	if(!talisman_item::SkillRefineResult(this, talisman_index, result))
	{
		return false;
	}

	//这里可能会升级，所以索性发送更新数据
	//通知客户端 经验改变
	PlayerGetItemInfo(IL_INVENTORY,talisman_index);
	return true;
}

void gplayer_imp::SetFamilySkill(const int * list, int size)
{
	_skill.SetFamilySkill((int*)list,size,this);
}

void 
gplayer_imp::OI_TalismanGainExp(int exp, bool is_aircraft)
{
	TalismanGainExp(exp, is_aircraft);
}

void 
gplayer_imp::PlayerTalismanBotBegin(bool active, bool need_question )
{
	if(active)
	{
		
		if(_talisman_info.talisman_state && !_talisman_info.bot_state
				&& (_talisman_info.stamina > _talisman_info.max_stamina * 0.5f || IsNewbie()))
		{
			if(_cheat_punish)
			{
				_runner->error_message(S2C::ERR_COMMAND_IN_SEALED);
				return;
			}
			StartTalismanBot( need_question );

			// 活跃度更新，16. 天人合一
			EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_TRHY);
		}
	}
	else
	{
		if(_talisman_info.talisman_state && _talisman_info.bot_state)
		{
			
			StopTalismanBot();
		}
	}

}

void 
gplayer_imp::PlayerTalismanBotContinue()
{
	if(_talisman_info.bot_state) return;
	int t = g_timer.get_systime() - _talisman_info.stop_bot_time;
	if(t > abase::Rand(30,40))
	{
		GLog::log(LOG_INFO,"用户%d非正常情况下使用天人合一被断线, t=%d, stop_bot_time=%d", _parent->ID.id, t, _talisman_info.stop_bot_time);
		//断线之 
		LostConnection();
	}
}

void    
gplayer_imp::FindCheater(int type, bool noreport)
{       
	_cooldown.SetCoolDown(COOLDOWN_INDEX_CHEATER,3600*1000);
	if(_cheat_mode) return ;        //已经进入作弊状态就不在计算了

	_cheat_mode = abase::Rand(81, 120);
	A3DVECTOR pos = _parent->pos;
	__PRINTF("发现%d作弊 %d(%f,%f,%f)\n", _parent->ID.id, type, pos.x,pos.y,pos.z);
	if(!noreport)
	{               
		GMSV::ReportCheater2Gacd(_parent->ID.id, type, NULL,0);
	}               
	GLog::log(LOG_INFO,"用户%d发现使用作弊工具%d当前坐标(%f,%f,%f)%d秒后进入惩罚状态", _parent->ID.id, type, pos.x, pos.y, pos.z, _cheat_mode);
}

void
gplayer_imp::FindCheater2()
{
	if(_cheat_report) return;
	_cheat_report = 1;
	GMSV::ReportCheater2Gacd(_parent->ID.id, 1, NULL,0);

	A3DVECTOR pos = _parent->pos;
	GLog::log(LOG_INFO,"用户%d发现使用脱机外挂，当前坐标(%f,%f,%f)，已汇报至反外挂服务器", _parent->ID.id,pos.x, pos.y, pos.z);
}


void
gplayer_imp::PunishCheater()
{
	//惩罚作弊者 
	_cheat_punish = 1;
	__PRINTF("%d作弊惩罚开始\n", _parent->ID.id);
}

void
gplayer_imp::TaskSendMessage(int task_id, int channel, int param, long var[3])
{
	struct
	{
		int self_id;
		int task_id;
		int channel;
		int param;
		int lineno;
		char name[MAX_USERNAME_LENGTH];
		long var[3];
		char factionname[MAX_MAFIA_NAME_LENGTH];
	}data;
	memset(&data, 0,sizeof(data));
	data.self_id = _parent->ID.id;
	data.task_id = task_id;
	data.channel = channel;
	data.param = param;
	data.lineno = gmatrix::GetServerIndex();
	size_t  len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.name, _username,len);
	if(NULL != var)
	{
		memcpy(data.var, var, sizeof(long) * 3);
	}
	len = _mafia_name_len;
	if (len > MAX_MAFIA_NAME_LENGTH) len = MAX_MAFIA_NAME_LENGTH;
	memcpy(data.factionname,_mafia_name, len);

	switch(channel)
	{
		case GMSV::CHAT_CHANNEL_TRADE:
		{
			slice * pPiece = GetParent()->pPiece;
			if(pPiece && _plane)
			{
				RegionChatMsg(_plane,pPiece,&data,sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, 0,0,0,TASK_CHAT_MESSAGE_ID);
			}
			return ;
		}

		case GMSV::CHAT_CHANNEL_FARCRY:
		case GMSV::CHAT_CHANNEL_BROADCAST:
		case GMSV::CHAT_CHANNEL_SPEAKER:
		case GMSV::CHAT_CHANNEL_RUMOR:
			broadcast_chat_msg(TASK_CHAT_MESSAGE_ID,&data,sizeof(data),GMSV::CHAT_CHANNEL_SYSTEM,0,0,0);
			return;

		case GMSV::CHAT_CHANNEL_TEAM:
			SendTeamChat(GMSV::CHAT_CHANNEL_SYSTEM, &data, sizeof(data), 0, 0, TASK_CHAT_MESSAGE_ID);
			return;

		case GMSV::CHAT_CHANNEL_WISPER:
			return;
		case GMSV::CHAT_CHANNEL_LOCAL:
		case GMSV::CHAT_CHANNEL_FACTION: 
		{
			if(_plane->GetWorldManager()->IsBattleWorld())
			{
				_plane->GetWorldManager()->BattleFactionSay(GetFaction(), &data, sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0, 0, TASK_CHAT_MESSAGE_ID);
			}
		}
		return ; 
	}

	return ;
}       

void gplayer_imp::LotteryTySendMessage(int item_id)
{
	struct
	{
		int player_id;
		int lineno;
		char name[MAX_USERNAME_LENGTH];
		int item_id;
	}data;
	memset(&data, 0,sizeof(data));
	data.player_id= _parent->ID.id;
	data.lineno = gmatrix::GetServerIndex();
	size_t  len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.name, _username,len);
	data.item_id = item_id;
	broadcast_chat_msg(LOTTERY_TY_CHAT_MESSAGE_ID,&data,sizeof(data),GMSV::CHAT_CHANNEL_SYSTEM,0,0,0);
}

void
gplayer_imp::WallowControl(int level, int msg, int p_time, int l_time, int h_time)
{
	if(level < 0 || level >= anti_wallow::MAX_WALLOW_LEVEL - 1) return ;
	_wallow_level = level + 1;
	SetRefreshState();
	_runner->player_wallow_info(level, msg, p_time, l_time, h_time);
}

void gplayer_imp::OnAntiCheatAttack()
{
	// 天人合一状态下并且玩家要求不答题的情况下，不会出题
	if(_talisman_info.bot_state && !_talisman_info.need_question ) return;
	// 如果是战场地图则不参与答题
	if( _plane->GetWorldManager()->IsBattleWorld() ) return;
	if( _plane->GetWorldManager()->IsRaidWorld() ) return;
	if( gmatrix::IsBattleFieldServer()) return;
	if(abase::RandUniform() < 0.05f)
	{
		if(CheckCoolDown(COOLDOWN_INDEX_ANTI_CHEAT))
		{
			SetCoolDown(COOLDOWN_INDEX_ANTI_CHEAT,12*60*1000);
			GMSV::TriggerQuestion2Gacd(_parent->ID.id);
			__PRINTF("用户%d反外挂答题了\n",_parent->ID.id);
		}
	}
}

void gplayer_imp::QuestionBonus()
{
	const float  abc[] = {0.4,0.3,0.15,0.15+0.01};
	int index = abase::RandSelect(abc, 4);
	__PRINTF("用户%d反外挂答题答对了，发奖%d\n",_parent->ID.id,index);
	GLog::log(LOG_INFO,"用户%d反外挂答题正确,奖品%d", _parent->ID.id,index);
	if(index == 2)
	{
		PlayerTaskInterface TI(this);
		if(!TI.IsDeliverLegal() || TI.CanDeliverCommonItem(1))
		{
			//如果无法发物品，则给经验
			index = 1;
		}
	}
	_runner->anti_cheat_bonus(index);
	switch(index)
	{
		case 0:
			//血蓝补满
			FullHPAndMP();
			SetRefreshState();
			break;
		case 1:	
			//给固定经验
			{
				int level = _basic.level;
				float f1 = (0.02714*level*level*level - 2.7*level*level  + 101*level - 1121);
				float f2 = level * 5;
				float exp;
				if(f1 > f2) exp = f1; else exp = f2;
				ReceiveTaskExp((int)(exp * 15));
			}
			break;
		case 2:
			//一个炼器符
			{
				PlayerTaskInterface TI(this);
				if(TI.IsDeliverLegal() && TI.CanDeliverCommonItem(1))
				{
					TI.DeliverCommonItem(ID_LIANQIFU,1,false,0,0);
				}
			}
			break;
		case 3:
			//15分钟双倍经验
			ActiveMultiExpTime(2, 5*60);
			break;
	}
}

bool gplayer_imp::ModifyTalentPoint(int offset)
{
	if(_talent_point + offset < 0) return false;
	_talent_point += offset;
	_runner->player_talent_notify(_talent_point);
	return true;
}

void gplayer_imp::InitSkillData(const void * data, size_t size)
{
	if(data && size > 0)
	{
		try
		{       
			raw_wrapper ar(data,size);
			_skill.LoadDatabase(object_interface(this),ar);
		}catch(...)
		{       
			GLog::log(GLOG_ERR,"用户%d技能装载出错",_parent->ID.id);
		}
	}
	_skill.EventReset(this);	//这个是激活被动技能，应在Forget之前调用
	
	if(!_skill.CheckConsistency())
	{
		/*
		if(_cooldown.TestCoolDown(COOLDOWN_SPEC_ERROR1))
		{
			_cooldown.SetCoolDown(COOLDOWN_SPEC_ERROR1,7*24*3600*1000);
			GLog::formatlog("formatlog:error:userid=%d:find_skill_point_error",_parent->ID.id);
		}
		*/
		GLog::log(GLOG_WARNING,"用户%d技能加点有误，全部清除", _parent->ID.id);
		_skill.Forget(true,this);
	}
	int status_point;
	_basic.status_point = status_point = player_template::GetSkillPoint(GetRebornCount(), _basic.level, _basic.dt_level);
	_basic.status_point -= _skill.GetSpSum();
	if(_basic.status_point < 0)
	{
		GLog::log(GLOG_WARNING,"用户%d技能加点有误，全部清除", _parent->ID.id);
		_skill.Forget(true,this);
		_basic.status_point = status_point;
	}
	
	_talent_point = player_template::GetTalentPoint(GetRebornCount(), _basic.level, _basic.dt_level);
	_talent_point -= _skill.GetTalentSum();
	if(_talent_point < 0)
	{	
		//天赋点数不正常，自动清除之
		//应当记录一个日志
		GLog::log(GLOG_WARNING,"用户%d天赋点数超过可用点数，自动清除",_parent->ID.id);
		_skill.ForgetTalent(this);
		_talent_point = player_template::GetTalentPoint(GetRebornCount(), _basic.level, _basic.dt_level);
	}
	if (!IsMarried())
	{
		//清除夫妻技能
		_skill.ClearSpouseSkill(this);
	}

}

void 
gplayer_imp::InitCombineSkillData(const void * data, size_t size)
{
	if(data && size > 0)
	{
		try
		{       
			raw_wrapper ar(data,size);
			_skill.LoadSkillElems(ar);
		}catch(...)
		{       
			GLog::log(GLOG_ERR,"用户%d组合技装载出错",_parent->ID.id);
		}
	}
}

bool    
gplayer_imp::OI_TestSafeLock() 
{       
	return ((gplayer_controller*)_commander)->InSafeLock();
}               


int gplayer_imp::PetServiceBaseCheck(size_t inv_index,size_t fee, bool check_lock)
{
	//检查是否越界
	if(inv_index >=  _inventory.Size()) return S2C::ERR_FATAL_ERR;
	//检查物品是否存在
	item& it = _inventory[inv_index];
	if(-1 == it.type || NULL == it.body) return S2C::ERR_FATAL_ERR;
	//检查操作的物品是不是宠物牌
	if(item_body::ITEM_TYPE_PET_BEDGE != ((item_pet_bedge*)(it.body))->GetItemType())
	{
		__PRINTF("操作的目标物品不是宠物牌,这里客户端应该判的\n" );
		return S2C::ERR_NEED_PET_BEDGE;
	}
	if(check_lock && it.IsLocked()) return S2C::ERR_PET_IS_LOCKED;
	if(GetMoney() < fee) return S2C::ERR_OUT_OF_FUND;
	return 0;
}

int gplayer_imp::PlayerFreePet(int inv_index)
{
	int rst = PetServiceBaseCheck(inv_index,(size_t)g_config.fee_free_pet);
	if(0 != rst)
	{
		return rst;
	}
	//基本校验通过，开启一个session 
	session_free_pet* pSession = new session_free_pet(this);
	pSession->SetTarget(inv_index);
	pSession->SetDelay(FREE_PET_TIME);
	AddStartSession(pSession);
	return 0;
}

int gplayer_imp::FreePet(int inv_index)
{
	int rst = PetServiceBaseCheck(inv_index,(size_t)g_config.fee_free_pet);
	if(0 != rst ) return rst;
	return ((item_pet_bedge*)(_inventory[inv_index].body))->FreePet(this,inv_index);
}

int gplayer_imp::PlayerAdoptPet(int inv_index)
{
	int rst = PetServiceBaseCheck(inv_index,(size_t)g_config.fee_adopt_pet);
	if(0 != rst)
	{
		return rst;
	}
	//基本校验通过，开启一个session 
	session_adopt_pet* pSession = new session_adopt_pet(this);
	pSession->SetTarget(inv_index);
	pSession->SetDelay(ADOPT_PET_TIME);
	AddStartSession(pSession);
	return 0;
}

int gplayer_imp::AdoptPet(int inv_index)
{
	int rst = PetServiceBaseCheck(inv_index,(size_t)g_config.fee_adopt_pet);
	if(0 != rst ) return rst;
	return ((item_pet_bedge*)(_inventory[inv_index].body))->AdoptPet(this,inv_index);
}

int gplayer_imp::PlayerRefinePet(int inv_index_pet,int inv_index_c,int inv_index_a)
{
	int rst = PetServiceBaseCheck(inv_index_pet,(size_t)g_config.fee_refine_pet,false);
	if(0 != rst)
	{
		return rst;
	}
	//基本校验通过，开启一个session 
	session_refine_pet* pSession = new session_refine_pet(this);
	pSession->SetTarget(inv_index_pet,inv_index_c,inv_index_a);
	pSession->SetDelay(REFINE_PET_TIME);
	AddStartSession(pSession);
	return 0;
}

int gplayer_imp::PlayerRefinePet2(int inv_index_pet,int type)
{
	int rst = PetServiceBaseCheck(inv_index_pet,(size_t)g_config.fee_refine_pet,false);
	if(0 != rst)
	{
		return rst;
	}
	//基本校验通过，开启一个session 
	session_refine_pet2* pSession = new session_refine_pet2(this);
	pSession->SetTarget(inv_index_pet,type);
	pSession->SetDelay(REFINE_PET_TIME);
	AddStartSession(pSession);
	return 0;
}

int gplayer_imp::RefinePet(int inv_index_pet,int inv_index_c,int inv_index_a)
{
	int rst = PetServiceBaseCheck(inv_index_pet,(size_t)g_config.fee_refine_pet,false);
	if(0 != rst ) return rst;
	return ((item_pet_bedge*)(_inventory[inv_index_pet].body))->RefinePet(this,inv_index_pet,inv_index_c,inv_index_a);
}

int gplayer_imp::RefinePet2(int inv_index_pet,int type)
{
	int rst = PetServiceBaseCheck(inv_index_pet,(size_t)g_config.fee_refine_pet,false);
	if(0 != rst ) return rst;
	return ((item_pet_bedge*)(_inventory[inv_index_pet].body))->RefinePet2(this,inv_index_pet,type);
}

int gplayer_imp::RenamePet(int inv_index,const char* buf,int size)
{
	int rst = PetServiceBaseCheck(inv_index,(size_t)g_config.fee_rename_pet, false);
	if(0 != rst ) return rst;
	return ((item_pet_bedge*)(_inventory[inv_index].body))->RenamePet(this,inv_index,buf,size);
}

bool 
gplayer_imp::CanMetempsychosis()
{
	if(GetRebornCount() >= MAX_REBORN_COUNT) return false;
	if(GetObjectLevel() < MIN_REBORN_LEVEL) return false;
	if(!_pstate.IsNormalState() && !_pstate.IsBindState()) return false;
	return true;
}

bool    
gplayer_imp::Metempsychosis( int new_prof)
{       
	if(!CanMetempsychosis()) return false;

	//清除所有BUFF DEBUFF
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH | filter::FILTER_MASK_BUFF | filter::FILTER_MASK_DEBUFF);

	//试图去除骑乘效果
	if(_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER))
	{
		_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
	}
	//取消原来转生的技能
	ActiveRebornBonus(false);

	//保存转生的等级等数据
	int value = ((GetObjectLevel() & 0xFFFF) << 16) | (GetPlayerClass() & 0xFFFF);
	_reborn_list.push_back(value);

	//洗点
	_skill.Forget(true, this);
	_skill.ForgetTalent(this);

	//清除任务物品栏
	_task_inventory.Clear();
	PlayerGetInventory(IL_TASK_INVENTORY);

        //修改对外数据
	GetParent()->level2 = GetRebornCount();

	GLog::formatlog("formatlog:reborn:userid=%d:origin_level=%d:reborn_count=%d:new_profession=%d",_parent->ID.id, GetObjectLevel(), _reborn_list.size(),new_prof);

	//修改级别经验
	_basic.level = 15;
	_basic.exp = 0;

	//设置技能点和天赋点
	_basic.status_point = player_template::GetSkillPoint(GetRebornCount(), _basic.level, _basic.dt_level);
	_talent_point = player_template::GetTalentPoint(GetRebornCount(), _basic.level, _basic.dt_level);

	std::string fmt = "用户%d进行了转生洗点操作，这是第%d次转生,依次为:";
	for(size_t i = 0; i < _reborn_list.size(); i ++)
	{
		char buf[64];
		snprintf(buf, sizeof(buf) - 1, "%d/%d", _reborn_list[i] & 0xFFFF, (_reborn_list[i] >> 16) & 0xFFFF);
		fmt += buf;
		if(i != _reborn_list.size() - 1) fmt+=",";
	}
	GLog::log(LOG_INFO,fmt.c_str(),_parent->ID.id, _reborn_list.size());

	//清除任务由任务系统完成

	//变成新手 同时也发广播了
	ChangeClass(new_prof);

	//激活新转生技能
	ActiveRebornBonus(true);
	UpdateRuneProp();

	_runner->query_info00();
	//发送自己新的数据给客户端
	_runner->self_get_property(_basic.status_point,_cur_prop);

	//发送剩余天赋点数给客户端
	_runner->player_talent_notify(_talent_point);
	_runner->player_reborn_info(&*(_reborn_list.begin()), _reborn_list.size());

	//在线倒计时奖励检查
	CheckPlayerOnlineAward(g_timer.get_systime());

	SyncPlayerMafiaInfo();
	return true;
}

void gplayer_imp::ActiveRebornBonus(bool active)
{
	const reborn_bonus_man & man = gmatrix::GetRebornBonusMan();
	int prof[4];
	prof[0] = GetRebornProf(0);
	prof[1] = GetRebornProf(1);
	prof[2] = GetRebornProf(2);
	prof[3] = GetRebornProf(3);
	const reborn_skill_bonus * pBonus = man.GetBonus(prof[0],prof[1],prof[2],prof[3]);
	if(!pBonus) return;
	for(size_t i = 0; i < 12; i ++)
	{
		if(pBonus->bonus_skill[i].skill <= 0) continue;
		if(active)
		{
			_skill.InsertSkill( pBonus->bonus_skill[i].skill, pBonus->bonus_skill[i].lvl , this);
		}
		else
		{
			_skill.RemoveSkill( pBonus->bonus_skill[i].skill, pBonus->bonus_skill[i].lvl , this);
		}
	}
}

int gplayer_imp::OI_RebornCount()
{
	return GetRebornCount();
}

int gplayer_imp::QueryRebornInfo(int level[3], int prof[4])
{
	//const reborn_bonus_man & man = gmatrix::GetRebornBonusMan();
	prof[0] = GetRebornProf(0);
	prof[1] = GetRebornProf(1);
	prof[2] = GetRebornProf(2);
	prof[3] = GetRebornProf(3);
	ASSERT(GetRebornCount() <= 3);

	memset(level, 0, sizeof(int)*3); 
	for(int i = 0; i < GetRebornCount(); i ++)
	{
		level[i] = GetRebornLvl(i);
	}

	return GetRebornCount();
}

bool
gplayer_imp::PhaseControl(const A3DVECTOR & target, float terrain_height, int mode, int use_time)
{
	return _ph_control.PhaseControl(this, target, terrain_height,mode, use_time);
}



/*原则1：	状态转换限制
	GROUND->其他 允许
	FALL->GROUND 允许
	JUMP->GROUND 允许
	FALL->JUMP 不允许
	JUMP->FALL 允许
	是否GROUND 使用碰撞来完成，而不是使用传入的状态
	*/

/*原则2：
	JUMP限制
	方向必须向上或几乎向上，Y值必须为正
	从第一个JUMP开始进行跳跃时间和高度累计
	跳跃累计时间和高度不能超过指定阈值（这个值根据游戏不同、根据地图不同，可能是一个地图的参数）
	*/

/*原则3：
	跌落限制
	方向必须向下或几乎向下
	从开始下落开始，进行下落速度统计，下落速度必须不断增大，不能小于1/2重力所造成的影响值
	*/

/*原则4：
	最后一次跌落到地面的时，由于传入的时间会偏大，各种判定可能需要重新处理
 	*/

bool 
phase_control::PhaseControl(gplayer_imp * pImp, const A3DVECTOR & target, float theight, int mode, int use_time)
{
	trace_manager & man = pImp->GetWorldManager()->GetTraceMan();
	if(!man.Valid()) return true;

	//确认玩家的目标点是否在空中
	bool is_ground = true;
	bool is_solid;
	float ratio;
	bool bRst = man.AABBTrace(target, A3DVECTOR(0,-10,0), A3DVECTOR(0.3,0.9,0.3), is_solid,ratio);
	if(bRst && is_solid) return false;	//目标点嵌入了建筑 直接返回即可

	if(target.y >  theight + 0.2f)	//不在地面上，根据碰撞进行是否悬空的判断
	{
		if(!bRst) 
		{
			is_ground = false;
		}
		else
		{
			is_ground = (ratio * 10.f < 0.2f);
		}
	}

	if(is_ground) 
	{
		state = STATE_GROUND;
		return true;
	}

	if(pImp->InFlying())
	{
		jump_distance = 0;
		jump_time = 0;
		state = STATE_FLY;
		return true;
	}

	A3DVECTOR offset = target;
	offset -= pImp->_parent->pos;
	mode = mode & 0x0F;
//	if(mode == C2S::MOVE_MODE_JUMP)
	if(offset.y > 0)
	{
		if(offset.y < 0) return false;
		if(state == STATE_GROUND) 
		{
			//刚开始跳跃
			state = STATE_JUMP;
			jump_distance = offset.y;
			jump_time = use_time;
		}
		else if(state == STATE_JUMP)
		{
			//接着跳跃
			float ndis  = jump_distance +  offset.y;
			int ntime = jump_time + use_time;

			if(ndis > 10.f) 
			{
				__PRINTF("跳跃距离超高%f", ndis);
				return false;
			}
			if(ntime > 2000) 
			{
				__PRINTF("跳跃时间超长%d", ntime);
				return false;
			}
			jump_distance = ndis;
			jump_time = ntime;
		}
		else
		{
			__PRINTF("跌落的时候往上跳\n");
			return false;
		}
		return true;
	}
	else
	{
		//这是跌落
		if(state != STATE_FALL)
		{
			//原来不是跌落
			//开始进行跌落控制
			state = STATE_FALL;
			drop_speed = 0;
		}
		else
		{
			float WORLD_GRAVITY_ACC = (9.8/2);
			float dis = - offset.y;
			if(dis < drop_speed * (use_time *0.001f))
			{
				__PRINTF("跌落速度太慢太慢\n");
				drop_speed = 0.f; //这样是不是有漏洞?? 不过实际时可能出现的
				return false;
			}
			drop_speed += WORLD_GRAVITY_ACC * (use_time *0.001f);
		}
	}
	return true;
}

void phase_control::Initialize(gplayer_imp * pImp)
{
	trace_manager & man = pImp->GetWorldManager()->GetTraceMan();
	if(!man.Valid()) 
	{
		state = STATE_GROUND;
		return;
	}
	
	A3DVECTOR pos(pImp->_parent->pos);
	float height = pImp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height + 0.2) 
	{
		state = STATE_GROUND;
		return;
	}

	bool is_solid;
	float ratio;
	bool bRst = man.AABBTrace(pos, A3DVECTOR(0,-10,0), A3DVECTOR(0.3,0.9,0.3), is_solid,ratio);
	if(bRst && is_solid) 
	{
		state = STATE_GROUND;
		return;
	}
	
	if(!bRst) 
	{
		state = STATE_FALL;
	}
	else
	{
		state = (ratio * 10.f < 0.2f)?STATE_GROUND:STATE_FALL;
	}
}




bool gplayer_imp::RenewMountItem(size_t inv_mount, size_t inv_material)
{
	if(inv_mount >= _inventory.Size() || inv_material >= _inventory.Size()) return false;
	item & it1 = _inventory[inv_mount];
	item & it2 = _inventory[inv_material];
	if(it1.type <= 0 || it2.type <= 0) return false;
	if(it2.type != g_config.renew_mount_material) return false; 
	if(it1.GetItemType() != item_body::ITEM_TYPE_MOUNT) return false;
	if(it1.IsLocked()) return false;

//重新生成物品	
	DATA_TYPE datatype;
	VEHICLE_ESSENCE & ess = *(VEHICLE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it1.type, ID_SPACE_ESSENCE, datatype);
	if(&ess == NULL || datatype != DT_VEHICLE_ESSENCE)	return false;
	int pData[128];
	int addon_count = abase::RandSelect(ess.addon_prop,sizeof(float), 5);
	pData[0] = addon_count;

	struct
	{
		int	id;
		float	prop;
	} addons[20];	//这里绝不会出现重复的属性
	memset(addons, 0, sizeof(addons));
	memcpy(addons, ess.addon, sizeof(addons));
	for(int i = 0; i < addon_count; i ++)
	{
		int index= abase::RandSelect(&(addons[0].prop),sizeof(addons[0]),20);
		int addon = 0;
		if(index >= 0 && index < 20) addon = addons[index].id;
		addons[index].id = 0;
		pData[i+1] = addon;
	}

	it1.SetContent(pData, (addon_count + 1) * sizeof(int));

//消耗材料
	int m_id = it2.type;
	UseItemLog(it2,1);
	_inventory.DecAmount(inv_material, 1);
	_runner->player_drop_item(IL_INVENTORY,inv_material,m_id, 1 ,S2C::DROP_TYPE_USE);

//通知客户端
	PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, inv_mount);
	return true;
}

void gplayer_imp::PlayerStartFly()
{
	//要加一个session
	class op : public session_general_operation::operation
	{
		public:
		op()
		{}

		virtual int GetID()
		{
			return S2C::GOP_FLY;
		}
		virtual bool NeedBoardcast()
		{
			return true;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(5);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
			pImp->_filters.AddFilter(new gather_interrupt_filter(pImp,_session_id,FILTER_INDEX_GATHER_SESSION));
		}

		virtual void PostEnd(gactive_imp * pImp)
		{
			pImp->_filters.RemoveFilter(FILTER_INDEX_GATHER_SESSION);
		}

		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			pImp->StartFly();
		}

		virtual bool OnAttacked(gactive_imp * obj)
		{	
			return true;
		}
	};
	AddSession(new session_general_operation(this, new op()));
	StartSession();
}

void gplayer_imp::StartFly()
{

	if(!IsAircraftAvailable()  || _bind_to_ground || InFlying() || GetWorldManager()->GetWorldLimit().nofly)
	{
		_runner->error_message(S2C::ERR_CANNOT_FLY);
		return ;
	}
	
	//检查是否可以起飞
	if(!_talisman_info.talisman_state || _talisman_info.stamina < _talisman_info.max_stamina - 1e-3)
	{
		_runner->error_message(S2C::ERR_CANNOT_FLY);
		return ;
	}

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_FLY_IN_COMBAT);
		return;
	}

	if(GetShape() != 0 && GetShape() != 8)
	{
		_runner->error_message(S2C::ERR_FLY_IN_SHAPING);
		return;
	}

	//更新活跃度，17. 驾驭飞剑
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FEIJIAN);

	_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
	_filters.AddFilter(new aircraft_filter(this,FILTER_INDEX_AIRCRAFT,_fly_info.speed + _en_point.flight_speed,_fly_info.stamina_cost, _fly_info.exp_add)); 
	NotifyMasterInfo();
}

void 
gplayer_imp::PlayerStopFly()
{	
	_filters.RemoveFilter(FILTER_INDEX_AIRCRAFT);
	NotifyMasterInfo();
}

void gplayer_imp::PreFly()
{
	_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
	_filters.RemoveFilter(FILTER_INDEX_AIRCRAFT);
}

bool gplayer_imp::CheckCanFly()
{
	if(_bind_to_ground || InFlying() || GetWorldManager()->GetWorldLimit().nofly)
	{
		_runner->error_message(S2C::ERR_CANNOT_FLY);
		return false;
	}
	
	if(GetShape() != 0 && GetShape() != 8)
	{
		_runner->error_message(S2C::ERR_FLY_IN_SHAPING);
		return false;
	}
	return true;
}

void 
gplayer_imp::ActiveFlyState(char type)
{
	gplayer * pPlayer = GetParent();
	bool is_mount = pPlayer->IsFlyMode();
	if(is_mount) return;

	_fly_info.active = 1;
	//加入命令限制
	_commander->DenyCmd(controller::CMD_MARKET);

	_skill.RemoveAura(this);
	
	//进行状态切换 
	pPlayer->SetObjectState(gactive_object::STATE_FLY);
	pPlayer->start_fly_type = type;

	//发送命令
	_runner->player_start_fly(1, type);
	
	//去除隐身
	if(pPlayer->IsInvisible()) _filters.RemoveFilter(FILTER_INDEX_INVISIBLE);
}

void 
gplayer_imp::DeactiveFlyState(char type)
{
	gplayer * pPlayer = GetParent();
	bool is_mount = pPlayer->IsFlyMode();
	if(!is_mount) return;

	_fly_info.active = 0;

	_commander->AllowCmd(controller::CMD_MARKET);
	pPlayer->start_fly_type = -1;

	//进行状态切换 
	pPlayer->ClrObjectState(gactive_object::STATE_FLY);
	
	//发送命令
	_runner->player_start_fly(0, type);
}

int 
gplayer_imp::OI_SpendTalismanStamina(float cost)
{
	float rst = _talisman_info.stamina - cost;
	if(rst < 0) rst = 0;
	if(rst != _talisman_info.stamina)
	{
		_talisman_info.stamina = rst;
		//按照数值下降
		SetRefreshState();
	}
	
	return (int) rst;
}

unsigned int 
gplayer_imp::GetDBMagicNumber()
{
	return _db_magic_number;
}

int gplayer_imp::SkillCreateItem(int id, int count, int period)
{
	if(GetInventory().GetEmptySlotCount()  == 0) return -1;
	if(count < 0) count = 1;
	DeliverItem(id,count, false, period, ITEM_INIT_TYPE_TASK);
	return 0;
}

void gplayer_imp::SelfHurt(int damage)
{
	if(_parent->IsZombie()) return;
	attacker_info_t info ={XID(-1,-1),0,0,0,0,0};
	int newdamage = DoDamage(damage);
	if(newdamage > damage) damage = newdamage;
	if(damage > 0)
	{
		OnHurt(XID(-1,-1),info,damage,false);
		if(_basic.hp <=0)
		{
			_basic.hp = 0;
			Die(info.attacker,false,0);
		}
	}
}

int gplayer_imp::TransformChatData(const void * data,size_t dsize, void * out_buffer, size_t len)
{
	if(dsize < sizeof(short)) return 0;
	int cmd = *(short*)data;
	switch(cmd)
	{
		case CHAT_C2S::CHAT_EQUIP_ITEM:
			{
				CHAT_C2S::chat_equip_item & cmd = *(CHAT_C2S::chat_equip_item *)data;
				if(sizeof(cmd) != dsize) return 0;

				item_list &inv  = GetInventory(cmd.where);
				size_t index = cmd.index;
				item_data_client data;
				if(inv.GetItemDataForClient(index, data) <= 0) return 0;
				if(data.use_wrapper)
				{
					packet_wrapper h1(data.ar.size() + sizeof(data));

					h1 << (short) CHAT_S2C::CHAT_EQUIP_ITEM << data.type << data.expire_date << (int) item::Proctype2State(data.proc_type) << (short)data.ar.size();
					h1.push_back(data.ar.data(), data.ar.size());

					if(len < h1.size()) return 0;
					memcpy(out_buffer, h1.data(), h1.size());
					return h1.size();
				}
				else
				{
					packet_wrapper h1(data.content_length + sizeof(data));

					h1 << (short) CHAT_S2C::CHAT_EQUIP_ITEM << data.type << data.expire_date << (int) item::Proctype2State(data.proc_type) << (short)data.content_length;
					h1.push_back(data.item_content, data.content_length);

					if(len < h1.size()) return 0;
					memcpy(out_buffer, h1.data(), h1.size());
					return h1.size();
				}
			}
			break;
		// Youshuang add
		case CHAT_C2S::CHAT_ACHIEVEMENT:
			{
				CHAT_C2S::chat_get_achievement & cmd = *(CHAT_C2S::chat_get_achievement *)data;
				if(sizeof(cmd) != dsize) return 0;

				unsigned short achieve_id = cmd.achieve_id;
				CHAT_S2C::chat_get_achievement tmp;
				tmp.achieve_id = achieve_id;
				tmp.name = GetPlayerName( tmp.namelen );
				tmp.finish_time = GetAchievementManager().GetAchivementFinishedTime( achieve_id );
				packet_wrapper h1( sizeof( tmp ) );
				h1 << (short) CHAT_S2C::CHAT_ACHIEVEMENT << tmp.achieve_id << tmp.finish_time << tmp.namelen;
				h1.push_back( tmp.name, tmp.namelen );
				if(len < h1.size()) return 0;
				memcpy(out_buffer, h1.data(), h1.size());
				return h1.size();
			}
			break;
		// end
	}
	return 0;
}

void 
gplayer_imp::UpdateSectID(int id)
{
	if(id == _parent->ID.id)
	{
		//自己成为师傅
		if(_sect_id != id)
		{
			ClearSect();
			SetSectID(id);
			_runner->sect_become_disciple(_parent->ID.id);	//成为师傅
		}
	}
	else
	{
		if(id == _sect_id) return ;

		int old_id = _sect_id;
		if(old_id > 0) ClearSect();	//先离开师门
		if(id)
		{
			//成为徒弟
			SetSectID(id);
			if(!old_id)
			{
				_extra_title.insert(SECT_MASTER_TITLE);
				_runner->player_add_title(SECT_MASTER_TITLE);
			}
			_sect_init_level = GetObjectLevel();
		}
		else
		{
			_extra_title.erase(SECT_MASTER_TITLE);
			_runner->player_del_title(SECT_MASTER_TITLE);
		}
		_runner->sect_become_disciple(id);     //离开师门，或者成为某人的徒弟
	}
}

static int GetContestRewardTaskID(int place)
{
	if(1 == place) return 12600;
	if(2 == place) return 12601;
	if(3 == place) return 12602;
	if(place >= 4 && place <= 10) return 12603;
	return 0;
}

void gplayer_imp::PlayerContestResult(int score,int place,int right_amount)
{
	GLog::log(GLOG_INFO,"玩家 %d 获得ContestResult奖励(%d,%d,%d)",_parent->ID.id,score,place,right_amount);
	if(score < 0) return;
	//奖励太极金单数量
	size_t count = score / 5 + 5;
	if(count > 105) count = 105;
	//13424只需要给1个 
	//13425的太极金丹按照积分给
	const item_data* pItem1 = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(13424);
	const item_data* pItem2 = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(13425);
	if(pItem1 && pItem1->pile_limit > 0 && pItem2 && pItem2->pile_limit && count > 0)
	{
		if(GetTaskInventory().GetEmptySlotCount() >= 2)
		{
			item_data* data1 = DupeItem(*pItem1);
			item_data* data2 = DupeItem(*pItem2);
			data1->count = 1;
			if(count > data2->pile_limit) count = data2->pile_limit;
			data2->count = count;
			if(ObtainItem(gplayer_imp::IL_TASK_INVENTORY,data1)) FreeItem(data1);
			if(ObtainItem(gplayer_imp::IL_TASK_INVENTORY,data2)) FreeItem(data2);
			GLog::log(GLOG_INFO,"用户答题 %d 得到 %d 个 %d",_parent->ID.id,1,13424);
			GLog::log(GLOG_INFO,"用户答题 %d 得到 %d 个 %d",_parent->ID.id,count,13425);
		}
	}
	//文彩值score * 10
	if(score > 0)
	{
		ModifyRegionReputation(7,score * 10);
	}
	//给东西
	int task_id = GetContestRewardTaskID(place);
	PlayerTaskInterface task_if(this);
	if(0 != task_id)
	{
		if(OnTaskCheckDeliver(&task_if,task_id,0))
		{
			__PRINTF("接到任务( %d )了...........\n",task_id);
		}
	}
	if(0 == place && right_amount >= 5)
	{
		//没有排名,答题数大于等于5的人给个任务
		task_id = 12871;
		if(OnTaskCheckDeliver(&task_if,task_id,0))
		{
			__PRINTF("接到任务( %d )了...........\n",task_id);
		}
	}
	object_interface oif(this);
	GetAchievementManager().OnGainScore(oif, 1, score);
}

void 
gplayer_imp::ActiveOfflineAgentTime(int agent_minute)
{
	_offline_agent_time += agent_minute;
	if(_offline_agent_time > 10000000) _offline_agent_time = 10000000;
	if(_offline_agent_time < 0) _offline_agent_time = 0;
	_runner->offline_agent_time(_offline_agent_time);
}


int gplayer_imp::DoLockItem(size_t index, int id)
{
	if(!CheckItemLockCondition(index, id)) return S2C::ERR_SERVICE_ERR_REQUEST;
	
	ASSERT(IsItemExist(index, id,1));
	//前面已经确认检查过是否可以进行绑定了

	if((size_t)GetMoney() < (size_t)g_config.lock_item_fee) return S2C::ERR_OUT_OF_FUND;
	
	item & it = _inventory[index];
	if(it.expire_date > 0) return S2C::ERR_CANNOT_LOCK_EXPIRE_ITEM;
	
	//立刻加锁
	it.Lock();
	SpendMoney(g_config.lock_item_fee);
	if (g_config.lock_item_fee) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=8:hint=%d",_parent->ID.id,_db_magic_number,g_config.lock_item_fee,it.type);

	_runner->spend_money(g_config.lock_item_fee);

	//再次发送一次物品数据
	PlayerGetItemInfo(IL_INVENTORY,index);

//	_runner->player_bind_success(index, id, it.GetProctypeState());
	GLog::log(GLOG_INFO,"用户%d加锁了物品%d，耗费金钱%d",_parent->ID.id,id, g_config.lock_item_fee);
	return 0;
}


int gplayer_imp::DoUnlockItem(size_t index, int id)
{
	if(!CheckItemUnlock(index, id)) return S2C::ERR_SERVICE_ERR_REQUEST;

	ASSERT(IsItemExist(index, id,1));
	//前面已经确认检查过是否可以进行绑定了

//确认有足够的手续费
	if((size_t)GetMoney() < (size_t)g_config.unlock_item_fee) return S2C::ERR_OUT_OF_FUND;
	if (g_config.unlock_item_fee) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=8:hint=%d",_parent->ID.id,_db_magic_number,g_config.unlock_item_fee,g_config.unlock_item_id);
	int item_index = -1;
	if(g_config.unlock_item_id &&  (item_index = _inventory.Find(0,g_config.unlock_item_id)) < 0) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
//进行物品打包操作
	item  it;
	_inventory.Remove(index, it);
	GDB::itemdata data;
	ConvertDBItemEntry(data, it, -1);

	//组织本物品的数据包
	packet_wrapper h1(200);
	h1 << data.id << data.index << data.count 
		<< (int)data.max_count << data.guid1 << data.guid2
		<< data.proctype << data.expire_date;

	h1 << data.size;
	h1.push_back(data.data,data.size);

	//用这个数据包组织新的物品
	item_data nit;
	nit.type        = g_config.item_unlocking_id;
	nit.proc_type	= 0;
	nit.count       = 1;
	nit.pile_limit  = 1;
	item::CustomBind(nit.proc_type);
	item::CustomLock(nit.proc_type);
	nit.guid.guid1  = 0;
	nit.guid.guid2  = 0;
	nit.price       = 0;
	nit.expire_date = g_timer.get_systime() + 3*24*3600;
	nit.content_length = h1.size();
	nit.item_content = (char *)h1.data();

	//通知客户端删除物品
	//并真的删除物品
	_runner->player_drop_item(IL_INVENTORY,index, it.type, it.count,S2C::DROP_TYPE_USE);
	it.Release();

	//检查是否需要记录消费值
	CheckSpecialConsumption(_inventory[item_index].type, 1);
	//删除手续费和原料
	UseItemLog(_inventory[item_index], 1);
	_inventory.DecAmount(item_index,1);
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item_index, g_config.unlock_item_id, 1, S2C::DROP_TYPE_USE);
	SpendMoney(g_config.unlock_item_fee);
	_runner->spend_money(g_config.unlock_item_fee);

	//将新的物品加入到玩家包裹之中
	int rst = _inventory.PushInEmpty(0, nit, 1);
	if(rst >=0)
	{       
		int state = item::Proctype2State(nit.proc_type);
		_runner->obtain_item(nit.type,nit.expire_date,1,_inventory[rst].count, IL_INVENTORY,rst,state);
	}
	else
	{       
		ASSERT(false);  //这是不应该出现的
	}
	GLog::log(GLOG_INFO,"用户%d试图恢复物品%d锁定耗费金钱%d",_parent->ID.id,id, g_config.unlock_item_fee);
	return 0;
}

void gplayer_imp::RestoreUnlock(item_list & list, int index, int where ,const item & it)
{
	ASSERT(list[index].type == -1);
	const void * data;
	size_t data_len;
	it.GetItemData(&data, data_len);
	if(data_len < sizeof(int)*9) return; //sizeof(int)*9 是物品的必要数据，考虑将此值用宏替代


	GDB::itemdata idata;
	raw_wrapper ar(data,data_len);
	
	ar >> idata.id >> idata.index >> idata.count
		>> (int&)idata.max_count >> idata.guid1 >> idata.guid2
		>> idata.proctype >> idata.expire_date;
	ar >> idata.size;

	idata.data = ar.cur_data();
	if(idata.size != ar.size() - ar.offset()) return;

	item new_item;
	if(!MakeItemEntry(new_item,idata)) return;
	new_item.ClearLockFlag();
	
	int type = new_item.type;
	int expire_date = new_item.expire_date;
	int rst = list.PushInEmpty(0, new_item);
	int state = item::Proctype2State(new_item.proc_type);
	_runner->obtain_item(type,expire_date,1,list[rst].count, where,rst,state);

	GLog::log(GLOG_INFO,"用户%d加锁的物品%d取消加锁完成了",_parent->ID.id,type);
	new_item.Clear();
	return ;
}

void gplayer_imp::DecSpiritPower(size_t index)
{
	if(index >= _equipment.Size()) return;
	item & it = _equipment[index];
	if(it.type == -1 || it.body == NULL) return ;
	int rst = it.body->DecSpiritPower(this, &it, 1, index);
	if(rst >= 0)
	{
		_runner->equip_spirit_decrease(index, 1, rst);
		if(rst == 0)
		{
			property_policy::UpdatePlayer(GetPlayerClass(),this);
		}
	}
}


void gplayer_imp::PlayerPostMessage(char message_type, unsigned char occupation, char gender, int level, 
		int faction_level, unsigned int message_len, char message[])
{
	char op_type = SNS_OP_PRESSMESSAGE;  
	if(CheckMessageCondition(message_type, op_type))
	{ 
		GMSV::SendPressMessage(_parent->ID.id, message_type, occupation, gender, level, faction_level, 
			message, message_len); 
	}	
}

void gplayer_imp::PlayerApplyMessage(char message_type, int message_id, unsigned int message_len, char message[])
{
	char op_type = SNS_OP_APPLY;  
	if(CheckMessageCondition(message_type, op_type))
	{ 
		GMSV::SendApplyMessage(_parent->ID.id, 0, message_type, message_id, 0, message, message_len);
	}
}

void gplayer_imp::PlayerVoteMessage(char vote_type, char message_type, int message_id)
{
	char op_type = vote_type ? SNS_OP_AGREE : SNS_OP_DENY;
	if(CheckMessageCondition(message_type, op_type))
	{ 
		GMSV::SendVoteMessage(_parent->ID.id, vote_type, message_type, message_id);
	}
}
  
void gplayer_imp::PlayerResponseMessage(char message_type, int message_id, short dst_index, unsigned int message_len, char message[])
{
	char op_type = SNS_OP_APPLYMSG;  
	if(CheckMessageCondition(message_type, op_type))
	{ 
		GMSV::SendApplyMessage(_parent->ID.id, 1, message_type, message_id, dst_index, message, message_len); 
	}
}

void gplayer_imp::PlayerHandleMessageResult(char message_type, char op_type, int charm)
{
	int money = player_template::GetSNSRequiredMoney(message_type, op_type);      
	int item_id = player_template::GetSNSRequiredItemID(message_type, op_type); 
	int item_id2 = player_template::GetSNSRequiredItemID2(message_type, op_type);
	int task_id = player_template::GetSNSRequiredTaskID(message_type, op_type, charm);

	if(!_pstate.IsNormalState() && !_pstate.IsBindState()) return ;
	
	if(money > 0)
	{
		if(GetMoney() < (size_t)money){
			money = GetMoney();
		}
		SpendMoney(money);
		_runner->spend_money(money);
	}

	if(item_id2 > 0 && IsItemExist(item_id2) )
	{
		TakeOutItem(item_id2, 1);
	}
	else if(item_id > 0 && IsItemExist(item_id))
	{
		TakeOutItem(item_id, 1);
	}

	if(task_id > 0)
	{
		PlayerTaskInterface task_if(this);
		if(OnTaskCheckDeliver(&task_if,task_id,0))
		{
			__PRINTF("征友平台接到任务( %d )了...........\n",task_id);
		}
	}
}

//检查是否消息操作是否满足条件
bool gplayer_imp::CheckMessageCondition(char message_type, char op_type)
{
	int money = player_template::GetSNSRequiredMoney(message_type, op_type);
	int item_id = player_template::GetSNSRequiredItemID(message_type, op_type);		
	int item_id2 = player_template::GetSNSRequiredItemID2(message_type, op_type);

	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;

	if(money > 0 && GetMoney() < (size_t)money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);		
		return false;
	}

	if((item_id >0 && !IsItemExist(item_id)) && (item_id2 >0 && !IsItemExist(item_id2)))
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);		
		return false;
	}
	return true;
}


void 
gplayer_imp::ChangePocketSize(int size)
{
	if(size > POCKET_INVENTORY_MAX_SIZE) size = POCKET_INVENTORY_MAX_SIZE;
	if(size < POCKET_INVENTORY_BASE_SIZE) size = POCKET_INVENTORY_BASE_SIZE;
	_pocket_inventory.SetSize(size);
	_runner->player_change_pocket_size(_pocket_inventory.Size());
}

void 
gplayer_imp::DumpPocketInventory()
{
	for(unsigned int i=0; i < _pocket_inventory.Size(); ++i)
	{
		char buf[512];
		sprintf(buf, "index=%d, type=%d, count=%d", i, _pocket_inventory[i].type, _pocket_inventory[i].count);	
		this->Say(buf);
	}
}

void 
gplayer_imp::PlayerCastItemSkill(int skill_id, char skill_type, int item_id, short item_idx, char force_attack, unsigned char target_count, int *targets)
{
	if(!IsItemExist(item_idx, item_id, 1)) return;

	int level = 0;
	if(item_id > 0)
	{
		level = GetItemSkillLevel(item_id, item_idx, skill_id);
	}	
	else
	{
		level = _skill.GetSkillLevel(skill_id); 
	}	

	//确保item_id和item_index需要大于0, item_id和item_index都为0代表的是变身类技能
	if(level <= 0 || item_id <=0 || item_idx < 0)
	{
		return;	
	}
	
	if(!_inventory[item_idx].CheckZoneUse(_inventory.GetLocation(), this)) return;

	bool isConsumable = IsConsumableItemSkill(item_id, item_idx); 

	//普通技能
	if(skill_type == 0)
	{
		session_skill *pSkill= new session_skill(this);
		pSkill->SetTarget(skill_id, force_attack, target_count, targets, level, item_id, item_idx, isConsumable);
		AddStartSession(pSkill);;

	}
	//瞬发技能
	else if(skill_type == 1)
	{
		session_instant_skill *pSkill= new session_instant_skill(this);
		pSkill->SetTarget(skill_id, force_attack, target_count, targets, level, item_id, item_idx, isConsumable, A3DVECTOR(), -1);
		AddStartSession(pSkill);;
	}
}


int 
gplayer_imp::GetItemSkillLevel(int item_id, short item_idx, int skill_id)
{
	if(!IsItemExist(item_idx, item_id, 1)) return -1;
	item &it = _inventory[item_idx];
	if(it.type == -1 || !it.body) return -1;
	return it.body->GetItemSkillLevel(skill_id, this);
}

bool
gplayer_imp::IsConsumableItemSkill(int item_id, short item_idx)
{
	if(!IsItemExist(item_idx, item_id, 1)) return false;
	item &it = _inventory[item_idx];
	if(it.type == -1 || !it.body) return false;
	return it.body->IsConsumableItemSkill();
}

bool
gplayer_imp::PlayerDoBillingShoppingStep1(int id, size_t order_index, size_t slot, size_t order_count)
{
	if(!_pstate.CanShopping()) return false;
	if(order_count == 0) return false;
	if(id <= 0) return false;
	if(GetWorldManager()->GetWorldLimit().noshop) return false;
	
	qgame::mall & shop = item_manager::GetShoppingMall();
	if(slot >= qgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	qgame::mall::node_t node;
	if(!shop.QueryGoods(order_index,node) || node.goods_id != id || node.entry[slot].cash_need <= 0)
	{	
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	
	int goods_count = node.goods_count;
	size_t cash_need = node.entry[slot].cash_need;

	if(GetBillingCash() <= 0 || GetBillingCash()/order_count < cash_need)
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	int total_count = goods_count * order_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	if(total_count <=0  || (int)(total_count/order_count) != goods_count || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return true;
	}

	//发送协议给Delivery
	GNET::SendBillingRequest(_parent->ID.id,node.goods_id, node.goods_count,
			node.entry[slot].expire_time, node.entry[slot].cash_need * order_count, order_count);
	return true;
}

bool
gplayer_imp::PlayerDoBillingShoppingStep2(int id, size_t goods_count, int expire_time, size_t total_cash, size_t order_count)
{
	//检查状态是否对
	if(!_pstate.CanShopping()) return false;
	if(order_count == 0) return false;
	if(id <= 0) return false;

	//delivery会回总的数量到goods_count和order_count里面
	ASSERT(goods_count == order_count);
	
	//检查金钱是否够 	
	if(GetBillingCash() <= 0 || (size_t)GetBillingCash() < total_cash)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	//检查物品是否正确
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return false;
	}

	//检查包裹是否已经满了	
	int total_count = goods_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	if(total_count <=0  ||  need_empty_slot == 0 || (size_t)need_empty_slot > _inventory.Size() || !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	int cash_need = total_cash/order_count;

	int expire_date = 0;
	if(expire_time)  expire_date = g_timer.get_systime() + expire_time;
	int self_id = GetParent()->ID.id;

	if(pItem->pile_limit == 1)
	{
		//需要GUID
		for(size_t i = 0; i < order_count; i ++)
		{
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * pItem2 = gmatrix::GetDataMan().generate_item(id,&tag,sizeof(tag));
			if(pItem2 == NULL) continue;
			//将物品放入包裹中
			int item_count = 1;

			int rst =_inventory.Push(*pItem2,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);

			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_SHOP);

			int state = item::Proctype2State(pItem2->proc_type);
			_runner->obtain_item(id,expire_date,1,_inventory[rst].count, 0,rst,state);

			_billing_cash -= cash_need;
			//专门记录日志  
			GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:item_id=%d:"
					"expire=%d:item_count=%d:cash_need=%d:cash_left=%d:guid=%d,%d",
					self_id, _db_magic_number, id, expire_date,goods_count,
					cash_need,_billing_cash, pItem2->guid.guid1,pItem2->guid.guid2);
			
			GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%d点",self_id,goods_count,id,expire_date,cash_need);

			GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%d:lv=%d:h_gold=%d:h_cash=%d:itemid=%d,%d", _db_magic_number, self_id, cash_need, GetObjectLevel(), GetMoney(), _billing_cash,id, goods_count);
			FreeItem(pItem2);
		}
	}
	else
	{
		//不需要GUID
		for(int i = total_count; i >0;)
		{
			//将物品放入包裹中
			int item_count = i;
			if((size_t)item_count > pItem->pile_limit) item_count = pItem->pile_limit;
			int ocount = item_count;
			int rst =_inventory.Push(*pItem,item_count,expire_date);
			ASSERT(rst >= 0 && item_count == 0);
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(id,expire_date,ocount,_inventory[rst].count, 0,rst,state);

			i -= ocount;
		}

		_billing_cash -= total_cash;
		//专门记录日志  
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%d:cash_left=%d:guid=0,0",
				self_id, _db_magic_number, id, expire_date,goods_count,cash_need,_billing_cash);
		GLog::log(GLOG_INFO, "用户%d百宝阁购买了%d个%d有效期%d花费了%d点",self_id,goods_count,id,expire_date,cash_need);

		GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%d:lv=%d:h_gold=%d:h_cash=%d:itemid=%d,%d", _db_magic_number, self_id, cash_need, GetObjectLevel(), GetMoney(), _billing_cash,id, goods_count);

	}

	_runner->player_cash(_billing_cash, _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	GNET::SendBillingBalance(_parent->ID.id);
	GLog::log(GLOG_INFO,"用户%d在百宝阁购买%d件物品，花费%d点剩余%d点",self_id,order_count,total_cash,_billing_cash);
	return true;
}


void 
gplayer_imp::RecycleBattleItem()
{
	//回收普通包裹里面的战场物品
	for(size_t i = 0; i < _inventory.Size(); ++i)
	{
		if(_inventory[i].type != -1 && _inventory[i].CanRecycle())
		{
			TakeOutItem(i, _inventory[i].type, _inventory[i].count);
		}		
	}

	//回收装备栏里面的战场物品
	for(size_t j = 0; j < _equipment.Size(); ++j)
	{
		if(_equipment[j].type != -1 && _equipment[j].CanRecycle())
		{
			TakeOutEquipItem(j, _equipment[j].type, _equipment[j].count);
		}
	}	
	
	//回收时装包裹里面的战场物品
	for(size_t k = 0; k < _fashion_inventory.Size(); ++k)
	{
		if(_fashion_inventory[k].type != -1 && _fashion_inventory[k].CanRecycle())
		{
			TakeOutFashionItem(k, _fashion_inventory[k].type, _fashion_inventory[k].count);
		}
	}	
	
	//回收仓库里面的战场物品
	for(size_t l = 0; l < _trashbox.Backpack().Size(); ++l)
	{
		if(_trashbox.Backpack()[l].type != -1 && _trashbox.Backpack()[l].CanRecycle())
		{
			TakeOutTrashItem(l, _trashbox.Backpack()[l].type, _trashbox.Backpack()[l].count);
		}
	}	

	//回收坐骑飞剑包裹里面的战场物品
	for(size_t w = 0; w < _mount_wing_inventory.Size(); ++w)
	{
		if(_mount_wing_inventory[w].type != -1 && _mount_wing_inventory[w].CanRecycle())
		{
			TakeOutMountWingItem(w, _mount_wing_inventory[w].type, _mount_wing_inventory[w].count);
		}
	}
}

bool 
gplayer_imp::RefinePetEquip(size_t item_index, int item_type, size_t stone_index)
{
	//检查包裹是否足够
	if(!_inventory.GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size || stone_index >= inv_size) return false;

	//检查物品是否存在
	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;
	if(it.IsLocked()) return false;

	//检查物品是否是宠物装备
	if(it.GetItemType() != item_body::ITEM_TYPE_PET_EQUIP) return false;

	//检查装备本体是否是宠物装备	
	DATA_TYPE dt2;
	const PET_ARMOR_ESSENCE& ess2 = *(const PET_ARMOR_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(item_type,ID_SPACE_ESSENCE,dt2);
	if(&ess2 == NULL || dt2 != DT_PET_ARMOR_ESSENCE) return false;		
	size_t need_money = ess2.fee_enhance;

	//检查强化费用是否足够
	if(GetMoney() < need_money) return false;			

	//检查石头是否存在
	int stone_type = _inventory[stone_index].type;	
	if(stone_type == -1) return false;

	bool need_bind = it.IsBind();

	//调用lua脚本
	int id = script_RefinePetEquip(item_type, stone_type);

	//检查宠物装备强化是否成功
	if(id == -1 || id == 0) return false;
	DATA_TYPE dt;
	const PET_ARMOR_ESSENCE& ess = *(const PET_ARMOR_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(id,ID_SPACE_ESSENCE,dt);                 
	if(&ess == NULL || dt != DT_PET_ARMOR_ESSENCE) return false;		
	const pet_equip_data_temp* pTemplate = pet_equip_dataman::Get(it.type);
	if(ess.equip_location != pTemplate->equip_location) return false;

	//消耗强化前的本体装备和强化材料
	item tmp;
	_inventory.Remove(item_index,tmp);
	_runner->player_drop_item(IL_INVENTORY,item_index,item_type,tmp.count,S2C::DROP_TYPE_USE);
	UseItemLog(tmp);
	tmp.Release();

	_inventory.DecAmount(stone_index, 1);
	_runner->player_drop_item(IL_INVENTORY,stone_index,stone_type, 1 ,S2C::DROP_TYPE_USE);

	//扣钱
	if(need_money > 0)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",GetParent()->ID.id,GetUserID(),need_money,item_type);
		SpendMoney(need_money);	
		_runner->spend_money(need_money);
	}	

	//生成新的装备
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * pItem = gmatrix::GetDataMan().generate_item(id,&tag,sizeof(tag));
	if(pItem == NULL) return false;
	
	int item_count = 1;
	int expire_date = 0;
	DeliverItem(id, item_count, need_bind, expire_date, ITEM_INIT_TYPE_COMBINE);
	

	GLog::log(GLOG_INFO,"用户%d强化宠物装备成功, 强化前装备ID=%d, 强化后装备ID=%d, 强化材料ID=%d",_parent->ID.id, item_type, id, stone_type);

	// 更新宠物操作活跃度
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return true;
}

void gplayer_imp::PlayerDiscover(int discover_type)
{
	script_DiscoverGain(_basic.level, IsPlayerFemale(), GetPlayerClass(), discover_type);
}

int 
gplayer_imp::script_RefinePetEquip(int item_type, int stone_type)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PLAYER);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return -1;
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetEquip_Refine_Entrance");
	lua_pushinteger(L,item_type);
	lua_pushinteger(L,stone_type);

	if(lua_pcall(L,2,2,0))
	{
		lua_pop(L,1);
		return -1;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2))
	{
		lua_pop(L,2);
		return -1;
	}

	int pet_id= lua_tointeger(L, -2);
	int msg_id= lua_tointeger(L, -1);

	lua_pop(L,2);

	_runner->script_message(_parent->ID.id,0 ,0, msg_id);

	return pet_id;
}

bool 
gplayer_imp::CatchPet(int monster_id, int monster_level, int monster_raceinfo, int monster_catchdifficulty, float monster_hp_ratio, int & petbedge_id)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PLAYER);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false;
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"Player_CatchPet_Entrance");
	lua_pushinteger(L,monster_id);
	lua_pushinteger(L,monster_level);
	lua_pushinteger(L,monster_raceinfo);
	lua_pushinteger(L,monster_catchdifficulty);
	lua_pushnumber(L, monster_hp_ratio); 
	lua_pushinteger(L,_basic.level);
	lua_pushlightuserdata(L, (void*)this);

	if(lua_pcall(L,7,3,0))
	{
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L, -3))
	{
		lua_pop(L,3);
		return false;
	}

	int pet_tid = lua_tointeger(L, -3);
	int channel_id = lua_tointeger(L, -2);
	int msg_id = lua_tointeger(L, -1);
	lua_pop(L,3);

	if(pet_tid <= 0)
	{
		_runner->script_message(_parent->ID.id,0 ,channel_id,msg_id);
		return false;
	}

	petbedge_id = pet_tid;
	return true;
}

bool 
gplayer_imp::CatchPetSuccess(int petbedge_id)
{
	if(petbedge_id <= 0) return false;

	if(!_inventory.GetEmptySlotCount())
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	DATA_TYPE datatype;
	const void * dataptr = gmatrix::GetDataMan().get_data_ptr(petbedge_id, ID_SPACE_ESSENCE, datatype);
	if(dataptr == NULL ||  datatype != DT_PET_BEDGE_ESSENCE)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}

	DeliverItem(petbedge_id, 1, false, 0, ITEM_INIT_TYPE_TASK);
	return true;
}

void 
gplayer_imp::script_ExtGainOfProduce(int recipe_id, int player_level, bool is_female, int player_class) 
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PLAYER);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return;
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"ExtGain_Produce_Entrance");
	lua_pushinteger(L,recipe_id);
	lua_pushinteger(L,player_level);
	lua_pushinteger(L, is_female ? 0 : 1);
	lua_pushinteger(L, player_class);
	lua_pushlightuserdata(L, (void*)this);

	if(lua_pcall(L,5,3,0))
	{
		lua_pop(L,1);
		return;
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L, -3))
	{
		lua_pop(L,3);
		return;
	}

	int item_id= lua_tointeger(L, -3);
	int item_count= lua_tointeger(L, -2);
	int msg_id= lua_tointeger(L, -1);
	lua_pop(L,3);

	if(item_id > 0)
	{
		_runner->script_message(_parent->ID.id,0 ,0, msg_id);
		DeliverItem(item_id, item_count, false, 0, ITEM_INIT_TYPE_TASK);
	}
}

void 
gplayer_imp::script_DiscoverGain(int player_level, bool is_female, int player_class, int discover_type)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PLAYER);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return;
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"Discover_Gain_Entrance");
	lua_pushinteger(L,player_level);
	lua_pushinteger(L, is_female ? 0 : 1);
	lua_pushinteger(L, player_class);
	lua_pushinteger(L,discover_type);
	lua_pushlightuserdata(L, (void*)this);

	if(lua_pcall(L,5,3,0))
	{
		lua_pop(L,1);
		return;
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L, -3))
	{
		lua_pop(L,3);
		return;
	}

	int item_id= lua_tointeger(L, -3);
	int item_count= lua_tointeger(L, -2);
	int msg_id= lua_tointeger(L, -1);
	lua_pop(L,3);

	if(item_id > 0)
	{
		_runner->script_message(_parent->ID.id,0 ,0, msg_id);
		DeliverItem(item_id, item_count, false, 0, ITEM_INIT_TYPE_TASK);
	}	
}

bool 
gplayer_imp::PlayerChangeStyle(unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid, size_t item_index)
{
	if(item_index >= _inventory.Size()) return false;
	//检查物品是否存在
	if(_inventory[item_index].type != g_config.item_change_style || _inventory[item_index].type == -1 || _inventory[item_index].type == 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}
	
	//检查是否需要记录消费值
	CheckSpecialConsumption(g_config.item_change_style, 1);
	//扣除物品
	_inventory.DecAmount(item_index, 1);
	_runner->player_drop_item(IL_INVENTORY, item_index, g_config.item_change_style, 1, S2C::DROP_TYPE_USE);

	GMSV::SendChangeStyle(_parent->ID.id, faceid, hairid, earid, tailid, fashionid);
	return true;
}

void 
gplayer_imp::HandleChangeStyle(unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid)
{
	_runner->player_change_style(faceid, hairid, earid, tailid, fashionid);
}

//被推广人第一次登陆的时候颁发奖励
void 
gplayer_imp::DeliveryReferrAward()
{
	if(_played_time == 0 && _referrer_id > 0)
	{
		PlayerTaskInterface tf(this);
		if(GetParent()->GetClass() == 0)
		{
			if(OnTaskCheckDeliver(&tf,14722,0))
			{
				GLog::log(GLOG_INFO, "用户%d领取被推广人登陆奖励",_parent->ID.id);
			}
		}
		else if(GetRace() == RACE_DIVINE)
		{
			if(OnTaskCheckDeliver(&tf,19484,0))
			{
				GLog::log(GLOG_INFO, "用户%d领取被推广人登陆奖励",_parent->ID.id);
			}
		}
		else if(GetRace() == RACE_TIANMAI)
		{
			if(OnTaskCheckDeliver(&tf,30596,0))
			{
				GLog::log(GLOG_INFO, "用户%d领取被推广人登陆奖励",_parent->ID.id);
			}
		}
	}
}

bool
gplayer_imp::CheckTradeSpecialReq(int special_id, int count)
{
	switch(special_id)
	{
		//仙基
		case 1:
		{
			if(GetBattleScore() < (size_t)count)
			{
				return false;
			}
		}
		break;	

		default:
		{
			return false;
		}
		break;
	}
	return true;
}

void
gplayer_imp::ConsumeTradeSpecialReq(int special_id, int count)
{
	switch(special_id)
	{
		//仙基
		case 1:
		{
			DecBattleScore(count);				
			return;
		}
		break;	

		default:
		{
			return; 
		}
		break;
	}
	return;
}

bool
gplayer_imp::PlayerEnableVIPState(int item_id)
{
	gplayer *pPlayer = GetParent();
	if(pPlayer->IsVIPState()) return false;

	DATA_TYPE dt;
	const VIP_CARD_ESSENCE & ess = *(VIP_CARD_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_id,ID_SPACE_ESSENCE,dt);
	if(dt != DT_VIP_CARD_ESSENCE || &ess == NULL) return false;

	pPlayer->object_state |= gactive_object::STATE_IN_VIP;
	pPlayer->vip_type = (unsigned char)ess.vip_type;

	_vipstate.exp_ratio = ess.multiple_exp;
	_vipstate.free_bot = ess.free_helper;
	_vipstate.no_exp_drop = ess.no_exp_drop;
	
	for(size_t i = 0; i < sizeof(ess.buff)/(sizeof(int)*2); ++i)
	{
		if(ess.buff[i].skill_id <= 0) continue;
		SKILL::Data data(ess.buff[i].skill_id);
		_skill.CastRune(data, this, ess.buff[i].skill_level, 0);

	}
	
	_runner->player_change_vipstate(pPlayer->vip_type);

	// 更新活跃度[炼丹炉-VIP卡状态]
//	EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_VIP_CARD_OPEN);

	GLog::log(GLOG_INFO,"用户%d激活使用VIP卡, item_id=%d, exp_ratio=%f, free_bot=%d, no_exp_drop=%d", 
        	_parent->ID.id, item_id, _vipstate.exp_ratio, _vipstate.free_bot, _vipstate.no_exp_drop);

	return true;
}

void
gplayer_imp::PlayerDisableVIPState()
{
	gplayer *pPlayer = GetParent();
	if(!pPlayer->IsVIPState()) return;

	pPlayer->object_state &= ~gactive_object::STATE_IN_VIP;
	pPlayer->vip_type = 0;
	_runner->player_change_vipstate(0);

	_vipstate.exp_ratio = 0;
	_vipstate.free_bot = false;
	_vipstate.no_exp_drop = false;
	
	GLog::log(GLOG_INFO,"用户%d停止使用VIP卡", _parent->ID.id); 
}


void
gplayer_imp::HandleNetbarReward(int netbar_level, int award_type)
{
	if(netbar_level > 3 || netbar_level <= 0) return;
	if(award_type > 4 || award_type <= 0) return;
#define NETBAR_INC_EXP 2007	//持续经验
#define NETBAR_INC_HP  2008	//持续回红
#define NETBAR_INC_MP  2009	//持续回蓝
#define NETBAR_INC_DODGE 2010 	//增加躲闪	
	int netbar_skill[] = {NETBAR_INC_EXP, NETBAR_INC_HP, NETBAR_INC_MP, NETBAR_INC_DODGE};

	SKILL::Data data(netbar_skill[award_type-1]);
	_skill.CastRune(data, this, netbar_level, 0);

}

void
gplayer_imp::HandleAddGTReward()
{
	{ SKILL::Data data(gmatrix::GetGTSkillID()); _skill.CastRune(data, this, 1, 0); }
}

void
gplayer_imp::HandleRemoveGTReward()
{
	for(size_t i = 0; i < 10; ++i)
	{
		_filters.RemoveFilter(gmatrix::GetGTBuffID()+i);
	}
}

void 
gplayer_imp::SendAchievementMessage(unsigned short achieve_id, int broad_type, int param, int finish_time)
{
	int roleid = _parent->ID.id;
	switch (broad_type)
	{
		case BROADTYPE_LOCAL:
			_runner->achievement_message(achieve_id, param, finish_time);
			break;
		case BROADTYPE_ALL:
			GMSV::SendAchievementMsg(broad_type, roleid, achieve_id, param, finish_time);
			break;
	}
}


//对老用户检查一下是否完成了某些成就
void
gplayer_imp::TryEnableAchievement()
{
	object_interface oif(this);
	for(int i = 0; i <= GetRebornCount(); ++i)
	{
		GetAchievementManager().OnLevelUp(oif, i, GetRebornLvl(i));
	}

}

void
gplayer_imp::SendPKMessage(int killer, int deader)
{
	_runner->send_pk_message(killer, deader);
//	GMSV::SendPKMsg(killer, deader);
}

void 
gplayer_imp::SendRefineMessage(int item_id, int refine_level)
{
	int roleid = _parent->ID.id;
	GMSV::SendRefineMsg(roleid, item_id, refine_level);
}

void
gplayer_imp::OnTaskComplete(int task_id, int count)
{
	object_interface oif(this);
	GetAchievementManager().OnFinishTask(oif, task_id);

	// 更新活跃度
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_TASK_DONE, task_id);

	// 更新台历相关任务
	if (item_sale_promotion::isRelatedTask(task_id)) {
		_sale_promotion_info.taskids.insert(task_id);	
		((gplayer_dispatcher*)_runner)->sale_promotion_info(_sale_promotion_info.taskids);
	}
	DeliverTopicSite( new TOPIC_SITE::task_in( task_id ) );  // Youshuang add
}

bool 
gplayer_imp::IsAchievementFinish(unsigned short id)
{
	return GetAchievementManager().IsFinished(id);
}

void
gplayer_imp::FinishAchievement(unsigned short id)
{
	object_interface oif(this);
	GetAchievementManager().FinishAchievement(oif, id);
}

void
gplayer_imp::UnfinishAchievement(unsigned short id)
{
	object_interface oif(this);
	GetAchievementManager().UnFinishAchievement(oif, id);
}

bool
gplayer_imp::RefineMagic(size_t index, int id, size_t stone_index, int stone_id, int refine_type)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(index >= inv_size || stone_index > inv_size) return false;
	
	//检查物品是否存在
	if(!IsItemExist(index,id,1) || !IsItemExist(stone_index,stone_id,1)) return false;
	
	item & it1 = _inventory[index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_MAGIC) return false; 
	if(it1.IsLocked()) return false;

	DATA_TYPE dt;
	const CHANGE_SHAPE_STONE_ESSENCE & ess_stone = *(CHANGE_SHAPE_STONE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(stone_id, ID_SPACE_ESSENCE,dt);
	if(dt != DT_CHANGE_SHAPE_STONE_ESSENCE || &ess_stone == NULL)
	{
		return false;
	}
	
	DATA_TYPE dt2;
	const CHANGE_SHAPE_CARD_ESSENCE & ess_card = *(CHANGE_SHAPE_CARD_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(id, ID_SPACE_ESSENCE,dt2);
	if(dt2 != DT_CHANGE_SHAPE_CARD_ESSENCE || &ess_stone == NULL)
	{
		return false;
	}

	if(refine_type != ess_stone.refine_type) return false;

	size_t need_money = 0; 
	switch(refine_type)
	{
		case REFINE_TYPE_RESET:
			need_money = ess_card.fee_restore;
			break;
		case REFINE_TYPE_LEVELUP:
			need_money = ess_card.fee_refine;
			break;
		case REFINE_TYPE_EXPUP:
			need_money = ess_card.fee_contract;
			break;
		default:
			break;
	}

	if(GetMoney() < need_money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}

	if(!it1.body->Refine(&it1, this, index, stone_index))
	{
		return false;
	}

	PlayerGetItemInfo(IL_INVENTORY,index);

	//删除材料
	UseItemLog(_inventory[stone_index],1);
	_inventory.DecAmount(stone_index, 1);
	_runner->player_drop_item(IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);

	//扣除金钱
	if(need_money > 0)
	{	
		SpendMoney(need_money);
		_runner->spend_money(need_money);
	}
	
	return true;
}

bool
gplayer_imp::RestoreMagic(size_t index, int id) 
{
	//检查是否越界
	if(index >= _inventory.Size() )return false;

	//检查物品是否存在
	if(!IsItemExist(index,id,1) ) return false;

	item & it = _inventory[index];
	if(it.GetItemType() != item_body::ITEM_TYPE_MAGIC) return false; 

	if(!it.body->RestoreDuration(&it,this)) return false;

	//更新客户端数据
	PlayerGetItemInfo(IL_INVENTORY,index);
	return true;
}

int
gplayer_imp::DecMagicDuration(int dec)
{
	item & it = _equipment[item::EQUIP_INDEX_MAGIC];
	if(it.type == -1 || it.body == NULL) return 0;
	int rst = it.body->DecMagicDuration(this, &it, dec, item::EQUIP_INDEX_MAGIC);
	if(rst >= 0)
	{
		_runner->magic_duration_decrease(item::EQUIP_INDEX_MAGIC, 1, rst);
	}
	return rst;
}

bool 
gplayer_imp::MagicGainExp(int exp)
{
	int index = item::EQUIP_INDEX_MAGIC;
	item & it = _equipment[index];
	if(it.type == -1 || it.body == NULL) return false;
	bool level_up = false;
	int rst = it.body->GainExp(item::BODY, exp, &it,this, index, level_up);
	if(rst >= 0) 
	{
		//汇报新经验
		_runner->magic_exp_notify(IL_EQUIPMENT,index, rst);
		if(level_up)
		{
			property_policy::UpdatePlayer(GetPlayerClass(),this);
		       	PlayerGetItemInfo(IL_EQUIPMENT, index);
			_runner->self_get_property(_basic.status_point,_cur_prop);
		}
		return true;
	}
	return false;
}

bool
gplayer_imp::PlayerStartTransform()
{
	if(!_pstate.IsNormalState()) return false;
	if(IsTransformState()) return false;
	if(GetParent()->IsMountMode()) return false;
	if(InFlying()) return false;		//暂时不支持

	item & magic_item = _equipment[item::EQUIP_INDEX_MAGIC];
	if(magic_item.type == -1 || magic_item.GetItemType() != item_body::ITEM_TYPE_MAGIC)
	{
		return false;
	}

	if(!_talisman_info.talisman_state ||  _talisman_info.bot_state || _talisman_info.stamina < _talisman_info.max_stamina * 0.5f)
	{
		return false;
	}

	
	DATA_TYPE dt;
	const CHANGE_SHAPE_CARD_ESSENCE & ess_card = *(CHANGE_SHAPE_CARD_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(magic_item.type, ID_SPACE_ESSENCE,dt);
	if(dt != DT_CHANGE_SHAPE_CARD_ESSENCE || &ess_card == NULL)
	{
		return false;
	}

	if(!CheckCoolDown(COOLDOWN_INDEX_TRANSFORM))
	{
		return false;
	}
	SetCoolDown(COOLDOWN_INDEX_TRANSFORM,5000);

	unsigned char level = 0;
	unsigned char exp_level = 0;
	float energy_drop_speed = 0.f;
	int exp_speed = 0;
	int duration = 0;

	if(!magic_item.GetTransformInfo(level, exp_level, energy_drop_speed, exp_speed, duration)) return false;
	if(level <=0 || exp_level <=0 || duration <= 0) return false; 

	DATA_TYPE dt2;
	const CHANGE_SHAPE_PROP_CONFIG & ess = *(CHANGE_SHAPE_PROP_CONFIG*)gmatrix::GetDataMan().get_data_ptr(ess_card.ref_temp_id, ID_SPACE_CONFIG,dt2);
	if(dt2 != DT_CHANGE_SHAPE_PROP_CONFIG || &ess == NULL)
	{
		ASSERT(false);
	}
	_transform_obj = new player_item_transform(this, player_transform::TRANSFORM_ITEM, ess_card.ref_temp_id, level, exp_level, energy_drop_speed, exp_speed);
	_transform_obj->StartTransform(this);

	// 更新活跃度，18. 幻化成功(通过物品)
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_HUANHUASHI);
	return true;
}

void
gplayer_imp::PlayerStopTransform()
{
	if(!IsTransformState()) return;
	if(_transform_obj == NULL) return;
	StopTransform(_transform_obj->GetTransformID(), player_transform::TRANSFORM_ITEM);
}

bool
gplayer_imp::StartTransform(int template_id, int level, int exp_level, int timeout, char type)
{
	ASSERT(type == player_transform::TRANSFORM_SKILL || type == player_transform::TRANSFORM_TASK);
	if(!_pstate.IsNormalState() && !_pstate.IsBindState()) return false;

	if(IsTransformState()) 
	{
		if(_transform_obj->CanModifyTransform(template_id, type, level, exp_level))
		{
			_transform_obj->ModifyTransform(this);
			return true;
		}
		else if( _transform_obj->CanChangeTransform(template_id, type))
		{	
			return ChangeTransform(template_id, level, exp_level, timeout, type);
		}
		else
		{
			return false;
		}
	}

	return Transform(template_id, level, exp_level, timeout, type);
}

bool
gplayer_imp::ChangeTransform(int template_id, int level, int exp_level, int timeout, char type)
{
	if(!IsTransformState()) return false;
	_transform_obj->StopTransform(this);

	gplayer *pPlayer = GetParent();
	pPlayer->ClrObjectState(gactive_object::STATE_TRANSFORM);
	pPlayer->transform_id = 0; 
	
	delete _transform_obj;
	_transform_obj = NULL;
	
	return Transform(template_id, level, exp_level, timeout, type);
}

bool
gplayer_imp::Transform(int template_id, int level, int exp_level, int timeout, char type)
{
	ASSERT(type == player_transform::TRANSFORM_SKILL || type == player_transform::TRANSFORM_TASK);
	//去除骑乘效果
	if(_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER))
	{
		_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
	}
	//去除飞行效果
	if(_filters.IsFilterExist(FILTER_INDEX_AIRCRAFT))
	{
		_filters.RemoveFilter(FILTER_INDEX_AIRCRAFT);
	}
	//去除相依相偎效果
	LeaveBindState();

	DATA_TYPE dt;
	const CHANGE_SHAPE_PROP_CONFIG & ess = *(CHANGE_SHAPE_PROP_CONFIG*)gmatrix::GetDataMan().get_data_ptr(template_id, ID_SPACE_CONFIG,dt);
	if(dt != DT_CHANGE_SHAPE_PROP_CONFIG || &ess == NULL)
	{
		ASSERT(false);
	}
	if(type == player_transform::TRANSFORM_SKILL)
	{
		_transform_obj = new player_skill_transform(this, type, template_id, level, exp_level, timeout); 
	}
	else if(type == player_transform::TRANSFORM_TASK)
	{
		_transform_obj = new player_task_transform(this, type, template_id, level, exp_level, timeout); 
	}
	_transform_obj->StartTransform(this);
	return true;
}

void
gplayer_imp::StopTransform(int template_id, char type)
{
	if(!IsTransformState()) return;
	if(_transform_obj == NULL) return;
	if(!_transform_obj->CanStopTransform(template_id, type)) return;

	_transform_obj->StopTransform(this);
}

void 
gplayer_imp::ActiveTransformState()
{
	OnActiveTransformState();
}

void 
gplayer_imp::OnActiveTransformState()
{
	ASSERT(_transform_obj);
	if(IsTransformState()) return;

	gplayer *pPlayer = GetParent();
	pPlayer->SetObjectState(gactive_object::STATE_TRANSFORM);
	pPlayer->transform_id = _transform_obj->GetTransformID(); 
	_transform_obj->OnActiveTransformState(this);

	property_policy::UpdatePlayer(GetPlayerClass(),this);
	_runner->self_get_property(_basic.status_point,_cur_prop);
}

void 
gplayer_imp::DeactiveTransformState()
{
	if(!IsTransformState()) return;
	OnDeactiveTransformState();
}

void 
gplayer_imp::OnDeactiveTransformState()
{
	if(!IsTransformState()) return;

	ASSERT(_transform_obj);
	_transform_obj->OnDeactiveTransformState(this);
	gplayer *pPlayer = GetParent();
	pPlayer->ClrObjectState(gactive_object::STATE_TRANSFORM);
	pPlayer->transform_id = 0; 
	
	delete _transform_obj;
	_transform_obj = NULL;
	property_policy::UpdatePlayer(GetPlayerClass(),this);
	_runner->self_get_property(_basic.status_point,_cur_prop);
}

bool
gplayer_imp::CanUsePotion()
{
	if(_transform_obj && !_transform_obj->CanUsePotion()) return false;
	return true;
}

void 
gplayer_imp::PlayerCastTransformSkill(int skill_id, char skill_type, char force_attack, unsigned char target_count, int *targets, const A3DVECTOR& pos)
{
	CastTransformSkill(skill_id, skill_type, force_attack, target_count, targets, pos);
}

void 
gplayer_imp::CastTransformSkill(int skill_id, char skill_type, char force_attack, unsigned char target_count, int *targets, const A3DVECTOR& pos)
{
	gplayer * pPlayer = GetParent();
	bool is_transform = pPlayer->IsTransformMode();
	if(!is_transform) return;

	int level = GetTransformSkillLevel(skill_id);
	if(level <= 0) return;

	//普通技能
	if(skill_type == 0)
	{
		session_skill *pSkill= new session_skill(this);
		pSkill->SetTarget(skill_id, force_attack, target_count, targets, level, 0, 0, false, pos);
		AddStartSession(pSkill);;

	}
	//瞬发技能
	else if(skill_type == 1)
	{
		session_instant_skill *pSkill= new session_instant_skill(this);
		pSkill->SetTarget(skill_id, force_attack, target_count, targets, level, 0, 0, false, pos, -1);
		AddStartSession(pSkill);;
	}
}


int
gplayer_imp::GetTransformSkillLevel(int skill_id)
{
	if(_transform_obj == NULL) return -1;
	return _transform_obj->GetSkillLevel(skill_id);

}

bool
gplayer_imp::GetTransformSkillData(archive & ar)
{
	if(_transform_obj == NULL) return false;
	_transform_obj->GetSkillData(ar);
	return true;

}

void 
gplayer_imp::OI_AddTransformSkill(int skill_id, int skill_level)
{
	if(_transform_obj)
	{
		_transform_obj->AddTransformSkill(skill_id, skill_level);
	}
}

int
gplayer_imp::OI_GetTransformID()
{
	if(_transform_obj) return _transform_obj->GetTransformID();
	return 0;
}

int
gplayer_imp::OI_GetTransformLevel()
{
	if(_transform_obj) return _transform_obj->GetTransformLevel();
	return 0;
}

int
gplayer_imp::OI_GetTransformExpLevel()
{
	if(_transform_obj) return _transform_obj->GetTransformExpLevel();
	return 0;
}

bool
gplayer_imp::OI_IsCloneExist()
{
	return _summonman.IsCloneExist(); 
}

void
gplayer_imp::OI_UnSummonMonster(char type)
{
	GetSummonMan().ClearSummonNPCByType(this, type);
}

bool gplayer_imp::OI_SummonExist(int tid, XID& id)
{ 
	return GetSummonMan().IsSummonExist(this, tid, id);
}

void 
gplayer_imp::OI_AddHP(int hp)
{
	if(_transform_obj)_transform_obj->AddHP(hp);
} 

void 
gplayer_imp::OI_AddMP(int mp)
{
	if(_transform_obj)_transform_obj->AddMP(mp);
} 

void 
gplayer_imp::OI_AddDefense(int defense)
{
	if(_transform_obj)_transform_obj->AddDefense(defense);
}

void 
gplayer_imp::OI_AddAttack(int attack)
{
	if(_transform_obj)_transform_obj->AddAttack(attack);
}

void 
gplayer_imp::OI_AddArmor(int armor)
{
	if(_transform_obj)_transform_obj->AddArmor(armor);
}

void 
gplayer_imp::OI_AddDamage(int damage)
{
	if(_transform_obj)_transform_obj->AddDamage(damage);
}	

void 
gplayer_imp::OI_AddResistance(const int resistance[6])
{
	if(_transform_obj)_transform_obj->AddResistance(resistance);
}	

void 
gplayer_imp::OI_AddCrit(int crit_rate, float crit_damage)
{
	if(_transform_obj)_transform_obj->AddCrit(crit_rate, crit_damage);
}	

void 
gplayer_imp::OI_AddSpeed(float speed)
{
	if(_transform_obj)_transform_obj->AddSpeed(speed);
}	

void 
gplayer_imp::OI_AddAntiCrit(int anti_crit, float anti_crit_damage)
{
	if(_transform_obj)_transform_obj->AddAntiCrit(anti_crit, anti_crit_damage);
}	

void 
gplayer_imp::OI_AddSkillRate(int skill_attack_rate, int skill_armor_rate)
{
	if(_transform_obj)_transform_obj->AddSkillRate(skill_attack_rate, skill_armor_rate);
}	

void
gplayer_imp::OI_AddAttackRange(float range)
{
	if(_transform_obj)_transform_obj->AddAttackRange(range);
}

void 
gplayer_imp::OI_AddScaleHP(int hp)
{
	if(_transform_obj)_transform_obj->AddScaleHP(hp);
}

void 
gplayer_imp::OI_AddScaleMP(int mp)
{
	if(_transform_obj)_transform_obj->AddScaleMP(mp);
}

void 
gplayer_imp::OI_AddScaleDefense(int defense)
{
	if(_transform_obj)_transform_obj->AddScaleDefense(defense);
}

void 
gplayer_imp::OI_AddScaleAttack(int attack)
{
	if(_transform_obj)_transform_obj->AddScaleAttack(attack);
}

void 
gplayer_imp::OI_AddScaleArmor(int armor)
{
	if(_transform_obj)_transform_obj->AddScaleArmor(armor);
}

void 
gplayer_imp::OI_AddScaleDamage(int damage)
{
	if(_transform_obj)_transform_obj->AddScaleDamage(damage);
}

void 
gplayer_imp::OI_AddScaleSpeed(int speed)
{
	if(_transform_obj)_transform_obj->AddScaleSpeed(speed);
}

void 
gplayer_imp::OI_AddScaleResistance(const int resistance[6])
{
	if(_transform_obj)_transform_obj->AddScaleResistance(resistance);
}


//玩家请求上交通工具
bool 
gplayer_imp::PlayerEnterCarrier(int carrier_id, const A3DVECTOR & rpos, unsigned char rdir)
{
	if(GetParent()->carrier_id != 0) return false;

	world::object_info info;
	bool rst = _plane->QueryObject(XID(GM_TYPE_NPC,carrier_id),info);
	if(!rst) return false;

	npc_template* pTemplate = npc_stubs_manager::Get(info.tid);
	if(!pTemplate) return false;
	if(!pTemplate->npc_data) return false;
	if(npc_template::npc_statement::NPC_TYPE_CARRIER != pTemplate->npc_data->npc_type)
	{
		return false;
	}

	LeaveBindState();

	msg_player_enter_carrier msg;
	msg.rpos = rpos;
	msg.rdir = rdir;
	SendTo<0>(GM_MSG_PLAYER_ENTER_CARRIER, XID(GM_TYPE_NPC,carrier_id) ,0, &msg, sizeof(msg));
	return true;
}

//玩家请求下交通工具
bool 
gplayer_imp::PlayerLeaveCarrier(int carrier_id, const A3DVECTOR & pos, unsigned char dir)
{
	gplayer* pPlayer = GetParent();
	if(!pPlayer->CheckExtraState(gplayer::STATE_CARRIER)) return false;
	if(carrier_id != GetParent()->carrier_id) return false;

	world::object_info info;
	bool rst = _plane->QueryObject(XID(GM_TYPE_NPC,carrier_id),info);
	if(!rst) LeaveCarrier(carrier_id);
	npc_template* pTemplate = npc_stubs_manager::Get(info.tid);
	if(!pTemplate || !pTemplate->npc_data || npc_template::npc_statement::NPC_TYPE_CARRIER != pTemplate->npc_data->npc_type)
	{
		LeaveCarrier(carrier_id);
	}

	LeaveCarrier(carrier_id, pos, dir);
	msg_player_leave_carrier msg;
	msg.pos = pos;
	msg.dir = dir;
	SendTo<0>(GM_MSG_PLAYER_LEAVE_CARRIER, XID(GM_TYPE_NPC,carrier_id) ,0, &msg, sizeof(msg));
	return true;
}

void 
gplayer_imp::EnterCarrier(int carrier_id,const A3DVECTOR & rpos,unsigned char rdir,const A3DVECTOR & carrier_pos,unsigned char carrier_dir)
{
	gplayer* pPlayer = GetParent();

	//设置好相对位置,方向和状态
	pPlayer->rpos = rpos;
	pPlayer->rdir = rdir;
	pPlayer->carrier_id = carrier_id;
	pPlayer->SetExtraState(gplayer::STATE_CARRIER);


	//设置绝对坐标和方向
	A3DVECTOR offset = rpos;
	offset += carrier_pos;
	offset -= _parent->pos;
	StepMove(offset);
	pPlayer->dir = rdir + carrier_dir -64;

	_runner->player_enter_carrier(carrier_id, rpos, rdir, true);
}

//玩家请求离开
void 
gplayer_imp::LeaveCarrier(int carrier_id,const A3DVECTOR & pos,unsigned char dir) 
{
	gplayer* pPlayer = GetParent();
	if(!pPlayer->CheckExtraState(gplayer::STATE_CARRIER)) return;

	//这里移动到指定位置
	A3DVECTOR offset = pos;
	offset -= pPlayer->pos;
	StepMove(offset);
	pPlayer->dir = dir;

	//设置好相对绝对位置和方向
	pPlayer->ClrExtraState(gplayer::STATE_CARRIER);
	pPlayer->carrier_id = 0;
	pPlayer->rpos = A3DVECTOR(0.0f,0.0f,0.0f);
	pPlayer->rdir = 0;

	_runner->player_leave_carrier(carrier_id,GetParent()->pos,GetParent()->dir, true);
}

//强制离开
void
gplayer_imp::LeaveCarrier(int carrier_id)
{
	gplayer* pPlayer = GetParent();
	if(!pPlayer->CheckExtraState(gplayer::STATE_CARRIER)) return;

	//设置好相对绝对位置和方向
	pPlayer->ClrExtraState(gplayer::STATE_CARRIER);
	pPlayer->carrier_id = 0;
	pPlayer->rpos = A3DVECTOR(0.0f,0.0f,0.0f);
	pPlayer->rdir = 0;

	_runner->player_leave_carrier(carrier_id,GetParent()->pos,GetParent()->dir, true);
}

void
gplayer_imp::OI_OnSilentSeal()
{
	//停止当前的skill session
	if(_cur_session && _cur_session->OnSilentSeal())
	{
		_cur_session->OnSilentSeal();
	}

	//被控制了的话天华的领域去掉
	if(_filters.IsFilterExist(FILTER_INDEX_AZONE))
	{
		_filters.RemoveFilter(FILTER_INDEX_AZONE);
	}
}

bool
gplayer_imp::PlayerEnterTerritory(int mafia_id, int territory_id, unsigned char is_assist)
{
	//不是佣兵
	if(is_assist == 0)
	{
		if(mafia_id == OI_GetMafiaID()) return true;
	}
	//佣兵
	else if(is_assist == 1)
	{
		for(size_t i = 0; i < _inventory.Size(); ++i)
		{
			item & it = _inventory[i];
			if(it.GetItemType() == item_body::ITEM_TYPE_ASSIST_CARD)
			{
				size_t len;
				const void* buf = it.GetContent(len);
				if(len != sizeof(int) * 2) continue;

				int item_mafia_id = *(int*)buf;
				int item_territory_id = *(int*)((char*)buf +4);
				if(item_mafia_id != mafia_id || (item_territory_id != 0 && item_territory_id != territory_id)) continue;

				return true;
			}
		}
	}

	return false;
}

bool 
gplayer_imp::PlayerGetTerritoryAward(int mafia_id, int reward_type, int item_id, int item_count, int money)
{
	if(mafia_id != OI_GetMafiaID()) return false;
	if(item_id <= 0 || item_count < 0 || reward_type < 1 || reward_type > 4 || money < 0) return false;
	if(_player_money + money > _money_capacity || _player_money + money < _player_money) return false;

	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
	if(pile_limit <= 0) return false;
	size_t need_slot_count = item_count / pile_limit + ((item_count % pile_limit == 0) ? 0 : 1); 
	if(need_slot_count > _inventory.GetEmptySlotCount()) return false;

	return true;
}

void
gplayer_imp::TerritoryItemGet(int retcode, int item_id, int item_count, int money)
{
	TradeUnLockPlayer();
	if(retcode != 0 ||  money < 0 || item_count < 0 || item_id < 0)
	{
		return;
	}
	
	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
	size_t need_slot_count = item_count / pile_limit + ((item_count % pile_limit == 0) ? 0 : 1); 
	if(need_slot_count > _inventory.GetEmptySlotCount())
	{
		return;
	}

	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
	if(pItem->pile_limit == 1)
	{
		for(size_t i = 0; i < (size_t)item_count; i++)
		{
			element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
			item_data * pItem2 = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
			if(pItem2 == NULL) continue;

			int count = 1;
			int rst = _inventory.Push(*pItem2,count,0);
			ASSERT(rst >= 0 && count == 0);
			int state = item::Proctype2State(pItem2->proc_type);
			_runner->obtain_item(item_id,0,1,_inventory[rst].count, 0,rst,state);
			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_TASK);
			FreeItem(pItem2);
		}
	}
	else
	{
		for(int i = item_count; i > 0; )
		{
			int count = i;
			if((size_t)count > pItem->pile_limit) count = pItem->pile_limit;
			int ocount = count;
			int rst =_inventory.Push(*pItem,count,0);
			ASSERT(rst >= 0 && count == 0);
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(item_id,0,ocount,_inventory[rst].count, 0,rst,state);
			i -= ocount;
		}
	}

	if(money > 0)
	{
		GainMoney(money);
		_runner->task_deliver_money(money,GetMoney());
	}
	GLog::log(GLOG_INFO, "用户%d领取领土战奖励, item_id=%d, item_count=%d, money=%d", _parent->ID.id, item_id, item_count, money);
	return;
}


void
gplayer_imp::UseComboSkill(int combo_type, int combo_color, int combo_color_num)
{
	ASSERT(combo_type == GNET::SERIAL_SKILL_START|| combo_type == GNET::SERIAL_SKILL_SUCCEED);

	//起手技
	if(combo_type == GNET::SERIAL_SKILL_START)
	{
		if(_combo_skill.state == 0)
		{
			_combo_skill.state = 1;
		}
		else
		{
			memset(&_combo_skill.color, 0, sizeof(_combo_skill.color));
		}
	}
	//继承技
	else if(combo_type == GNET::SERIAL_SKILL_SUCCEED)
	{
		if(_combo_skill.state == 0)
		{
			_combo_skill.state = 1;
		}	

		if(combo_color <= 0 || combo_color_num <= 0) return;
		if(combo_color_num >= 5) combo_color_num = 5;

		int cur_num = 0;
		for(size_t i = 5; i > 0; --i)
		{
			if(_combo_skill.color[i-1] != 0) 
			{
				cur_num = i;
				break;
			}
		}

		if(cur_num + combo_color_num <=5)
		{
			for(int i = cur_num; i < cur_num + combo_color_num; ++i)
			{
				_combo_skill.color[i] = combo_color;
			}
		}
		else
		{
			for(int i = 0; i < 5-combo_color_num; ++i)
			{
				_combo_skill.color[i] = _combo_skill.color[i+combo_color_num+cur_num-5];
			}
			for(int i = 5-combo_color_num; i < 5; ++i) _combo_skill.color[i] = combo_color;
		}
	}
	UpdateComboSkill();
}

void
gplayer_imp::UpdateComboSkill()
{
	_runner->update_combo_state(_combo_skill.state, _combo_skill.color);
}

void
gplayer_imp::ClearComboSkill()
{
	memset(&_combo_skill, 0, sizeof(_combo_skill));
	UpdateComboSkill();
}

void
gplayer_imp::GetComboColor(int& c1, int& c2, int& c3, int& c4, int& c5)
{
	c1 = c2 = c3 = c4 = c5 = 0;
	for(int i = 0; i < 5; ++i)
	{
		if(_combo_skill.color[i] == 1) ++c1;
		else if(_combo_skill.color[i] == 2) ++c2;
		else if(_combo_skill.color[i] == 3) ++c3;
		else if(_combo_skill.color[i] == 4) ++c4;
		else if(_combo_skill.color[i] == 5) ++c5;
	}
}

bool
gplayer_imp::IsVisibleTo(int player_id)
{
	int index = gmatrix::FindPlayer(player_id);
	if(index < 0) return false;

	gplayer* pPlayer = _plane->GetPlayerByIndex(index);
	if(pPlayer->IsActived() && pPlayer->pos.horizontal_distance(_parent->pos) < 150*150
			&& ((gplayer*)_parent)->invisible_rate <= pPlayer->anti_invisible_rate)
		return true;
	return false;
}

void
gplayer_imp::SendDisappearToTeam()
{
	gplayer * pPlayer = (gplayer*)_parent;
	if(!IsInTeam() || !pPlayer->IsInvisible()) return;

	packet_wrapper h1(16);
	using namespace S2C;
	CMD::Make<CMD::object_disappear>::From(h1,pPlayer,false);
	int count = _team.GetMemberNum();
	for(int i=0;i<count;i++)
	{
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.id.id != _parent->ID.id && !IsVisibleTo(ent.id.id))
			send_ls_msg(ent.cs_index,ent.id.id,ent.cs_sid,h1.data(),h1.size());
	}

}

void
gplayer_imp::SendDisappearToTeamMember(int id,int cs_index,int cs_sid)
{                                                                                                                                                                                                
	gplayer* player = (gplayer*)_parent;                                                                                                                                                 	
	if(!IsInTeam() || !player->IsInvisible()) return;                                                                                                                                        

	packet_wrapper h1(16);                                                                                                                                                                   
	using namespace S2C;                                                                                                                                                                     
	CMD::Make<CMD::object_disappear>::From(h1,player,false);                                                                                                                                       
	if(!IsVisibleTo(id))                                                                                                                                                                        
		send_ls_msg(cs_index,id,cs_sid,h1.data(),h1.size());                                                                                                                             
} 

void
gplayer_imp::SendAppearToTeam()
{
	gplayer* pPlayer = (gplayer*)_parent;
	if(!IsInTeam() || !pPlayer->IsInvisible()) return;

	packet_wrapper h1(32);
	using namespace S2C;
	CMD::Make<CMD::player_enter_world>::From(h1,pPlayer);
	int count = _team.GetMemberNum();
	for(int i=0;i<count;i++)
	{
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.id.id != _parent->ID.id && !IsVisibleTo(ent.id.id))
			send_ls_msg(ent.cs_index,ent.id.id,ent.cs_sid,h1.data(),h1.size());
	}
}

void
gplayer_imp::SendAppearToTeamMember(int id,int cs_index,int cs_sid)
{
	gplayer* pPlayer = (gplayer*)_parent;                                                                                                                                                     
	if(!IsInTeam() || !pPlayer->IsInvisible()) return;                                                                                                                                        

	packet_wrapper h1(32);                                                                                                                                                                   
	using namespace S2C;                                                                                                                                                                     
	CMD::Make<CMD::player_enter_world>::From(h1,pPlayer);                                                                                                                                     
	if(!IsVisibleTo(id))                                                                                                                                                                        
		send_ls_msg(cs_index,id,cs_sid,h1.data(),h1.size());   
}

bool
gplayer_imp::CanSetInvisible()
{
	gplayer* pPlayer = (gplayer*)_parent;                                                                                                                                                     
	if(pPlayer->IsInvisible()) return false;
	if(pPlayer->IsMountMode()) return false;
	if(InFlying()) return false;		
	if(!_pstate.IsNormalState()) return false;
	if(_cur_session && _session_state == gactive_imp::STATE_SESSION_GATHERING) return false;

	return true;
}	

void
gplayer_imp::SetInvisible(int invisible_rate)
{
	if(!CanSetInvisible()) return;

	gplayer* pPlayer = (gplayer*)_parent;                                                                                                                                                     
	pPlayer->SetExtraState(gplayer::STATE_INVISIBLE);
	_en_point.invisible_rate += invisible_rate;
	property_policy::UpdateInvisible(this);
	pPlayer->invisible_rate = GetInvisibleRate(); 

	_runner->inc_invisible(0, pPlayer->invisible_rate);
	_runner->enter_invisible_state(true);
	ClearEnemyAggro();

	if(OI_IsCloneExist())_summonman.RemoveCloneNPC(this);

	_petman.OnSetInvisible(this);	
}

void
gplayer_imp::ClearInvisible(int invisible_rate)
{
	gplayer* pPlayer = (gplayer*)_parent;                                                                                                                                                     
	if(!pPlayer->IsInvisible()) return;

	pPlayer->ClrExtraState(gplayer::STATE_INVISIBLE);
	_runner->dec_invisible(pPlayer->invisible_rate, 0);
	_runner->enter_invisible_state(false);

	_en_point.invisible_rate -= invisible_rate;
	property_policy::UpdateInvisible(this);
	pPlayer->invisible_rate = 0;

	_petman.OnClearInvisible(this);
}

void
gplayer_imp::ClearEnemyAggro()
{
	gplayer* pPlayer = (gplayer*)_parent;
	if(!pPlayer->IsInvisible()) return;

	size_t count = _enemy_list.size();
	if(!count) return;
	XID list[MAX_PLAYER_ENEMY_COUNT];
	ENEMY_LIST::iterator it = _enemy_list.begin();
	for(size_t i = 0;it != _enemy_list.end();i ++, ++it )
	{
		MAKE_ID(list[i],it->first);
	}

	MSG msg;
	BuildMessage(msg,GM_MSG_TRY_CLEAR_AGGRO,XID(-1,-1),_parent->ID,_parent->pos,((gplayer*)_parent)->invisible_rate);
	gmatrix::SendMessage(list, list + count, msg);
}

XID
gplayer_imp::GetCloneID()
{
	return _summonman.GetCloneID();
}

XID
gplayer_imp::GetSummonID()
{
	return _summonman.GetSummonID();
}

const XID &
gplayer_imp::GetCurTarget()
{
	return ((gplayer_controller*)_commander)->GetCurTarget();
}

void
gplayer_imp::SetDimState(bool is_dim)
{
	gplayer * pPlayer = GetParent();
	if(is_dim)
	{
		pPlayer->SetExtraState(gplayer::STATE_DIM);
		_runner->enter_dim_state(is_dim);
	}
	else
	{
		pPlayer->ClrExtraState(gplayer::STATE_DIM);
		_runner->enter_dim_state(is_dim);
	}
}

void
gplayer_imp::SetCloneInfo(gsummon_imp * sImp, bool exchange)
{
	gplayer * pPlayer = GetParent();
	if(pPlayer->IsInvisible()) _filters.RemoveFilter(FILTER_INDEX_INVISIBLE);

	if(exchange)
	{
		_buff.ExchangeSubscibeTo(this, sImp->_parent->ID, false, 10);
	}
	else
	{
		_buff.ClearSubscibeList(this);
	}
	_summonman.UpdateCloneStatus(this);
}

void 
gplayer_imp::ExchangePos(const XID & who)
{
	if(_parent->IsZombie()) return;
	world::object_info info;
	if(_plane->QueryObject(who,info))
	{
		A3DVECTOR destPos = info.pos;
		object_interface oif(this);
		if(oif.CanCharge(who, destPos, 0, 0))
		{
			SendTo<0>(GM_MSG_EXCHANGE_POS,who,0);
			A3DVECTOR start = _parent->pos; 
			float height = GetWorldManager()->GetTerrain().GetHeightAt(destPos.x, destPos.z);
			if(destPos.y < height) destPos.y = height;
			destPos.y += 0.3;
			A3DVECTOR offset(destPos.x - start.x, destPos.y-start.y, destPos.z - start.z);
			StepMove(offset);
			_runner->object_charge(who.id, 0, destPos);
			_buff.ExchangeSubscibeTo(this, who, true, 0);
		}
	}
}

void
gplayer_imp::ExchangeStatus(const XID & who)
{
	if(_parent->IsZombie()) return;
	world::object_info info;
	if(_plane->QueryObject(who,info))
	{
		if(info.hp > _basic.hp)
		{
			SendTo<0>(GM_MSG_EXCHANGE_STATUS,who,_basic.hp);
			int newhp = info.hp;
			if(newhp > GetMaxHP()) newhp = GetMaxHP();
			_basic.hp = newhp;
			SetRefreshState();
		}
	}
}

void
gplayer_imp::SetAbsoluteZone(int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state)
{
	_filters.AddFilter(new azone_filter(this,FILTER_INDEX_AZONE,skill_id, skill_level, radis, count, mp_cost, force, is_helpful, var1, var2, visible_state));
}

void
gplayer_imp::PlayerUpdateCombineSkill(int skill_id, int element_count, int element_id[])
{
	int cool_index = 0;
	int cool_time = 0;
	if(player_template::GetCombineEditCD(skill_id, cool_index, cool_time))
	{
		if(cool_index != 0 && cool_time != 0)
		{
			if(!CheckCoolDown(cool_index))
			{
				_runner->error_message(S2C::ERR_COMBINE_SKILL_IS_COOLING);
				return;
			}
			SetCoolDown(cool_index, cool_time);
		}
	}
	
	if(element_count < 0) return;
	for(size_t i = 0; i < (size_t)element_count; ++i)
	{
		_skill.SetSkillElement(skill_id, i, element_id[i]);
	}
}


bool
gplayer_imp::PlayerUniqueBidRequest(int money_upperbound, int money_lowerbound)
{
	if( (money_lowerbound > money_upperbound) || (money_lowerbound <= 0) || (money_upperbound <= 0) 
	 || (money_lowerbound > MONEY_CAPACITY_BASE) || (money_upperbound > MONEY_CAPACITY_BASE) )
	{
		return false;
	}
	
	if(OI_TestSafeLock()) return false;
	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;
	if(!_pstate.CanTrade() || _cur_session || _session_list.size()) return false;
	if(!_inventory.GetEmptySlotCount()) return false;

	if(money_upperbound == money_lowerbound)
	{
		if(GetMoney() < (size_t)money_upperbound) return false;
		if(g_config.item_bid_request >0 && !IsItemExist(g_config.item_bid_request)) return false;
	}
	else
	{
		//计算所需扣除的金钱和物品,total_money的值就是算一个等差数列求和
		int item_need = money_upperbound - money_lowerbound + 1;//所需消耗的bid的物品的个数
		
		int64_t money_need = (int64_t)((int64_t)item_need * ((int64_t)money_lowerbound + (int64_t)money_upperbound))/2;
		if(money_need > MONEY_CAPACITY_BASE)
			return false;

		size_t total_money = money_need;
		
		if(GetMoney() < total_money) return false;
		if(g_config.item_bid_request >0 && (_inventory.CountItemByID(g_config.item_bid_request) < item_need) ) return false;
	}

	if(!GNET::SendUniqueBidRequest(_parent->ID.id, money_upperbound, money_lowerbound, object_interface(this))) return false; 

	// 活跃度更新
	EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_LEAST_AUCTION);

	GLog::log(GLOG_INFO,"用户 %d 执行了唯一最低价拍卖操作", _parent->ID.id); 
	return true;
}

void
gplayer_imp::PlayerUniqueBidResult(int result, int money_upperbound, int money_lowerbound)
{
	TradeUnLockPlayer();
	if(result != 0) return;
	if( (money_lowerbound > money_upperbound) || (money_lowerbound <= 0) || (money_upperbound <= 0) 
	 || (money_lowerbound > MONEY_CAPACITY_BASE) || (money_upperbound > MONEY_CAPACITY_BASE) )
	{
		return;
	}
	
	//计算所需扣除的金钱和物品,total_money的值就是算一个等差数列求和
	int item_need = money_upperbound - money_lowerbound + 1;//所需消耗的bid的物品的个数

	int64_t money_need = (int64_t)((int64_t)item_need * ((int64_t)money_lowerbound + (int64_t)money_upperbound))/2;
	if(money_need > MONEY_CAPACITY_BASE)
		return;

	size_t total_money = money_need;


	//扣除金钱
	if(total_money) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=5:hint=%d",_parent->ID.id,_db_magic_number,total_money,g_config.item_bid_reward);
	SpendMoney(total_money);
	_runner->spend_money(total_money);
	
	//扣除相应的物品
	if(g_config.item_bid_request > 0)
	{
		TakeOutItem(g_config.item_bid_request, item_need);
		//检查是否需要记录消费值
		CheckSpecialConsumption(g_config.item_bid_request, item_need);
	}
	if(g_config.item_bid_reward > 0)
	{
		DeliverItem(g_config.item_bid_reward, 1, false, 0, ITEM_INIT_TYPE_TASK);
	}
	return;
}

bool
gplayer_imp::PlayerUniqueBidGet()
{
	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;
	if(_inventory.GetEmptySlotCount() <=0) return false;

	//发送请求数据
	if(!GNET::SendUniqueBidGetItem(_parent->ID.id, object_interface(this))); 
	{
		return false;
	}
	GLog::log(GLOG_INFO,"用户 %d 执行了唯一最低价拍卖领取奖励操作", _parent->ID.id); 
	return true;
}

void
gplayer_imp::PlayerUniqueBidReward(int result, int itemid)
{
	TradeUnLockPlayer();
	if(result != 0) return;

	if(itemid > 0)
	{
		DeliverItem(itemid, 1, false, 0, ITEM_INIT_TYPE_TASK);
	}
	
	GLog::log(GLOG_INFO,"用户 %d 领取了唯一最低价拍卖奖励, itemid = %d", _parent->ID.id, itemid); 
}

void
gplayer_imp::AddCirclePoint(unsigned int point)
{
	int circle_id = GetCircleID();
	if(circle_id == 0) return;

	GMSV::SendAddCirclePoint(circle_id, point);	
}

void
gplayer_imp::QueryCircleGroupData(const void* pData, int nSize)
{
	GMSV::SendCirlceAsyncData(_parent->ID.id, pData, nSize);
}

void
gplayer_imp::QueryFriendNum(int taskid)
{
	GMSV::SendQueryFriendNum(_parent->ID.id, taskid);
}

void
gplayer_imp::ReceiveFriendNum(int taskid, int number)
{
	PlayerTaskInterface  task_if(this);
	OnTaskReceiveFriendNumData(&task_if,taskid,number);
}

bool
gplayer_imp::ChargeTeleStation(int item_index, int item_id, int stone_index, int stone_id)
{
	if(item_index < 0 || item_index > (int)_inventory.Size()) return false;
	if(stone_index < 0 || stone_index > (int)_inventory.Size()) return false;

	item & it1 = _inventory[item_index];
	if(it1.type == -1 || it1.body == NULL || item_id != it1.type ) return false;
	if(it1.GetItemType() != item_body::ITEM_TYPE_TELESTATION) return false;
	if(stone_id == -1 || _inventory[stone_index].type != stone_id) return false;

	DATA_TYPE dt;
	const TELEPORTATION_STONE_ESSENCE & ess = *(TELEPORTATION_STONE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(stone_id, ID_SPACE_ESSENCE,dt);
	if(dt != DT_TELEPORTATION_STONE_ESSENCE || &ess == NULL)
	{
		return false;
	}
	int day = ess.day;
	if(day <= 0) return false;

	if(!it1.body->Charge(&it1, this, item_index, stone_index, day))
	{
		return false;
	}
	
	//检查是否需要记录消费值
	CheckSpecialConsumption(stone_id, 1);

	UseItemLog(_inventory[stone_index],1);
	_inventory.DecAmount(stone_index, 1);
	_runner->player_drop_item(IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);
	return true;
}


bool 
gplayer_imp::IsTerritoryOwner(int id)
{
	return gmatrix::IsTerritoryOwner(OI_GetMafiaID(), id);
}

int
gplayer_imp::GetTerritoryScore()
{
	return gmatrix::GetTerritoryScore(OI_GetMafiaID());
}

void
gplayer_imp::PlayerTryChangeDS(char type)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_CHANGE_DS))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	
	GMSV::SendPlayerTryChangeDS(_parent->ID.id, type); 
}

void
gplayer_imp::PlayerChangeDS(char type)
{
	if(!_pstate.CanLogout()) 
	{
		GMSV::SendPlayerChangeDSRe( GNET::ERR_CHGS_STATUSINVALID, GetParent()->ID.id, GetParent()->cs_index, GetParent()->cs_sid, type); 
		return;
	}
	LeaveAbnormalState();

	GLog::log(GLOG_INFO,"用户%d执行跨服换线登出逻辑, type=%d", _parent->ID.id, type);

	_team.PlayerLogout();
	CircleOfDoomPlayerLogout();
	class UserInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		gplayer * _player;
		int _userid;
		char _type;

		public:
		UserInfoWriteBack(gplayer * pPlayer, char type) :_player(pPlayer),_userid(pPlayer->ID.id), _type(type)
		{}

		virtual void OnTimeOut()
		{
			//目前并没有重新发送存盘请求
			GLog::log(GLOG_ERR, "换线时保存%d数据超时", _userid );
			OnPutRole(1);
		}
		virtual void OnFailed()
		{	
			//不会受到这个命令 
			GLog::log(GLOG_ERR, "换线时保存%d数据失败", _userid );
			OnPutRole(2);
		}
		virtual void OnPutRole(int retcode)
		{
			_player->Lock();
			if(_player->ID.id !=  _userid || _player->login_state != gplayer::WAITING_LOGOUT) 
			{
				//忽略错误，直接返回
				_player->Unlock();
				GLog::log(GLOG_INFO,"写入用户%d发生状态不一致错误 id2:%d state:%d",_userid,_player->ID.id,_player->login_state);
				delete this;
				return ;
			}
			ASSERT(_player->imp);
			GLog::log(GLOG_INFO,"写入用户%d数据完成(%d)，发出跨服换线请求",_userid,retcode);
			//将player对象删除
			GMSV::SendPlayerChangeDSRe( retcode?GNET::ERR_CHGS_DB_ERROR:GNET::ERR_CHGS_SUCCESS, _player->ID.id, _player->cs_index, _player->cs_sid, _type); 
			_player->imp->_commander->Release(); 
			_player->Unlock();
			record_leave_server(_userid);
			delete this;
		}
	};

	PlayerLeaveWorld();
	_runner->leave_world();

	//还要考虑断线逻辑
	//用户进入断线逻辑，并开始存盘
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->login_state = gplayer::WAITING_LOGOUT;

	ClearSession();
	ClearSpiritSession();

	//先将用户从世界中删除 在commander->Release中会见用户从管理器中移出
	slice * pPiece = pPlayer->pPiece;
	if(pPiece) _plane->RemovePlayer(pPlayer);
	pPlayer->pPiece = NULL;
	try
	{
		//写入磁盘 这里并不保存将来的存盘坐标，这里保存这个坐标是为了能够返回给delivery
		_trashbox.IncChangeCounter();
		UserInfoWriteBack * pCallback = new UserInfoWriteBack(pPlayer, type); 
		user_save_data((gplayer*)_parent,pCallback, 2, DBMASK_PUT_ALL);
	}
	catch(...)
	{
		ASSERT(false);
	}
}


bool
gplayer_imp::PlayerFlyTaskPos(int task_id, int npc_id, bool is_npc_task)
{
	if(task_id < 0 || ( is_npc_task && (npc_id < 0) ) ) return false;

	if(!IsKingdomPlayer())
	{
		//检查是否有飞天
		//国王帮成员不需要飞天符
		int paper_pos = _inventory.FindByType(0, item_body::ITEM_TYPE_TOWN_SCROLL_PAPER);
		if(paper_pos == -1) return false;
	}
	
	PlayerTaskInterface  task_if(this);
	
	//检查是否有这个task
	if(!task_if.HasTask(task_id)) return false;

	//检查这个task_id是否有这个npc_id
	if(is_npc_task && !task_if.CheckNPCAppearInTask(task_id, npc_id)) return false;
	
	//查找npc_id 对应的坐标id
	A3DVECTOR pos;
	int tag;
	if(is_npc_task)
	{
		if(!task_if.GetTaskNPCPos(npc_id, pos, tag)){return false;}
	}
	else
	{
		ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID( task_id );
		if(!pTempl || !pTempl->m_bAutoMoveForReachFixedSite || (pTempl->m_enumMethod != enumTMReachSite)){return false;}
		tag = pTempl->m_ulReachSiteId;
		pos = A3DVECTOR( pTempl->m_AutoMoveDestPos.v[0], pTempl->m_AutoMoveDestPos.v[1], pTempl->m_AutoMoveDestPos.v[2] );
	}
	
	//源位置
	world_manager *pSrcManager = _plane->GetWorldManager();
	if (!pSrcManager || pSrcManager->IsIdle() || pSrcManager->GetWorldLimit().no_flytask) return false;

	//目标位置是否可传送
	world_manager * pManager = gmatrix::FindWorld(tag);
	//基地内虽配置了 nocouponjump 此处也允许任务传送
	if(!pManager || pManager->IsIdle() || (!pManager->IsFacBase() && pManager->GetWorldLimit().nocouplejump)) return false;
	
	//加入一个session
	class op : public session_general_operation::operation
	{
		float _x;
		float _y;
		float _z;
		int _tag;
		bool _is_kingdom_player;
		public:
		op(float x, float y, float z, int tag, bool is_kingdom_player): _x(x),_y(y),_z(z), _tag(tag), _is_kingdom_player(is_kingdom_player)
		{}

		virtual int GetID()
		{
			return S2C::GOP_TELESTATION;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(3.0f);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			item_list &inv = pImp->GetInventory();

			if(!_is_kingdom_player)
			{
				int paper_pos = inv.FindByType(0, item_body::ITEM_TYPE_TOWN_SCROLL_PAPER);
				if(paper_pos >= 0)
				{
					if(pImp->LongJump(A3DVECTOR(_x,_y,_z), _tag))
					{
						//检查是否需要记录消费值
						pImp->CheckSpecialConsumption(inv[paper_pos].type, 1);

						pImp->DecInventoryItem(paper_pos, 1, S2C::DROP_TYPE_TAKEOUT);

						// 更新活跃度
						pImp->EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FEITIANFU);
						return;
					}
					else
					{
						pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
						return;
					}
				}
				else
				{
					pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
				}
			}
			else
			{
				if(pImp->LongJump(A3DVECTOR(_x,_y,_z), _tag))
				{
					return;
				}
				else
				{
					pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
					return;
				}
			}
		}
	};
	
	session_general_operation * pSession = new session_general_operation(this,  new op(pos.x, pos.y, pos.z, tag, IsKingdomPlayer())); 
	AddSession(pSession);
	StartSession();
	return true;
}

int
gplayer_imp::GetZoneCash()
{
	if(g_config.item_zone_cash <= 0) return 0;
	return GetInventory().CountItemByID(g_config.item_zone_cash); 
}

void
gplayer_imp::UseZoneCash(int count)
{
	ASSERT(count > 0);
	TakeOutItem(g_config.item_zone_cash, count);
	//检查是否需要记录消费值
	CheckSpecialConsumption(g_config.item_zone_cash, count);
}

bool
gplayer_imp::PlayerFlyActivityPos(int id)
{
	if(id <= 0) return false;

	//检查是否有飞天
	int paper_pos = _inventory.FindByType(0, item_body::ITEM_TYPE_TOWN_SCROLL_PAPER);
	if(paper_pos == -1) return false;
	
	
	//查找活动id对应的地址
	A3DVECTOR pos;
	int tag;
	if(!gmatrix::GetActivityInfo(id, tag, pos)) return false;
	
	//目标位置是否可传送
	world_manager * pManager = gmatrix::FindWorld(tag);
	if(!pManager || pManager->IsIdle() || pManager->GetWorldLimit().nocouplejump) return false;

	//加入一个session
	class op : public session_general_operation::operation
	{
		float _x;
		float _y;
		float _z;
		int _tag;
		public:
		op(float x, float y, float z, int tag): _x(x),_y(y),_z(z), _tag(tag)
		{}

		virtual int GetID()
		{
			return S2C::GOP_TELESTATION;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(3.0f);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			item_list &inv = pImp->GetInventory();
			int paper_pos = inv.FindByType(0, item_body::ITEM_TYPE_TOWN_SCROLL_PAPER);
			if(paper_pos >= 0)
			{
				if(pImp->LongJump(A3DVECTOR(_x,_y,_z), _tag))
				{
					pImp->DecInventoryItem(paper_pos, 1, S2C::DROP_TYPE_TAKEOUT);

					// 更新活跃度
					pImp->EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_FEITIANFU);

					return;
				}
				else
				{
					pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
					return;
				}
			}
			else
			{
				pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
			}
		}
	};
	
	session_general_operation * pSession = new session_general_operation(this,  new op(pos.x, pos.y, pos.z, tag));
	AddSession(pSession);
	StartSession();
	return true;
}

struct UpgradeItem
{
	int id;
	int index;
	int cnt;
	UpgradeItem() : id(-1), index(-1), cnt(0){}
};

//Add by Houjun 2011-03-09, 宝石相关操作
//宝石升品	
bool gplayer_imp::UpgradeGemLevel(int gem_id, int gem_index, int upgradeItemId[12], int upgradeItemIndex[12])
{
	item_list& inv = GetInventory();
	item & it = inv[gem_index];
	if(it.type == -1 || it.type != gem_id)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	if(it.IsLocked())
	{
		return false;
	}
	
	itemdataman & dataman = gmatrix::GetDataMan();
	DATA_TYPE dt;
	const GEM_ESSENCE& gemEss = *(const GEM_ESSENCE*)dataman.get_data_ptr(gem_id,ID_SPACE_ESSENCE,dt);
	if(!&gemEss || dt != DT_GEM_ESSENCE) return false;

	//检查宝石等级是否有效
	int level = gemEss.grade;
	if(level < 1 || level >= 10) return false;
	
	if(level >= g_config.gem_upgrade_upper_limit)
	{
		return false;
	}
	
	float prob = 0;
	
	//收集装备物品信息(包裹位置和数量)
	abase::vector<UpgradeItem, abase::fast_alloc<> > upgradeItems;
	for(int i = 0; i < 12; i ++)
	{
		if(upgradeItemId[i] <= 0 || upgradeItemIndex[i] < 0) continue;
		
		const GEM_DUST_ESSENCE& gemDustEss = *(const GEM_DUST_ESSENCE*)dataman.get_data_ptr(upgradeItemId[i], ID_SPACE_ESSENCE, dt);
		if(!&gemDustEss || dt != DT_GEM_DUST_ESSENCE) return false;

		prob += (gemEss.quality == 0 ? gemDustEss.upgrade_prob[level-1].ordinary_upgrade_prob : gemDustEss.upgrade_prob[level-1].perfect_upgrade_prob);
	
		int rst = -1;
		for(size_t j = 0; j < upgradeItems.size(); j ++)
		{
			UpgradeItem& uItem = upgradeItems[j];
			if(uItem.id == upgradeItemId[i] && uItem.index == upgradeItemIndex[i])
			{
				rst = j;
				break;
			}
		}
		if(rst != -1)
		{
			upgradeItems[rst].cnt ++;
		}
		else
		{
			UpgradeItem item;
			item.id = upgradeItemId[i];
			item.index = upgradeItemIndex[i];
			item.cnt ++;
			upgradeItems.push_back(item);
		}
	}

	//检测宝石分成是否存在
	for(size_t i = 0; i < upgradeItems.size(); i ++)
	{
		UpgradeItem& uIt = upgradeItems[i];
		if(!IsItemExist(uIt.index, uIt.id, uIt.cnt))
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
	}
	if(gemEss.fee_upgrade > 0 && GetMoney() < (size_t)gemEss.fee_upgrade)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	//需要一个包裹栏放入升品后的宝石
	if(inv.GetEmptySlotCount() < 1)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	//进行概率判断是否升品
	if(prob < 1.0f)
	{
		_runner->error_message(S2C::ERR_GEM_UPGRADE_LEVEL_PROB_NOT_ENOUGH);
		return false;
	}
	static float adjust_ratio[10] = {0.9f, 0.85f, 0.8f, 0.75f, 0, 0, 0, 0, 0, 0};
	int upgrade_type = abase::RandUniform() < (prob - 1.0f) * adjust_ratio[gemEss.grade - 1] ? 1 : 0;
	
	const GEM_CONFIG& gemConfig = *(const GEM_CONFIG*)dataman.get_data_ptr(gemEss.gem_config_id, ID_SPACE_CONFIG, dt);
	if(!&gemConfig || dt != DT_GEM_CONFIG) return false;

	int nextLevelId = (upgrade_type == 0 ? gemConfig.gem_id[level].ordinary_gem_id : gemConfig.gem_id[level].perfect_gem_id);
	if(!CreateItem(nextLevelId, 1, it.IsBind()))
	{
		return false;
	}
	_runner->gem_notify(gem_id, 0);

	//删除原来品级的宝石
	UseItemLog(inv, gem_index, 1);
	inv.DecAmount(gem_index, 1);
	_runner->player_drop_item(IL_INVENTORY, gem_index, gem_id, 1 ,S2C::DROP_TYPE_USE);

	//扣除宝石升品道具
	for(size_t i = 0; i < upgradeItems.size(); i ++)
	{	
		UpgradeItem& uIt = upgradeItems[i];
		UseItemLog(inv, uIt.index, uIt.cnt);
		inv.DecAmount(uIt.index, uIt.cnt);
		_runner->player_drop_item(IL_INVENTORY, uIt.index, uIt.id, uIt.cnt ,S2C::DROP_TYPE_USE);
	}

	//扣除升品手续费
	if(gemEss.fee_upgrade > 0)
	{
		SpendMoney(gemEss.fee_upgrade);
		_runner->spend_money(gemEss.fee_upgrade);
	}
	return true;
}

//宝石精练
bool gplayer_imp::UpgradeGemQuality(int gem_id, int gem_index, int upgradeItemId, int upgradeItemIndex)
{
	static float success_prob[9] = {0.9f, 0.66f, 0.33f, 0.03f, 0.01f, 0, 0, 0, 0};		

	item_list& inv = GetInventory();
	item & it = inv[gem_index];
	if(it.type == -1 || it.type != gem_id)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	if(it.IsLocked())
	{
		return false;
	}
	
	if(upgradeItemIndex < 0 || upgradeItemId < 1)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	bool valid = false;
	for(int i = 0; i < 3; i ++)
	{
		if(g_config.id_gem_refine_article[i] == upgradeItemId)
		{
			valid = true;
			break;
		}
	}
	
	if(!valid)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	item & upgradeIt = inv[upgradeItemIndex];
	if(upgradeIt.type == -1 || upgradeIt.type != upgradeItemId)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	if(inv.GetEmptySlotCount() < 1)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	if(g_config.fee_gem_refine > 0 && GetMoney() < (size_t)g_config.fee_gem_refine)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	DATA_TYPE dt;
	const GEM_ESSENCE& gemEss = *(const GEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(gem_id, ID_SPACE_ESSENCE, dt);
	if(!&gemEss || dt != DT_GEM_ESSENCE) return false;

	if(gemEss.quality == 1) return false;
	int level = gemEss.grade;
	if(level < 1 && level > 10) return false;

	if(abase::RandUniform() < success_prob[level-1])
	{
		const GEM_CONFIG& gemConfig = *(const GEM_CONFIG*)gmatrix::GetDataMan().get_data_ptr(gemEss.gem_config_id, ID_SPACE_CONFIG, dt);
		if(!&gemConfig || dt != DT_GEM_CONFIG) return false;
		int nextQualityId = gemConfig.gem_id[level - 1].perfect_gem_id;
		if(!CreateItem(nextQualityId, 1, it.IsBind()))
		{
			return false;
		}

		//删除原来品级的宝石
		UseItemLog(inv, gem_index, 1);
		inv.DecAmount(gem_index, 1);
		_runner->player_drop_item(IL_INVENTORY, gem_index, gem_id, 1, S2C::DROP_TYPE_USE);
	}

	//扣除精练道具
	UseItemLog(inv, upgradeItemIndex, 1);
	inv.DecAmount(upgradeItemIndex, 1);
	_runner->player_drop_item(IL_INVENTORY, upgradeItemIndex, upgradeItemId, 1, S2C::DROP_TYPE_USE);

	//扣除金钱
	if(g_config.fee_gem_refine > 0)
	{
		SpendMoney(g_config.fee_gem_refine);
		_runner->spend_money(g_config.fee_gem_refine);
	}
	return true;
}

//宝石萃取
bool gplayer_imp::ExtraceGem(int gem_id, int gem_index)
{
	item_list& inv = GetInventory();
	item & it = inv[gem_index];
	if(it.type == -1 || it.type != gem_id)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	if(it.IsLocked())
	{
		return false;
	}

	if(g_config.fee_gem_extract > 0 && GetMoney() < (size_t)g_config.fee_gem_extract)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	DATA_TYPE dt;
	const GEM_ESSENCE& gemEss = *(const GEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(gem_id, ID_SPACE_ESSENCE, dt);
	if(!&gemEss || dt != DT_GEM_ESSENCE) return false;

	const GEM_EXTRACT_CONFIG& geConfig = *(const GEM_EXTRACT_CONFIG*)gmatrix::GetDataMan().get_data_ptr(gemEss.gem_extract_config_id, ID_SPACE_CONFIG, dt);
	if(!&geConfig || dt != DT_GEM_EXTRACT_CONFIG) return false;

	size_t cnt = 0;
	for(int i = 0; i < 3; i ++)
	{
		cnt += geConfig.gem_dust_config[i].gem_dust_num;
	}

	if(inv.GetEmptySlotCount() < cnt)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	for(int i = 0; i < 3; i ++)
	{
		if(abase::RandUniform() < geConfig.gem_dust_config[i].gem_dust_prob)
		{
			if(!CreateItem(geConfig.gem_dust_config[i].gem_dust_id, geConfig.gem_dust_config[i].gem_dust_num, false))
			{
				return false;
			}
		}
	}

	//删除原来宝石
	UseItemLog(inv, gem_index, 1);
	inv.DecAmount(gem_index, 1);
	_runner->player_drop_item(IL_INVENTORY, gem_index, gem_id, 1 ,S2C::DROP_TYPE_USE);

	//扣除金钱
	if(g_config.fee_gem_extract > 0)
	{
		SpendMoney(g_config.fee_gem_extract);
		_runner->spend_money(g_config.fee_gem_extract);
	}
	return true;
}

//宝石融合
bool gplayer_imp::SmeltGem(int src_gem_id, int src_gem_index, int dest_gem_id, int dest_gem_index, int smelt_item_id, int smelt_item_index)
{
	item_list& inv = GetInventory();
	item & srcIt = inv[src_gem_index];
	if(srcIt.type == -1 || srcIt.type != src_gem_id)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}
	if(srcIt.IsLocked())
	{
		return false;
	}

	item & destIt = inv[dest_gem_index];
	if(destIt.type == -1 || destIt.type != dest_gem_id)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	if(destIt.IsLocked())
	{
		return false;
	}

	if(smelt_item_id < 1 || smelt_item_index < 0)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	bool valid = false;
	for(int i = 0; i < 3; i ++)
	{
		if(g_config.id_gem_smelt_article[i] == smelt_item_id)
		{
			valid = true;
			break;
		}
	}
	if(!valid)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	item & smeltIt = inv[smelt_item_index];
	if(smeltIt.type == -1 || smeltIt.type != smelt_item_id)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	if(inv.GetEmptySlotCount() < 2) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	if(g_config.fee_gem_smelt > 0 && GetMoney() < (size_t)g_config.fee_gem_smelt)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	DATA_TYPE dt;
	const GEM_ESSENCE& srcGemEss = *(const GEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(src_gem_id, ID_SPACE_ESSENCE, dt);
	if(!&srcGemEss || dt != DT_GEM_ESSENCE) return false;

	const GEM_ESSENCE& destGemEss = *(const GEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(dest_gem_id, ID_SPACE_ESSENCE, dt);
	if(!&destGemEss || dt != DT_GEM_ESSENCE) return false;
	
	const GEM_CONFIG& srcGemConfig = *(const GEM_CONFIG*)gmatrix::GetDataMan().get_data_ptr(srcGemEss.gem_config_id, ID_SPACE_CONFIG, dt);
	if(!&srcGemConfig || dt != DT_GEM_CONFIG) return false;

	const GEM_CONFIG& destGemConfig = *(const GEM_CONFIG*)gmatrix::GetDataMan().get_data_ptr(destGemEss.gem_config_id, ID_SPACE_CONFIG, dt);
	if(!&destGemConfig || dt != DT_GEM_CONFIG) return false;

	int srcLevel = srcGemEss.grade;
	int srcQuality =srcGemEss.quality;
	int destLevel = destGemEss.grade;
	int destQuality = destGemEss.quality;
	
	if(srcLevel < 1 || srcLevel > 10 || destLevel < 1 || destLevel > 10 || srcLevel <= destLevel)
	{
		return false;
	}
	
	if(srcQuality != destQuality) 
	{
		_runner->gem_notify(src_gem_id, S2C::GEM_SMELT_FAIL_SAME_QUALITY);
		return false;
	}

	int src_rst_gem_id = srcQuality == 0 ? srcGemConfig.gem_id[destLevel - 1].ordinary_gem_id : srcGemConfig.gem_id[destLevel - 1].perfect_gem_id;
	int dest_rst_gem_id = destQuality == 0 ? destGemConfig.gem_id[srcLevel - 1].ordinary_gem_id : destGemConfig.gem_id[srcLevel - 1].perfect_gem_id;

	if(src_rst_gem_id <= 0 || dest_rst_gem_id <= 0) return false;

	if(!CreateItem(src_rst_gem_id, 1, true))
	{
		return false;
	}
	GLog::log(GLOG_INFO,"用户%d熔炼宝石%d, 生成新的原始宝石%d", _parent->ID.id, src_gem_id, src_rst_gem_id);		
	//销毁原始宝石
	UseItemLog(_inventory[src_gem_index],1);
	_inventory.DecAmount(src_gem_index, 1);
	_runner->player_drop_item(IL_INVENTORY,src_gem_index, src_gem_id, 1 ,S2C::DROP_TYPE_USE);
	
	if(!CreateItem(dest_rst_gem_id, 1, true))
	{
		return false;
	}			
	GLog::log(GLOG_INFO,"用户%d熔炼宝石%d, 生成新的转移宝石%d", _parent->ID.id, dest_gem_id, dest_rst_gem_id);		
	//销毁转移后的宝石
	UseItemLog(_inventory[dest_gem_index],1);
	_inventory.DecAmount(dest_gem_index, 1);
	_runner->player_drop_item(IL_INVENTORY,dest_gem_index, dest_gem_id, 1 ,S2C::DROP_TYPE_USE);
	
	//扣除熔炼道具
	UseItemLog(_inventory[smelt_item_index],1);
	_inventory.DecAmount(smelt_item_index, 1);
	_runner->player_drop_item(IL_INVENTORY,smelt_item_index, smelt_item_id, 1 ,S2C::DROP_TYPE_USE);

	//扣除熔炼手续费
	if(g_config.fee_gem_smelt > 0)
	{
		SpendMoney(g_config.fee_gem_smelt);
		_runner->spend_money(g_config.fee_gem_smelt);
	}
	return true;
}

//创建一个新物品,item_id, item_cnt, 返回包裹栏index
bool gplayer_imp::CreateItem(int item_id, int item_cnt, bool isBind)
{
	if(item_id <= 0 || item_cnt <= 0) return false;
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * pItem = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));
	if(pItem)
	{
		int item_count = item_cnt;
		int rst =_inventory.Push(*pItem,item_count,0);
		FreeItem(pItem);
		if(rst >= 0 && item_count == 0)
		{
			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_TASK);
			if(isBind) 
			{
				BindItem(rst, item_id);
			}
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(item_id,0,1,_inventory[rst].count, 0,rst,state);
			return true;
		}
	}
	return false;
}
//Add end.


//-----------------------------阵法相关---------------------------------------------------------------
bool
gplayer_imp::SetCircleOfDoomPrepare(float radius, int faction, int max_member_num, int skill_id)
{
	bool need_cooldown = false;
	if(skill_id == 3454) need_cooldown = true;

	if(need_cooldown && !CheckCoolDown(COOLDOWN_INDEX_CIRCLE_OF_DOOM)) return false;

	int self_faction_id = -1;
	bool verified = false;
	switch( faction )
	{
		case object_interface::TEAM_CIRCLE_OF_DOOM:
			{
				if( IsInTeam() )
				{
					self_faction_id = GetTeamID();
					verified = true;
				}
				else
				{
					_circle_of_doom.SetSingle(skill_id);
				}
			}
			break;

		case object_interface::MAFIA_CIRCLE_OF_DOOM:
			{
				if( (self_faction_id = OI_GetMafiaID()) > 0 )
				{
					verified = true;
				}
				else
				{
					_circle_of_doom.SetSingle(skill_id);
				}
			}
			break;

		case object_interface::FAMILY_CIRCLE_OF_DOOM:
			{
				if( (self_faction_id = OI_GetFamilyID()) > 0 )
				{
					verified = true;
				}
				else
				{
					_circle_of_doom.SetSingle(skill_id);
				}
			}
			break;

		default:
			__PRINTF("阵法SetCircleOfDoomPrepare()参数错误！\n");
			break;

	}

	if(verified)
	{
		//下面三行顺序不能变,AddFilter要放最后。
		_circle_of_doom.SetSponsorPrepare(max_member_num, skill_id);
		_runner->notify_circleofdoom_start(_parent->ID.id, (char)faction, self_faction_id);
		_filters.AddFilter( new cod_sponsor_filter(this, radius, faction, self_faction_id, GetParent()->IsFlyMode(), FILTER_INDEX_COD_SPONSOR, skill_id) );
		if(need_cooldown)_filters.AddFilter(new cod_cooldown_filter(this, 60, FILTER_INDEX_COD_COOLDOWN));
	}
	return true;
}

void
gplayer_imp::SetCircleOfDoomStop()
{
	//如果不是阵眼不能执行后面的操作
	if( !_circle_of_doom.IsSponsor() )  
	{
		return ;
	}

	if( _circle_of_doom.IsInCircleOfDoom() )
	{
		_circle_of_doom.SetSponsorStop(_parent->ID, _parent->pos);
		_runner->notify_circleofdoom_stop(_parent->ID.id);
		gplayer * pPlayer = GetParent();
		pPlayer->ClrExtraState(gplayer::STATE_CIRCLEOFDOOM);
	}
}

void
gplayer_imp::CircleOfDoomClearMember()
{
	if( _circle_of_doom.IsSponsor() )
	{
		//下面三行顺序不能改，是他们逼我这么写的!
		_circle_of_doom.BroadcastMemberMsg(_parent->ID, _parent->pos, GM_MSG_CIRCLE_OF_DOOM_STOP);
		_circle_of_doom.ClearMember();
		SendCircleOfDoomInfo();
	}
	else
	{
		__PRINTF("player不是Sponsor但是调用了CircleOfDoomClearMember()!\n");
	}
}

void
gplayer_imp::SetCircleOfDoomStartup()
{
	//如果不是阵眼不能执行后面的操作
	if( !_circle_of_doom.IsSponsor() )  
	{
		return ;
	}

	if( _circle_of_doom.IsNormalRunning() )
	{
		_circle_of_doom.SetSponsorStartup(_parent->ID, _parent->pos);
		gplayer * pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CIRCLEOFDOOM);
		SendCircleOfDoomInfo();
	}
	else if( _circle_of_doom.IsSingle() )
	{
		gplayer * pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CIRCLEOFDOOM);
		SendCircleOfDoomInfo();
	}
}

//和delivery断开连接
void
gplayer_imp::CircleOfDoomPlayerDisconnect()
{
	CircleOfDoomPlayerLogout();
}

void
gplayer_imp::CircleOfDoomPlayerLogout()
{
	if( !_circle_of_doom.IsInCircleOfDoom() )
	{
		return;
	}

	if( _circle_of_doom.IsSponsor() )
	{
		SetCircleOfDoomStop();
	}
	else
	{
		LeaveCircleOfDoom();
		int sponsor_id = 0;
		if( _circle_of_doom.GetSponsorID(sponsor_id) )
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_LEAVE, XID(GM_TYPE_PLAYER, sponsor_id), _parent->ID, _parent->pos);
			gmatrix::SendMessage(msg);
		}
		else
		{
			__PRINTF("获取sponsor_id失败，CircleOfDoomPlayerLogout()\n");
		}
	}
}

int
gplayer_imp::GetPlayerInCircleOfDoom( std::vector<XID> &playerlist )
{
	return _circle_of_doom.GetMemberXIDList(playerlist, _parent->ID);
}

int  gplayer_imp::GetCircleMemberCnt()
{
	return _circle_of_doom.GetMemberCnt();
}

void 
gplayer_imp::SendCircleOfDoomInfo()
{
	abase::vector<int> member_list;
	_circle_of_doom.GetMemberIDList(member_list, _parent->ID.id);
	_runner->notify_circleofdoom_info(_circle_of_doom.GetSkillid(), _parent->ID.id, member_list.size(), member_list.begin());
}

void 
gplayer_imp::HandleMsgCircleOfDoomPrepare(const MSG & msg)
{
	//如果在摆摊则不能参加阵法，死亡也不能
	if( _pstate.IsMarketState() || IsDead() ) 
	{
		return;
	}


	if( !_circle_of_doom.IsInCircleOfDoom() && !(((gplayer*)_parent)->IsInvisible()) )
	{
		ASSERT( msg.content_length >= sizeof(cod_sponsor_filter::prepare_param) );
		cod_sponsor_filter::prepare_param param;
		memcpy(&param, msg.content, msg.content_length);

		if( GetParent()->IsFlyMode() != param.is_flymode )
		{
			return;
		}

		int sponsor_faction_id = param.faction_id;
		bool verified = false;
		bool need_cooldown = false;
		if(param.skill_id == 3454) need_cooldown = true;

		if(need_cooldown && !CheckCoolDown(COOLDOWN_INDEX_CIRCLE_OF_DOOM)) return;

		switch( param.faction )
		{
			case object_interface::TEAM_CIRCLE_OF_DOOM:
				{
					if( IsInTeam() && (sponsor_faction_id == ((gplayer*)_parent)->team_id) )
					{
						verified = true;	
					}
				}
				break;

			case object_interface::MAFIA_CIRCLE_OF_DOOM:
				{
					if( OI_GetMafiaID() > 0 && (sponsor_faction_id == ((gplayer*)_parent)->id_mafia) )
					{
						verified = true;
					}
				}
				break;

			case object_interface::FAMILY_CIRCLE_OF_DOOM:
				{
					if( OI_GetFamilyID() > 0 && (sponsor_faction_id == ((gplayer*)_parent)->id_family) )
					{
						verified = true;
					}
				}
				break;

			default:
				{
					__PRINTF("收到的GM_MSG_CIRCLE_OF_DOOM_PREPARE消息参数有误!\n");
				}
				break;
		}

		if(verified && _circle_of_doom.Insert(msg.source.id) )
		{
			cod_member_filter::cod_param cod_param;
			cod_param.radius = param.radius;
			cod_param.is_flymode = param.is_flymode;
			cod_param.faction = param.faction;
			
			//下面两行顺序不能变,AddFilter要放最后。
			_circle_of_doom.SetMemberPrepare();
			_filters.AddFilter(new cod_member_filter(this, msg.source, msg.pos, cod_param, FILTER_INDEX_COD_MEMBER));
			if(need_cooldown)_filters.AddFilter(new cod_cooldown_filter(this, 60, FILTER_INDEX_COD_COOLDOWN));
		}
	}
}

void 
gplayer_imp::HandleMsgCircleOfDoomStartup(const MSG & msg)
{
	if( _circle_of_doom.IsInCircleOfDoom() && _circle_of_doom.IsMember(msg.source.id) )	
	{
		_circle_of_doom.SetMemberStartup();
	}
	else
	{
		_circle_of_doom.SetMemberStop();
		__PRINTF("阵法成员收到消息有误！startup! id=%d, msg.sourceid=%d\n", _parent->ID.id, msg.source.id);
	}
}

void
gplayer_imp::HandleMsgCircleOfDoomStop(const MSG & msg)
{
	if( _circle_of_doom.IsInCircleOfDoom() && _circle_of_doom.IsMember(msg.source.id) )	
	{
		_filters.RemoveFilter(FILTER_INDEX_COD_MEMBER);
		_circle_of_doom.SetMemberStop();
	}
	else
	{
		_circle_of_doom.SetMemberStop();
		__PRINTF("阵法成员收到消息有误！stop! id=%d, msg.sourceid=%d\n", _parent->ID.id, msg.source.id);
	}
}

void
gplayer_imp::HandleMsgCircleOfDoomMemberEnter(const MSG & msg)
{
	if( _circle_of_doom.IsStop() )
	{
		MSG tempmsg;
		BuildMessage(tempmsg, GM_MSG_CIRCLE_OF_DOOM_STOP, msg.source, _parent->ID, _parent->pos);
		gmatrix::SendMessage(tempmsg);

		__PRINTF("阵法未开启或者已经结束，但收到GM_MSG_CIRCLE_OF_DOOM_ENTER消息!\n");
	}
	else if( _circle_of_doom.IsNormalRunning() )
	{
		if( _circle_of_doom.IsOverMaxMemberNum() )
		{
			MSG tempmsg;
			BuildMessage(tempmsg, GM_MSG_CIRCLE_OF_DOOM_STOP, msg.source, _parent->ID, _parent->pos);
			gmatrix::SendMessage(tempmsg);
		}
		else
		{
			if( _circle_of_doom.Insert(msg.source.id) )
			{
				if( _circle_of_doom.IsStartup() )
				{
					SendCircleOfDoomInfo();
				}
			}
			else
			{
				__PRINTF("阵法成员已经存在！\n");
			}
		}
	}
}

void
gplayer_imp::HandleMsgCircleOfDoomMemberLeave(const MSG & msg)
{
	if( _circle_of_doom.IsStop() )
	{
		__PRINTF("阵法未开启或者已经结束，但收到GM_MSG_CIRCLE_OF_DOOM_LEAVE消息!\n");
	}
	else if( _circle_of_doom.IsNormalRunning() )
	{
		if( _circle_of_doom.IsMember(msg.source.id) )
		{
			_circle_of_doom.Remove(msg.source.id);
			if( _circle_of_doom.IsStartup() )
			{
				SendCircleOfDoomInfo();
			}
		}
		else
		{
			__PRINTF("阵法成员不存在！\n");
		}
	}
}

void
gplayer_imp::HandleMsgCircleOfDoomQuery(const MSG & msg)
{
	if( _circle_of_doom.IsStop() )
	{
		__PRINTF("阵法未开启或者已经结束，但收到GM_MSG_CIRCLE_OF_DOOM_QUERY消息!\n");
	}
	else
	{
		abase::vector<int> member_list;
		_circle_of_doom.GetMemberIDList(member_list, _parent->ID.id);
		int sid = msg.param;
		int cs_index = msg.param2;

		_runner->notify_circleofdoom_info_single(msg.source, cs_index, sid, _circle_of_doom.GetSkillid(), _parent->ID.id, 
				member_list.size(), member_list.begin());
	}
}
////////////////////////阵法相关函数结束///////////////////////////////////////

//Add by Houjun 2011-07-04, 囚牢相关逻辑
void gplayer_imp::AddLimitCycleArea(int owner, const A3DVECTOR& center, float radius)
{
	CycleAreaMap::iterator it = _limit_area.find(owner);
	if(it != _limit_area.end())
	{
		cycle_area& ca = it->second;
		ca.center = center;
		ca.squared_radius = radius * radius;
	}
	else
	{	
		cycle_area ca;
		ca.center = center;
		ca.squared_radius = radius * radius;
		_limit_area[owner] = ca;
	}
	//DumpLimitCycleArea();
	_runner->player_move_cycle_area(0, 1, owner, radius, center);
}

void gplayer_imp::AddPermitCycleArea(int owner, const A3DVECTOR& center, float radius)
{		
	CycleAreaMap::iterator it = _permit_area.find(owner);
	if(it != _permit_area.end())
	{
		cycle_area& ca = it->second;
		ca.center = center;
		ca.squared_radius = radius * radius;
	}
	else
	{	
		cycle_area ca;
		ca.center = center;
		ca.squared_radius = radius * radius;
		_permit_area[owner] = ca;
	}
	//DumpPermitCycleArea();
	_runner->player_move_cycle_area(0, 0, owner, radius, center);
}

void gplayer_imp::RemoveLimitCycleArea(int owner)
{
	CycleAreaMap::iterator it = _limit_area.find(owner);
	if(it != _limit_area.end())
	{
		_limit_area.erase(it);
		_runner->player_move_cycle_area(1, 1, owner, -1, A3DVECTOR(0, 0, 0));
	}
	//DumpLimitCycleArea();
}

void gplayer_imp::RemovePermitCycleArea(int owner)
{
	CycleAreaMap::iterator it = _permit_area.find(owner);
	if(it != _permit_area.end())
	{
		_permit_area.erase(it);
		_runner->player_move_cycle_area(1, 0, owner, -1, A3DVECTOR(0, 0, 0));
	}
	//DumpPermitCycleArea();
}

void gplayer_imp::ClearLimitCycleArea()
{
	_limit_area.clear();
	_runner->player_move_cycle_area(2, 1, -1, -1, A3DVECTOR(0, 0, 0));
}

void gplayer_imp::ClearPermitCycleArea()
{
	_permit_area.clear();
	_runner->player_move_cycle_area(2, 0, -1, -1, A3DVECTOR(0, 0, 0));
}

bool gplayer_imp::CheckMoveArea(const A3DVECTOR& pos)
{	
	CycleAreaMap::iterator it = _permit_area.begin();
	for(; it != _permit_area.end(); ++ it)
	{
		cycle_area& c = it->second;
		if(pos.horizontal_distance(c.center) > c.squared_radius)
		{
			return false;
		}
	}
	
	it = _limit_area.begin();
	for(; it != _limit_area.end(); ++ it)
	{
		cycle_area& c = it->second;
		if(pos.horizontal_distance(c.center) <= c.squared_radius)
		{
			return false;
		}
	}
	return true;
}

bool gplayer_imp::IsLimitCycleAreaExist(int owner)
{
	CycleAreaMap::iterator it = _limit_area.find(owner);
	return it != _limit_area.end();
}

bool gplayer_imp::IsPermitCycleAreaExist(int owner)
{
	CycleAreaMap::iterator it = _permit_area.find(owner);
	return it != _permit_area.end();
}

void gplayer_imp::DumpLimitCycleArea()
{
	__PRINTF("=====================Limit Cycle Area===========================\n");
	CycleAreaMap::iterator it = _limit_area.begin();
	for(; it != _limit_area.end(); ++ it)
	{
		cycle_area& c = it->second;
		__PRINTF("%d: s_radius: %4f, pos: (%4f, %4f. %4f)\n", it->first, c.squared_radius, c.center.x, c.center.y, c.center.z);
	}
	__PRINTF("================================================================\n");
}

void gplayer_imp::DumpPermitCycleArea()
{
	__PRINTF("====================Permit Cycle Area===========================\n");
	CycleAreaMap::iterator it = _permit_area.begin();
	for(; it != _permit_area.end(); ++ it)
	{
		cycle_area& c = it->second;
		__PRINTF("%d: s_radius: %4f, pos: (%4f, %4f. %4f)\n", it->first, c.squared_radius, c.center.x, c.center.y, c.center.z);
	}
	__PRINTF("================================================================\n");
}

//Add end.

//Add by Houjun 2011-07-18, 副本相关逻辑
bool gplayer_imp::CheckOpenRoomCondition(int raid_id, const TRANSCRIPTION_CONFIG& raid_config)
{
	//开房间和加入副本所需物品，判断两种物品相同的情况
	int openItemId = raid_config.open_room_item_id;
	int openItemCnt = raid_config.open_room_item_count;
	if(openItemId <= 0 || openItemCnt <= 0)
	{
		openItemId = 0;
		openItemCnt = 0;
	}

	int joinItemId = raid_config.required_item_id;
	int joinItemCnt = raid_config.required_item_count;
	if(joinItemId <= 0 || joinItemId <= 0)
	{
		joinItemId = 0;
		joinItemCnt = 0;
	}

	if(openItemId > 0 && openItemId == joinItemId)
	{
		openItemCnt += joinItemCnt;
		joinItemId = 0;
		joinItemCnt = 0;
	}

	item_list& inv = GetInventory();	
	bool openflag = (openItemId == 0 && openItemCnt == 0);
	bool joinflag = (joinItemId == 0 && joinItemCnt == 0);

	for(size_t i = 0; i < inv.Size(); i ++)
	{
		if(!openflag && inv[i].type == openItemId)
		{
			openItemCnt -= inv[i].count;
			if(openItemCnt <= 0)
			{
				openflag = true;
				continue;
			}
		}
		if(!joinflag && inv[i].type == joinItemId)
		{
			joinItemCnt -= inv[i].count;
			if(joinItemCnt <= 0)
			{
				joinflag = true;
				continue;
			}
		}
		if(openflag && joinflag)
		{
			break;
		}
	}
	if(!openflag || !joinflag)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}
	return CheckJoinRoomConditionCommon(raid_id, raid_config);
}

bool gplayer_imp::CheckJoinRoomCondition(int raid_id, const TRANSCRIPTION_CONFIG& raid_config)
{
	//检查加入副本物品
	int joinItemId = raid_config.required_item_id;
	int joinItemCnt = raid_config.required_item_count;
	
	if(joinItemId > 0 && joinItemCnt > 0)
	{
		item_list& inv = GetInventory();
		bool flag = false;
		for(size_t i = 0; i < inv.Size(); i ++)
		{
			if(!flag && inv[i].type == joinItemId)
			{
				joinItemCnt -= inv[i].count;
				if(joinItemCnt <= 0)
				{
					flag = true;
					break;
				}
			}
		}
		if(!flag)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
	}
	return CheckJoinRoomConditionCommon(raid_id, raid_config);
}

bool gplayer_imp::CheckJoinRoomConditionCommon(int raid_id, const TRANSCRIPTION_CONFIG& raid_config)
{
	if(raid_config.max_finish_count != 0 && GetRaidCounter(raid_id) >= raid_config.max_finish_count)
	{
		printf("今天已经太多次参加副本 %d\n", GetRaidCounter(raid_id));
		return false;
	}
	
	if(_basic.level < raid_config.player_min_level || _basic.level > raid_config.player_max_level)
	{
		_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return false;
	}

	short cls = GetParent()->GetClass();
	if(!object_base_info::CheckCls(cls, raid_config.character_combo_id, raid_config.character_combo_id2))
	{
		printf("副本进入条件检查，职业条件不符合: %d\n", cls);
		return false;
	}
	if(raid_config.required_race == 0 && !(cls <= 0x1F || (cls > 0x3F && cls <= 0x5F)))
	{
		printf("副本进入条件检查，种族条件不符合: %d\n", cls);
		return false;
	}
	if(raid_config.required_race == 1 && !((cls > 0x1F && cls <= 0x3F) || (cls > 0x5F && cls <= 0x7F)))
	{
		printf("副本进入条件检查，种族条件不符合: %d\n", cls);
		return false;
	}
	if(GetRebornCount() < raid_config.renascence_count)
	{
		printf("副本进入条件检查，转生次数不符合: RebornCount=%d, required=%d\n", GetRebornCount(), raid_config.renascence_count);
		return false;
	}
	if(raid_config.god_devil_mask && !(GetCultivation() & raid_config.god_devil_mask))
	{
		printf("副本进入条件检查，造化条件不符合: %d\n", GetCultivation());
		return false;
	}
	if(raid_config.required_money > 0 && GetMoney() < (size_t)raid_config.required_money)
	{
		printf("副本进入条件检查，金钱不足 money=%d, required=%d\n", GetMoney(), raid_config.required_money);
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	for(int i = 0; i < 4; i ++)
	{
		int id = raid_config.required_reputation[i].reputation_type;
		int value = raid_config.required_reputation[i].reputation_value; 
		if(id >= 0 && value > 0 && GetRegionReputation(id) < value)
		{
			printf("副本进入条件检查，声望条件不符合 id=%d, value=%d, required=%d\n", id, GetRegionReputation(id), value);
			return false;
		}
	}
	
	return true;
}

bool gplayer_imp::CheckOpenRoomCondition(int raid_id, const TOWER_TRANSCRIPTION_CONFIG& raid_config)
{
	//开房间和加入副本所需物品，判断两种物品相同的情况
	int openItemId = raid_config.open_room_item_id;
	int openItemCnt = raid_config.open_room_item_count;
	if(openItemId <= 0 && openItemCnt <= 0)
	{
		openItemId = 0;
		openItemId = 0;
	}

	int joinItemId = raid_config.required_item_id;
	int joinItemCnt = raid_config.required_item_count;
	if(joinItemId <= 0 && joinItemId <= 0)
	{
		joinItemId = 0;
		joinItemCnt = 0;
	}

	if(openItemId == joinItemId)
	{
		openItemCnt += joinItemCnt;
		joinItemCnt = 0;
	}
	
	item_list& inv = GetInventory();	
	if(openItemId > 0 || joinItemId > 0)
	{
		bool openflag = false;
		bool joinflag = false;
		for(size_t i = 0; i < inv.Size(); i ++)
		{
			if(openItemId > 0)
			{
				if(inv[i].type == openItemId && openItemCnt > 0)
				{
					openItemCnt -= inv[i].count;
					if(openItemCnt <= 0)
					{
						openflag = true;
						continue;
					}
				}
			}
			else 
			{
				openflag = true;
			}
			
			if(joinItemId > 0 )
			{
				if(inv[i].type == joinItemId && joinItemCnt > 0)
				{
					joinItemCnt -= inv[i].count;
					if(joinItemCnt <= 0)
					{
						joinflag = true;
						continue;
					}
				}
			}
			else
			{
				joinflag = true;
			}
		}
		if(!openflag || !joinflag)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
	}
	return CheckJoinRoomConditionCommon(raid_id, raid_config);
}

bool gplayer_imp::CheckJoinRoomCondition(int raid_id, const TOWER_TRANSCRIPTION_CONFIG& raid_config)
{
	//检查加入副本物品
	int joinItemId = raid_config.required_item_id;
	int joinItemCnt = raid_config.required_item_count;
	
	if(joinItemId > 0 && joinItemCnt > 0)
	{
		item_list& inv = GetInventory();
		bool flag = false;
		for(size_t i = 0; i < inv.Size(); i ++)
		{
			if(inv[i].type == joinItemId && joinItemCnt > 0)
			{
				joinItemCnt -= inv[i].count;
				if(joinItemCnt <= 0)
				{
					flag = true;
					break;
				}
			}
		}
		if(!flag)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
	}
	return CheckJoinRoomConditionCommon(raid_id, raid_config);
}

bool gplayer_imp::CheckJoinRoomConditionCommon(int raid_id, const TOWER_TRANSCRIPTION_CONFIG& raid_config)
{
	if(GetRaidCounter(raid_id) >= raid_config.max_finish_count)
	{
		printf("今天已经太多次参加副本 %d\n", GetRaidCounter(raid_id));
		return false;
	}
	
	if(_basic.level < raid_config.player_min_level || _basic.level > raid_config.player_max_level)
	{
		_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return false;
	}

	short cls = GetParent()->GetClass();
	if(!object_base_info::CheckCls(cls, raid_config.character_combo_id, raid_config.character_combo_id2))
	{
		printf("副本进入条件检查，职业条件不符合: %d\n", cls);
		return false;
	}
	if(raid_config.required_race == 0 && !(cls <= 0x1F || (cls > 0x3F && cls <= 0x5F)))
	{
		printf("副本进入条件检查，种族条件不符合: %d\n", cls);
		return false;
	}
	if(raid_config.required_race == 1 && !((cls > 0x1F && cls <= 0x3F) || (cls > 0x5F && cls <= 0x7F)))
	{
		printf("副本进入条件检查，种族条件不符合: %d\n", cls);
		return false;
	}
	if(GetRebornCount() < raid_config.renascence_count)
	{
		printf("副本进入条件检查，转生次数不符合: reborn_cnt=%d, required=%d\n", GetRebornCount(), raid_config.renascence_count);
		return false;
	}
	if(raid_config.god_devil_mask && !(GetCultivation() & raid_config.god_devil_mask))
	{
		printf("副本进入条件检查，造化条件不符合: %d\n", GetCultivation());
		return false;
	}
	if(raid_config.required_money > 0 && GetMoney() < (size_t)raid_config.required_money)
	{
		printf("副本进入条件检查，金钱不足?money=%d, required=%d\n", GetMoney(), raid_config.required_money);
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	for(int i = 0; i < 4; i ++)
	{
		int id = raid_config.required_reputation[i].reputation_type;
		int value = raid_config.required_reputation[i].reputation_value; 
		if(id >= 0 && value > 0 && GetRegionReputation(id) < value)
		{
			printf("副本进入条件检查，声望条件不符合 id=%d, value=%d, required=%d\n", id, GetRegionReputation(id), value);
			return false;
		}
	}	
	return true;
}


bool gplayer_imp::CheckEnterRaid(int raid_id, int raid_template_id, int raid_type, const void* buf, size_t size)
{
	DATA_TYPE dt;
	const void* data = gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(!data)
	{
		printf("无效的副本模板: %d\n", raid_template_id);
		return false;
	}

	if(!_pstate.CanLogout() || ((raid_type != RT_COLLISION && raid_type != RT_COLLISION2 
	&& raid_type != RT_SEEK && raid_type != RT_CAPTURE) && IsCombatState()) || GetWorldManager()->IsFacBase())
	{
		printf("副本无法进入，玩家当前状态无法换线\n");
		return false;
	}

	if(dt == DT_TRANSCRIPTION_CONFIG && raid_id == ((const TRANSCRIPTION_CONFIG*)data)->map_id)
	{
		return CheckJoinRoomCondition(raid_id, *(const TRANSCRIPTION_CONFIG*)data);
	}
	else if(dt == DT_TOWER_TRANSCRIPTION_CONFIG && raid_id == ((const TOWER_TRANSCRIPTION_CONFIG*)data)->map_id)
	{
		if(size != sizeof(tower_raid_data))
		{
			printf("爬塔副本:%d初始化参数错误, raid_template_id=%d\n", raid_id, raid_template_id);
			return false;
		}
		const tower_raid_data* trd = (const tower_raid_data*)buf;
		return (GetTowerLevel() == -1 || trd->init_tower_level <= GetTowerLevel()) && CheckJoinRoomCondition(raid_id, *(const TOWER_TRANSCRIPTION_CONFIG*)data);
	}
	return true;	
}

bool gplayer_imp::CheckEnterBase()
{
	//要加一个session
	class op : public session_general_operation::operation
	{
		public:
		op()
		{}
		virtual int GetID()
		{
			return S2C::GOP_ENTER_FAC_BASE;
		}
		virtual bool NeedBoardcast()
		{
			return true;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(10);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			pImp->EnterBase();
		}
		virtual bool OnAttacked(gactive_imp * obj)
		{	
			return true;
		}
	};
	AddSession(new session_general_operation(this, new op()));
	StartSession();
	return true;
}

bool gplayer_imp::EnterBase()
{
	if(!_pstate.IsNormalState() || IsCombatState()/* || GetWorldManager()->IsFacBase()*/)
	{
		printf("玩家 %d 无法进入基地，玩家当前状态无法换线\n", GetParent()->ID.id);
		return false;
	}
	if (OI_GetMafiaID() <= 0)
		return false;
	return GNET::SendFacBaseEnter(GetParent()->ID.id);
}

bool gplayer_imp::OpenRaidRoom(int raid_id, int raid_template_id, char can_vote, char difficulty, const char* roomname, size_t roomname_len)
{
	DATA_TYPE dt;
	const void* data = gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(!data)
	{
		printf("无效的副本模板: %d\n", raid_template_id);
		return false;
	}

	if(dt == DT_TRANSCRIPTION_CONFIG && raid_id == ((const TRANSCRIPTION_CONFIG*)data)->map_id && CheckOpenRoomCondition(raid_id, *(const TRANSCRIPTION_CONFIG*)data))
	{
		const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*)data; 
		GNET::SendRaidOpen(_parent->ID.id, raid_id, raid_config.open_room_item_id, raid_config.open_room_item_count, can_vote, difficulty, roomname, roomname_len, object_interface(this));
		return true;
	}
	else if(dt == DT_TOWER_TRANSCRIPTION_CONFIG && raid_id == ((const TOWER_TRANSCRIPTION_CONFIG*)data)->map_id && CheckOpenRoomCondition(raid_id, *(const TOWER_TRANSCRIPTION_CONFIG*)data))
	{
		const TOWER_TRANSCRIPTION_CONFIG& raid_config = *(const TOWER_TRANSCRIPTION_CONFIG*)data; 
		GNET::SendRaidOpen(_parent->ID.id, raid_id, raid_config.open_room_item_id, raid_config.open_room_item_count, can_vote, difficulty, roomname, roomname_len, object_interface(this));
		return true;
	}
	printf("无效的副本模板: %d\n", raid_template_id);
	return false;
}

bool gplayer_imp::JoinRaidRoom(int raid_id, int raid_template_id, int room_id, int raid_faction)
{
	DATA_TYPE dt;
	const void* data = gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(!data)
	{
		printf("无效的副本模板: %d\n", raid_template_id);
		return false;
	}

	//如果是帮战副本，观战者需要检查金钱
	if( MAFIA_BATTLE_RAID_ID == raid_template_id && RF_VISITOR == raid_faction && MAFIA_BATTLE_RAID_VISITOR_COST > 0 && GetMoney() < (size_t)MAFIA_BATTLE_RAID_VISITOR_COST)
	{
		_runner->error_message(S2C::ERR_ENOUGH_MONEY_IN_TRASH_BOX);
		return false;
	}

	if(dt == DT_TRANSCRIPTION_CONFIG && raid_id == ((const TRANSCRIPTION_CONFIG*)data)->map_id && CheckJoinRoomCondition(raid_id, *(const TRANSCRIPTION_CONFIG*)data))
	{
		GNET::SendRaidJoin(_parent->ID.id, raid_id, room_id, GetParent()->cs_index, raid_faction);
		return true;
	}
	else if(dt == DT_TOWER_TRANSCRIPTION_CONFIG && raid_id == ((const TOWER_TRANSCRIPTION_CONFIG*)data)->map_id && CheckJoinRoomCondition(raid_id, *(const TOWER_TRANSCRIPTION_CONFIG*)data))
	{
		GNET::SendRaidJoin(_parent->ID.id, raid_id, room_id, GetParent()->cs_index, raid_faction);
		return true;
	}

	printf("无效的副本模板: %d\n", raid_template_id);
	return false;
}

void gplayer_imp::SaveRaidCountersDB(archive & ar)
{
	int version = 0;
	ar << version;
	ar << _time_raid_counters_reset;
	ar << _raid_counters.size();

	RaidCounterMap::iterator it = _raid_counters.begin();
	for(; it != _raid_counters.end(); ++ it)
	{
		ar.push_back(&it->second, sizeof(raid_counter));
	}
	//DumpRaidCounter("Save Raid Counter");
}

void gplayer_imp::LoadRaidCountersDB(const void * buf, size_t size)
{
	if(size <= 0) return;
	raw_wrapper ar(buf, size);
	int version = 0;
	ar >> version;
	if(version == 0)
	{
		ar >> _time_raid_counters_reset;
		size_t size = 0;
		ar >> size;
		for(size_t i = 0; i < size; i ++)
		{
			raid_counter rc;
			ar.pop_back(&rc, sizeof(rc));
			if(rc.raid_id > 0)
			{
				_raid_counters[rc.raid_id] = rc;
			}
		}
	}
	ResetRaidCounters();
	//DumpRaidCounter("Load Raid Counter");
}

void gplayer_imp::SaveRaidCountersClient(archive & ar)
{
	ar << _raid_counters.size();
	RaidCounterMap::iterator it = _raid_counters.begin();
	for(; it != _raid_counters.end(); ++ it)
	{
		raid_counter& rc = it->second;
		ar << rc.raid_id << rc.counter;
	}

}

void gplayer_imp::IncRaidCounter(int raid_id)
{
	ResetRaidCounters();
	RaidCounterMap::iterator it = _raid_counters.find(raid_id);
	if(it != _raid_counters.end())
	{
		it->second.counter ++;
	}
	else
	{
		raid_counter rc;
		rc.raid_id = raid_id;
		rc.counter = 1;
		_raid_counters[raid_id] = rc;
	}
	_runner->player_raid_counter();
}

void gplayer_imp::ResetRaidCounters()
{
	time_t curr_sec = (size_t)g_timer.get_systime();
	struct tm curr, last;
	localtime_r((time_t*)&curr_sec, &curr);
	localtime_r((time_t*)&_time_raid_counters_reset, &last);

	if(curr_sec < _time_raid_counters_reset || curr.tm_year > last.tm_year || (curr.tm_year == last.tm_year && curr.tm_yday > last.tm_yday))
	{   
		_time_raid_counters_reset = curr_sec;
		_raid_counters.clear();
		_runner->player_raid_counter();
	}
}

int gplayer_imp::GetRaidCounter(int raid_id)
{
	ResetRaidCounters();
	RaidCounterMap::iterator it = _raid_counters.find(raid_id);
	if(it != _raid_counters.end())
	{
		return it->second.counter;
	}
	return 0;
}

void gplayer_imp::ClearRaidCounter()
{
	_raid_counters.clear();
	_runner->player_raid_counter();
}

void gplayer_imp::DumpRaidCounter(const char* name)
{
	printf("----------------------%s------------------------\n", name);
	RaidCounterMap::iterator it = _raid_counters.begin();
	for(; it != _raid_counters.end(); ++ it)
	{
		printf("raid_id=%d, count=%d\n", it->first, it->second.counter);
	}
	printf("------------------------------------------------\n\n");
}
//Add end.

void gplayer_imp::FollowTarget(const XID& target, float speed, float stop_dist)
{

	session_follow_target *pSession = new session_follow_target(this);
	pSession->SetTarget(target,stop_dist,32.f,2.0f, speed);
	AddStartSession(pSession);
}

void gplayer_imp::StopFollowTarget()
{
	ClearSession();
	//ClearSpecSession(act_session::SS_MASK_FOLLOW_TARGET);
}

void gplayer_imp::SetIdPullingMe(int target, bool on)
{
	id_pulling_me = on ? target : -1;
	SetBePulledState(on);
}

void gplayer_imp::SetIdSpiritDragMe(int target, bool on)
{
	id_spirit_drag_me = on ? target : -1;
	SetBeSpiritDraggedState(on);
}

//---------------------------------活跃度相关函数------------------------------------

int gplayer_imp::GetNextMidNightTime(int cur_time)
{
	struct tm tm1;
	time_t t = (time_t)cur_time;
	localtime_r(&t, &tm1);
	tm1.tm_mday += 1;
	tm1.tm_hour = 0;
	tm1.tm_min = 0;
	tm1.tm_sec = 0;
	return (int)mktime(&tm1);
}

void gplayer_imp::SaveSalePromotion(archive & ar)
{
	int version = 0;
	ar << version;
	ar << _sale_promotion_info.taskids.size();
	for (std::set<int>::iterator it = _sale_promotion_info.taskids.begin(); it != _sale_promotion_info.taskids.end(); ++it)
   	{
		ar << *it;	
	}
}

void gplayer_imp::LoadSalePromotion(archive & ar)
{
	if (ar.size() == 0) {
		// 忽略，不用做初始化	
		return;
	}
	int version;
	ar >> version;
	if (version == 0) 
	{
		size_t size;
		ar >> size;	
		for (size_t i = 0; i < size; ++i) {
			int cur;
			ar >> cur;
			_sale_promotion_info.taskids.insert(cur);	
		}
	}
}

void gplayer_imp::SaveLiveness(archive & ar)
{
	// version 0: version | grade_timestamp[4] | clear_timestamp | cur_point | reward_size | reward_ids
	// version 1: 追加 cont_days, cont_days_timestamp
	int version = 1;
	ar << version;
	for (int i = 0; i < 4; i++) {
		ar << _liveness_info.grade_timestamp[i];	
	}
	ar << _liveness_info.midnight_clear_timestamp;
	ar << _liveness_info.cur_point;
	ar << _liveness_info.classifyid2ids.size();
	for (std::multimap<int, int>::iterator it = _liveness_info.classifyid2ids.begin(); it != _liveness_info.classifyid2ids.end(); ++it) 
	{
		ar << it->second;	
	}
	ar << _liveness_info.cont_days;
	ar << _liveness_info.cont_days_timestamp;
}
void gplayer_imp::LoadLiveness(archive & ar)
{
	int cur_time = g_timer.get_systime();
	if (ar.size() == 0) {
		// 初始化需要设置到时时间
		_liveness_info.midnight_clear_timestamp = GetNextMidNightTime(cur_time);
		return;
	}

	// version 0: version | grade_timestamp[4] | clear_timestamp | cur_point | reward_size | reward_ids
	// version 1: 追加 cont_days, cont_days_timestamp
	liveness_cfg* pCfg = player_template::GetLivenessCfg();	
	int version;
	ar >> version;
	if (version <= 1) {
		for (int i = 0; i < 4; i++) {
			ar >> _liveness_info.grade_timestamp[i];	
		}
		ar >> _liveness_info.midnight_clear_timestamp;
		ar >> _liveness_info.cur_point;
		size_t size; 	
		ar >> size;
		for (size_t i = 0; i < size; i++) {
			int liveness_id;
			ar >> liveness_id;	
			std::map<int, int>::iterator ret_it = pCfg->index2typeid.find(liveness_id);
			if (ret_it == pCfg->index2typeid.end()) continue; // 配置有修改???

			_liveness_info.classifyid2ids.insert(std::make_pair(ret_it->second, ret_it->first));
		}
	}

	if (version == 1) {
		ar >> _liveness_info.cont_days;
		ar >> _liveness_info.cont_days_timestamp;
	}

	if (_liveness_info.midnight_clear_timestamp <= cur_time) // 需要清除
	{
		memset(_liveness_info.grade_timestamp, 0, sizeof(_liveness_info.grade_timestamp));
		_liveness_info.classifyid2ids.clear();
		_liveness_info.cur_point = 0;
	}

	// 设置新的午夜清除的时间
	_liveness_info.midnight_clear_timestamp = GetNextMidNightTime(cur_time);
}

void gplayer_imp::OnHeartbeatLiveness(int cur_time)
{
	if (cur_time < _liveness_info.midnight_clear_timestamp) return; // 不到时间

	// 过了零点，清零
	memset(_liveness_info.grade_timestamp, 0, sizeof(_liveness_info.grade_timestamp));
	_liveness_info.classifyid2ids.clear();
	_liveness_info.cur_point = 0;

	// 设置新的午夜清除的时间
	_liveness_info.midnight_clear_timestamp = GetNextMidNightTime(cur_time);

	// 通告客户端[这里是清除后的零分通告]
	_runner->liveness_notify();

	// 检查vip卡是否开启中
	gplayer *pPlayer = GetParent();
	if(pPlayer->IsVIPState()) {
		// 更新活跃度[VIP卡状态]
		EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_VIP_CARD_OPEN);
	}
}

void gplayer_imp::TakeLivenessAward(int grade)
{
	// 一共四档次
	if (grade < 0 || grade >= 4) return;

	liveness_cfg* pCfg = player_template::GetLivenessCfg();	
	int gradePoints[4] = { 25, 50, 75, 100};
	if (_liveness_info.cur_point < gradePoints[grade]) return; // 分数不够
	if (_liveness_info.grade_timestamp[grade]) return; // 已经领取过了
	
	// 发物品
	int giftItemId = pCfg->gifts[grade];
	const item_data * pGiftItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(giftItemId);
	if (pGiftItem == NULL || pGiftItem->pile_limit == 0) return; // 策划配错了物品
	if (!InventoryHasSlot(1)) return; // 没有空位置
	DeliverItem(giftItemId, 1, true, 0, ITEM_INIT_TYPE_TASK);

	// 设置时间戳
	_liveness_info.grade_timestamp[grade] = g_timer.get_systime();

	// 通告客户端
	_runner->liveness_notify();
}
bool gplayer_imp::EventUpdateLiveness(int liveness_type, int param)
{
	liveness_cfg* pCfg = player_template::GetLivenessCfg();	

	int liveness_id = -1;	
	if (liveness_type == LIVENESS_UPDATE_TYPE_TASK_DONE) {
		std::map<int, int>::iterator it = pCfg->taskindexs.find(param);
		if (it != pCfg->taskindexs.end())
			liveness_id = it->second;
	} else if (liveness_type == LIVENESS_UPDATE_TYPE_TRANSFER_MAP) {
		std::map<int, int>::iterator it = pCfg->mapindexs.find(param);
		if (it != pCfg->mapindexs.end())
			liveness_id = it->second;
	} else if (liveness_type == LIVENESS_UPDATE_TYPE_SPECIAL) {
		std::map<int, int>::iterator it = pCfg->specialindexs.find(param);
		if (it != pCfg->specialindexs.end())
			liveness_id = it->second;
	}

	if (liveness_id == -1) return false; // 配置
	int liveness_type_id = -1;	
	{
		std::map<int, int>::iterator it = pCfg->index2typeid.find(liveness_id);
		if (it != pCfg->index2typeid.end())
			liveness_type_id = it->second;
	}
	if (liveness_type_id == -1) return false;
	if (liveness_type_id < 0 || liveness_type_id >= 15) return false;
	
	liveness_type_info& cur_type_info = pCfg->type_infos[liveness_type_id];		

	{
		std::multimap<int, int>::iterator it = _liveness_info.classifyid2ids.find(liveness_type_id);
		if (it == _liveness_info.classifyid2ids.end()) {
			int old_point = _liveness_info.cur_point;
			// 第一次触发此类，加分	
			_liveness_info.cur_point += cur_type_info.score;
			GMSV::SendFacActChange(OI_GetMafiaID(), cur_type_info.score);

			// 超过100分，发送成就事件
			if (old_point < 100 && _liveness_info.cur_point >= 100) {
				int cur_time = g_timer.get_systime();
				object_interface oif(this);	
				// 1. 100分成就事件
				GetAchievementManager().OnLiveness100P(oif);

				// 2. 连续7天活跃度100分事件
				if (_liveness_info.midnight_clear_timestamp - _liveness_info.cont_days_timestamp > 24 * 60 * 60)	
				{ // 今天以前
					if (_liveness_info.midnight_clear_timestamp - _liveness_info.cont_days_timestamp < 2 * 24 * 60 * 60)	
					{ // 昨天
						_liveness_info.cont_days_timestamp = cur_time;	
						_liveness_info.cont_days += 1;	
						if (_liveness_info.cont_days >= 7 || (_liveness_info.debug_needday > 0 && _liveness_info.cont_days >= _liveness_info.debug_needday)) {
							GetAchievementManager().OnLiveness7Day100P(oif);
							_liveness_info.cont_days_timestamp = 0;	
							_liveness_info.cont_days = 0;
						}
					} else { // 昨天以前，断了重新蓄
						_liveness_info.cont_days_timestamp = cur_time;	
						_liveness_info.cont_days = 1;
					}
				} // else 今天及以后忽略，可能是调试导致的
			}
		}

		bool is_recorded = false; // 此id是否完成
		for (; it != _liveness_info.classifyid2ids.end(); ++it) {
			if (it->first == liveness_type_id && it->second == liveness_id) {
				is_recorded = true;	
				break;
			}	
		}

		if (!is_recorded)
		{// 添加到已完成
			_liveness_info.classifyid2ids.insert(std::make_pair(liveness_type_id, liveness_id));
		}
	}

	// 通告客户端
	_runner->liveness_notify();
	return true;
}

//---------------------------------UI传送相关函数------------------------------------
// values --> masks
inline void EncodeToMask(abase::static_set<int>& values, vector<unsigned int>& masks)
{
	for (abase::static_set<int>::iterator it = values.begin(); it != values.end(); it++) {
		int v = *it;
		if (v <= 0) continue;
		ASSERT(v <= 65536);

		size_t mask_pos = (v / (8 * sizeof(unsigned int)) + ((v % (8 * sizeof(unsigned int))) ? 1 : 0)) - 1;
		unsigned int mask = 1 << ((v - 1) %  (8 * sizeof(unsigned int)));

		size_t cur_len = masks.size();
		if (mask_pos >= cur_len)
		{
			masks.reserve(mask_pos + 1);
			for (size_t i = cur_len; i <= mask_pos; i++) 
			{
				masks.push_back(0);
			}
		}
		masks[mask_pos] |= mask;
	}
}

// masks --> values
inline void DecodeFromMask(abase::static_set<int>& values, vector<unsigned int>& masks)
{
	int base = 0;
	for (vector<unsigned int>::iterator it = masks.begin(); it < masks.end(); it++, base += 8 * sizeof(unsigned int)) {
		unsigned int v = *it;
		for (size_t i = 0; i < 8 * sizeof(unsigned int); i++) {
			unsigned int cur_bit = 1 << i;
			if (v & cur_bit) {
				values.insert(base + i + 1);
			}
		}
	}
}

// index start from 1
bool gplayer_imp::ActiveUITransferIndex(int index) 
{
	if (index <= 0) return false;

	// 这里不检查是否重复插入
	_ui_transfer_info.opened_indexes.insert(index);
	_runner->ui_transfer_opened_notify();	
	return true;
}

// index start from 1
bool gplayer_imp::UseUITransferIndex(int index)
{
	if (index <= 0) return false;
	if (_ui_transfer_info.opened_indexes.end() == _ui_transfer_info.opened_indexes.find(index)) 
		return false; // 还没有开启
	
	ui_transfer_config_entry cfg;
	if (!player_template::GetUITransferEntry((size_t)index, cfg))
		return false; // 配置数据不对?

	if (!cfg.map_id || !cfg.fee) {
		__PRINTF("UseUITransfer: Failed in Cfg, map_id=%d, fee=%d", cfg.map_id, cfg.fee);
		return false;
	}

	// 扣钱
	if (GetMoney() < cfg.fee)
	{
		__PRINTF("UseUITransfer: Not Enough Money, map_id=%d, fee=%d", cfg.map_id, cfg.fee);
		return false;
	}

	SpendMoney(cfg.fee);
	_runner->spend_money(cfg.fee);

	// 跳转地图
	if (!LongJump(A3DVECTOR(cfg.pos[0], cfg.pos[1], cfg.pos[2]), cfg.map_id)) {	
		__PRINTF("Failed in LongJump"); 
		return false;
	}
	return true;
}

void gplayer_imp::SaveUITransfer(archive & ar)
{
	abase::vector<unsigned int> masks;
	EncodeToMask(_ui_transfer_info.opened_indexes, masks);
	ar << masks.size();
	for (abase::vector<unsigned int>::iterator it = masks.begin(); it != masks.end(); it++) {
		ar << *it;
	}
}

void gplayer_imp::LoadUITransfer(archive & ar)
{
	if(ar.size() == 0) return;

	size_t size;
	ar >> size;
	abase::vector<unsigned int> masks;
	for(size_t i = 0; i < size; i ++)
	{
		unsigned int mask;
		ar >> mask;
		masks.push_back(mask);
	}
	DecodeFromMask(_ui_transfer_info.opened_indexes, masks);
}

//---------------------------------鲜花发放逻辑------------------------------------
// Delivery扣除鲜花，55648免费花，55649收费花，写死了
// @return true/false: 成功/包裹空间不足
bool gplayer_imp::FlowerTakeoffByDelivery(int count)      					
{
	int freeFlowerId = 55648;
	int chargedFlowerId = 55649;	
	int freeFlowerCount = GetInventory().CountItemByID(freeFlowerId);
	int chargedFlowerCount = GetInventory().CountItemByID(chargedFlowerId);
	if (freeFlowerCount + chargedFlowerCount < count) return false;
	if(!_pstate.CanTrade() || _cur_session || _session_list.size()) return false;

	if (freeFlowerCount >= count) { // 免费的够用
		TakeOutItem(freeFlowerId, count);	
	} else {
		if (freeFlowerCount > 0) {
			TakeOutItem(freeFlowerId, freeFlowerCount);	
		}
		TakeOutItem(chargedFlowerId, count - freeFlowerCount);	
	}
	return true;
}

// Delivery发放领取鲜花奖励 
// award_type  0/1 礼包/Buff
// @return true/false 成功/检查失败(物品错误，包裹空间)
bool gplayer_imp::FlowerSendAwardByDelivery(int award_type, int award_id)
{
	int award_type_item = 0;
	int award_type_buff = 1;
	// 1. 检查
	if (award_type == award_type_item) { // 检查物品是否存在，包裹是否有空余位置
		if(!InventoryHasSlot(1)) return false;
		if (NULL == gmatrix::GetDataMan().get_item_for_sell(award_id)) 	return false;
	}	

	// 2. 发奖	
	{
		if (award_type == award_type_item) { // 物品
			DeliverItem(award_id, 1, 1, 0, ITEM_INIT_TYPE_TASK);
		} else if (award_type == award_type_buff) { // Buff
			SKILL::Data data(award_id); 
			_skill.CastRune(data, this, 1, 0);
		} // else 错误的类型，忽略 

		return true;
	}
}

//---------------------------------VIP奖励相关函数------------------------------------
/**
 * @brief SendPlayerVipAwardInfo 向客户端发送玩家的VIP奖励信息
 *
 * @return 
 */
bool gplayer_imp::SendPlayerVipAwardInfo(bool force)
{
	//设置冷却
	if(!force)
	{
		if(!CheckCoolDown(COOLDOWM_INDEX_GET_VIP_AWARD_INFO)) return false;
		SetCoolDown(COOLDOWM_INDEX_GET_VIP_AWARD_INFO, GET_VIP_AWARD_INFO_COOLDOWN_TIME);
	}

	std::vector<S2C::CMD::vip_award_info::award_data> award_list;
	int tmpendtime = 0;

	//跨服时不能领取
	if(IsZoneServer())
	{
		award_list.clear();
		_vip_award.GetVipAwardEndtime(tmpendtime);
		_runner->vip_award_info(tmpendtime, _vip_award.GetRecentTotalCash(), award_list);
		return true;
	}

	int cur_time = g_timer.get_systime();
	_vip_award.GetOrdinaryAvailableAward(cur_time, award_list, GetObjectLevel(), GetRebornCount());
	_vip_award.GetVipAvailableAward(cur_time, award_list);

	_vip_award.GetVipAwardEndtime(tmpendtime);
	_runner->vip_award_info(tmpendtime, _vip_award.GetRecentTotalCash(), award_list);

	//vip过期并且已经降成0的时候AU不会发消息过来，所以这里可以做个检查
	gplayer* pPlayer = GetParent();
	if(pPlayer->CheckExtraState(gplayer::STATE_VIPAWARD))
	{
		unsigned char tmplevel = 0;
		if(!_vip_award.IsVipAwardPlayer(tmplevel))
		{
			pPlayer->ClrExtraState(gplayer::STATE_VIPAWARD);
			pPlayer->vipaward_level = tmplevel;

			_runner->vip_award_change(_parent->ID.id, tmplevel, (char)_vip_award.IsHideVipLevel());
		}
	}

	return true;
}

/**
 * @brief ReceiveVipAwardInfoNotify : 收到delivery发来的vip状态信息，需要判断是否有变化，
 *                                    判断变化的依据是vip到期时间是否和上一次不一致
 * @param vipserver_curtime
 * @param start_time
 * @param end_time
 * @param vipaward_level
 */
void gplayer_imp::ReceiveVipAwardInfoNotify(int vipserver_curtime, int start_time, int end_time, int vipaward_level, int recent_total_cash)
{
	_vip_award.SetRecentTotalCash(recent_total_cash);
	//检查vip档次是否有变化
	if(_vip_award.CheckVipLevelChange(_parent->ID.id, vipaward_level, start_time, end_time, vipserver_curtime))
	{
		__PRINTF("\n\n\n用户%dVIP等级发生变化new_endtime=%d,new_level=%d\n\n\n", _parent->ID.id, end_time, vipaward_level);
	
		unsigned char tmplevel = 0;
		gplayer* pPlayer = GetParent();
		if(_vip_award.IsVipAwardPlayer(tmplevel))
		{
			pPlayer->SetExtraState(gplayer::STATE_VIPAWARD);
			pPlayer->vipaward_level = tmplevel;
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_VIPAWARD);
			pPlayer->vipaward_level = tmplevel;
		}

		//广播给周围玩家，包括自己
		_runner->vip_award_change(_parent->ID.id, tmplevel, (char)_vip_award.IsHideVipLevel());
	}
}

/**
 * @brief SaveVipAwardData : VIP奖励的存盘函数，存盘的数据是带有版本信息的
 *
 * @param ar
 */
void gplayer_imp::SaveVipAwardData(archive & ar)
{
	//跨服不存盘
	if(IsZoneServer())
	{
		return;
	}

	try
	{
		_vip_award.Save(ar);
	}
	catch(...)
	{
		GLog::log(GLOG_ERR,"用户%d保存VIP数据有误",_parent->ID.id);
		return;
	}
}

/**
 * @brief LoadVipAwardData : 根据不同的数据版本读取存盘的信息
 *
 * @param data
 * @param size
 */
void gplayer_imp::LoadVipAwardData(const void * data, size_t size)
{
	unsigned char tmplevel = 0;
	
	//跨服不读数据
	if(IsZoneServer())
	{
		return;
	}

	if(data && size > 0)
	{
		try
		{       
			raw_wrapper ar(data,size);
			_vip_award.Load(ar, _parent->ID.id);
		}catch(...)
		{       
			GLog::log(GLOG_ERR,"用户%d读取从DB读取VIP数据有误",_parent->ID.id);
			return;
		}

		if(_vip_award.IsVipAwardPlayer(tmplevel))
		{
			gplayer* pPlayer = GetParent();
			pPlayer->SetExtraState(gplayer::STATE_VIPAWARD);
			pPlayer->vipaward_level = tmplevel;
		}

		//是否隐藏VIP等级
		GetParent()->is_hide_vip_level = (char)_vip_award.IsHideVipLevel();
	}
}

/**
 * @brief PlayerObtainVipAward : player领取奖励的处理函数
 *
 * @param curtime
 * @param awardid
 * @param award_itemid
 *
 * @return 
 */
bool gplayer_imp::PlayerObtainVipAward(int curtime, int awardid, int award_itemid)
{
	if(!_pstate.CanShopping()) return false;
	if(award_itemid <= 0 || awardid <= 0) return false;
	if(IsZoneServer()) return false;

	vipgame::award_data & vipaward_data = item_manager::GetVipAwardData(); 
	vipgame::node_t node;

	if(!vipaward_data.QueryAward(awardid, node) || node.award_item_id != award_itemid)
	{
		_runner->error_message(S2C::ERR_VIPAWARD_INVALID_REQUEST);
		return false;
	}

	if(!_vip_award.TryGetAward(curtime, node, GetObjectLevel(), GetRebornCount()))
	{
		//_runner->error_message(S2C::ERR_VIPAWARD_INVALID_REQUEST);
		return false;
	}
	
	//生成物品
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.award_item_id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_VIPAWARD_INVALID_REQUEST);
		return false;
	}

	//计算需要多少个格子
	int total_count = node.award_item_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	
	if(total_count <=0  || total_count != node.award_item_count || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//堆叠上限为1的奖励物品的数量必须为1
	if(pItem->pile_limit == 1 && 1 != node.award_item_count)
	{
		_runner->error_message(S2C::ERR_VIPAWARD_INVALID_REQUEST);
		return false;
	}

	//进包裹
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	DeliverItem(node.award_item_id, node.award_item_count, bind, node.expire_time, ITEM_INIT_TYPE_TASK);
		
	//领取完成
	_vip_award.FinishAwardObtention(curtime, node);

	SendPlayerVipAwardInfo(true);
	GLog::log(GLOG_INFO,"用户%d领取VIP奖励id=%d,item_id=%d", GetParent()->ID.id, awardid, node.award_item_id);
	return true;
}

void gplayer_imp::SetVipLevelHide(char is_hide)
{
	if(_vip_award.SetVipLevelHide(is_hide))
	{
		unsigned char tmplevel = 0;
		_vip_award.IsVipAwardPlayer(tmplevel);

		//广播给周围玩家，包括自己
		_runner->vip_award_change(_parent->ID.id, tmplevel, (char)_vip_award.IsHideVipLevel());
	}

	GetParent()->is_hide_vip_level = (char)_vip_award.IsHideVipLevel();
}
//*************VIP奖励相关函数---结束*****************


//--------------------------------在线奖励相关函数---------------------------------------------
/**
 * @brief SendPlayerOnlineAwardInfo 
 * @param force_flag: 表示是否要强制客户端更新奖励信息，用于显示与服务器端同步
 * @return 
 */
bool gplayer_imp::SendPlayerOnlineAwardInfo(bool force_flag)
{
	std::vector<S2C::CMD::online_award_info::award_data> award_list;
	_online_award.GetCurOnlineAwardInfo(award_list);

	_runner->online_award_info((char)force_flag, award_list);

	return true;
}

bool gplayer_imp::CheckPlayerOnlineAward(time_t t)
{
	bool ret = false;
	player_online_award::player_info playerinfo;
	playerinfo.rebornCnt	= GetRebornCount();
	playerinfo.cls			= GetPlayerClass();
	playerinfo.gender		= IsPlayerFemale();
	playerinfo.level		= GetObjectLevel();

	bool force_flag = false;
	ret = _online_award.CheckTimeToActive(t, playerinfo, force_flag);
	if(ret)
	{
		SendPlayerOnlineAwardInfo(force_flag);
	}

	return ret;
}

void gplayer_imp::SendOnlineAwardDataDirectly(time_t t)
{
	player_online_award::player_info playerinfo;
	playerinfo.rebornCnt	= GetRebornCount();
	playerinfo.cls			= GetPlayerClass();
	playerinfo.gender		= IsPlayerFemale();
	playerinfo.level		= GetObjectLevel();

	bool force_flag = false;
	_online_award.CheckTimeToActive(t, playerinfo, force_flag);
	SendPlayerOnlineAwardInfo(true);
}

void gplayer_imp::SaveOnlineAwardData(archive & ar)
{
	try
	{
		_online_award.Save(ar);
	}
	catch(...)
	{
		GLog::log(GLOG_ERR,"用户%d保存在线倒计时奖励数据有误",_parent->ID.id);
		return;
	}
}

void gplayer_imp::LoadOnlineAwardData(const void * data, size_t size)
{
	if(data && size > 0)
	{
		try
		{       
			raw_wrapper ar(data,size);
			_online_award.Load(ar, _parent->ID.id);
		}catch(...)
		{       
			GLog::log(GLOG_ERR,"用户%d读取从DB读取在线倒计时奖励数据有误",_parent->ID.id);
			return;
		}
	}
}

bool gplayer_imp::PlayerTryGetOnlineAward(int id, time_t t, int index, int small_giftbag_id, int & retcode)
{
	player_online_award::player_info playerinfo;
	playerinfo.rebornCnt	= GetRebornCount();
	playerinfo.cls			= GetPlayerClass();
	playerinfo.gender		= IsPlayerFemale();
	playerinfo.level		= GetObjectLevel();
	
	if(player_online_award::ONLINE_SUCCESS !=(retcode = (int)_online_award.TryGetOnlineAward(id, index, t, small_giftbag_id)))
	{
		return false;
	}

	return true;
}

bool gplayer_imp::PlayerObtainOnlineAward(int awardid, time_t curtime, int awardindex, int small_giftbag_id)
{
	if(!_pstate.CanShopping()) return false;
	if(awardid <= 0 || awardindex < 0) return false;

	player_online_award::player_info playerinfo;
	playerinfo.rebornCnt	= GetRebornCount();
	playerinfo.cls			= GetPlayerClass();
	playerinfo.gender		= IsPlayerFemale();
	playerinfo.level		= GetObjectLevel();

	if(player_online_award::ONLINE_SUCCESS != _online_award.TryGetOnlineAward(awardid, awardindex, curtime, small_giftbag_id))
	{
		 //_runner->error_message(S2C::ERR_ONLINEAWARD_INVALID_REQUEST);
		return false;
	}

	//获取小礼包信息
	int award_item_id			= 0; 
	const int award_item_count	= 1; //礼包数量只能是1
	if(!_online_award.GetSmallGiftbagID(awardid, awardindex, award_item_id))
	{
		_runner->error_message(S2C::ERR_ONLINEAWARD_INVALID_REQUEST);
		return false;
	}
	
	//生成物品
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(award_item_id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_ONLINEAWARD_INVALID_REQUEST);
		return false;
	}

	//计算需要多少个格子
	int total_count = award_item_count;
	int need_empty_slot = total_count/pItem->pile_limit + ((total_count % pItem->pile_limit)?1:0);
	
	if(total_count <=0  || total_count != award_item_count || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//堆叠上限为1的奖励物品的数量必须为1
	if(pItem->pile_limit == 1 && 1 != award_item_count)
	{
		_runner->error_message(S2C::ERR_ONLINEAWARD_INVALID_REQUEST);
		return false;
	}

	//进包裹
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	DeliverItem(award_item_id, award_item_count, bind, 0, ITEM_INIT_TYPE_TASK);

	//领取完成
	bool is_closed = false;
	_online_award.FinishOnlineAwardObtain(_parent->ID.id, awardid, awardindex, curtime, is_closed);
	_runner->online_award_permit(awardid, awardindex, player_online_award::ONLINE_OBTAINED);
	//该礼包已经领完所有奖励,发给客户端把这个礼包信息去掉
	if(is_closed)
	{
		SendPlayerOnlineAwardInfo(false);
	}

	GLog::log(GLOG_INFO,"用户%d领取在线倒计时奖励id=%d,item_id=%d", GetParent()->ID.id, awardid, award_item_id);
	return true;
}
//*************在线奖励相关函数---结束****************



//ZX 5 years anniversary code, delete later, Add by Houjun 20120320
void gplayer_imp::SaveFiveAnniDataDB(archive & ar)
{
	if(!TestFiveAnniTime())
	{
		return;
	}
	int version = 0;
	ar << version;
	ar << _annu_five_star;
	ar << _time_annu_five_award;
	ar << _annu_sign_state;
}
//Add end.
void gplayer_imp::LoadFiveAnniDataDB(const void* buf, size_t size)
{
	if(!TestFiveAnniTime())
	{
		return;
	}
	if(size <= 0) return;
	raw_wrapper ar(buf, size);
	int version = 0;
	ar >> version;
	if(version == 0)
	{
		ar >> _annu_five_star >> _time_annu_five_award >> _annu_sign_state;
	}
	ResetAnniData();
}

void gplayer_imp::FiveAnniCongratuation()
{
	if(!TestFiveAnniTime())
	{
		return;
	}
	ResetAnniData();
	if(_annu_sign_state != ANNI_SIGN_INIT)
	{
		return;
	}
	_annu_sign_state = ANNI_SIGN_CONGRATUATED;
	if(_annu_five_star< 5)
	{
		_annu_five_star ++;
	}
	_runner->zx_five_anni_data(_annu_five_star, _annu_sign_state);
}

void gplayer_imp::FiveAnniDeliveryAward()
{
	if(!TestFiveAnniTime())
	{
		return;
	}
	ResetAnniData();
	if(_annu_sign_state != ANNI_SIGN_CONGRATUATED)
	{
		return;
	}
	if(_annu_five_star <= 0 || _annu_sign_state > 5)
	{
		return;
	}
	const static int taskIds[5] = {25710, 25709, 25708, 25707, 25706}; 
	PlayerTaskInterface task_if(this);
	if(OnTaskCheckDeliver(&task_if,taskIds[_annu_five_star - 1],0))
	{
		_annu_sign_state = ANNI_SIGN_AWARDDELIVERIED;
		_runner->zx_five_anni_data(_annu_five_star, _annu_sign_state);
	}
}

void gplayer_imp::ResetAnniData()
{
	if(!TestFiveAnniTime())
	{
		return;
	}
	time_t curr_sec = (size_t)g_timer.get_systime();
	struct tm curr, last;
	localtime_r((time_t*)&curr_sec, &curr);
	localtime_r((time_t*)&_time_annu_five_award, &last);

	if(curr_sec < _time_annu_five_award || curr.tm_year > last.tm_year || (curr.tm_year == last.tm_year && curr.tm_yday > last.tm_yday))
	{   
		_annu_sign_state = ANNI_SIGN_INIT;
		_time_annu_five_award = curr_sec;
	}
	_runner->zx_five_anni_data(_annu_five_star, _annu_sign_state);
}

bool gplayer_imp::TestFiveAnniTime()
{
	time_t curr_sec = (size_t)g_timer.get_systime();
	struct tm start = {0, 0, 10, 15, 4, 2012 - 1900, 0, 0, 0};
	struct tm end = {0, 0, 9, 29, 4, 2012 - 1900, 0, 0, 0};
	time_t start_sec = mktime(&start);
	time_t end_sec = mktime(&end);
	printf("Start_sec: %d, end_sec: %d, curr_sec: %d\n", (int)start_sec, (int)end_sec, (int)curr_sec);
	return curr_sec >= start_sec && curr_sec <= end_sec;
}

void
gplayer_imp::SetTreasureInfo(archive & ar)
{
	//30级才开启
	if(!IsTreasureActive()) return;

	InitTreasureRegionType();
	InitTreasureRegionTimer();

	//初次使用挖宝功能
	if(ar.size() == 0)
	{
		InitTreasureRegionInfo();
	}
	else
	{
		ASSERT(ar.size() == 4 * sizeof(int) + MAX_TREASURE_REGION * (sizeof(char)*2 + sizeof(int)));
		size_t region_count = 0;

		ar >> _treasure_info.version; 
		ar >> _treasure_info.update_timestamp;
		ar >> _treasure_info.remain_dig_count;
		ar >> region_count; 
		ASSERT(region_count == MAX_TREASURE_REGION);
		for(size_t i = 0; i < region_count; ++i)
		{
			ar >> _treasure_info.region[i].status;
			ar >> _treasure_info.region[i].level;
			ar >> _treasure_info.region[i].id;
		}

                int gmtoff = gmatrix::Instance()->GetServerGMTOff();
                int cur_time = g_timer.get_systime();
                int diff_days = (cur_time + gmtoff)/MAX_TREASURE_RESET_CYCLE - (_treasure_info.update_timestamp + gmtoff)/MAX_TREASURE_RESET_CYCLE;
                if(diff_days > 0)
		{
			ResetTreasureRegion();
		}

		//校正时间(可能是由于机器时间调整导致更新时间超前)
		if(_treasure_info.update_timestamp > cur_time)
		{
			_treasure_info.update_timestamp = cur_time;
		}

	}
	
}

void
gplayer_imp::InitTreasureRegion()
{
	InitTreasureRegionType();
	InitTreasureRegionTimer();
	InitTreasureRegionInfo();

	_runner->treasure_info();
}

void
gplayer_imp::InitTreasureRegionType()
{
	for(size_t i = 0; i < MAX_TREASURE_NORMAL_REGION; ++i)
	{
		_treasure_info.region[i].type = TREASURE_TYPE_NORMAL;
	}

	for(size_t j = MAX_TREASURE_NORMAL_REGION; j < MAX_TREASURE_REGION; ++j)
	{
		_treasure_info.region[j].type = TREASURE_TYPE_HIDDEN;
	}
}

void
gplayer_imp::InitTreasureRegionTimer()
{
	int cur_time = g_timer.get_systime();
	int gmtoff = gmatrix::Instance()->GetServerGMTOff();
	_treasure_info.reset_timestamp = cur_time + MAX_TREASURE_RESET_CYCLE - (cur_time + gmtoff)%MAX_TREASURE_RESET_CYCLE + 3;	//允许有三秒的误差
}

void
gplayer_imp::InitTreasureRegionInfo()
{
	_treasure_info.version = 1;
	_treasure_info.update_timestamp = g_timer.get_systime();

	for(size_t i = 0; i < MAX_TREASURE_NORMAL_REGION; ++i)
	{
		_treasure_info.region[i].status = TREASURE_STATUS_OPEN;
		_treasure_info.region[i].id = player_template::GetTreasureRegionID(i);
		_treasure_info.region[i].level = player_template::GetTreasureRegionLevel(_treasure_info.region[i].id); 
	}

	for(size_t j = MAX_TREASURE_NORMAL_REGION; j < MAX_TREASURE_REGION; ++j)
	{
		_treasure_info.region[j].status = TREASURE_STATUS_LOCK;
	}
}

void
gplayer_imp::GetTreasureInfo(archive & ar)
{
	if(!IsTreasureActive()) return;

	ar << _treasure_info.version;
	ar << _treasure_info.update_timestamp;
	ar << _treasure_info.remain_dig_count;
	ar << (size_t)MAX_TREASURE_REGION;

	for(size_t i = 0; i < MAX_TREASURE_REGION; ++i)
	{
		ar << _treasure_info.region[i].status;
		ar << _treasure_info.region[i].level;
		ar << _treasure_info.region[i].id;
	}
}

void 
gplayer_imp::GetClientTreasureInfo(archive & ar)
{
	if(!IsTreasureActive()) return;

	ar << _treasure_info.remain_dig_count;
	ar << MAX_TREASURE_REGION;

	for(size_t i = 0; i < MAX_TREASURE_REGION; ++i)
	{
		ar << _treasure_info.region[i].type;
		ar << _treasure_info.region[i].status;
		ar << _treasure_info.region[i].level;
		ar << _treasure_info.region[i].id;
	}
}

void 
gplayer_imp::UpdateTreasureInfo(int cur_time)
{
	if(!IsTreasureActive()) return;

	if(_treasure_info.reset_timestamp <= cur_time)
	{
                int gmtoff = gmatrix::Instance()->GetServerGMTOff();
                int cur_time = g_timer.get_systime();
                int diff_days = (cur_time + gmtoff)/MAX_TREASURE_RESET_CYCLE - (_treasure_info.update_timestamp + gmtoff)/MAX_TREASURE_RESET_CYCLE;
                
                if(diff_days > 0)
		{
			ResetTreasureRegion();
			
			_treasure_info.reset_timestamp = cur_time + MAX_TREASURE_RESET_CYCLE - (cur_time + gmtoff) % MAX_TREASURE_RESET_CYCLE + 3;	//允许有三秒的误差
			_runner->treasure_info();
		}
                else                                                                                                                                                                             
		{                                                                                                                                                                                
			_treasure_info.reset_timestamp += 5;                                                                                                                                        
		}                                              		
	}
}


void
gplayer_imp::ResetTreasureRegion()
{
	for(size_t i = 0; i < MAX_TREASURE_NORMAL_REGION; ++i)
	{
		_treasure_info.region[i].status = TREASURE_STATUS_OPEN;
		_treasure_info.region[i].level = player_template::GetTreasureRegionLevel(_treasure_info.region[i].id); 
	}
	_treasure_info.update_timestamp = g_timer.get_systime();
}


//升级区域等级
bool
gplayer_imp::PlayerUpgradeTreasureRegion(int item_id, int item_index, int region_id, int region_index)
{
	if(!IsTreasureActive()) return false;
	if(item_index < 0 || (size_t) item_index >= _inventory.Size()) return false;
	if(item_id <= 0 || _inventory[item_index].type != item_id ) return false;
	if(item_id != g_config.item_treasure_region_upgrade_id) return false;

	if(region_id <= 0 || region_index < 0 || region_index >= MAX_TREASURE_REGION) return false; 
	if(_treasure_info.region[region_index].type != TREASURE_TYPE_NORMAL) return false;
	if(_treasure_info.region[region_index].id != region_id) return false;

	if(_treasure_info.region[region_index].level >= MAX_TREASURE_REGION_LEVEL)
	{
		_treasure_info.region[region_index].level = MAX_TREASURE_REGION_LEVEL;
		return false;
	}

	int cur_lvl = _treasure_info.region[region_index].level;
	int new_lvl = cur_lvl;
	bool result;

	result = player_template::UpgradeTreasureRegion(region_id, region_index, cur_lvl, new_lvl);

	//成功
	if(result)
	{
		_treasure_info.region[region_index].level = new_lvl;
		_runner->treasure_region_upgrade(region_id, region_index, true, new_lvl);
	}
	//失败
	else
	{
		_runner->treasure_region_upgrade(region_id, region_index, false, new_lvl);
	}
	TakeOutItem(item_index, item_id, 1);
	return result;
}


//解锁隐藏区域
bool
gplayer_imp::PlayerUnlockTreasureRegion(int item_id, int item_index, int region_id, int region_index)
{
	if(!IsTreasureActive()) return false;
	if(item_index < 0 || (size_t) item_index >= _inventory.Size()) return false;
	if(item_id <= 0 || _inventory[item_index].type != item_id ) return false;
	
	if(region_id <= 0 || region_index < 0 || region_index >= MAX_TREASURE_REGION) return false; 
	if(_treasure_info.region[region_index].status != TREASURE_STATUS_LOCK) return false; 

	//判断一下解锁道具的物品类型, 以及该解锁道具对应的区域id和区域index
	DATA_TYPE dt;
	const SCROLL_UNLOCK_ESSENCE &ess= *(const SCROLL_UNLOCK_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE,dt); 
	if(dt != DT_SCROLL_UNLOCK_ESSENCE || &ess == NULL)
	{
		return false;
	}
	if(ess.region_id != (size_t)region_id || ess.region_pos != region_index) return false;
	
	int need_item_id = player_template::GetUnlockTreasureRegionID(region_id, region_index);
	if(item_id != need_item_id) return false;

	_treasure_info.region[region_index].status = TREASURE_STATUS_OPEN;
	_treasure_info.region[region_index].level = MAX_TREASURE_REGION_LEVEL;
	_treasure_info.region[region_index].id = region_id;

	_runner->treasure_region_unlock(region_id, region_index, true);

	TakeOutItem(item_index, item_id, 1);
	return true;
}


//挖宝
bool
gplayer_imp::PlayerDigTreasureRegion(int region_id, int region_index)
{
	if(!IsTreasureActive()) return false;
	if(region_id <= 0 || region_index < 0 || region_index >= MAX_TREASURE_REGION) return false; 
	if(_treasure_info.region[region_index].id != region_id) return false;
	if(_treasure_info.region[region_index].status != TREASURE_STATUS_OPEN) return false;
	if(_treasure_info.region[region_index].type == TREASURE_TYPE_NORMAL && _treasure_info.remain_dig_count <= 0) return false;
	
	int region_level = _treasure_info.region[region_index].level;
	char event_id = 0;
	abase::vector<treasure_award_item> award_list;

	player_template::GetTreasureAward(region_id, region_index, region_level, award_list, event_id); 
	if(award_list.empty()) 
	{
		_runner->treasure_region_dig(region_id, region_index, false, 0, award_list.size(), award_list.begin());
		return false;
	}
	else if(_inventory.GetEmptySlotCount() < award_list.size())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	for(size_t i = 0; i < award_list.size(); ++i)
	{
		if(award_list[i].item_id > 0 && award_list[i].item_count > 0)
		{
			DeliverItem(award_list[i].item_id, award_list[i].item_count, false, 0, ITEM_INIT_TYPE_TASK);
			GLog::log(GLOG_INFO,"用户%d执行了挖宝操作，获得物品item_id=%d, 物品数量item_count=%d", GetParent()->ID.id, award_list[i].item_id, award_list[i].item_count); 
		}
	}

	if(_treasure_info.region[region_index].type == TREASURE_TYPE_NORMAL)
	{
		_treasure_info.remain_dig_count--;
		_treasure_info.region[region_index].status = TREASURE_STATUS_CLOSE;
	}
	else if(_treasure_info.region[region_index].type == TREASURE_TYPE_HIDDEN)
	{
		_treasure_info.region[region_index].status = TREASURE_STATUS_LOCK;
	}

	_runner->treasure_region_dig(region_id, region_index, true, event_id, award_list.size(), award_list.begin());
	_runner->treasure_info();
	return true;
}

//增加探索次数
void
gplayer_imp::PlayerIncTreasureDigCount(int count)
{
	if(!IsTreasureActive()) return;
	_treasure_info.remain_dig_count += count;
	_runner->inc_treasure_dig_count(count, _treasure_info.remain_dig_count);
	_runner->treasure_info();
}

void gplayer_imp::LoadTowerData(const void* buf, size_t size)
{
	if(size <= 0) return;
	raw_wrapper ar(buf, size);
	int version = 0;
	ar >> version;
	if(version == 0)
	{
		ar >> _tower_raid_info.tower_level >> _tower_raid_info.start_level_today >> _tower_raid_info.end_level_today >>_tower_raid_info.tower_daily_reward_reset_time;
		int cnt = 0;
		ar >> cnt;
		for(int i = 0; i < cnt; i ++)
		{
			int monster_tid = 0;
			ar >> monster_tid;
			_tower_raid_info.tower_monster_list[monster_tid] = 0;
		}
		ar.pop_back(_tower_raid_info.levels, sizeof(_tower_raid_info.levels));
		ResetTowerData();
	}
}

void gplayer_imp::SaveTowerData(archive& ar)
{
	if(_tower_raid_info.tower_level == -1) return;

	int version = 0;
	ar << version;
	ar << _tower_raid_info.tower_level;
	ar << _tower_raid_info.start_level_today;
	ar << _tower_raid_info.end_level_today;
	ar << _tower_raid_info.tower_daily_reward_reset_time;
	ar << _tower_raid_info.tower_monster_list.size();

	TowerMonsterMap::iterator it = _tower_raid_info.tower_monster_list.begin();
	for(; it != _tower_raid_info.tower_monster_list.end(); ++ it)
	{
		ar << it->first;
	}
	ar.push_back(_tower_raid_info.levels, sizeof(_tower_raid_info.levels));
}

void gplayer_imp::ResetTowerData()
{
	time_t curr_sec = (size_t)g_timer.get_systime();
	struct tm curr, last;
	localtime_r((time_t*)&curr_sec, &curr);
	localtime_r((time_t*)&_tower_raid_info.tower_daily_reward_reset_time, &last);

	if(_tower_raid_info.tower_daily_reward_reset_time == 0)
	{
		_tower_raid_info.tower_daily_reward_reset_time = curr_sec; 
		return;
	}

	if(curr_sec < _tower_raid_info.tower_daily_reward_reset_time || curr.tm_year > last.tm_year || (curr.tm_year == last.tm_year && curr.tm_yday > last.tm_yday))
	{   
		_tower_raid_info.tower_daily_reward_reset_time = curr_sec;
		_tower_raid_info.start_level_today = -1;
		_tower_raid_info.end_level_today = -1;
		for(int i = 0; i < MAX_TOWER_RAID_LEVEL; i ++)
		{
			_tower_raid_info.levels[i].daily_reward_delivered = 0;
		}
	}
}

int gplayer_imp::GetTowerLevel() const
{
	return _tower_raid_info.tower_level;
}

void gplayer_imp::SetStartLevelToday(int level)
{
	_tower_raid_info.start_level_today = level;
}

void gplayer_imp::SetEndLevelToday(int level)
{
	_tower_raid_info.end_level_today = level;
}

void gplayer_imp::AddMeetTowerMonster(int tid)
{
	TowerMonsterMap::iterator it = _tower_raid_info.tower_monster_list.find(tid);
	if(it == _tower_raid_info.tower_monster_list.end())
	{
		_tower_raid_info.tower_monster_list[tid] = 0;
		_runner->add_tower_monster(tid);
	}
}

void gplayer_imp::SetDailyRewardDelivered(int level)
{
	if(level >= 0 && level < MAX_TOWER_RAID_LEVEL)
	{
		_tower_raid_info.levels[level].daily_reward_delivered = true;
	}
}

bool gplayer_imp::IsDailyRewardDeilivered(int level)
{
	return level >= 0 && level < MAX_TOWER_RAID_LEVEL && _tower_raid_info.levels[level].daily_reward_delivered;
}

void gplayer_imp::SetLifeLongRewardDelivered(int level)
{
	if(level >= 0 && level < MAX_TOWER_RAID_LEVEL)
	{
		_tower_raid_info.levels[level].lifelong_reward_delivered = true;
	}
}

void
gplayer_imp::DebugSetTreasureRegionLevel(int region_level)
{
	if(region_level < 0) region_level = 0;
	if(region_level > MAX_TREASURE_REGION_LEVEL) region_level = MAX_TREASURE_REGION_LEVEL;

	for(size_t i = 0; i < MAX_TREASURE_NORMAL_REGION; ++i)
	{
		_treasure_info.region[i].level = region_level; 
	}
	_runner->treasure_info();
}

void
gplayer_imp::SetLittlePetInfo(archive & ar)
{
	if(ar.size() == 0) return;
	ASSERT(ar.size() == sizeof(char)*2 + sizeof(int));

	ar >> _little_pet.cur_level;
	ar >> _little_pet.cur_award_level;
	ar >> _little_pet.cur_exp;

}

void
gplayer_imp::GetLittlePetInfo(archive & ar)
{
	if(_little_pet.cur_level <= 0) return;

	ar << _little_pet.cur_level;
	ar << _little_pet.cur_award_level;
	ar << _little_pet.cur_exp;
}

void 
gplayer_imp::PlayerAdoptLittlePet()
{
	if(_little_pet.cur_level != 0) return;

	_little_pet.cur_level = 1;
	_runner->littlepet_info(_little_pet.cur_level, _little_pet.cur_award_level, _little_pet.cur_exp);
	GLog::log(GLOG_INFO,"用户%d领取了诛小仙", GetParent()->ID.id); 
}

void 
gplayer_imp::PlayerFeedLittlePet(int item_id, int item_count)
{
	if(_little_pet.cur_level >= MAX_LITTLE_PET_LEVEL) return; 
	if(item_id < 0 || item_count < 0) return;
	if(_inventory.CountItemByID(item_id) < item_count) return;

	int feed_exp = player_template::GetLittlePetFeedExp(item_id);
	if(feed_exp <= 0) return;
	int total_exp = feed_exp * item_count;

	_little_pet.cur_exp += total_exp;
	int lvlup_exp = player_template::GetLittlePetLevelupExp(_little_pet.cur_level);

	while(_little_pet.cur_exp >= lvlup_exp)
	{
		_little_pet.cur_level++;
		_little_pet.cur_exp -= lvlup_exp;

		if(_little_pet.cur_level >= MAX_LITTLE_PET_LEVEL)
		{
			_little_pet.cur_exp = 0;
			break;
		}
		lvlup_exp = player_template::GetLittlePetLevelupExp(_little_pet.cur_level);
	}

	TakeOutItem(item_id, item_count);
	_runner->littlepet_info(_little_pet.cur_level, _little_pet.cur_award_level, _little_pet.cur_exp);
	GLog::log(GLOG_INFO,"用户%d执行了喂养诛小仙的操作, item_id=%d, item_count=%d, level=%d, exp=%d", GetParent()->ID.id, item_id, item_count, _little_pet.cur_level, _little_pet.cur_exp); 
}

void
gplayer_imp::PlayerGetLittlePetAward()
{
	if(_little_pet.cur_level <= 0) return;
	if(_little_pet.cur_award_level >= _little_pet.cur_level) return;

	size_t need_inventory_slot = _little_pet.cur_level - _little_pet.cur_award_level;
	if(_inventory.GetEmptySlotCount() < need_inventory_slot) return;


	for(int i = _little_pet.cur_award_level+1; i <= _little_pet.cur_level; ++i)
	{
		int award_item_id = player_template::GetLittlePetAwardItem(i);	
		if(award_item_id > 0) DeliverItem(award_item_id, 1, false, 0, ITEM_INIT_TYPE_TASK);
		GLog::log(GLOG_INFO,"用户%d领取了诛小仙奖励, item_id=%d", GetParent()->ID.id, award_item_id); 
	}

	_little_pet.cur_award_level = _little_pet.cur_level;
	_runner->littlepet_info(_little_pet.cur_level, _little_pet.cur_award_level, _little_pet.cur_exp);
}

void
gplayer_imp::ActiveRune()
{
	if(_active_rune) return;

	_active_rune = true;
	_runner->rune_info(_active_rune, _rune_score, GetRuneCustomizeInfo());
}


void
gplayer_imp::ActiveRuneProp()
{
	if(!_active_rune) return;
	

	int reborn_cnt = GetRebornCount();
        int level = _basic.level;
	int eff_level = reborn_cnt * 150 + level;	
	int active_level = player_template::GetRuneActiveLevel(eff_level);
	if(active_level == 0) return;

	int damage = 0;
	int hp = 0;
	int mp = 0;
	int dp = 0;
	player_template::GetRuneActiveProp(active_level, damage, hp, mp, dp);
	if(damage > 0) _en_point.damage += damage;
        if(hp > 0) _en_point.max_hp += hp;	
        if(mp > 0) _en_point.max_mp += mp;	
        if(dp > 0) _en_point.max_dp += dp;	

	_active_rune_level = active_level;
	SetRefreshState();
}

void
gplayer_imp::DeactiveRuneProp()
{
	if(!_active_rune) return;
	if(_active_rune_level <= 0) return;

	int damage = 0;
	int hp = 0;
	int mp = 0;
	int dp = 0;
	if(_active_rune_level > 0)
	{
		player_template::GetRuneActiveProp(_active_rune_level, damage, hp, mp, dp);
		if(damage > 0) _en_point.damage -= damage;
	        if(hp > 0) _en_point.max_hp -= hp;	
	        if(mp > 0) _en_point.max_mp -= mp;	
	        if(dp > 0) _en_point.max_dp -= dp;	
	}
	_active_rune_level = 0;
	SetRefreshState();
}

void
gplayer_imp::UpdateRuneProp()
{
	if(_active_rune_level <= 0) return;

	DeactiveRuneProp();
	ActiveRuneProp();
	property_policy::UpdatePlayer(GetPlayerClass(),this);
}


//元魂鉴定
void
gplayer_imp::PlayerIdentifyRune(int rune_index, int assist_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _assist_index;
		public:
		op(int rune_index, int assist_index) : _rune_index(rune_index), _assist_index(assist_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_IDENTIFY;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->IdentifyRune(_rune_index, _assist_index);
			if(!bRst) 
			{
			}
		}
	};
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, assist_index));
	AddStartSession(pSession);
}


bool
gplayer_imp::IdentifyRune(int rune_index, int assist_index)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	if(rune_index < 0 || (size_t)rune_index >= inv_size) return false;
	if(assist_index < 0 || (size_t)assist_index >= inv_size) return false;
	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}
	
	item & it1 = _inventory[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 

	item & it2 = _inventory[assist_index];
	if(it2.type == -1) return false;

	bool rst = it1.body->Identify(&it1, this, rune_index, assist_index);

	if(rst)
	{
		int assist_id = _inventory[assist_index].type;

		UseItemLog(_inventory[assist_index],1);
		_inventory.DecAmount(assist_index, 1);
		_runner->player_drop_item(IL_INVENTORY,assist_index,assist_id, 1 ,S2C::DROP_TYPE_USE);

		AddRuneScore(5);
		_runner->rune_info(_active_rune, _rune_score, GetRuneCustomizeInfo());

		//检查是否需要记录消费值
		CheckSpecialConsumption(assist_id, 1);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}

	GLog::log(GLOG_INFO,"用户%d执行了元婴鉴定操作, rune_index=%d, assist_index=%d, result=%d", GetParent()->ID.id, rune_index, assist_index, rst); 
	return rst;
}

// Youshuang add
bool gplayer_imp::CombineFashionColorant(int* idx, int cnt)
{
	size_t fashion_list_size = _inventory.Size();

	abase::vector<item*> items;
	std::map<int, size_t> counter;
	for( int i = 0; i < cnt; ++i )
	{
		if( idx[i] >= (int)fashion_list_size )
		{
			return false;
		}
		if( idx[i] >= 0 )
		{
			counter[ idx[i] ] = ( counter.find( idx[i] ) != counter.end() ) ? counter[ idx[i] ] + 1 : 1;
			item* pTmp = &_inventory[ idx[i] ];
			if( !pTmp ){ return false; }
			if( counter[ idx[i] ] > pTmp->count ){ return false; }
			if(pTmp->IsLocked() || ( pTmp->GetItemType() != item_body::ITEM_TYPE_COLORANT )) 
			{
				return false;
			}
			items.push_back( pTmp );
		}
	}
	if( items.size() < 2 )
	{
		return false;
	}
	int dst_color = 0;
	int dst_quality = 0;
	bool ret = colorant_item::CombineFashionColorant( items, dst_color, dst_quality );

	if( ret )
	{
		int colorant_id = player_template::GetInstance().GetID( dst_color, dst_quality );
		if( ( colorant_id < 0 ) || !HasEnoughSlot( IL_INVENTORY, colorant_id, 1 ) )
		{
			return false;
		}
		for( int i = 0; i < cnt; ++i )
		{
			if( idx[i] >= 0 )
			{
				TakeOutItem( idx[i], _inventory[idx[i]].type, 1 );
			}
		}
		
		int rst = -1;
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell( colorant_id );
		if(pItem == NULL || pItem->pile_limit <= 0)
		{
			return false;
		}
		bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
		DeliverItem( colorant_id, 1, bind, 0, ITEM_INIT_TYPE_TASK, rst );

		if( rst < 0 || rst >= (int)fashion_list_size )
		{
			return false;
		}
		item* pTmp = &_inventory[ rst ];
		if( !pTmp || pTmp->GetItemType() != item_body::ITEM_TYPE_COLORANT ){ return false; }
		PlayerGetItemInfo( IL_INVENTORY, rst );
		_runner->produce_colorant_item( rst, dst_color, dst_quality );
	}
	
	return ret;
}

bool gplayer_imp::AddFashionColor( int colorant_index, int where, int fashion_item_index )
{
	if( ( where != IL_FASHION ) && ( where != IL_INVENTORY ) ){ return false; }
	size_t inv_list_size = _inventory.Size();
	if( colorant_index < 0 || (size_t)colorant_index >= inv_list_size ) return false;
	
	size_t fashion_list_size = ( where == IL_FASHION ) ? _fashion_inventory.Size() : inv_list_size;
	if( fashion_item_index < 0 || (size_t)fashion_item_index >= fashion_list_size ) return false;
	
	item& colorant = _inventory[colorant_index];
	if(colorant.GetItemType() != item_body::ITEM_TYPE_COLORANT) return false; 
	if( colorant.count < 3 ) return false;
	
	item& fashion = ( where == IL_FASHION ) ? _fashion_inventory[fashion_item_index] : _inventory[fashion_item_index];
	if(fashion.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) return false; 

	if(colorant.IsLocked() || fashion.IsLocked()) return false;

	colorant_item* body = dynamic_cast<colorant_item*>( colorant.body );
	if( !body )
	{
		return false;
	}
	int colorant_type = colorant.type;
	bool ret = fashion.AddColor( colorant_type, body->GetQuality() );
	if(ret)
	{
		if( TakeOutItem( colorant_type, 3 ) < 0 )
		{
			return false;
		}
		PlayerGetItemInfo( where, fashion_item_index );
		_runner->use_colorant_item( where, fashion_item_index, body->GetColor(), body->GetQuality() );
	}
	
	GLog::log(GLOG_INFO,"用户%d将时装%d染上染色剂%d", GetParent()->ID.id, fashion.type, colorant_type );
	return ret;
}

bool gplayer_imp::GetAchievementAward(  unsigned short achieve_id,  unsigned int award_id ) 
{
	if( !GetAchievementManager().GetAchievementAward( achieve_id, award_id ) ){ return false; }
	
	//生成物品
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell( award_id );
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_ONLINEAWARD_INVALID_REQUEST);
		return false;
	}

	//计算需要多少个格子
	int total_count = 1;
	if( !HasEnoughSlot( IL_INVENTORY, award_id, total_count ))
	{ 
		return false; 
	}

	//堆叠上限为1的奖励物品的数量必须为1
	if(pItem->pile_limit == 1 && 1 != total_count)
	{
		_runner->error_message(S2C::ERR_ONLINEAWARD_INVALID_REQUEST);
		return false;
	}

	//进包裹
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	DeliverItem(award_id, total_count, bind, 0, ITEM_INIT_TYPE_TASK);

	_runner->get_achievement_award( achieve_id );

	GLog::log(GLOG_INFO,"用户%d领取成就奖励item_id=%d", GetParent()->ID.id, award_id);
	return true;
}

// Important Note: the arg data should be created by new operator!
void gplayer_imp::DeliverTopicSite( TOPIC_SITE::topic_data* data )
{
	data->_player = _parent->ID.id;
	data->DeliverTopic();
	delete data;
	data = NULL;
}

bool gplayer_imp::HasEnoughSlot( int where, const std::vector< item_cnt_info >& items )
{
	int empty_slot = 0;
	switch(where)
	{
		case IL_INVENTORY:
			empty_slot = _inventory.GetEmptySlotCount();
			break;
		case IL_EQUIPMENT:
			empty_slot = _equipment.GetEmptySlotCount();
			break;
		case IL_TASK_INVENTORY:
			empty_slot =_task_inventory.GetEmptySlotCount();
			break;
		case IL_PET_BEDGE:
			empty_slot = _pet_bedge_inventory.GetEmptySlotCount();
			break;
		case IL_PET_EQUIP:
			empty_slot = _pet_equip_inventory.GetEmptySlotCount();
			break;
		case IL_POCKET:
			empty_slot = _pocket_inventory.GetEmptySlotCount();
			break;
		case IL_FASHION:
			empty_slot = _fashion_inventory.GetEmptySlotCount();
			break;
		case IL_MOUNT_WING:
			empty_slot = _mount_wing_inventory.GetEmptySlotCount();
			break;
		case IL_GIFT:
			empty_slot = _gift_inventory.GetEmptySlotCount();
			break;
		case IL_FUWEN:
			empty_slot = _fuwen_inventory.GetEmptySlotCount();
			break;
		default:
			break;
	}
	int need_slot = 0;
	for( std::vector<item_cnt_info>::const_iterator iter = items.begin(); iter != items.end(); iter++ )
	{
		int pile_limit = gmatrix::GetDataMan().get_item_pile_limit( iter->item_id );
		int item_cnt = iter->item_cnt;
		if( pile_limit <= 0 || item_cnt <= 0 ){ return false; }
		need_slot += ( item_cnt / pile_limit + ( ( item_cnt % pile_limit ) ? 1 : 0 ) );
	}
	if( need_slot > empty_slot )
	{
		_runner->error_message( S2C::ERR_INVENTORY_IS_FULL ); // notify any pack is full
		return false;
	}
	return true;
}

bool gplayer_imp::HasEnoughSlot( int where, int item_id, int item_cnt )
{
	std::vector<item_cnt_info> tmp;
	tmp.push_back( item_cnt_info( item_id, item_cnt ) );
	return HasEnoughSlot( where, tmp );
}
	
bool gplayer_imp::DoShopFromFacbaseMall( int goods_idx, int item_id, int cnt )
{
	facbase_mall* pMall = gmatrix::GetFacbaseMall( GetParent()->id_mafia );
	if( (cnt <= 0) || !pMall || !pMall->IsMallItemActivate( goods_idx ) ){ return false; }
	const facbase_mall_item_info* iteminfo = player_template::GetInstance().GetItemInfoByIndex( goods_idx );
	if( !iteminfo || ( item_id != iteminfo->tid ) || ( cnt * iteminfo->needed_faction_coupon > _faction_coupon ) )
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}

	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell( item_id );
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}
	if( !HasEnoughSlot( IL_INVENTORY, item_id, cnt ))
	{ 
		return false; 
	}
	FactionCouponChange( -cnt * ( iteminfo->needed_faction_coupon ) );
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	GainItem( item_id, cnt, bind, 0, ITEM_INIT_TYPE_TASK );

	GLog::formatlog("formatlog:用户%d从帮派商城购买了%d个物品%d", GetParent()->ID.id, cnt, item_id );
	return true;
}

bool gplayer_imp::GetFacbaseMallInfo() const
{
	facbase_mall* pMall = gmatrix::GetFacbaseMall( ((gplayer*)_parent)->id_mafia );
	if( !pMall )
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}
	_runner->get_facbase_mall_info( pMall );
	return true;
}

gplayer_imp::FBASE_CMD_RET gplayer_imp::DoBidFromFacbaseMall( int idx, int item_id, int coupon, int name_len, char * player_name)
{
	if (OI_GetMafiaID() <= 0)
		return FBASE_CMD_FAIL;
	const facbase_auction_item_info* iteminfo = player_template::GetInstance().GetAuctionItemInfoByID( item_id );
	if( !iteminfo || ( iteminfo->needed_faction_coupon >= coupon ) || ( coupon > _faction_coupon ) )
		return FBASE_CMD_FAIL;
	if (name_len != (int)_username_len || strncmp(player_name, _username, _username_len) != 0)
	{
		GLog::log(GLOG_ERR, "DoBidFromFacbaseMall name invalid, roleid %d", _parent->ID.id);
		return FBASE_CMD_FAIL;
	}
	FactionCouponChange(-1*coupon);
	return FBASE_CMD_DELIVER;
}

void gplayer_imp::FactionCouponChange( int delta )
{
	_faction_coupon += delta;
	if( delta > 0 )
	{
		_fac_coupon_added = true;
		_fac_coupon_add += delta; 
	}
	_runner->get_faction_coupon( _faction_coupon );
	GLog::formatlog("formatlog:用户%d的帮派金券变化%d", GetParent()->ID.id, delta );
}

void gplayer_imp::SendFactionCouponAdd()
{
	if( !_fac_coupon_added ){ return; }
	GMSV::SendFacCouponAdd( _parent->ID.id, _fac_coupon_add );
	_fac_coupon_added = false;
}

bool gplayer_imp::ContributeFacbaseCash( int cnt )
{
	if (OI_GetMafiaID() <= 0){ return false; }
	if( cnt < 1 || (100 * cnt > _mallinfo.GetCash()) )
	{
		_runner->error_message( S2C::ERR_FBASE_NOT_ENOUGH_CASH );
		return false; 
	}
	int order_id = _mallinfo.GetOrderID();
	// 61299 is faction base cash id number
	PlayerUseCash( 61299, cnt, 0, cnt*100, order_id );
	DeliverFBaseCash( cnt );
	FactionCouponChange( 50 * cnt );
	_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	GMSV::SendFacDynDonateCash(OI_GetMafiaID(), _username, _username_len, cnt);
	
	GLog::formatlog("formatlog:用户%d往帮派基地捐献了%d金元宝", GetParent()->ID.id, cnt );
	return true;
}

gplayer_imp::FBASE_CMD_RET gplayer_imp::CheckPostFacBaseMsg(int fid, char message[], unsigned int len)
{//子类有直接实现
	if (OI_GetMafiaID() <= 0 || fid != OI_GetMafiaID())
		return FBASE_CMD_FAIL;
	if (len > MAX_FAC_BASE_MSG_LEN)
		return FBASE_CMD_FAIL;
	if (GetParent()->rank_mafia != gplayer::TITLE_MASTER 
			&& GetParent()->rank_mafia != gplayer::TITLE_VICEMASTER
			&& GetParent()->rank_mafia != gplayer::TITLE_CAPTAIN)
		return FBASE_CMD_FAIL;
	return FBASE_CMD_DELIVER;
}

// void gplayer_imp::LoadNewYearAwardInfo( archive& ar )
// {
	// if (ar.size() == 0) return;
	// ar >> _newyear_info.score >> _newyear_info.last_fetch_date;
	// for( int i = 0; i <= NEWYEAR_AWARD_LAST - NEWYEAR_AWARD_FIRST; ++i )
	// {
		// ar >> _newyear_info.fetch_level[ i ];
	// }
// }

// void gplayer_imp::SaveNewYearAwardInfo( archive& ar )
// {
	// ar << _newyear_info.score << _newyear_info.last_fetch_date;
	// for( int i = 0; i <= NEWYEAR_AWARD_LAST - NEWYEAR_AWARD_FIRST; ++i )
	// {
		// ar << _newyear_info.fetch_level[ i ];
	// }
// }

//static const char* NEWYEAR_AWARD_STRINGS[ NEWYEAR_AWARD_LAST - NEWYEAR_AWARD_FIRST + 1 ] = { "10", "100", "240" };

bool gplayer_imp::TryDeliverItem( int where, int item_id, int cnt )
{
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell( item_id );
	if( pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}
	if( !HasEnoughSlot( where, item_id, cnt ))
	{
		return false;
	}
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	GainItem( item_id, cnt, bind, 0, ITEM_INIT_TYPE_TASK );
	return true;
}

// void gplayer_imp::FetchNewYearAward( char award_type )
// {
	// if( award_type > NEWYEAR_AWARD_LAST || award_type < NEWYEAR_AWARD_FETCH_INFO )
	// {
		// _runner->error_message(S2C::ERR_FATAL_ERR);
		// return;
	// }
	// const newyear_award_template& tmp = player_template::GetInstance().GetAwardTemplate();
	// if( award_type == NEWYEAR_AWARD_FETCH_INFO )
	// {
		// PlayerGetNewYearAwardInfo();
		// return;
	// }
	// if( award_type == NEWYEAR_AWARD_DAILY )
	// {
		// if( !CanFetchNewYearDailyAward() )
		// {
			// _runner->error_message(S2C::ERR_FATAL_ERR);
			// return;
		// }
		
		// if( !TryDeliverItem( IL_INVENTORY, tmp.sign_award_item_id, 1 ) ){ return; }
		
		// //GLog::formatlog("formatlog:用户%d领取了新年每日奖励，获得物品%d", GetParent()->ID.id, tmp.sign_award_item_id );
		// GLog::formatlog("formatlog:get_newyear_daily_award:roleid=%d:item_id=%d", GetParent()->ID.id, tmp.sign_award_item_id );
		// _newyear_info.score += tmp.sign_score;
		// _newyear_info.last_fetch_date = GetNextMidNightTime( g_timer.get_systime() );
		// PlayerGetNewYearAwardInfo();
		// return;
	// }
	// if( !CanFetchNewYearAward( award_type ) )
	// {
		// _runner->error_message(S2C::ERR_FATAL_ERR);
		// return;
	// }
	// int award_idx = award_type - NEWYEAR_AWARD_FIRST;
	// newyear_award_item tmp_item = tmp.items[ award_idx ];
	
	// if( !TryDeliverItem( IL_INVENTORY, tmp_item.award_item_id, 1 ) ){ return; }
	
	// //GLog::formatlog("formatlog:用户%d领取了新年%s积分奖励, 获得物品%d", GetParent()->ID.id, NEWYEAR_AWARD_STRINGS[ award_idx ], tmp_item.award_item_id );
	// GLog::formatlog("formatlog:get_newyear_score_award:roleid=%d:score=%s:item_id=%d", GetParent()->ID.id, NEWYEAR_AWARD_STRINGS[ award_idx ], tmp_item.award_item_id );
	// _newyear_info.score -= tmp_item.score_needed;
	// ++_newyear_info.fetch_level[ award_idx ];
	// PlayerGetNewYearAwardInfo();
// }

// char gplayer_imp::CanFetchNewYearDailyAward()
// {
	// int cur_time = g_timer.get_systime();
	// const newyear_award_template& tmp = player_template::GetInstance().GetAwardTemplate();
	// if( tmp.start >= cur_time || tmp.end_sign <= cur_time ){ return 0; }
	
	// int nextday = GetNextMidNightTime( cur_time );
	// int last_fetch_date = _newyear_info.last_fetch_date;
	// if( (last_fetch_date == 0) || (last_fetch_date != nextday) )
	// {
		// return 1;
	// }
	// return 0;
// }

// char gplayer_imp::CanFetchNewYearAward( int idx ) const
// {
	// if( idx > NEWYEAR_AWARD_LAST || idx < NEWYEAR_AWARD_FIRST ){ return 0; }
	// int cur_time = g_timer.get_systime();
	// const newyear_award_template& tmp = player_template::GetInstance().GetAwardTemplate();
	// if( tmp.start >= cur_time || tmp.end_fetch_award <= cur_time ){ return 0; }
	// if( ((int)tmp.items.size()) <= (idx - NEWYEAR_AWARD_FIRST) ){ return 0; }

	// newyear_award_item tmp_item = tmp.items[ idx - NEWYEAR_AWARD_FIRST ];
	// if( tmp_item.fetch_count != 0 && tmp_item.fetch_count <= _newyear_info.fetch_level[ idx - NEWYEAR_AWARD_FIRST ] ){ return 0; }
	// if( tmp_item.score_needed > _newyear_info.score ){ return 0; }
	// return 1;
// }

// void gplayer_imp::PlayerGetNewYearAwardInfo()
// {
	// std::vector<char> tmp;
	// tmp.push_back( CanFetchNewYearDailyAward() );
	// for( int i = NEWYEAR_AWARD_FIRST; i <= NEWYEAR_AWARD_LAST; ++i )
	// {
		// tmp.push_back( CanFetchNewYearAward( i ) );
	// }
	// _runner->get_newyear_award_info( _newyear_info.score, tmp );
// }
// end

//元魂血炼
void
gplayer_imp::PlayerCombineRune(int rune_index, int assist_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _assist_index;
		public:
		op(int rune_index, int assist_index) : _rune_index(rune_index), _assist_index(assist_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_COMBINE;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->CombineRune(_rune_index, _assist_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}
	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, assist_index));
	AddStartSession(pSession);
}


bool
gplayer_imp::CombineRune(int rune_index, int assist_index)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	size_t eqp_size = _equipment.Size();

	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	if(assist_index < 0 || (size_t)assist_index >= inv_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	
	item & it2 = _inventory[assist_index];
	if(it2.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 

	if(it1.IsLocked() || it2.IsLocked()) return false;

	bool rst = it1.body->Combine(&it1, this, rune_index, assist_index);
	if(rst)
	{
		int assist_id = _inventory[assist_index].type;

		UseItemLog(_inventory[assist_index],1);
		_inventory.DecAmount(assist_index, 1);
		_runner->player_drop_item(IL_INVENTORY,assist_index,assist_id, 1 ,S2C::DROP_TYPE_USE);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}
	
	GLog::log(GLOG_INFO,"用户%d执行了元婴神合操作, rune_index=%d, assist_index=%d, result=%d", GetParent()->ID.id, rune_index, assist_index, rst); 
	return rst;
}

//元魂洗炼
void
gplayer_imp::PlayerRefineRune(int rune_index, int assist_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _assist_index;
		public:
		op(int rune_index, int assist_index) : _rune_index(rune_index), _assist_index(assist_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_REFINE;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->RefineRune(_rune_index, _assist_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, assist_index));
	AddStartSession(pSession);
}


//元魂洗炼
bool
gplayer_imp::RefineRune(int rune_index, int assist_index)
{
	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	if(it1.IsLocked()) return false;
	
	bool rst = ((rune_item*)it1.body)->RefineRune(&it1, this, rune_index, assist_index);
	if(rst)
	{
		if(assist_index != -1)
		{
			int assist_id = _inventory[assist_index].type;

			UseItemLog(_inventory[assist_index],1);
			_inventory.DecAmount(assist_index, 1);
			_runner->player_drop_item(IL_INVENTORY,assist_index,assist_id, 1 ,S2C::DROP_TYPE_USE);

			//检查是否需要记录消费值
			CheckSpecialConsumption(assist_id, 1);

			//更新元婴活跃度
			EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
		}
		_runner->rune_refine_result(0);
	}

	GLog::log(GLOG_INFO,"用户%d执行了元婴婴变操作, rune_index=%d, assist_index=%d, result=%d", GetParent()->ID.id, rune_index, assist_index, rst); 
	return rst;
}



//元魂洗炼结果
bool
gplayer_imp::PlayerRefineRuneAction(int rune_index, bool accept_result)
{
	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}

	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	if(it1.IsLocked()) return false;

	bool rst = ((rune_item*)it1.body)->RefineAction(&it1, this, rune_index, accept_result);

	if(rst)
	{
		_runner->rune_refine_result(1);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}

	GLog::log(GLOG_INFO,"用户%d执行了元婴婴变确认操作, rune_index=%d, is_accept=%d", GetParent()->ID.id, rune_index, accept_result); 
	return rst;
}

void
gplayer_imp::PlayerResetRune(int rune_index, int assist_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _assist_index;
		public:
		op(int rune_index, int assist_index) : _rune_index(rune_index), _assist_index(assist_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_RESET;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(1.7);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->ResetRune(_rune_index, _assist_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, assist_index));
	AddStartSession(pSession);
}


//元魂归元
bool
gplayer_imp::ResetRune(int rune_index, int assist_index)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	size_t eqp_size = _equipment.Size();

	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	if(assist_index < 0 || (size_t)assist_index >= inv_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	if(it1.IsLocked()) return false;
	
	item & it2 = _inventory[assist_index];
	if(it2.type == -1) return false;

	bool rst = it1.body->Reset(&it1, this, rune_index, assist_index);
	if(rst)
	{
		int assist_id = _inventory[assist_index].type;

		UseItemLog(_inventory[assist_index],1);
		_inventory.DecAmount(assist_index, 1);
		_runner->player_drop_item(IL_INVENTORY,assist_index,assist_id, 1 ,S2C::DROP_TYPE_USE);

		AddRuneScore(10);
		_runner->rune_info(_active_rune, _rune_score, GetRuneCustomizeInfo());

		//检查是否需要记录消费值
		CheckSpecialConsumption(assist_id, 1);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}

	GLog::log(GLOG_INFO,"用户%d执行了元婴归劫操作, rune_index=%d, assist_index=%d, result=%d", GetParent()->ID.id, rune_index, assist_index, rst); 
	return rst;
}

void
gplayer_imp::PlayerDecomposeRune(int rune_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		public:
		op(int rune_index) : _rune_index(rune_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_DECOMPOSE;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->DecomposeRune(_rune_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index));
	AddStartSession(pSession);
}


//元魂拆解
bool
gplayer_imp::DecomposeRune(int rune_index)
{
	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	if(it1.IsLocked()) return false;
	
	if(_inventory.GetEmptySlotCount() <=0) return false;
	
	int output_id = 0;
	bool rst = ((rune_item*)it1.body)->Decompose(&it1, this, output_id, rune_index); 

	if(rst && output_id > 0)
	{
		DeliverItem(output_id, 1, false, 0, ITEM_INIT_TYPE_TASK);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}

	GLog::log(GLOG_INFO,"用户%d执行了元婴采星操作, rune_index=%d, output_id=%d, result=%d", GetParent()->ID.id, rune_index, output_id, rst); 
	return rst;
}

bool
gplayer_imp::PlayerLevelupRune(int rune_index)
{
	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 

	int level = 0;	
	bool rst = ((rune_item*)it1.body)->LevelUp(&it1, this, rune_index, level); 

	if (rst) {
		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}
	GLog::log(GLOG_INFO,"用户%d执行了元婴升级操作, rune_index=%d, result=%d, cur_level=%d", GetParent()->ID.id, rune_index, rst, level); 
	return rst;
}

void
gplayer_imp::PlayerOpenRuneSlot(int rune_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		public:
		op(int rune_index) : _rune_index(rune_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_OPEN_SLOT;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->OpenRuneSlot(_rune_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index));
	AddStartSession(pSession);
}

bool
gplayer_imp::OpenRuneSlot(int rune_index)
{
	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	
	bool rst = ((rune_item*)it1.body)->OpenSlot(&it1, this, rune_index); 
	return rst;
}


void
gplayer_imp::PlayerChangeRuneSlot(int rune_index, int assist_index) 
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _assist_index;
		public:
		op(int rune_index, int assist_index) : _rune_index(rune_index), _assist_index(assist_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_CHANGE_SLOT;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->ChangeRuneSlot(_rune_index, _assist_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, assist_index));
	AddStartSession(pSession);
}

bool
gplayer_imp::ChangeRuneSlot(int rune_index, int assist_index)
{
	//检查是否越界
	size_t inv_size = _inventory.Size();
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	if(assist_index < 0 || (size_t)assist_index >= inv_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	if(it1.IsLocked()) return false;
	
	item & it2 = _inventory[assist_index];
	if(it2.type == -1) return false;
	
	bool rst = ((rune_item*)it1.body)->ChangeSlot(&it1, this, rune_index, assist_index); 
	if(rst)
	{
		int assist_id = _inventory[assist_index].type;

		UseItemLog(_inventory[assist_index],1);
		_inventory.DecAmount(assist_index, 1);
		_runner->player_drop_item(IL_INVENTORY,assist_index,assist_id, 1 ,S2C::DROP_TYPE_USE);

		//检查是否需要记录消费值
		CheckSpecialConsumption(assist_id, 1);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}

	GLog::log(GLOG_INFO,"用户%d执行了元婴星衍操作, rune_index=%d, assist_index=%d, result=%d", GetParent()->ID.id, rune_index, assist_index, rst); 
	return rst;
}


void
gplayer_imp::PlayerEraseRuneSlot(int rune_index, int slot_index)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _slot_index;
		public:
		op(int rune_index, int slot_index) : _rune_index(rune_index), _slot_index(slot_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_ERASE_SLOT;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->EraseRuneSlot(_rune_index, _slot_index);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, slot_index));
	AddStartSession(pSession);
}

bool
gplayer_imp::EraseRuneSlot(int rune_index, int slot_index)
{
	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	if(it1.IsLocked()) return false;

	if(_inventory.GetEmptySlotCount() <=0) return false;
	
	int stone_id = 0;
	bool rst = ((rune_item*)it1.body)->EraseSlot(&it1, this, rune_index, slot_index, stone_id); 

	if(rst && stone_id > 0)
	{
		DeliverItem(stone_id, 1, false, 0, ITEM_INIT_TYPE_TASK);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}
	GLog::log(GLOG_INFO,"用户%d执行了元婴摘星操作, rune_index=%d, slot_index=%d, result=%d", GetParent()->ID.id, rune_index, slot_index, rst); 
	return rst;
}

void
gplayer_imp::PlayerInstallRuneSlot(int rune_index, int slot_index, int stone_index, int stone_id)
{
	class op : public session_general_operation::operation
	{
		int _rune_index;
		int _slot_index;
		int _stone_index;
		int _stone_id;

		public:
		op(int rune_index, int slot_index, int stone_index, int stone_id) : _rune_index(rune_index), _slot_index(slot_index), _stone_index(stone_index), _stone_id(stone_id)
		{}

		virtual int GetID()
		{
			return S2C::GOP_RUNE_INSTALL_SLOT;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->InstallRuneSlot(_rune_index, _slot_index, _stone_index, _stone_id);
			if(!bRst) 
			{
			}
		}
	};

	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	session_general_operation * pSession = new session_general_operation(this,  new op(rune_index, slot_index, stone_index, stone_id));
	AddStartSession(pSession);
}

bool
gplayer_imp::InstallRuneSlot(int rune_index, int slot_index, int stone_index, int stone_id)
{
	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}

	//检查是否越界
	size_t eqp_size = _equipment.Size();
	if(rune_index < 0 || (size_t)rune_index >= eqp_size) return false;
	size_t inv_size = _inventory.Size();
	if(stone_index < 0 || (size_t)stone_index >= inv_size) return false;
	
	item & it1 = _equipment[rune_index];
	if(it1.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 
	
	item & it2 = _inventory[stone_index];
	if(it2.type != stone_id) return false;

	DATA_TYPE dt;
	const RUNE_SLOT_ESSENCE &ess= *(const RUNE_SLOT_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(stone_id, ID_SPACE_ESSENCE,dt); 
	if(dt != DT_RUNE_SLOT_ESSENCE || &ess == NULL)
	{
		return false;
	}

	bool rst = ((rune_item*)it1.body)->InstallSlot(&it1, this, rune_index, slot_index, stone_id); 
	if(rst)
	{
		UseItemLog(_inventory[stone_index],1);
		_inventory.DecAmount(stone_index, 1);
		_runner->player_drop_item(IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);

		//更新元婴活跃度
		EventUpdateLiveness(LIVENESS_UPDATE_TYPE_SPECIAL, LIVENESS_SPECIAL_YUANYING);
	}
	
	GLog::log(GLOG_INFO,"用户%d执行了元婴点星操作, rune_index=%d, slot_index=%d, stone_id=%d, result=%d", GetParent()->ID.id, rune_index, slot_index, stone_id, rst); 
	return rst;
}

void gplayer_imp::GainRuneExp(int exp)
{
	item & it = _equipment[item::EQUIP_INDEX_RUNE];
	if(it.GetItemType() != item_body::ITEM_TYPE_RUNE) return; 
	
	bool level_up = false;
	((rune_item*)it.body)->GainExp(item::BODY, exp, &it, this, item::EQUIP_INDEX_RUNE, level_up); 
}


bool gplayer_imp::IsLifeLongRewardDelivered(int level)
{
	return level >= 0 && level < MAX_TOWER_RAID_LEVEL && _tower_raid_info.levels[level].lifelong_reward_delivered;
}

bool gplayer_imp::DeliveryTowerReward(int raid_template_id, int level, size_t rewardItemIndex, bool longlife)
{
	if(GetWorldManager()->IsRaidWorld() && GetWorldManager()->GetRaidType() == RT_TOWER)
	{
		return false;
	}
	if(level < 0 || level > _tower_raid_info.tower_level)
	{
		return false;
	}
	if(longlife)
	{
		if(IsLifeLongRewardDelivered(level))
		{
			return false;
		}
	}
	else 
	{
		if(_tower_raid_info.end_level_today < 0 || _tower_raid_info.start_level_today < 0)
		{
			return false;
		}
		if(_tower_raid_info.end_level_today - _tower_raid_info.start_level_today <= 9 && (level > _tower_raid_info.end_level_today || level < _tower_raid_info.start_level_today))
		{
			return false;
		}
		else if(_tower_raid_info.end_level_today - _tower_raid_info.start_level_today > 9 && (level > _tower_raid_info.end_level_today || level < _tower_raid_info.end_level_today - 9))
		{
			return false;
		}
		else if(IsDailyRewardDeilivered(level))
		{
			return false;
		}
	}

	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_CONFIG& ttc = *(const TOWER_TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_CONFIG || &ttc == NULL)
	{
		__PRINTF("无效的爬塔副本模板: %d\n", raid_template_id);
		return false;
	}		

	const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& ttpc = *(const TOWER_TRANSCRIPTION_PROPERTY_CONFIG*) gmatrix::GetDataMan().get_data_ptr(ttc.property_config_id[level], ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG || &ttpc == NULL)
	{
		__PRINTF("无效的爬塔副本层属性模板: %d\n", ttc.property_config_id[level]);
		return false;
	}		
		
	if(longlife)
	{
		if(rewardItemIndex >= (sizeof(ttpc.life_time_award) / sizeof(TOWER_TRANSCRIPTION_PROPERTY_CONFIG::LIFE_TIME_AWARD_ITEM)))
		{
			__PRINTF("无效的奖励物品索引: raid_tid=%d, level=%d, rewardItemIndex=%d\n", raid_template_id, level, rewardItemIndex);
			return false;
		}
	}
	else 
	{
		if(rewardItemIndex >= (sizeof(ttpc.single_time_award) / sizeof(TOWER_TRANSCRIPTION_PROPERTY_CONFIG::SINGLE_TIME_AWARD_ITEM)))
		{
			__PRINTF("无效的奖励物品索引: raid_tid=%d, level=%d, rewardItemIndex=%d\n", raid_template_id, level, rewardItemIndex);
			return false;
		}
	}

	int item_id = longlife ? ttpc.life_time_award[rewardItemIndex].item_id : ttpc.single_time_award[rewardItemIndex].item_id;
	int item_cnt = longlife ? ttpc.life_time_award[rewardItemIndex].item_count : ttpc.single_time_award[rewardItemIndex].item_count;

	//生成物品
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		//_runner->error_message(S2C::ERR_VIPAWARD_INVALID_REQUEST);
		return false;
	}

	//计算需要多少个格子
	int need_empty_slot = item_cnt/pItem->pile_limit + ((item_cnt % pItem->pile_limit)?1:0);
	
	if(item_cnt <=0 || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//堆叠上限为1的奖励物品的数量必须为1
	if(pItem->pile_limit == 1 && 1 != item_cnt)
	{
		_runner->error_message(S2C::ERR_VIPAWARD_INVALID_REQUEST);
		return false;
	}

	//进包裹
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	DeliverItem(item_id, item_cnt, bind, 0, ITEM_INIT_TYPE_TASK);	
	GLog::log(GLOG_INFO,"用户%d领取爬塔副本%s奖励item_id=%d,item_cnt=%d", GetParent()->ID.id, longlife ? "终身" : "每日", item_id, item_cnt);
	if(longlife)
	{
		SetLifeLongRewardDelivered(level);
		_runner->set_tower_reward_state(1, level, true);
	}
	else
	{
		SetDailyRewardDelivered(level);
		_runner->set_tower_reward_state(0, level, true);
	}
	return true;
}

bool gplayer_imp::DeliveryTowerReward(int raid_template_id, int level, bool longlife)
{
	if(GetWorldManager()->IsRaidWorld() && GetWorldManager()->GetRaidType() == RT_TOWER)
	{
		return false;
	}
	if(level < 0 || level > _tower_raid_info.tower_level)
	{
		return false;
	}
	if(longlife)
	{
		if(IsLifeLongRewardDelivered(level))
		{
			return false;
		}
	}
	else 
	{
		if(_tower_raid_info.end_level_today < 0 || _tower_raid_info.start_level_today < 0)
		{
			return false;
		}
		if(_tower_raid_info.end_level_today - _tower_raid_info.start_level_today <= 9 && (level > _tower_raid_info.end_level_today || level < _tower_raid_info.start_level_today))
		{
			return false;
		}
		else if(_tower_raid_info.end_level_today - _tower_raid_info.start_level_today > 9 && (level > _tower_raid_info.end_level_today || level < _tower_raid_info.end_level_today - 9))
		{
			return false;
		}
		else if(IsDailyRewardDeilivered(level))
		{
			return false;
		}
	}

	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_CONFIG& ttc = *(const TOWER_TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_CONFIG || &ttc == NULL)
	{
		__PRINTF("无效的爬塔副本模板: %d\n", raid_template_id);
		return false;
	}		

	const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& ttpc = *(const TOWER_TRANSCRIPTION_PROPERTY_CONFIG*) gmatrix::GetDataMan().get_data_ptr(ttc.property_config_id[level], ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG || &ttpc == NULL)
	{
		__PRINTF("无效的爬塔副本层属性模板: %d\n", ttc.property_config_id[level]);
		return false;
	}		

	int item_ids[5];
	int item_cnts[5];
	bool item_bind[5];
	int need_empty_slot = 0;
	memset(item_ids, 0, sizeof(item_ids));
	memset(item_cnts, 0, sizeof(item_cnts));
	memset(item_bind, 0, sizeof(item_bind));
		
	if(longlife)
	{
		for(int i = 0; i < 5; i ++)
		{
			item_ids[i] = ttpc.life_time_award[i].item_id;
			item_cnts[i] = ttpc.life_time_award[i].item_count;

			if(item_ids[i] <= 0)
			{
				continue;
			}
			//生成物品
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_ids[i]);
			if(pItem == NULL || pItem->pile_limit <= 0)
			{
				_runner->error_message(S2C::ERR_TOWERWARD_INVALID_REQUEST);
				return false;
			}

			//堆叠上限为1的奖励物品的数量必须为1
			if(pItem->pile_limit == 1 && 1 != item_cnts[i])
			{
				_runner->error_message(S2C::ERR_TOWERWARD_INVALID_REQUEST);
				return false;
			}
			item_bind[i] = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);

			//计算需要多少个格子
			need_empty_slot += item_cnts[i]/pItem->pile_limit + ((item_cnts[i] % pItem->pile_limit)?1:0);
		}
	}
	else 
	{
		for(int i = 0; i < 5; i ++)
		{
			item_ids[i] = ttpc.single_time_award[i].item_id;
			item_cnts[i] = ttpc.single_time_award[i].item_count;

			//生成物品
			if(item_ids[i] <= 0)
			{
				continue;
			}
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_ids[i]);
			if(pItem == NULL || pItem->pile_limit <= 0)
			{
				_runner->error_message(S2C::ERR_TOWERWARD_INVALID_REQUEST);
				return false;
			}

			//堆叠上限为1的奖励物品的数量必须为1
			if(pItem->pile_limit == 1 && 1 != item_cnts[i])
			{
				_runner->error_message(S2C::ERR_TOWERWARD_INVALID_REQUEST);
				return false;
			}
			item_bind[i] = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);

			//计算需要多少个格子
			need_empty_slot += item_cnts[i]/pItem->pile_limit + ((item_cnts[i] % pItem->pile_limit)?1:0);
		}
	}

	
	if(need_empty_slot == 0 || (size_t)need_empty_slot > _inventory.Size() || !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//进包裹
	for(int i = 0; i < 5; i ++)
	{
		DeliverItem(item_ids[i], item_cnts[i], item_bind[i], 0, ITEM_INIT_TYPE_TASK);	
		GLog::log(GLOG_INFO,"用户%d领取爬塔副本%s奖励item_id=%d,item_cnt=%d", GetParent()->ID.id, longlife ? "终身" : "每日", item_ids[i], item_cnts[i]);
	}
	if(longlife)
	{
		SetLifeLongRewardDelivered(level);
		_runner->set_tower_reward_state(1, level, true);
	}
	else
	{
		SetDailyRewardDelivered(level);
		_runner->set_tower_reward_state(0, level, true);
	}
	return true;
}

void gplayer_imp::SaveTowerDataClient(archive& ar)
{
	ar << _tower_raid_info.tower_level << _tower_raid_info.start_level_today << _tower_raid_info.end_level_today;
	int cnt = 0;
	int best_time[10];
	memset(best_time, 0, sizeof(best_time));
	for(int i = 0; i < MAX_TOWER_RAID_LEVEL; i ++)
	{
		tower_level_info& info = _tower_raid_info.levels[i];
		best_time[cnt] += info.best_pass_time;
		if((i + 1) % 10 == 0)
		{
			cnt ++;
			if(cnt >= 10)
			{
				break;
			}
		}
	}
	ar.push_back(best_time, sizeof(best_time));
	ar << MAX_TOWER_RAID_LEVEL;
	for(int i = 0; i < MAX_TOWER_RAID_LEVEL; i ++)
	{
		tower_level_info& info = _tower_raid_info.levels[i];
		ar << info.daily_reward_delivered << info.lifelong_reward_delivered;
	}
}

void gplayer_imp::SaveTowerMonsterListClient(archive& ar)
{
	ar << _tower_raid_info.tower_monster_list.size();
	TowerMonsterMap::iterator it = _tower_raid_info.tower_monster_list.begin();
	for(; it != _tower_raid_info.tower_monster_list.end(); ++ it)
	{
		ar << it->first;
	}
}

void gplayer_imp::TowerLevelPass(int level, int pass_time)
{
	GLog::log(GLOG_INFO,"用户%d爬塔副本过关，职业=%d, level=%d，time=%d", _parent->ID.id, GetParent()->GetClass(), level, pass_time);
	__PRINTF("Player pass tower level=%d, pass_time=%d\n", level, pass_time);
	if(level < 0 || level >= MAX_TOWER_RAID_LEVEL || pass_time < 0)
	{
		return;
	}
	if(level > _tower_raid_info.tower_level)
	{
		_tower_raid_info.tower_level = level;
	}
	if(_tower_raid_info.levels[level].best_pass_time == 0 || pass_time < _tower_raid_info.levels[level].best_pass_time)
	{
		_tower_raid_info.levels[level].best_pass_time = pass_time;
	}
	_runner->tower_level_pass(level, _tower_raid_info.levels[level].best_pass_time);
}


void gplayer_imp::ResetTowerPlayerPet(bool fullHpAndMp)
{
	int pet_idx = _petman.GetActivePetIndex(this);
	if(pet_idx < 0) pet_idx = 0;
	if(_petman.IsPetCombine(pet_idx))
	{
		UncombinePet(pet_idx, _petman.GetPetCombineType(this, pet_idx));
	}
	A3DVECTOR pos;
	if(!_petman.TestCanSummonPet(this,pet_idx,pos))
	{
		SummonPet(pet_idx);
	}
	if(fullHpAndMp)
	{
		_petman.RecoverPetFull(this, pet_idx);
	}
}

void gplayer_imp::FullHPAndMP()
{
	Heal(GetMaxHP(), false, false);
	if(!IsRenMa()) InjectMana(GetMaxMP(), false);
	InjectDeity(GetMaxDP(), false);
}

/**
 * @brief ResetSkillProp 90级前免费洗点洗天书 
 */
bool gplayer_imp::PlayerResetSkillProp(int opcode)
{
	if(GetObjectLevel() > 90) 
	{
		return false;
	}

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return false;
	}

	//设置冷却
	switch(opcode)
	{
		case 1:
			//技能点
			if(!CheckCoolDown(COOLDOWN_INDEX_RESET_SKILL_PROP_SKILL)) 
			{
				return false;
			}
			SetCoolDown(COOLDOWN_INDEX_RESET_SKILL_PROP_SKILL, RESET_SKILL_PROP_COOLDOWN_TIME);
			break;

		case 2:
			//天书
			if(!CheckCoolDown(COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT)) 
			{
				return false;
			}
			SetCoolDown(COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT, RESET_SKILL_PROP_COOLDOWN_TIME);
			break;

		default:
			return false;
	}

	int rst = 0;
	const char * str = NULL;
	switch(opcode)
	{
	case 1:
		rst = _skill.Forget(true, this);	//全洗
		str = "所有类型";
		break;

	case 2:
		rst = _skill.ForgetTalent(this);	//洗天书
		if(rst > 0)
		{
			str = "天赋";
			ModifyTalentPoint(rst);
		}
		break;
	}

	if(rst <=0)	
	{
		if(1 == opcode)
		{
			ClrCoolDown(COOLDOWN_INDEX_RESET_SKILL_PROP_SKILL);
		}
		else if(2 == opcode)
		{
			ClrCoolDown(COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT);
		}

		_runner->error_message(S2C::ERR_NO_SKILL_TO_FORGET);
		return false;
	}
	GLog::log(GLOG_INFO,"用户%d执行了免费洗点洗天书操作(%d:%s)，恢复了'%d'点数，玩家等级=%d", 
			_parent->ID.id, opcode, str, rst, GetObjectLevel());
	_runner->get_skill_data();
	_runner->get_combine_skill_data();

	return true;
}

void gplayer_imp::PlayerGetTaskAward(int type)
{
	switch(type)
	{
		//补填身份信息奖励
		case 1:
		{
			if(_fill_info_mask & 0x00000010)
			{
				PlayerTaskInterface  task_if(this);
				OnTaskCheckDeliver(&task_if,26560,0);
			}
			else
			{
				return;
			}
		}
		break;

		//补填账号以及邮箱信息奖励
		case 2:
		{
			if(_fill_info_mask & 0x00000020)
			{
				PlayerTaskInterface  task_if(this);
				OnTaskCheckDeliver(&task_if,26561,0);
			}
			else
			{
				return;
			}
		}
		break;
	}
	
}

void gplayer_imp::SavePlayerBetData(archive& ar)
{
	int version = 1;
	ar << version;
	ar.push_back(_pk_bets, sizeof(_pk_bets));
	ar << (char)(_1st_bet ? 1 : 0);
	ar << (char)(_top3_bet ? 1 : 0);
	ar << (char)(_1st_bet_reward_deliveried ? 1 : 0);
	ar << (char)(_1st_bet_reward_result_deliveried ? 1 : 0);
	ar << (char)(_top3_bet_reward_deliveried ? 1 : 0);
	ar.push_back(_top3_bet_reward_result_deliveried, sizeof(_top3_bet_reward_result_deliveried));
}

void gplayer_imp::LoadPlayerBetData(const void* buf, size_t size)
{
	if(size <= 0) return;
	raw_wrapper ar(buf, size);
	int version = 0;
	ar >> version;
	if(version == 1)
	{
		ar.pop_back(_pk_bets, sizeof(_pk_bets));
		char res = -1;
		ar >> res;
		_1st_bet = (res == 1);
		ar >> res;
		_top3_bet = (res == 1);
		ar >> res;
		_1st_bet_reward_deliveried = (res == 1);
		ar >> res;
		_1st_bet_reward_result_deliveried = (res == 1);
		ar >> res;
		_top3_bet_reward_deliveried = (res == 1);
		ar.pop_back(_top3_bet_reward_result_deliveried, sizeof(_top3_bet_reward_result_deliveried));
	}
}


bool gplayer_imp::PlayerPk1stBet(size_t area, int cnt, const void* buf)
{
	if(area < 0 || area >= sizeof(_pk_bets) / sizeof(pk_bet))
	{
		return false;
	}
	const player_template::pk_bet_data& data = player_template::GetPKBetData();
	int curr_time = g_timer.get_systime() + gmatrix::Instance()->GetServerGMTOff();
	if(curr_time < data.guess_start_time || curr_time > data.guess_end_time)
	{
		return false;
	}	
	item_list & inv = GetInventory();
	C2S::CMD::pk_1st_guess::bet_item* items = (C2S::CMD::pk_1st_guess::bet_item*)buf;
	int itemCnt = 0;
	for(int i = 0; i < cnt; i ++)
	{
		C2S::CMD::pk_1st_guess::bet_item& item = items[i];
		if(!inv.IsItemExist(item.index, data.champion_guess_item, item.cnt))
		{
			return false;
		}
		itemCnt += item.cnt;
	}
	if(itemCnt % 10 != 0)
	{
		return false;
	}
	if(_pk_bets[area].bet_1st_num + (itemCnt / 10) > 10000)
	{
		return false;
	}

	for(int i = 0; i < cnt; i ++)
	{
		C2S::CMD::pk_1st_guess::bet_item& item = items[i];
		UseItemLog(inv,item.index, item.cnt);
		inv.DecAmount(item.index, item.cnt);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item.index, data.champion_guess_item, item.cnt, S2C::DROP_TYPE_USE);
	}
	_pk_bets[area].bet_1st_num += (itemCnt / 10);
	_1st_bet = true;
	_runner->get_player_pk_bet_data();
	return true;
}

bool gplayer_imp::PlayerPkTop3Bet(size_t area, int cnt, const void* buf, bool cancel)
{
	if(area < 0 || area >= sizeof(_pk_bets) / sizeof(pk_bet))
	{
		return false;
	}
	const player_template::pk_bet_data& data = player_template::GetPKBetData();
	int curr_time = g_timer.get_systime() + gmatrix::Instance()->GetServerGMTOff();
	if(curr_time < data.guess_start_time || curr_time > data.guess_end_time)
	{
		return false;
	}	
	if(!cancel)
	{
		int betCnt = 0;
		for(size_t i = 0; i < sizeof(_pk_bets) / sizeof(pk_bet); i ++)
		{
			if(_pk_bets[i].top3)
			{
				betCnt ++;
				if(betCnt >= 3)
				{
					return false;
				}
			}
		}

		item_list & inv = GetInventory();
		C2S::CMD::pk_top3_guess::bet_item* items = (C2S::CMD::pk_top3_guess::bet_item*)buf;
		int itemCnt = 0;
		for(int i = 0; i < cnt; i ++)
		{
			C2S::CMD::pk_top3_guess::bet_item& item = items[i];
			if(!inv.IsItemExist(item.index, data.guess_item, item.cnt))
			{
				return false;
			}
			itemCnt += item.cnt;
		}
		if(itemCnt != 10)
		{
			return false;
		}

		for(int i = 0; i < cnt; i ++)
		{
			C2S::CMD::pk_top3_guess::bet_item& item = items[i];
			UseItemLog(inv,item.index, item.cnt);
			inv.DecAmount(item.index, item.cnt);
			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item.index, data.guess_item, item.cnt, S2C::DROP_TYPE_USE);
		}
	}
	_pk_bets[area].top3 = !cancel;
	if(!_top3_bet && !cancel)
	{
		_top3_bet = true;
	}
	_runner->get_player_pk_bet_data();
	return true;
}

bool gplayer_imp::DeliveryPk1stBetReward()
{
	if(_1st_bet_reward_deliveried)
	{
		return false;
	}
	if(!_1st_bet)
	{
		return false;
	}
	const player_template::pk_bet_data& data = player_template::GetPKBetData();
	if(GetInventory().GetEmptySlotCount() < 1)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	if(!CreateItem(data.champion_guess_award, 1, false))
	{
		return false;
	}
	_1st_bet_reward_deliveried = true;
	_runner->get_player_pk_bet_data();
	return true;
}

bool gplayer_imp::DeliveryPk1stBetResultReward()
{
	if(_1st_bet_reward_result_deliveried)
	{
		return false;
	}
	const player_template::pk_bet_data& data = player_template::GetPKBetData();
	int curr_time = g_timer.get_systime() + gmatrix::Instance()->GetServerGMTOff();
	if(curr_time < data.accept_award_start_time || curr_time > data.accept_award_end_time)
	{
		return false;
	}	
	if(data.first_id < 0 || data.first_id >= 8)
	{
		return false;
	}

	int item_id = data.champion_guess_item;
	int item_cnt = (int)(data.award_back_ratio * _pk_bets[data.first_id].bet_1st_num * 10 + 0.5f);
	if(item_cnt <= 0)
	{
		return false;
	}
	
	//生成物品
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		return false;
	}

	//计算需要多少个格子
	int need_empty_slot = item_cnt/pItem->pile_limit + ((item_cnt % pItem->pile_limit)?1:0);
	
	if(item_cnt <=0 || need_empty_slot == 0
			|| (size_t)need_empty_slot > _inventory.Size() 
			|| !InventoryHasSlot(need_empty_slot))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	for(int i = 0; i < need_empty_slot; i ++)
	{
		if(item_cnt > (int)pItem->pile_limit)
		{
			//进包裹
			DeliverItem(item_id, pItem->pile_limit, false, 0, ITEM_INIT_TYPE_TASK);	
			GLog::log(GLOG_INFO,"用户%d领取2012PK竞猜奖励item_id=%d,item_cnt=%d", GetParent()->ID.id, item_id, pItem->pile_limit);
			item_cnt -= pItem->pile_limit;
		}
		else
		{
			DeliverItem(item_id, item_cnt, false, 0, ITEM_INIT_TYPE_TASK);	
			GLog::log(GLOG_INFO,"用户%d领取2012PK竞猜奖励item_id=%d,item_cnt=%d", GetParent()->ID.id, item_id, item_cnt);
		}
	}
	_1st_bet_reward_result_deliveried = true;
	_runner->get_player_pk_bet_data();
	return true;
}

bool gplayer_imp::DeliveryPkTop3BetReward()
{
	if(!_top3_bet)
	{
		return false;
	}
	if(_top3_bet_reward_deliveried)
	{
		return false;
	}
	const player_template::pk_bet_data& data = player_template::GetPKBetData();
	if(GetInventory().GetEmptySlotCount() < 1)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	if(!CreateItem(data.guess_award_item, 1, false))
	{
		return false;
	}
	_top3_bet_reward_deliveried = true;
	_runner->get_player_pk_bet_data();
	return true;
}

bool gplayer_imp::DeliveryPkTop3BetResultReward(char type)
{
	const player_template::pk_bet_data& data = player_template::GetPKBetData();
	int curr_time = g_timer.get_systime() + gmatrix::Instance()->GetServerGMTOff();
	if(curr_time < data.accept_award_start_time || curr_time > data.accept_award_end_time)
	{
		return false;
	}	
	if(GetInventory().GetEmptySlotCount() < 1)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	if(data.first_id < 0 || data.second_id < 0 || data.third_id < 0 || data.first_id >= 8 || data.second_id >= 8 || data.third_id >= 8)
	{
		return false;
	}
			
	int hitCnt = 0;
	for(int i = 0; i < (int)(sizeof(_pk_bets) / sizeof(pk_bet)); i ++)
	{
		if(_pk_bets[i].top3 && (i == data.first_id || i == data.second_id || i == data.third_id))
		{
			hitCnt ++;
		}
	}
	if(type == 0 && hitCnt == 3)
	{
		if(_top3_bet_reward_result_deliveried[0])
		{
			return false;
		}
		if(!CreateItem(data.guess_award_item3, 1, false))
		{
			return false;
		}
		_top3_bet_reward_result_deliveried[0] = true;
		_runner->get_player_pk_bet_data();
		return true;
	}
	else if(type == 1 && hitCnt >= 2)
	{
		if(_top3_bet_reward_result_deliveried[1])
		{
			return false;
		}
		if(!CreateItem(data.guess_award_item2, 1, false))
		{
			return false;
		}
		_top3_bet_reward_result_deliveried[1] = true;
		_runner->get_player_pk_bet_data();
		return true;
	}
	else if(type == 2 && hitCnt >= 1)
	{
		if(_top3_bet_reward_result_deliveried[2])
		{
			return false;
		}
		if(!CreateItem(data.guess_award_item1, 1, false))
		{
			return false;
		}
		_top3_bet_reward_result_deliveried[2] = true;
		_runner->get_player_pk_bet_data();
		return true;
	}
	return false;
}

void gplayer_imp::SavePlayerBetDataClient(archive& ar)
{
	S2C::CMD::pk_bet_data data;
	for(size_t i = 0; i < sizeof(_pk_bets) / sizeof(pk_bet); i ++)
	{
		data._pk_bets[i].bet_1st_num = _pk_bets[i].bet_1st_num;
		data._pk_bets[i].top3 = _pk_bets[i].top3;
	}
	data._top3_bet = _top3_bet;
	data._1st_bet_reward_deliveried =_1st_bet_reward_deliveried;
	data._1st_bet_reward_result_deliveried = _1st_bet_reward_result_deliveried;
	data._top3_bet_reward_deliveried = _top3_bet_reward_deliveried;
	for(int i = 0; i < 3; i ++)
	{
		data._top3_bet_reward_result_deliveried[i] = _top3_bet_reward_result_deliveried[i];
	}
	ar.push_back(&data, sizeof(data));
}

void gplayer_imp::PlayerStartSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp)
{
	StartSpecialMove(velocity, acceleration, cur_pos, collision_state, timestamp);
}

void gplayer_imp::PlayerSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, unsigned short stamp, char collision_state, int timestamp)
{
	SpecialMove(velocity, acceleration, cur_pos, stamp, collision_state, timestamp);
}

void gplayer_imp::PlayerStopSpecialMove(unsigned char dir, A3DVECTOR & cur_pos, unsigned short stamp)
{
	StopSpecialMove(dir, cur_pos, stamp);
}


//消费值相关
int gplayer_imp::CheckEquipConsumption(int item_type)
{
	int consumption_value = -1;
	if((consumption_value = gmatrix::GetConsumptionValue(gmatrix::EQUIP_CONSUMPTION, item_type)) > 0)
	{
		//配置表中取到的消费值是已经乘以CONSUMPTION_SCALE后得到的整数
		//GLog::log(GLOG_INFO, "玩家%d通过type:%d方式获得消费点数value:%d,id:%d,count:%d,origin_value:%lld", _parent->ID.id, gmatrix::EQUIP_CONSUMPTION, consumption_value, item_type, 1, _consumption_value);
		AddConsumptionValue((int64_t)consumption_value);
	}
	
	return consumption_value;
}

int gplayer_imp::CheckUseItemConsumption(int item_type, int count)
{
	int consumption_value = -1;
	if((consumption_value = gmatrix::GetConsumptionValue(gmatrix::USEITEM_CONSUMPTION, item_type)) > 0)
	{
		//配置表中取到的消费值是已经乘以CONSUMPTION_SCALE后得到的整数
		//GLog::log(GLOG_INFO, "玩家%d通过type:%d方式获得消费点数value:%d,id:%d,count:%d,origin_value:%lld", _parent->ID.id, gmatrix::USEITEM_CONSUMPTION, consumption_value * count, item_type, count, _consumption_value);
		AddConsumptionValue( (int64_t)((int64_t)consumption_value * (int64_t)count) );
	}

	return (consumption_value * count);
}

int gplayer_imp::CheckSpecialConsumption(int item_type, int count)
{
	int consumption_value = -1;
	if((consumption_value = gmatrix::GetConsumptionValue(gmatrix::SPECIAL_CONSUMPTION, item_type)) > 0)
	{
		//配置表中取到的消费值是已经乘以CONSUMPTION_SCALE后得到的整数
		//GLog::log(GLOG_INFO, "玩家%d通过type:%d方式获得消费点数value:%d,id:%d,count:%d,origin_value:%lld", _parent->ID.id, gmatrix::SPECIAL_CONSUMPTION, consumption_value * count, item_type, count, _consumption_value);
		AddConsumptionValue( (int64_t)((int64_t)consumption_value * (int64_t)count) );
	}

	return (consumption_value * count);
}

int64_t gplayer_imp::LoadConsumptionValue(int64_t value)
{
	if(value >= 0)
	{
		_consumption_value = value;
	}

	return _consumption_value;
}

void gplayer_imp::PlayerChangeName(char * name, size_t name_len)
{
	memset(_username, 0, sizeof(_username));
	if(name_len > MAX_USERNAME_LENGTH) name_len = MAX_USERNAME_LENGTH;
	memcpy(_username, name, name_len);
	_username_len = name_len;

	//没有检查_pet_equip_inventory和_pocket_inventory这两个包裹，因为这两个包裹不应该含有需要改名字的物品
	//添加新的包裹后也需要修改这里	

	ChangeItemOwnerName(_inventory);
	ChangeItemOwnerName(_equipment);
	ChangeItemOwnerName(_task_inventory);
	ChangeItemOwnerName(_trashbox.Backpack());
	ChangeItemOwnerName(_pet_bedge_inventory);
	ChangeItemOwnerName(_fashion_inventory);
	ChangeItemOwnerName(_mount_wing_inventory);

	_runner->player_change_name(_parent->ID.id, name_len, name);
	GLog::log(GLOG_INFO,"用户%d完成了改名操作", _parent->ID.id);
	
}

void gplayer_imp::PlayerChangeFactionName(int id, int fid, char type, char * name, size_t name_len)
{

	_runner->change_faction_name(id, fid, type, name_len, name);
	GLog::log(GLOG_INFO,"用户%d完成了改名操作", _parent->ID.id);
	
}


void gplayer_imp::ChangeItemOwnerName(item_list & inv)
{
	for(size_t i = 0; i < inv.Size(); ++i)
	{
		item & it = inv[i];
		if(it.type != -1 && it.body != NULL)
		{
			it.ChangeOwnerName(inv.GetLocation(), i, this);
		}
	}
}

/*
各重职业的对应的职业ID
   
职业 一 二 三 四 五   ID
少侠  0		      0
鬼王  1  2  3 13 14   1
合欢  4  5  6 16 17   2
青云  7  8  9 19 20   3
天音 10 11 12 22 23   4
鬼道 25 26 27 28 29   5
焚香 64 65 66 67 68   6 
九黎 33 34 35 36 37   7
烈山 39 40 41 42 43   8
怀光 45 46 47 48 49   9
天华 51 52 53 54 55   10
太昊 96 97 98 99 100  11
辰皇 56 57 58 59 60   12
萝莉 102 103 104 105 106
人马 108 109 110 111 112
*/

int gplayer_imp::GetPlayerOccupation(int cls)
{
	if(cls == 0) return 0;

	int occupation_list[][5] =
	{
		{1, 2, 3, 13, 14},
		{4, 5, 6, 16, 17},
		{7, 8, 9, 19, 20},
		{10, 11, 12, 22, 23},
		{25,26, 27, 28, 29},
		{64, 65, 66, 67, 68},
		{33, 34, 35, 36, 37},
		{39, 40, 41, 42, 43},
		{45, 46, 47, 48, 49},
		{51, 52, 53, 54, 55},
		{96, 97, 98, 99, 100},
		{56, 57, 58, 59, 60},
		{102, 103, 104, 105, 106},
		{108, 109, 110, 111, 112}
	}; 

	for(size_t i = 0; i < sizeof(occupation_list)/5*sizeof(int); ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			if(cls == occupation_list[i][j])
			{
				return i + 1;
			}
		}
	}

	return -1;
}

void gplayer_imp::PlayerApplyCollisionRaid(int raid_id, int raid_template_id, bool is_team, char is_cross)
{
	if(raid_id != COLLISION_RAID_ID && raid_id != COLLISION_RAID_ID2) return;
	if(is_team && (!IsInTeam() || !IsTeamLeader())) return;
	if(!GetWorldManager()->IsGlobalWorld()) return; 
	
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL || raid_config.map_id != raid_id)
	{
		printf("无效的副本模板: %d\n", raid_template_id);
		return;
	}	

	if(!CheckEnterRaid(raid_id, raid_template_id, 2, NULL, 0))
	{
		printf("不符合进入条件\n");
		return;
	}

	GMSV::raid_apply_info role_info;
	std::vector<GMSV::raid_apply_info> apply_id;

	int reborn_cnt = GetRebornCount(); 
	if(is_team)
	{
		int count = _team.GetMemberNum();
		for(int i = 0; i < count; ++i)
		{
			const player_team::member_entry &ent = _team.GetMember(i);
			if(reborn_cnt != ent.data.reborn_cnt) return;
			
			role_info.roleid = ent.id.id;
		        role_info.score = ent.data.collision_score;	
			role_info.occupation = GetPlayerOccupation(ent.data.cls_type); 
			apply_id.push_back(role_info);
		}
	}
	else
	{
		role_info.roleid = _parent->ID.id;
		role_info.score = GetCollisionScore();
		role_info.occupation = GetPlayerOccupation(GetPlayerClass()); 

		apply_id.push_back(role_info);
	}

	GMSV::SendApplyCollisionRaid(raid_id, apply_id, reborn_cnt, is_cross);
}

//物品回购
bool gplayer_imp::AddToRepurchaseInv(const item & item_record, int count)
{
	if(item_record.proc_type & item::ITEM_PROC_TYPE_RECYCLE) return false;

	if((int)item_record.count < count)
	{
		return false;
	}

	std::list<item>::iterator it_inv;
	while(_repurchase_inventory.size() >= REPURCHASE_INVENTORY_SIZE)
	{
		it_inv = _repurchase_inventory.begin();	
		(*it_inv).Release();
		_repurchase_inventory.pop_front();
	}

	item tmpitem;
	item_data data;
	item_list::ItemToData(item_record, data);
	if(!MakeItemEntry(tmpitem, data))
	{
		tmpitem.Clear();
		return false;
	}

	tmpitem.count = count;
	_repurchase_inventory.push_back(tmpitem);
	GLog::log(GLOG_INFO,"用户%d放入回购包裹%d个%d物品",_parent->ID.id, count, tmpitem.type);

	//body的析构在_repurchase_inventory中统一进行
	tmpitem.Clear();

	return true;
}

bool gplayer_imp::RepurchaseItem(size_t inv_index, int type, size_t count)
{
	if(inv_index >= _repurchase_inventory.size()) return false;
	if(GetInventory().GetEmptySlotCount() < 1)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	size_t tmpindex  = inv_index;
	std::list<item>::iterator it_inv = _repurchase_inventory.begin();
	for(; tmpindex > 0; tmpindex--) ++it_inv;

	item & item_repurchase = (*it_inv);
	if(item_repurchase.type == -1 || item_repurchase.type != type || item_repurchase.count < count) return false;
	if(!item_repurchase.CanSell()) return false;

	//进包裹,重新计算count
	item_data tmpdata;
	item_list::ItemToData(item_repurchase, tmpdata);
	if(count > tmpdata.pile_limit) count = tmpdata.pile_limit;
	tmpdata.count = count;

	//Menoy
	float decmoney = (float)item_repurchase.price * count;
	if(decmoney < 0) decmoney = 0;
	if(decmoney >1e11) decmoney = 0;
	size_t need_money = (size_t)(decmoney+0.5f);
	if(GetMoney() < need_money) 
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	//扣钱、获得物品
	item_data * data = DupeItem(tmpdata);
	if(ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
	SpendMoney(need_money);
	_runner->spend_money(need_money);

	//对回购包裹进行操作
	if(item_repurchase.count == count)
	{
		//(*it_inv).Release();
		//_repurchase_inventory.erase(it_inv);
		//it_inv = _repurchase_inventory.end();

		//在UpdateRepurchaseInv()里进行析构,这里只做一个标记
		(*it_inv).type = -1;
	}
	else
	{
		(*it_inv).count -= count;
	}

	GLog::log(GLOG_INFO,"用户%d回购%d个%d物品",_parent->ID.id, count, type);
	return true;
}

void gplayer_imp::UpdateRepurchaseInv()
{
	std::list<item>::iterator it_inv = _repurchase_inventory.begin();
	while(it_inv != _repurchase_inventory.end())
	{
		if(-1 == (*it_inv).type)
		{
			(*it_inv).Release();
			_repurchase_inventory.erase(it_inv++);
			continue;
		}
		++it_inv;
	}
}

void gplayer_imp::SendRepurchaseInvData()
{
	raw_wrapper ar;

	std::list<item>::iterator it_inv = _repurchase_inventory.begin();
	size_t count = _repurchase_inventory.size();
	ar << count;
	size_t i = 0; 
	for(; it_inv != _repurchase_inventory.end(); ++it_inv)
	{
		item & it = (*it_inv);
		count --;

		ar << i;
		ar << it.type;
		ar << it.expire_date;
		ar << (int)(it.GetProctypeState());
		ar << it.count;
		/*size_t len;
		const void * buf;
		it.GetItemDataForClient(&buf, len);
		ar << (short)len;
		if(len)
		{
			ar.push_back(buf,len);
		}*/
		item_data_client data;
		it.GetItemDataForClient(data);
		if(data.use_wrapper)
		{
			ar << (short)data.ar.size();
			if(data.ar.size())
			{
				ar.push_back(data.ar.data(), data.ar.size());
			}
		}
		else
		{
			ar << (short)data.content_length;
			if(data.content_length)
			{
				ar.push_back(data.item_content,data.content_length);
			}
		}

		i++;
	}

	ASSERT(count == 0);
	_runner->repurchase_inventory_data(REPURCHASE_INVENTORY_SIZE, ar.data(), ar.size());
}

void
gplayer_imp::PlayerExchangeMountWingItem(size_t index1, size_t index2)
{
	if(!_mount_wing_inventory.ExchangeItem(index1,index2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->exchange_mount_wing_item(index1,index2);
}

void
gplayer_imp::PlayerExchangeInvMountWing(size_t idx_inv, size_t idx_mw)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(idx_inv >= _inventory.Size() || idx_mw >= _mount_wing_inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if( _inventory[idx_inv].type != -1 && 
	   ((_inventory[idx_inv].GetItemType() != item_body::ITEM_TYPE_MOUNT) && ((_inventory[idx_inv].equip_mask & item::EQUIP_MASK_WING) == 0)) )
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	item it;
	_mount_wing_inventory.Exchange(idx_mw, it);
	_inventory.Exchange(idx_inv, it);
	_mount_wing_inventory.Exchange(idx_mw, it);
	_runner->exchange_inventory_mountwing_item(idx_inv, idx_mw);
}

void 
gplayer_imp::PlayerExchangeEquipMountWing(size_t index_equ, size_t index_mw)
{
	if(index_mw >= _mount_wing_inventory.Size() || index_equ >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	bool type1 = (_mount_wing_inventory[index_mw].type == -1);
	bool type2 = (_equipment[index_equ].type == -1);
	if(type1 && type2)
	{
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}

	if( !type1 && ((_mount_wing_inventory[index_mw].equip_mask & item::EQUIP_MASK_WING) == 0) )
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if( !type2 && ((_equipment[index_equ].equip_mask & item::EQUIP_MASK_WING) == 0) ) 
		//(item::EQUIP_MASK_FASHION_EYE | item::EQUIP_MASK_FASHION_HEAD | item::EQUIP_MASK_FASHION_BODY | item::EQUIP_MASK_FASHION_FOOT))== 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	
	if(!type1)
	{
		if(!(_mount_wing_inventory[index_mw].equip_mask & (1 << index_equ)))
		{	
			//装备位置不对
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}

		//交换之
		if(!ExchangeEquipMountWingItem(index_mw,index_equ))
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		}
		else
		{
			//交换成功，检查一下是否需要进行绑定操作
			item & it = _equipment[index_equ];
			ASSERT(it.type != -1);
			bool notify_equip_item = false;
			if(it.IsBindOnEquip())
			{
				//满足绑定条件，进行绑定
				it.BindOnEquip();
				notify_equip_item = true;
				GLog::log(LOG_INFO,"用户%d装备绑定物品%d,GUID(%d,%d),mount_wing_inv",_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
				//检查是否需要记录消费值
				CheckEquipConsumption(it.type);
			}
		
			int count1 = _mount_wing_inventory[index_mw].count;
			int count2 = _equipment[index_equ].count;
			ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
			_runner->exchange_equipment_mountwing_item(index_equ, index_mw);
			int64_t id1 = _equipment[index_equ].type | _equipment[index_equ].GetIdModify();
			CalcEquipmentInfo();
			_runner->equipment_info_changed(1<<index_equ , 0, &id1,sizeof(id1));//此函数使用了CalcEquipmentInfo的结果
			if(notify_equip_item)
			{
				PlayerGetItemInfo(IL_EQUIPMENT,index_equ);
			}

		}
		IncEquipChangeFlag();
		return ;
	}
	//拿下原来的，并刷新装备
	item  it1;
	_equipment.Remove(index_equ,it1);
	bool bRst = _mount_wing_inventory.Put(index_mw,it1);
	ASSERT(bRst);
	if(bRst)
	{
		it1.Clear();
	}
	else
	{
		//记录错误日志
		GLog::log(GLOG_ERR,"装备物品时发生致命错误,mount_wing_inv");
		it1.Release();
	}
	RefreshEquipment();
	int count1 = _mount_wing_inventory[index_mw].count;
	ASSERT(count1 > 0);
	_runner->exchange_equipment_mountwing_item(index_equ, index_mw);
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1<<index_equ , 0, 0); //此函数使用了CalcEquipmentInfo的结果
	IncEquipChangeFlag();
}

int
gplayer_imp::TakeOutMountWingItem(int mw_index, int item_id, int count)
{
	if(item_id <=0 || count <=0 || mw_index <0) return -1;
	if(_mount_wing_inventory.Size() < (size_t)(mw_index+1)) return -1;
	item &it = _mount_wing_inventory[mw_index];
	
	if(it.type != item_id) return -1;
	if(it.count < (size_t)count) count = it.count;
	UseItemLog(_mount_wing_inventory[mw_index], count);
	
	_mount_wing_inventory.DecAmount(mw_index, count);
	_runner->player_drop_item(IL_MOUNT_WING,mw_index,item_id,count,S2C::DROP_TYPE_TAKEOUT);

	return mw_index;
}

//轩辕光暗值
void gplayer_imp::SetDarkLight(int value)
{
	int newstate = -1;
	if(_dark_light >= 0 && value < 0)
	{
		newstate = gplayer::DLTYPE_DARK;
	}
	if(_dark_light < 0 && value >= 0)
	{
		newstate = gplayer::DLTYPE_LIGHT;
	}
	_dark_light = value;
	if(value > 100)
	{
		_dark_light = 100;
	}
	if(value < -100)
	{
		_dark_light = -100;
	}
	if(newstate != -1)
	{
		gplayer* pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_XY_DARKLIGHT_STATE);
		pPlayer->darklight_state = newstate;
		_runner->player_darklight_state();
	}
}

int gplayer_imp::GetDarkLight()
{
	return _dark_light;
}

void gplayer_imp::IncDarkLight(int inc)
{
	if(inc > 0)
	{
		int newstate = -1;
		if(_dark_light < 0 && _dark_light + inc >= 0)
		{
			newstate = gplayer::DLTYPE_LIGHT;
		}
		_dark_light += inc;
		if(_dark_light > 100)
		{
			_dark_light = 100;
		}
		if(newstate != -1)
		{
			gplayer* pPlayer = GetParent();
			pPlayer->SetExtraState(gplayer::STATE_XY_DARKLIGHT_STATE);
			pPlayer->darklight_state = newstate;
			_runner->player_darklight_state();
		}
	}
}

void gplayer_imp::DecDarkLight(int dec)
{
	if(dec > 0)
	{
		int newstate = -1;
		if(_dark_light >= 0 && _dark_light - dec < 0)
		{
			newstate = gplayer::DLTYPE_DARK;
		}
		_dark_light -= dec;
		if(_dark_light < -100)
		{
			_dark_light = -100;
		}
		if(newstate != -1)
		{
			gplayer* pPlayer = GetParent();
			pPlayer->SetExtraState(gplayer::STATE_XY_DARKLIGHT_STATE);
			pPlayer->darklight_state = newstate;
			_runner->player_darklight_state();
		}
	}
}

//轩辕灵体
void gplayer_imp::SetDarkLightSpirit(int idx, char type)
{
	if(!IsXuanYuan())
	{
		return;
	}
	if(idx < 0 || idx >= MAX_XUANYUAN_SPIRITS)
	{
		return;
	}
	if(type < gplayer::SPIRIT_TYPE_NONE || type >= gplayer::SPIRIT_TYPE_NUM)
	{
		return;
	}
	gplayer* pPlayer = GetParent();
	pPlayer->SetExtraState(gplayer::STATE_XY_SPIRITS);
	pPlayer->darklight_spirits[idx] = type;
	if(type == gplayer::SPIRIT_TYPE_NONE)
	{
		OrderDarkLightSpirit();	
	}
	_runner->player_darklight_spirit();
}

char gplayer_imp::GetDarkLightSpirit(int idx)
{
	if(!IsXuanYuan())
	{
		return -1;
	}
	if(idx < 0 && idx >= MAX_XUANYUAN_SPIRITS)
	{
		return gplayer::SPIRIT_TYPE_NONE;
	}
	gplayer* pPlayer = GetParent();
	return pPlayer->darklight_spirits[idx];
}

void gplayer_imp::AddDarkLightSpirit(char type)
{
	if(!IsXuanYuan())
	{
		return;
	}
	if(type < gplayer::SPIRIT_TYPE_DARK || type >= gplayer::SPIRIT_TYPE_NUM)
	{
		return;
	}
	gplayer* pPlayer = GetParent();
	pPlayer->SetExtraState(gplayer::STATE_XY_SPIRITS);
	int idx = -1;
	for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
	{
		if(pPlayer->darklight_spirits[i] == gplayer::SPIRIT_TYPE_NONE)
		{
			idx = i;
			break;
		}
	}
	if(idx == -1)
	{
		for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
		{
			if(pPlayer->darklight_spirits[i] != gplayer::SPIRIT_TYPE_DARKLIGHT)
			{
				pPlayer->darklight_spirits[i] = gplayer::SPIRIT_TYPE_NONE;
				break;
			}
		}	
	}
	OrderDarkLightSpirit();
	for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
	{
		if(pPlayer->darklight_spirits[i] == gplayer::SPIRIT_TYPE_NONE)
		{
			pPlayer->darklight_spirits[i] = type;
			break;
		}
	}
	_runner->player_darklight_spirit();
}

void gplayer_imp::DelDarkLightSpirit(int idx)
{
}

void gplayer_imp::OrderDarkLightSpirit()
{
	gplayer* pPlayer = GetParent();
	for(int i = 0; i < MAX_XUANYUAN_SPIRITS - 1; i ++)
	{
		if(pPlayer->darklight_spirits[i] == gplayer::SPIRIT_TYPE_NONE)
		{
			for(int j = i; j < MAX_XUANYUAN_SPIRITS - 1; j ++)
			{
				char s = pPlayer->darklight_spirits[j];
				pPlayer->darklight_spirits[j] = pPlayer->darklight_spirits[j + 1];
				pPlayer->darklight_spirits[j + 1] = s;
			}
		}
	}
}

void gplayer_imp::CastDarkLightSpiritOnAttackSkill(const XID& target)
{
	if(!IsDarkLightForm(DLFORM_NORMAL))
	{
		return;
	}
	for(int i = 0; i < MAX_XUANYUAN_SPIRITS; i ++)
	{
		char spirit_type = GetDarkLightSpirit(i);
		if(spirit_type == gplayer::SPIRIT_TYPE_NONE)
		{
			continue;
		}
		spirit_session_skill *pSkill= new spirit_session_skill(this);
		static int spirit_skills[3] = {4003, 4004, 4037};
		int id = target.id;
		int skill_id = spirit_skills[spirit_type - 1];
		if(spirit_type == 3)
		{
			skill_id = _dark_light >= 0 ? 4004 : 4003;
		}
		pSkill->SetTarget(skill_id, OI_GetPVPMask(), 1, &id, 0, -1, -1, false, _parent->pos, (char)i);
		if(AddSpiritSession(pSkill))
		{
			SetDarkLightSpirit(i, gplayer::SPIRIT_TYPE_NONE);
		}
		return;
	}
}

void gplayer_imp::SummonCastSkill(int summonTid, const XID& target, int skillid, int skilllevel)
{
	XID id;
	if(_summonman.IsSummonExist(this, summonTid, id))
	{
		msg_summon_cast_skill data;
		data.target = target;
		data.skillid = skillid;
		data.skilllevel = skilllevel;
		SendTo<0>(GM_MSG_SUMMON_CAST_SKILL, id, 0, &data, sizeof(data));
	}
}

bool gplayer_imp::GetTalismanRefineSkills(int& level, std::vector<short>& skills)
{
	item & it = _equipment[item::EQUIP_INDEX_TALISMAN1];
	if(it.type == -1 || it.body == NULL) 
	{
		return false;
	}
	it.body->GetRefineSkills(&it, level, skills);
	return true;
}

void gplayer_imp::SetTalismanEmbedSkillCooltime(int skill_id, int cooltime)
{
	item& it = _equipment[item::EQUIP_INDEX_TALISMAN1];
	if(it.type == -1 || it.body == NULL) 
	{
		return;
	}
	std::vector<int> addons;
	it.body->GetEmbedAddons(&it, addons);
	std::vector<int> skills;
	bool flag = false;
	for(size_t i = 0; i < addons.size(); ++ i)
	{
		const addon_data_spec * pSpec = addon_data_man::Instance().GetAddon(addons[i]);
		if(!pSpec || pSpec->data.id != 38 || pSpec->data.arg[0] <= 0)
		{
			continue;
		}
		if(pSpec->data.arg[0] == skill_id)
		{
			flag = true;
		}
		else
		{
			skills.push_back(pSpec->data.arg[0]);
		}
	}

	if(flag)
	{
		for(size_t i = 0; i < skills.size(); i ++)
		{
			SetCoolDown(skills[i] + SKILL_COOLINGID_BEGIN, cooltime);
		}
	}

}

void gplayer_imp::ActiveTalismanSkillEffects(const XID& target, const msg_talisman_skill* mts)
{
	//int level = 0;
	//std::vector<int> skills;
	//GetTalismanRefineSkills(level, skills);
	for(size_t i = 0; i < sizeof(mts->skillList) / sizeof(mts->skillList[0]); i ++)
	{
		int skillid = mts->skillList[i];
		int range_type = GNET::SkillWrapper::RangeType(skillid);
		if(range_type == -1)
		{
			continue;
		}
		int type = GNET::SkillWrapper::GetType(skillid);
		if(type != 3 || range_type != 0)
		{
			continue;
		}

		enchant_msg enchant;
		memset(&enchant,0,sizeof(enchant));
		enchant.skill        = skillid;
		enchant.skill_level  = mts->level;
		enchant.force_attack = mts->force;
		enchant.attack_range = mts->range > 0 ? mts->range : _cur_prop.attack_range;
		memcpy(enchant.skill_var, mts->skill_var, sizeof(enchant.skill_var));
		_skill.SetSkillTalent(skillid, enchant.skill_modify, object_interface(this));

		FillEnchantMsg(target,enchant);
		TranslateEnchant(target, enchant);
		SendEnchantMsg(GM_MSG_ENCHANT, target, enchant);
	}
}

bool gplayer_imp::IsXuanYuan()
{
	short cls = GetParent()->GetClass();
	return cls >= 56 && cls <= 60;
}

void gplayer_imp::SetDarkLightForm(char s)
{
	_dark_light_form = s;
}

void gplayer_imp::ClearDarkLightForm()
{
	_dark_light_form = DLFORM_NORMAL;
}

bool gplayer_imp::IsDarkLightForm(char s)
{
	if(_dark_light_form == DLFORM_NORMAL && s == _dark_light_form)
	{
		return true;
	}
	return _dark_light_form == s;
}

char gplayer_imp::GetDarkLightForm()
{
	return _dark_light_form;
}

bool gplayer_imp::AddSpiritSession(act_session* ses)
{
	if(!ses) return false;
	ses->_session_id = GetNextSpiritSessionID();
	SpiritSessionMap::iterator it = _spirit_session_list.find(ses->_session_id);
	if(it != _spirit_session_list.end())
	{
		delete ses;
		ses = 0;
		return false;
	}
	_spirit_session_list[ses->_session_id] = ses;
	bool rst = false;
	if(!(rst = ses->StartSession(NULL)))
	{
		EndSpiritSession(ses->_session_id);
	}
	else
	{
		ses->_start_tick = g_timer.get_tick();
	}
	return rst;
}

void gplayer_imp::EndSpiritSession(int session_id)
{
	SpiritSessionMap::iterator it = _spirit_session_list.find(session_id);
	if(it != _spirit_session_list.end())
	{
		act_session* ses = it->second;
		if(ses) 
		{
			ses->EndSession();
			delete ses;
			ses = 0;
			_spirit_session_list.erase(session_id);
		}
	}
}

void gplayer_imp::TerminateSpiritSession(int session_id)
{
	SpiritSessionMap::iterator it = _spirit_session_list.find(session_id);
	if(it != _spirit_session_list.end())
	{
		act_session* ses = it->second;
		if(ses && (ses->TerminateSession(false) || ses->TerminateSession(true)))
		{
			delete ses;
			ses = 0;
		}
	}
}

void gplayer_imp::ClearSpiritSession()
{
	for(size_t i = 0; i < _spirit_session_list.size(); ++ i)
	{
		act_session* ses = _spirit_session_list[i];
		if(ses)
		{
			ses->TerminateSession();
			delete ses;
			ses = NULL;
		}
	}
	_spirit_session_list.clear();
}

bool gplayer_imp::UseBattleFlag(int flag_item_id, int flag_item_index)
{
	DATA_TYPE dt;
	const FLAG_BUFF_ITEM_ESSENCE & ess = *(FLAG_BUFF_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(flag_item_id, ID_SPACE_ESSENCE,dt);
	if(dt != DT_FLAG_BUFF_ITEM_ESSENCE || &ess == NULL)
	{
		return false;
	}	
	object_interface obj(this);
	//obj.SummonMonster(ess.npc_id, 1, ess.exit_time, 2, 1);

	object_interface::minor_param prop;
	/*prop.exp_factor = 1.f;
	prop.drop_rate = 1.f;
	prop.money_scale = 1.f;
	prop.parent_is_leader = false;
	prop.use_parent_faction = true;
	prop.die_with_leader = false;*/
	prop.mob_id = ess.npc_id;
	prop.remain_time = ess.exit_time;
	//obj.CreateMinors(prop);
	obj.CreateNPC(prop);
	GMSV::SendBattleFlagStart( GetParent()->ID.id, flag_item_id, ess.exit_time, gmatrix::GetServerIndex(), GetWorldManager()->GetWorldTag(), GetParent()->id_mafia); 
	return true;
}

bool gplayer_imp::AddBattleFlag(int flag_item_id, int remain_time)
{
	DATA_TYPE dt;
	const FLAG_BUFF_ITEM_ESSENCE & ess = *(FLAG_BUFF_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(flag_item_id, ID_SPACE_ESSENCE,dt);
	if(dt != DT_FLAG_BUFF_ITEM_ESSENCE || &ess == NULL)
	{
		return false;
	}	
	battleflag_filter* filter = new battleflag_filter(this, remain_time);
	filter->hp = ess.hp;
	filter->mp = ess.mp;
	filter->attack = ess.dmg;
	filter->defence = ess.defence;
	filter->dec_damage = ess.extra_defence;
	filter->crit = ess.crit_rate;
	filter->crit_damage = ess.crit_damage;
	filter->resistance[0] = ess.anti_stunt;
	filter->resistance[1] = ess.anti_weak;
	filter->resistance[2] = ess.anti_twist;
	filter->resistance[3] = ess.anti_silence;
	filter->resistance[4] = ess.anti_sleep;
	filter->resistance[5] = ess.anti_slow;
	memcpy(filter->cult_defense, ess.cult_defense, sizeof(filter->cult_defense));
	memcpy(filter->cult_attack, ess.cult_attack, sizeof(filter->cult_attack));
	_filters.AddFilter(filter);
	return true;
}

bool gplayer_imp::EquipmentSlot(size_t item_index, int item_type, size_t stone_index, int stone_id)
{
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size || stone_index >= inv_size) return false;
	
	//检查物品是否存在
	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) return false;
	
	//检查道具是否存在
	item & it2 = _inventory[stone_index];
	if(it2.type ==-1 || stone_id != it2.type || stone_id <= 0)  return false;
	
	bool rst = ((equip_item*)it.body)->OpenSlot(&it, this, item_index, stone_index, stone_id);
	return rst;
}

bool gplayer_imp::EquipmentInstallAstrology(size_t item_index, int item_id, size_t stone_index, int stone_id, size_t slot_index)
{
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size || stone_index >= inv_size) return false;
	
	//检查装备是否存在
	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_id != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) return false;
	
	//检查星座是否存在
	item & it2 = _inventory[stone_index];
	if(it2.type ==-1 || it2.body == NULL || stone_id != it2.type )  return false;
	if(it2.GetItemType() != item_body::ITEM_TYPE_ASTROLOGY) return false;
	
	size_t need_money = g_config.fee_astrology_add; 	
	if(GetMoney() < need_money) 
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	bool rst = ((equip_item*)it.body)->InstallAstrology(this, &it, &it2, item_index, item_id, stone_index, stone_id, slot_index);
	return rst;
}



bool gplayer_imp::EquipmentUninstallAstrology(size_t item_index, int item_id, size_t slot_index)
{
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size) return false;

	if(_inventory.GetEmptySlotCount() <=0) return false;
	
	//检查装备是否存在
	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_id != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) return false;
	if(it.IsLocked()) return false;
	
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}
	
	size_t need_money = g_config.fee_astrology_remove; 
	if(GetMoney() < need_money) 
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
	
	bool rst = ((equip_item*)it.body)->UninstallAstrology(this, &it, item_index, item_id, slot_index);
	return rst;
}

void
gplayer_imp::PlayerIdentifyAstrology(size_t item_index, int item_id)
{
	class op : public session_general_operation::operation
	{
		size_t _item_index;
		int _item_id;
		public:
		op(size_t item_index, int item_id) : _item_index(item_index), _item_id(item_id)
		{}

		virtual int GetID()
		{
			return S2C::GOP_ASTROLOGY_IDENTIFY;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->IdentifyAstrology(_item_index, _item_id);
			if(!bRst) 
			{
			}
		}
	};

	session_general_operation * pSession = new session_general_operation(this,  new op(item_index, item_id));
	AddStartSession(pSession);
}


bool gplayer_imp::IdentifyAstrology(size_t item_index, int item_id)
{
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size) return false;

	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_id != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_ASTROLOGY) return false;
	
	size_t need_money = g_config.fee_astrology_identify; 
	if(GetMoney() < need_money) 
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	bool rst = ((item_astrology*)it.body)->AstrologyIdentify(this, &it, item_index, item_id);
	return rst;
}

void
gplayer_imp::PlayerUpgradeAstrology(size_t item_index, int item_id, size_t stone_index, int stone_id)
{
	class op : public session_general_operation::operation
	{
		size_t _item_index;
		int _item_id;
		size_t _stone_index;
		int _stone_id;

		public:
		op(size_t item_index, int item_id, size_t stone_index, int stone_id) : 
			_item_index(item_index), _item_id(item_id), _stone_index(stone_index), _stone_id(stone_id)
		{}

		virtual int GetID()
		{
			return S2C::GOP_ASTROLOGY_UPGRADE;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->UpgradeAstrology(_item_index, _item_id, _stone_index, _stone_id);
			if(!bRst) 
			{
			}
		}
	};

	session_general_operation * pSession = new session_general_operation(this,  new op(item_index, item_id, stone_index, stone_id));
	AddStartSession(pSession);
}


bool gplayer_imp::UpgradeAstrology(size_t item_index, int item_id, size_t stone_index, int stone_id)
{
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size || stone_index >= inv_size) return false;

	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_id != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_ASTROLOGY) return false;


	item & it2 = _inventory[stone_index];
	if(it.type == -1 || stone_id != it2.type || stone_id <= 0 )  return false;
	bool rst = ((item_astrology*)it.body)->AstrologyUpgrade(this, &it, item_index, item_id, stone_index, stone_id);
	return rst;
}

void
gplayer_imp::PlayerDestroyAstrology(size_t item_index, int item_id)
{
	class op : public session_general_operation::operation
	{
		size_t _item_index;
		int _item_id;
		public:
		op(size_t item_index, int item_id) : _item_index(item_index), _item_id(item_id)
		{}

		virtual int GetID()
		{
			return S2C::GOP_ASTROLOGY_DESTROY;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			bool bRst = pImp->DestroyAstrology(_item_index, _item_id);
			if(!bRst) 
			{
			}
		}
	};

	session_general_operation * pSession = new session_general_operation(this,  new op(item_index, item_id));
	AddStartSession(pSession);
}


bool gplayer_imp::DestroyAstrology(size_t item_index, int item_id)
{
	size_t inv_size = _inventory.Size();
	if(item_index >= inv_size) return false;

	item & it = _inventory[item_index];
	if(it.type ==-1 || it.body == NULL || item_id != it.type )  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_ASTROLOGY) return false;
	if(it.IsLocked()) return false;
	
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}

	bool rst = ((item_astrology*)it.body)->AstrologyDestroy(this, &it, item_index, item_id);
	return rst;
}


bool gplayer_imp::PurchaseRegionReputationItem(const reputation_item_info & info)
{
	if(!_pstate.CanShopping()) return false;

	//检查声望
	for(int i = 0; i < 3; ++i)
	{
		int repu_id_req = info.repu_required_pre[i].repu_id_req;
		//0表示不需要该项的声望
		if(0 == repu_id_req) 
		{
			continue;
		}

		//累计的声望必须达到要求
		int repu_req_value = (int)info.repu_required_pre[i].repu_req_value;
		if(repu_req_value > GetRegionReputation(repu_id_req))
		{
			_runner->error_message(S2C::ERR_LACK_OF_REPUTATION);
			return false;
		}

		//消耗值必须小于现在的可用值
		int consume_value = info.repu_required_pre[i].repu_consume_value * info.count; 
		if(consume_value > GetRegionReputation(info.repu_required_pre[i].repu_id_consume))
		{
			_runner->error_message(S2C::ERR_LACK_OF_REPUTATION);
			return false;
		}
	}

	if(_inventory.IsFull())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//生成物品
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(info.item_type);
	if(pItem == NULL || pItem->pile_limit <= 0 || info.count > (int)pItem->pile_limit || info.count <= 0)
	{
		_runner->error_message(S2C::ERR_INVALID_ITEM);
		return false;
	}

	//进包裹
	bool bind = (pItem->proc_type & item::ITEM_PROC_TYPE_BIND);
	DeliverItem(info.item_type, info.count, bind, pItem->expire_date, ITEM_INIT_TYPE_TASK);
	GLog::log(GLOG_INFO,"玩家%d兑换%d个声望物品%d", GetParent()->ID.id, info.item_type, info.count);

	//扣声望
	for(int i = 0; i < 3; ++i)
	{
		int repu_id_consume = info.repu_required_pre[i].repu_id_consume;
		//0表示不需要该项的声望
		if(0 == repu_id_consume)
		{
			continue;
		}
		int consume_value = -(info.repu_required_pre[i].repu_consume_value * info.count);
		ModifyRegionReputation(repu_id_consume, consume_value);	
		GLog::log(GLOG_INFO,"玩家%d兑换%d个声望物品%d,消耗声望id:%d,value:%d,剩余的声望值consume_value:%d", GetParent()->ID.id, info.item_type, info.count, repu_id_consume, consume_value, GetRegionReputation(repu_id_consume));
	}

	return true;
}

bool gplayer_imp::PlayerChangeWingColor(unsigned char wing_color)
{
	int id = _equipment[item::EQUIP_INDEX_WING].type;
	if(id <= 0) return false;

	DATA_TYPE dt;
	const TALISMAN_MAINPART_ESSENCE &ess = *(const TALISMAN_MAINPART_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(id, ID_SPACE_ESSENCE,dt); 
	if(dt != DT_TALISMAN_MAINPART_ESSENCE || &ess == NULL)
	{
		return false;
	}

	if(!ess.is_aircraft || !ess.color_change) return false;

	gplayer* pPlayer = GetParent();

	//设置好相对位置,方向和状态
	pPlayer->wing_color = wing_color;
	pPlayer->SetExtraState(gplayer::STATE_WING_COLOR);

	_runner->player_wing_change_color(_parent->ID.id, wing_color);
	return true;
}

void gplayer_imp::PlayerClearWingColor()
{
	gplayer* pPlayer = GetParent();
	pPlayer->wing_color = 0;
	pPlayer->ClrExtraState(gplayer::STATE_WING_COLOR);
}

void gplayer_imp::GainAstrologyEnergy(int value)
{
	_astrology_info.cur_energy += value;
	if(_astrology_info.cur_energy > MAX_ASTROLOGY_VALUE)
	{
		_astrology_info.cur_energy = MAX_ASTROLOGY_VALUE;
	}
	
	if(!IsAstrologyActive())
	{
		if(_astrology_info.cur_energy > 0) 
		{
			ActiveAstrology();
		}	
	}	
	_runner->notify_astrology_energy();
}

void gplayer_imp::ConsumeAstrologyEnergy(int value)
{
	if((!IsAstrologyActive())) return;

	if(_astrology_info.cur_energy >= value)
	{
		_astrology_info.cur_energy -= value;
	}
	else
	{
		_astrology_info.cur_energy = 0;
		DeactiveAstrology();
	}
	_runner->notify_astrology_energy();
}

void gplayer_imp::ActiveAstrology()
{
	if(IsAstrologyActive()) return;

	_astrology_info.is_active = true;
	size_t count = _equipment.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _equipment[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) continue;

	        ((equip_item*)it.body)->ActiveAstrologyProp(&it, this);	
	}
	//重新计算player 所有数据
	property_policy::UpdatePlayer(GetPlayerClass(),this);
}

void gplayer_imp::DeactiveAstrology()
{
	if(!IsAstrologyActive()) return;
	
	_astrology_info.is_active = false;
	size_t count = _equipment.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _equipment[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) continue;

	        ((equip_item*)it.body)->DeactiveAstrologyProp(&it, this);	
	}
	//重新计算player 所有数据
	property_policy::UpdatePlayer(GetPlayerClass(),this);
}

void gplayer_imp::CalcAstrologyConsume()
{
	int consume = 0;
	size_t count = _equipment.Size();
	for(size_t i = 0; i < count ; i++)
	{
		item & it = _equipment[i];
		if(it.type == -1 || it.body == NULL) continue;
		if(it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT) continue;

		consume += ((equip_item*)it.body)->GetAstrologyEnergyConsume(&it);
	}
	_astrology_info.consume_speed = consume;
}

void gplayer_imp::UpdateAstrologyInfo()
{
	if(IsAstrologyActive())
	{
		ConsumeAstrologyEnergy(_astrology_info.consume_speed);

	}
	else if(!IsAstrologyActive())
	{
		if(_astrology_info.cur_energy > 0)
		{
			ActiveAstrology();
		}
	}
}


void gplayer_imp::SaveAstrologyData(archive & ar)
{
	ar << _astrology_info.client_active <<  _astrology_info.cur_energy << _astrology_info.reservered[0] << _astrology_info.reservered[1];
}

void gplayer_imp::LoadAstrologyData(archive & ar)
{
	if(ar.size() == 0) return;
	
	ar >> _astrology_info.client_active >>  _astrology_info.cur_energy >> _astrology_info.reservered[0] >> _astrology_info.reservered[1];
}


void gplayer_imp::LoadCollisionData(archive & ar)
{
	if(ar.size() == 0) return;
//	if(ar.size() != sizeof(_collision_info)) return;

	ar >> _collision_info.collision_win >> _collision_info.collision_lost >> _collision_info.collision_draw 
	   >> _collision_info.collision_score_a >> _collision_info.collision_score_b >> _collision_info.collision_timestamp >> _collision_info.daily_award_timestamp 
	   >> _collision_info.day_win >> _collision_info.day_score_a >> _collision_info.day_score_b >> _collision_info.collision_score >> _collision_info.cs_personal_award_timestamp
	   >> _collision_info.cs_team_season_award_timestamp >> _collision_info.cs_weekly_exchange_award_timestamp >> _collision_info.cs_exchange_item_timestamp; 

	for(size_t i = 0; i < 3; ++i)
	{
		ar >> _collision_info.reservered[i];
	}

}

void gplayer_imp::SaveCollisionData(archive & ar)
{
	if(_collision_info.collision_timestamp ==  0 && _collision_info.cs_personal_award_timestamp == 0 &&
	   _collision_info.cs_team_season_award_timestamp == 0 && _collision_info.cs_weekly_exchange_award_timestamp == 0 && _collision_info.cs_exchange_item_timestamp == 0) return;

	ar << _collision_info.collision_win << _collision_info.collision_lost << _collision_info.collision_draw 
	   << _collision_info.collision_score_a << _collision_info.collision_score_b << _collision_info.collision_timestamp << _collision_info.daily_award_timestamp 
	   << _collision_info.day_win << _collision_info.day_score_a << _collision_info.day_score_b << _collision_info.collision_score << _collision_info.cs_personal_award_timestamp
	   << _collision_info.cs_team_season_award_timestamp << _collision_info.cs_weekly_exchange_award_timestamp << _collision_info.cs_exchange_item_timestamp; 

	for(size_t i = 0; i < 3; ++i)
	{
		ar << _collision_info.reservered[i];
	}
}

void gplayer_imp::LoadExchange6V6AwardInfo(archive & ar)
{
	if(ar.size() == 0) return;
	ASSERT((ar.size() % sizeof(int)) == 0);

	int count;
	int item_id;
	int exchange_count;
	ar >> count;
	for(int i = 0; i < count; ++i)
	{
		ar >> item_id;
		ar >> exchange_count;
		_exchange_cs6v6_award_info[item_id] = exchange_count;
	}

	//判断是否要刷新兑换奖励次数的限制
	if(_collision_info.cs_exchange_item_timestamp == 0)
	{
		_exchange_cs6v6_award_info.clear();
	}
	else
	{
		int cur_time = g_timer.get_systime();

		struct tm tm1, tm2;
		localtime_r((time_t*)&cur_time, &tm1);
		localtime_r((time_t*)&_collision_info.cs_exchange_item_timestamp, &tm2);

		//不是一年
		if(tm1.tm_year != tm2.tm_year)
		{
			_exchange_cs6v6_award_info.clear();
		}
		//不是一个赛季
		else if((tm1.tm_mon / 3) != (tm2.tm_mon /3))
		{
			_exchange_cs6v6_award_info.clear();
		}
	}
}

void gplayer_imp::SaveExchange6V6AwardInfo(archive & ar)
{
	if(_exchange_cs6v6_award_info.size() == 0) return;

	ar << _exchange_cs6v6_award_info.size();
	abase::hash_map<int,int>::iterator iter = _exchange_cs6v6_award_info.begin();
	for(; iter != _exchange_cs6v6_award_info.end(); ++iter)
	{
		ar << iter->first;
		ar << iter->second;
	}
}

void gplayer_imp::ResetExchange6V6AwardInfo()
{
	if(_collision_info.cs_exchange_item_timestamp == 0) return;

	int cur_time = g_timer.get_systime();

	struct tm tm1, tm2;
	localtime_r((time_t*)&cur_time, &tm1);
	localtime_r((time_t*)&_collision_info.cs_exchange_item_timestamp, &tm2);

	//不是一年
	if(tm1.tm_year != tm2.tm_year)
	{
		_exchange_cs6v6_award_info.clear();
	}
	//不是一个赛季
	else if((tm1.tm_mon / 3) != (tm2.tm_mon /3))
	{
		_exchange_cs6v6_award_info.clear();
	}
	SendCollisionRaidInfo();
}

void gplayer_imp::SaveExchange6V6AwardInfoForClient(archive & ar)
{
	ar << _exchange_cs6v6_award_info.size();
	abase::hash_map<int,int>::iterator iter = _exchange_cs6v6_award_info.begin();
	for(; iter != _exchange_cs6v6_award_info.end(); ++iter)
	{
		ar << iter->first;
		ar << iter->second;
	}
}

void gplayer_imp::ClearCollisionInfo()
{
	memset(&_collision_info, 0, sizeof(_collision_info));
	SendCollisionRaidInfo();
}

bool gplayer_imp::PlayerJoinRaidRoom(int raid_id, int raid_template_id, int room_id, int raid_faction)
{
	//现在只支持帮战副本可以直接通过界面发协议加入副本，不需要通过NPC报名
	if(MAFIA_BATTLE_RAID_ID != raid_template_id) 
	{
		return false;
	}

	return JoinRaidRoom(raid_id, raid_template_id, room_id, raid_faction);
}

void gplayer_imp::TalismanRefineSkillResult(int item_index, int item_id, char result)
{
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	bool bRst = TalismanSkillRefineResult(item_index, item_id, result);
	if( !bRst ) 
	{
		_runner->error_message(S2C::ERR_TALISMAN_SKILLREFINERESULT);
	}

}

bool
gplayer_imp::PlayerGetCollisionAward(bool daily_award, int award_index)
{
	if(_inventory.GetEmptySlotCount() <=0) return false;

	//领取每日大奖
	if(daily_award)
	{
        	int cur_time = g_timer.get_systime();
		//当天已经领取过了奖励
		if(IsSameDay(cur_time, _collision_info.daily_award_timestamp)) return false;
		//当天没有进行过比赛
		if(!IsSameDay(cur_time, _collision_info.collision_timestamp)) return false;
		//当天还没有胜利
		if(_collision_info.day_win <= 0) return false;

		unsigned int item_id = player_template::GetCollisionDailyAwardItem();
	       	if(item_id > 0)
		{	
			DeliverItem(item_id,1, false, 0, ITEM_INIT_TYPE_TASK);
			_collision_info.daily_award_timestamp = cur_time; 
		}

		SendCollisionRaidInfo();
		GLog::log(GLOG_INFO,"玩家%d领取了碰撞战场每日奖励", GetParent()->ID.id); 
	}
	//积分兑换奖励
	else
	{
		if(award_index < 0) return false;

		unsigned int item_id = 0;
		unsigned int item_count = 0;
		unsigned int need_score_a = 0;
		unsigned int need_score_b = 0;
		unsigned int win_num = 0;

		bool rst = player_template::GetCollisionAwardItem(award_index, item_id, item_count, need_score_a, need_score_b, win_num);
		if(!rst) return false;

		if(_collision_info.collision_score_a < (int)need_score_a || _collision_info.collision_score_b < (int)need_score_b) return false;
		if(item_id <= 0 || item_count <= 0) return false;
		if(GetCollisionWin() < (int)win_num) return false;

		_collision_info.collision_score_a -= need_score_a;
		_collision_info.collision_score_b -= need_score_b;
		_collision_info.collision_win -= win_num;
		
		DeliverItem(item_id,item_count, false, 0, ITEM_INIT_TYPE_TASK);
		SendCollisionRaidInfo();
		GLog::log(GLOG_INFO,"玩家%d兑换了碰撞战场奖励, index=%d, item_id=%d, item_count=%d, score_a=%d, score_b=%d, win_num=%d", 
				GetParent()->ID.id, award_index, item_id, item_count, need_score_a, need_score_b, win_num); 

	}

	return true;
}

bool
gplayer_imp::IsSameDay(int timestamp1, int timestamp2)
{
        int gmtoff = gmatrix::Instance()->GetServerGMTOff();
        int diff_days = (timestamp1 + gmtoff)/86400 - (timestamp2 + gmtoff)/86400;
	return diff_days == 0;
}


/*
bool
gplayer_imp::IsSameWeek(int timestamp1, int timestamp2)
{
        int gmtoff = gmatrix::Instance()->GetServerGMTOff();
	int day1 = (timestamp1 + gmtoff)/86400;
	int day2 = (timestamp2 + gmtoff)/86400;

	struct tm tm1, tm2;
	localtime_r((time_t*)&day1, &tm1);
	localtime_r((time_t*)&day2, &tm2);

	day1 -= tm1.tm_wday * 86400;
	day2 -= tm2.tm_wday * 86400;
	return day1 == day2;
			
}
*/

bool 
gplayer_imp::IsSameWeek(int timestamp1, int timestamp2)
{
        int gmtoff = gmatrix::Instance()->GetServerGMTOff();
	return ((timestamp1 + gmtoff- 86400 * 4)/86400 * 7) == ((timestamp2 + gmtoff- 86400 * 4)/86400 * 7);
}

bool 
gplayer_imp::IsSameSeason(int timestamp1, int timestamp2)
{
	struct tm tm1, tm2;
	localtime_r((time_t*)&timestamp1, &tm1);
	localtime_r((time_t*)&timestamp2, &tm2);

	return tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon;
}


void gplayer_imp::SendCollisionRaidInfo()
{
	_runner->collision_raid_info(_collision_info.collision_score_a, _collision_info.collision_score_b, _collision_info.collision_win,
	       	_collision_info.collision_lost, _collision_info.collision_draw, _collision_info.daily_award_timestamp, _collision_info.collision_timestamp, _collision_info.day_win,
		_collision_info.cs_personal_award_timestamp, _collision_info.cs_weekly_exchange_award_timestamp, _collision_info.cs_team_season_award_timestamp);
}

int gplayer_imp::GetTaskFinishCount(unsigned long task_id)
{
	if(task_id <= 0) return 0;
	PlayerTaskInterface  task_if(this);

	return task_if.GetTaskFinishCount(task_id);
}


//获取玩家的任务完成次数，给运营网站使用
void gplayer_imp::PlayerGetCouponsInfo(int userid, int toaid, int tozoneid, int roleid, std::vector<int> & task_ids)
{	
	//key->task_id value->finish count
	std::map<int, int> task_finish_count;
	for(size_t i = 0; i < task_ids.size(); ++i)
	{
		int task_id = task_ids[i];
		int finish_count = 0;

		finish_count = GetTaskFinishCount(task_id);
		task_finish_count[task_id] = finish_count;
	}

	GMSV::SendCouponsInfo(userid, toaid, tozoneid, roleid, task_finish_count);
}

void gplayer_imp::PlayerFirstExitReason(int reason)
{
	//冷却60秒
	if(!CheckCoolDown(COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON))
	{
		return;
	}
	SetCoolDown(COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON, 60 * 1000);

	GLog::log(GLOG_INFO, "新手玩家退出原因type:%d,roleid=%d", reason, GetParent()->ID.id);
	GLog::formatlog("formatlog:newbie_exit_game_reason:roleid=%d:type=%d",_parent->ID.id, reason);
}

//修正任务数据
//由于库任务增加了第9号库，导致新老数据的存盘格式发生变化
//需要在这里判断如果是老数据的话将数据修正为新数据
void gplayer_imp::VerifyActiveTaskData(size_t data_size)
{
	int task_count = *((char*)_active_task_list.begin());
	
	//如果是老数据(只有八个库任务的数据)需要将数据修复为9个库的数据
	if(data_size == (TASK_ACTIVE_LIST_HEADER_LEN + 8 * TASK_STORAGE_LEN * sizeof(short) + task_count * TASK_DATA_BUF_MAX_LEN))
	{
		_active_task_list.insert(_active_task_list.begin() + TASK_ACTIVE_LIST_HEADER_LEN + 8 * TASK_STORAGE_LEN * sizeof(short), TASK_STORAGE_LEN * sizeof(short), 0);
	}
}


//弥补未满级飞升玩家的属性差距
void gplayer_imp::PlayerRemedyMetempsychosisLevel(int item_index)
{
	if(item_index < 0 || (size_t)item_index > _inventory.Size()) return;
	if( !_pstate.IsNormalState() && !_pstate.IsBindState() ) return;
	if(GetRebornCount() == 0) return;

	int pre_level = GetRebornLvl(0);
	if(pre_level >= 150 || pre_level < 135) return;

	//从149到135级别对应需要的物品个数
	int item_req[] = {3, 6, 8, 10, 12, 14, 16, 18, 20, 22, 23, 24, 25, 26, 27}; 
	int item_id = g_config.item_fix_prop_lose;

	int diff_level = 150 - pre_level; 
	size_t req_count = item_req[diff_level-1];

	item & it = _inventory[item_index];
	if(it.type != item_id) return;	
	if(it.count < req_count) return;

	//修改飞升数据
	_reborn_list[0] &= 0x0000FFFF;
	_reborn_list[0] |= (150 << 16);

	player_template::InitPlayerData(GetParent()->GetClass(),this);
	property_policy::UpdatePlayer(GetParent()->GetClass(),this);

	if(!((gplayer*)_parent)->IsZombie())
	{
		//让玩家血和魔回满
		FullHPAndMP();
	}

	TakeOutItem(item_index, item_id, req_count); 
	_runner->player_remedy_metempsychosis_level();

	GLog::log(GLOG_INFO, "玩家执行了弥补未满级飞升属性差距操作, roleid=%d, pre_level=%d, item_index=%d, item_count=%d", GetParent()->ID.id, pre_level, item_index, req_count);
}

bool gplayer_imp::FillPotionBottle(int bottle_id, int bottle_index, int potion_id, int potion_index)
{
	if(bottle_index == potion_index)
	{
		_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
		return false;
	}
	if(bottle_index < 0 || (size_t)bottle_index >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
		return false;
	}
	if(potion_index < 0 || (size_t)potion_index >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
		return false;
	}

	item& bottle = _inventory[bottle_index];
	if(bottle.type == -1 || bottle.body == NULL || bottle_id != bottle.type)
	{
		_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
		return false;
	}

	item& potion = _inventory[potion_index];
	if(potion.type == -1 || potion.body == NULL || potion_id != potion.type || !potion.body->CanFillToBottle())
	{
		_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
		return false;
	}

        if(potion.expire_date > 0)
	{
		//特殊处理以下ID的限时物品，使其可以灌注到元力药容器60312里
		if(potion.type != 60306 && potion.type != 60307 && potion.type != 60308 && potion.type != 60309)
		{
			_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
			return false;
		}
	}

	int potion_left = 0;
	int capacity = potion.body->GetPotionCapacity(&potion);
	if(bottle.body->FillBottle(this, &bottle, bottle_index, potion.GetItemType(), potion_id, capacity, potion.proc_type & item::ITEM_PROC_TYPE_ZONEUSE, potion.proc_type & item::ITEM_PROC_TYPE_ZONENOUSE, potion.proc_type & item::ITEM_PROC_TYPE_ZONETRADE, potion_left) == -1)
	{
		_runner->error_message(S2C::ERR_FAILED_MERGE_POTION);
		return false;
	}
	if(potion_left <= 0)
	{
		UseItemLog(_inventory[potion_index],1);
		_inventory.DecAmount(potion_index, 1);
		_runner->player_drop_item(IL_INVENTORY,potion_index, potion_id, 1 ,S2C::DROP_TYPE_USE);
	}
	else
	{
		potion.body->SetPotionCapacity(&potion, potion_left);
		PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, potion_index);
	}
	GLog::log(GLOG_INFO, "玩家执行了药品合并roleid=%d, bottle_id=%d, bottle_index=%d, potion_id=%d, potion_index=%d, potion_value=%d, potion_left=%d", _parent->ID.id, bottle_id, bottle_index, potion_id, potion_index, capacity, potion_left);
	return true;
}

void gplayer_imp::CheckBuffArea()
{
	A3DVECTOR pos = _parent->pos;
	abase::vector<int> areaTids;
	buff_area* ba = GetWorldManager()->GetBuffArea();
	ba->GetBuffAreas(pos.x, pos.z, areaTids);
	for(size_t i = 0; i < areaTids.size(); i ++)
	{
		int npcId = -1;
		bool rst = GetWorldManager()->IsBuffAreaActive(areaTids[i], npcId);
		if(rst)
		{
			printf("buff area active npcId=%d, areaTids[i]=%d\n", npcId, areaTids[i]);
			MSG msg;
			BuildMessage(msg, GM_MSG_REQUEST_BUFFAREA_BUFF, XID(GM_TYPE_NPC, npcId), _parent->ID, pos, areaTids[i]);
			gmatrix::SendMessage(msg);
		}
	}
}


bool gplayer_imp::PlayerOpenTrashBox(unsigned int passwd_size, char passwd[])
{
	if(!IsKingdomPlayer()) return false;

	if(HasSession())
	{
		_runner->error_message(S2C::ERR_OTHER_SESSION_IN_EXECUTE);
		return false;
	}

	//检查是否已经打开
	if(IsTrashBoxOpen())
	{
		_runner->error_message(S2C::ERR_OTHER_SESSION_IN_EXECUTE);
		return false;
	}

	//检查密码
	if(!_trashbox.CheckPassword(passwd,passwd_size))
	{
		_runner->error_message(S2C::ERR_PASSWD_NOT_MATCH);
		return false;
	}

	session_use_trashbox *pSession = new session_use_trashbox(this, player_trashbox::TRASHBOX_OPEN);
	AddStartSession(pSession);
	return true;
}

void gplayer_imp::DeliveryGiftBox(size_t item_index, int item_id)
{
	if(item_index < 0 || (size_t)item_index >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FAILED_DELIVERYGIFTBOX);
		return;
	}

	item& gb = _inventory[item_index];
	if(gb.type == -1 || gb.body == NULL || item_id != gb.type)
	{
		_runner->error_message(S2C::ERR_FAILED_DELIVERYGIFTBOX);
		return;
	}
	if(gb.body->GetItemType() != item_body::ITEM_TYPE_GIFTBOX_DELIVERY)
	{
		_runner->error_message(S2C::ERR_FAILED_DELIVERYGIFTBOX);
		return;
	}

	if(!gb.body->DeliveryGiftBox(this, &gb))
	{
		_runner->error_message(S2C::ERR_FAILED_DELIVERYGIFTBOX);
		return;
	}
	
	if(gb.body->DeliveryGiftBox(this, &gb))
	{
		_runner->error_message(S2C::ERR_FAILED_DELIVERYGIFTBOX);
		return;
	}

	UseItemLog(_inventory[item_index],1);
	_inventory.DecAmount(item_index, 1);
	_runner->player_drop_item(IL_INVENTORY,item_index, item_id, 1 ,S2C::DROP_TYPE_USE);
	return;
}

void gplayer_imp::UseCashLotteryTY(int item_id, int item_cnt)
{
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
	if(pItem == NULL || pItem->pile_limit <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}

	int order_id = _mallinfo.GetOrderID(); 
	PlayerUseCash(item_id, item_cnt, 0, item_cnt*100, order_id);
	CheckSpecialConsumption(item_id, item_cnt);
}

void gplayer_imp::OnUseCash(int offset)
{
	if(offset <= 0) return;
	if(gmatrix::IsDisableCashGift()) return;	//海外版本关闭这个功能

	DeliverCashGiftPoint(offset);
}

void gplayer_imp::PlayerUseCash(int item_id, int item_count, int expire_date, int need_cash, int order_id)
{
	_mallinfo.IncOrderID(); 
	int total_cash = _mallinfo.GetCash2();

	float cash_need_log = need_cash;
	if(!gmatrix::IsDisableCashGift())
	{
		cash_need_log = need_cash * 0.77; 
	}
	float  cash_left_log = total_cash - cash_need_log;	

	GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
			_parent->ID.id,  _db_magic_number, order_id, item_id, expire_date, item_count, (item_id == g_config.item_zone_cash ) ? 0 : cash_need_log,cash_left_log); 
	GLog::log(GLOG_INFO, "用户%d购买了%d个%d有效期%d花费了%.2f点交易序号%d", _parent->ID.id, item_count, item_id, expire_date, cash_need_log, order_id);
	GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, _parent->ID.id, cash_need_log,
			GetObjectLevel(), GetMoney(), cash_left_log, item_id, item_count);

	if(!gmatrix::IsDisableCashGift())
	{
		int cash_gift_num =  need_cash / 10;
		float  cash_gift_need_log = need_cash * 0.23; 
		cash_left_log -= cash_gift_need_log;	
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
				_parent->ID.id, _db_magic_number, 0, gmatrix::GetCurCashGiftID1(), 0, cash_gift_num, cash_gift_need_log,cash_left_log); 
		GLog::log(GLOG_INFO, "用户%d购买了%d个%d有效期%d花费了%.2f点交易序号%d", _parent->ID.id, cash_gift_num, gmatrix::GetCurCashGiftID1(), 0, cash_gift_need_log, 0);
		GLog::action("shoptrade,huid=%d:hrid=%d:moneytype=2:price=%.2f:lv=%d:h_gold=%d:h_cash=%.2f:itemid=%d,%d", _db_magic_number, _parent->ID.id, cash_gift_need_log,
				GetObjectLevel(), GetMoney(), cash_left_log,  gmatrix::GetCurCashGiftID1(), cash_gift_num); 
	}

	_mallinfo.UseCash(need_cash, this);
	_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	SendRefCashUsed(need_cash);
}



//发放商城买赠积分物品
//每在商城消耗花费一个元宝，发放一个积分奖励物品
//商城消耗包括彩票 台历 元宵购买等
//奖励物品分为两种, 1个积分的和10000积分的, 当1个积分的物品数量超过10000个时,自动转换成10000积分的物品
void gplayer_imp::DeliverCashGiftPoint(int offset)
{
	if(offset < 10) return;
	int gift_id1 = gmatrix::GetCurCashGiftID1(); //1个积分对应的物品id
	int gift_id2 = gmatrix::GetCurCashGiftID2(); //10000个积分对应的物品id
	if(gift_id1 <= 0 || gift_id2 <= 0) return;
	if(_gift_inventory.GetEmptySlotCount() == 0) return; 

	int award_num = offset / 10;

	int gift_id1_count = _gift_inventory.CountItemByID(gift_id1);

	int total_id1_count = award_num + gift_id1_count;
	int id1_count_change = total_id1_count % 10000 - gift_id1_count; 
	int id2_count_change = total_id1_count / 10000;
	
	size_t pile_limit1 = (size_t)gmatrix::GetDataMan().get_item_pile_limit(gift_id1);
	size_t pile_limit2 = (size_t)gmatrix::GetDataMan().get_item_pile_limit(gift_id2);
	size_t need_slot = 0;
	if(id1_count_change > 0) need_slot = id1_count_change / pile_limit1 + ((id1_count_change % pile_limit1 == 0) ? 0 : 1);
	if(id2_count_change > 0) need_slot = need_slot + id2_count_change / pile_limit2 + ((id2_count_change % pile_limit2 == 0) ? 0 : 1);

	if(need_slot > _gift_inventory.Size())
	{
		GLog::log(GLOG_ERR, "用户%d在获得买赠积分物品的时候包裹空间不够, need_slot=%d, empty_slot=%d", _parent->ID.id, need_slot, _gift_inventory.Size());
	       	return;
	}

	//高积分物品id只会增加
	ASSERT(id2_count_change >= 0);
	if(id2_count_change > 0)
	{
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * data = gmatrix::GetDataMan().generate_item(gift_id2,&tag,sizeof(tag));
		ASSERT(data);
		ASSERT((size_t)id2_count_change < data->pile_limit);
		data->count = id2_count_change;
		_gift_inventory.Push(*data);
		ASSERT(data->count == 0);

		float log_cash = _mallinfo.GetCash2()-id2_count_change*2.3*10000;
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
				_parent->ID.id, _db_magic_number,0, gift_id2, 0, id2_count_change, id2_count_change*2.3*10000, log_cash); 

		log_cash = _mallinfo.GetCash2();
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%.2f:cash_left=%.2f:guid=0,0",
				_parent->ID.id, _db_magic_number,0, gift_id1, 0, -id2_count_change*10000, -id2_count_change*2.3*10000, log_cash); 
	}	

	//低积分物品id有可能减少
	if(id1_count_change < 0)
	{
		TakeOutGiftItem(gift_id1, -id1_count_change);
	}
	//低积分物品id也可能变多
	else if(id1_count_change > 0)
	{
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * data = gmatrix::GetDataMan().generate_item(gift_id1,&tag,sizeof(tag));
		ASSERT(data);
		ASSERT((size_t)id1_count_change <= data->pile_limit);
		data->count = id1_count_change;
		_gift_inventory.Push(*data);
		ASSERT(data->count == 0);
	}
	PlayerGetInventoryDetail(gplayer_imp::IL_GIFT);

	GLog::log(GLOG_INFO,"用户%d消费商城元宝获得买赠积分%d, 积分id1变化为%d, 积分id2变化为%d",_parent->ID.id, award_num, id1_count_change, id2_count_change);
	GLog::formatlog("formatlog:deliver_cash_gift_point:userid=%d:num=%d:id1_change=%d:id2_change=%d",_parent->ID.id, award_num, id1_count_change, id2_count_change);
}


//登录的时候自动检查是否可以兑换商城买赠礼包
//如果可以兑换，则自动兑换
void gplayer_imp::TryObtainCashGift()
{
	if(_gift_inventory.GetEmptySlotCount() == GIFT_INVENTORY_SIZE) return; 
	std::vector<gmatrix::cash_gift_award> & award_list = gmatrix::GetCashGiftAwardList();
	if(award_list.empty()) return;

	bool notify = false;
	int cur_time = g_timer.get_systime();
	for(size_t i = 0; i < award_list.size(); ++i)
	{
		int award_timestamp = award_list[i].award_timestamp;
		int one_year = 365*24*3600;
		//还没有到领取时间
		if(cur_time < award_timestamp) continue;

		int gift_award_id = award_list[i].award_id;

		int gift_id1 = award_list[i].gift_id1;
		int gift_id2 = award_list[i].gift_id2;
		int gift_id1_count = _gift_inventory.CountItemByID(gift_id1);
		int gift_id2_count = _gift_inventory.CountItemByID(gift_id2);
		if(gift_id1_count <= 0 && gift_id2_count <= 0) continue;
		
		int total_point = gift_id1_count + 10000*gift_id2_count;

		GLog::formatlog("formatlog:cash_gift_disappear:userid=%d:total_num=%d:gift_id=%d",_parent->ID.id, total_point, gift_id1);
		//如果总积分低于100, 直接去掉
		if(total_point < 100)
		{
			TakeOutGiftItem(gift_id1, gift_id1_count);
			continue;
		}

		if(gift_id1_count > 0) TakeOutGiftItem(gift_id1, gift_id1_count);
		if(gift_id2_count > 0) TakeOutGiftItem(gift_id2, gift_id2_count);

		//1年后不能领取
		if(cur_time > award_timestamp + one_year) continue; 

		//发放礼包物品
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * data = gmatrix::GetDataMan().generate_item(gift_award_id,&tag,sizeof(tag));
		ASSERT(data);
		data->count = 1;
		data->expire_date = award_timestamp + one_year; 
		int rst = _gift_inventory.Push(*data);
		ASSERT(data->count == 0 && rst >= 0);

		item & it = _gift_inventory[rst];
		if(it.body) it.body->SetGiftBoxAccPoint(total_point, &it);

		//只通知最近一期的积分
		if(!notify)
		{
			_runner->notify_cash_gift(gift_award_id, rst, total_point);
			notify = true;
		}

		GLog::log(GLOG_INFO,"用户%d登录时系统自动兑换了一个商城买赠礼包:gift_award_id=%d, total_point=%d, gift_id1=%d, gift_id2=%d, cur_timestmap=%d, award_timestamp=%d"
			       	,_parent->ID.id, gift_award_id, total_point, gift_id1, gift_id2, cur_time, award_timestamp);
		GLog::formatlog("formatlog:obtain_cash_gift:userid=%d:gift_award_id=%d:total_point=%d:gift_id1=%d:gift_id2=%d:cur_timestmap=%d:award_timestamp=%d"
				,_parent->ID.id, gift_award_id, total_point, gift_id1, gift_id2, cur_time, award_timestamp); 
	}
	PlayerGetInventoryDetail(gplayer_imp::IL_GIFT);
}

//玩家领取商城买赠礼包
bool gplayer_imp::GetCashGiftAward()
{
	if(_gift_inventory.CountItemByType(item_body::ITEM_TYPE_GIFTBOX_SET) > (int) _inventory.GetEmptySlotCount()) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	for(size_t i = 0; i < _gift_inventory.Size(); ++i)
	{
		item & it = _gift_inventory[i];
		if(it.type == -1) continue;
		if(it.GetItemType() != item_body::ITEM_TYPE_GIFTBOX_SET) continue;

		int point = it.body->GetGiftBoxAccPoint(&it);
		int gift_award_id= it.type;
		int expire_date = it.expire_date;
		TakeOutGiftItem(i, it.type, 1);

		//发放礼包物品
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * data = gmatrix::GetDataMan().generate_item(gift_award_id,&tag,sizeof(tag));
		ASSERT(data);
		data->count = 1;
		data->expire_date = expire_date;
		int rst = _inventory.Push(*data);
		ASSERT(data->count == 0 && rst >= 0);

		item & it2 = _inventory[rst];
		if(it2.body) it2.body->SetGiftBoxAccPoint(point, &it2);
		int state = item::Proctype2State(data->proc_type);
		_runner->obtain_item(data->type,data->expire_date, 1, _inventory[rst].count, IL_INVENTORY,rst,state);
		PlayerGetItemInfo(IL_INVENTORY, rst);
		PlayerGetInventoryDetail(gplayer_imp::IL_GIFT);

		GLog::log(GLOG_INFO,"用户%d领取了一个商城买赠礼包: gift_award_id=%d, point=%d, item_index=%d, item_index2=%d", _parent->ID.id, gift_award_id, point, i, rst); 
		GLog::formatlog("formatlog:get_cash_gift:userid=%d:gift_award_id=%d:point=%d,item_index=%d:item_index2=%d", _parent->ID.id, gift_award_id, point, i, rst); 
	}
	return true;
}


void gplayer_imp::IncPropAdd(int idx, int v)
{
	if(idx < 0 || idx >= PROP_ADD_NUM)
	{
		return;
	}
	_prop_add[idx] += v;
}

int gplayer_imp::GetPropAdd(int idx)
{
	return idx < 0 || idx >= PROP_ADD_NUM ? -1 : _prop_add[idx];
}

void gplayer_imp::GenPropAddItem(int materialId, short materialIdx, short materialCnt)
{
	if(materialId <= 0 || materialCnt <= 0)
	{
		_runner->error_message(S2C::ERR_FAILED_GEN_PROPADDITEM);
		return;
	}
	if(materialIdx < 0 || (size_t)materialIdx > GetInventory().Size())
	{
		_runner->error_message(S2C::ERR_FAILED_GEN_PROPADDITEM);
		return;
	}
	item& it = _inventory[materialIdx];
	if(it.type != materialId)
	{
		_runner->error_message(S2C::ERR_FAILED_GEN_PROPADDITEM);
		return;
	}
	if(!InventoryHasSlot(1))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	DATA_TYPE dt;
	const PROP_ADD_MATERIAL_ITEM_ESSENCE &ess= *(const PROP_ADD_MATERIAL_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(materialId, ID_SPACE_ESSENCE,dt); 
	if(dt != DT_PROP_ADD_MATERIAL_ITEM_ESSENCE || &ess == NULL || ess.require_num <= 0)
	{
		_runner->error_message(S2C::ERR_FAILED_GEN_PROPADDITEM);
		return;
	}
	if(it.count < (size_t)ess.require_num)
	{
		_runner->error_message(S2C::ERR_FAILED_GEN_PROPADDITEM_MATERIAL_NOT_ENOUGH);
		return;
	}
	int idx = RandSelect(&ess.forge_result[0].prop, sizeof(ess.forge_result[0]), sizeof(ess.forge_result) / sizeof(ess.forge_result[0]));
	int itemId = ess.forge_result[idx].id;
	int newItemInvIdx = -1;
	DeliverItem(itemId, 1, true, 0, ITEM_INIT_TYPE_TASK, newItemInvIdx);
	_runner->player_get_propadd_item(itemId, newItemInvIdx);
		
	UseItemLog(_inventory[materialIdx], ess.require_num);
	_inventory.DecAmount(materialIdx, ess.require_num);
	_runner->player_drop_item(IL_INVENTORY, materialIdx, materialId, ess.require_num ,S2C::DROP_TYPE_USE);
}

void gplayer_imp::RebuildPropAddItem(int itemId, short itemIdx, int forgeId, short forgeIdx)
{
	if(itemId <= 0 || itemIdx < 0 || (size_t)itemIdx > GetInventory().Size())
	{
		_runner->error_message(S2C::ERR_FAILED_REBUILD_PROPADDITEM);
		return;
	}

	if(forgeId <= 0 || forgeId < 0 || (size_t)forgeIdx > GetInventory().Size())
	{
		_runner->error_message(S2C::ERR_FAILED_REBUILD_PROPADDITEM);
		return;
	}
	
	item& it = _inventory[itemIdx];
	if(it.type != itemId || it.body->GetItemType() != item_body::ITEM_TYPE_PROPADD)
	{
		_runner->error_message(S2C::ERR_FAILED_REBUILD_PROPADDITEM);
		return;
	}
	if(!InventoryHasSlot(1))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	DATA_TYPE dt;
	const PROP_ADD_MATERIAL_ITEM_ESSENCE &ess= *(const PROP_ADD_MATERIAL_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it.body->GetPropAddMaterialId(), ID_SPACE_ESSENCE, dt); 
	if(dt != DT_PROP_ADD_MATERIAL_ITEM_ESSENCE || &ess == NULL)
	{
		_runner->error_message(S2C::ERR_FAILED_REBUILD_PROPADDITEM);
		return;
	}

	bool find = false;
	for(size_t i = 0; i < sizeof(ess.forge_item) / sizeof(ess.forge_item[0]); i ++)
	{
		if(ess.forge_item[i] == forgeId)
		{
			find = true;
			break;
		}
	}
	if(!find)
	{
		_runner->error_message(S2C::ERR_FAILED_REBUILD_PROPADDITEM_MATERIAL_NOT_ENOUGH);
		return;
	}

	item& forgeIt = _inventory[forgeIdx];
	if(forgeIt.type != forgeId)
	{
		_runner->error_message(S2C::ERR_FAILED_REBUILD_PROPADDITEM_MATERIAL_NOT_ENOUGH);
		return;
	}

	int idx = RandSelect(&ess.forge_result[0].prop, sizeof(ess.forge_result[0]), sizeof(ess.forge_result) / sizeof(ess.forge_result[0]));
	int newItemId = ess.forge_result[idx].id;
	int newItemInvIdx = -1;
	DeliverItem(newItemId, 1, true, 0, ITEM_INIT_TYPE_TASK, newItemInvIdx);
	_runner->player_get_propadd_item(newItemId, newItemInvIdx);

	UseItemLog(_inventory[itemIdx], 1);
	_inventory.DecAmount(itemIdx, 1);
	_runner->player_drop_item(IL_INVENTORY, itemIdx, itemId, 1, S2C::DROP_TYPE_USE);

	UseItemLog(_inventory[forgeIdx], 1);
	_inventory.DecAmount(forgeIdx, 1);
	_runner->player_drop_item(IL_INVENTORY, forgeIdx, forgeId, 1, S2C::DROP_TYPE_USE);
}

void gplayer_imp::SavePropAdd(archive& ar)
{
	int version = 0;
	ar << version;
	for(size_t i = 0; i < sizeof(_prop_add) / sizeof(_prop_add[0]); i ++)
	{
		ar << _prop_add[i];
	}
}

void gplayer_imp::LoadPropAdd(archive& ar)
{
	int version;
	if(ar.size() != sizeof(_prop_add) + sizeof(version))
	{
		return;
	}
	ar >> version;
	if(version == 0)
	{
		for(size_t i = 0; i < sizeof(_prop_add) / sizeof(_prop_add[0]); i ++)
		{
			ar >> _prop_add[i];
		}
	}
}

void gplayer_imp::DebugChangeEquipOwnerID()
{
	ChangeItemOwnerID(_inventory);
	ChangeItemOwnerID(_equipment);
	ChangeItemOwnerID(_task_inventory);
	ChangeItemOwnerID(_trashbox.Backpack());
	ChangeItemOwnerID(_pet_bedge_inventory);
	ChangeItemOwnerID(_fashion_inventory);
	ChangeItemOwnerID(_mount_wing_inventory);

}

void gplayer_imp::ChangeItemOwnerID(item_list & inv)
{
	for(size_t i = 0; i < inv.Size(); ++i)
	{
		item & it = inv[i];
		if(it.type != -1 && it.body != NULL)
		{
			it.ChangeOwnerID(inv.GetLocation(), i, this);
		}
	}
}


void gplayer_imp::PlayerBuyKingdomItem(char type, int index)
{
	if(type != 1 && type != 2) return;
	if(index < 0) return;

	//国王购买积分物品
	if(type == 1)
	{
		if(!IsKingdomKing()) return;
		int item_id = 0;
		int item_num = 0;
		int need_score = 0;

		player_template::GetKingdomShop(index, item_id, item_num, need_score);

		if(item_id <= 0 || item_num <= 0 || need_score <= 0) return;
		if(GetKingdomPoint() < need_score) return;

		//检查包裹是否足够
		if(!_inventory.GetEmptySlotCount())
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return;
		}


		ConsumeKingdomPoint(need_score);
		DeliverItem(item_id,item_num, false, 0, ITEM_INIT_TYPE_TASK);
		_runner->notify_kingdom_info(GetKingdomTitle(), GetKingdomPoint());

		struct
		{
			int item_id;
			int item_num;
			int score;
		}data;
		memset(&data, 0,sizeof(data));
		data.item_id = item_id;
		data.item_num = item_num;
	        data.score = need_score;	
		broadcast_chat_msg(KINGDOM_BUY_ITEM_CHAT_MESSAGE_ID,&data,sizeof(data),GMSV::CHAT_CHANNEL_SYSTEM,0,0,0);
		GLog::log(GLOG_INFO, "国王购买了国王积分物品, roleid=%d, mafia=%d, item_id=%d, item_num=%d, score=%d", _parent->ID.id, OI_GetMafiaID(), item_id, item_num, need_score);
	}
	//个人购买积分物品
	else if(type == 2)
	{
	}
}

void gplayer_imp::AddKingdomState()
{
	_filters.AddFilter(new kingdom_exp_filter(this,FILTER_INDEX_KINGDOM_EXP));
}

void gplayer_imp::RemoveKingdomState()
{
	//试图去除骑乘效果
	if(_filters.IsFilterExist(FILTER_INDEX_KINGDOM_EXP))
	{
		_filters.RemoveFilter(FILTER_INDEX_KINGDOM_EXP);
	}
}

void gplayer_imp::SyncKingdomTitle(int new_title)
{
	if(new_title == 0)
	{
		RemoveKingdomState();
	}
	else
	{
		_kingdom_title = new_title;
		AddKingdomState();
	}
}

void gplayer_imp::TryFixRebornLevel()
{
	if((_flag_mask & FIX_REBORN_LEVEL) == 0)
	{
		_flag_mask |= FIX_REBORN_LEVEL;

		//147-149的玩家判断如果经验够的话直接升级
		if(GetRebornCount() == 1 && GetObjectLevel() >= 147 && GetObjectLevel() < 150)
		{
			int64_t exp = player_template::GetLvlupExp(GetRebornCount(), _basic.level);
			if(_basic.exp >= exp)
			{
				GLog::log(GLOG_INFO,"用户%d登录后处于飞升147到飞升149等级段，执行了等级经验修正操作, level=%d", _parent->ID.id, GetObjectLevel());
				_basic.exp = exp;
				LevelUp();
			}
		}	
		//飞升150的玩家发布一个任务物品给与补偿
		else if(GetRebornCount() == 1 && GetObjectLevel() == 150)
		{
			if(_task_inventory.GetEmptySlotCount() != 0)
			{
				int task_id = 58276;
				DeliverTaskItem(task_id, 1);
				GLog::log(GLOG_INFO,"用户%d登录后处于飞升150等级，发放了任务补偿道具", _parent->ID.id);
			}
		}
	}	
}

void gplayer_imp::KingTryCallGuard()
{
	if(!IsKingdomKing()) return;
	if(!_pstate.IsNormalState()) 
	{
		_runner->error_message(S2C::ERR_KING_CALL_GUARD_NOT_ALLOWED);
		return;
	}

	if(!CheckCoolDown(COOLDOWN_INDEX_KING_CALL_GUARD))
	{
		_runner->error_message(S2C::ERR_KING_CALL_GUARD_IS_COOLING);
		return;
	}
	SetCoolDown(COOLDOWN_INDEX_KING_CALL_GUARD, 60000);

	GLog::log(GLOG_INFO,"国王%d执行了召唤亲卫操作, tag=%d", _parent->ID.id, GetWorldTag());

	//国王战地图帮战地图特殊逻辑，国王可以直接召唤本地图的所有亲卫，不走delivery
	if(_plane->GetWorldManager()->IsKingdomWorld() || (_plane->GetWorldManager()->IsRaidWorld() && _plane->GetWorldManager()->GetRaidType() == RT_MAFIA_BATTLE))
	{
		msg_kingdom_call_guard mkcg;
		memset(&mkcg, sizeof(mkcg), 0);
		memcpy(mkcg.kingname, _username, sizeof(mkcg.kingname));
		mkcg.kingname_len = _username_len;
		mkcg.line_id = gmatrix::GetServerIndex();
		mkcg.map_id = GetWorldTag();
		mkcg.pos = GetParent()->pos;
		
		MSG msg;	
		BuildMessage(msg,GM_MSG_KINGDOM_CALL_GUARD,XID(GM_TYPE_PLAYER,-1),XID(-1,-1),A3DVECTOR(0,0,0), 0, &mkcg, sizeof(mkcg)); 
		std::vector<exclude_target> empty;
		_plane->BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);
		return;
	}
	//其他战场和副本地图不可以召唤
	else if( _plane->GetWorldManager()->IsBattleWorld() || _plane->GetWorldManager()->IsRaidWorld() || _plane->GetWorldManager()->IsBattleFieldWorld()) 
	{
		_runner->error_message(S2C::ERR_KING_CALL_GUARD_WORLD_NOT_ALLOWED);
		return;
	}
	//普通大世界
	else
	{
		GMSV::SendKingTryCallGuard(_parent->ID.id, GetWorldTag(), _parent->pos.x, _parent->pos.y, _parent->pos.z);
	}

}


bool gplayer_imp::GuardCallByKing(int roleid, int lineid, int mapid, float pos_x, float pos_y, float pos_z)
{
	if(_plane->GetWorldManager()->IsBattleWorld() || _plane->GetWorldManager()->IsRaidWorld()) return false;
	if(!_pstate.IsNormalState()) return false;
	if(IsCombatState())
	{
		ActiveCombatState(false);
	}


	//本线直接传送
	if(gmatrix::GetServerIndex() == lineid)
	{
		return LongJump(A3DVECTOR(pos_x, pos_y, pos_z), mapid);
	}
	//换线逻辑
	else
	{
		A3DVECTOR pos(pos_x, pos_y, pos_z);
		ChangePlayerGameServer(lineid, mapid, pos, 0);
		return true;
	}
	return false;
}


void gplayer_imp::KingCallGuardInvite(char * kingname, size_t kingname_len, int line_id,  int map_id, A3DVECTOR & pos)
{
	if(!IsKingdomGuard()) return;
	if(!_pstate.IsNormalState()) return;

	GMSV::SendGuardCalledByKing(kingname, kingname_len, GetParent()->cs_index, GetParent()->cs_sid, GetParent()->ID.id, line_id, map_id, pos.x, pos.y, pos.z);
}

bool gplayer_imp::KingCallGuardReply(int line_id, int map_id, float pos_x, float pos_y, float pos_z)
{
	if(_plane->GetWorldManager()->IsKingdomWorld() || (_plane->GetWorldManager()->IsRaidWorld() && _plane->GetWorldManager()->GetRaidType() == RT_MAFIA_BATTLE))
	{
		if(gmatrix::GetServerIndex() != line_id || GetWorldTag() != map_id) return false;
		return LongJump(A3DVECTOR(pos_x, pos_y, pos_z), map_id);
	}
	return false;
}

bool gplayer_imp::PlayerGetTouchAward(char type, int item_count)
{
	if(type != 1 && type != 2) return false;
	if(item_count <=0 ) return false;
	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;
	if(!_pstate.CanTrade() || _cur_session || _session_list.size()) return false;

	int item_id = 0;
	int points_need = 0;
	if(type == 1) 
	{
		item_id = 23314;
		points_need = 50 * item_count;
	}
	else if(type == 2) 
	{
		item_id = 23315;
		points_need = 5 * item_count;
	}

	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
	size_t need_slot_count = item_count / pile_limit + ((item_count % pile_limit == 0) ? 0 : 1); 
	if(need_slot_count > _inventory.GetEmptySlotCount()) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	return GNET::SendTouchPointExchange(_parent->ID.id, type, item_count, points_need, object_interface(this));
}

bool gplayer_imp::PlayerExchangeTouchPoint(int retcode, unsigned int flag, unsigned int count)
{
	TradeUnLockPlayer();
	if(retcode != 0) return false;
	if(flag != 1 && flag != 2) return false;
	
	int item_id = 0;
	if(flag == 1) 
	{
		item_id = 23314;	//金瓜子
	}
	else if(flag == 2) 
	{
		item_id = 23315;	//银瓜子
	}

	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
	while(count > 0)
	{
		int item_count = count;
		if(count > pile_limit) 
		{
			item_count = pile_limit;
		}
		DeliverItem(item_id,item_count, false, 0, ITEM_INIT_TYPE_TASK);
		count -= item_count;
	}
	GLog::log(GLOG_INFO,"用户%d成功兑换touch点,count=%d, flag=%d", _parent->ID.id, flag, count);
	return true;
}	


bool gplayer_imp::PlayerEnterBath()
{
	if( _plane->GetWorldManager()->IsBattleWorld() || _plane->GetWorldManager()->IsRaidWorld() || _plane->GetWorldManager()->IsFacBase())
	{
		_runner->error_message(S2C::ERR_BATH_CANNOT_IN);
		return false;
	}

	if(!gmatrix::IsBathOpen())
	{
		_runner->error_message(S2C::ERR_BATH_NOT_OPEN);
		return false;
	}

	if(!IsPlayerFemale())
	{
		//第一次进入
		int bath_count = gmatrix::GetBathCount(_parent->ID.id);
		if(bath_count == -1)
		{
			size_t need_money = 1000000;
			if(GetMoney() < need_money) 
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
				return false;	//只报告金钱不够的错误
			}
			GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=7:hint=%d",GetParent()->ID.id,GetUserID(),need_money,KINGDOM_BATH_TAG_ID);
			SpendMoney(need_money);
			_runner->spend_money(need_money);
		}
	}

	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey( KINGDOM_BATH_TAG_ID, key );
	key.target = key.essence;
	key.target.key_level4 = 0;
	A3DVECTOR pos(0,0,0);//这个pos不重要进入战场后会调整

	_battle_faction = 1;
	if( GetWorldManager()->PlaneSwitch( this, pos, KINGDOM_BATH_TAG_ID, key, 0 ) < 0 )
	{
		return false;
	}
	return true;
}


bool gplayer_imp::PlayerDeliverKingdomTask(int task_type)
{
	if(task_type <= 0 || task_type > 4) return false;

	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;
	if(!_pstate.CanTrade() || _cur_session || _session_list.size()) return false;
	if(!IsKingdomKing()) return false;

	int cash_table[] = {0, 10000, 20000, 50000};
	int need_cash = cash_table[task_type-1];

	if(_mallinfo.GetCash() < need_cash)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}	
	return GNET::SendKingDeliverTask(_parent->ID.id, task_type, object_interface(this)); 
}

bool gplayer_imp::KingDeliverTaskResponse(int retcode, int task_type)
{
	TradeUnLockPlayer();
	if(retcode != 0) return false;

	if(task_type <= 0 || task_type > 4) return false;
	int cash_table[] = {0, 10000, 20000, 50000};
	int need_cash = cash_table[task_type-1];
	if(_mallinfo.GetCash() < need_cash)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}	
	if(need_cash <= 0) return true;

	int order_id = _mallinfo.GetOrderID(); 
	PlayerUseCash(57405, 1, 0, need_cash, order_id);
	GLog::log(GLOG_INFO,"国王%d成功发布任务,need_cash=%d, task_type=%d", _parent->ID.id, need_cash, task_type);
	return true;
}

bool gplayer_imp::PlayerReceiveKingdomTask(int task_type)
{
	if(task_type <= 0 || task_type > 4) return false;
	if(task_type != gmatrix::GetKingdomTaskType()) return false;
	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;

	int task_list[] = {28506, 28507, 28508, 28509};
	int task_id = task_list[task_type-1];
	if(task_id <= 0) return false;

	PlayerTaskInterface  task_if(this);
	if(OnTaskCheckDeliver(&task_if,task_id, 0))
	{
		GLog::log(GLOG_INFO,"玩家%d成功接收国王任务, task_type=%d, task_id=%d", _parent->ID.id, task_type, task_id);
		return true;
	}
	else
	{
		_runner->error_message(S2C::ERR_TASK_NOT_AVAILABLE);
		return false;
	}
	return false;
}

bool gplayer_imp::PlayerFlyBathPos()
{
	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;

	if( _plane->GetWorldManager()->IsBattleWorld() || _plane->GetWorldManager()->IsRaidWorld() || _plane->GetWorldManager()->IsFacBase() )  
	{
		_runner->error_message(S2C::ERR_WORLD_CANNOT_FLY_BATH_POS);
		return false;
	}

	if(!gmatrix::IsBathOpen())
	{
		_runner->error_message(S2C::ERR_BATH_NOT_OPEN);
		return false;
	}

	A3DVECTOR pos(515, 494.5, 330);
	LongJump(pos, 401);
	return true;
}


bool gplayer_imp::PlayerComposeFuwen(bool consume_extra_item, int extra_item_index)
{

	class op : public session_general_operation::operation
	{
	public:
		op(bool consume_extra_item, int extra_item_index) : _consume_extra_item(consume_extra_item), _extra_item_index(extra_item_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_FUWEN_COMPOSE;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(2);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
		}

		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			pImp->ComposeFuwen(_consume_extra_item, _extra_item_index);
		}

		virtual bool OnAttacked(gactive_imp * obj)
		{	
			return true;
		}

	private:
		bool _consume_extra_item;
		int _extra_item_index;
	};

	AddSession(new session_general_operation(this, new op(consume_extra_item, extra_item_index)));
	StartSession();
	return true;
}


bool gplayer_imp::ComposeFuwen(bool consume_extra_item, int extra_item_index)
{
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}

	//检查包裹是否足够
	if(!_inventory.GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	//检查今日合成次数是否达到上限
	if(_fuwen_compose_info.compose_count >= 20) return false;
	//如果当日使用额外合成次数达到上限则不能继续使用
	if(consume_extra_item && _fuwen_compose_info.extra_compose_count >= 10) return false;
	if(!consume_extra_item && (_fuwen_compose_info.compose_count - _fuwen_compose_info.extra_compose_count >= 10)) return false;

	//如果增加额外次数, 检查物品是否存在 
	if(consume_extra_item)
	{
		if(extra_item_index < 0 || (size_t)extra_item_index >= _inventory.Size()) return false;
		if(_inventory[extra_item_index].type == -1 || _inventory[extra_item_index].type == 0) return false;
		if(_inventory[extra_item_index].type != g_config.fuwen_compose_extra_id[0] && _inventory[extra_item_index].type != g_config.fuwen_compose_extra_id[1]) return false;
	}


	//检查符文碎片的数量是否足够
	int fuwen_fragment_count = 0;
	if(g_config.fuwen_fragment_id[0] > 0) fuwen_fragment_count += _inventory.CountItemByID(g_config.fuwen_fragment_id[0]);	
	if(g_config.fuwen_fragment_id[1] > 0) fuwen_fragment_count += _inventory.CountItemByID(g_config.fuwen_fragment_id[1]);	
	if(fuwen_fragment_count < 10) return false;


	int fuwen_id = gmatrix::GetFuwenComposeID();

	DATA_TYPE dt;
	const RUNE2013_ITEM_ESSENCE &ess= *(const RUNE2013_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(fuwen_id, ID_SPACE_ESSENCE,dt); 
	if(dt != DT_RUNE2013_ITEM_ESSENCE|| &ess == NULL)
	{
		return false;
	}

	//优先扣除第一个碎片物品
	int fuwen_fragment0_count = 0;
	if(g_config.fuwen_fragment_id[0] > 0) fuwen_fragment0_count = _inventory.CountItemByID(g_config.fuwen_fragment_id[0]);	
	int fuwen_fragment1_count = 0;
	if(g_config.fuwen_fragment_id[1] > 0) fuwen_fragment1_count = _inventory.CountItemByID(g_config.fuwen_fragment_id[1]);	
	if(fuwen_fragment0_count >= 10)
	{
		TakeOutItem(g_config.fuwen_fragment_id[0], 10);
	}
	else 
	{
		if(fuwen_fragment0_count > 0)
		{
			TakeOutItem(g_config.fuwen_fragment_id[0], fuwen_fragment0_count);
		}
		TakeOutItem(g_config.fuwen_fragment_id[0], 10-fuwen_fragment0_count);
	}

	//扣除道具
	if(consume_extra_item)
	{
		_fuwen_compose_info.extra_compose_count++;
		TakeOutItem(extra_item_index, _inventory[extra_item_index].type, 1);
	}

	_fuwen_compose_info.compose_count++;
	DeliverItem(fuwen_id,1, false, 0, ITEM_INIT_TYPE_TASK);
	_runner->fuwen_compose_info(_fuwen_compose_info.compose_count, _fuwen_compose_info.extra_compose_count);
	_runner->fuwen_compose_end();

	//极品符文世界喊话
	if(ess.quality == 3)
	{
		struct
		{
			int player_id;
			char name[MAX_USERNAME_LENGTH];
			int item_id;
		}data;
		memset(&data, 0,sizeof(data));
		data.player_id= _parent->ID.id;
		size_t  len = _username_len;
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(data.name, _username,len);
		data.item_id = fuwen_id;
		broadcast_chat_msg(FUWEN_COMPOSE_MESSAGE_ID,&data,sizeof(data),GMSV::CHAT_CHANNEL_SYSTEM,0,0,0);
	}

	GLog::log(GLOG_INFO,"玩家%d执行了符文碎片合成操作, fuwen_id=%d, quality=%d, consume_extra_item=%d, total_compose_count=%d, free_compose_count=%d", _parent->ID.id, 
			fuwen_id, ess.quality, consume_extra_item, _fuwen_compose_info.compose_count, _fuwen_compose_info.compose_count - _fuwen_compose_info.extra_compose_count); 
	return true;
}

void gplayer_imp::SendFuwenComposeInfo()
{
	_runner->fuwen_compose_info(_fuwen_compose_info.compose_count, _fuwen_compose_info.extra_compose_count);
}	      


void gplayer_imp::ResetFuwenComposeInfo()
{
	_fuwen_compose_info.compose_count = 0;
	_fuwen_compose_info.extra_compose_count = 0;
	SendFuwenComposeInfo();
}

void gplayer_imp::LoadFuwenComposeInfo(archive & ar)
{
	if(ar.size() == 0) return;
	ASSERT(ar.size() == sizeof(fuwen_compose));

	ar >> _fuwen_compose_info.compose_count >> _fuwen_compose_info.extra_compose_count;
}

void gplayer_imp::SaveFuwenComposeInfo(archive & ar)
{
	if(_fuwen_compose_info.compose_count == 0 && _fuwen_compose_info.extra_compose_count == 0) return;

	ar << _fuwen_compose_info.compose_count << _fuwen_compose_info.extra_compose_count;
}

bool gplayer_imp::PlayerUpgradeFuwen(int main_fuwen_index, int main_fuwen_where, int assist_count, int assist_fuwen_index[])
{
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}
	if(main_fuwen_where != IL_INVENTORY && main_fuwen_where != IL_FUWEN) return false;

	if(assist_count < 0 || assist_count > 200) return false;
	if(main_fuwen_where == IL_INVENTORY && (main_fuwen_index < 0 || (size_t)main_fuwen_index >= _inventory.Size())) return false;
	if(main_fuwen_where == IL_FUWEN && (main_fuwen_index < 0 || (size_t)main_fuwen_index >= _fuwen_inventory.Size())) return false;

	item & it = (main_fuwen_where == IL_INVENTORY ? _inventory[main_fuwen_index] : _fuwen_inventory[main_fuwen_index]);
	if(it.type ==-1 || it.body == NULL)  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_FUWEN) return false;
	if(((item_fuwen*)(it.body))->GetFuwenLevel(&it) >= player_template::GetFuwenMaxLevel()) return false;


	for(int i = 0; i < assist_count; ++i)
	{
		int assist_index = assist_fuwen_index[i];
		if(assist_index < 0 || (size_t)assist_index >= _inventory.Size()) return false;

		//检查是否是符文或者碎片
		item & it2 = _inventory[assist_index];
		if(it2.type == 0 || it2.type == -1) return false;
		if(it2.type != g_config.fuwen_fragment_id[0] && it2.type != g_config.fuwen_fragment_id[1] && it2.GetItemType() != item_body::ITEM_TYPE_FUWEN) return false;
		if(it2.IsLocked()) return false;
	}

	((item_fuwen*)(it.body))->UpgradeFuwen(&it, this, main_fuwen_index, main_fuwen_where, assist_count, assist_fuwen_index);

	for(int i = 0; i < assist_count; ++i)
	{
		int assist_index = assist_fuwen_index[i];

		item & it2 = _inventory[assist_index];
		TakeOutItem(assist_index, it2.type, it2.count);
	}

	GLog::log(GLOG_INFO,"玩家%d执行了符文升级操作, main_fuwen_index=%d, main_fuwen_where=%d, assist_fuwen_count=%d", _parent->ID.id, main_fuwen_index, main_fuwen_where, assist_count);
	return true;
}

bool gplayer_imp::PlayerInstallFuwen(int src_index, int dst_index)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return false;
	}

	if(src_index < 0 || (size_t)src_index >= _inventory.Size()) return false;
	if(dst_index < 0 || (size_t)dst_index >= _fuwen_inventory.Size()) return false;
	if(GetRebornCount() == 0 || GetObjectLevel() < 150) return false;

	item & it = _inventory[src_index];
	if(it.type ==-1 || it.body == NULL)  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_FUWEN) return false;

	int max_index = (GetObjectLevel() - 150) / 2;
	if(dst_index > max_index) return false;

	item & it2 = _fuwen_inventory[dst_index];
	if(it2.type != -1) return false;

	int prop_type = ((item_fuwen*)it.body)->GetFuwenPropType();
	if(prop_type < 0) return false;

	for(int i = 0; i <= max_index; ++i)
	{
		item & it3 = _fuwen_inventory[i];
		if(it3.type == -1 || it3.type == 0) continue;
		if(it3.GetItemType() != item_body::ITEM_TYPE_FUWEN) return false;
		if(prop_type == ((item_fuwen*)it3.body)->GetFuwenPropType()) return false;
	}

	int delta = MoveBetweenItemList(_inventory,_fuwen_inventory, src_index, dst_index, 1);
	if(delta >= 0)
	{
		_runner->fuwen_install_result(src_index, dst_index);
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
	RefreshFuwen();
	return true;
}


bool gplayer_imp::PlayerUninstallFuwen(int fuwen_index, int assist_index, int inv_index)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return false;
	}

	if(fuwen_index < 0 || (size_t)fuwen_index >= _fuwen_inventory.Size()) return false;
	if(assist_index < 0 || (size_t)assist_index >= _inventory.Size()) return false;
	if(inv_index < 0 || (size_t)inv_index >= _inventory.Size()) return false;

	
	item & it = _fuwen_inventory[fuwen_index];
	if(it.type ==-1 || it.body == NULL)  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_FUWEN) return false;
	
	item & it2 = _inventory[assist_index];
	if(it2.type == -1 || it2.type == 0) return false;
	if(it2.type != g_config.fuwen_erase_id[0] && it2.type != g_config.fuwen_erase_id[1]) return false; 

	item & it3 = _inventory[inv_index];
	if(it3.type != -1) return false;

	TakeOutItem(assist_index, it2.type, 1); 

	int delta = MoveBetweenItemList(_fuwen_inventory,_inventory, fuwen_index, inv_index, 1);
	if(delta >= 0)
	{
		_runner->fuwen_uninstall_result(fuwen_index, inv_index);
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
	RefreshFuwen();
	return true;
}

bool gplayer_imp::GetResizeInvCostItem(char type, size_t size, size_t new_size, int& cnt_below_96, int& cnt_above_96)
{
	cnt_below_96 = 0;
	cnt_above_96 = 0;
	if(new_size <= size) return false;

	if(size < 96)
	{
		if(new_size <= 96)
		{
			if(type == 0 && size <= 48)
			{
				cnt_below_96 += ((new_size - 48) / 12);
			}
			else
			{
				cnt_below_96 += (new_size - size) / 12;
			}
		}
		else
		{
			if(type == 0 && size <= 48)
			{
				cnt_below_96 += ((96 - 48) / 12);
			}
			else
			{
				cnt_below_96 += ((96 - size) / 12);
			}
			cnt_above_96 += (new_size - 96) / 6;
		}
	}
	else 
	{
		cnt_above_96 += (new_size - size) / 6;
	}
	return true;
}

bool gplayer_imp::ResizeInv(size_t size, char type, int cnt, const void* buf)
{
	if(gmatrix::IsZoneServer())
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}

	item_list & inv = GetInventory();
	item_list * tInv = 0;
	if(type == 0)
	{
		tInv = &_inventory;
		if(size > ITEM_LIST_MAX_SIZE) size = ITEM_LIST_MAX_SIZE;
		if(size < ITEM_LIST_BASE_SIZE) size = ITEM_LIST_BASE_SIZE;
	}
	else if(type == 1)
	{
		tInv = &_trashbox.Backpack();
		if(size > TRASHBOX_MAX_SIZE) size = TRASHBOX_MAX_SIZE;
		if(size < TRASHBOX_BASE_SIZE) size = TRASHBOX_BASE_SIZE;
	}
	if(!tInv)	
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}

	int oldSize = tInv->Size();
	if(size <= tInv->Size())
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}
	if(size % 6 != 0)
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}
	if(type == 0 && size <= 48)
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}
	if(size <= 96 && (size % 12 != 0))
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}
	if(size > 96 && ((size - 96) % 6 != 0))
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE);
		return false;
	}

	int cnt_below_96 = 0;
	int cnt_above_96 = 0;
	if(!GetResizeInvCostItem(type, tInv->Size(), size, cnt_below_96, cnt_above_96))
	{
		return false;
	}

	int trashbox_item_96_below = 4011;
	int trashbox_item_96_above = 13903;
	int inv_item_96_below = 4012;
	int inv_item_96_above = 13904;

	C2S::CMD::resize_inv::cost_item* list = (C2S::CMD::resize_inv::cost_item*)buf;
	for(int i = 0; i < cnt; i ++)
	{
		C2S::CMD::resize_inv::cost_item& it = list[i];
		if(!inv.IsItemExist(it.item_idx, it.item_id, it.item_cnt))
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		
		if(type == 0)
		{
			if(inv_item_96_below == it.item_id)
			{
				cnt_below_96 -= it.item_cnt;
			}
			else if(inv_item_96_above == it.item_id)
			{
				cnt_above_96 -= it.item_cnt;
			}
		}
		else if(type == 1)
		{
			if(trashbox_item_96_below == it.item_id)
			{
				cnt_below_96 -= it.item_cnt;
			}
			else if(trashbox_item_96_above == it.item_id)
			{
				cnt_above_96 -= it.item_cnt;
			}
		}
	}
	if(cnt_below_96 > 0 || cnt_above_96 > 0)
	{
		_runner->error_message(S2C::ERR_CHANGE_INV_TRASHBOX_SIZE_NOT_ENOUGH_ITEM);
		return false;
	}

	for(int i = 0; i < cnt; i ++)
	{
		C2S::CMD::resize_inv::cost_item& it = list[i];
		UseItemLog(inv, it.item_idx, it.item_cnt);
		inv.DecAmount(it.item_idx, it.item_cnt);
		_runner->use_item(gplayer_imp::IL_INVENTORY,it.item_idx, it.item_id, it.item_cnt);
	}

	tInv->SetSize(size);
	if(type == 0)
	{
		_runner->player_change_inventory_size(tInv->Size());
	}
	else if(type == 1)
	{
		((gplayer_dispatcher*)_runner)->trashbox_capacity_notify(tInv->Size());
	}

	GLog::log(GLOG_INFO,"用户%d将%s格数从%d扩展到%d",_parent->ID.id, type == 0 ? "包裹" : "仓库", oldSize, size);
	return true;
}

bool gplayer_imp::RegisterFlowBattle()
{
	std::vector<int> idList;
	if(_team.IsLeader())
	{
		for(int i = 0; i < _team.GetMemberNum(); i ++)
		{
			idList.push_back(_team.GetMember(i).id.id);
		}
	}
	else
	{
		idList.push_back(GetParent()->ID.id);
	}
	GNET::SendOpenBanquetJoin(gmatrix::GetServerIndex(), _team.IsLeader(), idList);
	return true;
}

bool
gplayer_imp::IsForbidTask(int ulTaskId)
{
	return forbid_manager::IsForbidTask(ulTaskId);
	
}


//玩家领取web订单
bool
gplayer_imp::PlayerGetWebOrder(int64_t order_id)
{
	if(order_id < 0) return false;
	if(_mallinfo._web_order_list.empty()) return false;

	for(size_t i = 0; i < _mallinfo._web_order_list.size(); ++i)
	{
		if(_mallinfo._web_order_list[i]._order_id == order_id && _parent->ID.id == _mallinfo._web_order_list[i]._roleid && _mallinfo._web_order_list[i]._status == 0)
		{
			if(_mallinfo._web_order_list[i]._order_package.empty()) return false;

			//检查cash和包裹位置是不是够
			int need_cash = 0;
			size_t need_total_slot = 0;
			for(size_t j = 0; j < _mallinfo._web_order_list[i]._order_package.size(); ++j)
			{
				if(_mallinfo._web_order_list[i]._pay_type == 2) need_cash += _mallinfo._web_order_list[i]._order_package[j]._count * _mallinfo._web_order_list[i]._order_package[j]._price;

				for(size_t k = 0; k < _mallinfo._web_order_list[i]._order_package[j]._order_goods.size(); ++k)
				{
					int item_id = _mallinfo._web_order_list[i]._order_package[j]._order_goods[k]._goods_id; 
					size_t item_count = _mallinfo._web_order_list[i]._order_package[j]._order_goods[k]._goods_count *  _mallinfo._web_order_list[i]._order_package[j]._count;
						
					size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
					size_t need_slot_count = item_count / pile_limit + ((item_count % pile_limit == 0) ? 0 : 1); 
					need_total_slot += need_slot_count;
				}	
			}

			if(_mallinfo.GetCash2() < need_cash) return false;
			if(_inventory.GetEmptySlotCount() < need_total_slot) return false;

			//标记为已经领取 
			_mallinfo._web_order_list[i]._status = 1;
			_mallinfo.CalcWebCash();

			//发放物品
			for(size_t j = 0; j < _mallinfo._web_order_list[i]._order_package.size(); ++j)
			{
				for(size_t k = 0; k < _mallinfo._web_order_list[i]._order_package[j]._order_goods.size(); ++k)
				{
					int item_id = _mallinfo._web_order_list[i]._order_package[j]._order_goods[k]._goods_id; 
					size_t item_count = _mallinfo._web_order_list[i]._order_package[j]._order_goods[k]._goods_count *  _mallinfo._web_order_list[i]._order_package[j]._count;
					bool is_bind = (_mallinfo._web_order_list[i]._order_package[j]._order_goods[k]._goods_flag & 0x01);
					int expire_time =  _mallinfo._web_order_list[i]._order_package[j]._order_goods[k]._goods_time;

					GainItem(item_id, item_count, is_bind, expire_time, ITEM_INIT_TYPE_TASK);
					GLog::log(GLOG_INFO,"用户%d从web商城购买获得item_id=%d, item_count=%d, is_bind=%d, expire_time=%d, order_id=%lld, package_id=%d",
							_parent->ID.id, item_id, item_count, is_bind, expire_time,  _mallinfo._web_order_list[i]._order_id, 
							_mallinfo._web_order_list[i]._order_package[j]._package_id);
				}	

				int total_cash = _mallinfo._web_order_list[i]._order_package[j]._count * _mallinfo._web_order_list[i]._order_package[j]._price;
				//扣除cash
				if(_mallinfo._web_order_list[i]._pay_type == 2)
				{
					PlayerUseCash( _mallinfo._web_order_list[i]._order_package[j]._package_id + WEB_PACKAGE_BASE_ID, _mallinfo._web_order_list[i]._order_package[j]._count, 0, total_cash, -2);
				}
			}

			_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
			_runner->web_order_info();
			return true;
		}
	}
	return false;
}

bool gplayer_imp::PlayerCustomizeRune(int type, int count, int prop[])
{
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}

        if(!_inventory.GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	int prop_count[] = {5, 6, 8, 8};
	int customize_count[] = {3, 4, 4, 5};
	int need_rune_score[] = {25000, 60000, 200000, 400000};
	int rune_customize_mask[] = {RUNE_CUSTOMIZE1, RUNE_CUSTOMIZE2, RUNE_CUSTOMIZE3, RUNE_CUSTOMIZE4};
	int rune_prop[34];

	memset(rune_prop, 0, sizeof(rune_prop));
	
	if(type <= 0 || type > 4) return false; 
	if(count <= 0 || count > 5) return false; 

	if(customize_count[type-1] !=  count) return false;
	if(GetRuneScore() < need_rune_score[type-1]) return false;
	if(_flag_mask & rune_customize_mask[type-1]) return false; 


	for(int i = 0; i < count; ++i)
	{
		if(prop[i] <= 0 || prop[i] > 33) return false;
		rune_prop[prop[i]]++; 
	}

	int front_prop = 0;
	int back_prop = 0;
	//前面27条属性最多可出现2次, 1-8中任意，若出现2条，则前4条中出现1条，后4条中出现1条
	for(size_t i = 0; i < 27; ++i)
	{
		if(rune_prop[i] > 2) return false;
		if(rune_prop[i] == 2 && prop_count[type-1] < 5) return false;
		if(rune_prop[i] == 2)
		{
			front_prop++;
		}
	}

	//28-33的属性,最多出现一次，如果出现必定在前面
	for(size_t i = 28; i < 34; ++i)
	{
		if(rune_prop[i] > 1) return false;
		if(rune_prop[i] == 1) front_prop++;
	}

	//检查前4条属性和后面4条属性个数
	if(front_prop > 4) return false;
	if(back_prop +4 > prop_count[type-1]) return false;
	
	item & it = _equipment[item::EQUIP_INDEX_RUNE];
	if(it.GetItemType() != item_body::ITEM_TYPE_RUNE) return false; 

	item_data * data = NULL;
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	data = gmatrix::GetDataMan().generate_item(51479,&tag,sizeof(tag));
	data->count = 1;
	item_list & inv = GetInventory(IL_INVENTORY);
	int rst = inv.Push(*data);
	if(rst >=0)
	{
		item & it = _inventory[rst];
		it.Bind();
		int state = item::Proctype2State(it.proc_type);
		_runner->obtain_item(51479,0,1, 1, IL_INVENTORY,rst, state);

		((rune_item*)(it.body))->Customize(&it, this, count, prop, rst, type);

		_flag_mask |= rune_customize_mask[type-1];
		_runner->rune_info(IsRuneActive(), GetRuneScore(), GetRuneCustomizeInfo());

		char log_prop[32];
		memset(log_prop, 0, sizeof(log_prop));
		char * p = log_prop;
		for(int i = 0; i < count; ++i)
		{
			int num = sprintf(p, "%d-", prop[i]);
			p += num;
		}
		GLog::log(GLOG_INFO,"用户%d执行了元婴定制操作, type=%d, count=%d, prop=%s", _parent->ID.id, type, count, log_prop);
		return true;



	}

	return false;

}


void gplayer_imp::PlayerControlTrap(int id, int tid)
{
	_summonman.ControlTrap(this, id, tid);
		
}

bool gplayer_imp::PlayerSummonTeleport(int npc_id)
{
	if(GetParent()->IsFlyMode()) return false;
	if(!CheckCoolDown(COOLDOWN_INDEX_TELEPORT1))
	{
		_runner->error_message(S2C::ERR_SUMMON_TELEPORT1_IS_COOLING);
		return false;
	}

	MSG msg;
	BuildMessage2(msg,GM_MSG_SUMMON_TELEPORT_REQUEST,XID(GM_TYPE_NPC, npc_id),_parent->ID,_parent->pos,GetWorldTag(), GetTeamID()); 
	gmatrix::SendMessage(msg);
	return true;

}

void gplayer_imp::HandleSummonTeleport(A3DVECTOR & pos, int type)
{
	LongJump(pos);
	if(type == 1)
	{
		SetCoolDown(COOLDOWN_INDEX_TELEPORT1, 20000);
	}
	else if(type == 2)
	{
		SetCoolDown(COOLDOWN_INDEX_TELEPORT2, 20000);
	}
}

void gplayer_imp::CurseTeleport(const XID & who)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_TELEPORT2))
	{
		return;
	}

	MSG msg;
	BuildMessage(msg,GM_MSG_SUMMON_TRY_TELEPORT,who,_parent->ID,_parent->pos,GetWorldTag());
	gmatrix::SendMessage(msg);
}

void gplayer_imp::PlayerMobActive(const C2S::CMD::mobactive_start & ma)
{
	_mobactiveman.PlayerMobActive(this,ma);
}

void gplayer_imp::PlayerMobActiveCancel()
{
	if (_mobactiveman.IsActive())
	{
		_mobactiveman.PlayerMobActiveCancel(this);
	}
}

void gplayer_imp::PlayerMobActiveFinish()
{
	if (_mobactiveman.IsActive())
	{
		_mobactiveman.PlayerMobActiveFinish(this);
	}
}

bool 
gplayer_imp::CheckPlayerMobActiveRequest()
{
	//检查是否能够发起请求
	//条件是,只能是normal状态
	//没有当前操作
	if(!_pstate.IsNormalState()) return false;
	if(_cur_session || HasNextSession()) return false;
	//TODO
	if(GetShape()) return false;
	if(IsTransformState()) return false;
	gplayer * pPlayer = GetParent();
	if(pPlayer->IsInvisible()) return false;
	if(pPlayer->mobactive_id)  return false;

	return true;
}
void gplayer_imp::SetPuppetForm(bool on, int num)
{
	gplayer* pPlayer = GetParent();
	if(on)
	{
		pPlayer->SetExtraState(gplayer::STATE_PUPPET);
		pPlayer->puppet_id = num;
		ChangeShape(6);
		_runner->puppet_form(on, _parent->ID.id, num);
	}
	else
	{
		pPlayer->ClrExtraState(gplayer::STATE_PUPPET);
		pPlayer->puppet_id = 0;
		ChangeShape(0);
		_runner->puppet_form(on, _parent->ID.id, num);

		if(_filters.IsFilterExist(FILTER_PUPPETSKILL))
		{
			_filters.RemoveFilter(FILTER_PUPPETSKILL);
		}
	}
}

void gplayer_imp::SetPuppetSkill(int skill_id, int immune_prob)
{
	_puppet_immune_prob = immune_prob;
	_puppet_skill_id = skill_id;
}


//弹飞
void gplayer_imp::BounceTo(const  A3DVECTOR & target_pos, float distance)
{
	A3DVECTOR dest_pos = _parent->pos;
	dest_pos -= target_pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= distance/sqrt(sq);
	dest_pos += _parent->pos;

	TryCharge(3, dest_pos, 0);
}

//吸引
void gplayer_imp::DrawTo(const XID & target, const A3DVECTOR & destPos)
{
	TryCharge(4, destPos, 0);
}

//击退
void gplayer_imp::Repel2(const  A3DVECTOR & target_pos, float distance)
{
	A3DVECTOR dest_pos = _parent->pos;
	dest_pos -= target_pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= distance/sqrt(sq);
	dest_pos += _parent->pos;

	TryCharge(5, dest_pos, 0);
}

//背摔
void gplayer_imp::Throwback(const A3DVECTOR & target_pos, float dist)
{
	A3DVECTOR dest_pos = target_pos;
	dest_pos -= _parent->pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= dist/sqrt(sq);
	dest_pos += target_pos; 

	TryCharge(6, dest_pos, 0);
}

void gplayer_imp::TryCharge(int type, const A3DVECTOR & dest_pos, int target_id)
{
	if(_filters.IsFilterExist(FILTER_INDEX_TRY_CHARGE)) return;

	_runner->object_try_charge(type, dest_pos, target_id);
	_filters.AddFilter(new charge_filter(this,FILTER_INDEX_TRY_CHARGE, 3)); 
}

void 
gplayer_imp::PlayerChargeTo(int type, int target_id, const A3DVECTOR & pos)
{
	if(!_filters.IsFilterExist(FILTER_INDEX_TRY_CHARGE)) return;
	_filters.RemoveFilter(FILTER_INDEX_TRY_CHARGE);
	if(_parent->pos.squared_distance(pos) > 60 * 60) return;

	gplayer *pPlayer= GetParent();
	A3DVECTOR offset = pos; 
	offset -= pPlayer->pos;
	StepMove(offset);
	_runner->object_be_charge_to(type, target_id, pos);
}

void gplayer_imp::SetQilinForm(bool on)
{
	gplayer* pPlayer = GetParent();
	if(on)
	{
		pPlayer->SetExtraState(gplayer::STATE_QILIN);
	}
	else
	{
		pPlayer->ClrExtraState(gplayer::STATE_QILIN);
	}
}

void gplayer_imp::SetRage(int v)
{
	if(!IsRenMa()) return;
	_basic.rage = v;
	SetRefreshState();
}

void gplayer_imp::PlayerCreateCrossVrTeam(int name_len, char team_name[])
{
	if(name_len <= 0 || name_len > 50) return;

	object_interface oif(this);
	if(!GNET::SendCreateCrossVr(_parent->ID.id, name_len, team_name, oif))
	{
		return;
	}
	GLog::log(GLOG_INFO,"用户%d创建跨服战队", _parent->ID.id);
}


bool gplayer_imp::PlayerGetKingReward()
{
	if(!IsKingdomKing()) return false;
	if(!_pstate.IsNormalState() && !_pstate.IsBindState())return false;
	if(!_pstate.CanTrade() || _cur_session || _session_list.size()) return false;

	//检查包裹是否足够
	if(!_inventory.GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	GLog::log(GLOG_INFO,"国王%d发起领取国王战连任奖励请求", _parent->ID.id);
	return GNET::SendKingGetReward(_parent->ID.id, object_interface(this)); 
}

bool gplayer_imp::KingGetRewardResponse(int retcode)
{
	TradeUnLockPlayer();
	if(retcode != 0) return false;
	if(!IsKingdomKing()) return false;

	//检查包裹是否足够
	if(!_inventory.GetEmptySlotCount())
	{
		return false;
	}

	DeliverItem(54908, 1, false, 0, ITEM_INIT_TYPE_TASK);
	GLog::log(GLOG_INFO,"国王%d领取了国王战连任奖励", _parent->ID.id);
	return true;
}

void gplayer_imp::CastCycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage,
		int mpcost, int hpcost,int dpcost, int inkcost, char force, float ratio, int plus, int period, int var[16], int crit_rate) 

{
	class CycleSkill : public abase::timer_task
	{
		int _skill_id;
		int _skill_level;
		char _skill_type;
		float _radius;
		int _coverage;
		int _mpcost;
		int _hpcost;
		int _dpcost;
		int _inkcost;
		char _force;
		float _ratio;
		int _plus;
		int _period;
		int _roleid;
		int _var[16];
		int _crit_rate;

	public:
		CycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage,
				int mpcost, int hpcost,int dpcost, int inkcost, char force, float ratio, int plus, int period, int roleid, int var[16], int crit_rate)
		{
			_skill_id = skill_id;
			_skill_level = skill_level;
			_skill_type = skill_type;
			_radius = radius;
			_coverage = coverage;
			_mpcost = mpcost;
			_hpcost = hpcost;
			_dpcost = dpcost;                                                                                                                                                               
			_inkcost = inkcost;                                                                                                                                                             
			_force = force;
			_ratio = ratio;                                                                                                                                                                 
			_plus = plus;                                                                                                                                                                   
			_period = period;                                                                                                                                                               
			_roleid = roleid;
			memcpy(_var, var, sizeof(_var));
			_crit_rate = crit_rate;
		}	

		void OnTimer(int index, int rtimes, bool & is_delete)
		{
			if(!TryCast())
			{
				RemoveSelf();
				is_delete = true;
				delete this;
				return;
			
			}

			if(rtimes == 0)
			{
				RemoveSelf();
				is_delete = true;
				delete this;
				return;
			}	
		}

		void Run()
		{
			SetTimer(g_timer, 10, _period*1000/500);
		}

		bool TryCast()
		{
			int index = gmatrix::FindPlayer(_roleid);
			if(index >= 0)
			{
				gplayer * pPlayer = gmatrix::GetPlayerByIndex(index);
				spin_autolock keeper(pPlayer->spinlock);
				if(pPlayer->IsActived() && pPlayer->imp && pPlayer->ID.id == _roleid
						&& pPlayer->login_state >= gplayer::LOGIN_OK)
				{
					gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
					if(!pImp->_filters.IsFilterExist(FILTER_CYCLESKILL)) return false;

					return pImp->TryCastCycleSkill(_skill_id, _skill_level, _skill_type, _radius, _coverage, _mpcost,
						       _hpcost, _dpcost, _inkcost, _ratio, _plus, _force, _var, _crit_rate);	
				}
			}
			return false;
		}
	};	


	(new CycleSkill(skill_id, skill_level, skill_type, radius, coverage, mpcost, hpcost, dpcost, inkcost, force, ratio, plus, period, _parent->ID.id, var, crit_rate))->Run();
}


bool gplayer_imp::TryCastCycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage,
		int mpcost, int hpcost,int dpcost, int inkcost, float ratio, int plus, char force, int var[16], int crit_rate)

{
	if(IsRenMa())
	{
		hpcost += mpcost;
		mpcost = 0;
	}
	if(GetMP() < mpcost) return false;
	if(GetHP() <= hpcost) return false;
	if(GetDP() < dpcost) return false;
	if(GetInk() < inkcost) return false;
	

	if(hpcost > 0) DecHP(hpcost);
	if(mpcost > 0) DrainMana(mpcost);
	if(dpcost > 0) DecDeity(dpcost);
	if(inkcost > 0) DecInk(inkcost);
	
	object_interface oif(this);
	if(ratio > 0.001 || plus > 0)	
	{
		attack_msg   msg;                                                                                                                                                       
		memset(&msg,0,sizeof(msg));                                                                                                                                             
		msg.damage_low   = (int)(ratio * oif.GetExtendProp().damage_low + plus);                                                                                          
		msg.damage_high  = (int)(ratio * oif.GetExtendProp().damage_high + plus);                                                                                         
		msg.force_attack = force;                                                                                                                                              
		msg.skill_id     = skill_id;                                                                                                                                                
		msg.attack_rate  = oif.GetExtendProp().attack;                                                                                                                      
		msg.attached_skill.skill = skill_id;                                                                                                                                        
		msg.attached_skill.level = skill_level;                                                                                                                                     
		memcpy(msg.skill_var, var, sizeof(msg.skill_var));
		oif.FillAttackMsg(XID(-1,-1), msg, 0);                                                                                                                              
		msg.crit_rate = crit_rate + _cur_prop.crit_rate;
		oif.GetSkillWrapper().SetSkillTalent(skill_id, msg.skill_modify, oif);                                                                                              
		std::vector<exclude_target> empty;                                                                                                                                      
		oif.RegionAttack1(oif.GetPos(), radius, msg, 0, coverage, empty);
	}
	else 
	{
		enchant_msg     msg;                                                                                                                                                    
		memset(&msg,0,sizeof(msg));                                                                                                                                             

		msg.skill        = skill_id;                                                                                                                                                
		msg.skill_level  = skill_level;                                                                                                                                             
		msg.force_attack = force;                                                                                                                                              
		msg.helpful      = 0;                                                                                                                                                   
		msg.attack_range = radius;                                                                                                                                             
		msg.skill_reserved1 =  (int)oif.GetExtendProp().damage_high;                                                                                                        
		memcpy(msg.skill_var, var, sizeof(msg.skill_var));
		oif.GetSkillWrapper().SetSkillTalent(skill_id, msg.skill_modify, oif);                                                                                              
		std::vector<exclude_target> empty;                                                                                                                                      
		oif.RegionEnchant1(oif.GetPos(), radius, msg, coverage, empty);  
	}
	return true;
}

void 
gplayer_imp::PlayerQilinInvite(const XID & target)
{
	_link_qilin_player.PlayerLinkInvite(this,target);
}


void 
gplayer_imp::PlayerQilinInviteReply(const XID & target, int param)
{
	_link_qilin_player.PlayerLinkInvReply(this,target,param);
}

void
gplayer_imp::PlayerQilinCancel()
{
	if(!_pstate.IsBindState())
	{
		//这应该是不可能的
		return;
	}
	_link_qilin_player.PlayerLinkCancel(this);
}

void
gplayer_imp::PlayerQilinDisconnect()
{
	gplayer *pParent = GetParent();
	if(pParent->bind_type != LINK_QILIN_MEMBER_TYPE) return;

	_link_qilin_player.PlayerLinkDisconnect(this);
}

void
gplayer_imp::PlayerQilinReconnect()
{
	gplayer *pParent = GetParent();
	if(pParent->bind_type != LINK_QILIN_MEMBER_TYPE) return;

	_link_qilin_player.PlayerLinkReconnect(this);
}

void
gplayer_imp::SetPhase(int id, bool on)
{
	abase::hash_map<int, bool>::iterator iter;
	iter = _phase_info.find(id);
	if(iter != _phase_info.end())
	{
		iter->second = on;
	
	}
	else
	{
		_phase_info[id] = on;
	}
	_runner->phase_info_change(id, on);
}

void
gplayer_imp::SavePhaseInfo(archive & ar)
{
	if(_phase_info.size() == 0) return;

	abase::vector<char> open_masks;
	abase::vector<char> close_masks;
	for(abase::hash_map<int, bool>::iterator iter = _phase_info.begin(); iter != _phase_info.end(); ++iter)
	{
		int phase_id = iter->first;
		bool phase_on = iter->second;
		char mask_value = 1 << ((phase_id-1)%8) ; 
		size_t mask_len = (phase_id-1) / 8 + 1;
		size_t cur_len;

		if(phase_on)
			cur_len = open_masks.size();
		else
			cur_len = close_masks.size();

		if(mask_len > cur_len)
		{
			for(size_t i = cur_len; i <= mask_len; ++i)
			{
				if(phase_on)
					open_masks.push_back(0);
				else
					close_masks.push_back(0);
				 
			}
		}

		if(phase_on)
		{
			open_masks[mask_len-1] |= mask_value;
		}
		else
		{
			close_masks[mask_len-1] |= mask_value;
		}
	}

	ar << open_masks.size();
	for(abase::vector<char>::iterator iter = open_masks.begin(); iter != open_masks.end(); iter++)
	{
		ar << *iter;
	}

	ar << close_masks.size();
	for(abase::vector<char>::iterator iter = close_masks.begin(); iter != close_masks.end(); iter++)
	{
		ar << *iter;
	}
	

}

void
gplayer_imp::LoadPhaseInfo(archive & ar)
{
	if(ar.size() == 0) return;

	size_t open_len;
	ar >> open_len;
	for(size_t i = 0; i < open_len; i++)
	{
		char mask;
		ar >> mask;

		for(size_t j = 0; j < 8; ++j)
		{
			char cur_bit = 1<< j;
			if(mask & cur_bit)
			{
				int phase_id = 8 * i + j + 1;
				_phase_info[phase_id] = true;
			}
		}
	}

	size_t close_len;
	ar >> close_len;
	for(size_t i = 0; i < close_len; i++)
	{
		char mask;
		ar >> mask;

		for(size_t j = 0; j < 8; ++j)
		{
			char cur_bit = 1<< j;
			if(mask & cur_bit)
			{
				int phase_id = 8 * i + j + 1;
				_phase_info[phase_id] = false;
			}
		}
	}

}


void 
gplayer_imp::SavePhaseInfoForClient(archive & ar)
{
	if(_phase_info.size() > 0)
	{
		ar << _phase_info.size();
		abase::hash_map<int, bool>::iterator iter;
		for(iter = _phase_info.begin(); iter != _phase_info.end(); ++iter)
		{
			ar << iter->first << iter->second;
		}
	}
}

void 
gplayer_imp::DeliverFBaseCMD(const void * buf, size_t size)
{
	GMSV::DeliverFacBaseCMD(_parent->ID.id, OI_GetMafiaID(), buf, size);
}

bool
gplayer_imp::PlayerRebornPet(size_t pet_index, size_t assist_index)
{
	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return false;
	}

	class op : public session_general_operation::operation
	{
	public:
		op(size_t pet_index, size_t assist_index) : _pet_index(pet_index), _assist_index(assist_index)
		{}

		virtual int GetID()
		{
			return S2C::GOP_PET_REBORN;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(3);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
		}

		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			pImp->RebornPet(_pet_index, _assist_index);
		}

		virtual bool OnAttacked(gactive_imp * obj)
		{	
			return true;
		}

	private:
		size_t _pet_index;
		size_t _assist_index;
	};

	AddSession(new session_general_operation(this, new op(pet_index, assist_index)));
	StartSession();
	return true;
}

bool
gplayer_imp::RebornPet(size_t pet_index, size_t assist_index)
{
	if(GetRebornCount() <=0) return false;
	if(pet_index >= _inventory.Size() || assist_index >= _inventory.Size()) return false;

	item & it = _inventory[pet_index];
	if(it.type ==-1 || it.body == NULL)  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE) return false;
	if(it.IsLocked()) return false;

	item & it2 = _inventory[assist_index];
	if(it2.type != 58852) return false; 

	
	bool rst = ((item_pet_bedge*)it.body)->RebornPet(this, pet_index);

	if(rst)
	{
		TakeOutItem(assist_index, it2.type, 1); 
		return true;
	}
	return false;
}

bool
gplayer_imp::PlayerChangePetShape(size_t pet_index, int shape_id)
{
	if(pet_index >= _pet_bedge_inventory.Size()) return false;
	if(shape_id <= 0 || shape_id > 4) return false;

	item & it = _pet_bedge_inventory[pet_index];
	if(it.type ==-1 || it.body == NULL)  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE) return false;

	 return ((item_pet_bedge*)it.body)->ChangeShape(this, pet_index, shape_id);
}


bool
gplayer_imp::PlayerRefinePetAttr(size_t pet_index, size_t attr_type, size_t assist_id, size_t assist_count)
{
	if(pet_index >= _inventory.Size()) return false;
	if(attr_type < 0 || attr_type > 14) return false;
	if(assist_id != 18794) return false;	//仙豆
	if(assist_count != 1 && assist_count != 10 && assist_count != 100) return false; //只能用1个,10个,100个
	if(_inventory.CountItemByID(assist_id) < (int) assist_count) return false;

	item & it = _inventory[pet_index];
	if(it.type ==-1 || it.body == NULL)  return false;
	if(it.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE) return false;

	bool rst = ((item_pet_bedge*)it.body)->RefineAttr(this, pet_index, attr_type, assist_id, assist_count);
	if(rst)
	{
		TakeOutItem(assist_id, assist_count);
		return true;
	}
	return false;
}


bool
gplayer_imp::CheckPlayerActiveEmoteInvite()
{
	//检查是否能够发起请求
	//条件是,只能是normal状态
	//没有当前操作
	if(!_pstate.IsNormalState()) return false;
	if(_cur_session || HasNextSession()) return false;
	if(GetShape()) return false;
	if (IsCombatState()) return false;
	if(IsTransformState()) return false;
	gplayer * pPlayer = GetParent();
	if(pPlayer->IsInvisible()) return false;
	if(pPlayer->IsMountMode()) return false;
	if(pPlayer->IsFlyMode()) return false;

	return true;
}

void
gplayer_imp::PlayerActiveEmoteInvite(const XID & target, int type)
{
	_active_emote_player.PlayerActiveEmoteInvite(this,target,type);
}

void
gplayer_imp::PlayerActiveEmoteInviteReply(const XID & target, int type, int param)
{
	_active_emote_player.PlayerActiveEmoteInvReply(this,target,type,param);
}

void
gplayer_imp::PlayerActiveEmoteCancel()
{
	_active_emote_player.PlayerActiveEmoteCancel(this);
}

int
gplayer_imp::GetPuppetid()
{
	gplayer *pPlayer = GetParent();
	return pPlayer->puppet_id;
}


void 
gplayer_imp::DeliverFBaseGrass(int num)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBasePropChange(_parent->ID.id, OI_GetMafiaID(), FBASE_PROP_GRASS, num);
}

void 
gplayer_imp::DeliverFBaseMine(int num)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBasePropChange(_parent->ID.id, OI_GetMafiaID(), FBASE_PROP_MINE, num);
}

void 
gplayer_imp::DeliverFBaseMonsterFood(int num)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBasePropChange(_parent->ID.id, OI_GetMafiaID(), FBASE_PROP_MONSTERFOOD, num);
}

void 
gplayer_imp::DeliverFBaseMonsterCore(int num)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBasePropChange(_parent->ID.id, OI_GetMafiaID(), FBASE_PROP_MONSTERCORE, num);
}

void 
gplayer_imp::DeliverFBaseCash(int num)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBasePropChange(_parent->ID.id, OI_GetMafiaID(), FBASE_PROP_CASH, num);
}

void
gplayer_imp::PlayerTryGetCS6V6Award(int type, int level)
{
	GMSV::SendGetCS6V6Info(_parent->ID.id, type, level);
}

bool
gplayer_imp::PlayerExchangeCS6V6Award(int index, int item_id, int currency_id)
{
	if(index < 0 || index >= 15) return false;
	if(item_id <= 0 || currency_id <= 0) return false;

	int exchange_item_id;
	int exchange_item_num;
	int require_item_id;
	int require_item_count;
	int expire_time;
	int exchange_max_count;
	if(!player_template::GetCS6V6ExchangeItemInfo(index, exchange_item_id, exchange_item_num, require_item_id, require_item_count, expire_time, exchange_max_count))
	{
		return false;
	}

	if(item_id != exchange_item_id || currency_id != require_item_id) 
	{
		return false;
	}

	if(_inventory.CountItemByID(require_item_id) < require_item_count) return false;

	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(exchange_item_id);
	size_t need_slot_count = exchange_item_num/ pile_limit + ((exchange_item_num % pile_limit == 0) ? 0 : 1); 
	if(_inventory.GetEmptySlotCount() < need_slot_count) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}


	//兑换有次数限制
	if(exchange_max_count > 0)
	{
		abase::hash_map<int,int>::iterator iter;
		if( (iter= _exchange_cs6v6_award_info.find(exchange_item_id)) != _exchange_cs6v6_award_info.end())
		{
			int cur_count = iter->second;
			//达到最大次数
			if(cur_count >= exchange_max_count) return false;

			_exchange_cs6v6_award_info[exchange_item_id]++ ;
		}
		else
		{
			_exchange_cs6v6_award_info[exchange_item_id] = 1;
		}
		SendCollisionRaidInfo();
	}
	_collision_info.cs_exchange_item_timestamp = g_timer.get_systime();

	GainItem(exchange_item_id, exchange_item_num, true, expire_time, ITEM_INIT_TYPE_TASK);
	TakeOutItem(require_item_id, require_item_count);
	GLog::log(GLOG_INFO, "用户%d兑换了跨服6V6奖励, item_id=%d, item_num=%d, currency_id=%d, currency_num=%d", 
			_parent->ID.id, exchange_item_id, exchange_item_num, require_item_id, require_item_count);

	return true;
}

bool
gplayer_imp::PlayerExchangeCS6V6Money(int type, int count)
{
	if(type < 0 || type > 2) return false;
	if(count < 0 || count > 200000) return false;

	int item_id = 0;
	int output_item_id = 0;
	int output_item_count = 0;
	if(type == 0){
		item_id = 60460;
		output_item_id = 60461;
		output_item_count = count * 10;
	}
	else if(type == 1){
		item_id = 60460;
		output_item_id = 60462;
		output_item_count = count * 100;
	}
	else if(type == 2){
		item_id = 60461;
		output_item_id = 60462;
		output_item_count = count * 10;
	}

	if(item_id == 0 || output_item_id == 0 || output_item_count == 0) return false;
	if(_inventory.CountItemByID(item_id) < count) return false;

	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(output_item_id);
	size_t need_slot_count = output_item_count / pile_limit + ((output_item_count  % pile_limit == 0) ? 0 : 1); 

	if(_inventory.GetEmptySlotCount() < need_slot_count) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}


	GainItem(output_item_id, output_item_count, false, 0, ITEM_INIT_TYPE_TASK); 
	TakeOutItem(item_id, count);

	return true;
}	

void 
gplayer_imp::PlayerChangePuppetForm()
{
	if(!CheckCoolDown(COOLDOWN_INDEX_CHANGE_PUPPET_FORM))
	{
		return;
	}
	SetCoolDown(COOLDOWN_INDEX_CHANGE_PUPPET_FORM, 50000);

	_runner->player_change_puppet_form();
}


bool
gplayer_imp::PlayerGetCS6V6Award(int type, int level, int score, int team_score, int team_rank, int award_timestamp)
{
	if(type != 0 && type != 1 && type != 2) return false;

	int cur_time = g_timer.get_systime();
	//每周分档奖励
	if(type == 0)
	{
		if(_collision_info.cs_personal_award_timestamp >= award_timestamp) 
		{
			_runner->error_message(S2C::ERR_CS6V6_CANNOT_GET_AWARD);
			return false;
		}

		int max_point;
		int min_point;
		if(!player_template::GetCS6V6PersonalLevelAwardPoint(level, max_point, min_point))
		{
			return false;
		}
		if(score < min_point) return false;

		struct award_item_info
		{
			int item_id;
			int item_num;
			int expire_time;
		};

		award_item_info award_item[5];
		memset(&award_item, 0, sizeof(award_item));

		if(! player_template::GetCS6V6PersonalLevelAwardItem(level, &award_item))
		{
			return false;
		}

		size_t need_slot = 0;
		for(int i = 0; i < 5; ++i)
		{
			if(award_item[i].item_id > 0 && award_item[i].item_num > 0)
			{
				size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(award_item[i].item_id);
				size_t need_slot_count = award_item[i].item_num / pile_limit + ((award_item[i].item_num  % pile_limit == 0) ? 0 : 1); 
				need_slot += need_slot_count;
			}

		}
		if(need_slot > _inventory.GetEmptySlotCount()) 
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}

		for(int i = 0; i < 5; ++i)
		{
			if(award_item[i].item_id > 0 && award_item[i].item_num > 0)
			{
				GainItem(award_item[i].item_id, award_item[i].item_num, true, award_item[i].expire_time, ITEM_INIT_TYPE_TASK); 
			}
		}

		_collision_info.cs_personal_award_timestamp = cur_time;
		SendCollisionRaidInfo();
		GLog::log(GLOG_INFO,"用户%d成功领取了跨服6V6奖励, time=%d, type=%d, level=%d, score=%d, team_score=%d, team_rank=%d", 
				_parent->ID.id, cur_time, type, level, score, team_score, team_rank);
		return true;
	}
	//每周兑换币奖励
	else if(type == 1)
	{
		if(_collision_info.cs_weekly_exchange_award_timestamp >= award_timestamp) 
		{
			_runner->error_message(S2C::ERR_CS6V6_CANNOT_GET_AWARD);
			return false;
		}

		int item_id = 0;
		int item_num = 0;
		int expire_time = 0;
		if(!player_template::GetCS6V6ExchangeAwardItem(team_rank, item_id, item_num, expire_time)) 
		{
			return false;
		}

		if(item_id > 0 && item_num > 0)
		{
			size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(item_id);
			size_t need_slot_count = item_num / pile_limit + (item_num  % pile_limit == 0) ? 0 : 1; 

			if(need_slot_count > _inventory.GetEmptySlotCount()) 
			{
				_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
				return false;
			}

			GainItem(item_id, item_num, true, expire_time, ITEM_INIT_TYPE_TASK); 

			_collision_info.cs_weekly_exchange_award_timestamp= cur_time;
			SendCollisionRaidInfo();
			GLog::log(GLOG_INFO,"用户%d成功领取了跨服6V6奖励, time=%d, type=%d, level=%d, score=%d, team_score=%d, team_rank=%d", 
					_parent->ID.id, cur_time, type, level, score, team_score, team_rank);
		}
		return true;
	}
	//战队赛季奖励
	else if(type == 2)
	{
		if(IsSameSeason(_collision_info.cs_team_season_award_timestamp, cur_time))
		{
			_runner->error_message(S2C::ERR_CS6V6_CANNOT_GET_AWARD);
			return false;
		}

		struct tm tm1; 
		localtime_r((time_t*)&cur_time, &tm1);
		if((tm1.tm_mon+1) % 3 != 0) return false;
		if(tm1.tm_mday <= 25) return false;

		struct award_item_info
		{
			int item_id;
			int item_num;
			int expire_time;
		};

		award_item_info award_item[6];
		memset(&award_item, 0, sizeof(award_item));
		if(! player_template::GetCS6V6TeamAwardItem(team_rank, &award_item))
		{
			return false;
		}

		size_t need_slot = 0;
		for(int i = 0; i < 6; ++i)
		{
			if(award_item[i].item_id > 0 && award_item[i].item_num > 0)
			{
				size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(award_item[i].item_id);
				size_t need_slot_count = award_item[i].item_num / pile_limit + ((award_item[i].item_num  % pile_limit == 0) ? 0 : 1); 
				need_slot += need_slot_count;
			}

		}

		if(need_slot > _inventory.GetEmptySlotCount()) 
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}

		for(int i = 0; i < 6; ++i)
		{
			if(award_item[i].item_id > 0 && award_item[i].item_num > 0)
			{
				GainItem(award_item[i].item_id, award_item[i].item_num, true, award_item[i].expire_time, ITEM_INIT_TYPE_TASK); 
			}
		}

		_collision_info.cs_team_season_award_timestamp = cur_time;
		SendCollisionRaidInfo();
		GLog::log(GLOG_INFO,"用户%d成功领取了跨服6V6奖励, time=%d, type=%d, level=%d, score=%d, team_score=%d, team_rank=%d", 
				_parent->ID.id, cur_time, type, level, score, team_score, team_rank);
		return true;
	}
	return false;
}

void 
gplayer_imp::OnFBaseBuildingProgress(unsigned long task_id, int value)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBaseBuildingProgress(_parent->ID.id, OI_GetMafiaID(), task_id, value);
}

void
gplayer_imp::DeliveryFBaseAuction(int itemid)
{
	if (OI_GetMafiaID() <= 0)
		return;
	GMSV::SendFacBasePutAuction(_parent->ID.id, _username, _username_len, OI_GetMafiaID(), itemid);
}

void 
gplayer_imp::NotifyBloodPoolStatus(bool on, int cur_hp, int max_hp)
{
	_runner->notify_bloodpool_status(on, cur_hp, max_hp);
}

int gplayer_imp::GetMobActiveTid()
{
	int npc_tid = 0;
	int npcid = GetParent()->mobactive_id;
	if (npcid == 0) return npc_tid;
	world::object_info info;
	bool rst = _plane->QueryObject(XID(GM_TYPE_NPC,npcid),info);
	if (rst)
	{
		npc_tid = info.tid;
	}
	return npc_tid;
}

bool
gplayer_imp::SetActiveEmoteState(const XID & target, int active_emote_type, const char type)
{
	if (!_pstate.IsNormalState()) return false;

	gplayer *pParent = GetParent();
	pParent->active_emote_id = target.id;
	pParent->active_emote_type = active_emote_type;
	pParent->active_emote_first_player = type;
	pParent->SetExtraState(gplayer::STATE_ACTIVE_EMOTE);
	session_active_emote *pSession = new session_active_emote(this);
	AddStartSession(pSession);
	return true;
}

void
gplayer_imp::ClrActiveEmoteState()
{
	gplayer *pParent = GetParent();
	pParent->active_emote_id = 0;
	pParent->active_emote_type = 0;
	pParent->active_emote_first_player = 0;
	pParent->ClrExtraState(gplayer::STATE_ACTIVE_EMOTE);
}

void
gplayer_imp::StartTravelAround(int travel_vehicle_id, float travel_speed, int travel_path_id, int travel_max_time)
{
	if(_filters.IsFilterExist(FILTER_INDEX_TRAVEL_AROUND)) return;

	if(_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER)) 
	{
		_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER); 
	}
	
	if(_filters.IsFilterExist(FILTER_INDEX_AIRCRAFT))
	{
		_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER); 
		NotifyMasterInfo();
	}

	if(_filters.IsFilterExist(FILTER_PUPPETFORM))
	{
		_filters.RemoveFilter(FILTER_PUPPETFORM); 
	}

	if(_filters.IsFilterExist(FILTER_QILINFORM))
	{
		_filters.RemoveFilter(FILTER_QILINFORM); 
	}

	if(_filters.IsFilterExist(FILTER_FOXSTATE))
	{
		_filters.RemoveFilter(FILTER_FOXSTATE); 
	}

	if(_filters.IsFilterExist(FILTER_GHOSTFORM))
	{
		_filters.RemoveFilter(FILTER_GHOSTFORM); 
	}

	if(_filters.IsFilterExist(FILTER_INSANITYFORM))
	{
		_filters.RemoveFilter(FILTER_INSANITYFORM); 
	}

	if(_filters.IsFilterExist(FILTER_FOGSTATE))
	{
		_filters.RemoveFilter(FILTER_FOGSTATE); 
	}

	if(_filters.IsFilterExist(FILTER_YUANLING))
	{
		_filters.RemoveFilter(FILTER_YUANLING); 
	}

	if(IsTransformState()) 
	{
		PlayerStopTransform();
	}

	_filters.AddFilter(new travel_filter(this,FILTER_INDEX_TRAVEL_AROUND, travel_vehicle_id, travel_speed, travel_path_id, travel_max_time, GetWorldTag()));
}

void
gplayer_imp::PlayerStopTravelAround()
{
	if(!_filters.IsFilterExist(FILTER_INDEX_TRAVEL_AROUND)) return;
	_filters.RemoveFilter(FILTER_INDEX_TRAVEL_AROUND);
}

XID
gplayer_imp::GetQilinLeaderID()
{
	gplayer *pPlayer = GetParent();
	if (pPlayer->bind_type != LINK_QILIN_LEADER_TYPE)
		return XID(-1,-1);
	return XID(GM_TYPE_PLAYER,pPlayer->bind_target);
}

void gplayer_imp::SendHideAndSeekRaidInfo()
{
	int cur_time = g_timer.get_systime();
	int day_has_award = !IsSameDay(cur_time,_hide_and_seek_info.daily_award_timestamp) 
		  && IsSameDay(cur_time,_hide_and_seek_info.hide_and_seek_timestamp) && (_hide_and_seek_info.score >= 200); 
	if (IsSameDay(cur_time,_hide_and_seek_info.daily_award_timestamp)) day_has_award = 2;
	_runner->hide_and_seek_raid_info(_hide_and_seek_info.hide_and_seek_timestamp,
			day_has_award);
}

void gplayer_imp::LoadHideAndSeekData(archive & ar)
{
	if (ar.size() == 0) return;

	ar >> _hide_and_seek_info.hide_and_seek_timestamp
	   >> _hide_and_seek_info.daily_award_timestamp  >> _hide_and_seek_info.score;

	for (size_t i = 0; i < 4; ++i)
	{
		ar >> _hide_and_seek_info.reservered[i];
	}
}

void gplayer_imp::SaveHideAndSeekData(archive & ar)
{
	if (_hide_and_seek_info.hide_and_seek_timestamp == 0 
		&& _hide_and_seek_info.daily_award_timestamp == 0
		&& _hide_and_seek_info.score == 0) return;

	ar << _hide_and_seek_info.hide_and_seek_timestamp 
	   << _hide_and_seek_info.daily_award_timestamp << _hide_and_seek_info.score;

	for (size_t i = 0; i < 4; ++i)
	{
		ar << _hide_and_seek_info.reservered[i];
	}
}

bool
gplayer_imp::PlayerGetHideAndSeekAward()
{
	if (_inventory.IsFull()) return false;

	int cur_time = g_timer.get_systime();

	if (IsSameDay(cur_time,_hide_and_seek_info.daily_award_timestamp)) return false;
	if (!IsSameDay(cur_time,_hide_and_seek_info.hide_and_seek_timestamp)) return false;

	unsigned int item_id = 62922;
	if (item_id > 0)
	{
		DeliverItem(item_id,1,false,0,ITEM_INIT_TYPE_TASK);
		_hide_and_seek_info.daily_award_timestamp = cur_time;
	}
	SendHideAndSeekRaidInfo();
	GLog::log(GLOG_INFO,"hideandseek:getaward:roleid=%d",GetParent()->ID.id);

	return true;
}

bool
gplayer_imp::PlayerBuySeekerSkill()
{
	return GetWorldManager()->PlayerBuySeekerSkill(this); 
}

void
gplayer_imp::PlayerApplyHideAndSeekRaid(int raid_id, int raid_template_id, bool is_team)
{
	if(raid_id != HIDE_AND_SEEK_RAID_ID) return;
	if(is_team && (!IsInTeam() || !IsTeamLeader())) return;
	if(!GetWorldManager()->IsGlobalWorld()) return; 
	
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL || raid_config.map_id != raid_id)
	{
		printf("无效的副本模板: %d\n", raid_template_id);
		return;
	}	

	if(!CheckEnterRaid(raid_id, raid_template_id, RT_SEEK, NULL, 0))
	{
		printf("不符合进入条件\n");
		return;
	}

	std::vector<int> apply_vec;
	world::object_info info;

	if(is_team)
	{
		int count = _team.GetMemberNum();
		for(int i = 0; i < count; ++i)
		{
			const player_team::member_entry &ent = _team.GetMember(i);
			bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER,ent.id.id),info,true);
			if (!rst || info.level < 90) return;
			apply_vec.push_back(ent.id.id);
		}
	}
	else
	{
		int player_id = _parent->ID.id;
		bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER,player_id),info);
		if (!rst || info.level < 90) return;
		apply_vec.push_back(player_id);
	}

	GMSV::SendApplyHideAndSeekRaid(raid_id, apply_vec);
}

void 
gplayer_imp::TryReconnect()
{
	//只有原服大世界挂机允许断线重入
	if(gmatrix::IsZoneServer() || _parent->b_disconnect)  
	{
		GMSV::SendReconnectRe(_parent->ID.id, 0, -1); 
		return;
	}

	LostConnection(PLAYER_OFF_RECONNECT);
}

void
gplayer_imp::PlayerApplyCaptureRaid(int raid_id, int raid_template_id, bool is_team)
{
	if(raid_id != CAPTURE_RAID_ID) return;
	if(is_team) return;
	if(!GetWorldManager()->IsGlobalWorld()) return; 
	if(!IsZoneServer()) return;
	
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL || raid_config.map_id != raid_id)
	{
		printf("无效的副本模板: %d\n", raid_template_id);
		return;
	}	

	if(!CheckEnterRaid(raid_id, raid_template_id, RT_CAPTURE, NULL, 0))
	{
		printf("不符合进入条件\n");
		return;
	}

	std::vector<int> apply_vec;
	world::object_info info;

	if(is_team)
	{
		int count = _team.GetMemberNum();
		for(int i = 0; i < count; ++i)
		{
			const player_team::member_entry &ent = _team.GetMember(i);
			bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER,ent.id.id),info,true);
			if (!rst || info.level < 155) return;
			apply_vec.push_back(ent.id.id);
		}
	}
	else
	{
		int player_id = _parent->ID.id;
		bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER,player_id),info);
		if (!rst || info.level < 155) return;
		apply_vec.push_back(player_id);
	}

	GMSV::SendApplyCaptureRaid(raid_id, apply_vec);
}
