#include "world.h"
#include "player_race.h"
#include "clstab.h"

DEFINE_SUBSTANCE(gplayer_race_pvp_imp,gplayer_imp,CLS_RACE_PVP_PLAYER_IMP)

int 
gplayer_race_pvp_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
		case GM_MSG_ENCHANT:
			return gactive_imp::MessageHandler(msg);
		case GM_MSG_ENCHANT_ZOMBIE:
			if(IsDead())
			{
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				HandleEnchantMsg(msg,&ech_msg);
			}
		return 0;
	}
	return gplayer_imp::MessageHandler(msg);
}

void
gplayer_race_pvp_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack =  0;
}

void
gplayer_race_pvp_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack =  0;
}

void 
gplayer_race_pvp_imp::PlayerEnterWorld()
{	
	gplayer* pPlayer = GetParent();
	pPlayer->SetExtraState(gplayer::STATE_ANONYMOUS);
	gplayer_imp::PlayerEnterWorld(); 

	//通知玩家进入竞技区
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(3);
}

void 
gplayer_race_pvp_imp::PlayerEnterMap(int old_tag , const A3DVECTOR& old_pos)
{
	gplayer* pPlayer = GetParent();
	pPlayer->SetExtraState(gplayer::STATE_ANONYMOUS);
	gplayer_imp::PlayerEnterMap(old_tag, old_pos); 

	//通知玩家进入竞技区
	
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(3);
}

void 
gplayer_race_pvp_imp::PlayerLeaveMap()
{
	//通知玩家进入离开竞技区
	gplayer* pPlayer = GetParent();
	pPlayer->ClrExtraState(gplayer::STATE_ANONYMOUS);
	EnableFreePVP(false);
	_runner->player_enable_free_pvp(0);
}

gactive_imp::attack_judge 
gplayer_race_pvp_imp::GetHookAttackJudge()
{
	return gplayer_race_pvp_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge 
gplayer_race_pvp_imp::GetHookEnchantJudge()
{
	return gplayer_race_pvp_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill 
gplayer_race_pvp_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill 
gplayer_race_pvp_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

enum
{	
	RACE_FACTION_1 	= (1 << 16),
	RACE_FACTION_2	= (1 << 17),
};


int 
gplayer_race_pvp_imp::GetFaction()
{
	if(GetRace() == RACE_HUMAN || GetRace() == RACE_TIANMAI)
	{
		return RACE_FACTION_1;
	}
	else
	{
		return RACE_FACTION_2;
	}
}

int 
gplayer_race_pvp_imp::GetEnemyFaction()
{
	if(GetRace() == RACE_HUMAN || GetRace() == RACE_TIANMAI)
	{
		return RACE_FACTION_2;
	}
	else
	{
		return RACE_FACTION_1;
	}
}



bool 
gplayer_race_pvp_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//按照通常的判定来进行
	amsg.force_attack = 0;
	return true;
}

bool 
gplayer_race_pvp_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void 
gplayer_race_pvp_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void 
gplayer_race_pvp_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

