#include "fungamemanager.h" 
#include "fungamepacket.h" 
#include "fungameprotocol.h" 
#include "fungameprotocol_imp.h" 
#include "gdeliveryserver.hpp" 
#include "gproviderserver.hpp" 
#include "s2cfungamedatasend.hpp"
#include "dbfungameputinfo.hrp"
#include "dbfungamegetinfo.hrp"
#include "gamedbclient.hpp"

namespace GNET
{

bool FunGameManager::Update()
{
	ROLE_INFO_MAP_IT it = _all_actors.lower_bound(_update_cursor), ie = _all_actors.end();
	for (int i=0; it!=ie&&i<30;)
	{
		int roleid = it->first;
		bool dirty = it->second.is_dirty;
		if (it->second.is_load && it->second.is_dirty)
		{
			i++;
			DBFunGamePutInfoArg arg(roleid, GFunGameInfo(it->second.money));
			DBFunGamePutInfo * rpc = (DBFunGamePutInfo *)Rpc::Call(RPC_DBFUNGAMEPUTINFO, arg);
			GameDBClient::GetInstance()->SendProtocol(rpc);
			it->second.is_dirty = false;
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
		if (!dirty && !it->second.is_active)
		{
/*
			for (git = _manager_map.begin(); git != _manager_map.end(); ++git)
				(git->second)->ErasePlayer(roleid);
*/
			_all_actors.erase(it++);
			LOG_TRACE("FunGameManager erase role %d", roleid);
		}
		else
			++it;
	}
	if (it != ie)
		_update_cursor = it->first;
	else
		_update_cursor = 0;

	GAME_MANAGER_MAP_IT git = _manager_map.begin();
	for(	; git!= _manager_map.end(); ++git)
	{
		(git->second)->Update();
	}
	return true;
}

void FunGameManager::OnSyncFunGame(int retcode, int roleid)
{
	if (retcode != ERR_SUCCESS)
	{
		LOG_TRACE("FunGameManager faild to sync fungame for role %d", roleid);
		ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
		if (it != _all_actors.end())
			it->second.is_dirty = true;
	}
}

void FunGameManager::OnSyncGameScore(int retcode, int gametype, int roleid)
{
	BaseGameManager * bgm = GetFunManager(gametype);
	if (bgm != NULL)
		bgm->OnSyncToDB(retcode, roleid);
}
	
bool FunGameManager::OnPlayerLogin(int roleid, unsigned int linksid, unsigned int localsid, int gameid)
{
	ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
/*
	if(it != _all_actors.end())
	{
		PlayerFunGameInfo &rginfo = it->second;
		if(rginfo.is_active==true) 
		{
			Log::log(LOG_INFO,"gdelivery::fungamemanager: OnPlayerLogin roleid=%d\n", roleid);
			return false;
		}

		GAME_MANAGER_MAP_IT git = _manager_map.find(rginfo.game_type);
		if(git!=_manager_map.end())
		{
			return (git->second)->OnPlayerLogin(roleid, rginfo.roomid, linksid, localsid, gameid);
		}
	}
*/
	if (it != _all_actors.end() && it->second.is_load)
		it->second.is_active = true;
	else
	{
		PlayerFunGameInfo player;
		player.roleid = roleid;
		player.is_active = true;
		_all_actors[roleid] = player;
		DBFunGameGetInfo *rpc = (DBFunGameGetInfo *)Rpc::Call(RPC_DBFUNGAMEGETINFO, DBFunGameGetInfoArg(roleid));
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	return true;
}

void FunGameManager::OnLoad(int roleid, const GFunGameInfo &fg_info)
{
	ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
	if (it != _all_actors.end() && !it->second.is_load)
	{
		it->second.money = fg_info.money;
		it->second.is_load = true;
/*
		std::map<int, Octets>::const_iterator it, ie = game_scores.end();
		for (it = game_score.begin(); it != ie; ++it)
		{
			BaseGameManager * git = GetFunManager(it->first);
			if (git != NULL)
				git->OnLoad(roleid, it->second);
		}
*/
	}
}

void FunGameManager::OnLoadScore(int ret, int roleid, int gametype, const Octets & score)
{
	BaseGameManager * bgm = GetFunManager(gametype);
	if (bgm != NULL)
		bgm->OnLoad(ret, roleid, score);
}

void FunGameManager::OnPlayerLogout(int roleid)
{
	ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
	if(it != _all_actors.end())
	{
		PlayerFunGameInfo &rginfo = it->second;
		rginfo.is_active = false;
		BaseGameManager * git = GetFunManager(rginfo.game_type);
		if(git!=NULL)
		{
			git->OnPlayerLogout(roleid, rginfo.roomid);
		}
	}
}

void FunGameManager::SetRoomInfo(int roleid, char game_type, short roomid)
{
	ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
	if (it != _all_actors.end())
	{
		it->second.game_type = game_type;
		it->second.roomid = roomid;
	}
	else
		Log::log(LOG_ERR, "FunGameManager SetRoomInfo for role %d dose not exist", roleid);
//	pinfo.is_active = true;
}

void FunGameManager::OnGSLeave(int gsid)
{
	ROLE_INFO_MAP_IT it, ite = _all_actors.end();
	for (it = _all_actors.begin(); it != ite; ++it)
	{
		if (gsid == it->second.gsid)
			OnPlayerLogout(it->first);
	}
}

void FunGameManager::SetPlayerGS(int roleid, int gsid)
{
	ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
	if (it != _all_actors.end())
		it->second.gsid = gsid;
}

//todo
void FunGameManager::DeletePlayer(int roleid, char game_type, short roomid) {
	ROLE_INFO_MAP_IT it = _all_actors.find(roleid);
	if(it != _all_actors.end())
	{
		if(it->second.game_type != game_type)  return;
		if(it->second.roomid != roomid) return;

//		_all_actors.erase(it);
//		it->second.is_active = false;
	}
}

bool FunGameManager::CheckCmd(char* data, unsigned int size)
{
	using namespace FUNGAME;
	if(size < sizeof(C2S::fun_game_c2s_cmd_header))
		return false;
	return true;
}

void FunGameManager::HandleClientCmd(int roleid, char* data, int size, PlayerInfo * pinfo, GRoleInfo * role)
{
	if(!CheckCmd(data, size)) return;

	using namespace FUNGAME;
	unsigned char game_type = ((const C2S::fun_game_c2s_cmd_header*)data) -> game_type;
	unsigned char cmd = ((const C2S::fun_game_c2s_cmd_header*)data) -> cmd_type;

	LOG_TRACE("FGMgr client cmd roleid %d gametype %d cmdtype %d", roleid, game_type, cmd);
	BaseGameManager *bgm = GetFunManager(game_type);
	if(bgm == NULL) return;

	switch(cmd)
	{
		case C2S::PLAYER_ENTER_HALL://进入游戏大厅
			{//add ly
/*
				ROLE_INFO_MAP_IT rit = _all_actors.find(roleid);
				if(rit != _all_actors.end()) return;
*/
				C2S::CMD::player_enter_hall &cmd = *(C2S::CMD::player_enter_hall *)data;
				if (size != sizeof(cmd)) return;
				bgm->PlayerEnterHall(roleid, pinfo, role);
			}
			break;
		case C2S::PLAYER_LEAVE_HALL:
			{
				C2S::CMD::player_leave_hall &cmd = *(C2S::CMD::player_leave_hall *)data;
				if (size != sizeof(cmd)) return;
				bgm->PlayerLeaveHall(roleid);
			}
			break;
		case C2S::GET_ROOM_LIST: //任何人都可以进行该操作
			{
				C2S::CMD::get_room_list &cmd = *(C2S::CMD::get_room_list *)data;
				if(size != sizeof(cmd)) return;

				bgm->GetRoomList(roleid, pinfo, cmd.index);
			}
			break;
		case C2S::PLAYER_ENTER_ROOM: //没有在游戏中的玩家可以进行此操作 
			{
	/*
				ROLE_INFO_MAP_IT rit = _all_actors.find(roleid);
				if(rit != _all_actors.end()) return;
	*/
				C2S::CMD::player_enter_room &cmd = *(C2S::CMD::player_enter_room *)data;
				if(size != sizeof(cmd)) return;

				bgm->PlayerEnterRoom(roleid, cmd.roomid, cmd.direction);
			}
			break;
		case C2S::PLAYER_REENTER_ROOM: //只有掉线的玩家才可以进行此操作 
			{
				ROLE_INFO_MAP_IT rit = _all_actors.find(roleid);
				if(rit == _all_actors.end() || rit->second.is_active== true) return;

				C2S::CMD::player_reenter_room &cmd = *(C2S::CMD::player_reenter_room *)data;
				if(size != sizeof(cmd)) return;

				bgm->PlayerReenterRoom(roleid, cmd.roomid, pinfo);
			}
			break;
		default:  //正在游戏中的玩家才可以进行该操作 
			{
				ROLE_INFO_MAP_IT rit = _all_actors.find(roleid);
				if(rit == _all_actors.end()) return;

				bgm->HandleClientCmd(roleid, rit->second.roomid, data, size);
			}
			break;
	}
}


}; //end namespace 

