#include "battleflag_filter.h"
#include "clstab.h"
#include "statedef.h"


DEFINE_SUBSTANCE(battleflag_filter,filter,CLS_FILTER_BATTLEFLAG)

void battleflag_filter::OnAttach()
{
	_parent.EnhanceMaxHP(hp);
	_parent.EnhanceMaxMP(mp);
	_parent.EnhanceDamage(attack);
	_parent.EnhanceDefense(defence);

	_parent.EnhanceCrit(crit);
	_parent.EnhanceCritDmg(crit_damage);

	for(int i = 0; i < 6; i ++)
	{
		_parent.EnhanceResistance(i, resistance[i]);
	}

	for(int i = 0; i < CULT_DEF_NUMBER; i ++)
	{
		_parent.EnhanceCultDefense(i, cult_defense[i]);
		_parent.EnhanceCultAttack(i, cult_attack[i]);
	}

	_parent.UpdateHPMP();
	_parent.UpdateAttackData();
	_parent.UpdateDefenseData();
	_parent.UpdateMagicData();
}

void battleflag_filter::OnDetach()
{
	_parent.ImpairMaxHP(hp);
	_parent.ImpairMaxMP(mp);
	_parent.ImpairDamage(attack);
	_parent.ImpairDefense(defence);
	_parent.ImpairCrit(crit);
	_parent.ImpairCritDmg(crit_damage);

	 for(int i = 0; i < 6; i ++)
	 {
		 _parent.ImpairResistance(i, resistance[i]);
	 }

	 for(int i = 0; i < CULT_DEF_NUMBER; i ++)
	 {
		 _parent.ImpairCultDefense(i, cult_defense[i]);
		 _parent.ImpairCultAttack(i, cult_attack[i]);
	 }
	 _parent.UpdateHPMP();
	 _parent.UpdateAttackData();
	 _parent.UpdateDefenseData();
	 _parent.UpdateMagicData();
}

