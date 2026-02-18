
#ifndef __GNET_GMRESTARTSERVER_HPP
#define __GNET_GMRESTARTSERVER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "mapforbid.h"
#include "maplinkserver.h"
#include "familymanager.h"
#include "circlemanager.h"
#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"
namespace GNET
{

class GMRestartServer : public GNET::Protocol
{
	#include "gmrestartserver"
	class GMRestartTask : public Thread::Runnable
	{
		static Thread::Mutex locker;
		static GMRestartTask instance;
		int roleid;
		int timeout;		//重启倒计时时间
		int forbid_login;	//离重启多长时间，禁止登陆
		int interval;		//通告客户端的时间间隔
		bool running;		//重启任务是否已经执行，如果已经执行，不允许添加新的重启任务
		GMRestartTask() { running=false; }
		GMRestartTask(int _roleid,int _restart=300,int _forbid=60,int _interval=60,bool _running=false) 
			: Runnable(1), roleid(_roleid),timeout(_restart),forbid_login(_forbid),interval(_interval),running(_running)
		{ }
	public:
		~GMRestartTask() { }
		static void ForbidLogin()
		{
			DEBUG_PRINT("!!!!!!!!!!!!!!!!gdlivery::GMRestartTask:: Forbid all users to login!!!!!!!!!!!!!!!\n");
			ForbidLogin::GetInstance().ForbidLoginGlobal();
		}
		static void AllowLogin()
		{
			DEBUG_PRINT("!!!!!!!!!!!!!!!!gdlivery::GMRestartTask:: Allow all users to login!!!!!!!!!!!!!!!\n");
			ForbidLogin::GetInstance().AllowLoginGlobal();
		}
		static void AnnounceGameServer()
		{
			DEBUG_PRINT("gdlivery::GMRestartTask:: Announce GameServer to restart,gmroleid=%d,timeout=%d",
				instance.roleid,instance.timeout);
			GProviderServer::GetInstance()->BroadcastProtocol(GMRestartServer(instance.roleid,0,0,instance.timeout));
		}
		static void AnnounceClient()
		{
			DEBUG_PRINT("gdlivery::GMRestartTask:: Announce Client restart in %d second\n",instance.timeout);
			LinkServer::GetInstance().BroadcastProtocol(GMRestartServer(instance.roleid,0,0,instance.timeout));
		}
		static bool StartTask(int gmroleid,int _restart=300,int _forbid=60,int _interval=20) 
		{
			Thread::Mutex::Scoped l(locker);
			if (!instance.running)
			{
				instance.roleid=gmroleid;
				if (_restart<_interval) _restart=_interval;
				if (_forbid>_restart) _forbid=_restart;
				instance.interval=_interval;
				instance.timeout=_restart;
				instance.forbid_login=_forbid;
				instance.running=true;
				Thread::HouseKeeper::AddTimerTask(&instance,_interval>_restart ? _restart : _interval);
				return true;
			}
			return false;
		}
		static bool IsRunning()
		{ 
			Thread::Mutex::Scoped l(locker);
			return instance.running; 
		}
		static void ChangeParam(int _interval,int _restart,int _forbid)
		{
			Thread::Mutex::Scoped l(locker);
			instance.interval = _interval;
			instance.timeout = _restart;
			instance.forbid_login = _forbid;
		}
		
		void Run()
		{
			Thread::Mutex::Scoped l(locker);
			instance.timeout -= instance.interval;
			if (instance.timeout<=0)
			{
				ForbidLogin();
				AnnounceGameServer();
				running = false;
			}
			else
			{
				AnnounceClient();
				if (instance.timeout<=instance.forbid_login || instance.timeout-instance.interval<10) 
					ForbidLogin();
				Thread::HouseKeeper::AddTimerTask(this,instance.interval);
			}
		}

	};
	void Save2DB()
	{
		FamilyManager::Instance()->Save2DB();
		FactionManager::Instance()->Save2DB();
		CircleManager::Instance()->Save2DB();
		if (GDeliveryServer::GetInstance()->IsCentralDS())
			CrossCrssvrTeamsManager::Instance()->Save2DB();
		else
			CrssvrTeamsManager::Instance()->Save2DB();
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("systemrestart","gmroleid=%d:gsid=%d:time=%d",gmroleid, gsid, restart_time);
		//add restart task to task-pool
		if (GMRestartTask::StartTask(gmroleid,restart_time))
		{
			//announce to all linkserver
			LinkServer::GetInstance().BroadcastProtocol(GMRestartServer(gmroleid,0,0,restart_time));
			Save2DB();	
		}
	}
};

};

#endif
