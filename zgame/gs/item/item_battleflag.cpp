#include "item_battleflag.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"
#include "../actsession.h"
#include "../mount_filter.h"

int 
item_battleflag::OnUse(item::LOCATION l,size_t index, gactive_imp* imp,item * parent) const
{
	gplayer_imp *pImp = (gplayer_imp *)imp;
	if(pImp->HasNextSession()) return 0;

	//检查冷却时间
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_BATTLE_FLAG))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}

	pImp->SetCoolDown(COOLDOWN_INDEX_BATTLE_FLAG, _cooltime * 1000);

	//加入一个session
	class op : public session_general_operation::operation
	{
		int item_index;
		int item_type;
		item::LOCATION location;
		public:
		op(int index, int type, item::LOCATION l):item_index(index),item_type(type),location(l)
		{}

		virtual int GetID()
		{
			return S2C::GOP_BATTLEFLAG;
		}
		virtual bool NeedBoardcast()
		{
			return true;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(3.0f);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			if(!pImp->IsItemExist((int)location, item_index, item_type, 1)) return;
			pImp->UseBattleFlag(item_type, item_index);
		}
	};

	session_general_operation * pSession = new session_general_operation(pImp,  new op(index, parent->type, l));
	pImp->AddSession(pSession);
	pImp->StartSession();
	return 0;
}

