#include "gamedbclient.hpp"
#include "userqueryagent2.h"
#include "gtmanager.h"
#include "dbgategetuser.hrp"
#include "dbgategetrole.hrp"
#include "gtcommon.h"
namespace GNET
{
int UserQueryOperation::TryQuery(UserQuery key1)
{
	return GTManager::Instance()->TryUserQuery(key1);
}
int UserQueryOperation::DBQuery1(UserQuery req)const
{
	DBGateGetUser * rpc=(DBGateGetUser*)Rpc::Call(
		RPC_DBGATEGETUSER,
		UserID(req.userid)
		);	
	rpc->pt_type = req.pt_type;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}
int UserQueryOperation::DBQuery2(int key,UserQuery req)const
{
	DBGateGetRole * rpc=(DBGateGetRole*)Rpc::Call(
		RPC_DBGATEGETROLE,
		RoleId(key)
		);	
	rpc->userid=req.userid;
	rpc->pt_type = req.pt_type;
	rpc->localuid = 0;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}
int UserQueryOperation::BuildResult(std::vector<RoleDetailBean> & roles,const GGateRoleInfo& info)const
{
	RoleStatusBean status = GTManager::Instance()->RoleStatusQuery(info.roleid); 
	roles.push_back(RoleDetailBean(GGateRoleInfo2RoleInfoBean(info),status));
	return 0;
}

};

