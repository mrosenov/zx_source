
#include "gtplatformclient.hpp"
#include "gtmanager.h"
#include "state.hxx"
//#include "timertask.h"
#include "gtplatformreconnecttask.h"
#include "localmacro.h"
#include "announcezoneidtoim.hpp"
#include "gaterolecache.h"

namespace GNET
{

GTPlatformClient GTPlatformClient::instance;

void GTPlatformClient::GTConnect()
{
	if(GTManager::Instance()->IsGTOpen()!=true)
		return;
	DEBUG_PRINT("GTPlatformClient::GTConnect");
	Thread::HouseKeeper::AddTimerTask(new GTPlatformReconnectTask(this, 1), backoff);
	backoff *= 2;
	if (backoff > BACKOFF_DEADLINE)
		backoff = BACKOFF_DEADLINE;
	SET_GT_CONN_STATE(GTCONNECTING);
}

bool GTPlatformClient::TryReconnect()
{
	DEBUG_PRINT("GTPlatformClient::TryReconnect");
	if(gamegate_conn_state)
	{
		if(gt_conn_state==GTCONNECTING)
		{
			Protocol::Client(this);
			DEBUG_PRINT("GTPlatformClient Reconnect Start");
			return true;
		}
	}
	else 
	{
		if(gt_conn_state==GTCONNECTING)
		{
			SET_GT_CONN_STATE(GTDISCONNECTED);
			DEBUG_PRINT("GTPlatformClient TryReconnect abandon");
			return true;
		}
	}
	DEBUG_PRINT("GTPlatformClient::TryReconnect gamegate state %d gtstate %d err",gamegate_conn_state,gt_conn_state);
	return false;
}

const Protocol::Manager::Session::State* GTPlatformClient::GetInitState() const
{
	return &state_GTPlatformClient;
}


int GTPlatformClient::SendInitMsg()
{
	AnnounceZoneidToIM msg;
	msg.aid = aid;
	msg.zoneid = zoneid;
	msg.boottime = gamesynctime;
	RawSend(&msg);
	return 0;
}

int GTPlatformClient::OnInitMsgResp(int64_t gtboottime,int code)
{
	SetGTBootTime(gtboottime);
	if(code == RET_OK)
		SetGTSessionState(true);
	else
	{
		Log::log(LOG_ERR, "GTPlatformClient::OnInitMsgResp code = %d err",code);
		DEBUG_PRINT("GTPlatformClient::OnInitMsgResp code = %d err",code);
		return -1;
	}
	GTManager::Instance()->SyncGameOnlineTOGT();
	GTManager::Instance()->Initialize();
	TeamCache::GetInstance()->OnGTConnect();
	return 0;
}

int GTPlatformClient::GtStateChange(GT_CONNECTION_STATE target)
{
	if(target == GTCONNECTED) 
	{
		if(gt_conn_state==GTCONNECTING)
		{
			SET_GT_CONN_STATE(GTCONNECTED);
			SendInitMsg();
			return 0;
		}
	}
	else if( target == GTDISCONNECTED)
	{
		if(gt_conn_state!= GTDISCONNECTED)
		{
			SET_GT_CONN_STATE(GTDISCONNECTED);
			SetGTSessionState(false);
			TeamCache::GetInstance()->OnGTDisconnect();
			return 0;
		}
	}

	Log::log(LOG_ERR, "GTPlatformClient::GtStateChange state err gt_conn_state=%d,target=%d", gt_conn_state,target);
	return -1;

}

void GTPlatformClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("GTPlatformClient::OnAddSession");
	if (++gt_connection_count>1)//no affection to gt_conn_state
	{
		Close(sid);
		return;
	}
	this->sid = sid;
	backoff = BACKOFF_INIT;
	if(GtStateChange(GTCONNECTED))
	{
		GTClose();
		return;
	}	
	StateCheck(GTCONNECT);
}

void GTPlatformClient::OnDelSession(Session::ID sid, int status)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("GTPlatformClient::OnDelSession, status=0x%d", status);
/*	conn_state = false;
	Reconnect();*/
	--gt_connection_count;
	if (this->sid != sid)//no affection to gt_conn_state
		return;
	GtStateChange(GTDISCONNECTED);
	StateCheck(GTDISCONNECT);
}

void GTPlatformClient::OnAbortSession(const SockAddr &sa)
{
	Thread::Mutex::Scoped l(locker_state);
	DEBUG_PRINT("GTPlatformClient::OnAbortSession");
/*	conn_state = false;
	Reconnect();*/
	GtStateChange(GTDISCONNECTED);
	StateCheck(GTDISCONNECT);
}

void GTPlatformClient::OnCheckAddress(SockAddr &sa) const
{
	//TODO
}

int GTPlatformClient::GameGateStateChange(bool target)
{
	if(GTManager::Instance()->IsGTOpen()!=true)
		return 0;
	if(gamegate_conn_state!=target)
	{
		gamegate_conn_state=target;
//		CalcPendingGamegatestateChanges();
		return 0;
	}
	else
	{
		Log::log(LOG_ERR, "GTPlatformClient::GameGateStateChange, state err gamegate_conn_state=%d,", gamegate_conn_state);
		return -1;
	}
}

int GTPlatformClient::OnGameGateSync()
{
	DEBUG_PRINT("GTPlatformClient::OnGameGateSync");
	if(GameGateStateChange(true))
		return -1;
	SetGameSyncTime();
	StateCheck(GAMEGATECONNECT);
	return 0;
}

int GTPlatformClient::OnGameGateDisconnect()
{
	DEBUG_PRINT("GTPlatformClient::OnGameGateDisconnect");
	if(GameGateStateChange(false))
		return -1;
	StateCheck(GAMEGATEDISCONNECT);
	return 0;
}

int GTPlatformClient::StateCheck(SESSION_OPERATION oper)
{
	if(GTManager::Instance()->IsGTOpen()!=true)
		return 0;
	DEBUG_PRINT("GTPlatformClient::StateCheck");
	//check argument
	switch(oper)
	{
	case GAMEGATECONNECT:
		if(gamegate_conn_state != true)
			return -1;
		break;
	case GAMEGATEDISCONNECT:
		if(gamegate_conn_state != false)
			return -1;
		break;
	case GTCONNECT:
		if(gt_conn_state != GTCONNECTED)
			return -1;
		break;
	case GTDISCONNECT:
		if(gt_conn_state != GTDISCONNECTED)
			return -1;
		break;
	default:
		return -1;
		break;
	}

	//state transition
	switch(oper)
	{
	case GAMEGATECONNECT:
		if(gt_conn_state ==GTDISCONNECTED)
		{
			GTConnect();
		}
		break;
	case GAMEGATEDISCONNECT:
		if(gt_conn_state ==GTCONNECTED)
		{
			DEBUG_PRINT("GTPlatformClient::StateCheck call GTClose gt_conn_state ==GTCONNECTED");
			GTClose();
		}
		break;
	case GTCONNECT:
		if(gamegate_conn_state == false) 
		{
		/*	if(gamegatestate_changes_during_gtstate_transforming%2 !=1)
				DEBUG_PRINT("GTPlatformClient::StateCheck gamegatestate_changes=%d unmatach",gamegatestate_changes_during_gtstate_transforming);
			 ClearGamegatestateChanges();
			 DEBUG_PRINT("GTPlatformClient::StateCheck call GTClose gamegate_conn_state false");*/
			 GTClose();
		}
		break;
	case GTDISCONNECT:
		if(gamegate_conn_state == true)
		{
		/*	if(gamegatestate_changes_during_gtstate_transforming%2 !=1)
				DEBUG_PRINT("GTPlatformClient::StateCheck gamegatestate_changes=%d unmatach",gamegatestate_changes_during_gtstate_transforming);
			 ClearGamegatestateChanges();*/
			 GTConnect();
		}
		break;
	}

	//end state check
	if(( gamegate_conn_state == true && gt_conn_state ==GTCONNECTED)\
		||( gamegate_conn_state == false && gt_conn_state ==GTDISCONNECTED)\
		||(gt_conn_state == GTDISCONNECTING)\
		||(gt_conn_state == GTCONNECTING) )
	{
		return 0;
	}
	
	else
	{
		Log::log(LOG_ERR, "GTPlatformClient::StateCheck end state err gamegate_conn_state=%d,gt_conn_state=%d", gamegate_conn_state,gt_conn_state);
		return -1;
	}
}

};
