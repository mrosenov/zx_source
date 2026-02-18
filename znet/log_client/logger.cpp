#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log2tcpclient.hpp"
#include "log2udpclient.hpp"
#include "octets.h"
#include "thread.h"
#include "marshal.h"
#include "pollio.h"
#include "logger.hpp"

namespace LOG
{
	void Log_Init_Udp()
	{
		GNET::Conf *conf = GNET::Conf::GetInstance();
                GNET::Log2UdpClient *manager = GNET::Log2UdpClient::GetInstance();
		std::string findstr = conf->find(manager->Identification(), "accumulate");
		if( findstr.empty() )
		{
			std::cerr << "logclient init error" << std::endl;
			return;
		}
                manager->SetAccumulate(atoi(findstr.c_str()));
                GNET::Protocol::Client(manager);
	}
	void Log_Init_Tcp()
	{
		GNET::Conf *conf = GNET::Conf::GetInstance();
        	GNET::Log2TcpClient *manager = GNET::Log2TcpClient::GetInstance();
		std::string findstr = conf->find(manager->Identification(), "accumulate");
		if( findstr.empty() )
		{
			std::cerr << "logclient init error" << std::endl;
			return;
		}
                manager->SetAccumulate(atoi(findstr.c_str()));
                GNET::Protocol::Client(manager);
	}

	void Log_Send_Tcp(const GNET::Protocol *protocol) { GNET::Log2TcpClient::GetInstance()->SendProtocol(protocol); }

	void Log_Send_Tcp(const GNET::Protocol &protocol) { GNET::Log2TcpClient::GetInstance()->SendProtocol(protocol); }

	void Log_Send_Udp(const GNET::Protocol *protocol) { GNET::Log2UdpClient::GetInstance()->SendProtocol(protocol); }

	void Log_Send_Udp(const GNET::Protocol &protocol) { GNET::Log2UdpClient::GetInstance()->SendProtocol(protocol); }

        /*Log_Init≥ı ºªØ*/
	void Log_Init() 
	{ 
		Log_Init_Udp(); Log_Init_Tcp(); 
	}

	void Log(const GNET::Protocol *protocol, bool reliability)
	{
		if(reliability)
			Log_Send_Tcp(protocol);
		else
			Log_Send_Udp(protocol);
	}

	void Log(const GNET::Protocol &protocol, bool reliability)
	{
		if(reliability)
			Log_Send_Tcp(protocol);
		else
			Log_Send_Udp(protocol);
	}
};//end namespace
