#ifndef __ONLINEGAME_GS_BG_MATTER_FLAG_IMP_H__
#define __ONLINEGAME_GS_BG_MATTER_FLAG_IMP_H__

#include "../matter.h"
#include "../faction.h"

/*

class bg_matter_flag_imp : public gmatter_mine_imp
{
	inline bool IsBattleOffense(int faction)
	{
		return faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND );
	}

	inline bool IsBattleDefence(int faction)
	{
		
		return faction & ( FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND );
	}
public:

	DECLARE_SUBSTANCE( bg_matter_flag_imp );
	int _gather_faction;
public:
	bg_matter_flag_imp():_gather_faction(0)
	{ }

	virtual void Init(world * pPlane,gobject*parent);
	virtual int MessageHandler( const MSG& msg );
	virtual void OnMined( int roleid );
	virtual void PrepareMine(const MSG & msg);
	virtual void Reborn();

};
*/

#endif
