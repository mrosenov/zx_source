#ifndef __GNET_FUNGAME_BASE_H
#define __GNET_FUNGAME_BASE_H

#include <map>
#include <vector>
#include "itimer.h" 
#include "fungamepacket.h" 
#include "gfungamescore"
#include "localmacro.h"

namespace GNET
{

class PlayerInfo;
class GRoleInfo;

struct GameActor
{
	enum 
	{
		ACTOR_STATE_NILL	= -1, //未进入大厅的状态
		ACTOR_STATE_ENTER	= 0, //进入房间
		ACTOR_STATE_READY 	= 1, //已准备好，等待其他玩家准备好
		ACTOR_STATE_GAME	= 2, //正在游戏中
		ACTOR_STATE_OFFLINE	= 3, //掉线 
		//ACTOR_STATE_EXIT	= 4, //强行退出状态 
		ACTOR_STATE_HALL	= 5, //在大厅中
		ACTOR_STATE_LOADING	= 6, //加载数据中
	};
	int roleid; 
	Octets rolename;
	char gender; //0男 1女
	char direction;  //座位标示
	char state;	
	int64_t score;
	int win_count;
	int lose_count;
	int draw_count;
	int escape_count;
	bool is_dirty;
	bool is_load;

	unsigned int linksid;
	unsigned int localsid;
	int gameid;
	GameActor():roleid(0),gender(0),direction(-1),state(ACTOR_STATE_NILL),score(0),win_count(0),lose_count(0),draw_count(0),escape_count(0),is_dirty(false),is_load(false),linksid(0),localsid(0),gameid(-1){}
	void Load(const GFunGameScore & gscore)
	{
		score = gscore.score;
		win_count = gscore.win_count;
		lose_count = gscore.lose_count;
		draw_count = gscore.draw_count;
		escape_count = gscore.escape_count;
	}
	void Dump(GFunGameScore & gscore)
	{
		gscore.score = score;
		gscore.win_count = win_count;
		gscore.lose_count = lose_count;
		gscore.draw_count = draw_count;
		gscore.escape_count = escape_count;
	}
	void Clear()
	{
		score = 0;
		win_count = 0;
		lose_count = 0;
		draw_count = 0;
		escape_count = 0;
	}
	void SetDirty(int b) { is_dirty = b;}
	bool IsDirty() { return is_dirty; }
	void ChangeScore(int delta)
	{
		score +=  delta;
		SetDirty(true);
	}
	void WinInc()
	{
		win_count++;
		SetDirty(true);
	}
	void LoseInc()
	{
		lose_count++;
		SetDirty(true);
	}
	void DrawInc()
	{
		draw_count++;
		SetDirty(true);
	}
	void EscapeInc()
	{	
		escape_count++;
		SetDirty(true);
	}
};

class Room;
class RoomControl 
{
public:
	virtual bool PlayerEnterRoom(Room* room, char direction, GameActor & actor) = 0; 
	virtual bool PlayerLeaveRoom(Room* room, int roleid) = 0;
	virtual bool PlayerOnline(Room* room, int roleid, unsigned int linksid, unsigned int localsid, int gameid) = 0;
	virtual bool PlayerOffline(Room* room, int roleid, bool & del) = 0; 
	virtual bool PlayerAction(Room* room, int roleid, char action, char* data, unsigned int size) = 0;
	virtual bool OnTimeout(Room* room, int & role_del)  { return false; }
};


class Room
{	
public:
	enum 
	{
		ROOM_STATE_IDLE 	= 0,
		ROOM_STATE_READY 	= 1,
		ROOM_STATE_PROCESS 	= 2, //正在进行中
		ROOM_STATE_OFFLINE 	= 3, //有人掉线
		ROOM_STATE_END 		= 4, //
		ROOM_STATE_SUM 		= 5, //
	};

	enum
	{
		TIMEOUT_TYPE_ACT	= 1,	//玩家动作超时
		TIMEOUT_TYPE_OFFLINE 	= 2,	//玩家掉线超时
		TIMEOUT_TYPE_ROOM 	= 3,	//房间的超时 
	};

	enum
	{
		ACT_TIME	= 30,	//玩家动作超时时间 30秒
		OFFLINE_TIME	= 1,	//玩家掉线超时时间 180秒
		ROOM_TIME	= 2,	//房间的超时 以2秒为单位 
	};


	typedef std::map<int, GameActor *> ActorMap; //key is roleid 
	typedef std::map<int, GameActor *>::iterator  ActorMapIt; 

	short _roomid;
	unsigned char _state;
	ActorMap _actors; //key is roleid 

	bool _game_end;

	//超时相关 
	char _time_type;
	int _timeout;
	int _cur_action_roleid; //当前行动的玩家
	std::map<int, int> _offline_actors; //key is roleid, value is timeout
public:
	static RoomControl* _room_control[ROOM_STATE_SUM];

public:
	Room(short roomid):_roomid(roomid),_state(ROOM_STATE_IDLE),_game_end(0),_time_type(0),_timeout(0),_cur_action_roleid(0) { }
	virtual ~Room() { }

	void Reset()
	{
		_state = ROOM_STATE_IDLE;
		ActorMapIt it = _actors.begin();
		for( 	; it!=_actors.end(); ++it)
			it->second->state = GameActor::ACTOR_STATE_ENTER;

		_game_end = false;

		_time_type = 0;
		_timeout = 0;
		_cur_action_roleid = 0;
		_offline_actors.clear();
	};

	void ChangeState(char state) { LOG_TRACE("Room::ChangeState.roomeid=%d,oldstate=%d,newstate=%d",_roomid, _state, state); _state = state; }
	int GetCurActionRoleid() { return _cur_action_roleid; }
	void SetCurActionRoleid(int roleid) { _cur_action_roleid = roleid; }
	bool IsGameEnd() { return _game_end; }
	void SetGameEnd(bool end) { _game_end = end; }

	bool PlayerIsFull() { return (GetRoomPlayerLimit() == _actors.size()); }
	bool PlayerAllReady() { return PlayerAllSameState(GameActor::ACTOR_STATE_READY); }
	bool PlayerAllGame() { return PlayerAllSameState(GameActor::ACTOR_STATE_GAME); }
	bool PlayerAllSameState(char state)
	{ 
		if(!PlayerIsFull()) return false; 
		for(ActorMapIt it = _actors.begin(); it!=_actors.end(); ++it)
		{
			if(it->second->state != state) 
				return false;
		}
		return true;
	}

public: 
	//状态控制相关 
	bool PlayerEnterRoom(char direction, GameActor & actor) 
	{
		return _room_control[_state]->PlayerEnterRoom(this, direction, actor);
	}

	bool PlayerLeaveRoom(int roleid)
	{
		return _room_control[_state]->PlayerLeaveRoom(this, roleid);
	}

	bool PlayerOnline(int roleid, unsigned int linksid, unsigned int localsid, int gameid)
	{
		return _room_control[_state]->PlayerOnline(this, roleid, linksid, localsid, gameid);
	}	

	bool PlayerOffline(int roleid, bool & del)
	{
		int ret = _room_control[_state]->PlayerOffline(this, roleid, del);
		if (ret)
			OnPlayerOffline(roleid); 
		return ret;
	}	

	bool PlayerAction(int roleid, char action, char* arg, int size)
	{
		return _room_control[_state]->PlayerAction(this, roleid, action, arg, size);
	}

	bool RoomTimeout(int & role_del)
	{
		return _room_control[_state]->OnTimeout(this, role_del);
	}

	//玩家登录，断线相关 
	bool OnPlayerLogin(int roleid, unsigned int linksid, unsigned int localsid, int gameid);
	void OnPlayerLogout(int roleid);

public:
	//超时相关
	void SetTimeOut(char timetype, int timeout, int roleid=0);
	void ClearTimeout(char timetype, int roleid=0);
	bool CheckOfflinePlayer(const int &now, int &roleid);
	void DeleteOfflinePlayer(int roleid);
	void CheckTimeout(const int& now, int & role_del);

public:
	//以下虚函数由子类来实现 
	virtual unsigned char GetGameType() { return GAME_TYPE_NO; }
	virtual unsigned char GetRoomPlayerLimit() { return 3; }
	virtual bool CheckDirection(char direction) { return false; }

	virtual bool OnGameReady() { return false; }
	virtual bool OnGameBegin() { return false; }
	virtual bool OnGameEnd() { return false; }
	virtual bool OnGameAction(int roleid, char action, char* data, unsigned int size) { return false; }

	virtual bool OnPlayerOnline(int roleid) { return false; }
	virtual bool OnPlayerOffline(int roleid) { return false; }

	virtual bool OnPlayerActTimeout(int roleid, FUNGAME::fun_game_packet_wrapper & cmd) { return false; }
	virtual bool OnPlayerOfflineTimeout(int roleid) { return false; }
	
	virtual void OnAddPlayer(int roleid, char direction) { }
	virtual void OnDeletePlayer(int roleid) { }
	virtual void PreDeletePlayer(int roleid) { }

public:
//	bool AddPlayer(int roleid, Octets& rolename, char gender, char direction, unsigned int linksid,unsigned int localsid, int gameid);
	bool AddPlayer(char dir, GameActor & pactor);
//	bool DeletePlayer(int roleid, bool gameing=false);
	bool DeletePlayer(int roleid);
	bool UpdatePlayerInfo(int roleid, unsigned int linksid,unsigned int localsid, int gameid);
	bool ChangePlayerState(int roleid, char state);
	bool ChangePlayerState(char state);
	bool CheckPlayerState(int roleid, char state);
	void NotifyGameReady();

	bool CheckExist(int roleid)
	{
		ActorMapIt it = _actors.find(roleid);
		if(it!=_actors.end()) return true;
		else return false;
	}
	bool CheckExist(int roleid, char direction)
	{
		for(ActorMapIt it= _actors.begin(); it!=_actors.end(); ++it)
		{
			if(it->first == roleid) return true;
			if(it->second->direction == direction) return true;
		}
		return false;
	}

	void SendPlayerData(int roleid, char* data, int size);
	void MultiBroadCast(char* data, int size);
	void MultiBroadCast(char* data, int size, int except_id);

private:
	void __NotifyRoomInfo(int roleid);
	void __PlayerEnterRoom(const GameActor& actor);
	void __PlayerLeaveRoom(int roleid);
	void __PlayerStateChange(GameActor& actor);
	void __NotifyGameReady();
	void __SendReenterInvite(int roleid);

};


class BaseGameManager
{
protected:
	virtual unsigned char GetGameType() { return GAME_TYPE_NO; }

public:
	enum 
	{ 
		MAX_ROOM_SUM = 200, 
		GET_ROOM_PER = 20, 
	};
	typedef std::map<int, GameActor> HallMembers; //key is roleid 
	typedef std::map<int, GameActor>::iterator  HallMemberIt; 

	BaseGameManager():_update_cursor(0) { }
	virtual ~BaseGameManager() 
	{
		for(int i=0; i<MAX_ROOM_SUM; ++i)
		{
			delete _roomlist[i]; 
		}
	}

	virtual void Init()
	{
		for(int i=0; i<MAX_ROOM_SUM; ++i)
		{
			Room *room = new Room(i);
			_roomlist[i] = room;
		}
	}
	bool CheckRoomId(short roomid) { return (roomid < MAX_ROOM_SUM && roomid >= 0); }
	bool CheckGetRoomIndex(char index)
	{	
		if(index * GET_ROOM_PER >= MAX_ROOM_SUM ) return false;
		else return true;
	}

	virtual void Update();
	virtual void GetRoomList(int roleid, PlayerInfo* pinfo, char index);
	virtual bool PlayerEnterHall(int roleid, PlayerInfo* pinfo, GRoleInfo * role);
	virtual bool PlayerLeaveHall(int roleid);
	virtual bool PlayerEnterRoom(int roleid, int roomid, char direction);
	virtual bool PlayerLeaveRoom(int roleid, int roomid);
	virtual bool PlayerReenterRoom(int roleid, int roomid, PlayerInfo* pinfo);
	virtual void HandleClientCmd(int roleid, int roomid, char* data, int size); 

	bool OnPlayerLogin(int roleid, int roomid, unsigned int linksid, unsigned int localsid, int gameid);
	void OnPlayerLogout(int roleid, int roomid); 
	void SendPlayerData(int roleid, char* data, int size);
	void MultiBroadCast(char* data, int size, int except_id=0);
	void OnLoad(int ret, int roleid, const Octets & score);
	bool IsDirty(int roleid);
	void SyncToDB(int roleid);
	void OnSyncToDB(int retcode , int roleid);

protected:
	Room* _roomlist[MAX_ROOM_SUM];
	HallMembers _total_members;//大厅中的全部成员 包含房间中的 _roomlist中为其指针
//	HallMembers _hall_members;//大厅成员 不包含房间的
private:
	int _update_cursor;
	void __NotifyHallMembers(int roleid);
	void __PlayerEnterRoom(int roomid, const GameActor& actor);
	void __PlayerLeaveRoom(int roomid, int roleid);
	void __PlayerEnterHall(const GameActor& actor);
	void __PlayerLeaveHall(int roleid);
	void DoEnterHall(GameActor & actor);
};


};

#endif

