#include "fungamebase.h" 
#include "fungamemanager.h" 
#include "fungamepacket.h" 
#include "fungameprotocol.h" 
#include "fungameprotocol_imp.h" 
#include "timer.h" 
#include "gdeliveryserver.hpp" 
#include "gproviderserver.hpp" 
#include "mapuser.h" 
#include "s2cfungamedatasend.hpp"
#include "dbfungameputscore.hrp"
#include "dbfungamegetscore.hrp"
#include "gamedbclient.hpp"

namespace GNET
{

class RoomControlIdle: public RoomControl
{
public:
	virtual bool PlayerEnterRoom(Room* room, char direction, GameActor & actor) 
	{
		if(!room->CheckDirection(direction)) return false;
		if(room->CheckExist(actor.roleid,direction)) return false;

		bool add = room->AddPlayer(direction, actor);
		if(add && room->PlayerIsFull())
		{
			room->ChangeState(Room::ROOM_STATE_READY);
			room->NotifyGameReady();
			room->OnGameReady();
		}
		return add;
	}

	virtual bool PlayerLeaveRoom(Room* room, int roleid)
	{
		return room->DeletePlayer(roleid);
	}

	virtual bool PlayerOnline(Room* room, int roleid, unsigned int linksid, unsigned int localsid, int gameid)
	{
		return false;
	}

	virtual bool PlayerOffline(Room* room, int roleid, bool & del)
	{
		del = false;
		bool ret = room->DeletePlayer(roleid);
		if (ret)
		{
			del = true;
			unsigned char gametype = room->GetGameType();
			FunGameManager::GetInstance().DeletePlayer(roleid, gametype, room->_roomid);
		}
		return ret;
	}

	virtual bool PlayerAction(Room* room, int roleid, char action, char* data, unsigned int size) 
	{
		return false;
	}

	virtual bool OnTimeout(Room* room, int & role_del) 
	{
		if(room->PlayerIsFull())
		{
			room->ChangeState(Room::ROOM_STATE_READY);
			room->NotifyGameReady();
			room->OnGameReady();
		}
		room->ClearTimeout(Room::TIMEOUT_TYPE_ROOM);

		return true;
	}
};

class RoomControlReady: public RoomControl
{
	virtual bool PlayerEnterRoom(Room* room, char direction, GameActor & actor) 
	{
		//已经准备好的状态， 怎么会有多余的人进入呢？ 以后可以添加旁观模式
		return false;
	}

	virtual bool PlayerLeaveRoom(Room* room, int roleid)
	{
		if(room->DeletePlayer(roleid))
		{
			room->ChangeState(Room::ROOM_STATE_IDLE);
			return true;
		}
		return false;
	}

	virtual bool PlayerOnline(Room* room, int roleid, unsigned int linksid, unsigned int localsid, int gameid)
	{
		return false;
	}

	virtual bool PlayerOffline(Room* room, int roleid, bool & del)
	{
		del = false;
		if(room->DeletePlayer(roleid))
		{
			del = true;
			room->ChangeState(Room::ROOM_STATE_IDLE);
			unsigned char gametype = room->GetGameType();
			FunGameManager::GetInstance().DeletePlayer(roleid, gametype, room->_roomid);
			return true;
		}
		return false;
	}

	virtual bool PlayerAction(Room* room, int roleid, char action, char* data, unsigned int size) 
	{
		using namespace FUNGAME;
		if(action != C2S::PLAYER_BEGIN_GAME)
			return false;

		if(!room->CheckExist(roleid)) return false;

		bool change = room->ChangePlayerState(roleid, GameActor::ACTOR_STATE_READY);
		if(change)
		{
			if(room->PlayerAllReady())
			{
				room->OnGameBegin();
				room->ChangeState(Room::ROOM_STATE_PROCESS);
				room->ChangePlayerState(GameActor::ACTOR_STATE_GAME);
			}
		}
		return change;
	}
};

class RoomControlProcess: public RoomControl
{
	virtual bool PlayerEnterRoom(Room* room, char direction, GameActor & actor) 
	{
		//游戏进行中的状态， 怎么会有多余的人进入呢？ 以后可以添加旁观模式
		return false;
	}

	virtual bool PlayerLeaveRoom(Room* room, int roleid)
	{
		if(!room->CheckExist(roleid))
			return false;
		//有人强行退出， 游戏结束；
		room->PreDeletePlayer(roleid);
		room->OnGameEnd();
		room->DeletePlayer(roleid);
		room->SetTimeOut(Room::TIMEOUT_TYPE_ROOM, Timer::GetTime()+Room::ROOM_TIME);
		room->ChangeState(Room::ROOM_STATE_END);
		return true;
	}

	virtual bool PlayerOnline(Room* room, int roleid, unsigned int linksid, unsigned int localsid, int gameid)
	{
		return false;
	}

	virtual bool PlayerOffline(Room* room, int roleid, bool & del)
	{
		del = false;
		if(room->ChangePlayerState(roleid, GameActor::ACTOR_STATE_OFFLINE))
		{
			// 注册该玩家的超时时间 
			room->SetTimeOut(Room::TIMEOUT_TYPE_OFFLINE, Timer::GetTime()+Room::OFFLINE_TIME, roleid);
			room->ChangeState(Room::ROOM_STATE_OFFLINE);
			return true;
		}
		return false;
	}

	virtual bool PlayerAction(Room* room, int roleid, char action, char* data, unsigned int size) 
	{
		if(roleid != room->GetCurActionRoleid()) return false;
		if(room->OnGameAction(roleid, action, data, size))
		{
			if(room->IsGameEnd())
			{
				LOG_TRACE("RoomControlProcess::PlayerAction:IsGameEnd.roomid=%d",room->_roomid);
				room->SetTimeOut(Room::TIMEOUT_TYPE_ROOM, Timer::GetTime()+Room::ROOM_TIME);
				room->ChangeState(Room::ROOM_STATE_END);
				room->OnGameEnd();
				return true;
			}
		}
		return false;
	}
};

class RoomControlOffline: public RoomControl
{
	virtual bool PlayerEnterRoom(Room* room, char direction, GameActor & actor) 
	{
		return true;
	}

	virtual bool PlayerLeaveRoom(Room* room, int roleid)
	{
		if(!room->CheckExist(roleid))
			return false;
		room->PreDeletePlayer(roleid);
		room->OnGameEnd();
		room->DeletePlayer(roleid);
		room->SetTimeOut(Room::TIMEOUT_TYPE_ROOM, Timer::GetTime()+Room::ROOM_TIME);
		room->ChangeState(Room::ROOM_STATE_END);
		return true;
/*
		if(room->DeletePlayer(roleid, true))
		{
			room->OnGameEnd();

			room->SetTimeOut(Room::TIMEOUT_TYPE_ROOM, Timer::GetTime()+Room::ROOM_TIME);
			room->ChangeState(Room::ROOM_STATE_END);
		}
		return false;
*/
	}

	virtual bool PlayerOnline(Room* room, int roleid, unsigned int linksid, unsigned int localsid, int gameid)
	{
		if(room->CheckPlayerState(roleid, GameActor::ACTOR_STATE_OFFLINE))
		{
			room->UpdatePlayerInfo(roleid, linksid, localsid, gameid);
			room->ChangePlayerState(roleid, GameActor::ACTOR_STATE_GAME);
			room->ClearTimeout(Room::TIMEOUT_TYPE_OFFLINE, roleid);

			room->OnPlayerOnline(roleid);
			if(room->PlayerAllGame()) 
				room->ChangeState(Room::ROOM_STATE_PROCESS);
			return true;
		}
		return false;
	}

	virtual bool PlayerOffline(Room* room, int roleid, bool & del)
	{
		del = false;
		if(room->ChangePlayerState(roleid, GameActor::ACTOR_STATE_OFFLINE))
		{
			// 注册该玩家的超时时间 
			room->SetTimeOut(Room::TIMEOUT_TYPE_OFFLINE, Timer::GetTime()+Room::OFFLINE_TIME, roleid);
			room->ChangeState(Room::ROOM_STATE_OFFLINE);
			return true;
		}
		return false;
	}

	virtual bool PlayerAction(Room* room, int roleid, char action, char* data, unsigned int size) 
	{
		return false;
	}

	virtual bool OnTimeout(Room* room, int & role_del) 
	{
		int roleid = 0;
		if(room->CheckOfflinePlayer(Timer::GetTime(), roleid))
		{
			role_del = roleid;
			room->OnPlayerOfflineTimeout(roleid);
			room->DeleteOfflinePlayer(roleid);

//			if(room->IsGameEnd())强行结束ly
				room->OnGameEnd();
			room->DeletePlayer(roleid);
			unsigned char gametype = room->GetGameType();
			FunGameManager::GetInstance().DeletePlayer(roleid, gametype, room->_roomid);
			room->ChangeState(Room::ROOM_STATE_IDLE);
			room->SetTimeOut(Room::TIMEOUT_TYPE_ROOM, Timer::GetTime()+Room::ROOM_TIME);
			return true;
		}

		return false;
	}
};

class RoomControlEnd: public RoomControl
{
	virtual bool PlayerEnterRoom(Room* room, char direction, GameActor & actor) 
	{
		// 结束状态， 持续固定时间后， 恢复成Idle状态；
		return false;
	}

	virtual bool PlayerLeaveRoom(Room* room, int roleid)
	{
		return room->DeletePlayer(roleid);
	}

	virtual bool PlayerOnline(Room* room, int roleid, unsigned int linksid, unsigned int localsid, int gameid)
	{
		return false;
	}

	virtual bool PlayerOffline(Room* room, int roleid, bool & del)
	{
		del = false;
		bool ret = room->DeletePlayer(roleid);
		if (ret)
		{
			del = true;
			unsigned char gametype = room->GetGameType();
			FunGameManager::GetInstance().DeletePlayer(roleid, gametype, room->_roomid);
		}
		return ret;
	}

	virtual bool PlayerAction(Room* room, int roleid, char action, char* data, unsigned int size) 
	{
		return false;
	}

	virtual bool OnTimeout(Room* room, int & role_del) 
	{
		room->ChangeState(Room::ROOM_STATE_IDLE);
		room->Reset();
		room->SetTimeOut(Room::TIMEOUT_TYPE_ROOM, Timer::GetTime()+Room::ROOM_TIME);
		return true;
	}
};

RoomControl * Room::_room_control[Room::ROOM_STATE_SUM] =
{
	new RoomControlIdle,
	new RoomControlReady,
	new RoomControlProcess,
	new RoomControlOffline,
	new RoomControlEnd,
};

bool Room::AddPlayer(char dir, GameActor & actor)
{
/*
	GameActor &actor = _actors[roleid]; 
	actor.roleid = roleid;
	actor.rolename = rolename;
	actor.gender = gender;
	actor.linksid = linksid;
	actor.localsid = localsid;
	actor.gameid = gameid;
	actor.direction = direction;  	
	actor.state = GameActor::ACTOR_STATE_ENTER;
*/
	int roleid = actor.roleid;
	actor.state = GameActor::ACTOR_STATE_ENTER;
	actor.direction = dir;

	_actors[roleid] = &actor;

	__NotifyRoomInfo(roleid);
//	__PlayerEnterRoom(actor); 改到外层大厅广播

	unsigned char gametype = GetGameType();
	FunGameManager::GetInstance().SetRoomInfo(roleid, gametype, _roomid);
	OnAddPlayer(roleid, dir);
	return true;
}

bool Room::DeletePlayer(int roleid) 
{ 
	LOG_TRACE("Room::DeletePlayer.roomid=%d,roleid=%d",_roomid,roleid);
	ActorMapIt it = _actors.find(roleid);
	if(it == _actors.end())  return false;

//	unsigned char gametype = GetGameType();
//	FunGameManager::GetInstance().SetPlayer(roleid, gametype, -1);
	OnDeletePlayer(roleid);

//	__PlayerLeaveRoom(roleid);改为在外层大厅广播
	_actors.erase(it);
	return true;
}

bool Room::UpdatePlayerInfo(int roleid, unsigned int linksid,unsigned int localsid, int gameid)
{
	ActorMapIt it = _actors.find(roleid);
	if(it != _actors.end())
	{
		it->second->linksid = linksid;
		it->second->localsid = localsid;
		it->second->gameid = gameid;
		FunGameManager::GetInstance().SetPlayerGS(roleid, gameid);

		__NotifyRoomInfo(roleid);
	}
	return false;
}

bool Room::ChangePlayerState(char state)
{ 
	for(ActorMapIt it = _actors.begin(); it != _actors.end(); ++it)
	{
		it->second->state = state;
		__PlayerStateChange(*(it->second));
	}
	return true;
}
bool Room::ChangePlayerState(int roleid, char state)
{ 
	ActorMapIt it = _actors.find(roleid);
	if(it != _actors.end())
	{
		it->second->state = state;
		__PlayerStateChange(*(it->second));
		return true;
	}
	return false;
}

void Room::NotifyGameReady()
{
	__NotifyGameReady();
}

bool Room::CheckPlayerState(int roleid, char state)
{ 
	ActorMapIt it = _actors.find(roleid);
	if(it != _actors.end())
	{
		if(it->second->state == state)  return true; 
	}
	return false;
}

void Room::SetTimeOut(char timetype, int timeout, int roleid) 
{
	if(timetype < _time_type) return;

	switch(timetype)
	{
	case TIMEOUT_TYPE_ACT:
		{
			_time_type = timetype;
			_timeout = timeout;
			_cur_action_roleid = roleid;	
		}
		break;
	case TIMEOUT_TYPE_OFFLINE:
		{
			_time_type = timetype;
			_timeout = 0;
			_offline_actors[roleid] = timeout;
		}
		break;
	case TIMEOUT_TYPE_ROOM:
		{
			_time_type = timetype;
			_timeout = timeout;
		}
		break;
	default:
		break;
	}
}

void Room::ClearTimeout(char timetype, int roleid)
{
	if(timetype !=  _time_type) return;

	switch(timetype)
	{
	case TIMEOUT_TYPE_ACT:
	case TIMEOUT_TYPE_ROOM:
		{
			_time_type = 0;
			_timeout = 0;
			_cur_action_roleid = 0;	
		}
		break;
	case TIMEOUT_TYPE_OFFLINE:
		{
			DeleteOfflinePlayer(roleid);
			if( _offline_actors.empty())
			{
				_time_type = 0;
				_timeout = 0;
			}
		}
		break;
	default:
		break;
	}
}

bool Room::CheckOfflinePlayer(const int &now, int &roleid)
{
	std::map<int, int>::iterator it = _offline_actors.begin();
	for( ; it!=_offline_actors.end(); ++it)
	{
		if(it->second < now)
		{
			roleid = it->first;
			return true;
		}
	}
	return false;
}

void Room::DeleteOfflinePlayer(int roleid)
{
	std::map<int, int>::iterator it = _offline_actors.find(roleid);
	if(it!=_offline_actors.end())
	{
		_offline_actors.erase(it);
	}	
}

void Room::CheckTimeout(const int& now, int & role_del)
{
	if(_time_type==0) return;
	if(_timeout >= now) return;
	switch(_time_type)
	{
	case TIMEOUT_TYPE_ACT:
		{
			FUNGAME::fun_game_packet_wrapper cmd;
			OnPlayerActTimeout(_cur_action_roleid, cmd);
			if (cmd.size())
			{
				using namespace FUNGAME;
				unsigned char cmd_type = ((const C2S::fun_game_c2s_cmd_header*)(char*)cmd.data()) -> cmd_type;
				PlayerAction(_cur_action_roleid, cmd_type, (char*)cmd.data(), cmd.size());
			}
		}
		break;
	case TIMEOUT_TYPE_OFFLINE:
		{
			//room 必须处于 ROOM_STATE_OFFLINE 状态
			RoomTimeout(role_del);
		}
		break;
	case TIMEOUT_TYPE_ROOM:
		{
			RoomTimeout(role_del);
		}
		break;
	default:
		break;
	}
}

bool Room::OnPlayerLogin(int roleid, unsigned int linksid, unsigned int localsid, int gameid)
{
	ActorMapIt it = _actors.find(roleid);
	if(it == _actors.end() || it->second->state != GameActor::ACTOR_STATE_OFFLINE) return false;
	
	//这个时候也要更新玩家的基本信息 
	it->second->linksid = linksid;
	it->second->localsid = localsid;
	it->second->gameid = gameid;
	FunGameManager::GetInstance().SetPlayerGS(roleid, gameid);

	__SendReenterInvite(roleid);
	return true;
}

void Room::OnPlayerLogout(int roleid)
{
	bool b;
	PlayerOffline(roleid, b);
}

void Room::SendPlayerData(int roleid, char* data, int size)
{
	ActorMapIt it = _actors.find(roleid);
	if(it != _actors.end())
	{
		S2CFunGamedataSend protocol(it->second->roleid, it->second->localsid, Octets(data, size));
		GDeliveryServer::GetInstance()->Send(it->second->linksid, protocol);
	}
}

void Room::MultiBroadCast(char* data, int size)
{
	if(_actors.empty()) return;

	S2CFunGamedataSend protocol(0, 0, Octets(data, size));
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	for(ActorMapIt it=_actors.begin(); it!=_actors.end(); ++it)
	{
		protocol.roleid = it->second->roleid;
		protocol.localsid = it->second->localsid;
		dsm->Send(it->second->linksid, protocol);
	}
}

void Room::MultiBroadCast(char* data, int size, int except_id)
{
	if(_actors.empty()) return;

	S2CFunGamedataSend protocol(0, 0, Octets(data, size));
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	for(ActorMapIt it=_actors.begin(); it!=_actors.end(); ++it)
	{
		if(it->second->roleid == except_id) continue;

		protocol.roleid = it->second->roleid;
		protocol.localsid = it->second->localsid;
		dsm->Send(it->second->linksid, protocol);
	}
}

void Room::__NotifyRoomInfo(int roleid)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::notify_room_info>::From(packet, gametype, this);
	SendPlayerData(roleid, (char*)packet.data(), packet.size());
}
	
void Room::__PlayerEnterRoom(const GameActor& actor)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_enter_room>::From(packet, gametype, _roomid, actor);
	MultiBroadCast((char*)packet.data(), packet.size(), actor.roleid);
}

void Room::__PlayerLeaveRoom(int roleid)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_leave_room>::From(packet, gametype, roleid, _roomid);
	MultiBroadCast((char*)packet.data(), packet.size());
}

void Room::__PlayerStateChange(GameActor& actor)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_state_change>::From(packet, gametype, _roomid, actor);
	MultiBroadCast((char*)packet.data(), packet.size());
}

void Room::__NotifyGameReady()
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::room_game_is_ready>::From(packet, gametype, _roomid);
	MultiBroadCast((char*)packet.data(), packet.size());
}

void Room::__SendReenterInvite(int roleid)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_reenter_invite>::From(packet, gametype, _roomid);
	SendPlayerData(roleid, (char*)packet.data(), packet.size());
}

void BaseGameManager::Update()
{
	time_t now = Timer::GetTime();
	int role_to_del = 0;
	for(unsigned int i=0 ; i<MAX_ROOM_SUM; ++i)
	{
		role_to_del = 0;
		(_roomlist[i])->CheckTimeout(now, role_to_del);
		if (role_to_del != 0)
		{
			PlayerLeaveRoom(role_to_del, i);
			PlayerLeaveHall(role_to_del);
	/*
			unsigned char gametype = GetGameType();
			FunGameManager::GetInstance().SetPlayer(roleid, gametype, -1);
			HallMemberIt it = _total_members.find(roleid);
			if (it != _total_members.end())
			{
				__PlayerLeaveRoom(roomid, roleid);
				it->second.state = GameActor::ACTOR_STATE_OFFLINE;
				_hall_members[roleid] = it->second;
				//__NotifyHallMembers(roleid);
			}	
	*/
		}
	}
	HallMemberIt it = _total_members.lower_bound(_update_cursor), ie = _total_members.end();
	for (int i=0; it!=ie&&i<30;)
	{
		int roleid = it->first;
		bool dirty = it->second.IsDirty();
		if (it->second.is_load && it->second.IsDirty())
		{
			i++;
			GFunGameScore gscore;
			it->second.Dump(gscore);
			DBFunGamePutScoreArg arg(roleid, GetGameType());
			arg.fungame_score.swap(Marshal::OctetsStream()<<gscore);
			DBFunGamePutScore * rpc = (DBFunGamePutScore *)Rpc::Call(RPC_DBFUNGAMEPUTSCORE, arg);
			GameDBClient::GetInstance()->SendProtocol(rpc);
			it->second.SetDirty(false);
		}
/*
		GAME_MANAGER_MAP_IT git = _manager_map.begin();
		for (; git != _manager_map.end(); ++git)
		{
			if ((git->second)->IsDirty(roleid))
			{
				dirty = true;
				(git->second)->SyncToDB(roleid);
			}
		}
*/
		if (!dirty && it->second.state == GameActor::ACTOR_STATE_NILL)
		{
/*
			for (git = _manager_map.begin(); git != _manager_map.end(); ++git)
				(git->second)->ErasePlayer(roleid);
*/
			_total_members.erase(it++);
			LOG_TRACE("FunGameBase[%d]erase role %d",GetGameType(), roleid);
		}
		else
			++it;
	}
	if (it != ie)
		_update_cursor = it->first;
	else
		_update_cursor = 0;
}

void BaseGameManager::GetRoomList(int roleid, PlayerInfo* pinfo, char index)
{
	if(!CheckGetRoomIndex(index)) return;
	int begin = index * GET_ROOM_PER;
	int end = (index + 1) * GET_ROOM_PER;
	if(end > MAX_ROOM_SUM) end = MAX_ROOM_SUM; 
	char size = (char)(end - begin);
	if(size <= 0) return;

	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::get_room_list>::From(packet, GetGameType(),  &(_roomlist[begin]), size);

	S2CFunGamedataSend protocol(roleid, pinfo->localsid, Octets(packet.data(), packet.size()));
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	dsm->Send(pinfo->linksid, protocol);
}

void BaseGameManager::SendPlayerData(int roleid, char* data, int size)
{
	HallMemberIt it = _total_members.find(roleid);
	if(it != _total_members.end())
	{
		S2CFunGamedataSend protocol(it->second.roleid, it->second.localsid, Octets(data, size));
		GDeliveryServer::GetInstance()->Send((it->second).linksid, protocol);
	}
}

void BaseGameManager::__NotifyHallMembers(int roleid)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::notify_hall_members>::From(packet, gametype, _total_members);
	SendPlayerData(roleid, (char*)packet.data(), packet.size());
}

void BaseGameManager::MultiBroadCast(char* data, int size, int except_id)//大厅成员广播
{
	if(_total_members.empty()) return;

	S2CFunGamedataSend protocol(0, 0, Octets(data, size));
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	for(HallMemberIt it=_total_members.begin(); it!=_total_members.end(); ++it)
	{
		if (it->second.state == GameActor::ACTOR_STATE_NILL) continue;
		if(except_id != 0 && it->second.roleid == except_id) continue;

		protocol.roleid = it->second.roleid;
		protocol.localsid = it->second.localsid;
		dsm->Send((it->second).linksid, protocol);
	}
}

void BaseGameManager::OnLoad(int ret, int roleid, const Octets & score)
{
	HallMemberIt it = _total_members.find(roleid);
	if (it == _total_members.end())
		return;

	if (it->second.state != GameActor::ACTOR_STATE_LOADING)
	{
		Log::log(LOG_ERR, "BaseGameManager[%d] :: load role %d repeatly",GetGameType(), roleid);
		return;
	}
	if (ret != ERR_SUCCESS)
	{
		Log::log(LOG_ERR, "BaseGameManager[%d]::load score for role %d error[%d]", GetGameType(), roleid, ret);
		it->second.state = GameActor::ACTOR_STATE_NILL;
		return;
	}
	if (score.size() != 0)
	{
		try
		{
			GFunGameScore gscore;
			Marshal::OctetsStream(score) >> gscore;
			it->second.Load(gscore);
		}
		catch (...)
		{
			Log::log(LOG_ERR, "BaseGameManager[%d]::load score for role %d unmarshal error", GetGameType(), roleid);
			it->second.state = GameActor::ACTOR_STATE_NILL;
			return;
		}
	}
	else
		it->second.Clear();
	it->second.is_load = true;
	LOG_TRACE("BaseGameManager[%d]::load score for role %d", GetGameType(), roleid);
	DoEnterHall(it->second);
}
/*	
bool BaseGameManager::IsDirty(int roleid)
{
	HallMembersIt it = _total_members.find(roleid)
	if (it == _total_members.end())
		return false;
	else
		return it->second.IsDirty();
}
*/
/*
void BaseGameManager::SyncToDB(int roleid)
{
	HallMembersIt it = _total_members.find(roleid)
	if (it == _total_members.end())
		return;
	else
	{
		GFunGameScore score;
		it->Dump(score);
		DBFunGamePutScoreArg arg(roleid, GetGameType());
		arg.fungame_score.swap(Marshall::OctetsStream()<<score);
		DBFunGamePutScore *rpc = (DBFunGamePutScore *)Rpc::Call(RPC_DBFUNGAMEPUTSCORE, arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		it->SetDirty(false);
		LOG_TRACE("BaseGameManager[%d] sync to DB for role %d", GetGameType(), roleid);
	}
}
*/		
void BaseGameManager::OnSyncToDB(int retcode , int roleid)
{
	if (retcode != ERR_SUCCESS)
	{
		LOG_TRACE("BaseGameManager[%d] fail to sync DB for role %d", GetGameType(), roleid);
		HallMemberIt it = _total_members.find(roleid);
		if (it != _total_members.end())  //可能刚好db返回前 玩家退出了 记录删除了
			it->second.SetDirty(true);
	}
}

void BaseGameManager::__PlayerEnterHall(const GameActor& actor)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_enter_hall>::From(packet, gametype, actor);
	MultiBroadCast((char*)packet.data(), packet.size(), actor.roleid);
}

void BaseGameManager::DoEnterHall(GameActor & actor)
{
	int roleid = actor.roleid;
	actor.state = GameActor::ACTOR_STATE_HALL;
	LOG_TRACE("role %d enterhall, hall size %d", roleid, _total_members.size());
	__NotifyHallMembers(roleid);
	__PlayerEnterHall(actor);
	FunGameManager::GetInstance().SetRoomInfo(roleid, GetGameType(), -1);
	FunGameManager::GetInstance().SetPlayerGS(roleid, actor.gameid);
}

bool BaseGameManager::PlayerEnterHall(int roleid, PlayerInfo* pinfo, GRoleInfo * role)
{
	if (_total_members.size() >= FUN_HALL_MEMBER_MAX)
		return false;
	HallMemberIt it = _total_members.find(roleid);
	if (it != _total_members.end() && it->second.state != GameActor::ACTOR_STATE_NILL)
		return false;
	GameActor & actor = _total_members[roleid];
	actor.roleid = roleid;
	actor.rolename = pinfo->name;
	actor.gender = role->gender;
	actor.linksid = pinfo->linksid;
	actor.localsid = pinfo->localsid;
	actor.gameid = pinfo->gameid;
	if (actor.is_load)
	{
		DoEnterHall(actor);
		return true;
	}
	else
	{
		LOG_TRACE("role %d enter fungamebase[%d] hall load score", roleid, GetGameType());
		actor.state = GameActor::ACTOR_STATE_LOADING;
		DBFunGameGetScoreArg arg(roleid, GetGameType());
		DBFunGameGetScore * rpc = (DBFunGameGetScore *)Rpc::Call(RPC_DBFUNGAMEGETSCORE, arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return true;
	}
/*
	GameActor &actor = _total_members[roleid]; 
	actor.roleid = roleid;
	actor.rolename = pinfo->name;
	actor.gender = role->gender;
	actor.linksid = pinfo->linksid;
	actor.localsid = pinfo->localsid;
	actor.gameid = pinfo->gameid;
	actor.state = GameActor::ACTOR_STATE_HALL;
	_hall_members[roleid] = _total_members[roleid];
	LOG_TRACE("role %d enterhall, hall size %d", roleid, _total_members.size());
	__NotifyHallMembers(roleid);
	__PlayerEnterHall(actor); ///here

	unsigned char gametype = GetGameType();
	FunGameManager::GetInstance().SetPlayer(roleid, gametype, -1);
	//OnAddPlayer(roleid, direction);
*/
}

void BaseGameManager::__PlayerLeaveHall(int roleid)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_leave_hall>::From(packet, gametype, roleid);
	MultiBroadCast((char*)packet.data(), packet.size());
}

bool BaseGameManager::PlayerLeaveHall(int roleid)
{
//here
	HallMemberIt it = _total_members.find(roleid);
	if (it == _total_members.end())
		return false;
	if (it->second.state == GameActor::ACTOR_STATE_HALL)
	{
		__PlayerLeaveHall(roleid);
		it->second.state = GameActor::ACTOR_STATE_NILL;
		FunGameManager::GetInstance().DeletePlayer(roleid, GetGameType(), -1);
		return true;
	}
	else if (it->second.state == GameActor::ACTOR_STATE_LOADING)
	{
		it->second.state = GameActor::ACTOR_STATE_NILL;
		return true;
	//防止数据load进来之前玩家就退出大厅
	}
	else
		return false;
}

void BaseGameManager::__PlayerEnterRoom(int roomid, const GameActor& actor)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_enter_room>::From(packet, gametype, roomid, actor);
	MultiBroadCast((char*)packet.data(), packet.size());
}

void BaseGameManager::__PlayerLeaveRoom(int roomid, int roleid)
{
	unsigned char gametype = GetGameType();
	FUNGAME::fun_game_packet_wrapper packet;
	using namespace FUNGAME::S2C;
	CMD::Make<CMD::player_leave_room>::From(packet, gametype, roleid, roomid);
	MultiBroadCast((char*)packet.data(), packet.size());
}

bool BaseGameManager::PlayerEnterRoom(int roleid, int roomid, char direction)
{
	if(!CheckRoomId(roomid)) return false;
	HallMemberIt it = _total_members.find(roleid);
	if (it == _total_members.end() || it->second.state != GameActor::ACTOR_STATE_HALL)
		return false;
	bool ret = _roomlist[roomid]->PlayerEnterRoom(direction, it->second);
	if (ret)
	{
/*
		it->second.state = GameActor::ACTOR_STATE_ENTER;
		it->second.direction = direction;
*/
		__PlayerEnterRoom(roomid, it->second);
	}
	return ret;
}

bool BaseGameManager::PlayerLeaveRoom(int roleid, int roomid)
{
	unsigned char gametype = GetGameType();
	FunGameManager::GetInstance().SetRoomInfo(roleid, gametype, -1);
	HallMemberIt it = _total_members.find(roleid);
	if (it != _total_members.end())
	{
		__PlayerLeaveRoom(roomid, roleid);
		it->second.state = GameActor::ACTOR_STATE_HALL;
		it->second.direction = -1;
		//_hall_members[roleid] = it->second;
		//__NotifyHallMembers(roleid);
	}	
	return true;
}

bool BaseGameManager::PlayerReenterRoom(int roleid, int roomid, PlayerInfo* pinfo)
{
	if(!CheckRoomId(roomid)) return false;

	return _roomlist[roomid]->PlayerOnline(roleid, pinfo->linksid, pinfo->localsid, pinfo->gameid);
}

bool BaseGameManager::OnPlayerLogin(int roleid, int roomid, unsigned int linksid, unsigned int localsid, int gameid)
{
	if(!CheckRoomId(roomid)) return false;

	return _roomlist[roomid]->OnPlayerLogin(roleid, linksid, localsid, gameid);
}

void BaseGameManager::OnPlayerLogout(int roleid, int roomid)
{
	bool del_player = true;
	if(CheckRoomId(roomid))
	{
		if (_roomlist[roomid]->PlayerOffline(roleid, del_player))
		{
			if (del_player)
			{
				PlayerLeaveRoom(roleid, roomid);
/*
				unsigned char gametype = GetGameType();
				FunGameManager::GetInstance().SetPlayer(roleid, gametype, -1);
				HallMemberIt it = _total_members.find(roleid);
				if (it != _total_members.end())
				{
					__PlayerLeaveRoom(roomid, roleid);
					it->second.state = GameActor::ACTOR_STATE_OFFLINE;
					_hall_members[roleid] = it->second;
					//__NotifyHallMembers(roleid);
				}	
*/
			}
		}
	}
	if (del_player)
		PlayerLeaveHall(roleid);
}

void BaseGameManager::HandleClientCmd(int roleid, int roomid, char* data, int size)
{
	if(!CheckRoomId(roomid)) return;

	using namespace FUNGAME;
	unsigned char cmd_type = ((const C2S::fun_game_c2s_cmd_header*)data) -> cmd_type;
	LOG_TRACE("BaseGameManager::HandleCmd.roomid=%d,stat=%d,roleid=%d,cmd=%d",roomid,_roomlist[roomid]->_state,roleid,cmd_type);

	switch(cmd_type)
	{
		case C2S::PLAYER_LEAVE_ROOM:
			{
				C2S::CMD::player_leave_room &cmd = *(C2S::CMD::player_leave_room *)data;
				if(size != sizeof(cmd)) return; 

				if (_roomlist[roomid]->PlayerLeaveRoom(roleid))
				{
					PlayerLeaveRoom(roleid, roomid);
/*
					unsigned char gametype = GetGameType();
					FunGameManager::GetInstance().SetPlayer(roleid, gametype, -1);
					HallMemberIt it = _total_members.find(roleid);
					if (it != _total_members.end())
					{
						__PlayerLeaveRoom(roomid, roleid);
						it->second.state = GameActor::ACTOR_STATE_HALL;
						_hall_members[roleid] = it->second;
						//__NotifyHallMembers(roleid);
					}	
*/
				}
			}
			break;
		default:
			{
				_roomlist[roomid]->PlayerAction(roleid, cmd_type, data, size);
			}
			break;
	}
}


}; //end namespace 

