#ifndef __ONLINEGAME_GS_FORBID_MANAGER_H__
#define __ONLINEGAME_GS_FORBID_MANAGER_H__

#include <map>
#include <set>
#include <vector>
#include <rwlock.h>


//
//通过iweb平台直接发送协议给游戏服务器动态关闭(任务,协议,服务,控制器,NPC,技能,物品)
//
class forbid_manager 
{
public:
	enum
	{
		FORBID_NONE,
		FORBID_TASK,
		FORBID_PROTOCOL,
		FORBID_SERVICE,
		FORBID_CTRL_ID,
		FORBID_NPC,
		FORBID_SKILL,
		FORBID_ITEM,
	};

	static bool IsForbidTask(int task_id)
	{
		return IsForbid(FORBID_TASK, task_id);
	}

	static bool IsForbidProtocol(int protocol_id)
	{
		return IsForbid(FORBID_PROTOCOL, protocol_id);
	}

	static bool IsForbidService(int service_id)
	{
		return IsForbid(FORBID_SERVICE, service_id);
	}

	static bool IsForbidCtrlID(int ctrl_id)
	{
		return IsForbid(FORBID_CTRL_ID, ctrl_id);
	}

	static bool IsForbidNPC(int npc_id)
	{
		return IsForbid(FORBID_NPC, npc_id);
	}

	static bool IsForbidSkill(int skill_id)
	{
		return IsForbid(FORBID_SKILL, skill_id);
	}

	static bool IsForbidItem(int item_id)
	{
		return IsForbid(FORBID_ITEM, item_id);
	}

	static void SyncForbidInfo(int type, std::vector<int> & ids);


	static void DebugForbidInfo(int type, int id);
private:
	static bool IsForbid(int type, int id)
	{
		if(!_is_forbid) return false;
		RWLock::Keeper l(_lock);
		l.LockRead();

		std::map<int, std::set<int> >::iterator iter;
		iter = _forbid_map.find(type);
		if(iter != _forbid_map.end())
		{
			std::set<int> & id_list = iter->second;
			if(id_list.find(id) != id_list.end())
			{
				return true;
			}
		}
		return false;
	}

	static void ForbidCtrlID(std::vector<int> & ids);
	static void ForbidNPC(std::vector<int> & ids);


private:
	static bool _is_forbid;
	static RWLock _lock;
	static std::map<int, std::set<int> > _forbid_map;
};



#endif
