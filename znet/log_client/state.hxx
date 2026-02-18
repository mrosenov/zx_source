#ifndef __GNET_LOG_CLIENT_STATE
#define __GNET_LOG_CLIENT_STATE

#ifdef WIN32
#include "gnproto.h"
#else
#include "protocol.h"
#endif

namespace GNET
{

extern GNET::Protocol::Manager::Session::State state_LoggerNull;

};

#endif
