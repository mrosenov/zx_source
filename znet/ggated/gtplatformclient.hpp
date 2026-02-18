#ifndef __GNET_GTPLATFORMCLIENT_HPP
#define __GNET_GTPLATFORMCLIENT_HPP

#include "protocol.h"
#include "thread.h"

namespace GNET
{

class GTPlatformClient : public Protocol::Manager
{
	static GTPlatformClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	enum GT_CONNECTION_STATE
	{
		GTCONNECTED=0,
		GTDISCONNECTING,
		GTDISCONNECTED,
		GTCONNECTING,
	};
	enum SESSION_OPERATION
	{
		GAMEGATECONNECT=0,
		GAMEGATEDISCONNECT,
		GTCONNECT,
		GTDISCONNECT,
	};
	int gt_conn_state;
	bool conn_state;
	Thread::Mutex	locker_state;
	enum { BACKOFF_INIT = 2, BACKOFF_DEADLINE = 256 };
	size_t		backoff;
	bool gamegate_conn_state;
//	int gamegatestate_changes_during_gtstate_transforming;
	int gt_connection_count;
	time_t gamesynctime;
	int64_t gtboottime;
	bool gt_session_state;
/*	void CalcPendingGamegatestateChanges(){
		if(gt_conn_state == GTDISCONNECTING || gt_conn_state == GTDISCONNECTING)
			gamegatestate_changes_during_gtstate_transforming++;
	}*/
//	void ClearGamegatestateChanges(){gamegatestate_changes_during_gtstate_transforming=0;}
	void Reconnect();
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid) { }
	void OnDelSession(Session::ID sid, int status);
	void OnAbortSession(const SockAddr &sa);
	void OnCheckAddress(SockAddr &) const;
	int StateCheck(SESSION_OPERATION oper);
	int GameGateStateChange(bool target);
	int GtStateChange(GT_CONNECTION_STATE target);
	void SET_GT_CONN_STATE(GT_CONNECTION_STATE target){gt_conn_state=target;}
	bool GTClose(){ SET_GT_CONN_STATE(GTDISCONNECTING);return Close(sid);}
	void GTConnect();
	int SendInitMsg();
	void SetGameSyncTime(){gamesynctime = Timer::GetTime();}
	void SetGTBootTime(int64_t timevalue){gtboottime = timevalue;}
	void SetGTSessionState(bool state){gt_session_state = state;}
	bool RawSend(const Protocol *protocol){return Send(sid, protocol);}
public:
	int zoneid;
	int aid;
	static GTPlatformClient *GetInstance() { return &instance; }
	std::string Identification() const { return "GTPlatformClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GTPlatformClient() : accumulate_limit(0), gt_conn_state(GTDISCONNECTED), conn_state(false),\
		locker_state("GTPlatformClient::locker_state"), backoff(BACKOFF_INIT),gamegate_conn_state(false),\
		gt_connection_count(0),gamesynctime(0),gtboottime(0),gt_session_state(false)
	{
		zoneid=0;aid=0;
	}
	int OnGameGateDisconnect();//{gamegate_conn_state=false; if(conn_state){Close(sid);conn_state=false;}}
	int OnGameGateSync();//{gamegate_conn_state=true; if(conn_state)Close(sid);conn_state=false;}
	int OnInitMsgResp(int64_t gtboottime,int code);
	bool IsGTSessionOK(){return gt_conn_state==GTCONNECTED && gt_session_state==true;}
	bool SendProtocol(const Protocol &protocol) { return SendProtocol(&protocol); }
	bool SendProtocol(const Protocol *protocol) { return IsGTSessionOK() && Send(sid, protocol); }
	bool SendProtocol(Protocol &protocol) { return SendProtocol(&protocol); }
	bool SendProtocol(Protocol *protocol) { return IsGTSessionOK() && Send(sid, protocol); }
	bool TryReconnect();
};

};
#endif
