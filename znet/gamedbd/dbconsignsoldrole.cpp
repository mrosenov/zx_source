//#include "localmacro.h"
//#include "macros.h"
#include "storage.h"
#include "storagetool.h"
#include "dbconsignsoldrole.hrp"
namespace GNET
{

//keep consist with accessdb.cpp
static ItemIdMan g_itemIdMan;
static bool itemloaded=false;
int DBConsignSoldRole::LoadItemid()
{
	if (!g_itemIdMan.LoadItemId("consignroleitemid.txt"))
        {
                Log::log(LOG_ERR, "DBConsignSoldRole::LoadItemid Failed to read item id from 'itemid.txt'\n");
		return -1;
	}
	itemloaded = true;
	LOG_TRACE("DBConsignSoldRole::LoadItemid");
	return 0;			 
}

void DBConsignSoldRole::CheckItemOwner(const std::string &container, int oldroleid, int newroleid, GRoleInventoryVector &items, Octets & newrolename)
{
	if(itemloaded==false)
	{
                Log::log(LOG_ERR, "DBConsignSoldRole::CheckItemOwner Failed to init itemidman ");
		LoadItemid();
		//return;
	}
	//LOG_TRACE("DBConsignSoldRole::CheckItemOwner");
	GRoleInventoryVector::iterator it;
	for(it=items.begin();it!=items.end();)
	{
		int item_type = g_itemIdMan.GetItemType(it->id);
	//	LOG_TRACE("DBConsignSoldRole::CheckItemOwner id=%d type=%d",it->id,item_type);
		if (item_type == ITEMTYPE_DELETE)
		{
			Log::formatlog("soldrole","DBConsignSoldRole::CheckItemOwner delete id=%d\n",it->id);
			it = items.erase(it);
			continue;
		}	
		if (item_type != -1)
		{
			int ownerid = GetOwnerID(item_type, it->data);
			if (ownerid > 0)
			{
				if (ownerid == oldroleid)
				{
					SetOwnerID(item_type, it->data, newroleid);
					SetOwnerName(item_type, it->data, newrolename);
					Log::formatlog("soldrole", "The owner's roleid of item id=%d in pos %d of %s of role id=%d is changed to %d\n", it->id, it->pos, container.c_str(), oldroleid, newroleid);
				}
			}
		}
		else
		{
			int specialid = GetSpecialItemID(it->id, it->data);
			if(specialid == -1)
			{
				++it;
				continue;
			}
			item_type = g_itemIdMan.GetItemType(specialid);
			if (item_type == ITEMTYPE_DELETE)
			{
				it = items.erase(it);
				continue;
			}	
			if (item_type != -1)
			{
				int ownerid = GetSpecialItemOwnerID(it->id, item_type, it->data);
				if (ownerid == oldroleid)
				{
					SetSpecialItemOwnerID(it->id, item_type, it->data, newroleid);
					SetSpecialItemOwnerName(it->id, item_type, it->data, newrolename);
					Log::formatlog("soldrole", "The owner's roleid of  item id=%d special id =%d in pos %d of %s of role id=%d is changed to %d\n", it->id, specialid, it->pos, container.c_str(), oldroleid, newroleid);
				}
			}
		}
		++it;
	}
}

};
