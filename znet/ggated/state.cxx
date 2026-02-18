#include "callid.hxx"

#ifdef WIN32
#include <winsock2.h>
#include "gnproto.h"
#include "gncompress.h"
#else
#include "protocol.h"
#include "binder.h"
#endif

namespace GNET
{

static GNET::Protocol::Type _state_GameGateClient[] = 
{
	PROTOCOL_GETKDCTOKENTOGATE,
	PROTOCOL_GATEROLELOGIN,
	PROTOCOL_GATEROLELOGOUT,
	PROTOCOL_GATEROLECREATE,
	PROTOCOL_GATEONLINELIST,
	PROTOCOL_GATEGETGTROLESTATUS,
	PROTOCOL_ROLEGROUPUPDATE,
	PROTOCOL_ROLEFRIENDUPDATE,
	PROTOCOL_FACTIONMEMBERUPDATE,
	PROTOCOL_FACTIONINFOUPDATE,
	PROTOCOL_REMOVEROLE,
	PROTOCOL_REMOVEFACTION,
	PROTOCOL_PRIVATECHAT,
	PROTOCOL_GATEFACTIONCHAT,
	RPC_GATEGETROLERELATION,
	PROTOCOL_SYNCROLEEVENT2SNS,
};

GNET::Protocol::Manager::Session::State state_GameGateClient(_state_GameGateClient,
						sizeof(_state_GameGateClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GameDBGateClient[] = 
{
	RPC_DBSNSGETROLE,
	RPC_DBGATEGETUSER,
	RPC_DBGATEGETROLE,
	RPC_DBGATEROLERELATION,
	RPC_DBGATEACTIVATE,
	RPC_DBGATEDEACTIVATE,
	RPC_DBFACTIONGET,
	RPC_DBFAMILYGET,
};

GNET::Protocol::Manager::Session::State state_GameDBGateClient(_state_GameDBGateClient,
						sizeof(_state_GameDBGateClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GTPlatformClient[] = 
{
	PROTOCOL_GAMEDATAREQ,
	PROTOCOL_IMKEEPALIVE,
	PROTOCOL_ANNOUNCERESP,
	PROTOCOL_ROLELISTREQ,
	PROTOCOL_ROLERELATIONREQ,
	PROTOCOL_ROLESTATUSREQ,
	PROTOCOL_ROLESTATUSRESP,
	PROTOCOL_ROLESTATUSUPDATE,
	PROTOCOL_ROLEMSG,
	PROTOCOL_ROLEOFFLINEMESSAGES,
	PROTOCOL_ROLEACTIVATION,
	PROTOCOL_ROLEINFOREQ,
	PROTOCOL_FACTIONINFOREQ,
	PROTOCOL_FACTIONMSG,
	PROTOCOL_ROLEENTERVOICECHANNEL,
	PROTOCOL_ROLELEAVEVOICECHANNEL,
};

GNET::Protocol::Manager::Session::State state_GTPlatformClient(_state_GTPlatformClient,
						sizeof(_state_GTPlatformClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_KdsClient[] = 
{
	PROTOCOL_KEYSRESP,
	PROTOCOL_KDSKEEPALIVE,
};

GNET::Protocol::Manager::Session::State state_KdsClient(_state_KdsClient,
						sizeof(_state_KdsClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GProviderGateServer[] = 
{
	PROTOCOL_KEEPALIVE,
	PROTOCOL_ANNOUNCEPROVIDERID,
	PROTOCOL_GTSYNCTEAMS,
	PROTOCOL_GTTEAMCREATE,
	PROTOCOL_GTTEAMDISMISS,
	PROTOCOL_GTTEAMMEMBERUPDATE,
	PROTOCOL_SYNCGSROLEINFO2PLATFORM,
	PROTOCOL_SYNCROLEEVENT2SNS,
};

GNET::Protocol::Manager::Session::State state_GProviderGateServer(_state_GProviderGateServer,
						sizeof(_state_GProviderGateServer)/sizeof(GNET::Protocol::Type), 120);

static GNET::Protocol::Type _state_SNSPlatformClient[] = 
{
	PROTOCOL_IMKEEPALIVE,
	PROTOCOL_ROLELISTREQ,
	PROTOCOL_ROLERELATIONREQ,
	PROTOCOL_FACTIONINFOREQ,
};

GNET::Protocol::Manager::Session::State state_SNSPlatformClient(_state_SNSPlatformClient,
						sizeof(_state_SNSPlatformClient)/sizeof(GNET::Protocol::Type), 86400);


};

