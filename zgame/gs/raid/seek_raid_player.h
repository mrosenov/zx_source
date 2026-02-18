#ifndef __ONLINEGAME_GS_SEEK_RAID_PLAYER_H
#define __ONLINEGAME_GS_SEEK_RAID_PLAYER_H

#include "raid_player.h"

class seek_raid_player_imp : public raid_player_imp
{
public:
	DECLARE_SUBSTANCE(seek_raid_player_imp);
	seek_raid_player_imp();
	virtual ~seek_raid_player_imp();
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual int MessageHandler(const MSG& msg);
	bool SetSeekRaidRandomSkillAttack(unsigned int & skill_id);
	void BeHurtOnSeekAndHideRaid(const XID & attacker);
	void SendClientHiderTaunted(bool isStart);
	void GetSeekAndHiderTauntedScore();
	void SendClientHiderBloodNum(int seeker_id, char blood_num);
	void SendClientSeekerSkillInfo(char skill_left,char skill_buy_num);
	void SendClientHideAndSeekRole(int player_id, char role_type);
	virtual void HandleRaidResult();
	void PlayerEnterSeekRaid();
	void SendClientRaidRoundInfo(int step, int end_timestamp);
};

#endif

