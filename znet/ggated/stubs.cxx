#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "gategetrolerelation.hrp"
#include "dbsnsgetrole.hrp"
#include "dbgategetuser.hrp"
#include "dbgategetrole.hrp"
#include "dbgaterolerelation.hrp"
#include "dbgateactivate.hrp"
#include "dbgatedeactivate.hrp"
#include "dbfactionget.hrp"
#include "dbfamilyget.hrp"
#include "keysreq.hpp"
#include "gateonlinelist_re.hpp"
#include "gategetgtrolestatus_re.hpp"
#include "gateupdatestatus.hpp"
#include "gateofflinechat.hpp"
#include "getkdctoken_re.hpp"
#include "announcezoneidtoim.hpp"
#include "gamedataresp.hpp"
#include "rolelistresp.hpp"
#include "rolerelationresp.hpp"
#include "factioninforesp.hpp"
#include "roleinforesp.hpp"
#include "roleinfoupdate.hpp"
#include "gtreconnect.hpp"
#include "roleentervoicechannelack.hpp"
#include "roleleavevoicechannelack.hpp"
#include "getkdctokentogate.hpp"
#include "gaterolelogin.hpp"
#include "gaterolelogout.hpp"
#include "gaterolecreate.hpp"
#include "gateonlinelist.hpp"
#include "gategetgtrolestatus.hpp"
#include "rolegroupupdate.hpp"
#include "rolefriendupdate.hpp"
#include "factionmemberupdate.hpp"
#include "factioninfoupdate.hpp"
#include "removerole.hpp"
#include "removefaction.hpp"
#include "privatechat.hpp"
#include "gatefactionchat.hpp"
#include "syncroleevent2sns.hpp"
#include "gamedatareq.hpp"
#include "imkeepalive.hpp"
#include "announceresp.hpp"
#include "rolelistreq.hpp"
#include "rolerelationreq.hpp"
#include "rolestatusreq.hpp"
#include "rolestatusresp.hpp"
#include "rolestatusupdate.hpp"
#include "rolemsg.hpp"
#include "roleofflinemessages.hpp"
#include "roleactivation.hpp"
#include "roleinforeq.hpp"
#include "factioninforeq.hpp"
#include "factionmsg.hpp"
#include "roleentervoicechannel.hpp"
#include "roleleavevoicechannel.hpp"
#include "keysresp.hpp"
#include "kdskeepalive.hpp"
#include "keepalive.hpp"
#include "announceproviderid.hpp"
#include "gtsyncteams.hpp"
#include "gtteamcreate.hpp"
#include "gtteamdismiss.hpp"
#include "gtteammemberupdate.hpp"
#include "syncgsroleinfo2platform.hpp"

namespace GNET
{

static GateGetRoleRelation __stub_GateGetRoleRelation (RPC_GATEGETROLERELATION, new RoleId, new DBGateRoleRelationRes);
static DBSNSGetRole __stub_DBSNSGetRole (RPC_DBSNSGETROLE, new DBSNSGetRoleArg, new DBSNSGetRoleRes);
static DBGateGetUser __stub_DBGateGetUser (RPC_DBGATEGETUSER, new UserID, new DBGateGetUserRes);
static DBGateGetRole __stub_DBGateGetRole (RPC_DBGATEGETROLE, new RoleId, new DBGateGetRoleRes);
static DBGateRoleRelation __stub_DBGateRoleRelation (RPC_DBGATEROLERELATION, new RoleId, new DBGateRoleRelationRes);
static DBGateActivate __stub_DBGateActivate (RPC_DBGATEACTIVATE, new DBGateActivateArg, new DBGateActivateRes);
static DBGateDeactivate __stub_DBGateDeactivate (RPC_DBGATEDEACTIVATE, new DBGateActivateArg, new DBGateActivateRes);
static DBFactionGet __stub_DBFactionGet (RPC_DBFACTIONGET, new FactionId, new FactionInfoRes);
static DBFamilyGet __stub_DBFamilyGet (RPC_DBFAMILYGET, new FamilyId, new FamilyGetRes);
static KeysReq __stub_KeysReq((void*)0);
static GateOnlineList_Re __stub_GateOnlineList_Re((void*)0);
static GateGetGTRoleStatus_Re __stub_GateGetGTRoleStatus_Re((void*)0);
static GateUpdateStatus __stub_GateUpdateStatus((void*)0);
static GateOfflineChat __stub_GateOfflineChat((void*)0);
static GetKDCToken_Re __stub_GetKDCToken_Re((void*)0);
static AnnounceZoneidToIM __stub_AnnounceZoneidToIM((void*)0);
static GameDataResp __stub_GameDataResp((void*)0);
static RoleListResp __stub_RoleListResp((void*)0);
static RoleRelationResp __stub_RoleRelationResp((void*)0);
static FactionInfoResp __stub_FactionInfoResp((void*)0);
static RoleInfoResp __stub_RoleInfoResp((void*)0);
static RoleInfoUpdate __stub_RoleInfoUpdate((void*)0);
static GTReconnect __stub_GTReconnect((void*)0);
static RoleEnterVoiceChannelAck __stub_RoleEnterVoiceChannelAck((void*)0);
static RoleLeaveVoiceChannelAck __stub_RoleLeaveVoiceChannelAck((void*)0);
static GetKDCTokenToGate __stub_GetKDCTokenToGate((void*)0);
static GateRoleLogin __stub_GateRoleLogin((void*)0);
static GateRoleLogout __stub_GateRoleLogout((void*)0);
static GateRoleCreate __stub_GateRoleCreate((void*)0);
static GateOnlineList __stub_GateOnlineList((void*)0);
static GateGetGTRoleStatus __stub_GateGetGTRoleStatus((void*)0);
static RoleGroupUpdate __stub_RoleGroupUpdate((void*)0);
static RoleFriendUpdate __stub_RoleFriendUpdate((void*)0);
static FactionMemberUpdate __stub_FactionMemberUpdate((void*)0);
static FactionInfoUpdate __stub_FactionInfoUpdate((void*)0);
static RemoveRole __stub_RemoveRole((void*)0);
static RemoveFaction __stub_RemoveFaction((void*)0);
static PrivateChat __stub_PrivateChat((void*)0);
static GateFactionChat __stub_GateFactionChat((void*)0);
static SyncRoleEvent2SNS __stub_SyncRoleEvent2SNS((void*)0);
static GameDataReq __stub_GameDataReq((void*)0);
static IMKeepAlive __stub_IMKeepAlive((void*)0);
static AnnounceResp __stub_AnnounceResp((void*)0);
static RoleListReq __stub_RoleListReq((void*)0);
static RoleRelationReq __stub_RoleRelationReq((void*)0);
static RoleStatusReq __stub_RoleStatusReq((void*)0);
static RoleStatusResp __stub_RoleStatusResp((void*)0);
static RoleStatusUpdate __stub_RoleStatusUpdate((void*)0);
static RoleMsg __stub_RoleMsg((void*)0);
static RoleOfflineMessages __stub_RoleOfflineMessages((void*)0);
static RoleActivation __stub_RoleActivation((void*)0);
static RoleInfoReq __stub_RoleInfoReq((void*)0);
static FactionInfoReq __stub_FactionInfoReq((void*)0);
static FactionMsg __stub_FactionMsg((void*)0);
static RoleEnterVoiceChannel __stub_RoleEnterVoiceChannel((void*)0);
static RoleLeaveVoiceChannel __stub_RoleLeaveVoiceChannel((void*)0);
static KeysResp __stub_KeysResp((void*)0);
static KDSKeepAlive __stub_KDSKeepAlive((void*)0);
static KeepAlive __stub_KeepAlive((void*)0);
static AnnounceProviderID __stub_AnnounceProviderID((void*)0);
static GTSyncTeams __stub_GTSyncTeams((void*)0);
static GTTeamCreate __stub_GTTeamCreate((void*)0);
static GTTeamDismiss __stub_GTTeamDismiss((void*)0);
static GTTeamMemberUpdate __stub_GTTeamMemberUpdate((void*)0);
static SyncGsRoleInfo2Platform __stub_SyncGsRoleInfo2Platform((void*)0);

};
