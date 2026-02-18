#ifndef __LOG_CONFIG_HPP__
#define __LOG_CONFIG_HPP__

#include <vector>
#include <set>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "marshal.h"

namespace LOG
{

#define SERIAL_LOGNAME_SPLIT '_'
#define FILENAME_SPLIT " "
#define FILE_MODE 0666
#define MIN_LIMIT_TIME 283996860 /*date = 19790101080100  time = 283996860 */
#define MAX_LIMIT_TIME 2147483647 /*date = 20500121280100  time = 2147483647*/
#define LOGSERVER_NAME "logserver"
#define CREATE_IDX_MIN_SIZE 102400
#define SERIAL_LEN 5
#define MAKER "maker"
#define QUERY "query"
#define LOG_EXT "log"
#define IDX_EXT "idx"
#define SLEEP 5
#define TIMEOUT 10
#define ROTATE	1 

#define _FILE_OFFSET_BITS 64
#if defined _FILE_OFFSET_BITS
	typedef struct stat64 STRUCT_STAT;
	typedef unsigned long long ULL;
	typedef off64_t OFF_T;
#else
	typedef size_t ULL;
	typedef struct stat STRUCT_STAT;
	typedef off_t OFF_T;
#endif

	typedef size_t type_t;
	typedef std::set< std::string > fileset_t;
	typedef std::set< std::string > FS;
	typedef std::vector< long > ValueSet;
	typedef std::vector< std::string > StrVec;
	typedef std::vector< StrVec > IdxVec;
	typedef std::map< type_t, fileset_t > type_file_map_t;
        typedef std::set< std::string > lognameset_t;
	typedef GNET::Marshal::OctetsStream keyvalue_t;
	typedef std::set< std::string > fileset_t;
	typedef std::map< std::string , std::string > filemap_t;
	typedef std::set< pid_t > pidset_t;
	
	typedef std::set< type_t > denyset_t;
        typedef std::map< std::string, int > RotateFile;


} //end namespace LOG
#endif
