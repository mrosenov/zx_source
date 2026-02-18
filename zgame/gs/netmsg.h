/*
 * 	处理连接服务器来的命令，包含客户端和delivery、link发来的协议
 */
#ifndef __ONLINEGAME_GS_NETMSG_H__
#define __ONLINEGAME_GS_NETMSG_H__

#include <octets.h>
#include <common/packetwrapper.h>
#include <common/protocol_imp.h>
#include <sys/uio.h>
#include <hashmap.h>

#include <gsp_if.h>
#include "slice.h"

void handle_user_cmd(int cs_index,int sid,int user_id,const void * buf, size_t size);
void handle_user_chat(int cs_index,int sid, int uid, const void * msg, size_t size, const void * aux_data, size_t size2, char channel, int item_idx);
size_t handle_chatdata(int uid, const void * aux_data, size_t size, void * buffer, size_t len);
void trade_end(int trade_id, int role1,int role2,bool need_read1,bool need_read2);
void trade_start(int trade_id, int role1,int role2, int localid1,int localid2);
void psvr_ongame_notify(int * start , size_t size,size_t step);
void psvr_offline_notify(int * start , size_t size,size_t step);
void get_task_data_reply(int taskid, int uid, const void * env_data, size_t env_size, const void * task_data, size_t task_size);
void get_task_data_timeout(int taskid, int uid, const void * env_data, size_t env_size);

void user_kickout(int cs_index,int sid,int uid);
void user_lost_connection(int cs_index,int sid,int uid);
void user_offline_consignrole(int cs_index, int sid, int uid);
void user_reconnect(int cs_index,int sid,int roleid);

void faction_trade_lock(int trade_id,int roleid,int localsid);
void gm_shutdown_server();
void player_cosmetic_result(int user_id, int ticket_id, int result, unsigned int crc);
void battleground_start( int battle_world_tag, int battle_id, int end_time, int apply_count, bool is_instnace_field , char battle_type);
void battleground_start_response( bool is_instance_field, int retcode, int battle_world_tag, int battle_id, char battle_type); 
bool player_enter_battleground( int role_id, int battle_world_tag, int battle_id, int battle_faction, int battle_type );
void OnDeliveryConnected();
void OnDeliveryDisconnect();
void OnDeliveryDisconnect2();
void handle_toplist();
void faction_battle_start(int attacker, int defender, int end_time);
void player_cash_notify(int role, int cash_plus_used);
void player_get_cash_total(int roleid);
void player_on_divorce(int id1, int id2);
void player_change_gs( int localsid, int roleid, int target_gs , int tag, float x, float y, float z, int reason);


void record_leave_server(int uid);	//记录玩家离开服务器的时间，用于有几秒钟忽略玩家数据的时间
int  total_login_player_number();


void sync_faction_hostiles_info( unsigned int faction_id, std::vector<unsigned int>& hostiles );
void add_faction_hostile( unsigned int faction_id, unsigned int hostile );
void del_faction_hostile( unsigned int faction_id, unsigned int hostile );

void battlefield_start(int battle_id,int world_tag,int attacker,int denfeder,int attacker_assistant,int end_time,bool force);

void sect_add_member(int master, int disciple);
void sect_del_member(int master, int disciple);

void player_contest_result(int role_id,int score,int place,int right_amount);

void handle_message_result(int roleid, char message_type, char op_type, int charm);

bool player_billing_approved(int userid, int itemid, int itemnum, int expire_time, int cost);
void player_billing_cash_notify(int role, int total_cash);

void ref_add_bonus(int roleid, int inc);
void ref_add_exp(int roleid, int64_t inc);
void player_change_style(int roleid, unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid);
void handle_netbar_reward(int role_id, int netbar_level, int award_type);

void change_shop_sale(unsigned int new_scheme);
void change_shop_discount(unsigned int new_discount);

void territory_start(int territory_id, int map_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp);
void territory_start_response(int retcode, int territory_id);
void sync_territory_owner(int territoryid, int territory_type, unsigned int owner);
void territory_item_get(int retcode, int roleid, int itemid, int itemcount, int money);

void unique_bid_result(int retcode, int roleid, int money_upperbound, int money_lowerbound);

void update_circle_info(int roleid, unsigned int circle_id, unsigned char circle_mask);

void recv_circle_exp(int roleid, unsigned int bonus_exp);
void recv_friend_num(int roleid, int taskid, int number);

void recv_circle_async_data(int uid, int result, const void * buf, size_t size);

void player_change_ds(int roleid, char flag);

void consign_end(int roleid, char need_read);
void consign_end_role(int roleid, int retcode);

//GT消息接口, add by Sunjunbo 2012-12-25
void OnRoleEnterVoiceChannel(long roleid); 
void OnRoleLeaveVoiceChannel(long roleid);

//鲜花接口, add by Sunjunbo 2013-03-04
// 发放奖励
bool OnFlowerSendAward(int roleid, int award_type, int award_id);
// 扣除鲜花
bool OnFlowerTakeoff(int roleid, int count);
// 激活控制器 
void OnFlowerActiveCtrl(int ctrl_id);

//副本Delivery发送的接口, add by Houjun 2011-7-14
void raid_start(int map_id, int room_id, const std::vector<int>& roleid_list, int raid_type, const void* buf, size_t size, char can_vote);
void raid_close(int room_id, int map_id, int index, int reason);
void raid_enter(int role_id, int map_id, int raid_template_id, int index, int raid_type, const void* buf, size_t size);
void raid_member_change(int role_id, int gs_id, int map_id, int index, unsigned char operation);
void raid_kickout(int role_id, int index);
//Add end
void facbase_start(int fid);


void change_name_response(int roleid, char * name, size_t name_len);
void battle_flag_buff_scope(const std::vector<int>& role_list, int flagid, int worldtag);

void kingdom_start(char fieldtype, int tag_id, int defender, std::vector<int> & attacker_list, void * defender_name, size_t name_len);
void kingdom_start_response(int retcode, char fieldtype, int tag_id, int defender, std::vector<int> &  attacker_list, void * defender_name, size_t name_len);
void kingdom_battle_stop(char fieldtype, int tag_id);
void kingdom_point_sync(int point);
void kingdom_info_sync(int mafia_id, int point, std::map<int, int> & kingdom_title);
void kingdom_title_sync(int roleid, char title);
bool guard_call_by_king(int roleid, int lineid, int map_id, float pos_x, float pos_y, float pos_z);
bool king_guard_invite_reply(int roleid, int line_id, int map_id, float pos_x, float pos_y, float pos_z);
void queen_open_bath();
void queen_close_bath();

bool touch_point_exchange(int retcode, int roleid, unsigned int flag, unsigned int count);

void flow_battle_clear_leave(int world_tag, int role_id, char faction);
void close_flow_battle(int gs_id, int map_id);
void forbid_sync(int type, std::vector<int> & ids);
void get_cash_avail(int roleid, int & cash_avail);
void add_web_order(int roleid, std::vector<GMSV::weborder> & list);
void king_get_reward_response(int retcode, int roleid);

namespace GNET
{
enum CHGS_ERR
{
	ERR_CHGS_SUCCESS        = 0,
	ERR_CHGS_INVALIDGS      = 1,            //不存在该gs号
	ERR_CHGS_MAXUSER        = 2,            //目的gs人数达到上限?
	ERR_CHGS_NOTINSERVER    = 3,            //用户切换gs时不在服务器内
	ERR_CHGS_STATUSINVALID  = 4,            //用户切换gs时状态不对
	ERR_CHGS_NOTGM          = 5,            //用户不是gm
	ERR_CHGS_MAPIDINVALID   = 6,            //地图不存在
	ERR_CHGS_SCALEINVALID   = 7,            //非法坐标
	ERR_CHGS_DB_ERROR       = 8,
};

}
#endif

