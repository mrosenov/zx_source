#include "mapuser.h"
#include "mapforbid.h"
#include "gproviderserver.hpp"
#include "blockedrole.h"
#include "itimer.h"
#include "localmacro.h"
#include "gamedbclient.hpp"
#include "consignmanager.h"
#include "gdeliveryserver.hpp"
#include "logoutroletask.h"
#include "playerconsignoffline.hpp"
#include "dbconsignprepostrole.hrp"
#include "dbconsignsoldrole.hrp"
#include "consignplayer_re.hpp"
#include "playerconsignoffline_re.hpp"
namespace GNET
{

LockRoleStatus BlockedRole::LockRole(int roleid)
{
	BlockedRoleMap::iterator it = rolemap.find(roleid);
	if(it!=rolemap.end())//permit to lock one role many times
		return it->second;

	int status=CheckLoginStatus(roleid);
	LockRoleStatus ret;
	switch(status)
	{
		case RoleBusy:
			ret = LockRoleFail; 
			break;
		case RoleNotInUse:
			rolemap.insert(std::make_pair(roleid,LockRoleOK));
			ret = LockRoleOK;
			break;
		case RoleOnGame:
		{	
			//ask gs to logout player
			PlayerInfo* role = UserContainer::GetInstance().FindRole(roleid);
			if(!role || !role->user)
			{
				Log::log( LOG_ERR,"LockRole err ,roleid=%d not found in case RoleOnGame",roleid);
				ret = LockRoleFail;
			}
			else
			{
				DEBUG_PRINT("BlockedRole::LockRole PlayerConsignOffline roleid=%d",roleid);
				UserInfo * pinfo = role->user;
				PlayerConsignOffline data(roleid,pinfo->linkid,pinfo->localsid);
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid,data);
				//dont add to ForbiddenUsers, not userlogout
				//ForbiddenUsers::GetInstance().Push(pinfo->userid,pinfo->roleid,pinfo->status);	
				ret = LockRoleLocking;
				rolemap.insert(std::make_pair(roleid,LockRoleLocking));
			}
			break;
		}
		default:
			ret = LockRoleFail;
			Log::log( LOG_ERR,"LockRole err ,roleid=%d invalid status",roleid);
			break;
	}
	return ret;
}

bool BlockedRole::IsRoleBlocked(int roleid)
{
	return rolemap.find(roleid)!=rolemap.end();
}

void BlockedRole::UnLockRole(int roleid)
{
	DEBUG_PRINT("BlockedRole::UnLockRole roleid=%d",roleid);
	rolemap.erase(roleid);
}

LockRoleStatus BlockedRole::RoleLockStatus(int roleid)
{
	BlockedRoleMap::iterator it = rolemap.find(roleid);
	if(it==rolemap.end())
		return LockRoleNotFound;
	return it->second;
}

bool BlockedRole::CompleteRoleLock(int roleid)
{
	DEBUG_PRINT("BlockedRole::CompleteRoleLock roleid=%d",roleid);
	BlockedRoleMap::iterator it = rolemap.find(roleid);
	if(it!=rolemap.end() && it->second==LockRoleLocking)
	{
		it->second=LockRoleOK;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if(pinfo)
			LogoutRoleTask::Add(*pinfo);
		return true;
	}
	DEBUG_PRINT("BlockedRole::CompleteRoleLock roleid=%d fail",roleid);
	return false;
}

int BlockedRole::CheckLoginStatus(int roleid)
{
        int userid = UserContainer::Roleid2Userid(roleid);
        UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
	//user offline
	if(pinfo == NULL)
	{
		if(ForbiddenUsers::GetInstance().IsExist(userid))
			return RoleBusy;//role may be logging out
		else
			return RoleNotInUse;
	}
	//otherwise user online
	if(pinfo->status==_STATUS_ONLINE || pinfo->roleid!=roleid)
		return RoleNotInUse;
	//target role in game
	if(pinfo->status==_STATUS_ONGAME)
		return RoleOnGame;
	//target role in otherstatus
	return RoleBusy;
}

LockRoleStatus BlockedRole::UpdateLockStatus(int roleid)
{
	BlockedRoleMap::iterator it = rolemap.find(roleid);
	if(it==rolemap.end())
		return LockRoleNotFound;
	int status=CheckLoginStatus(roleid);
	switch(status)
	{
		case RoleNotInUse:
			it->second = LockRoleOK;
			break;
		case RoleOnGame:
		{	
		//	status not change
			break;
		}
		case RoleBusy:
		default:
			Log::log( LOG_ERR,"UpdateLockStatus err ,roleid=%d invalid status %d",roleid,status);
			return LockRoleFail;
			break;
	}
	return it->second;	
}

void RoleTask::Update(int now)
{
	switch(taskstatus)
	{
	case TaskInit:
		UpdateOnInit(now);
		break;
	case RoleLocking:
		UpdateOnRoleLocking(now);
		break;
	case WaitDBResp:
	//	UpdateOnWaitDBResp(now);
		break;
	default:
		DEBUG_PRINT("RoleTask::Update status error status=%d ",taskstatus);
		break;
	}
	CheckTimeOut(now);
}

void RoleTask::CheckTimeOut(int now)
{
	if(taskstatus == RoleLocking || taskstatus == WaitDBResp)
	{
		if(status_start_time + STATUS_TIME_LIMIT < now)
			SetStatus(TaskFail,now);
	}
}

void ConsignRoleTask::UpdateOnRoleLocking(int now)
{
	LockRoleStatus lstatus = BlockedRole::GetInstance()->UpdateLockStatus(data.roleid);
	if(lstatus == LockRoleOK)
	{	
		DEBUG_PRINT("ConsignRoleTask::UpdateOnRoleLocking roleid=%d,lock ok",data.roleid);
		DBConsignPrePostRole* rpc=(DBConsignPrePostRole*)Rpc::Call(
			RPC_DBCONSIGNPREPOSTROLE,
			data
		);
		rpc->taskid=GetTaskID();
		GameDBClient::GetInstance()->SendProtocol(rpc);
		SetStatus(WaitDBResp,now);
	}
//	else
//		 DEBUG_PRINT("ConsignRoleTask::UpdateOnRoleLocking roleid=%d,lockstatus=%d",data.roleid,lstatus);
	//todo other lock status check
}

/*void ConsignRoleTask::UpdateOnWaitDBResp(int now)
{

}*/
void ConsignRoleTask::SetGSRoleInfo(const GConsignGsRoleInfo& gsroleinfo)
{
	data.roleinfo=gsroleinfo;
}

void ConsignRoleTask::OnFinish(TaskStatus current)
{
	DEBUG_PRINT("ConsignRoleTask::OnFinish taskstatus=%d",current);
//	if(current==RoleLocking || current==WaitDBResp)
	if(IsFail())
	{
//		if(current==RoleLocking)
		{
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(data.roleid);
			if ( NULL!=pinfo )
			{
				GDeliveryServer::GetInstance()->Send(
					pinfo->linksid,
					ConsignPlayer_Re(data.roleid,pinfo->localsid,ERR_WTC_TIMEOUT)
				);
			}
		}	
	}
	BlockedRole::GetInstance()->UnLockRole(data.roleid);
	ConsignManager::GetInstance()->ClearConsignStarting(data.roleid);
}

void ConsignRoleTask::UpdateOnInit(int now)
{
	LockRoleStatus lstatus = BlockedRole::GetInstance()->LockRole(data.roleid);
	switch(lstatus)
	{
	case LockRoleOK:
		{	
		DEBUG_PRINT("ConsignRoleTask::UpdateOnInit roleid=%d,lock ok",data.roleid);
		DBConsignPrePostRole* rpc=(DBConsignPrePostRole*)Rpc::Call(
			RPC_DBCONSIGNPREPOSTROLE,
			data
		);
		rpc->taskid=GetTaskID();
		GameDBClient::GetInstance()->SendProtocol(rpc);
		SetStatus(WaitDBResp,now);
		break;
		}
	case LockRoleLocking:
		SetStatus(RoleLocking,now);
		break;
	default:
		SetStatus(TaskFail,now);
		break;
	}
}

void SoldRoleTask::OnFinish(TaskStatus current)
{
	if(current==RoleLocking || current==WaitDBResp)
	{
	//	if(current==RoleLocking)
	//	{
	//		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(data.buyer_roleid);
	//		if ( NULL!=pinfo )
	//		{
	//			GWebTradeClient::GetInstance()->SendProtocol(Sold_Re(zoneid,userid,roleid,buyer_userid,buyer_roleid,sn,retcode,arg->orderid));
	//		}
	//	}
//		BlockedRole::GetInstance()->UnLockRole(data.buyer_roleid);
	}
//	DEBUG_PRINT("SoldRoleTask::OnFinish taskstatus=%d",current);
	if(IsFail())
	{
		ConsignManager::GetInstance()->ClearBusy(data.sn);
	}
	BlockedRole::GetInstance()->UnLockRole(data.buyer_roleid);
}

void SoldRoleTask::UpdateOnRoleLocking(int now)
{
	LockRoleStatus lstatus = BlockedRole::GetInstance()->UpdateLockStatus(data.buyer_roleid);
	if(lstatus == LockRoleOK)
	{	
		DEBUG_PRINT("SoldRoleTask::UpdateOnRoleLocking roleid=%d,lock ok",data.buyer_roleid);
		DBConsignSoldRole* rpc = (DBConsignSoldRole *)Rpc::Call(
			RPC_DBCONSIGNSOLDROLE,
			data
		);
		rpc->taskid=GetTaskID();
		rpc->zoneid=zoneid_;
		rpc->timestamp=timestamp_;
		rpc->userid=userid_;
		rpc->userid_mismatch=userid_mismatch_;
		rpc->roleid_mismatch=roleid_mismatch_;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		SetStatus(WaitDBResp,now);
	}
	//todo other lock status check
}

void SoldRoleTask::UpdateOnInit(int now)
{
	LockRoleStatus lstatus = BlockedRole::GetInstance()->LockRole(data.buyer_roleid);//seller should in consign status
	switch(lstatus)
	{
	case LockRoleOK:
		{	
		DBConsignSoldRole* rpc = (DBConsignSoldRole *)Rpc::Call(
			RPC_DBCONSIGNSOLDROLE,
			data
		);
		rpc->taskid=GetTaskID();
		rpc->zoneid=zoneid_;
		rpc->timestamp=timestamp_;
		rpc->userid=userid_;
		rpc->userid_mismatch=userid_mismatch_;
		rpc->roleid_mismatch=roleid_mismatch_;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		SetStatus(WaitDBResp,now);
		break;
		}
	case LockRoleLocking:
		SetStatus(RoleLocking,now);
		break;
	default:
		SetStatus(TaskFail,now);
		break;
	}
}

bool RoleTaskManager::Update()
{
	int now=Timer::GetTime();
	RoleTaskMap::iterator it,ite=taskmap.end();
	for ( it = taskmap.begin();it!=ite;)
	{
		it->second->Update(now);
		if(it->second->IsFail())
		{
			delete it->second;
			taskmap.erase(it++);
		}
		else
			++it;
	}
	return true;
}

bool RoleTaskManager::Initialize()
{
        IntervalTimer::AddTimer(this,ROLETASK_MANAGER_UPDATE_INTERVAL);
        return true;
}

int RoleTaskManager::DBConsignRoleResp(int taskid,int retcode,const GConsignDB& detail,const GConsignRole& role)
{
	RoleTaskMap::iterator it=taskmap.find(taskid);
	if(it == taskmap.end())
	{
		Log::formatlog("consign","DBConsignRoleResp timeout. sn=%lld,roleid=%d",\
	                detail.info.sn,detail.seller_roleid);
		return ERR_WT_TIMEOUT;
	}
	delete it->second;
	taskmap.erase(it);
	if(retcode == ERR_SUCCESS)
	{
		ConsignManager::GetInstance()->OnDBPrePostRole(detail,role);
		RoleInfoCache::Instance().SetSelling(detail.seller_roleid,true);
		return 0;
	}
	else
	{
		return retcode;
	}
}

int RoleTaskManager::ConsignRole(const DBConsignPrePostRoleArg& msg)
{
	DEBUG_PRINT("RoleTaskManager::ConsignRole roleid=%d",msg.roleid);
	int taskid=GetTaskID();
	std::pair<RoleTaskMap::iterator,bool> ret = taskmap.insert(std::make_pair(taskid,new ConsignRoleTask(taskid,msg)));
	if(ret.second==false)
		return -1;
	ret.first->second->Update(Timer::GetTime());
	if(ret.first->second->IsFail())
		return -1;
	return 0;
}

int RoleTaskManager::ConsignRoleOfflineAddData(int roleid, const GConsignGsRoleInfo& gsroleinfo)
{
	DEBUG_PRINT("RoleTaskManager::ConsignRoleOfflineAddData roleid=%d",roleid);
	RoleTaskMap::iterator it,ite=taskmap.end();
	for ( it = taskmap.begin();it!=ite;++it)
	{
		if(it->second->GetRoleID()==roleid)
		{
			if(!(it->second->IsLocking()))
			{
				Log::log( LOG_ERR, "ConsignRoleOfflineAddData status=%d err",it->second->GetStatus());
				break;
			}
			ConsignRoleTask* p = dynamic_cast<ConsignRoleTask *>(it->second);
			if(p==NULL)
			{
				Log::log( LOG_ERR, "task type error");
				break;
			}
			p->SetGSRoleInfo(gsroleinfo);
			return 0;
		}
	}
	if(it==ite)
		DEBUG_PRINT("RoleTaskManager::ConsignRoleOfflineAddData roleid=%d not found",roleid);
	return -1;
}

int RoleTaskManager::SoldRole(const DBConsignSoldArg& msg,int zoneid,int64_t timestamp,int userid,bool userid_mismatch,bool roleid_mismatch)
{
	DEBUG_PRINT("RoleTaskManager::SoldRole roleid=%d sellto=%d",msg.roleid,msg.buyer_roleid);
	int taskid=GetTaskID();
	std::pair<RoleTaskMap::iterator,bool> ret = taskmap.insert(std::make_pair(taskid,new SoldRoleTask(taskid,msg,zoneid,timestamp,userid,userid_mismatch,roleid_mismatch)));
	if(ret.second==false)
	{
		DEBUG_PRINT("RoleTaskManager::SoldRole taskid=%d err",taskid);
		return ERR_WT_ROLE_IS_BUSY;
	}
	ret.first->second->Update(Timer::GetTime());
	if(ret.first->second->IsFail())
		return ERR_WT_ROLE_IS_BUSY;
	return 0;
}

int RoleTaskManager::DBConsignSoldRoleResp(int taskid,int roleid, int64_t sn,int64_t timestamp)
{
	RoleTaskMap::iterator it=taskmap.find(taskid);
	if(it == taskmap.end())
	{
		DEBUG_PRINT("RoleTaskManager::DBConsignSoldRoleResp timeout taskid=%d",taskid);
		return ERR_WT_TIMEOUT;
	}
	delete it->second;
	taskmap.erase(it);
	//RoleInfoCache::Instance().SetSelling(roleid,false);//move to OnDBSoldRole success
	return ConsignManager::GetInstance()->OnDBSoldRole(roleid,sn,timestamp);
}

int RoleTaskManager::DBConsignSoldRoleFail(int taskid)
{
	RoleTaskMap::iterator it=taskmap.find(taskid);
	if(it == taskmap.end())
	{
		DEBUG_PRINT("RoleTaskManager::DBConsignSoldRoleFail timeout taskid=%d",taskid);
		return ERR_WT_TIMEOUT;
	}
	delete it->second;
	taskmap.erase(it);
	return 0;	
}

};
