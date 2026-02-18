
#ifndef __GNET_SOLD_HPP
#define __GNET_SOLD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class Sold : public GNET::Protocol
{
	#include "sold"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("Consign sold: receive. zoneid=%d,roleid=%lld,sn=%lld,buyerroleid=%lld,orderid=%lld\n",zoneid,sellerroleid,sn,buyerroleid,orderid);
		int retcode = ConsignManager::GetInstance()->DoSold(zoneid, selleruserid, sellerroleid, sn, buyeruserid, buyerroleid, orderid, stype, timestamp);
		if(retcode != ERR_SUCCESS)
		{
			Sold_Re re;
			re.zoneid = zoneid;
			re.selleruserid = selleruserid;
			re.sellerroleid = sellerroleid;
			re.buyeruserid = buyeruserid;
			re.buyerroleid = buyerroleid;
			re.sn = sn;
			re.orderid = orderid;
			switch(retcode)
			{
				default:
				case ERR_WT_UNOPEN:
				case ERR_WT_ROLE_IS_BUSY:	
					re.retcode = -1;
					break;
		//		case ERR_WT_ENTRY_HAS_BEEN_SOLD:
		//			re.retcode = 2;
		//			break;
				case ERR_WT_ENTRY_NOT_FOUND:
					re.retcode = 1; 
					break;
				case ERR_WT_ENTRY_IS_BUSY:
					re.retcode = 7; 
					break;
				case ERR_WT_TIMESTAMP_EQUAL:
					re.retcode = 0;
					break;
				case ERR_WT_DUPLICATE_RQST://sold with greater timestamp
					re.retcode = -1;
					break;
			}
			DEBUG_PRINT("Consign sold: err. retcode=%d,zoneid=%d,roleid=%lld,sn=%lld,buyerroleid=%lld,orderid=%lld\n",retcode,zoneid,sellerroleid,sn,buyerroleid,orderid);
			if(retcode==ERR_WT_ENTRY_NOT_FOUND)
			{
				ConsignManager::FinishedRecord record;
				retcode = ConsignManager::GetInstance()->GetFinishedRecord(sn, record);
				if (retcode == -1)
					re.retcode = -1;
				else if (retcode == ERR_SUCCESS)
				{
					if (record.orderid == orderid)
						re.retcode = 2; //sn order验证通过表明是一个成交成功的历史订单
					else
						re.retcode = 3;
				}
				else if (retcode == ERR_WT_ENTRY_NOT_FOUND)
					re.retcode = 1;
				else
					re.retcode = 3;//3表示其他错误 会引起平台那边的特殊注意
			}	
			GWebTradeClient::GetInstance()->SendProtocol(re);
		}
	}
};

};

#endif
