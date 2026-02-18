#include "item_mount.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"
#include "../actsession.h"
#include "../mount_filter.h"

bool 
item_mount::IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const 
{
	return  (/*gmatrix::IsBattleServer() ||*/ !_battle_ground_only);
}

int 
item_mount::OnUse(item::LOCATION l,size_t index, gactive_imp* imp,item * parent) const
{
	gplayer_imp *pImp = (gplayer_imp *)imp;

	if(pImp->GetWorldManager()->GetWorldLimit().nomount)
	{
		return 0; 
	}
	
	//检查player是否处于骑乘状态,如果是则取消当前骑乘
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER))
	{
		//取消骑乘
		pImp->_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
		return 0;
	}

	if(pImp->HasNextSession()) return 0;

	//不在骑乘状态
	if(_level_required > pImp->_basic.level)
	{
		pImp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}
	
	if(_reborn_required > pImp->GetRebornCount())
	{
		pImp->_runner->error_message(S2C::ERR_REBORN_COUNT_NOT_MATCH);
		return -1;
	}

	if(_kingdom_title_required > 0 && (pImp->GetKingdomTitle() <= 0 || !(1 << pImp->GetKingdomTitle()-1 & _kingdom_title_required)))
	{
		return -1;
	}

	//变身状态下不让骑马
	if(pImp->IsTransformState() || pImp->GetShape() == 3 || pImp->GetShape() == 4 || pImp->GetShape() == 5 || pImp->GetShape() == 6)
	{
		return -1;
	}

	if(_loli_only && !pImp->IsLoliOccupation())
	{
		return -1;
	}

	//检查冷却时间
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_MOUNT))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}

	pImp->SetCoolDown(COOLDOWN_INDEX_MOUNT,10000);

	//加入一个session
	class op : public session_general_operation::operation
	{
		int item_index;
		int item_type;
		int mount_id;
		char mount_type;
		float speed;
		item::LOCATION location;
		public:
		op(int index, int type,int id, float sp, char m_type, item::LOCATION l):item_index(index),item_type(id),mount_id(id), mount_type(m_type), speed(sp), location(l)
		{}

		virtual int GetID()
		{
			return S2C::GOP_MOUNT;
		}
		virtual bool NeedBoardcast()
		{
			return true;
		}
		virtual int GetDuration()
		{
			return SECOND_TO_TICK(1.0f);
		}
		virtual void OnStart(gactive_imp * pImp)
		{
			//do nothing
		}
		virtual void OnEnd(gactive_imp * obj)
		{
			gplayer_imp * pImp = (gplayer_imp *) obj;
			if(!pImp->IsItemExist((int)location, item_index, item_type,1)) return ;
			if(pImp->_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER)) return;
			item & it = pImp->GetInventory((int)location)[item_index];
			int addons[4];
			CollectAddons(it, addons, 4);
			pImp->_filters.AddFilter(new mount_filter(pImp,FILTER_INDEX_MOUNT_FILTER, mount_id, mount_type, 1,speed,0.0, addons));
			if(it.IsBindOnEquip())
			{
				it.BindOnEquip();
				pImp->PlayerGetItemInfo(location, item_index);
				GLog::log(LOG_INFO,"用户%d骑宠绑定%d,GUID(%d,%d)",pImp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
				
				//检查是否需要记录消费值
				pImp->CheckUseItemConsumption(it.type, 1);
			}
		}

	};
	gplayer * pObj = (gplayer*)pImp->_parent;
	if(pObj->CheckObjectState(gactive_object::STATE_IN_BIND) && (pObj->bind_type == 2 || pObj->bind_type == 4))
	{
		//在上面的时候不能骑乘
		return -1;
	}

	if(pObj->CheckObjectState(gactive_object::STATE_FLY)) 
	{
		//飞行的时候不能骑乘
		return -1;
	}

	if (pObj->mobactive_id > 0)
	{
		//互动状态中不让骑马
		return -1;
	}

	if(pObj->IsInvisible())
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_INVISIBLE);
	}
	
	session_general_operation * pSession = new session_general_operation(pImp,  new op(index, parent->type,_mount_id, _mount_speed, _mount_type, l));
	pImp->AddSession(pSession);
	pImp->StartSession();
	return 0;
}

void item_mount::CollectAddons(item & parent, int * addons, size_t count)
{
	memset(addons, 0, sizeof(int)* count);

	size_t len;
	const void * buf = parent.GetContent(len);
	if(len < sizeof(int) || (len & 0x03)) return;
	const int * list = (const int *) buf;
	if(list[0] <0 || list[0] > 128) return ;
	if(len != list[0] * sizeof(int) + sizeof(int)) return;
	size_t count_p = list[0];
	for(size_t i = 0; i < count_p && i < count; i ++)
	{
		addons[i] = list[1 + i];
	}
}


void item_mount::OnTakeOut(item::LOCATION l,size_t index, gactive_imp*obj,item * parent) const
{
	if( l == item::INVENTORY || l == item::MOUNT_WING_INVENTORY)
	{
		gplayer_imp * pImp = (gplayer_imp *)obj;
		gplayer * pPlayer  = pImp->GetParent();
		if(pPlayer->CheckObjectState(gactive_object::STATE_MOUNT) &&  pPlayer->mount_id == parent->type)
		{       
			obj->_filters.RemoveFilter(FILTER_INDEX_MOUNT_FILTER);
		}
	}
}

