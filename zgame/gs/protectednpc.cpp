#include "protectednpc.h"
#include "clstab.h"


DEFINE_SUBSTANCE(protected_npc,service_npc,CLS_PROTECTED_NPC_IMP)


void 
protected_npc::OnDeath(const XID & lastattack,bool is_invader, bool no_drop, bool is_hostile_duel, int time)
{
	MSG msg;
	BuildMessage2(msg,GM_MSG_PROTECTED_NPC_NOTIFY,XID(GM_TYPE_PLAYER, _owner_id),_parent->ID,_parent->pos, _task_id, enumTaskProtectNPCDie); 
	gmatrix::SendMessage(msg);

	gnpc_imp::OnDeath(lastattack, is_invader, no_drop, is_hostile_duel, time);
}

void
protected_npc::OnProtectedFinish()
{
	MSG msg;
	BuildMessage2(msg,GM_MSG_PROTECTED_NPC_NOTIFY,XID(GM_TYPE_PLAYER, _owner_id),_parent->ID,_parent->pos, _task_id, enumTaskProtectNPCSuccess); 
	gmatrix::SendMessage(msg);
}
