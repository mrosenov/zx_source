#include "../clstab.h"
#include "bf_matter_flag_imp.h"
#include "bf_world_manager.h"
/*

DEFINE_SUBSTANCE( bf_matter_flag_imp, gmatter_mine_imp, CLS_bf_MATTER_FLAG_IMP )


void bf_matter_flag_imp::Init(world * pPlane,gobject*parent)
{
	gmatter_imp::Init(pPlane, parent);

	gmatter * pMatter = (gmatter*) parent;
	pMatter->matter_state = gmatter::STATE_MASK_BATTLE_FLAG;
	pMatter->battle_flag = gmatter::BATTLE_FLAG_NEUTRAL;
}

int bf_matter_flag_imp::MessageHandler( const MSG& msg )
{
	gmatter * pMatter = (gmatter*) _parent;
	switch( msg.message )
	{
		case GM_MSG_GATHER_REQUEST:
			{
				if( !IsBattleOffense(msg.param) && !IsBattleDefence(msg.param))
				{
					// 其他人拔旗子，不允许\n
					return 0;
				}
				if( ( IsBattleOffense(msg.param) && pMatter->IsAttackerFlag())
					|| ( IsBattleDefence(msg.param) && pMatter->IsDefenderFlag()) )
				{
					// 已经属于自己方的旗子不能在拔了
					return 0;
				}
			}
			gmatter_mine_imp::MessageHandler( msg );
			break;
			
		default:
			gmatter_mine_imp::MessageHandler( msg );
	}
	return 0;
}

void bf_matter_flag_imp::PrepareMine(const MSG & msg)
{
	_gather_faction = msg.param;
}

void bf_matter_flag_imp::OnMined( int roleid )
{
	gmatter * pMatter = (gmatter*) _parent;
	world_manager * pManager = GetWorldManager();
	__PRINTF( "旗子 %d 被 %d 抗\n", pMatter->matter_type, roleid );
	if(  IsBattleOffense(_gather_faction) && !pMatter->IsAttackerFlag())
	{
		pManager->BattleChangeFlag( pMatter->matter_type, pMatter->battle_flag, gmatter::BATTLE_FLAG_ATTACKER, roleid );
		_runner->battle_flag_change(pMatter->battle_flag, gmatter::BATTLE_FLAG_ATTACKER);
		pMatter->battle_flag =gmatter::BATTLE_FLAG_ATTACKER;
		return;
	}

	if(  IsBattleDefence(_gather_faction) && !pMatter->IsDefenderFlag())
	{
		pManager->BattleChangeFlag( pMatter->matter_type, pMatter->battle_flag, gmatter::BATTLE_FLAG_DEFENDER, roleid );
		_runner->battle_flag_change(pMatter->battle_flag, gmatter::BATTLE_FLAG_DEFENDER);
		pMatter->battle_flag =gmatter::BATTLE_FLAG_DEFENDER;
		return;
	}
}

void bf_matter_flag_imp::Reborn()
{
	gmatter * pMatter = (gmatter*) _parent;
	pMatter->battle_flag = gmatter::BATTLE_FLAG_NEUTRAL;
	gmatter_mine_imp::Reborn();
}
*/
