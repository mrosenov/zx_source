#ifndef __GNET_TSPLATFORMAGENT_H
#define __GNET_TSPLATFORMAGENT_H
#include "localmacro.h"

namespace GNET
{

class TSPlatformAgent
{
	TSPlatformAgent(){}
	static bool tsopen;
public:
	~TSPlatformAgent(){}
	static void OpenTS(){tsopen=true;}
	static bool IsTSOpen(){return tsopen;}
	//static int AddFriend(int roleid,int friendid,const Octets& name);
	
	static int AddFamily(int roleid, int fid);
	static int AddFaction(int familyid, int factionid);
	static int LeaveFamily(int roleid, int fid);
	static int LeaveFaction(int familyid, int factionid);
};

};
#endif
