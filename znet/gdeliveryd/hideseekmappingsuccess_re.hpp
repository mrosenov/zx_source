
#ifndef __GNET_HIDESEEKMAPPINGSUCCESS_RE_HPP
#define __GNET_HIDESEEKMAPPINGSUCCESS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HideSeekMappingSuccess_Re : public GNET::Protocol
{
	#include "hideseekmappingsuccess_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("HideSeekMappingSuccess_Re roleid=%d, raidroom_id=%d, agree=%d, iscross=%d", roleid, raidroom_id, agree, iscross);
		RaidManager::GetInstance()->OnHideSeekRaidMappingSuccess_Re(roleid,raidroom_id,agree);
	}
};

};

#endif
