/* -----------------------------------------------------------------------
**
** FileName: equipscan.cpp
** Author: Houjun (houjun@wanmei.com)
** Date: 2010-08-30
** Description: Scan equipments with specified ID in the gamedbd
**
** ----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ext/hash_map>
#include <string>

#include "conf.h"
#include "log.h"

#include "gamedbmanager.h"
#include "storage.h"
#include "storagetool.h"

#include "roleid"
#include "userid"
#include "groleinventory"
#include "grolepocket"

#include "accessdb.h"
#include "itemowner.h"

namespace GNET
{
struct scan_equip_info
{
	int 		equip_id;
	int 		equip_can_refine;
	std::string equip_name;	
};

typedef __gnu_cxx::hash_map<int, scan_equip_info> ScanEquips;
static ScanEquips scanEquips;
static int player_level = 0;
#define LOGICUID(rid)  (int)((rid) & 0xFFFFFFF0)

bool InitScanEquips(const char* filepath)
{
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;
	char buf[256];
	int cnt = 1;
	while(fgets(buf,sizeof(buf), file))
	{
		int id = 0;
		int can_refine = 0;
		char name[256];
		memset(name, 0, sizeof(name));
		sscanf(buf, "%d,%d,%s", &id, &can_refine, name);
		ScanEquips::iterator it = scanEquips.find(id);
		if(id <= 0 || it != scanEquips.end())
		{
			printf("[ERROR]: Failed to read equip_ids file! Line: %d, id: %d, can_refine: %d, name: %s\n", cnt, id, can_refine, name);
			fclose(file);
			return false;
		}
		scan_equip_info info;
		info.equip_id = id;
		info.equip_can_refine = can_refine;
		info.equip_name = std::string(name);
		scanEquips[id] = info;
		cnt ++;
	}
	fclose(file);
	return true;
}

bool IsInScanEquips(int id, scan_equip_info& info)
{
	ScanEquips::iterator it = scanEquips.find(id);
	if(it != scanEquips.end())
	{
		info = it->second;
		return true;
	}
	return false;
}

bool QueryRoleBase( int roleid, GRoleBase& role)
{
	try
	{
		Marshal::OctetsStream key;
		Marshal::OctetsStream value;
		StorageEnv::CommonTransaction txn;
		try
		{
			StorageEnv::Storage *pbase = StorageEnv::GetStorage("base");
			key << roleid;
			if(pbase->find(key, value, txn))
			{
				value >> role;
				return true;
			}
			else
			{
				//printf("[ERROR]: Not find role base info: %d\n", roleid);
				return false;
			}
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		//printf("[ERROR]: QueryRoleBase failed, roleid=%d, what=%s\n", roleid, e.what());
		return false;
	}
	return true;
}

bool QueryRoleStatus(int roleid, GRoleStatus& status)
{
	try
	{
		Marshal::OctetsStream key;
		Marshal::OctetsStream value;
		StorageEnv::CommonTransaction txn;
		try
		{
			StorageEnv::Storage *pstatus = StorageEnv::GetStorage("status");
			key << roleid;
			if(pstatus->find(key, value, txn))
			{
				value >> status;
				return true;
			}
			else
			{
				//printf("[ERROR]: Not find role status: %d\n", roleid);
				return false;
			}
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		//printf("[ERROR]: QueryRoleStatus failed, roleid=%d, what=%s\n", roleid, e.what());
		return false;
	}
	return true;
}

bool QueryRoleStorehouse(int roleid, GRoleStorehouse& storehouse)
{
	try
	{
		Marshal::OctetsStream key;
		Marshal::OctetsStream value;
		StorageEnv::CommonTransaction txn;
		try
		{
			StorageEnv::Storage *pstorehouse = StorageEnv::GetStorage("storehouse");
			key << roleid;
			if(pstorehouse->find(key, value, txn))
			{
				value >> storehouse;
				return true;
			}
			else
			{
				//printf("[ERROR]: Not find role storehouse: %d\n", roleid);
				return false;
			}
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		//printf("[ERROR]: QueryRoleStoreHouse failed, roleid=%d, what=%s\n", roleid, e.what());
		return false;
	}
	return true;
}

void ScanEquipment(GRoleBase& roleBase, GRoleStatus& roleStatus, GRolePocket& pocket, GRoleStorehouse& storehouse)
{
	int userid = roleBase.userid;
	if(userid == 0)
	{
		userid = (int)(roleBase.id & 0xFFFFFFF0);
	}

	GRoleInventoryVector::iterator it = pocket.items.begin();
	for(; it != pocket.items.end(); ++ it)
	{
		scan_equip_info info;
		if(!IsInScanEquips(it->id, info)) continue;
		size_t size = roleStatus.reborndata.size();
		int count = size / sizeof(int);

		short rf_stone_level = 0;
		if(info.equip_can_refine)
		{
			char* buf = (char*)(it->data.begin());
			buf += sizeof(unsigned short);
			rf_stone_level = *(short*)buf;
		}
		//printf("UserID: %d, RoleID: %d, Level:%d, Reborn: %s, equip_id: %d, equip_name: %s, EquipRefineLevel: %d\n", roleBase.userid, roleBase.id, roleStatus.level, count > 0 ? "true" : "false", it->id, name.c_str(), rf_stone_level);
		printf("%d, %d, %d, %s, %s, %d\n", userid, roleBase.id, roleStatus.level, count > 0 ? "yes" : "no", info.equip_name.c_str(), rf_stone_level);
	}

	it = pocket.equipment.begin();
	for(; it != pocket.equipment.end(); ++ it)
	{
		scan_equip_info info;
		if(!IsInScanEquips(it->id, info)) continue;
		size_t size = roleStatus.reborndata.size();
		int count = size / sizeof(int);

		short rf_stone_level = 0;
		if(info.equip_can_refine)
		{
			char* buf = (char*)(it->data.begin());
			buf += sizeof(unsigned short);
			rf_stone_level = *(short*)buf;
		}
		//printf("UserID: %d, RoleID: %d, Level:%d, Reborn: %s, equip_id: %d, equip_name: %s, EquipRefineLevel: %d\n", roleBase.userid, roleBase.id, roleStatus.level, count > 0 ? "true" : "false", it->id, name.c_str(), rf_stone_level);
		printf("%d, %d, %d, %s, %s, %d\n", userid, roleBase.id, roleStatus.level, count > 0 ? "yes" : "no", info.equip_name.c_str(), rf_stone_level);
	}

	it = storehouse.items.begin();
	for(; it != storehouse.items.end(); ++ it)
	{
		scan_equip_info info;
		if(!IsInScanEquips(it->id, info)) continue;
		size_t size = roleStatus.reborndata.size();
		int count = size / sizeof(int);

		short rf_stone_level = 0;
		if(info.equip_can_refine)
		{
			char* buf = (char*)(it->data.begin());
			buf += sizeof(unsigned short);
			rf_stone_level = *(short*)buf;
		}
		//printf("UserID: %d, RoleID: %d, Level:%d, Reborn: %s, equip_id: %d, equip_name: %s, EquipRefineLevel: %d\n", roleBase.userid, roleBase.id, roleStatus.level, count > 0 ? "true" : "false", it->id, name.c_str(), rf_stone_level);
		printf("%d, %d, %d, %s, %s, %d\n", userid, roleBase.id, roleStatus.level, count > 0 ? "yes" : "no", info.equip_name.c_str(), rf_stone_level);
	}
}

class EquipScanQuery : public StorageEnv::IQuery
{
public:
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

			GRoleBase roleBase;
			QueryRoleBase(roleid.id, roleBase);
			
			GRoleStatus roleStatus;
			QueryRoleStatus(roleid.id, roleStatus);
	
			if(roleStatus.level >= player_level)
			{
				value_os >> pocket;
				GRoleStorehouse storehouse;
				QueryRoleStorehouse(roleid.id, storehouse);
				ScanEquipment(roleBase, roleStatus, pocket, storehouse);
			}
			return true;
		} catch ( Marshal::Exception & ) {
			//printf("[ERROR]: PocketQuery, error unmarshal, roleid=%d", roleid.id);
			return true;
		}
		return true;
	}
};

void EquipScan(const char* equip_ids_file, int level)
{

	if(!InitScanEquips(equip_ids_file))
	{
		return;
	}	
	player_level = level;
	try
	{
		EquipScanQuery qp;
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("inventory");
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.browse( qp );
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		//printf("[ERROR]: Equip scan failed, what=%s\n", e.what());
	}
}

// 返回true 需要过滤，返回false 不需要过滤
bool FilterEquipRoleScan(int roleid, int need_level, int diff_mon)
{
	GRoleBase roleBase;
	QueryRoleBase(roleid, roleBase);
	
	GRoleStatus roleStatus;
	QueryRoleStatus(roleid, roleStatus);

	int player_level = roleStatus.level + roleStatus.reborndata.size()/sizeof(int)*200;
	time_t now = Timer::GetTime();
	//LOG_TRACE("roleid=%d, player_level=%d, need_lvl=%d, diff_mon=%d, now=%d, lastlogin=%d",	roleid, player_level, need_level, diff_mon, now, roleBase.lastlogin_time);

	if(need_level == 0 && diff_mon == 0)
	{
		return false;
	}
	else if(need_level == 0 && diff_mon != 0)
	{
		// 只过滤最近登录时间大于diff_mon的
		if(now - roleBase.lastlogin_time > (86400 * diff_mon))
		{
			return true;
		}
		return false;
	}
	else if(need_level != 0 && diff_mon == 0)
	{
		// 只过滤等级
		if(player_level < need_level)
		{
			return true;
		}
		return false;
	}
	else/*(player_level != 0 && diff_mon != 0)*/
	{
		// 过滤两个条件都不符合的
		//LOG_TRACE("roleid=%d, diff_time=%d, diff_mon=%d", roleid, now - roleBase.lastlogin_time, 86400 * diff_mon );
		if((now - roleBase.lastlogin_time) > (86400 * diff_mon) || player_level < need_level)
		{
			return true;
		}
		//LOG_TRACE("roleid=%d, player_level=%d, need_lvl=%d, diff_mon=%d, now=%d, lastlogin=%d",	roleid, player_level, need_level, diff_mon, now, roleBase.lastlogin_time);
		return false;
	}

	return true;
}

class EquipRoleScanQuery : public StorageEnv::IQuery
{
public:
	int player_level;
	int diff_mon; // 角色上次登录时间与当前系统时间差值，单位为天 

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
			value_os >> pocket;
		} catch ( Marshal::Exception & ) {
			//printf("[ERROR]: EquipROleQuery, error unmarshal, roleid=%d", roleid.id);
			return true;
		}
	
		if(FilterEquipRoleScan(roleid.id, player_level, diff_mon))
		{
			return true;
		}

		SEquipsRole equipRole;
		memset(&equipRole, 0, sizeof(SEquipsRole));

		GRoleInventoryVector::iterator it = pocket.equipment.begin();
		for(; it != pocket.equipment.end(); ++ it)
		{
			GRoleInventory& inventory = *it;

			equip_essence_new equip_new;
			char* buf = (char*)(it->data.begin());
			
			if(it->data.size() == sizeof(equip_essence_new))
			{
				memcpy(&equip_new, buf, sizeof(equip_essence_new));
				if(inventory.pos == EQUIP_INDEX_HEAD)
				{
					equipRole.equip_head.id = inventory.id;
					equipRole.equip_head.rf_stone_level = equip_new.rf_stone_level;
				}
				if(inventory.pos == EQUIP_INDEX_BODY)
				{
					equipRole.equip_body.id = inventory.id;
					equipRole.equip_body.rf_stone_level = equip_new.rf_stone_level;
				}
				if(inventory.pos == EQUIP_INDEX_FOOT)
				{
					equipRole.equip_foot.id = inventory.id;
					equipRole.equip_foot.rf_stone_level = equip_new.rf_stone_level;
				}
				if(inventory.pos == EQUIP_INDEX_WEAPON)
				{
					equipRole.equip_weapon.id = inventory.id;
					equipRole.equip_weapon.rf_stone_level = equip_new.rf_stone_level;
				}
				//printf("new struct head_lvl=%d, body_lvl=%d, foot_lvl=%d, weapon_lvl=%d\n", 
				//		equip_new.rf_stone_level, equip_new.rf_stone_level, equip_new.rf_stone_level, equip_new.rf_stone_level);
			}
			else
			{
				buf += sizeof(unsigned short);
				if(inventory.pos == EQUIP_INDEX_HEAD)
				{
					equipRole.equip_head.id = inventory.id;
					equipRole.equip_head.rf_stone_level = *(short*)buf;
				}
				if(inventory.pos == EQUIP_INDEX_BODY)
				{
					equipRole.equip_body.id = inventory.id;
					equipRole.equip_body.rf_stone_level = *(short*)buf;
				}
				if(inventory.pos == EQUIP_INDEX_FOOT)
				{
					equipRole.equip_foot.id = inventory.id;
					equipRole.equip_foot.rf_stone_level = *(short*)buf;
				}
				if(inventory.pos == EQUIP_INDEX_WEAPON)
				{
					equipRole.equip_weapon.id = inventory.id;
					equipRole.equip_weapon.rf_stone_level = *(short*)buf;
				}
				//printf("old struct head_lvl=%d, body_lvl=%d, foot_lvl=%d, weapon_lvl=%d\n", 
				//		equipRole.equip_head.rf_stone_level,equipRole.equip_body.rf_stone_level,equipRole.equip_foot.rf_stone_level,equipRole.equip_weapon.rf_stone_level);
			}

			if(inventory.pos == EQUIP_INDEX_TALISMAN1)
			{
				equipRole.talisman1_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_NECK)
			{
				equipRole.neck_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_FINGER1)
			{
				equipRole.finger1_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_FINGER2)
			{
				equipRole.finger2_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_XITEM3)
			{
				equipRole.xitem3_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_FASHION_BACK)
			{
				equipRole.back_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_XITEM2)
			{
				equipRole.xitem2_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_XITEM1)
			{
				equipRole.xitem1_id = inventory.id;
			}
			if(inventory.pos == EQUIP_INDEX_FASHION_LIP)
			{
				equipRole.lip_id = inventory.id;
			}
		}

		printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",  roleid.id, equipRole.equip_head.id, equipRole.equip_head.rf_stone_level, equipRole.equip_body.id, equipRole.equip_body.rf_stone_level, equipRole.equip_foot.id, equipRole.equip_foot.rf_stone_level, equipRole.equip_weapon.id, equipRole.equip_weapon.rf_stone_level
				, equipRole.talisman1_id, equipRole.neck_id, equipRole.finger1_id, equipRole.finger2_id, equipRole.xitem3_id
				, equipRole.back_id, equipRole.xitem2_id, equipRole.xitem1_id, equipRole.lip_id);

		return true;
	}
};

void EquipRoleScan(int player_level, int diff_mon)
{
	EquipRoleScanQuery qp;
	qp.player_level = player_level;
	qp.diff_mon = diff_mon;
	try
	{
		printf("角色id, 帽子id, 等级, 衣服id, 等级, 鞋子id, 等级, 法宝, 项链, 左戒指, 右戒指, 罡气, 护符, 玺绶, 锦囊, 法身\n");

		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("inventory");
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( qp );
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		//printf("[ERROR]: EquipRolescan failed, what=%s\n", e.what());
		return;
	}
}

}
