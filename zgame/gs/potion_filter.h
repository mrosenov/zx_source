#ifndef __ONLINEGAME_GS_POTION_FILTER_H__
#define __ONLINEGAME_GS_POTION_FILTER_H__
#include "filter.h"
#include "actobject.h"


//----------------------------------------------------------------------------------------------------

class base_potion_filter : public filter
{
	int _timeout;
protected:
	base_potion_filter() {}
	enum
	{
		MASK = FILTER_MASK_HEARTBEAT|FILTER_MASK_UNIQUE|FILTER_MASK_REMOVE_ON_DEATH
	};
public:
	base_potion_filter(gactive_imp * imp)
		:filter(object_interface(imp),MASK), _timeout(0)
	{
	}

	virtual void OnPotion(int index) = 0;

private:
	virtual void Heartbeat(int tick)
	{
		_timeout ++;
		if((_timeout & 0x01) == 0)
		{
			int index = _timeout >> 1;
			OnPotion(index);
		}

		if(_timeout >= 6) 
		{
			_is_deleted = true;
			return;
		}
	}
	virtual bool Save(archive & ar)
	{
		ASSERT(false);
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ASSERT(false);
		return true;
	}
};

class healing_potion_filter : public base_potion_filter
{
	int _life[4];

protected:
	healing_potion_filter(){}
public:
	DECLARE_SUBSTANCE(healing_potion_filter);
	healing_potion_filter(gactive_imp * imp,const int life[4])
		:base_potion_filter(imp)
	{
		_filter_id = FILTER_INDEX_HEALING;
		memcpy(_life, life, sizeof(_life));
	}

	virtual void OnAttach() 
	{
		_parent.Heal(_life[0], true, false );
		_parent.UpdateBuff(31, 1, 0);
	}
	
	virtual void OnDetach()
	{
		_parent.RemoveBuff(31, 1);
	}
	

private:
	virtual void OnPotion(int index)
	{
		if(index < 4 && index >= 1)
			_parent.Heal(_life[index], true, false);
	}
	
};

class mana_potion_filter : public base_potion_filter
{
	int _mana[4];

protected:
	mana_potion_filter(){}
	
public:
	DECLARE_SUBSTANCE(mana_potion_filter);
	mana_potion_filter(gactive_imp * imp,const int mana[4])
		:base_potion_filter(imp)
	{
		_filter_id = FILTER_INDEX_MANA;
		memcpy(_mana, mana, sizeof(_mana));
	}

	virtual void OnAttach() 
	{
		_parent.InjectMana(_mana[0], true);
		_parent.UpdateBuff(32, 1, 0);
	}
	virtual void OnDetach()
	{
		_parent.RemoveBuff(32, 1);
	}

private:
	virtual void OnPotion(int index)
	{
		if(index < 4 && index >= 1)
			_parent.InjectMana(_mana[index], true);
	}
	
};

class rejuvenation_potion_filter : public base_potion_filter
{
	int _mana[4];
	int _life[4];

protected:
	rejuvenation_potion_filter(){}
	
public:
	DECLARE_SUBSTANCE(rejuvenation_potion_filter);
	rejuvenation_potion_filter(gactive_imp * imp,const int life[4],const int mana[4])
		:base_potion_filter(imp)
	{
		_filter_id = FILTER_INDEX_REJUVENATION;
		memcpy(_mana, mana, sizeof(_mana));
		memcpy(_life, life, sizeof(_life));
	}

	virtual void OnAttach() 
	{
		_parent.InjectMana(_mana[0], true);
		_parent.Heal(_life[0], true, false);
		_parent.UpdateBuff(31, 1, 0);
		_parent.UpdateBuff(32, 1, 0);
	}
	virtual void OnDetach()
	{
		_parent.RemoveBuff(31, 1);
		_parent.RemoveBuff(32, 1);
	}

private:
	virtual void OnPotion(int index)
	{
		if(index < 4 && index >= 1)
		{
			_parent.InjectMana(_mana[index], true);
			_parent.Heal(_life[index], true, false);
		}
	}
	
};

class deity_potion_filter : public base_potion_filter
{
	int _dp[4];

protected:
	deity_potion_filter(){}
public:
	DECLARE_SUBSTANCE(deity_potion_filter);
	deity_potion_filter(gactive_imp * imp,const int dp[4])
		:base_potion_filter(imp)
	{
		_filter_id = FILTER_INDEX_DEITY;
		memcpy(_dp, dp, sizeof(_dp));
	}

	virtual void OnAttach() 
	{
		_parent.InjectDeity(_dp[0], true);
		_parent.UpdateBuff(190, 1, 0); 
	}
	
	virtual void OnDetach()
	{
		_parent.RemoveBuff(190, 1);
	}
	

private:
	virtual void OnPotion(int index)
	{
		if(index < 4 && index >= 1)
			_parent.InjectDeity(_dp[index], true);
	}
	
};

#endif

