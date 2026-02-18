#include <common/packetwrapper.h>
#include <common/protocol.h>
#include <common/protocol_imp.h>
#include "active_buff.h"
#include "actobject.h"
#include "usermsg.h"
#include "world.h"

active_buff::active_buff()
{
	_icon_state_flag = false;
	_gfx_state_flag = false;
	_subscibe_timer = 0;
	_gfx_state_list.insert(_gfx_state_list.end(),64,0);
}

void 
active_buff::UpdateVisibleState(gactive_imp * pImp)
{
	if(_gfx_state_flag)
	{
		//察看状态位的改变
		int64_t state = 0;
		for(size_t i= 0; i < 64;i ++) if(_gfx_state_list[i]) state |= (int64_t)1 << i;

		//更新状态到广播数据中
		gactive_object * pObj = (gactive_object*)pImp->_parent;
		if(pObj->extend_state != state)
		{
			//修正结构并广播
			pObj->extend_state = state;
			pImp->_runner->update_gfx_state(state);
		}

		_gfx_state_flag = false;
	}
}

void 
active_buff::InsertSubscibe(gactive_imp * pImp,const XID & target, const link_sid & ld)
{
	ASSERT(target.id == ld.user_id);
	//避免重复订阅和更新最新订阅信息
	for(size_t i =0; i < _subscibe_list.size(); i ++)
	{
		link_sid & tmp  = _subscibe_list[i];
		if(ld.user_id == tmp.user_id)
		{
			tmp.cs_id = ld.cs_id;
			tmp.cs_sid = ld.cs_sid;
			return;
		}
	}

	_subscibe_list.push_back(ld);
	pImp->_runner->query_info00(target,ld.cs_id,ld.cs_sid);

	SendIconStateToOther(pImp,ld.user_id,ld.cs_id,ld.cs_sid); //立刻发送当前的状态给该订阅玩家
}

void 
active_buff::RemoveSubscibe(const XID & target)
{
	link_sid * last = _subscibe_list.end();
	link_sid * first = _subscibe_list.begin();
	for(;last != first;)
	{
		--last;
		if(target.id == last->user_id)
		{
			_subscibe_list.erase_noorder(last);
			if(!_subscibe_list.size()) _subscibe_timer = 0;
			return;
		}       
	}       
}       


void 
active_buff::ClearSubscibeList(gactive_imp * pImp)
{
	link_sid * last = _subscibe_list.end();
	link_sid * first = _subscibe_list.begin();
	XID id(GM_TYPE_PLAYER,0);
	for(;last != first;)
	{
		--last;
		id.id = last->user_id;
		pImp->SendTo<0>(GM_MSG_NOTIFY_SELECT_TARGET,id,-1); //发送消息 让目标取消选定
	}
	_subscibe_list.clear();
	_subscibe_timer = 0; 
}

void
active_buff::ExchangeSubscibeTo(gactive_imp * pImp, const XID & target, bool exchange, int latency)
{
	MSG msg;
	if(!_subscibe_list.empty())
	{
		int count = _subscibe_list.size();
		BuildMessage(msg,GM_MSG_EXCHANGE_SUBSCIBE,target,pImp->_parent->ID, pImp->_parent->pos,exchange,&_subscibe_list[0],sizeof(link_sid)*count);
	}
	else
	{
		BuildMessage(msg,GM_MSG_EXCHANGE_SUBSCIBE,target,pImp->_parent->ID, pImp->_parent->pos,exchange);
	}

	if(latency == 0)
	{
		gmatrix::SendMessage(msg);
	}
	else if(latency > 0)
	{
		gmatrix::SendMessage(msg, latency);
	}
}

void
active_buff::ExchangeSubscibeFrom(gactive_imp *pImp, const void * buf, size_t size)
{
	ASSERT(size % sizeof(link_sid) == 0);
	size_t count = size / sizeof(link_sid);

	link_sid * data = (link_sid*)buf; 
	for(size_t i = 0; i < count; ++i)
	{
		link_sid ld = data[i];
		XID target;
		MAKE_ID(target,ld.user_id);
		MSG msg;
		BuildMessage(msg,GM_MSG_NOTIFY_SELECT_TARGET,target,pImp->_parent->ID, pImp->_parent->pos,pImp->_parent->ID.id);
		gmatrix::SendMessage(msg);
	}
}

void 
active_buff::SendIconStateToOther(gactive_imp * pImp, int user_id,int cs_index, int cs_sid)
{
	using namespace S2C;
	size_t size = _icon_state.size();
	if(!size) return;
	packet_wrapper h1(size *sizeof(unsigned short) + 32);
	CMD::Make<CMD::object_buff_notify>::From(h1,pImp->_parent->ID, _icon_state.begin(),size);
	send_ls_msg(cs_index,user_id,cs_sid,h1.data(),h1.size());
}

void
active_buff::SendSelectInfoToPlayer(gactive_imp *pImp, int user_id, int cs_index, int cs_sid)
{
	using namespace S2C;
	packet_wrapper h1(32);
	CMD::Make<CMD::player_select_target>::From(h1,pImp->_parent->ID.id);
	send_ls_msg(cs_index,user_id,cs_sid,h1.data(),h1.size());
}


void
active_buff::UpdateSubscibeList(gactive_imp * pImp)
{
	if(_icon_state_flag) pImp->SendBuffData(_subscibe_list.begin(), _subscibe_list.end());

	if(_subscibe_list.size())
	{
		if(pImp->GetRefreshState()) pImp->SendInfo00(_subscibe_list.begin(), _subscibe_list.end());

		_subscibe_timer ++;
		if(_subscibe_timer >= 0x0F)
		{
			//每隔16 秒处理一次超时
			link_sid * last = _subscibe_list.end();
			link_sid * first = _subscibe_list.begin();
			XID id(GM_TYPE_PLAYER,0);
			for(;last != first;)
			{
				--last;
				id.id = last->user_id;
				world::object_info info;
				if(!pImp->_plane->QueryObject(id,info))
				{
					//目标无法发送数据
					_subscibe_list.erase_noorder(last);
				}
				else
				{
					//发送消息
					pImp->SendTo<0>(GM_MSG_SUBSCIBE_CONFIRM,id,0);
				} 
			}
			_subscibe_timer = 0; 
		}
		
	}
}


