#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "flowergifttake.hrp"
#include "flowertakeoff.hrp"
#include "commondatachange.hrp"
#include "gquerypasswd.hrp"
#include "matrixpasswd.hrp"
#include "matrixpasswd2.hrp"
#include "matrixtoken.hrp"
#include "userlogin.hrp"
#include "userlogin2.hrp"
#include "certchallenge.hrp"
#include "getaddcashsn.hrp"
#include "cashserial.hrp"
#include "putspouse.hrp"
#include "queryuserid.hrp"
#include "forbiduser.hrp"
#include "playeridentitymatch.hrp"
#include "passportgetrolelist.hrp"
#include "getlineplayerlimit.hrp"
#include "setlineplayerlimit.hrp"
#include "crssvrteamsinvite.hrp"
#include "factioninvite.hrp"
#include "sectinvite.hrp"
#include "combatinvite.hrp"
#include "dbgettopflowerdata.hrp"
#include "dbupdatetopflowerdata.hrp"
#include "dbflowergetroleid.hrp"
#include "dbflowergetroleexist.hrp"
#include "commondataquery.hrp"
#include "addfriendrqst.hrp"
#include "tradestartrqst.hrp"
#include "gmqueryroleinfo.hrp"
#include "getmaxonlinenum.hrp"
#include "gmgetgameattri.hrp"
#include "gmsetgameattri.hrp"
#include "dbverifymaster.hrp"
#include "gshopgetscheme.hrp"
#include "kingguardinvite.hrp"
#include "queenopenbathinvite.hrp"
#include "operationcmd.hrp"
#include "userlogout.hrp"
#include "netmarblelogin.hrp"
#include "netmarblededuct.hrp"
#include "circlegetasyncdata.hrp"
#include "getweeklytop.hrp"
#include "getdailytop.hrp"
#include "taskasyncdata.hrp"
#include "gterritoryitemget.hrp"
#include "guniquebid.hrp"
#include "guniquegetitem.hrp"
#include "getfriendnumber.hrp"
#include "getcashavail.hrp"
#include "dbcreatefacbase.hrp"
#include "getfacbase.hrp"
#include "putfacbase.hrp"
#include "dbsaveweborder.hrp"
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
#include "dbgettouchorder.hrp"
#include "dbgetserviceforbidcmd.hrp"
#include "debugoperationcmd.hrp"
#include "precreaterole.hrp"
#include "postcreaterole.hrp"
#include "postdeleterole.hrp"
#include "precreatefaction.hrp"
#include "postcrssvrteamscreate.hrp"
#include "postcrssvrteamsrename.hrp"
#include "postcrssvrteamsdel.hrp"
#include "precrssvrteamscreate.hrp"
#include "precrssvrteamsrename.hrp"
#include "postcreatefaction.hrp"
#include "postdeletefaction.hrp"
#include "precreatefamily.hrp"
#include "postcreatefamily.hrp"
#include "postdeletefamily.hrp"
#include "prechangerolename.hrp"
#include "postchangerolename.hrp"
#include "prechangefactionname.hrp"
#include "postchangefactionname.hrp"
#include "accountaddrole.hrp"
#include "accountdelrole.hrp"
#include "synccrosscrssvrteamsscore.hrp"
#include "synccrosscrssvrteamstop.hrp"
#include "synccrosscrssvrteamsendbrief.hrp"
#include "synccrosscrssvrteams.hrp"
#include "synccrosscrssvrteamsupdate.hrp"
#include "synccrosscrssvrteamsadd.hrp"
#include "synccrosscrssvrteamsdel.hrp"
#include "synccrosscrssvrteamsrename.hrp"
#include "synccrosscrssvrteamschgcaptain.hrp"
#include "synccrosscrssvrteamsdismiss.hrp"
#include "crosscrssvrteamsgettoptable.hrp"
#include "gategetrolerelation.hrp"
#include "factionmultiexpsync.hpp"
#include "syncroleevent2sns.hpp"
#include "getfacacttoptable_re.hpp"
#include "facbasestopnotice.hpp"
#include "createfacbase_re.hpp"
#include "facbaseenter_re.hpp"
#include "startfacbase.hpp"
#include "stopfacbase.hpp"
#include "facmallsync.hpp"
#include "getfactiondynamic_re.hpp"
#include "globaldropremainbroadcast.hpp"
#include "getkdctokentogate.hpp"
#include "raidjoinreq.hpp"
#include "raidopen_re.hpp"
#include "raidjoin_re.hpp"
#include "raidjoinapplylist.hpp"
#include "raidjoinaccept_re.hpp"
#include "raidenter_re.hpp"
#include "crossraidenter_re.hpp"
#include "raidgetlist_re.hpp"
#include "raidstart_re.hpp"
#include "raidappoint_re.hpp"
#include "raidappointnotify.hpp"
#include "raidstartnotify.hpp"
#include "raidkick_re.hpp"
#include "raidkicknotify.hpp"
#include "raidquit_re.hpp"
#include "raidgetroom_re.hpp"
#include "gcrssvrteamspostscore.hpp"
#include "sendraidstart.hpp"
#include "sendraidenter.hpp"
#include "graidmemberchange.hpp"
#include "sendraidclose.hpp"
#include "sendraidkickout.hpp"
#include "raidopenvote_re.hpp"
#include "raidbeginvotes.hpp"
#include "raidvoteresult.hpp"
#include "raidbriefinfo.hpp"
#include "teamraidapply_re.hpp"
#include "teamraidapplynotice.hpp"
#include "teamraidactiveapplynotice.hpp"
#include "teamraidmappingsuccess.hpp"
#include "hideseekmappingsuccess.hpp"
#include "teamraidbeginchoosechar.hpp"
#include "teamraidupdatechar.hpp"
#include "teamraidacktimeout.hpp"
#include "factionpkraidchallenge_re.hpp"
#include "factionpkraidchallengecancel_re.hpp"
#include "factionpkraidinvite.hpp"
#include "factionpkraidbegin.hpp"
#include "factionpkraidgetscore_re.hpp"
#include "factionpkraidgetlist_re.hpp"
#include "factionpkraidstopfight.hpp"
#include "factionwithdrawbonus_re.hpp"
#include "open_controler.hpp"
#include "gopenbanquetroledel.hpp"
#include "gopenbanquetclose.hpp"
#include "updatefriendgtstatus.hpp"
#include "circlelist_re.hpp"
#include "getcirclebaseinfo_re.hpp"
#include "bonusexp.hpp"
#include "updaterolecircle.hpp"
#include "circlenotifylink.hpp"
#include "consigncancelpost_re.hpp"
#include "consignquery_re.hpp"
#include "consigngetitem_re.hpp"
#include "consignlistall_re.hpp"
#include "consignlistrole_re.hpp"
#include "consignlistlargecategory_re.hpp"
#include "consignstarterr.hpp"
#include "dbconsignskipsn.hpp"
#include "consignplayer_re.hpp"
#include "gaterolelogin.hpp"
#include "gaterolecreate.hpp"
#include "gaterolelogout.hpp"
#include "gateonlinelist.hpp"
#include "gategetgtrolestatus.hpp"
#include "rolegroupupdate.hpp"
#include "rolefriendupdate.hpp"
#include "factionmemberupdate.hpp"
#include "factioninfoupdate.hpp"
#include "removerole.hpp"
#include "removefaction.hpp"
#include "post.hpp"
#include "gamepostcancel.hpp"
#include "webpostcancel_re.hpp"
#include "shelf_re.hpp"
#include "shelfcancel_re.hpp"
#include "sold_re.hpp"
#include "postexpire_re.hpp"
#include "webgetrolelist_re.hpp"
#include "newkeepalive.hpp"
#include "friendcallbackinfo_re.hpp"
#include "friendcallbackmail_re.hpp"
#include "friendcallbackaward_re.hpp"
#include "friendcallbacksubscribe_re.hpp"
#include "bonusitem.hpp"
#include "announcezoneid.hpp"
#include "announcezoneid2.hpp"
#include "announcezoneid3.hpp"
#include "announcechallengealgo.hpp"
#include "queryuserprivilege.hpp"
#include "queryuserprivilege2.hpp"
#include "queryuserforbid.hpp"
#include "queryuserforbid2.hpp"
#include "playerkickout.hpp"
#include "playeroffline.hpp"
#include "gettaskdata_re.hpp"
#include "settaskdata_re.hpp"
#include "playerstatusannounce.hpp"
#include "onlineannounce.hpp"
#include "rolelist_re.hpp"
#include "createrole_re.hpp"
#include "deleterole_re.hpp"
#include "undodeleterole_re.hpp"
#include "playerbaseinfo_re.hpp"
#include "playerbaseinfo2_re.hpp"
#include "playerbaseinfocrc_re.hpp"
#include "getplayeridbyname_re.hpp"
#include "setuiconfig_re.hpp"
#include "getuiconfig_re.hpp"
#include "sethelpstates_re.hpp"
#include "gethelpstates_re.hpp"
#include "getplayerbriefinfo_re.hpp"
#include "commondatasync.hpp"
#include "notifydailytablechange.hpp"
#include "accountingrequest.hpp"
#include "chatbroadcast.hpp"
#include "worldchat.hpp"
#include "chatmulticast.hpp"
#include "chatsinglecast.hpp"
#include "rolestatusannounce.hpp"
#include "addfriend_re.hpp"
#include "getfriends_re.hpp"
#include "getenemies_re.hpp"
#include "updateenemy_re.hpp"
#include "setgroupname_re.hpp"
#include "setfriendgroup_re.hpp"
#include "getsavedmsg_re.hpp"
#include "chatroomcreate_re.hpp"
#include "chatroominvite_re.hpp"
#include "chatroomjoin_re.hpp"
#include "chatroomlist_re.hpp"
#include "tradestart_re.hpp"
#include "tradeaddgoods_re.hpp"
#include "traderemovegoods_re.hpp"
#include "tradesubmit_re.hpp"
#include "trademoveobj_re.hpp"
#include "tradeconfirm_re.hpp"
#include "tradediscard_re.hpp"
#include "tradeend.hpp"
#include "gtradestart.hpp"
#include "gtradeend.hpp"
#include "certrequest.hpp"
#include "certanswer.hpp"
#include "certkey.hpp"
#include "certfinish.hpp"
#include "game2au.hpp"
#include "rolelogin.hpp"
#include "announcegm.hpp"
#include "gmonlinenum_re.hpp"
#include "gmlistonlineuser_re.hpp"
#include "gmkickoutuser_re.hpp"
#include "gmforbidsellpoint_re.hpp"
#include "gmkickoutrole_re.hpp"
#include "gmshutup_re.hpp"
#include "gmshutuprole_re.hpp"
#include "gmtogglechat_re.hpp"
#include "gmforbidrole_re.hpp"
#include "gmshutdownline_re.hpp"
#include "report2gm_re.hpp"
#include "complain2gm_re.hpp"
#include "announceforbidinfo.hpp"
#include "setmaxonlinenum_re.hpp"
#include "acwhoami.hpp"
#include "acstatusannounce.hpp"
#include "acstatusannounce2.hpp"
#include "announcenewmail.hpp"
#include "getmaillist_re.hpp"
#include "getmail_re.hpp"
#include "getmailattachobj_re.hpp"
#include "deletemail_re.hpp"
#include "preservemail_re.hpp"
#include "playersendmail_re.hpp"
#include "gmailendsync.hpp"
#include "syssendmail_re.hpp"
#include "sysrecoveredobjmail_re.hpp"
#include "auctionopen_re.hpp"
#include "auctionbid_re.hpp"
#include "auctionclose_re.hpp"
#include "auctionlist_re.hpp"
#include "auctionget_re.hpp"
#include "auctiongetitem_re.hpp"
#include "auctionattendlist_re.hpp"
#include "auctionexitbid_re.hpp"
#include "battlegetmap_re.hpp"
#include "battlegetlist_re.hpp"
#include "battlegetfield_re.hpp"
#include "battlejoin_re.hpp"
#include "battleleave_re.hpp"
#include "battleenter_re.hpp"
#include "battlestart_re.hpp"
#include "battlestart.hpp"
#include "battlestartnotify.hpp"
#include "sendbattleenter.hpp"
#include "vipinfonotify.hpp"
#include "sendinstancingstart.hpp"
#include "sendinstancingenter.hpp"
#include "sendinstancingjoin.hpp"
#include "instancinggetlist_re.hpp"
#include "instancinggetfield_re.hpp"
#include "instancingkick_re.hpp"
#include "instancingstart_re.hpp"
#include "instancingavailablelist_re.hpp"
#include "instancingjoinreq.hpp"
#include "instancingjoin_re.hpp"
#include "hometownexchgmoney_re.hpp"
#include "instancingleave_re.hpp"
#include "instancingappoint_re.hpp"
#include "instancingaccept_re.hpp"
#include "instancingappointnotify.hpp"
#include "instancingkicknotify.hpp"
#include "instancingenter_re.hpp"
#include "instancingstartnotify.hpp"
#include "instancingclosenotify.hpp"
#include "queryrewardtype.hpp"
#include "queryrewardtype_re.hpp"
#include "querygameserverattr_re.hpp"
#include "announceserverattribute.hpp"
#include "crssvrteamsrolenotify_re.hpp"
#include "crssvrteamsteambase_re.hpp"
#include "crssvrteamsrolelist_re.hpp"
#include "crssvrteamsnotify_re.hpp"
#include "crssvrteamssearch_re.hpp"
#include "factionappoint_re.hpp"
#include "factionannounce_re.hpp"
#include "factioncreate_re.hpp"
#include "familyrecord.hpp"
#include "factiondismiss_re.hpp"
#include "factionexpel_re.hpp"
#include "factionlist_re.hpp"
#include "factionleave_re.hpp"
#include "factionnickname_re.hpp"
#include "factionresign_re.hpp"
#include "factionrecruit_re.hpp"
#include "factionupgrade_re.hpp"
#include "gettoptable_re.hpp"
#include "sectlist_re.hpp"
#include "sectrecruit_re.hpp"
#include "sectexpel_re.hpp"
#include "sectexpel.hpp"
#include "sectquit.hpp"
#include "getfactionbaseinfo_re.hpp"
#include "getfactionhostile_re.hpp"
#include "playerfactioninfo_re.hpp"
#include "syncfactionlevel.hpp"
#include "syncplayerfaction.hpp"
#include "hostileadd_re.hpp"
#include "hostiledelete_re.hpp"
#include "hostileprotect_re.hpp"
#include "syncfactionhostiles.hpp"
#include "ondivorce.hpp"
#include "stockcommission_re.hpp"
#include "stockaccount_re.hpp"
#include "stocktransaction_re.hpp"
#include "stockbill_re.hpp"
#include "stockcancel_re.hpp"
#include "combatchallenge_re.hpp"
#include "combatcontrol.hpp"
#include "combatstatus.hpp"
#include "combattop_re.hpp"
#include "cashlock_re.hpp"
#include "cashpasswordset_re.hpp"
#include "syncfamilydata.hpp"
#include "netbarreward.hpp"
#include "sendasyncdata.hpp"
#include "domaincmd_re.hpp"
#include "autolockset_re.hpp"
#include "sendsiegestart.hpp"
#include "gsiegestatus.hpp"
#include "siegechallenge_re.hpp"
#include "siegesetassistant_re.hpp"
#include "siegeinfoget_re.hpp"
#include "siegeenter_re.hpp"
#include "siegetop_re.hpp"
#include "forwardchat.hpp"
#include "contestinvitebroadcast.hpp"
#include "contestinvite.hpp"
#include "contestbegin.hpp"
#include "contestquestion.hpp"
#include "contestanswer_re.hpp"
#include "contestplacenotify.hpp"
#include "contestend.hpp"
#include "contestresult.hpp"
#include "s2cfungamedatasend.hpp"
#include "s2chometowndatasend.hpp"
#include "sendsnsresult.hpp"
#include "snssetplayerinfo_re.hpp"
#include "snspressmessage_re.hpp"
#include "snscancelmessage_re.hpp"
#include "snscancelleavemessage_re.hpp"
#include "snsapply_re.hpp"
#include "snsapplynotify.hpp"
#include "snslistmessage_re.hpp"
#include "snsgetmessage_re.hpp"
#include "snsgetplayerinfo_re.hpp"
#include "snsvote_re.hpp"
#include "snsacceptapply_re.hpp"
#include "reflistreferrals_re.hpp"
#include "refwithdrawexp_re.hpp"
#include "refwithdrawbonus_re.hpp"
#include "sendrefaddexp.hpp"
#include "sendrefaddbonus.hpp"
#include "refgetreferencecode_re.hpp"
#include "sendchangeprofile_re.hpp"
#include "repairrolelist.hpp"
#include "gshopnotifyscheme.hpp"
#include "netmarblelogout.hpp"
#include "territorymapget_re.hpp"
#include "territorychallenge_re.hpp"
#include "territoryenter_re.hpp"
#include "territoryleave_re.hpp"
#include "sendterritorystart.hpp"
#include "syncterritorylist.hpp"
#include "territoryenterremind.hpp"
#include "territoryscoreupdate.hpp"
#include "uniquebidhistory_re.hpp"
#include "uniquebid_re.hpp"
#include "uniquegetitem_re.hpp"
#include "playerchangegs.hpp"
#include "playerchangeds.hpp"
#include "changeds_re.hpp"
#include "trychangegs_re.hpp"
#include "dochangegs_re.hpp"
#include "gconsignend.hpp"
#include "playerconsignoffline.hpp"
#include "gconsignendrole.hpp"
#include "disableautolock.hpp"
#include "fastpay_re.hpp"
#include "fastpaybindinfo.hpp"
#include "gchangerolename_re.hpp"
#include "gchangefactionname_re.hpp"
#include "queryrolenamehistory_re.hpp"
#include "changerolename_re.hpp"
#include "changefactionname_re.hpp"
#include "friendnamechange.hpp"
#include "kingdombattlestart.hpp"
#include "kingdombattlestop.hpp"
#include "kingdominfosync.hpp"
#include "kingdomtitlesync.hpp"
#include "kingdompointsync.hpp"
#include "kingdombattleenter_re.hpp"
#include "kingdombattleleave_re.hpp"
#include "kingdomannounce_re.hpp"
#include "kingdomappoint_re.hpp"
#include "kingdomdischarge_re.hpp"
#include "kingdomsetgameattri_re.hpp"
#include "kingdomgetinfo_re.hpp"
#include "kingdomgettitle_re.hpp"
#include "kingdomtitlechange.hpp"
#include "kingdompointchange.hpp"
#include "kingdomkingchange.hpp"
#include "kingcallguards_re.hpp"
#include "kingcallguard.hpp"
#include "queenopenbath.hpp"
#include "queenclosebath.hpp"
#include "queenopenbath_re.hpp"
#include "kingissuetask_re.hpp"
#include "gkingissuetask_re.hpp"
#include "gkinggetreward_re.hpp"
#include "getkingtask_re.hpp"
#include "kinggetreward_re.hpp"
#include "topflower_re.hpp"
#include "topflower_err.hpp"
#include "topflowermsg_re.hpp"
#include "openbanquetjoin_re.hpp"
#include "openbanquetnotify_re.hpp"
#include "openbanquetdrag_re.hpp"
#include "openbanquetqueuecount_re.hpp"
#include "openbanquetenter_re.hpp"
#include "openbanquetgetfield_re.hpp"
#include "gtouchpointexchange_re.hpp"
#include "gettouchpoint_re.hpp"
#include "getpassportcashadd_re.hpp"
#include "touchpointexchange_re.hpp"
#include "forbidservice.hpp"
#include "syssendmail4_re.hpp"
#include "webordernotice.hpp"
#include "tryreconnect_re.hpp"
#include "playerreconnect.hpp"
#include "facbasedatasend.hpp"
#include "createfacbase.hpp"
#include "getfacacttoptable.hpp"
#include "getfactiondynamic.hpp"
#include "getkdctoken.hpp"
#include "raidstart.hpp"
#include "raidenter.hpp"
#include "raidquit.hpp"
#include "teamraidquit.hpp"
#include "hideseekraidquit.hpp"
#include "raidgetlist.hpp"
#include "raidappoint.hpp"
#include "raidkick.hpp"
#include "raidgetroom.hpp"
#include "raidjoinaccept.hpp"
#include "raidopenvote.hpp"
#include "raidvotes.hpp"
#include "teamraidmappingsuccess_re.hpp"
#include "hideseekmappingsuccess_re.hpp"
#include "factionpkraidchallenge.hpp"
#include "factionpkraidchallengecancel.hpp"
#include "factionpkraidinvite_re.hpp"
#include "factionpkraidgetscore.hpp"
#include "factionpkraidgetlist.hpp"
#include "factionwithdrawbonus.hpp"
#include "getfriendgtstatus.hpp"
#include "gcirclechat.hpp"
#include "circlelist.hpp"
#include "getcirclebaseinfo.hpp"
#include "claimcirclegradbonus.hpp"
#include "consigncancelpost.hpp"
#include "consignquery.hpp"
#include "consigngetitem.hpp"
#include "consignlistall.hpp"
#include "consignlistrole.hpp"
#include "consignlistlargecategory.hpp"
#include "playerlogin.hpp"
#include "playerstatussync.hpp"
#include "enterworld.hpp"
#include "statusannounce.hpp"
#include "rolelist.hpp"
#include "createrole.hpp"
#include "deleterole.hpp"
#include "undodeleterole.hpp"
#include "playerbaseinfo.hpp"
#include "playerbaseinfo2.hpp"
#include "playerbaseinfocrc.hpp"
#include "getplayeridbyname.hpp"
#include "setuiconfig.hpp"
#include "getuiconfig.hpp"
#include "sethelpstates.hpp"
#include "gethelpstates.hpp"
#include "getplayerbriefinfo.hpp"
#include "battleflagbuffscope.hpp"
#include "getcouponsreq.hpp"
#include "publicchat.hpp"
#include "privatechat.hpp"
#include "addfriend.hpp"
#include "getfriends.hpp"
#include "setgroupname.hpp"
#include "setfriendgroup.hpp"
#include "delfriend.hpp"
#include "delfriend_re.hpp"
#include "friendstatus.hpp"
#include "getenemies.hpp"
#include "updateenemy.hpp"
#include "getsavedmsg.hpp"
#include "chatroomcreate.hpp"
#include "chatroominvite.hpp"
#include "chatroomjoin.hpp"
#include "chatroomleave.hpp"
#include "chatroomexpel.hpp"
#include "chatroomspeak.hpp"
#include "chatroomlist.hpp"
#include "friendcallbackinfo.hpp"
#include "friendcallbackmail.hpp"
#include "friendcallbackaward.hpp"
#include "friendcallbacksubscribe.hpp"
#include "tradestart.hpp"
#include "tradeaddgoods.hpp"
#include "traderemovegoods.hpp"
#include "tradesubmit.hpp"
#include "trademoveobj.hpp"
#include "tradeconfirm.hpp"
#include "tradediscard.hpp"
#include "matrixfailure.hpp"
#include "tryreconnect.hpp"
#include "gmrestartserver.hpp"
#include "gmshutdownline.hpp"
#include "gmonlinenum.hpp"
#include "gmlistonlineuser.hpp"
#include "gmkickoutuser.hpp"
#include "gmforbidsellpoint.hpp"
#include "gmkickoutrole.hpp"
#include "gmshutup.hpp"
#include "gmshutuprole.hpp"
#include "gmtogglechat.hpp"
#include "gmforbidrole.hpp"
#include "gmprivilegechange.hpp"
#include "report2gm.hpp"
#include "complain2gm.hpp"
#include "announcelinktype.hpp"
#include "setmaxonlinenum.hpp"
#include "verifymaster.hpp"
#include "verifymaster_re.hpp"
#include "acreport.hpp"
#include "acanswer.hpp"
#include "acprotostat.hpp"
#include "reportip.hpp"
#include "acaccuse.hpp"
#include "checknewmail.hpp"
#include "syssendmail.hpp"
#include "sysrecoveredobjmail.hpp"
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
#include "auctionattendlist.hpp"
#include "auctionexitbid.hpp"
#include "instancinggetlist.hpp"
#include "instancinggetfield.hpp"
#include "instancingkick.hpp"
#include "instancingstart.hpp"
#include "instancingavailablelist.hpp"
#include "instancingaccept.hpp"
#include "instancingleave.hpp"
#include "instancingappoint.hpp"
#include "instancingenter.hpp"
#include "battlegetmap.hpp"
#include "battlegetlist.hpp"
#include "battlegetfield.hpp"
#include "battlejoin.hpp"
#include "battleenter.hpp"
#include "battleleave.hpp"
#include "crssvrteamsgetrolenotify.hpp"
#include "crssvrteamsgetteambase.hpp"
#include "crssvrteamsgetrolelist.hpp"
#include "crssvrteamsrename.hpp"
#include "crssvrteamsdismiss.hpp"
#include "crssvrteamsrecruit.hpp"
#include "crssvrteamschgcaptain.hpp"
#include "crssvrteamsleave.hpp"
#include "crssvrteamskick.hpp"
#include "crssvrteamssearch.hpp"
#include "factionchat.hpp"
#include "factioncreate.hpp"
#include "factionlist.hpp"
#include "factiondismiss.hpp"
#include "factionrecruit.hpp"
#include "factionexpel.hpp"
#include "factionannounce.hpp"
#include "factionappoint.hpp"
#include "factionresign.hpp"
#include "factionleave.hpp"
#include "factionupgrade.hpp"
#include "factionnickname.hpp"
#include "getfactionbaseinfo.hpp"
#include "getfactionhostile.hpp"
#include "hostiledelete.hpp"
#include "hostileadd.hpp"
#include "hostileprotect.hpp"
#include "sectrecruit.hpp"
#include "sectlist.hpp"
#include "gettoptable.hpp"
#include "combatchallenge.hpp"
#include "combattop.hpp"
#include "cashlock.hpp"
#include "cashpasswordset.hpp"
#include "domainlogin.hpp"
#include "domainvalidate.hpp"
#include "domaincmd.hpp"
#include "autolockset.hpp"
#include "siegeinfoget.hpp"
#include "siegeenter.hpp"
#include "siegetop.hpp"
#include "contestinvite_re.hpp"
#include "contestanswer.hpp"
#include "contestexit.hpp"
#include "c2sfungamedatasend.hpp"
#include "c2shometowndatasend.hpp"
#include "snssetplayerinfo.hpp"
#include "snsgetplayerinfo.hpp"
#include "snscancelmessage.hpp"
#include "snscancelleavemessage.hpp"
#include "snslistmessage.hpp"
#include "snsgetmessage.hpp"
#include "snsacceptapply.hpp"
#include "reflistreferrals.hpp"
#include "refwithdrawexp.hpp"
#include "refwithdrawbonus.hpp"
#include "refgetreferencecode.hpp"
#include "gshopsetsalescheme.hpp"
#include "gshopsetdiscountscheme.hpp"
#include "territorymapget.hpp"
#include "uniquebidhistory.hpp"
#include "trychangegs.hpp"
#include "dochangegs.hpp"
#include "ssogetticketreq.hpp"
#include "fastpay.hpp"
#include "queryrolenamehistory.hpp"
#include "kingdomannounce.hpp"
#include "kingdomappoint.hpp"
#include "kingdomdischarge.hpp"
#include "kingdomsetgameattri.hpp"
#include "kingdomgetinfo.hpp"
#include "kingdomgettitle.hpp"
#include "queentryopenbath.hpp"
#include "getkingtask.hpp"
#include "sendflowerreq.hpp"
#include "topflowerreq.hpp"
#include "topflowergetgift.hpp"
#include "openbanquetcancel.hpp"
#include "openbanquetqueuecount.hpp"
#include "openbanquetenter.hpp"
#include "openbanquetgetfield.hpp"
#include "gettouchpoint.hpp"
#include "getpassportcashadd.hpp"
#include "keyexchange.hpp"
#include "kickoutuser.hpp"
#include "accountingresponse.hpp"
#include "queryuserprivilege_re.hpp"
#include "queryuserforbid_re.hpp"
#include "updateremaintime.hpp"
#include "transbuypoint_re.hpp"
#include "addcash.hpp"
#include "addcash_re.hpp"
#include "addictioncontrol.hpp"
#include "vipannounce.hpp"
#include "netbarannounce.hpp"
#include "billingrequest.hpp"
#include "billingbalance.hpp"
#include "authdversion.hpp"
#include "ssogetticketrep.hpp"
#include "discountannounce.hpp"
#include "au2game.hpp"
#include "syssendmail4.hpp"
#include "syncplayerfaccouponadd.hpp"
#include "startfactionmultiexp.hpp"
#include "facbasedatabroadcast.hpp"
#include "facbasedatadeliver.hpp"
#include "facmallchange.hpp"
#include "facdyndonatecash.hpp"
#include "facdynputauction.hpp"
#include "facdynwinauction.hpp"
#include "facdynbuyauction.hpp"
#include "facdynbuildingupgrade.hpp"
#include "facdynbuildingcomplete.hpp"
#include "gfacbaseenter.hpp"
#include "gfacbaseleave.hpp"
#include "facbasepropchange.hpp"
#include "facbasebuildingprogress.hpp"
#include "facauctionput.hpp"
#include "startfacbase_re.hpp"
#include "stopfacbase_re.hpp"
#include "gupdatefacact.hpp"
#include "gfacbaseserverregister.hpp"
#include "globaldropbroadcast.hpp"
#include "registerglobalcounter.hpp"
#include "graidserverregister.hpp"
#include "graidopen.hpp"
#include "graidjoin.hpp"
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
#include "addcirclepoint.hpp"
#include "announceproviderid.hpp"
#include "playerlogin_re.hpp"
#include "playerkickout_re.hpp"
#include "playerlogout.hpp"
#include "playeroffline_re.hpp"
#include "playerreconnect_re.hpp"
#include "queryplayerstatus.hpp"
#include "gettaskdata.hpp"
#include "settaskdata.hpp"
#include "setchatemotion.hpp"
#include "battleflagstart.hpp"
#include "querybattleflagbuff.hpp"
#include "getcouponsrep.hpp"
#include "gtradestart_re.hpp"
#include "gtradediscard.hpp"
#include "keepalive.hpp"
#include "disconnectplayer.hpp"
#include "gmrestartserver_re.hpp"
#include "auctiongetitem.hpp"
#include "querygameserverattr.hpp"
#include "acreportcheater.hpp"
#include "actriggerquestion.hpp"
#include "battleserverregister.hpp"
#include "battleenterfail.hpp"
#include "gbattleend.hpp"
#include "gbattleleave.hpp"
#include "debugcommand.hpp"
#include "gopenbanquetleave.hpp"
#include "gopenbanquetjoin.hpp"
#include "instancingregister.hpp"
#include "instancingenterfail.hpp"
#include "ginstancingend.hpp"
#include "sendinstancingstart_re.hpp"
#include "ginstancingleave.hpp"
#include "gbattlejoin.hpp"
#include "sendfactioncreate.hpp"
#include "playerfactioninfo.hpp"
#include "ghostileprotect.hpp"
#include "sendcrssvrteamscreate.hpp"
#include "syncroledata.hpp"
#include "stockcommission.hpp"
#include "stockaccount.hpp"
#include "stocktransaction.hpp"
#include "stockbill.hpp"
#include "stockcancel.hpp"
#include "combatkill.hpp"
#include "playerchangegs_re.hpp"
#include "familyuseskill.hpp"
#include "familyexpshare.hpp"
#include "gsiegeserverregister.hpp"
#include "gsiegeend.hpp"
#include "sendsiegestart_re.hpp"
#include "sendsiegechallenge.hpp"
#include "siegesetassistant.hpp"
#include "siegekill.hpp"
#include "siegebroadcat.hpp"
#include "sectupdate.hpp"
#include "sendsnspressmessage.hpp"
#include "sendsnsapply.hpp"
#include "sendsnsvote.hpp"
#include "billingrequest2.hpp"
#include "sendrefcashused.hpp"
#include "sendreflevelup.hpp"
#include "sendchangeprofile.hpp"
#include "querynetbarreward.hpp"
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
#include "syncbathtimes.hpp"
#include "isconnalive.hpp"
#include "s2clinebroadcast.hpp"
#include "gtouchpointexchange.hpp"
#include "delroleannounce.hpp"
#include "updatetoptable.hpp"
#include "updatequeen.hpp"
#include "acremotecode.hpp"
#include "acquestion.hpp"
#include "ackickoutuser.hpp"
#include "acforbiduser.hpp"
#include "acaccuse_re.hpp"
#include "post_re.hpp"
#include "gamepostcancel_re.hpp"
#include "webpostcancel.hpp"
#include "shelf.hpp"
#include "shelfcancel.hpp"
#include "sold.hpp"
#include "postexpire.hpp"
#include "webgetrolelist.hpp"
#include "loadexchange.hpp"
#include "dsannounceidentity.hpp"
#include "senddataandidentity.hpp"
#include "senddataandidentity_re.hpp"
#include "remoteloginquery.hpp"
#include "remotelogout.hpp"
#include "kickoutremoteuser_re.hpp"
#include "getremoteroleinfo_re.hpp"
#include "crossbattlejointeam.hpp"
#include "crosscrssvrteamsonlinestatus.hpp"
#include "crosscrssvrteamsgetreq.hpp"
#include "remoteloginquery_re.hpp"
#include "kickoutremoteuser.hpp"
#include "getremoteroleinfo.hpp"
#include "copenbanquetjoin.hpp"
#include "openbanquetlogout.hpp"
#include "getkdctoken_re.hpp"
#include "gateonlinelist_re.hpp"
#include "gategetgtrolestatus_re.hpp"
#include "gateupdatestatus.hpp"
#include "gateofflinechat.hpp"
#include "gatefactionchat.hpp"

namespace GNET
{

static FlowerGiftTake __stub_FlowerGiftTake (RPC_FLOWERGIFTTAKE, new FlowerGiftTakeArg, new FlowerGiftTakeRes);
static FlowerTakeOff __stub_FlowerTakeOff (RPC_FLOWERTAKEOFF, new FlowerTakeOffArg, new FlowerTakeOffRes);
static CommonDataChange __stub_CommonDataChange (RPC_COMMONDATACHANGE, new CommonDataArg, new CommonDataRes);
static GQueryPasswd __stub_GQueryPasswd (RPC_GQUERYPASSWD, new GQueryPasswdArg, new GQueryPasswdRes);
static MatrixPasswd __stub_MatrixPasswd (RPC_MATRIXPASSWD, new MatrixPasswdArg, new MatrixPasswdRes);
static MatrixPasswd2 __stub_MatrixPasswd2 (RPC_MATRIXPASSWD2, new MatrixPasswdArg2, new MatrixPasswdRes2);
static MatrixToken __stub_MatrixToken (RPC_MATRIXTOKEN, new MatrixTokenArg, new MatrixTokenRes);
static UserLogin __stub_UserLogin (RPC_USERLOGIN, new UserLoginArg, new UserLoginRes);
static UserLogin2 __stub_UserLogin2 (RPC_USERLOGIN2, new UserLoginArg2, new UserLoginRes2);
static CertChallenge __stub_CertChallenge (RPC_CERTCHALLENGE, new CertChallengeArg, new CertChallengeRes);
static GetAddCashSN __stub_GetAddCashSN (RPC_GETADDCASHSN, new GetAddCashSNArg, new GetAddCashSNRes);
static CashSerial __stub_CashSerial (RPC_CASHSERIAL, new CashSerialArg, new CashSerialRes);
static PutSpouse __stub_PutSpouse (RPC_PUTSPOUSE, new PutSpouseArg, new RpcRetcode);
static QueryUserid __stub_QueryUserid (RPC_QUERYUSERID, new QueryUseridArg, new QueryUseridRes);
static ForbidUser __stub_ForbidUser (RPC_FORBIDUSER, new ForbidUserArg, new ForbidUserRes);
static PlayerIdentityMatch __stub_PlayerIdentityMatch (RPC_PLAYERIDENTITYMATCH, new PlayerIdentityMatchArg, new PlayerIdentityMatchRes);
static PassportGetRoleList __stub_PassportGetRoleList (RPC_PASSPORTGETROLELIST, new PassportGetRoleListArg, new PassportGetRoleListRes);
static GetLinePlayerLimit __stub_GetLinePlayerLimit (RPC_GETLINEPLAYERLIMIT, new GetLinePlayerLimitArg, new LinePlayerNumberLimits);
static SetLinePlayerLimit __stub_SetLinePlayerLimit (RPC_SETLINEPLAYERLIMIT, new LinePlayerNumberLimits, new SetLinePlayerLimitRes);
static CrssvrTeamsInvite __stub_CrssvrTeamsInvite (RPC_CRSSVRTEAMSINVITE, new CrssvrTeamsInviteArg, new CrssvrTeamsInviteRes);
static FactionInvite __stub_FactionInvite (RPC_FACTIONINVITE, new FactionInviteArg, new FactionInviteRes);
static SectInvite __stub_SectInvite (RPC_SECTINVITE, new SectInviteArg, new SectInviteRes);
static CombatInvite __stub_CombatInvite (RPC_COMBATINVITE, new CombatInviteArg, new CombatInviteRes);
static DBGetTopFlowerData __stub_DBGetTopFlowerData (RPC_DBGETTOPFLOWERDATA, new DBGetTopFlowerDataArg, new DBGetTopFlowerDataRes);
static DBUpdateTopFlowerData __stub_DBUpdateTopFlowerData (RPC_DBUPDATETOPFLOWERDATA, new DBUpdateTopFlowerDataArg, new DBUpdateTopFlowerDataRes);
static DBFlowerGetRoleId __stub_DBFlowerGetRoleId (RPC_DBFLOWERGETROLEID, new DBFlowerGetRoleIdArg, new DBFlowerGetRoleIdRes);
static DBFlowerGetRoleExist __stub_DBFlowerGetRoleExist (RPC_DBFLOWERGETROLEEXIST, new DBFlowerGetRoleExistArg, new DBFlowerGetRoleExistRes);
static CommonDataQuery __stub_CommonDataQuery (RPC_COMMONDATAQUERY, new CommonDataQueryArg, new CommonDataQueryRes);
static AddFriendRqst __stub_AddFriendRqst (RPC_ADDFRIENDRQST, new AddFriendRqstArg, new AddFriendRqstRes);
static TradeStartRqst __stub_TradeStartRqst (RPC_TRADESTARTRQST, new TradeStartRqstArg, new TradeStartRqstRes);
static GMQueryRoleInfo __stub_GMQueryRoleInfo (RPC_GMQUERYROLEINFO, new RoleId, new GMQueryRoleInfoRes);
static GetMaxOnlineNum __stub_GetMaxOnlineNum (RPC_GETMAXONLINENUM, new GetMaxOnlineNumArg, new GetMaxOnlineNumRes);
static GMGetGameAttri __stub_GMGetGameAttri (RPC_GMGETGAMEATTRI, new GMGetGameAttriArg, new GMGetGameAttriRes);
static GMSetGameAttri __stub_GMSetGameAttri (RPC_GMSETGAMEATTRI, new GMSetGameAttriArg, new GMSetGameAttriRes);
static DBVerifyMaster __stub_DBVerifyMaster (RPC_DBVERIFYMASTER, new DBVerifyMasterArg, new DefFactionRes);
static GShopGetScheme __stub_GShopGetScheme (RPC_GSHOPGETSCHEME, new Integer, new GShopScheme);
static KingGuardInvite __stub_KingGuardInvite (RPC_KINGGUARDINVITE, new KingGuardInviteArg, new KingGuardInviteRes);
static QueenOpenBathInvite __stub_QueenOpenBathInvite (RPC_QUEENOPENBATHINVITE, new QueenOpenBathInviteArg, new QueenOpenBathInviteRes);
static OperationCmd __stub_OperationCmd (RPC_OPERATIONCMD, new OperationCmdArg, new OperationCmdRes);
static UserLogout __stub_UserLogout (RPC_USERLOGOUT, new UserLogoutArg, new UserLogoutRes);
static NetMarbleLogin __stub_NetMarbleLogin (RPC_NETMARBLELOGIN, new NetMarbleLoginArg, new NetMarbleLoginRes);
static NetMarbleDeduct __stub_NetMarbleDeduct (RPC_NETMARBLEDEDUCT, new NetMarbleDeductArg, new NetMarbleDeductRes);
static CircleGetAsyncData __stub_CircleGetAsyncData (RPC_CIRCLEGETASYNCDATA, new CircleAsyncData, new CircleAsyncData);
static GetWeeklyTop __stub_GetWeeklyTop (RPC_GETWEEKLYTOP, new Integer, new WeeklyTopRes);
static GetDailyTop __stub_GetDailyTop (RPC_GETDAILYTOP, new Integer, new DailyTopRes);
static TaskAsyncData __stub_TaskAsyncData (RPC_TASKASYNCDATA, new AsyncData, new AsyncData);
static GTerritoryItemGet __stub_GTerritoryItemGet (RPC_GTERRITORYITEMGET, new GTerritoryItemGetArg, new GTerritoryItemGetRes);
static GUniqueBid __stub_GUniqueBid (RPC_GUNIQUEBID, new GUniqueBidArg, new GUniqueBidRes);
static GUniqueGetItem __stub_GUniqueGetItem (RPC_GUNIQUEGETITEM, new GUniqueGetItemArg, new GUniqueGetItemRes);
static GetFriendNumber __stub_GetFriendNumber (RPC_GETFRIENDNUMBER, new GetFriendNumberArg, new GetFriendNumberRes);
static GetCashAvail __stub_GetCashAvail (RPC_GETCASHAVAIL, new GetCashAvailArg, new GetCashAvailRes);
static DBCreateFacBase __stub_DBCreateFacBase (RPC_DBCREATEFACBASE, new DBCreateFacBaseArg, new DBCreateFacBaseRes);
static GetFacBase __stub_GetFacBase (RPC_GETFACBASE, new GetFacBaseArg, new GetFacBaseRes);
static PutFacBase __stub_PutFacBase (RPC_PUTFACBASE, new PutFacBaseArg, new PutFacBaseRes);
static DBSaveWebOrder __stub_DBSaveWebOrder (RPC_DBSAVEWEBORDER, new DBSaveWebOrderArg, new DBSaveWebOrderRes);
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
static DBGetTouchOrder __stub_DBGetTouchOrder (RPC_DBGETTOUCHORDER, new DBGetTouchOrderArg, new DBGetTouchOrderRes);
static DBGetServiceForbidCmd __stub_DBGetServiceForbidCmd (RPC_DBGETSERVICEFORBIDCMD, new DBGetServiceForbidCmdArg, new DBGetServiceForbidCmdRes);
static DebugOperationCmd __stub_DebugOperationCmd (RPC_DEBUGOPERATIONCMD, new DebugOperationCmdArg, new DebugOperationCmdRes);
static PreCreateRole __stub_PreCreateRole (RPC_PRECREATEROLE, new PreCreateRoleArg, new PreCreateRoleRes);
static PostCreateRole __stub_PostCreateRole (RPC_POSTCREATEROLE, new PostCreateRoleArg, new PostCreateRoleRes);
static PostDeleteRole __stub_PostDeleteRole (RPC_POSTDELETEROLE, new PostDeleteRoleArg, new PostDeleteRoleRes);
static PreCreateFaction __stub_PreCreateFaction (RPC_PRECREATEFACTION, new PreCreateFactionArg, new PreCreateFactionRes);
static PostCrssvrTeamsCreate __stub_PostCrssvrTeamsCreate (RPC_POSTCRSSVRTEAMSCREATE, new PostCrssvrTeamsCreateArg, new PostCrssvrTeamsCreateRes);
static PostCrssvrTeamsRename __stub_PostCrssvrTeamsRename (RPC_POSTCRSSVRTEAMSRENAME, new PostCrssvrTeamsRenameArg, new PostCrssvrTeamsRenameRes);
static PostCrssvrTeamsDel __stub_PostCrssvrTeamsDel (RPC_POSTCRSSVRTEAMSDEL, new PostCrssvrTeamsDelArg, new PostCrssvrTeamsDelRes);
static PreCrssvrTeamsCreate __stub_PreCrssvrTeamsCreate (RPC_PRECRSSVRTEAMSCREATE, new PreCrssvrTeamsCreateArg, new PreCrssvrTeamsCreateRes);
static PreCrssvrTeamsRename __stub_PreCrssvrTeamsRename (RPC_PRECRSSVRTEAMSRENAME, new PreCrssvrTeamsRenameArg, new PreCrssvrTeamsRenameRes);
static PostCreateFaction __stub_PostCreateFaction (RPC_POSTCREATEFACTION, new PostCreateFactionArg, new PostCreateFactionRes);
static PostDeleteFaction __stub_PostDeleteFaction (RPC_POSTDELETEFACTION, new PostDeleteFactionArg, new PostDeleteFactionRes);
static PreCreateFamily __stub_PreCreateFamily (RPC_PRECREATEFAMILY, new PreCreateFamilyArg, new PreCreateFamilyRes);
static PostCreateFamily __stub_PostCreateFamily (RPC_POSTCREATEFAMILY, new PostCreateFamilyArg, new PostCreateFamilyRes);
static PostDeleteFamily __stub_PostDeleteFamily (RPC_POSTDELETEFAMILY, new PostDeleteFamilyArg, new PostDeleteFamilyRes);
static PreChangeRolename __stub_PreChangeRolename (RPC_PRECHANGEROLENAME, new PreChangeRolenameArg, new PreChangeRolenameRes);
static PostChangeRolename __stub_PostChangeRolename (RPC_POSTCHANGEROLENAME, new PostChangeRolenameArg, new PostChangeRolenameRes);
static PreChangeFactionname __stub_PreChangeFactionname (RPC_PRECHANGEFACTIONNAME, new PreChangeFactionnameArg, new PreChangeFactionnameRes);
static PostChangeFactionname __stub_PostChangeFactionname (RPC_POSTCHANGEFACTIONNAME, new PostChangeFactionnameArg, new PostChangeFactionnameRes);
static AccountAddRole __stub_AccountAddRole (RPC_ACCOUNTADDROLE, new AccountAddRoleArg, new AccountAddRoleRes);
static AccountDelRole __stub_AccountDelRole (RPC_ACCOUNTDELROLE, new AccountDelRoleArg, new AccountDelRoleRes);
static SyncCrossCrssvrTeamsScore __stub_SyncCrossCrssvrTeamsScore (RPC_SYNCCROSSCRSSVRTEAMSSCORE, new SyncCrssvrTeamsScoreArg, new SyncCrssvrTeamsScoreRes);
static SyncCrossCrssvrTeamsTop __stub_SyncCrossCrssvrTeamsTop (RPC_SYNCCROSSCRSSVRTEAMSTOP, new SyncCrossCrssvrTeamsTopArg, new SyncCrossCrssvrTeamsTopRes);
static SyncCrossCrssvrTeamsEndBrief __stub_SyncCrossCrssvrTeamsEndBrief (RPC_SYNCCROSSCRSSVRTEAMSENDBRIEF, new SyncCrossCrssvrTeamsEndBriefArg, new SyncCrossCrssvrTeamsEndBriefRes);
static SyncCrossCrssvrTeams __stub_SyncCrossCrssvrTeams (RPC_SYNCCROSSCRSSVRTEAMS, new SyncCrossCrssvrTeamsArg, new SyncCrossCrssvrTeamsRes);
static SyncCrossCrssvrTeamsUpdate __stub_SyncCrossCrssvrTeamsUpdate (RPC_SYNCCROSSCRSSVRTEAMSUPDATE, new SyncCrossCrssvrTeamsUpdateArg, new SyncCrossCrssvrTeamsUpdateRes);
static SyncCrossCrssvrTeamsAdd __stub_SyncCrossCrssvrTeamsAdd (RPC_SYNCCROSSCRSSVRTEAMSADD, new SyncCrossCrssvrTeamsAddArg, new SyncCrossCrssvrTeamsAddRes);
static SyncCrossCrssvrTeamsDel __stub_SyncCrossCrssvrTeamsDel (RPC_SYNCCROSSCRSSVRTEAMSDEL, new SyncCrossCrssvrTeamsDelArg, new SyncCrossCrssvrTeamsDelRes);
static SyncCrossCrssvrTeamsRename __stub_SyncCrossCrssvrTeamsRename (RPC_SYNCCROSSCRSSVRTEAMSRENAME, new SyncCrossCrssvrTeamsRenameArg, new SyncCrossCrssvrTeamsRenameRes);
static SyncCrossCrssvrTeamsChgCaptain __stub_SyncCrossCrssvrTeamsChgCaptain (RPC_SYNCCROSSCRSSVRTEAMSCHGCAPTAIN, new SyncCrossCrssvrTeamsChgCaptainArg, new SyncCrossCrssvrTeamsChgCaptainRes);
static SyncCrossCrssvrTeamsDismiss __stub_SyncCrossCrssvrTeamsDismiss (RPC_SYNCCROSSCRSSVRTEAMSDISMISS, new SyncCrossCrssvrTeamsDismissArg, new SyncCrossCrssvrTeamsDismissRes);
static CrossCrssvrTeamsGetTopTable __stub_CrossCrssvrTeamsGetTopTable (RPC_CROSSCRSSVRTEAMSGETTOPTABLE, new CrossCrssvrTeamsGetTopTableArg, new CrossCrssvrTeamsGetTopTableRes);
static GateGetRoleRelation __stub_GateGetRoleRelation (RPC_GATEGETROLERELATION, new RoleId, new DBGateRoleRelationRes);
static FactionMultiExpSync __stub_FactionMultiExpSync((void*)0);
static SyncRoleEvent2SNS __stub_SyncRoleEvent2SNS((void*)0);
static GetFacActTopTable_Re __stub_GetFacActTopTable_Re((void*)0);
static FacBaseStopNotice __stub_FacBaseStopNotice((void*)0);
static CreateFacBase_Re __stub_CreateFacBase_Re((void*)0);
static FacBaseEnter_Re __stub_FacBaseEnter_Re((void*)0);
static StartFacBase __stub_StartFacBase((void*)0);
static StopFacBase __stub_StopFacBase((void*)0);
static FacMallSync __stub_FacMallSync((void*)0);
static GetFactionDynamic_Re __stub_GetFactionDynamic_Re((void*)0);
static GlobalDropRemainBroadCast __stub_GlobalDropRemainBroadCast((void*)0);
static GetKDCTokenToGate __stub_GetKDCTokenToGate((void*)0);
static RaidJoinReq __stub_RaidJoinReq((void*)0);
static RaidOpen_Re __stub_RaidOpen_Re((void*)0);
static RaidJoin_Re __stub_RaidJoin_Re((void*)0);
static RaidJoinApplyList __stub_RaidJoinApplyList((void*)0);
static RaidJoinAccept_Re __stub_RaidJoinAccept_Re((void*)0);
static RaidEnter_Re __stub_RaidEnter_Re((void*)0);
static CrossRaidEnter_Re __stub_CrossRaidEnter_Re((void*)0);
static RaidGetList_Re __stub_RaidGetList_Re((void*)0);
static RaidStart_Re __stub_RaidStart_Re((void*)0);
static RaidAppoint_Re __stub_RaidAppoint_Re((void*)0);
static RaidAppointNotify __stub_RaidAppointNotify((void*)0);
static RaidStartNotify __stub_RaidStartNotify((void*)0);
static RaidKick_Re __stub_RaidKick_Re((void*)0);
static RaidKickNotify __stub_RaidKickNotify((void*)0);
static RaidQuit_Re __stub_RaidQuit_Re((void*)0);
static RaidGetRoom_Re __stub_RaidGetRoom_Re((void*)0);
static GCrssvrTeamsPostScore __stub_GCrssvrTeamsPostScore((void*)0);
static SendRaidStart __stub_SendRaidStart((void*)0);
static SendRaidEnter __stub_SendRaidEnter((void*)0);
static GRaidMemberChange __stub_GRaidMemberChange((void*)0);
static SendRaidClose __stub_SendRaidClose((void*)0);
static SendRaidKickout __stub_SendRaidKickout((void*)0);
static RaidOpenVote_Re __stub_RaidOpenVote_Re((void*)0);
static RaidBeginVotes __stub_RaidBeginVotes((void*)0);
static RaidVoteResult __stub_RaidVoteResult((void*)0);
static RaidBriefInfo __stub_RaidBriefInfo((void*)0);
static TeamRaidApply_Re __stub_TeamRaidApply_Re((void*)0);
static TeamRaidApplyNotice __stub_TeamRaidApplyNotice((void*)0);
static TeamRaidActiveApplyNotice __stub_TeamRaidActiveApplyNotice((void*)0);
static TeamRaidMappingSuccess __stub_TeamRaidMappingSuccess((void*)0);
static HideSeekMappingSuccess __stub_HideSeekMappingSuccess((void*)0);
static TeamRaidBeginChooseChar __stub_TeamRaidBeginChooseChar((void*)0);
static TeamRaidUpdateChar __stub_TeamRaidUpdateChar((void*)0);
static TeamRaidAckTimeout __stub_TeamRaidAckTimeout((void*)0);
static FactionPkRaidChallenge_Re __stub_FactionPkRaidChallenge_Re((void*)0);
static FactionPkRaidChallengeCancel_Re __stub_FactionPkRaidChallengeCancel_Re((void*)0);
static FactionPkRaidInvite __stub_FactionPkRaidInvite((void*)0);
static FactionPkRaidBegin __stub_FactionPkRaidBegin((void*)0);
static FactionPkRaidGetScore_Re __stub_FactionPkRaidGetScore_Re((void*)0);
static FactionPkRaidGetList_Re __stub_FactionPkRaidGetList_Re((void*)0);
static FactionPkRaidStopFight __stub_FactionPkRaidStopFight((void*)0);
static FactionWithDrawBonus_Re __stub_FactionWithDrawBonus_Re((void*)0);
static Open_Controler __stub_Open_Controler((void*)0);
static GOpenBanquetRoleDel __stub_GOpenBanquetRoleDel((void*)0);
static GOpenBanquetClose __stub_GOpenBanquetClose((void*)0);
static UpdateFriendGTStatus __stub_UpdateFriendGTStatus((void*)0);
static CircleList_Re __stub_CircleList_Re((void*)0);
static GetCircleBaseInfo_Re __stub_GetCircleBaseInfo_Re((void*)0);
static BonusExp __stub_BonusExp((void*)0);
static UpdateRoleCircle __stub_UpdateRoleCircle((void*)0);
static CircleNotifyLink __stub_CircleNotifyLink((void*)0);
static ConsignCancelPost_Re __stub_ConsignCancelPost_Re((void*)0);
static ConsignQuery_Re __stub_ConsignQuery_Re((void*)0);
static ConsignGetItem_Re __stub_ConsignGetItem_Re((void*)0);
static ConsignListAll_Re __stub_ConsignListAll_Re((void*)0);
static ConsignListRole_Re __stub_ConsignListRole_Re((void*)0);
static ConsignListLargeCategory_Re __stub_ConsignListLargeCategory_Re((void*)0);
static ConsignStartErr __stub_ConsignStartErr((void*)0);
static DBConsignSkipSN __stub_DBConsignSkipSN((void*)0);
static ConsignPlayer_Re __stub_ConsignPlayer_Re((void*)0);
static GateRoleLogin __stub_GateRoleLogin((void*)0);
static GateRoleCreate __stub_GateRoleCreate((void*)0);
static GateRoleLogout __stub_GateRoleLogout((void*)0);
static GateOnlineList __stub_GateOnlineList((void*)0);
static GateGetGTRoleStatus __stub_GateGetGTRoleStatus((void*)0);
static RoleGroupUpdate __stub_RoleGroupUpdate((void*)0);
static RoleFriendUpdate __stub_RoleFriendUpdate((void*)0);
static FactionMemberUpdate __stub_FactionMemberUpdate((void*)0);
static FactionInfoUpdate __stub_FactionInfoUpdate((void*)0);
static RemoveRole __stub_RemoveRole((void*)0);
static RemoveFaction __stub_RemoveFaction((void*)0);
static Post __stub_Post((void*)0);
static GamePostCancel __stub_GamePostCancel((void*)0);
static WebPostCancel_Re __stub_WebPostCancel_Re((void*)0);
static Shelf_Re __stub_Shelf_Re((void*)0);
static ShelfCancel_Re __stub_ShelfCancel_Re((void*)0);
static Sold_Re __stub_Sold_Re((void*)0);
static PostExpire_Re __stub_PostExpire_Re((void*)0);
static WebGetRoleList_Re __stub_WebGetRoleList_Re((void*)0);
static NewKeepAlive __stub_NewKeepAlive((void*)0);
static FriendCallbackInfo_Re __stub_FriendCallbackInfo_Re((void*)0);
static FriendCallbackMail_Re __stub_FriendCallbackMail_Re((void*)0);
static FriendCallbackAward_Re __stub_FriendCallbackAward_Re((void*)0);
static FriendCallbackSubscribe_Re __stub_FriendCallbackSubscribe_Re((void*)0);
static BonusItem __stub_BonusItem((void*)0);
static AnnounceZoneid __stub_AnnounceZoneid((void*)0);
static AnnounceZoneid2 __stub_AnnounceZoneid2((void*)0);
static AnnounceZoneid3 __stub_AnnounceZoneid3((void*)0);
static AnnounceChallengeAlgo __stub_AnnounceChallengeAlgo((void*)0);
static QueryUserPrivilege __stub_QueryUserPrivilege((void*)0);
static QueryUserPrivilege2 __stub_QueryUserPrivilege2((void*)0);
static QueryUserForbid __stub_QueryUserForbid((void*)0);
static QueryUserForbid2 __stub_QueryUserForbid2((void*)0);
static PlayerKickout __stub_PlayerKickout((void*)0);
static PlayerOffline __stub_PlayerOffline((void*)0);
static GetTaskData_Re __stub_GetTaskData_Re((void*)0);
static SetTaskData_Re __stub_SetTaskData_Re((void*)0);
static PlayerStatusAnnounce __stub_PlayerStatusAnnounce((void*)0);
static OnlineAnnounce __stub_OnlineAnnounce((void*)0);
static RoleList_Re __stub_RoleList_Re((void*)0);
static CreateRole_Re __stub_CreateRole_Re((void*)0);
static DeleteRole_Re __stub_DeleteRole_Re((void*)0);
static UndoDeleteRole_Re __stub_UndoDeleteRole_Re((void*)0);
static PlayerBaseInfo_Re __stub_PlayerBaseInfo_Re((void*)0);
static PlayerBaseInfo2_Re __stub_PlayerBaseInfo2_Re((void*)0);
static PlayerBaseInfoCRC_Re __stub_PlayerBaseInfoCRC_Re((void*)0);
static GetPlayerIDByName_Re __stub_GetPlayerIDByName_Re((void*)0);
static SetUIConfig_Re __stub_SetUIConfig_Re((void*)0);
static GetUIConfig_Re __stub_GetUIConfig_Re((void*)0);
static SetHelpStates_Re __stub_SetHelpStates_Re((void*)0);
static GetHelpStates_Re __stub_GetHelpStates_Re((void*)0);
static GetPlayerBriefInfo_Re __stub_GetPlayerBriefInfo_Re((void*)0);
static CommonDataSync __stub_CommonDataSync((void*)0);
static NotifyDailyTableChange __stub_NotifyDailyTableChange((void*)0);
static AccountingRequest __stub_AccountingRequest((void*)0);
static ChatBroadCast __stub_ChatBroadCast((void*)0);
static WorldChat __stub_WorldChat((void*)0);
static ChatMultiCast __stub_ChatMultiCast((void*)0);
static ChatSingleCast __stub_ChatSingleCast((void*)0);
static RoleStatusAnnounce __stub_RoleStatusAnnounce((void*)0);
static AddFriend_Re __stub_AddFriend_Re((void*)0);
static GetFriends_Re __stub_GetFriends_Re((void*)0);
static GetEnemies_Re __stub_GetEnemies_Re((void*)0);
static UpdateEnemy_Re __stub_UpdateEnemy_Re((void*)0);
static SetGroupName_Re __stub_SetGroupName_Re((void*)0);
static SetFriendGroup_Re __stub_SetFriendGroup_Re((void*)0);
static GetSavedMsg_Re __stub_GetSavedMsg_Re((void*)0);
static ChatRoomCreate_Re __stub_ChatRoomCreate_Re((void*)0);
static ChatRoomInvite_Re __stub_ChatRoomInvite_Re((void*)0);
static ChatRoomJoin_Re __stub_ChatRoomJoin_Re((void*)0);
static ChatRoomList_Re __stub_ChatRoomList_Re((void*)0);
static TradeStart_Re __stub_TradeStart_Re((void*)0);
static TradeAddGoods_Re __stub_TradeAddGoods_Re((void*)0);
static TradeRemoveGoods_Re __stub_TradeRemoveGoods_Re((void*)0);
static TradeSubmit_Re __stub_TradeSubmit_Re((void*)0);
static TradeMoveObj_Re __stub_TradeMoveObj_Re((void*)0);
static TradeConfirm_Re __stub_TradeConfirm_Re((void*)0);
static TradeDiscard_Re __stub_TradeDiscard_Re((void*)0);
static TradeEnd __stub_TradeEnd((void*)0);
static GTradeStart __stub_GTradeStart((void*)0);
static GTradeEnd __stub_GTradeEnd((void*)0);
static CertRequest __stub_CertRequest((void*)0);
static CertAnswer __stub_CertAnswer((void*)0);
static CertKey __stub_CertKey((void*)0);
static CertFinish __stub_CertFinish((void*)0);
static Game2AU __stub_Game2AU((void*)0);
static RoleLogin __stub_RoleLogin((void*)0);
static AnnounceGM __stub_AnnounceGM((void*)0);
static GMOnlineNum_Re __stub_GMOnlineNum_Re((void*)0);
static GMListOnlineUser_Re __stub_GMListOnlineUser_Re((void*)0);
static GMKickoutUser_Re __stub_GMKickoutUser_Re((void*)0);
static GMForbidSellPoint_Re __stub_GMForbidSellPoint_Re((void*)0);
static GMKickoutRole_Re __stub_GMKickoutRole_Re((void*)0);
static GMShutup_Re __stub_GMShutup_Re((void*)0);
static GMShutupRole_Re __stub_GMShutupRole_Re((void*)0);
static GMToggleChat_Re __stub_GMToggleChat_Re((void*)0);
static GMForbidRole_Re __stub_GMForbidRole_Re((void*)0);
static GMShutdownLine_Re __stub_GMShutdownLine_Re((void*)0);
static Report2GM_Re __stub_Report2GM_Re((void*)0);
static Complain2GM_Re __stub_Complain2GM_Re((void*)0);
static AnnounceForbidInfo __stub_AnnounceForbidInfo((void*)0);
static SetMaxOnlineNum_Re __stub_SetMaxOnlineNum_Re((void*)0);
static ACWhoAmI __stub_ACWhoAmI((void*)0);
static ACStatusAnnounce __stub_ACStatusAnnounce((void*)0);
static ACStatusAnnounce2 __stub_ACStatusAnnounce2((void*)0);
static AnnounceNewMail __stub_AnnounceNewMail((void*)0);
static GetMailList_Re __stub_GetMailList_Re((void*)0);
static GetMail_Re __stub_GetMail_Re((void*)0);
static GetMailAttachObj_Re __stub_GetMailAttachObj_Re((void*)0);
static DeleteMail_Re __stub_DeleteMail_Re((void*)0);
static PreserveMail_Re __stub_PreserveMail_Re((void*)0);
static PlayerSendMail_Re __stub_PlayerSendMail_Re((void*)0);
static GMailEndSync __stub_GMailEndSync((void*)0);
static SysSendMail_Re __stub_SysSendMail_Re((void*)0);
static SysRecoveredObjMail_Re __stub_SysRecoveredObjMail_Re((void*)0);
static AuctionOpen_Re __stub_AuctionOpen_Re((void*)0);
static AuctionBid_Re __stub_AuctionBid_Re((void*)0);
static AuctionClose_Re __stub_AuctionClose_Re((void*)0);
static AuctionList_Re __stub_AuctionList_Re((void*)0);
static AuctionGet_Re __stub_AuctionGet_Re((void*)0);
static AuctionGetItem_Re __stub_AuctionGetItem_Re((void*)0);
static AuctionAttendList_Re __stub_AuctionAttendList_Re((void*)0);
static AuctionExitBid_Re __stub_AuctionExitBid_Re((void*)0);
static BattleGetMap_Re __stub_BattleGetMap_Re((void*)0);
static BattleGetList_Re __stub_BattleGetList_Re((void*)0);
static BattleGetField_Re __stub_BattleGetField_Re((void*)0);
static BattleJoin_Re __stub_BattleJoin_Re((void*)0);
static BattleLeave_Re __stub_BattleLeave_Re((void*)0);
static BattleEnter_Re __stub_BattleEnter_Re((void*)0);
static BattleStart_Re __stub_BattleStart_Re((void*)0);
static BattleStart __stub_BattleStart((void*)0);
static BattleStartNotify __stub_BattleStartNotify((void*)0);
static SendBattleEnter __stub_SendBattleEnter((void*)0);
static VIPInfoNotify __stub_VIPInfoNotify((void*)0);
static SendInstancingStart __stub_SendInstancingStart((void*)0);
static SendInstancingEnter __stub_SendInstancingEnter((void*)0);
static SendInstancingJoin __stub_SendInstancingJoin((void*)0);
static InstancingGetList_Re __stub_InstancingGetList_Re((void*)0);
static InstancingGetField_Re __stub_InstancingGetField_Re((void*)0);
static InstancingKick_Re __stub_InstancingKick_Re((void*)0);
static InstancingStart_Re __stub_InstancingStart_Re((void*)0);
static InstancingAvailableList_Re __stub_InstancingAvailableList_Re((void*)0);
static InstancingJoinReq __stub_InstancingJoinReq((void*)0);
static InstancingJoin_Re __stub_InstancingJoin_Re((void*)0);
static HometownExchgMoney_Re __stub_HometownExchgMoney_Re((void*)0);
static InstancingLeave_Re __stub_InstancingLeave_Re((void*)0);
static InstancingAppoint_Re __stub_InstancingAppoint_Re((void*)0);
static InstancingAccept_Re __stub_InstancingAccept_Re((void*)0);
static InstancingAppointNotify __stub_InstancingAppointNotify((void*)0);
static InstancingKickNotify __stub_InstancingKickNotify((void*)0);
static InstancingEnter_Re __stub_InstancingEnter_Re((void*)0);
static InstancingStartNotify __stub_InstancingStartNotify((void*)0);
static InstancingCloseNotify __stub_InstancingCloseNotify((void*)0);
static QueryRewardType __stub_QueryRewardType((void*)0);
static QueryRewardType_Re __stub_QueryRewardType_Re((void*)0);
static QueryGameServerAttr_Re __stub_QueryGameServerAttr_Re((void*)0);
static AnnounceServerAttribute __stub_AnnounceServerAttribute((void*)0);
static CrssvrTeamsRoleNotify_Re __stub_CrssvrTeamsRoleNotify_Re((void*)0);
static CrssvrTeamsTeamBase_Re __stub_CrssvrTeamsTeamBase_Re((void*)0);
static CrssvrTeamsRoleList_Re __stub_CrssvrTeamsRoleList_Re((void*)0);
static CrssvrTeamsNotify_Re __stub_CrssvrTeamsNotify_Re((void*)0);
static CrssvrTeamsSearch_Re __stub_CrssvrTeamsSearch_Re((void*)0);
static FactionAppoint_Re __stub_FactionAppoint_Re((void*)0);
static FactionAnnounce_Re __stub_FactionAnnounce_Re((void*)0);
static FactionCreate_Re __stub_FactionCreate_Re((void*)0);
static FamilyRecord __stub_FamilyRecord((void*)0);
static FactionDismiss_Re __stub_FactionDismiss_Re((void*)0);
static FactionExpel_Re __stub_FactionExpel_Re((void*)0);
static FactionList_Re __stub_FactionList_Re((void*)0);
static FactionLeave_Re __stub_FactionLeave_Re((void*)0);
static FactionNickname_Re __stub_FactionNickname_Re((void*)0);
static FactionResign_Re __stub_FactionResign_Re((void*)0);
static FactionRecruit_Re __stub_FactionRecruit_Re((void*)0);
static FactionUpgrade_Re __stub_FactionUpgrade_Re((void*)0);
static GetTopTable_Re __stub_GetTopTable_Re((void*)0);
static SectList_Re __stub_SectList_Re((void*)0);
static SectRecruit_Re __stub_SectRecruit_Re((void*)0);
static SectExpel_Re __stub_SectExpel_Re((void*)0);
static SectExpel __stub_SectExpel((void*)0);
static SectQuit __stub_SectQuit((void*)0);
static GetFactionBaseInfo_Re __stub_GetFactionBaseInfo_Re((void*)0);
static GetFactionHostile_Re __stub_GetFactionHostile_Re((void*)0);
static PlayerFactionInfo_Re __stub_PlayerFactionInfo_Re((void*)0);
static SyncFactionLevel __stub_SyncFactionLevel((void*)0);
static SyncPlayerFaction __stub_SyncPlayerFaction((void*)0);
static HostileAdd_Re __stub_HostileAdd_Re((void*)0);
static HostileDelete_Re __stub_HostileDelete_Re((void*)0);
static HostileProtect_Re __stub_HostileProtect_Re((void*)0);
static SyncFactionHostiles __stub_SyncFactionHostiles((void*)0);
static OnDivorce __stub_OnDivorce((void*)0);
static StockCommission_Re __stub_StockCommission_Re((void*)0);
static StockAccount_Re __stub_StockAccount_Re((void*)0);
static StockTransaction_Re __stub_StockTransaction_Re((void*)0);
static StockBill_Re __stub_StockBill_Re((void*)0);
static StockCancel_Re __stub_StockCancel_Re((void*)0);
static CombatChallenge_Re __stub_CombatChallenge_Re((void*)0);
static CombatControl __stub_CombatControl((void*)0);
static CombatStatus __stub_CombatStatus((void*)0);
static CombatTop_Re __stub_CombatTop_Re((void*)0);
static CashLock_Re __stub_CashLock_Re((void*)0);
static CashPasswordSet_Re __stub_CashPasswordSet_Re((void*)0);
static SyncFamilyData __stub_SyncFamilyData((void*)0);
static NetBarReward __stub_NetBarReward((void*)0);
static SendAsyncData __stub_SendAsyncData((void*)0);
static DomainCmd_Re __stub_DomainCmd_Re((void*)0);
static AutolockSet_Re __stub_AutolockSet_Re((void*)0);
static SendSiegeStart __stub_SendSiegeStart((void*)0);
static GSiegeStatus __stub_GSiegeStatus((void*)0);
static SiegeChallenge_Re __stub_SiegeChallenge_Re((void*)0);
static SiegeSetAssistant_Re __stub_SiegeSetAssistant_Re((void*)0);
static SiegeInfoGet_Re __stub_SiegeInfoGet_Re((void*)0);
static SiegeEnter_Re __stub_SiegeEnter_Re((void*)0);
static SiegeTop_Re __stub_SiegeTop_Re((void*)0);
static ForwardChat __stub_ForwardChat((void*)0);
static ContestInviteBroadcast __stub_ContestInviteBroadcast((void*)0);
static ContestInvite __stub_ContestInvite((void*)0);
static ContestBegin __stub_ContestBegin((void*)0);
static ContestQuestion __stub_ContestQuestion((void*)0);
static ContestAnswer_Re __stub_ContestAnswer_Re((void*)0);
static ContestPlaceNotify __stub_ContestPlaceNotify((void*)0);
static ContestEnd __stub_ContestEnd((void*)0);
static ContestResult __stub_ContestResult((void*)0);
static S2CFunGamedataSend __stub_S2CFunGamedataSend((void*)0);
static S2CHometowndataSend __stub_S2CHometowndataSend((void*)0);
static SendSNSResult __stub_SendSNSResult((void*)0);
static SNSSetPlayerInfo_Re __stub_SNSSetPlayerInfo_Re((void*)0);
static SNSPressMessage_Re __stub_SNSPressMessage_Re((void*)0);
static SNSCancelMessage_Re __stub_SNSCancelMessage_Re((void*)0);
static SNSCancelLeaveMessage_Re __stub_SNSCancelLeaveMessage_Re((void*)0);
static SNSApply_Re __stub_SNSApply_Re((void*)0);
static SNSApplyNotify __stub_SNSApplyNotify((void*)0);
static SNSListMessage_Re __stub_SNSListMessage_Re((void*)0);
static SNSGetMessage_Re __stub_SNSGetMessage_Re((void*)0);
static SNSGetPlayerInfo_Re __stub_SNSGetPlayerInfo_Re((void*)0);
static SNSVote_Re __stub_SNSVote_Re((void*)0);
static SNSAcceptApply_Re __stub_SNSAcceptApply_Re((void*)0);
static RefListReferrals_Re __stub_RefListReferrals_Re((void*)0);
static RefWithdrawExp_Re __stub_RefWithdrawExp_Re((void*)0);
static RefWithdrawBonus_Re __stub_RefWithdrawBonus_Re((void*)0);
static SendRefAddExp __stub_SendRefAddExp((void*)0);
static SendRefAddBonus __stub_SendRefAddBonus((void*)0);
static RefGetReferenceCode_Re __stub_RefGetReferenceCode_Re((void*)0);
static SendChangeProfile_Re __stub_SendChangeProfile_Re((void*)0);
static RepairRoleList __stub_RepairRoleList((void*)0);
static GShopNotifyScheme __stub_GShopNotifyScheme((void*)0);
static NetMarbleLogout __stub_NetMarbleLogout((void*)0);
static TerritoryMapGet_Re __stub_TerritoryMapGet_Re((void*)0);
static TerritoryChallenge_Re __stub_TerritoryChallenge_Re((void*)0);
static TerritoryEnter_Re __stub_TerritoryEnter_Re((void*)0);
static TerritoryLeave_Re __stub_TerritoryLeave_Re((void*)0);
static SendTerritoryStart __stub_SendTerritoryStart((void*)0);
static SyncTerritoryList __stub_SyncTerritoryList((void*)0);
static TerritoryEnterRemind __stub_TerritoryEnterRemind((void*)0);
static TerritoryScoreUpdate __stub_TerritoryScoreUpdate((void*)0);
static UniqueBidHistory_Re __stub_UniqueBidHistory_Re((void*)0);
static UniqueBid_Re __stub_UniqueBid_Re((void*)0);
static UniqueGetItem_Re __stub_UniqueGetItem_Re((void*)0);
static PlayerChangeGS __stub_PlayerChangeGS((void*)0);
static PlayerChangeDS __stub_PlayerChangeDS((void*)0);
static ChangeDS_Re __stub_ChangeDS_Re((void*)0);
static TryChangeGS_Re __stub_TryChangeGS_Re((void*)0);
static DoChangeGS_Re __stub_DoChangeGS_Re((void*)0);
static GConsignEnd __stub_GConsignEnd((void*)0);
static PlayerConsignOffline __stub_PlayerConsignOffline((void*)0);
static GConsignEndRole __stub_GConsignEndRole((void*)0);
static DisableAutolock __stub_DisableAutolock((void*)0);
static FastPay_Re __stub_FastPay_Re((void*)0);
static FastPayBindInfo __stub_FastPayBindInfo((void*)0);
static GChangeRolename_Re __stub_GChangeRolename_Re((void*)0);
static GChangeFactionName_Re __stub_GChangeFactionName_Re((void*)0);
static QueryRolenameHistory_Re __stub_QueryRolenameHistory_Re((void*)0);
static ChangeRolename_Re __stub_ChangeRolename_Re((void*)0);
static ChangeFactionName_Re __stub_ChangeFactionName_Re((void*)0);
static FriendNameChange __stub_FriendNameChange((void*)0);
static KingdomBattleStart __stub_KingdomBattleStart((void*)0);
static KingdomBattleStop __stub_KingdomBattleStop((void*)0);
static KingdomInfoSync __stub_KingdomInfoSync((void*)0);
static KingdomTitleSync __stub_KingdomTitleSync((void*)0);
static KingdomPointSync __stub_KingdomPointSync((void*)0);
static KingdomBattleEnter_Re __stub_KingdomBattleEnter_Re((void*)0);
static KingdomBattleLeave_Re __stub_KingdomBattleLeave_Re((void*)0);
static KingdomAnnounce_Re __stub_KingdomAnnounce_Re((void*)0);
static KingdomAppoint_Re __stub_KingdomAppoint_Re((void*)0);
static KingdomDischarge_Re __stub_KingdomDischarge_Re((void*)0);
static KingdomSetGameAttri_Re __stub_KingdomSetGameAttri_Re((void*)0);
static KingdomGetInfo_Re __stub_KingdomGetInfo_Re((void*)0);
static KingdomGetTitle_Re __stub_KingdomGetTitle_Re((void*)0);
static KingdomTitleChange __stub_KingdomTitleChange((void*)0);
static KingdomPointChange __stub_KingdomPointChange((void*)0);
static KingdomKingChange __stub_KingdomKingChange((void*)0);
static KingCallGuards_Re __stub_KingCallGuards_Re((void*)0);
static KingCallGuard __stub_KingCallGuard((void*)0);
static QueenOpenBath __stub_QueenOpenBath((void*)0);
static QueenCloseBath __stub_QueenCloseBath((void*)0);
static QueenOpenBath_Re __stub_QueenOpenBath_Re((void*)0);
static KingIssueTask_Re __stub_KingIssueTask_Re((void*)0);
static GKingIssueTask_Re __stub_GKingIssueTask_Re((void*)0);
static GKingGetReward_Re __stub_GKingGetReward_Re((void*)0);
static GetKingTask_Re __stub_GetKingTask_Re((void*)0);
static KingGetReward_Re __stub_KingGetReward_Re((void*)0);
static TopFlower_Re __stub_TopFlower_Re((void*)0);
static TopFlower_Err __stub_TopFlower_Err((void*)0);
static TopFlowerMsg_Re __stub_TopFlowerMsg_Re((void*)0);
static OpenBanquetJoin_Re __stub_OpenBanquetJoin_Re((void*)0);
static OpenBanquetNotify_Re __stub_OpenBanquetNotify_Re((void*)0);
static OpenBanquetDrag_Re __stub_OpenBanquetDrag_Re((void*)0);
static OpenBanquetQueueCount_Re __stub_OpenBanquetQueueCount_Re((void*)0);
static OpenBanquetEnter_Re __stub_OpenBanquetEnter_Re((void*)0);
static OpenBanquetGetField_Re __stub_OpenBanquetGetField_Re((void*)0);
static GTouchPointExchange_Re __stub_GTouchPointExchange_Re((void*)0);
static GetTouchPoint_Re __stub_GetTouchPoint_Re((void*)0);
static GetPassportCashAdd_Re __stub_GetPassportCashAdd_Re((void*)0);
static TouchPointExchange_Re __stub_TouchPointExchange_Re((void*)0);
static ForbidService __stub_ForbidService((void*)0);
static SysSendMail4_Re __stub_SysSendMail4_Re((void*)0);
static WebOrderNotice __stub_WebOrderNotice((void*)0);
static TryReconnect_Re __stub_TryReconnect_Re((void*)0);
static PlayerReconnect __stub_PlayerReconnect((void*)0);
static FacBaseDataSend __stub_FacBaseDataSend((void*)0);
static CreateFacBase __stub_CreateFacBase((void*)0);
static GetFacActTopTable __stub_GetFacActTopTable((void*)0);
static GetFactionDynamic __stub_GetFactionDynamic((void*)0);
static GetKDCToken __stub_GetKDCToken((void*)0);
static RaidStart __stub_RaidStart((void*)0);
static RaidEnter __stub_RaidEnter((void*)0);
static RaidQuit __stub_RaidQuit((void*)0);
static TeamRaidQuit __stub_TeamRaidQuit((void*)0);
static HideSeekRaidQuit __stub_HideSeekRaidQuit((void*)0);
static RaidGetList __stub_RaidGetList((void*)0);
static RaidAppoint __stub_RaidAppoint((void*)0);
static RaidKick __stub_RaidKick((void*)0);
static RaidGetRoom __stub_RaidGetRoom((void*)0);
static RaidJoinAccept __stub_RaidJoinAccept((void*)0);
static RaidOpenVote __stub_RaidOpenVote((void*)0);
static RaidVotes __stub_RaidVotes((void*)0);
static TeamRaidMappingSuccess_Re __stub_TeamRaidMappingSuccess_Re((void*)0);
static HideSeekMappingSuccess_Re __stub_HideSeekMappingSuccess_Re((void*)0);
static FactionPkRaidChallenge __stub_FactionPkRaidChallenge((void*)0);
static FactionPkRaidChallengeCancel __stub_FactionPkRaidChallengeCancel((void*)0);
static FactionPkRaidInvite_Re __stub_FactionPkRaidInvite_Re((void*)0);
static FactionPkRaidGetScore __stub_FactionPkRaidGetScore((void*)0);
static FactionPkRaidGetList __stub_FactionPkRaidGetList((void*)0);
static FactionWithDrawBonus __stub_FactionWithDrawBonus((void*)0);
static GetFriendGTStatus __stub_GetFriendGTStatus((void*)0);
static GCircleChat __stub_GCircleChat((void*)0);
static CircleList __stub_CircleList((void*)0);
static GetCircleBaseInfo __stub_GetCircleBaseInfo((void*)0);
static ClaimCircleGradBonus __stub_ClaimCircleGradBonus((void*)0);
static ConsignCancelPost __stub_ConsignCancelPost((void*)0);
static ConsignQuery __stub_ConsignQuery((void*)0);
static ConsignGetItem __stub_ConsignGetItem((void*)0);
static ConsignListAll __stub_ConsignListAll((void*)0);
static ConsignListRole __stub_ConsignListRole((void*)0);
static ConsignListLargeCategory __stub_ConsignListLargeCategory((void*)0);
static PlayerLogin __stub_PlayerLogin((void*)0);
static PlayerStatusSync __stub_PlayerStatusSync((void*)0);
static EnterWorld __stub_EnterWorld((void*)0);
static StatusAnnounce __stub_StatusAnnounce((void*)0);
static RoleList __stub_RoleList((void*)0);
static CreateRole __stub_CreateRole((void*)0);
static DeleteRole __stub_DeleteRole((void*)0);
static UndoDeleteRole __stub_UndoDeleteRole((void*)0);
static PlayerBaseInfo __stub_PlayerBaseInfo((void*)0);
static PlayerBaseInfo2 __stub_PlayerBaseInfo2((void*)0);
static PlayerBaseInfoCRC __stub_PlayerBaseInfoCRC((void*)0);
static GetPlayerIDByName __stub_GetPlayerIDByName((void*)0);
static SetUIConfig __stub_SetUIConfig((void*)0);
static GetUIConfig __stub_GetUIConfig((void*)0);
static SetHelpStates __stub_SetHelpStates((void*)0);
static GetHelpStates __stub_GetHelpStates((void*)0);
static GetPlayerBriefInfo __stub_GetPlayerBriefInfo((void*)0);
static BattleFlagBuffScope __stub_BattleFlagBuffScope((void*)0);
static GetCouponsReq __stub_GetCouponsReq((void*)0);
static PublicChat __stub_PublicChat((void*)0);
static PrivateChat __stub_PrivateChat((void*)0);
static AddFriend __stub_AddFriend((void*)0);
static GetFriends __stub_GetFriends((void*)0);
static SetGroupName __stub_SetGroupName((void*)0);
static SetFriendGroup __stub_SetFriendGroup((void*)0);
static DelFriend __stub_DelFriend((void*)0);
static DelFriend_Re __stub_DelFriend_Re((void*)0);
static FriendStatus __stub_FriendStatus((void*)0);
static GetEnemies __stub_GetEnemies((void*)0);
static UpdateEnemy __stub_UpdateEnemy((void*)0);
static GetSavedMsg __stub_GetSavedMsg((void*)0);
static ChatRoomCreate __stub_ChatRoomCreate((void*)0);
static ChatRoomInvite __stub_ChatRoomInvite((void*)0);
static ChatRoomJoin __stub_ChatRoomJoin((void*)0);
static ChatRoomLeave __stub_ChatRoomLeave((void*)0);
static ChatRoomExpel __stub_ChatRoomExpel((void*)0);
static ChatRoomSpeak __stub_ChatRoomSpeak((void*)0);
static ChatRoomList __stub_ChatRoomList((void*)0);
static FriendCallbackInfo __stub_FriendCallbackInfo((void*)0);
static FriendCallbackMail __stub_FriendCallbackMail((void*)0);
static FriendCallbackAward __stub_FriendCallbackAward((void*)0);
static FriendCallbackSubscribe __stub_FriendCallbackSubscribe((void*)0);
static TradeStart __stub_TradeStart((void*)0);
static TradeAddGoods __stub_TradeAddGoods((void*)0);
static TradeRemoveGoods __stub_TradeRemoveGoods((void*)0);
static TradeSubmit __stub_TradeSubmit((void*)0);
static TradeMoveObj __stub_TradeMoveObj((void*)0);
static TradeConfirm __stub_TradeConfirm((void*)0);
static TradeDiscard __stub_TradeDiscard((void*)0);
static MatrixFailure __stub_MatrixFailure((void*)0);
static TryReconnect __stub_TryReconnect((void*)0);
static GMRestartServer __stub_GMRestartServer((void*)0);
static GMShutdownLine __stub_GMShutdownLine((void*)0);
static GMOnlineNum __stub_GMOnlineNum((void*)0);
static GMListOnlineUser __stub_GMListOnlineUser((void*)0);
static GMKickoutUser __stub_GMKickoutUser((void*)0);
static GMForbidSellPoint __stub_GMForbidSellPoint((void*)0);
static GMKickoutRole __stub_GMKickoutRole((void*)0);
static GMShutup __stub_GMShutup((void*)0);
static GMShutupRole __stub_GMShutupRole((void*)0);
static GMToggleChat __stub_GMToggleChat((void*)0);
static GMForbidRole __stub_GMForbidRole((void*)0);
static GMPrivilegeChange __stub_GMPrivilegeChange((void*)0);
static Report2GM __stub_Report2GM((void*)0);
static Complain2GM __stub_Complain2GM((void*)0);
static AnnounceLinkType __stub_AnnounceLinkType((void*)0);
static SetMaxOnlineNum __stub_SetMaxOnlineNum((void*)0);
static VerifyMaster __stub_VerifyMaster((void*)0);
static VerifyMaster_Re __stub_VerifyMaster_Re((void*)0);
static ACReport __stub_ACReport((void*)0);
static ACAnswer __stub_ACAnswer((void*)0);
static ACProtoStat __stub_ACProtoStat((void*)0);
static ReportIP __stub_ReportIP((void*)0);
static ACAccuse __stub_ACAccuse((void*)0);
static CheckNewMail __stub_CheckNewMail((void*)0);
static SysSendMail __stub_SysSendMail((void*)0);
static SysRecoveredObjMail __stub_SysRecoveredObjMail((void*)0);
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
static AuctionAttendList __stub_AuctionAttendList((void*)0);
static AuctionExitBid __stub_AuctionExitBid((void*)0);
static InstancingGetList __stub_InstancingGetList((void*)0);
static InstancingGetField __stub_InstancingGetField((void*)0);
static InstancingKick __stub_InstancingKick((void*)0);
static InstancingStart __stub_InstancingStart((void*)0);
static InstancingAvailableList __stub_InstancingAvailableList((void*)0);
static InstancingAccept __stub_InstancingAccept((void*)0);
static InstancingLeave __stub_InstancingLeave((void*)0);
static InstancingAppoint __stub_InstancingAppoint((void*)0);
static InstancingEnter __stub_InstancingEnter((void*)0);
static BattleGetMap __stub_BattleGetMap((void*)0);
static BattleGetList __stub_BattleGetList((void*)0);
static BattleGetField __stub_BattleGetField((void*)0);
static BattleJoin __stub_BattleJoin((void*)0);
static BattleEnter __stub_BattleEnter((void*)0);
static BattleLeave __stub_BattleLeave((void*)0);
static CrssvrTeamsGetRoleNotify __stub_CrssvrTeamsGetRoleNotify((void*)0);
static CrssvrTeamsGetTeamBase __stub_CrssvrTeamsGetTeamBase((void*)0);
static CrssvrTeamsGetRoleList __stub_CrssvrTeamsGetRoleList((void*)0);
static CrssvrTeamsRename __stub_CrssvrTeamsRename((void*)0);
static CrssvrTeamsDismiss __stub_CrssvrTeamsDismiss((void*)0);
static CrssvrTeamsRecruit __stub_CrssvrTeamsRecruit((void*)0);
static CrssvrTeamsChgCaptain __stub_CrssvrTeamsChgCaptain((void*)0);
static CrssvrTeamsLeave __stub_CrssvrTeamsLeave((void*)0);
static CrssvrTeamsKick __stub_CrssvrTeamsKick((void*)0);
static CrssvrTeamsSearch __stub_CrssvrTeamsSearch((void*)0);
static FactionChat __stub_FactionChat((void*)0);
static FactionCreate __stub_FactionCreate((void*)0);
static FactionList __stub_FactionList((void*)0);
static FactionDismiss __stub_FactionDismiss((void*)0);
static FactionRecruit __stub_FactionRecruit((void*)0);
static FactionExpel __stub_FactionExpel((void*)0);
static FactionAnnounce __stub_FactionAnnounce((void*)0);
static FactionAppoint __stub_FactionAppoint((void*)0);
static FactionResign __stub_FactionResign((void*)0);
static FactionLeave __stub_FactionLeave((void*)0);
static FactionUpgrade __stub_FactionUpgrade((void*)0);
static FactionNickname __stub_FactionNickname((void*)0);
static GetFactionBaseInfo __stub_GetFactionBaseInfo((void*)0);
static GetFactionHostile __stub_GetFactionHostile((void*)0);
static HostileDelete __stub_HostileDelete((void*)0);
static HostileAdd __stub_HostileAdd((void*)0);
static HostileProtect __stub_HostileProtect((void*)0);
static SectRecruit __stub_SectRecruit((void*)0);
static SectList __stub_SectList((void*)0);
static GetTopTable __stub_GetTopTable((void*)0);
static CombatChallenge __stub_CombatChallenge((void*)0);
static CombatTop __stub_CombatTop((void*)0);
static CashLock __stub_CashLock((void*)0);
static CashPasswordSet __stub_CashPasswordSet((void*)0);
static DomainLogin __stub_DomainLogin((void*)0);
static DomainValidate __stub_DomainValidate((void*)0);
static DomainCmd __stub_DomainCmd((void*)0);
static AutolockSet __stub_AutolockSet((void*)0);
static SiegeInfoGet __stub_SiegeInfoGet((void*)0);
static SiegeEnter __stub_SiegeEnter((void*)0);
static SiegeTop __stub_SiegeTop((void*)0);
static ContestInvite_Re __stub_ContestInvite_Re((void*)0);
static ContestAnswer __stub_ContestAnswer((void*)0);
static ContestExit __stub_ContestExit((void*)0);
static C2SFunGamedataSend __stub_C2SFunGamedataSend((void*)0);
static C2SHometowndataSend __stub_C2SHometowndataSend((void*)0);
static SNSSetPlayerInfo __stub_SNSSetPlayerInfo((void*)0);
static SNSGetPlayerInfo __stub_SNSGetPlayerInfo((void*)0);
static SNSCancelMessage __stub_SNSCancelMessage((void*)0);
static SNSCancelLeaveMessage __stub_SNSCancelLeaveMessage((void*)0);
static SNSListMessage __stub_SNSListMessage((void*)0);
static SNSGetMessage __stub_SNSGetMessage((void*)0);
static SNSAcceptApply __stub_SNSAcceptApply((void*)0);
static RefListReferrals __stub_RefListReferrals((void*)0);
static RefWithdrawExp __stub_RefWithdrawExp((void*)0);
static RefWithdrawBonus __stub_RefWithdrawBonus((void*)0);
static RefGetReferenceCode __stub_RefGetReferenceCode((void*)0);
static GShopSetSaleScheme __stub_GShopSetSaleScheme((void*)0);
static GShopSetDiscountScheme __stub_GShopSetDiscountScheme((void*)0);
static TerritoryMapGet __stub_TerritoryMapGet((void*)0);
static UniqueBidHistory __stub_UniqueBidHistory((void*)0);
static TryChangeGS __stub_TryChangeGS((void*)0);
static DoChangeGS __stub_DoChangeGS((void*)0);
static SSOGetTicketReq __stub_SSOGetTicketReq((void*)0);
static FastPay __stub_FastPay((void*)0);
static QueryRolenameHistory __stub_QueryRolenameHistory((void*)0);
static KingdomAnnounce __stub_KingdomAnnounce((void*)0);
static KingdomAppoint __stub_KingdomAppoint((void*)0);
static KingdomDischarge __stub_KingdomDischarge((void*)0);
static KingdomSetGameAttri __stub_KingdomSetGameAttri((void*)0);
static KingdomGetInfo __stub_KingdomGetInfo((void*)0);
static KingdomGetTitle __stub_KingdomGetTitle((void*)0);
static QueenTryOpenBath __stub_QueenTryOpenBath((void*)0);
static GetKingTask __stub_GetKingTask((void*)0);
static SendFlowerReq __stub_SendFlowerReq((void*)0);
static TopFlowerReq __stub_TopFlowerReq((void*)0);
static TopFlowerGetGift __stub_TopFlowerGetGift((void*)0);
static OpenBanquetCancel __stub_OpenBanquetCancel((void*)0);
static OpenBanquetQueueCount __stub_OpenBanquetQueueCount((void*)0);
static OpenBanquetEnter __stub_OpenBanquetEnter((void*)0);
static OpenBanquetGetField __stub_OpenBanquetGetField((void*)0);
static GetTouchPoint __stub_GetTouchPoint((void*)0);
static GetPassportCashAdd __stub_GetPassportCashAdd((void*)0);
static KeyExchange __stub_KeyExchange((void*)0);
static KickoutUser __stub_KickoutUser((void*)0);
static AccountingResponse __stub_AccountingResponse((void*)0);
static QueryUserPrivilege_Re __stub_QueryUserPrivilege_Re((void*)0);
static QueryUserForbid_Re __stub_QueryUserForbid_Re((void*)0);
static UpdateRemainTime __stub_UpdateRemainTime((void*)0);
static TransBuyPoint_Re __stub_TransBuyPoint_Re((void*)0);
static AddCash __stub_AddCash((void*)0);
static AddCash_Re __stub_AddCash_Re((void*)0);
static AddictionControl __stub_AddictionControl((void*)0);
static VIPAnnounce __stub_VIPAnnounce((void*)0);
static NetBarAnnounce __stub_NetBarAnnounce((void*)0);
static BillingRequest __stub_BillingRequest((void*)0);
static BillingBalance __stub_BillingBalance((void*)0);
static AuthdVersion __stub_AuthdVersion((void*)0);
static SSOGetTicketRep __stub_SSOGetTicketRep((void*)0);
static DiscountAnnounce __stub_DiscountAnnounce((void*)0);
static AU2Game __stub_AU2Game((void*)0);
static SysSendMail4 __stub_SysSendMail4((void*)0);
static SyncPlayerFacCouponAdd __stub_SyncPlayerFacCouponAdd((void*)0);
static StartFactionMultiExp __stub_StartFactionMultiExp((void*)0);
static FacBaseDataBroadcast __stub_FacBaseDataBroadcast((void*)0);
static FacBaseDataDeliver __stub_FacBaseDataDeliver((void*)0);
static FacMallChange __stub_FacMallChange((void*)0);
static FacDynDonateCash __stub_FacDynDonateCash((void*)0);
static FacDynPutAuction __stub_FacDynPutAuction((void*)0);
static FacDynWinAuction __stub_FacDynWinAuction((void*)0);
static FacDynBuyAuction __stub_FacDynBuyAuction((void*)0);
static FacDynBuildingUpgrade __stub_FacDynBuildingUpgrade((void*)0);
static FacDynBuildingComplete __stub_FacDynBuildingComplete((void*)0);
static GFacBaseEnter __stub_GFacBaseEnter((void*)0);
static GFacBaseLeave __stub_GFacBaseLeave((void*)0);
static FacBasePropChange __stub_FacBasePropChange((void*)0);
static FacBaseBuildingProgress __stub_FacBaseBuildingProgress((void*)0);
static FacAuctionPut __stub_FacAuctionPut((void*)0);
static StartFacBase_Re __stub_StartFacBase_Re((void*)0);
static StopFacBase_Re __stub_StopFacBase_Re((void*)0);
static GUpdateFacAct __stub_GUpdateFacAct((void*)0);
static GFacBaseServerRegister __stub_GFacBaseServerRegister((void*)0);
static GlobalDropBroadCast __stub_GlobalDropBroadCast((void*)0);
static RegisterGlobalCounter __stub_RegisterGlobalCounter((void*)0);
static GRaidServerRegister __stub_GRaidServerRegister((void*)0);
static GRaidOpen __stub_GRaidOpen((void*)0);
static GRaidJoin __stub_GRaidJoin((void*)0);
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
static AddCirclePoint __stub_AddCirclePoint((void*)0);
static AnnounceProviderID __stub_AnnounceProviderID((void*)0);
static PlayerLogin_Re __stub_PlayerLogin_Re((void*)0);
static PlayerKickout_Re __stub_PlayerKickout_Re((void*)0);
static PlayerLogout __stub_PlayerLogout((void*)0);
static PlayerOffline_Re __stub_PlayerOffline_Re((void*)0);
static PlayerReconnect_Re __stub_PlayerReconnect_Re((void*)0);
static QueryPlayerStatus __stub_QueryPlayerStatus((void*)0);
static GetTaskData __stub_GetTaskData((void*)0);
static SetTaskData __stub_SetTaskData((void*)0);
static SetChatEmotion __stub_SetChatEmotion((void*)0);
static BattleFlagStart __stub_BattleFlagStart((void*)0);
static QueryBattleFlagBuff __stub_QueryBattleFlagBuff((void*)0);
static GetCouponsRep __stub_GetCouponsRep((void*)0);
static GTradeStart_Re __stub_GTradeStart_Re((void*)0);
static GTradeDiscard __stub_GTradeDiscard((void*)0);
static KeepAlive __stub_KeepAlive((void*)0);
static DisconnectPlayer __stub_DisconnectPlayer((void*)0);
static GMRestartServer_Re __stub_GMRestartServer_Re((void*)0);
static AuctionGetItem __stub_AuctionGetItem((void*)0);
static QueryGameServerAttr __stub_QueryGameServerAttr((void*)0);
static ACReportCheater __stub_ACReportCheater((void*)0);
static ACTriggerQuestion __stub_ACTriggerQuestion((void*)0);
static BattleServerRegister __stub_BattleServerRegister((void*)0);
static BattleEnterFail __stub_BattleEnterFail((void*)0);
static GBattleEnd __stub_GBattleEnd((void*)0);
static GBattleLeave __stub_GBattleLeave((void*)0);
static DebugCommand __stub_DebugCommand((void*)0);
static GOpenBanquetLeave __stub_GOpenBanquetLeave((void*)0);
static GOpenBanquetJoin __stub_GOpenBanquetJoin((void*)0);
static InstancingRegister __stub_InstancingRegister((void*)0);
static InstancingEnterFail __stub_InstancingEnterFail((void*)0);
static GInstancingEnd __stub_GInstancingEnd((void*)0);
static SendInstancingStart_Re __stub_SendInstancingStart_Re((void*)0);
static GInstancingLeave __stub_GInstancingLeave((void*)0);
static GBattleJoin __stub_GBattleJoin((void*)0);
static SendFactionCreate __stub_SendFactionCreate((void*)0);
static PlayerFactionInfo __stub_PlayerFactionInfo((void*)0);
static GHostileProtect __stub_GHostileProtect((void*)0);
static SendCrssvrTeamsCreate __stub_SendCrssvrTeamsCreate((void*)0);
static SyncRoleData __stub_SyncRoleData((void*)0);
static StockCommission __stub_StockCommission((void*)0);
static StockAccount __stub_StockAccount((void*)0);
static StockTransaction __stub_StockTransaction((void*)0);
static StockBill __stub_StockBill((void*)0);
static StockCancel __stub_StockCancel((void*)0);
static CombatKill __stub_CombatKill((void*)0);
static PlayerChangeGS_Re __stub_PlayerChangeGS_Re((void*)0);
static FamilyUseSkill __stub_FamilyUseSkill((void*)0);
static FamilyExpShare __stub_FamilyExpShare((void*)0);
static GSiegeServerRegister __stub_GSiegeServerRegister((void*)0);
static GSiegeEnd __stub_GSiegeEnd((void*)0);
static SendSiegeStart_Re __stub_SendSiegeStart_Re((void*)0);
static SendSiegeChallenge __stub_SendSiegeChallenge((void*)0);
static SiegeSetAssistant __stub_SiegeSetAssistant((void*)0);
static SiegeKill __stub_SiegeKill((void*)0);
static SiegeBroadcat __stub_SiegeBroadcat((void*)0);
static SectUpdate __stub_SectUpdate((void*)0);
static SendSNSPressMessage __stub_SendSNSPressMessage((void*)0);
static SendSNSApply __stub_SendSNSApply((void*)0);
static SendSNSVote __stub_SendSNSVote((void*)0);
static BillingRequest2 __stub_BillingRequest2((void*)0);
static SendRefCashUsed __stub_SendRefCashUsed((void*)0);
static SendRefLevelUp __stub_SendRefLevelUp((void*)0);
static SendChangeProfile __stub_SendChangeProfile((void*)0);
static QueryNetBarReward __stub_QueryNetBarReward((void*)0);
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
static SyncBathTimes __stub_SyncBathTimes((void*)0);
static IsConnAlive __stub_IsConnAlive((void*)0);
static S2CLineBroadcast __stub_S2CLineBroadcast((void*)0);
static GTouchPointExchange __stub_GTouchPointExchange((void*)0);
static DelRoleAnnounce __stub_DelRoleAnnounce((void*)0);
static UpdateTopTable __stub_UpdateTopTable((void*)0);
static UpdateQueen __stub_UpdateQueen((void*)0);
static ACRemoteCode __stub_ACRemoteCode((void*)0);
static ACQuestion __stub_ACQuestion((void*)0);
static ACKickoutUser __stub_ACKickoutUser((void*)0);
static ACForbidUser __stub_ACForbidUser((void*)0);
static ACAccuse_Re __stub_ACAccuse_Re((void*)0);
static Post_Re __stub_Post_Re((void*)0);
static GamePostCancel_Re __stub_GamePostCancel_Re((void*)0);
static WebPostCancel __stub_WebPostCancel((void*)0);
static Shelf __stub_Shelf((void*)0);
static ShelfCancel __stub_ShelfCancel((void*)0);
static Sold __stub_Sold((void*)0);
static PostExpire __stub_PostExpire((void*)0);
static WebGetRoleList __stub_WebGetRoleList((void*)0);
static LoadExchange __stub_LoadExchange((void*)0);
static DSAnnounceIdentity __stub_DSAnnounceIdentity((void*)0);
static SendDataAndIdentity __stub_SendDataAndIdentity((void*)0);
static SendDataAndIdentity_Re __stub_SendDataAndIdentity_Re((void*)0);
static RemoteLoginQuery __stub_RemoteLoginQuery((void*)0);
static RemoteLogout __stub_RemoteLogout((void*)0);
static KickoutRemoteUser_Re __stub_KickoutRemoteUser_Re((void*)0);
static GetRemoteRoleInfo_Re __stub_GetRemoteRoleInfo_Re((void*)0);
static CrossBattleJoinTeam __stub_CrossBattleJoinTeam((void*)0);
static CrossCrssvrTeamsOnlineStatus __stub_CrossCrssvrTeamsOnlineStatus((void*)0);
static CrossCrssvrTeamsGetReq __stub_CrossCrssvrTeamsGetReq((void*)0);
static RemoteLoginQuery_Re __stub_RemoteLoginQuery_Re((void*)0);
static KickoutRemoteUser __stub_KickoutRemoteUser((void*)0);
static GetRemoteRoleInfo __stub_GetRemoteRoleInfo((void*)0);
static COpenBanquetJoin __stub_COpenBanquetJoin((void*)0);
static OpenBanquetLogout __stub_OpenBanquetLogout((void*)0);
static GetKDCToken_Re __stub_GetKDCToken_Re((void*)0);
static GateOnlineList_Re __stub_GateOnlineList_Re((void*)0);
static GateGetGTRoleStatus_Re __stub_GateGetGTRoleStatus_Re((void*)0);
static GateUpdateStatus __stub_GateUpdateStatus((void*)0);
static GateOfflineChat __stub_GateOfflineChat((void*)0);
static GateFactionChat __stub_GateFactionChat((void*)0);

};
