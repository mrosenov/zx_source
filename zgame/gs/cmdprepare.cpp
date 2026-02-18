#include <common/protocol.h>
#include <strtok.h>
#include "cmdprepare.h"

namespace
{
struct cmd_node_t
{
	int command;
	const char * valid_state;
	const char * invalid_state;		//这些是要禁止的状态，和valid状态同时使用

	const char * spec_state;
	const char * spec_handler;
};

//"all null"
//"normal trade market bind dead seal root"
using namespace C2S;
typedef player_state T;
#define ALWAYS_INVALID_STATE (1 << T::STATE_DISCONNECT)
//#define ALWAYS_INVALID_STATE 0
struct 
{
	const char * name;
	unsigned int mask;
} mask_list[] =
{
	"all"	, 0xFFFFFFFF,
	"null"	, 0,
	"normal", (1 << T::STATE_NORMAL),
	"trade"	, (1 << T::STATE_WAIT_TRADE) | (1 << T::STATE_TRADE) | (1 << T::STATE_WAIT_TRADE_COMPLETE) | 
		  (1 << T::STATE_WAIT_TRADE_READ) | (1 << T::STATE_WAIT_FACTION_TRADE) | 
		  (1 << T::STATE_WAIT_FACTION_TRADE_READ),
	"non-trade",~((1 << T::STATE_WAIT_TRADE) | (1 << T::STATE_TRADE) | 
		  (1 << T::STATE_WAIT_TRADE_COMPLETE) | (1 << T::STATE_WAIT_TRADE_READ) | 
		  (1 << T::STATE_WAIT_FACTION_TRADE) | (1 << T::STATE_WAIT_FACTION_TRADE_READ)),
	"market", (1 << T::STATE_MARKET) ,
	"bind", (1 << T::STATE_BIND),

	"idle", (1 << T::STATE_IDLE),
	"silent", (1 << T::STATE_SEAL_SILENT),
	"noattack", (1 << T::STATE_SEAL_MELEE),
	"diet", (1 << T::STATE_SEAL_DIET),
	"root", (1 << T::STATE_SEAL_ROOT),
	"SEAL", (1 << T::STATE_SEAL_SILENT)|(1<<T::STATE_SEAL_MELEE)|(1<<T::STATE_SEAL_DIET),
	"dead",	(1 << T::STATE_DEAD),	
	NULL,
};

cmd_node_t cmd_list[] =
{
	{PLAYER_MOVE, 		"normal bind"		,"dead root idle"	,"null" 	, ""},
	{LOGOUT, 		"normal dead bind market","idle SEAL root trade","null" 	, ""},
	{SELECT_TARGET, 	"all"			,"null"			,"null"		, ""},
	{NORMAL_ATTACK, 	"normal bind"		,"dead noattack idle"	,"null" 	, ""},
	{RESURRECT_IN_TOWN, 	"dead"			,"trade"		,"null"		, ""},
	
//	{RESURRECT_BY_ITEM, 	"dead"			,"null"			,"null"		, ""},
	{PICKUP,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{STOP_MOVE, 		"normal bind" 		,"dead root idle"	,"null" 	, ""},
	{UNSELECT,     		"all"			,"null"			,"null"		, ""},
	{GET_ITEM_INFO,		"all"			,"null"			,"null"		, ""},

	{GET_INVENTORY,		"all"			,"null"			,"null"		, ""},
	{GET_INVENTORY_DETAIL,   "all"			,"null"			,"null"		, ""},
	{EXCHANGE_INVENTORY_ITEM,"all"			,"trade market"		,"all"		, "UnlockHandler"},
	{MOVE_INVENTORY_ITEM,	"all"			,"trade market"		,"all"		, "UnlockHandler"},
	{DROP_INVENTORY_ITEM,	"normal bind"		,"market dead root idle"	,"all"		, "UnlockHandler"},
	
	{DROP_EQUIPMENT_ITEM,	"normal bind"		,"dead root idle"	,"all"		, "UnlockHandler"},
	{EXCHANGE_EQUIPMENT_ITEM,"normal bind"		,"dead root idle"	,"all"		, "UnlockHandler"},
	{EQUIP_ITEM,             "normal bind"		,"dead root idle"	,"all"		, "UnlockHandler"},
	{MOVE_ITEM_TO_EQUIPMENT,"normal bind"		,"dead root idle"	,"all"		, "UnlockHandler"},
//	{GOTO,			"null"			,"null"		, ""},

	{DROP_MONEY,      	"normal bind"		,"dead root idle"	,"null"		, ""},
	{SELF_GET_PROPERTY,	"all"			,"null"			,"null"		, ""},
	{LEARN_SKILL, 		"normal bind market"	,"dead"			,"null"		, ""},
	{GET_EXTPROP_BASE,	"all"			,"null"			,"null"		, ""},
	{GET_EXTPROP_MOVE,	"all"			,"null"			,"null"		, ""},

	{GET_EXTPROP_ATTACK,	"all"			,"null"			,"null"		, ""},
	{GET_EXTPROP_DEFENSE,	"all"			,"null"			,"null"		, ""},
	{TEAM_INVITE,		"all"			,"null"			,"null"		, ""},
	{TEAM_AGREE_INVITE,	"all"			,"null"			,"null"		, ""},
	{TEAM_REJECT_INVITE,	"all"			,"null"			,"null"		, ""},

	{TEAM_LEAVE_PARTY,	"all"			,"null"			,"null"		, ""},
	{TEAM_KICK_MEMBER,	"all"			,"null"			,"null"		, ""},
	{TEAM_GET_TEAMMATE_POS,	"all"			,"null"			,"null"		, ""},
	{GET_OTHERS_EQUIPMENT,	"all"			,"null"			,"null"		, ""},
	{CHANGE_PICKUP_FLAG,	"all"			,"null"			,"null"		, ""},

	{SERVICE_HELLO,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{SERVICE_GET_CONTENT,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{SERVICE_SERVE,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{GET_OWN_WEALTH,	"all"			,"null"			,"null"		, ""},
	{GET_ALL_DATA,		"all"			,"null"			,"null"		, ""},

	{USE_ITEM,		"normal bind"		,"dead idle diet"	,"null"		, ""},	
	{CAST_SKILL,		"normal bind"		,"dead silent idle"	,"null" 	, ""},
	{CANCEL_ACTION,		"normal bind"		,"dead idle"		,"null"		, ""},
//	RECHARGE_EQUIPPED_FLYSWORD,
//	RECHARGE_FLYSWORD,

	{USE_ITEM_WITH_TARGET,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{SIT_DOWN,		"normal"		,"dead root idle SEAL"	,"null"		, ""},
//	STAND_UP,
	{EMOTE_ACTION,		"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{TASK_NOTIFY,		"non-trade"		,"null"			,"null"	, ""},

	{ASSIST_SELECT,		"non-trade"		,"null"			,"null"	, ""},
	{CONTINUE_ACTION,	"normal bind"		,"dead idle silent"	,"null"		, ""},
	{STOP_FALL,      	"normal bind"		,"dead idle root"	,"null"		, ""},
	{GET_ITEM_INFO_LIST,	"all"			,"null"			,"null"		, ""},
	{GATHER_MATERIAL,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	
	{GET_TRASHBOX_INFO,	"all"			,"null"			,"null"		, ""},
	{EXCHANGE_TRASHBOX_ITEM,"normal"		,"dead root idle"	,"all"		, "UnlockHandler"},
	{MOVE_TRASHBOX_ITEM,	"normal"		,"dead root idle"	,"all"		, "UnlockHandler"},
	{EXHCANGE_TRASHBOX_INVENTORY,"normal"		,"dead root idle"	,"all"		, "UnlockHandler"},
	{MOVE_TRASHBOX_ITEM_TO_INVENTORY,"normal"	,"dead root idle"	,"all"		, "UnlockHandler"},

	{MOVE_INVENTORY_ITEM_TO_TRASHBOX,"normal"	,"dead root idle"	,"all"		, "UnlockHandler"},
	{EXCHANGE_TRASHBOX_MONEY,	"normal"	,"dead root idle"	,"all"		, "UnlockHandler"},
//	{TRICKS_ACTION,		"normal"		,"null"		, ""},
	{SET_ADV_DATA,		"normal bind"		,"daed"			,"null"		, ""},
	{CLR_ADV_DATA,		"normal bind"		,"dead"			,"null"		, ""},

	{TEAM_LFG_REQUEST,	"all"			,"null"			,"null"		, ""},
	{TEAM_LFG_REPLY,	"all"			,"null"			,"null"		, ""},
	{QUERY_PLAYER_INFO_1,	"all"			,"null"			,"null"		, ""},
	{QUERY_NPC_INFO_1,	"all"			,"null"			,"null"		, ""},
	{SESSION_EMOTE_ACTION,	"normal"		,"bind dead root idle SEAL","null"		, ""},

//	CONCURRECT_EMOTE_REQUEST,
//	CONCURRECT_EMOTE_REPLY,
	{TEAM_CHANGE_LEADER,	"all"			,"null"			,"null"		, ""},
//	DEAD_MOVE,
//	DEAD_STOP_MOVE,

	{ENTER_SANCTUARY,	"normal bind"		,"dead idle"		,"null"		, ""},
	{OPEN_PERSONAL_MARKET,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{CANCEL_PERSONAL_MARKET,"market"		,"dead root idle SEAL"	,"null"		, ""},
	{QUERY_PERSONAL_MARKET_NAME,"normal"		,"dead root idle SEAL"	,"null"		, ""},
//	COMPLETE_TRAVEL,	

	{CAST_INSTANT_SKILL,	"normal bind"		,"dead idle silent"	,"null"		, ""},
//	DESTROY_ITEM,
//	{ENABLE_PVP_STATE,	"all"			,"null"			,"null"		, ""},
//	{DISABLE_PVP_STATE,	"all"			,"null"			,"null"		, ""},
	{TEST_PERSONAL_MARKET,	"all"			,"null"			,"null"		, ""},


	{SWITCH_FASHION_MODE,	"all"			,"null"			,"null"		, ""},
	{REGION_TRANSPORT,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{RESURRECT_AT_ONCE, 	"dead"			,"trade"		,"null"		, ""},
	{NOTIFY_POS_TO_MEMBER,	"normal bind"		,"dead"			,"null"		, ""},
	{CAST_POS_SKILL,	"normal"                ,"dead root idle silent","null"         , ""},

//	{ACTIVE_RUSH_MODE,	"normal bind"		,"root idle"		,"null"		, ""},
	{QUERY_DOUBLE_EXP_INFO,	"all"			,"null"			,"null"		, ""},
	{DUEL_REQUEST,		"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{DUEL_REPLY,		"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{BIND_PLAYER_REQUEST,	"normal"		,"dead root idle SEAL"	,"null"		, ""},

	
	{BIND_PLAYER_INVITE,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{BIND_PLAYER_REQUEST_REPLY,"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{BIND_PLAYER_INVITE_REPLY,"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{BIND_PLAYER_CANCEL,	"bind"			,"dead idle"		,"null"		, ""},
	{QUERY_OTHER_EQUIP_DETAIL,"non-trade"		,"dead"			,"null"		, ""},


	{PRODUCE_ITEM,		"normal"		,"dead"			,"null"		, ""},
	
//	{MALL_SHOPPING,		"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{SELECT_TITLE,		"normal bind"		,"dead"			,"null"		,""},
	{DEBUG_DELIVERY_CMD,	"null"			,"null"			,"all"		,"DebugHandler"},
	{DEBUG_GS_CMD,		"null"			,"null"			,"all"		,"DebugHandler"},
	{LOTTERY_CASHING,	"normal bind"		,"dead idle diet"	,"null"		, ""},	

	{CHECK_RECORDER,	"all"			,"null"			,"null"		, ""},	
	{MALL_SHOPPING2,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{START_MOVE, 		"normal bind"		,"dead root idle"	,"null" 	, ""},
	{USE_ITEM_WITH_ARG,	"normal bind"		,"dead idle diet"	,"null"		, ""},	

	{AUTO_BOT_BEGIN,	"normal bind"		,"dead idle diet"	,"null"		, ""},	
	{AUTO_BOT_CONTINUE,	"normal bind"		,"dead idle diet"	,"null"		, ""},	
	{GET_BATTLE_SCORE,	"all"			,"null"			,"null"		, ""},
	{GET_BATTLE_INFO,	"all"			,"null"			,"null"		, ""},

	{EQUIP_PET_BEDGE,       "normal bind"           ,"dead root idle SEAL"  ,"all"          , "UnlockHandler"},
	{MOVE_PET_BEDGE,	"normal bind"           ,"dead root idle SEAL"  ,"all"          , "UnlockHandler"},
	{EQUIP_PET_EQUIP,       "normal bind"           ,"dead root idle SEAL"  ,"all"          , "UnlockHandler"},
	{SUMMON_PET,		"normal bind"		,"dead"			,"null"		, ""},
	{RECALL_PET,		"normal bind"		,"dead"			,"null"		, ""},
	{COMBINE_PET,		"normal bind"		,"dead"			,"null"		, ""},
	{UNCOMBINE_PET,		"normal bind"		,"dead"			,"null"		, ""},
	{SET_PET_STATUS,	"normal bind"		,"dead"			,"null"		, ""},
	{SET_PET_RANK,		"normal bind"		,"dead"			,"null"		, ""},
	{PET_CTRL_CMD,		"normal bind"		,"dead"			,"null"		, ""},
	{START_FLY,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{STOP_FLY,		"normal bind"		,"dead root"		,"null"		, ""},
	{SET_FASHION_MASK,	"normal bind"		,"dead"			,"null"		, ""},
	{START_ONLINE_AGENT,	"normal"		,"dead"			,"null"		, ""},
	{JOIN_INSTANCE,		"normal"		,"dead"			,"null"		, ""},
	{BIND_RIDE_INVITE,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{BIND_RIDE_INVITE_REPLY,"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{BIND_RIDE_CANCEL,	"bind"			,"dead idle"		,"null"		, ""},
	{BIND_RIDE_KICK,	"bind"			,"dead idle"		,"null"		, ""},
	{POST_SNS_MESSAGE,	"all"			,"null"			,"null"		, ""},
	{APPLY_SNS_MESSAGE,	"all"			,"null"			,"null"		, ""},
	{VOTE_SNS_MESSAGE,	"all"			,"null"			,"null"		, ""},
	{RESPONSE_SNS_MESSAGE,	"all"			,"null"			,"null"		, ""},
	{EXCHANGE_POCKET_ITEM,"all"			,"trade"		,"all"		, "UnlockHandler"},
	{MOVE_POCKET_ITEM,	"all"			,"trade"		,"all"		, "UnlockHandler"},
	{EXCHANGE_INVENTORY_POCKET_ITEM,"all"			,"trade"		,"all"		, "UnlockHandler"},
	{MOVE_POCKET_ITEM_TO_INVENTORY,"all"			,"trade"		,"all"		, "UnlockHandler"},
	{MOVE_INVENTORY_ITEM_TO_POCKET,	"all"			,"trade"		,"all"		, "UnlockHandler"},
	{MOVE_ALL_POCKET_ITEM_TO_INVENTORY,	"all"			,"trade"		,"all"		, "UnlockHandler"},
	{MOVE_MAX_POCKET_ITEM_TO_INVENTORY,	"all"			,"trade"		,"all"		, "UnlockHandler"},
	{CAST_ITEM_SKILL,		"normal bind"		,"dead silent idle trade"	,"null" 	, ""},
	{UPDATE_FASHION_HOTKEY,		"all"		,"null"	,"null" 	, ""},
	{EXCHANGE_FASHION_ITEM,		"all"		,"trade"	,"all" 	, "UnlockHandler"},
	{EXCHANGE_INVENTORY_FASHION_ITEM,		"all"		,"trade"	,"all" 	, "UnlockHandler"},
	{EXCHANGE_EQUIPMENT_FASHION_ITEM,		"normal bind"		,"trade"	,"all" 	, "UnlockHandler"},
//	{EXCHANGE_HOTKEY_EQUIPMENT_FASHION,		"normal bind"		,"trade"	,"all" 	, "UnlockHandler"},
	{BONUSMALL_SHOPPING,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{QUERY_OTHERS_ACHIEVEMENT,	"all"           ,"null"  		,"null"         , ""},
	{UPDATE_PVP_MASK,	"all"			,"null"			,"null"		, ""},
	{START_TRANSFORM,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{STOP_TRANSFORM,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{CAST_TRANSFORM_SKILL,		"normal bind"		,"dead silent idle trade"	,"null" 	, ""},
	{ENTER_CARRIER,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{LEAVE_CARRIER,	"all"		,"null"	,"null"		, ""},
	{MOVE_ON_CARRIER,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{STOP_MOVE_ON_CARRIER,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{EXCHANGE_HOMETOWN_MONEY,	"normal"		,"trade"	,"null"		, ""},
	{GET_SERVER_TIMESTAMP,		"all"			,"null"			,"null"		, ""},
	{TERRITORY_LEAVE,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{CAST_CHARGE_SKILL,	"normal"		,"dead idle silent"	,"null"		, ""},
	{UPDATE_COMBINE_SKILL, 	"normal bind market"	,"dead"			,"null"		, ""},
	{UNIQUE_BID_REQUEST,	"normal bind"		,"dead silent idle trade"	,"null" 	, ""},
	{UNIQUE_BID_GET,	"normal bind"		,"dead silent idle trade"	,"null" 	, ""},
	{GET_CLONE_EQUIPMENT,	"all"			,"null"			,"null"		, ""},
	{TASK_FLY_POS,		"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{ZONEMALL_SHOPPING,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{ACTIVITY_FLY_POS,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{DEITY_LEVELUP,		"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{QUERY_CIRCLE_OF_DOOM_INFO,		"all"		,"null"	,"null"		, ""},
	{RAID_LEAVE,		"normal"		,"null"	,"null"		, ""},
	{CANCEL_PULLING,	"normal"		,"null"	,"null"		, ""},
	{GET_RAID_INFO,		"normal"		,"null"	,"null"		, ""},
	{QUERY_BE_SPIRIT_DRAGGED,		"all"		,"null"	,"null"		, ""},
	{QUERY_BE_PULLED,		"all"		,"null"	,"null"		, ""},
	{GET_MALL_SALETIME_ITEM,	 "normal bind dead", "trade", "all", "InvalidHandler"},
	{GET_RAID_COUNT,	 "normal bind dead", "trade", "all", "InvalidHandler"},
	{GET_VIP_AWARD_INFO,	 "normal bind dead", "trade", "all", "InvalidHandler"},
	{GET_VIP_AWARD_BY_ID,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{TRY_GET_ONLINE_AWARD,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{GET_ONLINE_AWARD,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{FIVE_ANNI_REQUEST,		"normal"		,"dead root idle SEAL"	,"null"		, ""},

	{UPGRADE_TREASURE_REGION,		"normal bind"		,"null"	,"null"		, ""},
	{UNLOCK_TREASURE_REGION,		"normal bind"		,"null"	,"null"		, ""},
	{DIG_TREASURE_REGION,		"normal bind"		,"null"	,"null"		, ""},
	{ADOPT_LITTLEPET,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{FEED_LITTLEPET,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{GET_LITTLEPET_AWARD,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{RUNE_IDENTIFY,		"normal bind"		,"null"	,"null"		, ""},
	{RUNE_COMBINE,		"normal bind"		,"null"	,"null"		, ""},
	{RUNE_REFINE,		"normal bind"		,"null"	,"null"		, ""},
	{RUNE_RESET,		"normal bind"		,"null"	,"null"		, ""},
	{RUNE_DECOMPOSE,	"normal bind"		,"null"	,"null"		, ""},
	{RUNE_LEVELUP,		"normal bind"		,"null"	,"null"		, ""},
//	{RUNE_OPEN_SLOT,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{RUNE_CHANGE_SLOT,	"normal bind"		,"null"	,"null"		, ""},
	{RUNE_ERASE_SLOT,	"normal bind"		,"null"	,"null"		, ""},
	{RUNE_INSTALL_SLOT,	"normal bind"		,"null"	,"null"		, ""},
	{RUNE_REFINE_ACTION,	"normal bind"		,"null"	,"null"		, ""},
	{START_RANDOM_TOWER_MONSTER,		"all"		,"null"	,"null"		, ""},
	{TOWER_REWARD,		"normal bind"		,"trade"	,"null"		, ""},
	{RESET_SKILL_PROP,	"all"		,"null"	,"null"		, ""},
	{GET_TASK_AWARD,	"normal bind"		,"trade"	,"null"		, ""},

	{PK_1ST_GUESS,					"normal bind"		,"trade"	,"null"		, ""},
	{PK_TOP3_GUESS,					"normal bind"		,"trade"	,"null"		, ""},
	{PK_1ST_GUESS_REWARD, 			"normal bind"		,"trade"	,"null"		, ""},
	{PK_1ST_GUESS_RESULT_REWARD,	"normal bind"		,"trade"	,"null"		, ""},
	{PK_TOP3_GUESS_REWARD,			"normal bind"		,"trade"	,"null"		, ""},
	{PK_TOP3_GUESS_RESULT_REWARD,	"normal bind"		,"trade"	,"null"		, ""},
	{GET_PLAYER_BET_DATA,	"all"		,"null"	,"null"		, ""},

//	{START_SPECIAL_MOVE, 		"normal bind"		,"dead root idle"	,"null" 	, ""},
//	{SPECIAL_MOVE, 		"normal bind"		,"dead root idle"	,"null" 	, ""},
//	{STOP_SPECIAL_MOVE, 		"normal bind"		,"dead root idle"	,"null" 	, ""},
	{COLLISION_RAID_APPLY,		"all"		,"null"	,"null"		, ""},

	{GET_REPURCHASE_INV_DATA, 				"normal bind"		,"trade dead root idle"	,"null" 	, ""},
	{EXCHANGE_MOUNT_WING_ITEM,				"normal bind"		,"trade"	,"all" 	, "UnlockHandler"},
	{EXCHANGE_INVENTORY_MOUNTWING_ITEM,		"normal bind"		,"trade"	,"all" 	, "UnlockHandler"},
	{EXCHANGE_EQUIPMENT_MOUNTWING_ITEM,		"normal bind"		,"trade"	,"all" 	, "UnlockHandler"},
	{HIDE_VIP_LEVEL,	"all"		,"null"	,"null"		, ""},
	{CHANGE_WING_COLOR,	"all"		,"null"	,"null"		, ""},
	{ASTROLOGY_IDENTIFY,"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{ASTROLOGY_UPGRADE,	"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{ASTROLOGY_DESTROY,	"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{JOIN_RAID_ROOM,	"all"		,"null"	,"null"		, ""},
	{GET_COLLISION_AWARD,				"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{TALISMAN_REFINESKILL_RESULT,		"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{CANCEL_ICE_CRUST,					"normal"		,"dead"	,"null"		, ""},
	{PLAYER_FIRST_EXIT_REASON,			"all"		,"null"	,"null"		, ""},
	{REMEDY_METEMPSYCHOSIS_LEVEL,		"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{MERGE_POTION,						"normal bind"		,"trade dead root idle"	,"null"		, ""},	
	{KINGDOM_LEAVE,						"normal bind"		,"root idle"	,"null"		, ""},	
//	{GET_COLLISION_PLAYER_POS,	"all"		,"null"	,"null"		, ""},
	{TAKE_LIVENESS_AWARD,	"normal bind dead"	,"trade"		,"all"		, "InvalidHandler"},
	{RAID_LEVEL_AWARD,		"normal bind"	,"trade"		,"null"		, ""},
	{GET_RAID_LEVEL_RESULT,	"all"	,""		,"null"		, ""},
	{OPEN_TRASHBOX,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{GET_CASH_GIFT_AWARD,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{GEN_PROP_ADD_ITEM,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{REBUILD_PROP_ADD_ITEM,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{GET_PROPADD,	"all"	,""		,"null"		, ""},
	{BUY_KINGDOM_ITEM,	"normal bind"		,"trade"	,"null"		, ""},
	{KING_TRY_CALL_GUARD,	"normal bind"		,"trade"	,"null"		, ""},
	{GET_TOUCH_AWARD,	"normal bind"		,"trade"	,"null"		, ""},
	{FLOW_BATTLE_LEAVE,	"normal bind"		,"trade"	,"null"		, ""},
	{BATH_INVITE,		"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{BATH_INVITE_REPLY,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{DELIVER_KINGDOM_TASK,	"normal bind"		,"trade"	,"null"		, ""},
	{RECEIVE_KINGDOM_TASK,	"normal bind"		,"trade"	,"null"		, ""},
	{KINGDOM_FLY_BATH_POS,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{FUWEN_COMPOSE,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{FUWEN_UPGRADE,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{FUWEN_INSTALL,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{FUWEN_UNINSTALL,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{RESIZE_INV,		"normal"		,"dead root idle"	,"all"		, "InvalidHandler"},
	{REGISTER_FLOW_BATTLE,	"all"		,"null"	,"null"		, ""},
	{ARRANGE_INVENTORY,   "normal bind"			,"trade"			,"null"		, ""},
	{ARRANGE_TRASHBOX,   "normal bind"			,"trade"			,"null"		, ""},
	{ARRANGE_POCKET,   "normal bind"			,"trade"			,"null"		, ""},
	{GET_WEB_ORDER,		"normal bind"		,"trade"	,"null"		, ""},
	{CUSTOMIZE_RUNE,	"normal bind"		"null",		"null", 		"null"},	
	{GET_CROSS_SERVER_BATTLE_INFO,	"all"			,"null"			,"null"		, ""},
	{CONTROL_TRAP,	"normal bind"		"null",		"null", 		"null"},	
	{SUMMON_TELEPORT,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{CREATE_CROSSVR_TEAM,		"normal bind"		,"dead root idle"	,"null"		, ""},
	{GET_KING_REWARD,	"normal bind"		,"dead silent idle trade"	,"null" 	, ""},
	{MOBACTIVE_START,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{MOBACTIVE_FINISH,	"normal bind"		,"dead root idle SEAL"	,"null"		, ""},
	{ENTER_FACTION_BASE,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	{ADD_FAC_BUILDING,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	{UPGRADE_FAC_BUILDING,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	{REMOVE_FAC_BUILDING,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	{GET_FAC_BASE_INFO,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	
	{QILIN_INVITE,		"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{QILIN_INVITE_REPLY,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{QILIN_CANCEL,		"bind"			,"dead idle"		,"null"		, ""},
	{QILIN_DISCONNECT,	"bind"		,"dead root idle SEAL"	,"null"		, ""},
	{QILIN_RECONNECT,	"bind"		,"dead root idle SEAL"	,"null"		, ""},
	// Youshuang add
	{FASHION_COLORANT_COMBINE,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{FASHION_ADD_COLOR,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{GET_ACHIEVEMENT_AWARD,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{GET_FACBASE_MALL_INFO,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{SHOP_FROM_FACBASE_MALL,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{CONTRIBUTE_FACBASE_CASH,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{GET_FACBASE_CASH_ITEMS_INFO,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{BUY_FACBASE_CASH_ITEM,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{BID_ON_FACBASE,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{POST_FAC_BASE_MSG,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{TASK_FLY_TO_AREA,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{TALISMAN_ENCHANT_CONFIRM,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{NEWYEAR_AWARD,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	{USE_TITLE,	"normal bind"		,"dead root idle"	,"null" 	, ""},
	// end
	{PET_CHANGE_SHAPE,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{PET_REFINE_ATTR,	"normal bind"		,"dead root idle"	,"null"		, ""},
	{ACTIVE_EMOTE_ACTION_INVITE,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{ACTIVE_EMOTE_ACTION_INVITE_REPLY,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{ACTIVE_EMOTE_ACTION_CANCEL,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{GET_RAID_TRANSFORM_TASK,	"normal"		,"dead root idle SEAL"	,"null"		, ""},
	{GET_CS_6V6_AWARD,	"normal bind"		,"dead root idle trade"	,"null" 	, ""},
	{EXCHANGE_CS_6V6_AWARD,	"normal bind"		,"dead root idle trade"	,"null" 	, ""},
	{PUPPET_FORM_CHANGE,	"all"		,"null"	,"null"		, ""},
	{FAC_BASE_TRANSFER,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	{EXCHANGE_CS_6V6_MONEY,	"normal bind"		,"dead root idle trade"	,"null" 	, ""},
	{WITHDRAW_FACBASE_AUCTION,	"normal bind"		,"dead root idle trade"	,"null" 	, ""},
	{WITHDRAW_FACBASE_COUPON,	"normal bind"		,"dead root idle trade"	,"null" 	, ""},
	{LEAVE_FACTION_BASE,	"normal bind"		,"dead root idle SEAL"	,"null" 	, ""},
	{OBJECT_CHARGE_TO,	"all"		,"null"	,"null"		, ""},
	{STOP_TRAVEL_AROUND,	"all"		,"null"	,"null"		, ""},
	{GET_SUMMON_PETPROP,	"all"		,"null"	,"null"		, ""},
	{GET_SEEK_AWARD,				"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{SEEKER_BUY_SKILL,				"normal bind"		,"trade dead root idle SEAL"	,"null"		, ""},
	{HIDE_AND_SEEK_RAID_APPLY,		"all"		,"null"	,"null"		, ""},
	{CAPTURE_RAID_SUBMIT_FLAG,		"all"		,"null"	,"null"		, ""},
	{CAPTURE_RAID_APPLY,		"all"		,"null"	,"null"		, ""},

	{-1,			NULL			,NULL			,NULL		, NULL},
};

//-----------------------------------------------------------------------------------------------------

int GetCommandState(const char * str)
{
	int state = 0;
	abase::strtok tok(str," \t");
	const char * token;
	while((token = tok.token()))
	{
		if(!*token) continue;
		for(size_t i = 0; mask_list[i].name; i ++)
		{
			if(strcmp(token, mask_list[i].name) == 0)
			{
				state |= mask_list[i].mask;
				break;
			}
		}
	}
	return state;
}
}

bool 
player_cmd_dispatcher::InitCommandList()
{
	ASSERT(_standard_cmd.size() == 0);
	size_t index = 0;
	for(;;index ++)
	{
		if(cmd_list[index].command < 0 || cmd_list[index].valid_state == NULL) break;
		size_t cmd = cmd_list[index].command;
		if(cmd >= _standard_cmd.size()) 
		{
			cmd_define def = {0,0,0};
			_standard_cmd.insert(_standard_cmd.end(), cmd + 1 - _standard_cmd.size(), def);
		}
		cmd_define ent = {0,0,0};
		ent.state_mask  = GetCommandState(cmd_list[index].valid_state);
		ent.exclude_state_mask = GetCommandState(cmd_list[index].invalid_state) | ALWAYS_INVALID_STATE;
		ent.spec_state = GetCommandState(cmd_list[index].spec_state);
		if(ent.spec_state)
		{
			if(strcmp(cmd_list[index].spec_handler, "UnlockHandler") == 0)
			{
				ent.spec_handler = new exec_cmd1();
			}
			else
			if(strcmp(cmd_list[index].spec_handler, "InvalidHandler") == 0)
			{
				ent.spec_handler = new exec_cmd2();
			}
			else if(strcmp(cmd_list[index].spec_handler, "DebugHandler") == 0)
			{
				ent.spec_handler = new exec_cmd3();
			}
			else
			{
				ASSERT(false);
			}
		}
		_standard_cmd[cmd] = ent;
	}
	return true;
}

