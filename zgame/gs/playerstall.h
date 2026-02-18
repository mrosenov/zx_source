#ifndef __ONLINEGAME_GAME_GS_PLAYER_STALL_OBJ_H__
#define __ONLINEGAME_GAME_GS_PLAYER_STALL_OBJ_H__

#include <vector.h>
#include "item_list.h"

struct player_stall 
{
	struct entry_t
	{
		int type;
		size_t index;
		size_t count;
		size_t price;
		unsigned int crc;
		int expire_date;
	};

	item_list & _inv;
	abase::vector<entry_t, abase::fast_alloc<> > _goods_list;		//卖出列表
	char _market_name[PLAYER_MAERKET_NAME_LEN];
	size_t _market_name_len;
public:
#pragma pack(1)
	struct trade_request
	{
		int trade_id;
		size_t money;
		size_t count;
		struct entry_t
		{
			int type;
			unsigned short index;
			unsigned short inv_index;		//仅购买的时候有用
			unsigned short count;
		}list[];
	};
#pragma pack()
public:
	player_stall(item_list & inv):_inv(inv)
	{
		memset(_market_name,0,sizeof(_market_name));
		_market_name_len = 0;
	}
	
	void SetMarketName(const char name[PLAYER_MAERKET_NAME_LEN])
	{
		size_t i;
		for(i =0; i <PLAYER_MAERKET_NAME_LEN ; i +=2)
		{
			_market_name[i] = name[i];
			_market_name[i+1] = name[i+1];
			if(name[i] == 0 && name[i+1] == 0) 
			{
				i += 2;
				break;
			}
		}
		_market_name_len  = i;
	}

	inline const char *GetName()
	{
		return _market_name;
	}

	inline size_t GetNameLen()
	{
		return _market_name_len;
	}

	void AddTradeGoods(size_t index,int type, size_t count, size_t price)
	{
		entry_t ent;
		ent.type = type;
		ent.index = index;
		ent.count = count;
		ent.price = price;
		ent.expire_date = _inv[index].expire_date;
		ent.crc = _inv[index].GetCRC();
		_goods_list.push_back(ent);
	}

	void AddOrderGoods(size_t index,int type, size_t count, size_t price)
	{
		entry_t ent;
		ent.type = type;
		ent.index = 0xFFFF;
		ent.count = count;
		ent.price = price;
		ent.crc = 0;
		ent.expire_date = 0;
		_goods_list.push_back(ent);
	}

};
#endif

