#ifndef __GNET_PROTOCOL_EXECUTOR_H
#define __GNET_PROTOCOL_EXECUTOR_H
#include "thread.h"
#include "protocol.h"
namespace GNET
{
	
class ProtocolExecutor : public Thread::Runnable
{
	Protocol::Manager*	manager;
	unsigned int 		sid;
	Protocol*		p;
public:
	ProtocolExecutor(Protocol::Manager* m,unsigned int _sid,Protocol* _p,int prior=1) 
		: Runnable(prior),manager(m),sid(_sid),p(_p) 
	{ 
	}
	void Run()
	{
		p->Process(manager,sid);
		p->Destroy();
		delete this;
	}
};
/*
class RpcExecutor : public Thread::Runnable
{
	Protocol::Manager* manager;
	unsigned int	sid;
	Rpc * rpc;
public:
	RpcExecutor(Protocol::Manager * m, unsigned int s, Protocol::Type t, const Rpc::Data & arg, int prior=1)
		: Runnable(prior), manager(m), sid(s)
	{
		rpc = (Rpc *)Protocol::Create(t);
		if (rpc != NULL)
			*rpc->argument = arg;
	}
	void Run()
	{
		if (rpc != NULL)
		{
			rpc->Server(rpc->argument, rpc->result, manager, sid);
			rpc->Destroy();
		}
		delete this;
	}
};
*/
};
#endif
