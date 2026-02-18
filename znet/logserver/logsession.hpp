#ifndef __LOG_LOGSESSION_HPP__
#define __LOG_LOGSESSION_HPP__
#include "protocol.h"
#include "logrecord.hpp"
#include <time.h>
#include <sstream>
#include "logcommon/controlprotocol.hpp"
namespace LOG
{
class LogSession : public GNET::Protocol::Manager::Session
{
	virtual void OnRecv()
	{
		timer.Reset();
                GNET::Octets& input = Input();
                is.insert( is.end(), input.begin(), input.end() );
                input.clear();
		size_t count = 0;
                while( _Decode(is) )
                {
                          count ++;
                }
	}

	static bool _Decode( const GNET::Protocol::Manager::Session::Stream &is )
        {
                if ( is.eos() )
                        return false;

                try
                {
			RecordHead rh;
			// package  : compat(type) + Octects
			// Octects  : compat(size) + protocol
			// protocol : sample_id + sample_name
                        is >> GNET::Marshal::Begin;
			is >> GNET::CompactUINT( rh.type ) >> GNET::CompactUINT(rh.rlen);

			if ( rh.type == PROTOCOL_CONTROLPROTOCOL )
			{
				std::cout <<  "recv contro protocol " << std::endl;
				LogRec("recv contro protocol ");
				ControlProtocol ctl_pro;
				is >> ctl_pro;
				if( ctl_pro.commond ==  ROTATE )
					LOG::LogManager::GetInstance( )->Rotate( ctl_pro.content );
				is >> GNET::Marshal::Commit;
				return true;
			}

			// Record : rh + compat(size) + protocol
			// Record : rh + protocol

			rh.time = time( NULL );
			rh.rlen += RecordHead::SIZE; // protocol-size -> fullrecord-size

                        GNET::Marshal::OctetsStream m_os;
                        m_os << rh;
			GNET::Octets & x = m_os;
			x.reserve(rh.rlen);
			is.pop_byte((char*)x.end(), rh.rlen - RecordHead::SIZE); // pop protocol
			x.resize(rh.rlen);
			is >> GNET::Marshal::Commit;
			// m_os.size()=121
			//printf("is.size() = %d m_os.size()=%d\n", is.size(), m_os.size() );
                        LOG::LogManager::GetInstance( )->Write( rh, m_os.begin(), m_os.size());
			return  true;
		}
		catch ( Marshal::Exception &e )
                {
                         //fprintf(stderr,"%d(type=%d,size=%d) Decode Error.\n", ii++, type, size);
                         //fprintf(stderr,"(type=%d,size=%d) Decode Error.\n", type, size);
                         is >> GNET::Marshal::Rollback;
			 return false;
                }
		catch( LOG::Exception & e )
		{
			LogRec( "logsession catch exception: %s", e.what().c_str() );
			fprintf( stderr,"logsession catch exception: %s", e.what().c_str() );
			return false;
		}
	}

public:
	LogSession( GNET::Protocol::Manager * manager ):Session( manager ) 
	{  
		//std::cout << "create  LogSession" << std::endl; 
	}

	LogSession( const LogSession & ols ):Session( ols ) 
	{ 	
		//std::cout << "create  LogSession by ref" << std::endl; 
	}

	NetSession *Clone( ) const  
	{ 
		//std::cout << "new LogSession" << std::endl; 
		return new LogSession( *this ); 
	}
};

}//end namespace LOG

#endif
