#ifndef __ONLINEGAME_GS_TELESTATION_ITEM_H__
#define __ONLINEGAME_GS_TELESTATION_ITEM_H__

#include "../item.h"

/*
 *  老的存储结构
 *	struct old_item_telestation_essence  
 *	{
 *		int effect_time;
 *		int count;
 *		struct tele_pos;
 *		{
 *			int tag;	
 *			float x;
 *			float y;
 *			float z;
 *		}[count];
 *	}
 */

/*
 *  新的存储结构(给新盘添加注释)
 *	struct new_item_telestation_essence  
 *	{
 *		int effect_time;
 *		int count;
 *		char memo[16]; 
 *		struct tele_pos;
 *		{
 *			int tag;	
 *			float x;
 *			float y;
 *			float z;
 *		}[count];
 *	}
 */


class item_telestation : public item_body
{
	int _max_count;
	int _day;

	struct tele_pos
	{
		int tag;
		float x;
		float y;
		float z;
	};

	struct use_arg
	{
		int op_type;	//1: 记录位置   2: 删除位置   3: 使用位置  4:修改/添加备注
		int op_arg;
	};	

	struct use_arg2
	{
		int op_type;
		char memo[16];	//备注
	};

public:
	item_telestation(int count, int day) : _max_count(count), _day(day)
	{}
	
public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TELESTATION;}
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return true; }
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* obj,item* parent,const char* arg,size_t arg_size) const;
	virtual int OnGetUseDuration() const { return -1;}
	virtual void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;
	virtual bool Charge(item *parent, gactive_imp *imp, size_t index1, size_t index2, int day);

private:
	bool AddTelePosition(gplayer_imp *pImp, item * parent, const void * buf, size_t len) const;
	bool DelTelePosition(gplayer_imp *pImp, item * parent, const void * buf, size_t len, int index) const;
	bool UseTelePosition(gplayer_imp *pImp, item * parent, const void * buf, size_t len, int index) const; 
	bool AddTeleMemo(gplayer_imp * pImp, item * parent, const void * buf, size_t len, const char * arg, size_t arg_size) const; 
	bool IsOldEssence(size_t count, size_t size) const;
	bool IsNewEssence(size_t count, size_t size) const;
};

#endif
