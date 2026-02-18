
#ifndef __GNET_OPENBANQUETGETFIELD_HPP
#define __GNET_OPENBANQUETGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetmanager.h"
#include "openbanquetgetfield_re.hpp"

namespace GNET
{

class OpenBanquetGetField : public GNET::Protocol
{
	#include "openbanquetgetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//LOG_TRACE("OpenBanquetGetField roleid=%d, gs_id=%d, localsid=%d", roleid, gs_id, localsid);
		/*PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL == pinfo )
		{
			Log::log(LOG_INFO,"OpenBanquetGetField roleid=%d is not online", roleid);
			return;
		}*/
		{
			// ¿ç·þ
			SOpenBanquetID* pOpenBanquetID = OpenBanquetManager::GetInstance()->FindRolesServer(roleid);	
			if(NULL == pOpenBanquetID)
			{
				LOG_TRACE("OpenBanquetGetField roleid=%d FindRolesServer is NULL");
				return;
			}

			LOG_TRACE("OpenBanquetGetField roleid=%d, gs_id=%d, map_id=%d, localsid=%d", roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, localsid);
			std::vector<GOpenBanquetRoleInfo> attack_field;
			std::vector<GOpenBanquetRoleInfo> defend_field;
			bool role_field = OpenBanquetManager::GetInstance()->GetRoleField(roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, localsid, attack_field, defend_field);
			if(!role_field)
			{
				LOG_TRACE("OpenBanquetGetField GetRoleField is false");
				return;
			}
			
			GDeliveryServer::GetInstance()->Send(sid, OpenBanquetGetField_Re(ERR_SUCCESS, attack_field, defend_field, localsid));
		}	
	}
};

};

#endif
