
#ifndef __GNET_CONTESTINVITE_RE_HPP
#define __GNET_CONTESTINVITE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "contestmanager.h"
#include "contestbegin.hpp"
#include "gproviderserver.hpp"
#include "gdeliveryserver.hpp"


namespace GNET
{

class ContestInvite_Re : public GNET::Protocol
{
	#include "contestinvite_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!accept_invite)
			return;
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL==pinfo )
			return;

		ContestManager &cm = ContestManager::GetInstance();
		if(cm.IsPermitJoinState())
		{
			if(cm.IsPlayerActive(roleid))
			{
				Log::log(LOG_ERR,"gdelivery::ContestInvite_Re: player %d is active\n", roleid);
				return;
			}

			int star_count = cm.InsertPlayer(roleid, pinfo->level, pinfo->linksid, pinfo->localsid,pinfo->gameid);
			PlayerContestInfo* cinfo = cm.GetPlayerContestInfo(roleid);
			if(cinfo==NULL)
				return;
			ContestBegin cb(roleid, star_count, 0, 0, pinfo->localsid);
			cb.right_amount = cinfo->right_amount;
			cb.score = cinfo->score; 
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, cb);
	
		}

	}
};

};

#endif
