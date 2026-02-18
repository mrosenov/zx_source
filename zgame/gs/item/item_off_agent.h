#ifndef __ONLINEGAME_GS_OFFLINE_AGENT_ITEM_H__
#define __ONLINEGAME_GS_OFFLINE_AGENT_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_offline_agent : public item_body
{
	int _agent_time;
public:
	item_offline_agent(int time):_agent_time(time)
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_OFFLINE_AGENT;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(1);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return false;}
};
#endif



