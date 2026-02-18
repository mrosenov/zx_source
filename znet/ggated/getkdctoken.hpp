
#ifndef __GNET_GETKDCTOKEN_HPP
#define __GNET_GETKDCTOKEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include <string>
#include <sstream>
#include "tokengenerator.h"
#include "gamegateclient.hpp"
#include "getkdctoken_re.hpp"
#include "kdsclient.hpp"

namespace GNET
{

class GetKDCToken : public GNET::Protocol
{
	#include "getkdctoken"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		std::stringstream profile1;
                profile1<<"uid="<<userid<<"&roleid="<<roleid;
		profile1<<"&aid="<<KdsClient::GetInstance()->aid<<"&zoneid="<<KdsClient::GetInstance()->zoneid;
		std::string profile;
		profile1 >> profile;
		std::string token;
		int ret=TokenGenerator::GetInstance().BuildToken(profile,token);
		if(ret)
			DEBUG_PRINT("build token err\n");
		DEBUG_PRINT("GetToken profile=%.*s tokensize=%d token=%.*s",profile.size(),profile.c_str(),token.size(),
				token.size(),token.c_str());
		GameGateClient::GetInstance()->SendProtocol(GetKDCToken_Re(Octets(token.c_str(),token.size()),ret,localsid,linksid));
	}
};

};

#endif
