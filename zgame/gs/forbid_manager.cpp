#include "forbid_manager.h"
#include <glog.h>
#include "gmatrix.h"

bool forbid_manager::_is_forbid = false;
RWLock forbid_manager::_lock;
std::map<int, std::set<int> > forbid_manager::_forbid_map;

void forbid_manager::SyncForbidInfo(int type, std::vector<int> & ids)
{
	if(ids.empty()) return;

	RWLock::Keeper l(_lock);
	l.LockWrite();

	std::map<int, std::set<int> >::iterator iter;
	iter = _forbid_map.find(type);
	if(iter != _forbid_map.end())
	{
		std::set<int> & id_list = iter->second;
		for(size_t i = 0; i < ids.size(); ++i)
		{
			id_list.insert(ids[i]);
		}
	}
	else
	{
		std::set<int> id_list;
		for(size_t i = 0; i < ids.size(); ++i)
		{
			id_list.insert(ids[i]);
		}
		_forbid_map[type] = id_list;
	}
	_is_forbid = true;


	//关闭控制器
	if(type == FORBID_CTRL_ID) 
	{
		ForbidCtrlID(ids);
	}
	//让NPC协议
	else if(type == FORBID_NPC)
	{
		ForbidNPC(ids);
	}


	for(size_t i = 0; i < ids.size(); ++i)
	{
		GLog::log(GLOG_INFO, "收到iweb平台发送过来的封禁协议, type=%d, id=%d", type, ids[i]);
	}
}


void forbid_manager::ForbidCtrlID(std::vector<int> & ids)
{
	for(size_t i = 0; i < ids.size(); ++i)
	{
		gmatrix::TriggerSpawn(ids[i], false);
	}
}


void forbid_manager::ForbidNPC(std::vector<int> & ids)
{
	for(size_t i = 0; i < ids.size(); ++i)
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_FORBID_NPC,XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0), ids[i]);
		gmatrix::BroadcastAllNPCMessage(msg);
	}
}

void forbid_manager::DebugForbidInfo(int type, int id)
{
	std::vector<int> ids;
	ids.push_back(id);

	SyncForbidInfo(type, ids);
}
