#ifndef __ONLINEGAME_GS_POPPET_ITEM_H__
#define __ONLINEGAME_GS_POPPET_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include <crc.h>

class item_poppet : public item_body
{
public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POPPET;}
	virtual void OnPutIn(item::LOCATION  l ,size_t index , gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index, gactive_imp*,item * parent) const;
};
/*
class item_poppet_item : public item_body
{
public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POPPET_ITEM;}
	virtual void OnPutIn(item::LOCATION  l ,size_t index , gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index, gactive_imp*,item * parent) const;

};

class item_poppet_exp : public item_body
{

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POPPET_EXP;}
	virtual void OnPutIn(item::LOCATION ,size_t index , gactive_imp*,item * parent) const;
	virtual void OnTakeOut(item::LOCATION ,size_t index, gactive_imp*,item * parent) const;

};
*/
#endif

