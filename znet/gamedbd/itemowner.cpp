#include "itemowner.h"
#include "log.h"

namespace GNET
{

static int get_new_equip_essence_owner_id( const Octets& item_data )
{
	if( item_data.size() != sizeof( equip_essence_new ) ){ return 0; }
	equip_essence_new* tmp = ( equip_essence_new* )item_data.begin();
	if( tmp->version == EQUIP_ESSENCE_VERSION_OLD ){ return 0; }
	return tmp->master_id;
}

static bool set_id( equip_essence_new* ess, int owner_id )
{
	if( ess->version == EQUIP_ESSENCE_VERSION_OLD ){ return false; }
	ess->master_id = owner_id;
	return true;
}

static bool set_new_equip_essence_owner_id( Octets& item_data, int owner_id )
{
	if( item_data.size() != sizeof( equip_essence_new ) ){ return false; }
	equip_essence_new* tmp = ( equip_essence_new* )item_data.begin();
	return set_id( tmp, owner_id );
}

static bool set_special_new_equip_essence_owner_id( Octets& item_data, int owner_id )
{
	if( item_data.size() != (sizeof( equip_essence_new ) + sizeof(int)*ITEM_BASE_SIZE) ){ return false; }
	equip_essence_new* tmp = ( equip_essence_new* )( (char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE);
	return set_id( tmp, owner_id );
}

static bool set_name( equip_essence_new* ess, const Octets& owner_name, int namesize )
{
	if( ess->version == EQUIP_ESSENCE_VERSION_OLD ){ return false; }
	memset( ess->master_name, 0, 20 );
	memcpy( ess->master_name, owner_name.begin(), namesize);
	return true;
}

static bool set_new_equip_essence_owner_name( Octets& item_data, const Octets& owner_name, int namesize )
{
	if( item_data.size() != sizeof( equip_essence_new ) ){ return false; }
	equip_essence_new* tmp = ( equip_essence_new* )item_data.begin();
	return set_name( tmp, owner_name, namesize );
}

static bool set_special_new_equip_essence_owner_name( Octets& item_data, const Octets& owner_name, int namesize )
{
	if( item_data.size() != (sizeof( equip_essence_new ) + sizeof(int)*ITEM_BASE_SIZE) ){ return false; }
	equip_essence_new* tmp = ( equip_essence_new* )( (char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE);
	return set_name( tmp, owner_name, namesize );
}
// end

int GetOwnerID(int item_type, const Octets & item_data)
{
	int owner_id = 0;
	switch(item_type)
	{
		//法宝和飞剑
		case ITEMTYPE_TALISMAN:
		{
			if(!TestTalismanData(item_data)) return -1;
			owner_id = *(int*)item_data.begin();
		}
		break;

		//宠物
		case ITEMTYPE_PETBADGE:
		{
			char data_type = 0;
			if(!TestPetbedgeData(item_data, data_type)) return -1;

			if(data_type == 1)
			{
				owner_id = *(int*)item_data.begin();
			}
			else if(data_type == 2)
			{
				owner_id = *(int*)( (char*)item_data.begin() + 5);
			}
			else
			{
				return -1;
			}
		}
		break;
		
		//幻灵石
		case ITEMTYPE_TRANS_STONE:
		{
		//	LOG_TRACE("DBConsignSoldRole GetOwnerID ITEMTYPE_TRANS_STONE");
			if(!TestTransStoneData(item_data)) return -1;
			owner_id = *(int*)item_data.begin();
		//	LOG_TRACE("DBConsignSoldRole GetOwnerID ITEMTYPE_TRANS_STONE owner_id=%d",owner_id);
		}
		break;

		//装备
		case ITEMTYPE_EQUIPMENT:
		{
			if(item_data.size() == EQUIP_OLD_ESSENCE_SIZE)
			{
				return 0;
			}
			else if(item_data.size() == EQUIP_SPIRIT_ESSENCE_SIZE || item_data.size() == EQUIP_GEM_ESSENCE_SIZE || item_data.size() == EQUIP_ASTROLOGY_ESSENCE_SIZE)
			{
				owner_id = *(int*)((char*)item_data.begin() + EQUIP_OLD_ESSENCE_SIZE);
			}
			else if( item_data.size() == sizeof( equip_essence_new ) )
			{
				owner_id = get_new_equip_essence_owner_id( item_data );
			}
			else
			{
				return -1;
			}
		}
		break;

		default:
		{
			return -1;
		}
		break;
	}

	return owner_id;
}


bool SetOwnerID(int item_type, Octets & item_data, int owner_id)
{
	if(owner_id < 0) return false;

	switch(item_type)
	{
		//法宝和飞剑
		case ITEMTYPE_TALISMAN:
		{
			if(!TestTalismanData(item_data)) return false;
			*(int*)item_data.begin() = owner_id;
		}
		break;

		//宠物
		case ITEMTYPE_PETBADGE:
		{
			char data_type = 0;
			if(!TestPetbedgeData(item_data, data_type)) return false;
			if(data_type == 1)
			{
				*(int*)item_data.begin() = owner_id;
			}
			else if(data_type == 2)
			{
				 *(int*)( (char*)item_data.begin() + 5) = owner_id;
			}
			else
			{
				return false;
			}
		}
		break;


		//幻灵石
		case ITEMTYPE_TRANS_STONE:
		{
		//	LOG_TRACE("DBConsignSoldRole SetOwnerID ITEMTYPE_TRANS_STONE");
			if(!TestTransStoneData(item_data)) return false;
			*(int*)item_data.begin() = owner_id;
		//	LOG_TRACE("DBConsignSoldRole SetOwnerID ITEMTYPE_TRANS_STONE owner_id=%d",owner_id);
		}
		break;

		//装备
		case ITEMTYPE_EQUIPMENT:
		{
			if(item_data.size() == EQUIP_OLD_ESSENCE_SIZE)
			{
				return false;
			}
			else if(item_data.size() == EQUIP_SPIRIT_ESSENCE_SIZE || item_data.size() == EQUIP_GEM_ESSENCE_SIZE || item_data.size() == EQUIP_ASTROLOGY_ESSENCE_SIZE)
			{
				*(int*)((char*)item_data.begin() + EQUIP_OLD_ESSENCE_SIZE) = owner_id;
			}
			else if( item_data.size() == sizeof( equip_essence_new ) )
			{
				return set_new_equip_essence_owner_id( item_data, owner_id );
			}
			else
			{
				return false;
			}
		}
		break;

		default:
		{
			return false;
		}
		break;
	}
	return true;

}

bool SetOwnerName(int item_type, Octets & item_data, Octets & owner_name)
{
	int namesize=0;
	if(owner_name.size() > 20) 
		namesize = 0;
	else
		namesize = owner_name.size();

	switch(item_type)
	{
		//法宝和飞剑
		case ITEMTYPE_TALISMAN:
		{
			if(!TestTalismanData(item_data)) return false;
		//	*(int*)item_data.begin() = owner_id;
			memset(((int*)item_data.begin())+1,0,20);
			memcpy(((int*)item_data.begin()) + 1,owner_name.begin(),namesize);
		}
		break;

		//宠物
		case ITEMTYPE_PETBADGE:
		{
			char data_type = 0;
			if(!TestPetbedgeData(item_data, data_type)) return false;
		//	*(int*)item_data.begin() = owner_id;
			if(data_type == 1)
			{
				memset(((int*)item_data.begin())+1,0,20);
				memcpy(((int*)item_data.begin()) + 1,owner_name.begin(),namesize);
			}
			else if(data_type == 2)
			{
				memset(((char*)item_data.begin())+9,0,20);
				memcpy(((char*)item_data.begin()) + 9,owner_name.begin(),namesize);
			}
			else
			{
				return false;
			}
		}
		break;

		//幻灵石
		case ITEMTYPE_TRANS_STONE:
		{
		//	LOG_TRACE("DBConsignSoldRole SetOwnername ITEMTYPE_TRANS_STONE");
			if(!TestTransStoneData(item_data)) return false;
		//	*(int*)item_data.begin() = owner_id;
			memset(((int*)item_data.begin())+1,0,20);
			memcpy(((int*)item_data.begin()) + 1,owner_name.begin(),namesize);
		//	LOG_TRACE("DBConsignSoldRole etOwnername ITEMTYPE_TRANS_STONE");
		}
		break;
	
		//装备
		case ITEMTYPE_EQUIPMENT:
		{
			if(item_data.size() == EQUIP_OLD_ESSENCE_SIZE)
			{
				return false;
			}
			else if(item_data.size() == EQUIP_SPIRIT_ESSENCE_SIZE || item_data.size() == EQUIP_GEM_ESSENCE_SIZE || item_data.size() == EQUIP_ASTROLOGY_ESSENCE_SIZE)
			{
				memset(((char*)item_data.begin()) + EQUIP_OLD_ESSENCE_SIZE + sizeof(int),0,20);
				memcpy(((char*)item_data.begin()) + EQUIP_OLD_ESSENCE_SIZE + sizeof(int),owner_name.begin(),namesize);
			}
			else if( item_data.size() == sizeof( equip_essence_new ) )
			{
				return set_new_equip_essence_owner_name( item_data, owner_name, namesize );
			}
			else
			{
				return false;
			}
		}
		break;

		default:
		{
			return false;
		}
		break;
	}
	return true;

}



int GetSpecialItemID(int item_id, const Octets & item_data)
{
	if(item_id != ITEM_UNLOCKING_ID && item_id != ITEM_DESTROY_ID && item_id != ITEM_BROKEN_ID) return -1; 
	if(item_data.size() < sizeof(int)* ITEM_BASE_SIZE) return -1;
	int special_item_id = *(int*)((char*)item_data.begin());
	return special_item_id; 
	
}

int GetSpecialItemOwnerID(int item_id, int special_item_type, const Octets & item_data)
{
	if(item_id != ITEM_UNLOCKING_ID && item_id != ITEM_DESTROY_ID && item_id != ITEM_BROKEN_ID) return -1; 
	if(item_data.size() < sizeof(int)* ITEM_BASE_SIZE) return -1;
	Octets special_item_data((char*)item_data.begin()+ sizeof(int) * ITEM_BASE_SIZE, item_data.size() - sizeof(int)*ITEM_BASE_SIZE);
	return GetOwnerID(special_item_type, special_item_data);

}
bool SetSpecialItemOwnerName(int item_id, int special_item_type, Octets & item_data, Octets & owner_name)
{
	if(item_id != ITEM_UNLOCKING_ID && item_id != ITEM_DESTROY_ID && item_id != ITEM_BROKEN_ID) return false; 
	if(item_data.size() < sizeof(int)*ITEM_BASE_SIZE) return false;
	int namesize=0;
	if(owner_name.size() > 20) 
		namesize = 0;
	else
		namesize = owner_name.size();

	switch(special_item_type)
	{
		//法宝和飞剑
		case ITEMTYPE_TALISMAN:
		{
			if(!TestTalismanData(Octets((char*)item_data.begin()+sizeof(int)*ITEM_BASE_SIZE, item_data.size() - sizeof(int)*ITEM_BASE_SIZE))) return false;
		//	*(int*)((char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
			memset(((int*)item_data.begin())+1+ ITEM_BASE_SIZE,0,20);
			memcpy(((int*)item_data.begin()) + 1 + ITEM_BASE_SIZE,owner_name.begin(),namesize);
		}
		break;

		//宠物
		case ITEMTYPE_PETBADGE:
		{
			char data_type = 0;
			if(!TestPetbedgeData(Octets((char*)item_data.begin()+sizeof(int)*ITEM_BASE_SIZE, item_data.size() - sizeof(int)*ITEM_BASE_SIZE), data_type)) return false;
		//	*(int*)((char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
			if(data_type == 1)
			{
				memset(((int*)item_data.begin())+1+ ITEM_BASE_SIZE,0,20);
				memcpy(((int*)item_data.begin()) + 1 + ITEM_BASE_SIZE,owner_name.begin(),namesize);
			}
			else if(data_type == 2)
			{
				memset(((char*)item_data.begin())+9+ sizeof(int)*ITEM_BASE_SIZE,0,20);
				memcpy(((char*)item_data.begin())+9+ sizeof(int)*ITEM_BASE_SIZE,owner_name.begin(),namesize);
			}
			else
			{
				return false;
			}
		}
		break;

		//幻灵石
		case ITEMTYPE_TRANS_STONE:
		{
		//	LOG_TRACE("DBConsignSoldRole etOwnername sp ITEMTYPE_TRANS_STONE");
			if(!TestTransStoneData(Octets((char*)item_data.begin()+sizeof(int)*ITEM_BASE_SIZE, item_data.size() - sizeof(int)*ITEM_BASE_SIZE))) return false;
			memset(((int*)item_data.begin())+1+ ITEM_BASE_SIZE,0,20);
			memcpy(((int*)item_data.begin()) + 1 + ITEM_BASE_SIZE,owner_name.begin(),namesize);
		}
		break;

		//装备
		case ITEMTYPE_EQUIPMENT:
		{
			if(item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_OLD_ESSENCE_SIZE)
			{
				return false;
			}
			else if( (item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_SPIRIT_ESSENCE_SIZE) ||
				 (item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_GEM_ESSENCE_SIZE) ||
				 (item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_ASTROLOGY_ESSENCE_SIZE))
			{
				memset(((char*)item_data.begin()) + EQUIP_OLD_ESSENCE_SIZE + sizeof(int)*ITEM_BASE_SIZE+ sizeof(int),0,20);
				memcpy(((char*)item_data.begin()) + EQUIP_OLD_ESSENCE_SIZE + sizeof(int)*ITEM_BASE_SIZE + sizeof(int),owner_name.begin(),namesize);
			}
			else if( item_data.size() - sizeof(int)*ITEM_BASE_SIZE == sizeof( equip_essence_new ) )
			{
				set_special_new_equip_essence_owner_name( item_data, owner_name, namesize );
			}
			else
			{
				return false;
			}
		}
		break;

		default:
		{
			return false;
		}
		break;
	}
	return true;

}

bool SetSpecialItemOwnerID(int item_id, int special_item_type, Octets & item_data, int owner_id)
{
	if(item_id != ITEM_UNLOCKING_ID && item_id != ITEM_DESTROY_ID && item_id != ITEM_BROKEN_ID) return false; 
	if(item_data.size() < sizeof(int)*ITEM_BASE_SIZE) return false;

	switch(special_item_type)
	{
		//法宝和飞剑
		case ITEMTYPE_TALISMAN:
		{
			if(!TestTalismanData(Octets((char*)item_data.begin()+sizeof(int)*ITEM_BASE_SIZE, item_data.size() - sizeof(int)*ITEM_BASE_SIZE))) return false;
			*(int*)((char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
		}
		break;

		//宠物
		case ITEMTYPE_PETBADGE:
		{
			char data_type = 0;
			if(!TestPetbedgeData(Octets((char*)item_data.begin()+sizeof(int)*ITEM_BASE_SIZE, item_data.size() - sizeof(int)*ITEM_BASE_SIZE), data_type)) return false;
			if(data_type == 1)
			{
				*(int*)((char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
			}
			else if(data_type == 2)
			{
				*(int*)((char*)item_data.begin() + sizeof(int)*ITEM_BASE_SIZE + 5) = owner_id;
			}
			else
			{
				return false;
			}
		}
		break;

		//装备
		case ITEMTYPE_EQUIPMENT:
		{
			if(item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_OLD_ESSENCE_SIZE)
			{
				return false;
			}
			else if(item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_SPIRIT_ESSENCE_SIZE)
			{
				*(int*)((char*)item_data.begin() + EQUIP_OLD_ESSENCE_SIZE + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
			}
			else if(item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_GEM_ESSENCE_SIZE)
			{
				*(int*)((char*)item_data.begin() + EQUIP_OLD_ESSENCE_SIZE + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
			}
			else if(item_data.size() - sizeof(int)*ITEM_BASE_SIZE == EQUIP_ASTROLOGY_ESSENCE_SIZE)
			{
				*(int*)((char*)item_data.begin() + EQUIP_OLD_ESSENCE_SIZE + sizeof(int)*ITEM_BASE_SIZE) = owner_id;
			}
			else if( item_data.size() - sizeof(int)*ITEM_BASE_SIZE == sizeof( equip_essence_new ) )
			{
				return set_special_new_equip_essence_owner_id( item_data, owner_id );
			}
			else
			{
				return false;
			}
		}
		break;

		default:
		{
			return false;
		}
		break;
	}
	return true;

}

bool TestTalismanData(const Octets & item_data)
{
	size_t size = item_data.size();
	const void *_content = item_data.begin();

	if(size < TALISMAN_ESSENCE_SIZE + sizeof(int)*2) return false;

	size_t * pData = (size_t*)((char *)_content + TALISMAN_ESSENCE_SIZE);
	size -= TALISMAN_ESSENCE_SIZE;
	size_t tmpSize = pData[0];
	if(tmpSize > 0x7FFFFFFF) return false;
	if(size < sizeof(size_t) + tmpSize + sizeof(size_t)) return false;

	pData = (size_t*)((char *)_content + TALISMAN_ESSENCE_SIZE + sizeof(size_t) + tmpSize);
	size -= sizeof(size_t)  + tmpSize;
	tmpSize = pData[0];

	if(size < sizeof(size_t) + tmpSize) return false;

	size -= sizeof(size_t) + tmpSize;	
	if(size > 0)
	{
		pData = (size_t*)((char*)pData + pData[0] + sizeof(size_t));
		tmpSize = pData[0];	
		if(size != sizeof(size_t) + tmpSize) return false;	
	}
	return true;
}

bool TestPetbedgeData(const Octets & item_data, char & data_type)
{
	//LOG_TRACE("Try Convert Petbedge Data");
	size_t size = item_data.size();
	if(size <= 8) return false;
	size_t magic_number = *(size_t*)item_data.begin();

	const void *_content = item_data.begin();
	//新版本宠物数据
	if(magic_number == 0xFFFFFFFF)
	{
                if(size < PETBEDGE_ESSENCE_SIZE + sizeof(int)) return false;
                size_t magic = *(int*)_content;
                if(magic != 0xFFFFFFFF) return false;

                size_t * pData = (size_t*)((char *)_content + PETBEDGE_ESSENCE_SIZE);
                size -= PETBEDGE_ESSENCE_SIZE; 
                size_t tmpSize = pData[0];
                if(tmpSize > 0x7FFFFFFF) return false;
                if(size < sizeof(size_t) + tmpSize) return false;		

		data_type = 2;
		LOG_TRACE("Convert New Petbedge Data");
	}
	//老版本宠物数据
	else
	{	
		if(size < PETBEDGE_OLD_ESSENCE_SIZE + sizeof(int)) return false;
		size_t * pData = (size_t*)((char *)_content + PETBEDGE_OLD_ESSENCE_SIZE);
		size -= PETBEDGE_OLD_ESSENCE_SIZE;
		size_t tmpSize = pData[0];
		if(tmpSize > 0x7FFFFFFF) return false;
		if(size < sizeof(size_t) + tmpSize) return false;
		data_type = 1;
		//LOG_TRACE("Convert Old Petbedge Data");
	}
	return true;
}

bool TestTransStoneData(const Octets & item_data)
{
	size_t size = item_data.size();
	const void *_content = item_data.begin();
	if(size < TRANS_STONE_ESSENCE_SIZE + sizeof(int)*2) return false;
	
	int * pData = (int*)((char *)_content + TRANS_STONE_ESSENCE_SIZE);
	size -= TRANS_STONE_ESSENCE_SIZE;
	
	size_t tmpSize = pData[0];
	//ASSERT(tmpSize < 0xFFFF);
	if(size < sizeof(size_t) + tmpSize + sizeof(size_t)) return false;
	size -= sizeof(size_t)  + tmpSize;
	
	pData = (int*)(((char*)pData) + pData[0] + sizeof(int));
	tmpSize = pData[0];
	//ASSERT(tmpSize < 0xFFFF);
	if(size != sizeof(size_t) + tmpSize) return false;
	
	return true;
}

bool ItemIdMan::LoadItemId(const char *filename)
{
	printf("\nLoadItemId:\n");
	//	const char *filename = "itemid.txt";
	if(filename==NULL) return false;

	if (access(filename, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", filename);
		return false;
	}

	std::ifstream iof(filename);
	if (!iof.is_open())
	{
		Log::log(LOG_ERR, "Fail to open %s\n", filename);
		return false;
	}

	std::string line;
	while (!std::getline(iof, line).fail()) 
	{
		size_t itemtype_start = line.find_first_not_of(" \t");
		if (itemtype_start == std::string::npos) continue;
		size_t itemtype_end = line.find_first_of(" \t=", itemtype_start+1);
		if (itemtype_end == std::string::npos) continue;
		std::string itemtype = line.substr(itemtype_start, itemtype_end-itemtype_start);
		int item_type;
		if (itemtype == "equipment")
			item_type = ITEMTYPE_EQUIPMENT;
		else if (itemtype == "petbadge")
			item_type = ITEMTYPE_PETBADGE;
		else if (itemtype == "talisman")
			item_type = ITEMTYPE_TALISMAN;
		else if (itemtype == "trans_stone")
			item_type = ITEMTYPE_TRANS_STONE;
		else if (itemtype == "need_delete")
			item_type = ITEMTYPE_DELETE;
		else
		{
			Log::log(LOG_ERR, "Unknown item type '%s'\n", itemtype.c_str());
			return false;
		}
		std::set<int> &idset = m_setItemId[item_type];
		size_t id_start = line.find_first_not_of(" \t=", itemtype_end+1);
		while (id_start != std::string::npos)
		{
			size_t id_end = line.find_first_of(", \t", id_start+1);
			std::string id;
			if (id_end == std::string::npos)
				id = line.substr(id_start);
			else
				id = line.substr(id_start, id_end-id_start);
			int nid = atoi(id.c_str());
			if (nid > 0)
				idset.insert(nid);

			id_start = line.find_first_not_of(", \t", id_end);
		}

		printf("Load %d %s id\n", idset.size(), itemtype.c_str());
	}
	return true;
}

bool ItemIdMan::LoadItemId()
{
	printf("\nLoadItemId:\n");
	const char *filename = "itemid.txt";
	if (access(filename, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", filename);
		return false;
	}

	std::ifstream iof(filename);
	if (!iof.is_open())
	{
		Log::log(LOG_ERR, "Fail to open %s\n", filename);
		return false;
	}

	std::string line;
	while (!std::getline(iof, line).fail()) 
	{
		size_t itemtype_start = line.find_first_not_of(" \t");
		if (itemtype_start == std::string::npos) continue;
		size_t itemtype_end = line.find_first_of(" \t=", itemtype_start+1);
		if (itemtype_end == std::string::npos) continue;
		std::string itemtype = line.substr(itemtype_start, itemtype_end-itemtype_start);
		int item_type;
		if (itemtype == "equipment")
			item_type = ITEMTYPE_EQUIPMENT;
		else if (itemtype == "petbadge")
			item_type = ITEMTYPE_PETBADGE;
		else if (itemtype == "talisman")
			item_type = ITEMTYPE_TALISMAN;
		else if (itemtype == "trans_stone")
			item_type = ITEMTYPE_TRANS_STONE;
		else
		{
			Log::log(LOG_ERR, "Unknown item type '%s'\n", itemtype.c_str());
			return false;
		}
		std::set<int> &idset = m_setItemId[item_type];
		size_t id_start = line.find_first_not_of(" \t=", itemtype_end+1);
		while (id_start != std::string::npos)
		{
			size_t id_end = line.find_first_of(", \t", id_start+1);
			std::string id;
			if (id_end == std::string::npos)
				id = line.substr(id_start);
			else
				id = line.substr(id_start, id_end-id_start);
			int nid = atoi(id.c_str());
			if (nid > 0)
				idset.insert(nid);

			id_start = line.find_first_not_of(", \t", id_end);
		}

		printf("Load %d %s id\n", idset.size(), itemtype.c_str());
	}
	return true;
}

};
