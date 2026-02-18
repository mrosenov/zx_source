#ifndef __LOG_INDEXDB_HPP__
#define __LOG_INDEXDB_HPP__
#include "logcommon/baseclass.hpp"
#include "db.h"
#include <set>

namespace LOG
{

class IndexDB
{
public:
        IndexDB(const std::string &);
	~IndexDB();
	void flush();
        bool find(const GNET::Marshal::OctetsStream &, IndexInfoSet &iiset);          
        bool insert(const GNET::Marshal::OctetsStream &,  IndexInfo &);
	bool del(const GNET::Marshal::OctetsStream &key);
        bool insert(const GNET::Marshal::OctetsStream &,  IndexInfoSet &);
	enum { FLUSH_TIME = 3600 };
private:
	std::string idxFileName;
	GNET::DBStandalone *db;
	static time_t timestamp;
};

}//end namespace LOG
#endif
