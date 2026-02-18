
#ifndef __GNET_CROSSRAIDENTER_RE_HPP
#define __GNET_CROSSRAIDENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crossbattlemanager.hpp"
#include "centraldeliveryclient.hpp"
#include "trychangeds.hpp"
#include "protocolexecutor.h"

#include "sendraidenter.hpp"

namespace GNET
{

class CrossRaidEnter_Re : public GNET::Protocol
{
	#include "crossraidenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("CrossRaidEnter_Re::retcode=%d,roleid=%d,map_id=%d,gs_id=%d,templateid=%d,index=%d,raid_type=%d", retcode, roleid, map_id,gs_id,templateid,index,raid_type);
		
		if(retcode == ERR_SUCCESS)
		{
			CrossBattleManager *crossbattle = CrossBattleManager::GetInstance();
			crossbattle->InsertFightingTag(gs_id, roleid, 0, 0, 0, CRSSVRTEAMS_BATTLE);

			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if(pinfo)
			{
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, SendRaidEnter(roleid,map_id,templateid,index,raid_type,Octets(0)));
			}
			else
			{
				LOG_TRACE("CrossRaidEnter_Re roleid=%d is not online", roleid);
			}
			//ProtocolExecutor *task = new ProtocolExecutor(GProviderServer::GetInstance(), 0, new TryChangeDS(roleid, DS_TO_BATTLEMAP));
			//Thread::Pool::AddTask(task);
		}
	}
};

};

#endif
