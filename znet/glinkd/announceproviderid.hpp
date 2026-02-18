
#ifndef __GNET_ANNOUNCEPROVIDERID_HPP
#define __GNET_ANNOUNCEPROVIDERID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "conv_charset.h"

#include "gproviderserver.hpp"
#include "glinkserver.hpp"
#include "log.h"
#include "lineinfo"

namespace GNET
{

class AnnounceProviderID : public GNET::Protocol
{
	#include "announceproviderid"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("glinkd: announcepid received, id %d, attr=%d, lineid %d, namesize %d, attrsize %d\n",
			id, attr, lineinfo.id, lineinfo.name.size(), lineinfo.attribute.size());
		static int retry_time;
		GProviderServer* psm=GProviderServer::GetInstance();
		Thread::RWLock::WRScoped l(psm->locker_gameservermap);
		if (psm->gameservermap.find(id)!=psm->gameservermap.end())
		{
			Log::log(LOG_ERR,"Identical game server id(%d) exist. Check gameserver's \".conf\" file.",id);
			manager->Close(sid);
			return;
		}
		GLinkServer::GetInstance()->SetEdition(edition);
		retry_time=0;

		Octets u_name;
		CharsetConverter::conv_charset_g2u(lineinfo.name, u_name);
		lineinfo.name = u_name;
		
		psm->gameservermap.insert(std::make_pair(id, GProviderServer::LineInfoEx(sid, attr, lineinfo)));
		
		this->id=psm->GetProviderServerID();
		psm->Send(sid,this);
		
	}
};

};
#endif
