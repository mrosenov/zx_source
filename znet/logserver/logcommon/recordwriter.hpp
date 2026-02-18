#ifndef __LOG_RECORDWRITER_HPP__
#define __LOG_RECORDWRITER_HPP__

#include "logcommon/pagebuffer.hpp"
#include "logcommon/pagefile.hpp"
namespace LOG
{

class RecordWriter
{
public:
	RecordWriter(const std::string &filename): pages(filename, false) { }

	~RecordWriter() 
	{
		if( ! page.empty() )
			pages.write( page ); 
	}

	void write( const void *record, size_t size, const time_t time )	
	{
		flush(time);
		size_t wlen = page.fill_record( (char*)record, size );
		while( wlen < size )
		{
			flush(time);
			size_t len = std::min( size - wlen, page.free_space() );
			size_t fill = page.fill_piece( (char*)record + wlen, len );
			wlen += fill;
		}
	}
	
private:
	void flush( const time_t time )
	{
		if( page.free_space() <= sizeof(RecordHead) )
		{
			page.fill_null();
			pages.write( page );
			page.reset( time );
		}
	}

	PageFile pages;
	PageBuffer page;
};

} // end namespace LOG
#endif
