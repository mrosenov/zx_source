#ifndef __ONLINE_GAME_GS_GM_PLAYER_H__
#define __ONLINE_GAME_GS_GM_PLAYER_H__
#include "player_imp.h"

class gm_dispatcher : public  gplayer_dispatcher
{
	bool _invisible;
public:
	DECLARE_SUBSTANCE(gm_dispatcher);
public:
	bool Save(archive & ar)
	{
		gplayer_dispatcher::Save(ar);
		ar << _invisible;
		return true;
	}
	bool Load(archive & ar)
	{
		gplayer_dispatcher::Load(ar);
		ar >> _invisible;
		return true;
	}
	virtual void set_invisible(bool invisible ) {_invisible = invisible;}
	virtual bool is_invisible() {return _invisible;}
	gm_dispatcher():_invisible(false){}
	virtual void enter_slice(slice * ,const A3DVECTOR &pos);
	virtual void leave_slice(slice * ,const A3DVECTOR &pos);
	virtual void enter_world();	//cache
	virtual void leave_world();
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode);
	virtual void stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mmode);
	virtual void disappear(char at_once = 0);
	virtual void LoadFrom(gplayer_dispatcher * rhs)		//从原有的dispatcher中取得数据
	{
		gplayer_dispatcher::LoadFrom(rhs);
		gplayer * pPlayer = (gplayer*) _imp->_parent;
		_invisible = pPlayer->invisible;
	}
};

#endif
