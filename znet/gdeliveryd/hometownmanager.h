#ifndef _GNET_HOMETOWN_MANAGER_H_
#define _GNET_HOMETOWN_MANAGER_H_ 

#include <map>
#include <set>
#include "hometowndef.h"
#include "hometowntemplate.h"
#include "hometownpacket.h"
#include "marshal.h"
#include "ghometowninfo"
#include "hardreference.h"
#include <itimer.h>
#include "gmailsyncdata"

namespace GNET
{
	enum
	{
		STATUS_UNLOAD	= 0,		//没有发加载请求
		STATUS_LOADING	,			//已经发加载请求，还没收到数据库回应
		STATUS_LOADED	,			//已经成功加载, 数据库中没有信息或者加载失败的角色也都认为LOADED
	};

	class HometownPlayer;
	typedef LOCAL::HardReference<HometownPlayer> HometownPlayerRef;

	class HometownPlayer
	{
	public:
		int _roleid;
		Octets _name;
		int64_t _money;					//通用货币数目
		time_t _last_exchgmoney_time;
		std::set<int> _compo_list;		//已经添加的组件id列表
		std::set<int> _friend_list;		//好友列表，不需要存盘
		std::map<int, HometownPlayerRef> _friend_ref_list;
		char _status;
		bool _dirty;

		HometownPlayer():_roleid(0),_money(0),_last_exchgmoney_time(0),_status(STATUS_UNLOAD),_dirty(false){}
		int64_t GetMoney() { return _money; }
		void AddMoney(int64_t money) { _money+=money; SetDirty(true); }
		bool HasFriend(int rid) const { return _friend_list.find(rid) != _friend_list.end(); }
		void AddFriend(int rid) { _friend_list.insert(rid); }
		void DelFriend(int rid)
		{
			_friend_list.erase(rid);
			_friend_ref_list.erase(rid);
		}
		bool HasCompo(int compo_id) const { return _compo_list.find(compo_id) != _compo_list.end(); }
		void AddCompo(int compo_id) { _compo_list.insert(compo_id); SetDirty(true); }
		bool IsUnload() { return _status==STATUS_UNLOAD; }
		bool IsLoading() { return _status==STATUS_LOADING; }
		void SetLoading() { _status = STATUS_LOADING; }
		bool IsLoaded() { return _status==STATUS_LOADED; }
		void SetLoaded() { _status = STATUS_LOADED; }
		void SetDirty(bool dirty) { _dirty = dirty; }
		bool IsDirty() { return _dirty; }
	};

	class HometownManager : public IntervalTimer::Observer
	{
		friend class BaseCompoManager;

		typedef std::map<int, BaseCompoManager *> CompoMap;
		CompoMap _compo_map;

		typedef std::map<int, HometownPlayerRef> PlayerMap;	//记录玩家在家园系统的一些信息
		PlayerMap _player_map;								//自身或有朋友在线的玩家都会在这个map中，只有当引用计数为1时才删除
		PlayerMap _online_map;								//只有在线的玩家才会在这个map中，玩家下线时会删除
		int _update_cursor;

		const HOMETOWN_CONFIG *_config;

		void RegisterCompo(int compo_id, BaseCompoManager *compo)
		{
			_compo_map[compo_id] = compo;
		}
		bool CheckCmd(void *data, int size);

		HometownManager():_update_cursor(0),_config(NULL) {}
	public:
		static HometownManager *HometownManager::GetInstance() 
		{ 
			static HometownManager instance; 
			return &instance;
		}
		BaseCompoManager *GetCompo(int compo_id)
		{
			CompoMap::iterator it = _compo_map.find(compo_id);
			if (it != _compo_map.end()) 
				return it->second;
			else
				return NULL;
		}
		bool Init();
		bool Update();
		void HandleCmd(int roleid, void *data, int size, HOMETOWN::WRAPPER &response);
		bool IsFriend(int alice, int bob) const
		{
			if (alice == bob) return true;
			PlayerMap::const_iterator it = _player_map.find(alice);
			if (it==_player_map.end() || !it->second->HasFriend(bob)) return false;
			it = _player_map.find(bob);
			if (it==_player_map.end() || !it->second->HasFriend(alice)) return false;
			return true;
		}
		bool FindPlayer(int roleid, HometownPlayerRef &ref)
		{
			PlayerMap::iterator it = _player_map.find(roleid);
			if (it != _player_map.end())
			{
				ref = it->second;
				return true;
			}
			else
				return false;
		}
		bool FindPlayerOnline(int roleid, HometownPlayerRef &ref)
		{
			PlayerMap::iterator it = _online_map.find(roleid);
			if (it != _online_map.end())
			{
				ref = it->second;
				return true;
			}
			else
				return false;
		}
		void LoadPlayer(int roleid);
		void LoadFriend(const HometownPlayerRef &sponsor, int roleid);
		void OnLoad(int sponsor, int roleid, const Octets &name, const GHometownInfo &ht_info, const std::set<int> &friend_list, const std::map<int, Octets> &compo_info);
		void OnLoadFail(int sponsor, int roleid);
		void OnPlayerLogin(int roleid);
		void OnPlayerLogout(int roleid);
		void OnSyncCompo(int retcode, int compo_id, int roleid);
		void OnSyncHometown(int retcode, int roleid);
		void OnAddFriend(int srcroleid, int dstroleid);
		void OnDelFriend(int srcroleid, int dstroleid);
		bool GetPlayerName(int roleid, Octets &name);
		bool GetPlayerName(int roleid, char *buf, char &buflen);
		void ExchgMoney(int roleid, int gs_sid, int amount, const GMailSyncData &syncdata);
		void OnExchgMoney(int retcode, int roleid, int gs_sid, int amount, const GMailSyncData &syncdata);
	};

	class BaseCompoManager		//每个组件的基类
	{
	protected:
		virtual bool Init()
		{
			int compo_id = GetCompoID();
			if (compo_id == -1) return false;
			HometownManager::GetInstance()->RegisterCompo(compo_id, this);
			return true;
		}
	public:
		virtual ~ BaseCompoManager() {}
		virtual int GetCompoID() const { return -1; } 
		virtual bool IsDirty(int roleid) = 0;
		virtual void SyncToDB(int roleid) = 0;
		virtual void OnSyncToDB(int retcode, int roleid) = 0;
		virtual void HandleCmd(int roleid, int cmd_type, void *data, int size, HOMETOWN::WRAPPER &response) = 0;
		virtual void OnLoad(int roleid, const Octets &info) = 0;
		virtual void ErasePlayer(int roleid) = 0;
		virtual void Update() = 0;
	};
};
#endif
