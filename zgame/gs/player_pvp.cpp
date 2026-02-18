#include "world.h"
#include "player_pvp.h"
#include "clstab.h"

DEFINE_SUBSTANCE(gplayer_pvp_imp,gplayer_imp,CLS_PVP_PLAYER_IMP)
int 
gplayer_pvp_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{

		case GM_MSG_ATTACK:
			{
				ASSERT(msg.content_length >= sizeof(attack_msg));
				attack_msg & amsg = *(attack_msg*)msg.content;
				if(!(amsg.attacker_mode & attack_msg::PVP_FREE) 
						|| (amsg.attacker_mode & attack_msg::PVP_DUEL))
				{
					return gplayer_imp::MessageHandler(msg);
				}
				
				//进行处理
				attack_msg ack_msg = *(attack_msg*)msg.content;
				_filters.EF_TranslateRecvAttack(msg.source, ack_msg);

				//攻击判定和普通的一致，只是不会有红名惩罚
				if(!TestAttackMsg(msg,ack_msg)) return 0;
				ack_msg.is_invader =  false;

				//试着选择对象
				((gplayer_controller*)_commander)->TrySelect(msg.source);
				HandleAttackMsg(msg,&ack_msg);
				return 0;
			}
			break;
		case GM_MSG_ENCHANT:
			{
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg & emsg = *(enchant_msg*)msg.content;
				if(!(emsg.attacker_mode & attack_msg::PVP_FREE)
						|| (emsg.attacker_mode & attack_msg::PVP_DUEL))
				{
					return gplayer_imp::MessageHandler(msg);
				}

				//进行处理
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
				if(!ech_msg.helpful)
				{
					if(!TestHarmfulEnchant(msg,ech_msg)) return 0;
				}
				else
				{
					if(!TestHelpfulEnchant(msg,ech_msg)) return 0;
				}
				//有益法术可以随便使用，所以没有任何过滤和判断了
				ech_msg.is_invader = false;
				HandleEnchantMsg(msg,&ech_msg);
				return 0;
			}

		case GM_MSG_ENCHANT_ZOMBIE:
		{
			if(IsDead())
			{
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				if(!ech_msg.helpful)
				{
					return 0;
				}
				ech_msg.is_invader = false;
				HandleEnchantMsg(msg,&ech_msg);
			}
		}
		return 0;
	}
	return gplayer_imp::MessageHandler(msg);
}

void
gplayer_pvp_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack |= C2S::FORCE_ATTACK_ALL;
	attack.attacker_mode = attack_msg::PVP_FREE;
}

void
gplayer_pvp_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack |= C2S::FORCE_ATTACK_ALL;
	enchant.attacker_mode = attack_msg::PVP_FREE;
}

void 
gplayer_pvp_imp::PlayerEnterWorld()
{	
	gplayer_imp::PlayerEnterWorld(); 

	EnableFreePVP(true);
	//通知玩家进入竞技区
	_runner->player_enable_free_pvp(1);
}

void 
gplayer_pvp_imp::PlayerEnterMap(int old_tag , const A3DVECTOR& old_pos)
{
	gplayer_imp::PlayerEnterMap(old_tag, old_pos); 

	//通知玩家进入竞技区
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(1);
}

void 
gplayer_pvp_imp::PlayerLeaveMap()
{
	//通知玩家进入离开竞技区
	EnableFreePVP(false);
	_runner->player_enable_free_pvp(0);
}

gactive_imp::attack_judge 
gplayer_pvp_imp::GetHookAttackJudge()
{
	return gplayer_pvp_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge 
gplayer_pvp_imp::GetHookEnchantJudge()
{
	return gplayer_pvp_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill 
gplayer_pvp_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill 
gplayer_pvp_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

template <typename MESSAGE,typename EFFECT_MSG>
inline static  bool HookTestHarmfulEffect(gactive_imp * __this, const MESSAGE & msg, EFFECT_MSG & emsg)
{
	ASSERT(false);
	return true;
}

bool 
gplayer_pvp_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	if( (!(amsg.attacker_mode & attack_msg::PVP_FREE)) || (amsg.attacker_mode & attack_msg::PVP_DUEL))
	{
		//按照通常的判定来进行
		return gplayer_imp::__GetHookAttackJudge(__this,msg,amsg);
	}
	return HookTestHarmfulEffect(__this,msg,amsg);
				
}

bool 
gplayer_pvp_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	if( (!(emsg.attacker_mode & attack_msg::PVP_FREE)) || (emsg.attacker_mode & attack_msg::PVP_DUEL))
	{
		return gplayer_imp::__GetHookEnchantJudge(__this, msg,emsg);
	}

	if(!emsg.helpful)
	{
		return HookTestHarmfulEffect(__this,msg,emsg);
	}
	//有益法术可以随便使用，所以没有任何过滤和判断了
	emsg.is_invader = false;
	return true;
}

void 
gplayer_pvp_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack |= C2S::FORCE_ATTACK_ALL;
	attack.attacker_mode = attack_msg::PVP_FREE;
}

void 
gplayer_pvp_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack |= C2S::FORCE_ATTACK_ALL;
	enchant.attacker_mode = attack_msg::PVP_FREE;
}

