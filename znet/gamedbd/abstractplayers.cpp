#include <string>
#include "storagetool.h"
#include "macros.h"
#include "dbconfig"
#include "grolebase"
#include "grolebase2"
#include "grolestatus"
#include "grolepocket"
#include "grolestorehouse"
#include "groletask"
#include "groleachievement"
#include "log.h"
#include "marshal.h"
#include "storagewdb.h"
#include "localmacro.h"
#include "gcrssvrteamsdata"
#include "accessdb.h"

#define CONFIG_KEY 100
#define CHECKPOINT_THRESHOLD 1024

namespace GNET
{
static bool CheckDBType(const char *srcpath)
{
	std::string src_dir = srcpath;
        try {   
                StorageEnv::Storage *pstorage = StorageEnv::GetStorage("config");
                StorageEnv::CommonTransaction txn;
                Marshal::OctetsStream value;
                try {
                        if (pstorage->find(Marshal::OctetsStream()<<CONFIG_KEY, value, txn))
                        {
                                DBConfig config;
                                value >> config;
                                Log::log(LOG_INFO, "Dst DB type %d\n", config.is_central_db);
                                if (config.is_central_db != 0)
                                        return false;
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
                Log::log( LOG_ERR, "ReadDBConfig(dst) exception, what=%s\n", e.what() );
        }
        {
                DBStandalone * pstandalone = NULL;
                StorageEnv::Uncompressor * uncompressor = NULL;
                try
                {
                        pstandalone = new DBStandalone( (src_dir+"/config").c_str() );
                        pstandalone->init();
                        uncompressor = new StorageEnv::Uncompressor();
                        Marshal::OctetsStream t;
                        t << CONFIG_KEY;
                        size_t val_len;
                        if ( void *val = pstandalone->find( t.begin(), t.size(), &val_len ) )
                        {
                                GNET::Octets dbval = uncompressor->Update(GNET::Octets(val, val_len));
                                free(val);
                                DBConfig config;
                                Marshal::OctetsStream(dbval) >> config;
                                Log::log(LOG_INFO, "Src DB type %d\n", config.is_central_db);
                                if (config.is_central_db == 0)
                                {
                                        delete pstandalone;
                                        delete uncompressor;
                                        return false;
                                }
                        }
			else
			{
				Log::log(LOG_ERR, "Src DB not initialized");
				delete pstandalone;
				delete uncompressor;
				return false;
			}
                }
                catch ( ... )
                {
                        Log::log( LOG_ERR, "ReadDBConfig(src) exception");
                }
                delete pstandalone;
                delete uncompressor;
        }
	return true;
}

void AbstractPlayerData(GRoleBase & base, const GRoleBase & old_base, GRoleBase2 & base2, const GRoleBase2 & old_base2,
			GRoleStatus & status, const GRoleStatus & old_status)
{
	if (base.status == _ROLE_STATUS_FROZEN)
	{
		Log::log(LOG_ERR, "WARNING! AbstractPlayerData roleid %d _ROLE_STATUS_FROZEN, fix it!", base.id);
		base.status = _ROLE_STATUS_NORMAL;
	}
	base.datagroup = old_base.datagroup;
	/*
	base.datagroup[KEY_DATAGOURP_ROLE_CT_COOLTIME] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_COOLTIME];
	base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID];			
	base.datagroup[KEY_DATAGOURP_ROLE_CT_SCORE] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_SCORE];
	base.datagroup[KEY_DATAGOURP_ROLE_CT_MAX_SCORE] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_MAX_SCORE];
	base.datagroup[KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE];			
	base.datagroup[KEY_DATAGOURP_ROLE_CT_MAX_SCORE_UPDATETIME] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_MAX_SCORE_UPDATETIME];  
	base.datagroup[KEY_DATAGOURP_ROLE_CT_JOINBATTLE_TIME] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_JOINBATTLE_TIME];
	base.datagroup[KEY_DATAGOURP_ROLE_CT_BATTLE_TIMES] = old_base.datagroup[KEY_DATAGOURP_ROLE_CT_BATTLE_TIMES];
	*/
	base.familyid = old_base.familyid;
	base.title = old_base.title;
	base.jointime = old_base.jointime;
	//回来后可能被离婚了 上线时会清除夫妻技能
	base.spouse = old_base.spouse;
	base.sectid = old_base.sectid;
	base.circletrack = old_base.circletrack;

	base2.bonus_withdraw = old_base2.bonus_withdraw;
	base2.bonus_reward = old_base2.bonus_reward;
	base2.bonus_used = old_base2.bonus_used;
	base2.exp_withdraw_today = old_base2.exp_withdraw_today;
	base2.exp_withdraw_time = old_base2.exp_withdraw_time;
	base2.data_timestamp++;
	base2.src_zoneid = 0;

	status.worldtag = old_status.worldtag;
	status.posx = old_status.posx;
	status.posy = old_status.posy;
	status.posz = old_status.posz;
	status.contribution = old_status.contribution;
	status.devotion = old_status.devotion;
}

class FetchFrozenIdQuery : public StorageEnv::IQuery
{
public:
	std::set<int> frozen_ids;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);
			int id = -1;
			os_key >> id;
			GRoleBase base;
			os_value >> base;
			if (base.status == _ROLE_STATUS_FROZEN)
				frozen_ids.insert(id);
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "FetchFrozenIdQuery, exception\n" );
//			throw e;
		}
		return true;
	}
};

class FetchCrssvrTeamsZoneidQuery : public StorageEnv::IQuery
{
public:
	int new_zoneid;
	std::map<int, int> change_zoneid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int teamid;
			GCrssvrTeamsData crssvrteams;
			Marshal::OctetsStream(key) >> teamid;
			if(teamid <= 0) return true;
			Marshal::OctetsStream(value) >> crssvrteams;
			int old_zoneid = crssvrteams.info.zoneid;

			LOG_TRACE("FetchCrssvrTeamsZoneidQuery old_zoneid=%d, new_zoneid=%d, teamid=%d", old_zoneid, new_zoneid, teamid);
			if(new_zoneid != old_zoneid)
			{
				change_zoneid.insert(std::make_pair(teamid, new_zoneid));
			}
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "FetchCrssvrTeamsZoneidQuery, exception\n" );
//			throw e;
		}
		return true;
	}
};

void ChangeCrssvrTeamsZoneid(std::map<int, int>& change_zoneid, TableWrapper& tab_crssvrteams)
{
	try
	{
		StorageEnv::CommonTransaction txn;
		try
		{
			std::map<int, int>::iterator bit, bie=change_zoneid.end();
			for (bit=change_zoneid.begin(); bit != bie; ++bit)
			{
				Marshal::OctetsStream kteamid, value;
				GCrssvrTeamsData crssvrteams;

				int id = bit->first;
				if(id <= 0) continue;
				kteamid << id;	
				//LOG_TRACE("ChangeCrssvrTeamsZoneid teamid=%d, new_zoneid=%d", id, bit->second);

				value.clear();
				if( tab_crssvrteams.Find( kteamid, value ) )
				{
					value >> crssvrteams;
					crssvrteams.info.zoneid = bit->second;
					tab_crssvrteams.Put(kteamid, Marshal::OctetsStream()<<crssvrteams);
					LOG_TRACE("ChangeCrssvrTeamsZoneid teamid=%d, new_zoneid=%d", id, crssvrteams.info.zoneid);
				}
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}

		tab_crssvrteams.Checkpoint();
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ChangeCrssvrTeamsZoneid error, what=%s\n", e.what() );
		return;
	}
	StorageEnv::checkpoint();
}

void ChangeCrssvrTeamsZoneid(std::map<int, int>& change_zoneid, StorageEnv::Storage *pcrssvrteams)
{
	if(!pcrssvrteams) return;
	try
	{
		StorageEnv::CommonTransaction txn;
		//StorageEnv::Storage *pcrssvrteams = StorageEnv::GetStorage("crssvrteams");

		try
		{
			std::map<int, int>::iterator bit, bie=change_zoneid.end();
			for (bit=change_zoneid.begin(); bit != bie; ++bit)
			{
				Marshal::OctetsStream kteamid, value;
				GCrssvrTeamsData crssvrteams;

				int id = bit->first;
				if(id <= 0) continue;
				kteamid << id;	

				value.clear();
				if( pcrssvrteams->find( kteamid, value, txn ) )
				{
					value >> crssvrteams;
					crssvrteams.info.zoneid = bit->second;
					pcrssvrteams->insert(kteamid, Marshal::OctetsStream()<<crssvrteams, txn);
					LOG_TRACE("ChangeCrssvrTeamsZoneid teamid=%d, new_zoneid=%d", id, crssvrteams.info.zoneid);
				}

			}
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
		Log::log( LOG_ERR, "ChangeCrssvrTeamsZoneid error, what=%s\n", e.what() );
		return;
	}
	StorageEnv::checkpoint();
}

static bool find(DBStandalone * pdb, StorageEnv::Uncompressor * uncomp, const Octets & key, Octets & value)
{
	size_t value_len;
	if (void *val = pdb->find(key.begin(), key.size(), &value_len))
	{
		uncomp->Update(Octets(val, value_len)).swap(value);
		free(val);
		return true;
	}
	return false;
}

static bool put(DBStandalone * pdb, StorageEnv::Compressor * comp, const Octets & key, const Octets & value)
{
	if (key.size() == 0)
	{
		Log::log(LOG_ERR, "put key.size 0");
		return false;
	}
	Octets com_val = comp->Update(value);
	pdb->put(key.begin(), key.size(), com_val.begin(), com_val.size());
	return true;
}

void AbstractPlayers( const char * srcpath, int zoneid, int new_zoneid)
{
	enum
	{
		cache_low = 20000,
		cache_high = 25000,
	};

	std::string src_dir = srcpath;
	if (!CheckDBType(srcpath))
	{
		Log::log(LOG_ERR, "Check DB type failed");
		return;
	}
	Log::log(LOG_INFO, "FetchFrozenId begin...");
	FetchFrozenIdQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("base");
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
			cursor.walk( q );
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
		Log::log( LOG_ERR, "AbstractPlayers, error when walk for FetchFrozenIdQuery, what=%s\n", e.what() );
	}
	Log::log(LOG_INFO, "FetchFrozenId end, frozen set size %d", q.frozen_ids.size());

	FetchCrssvrTeamsZoneidQuery ct_src;
	ct_src.new_zoneid = new_zoneid;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("crssvrteams");
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
			cursor.walk( ct_src );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}

		ChangeCrssvrTeamsZoneid(ct_src.change_zoneid, pstorage);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "AbstractPlayers, error when walk for FetchCrssvrTeamsZoneidQuery, what=%s\n", e.what() );
	}
	Log::log(LOG_INFO, "FetchCrssvrTeamsZoneidQuery end, change_zoneid size=%d", ct_src.change_zoneid.size());

	FetchCrssvrTeamsZoneidQuery ct_cross;
	ct_cross.new_zoneid = new_zoneid;
	try
	{
		StorageEnv::AtomTransaction	txn;
		TableWrapper tab_crssvrteams((src_dir + "/crssvrteams").c_str());
		
		if(!tab_crssvrteams.Open(cache_high, cache_low, true))
		{
			Log::log(LOG_ERR, "Error: Open DB Failed");
			return;
		}

		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_crssvrteams.name.c_str(), tab_crssvrteams.ptable, tab_crssvrteams.uncompressor);
			cursor.walk( ct_cross );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}

		ChangeCrssvrTeamsZoneid(ct_cross.change_zoneid, tab_crssvrteams);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "AbstractPlayers, error when walk for CrossFetchCrssvrTeamsZoneidQuery, what=%s\n", e.what() );
	}
	Log::log(LOG_INFO, "Cross FetchCrssvrTeamsZoneidQuery end, change_zoneid size=%d", ct_cross.change_zoneid.size());

	StorageEnv::Uncompressor * uncompressor = new StorageEnv::Uncompressor();
	StorageEnv::Compressor * compressor = new StorageEnv::Compressor();

	DBStandalone *	pbase_alone = new DBStandalone( (src_dir+"/base").c_str() );
	DBStandalone *	pbase2_alone = new DBStandalone( (src_dir+"/base2").c_str() );
	DBStandalone *	pstatus_alone = new DBStandalone( (src_dir+"/status").c_str() );
	DBStandalone *	pinventory_alone = new DBStandalone( (src_dir+"/inventory").c_str() );
	DBStandalone *	pstore_alone = new DBStandalone( (src_dir+"/storehouse").c_str() );
	DBStandalone *	ptask_alone = new DBStandalone( (src_dir+"/task").c_str() );
	DBStandalone *	pachieve_alone = new DBStandalone( (src_dir+"/achievement").c_str() );

	pbase_alone->init();
	pbase2_alone->init();
	pstatus_alone->init();
	pinventory_alone->init();
	pstore_alone->init();
	ptask_alone->init();
	pachieve_alone->init();

	int ignore_count = 0;
	int fix_count = 0;
	int process_count = 0;
	bool finished = false;

	std::set<int>::const_iterator it = q.frozen_ids.begin(), ite = q.frozen_ids.end();
	while (!finished)
	{
		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
			StorageEnv::Storage * pachieve = StorageEnv::GetStorage("achievement");
			StorageEnv::CommonTransaction txn;
			try
			{
				int i = 0;
				for (; it != ite && i < CHECKPOINT_THRESHOLD; ++it,++i)
				{
					Marshal::OctetsStream os_key, os_base, os_base2, os_status, os_pocket, os_store, os_task, os_acheive;
					os_key << *it;
					bool src_valid = false;
					GRoleBase base, old_base, src_base;
					GRoleBase2 base2, old_base2;
					GRoleStatus status, old_status;
					GRolePocket pocket;
					GRoleStorehouse store;
					GRoleTask task;
					GRoleAchievement achieve;

					Marshal::OctetsStream(pbase->find(os_key, txn)) >> old_base;
					Marshal::OctetsStream(pbase2->find(os_key, txn)) >> old_base2;
					Marshal::OctetsStream(pstatus->find(os_key, txn)) >> old_status;
					bool need_fix = false;
					if (!find(pbase_alone, uncompressor, os_key, os_base))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->base, need fix", *it);
						need_fix = true;
					}
					else
					{
						os_base >> base;
						src_base = base;
						src_valid = true;
					}
					if (!find(pbase2_alone, uncompressor, os_key, os_base2))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->base2, need fix", *it);
						need_fix = true;
					}
					else
					{
						os_base2 >> base2;
						//与SavePlayerData保持一致
						if (base2.src_zoneid != zoneid)
						{
							Log::log(LOG_ERR, "role %d src_zoneid not match %d:%d, ignore item", *it, base2.src_zoneid, zoneid);
							ignore_count++;
							continue;
						}
						if (base2.data_timestamp <= old_base2.data_timestamp)
						{
							Log::log(LOG_ERR, "role %d data_timestamp invalid %d:%d, need fix", *it, base2.data_timestamp, old_base2.data_timestamp);
							need_fix = true;
						}
					}
					if (!find(pstatus_alone, uncompressor, os_key, os_status))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->status, need fix", *it);
						need_fix = true;
					}
					else
						os_status >> status;
					if (!find(pinventory_alone, uncompressor, os_key, os_pocket))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->inventory, need fix", *it);
						need_fix = true;
					}
					else
						os_pocket >> pocket;
					if (!find(pstore_alone, uncompressor, os_key, os_store))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->storehouse, need fix", *it);
						need_fix = true;
					}
					else
						os_store >> store;
					if (!find(ptask_alone, uncompressor, os_key, os_task))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->task, need fix", *it);
						need_fix = true;
					}
					else
						os_task >> task;
					if (!find(pachieve_alone, uncompressor, os_key, os_acheive))
					{
						Log::log(LOG_ERR, "Can not find %d in centraldb->achievement, need fix", *it);
						need_fix = true;
					}
					else
						os_acheive >> achieve;

					if (need_fix)
					{
						fix_count++;
						base = old_base;
						base.status = _ROLE_STATUS_NORMAL;
					}
					else
					{
						process_count++;
						AbstractPlayerData(base, old_base, base2, old_base2, status, old_status);
					}
					pbase->insert(os_key, Marshal::OctetsStream() << base, txn);
					if (!need_fix)
					{
						pstatus->insert(os_key, Marshal::OctetsStream() << status, txn);
						pbase2->insert(os_key, Marshal::OctetsStream() << base2, txn);
						pinventory->insert(os_key, Marshal::OctetsStream() << pocket, txn);
						pstorehouse->insert(os_key, Marshal::OctetsStream() << store, txn);
						ptask->insert(os_key, Marshal::OctetsStream() << task, txn);
						pachieve->insert(os_key, Marshal::OctetsStream() << achieve, txn);
					}
					if (src_valid)
					{
						//将跨服库中玩家状态置为FROZEN
						src_base.status = _ROLE_STATUS_FROZEN;
						put(pbase_alone, compressor, os_key, Marshal::OctetsStream()<<src_base);
					}
				//	Log::log(LOG_INFO, "Process roleid %d", *it);
				}
				if (it == ite)
					finished = true;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log(LOG_ERR, "Data Error roleid %d", *it);
				DbException ee(DB_OLD_VERSION);
//				txn.abort(ee); //不能执行 abort 否则 事务就回滚了
				throw ee;
			}
		}
		catch (DbException e)
		{
			Log::log(LOG_ERR, "AbstractPlayer, exception what=%s roleid %d", e.what(), *it);
			++it;
			ignore_count++;
			//finished = true;
		}
		StorageEnv::checkpoint();
		pbase_alone->checkpoint();
		Log::log(LOG_INFO, "checkpoint ignore %d records, fix %d records, process %d records",
			ignore_count, fix_count, process_count);
	}

	pbase_alone->checkpoint();

	delete pbase_alone;
	delete pbase2_alone;
	delete pstatus_alone;
	delete pinventory_alone;
	delete pstore_alone;
	delete ptask_alone;
	delete pachieve_alone;

	delete uncompressor;
	delete compressor;

	Log::log(LOG_INFO, "Abstract roles end. ignore %d records, fix %d records, process %d records",
			ignore_count, fix_count, process_count);
}
};//end of GNET
