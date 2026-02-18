#include "gamedbserver.hpp"
#include "conf.h"
#include "log.h"
#include "thread.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "gamedbmanager.h"
#include "storage.h"
#include "storagetool.h"

#include "clsconfig.h"
#include "accessdb.h"
#include "waitdelkey"
#include "forbidpolicy.h"
#include "xmlversion.h"
#include "circlepool.h"
#include "sysmailcontainer.h"
#include "xmlcoder.h"
#include "dbconsignsoldrole.hrp"
//#include "dbgettopflowerdata.hrp"
#include "topflowermanager.h"

using namespace GNET;

static char conf_filename[256];
class DbPolicy : public Thread::Pool::Policy
{
public:
	virtual void OnQuit( )
	{
		StorageEnv::checkpoint( );
		StorageEnv::Close();
	}
};

static DbPolicy	s_policy;

// clsconfig db end
///////////////////////////////////////////////////////////////////////////////
void printhelp( const char * cmd )
{
	std::cerr << "Gamedbd version " << XMLVERSION << std::endl << std::endl;
	std::cerr << "Usage: " << cmd << " conf-file" << std::endl
			<< "\t[ importclsconfig | exportclsconfig | clearclsconfig" << std::endl
			<< "\t| importrolelist rolelistfilenamelist| exportrolelist roleidfilename rolelistfilename" << std::endl
			<< "\t| printlogicuid | printunamerole zoneid | printunamefaction zoneid | printunamefamily zoneid" << std::endl
			<< "\t| gennameidx | exportunique zoneid" << std::endl
			<< "\t| exportuser userid" << std::endl
			<< "\t| query roleid | exportrole roleid | merge dbdatapath"
			<< "\t| listrole | listrolebrief [yyyy-mm-dd]| listuserbrief | listfaction | listfamily | listfamilyuser" << std::endl
			<< "\t| listshoplog | listwaitdel | listsyslog" << std::endl
			<< "\t| updateroles | convertdb | repairdb" << std::endl
			<< std::endl
			<< "\t| tablestat | tablestatraw | findmaxsize dumpfilename" << std::endl
			<< "\t| read tablename id" << std::endl
			<< "\t| rewritetable fromname toname | rewritedb" << std::endl
			<< "\t| listid tablename | rewritetable roleidfile fromname toname" << std::endl
			<< "\t| compressdb | decompressdb | verifydb | listterritory]" << std::endl
			<< "\t| equipscan equip_ids_file [level] | equiprolescan [levle] [day]" << std::endl
			<< "\t| compressdb | decompressdb | verifydb | listterritory | abstractroles dbdatapath zoneid new_zoneid" << std::endl
			<< "\t| delfrozenplayers day | allocnewlogicuid useridfile logicuid_begin" << std::endl
			<< "\t| exportcsvuseridexchange | dbhome" << std::endl
			<< "\t| cmpuseridexchange | dbhome" << std::endl
			<< "\t| listrolemoney | listdeityrole | listpkbet | walktables | listbase2" << std::endl
			<< "\t| listroleinventory [roleid] | syncnamechange | verifyname | delzoneplayers zoneid | movedbto unique-dbhome srcdbhome zoneid" << std::endl
			<< "\t| listduprolename | listitemcontainer | listcrssvrteamsrole" << std::endl
			<< "\t| repairflower | rewriteflowerroleid roleidmap | fixcitystore oldcitystore roleidmap factionidmap" << std::endl
			<< "\t| delinactiveusers day" << std::endl
			<< "\t| repairstockcancel | getreferrals roleid | getreferrer roleidfile" << std::endl
			<< "\t| checkabnormalrole" << std::endl;

}

class LogTask : public Thread::Runnable
{
public:

	static LogTask * GetInstance()
	{
		static LogTask t;
		return &t;
	}

	void Run()
	{
		std::string str;
		ForbidPolicy::GetInstance()->DumpPolicy(str);
		LOG_TRACE(str.c_str());
		Log::formatlog("configuration","putpocket=%s", Conf::GetInstance()->find("gamedbd", "allow_modify_inventory").c_str());
	}
};

int main(int argc, char *argv[])
{
	if (argc < 2 || access(argv[1], R_OK) == -1 )
	{
		LOG_TRACE(COMPILERNAME);
		LOG_TRACE("compile time: %s:%s XMLVERSION:%s", __DATE__, __TIME__, XMLVERSION);
		printhelp( argv[0] );
		exit(-1);
	}
	SysMailContainer::Init("sysmailinfo.conf");
	Conf *conf = Conf::GetInstance(argv[1]);
	strcpy(conf_filename,argv[1]);
	Log::setprogname("gamedbd");

	if(!StorageEnv::Open())
	{
		Log::log(LOG_ERR,"Initialize storage environment failed.\n");
		exit(-1);
	}

	if( argc == 3 && 0 == strcmp(argv[2],"importclsconfig") )
	{
		ClearClsConfig();
		ImportClsConfig();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	// Youshuang add
	else if( argc == 4 && 0 == strcmp( argv[2], "repairitemowner" ) )
	{
		int zoneid = atoi( conf->find(GameDBServer::GetInstance()->Identification(), "zoneid").c_str() );
		const char* badfile = argv[ 3 ];
		if( !badfile || access( badfile, R_OK ) != 0 ){ return -1; }

		RepairItemOwner_FromFile( badfile, zoneid );
		
		/*
		std::ifstream ifs( badfile );
		std::string line;
		while( std::getline(ifs, line) )
		{
			if( line.size() == 0 )
			{
				continue;
			}
			std::vector<std::string> subs;
			split( line.c_str(), subs );
			if( subs.size() < 3 ){ continue; }
			int idx = 0;
			int role_in_zone = atoi( subs[idx++].c_str() );
			int old_roleid = atoi( subs[idx++].c_str() );
			int new_roleid = atoi( subs[idx++].c_str() );
			
			if( zoneid != role_in_zone )
			{
				continue;
			}
			RepairItemOwner( old_roleid, new_roleid );
			StorageEnv::checkpoint();
		}
		*/
		
		StorageEnv::Close();
		return 0;
	}
	// end
	else if( argc == 3 && 0 == strcmp(argv[2],"exportclsconfig") )
	{
		ExportClsConfig();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"clearclsconfig") )
	{
		ClearClsConfig();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"printlogicuid") )
	{
		PrintLogicuid( );
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"printunamerole") )
	{
		PrintUnamerole( atoi(argv[3]) );
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"printunamefaction") )
	{
		PrintUnamefaction( atoi(argv[3]) );
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"printunamefamily") )
	{
		PrintUnamefamily( atoi(argv[3]) );
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"gennameidx") )
	{ 
		GenNameIdx();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"exportunique") )
	{ 
		ExportUnique( atoi(argv[3]) );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"query") )
	{
		QueryRole( atoi(argv[3]) );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"exportuser") )
	{
		ExportUser( atoi(argv[3]) );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"exportrole") )
	{
		ExportRole( atoi(argv[3]) );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"merge") )
	{
		StorageEnv::Close();

		SyncNameChange(StorageEnv::get_datadir().c_str());
		SyncNameChange(argv[3]);

		StorageEnv::Open();
		MergeDBAll( argv[3] );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"forcemerge") )
	{
		StorageEnv::Close();

		SyncNameChange(StorageEnv::get_datadir().c_str());
		SyncNameChange(argv[3]);

		StorageEnv::Open();
		MergeDBAll( argv[3], true);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listrole") )
	{
		ListRole();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listbase2") )
	{
		int zoneid=atoi(conf->find(GameDBServer::GetInstance()->Identification(), "zoneid").c_str());
		ListBase2(zoneid);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listitemcontainer") )
	{
		ListItemContainer();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listcrssvrteamsrole") )
	{
		ListCrssvrteamsRole();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( (argc == 3 || argc == 4) && 0 == strcmp(argv[2],"listrolebrief") )
	{
		if (argc == 3)
			ListRoleBrief(NULL);
		else
			ListRoleBrief(argv[3]);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listrolemoney") )
	{
		int zoneid=atoi(conf->find(GameDBServer::GetInstance()->Identification(), "zoneid").c_str());
		ListRoleMoney(zoneid);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listdeityrole") )
	{
		int zoneid=atoi(conf->find(GameDBServer::GetInstance()->Identification(), "zoneid").c_str());
		ListDeityRole(zoneid);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listpkbet") )
	{
		int zoneid=atoi(conf->find(GameDBServer::GetInstance()->Identification(), "zoneid").c_str());
		ListPKBet(zoneid);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( (argc == 3 || argc == 4) && 0 == strcmp(argv[2],"listroleinventory") )
	{
		int roleid = 0;
		if (argc == 4)
			roleid = atoi(argv[3]);
		ListRoleInventory(roleid);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listuserbrief") )
	{
		ListUserBrief();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listfaction") )
	{
		ListFaction();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listfamily") )
	{
		ListFamily();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listfamilyuser") )
	{
		ListFamilyUser();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listshoplog") )
	{
		ListShopLog();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listsyslog") )
	{
		ListSysLog();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"listwaitdel") )
	{
		WaitDelKeyVector rlist;
		GetTimeoutRole(rlist);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"updateroles") )
	{
		UpdateRoles();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"convertdb") )
	{
		ConvertDB();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"walktables") )
	{
		WalkTables();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"repairdb") )
	{
		RepairDB();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"tablestat") )
	{
		TableStat();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"tablestatraw") )
	{
		TableStatRaw();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"findmaxsize") )
	{
		FindMaxsizeValue(argv[3]);
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 5 && 0 == strcmp(argv[2],"read") )
	{
		ReadTable( argv[3], atoi(argv[4]) );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"rewritedb") )
	{
		RewriteDB();
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 5 && 0 == strcmp(argv[2],"rewritetable") )
	{
		RewriteTable( argv[3], argv[4] );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"listid") )
	{
		ListId( argv[3] );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 6 && 0 == strcmp(argv[2],"rewritetable") )
	{
		RewriteTable( argv[3], argv[4], argv[5] );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"compressdb") )
	{
		CompressDB();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 3 && 0 == strcmp(argv[2],"decompressdb") )
	{
		DecompressDB();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 5 && 0 == strcmp(argv[2], "exportrolelist"))
	{
		ExportRoleList(argv[3], argv[4]);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if ( argc >= 4 && 0 == strcmp(argv[2], "importrolelist"))
	{
		ImportRoleList((const char **)&argv[3], argc-3);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
        else if( argc == 3 && 0 == strcmp(argv[2],"verifydb") )
        {
                VerifyDB();
                StorageEnv::checkpoint();
                StorageEnv::Close();
                return 0;
        }
	else if( argc == 3 && 0 == strcmp(argv[2],"listterritory") )
	{
		ListTerritory();
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if(argc >= 4 && 0 == strcmp(argv[2],"equipscan"))
	{
		int level = 1;
		if(argc == 5)
		{
			level = atoi(argv[4]);
		}	
		EquipScan(argv[3], level);
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
        else if( argc == 5 && 0 == strcmp(argv[2],"equiprolescan") )
        {
                EquipRoleScan(atoi(argv[3]), atoi(argv[4]));
                StorageEnv::checkpoint();
                StorageEnv::Close();
                return 0;
        }
	else if( argc == 6 && 0 == strcmp(argv[2],"abstractroles") )
	{
		AbstractPlayers( argv[3], atoi(argv[4]), atoi(argv[5]) );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"delfrozenplayers") )
	{
		StorageEnv::Close();
		DelFrozenPlayers(StorageEnv::get_datadir().c_str(), atoi(argv[3]) );
//		StorageEnv::checkpoint();
//		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"delzoneplayers") )
	{
		StorageEnv::Close();
		DelZonePlayers(StorageEnv::get_datadir().c_str(), atoi(argv[3]) );
//		StorageEnv::checkpoint();
//		StorageEnv::Close();
		return 0;
	}
	else if(argc == 5 && 0 == strcmp(argv[2],"allocnewlogicuid"))
	{
		AllocNewLogicuid(argv[3], atoi(argv[4]));
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if(3 == argc && 0 == strcmp(argv[2], "syncnamechange"))
	{
		//StorageEnv::checkpoint();
		StorageEnv::Close();
		SyncNameChange(StorageEnv::get_datadir().c_str());
		return 0;
	}
	else if(3 == argc && 0 == strcmp(argv[2], "verifyname"))
	{
		//StorageEnv::checkpoint();
		StorageEnv::Close();
		VerifyName(StorageEnv::get_datadir().c_str());
		return 0;
	}
	else if(6 == argc && 0 == strcmp(argv[2], "movedbto"))
	{
		SyncNameChange((std::string(argv[4]) + "/dbdata").c_str());
		MoveDBTo(argv[3], argv[4], atoi(argv[5]));
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"exportcsvuseridexchange") )
	{
		ExportCsvUserIdExchange(argv[3]);
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if( argc == 4 && 0 == strcmp(argv[2],"cmpuseridexchange") )
	{
		CmpUserIdExchange(argv[3]);
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 3 && 0 == strcmp(argv[2], "listduprolename"))
	{
		ListDupRolename();
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 3 && 0 == strcmp(argv[2], "repairfaname"))
	{
		RepairFaName(); //修复 factionname familyname 表，改名导致的错误
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 3 && 0 == strcmp(argv[2], "repairflower"))
	{
		if(!ListTopFlowerDataManager::Instance()->Init()) //修复 flower 表
		{
			Log::log( LOG_INFO, "Init TopFlower fail." );
			return -1;
		}
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 3 && 0 == strcmp(argv[2], "rewritebase2"))
	{
		RewriteBase2();
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}

	else if (argc == 3 && 0 == strcmp(argv[2], "repairflower"))
	{
		if(!ListTopFlowerDataManager::Instance()->Init()) //修复 flower 表
		{
			Log::log( LOG_INFO, "Init TopFlower fail." );
			return -1;
		}
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 4 && 0 == strcmp(argv[2], "rewriteflowerroleid"))
	{
		RewriteFlowerRoleid(argv[3]);
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 4 && 0 == strcmp(argv[2], "getreferrals"))
	{
		GetReferrals(atoi(argv[3]));
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 4 && 0 == strcmp(argv[2], "getreferrer"))
	{
		int zoneid=atoi(conf->find(GameDBServer::GetInstance()->Identification(), "zoneid").c_str());
		GetReferrer(zoneid, argv[3]);
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 6 && 0 == strcmp(argv[2], "fixcitystore"))
	{
		FixCityStore(argv[3], argv[4], argv[5]);
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
	else if (argc == 4 && 0 == strcmp(argv[2], "delinactiveusers"))
	{
		StorageEnv::Close();
		DelInactiveUsers(StorageEnv::get_datadir().c_str(), atoi(argv[3]));
		//StorageEnv::checkpoint( );
		//StorageEnv::Close();
		return 0;
	}
	else if (argc == 3 && 0 == strcmp(argv[2], "repairstockcancel"))
	{
		RepairStockCancel("useridttt");
		StorageEnv::checkpoint( );
		StorageEnv::Close();
		return 0;
	}
        else if( argc == 3 && 0 == strcmp(argv[2],"checkabnormalrole") )
        {
                CheckAbnormalRole();
                StorageEnv::checkpoint();
                StorageEnv::Close();
                return 0;
        }
	else if( argc >= 4 || (argc == 3 && 0 != strcmp(argv[2], "norepair")))
	{
		printhelp( argv[0] );
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return -1;
	}

        LOG_TRACE(COMPILERNAME);
        LOG_TRACE("compile time: %s:%s XMLVERSION:%s", __DATE__, __TIME__, XMLVERSION);
	if (!(argc == 3 && 0==strcmp(argv[2], "norepair")))
	{
		LOG_TRACE("repair db begin");
		RepairFaName(); //修复 factionname familyname 表，改名导致的错误
		LOG_TRACE("repair db end");
	}
	if( 0 == atol(conf->find("gamedbd","noimportclsconfig").c_str()) )
	{
		Log::log( LOG_INFO, "Begin import clsconfig ..." );
		ImportClsConfig();
	}

	if (!GameDBManager::GetInstance()->InitGameDB())
	{
		Log::log( LOG_ERR, "Init GameDB, error initialize GameDB." );
		exit(EXIT_FAILURE);
	}
	else
	{
		Log::log( LOG_INFO, "Init GameDB successfully." );
	}
	{
		GameDBServer *manager = GameDBServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		manager->zoneid=atoi(conf->find(manager->Identification(), "zoneid").c_str());
		Log::log( LOG_INFO, "Init GameDB successfully zoneid=%d.",manager->zoneid );
		manager->aid = atoi(conf->find(manager->Identification(), "aid").c_str());
		Log::log( LOG_INFO, "Init GameDB successfully.aid=%d" ,manager->aid);
		Protocol::Server(manager);
	}
	CirclePool::Instance();//for init
	DBConsignSoldRole::LoadItemid();//load itemid.txt
	ForbidPolicy::GetInstance()->LoadPolicy("forbidpolicy.conf");

	Thread::HouseKeeper::AddTimerTask(LogTask::GetInstance(),10);

	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	pthread_t	th;
	pthread_create( &th, NULL, &StorageEnv::BackupThread, NULL );
	Thread::Pool::Run( &s_policy );
	return 0;
}

