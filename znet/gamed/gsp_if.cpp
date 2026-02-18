#include "types.h"
#include "obj_interface.h"
#include "libcommon.h"
#include "gsp_if.h"
#include "gproviderclient.hpp"
#include "s2cgamedatasend.hpp"
#include "playerlogin_re.hpp"
#include "playerlogout.hpp"
#include "playeroffline_re.hpp"
#include "playerkickout_re.hpp"
#include "playerreconnect_re.hpp"
#include "s2cmulticast.hpp"
#include "s2cmulticast2.hpp"
#include "chatmulticast.hpp"
#include "gtradediscard.hpp"
#include "gtradestart_re.hpp"
#include "chatsinglecast.hpp"
#include "playerheartbeat.hpp"
#include "disconnectplayer.hpp"
#include "gettaskdata.hpp"
#include "settaskdata.hpp"
#include "chatbroadcast.hpp"
#include "syncroledata.hpp"
#include "../gdbclient/db_if.h" 
#include "../include/localmacro.h" 
#include "gtopitem"
#include "gtoptable"
#include "getweeklytop.hrp"
#include "getdailytop.hrp"
#include "debugcommand.hpp"
#include "updateenemy.hpp"
#include "syncplayerfaction.hpp"
#include "putspouse.hrp"
#include "factionlevel"
#include "combatkill.hpp"
#include "siegekill.hpp"
#include "gfamilyskill"
#include "playerchangegs_re.hpp"
#include "battleserverregister.hpp"
#include "taskasyncdata.hrp"
#include "syncfamilydata.hpp"
#include "familyuseskill.hpp"
#include "familyexpshare.hpp"
#include "acreportcheater.hpp"
#include "setchatemotion.hpp"
#include "actriggerquestion.hpp"
#include "commondatasync.hpp"
#include "commondatachange.hrp"
#include "sectquit.hpp"
#include "sectupdate.hpp"
#include "certchallenge.hrp"
#include "sendsnspressmessage.hpp"
#include "sendsnsapply.hpp"
#include "sendsnsvote.hpp"
#include "gapplycondition"
#include "sendrefcashused.hpp"
#include "sendreflevelup.hpp"
#include "sendchangeprofile.hpp"
#include "querynetbarreward.hpp"
#include "achievementmessage.hpp"
#include "pkmessage.hpp"
#include "refinemessage.hpp"
#include "addcirclepoint.hpp"
#include "getfriendnumber.hrp"
#include "circlegetasyncdata.hrp"
#include "trychangeds.hpp"
#include "playerchangeds_re.hpp"
#include "gconsignstart.hpp"
#include "playerconsignoffline_re.hpp"
#include "gconsignstartrole.hpp"
#include "graidteamapply.hpp"
#include "battleflagstart.hpp"
#include "querybattleflagbuff.hpp"
#include "getcouponsrep.hpp"
#include "gtsyncteams.hpp"
#include "gtteamcreate.hpp"
#include "gtteamdismiss.hpp"
#include "gtteammemberupdate.hpp"
#include "s2clinebroadcast.hpp"
#include "gkingdompointchange.hpp"
#include "gkingtrycallguards.hpp"
#include "kingguardinvite.hrp"
#include "syncbathtimes.hpp"
#include "gupdatefacact.hpp"
#include "facbasepropchange.hpp"
#include "gcrssvrteamsgetscore.hpp"
#include "facbasebuildingprogress.hpp"
#include "facdyndonatecash.hpp"
#include "facdynputauction.hpp"
#include "facdynwinauction.hpp"
#include "facdynbuyauction.hpp"
#include "facdynbuildingupgrade.hpp"
#include "facdynbuildingcomplete.hpp"
#include "facmallchange.hpp"
#include "facbasedatasend.hpp"
#include "facbasedatadeliver.hpp"
#include "syncgsroleinfo2platform.hpp"
#include "syncroleevent2sns.hpp"
#include "facbasedatabroadcast.hpp"
#include "startfactionmultiexp.hpp"
#include "facauctionput.hpp"
#include "syncplayerfaccouponadd.hpp"
#include "ghideseekraidapply.hpp"
#include "gfengshenraidapply.hpp"

#include <pthread.h>
#include <conf.h>
#include <queue>
#include <math.h>

#include <assert.h>
void handle_user_cmd(int cs_index, int sid,int uid, const void * buf, size_t size);
void handle_toplist();
using namespace GNET;
namespace GMSV
{
enum DOMAIN_TYPE
{
	DOMAIN_TYPE_NULL = 0,
	DOMAIN_TYPE_3RD_CLASS,
	DOMAIN_TYPE_2ND_CLASS,
	DOMAIN_TYPE_1ST_CLASS,
};

static Thread::Mutex  g_city_lock;
static Thread::Mutex  g_faction_lock;
static Thread::Mutex  g_factio_bonus_lock;
static int __global_gid = -1;
static city_entry city_list[256] = {{0,0}};
struct faction_bonus
{
	int faction;
	int bonus;
	faction_bonus(int faction, int bonus):faction(faction),bonus(bonus)
	{}
};
static std::vector<faction_bonus> __faction_bonus;
static std::map<int,GTopTable>  __toptable;
static std::map<int,GTopTable>  __dailytoptable;
static std::map<int,int>  __factiontable;

void SetCityOwner(int id, int level,int owner)
{
	g_city_lock.Lock();
	id = id & 0xFF;
	city_list[id].level = level;
	city_list[id].owner = owner;
	g_city_lock.UNLock();
}

int GetCityOwner(unsigned int id)
{
	if(id >= 256) return -1;
	return city_list[id].owner;
}

int GetCityLevel(unsigned int id)
{
	if(id >= 256) return -1;
	return city_list[id].level;
}

struct __TBONUS
{
	int level1;
	int level2;
	int level3;
	__TBONUS():level1(0),level2(0),level3(0){}
};
void RefreshCityBonus()
{
	
	typedef std::map<int, __TBONUS> BONUSMAP;
	BONUSMAP __map;
	g_city_lock.Lock();
	for(size_t i = 0; i < 256; i ++)
	{
		if(city_list[i].owner <=0) continue;
		int owner = city_list[i].owner;
		switch(city_list[i].level)
		{
			case DOMAIN_TYPE_3RD_CLASS:
				__map[owner].level3 ++;
				break;
			case DOMAIN_TYPE_2ND_CLASS:
				__map[owner].level2 ++;
				break;
			case DOMAIN_TYPE_1ST_CLASS:
				__map[owner].level1 ++;
			break;
			default:
			break;
		}
	}
	g_city_lock.UNLock();

	
	std::vector<faction_bonus> tmp;
	BONUSMAP::iterator it = __map.begin();
	for(; it != __map.end(); ++it)
	{
		int bonus = 0;
		int level1 = it->second.level1;
		int level2 = it->second.level2;
		int level3 = it->second.level3;
		if(level3 > 0)
		{
			if(level3 == 1)
			{
				bonus = 1;
			}
			else
			{
				bonus = (int) ((sqrt(1 + 8*(level3 - 2)) - 1)/2 + 2);
			}
		}
		
		bonus += level2 * 2 + level1 * 3;
		tmp.push_back(faction_bonus(it->first,bonus));
		printf("faction:%d, bonus:%d , leve1:%d level2:%d level3:%d\n",it->first,bonus,level1,level2,level3);
	}

	g_factio_bonus_lock.Lock();
	__faction_bonus.swap(tmp);
	g_factio_bonus_lock.UNLock();
}

int GetFactionLevel(int faction)
{
	int level = -1;
	g_faction_lock.Lock();
	std::map<int,int>::iterator it=__factiontable.find(faction);
	if(it!=__factiontable.end())
		level = it->second; 
	g_faction_lock.UNLock();
	return level;
}
int GetFactionBonus(int faction)
{
	int bonus = 0;
	g_factio_bonus_lock.Lock();
	std::vector<faction_bonus>::iterator it = __faction_bonus.begin();
	for(; it !=  __faction_bonus.end(); ++it)
	{
		if(it->faction == faction)
		{
			bonus = it->bonus;
			break;
		}
	}
	g_factio_bonus_lock.UNLock();
	return bonus;
}


struct user_cmd_t
{
	int cs_index;
	int sid;
	int uid;
	Octets data;
};

#define USER_CMD_QUEUE_NUM 1
static std::deque<user_cmd_t> g_xlist[USER_CMD_QUEUE_NUM];
static Thread::Mutex  g_xlock[USER_CMD_QUEUE_NUM];

void RecordProtocolInfo(int cmd, size_t size, int count)
{
	//$$$$$$$$$$
}

void 
queue_user_cmd(int cs_index, int sid, int uid,Octets & data)
{
	user_cmd_t tmp;
	tmp.cs_index = cs_index;
	tmp.sid = sid;
	tmp.uid = uid;
	tmp.data.swap(data);
	int index = uid % USER_CMD_QUEUE_NUM;
	Thread::Mutex::Scoped keeper(g_xlock[index]);
	g_xlist[index].push_back(tmp);
}

bool SendToLS(int lid, GNET::Protocol * p)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(lid,p);
}

bool SendClientData(int lid, int userid/* actually is roleid*/,int sid,const void * buf, size_t size)
{
	if(lid <=0) return false;
#ifdef __GS_PROTOCOL_STAT__
	if(size > sizeof(short))
	{
		int cmd = *(unsigned short*)buf;
		RecordProtocolInfo(cmd, size, 1);
	}
#endif
	return GProviderClient::DispatchProtocol(lid,S2CGamedataSend(userid,sid,Octets(buf,size)));
}

bool MultiSendClientData(int lid,const puser * first, const puser * last,const void * buf, size_t size,int except_id)
{
	if(lid <=0) return false;
#ifdef __GS_PROTOCOL_STAT__
	if(size > sizeof(short))
	{
		int __cmd = *(unsigned short*)buf;
		RecordProtocolInfo(__cmd, size, last - first);	//可能少 1
	}
#endif
	S2CMulticast packet; 
	packet.data.insert(packet.data.end(),buf,size);
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		if(except_id !=  val.first)
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool MultiSendClientData2(int lid,const puser * first, const puser * last,const void * buf, size_t size,int except_id)
{
	if(lid <=0) return false;
	S2CMulticast2 packet; 
	packet.data.insert(packet.data.end(),buf,size);
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		if(except_id !=  val.first)
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	return GProviderClient::DispatchProtocol(lid,&packet);
}


bool MultiSendClientData(int lid, const puser * first, const puser * last,const void * buf, size_t size)
{
	if(lid <=0) return false;
#ifdef __GS_PROTOCOL_STAT__
	if(size > sizeof(short))
	{
		int __cmd = *(unsigned short*)buf;
		RecordProtocolInfo(__cmd, size, last - first);
	}
#endif
	S2CMulticast packet; 
	packet.data.insert(packet.data.end(),buf,size);
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool MultiChatMsg(int lid,const puser* first, const puser* last,const chat_msg & chat)
{
	if(lid <=0) return false;
	ChatMultiCast packet;
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	packet.srcroleid = chat.speaker;
	packet.channel = chat.channel;
	packet.emotion = chat.emote_id;
	packet.srcrolelevel = chat.level;
	packet.srcfactionid = chat.mafia_id;
	packet.srcsectid = chat.sect_id;
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool SendChatMsg(int lid, int userid, int sid,const chat_msg & chat)
{
	ChatSingleCast packet;
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	packet.srcroleid = chat.speaker;
	packet.channel = chat.channel;
	packet.dstroleid = userid;
	packet.dstlocalsid = sid;
	packet.emotion = chat.emote_id;
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool SetChatEmote(int userid, char emote_id)
{
	SetChatEmotion protocol;
	protocol.roleid = userid;
	protocol.emotion = emote_id;
	return GProviderClient::DispatchProtocol(0,&protocol);
}

bool BroadChatMsg(const chat_msg &chat)
{
	ChatBroadCast packet;
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	packet.srcroleid = chat.speaker;
	packet.channel = chat.channel;
	packet.emotion = chat.emote_id;
	return GProviderClient::DispatchProtocol(0,&packet);
}


bool BroadcastLineMessage(int gs_id, void * buf, size_t size)
{
	if(gs_id < 0) return false;
	S2CLineBroadcast packet; 
	packet.data.insert(packet.data.end(),buf,size);
	packet.lineid = gs_id;
	
	return GProviderClient::DispatchProtocol(0,&packet);
}


bool SendPlayerHeartbeat(int lid, int userid, int localsid)
{
	PlayerHeartBeat packet(userid,lid,localsid);
	return GProviderClient::DispatchProtocol(lid,&packet);
}

void SendMafiaDuelDeath(int mafia_id ,int dead_man, int attacker,int battle_id)
{
	CombatKill packet(battle_id, attacker, dead_man, mafia_id);
	GProviderClient::DispatchProtocol(0,&packet);
}

void SendBattlefieldDeath(int mafia_id,int dead_man,int attacker,int battle_id)
{
	SiegeKill packet(battle_id,attacker,dead_man,mafia_id);
	GProviderClient::DispatchProtocol(0,&packet);
}



bool SendLoginRe(int lid, int userid, int sid,int result, char flag, int tag, float x, float y, float z, char level, char oc, int fid,int ft,int con,int family, int devotion, int jointime, size_t battle_score, int circle_id, int client_tag)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,PlayerLogin_Re(result,userid,level, oc,fid,ft,family, circle_id, con,devotion,jointime,lid, battle_score, sid,tag,x,y,z, flag, client_tag));
}

bool SendSwitchServerSuccess(int lid, int userid, int localsid,int gs_id)
{
	return false;
}

bool SendSwitchServerTimeout(int lid, int userid, int localsid)
{
	return false;
}

bool SendSwitchServerCancel(int lid, int userid, int localsid)
{
	return false;
}

bool SendSwitchServerStart(int lid, int userid, int localsid,int src_gsid, int dst_gsid, const void * key_buf, size_t key_size)
{
	return false;
}

bool SendKickoutRe(int userid/* actually is roleid */, int sid , int result, int src_zoneid)
{
	return GProviderClient::DispatchProtocol(0,PlayerKickout_Re(result?-1:ERR_SUCCESS,userid,src_zoneid));
}

bool SendOfflineRe(int lid,int userid/*actually is roleid*/,int sid, int result)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,PlayerOffline_Re(result?result:ERR_SUCCESS,userid));
}

bool SendReconnectRe(int userid/* actually is roleid */, int sid , int result)
{
	return GProviderClient::DispatchProtocol(0,PlayerReconnect_Re(result?-1:ERR_SUCCESS,userid));
}

void MakeConsignGsRoleInfo(const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list, GNET::GConsignGsRoleInfo * roleinfo)
{
	//基本信息
	roleinfo->rolebasicinfo.resistance1				= basic_attr.resistance1;
	roleinfo->rolebasicinfo.resistance2				= basic_attr.resistance2;
	roleinfo->rolebasicinfo.resistance3				= basic_attr.resistance3;
	roleinfo->rolebasicinfo.resistance4				= basic_attr.resistance4;
	roleinfo->rolebasicinfo.resistance5				= basic_attr.resistance5;
	roleinfo->rolebasicinfo.level					= basic_attr.level;
	roleinfo->rolebasicinfo.cls						= basic_attr.cls;
	roleinfo->rolebasicinfo.level_before_reborn		= basic_attr.level_before_reborn;
	roleinfo->rolebasicinfo.cls_before_reborn		= basic_attr.cls_before_reborn;
	roleinfo->rolebasicinfo.talisman_value			= basic_attr.talisman_value;
	roleinfo->rolebasicinfo.reputation				= basic_attr.reputation;
	roleinfo->rolebasicinfo.pk_value					= basic_attr.pk_value;
	roleinfo->rolebasicinfo.cultivation				= basic_attr.cultivation;
	roleinfo->rolebasicinfo.exp						= basic_attr.exp;
	roleinfo->rolebasicinfo.max_hp					= basic_attr.max_hp;
	roleinfo->rolebasicinfo.max_mp					= basic_attr.max_mp;
	roleinfo->rolebasicinfo.damage_low				= basic_attr.damage_low;
	roleinfo->rolebasicinfo.damage_high				= basic_attr.damage_high;
	roleinfo->rolebasicinfo.defense					= basic_attr.defense;
	roleinfo->rolebasicinfo.attack					= basic_attr.attack;
	roleinfo->rolebasicinfo.armor					= basic_attr.armor;
	roleinfo->rolebasicinfo.crit_damage				= basic_attr.crit_damage;
	roleinfo->rolebasicinfo.crit_rate				= basic_attr.crit_rate;
	roleinfo->rolebasicinfo.skill_armor_rate			= basic_attr.skill_armor_rate;
	roleinfo->rolebasicinfo.skill_attack_rate		= basic_attr.skill_attack_rate;
	roleinfo->rolebasicinfo.anti_crit				= basic_attr.anti_crit;
	roleinfo->rolebasicinfo.anti_crit_damage			= basic_attr.anti_crit_damage;
	roleinfo->rolebasicinfo.cult_defense_xian		= basic_attr.cult_defense_xian;
	roleinfo->rolebasicinfo.cult_defense_mo			= basic_attr.cult_defense_mo;
	roleinfo->rolebasicinfo.cult_defense_fo			= basic_attr.cult_defense_fo;
	roleinfo->rolebasicinfo.battlescore				= basic_attr.battlescore;
	roleinfo->rolebasicinfo.reborn_count				= basic_attr.reborn_count;
	roleinfo->rolebasicinfo.gender					= basic_attr.gender;

	//元神信息
	roleinfo->deityinfo.atatck_xian					= deity_info.atatck_xian;
	roleinfo->deityinfo.atatck_fo					= deity_info.atatck_fo;
	roleinfo->deityinfo.atatck_mo					= deity_info.atatck_mo;
	roleinfo->deityinfo.deity_level					= deity_info.deity_level;
	roleinfo->deityinfo.deity_power					= deity_info.deity_power;
	roleinfo->deityinfo.max_dp						= deity_info.max_dp;
	roleinfo->deityinfo.deity_exp					= deity_info.deity_exp;

	//尊号信息
	Octets os_title(title_info.pdata, title_info.datasize);
	roleinfo->title.titlelist = os_title;

	//技能信息
	Octets os_skill(skill_info.pdata, skill_info.datasize);
	roleinfo->skillinfo = os_skill;

	//宠物prop_added
	for(size_t i = 0; i < pet_added_list.size(); i++)
	{
		GPetAddedInfo infodata;
		infodata.pet_index							= pet_added_list[i].pet_index;
		infodata.max_hp								= pet_added_list[i].maxhp;
		infodata.max_mp								= pet_added_list[i].maxmp;
		infodata.defence							= pet_added_list[i].defence;
		infodata.attack								= pet_added_list[i].attack;
		infodata.resistance1						= pet_added_list[i].resistance[0];
		infodata.resistance2						= pet_added_list[i].resistance[1];
		infodata.resistance3						= pet_added_list[i].resistance[2];
		infodata.resistance4						= pet_added_list[i].resistance[3];
		infodata.resistance5						= pet_added_list[i].resistance[4];
		infodata.hit								= pet_added_list[i].hit;
		infodata.jouk								= pet_added_list[i].jouk;
		infodata.crit_rate							= pet_added_list[i].crit_rate;
		infodata.crit_damage						= pet_added_list[i].crit_damage;
		infodata.equip_mask							= pet_added_list[i].equip_mask;

		roleinfo->pet_addinfo.push_back(infodata);
	}
}

void MakeSNSGsRoleInfo(const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list, const role_basic_attribute_ext & ext, GNET::GSNSGsRoleInfo * roleinfo)
{
	//基本信息
	roleinfo->rolebasicinfo.resistance1				= basic_attr.resistance1;
	roleinfo->rolebasicinfo.resistance2				= basic_attr.resistance2;
	roleinfo->rolebasicinfo.resistance3				= basic_attr.resistance3;
	roleinfo->rolebasicinfo.resistance4				= basic_attr.resistance4;
	roleinfo->rolebasicinfo.resistance5				= basic_attr.resistance5;
	roleinfo->rolebasicinfo.level					= basic_attr.level;
	roleinfo->rolebasicinfo.cls						= basic_attr.cls;
	roleinfo->rolebasicinfo.level_before_reborn		= basic_attr.level_before_reborn;
	roleinfo->rolebasicinfo.cls_before_reborn		= basic_attr.cls_before_reborn;
	roleinfo->rolebasicinfo.talisman_value			= basic_attr.talisman_value;
	roleinfo->rolebasicinfo.reputation				= basic_attr.reputation;
	roleinfo->rolebasicinfo.pk_value					= basic_attr.pk_value;
	roleinfo->rolebasicinfo.cultivation				= basic_attr.cultivation;
	roleinfo->rolebasicinfo.exp						= basic_attr.exp;
	roleinfo->rolebasicinfo.max_hp					= basic_attr.max_hp;
	roleinfo->rolebasicinfo.max_mp					= basic_attr.max_mp;
	roleinfo->rolebasicinfo.damage_low				= basic_attr.damage_low;
	roleinfo->rolebasicinfo.damage_high				= basic_attr.damage_high;
	roleinfo->rolebasicinfo.defense					= basic_attr.defense;
	roleinfo->rolebasicinfo.attack					= basic_attr.attack;
	roleinfo->rolebasicinfo.armor					= basic_attr.armor;
	roleinfo->rolebasicinfo.crit_damage				= basic_attr.crit_damage;
	roleinfo->rolebasicinfo.crit_rate				= basic_attr.crit_rate;
	roleinfo->rolebasicinfo.skill_armor_rate			= basic_attr.skill_armor_rate;
	roleinfo->rolebasicinfo.skill_attack_rate		= basic_attr.skill_attack_rate;
	roleinfo->rolebasicinfo.anti_crit				= basic_attr.anti_crit;
	roleinfo->rolebasicinfo.anti_crit_damage			= basic_attr.anti_crit_damage;
	roleinfo->rolebasicinfo.cult_defense_xian		= basic_attr.cult_defense_xian;
	roleinfo->rolebasicinfo.cult_defense_mo			= basic_attr.cult_defense_mo;
	roleinfo->rolebasicinfo.cult_defense_fo			= basic_attr.cult_defense_fo;
	roleinfo->rolebasicinfo.battlescore				= basic_attr.battlescore;
	roleinfo->rolebasicinfo.reborn_count				= basic_attr.reborn_count;
	roleinfo->rolebasicinfo.gender					= basic_attr.gender;

	//元神信息
	roleinfo->deityinfo.atatck_xian					= deity_info.atatck_xian;
	roleinfo->deityinfo.atatck_fo					= deity_info.atatck_fo;
	roleinfo->deityinfo.atatck_mo					= deity_info.atatck_mo;
	roleinfo->deityinfo.deity_level					= deity_info.deity_level;
	roleinfo->deityinfo.deity_power					= deity_info.deity_power;
	roleinfo->deityinfo.max_dp						= deity_info.max_dp;
	roleinfo->deityinfo.deity_exp					= deity_info.deity_exp;

	//尊号信息
	Octets os_title(title_info.pdata, title_info.datasize);
	roleinfo->title.titlelist = os_title;

	//技能信息
	Octets os_skill(skill_info.pdata, skill_info.datasize);
	roleinfo->skillinfo = os_skill;

	//宠物prop_added
	for(size_t i = 0; i < pet_added_list.size(); i++)
	{
		GPetAddedInfo infodata;
		infodata.pet_index							= pet_added_list[i].pet_index;
		infodata.max_hp								= pet_added_list[i].maxhp;
		infodata.max_mp								= pet_added_list[i].maxmp;
		infodata.defence							= pet_added_list[i].defence;
		infodata.attack								= pet_added_list[i].attack;
		infodata.resistance1						= pet_added_list[i].resistance[0];
		infodata.resistance2						= pet_added_list[i].resistance[1];
		infodata.resistance3						= pet_added_list[i].resistance[2];
		infodata.resistance4						= pet_added_list[i].resistance[3];
		infodata.resistance5						= pet_added_list[i].resistance[4];
		infodata.hit								= pet_added_list[i].hit;
		infodata.jouk								= pet_added_list[i].jouk;
		infodata.crit_rate							= pet_added_list[i].crit_rate;
		infodata.crit_damage						= pet_added_list[i].crit_damage;
		infodata.equip_mask							= pet_added_list[i].equip_mask;

		roleinfo->pet_addinfo.push_back(infodata);
	}
	roleinfo->basic_ext.renxing1 = ext.renxing1;
	roleinfo->basic_ext.renxing2 = ext.renxing2;
	roleinfo->basic_ext.renxing3 = ext.renxing3;
	roleinfo->basic_ext.renxing4 = ext.renxing4;
	roleinfo->basic_ext.renxing5 = ext.renxing5;

	roleinfo->basic_ext.jingtong1 = ext.jingtong1;
	roleinfo->basic_ext.jingtong2 = ext.jingtong2;
	roleinfo->basic_ext.jingtong3 = ext.jingtong3;
	roleinfo->basic_ext.jingtong4 = ext.jingtong4;
	roleinfo->basic_ext.jingtong5 = ext.jingtong5;
}

//寄售角色下线协议Re
bool SendPlayerConsignOfflineRe(int lid, int roleid, int sid, int result, const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list)
{
	if(lid <= 0) return false;

	GNET::GConsignGsRoleInfo roleinfo;
	MakeConsignGsRoleInfo(basic_attr, deity_info, title_info, skill_info, pet_added_list, &roleinfo);
	return GProviderClient::DispatchProtocol(0, PlayerConsignOffline_Re(result, roleid, roleinfo));
}

bool SendGsRoleInfo2Platform(int roleid, const role_basic_attribute & basic_attr, const role_deity_info & deity_info, const role_title_info & title_info, const role_skill_info & skill_info, const std::vector<role_pet_prop_added> & pet_added_list, const role_basic_attribute_ext & ext)
{
	GNET::GSNSGsRoleInfo roleinfo;
	MakeSNSGsRoleInfo(basic_attr, deity_info, title_info, skill_info, pet_added_list, ext, &roleinfo);
	//发送到 ggated
	return GProviderClient::DispatchProtocol(100, SyncGsRoleInfo2Platform(roleid, roleinfo));
}

//寄售角色下线协议Re
bool SendPlayerConsignFailedOfflineRe(int lid, int roleid, int sid, int result)
{
	if(lid <= 0) return false;
	//角色寄售成功不会调用这个函数
	if(0 == result) return false;
	return GProviderClient::DispatchProtocol(0, PlayerConsignOffline_Re(result, roleid));
}

bool SendLogout(int lid, int userid/*actually is roleid*/, int sid,int reason)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,PlayerLogout(reason,userid,lid,sid));
}

bool SendDisconnect(int lid, int userid/*actually is roleid*/, int sid,int reason)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,DisconnectPlayer(userid,lid,sid,__global_gid));
}


void DiscardTrade(int trade_id, int userid)
{
	GProviderClient::DispatchProtocol(0,GTradeDiscard(trade_id,0));
}

void SendCosmeticRequest(int userid, int ticket_index, int ticket_id)
{
}

void CancelCosmeticRequest(int userid)
{
}

void ReplyTradeRequest(int trade_id, int userid,int localsid,bool isSuccess)
{
	GProviderClient::DispatchProtocol(0,GTradeStart_Re(isSuccess?0:1,trade_id,userid,localsid));
}

void SendConsignRequest(int userid, char type, int item_id, int item_idx, int item_cnt, int item_type, int money, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len)
{
	GConsignStart data;
	data.roleid = (unsigned int)userid;
	data.consign_type= (unsigned char)type;
	data.item_id = item_id;
	data.item_cnt = item_cnt;
	data.item_idx = item_idx;
	data.category = item_type;
	data.money = money;
	data.shelf_period = sold_time;
	data.price = price;
	data.margin = margin;
	data.sell_to_rolename.replace(sellto_name, sellto_name_len);
	GProviderClient::DispatchProtocol(0, data);
}

//角色寄售发起
void SendConsignStartRole(int roleid, char type, int item_id, int item_idx, int item_cnt, int item_type, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len)
{
	GConsignStartRole data;
	data.roleid				= roleid;
	data.margin				= margin;
	data.price				= price;
	data.shelf_period		= sold_time;
	data.consign_type		= (unsigned char)type;
	data.token_item_id		= item_id;
	data.token_item_cnt		= item_cnt;
	data.token_item_idx		= item_idx;
	data.category			= item_type;
	data.sell_to_rolename.replace(sellto_name, sellto_name_len);

	GProviderClient::DispatchProtocol(0, data);
}

void SendDebugCommand(int roleid, int type, const char * buf, size_t size)
{
	DebugCommand data;
	data.roleid = (unsigned int)roleid;
	data.tag = type;
	data.data.replace(buf,size);
	GProviderClient::DispatchProtocol(0,data);
}

bool SendChangeGsRe( int retcode, int roleid, int localsid, int targetgs, int tag, float x, float y, float z, int reason )
{
	if( localsid <= 0 ) return false;
	return GProviderClient::DispatchProtocol(0,
			PlayerChangeGS_Re( retcode, roleid, localsid, targetgs, tag, reason, x, y, z ) );
	return false;
}


void SetTaskData(int taskid, const void * buf, size_t size)
{
	GNET::SetTaskData data;
	data.taskid = taskid;
	data.taskdata.insert(data.taskdata.end(),buf,size);
	GProviderClient::DispatchProtocol(0,data);
}

void GetTaskData(int taskid, int uid, const void * env_data, size_t env_size)
{
	GNET::GetTaskData data;
    data.taskid = taskid;
    data.playerid = uid;
    data.env.insert(data.env.end(),env_data,env_size);
    GProviderClient::DispatchProtocol(0,data);
}

void ReportCheater2Gacd(int roleid, int cheattype, const void *buf, size_t size)
{
    GNET::ACReportCheater acrc;
    acrc.roleid = roleid;
    acrc.cheattype = cheattype;
    if( buf ) acrc.cheatinfo.replace(buf, size);
    GProviderClient::DispatchProtocol(0, acrc);
}

void TriggerQuestion2Gacd(int roleid)
{
    GNET::ACTriggerQuestion actq;
    actq.roleid = roleid;
    GProviderClient::DispatchProtocol(0, actq);
}

void SetCouple(int id1, int id2, int op)
{
	Rpc *rpc = Rpc::Call(RPC_PUTSPOUSE, PutSpouseArg(op,id1,id2));
	GProviderClient::DispatchProtocol(0,rpc);
}

void * autoWakeUp(void *)
{
	do
	{
		usleep(50000);
		PollIO::WakeUp();
	}while(1);
	return NULL;
}       

void * cmdDispatcher(void * tmp)
{
	int index = (int)tmp;
	std::deque<user_cmd_t> tmpList;
	while(1)
	{
		while(g_xlist[index].empty()) {usleep(2000);}
		g_xlock[index].Lock();
		tmpList.swap(g_xlist[index]);
		g_xlock[index].UNLock();
		while(!tmpList.empty())
		{
			user_cmd_t & cmd = tmpList.front();
			handle_user_cmd(cmd.cs_index, cmd.sid,cmd.uid, cmd.data.begin(), cmd.data.size());
			tmpList.pop_front();
		}
	
	}
	return NULL;
}

bool InitGSP(const char * conf,int gid, const char * servername, int server_attr, const char * version, int pksetting)
{
	Conf::GetInstance(conf);
	pthread_t ph;
	pthread_create(&ph,NULL,autoWakeUp,NULL);
#ifdef __USE_SPEC_GAMEDATASEND__
	for(size_t i = 0; i < USER_CMD_QUEUE_NUM; i ++)
	{
		pthread_create(&ph,NULL,cmdDispatcher,(void*)i);
	}
#endif
	__global_gid = gid;
	return GProviderClient::Connect(gid, servername, version, server_attr, pksetting);
}

void StartPollIO()
{
	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();
}

void SyncRoleInfo(const GDB::base_info* info, const GDB::vecdata* data)
{
	GProviderClient::DispatchProtocol(0,SyncRoleData(info->id,info->level,info->cls,info->battlescore));
}

void SendUpdateEnemy(int userid, int enemy)
{
	GProviderClient::DispatchProtocol(0,UpdateEnemy(userid, enemy, ENEMY_INSERT,0));
}

void SendPlayerFaction(int userid, int fid, int mid, int level, int con, int fcon, unsigned battle_score, int reborn_cnt, int circle_id )
{
	GProviderClient::DispatchProtocol(0,SyncPlayerFaction(userid, mid, fid, 0, level, con,fcon, battle_score, reborn_cnt, circle_id));
}

static int weekly_top_ok = 0;
static int daily_top_ok = 0;
void FetchWeeklyTop()
{
	if(weekly_top_ok) return ;
	GetWeeklyTop* rpc = (GetWeeklyTop*) Rpc::Call(RPC_GETWEEKLYTOP,Integer(0));
	GProviderClient::DispatchProtocol(0,rpc);
}
void FetchDailyTop()
{
	if(daily_top_ok) return ;
	GetDailyTop* rpc = (GetDailyTop*) Rpc::Call(RPC_GETDAILYTOP,Integer(TOP_PERSONAL_LEVEL));
	GProviderClient::DispatchProtocol(0,rpc);
}

void OnLoadWeeklyTop(GNET::GTopTable& table)
{
	if(weekly_top_ok) return;
	__toptable[table.id] = table;
	if(__toptable.size()==TOPTABLE_COUNT)
	{
		weekly_top_ok = 1;
		handle_toplist();
		__toptable.clear();
	}
}

void OnLoadDailyTop(GNET::GTopTable& table)
{
	if(daily_top_ok) return;
	if(table.id == TOP_PERSONAL_LEVEL)
	{
		__dailytoptable[table.id] = table;
		daily_top_ok = 1;
	} 
}

void OnUpdateFactionLevel(const std::vector<FactionLevel>& list)
{
	g_faction_lock.Lock();
	for(std::vector<FactionLevel>::const_iterator it=list.begin(),ie=list.end();it!=ie;++it)
		__factiontable[it->fid] = it->level;
	g_faction_lock.UNLock();
}

void TopList2Title(int fd)
{
	std::map<int,GTopTable>::iterator it = __toptable.begin();
	FILE * file = fdopen(fd ,"w");
	for(;it != __toptable.end();it ++)
	{
		GNET::GTopTable & t = it->second;
		fprintf(file, "%d %d\n",t.id,t.items.size());
		for(size_t i =0; i < t.items.size(); i ++)
		{
			fprintf(file, "%d %d %d %lld %d\n",i, t.items[i].id, t.items[i].oldrank, t.items[i].value, t.items[i].occupation);
		}
	}
	fprintf(file,"%d %d\n",-1,0);
	fclose(file);
}

int GetRankLevel(int index)
{
	if(index > 500 || index <= 0) return 0;
        if(__dailytoptable.empty()) return 0;

        std::map<int,GTopTable>::iterator it = __dailytoptable.find(TOP_PERSONAL_LEVEL);
        if (it == __dailytoptable.end() ) return 0;

        GNET::GTopTable & t = it->second;
        if(t.items.size() < (size_t)index) return 0;

        return t.items[index-1].value;
}

void NotifyTableChange(int tableid)
{
	if(tableid == TOP_PERSONAL_LEVEL && daily_top_ok == 1)
	{
		daily_top_ok = 0;	
	}
}

void SendTaskAsyncData(int roleid, const void * buf, size_t size)
{
	AsyncData arg(roleid, Octets(buf, size), 0);
	Rpc *rpc = Rpc::Call(RPC_TASKASYNCDATA, arg);
	GProviderClient::DispatchProtocol(0,rpc);
}

void SendUpdateFamilySkill(int roleid, int familyid, int id, int ability)
{
	GProviderClient::DispatchProtocol(0,FamilyUseSkill(roleid, familyid, id, ability));
}
void SendFamilyExpShare(int roleid, int familyid, int level, int64_t exp)
{
	GProviderClient::DispatchProtocol(0,FamilyExpShare(roleid, familyid, level, exp));
}

void QuitSect(int sectid, char reason, int disciple)
{
	GProviderClient::DispatchProtocol(0,SectQuit(sectid, reason, disciple));
}

void UpdateSect(int sectid, int reputation, const int * skill_list, size_t skill_count)
{
	SectUpdate packet(sectid, sectid, reputation);
	packet.skills.reserve(skill_count);
	for(size_t i = 0; i < skill_count ; i++)
	{
		GSectSkill skill(skill_list[i*2], skill_list[i*2 + 1]);
		packet.skills.push_back(skill);
	}

	GProviderClient::DispatchProtocol(0,packet);
}
bool SendCommonData(int session_key, int data_type, void *p, size_t size)
{
	CommonDataSync pcd(session_key, data_type, Octets(p,size));
	return GProviderClient::DispatchProtocol (0, pcd);
}
bool SendCommonDataRpcCall (int session_key, int data_type, void *p, size_t size,
		void *user_data)
{
	CommonDataArg arg;
	arg.session_key = session_key;
	arg.data_type = data_type;
	arg.data.insert (arg.data.end(), p, size);
	arg.user_data = (int) user_data; //Not 64 bit compatiable

	Rpc *rpc = Rpc::Call(RPC_COMMONDATACHANGE, arg);
	return GProviderClient::DispatchProtocol(0,rpc);
}

bool SendCertChallenge(const void *c_buf, size_t c_size, const void *r_buf, size_t r_size)
{
/*
	Security *random = Security::Create(RANDOM);
	CertChallengeArg arg;
	arg.challenge.resize(32);
	arg.reserved.resize(8);
	random->Update(arg.challenge);
	random->Update(arg.reserved);
i*/
	CertChallengeArg arg;
	arg.challenge = Octets(c_buf, c_size);
	arg.reserved = Octets(r_buf, r_size);

	CertChallenge *rpc =(CertChallenge*) Rpc::Call(RPC_CERTCHALLENGE, arg);

	return GProviderClient::DispatchProtocol(0, rpc);
}


bool SendPressMessage(int roleid, int message_type, unsigned char occupation, char gender, int level,
		int faction_level, const void *message, size_t message_len) 
{
	GApplyCondition condition(occupation, gender, level, faction_level);
	SendSNSPressMessage sns_msg(roleid, message_type, Octets(message, message_len), condition); 
	return GProviderClient::DispatchProtocol(0, sns_msg);
}

bool SendApplyMessage(int roleid, char oper, char message_type, int message_id, int dst_index, const void *message, 
		size_t message_len) 
{
	SendSNSApply sns_reply(roleid, oper, message_type, message_id, Octets(message, message_len), dst_index); 
	return GProviderClient::DispatchProtocol(0, sns_reply);
}

bool SendVoteMessage(int roleid, char oper, char message_type, int message_id)
{
	SendSNSVote sns_vote(roleid, oper, message_type, message_id);
	return GProviderClient::DispatchProtocol(0, sns_vote);
}

bool SendCashUsed(int roleid, int bonus)
{
	SendRefCashUsed cash_used(roleid, bonus);
	return GProviderClient::DispatchProtocol(0, cash_used);
}

void SendFacActChange(int fid, int delta)
{
	int fac_lev = GetFactionLevel(fid);
	if (fac_lev < FAC_ACT_START_LEV)
		return;
	GUpdateFacAct update(fid, delta);
	GProviderClient::DispatchProtocol(0, update);
}

bool SendLevelUp(int roleid, int64_t exp, int level, int reborn_cnt)
{
	SendRefLevelUp level_up(roleid, exp, level, reborn_cnt);
	return GProviderClient::DispatchProtocol(0, level_up);
}

bool SendChangeStyle(int roleid, unsigned char face_id, unsigned char hair_id, unsigned char earid, unsigned char tailid, 
		unsigned char fashionid)
{
	SendChangeProfile change_profile(roleid, face_id, hair_id, earid, tailid, fashionid);
	return GProviderClient::DispatchProtocol(0, change_profile);
}

bool QueryNetbarReward(int roleid)
{
	QueryNetBarReward netbar_reward(roleid);
	return GProviderClient::DispatchProtocol(0, netbar_reward);
}

bool SendAchievementMsg(int broad_type, int roleid, unsigned short achieve_id, int param, int finish_time)
{
	AchievementMessage msg(roleid, achieve_id, param, Octets(), 0, finish_time);
	msg.localsid = broad_type;
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendPKMsg(int killer, int deader)
{
	PKMessage msg(killer, deader);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendRefineMsg(int roleid, int itemid, int refine_level)
{
	RefineMessage msg(roleid, itemid, refine_level);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendAddCirclePoint(int circleid, int point)
{
	AddCirclePoint msg(circleid, point);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendQueryFriendNum(int roleid, int taskid)
{
	GetFriendNumberArg arg;
	arg.roleid = roleid;
	GetFriendNumber *rpc = (GetFriendNumber*) Rpc::Call(RPC_GETFRIENDNUMBER, arg);
	return GProviderClient::DispatchProtocol(0,rpc);
}

bool SendCirlceAsyncData(int roleid, const void * buf, size_t size)
{
	CircleAsyncData arg(roleid, Octets(buf, size), 0);
	Rpc *rpc = Rpc::Call(RPC_CIRCLEGETASYNCDATA, arg);
	return GProviderClient::DispatchProtocol(0,rpc);
}

bool SendPlayerTryChangeDS(int roleid, char flag)
{
	TryChangeDS msg(roleid, flag); 
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendPlayerChangeDSRe(int retcode, int roleid, int gsid, int localsid, char flag)
{
	PlayerChangeDS_Re msg;
	msg.retcode = retcode;
	msg.roleid = roleid;
	msg.gsid = gsid;
	msg.flag = flag;	
	msg.localsid = localsid;
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendApplyCollisionRaid(int raid_id, std::vector<raid_apply_info> & apply_list, int reborn_cnt, char is_cross)
{
	GRaidTeamApply msg;
	msg.map_id = raid_id;

	std::vector<TeamRaidApplyingRole> role_info;
	TeamRaidApplyingRole role;
	for(size_t i = 0; i < apply_list.size(); ++i)
	{
		role.roleid = apply_list[i].roleid;
		role.score = apply_list[i].score;
		role.occupation = apply_list[i].occupation;
		role_info.push_back(role);
	}
	msg.roles = role_info;
	msg.reborn_cnt = reborn_cnt;
	msg.iscross = is_cross;
	msg.issrc = 0;
	
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendBattleFlagStart(int roleid, int flagid, int duration, int gsid, int worldtag, int factionid)
{
	BattleFlagStart msg;
	msg.roleid = roleid;
	msg.flagid = flagid;
	msg.duration = duration;
	msg.gsid = gsid;
	msg.worldtag = worldtag;
	msg.factionid = FactionIDBean(FACTIONTYPE, factionid);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendQueryBattleFlagBuff(int roleid, int gsid, int worldtag, int factionid)
{
	QueryBattleFlagBuff msg;
	msg.roleid = roleid;
	msg.gsid = gsid;
	msg.worldtag = worldtag;
	msg.factionid = FactionIDBean(FACTIONTYPE, factionid);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendCouponsInfo(int userid, int toaid, int tozoneid, int roleid, std::map<int, int> & task_finish_count)
{
	GetCouponsRep msg;
	msg.userid = userid;
	msg.toaid = toaid;
	msg.tozoneid = tozoneid;
	msg.roleid = roleid;
	msg.coupons = task_finish_count;
	return GProviderClient::DispatchProtocol(0, msg);
}

// Add By SunJunbo, 2012.10.17, GT Team Related
bool SendGTSyncTeams(std::vector<gt_team_info>& teams)
{
	if(teams.empty()) return true;

	GTSyncTeams msg;
	for (std::vector<gt_team_info>::iterator it = teams.begin(); it != teams.end(); ++it) {
		GTTeamBean gtteambean;
		gtteambean.teamid = it->team_id;
		gtteambean.captain = (int64_t)it->captain;
		for (std::vector<int>::iterator mem_it = it->members.begin(); mem_it != it->members.end(); ++mem_it) {
			if(it->captain == *mem_it)continue;
			gtteambean.members.push_back((int64_t)*mem_it);	
		}
		msg.teams.push_back(gtteambean);
	}
	return GProviderClient::DispatchProtocol(100, msg);
}

bool SendGTTeamCreate(gt_team_info& team)
{
	GTTeamCreate msg;	
	msg.team.teamid = team.team_id;
	msg.team.captain = (int64_t)team.captain;
	for (std::vector<int>::iterator mem_it = team.members.begin(); mem_it != team.members.end(); ++mem_it) {
		if(team.captain == *mem_it)continue;
		msg.team.members.push_back((int64_t)*mem_it);	
	}
	return GProviderClient::DispatchProtocol(100, msg);
}

bool SendGTTeamDismiss(int64_t team_id)
{
	GTTeamDismiss msg;
	msg.teamid = team_id;
	return GProviderClient::DispatchProtocol(100, msg);
}

bool SendGTTeamMemberUpdate(int64_t team_id, const std::vector<int>& members, char operation)
{
	GTTeamMemberUpdate msg;
	msg.teamid = team_id;
	msg.members.reserve(members.size());
	for (std::vector<int>::const_iterator it = members.begin(); it != members.end(); ++it) {
		msg.members.push_back(*it);
	}
	msg.operation = operation;
	return GProviderClient::DispatchProtocol(100, msg);
}
// Add End

bool SendChangeKingdomPoint(int point)
{
	GKingdomPointChange msg;
	msg.delta = point;

	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendKingTryCallGuard(int roleid, int map_id, float pos_x, float pos_y, float pos_z)
{
	GKingTryCallGuards msg;
	msg.roleid = roleid;
	msg.mapid = map_id;
	msg.posx = pos_x;
	msg.posy = pos_y;
	msg.posz = pos_z;

	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendGuardCalledByKing(char * kingname, size_t kingname_len, int cs_index, int localsid, int roleid, int line_id,  int map_id, float pos_x, float pos_y, float pos_z) 
{
	KingGuardInviteArg arg;
	arg.kingname = Octets(kingname, kingname_len);
	arg.localsid = localsid;
 
	KingGuardInvite *rpc =(KingGuardInvite*) Rpc::Call(RPC_KINGGUARDINVITE, arg);
	rpc->roleid = roleid;
	rpc->line_id = line_id;
	rpc->map_id = map_id;
	rpc->pos_x = pos_x;
	rpc->pos_y = pos_y;
	rpc->pos_z = pos_z;

	return GProviderClient::DispatchProtocol(cs_index, rpc);
}


bool SendSyncBathCount(int id, int count)
{
	SyncBathTimes msg;
	msg.roleid = id;
	msg.times = count;
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendFacBasePropChange(int roleid, int fid, int prop_type, int delta)
{
	FacBasePropChange msg(roleid, fid, prop_type, delta, 0);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendGetCS6V6Info(int roleid, int type, int level)
{
	GCrssvrTeamsGetScore msg;
	msg.roleid = roleid;
	msg.award_type = type;
	msg.award_level = level;
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendFacBaseBuildingProgress(int roleid, int fid, int task_id, int value)
{
	FacBaseBuildingProgress msg(roleid, fid, task_id, value, 0);
	return GProviderClient::DispatchProtocol(0, msg);
}

bool SendFacBasePutAuction(int roleid, const void * name, size_t name_len, int fid, int itemid)
{
	FacAuctionPut put(roleid, Octets(name, name_len), fid, itemid, 0);
	return GProviderClient::DispatchProtocol(0, put);
}

bool SendFacDynDonateCash(int fid, const void * name, size_t name_len, int cash)
{
	FacDynDonateCash dyn(fid, Octets(name, name_len), cash);
	return GProviderClient::DispatchProtocol(0, dyn);
}

bool SendFacDynPutAuction(int fid, const void * name, size_t name_len, int itemid, int time, int task)
{
	FacDynPutAuction dyn(fid, Octets(name, name_len), itemid, time, task);
	return GProviderClient::DispatchProtocol(0, dyn);
}

bool SendFacDynWinAuction(int fid, const void * name, size_t name_len, int itemid, int cost)
{
	FacDynWinAuction dyn(fid, Octets(name, name_len), itemid, cost);
	return GProviderClient::DispatchProtocol(0, dyn);
}

bool SendFacDynBuyAuction(int fid, const void * name, size_t name_len, int itemid, int time)
{
	FacDynBuyAuction dyn(fid, Octets(name, name_len), itemid, time);
	return GProviderClient::DispatchProtocol(0, dyn);
}

bool SendFacDynBuildingUpgrade(int fid, const void * name, size_t name_len, int tid, int level, int grass, int mine, int food, int core)
{
	FacDynBuildingUpgrade dyn(fid, Octets(name, name_len), tid, level, grass, mine, food, core);
	return GProviderClient::DispatchProtocol(0, dyn);
}

bool SendFacDynBuildingComplete(int fid, int tid, int level)
{
	FacDynBuildingComplete dyn(fid, tid, level);
	return GProviderClient::DispatchProtocol(0, dyn);
}

bool SendFacMallChange(int fid, const std::set<int> & mall)
{
	return GProviderClient::DispatchProtocol(0, FacMallChange(fid, mall));
}

bool SendFacBaseData(int linkid, int roleid,int sid,const void * buf, size_t size)
{
	//linkid 可以为0 表示发给deliveryd 通过gdeliveryd转发到客户端
	return GProviderClient::DispatchProtocol(linkid, FacBaseDataSend(roleid, sid, 0, Octets(buf, size), 0));
}

bool DeliverFacBaseCMD(int roleid, int fid, const void * buf, size_t size)
{
	return GProviderClient::DispatchProtocol(0, FacBaseDataDeliver(roleid, fid, Octets(buf, size)));
}

bool BroadcastFacBaseData(int fid,const void * buf, size_t size) //基地信息全帮派广播
{
	return GProviderClient::DispatchProtocol(0, FacBaseDataBroadcast(fid, Octets(buf, size)));
}
// Youshuang add
bool DeliverTopicSite( int64_t l_roleid,int l_event_type,const void* buf, size_t size )
{
	return GProviderClient::DispatchProtocol( 100, SyncRoleEvent2SNS( l_roleid, l_event_type, Octets(buf, size) ) );
}

bool SendFacCouponAdd( int64_t roleid,  int64_t fac_coupon_add )
{
	return GProviderClient::DispatchProtocol( 0, SyncPlayerFacCouponAdd( roleid, fac_coupon_add ) );
}

bool SendFactionMultiExp(int fid, int multi, int end_time)
{
	return GProviderClient::DispatchProtocol(0, StartFactionMultiExp(fid, multi, end_time));
}

// end

bool SendApplyHideAndSeekRaid(int raid_id, std::vector<int> & apply_vec)
{
	GHideSeekRaidApply msg;
	msg.map_id = raid_id;

	std::vector<TeamRaidApplyingRole> role_info;
	TeamRaidApplyingRole role;
	for (size_t i = 0; i < apply_vec.size(); ++i)
	{
		role.roleid = apply_vec[i];	
		role_info.push_back(role);
	}
	msg.roles = role_info;	
	msg.issrc = 0;

	return GProviderClient::DispatchProtocol(0,msg);
}

bool SendApplyCaptureRaid(int raid_id, std::vector<int> & apply_vec)
{
	GFengShenRaidApply msg;
	msg.map_id = raid_id;

	std::vector<TeamRaidApplyingRole> role_info;
	TeamRaidApplyingRole role;
	for (size_t i = 0; i < apply_vec.size(); ++i)
	{
		role.roleid = apply_vec[i];	
		role_info.push_back(role);
	}
	msg.roles = role_info;	
	msg.issrc = 0;

	return GProviderClient::DispatchProtocol(0,msg);
}

}
