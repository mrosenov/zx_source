#ifndef __ONLINEGAME_GS_IMP_H__
#define __ONLINEGAME_GS_IMP_H__

#include <octets.h>
#include <ASSERT.h>
#include <common/packetwrapper.h>
#include <vector.h>
#include <set>
#include <common/message.h>
#include "substance.h"
#include "attack.h"
#include <common/protocol.h>
#include <vector>
#include <map>
#include "itemdata.h"

namespace GNET
{
	struct syncdata_t;
	struct SkillWrapper;
}

namespace GDB
{
	struct itemlist;
}

struct item_data;
struct slice;
struct gobject;
class world;
class gobject_imp;
class item;
struct q_extend_prop;
struct instance_key;
class world_manager;
// Youshuang add
struct raid_boss_info;
struct facbase_mall;
// end

class controller :public substance
{
public:
DECLARE_SUBSTANCE(controller);
public:
	gobject_imp * _imp;

public:
	controller():_imp(NULL){}
	virtual void Init(gobject_imp * imp) {_imp = imp;} 
	virtual ~controller(){}

	/*
	 *	控制部分的消息处理函数，由于控制部分有时可能需要截取一些消息
	 *	所以这里也可以对消息进行处理
	 *	能够进行处理的前提是_do_msg的值为True
	 *	对消息处理函数的要求和实现部分是一样的
	 */
	virtual  int MessageHandler(const MSG & msg)
	{
		return 0;
	}

	

	/*
	 *	处理到来的命令
	 */
	virtual  int CommandHandler(int cmd_type,const void * buf, size_t size) = 0;

	/**
	 *	在两个slice间移动，由于可能是不同的对象，所以做成虚函数
	 *	由于controller必然要和正确的gobject绑定，所以在这里面作
	 *	返回非0表示移动失败，一般指对象已经不在src中
	 */
	virtual int MoveBetweenSlice(gobject * obj,slice * src, slice * dest) = 0;

	/**
	 *	释放对象本身，由于只有controller肯定与NPC、Player绑定，所以由这里释放。
	 */
	virtual void Release(bool free_parent = true) = 0;

	/**
	 *	出生	
	 */
	virtual void OnBorn()
	{
		ASSERT(false);
	}

	
	/**
	 *	重生
	 */
	virtual void Reborn()
	{
		ASSERT(false);
	}

	/**
	 	服务器被切换时发生的调用
	 */
	virtual void SwitchSvr(int dest, const A3DVECTOR & oldpos, const A3DVECTOR &newpos,const instance_key * switch_key) { ASSERT(false);}


	virtual void error_cmd(int msg)
	{
	}
	virtual void OnHeartbeat(size_t tick)
	{
	}
	virtual void  NPCSessionStart(int task_id, int session_id)
	{}

	virtual void NPCSessionEnd(int task_id,int session_id, int retcode)
	{}

	virtual void NPCSessionUpdateChaseInfo(int task_id,const void * buf ,size_t size)
	{}

	virtual bool NPCGetNextWaypoint(A3DVECTOR & target)
	{
		return false;
	} 

	virtual int GetNextMoveSeq() { return -1;}
	virtual int GetCurMoveSeq() { return -1;}
	virtual void SetNextMoveSeq(int seq) { return ;}

	virtual bool HasGMPrivilege() { return false;}

	virtual void DenyCmd(size_t cmd_type) {}
	virtual void AllowCmd(size_t cmd_type){}

public:
	enum
	{
		CMD_MOVE,
		CMD_ATTACK,
		CMD_PICKUP,
		CMD_MARKET,
		CMD_PET,
		CMD_SKILL,
		CMD_BOT,
		CMD_FLY,
		CMD_BIND,
		CMD_TRANSFORM,
		CMD_ITEM,
		CMD_FLY_POS,
		CMD_ACTIVE_EMOTE,
		CMD_MAX,
	};
};

class dispatcher :public substance
{
public:
DECLARE_SUBSTANCE(dispatcher);
protected:
	gobject_imp * _imp;
public:
	dispatcher():_imp(NULL){}
	virtual ~dispatcher(){}

	void set_imp(gobject_imp * imp) {_imp = imp;} 
	virtual void init(gobject_imp * imp) {_imp = imp;} 
	virtual void set_invisible(bool invisible = true) {}
	virtual bool is_invisible() {return false;}

	virtual void begin_transfer() = 0;				//开始一次数据传送
	virtual void end_transfer() = 0;				//结束一次数据传送，清空缓冲
	virtual void enter_slice(slice * ,const A3DVECTOR &) = 0;	//进入一个格子，告诉其他人进入，并且取得这个格子的信息
	virtual void leave_slice(slice * ,const A3DVECTOR &) = 0;	//离开一个格子，告诉其他人离开
	virtual void notify_pos(const A3DVECTOR & pos) {};		//通知自己的当前位置，只有player用这个命令
	virtual void get_base_info() = 0;				//发送自己的基础数据给自己，以后可能会更改
	virtual void enter_world() = 0;					//进入世界
	virtual	void leave_world() = 0;					//离开世界
	virtual void appear() { enter_world();}				//显形
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode) = 0;//派发移动指令
	virtual void stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mode){}//派发停止移动指令
	virtual void start_attack(const XID &){}
	//virtual void attack_once(unsigned char dec_amount){}
	virtual void stop_attack(int flag){}
	virtual void dodge_attack(const XID &attacker,int skilll_id, const attacker_info_t &, char at_state,char speed,bool orange) {}
	virtual void be_damaged(const XID & id, int skill_id,const attacker_info_t &,int damage,int dt_damage,int dura_index,char at_state,char speed,bool orange){}
	virtual void be_hurt(const XID & id, const attacker_info_t & ,int damage,bool invader){}
	virtual void equipment_damaged(int index);
	virtual void on_death(const XID & killer,bool reserver,int time = 0) {}
	virtual void error_message(int message) {}
	virtual void disappear(char at_once=0) {}					//发送消失的消息
	virtual void matter_pickup(int id) {}				//发送消失的消息
	virtual void resurrect(int){}
	virtual void pickup_money(int){}
	virtual void pickup_item(int type, int expire_date, int amount,int slot_amount, int where,int index,int state){}
	virtual void purchase_item(int type, size_t money,int amount,int slot_amount, int where,int index){}
	virtual void query_info00(const XID & target, int cs_index,int sid){}
	virtual void query_info00(){}					//返回自己的info00，只有player有效 
	virtual void receive_exp(int64_t exp){}			//接受经验值和技能点
	virtual void receive_bonus_exp(int64_t exp){}			//接受奖励经验
	virtual void level_up(){}					//升级
	virtual void deity_levelup(char success){}					//升级
	virtual void unselect(){}					//没有选中任何人
	virtual void player_select_target(int id){}	
	virtual void self_item_empty_info(int where,int index) {}
	virtual void self_item_info(int where,int index,item_data_client & data,unsigned short crc) {}
        // Youshuang add
	virtual void send_talisman_enchant_addon( int id, int index ){}
        virtual void produce_colorant_item( int index, int color, int quality ){}
        virtual void use_colorant_item( int where, int index, int color, int quality ){}
	virtual void get_achievement_award( unsigned short achiev_id ){}
	virtual void get_raid_boss_info( const std::vector<raid_boss_info>& monsters_been_killed ){}
	virtual void get_facbase_mall_info( facbase_mall* mall_info ){}
	virtual void notify_combine_mine_state( int state ){}
	virtual void get_faction_coupon( int faction_coupon ){}
	virtual void get_facbase_cash_items_info( const std::map<int, int>& item_cooldown){}
	virtual void get_facbase_cash_item_success( int index ){}
	virtual void get_newyear_award_info( int score, const std::vector<char>& status ){}
        // end  
	virtual void self_inventory_data(int where,unsigned char inv_size,const void * data, size_t len) {}
	virtual void self_inventory_detail_data(int where,unsigned char inv_size,const void * data, size_t len) {}
	virtual void exchange_item_list_info(unsigned char where,const void *data,size_t len) {}
	virtual void repurchase_inventory_data(unsigned char inv_size,const void * data, size_t len) {}
	virtual void exchange_inventory_item(size_t idx1,size_t idx2) {}
	virtual void move_inventory_item(size_t src,size_t dest,size_t count) {}
	virtual void player_drop_item(size_t where, size_t index,int type,size_t count,unsigned char drop_type) {}
	virtual void exchange_equipment_item(size_t index1,size_t index2) {}
	virtual void equip_item(size_t index_inv,size_t index_equip,int count_inv,int count_eq) {}
	virtual void move_equipment_item(size_t index_inv,size_t index_equip, size_t count){}
	virtual void self_get_property(size_t status_point, const q_extend_prop & ) {}
	virtual void get_extprop_base() {}
	virtual void get_extprop_move() {}
	virtual void get_extprop_attack() {}
	virtual void get_extprop_defense() {}
	virtual void player_reject_invite(const XID & member) {}
	virtual void leader_invite(const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid) {}
	virtual void join_team(const XID & leader,int pickup_flag) {}
	virtual void member_leave(const XID & leader, const XID& member,int type) {}
	virtual void leave_party(const XID & leader, int type) {}
	virtual void new_member(const XID & member) {}
	virtual void leader_cancel_party(const XID & leader) {}
	virtual void teammate_get_pos(const XID & target,const A3DVECTOR & pos,int tag) {}
	virtual void send_equipment_info(const XID & target, int cs_index, int sid) {}
	virtual void send_clone_equipment_info(const XID & target, int clone_id, int cs_index, int sid) {}
	virtual void equipment_info_changed(unsigned int madd,unsigned int mdel,const void * buf, size_t size){} 
	virtual void team_member_pickup(const XID & target,int type, int count) {}
	virtual void npc_greeting(const XID & provider) {}
	virtual void repair_all(size_t cost) {}
	virtual void repair(int where,int index,size_t cost) {}
	virtual void renew() {}
	virtual void spend_money(size_t cost){}
	virtual void get_player_money(size_t amount,size_t ) {}
	virtual void battle_score( size_t score ) {}
	virtual void gain_battle_score( size_t inc ) {}
	virtual void spend_battle_score( size_t dec ) {}
	virtual void cast_skill(int target_cnt, const XID* targets, int skill,unsigned short time, unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index) {}
	virtual void skill_interrupt(char reason, char spirit_index) {}
	virtual void skill_continue(int skill_id, char spirit_index) {}
	virtual void skill_perform(char spirit_index) {}
	virtual void stop_skill(char spirit_index) {}
	virtual void get_skill_data() {}
	virtual void clear_embedded_chip(unsigned short equip_idx,size_t use_money) {}
	virtual void learn_skill(int skill, int level) {}
	virtual void takeoff() {}
	virtual void landing() {}
	virtual void flysword_time_capacity(unsigned char where, unsigned char index, int cur_time){}
	virtual void obtain_item(int type, int expire_date, int amount,int slot_amount, int where,int index,int state){}
	virtual void produce_start(int type, int use_time ,int count){}
	virtual void produce_once(int type, int amount,int slot_amount, int where,int index,int state){}
	virtual void produce_end(){}
	virtual void decompose_start(int use_time,int type) {}
	virtual void decompose_end() {}
	virtual void get_task_data() {}
	virtual void send_task_var_data(const void * buf, size_t size) {}
	virtual void start_use_item(int item_type, int use_time) {}
	virtual void cancel_use_item() {}
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count){}	//给自己
	virtual void use_item(int item_type){} //给他人
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count,const char * arg, size_t arg_size){}//给自己
	virtual void use_item(int item_type,const char * arg, size_t arg_size){} //给他人
	virtual void start_use_item_with_target(int item_type, int use_time,const XID & target) {}

	virtual void sit_down() {}
	virtual void stand_up() {}
	virtual void do_emote(unsigned short emotion) {}
	virtual void do_emote_restore(unsigned short emotion) {}
	virtual void do_action(unsigned char action) {}
	virtual void send_timestamp() {}
	virtual void notify_root(unsigned char type) {}
	virtual void self_notify_root(unsigned char type) {}
	virtual void dispel_root(unsigned char type) {}
	virtual void pk_level_notify(char level){}
	virtual void pk_value_notify(int value){}
	virtual void player_change_class(){}
	virtual void update_gfx_state(int64_t  newstate);
	virtual void gather_start(int mine, unsigned char t) {}
	virtual void gather_stop() {}
	virtual void trashbox_passwd_changed(bool has_passwd) {}
	virtual void trashbox_passwd_state(bool has_passwd) {}
	virtual void trashbox_open(unsigned char where) {}
	virtual void trashbox_close() {}
	virtual void trashbox_wealth(size_t money) {}
	virtual void exchange_trashbox_item(unsigned char where, size_t idx1, size_t idx2) {}
	virtual void move_trashbox_item(unsigned char where, size_t src, size_t dest, size_t delta) {}
	virtual void exchange_trashbox_inventory(unsigned char where, size_t idx_tra,size_t idx_inv) {}
	virtual void trash_item_to_inventory(unsigned char where, size_t idx_tra, size_t idx_inv, size_t delta) {}
	virtual void inventory_item_to_trash(unsigned char where, size_t idx_inv, size_t idx_tra, size_t delta) {}
	virtual void exchange_trash_money(int inv_money, int tra_money) {}
	virtual void enchant_result(const XID & caster, int skill, char level,bool invader, char at_state,char stamp,int value);
	virtual void set_adv_data(int data1,int data2){}
	virtual void clear_adv_data(){}
	virtual void player_in_team(unsigned char state) {}
	virtual void send_party_apply(int id, int familyid, int mafiaid, int level, int sectid, int referid) {}
	virtual void query_info_1(int uid,int cs_index, int cs_sid) {}
	virtual void mafia_info_notify() {}
	virtual void task_deliver_reputaion(int offset,int cur_reputaion) {} 
	virtual void task_deliver_exp(int64_t exp, int sp, int dt_exp) {}
	virtual void task_deliver_money(size_t money,size_t cur_money) {}
	virtual void task_deliver_item(int type,int expire_date, int state, int amount,int slot_amount, int where,int index){}
	virtual void task_deliver_level2(int level) {}
	virtual void player_reputation(int reputation) {}
	virtual void identify_result(char index, char result){}
	virtual void change_shape(char shape){}
	virtual void enter_sanctuary(){}
	virtual void leave_sanctuary(){}
	virtual void begin_personal_market(int item_id, int market_id,const char * name,size_t len) {}
	virtual void cancel_personal_market() {}
	virtual void market_trade_success(int trader) {}
	virtual void send_market_name(const XID & target, int cs_index, int sid,const char * name ,size_t len) {}
	virtual void player_start_travel(int  line_no,const A3DVECTOR & dest_pos,float speed,int vehicle){}
	virtual void player_complete_travel(int vehicle) {}
	virtual void toggle_invisible(char tmp) {}
	virtual void toggle_invincible(char tmp) {}
	virtual void trace_cur_pos(unsigned short seq) {}
	virtual void cast_instant_skill(int target_cnt, const XID* targets, int skill,unsigned char level, const A3DVECTOR& pos, char spirit_index) {}
	virtual void activate_waypoint(unsigned short waypoint){}
	virtual void player_waypoint_list(const unsigned short * buf, size_t count){}
	virtual void unlock_inventory_slot(unsigned char where, unsigned short index) {}
	virtual void team_invite_timeout(int who) {}
	virtual void pvp_no_protect() {}
	virtual void send_cooldown_data(){}
	virtual void skill_ability_notify(int id, int ability){}
	virtual void personal_market_available(int index, int item_id) {}
	virtual void breath_data(int breath, int breath_capacity) {}
	virtual void stop_dive() {}
	virtual void trade_away_item(int buyer,short inv_idx,int type, size_t count) {}
	virtual void player_enable_fashion_mode(char is_enable) {}
	virtual void player_enable_free_pvp(char enable_type){}
	virtual void player_enable_effect(short effect) {}
	virtual void player_disable_effect(short effect) {}
	virtual void enable_resurrect_state(float exp_reduce) {}
	virtual void set_cooldown(int idx, int cooldown){}
	virtual void change_team_leader(const XID & old_leader, const XID & new_leader) {}
	virtual void kickout_instance(int time_out) {}
	virtual void kickout_raid(int raid_id, int timeout) {}
	virtual void begin_cosmetic(unsigned short inv_index) {}
	virtual void end_cosmetic(unsigned short inv_index) {}
	virtual void cosmetic_success(unsigned short crc) {}
	virtual void cast_pos_skill(const A3DVECTOR & pos, int skill, unsigned short time, unsigned char level){}
	virtual void change_move_seq(unsigned short seq) {}
	virtual void server_config_data() {}
	virtual void active_rush_mode(char is_active) {}
	virtual void produce_null(int recipe_id) {}
	virtual void active_pvp_combat_state(bool is_active) {}
	virtual void duel_recv_request(const XID &target) {}
	virtual void duel_reject_request(const XID &target,int reason) {}
	virtual void duel_prepare(const XID & target, int delay) {}
	virtual void duel_cancel(const XID & target) {}
	virtual void duel_start(const XID & who) {}
	virtual void duel_stop() {}
	virtual void duel_result(const XID & target, bool is_failed) {}
	virtual void player_bind_request(const XID & target) {}
	virtual void player_bind_invite(const XID & target) {}
	virtual void player_bind_request_reply(const XID & target, int param) {}
	virtual void player_bind_invite_reply(const XID & target, int param) {}
	virtual void player_bind_start(const XID & target) {}
	virtual void player_bind_stop() {}
	virtual void player_mounting(int mount_id, char mount_level, char mount_type = 0) {}
	virtual void send_equip_detail(int cs_indx, int cs_sid, int target, const void * data, size_t size,const void * data_skill, size_t size_skill,const void* data_pet,size_t size_pet,const void* data_pet_equip,size_t size_pet_equip){}
	virtual void enter_battleground( unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp ) {}
	virtual void leave_battleground() {}
	virtual void enter_battlefield(int faction_id_attacker,int faction_id_defender,int faction_id_attacker_assistant,int battle_id,int end_timestamp) {}
	virtual void leave_battlefield() {}
	virtual void battlefield_info(unsigned char count,const S2C::CMD::battlefield_info::structure_info_t* pInfo) {}
	virtual void send_turrent_leader(int id) {}
	virtual void battle_result(int result, int attacker_score, int defender_score) {}
	virtual void get_recipe_data(const unsigned short * buf, size_t count){}
	virtual void player_learn_recipe(unsigned short recipe_id) {}
	virtual void player_produce_skill_info(int level ,int exp) {}
	virtual void start_general_operation(int op_id,int duration, bool broadcast){}
	virtual void stop_general_operation(int op_id, bool broadcast){}
	virtual void refine_result(int item_index, int item_id, int result) {}
	virtual void player_cash(int cash,int used, int cash_add2){}
	virtual void player_bonus(int bonus,int used = -1){}
	virtual void player_skill_addon(int skill, int level) {}
	virtual void player_skill_common_addon(int level) {}
	virtual void player_extra_skill(int skill, int level) {}
	virtual void player_title_list(const short *  title,int count,const short * extra_title, size_t extra_count) {}
	virtual void player_add_title(short title) {}
	virtual void player_del_title(short title){}
	virtual void player_change_title(short title) {}
	virtual void player_use_title(short title) {}
	virtual void player_region_reputation(const int * rep, int count) {}
	virtual void player_change_region_reputation(int idx, int rep) {}
	virtual void player_change_inventory_size(int new_size) {}
	virtual void player_change_mountwing_inv_size(int new_size) {}
	virtual void player_bind_success(size_t index, int id,int state) {}
	virtual void player_change_spouse(int id) {}
	virtual void player_invader_state(bool inv) {}
	virtual void player_mafia_contribution(int con, int fcon) {}
	virtual void lottery_bonus(int lottery_id, int bonus_level, int bonus_item, int bonus_count, int bonus_money) {}
	virtual void player_recorder_start(int stamp) {}
	virtual void player_recorder_check(const char * , size_t) {}
	virtual void npc_service_result(int type) {}
	virtual void battle_flag_change(char os, int ns);
	virtual void battle_player_info( int id, unsigned char battle_faction,
			unsigned int battle_score, unsigned short kill, unsigned short death ) {}
	virtual void battle_info( unsigned char attacker_building_left, unsigned char defender_building_left,
				const char* buf, size_t size ) {}
	virtual void player_wallow_info(int level, int msg, int ptime, int light_t, int heavy_t) {}
	virtual void talisman_exp_notify(int where, int index, int exp) {}
	virtual void talisman_combine(int type1,int type2,int type3){}
	virtual void talisman_combine_value(int value){}
	virtual void talisman_bot_begin( unsigned char active ){}
	virtual void talisman_refineskill(int index, int id) {}
	virtual void anti_cheat_bonus(int type){}
	virtual void player_talent_notify(int value) {}
	virtual void notify_safe_lock(char active, int time, int max_time) {}
	virtual void battlefield_construction_info(int res_a,int res_b,const char* buf,size_t size) {}
	virtual void battlefield_contribution_info(unsigned char page,unsigned char max_page,const char* buf,size_t size) {}
	//宠物相关
	virtual void player_equip_pet_bedge(unsigned char inv_index,unsigned char pet_index) {}
	virtual void player_equip_pet_equip(unsigned char inv_index,unsigned char pet_index) {}
	virtual void player_set_pet_status(unsigned char pet_index,int pet_tid,unsigned char main_status,unsigned char sub_status) {}
	virtual void player_set_pet_rank(unsigned char pet_index,int pet_tid,unsigned char rank) {}
	virtual void player_summon_pet(unsigned char pet_index,int pet_tid,int pet_id) {}
	virtual void player_recall_pet(unsigned char pet_index,int pet_tid,int pet_id) {}
	virtual void player_combine_pet(unsigned char pet_index,int pet_tid,unsigned char type) {}
	virtual void player_uncombine_pet(unsigned char pet_index,int pet_tid) {}
	virtual void player_start_pet_operation(unsigned char pet_index,int pet_id,int delay,unsigned char operation) {}
	virtual void player_end_pet_operation(unsigned char operation) {}
	virtual void player_pet_recv_exp(unsigned char pet_index,int pet_id,int cur_exp) {}
	virtual void player_pet_level_up(unsigned char pet_index,int pet_id,int new_level,int cur_exp) {}
	virtual void player_pet_honor_point(unsigned char pet_index,int cur_honor_point,int max_honor_point) {}
	virtual void player_pet_hunger_point(unsigned char pet_index,int cur_hunger_point,int max_hunger_point) {}
	virtual void player_pet_age_life(unsigned char pet_index,int age,int life) {}
	virtual void player_pet_dead(unsigned char pet_index) {}
	virtual void player_pet_hp_notify(unsigned char pet_index,int cur_hp,int max_hp,int cur_vp,int max_vp) {}
	virtual void player_pet_ai_state(unsigned char pet_index,unsigned char aggro_state,unsigned char stay_state) {}
	virtual void player_pet_room_capacity(unsigned char capacity) {}
	virtual void player_pet_set_auto_skill(unsigned char pet_index,int pet_id,int skill_id,unsigned char set_flag) {}
	virtual void player_pet_set_skill_cooldown(unsigned char pet_index,int pet_id,int cooldown_index,int cooldown_time) {}
	virtual void player_reborn_info(const int * info, size_t count) {}
	virtual void script_message(int player_id,int pet_id,unsigned short channel_id,unsigned short msg_id,bool bBroadcast = false) {}
	virtual void player_pet_civilization(int civilization) {}
	virtual void player_pet_construction(int construction) {}
	virtual void player_move_pet_bedge(unsigned char src_index,unsigned char dst_index) {}
	virtual void player_start_fly(char active, char type) {}
	virtual void player_pet_prop_added(unsigned char pet_index,unsigned short maxhp,unsigned short maxmp,unsigned short defence,
		unsigned short attack,unsigned short* resistance,unsigned short hit,unsigned short jouk,float crit_rate,float crit_damage,int equip_mask) {}
	virtual void sect_become_disciple(int master_id) {}
	virtual void cultivation_notify(char cult) {}
	virtual void offline_agent_bonus(int time, int left_time, int64_t bonus_exp) {}
	virtual void offline_agent_time(int time) {}
	virtual void equip_spirit_decrease(short index, short amount ,int result) {}
	virtual void player_bind_ride_invite(const XID & target){}
	virtual void player_bind_ride_invite_reply(const XID & target, int param){}
	virtual void player_bind_ride_join(const XID & leader, const XID & member, char bind_seq){}
	virtual void player_bind_ride_leave(const XID & leader, const XID & member, char bind_seq){}
	virtual void player_bind_ride_kick(const XID & leader, char bind_seq){}
	virtual void exchange_pocket_item(size_t idx1, size_t idx2){}
	virtual void move_pocket_item(size_t src, size_t dest, size_t count){}
	virtual void exchange_inventory_pocket_item(size_t idx_inv, size_t idx_poc){}
	virtual void move_pocket_item_to_inventory(size_t idx_poc, size_t idx_inv, size_t count){} 
	virtual void move_inventory_item_to_pocket(size_t idx_inv, size_t idx_poc, size_t count){}
	virtual void player_change_pocket_size(int size){}
	virtual void move_all_pocket_item_to_inventory(){}
	virtual void move_max_pocket_item_to_inventory(){}
	virtual void notify_fashion_hotkey(int count,const S2C::CMD::notify_fashion_hotkey::key_combine* key){}
	virtual void exchange_fashion_item(size_t idx1, size_t idx2){}
	virtual void exchange_mount_wing_item(size_t idx1, size_t idx2) {}
	virtual void exchange_inventory_fashion_item(size_t idx_inv, size_t idx_fas){}
	virtual void exchange_inventory_mountwing_item(size_t idx_inv, size_t idx_mw) {}
	virtual void exchange_equipment_fashion_item(size_t idx_equ, size_t idx_fas){} 
	virtual void exchange_equipment_mountwing_item(size_t idx_equ, size_t idx_mw) {}
	virtual void exchange_hotkey_equipment_fashion(size_t idx_key, size_t idx_head, size_t idx_cloth, size_t idx_shoe){}
	virtual void peep_info(int id){}
	virtual void self_killingfield_info(int score, int rank, int con, int pos){}
	virtual void killingfield_info(int attacker_score, int defender_score, int attacker_count, int defender_count, int count, const char *info){}
 	virtual void arena_info(int kill_count, int score, int apply_count, int remain_count, int count, const char * info){}
	virtual void rank_change(char new_rank){}
	virtual void change_killingfield_score(int oldValue, int newValue){}
	virtual void player_change_style(unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid){}
	virtual void player_change_vipstate(unsigned char new_state){}
	virtual void crossserver_battle_info(const S2C::CMD::crossserver_battle_info & info) {}
	virtual void crossserver_off_line_score_info(const std::vector<S2C::CMD::cs_battle_off_line_info::player_off_line_in_cs_battle>& vec_info){}
	virtual void crossserver_team_battle_info(S2C::CMD::crossserver_team_battle_info & info, const char *pbuf, size_t size) {}
	//成就相关
	virtual void player_achievement() {}
	virtual void player_achievement_data(size_t size, const void * data) {} 
	virtual void player_achievement_finish(unsigned short achieve_id, int achieve_point, int finish_time) {} 
	virtual void player_premiss_data(unsigned short achieve_id, char premiss_id, size_t size, const void *data) {} 
	virtual void player_premiss_finish(unsigned short achieve_id, char premiss_id) {} 
	virtual void player_achievement_map(int cs_index, int cs_sid,int target, int achieve_point, const void * data, size_t size) {}
	virtual void achievement_message(unsigned short achieve_id, int param, int finish_time) {}
	
	virtual void self_instance_info(int level_score, int monster_score, int time_score, int death_penalty_score){}
	virtual void instance_info(int cur_level, size_t size, const void *data){}
	virtual void player_start_transform(int template_id, char active, char type, char level, char expLevel) {}
	virtual void magic_duration_decrease(short index, short amount ,int result) {}
	virtual void magic_exp_notify(int where, int index, int exp) {}
	virtual void transform_skill_data(){}
	virtual void player_enter_carrier(int carrier_id,const A3DVECTOR& rpos,unsigned char rdir,unsigned char success){}
	virtual void player_leave_carrier(int carrier_id,const A3DVECTOR& pos,unsigned char dir, unsigned char success){}
	virtual void player_move_on_carrier(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode){}
	virtual void player_stop_move_on_carrier(const A3DVECTOR & target, unsigned short speed ,unsigned char dir,unsigned char move_mode){}
	virtual void skill_proficiency(int id, int proficiency){}
	virtual void notify_mall_prop(int mall_prop){} 
	virtual void send_pk_message(int killer, int deader){} 
	virtual void inc_invisible(int prev, int cur){}
	virtual void dec_invisible(int pre, int cur){}	
	virtual void inc_anti_invisible(int prev, int cur){}
	virtual void dec_anti_invisible(int pre, int cur){}	
	virtual void update_combo_state(char state, char * color){}
	virtual void enter_dim_state(bool state){}
	virtual void enter_invisible_state(bool state){}
	virtual void object_charge(int id, char type, const A3DVECTOR & dest);
	virtual void object_try_charge(char type,  const A3DVECTOR & dest, int target_id){}
	virtual void object_be_charge_to(char type, int target_id, const A3DVECTOR & dest);
	virtual void enter_territory( unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp ) {}
	virtual void leave_territory(){} 
	virtual void last_logout_time(int time){}
	virtual void get_combine_skill_data(){}
	virtual void circle_info_notify(){}
	virtual void receive_deliveryd_exp(int64_t exp, unsigned int award_type){} 
	virtual void object_special_state(int type, char on);
	virtual void lottery2_bonus(int lottery_id, int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money) {}
	virtual void lottery3_bonus(int lottery_id, int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money) {}
	virtual void gem_notify(int gem_id, int msg){}
	//阵法相关
	virtual void notify_circleofdoom_start(int sponsor_id, char faction, int faction_id) {}
	virtual void notify_circleofdoom_info(int skill_id, int sponsor_id, int member_count, const int *pmember_id) {}
	virtual void notify_circleofdoom_stop(int sponsor_id) {}
	virtual void notify_circleofdoom_info_single(const XID & target, int cs_index, int sid, int skill_id,
			        int sponsor_id, int member_count, const int *pmember_id) {}

	virtual void mall_item_price(packet_wrapper &goodslist) {}
	virtual void vip_award_info(int vip_end_time, int recent_total_cash, std::vector<S2C::CMD::vip_award_info::award_data> & award_list) {}
	virtual void vip_award_change(int roleid, unsigned char vipaward_level, char is_hide) {}

	virtual void online_award_info(char force_flag, std::vector<S2C::CMD::online_award_info::award_data> & award_list) {}
	virtual void online_award_permit(int awardid, int awardindex, char permitcode) {}

	virtual void player_scale(char on, int scale_ratio) {}
	virtual void player_move_cycle_area(char mode, char type, int idx, float radius, const A3DVECTOR& pos) {}
	
	//副本相关
	virtual void enter_raid( unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp ) {}
	virtual void leave_raid() {}
	//liuyue-facbase
//	virtual void notify_raid_pos(const A3DVECTOR & pos){}		//通知自己在副本中的当前位置
	virtual void player_raid_counter() {}						//通知玩家副本计数
	virtual void player_single_raid_counter(int map_id) {}
	virtual void send_raid_global_vars(int cnt, const void* buf, size_t size) {}
	virtual void be_moved(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode) {}	//对象被服务器移动，现在仅在NPC更随时使用
	virtual void player_pulling(bool on) {}
	virtual void player_be_pulled(int player_pulling_id, bool on, unsigned char type) {}
	virtual void player_be_pulled(const XID & target, int cs_index, int sid, int player_pulling_id, bool on, unsigned char type) {}

	virtual void raid_info(const char* buf, size_t size) {}
	virtual void trigger_skill_time(short skill_id, short time) {}
	virtual void player_stop_skill(char spirit_index) {}
	virtual void addon_skill_permanent(int skill, int level) {}
	virtual void get_addon_skill_permanent_data() {}
	virtual void zx_five_anni_data(char star, char state) {}

	virtual void treasure_info(){}
	virtual void treasure_region_upgrade(int region_id, int region_index, bool success, int new_level){}  
	virtual void treasure_region_unlock(int region_id, int region_index, bool success){}  
	virtual void treasure_region_dig(int region_id, int region_index, bool success, char event_id, size_t award_item_num, void * buf){}  
	virtual void inc_treasure_dig_count(int inc_count, int total_dig_count){}

	virtual void littlepet_info(char level, char cur_award_lvl, int cur_exp){}
	virtual void rune_info(bool active, int rune_score, int customize_info){}
	virtual void rune_refine_result(char type){}

	//爬塔副本相关
	virtual void random_tower_monster(int level, char reenter){}
	virtual void random_tower_monster_result(int monster_tid, char client_idx) {}
	virtual void player_tower_data(){}
	virtual void player_tower_monster_list(){}
	virtual void player_tower_give_skills(const void* buf, size_t size) {} 
	virtual void set_tower_give_skill(int skill_id, int skill_level, bool used) {}
	virtual void add_tower_monster(int monster_tid) {}
	virtual void set_tower_reward_state(char type, int level, bool delivered) {}
	virtual void tower_level_pass(int level, int best_time) {}
	virtual void fill_platform_mask(int mask) {}
	virtual void put_item(int type, int index, int expire_date, int amount,int slot_amount, int where,int state){}
	virtual void object_start_special_move(int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp){} 
	virtual void object_special_move(int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp){} 
	virtual void object_stop_special_move(int id, unsigned char dir, A3DVECTOR & cur_pos){}
	virtual void player_change_name(int id, int name_len, char * name){} 
	virtual void change_faction_name(int id, int fid, char type, int name_len, char * name){} 
	virtual void enter_collision_raid(unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp){}
	virtual void collision_special_state(int type, char on){}
	virtual void collision_raid_result(char result, int score_a, int score_b){}
	virtual void collision_raid_info(int score_a, int score_b, int win_num, int lost_num, int draw_num, int daily_award_timestamp, int last_collision_timestamp, 
			int day_win, int cs_personal_award_timestamp, int cs_exchange_award_timestamp, int cs_team_award_timestamp){}

	//PK竞猜相关
	virtual void get_player_pk_bet_data(){}

	virtual void player_darklight(){}
	virtual void player_darklight_state(){}
	virtual void player_darklight_spirit(){}
	virtual void mirror_image_cnt(int cnt){}

	virtual void player_wing_change_color(int id, char wing_color){}
	virtual void notify_astrology_energy(){}
	virtual void liveness_notify(){}
	virtual void ui_transfer_opened_notify(){}
	virtual void astrology_upgrade_result(bool success, int new_level){}
	virtual void astrology_destroy_result(int gain_energy){}

	virtual void dir_visible_state(int id, int dir, bool on) {}
	virtual void player_remedy_metempsychosis_level(){}
	virtual void collision_player_pos(int roleid, A3DVECTOR & pos){}
	virtual void enter_kingdom(unsigned char battle_faction, char kingdom_type, char cur_phase, int end_timestamp, int defender_mafia_id, std::vector<int> & attacker_mafia_ids){}
	virtual void leave_kingdom(char kingdom_type){}
	virtual void kingdom_half_end(int win_faction, int end_timestamp){}
	virtual void kingdom_end_result(int win_mafia_id){}
	virtual void raid_level_start(char level) {}
	virtual void raid_level_end(char level, bool result) {}
	virtual void raid_level_result(bool final, bool result[10], bool final_award_deliveried, bool level_award_deliveried[10]) {}
	virtual void raid_level_info(char level, short matter_cnt, int start_time) {}
	virtual void sale_promotion_info(std::set<int> taskids) {}
	virtual void event_info(){} 
	virtual void cash_gift_ids(int id1, int id2){} 
	virtual void notify_cash_gift(int gift_id, int gift_index, int gift_score){} 

	virtual void player_propadd() {}
	virtual void player_get_propadd_item(int item_id, int item_idx) {}

	virtual void player_extra_equip_effect(){}
	virtual void kingdom_key_npc_info(int cur_hp, int max_hp, int hp_add, int hp_dec){}
	virtual void notify_kingdom_info(int kingdom_title, int kingdom_point){}
	virtual void cs_flow_player_info(int score, int c_kill_cnt, bool m_kill_change, int m_kill_cnt, short kill_cnt, short death_cnt, short max_ckill_cnt, short max_mkill_cnt) {}

	virtual void player_bath_invite(const XID & target) {}
	virtual void player_bath_invite_reply(const XID & target, int param) {}
	virtual void player_bath_start(const XID & target) {}
	virtual void player_bath_stop() {}
	virtual void enter_bath_world(int remain_bath_count){}
	virtual void leave_bath_world(){}
	virtual void bath_gain_item(int item_id, int item_count){}

	virtual void flow_battle_info(const char* buf, size_t size ) {}
	virtual void fuwen_compose_info(int compose_count, int extra_compose_count){}
	virtual void fuwen_compose_end(){}
	virtual void fuwen_install_result(int src_index, int dst_index){}
	virtual void fuwen_uninstall_result(int fuwen_index, int inv_index){}

	virtual void multi_exp_mode(int multi_exp_factor, int timestamp){}
	virtual void web_order_info(){}
	virtual void control_trap_info(int id, int tid, int time){}
	virtual void npc_invisible(bool on){}
	virtual void puppet_form(bool on, int player_id, int puppet_id){}
	virtual void teleport_skill_info(int leader_id, int npc_tid, int skill_id, int teleport_count){}
	virtual void blow_off(int roleid);

	virtual void player_mobactive_state_start(const XID & playerid, const XID & mobactive, int pos) {}
	virtual void player_mobactive_state_run(const XID & playerid, const XID & mobactive, int pos) {}
	virtual void player_mobactive_state_ending(const XID & playerid, const XID & mobactive, int pos) {}
	virtual void player_mobactive_state_finish(const XID & playerid, const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos) {}
	virtual void player_mobactive_state_cancel(const XID & playerid, const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos) {}

	virtual void player_qilin_invite(const XID & target) {}
	virtual void player_qilin_invite_reply(const XID & target, int param) {}
	virtual void player_qilin_start(const XID & target) {}
	virtual void player_qilin_stop() {}
	virtual void player_qilin_disconnect() {}
	virtual void player_qilin_reconnect(const A3DVECTOR & newpos){}

	virtual void phase_info() {}		
	virtual void phase_info_change(int phase_id, bool on){}

	virtual void fac_building_add(int index, int tid) {}
	virtual void fac_building_upgrade(int index, int tid, int lev) {}
	virtual void fac_building_complete(int index, int tid, int level) {}
	virtual void fac_building_remove(int index) {}
	virtual void player_fac_base_info(const S2C::CMD::player_fac_base_info & base_info, const std::vector<S2C::INFO::player_fac_field> & field_info, const abase::octets & msg) {}

	virtual void player_active_emote_invite(const XID & target, int active_emote_type) {}
	virtual void player_active_emote_invite_reply(const XID& target, int param) {}
	virtual void player_active_emote_start(const XID& target,int active_emote_type) {}
	virtual void player_active_emote_stop(const XID& target,int active_emote_type) {}

	virtual void be_taunted2(const XID & who, int time){}
	virtual void player_change_puppet_form(){}
	virtual void notify_bloodpool_status(bool on, int cur_hp, int max_hp){}

	virtual void player_start_travel_around(int travel_vehicle_id, float travel_speed, int travel_path_id){}
	virtual void player_stop_travel_around(){}
	virtual void sync_pet_prop(int cs_index, int uid, int sid, int pet_index, q_extend_prop & prop){}
	virtual void cs6v6_cheat_info(bool is_cheat, int cheat_counter){}
	virtual void hide_and_seek_raid_info(int last_hide_and_seek_timestamp, int day_has_get_award) {}

public:
	void MoveBetweenSlice(slice * pPiece,slice * pNewPiece,const A3DVECTOR &pos);
};


class gobject_imp : public substance
{
protected:
	enum OBJECT_TYPE
	{
		TYPE_PLAYER,
		TYPE_NPC,
		TYPE_PET,
		TYPE_SUMMON,
	};

public:
DECLARE_SUBSTANCE(gobject_imp);
public:
	world * _plane;
	gobject * _parent;

	controller * _commander;
	dispatcher * _runner;
public:
	gobject_imp():_plane(NULL),_parent(NULL),_commander(0),_runner(0){}
	virtual void Init(world * pPlane,gobject*parent)
	{
		_plane = pPlane;
		_parent = parent;
	}
	virtual ~gobject_imp(){}
	/**
	 *	调用这个函数时，_parent已经被锁定所以再次发送send_message和post_message时要注意先开锁,
	 *	返回值非零表示不需要再次告知外面不需要再开锁
	 *	即正常情况下(未开锁)一定要返回0
	 */
	virtual  int MessageHandler(const MSG & msg)
	{
		return 0;
	}
	
	typedef int (*msg_filter)(world * pPlane, const MSG & msg);
	/*
	 * 	 设置可能的消息过滤器，一个对象同时只能有一个消息过滤器
	 * 	 这个函数被设计为应该被controller对象调用
	 */
	msg_filter SetMsgFilter(msg_filter filter);
	

	/**
	 *	进行移动的函数，会进行比较复杂的操作，设置时钟进行移动判断。
	 */
	virtual int PrepareMove(const A3DVECTOR &dest) {return 0;};	//处理移动命令，会加入定时器，用来处理移动


	/**
	 *	怪物出生	
	 *	
	 */
	virtual void OnCreate()
	{
		ASSERT(false);
	}

	/**
	 *	重新复生，清除所有不利状态，
	 *	从死亡状态回复
	 */
	virtual void Reborn()
	{
		ASSERT(false);
	}

	/*
	 *	测试是否可以开始交易，只有player 可以交易
	 */
	virtual bool CanTrade(const XID & target)
	{
		return false;
	}

	/**
	 *	开始交易，只有player 可以完成这个操作
	 *
	 */
	virtual void StartTrade(int trade_id,const XID & target)
	{
		return ;
	}

	/*
	 *	测试是否可以进行组队关系更改
	 */
	virtual bool CanTeamRelation()
	{
		return false;
	}

	/*
	 *	开始帮派交易(帮派服务),	
	 */
	 virtual bool StartFactionTrade(int trade_id,int get_mask, int put_mask,bool no_response = false)
	 {
	 	ASSERT(false);
	 	return false;
	 }

	 /*
	  *	帮派交易完成，同时进行回写操作 参数是随便写的现在
	  */
	 virtual void FactionTradeComplete(int trade_id, const GNET::syncdata_t &data)
	 {
	 	ASSERT(false);
	 	return ;
	 }
	 
	 /*
	 	交易完成，同时进行回写操作
	 */
	 virtual void SyncTradeComplete(int trade_id, size_t money, const GDB::itemlist & item_change,bool write_trashbox,bool money_change, bool cash_change)
	 {
	 	ASSERT(false);
		return;
	 }

	/**
	 *	交易数据回写成功或者失败(在等待交易阶段调用)
	 */
	virtual void WaitingTradeWriteBack(int trade_id, bool bSuccess)
	{
		return;
	}

	/*
	 *	
	 *	交易服务器发来了交易完成的消息 （成功或者失败）
	 */
	virtual void TradeComplete(int trade_id,int reason, bool need_read){}

	
	/*
	*	取消切换服务器的操作
	*/
	virtual void CancelSwitch(){}

	/**
		更新帮派信息
	*/

	virtual void UpdateMafiaInfo(int m_id,int f_id,  char rank)
	{
		//do nothing
	}

	/*
	*	玩家强制下线的操作
	*/
	virtual void PlayerForceOffline(){}

	/*
	*	关闭的操作
	*/
	virtual void ServerShutDown(){}

	/**
	 * @Breif: 玩家寄售相关操作
	 */
	virtual int CanConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time){ return false; }
	virtual int CanConsignRole(char type, int item_idx, int sold_time, int price, int margin) { return -1; }
	virtual void StartConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int solde_time, char* sellto_name, char sellto_name_len){}
	virtual void StartConsignRole(char type, int item_id, int item_idx, int item_cnt, int item_type, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len){} 
	virtual void CompleteConsign(int reason, char need_read){}
	virtual void SetConsignRoleFlag(bool is_consign_role) {}

	int GetWorldTag();
	int GetClientTag();  //liuyue-facbse 返回客户端用的地图号 多个副本实例会共用同一个地图号
	world_manager* GetWorldManager();
public:
	/**
	 *	分发命令到控制器，要求将本对象上锁后再调用
	 *	和 消息处理类似，如果返回了非0，那么表示外面不需要解锁
	 */
	virtual int DispatchCommand(int cmd_type, const void * buf,size_t size)
	{
		ASSERT(_commander);
		return _commander->CommandHandler(cmd_type,buf,size);
	}

	virtual int DispatchMessage(const MSG & msg)
	{
		return MessageHandler(msg);
	}

	void EnterWorld()
	{
		_runner->begin_transfer();
		_runner->get_base_info();
		_runner->query_info00();
		_runner->enter_world();
		_runner->end_transfer();
	}
	
//protected:
public:
	/*
	 * 真实移动对象，移动一个较小的量，移动的量如果过大的话
	 * 会丢失给原来所在区域的离开信息，以后可以考虑加上此判断
	 * 返回值表示是否进行了移动
	 */
	virtual bool StepMove(const A3DVECTOR &offset);

	/*
	 * 这个函数判定对象是否可以发生移动，可以不调用这个函数而直接调用StepMove
	 * 这个函数在于不进行真正的移动而判断是否可以移动
	 */
	virtual bool CanMove() { return true; }

	/*
	 * 进行服务器的切换
	 */
	virtual void SwitchSvr(int dest,const A3DVECTOR &oldpos , const A3DVECTOR &newpos,const instance_key *)
	{
		ASSERT(false);
	}
	friend class gobject;
};

gobject_imp * 	CF_CreateImp(int guid,world * plane,gobject * obj);
dispatcher * 	CF_CreateDispatcher(int guid,gobject_imp*);
controller*	CF_CreateCtrl(int guid,gobject_imp*);

inline 	gobject_imp * CF_Create(int guid_imp,int guid_dis,int guid_ctrl,world *plane,gobject *obj)
{
	gobject_imp * imp = CF_CreateImp(guid_imp,plane,obj);
	ASSERT(imp);
	if(!imp) return NULL;
	controller *ctrl = CF_CreateCtrl(guid_ctrl,imp);
	dispatcher *runner = CF_CreateDispatcher(guid_dis,imp);
	ASSERT(ctrl && runner);
	if(!ctrl || !runner)
	{
		delete ctrl;
		delete runner;
		delete imp;
		return NULL;
	}
	imp->_runner = runner;
	imp->_commander = ctrl;
	return imp;
}

#endif
