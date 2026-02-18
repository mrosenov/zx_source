#include <stdlib.h>
#include "gimp.h"
#include "object.h"
#include "world.h"
#include "usermsg.h"
#include <threadpool.h>
#include <glog.h>


DEFINE_SUBSTANCE_ABSTRACT(gobject_imp,substance,-1)
DEFINE_SUBSTANCE_ABSTRACT(dispatcher,substance,-1)
DEFINE_SUBSTANCE_ABSTRACT(controller,substance,-1)

namespace
{
	class runner_enter
	{
		dispatcher * _runner;
		const A3DVECTOR & _pos;
	public:
		runner_enter(dispatcher * runner,const A3DVECTOR &pos):_runner(runner),_pos(pos){}
		void operator()(slice * pPiece)
		{
			_runner->enter_slice(pPiece,_pos);
		}
	};
	class runner_leave
	{
		dispatcher * _runner;
		const A3DVECTOR & _pos;
	public:
		runner_leave(dispatcher * runner,const A3DVECTOR &pos):_runner(runner),_pos(pos){}
		void operator()(slice * pPiece)
		{
			_runner->leave_slice(pPiece,_pos);
		}
	};
}

void
dispatcher::MoveBetweenSlice(slice * pPiece,slice * pNewPiece,const A3DVECTOR &pos)
{
	_imp->_plane->MoveBetweenSlice(pPiece,pNewPiece,
			runner_enter(this,pos),
			runner_leave(this,pos));
}

int 
gobject_imp::GetWorldTag() 
{ 
	return _plane->GetTag(); 
}

int 
gobject_imp::GetClientTag()  //liuyue-facbse 返回客户端用的地图号 多个副本实例会共用同一个地图号
{ 
	return GetWorldManager()->GetClientTag();
}

world_manager* 
gobject_imp::GetWorldManager() 
{ 
	return _plane->GetWorldManager(); 
}

bool
gobject_imp::StepMove(const A3DVECTOR &offset)
{
	//在调用这个地方之前必须要上锁
	ASSERT(_parent);
	gobject * pObject = _parent;
	slice *pPiece = pObject->pPiece;
	world *pPlane = _plane;
	if(pPiece == NULL)
	{
		//正巧不在格子内，这是有可能的 但是可能性偏低
		//需要做日志
		GLog::log(GLOG_ERR,"用户%d移动错误",pObject->ID.id);
		ASSERT(false);
		return false;
	}
	
	A3DVECTOR newpos(pObject->pos),oldpos(pObject->pos);
	newpos += offset;
	const grid * pGrid = &pPlane->GetGrid();
	dispatcher *pRunner = _runner;
	//检查是否超出了 整个服务器的范围
	if(pGrid->IsOutsideGrid(newpos.x,newpos.z))
	{
		//用于特殊情况的边界检查
		return false;
	}

	if(!pGrid->IsLocal(newpos.x,newpos.z))
	{
		//超出了边界
		return false;
	}

	ASSERT(pGrid->IsLocal(newpos.x,newpos.z));

	pObject->pos = newpos;
	pRunner->begin_transfer();
	if(pPiece->IsOutside(newpos.x,newpos.z))
	{
		slice *pNewPiece = pGrid->Locate(newpos.x,newpos.z);
		if(pNewPiece == pPiece) {
			pRunner->end_transfer();
			return true;	//这种情况是可能的，由于计算的误差而产生判断的不一致
		}
		if(_commander->MoveBetweenSlice(_parent,pPiece,pNewPiece))
		{
			//移动失败，直接返回
			GLog::log(GLOG_ERR,"用户%d在MoveBetweenSlice时失败",pObject->ID.id);

			pObject->pos = oldpos;
			pRunner->end_transfer();
			return true;
		}

		//处理格间的移动发送的消息
		pRunner->MoveBetweenSlice(pPiece,pNewPiece,newpos);
	}
//	pRunner->notify_pos();	//通知player自己的当前位置(NPC这个函数无实现)，现在不做，因为不需要如此频繁
	pRunner->end_transfer();
	return true;
}

void dispatcher::equipment_damaged(int index)
{
}

void
dispatcher::update_gfx_state(int64_t newstate)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::update_visible_state>::From(h1,pObject,newstate);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1,-1);
}

void 
dispatcher::enchant_result(const XID & caster, int skill, char level,bool invader, char at_state, char stamp, int value)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::enchant_result>::From(h1,pObject, caster, skill, level, invader,at_state,stamp,value);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1,-1);
}

void 
dispatcher::battle_flag_change(char os, int ns)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::battle_flag_change>::From(h1, pObject->ID.id, os, ns);

	AutoBroadcastCSMsg(_imp->_plane, pObject->pPiece, h1 , -1);
}

void
dispatcher::object_charge(int id, char type, const A3DVECTOR & dest)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::object_charge>::From(h1,pObject, id, type, dest);
	AutoBroadcastCSMsg(_imp->_plane, pObject->pPiece, h1 , -1);
}

void
dispatcher::object_be_charge_to(char type, int target_id, const A3DVECTOR & dest)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::object_be_charge_to>::From(h1,pObject, type, target_id, dest);
	AutoBroadcastCSMsg(_imp->_plane, pObject->pPiece, h1 , -1);
}

void
dispatcher::object_special_state(int type, char on)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::object_special_state>::From(h1,pObject, type, on);
	AutoBroadcastCSMsg(_imp->_plane, pObject->pPiece, h1 , -1);
}

void
dispatcher::blow_off(int roleid)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::blow_off>::From(h1, roleid);
	AutoBroadcastCSMsg(_imp->_plane, pObject->pPiece, h1 , -1);
}

