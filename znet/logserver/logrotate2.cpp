#include "logger.hpp"
#include "logcommon/util.hpp"
#include "logcommon/appconfig2.hpp"
#include "logcommon/controlprotocol.hpp"
#include "logcommon/baseclass.hpp"
#include "logcommon/filenamerule.hpp"
#include "logcommon/pagebuffer.hpp"
//#include "thread.h"

namespace LOG
{

class LogRotate : public GNET::Thread::Runnable
{
	std::string rotatefile;
	enum { SLEEPTIME = 10 };
public:
	LogRotate(std::string _rf):rotatefile(_rf) { }

	void Run()
	{
		Rotate();
		sleep(SLEEPTIME);
		kill(getpid(), 9);
	}

	void Rotate()
	{
		fprintf(stdout, "rotate file = %s\n", rotatefile.c_str());
		std::string return_string;
		if ( !file_state(rotatefile) ) return;
		int serial;
		std::string rotated_file(get_rotate_file(rotatefile, serial));
		if(rotated_file.empty()) return;
		pidset_t pidset;
		if( get_openfile_pidset( rotatefile, pidset ) )
		{
			rename( rotatefile, rotated_file );
			for( pidset_t::iterator pid_it = pidset.begin(); pid_it != pidset.end(); ++pid_it )
			{
				if( rename_tail_idxfile(rotated_file, serial, *pid_it) )
					send_signal_to_tail_pid( *pid_it );
				else
					fprintf(stdout, " rename_tail_idxfile error \n" );
			}
		}
		else
		{
			fprintf(stdout,  "not to send signal \n" );
			rename( rotatefile, rotated_file );
		}
	}

private:
	void rename( const std::string &src, const std::string &dest )
	{	
		if( ::rename( src.c_str(), dest.c_str()) < 0 )
		{
			fprintf(stdout, "rename error=%d src = %s dest = %s\n",errno, src.c_str(), dest.c_str());
		}
		else
		{
			fprintf(stdout, "rename src = %s dest = %s\n", src.c_str(), dest.c_str());
			ControlProtocol ctr_pro(src, ROTATE);
			LOG::Log(ctr_pro);
			sleep(SLEEPTIME);
		}
	}

	bool rename_tail_idxfile( const std::string &rotated_file, const int serial, const pid_t pid )
	{
		fprintf(stdout,  " whill send signal pid = %d, serial = %d \n", pid, serial );
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
				snprintf( buff, sizeof(buff), "%05d", serial);
				std::string src = logidx_dir + *itfs;
				std::string dest;
				dest.append( logidx_dir ).append( buff ).append( "_" ).append( *itfs );
				if( ::rename( src.c_str(), dest.c_str()) < 0 )
				{
					fprintf(stdout, "rename idx error src = %s, dest = %s \n", src.c_str(), dest.c_str() );
					flag = false;
				}
				else
				{
					fprintf(stdout,  "rename idx ok src = %s, dest = %s \n", src.c_str(), dest.c_str() );
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
			fprintf(stdout,  " send kill error.  pid = %d\n", pid );
			return false;
		}
		fprintf(stdout, " send kill ok. pid = %d\n", pid);
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
//			fprintf(stdout,  "str serial = %s", serial.c_str() );
			if( ! serial.empty() && atoi( serial.c_str() ) > ser )
				ser = atoi( serial.c_str() );
		}
		return ++ser;
	}

	std::string get_rotate_file(const std::string &rfile, int &serial )
	{
		char buff[11];
		serial = GetSerial( Config2::GetInstance()->GetRotateDir());
		snprintf( buff, sizeof(buff), "%05d", serial ); 
		std::string filename(Config2::GetInstance()->GetRotateDir());
		filename.append(buff).append("_");
		std::string name = rfile.substr(rfile.find_last_of('/') + 1);
		
		filename.append( name.substr(0, name.rfind(".")) );
		std::string start_time = get_file_start_time(rfile);
		if(start_time.empty())
			return "";
		filename.append(".").append(start_time).append(".log");
		fprintf(stdout, "get_rotate_file() src = %s ==> dest == %s\n", rfile.c_str(), filename.c_str());
		return filename;
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
} //end namespace

int main(int argc, char *argv[])
{
	if( argc != 4 || access(argv[1], R_OK) == -1 || access(argv[2], R_OK) == -1 || access(argv[3], R_OK) == -1 )
        {
                std::cout << "Usage:" << argv[0] << " config_file xmlfile rotatefile " << std::endl;
                exit(0);
        }
	GNET::Conf::GetInstance(argv[1]);
	LOG::Log_Init();
	LOG::Config2::GetInstance(argv[2]);
	GNET::Thread::HouseKeeper::AddTimerTask(new LOG::LogRotate(argv[3]),1);
	//GNET::Thread::Pool::AddTask(new LOG::LogRotate(argv[3]));
	GNET::Thread::Pool::AddTask(GNET::PollIO::Task::GetInstance());
	GNET::Thread::Pool::Run();
	return 0;
}
