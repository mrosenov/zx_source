
#ifndef __GNET_PLAYERBASEINFOCRC_RE_HPP
#define __GNET_PLAYERBASEINFOCRC_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class PlayerBaseInfoCRC_Re : public GNET::Protocol
{
	#include "playerbaseinfocrc_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidLocalsid(localsid,roleid))
			return;
		unsigned int tmplocalsid=localsid;
		this->localsid=_SID_INVALID;
		GLinkServer::GetInstance()->Send(tmplocalsid,this);

	}
};

};

#endif
