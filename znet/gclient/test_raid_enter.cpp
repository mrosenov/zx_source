#include "raidenter.hpp"
#include "raidappoint.hpp"
#include "raidstart.hpp"
#include "raidjoinaccept.hpp"
#include "dochangegs.hpp"
#include "glinkclient.h"
#include "test_raid_enter.h"

namespace GNET
{
int TestRaidEnter::StartRaid(int raidroom_id,int mapid)
{
	RaidStart msg(GLinkClient::GetInstance()->roleid,raidroom_id,mapid,_SID_INVALID);
	GLinkClient::GetInstance()->SendProtocol(msg);
	return 0;
}
int TestRaidEnter::RaidAccept(int masterid,int map_id,int raidroom_id,int requester)
{
	RaidJoinAccept msg(1,masterid,map_id,raidroom_id,requester,_SID_INVALID);
	GLinkClient::GetInstance()->SendProtocol(msg);
	return 0;
}
int TestRaidEnter::RaidAppointRole(int masterid,int raidroom_id,int requester)
{
	RaidAppoint msg(masterid,raidroom_id,requester,_SID_INVALID);
	GLinkClient::GetInstance()->SendProtocol(msg);
	return 0;
}
int TestRaidEnter::EnterRaid(int raidroom_id,int mapid)
{
	RaidEnter msg(GLinkClient::GetInstance()->roleid,mapid,raidroom_id,_SID_INVALID);
	GLinkClient::GetInstance()->SendProtocol(msg);
	return 0;
}

int TestRaidEnter::ChangeGS(int dstline)
{
	DoChangeGS msg(GLinkClient::GetInstance()->roleid,dstline,_SID_INVALID);
	GLinkClient::GetInstance()->SendProtocol(msg);
	return 0;
}




};
