
#ifndef __GNET_GETKDCTOKENTOGATE_HPP
#define __GNET_GETKDCTOKENTOGATE_HPP

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

class GetKDCTokenToGate : public GNET::Protocol
{
	#include "getkdctokentogate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		std::stringstream profile1,profile2;
        /*      profile1<<"uid="<<userid<<"&roleid="<<roleid;
		profile1<<"&aid="<<KdsClient::GetInstance()->aid<<"&zoneid="<<KdsClient::GetInstance()->zoneid;
		profile1<<"&rname=";
		std::string profile;
		profile1 >> profile;*/
		std::string passport_str((char*)passport.begin(), passport.size());
                profile1<<"uid="<<userid<<"&uname=";
		profile2<<"&aid="<<KdsClient::GetInstance()->aid<<"&zoneid="<<KdsClient::GetInstance()->zoneid;
		profile2<<"&rid="<<roleid<<"&passport="<<passport_str;
		std::string profile1s,profile2s;
		profile1 >> profile1s;
		profile2 >> profile2s;
		std::string token;
		int ret=TokenGenerator::GetInstance().BuildToken(profile1s,rolename,profile2s,token);
		if(ret)
			DEBUG_PRINT("build token err\n");
		DEBUG_PRINT("GetToken profile1s=%.*s rolenamesize=%d profile2s=%.*s tokensize=%d token=%.*s passport.size=%d",profile1s.size(),\
				profile1s.c_str(),rolename.size(),profile2s.size(),profile2s.c_str(),token.size(),token.size(),\
				token.c_str(), passport.size());
		GameGateClient::GetInstance()->SendProtocol(GetKDCToken_Re(Octets(token.c_str(),token.size()),ret,localsid,linksid));
	}
};

};

#endif
