#include <stdio.h>
#include "conf.h"
#include "log.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "roleid"
#include "groleinventory"
#include "grolepocket"
#include "grolestorehouse"
#include "gpocketinventory"
#include "storage.h"
#include "gmail"
#include "stocklog"
#include "user"
#include "userid"
#include "gmailbox"
#include "grolebase"
#include "grolebase2"
#include "dbconfig"
#include "localmacro.h"
#include "xmlversion.h"
#include "gconsigndb"

namespace GNET
{

#define ROLEIDMIN(x) x
#define ROLEIDMAX(x) (x | 0x0000000f)

std::string config_file = "cashstat.conf";
std::string dbhome = "dbhomewdb";
std::string shopdata_file;
bool loaded = false;

struct GSHOP_ITEM 
{
	unsigned int id;     			// object id of this item
	unsigned int num;    			// number of objects in this item
	char icon[128];      			// icon file path

	struct  LIST
	{
		unsigned int price;		// price of this item
		unsigned int time;      	// time of duration, 0 means forever
	} list[1];
	

	unsigned int props; 			// mask of all props, currently from low bit to high bit: æ–°å“ï¼Œä¿ƒé”€å“ï¼ŒæŽ¨èå“

	int main_type; 				// index into the main type array
	int sub_type; 				// index into the sub type arrray
	int local_id; 				// id of this shop item, used only for localization purpose

	unsigned short desc[512]; 		// simple description
	unsigned short szName[32];		// name of this item to show
};

static void TravelTable(const char *table_name, StorageEnv::IQuery &q)
{
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage(table_name);
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.browse( q );
		}
		catch (DbException e) { throw; }
		catch (...)
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log(LOG_ERR, "err occur when walking %s\n", table_name);
		return;
	}
}

struct UserData
{
	int logintime;
	unsigned int logicuid;
	unsigned int rolelist;
	unsigned int cash_add;
	unsigned int cash_buy;
	unsigned int cash_sell;
	unsigned int cash_used;
	bool obsolete;
	UserData():logintime(0),logicuid(0),cash_add(0),cash_buy(0),cash_sell(0),cash_used(0),obsolete(0) { }
};

typedef std::map<unsigned int /*itemid*/, unsigned int /*count*/> RoleItemMap;
typedef std::map<unsigned int /*itemid*/, unsigned int /*count*/> ItemCounter;

struct TRoleData
{
	int frozen;	//½ÇÉ«Êý¾ÝÊÇ·ñ±»¶³½á 1:ÓÐÐ§Êý¾ÝÔÚ¿ç·þDBÖÐ 0:ÓÐÐ§Êý¾ÝÔÚ±¾µØ
	int src_zoneid; //¿ç·þDBÖÐ½ÇÉ«ËùÀ´×ÔµÄ·þÎñÆ÷ID
	RoleItemMap item_map;
	TRoleData(int _frozen, int _zoneid) : frozen(_frozen), src_zoneid(_zoneid) {}
	TRoleData() : frozen(0), src_zoneid(0) {}
};
typedef std::map<unsigned int, TRoleData> RoleMap;
typedef std::map<unsigned int, UserData>  UserMap;
typedef std::map<int, int> ItemSet;
typedef std::set<unsigned int> RoleSet;

struct TDataBase
{
	bool is_centraldb;
	UserMap user_map;
	RoleMap role_map;
	ItemCounter item_counter;
	bool listall;
	time_t listthreshold;   //Ö»´òÓ¡×îºóµÇÂ¼Ê±¼ä>=listthresholdµÄ½ÇÉ«
	RoleSet care_roles;  //Èç¹ûlistallÎªfalse care_roles Ö»ÊÕ¼¯ÄÇÐ©ÉÏ´ÎµÇÂ¼Ê±¼ä>=listthreshold µÄroleid

	TDataBase() : is_centraldb(false), listall(true), listthreshold(0) {}
	struct PocketQuery : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			RoleId      roleid;
			GRolePocket pocket;
			try
			{
				key_os >> roleid;
				//value_os >> pocket;

//				if (pocket.items.size() == 0 && pocket.equipment.size() == 0)
//					return true;

				RoleMap::iterator it = tdatabase->role_map.end();
				RoleMap::iterator rit = tdatabase->role_map.find(roleid.id);
				if (rit != it && rit->second.frozen==1)
					return true;
				if (!tdatabase->listall && tdatabase->care_roles.find(roleid.id) == tdatabase->care_roles.end())
					return true;
				value_os >> pocket;
				GRoleInventoryVector::const_iterator cit = pocket.items.begin();
				for (; cit != pocket.items.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

				cit = pocket.equipment.begin();
				for (; cit != pocket.equipment.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

				cit = pocket.petbadge.begin();
				for (; cit != pocket.petbadge.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

				cit = pocket.petequip.begin();
				for (; cit != pocket.petequip.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

				cit = pocket.fashion.begin();
				for (; cit != pocket.fashion.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

				GPocketInventoryVector::const_iterator cpit = pocket.pocket_items.begin();
				for (; cpit != pocket.pocket_items.end(); ++ cpit)
					tdatabase->AddRoleItem(it, roleid.id, cpit->id, cpit->count);

				if (pocket.mountwing.size())
				{
					Marshal::OctetsStream os_mountwing(pocket.mountwing);
					GRoleInventoryVector mountwing;
					short mountwingsize = 0;
					os_mountwing >> mountwingsize;
					os_mountwing >> mountwing;
					cit = mountwing.begin();
					for (; cit != mountwing.end(); ++ cit)
						tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);
				}

				cit = pocket.gifts.begin();
				for (; cit != pocket.gifts.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "PocketQuery, error unmarshal");
				return true;
			}
			return true;
		}
	}; 

	struct StorehouseQuery : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			RoleId      roleid;
			GRoleStorehouse storehouse;
			try
			{
				key_os >> roleid;
//                                value_os >> storehouse;
				
//				if (storehouse.items.size() == 0) return true;

                                RoleMap::iterator it = tdatabase->role_map.end();
                                RoleMap::iterator rit = tdatabase->role_map.find(roleid.id);
				if (rit != it && rit->second.frozen==1)
					return true;
				if (!tdatabase->listall && tdatabase->care_roles.find(roleid.id) == tdatabase->care_roles.end())
					return true;
                                value_os >> storehouse;
                                GRoleInventoryVector::const_iterator cit = storehouse.items.begin();
                                for (; cit != storehouse.items.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

                                cit = storehouse.items2.begin();
                                for (; cit != storehouse.items2.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);

                                cit = storehouse.fuwen.begin();
                                for (; cit != storehouse.fuwen.end(); ++ cit)
					tdatabase->AddRoleItem(it, roleid.id, cit->id, cit->count);
			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "StorehouseQuery, error unmarshal.");
				return true;
			}
			return true;
		}
	};
	//Ä¿Ç°ÈÏÎªÓÊÏäÊý¾Ý²»»á´«µÝµ½¿ç·þ ·ñÔòÒª×öÏàÓ¦ÐÞ¸Ä
	struct MailQuery : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			RoleId roleid;
			GMailBox mailbox;
			try
			{
				key_os >> roleid;

				if (!tdatabase->listall && tdatabase->care_roles.find(roleid.id) == tdatabase->care_roles.end())
					return true;
				value_os >> mailbox;
				RoleMap::iterator it = tdatabase->role_map.end();
				GMailVector::iterator mit = mailbox.mails.begin();
				for (; mit != mailbox.mails.end(); ++mit)
				{
					if(mit->header.attribute & (1 << _MA_ATTACH_OBJ))
						tdatabase->AddRoleItem(it, roleid.id, mit->attach_obj.id, mit->attach_obj.count);
				}

			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "MailQuery, error unmarshal.");
				return true;
			}
			return true;
		}
	};
	//Ä¿Ç°ÈÏÎª¼ÄÊÛÊý¾Ý²»»á´«µÝµ½¿ç·þ ·ñÔòÒª×öÏàÓ¦ÐÞ¸Ä
	struct ConsignQuery : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			int64_t sn;
			GConsignDB detail;
			try
			{
				key_os >> sn;
				if(sn==0)
					return true;
				value_os >> detail;
					
				RoleMap::iterator it = tdatabase->role_map.end();
				int roleid = (detail.info.state != DSTATE_SOLD)? detail.seller_roleid: detail.buyer_roleid;
				if(detail.info.state == DSTATE_SOLD && detail.mail_status == BUYER_SENDED)
					return true;
				if (!tdatabase->listall && tdatabase->care_roles.find(roleid) == tdatabase->care_roles.end())
					return true;
				tdatabase->AddRoleItem(it,roleid,detail.item.id,detail.item.count);

			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "ConsignQuery, error unmarshal.");
				return true;
			}
			return true;
		}
	};

	struct BaseQuery : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			RoleId roleid;
			GRoleBase base;
			try
			{
				key_os >> roleid;
				value_os >> base;
				int userid = base.userid;
				if(!userid)
					userid = LOGICUID(roleid.id);
				UserMap::iterator iu = tdatabase->user_map.find(userid);
				if (iu != tdatabase->user_map.end()) 
				{
					if(base.lastlogin_time>iu->second.logintime)
						iu->second.logintime = base.lastlogin_time;
				}
				else
					Log::log( LOG_ERR, "BaseQuery, can not find user %d in user_map for role %d.", userid, roleid.id);
				if (base.status == _ROLE_STATUS_FROZEN)//¿ç·þÖÐÒ²ÐèÒª Èç¹ûÊÇfrozen×´Ì¬Ôò²»É¨Ãè°ü¹ü¡¢²Ö¿â
					tdatabase->role_map.insert(std::make_pair(roleid.id, TRoleData(1, 0)));
				if (!tdatabase->listall && base.lastlogin_time >= tdatabase->listthreshold 
						&& roleid.id > 0)
					tdatabase->care_roles.insert(roleid.id);
			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "BaseQuery, error unmarshal.");
				return true;
			}
			return true;
		}
	};

	struct Base2Query : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			RoleId roleid;
			try
			{
				key_os >> roleid;
				RoleMap::iterator it = tdatabase->role_map.find(roleid.id);
				if (it != tdatabase->role_map.end() && it->second.frozen == 0)
				{
					GRoleBase2 base2;
					value_os >> base2;
					it->second.src_zoneid = base2.src_zoneid;
				}
//				else
//					Log::log(LOG_ERR, "Base2Query, can not find role %d in role_map", roleid.id);	
			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "BaseQuery, error unmarshal.");
				return true;
			}
			return true;
		}
	};

	struct UserQuery : public StorageEnv::IQuery
	{   
		TDataBase *tdatabase;
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			Marshal::OctetsStream key_os;
			key_os = key;
			Marshal::OctetsStream value_os;
			value_os = value;

			UserID userid; 
			User user;
			try
			{
				key_os >> userid;
				value_os >> user;
	
				tdatabase->ReadUser(userid.id, user);
				
			} catch ( Marshal::Exception & ) {
				Log::log( LOG_ERR, "UserQuery, error unmarshal.");
				return true;
			}
			return true;
		}
	};

	bool CountItem(int id, int count)
	{
		ItemCounter::iterator it = item_counter.find(id);
		if (it != item_counter.end())
		{
			it->second += count;
			return true;
		}
		return false;
	}

	void AddRoleItem(RoleMap::iterator & iter, int roleid, int id, int count)
	{
		if (CountItem(id, count))
		{
			if (iter == role_map.end())
			{
				iter = role_map.find(roleid);
				if (iter == role_map.end())
					iter = role_map.insert(std::make_pair(roleid, TRoleData())).first;
			}	
			iter->second.item_map[id] += count;
		}
	}
	// load shop data from gshop.data 
	bool LoadShopData()
	{
		LoadConfItem();
		std::ifstream ifs(shopdata_file.c_str(), std::ios::in | std::ios::binary);
		if (!ifs) return  false;

		int size = sizeof(GSHOP_ITEM);
		GSHOP_ITEM data;
		char buf[size];	
		try
		{
			int timestamp, nCount;
			ifs.read((char*)&timestamp, 4);
			ifs.read((char*)&nCount, 4);

			if (nCount < 0 || nCount > 65535) return false;
			for(int i=0; i<nCount; i++)
			{
				ifs.read((char*)&data, size);
				item_counter.insert(std::make_pair(data.id, 0));
			}
		}
		catch(...) { }
		return true;
	}

	// update shop data to config file
	void UpdateConfigFile()
	{
		// read & modify config file 
		std::ifstream ifs(config_file.c_str(), std::ios::in);
		if (!ifs)
		{ 
			std::cout << "can't update config file, " << config_file << " open failed" << std::endl;
			return ;
		}
		std::ostringstream os;
		std::string line;
		bool inflag = false;
		while (std::getline(ifs, line))
		{
			if (!inflag)
			{
				if (line == "[items]")
				{
					inflag = true;
					os << line << "\n";
					ItemCounter::iterator it = item_counter.begin();	
					for (; it != item_counter.end(); ++ it)
						os << it->first << " = 1\n";
					continue;	
				}
				
				os << line << "\n";
			}
			else
				if (line[0] == '[') 
				{
					inflag = false;	
					os << line << "\n";
				}
			
		}
		ifs.close();
		// write config file
		std::ofstream ofs(config_file.c_str(), std::ios::out | std::ios::trunc);
		if (!ofs)
		{ 
			std::cout << "can't update config file, " << config_file << " open failed" << std::endl;
			return ;
		}
		ofs << os.str();
		ofs.close();
	}
	void ReadDBType()
	{
		try {   
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage("config");
			StorageEnv::CommonTransaction txn;
			Marshal::OctetsStream value;
			try {
				DBConfig config;
				Marshal::OctetsStream(pstorage->find(Marshal::OctetsStream()<<(int)100, txn)) >> config;
				is_centraldb = config.is_central_db;
				if (is_centraldb)
					std::cerr << "#DB type : Central DB" << std::endl;
				else
					std::cerr << "#DB type : Normal DB" << std::endl;
			} catch (DbException &e) { 
				throw;  
			} catch (...) { 
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}       
		}
		catch ( DbException e )
		{       
			Log::log( LOG_ERR, "ReadDBType exception, what=%s\n", e.what() );
			is_centraldb = false;
		}
	}
	void LoadConfItem()
	{
		// combine shop data    
		Conf * conf = Conf::GetInstance();
		std::vector<std::string> keys;
		conf->getkeys("items", keys);
		std::vector<std::string>::iterator it = keys.begin();
		for (; it != keys.end(); ++ it)
		{
			if (atoi(conf->find("items", *it).c_str()) == 1)
				item_counter.insert(std::make_pair(atoi(it->c_str()), 0));
		}
	}
	void Load()
	{
		ReadDBType();
		LoadConfItem();

		UserQuery uq;
		uq.tdatabase = this;
		TravelTable("user", uq);

		BaseQuery bq;
		bq.tdatabase = this;
		TravelTable("base", bq);
		LOG_TRACE("care_roles size %d", care_roles.size());

		PocketQuery qp;
		qp.tdatabase = this;
		TravelTable("inventory", qp);

		StorehouseQuery qs;
		qs.tdatabase = this;
		TravelTable("storehouse", qs);

		if (!is_centraldb)
		{
			MailQuery mq;
			mq.tdatabase = this;
			TravelTable("mailbox", mq);

			ConsignQuery cq;
			cq.tdatabase = this;
			TravelTable("consign", cq);
		}

		if (is_centraldb)
		{
			Base2Query b2q;
			b2q.tdatabase = this;
			TravelTable("base2", b2q);
		}
	}

	void PrintHeader(int userid, int roleid, UserData& data, int frozen, int zoneid)
	{
		std::cout << roleid << "," << userid << "," << data.logintime << "," << data.obsolete << "," << frozen << "," << zoneid << ",add:" << (int)data.cash_add << ";buy:" 
			<< (int)data.cash_buy << ";sell:" << (int)data.cash_sell << ";used:" << (int)data.cash_used << ";";
	}

	void Dump()
	{
		time_t now = time(NULL);
		for(UserMap::iterator iu=user_map.begin();iu!=user_map.end();++iu)
		{
			UserData& data = iu->second;
			if(data.logintime && now-data.logintime>365*24*3600)
			{
				// ³¬¹ý365Ìì²»µÇÂ½µÄÕË»§£¬ÉèÖÃ±êÊ¶
				data.obsolete = true;
			}
			int userid = iu->first;
			int roleid = data.logicuid;
			int lastrole = 0;
			bool output = false;
			bool need_cashdata = true; //ÊÇ·ñÐèÒªÊä³öÕËºÅµÄÔª±¦ÐÅÏ¢
			if (!listall && data.logintime < listthreshold)
				need_cashdata = false;
			for(int i=1;i<0x10000;i<<=1,roleid++)
			{
				if((i&data.rolelist)==0)
					continue;
				lastrole = roleid;
				RoleMap::iterator it = role_map.find(roleid);
				if(it==role_map.end())
					continue;
				if (is_centraldb && it->second.frozen==1)
				{
					//std::cout << "negelect frozen " << roleid << "\r\n";
					continue;//¿ç·þÉÏ²»Êä³öfrozenÊý¾Ý
				}
//				if (!listall && care_roles.find(roleid) == care_roles.end())
//					continue;
				PrintHeader(userid,roleid,data,it->second.frozen,it->second.src_zoneid);
				RoleItemMap::const_iterator cit = it->second.item_map.begin();
				for (; cit != it->second.item_map.end(); ++ cit)
					std::cout << cit->first << ":" << cit->second << ";";
				std::cout << "\r\n";
				output = true;
			}
			if(need_cashdata && !output && (data.cash_add||data.cash_buy||data.cash_sell||data.cash_used))
			{
				PrintHeader(userid,lastrole,data,0,0);
				std::cout << "\r\n";
			}
		}
		//DumpTotal();
	}

	void DumpTotal()
	{
		std::ofstream ofs("total.txt");
		if (!ofs)
		{
			printf("Err: output file total.txt open failed\n" );
			return;
		}
		
		ItemCounter::const_iterator it = item_counter.begin();
		for (; it != item_counter.end(); ++ it)
		{
			ofs << it->first << ":" << it->second << "\r\n";
		}
		ofs.close();
	}
	void ReadUser(int userid, const User & user)
	{
		UserData data;
		data.logicuid = user.logicuid;
		data.rolelist = user.rolelist;
		data.cash_add = user.cash_add;
		data.cash_buy = user.cash_buy;
		data.cash_sell = user.cash_sell;
		data.cash_used = user.cash_used;
		user_map[userid] = data;
	}
};

}; // namespace GNET

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Cashstat version " << XMLVERSION << std::endl;
		std::cerr << "Usage: " << argv[0] << " conf-file [dbhome] " << std::endl;
		exit(-1);
	}

	bool listall = true;
	time_t listthresh = 0;
	std::string date;
	if (argv[1][0] != '-')
	{
		if (access(argv[1], R_OK) == -1 )
		{
			std::cerr << "Usage: " << argv[0] << " conf-file [dbhome] " << std::endl;
			std::cerr << "Usage: " << argv[0] << " -c conf-file -D dbhome [-g shopdata] [-t yyyy-mm-dd]" << std::endl;
			exit(-1);
		}
		config_file = argv[1];

		if (argc >= 3)
		{
			if (-1 == access(argv[2], R_OK))
			{
				std::cerr << "Bad dbhome dir" << std::endl;
				exit(-1);
			}
			dbhome = argv[2];
		}

		if (argc >= 4)
		{
			if (-1 == access(argv[3], R_OK))
			{
				std::cerr << "Bad file : gshop.data" << std::endl;
				exit(-1);
			}	
			shopdata_file = argv[3];	
		}
	}	
	else
	{
		int opt = getopt(argc, argv, "c:D:g:ht:");
		while( opt != -1 ) 
		{
			switch( opt ) 
			{
				case 'c':
					config_file = optarg; /* true */
					break;

				case 'D':
					dbhome = optarg;
					break;

				case 'g':
					shopdata_file = optarg;
					break;
				case 't':
					listall = false;
					date = optarg;
					break;
				case 'h':
				default:
					std::cout << "Usage: " << argv[0] << " -c config_file -D dbhome_dir -g gshop.data -t yyyy-mm-dd\r\n";
					exit(0);
			}
			opt = getopt(argc, argv, "c:D:g:ht:");
		}
		
	}

	Conf *conf = Conf::GetInstance(config_file.c_str());
#ifdef USE_WDB
	conf->put("storagewdb", "homedir", dbhome);
#else
	conf->put("storage", "homedir", dbhome);
#endif
	
	if (!shopdata_file.empty())
	{
		TDataBase tdatabase;
		if (!tdatabase.LoadShopData())
		{
			std::cout << "Load shopdata file failed\n";
			exit(-1);
		}
		tdatabase.UpdateConfigFile();
		exit(0);
	}
	if (!listall)
	{
		struct tm dt;
		memset(&dt,0,sizeof(dt));
		if (!strptime(date.c_str(),"%Y-%m-%d", &dt) || dt.tm_year<102)
		{
			Log::log(LOG_ERR, "invalid date %s used", date.c_str());
			exit(-1);
		}
		listthresh = mktime(&dt);
	}
	Log::setprogname(argv[0]);
	bool r = StorageEnv::Open();
	if (r)
	{
		if (listall)
			LOG_TRACE("list all roles");
		else
			LOG_TRACE("list roles that logged in after %s, listthreshold=%d", date.c_str(), listthresh);
		TDataBase tdatabase;
		tdatabase.listall = listall;
		tdatabase.listthreshold = listthresh;
		tdatabase.Load();
		tdatabase.Dump();
	}
	else
		std::cout << "Err: open error" << std::endl;
	return 0;
}

