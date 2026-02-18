#include "item_telestation.h"
#include "../player_imp.h"
#include "../actsession.h"

extern abase::timer      g_timer;


int
item_telestation::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	size_t len;
	const void * buf = parent->GetContent(len);
	if(len < 2*sizeof(int) )return -1;
	gplayer_imp * pImp = (gplayer_imp * ) imp;

	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	int effect_time  =  *(int*)buf;
	if(effect_time == 0)
	{
		*(int*)buf = g_timer.get_systime() + _day*3600*24;
		parent->Bind();	
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
		//检查是否需要记录消费值
		pImp->CheckUseItemConsumption(parent->type, 1);
	}
	return 0;
}

int 
item_telestation::OnUse(item::LOCATION l,size_t index,gactive_imp* obj,item* parent,const char* arg,size_t arg_size) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;

	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	if(arg_size < sizeof(int) || arg_size > sizeof(use_arg2)) return -1;

	size_t len;
	const void* buf = parent->GetContent(len);
	int op_type = *(int*)((char*)arg);
	if(op_type == 1 || op_type == 2 || op_type == 3)
	{
		if(arg_size != sizeof(use_arg)) return -1;
		int op_index  = *(int*)((char*)arg+4);

		//检查是否存在
		if(len < 2*sizeof(int) )return -1;

		//检查有效时间
		int effect_time  =  *(int*)buf;
		if(g_timer.get_systime() > effect_time) return -1;


		//记录位置
		if(op_type == 1)
		{
			if(AddTelePosition(pImp, parent, buf, len)) 
			{
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
				return 0;
			}
		}
		//删除位置
		else if(op_type == 2)
		{
			if(DelTelePosition(pImp, parent, buf, len, op_index)) 
			{
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
				return 0;
			}

		}
		//使用位置
		else if(op_type == 3)
		{
			if(UseTelePosition(pImp, parent, buf, len, op_index)) return 0;
		}
	}
	//添加或者修改备注
	else if(op_type == 4)
	{
		AddTeleMemo(pImp, parent, buf, len, arg, arg_size); 
	}
	else
	{
		return -1;
	}


	return -1;
}

bool
item_telestation::AddTelePosition(gplayer_imp *pImp, item * parent, const void * buf, size_t len) const
{
	if(!pImp->GetWorldManager() || pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_IN);
		return false;
	}
	if(len < 2*sizeof(int) )return false;
	size_t count = *(size_t*)((char*)buf+4);
	if(count >= (size_t)_max_count) return false;

	*(size_t*)((char*)buf+4) = count +1;

	tele_pos new_pos;
	new_pos.tag = pImp->GetWorldTag();
	new_pos.x = pImp->_parent->pos.x;
	new_pos.y = pImp->_parent->pos.y;
	new_pos.z = pImp->_parent->pos.z;

	packet_wrapper h1(256);
	h1.push_back(buf, len);
	h1.push_back(&new_pos, sizeof(tele_pos));

	parent->SetContent(h1.data(), h1.size());
	return true;
}

bool
item_telestation::DelTelePosition(gplayer_imp *pImp, item * parent, const void * buf, size_t len, int index) const
{
	if(index < 0) return false;
	if(len < 2*sizeof(int) + (index+1) * sizeof(tele_pos)) return false;
	
	int effect_time  =  *(int*)buf;
	size_t count = *(size_t*)((char*)buf+4);
	if(count > (size_t)_max_count) return false;
	if((size_t)index >= count) return false;

	packet_wrapper h1(256);
	if(IsOldEssence(count, len))
	{
		abase::vector<tele_pos> pos_list((tele_pos*)((char*)buf+ 2*sizeof(int)), (tele_pos*)((char*)buf+ 2*sizeof(int))+count); 
		pos_list.erase(pos_list.begin() + index);
		--count;

		h1 << effect_time;
		h1 << count;
		if(pos_list.size() > 0) h1.push_back(&pos_list[0], pos_list.size()*sizeof(tele_pos));
	}
	else if(IsNewEssence(count, len))
	{
		abase::vector<tele_pos> pos_list((tele_pos*)((char*)buf+ 2*sizeof(int) + sizeof(char)*16), (tele_pos*)((char*)buf+ 2*sizeof(int)+ sizeof(char)*16)+count); 
		pos_list.erase(pos_list.begin() + index);
		--count;

		h1 << effect_time;
		h1 << count;
		h1.push_back((char*)buf+2*sizeof(int), 16);
		if(pos_list.size() > 0) h1.push_back(&pos_list[0], pos_list.size()*sizeof(tele_pos));
	}
	else
	{
		return false;
	}

	parent->SetContent(h1.data(), h1.size());
	return true;
}



bool
item_telestation::UseTelePosition(gplayer_imp *pImp, item * parent, const void * buf, size_t len, int index) const
{
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
			return S2C::GOP_TELESTATION;
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
				return;
			}
			else
			{
				pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
				return;
			}
		}
	};

	if(len < 2*sizeof(int) )return false;
	size_t count = *(size_t*)((char*)buf+4);
	if(count > (size_t)_max_count) return false;
	if(index < 0 || (size_t)index >= count) return false;

	tele_pos pos;
	if(IsOldEssence(count, len))
	{
		pos = *(tele_pos*)((char*)buf + 2*sizeof(int) + index*sizeof(tele_pos));
	}
	else if(IsNewEssence(count, len))
	{
		pos = *(tele_pos*)((char*)buf + 2*sizeof(int) + 16*sizeof(char) + index*sizeof(tele_pos));
	}
	else
	{
		return false;
	}

	int reborn_cnt = 0;
	int max_level = 0;
	int min_level = 0;
	int kick_city = 0;
	A3DVECTOR  kick_pos;

	world_manager * target_wm = gmatrix::FindWorld(pos.tag);
	if(target_wm && target_wm->GetCityRegion()->GetRegionLimit(pos.x, pos.z, reborn_cnt, max_level, min_level, kick_city, kick_pos))
	{
		int reborn_level =  reborn_cnt * 150; 
		int player_level = pImp->GetObjectLevel() + pImp->GetRebornCount() * 150; 
		if((max_level > 0 && player_level > max_level + reborn_level) || (min_level > 0 && player_level < min_level + reborn_level)) 
		{
			return false;
		}
	}

	float time = pImp->IsCombatState() ? 6.0 : 3.0;
	session_general_operation * pSession = new session_general_operation(pImp,  new op(pos.x, pos.y, pos.z, pos.tag, time));
	pImp->AddSession(pSession);
	pImp->StartSession();
	return true;
}

void 
item_telestation::InitFromShop(gplayer_imp* pImp,item* parent,int value) const
{
	struct tele_ess
	{
		int effect_time;
		int count;
	};

	size_t len;
	parent->GetContent(len);
	if(len == 0)
	{
		tele_ess ess;
		ess.effect_time = 0; 
		ess.count = 0;
		parent->SetContent(&ess,  sizeof(tele_ess));	
	}
}


bool 
item_telestation::Charge(item *parent, gactive_imp *imp, size_t index1, size_t index2, int day)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	item_list &inv = pImp->GetInventory();
	item &it1 = inv[index1];
	ASSERT(it1.GetItemType() == ITEM_TYPE_TELESTATION);
	
	size_t len;
	void * buff = parent->GetContent(len);
	if(len < sizeof(int) * 2 || (len % sizeof(int)) != 0) return false;

	//检查有效时间
	int effect_time  =  *(int*)buff;
	int now = g_timer.get_systime();

	//失效了
	if(now > effect_time)
	{
		effect_time = now + day*24*3600;
	}
	else
	{
		effect_time = effect_time + day*24*3600;
	}

	*(int*)buff = effect_time;
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index1);
	return true;
}

bool
item_telestation::AddTeleMemo(gplayer_imp * pImp, item * parent, const void * buf, size_t len, const char * arg, size_t arg_size) const
{
	if(len < 2*sizeof(int) )return false;
	if(arg_size < sizeof(int) || arg_size > sizeof(int) + sizeof(char) * 16) return false;
	int effect_time  =  *(int*)buf;
	size_t count = *(size_t*)((char*)buf+4);
	
	char memo[16];
	memset(&memo, 0, sizeof(memo));
	if(arg_size > sizeof(int)) memcpy(memo, (char*)arg+sizeof(int), arg_size-sizeof(int));
	
	packet_wrapper h1(256);
	h1 << effect_time;
	h1 << count;
	h1.push_back(memo, 16);
	if(IsOldEssence(count, len))
	{
		if(len - 2 *sizeof(int) > 0)
		{
			h1.push_back((char*)buf+sizeof(int)*2, len - 2* sizeof(int));
		}

	}
	else if(IsNewEssence(count, len))
	{
		if(len - 2 * sizeof(int) - 16 * sizeof(char) > 0)
		{
			h1.push_back((char*)buf+sizeof(int)*2+sizeof(char)*16, len - 2* sizeof(int) - 16* sizeof(char));
		}
	}
	else
	{
		return false;
	}
	parent->SetContent(h1.data(), h1.size());
	return true;
}



bool
item_telestation::IsOldEssence(size_t count, size_t size) const
{
	return size == (sizeof(int)*2 + count * sizeof(int) * 4);  
}


bool
item_telestation::IsNewEssence(size_t count, size_t size) const
{
	return size == (sizeof(int)*2 + count * sizeof(int) * 4 + 16 * sizeof(char));
}

