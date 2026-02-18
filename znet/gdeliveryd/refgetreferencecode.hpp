
#ifndef __GNET_REFGETREFERENCECODE_HPP
#define __GNET_REFGETREFERENCECODE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "refspreadcode.h"
#include "refgetreferencecode_re.hpp"
#include "mapuser.h"
#include "gdeliveryserver.hpp"
#include "conv_charset.h"
#include "localmacro.h"


namespace GNET
{

class RefGetReferenceCode : public GNET::Protocol
{
	#include "refgetreferencecode"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!ReferenceManager::GetInstance()->IsOpen())
			return;
		PlayerInfo *playerinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		GRoleInfo *pinfo = RoleInfoCache::Instance().Get(roleid);
		if (pinfo!=NULL && playerinfo!=NULL)
		{
			GDeliveryServer *dsm = GDeliveryServer::GetInstance();
			RefGetReferenceCode_Re re;
			re.roleid = roleid;
			re.localsid = localsid;
			if (pinfo->reborn_cnt==0 && pinfo->level<REF_LIMIT_REFERRERLEVEL)
				re.retcode = REF_ERR_REFREFERRERLEVEL;
			else
			{
				Octets temp;
				RefSpreadCode::Encode(playerinfo->userid, dsm->district_id, roleid, temp);
				CharsetConverter::conv_charset_t2u(temp, re.refcode);
				re.retcode = REF_ERR_SUCCESS;
			}
			dsm->Send(sid, re);
		}
	}
};

};

#endif
