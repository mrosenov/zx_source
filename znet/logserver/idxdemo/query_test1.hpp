#ifndef __LOG_QUERY_TEST1_HPP__
#define __LOG_QUERY_TEST1_HPP__

#include "logcommon/query.hpp"
#include "logcommon/indexquery.hpp"
#include "logcommon/config.hpp"
#include <iostream>
#include  "alltype.hpp"
namespace LOG
{

class Query_test : public LOG::Query
{
public:
        int key;
	int count;

        ~Query_test() { std::cout << "found: " << count << std::endl; }

        Query_test(int _key):key(_key),count(0){}

        virtual type_t get_type() { return 2004; }

        virtual std::string get_keyname() { return "race"; }

        virtual GNET::Marshal::OctetsStream get_keyvalue()
        {
                GNET::Marshal::OctetsStream keyvalue;
                keyvalue << key;
                return keyvalue;
        }

        //virtual TimeRange get_timerange() 
        //      { TimeRange tr(maketime("20070210154903"),maketime("20080210154918")); return tr; }

        virtual TimeRange get_timerange() { TimeRange tr(maketime("200001211"),maketime("20080210154918")); return tr; }

        virtual void process_protocol(GNET::Marshal::OctetsStream &pro_data)
        {
                GNET::Alltype protocol;
                pro_data >> protocol;
count ++ ;
                if( protocol.race == key )
                {
//                      count++;
                      protocol.dump();
                }
        }

        virtual bool enable_scan_if_not_found_idx() { return true; }

        //virtual std::string get_file_or_dir() { return "/home/lijinhua/logsysv3/logrotate/logdir/login.log"; }
        virtual std::string get_file_or_dir() { return "/home/lijinhua/log/any.log"; }
};

}//end namespace LOG
#endif
