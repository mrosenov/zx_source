#include <stdio.h>

#include "statistic.h"
#include "log.h"

#ifdef USE_LOG2
#include "logger.hpp"
#endif

#include "octets.h"
#include "conf.h"
#include "thread.h"
#include "logclientclient.hpp"
#include "logclienttcpclient.hpp"
#include "statinfovital.hpp"
#include "statinfo.hpp"
#include "remotelogvital.hpp"
#include "remotelog.hpp"

namespace GNET
{
	std::string g_hostname;
	std::string g_progname;

	void Log::setprogname( const char * __name )
	{
		instance().m_progname = __name;
		instance().LogFacility = LOG_LOCAL0;

		openlog( __name, LOG_CONS | LOG_PID, LOG_LOCAL0 );

		char buffer[256];
		memset( buffer, 0, sizeof(buffer) );
		gethostname( buffer, sizeof(buffer)-1 );
		g_hostname = buffer;
		g_progname = __name;

		Conf *conf = Conf::GetInstance();
		{
			LogclientClient *manager = LogclientClient::GetInstance();
			manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
			Protocol::Client(manager);
		}
		{
			LogclientTcpClient *manager = LogclientTcpClient::GetInstance();
			manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
			Protocol::Client(manager);
		}
	#ifdef USE_LOG2
		LOG::Log_Init();
	#endif
	}

	void Log::vlog( int __priority, const char * __fmt, va_list ap )
	{
		char buffer[1024];
		int len = vsnprintf( buffer, sizeof(buffer), __fmt, ap );
		len = len>=(int)sizeof(buffer) ? sizeof(buffer)-1 : len ;
		if( len > 0 )
		{
			RemoteLog rmtlog( __priority, std::string(buffer,len), g_hostname, g_progname) ;
			if( LogclientClient::GetInstance()->SendProtocol(rmtlog) )
			{
				if( Thread::Pool::Size() > 1 )
					PollIO::WakeUp();
			}
			else
			{
				Log::vsyslog( __priority, buffer, len );
			}
		#ifdef USE_LOG2
			LOG::Log(rmtlog, true);
		#endif
		}
	}

	void Log::vlogvital( int __priority, const char * __fmt, va_list ap )
	{
		char buffer[1024];
		int len = vsnprintf( buffer, sizeof(buffer), __fmt, ap );
		len = len>=(int)sizeof(buffer) ? sizeof(buffer)-1 : len ;
		if( len > 0 )
		{
			RemoteLogVital rmtlog( __priority, std::string(buffer,len), g_hostname, g_progname);
			if( LogclientTcpClient::GetInstance()->SendProtocol(rmtlog) )
			{
				if( Thread::Pool::Size() > 1 )
					PollIO::WakeUp();
			}
			else
			{
				Log::vsyslog( __priority,  buffer, len );
			}
		#ifdef USE_LOG2
			LOG::Log(rmtlog, true);
		#endif
		}
	}

	void Log::vstatinfo( int __priority, const char * __fmt, va_list ap )
	{
		char buffer[1024];
		int len = vsnprintf( buffer, sizeof(buffer), __fmt, ap );
		len = len>=(int)sizeof(buffer) ? sizeof(buffer)-1 : len ;
		if( len > 0 )
		{
			StatInfoVital rmtlog( __priority, std::string(buffer,len), g_hostname, g_progname);

			if( LogclientTcpClient::GetInstance()->SendProtocol(rmtlog) )
			{
				if( Thread::Pool::Size() > 1 )
					PollIO::WakeUp();
			}
			else
			{
				Log::vsyslog( __priority, buffer, len );
			}
		#ifdef USE_LOG2
			LOG::Log(rmtlog, true);
		#endif
		}
	}

}


