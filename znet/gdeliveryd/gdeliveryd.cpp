#include "gdeliveryserver.hpp"
#include "gauthclient.hpp"
#include "gproviderserver.hpp"
#include "gamedbclient.hpp"
#include "groledbclient.hpp"
#include "ganticheatclient.hpp"
#include "uniquenameclient.hpp"
#include "conf.h"
#include "log.h"
#include "thread.h"
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "itimer.h"
#include "deleteroletask.h"
#include "matcher.h"
#include "mapforbid.h"
#include "mapuser.h"
#include "battlemanager.h"
#include "auctionmarket.h"
#include "serverattr.h"
#include "factionmanager.h"
#include "familymanager.h"
#include "siegemanager.h"
#include "stockexchange.h"
#include "sectmanager.h"
#include "contestmanager.h"
#include "instancingmanager.h"
#include "xmlversion.h"
#include "commondata.h"
#include "commondataauto.h"
#include "snsplayerinfomanager.h"
#include "snsmessagemanager.h"
#include "referencemanager.h"
#include "billingagent.h"
#include "fungamemanager.h"
#include "hometownmanager.h"
#include "farmcompomanager.h"
#include "netmarble.h"
#include "territorymanager.h"
#include "uniqueauctionmanager.h"
#include "circlemanager.h"
#include "consignmanager.h"
#include "topflowermanager.h"
#include "centraldeliveryserver.hpp"
#include "centraldeliveryclient.hpp"
#include "friendcallbackmanager.hpp"
#include "gamegateserver.hpp"
#include "globaldropmanager.h"
#include "raidmanager.h"
#include "kingdommanager.h"
#include "vipinfocache.hpp"
#include "blockedrole.h"
#include "crossbattlemanager.hpp"
#include "openbanquetmanager.h"
#include "iwebcmd.h"
#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"

using namespace GNET;

void usage(char* name)
{
	printf ("Usage: %s [-v] [-h] [configurefile]\n", name);
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
		LOG_TRACE(COMPILERNAME);
		LOG_TRACE("compile time: %s:%s", __DATE__, __TIME__);
		usage(argv[0]);
		exit(-1);
	}
	LOG_TRACE(COMPILERNAME);
	LOG_TRACE("compile time: %s:%s", __DATE__, __TIME__);
	GDeliveryServer *manager = GDeliveryServer::GetInstance();
	{
		time_t now = Timer::GetTime();
		struct tm dt;
		localtime_r(&now, &dt);
		manager->zone_off = dt.tm_gmtoff;
	}
	manager->conf_file = argv[optind];
	Conf *conf = Conf::GetInstance(manager->conf_file.c_str());
	int ret = Matcher::GetInstance()->Load(const_cast<char*>(conf->find(manager->Identification(),"table_name").c_str()),
			"UCS2", conf->find(manager->Identification(),"name_charset").c_str(),
			conf->find(manager->Identification(), "table_charset").c_str());
	if(ret)
	{
		std::cerr<<"Cannot load table of sensitive words. check file rolename.txt."<<std::endl;
		exit(-1);
	}
	ret = CommonDataAuto::Instance()->LoadPolicy(conf->find(manager->Identification(), "cmndata_crontab").c_str());
	if (ret)
	{       
		std::cerr<<"Cannot load common data automation crontab. check file commondataauto.txt."<<std::endl;
	}   
	std::string central_ds = conf->find(manager->Identification(), "is_central_ds");
	if (central_ds == "true")
		manager->SetCentralDS(true);
	bool is_central = manager->IsCentralDS();

	std::string support_token_login = conf->find(manager->Identification(), "support_token_login");
	if(support_token_login == "true")
		manager->SetSupportTokenLogin(true);

	IntervalTimer::StartTimer(500000);
	if(!is_central && !FactionManager::Instance()->Initialize())
		exit(-1);
	if(!is_central && !FamilyManager::Instance()->Initialize())
		exit(-1);
	if(!is_central && !StockExchange::Instance()->Initialize())
		exit(-1);
	if(!CommonDataMan::Instance()->Initialize())
		exit(-1);
	if(!is_central && !SiegeManager::GetInstance()->Initialize())
		exit(-1);
	if(!SNSPlayerInfoManager::GetInstance()->Initialize())
		exit(-1);
	if(!SNSMessageManager::GetInstance()->Initialize())
		exit(-1);
	if(!is_central && !ReferenceManager::GetInstance()->Initialize())
		exit(-1);
//	if(!is_central && !CircleManager::Instance()->Initialize())
//		exit(-1);
	if (!is_central)
		SectManager::Instance()->Initialize();
	RoleInfoCache::Instance().Initialize();
	Log::setprogname("gdeliveryd");
	{

		std::string server_version = conf->find(manager->Identification(), "server_version");
		if (!server_version.length())
			manager->challenge_algo=0;
		else if (server_version=="korea")		//韩国版本开启明文密码和韩国网吧系统
		{
			/* if (!NetMarble::GetInstance()->Initialize())
			{
				Log::log(LOG_ERR,"Fatal: Initialize NetMarble failed.");
				exit(EXIT_FAILURE);
			} */
			manager->challenge_algo=ALGO_PLAINTEXT;
		}

		// 韩国网吧系统
		int netmarble_update_interval = atoi(conf->find(manager->Identification(), "netmarble_interval").c_str());
		std::string netmarble = conf->find(manager->Identification(), "korea_netmarble");
		if (netmarble == "true") {
			if (!is_central && !NetMarble::GetInstance()->Initialize(netmarble_update_interval)) {
				Log::log(LOG_ERR,"Fatal: Initialize NetMarble failed.");
				exit(EXIT_FAILURE);
			}
		}
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		manager->zoneid=atoi(conf->find(manager->Identification(), "zoneid").c_str());
		manager->aid = 11;
		manager->district_id=atoi(conf->find(manager->Identification(), "district_id").c_str());
		std::cerr<<"District id is "<<manager->district_id << std::endl;
		int max_player_num=atoi(conf->find(manager->Identification(), "max_player_num").c_str());
		if(!max_player_num)
			max_player_num = MAX_PLAYER_NUM_DEFAULT;
		UserContainer::GetInstance().SetPlayerLimit( max_player_num,max_player_num );
		DEBUG_PRINT("gdeliveryd::Max player allowed is %d\n",UserContainer::GetInstance().GetPlayerLimit());
		if (0==manager->zoneid) 
		{
			std::cerr<<"Invalid zone id(0). Check .conf file."<<std::endl;
			exit(-1);
		}
		int debug = atoi(conf->find(manager->Identification(), "debugmode").c_str());
		if(debug)                       
			manager->SetDebugmode(debug);  
		std::cout<<"zoneid="<<(int)manager->zoneid<<"  aid="<<(int)manager->aid<<std::endl;
		Protocol::Server(manager);

	}
	if (!is_central)
	{
		GAuthClient *manager = GAuthClient::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		std::string au=conf->find(manager->Identification(),"shared_key");
		manager->shared_key=Octets(au.c_str(),au.size());
		Protocol::Client(manager);
	}
	{
		GProviderServer *manager = GProviderServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		manager->SetProviderServerID(atoi(conf->find(manager->Identification(), "id").c_str()));
		Protocol::Server(manager);
	}
	{
		GameDBClient *manager = GameDBClient::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Client(manager);
	}
	{
		GAntiCheatClient *manager = GAntiCheatClient::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Client(manager);
	}
	std::string gate_is_open = conf->find(manager->Identification(), "gate_open");
	if(gate_is_open == "true")
	{
		GameGateServer *manager = GameGateServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Server(manager);
	}
	std::string gt_is_open = conf->find(manager->Identification(), "gt_open");
	if(!is_central && gt_is_open == "true" && gate_is_open == "true")
	{
		GameGateServer::OpenGT();
		GTPlatformAgent::OpenGT();
	}
	if (!is_central)
	{
		UniqueNameClient* manager=UniqueNameClient::GetInstance();
		Protocol::Client(manager);
	}
	int battle = atoi(conf->find(GDeliveryServer::GetInstance()->Identification(), "battlefield").c_str());
	if (battle && !BattleManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize Battle Field failed.");
   		exit(EXIT_FAILURE);
	}
	int instancing = atoi(conf->find(GDeliveryServer::GetInstance()->Identification(), "instancingfield").c_str());
	if (instancing && !InstancingManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize Instancing Field failed.");
   		exit(EXIT_FAILURE);
	}

	std::string qsfile = conf->find(GDeliveryServer::GetInstance()->Identification(),"contest_file");
	if(qsfile.size())
	{
		if (!ContestManager::GetInstance().Initialize(qsfile) )
		{
			Log::log(LOG_ERR,"Fatal: Initialize contest data failed.");
			exit(EXIT_FAILURE);
		}
	}
	
	std::string circle_is_open = conf->find(manager->Identification(), "circle_open");
	if(!is_central && circle_is_open == "true")
	{
		CircleManager::Instance()->Initialize();
		CircleManager::Instance()->OpenCircle();
	}
	if (!is_central)
		BillingAgent::Instance().Initialize();

	std::string consign_is_open = conf->find(manager->Identification(), "consign_open");
	std::string consignrole_is_open = conf->find(manager->Identification(), "consignrole_open");
	if(!is_central && consign_is_open == "true")
	{
		if(!ConsignManager::GetInstance()->Initialize())//must be put after gdeliveryserver init
			exit(-1);
		ConsignManager::GetInstance()->OpenConsign();
		if(consignrole_is_open  == "true")
			ConsignManager::GetInstance()->OpenConsignRole();
		RoleTaskManager::GetInstance()->Initialize();
                GWebTradeClient *manager = GWebTradeClient::GetInstance();
                manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
	 	Protocol::Client(manager);
	}

	std::string callback_is_open = conf->find(manager->Identification(), "callback_open");
	if(!is_central && "true" == callback_is_open)
	{
		FriendCallbackManager::GetInstance()->Initialize();
	}

	std::string vip_is_open = conf->find(manager->Identification(), "vip_open");
	if(vip_is_open != "false")
	{
		VIPInfoCache::GetInstance()->Initialize();
	}
	Coupon::GetInstance()->Init();

	RaidManager::GetInstance()->Initialize();
		
	FunGameManager::GetInstance().Init();

	if (!HometownManager::GetInstance()->Init())
	{
		Log::log(LOG_ERR, "Fatal: Initialize Hometown failed\n");
   		exit(EXIT_FAILURE);
	}

	if(!is_central && !TerritoryManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize territorymanager failed.");
		exit(-1);
	}
	if(is_central && !OpenBanquetManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize openbanquetmanager failed.");
		exit(-1);
	}
	if(!is_central && !KingdomManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize kingdommanager failed.");
		exit(-1);
	}
	if( !GlobalDropManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize GlobalDropManager failed.");
		exit(-1);
	}
/*	if( !TopFlowerManager::Instance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize TopFlowerManager failed.");
		exit(-1);
	}
*/	

	if( !is_central && !CrssvrTeamsManager::Instance()->Initialize() )
	{
		Log::log(LOG_ERR,"Fatal: Initialize CrssvrTeamsManager failed.");
		exit(-1);
	}

	if( is_central && !CrossCrssvrTeamsManager::Instance()->Initialize() )
	{
		Log::log(LOG_ERR,"Fatal: Initialize CrossCrssvrTeamsManager failed.");
		exit(-1);
	}

	if( !UniqueAuctionManager::GetInstance()->Initialize())
	{
		Log::log(LOG_ERR,"Fatal: Initialize uniqueauctionmanager failed.");
		exit(-1);
	}
	CrossBattleManager::GetInstance()->Initialize();
	if (is_central)
	{
		LOG_TRACE("Central Delivery Server start to listen...");
		CentralDeliveryServer * cds = CentralDeliveryServer::GetInstance();
		cds->SetDebug(atoi(conf->find(cds->Identification(), "isdebug").c_str()));
		cds->SetAccumulate(atoi(conf->find(cds->Identification(), "accumulate").c_str()));
		Protocol::Server(cds);
		Thread::Pool::AddTask(new LoadExchangeTask(15));
	}
	else
	{
		LOG_TRACE("Central Delivery Client start to connect...");
		CentralDeliveryClient * cdc = CentralDeliveryClient::GetInstance();
		Timer::Attach(cdc);
		cdc->SetAccumulate(atoi(conf->find(cdc->Identification(), "accumulate").c_str()));
		Protocol::Client(cdc);
	}

	manager->SetDebug(atoi(conf->find(manager->Identification(), "isdebug").c_str()));
	int max_kicktime = atoi( conf->find(manager->Identification(), "studio_user_max_kicktime").c_str());//工作室账号最长多久被踢下线
	if (max_kicktime > 0)
		ForbidStudioUser::GetInstance(max_kicktime);

	std::string forbid_role_login = conf->find(manager->Identification(), "forbid_role_login");
	if(forbid_role_login == "true")
	{
		ForbidLogin::GetInstance().SetAllowRoleLoginGlobal(false);
	}
	IwebCmd::GetInstance()->Initialize();

	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	
	Thread::HouseKeeper::AddTimerTask(new RemoveForbidden(),MAX_REMOVETIME/2);

	int interval = 0;
	if (!is_central)
	{
		interval=atoi( conf->find("Intervals","delrole_interval").c_str() );
		interval=interval<60 ? 60 : interval;
		Thread::HouseKeeper::AddTimerTask(DeleteRoleTask::GetInstance(interval/4),interval/2/* delay */);
	}
	
	interval=atoi( conf->find("Intervals","checkforbidlogin_interval").c_str() );
	interval=interval<60 ? 60 : interval;
	Thread::HouseKeeper::AddTimerTask(CheckTimer::GetInstance(interval),interval/* delay */);
	Thread::HouseKeeper::AddTimerTask(LinePlayerTimer::GetInstance(3),0);
	Thread::HouseKeeper::AddTimerTask(CircleCleaner::GetInstance(0),CircleCleaner::GetInstance()->calc_seconds_to_time(0));/* execute time of day, delay */
	Thread::Pool::Run();
	return 0;
}

