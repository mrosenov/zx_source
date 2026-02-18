#ifndef __LOGGER_HPP_
#define __LOGGER_HPP_
#include "protocol.h"
namespace LOG
{

	void Log_Init();
        void Log(const GNET::Protocol *protocol, bool reliability = true);
        void Log(const GNET::Protocol &protocol, bool reliability = true);
};

#endif
