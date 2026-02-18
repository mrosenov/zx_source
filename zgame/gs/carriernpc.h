#ifndef __ONLINEGAME_GS_CARRIER_NPC_H__
#define __ONLINEGAME_GS_CARRIER_NPC_H__

#include "servicenpc.h"
#include <set>

class carrier_npc : public service_npc
{
private:
	std::set<int> _player_set;	//´¬ÉÏÍæ¼Ò

	enum
	{
	
		MAX_CARRIER_RANGE = 50,
	};
public:
	DECLARE_SUBSTANCE(carrier_npc);


public:
	virtual int MessageHandler(const MSG& msg);
	virtual void OnHeartbeat(size_t tick);
	virtual bool StepMove(const A3DVECTOR& offset);

};

#endif

