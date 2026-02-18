
#ifndef __GNET_SELECTROLE_HPP
#define __GNET_SELECTROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "gproviderserver.hpp"
#include "playerlogin.hpp"
namespace GNET
{

class SelectRole : public GNET::Protocol
{
	#include "selectrole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		STAT_MIN5("LinkCompressRatio", (int64_t)(CompressARCFourSecurity::GetRatio()*100));
		//Log::trace("LinkCompressRatio %lld", (int64_t)(CompressARCFourSecurity::GetRatio()*100));
		GLinkServer* lsm=GLinkServer::GetInstance();
		int check_limit_res = GProviderServer::GetInstance()->CheckLineLimit(lineid, flag);
		
		Log::log(LOG_DEBUG, "glinkd: SelectRole check line player limit res %d of line %d, player %d flag %d", check_limit_res, lineid, roleid, flag);
		//DEBUG_PRINT("glinkd: SelectRole check line player limit res %d of line %d, player %d flag %d\n", check_limit_res, lineid, roleid, flag);
		if( check_limit_res != 0 )	
		{
			if( check_limit_res == -1 )
				GLinkServer::GetInstance()->SendErrorInfo(sid,ERR_NO_LINE_AVALIABLE,"Server Network Error.");
			if( check_limit_res == -2 )
				GLinkServer::GetInstance()->SendErrorInfo(sid,ERR_INVALID_LINEID,"Server Network Error.");
			if( check_limit_res == 1 )
				GLinkServer::GetInstance()->SendErrorInfo(sid,ERR_EXCEED_LINE_MAXNUM,"Server Network Error.");
			return;
		}
		PlayerLogin pl(roleid,lineid,GProviderServer::GetProviderServerID(),sid);
		pl.flag = flag;
		if (GDeliveryClient::GetInstance()->SendProtocol(pl))
		{
			Log::log(LOG_DEBUG,"glinkd: SelectRole send PlayerLogin, roleid=%d, sid=%d", roleid, sid);
			lsm->ChangeState(sid,&state_GSelectRoleReceive);
		}
		else
			GLinkServer::GetInstance()->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
	}
};

};

#endif
