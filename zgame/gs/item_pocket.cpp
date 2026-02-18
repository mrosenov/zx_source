#include "item_pocket.h"
#include "item_list.h"
#include "item_manager.h"
#include "gmatrix.h"

/*
 * 随身包裹的节省内存版本， 目前未使用这个版本
 *
*/ 

bool MakePocketItemEntry(pocket_item& entry, const item_data &data)
{
	entry.type = data.type;
	entry.count = data.count;
	entry.pile_limit = data.pile_limit; 
	return true;
}

bool MakePocketItemEntry(pocket_item& entry, const GDB::pocket_item &data)
{
	entry.type = data.id;
	entry.count = data.count;
	entry.pile_limit = gmatrix::GetDataMan().get_item_pile_limit(data.id); 
	if(((int)entry.pile_limit) <= 0 || !entry.count){
		return false;
	} 
	return true;
}

void ConvertDBItemEntry(GDB::pocket_item &data, const pocket_item &entry, int index)
{
	data.id = entry.type;
	data.count = entry.count;
	data.index = index; 
}


void ConvertItemToPocketItem(pocket_item &data, const item &item)
{
	data.type = item.type;
	data.count = item.count;
	data.pile_limit = item.pile_limit;
}

void ConvertPocketItemToItem(item &item, const pocket_item &data)
{
	item.type = data.type;
	item.count = data.count;
	item.pile_limit = data.pile_limit;
	item.equip_mask = 0;
	item.proc_type = 0;
	item.price = gmatrix::GetDataMan().get_item_sell_price(data.type);
	item.expire_date = 0;
	item.guid.guid1 = 0;
	item.guid.guid2 = 0;
	item.body = NULL;
	item.content = item_content();	
}


void ConvertPocketItemToItemData(item_data &item, const pocket_item &data)
{
	item.type = data.type;
	item.count = data.count;
	item.pile_limit = data.pile_limit;
	item.proc_type = 0;
	item.price = gmatrix::GetDataMan().get_item_sell_price(data.type);
	item.expire_date = 0;
	item.guid.guid1 = 0;
	item.guid.guid2 = 0;
	item.content_length = 0;
	item.item_content = NULL;
}

//堆叠物品
int pocket_item_list::_try_pile(int __type,size_t & __count,size_t __pile_limit,pocket_item *&pEmpty)
{
	int last_index = -1;
	//试图堆叠
	for(size_t i = 0; i < _list.size(); i ++)
	{
		int type = _list[i].type;
		if(type == -1)
		{
			if(pEmpty == NULL) pEmpty = &(_list[i]);
		}
		else if(type == __type && _list[i].count < _list[i].pile_limit)
		{
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

void pocket_item_list::_find_empty(pocket_item *&pEmpty)
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


bool pocket_item_list::MakeDBData(GDB::pocket_itemlist &list)
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

void pocket_item_list::ReleaseDBData(GDB::pocket_itemlist & list)
{
	if(!list.list) return;
	size_t size = list.count * sizeof(GDB::pocket_item);
	abase::fast_allocator::free(list.list,size);
	list.count = 0;
	list.list = 0;
}

int pocket_item_list::GetDBData(GDB::pocket_item *pData, size_t size)
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


bool pocket_item_list::InitFromDBData(const GDB::pocket_itemlist &list)
{
	ASSERT(_empty_slot_count == _list.size());
	size_t count = list.count;
	const GDB::pocket_item* pData = list.list;
	for(size_t i = 0; i < count ; i ++)
	{
		size_t index= pData[i].index;
		if(index >= _list.size()) continue;	//忽略多余的物品
		if(!MakePocketItemEntry(_list[index],pData[i]))
		{
			//出现错误的物品 直接清除
			_list[index].Clear();
			continue;
		}
		_empty_slot_count --;
	}
	return true;
}

void pocket_item_list::SetSize(size_t size)
{
	if(size <= _list.size()) return;
	abase::vector<pocket_item> 	tmplist(size,pocket_item());
	size_t offset = size - _list.size();
	for(size_t i = 0; i < _list.size(); i ++)
	{
		pocket_item tmp = _list[i];
		_list[i] = tmplist[i];
		tmplist[i] = tmp;
		tmp.Clear();
	}
	_list.swap(tmplist);

	_empty_slot_count += offset;
}


bool pocket_item_list::MoveItem(size_t src, size_t dest, size_t *pCount)
{
	if(src >= _list.size() || dest >= _list.size()) return false;
	size_t count = *pCount;
	if(src == dest) return true;
	if(!count) return false;
	pocket_item & __src = _list[src];
	pocket_item & __dest= _list[dest];

	if(__src.type == -1 || __src.count < count) 
	{
		*pCount = 0;
		return true;
	}

	if(__dest.type == -1)
	{
		__dest = __src;
		__src.count -= count;
		__dest.count = count;

		if(!__src.count)
		{
			__src.Clear();
		}
		else
		{
			_empty_slot_count --;
		}
		return true;
	}

	if(__dest.type != __src.type || __dest.count + count > __dest.pile_limit) 
	{
		*pCount = 0;
		return true;
	}

	__dest.count += count;
	__src.count -= count;
	if(!__src.count)
	{
		_empty_slot_count ++;
		__src.Clear();
	}
	return true;
}

int pocket_item_list::Push(pocket_item & it)
{
	if(it.type == -1)
	{
		return -1;
	}
	if(_empty_slot_count == 0 && it.pile_limit <=1)  return -1;

	pocket_item * pEmpty = NULL;
	if(it.pile_limit > 1)	//可以堆叠的物品
	{
		size_t oldcount = it.count;
		int rst = _try_pile(it.type,it.count,it.pile_limit,pEmpty);
		ASSERT(it.count >=0);
		if(it.count == 0) 
		{
			//全部放入，释放物品
			it.Clear();
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
		_find_empty(pEmpty);
	}

	if(pEmpty == NULL) return -1;
	*pEmpty = it;
	it.Clear();
	_empty_slot_count --;
	return pEmpty - _list.begin();
}


//检查物品是否能放入随身包裹
bool CanMoveToPocket(const item & _item)
{
	//有属性的物品不能放入随身包裹	
	if(_item.body != NULL || _item.content.size != 0){
		return false;
	}
	//随身包裹的物品不保留这些属性
	if(_item.equip_mask != 0 || _item.proc_type !=0 || _item.expire_date != 0){
		return false;
	}
	if(_item.type == -1){
		return true;
	}

	return gmatrix::IsPocketItem(_item.type);
}

//检查物品是否能放入随身包裹
bool CanMoveToPocket(const item_data & _itemdata)
{
	if(_itemdata.proc_type != 0 || _itemdata.expire_date !=0 || _itemdata.content_length !=0){
		return false;
	}
		
	if(_itemdata.type == -1){
		return true;
	}

	if(item_manager::GetInstance().GetItemBody(_itemdata.type) != NULL)
	{
		return false;
	}

	return gmatrix::IsPocketItem(_itemdata.type);
}

//交换普通包裹和随身包裹里面的物品实际逻辑
void Exchange(item_list &_item_list, pocket_item_list &_pocket_list, size_t item_idx, size_t pocket_idx)
{
	item tmp;
	pocket_item tmp_pocket;

	_item_list.Exchange(item_idx, tmp);
	ConvertItemToPocketItem(tmp_pocket, tmp);
	_pocket_list.Exchange(pocket_idx, tmp_pocket);
	ConvertPocketItemToItem(tmp, tmp_pocket);
	_item_list.Exchange(item_idx, tmp);
	tmp.Clear();
	tmp_pocket.Clear();
}

//交换普通包裹和随身包裹里面的物品
int ExchangeInventoryPocketItem(item_list &_item_list, pocket_item_list &_pocket_list, size_t item_idx, size_t pocket_idx)
{
	if(item_idx >= _item_list.Size() || pocket_idx >= _pocket_list.Size()){
		return -1;
	}
	if(!CanMoveToPocket(_item_list[item_idx])){
		return -2;
	}	
	Exchange(_item_list, _pocket_list, item_idx, pocket_idx);
	return 0;
}

//把普通包裹的东西移动到随身包裹
int MoveInventoryItemToPocket(item_list &src, pocket_item_list &dest, size_t src_idx, size_t dest_idx, size_t count)
{
	if(src_idx >= src.Size() || dest_idx >= dest.Size())
	{
		return -1;
	}
	if(!count) return -1;
	
	if(src[src_idx].type == -1 ||
			(dest[dest_idx].type != -1 && src[src_idx].type != dest[dest_idx].type))
	{
		return -1;
	}

	item &it = src[src_idx];
	if(!CanMoveToPocket(it))
	{
		return -2;
	}
	
	if(count > it.count) count = it.count;
	
	if(dest[dest_idx].type == -1)
	{
		if(count == it.count)
		{
			Exchange(src, dest, src_idx, dest_idx);
		}
		else
		{
			pocket_item tmp_pocket;
			ConvertItemToPocketItem(tmp_pocket, it);
			dest.Put(dest_idx, tmp_pocket);
			src.DecAmount(src_idx, count);							
		}
		return count;
	}

	//真正的移动
	int delta = dest.IncAmount(dest_idx,count);
	if(delta < 0)
	{
		//因为已经判断过了
		ASSERT(false);
		return -1;
	}
	src.DecAmount(src_idx,delta);
	return delta;
}

//把随身包裹里面的东西移动到普通包裹
int MovePocketItemToInventory(pocket_item_list &src, item_list &dest, size_t src_idx, size_t dest_idx, size_t count)
{
	if(src_idx >= src.Size() || dest_idx >= dest.Size())
	{
		return -1;
	}
	if(!count) return -1;
	
	if(src[src_idx].type == -1 ||
			(dest[dest_idx].type != -1 && src[src_idx].type != dest[dest_idx].type))
	{
		return -1;
	}
	pocket_item &it = src[src_idx];
	if(count > it.count) count = it.count;
	
	if(dest[dest_idx].type == -1)
	{
		if(count == it.count)
		{
			Exchange(dest, src, dest_idx, src_idx);
		}
		else
		{
			item tmp;
			ConvertPocketItemToItem(tmp, it);
			dest.Put(dest_idx, tmp);
			src.DecAmount(src_idx, count);
			tmp.Clear();			
		}
		return count;
	}

	//真正的移动
	int delta = dest.IncAmount(dest_idx,count);
	if(delta < 0)
	{
		//因为已经判断过了
		ASSERT(false);
		return -1;
	}
	src.DecAmount(src_idx,delta);
	return delta;
}

// 将所有物品放入普通包裹， 不考虑堆叠
int MoveAllPocketItemtoInventory(pocket_item_list &src, item_list &dest)
{
	if(src.Size() - src.GetEmptySlotCount() > dest.GetEmptySlotCount()){
		return -1;
	}

	int j = 0;
	for(size_t i=0; i< src.Size(); i++){
		if(src[i].type == -1) continue;
		item tmp;
		ConvertPocketItemToItem(tmp, src[i]);
		j = dest.PushInEmpty(j, tmp);
		j++;
		tmp.Clear();
	}
	src.Clear();
	return 0;
}
