#include <assert.h>
#include <octets.h>
#include "conf.h"
#include "thread.h"
#include "gamedbclient.hpp"
#include "getrole.hrp"
#include "putrole.hrp"
#include "getmoneyinventory.hrp"
#include "putmoneyinventory.hrp"
#include "gpet"
#include "gshoplog"
#include "db_if.h"
#include "glog.h"
#include "putspouse.hrp"
#include "dbsiegeget.hrp"
#include "dbsiegeputdetail.hrp"
#include "getcashtotal.hrp"
#include "addglobalcounter.hrp"
#include "gfactionbase"
#include "getfacbase.hrp"
#include "putfacbase.hrp"
#include "localmacro.h"

namespace GDB
{
	namespace
	{
		inline void push_back(GNET::Octets & os, const ivec & vec)
		{
			if(vec.size) os.insert(os.end(),vec.data,vec.size);
		}

		inline void get_buf(const GNET::Octets & os, ivec & vec)
		{
			if((vec.size = os.size()))
			{
				vec.data = os.begin();
			}
			else
			{
				vec.data = NULL;
			}
		}

		inline void get_loglist(const std::vector<GShopLog>& glog, loglist & list)
		{
			assert(list.list == NULL && list.count == 0);
			list.count = glog.size();
			list.list = NULL;
			if(list.count == 0) return;
			list.list = (shoplog*) malloc(sizeof(shoplog) * list.count);
			shoplog * pItem = list.list;
			for(size_t i = 0; i < list.count; i ++,pItem ++ )
			{
				const GShopLog &obj = glog[i]; 
				//pItem->roleid 	   = obj.roleid;
				pItem->order_id	   = obj.order_id;
				pItem->item_id 	   = obj.item_id;
				pItem->expire      = obj.expire;
				pItem->item_count  = obj.item_count;
				pItem->order_count = obj.order_count;
				pItem->cash_need   = obj.cash_need  ;
				pItem->time   = obj.time  ;
				pItem->guid1  = obj.guid1  ;
				pItem->guid2  = obj.guid2  ;
			}
		}

		inline void set_loglist(int roleid, std::vector<GShopLog>& glog, const loglist & list)
		{
			if(list.count == 0) return;
			const shoplog* p = list.list;
			glog.reserve(list.count);
			for(size_t i = 0; i < list.count; i ++,p++ )
			{
				GShopLog obj(roleid,p->order_id,p->item_id,p->expire,p->item_count,
					p->order_count,p->cash_need,p->time,p->guid1,p->guid2);
				glog.push_back(obj);
			}
		}

		inline void get_weborders(const WebOrderVector & db, std::vector<weborder> & list)
		{
			if (db.size() == 0)
				return;
			WebOrderVector::const_iterator it, ite = db.end();
			for (it = db.begin(); it != ite; ++it)
			{
				weborder order;
				order.orderid = it->orderid;
				order.userid = it->userid;
				order.roleid = it->roleid;
				order.paytype = it->paytype;
				order.status = it->status;
				order.timestamp = it->timestamp;

				WebMallFunctionVector::const_iterator fit, fite = it->functions.end();
				for (fit = it->functions.begin(); fit != fite; ++fit)
				{
					webmallfunc func;
					func.id = fit->function_id;
					//get_buf(fit->name, func.name);
					func.name = std::string((char*)fit->name.begin(), fit->name.size());
					func.count = fit->count;
					func.price = fit->price;

					WebMallGoodsVector::const_iterator git, gite = fit->goods.end();
					for (git = fit->goods.begin(); git != gite; ++git)
					{
						webmallgoods goods;
						goods.id = git->goods_id;
						goods.count = git->count;
						goods.flagmask = git->flagmask;
						goods.timelimit = git->timelimit;
						func.goods.push_back(goods);
					}
					order.funcs.push_back(func);
				}
				list.push_back(order);
			}
		}

		inline void get_itemlist(const GNET::GRoleInventoryVector & backpack, itemlist & list)
		{
			assert(list.list == NULL && list.count == 0);
			list.count = backpack.size();
			list.list = NULL;
			if(list.count == 0) return;
			list.list = (itemdata*) malloc(sizeof(itemdata) * list.count);
			itemdata * pItem = list.list;
			for(size_t i = 0; i < list.count; i ++,pItem ++ )
			{
				const GRoleInventory &obj = backpack[i]; 
				pItem->id 	= obj.id;
				pItem->index 	= obj.pos;
				pItem->count 	= obj.count;
				pItem->max_count= obj.max_count;
				pItem->guid1 	= obj.guid1;
				pItem->guid2 	= obj.guid2;
				pItem->proctype = obj.proctype;
				pItem->data	= obj.data.begin();
				pItem->size	= obj.data.size();
				pItem->expire_date = obj.expire_date;
				pItem->client_size = obj.client_size;
			}
		}

		inline void set_itemlist(GNET::GRoleInventoryVector & backpack, const itemlist & list)
		{
			if(list.count == 0) return;
			const itemdata * pItem = list.list;
			backpack.GetVector().reserve(list.count);
			for(size_t i = 0; i < list.count; i ++,pItem ++ )
			{
				GRoleInventory  obj(pItem->id,pItem->index,pItem->count, 0, pItem->max_count,0);
				obj.guid1 	= pItem->guid1;	
				obj.guid2 	= pItem->guid2; 	
				obj.proctype 	= pItem->proctype; 
				obj.expire_date = pItem->expire_date;
				obj.client_size = pItem->client_size;
				if(pItem->size) obj.data.replace(pItem->data,pItem->size);
				backpack.push_back(obj);
			}
		}

		inline 	void release_itemlist(itemlist & list)
		{
			if(list.count == 0) return;
			free(list.list);
			list.list = NULL;
			list.count = 0;
		}
		
		inline void get_itemlist(const GNET::GPocketInventoryVector& _pocket, pocket_itemlist & list)
		{
			assert(list.list == NULL && list.count == 0);
			list.count = _pocket.size();
			list.list = NULL;
			if(list.count == 0) return;
			list.list = (pocket_item*) malloc(sizeof(pocket_item) * list.count);
			pocket_item* pItem = list.list;
			for(size_t i = 0; i < list.count; i ++,pItem ++ )
			{
				const GPocketInventory &obj = _pocket[i]; 
				pItem->id 	= obj.id;
				pItem->index 	= obj.pos;
				pItem->count 	= obj.count;
			}
		}

		inline void set_itemlist(GNET::GPocketInventoryVector& _pocket, const pocket_itemlist & list)
		{
			if(list.count == 0) return;
			const pocket_item* pItem = list.list;
			_pocket.GetVector().reserve(list.count);
			for(size_t i = 0; i < list.count; i ++,pItem ++ )
			{
				GPocketInventory  obj(pItem->id,pItem->index,pItem->count);
				_pocket.push_back(obj);
			}
		}

		inline 	void release_itemlist(pocket_itemlist & list)
		{
			if(list.count == 0) return;
			free(list.list);
			list.list = NULL;
			list.count = 0;
		}
	};

	bool Role2Info(GNET::GRoleDetail * pRole,base_info & info,vecdata & data, GRoleInventoryVector &mountwing, int data_mask)
	{
		if(data_mask!=0x7F)
			return false;
		memset(&data,0,sizeof(data));
		info.id = pRole->status.id;
		info.userid = pRole->userid;
		info.cls = pRole->status.occupation;
		info.gender = pRole->gender;
		info.trashbox_active = ((data_mask&0x00000001)!=0);
		info.level = pRole->status.level;
		info.exp = pRole->status.exp;
		info.pp = pRole->status.pp;
		info.hp = pRole->status.hp;
		info.mp = pRole->status.mp;
		info.posx = pRole->status.posx;
		info.posy = pRole->status.posy;
		info.posz = pRole->status.posz;
		info.worldtag = pRole->status.worldtag;
		info.money = pRole->inventory.money;
		info.factionid = pRole->factionid;
		info.factionrole = pRole->title;
		info.reputation = pRole->status.reputation;
		info.contribution = pRole->status.contribution;
		info.combatkills = pRole->status.combatkills;
		info.devotion = pRole->status.devotion;
		info.talismanscore = pRole->status.talismanscore;
		info.battlescore = pRole->status.battlescore;
		info.spouse = pRole->spouse;
		info.jointime = pRole->jointime;
		info.familyid = pRole->familyid;
		info.sectid = pRole->sectid;
		info.initiallevel = pRole->initiallevel;
		info.cultivation = pRole->status.cultivation;

		info.bagsize = pRole->inventory.capacity;
		info.pocketsize = pRole->inventory.pocket_capacity;
		info.pkvalue = pRole->status.pkvalue;
		info.produceskill = pRole->status.produceskill;
		info.produceexp = pRole->status.produceexp;
		info.cash_add2 = pRole->cash_add2;
		info.cash_total = pRole->cash_total;
		info.cash_delta = 0;
		info.cash_used = pRole->cash_used;
		info.cash_serial = pRole->cash_serial;
		info.cash = info.cash_total - info.cash_used;
		info.cur_title = pRole->status.cur_title;

		info.time_used = pRole->status.time_used;
		info.create_time = pRole->create_time;
		info.storesize = pRole->storehouse.capacity;
		info.storesize2 = pRole->storehouse.capacity2;
		info.timestamp = pRole->inventory.timestamp;
		info.updatetime = pRole->status.updatetime;
		info.bonus_withdraw = pRole->base2.bonus_withdraw;
		info.bonus_reward = pRole->base2.bonus_reward;
		info.bonus_used = pRole->base2.bonus_used;
		info.referrer	= pRole->referrer;
		info.circleid	= pRole->circleid;
		info.circletitlemask = pRole->circletitlemask;
		info.src_zoneid = pRole->base2.src_zoneid;
		info.deity_level = pRole->base2.deity_level;
		info.deity_exp = pRole->base2.deity_exp;
		info.dp = pRole->base2.dp;
		info.flag_mask = pRole->base2.flag_mask;
		info.runescore = pRole->base2.runescore;
		info.fac_coupon_add = pRole->base2.datagroup[BASE2_DATA_FAC_COUPON_ADD];
		info.fac_coupon = pRole->base2.datagroup[BASE2_DATA_FAC_COUPON];
		info.used_title = pRole->base2.datagroup[BASE2_DATA_USED_TITLE];
		info.comsumption = pRole->base2.comsumption;

		get_buf(pRole->status.dbltime_data,data.dbltime_data);
		get_buf(pRole->name,data.user_name);
		get_buf(pRole->status.custom_status,data.custom_status);
		get_buf(pRole->status.filter_data,data.filter_data);
		get_buf(pRole->status.charactermode,data.charactermode);
		get_buf(pRole->status.instancekeylist,data.instancekeylist);
		get_buf(pRole->status.var_data,data.var_data);
		get_buf(pRole->status.skills,data.skill_data);
		get_buf(pRole->status.storehousepasswd,data.trashbox_passwd);
		get_buf(pRole->status.waypointlist,data.waypoint_list);
		get_buf(pRole->status.coolingtime,data.coolingtime);
		get_buf(pRole->status.recipes,data.recipes);
		get_buf(pRole->status.credit,data.region_reputation);
		get_buf(pRole->status.titlelist,data.title_list);
		get_buf(pRole->status.petdata,data.petdata);
		get_buf(pRole->status.reborndata,data.reborndata);
		get_buf(pRole->status.fashion_hotkey,data.fashion_hotkey);
		get_buf(pRole->status.raid_data,data.raid_data);
		get_buf(pRole->status.five_year, data.five_year);
		get_buf(pRole->status.treasure_info, data.treasure_info);
		get_buf(pRole->base2.tower_raid, data.tower_raid);
		get_buf(pRole->base2.ui_transfer, data.ui_transfer);
		get_buf(pRole->base2.collision_info, data.collision_info);
		get_buf(pRole->base2.astrology_info, data.astrology_info);
		get_buf(pRole->base2.liveness_info, data.liveness_info);
		get_buf(pRole->base2.sale_promotion_info, data.sale_promotion_info);
		get_buf(pRole->base2.propadd, data.propadd);
		get_buf(pRole->base2.multi_exp, data.multi_exp);
		get_buf(pRole->base2.fuwen_info, data.fuwen_info);
		get_buf(pRole->base2.phase, data.phase);
		get_buf(pRole->base2.award_info_6v6, data.award_info_6v6);
		get_buf(pRole->base2.hide_and_seek_info, data.hide_and_seek_info);
		get_buf(pRole->base2.newyear_award_info, data.newyear_award_info);

		get_buf(pRole->task.task_data,data.task_data);
		get_buf(pRole->task.task_complete,data.finished_task_data);
		get_buf(pRole->task.task_finishtime,data.finished_time_task_data);
		get_itemlist(pRole->inventory.items,data.inventory);
		get_itemlist(pRole->inventory.equipment,data.equipment);
		get_itemlist(pRole->inventory.petbadge,data.petbadge);
		get_itemlist(pRole->inventory.petequip,data.petequip);
		get_itemlist(pRole->task.task_inventory,data.task_inventory);
		get_itemlist(pRole->inventory.pocket_items, data.pocket);
		get_itemlist(pRole->inventory.fashion, data.fashion);

		info.mountwingsize = 0;
		if(pRole->inventory.mountwing.size())
		{
			Marshal::OctetsStream os_mountwing(pRole->inventory.mountwing);
			try
			{
				os_mountwing >> info.mountwingsize;
				os_mountwing >> mountwing;
			}
			catch(...)
			{
				Log::log(LOG_ERR, "Role2Info, error unmarshal, roleid=%d\n", info.id);
			}
		}
		get_itemlist(mountwing, data.mountwing);
		get_itemlist(pRole->inventory.gifts,data.gifts);
		get_itemlist(pRole->storehouse.fuwen,data.fuwen);

		get_buf(pRole->achievement.achieve_map,data.achieve_map);
		get_buf(pRole->achievement.achieve_active,data.achieve_active);
		get_buf(pRole->achievement.achieve_spec_info, data.achieve_spec_info);
		get_buf(pRole->achievement.achieve_award_map, data.achieve_award_map);
		get_buf(pRole->base2.composkills,data.composkills);
		get_buf(pRole->award.vipaward, data.vipaward);
		get_buf(pRole->award.onlineaward, data.onlineaward);
		get_buf(pRole->base2.littlepet,data.littlepet);
		//get_loglist(pRole->logs,data.logs);
		if(data_mask & GET_STOREHOUSE)
		{
			get_itemlist(pRole->storehouse.items,data.trash_box);
			get_itemlist(pRole->storehouse.items2,data.trash_box2);
			info.trash_money = pRole->storehouse.money;
		}
		get_weborders(pRole->weborders, data.weborders);
		return true;
	}

	void InventoryToItemList(const GNET::GRoleInventoryVector & backpack, itemlist & list)
	{
		return get_itemlist(backpack,list);
	}

	void ReleaseItemList( itemlist & list)
	{
		return release_itemlist(list);
	}
	

	void ReleaseAllInventory(vecdata & data)
	{
		release_itemlist(data.inventory);
		release_itemlist(data.equipment);
		release_itemlist(data.petequip);
		release_itemlist(data.petbadge);
		release_itemlist(data.task_inventory);
		release_itemlist(data.trash_box);
		release_itemlist(data.trash_box2);
		release_itemlist(data.pocket);
		release_itemlist(data.fashion);
		release_itemlist(data.mountwing);
		release_itemlist(data.gifts);
		release_itemlist(data.fuwen);
	}

	void Info2Role(GNET::GRoleDetail * pRole,const base_info & info,const vecdata & data)
	{
		pRole->id = info.id;
		pRole->userid = info.userid;
		pRole->status.id = info.id;
		pRole->gender = info.gender;
		pRole->status.level = info.level;
		pRole->status.occupation = info.cls;
		pRole->status.exp = info.exp;
		pRole->status.pp = info.pp;
		pRole->status.hp = info.hp;
		pRole->status.mp = info.mp;
		pRole->status.posx = info.posx;
		pRole->status.posy = info.posy;
		pRole->status.posz = info.posz;
		pRole->status.worldtag = info.worldtag;
		pRole->status.reputation = info.reputation;
		pRole->status.time_used = info.time_used;
		pRole->status.contribution = info.contribution;
		pRole->status.combatkills = info.combatkills;
		pRole->status.devotion = info.devotion;
		pRole->status.talismanscore = info.talismanscore;
		pRole->status.battlescore = info.battlescore;
		pRole->status.cultivation = info.cultivation;
		pRole->spouse = info.spouse;
		pRole->jointime = info.jointime;
		pRole->loginip = info.loginip;

		pRole->inventory.capacity = info.bagsize;
		pRole->inventory.money = info.money;
		pRole->inventory.timestamp = info.timestamp;
		pRole->storehouse.capacity = info.storesize;
		pRole->storehouse.capacity2 = info.storesize2;
		pRole->inventory.pocket_capacity = info.pocketsize;

		pRole->status.pkvalue = info.pkvalue;
		pRole->status.produceskill = info.produceskill;
		pRole->status.produceexp = info.produceexp;
		pRole->cash_used = info.cash_used - info.cash_delta;
		pRole->cash_serial = info.cash_serial;
		pRole->status.cur_title = info.cur_title;
		pRole->base2.id = info.id;
		pRole->base2.bonus_withdraw = info.bonus_withdraw;
		pRole->base2.bonus_reward = info.bonus_reward;
		pRole->base2.bonus_used = info.bonus_used;
	
		pRole->base2.deity_level = info.deity_level;
		pRole->base2.deity_exp = info.deity_exp;
		pRole->base2.dp = info.dp;
		pRole->base2.flag_mask = info.flag_mask;
		pRole->base2.runescore = info.runescore;
		pRole->base2.comsumption = info.comsumption;
		pRole->base2.datagroup[BASE2_DATA_FAC_COUPON_ADD]= info.fac_coupon_add;
		pRole->base2.datagroup[BASE2_DATA_FAC_COUPON]= info.fac_coupon;
		pRole->base2.datagroup[BASE2_DATA_USED_TITLE] = info.used_title;

		push_back(pRole->status.custom_status,data.custom_status);
		push_back(pRole->status.filter_data,data.filter_data);
		push_back(pRole->status.charactermode,data.charactermode);
		push_back(pRole->status.instancekeylist,data.instancekeylist);
		push_back(pRole->status.var_data,data.var_data);
		push_back(pRole->status.skills,data.skill_data);
		push_back(pRole->status.storehousepasswd,data.trashbox_passwd);
		push_back(pRole->status.waypointlist,data.waypoint_list);
		push_back(pRole->status.coolingtime,data.coolingtime);
		push_back(pRole->status.dbltime_data,data.dbltime_data);
		push_back(pRole->status.recipes,data.recipes);
		push_back(pRole->status.titlelist,data.title_list);
		push_back(pRole->status.credit,data.region_reputation);
		push_back(pRole->status.petdata,data.petdata);
		push_back(pRole->status.reborndata,data.reborndata);
		push_back(pRole->status.fashion_hotkey,data.fashion_hotkey);
		push_back(pRole->status.raid_data,data.raid_data);
		push_back(pRole->status.five_year, data.five_year);
		push_back(pRole->status.treasure_info, data.treasure_info);

		set_itemlist(pRole->inventory.items,data.inventory);
		set_itemlist(pRole->inventory.equipment,data.equipment);
		set_itemlist(pRole->inventory.petbadge,data.petbadge);
		set_itemlist(pRole->inventory.petequip,data.petequip);
		set_itemlist(pRole->inventory.pocket_items,data.pocket);
		set_itemlist(pRole->inventory.fashion,data.fashion);

		GNET::GRoleInventoryVector mountwing;
		set_itemlist(mountwing, data.mountwing);
		Marshal::OctetsStream os_mountwing;
		try
		{
			os_mountwing << info.mountwingsize;
			os_mountwing << mountwing;
			pRole->inventory.mountwing = os_mountwing;
		}
		catch(...)
		{
			Log::log(LOG_ERR, "Info2Role, error marshal roleid=%d\n", info.id);
		}
		set_itemlist(pRole->inventory.gifts,data.gifts);
		set_itemlist(pRole->storehouse.fuwen,data.fuwen);

		push_back(pRole->task.task_data,data.task_data);
		push_back(pRole->task.task_complete,data.finished_task_data);
		push_back(pRole->task.task_finishtime,data.finished_time_task_data);
		set_itemlist(pRole->task.task_inventory,data.task_inventory);
		set_loglist(info.id,pRole->logs,data.logs);
		if(info.trashbox_active)
		{
			set_itemlist(pRole->storehouse.items,data.trash_box);
			set_itemlist(pRole->storehouse.items2,data.trash_box2);
			pRole->storehouse.money = info.trash_money;
		}
		push_back(pRole->achievement.achieve_map,data.achieve_map);
		push_back(pRole->achievement.achieve_active,data.achieve_active);
		push_back(pRole->achievement.achieve_spec_info, data.achieve_spec_info);
		push_back(pRole->achievement.achieve_award_map, data.achieve_award_map);
		push_back(pRole->base2.composkills,data.composkills);
		push_back(pRole->award.vipaward, data.vipaward);
		push_back(pRole->award.onlineaward, data.onlineaward);
		push_back(pRole->base2.littlepet,data.littlepet);
		push_back(pRole->base2.tower_raid, data.tower_raid);
		push_back(pRole->base2.ui_transfer, data.ui_transfer);
		push_back(pRole->base2.collision_info, data.collision_info);
		push_back(pRole->base2.astrology_info, data.astrology_info);
		push_back(pRole->base2.liveness_info, data.liveness_info);
		push_back(pRole->base2.sale_promotion_info, data.sale_promotion_info);
		push_back(pRole->base2.propadd, data.propadd);
		push_back(pRole->base2.multi_exp, data.multi_exp);
		push_back(pRole->base2.fuwen_info, data.fuwen_info);
		push_back(pRole->base2.phase, data.phase);
		push_back(pRole->base2.award_info_6v6, data.award_info_6v6);
		push_back(pRole->base2.hide_and_seek_info, data.hide_and_seek_info);
		push_back(pRole->base2.newyear_award_info, data.newyear_award_info);

		pRole->processed_weborders = data.processed_weborders;
	}

	size_t
	convert_item(const GRoleInventoryVector & ivec, itemdata * list , size_t size)
	{
		if(ivec.size() == 0) return 0;
		size_t count = ivec.size();
		if(count > size) count = size;
		for(size_t i = 0; i < count; i ++)
		{
			itemdata * pItem = list + i;
			const GRoleInventory &obj = ivec[i]; 
			pItem->id 	= obj.id;
			pItem->index 	= obj.pos;
			pItem->count 	= obj.count;
			pItem->max_count= obj.max_count;
			pItem->guid1 	= obj.guid1;
			pItem->guid2 	= obj.guid2;
			pItem->proctype = obj.proctype;
			pItem->expire_date = obj.expire_date;
			pItem->client_size = obj.client_size;
			pItem->data	= obj.data.begin();
			pItem->size	= obj.data.size();
		}
		return count;
	}

	bool GetFBase(int fid, GNET::GFactionBase * db, fac_base_info & info)
	{
		info.fid = fid;
		info.grass = db->grass;
		info.mine = db->mine;
		info.monster_food = db->monster_food;
		info.monster_core = db->monster_core;
		info.cash = db->cash;
		info.timestamp = db->timestamp;
//		info.msg = std::string((char*)db->msg.begin(), db->msg.size());
		get_buf(db->msg, info.msg);

		GFactionFieldVector::const_iterator it, ite = db->fields.end();
		for (it = db->fields.begin(); it != ite; ++it)
		{
			fac_field field;
			field.index = it->index;
			field.tid = it->tid;
			field.level = it->level;
			field.status = it->status;
			field.task_id = it->task_id;
			field.task_count = it->task_count;
			field.task_need = it->task_need;
			info.fields.push_back(field);
		}

		GFactionAuctionVector::const_iterator ait, aite = db->auctions.end();
		for (ait = db->auctions.begin(); ait != aite; ++ait)
		{
			fac_auction auc;
			auc.timestamp = ait->timestamp;
			auc.itemid = ait->itemid;
			auc.winner = ait->winner;
			auc.cost = ait->cost; 
			//auc.winner_name = std::string((char*)ait->rolename.begin(), ait->rolename.size());
			get_buf(ait->rolename, auc.winner_name);
			auc.status = ait->status;
			auc.end_time = ait->end_time;
			info.auctions.push_back(auc);
		}
		GFactionAuctionFailerVector::const_iterator fit, fite = db->auction_failers.end();
		for (fit = db->auction_failers.begin(); fit != fite; ++fit)
		{
			fac_auc_failer failer;
			failer.roleid = fit->roleid;
			failer.return_coupon = fit->return_coupon;
			info.auction_failers.push_back(failer);
		}
		GFactionAuctionHistoryVector::const_iterator hit, hite = db->auction_history.end();
		for (hit = db->auction_history.begin(); hit != hite; ++hit)
		{
			fac_auc_history history;
			history.event_type = hit->event_type;
//			history.rolename = std::string((char*)hit->rolename.begin(), hit->rolename.size());
			get_buf(hit->rolename, history.rolename);
			history.cost = hit->cost;
			history.itemid = hit->itemid;
			info.auction_history.push_back(history);
		}
		return true;
	}

	bool SaveFBase(GNET::GFactionBase * db, const fac_base_info & info)
	{
		db->grass = info.grass;
		db->mine = info.mine;
		db->monster_food = info.monster_food;
		db->monster_core = info.monster_core;
		db->cash = info.cash;
		db->timestamp = info.timestamp;
//		db->msg = Octets(info.msg.c_str(), info.msg.length());
		push_back(db->msg, info.msg);

		std::vector<fac_field>::const_iterator it, ite = info.fields.end();
		for (it = info.fields.begin(); it != ite; ++it)
		{
			GFactionField field;
			field.index = it->index;
			field.tid = it->tid;
			field.level = it->level;
			field.status = it->status;
			field.task_id = it->task_id;
			field.task_count = it->task_count;
			field.task_need = it->task_need;
			db->fields.push_back(field);
		}

		std::vector<fac_auction>::const_iterator ait, aite = info.auctions.end();
		for (ait = info.auctions.begin(); ait != aite; ++ait)
		{
			GFactionAuction auc(ait->timestamp, ait->itemid, ait->winner, ait->cost,
					Octets(), ait->status, ait->end_time);
			push_back(auc.rolename, ait->winner_name);
			db->auctions.push_back(auc);
		}
		std::vector<fac_auc_failer>::const_iterator fit, fite = info.auction_failers.end();
		for (fit = info.auction_failers.begin(); fit != fite; ++fit)
		{
			GFactionAuctionFailer failer(fit->roleid, fit->return_coupon);
			db->auction_failers.push_back(failer);
		}
		std::vector<fac_auc_history>::const_iterator hit, hite = info.auction_history.end();
		for (hit = info.auction_history.begin(); hit != hite; ++hit)
		{
			GFactionAuctionHistory history(hit->event_type, Octets(), hit->cost, hit->itemid);
			push_back(history.rolename, hit->rolename);
			db->auction_history.push_back(history);
		}
		return true;
	}

	bool put_role(int line_id, int id,const base_info* pInfo,const vecdata* pData,Result* callback,int priority,int mask)
	{
		RolePair arg;
		arg.key.id = id;
		arg.priority = priority;
		arg.data_mask = mask;
		arg.line_id = line_id;

		if(!pInfo->trashbox_active)
			arg.data_mask ^= QPUT_STOREHOUSE;
		if(!pInfo->cash_active)
			arg.data_mask ^= QPUT_CASH;

		Info2Role(&arg.value,*pInfo,*pData);
		Rpc *rpc = Rpc::Call(RPC_PUTROLE, &arg);
		((PutRole *)rpc)->_callback = callback;
		return GamedbClient::GetInstance()->SendProtocol(*rpc);
	}
	
	bool get_cash_total(int roleid, Result *callback)
	{
		GetCashTotal *rpc = (GetCashTotal *) Rpc::Call(RPC_GETCASHTOTAL, RoleId(roleid));
		rpc->_callback = callback;
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if(success) PollIO::WakeUp();
		return success;
	}
	bool put_counter_drop(int template_id, int start_time, int duration, int max_count, CountDropResult* result)
	{
		AddGlobalCounter *rpc = (AddGlobalCounter *) Rpc::Call(RPC_ADDGLOBALCOUNTER, AddGlobalCounterArg(template_id,start_time,duration,max_count));
		rpc->_result = result;
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if(success) PollIO::WakeUp();
		return success;
	}
	bool get_role(int line_id, int id, Result * callback)
	{
		RoleArg arg( RoleId((unsigned int )id) , 0x0000007F, line_id);
		Rpc *rpc = Rpc::Call(RPC_GETROLE, &arg);
		((GetRole *)rpc)->_callback = callback;
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if(success) PollIO::WakeUp();
		return success;
	}
	bool get_faction_base(int fid, FBaseResult * callback)
	{
		GetFacBaseArg arg(fid);
		Rpc * rpc = Rpc::Call(RPC_GETFACBASE, &arg);
		((GetFacBase *)rpc)->_callback = callback;
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if (success) PollIO::WakeUp();
		return success;
	}
	bool put_faction_base(int faction_id, const fac_base_info & info, FBaseResult * callback)
	{
		PutFacBaseArg arg(faction_id);
		SaveFBase(&(arg.value), info);
		Rpc * rpc = Rpc::Call(RPC_PUTFACBASE, &arg);
		((PutFacBase *)rpc)->_callback = callback;
		return GamedbClient::GetInstance()->SendProtocol(*rpc);

	}
	bool put_money_inventory(int id, size_t money, itemlist & list,Result * callback)
	{
		PutMoneyInventoryArg arg;
		arg.roleid = id;
		arg.money = money;
		set_itemlist(arg.goods,list);

		Rpc *rpc = Rpc::Call(RPC_PUTMONEYINVENTORY, &arg);
		((PutMoneyInventory*)rpc)->_callback = callback;
		return GamedbClient::GetInstance()->SendProtocol(*rpc);
	}

	bool get_money_inventory(int id, Result * callback)
	{
		Rpc *rpc = Rpc::Call(RPC_GETMONEYINVENTORY, GetMoneyInventoryArg(id));
		((GetMoneyInventory*)rpc)->_callback = callback;
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if(success) PollIO::WakeUp();
		return success;

	}

	bool set_couple(int id1, int id2, int op)
	{
		Rpc *rpc = Rpc::Call(RPC_PUTSPOUSE, PutSpouseArg(op,id1,id2));
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if(success) PollIO::WakeUp();
		return success;
	}

	bool init_gamedb()
	{
		Conf *conf = Conf::GetInstance();
		GamedbClient *manager = GamedbClient::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		Protocol::Client(manager);
		return true;
	}

	bool put_city_detail(int battle_id,int stamp,const void* buf,size_t size,City_Op_Result* callback)
	{
		Octets temp(buf,size);
		DBSiegePutDetailArg arg(battle_id,stamp,temp);
		Rpc* rpc = Rpc::Call(RPC_DBSIEGEPUTDETAIL,&arg);
		((DBSiegePutDetail*)rpc)->_callback = callback;
		return GamedbClient::GetInstance()->SendProtocol(*rpc);
		return true;
	}

	bool get_city_detail(int battle_id,City_Op_Result* callback)
	{
		DBSiegeGetArg arg(battle_id);
		Rpc* rpc = Rpc::Call(RPC_DBSIEGEGET,&arg);
		((DBSiegeGet*)rpc)->_callback = callback;
		bool success = GamedbClient::GetInstance()->SendProtocol(*rpc);
		if(success) PollIO::WakeUp();
		return success;
	}
};

