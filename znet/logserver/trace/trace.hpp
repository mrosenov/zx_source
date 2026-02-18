#ifndef __LOG_TRACE_HPP__
#define __LOG_TRACE_HPP__

#include "logcommon/recordreader.hpp"

namespace LOG
{

class Trace
{
public:
	typedef std::set< GNET::Protocol::Type > type_set_t;
	Trace ( const std::string &filename, bool wait, type_set_t *ts = NULL )
		:recordreader(filename, wait), type_set(ts)
	{ }

	void tracetime( time_t begin_time )
	{
		recordreader.printptime();
		if (recordreader.seektime(begin_time) ) 
			return _trace();
		std::cout << "seektime time error" << std::endl;
	}

	void traceline(int lines) 
	{ 
		if ( recordreader.seektail( lines ) )
			return _trace();
		std::cout << "seekline time error" << std::endl;
	}

	void trace()
	{
		recordreader.seektime(0);
		_trace();
	}
private:
	void _trace()
	{
		while( true )
		{
			GNET::Marshal::OctetsStream data;
			RecordHead rh;
			if ( !recordreader.read( data, rh ) ) break;
			if ( type_set && type_set->find( rh.type ) == type_set->end() ) continue;
			if ( GNET::Protocol * protocol = GNET::Protocol::Create( rh.type ) )
			{
				data >> *protocol;
				std::cout << rh << *protocol << std::endl;
				protocol->Destroy();
			}
			else
				std::cout << "UNKNOWN PROTOCOL type=" << rh.type << std::endl;
		}
	}


	RecordReader recordreader;
	type_set_t *type_set;
};

} //end namespace LOG

#endif
