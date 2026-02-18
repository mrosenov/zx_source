#ifndef _GNET_SNS_PLAYER_INFO_MANAGER_H_
#define _GNET_SNS_PLAYER_INFO_MANAGER_H_ 

#include "gsnsroleinfo"
#include "snsroleinfo"
#include "gsnsplayerinfo"
#include "snsplayerinfo"
#include "snsplayerinfocache"
#include "gsnsplayerinfocache"
#include "localmacro.h"

#include <map>
#include <vector>
#include <algorithm>

using std::map;
using std::vector;
using std::upper_bound;

namespace GNET
{

template <typename SNSROLEINFO1, typename SNSROLEINFO2>
static void GSNSToSNSRoleInfo(const SNSROLEINFO1 &from, SNSROLEINFO2 &to)
{
	to.level = from.level;
	to.sub_level = from.sub_level;
	to.gender = from.gender;
	to.occupation = from.occupation;
	to.spouse = from.spouse;
	if (from.rolename.size() > 0)
		to.rolename = from.rolename;
	if (from.sectname.size() > 0)
		to.sectname = from.sectname;
	if (from.familyname.size() > 0)
		to.familyname = from.familyname;
	if (from.factionname.size() > 0)
	{
		to.factionname = from.factionname;
		to.factiontitle = from.factiontitle;
		to.faction_level = from.faction_level;
	}
}

template <typename SNSPLAYERINFO1, typename SNSPLAYERINFO2>
static void GSNSToSNSPlayerInfo(const SNSPLAYERINFO1 &from, SNSPLAYERINFO2 &to)
{
	to.roleid = from.roleid;
	to.is_private = from.is_private;
	if (from.rolename.size() > 0)
		to.rolename = from.rolename;
	to.nickname = from.nickname;
	to.age = from.age;
	to.birthday = from.birthday;
	to.province = from.province;
	to.city = from.city;
	to.career = from.career;
	to.address = from.address;
	to.introduce = from.introduce;
	to.charm_personal = from.charm_personal;
	to.charm_friend = from.charm_friend;
	to.charm_sect = from.charm_sect;
	to.charm_faction = from.charm_faction;
	to.press_time = from.press_time;
}

template <>
static void GSNSToSNSPlayerInfo<GSNSPlayerInfo, SNSPlayerInfo> (const GSNSPlayerInfo &from, SNSPlayerInfo &to)
{
	to.roleid = from.roleid;
	to.is_private = from.is_private;
	if (from.rolename.size() > 0)
		to.rolename = from.rolename;
	to.nickname = from.nickname;
	to.age = from.age;
	to.birthday = from.birthday;
	to.province = from.province;
	to.city = from.city;
	to.career = from.career;
	to.address = from.address;
	to.introduce = from.introduce;
	to.charm_personal = from.charm_personal;
	to.charm_friend = from.charm_friend;
	to.charm_sect = from.charm_sect;
	to.charm_faction = from.charm_faction;
	to.press_time = from.press_time;
	to.apply_time = to.leavemsg_time = to.vote_time = 0;
}

template <typename SNSPLAYERINFOCACHE1, typename SNSPLAYERINFOCACHE2>
static void GSNSToSNSPlayerInfoCache(const SNSPLAYERINFOCACHE1 &from, SNSPLAYERINFOCACHE2 &to)
{
	GSNSToSNSRoleInfo(from.roleinfo, to.roleinfo);
	GSNSToSNSPlayerInfo(from.playerinfo, to.playerinfo);
}

class FactionDetailInfo;
class SNSPlayerInfoManager
{
public:
	static SNSPlayerInfoManager *GetInstance() { static SNSPlayerInfoManager instance; return &instance; }

	bool Initialize();
	
	//服务器重启以后，从数据库加载发布过信息的用户
	void LoadSNSPlayerList(vector<int> &roleid_list);

	void OnLoad(const GSNSPlayerInfoCache &infocache)
	{
		GSNSToSNSPlayerInfoCache(infocache, m_cache[infocache.playerinfo.roleid]);
	}

	void OnLoad(const GSNSPlayerInfo &info);

	void OnLoad(int roleid, const GSNSRoleInfo &info)
	{
		GSNSToSNSRoleInfo(info, m_cache[roleid].roleinfo);
		SNSPlayerInfo &pinfo = m_cache[roleid].playerinfo;
		if (info.rolename.size() > 0)
			pinfo.rolename = info.rolename;
		pinfo.roleid = roleid;
	}

	//更新用户SNS信息
	void UpdateSNSPlayerInfo(const SNSPlayerInfo &info);

	void UpdateSNSRoleInfo(int roleid);

	void UpdateSNSRoleLevel(int roleid, unsigned char level);

	void UpdateSNSPlayerCharm(SNS_CHARM_TYPE type, int roleid, int charm_diff);

	void SetSNSPlayerCharm(SNS_CHARM_TYPE type, int roleid, int charm);

	int GetSNSPlayerCharm(SNS_CHARM_TYPE type, int roleid)
	{
		if (!IsPlayerExist(roleid))
		{
			LOG_TRACE("SNSPlayerInfoManager::GetSNSPlayerCharm, fail to find roleid=%d\n", roleid);
			return 0;
		}

		int charm = 0;
		switch (type)
		{
		case SNS_CHARM_TYPE_PERSONAL:
			charm = m_cache[roleid].playerinfo.charm_personal; 
			break;
		case SNS_CHARM_TYPE_FRIEND:
			charm = m_cache[roleid].playerinfo.charm_friend; 
			break;
		case SNS_CHARM_TYPE_SECT:
			charm = m_cache[roleid].playerinfo.charm_sect; 
			break;
		case SNS_CHARM_TYPE_FACTION:
			charm = m_cache[roleid].playerinfo.charm_faction; 
			break;
		default:
			break;
		}

		return charm;
	}

	//获得用户在某类信息上人气星级
	int GetSNSPlayerCharmStar(SNS_CHARM_TYPE type, int roleid)
	{
		int charm = GetSNSPlayerCharm(type, roleid);
		int *begin = &STAR_CHARM[0];
		int *end = &STAR_CHARM[SNS_LIMIT_CHARMSTAR+1];
		int *index = upper_bound(begin, end, charm);

		if (index == end)
			return SNS_LIMIT_CHARMSTAR;
		else if (index == begin)
			return 0;
		else
			return ((size_t)index-(size_t)begin)/sizeof(int)-1;
	}

	void UpdateSNSPlayerPressTime(int roleid, int time);

	int GetSNSPlayerPressTime(int roleid)
	{
		if (IsPlayerExist(roleid))
			return m_cache[roleid].playerinfo.press_time;
		else
			return 0;
	}

	void UpdateSNSPlayerApplyTime(int roleid, int time);

	int GetSNSPlayerApplyTime(int roleid)
	{
		if (IsPlayerExist(roleid))
			return m_cache[roleid].playerinfo.apply_time;
		else
			return 0;
	}

	void UpdateSNSPlayerLeaveMsgTime(int roleid, int time);

	int GetSNSPlayerLeaveMsgTime(int roleid)
	{
		if (IsPlayerExist(roleid))
			return m_cache[roleid].playerinfo.leavemsg_time;
		else
			return 0;
	}

	void UpdateSNSPlayerVoteTime(int roleid, int time);

	int GetSNSPlayerVoteTime(int roleid)
	{
		if (IsPlayerExist(roleid))
			return m_cache[roleid].playerinfo.vote_time;
		else
			return 0;
	}

	//用户登录时，从数据库获取该用户的信息
	void OnPlayerLogin(int roleid);

	void OnPlayerLogout(int roleid);

	bool GetSNSPlayerInfo(int roleid, SNSPlayerInfoCache &info)
	{
		map<int, SNSPlayerInfoCache>::iterator itr = m_cache.find(roleid);
		if (itr != m_cache.end())
		{
			info = itr->second;
			return true;
		}
		else
		{
			LOG_TRACE("SNSPlayerInfoManager::GetSNSPlayerInfo, fail to find roleid=%d cachesize=%d\n", roleid, m_cache.size());
			return false;
		}
	}

	void OnFactionAppointMaster(const FactionDetailInfo *pfaction, int master, int candidate);

	void OnFactionDelete(const FactionDetailInfo *pfaction);

	void OnRolenameChange(int roleid, const Octets & newname);

private:
	SNSPlayerInfoManager () : open(false) {}
	bool IsPlayerExist(int roleid) { return m_cache.find(roleid)!=m_cache.end(); }

	void SyncDB(const SNSPlayerInfo &info);
	void SyncDB(int roleid, const SNSRoleInfo &info);
	void SyncDB(int roleid, const GSNSRoleInfo &info);

public:
	typedef map<int, SNSPlayerInfoCache> PLAYERMAP;

private:
	bool open;
	PLAYERMAP m_cache;
	static int STAR_CHARM[SNS_LIMIT_CHARMSTAR+1];
};

};
#endif
