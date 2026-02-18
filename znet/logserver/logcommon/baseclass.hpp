#ifndef __LOG_BASECLASS_HPP__
#define __LOG_BASECLASS_HPP__

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string>
#include <iostream>
#include "config.hpp"
#include "marshal.h"
#include "octets.h"
#include "protocol.h"
#include "logcommon/util.hpp"


namespace LOG
{


struct PageHdr
{
	unsigned short foffset;
	unsigned short loffset;
	time_t timestamp;
};

struct Env
{
        bool tail ;
        bool running ;
        bool rebegin ;

	Env():tail(false), running(true), rebegin(false) {}

	static Env & Inst() 
	{ 
		static Env env; 
		return env; 
	} 
};

struct RecordHead : public GNET::Marshal
{
	enum { SIZE = 12};

	size_t rlen;
	size_t type;
	time_t time;

	RecordHead():rlen(0), type(0), time(0){}

	GNET::Marshal::OctetsStream &marshal(GNET::Marshal::OctetsStream &os) const
        { return os << rlen << type << time; }

        const GNET::Marshal::OctetsStream &unmarshal(const GNET::Marshal::OctetsStream &os)
        { return os >> rlen >> type >> time; }
};


/*
inline std::ostream & operator << ( std::ostream &os, const GNET::Protocol &protocol )
{
	return protocol.trace( os );
}

inline std::ostream & operator << ( std::ostream &os, const GNET::Protocol *protocol )
{
	return protocol->trace( os );
}

*/
struct IndexInfo : public GNET::Marshal
{
	size_t rlen;
	OFF_T offset;
	std::string filename;

        IndexInfo(const std::string &_fn, const OFF_T _ofs, const size_t _rl ):rlen(_rl), offset(_ofs), filename(_fn) {  }
	IndexInfo():rlen(0), offset(0) { }

        GNET::Marshal::OctetsStream &marshal(GNET::Marshal::OctetsStream &os) const
        { return os << rlen << offset << filename; }

        const GNET::Marshal::OctetsStream &unmarshal(const GNET::Marshal::OctetsStream &os)
        { return os >> rlen >> offset >> filename; }

	IndexInfo( const IndexInfo &rsh )
	{
		rlen = rsh.rlen;
		offset = rsh.offset;
		filename = rsh.filename;
	}

        bool operator < ( const IndexInfo &ii) const
        {
                // ordery by filename, offset, rlen
		if (filename < ii.filename) return true;
		if (filename > ii.filename) return false;
                if (offset < ii.offset) return true;
                if (offset > ii.offset) return false;
                return rlen < ii.rlen;
        }

        bool operator == (const IndexInfo &ii) const
        {
                return offset == ii.offset && rlen == ii.rlen && filename == ii.filename;
        }
};

typedef std::set< IndexInfo > iiset_t;

struct TimeRange
{
	time_t begin;
	time_t end;

	TimeRange(const time_t _b , const time_t _e ):begin(_b), end(_e){}

        TimeRange(const std::string &_b, const std::string &_e )
        {
                begin = maketime( _b );
                end = maketime( _e );
        }

        bool inRange(const TimeRange &tr) const
        {
                if(begin == 0 && end == 0)
                        return true;
                if(begin == 0 && end > tr.begin)
                        return true;
                if(end == 0 && begin < tr.end)
                        return true;
                if( end >tr.begin)
                        return false;
                if(begin < tr.end)
                        return false;
                return true;
        }
};

struct IndexInfoSet : public GNET::Marshal
{
	iiset_t iiset;
	
	iiset_t::const_iterator begin() const { return iiset.begin(); }
        iiset_t::const_iterator end() const { return iiset.end(); }
        iiset_t::iterator begin() { return iiset.begin(); }
        iiset_t::iterator end() { return iiset.end(); }
        iiset_t::size_type size() { return iiset.size(); }
        void clear() { return iiset.clear(); }
        bool empty() const   { return iiset.empty(); }

        void append(const IndexInfoSet &_iiset)
        {
                for(iiset_t::const_iterator it = _iiset.begin(); it != _iiset.end(); ++it)
                        iiset.insert(*it);
        }

        std::pair<iiset_t::iterator, bool> insert(const iiset_t::value_type& x)
        { return iiset.insert(x); }


        GNET::Marshal::OctetsStream &marshal(GNET::Marshal::OctetsStream &os) const
        { return os << GNET::MarshalContainer(iiset); }

        const GNET::Marshal::OctetsStream &unmarshal(const GNET::Marshal::OctetsStream &os)
        { return os >> GNET::MarshalContainer(iiset); }

	void dump()
        {
                std::cout << "IndexInfoSet dump. size = " << iiset.size() << std::endl;
                for(iiset_t::iterator it = iiset.begin(); it != iiset.end(); ++it)
                {
                        std::cout << " rlen =" << (*it).rlen << " offset = " << (*it).offset
                                << " filename = " << (*it).filename << std::endl;
                }
                std::cout << "-------IndexInfoSet dump end------- " << std::endl;
        }

};

struct FileHdr: public GNET::Marshal
{
#define LOGFLAG "log"
#define VERSION "1.0.0"

        std::string     logflag;
        std::string     version;
        std::string     logname;
        time_t          timestamp;
        std::string     uuid;
	/*  
	logflag = log
	version = 1.0.0
	logname = logserver
	timestamp = time(NULL);
	uuid = "";
	*/
	bool check( const GNET::Marshal::OctetsStream &os ) //throw MYTODO
	{
		unmarshal( os );
		Dump();
		if( logflag == LOGFLAG && version == VERSION )
			return true;
		return false;
	}

	void create( GNET::Marshal::OctetsStream &os, const std::string &_logname)
	{
		logflag = LOGFLAG;
		version = VERSION;
		logname = _logname;
		timestamp = time(NULL);
		marshal( os );
	}

	time_t gettimestamp() { return timestamp; }

	GNET::Marshal::OctetsStream &marshal(GNET::Marshal::OctetsStream &os) const
        {
                return  os << logflag << version << logname << timestamp << uuid;
        }

        const GNET::Marshal::OctetsStream &unmarshal(const GNET::Marshal::OctetsStream &os)
        {
                return  os >> logflag >> version >> logname >> timestamp >> uuid;
        }

        void Dump()
        {
                LogRec("filehdr logname = %s",logname.c_str());
                LogRec("filehdr logflag = %s",logflag.c_str());
                LogRec("filehdr version = %s",version.c_str());
                LogRec("filehdr timestamp = %d",timestamp);
                LogRec("filehdr uuid = %s",uuid.c_str());
                LogRec("------------------------");
        }
		
};

// not recursion dir
struct ReadDir
{
public:
	fileset_t getfileset() { return fileset; }

	explicit ReadDir( const std::string & _dir ):dir( _dir ), dirp(NULL), dp(NULL) 
	{
		if ( dir.empty() )
			throw LOG::Exception( "dir is empty" );
                if( dir[ dir.length() - 1 ] != '/' )
                        dir.append("/");
                if ( (dp = ::opendir(dir.c_str())) == NULL)
			throw LOG::Exception( " open dir err:" + dir );
		
                while ( (dirp = readdir(dp)) != NULL )
                {
                        if (strcmp(dirp->d_name, ".") == 0  || strcmp(dirp->d_name, "..") == 0)
                                continue;               /* ignore dot and dot-dot */
                        if( strlen(dirp->d_name) < SERIAL_LEN )
                                continue;
                        std::string fullpath = dir + dirp->d_name;
                        STAT( fullpath.c_str(), &statbuf );
                        if (S_ISDIR(statbuf.st_mode) == 0) /* not a directory */
                                fileset.insert( dirp->d_name );
                }
	}

        ~ReadDir()
        {
                if( dp && (closedir(dp) < 0) )
        		std::cout << " closedir error" << std::endl;
        }

private:
	std::string dir;
	STRUCT_STAT   statbuf;
	struct  dirent *dirp;
	DIR     *dp;
	fileset_t fileset;
};

} //end namespace LOG

/*
inline std::ostream & operator << ( std::ostream &os, const GNET::Marshal &marshal ) //MYTODO move to marshal.h protocol-->marshal
{
        return marshal.trace( os );
}

inline std::ostream & operator << ( std::ostream &os, const GNET::Marshal *marshal )
{
        return marshal->trace( os );
}

*/
inline std::ostream & operator << ( std::ostream &os, const LOG::RecordHead &rh )
{
	return os << "type=" << rh.type << "," << LOG::remaketime(rh.time) << ",";
}

#endif

