
#ifndef __GNET_ANNOUNCECHALLENGEALGO_HPP
#define __GNET_ANNOUNCECHALLENGEALGO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class AnnounceChallengeAlgo : public GNET::Protocol
{
	#include "announcechallengealgo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("AnnounceChallengeAlgo: algo=%d, auth_version=%d\n", challenge_algo, auth_version);
		GLinkServer *lnkserver = GLinkServer::GetInstance();
		lnkserver->SetChallengeAlgo(challenge_algo);
		lnkserver->SetAuthVersion(auth_version);
	}
};

};

#endif
