#ifndef __ONLINEGAME_GS_PLAYER_IMP_BATTLE_H__
#define __ONLINEGAME_GS_PLAYER_IMP_BATTLE_H__

#include "player_imp.h"

class gplayer_battleground: public gplayer_imp
{
	int turret_counter;
	int attack_faction; 	//攻击附加的faction
	int defense_faction;	//受到攻击时自己附加的faction
public:
	gplayer_battleground()
	{
		attack_faction = 0;
		defense_faction = 0;
		turret_counter = 0;
	}
private:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);
	
public:
	DECLARE_SUBSTANCE(gplayer_battleground);
	virtual	void OnHeartbeat(size_t tick);
	virtual int MessageHandler(const MSG & msg);
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target,enchant_msg & enchant);
	virtual void PlayerEnterWorld();  
	virtual void PlayerEnterMap(); 
	virtual void PlayerLeaveMap(); 
	virtual void PlayerLeaveWorld();
	virtual int GetFaction();
	virtual int GetEnemyFaction();
	virtual int  Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int target_tag);
	virtual int CheckUseTurretScroll();
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();
};


#endif

