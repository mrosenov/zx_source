#include "gamedbclient.hpp"
#include "factionqueryagent.h"
#include "gtmanager.h"
#include "dbfamilyget.hrp"
#include "dbfactionget.hrp"
//#include "roleinfobean"
//#include "rolebean"

namespace GNET
{

int FactionQueryOperation::TryQuery(const FactionReq& key1)
{
	DEBUG_PRINT("FactionQueryOperation TryQuery,fid=%d",key1.fid);
	return GTManager::Instance()->TryFactionQuery(key1);
}

int FactionQueryOperation::DBQuery1(FactionReq req)const
{
	DEBUG_PRINT("FactionQueryOperation DBQuery1,fid=%d",req.fid);
	DBFactionGet * rpc=(DBFactionGet*)Rpc::Call(
		RPC_DBFACTIONGET,
		FactionId(req.fid)
		);
	rpc->localuid=req.localuid;
	rpc->pt_type = req.pt_type;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}

int FactionQueryOperation::DBQuery2(int familyid, FactionReq req)const
{
	DEBUG_PRINT("FactionQueryOperation DBQuery2,famiid=%d fid=%d",familyid,req.fid);
	DBFamilyGet * rpc=(DBFamilyGet*)Rpc::Call(
		RPC_DBFAMILYGET,
		FamilyId(familyid)
		);	
	rpc->factionid=req.fid;
	rpc->localuid=0;
	rpc->pt_type = req.pt_type;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}

int FactionQueryOperation::BuildResult(FactionInfoResp & resp,const std::vector<GNET::GFolk>& info)const
{
//	DEBUG_PRINT("FactionQueryOperation BuildResult");
	return GTManager::Instance()->FamilyMemberSetData(resp,info,200);
}

FactionInfoBean GFactionInfo2FactionInfoBean(const GFactionInfo& f)
{
	return FactionInfoBean(f.name,f.announce);
}

};

