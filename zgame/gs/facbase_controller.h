#ifndef _FACBASE_CONTROLLER_
#define _FACBASE_CONTROLLER_

#include <hashmap.h>
#include <string>
#include <vector>

#include "pathfinding/pathfinding.h"
#include "traceman.h"
#include "template/pathman.h"
#include "template/city_region.h"
#include "buffarea.h"
#include "staticmap.h"
#include <set>

//基地地图实例所共享的资源
struct fbase_field_cfg
{
	std::set<int/*building_tid*/> avail_buildings;
	bool AvailBuilding(int tid)
	{
		return avail_buildings.find(tid) != avail_buildings.end();
	}
};
class facbase_controller
{
public:
	enum
	{
		FIELD_CAPACITY = 30, //目前地块最大值是 30 如果需要 此处可扩展
		TRANSFER_REGION_CAPACITY = FIELD_CAPACITY,
	};
private:
//	int tag; //地图id
	std::string servername;

	trace_manager traceman;
	path_manager* pathman;
	path_finding::PathfindMap* movemap;
	city_region* region;
	buff_area*   barea;

	typedef abase::static_map<int/*index*/, fbase_field_cfg, FIELD_CAPACITY> FieldConfig;
	FieldConfig field_cfg;
	typedef abase::static_map<int/*region_index*/, A3DVECTOR, TRANSFER_REGION_CAPACITY> TransferConfig;
	TransferConfig transfer_cfg;
public:
	facbase_controller()
	{
//		tag = 0;

		pathman = NULL;
		movemap = NULL;
		region = NULL;
		barea = NULL;
	}
	~facbase_controller();
	bool Init(const char * sName, int t);

	trace_manager& GetTraceMan() { return traceman; }
	path_finding::PathfindMap* GetMoveMap() { return movemap; }
	path_manager* GetPathMan() { return pathman; }
	city_region* GetCityRegion() { return region; }
	buff_area* GetBuffArea() { return barea; }

	fbase_field_cfg * GetFieldCfg(int index);
	bool GetTransferPos(int index, A3DVECTOR & pos);
};

#endif /*_RAID_CONTROLLER_*/
