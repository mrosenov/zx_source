#include <stdlib.h>
#include "msgqueue.h"
#include "world.h"
#include "gmatrix.h"
#include <glog.h>


void MsgQueue::Send()
{
	int tmpcount = 0;
	{
		MSGQUEUE::iterator it = _queue.begin();
		MSGQUEUE::iterator end = _queue.end();
		tmpcount += (end - it);
		for(;it != end; ++it)
		{
			gmatrix::DispatchMessage(**it);
		}
	}
	{
		MultiCast **it = _multi_queue.begin();
		MultiCast **end = _multi_queue.end();
		//发送消息依次
		for(;it != end; ++it)
		{
			(*it)->Send();
		}
	}

	{
		IDMultiCast **it = _id_multi_queue.begin();
		IDMultiCast **end = _id_multi_queue.end();
		//发送消息依次
		for(;it != end; ++it)
		{
			(*it)->Send();
		}
	}

	Clear();
}

void MsgQueue::MultiCast::Send()
{
	ObjList::iterator it = _list.begin();
	ObjList::iterator end = _list.end();
	//发送消息依次
	for(;it != end; ++it)
	{
		gobject * obj = *it;
		//刨除自己的ID  这个判断是否必要?
		if(!obj->IsActived() || obj->ID.id == _msg->source.id) {
			continue;
		}
		gmatrix::DispatchMessage(obj,*_msg);
	}
}

void MsgQueue::IDMultiCast::Send()
{
	IDList::iterator it = _id_list.begin();
	IDList::iterator end = _id_list.end();
	//先扫描一遍
	for(;it != end; ++it)
	{
		if(!it->IsValid())
		{
			GLog::log(GLOG_ERR,"IDMultiCast::Send 消息%d中存在错误的目标(%d,%d)",_msg->message,it->type,it->id);
		}
	}
	it = _id_list.begin();
	
	//发送消息依次
	for(;it != end; ++it)
	{
		_msg->target = *it;
		gmatrix::DispatchMessage(*_msg);
	}
}

void MsgQueue::Run()
{
	Send();
	delete this;
}

void MsgQueue::OnTimer(int index,int rtimes, bool & is_delete)
{
	ASSERT(index != _timer_index );
	ONET::Thread::Pool::AddTask(this);
}


void 
MsgQueueList::OnTimer(int index,int rtimes, bool & is_delete)
{
	if(_cur_queue_count > 0)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		if(!_cur_queue.IsEmpty())
		{
			_SEND_CUR_QUEUE();
		}
	}

	{
		ONET::Thread::Mutex::Scoped keeper(_lock);
		if(_list[_offset])
		{
			_list[_offset]->AddTask();
			_list[_offset] = NULL;
		}
		_offset ++;
		ASSERT(_offset <= MAX_MESSAGE_LATENCY);
		if(_offset >= MAX_MESSAGE_LATENCY) _offset = 0;
	}

}

MsgQueueList::~MsgQueueList()
{
}

//------------------------------------------------------------------------------------------

MsgQueue2::msg_t::msg_t(world * plane, MSG *__msg):pPlane(plane),msg(__msg)
{
	tag = pPlane->GetTag();
	serial = pPlane->w_index_in_man;
}

void MsgQueue2::Send()
{
	{
		MSGQUEUE::iterator it = _queue.begin();
		MSGQUEUE::iterator end = _queue.end();
		for(;it != end; ++it)
		{
			const MSG & msg = *(it->msg);
			/* BW
			int world_index = it->world_index;
			if(world_index < 0 
				|| world_manager::GetInstance()->GetWorldByIndex(world_index) != it->pPlane)
			{
				__PRINTF("a:抛弃了不符合的消息msg %d from %d to %d ,world_index %d, plane: %p\n",msg.message, msg.source.id,msg.target.id,world_index, it->pPlane);
				continue;
			}
			*/
			it->pPlane->DispatchPlaneMessage(msg);
		}
	}

	{
		MSGQUEUE2::iterator it = _queue2.begin();
		MSGQUEUE2::iterator end = _queue2.end();
		for(;it != end; ++it)
		{
			const MSG & msg = *(it->msg);
			gmatrix::DispatchWorldMessage(it->tag, msg);
		}
	}
	Clear();
}


void MsgQueue2::Run()
{
	Send();
	delete this;
}

void 
MsgQueue2::AddPlaneMsg(world * pPlane, const MSG & msg)
{
	_queue.push_back(msg_t(pPlane,DupeMessage(msg)));
}

void 
MsgQueue2::AddWorldMsg(int tag, const MSG & msg)
{
	_queue2.push_back(msg2_t(tag,DupeMessage(msg)));
}

void 
MsgQueueList2::OnTimer(int index,int rtimes, bool & is_delete)
{
	if(_cur_queue_count > 0)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		if(!_cur_queue.IsEmpty())
		{
			_SEND_CUR_QUEUE();
		}
	}

	{
		ONET::Thread::Mutex::Scoped keeper(_lock);
		if(_list[_offset])
		{
			_list[_offset]->AddTask();
			_list[_offset] = NULL;
		}
		_offset ++;
		ASSERT(_offset <= MAX_MESSAGE_LATENCY);
		if(_offset >= MAX_MESSAGE_LATENCY) _offset = 0;
	}

}

MsgQueueList2::~MsgQueueList2()
{
}

