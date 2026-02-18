#ifndef __ROTATETIMER_HPP__
#define __ROTATETIMER_HPP__
#include "logger.hpp"
#include "logcommon/appconfig2.hpp"
#include "logcommon/controlprotocol.hpp"
#include "logcommon/util.hpp"
#include "logcommon/baseclass.hpp"
#include "logcommon/filenamerule.hpp"
#include "logcommon/pagebuffer.hpp"

namespace LOG
{

	typedef std::map<std::string, int> RotateFile;
	typedef std::set<std::string> FileSet;

class RotateTimer : public GNET::Thread::Runnable
{
	enum { SENDTO_SER_SLEEP = 5 };
	static RotateFile rotatefile;
	size_t interval ;
	std::string appFile;
	struct stat statbuff;
	time_t mTime;
        int update_time;
        RotateTimer(int _time,int _proir=1) : Runnable(_proir),update_time(_time)  //MYTODO
	{
		appFile = Config2::GetInstance()->GetApplicationFile();
		Config2::GetInstance()->Dump();
		if(appFile.empty())
			Error("RotateTimer::appFile is empty");
		if(-1 == stat(appFile.c_str(), &statbuff))
			Error("RotateTimer::appFile stat() error");
		mTime = statbuff.st_mtime;
		interval = Config2::GetInstance()->GetInterval();
	}
	~RotateTimer() { LogRec(" logrotate exit! "); }
public:
        static RotateTimer* GetInstance(int _time=60,int _proir=1) 
	{
                static RotateTimer instance(_time,_proir);
		rotatefile = Config2::GetInstance()->GetRotateFile();
                return &instance;
        }
        void Run()
        {
		try
		{
			//file_rename();
			FileSet fileset;
			for(RotateFile::iterator it = rotatefile.begin(); it != rotatefile.end(); ++it)
			{
				(*it).second =  (*it).second - interval;
				if( (*it).second <= 0 )
					fileset.insert( (*it).first );
			}
			if( !fileset.empty() )
				Rotate(fileset);
			Reset(fileset);
		}
		catch(...)
		{
			LogRec("catch exception out ..");
		}
                GNET::Thread::HouseKeeper::AddTimerTask(this,interval);
        }
	
	void Reset(const FileSet & fileset)
	{
		RotateFile rf = Config2::GetInstance()->GetRotateFile();
		/* 调整已经触发过的时间 */
		for(FileSet::iterator it = fileset.begin(); it != fileset.end(); ++it)
			rotatefile[*it] = (*(rf.find(*it))).second;
		/*只加不减了*/
		for(RotateFile::iterator it = rf.begin(); it != rf.end(); ++it)
			if( rotatefile.find((*it).first) == rotatefile.end() )
				rotatefile.insert(*it);
	}

	void Rotate(const FileSet &fileset)
	{
		filesetDump("rotate fileset ", fileset);
		std::string return_string;
		for(FileSet::iterator it = fileset.begin(); it != fileset.end(); ++it)
		{
			if ( !file_state(*it) )
				continue;
			int serial;
			std::string rotated_file(get_rotate_file(*it, serial));
			if(rotated_file.empty())
				continue;
			pidset_t pidset;
			if( get_openfile_pidset( *it, pidset ) )
			{
				rename( *it, rotated_file );
				for( pidset_t::iterator pid_it = pidset.begin(); pid_it != pidset.end(); ++pid_it )
				{
					if( rename_tail_idxfile(rotated_file, serial, *pid_it) )
						send_signal_to_tail_pid( *pid_it );
					else
						LogRec( " rename_tail_idxfile error " );
				}
			}
			else
			{
				LogRec("not to send signal " );
				rename( *it, rotated_file );
			}
		}
	}

	void rename( const std::string &src, const std::string &dest )
	{	
		if( ::rename( src.c_str(), dest.c_str()) < 0 )
		{
			LogRec( "rename error=%d src = %s dest = %s",errno, src.c_str(), dest.c_str());
		}
		else
		{
			LogRec( "rename src = %s dest = %s", src.c_str(), dest.c_str());
			ControlProtocol ctr_pro(src, ROTATE);
			LOG::Log(ctr_pro);
			sleep(SENDTO_SER_SLEEP);
		}
	}

	bool rename_tail_idxfile( const std::string &rotated_file, const int serial, const pid_t pid )
	{
		LogRec( " whill send signal pid = %d, serial = %d ", pid, serial );
                std::string logidx_dir = Config2::GetInstance()->GetParseDir();
		ReadDir readdir( logidx_dir );
		fileset_t tempset = readdir.getfileset();
		fileset_t::const_iterator itfs = tempset.begin();
		std::string logname1 = FilenameRule::get_logrotatedir_logname( rotated_file );
		bool flag = false;
		for ( ; itfs != tempset.end(); ++itfs )
		{
			pidset_t pidset;
			std::string fullpath = logidx_dir + *itfs;

			if( ! FilenameRule::is_idx_file( fullpath ) ) continue;
			if( ! FilenameRule::is_tail_idx_file( fullpath ) ) continue;
			if( FilenameRule::get_logidxdir_logname( fullpath ) != logname1 ) continue;
			if( ! get_openfile_pidset( fullpath, pidset ) ) continue;
			if( pidset.size() == 1  && pidset.find( pid ) != pidset.end() )
			{
				char buff[11];
				sprintf(buff, "%05d", serial);
				std::string src = logidx_dir + *itfs;
				std::string dest;
				dest.append( logidx_dir ).append( buff ).append( "_" ).append( *itfs );
				if( ::rename( src.c_str(), dest.c_str()) < 0 )
				{
					LogRec( "rename idx error src = %s, dest = %s ", src.c_str(), dest.c_str() );
					flag = false;
				}
				else
				{
					LogRec( "rename idx ok src = %s, dest = %s ", src.c_str(), dest.c_str() );
					flag = true;
				}
			}
			else if ( pidset.size() > 1 )
			{
				Error( "multi pid open idx file:" + *itfs );
			}
                }
		return flag;
	}
	
	bool send_signal_to_tail_pid( pid_t pid )
	{
		if( -1 == kill( pid, SIGCHLD ) )
		{
			LogRec( " send kill error.  pid = %d", pid );
			return false;
		}
		LogRec(" send kill ok. pid = %d", pid);
		return true;
	}

	int GetSerial(const std::string &dir)
	{
		ReadDir readdir( dir );
		fileset_t tempset = readdir.getfileset();
		fileset_t::const_iterator it = tempset.begin();
		int ser = 0;
		for ( ; it != tempset.end(); ++it )
		{
			std::string serial = FilenameRule::get_serial( dir + *it );
//			printf( "str serial = %s", serial.c_str() );
			if( ! serial.empty() && atoi( serial.c_str() ) > ser )
				ser = atoi( serial.c_str() );
		}
		return ++ser;
	}

	std::string get_rotate_file(const std::string &rfile, int &serial )
	{
		char buff[11];
		serial = GetSerial( Config2::GetInstance()->GetRotateDir());
		sprintf(buff, "%05d", serial ); 
		std::string filename(Config2::GetInstance()->GetRotateDir());
		filename.append(buff).append("_");
		std::string name = rfile.substr(rfile.find_last_of('/') + 1);
		
		filename.append( name.substr(0, name.rfind(".")) );
		std::string start_time = get_file_start_time(rfile);
		if(start_time.empty())
			return "";
		filename.append(".").append(start_time).append(".log");
		LogRec("get_rotate_file() src = %s ==> dest == %s", rfile.c_str(), filename.c_str());
		return filename;
	}

	void filesetDump(const std::string &msg, const FileSet &fileset)
	{
		LogRec( "%s :dump file set...", msg.c_str());
		for(FileSet::iterator mit = fileset.begin(); mit != fileset.end(); ++mit)
			 LogRec( "fileset filename = %s", (*mit).c_str());
		LogRec( "%s :dump file set end", msg.c_str());
	}

	bool file_state(const std::string &filename)
	{
		if( 0 == access(filename.c_str(), R_OK) )
		{
			STRUCT_STAT statbuff;
			STAT(filename.c_str(), &statbuff);
			if( statbuff.st_size > PageBuffer::SIZE)
				return true;
		}
		return false;
	}
	
};

}
#endif
