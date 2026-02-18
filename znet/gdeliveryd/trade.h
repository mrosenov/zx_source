#ifndef __GNET_TRADE_H
#define __GNET_TRADE_H
#include <vector>
#include <set>
#include <map>

#include "../common/octets.h"
#include "../common/thread.h"
#include "../common/itimer.h"
#include "../rpc/macros.h"
#include "../rpc/errcode.h"
#include "groleinventory"
#include "grolepocket"

#define	TRADER_SERVER 0

namespace GNET
{
typedef std::vector<GRoleInventory> TradeItems;

class Transaction;
class Trader
{
	friend class Transaction;
public:
	struct id_t
	{
		int roleid;
		unsigned int linksid;
		unsigned int localsid;
		id_t() { }
		id_t(int _r,unsigned int _n=0, unsigned int _l=0) : roleid(_r),linksid(_n),localsid(_l) { }
	};
	enum Options
	{ OPT_ATTACHTRADER,OPT_SETPSSN, OPT_UPDATEGOODS, OPT_SUBMIT, OPT_CONFIRM, OPT_DISCARD, OPT_MOVEOBJ };

private:
	enum State
	{ INIT, INIT_HALF, EXCHANGE, READY, CONFIRM };

	enum
	{
		TRADE_ITEM_MAX	= 36,
	};

private:
	id_t			id;
	unsigned int 		tid;		//Transaction ID, which this trader joined in	
	bool			modified;	//Trader's possession is modified
	Trader::State		state;
	
	unsigned int 		capacity;  	//pocket capacity
	unsigned int 		exchg_money;  	//exchange money	
	unsigned int		pssn_money;	//possess money
	TradeItems		exchg_obj;	//exchange objects
	TradeItems		possession;	//possess objects
	
	explicit Trader(const id_t& _id) : id(_id),tid(0),modified(false),state(INIT),exchg_money(0),pssn_money(0) {  }	
	void SetPossession(GRolePocket& pocket);

	Trader::State GetState() { return state; }
	void ChangeState(Trader::State _state) { state=_state; }
	bool StatePolicy(Trader::Options opt) const
	{ 
		if (state==READY && opt==OPT_UPDATEGOODS) return false;
		if (state==CONFIRM && opt==OPT_DISCARD) return false;
		if (opt==OPT_CONFIRM && state!=READY) return false;
		if (opt==OPT_MOVEOBJ && state!=EXCHANGE) return false;
		return true;
	}
	
	bool VerifyGoods(GRoleInventory& goods,TradeItems& goods_list);	
	bool AddExchgObject(GRoleInventory& goods,unsigned int money);
	bool RemoveExchgObject(GRoleInventory& goods,unsigned int money);
	bool MoveObject(GRoleInventory& goods,int toPos);
	TradeItems::iterator Find(TradeItems& list, GRoleInventory& item)
	{
		TradeItems::iterator it=list.begin(),ie=list.end();
		for(;it!=ie;++it)
		{
			if(it->id==item.id && it->pos==item.pos)
				return it;
		}
		return it;
	}

	bool Submit(); 
	bool Confirm();
public:
	~Trader() 
	{ 
		exchg_obj.clear();
		possession.clear();
	}
};

class Transaction : public IntervalTimer::Observer
{
public:
	class State
	{
		int		timeout;
		std::set<Trader::Options> set;
	public:
		State(int t) : timeout(t) { }
		State(int t,Trader::Options* first,size_t size) : timeout(t),set(first,first+size) { }
		bool TimePolicy(int elapse_time) { return timeout==-1 ? true:elapse_time<timeout; }
		bool OptionPolicy(Trader::Options opt) { return set.find(opt) != set.end(); }
	};	

	typedef void (*CallBack) (unsigned int,bool/*timeout*/,bool/*Alice modified*/,bool/*Bob modified*/);
   	CallBack OnDestroy;
	int gs_id;	//game server id
private:
	unsigned int tid;
	IntervalTimer 	m_timer;
	
	Trader* Alice;
	Trader* Bob;
	
	Transaction::State* state;

	bool			blGenResult;	//whether generate transaction result
	unsigned int		_newmoney_alice;//new money value of alice after transaction
	unsigned int 		_newmoney_bob;	//new money value of bob after transaction
	TradeItems		_newposs_alice;	//new possess of alice after transaction
	TradeItems		_newposs_bob;	//new possess of bob after transaction

	typedef	std::map<unsigned int/*tid*/,Transaction* > TransactionMap;
	static TransactionMap& GetTransMap() { static TransactionMap trans_map; return trans_map; }
	typedef	std::map<int,Trader*> TraderMap;
	static TraderMap& GetTraderMap() { static TraderMap trader_map; return trader_map; }
	static Thread::Mutex locker_map;
	
	Thread::Mutex locker_self;
	
	Transaction() : Alice(NULL),Bob(NULL),state(NULL),locker_self("Transaction::locker_self") { 
		blGenResult=false; 
		OnDestroy=NULL;
	}
	
	void ChangeState(Transaction::State* _state) { state=_state; m_timer.Reset(); }
	Trader* FindTrader(int id) 
	{
		if (Alice != NULL && Alice->id.roleid==id) return Alice;
		if (Bob != NULL && Bob->id.roleid==id) return Bob;
		return NULL;
	}
	void ReleaseObject()
	{
		{
			Thread::Mutex::Scoped l(locker_map);
			TraderMap::iterator it;
			TraderMap& map = GetTraderMap();
			if (Alice != NULL) 
			{
				it=map.find(Alice->id.roleid);
				if ( it!=map.end() && (*it).second->tid==tid) 
					map.erase(it);
			}
			if (Bob != NULL) 
			{
				it=map.find(Bob->id.roleid);
				if ( it!=map.end() && (*it).second->tid==tid) 
					map.erase(it);
			}
			GetTransMap().erase(tid);
		}
		if (Alice != NULL) delete Alice;
		if (Bob != NULL) delete Bob;
		delete this;
	}
	bool GenerateResult(TradeItems& possesion,unsigned int* money,Trader* fromTrader,Trader* toTrader);
public:
	static Transaction* Create(const Trader::id_t alice, const Trader::id_t bob);
	static Transaction* GetTransaction(unsigned int tid)
	{
		Thread::Mutex::Scoped l(locker_map);
		TransactionMap::iterator it=GetTransMap().find(tid);
		if (it==GetTransMap().end()) 
			return NULL;
		else
			return (*it).second;
	}
	static bool VerifyTraders(int role1, int role2)
	{
		Thread::Mutex::Scoped l(locker_map);
		TraderMap& map = GetTraderMap();
		return role1!=role2 && map.find(role1)==map.end() && map.find(role2)==map.end();
	}	
	static Transaction* FindTransactionbyTrader(int roleid)
	{
		Thread::Mutex::Scoped l(locker_map);
		TraderMap::const_iterator it=GetTraderMap().find(roleid);
		if (it == GetTraderMap().end()) return NULL;
		
		TransactionMap::iterator it2=GetTransMap().find((*it).second->tid);
		if (it2==GetTransMap().end()) 
			return NULL;
		else
			return (*it2).second;

	}
	bool Update();
	
	unsigned int GetTid() { return tid; }
	int AttachTrader(int id);
	Trader::id_t* GetAlice() { Thread::Mutex::Scoped l(locker_self);  return Alice!=NULL ? &Alice->id : NULL; }
	Trader::id_t* GetBob() { Thread::Mutex::Scoped l(locker_self); return Bob!=NULL ? &Bob->id : NULL; }
	Trader::id_t* GetPartner(const Trader::id_t& id)
	{
		Thread::Mutex::Scoped l(locker_self);
		if (Alice->id.roleid == id.roleid) return &Bob->id;
		if (Bob->id.roleid == id.roleid) return &Alice->id;
		return NULL;
	}
	bool IsParticipant(int roleid)
	{
		Thread::Mutex::Scoped l(locker_self);
		return (Alice && Bob) && (Alice->id.roleid==roleid || Bob->id.roleid==roleid);
	}
	
	int SetPossession(int id1, GRolePocket& p1, int id2, GRolePocket& p2);
	int AddExchgObject(int id, GRoleInventory& goods, unsigned int money);
	int RemoveExchgObject(int id, GRoleInventory& goods, unsigned int money);
	int MoveObject(int id, GRoleInventory& goods, int toPos);
	int Submit(int id,int* problem_roleid=NULL);
	int Confirm(int id,int* problem_roleid=NULL);
	int Discard();
	
	bool GetExchgResult(TradeItems& exchg1,unsigned int* money1,TradeItems& exchg2, unsigned int* money2,bool& bSave);
	void ShowTrader();
	void LogTrade();
	
	~Transaction() { }
	void Destroy(bool blRollback=false);
	static void DiscardTransaction(int roleid);
};

}; //end of namespace
#endif
