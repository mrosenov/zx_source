#include "marshal.h"
#include "logcommon/util.hpp"
#include "logcommon/config.hpp"
#include "logcommon/exception.hpp"
#include "logcommon/pagefile.hpp"

namespace LOG
{

#if defined _FILE_OFFSET_BITS
	int OPEN(const char *pathname, int flags) 
	{ 
		int ir = open64(pathname, (off64_t)flags); 
		if(ir == -1)
		{
			std::ostringstream oss;
			oss << "open64 == -1, pathname = " << pathname << " flags = " << flags;
			Error(oss.str());
		}
		return ir;
	}

	int OPEN(const char *pathname, int flags, mode_t mode) 
	{ 
		int ir = open64(pathname, (off64_t)flags, mode); 
		if(ir == -1)
		{
			std::ostringstream oss;
			oss << "open64 == -1, pathname = " << pathname << " flags = " << flags << " mode = " << mode;
			Error(oss.str());
		}
		return ir; 
	}

	FILE * FOPEN(const char *path, const char *mode) 
	{ 
		FILE *fp = fopen64(path, mode); 
		if(fp == NULL)
		{
			std::ostringstream oss;
			oss << "fopen64 == NULL, path = " << path << " mode = " << mode;
			Error(oss.str());
		}

		return fp;
	}

	off64_t LSEEK(int fildes, off64_t offset, int whence) 
	{ 
		off64_t ot = lseek64(fildes,(off64_t)offset, whence); 
		if(ot == -1)
		{
			std::ostringstream oss;
			oss << "lseek64 == -1, fieldes = " << fildes << " offset = " << offset << " whence =" << whence;
			Error(oss.str());
		}
		return ot;
	}

	int STAT(const char *file_name, struct stat64 *buf) 
	{ 
		int st = stat64(file_name, buf); 
		if( -1 == st )
		{
			std::ostringstream oss;
			oss << "stat64 == -1 file_name = " << file_name;
			Error(oss.str());
		}
		return  st;
	}

	int FSTAT(int filedes, struct stat64 *buf) 
	{ 
		int st =  fstat64(filedes, buf); 
		if( -1 == st )
		{
			std::ostringstream oss;
			oss << "fstat64 == -1 filedes = " << filedes;
			Error(oss.str());
		}
		return st;
	}

	int LSTAT(const char *file_name, struct stat64 *buf) 
	{ 
		int st = lstat64(file_name, buf); 
		if( -1 == st )
		{
			std::ostringstream oss;
			oss << "lstat64 == -1 file_name = " << file_name;
			Error(oss.str());
		}
		return  st;
	}

#else
	int OPEN(const char *pathname, int flags) 
	{ 
		int ir = open(pathname, (off_t)flags); 
		if( -1 == ir )
		{
			std::ostringstream oss;
			oss << "open == -1 pathname = " << pathname << " flags = " << flags;
			Error(oss.str());
		}
		return ir;
	}

	int OPEN(const char *pathname, int flags, mode_t mode) 
	{
		int ir = open(pathname, (off_t)flags, mode); 
		if( -1 == ir )
		{
			std::ostringstream oss;
			oss << "open == -1 pathname = " << pathname << " flags = " << flags << " mode = " << mode;
			Error(oss.str());
		}
		return  ir;
	}

	off_t LSEEK(int fildes, off_t offset, int whence) 
	{ 
		off_t ir = lseek(fildes,(off_t)offset, whence); 
		if(ir == -1)
		{
			std::ostringstream oss;
			oss << "lseek == -1, fildes = " << fildes << " offset = " << offset << " whence " << whence;
			Error(oss.str());
		}
		return ir; 
	}
	
	int STAT(const char *file_name, struct stat *buf) 
	{ 
		int ir = stat(file_name, buf); 
		if( -1 == ir )
		{
			std::ostringstream oss;
			oss << "stat == -1 file_name = " << file_name;
			Error(oss.str());
		}
		return ir;
	}
	
	int FSTAT(int filedes, struct stat *buf) 
	{ 
		int ir = fstat(filedes, buf); 
		if( -1 == ir )
		{
			std::ostringstream oss;
			oss << "fstat == -1  fileds  = " << filedes;
			Error(oss.str());
		}	return 
	}
	
	int LSTAT(const char *file_name, struct stat *buf) 
	{ 
		int ir = lstat(file_name, buf); 
		if( -1 == ir )
		{
			std::ostringstream oss;
			oss << "lstat == -1 file_name = " << file_name;
			Error(oss.str());
		}
		return 
	}
	
	FILE * FOPEN(const char *path, const char *mode) 
	{ 
	
		FILE * fp = fopen(path, mode); 
		if(fp == NULL)
		{
			std::ostringstream oss;
			oss << "fopen == NULL path = " << path << " mode = " << mode;
			Error(oss.str());
		}
		return fp;
	}
#endif

	int ACCESS(const char *pathname, int mode)
	{
		int ret;
		if( (ret = access(pathname, R_OK)) < 0 )
		{
			std::ostringstream oss;
			oss << "access error";
			Error(oss.str());
		}
		return ret;
	}

	ssize_t READ(int fd, void * buf, size_t count)
	{
		ssize_t sr = read(fd, buf, count);
		if(-1 == sr)
		{
			std::ostringstream oss;
			oss << "read == -1 fd = " << fd << " count = " << count << " time = " << time(NULL);
			Error(oss.str());
		}
		return sr;

	}

	int RENAME(const char * oldpath,const char * newpath)
	{
		int ret =  rename( oldpath, newpath );
		if( ret < 0 )
			LogRec(" rename error src = %s, dest = %s", oldpath, newpath );
		return ret;
	}

	void err(char *msg)
	{
		printf("%s\n", msg);
		exit(0);
	}
	
	std::string get_today()
	{
		long t;
		time(&t);
		struct tm *tp;
		tp = localtime(&t);
		if( tp->tm_year > 69 )
			tp->tm_year += 1900;
		else
			tp->tm_year += 2000;
		char buff[9];		
		sprintf(buff, "%04d%02d%02d", tp->tm_year, tp->tm_mon+1, tp->tm_mday);
		std::string today(buff);
		return today;
	}
	
	std::string get_cur_time()
	{
		long t;
		time(&t);
		struct tm *tp;
		tp = localtime(&t);
		char cur_time[9];
		sprintf(cur_time, "%02d:%02d:%02d", tp->tm_hour, tp->tm_min, tp->tm_sec);
		std::string ct(cur_time);
		return ct;
	}
	
	std::string get_day_time()
	{
		long t;
		time(&t);
		struct tm *tp;
		tp = localtime(&t);
		if( tp->tm_year > 69 )
			tp->tm_year += 1900;
		else
			tp->tm_year += 2000;
		char daytime[20];
		sprintf(daytime, "%04d%02d%02d %02d:%02d:%02d", 
				tp->tm_year, tp->tm_mon+1, tp->tm_mday,tp->tm_hour, tp->tm_min, tp->tm_sec);
		return daytime;
	}

	std::string get_dt()
	{
		long t;
		time(&t);
		struct tm *tp;
		tp = localtime(&t);
		if( tp->tm_year > 69 )
			tp->tm_year += 1900;
		else
			tp->tm_year += 2000;
		char daytime[20];
		sprintf(daytime, "%04d%02d%02d%02d%02d%02d", 
			tp->tm_year, tp->tm_mon+1, tp->tm_mday,tp->tm_hour, tp->tm_min, tp->tm_sec);
		return daytime;
	}

	int getmon(char *mon) 
	{
		int i;
		static char *name[]={
			"illegal month", "Jan", "Feb", "Mar", "Apr", "May",
			"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
		};
		for(i=1; strcmp(mon,name[i])!=0; i++)
			if(i >= 12)
				break;
		return(i);
	}

	void mygetdate(int *year,int *month,int *day,int *hour,int *minute,int *second)
	{
		int	year0,month0,day0;
		char	mon[4],tod[4];
		long	ymd;
		time(&ymd);
		sscanf(ctime(&ymd),"%s %s %d %d:%d:%d %d",tod,mon,&day0,hour,minute,
				second,&year0);
		month0 = getmon(mon);

		*year = year0;	*month = month0;	*day = day0;
	}

	void LogRec( char *Fmt, ...)
	{
		char dtstr[100], fname[100];
		int year,month,day,hour,minute,second;
		FILE *fp;
		va_list Arg;
		char buf[200]; 
		getcwd(buf,sizeof(buf));

		mygetdate(&year,&month,&day,&hour,&minute,&second);
		sprintf(dtstr," %02d%02d%02d,%02d:%02d:%02d ",year%100,month,day,hour,minute,second);
		sprintf(fname,"%s/logsys.log", buf);
		fp=FOPEN(fname, "a");
		if(fp!=NULL) {
			fprintf(fp,"%s|%d|",dtstr, getpid());
			va_start(Arg, Fmt);
			vfprintf(fp, Fmt, Arg);
			va_end(Arg);
			fprintf(fp,"\n");
			fclose(fp);
		}
	}

	int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
	{
		struct flock    lock;

		lock.l_type = type;             /* F_RDLCK, F_WRLCK, F_UNLCK */
		lock.l_start = offset;  /* byte offset, relative to l_whence */
		lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
		lock.l_len = len;               /* #bytes (0 means to EOF) */

		return( fcntl(fd, cmd, &lock) );
	}

	pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
	{
		struct flock    lock;

		lock.l_type = type;             /* F_RDLCK or F_WRLCK */
		lock.l_start = offset;  /* byte offset, relative to l_whence */
		lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
		lock.l_len = len;               /* #bytes (0 means to EOF) */

		if (fcntl(fd, F_GETLK, &lock) < 0)
		{
			LogRec("lock_test() error fcntl < 0");
		}

		if (lock.l_type == F_UNLCK)
			return(0);              /* false, region is not locked by another proc */
		return(lock.l_pid);     /* true, return pid of lock owner */
	}

	time_t maketime( const std::string  &_date)
	{
		std::string date = _date;
		if (date.empty() || date == "0")
			return 0;
		if (date.length() > 14)
			throw LOG::Exception("defined time is bad: " + date);
		const char i = date[0];
		if (atoi(&i) > 2)
			return 0;
		for(std::string::iterator it = date.begin(); it != date.end(); ++it)
			if(! isdigit(*it))
				throw LOG::Exception("defined time is bad: " + date);
		switch(date.length())
		{
			case 4:
				date.append("0101000000");
				break;

			case 6:
				date.append("01000000");
				break;
			case 8:
				fill_n(back_inserter(date), 14 - date.length(), '0');
				break;
			default:
				fill_n(back_inserter(date), 14 - date.length(), '0');
				break;
		}
		struct tm tm;
		strptime(date.c_str(), "%Y%m%d%H%M%S", &tm);
		// strptime("2006-09-27 16:16:01", "%Y-%m-%d %H:%M:%S", &tm);
		time_t t = mktime(&tm);
		//	#define MIN_LIMIT_TIME 283996860 /*date = 19790101080100  time = 283996860 */
		//#define MAX_LIMIT_TIME 2147483647 /*date = 20500121280100  time = 2147483647*/
		if(t < MIN_LIMIT_TIME || t > MAX_LIMIT_TIME)
			throw LOG::Exception("defined time is bad: " + date);
		return t;
	}

	bool str_is_time( const std::string &date )
	{
		if (date.length() != 14)
			return false;
		for(std::string::const_iterator it = date.begin(); it != date.end(); ++it)
		{
			if(! isdigit(*it))
				return false;
		}
		struct tm tm;
		strptime(date.c_str(), "%Y%m%d%H%M%S", &tm); // strptime("2006-09-27 16:16:01", "%Y-%m-%d %H:%M:%S", &tm);
		time_t t = mktime(&tm);
		//	#define MIN_LIMIT_TIME 283996860 /*date = 19790101080100  time = 283996860 */
		//#define MAX_LIMIT_TIME 2147483647 /*date = 20500121280100  time = 2147483647*/
		if(t < MIN_LIMIT_TIME || t > MAX_LIMIT_TIME)
			return false;
		return true;

	}

	std::string remaketime(const time_t time)
	{
		if(time < 0)
			return "";

		struct tm *tp;
		tp = localtime(&time);
		if( tp->tm_year > 69 )
			tp->tm_year += 1900;
		else
			tp->tm_year += 2000;
		char daytime[20];
		memset(daytime, '\0', 20);
		sprintf(daytime, "%04d%02d%02d %02d:%02d:%02d", \
				tp->tm_year, tp->tm_mon+1, tp->tm_mday,tp->tm_hour, tp->tm_min, tp->tm_sec);
		return daytime;
	}

	std::string time2str(const time_t time)
	{
		if(time < 0)
			return "";

		struct tm *tp;
		tp = localtime(&time);
		if( tp->tm_year > 69 )
			tp->tm_year += 1900;
		else
			tp->tm_year += 2000;
		char daytime[20];
		memset(daytime, '\0', 20);
		sprintf(daytime, "%04d%02d%02d%02d%02d%02d", \
				tp->tm_year, tp->tm_mon+1, tp->tm_mday,tp->tm_hour, tp->tm_min, tp->tm_sec);
		return daytime;
	}

	/////////////////////////////////////////////////////////////////
	//
	// 检查目录,不存在则创建
	//
	/////////////////////////////////////////////////////////////////
	void CheckDir(const std::string &fn)
	{
		std::string::size_type pos = fn.find_first_of('/');
		if( pos == std::string::npos ) return;
		while( pos != std::string::npos )
		{
			struct stat buff;
			const char *pn = fn.substr(0, pos).c_str();
			if( -1 == stat(pn, &buff) )
			{
				if( errno == ENOENT )
					mkdir(pn, 0775);
				else
					throw LOG::Exception("CheckDir() error");
			}
			else
			{
				if( !(S_ISDIR(buff.st_mode)) )
					throw Exception("CheckDir() error");
			}

			pos = fn.find_first_of('/', pos+1);
		}
	}

	void Error(const std::string &err)
	{
		perror(err.c_str());
		LogRec("%s",err.c_str());
		throw LOG::Exception(err);
	}

	void Waring(const std::string &msg)
	{
		std::cerr << msg << std::endl;
		LogRec("%s", msg.c_str());
	}

	void sleep2(double f)
	{
		struct timeval val;
		char buf[100];
		sprintf(buf, "%f",f);
		std::string s(buf);
		val.tv_sec =atoi(s.substr(0,s.find('.')).c_str()) / 1000000;
		val.tv_usec = atoi(s.substr(s.find('.') + 1).c_str()) % 1000000;
		select(0, NULL, NULL, NULL, &val);
	}

	bool checkFilename(const std::string & fn)
	{
		STRUCT_STAT statbuff;
		STAT(fn.c_str(), &statbuff);
		if(S_ISDIR(statbuff.st_mode))
			return false;
		ACCESS(fn.c_str(), R_OK);
		return true;
	}

	std::string get_file_start_time(const std::string &rfile)
	{
		PageFile pages( rfile, true );
		return time2str(pages.gettimestamp());
	}

	bool is_digit( const std::string &str, const std::string::size_type pos, size_t com_len )
	{
		if( str.length() < com_len || str.length() - pos < com_len )
			return false;
		for( size_t i = 0; i < com_len ; ++i )
		{
			if( !::isdigit(str[pos + i]) )
				return false;
		}
		return true;
	}

	void ltrim( std::string &str )
	{
		while( str[0] == ' ' )
			str.erase( 0, 1 );
	}

	void rtrim( std::string &str )
	{
		while( str[ str.length() - 1 ] == ' ' )
			str.erase( str.length() - 1, 1 );
	}
	
	std::string ltrim( const std::string &str )
	{
		std::string ret = str;
		while( ret[0] == ' ' )
			ret.erase( 0, 1 );
		return ret;
	}

	std::string rtrim( const std::string &str )
	{
		std::string ret = str;
		while( ret[ ret.length() - 1 ] == ' ' )
			ret.erase( ret.length() - 1, 1 );
		return ret;
	}
	
	void eat_mid_blank( std::string &str )
	{
		bool eat = false;
		ltrim(str);
		rtrim(str);
		for( size_t i = 0 ; i < str.length(); ++i )
		{
			while( str.at(i) == ' ' )
			{
				str.erase(i, 1);
				eat = true;
			}
			if( eat )
			{
				eat = false;
				str.insert( i, " " );
			}
		}
	}	

	bool get_openfile_pidset( const std::string &opend_file, pidset_t &pidset )
        {
                std::string cmd("/usr/sbin/lsof ");
		char buff[11];
		sprintf(buff, "%d", getpid() );
                std::string tmpfile("/tmp/get_openfile_pidset");
		tmpfile.append(buff);
                cmd.append(opend_file).append(" > ").append(tmpfile);
                if( -1 == system(cmd.c_str()) )
                {
                        LogRec( "lsof cmd error, cmd = %s", cmd.c_str() );
                        return false;
                }
                std::ifstream ifs( tmpfile.c_str() );
                if( !ifs.is_open() )
                {
                        LogRec( "open file = %s error  " , tmpfile.c_str() );
                        return false;
                }

                std::string line;
                std::getline( ifs, line ); //skip common line
                while( !ifs.eof() )
                {
                        std::getline( ifs, line );
			eat_mid_blank( line );
                        if( !line.empty() && line.find(LOGSERVER_NAME, 0, strlen(LOGSERVER_NAME)) 
                                        == std::string::npos )
                        {
                                std::string::size_type pos = line.find( " " ) + 1;
                                std::string pid = line.substr( pos, line.find(" ", pos) - pos );
                                pidset.insert( atoi(pid.c_str()) );
                        }
                }
		remove( tmpfile.c_str() );
		ifs.close();
                return !pidset.empty();
        }

	void split_str( const std::string &str, const std::string split, fileset_t &fileset )
	{
		std::string tmpstr = str;
		std::string::size_type pos1 = 0;
		std::string::size_type pos2;
		ltrim(tmpstr);
		rtrim(tmpstr);
		eat_mid_blank(tmpstr);
		tmpstr.append(split);
		while( ( pos2 = tmpstr.find(split, pos1) ) != std::string::npos )
		{
			fileset.insert( tmpstr.substr(pos1, pos2 - pos1) );
			pos1 = pos2 + 1;
		}
	}

}//end namespace LOG
