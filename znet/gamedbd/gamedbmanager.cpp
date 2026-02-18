#include "gamedbmanager.h"

#include "conf.h"
#include "storage.h"
#include "dbbuffer.h"
#include "parsestring.h"
#include "conv_charset.h"

#include "log.h"

#include "roleid"

namespace GNET
{
	void GameDBManager::ReadConfig()
	{
		try {   
			StorageEnv::CommonTransaction txn;
			Marshal::OctetsStream key, value;
			key << (int)100;
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage("config");
			StorageEnv::Storage *pbase = StorageEnv::GetStorage("base");
			try {   
				if (pstorage->find(key, value, txn))
					value >> config;
				else {  
					if(pbase->count()<10000)
					{       
						config.init_time = time(0);
						config.open_time = 0;
					}       
					else    
					{       
						config.init_time = 1211353082;
						config.open_time = 1211353082;
					}       
					config.is_central_db = IsCentralDB()?1:0;
					pstorage->insert(key, Marshal::OctetsStream() << config, txn);
				}       
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
			Log::log( LOG_ERR, "ReadDBConfig exception, what=%s\n", e.what() );
		}       
		LOG_TRACE("ReadDBConfig init_time %d open_time %d is_central_db %d\n", config.init_time, config.open_time, config.is_central_db);
	}

    void GameDBManager::SaveConfig()
	{
		try {   
			StorageEnv::CommonTransaction txn;
			Marshal::OctetsStream key, value;
			key << (int)100;
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage("config");
			try {
				pstorage->insert(key, Marshal::OctetsStream() << config, txn);
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
			Log::log( LOG_ERR, "SaveDBConfig exception, what=%s\n", e.what() );
		}
	}

	void GameDBManager::OnCreateRole()
	{
		if (++create_count >= 256) {
			create_count = 256;
			if (config.open_time == 0)
			{       
				config.open_time = time(0);
				SaveConfig();
			}       
		}       
		LOG_TRACE("gamedbmanager::OnCreateRole() create_count=%d", create_count);
	}

	void GameDBManager::InitGUID()
	{
		guid = (int64_t)Timer::GetTime() * 0x100000000LL;
		StorageEnv::Storage * plog  = StorageEnv::GetStorage("syslog");
		Marshal::OctetsStream key, value;
		StorageEnv::AtomTransaction txn;
		key << guid;
		printf("GUID start from %lld\n",guid);
		while(plog->find(key,value,txn));
		{
			key.clear();
			guid += 0x100000000LL;
			key << guid;
		}
	}

	bool GameDBManager::InitGameDB()
	{
		Conf* conf=Conf::GetInstance();
		zoneid = atoi(conf->find("GameDBServer", "zoneid").c_str());
		areaid = atoi(conf->find("GameDBServer", "aid").c_str());
		std::string central_db = conf->find("GameDBServer", "is_central_db");
		if (central_db == "true")
			SetCentralDB(true);

		deletetimeout = atoi( conf->find("Role","delete_timeout").c_str() );
		Log::log( LOG_INFO, "InitGameDB, Role delete time is %d.\n",deletetimeout);

		std::string value = conf->find("OCCUPATION","occupations");
		if (0==value.size())
		{
			Log::log( LOG_ERR, "InitGameDB, no occupations find in configure file." );
			return false;
		}
		std::vector<string> occupation_list;
		if (!ParseStrings(value,occupation_list))
		{
			Log::log( LOG_ERR, "InitGameDB, occupations_list parse failed." );
			return false;
		}
		if (0==occupation_list.size())
		{
			Log::log( LOG_ERR, "InitGameDB, occupations_list parse result is zero." );
			return false;
		}
		ReadConfig();
		if (IsCentralDB() != (bool)config.is_central_db)
		{
			Log::log(LOG_ERR, "InitGameDB, Can not determine whether it is central DB");
			return false;
		}

		//get occupation information
		GRoleDetail role;
		for (size_t occp=0;occp<occupation_list.size();occp++)
		{
			role.name=Octets(0);
			role.status.level=1;
			role.status.exp=0;
			role.status.pp=0;
			
			string &occup = occupation_list[occp];
			const char *clsstr = conf->find(occup,"cls").c_str(); 
			//int cls = atoi(conf->find(occupation_list[occp],"cls").c_str());
			int cls = atoi(clsstr);
			role.status.hp=atoi(conf->find(occupation_list[occp],"hp").c_str());
			role.status.mp=atoi(conf->find(occupation_list[occp],"mp").c_str());
			role.status.posx=atof(conf->find(occupation_list[occp],"posx").c_str());
			role.status.posy=atof(conf->find(occupation_list[occp],"posy").c_str());
			role.status.posz=atof(conf->find(occupation_list[occp],"posz").c_str());
			role.status.worldtag = atoi(conf->find(occupation_list[occp],"world").c_str());

			// save
			char buffer[128];
			Octets name;
			role.gender = 0;
			sprintf( buffer, "cls%dgender%d", cls, role.gender );
			name.replace( buffer, strlen(buffer) );
			CharsetConverter::conv_charset_g2u( name, role.name );
			SaveClsDetail( cls, role.gender, role );
			role.gender = 1;
			sprintf( buffer, "cls%dgender%d", cls, role.gender );
			name.replace( buffer, strlen(buffer) );
			CharsetConverter::conv_charset_g2u( name, role.name );
			SaveClsDetail( cls, role.gender, role );
		}

		//save user table
		SaveUserDefault();
		InitGUID();
		return true;
	}

#define CLSGENDER2ROLEID(cls,gender)	(16+cls+cls+(gender?1:0))
#define CLSGENDER2ROLEID2(cls,gender)	(32+cls+cls+(gender?1:0))
#define CLSGENDER2ROLEID3(cls,gender)	(48+cls+cls+(gender?1:0))

	unsigned int GetDataRoleId(unsigned int cls)
	{
		return 16;
	}

	bool GameDBManager::SaveUserDefault( )
	{
		try
		{
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			StorageEnv::AtomTransaction txn;
			try
			{
				RoleList rolelist;
				rolelist.InitialRoleList();
				rolelist.AddRole( 16 % MAX_ROLE_COUNT );
				rolelist.AddRole( 17 % MAX_ROLE_COUNT );
				rolelist.AddRole( 18 % MAX_ROLE_COUNT );
				rolelist.AddRole( 19 % MAX_ROLE_COUNT );
				rolelist.AddRole( 20 % MAX_ROLE_COUNT );
				rolelist.AddRole( 21 % MAX_ROLE_COUNT );
				rolelist.AddRole( 22 % MAX_ROLE_COUNT );
				rolelist.AddRole( 23 % MAX_ROLE_COUNT );

				User user;
				user.logicuid = 16;
				user.rolelist = rolelist.GetRoleList();
				Marshal::OctetsStream key, value_user;
				key << user.logicuid;
				value_user << user;
				puser->insert( key, value_user, txn );
				
				RoleList rolelist2;
				rolelist2.InitialRoleList();
				rolelist2.AddRole( 32 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 33 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 34 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 35 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 36 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 37 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 38 % MAX_ROLE_COUNT);
				rolelist2.AddRole( 39 % MAX_ROLE_COUNT);
				User user2;
				user2.logicuid = 32;
				user2.rolelist = rolelist2.GetRoleList();
				Marshal::OctetsStream key2, value_user2;
				key2 << user2.logicuid;
				value_user2 << user2;
				puser->insert( key2, value_user2, txn );

				RoleList rolelist3;
				rolelist3.InitialRoleList();
				rolelist3.AddRole( 48 % MAX_ROLE_COUNT);
				User user3;
				user3.logicuid = 48;
				user3.rolelist = rolelist3.GetRoleList();
				Marshal::OctetsStream key3, value_user3;
				key3 << user3.logicuid;
				value_user3 << user3;
				puser->insert( key3, value_user3, txn );

				return true;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SaveUserDefault, what=%s\n", e.what() );
		}
		return false;
	}	

	bool GameDBManager::SaveClsDetail(unsigned int cls, char gender, GRoleDetail & role)
	{
		Marshal::OctetsStream key, value, value_base, value_status,
			value_inventory, value_equipment, value_task, value_storehouse;

		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
			StorageEnv::AtomTransaction txn;
			try
			{
				unsigned int roleid;
				if (cls == 0)
					roleid = CLSGENDER2ROLEID(0,gender);
				else if (cls == 33)
					roleid = CLSGENDER2ROLEID(1, gender);
				else if (cls == 39)
					roleid = CLSGENDER2ROLEID(2, gender);
				else if (cls == 45)
					roleid = CLSGENDER2ROLEID(3, gender);
				else if (cls == 51)
					roleid = CLSGENDER2ROLEID2(0, gender);
		                else if (cls == 96)
		                        roleid = CLSGENDER2ROLEID2(1, gender);
				else if (cls == 56)
		                        roleid = CLSGENDER2ROLEID2(2, gender);
				else if (cls == 102)
		                        roleid = CLSGENDER2ROLEID2(3, gender);
				else if (cls == 108)
					roleid = CLSGENDER2ROLEID3(0, 0);
				else
					return false;

				key << RoleId( roleid );
				role.status.id = roleid;
				role.status.occupation = cls;
				role.id = roleid;

				GRoleBase	base;
				GRoleDetailToBase( role, base );
				base.status = _ROLE_STATUS_NORMAL;
				value_base << base;
				value_status << role.status;
				value_inventory << role.inventory;
				value_task<< role.task;
				value_storehouse << role.storehouse;

				if( !pbase->find( key, value, txn ) )
					pbase->insert( key,value_base, txn );
				if(!pstatus->find( key, value, txn ) )
					pstatus->insert( key, value_status, txn );
				if(!pinventory->find( key, value, txn ) )
					pinventory->insert( key, value_inventory, txn );
				if( !pstorehouse->find( key, value, txn ) )
					pstorehouse->insert( key, value_storehouse, txn );
				if(!ptask->find( key, value, txn) )
					ptask->insert( key, value_task, txn );
				return true;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SaveClsDetail, what=%s\n", e.what() );
		}
		return false;
	}

	bool GameDBManager::GetClsDetail(unsigned int cls, char gender,
			GRoleBase &base, GRoleStatus &status,
			GRolePocket &inventory,
			GRoleStorehouse &storehouse)
	{
		Marshal::OctetsStream key, value_base, value_status, value_inventory, value_equipment,
		value_taskinventory, value_storehouse;

		unsigned int roleid = 0;
		if (cls == 0)
			roleid = CLSGENDER2ROLEID(0,gender);
		else if (cls == 33)
			roleid = CLSGENDER2ROLEID(1, gender);
		else if (cls == 39)
			roleid = CLSGENDER2ROLEID(2, gender);
		else if (cls == 45)
			roleid = CLSGENDER2ROLEID(3, gender);
		else if (cls == 51)
			roleid = CLSGENDER2ROLEID2(0, gender);
                else if (cls == 96)
                        roleid = CLSGENDER2ROLEID2(1, gender);
		else if (cls == 56)
                        roleid = CLSGENDER2ROLEID2(2, gender);
		else if (cls == 102)
			roleid = CLSGENDER2ROLEID2(3, gender);
		else if (cls == 108)
		{
			if (gender != 0)
				return false;
			roleid = CLSGENDER2ROLEID3(0, 0); //人马只有男性职业 如果再加一个无性别的职业 可以加到这里
		}
		else
		{
			Log::log( LOG_ERR, "GetClsDetail, occupation=%d, gender=%d not found\n", cls, gender);
			return false;
		}

		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			StorageEnv::AtomTransaction txn;
			try
			{
				key << RoleId( roleid );

				value_base = pbase->find( key, txn );
				value_status = pstatus->find( key, txn );
				value_inventory = pinventory->find( key, txn );
				value_storehouse = pstorehouse->find( key, txn );

				value_base >> base;
				value_status >> status;
				value_inventory >> inventory;
				value_storehouse >> storehouse;

				base.gender = gender;
				base.help_states.clear();

				status.custom_status.clear();
				status.filter_data.clear();
				status.charactermode.clear();
				status.instancekeylist.clear();
				return true;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "GetClsDetail, roleid=%d, occupation=%d, gender=%d, what=%s\n", roleid, cls, gender, e.what() );
		}
		return false;
	}

	bool GameDBManager::CheckForbidUserLogin(const User &user, time_t & left_time)
	{
		for (GRoleForbidVector::const_iterator it = user.forbid.begin(); it != user.forbid.end(); it++)
		{
			if (it->type == FORBID_USER_LOGIN)
			{
				left_time = it->time + it->createtime - Timer::GetTime();
				if (left_time > 0)
					return true;
			}
		}
		return false;
	}

	void GRoleBaseToDetail( const GRoleBase & base, GRoleDetail & detail )
	{
		detail.id = base.id;
		detail.name = base.name;
		detail.faceid = base.faceid;
		detail.hairid = base.hairid;
		detail.gender = base.gender;
		detail.create_time = base.create_time;
		detail.title = base.title;
		detail.familyid = base.familyid;
		detail.spouse = base.spouse;
		detail.jointime = base.jointime;
		detail.sectid = base.sectid;
		detail.initiallevel = base.initiallevel;
		detail.fashionid = base.fashionid;
		if(base.userid)
			detail.userid = base.userid;
		else
			detail.userid = LOGICUID(base.id);
	}

	void GRoleDetailToBase( const GRoleDetail & detail, GRoleBase & base )
	{
		base.id = detail.id;
		base.name = detail.name;
		base.faceid = detail.faceid;
		base.hairid = detail.hairid;
		base.gender = detail.gender;
		base.create_time = detail.create_time;
		base.title = detail.title;
		base.familyid = detail.familyid;
		base.fashionid = detail.fashionid;
	}

};

