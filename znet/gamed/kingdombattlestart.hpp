
#ifndef __GNET_KINGDOMBATTLESTART_HPP
#define __GNET_KINGDOMBATTLESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void kingdom_start(char fieldtype, int tag_id, int defender, std::vector<int> & attacker_list, void * defender_name, size_t name_len);

namespace GNET
{

class KingdomBattleStart : public GNET::Protocol
{
	#include "kingdombattlestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		kingdom_start(fieldtype, tagid, defender, attackers, def_name.begin(), def_name.size());
	}
};

};

#endif
