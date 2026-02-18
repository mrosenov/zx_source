#include "hometownitem.h"
#include "hometownitembody.h"
#include "hometowndef.h"
#include <assert.h>
#include "hometownstateobj.h"

//#include <typeinfo>

namespace GNET
{

HometownItemMan::HTIMap HometownItemMan::_map;

int HometownItem::CanUse(const HometownPlayerObj *user) const
{
	return _body->CanUse(user, this);
}

int HometownItem::CanUse(const HometownPlayerObj *user, const HometownTargetObj *target) const
{
	return _body->CanUse(user, this, target);
}

bool HometownItem::OnUse(HometownPlayerObj *user)
{
	return _body->OnUse(user, this);
}

bool HometownItem::OnUse(HometownPlayerObj *user, HometownTargetObj *target)
{
	return _body->OnUse(user, this, target);
}

int HometownItemMan::AddItem(const HometownItemData &idata)
{
	if (idata._count <= 0) return HT_ERR_SUCCESS;
	int pos = idata._pos;
	if (pos == -1)
	{
		std::vector<size_t> vacants;			//空闲包裹栏index列表
		std::vector<size_t> same_ids;			//同ID未满的位置列表,优先放这些位置
		int count = idata._count;
		for (size_t index = 0; index < _items.size(); ++index)
		{
			HometownItem *item = _items[index];
			if (item==NULL || item->_count<=0)
			{
				vacants.push_back(index);
			}
			else
			{
				if (item->_id==idata._id && item->_count<item->_max_count)
				{
					same_ids.push_back(index);
					count -= item->_max_count-item->_count;
					if (count <= 0)
						break;
				}
			}
		}
		if (count>0 && vacants.size()<(size_t)(count+idata._max_count-1)/idata._max_count) return HT_ERR_POCKETFULL;

		HometownItemData temp = idata;
		for (size_t i = 0; i < same_ids.size(); ++i)
		{
			HometownItem *item = _items[same_ids[i]];
			if (item->_count+temp._count <= item->_max_count)
			{
				item->_count += temp._count;
				temp._count = 0;
				break;
			}
			else
			{
				temp._count -= item->_max_count-item->_count;
				item->_count = item->_max_count;
			}
		}
		if (temp._count > 0)
		{
			for (size_t i = 0; i < vacants.size(); ++i)
			{
				size_t index = vacants[i];
				HometownItem *&item = _items[index];
				if (item != NULL) delete item;
				if (temp._count <= temp._max_count)
				{
					temp._pos = index;
					item = MakeItem(temp);
					break;
				}
				else
				{
					HometownItemData tmpitem = temp;
					tmpitem._count = tmpitem._max_count;
					tmpitem._pos = index;
					temp._count -= temp._max_count;
					item = MakeItem(tmpitem);
				}
			}
		}
	}
	else	//指定放在pos位置
	{
		if (idata._count > idata._max_count) return HT_ERR_POCKETPOSFULL;
		if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
		HometownItem *&item = _items[pos];
		if (item == NULL)
		{
			item = MakeItem(idata);
		}
		else if (item->_count == 0)
		{
			delete item;
			item = MakeItem(idata);
		}
		else if (item->_id==idata._id && item->_count+idata._count<=item->_max_count)
		{
			item->_count += idata._count;
		}
		else
			return HT_ERR_POCKETPOSFULL;
	}
	return HT_ERR_SUCCESS;
}

int HometownItemMan::AddItemAsMuch(HometownItemData &idata)
{
	if (idata._count <= 0) return HT_ERR_SUCCESS;
	int pos = idata._pos;
	if (pos == -1)
	{
		std::vector<size_t> vacants;
		for (size_t index = 0; index < _items.size(); ++index)
		{
			HometownItem *item = _items[index];
			if (item==NULL || item->_count<=0)
			{
				vacants.push_back(index);
			}
			else
			{
				if (item->_id==idata._id && item->_count<item->_max_count)
				{
					int avail = item->_max_count-item->_count;
					if (idata._count <= avail)
					{
						item->_count += idata._count;
						idata._count = 0;
						break;
					}
					else
					{
						idata._count -= avail;
						item->_count = item->_max_count;
					}
				}
			}
		}

		if (idata._count > 0)
		{
			for (size_t i = 0;  i < vacants.size(); ++i)
			{
				size_t index = vacants[i];
				HometownItem *&item = _items[index];
				if (item != NULL) delete item;
				if (idata._count <= idata._max_count)
				{
					idata._pos = index;
					item = MakeItem(idata);
					idata._count = 0;
					break;
				}
				else
				{
					HometownItemData tmpitem = idata;
					tmpitem._count = tmpitem._max_count;
					tmpitem._pos = index;
					idata._count -= idata._max_count;
					item = MakeItem(tmpitem);
				}
			}

			if (idata._count > 0) 
				return HT_ERR_POCKETFULL;
			else
				return HT_ERR_SUCCESS;
		}
		else
			return HT_ERR_SUCCESS;
	}
	else	//指定放在pos位置
	{
		if (idata._count > idata._max_count) return HT_ERR_POCKETPOSFULL;
		if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
		HometownItem *&item = _items[pos];
		if (item == NULL)
		{
			item = MakeItem(idata);
			idata._count = 0;
		}
		else if (item->_count == 0)
		{
			delete item;
			item = MakeItem(idata);
			idata._count = 0;
		}
		else if (item->_id==idata._id && item->_count<item->_max_count)
		{
			int avail = item->_max_count-item->_count;
			if (idata._count <= avail)
			{
				item->_count += idata._count;
				idata._count = 0;
			}
			else
			{
				item->_count = item->_max_count;
				idata._count -= avail;
			}
		}
		if (idata._count > 0)
			return HT_ERR_POCKETPOSFULL;
		else
			return HT_ERR_SUCCESS;
	}
}

int HometownItemMan::UseItem(HometownPlayerObj *user, int pos, char &consumed)
{
	if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
	HometownItem *&item = _items[pos];
	if (item==NULL || item->_count<=0) return HT_ERR_ITEMNOTFOUND;
	int useable = item->CanUse(user);
	if (useable == HT_ERR_SUCCESS)
	{
		if (item->OnUse(user))	//true表明要扣除物品
		{
			consumed = 1;
			--item->_count;
			if (item->_count <= 0)
			{
				delete item;
				item = NULL;
			}
			user->SetDirty(true);
		}
		else
			consumed = 0;
	}
	return useable;
}

int HometownItemMan::UseItem(HometownPlayerObj *user, int pos, HometownTargetObj *target, char &consumed)
{
	if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
	HometownItem *&item = _items[pos];
	if (item==NULL || item->_count<=0) return HT_ERR_ITEMNOTFOUND;
	int useable = item->CanUse(user, target);
	if (useable == HT_ERR_SUCCESS)
	{
		if (item->OnUse(user, target))	//true表明要扣除物品
		{
			consumed = 1;
			--item->_count;
			if (item->_count <= 0)
			{
				delete item;
				item = NULL;
			}
			user->SetDirty(true);
		}
		else
			consumed = 0;
	}
	return useable;
}

int HometownItemMan::SubItem(int pos, int count)
{
	assert(count > 0);
	if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
	HometownItem *&item = _items[pos];
	if (item==NULL || item->_count<=0) return HT_ERR_ITEMNOTFOUND;
	if (item->_count<count) return HT_ERR_ITEMNOTENOUGH;
	item->_count -= count;
	if (item->_count <= 0)
	{
		delete item;
		item = NULL;
	}
	return HT_ERR_SUCCESS;
}
};
