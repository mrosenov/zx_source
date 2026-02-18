
#ifndef __GNET_COMBATCHALLENGE_HPP
#define __GNET_COMBATCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "combatchallenge_re.hpp"

namespace GNET
{

class CombatChallenge : public GNET::Protocol
{
	#include "combatchallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("CombatChallenge, roleid=%d, factionid=%d", roleid, factionid);
		CombatInviteArg arg;
		Octets fname;
		int attacker, defender, level1,level2;
		unsigned int localsid;
		defender = factionid;

		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if(NULL == pinfo || pinfo->factiontitle>_R_VICEMASTER)
				return;
			arg.roleid = roleid;
			arg.role_name = pinfo->name;
			attacker = pinfo->factionid;
			localsid = pinfo->localsid;
		}
		arg.factionid = attacker;
		if(attacker==defender)
			return;
		if(!FactionManager::Instance()->GetBrief(attacker, arg.faction_name, level1))
			return;
		if(!FactionManager::Instance()->GetBrief(defender, fname, level2))
			return;
		if(level1<1 || level2<1)
		{
			CombatChallenge_Re re(ERR_COMBAT_LOWLEVEL, attacker, localsid);
			manager->Send(sid, re);
			return;
		}
		int ret = CombatManager::Instance()->CanChallenge(attacker, defender);
		if(ret)
		{
			CombatChallenge_Re re(ret, attacker, localsid);
			manager->Send(sid, re);
			return;
		}

		bool ret1, ret2;
		unsigned int linksid1, linksid2, localsid1, localsid2;
		
		ret1 = FactionManager::Instance()->FindMaster(defender, linksid1, localsid1);
		ret2 = FactionManager::Instance()->FindVice(defender, linksid2, localsid2);

		if(!ret1&&!ret2)
		{
			DEBUG_PRINT("CombatChallenge, master is not online, factionid=%d", factionid);
			CombatChallenge_Re re(ERR_COMBAT_MASTEROFFLINE, attacker, localsid);
			manager->Send(sid, re);
			return;
		}
		if(!FactionManager::Instance()->DecProsperity(attacker, 50))
		{
			CombatChallenge_Re re(ERR_COMBAT_NOPROSPERITY, attacker, localsid);
			manager->Send(sid, re);
			return;
		}
		CombatManager::Instance()->CreateController(attacker, defender, arg.faction_name, fname);
		if(ret1)
		{
			arg.localsid = localsid1;
			CombatInvite* rpc = (CombatInvite*) Rpc::Call( RPC_COMBATINVITE,arg);
			rpc->defender = defender;
			rpc->attacker = attacker;
			manager->Send(linksid1, rpc);
		}
		if(ret2)
		{
			arg.localsid = localsid2;
			CombatInvite* rpc = (CombatInvite*) Rpc::Call( RPC_COMBATINVITE,arg);
			rpc->defender = defender;
			rpc->attacker = attacker;
			manager->Send(linksid2, rpc);
		}
	}
};

};

#endif
