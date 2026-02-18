#include "item_lottery_ty.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"
#include "../item_manager.h"

int
item_lottery_ty::CheckScript(int id)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_LOTTERY);
	lua_State * L = keeper.GetState();
	if(L == NULL) return 1;

	int exp_get = 0;
	int exp_level[7];
	memset(exp_level, 0, sizeof(exp_level));
	
	item_lottery_ty l(exp_get, exp_level, 0, 0);
	l.SetTID(id);

	int exp = 0;
	char ticket = 0;
	if(!l.GenerateTicket(L, exp, ticket)) 
	{
		return -1;
	}

	int m[6];
	if(!l.GetLotteryBonus(L, m, ticket)) return -2;
	return 0;
}

int
item_lottery_ty::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_LOTTERY))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	player_mallinfo& mall = pImp->MallInfo();
	if(mall.GetCash()< _cost_item_cnt * 100)
	{
		return -1;
	}

	bool bModify = false;
	bool bError = false;

	lottery_ty_essence* ess = GetEssenece(parent);
	if(!ess)
	{
		return -1;
	}

	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_LOTTERY);
	lua_State * L = keeper.GetState();
	ASSERT(L);	
	//还需要再次生成
	if(GenerateTicket(L, ess->exp, ess->ticket))
	{
		//生成成功
		bModify = true;
	}
	else
	{
		bError = true;
	}
	
	int cooltime = LOTTERY_COOLDOWN_TIME;
	if(!bError) 
	{
		cooltime = 0;
	}
	else
	{
		cooltime += 15000;
	}
	if(cooltime) 
	{
		pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,cooltime);
	}
	pImp->UseCashLotteryTY(_cost_item_tid, _cost_item_cnt);
	
	if(bModify)
	{
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
	}

	if (item_manager::IsCashItem((size_t)_cost_item_cnt)) 
	{
		pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_LIANDANLU);
	}
	return 0;
}

bool 
item_lottery_ty::LotteryCashing(gactive_imp* obj, item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period)
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_LOTTERY))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return false;
	}

	lottery_ty_essence* ess = GetEssenece(parent);	
	if(!ess)
	{
		return false;
	}
	if(ess->ticket == 0)
	{
		return false;
	}
	pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,LOTTERY_COOLDOWN_TIME);

	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_LOTTERY);
	lua_State * L = keeper.GetState();
	ASSERT(L);	

	//调用脚本发奖
	int result[6];
	if(!GetLotteryBonus(L, result, ess->ticket))
	{
		return false;
	}

	//设置参数
	*bonus_level	= result[0];
	*prize_money	= result[1];
	*prize_id		= result[2];
	*prize_count	= result[3];
	*prize_bind 	= result[4];
	*prize_period	= result[5];

	if(*bonus_level >= 1 && *bonus_level <= 3)
	{
		pImp->LotteryTySendMessage(*prize_id);
	}

	return true;
}

bool 
item_lottery_ty::GenerateTicket(lua_State * L, int& exp, char& ticket) const 
{
	lua_getfield(L, LUA_GLOBALSINDEX, "Entry_Lottery_Generate");
	lua_pushinteger(L, _tid);
	lua_pushinteger(L, GetLevelByExp(exp));
	lua_pushinteger(L, ticket);
	if(lua_pcall(L, 3, 1, 0)) 
	{
		printf("error GenerateTicket %s\n", lua_tostring(L, -1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1)) 
	{
		return false;
	}
	int rst = lua_tointeger(L,-1);
	lua_pop(L,1);
	ticket = rst & 0xFF;

	if(exp < _exp_level[6])
	{
		float r = abase::Rand(0.0f, 1.0f);
		int ratio = 0;
		float curr = 0;
		for(int i = 0; i < 5; i ++)
		{
			curr += gmatrix::GetLotteryTyExpProb(i);
			if(r < curr * 0.001f)
			{
				ratio = i;
				break;
			}
		}
		exp += (_exp_get * (1 + ratio));
		if(exp > _exp_level[6])
		{
			exp = _exp_level[6];
		}
	}
	return true;
}

bool 
item_lottery_ty::GetLotteryBonus(lua_State * L, int n[6], char& ticket) const 
{
	lua_getfield(L, LUA_GLOBALSINDEX, "Entry_Lottery_Cashing");
	lua_pushinteger(L, _tid);
	lua_pushinteger(L, ticket);
	if(lua_pcall(L, 2, 6, 0)) 
	{
		printf("error GetLotteryBonus %s\n", lua_tostring(L, -1));
		lua_pop(L,1);
		return false;
	}
	for(int i = -6; i <= -1; i ++)
	{
		if(!lua_isnumber(L, i)) return false;
		n[i+6] = lua_tointeger(L, i);
	}
	lua_pop(L,6);

	//进行错误检查
	if(!CheckBonusLevel(n[0])) return false;
	if(!CheckBonusMoney(n[1])) return false;
	if(!CheckBonusItem(n[2],n[3])) return false;
	if(!CheckBonusBindPeriod(n[4],n[5])) return false;
	return true;
}

int item_lottery_ty::GetLevelByExp(int exp) const
{
	int max_level = sizeof(_exp_level) / sizeof(int);
	for(int i = 0; i < max_level; i ++)
	{
		if(exp < _exp_level[i])
		{
			return i;
		}
	}
	return max_level;
}

