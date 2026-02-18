#ifndef __ONLINEGAME_GS_KINGDOM_BATTLEPREPARE_FILTER_H__
#define __ONLINEGAME_GS_KINGDOM_BATTLEPREPARE_FILTER_H__

#include "../filter.h"
#include "../actobject.h"

class kingdom_battleprepare_filter : public timeout_filter
{
protected:
	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT | FILTER_MASK_TRANSLATE_ENCHANT | FILTER_MASK_TRANSLATE_RECV_MSG
	};
	kingdom_battleprepare_filter(){}

public:
	DECLARE_SUBSTANCE(kingdom_battleprepare_filter);
	kingdom_battleprepare_filter(gactive_imp * imp, int filter_id, short timeout)
		: timeout_filter(object_interface(imp), timeout, FILTER_MASK)
	{
		_filter_id = filter_id;
	}	     

private:
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg);
	virtual void TranslateRecvEnchant(const XID & attacker,enchant_msg & msg);

	virtual void OnAttach();
	virtual void OnDetach(); 

};

#endif
