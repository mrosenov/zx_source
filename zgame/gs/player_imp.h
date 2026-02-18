#ifndef __ONLINEGAME_GS_PLAYER_IMP_H__
#define __ONLINEGAME_GS_PLAYER_IMP_H__

#include "player.h"
#include "actobject.h"
#include "item_list.h"
#include "playerteam.h"
#include "playertrashbox.h"
#include <list>
#include <set>
#include <db_if.h>
#include <gsp_if.h>
#include "playerstall.h"
#include "cooldown.h"
#include "petman.h"
#include "playerduel.h"
#include "playerbind.h"
#include "playerlink.h"
#include "playertitle.h"
#include "playerstate.h"
#include "playermall.h"
#include "playervipaward.h"
#include "playeronlineaward.h"
#include "item_manager.h"
#include "faction.h"
#include "achievement.h"
#include "summonman.h"
#include "mobactiveman.h"
#include "playercircleofdoom.h"
#include "playeronlineaward.h"
#include "playertemplate.h"
#include "playeractiveemote.h"
#include "topic_site.h"

struct MSG;
struct attack_msg;
struct enchant_msg;
struct leader_prop;
struct recipe_template;
struct userlogin_t;
class gsummon_imp;

class gplayer_dispatcher : public  dispatcher
{
protected:
	packet_wrapper _mw;
	packet_wrapper _nw;
	packet_wrapper _pw;
	packet_wrapper _self;
	packet_wrapper _tbuf;
	link_sid       _header;
	abase::vector<int,abase::fast_alloc<> > _leave_list;		//保存离开时不再看到的对象ID
public:
	DECLARE_SUBSTANCE(gplayer_dispatcher);
public:
	bool Save(archive & ar)
	{
		ar << _header;
		return true;
	}
	bool Load(archive & ar)
	{
		ar >> _header;
		return true;
	}
	gplayer_dispatcher()
	{}
	virtual void init(gobject_imp * imp)
	{
		dispatcher::init(imp);
		gplayer * pPlayer = (gplayer*)imp->_parent;
		make_link_sid(pPlayer,_header);
	}
	virtual ~gplayer_dispatcher(){}
	virtual void begin_transfer();
	virtual void end_transfer();
	virtual void enter_slice(slice * ,const A3DVECTOR &pos);
	virtual void leave_slice(slice * ,const A3DVECTOR &pos);
	virtual void notify_pos(const A3DVECTOR & pos);
	virtual void get_base_info();
	virtual void enter_world();	//cache
	virtual void leave_world();
	virtual void appear();
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode);
	virtual void stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mmode);
	virtual void be_damaged(const XID & id,int skill_id, const attacker_info_t &,int damage,int dt_damage,int dura_index,char at_state,char speed,bool orange);
	virtual void dodge_attack(const XID &attacker, int skill_id, const attacker_info_t & , char at_state,char speed,bool orange);
	virtual void be_hurt(const XID & id, const attacker_info_t & ,int damage,bool invader);

	virtual void equipment_damaged(int index);
	virtual void start_attack(const XID &);
	virtual void attack_once(unsigned char dec_amount);
	virtual void stop_attack(int flag);
	virtual void on_death(const XID &,bool reserver, int time);
	virtual void error_message(int msg);
	virtual void resurrect(int);
	virtual void disappear(char at_once=0);
	virtual void pickup_money(int);
	virtual void pickup_item(int type,int expire_date, int amount,int slot_amount, int where,int index,int state);
	virtual void purchase_item(int type, size_t money, int amount,int slot_amount, int where,int index);
	virtual	void query_info00(const XID & target, int cs_index,int sid);
	virtual void query_info00();
	virtual void receive_exp(int64_t exp);
	virtual void receive_bonus_exp(int64_t exp);
	virtual void level_up();
	virtual void deity_levelup(char success);
	virtual void unselect();
	virtual void self_item_empty_info(int where,int index);
	virtual void self_item_info(int where,int index,item_data_client & data,unsigned short crc);
	virtual void self_inventory_data(int where,unsigned char inv_size,const void * data, size_t len);
	virtual void self_inventory_detail_data(int where,unsigned char inv_size,const void * data, size_t len);
	virtual void exchange_item_list_info(unsigned char where,const void *data,size_t len);
	virtual void repurchase_inventory_data(unsigned char inv_size,const void * data, size_t len);
	virtual void exchange_inventory_item(size_t idx1,size_t idx2);
	virtual void move_inventory_item(size_t src,size_t dest,size_t count);
	virtual void player_drop_item(size_t where, size_t index,int type,size_t count, unsigned char drop_type);
	// Youshuang add
	virtual void send_talisman_enchant_addon( int id, int index );
	virtual void produce_colorant_item( int index, int color, int quality );
	virtual void use_colorant_item( int where, int index, int color, int quality );
	virtual void get_achievement_award( unsigned short achiev_id );
	virtual void get_raid_boss_info( const std::vector<raid_boss_info>& monsters_been_killed );
	virtual void get_facbase_mall_info( facbase_mall* mall_info );
	virtual void get_faction_coupon( int faction_coupon );
	virtual void get_facbase_cash_items_info( const std::map<int, int>& item_cooldown );
	virtual void get_facbase_cash_item_success( int index );
	virtual void get_newyear_award_info( int score, const std::vector<char>& status );
	// end
	virtual void exchange_equipment_item(size_t index1,size_t index2);
	virtual void equip_item(size_t index_inv,size_t index_equip,int count_inv,int count_equip);
	virtual void move_equipment_item(size_t index_inv,size_t index_equip, size_t count);
	virtual void self_get_property(size_t status_point, const q_extend_prop & prop );
	virtual void player_select_target(int id);
	virtual void get_extprop_base();
	virtual void get_extprop_move();
	virtual void get_extprop_attack();
	virtual void get_extprop_defense();
	virtual void player_reject_invite(const XID & member);
	virtual void leader_invite(const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid); 
	virtual void join_team(const XID & leader,int pickup_flag);
	virtual void member_leave(const XID & leader, const XID& member,int type);
	virtual void leave_party(const XID & leader, int type);
	virtual void new_member(const XID & member);
	virtual void leader_cancel_party(const XID & leader);
	virtual void teammate_get_pos(const XID & target,const A3DVECTOR & pos, int tag);
	virtual void send_equipment_info(const XID & target, int cs_index, int sid);
	virtual void send_clone_equipment_info(const XID & target, int clone_id, int cs_index, int sid); 
	virtual void equipment_info_changed(unsigned int madd,unsigned int mdel,const void * buf, size_t size);
	virtual void team_member_pickup(const XID & target,int type, int count);
	virtual void npc_greeting(const XID & provider);
	virtual void repair_all(size_t cost);
	virtual void repair(int where,int index,size_t cost);
	virtual void renew();
	virtual void spend_money(size_t cost);
	virtual void get_player_money(size_t amount,size_t capacity);
	virtual void battle_score( size_t score );
	virtual void gain_battle_score( size_t inc );
	virtual void spend_battle_score( size_t dec );
	virtual void cast_skill(int target_cnt, const XID * targets, int skill,unsigned short time, unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index);
	virtual void skill_interrupt(char reason, char spirit_index);
	virtual void skill_continue(int skill_id, char spirit_index); 
	virtual void skill_perform(char spirit_index);
	virtual void stop_skill(char spirit_index);
	virtual void get_skill_data();
	virtual void clear_embedded_chip(unsigned short equip_idx,size_t use_money);
	virtual void learn_skill(int skill, int level);
	virtual void takeoff();
	virtual void landing();
	virtual void flysword_time_capacity(unsigned char where, unsigned char index, int cur_time);
	virtual void obtain_item(int type,int expire_date, int amount,int slot_amount, int where,int index,int state);
	virtual void produce_start(int type, int use_time ,int count);
	virtual void produce_once(int type, int amount,int slot_amount, int where,int index,int state);
	virtual void produce_end();
	virtual void decompose_start(int use_time,int type);
	virtual void decompose_end();
	virtual void get_task_data();
	virtual void send_task_var_data(const void * buf, size_t size);
	virtual void start_use_item(int item_type, int use_time);
	virtual void cancel_use_item();
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count); //给自己
	virtual void use_item(int item_type); //给他人
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count,const char * arg, size_t arg_size);//给自己
	virtual void use_item(int item_type,const char * arg, size_t arg_size); //给他人
	virtual void start_use_item_with_target(int item_type, int use_time,const XID & target);
	virtual void sit_down();
	virtual void stand_up();
	virtual void do_emote(unsigned short emotion);
	virtual void do_emote_restore(unsigned short emotion);
	virtual void do_action(unsigned char action);
	virtual void send_timestamp();
	virtual void notify_root(unsigned char type);
	virtual void self_notify_root(unsigned char type);
	virtual void dispel_root(unsigned char type);
	virtual void pk_level_notify(char level);
	virtual void pk_value_notify(int value);
	virtual void player_change_class();
	virtual void gather_start(int mine,unsigned char t);
	virtual void gather_stop();
	virtual void trashbox_passwd_changed(bool has_passwd);
	virtual void trashbox_passwd_state(bool has_passwd);
	virtual void trashbox_open(unsigned char where);
	virtual void trashbox_close();
	virtual void trashbox_wealth(size_t money);
	virtual void exchange_trashbox_item(unsigned char where, size_t idx1, size_t idx2);
	virtual void move_trashbox_item(unsigned char where,size_t src, size_t dest, size_t delta);
	virtual void exchange_trashbox_inventory(unsigned char where,size_t idx_tra,size_t idx_inv);
	virtual void trash_item_to_inventory(unsigned char where, size_t idx_tra, size_t idx_inv, size_t delta);
	virtual void inventory_item_to_trash(unsigned char where, size_t idx_inv, size_t idx_tra, size_t delta);
	virtual void exchange_trash_money(int inv_money, int tra_money);
	virtual void set_adv_data(int data1,int data2);
	virtual void clear_adv_data();
	virtual void player_in_team(unsigned char state);
	virtual void send_party_apply(int id, int familyid, int mafiaid, int level, int sectid, int referid); 
	virtual void query_info_1(int uid,int cs_index, int cs_sid);
	virtual void mafia_info_notify();
	virtual void task_deliver_reputaion(int offset,int cur_reputaion);
	virtual void task_deliver_exp(int64_t exp, int sp, int dt_exp);
	virtual void task_deliver_money(size_t amount,size_t cur_money);
	virtual void task_deliver_item(int type,int expire_date, int state, int amount,int slot_amount, int where,int index);
	virtual void task_deliver_level2(int level2);
	virtual void player_reputation(int reputation);
	virtual void identify_result(char index, char result);
	virtual void change_shape(char shape);
	virtual void enter_sanctuary();
	virtual void leave_sanctuary();
	virtual void begin_personal_market(int item_id, int market_id,const char *name, size_t len);
	virtual void cancel_personal_market();
	virtual void market_trade_success(int trader);
	virtual void send_market_name(const XID & target, int cs_index, int sid,const char * name ,size_t len);
	virtual void player_start_travel(int  line_no,const A3DVECTOR & dest_pos,float speed,int vehicle);
	virtual void player_complete_travel(int vehicle);
	virtual void toggle_invisible(char tmp);
	virtual void toggle_invincible(char tmp);
	virtual void trace_cur_pos(unsigned short seq);
	virtual void cast_instant_skill(int target_cnt, const XID* targets, int skill,unsigned char level, const A3DVECTOR& pos, char spirit_index);
	virtual void activate_waypoint(unsigned short waypoint);
	virtual void player_waypoint_list(const unsigned short * buf, size_t count);
	virtual void unlock_inventory_slot(unsigned char where, unsigned short index);
	virtual void team_invite_timeout(int who);
	virtual void pvp_no_protect();
	virtual void send_cooldown_data();
	virtual void update_cooldown_data(archive & ar);
	virtual void skill_ability_notify(int id, int ability);
	virtual void personal_market_available(int index, int item_id);
	virtual void breath_data(int breath, int breath_capacity);
	virtual void stop_dive();
	virtual void trade_away_item(int buyer,short inv_idx,int type, size_t count);
	virtual void player_enable_fashion_mode(char is_enable);
	virtual void player_enable_free_pvp(char is_enable);
	virtual void player_enable_effect(short effect);
	virtual void player_disable_effect(short effect);
	virtual void enable_resurrect_state(float exp_reduce);
	virtual void set_cooldown(int idx, int cooldown);
	virtual void change_team_leader(const XID & old_leader, const XID & new_leader);
	virtual void kickout_instance(int timeout);
	virtual void kickout_raid(int raid_id, int timeout);
	virtual void begin_cosmetic(unsigned short inv_index);
	virtual void end_cosmetic(unsigned short inv_index);
	virtual void cosmetic_success(unsigned short crc);
	virtual void cast_pos_skill(const A3DVECTOR & pos, int skill,unsigned short time,unsigned char level);
	virtual void change_move_seq(unsigned short seq);
	virtual void server_config_data();
	virtual void active_rush_mode(char is_active);
	virtual void produce_null(int recipe_id);
	virtual void active_pvp_combat_state(bool is_active);
	virtual void duel_recv_request(const XID &target);
	virtual void duel_reject_request(const XID &target,int reason);
	virtual void duel_prepare(const XID & target, int delay);
	virtual void duel_cancel(const XID & target);
	virtual void duel_start(const XID & who);
	virtual void duel_stop();
	virtual void duel_result(const XID & target, bool is_failed);
	virtual void player_bind_request(const XID & target);
	virtual void player_bind_invite(const XID & target);
	virtual void player_bind_request_reply(const XID & target, int param);
	virtual void player_bind_invite_reply(const XID & target, int param); 
	virtual void player_bind_start(const XID & target);
	virtual void player_bind_stop();
	virtual void player_mounting(int mount_id, char mount_level, char mount_type = 0);
	virtual void send_equip_detail(int cs_indx, int cs_sid, int target, const void * data, size_t size,const void * data_skill, size_t size_skill,const void* data_pet,size_t size_pet,const void* data_pet_equip,size_t size_pet_equip);

	virtual void enter_battleground( unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp );
	virtual void leave_battleground();
	virtual void enter_battlefield(int faction_id_attacker,int faction_id_defender,int faction_id_attacker_assistant,int battle_id,int end_timestamp);
	virtual void leave_battlefield();
	virtual void battlefield_info(unsigned char count,const S2C::CMD::battlefield_info::structure_info_t* pInfo);
	virtual void battle_result(int result, int attacker_score, int defender_score);

	virtual void get_recipe_data(const unsigned short * buf, size_t count);
	virtual void player_learn_recipe(unsigned short recipe_id);
	virtual void player_produce_skill_info(int level ,int exp);
	virtual void start_general_operation(int op_id,int duration, bool broadcast);
	virtual void stop_general_operation(int op_id, bool broadcast);
	virtual void refine_result(int item_index, int item_id, int item_result);
	virtual void player_cash(int cash,int used, int cash_add2);
	virtual void player_bonus(int bonus,int used);
	virtual void player_skill_addon(int skill, int level);
	virtual void player_skill_common_addon(int level);
	virtual void player_extra_skill(int skill, int level);
	virtual void player_title_list(const short* title,int count,const short*extra_title,size_t extra_count);
	virtual void player_add_title(short title);
	virtual void player_del_title(short title);
	virtual void player_change_title(short title);
	virtual void player_use_title(short title);
	virtual void player_region_reputation(const int * rep, int count);
	virtual void player_change_region_reputation(int idx, int rep);
	virtual void player_change_inventory_size(int new_size);
	virtual void player_change_mountwing_inv_size(int new_size);
	virtual void player_bind_success(size_t index, int id,int state);
	virtual void player_change_spouse(int id);
	virtual void player_invader_state(bool inv);
	virtual void player_mafia_contribution(int con, int fcon);
	virtual void lottery_bonus(int lottery_id, int bonus_level, int bonus_item, int bonus_count, int bonus_money);
	virtual void player_recorder_start(int stamp);
	virtual void player_recorder_check(const char * , size_t);
	virtual void npc_service_result(int type);
	virtual void battle_player_info( int id, unsigned char battle_faction,
			unsigned int battle_score, unsigned short kill, unsigned short death );
	virtual void battle_info( unsigned char attacker_building_left, unsigned char defender_building_left,
			const char* buf, size_t size );
	virtual void player_wallow_info(int level, int msg, int ptime,int light_t, int heavy_t);
	virtual void talisman_exp_notify(int where, int index, int exp);
	virtual void talisman_combine(int type1,int type2,int type3);
	virtual void talisman_combine_value(int value);
	virtual void talisman_bot_begin( unsigned char active );
	virtual void talisman_refineskill(int index, int id);
	virtual void anti_cheat_bonus(int type);
	virtual void player_talent_notify(int value);
	virtual void notify_safe_lock(char active, int time, int max_time);
	virtual void battlefield_construction_info(int res_a,int res_b,const char* buf,size_t size);
	virtual void battlefield_contribution_info(unsigned char page,unsigned char max_page,const char* buf,size_t size);

	//宠物相关
	virtual void player_equip_pet_bedge(unsigned char inv_index,unsigned char pet_index);
	virtual void player_equip_pet_equip(unsigned char inv_index,unsigned char pet_index);
	virtual void player_set_pet_status(unsigned char pet_index,int pet_tid,unsigned char main_status,unsigned char sub_status);
        virtual void player_set_pet_rank(unsigned char pet_index,int pet_tid,unsigned char rank);
	virtual void player_summon_pet(unsigned char pet_index,int pet_tid,int pet_id);
	virtual void player_recall_pet(unsigned char pet_index,int pet_tid,int pet_id);
	virtual void player_combine_pet(unsigned char pet_index,int pet_tid,unsigned char type);
	virtual void player_uncombine_pet(unsigned char pet_index,int pet_tid);
	virtual void player_start_pet_operation(unsigned char pet_index,int pet_id,int delay,unsigned char operation);
	virtual void player_end_pet_operation(unsigned char operation);
	virtual void player_pet_recv_exp(unsigned char pet_index,int pet_id,int cur_exp);
	virtual void player_pet_level_up(unsigned char pet_index,int pet_id,int new_level,int cur_exp);
	virtual void player_pet_honor_point(unsigned char pet_index,int cur_honor_point,int max_honor_point);
	virtual void player_pet_hunger_point(unsigned char pet_index,int cur_hunger_point,int max_hunger_point);
	virtual void player_pet_age_life(unsigned char pet_index,int age,int life);
	virtual void player_pet_dead(unsigned char pet_index);
	virtual void player_pet_hp_notify(unsigned char pet_index,int cur_hp,int max_hp,int cur_vp,int max_vp);
	virtual void player_pet_ai_state(unsigned char pet_index,unsigned char aggro_state,unsigned char stay_state);
        virtual void player_pet_room_capacity(unsigned char capacity);
	virtual void player_pet_set_auto_skill(unsigned char pet_index,int pet_id,int skill_id,unsigned char set_flag);
	virtual void player_pet_set_skill_cooldown(unsigned char pet_index,int pet_id,int cooldown_index,int cooldown_time);
	virtual void player_reborn_info(const int * info, size_t count);
	virtual void script_message(int player_id,int pet_id,unsigned short channel_id,unsigned short msg_id,bool bBroadcast = false);
	virtual void player_pet_civilization(int civilization);
	virtual void player_pet_construction(int construction);
	virtual void player_move_pet_bedge(unsigned char src_index,unsigned char dst_index);
	virtual void player_start_fly(char active, char type);
	virtual void player_pet_prop_added(unsigned char pet_index,unsigned short maxhp,unsigned short maxmp,unsigned short defence,
		unsigned short attack,unsigned short* resistance,unsigned short hit,unsigned short jouk,float crit_rate,float crit_damage,int equip_mask);
	virtual void sect_become_disciple(int master_id);
	virtual void cultivation_notify(char cult);
	virtual void offline_agent_bonus(int time, int left_time, int64_t bonus_exp);
	virtual void offline_agent_time(int time);
	virtual void equip_spirit_decrease(short index, short amount ,int result);
	virtual void player_bind_ride_invite(const XID & target);
	virtual void player_bind_ride_invite_reply(const XID & target, int param);
	virtual void player_bind_ride_join(const XID & leader, const XID & member, char bind_seq);
	virtual void player_bind_ride_leave(const XID & leader, const XID & member, char bind_seq);
	virtual void player_bind_ride_kick(const XID & leader, char bind_seq);
	virtual void exchange_pocket_item(size_t idx1, size_t idx2);
	virtual void move_pocket_item(size_t src, size_t dest, size_t count);
	virtual void exchange_inventory_pocket_item(size_t idx_inv, size_t idx_poc);
	virtual void move_pocket_item_to_inventory(size_t idx_poc, size_t idx_inv, size_t count); 
	virtual void move_inventory_item_to_pocket(size_t idx_inv, size_t idx_poc, size_t count);
	virtual void player_change_pocket_size(int size);
	virtual void move_all_pocket_item_to_inventory();
	virtual void move_max_pocket_item_to_inventory();
	virtual void notify_fashion_hotkey(int count,const S2C::CMD::notify_fashion_hotkey::key_combine* key);
	virtual void exchange_fashion_item(size_t idx1, size_t idx2);
	virtual void exchange_mount_wing_item(size_t idx1, size_t idx2);
	virtual void exchange_inventory_fashion_item(size_t idx_inv, size_t idx_fas);
	virtual void exchange_inventory_mountwing_item(size_t idx_inv, size_t idx_mw);
	virtual void exchange_equipment_fashion_item(size_t idx_equ, size_t idx_fas); 
	virtual void exchange_equipment_mountwing_item(size_t idx_equ, size_t idx_mw);
	virtual void exchange_hotkey_equipment_fashion(size_t idx_key, size_t idx_head, size_t idx_cloth, size_t idx_shoe);
	virtual void peep_info(int id);
	virtual void self_killingfield_info(int score, int rank, int con, int pos);
	virtual void killingfield_info(int attacker_score, int defender_score, int attacker_count, int defender_count, int count, const char *info);
 	virtual void arena_info(int kill_count, int score, int apply_count, int remain_count, int count, const char * info);
	virtual void rank_change(char new_rank);
	virtual void change_killingfield_score(int oldValue, int newValue);
	virtual void player_change_style(unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid);
	virtual void player_change_vipstate(unsigned char new_state);
	virtual void crossserver_battle_info(const S2C::CMD::crossserver_battle_info & info);
	virtual void crossserver_off_line_score_info(const std::vector<S2C::CMD::cs_battle_off_line_info::player_off_line_in_cs_battle>& vec_info);
	virtual void crossserver_team_battle_info(S2C::CMD::crossserver_team_battle_info & info, const char *pbuf, size_t size);
	virtual void self_instance_info(int level_score, int monster_score, int time_score, int death_penalty_score);
	virtual void instance_info(int cur_level, size_t size, const void *data);
	virtual void player_start_transform(int template_id, char active, char type, char level, char expLevel);
	virtual void magic_duration_decrease(short index, short amount ,int result); 
	virtual void magic_exp_notify(int where, int index, int exp); 
	virtual void transform_skill_data();
	
	virtual void player_achievement();
	virtual void player_achievement_data(size_t size, const void * data);
	virtual void player_achievement_finish(unsigned short achieve_id, int achieve_point, int finish_time);
	virtual void player_premiss_data(unsigned short achieve_id, char premiss_id, size_t size, const void *data);
	virtual void player_premiss_finish(unsigned short achieve_id, char premiss_id);
	virtual void player_achievement_map(int cs_index, int cs_sid,int target, int achieve_point, const void * data, size_t size); 
	virtual void achievement_message(unsigned short achieve_id, int param, int finish_time);
	virtual void player_enter_carrier(int carrier_id,const A3DVECTOR& rpos,unsigned char rdir,unsigned char success);
	virtual void player_leave_carrier(int carrier_id,const A3DVECTOR& pos,unsigned char dir, unsigned char success);
	virtual void player_move_on_carrier(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode);
	virtual void player_stop_move_on_carrier(const A3DVECTOR & target, unsigned short speed ,unsigned char dir,unsigned char move_mode);
	virtual void skill_proficiency(int id, int proficiency);
	virtual void notify_mall_prop(int mall_prop); 
	virtual void send_pk_message(int killer, int deader); 
	virtual void inc_invisible(int prev, int cur);
	virtual void dec_invisible(int pre, int cur);	
	virtual void inc_anti_invisible(int prev, int cur);
	virtual void dec_anti_invisible(int pre, int cur);	
	virtual void update_combo_state(char state, char * color);
	virtual void enter_dim_state(bool state);
	virtual void enter_invisible_state(bool state);
	virtual void enter_territory( unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp ); 
	virtual void leave_territory(); 
	virtual void last_logout_time(int time);
	virtual void get_combine_skill_data();
	virtual void circle_info_notify();
	virtual void receive_deliveryd_exp(int64_t exp, unsigned int award_type);
	virtual void lottery2_bonus(int lottery_id, int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money); 
	virtual void lottery3_bonus(int lottery_id, int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money); 
	virtual void gem_notify(int gem_id, int msg);

	//阵法相关
	virtual void notify_circleofdoom_start(int sponsor_id, char faction, int faction_id);
	virtual void notify_circleofdoom_info(int skill_id, int sponsor_id, int member_count, const int *pmember_id);
	virtual void notify_circleofdoom_stop(int sponsor_id);
	virtual void notify_circleofdoom_info_single(const XID & target, int cs_index, int sid, int skill_id,
			        int sponsor_id, int member_count, const int *pmember_id);

	virtual void mall_item_price(packet_wrapper &goodslist);
	virtual void vip_award_info(int vip_end_time, int recent_total_cash, std::vector<S2C::CMD::vip_award_info::award_data> & award_list);
	virtual void vip_award_change(int roleid, unsigned char vipaward_level, char is_hide);

	virtual void online_award_info(char force_flag, std::vector<S2C::CMD::online_award_info::award_data> & award_list);
	virtual void online_award_permit(int awardid, int awardindex, char permitcode);

	void mafia_trade_start();
	void mafia_trade_end();
	//下面函数是player专属的组队信息传递函数,其他对象暂时没有 所以暂时不是虚函数
	void send_team_data(const XID & leader,size_t team_count,size_t data_count, const player_team::member_entry ** list);
	void send_team_data(const XID & leader,size_t team_count,const player_team::member_entry * list);

	void item_to_money(size_t index, int type,size_t count , size_t price);
	void pickup_money_in_trade(size_t);
	void pickup_item_in_trade(int type, int amount);
	void receive_money_after_trade(size_t);
	void receive_item_after_trade(int type,int expire_date, int amount, int slot_amount, int index,int state);
	void embed_item(size_t chip_idx, size_t equip_idx);
	void object_is_invalid(int id);
	void trashbox_capacity_notify(int cap);
	void mafia_trashbox_capacity_notify(int cap);
	void player_scale(char on, int scale_ratio);
	void player_move_cycle_area(char mode, char type, int idx, float radius, const A3DVECTOR& pos);

	//副本相关
	virtual void enter_raid( unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp );
	virtual void leave_raid();	
//	virtual void notify_raid_pos(const A3DVECTOR & pos);		//通知自己在副本中的当前位置
	virtual void player_raid_counter();							//通知玩家副本计数
	virtual void player_single_raid_counter(int map_id);
	virtual void send_raid_global_vars(int cnt, const void* buf, size_t size);

	virtual void be_moved(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode);
	virtual void player_pulling(bool on);
	virtual void player_be_pulled(int player_pulling_id, bool on, unsigned char type);
	virtual void player_be_pulled(const XID & target, int cs_index, int sid, int player_pulling_id, bool on, unsigned char type);

	virtual void raid_info(const char* buf, size_t size);
	virtual void trigger_skill_time(short skill_id, short time);
	virtual void player_stop_skill(char spirit_index);
	virtual void addon_skill_permanent(int level, int skill);
	virtual void get_addon_skill_permanent_data();
	virtual void zx_five_anni_data(char star, char state);
	
	virtual void treasure_info();
	virtual void treasure_region_upgrade(int region_id, int region_index, bool success, int new_level);  
	virtual void treasure_region_unlock(int region_id, int region_index, bool success);  
	virtual void treasure_region_dig(int region_id, int region_index, bool success, char event_id, size_t award_item_num, void * buf); 
	virtual void inc_treasure_dig_count(int inc_count, int total_dig_count);

	virtual void littlepet_info(char level, char cur_award_lvl, int cur_exp);
	virtual void rune_info(bool active, int rune_score, int customize_info);
	virtual void rune_refine_result(char type);
	
	//爬塔副本相关
	virtual void random_tower_monster(int level, char reenter);
	virtual void random_tower_monster_result(int monster_tid, char client_idx);
	virtual void player_tower_data();
	virtual void player_tower_monster_list();
	virtual void player_tower_give_skills(const void* buf, size_t size);
	virtual void set_tower_give_skill(int skill_id, int skill_level, bool used);
	virtual void add_tower_monster(int monster_tid);
	virtual void set_tower_reward_state(char type, int level, bool delivered);	
	virtual void tower_level_pass(int level, int best_time);
	virtual void fill_platform_mask(int mask); 
	virtual void put_item(int type, int index, int expire_date, int amount,int slot_amount, int where,int state);
	virtual void object_start_special_move(int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp); 
	virtual void object_special_move(int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp);
	virtual void object_stop_special_move(int id, unsigned char dir, A3DVECTOR & cur_pos);
	virtual void player_change_name(int id, int name_len, char * name);
	virtual void change_faction_name(int id, int fid, char type, int name_len, char * name);
	virtual void enter_collision_raid(unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp);
	virtual void collision_special_state(int type, char on);
	virtual void collision_raid_result(char result, int score_a, int score_b);
	virtual void collision_raid_info(int score_a, int score_b, int win_num, int lost_num, int draw_num, int daily_award_timestamp, int last_collision_timestamp, 
			int day_win, int cs_personal_award_timestamp, int cs_exchange_award_timestamp, int cs_team_award_timestamp);

	//PK竞猜相关
	virtual void get_player_pk_bet_data();
	
	//光暗相关
	virtual void player_darklight();
	virtual void player_darklight_state();
	virtual void player_darklight_spirit();
	virtual void mirror_image_cnt(int cnt);
	
	virtual void player_wing_change_color(int id, char wing_color);
	virtual void ui_transfer_opened_notify();
	virtual void liveness_notify();

	virtual void notify_astrology_energy();
	virtual void astrology_upgrade_result(bool success, int new_level);
	virtual void astrology_destroy_result(int gain_energy);

	virtual void dir_visible_state(int id, int dir, bool on);
	virtual void player_remedy_metempsychosis_level();
	virtual void collision_player_pos(int roleid, A3DVECTOR & pos);
	virtual void enter_kingdom(unsigned char battle_faction, char kingdom_type, char cur_phase, int end_timestamp,int defender_mafia_id, std::vector<int> & attacker_mafia_ids); 
	virtual void leave_kingdom(char kingdom_type);
	virtual void kingdom_half_end(int win_faction, int end_timestamp);
	virtual void kingdom_end_result(int win_mafia_id);

	virtual void raid_level_start(char level);
	virtual void raid_level_end(char level, bool result);
	virtual void raid_level_result(bool final, bool result[10], bool final_award_deliveried, bool level_award_deliveried[10]);
	virtual void raid_level_info(char level, short matter_cnt, int start_time);
	virtual void sale_promotion_info(std::set<int> taskids);
	virtual void event_info();
	virtual void cash_gift_ids(int id1, int id2);
	virtual void notify_cash_gift(int gift_id, int gift_index, int gift_score); 

	virtual void player_propadd();
	virtual void player_get_propadd_item(int item_id, int item_idx);

	virtual void player_extra_equip_effect();
	virtual void kingdom_key_npc_info(int cur_hp, int max_hp, int hp_add, int hp_dec);
	virtual void notify_kingdom_info(int kingdom_title, int kingdom_point);
	virtual void cs_flow_player_info(int score, int c_kill_cnt, bool m_kill_change, int m_kill_cnt, short kill_cnt, short death_cnt, short max_ckill_cnt, short max_mkill_cnt);

	virtual void player_bath_invite(const XID & target); 
	virtual void player_bath_invite_reply(const XID & target, int param);
	virtual void player_bath_start(const XID & target); 
	virtual void player_bath_stop();

	virtual void enter_bath_world(int remain_bath_count);
	virtual void leave_bath_world();
	virtual void bath_gain_item(int item_id, int item_count);

	virtual void flow_battle_info(const char* buf, size_t size );
	virtual void fuwen_compose_info(int compose_count, int extra_compose_count); 
	virtual void fuwen_compose_end();
	virtual void fuwen_install_result(int src_index, int dst_index);
	virtual void fuwen_uninstall_result(int fuwen_index, int inv_index);

	virtual void multi_exp_mode(int multi_exp_factor, int timestamp);
	virtual void web_order_info();
	virtual void control_trap_info(int id, int tid, int time);
	virtual void puppet_form(bool on, int player_id, int puppet_id);
	virtual void teleport_skill_info(int player_id, int npc_tid, int skill_id, int teleport_count);

	virtual void player_mobactive_state_start(const XID & playerid, const XID & mobactive, int pos);
	virtual void player_mobactive_state_run(const XID & playerid, const XID & mobactive, int pos);
	virtual void player_mobactive_state_ending(const XID & playerid, const XID & mobactive, int pos);
	virtual void player_mobactive_state_finish(const XID & playerid, const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos);
	virtual void player_mobactive_state_cancel(const XID & playerid, const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos);

	virtual void player_qilin_invite(const XID & target); 
	virtual void player_qilin_invite_reply(const XID & target, int param); 
	virtual void player_qilin_start(const XID & target);
	virtual void player_qilin_stop(); 
	virtual void player_qilin_disconnect(); 
	virtual void player_qilin_reconnect(const A3DVECTOR & newpos);

	virtual void phase_info(); 
	virtual void phase_info_change(int phase_id, bool on);
	/*
	virtual void fac_building_add(int index, int tid);
	virtual void fac_building_upgrade(int index, int tid, int lev);
	virtual void fac_building_complete(int index, int tid, int level);
	virtual void fac_building_remove(int index);
	*/
	virtual void player_fac_base_info(const S2C::CMD::player_fac_base_info & base_info, const std::vector<S2C::INFO::player_fac_field> & field_info, const abase::octets & msg);

	virtual void player_active_emote_invite(const XID & target, int active_emote_type);
	virtual void player_active_emote_invite_reply(const XID& target, int param);
	virtual void player_active_emote_start(const XID& target,int active_emote_type);
	virtual void player_active_emote_stop(const XID& target, int active_emote_type);

	virtual void be_taunted2(const XID & who, int time);
	virtual void player_change_puppet_form();
	virtual void notify_bloodpool_status(bool on, int cur_hp, int max_hp);
	virtual void object_try_charge(char type, const A3DVECTOR & dest, int target_id);

	virtual void player_start_travel_around(int travel_vehicle_id, float travel_speed, int travel_path_id);
	virtual void player_stop_travel_around();
	virtual void cs6v6_cheat_info(bool is_cheat, int cheat_counter);

	virtual void hide_and_seek_raid_info(int last_hide_and_seek_timestamp, int day_has_get_award);

public:
	friend class gplayer_imp;
	virtual void LoadFrom(gplayer_dispatcher * rhs)		//从原有的dispatcher中取得数据
	{
		memcpy(&_header , &rhs->_header,sizeof(_header));
	}
protected:
	void enter_region();
};

class gplayer_controller : public controller
{
protected:
	XID  _cur_target;
	A3DVECTOR _last_pos;
	int  _load_stats;
	short _peep_counter;		//定时发送仇恨广播的定时器
	short _select_counter;		//定期进行查询对象是否存在的计数器
	bool  _debug_command_enable;	//特殊命令是否激活
	char  _banish_counter;
	unsigned short _move_cmd_seq;
	short  _deny_cmd_list[CMD_MAX];
	short  _pickup_counter;
	int    _pickup_timestamp;
	int    _safe_lock_timestamp;
	int    _max_safe_lock;
	GNET::Privilege * _gm_auth;
	enum 
	{
		MAX_PICKUP_PER_SECOND = 5, //最快每秒捡起五次
		STD_PICKUP_PER_SECOND = 2, //平均每秒捡起二次
	};
private:
	//放在这里禁止子类调用的目的是 禁止子类添加独特的需要保存的数据结构，使得各种结构保存均一致
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);


	virtual bool CheckBanish()
	{
		_load_stats ++;
		if(_banish_counter >0)
		{
			_load_stats += (_banish_counter >> 2) + 2;
			return false;
		}
		return true;
	}
	
	inline void IncreaseLoadStat(int offset)
	{
		_load_stats += offset;
	}

	inline bool CheckDeny(size_t  type)
	{
		return _deny_cmd_list[type];
	}
public:
	inline const XID & GetCurTarget() { return _cur_target;}
	void SetPrivilege(const void * data, size_t size);
	inline void SetDebugMode(bool is_debug)
	{
		_debug_command_enable = is_debug;
	}
	
	inline bool GetDebugMode()
	{
		return _debug_command_enable;
	}
	virtual bool HasGMPrivilege();

	inline void SetSafeLock(int locktime, int max_locktime)
	{
		_safe_lock_timestamp = locktime;
		_max_safe_lock= max_locktime;
		_imp->_runner->notify_safe_lock(locktime > 0?1:0, locktime, _max_safe_lock);
	}

	inline bool InSafeLock()
	{       
		return _safe_lock_timestamp > 0;
	}

	inline bool TestSafeLock()
	{
		if(InSafeLock())
		{
			error_cmd(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return false;
		}
		return true;
	}
	
	inline void DecreaseLoadStat(int offset)
	{
		_load_stats -= offset;
	}

public:
	DECLARE_SUBSTANCE(gplayer_controller);

	gplayer_controller();
	~gplayer_controller();

	virtual void Init(gobject_imp * imp);
	virtual void ReInit();
	
	virtual int GetCurMoveSeq() 
	{ 
		return _move_cmd_seq;
	}

	virtual int GetNextMoveSeq() 
	{ 
		_move_cmd_seq = (_move_cmd_seq + 1 ) & 0xFFFF;
		return _move_cmd_seq;
	}

	virtual void SetNextMoveSeq(int seq) 
	{ 
		_move_cmd_seq = seq;
	}

	virtual int MessageHandler(const MSG & msg);

	virtual int DebugCommandHandler(int cmd_type,const void * buf, size_t size);
	virtual int GMCommandHandler(int cmd_type,const void * buf, size_t size);
	//各种状态下的命令处理函数
	virtual int UnLockInventoryHandler(int cmd_type,const void * buf, size_t size);
	virtual int CommandHandler(int cmd_type,const void * buf, size_t size);
	virtual int InvalidCommandHandler(int cmd_type, const void * buf, size_t size);

	virtual int MoveBetweenSlice(gobject * obj,slice * src, slice * dest);
	virtual void Release(bool free_parent);	//这个函数会忽视free_parent
	virtual	void OnHeartbeat(size_t tick);
	virtual void DenyCmd(size_t cmd_type);
	virtual void AllowCmd(size_t cmd_type);

	void ResurrectByItem(float exp_reduce);
	void ResurrectInTown(float exp_reduce);
public:
	void OnResurrect();
protected:
	void TryPeepMobs();
	void error_cmd(int msg);
	int cmd_user_move(const void * buf, size_t size, bool on_carrier);
	int cmd_user_stop_move(const void * buf, size_t size, bool on_carrier);
	friend class gplayer_imp;
	
public:
	virtual	void LoadFrom(gplayer_controller * rhs)		//从原有的controller中取得数据
	{
		_cur_target = rhs->_cur_target;
		_last_pos = rhs->_last_pos;
		_peep_counter = rhs->_peep_counter;
		_select_counter = rhs->_select_counter;
		_load_stats = rhs->_load_stats;
		_debug_command_enable = rhs->_debug_command_enable;
		_banish_counter = rhs->_banish_counter;
		_move_cmd_seq = rhs->_move_cmd_seq;
		_safe_lock_timestamp = rhs->_safe_lock_timestamp;
		_max_safe_lock =  rhs->_max_safe_lock;
		
		memcpy(_deny_cmd_list,rhs->_deny_cmd_list,sizeof(_deny_cmd_list));

		ASSERT(!_gm_auth);
		_gm_auth = rhs->_gm_auth;
		rhs->_gm_auth = NULL;
	}
	
	//内部调用的函数
	template < typename T>
	inline void TrySelect(const T & attacker)
	{
		if(_cur_target.type == -1)
		{
			SelectTarget(attacker.id);
		}
	}

	void UnSelect();
	void SelectTarget(int id,bool client_cmd = false);
	void SubscibeConfirm(const XID & who);
};

class gplayer_imp;

//Web商城对应的商品信息
struct web_order_goods
{
	int _goods_id;
	int _goods_count;
	int _goods_flag; //0x01:绑定
	int _goods_time;
};

//Web商城对应的一个礼包
struct web_order_package
{
	int _package_id;
	char _package_name[32];
	size_t _name_len;
	int _count;
	int _price;
	std::vector<web_order_goods> _order_goods;
};

//Web商城对应的一次订单
struct web_order
{
	int64_t _order_id;
	int _userid;
	int _roleid;
	int _pay_type;	//1:coupon 2:cash
	int _status;	//0:未领取 1:已经领取
	int _timestamp;
	abase::vector<web_order_package> _order_package;

};

struct player_mallinfo
{
	int	  _mall_cash;			//原来可用点数
	int	  _mall_cash_used;		//总共用了多少点数
	int	  _mall_cash_offset;		//消耗了多少点数
	int	  _mall_order_id;		//商场购物的流水号
	int 	  _mall_order_id_saved;		//商场购物的流水号，最近一次存盘的流水号
	int	  _mall_cash_add2;		//总共充值金额
	int	  _mall_cash_web;		//web商城需要花费的cash金额
	abase::vector<qgame::mall_invoice> _mall_order_list;
	abase::vector<web_order> _web_order_list;
public:	
	player_mallinfo(): _mall_cash(0), _mall_cash_used(0), _mall_cash_offset(0), _mall_order_id(0), _mall_order_id_saved(0), _mall_cash_add2(0),_mall_cash_web(0)
	{}

	inline void SetInfo(int cash, int cash_used, int delta, int order_id, int cash_add2)
	{
		_mall_cash = cash;
		_mall_cash_used = cash_used;
		_mall_cash_offset = delta;
		_mall_order_id = order_id;
		_mall_order_id_saved = order_id;
		_mall_cash_add2 = cash_add2;
	}

	inline void GetInfo(int & cash, int &cash_used,int & delta, int & order_id)
	{
		cash = _mall_cash;
		cash_used = _mall_cash_used;
		delta = _mall_cash_offset;
		order_id = _mall_order_id;
	}

	inline void CalcWebCash()
	{
		_mall_cash_web = 0;
		for(size_t i = 0; i < _web_order_list.size(); ++i)
		{
			if(_web_order_list[i]._pay_type == 2 && _web_order_list[i]._status == 0)
			{
				for(size_t j = 0; j < _web_order_list[i]._order_package.size(); ++j)
				{
					_mall_cash_web += (_web_order_list[i]._order_package[j]._count *  _web_order_list[i]._order_package[j]._price);
				}
			}	
		}
	}

	inline void SetWebOrder(const std::vector<GDB::weborder> & weborders)
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
				const GDB::webmallfunc & w_mall_func = weborders[i].funcs[j];
				web_order_package w_order_package;
				w_order_package._package_id = w_mall_func.id;
				w_order_package._name_len = w_mall_func.name.length();
				if(w_order_package._name_len > 32)  w_order_package._name_len = 32;
				memcpy(w_order_package._package_name,w_mall_func.name.c_str(),  w_order_package._name_len);
				w_order_package._count = w_mall_func.count;
				w_order_package._price = w_mall_func.price;

				for(size_t k = 0; k < w_mall_func.goods.size(); ++k)
				{
					const GDB::webmallgoods & w_goods = w_mall_func.goods[k];
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
		}
		CalcWebCash();

	}

	void AddWebOrder(const std::vector<GMSV::weborder> & weborders, gplayer_imp * pImp);

	inline void GetWebOrder(std::vector<int64_t> & processed_weborders)
	{
		for(size_t i = 0; i < _web_order_list.size(); ++i)
		{
			if(_web_order_list[i]._status == 1)
			{
				processed_weborders.push_back(_web_order_list[i]._order_id);
			}
		}
	}

	inline int GetCash()
	{
		return _mall_cash + _mall_cash_offset - _mall_cash_web;
	}

	//没有扣web订单
	inline int GetCash2()
	{
		return _mall_cash + _mall_cash_offset;
	}

	inline int GetCashUsed()
	{
		return _mall_cash_used;
	}

	inline int GetCashAdd()
	{
		return _mall_cash_add2;
	}

	inline int GetCashWeb()
	{
		return _mall_cash_web;
	}

	inline bool NeedSave()
	{
		return _mall_order_id != _mall_order_id_saved;
	}
	
	inline void SetCurCash(int cash)
	{
		_mall_cash = cash;
	}

	inline void SetCashPlusUsedValue(int cash_plus_used)
	{
		_mall_cash = cash_plus_used - _mall_cash_used;
	}

	inline void SetCashAdd(int cash_add2)
	{
		_mall_cash_add2 = cash_add2;	
	}

	void UseCash(int offset, gplayer_imp * pImp);

	inline void SaveDone(int id)
	{
		_mall_order_id_saved = id;
		size_t i = 0;
		for(i = 0;i  < _mall_order_list.size(); i ++)
		{
			const qgame::mall_invoice & mi =  _mall_order_list[i];
			if(mi.order_id - id >= 0) break;
		}

		if(i == _mall_order_list.size()) 
		{
			_mall_order_list.clear();
		}
		else if( i > 0)
		{
			_mall_order_list.erase(_mall_order_list.begin(), _mall_order_list.begin() + i);
		}
	}

	inline int GetOrderID()
	{
		return _mall_order_id;
	}

	inline void IncOrderID()
	{
		_mall_order_id ++;
	}

	inline void AddOrder(int order_id, int item, int scount, int price, int expire_date, int total_unit,int guid1 = 0,int guid2 = 0)
	{
		_mall_order_list.push_back(qgame::mall_invoice(order_id, item, scount, price, expire_date, total_unit,g_timer.get_systime(),guid1,guid2));
	}

	inline size_t GetOrderCount() const
	{
		return _mall_order_list.size();
	}

	inline void GetOrder(abase::vector<GDB::shoplog, abase::fast_alloc<> > & list) const
	{
		list.reserve(_mall_order_list.size());
		GDB::shoplog sp;
		for(size_t i = 0;i  < _mall_order_list.size(); i ++)
		{
			const qgame::mall_invoice & mi =  _mall_order_list[i];
			sp.order_id	= mi.order_id;
			sp.item_id	= mi.item_id;
			sp.expire	= mi.expire_date;
			sp.item_count	= mi.item_count;
			sp.order_count	= mi.total_unit;
			sp.cash_need	= mi.total_cash;
			sp.time		= mi.timestamp;
			sp.guid1	= mi.guid1;
			sp.guid2	= mi.guid2;
			list.push_back(sp);
		}
	}

	inline int GetOrder(GDB::shoplog * pLog, size_t size) const
	{
		if(size < _mall_order_list.size()) return -1;
		for(size_t i = 0;i  < _mall_order_list.size(); i ++)
		{
			const qgame::mall_invoice & mi =  _mall_order_list[i];
			GDB::shoplog &sp =pLog[i];

			sp.order_id	= mi.order_id;
			sp.item_id	= mi.item_id;
			sp.expire	= mi.expire_date;
			sp.item_count	= mi.item_count;
			sp.order_count	= mi.total_unit;
			sp.cash_need	= mi.total_cash;
			sp.time		= mi.timestamp;
			sp.guid1	= mi.guid1;
			sp.guid2	= mi.guid2;
		}
		return (int)_mall_order_list.size();
	}

	inline void Swap(player_mallinfo & rhs)
	{
		abase::swap(_mall_cash          ,rhs._mall_cash          );
		abase::swap(_mall_cash_used     ,rhs._mall_cash_used     );
		abase::swap(_mall_cash_offset   ,rhs._mall_cash_offset   );
		abase::swap(_mall_order_id      ,rhs._mall_order_id      );
		abase::swap(_mall_order_id_saved,rhs._mall_order_id_saved);
		abase::swap(_mall_cash_add2 ,rhs._mall_cash_add2);
		abase::swap(_mall_cash_web ,rhs._mall_cash_web);
		_mall_order_list.swap(rhs._mall_order_list);
		_web_order_list.swap(rhs._web_order_list);
	}
};

struct player_bonusinfo
{
	int _bonus_withdraw;
	int _bonus_reward;
       	int _bonus_used;

	player_bonusinfo() : _bonus_withdraw(0), _bonus_reward(0), _bonus_used(0)
	{}

	inline void SetInfo(int bonus_withdraw, int bonus_reward, int bonus_used)	
	{
		_bonus_withdraw = bonus_withdraw;
		_bonus_reward = bonus_reward;
		_bonus_used = bonus_used;
	}	
	
	inline void GetInfo(int bonus_withdraw, int & bonus_reward, int & bonus_used)
	{
		bonus_withdraw = _bonus_withdraw;
		bonus_reward = _bonus_reward;
		bonus_used = _bonus_used;
	}

	inline int GetBonus()
	{
		return _bonus_withdraw + _bonus_reward - _bonus_used;	
	}

	inline int GetBonusUsed()
	{
		return _bonus_used;
	}
	
	inline void UseBonus(int count)
	{
		_bonus_used += count;
	}

	inline void AddBonus(int inc, char type = 0)
	{
		if(inc <= 0) return;

		//通过自己做任务或者元宝购物获得
		if(type == 0)
		{
			_bonus_reward += inc;	
		}
		else if(type == 1)
		{
			_bonus_withdraw += inc;
		}
		else
		{
		}
	}
		
	inline void Swap(player_bonusinfo & rhs)
	{
		abase::swap(_bonus_withdraw, rhs._bonus_withdraw);
		abase::swap(_bonus_reward, rhs._bonus_reward);
		abase::swap(_bonus_used, rhs._bonus_used);
	}
	
	
};

struct move_control
{
	int start_seq;
	int session_id;
	timeval val_start;
	timeval val_end;

	inline static int MilliRange(const timeval &start, const timeval &end)
	{
		return (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)/1000;
	}
public:
	move_control():start_seq(-1),session_id(-1){}
	bool IsInValid(int seq,int sid) 
	{
		return (start_seq < 0 || start_seq != seq)
			|| ( session_id < 0 || sid - session_id > 2);
			
	}

	int GetMilliRange()
	{
		return MilliRange(val_start, val_end); 
	}

};

class phase_control		//这是另一个级别的移动控制
{
	enum
	{
		STATE_GROUND,
		STATE_JUMP,
		STATE_FALL,
		STATE_FLY,
	};
	
	int   state;		//当前状态，可能是 站立（地面？）悬空(跳起/跌落)
	float jump_distance;	//跳起的距离和高度和
	int   jump_time;	//跳起的时间和
	float drop_speed;	//跌落速度控制
public:
	phase_control()
	{
		state = STATE_GROUND;
		jump_distance 	= 0;
		jump_time = 0;
	}

	void Swap(phase_control & rhs)
	{
		phase_control tmp(*this);
		*this = rhs;
		rhs = tmp;
	}
	
	void Initialize(gplayer_imp * pImp);

	bool PhaseControl(gplayer_imp * pImp, const A3DVECTOR & target, float theight, int mode, int use_time);	//每次移动后的状态，本次移动如若成功 会更新自己的状态
	bool OnGround()
	{
		return state == STATE_GROUND;
	}

	void SetGround()
	{
		state = STATE_GROUND;
	}
	
};

struct vip_card
{
	float exp_ratio;
	bool free_bot;
	bool no_exp_drop;
};


enum CHANGE_GS_REASON //与gdeliveryd localmacro.h 同步修改
{
	CHG_GS_PLAYER_LEAVE = 0,	//普通切换gs
	CHG_GS_LEAVE_BY_VOTE = 1,	//副本投票踢出玩家
	CHG_GS_ENTER_COLLISION_RAID = 2,	//进入碰撞副本
	CHG_GS_LEAVE_RAID = 3,
	CHG_GS_ENTER_FBASE = 4, //进入帮派基地
};

// Youshuang add
enum NEWYEAR_AWARD_TYPES
{
	NEWYEAR_AWARD_FETCH_INFO = 0,
	NEWYEAR_AWARD_DAILY = 1,
	NEWYEAR_AWARD_FIRST = 2,
	NEWYEAR_AWARD_LAST = 4
};

struct newyear_award_info
{
        int score;
        int last_fetch_date;
	char fetch_level[ NEWYEAR_AWARD_LAST - NEWYEAR_AWARD_FIRST + 1 ];
};
// end

class player_trade;
class player_stall;
class player_transform;
class player_consign;
class gplayer_imp : public gactive_imp 
{
protected:
	struct hurt_entry
	{
		int team_id;
		int team_seq;
		int damage;
		hurt_entry():team_id(-1),team_seq(0),damage(0){}
	};
	typedef abase::hash_map<XID,hurt_entry,XID_HashFunc, abase::fast_alloc<> > DAMAGE_MAP;
	typedef abase::static_multimap<int, int, abase::fast_alloc<> >  ENEMY_LIST;
	typedef S2C::CMD::notify_fashion_hotkey::key_combine fashion_hotkey;
	instance_key	_instance_switch_key;	//进行副本转移时使用的key
	int		_battle_faction;	//战场进入时使用的攻守方标志
	size_t		_battle_score;		//战场积分
	size_t		_killingfield_score;	//杀戮战场积分
	size_t		_arena_score;		//竞技场积分

	int 	_raid_faction;		//副本进入时使用的攻守方标志
	
	item_list 	_inventory;		//物品栏
	item_list	_equipment;		//装备栏
	item_list	_task_inventory;	//任务物品栏
	item_list	_pet_bedge_inventory;	//宠物牌包裹栏
	item_list	_pet_equip_inventory;	//宠物装备包裹栏
	item_list	_pocket_inventory; 	//随身包裹
	item_list 	_fashion_inventory;	//时装包裹
	item_list   	_mount_wing_inventory;  //坐骑、飞剑包裹
	item_list   	_gift_inventory;  	//元宝赠品包裹
	item_list   	_fuwen_inventory;  	//符文包裹
	std::list<item> _repurchase_inventory;  //物品回购包裹，不存盘
	size_t 		_player_money;		//钱数
	size_t		_money_capacity;	//钱数上限	
	player_state	_pstate;		//玩家的基本状态
	int		_combat_timer;		//计算最近一次格斗的时间
	player_team	_team;			//队伍数据
	player_duel	_duel;			//决斗数据
	mafia_duel	_mduel;			//帮派野战
	faction_hostile_duel _fh_duel;          //敌对帮派信息
	player_bind	_bind_player;		//相依相偎绑定
	player_link_ride  _link_ride_player; 	//多人骑乘绑定
	player_link_bath  _link_bath_player; 	//多人泡澡
	player_link_qilin _link_qilin_player; 	//麒麟邀请
	player_trade  * _trade_obj;		//交易对象，只有交易状态下生效
	player_stall  * _stall_obj;		//摆摊对象，只有摆摊的时候才生效
	player_consign* _consign_obj;	//寄售对象，只有寄售状态下生效
	player_transform * _transform_obj;	//变身对象, 只有变身的时候才生效
	int		_stall_trade_id;	//摆摊次数，用于校验摆摊的正确性
	int		_stall_trade_timer;	//记录上次摆摊的时间戳，必须有差额才能摆摊 不需要再切换中保存
	int		_last_move_mode;	//上一次的移动模式
	ENEMY_LIST _enemy_list;			//敌人列表，即自己处于那些怪物的仇恨度列表头部
	player_active_emote _active_emote_player;   //互动表情

	DAMAGE_MAP _dmg_list;
	struct
	{
		unsigned int mask;
		abase::octets data;
	} _equip_info;				//装备信息 即需要其他人知道的装备类型，供发给客户端显示用

	int _fashion_weapon_addon_id;		// Youshuang add, set when fashion weapon is used
	int _faction_coupon;			// Youshuang add, 新帮派贡献度
	int64_t _fac_coupon_add;		// Youshuang add
	bool _fac_coupon_added;			// Youshuang add
	newyear_award_info _newyear_info;	// Youshuang add
	int _used_title;			// Youshuang add
	
	struct
	{
		XID  id;
		A3DVECTOR pos;
		int id_mafia;
	} _provider;				//当前给自己提供服务的商人的坐标和id  


	unsigned char _inv_level;		//包裹栏的等级，是变化量 ,这个值决定了包裹栏的数量和金钱的最大上限 
	bool	_is_moved;			//给gnpc_controller使用的_is_moved
	bool	_pvp_flag_enable;		//是否开启了PVP标志 30级以后的玩家会被自动开启，这是一个隐藏标志
	unsigned char _write_counter;		//在本次服务器上写入的次数 目前没什么用
	bool	_kill_by_player;		//是否被玩家杀死的
	bool	_free_pvp_mode;			//是否在竞技场模式  这个在切换服务器时不保存
	char	_invader_counter;		//粉名计时时间
	bool    _sanctuary_mode;                //是否在安全区
	unsigned short _eq_change_counter;	//装备更改标志，每次更改装备这个值会加一，如果为0则表示已经写入
	char	_chat_emote;			//聊天表情ID

	float	_resurrect_exp_reduce;		//原地复活时减少的exp数目
	float	_resurrect_hp_recover;		//原地复活时回复的血值和魔法值
	int	_pvp_combat_timer;		//是否出于pvp中 以及pvp中的剩余时间(秒)

	bool	_resurrect_state;		//是否可以原地复活的标志
	int 	_resurrect_timestamp;
	unsigned char _logout_pos_flag;		//是否使用特殊坐标登出
	char      _cheat_punish;                //作弊惩罚
	char      _cheat_mode;                  //作弊标志
	char      _cheat_report;                //作弊记录
	A3DVECTOR _logout_pos;			//登出时使用的坐标
	int	  _logout_tag;			//登出时使用的tag
	int	  _logout_time;			//上次登出时间
	int   _create_time;         //角色创建时间

	A3DVECTOR _last_instance_pos;		//最后一次进入副本后的坐标位置 无需保存
	int	  _last_instance_tag;		//最后一次进入副本后的世界ID   无需保存
	int 	  _last_instance_timestamp;	//最后一次进入副本的时间戳     无需保存
	A3DVECTOR _last_source_instance_pos;	//最后一次进入副本后的坐标位置 无需保存
	int	  _last_source_instance_tag;	//最后一次进入副本后的世界ID   无需保存
	int	  _db_save_error;		//数据存盘错误计数
	int	  _db_timestamp;		//数据库存盘计数，这个很重要
	char	  _username[MAX_USERNAME_LENGTH];	//玩家的名字，最大10个unicode字符
	size_t	  _username_len;		//玩家名字的长度，以字节为单位
	
	int	  _double_exp_mode;		//双倍时间的类型 0 无，1 大卡 2 物品 3 其他
	int	  _rest_counter_time;		//上次计算双倍时间的计时点
	int 	  _rest_time_used;		//本计时周期内已经使用的计时点
	int	  _rest_time_capacity;		//目前可用多少双倍时间
	int	  _mafia_rest_time;		//帮派奖励的双倍时间 这个时间和上面的时间和起来是最终可用的双倍时间
	int	  _mafia_rest_counter_time;	//上次计算帮派奖励的时间点
	char 	  _mafia_name[MAX_MAFIA_NAME_LENGTH];
	size_t 	  _mafia_name_len;

	int	  _login_timestamp;		//登录时的时间戳
	int	  _played_time;			//本角色已经进行游戏的时间
	int	  _spec_task_reward;		//数据库发奖
	int	  _spec_task_reward_param;	//数据库发奖的参数
	int	  _fill_info_mask;		//平台账号登陆补填账号信息
	float	  _speed_ctrl_factor;		//限制速度的统计因子，意义是当前最近用过的最大合法速度此值无需存盘
	int	  _duel_target;			//决斗时的目标，传送给宠物使用的
	int	  _idle_player_state;		//idle seal root 封印的等效状态，给命令处理和消息处理使用的
	int	  _talent_point;		//天赋点数
	unsigned int _db_magic_number;
	unsigned int _ip_address;

	abase::static_set<unsigned short> _recipe_list;	//玩家身上掌握的配方列表
	int	  _produce_level;		//生产级别
	int	  _produce_exp;			//生产经验

	cd_manager _cooldown;
	pet_manager _petman;			//宠物管理
	summon_manager _summonman;		//召唤管理
	mobactive_manager _mobactiveman;        //互动物品

	player_mallinfo _mallinfo;		//玩家的点卡和百宝阁信息
	player_bonusinfo _bonusinfo;		//玩家的鸿利点数信息
	int 	  _referrer_id;			//玩家推广上线的ID, 无上线此值为0
	int 	  _billing_cash;		//韩国billing系统使用的元宝值
	int	  _mafia_contribution;		//帮派贡献度
	int	  _mafia_join_time;
	int 	  _family_contribution;		//家族贡献度
	int	  _recorder_timestamp;		//录像时间戳
	int	  _another_day_timestamp;	//每天晚上凌晨12点的时间戳

	abase::vector<int, abase::fast_alloc<> > _reborn_list;          //转生列表 高16位是 转生等级 低16位是转生职业id

	abase::static_set<short, abase::fast_alloc<> > _title_list;	//玩家的称谓列表
	abase::static_set<short, abase::fast_alloc<> > _extra_title;	//玩家的称谓列表
	int	  _region_reputation[REPUTATION_REGION];
	int	  _sect_id;
	int	  _sect_init_level;	//拜师级别

	int      _wallow_level;         //沉迷等级
	//wallow_object _wallow_obj;      //沉迷时间控制对象
	
	player_achieve_man	_achieve_man;	//成就管理
	struct achieve_cache
	{
		size_t money;
		int pk_value;

	} _achieve_cache;
	int _achieve_timer;
	
	friend class gplayer_controller;
	friend class gplayer_dispatcher;

	struct
	{
		int   talisman_value;	//法宝熔炼值
		int   stop_bot_time;	//停止法宝天人合一的时间点
		bool  talisman_state;	//法宝是否生效
		bool  bot_state;	//天人合一状态
		float stamina;		//法宝精力
		float max_stamina;	//法宝最大精力
		float energy_recover_speed;  
		float energy_recover_factor; 
		float energy_drop_speed;     
		bool need_question;	// 是否想答题
		struct			// 挂机时使用的buff和附加属性
		{
			bool addon_exist_flag;		// 是否存在附加属性标志
			bool addon_active_flag;		// 是否激活附加属性标志
			int buff_id;			// buff id
			int addon_id[2];		// 附加属性 id
		} autobot_arg;
	} _talisman_info;
	
	struct
	{
		float	speed;		//飞行器的数据
		int	exp_add;	//每30秒增加的飞剑经验
		char	state;		//是否装备了飞行器
		bool	active;		//是否飞起来了
		int 	stamina_cost;	//飞行每秒消耗
	} _fly_info;

	
	abase::vector<fashion_hotkey, abase::fast_alloc<> > _fashion_hotkey;  
	vip_card _vipstate;
	char _pvp_mask;

	struct
	{
		char state;		// 0 未开启   1 开启	
		char color[5]; 
	} _combo_skill;

	player_circle_of_doom		_circle_of_doom;	//阵法信息
	vipgame::player_vip_award	_vip_award;			//vip奖励，vip等级为0则表示是普通玩家,所有玩家都会发放普通奖励
	player_online_award			_online_award;		//在线奖励


	//囚牢限制移动的数据结构
	struct cycle_area
	{
		A3DVECTOR center;
		float squared_radius;
	};

	typedef abase::hash_map<int, cycle_area> CycleAreaMap;	
	CycleAreaMap _limit_area;	//玩家不能移动的圆形区域
	CycleAreaMap _permit_area;	//玩家能够移动的圆形区域

	//牵引和拉扯对象
	int id_spirit_drag_me;	//被谁牵引
	int id_pulling_me;		//被谁拉扯

	//玩家副本进入次数
	struct raid_counter
	{
		int raid_id;
		int counter;
	};
	typedef abase::hash_map<int, raid_counter> RaidCounterMap;	

	RaidCounterMap _raid_counters;
	int     _time_raid_counters_reset;	//上一次重置副本计数器的时间

	//ZX 5 years anniverisary code, delete later. Add by Houjun 20120320
	enum
	{
		ANNI_SIGN_INIT = 0,
		ANNI_SIGN_CONGRATUATED,
		ANNI_SIGN_AWARDDELIVERIED,
	};
	char _annu_five_star;
	int  _time_annu_five_award;
	char _annu_sign_state;


	//挖宝区域类型
	enum TREASURE_REGION_TYPE
	{
		TREASURE_TYPE_INVALID,
		TREASURE_TYPE_NORMAL,
		TREASURE_TYPE_HIDDEN,
	};
	
	//挖宝区域状态
	enum TREASURE_REGION_STATUS
	{
		TREASURE_STATUS_INVALID,
		TREASURE_STATUS_OPEN,
		TREASURE_STATUS_CLOSE,		
		TREASURE_STATUS_LOCK,		
	};
	
	//挖宝区域
	struct treasure_region
	{
		char type;			//区域类型
		char status;			//区域状态
		char level;			//区域等级
		int id;				//区域id
	};

	struct treasure 
	{	
		int version;
		int reset_timestamp;		//重置挖宝区域的计数器(不存盘)
		int update_timestamp;		//更新挖宝区域的时间戳
		int remain_dig_count;		//可以挖宝的次数
		treasure_region region[MAX_TREASURE_REGION];		//区域属性
	} _treasure_info;

	struct little_pet
	{
		char cur_level;
		char cur_award_level;
		int cur_exp;
	}_little_pet;

	bool _active_rune;			//元魂装备是否激活
	int _active_rune_level;		//激活元魂装备属性的等级
	int _rune_score;		//元魂洗练值

	unsigned char _flag_mask;

	//爬塔副本相关玩家数据
	struct tower_level_info
	{
		int best_pass_time;
		char daily_reward_delivered;
		char lifelong_reward_delivered;
	};

	//UI传送 -- 玩家数据
	struct ui_transfer_info
	{
		abase::static_set<int> opened_indexes;
		void Swap(ui_transfer_info& rhs) 
		{
			opened_indexes.swap(rhs.opened_indexes);				
		}
	} _ui_transfer_info;

	//国家职位
	int _kingdom_title;

	struct fuwen_compose
	{
		int compose_count;
		int extra_compose_count;
	}_fuwen_compose_info;

	struct item_multi_exp
	{
		int cur_multi_exp;
		int cur_multi_exp_timestamp;
		int multi_exp_timestamp[10];
	}_item_multi_exp;

	int _puppet_immune_prob;
	int _puppet_skill_id;

	//相位信息
	abase::hash_map<int, bool> _phase_info;
	struct fac_multi_exp //帮派多倍经验
	{
		int multi; //倍数 > 1 有效
		int end_timestamp; //失效时间
		//注意：目前是 memset 初始化的
	}_fac_multi_exp;
public:
	//UI传送 -- 玩家数据
	enum {
		LIVENESS_UPDATE_TYPE_TASK_DONE = 0,     // 完成任务
		LIVENESS_UPDATE_TYPE_TRANSFER_MAP,		// 传送地图
		LIVENESS_UPDATE_TYPE_SPECIAL,			// 特殊类型，子类型见下面枚举定义
		LIVENESS_UPDATE_TYPE_NUM,
	};
	enum {
		LIVENESS_SPECIAL_LEAST_AUCTION = 1,     // 1.参加最低拍卖
		LIVENESS_SPECIAL_FABAO = 2,  		    // 2.法宝(灌魔，血炼，归元，飞升，融合，传承)
		// 宠物驯养、宠物装备强化、星相修炼、外观修炼、宠物改名、改变前世、改变特质、斗气修炼、舍利修炼、宠物放生、改变限界、资质修炼、扩充技能、修炼技能
		LIVENESS_SPECIAL_CHONGWU_XIULIAN = 3,   // 3.宠物修炼
		LIVENESS_SPECIAL_EQUIP_XUELIAN = 4,     // 4.装备血炼
		LIVENESS_SPECIAL_YUANYING = 5, 		   	// 5.元婴
		LIVENESS_SPECIAL_FEITIANFU = 6,     	// 6.飞天符
		LIVENESS_SPECIAL_LIANDANLU = 7,     	// 7.炼丹炉，彩票
		LIVENESS_SPECIAL_VIP_CARD_OPEN = 8, 	// 8.包裹中有开启的VIP卡
		LIVENESS_SPECIAL_WANBI_FU = 9, 	   		// 9.完璧符
		LIVENESS_SPECIAL_HUAZHOU_QIAN = 10, 	// 10.化咒签
		LIVENESS_SPECIAL_JUEDOU = 11,			// 11.决斗
		LIVENESS_SPECIAL_XYXW = 12,				// 12.相依相偎
		LIVENESS_SPECIAL_TEAM = 13,				// 13.组队
		LIVENESS_SPECIAL_TALK_WITH_FRIEND = 14,	// 14.跟好友对话
		LIVENESS_SPECIAL_STALL_AND_SELL = 15,	// 15.摆摊并卖出物品
		LIVENESS_SPECIAL_TRHY = 16,				// 16.天人合一
		LIVENESS_SPECIAL_FEIJIAN = 17,			// 17.驾驭飞剑
		LIVENESS_SPECIAL_HUANHUASHI = 18,		// 18.幻化成功
		LIVENESS_SPECIAL_CB_ZHANCHANG_WIN = 19,	// 19.战场胜利
	};

	enum FBASE_CMD_RET
	{
		FBASE_CMD_SUCCESS, //基地命令处理完毕 无需再转发
		FBASE_CMD_FAIL, //基地命令处理失败 无需转发
		FBASE_CMD_DELIVER,//基地命令检查通过 需要转发给基地 gs 进行处理
	};

protected:
	struct liveness_info
	{
		int grade_timestamp[4];    			// 是否领取该档次的奖品[目前存时间戳]
		int midnight_clear_timestamp; 	    // 零点截止时间戳
		int cur_point; 					    // 当前的分数
		std::multimap<int, int>  classifyid2ids; // 分类id --> 活跃度模板id

		int cont_days;						// 连续天数
		int cont_days_timestamp;			// 连续天数时间戳
		int debug_needday;					// 非零表示，设置了调试几天连续

		liveness_info() : midnight_clear_timestamp(0), cur_point(0), cont_days(0), cont_days_timestamp(0), debug_needday(0)
		{
			memset(grade_timestamp, 0, sizeof(grade_timestamp));	
		}

		void Swap(liveness_info& rhs) {
			midnight_clear_timestamp	= rhs.midnight_clear_timestamp;
			cur_point = rhs.cur_point;
			cont_days = rhs.cont_days;
			cont_days_timestamp = rhs.cont_days_timestamp;
			debug_needday = rhs.debug_needday;
			memcpy(grade_timestamp, rhs.grade_timestamp, sizeof(grade_timestamp));
			for (std::multimap<int, int>::iterator it = rhs.classifyid2ids.begin(); it != rhs.classifyid2ids.end(); ++it) {
				classifyid2ids.insert(std::make_pair(it->first, it->second));	
			}
		}
	} _liveness_info;

	// 台历相关信息，记录台历相关的任务完成情况
	struct sale_promotion_info {
		std::set<int> taskids;

		void Swap(sale_promotion_info& rhs) {
			for (std::set<int>::iterator it = rhs.taskids.begin(); it != rhs.taskids.end(); ++it) {
				taskids.insert(*it);
			}
		}
	} _sale_promotion_info;

	typedef abase::hash_map<int, char> TowerMonsterMap;
	struct tower_raid_info
	{
		int tower_level;		//当前塔的层数
		int start_level_today;
		int end_level_today;
		int tower_daily_reward_reset_time;
		tower_level_info levels[MAX_TOWER_RAID_LEVEL];
		TowerMonsterMap tower_monster_list;

		tower_raid_info()
		{
			tower_level = -1;
			start_level_today = -1;
			end_level_today = -1;
			tower_daily_reward_reset_time = 0;
			memset(levels, 0, sizeof(levels));
		}
		
		void Swap(tower_raid_info& tri)
		{
			tower_level = tri.tower_level;
			start_level_today = tri.start_level_today;
			end_level_today = tri.end_level_today;
			tower_daily_reward_reset_time = tri.tower_daily_reward_reset_time;
			memcpy(levels, tri.levels, sizeof(levels));
			abase::swap(tower_monster_list, tri.tower_monster_list);
		}
	} _tower_raid_info;
	
	struct pk_bet
	{
		int bet_1st_num;
		bool top3;
	} _pk_bets[8];
	bool _1st_bet;
	bool _top3_bet;
	bool _1st_bet_reward_deliveried;
	bool _1st_bet_reward_result_deliveried;
	bool _top3_bet_reward_deliveried;
	bool _top3_bet_reward_result_deliveried[3];

	enum
	{
		DLFORM_NORMAL = 0,
		DLFORM_JUNIOR_DARK,
		DLFORM_JUNIOR_LIGHT,
		DLFORM_SENIOR_DARK,
		DLFORM_SENIOR_LIGHT,
		DLFORM_DARKUNIFORM,
		DLFORM_LIGHTUNIFORM,
	};

	int  _dark_light;				//轩辕光暗度
	int  _dark_light_form;			//轩辕光暗形态
	typedef abase::hash_map<int, act_session*> SpiritSessionMap;	
	SpiritSessionMap _spirit_session_list;

	struct astrology
	{
		bool is_active;		//当前装备的精力效果是否激活
		bool client_active;	//是否要激活显示精力值(星座鉴定后激活，激活后客户端显示精力条)
		int  counter;		//检查是否要激活和扣除精力的计数器
		int cur_energy;		//当前精力
		int consume_speed;	//星座精力消耗速度
		int reservered[2];
	}_astrology_info;

	//碰撞战场已经被取消
	//现在的随机匹配战场使用了原来碰撞战场的存盘数据和一些逻辑
	//跨服的6V6存盘也是用这个结构
	struct collision
	{
		int collision_win;	//碰撞战场胜局
		int collision_lost;	//碰撞战场败局
		int collision_draw;	//碰撞战场平局
		int collision_score_a;	//碰撞积分A
		int collision_score_b;	//碰撞积分B
		int collision_timestamp;//碰撞战场时间戳(上次战场结束的时间)
		int daily_award_timestamp;	//上次领取每日大奖的时间戳
		short day_win;		//当天的胜场数
		short day_score_a;	//当天获得的积分a
		short day_score_b;	//当天获得的积分b
		int collision_score;	//积分(匹配的时候使用), 基础积分需要加上1000分为玩家真实积分

		int cs_personal_award_timestamp;	//领取战队6V6个人奖励的时间
		int cs_team_season_award_timestamp;	//领取战队6V6赛季奖励的时间
		int cs_weekly_exchange_award_timestamp; //领取战队6V6每周兑换币奖励
		int cs_exchange_item_timestamp;		//战队6V6通过兑换币兑换物品的时间

		int reservered[3];	//保留数据
	}_collision_info;

	abase::hash_map<int, int> _exchange_cs6v6_award_info;
	
	int _check_buffarea_time;
	int _prop_add[PROP_ADD_NUM];

	enum{
		KINGDOM_TITLE_NONE = 0,
		KINGDOM_TITLE_KING = 1,		//国王
		KINGDOM_TITLE_QUEEN = 2,	//王后
		KINGDOM_TITLE_GENERAL = 3,	//将军
		KINGDOM_TITLE_OFFICIAL = 4,	//大臣
		KINGDOM_TITLE_MEMBER = 5,	//帮众
		KINGDOM_TITLE_GUARD = 6,	//亲卫
	};

	struct hide_and_seek
	{
		int hide_and_seek_timestamp;
		int daily_award_timestamp;
		short score;
		int reservered[4];
	} _hide_and_seek_info;
	
public:

	DECLARE_SUBSTANCE(gplayer_imp);
	int 	_disconnect_timeout;	//断线时的超时设置
	int 	_offline_type;		//断线的类型，登出，断线或者踢出
	int	_write_timer;		//写盘使用的计时器
	int	_link_notify_timer;	//给link server发送心跳的定时器
	int 	_general_timeout;	//通用的超时器，目前只有交易时使用
	int	_item_poppet_counter;	//物品替身娃娃计数
	int	_exp_poppet_counter;	//经验替身娃娃计数
	int	_cultivation;		//造化，仙魔佛的组合mask  0x01 仙 0x02魔  0x04佛
	int	_offline_agent_time;	//当前离线托管的可用时间 单位是分钟 
	int	_offline_agent_active;	//当前离线托管的可用时间 单位是分钟 
	int	_last_db_save_time;	//上一次存盘的时间

	move_control _mcontrol;
	phase_control _ph_control;
	struct 
	{
		int error_counter;
	} move_checker;

	bool _no_check_move;
	
	int DecMoveCheckerError(int offset)
	{
		if((move_checker.error_counter -= offset) < 0)
		{
			move_checker.error_counter = 0;
		}
		return 	move_checker.error_counter;
	}

	int IncMoveCheckerError(int offset)
	{
		return 	(move_checker.error_counter += offset);
	}

	void ClrMoveCheckerError()
	{
		move_checker.error_counter  = 0;
	}

	//任务相关的内容
	unsigned long _task_mask;
	abase::vector<char> _active_task_list;
	abase::vector<char> _finished_task_list;
	abase::vector<char> _finished_time_task_list;

	//副本key相关的内容
	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>  _cur_ins_key_list;
	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>  _team_ins_key_list;
	abase::vector<abase::pair<int,int> >  _cur_tag_counter;
	abase::vector<abase::pair<int,int> >  _ins_key_timer;		//记录副本key的使用时间，避免刷副本

	//仓库结构
	player_trashbox  _trashbox;

	//玩家的下线类型，这决定了给link发送何种协议
	enum
	{
		PLAYER_OFF_LOGOUT,
		PLAYER_OFF_OFFLINE,
		PLAYER_OFF_KICKOUT,
		PLAYER_OFF_LPG_DISCONNECT,
		PLAYER_OFF_CONSIGNROLE,
		PLAYER_OFF_RECONNECT,
	};

	//物品列表的定义，主要用于和客户端的联系
	enum
	{
		IL_INVENTORY,
		IL_EQUIPMENT,
		IL_TASK_INVENTORY,
		IL_TRASH_BOX,		//这个在GetInventroy中无法取得  仓库
		IL_TRASH_BOX2,		//这个在GetInventroy中无法取得  帮派仓库
		IL_PET_BEDGE,
		IL_PET_EQUIP,
		IL_POCKET,
		IL_FASHION,
		IL_MOUNT_WING,
		IL_GIFT,
		IL_FUWEN,
	};

	//角色交易错误码
	enum
	{
		CONSIGN_ROLE_SUCCESS			=  0,
		ERR_CONSIGN_ROLE_OFFLINE_1		= -1,
		ERR_CONSIGN_ROLE_OFFLINE_2		= -2,
		ERR_CONSIGN_ROLE_OFFLINE_3		= -3,
		ERR_CONSIGN_ROLE_SAVEDATA_1		= -4,
		ERR_CONSIGN_ROLE_SAVEDATA_2		= -5,
		ERR_CONSIGN_ROLE_SAVEDATA_3		= -6,
		ERR_CONSIGN_ROLE_SAVEDATA_4		= -7,
	};

	//阵法相关
	bool IsCircleOfDoomPrepare() { return _circle_of_doom.IsPrepare(); } 
	void LeaveCircleOfDoom()     { return _circle_of_doom.Leave(); }
	void SendCircleOfDoomInfo();
	void CircleOfDoomPlayerLogout();
	void CircleOfDoomPlayerDisconnect();
	void CircleOfDoomClearMember();
	virtual bool SetCircleOfDoomPrepare(float radius, int faction, int max_member_num, int skill_id);
	virtual void SetCircleOfDoomStartup();
	virtual void SetCircleOfDoomStop();
	virtual bool IsInCircleOfDoom()  { return _circle_of_doom.IsInCircleOfDoom(); }
	virtual int  GetPlayerInCircleOfDoom( std::vector<XID> &playerlist );
	virtual int  GetCircleMemberCnt();

	//角色寄售
	virtual void SetConsignRoleFlag(bool is_consign_role) { _is_consign_role = is_consign_role; }
	bool CheckToSendConsignRoleInfo(int result);
	void SendOnlineAwardDataDirectly(time_t t);

	//消费值
	int  CheckSpecialConsumption(int item_type, int count);
	int  CheckEquipConsumption(int item_type);
	int  CheckUseItemConsumption(int item_type, int count);
	int64_t  GetConsumptionValue() const { return _consumption_value; }
	int64_t  LoadConsumptionValue(int64_t value);

	//物品回购
	bool RepurchaseItem(size_t inv_index, int type, size_t count);
	void SendRepurchaseInvData();
	void UpdateRepurchaseInv();

	//飞剑坐骑包裹
	int TakeOutMountWingItem(int mw_index, int item_id, int count);
	void ChangeMountWingInventorySize(int size);

	//隐藏vip等级
	void SetVipLevelHide(char is_hide);

	//声望商店
	struct reputation_item_info
	{
		int item_type;
		int count;
		struct
		{
			unsigned int    repu_id_req;        
			unsigned int    repu_req_value;     
			unsigned int    repu_id_consume;
			unsigned int    repu_consume_value;
		} repu_required_pre[3];
	};
	bool PurchaseRegionReputationItem(const reputation_item_info & info);
	void PlayerFirstExitReason(int reason);
	
	//SNS
private:
	//阵法处理消息函数
	void HandleMsgCircleOfDoomPrepare(const MSG & msg);
	void HandleMsgCircleOfDoomStartup(const MSG & msg);
	void HandleMsgCircleOfDoomStop(const MSG & msg);
	void HandleMsgCircleOfDoomMemberEnter(const MSG & msg);
	void HandleMsgCircleOfDoomMemberLeave(const MSG & msg);
	void HandleMsgCircleOfDoomQuery(const MSG & msg);

	//vip奖励
	bool SendPlayerVipAwardInfo(bool force = false);

	//在线倒计时奖励
	bool SendPlayerOnlineAwardInfo(bool force_flag);
	bool CheckPlayerOnlineAward(time_t t);

	//角色寄售
	inline bool IsConsignRole() { return _is_consign_role; }
	bool		_is_consign_role;

	//消费值,该值除以config.h中定义的CONSUMPTION_SCALE后得到真实的值，这样处理为了避免保存为float型
	inline int	AddConsumptionValue(int64_t value) { return (_consumption_value += value); }
	int64_t		_consumption_value;

	//物品回购包裹
	bool		AddToRepurchaseInv(const item & item_record, int count);

	//坐骑飞剑包裹
	bool		ExchangeEquipMountWingItem(size_t index_mw, size_t index_equip);
	

public:
	gplayer_imp();
	~gplayer_imp();
public:
	inline gplayer * GetParent() { return (gplayer*)_parent; }
	inline int GetPlayerClass() { return ((gplayer*)_parent)->base_info.cls;}
	virtual int GetObjectClass() { return GetPlayerClass();}			//取得对象的职业
	int GetPlayerOccupation(int cls);
	virtual bool IsObjectFemale() { return IsPlayerFemale();}
	inline bool IsPlayerFemale() { return ((gplayer*)_parent)->IsFemale();}
	inline bool IsDead() { return ((gplayer*)_parent)->IsZombie();}
	inline int GetBattleFaction() { return _battle_faction; }
	inline void SetPlayerClass(int cls, bool is_female) 
	{ 
		gplayer * pPlayer = GetParent();
		pPlayer->SetClassGender(cls,is_female);
	}
	inline bool IsTrade() { return _trade_obj != NULL; }
	inline bool IsConsign() { return _consign_obj != NULL; }

	inline void GetPlayerClass(int & cls,bool & is_female)
	{
		gplayer * pPlayer = GetParent();
		cls = pPlayer->base_info.cls;
		is_female = pPlayer->IsFemale();
	}

	// Youshuang add
	struct item_cnt_info
	{
		explicit item_cnt_info( int id, int cnt ) : item_id( id ), item_cnt( cnt ){}
		int item_id;
		int item_cnt;
	};
	bool HasEnoughSlot( int where, int item_id, int item_cnt );
	bool HasEnoughSlot( int where, const std::vector< item_cnt_info >& items );
	inline void SetFashionWeaponAddon( int addon_id )
	{
		_fashion_weapon_addon_id = addon_id;
	}
	bool CombineFashionColorant(int* idx, int cnt);
	bool AddFashionColor( int colorant_index, int where, int fashion_item_index );
	bool GetAchievementAward(  unsigned short achieve_id, unsigned int award_id );
	virtual void DeliverTopicSite( TOPIC_SITE::topic_data* data );
	virtual XID GetQilinLeaderID();
	virtual void BeHurtOnSeekAndHideRaid(const XID & attacker) {}
	bool DoShopFromFacbaseMall( int goods_idx, int item_id, int cnt );
	bool GetFacbaseMallInfo() const;
	void SetFactionCoupon( int faction_coupon )
	{
		_faction_coupon = faction_coupon;
	}
	int GetFactionCoupon() const { return _faction_coupon; }
	void SetFactionCouponAdd( int64_t faction_coupon_add )
	{
		_fac_coupon_add = faction_coupon_add;
	}
	int64_t GetFactionCouponAdd() const{ return _fac_coupon_add; }
	void SendFactionCouponAdd();
	void FactionCouponChange( int delta );
	bool ContributeFacbaseCash( int cnt );
	virtual bool GetFacbaseCashItemsInfo(){ return true; }
	virtual void BuyFacbaseCashItem( int item_idx ){}
	//在基地中的子类可以直接修改留言
	virtual FBASE_CMD_RET CheckPostFacBaseMsg(int fid, char message[], unsigned int len );
	virtual FBASE_CMD_RET DoBidFromFacbaseMall( int idx, int item_id, int coupon, int name_len, char * player_name);
	bool TryDeliverItem( int where, int item_id, int cnt );
	void FetchNewYearAward( char award_type );
	void LoadNewYearAwardInfo( archive& ar );
	void SaveNewYearAwardInfo( archive& ar );
	void PlayerGetNewYearAwardInfo();
	char CanFetchNewYearDailyAward();
	char CanFetchNewYearAward( int idx ) const;
	void DebugAddNewYearScore( int score )
	{ 
		_newyear_info.score += score;
		PlayerGetNewYearAwardInfo();
	}
	inline short GetUsedTitle(){ return _used_title; }
	inline void SetUsedTitle( short title )
	{
		_used_title = title;
		_title.SetTitle( this, title );
	}
	inline void UseTitle( short title )
	{
		if( _used_title == title ) return;

		if( (title != 0) && CheckPlayerTitle( title ) )
		{
			SetUsedTitle( title );
			_runner->player_use_title( title );
			return;
		}
		
		SetUsedTitle( 0 );
		_runner->player_use_title( 0 );
		return;
	}
	// end
	
	//烈山职业
	inline bool IsLieshanOccupation()
	{
		int occupation = GetPlayerClass();
		return occupation >= 39 && occupation <=43;
	}

	
	//萝莉职业
	inline bool IsLoliOccupation()
	{
		int occupation = GetPlayerClass();
		return occupation >= 102 && occupation <=106;
	}

	//人马职业
	inline bool IsRenMaOccupation()
	{
		int occupation = GetPlayerClass();
		return occupation >= 108 && occupation <=112;
	}

	enum
	{
		RACE_NONE,
		RACE_HUMAN,		//人族
		RACE_DIVINE,		//神族
		RACE_TIANMAI,		//天脉
	};

	inline int GetRace()
	{
		int occupation = GetPlayerClass();
		if(occupation >= 0 && occupation <= 32) return RACE_HUMAN;
		else if(occupation > 32 && occupation < 64) return RACE_DIVINE;
		else if(occupation >= 64 && occupation <= 68) return RACE_HUMAN;
		else if(occupation >= 96 && occupation <= 100) return RACE_DIVINE;
		else if(occupation >= 102 && occupation <= 112) return RACE_TIANMAI;

		return RACE_NONE;

	}

	bool ChangeClass(int new_cls)
	{
		if(!player_template::VerifyClassInvalid(new_cls)) return false;
		
		_skill.ClearXPSkill(object_interface(this));
		_skill.SetXPSkill(player_template::GetXPSkill(new_cls));
		_runner->get_skill_data();
		_runner->get_combine_skill_data();

		gplayer * pPlayer = GetParent();
		pPlayer->SetClassGender(new_cls,pPlayer->IsFemale());
		_runner->player_change_class();
		player_template::InitPlayerData(new_cls,this);
		RefreshEquipment();

		//在线倒计时奖励，转职业
		CheckPlayerOnlineAward(g_timer.get_systime());
		return true;
	}

	inline void SetPlayerName(const void  * name, size_t size)
	{
		if(size > MAX_USERNAME_LENGTH) size = MAX_USERNAME_LENGTH;
		memcpy(_username,name,size);
		_username_len = size;
	}

	inline const void  * GetPlayerName(size_t & len)
	{
		len = _username_len;
		return _username;
	}

	inline void SetMafiaName(const void * name, size_t size)
	{
		if (size > MAX_MAFIA_NAME_LENGTH) size = MAX_MAFIA_NAME_LENGTH;
		memcpy(_mafia_name,name,size);
		_mafia_name_len = size;
	}

	inline const void * GetMafiaName(size_t & len)
	{
		len = _mafia_name_len;
		return _mafia_name;
	}

	player_mallinfo & MallInfo()
	{
		return _mallinfo;
	}
	
	player_bonusinfo & BonusInfo()
	{
		return _bonusinfo;
	}

	inline void SetMafiaJoinTime(int t)
	{
		_mafia_join_time = t;
	}

	inline int GetMafiaJoinTime()
	{
		return _mafia_join_time;
	}

	inline int GetBillingCash()
	{
		return _billing_cash;
	}

	inline void DeliveryNotifyCash(int cash_plus_used)
	{
		_mallinfo.SetCashPlusUsedValue(cash_plus_used);
		_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	}

	inline void DeliveryNotifyCashAdd(int cash_plus_used, int cash_add2)
	{
		_mallinfo.SetCashPlusUsedValue(cash_plus_used);
		_mallinfo.SetCashAdd(cash_add2);
		_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	}

	inline int GetCashAvail()
	{
		return _mallinfo.GetCash();
	}

	inline void AddWebOrder(std::vector<GMSV::weborder> & list)
	{
		_mallinfo.AddWebOrder(list, this);
		_runner->web_order_info();
		_runner->player_cash(_mallinfo.GetCash(), _mallinfo.GetCashUsed(), _mallinfo.GetCashAdd());
	}

	bool PlayerGetWebOrder(int64_t order_id);

	bool PlayerCustomizeRune(int type, int count, int prop[]);
	void PlayerControlTrap(int id, int tid);
	bool PlayerSummonTeleport(int npc_id);
	void HandleSummonTeleport(A3DVECTOR & pos, int type);

	inline void DeliveryNotifyBillingCash(int total_cash)
	{
		_billing_cash = total_cash;
		_runner->player_cash(_billing_cash, -1, -1);
	}

	inline void GetBasicData(short &level, unsigned short &dt_level, int64_t &exp,int64_t &dt_exp, int &hp,int &mp, int &dp, int & pp,int &money, size_t& battle_score, unsigned int & db_magic_id, int & loginip )
	{
		level = _basic.level;
		dt_level = _basic.dt_level;
		exp = _basic.exp;
		dt_exp = _basic.dt_exp;
		hp = _basic.hp;
		mp = _basic.mp;
		dp = _basic.dp;
		pp = _basic.status_point;

		money =_player_money;
		battle_score = _battle_score;
		db_magic_id = _db_magic_number;
		loginip = _ip_address;
	}

	inline void SetBasicData(int level, unsigned short deity_level, int64_t exp,int64_t dt_exp, int hp,int mp, int dp,  int pp,int money, size_t battle_score, unsigned int db_magic_id, int loginip )
	{
		_basic.level = level;
		_basic.dt_level = deity_level;
		_basic.exp = exp;
		_basic.dt_exp = dt_exp;
		_basic.hp = hp;
		_basic.mp = mp;
		_basic.dp = dp;
		_basic.status_point = pp;
		if(level < PVP_PROTECT_LEVEL) 
		{
			SetPVPFlag(false);
		}
		else 
		{
			SetPVPFlag(true);
		}

		_player_money = money;
		_battle_score = battle_score;
		_db_magic_number = db_magic_id;
		_ip_address = loginip;
	}
	
	//覆盖原有的函数
	//目前该状态设置只在player内部使用
	inline void ActiveCombatState(bool state) 
	{
		if(state != _combat_state) 
		{
			SetRefreshState();
		}
		if(state == false)
		{
			ClearHurtEntry();
			if(_combo_skill.state != 0) ClearComboSkill();
		}
		_combat_state = state;
	} 

	
	inline bool IsDeliverLegal()
	{
		return _pstate.IsNormalState() || _pstate.IsBindState();
	}

	inline int GetPlayerState()
	{
		return _pstate.GetStateValue();
	}

	inline bool CanSwitch()
	{
		return _pstate.CanSwitch();
	}
	
	inline bool CanProduceItem()
	{
		return _pstate.CanProduceItem();	
	}

	inline int GetIdlePlayerState()
	{
		return _idle_player_state;
	}

	inline bool IsWaitFactionTradeState()
	{
		return _pstate.IsWaitFactionTradeState();
	}

	inline bool IsMarketState()
	{
		return _pstate.IsMarketState();
	}

	inline item_list & GetInventory(){ return _inventory;}
	inline item_list & GetEquipInventory(){ return _equipment;}
	inline item_list & GetTaskInventory(){ return _task_inventory;}
	inline item_list & GetTrashBoxInventory(){ return _trashbox.Backpack();}
	inline item_list & GetTrashBoxInventoryMafia(){ return _trashbox.MafiaBackpack();}
	inline player_trashbox & GetTrashBox() { return _trashbox;}
	inline item_list & GetPetBedgeInventory(){ return _pet_bedge_inventory;}
	inline item_list & GetPetEquipInventory(){ return _pet_equip_inventory; }
	inline item_list & GetPocketInventory(){return _pocket_inventory;}
	inline item_list & GetFashionInventory(){return _fashion_inventory;}
	inline item_list & GetMountWingInventory(){return _mount_wing_inventory;}
	inline item_list & GetGiftInventory(){return _gift_inventory;}
	inline item_list & GetFuwenInventory(){return _fuwen_inventory;}

	inline item_list & GetInventory(int where)
	{
		switch(where)
		{
			case IL_INVENTORY:
			default:
				return _inventory;
			case IL_EQUIPMENT:
				return _equipment;
			case IL_TASK_INVENTORY:
				return _task_inventory;
			case IL_PET_BEDGE:
				return _pet_bedge_inventory;
			case IL_PET_EQUIP:
				return _pet_equip_inventory;
			case IL_POCKET:
				return _pocket_inventory;
			case IL_FASHION:
				return _fashion_inventory;
			case IL_MOUNT_WING:
				return _mount_wing_inventory;
			case IL_GIFT:
				return _gift_inventory;
			case IL_FUWEN:
				return _fuwen_inventory;
		}
	}

	inline bool InventoryIsFull(int item_type) { return _inventory.HasSlot(item_type);} 
	inline bool InventoryHasSlot(size_t count) { return count <= _inventory.GetEmptySlotCount();} 
	inline bool IsItemExist(size_t inv_index, int type, size_t count) { return _inventory.IsItemExist(inv_index,type,count); }
	inline bool IsItemExist(int where, size_t index, int type, size_t count)
	{
		switch(where)
		{
			case IL_INVENTORY:
				return _inventory.IsItemExist(index,type,count);
			case IL_EQUIPMENT:
				return _equipment.IsItemExist(index,type,count);
			case IL_TASK_INVENTORY:
				return _task_inventory.IsItemExist(index,type,count);
			case IL_PET_BEDGE:
				return _pet_bedge_inventory.IsItemExist(index,type,count);
			case IL_PET_EQUIP:
				return _pet_equip_inventory.IsItemExist(index,type,count);
			case IL_POCKET:
				return _pocket_inventory.IsItemExist(index, type, count);
			case IL_FASHION:
				return _fashion_inventory.IsItemExist(index, type, count);
			case IL_MOUNT_WING:
				return _mount_wing_inventory.IsItemExist(index, type, count);
			case IL_GIFT:
				return _gift_inventory.IsItemExist(index, type, count);
			case IL_FUWEN:
				return _fuwen_inventory.IsItemExist(index, type, count);
		}
		return false;
	}
	
	inline bool IsItemExist(int type)
	{
		if(type == 0 || type == -1) return false;
		return _inventory.Find(0,type) >= 0;
	}
	
	inline bool IsItemCanSell(size_t inv_index, int type, size_t count)
	{
		return _inventory.IsItemExist(inv_index,type,count) && 
			_inventory[inv_index].CanSell();
	}

	inline bool IsItemNeedRepair(int where, size_t index, int type)
	{
		return false;
	}

	inline bool HasEnoughMafiaTime(int t)
	{
		return _mafia_join_time + t < g_timer.get_systime();
	}

	inline void SetMafiaContribution(int con)
	{
		if(OI_GetMafiaID() > 0)
		{
			_mafia_contribution = con;
		}
		else
		{
			_mafia_contribution = 0;
		}
	}
	
	inline int GetMafiaContribution()
	{
		return _mafia_contribution;
	}

	inline void SyncPlayerMafiaInfo()
	{
		GMSV::SendPlayerFaction(_parent->ID.id,OI_GetFamilyID(), OI_GetMafiaID(), _basic.level,_mafia_contribution, _family_contribution, _battle_score, GetRebornCount(), GetCircleID() );
	}


	inline void SendRefLevelUp(int64_t need_exp1, int64_t need_exp2)
	{
		if(_referrer_id == 0 || _referrer_id == -1) return;
		int64_t exp_ctrb = 0;
			
		//未飞升玩家
		if(GetRebornCount() == 0)
		{
			exp_ctrb = (int64_t)(0.05 * need_exp1);	
		}
		else
		{
			exp_ctrb = (int64_t)((0.005 * need_exp1) > (0.5 * need_exp2) ? (0.005 * need_exp1) : (0.5 * need_exp2)); 
		}
		GMSV::SendLevelUp(_parent->ID.id, exp_ctrb, _basic.level, GetRebornCount());
	}

	inline void SendRefCashUsed(int cash_used)
	{
		if(_referrer_id == 0 || _referrer_id == -1) return;
		int cash_bonus = cash_used * 3 /100;
		GMSV::SendCashUsed(_parent->ID.id, cash_bonus);	
	}
	
	inline void ModifyMafiaContribution(int modify)
	{
		if(OI_IsMafiaMember())
		{
			_mafia_contribution += modify;
			SyncPlayerMafiaInfo();
		}
		else
		{
			_mafia_contribution = 0;
		}
		_runner->player_mafia_contribution(_mafia_contribution, _family_contribution);
	}


	inline void SetFamilyContribution(int con)
	{
		if(OI_GetFamilyID() > 0)
		{
			_family_contribution = con;
		}
		else
		{
			_family_contribution = 0;
		}
	}
	
	inline int GetFamilyContribution()
	{
		return _family_contribution;
	}

	inline void ModifyFamilyContribution(int modify)
	{
		if(GetParent()->id_family)
		{
			_family_contribution += modify;
			SyncPlayerMafiaInfo();
		}
		else
		{
			_family_contribution = 0;
		}
		_runner->player_mafia_contribution(_mafia_contribution, _family_contribution);
	}

	inline void UpdateCircleInfo(unsigned int id, unsigned char mask)
	{
		gplayer *pPlayer = GetParent();
		pPlayer->circle_id = id;
		pPlayer->circle_title = mask;
		if(id)
		{
			pPlayer->SetExtraState(gplayer::STATE_CIRCLE);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_CIRCLE);
		}
		if(pPlayer->pPiece)
		{
			_runner->circle_info_notify();
		}
	}

	inline void SetZoneId(int id)
	{
		gplayer *pPlayer = GetParent();
		pPlayer->id_zone = id;
	}

	inline int GetLogoutTime() const
	{
		return _logout_time;
	}

	inline void SetLogoutTime(int time)
	{
		_logout_time = time;
	}

	inline void SetRoleCreateTime(int create_time)
	{
		_create_time = create_time;
	}

	inline unsigned long GetRoleCreateTimeDuration() const
	{
		int nret = 0;
		nret = g_timer.get_systime() - _create_time;
		if(nret < 0)
		{
			nret = 0;
		}

		return (unsigned long)nret;
	}

/*
	inline int GetPKCount()
	{
		return _kill_count;
	}

	inline int GetPVPCoolDown()
	{
		return _pvp_cooldown;
	}
*/

	inline char GetDeadFlag()
	{
		if(!_parent->IsZombie())  return false;
		return _kill_by_player?player_var_data::KILLED_BY_PLAYER:player_var_data::KILLED_BY_NPC;
	}

	inline void SetDeadFlag(char d)
	{
		if(d)
		{
			_parent->b_zombie = true;
			if(d == player_var_data::KILLED_BY_PLAYER) 
				_kill_by_player = true;
			else 
				_kill_by_player = false;
		}
	}

	inline int GetResurrectTimestamp()
	{
		return _resurrect_timestamp;
	}

	inline void ClrResurrectTimestamp()
	{
		_resurrect_timestamp = 0;
	}

	inline bool GetResurrectState(float & exp_reduce, float & hp_recover)
	{
		if(!_parent->IsZombie()) return false;
		exp_reduce = _resurrect_exp_reduce;
		hp_recover = _resurrect_hp_recover;
		return _resurrect_state;
	}

	inline void SetResurrectState(bool state, float  exp_reduce)
	{
		if(!_parent->IsZombie()) return;
		if(exp_reduce <0.f) exp_reduce = 0.f;
		if(exp_reduce >1.0f) exp_reduce = 1.0f;
		_resurrect_state = state;
		_resurrect_exp_reduce = exp_reduce;
		_resurrect_hp_recover = 0.1f;
	}

/*
	inline void SetPVPState(int pkcount, int pvp_cooldown, bool pvp_flag)
	{
		_invade_ctrl._kill_count = pkcount;
		_pvp_cooldown = pvp_cooldown;
		//如果是红粉名，则强制开启PK开关
		if(_invader_state != INVADER_LVL_0) pvp_flag = true;
		_pvp_enable_flag = pvp_flag;
		if(pvp_flag)
		{
			GetParent()->object_state |= gactive_object::STATE_PVPMODE;
		}
	}
	*/

	inline size_t GetMoney()
	{
		return _player_money;
	}


/*
	inline void IncMoney(size_t inc)
	{
		size_t newmoney = _money + inc;
		ASSERT((int)_money >= 0);
		if((int)newmoney < (int)_money || newmoney > _money_capacity)
		{
			//这里溢出了
			newmoney = (size_t)-1;
			size_t delta = _money_capacity - _money;
			inc -= delta;
			_money = _money_capacity; 
			DropMoneyItem(_plane,_parent->pos,inc,_parent->ID,0,0);
		}
		else
		{
			_money = newmoney;
		}
	}

	inline void DecMoney(size_t offset)
	{
		ASSERT(offset <= _money);
		_money -= offset;
	}
*/
	inline bool CanUseService()
	{
		return _provider.id.id != -1 && 
			_provider.pos.squared_distance(_parent->pos) < MAX_SERVICE_DISTANCE*MAX_SERVICE_DISTANCE
			&& (!_provider.id_mafia || _provider.id_mafia == GetParent()->id_mafia);
	}

	
	inline bool IsTrashBoxOpen()
	{
		return _trashbox.IsOpened();
	}

	inline int GetTrashBoxOpenType()
	{
		//这个判定最好和_trashbox.GetCurBackpack() 一致， 以尽可能避免意外情况的发生
		if(_trashbox.IsMafiaBoxOpen())
		{
			return IL_TRASH_BOX2;
		}
		else
		{
			return IL_TRASH_BOX;
		}
		
	}

	inline void ClearTrahsBoxWriteFlag()
	{
		_trashbox.ClrChangeCounter();
	}

	inline bool IsTrashBoxChanged()
	{
		return _trashbox.IsTrashBoxChanged();
	}
	
	inline unsigned int GetTrashBoxCounter()
	{
		return _trashbox.GetChangeCounter();
	}

	inline void IncEquipChangeFlag()
	{
		_eq_change_counter ++;
		if(_eq_change_counter == 0)
		{
			_eq_change_counter = 1;
		}
	}


	inline bool IsBled()
	{
		//以后要加入特殊状态的检验
		return _basic.hp < GetMaxHP() ||_basic.mp < GetMaxMP();
	}

	inline void Renew()
	{
		_basic.hp = GetMaxHP();
		_basic.mp = GetMaxMP();
		SetRefreshState();
		_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);
		_runner->renew();
	}

	inline void EnterStayInState()
	{
		_commander->DenyCmd(controller::CMD_PICKUP);
		_commander->DenyCmd(controller::CMD_ATTACK);
		ObjectSitDown();
		//要发送坐下的消息给所有人
		_runner->sit_down();
	}

	inline void LeaveStayInState()
	{
		if(!IsSitDownMode()) return;
		_commander->AllowCmd(controller::CMD_PICKUP);
		_commander->AllowCmd(controller::CMD_ATTACK);
		ObjectStandUp();
		_runner->stand_up();
	}

	inline void LeaveBindState()
	{
		gplayer *pPlayer = GetParent();
		if(!_pstate.IsBindState()) return;
		if(pPlayer->bind_type == 1 || pPlayer->bind_type ==2){
			_bind_player.PlayerLinkCancel(this);
		}
		else if(pPlayer->bind_type == 3 || pPlayer->bind_type == 4){
			_link_ride_player.PlayerLinkCancel(this);
		}
		else if(pPlayer->bind_type == 5 || pPlayer->bind_type == 6){
			_link_bath_player.PlayerLinkCancel(this);
		}
	}
	
	inline player_team & GetTeamCtrl() { return _team;}
	inline bool IsTeamLeader() { return _team.IsLeader();}
	inline bool IsMember(const XID & member) { return _team.IsMember(member);}
	inline bool IsInTeam() { return _team.IsInTeam();}
	inline const XID & GetTeamLeader() { return _team.GetLeader();}
	inline int GetTeamMemberNum() { return _team.GetMemberNum(); }
	inline int GetTeamID()
	{
		if(IsInTeam()) 
			return _team.GetLeader().id;
		else
			return 0;
	}
	inline int GetTeamSeq() 
	{ 
		if(IsInTeam()) 
			return _team.GetTeamSeq();
		else
			return -1;
	}
	inline const player_team::member_entry & GetTeamMember(int index) {return _team.GetMember(index);}
	inline void TeamChangeLeader(int id) { return _team.CliChangeLeader(XID(GM_TYPE_PLAYER,id));}
	inline int GetMemberList(XID * list) { return _team.GetMemberList(list);}
	inline void SendTeamData(const XID & leader,size_t team_count,size_t data_count, 
			const player_team::member_entry ** list)
	{
		((gplayer_dispatcher*)_runner)->send_team_data(leader,team_count,data_count,list);
	}

	inline void SendTeamData(const XID & leader,size_t team_count,
			const player_team::member_entry * list)
	{
		((gplayer_dispatcher*)_runner)->send_team_data(leader,team_count,list);
	}
	
	inline void ReceiveTaskExp(int64_t exp) 		//任务加经验操作
	{
		if(exp < 0) exp = 0;
		IncExp(exp);
		_runner->task_deliver_exp(exp,0, 0);
	}

	inline void ReceiveBonusExp(int64_t exp)
	{
		ASSERT(exp >= 0);
		IncExp(exp);
		_runner->receive_bonus_exp(exp);
	}
	
	inline void ReceiveTaskDeityExp(int exp) 		//任务加封神经验
	{
		ASSERT(exp >= 0);
		IncDeityExp(exp);
		_runner->task_deliver_exp(0,0, exp);
	}


	inline bool IsDeityExpFull(){return _basic.dt_exp >= player_template::GetDeityLvlupExp(_basic.dt_level);}

	inline bool IsDeity() {return _basic.dt_level > 0;}
	
	bool DecDeityExp(int exp);				//直接扣除封神经验，不进行任何修正

	inline void ModifyReputation(int offset)
	{
		int r1 = GetReputation();
		SetReputation(r1 + offset);
		int r2 = GetReputation();
		if(r1 != r2) _runner->player_reputation(r2);
	}

	inline void SetReputation(int rep)
	{
		if(rep > MAX_REPUTATION) rep = MAX_REPUTATION;
		if(rep < 0) rep = 0;
		GetParent()->reputation = rep;
	}

	inline int GetReputation()
	{
		return GetParent()->reputation;
	}

	inline void ModifyRegionReputation(int idx, int offset)
	{
		int rep = SetRegionReputation(idx, GetRegionReputation(idx) + offset);
		_runner->player_change_region_reputation(idx, rep);
		object_interface oif(this);
		GetAchievementManager().OnReputationInc(oif, idx, rep);
	}

	inline int SetRegionReputation(size_t index, int rep)
	{
		if(index >= REPUTATION_REGION) return 0;
		if(rep > MAX_REPUTATION) rep = MAX_REPUTATION;
		if(rep < 0) rep = 0;
		return (_region_reputation[index] = rep);
	}

	inline int GetRegionReputation(size_t index)
	{
		if(index >= REPUTATION_REGION) return 0;
		return _region_reputation[index];
	}

	inline const int * GetRegionReputationData(size_t & len)
	{
		len = REPUTATION_REGION;
		return _region_reputation;
	}

	inline void SetRegionReputation(const void * buf, size_t size)
	{
		size_t count = size / sizeof(int);
		memset(_region_reputation,0, sizeof(_region_reputation));
		for(size_t i = 0;i < count && i < REPUTATION_REGION; i ++)
		{
			_region_reputation[i] = ((int*)buf)[i];
		}
	}
	


	inline void ClearExpiredCoolDown()
	{
		_cooldown.ClearExpiredCoolDown();
	}

	//只取得自己的副本KEY
	inline void GetSelfInstanceKey(int world_tag, instance_key & key)
	{
		key.essence.key_level1 = _parent->ID.id;
		key.essence.key_level3 = ((gplayer*)_parent)->id_mafia;
		key.essence.key_level4 = 0;

		abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
		it = _cur_ins_key_list.find(world_tag);
		if(it == _cur_ins_key_list.end())
		{
			//只有大世界和公共副本才会这样  所以都进一个副本
			key.essence.key_level2.first = key.essence.key_level2.second = 0;
		}
		else
		{
			key.essence.key_level2.first = it->second.first;
			key.essence.key_level2.second = it->second.second;
		}
	}

	inline void GetInstanceKey(int world_tag, instance_key & key)
	{
		key.essence.key_level1 = _parent->ID.id;
		key.essence.key_level3 = ((gplayer*)_parent)->id_mafia;

		abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
		abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc> * pMap;
		if(_team.IsInTeam())
		{
			pMap = &_team_ins_key_list; 
			it = _team_ins_key_list.find(world_tag);
		}
		else
		{
			pMap = &_cur_ins_key_list; 
			it = _cur_ins_key_list.find(world_tag);
		}

		if(it == pMap->end())
		{
			//只有大世界和公共副本才会这样  所以都进一个副本
			key.essence.key_level2.first = key.essence.key_level2.second = 0;
		}
		else
		{
			key.essence.key_level2.first = it->second.first;
			key.essence.key_level2.second = it->second.second;
		}
	}

	inline void SetLastInstancePos(int tag, const A3DVECTOR &pos, int timestamp)
	{
		_last_instance_pos = pos;
		_last_instance_tag = tag;
		_last_instance_timestamp = timestamp;
	}

	inline void SetLastInstanceSourcePos(int tag, const A3DVECTOR &pos)
	{
		_last_source_instance_pos = pos;
		_last_source_instance_tag = tag;
	}

	inline void GetLastInstancePos(int &tag, A3DVECTOR &pos, int & timestamp)
	{
		pos = _last_instance_pos;
		tag = _last_instance_tag;
		timestamp = _last_instance_timestamp;
	}

	inline void GetLastInstanceSourcePos(int &tag, A3DVECTOR &pos)
	{
		pos = _last_source_instance_pos;
		tag = _last_source_instance_tag;
	}

	inline int GetUserID()
	{
		return _db_magic_number;
	}

	inline void SetPVPCombatState()
	{
		if(!_pvp_combat_timer)
		{
			_runner->active_pvp_combat_state(true);
			GetParent()->SetObjectState(gactive_object::STATE_IN_PVP_COMBAT);
		}
		_pvp_combat_timer = PVP_COMBAT_HIGH_TH;
	}

	inline bool IsInPVPCombatStateHigh()
	{
		return _pvp_combat_timer > PVP_COMBAT_LOW_TH;
	}

	inline bool IsInPVPCombatState()
	{
		return _pvp_combat_timer;
	}

	inline int GetPVPCombatTimer()
	{
		return _pvp_combat_timer;
	}


	void SetRestParam(const void * buf, size_t size);
	
	inline void SetPlayEd(int timestamp, int playtime)
	{
		_login_timestamp = timestamp;
		_played_time = playtime;
	}
	
	inline int GetPlayEd()
	{
		return _played_time + (g_timer.get_systime() - _login_timestamp);
	}

	inline int GetWallowLevel()
	{
		return _wallow_level;
	}

	inline void SetWallowData(const void * buf, size_t len)
	{
	/*
		time_t t = g_timer.get_systime();
		_wallow_obj.Init(t, buf, len);
		if(gmatrix::AntiWallow())
		{
			_wallow_level = _wallow_obj.Tick(t);
		}
		else
		{
			_wallow_level = 0;
		}
		*/
	}

	inline void GetSpecailTaskAward(unsigned long & id,unsigned long &param )
	{
		id = _spec_task_reward;
		param = _spec_task_reward_param;
	}
	
	inline void SetSpecailTaskAward(int id,int param, int fill_info_mask)
	{
		_spec_task_reward = id;
		_spec_task_reward_param = param;
		_fill_info_mask = fill_info_mask;
	}

	inline int GetFillInfoMask(){ return _fill_info_mask;}

	void HandleNetbarReward(int netbar_level, int award_type);

	// 处理GT命中回调
	void HandleAddGTReward();
	void HandleRemoveGTReward();

	inline void SetSecLevel(unsigned long per)
	{
		_basic.sec_level = per & 0xFFFF;
		SetRefreshState();
		_runner->task_deliver_level2(_basic.sec_level);
	}

	inline void SetDBTimeStamp(int db_timestamp)
	{
		_db_timestamp = db_timestamp;
	}

	inline bool VerifyRecipe(unsigned short id)
	{
		return _recipe_list.find(id) != _recipe_list.end();
	}

	inline bool AddRecipe(unsigned short id)
	{
		if(VerifyRecipe(id)) return false;
		return _recipe_list.insert(id).second;
	}

	inline const unsigned short * GetRecipeBuf()
	{
		return _recipe_list.begin();
	}

	inline size_t GetRecipeCount()
	{
		return _recipe_list.size();
	}

	inline void SetRecipeData(const short * buf, size_t count)
	{
		if(count == 0 || count>= 65535) return;
		_recipe_list.clear();
		_recipe_list.reserve(count);
		for(size_t i = 0; i < count ; i ++)
		{
			_recipe_list.insert(buf[i]);
		}
	}

	inline int GetProduceLevel()
	{
		return _produce_level;
	}

	inline int GetProduceExp()
	{
		return _produce_exp;
	}

	inline void SetProduceSkill(int level, int exp)
	{
		_produce_level = level;
		_produce_exp = exp;
	}

	inline void SetParentPKLevel()
	{
		GetParent()->pk_level = _pk_level;
		if(_pk_level > 0)
		{
			_faction |= FACTION_PARIAH;
			_faction &= ~(FACTION_WHITE | FACTION_BLUE);
		}
		else if(_pk_level == 0)
		{
			_faction |= FACTION_WHITE;
			_faction &= ~(FACTION_PARIAH | FACTION_BLUE);
		}
		else
		{
			_faction |= FACTION_BLUE;
			_faction &= ~(FACTION_WHITE | FACTION_PARIAH );
		}
	}


	inline void SetRawPKValue(int value)
	{
		_pk_value = value;
		_pk_level = player_template::CalcPKLevel(GetPKValue());
		SetParentPKLevel();
	}
	
	inline void SetPKValue(int value)
	{
		_pk_value = value * PKVALUE_PER_POINT;
		_pk_level = player_template::CalcPKLevel(GetPKValue());
		SetParentPKLevel();
		_runner->pk_level_notify(GetPKLevel());
	}

	inline void ModifyPKValue(int offset)
	{
		int old_level = GetPKLevel();
		int pkvalue = GetPKValue() + offset;
		if(pkvalue < MIN_PK_VALUE) pkvalue = MIN_PK_VALUE;
		if(pkvalue > MAX_PK_VALUE) pkvalue = MAX_PK_VALUE;
		if(pkvalue * _pk_value <= 0)
		{
			//性相发生了改变 用新点数
			_pk_value = pkvalue * PKVALUE_PER_POINT;
		}
		else
		{
			//未改变，在原有基础上修改
			_pk_value += offset * PKVALUE_PER_POINT;
		}
		if(_pk_value < MIN_PK_VALUE * PKVALUE_PER_POINT) pkvalue = MIN_PK_VALUE * PKVALUE_PER_POINT;
		if(_pk_value > MAX_PK_VALUE * PKVALUE_PER_POINT) pkvalue = MAX_PK_VALUE * PKVALUE_PER_POINT;
		
		_pk_level = player_template::CalcPKLevel(GetPKValue());
		if(GetPKLevel() != old_level)
		{
			SetParentPKLevel();
			_runner->pk_level_notify(GetPKLevel());
		}
	}

	inline void SetPVPFlag(bool flag)
	{
		_pvp_flag_enable = flag;
		if(flag)
			GetParent()->SetObjectState(gactive_object::STATE_PVP_NOPROTECTED);
		else 
			GetParent()->ClrObjectState(gactive_object::STATE_PVP_NOPROTECTED);
	}

	inline void EnablePVPFlag()
	{
		if(!_pvp_flag_enable)
		{
			SetPVPFlag(true);
			_runner->pvp_no_protect();
		}
	}

	inline bool  InsertPlayerTitle(short title)
	{
		if(title == 0) return false;
		if(_title_list.insert(title).second)
		{
			object_interface oif(this);
			GetAchievementManager().OnGainTitle(oif, title);
			return true;
		}
		return false;
	}

	inline void  RemovePlayerTitle(short title)
	{
		if(title == 0) return;
		if(title == GetCurTitle())
		{
			SelectPlayerTitle(0);
		}
		if( title == _used_title )
		{
			UseTitle( 0 );
		}
		_title_list.erase(title);
	}

	inline bool InsertPlayerExtraTitle(short title)
	{
		if(title == 0) return false;
		if(_extra_title.insert(title).second)
		{
			object_interface oif(this);
			GetAchievementManager().OnGainTitle(oif, title);
			return true;
		}
		return false;
	}

	inline const short * GetPlayerTitleList(size_t & count)
	{
		count = _title_list.size();
		return _title_list.begin();
	}

	inline const short * GetPlayerExtraTitle(size_t & count)
	{
		count = _extra_title.size();
		return _extra_title.begin();
	}

	inline void SetPlayerTitleList(const short * list, size_t size)
	{
		_extra_title.clear();
		gmatrix::GetTopTitle(GetParent()->ID.id, this);
		if(_basic.level >= PVP_PROTECT_LEVEL) InsertPlayerExtraTitle(PLAYER_DEFAULT_TITLE_REPUTATION);
		if(_sect_id && _sect_id != _parent->ID.id) InsertPlayerExtraTitle(SECT_MASTER_TITLE);
		if(list == NULL || size == 0) return;
		for(size_t i = 0; i < size / sizeof(short) ; i ++)
		{
			InsertPlayerTitle(list[i]);
		}
	}

	playertitle _title;
	inline short GetCurTitle()
	{
		//return _cur_title.GetTitle();
		return GetParent()->title_id;
	}

	inline void SetCurTitle(short title)
	{
		GetParent()->title_id = title;
		//_title.SetTitle(this, title);
	}
	
	inline bool SelectPlayerTitle(short title, bool broadcast = true)
	{
		if(GetCurTitle() == title) return true;

		if(title == 0)
		{
			SetCurTitle(0);
			if(broadcast) _runner->player_change_title(title);
			return true;
		}
		
		if(CheckPlayerTitle(title))
		{
			SetCurTitle(title);
			if(broadcast) _runner->player_change_title(title);
			return true;
		}
		return false;
	}

	inline bool CheckPlayerTitle(short title)
	{
		bool bRst = _title_list.find(title) != _title_list.end();
		if(bRst) return bRst;
		return _extra_title.find(title) != _extra_title.end();
	}

	inline int GetCurPlayerTitle()
	{
		return GetCurTitle();
	}

	inline bool IsMarried()
	{
		return GetParent()->spouse_id != 0;
	}

	inline int GetSpouse()
	{
		return GetParent()->spouse_id;
	}

	inline void SetSpouse(int id)
	{
		//这个操作不负责发送广播
		gplayer * pPlayer = GetParent();
		if(id == 0)
		{
			pPlayer->ClrObjectState(gactive_object::STATE_SPOUSE);
			pPlayer->spouse_id = 0;
		}
		else
		{
			pPlayer->spouse_id = id;
			pPlayer->SetObjectState(gactive_object::STATE_SPOUSE);
		}
	}
	
	player_achieve_man & GetAchievementManager() { return _achieve_man; }

	inline void * GetSpecialAchievementInfo(size_t & spec_size)
	{
		return _achieve_man.GetSpecialAchievementInfo(spec_size);
	}

	inline void LoadSpecialAchievementInfo(const void *buf, size_t size)
	{
		return _achieve_man.LoadSpecialAchievementInfo(buf, size);
	}

	inline int GetAchievePoint()
	{
		return _achieve_man.GetAchievementPoint();
	}

	inline void ExternSaveDB()
	{
		if(_pstate.CanSave())
		{
			AutoSaveData();
		}
		else
		{
			//能这样村的外面必须检查
			ASSERT(false);
		}
	}

	inline void SetMafiaDuelKills(int kills)
	{
		_mduel.SetKills(kills);
	}

	inline void SetMafiaDuelDeaths(int deaths)
	{
		_mduel.SetDeaths(deaths);
	}

	inline int GetMafiaDuelKills()
	{
		return _mduel.GetKills();
	}
	
	inline int GetMafiaDuelDeaths()
	{
		return _mduel.GetDeaths();
	}

	inline int CalcMoneyAddon(int money)
	{
		float addon = _money_addon * 0.001f;;
		if(addon >=1.0f) addon = 1.0f;
		int ext_money = (int)((money * addon) + 0.1f);
		if(ext_money < 0) ext_money = 0;
		return ext_money;
	}

	inline void UseItemLog(int id, int guid1, int guid2, int count)
	{
		item_manager::UseItemLog(_parent->ID.id,  id, guid1, guid2, count);
	}

	inline void UseItemLog(const item & it, int count)
	{
		item_manager::UseItemLog(_parent->ID.id,  it.type, it.guid.guid1, it.guid.guid2, count);
	}

	inline void UseItemLog(const item & it)
	{
		item_manager::UseItemLog(_parent->ID.id,  it.type, it.guid.guid1, it.guid.guid2, it.count);
	}

	inline void UseItemLog(item_list & inv , size_t index, int count)
	{
		const item & it = inv[index];
		item_manager::UseItemLog(_parent->ID.id,  it.type, it.guid.guid1, it.guid.guid2, count);
	}


	inline void SetTalismanInfo(int max_stamina,float energy_recover_speed, float energy_recover_factor, float energy_drop_speed)
	{
		_talisman_info.talisman_state = true;
		_talisman_info.stamina = 0;
		_talisman_info.max_stamina = max_stamina;
		_talisman_info.energy_recover_speed = energy_recover_speed;
		_talisman_info.energy_recover_factor = energy_recover_factor;
		_talisman_info.energy_drop_speed = energy_drop_speed;
	}

	inline void SetTalismanMaxStamina(int max_stamina)
	{
		_talisman_info.max_stamina = max_stamina;
	}

	inline void ClrTalismanInfo()
	{
		if(_talisman_info.bot_state)
		{
			StopTalismanBot();
		}
		_talisman_info.talisman_state = false;
		_talisman_info.bot_state = false;
		_talisman_info.stamina = 0;
		_talisman_info.max_stamina = 0;
		_talisman_info.energy_recover_speed = 0;
		_talisman_info.energy_recover_factor = 0;
		_talisman_info.energy_drop_speed = 0;
		_talisman_info.need_question = false;
	}


	inline void ActiveTalismanAutobotAddon()
	{
		if( _talisman_info.autobot_arg.addon_active_flag )
		{
			// 已经处于激活状态了，不该进这里的，直接返回吧
			return;
		}
		UpdateBuff( _talisman_info.autobot_arg.buff_id, 1, 0, 1);
		const addon_data_spec *pSpec = NULL;
		pSpec = addon_data_man::Instance().GetAddon( _talisman_info.autobot_arg.addon_id[0] );
		if( pSpec && !pSpec->handler->IsTalismanAddonHandler())
		{       
			pSpec->handler->Activate( pSpec->data, NULL, this, NULL);
		}
		pSpec = addon_data_man::Instance().GetAddon( _talisman_info.autobot_arg.addon_id[1] );
		if( pSpec && !pSpec->handler->IsTalismanAddonHandler())
		{       
			pSpec->handler->Activate( pSpec->data, NULL, this, NULL);
		}
		property_policy::UpdatePlayer( GetPlayerClass(), this );
		_talisman_info.autobot_arg.addon_active_flag = true;
	}

	inline void DeactiveTalismanAutobotAddon()
	{
		if( !_talisman_info.autobot_arg.addon_active_flag )
		{
			// 没有激活直接返回
			return;
		}

		RemoveBuff( _talisman_info.autobot_arg.buff_id );
		const addon_data_spec *pSpec = NULL;
		pSpec = addon_data_man::Instance().GetAddon( _talisman_info.autobot_arg.addon_id[0] );
		if( pSpec && !pSpec->handler->IsTalismanAddonHandler())
		{       
			pSpec->handler->Deactivate( pSpec->data, NULL, this, NULL);
		}
		pSpec = addon_data_man::Instance().GetAddon( _talisman_info.autobot_arg.addon_id[1] );
		if( pSpec && !pSpec->handler->IsTalismanAddonHandler())
		{       
			pSpec->handler->Deactivate( pSpec->data, NULL, this, NULL);
		}
		property_policy::UpdatePlayer( GetPlayerClass(), this );
		_talisman_info.autobot_arg.addon_active_flag = false;
	}
	
	virtual void SetTalismanAutobotArg( const addon_data& data )
	{
		//试图取消一下法宝附加属性
		DeactiveTalismanAutobotAddon();
		
		_talisman_info.autobot_arg.addon_exist_flag = true;
		_talisman_info.autobot_arg.buff_id = data.arg[0];
		_talisman_info.autobot_arg.addon_id[0] = data.arg[1];
		_talisman_info.autobot_arg.addon_id[1] = data.arg[2];
	}

	virtual void ClearTalismanAutobotArg( const addon_data& data )
	{
		if( _talisman_info.autobot_arg.addon_exist_flag )
		{
			// 存在法宝附加属性，就要试图deactive一下，防止出错
			DeactiveTalismanAutobotAddon();
			_talisman_info.autobot_arg.addon_exist_flag = false;
			_talisman_info.autobot_arg.buff_id = 0;
			_talisman_info.autobot_arg.addon_id[0] = 0;
			_talisman_info.autobot_arg.addon_id[1] = 0;
		}
	}

	inline int GetTalismanValue()
	{
		return _talisman_info.talisman_value;
	}

	inline void SetTalismanValue(int value)
	{
		_talisman_info.talisman_value = value;
	}

	inline void StartTalismanBot( bool need_question )
	{
		_talisman_info.bot_state = true;
		_talisman_info.stop_bot_time = 0;
		_talisman_info.need_question = need_question;
		_runner->talisman_bot_begin(1);
		if( _talisman_info.autobot_arg.addon_exist_flag )
		{
			// 存在法宝附加属性才进行操作
			ActiveTalismanAutobotAddon();
		}
	}

	inline void StopTalismanBot()
	{
		_talisman_info.bot_state = false;
//		_talisman_info.stamina = 0;
		SetRefreshState();
		_talisman_info.stop_bot_time = g_timer.get_systime();
		_talisman_info.need_question = false;
		_runner->talisman_bot_begin(0);
		if( _talisman_info.autobot_arg.addon_exist_flag )
		{
			// 存在法宝附加属性才进行操作
			DeactiveTalismanAutobotAddon();
		}
	}

	inline bool IsTalismanAvailable()
	{
		return _talisman_info.talisman_state;
	}
	
	inline int ChargeTalismanStamina(int heal)
	{
		if(!_talisman_info.talisman_state) return 0;
		if(heal <=0) return 0;
		int need = (int)(_talisman_info.max_stamina - _talisman_info.stamina);
		if(need >= heal) 
		{
			_talisman_info.stamina += heal;
			if(_talisman_info.stamina > _talisman_info.max_stamina)
			{
				_talisman_info.stamina = _talisman_info.max_stamina;
			}
			SetRefreshState();
			return heal;
		}
		else
		{
			if(_talisman_info.stamina != _talisman_info.max_stamina)
			{
				_talisman_info.stamina = _talisman_info.max_stamina;
				SetRefreshState();
				return need;
			}
			else
			{
				return 0;
			}
		}
	}

	inline void ActiveAircraft(float speed, int cost, int exp_add)
	{
		_fly_info.state = 1;
		_fly_info.speed = speed;
		_fly_info.stamina_cost = cost;
		if(_fly_info.active && GetParent()->start_fly_type == 0)
		{
			_fly_info.active = false;
		}
		_fly_info.exp_add = exp_add;
	}

	inline void DisableAircraft()
	{
		_fly_info.state = 0;
		_fly_info.speed = 0;
		_fly_info.stamina_cost = 0;
		if(_fly_info.active && GetParent()->start_fly_type == 0)
		{
			_fly_info.active = false;
		}
		//_fly_info.active = false;
		_fly_info.exp_add = 0;
		SetRefreshState();
	}

	inline bool IsAircraftAvailable()
	{
		return _fly_info.state;
	}

	virtual bool InFlying()
	{
		return _fly_info.active;
	}

/*	inline int ChargeAircraftStamina(int heal)
	{
		if(!_fly_info.state) return 0;
		if(heal <=0) return 0;
		int need = (int)(_fly_info.max_stamina - _fly_info.stamina);
		if(need >= heal) 
		{
			_fly_info.stamina += heal;
			if(_fly_info.stamina > _fly_info.max_stamina)
			{
				_fly_info.stamina = _fly_info.max_stamina;
			}
			SetRefreshState();
			return heal;
		}
		else
		{
			if(_fly_info.stamina != _fly_info.max_stamina)
			{
				_fly_info.stamina = _fly_info.max_stamina;
				SetRefreshState();
				return need;
			}
			else
			{
				return 0;
			}
		}
	}
	*/

	inline int GetRebornCount() { return _reborn_list.size();}
	inline int GetRebornProf(int idx) 	//0 1 2 3 4 是 从正向开始 1转 2转 3转的数值 若idx等于转生次数，则返回当前职业  若idx 超过了 转生次数，则返回0
						//-1 -2 -3 是从反向开始，上一转的 上上一转的 转生数值
	{
		if(idx >= 0)
		{
			if((size_t)idx >= _reborn_list.size()) 
			{
				if((size_t)idx == _reborn_list.size()) return GetObjectClass();
				return 0;
			}
			return _reborn_list[idx] & 0xFFFF;
		}
		else
		{
			idx = (-idx) -1;
			if((size_t)idx >= _reborn_list.size()) return 0;
			return _reborn_list[idx] & 0xFFFF;
		}

	}
	inline int GetRebornLvl(int idx)		//返回的方式同GetRebornLvl
	{
		if(idx >= 0)
		{
			if((size_t)idx >= _reborn_list.size()) 
			{
				if((size_t)idx == _reborn_list.size()) return GetObjectLevel();	
				return 0;
			}
			return (_reborn_list[idx] >> 16) & 0xFFFF;
		}
		else
		{
			idx = (-idx) -1;
			if((size_t)idx >= _reborn_list.size()) return 0;
			return (_reborn_list[idx] >> 16) & 0xFFFF;
		}
	}
	inline int GetMaxLevel()
	{
		int max_level = 0;
		for(size_t i = 0; i < _reborn_list.size(); ++i)
		{
			if(GetRebornLvl(i) > max_level) max_level = GetRebornLvl(i);
		}
		return GetObjectLevel() > max_level ? GetObjectLevel() : max_level;
	}

	inline void InitRebornInfo(const void * buf, size_t size)
	{
		const int * list = (const int *) buf;
		size_t count = size / sizeof(int);
		for(size_t i =0; i < count; i ++)
		{
			_reborn_list.push_back(list[i]);
		}

		GetParent()->level2 = GetRebornCount();
	}

	inline void * GetRebornInfo( size_t * pSize)
	{
		*pSize = _reborn_list.size() * sizeof(int);
		if(_reborn_list.empty()) return NULL;
		return &*_reborn_list.begin();
	}

	inline pet_manager& GetPetMan() { return _petman; }

	void SetPetBedgeInventorySize(size_t size)
	{
		_petman.SetPetBedgeInventorySize(this,size);
	}
	
	inline summon_manager& GetSummonMan() { return _summonman; }
	inline mobactive_manager& GetMobactiveMan() { return _mobactiveman; }

	inline void ClearSect()
	{
		_sect_init_level = 0;
		_sect_id = 0;
		gplayer * pPlayer = GetParent();
		pPlayer->sect_flag = 0;
		pPlayer->sect_id = 0;

	}

	inline void SetSectID(int id)
	{
		_sect_id = id;
		gplayer * pPlayer = GetParent();
		pPlayer->sect_id = id;
		pPlayer->sect_flag = (id != 0)?1:0;
	}

	inline void SetSectInitLevel(int level)
	{
		_sect_init_level = level;
	}

	inline int GetSectInitLevel()
	{
		return _sect_init_level;
	}

	inline int GetSectID()
	{
		return _sect_id;
	}

	inline bool IsSectMaster()
	{
		return _sect_id && _sect_id == _parent->ID.id;
	}
	
	inline int GetCultivation() 
	{
		return _cultivation;
	}


	inline void SetCultivation(int cult)
	{
		int cul_old = _cultivation;
		_cultivation = cult;
		GetParent()->cultivation = cult & 0xFF;
	
		if(cul_old != _cultivation)
		{
			_skill.CultivationChange(this, cul_old, _cultivation);
		}
	}

	inline void SetKingdomInfo()
	{
		_kingdom_title = gmatrix::GetKingdomTitle(_parent->ID.id);
		if(_kingdom_title == 0)
		{
			if(OI_GetMafiaID() != 0 && OI_GetMafiaID() == gmatrix::GetKingdomMafiaID())
			{
				_kingdom_title = KINGDOM_TITLE_MEMBER;  
			}
		}

		if(_kingdom_title > 0)
		{
			AddKingdomState();
		}
	}

	inline int GetKingdomTitle()
	{
		return _kingdom_title;
	}

	
	bool HasKing()
	{
		return gmatrix::GetKingdomMafiaID() > 0;
	}

	inline bool IsKingdomKing() { return GetKingdomTitle() == KINGDOM_TITLE_KING;}
	inline bool IsKingdomQueen() { return GetKingdomTitle() == KINGDOM_TITLE_QUEEN;}
	inline bool IsKingdomGeneral() { return GetKingdomTitle() == KINGDOM_TITLE_GENERAL;}
	inline bool IsKingdomOfficial() { return GetKingdomTitle() == KINGDOM_TITLE_OFFICIAL;}
	inline bool IsKingdomMember() { return GetKingdomTitle() == KINGDOM_TITLE_MEMBER;}
	inline bool IsKingdomGuard() { return GetKingdomTitle() == KINGDOM_TITLE_GUARD;}

	inline bool IsKingdomPlayer(){return GetKingdomTitle() > 0; }

	inline int GetKingdomPoint() { return gmatrix::GetKingdomPoint();}
	inline void ConsumeKingdomPoint(int point){ return gmatrix::ConsumeKingdomPoint(point);}
	inline void ObtainKingdomPoint(int point){ return gmatrix::ObtainKingdomPoint(point);}

	void AddKingdomState();
	void RemoveKingdomState();
	void SyncKingdomTitle(int new_title);

	void TryFixRebornLevel();
	bool IsForbidTask(int ulTaskId); 

	inline bool IsMaxInvSize()
	{
		return _inventory.Size() == ITEM_LIST_MAX_ACTUAL_SIZE;
	}


	inline bool IsNewbie()
	{
		if(GetRebornCount() > 0) return false;
		if(_basic.level >= 90) return false;
		return true;
	}

	inline bool IsFreeBot()
	{
		if(IsNewbie()) return true;
		if(_vipstate.free_bot) return true;
		return false;
	}

	//挖宝活动是否开放
	inline bool IsTreasureActive()
	{
		if(GetRebornCount() == 0 && _basic.level < 30) return false;
		return true;
	}

	inline bool IsNoExpDrop()
	{
		if(IsNewbie()) return true;
		if(_vipstate.no_exp_drop) return true;
		return false;
	}

	inline void SetFashionHotkey(const void *buf, size_t size)
	{
		ASSERT(size % 4 == 0);
		const fashion_hotkey *hotkey = (const fashion_hotkey *)buf;
		size_t count = size / sizeof(fashion_hotkey);
		for(size_t i = 0; i < count; ++i){
			_fashion_hotkey.push_back(hotkey[i]);
		}	
	}

	inline void * GetFashionHotkey(size_t * pSize)
	{
		abase::vector<fashion_hotkey, abase::fast_alloc<> >::iterator iter; 
		for(iter = _fashion_hotkey.end(); iter != _fashion_hotkey.begin(); )
		{
			--iter;
			if(iter->index > HOTKEY_MAX_SIZE || iter->index < 0 || (iter->id_head == -1 && iter->id_cloth == -1 && iter->id_shoe == -1) )
			{
			       	_fashion_hotkey.erase(iter);
			}
		}
		
		*pSize = _fashion_hotkey.size()*sizeof(fashion_hotkey);
		if(_fashion_hotkey.empty()) return NULL;
		return &*_fashion_hotkey.begin();
	}

	inline void PlayerSaveFashionKey(int count, const void *buf)
	{
		const fashion_hotkey *hotkey = (const fashion_hotkey *)buf;
		_fashion_hotkey.clear();
		if(count > HOTKEY_MAX_SIZE) return;  
		for(int i = 0; i < count; ++i)
		{
			_fashion_hotkey.push_back(hotkey[i]);
		}
	}

	inline void PlayerNotifyHotkey()
	{
		_runner->notify_fashion_hotkey(_fashion_hotkey.size(), &*_fashion_hotkey.begin());	
	}

	inline void RefAddBonus(int inc)
	{
		_bonusinfo.AddBonus(inc, 1);
	}

	inline void RefAddExp(int64_t inc)
	{
		ReceiveBonusExp(inc);	
	}
	
	inline void SetReferID(int id)
	{
		_referrer_id = id;
	}

	inline int GetReferID()
	{
		return _referrer_id;
	}

	inline void UpdatePVPMask(char mask)
	{
		SetPVPMask(mask);
		NotifyMasterInfo();
	}
	
	inline void SetPVPMask(char mask)
	{
		_pvp_mask = mask;
	}	

	inline char GetPVPMask()
	{
		return _pvp_mask;
	}

	bool StartTransform(int template_id, int level, int exp_level, int timeout, char type);
	void StopTransform(int template_id, char type);
	bool CanUsePotion();
	int GetTransformSkillLevel(int skill_id);
	bool GetTransformSkillData(archive & ar);

	void TerritoryItemGet(int retcode, int item_id, int item_count, int money);
	void UseComboSkill(int combo_type, int combo_color, int combo_color_num);
	void UpdateComboSkill();

	int GetCircleID() 
	{
		return GetParent()->circle_id;
	}
	void AddCirclePoint(unsigned int point);
	inline void ReceiveDeliverydExp(int64_t exp, unsigned int award_type)
	{
		int64_t exp_bonus = (int64_t)exp;
		ASSERT(exp_bonus >= 0);
		IncExp(exp_bonus);
		_runner->receive_deliveryd_exp(exp, award_type);
	}

	inline void ReceiveDeliverydItem(unsigned int award_type, unsigned int item_id)
	{
		enum
		{
			CALLBACKMAIL = 1, //快雪时晴邮件召回
		};
		
		int type = (int)award_type;
		ASSERT(type >= 0);
		switch(type)
		{
			case CALLBACKMAIL:
				{
					DeliverTaskItem(item_id, 1);
				}
				break;
			default:
				break;
		}
	}

	//vip奖励
	void ReceiveVipAwardInfoNotify(int vipserver_curtime, int start_time, int end_time, int vipaward_level, int recent_total_cash);

	void QueryCircleGroupData(const void* pData, int nSize);

	void QueryFriendNum(int taskid);
	void ReceiveFriendNum(int taskid, int number);

	bool ChargeTeleStation(int item_index, int item_id, int stone_index, int stone_id);

	bool IsTerritoryOwner(int id);
	int GetTerritoryScore();

	void PlayerTryChangeDS(char type);
	void PlayerChangeDS(char type);

	
	bool PlayerFlyTaskPos(int task_id, int npc_id, bool is_npc_task = true);	//任务直接飞一个位置
	bool PlayerFlyActivityPos(int id);	//任务直接飞一个位置
	
	void DeityDuJie();	
	bool UseBattleFlag(int flag_item_id, int flag_item_index);
	bool AddBattleFlag(int flag_item_id, int remain_time);

	//装备星座
	bool IsAstrologyActive() const { return _astrology_info.is_active;}
	bool IsAstrologyClientActive() const { return _astrology_info.client_active;}
	int GetAstrologyEnergy() const { return _astrology_info.cur_energy;}
	int GetAstrologyConsumeSpeed() const { return _astrology_info.consume_speed;}
	void GainAstrologyEnergy(int value); 
	void ConsumeAstrologyEnergy(int value);
	void ActiveAstrology();
	void DeactiveAstrology();
	void CalcAstrologyConsume();
	void UpdateAstrologyInfo();
	void ActiveAstrologyClient() {_astrology_info.client_active = true;}
	void SaveAstrologyData(archive & ar);
	void LoadAstrologyData(archive & ar);

	//趣味碰撞
	void IncCollisionWin() {_collision_info.collision_win++;}
	void IncCollisionLost() {_collision_info.collision_lost++;}
	void IncCollisionDraw() {_collision_info.collision_draw++;}

	void AddCollisionWin(int count) {_collision_info.collision_win += count;} 
	void AddCollisionLost(int count) {_collision_info.collision_lost += count;} 
	void AddCollisionDraw(int count) {_collision_info.collision_draw += count;} 

	int GetCollisionWin() const { return _collision_info.collision_win;}
	int GetCollisionLost() const { return _collision_info.collision_lost;}
	int GetCollisionDraw() const { return _collision_info.collision_draw;}

	int GetCollisionScore(){return _collision_info.collision_score + BASE_COLLISION_SCORE;}
	void ChangeCollisionScore(int score)
	{
		_collision_info.collision_score += score;
		if(_collision_info.collision_score + BASE_COLLISION_SCORE < 0)
		{
			_collision_info.collision_score = -BASE_COLLISION_SCORE;
		}
	}

	void AddCollisionScoreA(int score) { _collision_info.collision_score_a += score;}
	void AddCollisionScoreB(int score) { _collision_info.collision_score_b += score;}

	int GetCollisionScoreA() const { return _collision_info.collision_score_a;}
	int GetCollisionScoreB() const { return _collision_info.collision_score_b;}

	void SaveCollisionData(archive & ar);
	void LoadCollisionData(archive & ar);
	void SendCollisionRaidInfo();
	void ClearCollisionInfo();
	void LoadExchange6V6AwardInfo(archive & ar);
	void SaveExchange6V6AwardInfo(archive & ar);
	void SaveExchange6V6AwardInfoForClient(archive & ar);
	void ResetExchange6V6AwardInfo(); 

	int GetTaskFinishCount(unsigned long task_id);
	void VerifyActiveTaskData(size_t data_size);

	void IncPropAdd(int idx, int v);
	int  GetPropAdd(int idx);
	
	void GenPropAddItem(int materialId, short materialIdx, short materialCnt);
	void RebuildPropAddItem(int itemId, short itemIdx, int forgeId, short forgeIdx);

	void SavePropAdd(archive& ar);
	void LoadPropAdd(archive& ar);

	void SaveHideAndSeekData(archive & ar);
	void LoadHideAndSeekData(archive & ar);
	void SendHideAndSeekRaidInfo();
	void DebugAddSeekRaidScore(int score)
	{
		_hide_and_seek_info.score += score; 
		_hide_and_seek_info.hide_and_seek_timestamp = g_timer.get_systime();
	}

	virtual void PlayerEnterSeekRaid() {}
	virtual void PlayerEnterCaptureRaid() {}

	int GetRage() { return _basic.rage;}
	int GetMaxRage() { return GetMaxHP();}

	void DecRage(int value)
	{
		_basic.rage -= value;
		if(_basic.rage < 0) _basic.rage = 0;
		SetRefreshState();
	}
	void SetFacMultiExp(int multi, int endtime)
	{
		_fac_multi_exp.multi = multi;
		_fac_multi_exp.end_timestamp = endtime;
	}
	int GetFacMultiExp()
	{
		if (_fac_multi_exp.multi <= 1)
			return 0;
		if (g_timer.get_systime() > _fac_multi_exp.end_timestamp)
			_fac_multi_exp.multi = 0;
		return _fac_multi_exp.multi;
	}
protected:
	template <typename MESSAGE,typename ENCHANT_MSG>
	inline bool TestHelpfulEnchant(const MESSAGE & msg, ENCHANT_MSG & emsg)
	{
		XID attacker = emsg.ainfo.attacker;
		if(attacker.id == GetParent()->ID.id) return true; 	//如果是自己则直接返回成功
		if(emsg.ainfo.mafia_id && _mduel.InBattle())
		{
			//进行帮派野战的判断
			int rst = _mduel.AdjustEnchant(GetParent()->id_mafia, emsg);
			if(rst >= 0) return rst;
		}
		if( emsg.ainfo.mafia_id )
		{
			int rst = _fh_duel.AdjustEnchant( GetParent()->id_mafia, emsg );
			if( rst >= 0 ) return rst;
		}

		if(attacker.IsPlayerClass())
		{
			//如果自己处于PVP状态 并且施放者处于PK保护状态，则不能使用技能
			if(!(emsg.attacker_mode & attack_msg::PVP_ENABLE)
					&& (IsInPVPCombatState() ) )
			{
				return false;
			}
		}

		emsg.is_invader = false;

		if(_pk_level > 0 || _invader_counter > 0)
		{
			emsg.is_invader = true;
		}
		return true;
	}

	template <typename MESSAGE,typename ENCHANT_MSG>
	inline bool TestHarmfulEnchant(const MESSAGE & msg, ENCHANT_MSG & emsg)
	{
		//有害法术的攻击判定和攻击判定完全一样 
		//由于变量名也一致，所以可以直接调用攻击判定的函数
		return TestAttackMsg(msg, emsg);
	}

	template <typename MESSAGE,typename ATTACK_MSG>
	inline bool TestAttackMsg(const MESSAGE & msg, ATTACK_MSG & amsg)
	{
		XID attacker = amsg.ainfo.attacker;
		//空战判断
		if(attacker.IsPlayerClass() && amsg.is_flying != GetParent()->IsFlyMode()) return false;

		//攻击的攻击判定
		//不能攻击自己和队友
		if(amsg.ainfo.mafia_id && _mduel.InBattle())
		{
			//进行帮派野战的判断
			if(!_mduel.AdjustAttack(GetParent()->id_mafia, amsg)) return false;
		}
		
		//开启了PK保护分线设置后，当玩家不处于决斗和帮战状态下，红名玩家不能打白名玩家， 白名玩家可以打红名玩家
		if(gmatrix::GetWorldParam().pk_forbidden && attacker.IsPlayerClass() && GetPKLevel() == 0  
			&& !(amsg.attacker_mode & (attack_msg::PVP_DUEL|attack_msg::PVP_MAFIA_DUEL)))
		{
			return false;
		}

		bool IsInvader = false;
		if(amsg.attacker_mode & (attack_msg::PVP_DUEL|attack_msg::PVP_MAFIA_DUEL))
		{
			//决斗状态一律有效
			amsg.target_faction = 0xFFFFFFFF;
		}
		else
		{
			if( attacker.IsPlayerClass())
			{
				//如果处于PK保护状态则不被攻击
				if(!_pvp_flag_enable) return false;

				//队友和自己不会被攻击
				if(attacker.id == _parent->ID.id || (IsInTeam() && IsMember(attacker))) return false;

				// 敌对帮派攻击优先级别高，不需要强制攻击和pk开关也会打
				if( amsg.ainfo.mafia_id && _fh_duel.HaveHostiles() )
				{
					if( !_fh_duel.AdjustAttack( GetParent()->id_mafia, amsg ) ) return false;
				}
				if( amsg.attacker_mode & attack_msg::PVP_HOSTILE_DUEL )
				{
					// 敌对帮派的攻击有效果，但是组队优先
					amsg.target_faction = 0xFFFFFFFF;
					amsg.is_invader = false;
					return true;
				}

				//如果没有强制攻击则不被攻击
				if(!amsg.force_attack) return false;

				//如果对方未开PK开关也不会被攻击
				if(!(amsg.attacker_mode & attack_msg::PVP_ENABLE))
				{
					return false;
				}

				//如果开启了帮派保护，并且攻击者和被攻击者的帮派相同，则不会被攻击到
				if(!(amsg.force_attack & C2S::FORCE_ATTACK_MAFIA))
				{
					//设置了保护帮派成员 判断帮派是否匹配
					if(amsg.ainfo.mafia_id && amsg.ainfo.mafia_id == ((gplayer*)_parent)->id_mafia) return false;
				}

				//如果开启了家族保护，并且攻击者和被攻击者的家族相同，则不会被攻击到
				if(!(amsg.force_attack & C2S::FORCE_ATTACK_FAMILY))
				{
					//设置了保护帮派成员 判断帮派是否匹配
					if(amsg.ainfo.family_id && amsg.ainfo.family_id == ((gplayer*)_parent)->id_family) return false;
				}

				if(_pk_level > 0 || _invader_counter > 0)
				{
					//如果自己是红名或者粉名且对方不攻击红名则返回
					if(!(amsg.force_attack & C2S::FORCE_ATTACK_RED)) return false;
				}
				else
				{
					//如果自己是白名且对方不攻击白名则返回
					if(!(amsg.force_attack & C2S::FORCE_ATTACK_WHITE)) return false;
				}
				IsInvader = true;
			}
		}
		amsg.is_invader = IsInvader;
		return true;
	}

	inline int CalcIdlePlayerState()
	{
		//计算封印和定身的等效状态
		typedef player_state T;
		int state = 0;
		if(_silent_seal_flag) state |= 1 << T::STATE_SEAL_SILENT;
		if(_diet_seal_flag) state |= 1 << T::STATE_SEAL_DIET;
		if(_melee_seal_flag) state |= 1 << T::STATE_SEAL_MELEE;
		if(_root_seal_flag) state |= 1 << T::STATE_SEAL_ROOT;
		if(state == ((1 << T::STATE_SEAL_SILENT) 
				| (1 << T::STATE_SEAL_DIET) 
				| (1 << T::STATE_SEAL_MELEE) 
				| (1 << T::STATE_SEAL_ROOT))) 
		{
			state |= 1 << T::STATE_IDLE; 
		}
		return state;
	} 

	static bool IsMafiaHeader(char rank);
	static bool IsFamilyHeader(char rank);
public:
//各种状态下的消息处理函数
	int MarketHandler(const MSG & msg);
	int WaitEnterHandler(const MSG & msg);
	virtual int MessageHandler(const MSG & msg);
	virtual int DispatchMessage(const MSG & msg);

	
	void FromTradeToNormal(int type = 0);		//从交易状态回归到正常状态
	void FromFactionTradeToNormal(int type = 0);	//从帮派交易状态回归到正常状态
	void FromConsignToNormal(int type = 0); 	//从寄售状态回归到正常状态

	void GetPlayerCharMode(archive & ar);		//保存数据库时，将玩家当前的一些状态进行保存
	void SetPlayerCharMode(const void * buf, size_t size);
	bool VerifyWhileSave();				//存盘时的校验操作


protected:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & msg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & msg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);

public:
//虚函数群 包含了npc和玩家统一的操作和以后可能扩展的操作
	virtual void PlayerEnterMap( int old_tag = 0, const A3DVECTOR& old_pos = A3DVECTOR(0,0,0) );//玩家进入服务器，用于切换服务器
	virtual void PlayerLeaveMap();		//玩家离开服务器，用于切换服务器
	virtual void PlayerEnterWorld();		//玩家进入世界
	virtual void PlayerLeaveWorld();		//玩家离开世界
	virtual void Init(world * pPlane,gobject*parent);
	virtual void SwitchSvr(int dest_tag, const A3DVECTOR & oldpos, const A3DVECTOR &newpos,const instance_key *switch_key);
	virtual int DoAttack(const XID & target, char force_attack);
	virtual void OnAttack();
	virtual void AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg);
	virtual void OnDamage(const XID & attacker,int skill_id,const attacker_info_t&info,int damage,int dt_damage,char at_state,char stamp,bool orange);
	virtual void OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader);
	virtual void OnHeal(const XID & healer, int life);
	virtual void OnHeal(int life, bool is_potion);
	virtual void OnInjectMana(int mana, bool is_potion);
	virtual bool CanTrade(const XID & target);
	virtual void StartTrade(int trade_id,const XID & target);
	virtual bool StartFactionTrade(int trade_id,int get_mask, int put_mask,bool no_response = false);
	virtual bool StartTradeCash(int trade_id, int get_mask, int put_mask);
	virtual void TradeComplete(int trade_id,int reason,bool need_read);
	virtual void FactionTradeTimeout();
	virtual void FactionTradeComplete(int trade_id,const GNET::syncdata_t & data);
	virtual void SyncTradeComplete(int trade_id, size_t money, const GDB::itemlist & item_change,bool writetrashbox,bool money_change,bool cash_change);
	virtual void WaitingTradeWriteBack(int trade_id, bool bSuccess);
	virtual void WaitingTradeReadBack(int trade_id,const GDB::itemlist * inventory,int money);
	virtual void WaitingFactionTradeReadBack(const GDB::itemlist * inventory,int money,int timestamp);
	virtual bool CanAttack(const XID & target);
	virtual bool StandOnGround();
	virtual int TakeOutItem(int item_id, int count);
	virtual int TakeOutNonExpireItem(int item_id, int count);
	virtual int TakeOutItem(int inv_index, int item_id, int count);
	virtual int TakeOutEquipItem(int equip_index, int item_id, int count);
	virtual int TakeOutFashionItem(int fas_index, int item_id, int count);
	virtual int TakeOutTrashItem(int tra_index, int item_id, int count);
	virtual int TakeOutGiftItem(int item_index, int item_id, int count);
	virtual int TakeOutGiftItem(int item_id, int count);
	virtual int UseItemByID(gplayer_imp * pImp, int where, int item_id, size_t num, int use_type);
	virtual bool CheckItemExist(int item_id, size_t count);
	virtual bool CheckNonExpireItemExist(int item_id, size_t count);
	virtual bool CheckItemExist(int inv_index,int item_id, size_t count);
	virtual int CheckItemPrice(int inv_index, int item_id);
	virtual size_t GetMoneyAmount() { return _player_money;}
	virtual void DecMoneyAmount(size_t money) { SpendMoney(money);_runner->spend_money(money);}
	virtual void AddMoneyAmount(size_t money){GainMoney(money); _runner->task_deliver_money(money, GetMoney());}
	
	virtual size_t GetBattleScore() { return _battle_score; }
	virtual size_t GetKillingfieldScore() { return _killingfield_score; }
	virtual size_t GetArenaScore() { return _arena_score; }
	virtual void DecBattleScore( size_t score ) { SpendBattleScore( score ); _runner->spend_battle_score( score ); }
	virtual void GetBattleInfo() {}
	virtual void GetRaidInfo() {}
	virtual void GetRaidLevelReward(int level) {}
	virtual void GetRaidLevelResult() {}
	virtual void GetKillingFieldInfo(char detail) {}
	virtual void GetRaidTransformTask() {}

	virtual int DispatchCommand(int cmd_type, const void * buf,size_t size);
	virtual bool StepMove(const A3DVECTOR &offset);
	virtual void SendInfo00(link_sid *begin, link_sid *end);
	virtual void SendBuffData(link_sid *begin, link_sid *end);
	virtual void SendTeamDataToMembers();
	virtual	void OnHeartbeat(size_t tick);
	virtual void SetCombatState();
	virtual void FillAttackMsg(const XID & target, attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target,enchant_msg & enchant);
	virtual void SendAttackMsg(const XID & target, attack_msg & attack);
	virtual int  GetCSIndex() { return GetParent()->cs_index;}
	virtual int  GetCSSid() { return GetParent()->cs_sid;}
	virtual void SendEnchantMsg(int message,const XID & target, enchant_msg & attack);
	virtual void SendMsgToTeam(const MSG & msg, float range, bool exclude_self, bool norangelimit, int max_count);
	virtual bool OI_IsMember(const XID & member) { return IsMember(member);}
	virtual bool OI_IsInTeam() { return IsInTeam();}
	virtual bool OI_IsTeamLeader() { return IsTeamLeader();}
        virtual int OI_GetTeamLeader() {
                if(!IsInTeam()) return 0;
                return _team.GetLeader().id;
        }
	virtual void KnockBack(const XID & target, const A3DVECTOR & source, float distance) {}
	virtual void AddAggroToEnemy(const XID & who,int rage);
	virtual void BeTaunted2(const XID & who,int time);
	virtual bool DrainMana(int mana);
	virtual bool DecDeity(int deity); 
	virtual bool DecInk(int ink); 
	virtual void OnSetSealMode();
	virtual bool Resurrect(float exp_reduce);
	virtual void OnDuelStart(const XID & target);
	virtual void OnDuelStop();
	virtual int GetDuelTargetID() { return _duel_target; }
	virtual void Die(const XID & attacker, bool is_pariah, char attacker_mode);
	virtual void OnDeath(const XID & lastattack,bool is_pariah, bool no_drop, bool is_hostile_duel = false, int time = 0 );
	virtual void OnUsePotion(char type, int value) {}
	virtual void OnGatherMatter(int matter_tid, int cnt) {}
	virtual bool IsInBattleCombatZone() { return false; }
	virtual void OnPickupMoney(size_t money,int drop_id);
	virtual void OnPickupItem(const A3DVECTOR & pos,const void * data, size_t size,bool isTeam,int drop_id);
	virtual void OnAttacked(const MSG & msg, attack_msg * attack, damage_entry & dmg,bool hit); 
	virtual void OnEnchant(const MSG & msg, enchant_msg * enchant);
	virtual void PlayerForceOffline();
	virtual void ServerShutDown();
	virtual void UpdateMafiaInfo(int m_id, int f_id, char rank);	//更新帮派信息
	virtual void OnUpdateMafiaInfo(){}
	virtual const A3DVECTOR & GetLogoutPos(int & world_tag);//取得登出时应该使用的坐标，可以被覆盖
	virtual bool CheckCoolDown(int idx);
	virtual void SetCoolDown(int idx, int msec);
	virtual void ClrCoolDown(int idx);
	virtual void ClrAllCoolDown();
	virtual bool CheckAndSetCoolDown(int idx, int msec);
	virtual void ReduceSkillCooldown(int idx, int msec);
	virtual void GetCoolDownData(archive & ar);
	virtual void GetCoolDownDataForClient(archive & ar);
	virtual int GetMonsterFaction() { return PLAYER_MONSTER_TYPE;}
	virtual int GetFactionAskHelp() { return PLAYER_MONSTER_TYPE;}
	virtual void EnableFreePVP(bool bVal);
	virtual void ObjReturnToTown();
	virtual void AddEffectData(short effect);
	virtual void RemoveEffectData(short effect);
	virtual bool IsPlayerClass() { return true;}
	virtual int GetObjectType() { return TYPE_PLAYER;}
	virtual int GetLinkIndex();
	virtual int GetLinkSID();
	virtual size_t OI_GetPetBedgeInventorySize();
	virtual int OI_GetPetBedgeInventoryDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetPetEquipInventorySize();
	virtual int OI_GetPetEquipInventoryDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetInventorySize();
	virtual int OI_GetInventoryDetail(GDB::itemdata * list, size_t size);
	virtual int OI_GetEquipmentDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetEquipmentSize();
	virtual size_t OI_GetPocketInventorySize();
	virtual int OI_GetPocketInventoryDetail(GDB::pocket_item * list, size_t size);
	virtual size_t OI_GetFashionInventorySize();
	virtual int OI_GetFashionInventoryDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetMountWingInventorySize();
	virtual int OI_GetMountWingInventoryDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetGiftInventorySize();
	virtual int OI_GetFuwenInventoryDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetFuwenInventorySize();
	virtual int OI_GetGiftInventoryDetail(GDB::itemdata * list, size_t size);
	virtual int TradeLockPlayer(int get_mask, int put_mask);
	virtual int TradeUnLockPlayer();
	virtual int TradeCashLockPlayer(int get_mask, int put_mask);
	virtual int TradeCashUnLockPlayer();
	virtual void ActiveMountState(int mount_id, int mount_lvl, bool no_notify, char mount_type = 0);
	virtual void DeactiveMountState();
	virtual bool OI_IsMafiaMember();
	virtual int OI_GetMafiaID();
	virtual int OI_GetFamilyID();
	virtual int OI_GetMasterID();
	virtual int OI_GetZoneID(); 
	virtual bool CheckGMPrivilege();
	virtual size_t OI_GetTrashBoxCapacity();
	virtual int OI_GetTrashBoxDetail(GDB::itemdata * list, size_t size);
	virtual size_t OI_GetMafiaTrashBoxCapacity();
	virtual int OI_GetMafiaTrashBoxDetail(GDB::itemdata * list, size_t size);
	virtual bool OI_IsTrashBoxModified();
	virtual bool OI_IsEquipmentModified();
	virtual size_t OI_GetTrashBoxMoney();
	virtual bool EnterBattleground( int target_battle_tag, int faction, int battle_type );
	virtual int OI_GetDBTimeStamp();
	virtual int OI_InceaseDBTimeStamp();
	virtual int  Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int target_tag, float hp_recover = 0.1f);
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();
	virtual bool OI_IsPVPEnable() { return _pvp_flag_enable;}
	virtual bool OI_IsInPVPCombatState() { return IsInPVPCombatState();}
	virtual bool OI_IsMount() { return  GetParent()->IsMountMode();}
	virtual bool OI_IsFlying() { return  InFlying();}
	virtual bool OI_IsGathering(); 
	virtual void OI_DenyAttack(){_commander->DenyCmd(controller::CMD_ATTACK);} 
	virtual void OI_AllowAttack(){ _commander->AllowCmd(controller::CMD_ATTACK);} 
	virtual int CheckUseTurretScroll();
	virtual void ModifyItemPoppet(int offset);
	virtual void ModifyExpPoppet(int offset);
	virtual bool OI_GetMallInfo(int &cash_used, int & cash, int &cash_delta,  int &order_id);
	virtual int OI_GetBonus();
	virtual void OI_AddBonus(int inc);
	virtual bool OI_IsCashModified();
	virtual bool CanTeamRelation();
	virtual void OI_UpdateAllProp();
	virtual size_t OI_GetMallOrdersCount();
	virtual int OI_GetMallOrders(GDB::shoplog * list, size_t size);
	virtual void PostLogin(const userlogin_t & user, char trole, char loginflag) {};
	virtual void OI_ToggleSanctuary(bool active);
	virtual void OI_TalismanGainExp(int exp, bool is_aircraft);
	virtual void OI_ReceiveTaskExp(int exp) { ReceiveTaskExp(exp);}
	virtual void OnAntiCheatAttack();
	virtual bool ModifyTalentPoint(int offset);
	virtual bool OI_TestSafeLock();
	virtual int OI_RebornCount();
	virtual int OI_SpendTalismanStamina(float cost);
	virtual size_t GetPetCivilization() const { return _petman.GetPetCivilization(); }
	virtual size_t GetPetConstruction() const { return _petman.GetPetConstruction(); }
	virtual int QueryRebornInfo(int level[3], int prof[4]);
	virtual unsigned int GetDBMagicNumber();
	virtual int SkillCreateItem(int id, int count, int period);
	virtual int OI_GetRegionReputation(int index) { return GetRegionReputation(index);}
	virtual void OI_ModifyRegionReputation(int index, int rep) { return ModifyRegionReputation(index, rep);}
	virtual int OI_GetCultivation() { return _cultivation;}
	virtual char OI_GetPVPMask() { return GetPVPMask();}
	virtual void CurseTeleport(const XID & who);
	virtual void SetPuppetForm(bool on, int num);
	virtual void SetPuppetSkill(int skill_id, int immune_prob);
	virtual void BounceTo(const  A3DVECTOR & target_pos, float distance);
	virtual void DrawTo(const XID & target, const A3DVECTOR & pos);
	virtual void Repel2(const A3DVECTOR & target_pos, float distance);
	virtual void Throwback(const A3DVECTOR & target_pos, float dist);
	virtual void TryCharge(int type, const A3DVECTOR & dest_pos, int target_id);
	virtual void CastCycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage, 
			int mpcost, int hpcost,int dpcost, int inkcost, char force, float ratio, int plus, int period, int var[16], int crit_rate);
	bool TryCastCycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage,
		int mpcost, int hpcost,int dpcost, int inkcost, float ratio, int plus, char force, int var[16], int crit_rate);
	virtual void SetQilinForm(bool on);
	virtual void SetRage(int v);
	virtual void AddRage(int value)
	{
		_basic.rage += value;
		if(_basic.rage > GetMaxHP()) _basic.rage = GetMaxHP();
		SetRefreshState();
	}
	virtual int GetPuppetid();
	virtual void NotifyBloodPoolStatus(bool on, int cur_hp, int max_hp);
	virtual void OnChangeShape(int shape);

	virtual bool OI_IsSpouse(const XID & target);
	virtual bool OI_IsMarried() { return IsMarried(); }
	virtual void OI_JumpToSpouse();
	virtual void OI_MagicGainExp(int exp){MagicGainExp(exp);} 
	virtual void OI_AddProficiency(int id, int add){_skill.AddProficiency(this, id, add);}
	virtual void OI_AddTransformSkill(int skill_id, int skill_level);
	virtual void OI_OnSilentSeal();
	virtual bool OI_StartTransform(int template_id, int level, int exp_level, int timeout, char type) {return StartTransform(template_id, level, exp_level, timeout, type);}
	virtual void OI_StopTransform(int template_id, char type){return StopTransform(template_id, type);}
	virtual int OI_GetTransformID();
	virtual int OI_GetTransformLevel();
	virtual int OI_GetTransformExpLevel();
	virtual bool OI_IsCloneExist();
	virtual void OI_UnSummonMonster(char type);
	virtual bool OI_SummonExist(int tid, XID& id);

	virtual void OI_AddHP(int hp);
	virtual void OI_AddMP(int mp);
	virtual void OI_AddDefense(int defense);
     	virtual void OI_AddAttack(int attack);
	virtual void OI_AddArmor(int armor);
	virtual void OI_AddDamage(int damage);
	virtual void OI_AddResistance(const int resistance[6]);
	virtual void OI_AddCrit(int crit_rate, float crit_damage);
	virtual void OI_AddSpeed(float speed);
	virtual void OI_AddAntiCrit(int anti_crit, float anti_crit_damage);
	virtual void OI_AddSkillRate(int skill_attack_rate, int skill_armor_rate);
	virtual void OI_AddAttackRange(float range);
	virtual void OI_AddScaleHP(int hp);
	virtual void OI_AddScaleMP(int mp);
	virtual void OI_AddScaleDefense(int defense);
	virtual void OI_AddScaleAttack(int attack);
	virtual void OI_AddScaleArmor(int armor);
	virtual void OI_AddScaleDamage(int damage);
	virtual void OI_AddScaleSpeed(int speed);
	virtual void OI_AddScaleResistance(const int resistance[6]);


	virtual bool QueryBattlefieldConstructInfo(int type,int page = 0) { return false; }
	virtual bool BattlefieldContribute(int res_type) { return false; }
	virtual bool BattlefieldConstruct(int key,int type,int cur_level) { return false; }

	int PetServiceBaseCheck(size_t inv_index,size_t fee,bool check_lock = true);
	virtual int PlayerFreePet(int pet_index);
	virtual int FreePet(int inv_index);
	virtual int PlayerAdoptPet(int inv_index);
	virtual int AdoptPet(int inv_index);
	virtual int PlayerRefinePet(int inv_index,int cid,int aid);
	virtual int PlayerRefinePet2(int inv_index,int type);
	virtual int RefinePet(int inv_index,int cid,int aid);
	virtual int RefinePet2(int inv_index,int type);
	virtual int RenamePet(int inv_index,const char* buf,int size);
	virtual bool PlayerRebornPet(size_t inv_index, size_t assist_index);
	virtual bool RebornPet(size_t inv_index, size_t assist_index);
	virtual bool PlayerChangePetShape(size_t pet_index, int shape_id);
	virtual bool PlayerRefinePetAttr(size_t pet_index, size_t attr_type, size_t assist_id, size_t assist_count);
	
	virtual bool UseItem(item_list & inv, int inv_index, int where, int item_type);
	virtual bool UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size);
	
	virtual void DumpBattleSelfInfo(){}
	virtual void DumpBattleInfo(){}
	virtual void DebugIncBattleScore(int inc){}	
	virtual void SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data, size_t dsize);
	
	virtual void ChangeKillingfieldScore (int value){}
	virtual void ResetKillingfieldScore(){}
	virtual bool IsTransformState()
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsTransformMode();
	}
	
	virtual bool IsBindState()
	{
		return _pstate.IsBindState();
	}

	virtual bool CanDropExpPVPMode() const {return false;}
	virtual void PlayerLeaveTerritory(){}
	virtual void PlayerChangeTerritoryResult(int result){}
	
	virtual void PlayerLeaveKingdom(){}
	virtual void PlayerGetCollisionPlayerPos(int roleid){}
	virtual bool PlayerOpenTrashBox(unsigned int passwd_size, char passwd[]);

	virtual void ClearComboSkill();
	virtual void GetComboColor(int& c1, int& c2, int& c3, int& c4, int& c5);

	virtual bool CanSetInvisible();
	virtual void SetInvisible(int invisible_rate);
	virtual void ClearInvisible(int invisible_rate);
	virtual XID GetCloneID();
	virtual XID GetSummonID();
	virtual const XID & GetCurTarget();
	virtual void SetDimState(bool is_dim);
	virtual void ExchangePos(const XID & who);
	virtual void ExchangeStatus(const XID & who);
	virtual void SetAbsoluteZone(int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state);
		
	virtual int CanConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time);
	virtual int CanConsignRole(char type, int item_idx, int sold_time, int price, int margin);
	virtual void StartConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, char* sellto_name, char sellto_name_len);
	virtual void StartConsignRole(char type, int item_id, int item_idx, int item_cnt, int item_type, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len);
	virtual void CompleteConsign(int reason, char need_read);
	virtual void WaitingConsignWriteBack(bool bSuccess, char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, char* sellto_name, char sellto_name_len);
	virtual void WaitingConsignReadBack(const GDB::itemlist * inventory,int money);

	virtual bool IsIgniteState() 
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsIgniteState();
	}
	virtual bool IsFrozenState()
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsFrozenState();
	}
	virtual bool IsColdInjureState()
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsColdInjureState();
	}
	virtual bool IsFuryState() 
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsFuryState();
	}

	virtual bool IsTurnBuffState()
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsTurnBuffState();
	}

	virtual bool IsTurnDebuffState()
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsTurnDebuffState();
	}

	virtual bool IsBloodThirstyState() 
	{
		gplayer * pPlayer = GetParent();
		return pPlayer->IsBloodThirstyState();
	}

	virtual void SetIgniteState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_IGNITE);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_IGNITE);
		}
		_runner->object_special_state(S2C::STATE_IGNITE, on);
	}
	
	virtual void SetFrozenState(bool on) 
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_FROZEN);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_FROZEN);
		}
		_runner->object_special_state(S2C::STATE_FROZEN, on);
	}

	virtual void SetFuryState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_FURY);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_FURY);
		}
		_runner->object_special_state(S2C::STATE_FURY, on);
	}

	virtual void SetColdInjureState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_COLDINJURE);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_COLDINJURE);
		}
	}
	
	virtual void SetDarkState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_DARK);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_DARK);
		}
		_runner->object_special_state(S2C::STATE_DARK, on);
	}

	virtual void SetCycleState(bool on)
	{
		_runner->object_special_state(S2C::STATE_CYCLE_SKILL, on);
	}

	virtual void SetTurnBuffState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_TURNBUFF);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_TURNBUFF);
		}
	}

	virtual void SetTurnDebuffState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_TURNDEBUFF);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_TURNDEBUFF);
		}
	}

	virtual void SetBloodThirstyState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_BLOODTHIRSTY);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_BLOODTHIRSTY);
		}
	}

	virtual void SetBePulledState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_BEPULLED);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_BEPULLED);
		}
	}
	
	virtual void SetBeSpiritDraggedState(bool on)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_BESPIRITDRAGGED);
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_BESPIRITDRAGGED);
		}
	}

	virtual void SetExtraEquipEffectState(bool on, char level)
	{
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetExtraState(gplayer::STATE_EXTRA_EQUIP_EFFECT);
			pPlayer->weapon_effect_level = level;
		}
		else
		{
			pPlayer->ClrExtraState(gplayer::STATE_EXTRA_EQUIP_EFFECT);
			pPlayer->weapon_effect_level = 0;
		}
		_runner->player_extra_equip_effect();
	}

	//爬塔副本相关
	virtual void PlayerStartRandomTowerMonster(char client_idx) {}

	//检查碰撞战场里面的移动
	virtual void CheckSpecialMove(){}
	//是否要检查速度合法(碰撞战场不需要检查)
	virtual bool NeedCheckMove(){return !_no_check_move;}
	virtual void SetNoCheckMove(bool val){_no_check_move = val;}
	virtual void StartSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp){}
	virtual void SpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, unsigned short stamp, char collision_state, int timestamp){}
	virtual void StopSpecialMove(unsigned char dir, A3DVECTOR & cur_pos, unsigned short stamp){}
	
	//碰撞战场技能
	virtual void SetCollisionMaxSpeed()
	{
		_runner->collision_special_state(S2C::COLLISION_SKILL_MAXSPEED, true);
	}

	virtual void SetCollisionHalfSpeed()
	{
		_runner->collision_special_state(S2C::COLLISION_SKILL_HALFSPEED, true);
	}

	virtual void SetCollisionAshill(bool on)
	{
		/*
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetCollisionState(gplayer::STATE_COLLISION_ASHILL);
		}
		else
		{
			pPlayer->ClrCollisionState(gplayer::STATE_COLLISION_ASHILL);
		}
		_runner->collision_special_state(S2C::COLLISION_SKILL_ASHILL, on);
		*/
	}

	virtual void SetCollisionNomove(bool on)
	{
		/*
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetCollisionState(gplayer::STATE_COLLISION_NOMOVE);
		}
		else
		{
			pPlayer->ClrCollisionState(gplayer::STATE_COLLISION_NOMOVE);
		}
		_runner->collision_special_state(S2C::COLLISION_SKILL_NOMOVE, on);
		*/
	}

	virtual void SetCollisionChaos(bool on)
	{
		/*
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetCollisionState(gplayer::STATE_COLLISION_CHAOS);
		}
		else
		{
			pPlayer->ClrCollisionState(gplayer::STATE_COLLISION_CHAOS);
		}
		_runner->collision_special_state(S2C::COLLISION_SKILL_CHAOS, on);
		*/
	}

	virtual void SetCollisionVoid(bool on)
	{
		/*
		gplayer * pPlayer = GetParent();
		if(on)
		{
			pPlayer->SetCollisionState(gplayer::STATE_COLLISION_VOID);
		}
		else
		{
			pPlayer->ClrCollisionState(gplayer::STATE_COLLISION_VOID);
		}
		_runner->collision_special_state(S2C::COLLISION_SKILL_VOID, on);
		*/
	}

	virtual void DebugChangeCollisionResult(int faction, int score){}
	

	virtual void OnMineStart(int mine_id){}
	virtual void OnMineEnd(int item_id){}
	virtual bool IsResurrectPosActive(){return true;}

	virtual void AddFacBuilding(int fid, int index, int tid) {}
	virtual void UpgradeFacBuilding(int fid, int index, int tid, int cur_lev) {}
	virtual void RemoveFacBuilding(int fid, int index, int tid) {}
	virtual void PlayerGetFacBase() {}
	virtual void DebugChangeBaseProp(int type, int delta) {}
	virtual void FacBaseTransfer(int index) {}

	virtual int GetFacBuildingLevel(int iBuildId) { return 0; } //只有在自己的基地中才能获得正确值

	virtual int GetFBaseGrass() { return 0; }//只有在自己的基地中才能获得正确值
	virtual void DeductFBaseGrass(int num) {} //只有在自己基地中才能正确扣减
	virtual void DeliverFBaseGrass(int num);//基类提供默认实现 子类会重载

	virtual int GetFBaseMine() { return 0; }//只有在自己的基地中才能获得正确值
	virtual void DeductFBaseMine(int num) {} //只有在自己基地中才能正确扣减
	virtual void DeliverFBaseMine(int num);//基类提供默认实现 子类会重载

	virtual int GetFBaseMonsterFood() { return 0; }//只有在自己的基地中才能获得正确值
	virtual void DeductFBaseMonsterFood(int num) {} //只有在自己基地中才能正确扣减
	virtual void DeliverFBaseMonsterFood(int num);//基类提供默认实现 子类会重载

	virtual int GetFBaseMonsterCore() { return 0; }//只有在自己的基地中才能获得正确值
	virtual void DeductFBaseMonsterCore(int num) {} //只有在自己基地中才能正确扣减
	virtual void DeliverFBaseMonsterCore(int num);//基类提供默认实现 子类会重载

	virtual int GetFBaseCash() { return 0; }//只有在自己的基地中才能获得正确值
	virtual void DeductFBaseCash(int num) {} //只有在自己基地中才能正确扣减
	virtual void DeliverFBaseCash(int num);//基类提供默认实现 子类会重载
	virtual void OnFBaseBuildingProgress(unsigned long task_id, int value);//基类提供默认实现 子类会重载
	virtual int GetFacInBuildingLevel() { return 0; } //只有在基地中才能获得正确值

	virtual void WithdrawFacBaseAuction(int item_index) {} //要在基地中才能领取
	virtual void WithdrawFacBaseCoupon() {} //要在基地中才能领取
	virtual bool CheckEnterBase();
	virtual bool LeaveFacBase() { return false; }
	virtual void DeliveryFBaseAuction(int itemid);

	virtual bool PlayerGetHideAndSeekAward();
	virtual bool PlayerBuySeekerSkill();
	void PlayerApplyHideAndSeekRaid(int raid_id, int raid_template_id, bool is_team);

	virtual void PlayerSubmitCaptureRaidFlag() {}
	virtual int CheckCanMineInCaptureRaid(int target) { return 0; }
	void PlayerApplyCaptureRaid(int raid_id, int raid_template_id, bool is_team);

	//轩辕光暗值
	void SetDarkLight(int);
	int GetDarkLight();
	void IncDarkLight(int);
	void DecDarkLight(int);

	void SetDarkLightForm(char s);
	void ClearDarkLightForm();
	bool IsDarkLightForm(char s);
	virtual char GetDarkLightForm();

	//轩辕灵体
	void SetDarkLightSpirit(int idx, char type);
	char GetDarkLightSpirit(int idx);
	void AddDarkLightSpirit(char type);
	void DelDarkLightSpirit(int idx);
	void OrderDarkLightSpirit();

	void CastDarkLightSpiritOnAttackSkill(const XID& target);

	void SummonCastSkill(int summonId, const XID& target, int skillid, int skilllevel);

	bool AddSpiritSession(act_session* ses);
	void EndSpiritSession(int session_id);
	void TerminateSpiritSession(int session_id);
	void ClearSpiritSession();

	bool GetTalismanRefineSkills(int& level, std::vector<short>& skills);
	void SetTalismanEmbedSkillCooltime(int skill_id, int cooltime);
	void ActiveTalismanSkillEffects(const XID& target, const msg_talisman_skill* mts);
	virtual void CastTransformSkill(int skill_id, char skill_type, char force_attack, unsigned char target_count, int *targets, const A3DVECTOR& pos);

	bool FillPotionBottle(int bottle_id, int bottle_index, int potion_id, int potion_index);
	void CheckBuffArea();
	
	void ProduceJinfashen(char produce_type, char produce_idx, int zhaohua[3], int maCnt);

public:
//逻辑操作函数群
	void DecSpiritPower(size_t index);
	void CalcEquipmentInfo();		//计算装备代表的数据内容
	bool ObtainItem(int where, item_data * pData, bool isTask = false,int deliver_type = ITEM_INIT_TYPE_TASK);//返回false表示这个data被使用了，不要释放
	bool ObtainItem(int where, item_data * pData, int& inv_idx, bool isTask = false,int deliver_type = ITEM_INIT_TYPE_TASK);//返回false表示这个data被使用了，不要释放
//	void DropItemOnDeath(size_t drop_count_inv,size_t drop_count_equip);
	void DropItemOnDeath(size_t inv_drop,size_t eq_drop, const XID & owner , int team_id, int team_seq);
	bool ReturnToTown();
	bool CanResurrect();
	bool SitDown();
	bool CanSitDown();
	void StandUp();
	void SelfHurt(int dmg);
	void PurchaseItem(abase::pair<const item_data *, int> *pItem,size_t size,size_t money);
	bool ItemToMoney(size_t inv_index, int type, size_t count);
	float CalcIncMoney(size_t inv_index, int type, size_t count);
	bool UseItemWithTarget(item_list & inv, int inv_index, int where, int item_type,const XID& target, char force_attack);
	inline bool UseItemWithTarget(int where,size_t inv_index,int item_type,const XID & target,char force_attack) //内部调用
	{
		ASSERT(where == IL_INVENTORY || where == IL_EQUIPMENT);
		item_list & inv = GetInventory(where);
		ASSERT(inv_index < inv.Size());
		if(inv[inv_index].type != item_type) return false;
		return UseItemWithTarget(inv,inv_index,where, item_type, target,force_attack);
	}

	inline bool UseItem(int where, size_t inv_index, int item_type) //内部调用
	{
		ASSERT(where == IL_INVENTORY || where == IL_EQUIPMENT || where == IL_MOUNT_WING);
		item_list & inv = GetInventory(where);
		ASSERT(inv_index < inv.Size());
		if(inv[inv_index].type != item_type) return false;
		return UseItem(inv,inv_index,where,item_type);
	}
	bool EmbedChipToEquipment(size_t chip,size_t equip);
	void LongJump(const A3DVECTOR &pos); 			//空间跳转
	virtual bool LongJump(const A3DVECTOR &pos,int target_tag); 	//穿越位面跳转
	void DebugJump();
	void Swap(gplayer_imp * rhs);		//交换玩家数据，以更改玩家逻辑
	bool TestSanctuary();                   //检查是否出于安全区
	bool TestSanctuaryNormal();             //检查是否出于安全区
	bool TestRegion();			//检查区域里面的等级和飞升条件限制
	int CheckPlayerMove(const A3DVECTOR & offset, int mode, int use_time);
	void SetCoolDownData(const void * buf, size_t size);
	void SwitchFashionMode();
	void SetFashionMask(int fashion_mask);
	bool RegionTransport(int rindex, int tag);
	void EnterResurrectReadyState(float exp_reduce, float hp_recover = 0.1f);
	void RebuildAllInstanceData(int create_time);	  //重建所有的数据
	void RebuildInstanceKey();			  //重建所有的key
	bool GetInstanceKeyBuf(char * buf, size_t size);  //取得Instance Key buf
	bool SetInstanceKeyBuf(const void * buf, size_t size); //设置所有的instance key 
	int  CheckInstanceTimer(int tag);		  //检查是否可以进行副本重置，如果可以设置当前的timer
	void SetInstanceTimer(int tag);			  //设置进入副本的时间，用来限定是否可以进行副本重置的 
	void SaveInstanceKey(archive & ar);		  //保存副本key数据进入数据库
	void SaveVipAwardData(archive & ar);						//保存VIP奖励信息
	void LoadVipAwardData(const void * data, size_t size);		//从DB载入VIP奖励数据
	void SaveOnlineAwardData(archive & ar);						//保存在线倒计时奖励数据
	void LoadOnlineAwardData(const void * data, size_t size);	//从DB载入在线倒计时奖励数据
	void InitInstanceKey(int create_time, const void * buf , size_t size);	//从数据库初始化副本数据，只有在buf和size为0的时候create_time才有用
	void CosmeticSuccess(int ticket_inv_idx, int ticket_id, int result, unsigned int crc);
	void GainMoneyWithDrop(size_t inc, bool log = true);
	void GainMoney(size_t inc);		//no drop
	void SpendMoney(size_t delta,bool log = true);
	bool CheckIncMoney(size_t inc);
	bool CheckDecMoney(size_t delta);

	void GainBattleScore( size_t inc );
	void SpendBattleScore( size_t dec, bool log = true );
	bool CheckIncBattleScore( size_t inc );
	bool CheckDecBattleScore( size_t dec );
	
	void LeaveDoubleExpMode();
//	int EnterDoubleExpMode(int time_mode, int timeout);	//返回当前可用的双倍时间
//	void ActiveDoubleExpTime(int time, bool is_rest = false);
	void ActiveOfflineAgentTime(int agent_minute);



	void LoadMultiExp(archive & ar);
	void SaveMultiExp(archive & ar);
	void ActiveMultiExpTime(int multi_exp, int time);
	void UpdateMultiExp(bool need_notify);
	int GetMultiExp();
	void SendMultiExp();
	void ClearMultiExp();
	int GetMultiExpTime(int multi_exp);

	void TrashBoxOpen(int type);
	void TrashBoxClose();
	void TryClearTBChangeCounter();

	bool CheckPlayerBindRequest();
	bool CheckPlayerBindInvite();
	bool EnterBindMoveState(const XID & target, const char type);
	bool EnterBindFollowState(const XID & leader, const char type, const char pos);
	void ReturnBindNormalState(char link_type);

	int SummonPet(size_t pet_index);
	int RecallPet(size_t pet_index);
	int CombinePet(size_t pet_index,int type);
	int UncombinePet(size_t pet_index,int type);
	void SetLeaderData(leader_prop & data);
	void NotifyStartAttack(const XID& target,char force_attack);
	void ClearPetSkillCoolDown();
	void PlayerGetSummonPetProp();

	void SayHelloToNPC(const XID & target);

	void LeaveAbnormalState();		//试图离开非正常状态
	bool TaskLeaveAbnormalState();		//任务试图离开非正常状态
	void SendFarCryChat(char channel, const void * msg, size_t size, const void * data, size_t dsize);
	void SendGlobalChat(char channel, const void * msg, size_t size, const void * data, size_t dsize, int item_idx);
	void SendRumorChat(char channel, const void * msg, size_t size, const void * data, size_t dsize, int item_idx);
	void SendZoneChat(char channgel, const void * msg, size_t size, const void * data, size_t dsize);
	void NotifyMasterInfo();
	bool RefineItemAddon(size_t index, int item_type, size_t stone_index, int rt_index);
	bool UpgradeEquipment(size_t index, int item_type, size_t stone_index, int rt_index);
	bool RemoveRefineAddon(size_t index, int item_type, bool remove_pstone);
	bool ProduceItem(const  recipe_template & rt, bool npc_produce = false);
	void ResetPKValue(int point, size_t need_money);
	void IncProduceExp(int exp);
	void OnPickupRawMoney(size_t money,bool isteam);
	void RemoveAllExpireItems();
	void ClearHurtEntry();
	void AddHurtEntry(const XID & attacker, int damage, int team,int seq);
	void GetMaxHurtTeam(XID & owner, int team_id, int team_seq);
	void ChangeInventorySize(int size);
	void ChangePocketSize(int size);
	bool CheckItemBindCondition(size_t index,  int id);
	bool CheckItemLockCondition(size_t index,  int id);
	bool BindItem(size_t index, int id);
	bool CheckBindItemDestroy(size_t index, int id);
	bool CheckItemUnlock(size_t index,  int id);
	bool DestroyBindItem(size_t index, int id, item_list& inv, int where = IL_INVENTORY,  int type = 0);	//type == 0 是普通摧毁模式  type == 1是碎片模式
	bool CheckRestoreDestroyItem(size_t index, int id);
	bool CheckRestoreBrokenItem(size_t index, int id);
	bool RestoreDestroyItem(size_t index, int id);
	bool ForgetSkill(int type);
	void ForgetCulSkills();
	void ForgetDeitySkills();
	void DoTeamRelationTask(int reason);
	void PVPCombatHeartbeat();
	void DeliverItem(int item_id,int count, bool bind, int lPeriod, int deliver_type);
	void DeliverItem(int item_id,int count, bool bind, int lPeriod, int deliver_type, int& inv_idx);
	void GainItem(int item_id,int count, bool bind, int lPeriod, int deliver_type);	//可以超过堆叠
	void DeliverTaskItem(int item_id, int count);
	void DeliverReinforceItem(int item_id,int reinforce_level, bool bind, int lPeriod,int deliver_type);
	void StartRecorder();
	void QueryRecorder(size_t record_item);
	void ChangePlayerGameServer(int target_gs, int world_id, const A3DVECTOR & pos, int reason);
	bool TalismanGainExp(int exp, bool is_aircraft);
	bool RefineTalisman(size_t index, int id, int material_id);
	bool TalismanLevelUp(size_t index, int item_type);
	bool TalismanReset(size_t index, int item_type);
	bool TalismanCombine(size_t index1, int item_type1, size_t index2, size_t item_type2, int catalyst_id);
	bool TalismanEnchant( size_t index1, int item_type1, size_t index2, int item_type2 );
	bool TalismanHolyLevelup(size_t talisman_index, int talisman_id, int levelup_id);
	bool TalismanEmbedSkill(size_t talisman1_index, int talisman1_id, size_t talisman2_index, int talisman2_id, int needitem1_id, int needitem1_index, int needitem2_id, int needitem2_index);
	bool TalismanSkillRefine(size_t talisman1_index, int talisman1_id, size_t talisman2_index, int talisman2_id, int needitem_id);
	bool TalismanSkillRefineResult(size_t talisman1_index, int talisman1_id, char result);
	
	void SetFamilySkill(const int * list, int size);
	void ReceiveShareExp(int64_t exp);
	void FindCheater(int type, bool noreport);
	void FindCheater2();
	void PunishCheater();
	void SendTeamChat(char channel, const void * buf, size_t len, const void * aux_data, size_t dsize, int useid = 0);
	void SetChatEmote(int emote_id);
	void TaskSendMessage(int task_id, int channel, int param, long var[3]);
	void LotteryTySendMessage(int item_id);
	void WallowControl(int level, int msg, int p_time, int l_time, int h_time);
	void QuestionBonus();
	void InitSkillData(const void * data, size_t size);
	void InitCombineSkillData(const void * data, size_t size);
	bool CanMetempsychosis();
	bool Metempsychosis(int new_prof);
	void ActiveRebornBonus(bool active);
	bool PhaseControl(const A3DVECTOR & target, float terrain_height, int mode, int use_time);
	bool RenewMountItem(size_t inv_mount, size_t inv_material);
	void StartFly();
	virtual void PreFly();
	virtual bool CheckCanFly();
	virtual void ActiveFlyState(char type);
	virtual void DeactiveFlyState(char type);
	int TransformChatData(const void * data,size_t dsize, void * out_buffer, size_t len);
	void UpdateSectID(int id);
	static int64_t GetAgentExpBonus(int level, int reborn_count, bool is_offline);
	int DoLockItem(size_t index, int id);
	int DoUnlockItem(size_t index, int id);
	void RestoreUnlock(item_list & list, int index, int where ,const item & it);
	void DisappearOnDrop(item_list & inv,int index, int where,int max_count);
	bool RefinePetEquip(size_t item_index, int item_type, size_t stone_index);
	
	int GetMountType()
	{
		gplayer * pPlayer = GetParent();
		if(pPlayer && pPlayer->IsMountMode()){
			return pPlayer->mount_type;
		}
		return 0;
	}
	void HandleChangeStyle(unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid);
	void DeliveryReferrAward();
	bool CheckTradeSpecialReq(int special_id, int count);
	void ConsumeTradeSpecialReq(int special_id, int count);
	void PlayerDiscover(int discover_type); 
	
	void SendAchievementMessage(unsigned short achieve_id, int broad_type, int param, int finish_time);
	void SendPKMessage(int killer, int deader);
	void SendRefineMessage(int item_id, int refine_level);
		
	void TryEnableAchievement();
	
	void OnTaskComplete(int task_id, int count);
	bool IsAchievementFinish(unsigned short id);
	void FinishAchievement(unsigned short id);
	void UnfinishAchievement(unsigned short id);

	bool RefineMagic(size_t index, int id, size_t stone_index, int stone_id, int refine_type);
	bool RestoreMagic(size_t index, int id);
	int DecMagicDuration(int dec);
	bool MagicGainExp(int exp);

	void ActiveTransformState();
	void OnActiveTransformState();
	void DeactiveTransformState();
	void OnDeactiveTransformState();

	bool IsVisibleTo(int player_id);
	void SendDisappearToTeam();
	void SendDisappearToTeamMember(int id,int cs_index,int cs_sid);
	void SendAppearToTeam();
	void SendAppearToTeamMember(int id,int cs_index,int cs_sid);
	void ClearEnemyAggro();

	void SetCloneInfo(gsummon_imp * sImp, bool exchange);
	void DecInventoryItem(int index, int count, int drop_type);

	//Add by Houjun 2011-03-09, 宝石相关操作
	//宝石升品	
	bool UpgradeGemLevel(int gem_id, int gem_index, int upgradeItemId[12], int upgradeItemIndex[12]);

	//宝石精练
	bool UpgradeGemQuality(int gem_id, int gem_index, int upgradeItemId, int upgradeItemIndex);

	//宝石萃取
	bool ExtraceGem(int gem_id, int gem_index);

	//宝石融合
	bool SmeltGem(int src_gem_id, int src_gem_index, int dest_gem_id, int dest_gem_index, int smelt_item_id, int smelt_item_index);

	//创建一个新物品,item_id, item_cnt, 返回包裹栏index
	bool CreateItem(int item_id, int item_cnt, bool isBind);
	//Add end.

	//Add by Houjun 2011-07-04, 囚牢相关逻辑
	void AddLimitCycleArea(int owner, const A3DVECTOR& center, float radius);
	void AddPermitCycleArea(int owner, const A3DVECTOR& center, float radius);
	void RemoveLimitCycleArea(int owner);
	void RemovePermitCycleArea(int owner);
	void ClearLimitCycleArea();
	void ClearPermitCycleArea();
	bool CheckMoveArea(const A3DVECTOR& pos);

	bool IsLimitCycleAreaExist(int owner);
	bool IsPermitCycleAreaExist(int owner);
	
	void DumpLimitCycleArea();
	void DumpPermitCycleArea();
	//Add end.

	//Add by Houjun 2011-07-18, 副本相关逻辑
	bool CheckOpenRoomCondition(int raid_id, const TRANSCRIPTION_CONFIG& raid_config);
	bool CheckJoinRoomCondition(int raid_id, const TRANSCRIPTION_CONFIG& raid_config);
	bool CheckJoinRoomConditionCommon(int raid_id, const TRANSCRIPTION_CONFIG& raid_config);

	bool CheckOpenRoomCondition(int raid_id, const TOWER_TRANSCRIPTION_CONFIG& raid_config);
	bool CheckJoinRoomCondition(int raid_id, const TOWER_TRANSCRIPTION_CONFIG& raid_config);
	bool CheckJoinRoomConditionCommon(int raid_id, const TOWER_TRANSCRIPTION_CONFIG& raid_config);
	
	bool OpenRaidRoom(int raid_id, int raid_template_id, char can_vote, char difficulty, const char* roomname, size_t roomname_len);
	bool JoinRaidRoom(int raid_id, int raid_template_id, int room_id, int raid_faction);
	bool PlayerJoinRaidRoom(int raid_id, int raid_template_id, int room_id, int raid_faction);
	int GetRaidFaction() { return _raid_faction; }
	bool CheckEnterRaid(int raid_id, int raid_template_id, int raid_type, const void* buf, size_t size);
	virtual void PlayerLeaveRaid(int reason){}

	void SaveRaidCountersDB(archive & ar);
	void SaveRaidCountersClient(archive & ar);
	void LoadRaidCountersDB(const void * buf, size_t size);
	void IncRaidCounter(int raid_id);
	void ResetRaidCounters();
	int GetRaidCounter(int raid_id);
	void DumpRaidCounter(const char* name);
	void ClearRaidCounter();
	//Add end.

	//拉扯相关
	void FollowTarget(const XID& target, float speed, float stop_dist);
	void StopFollowTarget();
	void SetIdPullingMe(int target, bool on); 
	void SetIdSpiritDragMe(int target, bool on); 
	
	//ZX 5 years anniversary code, delete later, Add by Houjun 20120320
	void SaveFiveAnniDataDB(archive & ar);
	void LoadFiveAnniDataDB(const void* buf, size_t size);
	void FiveAnniCongratuation();
	void FiveAnniDeliveryAward();
	void ResetAnniData();
	bool TestFiveAnniTime();
	//Add end.

	//挖宝相关
	void SetTreasureInfo(archive & ar);
	void GetTreasureInfo(archive & ar);
	void GetClientTreasureInfo(archive & ar);
	void InitTreasureRegion();
	void InitTreasureRegionType();
	void InitTreasureRegionTimer();
	void InitTreasureRegionInfo();
	void UpdateTreasureInfo(int cur_time);
	void ResetTreasureRegion();

	//诛小仙相关
	char GetLittlePetLevel() const { return _little_pet.cur_level;}
	char GetLittlePetAwardLevel() const { return _little_pet.cur_award_level;}
	int GetLittlePetCurExp() const { return _little_pet.cur_exp;}
	void SetLittlePetInfo(archive & ar);
	void GetLittlePetInfo(archive & ar);

	void ActiveRune(); 		//激活元魂装备位
	void ActiveRuneProp(); 		//激活元魂装备位的基本属性
	void DeactiveRuneProp(); 	//停止激活
	void UpdateRuneProp();  	//级别变化的时候更新属性

	// 台历相关
	bool isSalePromotionTaskCompleted(int taskid) { 
		return _sale_promotion_info.taskids.find(taskid) != _sale_promotion_info.taskids.end(); 
	}
	void SaveSalePromotion(archive & ar);      				// 存储 
	void LoadSalePromotion(archive & ar);      				// 读取 

	// 活跃度相关
	void SaveLiveness(archive & ar);      				// 存储 
	void LoadLiveness(archive & ar);      				// 读取 
	void TakeLivenessAward(int grade);      			// 领取活跃度奖励
	// liveness_type  |    param 
	// -----------------------------
	//   task         |   task_id
	//   map          |   map_id
	//   special      |   LIVENESS_SPECIAL_XXX枚举，等于写死
	// @return 是否更新成功
	bool EventUpdateLiveness(int liveness_type, int param);	// 更新活跃度模块
	int GetNextMidNightTime(int cur_time);	    // 检查到下个午夜的UTC时间
	void OnHeartbeatLiveness(int cur_time);		// 心跳驱动检查是否到零点

	// UI传送相关(上古传送)
	void SaveUITransfer(archive & ar);      			// 存储 
	void LoadUITransfer(archive & ar);					// 读取
	bool ActiveUITransferIndex(int index);				// 激活传送点
	bool UseUITransferIndex(int index);					// 使用传送点

	// 鲜花逻辑
	bool FlowerTakeoffByDelivery(int count);      					// Delivery扣除鲜花
	// award_type  0/1:礼包/Buff
	bool FlowerSendAwardByDelivery(int award_type, int award_id);	// Delivery发放领取鲜花奖励

	bool IsRuneActive() { return _active_rune;}
	void AddRuneScore(int score){ _rune_score += score;}
	int GetRuneScore(){return _rune_score;}
	int GetRuneCustomizeInfo()
	{
		int customize_mask = 0;
		if(_flag_mask & RUNE_CUSTOMIZE1) customize_mask |= 0x01;
		if(_flag_mask & RUNE_CUSTOMIZE2) customize_mask |= 0x02;
		if(_flag_mask & RUNE_CUSTOMIZE3) customize_mask |= 0x04;
		if(_flag_mask & RUNE_CUSTOMIZE4) customize_mask |= 0x08;
		return customize_mask;
	}

	void ClrRuneCustomizeInfo()
	{
		_flag_mask &= ~(RUNE_CUSTOMIZE1 | RUNE_CUSTOMIZE2 | RUNE_CUSTOMIZE3 | RUNE_CUSTOMIZE4);
	}
	
	int GetRuneActiveLevel(){return _active_rune_level;}
	void GainRuneExp(int exp);
	void SetRuneRefineValue(int score){ _rune_score = score;}

	//设置人物的一些mask属性
	enum FLAG_MASK
	{
		RUNE_ACTIVE= 0x01, 
		FIX_REBORN_LEVEL = 0x02,	//对6周年之前飞升150玩家进行补偿
		RUNE_CUSTOMIZE1 = 0x04,
		RUNE_CUSTOMIZE2 = 0x08,
		RUNE_CUSTOMIZE3 = 0x10,
		RUNE_CUSTOMIZE4 = 0x20,

	};
	
	void SetFlagMask(unsigned char mask)
	{
		if(mask & RUNE_ACTIVE)
		{
			_active_rune = true;
		}

		_flag_mask = mask;
	}

	void GetFlagMask(unsigned char & mask)
	{
		mask = _flag_mask;
		if(_active_rune)
		{
			mask |= RUNE_ACTIVE;
		}
	}

	//爬塔副本
	void LoadTowerData(const void* buf, size_t size);
	void SaveTowerData(archive& ar);
	void ResetTowerData();
	int  GetTowerLevel() const;
	void SetStartLevelToday(int level);
	void SetEndLevelToday(int level);
	void AddMeetTowerMonster(int tid);
	void SetDailyRewardDelivered(int level);
	bool IsDailyRewardDeilivered(int level);
	void SetLifeLongRewardDelivered(int level);
	bool IsLifeLongRewardDelivered(int level);
	bool DeliveryTowerReward(int raid_template_id, int level, size_t rewardItemIndex, bool longlife);
	bool DeliveryTowerReward(int raid_template_id, int level, bool longlife);

	void TowerLevelPass(int level, int pass_time);

	void SaveTowerDataClient(archive& ar);
	void SaveTowerMonsterListClient(archive& ar);
	virtual void OnResurrect() {}
	void ResetTowerPlayerPet(bool fullMpAndHp = true);
	void FullHPAndMP();
	
	virtual bool IsXuanYuan();

	//人马职业不消耗蓝
	virtual bool IsRenMa()
	{
		return IsRenMaOccupation();
	}
	
	bool EquipmentSlot(size_t item_index, int item_type, size_t stone_index, int stone_id); 
	bool EquipmentInstallAstrology(size_t item_index, int item_id, size_t stone_index, int stone_id, size_t slot_index);
	bool EquipmentUninstallAstrology(size_t item_index, int item_id, size_t slot_index);

	virtual void PlayerFlowBattleLeave(){}

	virtual void PlayerBathInvite(const XID &target){}
	virtual void PlayerBathInviteReply(const XID & target, int param){}
	virtual void OnBathStart(bool is_leader){}
	virtual void OnBathEnd(bool is_leader){}

	virtual void PlayerQilinInvite(const XID & target);
	virtual void PlayerQilinInviteReply(const XID & target, int param);
	virtual void PlayerQilinCancel();
	virtual void PlayerQilinDisconnect();
	virtual void PlayerQilinReconnect();

	virtual void SetLogoutDisconnect() {}

	bool GetResizeInvCostItem(char type, size_t size, size_t new_size, int& cnt_below_96, int& cnt_above_96);
	bool ResizeInv(size_t size, char type, int cnt, const void* buf);
	bool RegisterFlowBattle();
	void SetAntiNPCInvisible(bool value);

	void SetPhase(int id, bool on);
	void SavePhaseInfo(archive & ar);
	void LoadPhaseInfo(archive & ar);
	void SavePhaseInfoForClient(archive & ar);

	void DeliverFBaseCMD(const void * buf, size_t size);//将客户端发来的cmd命令转发给基地gs处理
	bool EnterBase();
public:
//脚本操作函数
	int script_RefinePetEquip(int item_type, int stone_type);
	void script_ExtGainOfProduce(int recipe_id, int player_level, bool is_female, int player_class);
	void script_DiscoverGain(int player_level, bool is_female, int player_class, int discover_type);

public:
//玩家操作函数群
//这些函数都是player的操作，所以参数可能是错误的
	int  PlayerGetItemInfo(int where, int index);
	int  PlayerGetItemInfoList(int where, size_t count , unsigned char *list);
	void PlayerGetItemExchangeList(unsigned char where, const std::vector<exchange_item_info>& vec_exchange_info);
	void PlayerGetInventory(int where);
	void PlayerGetInventoryDetail(int where);
	void PlayerGetTrashBoxInfo(bool detail);
	void PlayerExchangeInvItem(size_t idx1,size_t idx2);
	void PlayerMoveInvItem(size_t src,size_t dest,size_t count);
	void PlayerExchangeTrashItem(size_t idx1,size_t idx2);
	void PlayerMoveTrashItem(size_t src,size_t dest,size_t count);
	void PlayerTrashItemToInv(size_t idx_tra, size_t idx_inv, size_t count);
	void PlayerInvItemToTrash(size_t idx_inv, size_t idx_tra, size_t count);
	void PlayerExchangeTrashInv(size_t idx_tra, size_t idx_inv);
	void PlayerExchangeTrashMoney(size_t inv_money, size_t tra_money);
	void PlayerDropInvItem(size_t index,size_t count,bool isProtected);
	void PlayerDropEquipItem(size_t index,bool isProtected);
	void PlayerExchangeEquipItem(size_t index1,size_t index2);
	void PlayerEquipItem(size_t index_inv, size_t index_equip);
	void PlayerMoveEquipItem(size_t index_inv, size_t index_equip);
	void PlayerExchangePocketItem(size_t index1, size_t index2);
	void PlayerMovePocketItem(size_t src, size_t dest, size_t count);
	void PlayerExchangeInvPocket(size_t idx_inv, size_t idx_poc);
	void PlayerMovePocketItemToInv(size_t idx_poc, size_t idx_inv, size_t count);
	void PlayerMoveInvItemToPocket(size_t idx_inv, size_t idx_poc, size_t count);
	void PlayerMoveAllPocketItemToInv(); 
	void PlayerMoveMaxPocketItemToInv(); 
	void PlayerExchangeFashionItem(size_t index1, size_t index2);
	void PlayerExchangeMountWingItem(size_t index1, size_t index2);
	void PlayerExchangeInvFashion(size_t idx_inv, size_t idx_fas);
	void PlayerExchangeInvMountWing(size_t idx_inv, size_t idx_mw);
	void PlayerExchangeEquipFashion(size_t idx_equ, size_t idx_fas);
	void PlayerExchangeEquipMountWing(size_t index_equ, size_t index_mw);
	void PlayerGoto(const A3DVECTOR &pos);
	void PlayerDropMoney(size_t amount,bool isProtected);
	void PlayerGetProperty();
	void PlayerLearnSkill(int skill_id);
	bool PlayerSitDown();
	bool PlayerStandUp();
	void PlayerRestartSession();
	void PlayerAssistSelect(const XID & cur_target);
	virtual bool PlayerUseItem(int where, size_t inv_index, int item_type,size_t count);
	virtual bool PlayerUseItemWithArg(int where,size_t inv_index,int item_type,size_t count, const char * buf, size_t buf_size);
	bool PlayerSitDownUseItem(int where, size_t inv_index, int item_type,size_t count);
	bool PlayerUseItemWithTarget(int where,size_t inv_index,int item_type,char force_attack);
	bool PlayerOpenPersonalMarket(int index, int item_id, size_t count, const char name[PLAYER_MAERKET_NAME_LEN], int * entry_list);
	bool PlayerTestPersonalMarket(int index, int item_id);
	bool PlayerDestroyItem(int where, size_t index, int item_type);
	void PlayerRegionTransport(int rindex, int tag);
	void PlayerLogout(int type, int offline_agent);
	void PlayerDuelRequest(const XID & target);
	void PlayerDuelReply(const XID & target,int param);
	void PlayerBindRequest(const XID & target);
	void PlayerBindInvite(const XID & target);
	void PlayerBindRequestReply(const XID & target, int param);
	void PlayerBindInviteReply(const XID & target, int param);
	void PlayerBindCancel();
	bool PlayerProduceItem(int recipe_id);
	bool PlayerGetMallSaleTimeItem();
	bool PlayerDoShopping(int id, size_t order_index, size_t slot, size_t count);
	bool PlayerDoBonusShopping(int id, size_t order_index, size_t slot, size_t count);
	bool PlayerDoZoneShopping(int id, size_t order_index, size_t slot, size_t count);
	bool PlayerDoBillingShoppingStep1(int id, size_t order_index, size_t slot, size_t order_count);
	bool PlayerDoBillingShoppingStep2(int id, size_t goods_count, int expire_time, size_t total_cash, size_t order_count);
	bool PlayerObtainVipAward(int curtime, int award_id, int award_item_id);
	bool PlayerTryGetOnlineAward(int id, time_t t, int index, int small_giftbag_id, int & retcode);
	bool PlayerObtainOnlineAward(int awardid, time_t curtime, int awardindex, int small_giftbag_id);
	bool PlayerResetSkillProp(int opcode);
	bool PlayerLotteryCashing(size_t item_index);
	void PlayerTalismanBotBegin(bool active, bool need_question );
	void PlayerTalismanBotContinue();
	void PlayerRideInvite(const XID &target);
	void PlayerRideInviteReply(const XID & target, int param);
	void PlayerRideCancel();
	void PlayerRideKick(char pos);
	void PlayerMobActive(const C2S::CMD::mobactive_start &ma);
	void PlayerMobActiveCancel();
	void PlayerMobActiveFinish();
	void PlayerPostMessage(char message_type, unsigned char occupation, char gender, int level, int faction_level, unsigned message_len, char message[]);
	void PlayerApplyMessage(char message_type, int message_id, unsigned int message_len, char message[]);
	void PlayerVoteMessage(char vote_type, char message_type, int message_id);  
	void PlayerResponseMessage(char message_type, int message_id, short dst_index, unsigned int message_len, char message[]); 
	void PlayerHandleMessageResult(char message_type, char op_type, int charm);	
	bool CheckMessageCondition(char message_type, char op_type);
	void PlayerEquipPetBedge(size_t inv_index,size_t pet_index);
	void PlayerMovePetBedge(size_t src_index,size_t dst_index);
	void PlayerEquipPetEquip(size_t inv_index,size_t pet_index);
	int PlayerSummonPet(size_t pet_index);
	int PlayerRecallPet(size_t pet_index);
	int PlayerCombinePet(size_t pet_index,int type);
	int PlayerUncombinePet(size_t pet_index);
	int PlayerSetPetStatus(size_t pet_index,int pet_tid,int main_status,int sub_status);
	int PlayerSetPetRank(size_t pet_index,int pet_tid,int rank);
	void PlayerSendPetCommand(size_t pet_index,int cur_target,int cmd,const void* buf,size_t size);
	int PlayerFeedPet(size_t pet_index,item *parent,int pet_level_min,int pet_level_max,
		unsigned int pet_type_mask,int food_usage,unsigned int food_type,int food_value,int& cur_value);
	int PlayerAutoFeedPet(size_t pet_index,item *parent,float hp_gen,float mp_gen,float hunger_gen,int mask,int& cur_value);
	void PlayerStartFly();
	void PlayerStopFly();
	bool PlayerChangeStyle(unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid, size_t item_index);
	bool PlayerEnableVIPState(int item_id); 
	void PlayerDisableVIPState();
	bool PlayerStartTransform();
	void PlayerStopTransform();
	void PlayerCastItemSkill(int skill_id, char skill_type, int item_id, short item_index, char force_attack, 
			unsigned char target_count, int *target); 
	void PlayerCastTransformSkill(int skill_id, char skill_type, char force_attack, unsigned char target_count, int *targets, const A3DVECTOR& pos);
	bool PlayerEnterCarrier(int carrier_id, const A3DVECTOR & rpos, unsigned char rdir);
	bool PlayerLeaveCarrier(int carrier_id, const A3DVECTOR & pos, unsigned char dir);

	bool PlayerEnterTerritory(int mafia_id, int territory_id, unsigned char is_assist);
	bool PlayerGetTerritoryAward(int mafia_id, int reward_type, int item_id, int item_count, int money);

	void PlayerUpdateCombineSkill(int skill_id, int element_count, int element_id[]);

	bool PlayerUniqueBidRequest(int money_upperbound, int money_lowerbound);
	void PlayerUniqueBidResult(int result, int money_upperbound, int money_lowerbound);
	bool PlayerUniqueBidGet();
	void PlayerUniqueBidReward(int result, int itemid);

	bool CheckPlayerMobActiveRequest();
	int  GetMobActiveTid();
	
	bool CheckPlayerActiveEmoteInvite();
	void PlayerActiveEmoteInvite(const XID & target, int type);
	void PlayerActiveEmoteInviteReply(const XID & target, int type,int param);
	void PlayerActiveEmoteCancel();
	bool SetActiveEmoteState(const XID & target, int active_emote_type, const char type);
	void ClrActiveEmoteState();

	//古卷挖宝相关操作
	bool PlayerUpgradeTreasureRegion(int item_id, int item_index, int region_id, int region_index);
	bool PlayerUnlockTreasureRegion(int item_id, int item_index, int region_id, int region_index);
	bool PlayerDigTreasureRegion(int region_id, int region_index);
	void PlayerIncTreasureDigCount(int count);
	void DebugSetTreasureRegionLevel(int region_level);

	//诛小仙相关
	void PlayerAdoptLittlePet();
	void PlayerFeedLittlePet(int item_id, int item_count);
	void PlayerGetLittlePetAward(); 
	
	//元魂相关操作
	void PlayerIdentifyRune(int rune_index, int assist_index);		//鉴定
	void PlayerCombineRune(int rune_index, int assist_index); 		//合成
	void PlayerRefineRune(int rune_index, int assist_index);		//洗炼
	void PlayerResetRune(int rune_index, int assist_index);			//归元
	void PlayerDecomposeRune(int rune_index);				//拆解
	bool PlayerLevelupRune(int rune_index);					//升级

	void PlayerOpenRuneSlot(int rune_index);				//开孔
	void PlayerChangeRuneSlot(int rune_index, int assist_index);		//改写孔位
	void PlayerEraseRuneSlot(int rune_index, int slot_index);		//擦孔
	void PlayerInstallRuneSlot(int rune_index, int slot_index, int stone_index, int stone_id);	//安装符文
	bool PlayerRefineRuneAction(int rune_index, bool accept_result);
	
	bool IdentifyRune(int rune_index, int assist_index);		//鉴定
	bool CombineRune(int rune_index, int assist_index); 		//合成
	bool RefineRune(int rune_index, int assist_index);		//洗炼
	bool ResetRune(int rune_index, int assist_index);			//归元
	bool DecomposeRune(int rune_index);				//拆解
	bool OpenRuneSlot(int rune_index);				//开孔
	bool ChangeRuneSlot(int rune_index, int assist_index);		//改写孔位
	bool EraseRuneSlot(int rune_index, int slot_index);		//擦孔
	bool InstallRuneSlot(int rune_index, int slot_index, int stone_index, int stone_id);	//安装符文

	void PlayerGetTaskAward(int type);
	
	bool PlayerPk1stBet(size_t area, int cnt, const void* buf);
	bool PlayerPkTop3Bet(size_t area, int cnt, const void* buf, bool cancel = false);
	bool DeliveryPk1stBetReward();
	bool DeliveryPk1stBetResultReward();
	bool DeliveryPkTop3BetReward();
	bool DeliveryPkTop3BetResultReward(char type);
	
	void SavePlayerBetData(archive& ar);
	void LoadPlayerBetData(const void* buf, size_t size);
	void SavePlayerBetDataClient(archive& ar);

	void PlayerStartSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp);
	void PlayerSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, unsigned short stamp, char collision_state, int timestamp);
	void PlayerStopSpecialMove(unsigned char dir, A3DVECTOR & cur_pos, unsigned short stamp);

	void PlayerChangeName(char * name, size_t name_len);
	void PlayerChangeFactionName(int roleid, int fid, char type, char * name, size_t name_len);
	void ChangeItemOwnerName(item_list & inv);
	void ChangeItemOwnerID(item_list & inv);

	void PlayerApplyCollisionRaid(int raid_id, int raid_template_id, bool is_team, char is_cross);

	bool PlayerChangeWingColor(unsigned char color);
	void PlayerClearWingColor();
	
	void PlayerIdentifyAstrology(size_t item_indx, int item_id);
	void PlayerUpgradeAstrology(size_t item_index, int item_type, size_t stone_index, int stone_id); 
	void PlayerDestroyAstrology(size_t item_indx, int item_id);
	
	bool IdentifyAstrology(size_t item_indx, int item_id);
	bool UpgradeAstrology(size_t item_index, int item_type, size_t stone_index, int stone_id); 
	bool DestroyAstrology(size_t item_indx, int item_id);

	void TalismanRefineSkillResult(int item_index, int item_id, char result);

	bool PlayerGetCollisionAward(bool daily_award, int award_index);
	void PlayerGetCouponsInfo(int userid, int toaid, int tozoneid, int roleid, std::vector<int> & task_ids);
	void PlayerRemedyMetempsychosisLevel(int item_index);

	void DeliveryGiftBox(size_t item_index, int item_id);
	void UseCashLotteryTY(int item_id, int item_cnt);
	void OnUseCash(int offset);
	void PlayerUseCash(int item_id, int item_count, int expire_date, int need_cash, int order_id);
	void DeliverCashGiftPoint(int offset);
	void TryObtainCashGift();
	bool GetCashGiftAward();

	void PlayerBuyKingdomItem(char type, int index);
	bool PlayerGetTouchAward(char type, int item_count);
	bool PlayerExchangeTouchPoint(int retcode, unsigned int flag, unsigned int count);

	virtual void KingTryCallGuard();
	bool GuardCallByKing(int roleid, int lineid, int mapid, float pos_x, float pos_y, float pos_z);
	void KingCallGuardInvite(char * kingname, size_t kingname_len, int line_id,  int map_id, A3DVECTOR & pos);
	bool KingCallGuardReply(int line_id, int map_id, float pos_x, float pos_y, float pos_z);

	bool PlayerEnterBath();
	bool PlayerDeliverKingdomTask(int task_type);
	bool PlayerReceiveKingdomTask(int task_type);
	bool KingDeliverTaskResponse(int retcode, int task_type);
	bool PlayerFlyBathPos();
	bool PlayerGetKingReward();
	bool KingGetRewardResponse(int retcode);

	bool PlayerComposeFuwen(bool consume_extra_item, int extra_item_index);
	bool ComposeFuwen(bool consume_extra_item, int extra_item_index);
	void SendFuwenComposeInfo();
	void ResetFuwenComposeInfo();
	bool PlayerUpgradeFuwen(int main_fuwen_index, int main_fuwen_where, int assist_count, int assist_fuwen_index[]); 
	bool PlayerInstallFuwen(int src_index, int dst_index);
	bool PlayerUninstallFuwen(int fuwen_index, int assist_index, int inv_index);
	void LoadFuwenComposeInfo(archive & ar);
	void SaveFuwenComposeInfo(archive & ar);
	void PlayerCreateCrossVrTeam(int name_len, char team_name[]); 

	void  PlayerTryGetCS6V6Award(int type, int level);
	bool PlayerExchangeCS6V6Award(int index, int item_id, int currency_id); 
	bool PlayerExchangeCS6V6Money(int type, int count);
	bool PlayerGetCS6V6Award(int type, int level, int score, int team_score, int team_pos, int award_timestamp); 

	void PlayerChangePuppetForm();

	void PlayerChargeTo(int type, int target_id, const A3DVECTOR & pos);
	void StartTravelAround(int travel_vehicle_id, float travel_speed, int travel_path_id, int travel_max_time);
	void PlayerStopTravelAround();
private:
	bool PlayerDoShopping(size_t item_count,const short * order);	//此函数已经作废

public:
//断线操作函数
	void LostConnection(int offline_type = PLAYER_OFF_OFFLINE);
	void KickOut() { LostConnection(PLAYER_OFF_KICKOUT); }
	void SendLogoutRequest(int type);
public:
	void PickupMoneyInTrade(size_t money);
	void PickupItemInTrade(const A3DVECTOR & pos,const void * data, size_t size,bool isTeam);
	bool TradeObjectHeartbeat();
	void ReplyTradeRequest(bool v);
	void DiscardTrade();
	void FactionTradeTerminateSave();
	bool DisconnectHeartbeat();
	void PlayerContestResult(int score,int place,int right_amount);

	bool ConsignObjectHeartbeat();

	void DebugChangeEquipOwnerID();
	void TryReconnect();
	
protected:
//私有的函数逻辑
	static int DoPlayerMarketTrade(const XID & trader,const XID & customer ,gplayer * pTrade, gplayer *pBuyer, const void * order, size_t length);		//向商店购买
	static int DoPlayerMarketPurchase(const XID & trader,const XID & customer,gplayer * pTrade, gplayer *pBuyer, const void * order, size_t length);		//向商店卖出
	void DoPlayerMarketTrade(player_stall::trade_request & req, gplayer_imp * pImp, size_t need_money);
	bool CheckMarketTradeRequest(player_stall::trade_request & req, size_t &need_money);
	bool CheckMarketTradeRequire(player_stall::trade_request & req, size_t need_money);
	bool CancelPersonalMarket();

	bool CheckMarketPurchaseRequest(player_stall::trade_request & req,size_t & total_price);
	bool CheckMarketPurchaseRequire(player_stall::trade_request & req,size_t total_price);
	void DoPlayerMarketPurchase(player_stall::trade_request & req, gplayer_imp * pImp, size_t total_price);

	void RefreshPropInventory();
	void RefreshEquipment();		//重新计算装备数据
	bool EquipItem(size_t index_inv,size_t index_equip);	//装备物品
	bool ExchangeFashionItem(size_t index_fas,size_t index_equip);	//装备时装
	void Logout(int type, int offline_agent = 0);			//登出操作
	void DummyLogout(int type);			//登出操作
	void RefreshFuwen();		//重新计算符文数据

	void QueryOtherPlayerInfo1(size_t count, int id[]);
	void QueryNPCInfo1(size_t count, int id[]);

	void PutItemAfterTrade(item_list & backpack);
	void DropAllAfterTrade(item_list & backpack,size_t money);
	void AutoSaveData();
	void AddExp(int64_t& exp, int64_t ext_exp); 		//增加经验，考虑多倍经验加成		
	void IncExp(int64_t &exp);				//直接加经验, 不考虑任何多倍经验加成
	void ReceiveExp(int64_t exp);				//直接加经验，不进行级别惩罚，但进行经验加成
	void ReceiveExp(const msg_exp_t & entry);		//会进行经验惩罚
	void ReceiveGroupExp(const A3DVECTOR &pos,int64_t total_damage,int64_t exp,int level,size_t count,int npcid, int npctag, float r, const msg_grpexp_t * list, bool no_exp_punish);
	void LevelUp();
	float GetExtraExpRatio();
	void IncDeityExp(int& exp);				//直接加封神经验，不进行任何修正 其他函数都会到这里
	void DeityLevelUp();
	void SendServiceRequest(int service_type, const void * buf, size_t length);
	void QueryServiceContent(int service_type);
	void ThrowInvItem(size_t index, size_t count, bool isProtected, unsigned char drop_type);
	void ThrowEquipItem(size_t index, bool isProtected,unsigned char drop_type);
	void ThrowInvItem(size_t index, size_t count, unsigned char drop_type,XID owner, int team, int seq);
	void ThrowEquipItem(size_t index, unsigned char drop_type,XID owner, int team, int seq);
	void ThrowMoney(size_t amount, bool isProtected);
	void ReduceSaveTimer(int counter) { _write_timer -= counter;}
	int GetItemSkillLevel(int item_id, short item_idx, int skill_id);
	bool IsConsumableItemSkill(int item_id, short item_idx);
	
	template <int foo>
	inline void DoWeaponOperation()
	{
	/*	 没有箭枝 也没有耐久度了
		bool is_update = false;
		_equipment[item::EQUIP_INDEX_WEAPON].AfterAttack(_equipment,&is_update);
		if(is_update)
		{
			RefreshEquipment();
		}
		*/
	}

	//回收战场物品
	void RecycleBattleItem();
	
	void DumpPocketInventory();	

	bool CatchPet(int monster_id, int monster_level, int monster_raceinfo, int monster_catchdifficulty, float monster_hp_ratio, int &pet_id);
	bool CatchPetSuccess(int pet_tid);
	
	void EnterCarrier(int carrier_id,const A3DVECTOR & rpos,unsigned char rdir,const A3DVECTOR & carrier_pos,unsigned char carrier_dir);
	void LeaveCarrier(int carrier_id,const A3DVECTOR & pos,unsigned char dir);
	void LeaveCarrier(int carrier_id);
	
	bool ChangeTransform(int template_id, int level, int exp_level, int timeout, char type);
	bool Transform(int template_id, int level, int exp_level, int timeout, char type);

	int GetZoneCash();
	void UseZoneCash(int count);

	bool IsSameDay(int timestamp1, int timestamp2);
	bool IsSameWeek(int timestamp1, int timestamp2);
	bool IsSameSeason(int timestamp1, int timestamp2);
	void GetGsRoleInfoForConsign(GMSV::role_basic_attribute & basic_attr, GMSV::role_deity_info & deity_info, raw_wrapper & title_info, raw_wrapper & skill_info, std::vector<GMSV::role_pet_prop_added> & pet_prop_added_list);
	void GetGsRoleInfoForPlatform(GMSV::role_basic_attribute & basic_attr, GMSV::role_deity_info & deity_info, raw_wrapper & title_info, raw_wrapper & skill_info, std::vector<GMSV::role_pet_prop_added> & pet_prop_added_list, GMSV::role_basic_attribute_ext & ext);

};

void TrySwapPlayerData(world * pPlane,const int cid[3],gplayer * pPlayer);

#endif

