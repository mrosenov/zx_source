#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbbuffer.h"
#include "utilfunction.h"
#include "user"
#include "gsyslog"
#include "grolestorehouse"
#include "gamedbmanager.h"

namespace GNET
{
void UpdateCash(StorageEnv::Storage *puser, GMailSyncData& data, StorageEnv::Transaction& txn)
{
	/*
	if(puser)
	{
		Marshal::OctetsStream key;
		User user;
		key<< data.userid;
		Marshal::OctetsStream(puser->find(key,txn))>>user;
		if(user.use_serial<data.cash_serial && user.cash_used<data.cash_used)
		{
			GameDBManager::GetInstance()->UpdateCash(data.cash_used - user.cash_used);
			user.use_serial = data.cash_serial;
			user.cash_used = data.cash_used;
			puser->insert( key, Marshal::OctetsStream()<<user, txn );
		}
	}
	*/
}
void UpdateStore(int roleid, StorageEnv::Storage *pstore, GMailSyncData& data, StorageEnv::Transaction& txn, int money_delta)
{
	/*
	Marshal::OctetsStream key;
	key << roleid;
        if(pstore)
        {               
		Marshal::OctetsStream value;
		if(pstore->find(key, value, txn))
		{
			GRoleStorehouse store;
			value >> store;
			money_delta += data.storehouse.money - store.money;
			pstore->insert(key, Marshal::OctetsStream()<<data.storehouse,txn);
			data.data_mask |= SYNC_STOTEHOUSE;
		}
        }                       
	*/
	Marshal::OctetsStream key;
	key << roleid;
	Marshal::OctetsStream value;
	if(pstore->find(key, value, txn))
	{
		GRoleStorehouse store;
		value >> store;
		if (data.data_mask & SYNC_STOTEHOUSE)
		{
			money_delta += data.storehouse.money - store.money;
			store = data.storehouse;
		}
		else //符文包裹属于包裹范畴 一定要存
			store.fuwen = data.storehouse.fuwen;
		pstore->insert(key, Marshal::OctetsStream()<<store,txn);
	}
	if(money_delta)
		GameDBManager::GetInstance()->UpdateMoney(roleid, money_delta);
}
void UpdateShoplog(StorageEnv::Storage *plog, GMailSyncData& data, StorageEnv::Transaction& txn)
{
	/*
        if(plog)
        {               
                Marshal::OctetsStream keylog;
                keylog << GameDBManager::GetInstance()->GetShoplogid();
                plog->insert(keylog, Marshal::OctetsStream()<<data.logs, txn);
                data.data_mask |= SYNC_SHOPLOG;
        }
	*/
}
void PutSyslog(StorageEnv::Storage *plog,StorageEnv::Transaction& txn,int roleid,int ip, GRoleInventory& inv )
{
        if(plog)
        {               
		GSysLog log(roleid,Timer::GetTime(),ip,0,0);
		log.items.push_back(inv);
                Marshal::OctetsStream keylog;
                keylog << GameDBManager::GetInstance()->GetGUID();
                plog->insert(keylog, Marshal::OctetsStream()<<log, txn);
        }
}
void PutSyslog(StorageEnv::Storage *plog,StorageEnv::Transaction& txn,int roleid,int ip, int money, GRoleInventoryVector& invs)
{
        if(plog)
        {               
		GSysLog log(roleid,Timer::GetTime(),ip,1, money);
		log.items.swap(invs);
                Marshal::OctetsStream keylog;
                keylog << GameDBManager::GetInstance()->GetGUID();
                plog->insert(keylog, Marshal::OctetsStream()<<log, txn);
        }
}

bool DecPocketItemInPos(GRoleInventoryVector & list, int pos, int itemid, int count, GRoleInventoryVector & change_items)
{
	GRoleInventoryVector::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{       
		GRoleInventory & item = *it;
		if (item.pos == pos)
		{
			if ((int)item.id != itemid || item.count < count)
				return false;
			item.count -= count;
			change_items.push_back(item);
			if (item.count == 0)
			{
				list.erase(it);
			}
			return true;
		}
	}
	return false;
}
};
