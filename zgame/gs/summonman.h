#ifndef __ONLINE_GAME_GS_SUMMON_MAN_H__
#define __ONLINE_GAME_GS_SUMMON_MAN_H__

#include <amemory.h>
#include <vector.h>
#include <common/types.h>
#include <lua.hpp>
#include "property.h"

struct summon_data
{
	int id;
	int type;
	int tid;
	int summon_stamp;
	int cur_notify_counter;
	int heartbeat_stamp;
	bool is_invisible;
	A3DVECTOR pos;
	int world_id;
};

class gsummon_imp;
class gplayer_imp;
class world_manager;

class summon_manager
{
private:
	friend class object_interface;
	friend class gsummon_imp;

	abase::vector<summon_data, abase::fast_alloc<> > _summon_list;
	
	int _heartbeat_counter;
	bool _clone_exist_flag;
	
	enum SUMMON_TYPE
	{
		SUMMON_TYPE_NONE = 0,
		SUMMON_TYPE_ANIMAL = 1,
		SUMMON_TYPE_PLANT = 2,
		SUMMON_TYPE_CLONE = 3,
		SUMMON_TYPE_SKILLOBJECT = 4,
		SUMMON_TYPE_MACHINE = 5,	//»ú¹Ø 
		SUMMON_TYPE_TRAP = 6,		//ÏÝÚå
		SUMMON_TYPE_CONTROL_TRAP = 7,	//¿É¿ØÖÆÏÝÚå
		SUMMON_TYPE_TELEPORT1 = 8,	//×£¸£´«ËÍ
		SUMMON_TYPE_TELEPORT2 = 9,	//×çÖä´«ËÍ
	};

	enum
	{
		MAX_SUMMON_COUNT = 6,
		MAX_SUMMON_LIFETIME = 3600,
		NOTIFY_MASTER_TIME = 15,
	};

	void PlayerSendAllMessage(gplayer_imp * pImp, int msg_id, int param = 0, const void * content = NULL,size_t content_length = 0);
	void PlayerSendMessageTo(gplayer_imp *pImp, int msg_id, int id, int param = 0, const void * content = NULL,size_t content_length = 0);
	void UpdateNotifyCounter(int counter);
	void CheckNotifyCounter(gplayer_imp *pImp);
	void CheckSummonStatus(gplayer_imp *pImp);

public:
	summon_manager()
	{
		_heartbeat_counter = 0;
		_clone_exist_flag = false;
	}

	void Swap(summon_manager& rhs)
	{
		_summon_list.swap(rhs._summon_list);
		std::swap(_heartbeat_counter, rhs._heartbeat_counter);
		std::swap(_clone_exist_flag, rhs._clone_exist_flag);
	}
	
	bool IsCloneExist();
	XID GetCloneID();
	XID GetSummonID();
	void UpdateCloneStatus(gplayer_imp *pImp);
	void RemoveCloneFilter(gplayer_imp *pImp);
	bool IsSummonExist(gplayer_imp *pImp, int tid, XID& id);
	int GetSummonNPCCountByType(char type);
	int GetSummonNPCCountByID(int id);
	
	bool TestCanSummonNPC(gplayer_imp* pImp,A3DVECTOR& pos);
	void InsertSummonNPC(int id, char type, int tid, int timestamp, bool is_invisible, A3DVECTOR& pos, int world_id);
	void RemoveCloneNPC(gplayer_imp *pImp);
	void ClearSummonNPCByType(gplayer_imp *pImp, char type);
	void ClearSummonNPCByType(gplayer_imp * pImp, char type, int count);
	void ClearSummonNPCByID(gplayer_imp * pImp, int tid, int count);
	void ClearAllSummonNPC(gplayer_imp * pImp);
	void ControlTrap(gplayer_imp * pImp, int id, int tid);

	void PreSummonNPC(gplayer_imp * pImp, char type, int tid, int count, int max_count); 
	void PostSummonNPC(gplayer_imp * pImp, char type, int id, int tid, int lifetime, int skill_id, int skill_cd, A3DVECTOR& pos); 

	void Heartbeat(gplayer_imp* pImp);

	void PlayerBeAttacked(gplayer_imp* pImp,const XID& attacker);
	void NotifyStartAttack(gplayer_imp* pImp,const XID& target,char force_attack);
	void NotifyDuelStop(gplayer_imp *pImp);
	bool SummonNPCDeath(gplayer_imp* pImp,const XID& who, int type, int summon_stamp);
	void NotifyMasterInfo(gplayer_imp* pImp, leader_prop & data);
	bool SummonRelocatePos(gplayer_imp* pImp,const XID& who, int stamp,bool force_disappear = false);
	bool SummonHeartbeat(gplayer_imp * pImp, const XID& who, int type);

	void OnLeaveMap(gplayer_imp * pImp);
	void OnLeaveWorld(gplayer_imp * pImp);
	
	static bool InitFromCaster(gsummon_imp * sImp, gplayer_imp * pImp, int id, int skill_level, int index);
	static bool FindGroundPos(world_manager* manager,A3DVECTOR& pos);

	inline bool HasSummonNPC()
	{
		return _summon_list.size() != 0;
	}
	
};



#endif
