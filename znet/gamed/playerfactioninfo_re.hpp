
#ifndef __GNET_PLAYERFACTIONINFO_RE_HPP
#define __GNET_PLAYERFACTIONINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "guserfaction"
namespace GNET
{
void ReceivePlayerFactionInfo(int roleid,unsigned int faction_id,unsigned int family_id , char faction_title, void * faction_name ,size_t name_len);

class PlayerFactionInfo_Re : public GNET::Protocol
{
	#include "playerfactioninfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		for(std::vector<GUserFaction>::iterator it=list.begin(),ie=list.end();it!=ie;++it)
			ReceivePlayerFactionInfo(it->rid,it->factionid,it->familyid,it->title, NULL, 0);
			;
		
	}
};

};

#endif
