#include "indexdb.hpp"
#include <stdexcept>

namespace LOG
{

	time_t IndexDB::timestamp = time(NULL);

	IndexDB::IndexDB( const std::string &_ifn ):idxFileName(_ifn)
        {
                db = new GNET::DBStandalone( idxFileName.c_str() );
                db->init();
        }

        IndexDB::~IndexDB()
        {
		db->checkpoint();
                delete db;
        }

	void IndexDB::flush()
	{
		if( ( time(NULL) - FLUSH_TIME ) >= IndexDB::timestamp ) 
		{
			db->checkpoint();
			IndexDB::timestamp = time(NULL);
		}
	}

	bool IndexDB::insert( const GNET::Marshal::OctetsStream &key, IndexInfo &indexinfo )	
	{
		IndexInfoSet indexinfo_set;
		find(key, indexinfo_set);
		indexinfo_set.insert(indexinfo);
		GNET::Marshal::OctetsStream os;
		indexinfo_set.marshal(os);
		if( ! db->put(key.begin(), key.size(), os.begin(), os.size()) )
			throw std::runtime_error( errno == EINTR ? "db.write interrupt" : "db.write error" );
		flush();
		return true;
	}

	bool IndexDB::del( const GNET::Marshal::OctetsStream &key )
	{
		db->del( key.begin(), key.size() );
		return true;
	}

	bool IndexDB::find( const GNET::Marshal::OctetsStream &key, IndexInfoSet &iiset )
	{
		size_t val_len;
		void *val = db->find( key.begin(), key.size(), &val_len );
		if ( val )
		{
			GNET::Octets oc(val, val_len);
			free (val);
			GNET::Marshal::OctetsStream os(oc);
			iiset.unmarshal(os);
			return true;
		}
		return false;
	}	

	//delete  一般的调用没有插入indexinfoset的
	bool IndexDB::insert(const GNET::Marshal::OctetsStream &key, IndexInfoSet &iiset)	
	{
		IndexInfoSet indexinfo_set;
		find(key, indexinfo_set);
		for(iiset_t::iterator it = iiset.begin(); it != iiset.end(); ++it)
			indexinfo_set.insert(*it);
		GNET::Marshal::OctetsStream os;
		indexinfo_set.marshal(os);
		if( ! db->put(key.begin(), key.size(), os.begin(), os.size()) )
			throw std::runtime_error( errno == EINTR ? "db.write interrupt" : "db.write error" );
		flush();
		return true;
	}
	
}//end namespace LOG
