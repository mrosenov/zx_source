#ifndef __GNET_LIBCOMMON_H
#define __GNET_LIBCOMMON_H

#include "../gdbclient/db_if.h" //for struct itemdata
#include "../include/localmacro.h"
#include "groleinventory"
#include "grolestorehouse"
#include "gmailsyncdata"
namespace GNET
{
	static void CvtItm2Ivry( const GDB::itemdata* list, size_t size, GRoleInventoryVector& inventory )
	{
		for ( size_t i=0;i<size;++i )
		{
			inventory.add(
				GRoleInventory(
					list[i].id,
					list[i].index,
					list[i].count,
					list[i].client_size,
					list[i].max_count,
					Octets( list[i].data,list[i].size ),
					list[i].proctype,
					list[i].expire_date,
					list[i].guid1,
					list[i].guid2
				)	
			);
		}
	}
	
	static void CvtPocItm2Ivry( const GDB::pocket_item* list, size_t size, GPocketInventoryVector& inventory )
	{
		for ( size_t i=0;i<size;++i )
		{
			inventory.add(
				GPocketInventory(
					list[i].id,
					list[i].index,
					list[i].count
				)	
			);
		}
	}
	static bool GetInventory( GRolePocket& inv,object_interface& obj_if )
	{
		{
			size_t inv_size=obj_if.GetInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtItm2Ivry( &list[0],item_size,inv.items );
		}
		{
			size_t inv_size=obj_if.GetPetBedgeInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetPetBedgeInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtItm2Ivry( &list[0],item_size,inv.petbadge );
		}
		{
			size_t inv_size=obj_if.GetPetEquipInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetPetEquipInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtItm2Ivry( &list[0],item_size,inv.petequip );
		}

		{
			size_t inv_size=obj_if.GetPocketInventorySize();
			int item_size=0;
			GDB::pocket_item list[inv_size];
			if ( (item_size=obj_if.GetPocketInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtPocItm2Ivry( &list[0],item_size,inv.pocket_items);
		}

		{
			size_t inv_size=obj_if.GetFashionInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetFashionInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtItm2Ivry( &list[0],item_size,inv.fashion);

		}

		{
			size_t inv_size=obj_if.GetGiftInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetGiftInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtItm2Ivry( &list[0],item_size,inv.gifts);

		}
		
		{
			size_t inv_size=obj_if.GetMountWingInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetMountWingInventoryDetail(&list[0],inv_size))<0 )
				return false;

			GRoleInventoryVector mountwing;
			CvtItm2Ivry( &list[0],item_size, mountwing);

			try
			{
				GNET::Marshal::OctetsStream stream;
				stream << (short)inv_size;
				stream << mountwing;
				inv.mountwing = stream;
			}
			catch(...)
			{
				printf("\n\nERR:Info2Role Marshal GRoleInventory mountwing Error - libcommon.h\n\n");
				Log::log(LOG_ERR, "Info2Role Marshal GRoleInventory mountwing Error - libcommon.h\n");
			}
		}

		return true;
	}

	/*
	static bool GetShoplog( std::vector<GShopLog>& logs,object_interface& obj_if )
	{
		size_t size = obj_if.GetMallOrdersCount();
		if(!size)
			return true;
		GDB::shoplog *list = new GDB::shoplog[size];
		if ( obj_if.GetMallOrders(list,size)<0 )
		   	return false;
		int roleid = obj_if.GetSelfID().id;
		for(size_t i=0;i<size;++i)
		{
			logs.push_back(GShopLog(
						roleid,
						list[i].order_id,
						list[i].item_id, 
						list[i].expire, 
						list[i].item_count, 
						list[i].order_count,
						list[i].cash_need, 
						list[i].time,
						list[i].guid1,
						list[i].guid2
						)
					);
		}
		delete list;
		return true;
	}
	*/

	static bool GetEquipment( GRoleInventoryVector& inventory,object_interface& obj_if )
	{
		size_t inv_size = obj_if.GetEquipmentSize();
		int item_size=0;
		GDB::itemdata list[inv_size];
		if ( (item_size=obj_if.GetEquipmentDetail(&list[0],inv_size))<0 )
		   	return false;
		CvtItm2Ivry( &list[0],item_size,inventory );
		return true;
	}

	static bool GetStorehouse( GRoleStorehouse& store,object_interface& obj_if )
	{
		store.money = obj_if.GetTrashBoxMoney();
		{
			size_t size = obj_if.GetTrashBoxCapacity();
			store.capacity = size;
			GDB::itemdata list[size];
			int item_size=0;
			if ( (item_size=obj_if.GetTrashBoxDetail(list,size))<0 )
				return false;
			CvtItm2Ivry( list,item_size,store.items );
		}
		{
			size_t size = obj_if.GetMafiaTrashBoxCapacity();
			store.capacity2 = size;
			GDB::itemdata list[size];
			int item_size=0;
			if ( (item_size=obj_if.GetMafiaTrashBoxDetail(list,size))<0 )
				return false;
			CvtItm2Ivry( list,item_size,store.items2 );
		}
		return true;
	}

	static bool GetFuwenInventory( GRoleStorehouse& store,object_interface& obj_if )
	{
		{
			size_t inv_size=obj_if.GetFuwenInventorySize();
			int item_size=0;
			GDB::itemdata list[inv_size];
			if ( (item_size=obj_if.GetFuwenInventoryDetail(&list[0],inv_size))<0 )
				return false;
			CvtItm2Ivry( &list[0],item_size,store.fuwen);

		}
		return true;
	}

	static bool GetSyncData(GMailSyncData& data, object_interface& obj_if)
	{
		int used, delta;
		data.userid = obj_if.GetDBMagicNumber();
		data.data_mask = 0;
		data.inventory.money = obj_if.GetMoney();
		data.inventory.capacity = obj_if.GetInventorySize();
		data.inventory.timestamp = obj_if.InceaseDBTimeStamp();
		data.inventory.pocket_capacity = obj_if.GetPocketInventorySize();
		if(obj_if.IsCashModified())
			data.data_mask |= SYNC_CASHUSED;
		if(!obj_if.GetMallInfo(used, data.cash_total, delta, data.cash_serial))
			return false;
		data.cash_total += used;	//gs和delivery的意义转换
		data.cash_used = used - delta;
		//data.cash_total - data.cash_used应当是实际百宝阁可用cash数量

		if (!GetInventory(data.inventory,obj_if)) 
			return false;
		if (!GetEquipment(data.inventory.equipment,obj_if)) 
			return false;
		//if (!GetShoplog(data.logs,obj_if)) 
		//	return false;
		if(obj_if.IsTrashBoxModified())
		{
			data.data_mask |= SYNC_STOTEHOUSE;
			if(!GetStorehouse(data.storehouse,obj_if)) 
				return false;
		}
		if (!GetFuwenInventory(data.storehouse,obj_if)) 
			return false;
		return true;
	}

}
#endif
