#include "logtcpserver.hpp"
#include "logudpserver.hpp"
#include "conf.h"
#include "log.h"
#include "thread.h"
#include "protocol.h"
#include "passiveio.h"
#include <iostream>
#include <unistd.h>
#include "logcommon/appconfig2.hpp"
#include "logsession.hpp"
#include "logrecord.hpp"
#include "logcommon/util.hpp"
#include "logcommon/config.hpp"
//#include "rotatetimer.hpp"
int ii = 1;
using namespace GNET;
using namespace LOG;

struct ExitTask : public GNET::Thread::Runnable
{
	virtual void Run()
	{
        	LogRec("program exit");
        	LogManager::Delete();
		kill(getpid(), 9);
	}
};

void erase( int signum )
{
        LogRec("catch signal signum = %d", signum);
	GNET::Thread::Pool::AddTask(new ExitTask());
}

int main(int argc, char *argv[])
{
	if (argc != 3 || access(argv[1], R_OK) == -1 || access(argv[2], R_OK) == -1)
	{
		std::cerr << "Usage: " << argv[0] << " ConfigFile" << " XmlFile" << std::endl;
		exit(-1);
	}

        signal( SIGTERM, erase ); 
        Conf *conf = Conf::GetInstance(argv[1]); 
        Config2 *conf2 = Config2::GetInstance(argv[2]); 
	conf2->Dump();
	LogManager::GetInstance();
	//udp
	{
		LogUdpServer *manager = LogUdpServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		PassiveIO::Open(LOG::LogSession(manager));
	}
	//tcp
        {
		LogTcpServer *manager = LogTcpServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		PassiveIO::Open(LOG::LogSession(manager));
        }
	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();
	return 0;
}
