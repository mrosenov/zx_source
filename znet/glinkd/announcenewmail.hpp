
#ifndef __GNET_ANNOUNCENEWMAIL_HPP
#define __GNET_ANNOUNCENEWMAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
namespace GNET
{

class AnnounceNewMail : public GNET::Protocol
{
	#include "announcenewmail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		unsigned int tmpsid=localsid;
		localsid=0;
		GLinkServer::GetInstance()->Send( tmpsid,this );
	}
};

};

#endif
