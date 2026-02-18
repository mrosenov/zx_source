#ifndef __ONLINEGAME_GS_MOUNT_FILTER_H__
#define __ONLINEGAME_GS_MOUNT_FILTER_H__
#include "filter.h"
#include "item/item_addon.h"

class mount_filter : public filter
{
	int _mount_id;
	int _mount_lvl;
	float _speedup;
	float _drop_rate;
	char _mount_type; // 0 -- ÆÕÍ¨Õ½Æï  1 --¶àÈËÕ½Æï

	struct {
		int  id;
		addon_data data;
		addon_handler * handler;
	}_addons[4];
	bool _no_broadcast;
public:
	DECLARE_SUBSTANCE(mount_filter);
	mount_filter() {}
	mount_filter(gactive_imp * imp,int filter_id, int mount_id, char mount_type, int mount_lvl, float speed_up,float drop_rate,int addons[4], bool no_broadcast = false)
		:filter(object_interface(imp),/*FILTER_MASK_ADJUST_DAMAGE|*/FILTER_MASK_MERGE|FILTER_MASK_REMOVE_ON_DEATH),_no_broadcast(no_broadcast)
	{
		_filter_id = filter_id;
		_mount_id = mount_id;
		_mount_type = mount_type;
		_mount_lvl = mount_lvl;
		_speedup = speed_up;
		_drop_rate = drop_rate;
		memset(_addons, 0, sizeof(_addons));
		for(size_t i = 0; i < 4; i ++)
		{
			_addons[i].id = addons[i];
		}
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void AdjustDamage(damage_entry&,  const XID &, const attack_msg&);
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	virtual void Merge(filter * f);
};

#endif

