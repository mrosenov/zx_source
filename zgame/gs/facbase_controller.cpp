#include "facbase_controller.h"
#include "player_imp.h"
#include <meminfo.h>
#include <conf.h>

facbase_controller::~facbase_controller()
{
	if(pathman)
	{
		delete pathman;
		pathman = NULL;
	}
	if(movemap)
	{
		delete movemap;
		movemap = NULL;
	}
	if(region)
	{
		delete region;
		region = NULL;
	}
	if(barea)
	{
		delete barea;
		barea = NULL;
	}
	traceman.Release();
}

bool facbase_controller::Init(const char * sName, int t)
{
	servername = sName;
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
	region->SetWorldTag(t);
	if(!(region->InitRegionData(regionfile.c_str(),regionfile2.c_str())))
	{
		printf("初始化基地城市区域失败 %s\n", regionfile.c_str());
		return false;
	}

	std::string ground = base_path + conf->find("MoveMap","Path");
	std::string water = base_path + conf->find("MoveMap","WaterPath");
	std::string air = base_path + conf->find("MoveMap","AirPath");
	movemap = path_finding::InitMoveMap(ground.c_str(), water.c_str(), air.c_str());
	if(!movemap)
	{
		__PRINTF("无法读入基地NPC通路图或者无法读入水域图㘎n");
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

	//加载基地相关模板
	DATA_TYPE dt;
	BUILDING_REGION_CONFIG * base_cfg = (BUILDING_REGION_CONFIG *)gmatrix::GetDataMan().get_data_ptr(gmatrix::GetFacBaseInfo().region_tid, ID_SPACE_CONFIG, dt);
	if (dt != DT_BUILDING_REGION_CONFIG || !base_cfg)
	{
		printf("无效的基地模板: %d\n", gmatrix::GetFacBaseInfo().region_tid);
		return false;
	}
	for (unsigned int i = 0; i < sizeof(base_cfg->build_region)/sizeof(base_cfg->build_region[0]); i++)
	{
		if (base_cfg->build_region[i].region_index > 0)
		{
			REGION_BLOCK_ESSENCE * block_cfg = (REGION_BLOCK_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(base_cfg->build_region[i].region_id, ID_SPACE_ESSENCE, dt);
			if (dt != DT_REGION_BLOCK_ESSENCE || !block_cfg)
			{
				printf("无效的基地地块模板: %d\n", base_cfg->build_region[i].region_id);
				return false;
			}
			std::set<int> tids;
			for (unsigned int j = 0; j < sizeof(block_cfg->building)/sizeof(block_cfg->building[0]); j++)
			{
				if (block_cfg->building[j] > 0)
					tids.insert(block_cfg->building[j]);
			}
			if (tids.empty())
			{
				printf("无效的基地地块模板: %d 不能建设任何建筑\n", base_cfg->build_region[i].region_id);
				return false;
			}
			fbase_field_cfg cfg;
			cfg.avail_buildings.swap(tids);
//			if (field_cfg.insert(abase::pair(base_cfg->build_region[i].region_index, cfg)).second == false)
			if (field_cfg.find(base_cfg->build_region[i].region_index) != field_cfg.end())
			{
				printf("无效的基地地块模板: %d 地块编号 %d 重复", base_cfg->build_region[i].region_id, base_cfg->build_region[i].region_index);
				return false;
			}
			__PRINTF("加载地块模板 index %d\n", base_cfg->build_region[i].region_index);
			field_cfg[base_cfg->build_region[i].region_index] = cfg;
		}
	}
	if (field_cfg.empty())
	{
		printf("无效的基地模板: %d 没有任何有效地块配置?\n", gmatrix::GetFacBaseInfo().region_tid);
		return false;
	}
	//基地传送配置
	FACTION_TRANSFER_CONFIG * trans_cfg = (FACTION_TRANSFER_CONFIG *)gmatrix::GetDataMan().get_data_ptr(gmatrix::GetFacBaseInfo().region_transfer_tid, ID_SPACE_CONFIG, dt);
	if (dt != DT_FACTION_TRANSFER_CONFIG || !trans_cfg)
	{
		printf("无效的基地传送模板: %d\n", gmatrix::GetFacBaseInfo().region_transfer_tid);
		return false;
	}
	for (unsigned int i = 0; i < sizeof(trans_cfg->transfer_location)/sizeof(trans_cfg->transfer_location[0]); i++)
	{
		if (trans_cfg->transfer_location[i].region_index > 0)
			transfer_cfg[trans_cfg->transfer_location[i].region_index] = A3DVECTOR(trans_cfg->transfer_location[i].x, trans_cfg->transfer_location[i].y, trans_cfg->transfer_location[i].z);
	}
	if (transfer_cfg.empty())
	{
		printf("无效的基地传送模板: %d 没有任何有效地块配置?\n", gmatrix::GetFacBaseInfo().region_transfer_tid);
		return false;
	}
	return true;
}

fbase_field_cfg * facbase_controller::GetFieldCfg(int index)
{
	FieldConfig::iterator it = field_cfg.find(index);
	if (it == field_cfg.end())
		return NULL;
	return &(it->second);
}

bool facbase_controller::GetTransferPos(int index, A3DVECTOR & pos)
{
	TransferConfig::const_iterator it = transfer_cfg.find(index);
	if (it == transfer_cfg.end())
		return false;
	pos = it->second;
	return true;
}
