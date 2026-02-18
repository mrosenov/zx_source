#include "gmatrix.h"
#include "item_list.h"
#include "world.h"

/**
 *		物品列表
 */

inline int 
item_list::__try_pile(int& __bind, int __type,size_t & __count,size_t __pile_limit,item*&pEmpty)
{
	int last_index = -1;
	// Youshuang add
	item::BindPileItem( __bind );
	// end
	//试图堆叠
	for(size_t i = 0; i < _list.size(); i ++)
	{
		int type = _list[i].type;
		int bind = _list[i].proc_type;
		if(type == -1)
		{
			if(pEmpty == NULL) pEmpty = &(_list[i]);
		}
		else if(type == __type && _list[i].count < _list[i].pile_limit && bind == __bind)
		{
			//ASSERT(_list[i].pile_limit == __pile_limit);
			int tmp = __count;
			if(tmp + _list[i].count > _list[i].pile_limit)
			{
				tmp =  _list[i].pile_limit - _list[i].count;
			}
			
			_list[i].count += tmp;
			__count -= tmp;
			last_index = i;
			if(__count > 0) continue;
			break;
		}
	}
	return last_index;
}

inline void
item_list::__find_empty(item*&pEmpty)
{
	for(size_t i = 0; i < _list.size(); i ++)
	{
		int type = _list[i].type;
		if(type == -1)
		{
			pEmpty = &(_list[i]);
			break;
		}
	}
	return ;
}

int 
item_list::Push(item_data & data)
{
	if(data.type == -1)
	{
		ASSERT(false && "物品数据不正确");
		return -1;
	}
	item * pEmpty = NULL;
	if(data.pile_limit > 1)	//可以堆叠的物品
	{	
		size_t oldcount = data.count;
		int rst = __try_pile(data.proc_type, data.type,data.count,data.pile_limit,pEmpty);
		ASSERT(data.count >=0);
		if(data.count == 0) return rst;
		if(data.count != oldcount && pEmpty == NULL) return rst;
	}
	else
	{
		__find_empty(pEmpty);
	}

	if(pEmpty == NULL) return -1;
	item it;
	if(!MakeItemEntry(it,data)) 
	{
		it.Clear();
		return -1;
	}
	*pEmpty  = it;
	it.Clear();
	data.count = 0;
	pEmpty->PutIn(_location,pEmpty - _list.begin(),_owner);
	_empty_slot_count --;
	return pEmpty - _list.begin();
}

int 
item_list::Push(const item_data & __data, int & count, int expire_date)
{
	ASSERT(count > 0);
	item_data data = __data;
	data.count = count;
	data.expire_date = expire_date;
	int rst = Push(data);
	count = data.count;
	item_data& tmp = const_cast<item_data&>(__data);
	tmp.proc_type = data.proc_type;
	return rst;
}

/*
 * 分页整理物品
 * @param pageind = 0时为整理全部物品
 */
bool
item_list::ArrangeItem(int pageind,std::vector<exchange_item_info>& vec_exchange_info)
{
	size_t page_slot_start;
	size_t page_slot_end;
	size_t slot_ind;
	size_t page_item_num = 0;
	size_t page_item_total_num = 0;
	const size_t PAGE_SLOT_NUM = 48;
	size_t item_num = _list.size() - _empty_slot_count;

	if (pageind < 0) return false; //非法
	if (pageind == 0) //整理全部物品
	{
		page_slot_start = 0;
		page_slot_end = _list.size() - 1;
		slot_ind = page_slot_start;
		page_item_total_num = item_num;
	}
	else
	{
		//一页最多有48个格子
		if (_list.size() < (pageind - 1) * PAGE_SLOT_NUM || item_num == 0) return false;
		page_slot_start = (pageind - 1) * PAGE_SLOT_NUM;
		page_slot_end = std::min(_list.size() - 1,page_slot_start + PAGE_SLOT_NUM - 1);
		slot_ind = page_slot_start;
		page_item_total_num = 0;
		for (; slot_ind <= page_slot_end; slot_ind++)
			if (_list[slot_ind].type != -1) page_item_total_num++;
	}

	if (page_item_total_num == 0) return false;

	for (slot_ind = page_slot_start;slot_ind <= page_slot_end; slot_ind++)
	{
		if (_list[slot_ind].type != -1) 
		{
			if (slot_ind != page_slot_start+page_item_num)
			{
				vec_exchange_info.push_back(exchange_item_info(slot_ind, page_slot_start+page_item_num));
				ExchangeItem(slot_ind,page_slot_start+page_item_num);
			}
			page_item_num++;
		}
		if (page_item_num == page_item_total_num) break;
	}

	std::map<int,int>& order_item_list = gmatrix::GetOrderListMap();
	for (slot_ind = page_slot_start; slot_ind < page_slot_start + page_item_num-1; ++slot_ind)
	{
		size_t min_ind = slot_ind;
		int min_id = order_item_list[_list[slot_ind].type];
		for (size_t ind1 = slot_ind + 1; ind1 < page_slot_start + page_item_num; ++ind1)
		{
			if (order_item_list[_list[ind1].type] < min_id)
			{
				min_ind = ind1;
				min_id = order_item_list[_list[ind1].type];
			}
		}
		if (min_ind != slot_ind)
		{
			vec_exchange_info.push_back(exchange_item_info(min_ind, slot_ind));
			ExchangeItem(min_ind,slot_ind);
		}
	}	

	return true;
}	

int 
item_list::Push(item & it)
{
	if(it.type == -1)
	{
		ASSERT(false && "物品数据不正确");
		return -1;
	}
	if(_empty_slot_count == 0 && it.pile_limit <=1)  return -1;

	item * pEmpty = NULL;
	if(it.pile_limit > 1)	//可以堆叠的物品
	{
		size_t oldcount = it.count;
		int rst = __try_pile(it.proc_type,it.type,it.count,it.pile_limit,pEmpty);
		ASSERT(it.count >=0);
		if(it.count == 0) 
		{
			//全部放入，释放物品
			it.Release();
			return rst;
		}
		if(it.count != oldcount && pEmpty == NULL)
		{
			//部分放入 返回错误 不释放物品
			return -1;
		}
	}
	else
	{
		__find_empty(pEmpty);
	}

	if(pEmpty == NULL) return -1;
	*pEmpty = it;
	it.Clear();
	pEmpty->PutIn(_location,pEmpty - _list.begin(),_owner);
	_empty_slot_count --;
	return pEmpty - _list.begin();
}

int 
item_list::PushInEmpty(int start, const item_data & data , int count)
{
	ASSERT(data.pile_limit >= (size_t)count);
	for(int i = start; (size_t)i < _list.size(); i ++)
	{
		if(_list[i].type != -1) continue;

		item it;
		if(!MakeItemEntry(it,data)) 
		{
			it.Clear();
			return -1;
		}
		it.count = count;
		_list[i] = it;
		it.Clear();
		_list[i].PutIn(_location,i,_owner);
		_empty_slot_count --;
		return i;
	}
	return -1;
}

int 
item_list::PushInEmpty(int start, item & it)
{
	for(int i = start; (size_t)i < _list.size(); i ++)
	{
		if(_list[i].type != -1) continue;

		_list[i] = it;
		it.Clear();
		_list[i].PutIn(_location,i,_owner);
		_empty_slot_count --;
		return i;
	}
	return -1;
}

bool item_list::MakeDBData(GDB::pocket_itemlist &list)
{
#ifdef _DEBUG
	size_t count = 0;
	for(size_t i = 0; i < _list.size(); i ++)
	{
		if(_list[i].type == -1) count ++;
	}
	ASSERT(count == _empty_slot_count);
#endif
	ASSERT(!list.count && !list.list);
	ASSERT(list.list == NULL || list.count == 0);
	size_t item_count = _list.size() - _empty_slot_count;
	if(item_count == 0) return true;
	list.list = (GDB::pocket_item*)abase::fast_allocator::alloc(item_count*sizeof(GDB::pocket_item));
	list.count = item_count;
	size_t index = 0;
	GDB::pocket_item* pData = list.list;
	for(size_t i = 0; i < _list.size(); i ++)
	{
		if(_list[i].type == -1) continue;
		ConvertDBItemEntry(pData[index], _list[i], i );
		index ++;
	}
	return true;
} 

void item_list::ReleaseDBData(GDB::pocket_itemlist & list)
{
	if(!list.list) return;
	size_t size = list.count * sizeof(GDB::pocket_item);
	abase::fast_allocator::free(list.list,size);
	list.count = 0;
	list.list = 0;
}

int item_list::GetDBData(GDB::pocket_item *pData, size_t size)
{
#ifdef _DEBUG
	size_t count = 0;
	for(size_t i = 0; i < _list.size(); i ++)
	{
		if(_list[i].type == -1) count ++;
	}
	ASSERT(count == _empty_slot_count);
#endif
	if(size < _list.size())
	{
		ASSERT(false);
		return -1;
	}
	size_t item_count = _list.size() - _empty_slot_count;
	if(item_count == 0) return 0;
	size_t index = 0;
	for(size_t i = 0; i < _list.size(); i ++)
	{
		if(_list[i].type == -1) continue;
		ConvertDBItemEntry(pData[index], _list[i], i);
		index ++;
	}
	return index;
}


bool item_list::InitFromDBData(const GDB::pocket_itemlist &list)
{
	ASSERT(_empty_slot_count == _list.size());
	size_t count = list.count;
	const GDB::pocket_item* pData = list.list;
	for(size_t i = 0; i < count ; i ++)
	{
		size_t index= pData[i].index;
		if(index >= _list.size()) continue;	//忽略多余的物品
		if(!MakeItemEntry(_list[index],pData[i]))
		{
			//出现错误的物品 直接清除
			_list[index].Clear();
			continue;
		}
		_empty_slot_count --;
	}
	return true;
}
