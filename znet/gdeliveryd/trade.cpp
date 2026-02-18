#include "trade.h"
#include "../common/log.h"
#include <algorithm>
#include <set>
#include "tradediscard_re.hpp"
#include "gdeliveryserver.hpp"
#include "glog.h"
#include "localmacro.h"
namespace GNET
{
static Trader::Options _Init[]={Trader::OPT_ATTACHTRADER};
Transaction::State _TState_Init(300,_Init,sizeof(_Init)/sizeof(Trader::Options));

static Trader::Options _Init_half[]={Trader::OPT_ATTACHTRADER};
Transaction::State _TState_Init_half(120,_Init_half,sizeof(_Init_half)/sizeof(Trader::Options));

static Trader::Options _SetPossession[]={Trader::OPT_SETPSSN};
Transaction::State _TState_SetPossession(60,_SetPossession,sizeof(_SetPossession)/sizeof(Trader::Options));

static Trader::Options _Exchange[]={Trader::OPT_UPDATEGOODS,Trader::OPT_SUBMIT,Trader::OPT_DISCARD,Trader::OPT_MOVEOBJ};
Transaction::State _TState_Exchange(600,_Exchange,sizeof(_Exchange)/sizeof(Trader::Options));

static Trader::Options _Ready_half[]={Trader::OPT_UPDATEGOODS,Trader::OPT_SUBMIT,Trader::OPT_DISCARD,Trader::OPT_MOVEOBJ};
Transaction::State _TState_Ready_half(120,_Ready_half,sizeof(_Ready_half)/sizeof(Trader::Options));

static Trader::Options _Ready[]={Trader::OPT_CONFIRM,Trader::OPT_DISCARD};
Transaction::State _TState_Ready(60,_Ready,sizeof(_Ready)/sizeof(Trader::Options));

static Trader::Options _Confirm_half[]={Trader::OPT_CONFIRM,Trader::OPT_DISCARD};
Transaction::State _TState_Confirm_half(60,_Confirm_half,sizeof(_Confirm_half)/sizeof(Trader::Options));

Transaction::State _TState_Confirm(-1);
Transaction::State _TState_Null(0);
Thread::Mutex Transaction::locker_map("Transaction::locker_map");

	void Trader::SetPossession(GRolePocket& pocket)
	{
		possession.swap(pocket.items.GetVector());
		pssn_money = pocket.money;
		capacity = pocket.capacity;
	}
	bool Trader::VerifyGoods(GRoleInventory& goods,TradeItems& goods_list)
	{
		TradeItems::iterator it = Find(goods_list,goods);
		if (it == goods_list.end()) return false;
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		return it->count>=goods.count && goods.count>=0 && (it->proctype&MASK_ITEM_NOTRADE)==0 && (!is_central || it->proctype&ITEM_PROC_TYPE_CD_TRADE);
	}
	bool Trader::AddExchgObject(GRoleInventory& goods,unsigned int money)
	{
		if (! StatePolicy(OPT_UPDATEGOODS)) return false;
		if( exchg_obj.size()>=TRADE_ITEM_MAX)
			return false;
		if (goods.id && goods.count>0)
		{
			if (! VerifyGoods(goods,possession) ) return false;
			//add to exchg_obj
			TradeItems::iterator it = Find(exchg_obj,goods);
			TradeItems::iterator it2 = Find(possession,goods);
			if (it==exchg_obj.end()) 
			{
				int tmp=goods.count;
				goods = *it2;
				goods.count = tmp;
				exchg_obj.push_back(goods);
			}
			else
				it->count += goods.count;
			//remove from possession
			it2->count -= goods.count;
		}
		if (money!=0) 
		{
			if (pssn_money < money) return false;
			exchg_money += money;
			pssn_money -= money;	
		}
		ChangeState(Trader::EXCHANGE);
		if(goods.client_size)
			goods.data.resize(goods.client_size);
		return true;
	}
	bool Trader::RemoveExchgObject(GRoleInventory& goods,unsigned int money)
	{
		if (! StatePolicy(OPT_UPDATEGOODS)) return false;
		if (goods.id && goods.count>0)
		{
			if (! VerifyGoods(goods,exchg_obj) ) return false;
			//remove from exchg_obj
			TradeItems::iterator it = Find(exchg_obj,goods);
			if ((it->count -= goods.count) == 0) 
				exchg_obj.erase(it);
			
			//add to possession
			it = Find(possession,goods);
			if (it == possession.end()) 
				return false;
			it->count += goods.count;
		}
		if (money != 0)
		{
			if (exchg_money < money ) return false;
			exchg_money -= money;
			pssn_money += money;
		}
		ChangeState(Trader::EXCHANGE);
		return true;
	}
	bool Trader::MoveObject(GRoleInventory& goods,int toPos)
	{
		if (! StatePolicy(OPT_MOVEOBJ)) return false;
		if ( goods.pos==toPos || goods.count==0 || toPos>=(int)capacity || toPos<0) return false;
		if (!VerifyGoods(goods,possession)) return false;
		//whether goods is in exchange container
		for (size_t i=0;i<exchg_obj.size();i++)
			if (exchg_obj[i].pos==goods.pos || exchg_obj[i].pos==toPos) return false;
		//find toPos Goods
		GRoleInventory* origin_goods=NULL;
		for (size_t i=0;i<possession.size();i++)
			if (possession[i].pos==toPos) {origin_goods=&possession[i]; break; }
		if (origin_goods==NULL)
		{
			//move directly
			TradeItems::iterator it = Find(possession,goods);
			if (goods.count == (*it).count)  //move totallly
				(*it).pos=toPos;
			else
			{
				(*it).count -= goods.count;
				GRoleInventory new_goods((*it));
				new_goods.pos = toPos;
				new_goods.count = goods.count;
				possession.push_back(new_goods);
			}
		}
		else
		{
			//overlap check and compute
			//堆叠还要验证绑定信息
			if (origin_goods->id != goods.id || origin_goods->proctype != goods.proctype) 
			{
				//exchange object
				TradeItems::iterator it = Find(possession,goods);
				it->pos = toPos;
				origin_goods->pos = goods.pos;
				modified = true;
			   	return true;
			}
			if (origin_goods->count == origin_goods->max_count) return false;
			if (origin_goods->count + goods.count > origin_goods->max_count)
			{
				TradeItems::iterator it = Find(possession,goods);
				goods.count = origin_goods->max_count - origin_goods->count; //count that can move
				it->count -= goods.count;
				origin_goods->count = origin_goods->max_count;
			}
			else
			{
				TradeItems::iterator it = Find(possession,goods);
				origin_goods->count += goods.count;
				it->count -= goods.count;
				if (it->count == 0)
					possession.erase(it);
			}
		}
		modified = true;
		return true;
	}
	bool Trader::Submit()
	{
		if(state==Trader::READY)
			return true;
		if (! StatePolicy(OPT_SUBMIT)) return false;
		ChangeState(Trader::READY);
		return true;
	}
	bool Trader::Confirm()
	{
		if(state==Trader::CONFIRM)
			return true;
		if (! StatePolicy(OPT_CONFIRM)) return false;
		ChangeState(Trader::CONFIRM);
		return true;
	}

	
	/************ memeber functions of Transaction **************/
	bool Transaction::Update()
   	{ 
		if (!state->TimePolicy(m_timer.Elapse()))
		{
			if (OnDestroy!=NULL && state!=&_TState_Init)
			   	(*OnDestroy)(tid,state!=&_TState_Null,Alice->modified,Bob->modified);
			this->ReleaseObject();
			return false;
		}
		else
			return true;		
	}

	Transaction* Transaction::Create(const Trader::id_t alice, const Trader::id_t bob)  //static function
	{
		static unsigned int _tid=1;
		Transaction* t=new Transaction();
		t->tid=_tid++;
		Thread::Mutex::Scoped l(locker_map);
		GetTransMap()[t->tid]=t;
		t->ChangeState(&_TState_Init);
		t->Alice = new Trader(alice);
		t->Bob = new Trader(bob);
		//set tid to Trader
		t->Alice->tid = t->tid;
		t->Bob->tid = t->tid;

		IntervalTimer::Attach(t,1000000/IntervalTimer::Resolution());
		return t;
	}
	
	//int Transaction::AttachTrader(const Trader::id_t& id,Trader::TradeItems& pssn,unsigned int money)
	int Transaction::AttachTrader(int id)
	{
		Thread::Mutex::Scoped l(locker_self);
		if (!state->OptionPolicy(Trader::OPT_ATTACHTRADER)) return ERR_TRADE_INVALID_TRADER;

		Trader* trader=FindTrader(id);
		if (trader==NULL)
		   return ERR_TRADE_INVALID_TRADER;
		
		{
			Thread::Mutex::Scoped l(locker_map);
			Transaction::TraderMap::iterator it=GetTraderMap().find(id);
			if (it != GetTraderMap().end()) return ERR_TRADE_BUSY_TRADER;
			trader->ChangeState(Trader::INIT_HALF);
			GetTraderMap()[id] = trader;
		}	
		{
			if (Alice->state != Trader::INIT_HALF || Bob->state != Trader::INIT_HALF )
			{
				ChangeState(&_TState_Init_half);
				return ERR_TRADE_ATTACH_HALF;
			}
			else
			{
				ChangeState(&_TState_SetPossession);
				return ERR_TRADE_ATTACH_DONE;
			}
		}
	}
	
	int Transaction::SetPossession(int id1,GRolePocket& p1, int id2, GRolePocket& p2) 
	{
		Thread::Mutex::Scoped l(locker_self);
		if (!state->OptionPolicy(Trader::OPT_SETPSSN)) return ERR_TRADE_SETPSSN;
		Trader* t=FindTrader(id1);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER;
		t->SetPossession(p1);

		t=FindTrader(id2);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER;
		t->SetPossession(p2);
		//change transaction's state
		ChangeState(&_TState_Exchange);
		//change trader's state
		Alice->ChangeState(Trader::EXCHANGE);
		Bob->ChangeState(Trader::EXCHANGE);
		return ERR_SUCCESS;
	}

	int Transaction::AddExchgObject(int id,GRoleInventory& goods,unsigned int money)
	{
		Thread::Mutex::Scoped l(locker_self);
		Trader* t=FindTrader(id);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER;
		if (!state->OptionPolicy(Trader::OPT_UPDATEGOODS))
			return ERR_TRADE_ADDGOODS;
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		if ((is_central&&money) || goods.count<0 || (goods.id&&!goods.count) || (!goods.id&&!money))
		{
			Log::log(LOG_ERR,"trade, invalid command, roleid=%d,item=%d,count=%d,money=%d", 
				id,goods.id,goods.count,money);
			return ERR_TRADE_ADDGOODS;
		}
		
		if (t->AddExchgObject(goods,money))
		{
			ChangeState(&_TState_Exchange);
			if (Alice->GetState() == Trader::READY) Alice->ChangeState(Trader::EXCHANGE);
			if (Bob->GetState() == Trader::READY) Bob->ChangeState(Trader::EXCHANGE);
			return ERR_SUCCESS;
		}
		else
			return ERR_TRADE_ADDGOODS;
	}
	int Transaction::RemoveExchgObject(int id, GRoleInventory& goods,unsigned int money)
	{
		Thread::Mutex::Scoped l(locker_self);
		Trader* t=FindTrader(id);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER;
		if (!state->OptionPolicy(Trader::OPT_UPDATEGOODS))
			return ERR_TRADE_RMVGOODS;
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		if (is_central && money!=0)
			return ERR_TRADE_RMVGOODS;
		if (t->RemoveExchgObject(goods,money))
		{
			ChangeState(&_TState_Exchange);
			if (Alice->GetState() == Trader::READY) Alice->ChangeState(Trader::EXCHANGE);
			if (Bob->GetState() == Trader::READY) Bob->ChangeState(Trader::EXCHANGE);
			return ERR_SUCCESS;
		}
		else
			return ERR_TRADE_RMVGOODS;
	}
	int Transaction::MoveObject(int id, GRoleInventory& goods, int toPos)
	{
		Thread::Mutex::Scoped l(locker_self);
		Trader* t=FindTrader(id);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER; 
		if (!state->OptionPolicy(Trader::OPT_MOVEOBJ))
			return ERR_TRADE_MOVE_FAIL;
		if (!t->MoveObject(goods,toPos))
			return ERR_TRADE_MOVE_FAIL;
		return ERR_SUCCESS;
	}
	int Transaction::Submit(int id,int* problem_roleid)
	{
		Thread::Mutex::Scoped l(locker_self);
		Trader* t=FindTrader(id);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER; 
		if (!state->OptionPolicy(Trader::OPT_SUBMIT))
			return ERR_TRADE_SUBMIT_FAIL;
		if (!t->Submit()) return ERR_TRADE_SUBMIT_FAIL;
		if (Alice->GetState() == Bob->GetState())
		{
			bool _alice=true;
			bool _bob=true;
			if (!(_alice=GenerateResult(_newposs_alice,&_newmoney_alice,Bob,Alice)) ||
				!(_bob=GenerateResult(_newposs_bob,&_newmoney_bob,Alice,Bob)) )
			{
				if (!_alice && problem_roleid!=NULL) *problem_roleid=Alice->id.roleid;
				if (!_bob && problem_roleid!=NULL) *problem_roleid=Bob->id.roleid;
				blGenResult=false;
				//change state to exchange, let traders to adjust their objects
				ChangeState(&_TState_Exchange);
				if (Alice->GetState() == Trader::READY) Alice->ChangeState(Trader::EXCHANGE);
				if (Bob->GetState() == Trader::READY) Bob->ChangeState(Trader::EXCHANGE);

				return ERR_TRADE_SPACE;	//insufficient space
			}
			else
			{
				blGenResult=true;	//generation of transaction result is done!
				ChangeState(&_TState_Ready);
				return ERR_TRADE_READY;
			}
		}
		else
		{
			ChangeState(&_TState_Ready_half);
			return ERR_TRADE_READY_HALF;
		}
	}
	int Transaction::Confirm(int id,int* problem_roleid)
	{
		Thread::Mutex::Scoped l(locker_self);
		Trader* t=FindTrader(id);
		if (t==NULL) return ERR_TRADE_INVALID_TRADER;
		if (!state->OptionPolicy(Trader::OPT_CONFIRM))
			return ERR_TRADE_CONFIRM_FAIL;
		if (!t->Confirm()) return ERR_TRADE_CONFIRM_FAIL;
		if (Alice->GetState() == Bob->GetState())
		{
			ChangeState(&_TState_Confirm);
			DEBUG_PRINT("Transaction Done. alice state=%d, bob state=%d(id:%d)\n",
					Alice->GetState(),Bob->GetState(),id);
			if (!Alice->exchg_money && !Alice->exchg_obj.size() && !Bob->exchg_money && !Bob->exchg_obj.size())
			{
				//Alice and Bob's modified is decided by whether they moved objects
			}
			else
			{
				if (!blGenResult)
				{
					TradeItems _pssn_A,_pssn_B;
					unsigned int _money_A,_money_B;
					if (!GenerateResult(_pssn_A,&_money_A,Bob,Alice)) return ERR_TRADE_SPACE;
					if (!GenerateResult(_pssn_B,&_money_B,Alice,Bob)) return ERR_TRADE_SPACE;
					Alice->possession.swap(_pssn_A);
					Alice->pssn_money=_money_A;
					Bob->possession.swap(_pssn_B);
					Bob->pssn_money=_money_B;
				}
				else
				{
					Alice->possession.swap(_newposs_alice);
					Alice->pssn_money=_newmoney_alice;
					Bob->possession.swap(_newposs_bob);
					Bob->pssn_money=_newmoney_bob;
				}
				Alice->modified = true;
				Bob->modified = true;
				LogTrade();
			}
			return ERR_TRADE_DONE;
		}
		else
		{
			ChangeState(&_TState_Confirm_half);
			DEBUG_PRINT("Confirm half. roleid=%d, alice state=%d, bob state=%d(id:%d)\n",
					id, Alice->GetState(),Bob->GetState(),id);
			return ERR_TRADE_HALFDONE;
		}
	}
	int Transaction::Discard()
	{
		Thread::Mutex::Scoped l(locker_self);
		if (!state->OptionPolicy(Trader::OPT_DISCARD))
			return ERR_TRADE_DISCARDFAIL;
		
		TradeItems _pssn;
		unsigned int _money;
		GenerateResult(_pssn,&_money,Alice,Alice);
		Alice->possession.swap(_pssn); Alice->pssn_money = _money;
		_pssn.clear();
		GenerateResult(_pssn,&_money,Bob,Bob);
		Bob->possession.swap(_pssn); Bob->pssn_money = _money;

		ChangeState(&_TState_Init);
		//Alice and Bob's modified is decided only by whether they moved objects
		return ERR_SUCCESS;
	}
	bool Transaction::GetExchgResult(TradeItems& exchg1,unsigned int* money1,TradeItems& exchg2,
		unsigned int* money2,bool& blNeedSave)
	{
		Thread::Mutex::Scoped l(locker_self);
		if (Alice==NULL || Bob==NULL) return false;
		
	   	exchg1.swap(Alice->possession);
		exchg2.swap(Bob->possession);
		*money1 = Alice->pssn_money;
		*money2 = Bob->pssn_money;	
		blNeedSave=(Alice->modified || Bob->modified);
		return true;
	}
	void Transaction::Destroy(bool blRollback)
	{
		Thread::Mutex::Scoped l(locker_self);
		if ( blRollback )
		{
			if ( Alice ) Alice->modified=false;
			if ( Bob ) Bob->modified=false;
		}
		ChangeState(&_TState_Null);
	}
	bool Transaction::GenerateResult(TradeItems& possession,unsigned int* money,Trader* fromTrader,Trader* toTrader)
	{
		if ( toTrader==NULL || fromTrader==NULL ) return false;
		//check money
		if (toTrader->pssn_money>NEW_MAX_CASH_IN_POCKET || fromTrader->exchg_money>NEW_MAX_CASH_IN_POCKET || toTrader->pssn_money>NEW_MAX_CASH_IN_POCKET-fromTrader->exchg_money )
			return false;

		*money = toTrader->pssn_money+fromTrader->exchg_money;
		std::set<size_t> freeslot;

		TradeItems::iterator iv,ive;
		TradeItems::const_iterator it=fromTrader->exchg_obj.begin(),ie=fromTrader->exchg_obj.end();

		for(size_t i=0;i<toTrader->capacity;i++)
			freeslot.insert(i);
		possession.clear();
		for(iv=toTrader->possession.begin(),ive=toTrader->possession.end();iv!=ive;++iv)
		{
			if(iv->count)
			{
				freeslot.erase(iv->pos);
				possession.push_back(*iv);
			}
		}
		for(;it!=ie;++it)
		{
			int count = it->count;
			for(iv=possession.begin(),ive=possession.end();iv!=ive;++iv)
			{
				if(iv->id==it->id && iv->proctype==it->proctype && iv->count<iv->max_count) 
				{
					if (iv->count+count > iv->max_count)
					{
						count = iv->count + count - iv->max_count;
						iv->count = iv->max_count;
					}
					else 
					{
						iv->count += count;
						count = 0;
						break; 
					}
				}
			}
			if(count)
			{
				if(!freeslot.size())
					return false;
				possession.push_back(*it);
				GRoleInventory& item = possession.back();
				item.count = count;
				item.pos = *(freeslot.begin());
				freeslot.erase(freeslot.begin());
			}
		}
		return true;
	}
	void Transaction::LogTrade()
	{

		std::string items = "hitemid=";
		Log::ObjectVector AliceExgObjs,BobExgObjs;
		for (TradeItems::iterator it=Alice->exchg_obj.begin();it!=Alice->exchg_obj.end();++it)
		{
			AliceExgObjs.push_back( Log::trade_object(it->id,it->pos,it->count));
			char buf[32];
			sprintf( buf, "%d,%d:", it->id, it->count);
			items += buf; 
		}
		items += "citemid=";
		for (TradeItems::iterator it=Bob->exchg_obj.begin();it!=Bob->exchg_obj.end();++it)
		{
			BobExgObjs.push_back( Log::trade_object(it->id,it->pos,it->count));
			char buf[32];
			sprintf( buf, "%d,%d:", it->id, it->count);
			items += buf; 
		}
		Log::trade(Alice->id.roleid,Bob->id.roleid,Alice->exchg_money,Bob->exchg_money,AliceExgObjs,BobExgObjs);
		GLog::action("tradeend, hrid=%d:crid=%d:tid=%d:%shgold=%d:cgold=%d", Alice->id.roleid,Bob->id.roleid,tid,items.c_str(),Alice->exchg_money,Bob->exchg_money);
	}
	void Transaction::DiscardTransaction(int roleid)
	{
		if(!roleid)
			return;
		GNET::Transaction* t;
		if ((t=Transaction::FindTransactionbyTrader(roleid)))
		{
			if(t->Discard()==ERR_SUCCESS )
			{
				GDeliveryServer* dsm = GDeliveryServer::GetInstance();
				Trader::id_t* id;
				if((id=t->GetAlice()))
					dsm->Send(id->linksid,TradeDiscard_Re(ERR_SUCCESS,t->GetTid(),roleid,id->roleid,id->localsid));
				if((id=t->GetBob()))
					dsm->Send(id->linksid,TradeDiscard_Re(ERR_SUCCESS,t->GetTid(),roleid,id->roleid,id->localsid));
				t->Destroy(true);
			}
		}
	}
};
