#ifndef __LOG_LOGRECORD_HPP__
#define __LOG_LOGRECORD_HPP__

#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include "marshal.h"
#include "thread.h"
#include "octets.h"
#include "logcommon/recordwriter.hpp"
#include "logcommon/appconfig2.hpp"

namespace LOG
{

class LogRecorder
{
	typedef std::set< RecordWriter * >             		writer_set_t;
        typedef std::map< GNET::Protocol::Type, writer_set_t >  type_writer_map_t;
        typedef std::map< std::string, RecordWriter * >    	file_writer_map_t;
        type_writer_map_t    					type_writer_map;
        // file_writer_map 负责唯一的recordwriter管理和销毁
        file_writer_map_t    					file_writer_map;

	typedef std::set< GNET::Protocol::Type > 		denyset_t;
	denyset_t 						denyset;

	typedef std::set< std::string > fileset_t;
	typedef std::map< GNET::Protocol::Type, fileset_t > type_file_map_t;

	RecordWriter *anylog;
	RecordWriter *defaultlog;
public:
	LogRecorder()
	{
		denyset = Config2::GetInstance()->GetDenySet();
		
		type_file_map_t type_file_map = Config2::GetInstance()->get_type_file_map();
		type_file_map_t::const_iterator tfm_it = type_file_map.begin();
		for ( ; tfm_it != type_file_map.end(); ++ tfm_it )
		{
			fileset_t::const_iterator fs_it = tfm_it->second.begin();	
			for ( ; fs_it != tfm_it->second.end(); ++fs_it )
			{
				if ( RecordWriter * rw = new_writer( *fs_it ) )
					type_writer_map[ tfm_it->first ].insert( rw );
			}
		}
		std::string deflog = Config2::GetInstance()->GetDefaultFile();
		if ( ! deflog.empty() ) defaultlog = new_writer( deflog );
		std::string alog = Config2::GetInstance()->GetAnyFile();
		if ( ! alog.empty() ) anylog = new_writer( alog );
	}
	
	~LogRecorder()
	{
		file_writer_map_t::iterator it = file_writer_map.begin();
		for ( ; it != file_writer_map.end(); ++it )
			delete it->second;
	}
	
	void write( const RecordHead &rh, const void * record, const size_t size )
	{
		type_writer_map_t::iterator it = type_writer_map.find( rh.type );
                if ( it != type_writer_map.end() )
                {
                        writer_set_t::iterator mpit = it->second.begin(); 
			writer_set_t::iterator end  = it->second.end();
			for ( ; mpit != end; ++mpit )
                                (*mpit)->write((char *)record, size, rh.time);
                }
                else if ( defaultlog && ( denyset.find(rh.type) == denyset.end() ) )
                {
			defaultlog->write((char *)record, size, rh.time);
                }
                if ( anylog )
                        anylog->write((char *)record, size, rh.time);
	}

        void rotate( const fileset_t &fileset )
        {
                for( fileset_t::iterator it = fileset.begin(); it != fileset.end(); ++it )
                {
                        file_writer_map_t::iterator fwm_it = file_writer_map.find( *it );
                        if( fwm_it != file_writer_map.end() ) 
			{
				RecordWriter *old = fwm_it->second;
                                delete fwm_it->second;
				fwm_it->second = NULL;
				RecordWriter * rw = new_writer( fwm_it->first );	
				if( Config2::GetInstance()->GetDefaultFile() == fwm_it->first && defaultlog )
					defaultlog = rw;
				else if(  Config2::GetInstance()->GetAnyFile() == fwm_it->first && anylog )
					anylog = rw;
				else
					replace_in_typewritermap( old, rw );
			}
                }               
        }     

private:
	void replace_in_typewritermap( RecordWriter *old, RecordWriter *rw )
	{
		type_writer_map_t::iterator it = type_writer_map.begin();
		for (; it != type_writer_map.end(); ++it )
		{
			if( it->second.find(old) != it->second.end() )
			{
				it->second.erase( old );
				it->second.insert( rw );
			}
		}
	}

	RecordWriter * new_writer( const std::string &filename )
	{
		std::pair< file_writer_map_t::iterator, bool > r
			= file_writer_map.insert( file_writer_map_t::value_type( filename, NULL ) );
		if ( r.first->second == NULL )
			r.first->second = new RecordWriter( filename );
		return r.first->second;
	}

};

class LogManager
{
private:
	LogRecorder logrecorder;
	LogManager()                { LogRec("LogManager Create"); }
	~LogManager()               { instance = NULL; }

	static LogManager * instance;
public:
        static void Delete() { delete instance; }

	static LogManager * GetInstance() 
	{ 
		if (NULL == instance)
			instance = new LogManager();
		return instance;
	}

	void Write(const RecordHead &rh, const void * fullrecord, size_t size)
	{
		logrecorder.write( rh, fullrecord, size );
	}
	
	void Rotate(const std::string &filelist)
	{
		fileset_t fileset;
		SplitFilelist(filelist, fileset);
		logrecorder.rotate( fileset );
	}

private:
	void SplitFilelist(const std::string &filelist, fileset_t &fileset)
	{
		LogRec( " recv filelist = %s", filelist.c_str() );
		std::string file = filelist;
		if ( file[ file.length() -1 ] != ',' )
			file.append(",");
		std::string::size_type pos1 = 0, pos2;
		while( ( pos2 = file.find(",", pos1) ) != std::string::npos )
		{
			fileset.insert(file.substr(pos1,pos2 - pos1));
			pos1 = pos2 + 1;
		}
	}
};

}//end namespace LOG
#endif
