
#ifndef __GNET_WEBGETROLELIST_HPP
#define __GNET_WEBGETROLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "dbconsigngetrolesimpleinfo.hrp"
namespace GNET
{

class WebGetRoleList : public GNET::Protocol
{
	#include "webgetrolelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		DEBUG_PRINT("webgetrolelist: userid=%d\n", userid); 
	
		DBConsignGetRoleSimpleInfo * rpc = (DBConsignGetRoleSimpleInfo *)Rpc::Call(
			RPC_DBCONSIGNGETROLESIMPLEINFO,
			DBConsignGetRoleSimpleInfoArg(userid)		
		);
		rpc->aid = GDeliveryServer::GetInstance()->aid; 
		rpc->messageid = messageid;
		rpc->timestamp = timestamp;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
