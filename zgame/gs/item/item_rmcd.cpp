#include "item_rmcd.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

item_remove_cd::item_remove_cd(int cooltime, int skills[], size_t count):_cooltime(cooltime),_skills_count(0)
{
	for(size_t i = 0; i < count && _skills_count < SKILL_MAX_COUNT; i++)
	{
		if(skills[i] <= 0) continue;
		_skills[_skills_count] = skills[i];
		_skills_count ++;
	}
}

int
item_remove_cd::CheckUseCondition(gactive_imp* pImp) const
{
	if(_skills_count == 0) return S2C::ERR_USE_ITEM_FAILED;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_CD_ERASER)) return S2C::ERR_OBJECT_IS_COOLING;

	int cd_count = 0;
	for(size_t i = 0; i < _skills_count; i++)
	{
		//check need cooldown
		cd_count ++;
	}
	if(cd_count == 0) return S2C::ERR_SKILL_IS_NOT_COOLING;
	return 0;
}

int 
item_remove_cd::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp * ) obj;

	if(int errcode = CheckUseCondition(pImp))
	{
		pImp->_runner->error_message(errcode);
		return -1;
	}


	pImp->SetCoolDown(COOLDOWN_INDEX_CD_ERASER,_cooltime);
	for(size_t i = 0; i < _skills_count; i++)
	{
		int cd_idx = 0;
		//get skill cooldown index
		pImp->ClrCoolDown(cd_idx);
	}

	return 1;
}

