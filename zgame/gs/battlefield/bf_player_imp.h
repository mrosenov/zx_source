#ifndef __ONLINEGAME_GS_BF_PLAYER_IMP_H__
#define __ONLINEGAME_GS_BF_PLAYER_IMP_H__

#include "../player_imp.h"

class bf_player_imp : public gplayer_imp
{
friend class bf_world_manager_base;
	enum
	{
		DEFAULT_KICKOUT_TIME = 5,	//异常情况下多少秒后踢出
	};
	int _attack_faction;
	int _defense_faction;
	//保存进入战场时所处位置,作为返回点
	int _old_world_tag;
	A3DVECTOR _old_pos;
	
	int _timeout;
	int _kickout;
	bool _battle_result_sent;
	int _turret_counter;
	int _battle_info_seq;

public:
	DECLARE_SUBSTANCE(bf_player_imp)

	bf_player_imp();

	virtual void FillAttackMsg(const XID& target,attack_msg& attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID& target,enchant_msg& enchant);
	virtual int GetFaction();
	virtual int GetEnemyFaction();
	int MessageHandler(const MSG& msg);
	virtual void OnHeartbeat(size_t tick);
	void PostLogin(const userlogin_t& user, char trole, char loginflag);
	virtual void PlayerEnterWorld();
	virtual void PlayerLeaveWorld();
	virtual void PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos);
	virtual void PlayerLeaveMap();
	virtual void OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time);
	virtual void GetLogoutPos(int& world_tag,A3DVECTOR& pos);
	virtual void SendNormalChat(char channel,const void* buf,size_t len);

	virtual bool QueryBattlefieldConstructInfo(int type,int page = 0);
	virtual bool BattlefieldContribute(int res_type);
	virtual bool BattlefieldConstruct(int key,int type,int cur_level);
	virtual int CheckUseTurretScroll();

};

#endif

