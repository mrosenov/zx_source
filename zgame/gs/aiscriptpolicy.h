#ifndef __ONLINE_GAME_GS_AI_SCRIPT_POLICY_H__
#define __ONLINE_GAME_GS_AI_SCRIPT_POLICY_H__

#include "aipolicy.h"
#include <lua.hpp>

class ai_script_policy : public ai_policy
{
protected:
	lua_State * _L;

public:
DECLARE_SUBSTANCE(ai_script_policy);

	ai_script_policy();
	~ai_script_policy();

	bool Save(archive & ar)
	{
		return false;
	}
	
	bool Load(archive & ar)
	{
		return false;
	}

	virtual void Init(const ai_object & self, const ai_param & aip);
	virtual float GetReturnHomeRange() { return 1e20;}
public:
	virtual void OnAggro();			//仇恨度变化事件
	virtual void OnHeartbeat();		//心跳
	virtual void Reborn();
};

#endif

