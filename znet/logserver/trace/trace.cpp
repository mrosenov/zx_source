#include "trace.hpp"
#include "logcommon/config.hpp"
#include "logcommon/util.hpp"

using namespace GNET;

void usage()
{       
        std::cout << "Usage: trace [-f] [-n lines / -t begintime] logfile\n" 
		"\t-f  waiting\n"
		"\t-n  list lines\n"
		"\t-t  begin time\n"
		;

        exit(-1);
}

int main(int argc, char **argv)
{
	// parse

        bool wait = false;
        int lines = 0;
	time_t time = 1176124541;
	int seek = 0;

        char c;
        while ( (c = getopt(argc, argv, "fn:t:")) != -1)
	{
                switch (c)
		{
                case 'f': wait = true; break;
                case 'n': seek = 1; lines = atoi( optarg ); break;
		case 't': seek = 2; time = atoi( optarg ); break;
                default:  usage(); break;
			break;
                }
        }

	// check
        if (optind != argc - 1)
		usage();
	char *file = argv[ optind ];

	// 
	try
	{
		LOG::Trace t(file, wait); 
		switch (seek)
		{
		case 1:
			t.traceline(lines);
			break;
		case 2:
			t.tracetime(time);
			break;
		case 0:
			t.trace();
			break;
		default:
			usage();
			break;
		}
		/*
		//t.tracetime( 1176124541 ); //< first
		//t.tracetime( 1176124561 ); //first
		t.tracetime( 1176124636 ); //mid ,no match
		//t.trace();
		//t.tracetime( 1176124639 ); //mid , match
		//t.tracetime( 1176124693 ); //last
		//t.tracetime( 1176124694 ); //> last
		*/
	}
	catch( LOG::Exception &e )
	{
		std::cout << "error: " << e.what() << std::endl;
	}
        exit(0);
}
