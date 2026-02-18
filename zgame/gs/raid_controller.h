#ifndef _RAID_CONTROLLER_
#define _RAID_CONTROLLER_

#include <hashmap.h>
#include <string>
#include <vector>

#include "pathfinding/pathfinding.h"
#include "traceman.h"
#include "template/pathman.h"
#include "template/city_region.h"
#include "buffarea.h"

//-------------------------------------------------------------------
// IMPORTANT, gamed下factionlib.cpp文件
// SendRaidServerRegister函数也包含raid_world_info, 修改请保持一致
//-------------------------------------------------------------------
struct raid_world_info
{
	int raid_id;					//副本id, 也就是地图ID
	int raid_template_id;
	int raid_type;					//副本类型，暂时不填
	int queuing_time;				//房间存在时间
	int raid_max_instance;			//最大实例数量
	struct 
	{
		int max_player_limit;		//玩家上限
		int min_start_player_num;	//副本开始玩家下线
	} group_limit;
};

class raid_world_template
{
public:
	raid_world_info rwinfo;
	std::string servername;
	int cur_instance_num;
	int level_config_tid;			//关卡类副本条件检测模板

protected:
	trace_manager traceman;
	path_manager* pathman;
	path_finding::PathfindMap* movemap;
	city_region* region;
	buff_area*   barea;


public:
	raid_world_template() : pathman(0), movemap(0), region(0), barea(0)
	{
		memset(&rwinfo, 0, sizeof(rwinfo));
		rwinfo.raid_id = -1;
		cur_instance_num = 0;		//当前GS上已经有的副本实例数量
	}
	~raid_world_template();
	
	raid_world_template(const raid_world_template& rwt) 
	{
		memcpy(&rwinfo, &rwt.rwinfo, sizeof(rwinfo));
		servername = rwt.servername;	
		cur_instance_num = rwt.cur_instance_num;
	}

	raid_world_template& operator=(const raid_world_template& rwt)
	{
		memcpy(&rwinfo, &rwt.rwinfo, sizeof(rwinfo));
		servername = rwt.servername;	
		cur_instance_num = rwt.cur_instance_num;
		return *this;
	}

	bool Init(const char* sName, int raid_id);

	trace_manager& GetTraceMan() { return traceman; }

	path_finding::PathfindMap* GetMoveMap() { return movemap; }

	path_manager* GetPathMan() { return pathman; }

	city_region* GetCityRegion() { return region; }

	buff_area* GetBuffArea() { return barea; }

};

class raid_controller
{
public:
	typedef abase::hash_map<int, raid_world_template*> RaidWorldInfoMap;

private:
	int rwtmap_lock;
	RaidWorldInfoMap rwtMap;

public:
	raid_controller() : rwtmap_lock(0){}
	~raid_controller();

	bool AddRaidWorldTemplate(raid_world_template* t);
	raid_world_template* GetRaidWorldTemplate(int raid_id) const;	
	const RaidWorldInfoMap& GetRaidWorldInfoMap() const;
	void GetRaidWorldInfos(std::vector<raid_world_info>& infos);
	void IncRaidWorldInstance(int raid_id);
	void DecRaidWorldInstance(int raid_id);
};

#endif /*_RAID_CONTROLLER_*/
