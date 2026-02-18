#include <stdlib.h>
#include "filter_man.h"

#ifdef FILTER_USE_SYSTIME
#include <time.h>
#define SYS_TIME  time(NULL)
#else
#include <timer.h>
extern abase::timer g_timer;
#define SYS_TIME  g_timer.get_systime()
#endif

void filter_man::Clear()
{
	for(FILTER_MAP::iterator it =  _filter_map.begin();it != _filter_map.end(); ++it) 
	{
		delete it->second;
	}
	_filter_map.clear();

	for(size_t i = 0; i < FILTER_IDX_MAX; ++i) 
	{
		_filter_pool[i].clear();
	}
	//Wait for delete 和wait for insert需要释放吗 ?????
}

void filter_man::Init(gactive_imp* imp)
{
	FILTER_MAP::iterator it =  _filter_map.begin();
	object_interface parent(imp);
	for(;it != _filter_map.end(); ++it)
	{
		it->second->_parent = parent;
	}
}

void filter_man::DBInit(gactive_imp* imp)
{
	FILTER_MAP::iterator it =  _filter_map.begin();
	object_interface parent(imp);
	for(;it != _filter_map.end(); ++it)
	{       
		it->second->_parent = parent;
		it->second->OnAttach();
	}
}

void filter_man::Swap(filter_man& rhs,gactive_imp* imp)
{       
	for(size_t i = 0; i < FILTER_IDX_MAX; ++i) _filter_pool[i].swap(rhs._filter_pool[i]);
	_filter_map.swap(rhs._filter_map);
	_wait_for_insert.swap(rhs._wait_for_insert);
	_wait_for_delete.swap(rhs._wait_for_delete);
	abase::swap(_operation_count,rhs._operation_count);
	abase::swap(_min_timeout_time, rhs._min_timeout_time);

	//重新设置新的object_interface
	Init(imp);
}

bool filter_man::AddFilter(filter * obj)
{
	OperationAux  op(*this);
	int mask = obj->GetMask();
	int id = obj->GetFilterID();
	FILTER_MAP::iterator it = _filter_map.find(id);
	if(it != _filter_map.end())
	{
		//同样的filter ID存在
		if((mask & filter::FILTER_MASK_UNIQUE))
		{
			//filter是排他的
			//存在这样的filter 先删除其他已存在的filter
			while(it != _filter_map.end() && it->first == id)
			{
				op.RemoveFilter(it);
			}
		}
		else if(mask & filter::FILTER_MASK_WEAK)
		{
			//filter 是弱的,即有相同的filter存在的情况下不再次加入
			//这里，检查一下是否有active的，如有，则不再加入
			while(it != _filter_map.end() && it->first == id)
			{
				filter * old = it->second;
				if(old->IsActived())
				{
					delete obj;
					return false;
				}
				++it;
			}
		}
	}       
	//可以加入了    
	op.AddFilter(obj);
	return true;
}

void filter_man::RemoveFilter(int id)
{
	OperationAux  op(*this);
	FILTER_MAP::iterator it = _filter_map.find(id);
	while(it != _filter_map.end() && it->first == id)
	{
		op.RemoveFilter(it);
	}
}

int filter_man::ClearSpecFilter(int mask, int max_count)
{
	OperationAux  op(*this);
	int count = 0;
	FILTER_MAP::iterator it = _filter_map.begin();
	for(; it != _filter_map.end();)
	{       
		//存在这样的filter 删除之
		filter * old = it->second;
		int oldmask = old->GetMask();
		if(oldmask & mask)
		{
			bool bRst = op.RemoveFilter(it);
			count += bRst?1:0;
			if(max_count && count >= max_count) break;
		}
		else
		{
			++it;
		}
	}
	return count;
}


//随机删除
int filter_man::ClearRandomSpecFilter(int mask, int max_count)
{
	OperationAux  op(*this);
	int count = 0;
	FILTER_MAP::iterator it = _filter_map.begin();

	std::vector<filter *> random_list;  
	for(; it != _filter_map.end();++it)
	{       
		//存在这样的filter 删除之
		filter * old = it->second;
		int oldmask = old->GetMask();
		if(oldmask & mask)
		{
			random_list.push_back(it->second);
		}
	}

	if(!random_list.empty())std::random_shuffle(random_list.begin(), random_list.end());
	std::vector<filter * >::iterator it2 = random_list.begin();
	for(; it2 != random_list.end(); ++it2)
	{
		bool bRst = op.RemoveFilter(*it2);
		count += bRst ? 1 : 0;

		if(max_count && count >= max_count) break;
	}

	return count;
}

bool filter_man::ModifyFilter(int filterid, int ctrlname, void * ctrlval, size_t ctrllen)
{
	FILTER_MAP::iterator it = _filter_map.find(filterid);
	if(it == _filter_map.end()) return false;
	OperationAux  op(*this);
	while(it != _filter_map.end() && it->first == filterid)
	{
		filter * obj = it->second;
		if(obj->IsActived())
		{
			obj->Modify(ctrlname,ctrlval,ctrllen);
			if(obj->_is_deleted) op.RemoveFilter(obj);
		}
		++it;
	}
	return true;
}

int filter_man::FilterCnt(int buff_type)
{
	int cnt = 0;
	FILTER_MAP::iterator it = _filter_map.begin();
	for(; it != _filter_map.end(); ++it)
	{
		if(buff_type & it->second->GetMask())
		{
			cnt ++;
		}
	}
	return cnt;
}

bool filter_man::Save(archive & ar)
{
	__DelayExecute();
	size_t count = 0;
	FILTER_MAP::iterator it = _filter_map.begin();
	for(; it != _filter_map.end(); ++ it)
	{
		if(!(it->second->GetMask() & filter::FILTER_MASK_NOSAVE))
		{
			count ++;
		}
	}
	ar << count;
	it = _filter_map.begin();
	for(; it != _filter_map.end(); ++ it)
	{
		if(!(it->second->GetMask() & filter::FILTER_MASK_NOSAVE))
		{
			it->second->SaveInstance(ar);
			count --;
		}
	}
	ASSERT(count == 0);
	return true;
}

bool filter_man::SaveSpecFilters(archive & ar,int mask)
{
	__DelayExecute();
	size_t count = 0;
	FILTER_MAP::iterator it = _filter_map.begin();
	for(; it != _filter_map.end(); ++ it)
	{
		if(it->second->GetMask() & mask)
		{
			count ++;
		}
	}
	ar << count;
	for(it = _filter_map.begin(); it != _filter_map.end(); ++ it)
	{
		if(it->second->GetMask() & mask)
		{
			it->second->SaveInstance(ar);
			count --; 
		}
	}
	ASSERT(count == 0);
	return true;
}

bool filter_man::Load(archive & ar)
{
	ASSERT(_filter_map.size() == 0);
	Clear();
	size_t size;
	ar >> size;
	for(size_t i = 0; i < size; i ++)
	{
		substance * pSub = substance::LoadInstance(ar);
		filter * f = substance::DynamicCast<filter>(pSub);
		if(!f)
		{
			delete pSub;
			ASSERT(false);
			continue;
		}

		__RawAddFilter(f);
	}
	ASSERT(_filter_map.size() == size);
	return true;
}

void filter_man::__RawAddFilter(filter * obj)
{
	int mask = obj->GetMask();
	int id = obj->GetFilterID();
	//无需检查直接放入，这适用于非延迟执行的情况
	_filter_map.insert(abase::pair<int,filter*>(id,obj));
	for(size_t i = 0; i < FILTER_IDX_MAX; ++i)
	{
		if( (1<<i) & mask) _filter_pool[i].push_back(obj);
	}
	obj->Attach();
	int timeout = obj->GetTimeout();
	if(timeout > 0)
	{
		timeout += SYS_TIME;
		obj->_timeout_t = timeout;
		__RecordNewTimeout(timeout);
	}
}

void filter_man::__RemoveFilterFromPool(filter * obj)
{
	int oldmask = obj->GetMask();
	for(size_t i = 0; i < FILTER_IDX_MAX; ++i)
	{
		if( (1<<i) & oldmask)
		{
			list & __list = _filter_pool[i];
			list::iterator it = std::find(__list.begin(),__list.end(),obj);
			if(it != __list.end())
			{
				__list.erase(it);
			}
			else
			{
				//这个错误是不应该的
				ASSERT(false);
			}
		}
	}
}

void filter_man::__DelayExecute()
{
	if(!_wait_for_delete.empty())           //删除列表不为空 则删除里面的对象
	{
		_operation_count ++;           //计数增一的目的是为了避免在删除中也会出现新的add和remove操作
		list tmp;
		tmp.swap(_wait_for_delete);     //这里交换的目的是后面的调用里还可能改变_wait_for_delete
		for(size_t i = 0; i < tmp.size(); i ++)
		{
			__RawDelFilter(tmp[i]);
		}
		_operation_count --;
	}
	if(!_wait_for_insert.empty())
	{
		list tmp;
		tmp.swap(_wait_for_insert);     //这里交换的目的是后面的调用里还可能改变_wait_for_insert
		for(size_t i = 0; i < tmp.size(); i ++)
		{
			//再次加入filter
			AddFilter(tmp[i]);
		}
	}
}

void filter_man::__RecordNewTimeout(int t)
{
	if(_min_timeout_time == 0 ||  t < _min_timeout_time)
	{
		_min_timeout_time = t;
	}
}

void filter_man::__ScanTimeoutFilter(int cur_t)
{
	OperationAux  op(*this);
	int new_t = 0x7FFFFFFF;
	FILTER_MAP::iterator it =  _filter_map.begin();
	for(;it != _filter_map.end();)
	{
		filter * old = it->second;
		int t = old->_timeout_t;
		if(t > 0 &&  new_t > t) new_t = t;
		if(t == 0 || t > cur_t)
		{
			++it;
			continue;
		}
		op.RemoveFilter(it);
	}
}

void filter_man::__RawDelFilter(filter * obj)
{
	int id = obj->GetFilterID();
	FILTER_MAP::iterator it = _filter_map.find(id);
	ASSERT(it != _filter_map.end());
	while(it != _filter_map.end() && it->first == id)
	{
		if(it -> second == obj)
		{
			filter * old = it->second;
			__RemoveFilterFromPool(old);
			old->Release();
			it = _filter_map.erase(it);
			break;
		}       
		++it;   
	}  
}

void filter_man::Heartbeat(int tick)
{       
	EF_Heartbeat(tick);
	int cur_t = SYS_TIME;
	if(_min_timeout_time && _min_timeout_time <= cur_t)
	{
		__ScanTimeoutFilter(cur_t);
	}       
	//试着进行一次延迟执行
	__DelayExecute();
}  

