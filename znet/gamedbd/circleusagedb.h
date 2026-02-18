#ifndef __GNET_GCIRCLEUSAGEDB_H
#define __GNET_GCIRCLEUSAGEDB_H

//#include "circlemanager.h"
#include "gcirclehistorydb"
#include "gcirclehistory"
//#include "gcirclechat.hpp"
namespace GNET
{
	
//namespace CircleUsage{

//void CircleChat(GCircleChat & chat);
void GCircleHis_to_GCircleHisDB(GCircleHistory& gc, GCircleHistoryDB& gcdb)
{
	gcdb.circleid = gc.circleid;
	gcdb.circlename = gc.circlename;
	gcdb.titlemask = gc.titlemask;
	gcdb.jointime = gc.jointime;
	gcdb.graduate_time = gc.graduate_time;
	gcdb.circlegrade = gc.circlegrade;
}
void GCircleHisDB_to_GCircleHis(GCircleHistoryDB& gcdb, GCircleHistory& gc)
{
	gc.circleid = gcdb.circleid;
	gc.circlename = gcdb.circlename;
	gc.titlemask = gcdb.titlemask;
	gc.jointime = gcdb.jointime;
	gc.graduate_time = gcdb.graduate_time;
	gc.circlegrade = gcdb.circlegrade;
}
//};//namespace CircleUsage

};

#endif
