#ifndef __GNET_GAMEDBMANAGER_H
#define __GNET_GAMEDBMANAGER_H
/*		this class provide management tools for gamedb, such as:
 *	   	1. initialize tab and data
 *		2. query data
 *		3. show statistic informations
 */

#include <vector>
#include <map>

#include "thread.h"
#include "conv_charset.h"
#include "statistic.h"

#include "groleforbid"
#include "grolebase"
#include "groleinventory"
#include "grolepocket"
#include "grolestatus"
#include "gshoplog"
#include "groledetail"
#include "roleid"
#include "stocklog"
#include "user"
#include "userid"
#include "localmacro.h"
#include "dbconfig"

#include "map.h"

#define ROLELIST_DEFAULT	0x80000000
#define MAX_ROLE_COUNT		16
namespace GNET
{
	class RoleList
	{
		unsigned int rolelist;
		int count;
		int cur_role;
	public:
		RoleList() : rolelist(0),count(0),cur_role(0) { }
		RoleList(unsigned int r) : rolelist(r),count(0),cur_role(0) { }
		~RoleList() { }
		void operator=(const RoleList& rhs) { rolelist=rhs.rolelist; }
		bool IsRoleListInitialed()
		{
			return (rolelist & ROLELIST_DEFAULT) != 0;
		}
		bool IsRoleExist(int roleid)
		{
			return (rolelist & (1<<(roleid % MAX_ROLE_COUNT))) != 0;
		}
		void InitialRoleList()
		{
			rolelist = ROLELIST_DEFAULT;
		}
		unsigned int GetRoleList() 
		{
			return rolelist;
		}
		int GetRoleCount()
		{
			if (!IsRoleListInitialed()) return -1;  //rolelist is not initialized
			count=0;
			for (int id=0;id<MAX_ROLE_COUNT;id++)
			{
				if (IsRoleExist(id)) count++;
			}
			return count;
		}
		int AddRole()
		{
			if (!IsRoleListInitialed()) { return -1; }
			if (GetRoleCount()==MAX_ROLE_COUNT) { return -1; }
			int id=0;
			for (;id<MAX_ROLE_COUNT && IsRoleExist(id);id++);
			rolelist +=(1<<id);
			return id;	
		}		
		int AddRole(int roleid)
		{
			if (!IsRoleListInitialed()) { return -1; }
			if (IsRoleExist(roleid)) { return roleid; } //the role will be overlayed
			if (GetRoleCount()==MAX_ROLE_COUNT) { return -1; }
			if (roleid<0 || roleid >MAX_ROLE_COUNT-1) { return -1;}
			rolelist +=(1<<roleid);  
			return roleid;
		}
		bool DelRole(int roleid)
		{
			if (!IsRoleListInitialed()) return false;
			if (!IsRoleExist(roleid)) return false;
			return (rolelist -= 1<<(roleid % MAX_ROLE_COUNT)) != 0;
		}
		int GetNextRole()
		{
			while (cur_role<MAX_ROLE_COUNT)
				if (IsRoleExist(cur_role++)) return cur_role-1;
			return -1;
		}
		void SeekToBegin()
		{
			cur_role=0;
		}
	};

	#define	OBJECT_COUNT	4096
	#define	MATERIAL_COUNT	5
	class GameDBManager
	{
		int areaid;
		int zoneid;
		int deletetimeout;
		int max_delta;
		int create_count;
		int64_t guid;
		bool is_central_db;
		Thread::Mutex	locker;
		//Thread::Mutex	locker_region;
		DBConfig config;
		GameDBManager() 
		{
			deletetimeout = 604800;	// a week
			max_delta = 0;
			guid = 0;
			areaid = 0;
			zoneid = 0;
			create_count = 0;
			is_central_db = false;
		}
	public:
		~GameDBManager() { }
		static GameDBManager* GetInstance() { static GameDBManager instance; return &instance;}
		//initialize role base information when they are created
		bool InitGameDB();
		int GetDeleteTimeout() {return deletetimeout;}
		void InitGUID();
		int64_t GetGUID() { return guid++; }
		static int Zoneid() { return GetInstance()->zoneid; }
		static int Areaid() { return GetInstance()->areaid; }
		//list 
		void ListUser() { }
		void ListUser(const UserID& userid) { }
		void ListRole() { }
		void ListRole(const RoleId& roleid) { }
		//rase details
		bool SaveUserDefault( );
		bool SaveClsDetail(unsigned int id, char gender, GRoleDetail &role);
		bool GetClsDetail(unsigned int id, char gender, GRoleBase &base, GRoleStatus &status,
					GRolePocket &inventory, GRoleStorehouse &storehouse);
		bool GetClsPos(unsigned int id, char gender, float &posx, float &posy, float &posz);
		void UpdateMoney(int roleid, int delta)
		{
			if(delta>max_delta)
			{
				LOG_TRACE("UpdateMoney, update max delta roleid=%d", roleid);
				max_delta = delta;
			}

			STAT_MIN5("Money",delta);
		}
		void UpdateCash( int delta )
		{
			STAT_MIN5("Cash",delta);
		}
		void UpdateTradeMoney( int money1, int money2 )
		{
			STAT_MIN5("TradeMoney", (int64_t)money1 + money2 );
		}
		bool MustDelete(const GRoleBase& base)
		{
			return (base.status == _ROLE_STATUS_READYDEL && Timer::GetTime()-base.delete_time > deletetimeout);
		}
		void ReadConfig();
		void SaveConfig();
		bool NewServer()
		{
			return config.open_time==0 || Timer::GetTime()-config.open_time < 86400;
		}
		void OnCreateRole();
		bool IsCentralDB() { return is_central_db; }
		void SetCentralDB(bool b) { is_central_db = b; }

		bool CheckForbidUserLogin(const User &user, time_t & left_time);

	};

	void GRoleBaseToDetail( const GRoleBase & base, GRoleDetail & detail );
	void GRoleDetailToBase( const GRoleDetail & detail, GRoleBase & base );
};
#endif

