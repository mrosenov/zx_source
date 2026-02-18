#include "glinkclient.h"
#include "conf.h"

using namespace GNET;

int main(int argc, char *argv[])
{        
	std::cerr << "Usage: " << argv[0] << " configurefile " << "username ("<<argv[2]<< ")password ("<<argv[3]<<") blkickuser"<<std::endl; 
	if (argc!=6  || access(argv[1], R_OK) == -1)
	{
		for(int i=0;i<argc;i++)
			printf("argv %d,%s\n",i,argv[i]);
		std::cerr << "Usage: " << argv[0] << " configurefile " << "username "<< "password "<<"blkickuser"<<std::endl;
		exit(-1);
	}

	int seed = getpid();
	srandom(seed);
	sleep (random()%10);

	Conf *conf = Conf::GetInstance(argv[1]);
	Log::setprogname("gclient");
	{
		GLinkClient *manager = GLinkClient::GetInstance();
		manager->SetChoice(argv[5]);
		manager->SetConf(conf);
		manager->SetUsername(argv[2]);
//		manager->SetRoomID(atoi(argv[6]));
		manager->identity=Octets(argv[2],strlen(argv[2]));
		manager->password=Octets(argv[3],strlen(argv[3]));
		manager->blkickuser=!!atoi(argv[4]);
		manager->SetTimerSenderSize(5);
		IntervalTimer::StartTimer(atoi(conf->find(manager->Identification(), "sender_interval").c_str()));
		manager->RunTimerSender();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		
		manager->SetLineId(atoi(conf->find(manager->Identification(), "lineid").c_str()));
		manager->SetOccup(atoi(conf->find(manager->Identification(), "occup").c_str()));
		//manager->blCompress=!!(atoi(conf->find(manager->Identification(),"compress").c_str()));
		Protocol::Client(manager);
	}
	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();

	return 0;
}   
