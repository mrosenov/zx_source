#ifndef __GNET_FACBASE_MANAGER_H__
#define __GNET_FACBASE_MANAGER_H__

#include "log.h"
#include <map>
#include "gproviderserver.hpp"
#include "gtoptable"
namespace GNET
{
	/*管理一条线上的实例数*/
	class FBaseLineNumber
	{
		const int max_number;//max instance number, this line
		int running_number;
//		int applying_number;
	public:	
		FBaseLineNumber(int max):max_number(max), running_number(0)/*, applying_number(0)*/{}
		void Free()
		{
			running_number--;
			LOG_TRACE("FacBase after free, running_number=%d max_number=%d", running_number, max_number);
		}
		bool Alloc()
		{
			if (GetFree() > 0)
			{
				running_number++;
				LOG_TRACE("alloc facbase, running_number %d max_number %d", running_number, max_number);
				return true;
			}
			else
				return false;
		}
		int GetFree()
		{
			return max_number - running_number;
		}
		int GetMax()
		{
			return max_number;
		}
	};
	
	class FacBaseInfo
	{
		enum
		{
			SWITCH_TIMEOUT = 40,
			CLOSING_TIME = 180,	//gs 要先踢人 再关闭
			DISCONNECT_TIMEOUT = 180, //断线后多久自动恢复状态
		};
		enum
		{
			STATUS_INIT = 0,//已占用坑位，但未申请gs 假设gs一共允许开放100个基地。
			STATUS_ALLOCATING = 1, //正在分配gs
			STATUS_ALLOCATE = 2,//已分配gs
			STATUS_CLOSING = 3, //帮派置为删除状态 关闭基地			
			STATUS_DISCONNECT = 4,///与基地 gs 连接断开，维持此状态等待基地 gs 存盘成功
		};
		int gsid;
		int index;
		int fid;
		int status;
		int status_timeout;
		int noti_roleid;
		std::set<int> mall_indexes;
	public:
		FacBaseInfo(int faction) : gsid(0), index(-1), fid(faction), status(STATUS_INIT), status_timeout(0), noti_roleid(0) {}
		void CheckStatusChange();
		bool IsAllocated()
		{
			CheckStatusChange();
			return status == STATUS_ALLOCATE;
		}
		bool IsFree() 
		{
			CheckStatusChange();
			return status == STATUS_INIT;
		}
		void TryStart(int gs, int roleid);
		void SetStatus(int st);
		void OnStart(int retcode, int faction, int gs, int in, const std::set<int> & mall);
//		int Enter(int & gs, int & in);
		int GetIndex(int & gs, int & in);
		int GetGS() { return gsid; }
		void OnDisconnect(int gs);
		void OnMallChange(const std::set<int> & mall);
		const std::set<int> & GetMall() const { return mall_indexes; }
		void OnFactionDel();
		void Reset()
		{
			gsid = 0;
			index = -1;
			noti_roleid = 0;
			mall_indexes.clear();
		}
		int GetStatus() { return status; }
		bool OnStop(int retcode, int faction, int gs, int in);
	};
	class FacBaseManager //帮派基地管理器
	{
	public:
		enum
		{
			CARE_TOP_NUM = 100, //排行榜前100名的帮派有强制进入基地的权利
		};
	private:
		typedef std::map<int/*gsid*/, FBaseLineNumber> LineNumber;
		LineNumber line_num; //每条线上的数量管理器
		int total_num; //所有线上可分配的基地总和
		typedef std::map<int/*fid*/, FacBaseInfo> BaseMap;
		BaseMap base_map; //里面包含未分配 gs 的基地
//		typedef std::set<int/*fid*/> FactionInTop; //帮派活跃度周排行榜前 100 名
//		FactionInTop fac_top;
		bool load_top;

		FacBaseManager() : total_num(0), load_top(false) {};
	public:
		void RegisterServer(int gs_id, int count);
		bool TryAllocGS(int & gs);
		void OnActTopUpdate(const GTopTable & weektop);
		int TryStartBase(int faction, int noti_roleid=0);
		FacBaseInfo * FindBase(int faction);
		void OnBaseStart(int retcode, int fid, int gsid, int index, const std::set<int> & mall);
		static FacBaseManager * GetInstance()
		{
			static FacBaseManager inst;
			return &inst;
		}
		void OnDisconnect(int gs);
		int GetBaseIndex(int fid, int & gs, int & index);
		/*
		int EnterBase(int fid, int & gs, int & index);
		int OnBasePropChange(int fid, int & base_gs, int & base_index);
		int OnBaseBuildingProgress(int fid, int & base_gs, int & base_index);
		*/
		void OnMallChange(int fid, const std::set<int> & mall);
		void OnGSConnect(unsigned int sid);
		void OnFactionDel(int fid);
		void OnBaseStop(int retcode, int fid, int gsid, int index);
		void DebugSetLimit(int n) { total_num = n; }
		int GetCurNum() { return base_map.size(); }
	};
} // namespace GNET

#endif
