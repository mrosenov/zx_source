#include "glinkclient.h"
#include "conf.h"
#include "log.h"
#include "thread.h"
#include <iostream>
#include <unistd.h>
#include "itimer.h"
#include "timersender.h"
using namespace GNET;

int main(int argc, char *argv[])
{
	*if (argc != 5 || access(argv[1], R_OK) == -1)
	{
		std::cerr << "Usage: " << argv[0] << " configurefile " << "username "<< "password "<<"blkickuser"<<std::endl;
		exit(-1);
	}

	Conf *conf = Conf::GetInstance(argv[1]);
	Log::setprogname("gclient");
	{
		GLinkClient *manager = GLinkClient::GetInstance();
		manager->identity=Octets(argv[2],strlen(argv[2]));
		manager->password=Octets(argv[3],strlen(argv[3]));
		manager->blkickuser=!!atoi(argv[4]);
		//manager->SetTimerSenderSize(5);
		IntervalTimer::StartTimer(atoi(conf->find(manager->Identification(), "sender_interval").c_str()));
		manager->RunTimerSender();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		//manager->blCompress=!!(atoi(conf->find(manager->Identification(),"compress").c_str()));
		Protocol::Client(manager);
	}
	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();

	return 0;
}

