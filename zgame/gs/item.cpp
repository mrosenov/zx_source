#include "gmatrix.h"
#include "item.h"
#include "clstab.h"
#include "world.h"
#include <common/protocol.h>
#include "template/itemdataman.h"
#include "item_manager.h"

DEFINE_SUBSTANCE_ABSTRACT(item_body,substance,CLS_ITEM)

DEFINE_SUBSTANCE(gmatter_item_imp,gmatter_item_base_imp,CLS_MATTER_ITEM_IMP)
DEFINE_SUBSTANCE(gmatter_item_controller,gmatter_imp,CLS_MATTER_ITEM_CONTROLLER)


bool MakeItemEntry(item& entry,const item_data & data)
{
	entry.type = data.type;
	entry.count = data.count;
	entry.pile_limit = data.pile_limit;
	entry.equip_mask = 0;
	entry.proc_type = data.proc_type;
	if( data.pile_limit > 1){ item::BindPileItem( entry.proc_type ); } // Youshuang add
	entry.price = gmatrix::GetDataMan().get_item_sell_price(data.type);
	entry.expire_date = data.expire_date;
	entry.guid.guid1 = data.guid.guid1;
	entry.guid.guid2 = data.guid.guid2;
	entry.LocateBody();
	if(entry.body) 
		entry.equip_mask = entry.body->GetEquipMask();
	else 
		entry.equip_mask = 0;
	entry.SetContent(data.item_content, data.content_length);
	return true;
}

bool MakeItemEntry(item& entry,const GDB::itemdata & data)
{
	item_data idata;
	idata.type = data.id;
	idata.count = data.count;
	idata.proc_type = data.proctype;
	idata.expire_date = data.expire_date;
	idata.pile_limit = gmatrix::GetDataMan().get_item_pile_limit(data.id);
	if(((int)idata.pile_limit) <= 0 || !idata.count) 
	{
		__PRINTF("正常情况下不会出现此错误，除非物品被删除掉了 物品%d的堆叠上限或者物品数目为0\n",data.id);
		return false;
	}
	//永远使用模板里的nosell
	int proc_type = gmatrix::GetDataMan().get_item_proc_type(data.id);
	proc_type &= item::ITEM_PROC_TYPE_NOSELL | item::ITEM_PROC_TYPE_NODROP;
	idata.proc_type &= ~(item::ITEM_PROC_TYPE_NOSELL | item::ITEM_PROC_TYPE_NODROP);
	idata.proc_type |= proc_type;

	if(idata.count > idata.pile_limit) idata.count = idata.pile_limit;
	idata.guid.guid1 = data.guid1;
	idata.guid.guid2 = data.guid2;
	idata.item_content = (char*)data.data;
	idata.content_length = data.size;

	return MakeItemEntry(entry,idata);
}

bool MakeItemEntry(item& entry, const GDB::pocket_item &data)
{
	item_data idata;
	idata.type = data.id;
	idata.count = data.count;
	idata.proc_type = 0;
	idata.expire_date = 0;
	idata.pile_limit = gmatrix::GetDataMan().get_item_pile_limit(data.id);
	if(((int)idata.pile_limit) <= 0 || !idata.count) 
	{
		__PRINTF("正常情况下不会出现此错误，除非物品被删除掉了 物品%d的堆叠上限或者物品数目为0\n",data.id);
		return false;
	}
	//永远使用模板里的nosell
	int proc_type = gmatrix::GetDataMan().get_item_proc_type(data.id);
	proc_type &= item::ITEM_PROC_TYPE_NOSELL | item::ITEM_PROC_TYPE_NODROP;
	idata.proc_type &= ~(item::ITEM_PROC_TYPE_NOSELL | item::ITEM_PROC_TYPE_NODROP);
	idata.proc_type |= proc_type;

	if(idata.count > idata.pile_limit) idata.count = idata.pile_limit;
	idata.guid.guid1 = 0;
	idata.guid.guid2 = 0;
	idata.item_content = NULL; 
	idata.content_length = 0;
	return MakeItemEntry(entry,idata);
}

void ConvertDBItemEntry(GDB::itemdata &data, const item& it, int index)
{
	data.id = it.type;
	data.index = index;
	data.count =it.count;
	data.max_count = it.pile_limit;
	data.guid1 = it.guid.guid1;
	data.guid2 = it.guid.guid2;
	data.proctype = it.proc_type;
	data.expire_date = it.expire_date;
	data.client_size = it.GetClientSize();
	data.data = NULL;
	data.size = 0;
	it.GetItemData(&(data.data), data.size);
}

void ConvertDBItemEntry(GDB::pocket_item &data, const item& it, int index)
{
	data.id = it.type;
	data.count = it.count;
	data.index = index; 
}

void ConvertItemToData(item_data & data,const item & it)
{
	data.type = it.type;
	data.count = it.count;
	data.pile_limit = it.pile_limit;
	data.expire_date = it.expire_date;
	data.proc_type = it.proc_type;
	data.guid.guid1 = it.guid.guid1;
	data.guid.guid2 = it.guid.guid2;
	it.GetItemData((const void **)&(data.item_content), data.content_length);
}

void ConvertItemToDataForClient(item_data_client & data,const item & it)
{
	data.type = it.type;
	data.count = it.count;
	data.pile_limit = it.pile_limit;
	data.expire_date = it.expire_date;
	data.proc_type = it.proc_type;
	data.guid.guid1 = it.guid.guid1;
	data.guid.guid2 = it.guid.guid2;
	//it.GetItemDataForClient((const void **)&(data.item_content), data.content_length);
	it.GetItemDataForClient(data);
}


void 
item::LocateBody()
{
	body = item_manager::GetInstance().GetItemBody(type);
}


bool item::Save(archive & ar)
{
	ar << type;
	if(type == -1) return true;
	ar << count;
	ar << pile_limit;
	ar << equip_mask;
	ar << proc_type;
	ar << price;
	ar << expire_date;
	ar << guid.guid1;
	ar << guid.guid2;
	ar << is_active;
	ar << content.size;
	if(content.size) ar.push_back(content.buf, content.size);

	return true;
}

bool item::Load(archive & ar)
{
	ASSERT(body == NULL);
	ar >> type;
	if(type == -1)
	{
		Clear();
		return true;
	}
	ar >> count;
	ar >> pile_limit;
	ar >> equip_mask;
	ar >> proc_type;
	ar >> price;
	ar >> expire_date;
	ar >> guid.guid1;
	ar >> guid.guid2;
	ar >> is_active;
	size_t size;
	ar >> size;
	if(size)
	{
		SetContent(ar.cur_data(),size);
		ar.shift(size);
	}

	//重定位 body
	LocateBody();
	return true;
}

//判断是否能移入随身包裹
bool item::CanMoveToPocket() const
{
	if(type == -1){
		return true;
	}

	//有属性的物品不能放入随身包裹	
	if(body != NULL || content.size != 0){
		return false;
	}
	//随身包裹的物品不保留这些属性
	if(equip_mask != 0 || proc_type !=0 || expire_date != 0){
		return false;
	}
	return gmatrix::IsPocketItem(type);
}

bool item_data::CanMoveToPocket() const
{
	if(type == -1){
		return true;
	}
	if(proc_type != 0 || expire_date !=0 || content_length !=0){
		return false;
	}
		
	if(item_manager::GetInstance().GetItemBody(type) != NULL)
	{
		return false;
	}

	return gmatrix::IsPocketItem(type);
}

//物品能不能做move操作
bool item::CanMove() const
{
	if(GetItemType() == item_body::ITEM_TYPE_VIPCARD) return false;
	return true;
}

/*
 * 	世界中物品对象的实现
 */

void
gmatter_item_imp::SetData(const item_data & data)
{
	ASSERT(!_data);
	_data = DupeItem(data);
}

void
gmatter_item_imp::AttachData(item_data *data)
{
	ASSERT(!_data);
	ASSERT(!data->content_length || data->item_content == ((char*)data) + sizeof(item_data));
	_data = data;
}

gmatter_item_imp::~gmatter_item_imp()
{
	if(_data) FreeItem(_data);
}


void 
gmatter_item_imp::OnPickup(const XID & who,int team_id,bool is_team)
{
/*
	int drop_id = _drop_user;
	if(team_id) drop_id |= 0x80000000;
	MSG  msg;
	BuildMessage(msg,GM_MSG_PICKUP_ITEM,who,_parent->ID,
			_parent->pos,drop_id,_data,sizeof(item_data) + _data->content_length);
	gmatrix::SendMessage(msg);
	*/

	if(is_team)
	{
		//组队分配时drop ID 肯定是0
		MSG  msg;
		BuildMessage(msg,GM_MSG_PICKUP_TEAM_ITEM,XID(GM_TYPE_PLAYER,team_id) ,_parent->ID,
				_parent->pos,GetWorldTag(), _data,sizeof(item_data) + _data->content_length);
		gmatrix::SendMessage(msg);
	}
	else
	{
		int drop_id = _drop_user;
		if(team_id) drop_id |= 0x80000000;
		MSG  msg;
		BuildMessage(msg,GM_MSG_PICKUP_ITEM,who,_parent->ID,
				_parent->pos,drop_id, _data,sizeof(item_data) + _data->content_length);
		gmatrix::SendMessage(msg);
	}

}

void 
gmatter_item_imp::OnRelease()
{
	if(_drop_user)
	{
		item_manager::UseItemLog(_drop_user, _data->type, _data->guid.guid1, _data->guid.guid2, _data->count);
	}
}


int 
gmatter_item_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_PICKUP:
			if(msg.content_length == sizeof(msg_pickup_t))
			{
				msg_pickup_t * mpt = (msg_pickup_t*)msg.content;
				Pickup<0>(msg.source,msg.param, mpt->team_id, mpt->team_seq,msg.pos, mpt->who,true);
			}
			else
			{
				ASSERT(false);
			}
			return 0;
		case GM_MSG_FORCE_PICKUP:
			if(msg.content_length == sizeof(XID))
			{
				Pickup<0>(msg.source,0, msg.param,0,msg.pos,*(XID*)msg.content,false);
			}
			return 0;
		default:
			return gmatter_item_base_imp::MessageHandler(msg);
	}
}

void DropItemFromData(world *pPlane,const A3DVECTOR &pos, const item_data & data,const XID &owner,int owner_team,int seq,int drop_id)
{	
	//调用这里时， 常常是由加锁的玩家进行调用的，由于物品和玩家不会有另外的多重锁的情况出现，
	//因此不会出现死锁
	gmatter * matter = pPlane->AllocMatter();
	if(matter == NULL) return ;
	matter->SetActive();
	matter->pos = pos;
	matter->ID.type = GM_TYPE_MATTER;
	matter->ID.id= MERGE_ID<gmatter>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetMatterIndex(matter)));
	matter->SetDirUp(0,0,abase::Rand(0,255));
	gmatter_item_imp *imp = new gmatter_item_imp();
	if(data.expire_date)
	{
		//存在过期时间
		int t = data.expire_date - g_timer.get_systime();
		if(t <= 0) t = 1;
		if(t < imp->GetLife())
		{
			imp->SetLife(t);
		}
	}
	imp->SetOwner(owner,owner_team,seq);
	imp->SetData(data);
	imp->SetDrop(drop_id);
	imp->Init(pPlane,matter);
	matter->imp = imp;
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_item_controller();
	imp->_commander->Init(imp);
	
	pPlane->InsertMatter(matter);
	imp->_runner->enter_world();
	matter->Unlock();
}

void DropItemData(world * pPlane,const A3DVECTOR &pos, item_data *data,const XID &owner,int owner_team,int seq, int name_id, char battle_faction)
{
	//调用这里时， 常常是由加锁的玩家进行调用的，由于物品和玩家不会有另外的多重锁的情况出现，
	//因此不会出现死锁
	gmatter * matter = pPlane->AllocMatter();
	if(matter == NULL) 
	{
		//无法产生，释放该对象
		FreeItem(data);
		return ;
	}
	matter->SetActive();
	matter->pos = pos;
	matter->ID.type = GM_TYPE_MATTER;
	matter->ID.id= MERGE_ID<gmatter>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetMatterIndex(matter)));
	matter->SetDirUp(0,0,abase::Rand(0,255));
	matter->name_id = name_id;
	matter->battle_flag = battle_faction;
	if(battle_faction != 0)
	{
		matter->matter_state |= gmatter::STATE_MASK_BATTLE_FLAG;
	}	
	
	gmatter_item_imp *imp = new gmatter_item_imp();
	if(data->expire_date)
	{
		//存在过期时间
		int t = data->expire_date - g_timer.get_systime();
		if(t <= 0) t = 1;
		if(t < imp->GetLife())
		{
			imp->SetLife(t);
		}
	}
	imp->SetOwner(owner,owner_team,seq);
	imp->AttachData(data);
	imp->Init(pPlane,matter);
	matter->imp = imp;
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_item_controller();
	imp->_commander->Init(imp);
	
	pPlane->InsertMatter(matter);
	imp->_runner->enter_world();
	matter->Unlock();
}

