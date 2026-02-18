#ifndef __QNETGAME_GS_ITEM_MANAGER_H__
#define __QNETGAME_GS_ITEM_MANAGER_H__

/*
	由于现在物品均为静态生成（除去优化策略等）因此使用一个item_manager来事先生成并保存所有的物品
*/
#include "item.h"
#include "item/item_addon.h"
#include "playermall.h"
#include "playervipaward.h"
#include <hashmap.h>
#include <crc.h>

enum { SPIRIT_SET_SLOT_COUNT = 5};
class itemdataman; 
class gactive_imp;
class item_manager
{
public:
	struct spirit_set	//器魄组合
	{
		int setlist[SPIRIT_SET_SLOT_COUNT];
		spirit_set(int list[])
		{
			memcpy(setlist,list,sizeof(setlist));
		}

		spirit_set() {}
		
		unsigned int operator()(const spirit_set & rhs) const
		{
			unsigned int crc = crc32((const char*)rhs.setlist, sizeof(rhs.setlist));
			if(crc == 0) crc = 1;
			return crc;
		}

		bool operator==(const spirit_set & rhs) const
		{
			return memcmp(setlist, rhs.setlist, sizeof(setlist)) == 0;
		}
	};
private:
	static item_manager __instance;
	bool __InitFromDataMan(itemdataman & dataman);
	bool __InitMall(itemdataman &dataman);
	bool __InitBonusMall(itemdataman &dataman);
	bool __InitZoneMall(itemdataman &dataman);
	bool __InitVipAward(itemdataman & dataman);

	typedef abase::hash_map<int, item_body *> __ITEM_MAP;

	enum{MAX_ITEM_INDEX = 65528*2};
	static char _cash_item_flag[MAX_ITEM_INDEX];
	
	
	//abase::hash_map<spirit_set, std::vector<int>, spirit_set> _spirit_set_table;
	//abase::hash_map<unsigned int, int> _sp_set_chk_table;
	abase::hash_map<unsigned int, std::vector<int> > _sp_eid_table;

	__ITEM_MAP _map;
	qgame::mall _mall;		//普通商城
	qgame::mall _bonusmall;		//鸿利商城
	qgame::mall _zonemall;		//跨服商城

	vipgame::award_data _vip_award;		//vip奖励，普通玩家奖励
	int _vipaward_timestamp;


	int _mall_timestamp;
	int _bonusmall_timestamp;
	int _zonemall_timestamp;

	int __GetSpiritAddonSet(int crc, int soul_eid, int * pData, int dataCount) const;
private:
	static inline void SetCashItem(size_t citem_id)
	{
		ASSERT(citem_id < MAX_ITEM_INDEX);
		_cash_item_flag[citem_id] = 1;
	}

public: 
	static void UseItemLog(int rid, int id, int guid1,int guid2,int count);
	static inline item_manager & GetInstance()
	{
		return __instance;
	}

	static inline bool IsCashItem(size_t item_id)
	{
		if(item_id >= MAX_ITEM_INDEX) return false;
		return _cash_item_flag[item_id];
	}
	
	static bool InitFromDataMan(itemdataman & dataman)
	{
		return GetInstance().__InitFromDataMan(dataman);
	}

	static vipgame::award_data & GetVipAwardData()
	{
		return GetInstance()._vip_award;
	}

	static int GetVipAwardTimestamp()
	{
		return GetInstance()._vipaward_timestamp;
	}

	static qgame::mall & GetShoppingMall() 
	{
		return GetInstance()._mall;
	}

	static int GetMallTimestamp()
	{
		return GetInstance()._mall_timestamp;
	}
	
	static qgame::mall & GetBonusShoppingMall() 
	{
		return GetInstance()._bonusmall;
	}

	static int GetBonusMallTimestamp()
	{
		return GetInstance()._bonusmall_timestamp;
	}
	
	static qgame::mall & GetZoneShoppingMall() 
	{
		return GetInstance()._zonemall;
	}

	static int GetZoneMallTimestamp()
	{
		return GetInstance()._zonemall_timestamp;
	}


	static int GetSpiritAddonSet(int crc, int soul_eid, int * pData, int dataCount)
	{
		return GetInstance().__GetSpiritAddonSet(crc, soul_eid, pData, dataCount);
	}
	
public:
	item_body * GetItemBody(int id)
	{
		__ITEM_MAP::iterator it = _map.find(id);
		if(it == _map.end()) return NULL;
		return it->second;
	}
	
};

class title_manager
{
	static title_manager __instance;

	struct  node_t
	{
		struct  entry_t
		{
			addon_handler * _handler;
			addon_data  _data;
		} addon[3];
		int count;

		node_t():count(0)
		{
			memset(addon, 0, sizeof(addon));
		}

		void AddAddon(addon_handler * handler, const addon_data  & data)
		{
			ASSERT(count < 3);
			addon[count]._handler = handler;
			addon[count]._data = data;
			count ++;
		}
	};
	
	typedef abase::hash_map<int, node_t>  __TITLE_MAP;

	__TITLE_MAP _map;

	bool __InitFromDataMan(itemdataman & dataman);
	bool __AddTitleHandler(itemdataman & dataman,int id,  int addon_id[3]);
	bool __AddAddon(itemdataman & dataman,node_t & node, int addon_id);
	bool __ActiveTitle(gactive_imp * imp, int title);
	bool __DeactiveTitle(gactive_imp * imp, int title);
public:
	inline static title_manager & GetInstance() 
	{
		return __instance;
	}
	inline static bool InitFromDataMan(itemdataman & dataman)
	{
		return GetInstance().__InitFromDataMan(dataman);
	}

	inline static bool ActiveTitle(gactive_imp * imp, int title)
	{
		return GetInstance().__ActiveTitle(imp,title);
	}
	inline static bool DeactiveTitle(gactive_imp * imp, int title)
	{
		return GetInstance().__DeactiveTitle(imp,title);
	}
};

#endif

