#ifndef __ONLINEGAME_COMMON_PROTOCOL_H__
#define __ONLINEGAME_COMMON_PROTOCOL_H__

#include "types.h"

#pragma pack(1)
namespace S2C
{
	namespace INFO
	{
		struct player_info_1
		{
			int cid;
			A3DVECTOR pos;
			unsigned short equip_crc;
			unsigned char  dir;		//256的定点数表示度数
			unsigned char  cls;		//职业
			unsigned char  level;		//修真级别
			unsigned char level2;
			char  pk_level;			//pk级别 -1 0 1 2 3 4 5 6
			char  cultivation;		//仙魔佛造化
			int   reputation;		//威望
			unsigned int   object_state;	//当前状态，第0位表示是否死亡
			unsigned int   extra_state;	//玩家的另外状态
		};

		struct player_info_2			//name ,customize data 不常改变
		{
			unsigned char size;
			char data[];
		};

		struct player_info_3			//viewdata of equipments and special effect
		{
			unsigned char size;
			char  data[];
		};

		struct player_info_4			//detail
		{
			unsigned short size;
			char	data[];
		};
		struct player_info_00			//自己会经常改变的内容
		{
			short lvl;
			short dt_lvl;
			unsigned char combat_state;
			unsigned char sec_level;
			int hp;
			int max_hp;
			int mp;
			int max_mp;
			int dp;
			int max_dp;
		};
		struct self_info_00
		{
			short lvl;
			short dt_lvl;
			unsigned char combat_state;
			unsigned char sec_level;
			int hp;
			int max_hp;
			int mp;
			int max_mp;
			int dp;
			int max_dp;
			int talisman_stamina;
			double exp;
			double dt_exp;
		};

		struct npc_info
		{
			int nid;			//npc id
			int tid;			//template id
			A3DVECTOR pos;
			unsigned short seed;		// seed of customize data
			unsigned char dir;
			unsigned int  object_state;
		};

		struct npc_info_00
		{
			int hp;
			int max_hp;
		};

		struct matter_info_1
		{
			int mid;
			int tid;
			int name_id;			 //name owner id
			A3DVECTOR pos;
			unsigned char dir0;
			unsigned char dir1;
			unsigned char rad;
			unsigned char state;
			unsigned char custom;
		};

		struct matter_info_2
		{
			int mid;
			unsigned char size;
			unsigned char data[];
		};

		struct self_info_1
		{
			double exp;
			int cid;
			A3DVECTOR pos;
			unsigned short crc;
			unsigned char  dir;		//256的定点数表示度数
			unsigned char  cls;		//256的定点数表示度数
			unsigned char  level;		//修真级别
			unsigned char  level2;		//修真级别
			char  pk_level;			//pk level
			char cultivation;		//仙魔佛造化
			int   reputation;		//威望
			unsigned int state;
		};

		struct move_info
		{
			int cid;
			A3DVECTOR dest;
			unsigned short use_time;
			unsigned short speed;
			unsigned char  move_mode;
		};
		
		union  market_goods
		{
			struct 
			{
				int type;		//物品类型  如果是0 表示没有内容了
			}empty_item;
			struct 
			{
				int type;		//物品类型
				int count;		//剩余多少个 负数表示是买
				size_t price;		//单价
				int expire_date;	//过期时间
				int state;		//特殊状态
				unsigned short content_length;
				char content[];
			}item;

			struct 
			{
				int type;		//物品类型
				int count;		//剩余多少个 负数表示是买
				size_t price;		//单价
			}order_item;
		};


	}

	enum		//CMD
	{
		PLAYER_INFO_1,
		PLAYER_INFO_2,
		PLAYER_INFO_3,
		PLAYER_INFO_4,
		PLAYER_INFO_1_LIST,

		PLAYER_INFO_2_LIST,			//5
		PLAYER_INFO_3_LIST,
		PLAYER_INFO_23_LIST,
		SELF_INFO_1,
		NPC_INFO_LIST,

		MATTER_INFO_LIST,			//10
		NPC_ENTER_SLICE,
		PLAYER_ENTER_SLICE,
		OBJECT_LEAVE_SLICE,
		OBJECT_NOTIFY_POS,		//这个命令时暂时的？
		
		OBJECT_MOVE,				//15
		NPC_ENTER_WORLD,
		PLAYER_ENTER_WORLD,
		MATTER_ENTER_WORLD,
		PLAYER_LEAVE_WORLD,
		
		NPC_DEAD,				//20
		OBJECT_DISAPPEAR,
		OBJECT_START_ATTACK,			
		SELF_STOP_ATTACK,			//
		SELF_ATTACK_RESULT,			//作废
		
		ERROR_MESSAGE,				//25	
		BE_ATTACKED,
		PLAYER_DEAD,
		BE_KILLED,
		PLAYER_REVIVAL,
		
		PICKUP_MONEY,				//30
		PICKUP_ITEM,
		PLAYER_INFO_00,
		NPC_INFO_00,
		OUT_OF_SIGHT_LIST,
		
		OBJECT_STOP_MOVE,			//35
		RECEIVE_EXP,
		LEVEL_UP,
		SELF_INFO_00,
		UNSELECT,

		SELF_ITEM_INFO,				//40
		SELF_ITEM_EMPTY_INFO,
		SELF_INVENTORY_DATA,
		SELF_INVENTORY_DETAIL_DATA,
		EXCHANGE_INVENTORY_ITEM,

		MOVE_INVENTORY_ITEM,			//45
		PLAYER_DROP_ITEM,
		EXCHANGE_EQUIPMENT_ITEM,
		EQUIP_ITEM,
		MOVE_EQUIPMENT_ITEM,	

		SELF_GET_EXT_PROPERTY,			//50
		SET_STATUS_POINT_NULL,			//已经作废
		SELECT_TARGET,
		PLAYER_EXTPROP_BASE,
		PLAYER_EXTPROP_MOVE,

		PLAYER_EXTPROP_ATTACK,			//55
		PLAYER_EXTPROP_DEFENSE,
		TEAM_LEADER_INVITE,
		TEAM_REJECT_INVITE,
		TEAM_JOIN_TEAM,

		TEAM_MEMBER_LEAVE,			//60
		TEAM_LEAVE_PARTY,
		TEAM_NEW_MEMBER,
		TEAM_LEADER_CANCEL_PARTY,
		TEAM_MEMBER_DATA,
							
		TEAMMATE_POS,				//65
		EQUIPMENT_DATA,			//用户的装备数据，影响表现
		EQUIPMENT_INFO_CHANGED,	
		EQUIPMENT_DAMAGED,		//装备损坏
		TEAM_MEMBER_PICKUP,		//队友捡起装备

		NPC_GREETING,				//70
		NPC_SERVICE_CONTENT,
		PURCHASE_ITEM,
		ITEM_TO_MONEY,	
		REPAIR_ALL,

		REPAIR,					//75
		RENEW,
		SPEND_MONEY,
		PICKUP_MONEY_IN_TRADE,
		PICKUP_ITEM_IN_TRADE,

		PICKUP_MONEY_AFTER_TRADE,		//80
		PICKUP_ITEM_AFTER_TRADE,
		GET_OWN_MONEY,
		OBJECT_ATTACK_ONCE,			//已经作废 ，完全没有用
		SELF_START_ATTACK,			//已经作废
	
		OBJECT_CAST_SKILL,			//85
		SKILL_INTERRUPTED,
		SELF_SKILL_INTERRUPTED,
		SKILL_PERFORM,
		OBJECT_BE_ATTACKED,			//已经作废 ，完全没有用

		SKILL_DATA,				//90
		PLAYER_USE_ITEM,
		EMBED_ITEM,
		CLEAR_EMBEDDED_CHIP,
		COST_SKILL_POINT,

		LEARN_SKILL,				//95
		OBJECT_TAKEOFF,
		OBJECT_LANDING,
		FLYSWORD_TIME_CAPACITY,
		OBTAIN_ITEM,

		PRODUCE_START,				//100
		PRODUCE_ONCE,
		PRODUCE_END,
		DECOMPOSE_START,
		DECOMPOSE_END,

		TASK_DATA,				//105
		TASK_VAR_DATA,
		OBJECT_START_USE,
		OBJECT_CANCEL_USE,
		OBJECT_USE_ITEM,

		OBJECT_START_USE_WITH_TARGET,		//110
		OBJECT_SIT_DOWN,
		OBJECT_STAND_UP,
		OBJECT_DO_EMOTE,
		SERVER_TIMESTAMP,

		NOTIFY_ROOT,				//115
		DISPEL_ROOT,
		PK_LEVEL_NOTIFY,
		PK_VALUE_NOTIFY,
		PLAYER_CHANGE_CLASS,

		OBJECT_ATTACK_RESULT,			//120
		BE_HURT,
		HURT_RESULT,
		SELF_STOP_SKILL,
		UPDATE_VISIBLE_STATE,

		OBJECT_BUFF_NOTIFY,			//125
		PLAYER_GATHER_START,
		PLAYER_GATHER_STOP,
		TRASHBOX_PASSWD_CHANGED,
		TRASHBOX_PASSWD_STATE,

		TRASHBOX_OPEN,				//130
		TRASHBOX_CLOSE,	
		TRASHBOX_WEALTH,
		EXCHANGE_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,
		
		EXCHANGE_TRASHBOX_INVENTORY,		//135
		INVENTORY_ITEM_TO_TRASH,
		TRASH_ITEM_TO_INVENTORY,
		EXCHANGE_TRASH_MONEY,
		ENCHANT_RESULT,

		SELF_NOTIFY_ROOT,			//140
		OBJECT_DO_ACTION,
		RECORDER_START,
		OBJECT_SKILL_ATTACK_RESULT,
		BE_SKILL_ATTACKED,

		PLAYER_SET_ADV_DATA,			//145
		PLAYER_CLR_ADV_DATA,
		PLAYER_IN_TEAM,		
		TEAM_APPLY_REQUEST,
		OBJECT_DO_EMOTE_RESTORE,

		CONCURRENT_EMOTE_REQUEST,		//150 //$$$$$$$$$$
		DO_CONCURRENT_EMOTE,			      //$$$$$$$$$$
		MATTER_PICKUP,
		MAFIA_INFO_NOTIFY,
		MAFIA_TRADE_START,
		
		MAFIA_TRADE_END,			//155
		TASK_DELIVER_ITEM,
		TASK_DELIVER_REPUTATION,
		TASK_DELIVER_EXP,
		TASK_DELIVER_MONEY,

		TASK_DELIVER_LEVEL2,			//160
		PLAYER_REPUTATION,
		IDENTIFY_RESULT,
		PLAYER_CHANGE_SHAPE,
		PLAYER_ENTER_SANCTUARY,

		PLAYER_LEAVE_SANCTUARY,			//165
		PLAYER_OPEN_MARKET,
		SELF_OPEN_MARKET,
		PLAYER_CANCEL_MARKET,
		PLAYER_MARKET_INFO,

		PLAYER_MARKET_TRADE_SUCCESS,		//170
		PLAYER_MARKET_NAME,
		PLAYER_START_TRAVEL,
		SELF_START_TRAVEL,
		PLAYER_COMPLETE_TRAVEL,

		GM_TOGGLE_INVINCIBLE,			//175
		GM_TOGGLE_INVISIBLE,
		SELF_TRACE_CUR_POS,
		OBJECT_CAST_INSTANT_SKILL,	
		ACTIVATE_WAYPOINT,

		PLAYER_WAYPOINT_LIST,			//180
		UNLOCK_INVENTORY_SLOT,
		TEAM_INVITE_PLAYER_TIMEOUT,
		PLAYER_PVP_NO_PROTECT,
		PLAYER_DISABLE_PVP,

		PLAYER_PVP_COOLDOWN,			//185
		COOLDOWN_DATA,
		SKILL_ABILITY_NOTFIY,
		PERSONAL_MARKET_AVAILABLE,
		BREATH_DATA,

		PLAYER_STOP_DIVE,			//190
		TRADE_AWAY_ITEM,
		PLAYER_ENABLE_FASHION_MODE,
		ENABLE_FREE_PVP_MODE,
		OBJECT_IS_INVALID,

		PLAYER_ENABLE_EFFECT,			//195
		PLAYER_DISABLE_EFFECT,
		ENABLE_RESURRECT_STATE,		
		SET_COOLDOWN,
		CHANGE_TEAM_LEADER,		

		KICKOUT_INSTANCE,			//200
		PLAYER_COSMETIC_BEGIN,
		PLAYER_COSMETIC_END,
		COSMETIC_SUCCESS,
		OBJECT_CAST_POS_SKILL,

		CHANGE_MOVE_SEQ,			//205
		SERVER_CONFIG_DATA,
		PLAYER_RUSH_MODE,
		TRASHBOX_CAPACITY_NOTIFY,
		NPC_DEAD_2,

		PRODUCE_NULL,				//210
		ACTIVE_PVP_COMBAT_STATE,
		DOUBLE_EXP_TIME,
		AVAILABLE_DOUBLE_EXP_TIME,
		DUEL_RECV_REQUEST,

		DUEL_REJECT_REQUEST,			//215
		DUEL_PREPARE,
		DUEL_CANCEL,
		DUEL_START,
		DUEL_STOP,
		
		DUEL_RESULT,				//220
		PLAYER_BIND_REQUEST,
		PLAYER_BIND_INVITE,
		PLAYER_BIND_REQUEST_REPLY,
		PLAYER_BIND_INVITE_REPLY,

		PLAYER_BIND_START,			//225
		PLAYER_BIND_STOP,
		PLAYER_MOUNTING,
		PLAYER_EQUIP_DETAIL,
		ELSE_DUEL_START,

		PARIAH_DURATION,			//230
		PLAYER_GAIN_PET,
		PLAYER_FREE_PET,
		PLAYER_SUMMON_PET,
		PLAYER_RECALL_PET,

		PLAYER_START_PET_OP,			//235
		PLAYER_STOP_PET_OP,
		PLAYER_PET_RECV_EXP,
		PLAYER_PET_LEVELUP,
		PLAYER_PET_ROOM,

		PLAYER_PET_ROOM_CAPACITY,		//240
		PLAYER_PET_HONOR_POINT,
		PLAYER_PET_HUNGER_POINT,
		ENTER_BATTLEGROUND,
		TURRET_LEADER_NOTIFY,

		BATTLE_RESULT,				//245
		BATTLE_INFO,
		PLAYER_PET_DEAD,
		PET_REVIVE,
		PLAYER_PET_HP_NOTIFY,

		PLAYER_PET_AI_STATE,			//250
		PLAYER_RECIPE_DATA,
		PLAYER_LEARN_RECIPE,
		PLAYER_PRODUCE_SKILL_INFO,
		PLAYER_REFINE_RESULT,

		PLAYER_START_GENERAL_OPERATION,		//255
		PLAYER_STOP_GENERAL_OPERATION,
		PLAYER_CASH,
		PLAYER_SKILL_ADDON,
		PLAYER_SKILL_COMMON_ADDON,

		PLAYER_EXTRA_SKILL,			//260
		PLAYER_TITLE_LIST,
		PLAYER_ADD_TITLE,
		PLAYER_DEL_TITLE,
		PLAYER_REGION_REPUTATION,

		PLAYER_CHANGE_REGION_REPUTATION,	//265
		PLAYER_CHANGE_TITLE,		
		PLAYER_CHANGE_INVENTORY_SIZE,
		PLAYER_BIND_SUCCESS,
		PLAYER_CHANGE_SPOUSE,
		
		PLAYER_INVADER_STATE,			//270
		PLAYER_MAFIA_CONTRIBUTION,
		LOTTERY_BONUS,
		RECORDER_CHECK_RESULT,
		PLAYER_USE_ITEM_WITH_ARG,

		OBJECT_USE_ITEM_WITH_ARG,		//275
		MAFIA_TRASHBOX_CAPACITY,
		NPC_SERVICE_RESULT,
		BATTLE_FLAG_CHANGE,
		BATTLE_START,

		BATTLE_FLAG_CHANGE_STATUS,		//280
		TALISMAN_EXP_NOTIFY,
		TALISMAN_COMBINE,
		TALISMAN_VALUE_NOTIFY,
		AUTO_BOT_BEGIN,

		PLAYER_WALLOW_INFO,			//285
		ANTI_CHEAT_BONUS,
		BATTLE_SCORE,
		GAIN_BATTLE_SCORE,
		SPEND_BATTLE_SCORE,

		BATTLE_PLAYER_INFO,			//290
		TALENT_NOTIFY,
		LEAVE_BATTLEGROUND,
		NOTIFY_SAFE_LOCK,
		ENTER_BATTLEFIELD,

		LEAVE_BATTLEFIELD,			//295
		BATTLEFIELD_INFO,
		BATTLEFIELD_START,
		BATTLEFIELD_END,
		BATTLEFIELD_CONSTRUCTION_INFO,
		
		BATTLEFIELD_CONTRIBUTION_INFO,		//300
		PLAYER_EQUIP_PET_BEDGE,
		PLAYER_EQUIP_PET_EQUIP,
		PLAYER_SET_PET_STATUS,
		PLAYER_COMBINE_PET,

		PLAYER_UNCOMBINE_PET,			//305
		PLAYER_PET_AGE_LIFE,
		PLAYER_PET_SET_AUTO_SKILL,
		PLAYER_PET_SET_SKILL_COOLDOWN,
		BATTLEFIELD_BUILDING_STATUS_CHANGE,

		PLAYER_SET_PET_RANK,			//310
		PLAYER_REBORN_INFO,
		SCRIPT_MESSAGE,
		PLAYER_PET_CIVILIZATION,
		PLAYER_PET_CONSTRUCTION,

		PLAYER_MOVE_PET_BEDGE,			//315
		PLAYER_START_FLY,
		PLAYER_PET_PROP_ADDED,
		SECT_BECOME_DISCIPLE,
		CULTIVATION_NOTIFY,

		OFFLINE_AGENT_BONUS,			//320
		OFFLINE_AGENT_TIME,
		SPIRIT_POWER_DECREASE,
		PLAYER_BIND_RIDE_INVITE,
		PLAYER_BIND_RIDE_INVITE_REPLY,

		PLAYER_BIND_RIDE_JOIN,			//325
		PLAYER_BIND_RIDE_LEAVE,
		PLAYER_BIND_RIDE_KICK,
		EXCHANGE_POCKET_ITEM,
		MOVE_POCKET_ITEM,

		EXCHANGE_INVENTORY_POCKET_ITEM,		//330
		MOVE_POCKET_ITEM_TO_INVENTORY,
		MOVE_INVENTORY_ITEM_TO_POCKET,
		PLAYER_CHANGE_POCKET_SIZE,
		MOVE_ALL_POCKET_ITEM_TO_INVENTORY,

		SELF_BUFF_NOTIFY,			//335	
		NOTIFY_FASHION_HOTKEY,
		EXCHANGE_FASHION_ITEM,
		EXCHANGE_INVENTORY_FASHION_ITEM,
		EXCHANGE_EQUIPMENT_FASHION_ITEM,

		EXCHANGE_HOTKEY_EQUIPMENT_FASHION,	//340
		PEEP_INFO,
		PLAYER_BONUS,
		SELF_KILLINGFIELD_INFO,
		KILLINGFIELD_INFO,

		ARENA_INFO,				//345
		RANK_CHANGE,
		CHANGE_KILLINGFIELD_SCORE,
		PLAYER_CHANGE_STYLE,	
		PLAYER_CHANGE_VIPSTATE,

		RECEIVE_BONUS_EXP,			//350
		CROSSSERVER_BATTLE_INFO,
		PLAYER_ACHIEVEMENT,
		PLAYER_ACHIEVEMENT_FINISH,
		PLAYER_ACHIEVEMENT_ACTIVE,

		PLAYER_PREMISS_DATA,			//355
		PLAYER_PREMISS_FINISH,		
		PLAYER_ACHIEVEMENT_MAP,			
		ACHIEVEMENT_MESSAGE,			
		SELF_INSTANCE_INFO,	

		INSTANCE_INFO,				//360
		SKILL_CONTINUE,
		PLAYER_START_TRANSFORM,	
		MAGIC_DURATION_DECREASE,
		MAGIC_EXP_NOTIFY,

		TRANSFORM_SKILL_DATA,			//365
		PLAYER_ENTER_CARRIER,
		PLAYER_LEAVE_CARRIER,
		PLAYER_MOVE_ON_CARRIER,
		PLAYER_STOP_MOVE_ON_CARRIER,

		SKILL_PROFICIENCY,			//370	
		NOTIFY_MALL_PROP,
		SEND_PK_MESSAGE,
		UPDATE_COMBO_STATE,		
		ENTER_DIM_STATE,

		ENTER_INVISIBLE_STATE,			//375
		OBJECT_CHARGE,	
		ENTER_TERRITORY,
		LEAVE_TERRITORY,
		LAST_LOGOUT_TIME,	

		COMBINE_SKILL_DATA,			//380
		SEND_CLONE_EQUIPMENT_INFO,
		CIRCLE_INFO_NOTIFY,
		DELIVERY_EXP_NOTIFY,
		DEITY_LEVELUP,

		OBJECT_SPECIAL_STATE,			//385	
		LOTTERY2_BONUS,
		GEM_NOTIFY,
		CRSSVR_TEAM_BATTLE_INFO,
		NOTIFY_CIRCLE_OF_DOOM_START,

		NOTIFY_CIRCLE_OF_DOOM_INFO,     //390
		NOTIFY_CIRCLE_OF_DOOM_STOP,
		PLAYER_SCALE,		
		PLAYER_MOVE_CYCLE_AREA,
		ENTER_RAID,
		
		LEAVE_RAID,						//395
		OBJECT_NOTIFY_RAID_POS,
		PLAYER_RAID_COUNTER,
		OBJECT_BE_MOVED,
		PLAYER_PULLING,

		PLAYER_BE_PULLED,				//400
		LOTTERY3_BONUS,
		RAID_INFO,
		TRIGGER_SKILL_TIME,
	};

	enum 
	{
		//0
		ERR_SUCCESS,
		ERR_INVALID_TARGET,
		ERR_OUT_OF_RANGE,
		ERR_FATAL_ERR,
		ERR_COMMAND_IN_ZOMBIE,
		//5
		ERR_ITEM_NOT_IN_INVENTORY,
		ERR_ITEM_CANT_PICKUP,
		ERR_INVENTORY_IS_FULL,
		ERR_ITEM_CANNOT_EQUIP,
		ERR_CANNOT_ATTACK,
		//10
		ERR_TEAM_CANNOT_INVITE,
		ERR_TEAM_JOIN_FAILED,
		ERR_TEAM_ALREADY_INVITE,
		ERR_TEAM_INVITE_TIMEOUT,
		ERR_SERVICE_UNAVILABLE,
		//15
		ERR_SERVICE_ERR_REQUEST,
		ERR_OUT_OF_FUND,
		ERR_CANNOT_LOGOUT,
		ERR_CANNOT_USE_ITEM,
		ERR_TASK_NOT_AVAILABLE,
		//20
		ERR_SKILL_NOT_AVAILABLE,
		ERR_CANNOT_EMBED,
		ERR_CANNOT_LEARN_SKILL,
		ERR_CANNOT_HEAL_IN_COMBAT,
		ERR_CANNOT_RECHARGE,
		//25
		ERR_NOT_ENOUGH_MATERIAL,
		ERR_PRODUCE_FAILED,
		ERR_DECOMPOSE_FAILED,
		ERR_CANNOT_SIT_DOWN,
		ERR_CANNOT_EQUIP_NOW,
		//30
		ERR_MINE_HAS_BEEN_LOCKED,
		ERR_MINE_HAS_INVALID_TOOL,
		ERR_MINE_GATHER_FAILED,
		ERR_OTHER_SESSION_IN_EXECUTE,
		ERR_INVALID_PASSWD_FORMAT,
		//35
		ERR_PASSWD_NOT_MATCH,
		ERR_TRASH_BOX_NOT_OPEN,
		ERR_ENOUGH_MONEY_IN_TRASH_BOX,
		ERR_TEAM_REFUSE_APPLY,
		ERR_CONCURRENT_EMOTE_REFUSED,		//$$$$$$$$$$$$$$$$$$
		//40
		ERR_EQUIPMENT_IS_LOCKED,
		ERR_CANNOT_OPEN_PLAYER_MARKET,
		ERR_INVALID_ITEM,
		ERR_YOU_HAS_BEEN_BANISHED,
		ERR_CAN_NOT_DROP_ITEM,
		//45
		ERR_INVALID_PRIVILEGE,
		ERR_PLAYER_NOT_EXIST,
		ERR_CAN_NOT_DISABLE_PVP_STATE,
		ERR_CAN_NOT_UNLEARN_SKILL,
		ERR_COMMAND_IN_IDLE,
		//50
		ERR_COMMAND_IN_SEALED,
		ERR_LEVEL_NOT_MATCH,
		ERR_CANNOT_ENTER_INSTANCE,
		ERR_SKILL_IS_COOLING,
		ERR_OBJECT_IS_COOLING,
		//55
		ERR_CANNOT_FLY,
		ERR_CAN_NOT_RESET_INSTANCE,
		ERR_INVENTORY_IS_LOCKED,
		ERR_TOO_MANY_PLAYER_IN_INSTANCE,
		ERR_TOO_MANY_INSTANCE,
		//60
		ERR_FACTION_BASE_NOT_READY,
		ERR_FACTION_BASE_DENIED,
		ERR_CAN_NOT_JUMP_BETWEEN_INSTANCE,
		ERR_NOT_ENOUGH_REST_TIME,
		ERR_CANNOT_DUEL_TWICE,
		//65
		ERR_CREATE_DUEL_FAILED,
		ERR_INVALID_OPERATION_IN_COMBAT,
		ERR_INVALID_GENDER,
		ERR_INVALID_BIND_REQUEST,
		ERR_INVALID_BIND_REPLY,
		//70
		ERR_FORBIDDED_OPERATION,
		ERR_PET_IS_ALEARY_ACTIVE,
		ERR_PET_IS_NOT_EXIST,
		ERR_PET_IS_NOT_ACTIVE,
		ERR_PET_FOOD_TYPE_NOT_MATCH,
		//75
		ERR_BATTLEFIELD_IS_CLOSED,
		ERR_PET_CAN_NOT_BE_HATCHED,
		ERR_PET_CAN_NOT_BE_RESTORED,
		ERR_FACTION_IS_NOT_MATCH,
		ERR_CANNOT_QUERY_ENEMY_EQUIP,
		//80
		ERR_NPC_SERVICE_IS_BUSY,
		ERR_PET_CAN_NOT_MOUNT,
		ERR_CAN_NOT_RESET_PP,
		ERR_BATTLEFIELD_IS_FINISHED,
		ERR_HERE_CAN_NOT_DUEL,
		//85
		ERR_SUMMON_PET_INVALID_POS,
		ERR_CONTROL_TOO_MANY_TURRETS,
		ERR_CANNOT_SUMMON_DEAD_PET,
		ERR_PET_IS_NOT_DEAD,
		ERR_CANNOT_BIND_HERE,
		//90
		ERR_INVALID_PLAYER_CALSS,
		ERR_RUNE_IS_IN_EFFECT,
		ERR_REINFORCE_FAILED,
		ERR_LEARN_RECIPE_FAILED,
		ERR_CAN_NOT_REFINE,
		//95
		ERR_CAN_NOT_UNINSTALL,
		ERR_GSHOP_INVALID_REQUEST,
		ERR_CAN_NOT_FIND_SPOUSE,
		ERR_NO_SKILL_TO_FORGET,
		ERR_RESTORE_DESTROY,
		//100
		ERR_USE_ITEM_FAILED,
		ERR_SKILL_IS_NOT_COOLING,
		ERR_CAN_TRANSMIT_IN,
		ERR_CAN_TRANSMIT_OUT,
		ERR_NOT_MAFIA_MEMBER,
		//105
		ERR_MAFIA_TIME_NOT_ENOUGH,
		ERR_ITEM_NEED_BATTLEGROUND,
		ERR_TALISMAN_REFINE_ERROR,
		ERR_TALISMAN_LEVEL_UP,
		ERR_TALISMAN_RESET,
		//110
		ERR_TALISMAN_COMBINE,
		ERR_TALISMAN_ENCHANT,
		ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK,
		ERR_DO_NOT_HAVE_SIEGE_TOCKEN, //113
		ERR_ARCHER_MAX,
		//115
		ERR_NOT_MAFIA_LEADER_CAN_NOT_CONSTRUCT,
		ERR_NOT_ENOUGH_RES_TO_CONSTRUCT,
		ERR_CONSTRUCT_MAX_LEVEL,
		ERR_NOT_ENOUGH_RES_TO_CONTRIBUTE,
		ERR_PET_BEDGE_IS_LOCKED,
		//120
		ERR_PET_EQUIP_IS_LOCKED,
		ERR_NEED_PET_BEDGE,
		ERR_PET_ALREADY_ADOPTED,
		ERR_PET_LEVEL_TO_HIGH,
		ERR_PET_NOT_ADOPTED,
		//125
		ERR_NOT_ENOUGH_ROOM_IN_PACKBACK,
		ERR_PET_ACTIVE_TOO_MUCH,
		ERR_NEED_EQUIP_PET_BEDGE,
		ERR_PET_NOT_IN_BATTLE_STATUS,
		ERR_PET_SKILL_IN_COOLDOWN,
		//130
		ERR_NOT_HAVE_BUY_ARCHER_ITEM,
		ERR_CAN_NOT_SET_MORE_PET_FOR_COMBET_STATUS,
		ERR_PET_NOT_SUMMON,
		ERR_PET_NOT_COMBINE,
		ERR_NOT_HAVE_ENOUGH_PET_CONSTRUCTION,
		//135
		ERR_NOT_HAVE_ENOUGH_PET_ABILITY,
		ERR_NOT_ENOUGH_CIVILIZATION,
		ERR_NOT_ENOUGH_PET_LEVEL,
		ERR_CUR_PET_RANK_MAX,
		ERR_FLY_IN_SHAPING,
		//140
		ERR_FLY_IN_COMBAT,
		ERR_PET_LEVEL_TOO_HIGH_CAN_NOT_USE_ITEM,
		ERR_PET_COMBINE1_COOLDOWN,
		ERR_PET_COMBINE2_COOLDOWN,
		ERR_AGE_LIFE_INVALID,
		//145
		ERR_PRODUCE_IN_COOLDOWN,
		ERR_REQUIRE_REINFORCE_LEVEL,
		ERR_ONLINE_AGENT_POS,
		ERR_PET_IS_LOCKED,
		ERR_BLOOD_ENCHANT_FAILED,
		//150
		ERR_SPIRIT_ADDON_FAILED,
		ERR_NOT_ENOUGH_SPIRIT_POWER,
		ERR_RESTORE_SPIRIT_POWER,
		ERR_SPIRIT_DECOMPOSE_FAILED,
		ERR_SPIRIT_MASTER_NOT_MATCH,
		//155
		ERR_SPIRIT_ADDON_REMOVE_FAILED,
		ERR_INVALID_LINK_RIDE_REQUEST,
		ERR_CANNOT_LINK_RIDE_HERE,
		ERR_LINK_RIDE_INVITE_FULL,
		ERR_LINK_RIDE_ALREADY_INVITE,
		//160
		ERR_CANNOT_MOVE_TO_POCKET,
		ERR_CANNOT_MOVE_ALL_TO_INVENTORY,	
		ERR_CANNOT_LOCK_EXPIRE_ITEM,
		ERR_REBORN_COUNT_NOT_MATCH,
		ERR_CAN_NOT_REFINE_PETEQUIP,
		//165
		ERR_CULT_NOT_MATCH,
		ERR_MAGIC_REFINE_ERROR,
		ERR_MAGIC_RESTORE_ERROR,
		ERR_MINE_NOT_OWNER,
		ERR_NOT_TERRITORY_OWNER,
		//170
		ERR_COMBINE_SKILL_IS_COOLING,
		ERR_CAN_NOT_BID,
		ERR_CAN_NOT_UPGRADE_EQUIPMENT,
		ERR_NEWBIE_CHAT_IS_COOLING,
		ERR_FAILED_IDENTIFY_GEMSLOT,
		//175
		ERR_FAILED_REBUILD_GEMSLOTS,
		ERR_FAILED_CUSTOMIZE_GEMSLOTS,
		ERR_FAILED_EMBED_GEMS,
		ERR_FAILED_REMOVE_GEMS,
		ERR_FAILED_UPGRADE_GEM_LEVEL,
		//180
		ERR_FAILED_UPGRADE_GEM_QUALITY,
		ERR_FAILED_EXTRACT_GEM,
		ERR_FAILED_SMELT_GEM,
		ERR_SAME_GEM_SLOT_SHAPE,
		ERR_ITEM_CAN_NOT_GEM_SLOT_IDENTIFY,

		//185
		ERR_PLAYER_KEEP_ALL_SLOTS_WHEN_REBUILD,
		ERR_GEM_UPGRADE_LEVEL_PROB_NOT_ENOUGH,
	};

	enum
	{
		GOP_INSTALL,
		GOP_UNINSTALL,
		GOP_BIND,
		GOP_BIND_DESTORY,
		GOP_BIND_DESTORY_RESTORE,
		GOP_MOUNT,
		GOP_TALISMAN_REFINE,
		GOP_TALISMAN_LVLUP,
		GOP_TALISMAN_RESET,
		GOP_TALISMAN_COMBINE,
		GOP_TALISMAN_ENCHANT,
		GOP_FLY,
		GOP_LOCK,
		GOP_UNLOCK,
		GOP_SPIRIT_OPERATION,
		GOP_GIFT,
		GOP_PETEQUIP_REFINE,
		GOP_CHANGE_STYLE,
		GOP_MAGIC_REFINE,
		GOP_MAGIC_RESTORE,
		GOP_TRANSFORM,
		GOP_TELESTATION,
		GOP_REPAIR_DAMAGE_ITEM,
		GOP_UPGRADE_EQUIPMENT,
		GOP_CROSS_SERVER,
		GOP_TOWNSCROLL,
		GOP_GEM_OPERATION,
	};

	enum
	{
		DROP_TYPE_GM,
		DROP_TYPE_PLAYER,
		DROP_TYPE_TAKEOUT,
		DROP_TYPE_TASK,
		DROP_TYPE_RECHARGE,
		DROP_TYPE_DESTROY,
		DROP_TYPE_DEATH,
		DROP_TYPE_PRODUCE,
		DROP_TYPE_DECOMPOSE,
		DROP_TYPE_TRADEAWAY,
		DROP_TYPE_RESURRECT,
		DROP_TYPE_USE,
		DROP_TYPE_RUNE,
		DROP_TYPE_EXPIRE,
		DROP_TYPE_DEATH_DESTROY,
		DROP_TYPE_DEATH_DAMAGE,

	};

	enum
	{
		STATE_IGNITE,
		STATE_FROZEN,
		STATE_FURY,
		STATE_DARK,
	};

	enum GEM_NOTIFY_MSG
	{
		//客户端弹框的消息
		GEM_UPGRADE_SUCC = 0,
		GEM_UPGRADE_FAIL,
		GEM_SMELT_FAIL_SAME_QUALITY,

		//客户端聊天框显示的消息
		GEM_SLOT_IDENTIFY_SUCC = 50,
		GEM_SLOT_REBUILD_SUCC,
		GEM_SLOT_CUSTOMIZE_SUCC,
		GEM_EMBED_SUCC,
		GEM_REMOVE_SUCC,
		GEM_UPGRADE_QUALITY_SUCC,
		GEM_EXTRACT_SUCC,
		GEM_SMELT_SUCC,	
	};

	namespace CMD
	{
		using namespace INFO;

		struct player_enter_slice	//player进入可见区域
		{	
			single_data_header header;
			player_info_1 data;
		};

		struct npc_enter_slice
		{
			single_data_header header;
			npc_info data;
		};

		struct leave_slice	//player离开可见区域
		{	
			single_data_header header;
			int id;
		};

		struct notify_pos		//player更新位置
		{	
			single_data_header header;
			A3DVECTOR pos;
			int tag;
		};

		struct self_info_1
		{
			single_data_header header;
			INFO::self_info_1 info;
		};

		struct	player_info_1_list 		//player list
		{
			multi_data_header header;
			player_info_1 list[];
		};

		struct player_info_2_list 
		{
			multi_data_header header;
			/*
			   struct		//假设的代码，实际上由于info2是变长的结构，所以无法这样组织
			   {
			   int cid;
			   player_info_2 info;
			   }list[];
			 */
			char data[1];
		};

		struct player_info_3_list 
		{
			multi_data_header header;
			/*
			   struct		//假设的代码，实际上由于info3是变长的结构，所以无法这样组织
			   {
			   int cid;
			   player_info_3 info;
			   }list[];
			 */
			char data[1];
		};

		struct player_info_23_list 
		{
			multi_data_header header;
			/*
			   struct		//假设的代码，实际上由于info2/3是变长的结构，所以无法这样组织
			   {
			   int cid;
			   player_info_2 info2;
			   player_info_3 info3;
			   }list[];
			 */
			char data[1];
		};

		struct	npc_info_list 		// npc list
		{
			multi_data_header header;
			npc_info list[1];
		};

		struct matter_info_list
		{
			multi_data_header header;
			matter_info_1 list[1];
		};

		struct matter_enter_world
		{
			single_data_header header;
			matter_info_1 data;
		};

		struct npc_enter_world
		{
			single_data_header header;
			npc_info info;
		};

		struct player_info_00
		{
			single_data_header header;
			int pid;
			INFO::player_info_00 info;
		};

		struct self_info_00
		{
			single_data_header header;
			INFO::self_info_00 info;
			
		};

		struct npc_info_00
		{
			single_data_header header;
			int nid;
			INFO::npc_info_00 info;
		};
		
		struct object_move
		{
			single_data_header header;
			move_info data;
		};
		struct player_enter_world
		{
			single_data_header header;
			player_info_1	info;
		};
		struct player_leave_world
		{
			single_data_header header;
			int	   cid;
		};
		struct player_select_target
		{
			single_data_header header;
			int	id;
		};

		struct npc_dead
		{
			single_data_header header;
			int	   nid;
			int 	   attacker;
		};
		struct object_disappear
		{
			single_data_header header;
			int	   nid;
		};
		struct object_start_attack
		{
			single_data_header header;
			int	oid;
			int	tid;	//target id
			unsigned char   attack_stamp;
		};

		struct self_stop_attack
		{	
			single_data_header header;
			int	flag;
		};

		struct hurt_result //hurt_result
		{
			single_data_header header;
			int 	target_id;
			int	damage;
		};

		struct object_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int damage;
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			unsigned char attack_stamp;	//攻击的时间戳
			
		};

		struct object_skill_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int skill_id;
			int damage;
			int dt_damage;
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			unsigned char attack_stamp;
			
		};

		struct error_msg
		{
			single_data_header header;
			int  msg;
		};

		struct be_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	damage;
			unsigned char eq_index; //高位代表这次攻击是不是应该变橙色
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			char speed;			//攻击速度
		};

		struct be_skill_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	skill_id;
			int 	damage;
			unsigned char eq_index; //高位代表这次攻击是不是应该变橙色
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			char speed;			//攻击速度
		};


		struct be_hurt
		{
			single_data_header header;
			int 	attacker_id;
			int 	damage;
			unsigned char invader;
		};
		
		struct player_dead
		{
			single_data_header header;
			int  killer;
			int  player;
		};
		struct be_killed
		{
			single_data_header header;
			int  killer;
			A3DVECTOR pos;
			int time;	//复活需要的时间， 缺省为0
		};

		struct player_revival
		{
			single_data_header header;
			int id;
			short type;	//复活的类型 0 回城复活，1 开始复活 2：复活完全完成
			A3DVECTOR pos;
		};
		struct player_pickup_money
		{
			single_data_header header;
			int amount;
		};
		struct player_pickup_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
		};

		struct player_purchase_item
		{
			single_data_header header;
			int trade_id;
			size_t cost;
			unsigned char type;		//表示顾客买入还是卖出
			unsigned short item_count;
			struct
			{
				int item_id;
				unsigned short count;
				unsigned short inv_index;
				unsigned char  stall_index;
			} item_list[];
		};
		
		struct OOS_list
		{
			single_data_header header;
			unsigned int count;
			int id_list[1];
		};
		struct object_stop_move
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;
		};
		struct receive_exp
		{
			single_data_header header;
			int exp;
		};
		
		struct receive_bonus_exp
		{
			single_data_header header;
			double exp;
		};

		struct level_up
		{
			single_data_header header;
			int id;
		};

		struct unselect 
		{
			single_data_header header;
		};
		
		struct self_item_info
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int type;
			int expire_date;
			size_t count;
			unsigned short crc;
			unsigned short content_length;
			char content[];
		};
		struct self_item_empty_info
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
		};
		struct self_inventory_data
		{
			single_data_header header;
			unsigned char where;
			unsigned char inv_size;
			size_t content_length;
			char content[];
		};

		struct self_inventory_detail_data
		{
			single_data_header header;
			unsigned char where;
			unsigned char inv_size;
			size_t content_length;
			char content[];
		};

		struct  exchange_inventory_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};
		
		struct move_inventory_item
		{
			single_data_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short count;
		};

		struct player_drop_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			unsigned short count;
			int type;
			unsigned char drop_type;
		};
		struct exchange_equipment_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct equip_item
		{
			single_data_header header;
			unsigned char index_inv;
			unsigned char index_equip;
			unsigned short count_inv;
			unsigned short count_equip;
		};

		struct move_equipment_item
		{
			single_data_header header;
			unsigned char index_inv;
			unsigned char index_equip;
			unsigned short amount;
		};

		struct self_get_property
		{
			single_data_header header;
			unsigned int index_inv;
		//	q_extend_prop prop;		//不想引用头文件，所以注释掉了
		};


		struct player_extprop_base
		{
			single_data_header header;
			int id;
			int max_hp;		//最大hp
			int max_mp;		//最大mp
			int max_dp;
		};

		struct player_extprop_move
		{
			single_data_header header;
			int id;
			float walk_speed;	//行走速度 单位  m/s
			float run_speed;	//奔跑速度 单位  m/s
		};

		struct player_extprop_attack
		{
			single_data_header header;
			int id;
			int attack;		//攻击率 attack rate
			int damage_low;		//最低damage
			int damage_high;	//最大物理damage
			float attack_range;	//攻击范围
			int crit_rate;		//致命一击概率 千分数
			float crit_damage;	//致命一击时造成的额外伤害

		};

		struct player_extprop_defense
		{
			single_data_header header;
			int id;
			int resistance[6];	//魔法抗性
			int defense;		//防御力
			int armor;		//闪躲率（装甲等级）
		};
		
		struct team_leader_invite
		{
			single_data_header header;
			int leader;
			int seq;
			short pickup_flag;
			int familyid;
			int mafiaid;
		};

		struct team_reject_invite
		{
			single_data_header header;
			int member;
		};

		struct team_join_team
		{
			single_data_header header;
			int leader;
			short pickup_flag;
		};

		struct  team_member_leave
		{
			single_data_header header;
			int leader;
			int member;
			short type;
		};
		
		//表明自己离开了队伍
		struct team_leave_party
		{
			single_data_header header;
			int leader;
			short type;
		};

		struct team_new_member
		{
			single_data_header header;
			int member;
		};

		struct team_leader_cancel_party
		{
			single_data_header header;
			int leader;
		};

		struct team_member_data
		{
			single_data_header header;
			unsigned char member_count;
			unsigned char data_count;
			int leader;
			struct 
			{
				int id;
				short level;
				short dt_level;
				unsigned char combat_state;
				unsigned char pk_level;
				unsigned char wallow_level;
				unsigned char cls_type;
				int hp;
				int mp;
				int dp;
				int max_hp;
				int max_mp;
				int max_dp;
				int family_id;
				int master_id;
			} data[1];
		};

		struct teammate_pos
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int tag;
		};

		struct send_equipment_info
		{
			single_data_header header;
			unsigned short crc;
			int id;		//who
			unsigned int mask;
			struct 
			{
				short item;
				short mask;
			}data[];	//0 ~ 16
		};

		struct send_clone_equipment_info
		{
			single_data_header header;
			int id;		//who
			player_info_1 info;
			unsigned short crc;
			unsigned int mask;
			struct 
			{
				short item;
				short mask;
			}data[];	//0 ~ 16
		};
		
		struct equipment_info_changed
		{
			single_data_header header;
			unsigned short crc;
			int id;		//who
			unsigned int mask_add;
			unsigned int mask_del;
			struct 
			{
				short item;
				short mask;
			}data_add[];	//0 ~ 16
		};

		struct equipment_damaged
		{
			single_data_header header;
			unsigned char index;
		};

		struct team_member_pickup
		{
			single_data_header header;
			int id;
			int type;
			int count;
		};

		struct npc_greeting
		{
			single_data_header header;
			int id;
		};

		struct npc_service_content
		{
			single_data_header header;
			int id;
			int type;	//服务的类型
			size_t length;
			char data[];
		};

		struct item_to_money
		{
			single_data_header header;
			unsigned short index;		//在包裹栏里的索引号码
			int type;			//物品的类型
			size_t count;
			size_t money;
		};

		struct repair_all
		{
			single_data_header header;
			size_t cost;
		};
		
		struct repair
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			size_t cost;
		};

		struct renew
		{
			single_data_header header;
		};

		struct spend_money
		{
			single_data_header header;
			size_t cost;
		};

		struct player_pickup_money_in_trade
		{
			single_data_header header;
			int amount;
		};
		struct player_pickup_item_in_trade
		{
			single_data_header header;
			int type;
			unsigned short amount;
		};

		struct player_pickup_money_after_trade
		{
			single_data_header header;
			size_t amount;
		};
		struct player_pickup_item_after_trade
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned short index;
		};
		struct get_own_money
		{	
			single_data_header header;
			size_t amount;
			size_t capacity;
		};
		struct object_attack_once
		{
			single_data_header header;
			unsigned char arrow_dec;
		};

		struct object_cast_skill
		{
			single_data_header header;
			int caster;
			int target;
			int skill;
			unsigned short time;
			unsigned char level;
			unsigned char attack_stamp;
			unsigned char state;
			short cast_speed_rate;
		};

		struct skill_interrupted
		{
			single_data_header header;
			int caster;
		};

		struct self_skill_interrupted
		{
			single_data_header header;
			unsigned char reason;
		};

		struct skill_perform
		{
			single_data_header header;
		};

		struct object_be_attacked
		{
			single_data_header header;
			int id;
		};
		
		struct skill_data
		{
			single_data_header header;
			char content[];
		};

		struct player_use_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int item_id;
			unsigned short use_count;
		};

		struct embed_item
		{
			single_data_header header;
			unsigned char chip_idx;
			unsigned char equip_idx;
		};

		struct clear_embedded_chip
		{
			single_data_header header;
			unsigned short equip_idx;
			size_t  cost;
		};

		struct cost_skill_point
		{
			single_data_header header;
			int skill_point;
		};

		struct learn_skill
		{
			single_data_header header;
			int skill_id;
			int skill_level;
		};

		struct object_takeoff
		{
			single_data_header header;
			int object_id;
		};

		struct object_landing
		{
			single_data_header header;
			int object_id;
		};

		struct flysword_time_capacity
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int cur_time;
		};

		struct player_obtain_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
		};

		struct produce_start
		{
			single_data_header header;
			unsigned short use_time;
			unsigned short count;
			int type;
		};

		struct produce_once
		{
			single_data_header header;
			int type;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
		};
		
		struct produce_end
		{
			single_data_header header;
		};

		struct decompose_start
		{
			single_data_header header;
			unsigned short use_time;
			int type;
		};

		struct decompose_end
		{
			single_data_header header;
		};

		struct task_data
		{
			single_data_header header;
			size_t active_list_size;
			char active_list[1];
			size_t finished_list_size;
			char finished_list[1];
			size_t finished_time_size;
			char finished_time[1];
		};

		struct task_var_data
		{
			single_data_header header;
			size_t size;
			char data[1];
		};

		struct object_start_use
		{
			single_data_header header;
			int user;
			int item;
			unsigned short time;
		};
		
		struct object_cancel_use
		{
			single_data_header header;
			int user;
		};

		struct object_use_item
		{
			single_data_header header;
			int user;
			int item;
		};
		
		struct object_start_use_with_target
		{
			single_data_header header;
			int user;
			int target;
			int item;
			unsigned short time;
		};

		struct object_sit_down
		{
			single_data_header header;
			int id;
		//		A3DVECTOR pos; $$$$$$$$$$$$$
		};

		struct object_stand_up
		{
			single_data_header header;
			int id;
		};

		struct object_do_emote
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct server_timestamp
		{
			single_data_header header;
			int timestamp;
			int timezone_bias;
			int lua_version;
		};

		struct notify_root
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
		};

		struct self_notify_root
		{
			single_data_header header;
			A3DVECTOR pos;
			unsigned char type;
		};

		struct dispel_root
		{
			single_data_header header;
			unsigned char type;
		};

		struct pk_level_notify
		{
			single_data_header header;
			int id;
			char pk_level;
		};

		struct pk_value_notify
		{
			single_data_header header;
			int id;
			int value;
		};

		struct player_change_class
		{
			single_data_header header;
			int id;
			unsigned char cls;
		};

		struct self_stop_skill
		{
			single_data_header header;
		};

		struct update_visible_state
		{
			single_data_header header;
			int oid;
			int64_t newstate;
		};
	
		struct object_buff_notify               //OBJECT_BUFF_NOTIFY
		{       
			single_data_header header;
			int oid;
			unsigned short count;           
			struct
			{
				unsigned short state;
				unsigned short level;
			}list[];
		};

		struct self_buff_notify			//SELF_BUFF_NOTIFY
		{       
			single_data_header header;
			int oid;
			unsigned short count;
			struct
			{
				unsigned short state;
				unsigned short level;
				int buff_endtime;
			}list[];
		};
	
		struct player_gather_start
		{
			single_data_header header;
			int pid;		//player id
			int mid;		//mine id
			unsigned char use_time; //use time in sec;
		};

		struct player_gather_stop
		{
			single_data_header header;
			int pid;		//player id
		};

		struct trashbox_passwd_changed
		{
			single_data_header header;
			unsigned char has_passwd;
		};

		struct trashbox_passwd_state
		{
			single_data_header header;
			unsigned char has_passwd;
		};
		
		struct trashbox_open
		{
			single_data_header header;
			unsigned char where;
			unsigned short trashbox_size;
		};

		struct trashbox_close
		{
			single_data_header header;
		};

		struct trashbox_wealth
		{
			single_data_header header;
			size_t money;
		};

		struct exchange_trashbox_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char idx1;
			unsigned char idx2;
		};
		struct move_trashbox_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char src;
			unsigned char dest;
			size_t amount;
		};
		
		struct exchange_trashbox_inventory
		{
			single_data_header header;
			unsigned char where;
			unsigned char idx_tra;
			unsigned char idx_inv;
		};
		
		struct trash_item_to_inventory
		{
			single_data_header header;
			unsigned char where;
			unsigned char src;
			unsigned char dest;
			size_t amount;
		};
		
		struct inventory_item_to_trash
		{
			single_data_header header;
			unsigned char where;
			unsigned char src;
			unsigned char dest;
			size_t amount;
		};
		
		struct exchange_trash_money
		{
			single_data_header header;
			int inv_delta;
			int tra_delta;
		};

		struct enchant_result
		{
			single_data_header header;
			int 	caster;
			int 	target;
			int 	skill;
			char 	level;
			char	orange_name;
			char 	at_state;
			char	attack_stamp;
			int     enchant_value;
		};
		
		struct object_do_action
		{
			single_data_header header;
			int id;
			unsigned char emotion;
		};

		struct player_set_adv_data
		{
			single_data_header header;
			int id;
			int data1;
			int data2;
			
		};

		struct player_clr_adv_data 
		{
			single_data_header header;
			int id;
		};

		struct player_in_team
		{
			single_data_header header;
			int id;
			unsigned char state;	// 0 no team 1, leader, 2 member
		};

		struct team_apply_request
		{
			single_data_header header;
			int id;			//who
			int familyid;
			int mafiaid;
		};
		
		struct object_do_emote_restore
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct concurrent_emote_request
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct do_concurrent_emote 
		{
			single_data_header header;
			int id1;
			int id2;
			unsigned short emotion;
		};

		struct matter_pickup
		{
			single_data_header header;
			int matter_id;
			int who;
		};

		struct mafia_info_notify
		{
			single_data_header header;
			int pid;
			int mafia_id;
			int family_id;
			char mafia_rank;
		};

		struct mafia_trade_start
		{
			single_data_header header;
		};

		struct mafia_trade_end
		{
			single_data_header header;
		};
		
		struct task_deliver_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
			int state;                  //item状态：绑定、装备后绑定、锁定
		};

		struct task_deliver_reputaion
		{
			single_data_header header;
			int delta;
			int cur_reputaion;
		};

		struct task_deliver_exp
		{
			single_data_header header;
			int exp;
			int sp;
			int dt_exp;	//封神经验
		};

		struct task_deliver_money
		{
			single_data_header header;
			size_t amount;
			size_t cur_money;
		};

		struct task_deliver_level2
		{
			single_data_header header;
			int level2;
		};

		struct player_reputation
		{
			single_data_header header;
			int who;
			int reputation;
		};

		struct identify_result
		{
			single_data_header header;
			char index;
			char result;	//0 	
		};

		struct player_change_shape
		{
			single_data_header header;
			int pid;
			char shape;
		};

		struct player_enter_sanctuary
		{
			single_data_header header;
		};

		struct player_leave_sanctuary
		{
			single_data_header header;
		};

		struct player_open_market
		{
			single_data_header header;
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];		//最大28
		};

		struct self_open_market
		{
			single_data_header header;
			unsigned short count;
			struct 
			{
				int type;		//物品类型
				unsigned short index;	//如果是0xFFFF，表示是购买
				unsigned short count;	//卖多少个
				size_t price;		//单价
			} item_list;
			
		};

		struct player_cancel_market
		{
			single_data_header header;
			int pid;
		};

		struct player_market_info
		{
			single_data_header header;
			int pid;
			int market_id;
			size_t count;
			market_goods item_list[];
		};

		struct player_market_trade_success
		{
			single_data_header header;
			int trader;
		};

		struct player_market_name
		{
			single_data_header header;
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];	//最大28字节
		};

		struct player_start_travel
		{
			single_data_header header;
			int pid;
			unsigned char vehicle;
		};

		struct self_start_travel
		{
			single_data_header header;
			float speed;
			A3DVECTOR dest;
			int line_no;
			unsigned char vehicle;
		};

		struct player_complete_travel
		{
			single_data_header header;
			int pid;
			unsigned char vehicle;
		};

		struct gm_toggle_invisible
		{
			single_data_header header;
			unsigned char is_visible;
		};

		struct gm_toggle_invincible
		{
			single_data_header header;
			unsigned char is_invincible;
		};

		struct self_trace_cur_pos
		{
			single_data_header header;
			A3DVECTOR pos;
			unsigned short seq;
		};

		struct object_cast_instant_skill
		{
			single_data_header header;
			int id;
			int target;
			int skill;
			unsigned char level;
		};

		struct activate_waypoint
		{
			single_data_header header;
			unsigned short waypoint;
		};

		struct player_waypoint_list
		{
			single_data_header header;
			size_t count;
			unsigned short list[];
		};
		
		struct unlock_inventory_slot
		{
			single_data_header header;
			unsigned char where;
			unsigned short index;
		};

		struct team_invite_timeout
		{
			single_data_header header;
			int who;
		};

		struct player_pvp_no_protect
		{
			single_data_header header;
			int who;
		};

		struct cooldown_data
		{
			single_data_header header;
			unsigned short count;
			struct
			{
				unsigned short idx;
				int cooldown;
			}list[1];
		};

		struct skill_ability_notify
		{
			single_data_header header;
			int skill_id;
			int skill_ability;
		};

		struct personal_market_available
		{
			single_data_header header;
		};

		struct breath_data
		{
			single_data_header header;
			int breath;
			int breath_capacity;
		};

		struct player_stop_dive
		{
			single_data_header header;
		};

		struct trade_away_item
		{
			single_data_header header;
			short inv_index;
			int item_type;
			size_t item_count;
			int buyer;
		};

		struct player_enable_fashion_mode
		{
			single_data_header header;
			int who;
			int fashion_mask;
			unsigned char is_enable;
		};

		struct enable_free_pvp_mode
		{
			single_data_header header;
			unsigned char enable_type;	//0 close 1 free pvp 2 cultivation pvp 3 race pvp
		};

		struct object_is_invalid
		{
			single_data_header header;
			int id;
		};

		struct player_enable_effect
		{
			single_data_header header;
			short effect;
			int id;
		};

		struct player_disable_effect
		{
			single_data_header header;
			short effect;
			int id;
		};
		
		struct enable_resurrect_state
		{
			single_data_header header;
			float exp_reduce;
		};

		struct set_cooldown
		{
			single_data_header header;
			int cooldown_index; 
			int cooldown_time;
		};

		struct change_team_leader
		{
			single_data_header header;
			int old_leader; 
			int new_leader;
		};

		struct kickout_instance
		{
			single_data_header header;
			int instance_tag; 
			int timeout;		//如果是-1表示取消此次踢出
		};

		struct player_cosmetic_begin
		{
			single_data_header header;
			unsigned short index;
		};

		struct player_cosmetic_end
		{
			single_data_header header;
			unsigned short index;
		};

		struct cosmetic_success
		{
			single_data_header header;
			unsigned short crc;
			int id;
		};

		struct object_cast_pos_skill
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int skill;
			unsigned short time;
			unsigned char level;
		};

		struct change_move_seq
		{
			single_data_header header;
			unsigned short seq;
		};

		struct server_config_data
		{
			single_data_header header;
			int world_tag;
			int region_time;
			int precinct_time;
			int mall_time;
		};

		struct player_rush_mode
		{
			single_data_header header;
			char is_active;
		};

		struct trashbox_capacity_notify
		{
			single_data_header header;
			int capacity;
		};

		struct npc_dead_2
		{
			single_data_header header;
			int nid;
			int attacker;
		};

		struct produce_null
		{
			single_data_header header;
			int recipe_id;
		};

		struct double_exp_time
		{
			single_data_header header;
			int mode;
			int end_time;		//结束时间
		};
		
		struct available_double_exp_time
		{
			single_data_header header;
			int available_time;	//剩余时间
		};

		struct active_pvp_combat_state
		{
			single_data_header header;
			int who;
			char is_active;
		};

		struct duel_recv_request
		{
			single_data_header header;
			int player_id;
		};

		struct duel_reject_request
		{
			single_data_header header;
			int player_id;
			int reason;
		};

		struct duel_prepare
		{
			single_data_header header;
			int player_id;
			int delay;		//sec
		};
		
		struct duel_cancel
		{
			single_data_header header;
			int player_id;
		};

		struct duel_start
		{
			single_data_header header;
			int player_id;
		};
		
		struct duel_stop
		{
			single_data_header header;
			int player_id;
		};
		
		struct duel_result
		{
			single_data_header header;
			int id1;
			int id2;
			char result;	//1 id1 win 2 draw
		};

		struct player_bind_request
		{
			single_data_header header;
			int who;
		};

		struct player_bind_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bind_request_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_start
		{
			single_data_header header;
			int mule;
			int rider;
		};

		struct player_bind_stop
		{
			single_data_header header;
			int who;
		};

		struct player_mounting
		{
			single_data_header header;
			int id;
			int mount_id;
			char mount_level;
			char mount_type; // 0 - 默认类型   1 - 多人骑乘类型 
		};

		struct player_equip_detail
		{
			single_data_header header;
			int id;
			size_t content_length;
			char content[];
		};
		// skill
		// pet
		// pet_equip

		struct else_duel_start
		{
			single_data_header header;
			int player_id;
		};

		struct pariah_duration
		{
			single_data_header header;
			int time_left;
		};

		struct enter_battleground
		{
			single_data_header header;
			unsigned char battle_faction;	// 0 中立 1 攻方 2 守方 3 旁观 4 混战
			unsigned char battle_type;
			int battle_id;			// 战场id
			int end_timestamp;		// 结束时间

		};
		
		struct leave_battleground
		{
			single_data_header header;
		};

		struct enter_battlefield
		{
			single_data_header header;
			int faction_id_attacker;
			int faction_id_defender;
			int faction_id_attacker_assistant;
			int battle_id;			// 战场id
			int end_timestamp;		// 结束时间
		};
		
		struct leave_battlefield
		{
			single_data_header header;
		};

		struct battlefield_info
		{
			single_data_header header;
			unsigned char count;
			struct structure_info_t
			{
				unsigned char key;
				unsigned char status; //0 death
				A3DVECTOR pos;
				float hp_factor;
			}nodes[0];
		};

		struct battlefield_start
		{
			single_data_header header;
		};

		struct battlefield_end
		{
			single_data_header header;
			unsigned char result; // 1:攻方获胜 2:守方获胜
		};

		struct turret_leader_notify
		{
			single_data_header header;
			int turret_id;
			int turret_tid;
			int leader_id;
		};

		struct battle_result
		{
			single_data_header header;
			int result;
			int attacker_score;
			int defender_score;
		};

		struct player_recipe_data
		{
			single_data_header header;
			unsigned short count;
			unsigned short buf[];
		};

		struct player_learn_recipe
		{
			single_data_header header;
			unsigned short recipe_id;
		};
		
		struct player_produce_skill_info
		{
			single_data_header header;
			int level;
			int exp;
		};

		struct player_refine_result
		{
			single_data_header header;
			int item_index;
			int item_id;
			int result; // 0 成功 1 失败 删除材料 2失败删除材料 删除属性 3失败 装备爆掉 4 拆卸属性石 
				    // 5拆卸技能石
		};

		struct player_start_general_operation
		{
			single_data_header header;
			int player_id;
			int op_id;
			int duration;	//以0.05秒为单位的tick
		};

		struct player_stop_general_operation
		{
			single_data_header header;
			int player_id;
			int op_id;
		};

		struct player_cash
		{
			single_data_header header;
			int cash_amount;
			int cash_used;
		};

		struct player_bonus
		{
			single_data_header header;
			int bonus_amount;
			int bonus_used;
		};

		struct player_skill_addon
		{
			single_data_header header;
			int skill;
			int level;
		};

		struct player_skill_common_addon
		{
			single_data_header header;
			int addon;
		};
		struct player_extra_skill
		{
			single_data_header header;
			int skill;
			int level;
		};

		struct player_title_list
		{
			single_data_header header;
			size_t count;	//不是 byte size
			short title[];
		};
		struct player_add_title
		{
			single_data_header header;
			short title;
		};
		struct player_del_title
		{
			single_data_header header;
			short title;
		};

		struct player_region_reputation
		{
			single_data_header header;
			int count;
			int reputation[];
		};

		struct player_change_region_reputation
		{
			single_data_header header;
			int index;
			int new_reputation;
		};

		struct player_change_title
		{
			single_data_header header;
			int who;
			short title;
		};

		struct player_change_inventory_size
		{
			single_data_header header;
			int new_size;
		};

		struct player_bind_success
		{
			unsigned short inv_index;
			int item_id;
			int item_state;	 
		};

		struct player_change_spouse
		{
			single_data_header header;
			int who;
			int id;
		};

		struct player_invader_state
		{
			single_data_header header;
			int who;
			char bState;
		};

		struct player_mafia_contribution
		{
			single_data_header header;
			int contribution;
			int family_contribution;
		};

		struct lottery_bonus
		{
			single_data_header header;
			int lottery_id;
			int bonus_level;
			int bonus_item;
			int bonus_count;
			int bonus_money;
		};

		struct recorder_start
		{
			single_data_header header;
			int timestamp;
		};

		struct recorder_check_result
		{
			single_data_header header;
			char value[16];
		};

		struct player_use_item_with_arg
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int item_id;
			unsigned short use_count;
			unsigned short arg_size;
			char arg[];
		};

		struct object_use_item_with_arg
		{
			single_data_header header;
			int user;
			int item;
			unsigned short arg_size;
			char arg[];
		};

		struct mafia_trashbox_capacity_notify
		{
			single_data_header header;
			int cap;
		};

		struct npc_service_result
		{
			single_data_header header;
			int type;
		};

		// 只能发给附近的人
		struct battle_flag_change
		{
			single_data_header header;
			int matter_id;
			char origin_state;
			char new_state;
		};

		struct battle_player_info
		{
			single_data_header header;
			int id;
			unsigned char battle_faction;
			unsigned int battle_score;
			unsigned short kill;
			unsigned short death;
		};

		struct battle_info
		{
			struct player_info_in_battle
			{
				int id;
				unsigned char battle_faction;
				unsigned int battle_score;
				unsigned short kill;
				unsigned short death;
			};
			single_data_header header;
			unsigned char attacker_building_left;
			unsigned char defender_building_left;
			unsigned char player_count;
			player_info_in_battle info[0];
		};
		// 战场开始
		struct battle_start
		{
			single_data_header header;
		};

		// 广播使用
		struct battle_flag_change_status
		{
			single_data_header header;
			int flag_id;
			char origin_state;
			char new_state;
		};

		struct player_wallow_info               
		{       
			single_data_header header;
			unsigned char anti_wallow_active;
			unsigned char wallow_level;
			int msg;
			int play_time;
			int light_timestamp;
			int heavy_timestamp;
		};

		struct talisman_exp_notify
		{
			single_data_header header;
			unsigned short where;
			unsigned short index;
			int new_exp;
		};

		struct talisman_combine
		{
			single_data_header header;
			int id;
			int type1;
			int type2;
			int type3;
		};

		struct talisman_value
		{
			single_data_header header;
			int value;
		};

		struct auto_bot_begin
		{
			single_data_header header;
			unsigned char active;
		};

		struct anti_cheat_bonus
		{
			single_data_header header;
			unsigned short bonus_type;
		};
		
		// 玩家的战场积分
		struct battle_score
		{	
			single_data_header header;
			size_t score;
		};
		struct gain_battle_score
		{
			single_data_header header;
			size_t inc;
		};
		struct spend_battle_score
		{
			single_data_header header;
			size_t dec;
		};

		struct talent_notify
		{
			single_data_header header;
			int value;
		};

		struct notify_safe_lock
		{
			single_data_header header;
			unsigned char active;
			int time;
			int max_time;
		};

		struct battlefield_construction_info
		{
			single_data_header header;
			int res_a;
			int res_b;
			struct node_t
			{
				int type;
				int key;
				struct attr_t
				{
					unsigned char attack_level;
					unsigned char defence_level;
					unsigned char range_level;
					unsigned char ai_level;
					int maxhp_level;
				};
				attr_t attr;
			};
			unsigned char count;
			node_t nodes[0];
		};

		struct battlefield_contribution_info
		{
			single_data_header header;
			struct node_t
			{
				int player_id;
				int res_a;
				int res_b;
			};
			unsigned char page;		//当前页号,0开头
			unsigned char max_page;		//最大页号,0开头
			unsigned char count;		//个数
			node_t nodes[0];
		};

		struct player_equip_pet_bedge
		{
			single_data_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};
		
		struct player_equip_pet_equip
		{
			single_data_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};

		struct player_set_pet_status
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char main_status;	// 0 战斗 1 采集 2 制造 3 休息
			unsigned char sub_status;	// 只有main_status为 1时有用 ,0 种植 1 伐木 2 狩猎 3 钓鱼 4 采矿 5 考古
		};

		struct player_summon_pet
		{
			single_data_header header;
			unsigned char pet_index;        
			int pet_tid;			//宠物的模版id
			int pet_id;			//宠物的实际id
		};

		struct player_recall_pet
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;			//宠物的模版id
			int pet_id;			//宠物的实际id
		};

		struct player_combine_pet
		{
			single_data_header header;
			unsigned char pet_index;        //宠物索引
			int pet_tid;			//宠物的模版id
			unsigned char type;		//0 通灵,1 御宝
		};

		struct player_uncombine_pet
		{
			single_data_header header;
			unsigned char pet_index;	//宠物索引
			int pet_tid;			//宠物的模版id
		};

		struct player_start_pet_op
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int delay;			//延迟时间,单位是50ms的tick
			unsigned char operation;	//操作类型 0:放出 1:召回 2:通灵 3:解除通灵 4:御宝 5:解除御宝
		};

		struct player_stop_pet_op
		{
			single_data_header header;
			unsigned char operation;
		};

		struct player_pet_recv_exp
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int cur_exp;			//增加的经验
		};

		struct player_pet_levelup
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int new_level;			//新级别
			int cur_exp;			//当前的经验值 
		};
		
		struct player_pet_honor_point
		{
			single_data_header header;
			unsigned char pet_index;
			int cur_honor_point;
			int max_honor_point;
		};

		struct player_pet_hunger_point
		{
			single_data_header header;
			unsigned char pet_index;
			int cur_hunge_point;
			int max_hunge_point;
		};

		struct player_pet_age_life
		{
			single_data_header header;
			unsigned char pet_index;
			int age;
			int life;
		};

		struct player_pet_hp_notify
		{
			single_data_header header;
			unsigned char pet_index;
			int cur_hp;
			int max_hp;
			int cur_vp;
			int max_vp;
		};

		struct player_pet_dead
		{
			single_data_header header;
			unsigned char pet_index;
		};

		struct player_pet_ai_state
		{
			single_data_header header;
			unsigned char pet_index;
			unsigned char stay_state;
			unsigned char aggro_state;
		};

		struct player_pet_room_capacity
		{
			single_data_header header;
			unsigned char capacity;
		};

		struct player_pet_set_auto_skill
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int skill_id;
			unsigned char set_flag; //1设置 0取消
		};

		struct player_pet_set_skill_cooldown
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int cooldown_index;
			int cooldown_time;
		};

		struct battlefield_building_status_change
		{
			single_data_header header;
			unsigned char key;
			unsigned char status; //0 death
			A3DVECTOR pos;
			float hp_faction;
		};

		struct player_set_pet_rank
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char rank;
		};

		struct player_reborn_info
		{
			single_data_header header;
			unsigned short count;	//转了几次，不是字节数
			struct
			{
				short level;	//本次转生时的等级
				short prof;	//本次转生时的职业
			} info[0/*count*/];
		};

		struct script_message
		{
			single_data_header header;
			int player_id;
			int pet_id;
			unsigned short channel_id;
			unsigned short msg_id;
		};

		struct player_pet_civilization
		{
			single_data_header header;
			int civilization;	//宠物文明度
		};

		struct player_pet_construction
		{
			single_data_header header;
			int construction;	//宠物工坊建设度
		}; 

		struct player_move_pet_bedge
		{
			single_data_header header;
			unsigned char src_index;
			unsigned char dst_index;
		};

		struct player_start_fly
		{
			single_data_header header;
			int who;
			char active;
		};

		struct player_pet_prop_added
		{
			single_data_header header;
			unsigned char pet_index;
			unsigned short maxhp;
			unsigned short maxmp;
			unsigned short defence;
			unsigned short attack;
			unsigned short resistance[6];
			unsigned short hit;
			unsigned short jouk;
			float crit_rate;
			float crit_damage;
			int equip_mask;
		};

		struct sect_become_disciple
		{
			single_data_header header;
			int who;
			int master_id;
		};

		struct cultivation_notify
		{
			single_data_header header;
			int who;
			char new_cult;
		};

		struct offline_agent_bonus
		{
			single_data_header header;
			int bonus_time;		//min
			int left_time; 		//min
			double bonus_exp;
		};

		struct offline_agent_time
		{
			single_data_header header;
			int left_time; 		//min
		};
		
		struct spirit_power_decrease
		{
			single_data_header header;
			short index;
			short dec_amount;
			int result;
		};

		struct player_bind_ride_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bind_ride_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_ride_join
		{
			single_data_header header;
			int leader;
			int member_id;
			char bind_pos;
		};

		struct player_bind_ride_leave
		{
			single_data_header header;
			int leader;
			int member_id;
			char bind_pos;
		};
		
		struct player_bind_ride_kick
		{
			single_data_header header;
			int leader;
			char bind_pos;
		};
		
		struct exchange_pocket_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_pocket_item
		{
			single_data_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct exchange_inventory_pocket_item
		{
			single_data_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
		};

		struct move_pocket_item_to_inventory
		{
			single_data_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
			unsigned short amount;
		};

		struct move_inventory_item_to_pocket
		{
			single_data_header header;
			unsigned char idx_inv;
			unsigned char idx_poc;
			unsigned short amount;
			
		};

		struct move_all_pocket_item_to_inventory
		{
			single_data_header header;
		};

		struct player_change_pocket_size
		{
			single_data_header header;
			int new_size;
		};

		struct notify_fashion_hotkey
		{
			single_data_header header;
			int count;
			struct key_combine
			{
				int index;
				int id_head;
				int id_cloth;
				int id_shoe;
			} key[];
		};

		struct exchange_fashion_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct exchange_inventory_fashion_item
		{
			single_data_header header;
			unsigned char idx_inv;
			unsigned char idx_fas;
		};

		struct exchange_equipment_fashion_item
		{
			single_data_header header;
			unsigned char idx_equ;
			unsigned char idx_fas;
		};

		struct exchange_hotkey_equipment_fashion
		{
			single_data_header header;
			unsigned char idx_key;
			unsigned char idx_head; //0xff代表那个部位不需要替换
			unsigned char idx_cloth;
			unsigned char idx_shoe;  	
		};
		
		struct peep_info
		{
			single_data_header header;
			int id;	
		};

		struct self_killingfield_info
		{
			single_data_header header;
			int battle_score;
			int rank;
			int conKill;
			int postion;
		};

		struct killingfield_info
		{
			struct player_info_in_killingfield
			{
				int id;
				unsigned char faction;
				int score;
				int rank;
				int conKill;
			};
			single_data_header header;
			int attacker_score;
			int defender_score;
			int attacker_count;
			int defender_count;
			int player_count;
			player_info_in_killingfield info[0];
		};

		struct arena_info
		{
			struct player_info_in_arena
			{
				int score;
				int kill_count;
			};
			single_data_header header;
			int kill_count;
			int score;
			int apply_count;
			int remain_count;
			int player_count;
			player_info_in_arena info[0];
		};

		struct rank_change  //玩家军衔变化
		{
			single_data_header header;
			int player_id;
			char new_rank;
		};

		struct change_killingfield_score
		{
			single_data_header header;
			int old_score;
			int new_score;
		};

		struct player_change_style
		{
			single_data_header header;
			int player_id;
			unsigned char faceid;
			unsigned char hairid;
			unsigned char earid;
		    unsigned char tailid;	
			unsigned char fashionid;
		};
		
		struct player_change_vipstate
		{
			single_data_header header;
			int player_id;
			unsigned char new_state;
		};

		struct crossserver_battle_info
		{
			single_data_header header;
			int attacker_score;
			int defender_score;
			int kill_count;
			int death_count;
		};

		struct crossserver_team_battle_info
		{
			struct history_score
			{
				int attacker_score;
				int defender_score;
			};

			single_data_header header;
			unsigned char battle_state; //战场当前状态，0表示休息，1表示战斗
			int cur_state_end_time;     //当前状态的结束时间，该值为绝对时间
			int attacker_score;         //当前红方分数
			int defender_score;         //当前蓝方分数
			int best_killer_id;         //每小局累计击杀数最高的player

			unsigned char history_round_count;  //历史局数 
			history_score score_info[0];
		};


		/**
		 * @brief 阵法开始启动，想客户端广播一条消息
		 */
		struct notify_circleofdoom_start
		{
			single_data_header header;
			int  sponsor_id; //阵法发起者的id
			char faction;    //1表示按组队收集，2表示按帮派收集，3表示按家族收集
		};

		
		/**
		 * @brief 阵法发起成功后，定时向客户端同步阵法内成员信息
		 */
		struct notify_circleofdoom_info
		{
			single_data_header header;
			int sponsor_id;   //阵法发起者的id 
			int member_count; //阵法成员的个数，不包含阵法发起者
			int member_id[0];
		};


		/**
		 * @brief 通知客户端哪个阵法停止
		 */
		struct notify_circleofdoom_stop
		{
			single_data_header header;
			int sponsor_id;  //阵法发起者的id
		};

		
		struct achievement
		{
			unsigned short achieve_id;	// 成就id 

			unsigned short premiss_mask;	// 以完成前提位图
			char premiss_count;		// 前提数量
			struct _premiss			// 前提的数据结构
			{
				char premiss_id;	// 前提id;
				char data[1];		// 前提数据
			} premiss[1];
		};

		struct achievement_spec_info
		{
			unsigned short id;
			int finish_time;
		};

		struct player_achievement
		{
			single_data_header header;

			size_t 	map_count;			//成就map大小
			char 	map_data[1];			//成就map数据

			unsigned short  active_count;		//激活中的成就数量
			achievement achieve[1];			//激活中的成就数据

			int achieve_point;			//成就点
			int current_achieve_point;		//剩余的成绩点

			size_t spec_count;			//特殊成就数量
			achievement_spec_info info[1];		//特殊成就数据
			
		};

		struct player_achievement_finish
		{
			single_data_header header;
			unsigned short achieve_id;	
			int achieve_point;	
			int finish_time;
		};

		struct player_achievement_active
		{
			single_data_header header;
			achievement achieve;
		};

		struct player_premiss_data
		{
			single_data_header header;
			unsigned short achieve_id;	
			char premiss_id;
			char data[1];
		};
		struct player_premiss_finish
		{
			single_data_header header;
			unsigned short achieve_id;	
			char premiss_id;
		};
		struct player_achievement_map
		{
			single_data_header header;
			int 	target;
			int achieve_point;	
			size_t 	map_count;			//成就map大小
			char 	map_data[1];			//成就map数据
		};
		struct achievement_message
		{
			single_data_header header;
			unsigned short achieve_id;
			int param;
			unsigned char name_len;
			char name[];		
		};
		struct self_instance_info
		{
			single_data_header header;
			int level_score;
			int monster_score;
			int time_score;	
			int death_penalty_score;
		};
		struct instance_info
		{
			single_data_header header;
			int cur_level; 
			size_t level_count;
			char level_status[0];
		};
		struct skill_continue
		{
			single_data_header header;
			int caster_id;
			int skill_id;
		};
		struct player_start_transform
		{
			single_data_header header;
			int who;
			int template_id;
			char active;
			char type;
			char level;
			char expLevel;
		};
		struct magic_duration_decrease
		{
			single_data_header header;
			short index;
			short dec_amount;
			int result;
		};
		struct magic_exp_notify
		{
			single_data_header header;
			unsigned short where;
			unsigned short index;
			int new_exp;
		};
		
		struct transform_skill_data
		{
			single_data_header header;
			char content[];
		};
		
		struct player_enter_carrier  
		{
			single_data_header header;
			int player_id;
			int carrier_id; 		//想要乘坐的交通工具id
			A3DVECTOR rpos;			//相对坐标和方向
			unsigned char rdir;
			unsigned char success;
		};

		struct player_leave_carrier 
		{
			single_data_header header;
			int player_id;
			int carrier_id; 		//想要离开的交通工具id
			A3DVECTOR pos;			//绝对坐标和方向
			unsigned char dir;
			unsigned char success;
		};
		
		struct player_move_on_carrier	
		{
			single_data_header header;
			move_info info;
		};

		struct player_stop_move_on_carrier
		{
			single_data_header header;
			int id;
			A3DVECTOR rpos;
			unsigned short speed;
			unsigned char rdir;
			unsigned char move_mode;
		};

		struct skill_proficiency
		{
			single_data_header header;
			int id;
			int proficiency;
		};

		struct notify_mall_prop
		{
			single_data_header header;
			int mall_prop; 
		};

		struct send_pk_message
		{
			single_data_header header;
			int killer;
			int deader;
		};

		struct update_combo_state
		{
			single_data_header header;
			char is_start;
		       	char color[5];	
		};

		struct enter_dim_state
		{
			single_data_header header;
			int id;
			bool state;
		};
		
		struct enter_invisible_state
		{
			single_data_header header;
			int id;
			bool state;
		};

		struct object_charge
		{
			single_data_header header;
			int id;
			char type; 		//0, 向目标瞬移 1, 向目标冲锋
			int target_id;
			A3DVECTOR destPos;
		};
		
		struct enter_territory
		{
			single_data_header header;
			unsigned char battle_faction;	// 0 中立 1 攻方 2 守方 3 旁观 4 混战
			unsigned char battle_type;
			int battle_id;			// 战场id
			int end_timestamp;		// 结束时间

		};

		struct leave_territory
		{
			single_data_header header;
		};

		struct last_logout_time
		{
			single_data_header header;
			int time;
		};
		
		struct combine_skill_data
		{
			single_data_header header;
			char content[];
		};
		
		struct circle_info_notify
		{
			single_data_header header;
			int roleid;
			unsigned int circle_id;
			unsigned char circle_title;
		};

		struct receive_deliveryd_exp
		{
			single_data_header header;
			int exp;
			unsigned int award_type;
		};

		struct deity_levelup
		{
			single_data_header header;
			int id;
			short cur_lvl;	//当前等级
			char success;	//0 - 失败  1-成功
		};

		struct object_special_state
		{
			single_data_header header;
			int roleid;
			int type;
			char on;	//0- 停止 1-开始 
		};
		
		struct lottery2_bonus
		{
			single_data_header header;
			int lottery_id;
			int bonus_index;
			int bonus_level;
			int bonus_item;
			int bonus_count;
			int bonus_money;
		};

		struct gem_notify	
		{
			single_data_header header;
			int gem_id;
			int msg; 	//Define by enum GEM_NOTIFY_MSG
		};
		
		struct player_scale
		{
			single_data_header header;
			int roleid;
			char on;				//0, 进行缩放， //1，还原
			int scale_ratio;	 	//缩放比例, 百分数
		};

		struct player_move_cycle_area
		{
			char mode;	//0, Add; 1, remove; 2, clear
			char type;	//0, permit area; 1, limit area;
			int idx;
			float radius;		//半径
			A3DVECTOR center;	//中心点
		};

		struct enter_raid
		{
			single_data_header header;
			unsigned char raid_faction;	// 0 中立 1 攻方 2 守方 3 旁观
			unsigned char raid_type;
			int raid_id;			// 战场id
			int end_timestamp;		// 结束时间

		};
		
		struct leave_raid
		{
			single_data_header header;
		};
		/*liuyue-facbase
		struct notify_raid_pos
		{
			single_data_header header;
			A3DVECTOR pos;
			int raid_map_id;		//副本地图号, 客户端加载资源应该用这个ID
			int tag;				//副本实例tag
		};
		*/
		struct player_raid_counter
		{
			single_data_header header;
			int cnt;
			struct
			{
				int raid_map_id;	//副本地图号
				int counter;		//当日进入次数
			} raid_counter[];
		};

		struct object_be_moved
		{
			single_data_header header;
			move_info data;
		};

		struct player_pulling
		{
			single_data_header header;
			bool on;
		};

		struct player_be_pulled
		{
			single_data_header header;
			int player_pulling_id;
			int player_pulled_id;
			bool on;
			unsigned char type;		//0, 拉扯，1.牵引
		};

		struct lottery3_bonus
		{
			single_data_header header;
			int lottery_id;
			int bonus_index;
			int bonus_level;
			int bonus_item;
			int bonus_count;
			int bonus_money;
		};

		struct raid_info
		{
			struct player_info_in_raid
			{
				int id;
				short cls;
				short level;
				short sec_level;
			};

			single_data_header header;
			unsigned char player_count;
			player_info_in_raid info[0];
		};

		struct trigger_skill_time
		{
			single_data_header header;
			short skill_id;
			short time;//秒
		};
	}
}

namespace C2S
{
	enum MOVE_MODE
	{
		MOVE_MODE_WALK		= 0x00,
		MOVE_MODE_RUN		= 0x01,
		MOVE_MODE_STAND		= 0x02,
		MOVE_MODE_FALL		= 0x03,
		MOVE_MODE_SLIDE		= 0x04,
		MOVE_MODE_KNOCK		= 0x05,
		MOVE_MODE_FLY_FALL 	= 0x06,
		MOVE_MODE_RETURN	= 0x07,
		MOVE_MODE_JUMP		= 0x08,
		MOVE_MODE_DODGE		= 0x09,
		MOVE_MODE_PULL		= 0x0A,
		MOVE_MASK_DEAD		= 0x20,
		MOVE_MASK_SKY		= 0x40,
		MOVE_MASK_WATER		= 0x80

	};

	enum FORCE_ATTACK_MASK
	{	
		FORCE_ATTACK_WHITE	= 0x01,		//攻击白名
		FORCE_ATTACK_RED	= 0x02,		//攻击红名
		FORCE_ATTACK_FAMILY	= 0x04,		//攻击家族
		FORCE_ATTACK_MAFIA	= 0x08,		//攻击帮派成员
		FORCE_ATTACK_ZONE	= 0x10,		//攻击跨服玩家
		FORCE_ATTACK_ALL	= 0x1F,		//攻击所有
	};
	
	namespace INFO
	{
		struct move_info
		{
			A3DVECTOR cur_pos;
			A3DVECTOR next_pos;
			unsigned short use_time;	//使用的时间 单位是ms
							//使用的时间对于逻辑服务器来说，只是一个参考值
							//同时用于检测用户的指令是否正确，无论如何，
							//用户的移动都会在固定的0.5秒钟以后进行
			unsigned short speed;
			unsigned char  move_mode;		//walk run swim fly .... walk_back run_back
		};
	}

	enum
	{
		PLAYER_MOVE,
		LOGOUT,
		SELECT_TARGET,
		NORMAL_ATTACK,
		RESURRECT_IN_TOWN,	//城镇复生
//5		
		RESURRECT_BY_ITEM,	//物品复生
		PICKUP,			//检起物品或者金钱
		STOP_MOVE,
		UNSELECT,		//中止当前选定的目标
		GET_ITEM_INFO,		//取得物品特定位置的信息

//10
		GET_INVENTORY_LIST,		//取得某个位置上的所有物品列表
		GET_INVENTORY_DETAIL,	//取得某个位置上的所有物品列表，包含详细的物品数据
		EXCHANGE_INVENTORY_ITEM,
		MOVE_INVENTORY_ITEM,
		DROP_INVENTORY_ITEM,

//15		
		DROP_EQUIPMENT_ITEM,
		EXCHANGE_EQUIPMENT_ITEM,
		EQUIP_ITEM,		//装备物品，将物品栏上和装备栏上的两个位置进行调换
		MOVE_ITEM_TO_EQUIPMENT,
		GOTO,

//20		
		DROP_MONEY,		//扔出钱到地上
		SELF_GET_PROPERTY,
		LEARN_SKILL,		//学习技能
		GET_EXTPROP_BASE,
		GET_EXTPROP_MOVE,

//25
		GET_EXTPROP_ATTACK,
		GET_EXTPROP_DEFENSE,
		TEAM_INVITE,
		TEAM_AGREE_INVITE,
		TEAM_REJECT_INVITE,
//30
		TEAM_LEAVE_PARTY,
		TEAM_KICK_MEMBER,
		TEAM_GET_TEAMMATE_POS,
		GET_OTHERS_EQUIPMENT,
		CHANGE_PICKUP_FLAG,

//35		
		SERVICE_HELLO,
		SERVICE_GET_CONTENT,
		SERVICE_SERVE,
		GET_OWN_WEALTH,
		GET_ALL_DATA,
//40		
		USE_ITEM,
		CAST_SKILL,
		CANCEL_ACTION,
		RECHARGE_EQUIPPED_FLYSWORD,
		RECHARGE_FLYSWORD,
//45		
		USE_ITEM_WITH_TARGET,
		SIT_DOWN,
		STAND_UP,
		EMOTE_ACTION,
		TASK_NOTIFY,
//50
		ASSIST_SELECT,
		CONTINUE_ACTION,
		STOP_FALL,	//终止跌落
		GET_ITEM_INFO_LIST,
		GATHER_MATERIAL,
//55		
		GET_TRASHBOX_INFO,
		EXCHANGE_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,
		EXHCANGE_TRASHBOX_INVENTORY,
		MOVE_TRASHBOX_ITEM_TO_INVENTORY,
//60		
		MOVE_INVENTORY_ITEM_TO_TRASHBOX,
		EXCHANGE_TRASHBOX_MONEY,
		TRICKS_ACTION,
		SET_ADV_DATA,
		CLR_ADV_DATA,
//65		
		TEAM_LFG_REQUEST,
		TEAM_LFG_REPLY,
		QUERY_PLAYER_INFO_1,
		QUERY_NPC_INFO_1,
		SESSION_EMOTE_ACTION,
//70 	
		CONCURRECT_EMOTE_REQUEST_NULL,
		CONCURRECT_EMOTE_REPLY_NULL,
		TEAM_CHANGE_LEADER,
		DEAD_MOVE,
		DEAD_STOP_MOVE,

//75
		ENTER_SANCTUARY,
		OPEN_PERSONAL_MARKET,
		CANCEL_PERSONAL_MARKET,
		QUERY_PERSONAL_MARKET_NAME,
		COMPLETE_TRAVEL,

//80
		CAST_INSTANT_SKILL,
		DESTROY_ITEM,
		ENABLE_PVP_STATE,
		DISABLE_PVP_STATE,
		TEST_PERSONAL_MARKET,

//85
		SWITCH_FASHION_MODE,
		REGION_TRANSPORT,
		RESURRECT_AT_ONCE,
		NOTIFY_POS_TO_MEMBER,
		CAST_POS_SKILL,

//90	
		ACTIVE_RUSH_MODE,
		QUERY_DOUBLE_EXP_INFO,
		DUEL_REQUEST,
		DUEL_REPLY,
		BIND_PLAYER_REQUEST,

//95
		BIND_PLAYER_INVITE,
		BIND_PLAYER_REQUEST_REPLY,
		BIND_PLAYER_INVITE_REPLY,
		BIND_PLAYER_CANCEL,
		QUERY_OTHER_EQUIP_DETAIL,

//100
		SUMMON_PET,	//召唤宠物
		RECALL_PET,	//收回宠物
		NO_USE_BANISH_PET,
		PET_CTRL_CMD,
		PRODUCE_ITEM,

//105
		MALL_SHOPPING,
		SELECT_TITLE,
		DEBUG_DELIVERY_CMD,
		DEBUG_GS_CMD,
		LOTTERY_CASHING,
//110
		CHECK_RECORDER,
		MALL_SHOPPING2,
		START_MOVE,
		USE_ITEM_WITH_ARG,
		AUTO_BOT_BEGIN,

//115
		AUTO_BOT_CONTINUE,
		GET_BATTLE_SCORE,
		GET_BATTLE_INFO,
		EQUIP_PET_BEDGE,	//装备宠物牌,将宠物牌栏上和装备栏上的两个位置进行调换
		EQUIP_PET_EQUIP,	//装备宠物装备,将宠物装备栏上和装备栏上的两个位置进行调换

//120
		COMBINE_PET,		//宠物合体
		UNCOMBINE_PET,		//解除合体
		SET_PET_STATUS,		//设置宠物状态
		SET_PET_RANK,		//设置宠物阶级
		MOVE_PET_BEDGE,		//调换宠物牌的位置

//125
		START_FLY,
		STOP_FLY,
		SET_FASHION_MASK,
		START_ONLINE_AGENT,
		JOIN_INSTANCE,
		
//130
		BIND_RIDE_INVITE, 		//多人骑乘邀请
		BIND_RIDE_INVITE_REPLY, 	//多人骑乘邀请的回复
		BIND_RIDE_CANCEL, 		//离开多人骑乘
		BIND_RIDE_KICK,             	//主骑踢人
		POST_SNS_MESSAGE,		//玩家发布信息

//135
		APPLY_SNS_MESSAGE,		//玩家应征信息
		VOTE_SNS_MESSAGE,		//玩家投票
		RESPONSE_SNS_MESSAGE, 		//玩家留言		
		EXCHANGE_POCKET_ITEM,
		MOVE_POCKET_ITEM,

//140
		EXCHANGE_INVENTORY_POCKET_ITEM,
		MOVE_POCKET_ITEM_TO_INVENTORY,
		MOVE_INVENTORY_ITEM_TO_POCKET,
		MOVE_ALL_POCKET_ITEM_TO_INVENTORY,
		CAST_ITEM_SKILL,
	
//145		
		UPDATE_FASHION_HOTKEY,
		EXCHANGE_FASHION_ITEM,
		EXCHANGE_INVENTORY_FASHION_ITEM,
		EXCHANGE_EQUIPMENT_FASHION_ITEM,
		EXCHANGE_HOTKEY_EQUIPMENT_FASHION,

//150
		BONUSMALL_SHOPPING,
		QUERY_OTHERS_ACHIEVEMENT,		// 查看其他人的成就
		UPDATE_PVP_MASK,
		START_TRANSFORM,			//开始变身
		STOP_TRANSFORM,				//停止变身
		
//155	
		CAST_TRANSFORM_SKILL,
		ENTER_CARRIER,
		LEAVE_CARRIER,
		MOVE_ON_CARRIER,
		STOP_MOVE_ON_CARRIER,

//160
		EXCHANGE_HOMETOWN_MONEY,
		GET_SERVER_TIMESTAMP,
		TERRITORY_LEAVE,
		CAST_CHARGE_SKILL,
		UPDATE_COMBINE_SKILL,

//165
		UNIQUE_BID_REQUEST,				//唯一最低价拍卖竞价
		UNIQUE_BID_GET,					//唯一最低价获取奖励物品
		GET_CLONE_EQUIPMENT,				//获取分身主人的装备信息
		TASK_FLY_POS,
		ZONEMALL_SHOPPING,

		
//170
		ACTIVITY_FLY_POS,
		DEITY_LEVELUP,
		QUERY_CIRCLE_OF_DOOM_INFO,      //查询阵法成员信息，用于客户端显示
		RAID_LEAVE,						//玩家主动离开
		CANCEL_PULLING,					//取消拉扯

//175
		GET_RAID_INFO,					//获得副本信息

		
//200		
		GM_COMMAND_START = 200,
		GMCMD_MOVE_TO_PLAYER,		//201
		GMCMD_RECALL_PLAYER,		//202
		GMCMD_OFFLINE,			//203
		GMCMD_TOGGLE_INVISIBLE,		//204
		GMCMD_TOGGLE_INVINCIBLE,	//205
		GMCMD_DROP_GENERATOR,		//206
		GMCMD_ACTIVE_SPAWNER,		//207
		GMCMD_GENERATE_MOB,             //208

		GMCMD_PLAYER_INC_EXP,		//209
		GMCMD_RESURRECT,		//210
		GMCMD_ENDUE_ITEM,		//211	
		GMCMD_ENDUE_SELL_ITEM,		//212
		GMCMD_REMOVE_ITEM,		//213
		GMCMD_ENDUE_MONEY,		//214
		GMCMD_ENABLE_DEBUG_CMD,		
		GMCMD_RESET_PROP,		
		GM_COMMAND_END,

	};

	namespace CMD
	{
		using namespace INFO;
		struct player_move
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//命令序号
		};
		struct player_logout
		{
			cmd_header header;
		};
		struct select_target
		{	
			cmd_header header;
			int id;
		};
		struct normal_attack
		{
			cmd_header header;
			char force_attack;		//强制攻击位 0x01 普通强制攻击 0x02 攻击本帮派 0x04攻击白名 0x08 攻击红名 0x10 攻击家族
							//技能和物品的的定义与此相同
		};

		struct pickup_matter
		{
			cmd_header header;
			int mid;
			int type;
		};
		
		struct resurrect
		{
			cmd_header header;
		};
		struct player_stop_move
		{
			cmd_header header;
			A3DVECTOR pos;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;		//命令序号
			unsigned short use_time;
		};

		struct get_item_info
		{
			cmd_header header;
			unsigned char where;
			unsigned char index;
		};

		struct get_inventory
		{
			cmd_header header;
			unsigned char where;
		};

		struct get_inventory_detail
		{
			cmd_header header;
			unsigned char where;
		};
		
		struct exchange_inventory_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_inventory_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct drop_inventory_item
		{
			cmd_header header;
			unsigned char index;
			unsigned short amount;
		};

		struct drop_equipment_item
		{
			cmd_header header;
			unsigned char index;
		};

		struct exchange_equip_item
		{
			cmd_header header;
			unsigned char idx1;
			unsigned char idx2;
		};

		struct equip_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_eq;
		};

		struct move_item_to_equipment
		{
			cmd_header header;
			unsigned char idx_inv;  //src
			unsigned char idx_eq;	 //dest
		};

		struct player_goto
		{
			cmd_header header;
			A3DVECTOR pos;
		};

		struct drop_money
		{
			cmd_header header;
			size_t amount; 
		};

		struct self_get_property
		{
			cmd_header header;
		};

		struct learn_skill 
		{
			cmd_header header;
			size_t skill_id;
		};

		struct get_extprop_base
		{
			cmd_header header;
		};

		struct get_extprop_move
		{
			cmd_header header;
		};

		struct get_extprop_attack
		{
			cmd_header header;
		};

		struct get_extprop_defense
		{
			cmd_header header;
		};

		struct team_invite
		{
			cmd_header header;
			int id;		//想谁发起邀请 
		};

		struct team_agree_invite
		{
			cmd_header header;
			int id;		//谁进行的邀请
			int team_seq;
		};

		struct team_reject_invite
		{
			cmd_header header;
			int id;		//谁进行的邀请
		};

		struct team_leave_party
		{
			cmd_header header;
		};

		struct team_kick_member
		{
			cmd_header header;
			int id;
		};
		
		struct team_get_teammate_pos
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct get_others_equipment
		{
			cmd_header header;
			unsigned short size;
			int idlist[];
		};


		struct set_pickup_flag
		{
			cmd_header header;
			short pickup_flag;
		};


		struct service_hello
		{
			cmd_header header;
			int id;
		};

		struct service_get_content
		{
			cmd_header header;
			int service_type;
		};

		struct service_serve
		{
			cmd_header header;
			int service_type;
			size_t len;
			char content[];
		};

		struct logout
		{
			cmd_header header;
			int logout_type;
			int offline_agent;
		};

		struct get_own_wealth
		{
			cmd_header header;
			char detail_inv;
			char detail_equip;
			char detail_task;
		};

		struct get_all_data
		{
			cmd_header header;
			char detail_inv;
			char detail_equip;
			char detail_task;
		//	char detail_fashion;
		};

		struct use_item
		{
			cmd_header header;
			unsigned char where;
			unsigned char count;
			unsigned short index;
			int  item_id;
		};


		struct cast_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};

		//查询阵法信息
		struct query_circleofdoom_info
		{
			cmd_header header;
			int sponsor_id;     //阵法发起者的id
		};

		struct cancel_action
		{
			cmd_header header;
		};

		struct recharge_equipped_flysword
		{	
			cmd_header header;
			unsigned char element_index; 
			int count;
		};
		
		struct recharge_flysword
		{	
			cmd_header header;
			unsigned char element_index; 
			unsigned char flysword_index;
			int count;
			int flysword_id;
		};

		struct use_item_with_target
		{
			cmd_header header;
			unsigned char where;
			unsigned char force_attack; //只对攻击性物品有效
			unsigned short index;
			int  item_id;
		};

		struct sit_down
		{
			cmd_header header;
		};

		struct stand_up
		{
			cmd_header header;
		};
		
		struct emote_action 
		{
			cmd_header header;
			unsigned short action;
		};

		struct task_notify
		{
			cmd_header header;
			unsigned int size;
			char buf[0];
		};

		struct assist_select
		{
			cmd_header header;
			int partner;
		};

		struct continue_action
		{
			cmd_header header;
		};

		struct get_item_info_list
		{
			cmd_header header;
			char  where;
			unsigned char  count;
			unsigned char  item_list[];
		};

		struct gather_material
		{
			cmd_header header;
			int mid;
			short tool_where;
			short tool_index;
			int tool_type;
			int task_id;
		};

		struct get_trashbox_info
		{
			cmd_header header;
			char detail;
		};

		struct exchange_trashbox_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_trashbox_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct exchange_trashbox_inventory
		{
			cmd_header header;
			unsigned char idx_tra;
			unsigned char idx_inv;
		};

		struct move_trashbox_item_to_inventory
		{
			cmd_header header;
			unsigned char idx_tra;
			unsigned char idx_inv;
			unsigned short amount;
		};

		struct move_inventory_item_to_trashbox
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_tra;
			unsigned short amount;
		};
		
		struct excnahge_trashbox_money
		{
			cmd_header header;
			unsigned int inv_money;
			unsigned int trashbox_money;
		};

		struct tricks_action 
		{
			cmd_header header;
			unsigned char action;
		};

		struct set_adv_data
		{
			cmd_header header;
			int data1;
			int data2;
		};

		struct clr_adv_data
		{
			cmd_header header;
		};

		struct team_lfg_request
		{
			cmd_header header;
			int id;
		};

		struct team_lfg_reply
		{
			cmd_header header;
			int id;
			bool result;
		};

		struct query_player_info_1
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct query_npc_info_1
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct session_emote_action
		{
			cmd_header header;
			//限制为256之内
			unsigned char action;
		};

		struct concurrent_emote_request
		{
			cmd_header header;
			unsigned short action;
			int target;
		};

		struct concurrent_emote_reply
		{
			cmd_header header;
			unsigned short result;
			unsigned short action;
			int target;
		};

		struct team_change_leader
		{
			cmd_header header;
			int new_leader;
		};

		struct dead_move 
		{
			cmd_header header;
			float y;
			unsigned short use_time;	//使用的时间 单位是ms
							//使用的时间对于逻辑服务器来说，只是一个参考值
							//同时用于检测用户的指令是否正确，无论如何，
							//用户的移动都会在固定的0.5秒钟以后进行
			unsigned short speed;
			unsigned char  move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;
		};
		
		struct dead_stop_move 
		{
			cmd_header header;
			float y;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;
		};

		struct enter_sanctuary
		{
			cmd_header header;
		};

		struct open_personal_market
		{
			cmd_header header;
			unsigned short count;
			char name[28];
			struct entry_t
			{
				int type;
				size_t index;
				size_t count;
				size_t price;
			} list[];
		};

		struct cancel_personal_market
		{
			cmd_header header;
		};

		struct query_personal_market_name
		{
			cmd_header header;
			unsigned short count;
			int list[];
		};

		struct complete_travel
		{
			cmd_header header;
		};

		struct cast_instant_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};
		
		struct cast_charge_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			int charge_target;
			A3DVECTOR charge_pos;
			unsigned char target_count;
			int  targets[];
		};

		struct destroy_item
		{
			cmd_header header;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//索引
			int type;			//是哪种物品
		};

		struct enable_pvp_state
		{
			cmd_header header;
		};

		struct disable_pvp_state
		{
			cmd_header header;
		};

		struct switch_fashion_mode
		{
			cmd_header header;
		};

		struct region_transport
		{
			cmd_header header;
			int region_index;
			int target_tag;
		};

		struct cast_pos_skill
		{
			cmd_header header;
			int skill_id;
			A3DVECTOR pos;
		};

		struct active_rush_mode
		{
			cmd_header header;
			int is_active;
		};

		struct query_double_exp_info
		{
			cmd_header header;
		};

		struct duel_request
		{
			cmd_header header;
			int target;
		};

		struct duel_reply
		{
			cmd_header header;
			int who;
			int param; // 0 同意  1 不同意
		};

		struct bind_player_request {
			cmd_header header;
			int who;
		};

		struct bind_player_invite
		{
			cmd_header header;
			int who;
		};

		struct bind_player_request_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct bind_player_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct query_other_equip_detail
		{
			cmd_header header;
			int target;
		};

		struct produce_item
		{
			cmd_header header;
			int recipe_id;
		};

		struct mall_shopping
		{
			cmd_header header;
			unsigned int count;
			struct __entry
			{
				short goods_id;
				short goods_index; 
				short goods_slot;
			}list[];
			//.....
		};

		struct  select_title
		{
			cmd_header header;
			short title;
		};

		struct debug_delivery_cmd
		{
			cmd_header header;
			short  type;
			char buf[];
		};
		
		struct lottery_cashing 
		{
			cmd_header header;
			int item_index;
		};

		struct check_recorder
		{
			cmd_header header;
			int item_index;
		};

		struct mall_shopping_2
		{
			cmd_header header;
			unsigned short goods_id;
			unsigned short goods_index; 
			unsigned short goods_slot;
			unsigned short count;
		};

		struct use_item_with_arg
		{
			cmd_header header;
			unsigned char where;
			unsigned char count;
			unsigned short index;
			int  item_id;
			char arg[];
		};

		struct auto_bot_begin
		{
			cmd_header header;
			unsigned char active;
			unsigned char need_question;
		};

		struct auto_bot_continue
		{
			cmd_header header;
		};

		struct get_battle_score
		{
			cmd_header header;
		};

		struct get_battle_info
		{
			cmd_header header;
		};

		struct equip_pet_bedge
		{
			cmd_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};

		struct equip_pet_equip
		{
			cmd_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};

		struct set_pet_status
		{
			cmd_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char main_status; // 0 战斗 1 采集 2 制造 3 休息
			unsigned char sub_status; // 只有main_status为 1时有用 ,0 种植 1 伐木 2 狩猎 3 钓鱼 4 采矿 5 考古
		};

		struct set_pet_rank
		{
			cmd_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char rank;
		};

		struct summon_pet
		{
			cmd_header header;
			unsigned char pet_index;
		};

		struct recall_pet
		{
			cmd_header header;
			unsigned char pet_index;
		};

		struct pet_ctrl_cmd
		{
			cmd_header header;
			unsigned char pet_index;
			int target;
			int pet_cmd;
			char buf[];
		};
		
		struct combine_pet
		{
			cmd_header header;
			unsigned char pet_index;
			unsigned char combine_type;	//0 通灵 1 御宝
		};

		struct uncombine_pet
		{
			cmd_header header;
			unsigned char pet_index;
		};

		struct move_pet_bedge
		{
			cmd_header header;
			unsigned char src_index;
			unsigned char dst_index;
		};

		struct set_fashion_mask
		{
			cmd_header header;
			int fashion_mask;
		};

		struct join_instance
		{
			cmd_header header;
			int battle_id;
		};

		struct bind_ride_invite
		{
			cmd_header header;
			int who;
		};

		struct bind_ride_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct bind_ride_kick
		{
			cmd_header header;
			char pos;
		};

		struct post_sns_message
		{
			cmd_header header;
			char message_type;  // 0 --marriage  1--friend 2--mentor 3--faction
			unsigned char occupation;
			char gender; 
			int level;
			int faction_level;
			unsigned int message_len;
			char message[];
			
		};

		struct apply_sns_message
		{
			cmd_header header;
			char message_type;
			int message_id;
			unsigned int message_len;
			char message[];
		};

		struct vote_sns_message
		{
			cmd_header header;
			char vote_type; // 0 --against  1--support
			char message_type;
			int message_id;
		};

		struct response_sns_message
		{
			cmd_header header;
			char message_type;
			int message_id;
			short dst_index;  //responsed message's id
			unsigned int message_len;
			char message[];
		};

		struct exchange_pocket_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_pocket_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct exchange_inventory_pocket_item
		{
			cmd_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
		};

		struct move_pocket_item_to_inventory
		{
			cmd_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
			unsigned short amount;
		};

		struct move_inventory_item_to_pocket
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_poc;
			unsigned short amount;
			
		};

		struct move_all_pocket_item_to_inventory
		{
			cmd_header header;
		};

		struct cast_item_skill
		{
			cmd_header header;
			int skill_id;
			char skill_type; //技能类型: 0--普通技能  1--瞬发技能
			int item_id;
			short item_index;
			unsigned char force_attack;
			unsigned char target_count;
			int targets[];
		};

		struct update_fashion_hotkey
		{
			cmd_header header;
			int count;
			struct key_combine
			{
				int index;
				int id_head;
				int id_cloth;
				int id_shoe;
			}key[];
		};

		struct exchange_fashion_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct exchange_inventory_fashion_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_fas;
		};

		struct exchange_equipment_fashion_item
		{
			cmd_header header;
			unsigned char idx_equ;
			unsigned char idx_fas;
		};
		
		struct exchange_hotkey_equipment_fashion
		{
			cmd_header header;
			unsigned char idx_key;
			unsigned char idx_head;		//0xff代表那个部位不需要替换
		       	unsigned char idx_cloth;
			unsigned char idx_shoe;	
		};
		
		struct bonusmall_shopping
		{
			cmd_header header;
			unsigned short goods_id;
			unsigned short goods_index; 
			unsigned short goods_slot;
			unsigned short count;
		};

		struct query_others_achievement
		{
			cmd_header header;
			int target;
		};

		struct update_pvp_mask
		{
			cmd_header header;
			char pvp_mask;
		};
		
		struct start_transform 
		{
			cmd_header header;
		};

		struct stop_transform 
		{
			cmd_header header;
		};
		
		struct cast_transform_skill
		{
			cmd_header header;
			int skill_id;
			char skill_type; //技能类型: 0--普通技能  1--瞬发技能
			unsigned char force_attack;
			unsigned char target_count;
			int targets[];
		};

		struct enter_carrier  
		{
			cmd_header header;
			int carrier_id; 		//想要乘坐的交通工具id
			A3DVECTOR rpos;			//相对坐标
			unsigned char rdir;		//相对方向
		};

		struct leave_carrier 
		{
			cmd_header header;
			int carrier_id; 		//想要离开的交通工具id
			A3DVECTOR pos;			//绝对坐标和方向
			unsigned char dir;		//绝对方向
		};
		
		struct move_on_carrier	
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//命令序号
		};

		struct stop_move_on_carrier
		{
			cmd_header header;
			A3DVECTOR rpos;
			unsigned short speed;
			unsigned char rdir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;		//命令序号
			unsigned short use_time;
		};

		struct exchange_hometown_money
		{
			cmd_header header;
			int amount;
		};
		
		struct get_server_timestamp 
		{
			cmd_header header;
		};

		struct territory_leave
		{
			cmd_header header;
		};

		struct update_combine_skill
		{
			cmd_header header;
			int skill_id;
			int element_count;
			int element_id[];
		};

		struct unique_bid_request
		{
			cmd_header header;
			int bid_money_lowerbound;
			int bid_money_upperbound;
		};

		struct unique_bid_get
		{
			cmd_header header;
		};	

		struct get_clone_equipment
		{
			cmd_header header;
		        int master_id;	
			int clone_id;
		};

		struct task_fly_pos
		{
			cmd_header header;
			int task_id;
			int npc_id;
		};
		
		struct zonemall_shopping
		{
			cmd_header header;
			unsigned short goods_id;
			unsigned short goods_index; 
			unsigned short goods_slot;
			unsigned short count;
		};
		
		struct activity_fly_pos
		{
			cmd_header header;
			int id;
		};

		struct deity_levelup
		{
			cmd_header header;
		};

/*------------------------------内部GM 命令------------------------------------*/		
		struct  gmcmd_move_to_player
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_recall_player
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_player_inc_exp
		{
			cmd_header header;
			int exp;
			int sp;
		};

		struct gmcmd_endue_item
		{
			cmd_header header;
			int item_id;
			size_t count;
		};

		struct gmcmd_endue_sell_item
		{
			cmd_header header;
			int item_id;
			size_t count;
		};

		struct gmcmd_remove_item
		{
			cmd_header header;
			int item_id;
			size_t count;
		};

		struct gmcmd_endue_money
		{
			cmd_header header;
			int money;
		};


		struct gmcmd_offline
		{
			cmd_header header;
		};

		struct gmcmd_resurrect
		{
			cmd_header header;
		};

		struct gmcmd_enable_debug_cmd
		{
			cmd_header header;
		};

		struct gmcmd_drop_generator
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_active_spawner
		{
			cmd_header header;
			bool is_active;
			int sp_id;
		};

		struct gmcmd_generate_mob
		{
			cmd_header header;
			int mob_id;
			int vis_id;
			short count;
			short life;
			size_t name_len;
			char name[];
		};

		struct raid_leave
		{
			cmd_header header;
		};

		struct cancel_pulling
		{
			cmd_header header;
		};

		struct get_raid_info
		{
			cmd_header header;
		};
	}
}

#pragma pack()
#endif

