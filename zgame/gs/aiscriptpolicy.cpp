#include <stdio.h>
#include "clstab.h"
#include "skillwrapper.h"
#include <amemory.h>
#include "npcsession.h"
#include "aipolicy.h"
#include "obj_interface.h"
#include <common/message.h>
#include <arandomgen.h>
#include "aitrigger.h"
#include "aiscriptpolicy.h"

DEFINE_SUBSTANCE(ai_script_policy,ai_policy, 3358)
ai_script_policy::ai_script_policy()
{
	_L = NULL;
}

ai_script_policy::~ai_script_policy()
{
	if(_L)
	{
		lua_close(_L);
	}
}

static int  script_AddPosTask(lua_State *L)
{
	int n = lua_gettop(L);    /* number of arguments */
	if(n != 4) return 0;	// self, x, y, z; errror???

	float x,y,z;
	ai_policy * pPolicy = (ai_policy*)lua_touserdata(L,1);
	x = lua_tonumber(L, 2);
	y = lua_tonumber(L, 3);
	z = lua_tonumber(L, 4);

	A3DVECTOR pos(x,y,z);
	pPolicy->AddPosTask<ai_patrol_task>(pos);

	lua_pushnumber(L,1);
	return 1;

}

static int  script_CheckSelfDistance(lua_State *L)
{
	int n = lua_gettop(L);    /* number of arguments */
	if(n != 4) return 0;	// self, x, y, z; errror???

	float x,y,z;
	ai_policy * pPolicy = (ai_policy*)lua_touserdata(L,1);
	x = lua_tonumber(L, 2);
	y = lua_tonumber(L, 3);
	z = lua_tonumber(L, 4);

	A3DVECTOR pos(x,y,z);
	A3DVECTOR spos;
	pPolicy->GetSelf()->GetPos(spos);
	lua_pushnumber(L,spos.horizontal_distance(pos));
	return 1;

}

static int  script_Say(lua_State *L)
{
	int n = lua_gettop(L);    /* number of arguments */
	if(n != 2) return 0;	// self, x, y, z; errror???

	ai_policy * pPolicy = (ai_policy*)lua_touserdata(L,1);
	const char * str = lua_tostring(L,2);
	pPolicy->GetSelf()->Say(str);
	return 0;

}

void ai_script_policy::Init(const ai_object & self, const ai_param & aip)
{
	ai_policy::Init(self,aip);
	if(aip.script_data)
	{
		lua_State *L = luaL_newstate ();
		luaL_openlibs(L);
		lua_register (L,"AddMoveTask",script_AddPosTask);
		lua_register (L,"CheckSelfDistance",script_CheckSelfDistance);
		lua_register (L,"Say",script_Say);

		if(luaL_loadbuffer(L,aip.script_data,aip.script_size,"aiscirpt_name") == 0)
		{
			lua_pcall(L, 0, LUA_MULTRET, 0);
			_L = L;

			lua_getfield(_L, LUA_GLOBALSINDEX, "Init");
			lua_pushlightuserdata(_L, this);
			lua_call(_L, 1, 0);
		}
		else
		{
			lua_close(L);
		}
	}
}

void ai_script_policy::OnAggro()
{
	ai_policy::OnAggro();
}

void ai_script_policy::OnHeartbeat()
{
	if(_L == NULL)
	{
		ai_policy::OnHeartbeat();
		return ;
	}

	if(_self->GetState() & ai_object::STATE_ZOMBIE) return;
	//判断寿命
	if(_life > 0)
	{
		if(--_life <= 0)
		{
			//让自己强行死亡
			XID id;
			_self->GetID(id);
			_self->SendMessage(id,GM_MSG_DEATH);
			_life = 1;
			return ;
		}
	}

	if(_idle_mode) return;
	if(_at_policy)
	{
		if(InCombat())
		{
			_at_policy->OnHeartbeat();
			_at_policy->OnPeaceHeartbeatInCombat();
		}
	}
	if(_cur_task) 
	{
		//优先做当前的任务
		_cur_task->OnHeartbeat();
		if(InCombat() && _self->GetAggroCount() == 0)
		{
			RollBack();
		}
	}
	else
	{
		if(!InCombat())
		{
			// 调用Heartbeat 无参数
			lua_getfield(_L, LUA_GLOBALSINDEX, "Heartbeat");
			lua_call(_L, 0, 0);
			return;
		}

		if(_self->GetAggroCount())
		{
			DeterminePolicy();
		}
		else
		{
			RollBack();
			return;
		}
	}

	_tick ++;
}

void ai_script_policy::Reborn()
{
	if(_L)
	{
		//.... reset...
	}
	ai_policy::Reborn();
}


