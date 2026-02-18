#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "ch_player.h"
#include "../usermsg.h"
#include "../actsession.h"
#include "../userlogin.h"
#include "../playertemplate.h"
#include "../serviceprovider.h"
#include <common/protocol_imp.h>
#include "../task/taskman.h"
#include "../playerstall.h"
#include "../pvplimit_filter.h"
#include <glog.h>
#include "../pathfinding/pathfinding.h"
#include "../player_mode.h"
#include "../cooldowncfg.h"
#include "../template/globaldataman.h"
#include "../petnpc.h"
#include "../item_manager.h"
#include "../netmsg.h"
#include "../mount_filter.h"
#include "bg_world_manager.h"
#include "../faction.h"
#include <vector>

DEFINE_SUBSTANCE( ch_player_imp, gplayer_imp, CLS_CH_PLAYER_IMP )


void ch_player_imp::OnDeath( const XID& lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time)
{
	gplayer_imp::OnDeath( lastattack, is_pariah, true, false, 0);
}


