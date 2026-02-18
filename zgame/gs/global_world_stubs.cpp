#include "global_manager.h"
#include "instance/instance_manager.h"
#include "instance/battleground_manager.h"
#include <strtok.h>
#include "gmatrix.h"
#include "item/item_addon.h"
#include <conf.h>

//static global_world_manager *gwm = NULL;
//static instance_world_manager * iwm = NULL;

namespace {
enum 
{
	INIT_ERROR = -1,
	INIT_BIG_WORLD,
	INIT_INSTANCE,
	INIT_FACTION,
	INIT_BATTLEGROUND,
};
};

int InitWorld(const char * conf_file, const char * gmconf_file, const char *alias_file)
{
	ONET::Conf::GetInstance(conf_file);
	ONET::Conf::AppendConfFile(alias_file);
	ONET::Conf *conf = ONET::Conf::GetInstance();
	
	//conf->dump(stdout);
	
	abase::vector<char *> ins_list;
	abase::vector<char *> world_list;
	abase::vector<char *> zone_list;
	
	std::string servers = conf->find("General","instance_servers");
	abase::strtok tok(servers.c_str(),";,\r\n");
	const char * token;
	while((token = tok.token()))
	{       
		if(!*token) continue;
		char * name = new char[strlen(token) + 1];
		strcpy(name, token);
		ins_list.push_back(name);
	}
	ins_list.push_back(NULL);

	servers = conf->find("General","world_servers").c_str();
	abase::strtok tok2(servers.c_str(),";,\r\n");
	while((token = tok2.token()))
	{       
		if(!*token) continue;
		char * name = new char[strlen(token) + 1];
		strcpy(name, token);
		world_list.push_back(name);
	}
	
	servers = conf->find("General","zone_servers").c_str();
	abase::strtok tok3(servers.c_str(),";,\r\n");
	while((token = tok3.token()))
	{       
		if(!*token) continue;
		char * name = new char[strlen(token) + 1];
		strcpy(name, token);
		zone_list.push_back(name);
	}


	world_list.push_back(NULL);
	zone_list.push_back(NULL);
	InitAllAddon();

	int rst =  gmatrix::Instance()->Init(gmconf_file, world_list.begin(), ins_list.begin(), zone_list.begin());
	abase::clear_ptr_vector(world_list);
	abase::clear_ptr_vector(ins_list);
	abase::clear_ptr_vector(zone_list);
	return rst;
	/*
	switch (is_instance)
	{
		case INIT_BIG_WORLD:
			printf("开始世界逻辑初始化...\n");
//			gwm = new global_world_manager();
//			return gwm->Init(gmconf_file,servername);
			gmatrix::Instance()->Load(gmconf_file, servername);
			return 0;
		case INIT_INSTANCE:
			printf("开始副本逻辑初始化...\n");
			iwm = new instance_world_manager();
			return iwm->Init(gmconf_file,servername);
		case INIT_FACTION:
			printf("开始帮派基地初始化...\n");
			iwm = new faction_world_manager();
			return iwm->Init(gmconf_file,servername);
		case INIT_BATTLEGROUND:
			printf("开始战场初始化...\n");
			iwm = new battleground_world_manager();
			return iwm->Init(gmconf_file,servername);
		default:
			printf("错误：在配置文件'%s'中没有找到正确的条目'%s'\n",conf_file,servername);
			return -2003;
	}*/
}

