#ifndef __LOG_INDEXQUERY_HPP__
#define __LOG_INDEXQUERY_HPP__
#include "query.hpp"
#include "logcommon/appconfig2.hpp"
#include "logcommon/config.hpp"

namespace LOG
{

class IndexQuery
{
public:
	~IndexQuery();
	void go();
	void add( Query *query );
private:
	void init();
        bool idxquery( Query *query );
	void scan( Query *query );
        bool get_idxfile(const TimeRange &timerange, const type_t &type, const std::string &logname, 
				const std::string &keyname, fileset_t &idxfile_set);
	void get_indexinfo_set( IndexInfoSet &indexinfo_set, const fileset_t &idxfile_set, 
				const GNET::Marshal::OctetsStream &keyvalue );
	void search_in_log( IndexInfoSet &indexinfo_set, Query *query );
	void get_logfile( const TimeRange &timerange, const std::string &logname, fileset_t &logfile_set );
	std::string get_one_matched_logname( const type_t type );
	std::string logdir, logidxdir, logrotatedir;
	typedef std::set< Query * > queryset_t;
	queryset_t queryset;
};

}
#endif
