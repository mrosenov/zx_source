#ifndef __XMLCODER_H
#define __XMLCODER_H

#include <string>
#include <vector>
#include <set>
#include "rpcdefs.h"
#include "grolenamehis"
#include "guniquewinitem"
#include "clearroledata"
#include "gtoptable"
#include "gapplyinfo"
#include "grolebase"
#include "gsnsroleinfo"
#include "gfactioninfo"
#include "guniqueauction"
#include "gsnsplayerinfo"
#include "gtchallenge"
#include "gserviceforbidcmdlist"
#include "greferral"
#include "gfolk"
#include "gfriendinfo"
#include "gtopdetail"
#include "gfamilyskill"
#include "gkingdomperiodlimit"
#include "stocklog"
#include "gtabletopflowerdata"
#include "hostileinfo"
#include "gconsigndb"
#include "gsectskill"
#include "user"
#include "gsnsmessage"
#include "grolestorehouse"
#include "hostilefaction"
#include "genemyinfo"
#include "gcircledb"
#include "groletask"
#include "gterritorystore"
#include "ggtactivate"
#include "gtabledefinition"
#include "gfactionfield"
#include "groleaward"
#include "guniquebidder"
#include "grolestatus"
#include "gfactionbase"
#include "gcallbackaward"
#include "gcrssvrteamsinfo"
#include "groletableclsconfig"
#include "webmallgoods"
#include "gpair"
#include "gdisciple"
#include "gfactionauction"
#include "webmallfunction"
#include "gcooldown"
#include "gfamily"
#include "gmailheader"
#include "gfactionauctionhistory"
#include "weborder"
#include "gterritoryinfo"
#include "gfriendlist"
#include "gmail"
#include "gfriendcallback"
#include "guniquepriceinfo"
#include "gcallbackinfo"
#include "gconsignitem"
#include "gleavemessage"
#include "gpocketinventory"
#include "gkingdomfunctionary"
#include "gkingdomhistory"
#include "gsect"
#include "gkingdominfo"
#include "gcrssvrteamsscore"
#include "gtopitem"
#include "gfactionauctionfailer"
#include "serviceforbidcmd"
#include "gsnsplayerinfocache"
#include "grolebase2"
#include "message"
#include "gcrssvrteamsdata"
#include "guniquewinner"
#include "groleachievement"
#include "stockorder"
#include "gcirclememberdb"
#include "gapplycondition"
#include "grolepocket"
#include "gsyslog"
#include "groledata"
#include "groleforbid"
#include "groleinventory"
#include "gmailbox"
#include "familyid"
#include "ggroupinfo"
#include "gcrssvrteamsrole"
#include "gtaskdata"

namespace GNET
{

class XmlCoder
{
protected:
	std::string  data;
	Octets       buffer;
	std::set<unsigned short> entities; 
public:
	XmlCoder() : buffer(32) 
	{ 
		entities.insert(0);
		entities.insert(34);
		entities.insert(38);
		entities.insert(39);
		entities.insert(60);
		entities.insert(62);
	}
	const char * c_str() { return data.c_str(); }

	void append_header()
	{
		data = "<?xml version=\"1.0\"?>\n";
	}
	void append_variable(const char* name, const char* type, const std::string& value)
	{
		data = data + "<variable name=\"" + name + "\" type=\"" + type + "\">" + value + "</variable>\n";
	}
	const std::string toString(char x)
	{
		sprintf((char*)buffer.begin(), "%d", (int)x);
		return (char*)buffer.begin();
	}
	const std::string toString(short x)
	{
		sprintf((char*)buffer.begin(), "%d", (int)x);
		return (char*)buffer.begin();
	}
	const std::string toString(int x)
	{
		sprintf((char*)buffer.begin(), "%d", x);
		return (char*)buffer.begin();
	}
	const std::string toString(float x)
	{
		sprintf((char*)buffer.begin(), "%.9g", x);
		return (char*)buffer.begin();
	}
	const std::string toString(int64_t x)
	{
		sprintf((char*)buffer.begin(), "%lld", x);
		return (char*)buffer.begin();
	}
	const std::string toString(const Octets& x)
	{
		buffer.resize(x.size()*2+1);
		unsigned char* p = (unsigned char*)x.begin();
		char* out = (char*)buffer.begin();
		*out = 0;
		for(size_t i=0;i<x.size();++i,out+=2)
		{
			sprintf(out,"%02x", p[i]);
		}
		return (char*)buffer.begin();
	}
	void append_string(const char* name, const Octets& x)
	{
		std::string result;
		const unsigned short *p = (const unsigned short*)x.begin();
		for(size_t len = x.size()/2;len>0;len--,p++)
		{
			unsigned short c = *p;
			if(c>0x7F || entities.find(c)!=entities.end()) 
				result.append("&#" + toString(c) + ";");
			else
				result += (char)c;
		}
		append_variable(name,"Octets",result);
	}
	void append(const char* name, char x)
	{
		append_variable(name, "byte", toString(x));
	}
	void append(const char* name, unsigned char x)
	{
		append(name, (char) x);
	}
	void append(const char* name, short x)
	{
		append_variable(name, "short", toString(x));
	}
	void append(const char* name, unsigned short x)
	{
		append(name, (short) x);
	}
	void append(const char* name, int x)
	{
		append_variable(name, "int", toString(x));
	}
	void append(const char* name, unsigned int x)
	{
		append(name, (int) x);
	}
	void append(const char* name, float x)
	{
		append_variable(name, "float", toString(x));
	}
	void append(const char* name, int64_t x)
	{
		append_variable(name, "long", toString(x));
	}
	void append(const char* name, const Octets& x)
	{
		append_variable(name, "Octets", toString(x));
	}
	template<typename T>
	void append(const char* name, const std::vector<T> &x) 
	{
		typedef const std::vector<T> VECTOR;
		for( typename VECTOR::const_iterator i=x.begin(),e=x.end();i!=e;++i)
			append(name, *i);
	}
	template<typename T>
	void append(const char* name, const std::list<T> &x) 
	{
		typedef const std::list<T> LIST;
		for( typename LIST::const_iterator i=x.begin(),e=x.end();i!=e;++i)
			append(name, *i);
	}
	template<typename T>
	void append(const char* name, const GNET::RpcDataVector<T> &x) 
	{
		typedef const GNET::RpcDataVector<T> VECTOR;
		for(typename VECTOR::const_iterator i=x.begin(),e=x.end();i!=e;++i)
			append(name, *i);
	}
	template<typename T>
	void append(const char* name, const std::set<T> &x)
	{
		for (typename std::set<T>::const_iterator i=x.begin(),e=x.end();i!=e;++i)
			append(name, *i);
	}
	template<typename K, typename V>
	void append(const char* name, const std::pair<K, V> &x)
	{
		data = data + "<" + name + ">\n";
		append("__first", x.first);
		append("__second", x.first);
		data = data + "</" + name + ">\n";
	}
	template<typename K, typename V>
	void append(const char* name, const std::map<K, V> &x)
	{
		for (typename std::map<K, V>::const_iterator i=x.begin(),e=x.end();i!=e;++i)
			append(name, *i);
	}
	void append(const char* name, const GRoleNameHis& x)
	{
		data = data + "<" + name + ">\n";
		append_string("name",  x.name);
		append("endtime",  x.endtime);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GUniqueWinItem& x)
	{
		data = data + "<" + name + ">\n";
		append("itemid",  x.itemid);
		append("bid_min",  x.bid_min);
		append("reserve1",  x.reserve1);
		append("reserve2",  x.reserve2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const ClearRoleData& x)
	{
		data = data + "<" + name + ">\n";
		append("time",  x.time);
		append("pocket",  x.pocket);
		append("storehouse",  x.storehouse);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTopTable& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("maxsize",  x.maxsize);
		append("time",  x.time);
		append("items",  x.items);
		append("weekly",  x.weekly);
		append("items_detail",  x.items_detail);
		append("reserved",  x.reserved);
		append("reserved1",  x.reserved1);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GApplyInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("rolename",  x.rolename);
		append("count",  x.count);
		append("last_time",  x.last_time);
		append("level",  x.level);
		append("gender",  x.gender);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleBase& x)
	{
		data = data + "<" + name + ">\n";
		append("version",  x.version);
		append("id",  x.id);
		append_string("name",  x.name);
		append("faceid",  x.faceid);
		append("hairid",  x.hairid);
		append("gender",  x.gender);
		append("status",  x.status);
		append("delete_time",  x.delete_time);
		append("create_time",  x.create_time);
		append("lastlogin_time",  x.lastlogin_time);
		append("familyid",  x.familyid);
		append("title",  x.title);
		append("config_data",  x.config_data);
		append("help_states",  x.help_states);
		append("forbid",  x.forbid);
		append("spouse",  x.spouse);
		append("jointime",  x.jointime);
		append("userid",  x.userid);
		append("sectid",  x.sectid);
		append("initiallevel",  x.initiallevel);
		append("earid",  x.earid);
		append("tailid",  x.tailid);
		append("circletrack",  x.circletrack);
		append("fashionid",  x.fashionid);
		append("datagroup",  x.datagroup);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSNSRoleInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("level",  x.level);
		append("sub_level",  x.sub_level);
		append("gender",  x.gender);
		append("occupation",  x.occupation);
		append("spouse",  x.spouse);
		append("rolename",  x.rolename);
		append("sectname",  x.sectname);
		append("familyname",  x.familyname);
		append("factionname",  x.factionname);
		append("factiontitle",  x.factiontitle);
		append("faction_level",  x.faction_level);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		append("reserved6",  x.reserved6);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFactionInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("fid",  x.fid);
		append_string("name",  x.name);
		append("announce",  x.announce);
		append("level",  x.level);
		append("member",  x.member);
		append("master",  x.master);
		append("prosperity",  x.prosperity);
		append("createtime",  x.createtime);
		append("deletetime",  x.deletetime);
		append("population",  x.population);
		append("contribution",  x.contribution);
		append("status",  x.status);
		append("nimbus",  x.nimbus);
		append("hostileinfo",  x.hostileinfo);
		append("charm",  x.charm);
		append("changenametime",  x.changenametime);
		append("namehis",  x.namehis);
		append("pk_bonus",  x.pk_bonus);
		append("dynamic",  x.dynamic);
		append("datagroup",  x.datagroup);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GUniqueAuction& x)
	{
		data = data + "<" + name + ">\n";
		append("pricelist",  x.pricelist);
		append("itemtobid",  x.itemtobid);
		append("winners",  x.winners);
		append("winner",  x.winner);
		append("win_item",  x.win_item);
		append("reserve1",  x.reserve1);
		append("reserve2",  x.reserve2);
		append("reserve3",  x.reserve3);
		append("reserve4",  x.reserve4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSNSPlayerInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("is_private",  x.is_private);
		append("rolename",  x.rolename);
		append("nickname",  x.nickname);
		append("age",  x.age);
		append("birthday",  x.birthday);
		append("province",  x.province);
		append("city",  x.city);
		append("career",  x.career);
		append("address",  x.address);
		append("introduce",  x.introduce);
		append("charm_personal",  x.charm_personal);
		append("charm_friend",  x.charm_friend);
		append("charm_sect",  x.charm_sect);
		append("charm_faction",  x.charm_faction);
		append("press_time",  x.press_time);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		append("reserved6",  x.reserved6);
		append("reserved7",  x.reserved7);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTChallenge& x)
	{
		data = data + "<" + name + ">\n";
		append("factionid",  x.factionid);
		append("itemcount",  x.itemcount);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GServiceForbidCmdList& x)
	{
		data = data + "<" + name + ">\n";
		append("version",  x.version);
		append("cmdlist",  x.cmdlist);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GReferral& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("bonus_total1",  x.bonus_total1);
		append("bonus_total2",  x.bonus_total2);
		append("bonus_withdraw",  x.bonus_withdraw);
		append("exp_total1",  x.exp_total1);
		append("exp_total2",  x.exp_total2);
		append("exp_withdraw",  x.exp_withdraw);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFolk& x)
	{
		data = data + "<" + name + ">\n";
		append("rid",  x.rid);
		append_string("name",  x.name);
		append("nickname",  x.nickname);
		append("level",  x.level);
		append("title",  x.title);
		append("occupation",  x.occupation);
		append("contribution",  x.contribution);
		append("jointime",  x.jointime);
		append("devotion",  x.devotion);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFriendInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("rid",  x.rid);
		append("lineid",  x.lineid);
		append("gid",  x.gid);
		append_string("name",  x.name);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTopDetail& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("content",  x.content);
		append("value1",  x.value1);
		append("value2",  x.value2);
		append("value3",  x.value3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFamilySkill& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("level",  x.level);
		append("ability",  x.ability);
		append("reserved",  x.reserved);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GKingdomPeriodLimit& x)
	{
		data = data + "<" + name + ">\n";
		append("func_type",  x.func_type);
		append("period_type",  x.period_type);
		append("period_times",  x.period_times);
		append("last_timestamp",  x.last_timestamp);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const StockLog& x)
	{
		data = data + "<" + name + ">\n";
		append("tid",  x.tid);
		append("time",  x.time);
		append("result",  x.result);
		append("volume",  x.volume);
		append("cost",  x.cost);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTableTopFlowerData& x)
	{
		data = data + "<" + name + ">\n";
		append("userid",  x.userid);
		append("rolename",  x.rolename);
		append("gender",  x.gender);
		append("recv_count",  x.recv_count);
		append("recv_stamp",  x.recv_stamp);
		append("recv_isGiftTake",  x.recv_isGiftTake);
		append("send_count",  x.send_count);
		append("send_stamp",  x.send_stamp);
		append("send_isGiftTake",  x.send_isGiftTake);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const HostileInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("updatetime",  x.updatetime);
		append("actionpoint",  x.actionpoint);
		append("protecttime",  x.protecttime);
		append("status",  x.status);
		append("hostiles",  x.hostiles);
		append("reserved1",  x.reserved1);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GConsignDB& x)
	{
		data = data + "<" + name + ">\n";
		append("info",  x.info);
		append("item",  x.item);
		append("post_time",  x.post_time);
		append("game_timestamp",  x.game_timestamp);
		append("largest_web_timestamp",  x.largest_web_timestamp);
		append("margin",  x.margin);
		append("loginip",  x.loginip);
		append("seller_roleid",  x.seller_roleid);
		append("seller_userid",  x.seller_userid);
		append("seller_name",  x.seller_name);
		append("buyer_roleid",  x.buyer_roleid);
		append("buyer_userid",  x.buyer_userid);
		append("buyer_name",  x.buyer_name);
		append("orderid",  x.orderid);
		append("category",  x.category);
		append("mail_status",  x.mail_status);
		append("commodityid",  x.commodityid);
		append("shelf_time",  x.shelf_time);
		append("gs_roleinfo",  x.gs_roleinfo);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSectSkill& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("level",  x.level);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const User& x)
	{
		data = data + "<" + name + ">\n";
		append("logicuid",  x.logicuid);
		append("rolelist",  x.rolelist);
		append("cash",  x.cash);
		append("money",  x.money);
		append("cash_add",  x.cash_add);
		append("cash_buy",  x.cash_buy);
		append("cash_sell",  x.cash_sell);
		append("cash_used",  x.cash_used);
		append("add_serial",  x.add_serial);
		append("use_serial",  x.use_serial);
		append("exg_log",  x.exg_log);
		append("cash_password",  x.cash_password);
		append("autolock",  x.autolock);
		append("status",  x.status);
		append("forbid",  x.forbid);
		append("referrer",  x.referrer);
		append("flag",  x.flag);
		append("lastlogin_ip",  x.lastlogin_ip);
		append("data_group",  x.data_group);
		append("unprocessed_orders",  x.unprocessed_orders);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSNSMessage& x)
	{
		data = data + "<" + name + ">\n";
		append("message_id",  x.message_id);
		append("message_type",  x.message_type);
		append("roleid",  x.roleid);
		append("rolename",  x.rolename);
		append("gender",  x.gender);
		append("factionid",  x.factionid);
		append("charm",  x.charm);
		append("message",  x.message);
		append("press_time",  x.press_time);
		append("condition",  x.condition);
		append("leave_msgs",  x.leave_msgs);
		append("apply_list",  x.apply_list);
		append("agree_list",  x.agree_list);
		append("deny_list",  x.deny_list);
		append("accept_list",  x.accept_list);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		append("reserved6",  x.reserved6);
		append("reserved7",  x.reserved7);
		append("reserved8",  x.reserved8);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleStorehouse& x)
	{
		data = data + "<" + name + ">\n";
		append("capacity",  x.capacity);
		append("money",  x.money);
		append("items",  x.items);
		append("capacity2",  x.capacity2);
		append("items2",  x.items2);
		append("fuwen",  x.fuwen);
		append("reserved3",  x.reserved3);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const HostileFaction& x)
	{
		data = data + "<" + name + ">\n";
		append("fid",  x.fid);
		append_string("name",  x.name);
		append("addtime",  x.addtime);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GEnemyInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("rid",  x.rid);
		append("lineid",  x.lineid);
		append("status",  x.status);
		append("pktime",  x.pktime);
		append_string("name",  x.name);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCircleDB& x)
	{
		data = data + "<" + name + ">\n";
		append("circleid",  x.circleid);
		append("circlename",  x.circlename);
		append("master_rid",  x.master_rid);
		append("member",  x.member);
		append("circlegrade",  x.circlegrade);
		append("circleversion",  x.circleversion);
		append("taskpoint",  x.taskpoint);
		append("createtime",  x.createtime);
		append("tasksavetime",  x.tasksavetime);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		append("reserved6",  x.reserved6);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleTask& x)
	{
		data = data + "<" + name + ">\n";
		append("task_data",  x.task_data);
		append("task_complete",  x.task_complete);
		append("task_finishtime",  x.task_finishtime);
		append("task_inventory",  x.task_inventory);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTerritoryStore& x)
	{
		data = data + "<" + name + ">\n";
		append("status",  x.status);
		append("tlist",  x.tlist);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GGTActivate& x)
	{
		data = data + "<" + name + ">\n";
		append("activatemask",  x.activatemask);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTableDefinition& x)
	{
		data = data + "<" + name + ">\n";
		append("user",  x.user);
		append("base",  x.base);
		append("status",  x.status);
		append("task",  x.task);
		append("inventory",  x.inventory);
		append("storehouse",  x.storehouse);
		append("mailbox",  x.mailbox);
		append("friends",  x.friends);
		append("messages",  x.messages);
		append("family",  x.family);
		append("faction",  x.faction);
		append("sect",  x.sect);
		append("order",  x.order);
		append("gtask",  x.gtask);
		append("citystore",  x.citystore);
		append("syslog",  x.syslog);
		append("top",  x.top);
		append("config",  x.config);
		append("factionname",  x.factionname);
		append("familyname",  x.familyname);
		append("waitdel",  x.waitdel);
		append("clsconfig",  x.clsconfig);
		append("rolename",  x.rolename);
		append("shoplog",  x.shoplog);
		append("auction",  x.auction);
		append("commondata",  x.commondata);
		append("snsplayerinfo",  x.snsplayerinfo);
		append("snsmessage",  x.snsmessage);
		append("base2",  x.base2);
		append("referral",  x.referral);
		append("clrrole",  x.clrrole);
		append("achievement",  x.achievement);
		append("hometown",  x.hometown);
		append("topflower",  x.topflower);
		append("kingdom",  x.kingdom);
		append("crssvrteams",  x.crssvrteams);
		append("crssvrteamsname",  x.crssvrteamsname);
		append("uniqueauction",  x.uniqueauction);
		append("circle",  x.circle);
		append("consign",  x.consign);
		append("finished_consign",  x.finished_consign);
		append("gtactivate",  x.gtactivate);
		append("friendcallback",  x.friendcallback);
		append("award",  x.award);
		append("dropcounter",  x.dropcounter);
		append("rolenamehis",  x.rolenamehis);
		append("operationcmd",  x.operationcmd);
		append("weborder",  x.weborder);
		append("factionbase",  x.factionbase);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFactionField& x)
	{
		data = data + "<" + name + ">\n";
		append("index",  x.index);
		append("tid",  x.tid);
		append("level",  x.level);
		append("status",  x.status);
		append("task_id",  x.task_id);
		append("task_count",  x.task_count);
		append("task_need",  x.task_need);
		append("reserved11",  x.reserved11);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleAward& x)
	{
		data = data + "<" + name + ">\n";
		append("vipaward",  x.vipaward);
		append("onlineaward",  x.onlineaward);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GUniqueBidder& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("rolename",  x.rolename);
		append("bidtime",  x.bidtime);
		append("reserve1",  x.reserve1);
		append("reserve2",  x.reserve2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleStatus& x)
	{
		data = data + "<" + name + ">\n";
		append("version",  x.version);
		append("id",  x.id);
		append("occupation",  x.occupation);
		append("level",  x.level);
		append("cur_title",  x.cur_title);
		append("exp",  x.exp);
		append("pp",  x.pp);
		append("hp",  x.hp);
		append("mp",  x.mp);
		append("posx",  x.posx);
		append("posy",  x.posy);
		append("posz",  x.posz);
		append("pkvalue",  x.pkvalue);
		append("worldtag",  x.worldtag);
		append("time_used",  x.time_used);
		append("reputation",  x.reputation);
		append("produceskill",  x.produceskill);
		append("produceexp",  x.produceexp);
		append("custom_status",  x.custom_status);
		append("filter_data",  x.filter_data);
		append("charactermode",  x.charactermode);
		append("instancekeylist",  x.instancekeylist);
		append("dbltime_data",  x.dbltime_data);
		append("petcorral",  x.petcorral);
		append("var_data",  x.var_data);
		append("skills",  x.skills);
		append("storehousepasswd",  x.storehousepasswd);
		append("coolingtime",  x.coolingtime);
		append("recipes",  x.recipes);
		append("waypointlist",  x.waypointlist);
		append("credit",  x.credit);
		append("titlelist",  x.titlelist);
		append("contribution",  x.contribution);
		append("combatkills",  x.combatkills);
		append("devotion",  x.devotion);
		append("talismanscore",  x.talismanscore);
		append("updatetime",  x.updatetime);
		append("battlescore",  x.battlescore);
		append("petdata",  x.petdata);
		append("reborndata",  x.reborndata);
		append("cultivation",  x.cultivation);
		append("reserved1",  x.reserved1);
		append("fashion_hotkey",  x.fashion_hotkey);
		append("raid_data",  x.raid_data);
		append("five_year",  x.five_year);
		append("treasure_info",  x.treasure_info);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFactionBase& x)
	{
		data = data + "<" + name + ">\n";
		append("activity",  x.activity);
		append("act_uptime",  x.act_uptime);
		append("status_mask",  x.status_mask);
		append("grass",  x.grass);
		append("mine",  x.mine);
		append("monster_food",  x.monster_food);
		append("monster_core",  x.monster_core);
		append("cash",  x.cash);
		append("fields",  x.fields);
		append("msg",  x.msg);
		append("auctions",  x.auctions);
		append("auction_failers",  x.auction_failers);
		append("createtime",  x.createtime);
		append("auction_history",  x.auction_history);
		append("reserved8",  x.reserved8);
		append("reserved9",  x.reserved9);
		append("timestamp",  x.timestamp);
		append("reserved11",  x.reserved11);
		append("reserved12",  x.reserved12);
		append("reserved13",  x.reserved13);
		append("reserved14",  x.reserved14);
		append("reserved15",  x.reserved15);
		append("reserved16",  x.reserved16);
		append("reserved17",  x.reserved17);
		append("reserved18",  x.reserved18);
		append("reserved19",  x.reserved19);
		append("reserved20",  x.reserved20);
		append("reserved21",  x.reserved21);
		append("reserved22",  x.reserved22);
		append("reserved23",  x.reserved23);
		append("reserved24",  x.reserved24);
		append("reserved25",  x.reserved25);
		append("reserved26",  x.reserved26);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCallbackAward& x)
	{
		data = data + "<" + name + ">\n";
		append("returnplayer_list",  x.returnplayer_list);
		append("recaller_list",  x.recaller_list);
		append("returnaward_num",  x.returnaward_num);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCrssvrTeamsInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("createtime",  x.createtime);
		append("zoneid",  x.zoneid);
		append("teamid",  x.teamid);
		append("master",  x.master);
		append_string("name",  x.name);
		append("score",  x.score);
		append("battle_losses ",  x.battle_losses );
		append("battle_draws ",  x.battle_draws );
		append("battle_wins ",  x.battle_wins );
		append("changenametime",  x.changenametime);
		append("weekbattletimes",  x.weekbattletimes);
		append("lastbattletime",  x.lastbattletime);
		append("seasonlastbattletime",  x.seasonlastbattletime);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleTableClsconfig& x)
	{
		data = data + "<" + name + ">\n";
		append("version",  x.version);
		append("base",  x.base);
		append("status",  x.status);
		append("inventory",  x.inventory);
		append("storehouse",  x.storehouse);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const WebMallGoods& x)
	{
		data = data + "<" + name + ">\n";
		append("goods_id",  x.goods_id);
		append("count",  x.count);
		append("flagmask",  x.flagmask);
		append("timelimit",  x.timelimit);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GPair& x)
	{
		data = data + "<" + name + ">\n";
		append("key",  x.key);
		append("value",  x.value);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GDisciple& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append_string("name",  x.name);
		append("level",  x.level);
		append("occupation",  x.occupation);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFactionAuction& x)
	{
		data = data + "<" + name + ">\n";
		append("timestamp",  x.timestamp);
		append("itemid",  x.itemid);
		append("winner",  x.winner);
		append("cost",  x.cost);
		append("rolename",  x.rolename);
		append("status",  x.status);
		append("end_time",  x.end_time);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const WebMallFunction& x)
	{
		data = data + "<" + name + ">\n";
		append("function_id",  x.function_id);
		append_string("name",  x.name);
		append("count",  x.count);
		append("price",  x.price);
		append("price_before_discount",  x.price_before_discount);
		append("goods",  x.goods);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCooldown& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("send_time",  x.send_time);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFamily& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append_string("name",  x.name);
		append("master",  x.master);
		append("factionid",  x.factionid);
		append("member",  x.member);
		append("skills",  x.skills);
		append("task_record",  x.task_record);
		append("task_data",  x.task_data);
		append("announce",  x.announce);
		append("createtime",  x.createtime);
		append("jointime",  x.jointime);
		append("deletetime",  x.deletetime);
		append("changenametime",  x.changenametime);
		append("namehis",  x.namehis);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GMailHeader& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("sender",  x.sender);
		append("sndr_type",  x.sndr_type);
		append("receiver",  x.receiver);
		append("title",  x.title);
		append("send_time",  x.send_time);
		append("attribute",  x.attribute);
		append("sender_name",  x.sender_name);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFactionAuctionHistory& x)
	{
		data = data + "<" + name + ">\n";
		append("event_type",  x.event_type);
		append("rolename",  x.rolename);
		append("cost",  x.cost);
		append("itemid",  x.itemid);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const WebOrder& x)
	{
		data = data + "<" + name + ">\n";
		append("orderid",  x.orderid);
		append("userid",  x.userid);
		append("roleid",  x.roleid);
		append("paytype",  x.paytype);
		append("functions",  x.functions);
		append("status",  x.status);
		append("timestamp",  x.timestamp);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTerritoryInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("owner",  x.owner);
		append("occupy_time",  x.occupy_time);
		append("color",  x.color);
		append("challengelist",  x.challengelist);
		append("defender",  x.defender);
		append("success_challenge",  x.success_challenge);
		append("success_award",  x.success_award);
		append("start_time",  x.start_time);
		append("assis_drawn_num",  x.assis_drawn_num);
		append("rand_award_itemid",  x.rand_award_itemid);
		append("rand_award_itemcount",  x.rand_award_itemcount);
		append("rand_award_drawn",  x.rand_award_drawn);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		append("reserved6",  x.reserved6);
		append("reserved7",  x.reserved7);
		append("reserved8",  x.reserved8);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFriendList& x)
	{
		data = data + "<" + name + ">\n";
		append("groups",  x.groups);
		append("friends",  x.friends);
		append("enemies",  x.enemies);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GMail& x)
	{
		data = data + "<" + name + ">\n";
		append("header",  x.header);
		append("context",  x.context);
		append("attach_obj",  x.attach_obj);
		append("attach_money",  x.attach_money);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFriendCallback& x)
	{
		data = data + "<" + name + ">\n";
		append("callback_info",  x.callback_info);
		append("callback_award",  x.callback_award);
		append("reserved",  x.reserved);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GUniquePriceInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("money",  x.money);
		append("bidderlist",  x.bidderlist);
		append("reserve1",  x.reserve1);
		append("reserve2",  x.reserve2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCallbackInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("mailnum",  x.mailnum);
		append("timestamp",  x.timestamp);
		append("is_subscribe",  x.is_subscribe);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("cooldown_list",  x.cooldown_list);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GConsignItem& x)
	{
		data = data + "<" + name + ">\n";
		append("sn",  x.sn);
		append("state",  x.state);
		append("fin_type",  x.fin_type);
		append("consign_type",  x.consign_type);
		append("money",  x.money);
		append("item_id",  x.item_id);
		append("item_cnt",  x.item_cnt);
		append("post_endtime",  x.post_endtime);
		append("show_endtime",  x.show_endtime);
		append("sell_endtime",  x.sell_endtime);
		append("price",  x.price);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GLeaveMessage& x)
	{
		data = data + "<" + name + ">\n";
		append("index",  x.index);
		append("dst_index",  x.dst_index);
		append("type",  x.type);
		append("roleid",  x.roleid);
		append("rolename",  x.rolename);
		append("time",  x.time);
		append("message",  x.message);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GPocketInventory& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("pos",  x.pos);
		append("count",  x.count);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GKingdomFunctionary& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("title",  x.title);
		append("rolename",  x.rolename);
		append("privileges_mask",  x.privileges_mask);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GKingdomHistory& x)
	{
		data = data + "<" + name + ">\n";
		append("faction",  x.faction);
		append("points",  x.points);
		append("win_times",  x.win_times);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSect& x)
	{
		data = data + "<" + name + ">\n";
		append_string("name",  x.name);
		append("disciples",  x.disciples);
		append("skills",  x.skills);
		append("reputation",  x.reputation);
		append("cooldown",  x.cooldown);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GKingdomInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("king",  x.king);
		append("kinggender",  x.kinggender);
		append("queen",  x.queen);
		append("kingdomname",  x.kingdomname);
		append("kingfaction",  x.kingfaction);
		append("kingfacname",  x.kingfacname);
		append("announce",  x.announce);
		append("points",  x.points);
		append("win_times",  x.win_times);
		append("occupy_time",  x.occupy_time);
		append("reserved",  x.reserved);
		append("functionaries",  x.functionaries);
		append("limits",  x.limits);
		append("history",  x.history);
		append("taskstatus",  x.taskstatus);
		append("tasktype",  x.tasktype);
		append("taskissuetime",  x.taskissuetime);
		append("taskpoints",  x.taskpoints);
		append("reward_mask",  x.reward_mask);
		append("reserved6",  x.reserved6);
		append("reserved7",  x.reserved7);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCrssvrTeamsScore& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("ct_max_score",  x.ct_max_score);
		append("ct_last_max_score",  x.ct_last_max_score);
		append("ct_score_updatetime",  x.ct_score_updatetime);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTopItem& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append_string("name",  x.name);
		append("occupation",  x.occupation);
		append("oldrank",  x.oldrank);
		append("value",  x.value);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GFactionAuctionFailer& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("return_coupon",  x.return_coupon);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const ServiceForbidCmd& x)
	{
		data = data + "<" + name + ">\n";
		append("cmdtype",  x.cmdtype);
		append("arglist",  x.arglist);
		append("dst",  x.dst);
		append("timestamp",  x.timestamp);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSNSPlayerInfoCache& x)
	{
		data = data + "<" + name + ">\n";
		append("roleinfo",  x.roleinfo);
		append("playerinfo",  x.playerinfo);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleBase2& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("bonus_withdraw",  x.bonus_withdraw);
		append("bonus_reward",  x.bonus_reward);
		append("bonus_used",  x.bonus_used);
		append("exp_withdraw_today",  x.exp_withdraw_today);
		append("exp_withdraw_time",  x.exp_withdraw_time);
		append("composkills",  x.composkills);
		append("tower_raid",  x.tower_raid);
		append("deity_level",  x.deity_level);
		append("data_timestamp",  x.data_timestamp);
		append("src_zoneid",  x.src_zoneid);
		append("deity_exp",  x.deity_exp);
		append("dp",  x.dp);
		append("littlepet",  x.littlepet);
		append("flag_mask",  x.flag_mask);
		append("ui_transfer",  x.ui_transfer);
		append("collision_info",  x.collision_info);
		append("runescore",  x.runescore);
		append("comsumption",  x.comsumption);
		append("astrology_info",  x.astrology_info);
		append("liveness_info",  x.liveness_info);
		append("sale_promotion_info",  x.sale_promotion_info);
		append("propadd",  x.propadd);
		append("multi_exp",  x.multi_exp);
		append("fuwen_info",  x.fuwen_info);
		append("datagroup",  x.datagroup);
		append("phase",  x.phase);
		append("award_info_6v6",  x.award_info_6v6);
		append("hide_and_seek_info",  x.hide_and_seek_info);
		append("newyear_award_info",  x.newyear_award_info);
		append("reserved14",  x.reserved14);
		append("reserved15",  x.reserved15);
		append("reserved16",  x.reserved16);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const Message& x)
	{
		data = data + "<" + name + ">\n";
		append("channel",  x.channel);
		append("src_name",  x.src_name);
		append("srcroleid",  x.srcroleid);
		append("dst_name",  x.dst_name);
		append("dstroleid",  x.dstroleid);
		append("msg",  x.msg);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCrssvrTeamsData& x)
	{
		data = data + "<" + name + ">\n";
		append("info",  x.info);
		append("roles",  x.roles);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GUniqueWinner& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("rolename",  x.rolename);
		append("money",  x.money);
		append("itemid",  x.itemid);
		append("time",  x.time);
		append("reserve1",  x.reserve1);
		append("reserve2",  x.reserve2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleAchievement& x)
	{
		data = data + "<" + name + ">\n";
		append("version",  x.version);
		append("achieve_map",  x.achieve_map);
		append("achieve_active",  x.achieve_active);
		append("achieve_spec_info",  x.achieve_spec_info);
		append("achieve_award_map",  x.achieve_award_map);
		append("reserved4",  x.reserved4);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const StockOrder& x)
	{
		data = data + "<" + name + ">\n";
		append("tid",  x.tid);
		append("time",  x.time);
		append("userid",  x.userid);
		append("price",  x.price);
		append("volume",  x.volume);
		append("status",  x.status);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCircleMemberDB& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append_string("name",  x.name);
		append("level",  x.level);
		append("reborn_cnt",  x.reborn_cnt);
		append("occupation",  x.occupation);
		append("gender",  x.gender);
		append("jointime",  x.jointime);
		append("graduate_time",  x.graduate_time);
		append("grad_bonus_count",  x.grad_bonus_count);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GApplyCondition& x)
	{
		data = data + "<" + name + ">\n";
		append("occupation",  x.occupation);
		append("gender",  x.gender);
		append("level",  x.level);
		append("faction_level",  x.faction_level);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRolePocket& x)
	{
		data = data + "<" + name + ">\n";
		append("capacity",  x.capacity);
		append("timestamp",  x.timestamp);
		append("money",  x.money);
		append("items",  x.items);
		append("equipment",  x.equipment);
		append("petbadge",  x.petbadge);
		append("petequip",  x.petequip);
		append("pocket_capacity",  x.pocket_capacity);
		append("pocket_items",  x.pocket_items);
		append("fashion",  x.fashion);
		append("mountwing",  x.mountwing);
		append("gifts",  x.gifts);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GSysLog& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append("time",  x.time);
		append("ip",  x.ip);
		append("source",  x.source);
		append("money",  x.money);
		append("items",  x.items);
		append("reserved1",  x.reserved1);
		append("reserved2",  x.reserved2);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleData& x)
	{
		data = data + "<" + name + ">\n";
		append("base",  x.base);
		append("status",  x.status);
		append("pocket",  x.pocket);
		append("storehouse",  x.storehouse);
		append("task",  x.task);
		append("base2",  x.base2);
		append("achievement",  x.achievement);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleForbid& x)
	{
		data = data + "<" + name + ">\n";
		append("type",  x.type);
		append("time",  x.time);
		append("createtime",  x.createtime);
		append("reason",  x.reason);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GRoleInventory& x)
	{
		data = data + "<" + name + ">\n";
		append("id",  x.id);
		append("pos",  x.pos);
		append("count",  x.count);
		append("client_size",  x.client_size);
		append("max_count",  x.max_count);
		append("data",  x.data);
		append("proctype",  x.proctype);
		append("expire_date",  x.expire_date);
		append("guid1",  x.guid1);
		append("guid2",  x.guid2);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GMailBox& x)
	{
		data = data + "<" + name + ">\n";
		append("timestamp",  x.timestamp);
		append("status",  x.status);
		append("mails",  x.mails);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const FamilyId& x)
	{
		data = data + "<" + name + ">\n";
		append("fid",  x.fid);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GGroupInfo& x)
	{
		data = data + "<" + name + ">\n";
		append("gid",  x.gid);
		append_string("name",  x.name);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GCrssvrTeamsRole& x)
	{
		data = data + "<" + name + ">\n";
		append("roleid",  x.roleid);
		append_string("name",  x.name);
		append("level",  x.level);
		append("occupation",  x.occupation);
		append("score",  x.score);
		append("camp",  x.camp);
		append("isonline",  x.isonline);
		append("data_score",  x.data_score);
		append("reserved",  x.reserved);
		append("ct_last_battle_count",  x.ct_last_battle_count);
		append("ct_last_punish_time",  x.ct_last_punish_time);
		append("reserved3",  x.reserved3);
		append("reserved4",  x.reserved4);
		append("reserved5",  x.reserved5);
		data = data + "</" + name + ">\n";
	}
	void append(const char* name, const GTaskData& x)
	{
		data = data + "<" + name + ">\n";
		append("taskid",  x.taskid);
		append("settime",  x.settime);
		append("taskdata",  x.taskdata);
		data = data + "</" + name + ">\n";
	}

};

};

#endif
