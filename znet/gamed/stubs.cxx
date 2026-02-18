#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "putspouse.hrp"
#include "commondatachange.hrp"
#include "certchallenge.hrp"
#include "flowergifttake.hrp"
#include "flowertakeoff.hrp"
#include "circlegetasyncdata.hrp"
#include "getweeklytop.hrp"
#include "getdailytop.hrp"
#include "taskasyncdata.hrp"
#include "gterritoryitemget.hrp"
#include "guniquebid.hrp"
#include "guniquegetitem.hrp"
#include "getfriendnumber.hrp"
#include "kingguardinvite.hrp"
#include "getcashavail.hrp"
#include "syncplayerfaccouponadd.hpp"
#include "startfactionmultiexp.hpp"
#include "facbasedatabroadcast.hpp"
#include "syncgsroleinfo2platform.hpp"
#include "syncroleevent2sns.hpp"
#include "facdyndonatecash.hpp"
#include "facdynputauction.hpp"
#include "facdynwinauction.hpp"
#include "facdynbuyauction.hpp"
#include "facdynbuildingupgrade.hpp"
#include "facdynbuildingcomplete.hpp"
#include "gfacbaseenter.hpp"
#include "gfacbaseleave.hpp"
#include "startfacbase_re.hpp"
#include "stopfacbase_re.hpp"
#include "gupdatefacact.hpp"
#include "gfacbaseserverregister.hpp"
#include "globaldropbroadcast.hpp"
#include "registerglobalcounter.hpp"
#include "graidserverregister.hpp"
#include "graidopen.hpp"
#include "graidjoin.hpp"
#include "graidmemberchange.hpp"
#include "graidleave.hpp"
#include "graidentersuccess.hpp"
#include "graidend.hpp"
#include "sendraidstart_re.hpp"
#include "sendraidenter_re.hpp"
#include "gcrssvrteamsgetscore.hpp"
#include "graidteamapply.hpp"
#include "ghideseekraidapply.hpp"
#include "gfengshenraidapply.hpp"
#include "gfactionpkraidkill.hpp"
#include "gfactionpkraiddeposit.hpp"
#include "gfactionpkraidfightnotify.hpp"
#include "gcirclechat.hpp"
#include "addcirclepoint.hpp"
#include "playerkickout_re.hpp"
#include "playerlogin_re.hpp"
#include "playeroffline_re.hpp"
#include "playerlogout.hpp"
#include "playerreconnect_re.hpp"
#include "queryplayerstatus.hpp"
#include "gettaskdata.hpp"
#include "settaskdata.hpp"
#include "s2cgamedatasend.hpp"
#include "s2cmulticast.hpp"
#include "s2cmulticast2.hpp"
#include "s2cbroadcast.hpp"
#include "setchatemotion.hpp"
#include "battleflagstart.hpp"
#include "querybattleflagbuff.hpp"
#include "getcouponsrep.hpp"
#include "chatmulticast.hpp"
#include "chatbroadcast.hpp"
#include "chatsinglecast.hpp"
#include "keepalive.hpp"
#include "playerheartbeat.hpp"
#include "disconnectplayer.hpp"
#include "gtradestart_re.hpp"
#include "gtradediscard.hpp"
#include "battleenter.hpp"
#include "battleserverregister.hpp"
#include "battleenterfail.hpp"
#include "gbattleend.hpp"
#include "gbattleleave.hpp"
#include "battlestart_re.hpp"
#include "gopenbanquetleave.hpp"
#include "gopenbanquetjoin.hpp"
#include "instancingregister.hpp"
#include "instancingenterfail.hpp"
#include "ginstancingend.hpp"
#include "sendinstancingstart_re.hpp"
#include "ginstancingleave.hpp"
#include "sendinstancingjoin.hpp"
#include "gmrestartserver_re.hpp"
#include "getmaillist.hpp"
#include "getmail.hpp"
#include "getmailattachobj.hpp"
#include "deletemail.hpp"
#include "preservemail.hpp"
#include "playersendmail.hpp"
#include "auctionopen.hpp"
#include "auctionbid.hpp"
#include "sendauctionbid.hpp"
#include "auctionlist.hpp"
#include "auctionclose.hpp"
#include "auctionget.hpp"
#include "auctiongetitem.hpp"
#include "auctionattendlist.hpp"
#include "auctionexitbid.hpp"
#include "queryrewardtype.hpp"
#include "queryrewardtype_re.hpp"
#include "querynetbarreward.hpp"
#include "actriggerquestion.hpp"
#include "sendcrssvrteamscreate.hpp"
#include "querygameserverattr.hpp"
#include "sendfactioncreate.hpp"
#include "factioncreate.hpp"
#include "factiondismiss.hpp"
#include "factionupgrade.hpp"
#include "playerfactioninfo.hpp"
#include "syncplayerfaction.hpp"
#include "debugcommand.hpp"
#include "syncroledata.hpp"
#include "updateenemy.hpp"
#include "stockcommission.hpp"
#include "stockaccount.hpp"
#include "stocktransaction.hpp"
#include "stockbill.hpp"
#include "stockcancel.hpp"
#include "combatkill.hpp"
#include "playerchangegs_re.hpp"
#include "familyuseskill.hpp"
#include "familyexpshare.hpp"
#include "ghostileprotect.hpp"
#include "hostileprotect.hpp"
#include "commondatasync.hpp"
#include "gsiegeserverregister.hpp"
#include "gsiegeend.hpp"
#include "sendsiegestart_re.hpp"
#include "sendsiegechallenge.hpp"
#include "siegesetassistant.hpp"
#include "siegechallenge.hpp"
#include "siegekill.hpp"
#include "siegebroadcat.hpp"
#include "sectupdate.hpp"
#include "sendsnspressmessage.hpp"
#include "sendsnsapply.hpp"
#include "sendsnsvote.hpp"
#include "sendrefcashused.hpp"
#include "sendreflevelup.hpp"
#include "sendchangeprofile.hpp"
#include "achievementmessage.hpp"
#include "pkmessage.hpp"
#include "refinemessage.hpp"
#include "ginstancingstatus.hpp"
#include "sendhometownexchgmoney.hpp"
#include "gterritorychallenge.hpp"
#include "territoryserverregister.hpp"
#include "sendterritorystart_re.hpp"
#include "gterritoryend.hpp"
#include "gterritoryenter.hpp"
#include "gterritoryleave.hpp"
#include "trychangeds.hpp"
#include "playerchangeds_re.hpp"
#include "gconsignstart.hpp"
#include "gconsignstartrole.hpp"
#include "playerconsignoffline_re.hpp"
#include "gchangerolename.hpp"
#include "gchangefactionname.hpp"
#include "gtsyncteams.hpp"
#include "gtteamcreate.hpp"
#include "gtteamdismiss.hpp"
#include "gtteammemberupdate.hpp"
#include "gkingdombattleregister.hpp"
#include "kingdombattlestart_re.hpp"
#include "gkingdombattleend.hpp"
#include "gkingdombattlehalf.hpp"
#include "gkingdomattackerfail.hpp"
#include "gkingdombattleenter.hpp"
#include "gkingdombattleleave.hpp"
#include "gkingdompointchange.hpp"
#include "gkingtrycallguards.hpp"
#include "gkingissuetask.hpp"
#include "gkinggetreward.hpp"
#include "s2clinebroadcast.hpp"
#include "gtouchpointexchange.hpp"
#include "gopenbanquetroledel.hpp"
#include "gopenbanquetclose.hpp"
#include "factionmultiexpsync.hpp"
#include "facbasedatasend.hpp"
#include "facbasedatadeliver.hpp"
#include "facmallchange.hpp"
#include "startfacbase.hpp"
#include "stopfacbase.hpp"
#include "facmallsync.hpp"
#include "facbasepropchange.hpp"
#include "facbasebuildingprogress.hpp"
#include "facauctionput.hpp"
#include "gcrssvrteamspostscore.hpp"
#include "sendraidstart.hpp"
#include "sendraidenter.hpp"
#include "sendraidclose.hpp"
#include "sendraidkickout.hpp"
#include "open_controler.hpp"
#include "addcash_re.hpp"
#include "bonusexp.hpp"
#include "updaterolecircle.hpp"
#include "bonusitem.hpp"
#include "announceproviderid.hpp"
#include "playerkickout.hpp"
#include "playerlogin.hpp"
#include "playeroffline.hpp"
#include "playerreconnect.hpp"
#include "playerstatussync.hpp"
#include "playerstatusannounce.hpp"
#include "battleflagbuffscope.hpp"
#include "getcouponsreq.hpp"
#include "gettaskdata_re.hpp"
#include "settaskdata_re.hpp"
#include "enterworld.hpp"
#include "c2sgamedatasend.hpp"
#include "publicchat.hpp"
#include "privatechat.hpp"
#include "factionchat.hpp"
#include "gtradestart.hpp"
#include "gtradeend.hpp"
#include "announcegm.hpp"
#include "gmrestartserver.hpp"
#include "gmshutdownline.hpp"
#include "gmailendsync.hpp"
#include "querygameserverattr_re.hpp"
#include "acreportcheater.hpp"
#include "battlestart.hpp"
#include "sendbattleenter.hpp"
#include "sendinstancingstart.hpp"
#include "sendinstancingenter.hpp"
#include "gbattlejoin.hpp"
#include "playerfactioninfo_re.hpp"
#include "syncfactionlevel.hpp"
#include "syncfamilydata.hpp"
#include "syncfactionhostiles.hpp"
#include "netbarreward.hpp"
#include "notifydailytablechange.hpp"
#include "ondivorce.hpp"
#include "combatcontrol.hpp"
#include "playerchangegs.hpp"
#include "sendasyncdata.hpp"
#include "addictioncontrol.hpp"
#include "vipinfonotify.hpp"
#include "hostileprotect_re.hpp"
#include "hostiledelete_re.hpp"
#include "hostileadd_re.hpp"
#include "sendsiegestart.hpp"
#include "gsiegestatus.hpp"
#include "sectquit.hpp"
#include "sectrecruit_re.hpp"
#include "sectexpel_re.hpp"
#include "contestresult.hpp"
#include "sendsnsresult.hpp"
#include "billingrequest2.hpp"
#include "billingbalance.hpp"
#include "sendrefaddexp.hpp"
#include "sendrefaddbonus.hpp"
#include "sendchangeprofile_re.hpp"
#include "gshopsetsalescheme.hpp"
#include "gshopsetdiscountscheme.hpp"
#include "sendterritorystart.hpp"
#include "syncterritorylist.hpp"
#include "playerchangeds.hpp"
#include "gconsignend.hpp"
#include "playerconsignoffline.hpp"
#include "gconsignendrole.hpp"
#include "gchangerolename_re.hpp"
#include "gchangefactionname_re.hpp"
#include "gtreconnect.hpp"
#include "roleentervoicechannel.hpp"
#include "roleleavevoicechannel.hpp"
#include "kingdombattlestart.hpp"
#include "kingdombattlestop.hpp"
#include "kingdominfosync.hpp"
#include "kingdomtitlesync.hpp"
#include "kingdompointsync.hpp"
#include "kingcallguard.hpp"
#include "queenopenbath.hpp"
#include "queenclosebath.hpp"
#include "syncbathtimes.hpp"
#include "gkingissuetask_re.hpp"
#include "gkinggetreward_re.hpp"
#include "isconnalive.hpp"
#include "gtouchpointexchange_re.hpp"
#include "forbidservice.hpp"
#include "webordernotice.hpp"

namespace GNET
{

static PutSpouse __stub_PutSpouse (RPC_PUTSPOUSE, new PutSpouseArg, new RpcRetcode);
static CommonDataChange __stub_CommonDataChange (RPC_COMMONDATACHANGE, new CommonDataArg, new CommonDataRes);
static CertChallenge __stub_CertChallenge (RPC_CERTCHALLENGE, new CertChallengeArg, new CertChallengeRes);
static FlowerGiftTake __stub_FlowerGiftTake (RPC_FLOWERGIFTTAKE, new FlowerGiftTakeArg, new FlowerGiftTakeRes);
static FlowerTakeOff __stub_FlowerTakeOff (RPC_FLOWERTAKEOFF, new FlowerTakeOffArg, new FlowerTakeOffRes);
static CircleGetAsyncData __stub_CircleGetAsyncData (RPC_CIRCLEGETASYNCDATA, new CircleAsyncData, new CircleAsyncData);
static GetWeeklyTop __stub_GetWeeklyTop (RPC_GETWEEKLYTOP, new Integer, new WeeklyTopRes);
static GetDailyTop __stub_GetDailyTop (RPC_GETDAILYTOP, new Integer, new DailyTopRes);
static TaskAsyncData __stub_TaskAsyncData (RPC_TASKASYNCDATA, new AsyncData, new AsyncData);
static GTerritoryItemGet __stub_GTerritoryItemGet (RPC_GTERRITORYITEMGET, new GTerritoryItemGetArg, new GTerritoryItemGetRes);
static GUniqueBid __stub_GUniqueBid (RPC_GUNIQUEBID, new GUniqueBidArg, new GUniqueBidRes);
static GUniqueGetItem __stub_GUniqueGetItem (RPC_GUNIQUEGETITEM, new GUniqueGetItemArg, new GUniqueGetItemRes);
static GetFriendNumber __stub_GetFriendNumber (RPC_GETFRIENDNUMBER, new GetFriendNumberArg, new GetFriendNumberRes);
static KingGuardInvite __stub_KingGuardInvite (RPC_KINGGUARDINVITE, new KingGuardInviteArg, new KingGuardInviteRes);
static GetCashAvail __stub_GetCashAvail (RPC_GETCASHAVAIL, new GetCashAvailArg, new GetCashAvailRes);
static SyncPlayerFacCouponAdd __stub_SyncPlayerFacCouponAdd((void*)0);
static StartFactionMultiExp __stub_StartFactionMultiExp((void*)0);
static FacBaseDataBroadcast __stub_FacBaseDataBroadcast((void*)0);
static SyncGsRoleInfo2Platform __stub_SyncGsRoleInfo2Platform((void*)0);
static SyncRoleEvent2SNS __stub_SyncRoleEvent2SNS((void*)0);
static FacDynDonateCash __stub_FacDynDonateCash((void*)0);
static FacDynPutAuction __stub_FacDynPutAuction((void*)0);
static FacDynWinAuction __stub_FacDynWinAuction((void*)0);
static FacDynBuyAuction __stub_FacDynBuyAuction((void*)0);
static FacDynBuildingUpgrade __stub_FacDynBuildingUpgrade((void*)0);
static FacDynBuildingComplete __stub_FacDynBuildingComplete((void*)0);
static GFacBaseEnter __stub_GFacBaseEnter((void*)0);
static GFacBaseLeave __stub_GFacBaseLeave((void*)0);
static StartFacBase_Re __stub_StartFacBase_Re((void*)0);
static StopFacBase_Re __stub_StopFacBase_Re((void*)0);
static GUpdateFacAct __stub_GUpdateFacAct((void*)0);
static GFacBaseServerRegister __stub_GFacBaseServerRegister((void*)0);
static GlobalDropBroadCast __stub_GlobalDropBroadCast((void*)0);
static RegisterGlobalCounter __stub_RegisterGlobalCounter((void*)0);
static GRaidServerRegister __stub_GRaidServerRegister((void*)0);
static GRaidOpen __stub_GRaidOpen((void*)0);
static GRaidJoin __stub_GRaidJoin((void*)0);
static GRaidMemberChange __stub_GRaidMemberChange((void*)0);
static GRaidLeave __stub_GRaidLeave((void*)0);
static GRaidEnterSuccess __stub_GRaidEnterSuccess((void*)0);
static GRaidEnd __stub_GRaidEnd((void*)0);
static SendRaidStart_Re __stub_SendRaidStart_Re((void*)0);
static SendRaidEnter_Re __stub_SendRaidEnter_Re((void*)0);
static GCrssvrTeamsGetScore __stub_GCrssvrTeamsGetScore((void*)0);
static GRaidTeamApply __stub_GRaidTeamApply((void*)0);
static GHideSeekRaidApply __stub_GHideSeekRaidApply((void*)0);
static GFengShenRaidApply __stub_GFengShenRaidApply((void*)0);
static GFactionPkRaidKill __stub_GFactionPkRaidKill((void*)0);
static GFactionPkRaidDeposit __stub_GFactionPkRaidDeposit((void*)0);
static GFactionPkRaidFightNotify __stub_GFactionPkRaidFightNotify((void*)0);
static GCircleChat __stub_GCircleChat((void*)0);
static AddCirclePoint __stub_AddCirclePoint((void*)0);
static PlayerKickout_Re __stub_PlayerKickout_Re((void*)0);
static PlayerLogin_Re __stub_PlayerLogin_Re((void*)0);
static PlayerOffline_Re __stub_PlayerOffline_Re((void*)0);
static PlayerLogout __stub_PlayerLogout((void*)0);
static PlayerReconnect_Re __stub_PlayerReconnect_Re((void*)0);
static QueryPlayerStatus __stub_QueryPlayerStatus((void*)0);
static GetTaskData __stub_GetTaskData((void*)0);
static SetTaskData __stub_SetTaskData((void*)0);
static S2CGamedataSend __stub_S2CGamedataSend((void*)0);
static S2CMulticast __stub_S2CMulticast((void*)0);
static S2CMulticast2 __stub_S2CMulticast2((void*)0);
static S2CBroadcast __stub_S2CBroadcast((void*)0);
static SetChatEmotion __stub_SetChatEmotion((void*)0);
static BattleFlagStart __stub_BattleFlagStart((void*)0);
static QueryBattleFlagBuff __stub_QueryBattleFlagBuff((void*)0);
static GetCouponsRep __stub_GetCouponsRep((void*)0);
static ChatMultiCast __stub_ChatMultiCast((void*)0);
static ChatBroadCast __stub_ChatBroadCast((void*)0);
static ChatSingleCast __stub_ChatSingleCast((void*)0);
static KeepAlive __stub_KeepAlive((void*)0);
static PlayerHeartBeat __stub_PlayerHeartBeat((void*)0);
static DisconnectPlayer __stub_DisconnectPlayer((void*)0);
static GTradeStart_Re __stub_GTradeStart_Re((void*)0);
static GTradeDiscard __stub_GTradeDiscard((void*)0);
static BattleEnter __stub_BattleEnter((void*)0);
static BattleServerRegister __stub_BattleServerRegister((void*)0);
static BattleEnterFail __stub_BattleEnterFail((void*)0);
static GBattleEnd __stub_GBattleEnd((void*)0);
static GBattleLeave __stub_GBattleLeave((void*)0);
static BattleStart_Re __stub_BattleStart_Re((void*)0);
static GOpenBanquetLeave __stub_GOpenBanquetLeave((void*)0);
static GOpenBanquetJoin __stub_GOpenBanquetJoin((void*)0);
static InstancingRegister __stub_InstancingRegister((void*)0);
static InstancingEnterFail __stub_InstancingEnterFail((void*)0);
static GInstancingEnd __stub_GInstancingEnd((void*)0);
static SendInstancingStart_Re __stub_SendInstancingStart_Re((void*)0);
static GInstancingLeave __stub_GInstancingLeave((void*)0);
static SendInstancingJoin __stub_SendInstancingJoin((void*)0);
static GMRestartServer_Re __stub_GMRestartServer_Re((void*)0);
static GetMailList __stub_GetMailList((void*)0);
static GetMail __stub_GetMail((void*)0);
static GetMailAttachObj __stub_GetMailAttachObj((void*)0);
static DeleteMail __stub_DeleteMail((void*)0);
static PreserveMail __stub_PreserveMail((void*)0);
static PlayerSendMail __stub_PlayerSendMail((void*)0);
static AuctionOpen __stub_AuctionOpen((void*)0);
static AuctionBid __stub_AuctionBid((void*)0);
static SendAuctionBid __stub_SendAuctionBid((void*)0);
static AuctionList __stub_AuctionList((void*)0);
static AuctionClose __stub_AuctionClose((void*)0);
static AuctionGet __stub_AuctionGet((void*)0);
static AuctionGetItem __stub_AuctionGetItem((void*)0);
static AuctionAttendList __stub_AuctionAttendList((void*)0);
static AuctionExitBid __stub_AuctionExitBid((void*)0);
static QueryRewardType __stub_QueryRewardType((void*)0);
static QueryRewardType_Re __stub_QueryRewardType_Re((void*)0);
static QueryNetBarReward __stub_QueryNetBarReward((void*)0);
static ACTriggerQuestion __stub_ACTriggerQuestion((void*)0);
static SendCrssvrTeamsCreate __stub_SendCrssvrTeamsCreate((void*)0);
static QueryGameServerAttr __stub_QueryGameServerAttr((void*)0);
static SendFactionCreate __stub_SendFactionCreate((void*)0);
static FactionCreate __stub_FactionCreate((void*)0);
static FactionDismiss __stub_FactionDismiss((void*)0);
static FactionUpgrade __stub_FactionUpgrade((void*)0);
static PlayerFactionInfo __stub_PlayerFactionInfo((void*)0);
static SyncPlayerFaction __stub_SyncPlayerFaction((void*)0);
static DebugCommand __stub_DebugCommand((void*)0);
static SyncRoleData __stub_SyncRoleData((void*)0);
static UpdateEnemy __stub_UpdateEnemy((void*)0);
static StockCommission __stub_StockCommission((void*)0);
static StockAccount __stub_StockAccount((void*)0);
static StockTransaction __stub_StockTransaction((void*)0);
static StockBill __stub_StockBill((void*)0);
static StockCancel __stub_StockCancel((void*)0);
static CombatKill __stub_CombatKill((void*)0);
static PlayerChangeGS_Re __stub_PlayerChangeGS_Re((void*)0);
static FamilyUseSkill __stub_FamilyUseSkill((void*)0);
static FamilyExpShare __stub_FamilyExpShare((void*)0);
static GHostileProtect __stub_GHostileProtect((void*)0);
static HostileProtect __stub_HostileProtect((void*)0);
static CommonDataSync __stub_CommonDataSync((void*)0);
static GSiegeServerRegister __stub_GSiegeServerRegister((void*)0);
static GSiegeEnd __stub_GSiegeEnd((void*)0);
static SendSiegeStart_Re __stub_SendSiegeStart_Re((void*)0);
static SendSiegeChallenge __stub_SendSiegeChallenge((void*)0);
static SiegeSetAssistant __stub_SiegeSetAssistant((void*)0);
static SiegeChallenge __stub_SiegeChallenge((void*)0);
static SiegeKill __stub_SiegeKill((void*)0);
static SiegeBroadcat __stub_SiegeBroadcat((void*)0);
static SectUpdate __stub_SectUpdate((void*)0);
static SendSNSPressMessage __stub_SendSNSPressMessage((void*)0);
static SendSNSApply __stub_SendSNSApply((void*)0);
static SendSNSVote __stub_SendSNSVote((void*)0);
static SendRefCashUsed __stub_SendRefCashUsed((void*)0);
static SendRefLevelUp __stub_SendRefLevelUp((void*)0);
static SendChangeProfile __stub_SendChangeProfile((void*)0);
static AchievementMessage __stub_AchievementMessage((void*)0);
static PKMessage __stub_PKMessage((void*)0);
static RefineMessage __stub_RefineMessage((void*)0);
static GInstancingStatus __stub_GInstancingStatus((void*)0);
static SendHometownExchgMoney __stub_SendHometownExchgMoney((void*)0);
static GTerritoryChallenge __stub_GTerritoryChallenge((void*)0);
static TerritoryServerRegister __stub_TerritoryServerRegister((void*)0);
static SendTerritoryStart_Re __stub_SendTerritoryStart_Re((void*)0);
static GTerritoryEnd __stub_GTerritoryEnd((void*)0);
static GTerritoryEnter __stub_GTerritoryEnter((void*)0);
static GTerritoryLeave __stub_GTerritoryLeave((void*)0);
static TryChangeDS __stub_TryChangeDS((void*)0);
static PlayerChangeDS_Re __stub_PlayerChangeDS_Re((void*)0);
static GConsignStart __stub_GConsignStart((void*)0);
static GConsignStartRole __stub_GConsignStartRole((void*)0);
static PlayerConsignOffline_Re __stub_PlayerConsignOffline_Re((void*)0);
static GChangeRolename __stub_GChangeRolename((void*)0);
static GChangeFactionName __stub_GChangeFactionName((void*)0);
static GTSyncTeams __stub_GTSyncTeams((void*)0);
static GTTeamCreate __stub_GTTeamCreate((void*)0);
static GTTeamDismiss __stub_GTTeamDismiss((void*)0);
static GTTeamMemberUpdate __stub_GTTeamMemberUpdate((void*)0);
static GKingdomBattleRegister __stub_GKingdomBattleRegister((void*)0);
static KingdomBattleStart_Re __stub_KingdomBattleStart_Re((void*)0);
static GKingdomBattleEnd __stub_GKingdomBattleEnd((void*)0);
static GKingdomBattleHalf __stub_GKingdomBattleHalf((void*)0);
static GKingdomAttackerFail __stub_GKingdomAttackerFail((void*)0);
static GKingdomBattleEnter __stub_GKingdomBattleEnter((void*)0);
static GKingdomBattleLeave __stub_GKingdomBattleLeave((void*)0);
static GKingdomPointChange __stub_GKingdomPointChange((void*)0);
static GKingTryCallGuards __stub_GKingTryCallGuards((void*)0);
static GKingIssueTask __stub_GKingIssueTask((void*)0);
static GKingGetReward __stub_GKingGetReward((void*)0);
static S2CLineBroadcast __stub_S2CLineBroadcast((void*)0);
static GTouchPointExchange __stub_GTouchPointExchange((void*)0);
static GOpenBanquetRoleDel __stub_GOpenBanquetRoleDel((void*)0);
static GOpenBanquetClose __stub_GOpenBanquetClose((void*)0);
static FactionMultiExpSync __stub_FactionMultiExpSync((void*)0);
static FacBaseDataSend __stub_FacBaseDataSend((void*)0);
static FacBaseDataDeliver __stub_FacBaseDataDeliver((void*)0);
static FacMallChange __stub_FacMallChange((void*)0);
static StartFacBase __stub_StartFacBase((void*)0);
static StopFacBase __stub_StopFacBase((void*)0);
static FacMallSync __stub_FacMallSync((void*)0);
static FacBasePropChange __stub_FacBasePropChange((void*)0);
static FacBaseBuildingProgress __stub_FacBaseBuildingProgress((void*)0);
static FacAuctionPut __stub_FacAuctionPut((void*)0);
static GCrssvrTeamsPostScore __stub_GCrssvrTeamsPostScore((void*)0);
static SendRaidStart __stub_SendRaidStart((void*)0);
static SendRaidEnter __stub_SendRaidEnter((void*)0);
static SendRaidClose __stub_SendRaidClose((void*)0);
static SendRaidKickout __stub_SendRaidKickout((void*)0);
static Open_Controler __stub_Open_Controler((void*)0);
static AddCash_Re __stub_AddCash_Re((void*)0);
static BonusExp __stub_BonusExp((void*)0);
static UpdateRoleCircle __stub_UpdateRoleCircle((void*)0);
static BonusItem __stub_BonusItem((void*)0);
static AnnounceProviderID __stub_AnnounceProviderID((void*)0);
static PlayerKickout __stub_PlayerKickout((void*)0);
static PlayerLogin __stub_PlayerLogin((void*)0);
static PlayerOffline __stub_PlayerOffline((void*)0);
static PlayerReconnect __stub_PlayerReconnect((void*)0);
static PlayerStatusSync __stub_PlayerStatusSync((void*)0);
static PlayerStatusAnnounce __stub_PlayerStatusAnnounce((void*)0);
static BattleFlagBuffScope __stub_BattleFlagBuffScope((void*)0);
static GetCouponsReq __stub_GetCouponsReq((void*)0);
static GetTaskData_Re __stub_GetTaskData_Re((void*)0);
static SetTaskData_Re __stub_SetTaskData_Re((void*)0);
static EnterWorld __stub_EnterWorld((void*)0);
static C2SGamedataSend __stub_C2SGamedataSend((void*)0);
static PublicChat __stub_PublicChat((void*)0);
static PrivateChat __stub_PrivateChat((void*)0);
static FactionChat __stub_FactionChat((void*)0);
static GTradeStart __stub_GTradeStart((void*)0);
static GTradeEnd __stub_GTradeEnd((void*)0);
static AnnounceGM __stub_AnnounceGM((void*)0);
static GMRestartServer __stub_GMRestartServer((void*)0);
static GMShutdownLine __stub_GMShutdownLine((void*)0);
static GMailEndSync __stub_GMailEndSync((void*)0);
static QueryGameServerAttr_Re __stub_QueryGameServerAttr_Re((void*)0);
static ACReportCheater __stub_ACReportCheater((void*)0);
static BattleStart __stub_BattleStart((void*)0);
static SendBattleEnter __stub_SendBattleEnter((void*)0);
static SendInstancingStart __stub_SendInstancingStart((void*)0);
static SendInstancingEnter __stub_SendInstancingEnter((void*)0);
static GBattleJoin __stub_GBattleJoin((void*)0);
static PlayerFactionInfo_Re __stub_PlayerFactionInfo_Re((void*)0);
static SyncFactionLevel __stub_SyncFactionLevel((void*)0);
static SyncFamilyData __stub_SyncFamilyData((void*)0);
static SyncFactionHostiles __stub_SyncFactionHostiles((void*)0);
static NetBarReward __stub_NetBarReward((void*)0);
static NotifyDailyTableChange __stub_NotifyDailyTableChange((void*)0);
static OnDivorce __stub_OnDivorce((void*)0);
static CombatControl __stub_CombatControl((void*)0);
static PlayerChangeGS __stub_PlayerChangeGS((void*)0);
static SendAsyncData __stub_SendAsyncData((void*)0);
static AddictionControl __stub_AddictionControl((void*)0);
static VIPInfoNotify __stub_VIPInfoNotify((void*)0);
static HostileProtect_Re __stub_HostileProtect_Re((void*)0);
static HostileDelete_Re __stub_HostileDelete_Re((void*)0);
static HostileAdd_Re __stub_HostileAdd_Re((void*)0);
static SendSiegeStart __stub_SendSiegeStart((void*)0);
static GSiegeStatus __stub_GSiegeStatus((void*)0);
static SectQuit __stub_SectQuit((void*)0);
static SectRecruit_Re __stub_SectRecruit_Re((void*)0);
static SectExpel_Re __stub_SectExpel_Re((void*)0);
static ContestResult __stub_ContestResult((void*)0);
static SendSNSResult __stub_SendSNSResult((void*)0);
static BillingRequest2 __stub_BillingRequest2((void*)0);
static BillingBalance __stub_BillingBalance((void*)0);
static SendRefAddExp __stub_SendRefAddExp((void*)0);
static SendRefAddBonus __stub_SendRefAddBonus((void*)0);
static SendChangeProfile_Re __stub_SendChangeProfile_Re((void*)0);
static GShopSetSaleScheme __stub_GShopSetSaleScheme((void*)0);
static GShopSetDiscountScheme __stub_GShopSetDiscountScheme((void*)0);
static SendTerritoryStart __stub_SendTerritoryStart((void*)0);
static SyncTerritoryList __stub_SyncTerritoryList((void*)0);
static PlayerChangeDS __stub_PlayerChangeDS((void*)0);
static GConsignEnd __stub_GConsignEnd((void*)0);
static PlayerConsignOffline __stub_PlayerConsignOffline((void*)0);
static GConsignEndRole __stub_GConsignEndRole((void*)0);
static GChangeRolename_Re __stub_GChangeRolename_Re((void*)0);
static GChangeFactionName_Re __stub_GChangeFactionName_Re((void*)0);
static GTReconnect __stub_GTReconnect((void*)0);
static RoleEnterVoiceChannel __stub_RoleEnterVoiceChannel((void*)0);
static RoleLeaveVoiceChannel __stub_RoleLeaveVoiceChannel((void*)0);
static KingdomBattleStart __stub_KingdomBattleStart((void*)0);
static KingdomBattleStop __stub_KingdomBattleStop((void*)0);
static KingdomInfoSync __stub_KingdomInfoSync((void*)0);
static KingdomTitleSync __stub_KingdomTitleSync((void*)0);
static KingdomPointSync __stub_KingdomPointSync((void*)0);
static KingCallGuard __stub_KingCallGuard((void*)0);
static QueenOpenBath __stub_QueenOpenBath((void*)0);
static QueenCloseBath __stub_QueenCloseBath((void*)0);
static SyncBathTimes __stub_SyncBathTimes((void*)0);
static GKingIssueTask_Re __stub_GKingIssueTask_Re((void*)0);
static GKingGetReward_Re __stub_GKingGetReward_Re((void*)0);
static IsConnAlive __stub_IsConnAlive((void*)0);
static GTouchPointExchange_Re __stub_GTouchPointExchange_Re((void*)0);
static ForbidService __stub_ForbidService((void*)0);
static WebOrderNotice __stub_WebOrderNotice((void*)0);

};
