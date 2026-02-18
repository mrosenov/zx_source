#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>

#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>

#include "guardnpc.h"


DEFINE_SUBSTANCE(guard_policy,ai_policy, CLS_NPC_AI_POLICY_GUARD)
DEFINE_SUBSTANCE(service_policy,ai_policy, CLS_NPC_AI_POLICY_SERVICE)

DEFINE_SUBSTANCE(guard_agent,substance, CLS_NPC_GUARD_AGENT)

bool 
guard_agent::GatherTarget(ai_object * self, ai_policy * policy)
{
	//search target
	A3DVECTOR pos;
	self->GetPos(pos);
	search_target<slice> worker(self,self->GetSightRange(),self->GetEnemyFaction());
	self->GetImpl()->_plane->ForEachSlice(pos,self->GetSightRange(),worker);
	XID target;
	if(self->GetAggroEntry(0,target))
	{
		policy->OnAggro();
		return true;
	}
	return false;
}

void 
guard_policy::OnHeartbeat() 
{
	ai_policy::OnHeartbeat();
	if(!InCombat() && !_self->GetAggroCount()) 
	{
		_agent->GatherTarget(_self,this);
	}
}

bool 
service_policy::OnGreeting()
{
	if(InCombat()) return false;
	_self->ClearSession();
	ClearTask();
	ai_silent_task  * pTask = new ai_silent_task(15);
	pTask->Init(_self, this);
	AddTask(pTask);
	return true;
}

