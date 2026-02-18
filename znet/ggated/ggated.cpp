#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sstream>
#include "xmlversion.h"
#include "conf.h"
#include "log.h"
#include "thread.h"
#include "gtplatformclient.hpp"
#include "snsplatformclient.hpp"
#include "gamedbclient.hpp"
#include "gamegateclient.hpp"
#include "gtmanager.h"
#include "tokengenerator.h"
#include "kdsclient.hpp"
#include "gproviderserver.hpp"

using namespace GNET;

void usage(char* name)
{
	printf ("Usage: %s [-v] [-h] <configurefile>\n", name);
}

int main(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "hv")) != EOF)
	{
		switch(opt)
		{
			case 'v':
				printf("%s\n", XMLVERSION);
				exit(0);
			default:
				usage(argv[0]);
				exit(0);
		}
	}
	if (optind >= argc || access(argv[optind], R_OK) == -1)
	{
		usage(argv[0]);
		exit(-1);
	}
	Conf *conf = Conf::GetInstance(argv[optind]);
	char buf[256];
	snprintf(buf,256,"ggated");
	int global_aid=0;
	int global_zoneid=0;
	Log::setprogname(buf);

	IntervalTimer::StartTimer(1000000);
	{
		GTPlatformClient *manager = GTPlatformClient::GetInstance();
		std::string gt_is_open = conf->find(manager->Identification(), "gt_open");
		if(gt_is_open == "true")
		{	
			manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
			manager->zoneid=atoi(conf->find(manager->Identification(), "zoneid").c_str());
			Log::log( LOG_INFO, "Init GTPlatformClient successfully zoneid=%d.",manager->zoneid );
			manager->aid = atoi(conf->find(manager->Identification(), "aid").c_str());
			Log::log( LOG_INFO, "Init GTPlatformClient successfully.aid=%d" ,manager->aid);
			int faction_limit = atoi(conf->find(manager->Identification(), "faction_limit").c_str());
		       	int user_limit = atoi(conf->find(manager->Identification(), "user_limit").c_str());
			int normal_limit = atoi(conf->find(manager->Identification(), "normal_limit").c_str());
			GTManager::Instance()->SetQueryLimit(faction_limit,user_limit,normal_limit);
			GTManager::Instance()->OpenGT();
		}
	}
	{
		GameDBClient *manager = GameDBClient::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Client(manager);
	}
	{
		GameGateClient *manager = GameGateClient::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Client(manager);
	}
	{
		KdsClient *manager = KdsClient::GetInstance();
		manager->zoneid=atoi(conf->find(manager->Identification(), "zoneid").c_str());
		global_zoneid = manager->zoneid;
		manager->aid = atoi(conf->find(manager->Identification(), "aid").c_str());
		global_aid = manager->aid;
		std::string kcname = conf->find(manager->Identification(), "kc_name");
		int kc_activeid = atoi(conf->find(manager->Identification(), "kc_activeid").c_str());
		std::stringstream share_keyid;
		share_keyid<<kcname<<"-"<<kc_activeid;
		std::string sharekeyid;
		share_keyid >> sharekeyid;
		std::string share_key = conf->find(manager->Identification(), sharekeyid.c_str());
		TokenGenerator::GetInstance().Initialize(share_key,kcname,kc_activeid);
		DEBUG_PRINT("sharekey=%.*s",share_key.size(),share_key.c_str());
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Client(manager);
	}
	{
		GProviderServer * manager = GProviderServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		manager->SetProviderServerID(atoi(conf->find(manager->Identification(), "id").c_str()));
		Protocol::Server(manager);
	}
	{
		SNSPlatformClient * manager = SNSPlatformClient::GetInstance();
		std::string sns_is_open = conf->find(manager->Identification(), "sns_open");
		if(sns_is_open == "true")
		{	
			manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
			manager->zoneid = global_zoneid;
			Log::log( LOG_INFO, "Init SNSPlatformClient successfully zoneid=%d.",manager->zoneid );
			manager->aid = global_aid;
			Log::log( LOG_INFO, "Init SNSPlatformClient successfully.aid=%d" ,manager->aid);
//			int faction_limit = atoi(conf->find(manager->Identification(), "faction_limit").c_str());
//		       	int user_limit = atoi(conf->find(manager->Identification(), "user_limit").c_str());
//			int normal_limit = atoi(conf->find(manager->Identification(), "normal_limit").c_str());
//			GTManager::Instance()->SetQueryLimit(faction_limit,user_limit,normal_limit);
//			GTManager::Instance()->OpenGT();
			Protocol::Client(manager);
		}
	}

	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();
	return 0;
}

