#include "logcommon/indexmaker.hpp"
#include "logcommon/index.hpp"
#include "logcommon/util.hpp"
#include <sys/types.h>
#include <unistd.h>
//#include "idxdemo/index2004.hpp"
//#include "idxdemo/index2004_name.hpp"
//#include "idxdemo/index2001_id.hpp"
#include "logcommon/filenamerule.hpp"

void reg( const std::auto_ptr< LOG::IndexMaker > & indexmaker )
{
	//TODO
//	indexmaker->add(new Index2004());
//	indexmaker->add(new Index2004_name());
//	indexmaker->add(new Index2001_id());
}

void handle( int signum )
{
	switch (signum)
	{
	case SIGINT:
		LOG::Env::Inst().running = false;
		break;

	case SIGTERM:
		LOG::Env::Inst().running = false;

	case SIGCHLD:
		LOG::Env::Inst().tail = false;
		LOG::Env::Inst().rebegin = true;
		break;
 	//case SIGHUP:
	}
}

void siginit()
{
	if(        signal( SIGINT, handle ) == SIG_ERR
		|| signal( SIGCHLD, handle ) == SIG_ERR
		|| signal( SIGTERM, handle ) == SIG_ERR
		)
	{
		perror("siginit");
		exit(1);
	}
	else
		std::cout << "signal(SIGINT SIGCHLD SIGTERM) regist ok" << std::endl;
}

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		////////////////////////////////////
	        // file_list rule:
       		// "file1 file2 file3 "
		// Tail file is need only one logfile.
	        ////////////////////////////////////
		printf("Usage: %s xml_file file_list [make|tail]\n", argv[0]);
		exit(0);
	}

	char * configfile = argv[1];
	char * logfile = argv[2];

	
        float timeuse;
        struct timeval tpstart,tpend;
        gettimeofday(&tpstart,NULL);
	siginit();
	LOG::Config2::GetInstance(configfile);
	do
	{
		LOG::Env::Inst().rebegin = false;
		if (argc > 3)
			LOG::Env::Inst().tail = strcmp("tail", argv[3]) == 0 ? true : false;
		if( LOG::Env::Inst().tail && ( ! LOG::FilenameRule::is_one_file(argv[2]) || ! LOG::FilenameRule::is_file(argv[2])) )
		{
			printf("\n Tail file is need only one logfile.\n" );
			exit(0);
		}

		try
		{
			std::auto_ptr<LOG::IndexMaker> indexmaker(new LOG::IndexMaker(logfile));
			reg( indexmaker );
			indexmaker->make(  );
		}
		catch(LOG::Exception &e)
		{
			std::cout << e.what() << std::endl;
		}
		catch(std::exception & e)
		{
			std::cout << e.what() << std::endl;
		}
	}while(LOG::Env::Inst().rebegin);

        gettimeofday(&tpend,NULL);
        timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
        tpend.tv_usec-tpstart.tv_usec;
        timeuse/=1000000;
        std::cout << "Used Time:" << timeuse << std::endl;
        return 0;
}
