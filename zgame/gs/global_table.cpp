#include "global_table.h"
#include "config.h"
#include "lua.hpp"
#include "gmatrix.h"
#include <hashmap.h>
#include <hashtab.h>

#define MAX_REINFORCE_LEVEL 21

struct LevelSpirit
{
	LevelSpirit() { memset(spirit, 0, sizeof(spirit)); }
	int spirit[MAX_REINFORCE_LEVEL];
};

typedef abase::hash_map<int, LevelSpirit*> SpiritTable;
SpiritTable spirit_table;

int GetSpiritMax(unsigned int equip_level, unsigned int enhance_level)
{
	ASSERT(enhance_level < MAX_REINFORCE_LEVEL && enhance_level >= 0);

	SpiritTable::iterator it = spirit_table.find(equip_level);
	ASSERT(it != spirit_table.end());
	return it->second->spirit[enhance_level];
}

bool LoadSpiritConfig(const char * filename)
{
	lua_State *L = luaL_newstate ();
	luaL_openlibs(L);

	if(luaL_dofile(L,filename))
	{       
		return false;
	}       

	lua_getglobal(L, "soul_power");
	if (!lua_istable(L, -1))
	{
		return false;
	}
	lua_pushnil(L);
	while (0 != lua_next(L, -2))
	{
		if (!lua_isnumber(L, -2) || !lua_istable(L, -1))
		{
			return false;
		}
		int level = (int)lua_tonumber(L, -2);
		LevelSpirit * table = new LevelSpirit();
		spirit_table[level] = table;

		lua_pushnil(L);
		while (0 != lua_next(L, -2))
		{
			if (!lua_isnumber(L, -2) || !lua_isnumber(L, -1))
			{
				return false;
			}
			int idx = (int)lua_tonumber(L, -2);
			int spirit = (int)lua_tonumber(L, -1);
		
			table->spirit[idx-1] = spirit;
			lua_pop(L, 1);
		}
		lua_pop(L, 1 );
	}
	lua_close(L);
	return true;
}

bool LoadMenologyConfig()
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_MISC);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;
	
	lua_getfield(L, LUA_GLOBALSINDEX, "Menology_Init");

	if(lua_pcall(L, 0, 1,0))
	{
		lua_pop(L,1);
		return false;
	}
	lua_pop(L,1);
	return true;
}


