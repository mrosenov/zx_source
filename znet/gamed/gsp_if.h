#ifndef __GS_PROTO_IF_H__
#define __GS_PROTO_IF_H__

#include <string>
#include <vector>
#include <map>
#include <set>

#include "privilege.hxx"

void user_login(int cs_index,int sid,int uid,const void * auth_buf, size_t auth_size);
void report_cheater(int roleid, int cheattype, const void *cheatinfo, size_t size);
void acquestion_ret(int roleid, int ret); // ret: 0 正确, 1 错误, 2 超时

namespace GDB
{
	struct base_info;
	struct vecdata;
}
namespace GNET
{
class Protocol;
class GTopTable;
class FactionLevel;
class GFamilySkill;
class GConsignGsRoleInfo;
}

namespace GMSV
{
	enum
	{
		PLAYER_LOGOUT_FULL  =  0,
		PLAYER_LOGOUT_HALF  =  1,
	};
	enum
	{
		CHAT_CHANNEL_LOCAL = 0,
		CHAT_CHANNEL_FARCRY ,
		CHAT_CHANNEL_TEAM ,
		CHAT_CHANNEL_FACTION ,
		CHAT_CHANNEL_WISPER ,
		CHAT_CHANNEL_DAMAGE,
		CHAT_CHANNEL_FIGHT,
		CHAT_CHANNEL_TRADE ,
		CHAT_CHANNEL_SYSTEM,
		CHAT_CHANNEL_BROADCAST,
		CHAT_CHANNEL_MISC,
		CHAT_CHANNEL_FAMILY,
		CHAT_CHANNEL_CIRCLE,	
		CHAT_CHANNEL_ZONE,
		CHAT_CHANNEL_SPEAKER,		//大喇叭
		CHAT_CHANNEL_RUMOR,		//谣言
	};

	struct city_entry
	{
		int level;
		int owner;
	};

	//目前用于角色寄售
	struct role_basic_attribute 
	{
		int resistance1;
		int resistance2;
		int resistance3;
		int resistance4;
		int resistance5;
		int level;
		int cls;
		int level_before_reborn;
		int cls_before_reborn;
		int talisman_value;
		int reputation;
		int pk_value;
		char cultivation;
		int64_t exp;
		int max_hp;
		int max_mp;
		int damage_low;
		int damage_high;
		int defense;
		int attack;
		int armor;
		float crit_damage;
		float crit_rate;
		float skill_armor_rate;
		float skill_attack_rate;
		float anti_crit;
		float anti_crit_damage;
		float cult_defense_xian;
		float cult_defense_mo;
		float cult_defense_fo;
		int battlescore;
		char reborn_count;
		char gender;
	};

	struct role_basic_attribute_ext
	{
		int renxing1;
		int renxing2;
		int renxing3;
		int renxing4;
		int renxing5;
		int jingtong1;
		int jingtong2;
		int jingtong3;
		int jingtong4;
		int jingtong5;
	};
	//目前用于角色寄售
	struct role_deity_info 
	{
		float atatck_xian;
		float atatck_fo;
		float atatck_mo;
		int deity_level;
		int deity_power;
		int max_dp;
		int64_t deity_exp;
	};

	//目前用于角色寄售
	struct role_title_info
	{
		int datasize;
		void * pdata;
	};

	//目前用于角色寄售
	struct role_pet_prop_added
	{
		unsigned char pet_index;
		unsigned short maxhp;
		unsigned short maxmp;
		unsigned short defence;
		unsigned short attack;
		unsigned short resistance[6];
		unsigned short hit;
		unsigned short jouk;
		float crit_rate;
		float crit_damage;
		int equip_mask;
	};

	//目前用于角色寄售
	struct role_skill_info
	{
		int datasize;
		void * pdata;
	};

	struct raid_apply_info
	{
		int roleid;
		int score;	//积分
		int occupation;
	};

	struct gt_team_info 
	{
		int64_t team_id;
		int captain;
		std::vector<int> members;
	};

	struct webmallgoods
	{
		int id;
		int count;
		int flagmask;
		int timelimit;
	};

	struct webmallfunc //礼包结构
	{
		int id;
		std::string name;//礼包名称
		int count;
		int price;

		std::vector<webmallgoods> goods;
	};

	struct weborder //订单结构
	{
		int64_t orderid;
		int userid;
		int roleid;
		int paytype;
		int status;
		int timestamp;
		
		std::vector<webmallfunc> funcs;
	};


	void SetCityOwner(int id, int level,int owner);
	int GetCityOwner(unsigned int id);
	int GetCityLevel(unsigned int id);
	int GetFactionBonus(int faction);
	void RefreshCityBonus();
	
//	typedef std::vector< > player_list;
	typedef std::pair<int /*userid*/,int /*sid*/>  puser;

	void StartPollIO();	//开始处理PollIO，这个函数会阻塞住，一直执行
	bool InitGSP(const char* conf,int gid, const char * servername, int server_attr, const char * version, int pksetting);
	bool SendClientData(int lid, int userid,int sid,const void * buf, size_t size);
	bool MultiSendClientData(int lid, const puser * first, const puser * last,const void * buf, size_t size,int except_id);
	bool MultiSendClientData2(int lid, const puser * first, const puser * last,const void * buf, size_t size,int except_id);
	bool MultiSendClientData(int lid, const puser * first, const puser * last,const void * buf, size_t size);
	bool SendLoginRe(int lid, int userid, int sid,int result, char flag, int tag = -1, float x = 0, float y =0, float z =0,char level = -1, char oc=-1, int fid=0,int ft=0, int con = 0,int family = 0, int devotion=0, int jointime=0, size_t battle_score = 0, int circle_id = 0, int client_tag = 0);
	bool SendKickoutRe(int userid, int sid , int result, int src_zoneid);
	bool SendReconnectRe(int userid, int sid , int result);
	bool SendDisconnect(int lid, int userid,int sid, int result);
	bool SendLogout(int lid, int userid, int sid,int type=PLAYER_LOGOUT_FULL);
	bool SendPlayerConsignOfflineRe(int lid, int roleid, int sid, int result, const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list);
	bool SendOfflineRe(int lid,int userid,int sid, int result);
	bool SendPlayerConsignFailedOfflineRe(int lid, int roleid, int sid, int result);
	void DiscardTrade(int trade_id, int userid);
	void ReplyTradeRequest(int trade_id, int userid,int localsid,bool isSuccess);
	void SyncRoleInfo(const GDB::base_info* pInfo, const GDB::vecdata* data);
	void SendUpdateEnemy(int userid, int enemy);
	void SendPlayerFaction(int userid, int fid, int mid, int level, int con, int fcon, unsigned battle_score, int reborn_cnt, int circle_id);
	void SendMafiaDuelDeath(int mafia_id ,int dead_man, int attacker,int battle_id = -1);
	void SendBattlefieldDeath(int mafia_id,int dead_man,int attacker,int battle_id);
	void SendConsignRequest(int userid, char type, int item_id, int item_idx, int item_cnt, int item_type, int money, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len);
	void SendConsignStartRole(int roleid, char type, int item_id, int item_idx, int item_cnt, int item_type, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len);

	void FetchWeeklyTop();
	void FetchDailyTop();
	void OnLoadWeeklyTop(GNET::GTopTable& table);
	void OnLoadDailyTop(GNET::GTopTable& table);
	void TopList2Title(int fd);
	int GetRankLevel(int index);
	void NotifyTableChange(int tableid);
	void SetCouple(int id1, int id2, int op); //op = 1 marry op = 0 unmarry
	int  GetFactionLevel(int faction);
	void OnUpdateFactionLevel(const std::vector<GNET::FactionLevel>& list);

	struct chat_msg
	{
		int speaker;
		const void * msg;
		size_t size;
		const void * data;
		size_t dsize;
		char channel;
		char emote_id;
		int level;
		int mafia_id;
		int sect_id;
	};
	bool MultiChatMsg(int lid,const puser * first, const puser * last, const chat_msg & chat);
	bool SendChatMsg(int lid, int userid, int sid,const chat_msg & chat);
	bool BroadChatMsg(const chat_msg &chat);
	bool BroadcastLineMessage(int gs_id, void * buf, size_t size);
	bool SetChatEmote(int userid, char emote_id);
	bool SendPlayerHeartbeat(int lid, int userid, int localsid);
	bool SendSwitchServerSuccess(int lid, int userid, int localsid,int gs_id);
	bool SendSwitchServerTimeout(int lid, int userid, int localsid);
	bool SendSwitchServerCancel(int lid, int userid, int localsid);
	bool SendSwitchServerStart(int lid, int userid, int localsid,int src_gsid, int dst_gsid,const void * key,size_t key_size);
	void SendCosmeticRequest(int userid, int ticket_index, int ticket_id);
	void CancelCosmeticRequest(int userid);
	void SendDebugCommand(int roleid, int type, const char * buf, size_t size);
	bool SendChangeGsRe( int retcode, int roleid, int localsid, 
			int targetgs =-1, int tag = -1, float x = 0, float y = 0, float z = 0, int reason = 0);

	void SendTaskAsyncData(int roleid, const void * buf, size_t size);		//家族人物
	void SendUpdateFamilySkill(int roleid, int familyid, int id, int ability);	//家族技能熟练度修正
	void SendFamilyExpShare(int roleid, int familyid, int level, int64_t exp);		//家族经验分享

	/*
		下面的函数一般不需要调用
	*/
	bool SendToLS(int lid, GNET::Protocol * p);

	void SetTaskData(int taskid, const void * buf, size_t size);
	void GetTaskData(int taskid, int uid, const void * env_data, size_t env_size);
	void QuitSect(int sectid, char reason, int disciple);
	void UpdateSect(int sectid, int reputation, const int * skill_list, size_t skill_count);	//skill_list里保存的是 skill/level对， skill_count 是skill/level对的数量

	bool SendCommonData (int session_key, int data_type, void *p, size_t size);
	bool SendCommonDataRpcCall (int session_key, int data_type, void *p, size_t size, void *user_data);


//call back
	void RecvTaskAsyncData(int roleid, int result, const void * buf, size_t size);
	void OnSyncFamilyData(int roleid, int familyid, const int * list,size_t count);

	void ReportCheater2Gacd(int roleid, int cheattype, const void *buf, size_t size);
	void TriggerQuestion2Gacd(int roleid);

//Cert related
	bool SendCertChallenge(const void *c_buf, size_t c_size, const void *r_buf, size_t r_size);
	void RecvCertChallengeResult(int result, const void *response, size_t size); //gs should implement this

//SNS message
	bool SendPressMessage(int roleid, int message_type, unsigned char occupation, char gender, int level,
		int faction_leve, const void *message, size_t message_len); 
	bool SendApplyMessage(int roleid, char oper, char message_type, int message_id, int dst_index, const void *message, 
		size_t message_len); 
	bool SendVoteMessage(int roleid, char oper, char message_type, int message_id); 
	
	bool SendCashUsed(int roleid,  int bonus);
	bool SendLevelUp(int roleid, int64_t exp, int level, int reborn_cnt);

	bool SendChangeStyle(int roleid, unsigned char face_id, unsigned char hair_id, unsigned char earid, unsigned char tailid, unsigned char fashionid);

	bool QueryNetbarReward(int roleid);
	
	bool SendAchievementMsg(int broad_type, int roleid, unsigned short achieve_id, int param, int finish_time);
	bool SendPKMsg(int killer, int deader);
	bool SendRefineMsg(int roleid, int itemid, int refine_level);

	bool SendAddCirclePoint(int circleid, int point);
	bool SendQueryFriendNum(int roleid, int taskid);
	bool SendCirlceAsyncData(int roleid, const void * buf, size_t size);


//
	bool SendPlayerTryChangeDS(int roleid, char flag);
	bool SendPlayerChangeDSRe(int retcode, int roleid, int gsid, int localsid, char flag); 

	bool SendApplyCollisionRaid(int raid_id, std::vector<raid_apply_info> & apply_list, int reborn_cnt, char is_cross);	//is_reborn(飞升)
	bool SendApplyHideAndSeekRaid(int raid_id,std::vector<int> & apply_vec); 
	bool SendApplyCaptureRaid(int raid_id,std::vector<int> & apply_vec); 

	bool SendBattleFlagStart(int roleid, int flagid, int duration, int gsid, int worldtag, int factionid);
	bool SendQueryBattleFlagBuff(int roleid, int gsid, int worldtag, int factionid); 

	bool SendCouponsInfo(int userid, int toaid, int tozoneid, int roleid, std::map<int, int> & task_finish_count);

	// Add by Sunjunbo, 2010.10.17, GT Team Related
	bool SendGTSyncTeams(std::vector<gt_team_info>& teams);
	bool SendGTTeamCreate(gt_team_info& team);
	bool SendGTTeamDismiss(int64_t team_id);
	// @param operation 0: team_add  1: team_del  2: team_cap
	bool SendGTTeamMemberUpdate(int64_t team_id, const std::vector<int>& members, char operation);
	// Add End

	bool SendChangeKingdomPoint(int point);
	bool SendKingTryCallGuard(int roleid, int map_id, float pos_x, float pos_y, float pos_z);
	bool SendGuardCalledByKing(char * kingname, size_t kingname_len, int cs_index, int localsid, int roleid, int line_id,  int map_id, float pos_x, float pos_y, float pos_z); 
	bool SendSyncBathCount(int id, int count);

	void SendFacActChange(int fid, int delta);
	bool SendFacBasePropChange(int roleid, int fid, int prop_type, int delta);
	bool SendGetCS6V6Info(int roleid, int type, int level);
	bool SendFacBaseBuildingProgress(int roleid, int fid, int task_id, int value);
	bool SendFacDynDonateCash(int fid, const void * name, size_t name_len, int cash);
	bool SendFacDynPutAuction(int fid, const void * name, size_t name_len, int itemid, int time, int task);
	bool SendFacDynWinAuction(int fid, const void * name, size_t name_len, int itemid, int cost);
	bool SendFacDynBuyAuction(int fid, const void * name, size_t name_len, int itemid, int time);
	bool SendFacDynBuildingUpgrade(int fid, const void * name, size_t name_len, int tid, int level, int grass, int mine, int food, int core);
	bool SendFacDynBuildingComplete(int fid, int tid, int level);
	bool SendFacMallChange(int fid, const std::set<int> & mall);
	bool SendFacBaseData(int linkid, int roleid,int sid,const void * buf, size_t size);
	bool BroadcastFacBaseData(int fid,const void * buf, size_t size); //基地信息全帮派广播
	bool DeliverFacBaseCMD(int roleid, int fid, const void * buf, size_t size);
	bool DeliverTopicSite( int64_t l_roleid,int l_event_type,const void* buf, size_t size ); // Youshuang add
	bool SendFacCouponAdd( int64_t roleid,  int64_t fac_coupon_add );  // Youshuang add
	bool SendFactionMultiExp(int fid, int multi, int end_time);
	bool SendFacBasePutAuction(int roleid, const void * name, size_t name_len, int fid, int itemid);

	void MakeGsRoleInfo(const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list, GNET::GConsignGsRoleInfo * roleinfo);
	bool SendGsRoleInfo2Platform(int roleid, const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list, const role_basic_attribute_ext & ext);
}

#endif

