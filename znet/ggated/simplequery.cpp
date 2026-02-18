#include "simplequery.h"
#include "gamedbclient.hpp"
#include "gamegateclient.hpp"
#include "gtmanager.h"
#include "dbgaterolerelation.hrp"
#include "dbgategetrole.hrp"
#include "dbfamilyget.hrp"
#include "gategetrolerelation.hrp"

namespace GNET
{

int RoleRelationTask::TryQuery()const
{
//	DEBUG_PRINT("RoleRelationTask::TryQuery roleid=%d",roleid);
	if(GTManager::Instance()->GameOnline(roleid))
	{
		GateGetRoleRelation * rpc=(GateGetRoleRelation*)Rpc::Call(
			RPC_GATEGETROLERELATION,
			RoleId(roleid)
		);
		rpc->userid = userid;	
		rpc->pt_type = pt_type;
		GameGateClient::GetInstance()->SendProtocol(rpc);
	}
	else
	{
		DBGateRoleRelation * rpc=(DBGateRoleRelation*)Rpc::Call(
			RPC_DBGATEROLERELATION,
			RoleId(roleid)
		);
		rpc->userid = userid;
		rpc->pt_type = pt_type;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	return 0;
}

int RoleInfoTask::TryQuery()const
{
//	DEBUG_PRINT("RoleInfoTask::TryQuery roleid=%d",roleid);
	DBGateGetRole * rpc=(DBGateGetRole*)Rpc::Call(
		RPC_DBGATEGETROLE,
		RoleId(roleid)
		);
	rpc->localuid = localuid;
	rpc->userid = 0;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}

int FamilyInfoTask::TryQuery()const
{
//	DEBUG_PRINT("FamilyInfoTask::TryQuery fid=%d",fid);
	DBFamilyGet * rpc=(DBFamilyGet*)Rpc::Call(
        	RPC_DBFAMILYGET,
		FamilyId(fid)
		);
	rpc->localuid = localuid;
	rpc->factionid=0;	
	GameDBClient::GetInstance()->SendProtocol(rpc);

	return 0;
}
};
