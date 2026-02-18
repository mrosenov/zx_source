#ifndef __ONLINEGAME_GS_PROPADD_ITEM_H_
#define __ONLINEGAME_GS_PROPADD_ITEM_H_

#include "../item.h"
#include "../config.h"
#include "../template/exptypes.h"

class item_propadd : public item_body
{
	int _material_id;
	int _require_level;
	int _require_reborn_count;
	int _prop_add[PROP_ADD_NUM];
public:	
	item_propadd(int material_id, int require_level, int renascence_count, const int prop_add[PROP_ADD_NUM])
	{
		_material_id = material_id;
		_require_level = require_level;
		_require_reborn_count = renascence_count;
		memcpy(_prop_add, prop_add, sizeof(_prop_add));
	}
	
	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_PROPADD;}
	virtual	bool IsItemCanUse(item::LOCATION l, gactive_imp *pImp) const {return true;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;

	virtual int GetPropAddMaterialId() { return _material_id; }
};

#endif /*__ONLINEGAME_GS_PROPADD_ITEM_H_*/
