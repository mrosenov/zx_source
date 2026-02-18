#ifndef __GNET_ACCESSDB_H
#define __GNET_ACCESSDB_H

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "log.h"
#include "dbbuffer.h"

#include "stocklog"
#include "user"
#include "roleid"
#include "groleforbid"
#include "grolebase"
#include "groleinventory"
#include "grolestatus"
#include "grolestorehouse"
#include "grolepocket"
#include "gshoplog"
#include "groledetail"
#include "gmember"
#include "gfolk"
#include "gfamilyskill"
#include "gfamily"
#include "familyid"
#include "hostilefaction"
#include "hostileinfo"
#include "gfactioninfo"
#include "guserfaction"
#include "waitdelkey"

#include "gamedbmanager.h"
#include "timer.h"
#include "storagewdb.h"

namespace GNET
{
class TableWrapper  //目前不支持实例的赋值 !!
{
	//目前不支持实例复制
	TableWrapper(const TableWrapper &);
	TableWrapper & operator = (const TableWrapper &);
public:
	string name;//完整相对路径
	bool checkpoint_on_destruct;
	DBStandalone *ptable;
	StorageEnv::DataCoder * uncompressor;
	StorageEnv::DataCoder * compressor;
	TableWrapper(const string & str = "") : name(str), checkpoint_on_destruct(false), ptable(NULL)
	{
		uncompressor = new StorageEnv::Uncompressor();
		compressor = new StorageEnv::Compressor();
	}
	void SetCompressor( StorageEnv::DataCoder *c, StorageEnv::DataCoder *u )
	{
		delete uncompressor;
		delete compressor;
		compressor = c;
		uncompressor = u;
	}
	void SetName(const string & str)
	{
		name = str;
	}
	~TableWrapper() 
	{
		if (checkpoint_on_destruct)
			Checkpoint();
		if (ptable)
			delete ptable;
		ptable = NULL;
		delete uncompressor;
		delete compressor;
	}
	bool Open(size_t high=4096, size_t low=3072, bool cp_on_dest = false)
	{
		if (ptable != NULL)
			return false;
		ptable = new DBStandalone(name.c_str(), high, low);	
		if (!ptable)
			return false;
		checkpoint_on_destruct = cp_on_dest;
		return ptable->init();
	}
	bool IsOpen() { return ptable != NULL; }
	/*
	bool ReOpen(size_t high=4096, size_t low=3072, bool cp_on_dest = false)
	{
		if (ptable)
		{
			time_t begin = time(NULL);
			LOG_TRACE("%s beore checkpoint time:%s", name.c_str(), ctime(&begin));
			ptable->performance_dump();
			if (checkpoint_on_destruct)
				Checkpoint();
			ptable->performance_dump();
			delete ptable;
			ptable = NULL;
		}
		ptable = new DBStandalone(name.c_str(), high, low);	
		if (!ptable)
			return false;
		checkpoint_on_destruct = cp_on_dest;
		return ptable->init();
	}
	*/
	bool Find(const Octets & key, Octets & value)
	{
		size_t value_len;
		if (void *val = ptable->find(key.begin(), key.size(), &value_len))
		{
			uncompressor->Update(Octets(val, value_len)).swap(value);
			free(val);
			return true;
		}       
		return false;
	}
	bool FindCompressed(const Octets & key, Octets & value)
	{
		size_t value_len;
		if (void *val = ptable->find(key.begin(), key.size(), &value_len))
		{
			value = Octets(val, value_len);
			free(val);
			return true;
		}       
		return false;
	}
	bool Put(const Octets & key, const Octets & value)
	{
		if (key.size() == 0)
		{
			Log::log(LOG_ERR, "put key.size 0");
			return false;
		}
		Octets com_val = compressor->Update(value);
		ptable->put(key.begin(), key.size(), com_val.begin(), com_val.size());
		return true;
	}
	bool PutCompressed(const Octets & key, const Octets & value)
	{
		if (key.size() == 0)
		{
			Log::log(LOG_ERR, "put key.size 0");
			return false;
		}
		ptable->put(key.begin(), key.size(), value.begin(), value.size());
		return true;
	}
	void Del(const Octets & key)
	{
		ptable->del(key.begin(), key.size());
	}
	bool Checkpoint()
	{
		time_t begin = time(NULL);
		LOG_TRACE("checkpoint table %s begin time:%s", name.c_str(), ctime(&begin));
		if (ptable)
		{
			ptable->checkpoint();
			time_t end = time(NULL);
			LOG_TRACE("checkpoint table %s end time:%s %d seconds", name.c_str(), ctime(&end), end-begin);
		}
		return true;
	}
	size_t Count()
	{
		return ptable->record_count();
	}
/*
	void Close()
	{
		if(pdst) delete pdst;
		if(pbak) delete pbak;
		pdst = NULL;
		pbak = NULL;
	}
	size_t BakSize()
	{
		return pbak ? pbak->record_count():0;
	}
*/
        void Rebuild()
        {
		time_t begin = time(NULL);
		LOG_TRACE("rebuild table %s begin time:%s", name.c_str(), ctime(&begin));
		string bakname = name + "-bk";
		PageRebuild rebuild(bakname.c_str(), name.c_str(), 65535);
		size_t corrupt_count;
		size_t record_count = rebuild.action(&corrupt_count);
		rename(bakname.c_str(), name.c_str());
		time_t end = time(NULL);
		LOG_TRACE("rebuild table %s in %d seconds, record count=%d", name.c_str(), (int)(end-begin), record_count);
		if (corrupt_count)
			Log::log(LOG_ERR, "rebuild table %s corrupt count=%d", name.c_str(), corrupt_count);
        }
};

void PreLoadTable(const char* path);
void PrintLogicuid( );
void PrintUnamerole( int zoneid );
void PrintUnamefaction( int zoneid );
void PrintUnamefamily( int zoneid );
void GenNameIdx( );
void ExportUnique( int zoneid );

bool QueryRole( int roleid );

bool ImportCsvUserIdExchange( const char * filename );
void ExportCsvUserIdExchange( const char * srcpath );
void CmpUserIdExchange( const char * srcpath );

bool ExportRole( int roleid );
bool ExportUser( int userid );

void ListRole( );
void ListRoleBrief(const char * date);
void ListUserBrief( );
void ListFaction( );
void ListFamily( );
void ListFamilyUser( );
void ListShopLog();
void ListSysLog();
void ListRoleMoney(int zoneid);
void ListDeityRole(int zoneid);
void ListRoleInventory(int roleid); //roleid = 0 表示扫描全库
void ListPKBet(int zoneid);

void UpdateRoles( );
void ConvertDB( );
void RepairDB( );

void MergeDB( const char * srcpath, const char * srcdbname, const char * destdbname );
void MergeDBAll( const char * srcpath, bool force = false );

void GetTimeoutRole( WaitDelKeyVector& rolelist );
void SetCashInvisible(const char* file);

void ImportRoleList(const char *rolelistfiles[], int filecount);
void ExportRoleList(const char *roleidfile, const char *rolelistfile);

void VerifyDB( );
void ListTerritory();
void EquipScan(const char* equip_ids_file, int level);
void AbstractPlayers(const char * srcpath, int zoneid, int new_zoneid );
void DelFrozenPlayers(const char * dbdata, int day);
void DelZonePlayers(const char * dbdata, int zoneid);
void AllocNewLogicuid(const char * useridfile, int default_logicuid_start);
bool SyncNameChange(const char * dbdata);
void VerifyName(const char * dbdata);
void MoveDBTo(const char * unique_dbhome, const char * srcdbhome, int zoneid);
void WalkTables();
void ListDupRolename();
void ListBase2(int zoneid);
void ListItemContainer();
void ListCrssvrteamsRole();
void RepairFaName();
void RewriteFlowerRoleid(const char * roleidmapfile);
void FixCityStore(const char * oldcitystore, const char * roleidmapfile, const char * factionidmapfile);
void RewriteFlowerRoleid(const char * roleidmapfile);
void FixXueqiItemOwner(const char *);
int GetDBType(TableWrapper & tab_config);
void DelInactiveUsers(const char * dbdata, int day);

bool ImportStockCancelUserId( const char* filename, std::vector<int>& userid_vec );
void RepairStockCancel(const char* filename);
void GetCashItems(const GRolePocket & pocket, std::map<int, int> & out);
void GetCashItems(const GRoleStorehouse & store, std::map<int, int> & out);
void GetReferrals(int roleid);
void GetReferrer(int zoneid, const char * roleidfile);
bool FilterEquipRoleScan(int roleid, int player_level, int diff_mon);
void EquipRoleScan(int player_level, int diff_mon);
void RewriteBase2();
void CheckAbnormalRole();
void RepairItemOwner( int old_roleid, int new_roleid ); // Youshuang add
void RepairItemOwner_FromFile( const char* badfile, int zoneid ); // Youshuang add
}

#endif

