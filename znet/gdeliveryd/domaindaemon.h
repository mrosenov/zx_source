#ifndef __GNET_DOMAINDAEMON_H
#define __GNET_DOMAINDAEMON_H

#include "thread.h"

namespace GNET
{

class DomainCmd;
class DomainCmd_Re;
class DomainDaemon 
{
	enum
	{
		STATUS_CLOSED       = 0,
		STATUS_LISTEN       = 1,
		STATUS_LOGIN        = 2,
		STATUS_ESTABLISHED  = 3,   
	};
	int status;
	int localsid;
	int linksid;
	time_t logintime;
	Octets challenge;

	DomainDaemon() : status(STATUS_CLOSED),localsid(0),linksid(0),logintime(0){}  
public:

	static DomainDaemon* Instance() { static DomainDaemon instance; return &instance;}
	bool ValidSession(int _linksid, int _localsid)
	{
		return status==STATUS_ESTABLISHED && linksid==_linksid && localsid==_localsid;
	}

	void Process(int _linksid, DomainCmd& data);
	void Deliver(DomainCmd_Re& data);
	void OnLogin(Octets& nonce, int _linksid, int _localsid);
	void OnValidate(Octets& nonce, int _linksid, int _localsid);
	void StartListen()
	{
		status = STATUS_LISTEN;
	}
};

};
#endif

