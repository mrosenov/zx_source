#include "rotatetimer.hpp"

int main(int argc, char *argv[])
{
	if( argc != 3 || access(argv[1], R_OK) == -1 || access(argv[2], R_OK) == -1 )
        {
                std::cout << "Usage:" << argv[0] << " config_file"  << " xml_file " << std::endl;
                exit(0);
        }
	GNET::Conf::GetInstance(argv[1]);
	LOG::Log_Init();
        LOG::Config2::GetInstance(argv[2]);
	GNET::Thread::HouseKeeper::AddTimerTask(LOG::RotateTimer::GetInstance(),1);
	GNET::Thread::Pool::AddTask(GNET::PollIO::Task::GetInstance());
	GNET::Thread::Pool::Run();
	return 0;
}
