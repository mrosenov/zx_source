#ifndef __GNET_TEST_RAID_ENTER_H
#define __GNET_TEST_RAID_ENTER_H
namespace GNET
{
class TestRaidEnter
{
public:
static int RaidAppointRole(int masterid,int raidroom_id,int requester);
static int RaidAccept(int masterid,int map_id,int raidroom_id,int requester);
static int StartRaid(int raidroom_id,int mapid);

static int EnterRaid(int raidroom_id,int mapid);

static int ChangeGS(int dstline);

};

};
#endif

