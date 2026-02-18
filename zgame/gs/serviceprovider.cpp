#include "serviceprovider.h"
#include "world.h"
#include "player_imp.h"
#include "item.h"
#include <common/protocol_imp.h>
#include "usermsg.h"
#include "clstab.h"
#include "item.h"
#include "template/itemdataman.h"
#include "skillwrapper.h"
#include "npcgenerator.h"
#include "actsession.h"
#include "task/taskman.h"
#include <factionlib.h>
#include <mailsyslib.h>
#include <auctionsyslib.h>
#include "playerstall.h"
#include "servicenpc.h"
#include "cooldowncfg.h"
#include "sfilterdef.h"
#include <sellpointlib.h>
#include <stocklib.h>
#include "item/item_petbedge.h"
#include "template_loader.h"

namespace NG_ELEMNET_SERVICE
{

class general_provider: public service_provider
{
protected:	
	inline void SendServiceContent(int id, int cs_index, int sid, const void *buf, size_t size)
	{
		packet_wrapper  h1(size + 32);
		using namespace S2C;
		gobject * pObj = _imp->_parent;
		CMD::Make<CMD::npc_service_content>::From(h1,pObj->ID, _type, buf,size);
		send_ls_msg(cs_index,id,sid,h1);
	}

	inline void SendMessage(int message, const XID & target,int param, const void * buf, size_t size)
	{
		MSG msg;
		BuildMessage(msg,message,target,_imp->_parent->ID,_imp->_parent->pos,
				param,buf,size);
		gmatrix::SendMessage(msg);
	}
	
};

class feedback_provider : public general_provider
{
	virtual feedback_provider * Clone()
	{
		ASSERT(!_is_init);
		return new feedback_provider(*this);
	}

	virtual bool Save(archive & ar)
	{
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		return true;
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		ASSERT(size == 0);
		return true;
	}
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		//将数据发回,表示有这个服务
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
	}
};

class vendor_provider : public general_provider
{
	float _discount_rate;	//折扣 0.0f - 1.0f
//	abase::vector<int> 		_left_list;	//物品剩余数量的列表
//	abase::vector<int> 		_amount_list;	//物品数量的列表
	abase::vector<const item_data *, abase::fast_alloc<> >   	_item_list;	//物品的数据列表
	abase::vector<int , abase::fast_alloc<> >   	_price_list;			//物品的价格列表

	virtual bool Save(archive & ar)
	{
		ar << _discount_rate;
		ar << (int)_item_list.size();
		for(size_t i =0; i < _item_list.size(); i ++)
		{
			if(_item_list[i])
			{
				ar << 0;
			}
			else
			{
				ar << _item_list[i]->type;
			}
		}
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		ar >> _discount_rate;
		size_t size;
		ar >> size;
		if(size > 1024) 
		{
			ASSERT(false);
			size = 1024;
		}
		abase::vector<int, abase::fast_alloc<> > tmpList;
		tmpList.reserve(size);
		for(size_t i =0; i < size; i ++)
		{
			int item_type;
			ar >>item_type;
			tmpList.push_back(item_type);
		}
		OnInit(tmpList.begin(),tmpList.size()*sizeof(int));
		return true;
	}
public:
	virtual ~vendor_provider()
	{
		_item_list.clear();
	}

	struct bag 
	{
		int item_type;
		size_t index;
		size_t count;
	};

	struct request
	{
		size_t money;
		size_t count;
		bag item_list[];
	};

	vendor_provider():_discount_rate(0.0f)
	{
	}

private:
	
	
	virtual vendor_provider * Clone()
	{
		ASSERT(!_is_init);
		return new vendor_provider(*this);
	}
	
	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size % sizeof(npc_template::npc_statement::goods_t)) 
		{
			ASSERT(false);
			return false;
		}
		npc_template::npc_statement::goods_t* list = (npc_template::npc_statement::goods_t*)buf;
		size_t count = size / sizeof(npc_template::npc_statement::goods_t);

		_item_list.clear();_item_list.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			if(list[i].goods == 0)
			{
				_item_list.push_back(NULL);
				_price_list.push_back(0);
				continue;
			}

			const void *pBuf = gmatrix::GetDataMan().get_item_for_sell(list[i].goods);
			if(pBuf)
			{
				const item_data * data = (const item_data*)pBuf;
				_item_list.push_back(data);	
				int shop_price = gmatrix::GetDataMan().get_item_shop_price(list[i].goods);
				shop_price = (int)(0.5f + shop_price * (1.0f + list[i].discount));
				if(shop_price < (int)data->price)
				{
					__PRINTF("商店卖出价小于卖店价格 %d,折扣(%f)\n",list[i].goods,list[i].discount);
					ASSERT(false);
					shop_price = data->price;
				}
				_price_list.push_back(shop_price);
			}
			else
			{
				__PRINTF("can not init vendor goods %d\n", list[i].goods);
				_item_list.push_back(NULL);
				_price_list.push_back(0);
			}
		}
		return true;
	}
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能需要返回税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		//param 是 索引 buf 代表数量和自己的钱数(初步判断,未必准确)
		request * param = (request *)buf;
		if(size != sizeof(request) + param->count*sizeof(bag)) return;
		
		size_t money_need = 0;
		for(size_t i=0; i < param->count; ++i)
		{
			size_t index = param->item_list[i].index;
			if(index >=_item_list.size()) 
			{
				//客户端传来的错误的数据
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}
			const item_data * pItem = _item_list[index];
			int price = _price_list[index];
			if(!pItem)
			{
				//没有找到正确的数据
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}
			ASSERT(pItem->item_content == ((const char * )pItem) + sizeof(item_data));
			size_t count = param->item_list[i].count; 
			if(!count || count > pItem->pile_limit || param->item_list[i].item_type != pItem->type)
			{
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}
			float p = (float)money_need + (float)(price) * (float)count * (1.0f - _discount_rate);
			if(p > MONEY_CAPACITY_BASE)
			{
				//钱数太多
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
				return ;
			}
			money_need += (int)(price * count * (1.0f - _discount_rate) + 0.5f);
			if(money_need > 0x7FFFFFFF)
			{
				//没有找到正确的数据
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
				return ;
			}
		}

		if(money_need > param->money)
		{
			//钱数不够，返回错误信息
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
		}

		param->money = money_need;

		//发出物品数据和要求的钱数
		//发出回馈数据
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}

};

class vendor_executor : public service_executor
{
public:
	typedef vendor_provider::request player_request;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		player_request * req= (player_request*)buf;
		if(req->count <= 0 || 
			size != sizeof(player_request)+req->count*sizeof(vendor_provider::bag )) return false;
		size_t money = pImp->GetMoney();
		if(money == 0) return false;
		req->money = pImp->GetMoney();

		if(!pImp->InventoryHasSlot(req->count) || req->money == 0) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		player_request * req= (player_request*)buf;
		if(req->count <= 0 || 
				size != sizeof(player_request)+req->count*sizeof(vendor_provider::bag ))
		{
			ASSERT(false);
			return false;
		}
		
		if(!pImp->InventoryHasSlot(req->count)) 
		{
			pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}
		
		//检查钱数
		if(pImp->GetMoney() < req->money)
		{
			//钱数不够
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}

		//给player添加物品
		abase::vector<abase::pair<const item_data*,int> ,abase::fast_alloc<> > list;
		list.reserve(req->count);
		for(size_t i = 0; i < req->count; i ++)
		{
			int item_id = req->item_list[i].item_type;
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
			ASSERT(pItem);
			if(pItem) list.push_back(abase::pair<const item_data*,int>(pItem,req->item_list[i].count));
		}
		pImp->PurchaseItem(list.begin(),list.size(), req->money);
		return true;
	}
};

typedef feedback_provider purchase_provider;

class purchase_executor : public service_executor
{
	enum{ SELL, REPURCHASE };
public:
#pragma pack(1)
	struct  player_request
	{
		char   op_type;     //操作类型：0表示卖物品，1表示回购物品
		size_t item_count;
		struct item_data
		{
			int type;		//物品类型
			size_t inv_index; 	//在包裹栏里的号码
			size_t count;		//数量
		} list[1];
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		player_request * req = (player_request*)buf;

		switch (req->op_type)
		{
			case SELL:
				{
					if(req->item_count > pImp->GetInventory().Size() || size != sizeof(player_request) + (req->item_count-1)*sizeof(player_request::item_data) ) return false;

					for(size_t i = 0; i < req->item_count; i ++)
					{
						pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,req->list[i].inv_index);
					}

					if(pImp->OI_TestSafeLock())
					{
						pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
						return true;
					}

					float incmoney = 0.0f;
					for(size_t i = 0; i < req->item_count; ++i)
					{	
						incmoney += pImp->CalcIncMoney(req->list[i].inv_index,req->list[i].type,req->list[i].count);
					}

					if(incmoney + pImp->GetMoney() + 0.5f > MONEY_CAPACITY_BASE)
					{
						return true;
					}	     

					/*
					//校验该物品是否存在
					if(!pImp->IsItemCanSell(req->inv_index, req->type,req->count))
					{
					return false;
					}
					由于是批量卖出操作，不再进行物品是否存在的检查
					*/
				}
				break;

			case REPURCHASE:
				{
					if(size != sizeof(player_request) + (req->item_count-1)*sizeof(player_request::item_data)) return false;

					if(pImp->OI_TestSafeLock())
					{
						pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
						return true;
					}

					if(pImp->GetInventory().GetEmptySlotCount() < req->item_count)
					{
						pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
					}
				}
				break;

			default:
				return true;
		}

		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size < sizeof(player_request))
		{
			//发出的时候就应该正确的
			ASSERT(false);
			return false;
		}
		player_request * req = (player_request*)buf;
		ASSERT(req->item_count  <= 0x7FFF&&size==sizeof(player_request)+(req->item_count-1)*sizeof(player_request::item_data));

		switch (req->op_type)
		{
			case SELL:
				{
					for(size_t i = 0; i < req->item_count;i ++)
					{
						if(!pImp->ItemToMoney(req->list[i].inv_index,req->list[i].type,req->list[i].count))
						{
							return true;
						}
					}
				}
				break;

			case REPURCHASE:
				{

					for(size_t i = 0; i < req->item_count;i ++)
					{
						if(!pImp->RepurchaseItem(req->list[i].inv_index,req->list[i].type,req->list[i].count))
						{
							//删除已经被回购的物品，然后发给客户端
							pImp->UpdateRepurchaseInv();
							pImp->SendRepurchaseInvData();
							return true;
						}
					}

					//删除已经被回购的物品，然后发给客户端
					pImp->UpdateRepurchaseInv();
					pImp->SendRepurchaseInvData();
				}
				break;

			default:
				return true;
		}

		return true;
	}
};

typedef feedback_provider heal_provider;
class heal_executor : public service_executor
{
public:
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != 0) return false;
		if(!pImp->IsBled())
		{
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(!pImp->IsCombatState()) 
			pImp->Renew();
		else
			pImp->_runner->error_message(S2C::ERR_CANNOT_HEAL_IN_COMBAT);
		return true;
	}
};

class general_id_provider : public general_provider
{
protected:
	abase::vector<int, abase::fast_alloc<> > _list;

	virtual bool Save(archive & ar)
	{
		size_t size = _list.size();
		ar << (int)size;
		ar.push_back(_list.begin(),sizeof(int)*size);
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		size_t size;
		ar >> size;
		for(size_t i = 0; i < size; i ++)
		{
			int entry;
			ar.pop_back(&entry,sizeof(entry));
			_list.push_back(entry);
		}
		return true;
	}
protected:
	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size % sizeof(int) || !size) 
		{
			ASSERT(false);
			return false;
		}
		int * list = (int*)buf;
		size_t count = size / sizeof(int);
		_list.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			_list.push_back(list[i]);
		}
		std::sort(_list.begin(),_list.end());
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(int))
		{
			ASSERT(false);
			return ;
		}
		int id = *(int*)buf;
		if(std::binary_search(_list.begin(),_list.end(),id))
		{
			//找到回应
			SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
		}
		else
		{
			//未找到，发送错误
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_TASK_NOT_AVAILABLE,NULL,0);
		}
	}
	
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能需要返回任务列表
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
	}

	general_id_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new general_id_provider(*this);
	}
};

class task_provider : public general_id_provider
{

public:
	struct request
	{
		//int npc_id;
		int task_id;
	};

private:
	task_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new task_provider(*this);
	}
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能需要返回任务列表
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		int id = req->task_id;
		//if(req->npc_id==((gnpc*)(_imp->_parent))->tid && std::binary_search(_list.begin(),_list.end(),id))
		if(std::binary_search(_list.begin(),_list.end(),id))
		{
			//找到回应
			SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
		}
		else
		{
			//未找到，发送错误
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_TASK_NOT_AVAILABLE,NULL,0);
		}
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
	}
};

class task_executor : public service_executor
{

public:
	typedef task_provider::request player_request;
	task_executor(){}
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}
	
};

class task_in_provider : public task_provider
{
public:
	struct request
	{
		int task_id;
		int choice;
	};
private:
	task_in_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new task_in_provider(*this);
	}
	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		int id = req->task_id;
		//if(req->npc_id==((gnpc*)(_imp->_parent))->tid && std::binary_search(_list.begin(),_list.end(),id))
		if(std::binary_search(_list.begin(),_list.end(),id))
		{
			//找到回应
			SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
		}
		else
		{
			//未找到，发送错误
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_TASK_NOT_AVAILABLE,NULL,0);
		}
	}

};

class task_in_executor : public service_executor
{

	typedef task_in_provider::request player_request;
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		PlayerTaskInterface  task_if(pImp);
		player_request * req = (player_request*)buf;
		OnTaskCheckAward(&task_if,req->task_id,req->choice);
		return true;
	}
};

class task_out_provider : public task_provider
{
public:
	struct request
	{
		int task_id;
		int task_set_id;
	};
	int _task_set_id;
private:
	task_out_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new task_out_provider(*this);
	}
	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size % sizeof(int) || !size) 
		{
			ASSERT(false);
			return false;
		}

		_task_set_id = *(int*)buf;
		return general_id_provider::OnInit( ((const char *)buf) + sizeof(int),size - sizeof(int));
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		int id = req->task_id;
		if((_task_set_id == 0 || _task_set_id == req->task_set_id) 
				&& std::binary_search(_list.begin(),_list.end(),id))
		{
			//找到回应
			SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
		}
		else
		{
			//未找到，发送错误
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_TASK_NOT_AVAILABLE,NULL,0);
		}
	}

};

class task_out_executor : public task_executor
{
	typedef task_out_provider::request player_request;

private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		player_request * req = (player_request*) buf;
/*		int taskid = req->task_id;
		if(taskid > 0)
		{
			int disable_task_list[] = {13048, 13049, 13051, 13052, 13054, 13031, 13032, 13033, 13035, 13036, 13042, 13043, 13044, 13045, 13046, 13026, 13027, 13028, 13029, 13030, 13037, 13038, 13039, 13040, 13041, 13080, 13081, 13082, 13083, 13084};
			for(int i =0; i < sizeof(disable_task_list)/sizeof(int); i++)
			{
				if(disable_task_list[i] == taskid) return false;
			}
		}*/
		
		
		PlayerTaskInterface  task_if(pImp);
		if(OnTaskCheckDeliver(&task_if,req->task_id, req->task_set_id))
		{
			//接到任务了
			__PRINTF("接到任务了...........\n");
			//pImp->DeliverTopicSite( new TOPIC_SITE::task_out( req->task_id ) );  // Youshuang add
		}
		else
		{
			pImp->_runner->error_message(S2C::ERR_TASK_NOT_AVAILABLE);
		}
		return true;
	}
};

class task_matter_provider : public task_provider
{
public:
	struct feed_back
	{
		int task_id;
		int npc_tid;
	};
private:
	task_matter_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new task_matter_provider(*this);
	}
	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		int id = req->task_id;
		//if(req->npc_id==((gnpc*)(_imp->_parent))->tid && std::binary_search(_list.begin(),_list.end(),id))
		if(std::binary_search(_list.begin(),_list.end(),id))
		{
			//找到回应
			gnpc * pNPC = (gnpc*)_imp->_parent;
			feed_back fb = { id, pNPC->tid };
			SendMessage(GM_MSG_SERVICE_DATA,player,_type,&fb,sizeof(fb));
		}
		else
		{
			//未找到，发送错误
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_TASK_NOT_AVAILABLE,NULL,0);
		}
	}

};

class task_matter_executor : public task_executor
{

private:
	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(task_matter_provider::feed_back));
		task_matter_provider::feed_back & fb = *(task_matter_provider::feed_back*)buf;
		PlayerTaskInterface  task_if(pImp);
		OnNPCDeliverTaskItem(&task_if,fb.npc_tid,fb.task_id);
		return true;
	}
};

typedef feedback_provider install_provider;
class install_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//物品 id
		int stone_index;		//各种石头的索引
		int rt_index;			//保留符道具的索引 如果没有填入-1
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_INSTALL;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->RefineItemAddon(_req.item_index, _req.item_id, _req.stone_index, _req.rt_index);
				if(!bRst) 
				{
					//精炼出错，要报告一个错误
					pImp->_runner->error_message(S2C::ERR_CAN_NOT_REFINE);
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider uninstall_provider;
class uninstall_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		size_t item_index;
		int item_id;
		bool uninstall_pstone;		//删除属性石还是技能石
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_UNINSTALL;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->RemoveRefineAddon(_req.item_index, _req.item_id, _req.uninstall_pstone);
				if(!bRst) 
				{
					//精炼出错，要报告一个错误
					pImp->_runner->error_message(S2C::ERR_CAN_NOT_UNINSTALL);
				}
			}
			
		};
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider produce_provider;
class produce_executor : public service_executor
{

public:
	typedef struct 
	{
		int id;
	} player_request;
	produce_executor() {}
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request * req = (player_request*)buf;
		
		//校验物品配方是否存在
		const recipe_template *rt = recipe_manager::GetRecipe(req->id);
		if(!rt) return false;

		//校验配方技能和级别是否匹配
		if(rt->level > pImp->GetProduceLevel()) return false;

		//检查金钱是否足够
		if(pImp->GetMoney() < rt->fee) 
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return true;	//只报告金钱不够的错误
		}

		//发出功能请求
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf, size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			//发出的时候就应该正确的
			ASSERT(false);
			return false;
		}
		player_request * req = (player_request*)buf;

		//	加入制造的session
		const recipe_template *rt = recipe_manager::GetRecipe(req->id);
		if(!rt || pImp->GetProduceLevel() < rt->level) 
		{
			__PRINTF("配方%d不存在在分解服务\n",req->id);
			return false;
		}
		if(rt->cool_type > 0 && rt->cool_type < 200)	//多留200余量
		{
			if(!pImp->CheckCoolDown(COOLDOWN_INDEX_PRODUCE_BEGIN + rt->cool_type ))
			{
				pImp->_runner->error_message(S2C::ERR_PRODUCE_IN_COOLDOWN);
				return false;
			}
		}

		pImp->AddStartSession(new session_produce(pImp,rt,1));
		return true;
	}
	
};

class reset_pkvalue_provider : public general_provider
{
public:
	struct request
	{
		int reset_value;
		size_t money;
	};
private:
	int _fee_per_unit;
	virtual reset_pkvalue_provider * Clone()
	{
		ASSERT(!_is_init);
		return new reset_pkvalue_provider(*this);
	}

	virtual bool Save(archive & ar)
	{
		ar << _fee_per_unit;
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		ar >> _fee_per_unit;
		return true;
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		ASSERT(size == sizeof(int));
		_fee_per_unit = *(int*)buf;
		return true;
	}
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		//将数据发回,表示有这个服务
		ASSERT(size == sizeof(request));
		request * req = (request*)buf;
		size_t need_money = (size_t)req->reset_value * _fee_per_unit;
		if(req->money < need_money)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
			return;
		}
		request reply = *req;
		reply.money = need_money;
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,&reply, sizeof(reply));
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
	}
};
class reset_pkvalue_executor : public service_executor
{
public:
	struct  player_request
	{
		int reset_value;		//只能是1 或者 10
	};
	typedef reset_pkvalue_provider::request request;
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request * req = (player_request*) buf;
		if(req->reset_value != 1 && req->reset_value != 10) return false;
		if(!pImp->GetPKValue()) return false;

		request ireq = {req->reset_value, pImp->GetMoney()};
		//不考虑钱数
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&ireq, sizeof(ireq));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(request));
		request *req = (request*)buf;
		if(req->money > pImp->GetMoney())
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return true;
		}
		if(pImp->GetPKValue() == 0)
		{
			return true;
		}
		pImp->ResetPKValue(req->reset_value, req->money);
		return true;
	}
};

typedef feedback_provider trashbox_passwd_provider;
class trashbox_passwd_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		unsigned short origin_size;
		unsigned short new_size;
		char origin_passwd[1];
		char new_passwd[1];
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(unsigned short) * 2) return false;
		player_request * req = (player_request*)buf;
		if(size != sizeof(unsigned short) * 2 + req->origin_size + req->new_size) return false;

		//检查原始密码的正确与否

		const char * origin_passwd = req->origin_passwd;
		const char * new_passwd = req->origin_passwd + req->origin_size;

		if(!pImp->_trashbox.IsPasswordValid(new_passwd,req->new_size))
		{
			pImp->_runner->error_message(S2C::ERR_INVALID_PASSWD_FORMAT);
			return true;
		}

		if(!pImp->_trashbox.CheckPassword(origin_passwd,req->origin_size))
		{
			pImp->_runner->error_message(S2C::ERR_PASSWD_NOT_MATCH);
			return true;
		}

		//不考虑钱数
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,new_passwd,req->new_size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		GLog::log(GLOG_INFO,"用户%d修改了仓库密码，新密码长度%d",pImp->_parent->ID.id,size);
		pImp->_trashbox.SetPassword((const char *)buf,size);
		pImp->_runner->trashbox_passwd_changed(pImp->_trashbox.HasPassword());
		return true;
	}
};

typedef feedback_provider trashbox_open_provider;
class trashbox_open_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		unsigned int passwd_size;
		char passwd[];
	};
#pragma pack()
protected:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(unsigned int)) return false;
		player_request * req = (player_request*)buf;
		if(size != req->passwd_size + sizeof(unsigned int)) return false;

		if(pImp->HasSession())
		{
			pImp->_runner->error_message(S2C::ERR_OTHER_SESSION_IN_EXECUTE);
			return true;
		}

		//检查是否已经打开
		if(pImp->IsTrashBoxOpen())
		{
			pImp->_runner->error_message(S2C::ERR_OTHER_SESSION_IN_EXECUTE);
			return true;
		}

		//考虑添加密码重试计数策略  $$$$$$$$
		//检查密码
		const char * passwd = req->passwd;
		if(!pImp->_trashbox.CheckPassword(passwd,req->passwd_size))
		{
			pImp->_runner->error_message(S2C::ERR_PASSWD_NOT_MATCH);
			return true;
		}

		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,0,0);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		//开启钱箱
		//加入当前的
		if(!pImp->HasSession())
		{	
			__PRINTF("芝麻开门啦、\n");
			session_use_trashbox *pSession = new session_use_trashbox(pImp, player_trashbox::TRASHBOX_OPEN);
			//session_use_trashbox *pSession = new session_use_trashbox(pImp, player_trashbox::MAFIA_TB_OPEN);
			pImp->AddStartSession(pSession);
		}
		return true;
	}
};

class mafia_trashbox_open_executor : public trashbox_open_executor
{

public:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		//必须是帮派成员 
		if(!pImp->OI_IsMafiaMember())
		{
			pImp->_runner->error_message(S2C::ERR_NOT_MAFIA_MEMBER);
			pImp->_runner->npc_service_result(-1);
			return true;
		}
	
		//帮派需要加入足够的时间
		//if(!pImp->HasEnoughMafiaTime(3600*14))
		if(!pImp->HasEnoughMafiaTime(14))
		{
			pImp->_runner->error_message(S2C::ERR_MAFIA_TIME_NOT_ENOUGH);
			pImp->_runner->npc_service_result(-1);
			return true;
		}
		return trashbox_open_executor::SendRequest(pImp,provider, buf, size);
	}
	
	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		//开启钱箱
		//加入当前的
		if(!pImp->HasSession())
		{	
			__PRINTF("芝麻开2号门啦、\n");
			session_use_trashbox *pSession = new session_use_trashbox(pImp, player_trashbox::MAFIA_TB_OPEN);
			pImp->AddStartSession(pSession);
		}
		return true;
	}
};

class plane_switch_provider : public general_provider
{
public:
	typedef npc_template::npc_statement::__st_ent transmit_entry;

private:
	abase::vector<transmit_entry,abase::fast_alloc<> > _target_list;	//传送的目标列表
	
	virtual bool Save(archive & ar)
	{
		size_t size = _target_list.size();
		ar << (int)size;
		ar.push_back(_target_list.begin(),sizeof(transmit_entry)*size);
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		size_t size;
		ar >> size;
		for(size_t i = 0; i < size; i ++)
		{
			transmit_entry  entry;
			ar.pop_back(&entry,sizeof(entry));
			_target_list.push_back(entry);
		}
		return true;
	}
public:
	struct request
	{
		size_t index;	//目标点索引
		size_t money;	//player当前的钱数
		int    level;	//玩家的级别
	};

	plane_switch_provider()
	{}
private:
	
	
	virtual plane_switch_provider * Clone()
	{
		ASSERT(!_is_init);
		return new plane_switch_provider(*this);
	}
	
	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size % sizeof(transmit_entry) || size == 0) 
		{
			ASSERT(false);
			return false;
		}
		transmit_entry * list = (transmit_entry *)buf;
		size_t count = size / sizeof(transmit_entry);

		_target_list.clear();_target_list.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			//测试结束
			_target_list.push_back(list[i]);
		}
		return true;
	}

	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能要返回位置列表或者税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request) ) return;
		request * param = (request *)buf;
		size_t index = param->index;
		if(index >=_target_list.size()) 
		{
			//客户端传来的错误的数据
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
			return ;
		}
		size_t money = param->money;
		if(money < _target_list[index].fee) 
		{
			//通知该玩家钱不够
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
			return ;
		}

		if(param->level < _target_list[index].require_level)
		{
			//通知该玩家级别不够
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_LEVEL_NOT_MATCH,NULL,0);
			return;
		}
		//发出数据
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,&(_target_list[index]),sizeof(transmit_entry));
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}
};

//此服务并未真正启用 但后面的注释很有意义， 
class plane_switch_executor : public service_executor
{
public:
	struct  player_request
	{
		size_t index;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		player_request * req= (player_request*)buf;
		plane_switch_provider::request data;
		data.index = req->index;
		data.money = pImp->GetMoney();
		data.level = pImp->_basic.level;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&data,sizeof(data));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(plane_switch_provider::transmit_entry));
		plane_switch_provider::transmit_entry * entry = (plane_switch_provider::transmit_entry*) buf;
		
		//检查钱数
		size_t fee = entry->fee;
		if(pImp->GetMoney() < fee)
		{
			//钱数不够
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}

		pImp->SpendMoney(fee);
		if (fee) GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=7:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),fee,entry->target_tag);
		pImp->_runner->spend_money(fee);
		pImp->LongJump(entry->target_pos,entry->target_tag);
		return true;
	}
};

typedef feedback_provider faction_service_provider;

class faction_service_executor : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		
		//检查是否可以开始
		if(gmatrix::GetWorldParam().forbid_faction)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
		if(gmatrix::GetWorldParam().forbid_faction)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		player_request * req = (player_request*)buf;
		//发送请求数据
		if(!GNET::ForwardFactionOP(req->service_id,pImp->_parent->ID.id,req->buf, size - sizeof(player_request),object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户%d执行了帮派操作",pImp->_parent->ID.id);
		return true;
	}
};

//这个provider并没有用处，只是空实现
class player_market_provider :public general_provider
{

	virtual bool Save(archive & ar)
	{
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		return true;
	}
private:
	player_market_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new player_market_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		return true;
	}
	
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能需要返回任务列表
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_UNAVILABLE,NULL,0);
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
	}
};

class player_market_executor : public service_executor
{
public:
	typedef player_stall::trade_request player_request;
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*)buf;
		if(req->count == 0 || req->count > PLAYER_MARKET_SLOT_CAP) return false;
		if(req->count * sizeof(player_request::entry_t) + sizeof(player_request) != size) return false;
		req->money = pImp->GetMoney();
		if(pImp->GetInventory().GetEmptySlotCount() < req->count)
		{
			pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return true;
		}
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(false);
		//执行不是如此进行地
		return true;
	}
};


class player_market_executor2 : public service_executor
{
public:
	typedef player_stall::trade_request player_request;
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}       
		player_request * req = (player_request*)buf;
		if(req->count > PLAYER_MARKET_SLOT_CAP) return false;
		if(req->count * sizeof(player_request::entry_t) + sizeof(player_request) != size) return false;

		for(size_t i = 0; i < req->count; i ++)
		{
			pImp->_runner->unlock_inventory_slot(gplayer_imp::IL_INVENTORY,req->list[i].inv_index);
		}

		//检查是否有这些物品
		req->money = pImp->GetMoney();

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(false);
		//执行不是如此进行地
		return true;
	}
};

typedef feedback_provider mail_service_provider;

class mail_service_executor : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		
		//检查是否可以开始
		if(gmatrix::GetWorldParam().forbid_mail)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
		if(gmatrix::GetWorldParam().forbid_mail)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		player_request * req = (player_request*)buf;
		//发送请求数据
		int rst = GNET::ForwardMailSysOP(req->service_id,req->buf, size - sizeof(player_request),object_interface(pImp));
		if(rst == 2)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return false;
		}
		else if(rst)
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户%d执行了邮件操作",pImp->_parent->ID.id);
		return true;
	}
};

typedef feedback_provider double_exp_provider;
class double_exp_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		size_t bonus_index;
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		/*
		static int bonus_time[] = {3600,3600*2,3600*3,3600*4};
		if(size != sizeof(player_request)) return false;
		player_request * req = (player_request*)buf;
		if(req->bonus_index >= 4) return false;
		if(!pImp->TestRestTime(bonus_time[req->bonus_index]))
		{
			//没有足够的双倍时间
			pImp->_runner->error_message(S2C::ERR_NOT_ENOUGH_REST_TIME);
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		*/
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		/*
		static int bonus_time[] = {3600,3600*2,3600*3,3600*4};
		if(size != sizeof(player_request))
		{
			//发出的时候就应该正确的
			ASSERT(false);
			return false;
		}

		player_request * req = (player_request*)buf;
		if(!pImp->TestRestTime(bonus_time[req->bonus_index]))
		{
			//没有足够的双倍时间
			pImp->_runner->error_message(S2C::ERR_NOT_ENOUGH_REST_TIME);
			return false;
		}
		int t = bonus_time[req->bonus_index];

		//完成操作，增加双倍时间，增加双倍时间消耗
		pImp->ActiveMultiExpTime(2, t);
		*/
		return true;
	}
};

typedef feedback_provider auction_service_provider;

class auction_service_executor : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		
		//检查是否可以开始
		if(gmatrix::GetWorldParam().forbid_auction)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
		if(gmatrix::GetWorldParam().forbid_auction)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		player_request * req = (player_request*)buf;
		//发送请求数据
		if(!GNET::ForwardAuctionSysOP(req->service_id,req->buf, size - sizeof(player_request),object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户%d执行了拍卖操作",pImp->_parent->ID.id);
		return true;
	}
};


typedef feedback_provider battle_service_provider;

class battle_service_executor : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		// 这里关于战场
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
//		player_request * req = (player_request*)buf;
		//发送请求数据 
//		if(!GNET::ForwardBattleOP(req->service_id,req->buf, size - sizeof(player_request),object_interface(pImp)))
//		{
//			//发送错误
//			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
//			return false;
//		}
//		GLog::log(GLOG_INFO,"用户%d执行了战场操作",pImp->_parent->ID.id);
//		return true;
		return false;
	}
};

/*
class towerbuild_provider : public general_provider
{
public:
	struct request
	{
		size_t index;
		size_t money;
		int item_id;
	};

	typedef npc_template::npc_statement::__npc_tower_build  entry_t;
	entry_t _build_list[4];
	int _time_counter;

private:
	virtual bool Save(archive & ar)
	{
		ar.push_back(_build_list,sizeof(_build_list));
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		ar.pop_back(_build_list,sizeof(_build_list));
		return true;
	}
public:

	towerbuild_provider()
	{
		_time_counter = 0;
	}

private:
	virtual towerbuild_provider * Clone()
	{
		ASSERT(!_is_init);
		return new towerbuild_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(_build_list)) 
		{
			ASSERT(false);
			return false;
		}
		memcpy(_build_list, buf, size);
		return true;
	}
	
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能要返回位置列表或者税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		size_t index = req->index;
		if(index >= 4)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_UNAVILABLE,NULL,0);
			return;
		}

		if(!_build_list[index].id_in_build || !_build_list[index].id_buildup)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_UNAVILABLE,NULL,0);
			return;
		}

		if(_time_counter)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_NPC_SERVICE_IS_BUSY,NULL,0);
			return;
		}
		
		if(req->money < (size_t)_build_list[index].fee || req->item_id != _build_list[index].id_object_need)
		{
			//发送金钱不够
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
			return ;
		}

		_time_counter = 2;
		
		//发回回应数据
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,_build_list + index,sizeof(_build_list[index]));
	}

	virtual void OnHeartbeat()
	{
		if(_time_counter)
		{
			_time_counter--;
			if(_time_counter <=0)
			{
				_time_counter = 0;
			}
		}
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}
};

class towerbuild_executor : public service_executor
{
public:
	struct player_request
	{
		size_t index;		//服务索引
		size_t item_id;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		player_request *preq = (player_request*)buf;
		
		//检查物品是否存在
		if(preq->item_id != 0 && !pImp->IsItemExist(preq->item_id))
		{
			return false;
		}
		
		towerbuild_provider::request req = {preq->index,pImp->GetMoney(),preq->item_id};

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&req,sizeof(req));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(towerbuild_provider::entry_t));
		towerbuild_provider::entry_t * pEnt = (towerbuild_provider::entry_t *)buf;
		if(pImp->GetMoney() < (size_t)pEnt->fee)
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}

		item_list & inv = pImp->GetInventory();
		int item_index = -1;
		if(pEnt->id_object_need && (item_index = inv.Find(0,pEnt->id_object_need)) < 0)
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}
		
		//可以进行服务，减少金钱和物品
		if(pEnt->fee)
		{
			pImp->SpendMoney(pEnt->fee);
			pImp->_runner->spend_money(pEnt->fee);
		}

		if(item_index >= 0)
		{
			pImp->UseItemLog(inv, item_index,1);
			inv.DecAmount(item_index,1);
			pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,item_index, pEnt->id_object_need,1);
		}

		//发送一个激活消息通知对方
		int flag = msg_npc_transform::FLAG_DOUBLE_DMG_IN_BUILD;
		msg_npc_transform data = {pEnt->id_in_build, pEnt->time_use , flag, pEnt->id_buildup};
		pImp->SendTo<0>(GM_MSG_NPC_TRANSFORM,provider,0,&data,sizeof(data));
		return true;
	}
};

typedef feedback_provider battle_leave_provider;
class battle_leave_executor : public service_executor
{
public:
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != 0) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		pImp->_filters.ModifyFilter(FILTER_CHECK_INSTANCE_KEY,FMID_CLEAR_AEBF,NULL,0);
		return true;
	}
};
*/

typedef feedback_provider spec_trade_provider;

class spec_trade_executor : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;

		if(gmatrix::GetWorldParam().forbid_cash_trade)
		{
			return false;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}       
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
		player_request * req = (player_request*)buf;
		//发送请求数据 
		if(!GNET::ForwardSellPointSysOP(req->service_id,req->buf, size - sizeof(player_request),object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户%d执行了点卡操作",pImp->_parent->ID.id);
		return true;
	}
};

class bind_item_provider : public general_provider
{
public:
	struct request
	{
		int money_need;
		int item_need;

		int item_index;
		int item_id;
	};

private:
	virtual bool Save(archive & ar)
	{
		//未做
		ASSERT(false);
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		//未做
		ASSERT(false);
		return true;
	}
	int _money_need;
	int _item_need;
public:

	bind_item_provider()
	{
	}

private:
	virtual bind_item_provider * Clone()
	{
		ASSERT(!_is_init);
		return new bind_item_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(int) + sizeof(int))
		{
			ASSERT(false);
			return false;
		}
		_money_need = ((int *)buf)[0];
		_item_need = ((int*)buf)[1];
		return true;
	}
	
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能要返回位置列表或者税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		if(req->money_need <_money_need)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_OUT_OF_FUND,NULL,0);
			return;
		}

		request reply = *req;
		reply.money_need = _money_need;
		reply.item_need = _item_need;

		//发回回应数据
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,&reply,sizeof(reply));
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}
};

class bind_item_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};
	typedef bind_item_provider::request request;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		return false;	//此服务永远不会出现了，已经被lock_item_provider代替了
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		//检查物品是否能够进行绑定
		if(!pImp->CheckItemBindCondition(preq->item_index, preq->item_id))
		{
			return false;
		}

		request req;
		req.item_need = 0;
		req.money_need = pImp->GetMoney();
		req.item_index = preq->item_index;
		req.item_id = preq->item_id;
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&req, sizeof(req));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(request));
		class op : public session_general_operation::operation
		{
			request _req; 
		public:
			op(const request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_BIND;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;

				if(_req.money_need > 0)
				{
					if(pImp->GetMoney() < (size_t)_req.money_need)
					{
						pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
						return;
					}
				}

				if(!pImp->CheckItemBindCondition(_req.item_index, _req.item_id))
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}
				
				item_list & inv = pImp->GetInventory();
				int item_index = -1;
				if(_req.item_need && (item_index = inv.Find(0,_req.item_need)) < 0)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				//进行绑定操作
				if(!pImp->BindItem(_req.item_index, _req.item_id))
				{
					//这个错误一般都不会出现的
					return ;
				}

				//日志在player中完成

				//删除需求物品
				if(item_index >= 0)
				{
					pImp->UseItemLog(inv, item_index,1);
					inv.DecAmount(item_index,1);
					pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,item_index, _req.item_need,1);
				}

				//删除金钱
				if(_req.money_need > 0)
				{
					pImp->SpendMoney(_req.money_need);
					pImp->_runner->spend_money(_req.money_need);
				}
			}

		};
		request * req = (request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef bind_item_provider destroy_bind_item_provider;
class destroy_bind_item_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};
	typedef destroy_bind_item_provider::request request;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		//检查物品是否能够进行绑定销毁
		if(!pImp->CheckBindItemDestroy(preq->item_index, preq->item_id))
		{
			return false;
		}

		request req;
		req.item_need = 0;
		req.money_need = pImp->GetMoney();
		req.item_index = preq->item_index;
		req.item_id = preq->item_id;
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&req, sizeof(req));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(request));
		class op : public session_general_operation::operation
		{
			request _req; 
		public:
			op(const request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_BIND_DESTORY;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;

				if(_req.money_need > 0)
				{
					if(pImp->GetMoney() < (size_t)_req.money_need)
					{
						pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
						return;
					}
				}

				if(!pImp->CheckBindItemDestroy(_req.item_index, _req.item_id))
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}
				
				item_list & inv = pImp->GetInventory();
				int item_index = -1;
				if(_req.item_need && (item_index = inv.Find(0,_req.item_need)) < 0)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				//进行绑定操作
				if(!pImp->DestroyBindItem(_req.item_index, _req.item_id, pImp->GetInventory()))
				{
					//这个错误一般都不会出现的
					return ;
				}

				//日志在player中完成

				//删除需求物品
				if(item_index >= 0)
				{
					pImp->UseItemLog(inv, item_index,1);
					inv.DecAmount(item_index,1);
					pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,item_index, _req.item_need,1);
				}

				//删除金钱
				if(_req.money_need > 0)
				{
					pImp->SpendMoney(_req.money_need);
					pImp->_runner->spend_money(_req.money_need);
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=8:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),_req.money_need,_req.item_id);
				}
			}

		};
		request * req = (request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef bind_item_provider destroy_item_restore_provider;
class destroy_item_restore_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};
	typedef destroy_item_restore_provider::request request;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		//检查物品是否能够进行绑定销毁
		if(!pImp->CheckRestoreDestroyItem(preq->item_index, preq->item_id))
		{
			return false;
		}

		request req;
		req.item_need = 0;
		req.money_need = pImp->GetMoney();
		req.item_index = preq->item_index;
		req.item_id = preq->item_id;
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&req, sizeof(req));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(request));
		class op : public session_general_operation::operation
		{
			request _req; 
		public:
			op(const request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_BIND_DESTORY_RESTORE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;

				if(_req.money_need > 0)
				{
					if(pImp->GetMoney() < (size_t)_req.money_need)
					{
						pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
						return;
					}
				}

				if(!pImp->CheckRestoreDestroyItem(_req.item_index,_req.item_id))
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}
				
				item_list & inv = pImp->GetInventory();
				int item_index = -1;
				if(_req.item_need && (item_index = inv.Find(0,_req.item_need)) < 0)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				//进行绑定操作
				if(!pImp->RestoreDestroyItem(_req.item_index, _req.item_id))
				{
					//这个错误出现的概率较高，需要有正确的返回信息
					pImp->_runner->error_message(S2C::ERR_RESTORE_DESTROY);
					return ;
				}

				//日志在player中完成

				//删除需求物品
				if(item_index >= 0)
				{
					pImp->UseItemLog(inv, item_index,1);
					inv.DecAmount(item_index,1);
					pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,item_index, _req.item_need,1);
				}

				//删除金钱
				if(_req.money_need > 0)
				{
					pImp->SpendMoney(_req.money_need);
					pImp->_runner->spend_money(_req.money_need);
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=8:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),_req.money_need,_req.item_id);
				}
			}

		};
		request * req = (request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class resetprop_provider : public general_provider
{
public:
	struct request
	{
		size_t index;
		int item_id;
	};

	typedef npc_template::npc_statement::__reset_prop  entry_t;
	abase::vector<entry_t> _reset_list;
public:
	resetprop_provider() { }

private:
	virtual bool Save(archive & ar)
	{
		//未做
		ASSERT(false);
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		//未做
		ASSERT(false);
		return true;
	}
	virtual resetprop_provider * Clone()
	{
		ASSERT(!_is_init);
		return new resetprop_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size % sizeof(entry_t))
		{
			ASSERT(false);
			return false;
		}
		size_t t = size / sizeof(entry_t);
		if(t > 15)
		{
			ASSERT(false);
			return false;
		}
		
		entry_t * pEnt = (entry_t *)buf;
		_reset_list.reserve(t);
		for(size_t i = 0 ;i < t ; i++)
		{
			_reset_list.push_back(pEnt[i]);
		}

		return true;
	}
	
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能要返回位置列表或者税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(request))
		{
			ASSERT(false);
			return ;
		}
		request * req = (request*)buf;
		size_t index = req->index;
		if(index >= _reset_list.size())
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_UNAVILABLE,NULL,0);
			return;
		}
		
		if(req->item_id != _reset_list[index].object_need)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_ITEM_NOT_IN_INVENTORY,NULL,0);
			return ;
		}

		
		//发回回应数据
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,&(_reset_list[index]),sizeof(_reset_list[index]));
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}
};

class resetprop_executor : public service_executor
{
public:
	typedef resetprop_provider::request player_request;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		player_request *preq = (player_request*)buf;
		
		//检查物品是否存在
		if(preq->item_id == 0 || !pImp->IsItemExist(preq->item_id))
		{
			return false;
		}
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(resetprop_provider::entry_t));
		resetprop_provider::entry_t * pEnt = (resetprop_provider::entry_t *)buf;

		item_list & inv = pImp->GetInventory();
		int item_index = -1;
		if(!pEnt->object_need || (item_index = inv.Find(0,pEnt->object_need)) < 0)
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		
		//发送一个激活消息通知对方
		//洗点
		if(!pImp->ForgetSkill(pEnt->type))
		{
			//失败了
			pImp->_runner->error_message(S2C::ERR_NO_SKILL_TO_FORGET);
			return false;
		}
		
		if(item_index >= 0)
		{
			//检查是否需要记录消费值
			pImp->CheckSpecialConsumption(pEnt->object_need, 1);

			pImp->UseItemLog(inv, item_index,1);
			inv.DecAmount(item_index,1);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item_index,pEnt->object_need, 1 ,S2C::DROP_TYPE_USE);
		}

		return true;
	}
};

typedef feedback_provider stock_service_provider;

class stock_service_executor2 : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		
		//检查是否可以开始
		if(gmatrix::GetWorldParam().forbid_cash_trade)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
		if(gmatrix::GetWorldParam().forbid_cash_trade)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		player_request * req = (player_request*)buf;
		//发送请求数据
		int rst = GNET::ForwardStockCmd(req->service_id,req->buf, size - sizeof(player_request),object_interface(pImp));
		if(rst == 2)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return false;
		}
		else if(rst)
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		return true;
	}
};

class stock_service_executor1 : public service_executor
{
public:
	struct player_request
	{
		int withdraw;
		int cash;
		int money;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		
		//检查是否可以开始
		if(gmatrix::GetWorldParam().forbid_cash_trade)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));

		//检查是否开始
		if(gmatrix::GetWorldParam().forbid_cash_trade)
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION);
			return true;
		}
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		player_request * req = (player_request*)buf;
		//发送请求数据
		if(!GNET::SendStockTransaction(object_interface(pImp),req->withdraw, req->cash, req->money))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		return true;
	}
};

typedef feedback_provider talisman_refine_provider;
class talisman_refine_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//法宝 id
		int material_id;		//血炼材料的ID
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_REFINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->RefineTalisman(_req.item_index, _req.item_id, _req.material_id);
				if(!bRst) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_REFINE_ERROR);
				}
			}
			
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider talisman_lvlup_provider;
class talisman_lvlup_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//法宝 id
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_LVLUP;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->TalismanLevelUp(_req.item_index, _req.item_id);
				if(!bRst) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_LEVEL_UP);
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider talisman_reset_provider;
class talisman_reset_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//法宝 id
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_RESET;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->TalismanReset(_req.item_index, _req.item_id);
				if(!bRst) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_LEVEL_UP);
				}
			}
			
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};
typedef feedback_provider talisman_combine_provider;
class talisman_combine_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index1;		//物品 包裹栏索引
		int item_id1;			//法宝 id
		int item_index2;		//物品 包裹栏索引
		int item_id2;			//法宝 id
		int catalyst_id;		//法宝熔炼物品ID
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_COMBINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst=pImp->TalismanCombine(_req.item_index1,_req.item_id1,
								_req.item_index2,_req.item_id2,_req.catalyst_id);
				if(!bRst) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_COMBINE);
				}
			}
			
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

// 法宝灌魔
typedef feedback_provider talisman_enchant_provider;
class talisman_enchant_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index1;		// 物品 包裹栏索引 法宝
		int item_type1;			// id
		int item_index2;		// 物品 包裹栏索引 法宝熔炼物品
		int item_type2;			// id
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_ENCHANT;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp* pImp = (gplayer_imp*)obj;
				bool bRst = pImp->TalismanEnchant( _req.item_index1, _req.item_type1, 
								_req.item_index2, _req.item_type2 );
				if( !bRst ) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_ENCHANT);
				}
			}
			
		};
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider battle_field_challenge_service_provider;
class battle_field_challenge_service_executor : public service_executor
{
public:
	struct player_request
	{
		int service_id;
		char buf[];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < sizeof(player_request)) return false;
		if(size > 4096) return false;
		//检查是否可以开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size >= sizeof(player_request));
		//检查是否开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		//其他判断交给delivery吧(帮主,建立时间等等)
		player_request * req = (player_request*)buf;
		//发送请求数据
		if(!GNET::ForwardBattleOP(req->service_id,pImp->_parent->ID.id,req->buf,size - sizeof(player_request),object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		//pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		GLog::log(GLOG_INFO,"用户 %d 执行了战场操作",pImp->_parent->ID.id);
		return true;
	}
};

typedef feedback_provider battle_field_construct_service_provider;
class battle_field_construct_service_executor : public service_executor
{
public:
	struct player_request
	{
		int mode;	//0 察看信息, 1 捐献, 2 建设
		int type;	//mode为 0 时(0为取建筑信息1为取玩家捐献信息),	   mode为 1 时 这个表示捐献物资id,  mode为 2 时 表示建设类型
		int key;	//mode为 0 时,type为1时(表示取哪页,每页10个,0开始),type为 1 时 这个表示捐献物资key, mode为 2 时 这个表示建设的key
		int cur_level;	//只有mode为2时有用,当前建设等级用于校验
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(sizeof(player_request) != size) return false;
		//检查是否可以开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		//检查是否开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		player_request * req = (player_request*)buf;
		//发送请求数据
		if(0 == req->mode)
		{
			bool rst = pImp->QueryBattlefieldConstructInfo(req->type,req->key);
			if(!rst)
			{
				pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			}
		}
		else if(1 == req->mode)
		{
			bool rst = pImp->BattlefieldContribute(req->type);
			if(!rst)
			{
				pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			}
		}
		else if(2 == req->mode)
		{
			bool rst = pImp->BattlefieldConstruct(req->key,req->type,req->cur_level);
			if(!rst)
			{
				pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			}
		}
		else
		{
			__PRINTF("城战建设服务数据错误(mode: %d,type: %d,key: %d)\n",req->mode,req->type,req->key);
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
		}
		return true;
	}
};

class battle_field_employ_service_provider : public feedback_provider
{
public:
	int price;
	int war_material_id;
	int war_material_count;
	int item_wanted;
	int remove_one_item;
	int controller_id[20];

	struct data_need
	{
		int price;
		int war_material_id;
		int war_material_count;
		int item_wanted;
		int remove_one_item;
		int controller_id[20];
	};

private:
	battle_field_employ_service_provider *  Clone()
	{
		ASSERT(!_is_init);
		return new battle_field_employ_service_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(npc_template::npc_statement::__employ_t)) 
		{
			ASSERT(false);
			return false;
		}
		npc_template::npc_statement::__employ_t* p = (npc_template::npc_statement::__employ_t*)buf;
		price = p->price;
		war_material_id = p->war_material_id;
		war_material_count = p->war_material_count;
		item_wanted = p->item_wanted;
		remove_one_item = p->remove_one_item;
		memcpy(controller_id,p->controller_id,sizeof(int) * 20);
		for(int i = 0;i < MAX_ARCHER_COUNT; ++i)
		{
			if(controller_id[i] == 0)
			{
				return false;
			}
		}
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		data_need temp;
		temp.price = price;
		temp.war_material_id = war_material_id;
		temp.war_material_count = war_material_count;
		temp.item_wanted = item_wanted;
		temp.remove_one_item = remove_one_item;
		memcpy(temp.controller_id,controller_id,sizeof(int) * 20);
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,&temp,sizeof(data_need));
	}
};

class battle_field_employ_service_executor : public service_executor
{
public:
	struct data_need
	{
		int price;
		int war_material_id;
		int war_material_count;
		int item_wanted;
		int remove_one_item;
		int controller_id[20];
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		//检查是否可以开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(sizeof(data_need) == size);
		//检查是否开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		data_need* p = (data_need*)buf;
		//暂时不需要帮派资源
		//检测数据扣除物品钱,激活控制器
		int index = -1;
		for(int i = 0;i < MAX_ARCHER_COUNT;++i)
		{
			if(!pImp->GetWorldManager()->CheckServiceCtrlID(p->controller_id[i]))
			{
				index = i;
				break;
			}
		}
		if(-1 == index)
		{
			pImp->_runner->error_message(S2C::ERR_ARCHER_MAX);
			return false;
		}
		if(pImp->GetMoney() < (size_t)p->price)
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}
		if(!pImp->CheckItemExist(p->item_wanted,1))
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		pImp->SpendMoney((size_t)p->price);
		pImp->_runner->spend_money(p->price);
		if(p->remove_one_item)
		{
			int rst = 0;
			rst = pImp->GetInventory().Find(rst,p->item_wanted);
			pImp->GetInventory().DecAmount(rst,1);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,p->item_wanted,1,S2C::DROP_TYPE_USE);
		}
		world_manager::ActiveSpawn(pImp->GetWorldManager(),p->controller_id[index],true);
		pImp->GetWorldManager()->OnBuyArcher(pImp);
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d) 玩家 %d 执行了购买弓箭手操作",
			gmatrix::GetServerIndex(),pImp->GetWorldManager()->GetBattleID(),
			pImp->GetWorldManager()->GetWorldTag(),pImp->_parent->ID.id);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d) 玩家 %d 执行了购买弓箭手操作\n",
			gmatrix::GetServerIndex(),pImp->GetWorldManager()->GetBattleID(),
			pImp->GetWorldManager()->GetWorldTag(),pImp->_parent->ID.id);
		return true;
	}
};

typedef feedback_provider pet_adopt_provider;
class pet_adopt_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int inv_index;	//物品 包裹栏索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		int rst = pImp->PlayerAdoptPet(req->inv_index);
		if(0 != rst)
		{
			pImp->_runner->error_message(rst);
			//pImp->_runner->error_message( S2C::ERR_CAN_NOT_ADOPT_PET );
			return false;
		}
		return true;
	}
};

typedef feedback_provider pet_free_provider;
class pet_free_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int inv_index;	//物品 包裹栏索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		int rst = pImp->PlayerFreePet(req->inv_index);
		if(0 != rst)
		{
			pImp->_runner->error_message(rst);
		}
		return true;
	}
};

typedef feedback_provider pet_combine_provider;
class pet_combine_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int inv_index_pet;	//物品 包裹栏索引
		int inv_index_c;	//修炼道具 包裹索引栏
		int inv_index_a;	//辅助道具 包裹索引栏
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		int rst = pImp->PlayerRefinePet(req->inv_index_pet,req->inv_index_c,req->inv_index_a);
		if(0 != rst)
		{
			pImp->_runner->error_message(rst);
		}
		return true;
	}
};

typedef feedback_provider pet_combine2_provider;
class pet_combine2_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int inv_index_pet;	//物品 包裹栏索引
		int type; 
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		int rst = pImp->PlayerRefinePet2(req->inv_index_pet,req->type);
		if(0 != rst)
		{
			pImp->_runner->error_message(rst);
		}
		return true;
	}
};


typedef feedback_provider pet_rename_provider;
class pet_rename_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int inv_index;	//物品 包裹栏索引
		char name[pet_bedge_enhanced_essence::MAX_PET_NAME_LENGTH];
		int size;
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		int rst = pImp->RenamePet(req->inv_index,req->name,req->size);
		if(0 != rst)
		{
			pImp->_runner->error_message(rst);
		}
		return true;
	}
};

typedef feedback_provider mount_item_renew_provider;
class mount_item_renew_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int inv_mount;
		int inv_material;
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		pImp->RenewMountItem(req->inv_mount, req->inv_material);
		return true;
	}
};


class item_trade_provider : public general_provider
{
	int _trade_template_id[4];
public:
	virtual ~item_trade_provider()
	{
	}

	struct bag
	{
		int item_type;
		size_t index;
		size_t count;
	};

	struct request
	{
		int index;	//用第几个
		size_t count;
		bag item_list[];
	};


	struct item_trade_t
	{
		int item_type; 
		int item_num;  //每组的个数
		
		int count;     //购买的组数

		struct
		{
			int id;
			int count;
		}need_rep[2];

		struct
		{
			int id;
			int count;
		}need_item[2];
		
		struct
		{
			int id;
			int count;
		}need_special;
	};

	item_trade_provider()
	{
	}

private:
	virtual bool Save(archive & ) {return true;}
	virtual bool Load(archive & ) {return true;}
	
	
	virtual item_trade_provider * Clone()
	{
		ASSERT(!_is_init);
		return new item_trade_provider(*this);
	}
	
	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(_trade_template_id)) 
		{
			ASSERT(false);
			return false;
		}
		memcpy(_trade_template_id, buf, size);
		return true;
	}
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能需要返回税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		//param 是 索引 buf 代表数量和自己的钱数(初步判断,未必准确)
		request * param = (request *)buf;
		if(size != sizeof(request) + param->count*sizeof(bag)) return;

		abase::vector<item_trade_t, abase::fast_alloc<> > list;
		list.reserve(param->count);  //这里的count代表交易的笔数
		
		if(param->index < 0 || param->index >= 4) return;
		if(_trade_template_id[param->index] <= 0) return;

		item_trade_template * pIt = item_trade_manager::GetItemTradeTemplate(_trade_template_id[param->index]);
		if(pIt == NULL) return;

		for(size_t i=0; i < param->count; ++i)
		{       
			size_t index = param->item_list[i].index;
			if(index < 0 || index >= 48*4 || param->item_list[i].count <= 0)
			{       
				//客户端传来的错误的数据
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}

			item_trade  p = pIt->item_trade_goods[index];
			if(p.id <= 0 || p.id != param->item_list[i].item_type || p.item_num <= 0)
			{
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}

			struct item_trade_t entry;
			entry.item_type = p.id; 
			entry.item_num = p.item_num;
			entry.count = param->item_list[i].count;
			entry.need_item[0].id 		=	p.item_require[0].id;
			entry.need_item[0].count	=	p.item_require[0].count;
			entry.need_item[1].id		=	p.item_require[1].id;
			entry.need_item[1].count	=	p.item_require[1].count;

			entry.need_rep[0].id 		=	p.rep_require[0].id;
			entry.need_rep[0].count		=	p.rep_require[0].count;
			entry.need_rep[1].id		=	p.rep_require[1].id;
			entry.need_rep[1].count		=	p.rep_require[1].count;
			
			entry.need_special.id 		=	p.special_require.id;
			entry.need_special.count	=	p.special_require.count;
			list.push_back(entry);

		}
		
		//发出物品数据和要求的钱数
		//发出回馈数据
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,list.begin(),sizeof(item_trade_t) * list.size());
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}

};

class item_trade_executor : public service_executor
{
public:
	typedef item_trade_provider::request player_request;
	typedef item_trade_provider::item_trade_t  trade_entry;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		player_request * req= (player_request*)buf;
		if(req->count <= 0 || 
			size != sizeof(player_request)+req->count*sizeof(item_trade_provider::bag )) return false;
		if( req->count > 48) return false;

		if(!pImp->InventoryHasSlot(req->count)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}
	
	static bool IncNumber(int & number, int inc, int count)
	{
		int old_n = number;
		int offset = inc * count;
		if(offset / count != inc) return false;
		int new_n = old_n + offset;
		if(new_n < old_n) return false;
		number = new_n;
		return true;
	}
	


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size ==0 || (size % sizeof(trade_entry)) != 0) 
		{
			return false;
		}

		
		size_t count = size / sizeof(trade_entry);
		if(!pImp->InventoryHasSlot(count)) 
		{
			pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}

		const trade_entry * list = ( const trade_entry *) buf;
		std::map<int ,int > need_item;
		std::map<int ,int > need_rep;
		std::map<int, int > need_special;
		for(size_t i = 0; i < count; i ++)
		{
			const trade_entry & p = list[i];
			int type = p.item_type;
			size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(type);
			if(p.count == 0 || pile_limit == 0 || (size_t)p.count * p.item_num >pile_limit) 
			{
				return false;
			}
			
			if(!IncNumber(need_item[p.need_item[0].id], p.need_item[0].count, p.count)) return false;
			if(!IncNumber(need_item[p.need_item[1].id], p.need_item[1].count, p.count)) return false;
			if(!IncNumber(need_rep[p.need_rep[0].id], p.need_rep[0].count, p.count)) return false;
			if(!IncNumber(need_rep[p.need_rep[1].id], p.need_rep[1].count, p.count)) return false;
			if(!IncNumber(need_special[p.need_special.id], p.need_special.count, p.count)) return false;
		}
		need_item.erase(0);
		need_rep.erase(0);
		need_special.erase(0);
		
		//检查材料和声望是否足够
		std::map<int ,int >::iterator it;
		for(it = need_item.begin(); it != need_item.end(); ++ it) 
		{
			if(it->second <= 0) continue;
			if(!pImp->CheckNonExpireItemExist(it->first, it->second)) 
			{
				//$$$$$$$$$$$
				return false;
			}
		}
		
		for(it = need_rep.begin(); it != need_rep.end(); ++ it) 
		{
			if(it->second <= 0) continue;
			if(pImp->GetRegionReputation(it->first) < it->second ) 
			{
				//$$$$$$$$$$$
				return false;
			}
		}
		
		for(it = need_special.begin(); it != need_special.end(); ++ it) 
		{
			if(it->second <= 0) continue;
			if(!pImp->CheckTradeSpecialReq(it->first, it->second))
			{
				return false;
			}
		}

		//扣除物品
		for(it = need_item.begin(); it != need_item.end(); ++ it) 
		{
			if(it->second <= 0) continue;
			//检查是否需要记录消费值
			pImp->CheckSpecialConsumption(it->first, it->second);
			pImp->TakeOutNonExpireItem(it->first, it->second);
		}
		//扣除声望	
		for(it = need_rep.begin(); it != need_rep.end(); ++ it) 
		{
			if(it->second <= 0) continue;
			pImp->ModifyRegionReputation(it->first, -it->second);
		}
		//扣除其他约定的东西
		for(it = need_special.begin(); it != need_special.end(); ++ it) 
		{
			if(it->second <= 0) continue;
			pImp->ConsumeTradeSpecialReq(it->first, it->second);
		}


		//给player添加物品
		abase::vector<abase::pair<const item_data*,int> ,abase::fast_alloc<> > item_list;
		item_list.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			int item_id = list[i].item_type;
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
			if(pItem) item_list.push_back(abase::pair<const item_data*,int>(pItem,list[i].count*list[i].item_num));
		}
		pImp->PurchaseItem(item_list.begin(),item_list.size(), 0);
		return true;
	}
};

typedef feedback_provider lock_item_provider;
class lock_item_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		if(!pImp->CheckItemLockCondition(preq->item_index, preq->item_id))
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_LOCK;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				int rst = pImp->DoLockItem(_req.item_index,_req.item_id);
				if(rst )
				{
					pImp->_runner->error_message(rst);
				}
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider unlock_item_provider;
class unlock_item_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		if(!pImp->CheckItemUnlock(preq->item_index, preq->item_id))
		{
			return false;
		}
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf, size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_UNLOCK;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				int rst = pImp->DoUnlockItem(_req.item_index,_req.item_id);
				if(rst )
				{
					pImp->_runner->error_message(rst);
				}
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class broken_item_restore_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		//检查物品是否能够进行绑定销毁
		if(!pImp->CheckRestoreBrokenItem(preq->item_index, preq->item_id))
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_BIND_DESTORY_RESTORE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(10);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckRestoreBrokenItem(_req.item_index,_req.item_id))
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}
				
				item_list & inv = pImp->GetInventory();
				int item_index = -1;
				int item_need = g_config.item_restore_broken_id;
				if(item_need && (item_index = inv.Find(0,item_need)) < 0)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				
				//进行恢复操作
				if(!pImp->RestoreDestroyItem(_req.item_index, _req.item_id))
				{
					//这个错误出现的概率较高，需要有正确的返回信息
					pImp->_runner->error_message(S2C::ERR_RESTORE_DESTROY);
					return ;
				}

				//日志在player中完成

				//删除需求物品
				if(item_index >= 0)
				{
					pImp->UseItemLog(inv, item_index,1);
					inv.DecAmount(item_index,1);
					pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,item_index, item_need,1);
					//检查是否需要记录消费值
					pImp->CheckSpecialConsumption(item_need, 1);
				}
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class blood_enchant_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_SPIRIT_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(10);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				item_list & inv = pImp->GetInventory();
				int item_index = -1;
				int item_need = g_config.item_blood_enchant;
				if(!item_need || (item_index = inv.Find(0,item_need)) < 0)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				item & it = inv[_req.item_index];
				if(it.type == -1 || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_BLOOD_ENCHANT_FAILED);
					return ;
				}

				if(!it.body->DoBloodEnchant(_req.item_index, obj, &it))
				{
					pImp->_runner->error_message(S2C::ERR_BLOOD_ENCHANT_FAILED);
					return ;
				}
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);
				pImp->UseItemLog(inv, item_index,1);
				inv.DecAmount(item_index,1);
				pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,item_index, item_need,1);
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class spirit_insert_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
		int soul_index;
		int soul_slot;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_SPIRIT_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(4);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				item_list & inv = pImp->GetInventory();
				int soul_id =  0;
				if(_req.soul_index < 0 || _req.soul_index >= (int)inv.Size() || (soul_id = inv[_req.soul_index].type) <= 0)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				item & it = inv[_req.item_index];
				if(it.type == -1 || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_SPIRIT_ADDON_FAILED);
					return ;
				}

				int rst = it.body->InsertSpiritAddon(obj, &it, soul_id, _req.soul_slot);
				if(rst)
				{
					pImp->_runner->error_message(rst>0?rst:S2C::ERR_SPIRIT_ADDON_FAILED);
					return ;
				}

				pImp->UseItemLog(inv, _req.soul_index,1);
				inv.DecAmount(_req.soul_index,1);
				pImp->_runner->use_item(gplayer_imp::IL_INVENTORY,_req.soul_index, soul_id,1);

				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class spirit_remove_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
		int soul_slot;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_SPIRIT_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(4);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				item_list & inv = pImp->GetInventory();
				int soul_id =  0;

				item & it = inv[_req.item_index];
				if(it.type == -1 || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED);
					return ;
				}

				int rst = it.body->RemoveSpiritAddon(obj, &it, _req.soul_slot, &soul_id);
				if(rst)
				{
					pImp->_runner->error_message(rst>0?rst:S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED);
					return ;
				}

				if(soul_id > 0)
				{
					PlayerTaskInterface  task_if(pImp);
					task_if.DeliverCommonItem(soul_id, 1, false, 0);
				}

				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class spirit_power_restore_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_SPIRIT_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(4);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				item_list & inv = pImp->GetInventory();
				item & it = inv[_req.item_index];
				if(it.type == -1 || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED);
					return ;
				}

				int rst = it.body->RestoreSpiritPower(obj, &it);
				if(rst)
				{
					pImp->_runner->error_message(rst>0?rst:S2C::ERR_RESTORE_SPIRIT_POWER);
					return ;
				}

				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class spirit_decompose_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//检查安全锁
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_SPIRIT_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(4);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				item_list & inv = pImp->GetInventory();

				item & it = inv[_req.item_index];
				if(it.type == -1 || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED);
					return ;
				}

				//装备是否锁定
				if(it.IsLocked()) 
				{
					return ;
				}

				//限时装备不可以拆解
				if(it.expire_date > 0)
				{
					return;
				}

				int spirit_unit_base = 0;
				int spirit_unit_adv = 0;
				int rst = it.body->SpiritDecompose(obj, &it, &spirit_unit_adv, &spirit_unit_base);
				if(rst)
				{
					pImp->_runner->error_message(rst>0?rst:S2C::ERR_SPIRIT_DECOMPOSE_FAILED);
					return ;
				}

				//删除装备本身
				pImp->UseItemLog(inv, _req.item_index,it.count);
				pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,_req.item_index,it.type, it.count,S2C::DROP_TYPE_TAKEOUT);
				item mt;
				inv.Remove(_req.item_index, mt);
				mt.Release();

				//增加元魂珠

				PlayerTaskInterface  task_if(pImp);
				if(spirit_unit_base > 0 && g_config.base_spirit_stone_id > 0) task_if.DeliverCommonItem(g_config.base_spirit_stone_id, spirit_unit_base, false, 0);
				if(spirit_unit_adv > 0 && g_config.adv_spirit_stone_id > 0) task_if.DeliverCommonItem(g_config.adv_spirit_stone_id, spirit_unit_adv, false, 0);
			}

		};
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class npc_produce_provider : public feedback_provider
{
public:
	struct player_request
	{
		int recipe_id;
		int territory_id;
	};
	int _territory_id;
	abase::vector<int> _recipe_list; 

private:
	npc_produce_provider * Clone()
	{
		ASSERT(!_is_init);
		return new npc_produce_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(int))
		{
			ASSERT(false);
			return false;
		}
		int npc_produce_id = *(int*)buf;
		if(npc_produce_id <= 0) return false;
		
		npc_produce_template * pNp = npc_produce_manager::GetNPCProduceTemplate(npc_produce_id);
		if(pNp == NULL) return false;

		int * list = pNp->npc_produce_list;
		size_t count = sizeof(pNp->npc_produce_list) / sizeof(int);

		_recipe_list.reserve(count);
		for(size_t i = 0; i < count; i ++)
		{
			_recipe_list.push_back(list[i]);
		}
		std::sort(_recipe_list.begin(),_recipe_list.end());

		_territory_id = _imp->OI_GetTerritoryID();
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return;
		player_request * req = (player_request*) buf;

		if(_territory_id != req->territory_id)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
			return ;
		}

		if(!std::binary_search(_recipe_list.begin(),_recipe_list.end(),req->recipe_id))
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
			return ;
		}
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
	}
};

class npc_produce_executor : public service_executor
{
public:
	struct player_request
	{
		int recipe_id;
		int territory_id;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			const recipe_template * _rt;
		public:
			op(const recipe_template *rt ):_rt(rt)
			{}

			virtual int GetID()
			{
				return S2C::GOP_SPIRIT_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(4);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				pImp->ProduceItem(*_rt, true);
			}

		};

		player_request * req = (player_request*) buf;

		if(req->territory_id != 0 && !pImp->IsTerritoryOwner(req->territory_id))
		{
			pImp->_runner->error_message(S2C::ERR_NOT_TERRITORY_OWNER);
			return false;
		}
		
		const recipe_template *rt = recipe_manager::GetRecipe(req->recipe_id);
		if(!rt) return false;

		session_general_operation * pSession = new session_general_operation(pImp,  new op(rt));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider petequip_refine_provider;
class petequip_refine_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//物品 id
		int stone_index;		//各种石头的索引
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_PETEQUIP_REFINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->RefinePetEquip(_req.item_index, _req.item_id, _req.stone_index);
				if(!bRst) 
				{
					//宠物装备强化错误
					pImp->_runner->error_message(S2C::ERR_CAN_NOT_REFINE_PETEQUIP);
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};


typedef feedback_provider change_style_provider;
class change_style_executor: public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		unsigned char faceid;
		unsigned char hairid;
		unsigned char earid;
		unsigned char tailid;
		unsigned char fashionid;
		int item_index;
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_CHANGE_STYLE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				pImp->PlayerChangeStyle(_req.faceid, _req.hairid, _req.earid, _req.tailid, _req.fashionid, _req.item_index);
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider magic_refine_provider;
class magic_refine_executor: public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;	
		int item_id; 
		int stone_index;
		int stone_id;
		int refine_type;
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_MAGIC_REFINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->RefineMagic(_req.item_index, _req.item_id, _req.stone_index, _req.stone_id, _req.refine_type); 
				if(!bRst) 
				{
					pImp->_runner->error_message(S2C::ERR_MAGIC_REFINE_ERROR);
				}
			}
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider magic_restore_provider;
class magic_restore_executor: public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;
		int item_id;
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_MAGIC_REFINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->RestoreMagic(_req.item_index, _req.item_id);
				if(!bRst) 
				{
					pImp->_runner->error_message(S2C::ERR_MAGIC_RESTORE_ERROR);
				}
			}
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider territory_challenge_provider;
class territory_challenge_executor : public service_executor
{
public:
	struct player_request
	{
		int mafia_id;
		int territory_id;
		int item_id;
		int item_amount;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//检查是否可以开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		player_request * req = (player_request*) buf;
		//检查是否开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		if(pImp->OI_GetMafiaID() != req->mafia_id)
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		
		if(req->item_id < 0 ||  req->item_amount < 0 ) 
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		if(pImp->GetInventory().CountItemByID(req->item_id) < req->item_amount)
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		//发送请求数据
		if(!GNET::SendTerritoryChallenge(pImp->_parent->ID.id, req->territory_id, (unsigned int)req->mafia_id, 
					req->item_id, req->item_amount, object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户 %d 执行了领土战宣战报名操作, mafia_id=%d, territory_id=%d, item_id=%d,  item_amount=%d",
				pImp->_parent->ID.id, req->mafia_id, req->territory_id, req->item_id,  req->item_amount);
		return true;
	}
};


class territory_enter_provider : public feedback_provider
{
public:
#pragma pack(1)
	struct player_request
	{
		int mafia_id;
		int territory_id;
		unsigned char is_assist;
	};
#pragma pack()
	int id;

private:
	territory_enter_provider * Clone()
	{
		ASSERT(!_is_init);
		return new territory_enter_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(int))
		{
			ASSERT(false);
			return false;
		}
		id = *(int*)buf;
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return;
		player_request * req = (player_request*) buf;

		if(id != req->territory_id)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
			return ;
		}
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
	}
};

class territory_enter_executor : public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int mafia_id;
		int territory_id;
		unsigned char is_assist;
	};
#pragma pack()

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		player_request * req = (player_request*) buf;
		//需要检查是否拥有佣兵证书等
		if(!pImp->PlayerEnterTerritory(req->mafia_id, req->territory_id, req->is_assist))
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		
		//发送请求数据
		if(!GNET::SendTerritoryEnter(pImp->_parent->ID.id, (unsigned int)req->mafia_id, req->territory_id))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户 %d 执行了领土战进入操作, mafia_id=%d, territory_id=%d, is_assist=%d",
				pImp->_parent->ID.id, req->mafia_id, req->territory_id, (int)req->is_assist);
		return true;
	}
};

typedef feedback_provider territory_reward_provider;
class territory_reward_executor : public service_executor
{
public:
	struct player_request
	{
		int mafia_id;
		int territory_id;
		int reward_type; //1：佣兵之证 2：竞标失败宣战金返还 3：战斗胜利宣战金奖励 4：神器
		int item_id;
		int item_count;
		int money;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		
		if(!pImp->IsDeliverLegal() || pImp->_cur_session || pImp->_session_list.size())
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		player_request * req = (player_request*) buf;
		if(!pImp->IsDeliverLegal() || pImp->_cur_session || pImp->_session_list.size())
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		if(!pImp->PlayerGetTerritoryAward(req->mafia_id,  req->reward_type, req->item_id, req->item_count, req->money))
		{
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		
		//发送请求数据
		if(!GNET::SendTerritoryGetAward(pImp->_parent->ID.id, (unsigned int)req->mafia_id, req->territory_id, (char)req->reward_type, req->item_id, req->item_count, req->money, object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户 %d 执行了领取领土战物品操作, mafia_id=%d, territory_id=%d, reward_type=%d, item_id=%d, item_count=%d, money=%d",
				pImp->_parent->ID.id, req->mafia_id, req->territory_id, req->reward_type, req->item_id, req->item_count, req->money);
		return true;
	}
};
typedef feedback_provider arena_challenge_provider;
class arena_challenge_executor : public service_executor
{
public:
	struct player_request
	{
		int map_id;
		int item_index;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//检查是否可以开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		//检查是否开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		//其他判断交给delivery吧(帮主,建立时间等等)
		player_request * req = (player_request*)buf;
		if(req->item_index < 0 || (size_t)req->item_index > pImp->GetInventory().Size())
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		int item_id = pImp->GetRebornCount() == 0 ? g_config.item_arena : g_config.item_reborn_arena;
		item & it = pImp->GetInventory()[req->item_index];
		if(it.type != item_id && item_id != 0)
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}

		//发送请求数据
		if(!GNET::SendArenaChallenge(pImp->_parent->ID.id, gmatrix::GetServerIndex(), req->map_id, item_id, req->item_index, object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户 %d 执行了竞技场报名操作",pImp->_parent->ID.id);
		return true;
	}
};

typedef feedback_provider charge_telestation_provider;
class charge_talestation_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
		int stone_index;
		int stone_id;
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//检查是否可以开始
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(!pImp->CanTrade(provider))
		{
			return false;
		}
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_MAGIC_REFINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->ChargeTeleStation(_req.item_index, _req.item_id, _req.stone_index, _req.stone_id); 
				if(!bRst) 
				{
				}
			}
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};


typedef bind_item_provider repair_damage_item_provider;
class repair_damage_item_executor : public service_executor
{
public:
	struct player_request
	{
		int item_index;
		int item_id;
	};
	typedef repair_damage_item_provider::request request;

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request *preq = (player_request*)buf;

		//检查物品是否能够进行绑定销毁
		if(!pImp->CheckRestoreDestroyItem(preq->item_index, preq->item_id))
		{
			return false;
		}

		request req;
		req.item_need = 0;
		req.money_need = 500000000;
		req.item_index = preq->item_index;
		req.item_id = preq->item_id;
		if(pImp->GetMoney() < (size_t)req.money_need)
		{
			return false;
		}
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,&req, sizeof(req));
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(request));
		class op : public session_general_operation::operation
		{
			request _req; 
		public:
			op(const request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_REPAIR_DAMAGE_ITEM;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;

				if(_req.money_need > 0)
				{
					if(pImp->GetMoney() < (size_t)_req.money_need)
					{
						pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
						return;
					}
				}

				if(!pImp->CheckRestoreDestroyItem(_req.item_index,_req.item_id))
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return ;
				}

				//进行绑定操作
				if(!pImp->RestoreDestroyItem(_req.item_index, _req.item_id))
				{
					//这个错误出现的概率较高，需要有正确的返回信息
					pImp->_runner->error_message(S2C::ERR_RESTORE_DESTROY);
					return ;
				}

				//删除金钱
				if(_req.money_need > 0)
				{
					pImp->SpendMoney(_req.money_need);
					pImp->_runner->spend_money(_req.money_need);
				}

				GLog::log(GLOG_INFO,"用户%d用%d金钱修复了破碎的物品%d", pImp->_parent->ID.id, _req.money_need, _req.item_id); 
			}

		};
		request * req = (request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider equipment_upgrade_provider;
class equipment_upgrade_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//物品 id
		int stone_index;		//模具 index 
		int rt_index;			//精炼保留道具 index
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_UPGRADE_EQUIPMENT;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->UpgradeEquipment(_req.item_index, _req.item_id, _req.stone_index, _req.rt_index);
				if(!bRst) 
				{
					//精炼出错，要报告一个错误
					pImp->_runner->error_message(S2C::ERR_CAN_NOT_UPGRADE_EQUIPMENT);
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

class consign_provider : public general_provider
{
public:
	enum 
	{ 
		CONSIGN_MONEY			= 1, 
		CONSIGN_ITEM			= 2, 
		CONSIGN_ROLE			= 4, 
	};

#pragma pack(1)
	struct player_request
	{
		char 	type; 				//1, 金钱; 2, 物品; 4, 角色
		int 	item_id;			//寄售物品ID
		int 	item_idx;			//寄售物品包裹中的索引
		short 	item_cnt;			//寄售物品的数量
		int 	money;				//寄售金钱数量
		int		sold_time;			//上架时间, 时间为天
		int		price;				//卖出价格，单位RMB元
		char 	sellto_name_len;	//卖出给特定玩家名字长度
		char	sellto_name[28]; 	//卖出给特定玩家名字
	};
#pragma pack()
	
	struct request_internal
	{
		player_request* preq;
		int item_type; //寻宝网用物品类型
		int margin;
	};

	int margin; //寄售保证金,寄售角色不需要保证金

private:

	virtual consign_provider * Clone()
	{
		ASSERT(!_is_init);
		return new consign_provider(*this);
	}

	virtual bool Save(archive & ar)
	{
		ar << margin;
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		ar >> margin;
		return true;
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		ASSERT(size == sizeof(int));
		margin = *(int*)buf;
		return true;
	}
	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		//将数据发回,表示有这个服务
		ASSERT(size == sizeof(player_request));
		request_internal ireq;
		ireq.preq = (player_request*)buf;
		if(ireq.preq->type == 1)
		{
			ireq.item_type = gmatrix::GetConsignItemType(0);
		}
		else if(ireq.preq->type == 2)
		{
			ireq.item_type = gmatrix::GetConsignItemType(ireq.preq->item_id);
		}
		else if(CONSIGN_ROLE == ireq.preq->type)
		{
			ireq.item_type		= g_config.consign_role_type;
			ireq.preq->item_id	= g_config.consign_role_item_id;
		}

		//保证金
		if(CONSIGN_ROLE == ireq.preq->type)
		{
			ireq.margin = 0; //寄售角色不需要保证金 
		}
		else
		{
			ireq.margin = 100000;
		}

		SendMessage(GM_MSG_SERVICE_DATA,player,_type, &ireq, sizeof(ireq));
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
	}
};

class consign_executor : public service_executor
{
private:	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(consign_provider::player_request)) return false;

		//检查安全锁
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(consign_provider::request_internal))
		{
			ASSERT(false);
			return false;
		}

		consign_provider::request_internal * req = (consign_provider::request_internal*) buf;
		if(consign_provider::CONSIGN_ROLE == req->preq->type)
		{
			int retcode = pImp->CanConsignRole(req->preq->type, req->preq->item_idx, req->preq->sold_time, req->preq->price, req->margin);
			if(retcode != 0)
			{
				return false;
			}
			pImp->StartConsignRole(req->preq->type, req->preq->item_id, req->preq->item_idx, 1, req->item_type,
					req->preq->sold_time, req->preq->price, req->margin, req->preq->sellto_name, req->preq->sellto_name_len);
		}
		else
		{
			int retcode = pImp->CanConsign(req->preq->type, req->margin, req->preq->item_id, req->preq->item_cnt, req->preq->item_idx, 
					req->item_type, req->preq->money, req->preq->price, req->preq->sold_time);
			if(retcode != 0)
			{
				return false;
			}
			pImp->StartConsign(req->preq->type, req->margin, req->preq->item_id, req->preq->item_cnt, req->preq->item_idx, req->item_type, 
					req->preq->money, req->preq->price, req->preq->sold_time, req->preq->sellto_name, req->preq->sellto_name_len);
		}

		return true;
	}
};

typedef feedback_provider crossserver_in_provider;
class crossserver_in_executor : public service_executor
{
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != 0) return false;
		
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != 0) return false;
		if(pImp->IsCombatState()) return false;
		if(!pImp->IsDeity() && !pImp->CheckGMPrivilege() ) return false;

		pImp->PlayerTryChangeDS(1);
		return true;
	}
};

typedef feedback_provider crossserver_out_provider;
class crossserver_out_executor : public service_executor
{
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != 0) return false;
		
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != 0) return false;
		if(pImp->IsCombatState()) return false;
		if(!pImp->IsDeity() && !pImp->CheckGMPrivilege() ) return false;

		pImp->PlayerTryChangeDS(2);
		return true;
	}
};

//---------------------------------------------------------------------------
// 宝石镶嵌相关服务, Add by Houjun 2011-03-08
//---------------------------------------------------------------------------

//装备宝石插槽鉴定服务
class identify_gem_slots_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//装备ID
	};
#pragma pack()

private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{	
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				if(g_config.fee_gem_slot_identify > 0 && pImp->GetMoney() < (size_t)g_config.fee_gem_slot_identify)
				{
					pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
					return;
				}

				item_list & inv = pImp->GetInventory();
				item & it = inv[_req.item_index];
				if(it.type == -1 || it.type != _req.item_id || it.body == NULL || it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}
				/*if(it.IsLocked())
				{					
					return;
				}*/

				if(!it.body->IdentifyGemSlots(_req.item_index, obj, &it))
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_IDENTIFY_GEMSLOT);
					return;
				}
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);

				//扣除金钱
				if(g_config.fee_gem_slot_identify > 0)
				{
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=9:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),g_config.fee_gem_slot_identify,it.type);
					pImp->SpendMoney(g_config.fee_gem_slot_identify);
					pImp->_runner->spend_money(g_config.fee_gem_slot_identify);
				}
				pImp->_runner->gem_notify(_req.item_id, S2C::GEM_SLOT_IDENTIFY_SUCC);

			}
		};
		
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//装备宝石插槽重铸服务
class rebuild_gem_slots_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//装备ID
		bool lock[3];			//标明是否锁定，锁定的不重铸
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}
				
				item_list & inv = pImp->GetInventory();
				item & it = inv[_req.item_index];
				if(it.type == -1 || it.type != _req.item_id || it.body == NULL || it.GetItemType() != item_body::ITEM_TYPE_EQUIPMENT)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				/*if(it.IsLocked())
				{
					return;
				}*/

				if(g_config.fee_gem_slot_rebuild > 0 && pImp->GetMoney() < (size_t)g_config.fee_gem_slot_rebuild)
				{
					pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
					return;
				}
	
				if(!it.body->RebuildGemSlots(obj, &it, _req.lock))
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_REBUILD_GEMSLOTS);
					return;
				}
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);	

				//扣除金钱
				if(g_config.fee_gem_slot_rebuild > 0)
				{
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=9:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),g_config.fee_gem_slot_rebuild,it.type);
					pImp->SpendMoney(g_config.fee_gem_slot_rebuild);
					pImp->_runner->spend_money(g_config.fee_gem_slot_rebuild);
				}
				pImp->_runner->gem_notify(_req.item_id, S2C::GEM_SLOT_REBUILD_SUCC);
			}
		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//装备宝石插槽定制服务
class customize_gem_slots_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//装备ID
		int gem_seal_id[3];		//定制宝石魔印的ID
		int gem_seal_index[3];	//定制宝石魔印在包裹栏的索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				item_list & inv = pImp->GetInventory();
				item & it = inv[_req.item_index];
				if(it.type == -1 || it.type != _req.item_id || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				/*if(it.IsLocked())
				{
					return;
				}*/

				if(g_config.fee_gem_slot_customize > 0 && pImp->GetMoney() < (size_t)g_config.fee_gem_slot_customize)
				{
					pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
					return;
				}

				if(!it.body->CustomizeGemSlots(pImp, &it, _req.gem_seal_id, _req.gem_seal_index))
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_CUSTOMIZE_GEMSLOTS);
					return;
				}	
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);	

				//扣除金钱
				if(g_config.fee_gem_slot_customize > 0)
				{
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=9:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),g_config.fee_gem_slot_customize,it.type);
					pImp->SpendMoney(g_config.fee_gem_slot_customize);
					pImp->_runner->spend_money(g_config.fee_gem_slot_customize);
				}
				pImp->_runner->gem_notify(_req.item_id, S2C::GEM_SLOT_CUSTOMIZE_SUCC);
			}
		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//宝石镶嵌服务
class embed_gems_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//装备ID
		int gem_id[3];			//定制宝石的ID
		int gem_index[3];		//定制宝石在包裹栏的索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}


				item_list & inv = pImp->GetInventory();
				item & it = inv[_req.item_index];
				if(it.type == -1 || it.type != _req.item_id || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_CUSTOMIZE_GEMSLOTS);
					return;
				}

				/*if(it.IsLocked())
				{
					return;
				}*/

				if(g_config.fee_gem_tessellation > 0 && pImp->GetMoney() < (size_t)g_config.fee_gem_tessellation)
				{
					pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
					return;
				}
				if(!it.body->EmbededGems(pImp, &it, _req.gem_id, _req.gem_index))
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_EMBED_GEMS);
					return;
				}	
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);	
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
				//扣除金钱
				if(g_config.fee_gem_tessellation > 0)
				{
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=10:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),g_config.fee_gem_tessellation,it.type);
					pImp->SpendMoney(g_config.fee_gem_tessellation);
					pImp->_runner->spend_money(g_config.fee_gem_tessellation);
				}
				pImp->_runner->gem_notify(_req.item_id, S2C::GEM_EMBED_SUCC);
			}
		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//宝石拆除服务
class remove_gems_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//装备ID
		bool remove[3];			//表示对应槽的宝石是否拆除
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->CheckItemExist(_req.item_index, _req.item_id,1)) 
				{
					pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;
				}

				item_list & inv = pImp->GetInventory();
				item & it = inv[_req.item_index];
				if(it.type == -1 || it.type != _req.item_id || it.body == NULL)
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_REMOVE_GEMS);
					return;
				}
				if(g_config.fee_gem_dismantle > 0 && pImp->GetMoney() < (size_t)g_config.fee_gem_dismantle)
				{
					pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
					return;
				}

				/*if(it.IsLocked())
				{
					return;
				}*/

				if(!it.body->RemoveGems(pImp, &it, _req.remove))
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_REMOVE_GEMS);
					return;
				}
				pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,_req.item_index);			
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
				//扣除金钱
				if(g_config.fee_gem_dismantle > 0)
				{
					GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=10:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),g_config.fee_gem_dismantle,it.type);
					pImp->SpendMoney(g_config.fee_gem_dismantle);
					pImp->_runner->spend_money(g_config.fee_gem_dismantle);
				}
				pImp->_runner->gem_notify(_req.item_id, S2C::GEM_REMOVE_SUCC);
			}
		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//宝石升品服务
class upgrade_gem_level_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int gem_index;			//宝石 包裹栏索引
		int gem_id;				//宝石ID
		int upgradeItemId[12];		//精练道具ID		
		int upgradeItemIndex[12];	//精炼道具包裹栏索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool rst = pImp->UpgradeGemLevel(_req.gem_id, _req.gem_index, _req.upgradeItemId, _req.upgradeItemIndex);
				if(!rst) 
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_UPGRADE_GEM_LEVEL);
				}
				pImp->PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
			}

		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//宝石精练服务
class upgrade_gem_quality_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int gem_index;			//宝石 包裹栏索引
		int gem_id;				//宝石ID
		int upgradeItemId;		//精练道具ID		
		int upgradeItemIndex;	//精炼道具包裹栏索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->UpgradeGemQuality(_req.gem_id, _req.gem_index, _req.upgradeItemId, _req.upgradeItemIndex)) 
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_UPGRADE_GEM_QUALITY);
					return;
				}
				pImp->_runner->gem_notify(_req.gem_id, S2C::GEM_UPGRADE_QUALITY_SUCC);
			}

		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//宝石萃取服务
class extract_gem_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int gem_index;		//宝石 包裹栏索引
		int gem_id;			//宝石ID
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->ExtraceGem(_req.gem_id, _req.gem_index)) 
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_EXTRACT_GEM);
					return;
				}
				pImp->_runner->gem_notify(_req.gem_id, S2C::GEM_EXTRACT_SUCC);
			}

		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//宝石熔炼服务
class smelt_gem_executor : public service_executor
{
#pragma pack(1)
	struct  player_request
	{
		int src_gem_index;		//宝石 包裹栏索引
		int src_gem_id;			//宝石ID
		int dest_gem_index;		//转移宝石包裹栏索引
		int dest_gem_id;		//转移宝石ID
		int smelt_item_id;	//熔炼道具ID
		int smelt_item_index; //熔炼道具包裹栏索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(player_request));
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_GEM_OPERATION;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				if(!pImp->SmeltGem(_req.src_gem_id, _req.src_gem_index, _req.dest_gem_id, _req.dest_gem_index, _req.smelt_item_id, _req.smelt_item_index)) 
				{
					pImp->_runner->error_message(S2C::ERR_FAILED_SMELT_GEM);
					return;
				}
				pImp->_runner->gem_notify(_req.src_gem_id, S2C::GEM_SMELT_SUCC);
			}

		};

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}	
};

//副本房间开启服务
class open_raid_room_provider : public feedback_provider
{
public:
#pragma pack(1)
	struct  player_request
	{
		int map_id;
		int raid_template_id;
		char can_vote;
		char difficulty;
		size_t roomname_len;
		char roomname[];	
	};
#pragma pack()
	int raid_template_id;

private:
	open_raid_room_provider * Clone()
	{
		ASSERT(!_is_init);
		return new open_raid_room_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(raid_template_id))
		{
			ASSERT(false);
			return false;
		}
		raid_template_id = *(int*)buf;
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size < 3 * sizeof(int) + sizeof(char) + sizeof(char)) return;
		open_raid_room_provider::player_request * req = (open_raid_room_provider::player_request*) buf;
		if(size != 3 * sizeof(int) + sizeof(char) + sizeof(char) +  req->roomname_len) return;	

		if(raid_template_id != req->raid_template_id)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
			return ;
		}
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
	}
};

class open_raid_room_executor : public service_executor
{
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size < 3 * sizeof(int) + sizeof(char) + sizeof(char)) return false;
		open_raid_room_provider::player_request * req = (open_raid_room_provider::player_request*) buf;
		if(size != 3 * sizeof(int) + sizeof(char) + sizeof(char) + req->roomname_len) return false;	
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size < 3 * sizeof(int) + sizeof(char) + sizeof(char)) return false;
		open_raid_room_provider::player_request * req = (open_raid_room_provider::player_request*) buf;
		if(size != 3 * sizeof(int) + sizeof(char) + sizeof(char) + req->roomname_len) return false;	

		pImp->OpenRaidRoom(req->map_id, req->raid_template_id, req->can_vote, req->difficulty, req->roomname, req->roomname_len);
		return true;
	}	
};

class join_raid_room_provider : public feedback_provider
{
public:
#pragma pack(1)
	struct  player_request
	{
		int map_id;
		int raid_template_id;
		int room_id;
		char raid_faction; //阵营：0 无阵营，1 攻，2 守，3 观察者
	};
#pragma pack()
	int raid_template_id;

private:
	join_raid_room_provider * Clone()
	{
		ASSERT(!_is_init);
		return new join_raid_room_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(raid_template_id))
		{
			ASSERT(false);
			return false;
		}
		raid_template_id = *(int*)buf;
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return;
		player_request * req = (player_request*) buf;

		if(raid_template_id != req->raid_template_id)
		{
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
			return ;
		}
		SendMessage(GM_MSG_SERVICE_DATA,player,_type,buf,size);
	}
};

class join_raid_room_executor : public service_executor
{
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(join_raid_room_provider::player_request)) return false;
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf , size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		ASSERT(size == sizeof(join_raid_room_provider::player_request));
		join_raid_room_provider::player_request * req = (join_raid_room_provider::player_request*) buf;
		pImp->JoinRaidRoom(req->map_id, req->raid_template_id, req->room_id, req->raid_faction);
		return true;
	}	
};

typedef feedback_provider change_name_provider;
class change_name_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int item_index;
		int item_id;
		int name_len;
		char name[0];
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(size <= sizeof(player_request)) return false;
		player_request* req = (player_request*)buf;
		if(req->name_len <= 0 || req->name_len > MAX_USERNAME_LENGTH) return false; 
		if(req->item_index < 0 || (size_t)req->item_index > pImp->GetInventory().Size()) return false;
		if(req->item_id <= 0) return false;
		if(size != sizeof(player_request) + req->name_len) return false;

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		
		if(size <= sizeof(player_request)) return false;
		player_request* req = (player_request*)buf;
		if(req->name_len <= 0 || req->name_len > MAX_USERNAME_LENGTH) return false; 
		if(size != sizeof(player_request) + req->name_len) return false;
		if(req->item_index < 0 || (size_t)req->item_index > pImp->GetInventory().Size()) return false;
		if(req->item_id <= 0) return false;

		item & it = pImp->GetInventory()[req->item_index];
		if(it.type != req->item_id) return false;
		if(req->item_id != g_config.item_change_name_id[0] && req->item_id != g_config.item_change_name_id[1] && req->item_id != g_config.item_change_name_id[2]) return false; 

		if(!GNET::SendChangeRoleName(pImp->_parent->ID.id, req->name_len, req->name, req->item_id, req->item_index, object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户%d执行了改名操作, item_id=%d, item_index=%d",pImp->_parent->ID.id, req->item_id, req->item_index);
		return true;
	}
};

// 法宝飞升
typedef feedback_provider talisman_holylevelup_provider;
class talisman_holylevelup_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int talisman_index;		// 法宝包裹栏索引
		int talisman_id;		// 法宝ID
		int levelup_id;			// 飞升灵媒ID
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_HOLYLEVELUP;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp* pImp = (gplayer_imp*)obj;
				bool bRst = pImp->TalismanHolyLevelup( _req.talisman_index, _req.talisman_id, _req.levelup_id);
				if( !bRst ) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_HOLYLEVELUP);
				}
			}
			
		};
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

// 法宝技能镶嵌
typedef feedback_provider talisman_embedskill_provider;
class talisman_embedskill_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int talisman1_index;	// 法宝包裹栏索引
		int talisman1_id;		// 法宝ID
		int talisman2_index;	// 法宝包裹栏索引
		int talisman2_id;		// 法宝ID
		int needitem1_id;		// 飞升灵媒1 ID
		int needitem1_index;	// 飞升灵媒1包裹栏索引
		int needitem2_id;		// 飞升灵媒2 ID
		int needitem2_index;	// 飞升灵媒2包裹栏索引
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_EMBEDSKILL;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp* pImp = (gplayer_imp*)obj;
				bool bRst = pImp->TalismanEmbedSkill( _req.talisman1_index, _req.talisman1_id, _req.talisman2_index, _req.talisman2_id, _req.needitem1_id, _req.needitem1_index, _req.needitem2_id, _req.needitem2_index);
				if( !bRst ) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_EMBEDSKILL);
				}
			}
			
		};
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

// 法宝技能洗练
typedef feedback_provider talisman_skillrefine_provider;
class talisman_skillrefine_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int talisman1_index;	// 法宝包裹栏索引
		int talisman1_id;		// 法宝ID
		int talisman2_index;	// 法宝包裹栏索引
		int talisman2_id;		// 法宝ID
		int needitem_id;		// 洗练灵媒 ID
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_SKILLREFINE;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp* pImp = (gplayer_imp*)obj;
				bool bRst = pImp->TalismanSkillRefine( _req.talisman1_index, _req.talisman1_id, _req.talisman2_index, _req.talisman2_id, _req.needitem_id );
				if( !bRst ) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_SKILLREFINE);
				}
			}
			
		};
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

// 法宝技能洗练结果
typedef feedback_provider talisman_skillrefine_result_provider;
class talisman_skillrefine_result_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int talisman1_index;	// 法宝包裹栏索引
		int talisman1_id;		// 法宝ID
		char result;			// 0: 保留；1：使用洗练后的属性
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_TALISMAN_SKILLREFINERESULT;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp* pImp = (gplayer_imp*)obj;
				bool bRst = pImp->TalismanSkillRefineResult( _req.talisman1_index, _req.talisman1_id, _req.result);
				if( !bRst ) 
				{
					pImp->_runner->error_message(S2C::ERR_TALISMAN_SKILLREFINE);
				}
			}
			
		};
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider change_faction_name_provider;
class change_faction_name_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int item_index;
		int item_id;
		char type;	//0- 帮派  1-家族
		int name_len;
		char name[0];
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(size <= sizeof(player_request)) return false;
		player_request* req = (player_request*)buf;
		if(req->name_len <= 0 || req->name_len > MAX_USERNAME_LENGTH) return false; 
		if(req->item_index < 0 || (size_t)req->item_index > pImp->GetInventory().Size()) return false;
		if(req->item_id <= 0) return false;
		if(size != sizeof(player_request) + req->name_len) return false;
		if(req->type != 0 && req->type != 1) return false;

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		
		if(size <= sizeof(player_request)) return false;
		player_request* req = (player_request*)buf;
		if(req->name_len <= 0 || req->name_len > MAX_USERNAME_LENGTH) return false; 
		if(size != sizeof(player_request) + req->name_len) return false;
		if(req->item_index < 0 || (size_t)req->item_index > pImp->GetInventory().Size()) return false;
		if(req->item_id <= 0) return false;

		item & it = pImp->GetInventory()[req->item_index];
		if(it.type != req->item_id) return false;

		if(req->type == 0 && req->item_id != g_config.item_change_faction_name_id[0] && req->item_id != g_config.item_change_faction_name_id[1] 
				&& req->item_id != g_config.item_change_faction_name_id[2]) return false; 
		if(req->type == 1 && req->item_id != g_config.item_change_family_name_id[0] && req->item_id != g_config.item_change_family_name_id[1] 
				&& req->item_id != g_config.item_change_family_name_id[2]) return false; 

		int fid = 0;
		if(req->type == 0) fid = pImp->OI_GetMafiaID();
		else if(req->type == 1) fid = pImp->OI_GetFamilyID();

		if(!GNET::SendChangeFactionName(pImp->_parent->ID.id, fid, req->name_len, req->name, req->type, req->item_id, req->item_index, object_interface(pImp)))
		{
			//发送错误
			pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
			return false;
		}
		GLog::log(GLOG_INFO,"用户%d执行了帮派家族改名操作, fid=%d, type=%d, item_id=%d, item_index=%d",pImp->_parent->ID.id, fid, req->type, req->item_id, req->item_index);
		return true;
	}
};

// --------------  激活上古传送服务 by sunjunbo  -----------------
class active_ui_transfer_provider : public general_provider
{
public:
#pragma pack(1)
	struct request
	{
		int index;			// 激活的保存点索引
	};
#pragma pack()
	virtual bool Save(archive & ar) { return true; }
	virtual bool Load(archive & ar) { return true; }
private:
	int index; // 激活的index

	virtual service_provider * Clone()
   	{
		ASSERT(!_is_init);
		return new active_ui_transfer_provider(*this);
	}

    virtual void GetContent(const XID & player,int cs_index,int sid) {}
    virtual void OnHeartbeat() {}
    virtual void OnControl(int param ,const void * buf, size_t size) {}	

	virtual bool OnInit(const void * buf, size_t size)
	{
		ASSERT(size == sizeof(int));
			
		index = *((int*)buf);
		return true;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		ASSERT (size == sizeof(request));
		request * pReq = (request*) buf;
		if (pReq->index != index) {
			SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
		}
		SendMessage(GM_MSG_SERVICE_DATA, player, _type, buf, size);
	}
};

class active_ui_transfer_executor : public service_executor
{
public:
	typedef active_ui_transfer_provider::request player_request;
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}
	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request * pReq = (player_request*) buf;
		__PRINTF("Active UI Transfer OnS [Enter]: %d\n", pReq->index);

		if (!pImp->ActiveUITransferIndex(pReq->index)) 
		{
			return false;	
		}

		__PRINTF("Active UI Transfer OnS [Sucess]: %d\n", pReq->index);
		return true;
	}
};

// --------------  使用上古传送服务 by sunjunbo  -----------------
class use_ui_transfer_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int index;			// 使用的保存点索引
	};
#pragma pack()
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request * pReq = (player_request*) buf;
		// 是否开启在OnServe中检查
		__PRINTF("Use UI Transfer SendR [Success]: %d\n", pReq->index);
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}
	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		player_request * pReq = (player_request*) buf;
		__PRINTF("Use UI Transfer OnS [Enter]: %d\n", pReq->index);

		if (!pImp->UseUITransferIndex(pReq->index)) return false;

		__PRINTF("Use UI Transfer OnS [Success]: %d\n", pReq->index);
		return true;
	}
};


typedef feedback_provider equipment_slot_provider;
class equipment_slot_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//物品 id
		int stone_index;		//道具 index
		int stone_id;			//道具 id
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_EQUIPMENT_SLOT;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->EquipmentSlot(_req.item_index, _req.item_id, _req.stone_index, _req.stone_id); 
				if(!bRst) 
				{
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};


typedef feedback_provider equipment_install_astrology_provider;
class equipment_install_astrology_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//物品 id
		int stone_index;		//星座index
		int stone_id;			//星座id
		int slot_index;
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_EQUIPMENT_INSTALL_ASTROLOGY;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->EquipmentInstallAstrology(_req.item_index, _req.item_id, _req.stone_index, _req.stone_id, _req.slot_index); 
				if(!bRst) 
				{
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider equipment_uninstall_astrology_provider;
class equipment_uninstall_astrology_executor : public service_executor
{
public:
#pragma pack(1)
	struct  player_request
	{
		int item_index;			//物品 包裹栏索引
		int item_id;			//物品 id
		int slot_index;
	};
#pragma pack()
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}

		class op : public session_general_operation::operation
		{
			player_request _req; 
		public:
			op(const player_request & req):_req(req)
			{}

			virtual int GetID()
			{
				return S2C::GOP_EQUIPMENT_UNINSTALL_ASTROLOGY;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(3);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				bool bRst = pImp->EquipmentUninstallAstrology(_req.item_index, _req.item_id, _req.slot_index); 
				if(!bRst) 
				{
				}
			}
			
		};
		
		player_request * req = (player_request*) buf;
		session_general_operation * pSession = new session_general_operation(pImp,  new op(*req));
		pImp->AddStartSession(pSession);
		return true;
	}
};


//声望商店
class reputation_shop_provider : public general_provider
{
	struct reputation_data
	{
		struct
		{
			unsigned int    repu_id_req;        //所需声望id
			unsigned int    repu_req_value;     //物品购买所需达到的声望值
			unsigned int    repu_id_consume;    //购买物品消耗的声望值id
			unsigned int    repu_consume_value; //购买物品需要消耗多少声望值
		} repu_required_pre[3];
	};

	std::vector<const item_data *>		_item_list;				//物品的数据列表
	std::vector<reputation_data>		_reputation_data_list;  //物品所用的声望列表


public:
#pragma pack(1)
	//客户端发过来的
	struct request
	{
		struct bag
		{
			int item_type;
			int index;
			int count;
		};

		int count;
		bag item_list[];
	};

	//内部使用的
	struct request_convert
	{
		struct bag
		{
			int item_type;
			int count;
			struct
			{
				unsigned int    repu_id_req;        
				unsigned int    repu_req_value;     
				unsigned int    repu_id_consume;
				unsigned int    repu_consume_value;
			} repu_required_pre[3];
		};

		int count;
		bag item_list[];
	};
#pragma pack()

	reputation_shop_provider() {}

	virtual ~reputation_shop_provider()
	{
		_item_list.clear();
		_reputation_data_list.clear();
	}


private:
	virtual reputation_shop_provider * Clone()
	{
		ASSERT(!_is_init);
		return new reputation_shop_provider(*this);
	}

	virtual bool OnInit(const void * buf, size_t size)
	{
		if(size != sizeof(int)) 
		{
			ASSERT(false);
			return false;
		}
		int reputation_shop_id = *(int*)buf;
		if(reputation_shop_id <= 0) return false;

		reputation_shop_template * pRs = reputation_shop_manager::GetReputationShopTemplate(reputation_shop_id); 
		reputation_shop_goods_t * list = pRs->reputation_shop_sell_goods; 
		size_t count = sizeof(pRs->reputation_shop_sell_goods)/sizeof(reputation_shop_goods_t); 

		if(list == NULL) return false;

		_item_list.clear();
		_item_list.reserve(count);
		for(size_t i = 0; i < count; i++)
		{
			if(list[i].id_goods == 0)
			{
				reputation_data tmpdata;
				memset(&tmpdata, 0, sizeof(tmpdata));
				_item_list.push_back(NULL);
				_reputation_data_list.push_back(tmpdata);
				continue;
			}

			const void * pBuf = gmatrix::GetDataMan().get_item_for_sell(list[i].id_goods);
			if(pBuf)
			{
				const item_data * data = (const item_data *)pBuf;
				_item_list.push_back(data);

				reputation_data tmpdata;
				for(int k = 0; k < 3; ++k)
				{
					tmpdata.repu_required_pre[k].repu_id_req = list[i].repu_required_pre[k].repu_id_req;
					tmpdata.repu_required_pre[k].repu_req_value = list[i].repu_required_pre[k].repu_req_value;
					tmpdata.repu_required_pre[k].repu_id_consume = list[i].repu_required_pre[k].repu_id_consume;
					tmpdata.repu_required_pre[k].repu_consume_value = list[i].repu_required_pre[k].repu_consume_value;
				}
				_reputation_data_list.push_back(tmpdata);
			}
			else
			{
				reputation_data tmpdata;
				memset(&tmpdata, 0, sizeof(tmpdata));
				_item_list.push_back(NULL);
				_reputation_data_list.push_back(tmpdata);
			}
		}

		return true;
	}

	virtual void GetContent(const XID & player,int cs_index, int sid)
	{
		//可能需要返回税率
		//SendServiceContent(player.id, cs_idnex, sid, _left_list.begin(),_left_list.size() * sizeof(int));
		return ;
	}

	virtual void TryServe(const XID & player, const void * buf, size_t size)
	{
		request * param = (request *)buf;
		if(size != sizeof(request) + param->count*sizeof(request::bag)) return;

		packet_wrapper h1;
		h1 << param->count;
		for(int i = 0; i < param->count; ++i)
		{
			int index = param->item_list[i].index;
			if(index >= (int)_item_list.size() || index < 0)
			{
				//客户端传来的错误的数据
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}

			const item_data * pItem = _item_list[index];
			if(!pItem)
			{
				//没有找到正确的数据
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}
			ASSERT(pItem->item_content == ((const char * )pItem) + sizeof(item_data));
			size_t count = param->item_list[i].count;
			if(!count || count > pItem->pile_limit || param->item_list[i].item_type != pItem->type)
			{
				SendMessage(GM_MSG_ERROR_MESSAGE,player,S2C::ERR_SERVICE_ERR_REQUEST,NULL,0);
				return ;
			}
		
			//检查通过
			request_convert::bag tmpbag;
			tmpbag.item_type	= param->item_list[i].item_type; 
			tmpbag.count		= param->item_list[i].count;
			for(int k = 0; k < 3; ++k)
			{
				tmpbag.repu_required_pre[k].repu_id_req			= _reputation_data_list[index].repu_required_pre[k].repu_id_req;
				tmpbag.repu_required_pre[k].repu_req_value		= _reputation_data_list[index].repu_required_pre[k].repu_req_value;
				tmpbag.repu_required_pre[k].repu_id_consume		= _reputation_data_list[index].repu_required_pre[k].repu_id_consume;
				tmpbag.repu_required_pre[k].repu_consume_value  = _reputation_data_list[index].repu_required_pre[k].repu_consume_value;
			}
			MakeForm(h1, tmpbag);
		}

		SendMessage(GM_MSG_SERVICE_DATA,player,_type,h1.data(),h1.size());
	}

	void MakeForm(packet_wrapper & h1, const request_convert::bag & bag)
	{
		h1 << bag.item_type << bag.count;
		for(int i = 0; i < 3; ++i)
		{
			h1 << bag.repu_required_pre[i].repu_id_req;
			h1 << bag.repu_required_pre[i].repu_req_value;
			h1 << bag.repu_required_pre[i].repu_id_consume;
			h1 << bag.repu_required_pre[i].repu_consume_value;
		}
	}

	virtual void OnHeartbeat()
	{
	}
	
	virtual void OnControl(int param ,const void * buf, size_t size)
	{
		//主要是控制税率
	}

	virtual bool Save(archive & ar)
	{
		return true;
	}
	virtual bool Load(archive & ar) 
	{
		return true;
	}
};

class reputation_shop_executor : public service_executor
{
public:
	typedef reputation_shop_provider::request_convert player_request;

private:

	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		player_request * req = (player_request *)buf;
		if(req->count <= 0 || 
		  size != sizeof(player_request) + req->count*sizeof(reputation_shop_provider::request::bag))
		{
			return false;
		}

		if(!pImp->InventoryHasSlot(req->count))
		{
			return false;
		}

		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		player_request * req = (player_request *)buf;
		if(req->count <= 0 || 
		  size != (sizeof(player_request) + req->count*sizeof(player_request::bag)))
		{
			ASSERT(false);
			return false;
		}

		if(!pImp->InventoryHasSlot(req->count))
		{
			pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}

		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}

		for(int i = 0; i < req->count; ++i)
		{
			player_request::bag & req_bag = req->item_list[i];
			gplayer_imp::reputation_item_info info;
			info.item_type		= req_bag.item_type;
			info.count			= req_bag.count;
			for(int j = 0; j < 3; j++)
			{
				info.repu_required_pre[j].repu_id_req = req_bag.repu_required_pre[j].repu_id_req;
				info.repu_required_pre[j].repu_req_value = req_bag.repu_required_pre[j].repu_req_value;
				info.repu_required_pre[j].repu_id_consume = req_bag.repu_required_pre[j].repu_id_consume;
				info.repu_required_pre[j].repu_consume_value = req_bag.repu_required_pre[j].repu_consume_value;
			}

			//挨个处理
			if(!pImp->PurchaseRegionReputationItem(info))
			{
				return false;
			}
		}

		return true;
	}
};

typedef feedback_provider crossserver_battle_sign_up_provider;
class crossserver_battle_sign_up_executor : public service_executor
{
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != 0) return false;
		
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != 0) return false;
		if(pImp->IsCombatState()) return false;
		if(pImp->GetRebornCount() == 0 && pImp->GetObjectLevel() < 90) 
		{
			pImp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
			return false;
		}

		pImp->PlayerTryChangeDS(5);
		return true;
	}
};

typedef feedback_provider crossserver_battle_out_provider;
class crossserver_battle_out_executor : public service_executor
{
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != 0) return false;
		
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != 0) return false;
		if(pImp->IsCombatState()) return false;

		pImp->PlayerTryChangeDS(2);
		return true;
	}
};

typedef feedback_provider kingdom_enter_provider;
class kingdom_enter_executor : public service_executor
{
public:
	struct player_request
	{
		char field_type;	//1-主战场 2-辅战场 3-澡堂
	};
private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		if(size != sizeof(player_request)) return false;
		if(pImp->IsCombatState()) return false;
		if(pImp->_cur_session || pImp->_session_list.size())
		{
			return false;
		}

		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		if(size != sizeof(player_request))
		{
			ASSERT(false);
			return false;
		}
		player_request * req = (player_request*) buf;

		if(req->field_type == 1 || req->field_type == 2)
		{
			//发送请求数据
			if(!GNET::SendKingdomEnter(pImp->_parent->ID.id, req->field_type)) 
			{
				//发送错误
				pImp->_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
				return false;
			}
			GLog::log(GLOG_INFO,"用户 %d 执行了进入国王战操作, mafia_id=%d, type=%d",  pImp->_parent->ID.id, pImp->OI_GetMafiaID(),  req->field_type);
		}
		else if(req->field_type == 3)
		{
			pImp->PlayerEnterBath();
		}
		return true;
	}
};

typedef feedback_provider produce_jinfashen_provider;
class produce_jinfashen_exector : public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		char product_type; //0:金身 1:法身
		char product_idx;
		int zhaohua[3]; //0:仙 1:佛 2:魔
		short money_item_cnt;	//钱物品的数量
		short material_cnt;
		struct
		{
			char config_idx;
			char type;
			int item_id;
			short item_idx;
		} material[];
	};
#pragma pack()

	struct valid_material
	{
		int item_id;
		int item_idx;
	};
	
	struct money_item
	{
		int item_idx;
		int num;
	};

private:
	
	virtual bool SendRequest(gplayer_imp * pImp, const XID & provider,const void * buf, size_t size)
	{
		size_t preSize = sizeof(char) + sizeof(char) + sizeof(int) * 3 + sizeof(short) + sizeof(short);
		if(size < preSize) return false;
		short maCnt = *(short*)((char*)buf + (preSize - sizeof(short)));
		if(maCnt < 0 || size != preSize + maCnt * (sizeof(char) + sizeof(char) + sizeof(int) + sizeof(short))) return false;
		
		//发送转发数据
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true;
	}

	static void DoProduce(gplayer_imp* pImp, raw_wrapper& ar)
	{
		if(!pImp->InventoryHasSlot(1)) 
		{
			pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return;
		}
		player_request * req = (player_request*)ar.data();
		if(req->product_idx < 0 || req->product_idx > 12) 
		{
			return;
		}
		DATA_TYPE dt;
		JINFASHEN_TO_MONEY_CONFIG * cfgData = (JINFASHEN_TO_MONEY_CONFIG*)gmatrix::GetDataMan().get_data_ptr(g_config.id_produce_jinfashen, ID_SPACE_CONFIG,dt); 
		if(cfgData == NULL || dt != DT_JINFASHEN_TO_MONEY_CONFIG) 
		{
			return;
		}
		JINFASHEN_TO_MONEY_CONFIG::Config* cfgList = 0;
		if(req->product_type == 0)
		{
			cfgList = cfgData->jinshen;
		}
		else if(req->product_type == 1)
		{
			cfgList = cfgData->fashen;
		}
		else
		{
			return;
		}
		JINFASHEN_TO_MONEY_CONFIG::Config& c = cfgList[req->product_idx];
		if(c.id < 0 || c.money < 0 || c.reputation < 0)
		{
			return;
		}

		int maRepu = 0;
		int maMoney = 0;
		item_list & inv = pImp->GetInventory();
		abase::vector<valid_material> valid_material_list;
		for(short i = 0; i < req->material_cnt; i ++)
		{
			JINFASHEN_TO_MONEY_CONFIG::Config* maCfgList = 0;
			if(req->material[i].type == 0)
			{
				maCfgList = cfgData->jinshen;
			}
			else if(req->material[i].type == 1)
			{
				maCfgList = cfgData->fashen;
			}
			else
			{
				continue;
			}

			size_t cfgIdx = req->material[i].config_idx;
			if(cfgIdx < 0 || cfgIdx >= sizeof(cfgData->jinshen) / sizeof(cfgData->jinshen[0]))
			{
				continue;
			}

			int item_id = req->material[i].item_id;
			int item_idx = req->material[i].item_idx;
			if(item_id <= 0 || item_idx < 0)
			{
				continue;
			}
			item& it = inv[item_idx];
			if(it.type != item_id)
			{
				continue;
			}
			if(item_id != maCfgList[cfgIdx].id)
			{
				continue;
			}
			if(it.expire_date > 0)
			{
				continue;
			}
			maRepu += maCfgList[cfgIdx].reputation;
			maMoney += maCfgList[cfgIdx].money;

			valid_material vm;
			vm.item_id = item_id;
			vm.item_idx = item_idx;
			valid_material_list.push_back(vm);
		}	

		if(maRepu > c.reputation)
		{
			return;
		}

		size_t num = req->money_item_cnt <= 0 ? 0 : req->money_item_cnt;
		abase::vector<money_item> money_item_list;
		if(num > 0)
		{
			for(size_t i = 0; i < inv.Size(); i ++)
			{
				item& it = inv[i];
				if(num <= 0)
				{
					break;
				}
				if(it.type == 19681 && it.count > 0)
				{
					money_item mi;
					mi.item_idx = i;
					mi.num = it.count < num ? it.count : num;
					num -= mi.num;
					maMoney += (mi.num * 5000);
					money_item_list.push_back(mi);
				}
			}
		}

		int idx[3] =  {REPU_XIAN, REPU_FO, REPU_MO};
		for(int i = 0; i < 3; i ++)
		{
			if(req->zhaohua[i] > 0 && req->zhaohua[i] > pImp->GetRegionReputation(idx[i]))
			{
				pImp->_runner->error_message(S2C::ERR_LACK_OF_REPUTATION);
				return;
			}
		}

		if(maRepu + req->zhaohua[0] + req->zhaohua[1] + req->zhaohua[2] < c.reputation)
		{
			pImp->_runner->error_message(S2C::ERR_LACK_OF_REPUTATION);
			return;
		}

		int64_t money_need = (c.money - maMoney) * 10000;
		if(money_need > MONEY_CAPACITY_BASE || (money_need > 0 && (size_t)money_need > pImp->GetMoney()))
		{
			pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return;	
		}

		pImp->DeliverItem(c.id, 1, true, 0, ITEM_INIT_TYPE_TASK);
		for(size_t i = 0; i < valid_material_list.size(); i ++)
		{
			valid_material& vm = valid_material_list[i];
			pImp->UseItemLog(inv, vm.item_idx,1);
			inv.DecAmount(vm.item_idx, 1);
			pImp->_runner->use_item(gplayer_imp::IL_INVENTORY, vm.item_idx, vm.item_id, 1);	
		}

		for(size_t i = 0; i < money_item_list.size(); i ++)
		{
			money_item& mi = money_item_list[i];
			pImp->UseItemLog(inv, mi.item_idx, mi.num);
			inv.DecAmount(mi.item_idx, mi.num);
			pImp->_runner->use_item(gplayer_imp::IL_INVENTORY, mi.item_idx, 19681, mi.num);	
		}

		if(money_need > 0)
		{

			pImp->SpendMoney((size_t)money_need);
			GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%lld:type=2:reason=15:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),money_need,c.id);
			pImp->_runner->spend_money((size_t)money_need);
		}

		for(int i = 0; i < 3; i ++)
		{
			if(req->zhaohua[i] > 0)
			{
				pImp->ModifyRegionReputation(idx[i], -req->zhaohua[i]);
			}
		}
		GLog::log(GLOG_INFO,"玩家%d进行了金身法身快速制造, 获得%d, 抵扣金钱%d, 抵扣造化%d, 消耗玩家身上金钱%lld, 消耗玩家造化仙=%d, 佛=%d, 魔=%d", pImp->_parent->ID.id, c.id, maMoney, maRepu, money_need, req->zhaohua[0], req->zhaohua[1], req->zhaohua[2]);
	}


	virtual bool OnServe(gplayer_imp *pImp,const XID & provider, const A3DVECTOR & pos,const void * buf, size_t size)
	{
		size_t preSize = sizeof(char) + sizeof(char) + sizeof(int) * 3 + sizeof(short) + sizeof(short);
		if(size < preSize) return false;
		short maCnt = *(short*)((char*)buf + (preSize - sizeof(short)));
		if(maCnt < 0 || size != preSize + maCnt * (sizeof(char) + sizeof(char) + sizeof(int) + sizeof(short))) return false;


		class op : public session_general_operation::operation
		{
			raw_wrapper ar;
		public:
			op(const void* buf, size_t size): ar(buf, size)
			{}

			virtual int GetID()
			{
				return S2C::GOP_EQUIPMENT_UNINSTALL_ASTROLOGY;
			}
			virtual bool NeedBoardcast()
			{
				return true;
			}
			virtual int GetDuration()
			{
				return SECOND_TO_TICK(2);
			}
			virtual void OnStart(gactive_imp * pImp)
			{
				//do nothing
			}
			virtual void OnEnd(gactive_imp * obj)
			{
				gplayer_imp * pImp = (gplayer_imp *) obj;
				produce_jinfashen_exector::DoProduce(pImp, ar);
			}
		};
		session_general_operation * pSession = new session_general_operation(pImp,  new op(buf, size));
		pImp->AddStartSession(pSession);
		return true;
	}
};

typedef feedback_provider pet_reborn_provider;
class pet_reborn_executor: public service_executor
{
public:
#pragma pack(1)
	struct player_request
	{
		int pet_index;	//物品 包裹栏索引
		int assist_index;
	};
#pragma pack()
private:
	virtual bool SendRequest(gplayer_imp* pImp,const XID& provider,const void* buf,size_t size)
	{
		if(sizeof(player_request) != size) return false;
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		pImp->SendTo<0>(GM_MSG_SERVICE_REQUEST,provider,_type,buf,size);
		return true; 
	}

	virtual bool OnServe(gplayer_imp *pImp,const XID& provider,const A3DVECTOR& pos,const void* buf,size_t size)
	{
		ASSERT(sizeof(player_request) == size);
		if(pImp->OI_TestSafeLock())
		{
			pImp->_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return true;
		}
		player_request* req = (player_request*)buf;
		pImp->PlayerRebornPet(req->pet_index,req->assist_index);
		return true;
	}
};


}
using namespace NG_ELEMNET_SERVICE;

static service_inserter si1 = SERVICE_INSERTER(vendor_provider,vendor_executor,1);
static service_inserter si2 = SERVICE_INSERTER(purchase_provider,purchase_executor,2);
static service_inserter si4 = SERVICE_INSERTER(heal_provider,heal_executor,4);
static service_inserter si5 = SERVICE_INSERTER(plane_switch_provider, plane_switch_executor,5);
static service_inserter si6 = SERVICE_INSERTER(task_in_provider,task_in_executor,6);
static service_inserter si7 = SERVICE_INSERTER(task_out_provider,task_out_executor,7);
static service_inserter si8 = SERVICE_INSERTER(task_matter_provider,task_matter_executor,8);
static service_inserter si10 = SERVICE_INSERTER(install_provider,install_executor,10);
static service_inserter si11 = SERVICE_INSERTER(uninstall_provider,uninstall_executor,11);
static service_inserter si12 = SERVICE_INSERTER(produce_provider,produce_executor,12);
static service_inserter si13 = SERVICE_INSERTER(reset_pkvalue_provider,reset_pkvalue_executor,13);
static service_inserter si14 = SERVICE_INSERTER(trashbox_passwd_provider,trashbox_passwd_executor,14);
static service_inserter si15 = SERVICE_INSERTER(trashbox_open_provider,trashbox_open_executor,15); 
static service_inserter si16 = SERVICE_INSERTER(trashbox_open_provider,mafia_trashbox_open_executor,16); 
static service_inserter si18 = SERVICE_INSERTER(faction_service_provider, faction_service_executor,18);
static service_inserter si19 = SERVICE_INSERTER(player_market_provider, player_market_executor,19);
static service_inserter si21 = SERVICE_INSERTER(player_market_provider, player_market_executor2,21);
static service_inserter si25 = SERVICE_INSERTER(mail_service_provider,mail_service_executor,25);
static service_inserter si26 = SERVICE_INSERTER(auction_service_provider,auction_service_executor,26);
static service_inserter si27 = SERVICE_INSERTER(double_exp_provider,double_exp_executor,27);
//static service_inserter si30 = SERVICE_INSERTER(battle_service_provider,battle_service_executor,30);
//static service_inserter si31 = SERVICE_INSERTER(towerbuild_provider,towerbuild_executor,31);
//static service_inserter si32 = SERVICE_INSERTER(battle_leave_provider,battle_leave_executor,32);
static service_inserter si33 = SERVICE_INSERTER(resetprop_provider,resetprop_executor,33);
static service_inserter si34 = SERVICE_INSERTER(spec_trade_provider,spec_trade_executor,34);
static service_inserter si35 = SERVICE_INSERTER(bind_item_provider,bind_item_executor,35);
static service_inserter si36 = SERVICE_INSERTER(destroy_bind_item_provider,destroy_bind_item_executor,36);
static service_inserter si37 = SERVICE_INSERTER(destroy_item_restore_provider,destroy_item_restore_executor,37);
static service_inserter si38 = SERVICE_INSERTER( stock_service_provider,stock_service_executor1,38);
static service_inserter si39 = SERVICE_INSERTER( stock_service_provider,stock_service_executor2,39);
static service_inserter si40 = SERVICE_INSERTER( talisman_refine_provider,talisman_refine_executor ,40);
static service_inserter si41 = SERVICE_INSERTER( talisman_lvlup_provider,talisman_lvlup_executor ,41);
static service_inserter si42 = SERVICE_INSERTER( talisman_reset_provider, talisman_reset_executor ,42);
static service_inserter si43 = SERVICE_INSERTER( talisman_combine_provider, talisman_combine_executor ,43);
static service_inserter si44 = SERVICE_INSERTER( talisman_enchant_provider, talisman_enchant_executor ,44);
//城战相关
static service_inserter si45 = SERVICE_INSERTER(battle_field_challenge_service_provider,battle_field_challenge_service_executor,45);
static service_inserter si46 = SERVICE_INSERTER(battle_field_construct_service_provider,battle_field_construct_service_executor,46);
static service_inserter si47 = SERVICE_INSERTER(battle_field_employ_service_provider,battle_field_employ_service_executor,47);
//宠物相关
static service_inserter si48 = SERVICE_INSERTER(pet_adopt_provider,pet_adopt_executor,48);
static service_inserter si49 = SERVICE_INSERTER(pet_free_provider,pet_free_executor,49);
static service_inserter si50 = SERVICE_INSERTER(pet_combine_provider,pet_combine_executor,50);
static service_inserter si51 = SERVICE_INSERTER(pet_rename_provider,pet_rename_executor,51);
static service_inserter si52 = SERVICE_INSERTER(mount_item_renew_provider,mount_item_renew_executor,52);
static service_inserter si53 = SERVICE_INSERTER(item_trade_provider, item_trade_executor,53);
static service_inserter si54 = SERVICE_INSERTER(feedback_provider, lock_item_executor,54);
static service_inserter si55 = SERVICE_INSERTER(feedback_provider, unlock_item_executor,55);
static service_inserter si56 = SERVICE_INSERTER(feedback_provider, broken_item_restore_executor,56);
static service_inserter si57 = SERVICE_INSERTER(feedback_provider, blood_enchant_executor,57);
static service_inserter si58 = SERVICE_INSERTER(feedback_provider, spirit_insert_executor,58);
static service_inserter si59 = SERVICE_INSERTER(feedback_provider, spirit_remove_executor,59);
static service_inserter si60 = SERVICE_INSERTER(feedback_provider, spirit_power_restore_executor,60);
static service_inserter si61 = SERVICE_INSERTER(feedback_provider, spirit_decompose_executor,61);
static service_inserter si62 = SERVICE_INSERTER(npc_produce_provider, npc_produce_executor,62);
static service_inserter si63 = SERVICE_INSERTER(petequip_refine_provider, petequip_refine_executor,63); 
static service_inserter si64 = SERVICE_INSERTER(change_style_provider, change_style_executor,64); 
static service_inserter si65 = SERVICE_INSERTER(magic_refine_provider, magic_refine_executor,65); 
static service_inserter si66 = SERVICE_INSERTER(magic_restore_provider, magic_restore_executor,66); 
static service_inserter si67 = SERVICE_INSERTER(arena_challenge_provider, arena_challenge_executor,67); 
static service_inserter si68 = SERVICE_INSERTER(territory_challenge_provider, territory_challenge_executor,68); 
static service_inserter si69 = SERVICE_INSERTER(territory_enter_provider, territory_enter_executor,69); 
static service_inserter si70 = SERVICE_INSERTER(territory_reward_provider, territory_reward_executor,70); 
static service_inserter si71 = SERVICE_INSERTER(charge_telestation_provider, charge_talestation_executor,71); 
static service_inserter si72 = SERVICE_INSERTER(repair_damage_item_provider, repair_damage_item_executor,72); 
static service_inserter si73 = SERVICE_INSERTER(equipment_upgrade_provider, equipment_upgrade_executor,73); 
static service_inserter si74 = SERVICE_INSERTER(consign_provider, consign_executor,74); 
static service_inserter si75 = SERVICE_INSERTER(crossserver_in_provider, crossserver_in_executor,75); 
static service_inserter si76 = SERVICE_INSERTER(crossserver_out_provider, crossserver_out_executor,76);

//宝石镶嵌相关服务
static service_inserter si77 = SERVICE_INSERTER(feedback_provider, identify_gem_slots_executor,77);
static service_inserter si78 = SERVICE_INSERTER(feedback_provider, rebuild_gem_slots_executor,78);
static service_inserter si79 = SERVICE_INSERTER(feedback_provider, customize_gem_slots_executor,79);
static service_inserter si80 = SERVICE_INSERTER(feedback_provider, embed_gems_executor,80);
static service_inserter si81 = SERVICE_INSERTER(feedback_provider, remove_gems_executor,81);
static service_inserter si82 = SERVICE_INSERTER(feedback_provider, upgrade_gem_level_executor,82);
static service_inserter si83 = SERVICE_INSERTER(feedback_provider, upgrade_gem_quality_executor,83);
static service_inserter si84 = SERVICE_INSERTER(feedback_provider, extract_gem_executor,84);
static service_inserter si85 = SERVICE_INSERTER(feedback_provider, smelt_gem_executor,85);

//副本相关服务
static service_inserter si86 = SERVICE_INSERTER(open_raid_room_provider, open_raid_room_executor,86);
static service_inserter si87 = SERVICE_INSERTER(join_raid_room_provider, join_raid_room_executor,87);
static service_inserter si88 = SERVICE_INSERTER(pet_combine2_provider,pet_combine2_executor,88);
//玩家改名
static service_inserter si89 = SERVICE_INSERTER(change_name_provider,change_name_executor,89);
//家族帮派改名
static service_inserter si90 = SERVICE_INSERTER(change_faction_name_provider,change_faction_name_executor,90);

//法宝飞升服务
static service_inserter si91 = SERVICE_INSERTER(talisman_holylevelup_provider,talisman_holylevelup_executor,91);
static service_inserter si92 = SERVICE_INSERTER(talisman_embedskill_provider,talisman_embedskill_executor,92);
static service_inserter si93 = SERVICE_INSERTER(talisman_skillrefine_provider,talisman_skillrefine_executor,93);
static service_inserter si94 = SERVICE_INSERTER(talisman_skillrefine_result_provider,talisman_skillrefine_result_executor,94);
//声望商店
static service_inserter si95 = SERVICE_INSERTER(reputation_shop_provider,reputation_shop_executor,95);

//升级成封神装备
//static service_inserter si96 = SERVICE_INSERTER(equipment_upgrade2_provider, equipment_upgrade2_executor,96); 
//装备打孔服务
static service_inserter si97 = SERVICE_INSERTER(equipment_slot_provider, equipment_slot_executor,97); 
//装备镶嵌星座
static service_inserter si98 = SERVICE_INSERTER(equipment_install_astrology_provider, equipment_install_astrology_executor,98); 
//装备卸载星座
static service_inserter si99 = SERVICE_INSERTER(equipment_uninstall_astrology_provider, equipment_uninstall_astrology_executor,99); 

//星座改用协议实现
//星座鉴定服务
//static service_inserter si100 = SERVICE_INSERTER(astrology_identify_provider, astrology_identify_executor,100); 
//星座升级服务
//static service_inserter si101 = SERVICE_INSERTER(astrology_upgrade_provider, astrology_upgrade_executor,101); 
//星座粉碎服务
//static service_inserter si102 = SERVICE_INSERTER(astrology_destroy_provider, astrology_destroy_executor,102); 


//激活UI传送服务
static service_inserter si103 = SERVICE_INSERTER(active_ui_transfer_provider, active_ui_transfer_executor,103); 
//使用UI传送服务
static service_inserter si104 = SERVICE_INSERTER(feedback_provider, use_ui_transfer_executor, 104); 

//报名跨服战场的传送服务
static service_inserter si105 = SERVICE_INSERTER(crossserver_battle_sign_up_provider, crossserver_battle_sign_up_executor,105); 
static service_inserter si106 = SERVICE_INSERTER(crossserver_battle_out_provider, crossserver_battle_out_executor,106); 
static service_inserter si107 = SERVICE_INSERTER(kingdom_enter_provider, kingdom_enter_executor,107); 
static service_inserter si108 = SERVICE_INSERTER(produce_jinfashen_provider, produce_jinfashen_exector,108); 
static service_inserter si109 = SERVICE_INSERTER(pet_reborn_provider,pet_reborn_executor,109);

