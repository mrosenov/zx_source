#ifndef __GNET_FACTIONLIB_H
#define __GNET_FACTIONLIB_H

namespace GNET
{
	struct syncdata_t
	{
		unsigned int money;
		int sp;
		syncdata_t(unsigned int _m,int _s) : money(_m),sp(_s) { }
	};
	bool ForwardFactionOP( int optype,int roleid,const void* pParams,size_t param_len,object_interface obj_if );
	bool ForwardFamilyOP( int optype,int roleid,const void* pParams,size_t param_len,object_interface obj_if );
	bool QueryPlayerFactionInfo( int roleid );
	bool QueryPlayerFactionInfo( const int* list, int list_len );
	bool SendFactionLockResponse(int retcode,int tid,int roleid,const syncdata_t& syncdata);
	bool SendBattleEnd( int gs_id, int battle_world_tag, int battle_id, char battle_type);
	bool SendServerRegister( int gs_id, const void* param, size_t len );
	bool ResponseBattleStart( int retcode, int gs_id, int battle_world_tag, int battle_id, char battle_type);
	bool SendArenaChallenge(int roleid, int gs_id, int map_id, int item_id, int item_index, object_interface obj_if);

	//玩家离开战场通知
	bool SendPlayerLeave(int gs_id,int battle_id,int world_tag,int player_id,int cond_kick);
	void SendBattleStatus(int gs_id ,int battle_id, int tag ,int status);

	//剧情战场相关
	bool SendInstanceRegister(int gs_id,const void* param,size_t len);
	bool SendInstancingJoinToDelivery(int role_id,int gs_id,int battle_id,object_interface obj_if);
	bool SendPlayerLeaveInstence(int gs_id,int battle_id,int player_id,int cond_kick);
	bool ResponseInstanceStart( int retcode, int gs_id, int battle_id);


	//城战相关
	bool ForwardBattleOP( int optype,int roleid,const void* pParams,size_t param_len,object_interface obj_if );
	bool SendBattleFieldRegister(int gs_id,int battle_id,int world_tag,unsigned char use_for_battle);
	bool SendBattleFieldStartRe(int gs_id,int battle_id,int world_tag,int retcode);
	bool SendBattleFieldEnd(int gs_id,int battle_id,int world_tag,int result);
	bool SendBattleFieldBroadcast(int mafia_id,int msg_type,const void* buf,size_t size);

	//领土战相关
	bool SendTerritoryRegister(int gs_id,const void* param,size_t len);
	bool ResponseTerritoryStart( int retcode, int territory_id);
	bool SendTerritoryEnd(int territory_id, int result);
	bool SendTerritoryEnter(int roleid, unsigned int mafia_id, int territory_id);
	bool SendTerritoryLeave(int roleid, int territory_id, int map_id, float x, float y, float z);
	bool SendTerritoryChallenge(int roleid, int territory_id, unsigned int faction_id, int itemid, int item_count, object_interface obj_if);
	bool SendTerritoryGetAward(int roleid, unsigned int faction_id, int territory_id, char reward_type, int itemid, int itemcount, int money, object_interface obj_if);

	//唯一最低价竞价
	bool SendUniqueBidRequest(int roleid, int money_upperbound, int money_lowerbound, object_interface obj_if);
	bool SendUniqueBidGetItem(int roleid, object_interface obj_if);

	
	/* To implement the following interfaces */
	void ReceivePlayerFactionInfo(int roleid,unsigned int faction_id,unsigned int family_id, char faction_role, void * faction_name, size_t name_len);
	void FactionLockPlayer(unsigned int tid,int roleid);
	void FactionUnLockPlayer(unsigned int tid,int roleid,const syncdata_t& syncdata);
	
	bool SendExchangeHometownMoney(int roleid, int amount, object_interface obj_if);

	/**
	 * 副本相关协议接口
	 */
	bool SendRaidServerRegister(int gs_id, const void* param, size_t len);
	bool SendRaidOpen(int roleid, int map_id, int itemid, int item_count, char can_vote, char difficulty, const char* roomname, size_t roomname_len, object_interface obj_if);
	bool SendRaidJoin(int roleid, int map_id, int raidroom_id, int localsid, int raid_faction);
	bool SendRaidStartRe(int retcode, int gs_id, int map_id, int raidroom_id, int index);
	bool SendRaidEnterRe(int roleid, int gs_id, int retcode, int map_id, int index, int raid_type);
	bool SendRaidLeave(int gs_id, int roleid, int map_id, int index, float scalex, float scaley, float scalez, int reason);		
	bool SendRaidEnd(int gs_id, int map_id, int index, int result);	
	bool SendRaidEnterSuccess(int gs_id, int role_id, int map_id, int index);
	bool SendGFactionPkRaidFightNotify(int map_id, int raidroom_id, int end_time, char is_start);
	bool SendGFactionPkRaidDeposit(int map_id, int index, unsigned int money);
	bool SendGFactionPkRaidKill(int map_id, int index, int killer, int victim, int victim_faction);

	bool SendGlobalCountRegister(const void* param, size_t len);
	bool SendGlobalDropBoardcast(int role_id, int item_id, int left_cnt);

	bool SendChangeRoleName(int role_id, size_t name_len, char * name, int item_id, int item_pos, object_interface obj_if);
	bool SendChangeFactionName(int role_id, int fid, size_t name_len, char * name, char type, int item_id, int item_pos, object_interface obj_if);

	//国王战相关
	bool SendKingdomRegister(int gs_id, const void * param, size_t len);
	bool ResponseKingdomStart(int retcode, char fieldtype, int tag_id,  int defender_id, std::vector<int> & attacker_list, void * defender_name, size_t name_len);
	bool SendKingdomEnter(int roleid, char fieldtype);
	bool SendKingdomLeave(int roleid, char fieldtype, int tagid, int map_id, float x, float y, float z);
	bool SendKingdomBattleHalf(char fieldtype, int tag_id, char result, std::vector<int> & failattackers);
	bool SendKingdomEnd(char fieldtype, int tag_id, int winer_mafia_id);
	bool SendKingdomAttackerFail(char fieldtype, int tagid, int failer);
	bool SendKingDeliverTask(int roleid, int task_type, object_interface obj_if); 

	//领取touch奖励
	bool SendTouchPointExchange(int roleid, int flag, int count, int points_need, object_interface obj_if);

	bool SendOpenBanquetLeave(int roleid, int gs_id, int map_id);
	bool SendOpenBanquetJoin(int gs_id, bool is_team, std::vector<int>& roles);
	bool SendCreateCrossVr(int roleid, int name_len, char team_name[], object_interface obj_if);

	//领取国王连任奖励
	bool SendKingGetReward(int roleid, object_interface obj_if);

	//帮派基地相关
	bool SendFacBaseRegister(int gs_id, int max_base);
	bool SendFacBaseStartRe(int retcode, int fid, int gs_id, int index, const std::set<int> & mall_indexes);
	bool SendFacBaseEnter(int roleid);
	bool SendFacBaseLeave(int roleid, int map_id, float x, float y, float z);
	bool SendFacBaseStopRe(int retcode, int fid, int gs_id, int index);
};
#endif
