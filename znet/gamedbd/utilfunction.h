#ifndef __UTIL_FUNCTION_H
#define __UTIL_FUNCTION_H

#include "gmailheader"
#include "gmail"
#include "gmailbox"
#include "gmember"
#include "gfolk"
#include "gmailsyncdata"
#include "gfamilyskill"
#include "gfamily"
#include "familyid"
#include "hostilefaction"
#include "hostileinfo"
#include "gfactioninfo"
#include "localmacro.h"

namespace GNET
{
void UpdateCash(StorageEnv::Storage *puser, GMailSyncData& data, StorageEnv::Transaction& txn);
void UpdateStore(int roleid, StorageEnv::Storage *pstore, GMailSyncData& data, StorageEnv::Transaction& txn, int money_delta);
void UpdateShoplog(StorageEnv::Storage *plog, GMailSyncData& data, StorageEnv::Transaction& txn);
void PutSyslog(StorageEnv::Storage *plog,StorageEnv::Transaction& txn,int roleid,int ip, GRoleInventory& inv);
void PutSyslog(StorageEnv::Storage *plog,StorageEnv::Transaction& txn,int roleid,int ip, int money, GRoleInventoryVector& invs);
bool DecPocketItemInPos(GRoleInventoryVector & list, int pos, int itemid, int count, GRoleInventoryVector & change_items);
class MailSender
{
public:
	const static unsigned int MAILBOX_MAIL_LIMITS = 128;
	static bool NextId(GMailBox& box, unsigned char & id)
	{
		size_t size = box.mails.size();
		if(size > MAILBOX_MAIL_LIMITS)
			return false;
		id = 0;
		if(size) 
			id = box.mails[size-1].header.id+1;
		for(GMailVector::iterator it=box.mails.begin(),ie=box.mails.end();it!=ie;it++)
		{
			if(it->header.id==id)
			{
				id++;
				it = box.mails.begin();
			}
		}
		return true;
	}
	
	/*
	 * return: -1 send fail, 0 send success;
	 */
	static int Send(StorageEnv::Storage * pmailbox, GMailHeader& header,GRoleInventory& inv, unsigned int money, 
			StorageEnv::Transaction& txn)
	{
		Marshal::OctetsStream key, value;
		GMailBox box;
		time_t now = Timer::GetTime();
		GMail mail;
		mail.attach_money = 0;
		mail.attach_obj = inv;
		key << header.receiver;
		if(pmailbox->find(key,value,txn))
			value >> box;
		else
			box.timestamp = now;
		if(box.mails.size()>MAILBOX_MAIL_LIMITS)
			return -1;
		if(!NextId(box,header.id))
			return -1;
		header.attribute = (1<<_MA_UNREAD);
		if(header.sndr_type == _MST_LOCAL_CONSIGN)
		{
			header.attribute ^= (1<<_MA_PRESERVE);
		}
		header.send_time = now;
		if(inv.count>0)
		{
			mail.attach_obj = inv;
			header.attribute ^= (1<<_MA_ATTACH_OBJ);
		}
		if(money>0)
		{
			mail.attach_money = money;
			header.attribute ^= (1<<_MA_ATTACH_MONEY);
		}
						
		Log::formatlog("mail","type=send:src=%d:dst=%d:mid=%d:size=%d:money=%d:item=%d:count=%d:pos=%d",
			   	header.sender, header.receiver, header.id, 0, 0, mail.attach_obj.id, 
				mail.attach_obj.count, 0);

		mail.header = header;
		mail.header.receiver = now;
		box.mails.push_back(mail);
		pmailbox->insert( key, Marshal::OctetsStream()<<box, txn );
		return 0;
	}
	
	/*
	 * return: -1 send fail, 0 send success;
	 */
	static int Send(StorageEnv::Storage* pmailbox,GMailHeader& header,unsigned int money,StorageEnv::Transaction& txn)
	{
		Marshal::OctetsStream key, value;
		GMailBox box;
		time_t now = Timer::GetTime();
		GMail mail;
		mail.attach_money = money;
		mail.attach_obj.id = 0;
		mail.attach_obj.count = 0;
		mail.attach_obj.pos = -1;
		key << header.receiver;
		if(pmailbox->find(key,value,txn))
			value >> box;
		else
			box.timestamp = now;
		if(box.mails.size()>MAILBOX_MAIL_LIMITS)
			return -1;
		if(!NextId(box,header.id))
			return -1;
		header.attribute = (1<<_MA_UNREAD); 
		if((header.sndr_type == _MST_LOCAL_CONSIGN) || (header.sndr_type ==_MST_FACTION_PK_BONUS_NOTIFY))
		{
			header.attribute ^= (1<<_MA_PRESERVE);
		}
		header.send_time = now;
		if(money>0)
		{
			mail.attach_money = money;
			header.attribute ^= (1<<_MA_ATTACH_MONEY);
		}
		Log::formatlog("mail","type=send:src=%d:dst=%d:mid=%d:size=%d:money=%d:item=%d:count=%d:pos=%d",
			   	header.sender, header.receiver, header.id, 0, money, 0, 0, -1);
		mail.header = header;
		mail.header.receiver = now;
		box.mails.push_back(mail);
		pmailbox->insert( key, Marshal::OctetsStream()<<box, txn );
		return 0;
	}


	/*
	 * return: -1 send fail, 0 send success;
	 */
	static int SendMaster(StorageEnv::Storage* pmailbox,StorageEnv::Storage* pcache,GMailHeader& header, 
			unsigned int fid, unsigned int money, StorageEnv::Transaction& txn)
	{
		Marshal::OctetsStream key, value;
		GMailBox box;
		time_t now = Timer::GetTime();
		GMail mail;
		GFactionInfo info;
		if( pcache->find( Marshal::OctetsStream()<<fid, value, txn ) )
			value >> info;
		else 
			return -1;
		int roleid = info.master;

		if(!roleid)
			return -1;

		header.receiver = roleid;
		mail.attach_money = 0;
		key << header.receiver;
		value.clear();
		if(pmailbox->find(key,value,txn))
			value >> box;
		else
			box.timestamp = now;
		if(box.mails.size()>MAILBOX_MAIL_LIMITS)
			return -1;
		if(!NextId(box,header.id))
			return -1;
		header.attribute = (1<<_MA_UNREAD); 
		header.send_time = now;
		if(money>0)
		{
			mail.attach_money = money;
			header.attribute ^= (1<<_MA_ATTACH_MONEY);
		}
						
		Log::formatlog("battlemail","faction=%d:src=%d:dst=%d:mid=%d:size=%d:money=%d:item=%d:count=%d:"
				"pos=%d", fid, header.sender, header.receiver, header.id, 0, money, 
				mail.attach_obj.id, mail.attach_obj.count, 0);

		mail.header = header;
		mail.header.receiver = now;
		box.mails.push_back(mail);
		pmailbox->insert( key, Marshal::OctetsStream()<<box, txn );
		return 0;
	}

	//item_count 不会大于 max_count
	static bool DBSendObj(GMailBox & box, int itemid, int proc_type, int max_count, int item_count, int mailtype)
	{
		unsigned char mailid = 0;
		if (box.mails.size() >= SYSMAIL_LIMIT || !NextId(box, mailid))
			return false;
		if (item_count > max_count)
		{
			Log::log(LOG_ERR, "DBSendObj item_count %d is greater than max_count %d, itemid = %d",
					item_count, max_count, itemid);
			item_count = max_count;
		}
		if (item_count <= 0)
		{
			Log::log(LOG_ERR, "DBSendObj item_count %d invalid, itemid = %d",
					item_count, itemid);
			return false;
		}
		time_t now = time(NULL);
		if (box.timestamp == 0)
			box.timestamp = now;
		GMail mail;
		mail.header.id = mailid;
		mail.header.sender = 0;
		mail.header.sndr_type = mailtype;
		mail.header.receiver = now;
		//mail.header.title;
		mail.header.send_time = now;
		mail.header.attribute = (1<<_MA_UNREAD)|(1<<_MA_ATTACH_OBJ)|(1<<_MA_PRESERVE);
		static char sys_name[] = {'G',0,'M',0};
		mail.header.sender_name = Octets(sys_name, 4);
		mail.attach_obj.id = itemid;
		mail.attach_obj.pos = 0;
		mail.attach_obj.count = item_count;
		mail.attach_obj.max_count = max_count;
		mail.attach_obj.proctype = proc_type;
		mail.attach_money = 0;

		box.mails.push_back(mail);
		return true;
	}
};
};
#endif
