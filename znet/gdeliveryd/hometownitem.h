#ifndef _GNET_HOMETOWN_ITEM_H_
#define _GNET_HOMETOWN_ITEM_H_

#include <map>
#include <octets.h>
#include <marshal.h>
#include <assert.h>
#include "hometowndef.h"
#include "hometownpacket.h"

namespace GNET
{
	class HometownItemBody;
	class HometownTargetObj;
	class HometownPlayerObj;

	struct HometownItemData : public Marshal
	{
		unsigned int _id;
		int _pos;
		int _count;
		int _max_count;
		Octets _data;
		HometownItemData():_id(0),_pos(-1),_count(0),_max_count(0){}
		OctetsStream& marshal(OctetsStream &os) const 
		{ 
			os << _id;
			os << _pos;
			os << _count;
			os << _max_count;
			os << _data;
			return os;
		}
		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> _id;
			os >> _pos;
			os >> _count;
			os >> _max_count;
			os >> _data;
			return os;
		}
	};

	class HometownItem
	{
		void FromData(const HometownItemData &idata)
		{
			_id = idata._id;
			_pos = idata._pos;
			_count = idata._count;
			_max_count = idata._max_count;
			UnpackData(idata._data);
		}

		void ToData(HometownItemData &idata) const
		{
			idata._id = _id;
			idata._pos = _pos;
			idata._count = _count;
			idata._max_count = _max_count;
			idata._data.clear();
			PackData(idata._data);
		}
		friend class HometownItemMan;

	protected:
		virtual void PackData(Octets &data) const {}
		virtual void UnpackData(const Octets &data) {}
	public:
		unsigned int _id;
		int _pos;
		int _count;
		int _max_count;
		HometownItemBody *_body;

	public:
		HometownItem(unsigned int id, HometownItemBody *body):_id(id),_pos(-1),_count(0),_max_count(0),_body(body){}
		virtual ~HometownItem() {}
		int CanUse(const HometownPlayerObj *user) const;
		int CanUse(const HometownPlayerObj *user, const HometownTargetObj *target) const;
		//返回值表示是否消失
		bool OnUse(HometownPlayerObj *user);
		bool OnUse(HometownPlayerObj *user, HometownTargetObj *target);
		virtual HometownItem *Clone() const { return new HometownItem(*this); }
	};

	typedef HometownItem HometownSimpleItem; //普通物品如道具只有body成员，没有其他的

	class FarmCropItem : public HometownItem
	{
	public:
		int _grow_period;		//剩余生长期
		int _protect_period;	//剩余防偷保护期
		int _fruit_left;		//剩余果实数
		int _fruit_total;		//总果实数

		FarmCropItem(unsigned int id, HometownItemBody *body):HometownItem(id, body),_grow_period(0),_protect_period(0),_fruit_left(0),_fruit_total(0){}
		HometownItem *Clone() const { return new FarmCropItem(*this); }

	protected:
		void PackData(Octets &data) const
		{
			data.insert(data.end(), &_grow_period, sizeof(_grow_period));
			data.insert(data.end(), &_protect_period, sizeof(_protect_period));
			data.insert(data.end(), &_fruit_left, sizeof(_fruit_left));
			data.insert(data.end(), &_fruit_total, sizeof(_fruit_total));
		}
		void UnpackData(const Octets &data)
		{
			assert(data.size() == 4*sizeof(int));
			const int *p = (const int *)data.begin();
			_grow_period = *p++;
			_protect_period = *p++;
			_fruit_left = *p++;
			_fruit_total = *p;
		}
	};

	class HometownItemMan : public Marshal
	{
		typedef std::map<unsigned int, const HometownItem *> HTIMap;
		static HTIMap _map;

		typedef std::vector<HometownItem *> ItemList;
		ItemList _items;
		int _capacity;

		HometownItem *MakeItem(const HometownItemData &idata)
		{
			HometownItem *item = NULL;
			HTIMap::iterator it = _map.find(idata._id);
			if (it != _map.end())
			{
				item = it->second->Clone();
				item->FromData(idata);
			}
			return item;
		}

	public:
		HometownItemMan():_capacity(0) { }
		~HometownItemMan() { Clear(); }
		static void AddItemTemplate(unsigned int id, const HometownItem *item) { _map[id] = item; }

		int GetCapacity() const { return _capacity; }
		void SetCapacity(int capacity)
		{
			assert(_capacity == 0);
			_capacity = capacity;
			for (int i = 0; i < capacity; ++i)
				_items.push_back(NULL);
		}
		void IncCapacity(int capacity)
		{
			assert(capacity>0);
			_capacity += capacity;
			for (int i = 0; i < capacity; ++i)
				_items.push_back(NULL);
		}
		void Clear()
		{
			ItemList::iterator it = _items.begin(), ie = _items.end();
			for (; it != ie; ++it)
			{
				HometownItem *item = *it;
				if (item!=NULL) delete item;
			}
			_items.clear();
			_capacity = 0;
		}

		int GetItemID(int pos, int &id)
		{
			if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
			HometownItem *item = _items[pos];
			if (item==NULL || item->_count<=0) return HT_ERR_ITEMNOTFOUND;
			id = item->_id;
			return HT_ERR_SUCCESS;
		}

		int GetData(int pos, HometownItemData &idata)
		{
			if (pos<0 || pos>=_capacity) return HT_ERR_POCKETPOS;
			HometownItem *item = _items[pos];
			if (item==NULL || item->_count<=0) return HT_ERR_ITEMNOTFOUND;
			item->ToData(idata);
			return HT_ERR_SUCCESS;
		}

		void GetData(std::vector<HometownItemData> &idatas) const
		{
			ItemList::const_iterator it = _items.begin(), ie = _items.end();
			for (; it != ie; ++it)
			{
				const HometownItem *item = *it;
				if (item!=NULL && item->_count>0)
				{
					idatas.push_back(HometownItemData());
					item->ToData(idatas.back());
				}
			}
		}

		void SetData(const std::vector<HometownItemData> &idatas)
		{
			std::vector<HometownItemData>::const_iterator it, ie = idatas.end();
			for (it = idatas.begin(); it != ie; ++it) AddItem(*it);
		}


		//添加的位置在idata.pos中指定，如果为-1则不指定位置
		int AddItem(const HometownItemData &idata);				//如果不能全部放入则失败
		int AddItemAsMuch(HometownItemData &idata);				//尽量放入最多，遗留没放入的记在idata中
		int UseItem(HometownPlayerObj *user, int pos, char &consumed);
		int UseItem(HometownPlayerObj *user, int pos, HometownTargetObj *target, char &consumed);
		int SubItem(int pos, int count);

		OctetsStream& marshal(OctetsStream &os) const 
		{
			os << _capacity;
			std::vector<HometownItemData> idatas;
			GetData(idatas);
			os << idatas;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			Clear();
			int capacity; 
			os >> capacity;
			SetCapacity(capacity);
			std::vector<HometownItemData> idatas;
			os >> idatas;
			SetData(idatas);
			return os;
		}
	};
};

#endif
