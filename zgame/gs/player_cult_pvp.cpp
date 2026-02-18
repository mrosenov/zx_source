#include "world.h"
#include "player_cult_pvp.h"
#include "clstab.h"

DEFINE_SUBSTANCE(gplayer_cult_pvp_imp,gplayer_imp,CLS_CULT_PVP_PLAYER_IMP)

int 
gplayer_cult_pvp_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
		case GM_MSG_ENCHANT:
		{
			return gactive_imp::MessageHandler(msg);
		}
		return 0;

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
gplayer_cult_pvp_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack =  0;
	FillAttackFation(OI_GetCultivation(), attack.attacker_faction ,attack.target_faction);
}

void
gplayer_cult_pvp_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack =  0;
	FillAttackFation(OI_GetCultivation(), enchant.attacker_faction ,enchant.target_faction);
}

void 
gplayer_cult_pvp_imp::PlayerEnterWorld()
{	
	gplayer_imp::PlayerEnterWorld(); 

	//通知玩家进入竞技区
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(2);
}

void 
gplayer_cult_pvp_imp::PlayerEnterMap(int old_tag , const A3DVECTOR& old_pos)
{
	gplayer_imp::PlayerEnterMap(old_tag, old_pos); 

	//通知玩家进入竞技区
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(2);
}

void 
gplayer_cult_pvp_imp::PlayerLeaveMap()
{
	//通知玩家进入离开竞技区
	EnableFreePVP(false);
	_runner->player_enable_free_pvp(0);
}

gactive_imp::attack_judge 
gplayer_cult_pvp_imp::GetHookAttackJudge()
{
	return gplayer_cult_pvp_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge 
gplayer_cult_pvp_imp::GetHookEnchantJudge()
{
	return gplayer_cult_pvp_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill 
gplayer_cult_pvp_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill 
gplayer_cult_pvp_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

enum
{	
	CULT_FACTION_1 	= (1 << 12),
	CULT_FACTION_2	= (1 << 13),
	CULT_FACTION_3	= (1 << 14),
	CULT_FACTION_4	= (1 << 15),
};


int 
gplayer_cult_pvp_imp::GetFaction()
{
	switch(_cultivation)
	{
		case 0x01:
				return CULT_FACTION_1; 
		case 0x02:
				return CULT_FACTION_2; 
		case 0x04:
				return CULT_FACTION_3; 
		default:
				return CULT_FACTION_4; 
	}
}
int 
gplayer_cult_pvp_imp::GetEnemyFaction()
{
	switch(_cultivation)
	{
		case 0x01:
				return CULT_FACTION_2|CULT_FACTION_3|CULT_FACTION_4;
		case 0x02:
				return CULT_FACTION_1|CULT_FACTION_3|CULT_FACTION_4;
		case 0x04:
				return CULT_FACTION_2|CULT_FACTION_1|CULT_FACTION_4;
		default:
				return CULT_FACTION_2|CULT_FACTION_3|CULT_FACTION_1;
	}
}



void 
gplayer_cult_pvp_imp::FillAttackFation(int cultivation, int & faction , int &enemy_faction)
{
	switch(cultivation)
	{
		case 0x01:
				faction = CULT_FACTION_1; enemy_faction = CULT_FACTION_2|CULT_FACTION_3|CULT_FACTION_4;
				break;
		case 0x02:
				faction = CULT_FACTION_2; enemy_faction = CULT_FACTION_1|CULT_FACTION_3|CULT_FACTION_4;
				break;
		case 0x04:
				faction = CULT_FACTION_3; enemy_faction = CULT_FACTION_2|CULT_FACTION_1|CULT_FACTION_4;
				break;
		default:
				faction = CULT_FACTION_4; enemy_faction = CULT_FACTION_2|CULT_FACTION_3|CULT_FACTION_1;
				break;
	}
}

bool 
gplayer_cult_pvp_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//按照通常的判定来进行
	amsg.force_attack = 0;
	return true;
}

bool 
gplayer_cult_pvp_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void 
gplayer_cult_pvp_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
	FillAttackFation(__this->OI_GetCultivation(), attack.attacker_faction ,attack.target_faction);
}

void 
gplayer_cult_pvp_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
	FillAttackFation(__this->OI_GetCultivation(), enchant.attacker_faction ,enchant.target_faction);
}

DEFINE_SUBSTANCE(gplayer_cult2_pvp_imp,gplayer_cult_pvp_imp,CLS_CULT2_PVP_PLAYER_IMP)
