#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "passportgetrolelist.hrp"
#include "dbcreatefacbase.hrp"
#include "getfacbase.hrp"
#include "putfacbase.hrp"
#include "dbsaveweborder.hrp"
#include "getcashavail.hrp"
#include "dbcreaterole.hrp"
#include "dbdeleterole.hrp"
#include "dbundodeleterole.hrp"
#include "dbdeletefaction.hrp"
#include "dbundeletefaction.hrp"
#include "dbdeletefamily.hrp"
#include "dbundeletefamily.hrp"
#include "putuser.hrp"
#include "getuser.hrp"
#include "deluser.hrp"
#include "getrole.hrp"
#include "delrole.hrp"
#include "putrolebase.hrp"
#include "updaterolebase.hrp"
#include "getrolebase.hrp"
#include "getroleinfo.hrp"
#include "putrolestatus.hrp"
#include "getrolestatus.hrp"
#include "putrolepocket.hrp"
#include "getrolepocket.hrp"
#include "putroletask.hrp"
#include "getroletask.hrp"
#include "tradeinventory.hrp"
#include "tradesave.hrp"
#include "putrole.hrp"
#include "getmoneyinventory.hrp"
#include "putmoneyinventory.hrp"
#include "getrolebasestatus.hrp"
#include "putrolestorehouse.hrp"
#include "getrolestorehouse.hrp"
#include "putroleforbid.hrp"
#include "getroleforbid.hrp"
#include "getroleid.hrp"
#include "getfriendlist.hrp"
#include "putfriendlist.hrp"
#include "putmessage.hrp"
#include "getmessage.hrp"
#include "dbfriendcallbackinfo.hrp"
#include "dbfriendcallbackmail.hrp"
#include "dbupdatecallbackinfo.hrp"
#include "dbcheckaward.hrp"
#include "dbfriendcallbacksubscribe.hrp"
#include "getuserroles.hrp"
#include "canchangerolename.hrp"
#include "clearstorehousepasswd.hrp"
#include "renamerole.hrp"
#include "uid2logicuid.hrp"
#include "roleid2uid.hrp"
#include "gettaskdatarpc.hrp"
#include "dbsetcashpassword.hrp"
#include "puttaskdatarpc.hrp"
#include "transactionacquire.hrp"
#include "transactionabort.hrp"
#include "transactioncommit.hrp"
#include "dbverifymaster.hrp"
#include "dbgetmaillist.hrp"
#include "dbgetmail.hrp"
#include "dbgetmailattach.hrp"
#include "dbsetmailattr.hrp"
#include "dbsendmail.hrp"
#include "dbdeletemail.hrp"
#include "dbgetmailall.hrp"
#include "dbauctionopen.hrp"
#include "dbauctionbid.hrp"
#include "dbauctionget.hrp"
#include "dbauctionclose.hrp"
#include "dbauctionlist.hrp"
#include "dbauctiontimeout.hrp"
#include "getaddcashsn.hrp"
#include "cashserial.hrp"
#include "dbcrssvrteamsget.hrp"
#include "dbcrssvrteamscreate.hrp"
#include "dbcrssvrteamsrename.hrp"
#include "dbcrssvrteamsdel.hrp"
#include "dbcrssvrteamsjoin.hrp"
#include "dbcrssvrteamschgcaptain.hrp"
#include "dbcrssvrteamsleave.hrp"
#include "dbcrssvrteamsrolepunishscore.hrp"
#include "dbcrssvrteamsgetseasontop.hrp"
#include "dbcrssvrteamsseasontopsave.hrp"
#include "dbcrosscrssvrteams.hrp"
#include "dbcrosscrssvrteamsdismiss.hrp"
#include "dbcrosscrssvrteamsrename.hrp"
#include "dbcrosscrssvrteamssearch.hrp"
#include "synccrssvrteamsscore.hrp"
#include "synccrssvrteamsrole.hrp"
#include "synccrssvrteams.hrp"
#include "dbfactionadd.hrp"
#include "dbfactiondel.hrp"
#include "dbfactionjoin.hrp"
#include "dbfactionget.hrp"
#include "dbfactionleave.hrp"
#include "dbfactionsync.hrp"
#include "dbfactionupdate.hrp"
#include "dbfactionbrief.hrp"
#include "dbfactionappoint.hrp"
#include "dbfactionrename.hrp"
#include "dbsectjoin.hrp"
#include "dbsectquit.hrp"
#include "dbsectget.hrp"
#include "dbsectupdate.hrp"
#include "dbfamilyadd.hrp"
#include "dbfamilyget.hrp"
#include "dbfamilydel.hrp"
#include "dbfamilyjoin.hrp"
#include "dbfamilyleave.hrp"
#include "dbfamilyupdate.hrp"
#include "dbfamilyappoint.hrp"
#include "dbfamilysync.hrp"
#include "dbhostileadd.hrp"
#include "dbhostiledelete.hrp"
#include "dbhostileupdate.hrp"
#include "dbhostileprotect.hrp"
#include "dbfactionaddmoney.hrp"
#include "dbfactionwithdrawmoney.hrp"
#include "dbputtoptable.hrp"
#include "dbgettoptable.hrp"
#include "putspouse.hrp"
#include "dbstockload.hrp"
#include "dbstocktransaction.hrp"
#include "dbstockbalance.hrp"
#include "dbstockcommission.hrp"
#include "dbstockcancel.hrp"
#include "dbautolockset.hrp"
#include "dbsiegeload.hrp"
#include "dbsiegeend.hrp"
#include "dbsiegesetassistant.hrp"
#include "dbsiegechallenge.hrp"
#include "dbinstancingactivate.hrp"
#include "dbhometownexchgmoney.hrp"
#include "dbsiegeget.hrp"
#include "dbsiegeputdetail.hrp"
#include "dbbattlejoin.hrp"
#include "queryuserid.hrp"
#include "forbiduser.hrp"
#include "dbforbiduser.hrp"
#include "dbgetcommondata.hrp"
#include "dbputcommondata.hrp"
#include "dbsnsgetplayerinfo.hrp"
#include "dbsnsgetplayerinfocache.hrp"
#include "dbsnsupdateroleinfo.hrp"
#include "dbsnsgetroleinfo.hrp"
#include "dbsnsupdateplayerinfo.hrp"
#include "dbsnsgetmessage.hrp"
#include "dbsnsupdatemessage.hrp"
#include "dbrefgetreferral.hrp"
#include "dbrefgetreferrer.hrp"
#include "dbrefupdatereferrer.hrp"
#include "dbrefupdatereferral.hrp"
#include "dbrefwithdrawtrans.hrp"
#include "dbchangeprofile.hrp"
#include "dbclearconsumable.hrp"
#include "dbhometowngetinfo.hrp"
#include "dbhometownputinfo.hrp"
#include "dbhometownputcompoinfo.hrp"
#include "dbfungamegetinfo.hrp"
#include "dbfungamegetscore.hrp"
#include "dbfungameputinfo.hrp"
#include "dbfungameputscore.hrp"
#include "dbterritorylistload.hrp"
#include "dbterritorylistsave.hrp"
#include "dbterritorychallenge.hrp"
#include "dbterritorysave.hrp"
#include "dbuniqueauctionload.hrp"
#include "dbuniqueauctionsave.hrp"
#include "modifyrolecreatetime.hrp"
#include "dbcircleget.hrp"
#include "dbcirclehisget.hrp"
#include "dbcirclejoin.hrp"
#include "dbcircleappoint.hrp"
#include "dbcirclesync.hrp"
#include "dbconsigntableload.hrp"
#include "dbconsignprepost.hrp"
#include "dbconsignquery.hrp"
#include "dbconsignfail.hrp"
#include "dbconsignpost.hrp"
#include "dbconsignprecancel.hrp"
#include "dbconsignmail.hrp"
#include "dbconsigngetrolesimpleinfo.hrp"
#include "dbconsignshelfcancel.hrp"
#include "dbconsignshelf.hrp"
#include "dbconsignsold.hrp"
#include "dbconsignquerybackup.hrp"
#include "dbconsignprepostrole.hrp"
#include "dbconsignsoldrole.hrp"
#include "dbconsigngetrole.hrp"
#include "dbconsignrolefail.hrp"
#include "fetchplayerdata.hrp"
#include "activateplayerdata.hrp"
#include "touchplayerdata.hrp"
#include "saveplayerdata.hrp"
#include "freezeplayerdata.hrp"
#include "playeridentitymatch.hrp"
#include "delplayerdata.hrp"
#include "getcashtotal.hrp"
#include "dbraidopen.hrp"
#include "addglobalcounter.hrp"
#include "getglobalcounter.hrp"
#include "dbchangerolename.hrp"
#include "dbrolenamelist.hrp"
#include "dbqueryrolename.hrp"
#include "dbchangefactionname.hrp"
#include "dbkingdomload.hrp"
#include "dbkingdomsetwinner.hrp"
#include "dbkingdomsave.hrp"
#include "dbkingdomappoint.hrp"
#include "dbkingdomdischarge.hrp"
#include "dbkingdomtaskissue.hrp"
#include "dbkingdomtaskclose.hrp"
#include "dbkingdomtaskend.hrp"
#include "dbkinggetreward.hrp"
#include "dbgettopflowerdata.hrp"
#include "dbupdatetopflowerdata.hrp"
#include "dbflowergetroleid.hrp"
#include "dbflowergetroleexist.hrp"
#include "dbgettouchorder.hrp"
#include "operationcmd.hrp"
#include "dbgetserviceforbidcmd.hrp"
#include "debugoperationcmd.hrp"
#include "delroleannounce.hpp"
#include "addcash.hpp"
#include "addcash_re.hpp"
#include "updatetoptable.hpp"
#include "domaincmd_re.hpp"
#include "updatequeen.hpp"

namespace GNET
{

static PassportGetRoleList __stub_PassportGetRoleList (RPC_PASSPORTGETROLELIST, new PassportGetRoleListArg, new PassportGetRoleListRes);
static DBCreateFacBase __stub_DBCreateFacBase (RPC_DBCREATEFACBASE, new DBCreateFacBaseArg, new DBCreateFacBaseRes);
static GetFacBase __stub_GetFacBase (RPC_GETFACBASE, new GetFacBaseArg, new GetFacBaseRes);
static PutFacBase __stub_PutFacBase (RPC_PUTFACBASE, new PutFacBaseArg, new PutFacBaseRes);
static DBSaveWebOrder __stub_DBSaveWebOrder (RPC_DBSAVEWEBORDER, new DBSaveWebOrderArg, new DBSaveWebOrderRes);
static GetCashAvail __stub_GetCashAvail (RPC_GETCASHAVAIL, new GetCashAvailArg, new GetCashAvailRes);
static DBCreateRole __stub_DBCreateRole (RPC_DBCREATEROLE, new DBCreateRoleArg, new DBCreateRoleRes);
static DBDeleteRole __stub_DBDeleteRole (RPC_DBDELETEROLE, new DBDeleteRoleArg, new DBDeleteRoleRes);
static DBUndoDeleteRole __stub_DBUndoDeleteRole (RPC_DBUNDODELETEROLE, new DBUndoDeleteRoleArg, new DBUndoDeleteRoleRes);
static DBDeleteFaction __stub_DBDeleteFaction (RPC_DBDELETEFACTION, new DBDeleteFactionArg, new DBDeleteFactionRes);
static DBUndeleteFaction __stub_DBUndeleteFaction (RPC_DBUNDELETEFACTION, new FactionId, new DelFactionRes);
static DBDeleteFamily __stub_DBDeleteFamily (RPC_DBDELETEFAMILY, new DBDeleteFamilyArg, new DBDeleteFamilyRes);
static DBUndeleteFamily __stub_DBUndeleteFamily (RPC_DBUNDELETEFAMILY, new FamilyId, new DelFactionRes);
static PutUser __stub_PutUser (RPC_PUTUSER, new UserPair, new RpcRetcode);
static GetUser __stub_GetUser (RPC_GETUSER, new UserArg, new UserRes);
static DelUser __stub_DelUser (RPC_DELUSER, new UserID, new RpcRetcode);
static GetRole __stub_GetRole (RPC_GETROLE, new RoleArg, new RoleRes);
static DelRole __stub_DelRole (RPC_DELROLE, new RoleId, new RpcRetcode);
static PutRoleBase __stub_PutRoleBase (RPC_PUTROLEBASE, new RoleBasePair, new RpcRetcode);
static UpdateRoleBase __stub_UpdateRoleBase (RPC_UPDATEROLEBASE, new UpdateRoleBaseArg, new RpcRetcode);
static GetRoleBase __stub_GetRoleBase (RPC_GETROLEBASE, new RoleId, new RoleBaseRes);
static GetRoleInfo __stub_GetRoleInfo (RPC_GETROLEINFO, new RoleId, new RoleInfoRes);
static PutRoleStatus __stub_PutRoleStatus (RPC_PUTROLESTATUS, new RoleStatusPair, new RpcRetcode);
static GetRoleStatus __stub_GetRoleStatus (RPC_GETROLESTATUS, new RoleId, new RoleStatusRes);
static PutRolePocket __stub_PutRolePocket (RPC_PUTROLEPOCKET, new RolePocketPair, new RpcRetcode);
static GetRolePocket __stub_GetRolePocket (RPC_GETROLEPOCKET, new RoleId, new RolePocketRes);
static PutRoleTask __stub_PutRoleTask (RPC_PUTROLETASK, new RoleTaskPair, new RpcRetcode);
static GetRoleTask __stub_GetRoleTask (RPC_GETROLETASK, new RoleId, new RoleTaskRes);
static TradeInventory __stub_TradeInventory (RPC_TRADEINVENTORY, new TradeInventoryArg, new TradeInventoryRes);
static TradeSave __stub_TradeSave (RPC_TRADESAVE, new TradeSaveArg, new TradeSaveRes);
static PutRole __stub_PutRole (RPC_PUTROLE, new RolePair, new RpcRetcode);
static GetMoneyInventory __stub_GetMoneyInventory (RPC_GETMONEYINVENTORY, new GetMoneyInventoryArg, new GetMoneyInventoryRes);
static PutMoneyInventory __stub_PutMoneyInventory (RPC_PUTMONEYINVENTORY, new PutMoneyInventoryArg, new RpcRetcode);
static GetRoleBaseStatus __stub_GetRoleBaseStatus (RPC_GETROLEBASESTATUS, new RoleId, new GetRoleBaseStatusRes);
static PutRoleStorehouse __stub_PutRoleStorehouse (RPC_PUTROLESTOREHOUSE, new RoleStorehousePair, new RpcRetcode);
static GetRoleStorehouse __stub_GetRoleStorehouse (RPC_GETROLESTOREHOUSE, new RoleId, new RoleStorehouseRes);
static PutRoleForbid __stub_PutRoleForbid (RPC_PUTROLEFORBID, new RoleForbidPair, new RpcRetcode);
static GetRoleForbid __stub_GetRoleForbid (RPC_GETROLEFORBID, new GetRoleForbidArg, new GetRoleForbidRes);
static GetRoleId __stub_GetRoleId (RPC_GETROLEID, new GetRoleIdArg, new GetRoleIdRes);
static GetFriendList __stub_GetFriendList (RPC_GETFRIENDLIST, new RoleId, new FriendListRes);
static PutFriendList __stub_PutFriendList (RPC_PUTFRIENDLIST, new FriendListPair, new RpcRetcode);
static PutMessage __stub_PutMessage (RPC_PUTMESSAGE, new Message, new RpcRetcode);
static GetMessage __stub_GetMessage (RPC_GETMESSAGE, new RoleId, new GetMessageRes);
static DBFriendCallbackInfo __stub_DBFriendCallbackInfo (RPC_DBFRIENDCALLBACKINFO, new DBFriendCallbackInfoArg, new DBFriendCallbackInfoRes);
static DBFriendCallbackMail __stub_DBFriendCallbackMail (RPC_DBFRIENDCALLBACKMAIL, new DBFriendCallbackMailArg, new DBFriendCallbackMailRes);
static DBUpdateCallbackInfo __stub_DBUpdateCallbackInfo (RPC_DBUPDATECALLBACKINFO, new DBUpdateCallbackInfoArg, new DBUpdateCallbackInfoRes);
static DBCheckAward __stub_DBCheckAward (RPC_DBCHECKAWARD, new DBCheckAwardArg, new DBCheckAwardRes);
static DBFriendCallbackSubscribe __stub_DBFriendCallbackSubscribe (RPC_DBFRIENDCALLBACKSUBSCRIBE, new DBFriendCallbackSubscribeArg, new DBFriendCallbackSubscribeRes);
static GetUserRoles __stub_GetUserRoles (RPC_GETUSERROLES, new GetUserRolesArg, new GetUserRolesRes);
static CanChangeRolename __stub_CanChangeRolename (RPC_CANCHANGEROLENAME, new CanChangeRolenameArg, new CanChangeRolenameRes);
static ClearStorehousePasswd __stub_ClearStorehousePasswd (RPC_CLEARSTOREHOUSEPASSWD, new ClearStorehousePasswdArg, new RpcRetcode);
static RenameRole __stub_RenameRole (RPC_RENAMEROLE, new RenameRoleArg, new RpcRetcode);
static Uid2Logicuid __stub_Uid2Logicuid (RPC_UID2LOGICUID, new Uid2LogicuidArg, new Uid2LogicuidRes);
static Roleid2Uid __stub_Roleid2Uid (RPC_ROLEID2UID, new Roleid2UidArg, new Roleid2UidRes);
static GetTaskDataRpc __stub_GetTaskDataRpc (RPC_GETTASKDATARPC, new GTaskData, new GTaskData);
static DBSetCashPassword __stub_DBSetCashPassword (RPC_DBSETCASHPASSWORD, new DBSetCashPasswordArg, new DBSetCashPasswordRes);
static PutTaskDataRpc __stub_PutTaskDataRpc (RPC_PUTTASKDATARPC, new GTaskData, new RpcRetcode);
static TransactionAcquire __stub_TransactionAcquire (RPC_TRANSACTIONACQUIRE, new TransactionTimeout, new TransactionId);
static TransactionAbort __stub_TransactionAbort (RPC_TRANSACTIONABORT, new TransactionId, new RpcRetcode);
static TransactionCommit __stub_TransactionCommit (RPC_TRANSACTIONCOMMIT, new TransactionId, new RpcRetcode);
static DBVerifyMaster __stub_DBVerifyMaster (RPC_DBVERIFYMASTER, new DBVerifyMasterArg, new DefFactionRes);
static DBGetMailList __stub_DBGetMailList (RPC_DBGETMAILLIST, new RoleId, new DBGetMailListRes);
static DBGetMail __stub_DBGetMail (RPC_DBGETMAIL, new GMailID, new DBGetMailRes);
static DBGetMailAttach __stub_DBGetMailAttach (RPC_DBGETMAILATTACH, new DBGetMailAttachArg, new DBGetMailAttachRes);
static DBSetMailAttr __stub_DBSetMailAttr (RPC_DBSETMAILATTR, new DBSetMailAttrArg, new DBSetMailAttrRes);
static DBSendMail __stub_DBSendMail (RPC_DBSENDMAIL, new DBSendMailArg, new DBSendMailRes);
static DBDeleteMail __stub_DBDeleteMail (RPC_DBDELETEMAIL, new DBDeleteMailArg, new GMailDefRes);
static DBGetMailAll __stub_DBGetMailAll (RPC_DBGETMAILALL, new RoleId, new DBGetMailAllRes);
static DBAuctionOpen __stub_DBAuctionOpen (RPC_DBAUCTIONOPEN, new DBAuctionOpenArg, new DBAuctionOpenRes);
static DBAuctionBid __stub_DBAuctionBid (RPC_DBAUCTIONBID, new DBAuctionBidArg, new DBAuctionBidRes);
static DBAuctionGet __stub_DBAuctionGet (RPC_DBAUCTIONGET, new DBAuctionGetArg, new DBAuctionGetRes);
static DBAuctionClose __stub_DBAuctionClose (RPC_DBAUCTIONCLOSE, new DBAuctionCloseArg, new DBAuctionCloseRes);
static DBAuctionList __stub_DBAuctionList (RPC_DBAUCTIONLIST, new DBAuctionListArg, new DBAuctionListRes);
static DBAuctionTimeout __stub_DBAuctionTimeout (RPC_DBAUCTIONTIMEOUT, new AuctionId, new DBAuctionTimeoutRes);
static GetAddCashSN __stub_GetAddCashSN (RPC_GETADDCASHSN, new GetAddCashSNArg, new GetAddCashSNRes);
static CashSerial __stub_CashSerial (RPC_CASHSERIAL, new CashSerialArg, new CashSerialRes);
static DBCrssvrTeamsGet __stub_DBCrssvrTeamsGet (RPC_DBCRSSVRTEAMSGET, new TeamId, new DBCrssvrTeamsGetRes);
static DBCrssvrTeamsCreate __stub_DBCrssvrTeamsCreate (RPC_DBCRSSVRTEAMSCREATE, new DBCrssvrTeamsCreateArg, new DBCrssvrTeamsCreateRes);
static DBCrssvrTeamsRename __stub_DBCrssvrTeamsRename (RPC_DBCRSSVRTEAMSRENAME, new DBCrssvrTeamsRenameArg, new DBCrssvrTeamsRenameRes);
static DBCrssvrTeamsDel __stub_DBCrssvrTeamsDel (RPC_DBCRSSVRTEAMSDEL, new TeamId, new DelCrssvrTeamsRes);
static DBCrssvrTeamsJoin __stub_DBCrssvrTeamsJoin (RPC_DBCRSSVRTEAMSJOIN, new DBCrssvrTeamsJoinArg, new DBCrssvrTeamsJoinRes);
static DBCrssvrTeamsChgCaptain __stub_DBCrssvrTeamsChgCaptain (RPC_DBCRSSVRTEAMSCHGCAPTAIN, new DBCrssvrTeamsChgCaptainArg, new DBCrssvrTeamsChgCaptainRes);
static DBCrssvrTeamsLeave __stub_DBCrssvrTeamsLeave (RPC_DBCRSSVRTEAMSLEAVE, new DBCrssvrTeamsLeaveArg, new DBCrssvrTeamsLeaveRes);
static DBCrssvrTeamsRolePunishScore __stub_DBCrssvrTeamsRolePunishScore (RPC_DBCRSSVRTEAMSROLEPUNISHSCORE, new DBCrssvrTeamsRolePunishScoreArg, new DBCrssvrTeamsRolePunishScoreRes);
static DBCrssvrTeamsGetSeasonTop __stub_DBCrssvrTeamsGetSeasonTop (RPC_DBCRSSVRTEAMSGETSEASONTOP, new DBCrssvrTeamsGetSeasonTopArg, new DBCrssvrTeamsGetSeasonTopRes);
static DBCrssvrTeamsSeasonTopSave __stub_DBCrssvrTeamsSeasonTopSave (RPC_DBCRSSVRTEAMSSEASONTOPSAVE, new DBCrssvrTeamsSeasonTopSaveArg, new DBCrssvrTeamsSeasonTopSaveRes);
static DBCrossCrssvrTeams __stub_DBCrossCrssvrTeams (RPC_DBCROSSCRSSVRTEAMS, new DBCrossCrssvrTeamsArg, new DBCrossCrssvrTeamsRes);
static DBCrossCrssvrTeamsDismiss __stub_DBCrossCrssvrTeamsDismiss (RPC_DBCROSSCRSSVRTEAMSDISMISS, new DBCrossCrssvrTeamsDismissArg, new DBCrossCrssvrTeamsDismissRes);
static DBCrossCrssvrTeamsRename __stub_DBCrossCrssvrTeamsRename (RPC_DBCROSSCRSSVRTEAMSRENAME, new DBCrossCrssvrTeamsRenameArg, new DBCrossCrssvrTeamsRenameRes);
static DBCrossCrssvrTeamsSearch __stub_DBCrossCrssvrTeamsSearch (RPC_DBCROSSCRSSVRTEAMSSEARCH, new DBCrossCrssvrTeamsSearchArg, new DBCrossCrssvrTeamsSearchRes);
static SyncCrssvrTeamsScore __stub_SyncCrssvrTeamsScore (RPC_SYNCCRSSVRTEAMSSCORE, new SyncCrssvrTeamsScoreArg, new SyncCrssvrTeamsScoreRes);
static SyncCrssvrTeamsRole __stub_SyncCrssvrTeamsRole (RPC_SYNCCRSSVRTEAMSROLE, new SyncCrssvrTeamsRoleArg, new SyncCrssvrTeamsRoleRes);
static SyncCrssvrTeams __stub_SyncCrssvrTeams (RPC_SYNCCRSSVRTEAMS, new SyncCrssvrTeamsArg, new SyncCrssvrTeamsRes);
static DBFactionAdd __stub_DBFactionAdd (RPC_DBFACTIONADD, new DBFactionAddArg, new DBFactionAddRes);
static DBFactionDel __stub_DBFactionDel (RPC_DBFACTIONDEL, new FactionId, new DelFactionRes);
static DBFactionJoin __stub_DBFactionJoin (RPC_DBFACTIONJOIN, new DBFactionJoinArg, new DBFactionJoinRes);
static DBFactionGet __stub_DBFactionGet (RPC_DBFACTIONGET, new FactionId, new FactionInfoRes);
static DBFactionLeave __stub_DBFactionLeave (RPC_DBFACTIONLEAVE, new DBFactionLeaveArg, new DefFactionRes);
static DBFactionSync __stub_DBFactionSync (RPC_DBFACTIONSYNC, new DBFactionSyncArg, new DBFactionSyncRes);
static DBFactionUpdate __stub_DBFactionUpdate (RPC_DBFACTIONUPDATE, new DBFactionUpdateArg, new DefFactionRes);
static DBFactionBrief __stub_DBFactionBrief (RPC_DBFACTIONBRIEF, new FactionId, new FactionBriefRes);
static DBFactionAppoint __stub_DBFactionAppoint (RPC_DBFACTIONAPPOINT, new DBFactionAppointArg, new DBFactionAppointRes);
static DBFactionRename __stub_DBFactionRename (RPC_DBFACTIONRENAME, new DBFactionRenameArg, new DefFactionRes);
static DBSectJoin __stub_DBSectJoin (RPC_DBSECTJOIN, new DBSectJoinArg, new DBSectJoinRes);
static DBSectQuit __stub_DBSectQuit (RPC_DBSECTQUIT, new DBSectQuitArg, new DBSectQuitRes);
static DBSectGet __stub_DBSectGet (RPC_DBSECTGET, new Integer, new DBSectGetRes);
static DBSectUpdate __stub_DBSectUpdate (RPC_DBSECTUPDATE, new DBSectUpdateArg, new DBSectUpdateRes);
static DBFamilyAdd __stub_DBFamilyAdd (RPC_DBFAMILYADD, new DBFamilyAddArg, new DBFamilyAddRes);
static DBFamilyGet __stub_DBFamilyGet (RPC_DBFAMILYGET, new FamilyId, new FamilyGetRes);
static DBFamilyDel __stub_DBFamilyDel (RPC_DBFAMILYDEL, new FamilyId, new DelFactionRes);
static DBFamilyJoin __stub_DBFamilyJoin (RPC_DBFAMILYJOIN, new DBFamilyJoinArg, new DBFamilyJoinRes);
static DBFamilyLeave __stub_DBFamilyLeave (RPC_DBFAMILYLEAVE, new DBFamilyLeaveArg, new DelFactionRes);
static DBFamilyUpdate __stub_DBFamilyUpdate (RPC_DBFAMILYUPDATE, new DBFamilyUpdateArg, new DefFactionRes);
static DBFamilyAppoint __stub_DBFamilyAppoint (RPC_DBFAMILYAPPOINT, new DBFamilyAppointArg, new DefFactionRes);
static DBFamilySync __stub_DBFamilySync (RPC_DBFAMILYSYNC, new DBFamilySyncArg, new DefFactionRes);
static DBHostileAdd __stub_DBHostileAdd (RPC_DBHOSTILEADD, new DBHostileAddArg, new DBHostileAddRes);
static DBHostileDelete __stub_DBHostileDelete (RPC_DBHOSTILEDELETE, new DBHostileDeleteArg, new DefFactionRes);
static DBHostileUpdate __stub_DBHostileUpdate (RPC_DBHOSTILEUPDATE, new DBHostileUpdateArg, new DefFactionRes);
static DBHostileProtect __stub_DBHostileProtect (RPC_DBHOSTILEPROTECT, new DBHostileProtectArg, new DBHostileProtectRes);
static DBFactionAddMoney __stub_DBFactionAddMoney (RPC_DBFACTIONADDMONEY, new DBFactionAddMoneyArg, new DBFactionAddMoneyRes);
static DBFactionWithDrawMoney __stub_DBFactionWithDrawMoney (RPC_DBFACTIONWITHDRAWMONEY, new DBFactionWithDrawMoneyArg, new DBFactionWithDrawMoneyRes);
static DBPutTopTable __stub_DBPutTopTable (RPC_DBPUTTOPTABLE, new GTopTable, new Integer);
static DBGetTopTable __stub_DBGetTopTable (RPC_DBGETTOPTABLE, new Integer, new TopTableRes);
static PutSpouse __stub_PutSpouse (RPC_PUTSPOUSE, new PutSpouseArg, new RpcRetcode);
static DBStockLoad __stub_DBStockLoad (RPC_DBSTOCKLOAD, new DBStockLoadArg, new DBStockLoadRes);
static DBStockTransaction __stub_DBStockTransaction (RPC_DBSTOCKTRANSACTION, new DBStockTransactionArg, new DBStockTransactionRes);
static DBStockBalance __stub_DBStockBalance (RPC_DBSTOCKBALANCE, new DBStockBalanceArg, new DBStockBalanceRes);
static DBStockCommission __stub_DBStockCommission (RPC_DBSTOCKCOMMISSION, new DBStockCommissionArg, new DBStockCommissionRes);
static DBStockCancel __stub_DBStockCancel (RPC_DBSTOCKCANCEL, new DBStockCancelArg, new DBStockCancelRes);
static DBAutolockSet __stub_DBAutolockSet (RPC_DBAUTOLOCKSET, new DBAutolockSetArg, new Integer);
static DBSiegeLoad __stub_DBSiegeLoad (RPC_DBSIEGELOAD, new DBSiegeLoadArg, new DBSiegeLoadRes);
static DBSiegeEnd __stub_DBSiegeEnd (RPC_DBSIEGEEND, new DBSiegeEndArg, new DBSiegeEndRes);
static DBSiegeSetAssistant __stub_DBSiegeSetAssistant (RPC_DBSIEGESETASSISTANT, new DBSiegeSetAssistantArg, new DBSiegeSetAssistantRes);
static DBSiegeChallenge __stub_DBSiegeChallenge (RPC_DBSIEGECHALLENGE, new DBSiegeChallengeArg, new DBSiegeChallengeRes);
static DBInstancingActivate __stub_DBInstancingActivate (RPC_DBINSTANCINGACTIVATE, new DBInstancingActivateArg, new DBInstancingActivateRes);
static DBHometownExchgMoney __stub_DBHometownExchgMoney (RPC_DBHOMETOWNEXCHGMONEY, new DBHometownExchgMoneyArg, new DBHometownExchgMoneyRes);
static DBSiegeGet __stub_DBSiegeGet (RPC_DBSIEGEGET, new DBSiegeGetArg, new DBSiegeGetRes);
static DBSiegePutDetail __stub_DBSiegePutDetail (RPC_DBSIEGEPUTDETAIL, new DBSiegePutDetailArg, new DBSiegePutDetailRes);
static DBBattleJoin __stub_DBBattleJoin (RPC_DBBATTLEJOIN, new DBBattleJoinArg, new DBBattleJoinRes);
static QueryUserid __stub_QueryUserid (RPC_QUERYUSERID, new QueryUseridArg, new QueryUseridRes);
static ForbidUser __stub_ForbidUser (RPC_FORBIDUSER, new ForbidUserArg, new ForbidUserRes);
static DBForbidUser __stub_DBForbidUser (RPC_DBFORBIDUSER, new ForbidUserArg, new RpcRetcode);
static DBGetCommonData __stub_DBGetCommonData (RPC_DBGETCOMMONDATA, new DBGetCommonDataArg, new DBGetCommonDataRes);
static DBPutCommonData __stub_DBPutCommonData (RPC_DBPUTCOMMONDATA, new DBPutCommonDataArg, new Integer);
static DBSNSGetPlayerInfo __stub_DBSNSGetPlayerInfo (RPC_DBSNSGETPLAYERINFO, new DBSNSGetPlayerInfoArg, new DBSNSGetPlayerInfoRes);
static DBSNSGetPlayerInfoCache __stub_DBSNSGetPlayerInfoCache (RPC_DBSNSGETPLAYERINFOCACHE, new DBSNSGetPlayerInfoCacheArg, new DBSNSGetPlayerInfoCacheRes);
static DBSNSUpdateRoleInfo __stub_DBSNSUpdateRoleInfo (RPC_DBSNSUPDATEROLEINFO, new DBSNSUpdateRoleInfoArg, new DBSNSUpdateRoleInfoRes);
static DBSNSGetRoleInfo __stub_DBSNSGetRoleInfo (RPC_DBSNSGETROLEINFO, new DBSNSGetRoleInfoArg, new DBSNSGetRoleInfoRes);
static DBSNSUpdatePlayerInfo __stub_DBSNSUpdatePlayerInfo (RPC_DBSNSUPDATEPLAYERINFO, new DBSNSUpdatePlayerInfoArg, new DBSNSUpdatePlayerInfoRes);
static DBSNSGetMessage __stub_DBSNSGetMessage (RPC_DBSNSGETMESSAGE, new DBSNSGetMessageArg, new DBSNSGetMessageRes);
static DBSNSUpdateMessage __stub_DBSNSUpdateMessage (RPC_DBSNSUPDATEMESSAGE, new DBSNSUpdateMessageArg, new DBSNSUpdateMessageRes);
static DBRefGetReferral __stub_DBRefGetReferral (RPC_DBREFGETREFERRAL, new RoleId, new DBRefGetReferralRes);
static DBRefGetReferrer __stub_DBRefGetReferrer (RPC_DBREFGETREFERRER, new RoleId, new DBRefGetReferrerRes);
static DBRefUpdateReferrer __stub_DBRefUpdateReferrer (RPC_DBREFUPDATEREFERRER, new DBRefUpdateReferrerArg, new RpcRetcode);
static DBRefUpdateReferral __stub_DBRefUpdateReferral (RPC_DBREFUPDATEREFERRAL, new DBRefUpdateReferralArg, new RpcRetcode);
static DBRefWithdrawTrans __stub_DBRefWithdrawTrans (RPC_DBREFWITHDRAWTRANS, new DBRefWithdrawTransArg, new RpcRetcode);
static DBChangeProfile __stub_DBChangeProfile (RPC_DBCHANGEPROFILE, new DBChangeProfileArg, new RpcRetcode);
static DBClearConsumable __stub_DBClearConsumable (RPC_DBCLEARCONSUMABLE, new DBClearConsumableArg, new RpcRetcode);
static DBHometownGetInfo __stub_DBHometownGetInfo (RPC_DBHOMETOWNGETINFO, new DBHometownGetInfoArg, new DBHometownGetInfoRes);
static DBHometownPutInfo __stub_DBHometownPutInfo (RPC_DBHOMETOWNPUTINFO, new DBHometownPutInfoArg, new RpcRetcode);
static DBHometownPutCompoInfo __stub_DBHometownPutCompoInfo (RPC_DBHOMETOWNPUTCOMPOINFO, new DBHometownPutCompoInfoArg, new RpcRetcode);
static DBFunGameGetInfo __stub_DBFunGameGetInfo (RPC_DBFUNGAMEGETINFO, new DBFunGameGetInfoArg, new DBFunGameGetInfoRes);
static DBFunGameGetScore __stub_DBFunGameGetScore (RPC_DBFUNGAMEGETSCORE, new DBFunGameGetScoreArg, new DBFunGameGetScoreRes);
static DBFunGamePutInfo __stub_DBFunGamePutInfo (RPC_DBFUNGAMEPUTINFO, new DBFunGamePutInfoArg, new RpcRetcode);
static DBFunGamePutScore __stub_DBFunGamePutScore (RPC_DBFUNGAMEPUTSCORE, new DBFunGamePutScoreArg, new RpcRetcode);
static DBTerritoryListLoad __stub_DBTerritoryListLoad (RPC_DBTERRITORYLISTLOAD, new DBTerritoryListLoadArg, new DBTerritoryListLoadRes);
static DBTerritoryListSave __stub_DBTerritoryListSave (RPC_DBTERRITORYLISTSAVE, new DBTerritoryListSaveArg, new DBTerritoryListSaveRes);
static DBTerritoryChallenge __stub_DBTerritoryChallenge (RPC_DBTERRITORYCHALLENGE, new DBTerritoryChallengeArg, new DBTerritoryChallengeRes);
static DBTerritorySave __stub_DBTerritorySave (RPC_DBTERRITORYSAVE, new DBTerritorySaveArg, new DBTerritorySaveRes);
static DBUniqueAuctionLoad __stub_DBUniqueAuctionLoad (RPC_DBUNIQUEAUCTIONLOAD, new DBUniqueAuctionLoadArg, new DBUniqueAuctionLoadRes);
static DBUniqueAuctionSave __stub_DBUniqueAuctionSave (RPC_DBUNIQUEAUCTIONSAVE, new DBUniqueAuctionSaveArg, new DBUniqueAuctionSaveRes);
static ModifyRoleCreateTime __stub_ModifyRoleCreateTime (RPC_MODIFYROLECREATETIME, new ModifyRoleCreateTimeArg, new ModifyRoleCreateTimeRes);
static DBCircleGet __stub_DBCircleGet (RPC_DBCIRCLEGET, new CircleID, new DBCircleGetRes);
static DBCircleHisGet __stub_DBCircleHisGet (RPC_DBCIRCLEHISGET, new RoleId, new DBCircleHisGetRes);
static DBCircleJoin __stub_DBCircleJoin (RPC_DBCIRCLEJOIN, new DBCircleJoinArg, new DBCircleJoinRes);
static DBCircleAppoint __stub_DBCircleAppoint (RPC_DBCIRCLEAPPOINT, new DBCircleAppointArg, new DBCircleAppointRes);
static DBCircleSync __stub_DBCircleSync (RPC_DBCIRCLESYNC, new DBCircleSyncArg, new DefCircleRes);
static DBConsignTableLoad __stub_DBConsignTableLoad (RPC_DBCONSIGNTABLELOAD, new DBConsignTableLoadArg, new DBConsignTableLoadRes);
static DBConsignPrePost __stub_DBConsignPrePost (RPC_DBCONSIGNPREPOST, new DBConsignPrePostArg, new DBConsignPrePostRes);
static DBConsignQuery __stub_DBConsignQuery (RPC_DBCONSIGNQUERY, new DBConsignQueryArg, new DBConsignQueryRes);
static DBConsignFail __stub_DBConsignFail (RPC_DBCONSIGNFAIL, new DBConsignFailArg, new DBConsignFailRes);
static DBConsignPost __stub_DBConsignPost (RPC_DBCONSIGNPOST, new DBConsignPostArg, new DBConsignPostRes);
static DBConsignPreCancel __stub_DBConsignPreCancel (RPC_DBCONSIGNPRECANCEL, new DBConsignPreCancelArg, new DBConsignPreCancelRes);
static DBConsignMail __stub_DBConsignMail (RPC_DBCONSIGNMAIL, new DBConsignMailArg, new DBConsignMailRes);
static DBConsignGetRoleSimpleInfo __stub_DBConsignGetRoleSimpleInfo (RPC_DBCONSIGNGETROLESIMPLEINFO, new DBConsignGetRoleSimpleInfoArg, new DBConsignGetRoleSimpleInfoRes);
static DBConsignShelfCancel __stub_DBConsignShelfCancel (RPC_DBCONSIGNSHELFCANCEL, new DBConsignShelfCancelArg, new DBConsignShelfCancelRes);
static DBConsignShelf __stub_DBConsignShelf (RPC_DBCONSIGNSHELF, new DBConsignShelfArg, new DBConsignShelfRes);
static DBConsignSold __stub_DBConsignSold (RPC_DBCONSIGNSOLD, new DBConsignSoldArg, new DBConsignSoldRes);
static DBConsignQueryBackup __stub_DBConsignQueryBackup (RPC_DBCONSIGNQUERYBACKUP, new DBConsignQueryBackupArg, new DBConsignQueryBackupRes);
static DBConsignPrePostRole __stub_DBConsignPrePostRole (RPC_DBCONSIGNPREPOSTROLE, new DBConsignPrePostRoleArg, new DBConsignPrePostRoleRes);
static DBConsignSoldRole __stub_DBConsignSoldRole (RPC_DBCONSIGNSOLDROLE, new DBConsignSoldArg, new DBConsignSoldRes);
static DBConsignGetRole __stub_DBConsignGetRole (RPC_DBCONSIGNGETROLE, new DBConsignGetRoleArg, new DBConsignGetRoleRes);
static DBConsignRoleFail __stub_DBConsignRoleFail (RPC_DBCONSIGNROLEFAIL, new DBConsignRoleFailArg, new DBConsignRoleFailRes);
static FetchPlayerData __stub_FetchPlayerData (RPC_FETCHPLAYERDATA, new FetchPlayerDataArg, new FetchPlayerDataRes);
static ActivatePlayerData __stub_ActivatePlayerData (RPC_ACTIVATEPLAYERDATA, new ActivatePlayerDataArg, new ActivatePlayerDataRes);
static TouchPlayerData __stub_TouchPlayerData (RPC_TOUCHPLAYERDATA, new TouchPlayerDataArg, new TouchPlayerDataRes);
static SavePlayerData __stub_SavePlayerData (RPC_SAVEPLAYERDATA, new SavePlayerDataArg, new SavePlayerDataRes);
static FreezePlayerData __stub_FreezePlayerData (RPC_FREEZEPLAYERDATA, new FreezePlayerDataArg, new FreezePlayerDataRes);
static PlayerIdentityMatch __stub_PlayerIdentityMatch (RPC_PLAYERIDENTITYMATCH, new PlayerIdentityMatchArg, new PlayerIdentityMatchRes);
static DelPlayerData __stub_DelPlayerData (RPC_DELPLAYERDATA, new DelPlayerDataArg, new DelPlayerDataRes);
static GetCashTotal __stub_GetCashTotal (RPC_GETCASHTOTAL, new RoleId, new GetCashTotalRes);
static DBRaidOpen __stub_DBRaidOpen (RPC_DBRAIDOPEN, new DBRaidOpenArg, new DBRaidOpenRes);
static AddGlobalCounter __stub_AddGlobalCounter (RPC_ADDGLOBALCOUNTER, new AddGlobalCounterArg, new AddGlobalCounterRes);
static GetGlobalCounter __stub_GetGlobalCounter (RPC_GETGLOBALCOUNTER, new GetGlobalCounterArg, new GetGlobalCounterRes);
static DBChangeRolename __stub_DBChangeRolename (RPC_DBCHANGEROLENAME, new DBChangeRolenameArg, new DBChangeRolenameRes);
static DBRoleNameList __stub_DBRoleNameList (RPC_DBROLENAMELIST, new DBRoleNameListArg, new DBRoleNameListRes);
static DBQueryRoleName __stub_DBQueryRoleName (RPC_DBQUERYROLENAME, new DBQueryRoleNameArg, new DBQueryRoleNameRes);
static DBChangeFactionname __stub_DBChangeFactionname (RPC_DBCHANGEFACTIONNAME, new DBChangeFactionnameArg, new DBChangeFactionnameRes);
static DBKingdomLoad __stub_DBKingdomLoad (RPC_DBKINGDOMLOAD, new DBKingdomLoadArg, new DBKingdomLoadRes);
static DBKingdomSetWinner __stub_DBKingdomSetWinner (RPC_DBKINGDOMSETWINNER, new DBKingdomSetWinnerArg, new DBKingdomSetWinnerRes);
static DBKingdomSave __stub_DBKingdomSave (RPC_DBKINGDOMSAVE, new DBKingdomSaveArg, new DBKingdomSaveRes);
static DBKingdomAppoint __stub_DBKingdomAppoint (RPC_DBKINGDOMAPPOINT, new DBKingdomAppointArg, new DBKingdomAppointRes);
static DBKingdomDischarge __stub_DBKingdomDischarge (RPC_DBKINGDOMDISCHARGE, new DBKingdomDischargeArg, new DBKingdomDischargeRes);
static DBKingdomTaskIssue __stub_DBKingdomTaskIssue (RPC_DBKINGDOMTASKISSUE, new DBKingdomTaskIssueArg, new DBKingdomTaskIssueRes);
static DBKingdomTaskClose __stub_DBKingdomTaskClose (RPC_DBKINGDOMTASKCLOSE, new DBKingdomTaskCloseArg, new DBKingdomTaskCloseRes);
static DBKingdomTaskEnd __stub_DBKingdomTaskEnd (RPC_DBKINGDOMTASKEND, new DBKingdomTaskEndArg, new DBKingdomTaskEndRes);
static DBKingGetReward __stub_DBKingGetReward (RPC_DBKINGGETREWARD, new DBKingGetRewardArg, new DBKingGetRewardRes);
static DBGetTopFlowerData __stub_DBGetTopFlowerData (RPC_DBGETTOPFLOWERDATA, new DBGetTopFlowerDataArg, new DBGetTopFlowerDataRes);
static DBUpdateTopFlowerData __stub_DBUpdateTopFlowerData (RPC_DBUPDATETOPFLOWERDATA, new DBUpdateTopFlowerDataArg, new DBUpdateTopFlowerDataRes);
static DBFlowerGetRoleId __stub_DBFlowerGetRoleId (RPC_DBFLOWERGETROLEID, new DBFlowerGetRoleIdArg, new DBFlowerGetRoleIdRes);
static DBFlowerGetRoleExist __stub_DBFlowerGetRoleExist (RPC_DBFLOWERGETROLEEXIST, new DBFlowerGetRoleExistArg, new DBFlowerGetRoleExistRes);
static DBGetTouchOrder __stub_DBGetTouchOrder (RPC_DBGETTOUCHORDER, new DBGetTouchOrderArg, new DBGetTouchOrderRes);
static OperationCmd __stub_OperationCmd (RPC_OPERATIONCMD, new OperationCmdArg, new OperationCmdRes);
static DBGetServiceForbidCmd __stub_DBGetServiceForbidCmd (RPC_DBGETSERVICEFORBIDCMD, new DBGetServiceForbidCmdArg, new DBGetServiceForbidCmdRes);
static DebugOperationCmd __stub_DebugOperationCmd (RPC_DEBUGOPERATIONCMD, new DebugOperationCmdArg, new DebugOperationCmdRes);
static DelRoleAnnounce __stub_DelRoleAnnounce((void*)0);
static AddCash __stub_AddCash((void*)0);
static AddCash_Re __stub_AddCash_Re((void*)0);
static UpdateTopTable __stub_UpdateTopTable((void*)0);
static DomainCmd_Re __stub_DomainCmd_Re((void*)0);
static UpdateQueen __stub_UpdateQueen((void*)0);

};
