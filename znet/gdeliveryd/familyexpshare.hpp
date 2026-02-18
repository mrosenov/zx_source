
#ifndef __GNET_FAMILYEXPSHARE_HPP
#define __GNET_FAMILYEXPSHARE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "familymanager.h"

namespace GNET
{

class FamilyExpShare : public GNET::Protocol
{
	#include "familyexpshare"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//DEBUG_PRINT("ShareExp,familyid=%d,roleid=%d,exp=%d,level=%d",familyid,roleid,exp,level);
		FamilyManager::Instance()->ExpShare(familyid, roleid, exp, level);
	}
};

};

#endif
