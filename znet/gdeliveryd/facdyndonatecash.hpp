
#ifndef __GNET_FACDYNDONATECASH_HPP
#define __GNET_FACDYNDONATECASH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynDonateCash : public GNET::Protocol
{
	#include "facdyndonatecash"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facdyndonatecash fid %d cash %d", fid, cash);

		FAC_DYNAMIC::cash_donate log = {cash, 20, {}};
		FactionDynamic::GetName(rolename, log.rolename, log.rolenamesize);
		FactionManager::Instance()->RecordDynamic(fid, FAC_DYNAMIC::CASH_DONATE, log);
	}
};

};

#endif
