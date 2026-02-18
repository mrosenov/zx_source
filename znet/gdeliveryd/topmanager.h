#ifndef __GNET_TOPMANAGER_H
#define __GNET_TOPMANAGER_H

#include <vector>
#include <map>

#include "gtopitem"
#include "gtoptable"
#include "localmacro.h"

namespace GNET
{

extern int TopTableID[];
class GetTopTable_Re;

enum STATUS{
	TOP_STATUS_READY = 1,
};

class TopManager
{
	typedef std::map<int,GTopTable>  Map;
	Map tables;

	TopManager() : /*fac_act_toptime(0),*/locker("topmanager::locker_top"),id_load(0),status(0){}  
//	int fac_act_toptime; //faction活跃度周排行榜的排榜时间
public:
	Thread::RWLock locker;
	int id_load;
	int status;

	~TopManager() {}

	static TopManager* Instance() { static TopManager instance; return &instance;}
	bool Initialize();
	void GetDailyTable(int selfid, GetTopTable_Re& re);
	int  GetDailyTable(int tableid, GTopTable& ret);
	int  GetWeeklyTable(int index, GTopTable& ret);
	int  GetWeeklyTableByTableID(int tableid, GTopTable& ret);
	void OnDBConnect(Protocol::Manager *manager, int sid);
	void UpdateTop(GTopTable& table, int weekly);
	void UpdateTopFromCentral(int table_type, GTopTable& table);
	int  OnTableLoad(GTopTable& daily, GTopTable& weekly );
	bool GetCredit(int index, unsigned int fid, int64_t* credit) const;
	bool IsRankIn(int index, unsigned int fid, unsigned int rank) const;
//	int GetFacActTopTime() { return fac_act_toptime; }
	bool Ready() { return status == TOP_STATUS_READY; }
	void ClientGetWeeklyTable(int tableid, int pageid, int & maxsize, std::vector<GTopItem> & items);
};

};
#endif

