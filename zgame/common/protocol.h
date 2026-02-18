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
			unsigned char  dir;		//256µÄ¶¨µãÊı±íÊ¾¶ÈÊı
			unsigned char  cls;		//Ö°Òµ
			unsigned char  level;		//ĞŞÕæ¼¶±ğ
			unsigned char level2;
			char  pk_level;			//pk¼¶±ğ -1 0 1 2 3 4 5 6
			char  cultivation;		//ÏÉÄ§·ğÔì»¯
			int   reputation;		//ÍşÍû
			unsigned int   object_state;	//µ±Ç°×´Ì¬£¬µÚ0Î»±íÊ¾ÊÇ·ñËÀÍö
			unsigned int   extra_state;	//Íæ¼ÒµÄÁíÍâ×´Ì¬
			unsigned int   extra_state2;    //Íæ¼ÒµÄÁíÍâ×´Ì¬2
		};

		struct player_info_2			//name ,customize data ²»³£¸Ä±ä
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
		struct player_info_00			//×Ô¼º»á¾­³£¸Ä±äµÄÄÚÈİ
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
			int rage;
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
			int rage;
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
			unsigned char  dir;		//256µÄ¶¨µãÊı±íÊ¾¶ÈÊı
			unsigned char  cls;		//256µÄ¶¨µãÊı±íÊ¾¶ÈÊı
			unsigned char  level;		//ĞŞÕæ¼¶±ğ
			unsigned char  level2;		//ĞŞÕæ¼¶±ğ
			char  pk_level;			//pk level
			char cultivation;		//ÏÉÄ§·ğÔì»¯
			int   reputation;		//ÍşÍû
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
				int type;		//ÎïÆ·ÀàĞÍ  Èç¹ûÊÇ0 ±íÊ¾Ã»ÓĞÄÚÈİÁË
			}empty_item;
			struct 
			{
				int type;		//ÎïÆ·ÀàĞÍ
				int count;		//Ê£Óà¶àÉÙ¸ö ¸ºÊı±íÊ¾ÊÇÂò
				size_t price;		//µ¥¼Û
				int expire_date;	//¹ıÆÚÊ±¼ä
				int state;		//ÌØÊâ×´Ì¬
				unsigned short content_length;
				char content[];
			}item;

			struct 
			{
				int type;		//ÎïÆ·ÀàĞÍ
				int count;		//Ê£Óà¶àÉÙ¸ö ¸ºÊı±íÊ¾ÊÇÂò
				size_t price;		//µ¥¼Û
			}order_item;
		};

		struct player_fac_field
		{
			int index;
			int tid;
			int level;
			int status;
		};

		struct fac_base_prop
		{
			int grass;
			int mine;
			int monster_food;
			int monster_core;
			int cash;
			int task_id;
			int task_count;
			int task_need;
		};

		struct fac_base_auc_item
		{
			enum
			{
				ST_NILL = 0, //ÎŞĞ§×´Ì¬
				ST_BIDDING,  //ÅÄÂô½øĞĞ×´Ì¬
				ST_END,	     //ÅÄÂô½áÊø×´Ì¬
			};
			int idx;
			int itemid;
			int winner;
			int cost;
			int name_len;
			char playername[20];
			int status;
			int end_time;
		};

		struct fac_base_auc_history
		{
			int event_type;	
			int name_len;
			char playername[20];
			int cost;
			int itemid;
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
		OBJECT_NOTIFY_POS,		//Õâ¸öÃüÁîÊ±ÔİÊ±µÄ£¿
		
		OBJECT_MOVE,				//15
		NPC_ENTER_WORLD,
		PLAYER_ENTER_WORLD,
		MATTER_ENTER_WORLD,
		PLAYER_LEAVE_WORLD,
		
		NPC_DEAD,				//20
		OBJECT_DISAPPEAR,
		OBJECT_START_ATTACK,			
		SELF_STOP_ATTACK,			//
		SELF_ATTACK_RESULT,			//×÷·Ï
		
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
		SET_STATUS_POINT_NULL,			//ÒÑ¾­×÷·Ï
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
		EQUIPMENT_DATA,			//ÓÃ»§µÄ×°±¸Êı¾İ£¬Ó°Ïì±íÏÖ
		EQUIPMENT_INFO_CHANGED,	
		EQUIPMENT_DAMAGED,		//×°±¸Ëğ»µ
		TEAM_MEMBER_PICKUP,		//¶ÓÓÑ¼ñÆğ×°±¸

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
		OBJECT_ATTACK_ONCE,			//ÒÑ¾­×÷·Ï £¬ÍêÈ«Ã»ÓĞÓÃ
		SELF_START_ATTACK,			//ÒÑ¾­×÷·Ï
	
		OBJECT_CAST_SKILL,			//85
		SKILL_INTERRUPTED,
		SELF_SKILL_INTERRUPTED,
		SKILL_PERFORM,
		OBJECT_BE_ATTACKED,			//ÒÑ¾­×÷·Ï £¬ÍêÈ«Ã»ÓĞÓÃ

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
		DOUBLE_EXP_TIME,			//×÷·Ï
		AVAILABLE_DOUBLE_EXP_TIME,		//×÷·Ï
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
		OBJECT_NOTIFY_RAID_POS,		//liuyue ·ÏÆúÁË´ËĞ­Òé
		PLAYER_RAID_COUNTER,
		OBJECT_BE_MOVED,
		PLAYER_PULLING,

		PLAYER_BE_PULLED,				//400
		LOTTERY3_BONUS,
		RAID_INFO,
		TRIGGER_SKILL_TIME,
		PLAYER_STOP_SKILL,

		MALL_ITEM_PRICE,				//405
		PLAYER_SINGLE_RAID_COUNTER,
		PLAYER_VIP_AWARD_INFO,
		VIP_AWARD_CHANGE,
		ONLINE_AWARD_INFO,

		ONLINE_AWARD_PERMIT,			//410
		ADDON_SKILL_PERMANENT,
		ADDON_SKILL_PERMANENT_DATA,
		ZX_FIVE_ANNI_DATA,
		TREASURE_INFO,

		TREASURE_REGION_UPGRADE,		//415
		TREASURE_REGION_UNLOCK,	
		TREASURE_REGION_DIG,
		INC_TREASURE_DIG_COUNT,
		RAID_GLOBAL_VAR,				//¸±±¾È«¾Ö±äÁ¿

		RANDOM_TOWER_MONSTER,			//420 Í¨Öª¿Í»§¶Ëµ¯³öÅÀËş¸±±¾½çÃæ
		RANDOM_TOWER_MONSTER_RESULT,		//·şÎñÆ÷Í¨ÖªÅÀËş¸±±¾Ëæ»ú¹ÖÎï½á¹û
		PLAYER_TOWER_DATA,			//Íæ¼ÒÅÀËş¸±±¾Ïà¹ØµÄÊı¾İ
		PLAYER_TOWER_MONSTER_LIST,		//Íæ¼ÒÅÀËş¸±±¾¹ÖÎïÍ¼¼øÁĞ±í
		PLAYER_TOWER_GIVE_SKILLS,		//ÅÀËş¸±±¾¼¼ÄÜÊÖÅÆ

		SET_TOWER_GIVE_SKILL,			//425 µ¥¸öÊÖÅÆÊ¹ÓÃºó·¢ËÍÍ¨Öª¿Í»§¶Ë¸Ä±ä×´Ì¬
		ADD_TOWER_MONSTER,			//»ñµÃÅÀËş¸±±¾Ò»¸ö¹ÖÎïÍ¼¼ø
		SET_TOWER_REWARD_STATE,			//ÅÀËş¸±±¾½±ÀøÁìÈ¡³É¹¦ºó·¢ËÍ¸ø¿Í»§¶Ë×´Ì¬Ğ­Òé
		LITTLEPET_INFO,				//ÖïĞ¡ÏÉĞÅÏ¢
		RUNE_INFO,				//Ôª»ê×°±¸Î»¼¤»î

		RUNE_REFINE_RESULT,			//430	Ôª»êÏ´Á·½á¹û		
		TOWER_LEVEL_PASS,			//Í¨¹ıÅÀËş¸±±¾µÄÒ»¹Ø
		FILL_PLATFORM_MASK,			//²¹ÌîµÄÆ½Ì¨ĞÅÏ¢ mask
		PK_BET_DATA,				//PK¾º²ÂÏà¹ØÊı¾İ
		PUT_ITEM,				//½«ÎïÆ··ÅÈë°ü¹üÖ¸¶¨Î»ÖÃ

		OBJECT_START_SPECIAL_MOVE,		//435	Íæ¼Ò¿ªÊ¼ÌØÊâÒÆ¶¯
		OBJECT_SPECIAL_MOVE,			//Íæ¼ÒÌØÊâÒÆ¶¯
		OBJECT_STOP_SPECIAL_MOVE,		//Íæ¼ÒÍ£Ö¹ÌØÊâÒÆ¶¯
		PLAYER_CHANGE_NAME,
		ENTER_COLLISION_RAID,

		COLLISION_RAID_SCORE,				//440
		REPURCHASE_INV_DATA,				//ÎïÆ·»Ø¹º°ü¹üĞÅÏ¢
		COLLISION_SPECIAL_STATE,		
		EXCHANGE_MOUNT_WING_ITEM,			//×øÆï·É½£°ü¹üÀï½»»»ÎïÆ·
		EXCHANGE_INVENTORY_MOUNTWING_ITEM,	//ÆÕÍ¨°ü¹ü <-> ×øÆï·É½£°ü¹ü

		EXCHANGE_EQUIPMENT_MOUNTWING_ITEM,	//445 //×°±¸À¸ <-> ×øÆï·É½£°ü¹ü 
		PLAYER_CHANGE_MOUNTWING_INV_SIZE,
		PLAYER_DARKLIGHT,					//ĞùÔ¯¹â°µ¶È
		PLAYER_DARKLIGHT_STATE,				//ĞùÔ¯¹â°µ×´Ì¬
		PLAYER_DARKLIGHT_SPIRIT,			//ĞùÔ¯Ö°ÒµµÄÁé
		
		MIRROR_IMAGE_CNT,					//450, ¾µÏñÊıÁ¿
		CHANGE_FACTION_NAME,
		PLAYER_WING_CHANGE_COLOR,		//·É½£¸Ä±äÑÕÉ«
		UI_TRANSFER_OPENED_NOTIFY,		//UI´«ËÍ¿ªÆôµÄ´«ËÍµãÍ¨¸æ
		NOTIFY_ASTROLOGY_ENERGY,

		ASTROLOGY_UPGRADE_RESULT,		//455 ĞÇ×ùÉı¼¶½á¹û
		ASTROLOGY_DESTROY_RESULT,		//ĞÇ×ù·ÛËé½á¹û
		TALISMAN_REFINESKILL,			//Í¨Öª¿Í»§¶Ë·¨±¦¼¼ÄÜÏ´Á·½á¹û
		COLLISION_RAID_RESULT,			//Àë¿ªÅö×²¸±±¾½á¹û
		COLLISION_RAID_INFO,			//Åö×²Õ½³¡ĞÅÏ¢

		DIR_VISIBLE_STATE,				//460, ÉèÖÃ³¯ÏòµÄ×´Ì¬Ğ§¹û
		PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL,	//Íæ¼ÒÃÖ²¹Î´Âú¼¶·ÉÉıÊıÖµ
		COLLISION_PLAYER_POS,
		LIVENESS_NOTIFY,				//Í¨¸æ»îÔ¾¶ÈĞÅÏ¢
		ENTER_KINGDOM,

		LEAVE_KINGDOM,		//465
		RAID_LEVEL_START,	//¹Ø¿¨¿ªÊ¼
		RAID_LEVEL_END,		//¹Ø¿¨½áÊø
		RAID_LEVEL_RESULT,	//¹Ø¿¨Í¨¹ı½á¹û
		RAID_LEVEL_INFO,	//¹Ø¿¨Í³¼ÆµÄ¶¯Ì¬ĞÅÏ¢

		KINGDOM_HALF_END,	//470 ¹úÍõÕ½°ë³¡½áÊø
		KINGDOM_END_RESULT,	//¹úÍõÕ½½áÊø½á¹û
		KINGDOM_MAFIA_INFO,	//¹úÍõÕ½°ïÅÉ»ı·Ö,É±µĞÊı£¬ËÀÍöÊı
		SALE_PROMOTION_INFO,	//Ì¨ÀúÎïÆ·Ç°ÌáÌõ¼şÍ¨¸æ(ÈÎÎñ¼°³É¾Í)
		EVENT_START,		//ÊÂ¼ş¿ªÊ¼

		EVENT_STOP,		//475 ÊÂ¼ş½áÊø
		EVENT_INFO,		//ÊÂ¼şĞÅÏ¢£¬Æô¶¯µÄÊ±ºò·¢ËÍ
		CASH_GIFT_IDS,		//ÉÌ³ÇÂòÔù»ı·ÖID
		NOTIFY_CASH_GIFT,	//Í¨ÖªÁìÈ¡Àñ°ü
		PLAYER_PROPADD,	//ÎïÆ·Ôö¼ÓµÄÍæ¼ÒÊôĞÔ
		
		PLAYER_GET_PROPADD_ITEM,	//480, Íæ¼Ò»ñµÃÊôĞÔÔö¼ÓÎïÆ·
		PLAYER_EXTRA_EQUIP_EFFECT,	//Íæ¼Ò¶îÍâÎäÆ÷¹âĞ§
		KINGDOM_KEY_NPC_INFO,
		NOTIFY_KINGDOM_INFO,
		CSFLOW_BATTLE_PLAYER_INFO,	//Õ½³¡

		PLAYER_BATH_INVITE,	//485  ÑûÇë´êÔè
		PLAYER_BATH_INVITE_REPLY,	//ÑûÇë´êÔè»Ø¸´
		PLAYER_BATH_START,		//¿ªÊ¼´êÔè
		PLAYER_BATH_STOP,		//½áÊø´êÔè
		ENTER_BATH_WORLD,		//½øÈë´êÔèµØÍ¼

		LEAVE_BATH_WORLD,	//490	Àë¿ª´êÔèµØÍ¼
		BATH_GAIN_ITEM,		//´êÔè»ñµÃÎïÆ·
		FLOW_BATTLE_INFO,
		KINGDOM_GATHER_MINE_START,
		FUWEN_COMPOSE_INFO,


		FUWEN_COMPOSE_END,	//495 ·ûÎÄËéÆ¬ºÏ³É½áÊø
		FUWEN_INSTALL_RESULT,
		FUWEN_UNINSTALL_RESULT,
		MULTI_EXP_MODE,
		EXCHANGE_ITEM_LIST_INFO, //·şÎñÆ÷¶Ë½»»»ÕûÀí°ü¹ü

		WEB_ORDER_INFO,		//500 web¶©µ¥ĞÅÏ¢
		CS_BATTLE_OFF_LINE_INFO,
		CONTROL_TRAP_INFO,
		NPC_INVISIBLE,
		PUPPET_FORM,

		TELEPORT_SKILL_INFO,	//505
		MOB_ACTIVE_READY_START, //»¥¶¯¿ªÊ¼
		MOB_ACTIVE_RUNNING_START, //»¥¶¯¹ı³Ì¿ªÊ¼
		MOB_ACTIVE_END_START, //»¥¶¯½áÊø¿ªÊ¼
		MOB_ACTIVE_END_FINISH, //»¥¶¯½áÊøÍê³É
		
		MOB_ACTIVE_CANCEL, //»¥¶¯È¡Ïû, 510
		FASHION_COLORANT_PRODUCE,  // Youshuang add
		FASHION_COLORANT_USE,   // Youshuang add
		BLOW_OFF,		// »÷·É
		PLAYER_QILIN_INVITE,	//  ÑûÇë´êÔè

		PLAYER_QILIN_INVITE_REPLY,	//515 ÑûÇë´êÔè»Ø¸´
		PLAYER_QILIN_START,		//¿ªÊ¼´êÔè
		PLAYER_QILIN_STOP,		//½áÊø´êÔè
		PLAYER_QILIN_DISCONNECT,
		PLAYER_QILIN_RECONNECT,

		PHASE_INFO,			//520
		PHASE_INFO_CHANGE,		
		GET_ACHIEVEMENT_AWARD,  // Youshuang add
		FAC_BUILDING_ADD,
		FAC_BUILDING_UPGRADE,

		FAC_BUILDING_REMOVE,	//525
		FAC_BUILDING_COMPLETE,
		PLAYER_ACTIVE_EMOTE_INVITE,
		PLAYER_ACTIVE_EMOTE_INVITE_REPLY,
		PLAYER_ACTIVE_EMOTE_START,

		PLAYER_ACTIVE_EMOTE_STOP,  // 530
		PLAYER_FAC_BASE_INFO,
		RAID_BOSS_BEEN_KILLED,	// Youshuang add
		FAC_BASE_PROP_CHANGE,
		BE_TAUNTED2,

		COMBINE_MINE_STATE_CHANGE,  // 535, Youshuang add
		GET_FACBASE_MALL_INFO, // Youshuang add
		GET_FACTION_COUPON,	// Youshuang add
		GET_FACBASE_CASH_ITEMS_INFO, // Youshuang add
		PLAYER_CHANGE_PUPPET_FORM,

		STEP_RAID_SCORE_INFO, //540
		NOTIFY_BLOODPOOL_STATUS,
		PLAYER_FACBASE_AUCTION,	//·µ»Ø¸ø¿Í»§¶ËµÄ»ùµØÅÄÂôÁĞ±í
		FACBASE_AUCTION_UPDATE, //»ùµØÅÄÂôĞÅÏ¢·¢Éú±ä»¯ »áÍ¨¹ıdeliveryd¸øËùÓĞ°ïÖÚ·¢ËÍ
		FAC_COUPON_RETURN,	//¸üĞÂ»ùµØÅÄÂôÍË»ØµÄ½ğÈ¯ÊıÁ¿

		OBJECT_TRY_CHARGE,	//545	
		OBJECT_BE_CHARGE_TO,
		PLAYER_START_TRAVEL_AROUND,
		PLAYER_STOP_TRAVEL_AROUND,
		PLAYER_GET_SUMMON_PETPROP,

		GET_FACBASE_CASH_ITEM_SUCCESS,	// 550, Youshuang add
		FACBASE_AUCTION_ADD_HISTORY,	//Ôö¼ÓÅÄÂô¼ÍÂ¼
		FACBASE_MSG_UPDATE,		//¸üĞÂ»ùµØÁôÑÔÏûÏ¢
		CS6V6_CHEAT_INFO,
		SEND_TALISMAN_ENCHANT_ADDON,	// 554, Youshuang add

	 	HIDE_AND_SEEK_RAID_INFO, //555	
		HIDE_AND_SEEK_ROUND_INFO,
		HIDE_AND_SEEK_PLAYERS_INFO,
		HIDE_AND_SEEK_SKILL_INFO,
		HIDE_AND_SEEK_BLOOD_NUM,

		HIDE_AND_SEEK_ROLE, //560
		HIDE_AND_SEEK_ENTER_RAID,
		HIDE_AND_SEEK_LEAVE_RAID,
		HIDE_AND_SEEK_HIDER_TAUNTED,
		MOVE_MAX_POCKET_ITEM_TO_INVENTORY,

		GET_NEWYEAR_AWARD_INFO, // 565, Youshuang add
		CAPTURE_RAID_PLAYER_INFO,
		CAPTURE_BROADCAST_SCORE,
		CAPTURE_SUBMIT_FLAG,
		CAPTURE_RESET_FLAG,
		
		CAPTURE_GATHER_FLAG, //570
		CAPTURE_MONSTER_FLAG,
		PLAYER_USE_TITLE,  // 572, Youshuang add
		CAPTURE_BROADCAST_TIP,
		CAPTURE_BROADCAST_FLAG_MOVE,

	}; //S2C


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
		ERR_VIPAWARD_INVALID_REQUEST,
		ERR_ONLINEAWARD_INVALID_REQUEST,
		ERR_CONSIGN_ROLE_REDNAME,

		//190
		ERR_CONSIGN_ROLE_LOGOUT,
		ERR_CONSIGN_IN_COMBAT,
		ERR_TOWERWARD_INVALID_REQUEST,
		ERR_PET_CAN_NOT_BE_SUMMONED,
		ERR_PET_CAN_NOT_BE_COMBINED,

		//195
		ERR_PET_CAN_NOT_BE_RECALLED,
		ERR_PET_CAN_NOT_BE_UNCOMBINED,
		ERR_CIRCLE_CANCEL_TEAM_CHANGE,
		ERR_DBEXP_TIME_LIMIT,
		ERR_HAS_SUMMON_CAN_NOT_CONSIGNROLE,

		//200
		ERR_TALISMAN_HOLYLEVELUP,			//·¨±¦·ÉÉıÊ§°Ü
		ERR_TALISMAN_EMBEDSKILL,			//·¨±¦¼¼ÄÜÏâÇ¶Ê§°Ü
		ERR_TALISMAN_SKILLREFINE,			//·¨±¦ÈÚºÏÊ§°Ü
		ERR_TALISMAN_SKILLREFINERESULT,
		ERR_LACK_OF_REPUTATION,

		//205
		ERR_FAILED_MERGE_POTION,			//Ò©Æ·¹à×¢Ê§°Ü
		ERR_FAILED_DELIVERYGIFTBOX,			//¶à´Î¿ªÆôÀñ°üÁì½±Ê§°Ü
		ERR_FAILED_GEN_PROPADDITEM,		//ÊôĞÔµ¤Éú³ÉÊ§°Ü
		ERR_FAILED_GEN_PROPADDITEM_MATERIAL_NOT_ENOUGH,		//ÊôĞÔµ¤Éú³ÉÊ§°Ü²ÄÁÏ²»×ã
		ERR_FAILED_REBUILD_PROPADDITEM,	//ÖØÁ¶ÊôĞÔµ¤Ê§°Ü

		//210
		ERR_FAILED_REBUILD_PROPADDITEM_MATERIAL_NOT_ENOUGH,	//ÖØÁ¶ÊôĞÔµ¤Ê§°Ü, ²ÄÁÏ²»×ã
		ERR_RESURRECT_POS_NOT_EXIST,	//¸´»îµã²»´æÔÚ£¬ÎŞ·¨¸´»î
		ERR_BATH_INVALID_REQUEST,	//ÎŞĞ§µÄ´êÔèÑûÇë
		ERR_BATH_INVITE_FULL,		//ÑûÇë¶ÓÁĞÒÑÂú
		ERR_BATH_ALREADY_INVITE,	//ÒÑ¾­ÑûÇëÁË¸ÃÍæ¼Ò

		//215
		ERR_BATH_MAX_LIMIT,		//³¬¹ıÔèÌÃ×î¶àÑûÇë´ÎÊı
		ERR_BATH_NOT_OPEN,		//ÔèÌÃÎ´¿ª·Å£¬²»ÄÜ½øÈë
		ERR_BATH_CANNOT_IN,		//µ±Ç°µØÍ¼²»ÄÜ½øÈëÔèÌÃ
		ERR_WORLD_CANNOT_FLY_BATH_POS,  //¸ÃµØÍ¼²»ÄÜ´«ËÍµ½ÔèÌÃ½øÈëµã
		ERR_MULTI_EXP_TIME_LIMIT,	//¶à±¶¾­Ñé´ïµ½ÉÏÏŞ

		//220
		ERR_KING_CALL_GUARD_IS_COOLING, //Ç×ÎÀÕÙ»½ÀäÈ´
		ERR_CHANGE_INV_TRASHBOX_SIZE,	//¸Ä±ä°ü¹üºÍ²Ö¿â´óĞ¡´íÎó
		ERR_CHANGE_INV_TRASHBOX_SIZE_NOT_ENOUGH_ITEM,	//ÎïÆ·²»×ã
		ERR_BATH_IS_COOLDOWN,		//Ï´Ôè²Ù×÷ÕıÔÚÀäÈ´
		ERR_KING_CALL_GUARD_NOT_ALLOWED, //µ±Ç°×´Ì¬ÏÂ²»ÔÊĞí·¢ÆğÇ×ÎÀ´«ËÍ

		//225
		ERR_KING_CALL_GUARD_WORLD_NOT_ALLOWED, //¸ÃµØÍ¼²»ÔÊĞí·¢ÆğÇ×ÎÀ´«ËÍ
		ERR_FORBID_SKILL,		//¸Ã¼¼ÄÜÔİÊ±½ûÖ¹Ê¹ÓÃ
		ERR_FORBID_ITEM,		//¸ÃÎïÆ·ÔİÊ±½ûÖ¹Ê¹ÓÃ
		ERR_FORBID_SERVICE,		//¸Ã·şÎñÔİÊ±Í£Ö¹Ê¹ÓÃ
		ERR_FORBID_PROTOCOL,		//¸Ã¹¦ÄÜÔİÊ±Í£Ö¹Ê¹ÓÃ

		//230
		ERR_INVENTORY_HAS_SORT,  //±³°üÒÑ¾­ÓĞĞò
		ERR_TRASHBOX_HAS_SORT,  //²Ö¿âÒÑ¾­ÓĞĞò
		ERR_CROSS_SEVER_MAP_ID,
		ERR_BATTLE_READY,
		ERR_SUMMON_TELEPORT1_IS_COOLING,
		
		//235
		ERR_MOB_ACTIVE_INVALID,
		ERR_MOB_ACTIVE_LOCKED,
		ERR_MOB_ACTIVE_POINT_MAX,
		ERR_MOB_ACTIVE_HAS_INVALID_TOOL,
		ERR_MOB_ACTIVE_HAS_INVALID_TASK,

		//240
		ERR_MOB_ACTIVE_TIMES_MAX,
		ERR_MOB_ACTIVE_FINISH_GET_ITEM_FAILED,
		ERR_QILIN_INVALID_REQUEST,
		ERR_QILIN_INVITE_FULL,
		ERR_QILIN_ALREADY_INVITE,

		//245
		ERR_FBASE_LOGIC,	//°ïÅÉ»ùµØÂß¼­´íÎó
		ERR_FBASE_BUILD_REPEAT, //ÒÑ¾­ÓĞ½¨ÖşÔÚ½¨ÉèÖĞ ²»ÄÜ½¨ÉèĞÂ½¨Öş
		ERR_FBASE_FIELD_INDEX,  //µØ¿é´íÎó
		ERR_FBASE_FIELD_REPEAT, //µØ¿éÖØ¸´½¨Öş
		ERR_FBASE_BUILDING_UNAVAIL, //´ËµØ¿é²»ÄÜ½¨Éè´Ë½¨Öş

		//250
		ERR_FBASE_BUILDING_TID, //·Ç·¨½¨Öş
		ERR_FBASE_BUILDING_COND, //½¨ÉèÌõ¼ş²»Âú×ã
		ERR_FBASE_PRIVILEGE, //Íæ¼ÒÎŞÈ¨ÏŞ²Ù×÷»ùµØ
		ERR_CANNOT_SUMMON_REBORN_PET,	//Ö»ÓĞ·ÉÉıÍæ¼ÒÄÜ¹»ÕÙ»½·ÉÉı³èÎï
		ERR_PET_SUMMON_COOLDOWN,	//·ÉÉı³èÎïÔÚÕ½¶·×´Ì¬ÏÂÁ½´ÎÕÙ»½µÄÀäÈ´Ê±¼äÎª30s

		//255
		ERR_INVALID_ACTIVE_EMOTE_INVITE,
		ERR_CANNOT_ACTIVE_EMOTE,
		ERR_FBASE_UPGRADE_LEV, //Éı¼¶½¨Öş ½¨ÖşµÈ¼¶²»Æ¥Åä
		ERR_FBASE_UPGRADE_FULL, //ÒÑ´ïÂú¼¶ ²»ÄÜÔÙÉı¼¶
		ERR_FBASE_REMOVE_INDEX, //¸ÃµØ¿éÃ»ÓĞ½¨Öş ²»ÄÜ²ğ³ı

		//260
		ERR_FBASE_UPGRADE_INDEX, //¸ÃµØ¿éÃ»ÓĞ½¨Öş ²»ÄÜÉı¼¶
		ERR_FBASE_UPGRADE_TID, //½¨Öş tid ²»Æ¥Åä
		ERR_FBASE_REMOVE_WHILE_INBUILDING, //½¨ÉèÖĞµÄ½¨Öş²»ÄÜ²ğ³ı
		ERR_FBASE_NOT_ENOUGH_CASH,	// Youshuang add
		ERR_FBASE_CASH_ITEM_IN_COOLDOWN,  // Youshuang add

		//265
		ERR_FBASE_AUC_NOT_EXIST, //ÅÄÂôÆ·²»´æÔÚ
		ERR_FBASE_AUC_NOT_WINNER, //·ÇÅÄÂô»ñÊ¤Õß ²»ÄÜÁìÈ¡
		ERR_FBASE_STATUS,	//»ùµØ×´Ì¬´íÎó
		ERR_FBASE_AUC_SIZE,	//»ùµØÅÄÂôÌõÊıÒÑ´ïÉÏÏŞ
		ERR_CS6V6_CANNOT_GET_AWARD, //µ±Ç°²»ÄÜÁìÈ¡¸Ã½±Àø

		// 270
		ERR_POCKET_HAS_SORT,  // Youshuang add
		ERR_MINE_CANNOT_GATHER, //²»ÄÜ²É¼¯±¾ÅÉµÄ¿ó
		ERR_HAS_NOT_CAPTURE_RAID_FLAG, //°ü¹üÀïÃ»ÓĞ¾üÆì
		ERR_CANNOT_SUBMIT_CAPTURE_RAID_FLAG, //¶Ô·½ÊÖÉÏÒ²ÓĞ¾üÆì²»ÄÜ½»
	};

	enum
	{
	//0
		GOP_INSTALL,
		GOP_UNINSTALL,
		GOP_BIND,
		GOP_BIND_DESTORY,
		GOP_BIND_DESTORY_RESTORE,

	//5
		GOP_MOUNT,
		GOP_TALISMAN_REFINE,
		GOP_TALISMAN_LVLUP,
		GOP_TALISMAN_RESET,
		GOP_TALISMAN_COMBINE,

	//10
		GOP_TALISMAN_ENCHANT,
		GOP_FLY,
		GOP_LOCK,
		GOP_UNLOCK,
		GOP_SPIRIT_OPERATION,

	//15
		GOP_GIFT,
		GOP_PETEQUIP_REFINE,
		GOP_CHANGE_STYLE,
		GOP_MAGIC_REFINE,
		GOP_MAGIC_RESTORE,

	//20
		GOP_TRANSFORM,
		GOP_TELESTATION,
		GOP_REPAIR_DAMAGE_ITEM,
		GOP_UPGRADE_EQUIPMENT,
		GOP_CROSS_SERVER,

	//25
		GOP_TOWNSCROLL,
		GOP_GEM_OPERATION,
		GOP_RUNE_IDENTIFY,	//Ôª»ê¼ø¶¨
		GOP_RUNE_COMBINE,	//Ôª»êÑªÁ¶
		GOP_RUNE_REFINE,	//Ôª»êÏ´Á·

	//30	
		GOP_RUNE_RESET,		//Ôª»ê¹éÔª
		GOP_RUNE_DECOMPOSE,	//Ôª»ê²ğ½â
		GOP_RUNE_OPEN_SLOT,	//Ôª»ê´ò¿×
		GOP_RUNE_CHANGE_SLOT,	//Ôª»ê¸Ä¿×
		GOP_RUNE_ERASE_SLOT,	//Ôª»ê²Á¿×

	//35
		GOP_RUNE_INSTALL_SLOT,	//Ôª»êÏâÇ¶
		GOP_TALISMAN_HOLYLEVELUP,	//·¨±¦·ÉÉı
		GOP_TALISMAN_EMBEDSKILL,	//ÏâÇ¶¼¼ÄÜ
		GOP_TALISMAN_SKILLREFINE,	//¼¼ÄÜÏ´Á¶
		GOP_UPGRADE_EQUIPMENT2,		//Éı¼¶³É·âÉñ×°±¸

	//40
		GOP_EQUIPMENT_SLOT,	//×°±¸´ò¿×
		GOP_EQUIPMENT_INSTALL_ASTROLOGY, //ÏâÇ¶ĞÇ×ù
		GOP_EQUIPMENT_UNINSTALL_ASTROLOGY, //Õª³ıĞÇ×ù
		GOP_ASTROLOGY_IDENTIFY,	//ĞÇ×ù¼ø¶¨
		GOP_TALISMAN_SKILLREFINERESULT,	//¼¼ÄÜÏ´Á¶

	//45
		GOP_BATTLEFLAG,
		GOP_ASTROLOGY_UPGRADE,
		GOP_ASTROLOGY_DESTROY,
		GOP_BATH,
		GOP_FUWEN_COMPOSE,

	//50
		GOP_PET_REBORN,		//³èÎï·ÉÉı
		GOP_ENTER_FAC_BASE,	//½øÈë°ïÅÉ»ùµØ
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

	enum OBJECT_STATE
	{
		STATE_IGNITE,
		STATE_FROZEN,
		STATE_FURY,
		STATE_DARK,
		STATE_CYCLE_SKILL,	//Ğı·çÕ¶
	};

	enum COLLISION_SKILL_TYPE
	{
		COLLISION_SKILL_MAXSPEED,	//¼ÓËÙ
		COLLISION_SKILL_HALFSPEED,	//¼õËÙ
		COLLISION_SKILL_ASHILL,		//²»¶¯ÈçÉ½
		COLLISION_SKILL_NOMOVE,		//¶¨Éí
		COLLISION_SKILL_CHAOS,		//»ìÂÒ
		COLLISION_SKILL_VOID,		//ĞéÎŞ
	};

	enum GEM_NOTIFY_MSG
	{
		//¿Í»§¶Ëµ¯¿òµÄÏûÏ¢
		GEM_UPGRADE_SUCC = 0,
		GEM_UPGRADE_FAIL,
		GEM_SMELT_FAIL_SAME_QUALITY,

		//¿Í»§¶ËÁÄÌì¿òÏÔÊ¾µÄÏûÏ¢
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

		struct player_enter_slice	//player½øÈë¿É¼ûÇøÓò
		{	
			single_data_header header;
			player_info_1 data;
		};

		struct npc_enter_slice
		{
			single_data_header header;
			npc_info data;
		};

		struct leave_slice	//playerÀë¿ª¿É¼ûÇøÓò
		{	
			single_data_header header;
			int id;
		};

		struct notify_pos		//player¸üĞÂÎ»ÖÃ
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
			   struct		//¼ÙÉèµÄ´úÂë£¬Êµ¼ÊÉÏÓÉÓÚinfo2ÊÇ±ä³¤µÄ½á¹¹£¬ËùÒÔÎŞ·¨ÕâÑù×éÖ¯
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
			   struct		//¼ÙÉèµÄ´úÂë£¬Êµ¼ÊÉÏÓÉÓÚinfo3ÊÇ±ä³¤µÄ½á¹¹£¬ËùÒÔÎŞ·¨ÕâÑù×éÖ¯
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
			   struct		//¼ÙÉèµÄ´úÂë£¬Êµ¼ÊÉÏÓÉÓÚinfo2/3ÊÇ±ä³¤µÄ½á¹¹£¬ËùÒÔÎŞ·¨ÕâÑù×éÖ¯
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
			unsigned char attack_flag;	//±ê¼Ç¸Ã¹¥»÷ÊÇ·ñÓĞ¹¥»÷ÓÅ»¯·ûºÍ·ÀÓùÓÅ»¯·ûºÍÖØ»÷·¢Éú
			unsigned char attack_stamp;	//¹¥»÷µÄÊ±¼ä´Á
			
		};

		struct object_skill_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int skill_id;
			int damage;
			int dt_damage;
			unsigned char attack_flag;	//±ê¼Ç¸Ã¹¥»÷ÊÇ·ñÓĞ¹¥»÷ÓÅ»¯·ûºÍ·ÀÓùÓÅ»¯·ûºÍÖØ»÷·¢Éú
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
			unsigned char eq_index; //¸ßÎ»´ú±íÕâ´Î¹¥»÷ÊÇ²»ÊÇÓ¦¸Ã±ä³ÈÉ«
			unsigned char attack_flag;	//±ê¼Ç¸Ã¹¥»÷ÊÇ·ñÓĞ¹¥»÷ÓÅ»¯·ûºÍ·ÀÓùÓÅ»¯·ûºÍÖØ»÷·¢Éú
			char speed;			//¹¥»÷ËÙ¶È
		};

		struct be_skill_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	skill_id;
			int 	damage;
			unsigned char eq_index; //¸ßÎ»´ú±íÕâ´Î¹¥»÷ÊÇ²»ÊÇÓ¦¸Ã±ä³ÈÉ«
			unsigned char attack_flag;	//±ê¼Ç¸Ã¹¥»÷ÊÇ·ñÓĞ¹¥»÷ÓÅ»¯·ûºÍ·ÀÓùÓÅ»¯·ûºÍÖØ»÷·¢Éú
			char speed;			//¹¥»÷ËÙ¶È
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
			int time;	//¸´»îĞèÒªµÄÊ±¼ä£¬ È±Ê¡Îª0
		};

		struct player_revival
		{
			single_data_header header;
			int id;
			short type;	//¸´»îµÄÀàĞÍ 0 »Ø³Ç¸´»î£¬1 ¿ªÊ¼¸´»î 2£º¸´»îÍêÈ«Íê³É
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
			unsigned char where;		//ÔÚÄÄ¸ö°ü¹üÀ¸£¬0 ±ê×¼£¬2 ÈÎÎñ£¬1 ×°±¸
			unsigned char index;		//×îºó²¿·Ö·ÅÔÚÄÄ¸öÎ»ÖÃ
			int state;
		};

		struct player_purchase_item
		{
			single_data_header header;
			int trade_id;
			size_t cost;
			unsigned char type;		//±íÊ¾¹Ë¿ÍÂòÈë»¹ÊÇÂô³ö
			unsigned short item_count;
			struct
			{
				int item_id;
				unsigned short count;
				unsigned short inv_index;
				unsigned char  stall_index;
				int state;
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
			int64_t exp;
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

		/**
		 * @brief ÎïÆ·»Ø¹º°ü¹üĞÅÏ¢£¬·¢µÄÊÇdetail_dataÏêÏ¸ĞÅÏ¢
		 */
		struct repurchase_inventory_data
		{
			single_data_header header;
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

		// Youshuang 
		struct send_talisman_enchant_addon
		{
			single_data_header header;
			int id;
			int index;
		};
		
		struct fashion_colorant_produce
		{
			single_data_header header;
			int index;
			int color;
			int quality;
		};
		
		struct fashion_colorant_use
		{
			single_data_header header;
			int where;
			int index;
			int color;
			int quality;
		};

		struct get_achieve_award
		{
			single_data_header header;
			unsigned short achieve_id;
		};
		
		struct raid_boss_been_killed
		{
			single_data_header header;
			unsigned short boss_cnt;
			struct
			{
				unsigned char is_killed;
				unsigned char achievement;
				unsigned char cur_num;
				unsigned char max_num;
			}boss[1];
		};

		struct facbase_mall_item_info
		{
			single_data_header header;
			unsigned short items_cnt;
			int actived_items[1];
		};

		struct combine_mine_state_change
		{
			single_data_header header;
			int matter_id;
			int state;
		};

		struct get_faction_coupon
		{
			single_data_header header;
			int faction_coupon;
		};

		struct get_facbase_cash_items_info
		{
			single_data_header header;
			unsigned short items_cnt;
			struct
			{
				int idx;
				int cooldown;
			}item_cooldown[1];
		};

		struct player_change_puppet_form 
		{
			single_data_header header;
			int player_id;
		};

		struct get_facbase_cash_item_success
		{
			single_data_header header;
			int item_index;
		};

		struct get_newyear_award_info
		{
			single_data_header header;
			int score;
			int cnt;
			char award_level[1];
		};
		
		struct player_use_title
		{
			single_data_header header;
			short title;
		};
		// end
		
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
		//	q_extend_prop prop;		//²»ÏëÒıÓÃÍ·ÎÄ¼ş£¬ËùÒÔ×¢ÊÍµôÁË
		};


		struct player_extprop_base
		{
			single_data_header header;
			int id;
			int max_hp;		//×î´óhp
			int max_mp;		//×î´ómp
			int max_dp;
		};

		struct player_extprop_move
		{
			single_data_header header;
			int id;
			float walk_speed;	//ĞĞ×ßËÙ¶È µ¥Î»  m/s
			float run_speed;	//±¼ÅÜËÙ¶È µ¥Î»  m/s
		};

		struct player_extprop_attack
		{
			single_data_header header;
			int id;
			int attack;		//¹¥»÷ÂÊ attack rate
			int damage_low;		//×îµÍdamage
			int damage_high;	//×î´óÎïÀídamage
			float attack_range;	//¹¥»÷·¶Î§
			int crit_rate;		//ÖÂÃüÒ»»÷¸ÅÂÊ Ç§·ÖÊı
			float crit_damage;	//ÖÂÃüÒ»»÷Ê±Ôì³ÉµÄ¶îÍâÉËº¦

		};

		struct player_extprop_defense
		{
			single_data_header header;
			int id;
			int resistance[6];	//Ä§·¨¿¹ĞÔ
			int defense;		//·ÀÓùÁ¦
			int armor;		//ÉÁ¶ãÂÊ£¨×°¼×µÈ¼¶£©
		};
		
		struct team_leader_invite
		{
			single_data_header header;
			int leader;
			int seq;
			short pickup_flag;
			int familyid;
			int mafiaid;
			int level;
			int sectid;
			int referid;
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
		
		//±íÃ÷×Ô¼ºÀë¿ªÁË¶ÓÎé
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
				char reborn_cnt;
				int rage;
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
			int type;	//·şÎñµÄÀàĞÍ
			size_t length;
			char data[];
		};

		struct item_to_money
		{
			single_data_header header;
			unsigned short index;		//ÔÚ°ü¹üÀ¸ÀïµÄË÷ÒıºÅÂë
			int type;			//ÎïÆ·µÄÀàĞÍ
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
			int state;
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
			int skill;
			unsigned short time;
			unsigned char level;
			unsigned char attack_stamp;
			unsigned char state;
			short cast_speed_rate;
			A3DVECTOR skillpos;
			char spirit_index; //0-2, ÁéµÄindex
			int target_cnt;
			int targets[];
		};

		struct skill_interrupted
		{
			single_data_header header;
			int caster;
			char spirit_index; //0-2, ÁéµÄindex
		};

		struct self_skill_interrupted
		{
			single_data_header header;
			unsigned char reason;
			char spirit_index; //0-2, ÁéµÄindex
		};

		struct skill_perform
		{
			single_data_header header;
			char spirit_index; //0-2, ÁéµÄindex
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
			unsigned char where;		//ÔÚÄÄ¸ö°ü¹üÀ¸£¬0 ±ê×¼£¬2 ÈÎÎñ£¬1 ×°±¸
			unsigned char index;		//×îºó²¿·Ö·ÅÔÚÄÄ¸öÎ»ÖÃ
			int state;
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
			unsigned char where;		//ÔÚÄÄ¸ö°ü¹üÀ¸£¬0 ±ê×¼£¬2 ÈÎÎñ£¬1 ×°±¸
			unsigned char index;		//×îºó²¿·Ö·ÅÔÚÄÄ¸öÎ»ÖÃ
			int state;
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
			char spirit_index; //0-2, ÁéµÄindex
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
				char overlay_cnt;
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
				char overlay_cnt;
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
			int level;
			int sectid;
			int referid;
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
			unsigned char where;		//ÔÚÄÄ¸ö°ü¹üÀ¸£¬0 ±ê×¼£¬2 ÈÎÎñ£¬1 ×°±¸
			unsigned char index;		//×îºó²¿·Ö·ÅÔÚÄÄ¸öÎ»ÖÃ
			int state;                  //item×´Ì¬£º°ó¶¨¡¢×°±¸ºó°ó¶¨¡¢Ëø¶¨
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
			int64_t exp;
			int sp;
			int dt_exp;	//·âÉñ¾­Ñé
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
			int item_id; //²»Ê¹ÓÃµÀ¾ß°ÚÌ¯Ê±Îª-1
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];		//×î´ó28
		};

		struct self_open_market
		{
			single_data_header header;
			int index;    //²»Ê¹ÓÃµÀ¾ß°ÚÌ¯Ê±Îª-1
			int item_id;  //²»Ê¹ÓÃµÀ¾ß°ÚÌ¯Ê±Îª-1
			unsigned short count;
			struct 
			{
				int type;		//ÎïÆ·ÀàĞÍ
				unsigned short index;	//Èç¹ûÊÇ0xFFFF£¬±íÊ¾ÊÇ¹ºÂò
				unsigned short count;	//Âô¶àÉÙ¸ö
				size_t price;		//µ¥¼Û
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
			char name[];	//×î´ó28×Ö½Ú
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
			int skill;
			unsigned char level;
			A3DVECTOR skillpos;
			char spirit_index; //0-2, ÁéµÄindex
			int target_cnt;
			int targets[];
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
			int index;    //²»Ê¹ÓÃ°ÚÌ¯µÀ¾ßÊ±¸ÃÖµÎª-1
			int item_id;  //²»Ê¹ÓÃ°ÚÌ¯µÀ¾ßÊ±¸ÃÖµÎª-1
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
			int timeout;		//Èç¹ûÊÇ-1±íÊ¾È¡Ïû´Ë´ÎÌß³ö
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
			char mount_type; // 0 - Ä¬ÈÏÀàĞÍ   1 - ¶àÈËÆï³ËÀàĞÍ 
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
			unsigned char battle_faction;	// 0 ÖĞÁ¢ 1 ¹¥·½ 2 ÊØ·½ 3 ÅÔ¹Û 4 »ìÕ½
			unsigned char battle_type;
			int battle_id;			// Õ½³¡id
			int end_timestamp;		// ½áÊøÊ±¼ä

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
			int battle_id;			// Õ½³¡id
			int end_timestamp;		// ½áÊøÊ±¼ä
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
			unsigned char result; // 1:¹¥·½»ñÊ¤ 2:ÊØ·½»ñÊ¤
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
			int result; // 0 ³É¹¦ 1 Ê§°Ü É¾³ı²ÄÁÏ 2Ê§°ÜÉ¾³ı²ÄÁÏ É¾³ıÊôĞÔ 3Ê§°Ü ×°±¸±¬µô 4 ²ğĞ¶ÊôĞÔÊ¯ 
				    // 5²ğĞ¶¼¼ÄÜÊ¯
		};

		struct player_start_general_operation
		{
			single_data_header header;
			int player_id;
			int op_id;
			int duration;	//ÒÔ0.05ÃëÎªµ¥Î»µÄtick
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
			int cash_add2;
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
			size_t count;	//²»ÊÇ byte size
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

		struct player_change_mountwing_inv_size
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

		// Ö»ÄÜ·¢¸ø¸½½üµÄÈË
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
		// Õ½³¡¿ªÊ¼
		struct battle_start
		{
			single_data_header header;
		};

		// ¹ã²¥Ê¹ÓÃ
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
		
		// Íæ¼ÒµÄÕ½³¡»ı·Ö
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
			unsigned char page;		//µ±Ç°Ò³ºÅ,0¿ªÍ·
			unsigned char max_page;		//×î´óÒ³ºÅ,0¿ªÍ·
			unsigned char count;		//¸öÊı
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
			unsigned char main_status;	// 0 Õ½¶· 1 ²É¼¯ 2 ÖÆÔì 3 ĞİÏ¢
			unsigned char sub_status;	// Ö»ÓĞmain_statusÎª 1Ê±ÓĞÓÃ ,0 ÖÖÖ² 1 ·¥Ä¾ 2 á÷ÁÔ 3 µöÓã 4 ²É¿ó 5 ¿¼¹Å
		};

		struct player_summon_pet
		{
			single_data_header header;
			unsigned char pet_index;        
			int pet_tid;			//³èÎïµÄÄ£°æid
			int pet_id;			//³èÎïµÄÊµ¼Êid
		};

		struct player_recall_pet
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;			//³èÎïµÄÄ£°æid
			int pet_id;			//³èÎïµÄÊµ¼Êid
		};

		struct player_combine_pet
		{
			single_data_header header;
			unsigned char pet_index;        //³èÎïË÷Òı
			int pet_tid;			//³èÎïµÄÄ£°æid
			unsigned char type;		//0 Í¨Áé,1 Óù±¦
		};

		struct player_uncombine_pet
		{
			single_data_header header;
			unsigned char pet_index;	//³èÎïË÷Òı
			int pet_tid;			//³èÎïµÄÄ£°æid
		};

		struct player_start_pet_op
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int delay;			//ÑÓ³ÙÊ±¼ä,µ¥Î»ÊÇ50msµÄtick
			unsigned char operation;	//²Ù×÷ÀàĞÍ 0:·Å³ö 1:ÕÙ»Ø 2:Í¨Áé 3:½â³ıÍ¨Áé 4:Óù±¦ 5:½â³ıÓù±¦
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
			int cur_exp;			//Ôö¼ÓµÄ¾­Ñé
		};

		struct player_pet_levelup
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int new_level;			//ĞÂ¼¶±ğ
			int cur_exp;			//µ±Ç°µÄ¾­ÑéÖµ 
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
			unsigned char set_flag; //1ÉèÖÃ 0È¡Ïû
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
			unsigned short count;	//×ªÁË¼¸´Î£¬²»ÊÇ×Ö½ÚÊı
			struct
			{
				short level;	//±¾´Î×ªÉúÊ±µÄµÈ¼¶
				short prof;	//±¾´Î×ªÉúÊ±µÄÖ°Òµ
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
			int civilization;	//³èÎïÎÄÃ÷¶È
		};

		struct player_pet_construction
		{
			single_data_header header;
			int construction;	//³èÎï¹¤·»½¨Éè¶È
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
			char type; //0:·É½£, 1:¼¼ÄÜ
		};

		/**
		 * @brief ¸Ã½á¹¹ÓëGMSV::role_pet_prop_addedµÄ½á¹¹Ïà¶ÔÓ¦£¬ºóÕßÓÃÓÚ½ÇÉ«½»Ò×
		 *        Òò´Ëplayer_pet_prop_addedĞŞ¸ÄÊ±×¢Òâ½ÇÉ«¼ÄÊÛÊÇ·ñĞèÒª¸Ä¡£
		 */
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

		struct move_max_pocket_item_to_inventory
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

		/**
		 * @brief ×øÆï·É½£°ü¹üÀï½»»»ÎïÆ·
		 */
		struct exchange_mount_wing_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		/**
		 * @brief ÆÕÍ¨°ü¹ü <-> ×øÆï·É½£°ü¹ü 
		 */
		struct exchange_inventory_mountwing_item
		{
			single_data_header header;
			unsigned char idx_inv;
			unsigned char idx_mw;
		};

		/**
		 * @brief ×°±¸À¸ <-> ×øÆï·É½£°ü¹ü
		 */
		struct exchange_equipment_mountwing_item
		{
			single_data_header header;
			unsigned char idx_equ;
			unsigned char idx_mw;
		};

		struct exchange_hotkey_equipment_fashion
		{
			single_data_header header;
			unsigned char idx_key;
			unsigned char idx_head; //0xff´ú±íÄÇ¸ö²¿Î»²»ĞèÒªÌæ»»
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

		struct rank_change  //Íæ¼Ò¾üÏÎ±ä»¯
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
			int attacker_score; //µ±Ç°ºì·½·ÖÊı
			int defender_score; //µ±Ç°À¶·½·ÖÊı
			int kill_count;
			int death_count;
			int attacker_off_line_score; //µ±Ç°ºì·½ÀëÏß³Í·£·ÖÊı
			int defender_off_line_score; //µ±Ç°À¶·½ÀëÏß³Í·£·ÖÊı
			unsigned char attacker_count; //µ±Ç°ºì·½ÈËÊı
			unsigned char defender_count; //µ±Ç°À¶·½ÈËÊı
			unsigned char min_player_deduct_score; //ÀëÏß³Í·£ÈËÊıÏŞÖÆ
			int end_timestamp;
		};

		struct crossserver_team_battle_info
		{
			struct history_score
			{
				int attacker_score;
				int defender_score;
			};

			single_data_header header;
			unsigned char battle_state; //Õ½³¡µ±Ç°×´Ì¬£¬0±íÊ¾ĞİÏ¢£¬1±íÊ¾Õ½¶·
			int cur_state_end_time;     //µ±Ç°×´Ì¬µÄ½áÊøÊ±¼ä£¬¸ÃÖµÎª¾ø¶ÔÊ±¼ä
			int attacker_score;         //µ±Ç°ºì·½·ÖÊı
			int defender_score;         //µ±Ç°À¶·½·ÖÊı
			int best_killer_id;         //Ã¿Ğ¡¾ÖÀÛ¼Æ»÷É±Êı×î¸ßµÄplayer

			unsigned char history_round_count;  //ÀúÊ·¾ÖÊı 
			history_score score_info[0];
		};

		/**
		 * @brief Õó·¨¿ªÊ¼Æô¶¯£¬Ïë¿Í»§¶Ë¹ã²¥Ò»ÌõÏûÏ¢
		 */
		struct notify_circleofdoom_start
		{
			single_data_header header;
			int  sponsor_id; //Õó·¨·¢ÆğÕßµÄid
			char faction;    //1±íÊ¾°´×é¶ÓÊÕ¼¯£¬2±íÊ¾°´°ïÅÉÊÕ¼¯£¬3±íÊ¾°´¼Ò×åÊÕ¼¯
		};
		
		/**
		 * @brief Õó·¨·¢Æğ³É¹¦ºó£¬¶¨Ê±Ïò¿Í»§¶ËÍ¬²½Õó·¨ÄÚ³ÉÔ±ĞÅÏ¢
		 */
		struct notify_circleofdoom_info
		{
			single_data_header header;
			int skill_id;     //Õó·¨·¢ÆğÕßËùÊ¹ÓÃµÄÕó·¨¼¼ÄÜid
			int sponsor_id;   //Õó·¨·¢ÆğÕßµÄid 
			int member_count; //Õó·¨³ÉÔ±µÄ¸öÊı£¬²»°üº¬Õó·¨·¢ÆğÕß
			int member_id[0];
		};

		/**
		 * @brief Í¨Öª¿Í»§¶ËÄÄ¸öÕó·¨Í£Ö¹
		 */
		struct notify_circleofdoom_stop
		{
			single_data_header header;
			int sponsor_id;  //Õó·¨·¢ÆğÕßµÄid
		};

		/**
		 * @brief ²éÑ¯ÄÄĞ©ÏŞÊ±ÉÌÆ·ÕıÔÚÏúÊÛ 
		 */
		struct mall_item_price
		{
			struct saleitem_list
			{
				int   good_index;     //ÔÚÉÌ³ÇÀïµÄindex
				int   good_id;        //ÎïÆ·id
				int   remaining_time; //ÏŞÊ±ÏúÊÛµÄÊ£ÓàµÄÃëÊı£¬-1´ú±íÃ»ÓĞÊ±ÏŞ
			};

			single_data_header header;
			int good_count;
			saleitem_list list[0];
		};

		/**
		 * @brief Í¨Öª¿Í»§¶ËÓĞÄÄĞ©½±Àø¿ÉÒÔÁìÈ¡
		 */
		struct vip_award_info
		{
			enum{IS_NOT_OBTAINED = 0, IS_OBTAINED = 1,};

			struct award_data
			{
				int		award_id;			//½±Àøid
				char	is_obtained;		//±¾½±ÀøÊÇ·ñÒÑ¾­ÁìÈ¡£º0±íÊ¾Î´ÁìÈ¡, 1±íÊ¾ÒÑ¾­ÁìÈ¡
			};

			single_data_header header;
			int vip_end_time;				//vipÌØÊâÎïÆ·ÁìÈ¡µÄ½ØÖ¹Ê±¼ä
			int recent_total_cash;			//½üÆÚ£¨30Ìì£©µÄÀÛ¼Æ³äÖµÊı
			int award_count;				//½±ÀøÊıÁ¿
			award_data list[0];				//½±ÀøidÁĞ±í
		};

		/**
		 * @brief vip½±ÀøµÈ¼¶·¢Éú±ä»¯Ê±£¬¹ã²¥Í¨Öª¸ÃÇøÓòÄÚµÄÍæ¼Ò
		 */
		struct vip_award_change
		{
			single_data_header header;
			int roleid;
			unsigned char vipaward_level;
			char is_hide;
		};

		/**
		 * @brief Í¨Öª¿Í»§¶ËÍæ¼ÒÉíÉÏÓĞÄÄĞ©ÔÚÏßµ¹¼ÆÊ±½±Àø
		 */
		struct online_award_info
		{
			struct award_data
			{
				int		award_id;			//ÔÚÏßµ¹¼ÆÊ±½±Àøid
				int		index;				//½±Àøµ¹¼ÆÊ±µ½µÚ¼¸ÂÖ
				int		small_giftbag_id;	//±¾ÂÖĞ¡Àñ°üid
				int		remaining_time;		//±¾ÂÖµ¹¼ÆÊ±Ê£Óà¶àÉÙÃë£¬µ¥Î»ÊÇÃë
			};

			single_data_header header;
			char force_flag;				//Ç¿ÖÆ¿Í»§¶Ë¸üĞÂ½±ÀøĞÅÏ¢±êÖ¾£¬·Ç0±íÊ¾Ç¿ÖÆ¸üĞÂ
			int  award_count;				//±¾ÌõÏûÏ¢½±ÀøµÄ¸öÊı
			award_data list[0];
		};

		/**
		 * @brief Í¨Öª¿Í»§¶ËÊÇ·ñÒÑ¾­¿ÉÒÔÁìÈ¡ÔÚÏß½±Àø
		 */
		struct online_award_permit
		{
			single_data_header header;
			int  award_id;
			int  index;
			char permit_code;			//0  ±íÊ¾¿ÉÒÔÁìÈ¡½±Àø, 1±íÊ¾³É¹¦½ø°ü¹ü,¿ÉÒÔ½øĞĞÏÂÒ»ÂÖµÄµ¹¼ÆÊ±
										//-1 ¸ÃÀñ°üÃ»¼¤»î, -2 ¸ÃÀñ°üÒÑ¾­ÁìÈ¡, -3 Ã»ÓĞÕÒµ½¸ÃÀñ°ü
										//-4 ¸ÃÀñ°ü¶ÔÓ¦µÄindexÃ»ÓĞĞ¡Àñ°üĞÅÏ¢, -5 ÎŞĞ§µÄplayer£¬²»ÄÜÁìÈ¡
										//-6 ÎŞĞ§µÄindex£¨index±íÊ¾µ¹¼ÆÊ±µ½ÄÇÒ»ÂÖ£©
										//-7 ±¾ÂÖµÄµ¹¼ÆÊ±»¹Ã»ÓĞ½áÊø
		};
		
		struct achievement
		{
			unsigned short achieve_id;	// ³É¾Íid 

			unsigned short premiss_mask;	// ÒÔÍê³ÉÇ°ÌáÎ»Í¼
			char premiss_count;		// Ç°ÌáÊıÁ¿
			struct _premiss			// Ç°ÌáµÄÊı¾İ½á¹¹
			{
				char premiss_id;	// Ç°Ìáid;
				char data[1];		// Ç°ÌáÊı¾İ
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

			size_t 	map_count;			//³É¾Ímap´óĞ¡
			char 	map_data[1];			//³É¾ÍmapÊı¾İ

			unsigned short  active_count;		//¼¤»îÖĞµÄ³É¾ÍÊıÁ¿
			achievement achieve[1];			//¼¤»îÖĞµÄ³É¾ÍÊı¾İ

			int achieve_point;			//³É¾Íµã
			int current_achieve_point;		//Ê£ÓàµÄ³É¼¨µã

			size_t spec_count;			//ÌØÊâ³É¾ÍÊıÁ¿
			achievement_spec_info info[1];		//ÌØÊâ³É¾ÍÊı¾İ

			// Youshuang add
			size_t award_map_count;
			unsigned char award_map_data[1];
			// end
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
			size_t 	map_count;			//³É¾Ímap´óĞ¡
			char 	map_data[1];			//³É¾ÍmapÊı¾İ
		};
		struct achievement_message
		{
			single_data_header header;
			unsigned short achieve_id;
			int param;
			unsigned char name_len;
			char name[];		
			int finish_time;
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
			char spirit_index; //0-2, ÁéµÄindex
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
			int carrier_id; 		//ÏëÒª³Ë×øµÄ½»Í¨¹¤¾ßid
			A3DVECTOR rpos;			//Ïà¶Ô×ø±êºÍ·½Ïò
			unsigned char rdir;
			unsigned char success;
		};

		struct player_leave_carrier 
		{
			single_data_header header;
			int player_id;
			int carrier_id; 		//ÏëÒªÀë¿ªµÄ½»Í¨¹¤¾ßid
			A3DVECTOR pos;			//¾ø¶Ô×ø±êºÍ·½Ïò
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
			char type;//0, ÏòÄ¿±êË²ÒÆ 1, ÏòÄ¿±ê³å·æ, 2, ÏòÃæÏòÉÁÏÖ 3, µ¯·É  4 ÎüÒı  5 »÷ÍË 6 ±³Ë¤
			int target_id;
			A3DVECTOR destPos;
		};

		//·şÎñÆ÷¶Ë·¢ÆğµÄ¶ÔÈËÉúĞ§µÄË²ÒÆ
		struct object_try_charge
		{
			single_data_header header;
			char type;//0, ÏòÄ¿±êË²ÒÆ 1, ÏòÄ¿±ê³å·æ, 2, ÏòÃæÏòÉÁÏÖ 3, µ¯·É  4 ÎüÒı  5 »÷ÍË 6 ±³Ë¤
			int target_id;
			A3DVECTOR destPos;
		};

		struct object_be_charge_to
		{
			single_data_header header;
			int id;
			char type;//0, ÏòÄ¿±êË²ÒÆ 1, ÏòÄ¿±ê³å·æ, 2, ÏòÃæÏòÉÁÏÖ 3, µ¯·É  4 ÎüÒı  5 »÷ÍË 6 ±³Ë¤
			int target_id;
			A3DVECTOR destPos;
		};
		
		struct enter_territory
		{
			single_data_header header;
			unsigned char battle_faction;	// 0 ÖĞÁ¢ 1 ¹¥·½ 2 ÊØ·½ 3 ÅÔ¹Û 4 »ìÕ½
			unsigned char battle_type;
			int battle_id;			// Õ½³¡id
			int end_timestamp;		// ½áÊøÊ±¼ä

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
			int64_t exp;
			unsigned int award_type;
		};

		struct deity_levelup
		{
			single_data_header header;
			int id;
			short cur_lvl;	//µ±Ç°µÈ¼¶
			char success;	//0 - Ê§°Ü  1-³É¹¦
		};

		struct object_special_state
		{
			single_data_header header;
			int roleid;
			int type;
			char on;	//0- Í£Ö¹ 1-¿ªÊ¼ 
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
			char on;				//0, ½øĞĞËõ·Å£¬ //1£¬»¹Ô­
			int scale_ratio;	 	//Ëõ·Å±ÈÀı, °Ù·ÖÊı
		};

		struct player_move_cycle_area
		{
			single_data_header header;
			char mode;	//0, Add; 1, remove; 2, clear
			char type;	//0, permit area; 1, limit area;
			int idx;
			float radius;		//°ë¾¶
			A3DVECTOR center;	//ÖĞĞÄµã
		};

		struct enter_raid
		{
			single_data_header header;
			unsigned char raid_faction;	// 0 ÖĞÁ¢ 1 ¹¥·½ 2 ÊØ·½ 3 ÅÔ¹Û
			unsigned char raid_type;
			int raid_id;			// ¸±±¾id
			int end_timestamp;		// ½áÊøÊ±¼ä
		};
		
		struct leave_raid
		{
			single_data_header header;
		};
		/* liuyue-facbase
		struct notify_raid_pos
		{
			single_data_header header;
			A3DVECTOR pos;
			int raid_map_id;		//¸±±¾µØÍ¼ºÅ, ¿Í»§¶Ë¼ÓÔØ×ÊÔ´Ó¦¸ÃÓÃÕâ¸öID
			int tag;				//¸±±¾ÊµÀıtag
		};
		*/
		struct player_raid_counter
		{
			single_data_header header;
			int cnt;
			struct
			{
				int raid_map_id;	//¸±±¾µØÍ¼ºÅ
				int counter;		//µ±ÈÕ½øÈë´ÎÊı
			} raid_counter[];
		};

		struct player_single_raid_counter
		{
			single_data_header header;
			int map_id;
			short counter;
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
			unsigned char type;		//0, À­³¶£¬1.Ç£Òı
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
				short reborn_cnt;
			};

			single_data_header header;
			unsigned char player_count;
			player_info_in_raid info[0];
		};

		struct trigger_skill_time
		{
			single_data_header header;
			short skill_id;
			short time;//Ãë
		};

		struct player_stop_skill
		{
			single_data_header header;
			int player_id;
			char spirit_index; //0-2, ÁéµÄindex
		};

		struct addon_skill_permanent
		{
			single_data_header header;
			int skill_id;
			int skill_level;
		};

		struct addon_skill_permanent_data
		{
			single_data_header header;
			char content[];
		};

		struct zx_five_anni_data
		{
			single_data_header header;
			char star;
			char state; //0, ³õÊ¼£»1£¬µã»÷¹ı¹§Ï²°´Å¥£»2£¬µã»÷¹ıÁì½±°´Å¥
		};

		struct treasure_info
		{
			single_data_header header;

			int remain_dig_count;	//Ê£ÓàÌ½Ë÷´ÎÊı
			int region_count;	//ÇøÓòÊıÁ¿
			struct 
			{
				char type;	//0-ÎŞĞ§ 1-ÆÕÍ¨ 2-Òş²Ø
				char status;	//0-ÎŞĞ§ 1-¿ÉÌ½Ë÷ 2-¹Ø±Õ(ÆÕÍ¨ÇøÓò) 3-Ëø¶¨(Òş²ØÇøÓò)
				char level;
				int id;
			}treasure_region[];
		};

		struct treasure_region_upgrade
		{
			single_data_header header;
			int region_id;
			int region_index;
			bool success;
			int new_level;
		};

		struct treasure_region_unlock
		{
			single_data_header header;
			int region_id;
			int region_index;
			bool success;
		};

		struct treasure_region_dig
		{
			single_data_header header;
			int region_id;
			int region_index;
			bool success;
			char event_id;

			int award_item_num;
			struct 
			{
				int item_id;
				int item_count;
			}award_item_list[];
		};

		struct inc_treasure_dig_count
		{
			single_data_header header;
			int inc_count;			//Ôö¼ÓµÄ´ÎÊı
			int total_dig_count;		//ÏÖÔÚ×Ü¹²Ê£ÓàµÄÌ½Ë÷´ÎÊı
		};
		
		struct raid_global_var
		{       
			single_data_header header;
			int count;           
			struct
			{
				int key;
				int value;
			}var_list[];
		};	

		struct random_tower_monster
		{
			single_data_header header;
			int level;			//µ±Ç°²ã
			char reenter;		//0, ·ñ£¬1£¬ÊÇ
		};

		struct random_tower_monster_result
		{
			single_data_header header;
			int _monste_tid;	//-1±íÊ¾Îª¿Õ
			char _client_idx;
		};	

		struct player_tower_data
		{
			single_data_header header;
			int tower_level;
			int start_level_today;	 //µ±ÈÕÍæ¼ÒÑ¡Ôñ¿ªÊ¼²ã
			int end_level_today;	 //µ±ÈÕÍæ¼Ò´ïµ½µÄ×î¸ß²ã
			int best_pass_times[10]; //Ã¿10²ãµÄÊ±¼ä, Ã»ÓĞÊ±¼äÎª0
			int level_cnt;
			struct
			{
				char daily_reward_delivered;
				char lifelong_reward_delivered;
			}tower_reward_state[];
		};

		struct player_tower_monster_list
		{
			single_data_header header;
			int _tower_monster_list_cnt;
			int _tower_monster_list[]; 
		};

		struct player_tower_give_skills
		{
			single_data_header header;
			int cnt;
			struct 
			{
				int skill_id;
				int skill_level;
				bool used;
			}skills[];
		};

		struct set_tower_give_skill
		{
			single_data_header header;
			int skill_id;
			int skill_level;
			bool used;
		};

		struct add_tower_monster
		{
			single_data_header header;
			int monster_tid;
		};

		struct set_tower_reward_state
		{
			single_data_header header;
			char type;	//0, Ã¿ÈÕ; 1, ÖÕÉí
			int level;
			bool deliveried; 	//ÊÇ·ñ·¢ËÍ³É¹¦
		};

		struct littlepet_info
		{
			single_data_header header;
			char level;
			char cur_award_lvl;	//µ±Ç°ÁìÈ¡µ½¼¸¼¶½±Àø
			int cur_exp;
		};

		//Ôª»êĞÅÏ¢
		struct rune_info
		{
			single_data_header header;
			bool active;			//Ôª»ê×°±¸Î»ÊÇ·ñ¼¤»î
			int rune_score;			//Ôª»êÏ´Á·Öµ
			int customize_info;		//Ôª»ê¶¨ÖÆĞÅÏ¢ (0x01 0x02 0x04 0x08)
		};

		struct rune_refine_result
		{
			single_data_header header;
			char type; // 0 -Ï´Á·  1-½ÓÊÜÏ´Á¶½á¹û
		};

		struct tower_level_pass
		{
			single_data_header header;
			int level;			//Í¨¹ı¹Ø¿¨µÄ²ãÊı
			int best_time;		//×îºÃÊ±¼ä
		};

		struct fill_platform_mask
		{
			single_data_header header;
			int mask;
		};
		

		struct pk_bet_data
		{
			struct pk_bet
			{
				int bet_1st_num;
				bool top3;
			} _pk_bets[8];
			bool _top3_bet;
			bool _1st_bet_reward_deliveried;
			bool _1st_bet_reward_result_deliveried;
			bool _top3_bet_reward_deliveried;
			bool _top3_bet_reward_result_deliveried[3];
		};
		
		struct put_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned char index;		//·ÅÔÚÄÄ¸öÎ»ÖÃ
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//ÔÚÄÄ¸ö°ü¹üÀ¸£¬0 ±ê×¼£¬2 ÈÎÎñ£¬1 ×°±¸
			int state;
		};

		struct object_start_special_move
		{
			single_data_header header;
			int id;
			A3DVECTOR velocity;
			A3DVECTOR acceleration;
			A3DVECTOR cur_pos;
			char collision_state;		//0: Ã»ÓĞÅö×²   1:·¢ÉúÅö×² 
			int timestamp;
		};

		struct object_special_move
		{
			single_data_header header;
			int id;
			A3DVECTOR velocity;
			A3DVECTOR acceleration;
			A3DVECTOR cur_pos;
			char collision_state;		//0: Ã»ÓĞÅö×²   1:·¢ÉúÅö×² 
			int timestamp;
		
		};

		struct object_stop_special_move
		{
			single_data_header header;
			int id;
			unsigned char dir;
			A3DVECTOR cur_pos;
		};

		struct player_change_name
		{
			single_data_header header;
			int id;
			int name_len;
			char name[0];
		};

		struct enter_collision_raid
		{
			single_data_header header;
			int roleid;
			unsigned char raid_faction;	// 0 ÖĞÁ¢ 1 ¹¥·½ 2 ÊØ·½ 3 ÅÔ¹Û
			unsigned char raid_type;
			int raid_id;			// ¸±±¾id
			int end_timestamp;		// ½áÊøÊ±¼ä
		};

		struct collision_raid_score
		{
			single_data_header header;
			int attacker_score;
			int defender_score;
			int max_killer_id;
			int count;
			struct
			{
				int roleid;
				int faction;
				int kill_count;
				int death_count;
			}member_info[];
		};	
		
		struct collision_special_state
		{
			single_data_header header;
			int roleid;
			int type;
			char on;	//0- Í£Ö¹ 1-¿ªÊ¼ 
		};
		
		struct change_faction_name
		{
			single_data_header header;
			int id;
			int fid;
			char type;	//0 -°ïÅÉ 1-¼Ò×å
			int name_len;
			char name[0];
		};

		struct player_darklight
		{
			single_data_header header;
			int roleid;					//Íæ¼ÒID
			int dark_light_value;		//-100 to 100
		};

		struct player_darklight_state
		{
			single_data_header header;
			int id;					//Íæ¼ÒID
			char dark_light_state;	//0, ¹â; 1. °µ;
		};

		struct player_darklight_spirit
		{
			single_data_header header;
			int id;
			char darklight_spirits[3];	//0: °µÁé£»1£º¹âÁé£»2£ººÏÒ»Áé£»
		};

		struct mirror_image_cnt
		{
			single_data_header header;
			int id;
			char mirror_image_cnt;		//µ±Ç°¾µÏñµÄÊıÁ¿
		};

		struct player_wing_change_color
		{
			single_data_header header;
			int id;
			unsigned char wing_color;
		};

		// ÉÏ¹ÅÊÀ½ç´«ËÍ£¬Í¨¸æ¿Í»§¶Ëµ±Ç°¿ªÆôµÄ´«ËÍµã
		// by sunjunbo 2012-8-20
		struct ui_transfer_opened_notify
		{
			single_data_header header;
			unsigned short length;						// ³¤¶È
			int opened_ui_transfer_indexs[];    		// ´ò¿ªµÄ´«ËÍµãË÷Òı	
		};

		// Í¨¸æ»îÔ¾¶ÈÏà¹ØĞÅÏ¢
		// by sunjunbo 2012-11-07
		struct liveness_notify
		{
			single_data_header header;
			int cur_point;       // µ±Ç°µÄ·ÖÊı		
			char is_taken[4];    // ½ñÌìÊÇ·ñÁìÈ¡¹ıÁË	
			int length;	         // Íê³ÉµÄidÊıÄ¿
			int achieved_ids[];  // Íê³ÉµÄids
		};

		struct notify_astrology_energy
		{
			single_data_header header;
			bool active;					//µ±Ç°ÊÇ·ñÏÔÊ¾¾«Á¦Ìõ
			int cur_energy; 				//µ±Ç°ĞÇ×ù¾«Á¦Öµ
		};

		struct astrology_upgrade_result
		{
			single_data_header header;
			bool success;					//false - Ê§°Ü  1-³É¹¦
			int new_level;				
		};

		struct astrology_destroy_result
		{
			single_data_header header;
			int gain_energy;				//»ñµÃµÄ¾«Á¦Öµ
		};

		struct talisman_refineskill
		{
			single_data_header header;
			int id;
			int index;
		};

		struct collision_raid_result
		{
			single_data_header header;
			char result;	//0-Æ½¾Ö  1-Ó® 2-Êä
			int score_a;	//A»ı·Ö
			int score_b;	//B»ı·Ö
		};

		struct collision_raid_info
		{
			single_data_header header;
			int score_a;	//A»ı·Ö
			int score_b;	//B»ı·Ö
			int win_num;	//Ê¤³¡
			int lost_num;	//°Ü³¡
			int draw_num;	//Æ½¾Ö
			int daily_award_timestamp;	//ÉÏ´ÎÁìÈ¡Ã¿ÈÕ½±ÀøµÄÊ±¼ä´Á
			int last_collision_timestamp;	//ÉÏ´Î²Î¼ÓÅö×²Õ½³¡µÄÊ±¼ä´Á
			int day_win;		//×î½üÒ»´Î²Î¼ÓÕ½³¡µ±ÌìµÄÊ¤³¡Êı
			int cs_personal_award_timestamp;  //¿ç·ş6v6¸öÈË¿ç·şÃ¿ÖÜ·Öµµ½±Àø 
			int cs_exchange_award_timestamp; //¿ç·ş6v6Ã¿ÖÜ¶Ò»»±Ò½±Àø
			int cs_team_award_timestamp;	//¿ç·ş6v6Õ½¶ÓÈü¼¾½±Àø

			int exchange_award_info_count;
			struct exchange_info
			{
				int item_id;
				int exchange_count;
			};
			exchange_info exchange_award_info[0];
		};

		struct dir_visible_state
		{
			single_data_header header;
			int player_id;	//Íæ¼ÒID
			int id;		//Ğ§¹ûID
			int dir;	//0-360¶È
			bool on;	//¿ª¹Ø
		};

		struct player_remedy_metempsychosis_level
		{
			single_data_header header;
		};

		struct collision_player_pos
		{
			single_data_header header;
			int roleid;
			A3DVECTOR pos;
		};

		struct enter_kingdom
		{
			single_data_header header;
			unsigned char battle_faction;	//0 ÖĞÁ¢ 1 ¹¥·½ 2 ÊØ·½ 3 ÅÔ¹Û 4 »ìÕ½
			char kingdom_type;	//1-Ö÷Õ½³¡ 2-¸¨Õ½³¡
			char cur_phase;		//µ±Ç°½×¶Î
			int end_timestamp;		// ½áÊøÊ±¼ä
			int defender_mafia_id;		//·ÀÊØ·½°ïÅÉid
			int attacker_mafia_count;
			int attacker_mafia_ids[];

		};

		struct leave_kingdom
		{
			single_data_header header;
			char kingdom_type;	//1-Ö÷Õ½³¡ 2-¸¨Õ½³¡
		};

		struct raid_level_start
		{
			single_data_header header;
			char level; // -1±íÊ¾×ÜÌôÕ½£¬0-9±íÊ¾ÆäËû¹Ø¿¨
		};

		struct raid_level_end
		{
			single_data_header header;
			char level; // -1±íÊ¾×ÜÌôÕ½£¬0-9±íÊ¾ÆäËû¹Ø¿¨
			bool result;
		};

		struct raid_level_result
		{
			single_data_header header;
			bool final;
			bool level_result[10];
			bool final_award_deliveried;
			bool level_award_deliveried[10];
		};

		struct raid_level_info
		{
			single_data_header header;
			char level; // -1±íÊ¾×ÜÌôÕ½£¬0-9±íÊ¾ÆäËû¹Ø¿¨
			short matter_cnt;
			int start_time;
		};
		
		struct kingdom_half_end 
		{
			single_data_header header;
			int win_faction;	//1-·ÀÊØ·½Ê¤Àû  2-½ø¹¥·½Ê¤Àû
			int end_timestamp;	//ÏÂÒ»³¡½áÊøµÄÊ±¼ä(Èç¹ûÊØ·½»ñÊ¤ÔòÖ±½Ó½áÊø£¬¸ÄÖµÎª0)
		};

		struct kingdom_end_result
		{
			single_data_header header;
			int win_mafia_id;	//Ê¤Àû·½µÄ°ïÅÉid
		};

		struct kingdom_mafia_info
		{
			struct player_info_in_kingdom
			{
				int roleid;
				int mafia_id;
				int kill_count;
				int death_count;
			};
			
			single_data_header header;
			int player_count;
			player_info_in_kingdom info[0];
			
		};
		struct sale_promotion_info
		{
			single_data_header header;
			int task_count;
			int data[0]; // task ids
		};

		struct event_start
		{
			single_data_header header;
			int event_id;
		};

		struct event_stop
		{
			single_data_header header;
			int event_id;
		};

		struct event_info
		{
			single_data_header header;
			int event_count;
			int events[0];
		};

		struct cash_gift_ids
		{
			single_data_header header;
			int gift_id1;			//1¸ö»ı·Ö¶ÔÓ¦µÄid
			int gift_id2;			//10000¸ö»ı·Ö¶ÔÓ¦µÄid
		};

		struct notify_cash_gift
		{
			single_data_header header;
			int gift_id;			//Àñ°üid
			int gift_index;			//Àñ°üÎ»ÖÃ
			int gift_score;			//Àñ°ü»ı·Ö
		};

		struct player_propadd
		{
			single_data_header header;
			int add_prop[30];
		};

		struct player_get_propadd_item
		{
			single_data_header header;
			int item_id;
			int item_idx;
		};

		struct player_extra_equip_effect
		{
			single_data_header header;
			int player_id;
			char weapon_effect_level;
		};

		struct kingdom_key_npc_info
		{
			single_data_header header;
			int cur_hp;
			int max_hp;
			int hp_add;
			int hp_dec;
		};

		struct notify_kingdom_info
		{
			single_data_header header;
			int kingdom_title;		//¹ú¼ÒÖ°Î» 1:¹úÍõ 2:Íõºó 3:½«¾ü 4:´ó³¼ 5:³ÉÔ±
			int kingdom_point;		//¹ú¼Ò»ı·Ö
		};


		struct cs_flow_player_info
		{
			single_data_header header;
			int player_id;	//Íæ¼ÒID
			int score;		//Íæ¼Òµ±Ç°·Ö
			int c_kill_cnt;	//Á¬Ğø»÷É±´ÎÊı
			bool m_kill_change;
			int m_kill_cnt;	//¶àÉ±Êı
			short kill_cnt;
			short death_cnt; 
			short max_ckill_cnt;
			short max_mkill_cnt;
		};

		struct player_bath_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bath_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bath_start
		{
			single_data_header header;
			int invitor;	//ÑûÇëÕß(´êÔèµÄÈË)
			int invitee;	//±»ÑûÇëµÄ(±»´êµÄ)
		};

		struct player_bath_stop
		{
			single_data_header header;
			int who;
		};

		struct enter_bath_world 
		{
			single_data_header header;
			int remain_bath_count;
		};

		struct leave_bath_world 
		{
			single_data_header header;
		};

		struct bath_gain_item
		{
			single_data_header header;
			int roleid;
			int item_id;
			int item_count;
		};

		struct flow_battle_info
		{
			struct player_info_in_flow_battle
			{
				int   id;
				short level;
				int   cls;
				unsigned char battle_faction;
				unsigned short kill;
				unsigned short death;
				unsigned int  zone_id;
				unsigned int  flow_score;
			};
			single_data_header header;
			unsigned char player_count;
			player_info_in_flow_battle info[0];
		};

		struct kingdom_gather_mine_start
		{
			single_data_header header;
			int roleid;
			int mafia_id;
		};

		struct fuwen_compose_info
		{
			single_data_header header;
			int compose_count;	//½ñÌìÒÑ¾­ºÏ³É¶àÉÙ´Î
			int extra_compose_count;	//½ñÌìÊ¹ÓÃ¶îÍâµÀ¾ßºÏ³ÉµÄ´ÎÊı
		};

		struct fuwen_compose_end
		{
			single_data_header header;
		};

		struct fuwen_install_result
		{
			single_data_header header;
			int src_index;
			int dst_index;
		};

		struct fuwen_uninstall_result
		{
			single_data_header header;
			int fuwen_index;
			int inv_index;
		};

		struct multi_exp_mode
		{
			single_data_header header;
			int mulit_exp_factor;	//¶à±¶¾­Ñé±¶Êı
			int timestamp;		//µ½ÆÚÊ±¼ä
		};

		struct exchange_item_list_info
		{
			single_data_header header;
			unsigned char where;
			size_t content_length;
			char content[];
		};
		
		//WebÉÌ³Ç¶ÔÓ¦µÄÉÌÆ·ĞÅÏ¢
		struct web_order_goods
		{
			int _goods_id;
			int _goods_count;
			int _goods_flag;
			int _goods_time;
		};

		//WebÉÌ³Ç¶ÔÓ¦µÄÒ»¸öÀñ°ü
		struct web_order_package
		{
			int _package_id;
			size_t _name_len;
			char _package_name[0];
			int _count;
			int _price;
			int _goods_count;
			web_order_goods * _order_goods;
		};

		//WebÉÌ³Ç¶ÔÓ¦µÄÒ»´Î¶©µ¥
		struct web_order_info
		{
			int64_t _order_id;
			int _pay_type;	//1:coupon 2:cash
			int _status;	//0:Î´ÁìÈ¡ 1:ÒÑ¾­ÁìÈ¡
			int _timestamp;
			int _package_count;
			web_order_package * _order_package;
		};

		struct web_order_list
		{
			int web_order_count;
			web_order_info * _order_info;
		};

		struct cs_battle_off_line_info
		{
			single_data_header header;
			struct player_off_line_in_cs_battle
			{
				unsigned char battle_faction;
				int _timestamp; //¹ıÁË¶àÉÙÃë
				unsigned char off_line_count;
				unsigned char off_line_score;
			};
			unsigned char off_line_count;
			player_off_line_in_cs_battle off_line[0];
		};

		struct control_trap_info
		{
			single_data_header header;
			int id;
			int tid;
			int time;
		};

		struct step_raid_score_info
		{    
			single_data_header header;
			char tower_index;
			char monster_index;
			char award_icon;
			int award_id;
			int score;
		};  

		struct npc_invisible
		{
			single_data_header header;
			int npc_id;
			bool on;
		};

		struct puppet_form
		{
			single_data_header header;
			bool on;
			int player_id;
			int puppet_id;
		};

		struct teleport_skill_info
		{
			single_data_header header;
			int player_id;
			int npc_tid;
			int skill_id;
			int teleport_count;
		};
		
		struct mob_active_ready_start
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
		};

		struct mob_active_running_start
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
		};

		struct mob_active_end_start
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
		};

		struct mob_active_end_finish
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
			A3DVECTOR player_pos;
			A3DVECTOR mobactive_pos;
		};

		struct mob_active_cancel
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
			A3DVECTOR player_pos;
			A3DVECTOR mobactive_pos;
		};

		struct blow_off
		{
			single_data_header header;
			int roleid;
		};

		struct player_qilin_invite
		{
			single_data_header header;
			int who;
		};

		struct player_qilin_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_qilin_start
		{
			single_data_header header;
			int invitor;	//÷è÷ë
			int invitee;	//±»ÑûÇëµÄ
		};

		struct player_qilin_stop
		{
			single_data_header header;
			int who;
		};

		struct player_qilin_disconnect
		{
			single_data_header header;
			int who;
		};

		struct player_qilin_reconnect
		{
			single_data_header header;
			int who;
			A3DVECTOR newpos;
		};

		struct phase_info
		{
			single_data_header header;
			int phase_count;

			struct phase
			{
				int phase_id;
				bool on;
			};
			phase phase_list[0];
		};

		struct phase_info_change
		{
			single_data_header header;
			int phase_id;
			bool on;
		};

		struct fac_building_add
		{
			single_data_header header;
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
			int building_tid; //½¨ÖşµÄÄ£°å id
		};

		struct fac_building_upgrade
		{
			single_data_header header;
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
			int building_tid; //½¨ÖşµÄÄ£°å id
			int new_level;
		};

		struct fac_building_remove
		{
			single_data_header header;
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
		};

		struct fac_building_complete //½¨Öş½¨Éè»òÉı¼¶³É¹¦
		{
			single_data_header header;
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
			int building_tid; //½¨ÖşµÄÄ£°å id
			int level;
		};

		struct fac_base_prop_change
		{
			single_data_header header;
			fac_base_prop value;
			int noti_roleid;
		};

		struct player_active_emote_invite
		{
			single_data_header header;
			int who;
			int type;
		};

		struct player_active_emote_invite_reply
		{
			single_data_header header;
			int who;
			int type;
			int param;
		};

		struct player_active_emote_start
		{
			single_data_header header;
			int who;
			int who1;
			int type;
		};

		struct player_active_emote_stop
		{
			single_data_header header;
			int who;
			int who1;
			int type;
		};

		struct player_fac_base_info
		{
			single_data_header header;
			fac_base_prop prop;
			int msg_size; //»ùµØ ÁôÑÔ size
			char msg[0];  //»ùµØ ÁôÑÔ
			unsigned char field_count;
			player_fac_field fields[0];
		};

		struct be_taunted2
		{
			single_data_header header;
			int id;
			int time;
		};

		struct notify_bloodpool_status
		{
			single_data_header header;
			int player_id;
			bool on;
			int cur_hp;
			int max_hp;
		};

		struct player_facbase_auction
		{
			single_data_header header;
			int return_coupon;		//ÍË»ØµÄ½ğÈ¯Êı
			int size;
			fac_base_auc_item list[];
			int history_size;
			fac_base_auc_history history[];
		};

		struct facbase_auction_update
		{
			single_data_header header;
			enum
			{
				ADD = 1, //ĞÂÅÄÂôÉÏ¼Ü
				END,	 //ÅÄÂô½áÊø
				DEL, 	 //ÅÄÂôÉ¾³ı
				REFRESH, //ÅÄÂôË¢ĞÂ
			};
			int type;
			fac_base_auc_item item;
		};

		struct fac_coupon_return
		{
			single_data_header header;
			int coupon_return;
		};

		struct player_start_travel_around
		{
			single_data_header header;
			int player_id;
			int travel_vehicle_id;
			float travel_speed;
			int travel_path_id;
		};

		struct player_stop_travel_around
		{
			single_data_header header;
			int player_id;
		};

		struct player_get_summon_petprop
		{
			single_data_header header;
			int pet_index;
		//	q_extend_prop prop;		//²»ÏëÒıÓÃÍ·ÎÄ¼ş£¬ËùÒÔ×¢ÊÍµôÁË
		};

		struct facbase_auction_add_history
		{
			single_data_header header;
			fac_base_auc_history entry;
		};

		struct facbase_msg_update
		{
			single_data_header header;
			int msg_size; //»ùµØ ÁôÑÔ size
			char msg[0];  //»ùµØ ÁôÑÔ
		};

		struct cs6v6_cheat_info
		{
			single_data_header header;
			bool is_cheat;
			int cheat_counter;	//µ¹¼ÆÊ±
		};

		struct hide_and_seek_raid_info
		{
			single_data_header header;
			int last_hide_and_seek_timestamp;
			int day_has_get_award; // 0²»ÄÜÁìÈ¡,1¿ÉÒÔÁìÈ¡,2µ±ÈÕÒÑ¾­ÁìÈ¡¹ı
		};

		struct hide_and_seek_round_info
		{
			single_data_header header;
			char round;
			char round_status;
			int end_timestamp;
		};

		struct hide_and_seek_players_info
		{
			single_data_header header;
			int total_num;
			struct seek_player_info
			{
				int playerid;
				short score;
				char status;
				int occupation;
			};
			seek_player_info player_info[1];
		};

		struct hide_and_seek_skill_info
		{
			single_data_header header;
			char skill_left_num;
			char skill_buy_num;
		};

		struct hide_and_seek_blood_num
		{
			single_data_header header;
			int seeker_id;
			int hider_id;
			char blood_num;
		};

		struct hide_and_seek_role
		{
			single_data_header header;
			int player_id;
			char role_type;
		};

		struct hide_and_seek_enter_raid
		{
			single_data_header header;
		};

		struct hide_and_seek_leave_raid
		{
			single_data_header header;
		};

		struct hide_and_seek_hider_taunted
		{
			single_data_header header;
			int playerid;
			A3DVECTOR pos;
			bool isstart;
		};

		struct capture_raid_player_info
		{
			single_data_header header;
			int attacker_score; // ¹¥·½»ı·Ö
			int defender_score; // ÊØ·½»ı·Ö
			char attacker_flag_count; //¹¥·½¶áÆìÊı
			char defender_flag_count; //ÊØ·½¶áÆìÊı
			int max_attacker_killer_id; //¹¥·½É±Éñ 
			int max_defender_killer_id; //ÊØ·½É±Éñ
			short max_attacker_kill_count; //¹¥·½É±ÉñÉ±ÈËÊı
			short max_defender_kill_count; //ÊØ·½É±ÉñÉ¶ÈËÊı
			int score_info_count;
			struct capture_score_info
			{
				int roleid;
				short kill_count;
				short death_count;
				char faction;
				char flag_count;
			};
			capture_score_info info[0];
		};

		struct capture_broadcast_score
		{
			single_data_header header;
			char faction;
			char score;
		};

		struct capture_submit_flag
		{
			single_data_header header;
			char faction;
			int player_id;
			int name_len;
			char name[];
		};

		struct capture_reset_flag
		{
			single_data_header header;
			char faction;
		};

		struct capture_gather_flag
		{
			single_data_header header;
			char faction;
			int player_id;
			int name_len;
			char name[];
		};

		struct capture_monster_flag
		{
			single_data_header header;
			char faction;
			int monster_id;
		};

		struct capture_broadcast_tip
		{
			single_data_header header;
			char tip; // 1,¸ñ×Ó£ 2ÈÃÒÆ¶¯ 
		};

		struct capture_broadcast_flag_move
		{
			single_data_header header;
			char faction;
			A3DVECTOR pos;
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
		FORCE_ATTACK_WHITE	= 0x01,		//¹¥»÷°×Ãû
		FORCE_ATTACK_RED	= 0x02,		//¹¥»÷ºìÃû
		FORCE_ATTACK_FAMILY	= 0x04,		//¹¥»÷¼Ò×å
		FORCE_ATTACK_MAFIA	= 0x08,		//¹¥»÷°ïÅÉ³ÉÔ±
		FORCE_ATTACK_ZONE	= 0x10,		//¹¥»÷¿ç·şÍæ¼Ò
		FORCE_ATTACK_ALL	= 0x1F,		//¹¥»÷ËùÓĞ
	};
	
	namespace INFO
	{
		struct move_info
		{
			A3DVECTOR cur_pos;
			A3DVECTOR next_pos;
			unsigned short use_time;	//Ê¹ÓÃµÄÊ±¼ä µ¥Î»ÊÇms
							//Ê¹ÓÃµÄÊ±¼ä¶ÔÓÚÂß¼­·şÎñÆ÷À´Ëµ£¬Ö»ÊÇÒ»¸ö²Î¿¼Öµ
							//Í¬Ê±ÓÃÓÚ¼ì²âÓÃ»§µÄÖ¸ÁîÊÇ·ñÕıÈ·£¬ÎŞÂÛÈçºÎ£¬
							//ÓÃ»§µÄÒÆ¶¯¶¼»áÔÚ¹Ì¶¨µÄ0.5ÃëÖÓÒÔºó½øĞĞ
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
		RESURRECT_IN_TOWN,	//³ÇÕò¸´Éú
//5		
		RESURRECT_BY_ITEM,	//ÎïÆ·¸´Éú
		PICKUP,			//¼ìÆğÎïÆ·»òÕß½ğÇ®
		STOP_MOVE,
		UNSELECT,		//ÖĞÖ¹µ±Ç°Ñ¡¶¨µÄÄ¿±ê
		GET_ITEM_INFO,		//È¡µÃÎïÆ·ÌØ¶¨Î»ÖÃµÄĞÅÏ¢

//10
		GET_INVENTORY,		//È¡µÃÄ³¸öÎ»ÖÃÉÏµÄËùÓĞÎïÆ·ÁĞ±í
		GET_INVENTORY_DETAIL,	//È¡µÃÄ³¸öÎ»ÖÃÉÏµÄËùÓĞÎïÆ·ÁĞ±í£¬°üº¬ÏêÏ¸µÄÎïÆ·Êı¾İ
		EXCHANGE_INVENTORY_ITEM,
		MOVE_INVENTORY_ITEM,
		DROP_INVENTORY_ITEM,

//15		
		DROP_EQUIPMENT_ITEM,
		EXCHANGE_EQUIPMENT_ITEM,
		EQUIP_ITEM,		//×°±¸ÎïÆ·£¬½«ÎïÆ·À¸ÉÏºÍ×°±¸À¸ÉÏµÄÁ½¸öÎ»ÖÃ½øĞĞµ÷»»
		MOVE_ITEM_TO_EQUIPMENT,
		GOTO,

//20		
		DROP_MONEY,		//ÈÓ³öÇ®µ½µØÉÏ
		SELF_GET_PROPERTY,
		LEARN_SKILL,		//Ñ§Ï°¼¼ÄÜ
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
		STOP_FALL,	//ÖÕÖ¹µøÂä
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
		SUMMON_PET,	//ÕÙ»½³èÎï
		RECALL_PET,	//ÊÕ»Ø³èÎï
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
		EQUIP_PET_BEDGE,	//×°±¸³èÎïÅÆ,½«³èÎïÅÆÀ¸ÉÏºÍ×°±¸À¸ÉÏµÄÁ½¸öÎ»ÖÃ½øĞĞµ÷»»
		EQUIP_PET_EQUIP,	//×°±¸³èÎï×°±¸,½«³èÎï×°±¸À¸ÉÏºÍ×°±¸À¸ÉÏµÄÁ½¸öÎ»ÖÃ½øĞĞµ÷»»

//120
		COMBINE_PET,		//³èÎïºÏÌå
		UNCOMBINE_PET,		//½â³ıºÏÌå
		SET_PET_STATUS,		//ÉèÖÃ³èÎï×´Ì¬
		SET_PET_RANK,		//ÉèÖÃ³èÎï½×¼¶
		MOVE_PET_BEDGE,		//µ÷»»³èÎïÅÆµÄÎ»ÖÃ

//125
		START_FLY,
		STOP_FLY,
		SET_FASHION_MASK,
		START_ONLINE_AGENT,
		JOIN_INSTANCE,
		
//130
		BIND_RIDE_INVITE, 		//¶àÈËÆï³ËÑûÇë
		BIND_RIDE_INVITE_REPLY, 	//¶àÈËÆï³ËÑûÇëµÄ»Ø¸´
		BIND_RIDE_CANCEL, 		//Àë¿ª¶àÈËÆï³Ë
		BIND_RIDE_KICK,             	//Ö÷ÆïÌßÈË
		POST_SNS_MESSAGE,		//Íæ¼Ò·¢²¼ĞÅÏ¢

//135
		APPLY_SNS_MESSAGE,		//Íæ¼ÒÓ¦Õ÷ĞÅÏ¢
		VOTE_SNS_MESSAGE,		//Íæ¼ÒÍ¶Æ±
		RESPONSE_SNS_MESSAGE, 		//Íæ¼ÒÁôÑÔ		
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
		QUERY_OTHERS_ACHIEVEMENT,		// ²é¿´ÆäËûÈËµÄ³É¾Í
		UPDATE_PVP_MASK,
		START_TRANSFORM,			//¿ªÊ¼±äÉí
		STOP_TRANSFORM,				//Í£Ö¹±äÉí
		
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
		UNIQUE_BID_REQUEST,				//Î¨Ò»×îµÍ¼ÛÅÄÂô¾º¼Û
		UNIQUE_BID_GET,					//Î¨Ò»×îµÍ¼Û»ñÈ¡½±ÀøÎïÆ·
		GET_CLONE_EQUIPMENT,				//»ñÈ¡·ÖÉíÖ÷ÈËµÄ×°±¸ĞÅÏ¢
		TASK_FLY_POS,
		ZONEMALL_SHOPPING,

		
//170
		ACTIVITY_FLY_POS,
		DEITY_LEVELUP,
		QUERY_CIRCLE_OF_DOOM_INFO,      //²éÑ¯Õó·¨³ÉÔ±ĞÅÏ¢£¬ÓÃÓÚ¿Í»§¶ËÏÔÊ¾
		RAID_LEAVE,						//Íæ¼ÒÖ÷¶¯Àë¿ª
		CANCEL_PULLING,					//È¡ÏûÀ­³¶

//175
		GET_RAID_INFO,					//»ñµÃ¸±±¾ĞÅÏ¢
		QUERY_BE_SPIRIT_DRAGGED,		//»ñÈ¡Ä³¸öÍæ¼Ò±»ÁéÁ¦Ç£ÒıµÄĞÅÏ¢
		QUERY_BE_PULLED,				//»ñÈ¡Ä³¸öÍæ¼Ò±»À­³¶µÄĞÅÏ¢
		GET_MALL_SALETIME_ITEM,         //»ñÈ¡ÉÌ³Çµ±Ç°ÕıÔÚÏúÊÛµÄÏŞÊ±ÏúÊÛÎïÆ·
		GET_RAID_COUNT,					//»ñÈ¡¸±±¾½øÈë´ÎÊı

//180
		GET_VIP_AWARD_INFO,				//»ñÈ¡VIP½±ÀøĞÅÏ¢
		GET_VIP_AWARD_BY_ID,			//»ñÈ¡VIP½±Àø¶ÔÓ¦µÄ½±ÀøÎïÆ·
		TRY_GET_ONLINE_AWARD,			//³¢ÊÔ»ñÈ¡ÔÚÏß½±Àø
		GET_ONLINE_AWARD,				//»ñÈ¡ÔÚÏß½±Àø
		FIVE_ANNI_REQUEST,				//5ÖÜÄêÇ©µ½ÇëÇó
		
//185
		UPGRADE_TREASURE_REGION,		//Éı¼¶ÍÚ±¦ÇøÓòµÈ¼¶
		UNLOCK_TREASURE_REGION,			//½âËøÍÚ±¦ÇøÓò(½ö¶ÔÒş²ØÇø)
		DIG_TREASURE_REGION,			//ÍÚ±¦
		START_RANDOM_TOWER_MONSTER, 	//¿ªÊ¼Ëæ»úÅÀËş¸±±¾¹ÖÎï
		ADOPT_LITTLEPET,		//ÁìÑøÖïĞ¡ÏÉ

//190
		FEED_LITTLEPET,			//Î¹ÑøÖïĞ¡ÏÉ
		GET_LITTLEPET_AWARD,		//ÁìÈ¡ÖïĞ¡ÏÉ½±Àø
		RUNE_IDENTIFY,			//Ôª»ê¼ø¶¨
		RUNE_COMBINE,			//Ôª»êºÏ³É
		RUNE_REFINE,			//Ôª»êÏ´Á·

//195
		RUNE_RESET,			//Ôª»ê¹éÔª
		RUNE_DECOMPOSE,			//Ôª»ê·Ö½â
		RUNE_LEVELUP,			//Ôª»êÉı¼¶
		RUNE_OPEN_SLOT,			//Ôª»ê¿ªÆô·ûÎÄ (×÷·Ï)
		RUNE_CHANGE_SLOT,		//Ôª»ê¸Ä±ä¿×Î»

//200
		RUNE_ERASE_SLOT,		//Ôª»ê²ÁĞ´·ûÓï
		RUNE_INSTALL_SLOT,		//Ôª»êÏâÇ¶·ûÎÄ
		RUNE_REFINE_ACTION,		//Ôª»êÏ´Á¶²Ù×÷ (½ÓÊÜ»òÕß²»½ÓÊÜÏ´Á·½á¹û)
		TOWER_REWARD,					//ÁìÈ¡ÅÀËş¸±±¾½±Àø
		RESET_SKILL_PROP,       //90¼¶Ç°Ãâ·ÑÏ´µãÏ´ÌìÊé

//205
		GET_TASK_AWARD,
		PK_1ST_GUESS,
		PK_TOP3_GUESS,
		PK_1ST_GUESS_REWARD,
		PK_1ST_GUESS_RESULT_REWARD,

//210
		PK_TOP3_GUESS_REWARD,
		PK_TOP3_GUESS_RESULT_REWARD,
		GET_PLAYER_BET_DATA,	
		START_SPECIAL_MOVE,
		SPECIAL_MOVE,

//215
		STOP_SPECIAL_MOVE,
		COLLISION_RAID_APPLY,
		GET_REPURCHASE_INV_DATA,
		EXCHANGE_MOUNT_WING_ITEM, //×øÆï·É½£°ü¹üÀï½»»»Î»ÖÃ
		EXCHANGE_INVENTORY_MOUNTWING_ITEM, //ÆÕÍ¨°ü¹ü <-> ×øÆï·É½£°ü¹ü

//220
		EXCHANGE_EQUIPMENT_MOUNTWING_ITEM, //×°±¸À¸ <-> ×øÆï·É½£°ü¹ü 
		HIDE_VIP_LEVEL, //Òş²ØVIPµÈ¼¶ĞÅÏ¢
		CHANGE_WING_COLOR,		//¸Ä±ä·É½£ÑÕÉ«
		ASTROLOGY_IDENTIFY,		//ĞÇ×ù¼ø¶¨
		ASTROLOGY_UPGRADE,		//ĞÇ×ùÉı¼¶

//225
		ASTROLOGY_DESTROY,		//ĞÇ×ùÆÆËé
		JOIN_RAID_ROOM, 
		TALISMAN_REFINESKILL_RESULT, //¿Í»§¶ËÊÇ·ñ½ÓÊÜ·¨±¦¼¼ÄÜÏ´Á·½á¹û
		GET_COLLISION_AWARD,
		CANCEL_ICE_CRUST,

//230
		PLAYER_FIRST_EXIT_REASON,
		REMEDY_METEMPSYCHOSIS_LEVEL,	//ÃÖ²¹·ÉÉıµÈ¼¶ (Î´Âú¼¶·ÉÉıÓĞÊıÖµËğÊ§)
		MERGE_POTION,
		KINGDOM_LEAVE,
		GET_COLLISION_PLAYER_POS,

//235
		TAKE_LIVENESS_AWARD,
		RAID_LEVEL_AWARD,
		GET_RAID_LEVEL_RESULT,
		OPEN_TRASHBOX,
		DELIVERY_GIFT_BAG,

//240
		GET_CASH_GIFT_AWARD,
		GEN_PROP_ADD_ITEM,
		REBUILD_PROP_ADD_ITEM,
		GET_PROPADD,
		BUY_KINGDOM_ITEM,

//245
		GET_TOUCH_AWARD,		//¶Ò»»touch½±Àø
		FLOW_BATTLE_LEAVE,
		KING_TRY_CALL_GUARD,		//Ç×ÎÀ´«ËÍ
		BATH_INVITE,			//´êÔèÑûÇë
		BATH_INVITE_REPLY,		//´êÔè½±Àø

//250
		DELIVER_KINGDOM_TASK,		//·¢²¼¹úÍõÈÎÎñ
		RECEIVE_KINGDOM_TASK,		//½ÓÊÜ¹úÍõÈÎÎñ
		KINGDOM_FLY_BATH_POS,
		FUWEN_COMPOSE,			//·ûÎÄºÏ³É
		FUWEN_UPGRADE,			//·ûÎÄÉı¼¶

//255
		FUWEN_INSTALL,			//·ûÎÄÏâÇ¶
		FUWEN_UNINSTALL,			//·ûÎÄ²ğ³ı
		RESIZE_INV,
		REGISTER_FLOW_BATTLE,
		ARRANGE_INVENTORY,                //ÕûÀí±³°ü

//260		
		ARRANGE_TRASHBOX,                 //ÕûÀí²Ö¿â
		GET_WEB_ORDER,			  //ÁìÈ¡web¶©µ¥ÎïÆ·
		CUSTOMIZE_RUNE,			  //¶¨ÖÆÔªÓ¤
		GET_CROSS_SERVER_BATTLE_INFO,
		CONTROL_TRAP,

//265
		SUMMON_TELEPORT,
		CREATE_CROSSVR_TEAM,
		GET_KING_REWARD,		  //»ñµÃ¹úÍõÁ¬ÈÎ½±Àø
		MOBACTIVE_START,
		MOBACTIVE_FINISH,  //»¥¶¯ÎïÆ·Íæ¼Òµã»÷Ö÷¶¯Íê³É
		
//270
		FASHION_COLORANT_COMBINE, 	// Youshuang add
		FASHION_ADD_COLOR, 		// Youshuang add
		ENTER_FACTION_BASE,		//½øÈë°ïÅÉ»ùµØ
		QILIN_INVITE,			  //÷è÷ëÑûÇë
		QILIN_INVITE_REPLY,		  //÷è÷ëÑûÇë»Ø¸´

//275
		QILIN_CANCEL,			  //÷è÷ëÈ¡Ïû
		QILIN_DISCONNECT,		  //÷è÷ëÏÂÂí
		QILIN_RECONNECT,		  //÷è÷ëÉÏÂí
		GET_ACHIEVEMENT_AWARD,	// Youshuang add
		ADD_FAC_BUILDING,		//´´½¨»ùµØ½¨Öş	
//280
		UPGRADE_FAC_BUILDING,		//Éı¼¶»ùµØ½¨Öş
		REMOVE_FAC_BUILDING,		//²ğ³ı»ùµØ½¨Öş
		PET_CHANGE_SHAPE,		//³èÎï¸Ä±äĞÎÏó
		PET_REFINE_ATTR,		//³èÎï¸Ä±äÊôĞÔ
		ACTIVE_EMOTE_ACTION_INVITE,

//285		
		ACTIVE_EMOTE_ACTION_INVITE_REPLY,
		ACTIVE_EMOTE_ACTION_CANCEL,
		GET_RAID_TRANSFORM_TASK,
		GET_FAC_BASE_INFO,
		GET_CS_6V6_AWARD,

//290
		GET_FACBASE_MALL_INFO,		// Youshuang add
		SHOP_FROM_FACBASE_MALL,		// Youshuang add
		CONTRIBUTE_FACBASE_CASH,	// Youshuang add
		GET_FACBASE_CASH_ITEMS_INFO,	// Youshuang add
		BUY_FACBASE_CASH_ITEM,		// Youshuang add

//295
		BID_ON_FACBASE,			// Youshuang add
		POST_FAC_BASE_MSG,		// »ùµØÁôÑÔ°å
		EXCHANGE_CS_6V6_AWARD,
		PUPPET_FORM_CHANGE,
		FAC_BASE_TRANSFER,		//»ùµØÄÚ´«ËÍ

//300
		EXCHANGE_CS_6V6_MONEY,
		WITHDRAW_FACBASE_AUCTION,	//ÁìÈ¡»ùµØÅÄÂôÎïÆ·
		WITHDRAW_FACBASE_COUPON,	//ÁìÈ¡ÅÄÂôÊ§°ÜÍË»ØµÄ½ğÈ¯
		GET_FACBASE_AUCTION,		//¿Í»§¶Ë»ñÈ¡»ùµØÅÄÂôÁĞ±í gdeliveryd×ª·¢¹ıÀ´µÄÏûÏ¢
		LEAVE_FACTION_BASE,		//Àë¿ª°ïÅÉ»ùµØ

//305
		OBJECT_CHARGE_TO,		
		STOP_TRAVEL_AROUND,
		GET_SUMMON_PETPROP,
		TASK_FLY_TO_AREA,		// Youshuang add
		TALISMAN_ENCHANT_CONFIRM,	// Youshuang add

//310
		GET_SEEK_AWARD,
		SEEKER_BUY_SKILL,
		HIDE_AND_SEEK_RAID_APPLY,
		ARRANGE_POCKET,				// Youshuang add
		MOVE_MAX_POCKET_ITEM_TO_INVENTORY,	// Youshuang add

//315
		NEWYEAR_AWARD,				// Youshuang add
		CAPTURE_RAID_SUBMIT_FLAG,
		CAPTURE_RAID_APPLY,
		USE_TITLE,				// Youshuang add
		
//1000		
		GM_COMMAND_START = 1000,
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
		GMCMD_MOVETOMAP,
		GM_COMMAND_END,

	};//C2S

	namespace CMD
	{
		using namespace INFO;
		struct player_move
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//ÃüÁîĞòºÅ
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
			char force_attack;		//Ç¿ÖÆ¹¥»÷Î» 0x01 ÆÕÍ¨Ç¿ÖÆ¹¥»÷ 0x02 ¹¥»÷±¾°ïÅÉ 0x04¹¥»÷°×Ãû 0x08 ¹¥»÷ºìÃû 0x10 ¹¥»÷¼Ò×å
							//¼¼ÄÜºÍÎïÆ·µÄµÄ¶¨ÒåÓë´ËÏàÍ¬
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
			unsigned short cmd_seq;		//ÃüÁîĞòºÅ
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
			int id;		//ÏëË­·¢ÆğÑûÇë 
		};

		struct team_agree_invite
		{
			cmd_header header;
			int id;		//Ë­½øĞĞµÄÑûÇë
			int team_seq;
		};

		struct team_reject_invite
		{
			cmd_header header;
			int id;		//Ë­½øĞĞµÄÑûÇë
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
			A3DVECTOR skillpos;
			int  targets[];
		};

		/**
		 * @brief ²éÑ¯Õó·¨ĞÅÏ¢
		 */
		struct query_circleofdoom_info
		{
			cmd_header header;
			int sponsor_id;     //Õó·¨·¢ÆğÕßµÄid
		};

		/**
		 * @brief »ñÈ¡¶ÔÓ¦µÄVIP½±Àø£¬´øÉÏaward_item_idÊÇ¼ì²é¿Í»§¶ËÊı¾İ°æ±¾ÊÇ·ñÓë·şÎñÆ÷Ò»ÖÂ
		 */
		struct obtain_vip_award
		{
			cmd_header header;
			int award_id;				//½±Àøid
			int award_item_id;			//½±ÀøËù¶ÔÓ¦µÄÎïÆ·µÄid
		};

		/**
		 * @brief »ñÈ¡ÔÚÏßµ¹¼ÆÊ±½±Àø£¬³¢ÊÔ»ñÈ¡¼°»ñÈ¡¶¼ÊÇÓÃÕâ¸ö½á¹¹
		 */
		struct obtain_online_award
		{
			cmd_header header;
			int		award_id;				//½±Àøid
			int		award_index;			//½±ÀøµÄµÚ¼¸ÂÖµ¹¼ÆÊ±
			int     small_giftbag_id;		//±¾ÂÖĞ¡Àñ°üid
		};

		/**
		 * @brief 90¼¶Ç°Ãâ·ÑÏ´µãÏ´ÌìÊé
		 */
		struct reset_skill_prop
		{
			cmd_header header;
			unsigned char opcode;		//1±íÊ¾Ï´¼¼ÄÜµã£¬2±íÊ¾Ï´ÌìÊé
		};

		/**
		 * @brief ×¼±¸°ÚÌ¯
		 */
		struct test_personal_market
		{
			cmd_header header;
			int index;    //Èç¹ûÃ»ÓĞÊ¹ÓÃµÀ¾ß°ÚÌ¯£¬indexÉèÎª-1
			int item_id;  //Èç¹ûÃ»ÓĞÊ¹ÓÃµÀ¾ß£¬item_idÉèÎª-1»ò0¶¼¿ÉÒÔ
		};

		/**
		 * @brief ¼ÓÈë¸±±¾£¬ÕâÀïÃ»ÓĞ×ßNPC·şÎñ£¬ÏÖÔÚÓÃÓÚ°ïÕ½¸±±¾£¬ÒòÎª°ïÖÚ²»ĞèÒª±¨Ãû
		 */
		struct join_raid_room
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			int room_id;
			char raid_faction; //ÕóÓª£º0 ÎŞÕóÓª 1 ¹¥ 2 ÊØ 3 ¹Û²ìÕß
		};

		/**
		 * @brief playerÊ×´ÎÍË³öÔ­Òò£¬ÓÃÀ´¼Çlog
		 */
		struct player_first_exit_reason
		{
			cmd_header header;
			unsigned char reason;
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
			unsigned char force_attack; //Ö»¶Ô¹¥»÷ĞÔÎïÆ·ÓĞĞ§
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
			//ÏŞÖÆÎª256Ö®ÄÚ
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
			unsigned short use_time;	//Ê¹ÓÃµÄÊ±¼ä µ¥Î»ÊÇms
							//Ê¹ÓÃµÄÊ±¼ä¶ÔÓÚÂß¼­·şÎñÆ÷À´Ëµ£¬Ö»ÊÇÒ»¸ö²Î¿¼Öµ
							//Í¬Ê±ÓÃÓÚ¼ì²âÓÃ»§µÄÖ¸ÁîÊÇ·ñÕıÈ·£¬ÎŞÂÛÈçºÎ£¬
							//ÓÃ»§µÄÒÆ¶¯¶¼»áÔÚ¹Ì¶¨µÄ0.5ÃëÖÓÒÔºó½øĞĞ
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
			int index;    //Èç¹ûÃ»ÓĞÊ¹ÓÃ°ÚÌ¯µÀ¾ßindexÎª-1£¬Ê¹ÓÃÁËindexÎªitemÔÚ°ü¹üÖĞµÄÎ»ÖÃ
			int item_id;  //Èç¹ûÃ»ÓĞÊ¹ÓÃ°ÚÌ¯µÀ¾ßitem_idÎª-1£¬Ê¹ÓÃÁËÔòÎªµÀ¾ßµÄid
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
			A3DVECTOR skillpos;
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
			unsigned char where;		//ÔÚÄÄ¸ö°ü¹üÀ¸£¬0 ±ê×¼£¬2 ÈÎÎñ£¬1 ×°±¸
			unsigned char index;		//Ë÷Òı
			int type;			//ÊÇÄÄÖÖÎïÆ·
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
			int param; // 0 Í¬Òâ  1 ²»Í¬Òâ
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
			int goods_id;
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
			unsigned char main_status; // 0 Õ½¶· 1 ²É¼¯ 2 ÖÆÔì 3 ĞİÏ¢
			unsigned char sub_status; // Ö»ÓĞmain_statusÎª 1Ê±ÓĞÓÃ ,0 ÖÖÖ² 1 ·¥Ä¾ 2 á÷ÁÔ 3 µöÓã 4 ²É¿ó 5 ¿¼¹Å
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
			unsigned char combine_type;	//0 Í¨Áé 1 Óù±¦
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

		struct move_max_pocket_item_to_inventory
		{
			cmd_header header;
		};

		struct cast_item_skill
		{
			cmd_header header;
			int skill_id;
			char skill_type; //¼¼ÄÜÀàĞÍ: 0--ÆÕÍ¨¼¼ÄÜ  1--Ë²·¢¼¼ÄÜ
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
		
		/**
		 * @brief ×øÆï·É½£°ü¹üÀï½»»»ÎïÆ·
		 */
		struct exchange_mount_wing_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		/**
		 * @brief ÆÕÍ¨°ü¹ü <-> ×øÆï·É½£°ü¹ü 
		 */
		struct exchange_inventory_mountwing_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_mw;
		};

		/**
		 * @brief ×°±¸À¸ <-> ×øÆï·É½£°ü¹ü
		 */
		struct exchange_equipment_mountwing_item
		{
			cmd_header header;
			unsigned char idx_equ;
			unsigned char idx_mw;
		};

		/**
		 * @brief Òş²ØvipµÈ¼¶ĞÅÏ¢
		 */
		struct hide_vip_level
		{
			cmd_header header;
			char is_hide;	//0±íÊ¾ÏÔÊ¾vipµÈ¼¶ĞÅÏ¢£¬1±íÊ¾Òş²ØvipµÈ¼¶ĞÅÏ¢
		};
		
		struct exchange_hotkey_equipment_fashion
		{
			cmd_header header;
			unsigned char idx_key;
			unsigned char idx_head;		//0xff´ú±íÄÇ¸ö²¿Î»²»ĞèÒªÌæ»»
		       	unsigned char idx_cloth;
			unsigned char idx_shoe;	
		};
		
		struct bonusmall_shopping
		{
			cmd_header header;
			int goods_id;
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
			char skill_type; //¼¼ÄÜÀàĞÍ: 0--ÆÕÍ¨¼¼ÄÜ  1--Ë²·¢¼¼ÄÜ
			unsigned char force_attack;
			unsigned char target_count;
			A3DVECTOR pos;
			int targets[];
		};

		struct enter_carrier  
		{
			cmd_header header;
			int carrier_id; 		//ÏëÒª³Ë×øµÄ½»Í¨¹¤¾ßid
			A3DVECTOR rpos;			//Ïà¶Ô×ø±ê
			unsigned char rdir;		//Ïà¶Ô·½Ïò
		};

		struct leave_carrier 
		{
			cmd_header header;
			int carrier_id; 		//ÏëÒªÀë¿ªµÄ½»Í¨¹¤¾ßid
			A3DVECTOR pos;			//¾ø¶Ô×ø±êºÍ·½Ïò
			unsigned char dir;		//¾ø¶Ô·½Ïò
		};
		
		struct move_on_carrier	
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//ÃüÁîĞòºÅ
		};

		struct stop_move_on_carrier
		{
			cmd_header header;
			A3DVECTOR rpos;
			unsigned short speed;
			unsigned char rdir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;		//ÃüÁîĞòºÅ
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
			int goods_id;
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

		struct query_be_spirit_dragged
		{
			cmd_header header;
			int id_be_spirit_dragged;
		};

		struct query_be_pulled
		{
			cmd_header header;
			int id_be_pulled;
		};

		struct get_raid_count
		{
			cmd_header header;
			int map_id;
		};
		
		struct five_anni_request
		{
			cmd_header header;
			char request; //0,»ñÈ¡5ÖÜÄêÊı¾İ; 1,¹§Ï²£»2,Áì½±
		};

		struct upgrade_treasure_region
		{
			cmd_header header;
			int item_id;
			int item_index;
			int region_id;
			int region_index;
		};	

		struct unlock_treasure_region
		{
			cmd_header header;
			int item_id;
			int item_index;
			int region_id;
			int region_index;
		};

		struct dig_treasure_region
		{
			cmd_header header;
			int region_id;
			int region_index;
		};

		struct start_random_tower_monster
		{
			cmd_header header;
			char client_idx;
		};
		
		struct adopt_littlepet
		{
			cmd_header header;
		};

		struct feed_littlepet
		{
			cmd_header header;
			int item_id;
			int item_count;
		};

		struct get_littlepet_award
		{
			cmd_header header;
		};

		struct rune_identify
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ°ü¹üÀïÃæµÄÎ»ÖÃ
			int assist_index;		//¸¨ÖúµÀ¾ß
		};

		struct rune_combine
		{
			cmd_header header;
			int rune_index;			//Ôª»ê (×°±¸À¸)
			int assist_index;		//¸¨ÖúÔª»ê (°ü¹üÀ¸)
		};

		// Youshuang
		struct talisman_enchant_confirm
		{
			cmd_header header;
			size_t index1;
			char confirm;
		};
		
		struct fashion_colorant_combine
		{
			cmd_header header;
			int colorant_index1;
			int colorant_index2;
			int colorant_index3;
		};

		struct fashion_add_color
		{
			cmd_header header;
			int colorant_index;
			int where;
			int fashion_item_index;
		};

		struct get_achieve_award
		{
			cmd_header header;
			unsigned short achieve_id;
			unsigned int award_id;
		};

		struct get_facbase_mall_info
		{
			cmd_header header;
		};

		struct shop_from_facbase_mall
		{
			cmd_header header;
			int item_index;
			int item_id;
			int cnt;
		};

		struct contribute_facbase_cash
		{
			cmd_header header;
			int cnt;
		};

		struct get_facbase_cash_items_info
		{
			cmd_header header;
		};


		struct buy_facbase_cash_item
		{
			cmd_header header;
			int item_index;
		};
		
		struct bid_on_facbase
		{
			cmd_header header;
			int item_index;
			int item_id;
			int coupon;
			int name_len;
			char player_name[];
		};

		struct task_fly_to_area
		{
			cmd_header header;
			int task_id;
		};

		struct newyear_award
		{
			cmd_header header;
			char award_type;
		};
		
		struct  use_title
		{
			cmd_header header;
			short title;
		};
		// end
		
		struct rune_refine
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
			int assist_index;		//¸¨ÖúµÀ¾ß
		};

		struct rune_reset
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
			int assist_index;		//¸¨ÖúµÀ¾ß
		};

		struct rune_decompose
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
		};

		struct rune_levelup
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
		};

		struct rune_open_slot
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
		};

		struct rune_change_slot
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
			int assist_index;
		};

		struct rune_erase_slot
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
			int stone_index;
		};

		struct tower_reward
		{
			cmd_header header;
			int raid_template_id;		
			size_t level;
			int type;	//ÖÕÉí£¬Ã¿ÈÕ
		};
	
		struct rune_install_slot
		{
			cmd_header header;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
			int slot_index;			//°²×°µÄslotÎ»ÖÃ
			int stone_index;		//°²×°µÄ·ûÎÄindex
			int stone_id;			//·ûÎÄid
		};

		struct rune_refine_action
		{
			cmd_header header;
			bool accept_result;
			int rune_index;			//Ôª»êÔÚ×°±¸À¸ÀïÃæµÄÎ»ÖÃ
		};

		struct get_task_award
		{
			cmd_header header;
			int type;	//1-²¹ÌîÉí·İĞÅÏ¢½±Àø  2-²¹ÌîÕËºÅÒÔ¼°ÓÊÏäĞÅÏ¢½±Àø 
		};

		struct pk_1st_guess
		{
			cmd_header header;
			int area;
			int cnt;
			struct bet_item
			{
				int index;
				int cnt;
			}items[];
		};

		struct pk_top3_guess
		{
			cmd_header header;
			bool cancel;
			int area;
			int cnt;
			struct bet_item
			{
				int index;
				int cnt;
			}items[];
		};

		struct pk_1st_guess_reward
		{
			cmd_header header;
		};
		
		struct pk_1st_guess_result_reward
		{
			cmd_header header;
		};

		struct pk_top3_guess_reward
		{
			cmd_header header;
		};

		struct pk_top3_guess_result_reward
		{
			cmd_header header;
			char type; //0, È«ÖĞ; 1, ÖĞ2¸ö; 2, ÖĞ1¸ö
		};

		struct get_player_bet_data
		{
			cmd_header header;
		};

		struct start_special_move
		{
			cmd_header header;
			A3DVECTOR velocity;		//µ±Ç°ËÙ¶È
			A3DVECTOR acceleration; 	//µ±Ç°¼ÓËÙ¶È
			A3DVECTOR cur_pos;
			char collision_state;		//0: Ã»ÓĞÅö×²   1:·¢ÉúÅö×² 
			int timestamp;
		};

		struct special_move
		{
			cmd_header header;
			A3DVECTOR velocity;		//µ±Ç°ËÙ¶È
			A3DVECTOR acceleration; 	//µ±Ç°¼ÓËÙ¶È
			A3DVECTOR cur_pos;		//µ±Ç°Î»ÖÃĞÅÏ¢
			unsigned short stamp;		//·¢ËÍÏûÏ¢Ê±¼ä´Á
			char collision_state;		//0: Ã»ÓĞÅö×²   1:·¢ÉúÅö×² 
			int timestamp;
		};

		struct stop_special_move
		{
			cmd_header header;
			unsigned char dir;			//Íæ¼Òµ±Ç°µÄ³¯Ïò
			A3DVECTOR cur_pos;		//µ±Ç°Î»ÖÃ
			unsigned short stamp;		//·¢ËÍÏûÏ¢Ê±¼ä´Á
		};

		struct collision_raid_apply
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			bool is_team;
			char is_cross;	//ÊÇ·ñ±¨Ãû¿ç·ş6vs6n
		};

		struct change_wing_color
		{
			cmd_header header;
			unsigned char wing_color;
		};

		struct astrology_identify
		{
			cmd_header header;
			int item_index;
			int item_id;
		};

		struct astrology_upgrade
		{
			cmd_header header;
			int item_index;
			int item_id;
			int stone_index;
			int stone_id;
		};

		struct astrology_destroy
		{
			cmd_header header;
			int item_index;
			int item_id;
		};

		struct talisman_refineskill_result
		{
			cmd_header header;
			int talisman1_index;
			int talisman1_id;
			char result;
		};

		struct get_collision_award
		{
			cmd_header header;
			bool daily_award;		//ÊÇ·ñÁìÈ¡Ã¿ÈÕ½±Àø
			int award_index;		//½±ÀøÎïÆ·¶ÔÓ¦µÄindex
		};

		struct cancel_ice_crust
		{
			cmd_header header;
		};

		struct remedy_metempsychosis_level
		{
			cmd_header header;
			int item_index;
		};
		
		struct merge_potion
		{
			cmd_header header;
			int bottle_id;
			int bottle_index;
			int potion_id;
			int potion_index;
		};

		struct kingdom_leave
		{
			cmd_header header;
		};

		struct get_collision_player_pos
		{
			cmd_header header;
			int roleid;
		};

		struct take_liveness_award
		{
			cmd_header header;
			int grade;  // 0, 1, 2, 3±íÊ¾µµ´Î
		};

		struct raid_level_award
		{
			cmd_header header;
			char level; //-1±íÊ¾final£¬0-9±íÊ¾¹Ø¿¨
		};

		struct get_raid_level_result
		{
			cmd_header header;
		};

		struct open_trashbox
		{
			cmd_header header;
			unsigned int passwd_size;
			char passwd[];
		};

		struct delivery_gift_bag
		{
			cmd_header header;
			int index;
			int item_id;
		};

		struct get_cash_gift_award
		{
			cmd_header header;
		};

		struct gen_prop_add_item
		{
			cmd_header header;
			int material_id;
			short material_idx;
			short material_cnt;
		};

		struct rebuild_prop_add_item
		{
			cmd_header header;
			int itemId;
			short itemIdx;
			int forgeId;	//¸¨ÖúµÀ¾ßID
			short forgeIdx;	//¸¨ÖúµÀ¾ßIdx
		};

		struct get_propadd
		{
			cmd_header header;
		};

		struct buy_kingdom_item
		{
			cmd_header header;
			char type;		//1-¹úÍõ  2-¸öÈË  
			int index;
		};

		struct king_try_call_guard 
		{
			cmd_header header;
		};

		struct get_touch_award
		{
			cmd_header header;
			char type;		//1-½ğ¹Ï×Ó 2-Òø¹Ï×Ó
			int count;
		};

		struct bath_invite
		{
			cmd_header header;
			int who;
		};

		struct bath_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct flow_battle_leave
		{
			cmd_header header;
		};

		struct deliver_kingdom_task
		{
			cmd_header header;
			int task_type;	//1-°×É« 2-ÂÌÉ« 3-À¶É« 4-³ÈÉ«
		};

		struct receive_kingdom_task
		{
			cmd_header header;
			int task_type;	//1-°×É« 2-ÂÌÉ« 3-À¶É« 4-³ÈÉ«
		};

		struct kingdom_fly_bath_pos
		{
			cmd_header header;
		};

		struct fuwen_compose
		{
			cmd_header header;
			bool consume_extra_item;	//ÊÇ·ñÏûºÄ¶îÍâµÀ¾ßÔö¼ÓºÏ³É´ÎÊı
			int extra_item_index;		//ÏûºÄµÄ¶îÍâµÀ¾ßindex
		};

		struct fuwen_upgrade
		{
			cmd_header header;
			int main_fuwen_index;		//Ö÷·ûÎÄ
			int main_fuwen_where;		//Ö÷·ûÎÄËùÔÚµÄ°ü¹ü
			int assist_count;
			int assist_fuwen_index[];		//¸¨Öú·ûÎÄ
		};

		struct fuwen_install
		{
			cmd_header header;
			int src_index;			//·ûÎÄÔÚ°ü¹üÀïÃæµÄÎ»ÖÃ
			int dst_index;			//Òª°²×°µ½·ûÎÄ°ü¹üÀïÃæµÄÄ¿±êÎ»ÖÃ (0¿ªÊ¼)
		};

		struct fuwen_uninstall
		{
			cmd_header header;
			int fuwen_index;		//·ûÎÄÔÚ·ûÎÄ°ü¹üÀïÃæµÄÎ»ÖÃ
			int assist_index;		//µÀ¾ßÔÚÆÕÍ¨°ü¹üÀïÃæµÄÎ»ÖÃ
			int inv_index;			//·ûÎÄ²ğ³ıºó·ÅÔÚ°ü¹üÊ²Ã´Î»ÖÃ
		};

		struct resize_inv
		{
			cmd_header header;
			char type;				//0: inv; 1: trashbox
			int  new_size;
			int  cnt;
			struct cost_item
			{
				int item_idx;
				int item_id;
				int item_cnt;
			}items[];
		};

		struct register_flow_battle
		{
			cmd_header header;
		};

		struct arrange_inventory
		{
			cmd_header header;
			char pageind; //0->ÕûÀíÈ«²¿
		};

		struct arrange_trashbox
		{
			cmd_header header;
			char pageind; //0->ÕûÀíÈ«²¿
		};

		struct arrange_pocket
		{
			cmd_header header;
		};

		struct get_web_order
		{
			cmd_header header;
			int64_t order_id;
		};

		struct customize_rune
		{
			cmd_header header;
			int type;
			int count;	//ÊıÁ¿
			int prop[];
		};

		struct get_cross_server_battle_info
		{
			cmd_header header;
			int map_id;
		};

		struct control_trap
		{
			cmd_header header;
			int id;
			int tid;
		};

		struct summon_teleport
		{
			cmd_header header;
			int npc_id;
		};

		struct create_crossvr_team
		{
			cmd_header header;
			int name_len;
			char team_name[];
		};

		struct get_king_reward
		{
			cmd_header header;
		};

		struct mobactive_start
		{
			cmd_header header;
			int npc_id;
			int tool_type;
			int task_id;
		};

		struct mobactive_finish 
		{
			cmd_header header;
		};

		struct qilin_invite
		{
			cmd_header header;
			int who;
		};

		struct qilin_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct qilin_cancel
		{
			cmd_header header;
		};

		struct qilin_up
		{
			cmd_header header;
		};

		struct qilin_down
		{
			cmd_header header;
		};

		struct enter_faction_base
		{
			cmd_header header;
		};

		struct add_fac_building
		{
			cmd_header header;
			int fid;	//°ïÅÉ id
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
			int building_tid; //½¨ÖşµÄÄ£°å id
		};
		struct upgrade_fac_building
		{
			cmd_header header;
			int fid;
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
			int building_tid; //½¨ÖşµÄÄ£°å id
			int cur_level; //µ±Ç°µÈ¼¶
		};
		struct remove_fac_building
		{
			cmd_header header;
			int fid;
			int field_index; //µØ¿éË÷Òı ´Ó 1 ¿ªÊ¼±àºÅ
			int building_tid; //½¨ÖşµÄÄ£°å id
		};
		struct post_fac_base_msg
		{
			cmd_header header;
			int fid;
			size_t msg_len;
			char msg[];
		};
		struct pet_change_shape
		{
			cmd_header header;
			int pet_index;
			int shape_id; //1-7
		};

		struct pet_refine_attr
		{
			cmd_header header;
			int pet_index;
			int attr_type; // 0 - 14
			int assist_id;
			int assist_count; //1 10 100
		};

		struct active_emote_action_invite
		{
			cmd_header header;
			int who;
			int type;
		};

		struct active_emote_action_invite_reply
		{
			cmd_header header;
			int who;
			int type;
			int param;
		};

		struct active_emote_action_cancel
		{
			cmd_header header;
		};

		struct get_raid_transform_task
		{
			cmd_header header;
		};


		struct get_fac_base_info
		{
			cmd_header header;
		};

		struct get_cs_6v6_award
		{
			cmd_header header;
			int award_type;	//0-Ã¿ÖÜ·Öµµ½±Àø 1-Ã¿ÖÜ¶Ò»»±Ò½±Àø 2-Õ½¶ÓÈü¼¾½±Àø
			int award_level; //½±ÀøµÄµµÎ»
		};

		struct exchange_cs_6v6_award
		{
			cmd_header header;
			int index;
			int item_id;
			int currency_id;
		};

		struct exchange_cs_6v6_money
		{
			cmd_header header;
			int type;	//0: ½ğ¶Ò»»Òø  1:½ğ¶Ò»»Í­  2:Òø¶Ò»»Í­
			int count;	//²Î¼Ó¶Ò»»µÄÊıÁ¿
		};

		struct puppet_form_change
		{
			cmd_header header;
		};

		struct fac_base_transfer
		{
			cmd_header header;
			int region_index;
		};

		struct withdraw_facbase_auction
		{
			cmd_header header;
			int item_index;
		};

		struct withdraw_facbase_coupon
		{
			cmd_header header;
		};

		struct get_facbase_auction
		{
			cmd_header header;
		};

		struct leave_faction_base
		{
			cmd_header header;
		};

		struct object_charge_to
		{
			cmd_header header;
			int type;
			int target_id;
			A3DVECTOR pos;
		};

		struct stop_travel_around
		{
			cmd_header header;
		};

		struct get_summon_petprop 
		{
			cmd_header header;
		};

		struct get_seek_award
		{
			cmd_header header;
		};

		struct seeker_buy_skill
		{
			cmd_header header;
		};

		struct hide_and_seek_raid_apply
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			bool is_team;
		};

		struct capture_raid_submit_flag
		{
			cmd_header header;
		};

		struct capture_raid_apply
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			bool is_team;
		};

/*------------------------------ÄÚ²¿GM ÃüÁî------------------------------------*/		
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

		struct gmcmd_movetomap
		{
			cmd_header header;
			int world_tag;
			float x;
			float y;
			float z;	
		};
	}
}


#pragma pack()
#endif

