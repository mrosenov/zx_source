#ifndef __ONLINEGAME_GS_GLOBAL_MANAGER_H__
#define __ONLINEGAME_GS_GLOBAL_MANAGER_H__

#include "world.h"
#include <string>

class global_world_manager : public world_manager
{
protected:
	world  _plane;  
	player_cid   _cid;
	bool InitNetClient(const char * gmconf);
	friend class global_world_message_handler;
public:
	global_world_manager()
	{}
	virtual int Init(const char * gmconf_file,const char * servername, int tag, int index);

	//副本用释放函数，新增加资源一定要全部释放
	virtual void Release();

	virtual void Heartbeat();
public:
	virtual bool InitNetIO(const char * servername);
	virtual void GetPlayerCid(player_cid & cid);
	virtual world * GetWorldByIndex(size_t index);
	virtual size_t GetWorldCapacity();
	virtual int GetOnlineUserNumber();
	virtual void GetLogoutPos(gplayer_imp * pImp, int &world_tag, A3DVECTOR & pos);
	virtual void SwitchServerCancel(int link_id,int user_id, int localsid);
	virtual bool IsUniqueWorld();
	virtual world * GetWorldLogin(int id, const GDB::base_info * pInfo, const GDB::vecdata * data);
	virtual int OnMobDeath(world * pPlane, int faction,int tid,const A3DVECTOR& pos, int attacker_id) { return 0; }

public:
	virtual void HandleSwitchStart(int uid, int source_tag, const A3DVECTOR & pos, const instance_key & key);
	virtual int SwitchPlayerFrom(world_manager *from, gplayer * pPlayer, const A3DVECTOR & pos, const instance_key & ins_key);
};

class global_world_message_handler : public world_message_handler
{
protected:
	world * _plane;
	virtual ~global_world_message_handler(){}
	int PlayerComeIn(world * pPlane,const MSG &msg);
public:
	global_world_message_handler(global_world_manager * man, world * plane):world_message_handler(man),_plane(plane) {}
	virtual int HandleMessage(const MSG& msg);
	virtual int RecvExternMessage(int msg_tag,const MSG & msg){return 0;}
};

#endif

