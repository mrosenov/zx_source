#ifndef __GNET_GGATED_STATE
#define __GNET_GGATED_STATE

#ifdef WIN32
#include "gnproto.h"
#else
#include "protocol.h"
#endif

namespace GNET
{

extern GNET::Protocol::Manager::Session::State state_GameGateClient;

extern GNET::Protocol::Manager::Session::State state_GameDBGateClient;

extern GNET::Protocol::Manager::Session::State state_GTPlatformClient;

extern GNET::Protocol::Manager::Session::State state_KdsClient;

extern GNET::Protocol::Manager::Session::State state_GProviderGateServer;

extern GNET::Protocol::Manager::Session::State state_SNSPlatformClient;

};

#endif
