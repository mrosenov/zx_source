#ifndef __ONLINEGAME_GS_PLAYER_IMP_FREE_CULT_PVP_H__
#define __ONLINEGAME_GS_PLAYER_IMP_FREE_CULT_PVP_H__

#include "player_imp.h"


//混沌使用
//正常PVP逻辑，怪物打死不掉经验
class gplayer_cult_pvp_imp: public gplayer_imp
{
private:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);

	static void FillAttackFation(int cultivation, int & faction , int &enemy_faction);
	
public:
	DECLARE_SUBSTANCE(gplayer_cult_pvp_imp);
	virtual int MessageHandler(const MSG & msg);
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target,enchant_msg & enchant);
	virtual void PlayerEnterWorld();  
	virtual void PlayerEnterMap( int old_tag , const A3DVECTOR& old_pos);
	virtual void PlayerLeaveMap(); 

	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();
	virtual int GetFaction();
	virtual int GetEnemyFaction();
};


//仙魔佛使用
//被怪物打死仍然会正常掉经验
class gplayer_cult2_pvp_imp : public gplayer_cult_pvp_imp
{
public:
	DECLARE_SUBSTANCE(gplayer_cult2_pvp_imp);
	virtual bool CanDropExpPVPMode() const {return true;}
};

#endif

