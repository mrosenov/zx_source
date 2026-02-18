#ifndef __GNET_DELETEROLE_TASK_H
#define __GNET_DELETEROLE_TASK_H

#include "thread.h"
#include "gamedbclient.hpp"
#include "delroleannounce.hpp"
namespace GNET
{
	class DeleteRoleTask : public Thread::Runnable
	{
		Octets key_cur;
		int update_time;
		DeleteRoleTask(int _update,int prior=1) : Runnable(prior), update_time(_update) { }
	public:
		~DeleteRoleTask() { }
		static DeleteRoleTask* GetInstance(int _update,int prior=1) { 
			static DeleteRoleTask instance(_update,prior); 
			return &instance; 
		}
		void Run()
		{
			//statinfo打印转移到map.cpp中处理
			GameDBClient::GetInstance()->SendProtocol(DelRoleAnnounce(WaitDelKeyVector()));
			Thread::HouseKeeper::AddTimerTask(this,update_time);
		}
	};
};
#endif
