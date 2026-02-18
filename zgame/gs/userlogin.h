#ifndef __ONLINEGAME_GS_USERLOGIN_H__
#define __ONLINEGAME_GS_USERLOGIN_H__

#include "object.h"
#include <threadpool.h>
#include <amemobj.h>
#include <db_if.h>
#include <interlocked.h>

void	user_login(int cs_index,int sid,int uid, const void * auth_data, size_t auth_size,
			bool use_spec_pos, int spec_tag,float px,float py, float pz, char trole, char flag);
void 	user_enter_world(int cs_idnex,int sid,int uid);

struct gplayer;
void 	user_save_data(gplayer * pPlayer,GDB::Result * callback = NULL, 
		int priority = 0,int mask = DBMASK_PUT_ALL, 
		bool spec_save = false, int spec_tag = -1, const A3DVECTOR & spec_pos = A3DVECTOR(0,0,0));

class world;
struct userlogin_t
{
	gplayer * _player;
	world * _plane;
	int _uid;
	void * _auth_data;
	size_t _auth_size;
	int _loginip;

	int _origin_tag;
	A3DVECTOR _origin_pos;
};

void do_player_login(const A3DVECTOR & ppos,const GDB::base_info * pInfo, const GDB::vecdata * data,const userlogin_t &user, char trole, char flag);

bool do_login_check_data(const GDB::base_info * pInfo, const GDB::vecdata * data);

class delivery_state
{
	enum 
	{
		STATE_NO_CONNECTION,
		STATE_OK,
		STATE_DISCONNECT_WAITING,
		STATE_DISCONNECT_WRITING,
	};
	int _lock;
	int _state;
	int _shutdown_counter;
	bool _connected;
	bool _can_login;
	
	class WriteTask :  public abase::ASmallObject , public ONET::Thread::Runnable
	{
		delivery_state * _state;
		public:
		WriteTask(delivery_state * pState):_state(pState){}
		virtual void Run();
	};
	
	bool ReadyWriteBack();
	void WriteBackDone();

public:
	delivery_state(): _lock(0), _state(STATE_NO_CONNECTION),_shutdown_counter(0), _connected(true), _can_login(false)
	{
	}

	void OnConnect();
	void OnDisconnect();
	inline bool CanLogin() 
	{
		return _can_login;
	}

	inline int GetShutDownCounter()
	{
		return _shutdown_counter;
	}

	inline int IncShutDownCounter()
	{
		return interlocked_increment(&_shutdown_counter);
	}
};
extern delivery_state g_d_state;
#endif

