#include "marshal.h"
#include "rpc.h"
#include "conv_charset.h"
#include "ggroupinfo"
#include "gfriendinfo"
#include "genemyinfo"
#include "gmember"
#include "factionid"
#include "gfactioninfo"
#include "guserfaction"
#include "gfriendlist"
#include "gshoplog"
#include "grolestatus"
#include "gmailheader"
#include "gmail"
#include "gmailbox"
#include "groleinventory"
#include "grolestorehouse"
#include "groleforbid"
#include "grolebase"
#include "gauctionitem"
#include "gauctiondetail"
#include "grolepocket"
#include "groletask"
#include "stocklog"
#include "user"
#include "gtopitem"
#include "gtoptable"
#include "stockorder"
#include "storage.h"
#include "dbbuffer.h"
#include "databrowser.h"

using namespace GNET;
extern iconv_t _iconvu2g;
extern iconv_t _iconvg2u;

typedef void (*dumpper_t)(FILE* fp, GNET::Marshal::OctetsStream& data);
typedef std::map<std::string, dumpper_t> DumpperMap;
DumpperMap dumppers;
extern void hexdump(FILE* fp, const char* data, int len);
extern int dec2hex(const char* data, int slen, char* dst, int dlen);
extern unsigned char hex2dec(unsigned char c);
extern int hex2dec(const char* data, int len, char* dst, int dlen);
extern int IconvOctets(Octets& src, Octets& dst, iconv_t cd);

void DumpFriends(FILE* fp, Marshal::OctetsStream& data)
{
	GFriendList list;
	data >> list;
	fprintf(fp, "[friends]\n");
	for(size_t i=0;i<list.friends.size();i++)
	{
		Octets name;
		IconvOctets(list.friends[i].name,name,_iconvu2g);
		fprintf(fp, "%10d  %2d  %-.*s\n", list.friends[i].rid, list.friends[i].lineid, name.size(), (char*)name.begin());
	}
	fprintf(fp, "[enemies]\n");
	for(size_t i=0;i<list.enemies.size();i++)
	{
		Octets name;
		IconvOctets(list.enemies[i].name,name,_iconvu2g);
		fprintf(fp, "%10d  %2d %d %-.*s", list.enemies[i].rid, list.enemies[i].status, list.enemies[i].pktime,
			name.size(), (char*)name.begin());
	}
}
void DumpFactionInfo(FILE* fp, Marshal::OctetsStream& data)
{
	char buf[1024];
	GFactionInfo info;
	data >> info;
	size_t len, slen, dlen = 1024;
	char *in,*out;

	fprintf(fp, "[members]\n");
	for(size_t i=0;i<info.member.size();i++)
	{
		fprintf(fp, "    roleid         %d\n", info.member[i].fid);
	}
	fprintf(fp, "[faction info]\n");
	dlen = 1024;
	slen = info.name.size();
	in = (char*)(info.name.begin());
	out = buf;
	len = iconv(_iconvu2g,&in,&slen,&out,&dlen);
	if (((size_t)-1)==len)
	{
		printf("ERROR: conversion failed\n");
		perror("iconv");
		return;
	}
	
	fprintf(fp, "factionid       %d\n"
		    "name            %-.*s\n"
		    "size            %d\n"
		    "level           %d\n"
		    "master          %d\n"
		    "prosperity      %d\n"
		    "population      %d\n"
		    "contribution    %d\n"
		    "createtime      %s\n",
		    info.fid, 1024-dlen, buf, info.member.size(), info.level, info.master, info.prosperity,
		    info.population, info.contribution, ctime((time_t*)&(info.createtime)));
}

void DumpStatus(FILE* fp, Marshal::OctetsStream& data)
{
	GRoleStatus info;
	data >> info;
	fprintf(fp, "id            %-d\n"
		    "size          %-d\n"
		    "level         %-d\n"
		    "exp           %-d\n"
		    "hp            %-d\n"
		    "mp            %-d\n"
		    "skill         %-d\n" 
		    "recipes       %-d\n" 
		    "cooling       %-d\n" 
		    "filter        %-d\n"
		    "vardata       %-d\n" 
		    "titlelist     %-d\n" 
		    "credit        %-d\n" 
		    "contribution  %-d\n",
		info.id, data.size(), info.level, info.exp, info.hp, info.mp, 
		info.skills.size(), info.recipes.size(), info.coolingtime.size(), info.filter_data.size(), 
		info.var_data.size(), info.titlelist.size(), info.credit.size(), info.contribution);
}

void DumpMail(FILE* fp, Marshal::OctetsStream& data)
{
	GMailBox box;
	data >> box;
	char buf[1024];
	for(size_t i=0;i<box.mails.size();i++)
	{
		size_t dlen = 1024;
		size_t slen = box.mails[i].header.sender_name.size();
		char *in = (char*)(box.mails[i].header.sender_name.begin());
		char *out = buf;
		size_t len;
		if(box.mails[i].header.sndr_type==0)
		{
			len = iconv(_iconvu2g,&in,&slen,&out,&dlen);
			if (((size_t)-1)==len)
			{
				printf("ERROR: conversion failed\n");
				perror("iconv");
				return;
			}
		}
		else
			dlen = 1024;
		fprintf(fp, "From: %-.*s(%d)\nSend_time: %s", 
				1024-dlen, buf, box.mails[i].header.sender, 
				ctime((time_t*)&(box.mails[i].header.send_time)));
		dlen = 1024;
		slen = box.mails[i].header.title.size();
		in = (char*)(box.mails[i].header.title.begin());
		out = buf;
		if(box.mails[i].header.sndr_type==0)
		{
			len = iconv(_iconvu2g,&in,&slen,&out,&dlen);
			if (((size_t)-1)==len)
			{
				printf("ERROR: conversion failed\n");
				perror("iconv");
				return;
			}
		}
		else
			dlen = 1024;
		fprintf(fp, "Read_time: %sID(%d) Status(%d)\nSubject: %-.*s\n",
			ctime((time_t*)&(box.mails[i].header.receiver)),  
			box.mails[i].header.id, box.mails[i].header.attribute, 1024-dlen, buf);
		fprintf(fp, "Money: %d\nItem: %d %d\n\n",box.mails[i].attach_money, box.mails[i].attach_obj.id, 
			box.mails[i].attach_obj.count);
	}
}

void DumpBin(FILE* fp, Marshal::OctetsStream& data)
{
	fwrite(data.begin(), 1, data.size(), fp);
}

void DumpItems(FILE* fp, Marshal::OctetsStream& data)
{
	GRolePocket v;
	data >> v;
	//fprintf(fp, "size        %d\n", data.size());
	fprintf(fp, "capacity:%d;", v.capacity);
	fprintf(fp, "timestamp:%d;", v.timestamp);
	fprintf(fp, "money:%d;", v.money);
	fprintf(fp, "pos_used:%d", v.items.size());
	/*
	if(v.items.size())
	{
		fprintf(fp, "\n[inventory]\n id  pos  count  size\n");
		for(unsigned int i=0;i<v.items.size();i++)
		{
			fprintf(fp, "%-5d %-2d   %-4d  %-3d\n", v.items[i].id, v.items[i].pos, v.items[i].count, 
				v.items[i].data.size());
		}
	}
	if(v.equipment.size())
	{
		fprintf(fp, "\n[equipment]\n id  pos  count  size\n");
		for(unsigned int i=0;i<v.equipment.size();i++)
		{
			fprintf(fp, "%-5d %-2d   %-4d  %-3d\n", v.equipment[i].id, v.equipment[i].pos, v.equipment[i].count, 
				v.equipment[i].data.size());
		}
	}
	*/
	fprintf(fp, "\n");
}

void DumpUser(FILE* fp, Marshal::OctetsStream& data)
{
	User s;
	data >> s;

	fprintf(fp, "logicuid:%d;", s.logicuid);
	fprintf(fp, "roles:%x;", s.rolelist);
	fprintf(fp, "cash:%d;", s.cash);
	fprintf(fp, "money:%d;", s.money);
	fprintf(fp, "cash_add:%d;", s.cash_add);
	fprintf(fp, "cash_buy:%d;", s.cash_buy);
	fprintf(fp, "cash_sell:%d;", s.cash_sell);
	fprintf(fp, "cash_used:%d;", s.cash_used);
	fprintf(fp, "add_serial:%d;", s.add_serial);
	fprintf(fp, "use_serial:%d;", s.use_serial);
	fprintf(fp, "exg_log:%d;", s.exg_log.size());
	fprintf(fp, "cash_pass:%d;", s.cash_password.size());
	fprintf(fp, "autolock:%d;", s.autolock.size());
	fprintf(fp, "status:%d;", s.status);
	fprintf(fp, "forbid:%d;", s.forbid.size());
	fprintf(fp, "\n");
}

void DumpTop(FILE* fp, Marshal::OctetsStream& data)
{
	GTopTable s;
	data >> s;
	char buf[1024];

	for(unsigned int i=0;i<s.items.size();i++)
	{
		Octets name;
		IconvOctets(s.items[i].name,name,_iconvu2g);
		fprintf(fp, "  %-10d  %-20.*s %-lld %-8d %-3d\n", s.items[i].id, name.size(), (char*)name.begin(), buf, s.items[i].value,
				s.items[i].occupation, s.items[i].oldrank);
	}
	fprintf(fp, "\nid           %d\n", s.id);
	fprintf(fp, "maxsize      %d\n", s.maxsize);
	fprintf(fp, "size         %d\n", s.items.size());
	fprintf(fp, "time         %s\n", ctime((time_t*)&s.time));
}

void DumpTask(FILE* fp, Marshal::OctetsStream& data)
{
	GRoleTask s;
	data >> s;

	fprintf(fp, "size        %d\n", data.size());
	fprintf(fp, "data        %d\n", s.task_data.size());
	fprintf(fp, "complete    %d\n", s.task_complete.size());
	fprintf(fp, "time        %d\n", s.task_finishtime.size());
	if(s.task_inventory.size())
	{
		fprintf(fp, "\n id   pos  count  size\n");
		for(unsigned int i=0;i<s.task_inventory.size();i++)
		{
			fprintf(fp, "%-5d  %-2d    %-4d   %-3d\n", s.task_inventory[i].id, s.task_inventory[i].pos, 
				s.task_inventory[i].count, s.task_inventory[i].data.size());
		}
	}
}

void DumpDepot(FILE* fp, Marshal::OctetsStream& data)
{
	GRoleStorehouse s;
	data >> s;

	//fprintf(fp, "size:%d", data.size());
	fprintf(fp, "capacity:%d;", s.capacity);
	fprintf(fp, "money:%d;", s.money);
	fprintf(fp, "pos_used:%d;", s.items.size());
	/*
	fprintf(fp, "\n id   pos  count  size\n");
	for(unsigned int i=0;i<s.items.size();i++)
	{
		fprintf(fp, "%-5d  %-2d    %-4d   %-3d\n", s.items[i].id, s.items[i].pos, s.items[i].count, 
				s.items[i].data.size());
	}
	*/
	fprintf(fp, "\n");
}
void DumpBase(FILE* fp, Marshal::OctetsStream& data)
{
	GRoleBase s;
	data >> s;

	Octets name;
	IconvOctets(s.name,name,_iconvu2g);
	fprintf(fp, "id           %-d\n"
		    "name         %-.*s\n"
		    "userid       %-d\n"
		    "size         %-d\n"
		    "config       %-d\n"
		    "help         %-d\n"
		    "spouse       %-d\n"
		    "jointime     %-s\n",
		    s.id, name.size(),(char*)name.begin(), s.userid, data.size(), 
		    s.config_data.size(), s.help_states.size(), 
		    s.spouse, ctime((time_t*)&(s.jointime)));
}

void DumpBase2(FILE* fp, Marshal::OctetsStream& data)
{
	GRoleBase2 s;
	data >> s;

	fprintf(fp, "id:%d;", s.id);
	fprintf(fp, "consumption:%d;", s.comsumption);
	fprintf(fp, "\n");
}

void DumpOrder(FILE* fp, Marshal::OctetsStream& data)
{
	StockOrder s;
	data >> s;

	fprintf(fp, "tid      %-8d\ntime     %-8d\nroleid   %-8d\nprice    %-8d\nvolume   %-8d\nstatus   %-4d\n",
		 s.tid, s.time, s.userid, s.price, s.volume, s.status);
}
void DumpName(FILE* fp, Marshal::OctetsStream& data)
{
	int roleid;
	data >> roleid;
	fprintf(fp, "roleid=%d\n", roleid);
}
void DumpLog(FILE* fp, Marshal::OctetsStream& data)
{
	std::vector<GShopLog> s;
	data >> s;

	fprintf(fp, "roleid     order_id item_id expire     item_count order_count cost time       guid1    guid2\n");
	for(std::vector<GShopLog>::iterator it=s.begin(),ie=s.end();it!=ie;++it)
	{
		fprintf(fp, "%-11d%-9d%-8d%-11d%-11d%-12d%-5d%-11d%-8x %-4x\n", it->roleid, it->order_id, it->item_id, 
			it->expire, it->item_count, it->order_count, it->cash_need, it->time, it->guid1, it->guid2);
	}
}
void InitDumpType()
{
	dumppers.insert(std::make_pair("bin",          DumpBin));
	dumppers.insert(std::make_pair("base",         DumpBase));
	dumppers.insert(std::make_pair("gfriendlist",  DumpFriends));
	dumppers.insert(std::make_pair("friends",      DumpFriends));
	dumppers.insert(std::make_pair("grolestatus",  DumpStatus));
	dumppers.insert(std::make_pair("status",       DumpStatus));
	dumppers.insert(std::make_pair("gfactioninfo", DumpFactionInfo));
	dumppers.insert(std::make_pair("faction",      DumpFactionInfo));
	dumppers.insert(std::make_pair("factioncache",      DumpFactionInfo));
	dumppers.insert(std::make_pair("gmailbox",     DumpMail));
	dumppers.insert(std::make_pair("mailbox",      DumpMail));
	dumppers.insert(std::make_pair("inventory",    DumpItems));
	dumppers.insert(std::make_pair("task",         DumpTask));
	dumppers.insert(std::make_pair("gtask",         DumpTask));
	dumppers.insert(std::make_pair("storehouse",   DumpDepot));
	dumppers.insert(std::make_pair("user",         DumpUser));
	dumppers.insert(std::make_pair("top",          DumpTop));
	dumppers.insert(std::make_pair("order",        DumpOrder));
	dumppers.insert(std::make_pair("log",          DumpLog));
	dumppers.insert(std::make_pair("shoplog",      DumpLog));
	dumppers.insert(std::make_pair("rolename",     DumpName));
	dumppers.insert(std::make_pair("base2",     DumpBase2));
}

void DumpData(FILE* fp, const char* type, Octets& data)
{
	try{
		Marshal::OctetsStream copy(data);
		DumpperMap::iterator it = dumppers.find(type);
		if(it==dumppers.end())
		{
			printf("Unrecognized data type\n");
			return;
		}
		(it->second)(fp, copy);
	}
	catch(...)
	{
		printf("Dump exception: data size %d\n", data.size());
	}
}
