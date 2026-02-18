#include "player_zone.h"
#include "clstab.h"
#include "filter_man.h"


DEFINE_SUBSTANCE(gplayer_zone_imp,gplayer_imp,CLS_ZONE_PLAYER_IMP)

int
gplayer_zone_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
		{
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);

			XID attacker = ack_msg.ainfo.attacker;
			if(attacker.id == _parent->ID.id) return 0;
			if(ack_msg.attacker_mode & (attack_msg::PVP_SANCTUARY) && !(ack_msg.attacker_mode & (attack_msg::PVP_DUEL)) && ack_msg.ainfo.attacker.IsPlayerClass()) return 0;
			if(ack_msg.ainfo.zone_id == GetParent()->id_zone && !(ack_msg.force_attack & C2S::FORCE_ATTACK_ZONE) && !(ack_msg.attacker_mode & (attack_msg::PVP_DUEL)) ) return 0;
			if(attacker.id != _duel_target && (IsInTeam() && IsMember(attacker))) return 0;
			if(attacker.IsPlayerClass() && ack_msg.is_flying != GetParent()->IsFlyMode()) return 0;

                        ack_msg.target_faction = 0xFFFFFFFF;
			ack_msg.is_invader =  false;

			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(msg,&ack_msg);
		}
		return 0;

		case GM_MSG_ENCHANT:
		{
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);

			if(ech_msg.helpful)
			{
				//空中不能给地面的玩家加祝福
				if(ech_msg.ainfo.attacker.IsPlayerClass() && ech_msg.is_flying != GetParent()->IsFlyMode()) return 0;
				if(ech_msg.ainfo.zone_id != GetParent()->id_zone ) return 0;
			}
			else
			{
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
				if(ech_msg.attacker_mode & (attack_msg::PVP_SANCTUARY) && !(ech_msg.attacker_mode & (attack_msg::PVP_DUEL)) && ech_msg.ainfo.attacker.IsPlayerClass()) return 0;
				if(ech_msg.ainfo.zone_id == GetParent()->id_zone && !(ech_msg.force_attack & C2S::FORCE_ATTACK_ZONE) && !(ech_msg.attacker_mode & (attack_msg::PVP_DUEL)) ) return 0;
				if(attacker.id != _duel_target && (IsInTeam() && IsMember(attacker))) return 0;
				if(attacker.IsPlayerClass() && ech_msg.is_flying != GetParent()->IsFlyMode()) return 0;

                                ech_msg.target_faction = 0xFFFFFFFF;
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}

			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
		}
		return 0;
			
		case GM_MSG_ENCHANT_ZOMBIE:
		{
			if(IsDead())
			{
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				if( !ech_msg.helpful )
				{
					return 0;
				}
				if(ech_msg.ainfo.zone_id != GetParent()->id_zone) return 0;

				HandleEnchantMsg(msg,&ech_msg);
			}
		}
		return 0;
		
		case GM_MSG_TEAM_INVITE:
		{
			ASSERT(msg.content_length == sizeof(msg_team_invite));
			const msg_team_invite * pMsg = (const msg_team_invite*)msg.content;

			if(pMsg->zone_id != GetParent()->id_zone)
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_TEAM_CANNOT_INVITE);
				return 0;
			}

			if(!_team.MsgInvite(msg.source,msg.param,pMsg->pickup_flag, pMsg->family_id, pMsg->mafia_id, pMsg->level, pMsg->sectid, pMsg->referid))
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_TEAM_CANNOT_INVITE);
			}
		
		}
		return 0;
	}
	return gplayer_imp::MessageHandler(msg);
}

void
gplayer_zone_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.ainfo.zone_id = GetParent()->id_zone;
}

void
gplayer_zone_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.ainfo.zone_id = GetParent()->id_zone;
}

void 
gplayer_zone_imp::PlayerEnterWorld()
{	
	gplayer* pPlayer = GetParent();
	pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);
	gplayer_imp::PlayerEnterWorld(); 

	//通知玩家进入竞技区
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(4);
}

void 
gplayer_zone_imp::PlayerEnterMap(int old_tag , const A3DVECTOR& old_pos)
{
	gplayer* pPlayer = GetParent();
	pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);
	gplayer_imp::PlayerEnterMap(old_tag, old_pos); 

	//通知玩家进入竞技区
	EnableFreePVP(true);
	_runner->player_enable_free_pvp(4);
}

void 
gplayer_zone_imp::PlayerLeaveMap()
{
	gplayer* pPlayer = GetParent();
	pPlayer->ClrExtraState(gplayer::STATE_CROSSZONE);
	//通知玩家进入离开竞技区
	EnableFreePVP(false);
	_runner->player_enable_free_pvp(0);
}


gactive_imp::attack_judge 
gplayer_zone_imp::GetHookAttackJudge()
{
	return gplayer_zone_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge 
gplayer_zone_imp::GetHookEnchantJudge()
{
	return gplayer_zone_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill 
gplayer_zone_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill 
gplayer_zone_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

bool 
gplayer_zone_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//安全区且不是决斗目标
	if(!__this->OI_IsPVPEnable() && __this->OI_GetDuelTarget() != msg.source.id && amsg.ainfo.attacker.IsPlayerClass()) return false;
	//同一个服务器且不是决斗目标且不是强制攻击
	if(amsg.ainfo.zone_id == __this->OI_GetZoneID() && !(amsg.force_attack & C2S::FORCE_ATTACK_ZONE) && __this->OI_GetDuelTarget() != msg.source.id ) return false;
	if(__this->OI_GetDuelTarget() != msg.source.id && __this->OI_IsInTeam() && __this->OI_IsMember(amsg.ainfo.attacker)) return false; 
	amsg.target_faction = 0xFFFFFFFF;
	amsg.is_invader =  false;

	return true;
}

bool 
gplayer_zone_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	//安全区且不是决斗目标
	if(!__this->OI_IsPVPEnable() && __this->OI_GetDuelTarget() != msg.source.id && emsg.ainfo.attacker.IsPlayerClass()) return false;
	//同一个服务器且不是决斗目标且不是强制攻击
	if(emsg.ainfo.zone_id == __this->OI_GetZoneID() && !(emsg.force_attack & C2S::FORCE_ATTACK_ZONE) && __this->OI_GetDuelTarget() != msg.source.id ) return false;
	if(__this->OI_GetDuelTarget() != msg.source.id && __this->OI_IsInTeam() && __this->OI_IsMember(emsg.ainfo.attacker)) return false; 
	emsg.target_faction = 0xFFFFFFFF;
	emsg.is_invader =  false;

	return true;
}

void 
gplayer_zone_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.ainfo.zone_id = __this->OI_GetZoneID();
}

void 
gplayer_zone_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.ainfo.zone_id = __this->OI_GetZoneID();
}

