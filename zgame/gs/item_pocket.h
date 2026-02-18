#ifndef __ONLINEGAME_GS_ITEM_POCKET_H__
#define __ONLINEGAME_GS_ITEM_POCKET_H__

/*
 * 随身包裹的节省内存版本， 目前未使用这个版本
 *
 */ 


#include "item.h"
#include "itemdata.h"
#include <vector.h>
#include <amemobj.h>
#include <amemory.h>
#include <common/packetwrapper.h>
#include <db_if.h>

//放在随身包裹里面的物品, 比如材料
class pocket_item
{
public:
	pocket_item():type(-1), count(0), pile_limit(0){}
	~pocket_item(){
		type = -1;
		count = 0;
		pile_limit = 0;
	}	

	void Clear(){
		type = -1;
		count = 0;
		pile_limit = 0;
	}

	void Save(archive &ar){
		ar << type;
		ar << count;
		ar << pile_limit;	
	}

	void Load(archive &ar){
		ar >> type;
		ar >> count;
		ar >> pile_limit;
	}
	
	int type;		//类型
	size_t count;	//数量
	size_t pile_limit; //堆叠限制， 不存盘
};


bool MakePocketItemEntry(pocket_item& entry, const item_data &data);
bool MakePocketItemEntry(pocket_item& entry, const GDB::pocket_item &data);
void ConvertItemToPocketItem(pocket_item &data, const item &item);
void ConvertPocketItemToItem(item &item, const pocket_item &data);
void ConvertPocketItemToItemData(item_data &item, const pocket_item &data);


//实现随身包裹
class pocket_item_list : public abase::ASmallObject
{
public:
	pocket_item_list(size_t size):_list(size, pocket_item()),_empty_slot_count(size)
	{}

	~pocket_item_list()
	{}

	void Swap(pocket_item_list &rhs){
		_list.swap(rhs._list);
		abase::swap(_empty_slot_count,rhs._empty_slot_count);	
	}

	bool SaveToClient(archive & ar)
	{
		size_t count = _list.size() - _empty_slot_count;
		ar << count;
		size_t i;
		for(i = 0; i < _list.size(); i ++)
		{
			pocket_item & it = _list[i];
			if(it.type == -1)
			{
				continue;
			}
			count --;
			ar << i;
			ar << it.type;
			ar << it.count;
		}
		ASSERT(count == 0);
		return true;
	}

	bool Save(archive & ar)
	{
		ar << _list.size();
		size_t i;
		for(i = 0; i < _list.size(); i ++)
		{
			_list[i].Save(ar);
		}
		return true;
	}

	bool Load(archive & ar)
	{
		size_t count;
		ar >> count;
		ASSERT(count >= _list.size());
		if(count > _list.size())
		{
			SetSize(count);
		}
		ASSERT(_empty_slot_count == count);
		Clear();
		size_t i;
		for(i = 0; i < count; i ++)
		{
			_list[i].Load(ar);
			if(_list[i].type != -1) _empty_slot_count --; 
		}
		return true;
	}
	//为了方便客户端处理，这里会发送另外的数据(expire_time, proc_type)给客户端
	void SimpleSave(abase::octets & data)
	{
		data.reserve(_list.size() * (sizeof(int)*2));
		int expire_time = 0;
		int proc_type = 0;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			int type = _list[i].type;
			data.push_back(&type,sizeof(type));
			if(type < 0) continue;
			data.push_back(&expire_time, sizeof(expire_time));
			data.push_back(&proc_type, sizeof(proc_type));
			int count = _list[i].count;
			data.push_back(&count,sizeof(count));
		}
	}
	
	//为了方便客户端处理，这里会发送另外的数据(expire_time, proc_type)给客户端
	bool DetailSave(archive & ar)
	{
		size_t count = _list.size() - _empty_slot_count;
		ar << count;
		size_t i;
		int expire_time = 0;
		int proc_type = 0;
		short len = 0;
		for(i = 0; i < _list.size(); i ++)
		{
			pocket_item & it = _list[i];
			if(it.type == -1)
			{
				continue;
			}
			count --;
			ar << i;
			ar << it.type;
			ar << expire_time;
			ar << proc_type;
			ar << it.count;
			ar << len;
		}
		ASSERT(count == 0);
		return true;
	}

	size_t Size() const { return _list.size();}
	bool IsSlotEmpty(int index) { return _list[index].type == -1;}
	size_t GetEmptySlotCount() { return _empty_slot_count;}
	
	const pocket_item & operator[](size_t index) const  { return _list[index];}
	pocket_item & operator[](size_t index) { return _list[index];}
	
	int  Find(int start,int type)
	{
		for(size_t i = start; i < _list.size(); i ++)
		{
			if(_list[i].type == type) return i;
		}
		return -1;
	}
	
	//	寻找能放东西的第一个空位，考虑了堆叠因素
	int FindEmpty(int type)
	{
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1 ) return i;
			if(_list[i].type == type && _list[i].count < _list[i].pile_limit) return i;
		}
		return -1;
	}

	// 	对特定的物品是否有空闲的位置	
	bool HasSlot(int type)
	{
		if(_empty_slot_count) return true;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == type && _list[i].count < _list[i].pile_limit) return true;
		}
		return false;
	}

	//	对特定的物品是否有空闲的位置	 按照指定的位置来计算
	bool HasSlot(int type,int count)
	{
		if(_empty_slot_count) return true;
		for(size_t i = 0; i < _list.size() && count > 0; i ++)
		{
			if(_list[i].type == type)
			{
				count -= _list[i].pile_limit - _list[i].count;
			}
		}
		return count <= 0;
	}


	//	是否当前已经没有空的位子
	bool IsFull()
	{
		return !_empty_slot_count;
	}

	//	察看特定位置是否有指定的物品
	bool IsItemExist(size_t index,int type, size_t count)
	{
		if(index >= Size()) return false;
		const pocket_item & it = _list[index];
		if(it.type == -1 || it.type != type) return false;
		if(it.count < count) return false;
		return true;
	}

	//在指定的位置放入一个pocket item
	void Exchange(int index, pocket_item & it)
	{
		if(it.type != -1) _empty_slot_count --;
		pocket_item & src = _list[index];
		pocket_item tmp = it; it = src; src = tmp;
		tmp.Clear();
		if(it.type != -1)
		{
			_empty_slot_count ++;
		}
		return ;
	}

	//	交换物品表里的两个位置
	bool ExchangeItem(size_t ind1, size_t ind2 )
	{
		if(ind1 >= _list.size() || ind2 >= _list.size()) return false;
		if(ind1 == ind2) return true;
		pocket_item tmp = _list[ind1];
		_list[ind1] = _list[ind2];
		_list[ind2] = tmp;
		tmp.Clear();
		return true;
	}
	
	//返回还剩余多少，0表示对象没有了
	int DecAmount(size_t index,size_t count)
	{
		if(_list[index].type == -1) 
		{
			ASSERT(false);
			return 0;
		}

		pocket_item & it = _list[index];
		if(it.count <= count)
		{
			it.Clear();
			_empty_slot_count ++;
		}
		else
		{
			it.count -= count;
		}
		return it.count;
	}

	//返回有多少数量被加入
	int IncAmount(size_t index, size_t count) 
	{
		if(_list[index].type == -1) 
		{
			ASSERT(false);
			return -1;
		}

		pocket_item & it = _list[index];
		if(it.count >= it.pile_limit) return 0;
		size_t delta = it.pile_limit - it.count;
		if(delta > count) delta = count;
		it.count += delta;
		return (int)delta;
	}

	int Push(pocket_item & it);

	//指定的位置放入物品
	bool Put(int index, pocket_item & it)
	{
		pocket_item & old = _list[index]; 
		if(old.type == -1)
		{
			if(it.type == -1) return true;
			//原来没有物品，直接放入
			old = it;
			it.Clear();
			_empty_slot_count  --;
			return true;
		}
		else
		{
			if(old.type == it.type && old.pile_limit >= old.count + it.count)
			{ 
				//原来有物品，而且可以放入
				//叠加上去，并且释放传入的物品
				ASSERT(it.pile_limit == old.pile_limit);
				old.count += it.count;
				it.Clear();
				return true;
			}
		}
		return false;
	}

	// 把一个item移到另外一个位置， 不是互换
	bool MoveItem(size_t src, size_t dest, size_t *pCount);

	bool MakeDBData(GDB::pocket_itemlist &list);
	void ReleaseDBData(GDB::pocket_itemlist & list);
	int GetDBData(GDB::pocket_item *pData, size_t size);

	bool InitFromDBData(const GDB::pocket_itemlist & list);
	void SetSize(size_t size);
	void Clear(){
		for(size_t i=0; i < _list.size(); i++){
			if(_list[i].type != -1){
				_list[i].Clear();
			}
		}	
		_empty_slot_count = _list.size();
	}
private:
	inline int _try_pile(int type, size_t &count, size_t pile_limit, pocket_item* &pEmpty);
	inline void _find_empty(pocket_item *&pEmpty);

private:
	abase::vector<pocket_item>  _list;
	size_t _empty_slot_count;
};

bool CheckPocketItemFilterList(int item_type);
bool CanMoveToPocket(const item & _item);
bool CanMoveToPocket(const item_data & _itemdata);
void Exchange(item_list &_item_list, pocket_item_list &_pocket_list, size_t item_idx, size_t pocket_idx);

//交换普通包裹和随身包裹里面的物品
int ExchangeInventoryPocketItem(item_list &_item_list, pocket_item_list &pocket_list, size_t item_idx, size_t pocket_idx);

//把普通包裹的东西移动到随身包裹
int MoveInventoryItemToPocket(item_list &src, pocket_item_list &dest, size_t src_idx, size_t dest_idx, size_t count);

//把随身包裹里面的东西移动到普通包裹
int MovePocketItemToInventory(pocket_item_list &src, item_list &dest, size_t src_idx, size_t dest_idx, size_t count);

// 将所有物品放入普通包裹， 不考虑堆叠
int MoveAllPocketItemtoInventory(pocket_item_list &src, item_list &dest);

#endif

