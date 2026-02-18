
#ifndef __GNET_PLAYERLOGIN_HPP
#define __GNET_PLAYERLOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#ifdef _TESTCODE
#include "playerlogin_re.hpp"
#include "playerlogout.hpp"
#endif

void user_login(int cs_index,int sid,int uid,const void * auth_buf, size_t auth_size,int loginip,
		bool use_spec_pos, int spec_tag,float px,float py, float pz, char trole, char flag);
namespace GNET
{

class PlayerLogin : public GNET::Protocol
{
	#include "playerlogin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		user_login(provider_link_id,localsid,roleid, 
				(const void*) &(*auth.begin()),auth.size(),loginip,
				usepos, worldtag, x, y, z, territory_role, flag);
	}
};

};

#endif
