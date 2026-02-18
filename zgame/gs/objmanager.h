#ifndef __ONLINEGAME_GS_OBJ_MANAGER_H__
#define __ONLINEGAME_GS_OBJ_MANAGER_H__

#include <set>
#include <map>
#include <timer.h>
#include <common/message.h>
#include <threadpool.h>

/*
template <int name> bool CheckObjHeartbeat(gobject *obj);
template <> bool CheckObjHeartbeat<0>(gobject * obj)
{
	return (!obj->IsActived() || !obj->imp);
}

template <> bool CheckObjHeartbeat<1>(gobject * obj)
{
	return (!obj->IsActived() || !obj->imp || obj->plane->w_activestate != 1);
}
*/


template <typename T>
struct obj_manager_basic
{
	T *_pool;
	T *_header;
	T *_tail;
	ONET::Thread::Mutex _lock;
	int _count;
	size_t _pool_size;
public:
	obj_manager_basic():_pool(NULL),_header(NULL),_tail(NULL),_count(0),_pool_size(0)
	{}
	~obj_manager_basic()
	{
		//还没有处理
		printf("object manager not free member pool yet\n");
	}

	bool Init(size_t size)
	{
		_pool = new T[size];
		memset(_pool, 0,sizeof(T)* size);
		size_t i;
		for(i = 0; i < size; i++)
		{
			_pool[i].pPrev = _pool + (i-1);
			_pool[i].pNext = _pool + (i+1);
		}
		_pool[0].pPrev = NULL;
		_pool[size- 1].pNext = NULL;
		_header = _pool;
		_tail = _pool + (size- 1);
		_pool_size = size;
		return true;
	}

	T * GetPool() const { return _pool;}
	T * GetByIndex(size_t index) const { ASSERT(index < _pool_size); return _pool + index; } 
	size_t GetIndex( T * obj) const { return obj - _pool; }

	T * Alloc()
	{
		ONET::Thread::Mutex::Scoped keeper(_lock);
		if(!_header) return NULL;
		T * pObj = _header;
		_header = (T*)_header->pNext;
		if(_header)
		{
			_header->pPrev = NULL;
		}
		else
		{
			_tail = NULL;
		}
		_count ++;
		keeper.Unlock();
		keeper.Detach();
		ASSERT(pObj->pPiece == NULL && !pObj->IsActived());
		
		pObj->Lock();
		ASSERT(pObj->pPiece == NULL && !pObj->IsActived());
		pObj->SetActive();
		return pObj;
	}
	
	void Free(T * pObj)					//要求传入的结构保持锁定
	{
		ASSERT(pObj->spinlock);
		ASSERT(!pObj->imp);
		if(pObj->IsActived())
		{
			pObj->Clear();
		}

		ONET::Thread::Mutex::Scoped _keeper(_lock);
		if(!_tail)
		{
			_header = _tail = pObj;
			pObj->pPrev = pObj->pNext = NULL;
		}
		else
		{
			_tail->pNext = pObj;
			pObj->pPrev = _tail;
			pObj->pNext = NULL;
			_tail = pObj;
		}
		_count --;
	}

	inline int GetAllocedCount() const 
	{
		return _count;
	}

	inline size_t GetCapacity() const
	{
		return _pool_size;
	}
};

template <typename OBJECT ,size_t MM_HEARTBEAT_TICK, typename Insertor>
class obj_manager : public obj_manager_basic<OBJECT>, public abase::timer_task, public ONET::Thread::Runnable
{
	ONET::Thread::Mutex _lock_change;
	ONET::Thread::Mutex _lock_heartbeat;

	typedef OBJECT T;
	typedef std::set<OBJECT *> 	OBJECT_SET;
	typedef std::map<OBJECT *,int> 	OBJECT_MAP;

	OBJECT_SET		_obj_set;
	OBJECT_MAP		_change_list;
	typename std::set<OBJECT *>::iterator	_cur_cursor;
	T*  			_cur_obj;
	int			_heart_obj_count;

	inline void DoDelete(T * pObj)
	{
		if(_cur_obj == pObj)
		{
			_cur_cursor = _obj_set.find(_cur_obj);
			++_cur_cursor;
			if(_cur_cursor == _obj_set.end())
			{
				_cur_obj = NULL;
			}
			else
			{
				_cur_obj = *_cur_cursor;
			}
		}

		if(!_obj_set.erase(pObj))
		{
			ASSERT(false);
		}
	}

	inline void DoInsert(T * pObj)
	{
		if(!_obj_set.insert(pObj).second)
		{
			//对多次插入报告错误
			//未来release版应该怎么办？
			ASSERT(false);
		}
	}

	void DoChange()
	{
		ONET::Thread::Mutex::Scoped keeper(_lock_change);
		typename std::map<OBJECT *,int>::const_iterator it = _change_list.begin();
		for(;it != _change_list.end(); ++it)
		{
			if(!it->second) continue;
			ASSERT(it->second == -1 || it->second == 1);
			if(it->second > 0)
			{
				DoInsert(it->first);
				
			}
			else
			{
				DoDelete(it->first);
			}
		}
		_change_list.clear();
	}

	void CollectHeartbeatObject(abase::vector<T *> &list)
	{
		DoChange();
		int size = _obj_set.size();
		_heart_obj_count += size;
		if(size == 0)
		{
			_heart_obj_count = 0;
			_cur_obj = NULL;
			return ;
		}

		typename std::set<OBJECT *>::iterator	end = _obj_set.end();
		if(_cur_obj == NULL)
		{
			_cur_cursor = end;
		}
		else
		{
			_cur_cursor = _obj_set.find(_cur_obj);
		}
#ifdef _DEBUG
		int idle_count = 0;
#endif
		
		list.reserve(size / MM_HEARTBEAT_TICK + 5);
		for(;_heart_obj_count >= (int)MM_HEARTBEAT_TICK; _heart_obj_count -= (int)MM_HEARTBEAT_TICK)
		{
			if(_cur_cursor == end)
			{
				_cur_cursor = _obj_set.begin();
			}

			_cur_obj = *_cur_cursor;

			//将这个对象加入到收集列表中
#ifdef _DEBUG
			idle_count += Insertor::push_back(list,_cur_obj);
#else
			Insertor::push_back(list,_cur_obj);
#endif
			++_cur_cursor;
		}
		if(_cur_cursor != end)
		{
			_cur_obj = *_cur_cursor;
		}
		else
		{
			_cur_obj = NULL;
		}
		
	}
public:
	obj_manager():_cur_obj(NULL),_heart_obj_count(0)
	{}
	~obj_manager()
	{
		//这里不释放timer了，反正不会报错了
		//RemoveTimer();
	}
	
	bool Init(size_t size)
	{
		obj_manager_basic<OBJECT>::Init(size);
		
		//由world来负责定期调用Run，不再使用定时期
		//int rst = SetTimer(g_timer,1,0);
		//ASSERT(rst >=0);
		return true;
	}
	
	void Insert(T * pObj)
	{
	/*
		ONET::Thread::Mutex::Scoped keeper(_lock_insert);
		_ins_list.push_back(pObj);
		*/
		ONET::Thread::Mutex::Scoped keeper(_lock_change);
		_change_list[pObj] ++;
	}

	void Remove(T * pObj)
	{
	/*
		ONET::Thread::Mutex::Scoped keeper(_lock_delete);
		_del_list.push_back(pObj);
		*/
		ONET::Thread::Mutex::Scoped keeper(_lock_change);
		_change_list[pObj] --;
	}
	

	void OnHeartbeat()
	{

		ONET::Thread::Mutex::Scoped keeper(_lock_heartbeat);
		abase::vector<T *> list;
		CollectHeartbeatObject(list);
		keeper.Unlock();
		keeper.Detach();
		if(!list.size()) return;

		MSG msg;
		memset(&msg,0,sizeof(msg));
		msg.message = GM_MSG_HEARTBEAT;
		msg.param = MM_HEARTBEAT_TICK/TICK_PER_SEC;

		T** it = list.begin();
		T** end = list.end();
		for(;it != end; ++it)
		{
			OBJECT * obj = *it;
			obj->Lock();
			//刨除自己的ID  这个判断是否必要?
			if(!obj->IsActived() || !obj->imp || !obj->plane || obj->plane->w_activestate != 1) 
	//		if(CheckObjHeartbeat<1>(obj))
			{
				obj->Unlock();
				continue;
			}
			int rst = 0;
			ASSERT(obj->plane);
			rst = obj->imp->DispatchMessage(msg);
			if(!rst)
			{
				ASSERT(obj->spinlock && "这里必须是上锁状态");
				obj->Unlock();
			}
			else
			{
				ASSERT(!obj->spinlock && "没有解开锁，可能是时序问题，但更可能是错误");
			}
		}
	}

	virtual void Run()
	{
		OnHeartbeat();
	}


protected:
	
	void OnTimer(int index,int rtimes, bool & is_delete)
	{
		ONET::Thread::Pool::AddTask(this);
	}
};

#endif

