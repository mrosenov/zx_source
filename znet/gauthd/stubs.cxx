#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "gquerypasswd.hrp"
#include "matrixpasswd.hrp"
#include "matrixpasswd2.hrp"
#include "matrixtoken.hrp"
#include "userlogin.hrp"
#include "userlogin2.hrp"
#include "userlogout.hrp"
#include "getaddcashsn.hrp"
#include "cashserial.hrp"
#include "netmarblelogin.hrp"
#include "netmarblededuct.hrp"
#include "queryuserprivilege_re.hpp"
#include "queryuserforbid_re.hpp"
#include "kickoutuser.hpp"
#include "accountingresponse.hpp"
#include "authdversion.hpp"
#include "keyexchange.hpp"
#include "statusannounce.hpp"
#include "accountingrequest.hpp"
#include "announcezoneid.hpp"
#include "announcezoneid2.hpp"
#include "announcezoneid3.hpp"
#include "game2au.hpp"
#include "queryuserprivilege.hpp"
#include "queryuserprivilege2.hpp"
#include "queryuserforbid.hpp"
#include "queryuserforbid2.hpp"
#include "gmkickoutuser.hpp"
#include "gmforbidsellpoint.hpp"
#include "gmshutup.hpp"
#include "transbuypoint.hpp"
#include "addcash.hpp"
#include "addcash_re.hpp"
#include "verifymaster.hpp"
#include "verifymaster_re.hpp"
#include "matrixfailure.hpp"
#include "billingrequest.hpp"
#include "billingbalance.hpp"
#include "netmarblelogout.hpp"

namespace GNET
{

static GQueryPasswd __stub_GQueryPasswd (RPC_GQUERYPASSWD, new GQueryPasswdArg, new GQueryPasswdRes);
static MatrixPasswd __stub_MatrixPasswd (RPC_MATRIXPASSWD, new MatrixPasswdArg, new MatrixPasswdRes);
static MatrixPasswd2 __stub_MatrixPasswd2 (RPC_MATRIXPASSWD2, new MatrixPasswdArg2, new MatrixPasswdRes2);
static MatrixToken __stub_MatrixToken (RPC_MATRIXTOKEN, new MatrixTokenArg, new MatrixTokenRes);
static UserLogin __stub_UserLogin (RPC_USERLOGIN, new UserLoginArg, new UserLoginRes);
static UserLogin2 __stub_UserLogin2 (RPC_USERLOGIN2, new UserLoginArg2, new UserLoginRes2);
static UserLogout __stub_UserLogout (RPC_USERLOGOUT, new UserLogoutArg, new UserLogoutRes);
static GetAddCashSN __stub_GetAddCashSN (RPC_GETADDCASHSN, new GetAddCashSNArg, new GetAddCashSNRes);
static CashSerial __stub_CashSerial (RPC_CASHSERIAL, new CashSerialArg, new CashSerialRes);
static NetMarbleLogin __stub_NetMarbleLogin (RPC_NETMARBLELOGIN, new NetMarbleLoginArg, new NetMarbleLoginRes);
static NetMarbleDeduct __stub_NetMarbleDeduct (RPC_NETMARBLEDEDUCT, new NetMarbleDeductArg, new NetMarbleDeductRes);
static QueryUserPrivilege_Re __stub_QueryUserPrivilege_Re((void*)0);
static QueryUserForbid_Re __stub_QueryUserForbid_Re((void*)0);
static KickoutUser __stub_KickoutUser((void*)0);
static AccountingResponse __stub_AccountingResponse((void*)0);
static AuthdVersion __stub_AuthdVersion((void*)0);
static KeyExchange __stub_KeyExchange((void*)0);
static StatusAnnounce __stub_StatusAnnounce((void*)0);
static AccountingRequest __stub_AccountingRequest((void*)0);
static AnnounceZoneid __stub_AnnounceZoneid((void*)0);
static AnnounceZoneid2 __stub_AnnounceZoneid2((void*)0);
static AnnounceZoneid3 __stub_AnnounceZoneid3((void*)0);
static Game2AU __stub_Game2AU((void*)0);
static QueryUserPrivilege __stub_QueryUserPrivilege((void*)0);
static QueryUserPrivilege2 __stub_QueryUserPrivilege2((void*)0);
static QueryUserForbid __stub_QueryUserForbid((void*)0);
static QueryUserForbid2 __stub_QueryUserForbid2((void*)0);
static GMKickoutUser __stub_GMKickoutUser((void*)0);
static GMForbidSellPoint __stub_GMForbidSellPoint((void*)0);
static GMShutup __stub_GMShutup((void*)0);
static TransBuyPoint __stub_TransBuyPoint((void*)0);
static AddCash __stub_AddCash((void*)0);
static AddCash_Re __stub_AddCash_Re((void*)0);
static VerifyMaster __stub_VerifyMaster((void*)0);
static VerifyMaster_Re __stub_VerifyMaster_Re((void*)0);
static MatrixFailure __stub_MatrixFailure((void*)0);
static BillingRequest __stub_BillingRequest((void*)0);
static BillingBalance __stub_BillingBalance((void*)0);
static NetMarbleLogout __stub_NetMarbleLogout((void*)0);

};
