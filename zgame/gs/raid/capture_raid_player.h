#ifndef __ONLINEGAME_GS_CAPTURE_RAID_PLAYER_H
#define __ONLINEGAME_GS_CAPTURE_RAID_PLAYER_H

#include "raid_player.h"

class capture_raid_player_imp : public raid_player_imp
{
public:
	enum
	{
		CAPTURE_CLEAR_SKILL_ID = 3290,
		CAPTURE_CLEAR_SKILL_ID2 = 3170,
		CAPTURE_FLAG_TRANSFORM_ID = 5349,
		ATTACKER_MATTER_ID = 63234, //ÒþÏÉ¸ó¿ó
		DEFENDER_MATTER_ID = 63231, //ÒÝÁúÐù¿ó
		ATTACKER_MATTER_ITEM_ID = 63233, //ÒþÏÉ¸ó¾üÆì
		DEFENDER_MATTER_ITEM_ID = 63232, //ÒÝÁúÐù¾üÆì
		AWARD_ITEM_ID = 63340 ,
		AWARD_ITEM_ID2 = 63388 ,
		CAPTURE_FLAG_TRANSFORM_SKILL0 = 5350,
		CAPTURE_FLAG_TRANSFORM_SKILL1 = 5351,
	};

	DECLARE_SUBSTANCE(capture_raid_player_imp);
	capture_raid_player_imp();
	virtual ~capture_raid_player_imp();

	virtual int MessageHandler(const MSG& msg);
	virtual void PlayerEnter();
	virtual void PlayerEnterWorld();
	virtual void OnGatherMatter(int matter_tid, int cnt);
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void OnResurrect();
	virtual void HandleDeath();
	virtual void PlayerSubmitCaptureRaidFlag();
	virtual int CheckCanMineInCaptureRaid(int matter_id);
	virtual void HandleRaidResult();
	void PlayerEnterCaptureRaid();
	virtual void OnHeartbeat( size_t tick);
	virtual bool StepMove(const A3DVECTOR &offset);
	

private:
	int _revive_counter;		//Ç¿ÖÆ¸´»î¼ÆÊ±Æ÷
	int _transform_counter;
	bool _is_flag_player;

	enum
	{
		REVIVE_MAX_TIMEOUT = 10,
	};
	
};

#endif
