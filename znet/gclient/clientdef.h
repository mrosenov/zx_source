#ifndef __CLIENT_COMMON_DEFIND_H__
#define __CLIENT_COMMON_DEFIND_H__ 

#include "ABaseDef.h"
#include "types.h"
#include "gpdatatype.h"

#pragma pack(1)
namespace C2S
{      
	#define FLOATTOFIX8(x)		((short)((x) * 256.0f + 0.5f)) 
/*	
	//	Commands ----------------------------
	enum
	{
		PLAYER_MOVE,		//	0
		LOGOUT,
		SELECT_TARGET,
		NORMAL_ATTACK,
		REVIVE_VILLAGE,		//	Revive in near village

		REVIVE_ITEM,		//	5, Revive by using item
		PICKUP,
		STOP_MOVE,
		UNSELECT,
		GET_ITEM_INFO,

		GET_IVTR,			//	10, Get inventory information
		GET_IVTR_DETAIL,	//	Get inventory detail information
		EXG_IVTR_ITEM,
		MOVE_IVTR_ITEM,
		DROP_IVTR_ITEM,

		DROP_EQUIP_ITEM,	//	15
		EXG_EQUIP_ITEM,
		EQUIP_ITEM,
		MOVE_ITEM_TO_EQUIP,
		GOTO,
		
		THROW_MONEY,		//	20
		GET_EXT_PROP,
		LEARN_SKILL,
		GET_EXT_PROP_BASE,
		GET_EXT_PROP_MOVE,

		GET_EXT_PROP_ATK,	//	25
		ASSIGN_STATUS_POINT,
		TEAM_INVITE,
		TEAM_AGREE_INVITE,
		TEAM_REJECT_INVITE,

		TEAM_LEAVE_PARTY,	//	30
		TEAM_KICK_MEMBER,
		TEAM_MEMBER_POS,	//	Get team member's position
		GET_OTHER_EQUIP,
		TEAM_SET_PICKUP,	//	Change team pickup flag

		SEVNPC_HELLO,		//	35, say hello to service NPC
		SEVNPC_GET_CONTENT,
		SEVNPC_SERVE,
		GET_OWN_WEALTH,
		GET_ALL_DATA,		//  取得所有数据 进入游戏时使用，传回所有的包裹，金钱和技能

		USE_ITEM,			//	40, 使用一个物品
		CAST_SKILL,
		CANCEL_ACTION,
		CHARGE_E_FLYSWORD,	//	Charge flysword which is on equipment bar
		CHARGE_FLYSWORD,

		USE_ITEM_T,			//	45, use item with target
        SIT_DOWN,
        STAND_UP,
        EMOTE_ACTION,
        TASK_NOTIFY,

		ASSIST_SELECT,		//	50
		CONTINUE_ACTION,
		STOP_FALL,			//	终止跌落
		GET_ITEM_INFO_LIST,
		GATHER_MATERIAL,

		GET_TRASHBOX_INFO,	//	55
		EXG_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,
		EXG_TRASHBOX_IVTR,
		TRASHBOX_ITEM_TO_IVTR,

		IVTR_ITEM_TO_TRASHBOX,	//	60
		EXG_TRASHBOX_MONEY,
		TRICK_ACTION,
        SET_ADV_DATA,
        CLR_ADV_DATA,

        TEAM_ASK_TO_JOIN,		//	65
        TEAM_REPLY_JOIN_ASK,
		QUERY_PLAYER_INFO_1,
		QUERY_NPC_INFO_1,
		SESSION_EMOTE,

		CON_EMOTE_REQUEST,		//	70
		CON_EMOTE_REPLY,
		CHANGE_TEAM_LEADER,		
		DEAD_MOVE,
		DEAD_STOP_MOVE,

        ENTER_SANCTUARY,		//	75
		OPEN_BOOTH,
		CLOSE_BOOTH,
		QUERY_BOOTH_NAME,
		COMPLETE_TRAVEL,

		CAST_INSTANT_SKILL,		//	80
		DESTROY_ITEM,
		ENABLE_PVP_STATE,
		DISABLE_PVP_STATE,
		OPEN_BOOTH_TEST,

		SWITCH_FASHION_MODE,	//	85
		ENTER_INSTANCE,
		REVIVAL_AGREE,
		NOTIFY_POS_IN_TEAM,
		CAST_POS_SKILL,

		ACTIVE_RUSH_FLY,		//	90
		QUERY_DOUBLE_EXP,
		DUEL_REQUEST,
		DUEL_REPLY,
		BIND_PLAYER_REQUEST,

		BIND_PLAYER_INVITE,		//	95
		BIND_PLAYER_REQUEST_REPLY,
		BIND_PLAYER_INVITE_REPLY,
		CANCEL_BIND_PLAYER,
		GET_OTHER_EQUIP_DETAIL,

		SUMMON_PET,				//	100
		RECALL_PET,
		BANISH_PET,
		PET_CTRL_CMD,
		PRODUCE_ITEM,

		MALL_SHOPPING,			// 105
		SELECT_TITLE,
		DEBUG_DELIVER_CMD,
		DEBUG_GS_CMD,			
		LOTTERY_CASHING,

		CHECK_RECORD,			//	110
		MALL_SHOPPING2,
		START_MOVE,
		USE_ITEM_WITH_ARG,
		BOT_BEGIN,

		BOT_RUN,				//	115
		GET_BATTLE_SCORE,
		GET_BATTLE_INFO,
		SWITCH_FAST_FLY,
		EQUIP_PET_BEDGE,

		EQUIP_PET_EQUIP,		//	120
		COMBINE_PET,
		UNCOMBINE_PET,
		CHANGE_SHAPE,
		SELECT_TALENT,

		EQUIP_UPGRADE,			//	125
		MOVE_PET_BEDGE,
		CHANGE_PET_NAME,
		CHANGE_FATE,
		LEARN_PET_SKILL,

		FORGET_PET_SKILL,		//	130
		PET_SWALLOW_ITEM,
		IDENTIFY_PET,
		RESURRECT_PET,
		SET_PET_EXP_RATIO,
		
		FORGET_PRODUCE_SKILL,	//	135
		SET_PET_INSIGHT_SKILL,
		MOVE_ITEM_BETWEEN_INVENTORY_SPEC_TRASH_BOX,
		EXCHANGE_INVENTORY_SPEC_TRASH_BOX,
		PLAYER_TEAM_RECRUIT,

		FAST_SUMMON_PET,		// 140
		FAST_COMBINE_PET,
		QUERY_FACTION_TERRITORY_INFO,
		CHALLENGE_CITY,	    	// 帮主报名城战
		OPEN_CITYBATTLE,		// 挑战帮帮主试图开启城战
		
		ENTER_CITYBATTLE,		//  145 玩家请求进入城战
        GET_CITYBATTLE_INFO,    //  请求城战帮主控制的资源，按钮相关信息
        CITYBATTLE_PRESS_BUTTON,//  城战某方帮主按下了某按钮
		SPECIAL_HORN,           //  特殊号角发言
		QUERY_TERRITORY_DOMINANCE, //请求领土占领度列表

		GET_MALL_ITEM_PRICE,    //  150 询问商城物品价格
		AUTO_GATHER_BEGIN,      //  真理之手开始自动采集
		PETBOX_MOVE_PET,        //  与宠物盒子交换宠物牌
		CAST_INSTANT_SKILL_WITH_BLINK,
		BIRTH_SKILL,            //  生辰技能
		
		CMD_GARDEN_EXCHANGE,    //  155 宠物家园与现实物品兑换请求
		EMBED_RUNE,             // 镶嵌符文                          
		REMOVE_RUNE,            // 移除符文
		ACTIVATE_RUNE_WORD,     // 激活符文之语
		MERGE_ANYWHERE,         // 随身八卦炉

		RAID_LEAVE,				//  160 玩家主动离开
		GET_RAID_INFO,			// 获得副本信息
		QUERY_RAID_ENTER_COUNT,	// 获取副本进入次数   QUERY_RAID_ENTER_COUNT		


		NUM_C2SCMD,				//	Number of C2S commands.

		//	Below are GM commands
		GM_CMD_START = 200,		//	200
		GM_MOVETO_PLAYER,
		GM_CALLIN_PLAYER,
		GM_KICK_PLAYER,			//	需要选中目标,无参数
		GM_INVISIBLE,			//	切换自身隐身,无参数

		GM_INVINCIBLE,			//	205, 切换自身无敌,无参数
		GM_GENERATE,
		GM_ACTIVE_SPAWNER,
		GM_GENERATE_MOB,
	};
*/
	//	Move environment
	enum
	{
		MOVEENV_GROUND = 0,	//	Move on ground
		MOVEENV_WATER,
		MOVEENV_AIR,
	};
	//	Move mode
	enum
	{
		GP_MOVE_WALK	= 0,
		GP_MOVE_RUN		= 1,
		GP_MOVE_STAND	= 2,
		GP_MOVE_FALL	= 3,
			GP_MOVE_SLIDE	= 4,
		GP_MOVE_PUSH	= 5,
		GP_MOVE_FLYFALL	= 6,
		GP_MOVE_RETURN	= 7,
		GP_MOVE_JUMP	= 8,
		GP_MOVE_QINGGONG= 9,
		GP_MOVE_MASK	= 0xf,

		GP_MOVE_TURN	= 0x10,		//	Turnaround
		GP_MOVE_DEAD	= 0x20,

		GP_MOVE_AIR		= 0x40,
		GP_MOVE_WATER	= 0x80,
		GP_MOVE_ENVMASK	= 0xc0,
	};

	enum EPlayerStatus
	{
		EPLAYER_STATUS_CLOSE			= 0,

		EPLAYER_STATUS_WAITINITPOS,
		EPLAYER_STATUS_WAITMOVESTART,
		EPLAYER_STATUS_WAITMOVEEND,
		EPLAYER_STATUS_WANDER,
	};
/*
        struct cmd_header
	{
        	unsigned short cmd;
	};
*/
	struct cmd_player_move
        {
                A3DVECTOR vCurPos;
                A3DVECTOR vNextPos;
                unsigned short use_time;
                short sSpeed;                           //      Move speed 8.8 fix-point
                unsigned char move_mode;        //      Walk run swim fly .... walk_back run_back
                unsigned short stamp;           //      move command stamp
        };

	struct cmd_stop_move
	{
		A3DVECTOR vCurPos;
		short sSpeed;				//	Moving speed in 8.8 fix-point
		unsigned char dir;			//	对象的方向
		unsigned char move_mode;	//	Walk run swim fly .... walk_back run_back
		unsigned short stamp;		//	move command stamp
		unsigned short use_time;
	};
	
        struct cmd_jump_move
        {
                int tag;
                int x;
        	int y;
                int z;
        };
}
#pragma pack()

#endif
