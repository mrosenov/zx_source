
#include "raid_controller.h"
#include "player_imp.h"
#include <meminfo.h>
#include <conf.h>

raid_world_template::~raid_world_template()
{
	if(pathman)
	{
		delete pathman;
		pathman = 0;
	}

	if(movemap)
	{
		delete movemap;
		movemap = 0;
	}
	if(region)
	{
		delete region;
		region = 0;
	}
	if(barea)
	{
		delete barea;
		barea = 0;
	}
	traceman.Release();
}

bool raid_world_template::Init(const char * sName, int raid_id)
{
	servername = sName;
	rwinfo.raid_id = raid_id;
	ONET::Conf *conf = ONET::Conf::GetInstance();
	std::string root = conf->find("Template","Root");
	ONET::Conf::section_type section = "World_";
	section += servername;
	std::string base_path = root + conf->find(section,"base_path");

	std::string trace_file = conf->find("Template","CollisionFile");
	if(strlen(trace_file.c_str()) && ( GetMemTotal() >3*1024*1024 ||player_template::GetDebugMode()) )   //$$$$$$$ 这个是否应该用参数控制 
	{
		std::string trace_path = base_path + trace_file;
		if(!traceman.Load(trace_path.c_str()))
		{
			printf("加载凸包数据'%s'失败\n",trace_path.c_str());
			return false;
		}
		else
		{
			
			printf("加载'%s'完成\n",trace_path.c_str());
		}
	}

	std::string pathfile = base_path + conf->find("Template","PathFile");
	pathman = new path_manager;
	if(!pathman->Init(pathfile.c_str()))
	{
		printf("无法打开路线文件 %s 失败\n", pathfile.c_str());
		return false;
	}

	std::string  regionfile = base_path + conf->find("Template","RegionFile");
	std::string  regionfile2 = base_path + conf->find("Template","RegionFile2");

	region = new city_region;
	//副本的city_region使用的worldtag为raid_id, 每一个副本一个
	region->SetWorldTag(rwinfo.raid_id);
	if(!(region->InitRegionData(regionfile.c_str(),regionfile2.c_str())))
	{
		printf("初始化副本城市区域失败 %s\n", regionfile.c_str());
		return false;
	}

	std::string ground = base_path + conf->find("MoveMap","Path");
	std::string water = base_path + conf->find("MoveMap","WaterPath");
	std::string air = base_path + conf->find("MoveMap","AirPath");
	movemap = path_finding::InitMoveMap(ground.c_str(), water.c_str(), air.c_str());
	if(!movemap)
	{
		__PRINTF("无法读入副本NPC通路图或者无法读入水域图㘎n");
		return false;
	}

	ONET::Conf::section_type tsection = "Terrain_";
	tsection += servername;	

	TERRAINCONFIG config;
	config.nAreaWidth = atoi(conf->find(tsection,"nAreaWidth").c_str());
	config.nAreaHeight = atoi(conf->find(tsection,"nAreaHeight").c_str());
	config.nNumAreas = atoi(conf->find(tsection,"nNumAreas").c_str());
	config.nNumCols = atoi(conf->find(tsection,"nNumCols").c_str());
	config.nNumRows =atoi(conf->find(tsection,"nNumRows").c_str());
	config.vGridSize = atof(conf->find(tsection,"vGridSize").c_str());
	config.vHeightMin = atof(conf->find(tsection,"vHeightMin").c_str());
	config.vHeightMax = atof(conf->find(tsection,"vHeightMax").c_str());
	memset(config.szMapPath,0,sizeof(config.szMapPath));
	std::string path = base_path + conf->find(tsection,"szMapPath");
	strncpy(config.szMapPath, path.c_str(),sizeof(config.szMapPath) - 1);

	rect rt;
	sscanf(conf->find(section, "local_region").c_str(),"{%f,%f} , {%f,%f}",&rt.left,&rt.top,&rt.right,&rt.bottom);

	ASSERT(rt.left <= rt.right && rt.top <= rt.bottom);
	if(!path_finding::InitTerrain(movemap, config,rt.left,rt.top,rt.right,rt.bottom))
	{
		printf("无法初始化地形数据\n");
		return false;
	}
	printf("地形数据已经加载\n");

	barea = new buff_area;
	if(!barea->Init(rt.left, rt.top, rt.right, rt.bottom, 1.0f, region))
	{
		printf("无法初始化BUFF区域\n");
		return false;
	}
	printf("地图buff区域数据已经加载\n");

	level_config_tid = atoi(conf->find(section, "level_config_tid").c_str());
	return true;
}


raid_controller::~raid_controller()
{	
	RaidWorldInfoMap::iterator it = rwtMap.begin();
	for(; it != rwtMap.end(); ++ it)
	{
		if(it->second)
		{
			delete it->second;
			it->second = 0;
		}
	}
	rwtMap.clear();
}

bool raid_controller::AddRaidWorldTemplate(raid_world_template* t)
{
	RaidWorldInfoMap::iterator it = rwtMap.find(t->rwinfo.raid_id);
	if(it != rwtMap.end()) return false;
	rwtMap[t->rwinfo.raid_id] = t;
	return true;
}


raid_world_template* raid_controller::GetRaidWorldTemplate(int raid_id) const
{
	RaidWorldInfoMap::const_iterator it = rwtMap.find(raid_id);
	if(it != rwtMap.end())
	{
		return it->second;
	}
	return NULL;
}

const raid_controller::RaidWorldInfoMap& raid_controller::GetRaidWorldInfoMap() const
{
	return rwtMap;
}

void raid_controller::GetRaidWorldInfos(std::vector<raid_world_info>& infos)
{
	RaidWorldInfoMap::iterator  it = rwtMap.begin();
	for(; it != rwtMap.end(); ++ it)
	{
		raid_world_template* rwt = it->second;
		infos.push_back(rwt->rwinfo);
	}
}

void raid_controller::IncRaidWorldInstance(int raid_id)
{
	spin_autolock keeper(rwtmap_lock);
	RaidWorldInfoMap::iterator it = rwtMap.find(raid_id);
	if(it != rwtMap.end())
	{
		raid_world_template* rwt = it->second;
		rwt->cur_instance_num ++;
	}
}

void raid_controller::DecRaidWorldInstance(int raid_id)
{
	spin_autolock keeper(rwtmap_lock);
	RaidWorldInfoMap::iterator it = rwtMap.find(raid_id);
	if(it != rwtMap.end())
	{
		raid_world_template* rwt = it->second;
		rwt->cur_instance_num --;
	}
}
