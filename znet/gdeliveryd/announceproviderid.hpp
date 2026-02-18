
#ifndef __GNET_ANNOUNCEPROVIDERID_HPP
#define __GNET_ANNOUNCEPROVIDERID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gproviderserver.hpp"
#include "log.h"

#include <gdeliveryserver.hpp>
#include <playerstatusannounce.hpp>
#include "mapforbid.h"
#include "mapuser.h"
#include "lineinfo"

namespace GNET
{

class AnnounceProviderID : public GNET::Protocol
{
	#include "announceproviderid"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//DEBUG_PRINT("gdeliveryserver::receive announce_provider_id\n");
		DEBUG_PRINT("gdeliveryd: announcepid received, id %d, attr %d, lineid %d, namesize %d, attrsize %d, edition.size %d\n",
			id, attr, lineinfo.id, lineinfo.name.size(), lineinfo.attribute.size(), edition.size());
		{
			GProviderServer* psm=GProviderServer::GetInstance();
			Thread::RWLock::WRScoped l(psm->locker_gameservermap);
			if (psm->gameservermap.find(id)!=psm->gameservermap.end())
			{
				Log::log(LOG_ERR,"Identical game server id(%d) exist. Check gameserver's \".conf\" file.",
					id);
				manager->Close(sid);
				return;
			}
			bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
			if (is_central != (bool)(attr&GS_ATTR_CENTRAL))
			{
				Log::log(LOG_ERR, "AnnounceProviderID gdeliveryd type %d, gs type %d", is_central==false?0:1, attr);
				manager->Close(sid);
			}
			DEBUG_PRINT("gdeliveryserver::gameserver %d" , id);
			psm->gameservermap[id].sid = sid;
			psm->gameservermap[id].attr = attr;
			psm->gameservermap[id].max_num = 1000;
	
			GDeliveryServer::GetInstance()->SetEdition(edition);
		
			this->id=psm->GetProviderServerID();
			this->serialno = psm->GetSerialno();
			psm->Send(sid,this);
		}
		
		/* send player's status to gameserver */
		/* Allow userlogin */
		ForbidLogin::GetInstance().AllowLoginGlobal();
	}
};

};

#endif
