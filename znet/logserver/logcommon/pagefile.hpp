#ifndef __LOG_PAGEFILE_HPP__
#define __LOG_PAGEFILE_HPP__
#include "logcommon/util.hpp"
#include "logcommon/config.hpp"
#include "logcommon/pagebuffer.hpp"
#include "logcommon/baseclass.hpp"

namespace LOG
{
class PageFile
{
	class BigFile
	{
	public:
		BigFile( const std::string &_filename, bool readonly ):filename(_filename),fd(-1)
		{ 
			open(filename, readonly); 
		}
	        BigFile( ):fd(-1){ }
	        ~BigFile( ) { close(fd);}

		ULL size() const
		{
			STRUCT_STAT statbuff;
			FSTAT( fd,  &statbuff);
			return statbuff.st_size;
		}

		bool read(void * buffer, size_t size, bool wait)
		{
			size_t len = 0;
			while ( len < size )
			{
				int ret = ::read(fd, (char *)buffer + len , size - len );
				if ( ret < 0 )
					throw LOG::Exception(errno == EINTR ? "interrupt!" : strerror(errno));

				if( ret > 0 )
					len += ret;
				else if( ret == 0 )
				{
					if( !tail( wait ) ) 
						break;
				}
			}
			if( len == size )
				return true;
			return false;
		}

		void write(void * buffer, size_t size) 
		{ 
			::write( fd, buffer, size ); 
			fsync( fd );
		}

		OFF_T current() { return LSEEK(fd, 0, SEEK_CUR); } 

		//  whence XXX
		bool seek(OFF_T offset, int whence) { return LSEEK( fd, offset, whence ) >= 0; }

	private:
	        void open(const std::string &_filename, bool readonly)
		{
			filename = _filename;
			CheckDir( filename );
			if (fd > 0)
				close(fd);
			if ( readonly )
				fd = OPEN(filename.c_str(), O_RDONLY);
			else
				fd = OPEN( filename.c_str(), O_RDWR | O_CREAT , FILE_MODE );
		}

        	bool tail( bool wait )
		{
			STRUCT_STAT stat;
			FSTAT(fd, &stat);
			time_t mTime = stat.st_mtime; 
			while (wait && Env::Inst().tail)
			{
				sleep(SLEEP);
				FSTAT(fd, &stat);
				if(mTime != stat.st_mtime)
					return true;
			}
			return false;
		}
	private:
		std::string filename;
		int fd;
	};
public:
	PageFile(const std::string &filename, bool readonly) : file(filename, readonly), timestamp(0)
        {
		if ( !readonly && (file.size() == 0) )
			writehead( filename ); 
                else
			check( readonly );
        }

	bool read( PageBuffer &page, bool wait )
	{ 
		page.offset = file.current();
		if (file.read(page.buffer, PageBuffer::SIZE, wait))
		{
			page.check();
			return true;
		}
		return false;
	}

        void write (PageBuffer & page) { file.write(page.buffer, PageBuffer::SIZE); }
        bool seek (size_t pages)       { return file.seek( pages * PageBuffer::SIZE, SEEK_SET); }
        size_t count ( )               { return file.size() <= PageBuffer::SIZE ? 0 : file.size() / PageBuffer::SIZE - 1; }
	time_t gettimestamp ( )        { return timestamp; }
private:
	void check( bool readonly )
	{
		ULL size = file.size();
		if ( ! readonly && size % PageBuffer::SIZE != 0 ) //writing check size
			throw LOG::Exception ("bad file size" );
		if ( size == 0 )
			throw LOG::Exception ("empty file " );

		PageBuffer header;
		if (!file.read(header.buffer, PageBuffer::SIZE, false)) 
			throw LOG::Exception("bad file header");

		FileHdr filehdr;
		GNET::Marshal::OctetsStream os;
		os.insert(os.begin(), header.buffer, PageBuffer::SIZE);
		if ( ! filehdr.check (os) ) throw LOG::Exception("Bad logfile");
		timestamp = filehdr.gettimestamp();

		// page count: // 0 1 2 3 , size = 4096 * 4
		// seek end = count + 1
		if ( readonly )
			seek( 1 );
		else
			seek( count() + 1 ); 
	}

	void writehead ( const std::string &logname )
	{
		GNET::Marshal::OctetsStream os;
		FileHdr filehdr;
		filehdr.create( os, logname );
		timestamp = filehdr.gettimestamp();
		PageBuffer header;
		memcpy( header.buffer, os.begin(), os.size() );
		seek( 0 );
		write( header );
	}

        BigFile file;
	time_t timestamp; //current file head timestamp;
};

} //end namespace LOG

#endif
