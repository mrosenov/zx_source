#ifndef __LOG_FILENAMERULE_HPP__
#define __LOG_FILENAMERULE_HPP__

#include "logcommon/util.hpp"
#include "logcommon/filenamerule.hpp"
#include "logcommon/baseclass.hpp"

namespace LOG
{

struct FilenameRule
{

	static std::string idxfile2logfile( const std::string &idxfile , 
						const std::string &logdir, 
						const std::string &logrotatedir )
	{       
		std::string filename = idxfile.substr(idxfile.rfind("/") + 1);
		std::string::size_type pos = filename.find(".") + 1;
		if( is_tail_idx_file( idxfile ) )
			return logdir + filename.substr(0, filename.find(".", pos)) + ".log";
		std::string::size_type pos1 = filename.find(".") + 1;
		return  logrotatedir + filename.substr(0, filename.find(".", pos1)) + ".log";
	}             
	static std::string log2idx( const std::string &logfile, 
					const std::string &logidxdir, 
					const type_t type, 
					const std::string key_name )
        {
                std::string tmp = logfile.substr(logfile.rfind("/") + 1);
                std::string filename = tmp.substr(0, tmp.rfind("."));
                char buff[11];
                sprintf(buff, "%d", type);
                std::string type_name = buff;

                if( !have_start_time( logfile ) )
                {
                        std::string ret(logidxdir);
                        ret.append(filename).append(".").append(get_file_start_time(logfile));
                        ret.append(".").append(type_name).append(".").append(key_name).append(".idx");

                        return ret;
                }
                else
                        return logidxdir + filename + "." + type_name + "." + key_name + ".idx";
        }

        static bool have_start_time( const std::string &file )
        {
                std::string::size_type pos1 = file.find(".") + 1;
                std::string::size_type pos2 = file.find(".", pos1);
                std::string date = file.substr( pos1, pos2 - pos1 );
                if( str_is_time(date) )
                        return true;
                return false;
        }

	/////////////////////////////////////////////////////////////////////////
	//
	//判断文件是实时索引文件
	//
	/////////////////////////////////////////////////////////////////////////
	static bool is_tail_idx_file( const std::string &full_filename )
	{
		if( !is_idx_file( full_filename ) ) return false;
		std::string::size_type pos = full_filename.rfind( "/" ) + 1;
		if( is_digit(full_filename, pos, SERIAL_LEN) )
			return false;
		return true;
	}


	static bool match_type( const std::string &full_filename, const size_t type ) 
	{
		std::string type_str = get_type( full_filename );
		size_t new_type = atoi( type_str.c_str() );
		return new_type == type ? true : false;
	}

	static bool match_keyname( const std::string &full_filename, const std::string &keyname )
	{
		if( keyname == get_keyname( full_filename ))
			return true;
		return false;
	}

	static bool match_time_range( const std::string &full_filename, const TimeRange &timerange )
        {
		if( ! have_start_time( full_filename ) )
			return false;
                if( timerange.begin == 0 && timerange.end == 0)
                        return true;

		TimeRange tr = timerange;
		if( 0 == timerange.begin )
			tr.begin = MIN_LIMIT_TIME;
		if( 0 == timerange.end )
			tr.end = time( NULL );
                std::string time_str = get_start_time( full_filename );
                if( maketime(time_str) >= tr.begin && maketime(time_str) <= tr.end )
                        return true;
                return false;
        }

	static std::string get_serial( const std::string &full_filename )	
	{
		if( is_tail_idx_file( full_filename ) )
			return "";
		std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
		std::string serial = full_filename.substr(pos1, SERIAL_LEN);
		if( is_digit( serial, 0, SERIAL_LEN ) ) //will dele ??
			return serial;
		return "";
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// eg: /home/lijinhua/log/login.log
	//
	///////////////////////////////////////////////////////////////////////////
	static std::string get_logdir_logname( const std::string & full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
		std::string::size_type pos2 = full_filename.find( ".", pos1 );
		if( pos1 == std::string::npos || pos2 == std::string::npos )
			throw LOG::Exception("get_logdir_logname(): bad file" + full_filename);
		return full_filename.substr(pos1, pos2 - pos1);
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// eg1: /home/lijinhua/logidx/serial_login.starttime.type.typename.idx
	// eg2: /home/lijinhua/logidx/login.starttime.type.typename.idx (tailing)
	//
	///////////////////////////////////////////////////////////////////////////
	static std::string get_logidxdir_logname( const std::string & full_filename )
	{
		if( is_tail_idx_file( full_filename ) ) //eg2
		{
			std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
			std::string::size_type pos2 = full_filename.find( ".", pos1 );
			if( pos1 == std::string::npos || pos2 == std::string::npos )
				throw LOG::Exception("get_logidxdir_logname():bad file" + full_filename);
			return full_filename.substr(pos1, pos2 - pos1);
		}
		else //eg1
		{
			std::string::size_type pos1 = full_filename.find( "_" ) + 1;
			std::string::size_type pos2 = full_filename.find( ".", pos1 );
			if( pos1 == std::string::npos || pos2 == std::string::npos )
				throw LOG::Exception("get_logidxdir_logname():bad file" + full_filename);
			return full_filename.substr(pos1, pos2 - pos1);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//
	// eg: /home/lijinhua/logrotatedir/serial_login.starttime.log
	//
	///////////////////////////////////////////////////////////////////////////
	static std::string get_logrotatedir_logname( const std::string & full_filename )
	{
		std::string::size_type pos = full_filename.rfind( "/" ) + 1;
		if( ! is_digit(full_filename, pos, SERIAL_LEN) )
			throw LOG::Exception( "get_logrotatedir_logname():bad file " + full_filename );
		std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
		std::string::size_type pos2 = full_filename.find( "_", pos1 ) + 1;
		std::string::size_type pos3 = full_filename.find( ".", pos2 );
		if( pos1 == std::string::npos || pos2 == std::string::npos  || pos3 == std::string::npos )
			throw LOG::Exception("get_logrotatedir_logname():bad file" + full_filename);
		return full_filename.substr(pos2, pos3 - pos2);
	}

	/*
	static std::string get_logname( const std::string &full_filename ) //get_rotatedir_logname 
	{
		if( is_tail_idx_file( full_filename ) )
		{
			std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
			std::string::size_type pos2 = full_filename.find( ".", pos1 );
			if( pos1 == std::string::npos || pos2 == std::string::npos )
				throw LOG::Exception("bad file" + full_filename);
			return full_filename.substr(pos1, pos2 - pos1);
		}
		else
		{
			std::string::size_type pos1 = full_filename.find( "_" ) + 1;
			std::string::size_type pos2 = full_filename.find( ".", pos1 );
			if( pos1 == std::string::npos || pos2 == std::string::npos )
				throw LOG::Exception("bad file" + full_filename);
			return full_filename.substr(pos1, pos2 - pos1);
		}
	}
	*/

	static std::string get_start_time( const std::string &full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
		std::string::size_type pos2 = full_filename.find( ".", pos1 ) + 1;
		std::string::size_type pos3 = full_filename.find( ".", pos2 + 1 );
		if( pos1 == std::string::npos 
			|| pos2 == std::string::npos 
			|| pos3 == std::string::npos)

			throw LOG::Exception("get_start_time():bad file" + full_filename);
		std::string time_str = full_filename.substr( pos2, pos3 - pos2 );
		if( ! is_digit(time_str, 0, time_str.length()) )
			throw LOG::Exception("get_start_time():bad time" + full_filename);
		return time_str;
	}
	
	static std::string get_type( const std::string &full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind( "/" ) + 1;
		std::string::size_type pos2 = full_filename.find( ".", pos1 );
		std::string::size_type pos3 = full_filename.find( ".", pos2 + 1 ) + 1;
		std::string::size_type pos4 = full_filename.find( ".", pos3 + 1 );
		if( pos1 == std::string::npos 
			|| pos2 == std::string::npos 
			|| pos3 == std::string::npos 
			|| pos4 == std::string::npos )

			throw LOG::Exception("get_type():bad file" + full_filename);
		return full_filename.substr( pos3, pos4 - pos3 );
	}
	
	static std::string get_keyname( const std::string &full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind( "." ) - 1;
		std::string::size_type pos2 = full_filename.rfind( ".", pos1 ) + 1;
		if( pos1 == std::string::npos || pos2 == std::string::npos )
			throw LOG::Exception("get_keyname():bad file" + full_filename);
		return full_filename.substr( pos2, pos1 + 1 - pos2 );
	}
	
	static bool is_log_file( const std::string &full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind( "." ) + 1;
		if( pos1 == std::string::npos )
			throw LOG::Exception("is_log_file():bad file" + full_filename);
		return  LOG_EXT == full_filename.substr( pos1 ) ? true : false;
	}

	static bool is_idx_file( const std::string &full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind( "." ) + 1;
		if( pos1 == std::string::npos )
			throw LOG::Exception("is_idx_file():bad file" + full_filename);
		return  IDX_EXT == full_filename.substr( pos1 ) ? true : false;
	}
	
	static bool is_file( const std::string &full )
	{
		STRUCT_STAT buff;
		STAT( full.c_str(), &buff );
		return S_ISDIR(buff.st_mode) == 0 ? true : false;
	}

	static bool is_dir( const std::string &full )
	{
		STRUCT_STAT buff;
		STAT( full.c_str(), &buff );
		return S_ISDIR(buff.st_mode) != 0 ? true : false;
	}
	
	static bool is_one_file( const std::string file_list ) //modi
	{
		ltrim(file_list);
		rtrim(file_list);
		return std::string::npos == file_list.find(" ") ? true : false;
	}
	
	static void get_file_or_dir_fileset( const std::string &file_or_dir, fileset_t &fileset ) //MYTOD remove to util.cpp
	{
		fileset_t tmp_set;
		split_str( file_or_dir, FILENAME_SPLIT, tmp_set );
		for( fileset_t::iterator it = tmp_set.begin(); it != tmp_set.end(); ++ it )
		{
			if( FilenameRule::is_file(*it) )
				fileset.insert( *it );
			else
			{
				ReadDir readdir( *it );
				//fileset_t tempset = readdir.getfileset();
				fileset.insert( readdir.getfileset().begin(), readdir.getfileset().end() );
			}
		}
	}
	
	static bool check_logdir_file( const std::string & full_filename )
	{
		std::string::size_type pos1 = full_filename.rfind("/") + 1;
		std::string::size_type pos2 = full_filename.find(".") + 1;
		if( pos1 == std::string::npos || pos2 == std::string::npos )
			return false;
		if( full_filename.substr( pos2 ) != LOG_EXT )
			return false;
		return true;
	}

	static bool check_logrotatedir_file( const std::string & full_filename )
	{
		std::string::size_type pos = full_filename.rfind("/") + 1;
		if( ! is_log_file( full_filename ) 
			|| ! is_digit( full_filename, pos, SERIAL_LEN ) 
			|| ! (full_filename.at(pos + SERIAL_LEN) == SERIAL_LOGNAME_SPLIT ) 
			)
			return false;
		get_start_time( full_filename );
		return true;
	}

	static bool check_logidxdir_file( const std::string & full_filename )
	{
		if( ! is_idx_file( full_filename ) )
			return false;
		get_start_time( full_filename );
		/*
		std::string::size_type pos = full_filename.rfind("/") + 1;
		std::string serial = full_filename.substr(pos, SERIAL_LEN);
		// 不包括实时索引文件
		if ( is_digit(serial, 0, SERIAL_LEN) &&  (full_filename.at(pos + SERIAL_LEN) == SERIAL_LOGNAME_SPLIT ) )
			return true;
		*/
		return true;
	}

};

}//end namespace LOG

#endif
