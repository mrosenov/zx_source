#ifdef WIN32
#include <winsock2.h>
#include "gncompress.h"
#else
#include "binder.h"
#endif
#include "precreaterole.hrp"
#include "postcreaterole.hrp"
#include "postdeleterole.hrp"
#include "postcrssvrteamscreate.hrp"
#include "postcrssvrteamsrename.hrp"
#include "postcrssvrteamsdel.hrp"
#include "precreatefaction.hrp"
#include "precrssvrteamscreate.hrp"
#include "precrssvrteamsrename.hrp"
#include "postcreatefaction.hrp"
#include "postdeletefaction.hrp"
#include "rolenameexists.hrp"
#include "userrolecount.hrp"
#include "moverolecreate.hrp"
#include "precreatefamily.hrp"
#include "postcreatefamily.hrp"
#include "postdeletefamily.hrp"
#include "dbrawread.hrp"
#include "prechangerolename.hrp"
#include "postchangerolename.hrp"
#include "prechangefactionname.hrp"
#include "postchangefactionname.hrp"
#include "keepalive.hpp"

namespace GNET
{

static PreCreateRole __stub_PreCreateRole (RPC_PRECREATEROLE, new PreCreateRoleArg, new PreCreateRoleRes);
static PostCreateRole __stub_PostCreateRole (RPC_POSTCREATEROLE, new PostCreateRoleArg, new PostCreateRoleRes);
static PostDeleteRole __stub_PostDeleteRole (RPC_POSTDELETEROLE, new PostDeleteRoleArg, new PostDeleteRoleRes);
static PostCrssvrTeamsCreate __stub_PostCrssvrTeamsCreate (RPC_POSTCRSSVRTEAMSCREATE, new PostCrssvrTeamsCreateArg, new PostCrssvrTeamsCreateRes);
static PostCrssvrTeamsRename __stub_PostCrssvrTeamsRename (RPC_POSTCRSSVRTEAMSRENAME, new PostCrssvrTeamsRenameArg, new PostCrssvrTeamsRenameRes);
static PostCrssvrTeamsDel __stub_PostCrssvrTeamsDel (RPC_POSTCRSSVRTEAMSDEL, new PostCrssvrTeamsDelArg, new PostCrssvrTeamsDelRes);
static PreCreateFaction __stub_PreCreateFaction (RPC_PRECREATEFACTION, new PreCreateFactionArg, new PreCreateFactionRes);
static PreCrssvrTeamsCreate __stub_PreCrssvrTeamsCreate (RPC_PRECRSSVRTEAMSCREATE, new PreCrssvrTeamsCreateArg, new PreCrssvrTeamsCreateRes);
static PreCrssvrTeamsRename __stub_PreCrssvrTeamsRename (RPC_PRECRSSVRTEAMSRENAME, new PreCrssvrTeamsRenameArg, new PreCrssvrTeamsRenameRes);
static PostCreateFaction __stub_PostCreateFaction (RPC_POSTCREATEFACTION, new PostCreateFactionArg, new PostCreateFactionRes);
static PostDeleteFaction __stub_PostDeleteFaction (RPC_POSTDELETEFACTION, new PostDeleteFactionArg, new PostDeleteFactionRes);
static RolenameExists __stub_RolenameExists (RPC_ROLENAMEEXISTS, new RolenameExistsArg, new RolenameExistsRes);
static UserRoleCount __stub_UserRoleCount (RPC_USERROLECOUNT, new UserRoleCountArg, new UserRoleCountRes);
static MoveRoleCreate __stub_MoveRoleCreate (RPC_MOVEROLECREATE, new MoveRoleCreateArg, new MoveRoleCreateRes);
static PreCreateFamily __stub_PreCreateFamily (RPC_PRECREATEFAMILY, new PreCreateFamilyArg, new PreCreateFamilyRes);
static PostCreateFamily __stub_PostCreateFamily (RPC_POSTCREATEFAMILY, new PostCreateFamilyArg, new PostCreateFamilyRes);
static PostDeleteFamily __stub_PostDeleteFamily (RPC_POSTDELETEFAMILY, new PostDeleteFamilyArg, new PostDeleteFamilyRes);
static DBRawRead __stub_DBRawRead (RPC_DBRAWREAD, new DBRawReadArg, new DBRawReadRes);
static PreChangeRolename __stub_PreChangeRolename (RPC_PRECHANGEROLENAME, new PreChangeRolenameArg, new PreChangeRolenameRes);
static PostChangeRolename __stub_PostChangeRolename (RPC_POSTCHANGEROLENAME, new PostChangeRolenameArg, new PostChangeRolenameRes);
static PreChangeFactionname __stub_PreChangeFactionname (RPC_PRECHANGEFACTIONNAME, new PreChangeFactionnameArg, new PreChangeFactionnameRes);
static PostChangeFactionname __stub_PostChangeFactionname (RPC_POSTCHANGEFACTIONNAME, new PostChangeFactionnameArg, new PostChangeFactionnameRes);
static KeepAlive __stub_KeepAlive((void*)0);

};
