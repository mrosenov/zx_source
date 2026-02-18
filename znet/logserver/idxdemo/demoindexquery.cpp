//#include "idxdemo/query_test1.hpp"
#include "logcommon/query.hpp"
#include "logcommon/indexquery.hpp"
#include "logcommon/config.hpp"
#include <iostream>

void test1(const int i)
{
	LOG::IndexQuery indexquery;
//	indexquery.add(new LOG::Query_test(i));	
//	indexquery.go();
}

int main(int argc, char **argv)
{
/*
	if(argc != 2 || access(argv[1], R_OK) != 0)
	{
		std::cerr << "Usage: " << argv[0] <<  " xml_file " << std::endl;
		exit(-1);
	}
*/

	float timeuse;
        struct timeval tpstart,tpend;
        gettimeofday(&tpstart,NULL);

	try
	{	
		LOG::Config2::GetInstance(argv[1]);
		std::cout << "you search key is : " << argv[2] << std::endl;
		test1( atoi(argv[2]) );
	}
	catch( LOG::Exception &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch(std::exception & e)
        {
                std::cout << e.what() << std::endl;
        }


        gettimeofday(&tpend,NULL);
        timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
        tpend.tv_usec-tpstart.tv_usec;
        timeuse/=1000000;
        std::cout << "Used Time:" << timeuse << std::endl;
	return 0;
}
