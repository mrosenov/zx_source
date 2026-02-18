#ifndef __ONLINEGAME_GS_MSGQUEUE_H__
#define __ONLINEGAME_GS_MSGQUEUE_H__

#include <threadpool.h>
#include <hashtab.h>
#include <vector.h>
#include <common/types.h>
#include <common/message.h>
#include <timer.h>
#include <amemobj.h>
#include <amemory.h>


#include "config.h"

class world;
struct gobject;
//发送消息队列
class MsgQueue : public ONET::Thread::Runnable, public abase::timer_task , public abase::ASmallObject
{
	typedef abase::vector<gobject*,abase::fast_alloc<> > ObjList;
	typedef abase::vector<XID,abase::fast_alloc<> >  IDList;
	struct MultiCast : abase::ASmallObject
	{
		ObjList _list;
		MSG * _msg;
		MultiCast(ObjList &list, const MSG &msg): _msg(DupeMessage(msg))
		{	
			//注意，这里更改了传入的参数 
			_list.swap(list);
		}

		~MultiCast()
		{
			FreeMessage(_msg);
		}
		void Send();
	};

	struct IDMultiCast : abase::ASmallObject
	{
		IDList	_id_list;
		MSG * _msg;

		IDMultiCast(IDList &list, const MSG & msg):_msg(DupeMessage(msg))
		{
			//注意，这里更改了传入的参数 
			_id_list.swap(list);
		}

		IDMultiCast(const XID *first, const XID * last, const MSG & msg):_msg(DupeMessage(msg))
		{
			ASSERT((int)(last - first) > 0);
			_id_list.reserve(last - first);
			for(;first != last; ++first)
			{
				ASSERT(!(*first).IsErrorType());
				_id_list.push_back(*first);
			}
		}

		IDMultiCast(size_t count,const int * playerlist, const MSG & msg):_msg(DupeMessage(msg))
		{
			_id_list.reserve(count);
			XID id(GM_TYPE_PLAYER,-1);
			for(size_t i = 0; i <count ; i ++,playerlist ++)
			{
				id.id = * playerlist;
				_id_list.push_back(id);
			}
		}

		~IDMultiCast()
		{
			FreeMessage(_msg);
		}
		void Send();
	};
	typedef abase::vector<MSG*,abase::fast_alloc<> > MSGQUEUE;
	typedef abase::vector<MultiCast*,abase::fast_alloc<> > MULTICASTQUEUE;
	typedef abase::vector<IDMultiCast*,abase::fast_alloc<> > IDMULTICASTQUEUE;
	MSGQUEUE _queue;
	MULTICASTQUEUE _multi_queue;
	IDMULTICASTQUEUE _id_multi_queue;

	template <typename WORLD>
	static inline void DispatchMessage(WORLD *pPlane,const MSG & msg)
	{	
		pPlane->DispatchMessage(msg);
	}

public:
	void Swap(MsgQueue & queue)
	{
		_queue.swap(queue._queue);
		_multi_queue.swap(queue._multi_queue);
		_id_multi_queue.swap(queue._id_multi_queue);
	}
	
	MsgQueue()
	{}

	~MsgQueue()
	{
		if(_queue.size() || _multi_queue.size() || _id_multi_queue.size()) Clear();
	}
	void AddMsg(const MSG & msg)
	{
		_queue.push_back(DupeMessage(msg));
	}

	void AddMultiMsg(ObjList & list,const MSG & msg)
	{
		_multi_queue.push_back(new MultiCast(list,msg));
	}

	void AddMultiMsg(const XID * first, const XID * last, const MSG & msg)
	{
		_id_multi_queue.push_back(new IDMultiCast(first,last,msg));
	}

	void AddPlayerMultiMsg(size_t count, const int *player_list, const MSG & msg)
	{
		_id_multi_queue.push_back(new IDMultiCast(count,player_list,msg));
	}

	bool IsEmpty() 
	{
		return _queue.empty() && _multi_queue.empty() && _id_multi_queue.empty();
	}

	void Clear()
	{
		MSGQUEUE::iterator it = _queue.begin();
		MSGQUEUE::iterator end = _queue.end();
		for(;it != end; ++it)
		{
			FreeMessage(*it);
		}
		_queue.clear();
		abase::clear_ptr_vector(_multi_queue);
		abase::clear_ptr_vector(_id_multi_queue);
	}
	
	void Send();
	void AddTask()
	{
		ONET::Thread::Pool::AddTask(this);
	}
	
	virtual void OnTimer(int index,int rtimes, bool & is_delete);
	virtual void Run();
};

class MsgQueueList : public abase::timer_task
{
	typedef abase::vector<gobject*,abase::fast_alloc<> > ObjList;
	MsgQueue * 		_list[MAX_MESSAGE_LATENCY];
	MsgQueue 		_cur_queue;
	int 	   		_offset;
	ONET::Thread::Mutex 	_lock;
	ONET::Thread::Mutex 	_lock_cur;
	ONET::Thread::Mutex 	_lock_hb;
	size_t			_cur_queue_count;
	enum
	{	
		SIZE = MAX_MESSAGE_LATENCY
	};

	MsgQueue * GetQueue(int target)
	{
		if(_list[target] == NULL)
		{
			_list[target] = new MsgQueue();
		}
		return _list[target];
	}
	enum 
	{
		TASK_SIZE = 256
	};

	inline void _SEND_CUR_QUEUE()
	{
		MsgQueue * pQueue = new MsgQueue;
		_cur_queue.Swap(*pQueue);
		pQueue->AddTask();
		_cur_queue_count = 0;
	}
	inline void _TEST_SEND_CUR_QUEUE()
	{	
		if(++_cur_queue_count < TASK_SIZE) return;
		_SEND_CUR_QUEUE();
	}
public:	
	MsgQueueList():_offset(0)
	{
		_cur_queue_count = 0;
		for(int i = 0; i< SIZE; i++)
		{
			_list[i] = NULL;
		}
	}
	~MsgQueueList();

	virtual void OnTimer(int index,int rtimes, bool & is_delete);
	void AddMsg(const MSG & msg)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		_cur_queue.AddMsg(msg);
		_TEST_SEND_CUR_QUEUE();
	}

	void AddMultiMsg(abase::vector<gobject*,abase::fast_alloc<> > &list, const MSG & msg)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		_cur_queue.AddMultiMsg(list,msg);
		_TEST_SEND_CUR_QUEUE();
		return ;
	}

	void AddMultiMsg(const XID * first, const XID * last ,const MSG & msg)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		_cur_queue.AddMultiMsg(first,last,msg);
		_TEST_SEND_CUR_QUEUE();
		return ;
	}

	void AddPlayerMultiMsg(size_t count , const int * player_list, const MSG & msg)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		_cur_queue.AddPlayerMultiMsg(count,player_list,msg);
		_TEST_SEND_CUR_QUEUE();
		return ;
	}

	void SendCurQueue()
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		if(_cur_queue.IsEmpty()) return;
		MsgQueue queue;
		queue.Swap(_cur_queue);
		keeper.Unlock();
		keeper.Detach();
		queue.Send();
	}

	void AddMsg(const MSG & msg,size_t latency)
	{
		ASSERT(latency < MAX_MESSAGE_LATENCY);
		ONET::Thread::Mutex::Scoped keeper(_lock);
		int target = _offset + latency;
		if(target >= SIZE) target %= SIZE;
		GetQueue(target)->AddMsg(msg);
		return ;
	}

	void AddMultiMsg(abase::vector<gobject*,abase::fast_alloc<> > &list, const MSG & msg,size_t latency)
	{
		ASSERT(latency < MAX_MESSAGE_LATENCY);
		ONET::Thread::Mutex::Scoped keeper(_lock);
		int target = _offset + latency;
		if(target >= SIZE) target %= SIZE;
		GetQueue(target)->AddMultiMsg(list,msg);
		return ;
	}
};

//-----------------------------------------------------------------------------------------
class MsgQueue2 : public ONET::Thread::Runnable, public abase::ASmallObject
{
	struct  msg_t
	{
		world * pPlane;
		MSG * msg;
		int tag;
		int serial;
		msg_t(world * plane, MSG *__msg);
		msg_t()
		{}
	};

	struct  msg2_t
	{
		int tag;
		MSG * msg;
		msg2_t(int __tag, MSG *__msg):tag(__tag),msg(__msg)
		{
		}
		msg2_t()
		{
		}
	};
	
	typedef abase::vector<msg_t ,abase::fast_alloc<> > MSGQUEUE;
	typedef abase::vector<msg2_t ,abase::fast_alloc<> > MSGQUEUE2;
	MSGQUEUE _queue;
	MSGQUEUE2 _queue2;

public:
	void Swap(MsgQueue2 & queue)
	{
		_queue.swap(queue._queue);
		_queue2.swap(queue._queue2);
	}
	
	MsgQueue2()
	{
	}
	~MsgQueue2()
	{
		if(_queue.size() || _queue2.size()) Clear();
	}
	
	void AddPlaneMsg(world * pPlane, const MSG & msg);
	void AddWorldMsg(int tag, const MSG & msg);

	bool IsEmpty() 
	{
		return _queue.empty() && _queue2.empty();
	}

	void Clear()
	{
		{
			MSGQUEUE::iterator it = _queue.begin();
			MSGQUEUE::iterator end = _queue.end();
			for(;it != end; ++it)
			{
				FreeMessage(it->msg);
			}
			_queue.clear();
		}

		{ 
			MSGQUEUE2::iterator it = _queue2.begin();
			MSGQUEUE2::iterator end = _queue2.end();
			for(;it != end; ++it)
			{
				FreeMessage(it->msg);
			}
			_queue2.clear();
		}
	}
	
	void Send();
	void AddTask()
	{
		ONET::Thread::Pool::AddTask(this);
	}

	virtual void Run();
};

class MsgQueueList2 : public abase::timer_task
{
	typedef abase::vector<gobject*,abase::fast_alloc<> > ObjList;
	MsgQueue2 * 		_list[MAX_MESSAGE_LATENCY];
	MsgQueue2 		_cur_queue;
	int 	   		_offset;
	ONET::Thread::Mutex 	_lock;
	ONET::Thread::Mutex 	_lock_cur;
	ONET::Thread::Mutex 	_lock_hb;
	size_t			_cur_queue_count;
	enum
	{	
		SIZE = MAX_MESSAGE_LATENCY
	};

	MsgQueue2 * GetQueue(int target)
	{
		if(_list[target] == NULL)
		{
			_list[target] = new MsgQueue2();
		}
		return _list[target];
	}
	enum 
	{
		TASK_SIZE = 256
	};
	
	inline void _SEND_CUR_QUEUE()
	{
		MsgQueue2 * pQueue = new MsgQueue2;
		_cur_queue.Swap(*pQueue);
		pQueue->AddTask();
		_cur_queue_count = 0;
	}
	inline void _TEST_SEND_CUR_QUEUE()
	{	
		if(++_cur_queue_count < TASK_SIZE) return;
		_SEND_CUR_QUEUE();
	}
public:	
	MsgQueueList2():_offset(0)
	{
		_cur_queue_count = 0;
		for(int i = 0; i< SIZE; i++)
		{
			_list[i] = NULL;
		}
	}
	~MsgQueueList2();
	
	void Init()
	{
		//不再设置定时期了，由world来负责调用run
		//	SetTimer(tm,1,0);
	}

	virtual void OnTimer(int index,int rtimes, bool & is_delete);
	void AddMsg(world * plane,const MSG & msg)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		_cur_queue.AddPlaneMsg(plane,msg);
		_TEST_SEND_CUR_QUEUE();
	}

	void AddMsg(int tag ,const MSG & msg)
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		_cur_queue.AddWorldMsg(tag,msg);
		_TEST_SEND_CUR_QUEUE();
	}

	void AddMsg(world * plane,const MSG & msg,size_t latency)
	{
		ASSERT(latency < MAX_MESSAGE_LATENCY);
		ONET::Thread::Mutex::Scoped keeper(_lock);
		int target = _offset + latency;
		if(target >= SIZE) target %= SIZE;
		GetQueue(target)->AddPlaneMsg(plane,msg);
		return ;
	}

	void AddMsg(int tag,const MSG & msg,size_t latency)
	{
		ASSERT(latency < MAX_MESSAGE_LATENCY);
		ONET::Thread::Mutex::Scoped keeper(_lock);
		int target = _offset + latency;
		if(target >= SIZE) target %= SIZE;
		GetQueue(target)->AddWorldMsg(tag,msg);
		return ;
	}


	void SendCurQueue()
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_cur);
		if(_cur_queue.IsEmpty()) return;
		MsgQueue2 queue;
		queue.Swap(_cur_queue);
		keeper.Unlock();
		keeper.Detach();
		queue.Send();
	}

};
#endif

