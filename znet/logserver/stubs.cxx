#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "statinfovital.hpp"
#include "statinfo.hpp"
#include "remotelogvital.hpp"
#include "remotelog.hpp"
#include "logstockcommission.hpp"
#include "logstockbalance.hpp"
#include "logstocktransaction.hpp"

namespace GNET
{

static StatInfoVital __stub_StatInfoVital((void*)0);
static StatInfo __stub_StatInfo((void*)0);
static RemoteLogVital __stub_RemoteLogVital((void*)0);
static RemoteLog __stub_RemoteLog((void*)0);
static LogStockCommission __stub_LogStockCommission((void*)0);
static LogStockBalance __stub_LogStockBalance((void*)0);
static LogStockTransaction __stub_LogStockTransaction((void*)0);

};
