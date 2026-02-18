#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>

#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h" 
#include "clstab.h"
#include "actsession.h"
#include "userlogin.h"
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>
#include "trade.h"
#include "task/taskman.h"
#include "playerstall.h"
#include "gm_player.h"

DEFINE_SUBSTANCE(gm_dispatcher,gplayer_dispatcher, CLS_GM_DISPATCHER)

void 
gm_dispatcher::enter_slice(slice *  pPiece,const A3DVECTOR &pos)
{
	if(!_invisible) 
	{
		return gplayer_dispatcher::enter_slice(pPiece, pos);
	}
	else
	{
		//取得新区域数据
		get_slice_info(pPiece,_nw,_mw,_pw);
	}
}

void 
gm_dispatcher::leave_slice(slice * pPiece,const A3DVECTOR &pos)
{
	if(!_invisible)
	{
		return gplayer_dispatcher::leave_slice(pPiece, pos);
	}
	else
	{
		pPiece->Lock();
		gather_slice_object(pPiece,_leave_list);
		pPiece->Unlock();
	}
}

void
gm_dispatcher::move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	if(_invisible) return;
	gplayer_dispatcher::move(target,cost_time,speed,move_mode);
}

void
gm_dispatcher::stop_move(const A3DVECTOR & target, unsigned short speed ,unsigned char dir,unsigned char move_mode)
{
	if(_invisible) return;
	gplayer_dispatcher::stop_move(target,speed,dir,move_mode);
}

void
gm_dispatcher::enter_world()
{
	if(!_invisible)
	{
		return gplayer_dispatcher::enter_world(); 
	}
	else
	{
		//取得进入一个区域应当取得数据
		enter_region();
	}
}

void
gm_dispatcher::leave_world()
{
	if(!_invisible)
	{
		return gplayer_dispatcher::leave_world();
	}
}

void
gm_dispatcher::disappear(char at_once)
{
	if(!_invisible)
	{
		return gplayer_dispatcher::disappear();
	}
}

