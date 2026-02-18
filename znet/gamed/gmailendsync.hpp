
#ifndef __GNET_GMAILENDSYNC_HPP
#define __GNET_GMAILENDSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "groleinventory"
#include "gmailsyncdata"
#include "../gdbclient/db_if.h"
#include "../include/localmacro.h"

void player_end_sync(int role_id, unsigned int money, const GDB::itemlist & item_change_list, bool storesaved, bool logsaved);
void player_cancel_sync(int role_id);
void player_cash_notify(int role_id, int cash_plus_used);
namespace GNET
{

class GMailEndSync : public GNET::Protocol
{
	#include "gmailendsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(retcode==ERR_SUCCESS)
		{
			GDB::itemdata ar[288];
			if(syncdata.inventory.items.size() > 288)
			{
				Log::log(LOG_ERR,"gamed:: invalid syncdata roleid=%d", roleid);
				return;
			}
			GDB::itemlist list;
			list.list = ar;
			list.count = GDB::convert_item(syncdata.inventory.items,ar,288);
			if(syncdata.data_mask&SYNC_CASHTOTAL)
				player_cash_notify(roleid, syncdata.cash_total);
			bool storesaved = syncdata.data_mask&SYNC_STOTEHOUSE;
			bool logsaved   = syncdata.data_mask&SYNC_SHOPLOG;
			player_end_sync(roleid, syncdata.inventory.money, list, storesaved, logsaved);
		}
		else
		{
			player_cancel_sync(roleid);
			Log::log(LOG_ERR,"gamed:: gmailendsync return errcode,  roleid=%d errcode=%d", roleid, retcode);
		}
	}
};

};

#endif
