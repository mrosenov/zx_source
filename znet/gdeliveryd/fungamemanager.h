#ifndef __GNET_FUNGAME_MANAGER_H
#define __GNET_FUNGAME_MANAGER_H

#include <map>
#include "itimer.h" 
#include "fungamebase.h" 
#include "landlordgame.h"
#include "gfungameinfo"

namespace GNET
{

struct PlayerFunGameInfo
{
	int roleid;
	int64_t money;
	char game_type;
	short roomid;

	int gsid;
	bool is_active;
	bool is_load;
	bool is_dirty;
	PlayerFunGameInfo():roleid(0),money(0),game_type(GAME_TYPE_NO),roomid(-1),is_active(false),is_load(false),is_dirty(false){}
};

class LandlordGameManager : public BaseGameManager
{
protected:
	unsigned char GetGameType() { return GAME_TYPE_LANDLORD; }
	virtual void Init()
	{
		for(int i= 0; i<MAX_ROOM_SUM; ++i)
		{
			Room *room = new LandLordRoom(i);
			_roomlist[i] = room;
		}
	}

public:
	static LandlordGameManager* GetInstance() { static LandlordGameManager instance; return &instance; }
};

class CardGameManager : public BaseGameManager
{
protected:
	unsigned char GetGameType() { return GAME_TYPE_CARD; }
public:
	static CardGameManager* GetInstance() { static CardGameManager instance; return &instance; }
};

class FunGameManager: public IntervalTimer::Observer 
{
public:
	typedef std::map<int, BaseGameManager*> GAME_MANAGER_MAP;
	typedef std::map<int, BaseGameManager*>::iterator GAME_MANAGER_MAP_IT;
	typedef std::map<int, PlayerFunGameInfo> ROLE_INFO_MAP;/* key roleid */
	typedef std::map<int, PlayerFunGameInfo>::iterator ROLE_INFO_MAP_IT;


	void Init()
	{
		RegisterManager();

		GAME_MANAGER_MAP_IT it = _manager_map.begin();
		for(	; it!= _manager_map.end(); ++it)
		{
			(it->second)->Init();
		}

		_is_active = true;
		IntervalTimer::Attach(this,2000000/IntervalTimer::Resolution());
	}

	bool Update();

	bool CheckCmd(char* data, unsigned int size);
	void HandleClientCmd(int roleid, char* data, int size, PlayerInfo * pinfo, GRoleInfo * role);

	bool OnPlayerLogin(int roleid, unsigned int linksid, unsigned int localsid, int gameid);
	void OnPlayerLogout(int roleid);
	void OnGSLeave(int gsid);

	void SetRoomInfo(int roleid, char game_type, short roomid);
	void SetPlayerGS(int roleid, int gsid);
	void DeletePlayer(int roleid, char game_type, short roomid);

	bool IsActive() { return _is_active; }
	static FunGameManager& GetInstance() { static FunGameManager instance; return instance; }
	void OnLoad(int roleid, const GFunGameInfo &fg_info);
	void OnLoadScore(int ret, int roleid, int gametype, const Octets & score);
	void OnSyncFunGame(int retcode, int roleid);
	void OnSyncGameScore(int retcode, int gametype, int roleid);
private:
	int _update_cursor;
	void RegisterManager()
	{
		_manager_map[GAME_TYPE_LANDLORD] = LandlordGameManager::GetInstance();
		_manager_map[GAME_TYPE_CARD] = CardGameManager::GetInstance();

		//_game_types.insert(GAME_TYPE_LANDLORD);
		//_game_types.insert(GAME_TYPE_CARD);
	}

	FunGameManager():_update_cursor(0),_is_active(false) { }
	BaseGameManager * GetFunManager(int gametype)
	{
		GAME_MANAGER_MAP_IT it = _manager_map.find(gametype);
		if (it != _manager_map.end())
			return it->second;
		else
			return NULL;
	}
private:
	bool _is_active;
	//std::set<int> _game_types;
	GAME_MANAGER_MAP _manager_map;
	ROLE_INFO_MAP  _all_actors;
};

};

#endif

