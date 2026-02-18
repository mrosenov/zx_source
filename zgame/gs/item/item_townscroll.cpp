#include "item_townscroll.h"
#include "../player_imp.h"
#include "../actsession.h"
#include "../cooldowncfg.h"

int 
item_townscroll_paper::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;

	if(!pImp->GetWorldManager() || pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_IN);
		return -1;
	}

	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	item_list & list = pImp->GetInventory();
	if(list.GetEmptySlotCount() > 0)
	{
		int id = g_config.item_townscroll_id;

		//物品存在且是卷轴
		DATA_TYPE dt;
		if(!gmatrix::GetDataMan().get_data_ptr(id, ID_SPACE_ESSENCE,dt) || dt != DT_TOWNSCROLL_ESSENCE)
		{
			return -1;
		}

		//物品可生成
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(id);
		if(pItem == NULL || pItem->pile_limit != 1) return -1;

		struct
		{
			int tag;
			float x, y,z;
		} pos;
		pos.tag = pImp->GetWorldTag();
		pos.x = pImp->_parent->pos.x;
		pos.y = pImp->_parent->pos.y;
		pos.z = pImp->_parent->pos.z;

		item_data data = *pItem;
		data.content_length = sizeof(pos);
		data.item_content = (char*)&pos;

		item_data * pData = DupeItem(data);
		if(pImp->ObtainItem(gplayer_imp::IL_INVENTORY,pData)) FreeItem(pData);
		return 1;
	}
	else
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return -1;
	}
}

int 
item_townscroll::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	/*
	if(!pImp->GetWorldManager() | pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
		return -1;
	}
	*/

	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(ts_pos))
	{
		return -1;
	}

	//加入一个session
	class op : public session_general_operation::operation
	{
		float _x;
		float _y;
		float _z;
		int _tag;
		float _time;
		public:
		op(float x, float y, float z, int tag, float time): _x(x),_y(y),_z(z), _tag(tag), _time(time)
		{}

		virtual int GetID()
		{
			return S2C::GOP_TOWNSCROLL;
		}
		virtual bool NeedBoardcast()
		{
			return false;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(_time);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp * ) obj;
			if(pImp->LongJump(A3DVECTOR(_x,_y,_z), _tag))
			{
				return ;
			}
			else
			{
				pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
				return ;
			}
		}
	};

	float time = pImp->IsCombatState() ? 6.0 : 3.0;
	ts_pos &pos = *(ts_pos *)buf;
	session_general_operation * pSession = new session_general_operation(pImp,  new op(pos.x, pos.y, pos.z, pos.tag, time));
	pImp->AddSession(pSession);
	pImp->StartSession();

	return 1;
}


