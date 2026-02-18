
#ifndef __GNET_ADDCASH_RE_HPP
#define __GNET_ADDCASH_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"

namespace GNET
{

class AddCash_Re : public GNET::Protocol
{
	#include "addcash_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GAuthClient::GetInstance()->SendProtocol( *this );
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if(pinfo!=NULL && pinfo->status==_STATUS_ONGAME)
		{
	       		userid = pinfo->roleid;//send roleid to gs in userid field
			if(pinfo->roleid && GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, this))
				LOG_TRACE("AddCash_Re:Send to GS success.roleid=%d\n", pinfo->roleid);
			else
				Log::log(LOG_ERR, "AddCash_Re:Send to GS failed.roleid=%d\n", pinfo->roleid);
		}
	}
};

};

#endif
