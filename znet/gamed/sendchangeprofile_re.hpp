
#ifndef __GNET_SENDCHANGEPROFILE_RE_HPP
#define __GNET_SENDCHANGEPROFILE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_change_style(int roleid, unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, 
		unsigned char fashionid);

namespace GNET
{

class SendChangeProfile_Re : public GNET::Protocol
{
	#include "sendchangeprofile_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(0 == retcode)
		{
			player_change_style(roleid, faceid, hairid, earid, tailid, fashionid);
		}		
	}
};

};

#endif
