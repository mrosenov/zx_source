#ifndef __LOG_UTIL_HPP__
#define __LOG_UTIL_HPP__

#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <sstream>
#include "config.hpp"
#include "exception.hpp"

namespace LOG
{

	int OPEN(const char *pathname, int flags, mode_t mode);
	int OPEN(const char *pathname, int flags);
	ssize_t READ(int, void *, size_t);
	int ACCESS(const char *pathname, int mode);
#if defined _FILE_OFFSET_BITS
	off64_t LSEEK(int fildes, off64_t offset, int whence);
	int STAT(const char *file_name, struct stat64 *buf);
	int FSTAT(int filedes, struct stat64 *buf);
	int LSTAT(const char *file_name, struct stat64 *buff);
#else
	off_t LSEEK(int fildes, off_t offset, int whence);
	int STAT(const char *file_name, struct stat *buf);
	int FSTAT(int filedes, struct stat *buf);
	int LSTAT(const char *file_name, struct stat *buff);
#endif

#define	read_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define	readw_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define	write_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define	writew_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define	un_lock(fd, offset, whence, len) \
	lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

	int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

	pid_t	lock_test(int, int, off_t, int, off_t);
	/* {Prog locktest} */

#define	is_readlock(fd, offset, whence, len) \
	lock_test(fd, F_RDLCK, offset, whence, len)
#define	is_writelock(fd, offset, whence, len) \
	lock_test(fd, F_WRLCK, offset, whence, len)

	void err(char *msg);
	std::string get_today();
	std::string get_cur_time();
	std::string get_day_time();
	std::string get_dt();
	int getmon(char *mon);
	void mygetdate(int *year,int *month,int *day,int *hour,int *minute,int *second);
	void LogRec( char *Fmt, ...);
	time_t maketime( const std::string &date);
	std::string remaketime(const time_t );
	std::string time2str(const time_t );
	void CheckDir(const std::string &);
	void Error(const std::string &);
	void Waring(const std::string &);
	void sleep2(double f);
	bool checkFilename(const std::string & fn);
	std::string getStartEnd(const std::string &rfile);
	std::string get_file_start_time(const std::string &rfile);
	int RENAME(const char * oldpath,const char * newpath);
	bool str_is_time( const std::string &date );
	bool is_digit( const std::string &str, const std::string::size_type pos, size_t com_len );
	void ltrim( std::string &str );
	void rtrim( std::string &str );
	std::string ltrim( const std::string &str );
	std::string rtrim( const std::string &str );
	bool get_openfile_pidset( const std::string &opend_file, pidset_t &pidset );
	void split_str( const std::string &str, const std::string split, fileset_t &fileset );
}//end namespace LOG

#endif
