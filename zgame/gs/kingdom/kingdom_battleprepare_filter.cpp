#include "kingdom_battleprepare_filter.h"
#include "../clstab.h"
#include "../player_imp.h"
#include "../statedef.h"


DEFINE_SUBSTANCE(kingdom_battleprepare_filter,filter,CLS_FILTER_KINGDOM_BATTLEPREPARE)

void 
kingdom_battleprepare_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack  = 0;
	msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
}	

void 
kingdom_battleprepare_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	msg.target_faction = 0;
	msg.force_attack = 0; 
	msg.attack_state |= gactive_imp::AT_STATE_IMMUNE;
}


void 
kingdom_battleprepare_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();

	pImp->_commander->DenyCmd(controller::CMD_MOVE);
	pImp->_commander->DenyCmd(controller::CMD_SKILL);
	pImp->_commander->DenyCmd(controller::CMD_BOT);
	pImp->_commander->DenyCmd(controller::CMD_ATTACK);
	pImp->_commander->DenyCmd(controller::CMD_BIND);
	_parent.SendClientNotifyRoot(0);
	_parent.UpdateBuff(GNET::HSTATE_INVINCIBLE, _filter_id, _timeout);
}

void 
kingdom_battleprepare_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->_commander->AllowCmd(controller::CMD_MOVE);
	pImp->_commander->AllowCmd(controller::CMD_SKILL);
	pImp->_commander->AllowCmd(controller::CMD_BOT);
	pImp->_commander->AllowCmd(controller::CMD_ATTACK);
	pImp->_commander->AllowCmd(controller::CMD_BIND);
	_parent.SendClientDispelRoot(0);
	_parent.RemoveBuff(GNET::HSTATE_INVINCIBLE, _filter_id);
}
