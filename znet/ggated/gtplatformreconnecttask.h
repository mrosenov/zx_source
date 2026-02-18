#ifndef  __GNET_GTPLATFORMTIMERTASK
#define  __GNET_GTPLATFORMTIMERTASK

#include "protocol.h"
#include "gtplatformclient.hpp"
namespace GNET
{
	class GTPlatformReconnectTask: public Thread::Runnable
	{
	public:
		Protocol::Manager* manager;

		GTPlatformReconnectTask(Protocol::Manager* m,int priority): Runnable(priority),manager(m) {}
		void Run()
		{
			GTPlatformClient::GetInstance()->TryReconnect();
			delete this;
		}
	};
}
#endif
