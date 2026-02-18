#ifndef __GNET_LOCALMACRO_H
#define __GNET_LOCALMACRO_H

#include "groleinventory"
#include "gshoplog"
#include "gmailsyncdata"
#include "familyid"
#include "hostilefaction"
#include "hostileinfo"
#include "gfactioninfo"

#ifdef USE_LOG2 
#include "logger.hpp"
#endif

#define  _ROLE_STATUS_NORMAL    1
#define  _ROLE_STATUS_MUSTDEL   2
#define  _ROLE_STATUS_READYDEL  3
#define  _ROLE_STATUS_FROZEN	4
#define  _ROLE_STATUS_SELLING	5

#define  ERR_NO_LINE_CHGS       0

namespace GNET
{

// 寻宝网寄售角色过程中

	
// 寻宝网角色登记期
#define WEBTRADE_ROLE_REGISTER_PERIOD 	3*24*3600
// 寻宝网角色寄售时限
#define WEBTRADE_ROLE_POST_TIME_LIMIT 	6*24*3600
// 寻宝网寄售转生限制
#define WEBTRADE_ROLE_SOLD_REBORN_LIMIT  2
// 寻宝网寄售等级限制
#define WEBTRADE_ROLE_SOLD_LEVEL_LIMIT  90
// 寻宝网买家最高等级
#define WEBTRADE_ROLE_SOLD_BUYER_LEVEL_LIMIT 90

#define LOGICUID_START    1024
#define LOGICUID_MAX      0x7FFFFFFF 
#define LOGICUID(rid)     (int)((rid) & 0xFFFFFFF0)
#define MAX_NAME_SIZE     40
//#define MAX_ROLENAME_RECORD	10	//每个角色保留最新10次改名记录
#define ROLENAME_PAGESIZE 100

#define CRSSVRTEAMS_MAX_ROLES 8 // 跨服战队最大人数
//#define CRSSVRTEAMS_APPLAY_ROLES 6 // 跨服战队报名人数
#define CRSSVRTEAMS_INIT_ROLE_SCORE 1000 // 玩家战队初始积分
#define CRSSVRTEAMS_CALC_SCORE_LOW 1500 // 小于这个数，结算不扣分

const static int HIDESEEK_MEMBER_LIMIT = 20; // 捉迷藏副本开启人数
const static int FENGSHEN_MEMBER_LIMIT = 12; // 封神副本开启人数

const static int CRSSVRTEAMS_SEASON_CLOSE_DAY = 26; 
const static int CRSSVRTEAMS_SEASON_OPEN_DAY = 1;
const static int CRSSVRTEAMS_SEASON_REOPEN_DAY = 31;

#define NEW_MAX_CASH_IN_POCKET 	2000000000  //包裹内钱上线扩充
#define NEW_MAX_CASH_IN_STOCK	1000000000	//钱庄中能存的钱的上限

#define MAX_USER_CASH           2000000000
#define CASH_BALANCE_THRESHOLD  10000000
#define STOCK_TAX_RATE    1.02
#define STOCK_BALANCE_ID  1

#define UNIQUENAME_ENGAGED      1
#define UNIQUENAME_USED         2
#define UNIQUENAME_OBSOLETE     3  //角色改名 使用过的名字
#define UNIQUENAME_PREALLOCATE	4  //预先分配的名字 不允许玩家再取

#define SYSMAIL_LIMIT		64   //系统邮件数量上限
const static int OCCUPATION_PLAYER_LIMIT=2; // 6v6中每种职业不能超过2个

enum{
	ALGORITHM_MASK_HIGH    = 0x000F0000,
	ALGORITHM_MASK_OPT     = 0x0000FF00,
	ALGORITHM_OPT_NOCACHE  = 0x00000100,
	ALGORITHM_NONE         = 0x0,
	ALGORITHM_CARD         = 0x00010000,
	ALGORITHM_HANDSET      = 0x00020000,
	ALGORITHM_USBKEY2      = 0x00050000,
	ALGORITHM_PHONETOKEN   = 0x00060000,
	ALGORITHM_SINGLE_SING_ON =  0x00001000,
	ALGORITHM_OPT_STUDIO_USER = 0x00100000,
	ALGORITHM_FILL_INFO_MASK  = 0x000000FF, //补填提示与奖励信息
};

enum{
	CASH_GETSERIAL_FAILED = -16,
	CASH_ADD_FAILED       = -17,
	CASH_NOT_ENOUGH       = -18
};

enum{
	ITEM_PROC_TYPE_NOTRADE   = 0x00000010,   //玩家间不能交易
	ITEM_PROC_TYPE_CD_TRADE  = 0x00000400,	 //(Central Deliveryd)跨服服务器上可以交易
	ITEM_PROC_TYPE_LOCKED    = 0x80000000,   //锁定
};
#define MASK_ITEM_NOTRADE  (ITEM_PROC_TYPE_NOTRADE|ITEM_PROC_TYPE_LOCKED)

enum{
	SYNC_STOTEHOUSE  = 0x01,
	SYNC_CASHUSED    = 0x02,
	SYNC_CASHTOTAL   = 0x04,
	SYNC_SHOPLOG     = 0x08,
};
#define TOP_DEITY_NUM 4
enum{
	TOP_PERSONAL_LEVEL = 1,
	TOP_PERSONAL_MONEY = 2,
	TOP_PERSONAL_REPUTATION = 3,
	TOP_PERSONAL_CREDIT_START = 10,

	TOP_FACTION_LEVEL = 31,
	TOP_FACTION_MONEY = 32,
	TOP_FACTION_POPULATION = 33,
	TOP_FACTION_PROSPERITY = 34,
	TOP_FACTION_NIMBUS  = 35,
	TOP_FACTION_ACTIVITY = 36,
	TOP_FACTION_CREDIT_START = 40,

	TOP_FAMILY_TASK_START = 60,

	TOP_PERSONAL_CHARM= 80,
	TOP_FRIEND_CHARM = 81,
	TOP_SECT_CHARM = 82,
	TOP_FACTION_CHARM = 83,

	TOP_PERSONAL_DEITY_LEVEL_START = 90,
	TOP_PERSONAL_DEITY_LEVEL_END = TOP_PERSONAL_DEITY_LEVEL_START+TOP_DEITY_NUM,

	TOP_TOWERRAID_LEVEL = 95,
	TOP_CRSSVRTEAMS_SCORE = 96,
	TOP_PERSONAL_CREDIT_START2 = 200,
	TOP_FACTION_CREDIT_START2 = 300,
	TOP_TABLEID_MAX = TOP_FACTION_CREDIT_START2+100, //预留了100个声望 id
};

//#define FIRST_TOPTABLE_ID     (TOP_PERSONAL_LEVEL);  liuyue
//#define LAST_TOPTABLE_ID      (TOP_FAMILY_TASK_START+8)
#define LAST_TOPTABLE_ID      (TOP_TABLEID_MAX-1)
#define TOP_ITEM_PER_PAGE     20
//#define REGION_COUNT          15
#define TOPTABLE_COUNT        71
#define WEEKLYTOP_BEGIN       1000

enum ERR_TOP_TABLE{
	TOP_DATE_NOTREADY = 1,
	TOP_INVALID_ID = 2,
};

enum ERR_COMBAT
{
        ERR_COMBAT_MASTEROFFLINE = 1,
        ERR_COMBAT_NOPROSPERITY  = 2,
        ERR_COMBAT_COOLING       = 3,
        ERR_COMBAT_BUSY          = 4,
        ERR_COMBAT_LOWLEVEL      = 5,
	ERR_COMBAT_INBATTLE      = 6,
};

enum ERR_STOCK
{
        ERR_STOCK_CLOSED        = 1, 
        ERR_STOCK_ACCOUNTBUSY   = 2, 
        ERR_STOCK_INVALIDINPUT  = 3,
        ERR_STOCK_OVERFLOW      = 4,
        ERR_STOCK_DATABASE      = 5, 
        ERR_STOCK_NOTENOUGH     = 6, 
        ERR_STOCK_MAXCOMMISSION = 7, 
        ERR_STOCK_NOTFOUND      = 8, 
	ERR_STOCK_CASHLOCKED    = 9,
	ERR_STOCK_CASHUNLOCKFAILED= 10,
};
enum ORDER_RESULT
{
        STOCK_ORDER_DONE        = 0, 
        STOCK_ORDER_CANCEL      = 1, 
        STOCK_ORDER_TIMEOUT     = 2,
};

enum OPER_ENEMY{
	ENEMY_REMOVE = 0,
	ENEMY_FREEZE = 1,
	ENEMY_INSERT = 2
};
enum GET_ENEMY{
	ENEMY_ONLINE   = 0,
	ENEMY_FULLLIST = 1,
	ENEMY_NEW      = 2,
	ENEMY_IDLIST   = 3
};
enum
{
	MSG_BIDSTART             = 1,  // 开始竞价
	MSG_BIDEND               = 2,  // 竞价结束
	MSG_BATTLESTART          = 3,  // 城战开始
	MSG_BATTLEEND            = 4,  // 城战结束
	MSG_BIDSUCCESS           = 5,  // 竞价成功
	MSG_BONUSSEND            = 6,  // 领土收益发送
	MSG_MARRIAGE             = 10, // 结婚
	MSG_DIVORCE              = 11, // 离婚
	MSG_COMBATCHALLENGE      = 12, // 野战挑战
	MSG_COMBATSTART          = 13, // 野战开始
	MSG_COMBATREFUSE         = 14, // 拒绝野战邀请
	MSG_COMBATEND            = 15, // 野战结束
	MSG_COMBATTIMEOUT        = 16, // 野战邀请超时
	MSG_CITYNOOWN            = 17, // 帮派降级后城市无主
	MSG_BATTLE1START         = 18, // 有领土对有领土帮派城战开始
	MSG_BATTLE2START         = 19, // 无领土对有领土帮派城战开始 
	MSG_FAMILYDEVOTION       = 20, // 获得家族贡献度
	MSG_FAMILYSKILLABILITY   = 21, // 家族技能等级变化
	MSG_FAMILYSKILLEVEL      = 22, // 家族技能熟练度变化
	MSG_FACTIONNIMBUS        = 23, // 帮派灵气变化
	MSG_TASK                 = 24, // 任务喊话
	MSG_BATTLE               = 25, // 城战喊话
	MSG_BATTLEWILLSTART      = 26, // 战场将在一分钟后开启
	MSG_SIEGEOWNER		 = 27, // 守方通知
	MSG_SIEGEATTACKER	 = 28, // 攻方通知
	MSG_SIEGEASSISTANT  	 = 29, // 辅助方通知
	MSG_SIEGEBUILD  	 = 30, // 战场建设信息
	MSG_SIEGEBROADCAST  	 = 31, // 全服广播战场开始
	MSG_SIEGEFACTIONCAST 	 = 32, // 帮派广播战场开始
	MSG_CONTESTTIME          = 33, // 答题时间通知
	MSG_CONTESTEND           = 34, // 答题结束
	MSG_SNSPRESSMSG		 = 35, // 交友平台发布消息
	MSG_SNSACCEPTAPPLY	 = 36, // 交友平台发布者接受申请
	MSG_TERRI_BID		 = 37, // 领土战宣战公告 type
	MSG_TERRI_START		 = 38, // 领土战开启
	MSG_TERRI_AWARD		 = 39, // 领土战神器奖励  territoryid itemid
	MSG_UNIQUEAUCTION_BEGIN	 = 40, // 唯一价竞拍开始前30分钟 win_itemid
	MSG_UNIQUEAUCTION_LAST	 = 41, // 唯一价竞拍最后提示
	MSG_UNIQUEAUCTION_END	 = 42, // 唯一价竞拍结束 winner_id winner_name price itemid
	MSG_UNIQUEAUCTION_START  = 43, // 唯一价竞拍开始 win_itemid
	MSG_TERRI_END		 = 44, // 领土战结束 territoryid,defener_id(守方id,为0表示无守方),defender_name,attacker_id,attacker_name,result(int:1:攻方胜2:守方胜),time(int:0:慢速结束1:快速结束)
	MSG_KINGDOM_FIRSTHALF_END   = 45, //国战第一阶段结束 res(char类型1守方胜2攻方胜),defender(守方帮派id),defender_name,attacker_name1,attacker_name2,attacker_name3,attacker_name4
	MSG_KINGDOM_NEW_WINNER	    = 46, //国战第二阶段结束 win_fac_name(获胜帮派名),(新国王名)
	MSG_KINGDOM_ANNOUNCE	    = 47, //国王宣旨广播 msg 字段直接为圣旨内容 无需 unmarshal
	MSG_KINGDOM_SET_ATTR	    = 48, //国王开启服务器权限 flag(int:1三倍经验2双倍掉落),cost(消耗的积分)
	MSG_KINGDOM_APPOINT	    = 49, //国王封职务 title(char),rolename
	MSG_KINGDOM_OPEN_BATH	    = 50, //天后开启泡澡 queenname
	MSG_KINGDOM_ISSUE_TASK      = 51, //天帝发布任务 kingname, (int)task_type
};

enum CHGS_ERR
{
	ERR_CHGS_SUCCESS         = 0,
	ERR_CHGS_INVALIDGS       = 1,	//不存在该gs号
	ERR_CHGS_MAXUSER         = 2,	//目的gs人数达到上限
	ERR_CHGS_NOTINSERVER     = 3,	//用户切换gs时不在服务器内
	ERR_CHGS_STATUSINVALID   = 4,	//用户切换gs时状态不对
	ERR_CHGS_NOTGM           = 5,	//用户不是gm
	ERR_CHGS_MAPIDINVALID    = 6,	//地图不存在
	ERR_CHGS_SCALEINVALID    = 7,	//非法坐标
	ERR_CHGS_DBERROR         = 8,	//数据库错误
};

enum {
	TITLE_FREEMAN    = 0, //非家族成员
	TITLE_SYSTEM     = 1, //无需显示
	TITLE_MASTER     = 2, //帮主
	TITLE_VICEMASTER = 3, //副帮主
	TITLE_CAPTAIN    = 4, //长老
	TITLE_HEADER     = 5, //舵主
	TITLE_MEMBER     = 6, //普通成员
}; //end of Roles

//ggated related
enum ACTIVATETYPE
{
	ACTIVATE_NONE = 0,
	ACTIVATE_GT = 1,//bit 1
	ACTIVATE_SNS = 2,//bit 2
};
//gt enums
enum ERR_GT
{
	ERR_ROLEREMOVED = 1,
	ERR_HASNOT_ACTIVATED,
	ERR_HAS_ACTIVATED,
	ERR_GT_DB_FAILURE,
	ERR_GT_DATANOTFIND,
};
enum FType
{
	FACTIONTYPE=0,
	FAMILYTYPE,
};
enum GroupType
{
	FRIENDTYPE=0,
};
enum RouteType
{
	FROM_GT=0,
	FROM_GAME,
	TO_GT,
	TO_GAME,
	TO_GT_AND_GAME,
};
/*enum CHAT_SRC_ROUTE
{
	FROM_GAME=0,
	FROM_GT,
};*/
enum FriendOperation
{
	FRIEND_ADD=0,
	FRIEND_DEL,
	FRIEND_MOVE,
};
enum GroupOperation
{
	OP_ADD=0,
	OP_DEL,
	OP_MOD,
};
enum FactionOperation
{
	MEMBER_ADD=0,
	MEMBER_DEL,
	MEMBER_MOVE,
};
enum FactionInfoOperation
{
	FAC_UPDATE_NAME=1,
	FAC_UPDATE_ANNOUNCE=2,
	FAC_UPDATE_EXT=32768,
};
/*enum GameOnlineSyncStatus
{
	SYNC_START = 0,
	SYNC_CONTINUE,
	SYNC_FINISH,
};*/
enum AnnounceRespCode
{
	RET_OK=0,
	RET_INVALID_ZONE,
	RET_DUPLICATE,
	RET_MULTIZONE,
};
enum GT_Role_Change 
{
	UPDATE_NAME=1,
	UPDATE_GENDER=2,
	UPDATE_RACE=4,
	UPDATE_OCCUPATION=8,
	UPDATE_LEVEL =16,
	//UPDATE_ADD_NEW_ROLE=32,
	UPDATE_EXT=64,
	UPDATE_ROLE_CREATE = 32768,
};
/* GT enums end*/
// Circle enums
enum CircleTitle{
	CIRCLE_MEMBER = 0,
	CIRCLE_MASTER = 1,
	CIRCLE_BOY = 2,
	CIRCLE_GIRL = 4,
};
enum Jointype{
	JOIN_CIRCLE = 1,
	LEAVE_AND_JOIN_CIRCLE,
	LEAVE_CIRCLE,
};
enum CircleMsgType{
	CIRCLE_JOIN = 1,
	CIRCLE_GRADUATE,
	CIRCLE_APPOINT_MASTER,
	CIRCLE_APPOINT_BOY,
	CIRCLE_APPOINT_GIRL,
	CIRCLE_POINT_OVER_THRESHOLD,
};

//Circle end
enum ERR_GLOBAL_DROP_COUNTER
{
	ERR_DROP_COUNTER_TIME=1,
	ERR_DROP_COUNTER_ARGU,
	ERR_DROP_COUNTER_NUMBER,
};

//Consign start
enum CONSIGN_TYPE //consist with Post::posttype
{
	CONSIGNTYPE_MONEY=1,
	CONSIGNTYPE_ITEM=2, 
//	CONSIGNTYPE_PET=3, //pet is item
	CONSIGNTYPE_ROLE=4,
};
enum CONSIGN_INFORM //mail type
{
	_CONSIGN_BUY,
	_CONSIGN_SOLD, 
	_CONSIGN_CANCEL,
	_CONSIGN_FORCED_CANCEL,
	_CONSIGN_EXPIRE,
	_CONSIGN_FAIL,
};
enum CONSIGNROLE_INFORM //mail type
{
	_CONSIGNROLE_POST,
};
enum ConsignState
{
	DSTATE_PRE_POST=0,					//预寄售,未与平台同步
	DSTATE_POST,					//寄售成功,与平台同步完毕,下架
	DSTATE_PRE_CANCEL_POST,				//预取消寄售,未与平台同步
//	DSTATE_SHOW,						//公示
	DSTATE_SELL,						//上架
	DSTATE_SOLD, //tricky,all finish state be put behind this one
	DSTATE_EXPIRE,
	DSTATE_POST_FAIL,
	DSTATE_POST_WEB_CANCEL,
	DSTATE_POST_GAME_CANCEL,
	DSTATE_POST_FORCE_CANCEL,
};
enum ConsignPostType
{
	POST_NORMAL,
	POST_RESTORE,
};
enum CheckStateChangeRes
{
	CHANGE_FORBID = 0,
	CHANGE_PERMIT,
	CHANGE_FORCED,
	CHANGE_REPEAT,
	CHANGE_IGNORE,
};
enum ConsignMailStatus
{
	NOT_SEND = 0,
	BUYER_SENDED,
	SELLER_SENDED,
	ALL_SENDED,
};
enum ERR_WEBTRADE
{
	ERR_WT_UNOPEN = 1,
	ERR_WT_ENTRY_NOT_FOUND,
	ERR_WT_ENTRY_IS_BUSY,
	ERR_WT_SN_ROLEID_MISMATCH,
	ERR_WT_TIMESTAMP_OLD,
	ERR_WT_TIMESTAMP_EQUAL,
	ERR_WT_TIMESTAMP_MISMATCH,
	ERR_WT_TIMESTAMP_ERR_SEQUENCE,
	ERR_WT_BUYER_NOT_EXIST,
	ERR_WT_TOO_MANY_ATTEND_SELL,
	ERR_WT_MAILBOX_FULL,//11
	ERR_WT_BUYERNAME_INVALID,
	ERR_WT_DB_FAILURE,
	ERR_WT_DB_STATEERR,
	ERR_WT_DB_KEYZERO,
	ERR_WT_DB_INVALID_ARGUMENT,
	ERR_WT_TIMEOUT,
	ERR_WT_GCONSIGNSTART_PROCESSING,
	ERR_WT_DB_DUPLICATE_RQST,  //occur when db write success but delivery timeout
	ERR_WT_STATEERR,
	ERR_WT_DUPLICATE_RQST,//21
	ERR_WT_PREPOST_ARG_ERR,
	ERR_WT_BUYER_SAMEUSER,
	ERR_WT_ROLE_IS_BUSY,
	ERR_WT_INVALID_ARGUMENT,
	ERR_WT_SELL_ROLE_WHILE_ITEM_SELLING,
	ERR_WT_SOLD_ROLE_STATUS_ERR,
	ERR_WT_SOLD_ROLE_BUYER_COND,//sect or spouse
	ERR_WT_SOLD_ROLE_SELLER_NOT_EXIST,
	ERR_WT_SELL_ROLE_WHILE_FORBID,
	ERR_WT_SELL_ROLE_HAS_FAC_RELATION, // 31
	ERR_WT_SELL_ROLE_HAS_SECT_RELATION,
	ERR_WT_SELL_ROLE_HAS_SPOUSE_RELATION,
	ERR_WT_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION,
};
enum ERR_WEBTRADE_CLIENT
{
	ERR_WTC_UNOPEN = 1,
	ERR_WTC_ENTRY_NOT_FOUND,
	ERR_WTC_ENTRY_IS_BUSY,
	ERR_WTC_SN_ROLEID_MISMATCH,
	ERR_WTC_TOO_MANY_ATTEND_SELL,
	ERR_WTC_MAILBOX_FULL,
	ERR_WTC_BUYERNAME_INVALID,
	ERR_WTC_TIMEOUT,
	ERR_WTC_GCONSIGNSTART_PROCESSING,
	ERR_WTC_STATEERR,
	ERR_WTC_DUPLICATE_RQST,//11
	ERR_WTC_PREPOST_ARG_ERR,
	ERR_WTC_DB_ERR,
	ERR_WTC_INNER,
	ERR_WTC_BUYER_SAMEUSER,
	ERR_WTC_LISTALL_ARG_ERR,
	ERR_WTC_LISTALL_FINISH,
	ERR_WTC_SELL_ROLE_WHILE_ITEM_SELLING,
	ERR_WTC_SELL_ROLE_WHILE_FORBID,
	ERR_WTC_SELL_ROLE_HAS_FAC_RELATION,
	ERR_WTC_SELL_ROLE_HAS_SECT_RELATION,
	ERR_WTC_SELL_ROLE_HAS_SPOUSE_RELATION,
	ERR_WTC_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION,
};

//Consign end

// Friend Callback start
enum ERR_FRIENDCALLBACK
{
	ERR_CALLBACK_SUCCESS,               // 0.操作成功
	ERR_CALLBACK_TIMEOUT,               // 1.消息处理超时
	ERR_CALLBACK_UNVALIDTEMPID,         // 2.发送召回邮件的邮件模板ID无效
	ERR_CALLBACK_LIMITNOTINIT,          // 3.玩家发送召回邮件的限制条件没有加载
	ERR_CALLBACK_MSGCOOLDOWN,           // 4.发送召回邮件的时间间隔过短
	ERR_CALLBACK_OFFLINE,               // 5.发送者不在线
	ERR_CALLBACK_FRIENDONLINE,          // 6.召回邮件的接收者在线
	ERR_CALLBACK_FDLISTNOTINIT,         // 7.玩家的好友列表没有加载
	ERR_CALLBACK_NOTHISFRIEND,          // 8.接收者不是发送者的好友
	ERR_CALLBACK_MAXMAILNUM,            // 9.发送者已经达到当天发送召回邮件的上限
	ERR_CALLBACK_COOLDOWN,              // 10.召回邮件的接收者还处于冷却期
	ERR_CALLBACK_EXCEPTION,             // 11.数据库操作异常
	ERR_CALLBACK_ACCOUNTFROZEN,         // 12.召回邮件的接收者为老玩家
	ERR_CALLBACK_MAILBOXFULL,           // 13.邮箱已满
	ERR_CALLBACK_NOAWARD,               // 14.没有奖励
};

enum FRIENDCALLBACK_AWARDTYPE
{
	RECALL_AWARD = 1, 		// 由于召回老玩家而获得的奖励
	RETURN_AWARD = 2, 		// 老玩家回归奖励
};

enum
{
	BONUS_CIRCLE,
	BONUS_CALLBACK,
};

//Mail sender type, local extention
enum MAIL_SENDER_TYPE_LOCAL{
	_MST_LOCAL_START = 32,
	_MST_LOCAL_CONSIGN = _MST_LOCAL_START, //consign
	_MST_CALLBACK,
	_MST_CALLBACK_AWARD,
	_MST_ROLE_CONSIGN_NOTIFY,
	_MST_GAMEDB,
	_MST_KINGDOM_AWARD,  //国王登基 成员发放时装
	_MST_FACTION_PK_BONUS_NOTIFY=40,
};

enum FACTION_DEFINE
{
	MAX_HOSTILE_NUMBER = 5,
	HOSTILE_MINIMUM = 3*3600,
	HOSTILE_TIMEOUT = 3*24*3600,
	PROTECT_TIMEOUT = 7*24*3600,
};

enum HOSTILE_DEFINE
{
	HOSTILE_PROTECTED = 0x01,
};

enum HOSTILE_DATAMASK
{
	OP_PROTECT = 0x01,
       	OP_ACTIONPOINT = 0x02, 
};

enum TERRI_SAVE_REASON
{
	TERRITORY_SAVE_BIDBEGIN,
	TERRITORY_SAVE_BIDEND,
};

inline HostileInfo & GetHostileInfo(GFactionInfo &info) { return info.hostileinfo.front();}
inline std::vector<HostileFaction> & Hostiles(GFactionInfo &info) { return GetHostileInfo(info).hostiles;}

enum FAMILY_DEFINE
{
	MAX_FAMILY_MEMBER_NUMBER = 15,
	FAMILYSKILL_BASE         = 440,
	FAMILYSKILL_SUM          = 4,
};


enum ERR_HOSTILE
{
	ERR_FC_INFACTION 		= 125,
	ERR_HOSTILE_COOLING 		= 126,  // 加为敌对帮派不足10小时，不能撤销敌对状态
	ERR_HOSTILE_ALREADY 		= 127,  // 已经是敌对状态
	ERR_HOSTILE_FULL 		= 128,  // 敌对帮派数达到上限
	ERR_HOSTILE_PEER_FULL 		= 129,  // 对方敌对状态数达到上限
	ERR_HOSTILE_LEVEL_LIMIT 	= 130,  // 两个帮派等级相差过大，不能进入敌对状态
	ERR_HOSTILE_ITEM 		= 131,  // 没有行动点
	ERR_HOSTILE_PROTECTED 		= 132,  // 特殊物品保护，不能进入敌对状态
	ERR_HOSTILE_PEER_PROTECTED 	= 133,  // 对方有特殊物品保护，不能进入敌对状态
	ERR_HOSTILE_PRIVILEGE 		= 134,  // 无权进行此操作
	ERR_HOSTILE_PROTECTITEM		= 135,  // 没有找到特殊保护物品
	ERR_FC_MAILBOXFULL 	   	= 136,  //领取帮战奖金时邮箱已满
	ERR_FC_FACTION_INEXIST 	   	= 137,  //没有对应帮派	  
	/*
	ERR_FC_MSG_COOLDOWN		= 138,  //帮派留言冷却
	ERR_FC_MSG_ARG			= 139,  //帮派留言参数错误
	ERR_FC_MSG_LENGTH		= 140,  //留言超长
	*/
};

enum RAID_TYPE
{
	ROOM_RAID = 0,
	TOWER_RAID = 1,//爬塔
	TEAM_RAID = 2,//6v6
	FACTION_PK_RAID = 3,
	LEVEL_RAID = 4, //关卡副本
	CRSSVRTEAMS_RAID = 5, // 跨服战队6v6
	TRANSFORM_RAID = 6, //变身副本
	STEP_RAID = 7,
	HIDEANDSEEK_RAID = 8, // 捉迷藏副本
	FENGSHEN_RAID = 9, // 飞升150-160
};
enum RAID_TEAM_TYPE//TEAM_RAID
{
	RAIDTEAM_NEUTRAL = 0,//not used
	RAIDTEAM_ATTACK,
	RAIDTEAM_DEFEND,
	RAIDTEAM_LOOKER,
	RAIDTEAM_END = RAIDTEAM_LOOKER,
};
enum ERR_RAID
{
	ERR_RAID_MAP_NOTEXIST 			= 1, //地图不存在
	ERR_RAID_ROOM_NUMBER_FULL 		= 2, //房间数到达上限
	ERR_RAID_JOIN_ALREADY                   = 3, //已经加入副本
	ERR_RAID_ROOMID_EXIST 			= 4, //副本房间id重复
	ERR_RAID_ROLE_OFFLINE 			= 5, //玩家已下线
	ERR_RAID_ROOM_INEXIST 			= 6, //副本房间不存在
	ERR_RAID_ARGUMENT_ERR 			= 7, //参数错误
	ERR_RAID_STARTED 			= 8, //副本已开始
	ERR_RAID_APPLY_TEAM_FULL		= 9, //副本报名队伍已满
	ERR_RAID_ROLE_ALREADY_IN_ROOM 		= 10,//已经在副本房间中
	ERR_RAID_OPERATOR_NOT_MASTER 		= 11,//操作人不是房主
	ERR_RAID_TEAM_FULL			= 12,//副本正式队伍已满
	ERR_RAID_NOT_IN_APPLYLIST 		= 13,//不在申请队伍中
	ERR_RAID_JOIN_REFUSED 			= 14,//房主拒绝申请者
	ERR_RAID_START_TEAMNUMBER 		= 15,//房间人数不足
	ERR_RAID_GET_INSTANCE_FAIL 		= 16,//DELIVER获取实例失败
	ERR_RAID_ROLE_NOT_IN_ROOM 		= 17,//玩家不在此房间
	ERR_RAID_STATUS 	 		= 18,//房间当前状态不允许此操作
	ERR_RAID_INSTANCE_INEXIST 		= 19,//副本实例不存在
	ERR_RAID_ROOMCLOSE 	 		= 20,//副本房间关闭
	ERR_RAID_ROLE_NOT_IN_RAID 		= 21,//玩家未报名副本
	ERR_RAID_ITEMLIMIT 			= 22,//开房间物品不足
	ERR_RAID_STATUS_CHANGE 			= 23,//状态切换错误
	ERR_RAID_GS_GET_INSTANCE_FAIL 		= 24,//GS获取实例失败
	ERR_RAID_IS_OPENING 			= 25,//玩家已经在申请开房间
	ERR_RAID_GS_ENTER_CHECK_FAIL 		= 26,//GS检查进入副本条件失败
	ERR_RAID_DB_TIMEOUT 			= 27,//DB处理超时
//	ERR_RAID_INSTANCE_START 		= 28,//开启副本失败
//	ERR_RAID_INSTANCE_ENTER 		= 29,//进入副本失败
//	ERR_RAID_INSTANCE_CLOSE 		= 30,//副本关闭
//	ERR_RAID_KICKOUT 			= 31,//被房主踢出
	ERR_RAID_ROOMNAME 			= 28,//房间名错误
	ERR_RAID_VOTECOOLDOWN			= 29,//投票冷却中
	ERR_RAID_VOTEING 			= 30,//正在进行其他投票
	ERR_RAID_VOTEDROLE_NOT_IN_ROOM 		= 31,//被投票人不在房间中
	ERR_RAID_VOTEDROLE_NOT_IN_RAID 		= 32,//被投票人不在副本中
	ERR_RAID_VOTER_NOT_IN_ROOM 		= 33,//投票人不在房间中
	ERR_RAID_VOTER_NOT_IN_RAID 		= 34,//投票人不在副本中
	ERR_RAID_NOT_VOTEING 			= 35,//不在进行投票
	ERR_RAID_VOTEDROLE_MISMATCH		= 36,//被投票人不匹配
	ERR_RAID_ROOM_FORBID_VOTE 		= 37,//房间不允许投票
	ERR_RAID_TYPE 				= 38,//副本类型错误
	ERR_RAID_TEAM_SIZE 			= 39,//副本人数错误
	ERR_RAID_IS_APPLYING 			= 40,//正在组队报名6v6副本
	ERR_RAID_NOT_VALID_FACTITLE             = 41,//帮派职务不符
	ERR_RAID_TARGET_NOT_ONLINE 		= 42,//被挑战人不在线
	ERR_RAID_NOT_IN_FACTION 		= 43,//未参加帮派
	ERR_RAID_IN_SAME_FACTION 		= 44,//属于同一帮派
	ERR_RAID_FAC_LOWLEVEL 			= 45,//帮派等级不符
	ERR_RAID_CHALLENGE_COOLING 		= 46,//帮派挑战冷却中
	ERR_RAID_FACTION_COMBAT_BUSY 		= 47,//正在帮战中
	ERR_RAID_TARGET_MASTER_NOT_ONLINE	= 48,//被挑战帮主不在线
	ERR_RAID_FACTION_NOPROSPERITY  		= 49,//帮贡不足
	ERR_RAID_FACTION_CHALLENGE_REJECTED 	= 50,//帮战被拒绝
	ERR_RAID_FACTION_CHALLENGE_TIMEOUT 	= 51,//帮主邀请超时	
	ERR_RAID_FACTION_JOIN_GROUP 		= 52,//报名阵营错误
	ERR_RAID_FACTION_CANCEL_ARU_ERR 	= 53,//取消帮战错误
	ERR_RAID_FACTION_KILL_INVALID 		= 54,//无效帮战击杀
	ERR_RAID_TEAMAPPLY_COOLING		= 55,//组队副本冷却十分钟
	ERR_RAID_NOT_APPLY_TEAM			= 56,//没有申请组队副本
	ERR_RAID_NOT_TEAMAPPLY_TIME		= 57,//非组队副本报名时间
	ERR_RAID_TEAM_APPLY_COND		= 58,//某个职业人数>2不能报名
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_HAVE_OTHER  	= 59,//您所在的小队有其他战队成员，不能进行报名
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_ROLESIZE 	= 60,//您所在的小队没有组满6人，不能进行报名
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_NOT_IN		= 61,//您还不是战队成员，请加入战队才可进行报名	
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_MUST_TEAM	= 62,//战队报名必须在组队状态下，请您先跟自己战队成员组成小队，进行报名
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_MUST_ONTIME	= 63,//您所在的战队已经报名，请等他们结束后再开始报名
};
enum ERR_BATTLE
{
	ERR_BATTLE_TEAM_FULL				= 140,	// 阵营已满
	ERR_BATTLE_GAME_SERVER				= 141,	// 不在同一条线
	ERR_BATTLE_JOIN_ALREADY 			= 142,	// 已经加入队伍
	ERR_BATTLE_MAP_NOTEXIST				= 143,	// 没有找到地图
	ERR_BATTLE_COOLDOWN					= 144,	// 离上次战斗时间不足冷却时间，不能报名
	ERR_BATTLE_NOT_INTEAM				= 145,  // 用户不在队伍中
	ERR_BATTLE_LEVEL_LIMIT				= 146,  // 用户不符合战场级别限制
	ERR_BATTLE_CLOSE					= 147,  // 战场没有开放
	ERR_BATTLE_QUEUELIMIT           	= 148,  // 玩家排队超过限制
	ERR_BATTLE_INFIGHTING           	= 149,  // 已经进入战场，不能退出排队
	ERR_BATTLE_MONEY           			= 150,  // 钱不够，不能开启战场
	ERR_BATTLE_REBORN_NEED     			= 151,  // 需要飞升才能加入飞升战场
	ERR_BATTLE_STARTED     				= 152,  // 宋金战场开启后不能再报名
	ERR_BATTLE_STARTED10     			= 153,  // 宋金战场开启10分钟后不能再进入战场
	ERR_BATTLE_JOIN_REFUSED				= 154,	// 报名被首领拒绝
	ERR_BATTLE_APPOINT_TARGETNOTFOUND 	= 155,	// 接受传位的人不在线
	ERR_BATTLE_TEAM_SIZE			 	= 156,	// 排队人数达不到开启要求
	ERR_BATTLE_NOT_MASTER			 	= 157,	// 不是首领，没有权限开启战场
	ERR_BATTLE_MONEYLIMIT				= 158,	// 金钱不足激活战场
	ERR_BATTLE_ITEMLIMIT				= 159,	// 物品不足激活战场
	ERR_BATTLE_OCCUPATION           	= 160,  // 玩家阵营限制
	ERR_BATTLE_ACCEPT_REQUESTERNOTFOUND	= 161,	// 报名的人不在线
	ERR_BATTLE_ACTIVATECOOLDOWN			= 162,	// 战场激活冷却
	ERR_BATTLE_REBORN_CNT				= 163,	// 飞升次数不满足要求
	ERR_BATTLE_MUSTNOT_REBORN			= 164,	// 已飞升玩家不能报名
	ERR_BATTLE_TIMEOUT 				= 165, 	// GS处理超时
	ERR_BATTLE_CROSSBATTLE 				= 166,  // 角色有跨服战场信息,主要用于不让客户端提示加入战场成功
	ERR_BATTLE_ENTERCROSSBATTLE 	 		= 167,  // 角色在跨服进入战场
	ERR_BATTLE_ENTER_MAXTIMES			= 168,  //进入次数到达上限
	ERR_BATTLE_ENTER_NOT_IN_JOINROOM		= 169,  // 首次进入需在报名房间npc处，请联系GM
};

enum SIEGE_RESULT
{
	SIEGE_ATTACKER_WIN = 1,
	SIEGE_DEFENDER_WIN = 2,
	SIEGE_TIMEOUT = 3,
	SIEGE_CANCEL = 4,
};

enum SIEGE_RELATIVE
{
	SIEGE_QUEUE_MAX = 15,

	MONEY_ONEDAY_SIEGE = 100*10000,
	MONEY_TWODAY_SIEGE = 2*MONEY_ONEDAY_SIEGE,
	MONEY_THREEDAY_SIEGE = 3*MONEY_ONEDAY_SIEGE,

};
enum ERR_SIEGE
{
	ERR_SIEGE_QUEUEFULL 		= 150, // 排队帮派过多
	ERR_SIEGE_FACTIONLIMIT 		= 151, // 帮派级别限制
	ERR_SIEGE_NOTFOUND 		= 152, // 没有找到战场
	ERR_SIEGE_CHALLENGED		= 153, // 已经报名
	ERR_SIEGE_SELF			= 154, // 不能挑战自己
	ERR_SIEGE_CHECKFAILED		= 155, // 参数检查失败
	ERR_SIEGE_TIMEERROR		= 156, // 现在不能设置辅助帮派
	ERR_SIEGE_MONEYLIMIT		= 157, // 报名失败，金币不足
	ERR_SIEGE_ITEMLIMIT		= 158, // 报名失败，缺少物品
	ERR_SIEGE_FACTIONNOTFOUND	= 159, // 没有找到辅助帮派
	ERR_SIEGE_CANTENTERNORMAL	= 161, // 不能进入
	ERR_SIEGE_CANTENTERBATTLE	= 162, // 不能进入
	ERR_SIEGE_NOFACTION		= 163, // 不能进入
	ERR_SIEGE_ASSISTANT_TIME 	= 164, // 辅助帮成立时间不足
	ERR_SIEGE_ASSISTANT_LEVEL 	= 165, // 辅助帮级别不足
	ERR_SIEGE_ASSISTANT_SELF 	= 166, // 辅助帮为进攻帮
	ERR_SIEGE_ASSISTANT_OWNER 	= 167, // 辅助帮为城主帮
	ERR_SIEGE_COOLING			= 168,	//连续报名要间隔10分钟

};

enum ERR_TERRITORY
{
				    //以下是通用错误码
	ERR_TERRI_NOTFOUND 		= 1,//操作领土不存在
	ERR_TERRI_LOGIC,		    //逻辑错误
	ERR_TERRI_OUTOFSERVICE,		    //服务器错误
				   //以下是宣战错误码
	ERR_TERRI_BID_TIME,		   //非宣战时间
	ERR_TERRI_BID_REPEAT,		   //重复宣战，本周已对其他领土宣战
	ERR_TERRI_BID_BUSY,		   //数据库繁忙
	ERR_TERRI_BID_FAC_NOTFOUND,	   //宣战帮派不存在
	ERR_TERRI_BID_NOTMASTER,	   //非帮主
	ERR_TERRI_BID_SELF,	      	   //对自己领土宣战
	ERR_TERRI_BID_ITEMTYPE,		   //宣战物品类型错误
	ERR_TERRI_BID_ITEM_MIN,		   //宣战金过少
	ERR_TERRI_BID_LISTSIZE,		   //宣战帮派过多
	ERR_TERRI_BID_ITEM_MAX,		   //宣战金超上限
	ERR_TERRI_BID_FAC_LEVEL,	   //帮派等级不足
	ERR_TERRI_BID_ADJACENT,		   //宣战帮派不接壤
	ERR_TERRI_BID_OCCUPY_NUM,	   //已占领城数不足
				   //以下是进入错误码
	ERR_TERRI_ENTER_NOTSTART,	   //战斗未开启			17
	ERR_TERRI_ENTER_NOTBATTLESIDE,	   //非战斗双方成员
	ERR_TERRI_ENTER_ALREADYIN,	   //已经在战斗中
	ERR_TERRI_ENTER_NUMLIMIT,	   //人数过多			20
	ERR_TERRI_ENTER_JOINTIME,	   //加入帮派不满一周
				   //以下是离开错误码
	ERR_TERRI_LEAVE_NOTIN,		   //玩家未在领土战中
				   //以下是领物品错误码
	ERR_TERRI_GETITEM_NOTMASTER,	   //非帮主			23
	ERR_TERRI_GETITEM_NUM,		   //数量错误
	ERR_TERRI_GETITEM_TIME,		   //事件错误
	ERR_TERRI_GETITEM_ITEMID,	   //领取物品类型错误
	ERR_TERRI_GETITEM_NOTWINNER,	   //非胜利方
	ERR_TERRI_GETITEM_PEACE,	   //本周没打领土战，所以不会有宣战金奖励
	ERR_TERRI_GETITEM_NOTBIDDER,	   //没有宣过站或者是宣战第一名，不能退还宣战金
	ERR_TERRI_GETITEM_NORAND,	   //该领土没有神器 或者神器已经领完
	ERR_TERRI_GETITEM_INVALIDTYPE,	   //神器类型错误
	ERR_TERRI_BID_COOLING,		   //宣战冷却       32
	ERR_TERRI_BID_CREATETIME,	   //帮派创建满一周才可以宣战
};

enum ERR_SECT
{
	ERR_SECT_OFFLINE        = 1,   //玩家不在线
	ERR_SECT_UNAVAILABLE    = 2,   //对方已经拜师
	ERR_SECT_FULL           = 3,   //徒弟数量已经达到上限
	ERR_SECT_BUSY           = 4,   //系统繁忙，请稍后再试
	ERR_SECT_REFUSE         = 5,   //对方拒绝了你的收徒邀请
	ERR_SECT_INVALIDLEVEL   = 6,   //对方级别不满足要求
	ERR_SECT_COOLING        = 7,   //一天只能招收一弟子
};

enum USER_STATUS
{
	STATUS_CASHINVISIBLE   =   0x01,
};

class FactionHelper
{
public :
	static unsigned int GetMemberCapacity(int level)
	{
		unsigned int _capacity[10] = {5, 7, 9, 11, 13, 15, 17, 19, 21, 23};
	        return _capacity[level%10];
	}

};

enum WAITDEL_TYPE
{
	TYPE_ROLE        = 1,
	TYPE_FACTION     = 2,
	TYPE_FAMILY      = 3,
};

enum FORBID
{
	FORBID_USER_LOGIN = 100,
};

enum SNS_CHARM_TYPE
{
	SNS_CHARM_TYPE_PERSONAL = 0,
	SNS_CHARM_TYPE_FRIEND,
	SNS_CHARM_TYPE_SECT,
	SNS_CHARM_TYPE_FACTION,
};

enum SNS_MESSAGE_TYPE
{
	SNS_MESSAGE_TYPE_WEDDING = 0,
	SNS_MESSAGE_TYPE_FRIEND,
	SNS_MESSAGE_TYPE_SECT,
	SNS_MESSAGE_TYPE_FACTION,
	SNS_MESSAGE_TYPE_MAX,
};

enum SNS_LEAVEMESSAGE_TYPE
{
	SNS_LEAVEMESSAGE_TYPE_PRESS = 0,
	SNS_LEAVEMESSAGE_TYPE_APPLY,
	SNS_LEAVEMESSAGE_TYPE_LEAVE,
};

enum SNS_OPERATION_TYPE
{
	SNS_OP_PRESSMESSAGE = 0,
	SNS_OP_APPLY,
	SNS_OP_APPLYMSG,
	SNS_OP_AGREE,
	SNS_OP_DENY,
	SNS_OP_ACCEPTAPPLY,
	SNS_OP_REJECTAPPLY,
};

enum SNS_ERRCODE
{
	SNS_ERR_SUCCESS = 0,
	SNS_ERR_PRIVILEGE,
	SNS_ERR_ROLEOFFLINE,
	SNS_ERR_LEVEL,
	SNS_ERR_FACTIONLEVEL,
	SNS_ERR_GENDER,
	SNS_ERR_OCCUPATION,
	SNS_ERR_VOTETIME15MIN,
	SNS_ERR_LENGTH,
	SNS_ERR_REPEAT,
	SNS_ERR_MSGNOTFOUND,
	SNS_ERR_APPLYROLEIDNOTFOUND,
	SNS_ERR_MARRIED,
	SNS_ERR_TITLE,
	SNS_ERR_FORBIDSELF,
	SNS_ERR_NOTREADY,
	SNS_ERR_PRESSTIME24HOUR,
	SNS_ERR_PRESSTIME12HOUR,
	SNS_ERR_APPLYTIME24HOUR,
	SNS_ERR_SECTAPPLYLEVEL,
	SNS_ERR_APPLYCOOLTIME,
	SNS_ERR_LEAVEMSGCOOLTIME,
	SNS_ERR_VOTECOOLTIME,
	SNS_ERR_LEAVEMSGLEVEL,
	SNS_ERR_LEAVEMSGFULL,
	SNS_ERR_ALREADYINSECT,
	SNS_ERR_ALREADYINFACTION,
	SNS_ERR_MARRYMULTIACCEPT,
	SNS_ERR_APPLYMULTIACCEPT,
	SNS_ERR_MARRYALREADYACCEPT,
	SNS_ERR_LEAVEMSGNOTFOUND,
};

enum SNS_LIMIT
{
	SNS_LIMIT_COOLTIME = 15*60,						//两次应征、投票要间隔15分钟
	SNS_LIMIT_LEAVEMSGCOOLTIME = 60,				//两次留言要间隔1分钟
	SNS_LIMIT_PRESSLEVEL = 15,						//15级以上才能发布信息
	SNS_LIMIT_SECTPRESSLEVEL = 105,					//105级以上才能发布师门招募信息
	SNS_LIMIT_SECTAPPLYLEVEL = 90,					//90级以上不能应征师门
	SNS_LIMIT_MESSAGESIZE = 60,						//消息或留言大小不能超过40字节
	SNS_LIMIT_VOTETIME = 15*60,						//同一条消息的投票要间隔15分钟
	SNS_LIMIT_APPLYTIME = 24*3600,					//同一条消息的应征要间隔24小时
	SNS_LIMIT_PRESSTIME_CHARMSTARGE4 = 12*3600,		//魅力值4星及以上发布时间间隔12小时
	SNS_LIMIT_PRESSTIME_CHARMSTARLT4 = 24*3600,		//魅力值4星以下发布时间间隔24小时
	SNS_LIMIT_APPLYINCCHARM = 1,					//申请使得该信息魅力值加1
	SNS_LIMIT_LEAVEMSGINCCHARM = 1,					//留言使得该信息魅力值加1
	SNS_LIMIT_VOTEAGREEINCCHARM = 50,				//投票支持一条消息使其魅力值加1
	SNS_LIMIT_VOTEDENYDECCHARM = 50,				//投票反对一条消息使其魅力值减1
	SNS_LIMIT_MSGCHARM = 9999,						//一条消息魅力值上限9999
	SNS_LIMIT_MSGLIFETIME = 7*24*3600,				//消息的保留时间为一个星期
	SNS_LIMIT_MSGNUM = 50,							//每类信息最多保留50条
	SNS_LIMIT_LEAVEMSGNUM = 50,						//同一条消息的留言最多保留50条
	SNS_LIMIT_APPLYVOTENUM = 200,					//应征、支持、反对人列表最多保留200个人
	SNS_LIMIT_CHARMSTAR = 10,						//魅力值星级最多10级
	SNS_LIMIT_LEAVEMSGMAXLEVEL = 8,					//留言最大层数
};

enum REF_ERRCODE	//线上推广相关错误代码
{
	REF_ERR_SUCCESS = 0,
	REF_ERR_NOREFERRALFOUND,						//玩家没有在线的下线
	REF_ERR_REFERRERNOTLOADED,						//上线信息还没加载
	REF_ERR_REFERRERINTRANSACTION,					//上线正在提取鸿利值或经验值
	REF_ERR_REFERRERLOGOUT,							//上线不在线
	REF_ERR_NOEXPAVAILABLE,							//上线没有可提取的经验值
	REF_ERR_NOBONUSAVAILABLE,						//上线没有可提取的鸿利值
	REF_ERR_SUBMITTODB,								//提取事务提交给数据库错误
	REF_ERR_INVALIDSPREADCODE,						//推广码不正确
	REF_ERR_REFERRERNOTINSERVER,					//推广人不在本服务器
	REF_ERR_REFREFERRERLEVEL,						//60级以上才能推广下线
	REF_ERR_REFERRERNOTINDISTRICT,					//推广人不在本区
};

enum REF_LIMIT
{
	REF_LIMIT_REFERRALPERPAGE = 15,					//客户端每次获取15个下线
	REF_LIMIT_REFERRERLEVEL	  = 60,					//上线必须达到60级才能推广
};

enum CHALLENGE_ALGO
{
	ALGO_NONE = -1,
	ALGO_MD5 = 0,
	ALGO_PLAINTEXT = 1, 
};

enum ERR_UNIQUEAUCTION
{
	ERR_UNIQUE_BID_TIME		= 1, //非竞拍时间
	ERR_UNIQUE_BID_MONEY,		     //竞拍价格不符合条件
	ERR_UNIQUE_BID_TIMES_PERSON,	     //竞拍超过每人每天规定的次数
	ERR_UNIQUE_BID_TIMES_TOTAL,	     //全服竞拍总次数超过每天规定的次数
	ERR_UNIQUE_BID_REPEAT,		     //竞拍了相同的价格
	ERR_UNIQUE_GETITEM_NOTWINNER,	     //非竞拍获奖者
	ERR_UNIQUE_GETITEM_ITEMID,	     //竞拍物品类型错误，正常情况不会出现
};

enum ERR_KINGDOM
{
	ERR_KINGDOM_BATTLE_STATE	= 1, //不是战斗时间 无法进入战场
	ERR_KINGDOM_INVALID_FAC,	     //身份不符合国战条件，无法进入
	ERR_KINGDOM_ROLE_LIMIT,		     //己方战场人数达到上限，暂时无法进入
	ERR_KINGDOM_INVALID_TAG,	     //地图号错误
	ERR_KINGDOM_NOTIN,		     //您不在战场中
	ERR_KINGDOM_FIELD_UNREGISTER,        //该战场类型不存在
	ERR_KINGDOM_BATTLE_STILL_IN,	     //异常退出 请稍后进入
	ERR_KINGDOM_ALREADY_FAIL,	     //己方复活点已被摧毁，无法进入地图1
	ERR_KINGDOM_SUB_LIMIT,		     //战场地图人数达到上限，暂时无法进入

	ERR_KINGDOM_PRIVILEGE,		     //无行驶权利权限 10
	ERR_KINGDOM_LOGIC,		     //逻辑错误
	ERR_KINGDOM_POINTS_NOTENOUGH,	     //国家积分不足
	ERR_KINGDOM_NOT_INIT,		     //数据未加载
	ERR_KINGDOM_SVR_ATTR_REPEAT,	     //服务已经开启 不能重复开启
	ERR_KINGDOM_SVR_ATTR_CD,	     //服务器属性更改冷却
	ERR_KINGDOM_SVR_ATTR_COND,	     //服务器属性修改开启条件限制
	ERR_KINGDOM_FUNC_SIZE,		     //职位人数已满
	ERR_KINGDOM_INVALID_NAME,	     //角色名不存在
	ERR_KINGDOM_TITLE_REPEAT,	     //玩家已有国家职位
	ERR_KINGDOM_INVALID_CHAR,	     //圣旨中包含非法字符
	ERR_KINGDOM_CALLGUARDS_CD,	     //您今天传送亲卫的次数已经达到上限
	ERR_KINGDOM_NOGUARD_ONLINE,	     //没有亲卫在线
	ERR_KINGDOM_TIMEOUT,		     //天帝没有注意到您的申请 请稍后再做请求
	ERR_KINGDOM_KING_REFUSE,	     //天帝没有同意您的全服泡澡请求
	ERR_KINGDOM_KING_NOTONLINE,	     //国王没有在线
	ERR_KINGDOM_SYS_BATH_TIME,	     //系统开启泡澡时间段不能开启泡澡
	ERR_KINGDOM_DATABASE,		     //数据库错误
	ERR_KINGDOM_TASK_CD,		     //国王发布任务冷却
	ERR_KINGDOM_TASK_REPEAT,	     //国王已经发布了任务不能重复发
};

enum ERR_TOUCH
{
	ERR_TOUCH_SUCCESS		= 0,
	ERR_TOUCH_COMMUNICATION 	= 1, //服务器间通信错误
	ERR_TOUCH_EXCHG_FAIL		= 2, //兑换 touch 点 失败
	ERR_TOUCH_GEN_ORDERID		= 3, //生成 touch 兑换订单号失败
	ERR_TOUCH_POINT_NOTENOUGH	= 4, //touch 点不足
};

class Utility
{
public:
	static void SwapSyncData(GMailSyncData& data1, GMailSyncData& data2)
	{
		/*std::swap(data1.data_mask, data2.data_mask);
		std::swap(data1.cash_total, data2.cash_total);
		std::swap(data1.cash_used, data2.cash_used);
		std::swap(data1.cash_serial, data2.cash_serial);
		std::swap(data1.inventory.capacity, data2.inventory.capacity);
		std::swap(data1.inventory.timestamp, data2.inventory.timestamp);
		std::swap(data1.inventory.money, data2.inventory.money);

		data1.inventory.items.swap(data2.inventory.items);
		data1.inventory.equipment.swap(data2.inventory.equipment);
		data1.inventory.petbadge.swap(data2.inventory.petbadge);
		data1.inventory.petequip.swap(data2.inventory.petequip);

		std::swap(data1.storehouse.capacity, data2.storehouse.capacity);
		std::swap(data1.storehouse.capacity2, data2.storehouse.capacity2);
		std::swap(data1.storehouse.money, data2.storehouse.money);
		data1.storehouse.items.swap(data2.storehouse.items);
		data1.storehouse.items2.swap(data2.storehouse.items2);
		data1.logs.swap(data2.logs);*/
		data1 = data2;
	}
};

enum FUNGAME_LIMIT
{
	FUN_HALL_MEMBER_MAX 	= 600,
};

#ifdef USE_LOG2 
        #define LogProto LOG::Log
#else
        #define LogProto
#endif

#define QPUT_ACHIEVEMENT    0x00000010
#define GET_ACHIEVEMENT	   0x00000040
enum 
{
	GAME_TYPE_NO	= 0, //游戏框架
	GAME_TYPE_LANDLORD = 1, //斗地主游戏 
	GAME_TYPE_CARD = 2, //卡牌游戏
};

enum //flag value in playerlogin_re.hpp
{
	DS_TO_CENTRALDS = 1,
	CENTRALDS_TO_DS = 2,
	DIRECT_TO_CENTRALDS = 3,
	DIRECT_TO_RAID = 4,//direct login raid line 原服断线重入副本
	DS_TO_BATTLENPC = 5, 	//直接进入跨服战场报名地图
	DS_TO_BATTLEMAP = 6, 	//直接进入跨服战场地图
	DIRECT_TO_CNETRALMAP = 7,// direct login 断线重入跨服战场或副本
	//DS_TO_OPENBANQUET = 8, // 原服登录到跨服流水?
};

inline bool IsTransToCentralDS(char flag) 
{ 
	return (flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLENPC || flag == DS_TO_BATTLEMAP) ? true : false;
}
inline bool IsTransToNormalDS(char flag)
{
	return (flag == CENTRALDS_TO_DS) ? true : false;
}

enum
{
	KICKOUT_LOCAL	= 1,
	KICKOUT_REMOTE	= 2,
};

enum GS_ATTRIBUTE_MASK
{
	GS_ATTR_HIDE	= 0x01,
	GS_ATTR_CENTRAL = 0x02,
};

enum GATE_ATTR_MASK
{
	GT_SERVER_CONNECTED = 0x01,
};

enum ErrCode_Local  //续../rpc/errcode.h里面的 enum ErrCode
{
	ERR_IDENTITY_TIMEOUT       = 57, //跨服 验证码超时
	ERR_IDENTITY_NOTMATCH      = 58, //跨服 验证码不匹配
	ERR_REMOTE_VERIFYFAILED    = 59, //跨服 跨服中玩家数据错误
	ERR_CHG_GS_STATUS	   = 67, //当前状态下无法执行此操作
	ERR_CDS_COMMUNICATION	   = 90, //天界服务器正在维护
	ERR_DELROLE_SECT	   = 91, //有师徒关系情况下不能删号
	ERR_FORBIDROLE_GLOBAL	   = 92, //预注册阶段禁止登陆角色

	ERR_FC_CHANGE_KINGDOM      = 126, //国王帮不能传位或解散
	ERR_FC_RECRUIT_FAMILY_CD   = 127, //家族入帮冷却
};

enum
{
	WEB_ACT_GET_SEND_TOP = 0,
	WEB_ACT_GET_RECV_TOP = 1,
	WEB_ACT_GET_CASH_AVAIL = 2,
};

enum AuReqType
{
	AU_REQ_FASTPAY = 4, //快捷支付
	AU_REQ_FASTPAY_BINDINFO = 6, //绑定了哪些快捷支付商家
	AU_REQ_TOUCH_POINT = 8, //获取 touch 点
	AU_REQ_TOUCH_POINT_EXCHG = 9, //兑换 touch 点
	AU_REQ_FLOWER_TOP = 10, //网站从游戏中取鲜花榜
	AU_REQ_BROADCAST = 11, //游戏向全区全服广播的协议
	AU_REQ_PASSPORT_CASHADD = 12, //获取通行证的累积充值额
};

enum 
{
	USER_TABLE_CRUSHED = 0x01,
	STATUS_TABLE_CRUSHED = 0x02,
};

enum ETopFlowerStatus
{
	TOPFLOWER_OPT_OK = 0, // 执行成功
	TOPFLOWER_NOTREADY = 1,// 未准备好
	TOPFLOWER_READY = 2,// 准备好
};

enum ETopFLowerErr
{
	TOPFLOWER_LOAD_TOP_SUCCESS = 0,// 加载top500榜成功
	TOPFLOWER_LOAD_ADD_SUCCESS = 1,// 加载附加榜成功
	TOPFLOWER_LOAD_TOP_ERR = 2, // 加载top500失败
	TOPFLOWER_LOAD_ADD_ERR = 3, // 加载附加榜失败
	TOPFLOWER_LOAD_END = 4, // 加载结束	

	TOPFLOWER_FLOWER_TAKEOFF_SUCCESS,// 扣鲜花成功
	TOPFLOWER_FLOWER_TAKEOFF_ERR,//扣鲜花失败
	TOPFLOWER_FLOWER_GIFT_TAKE_SUCCESS, // 领取礼品成功
	TOPFLOWER_FLOWER_GIFT_TAKE_ERR,// 领取礼品失败

	TOPFLOWER_GETROLEID_SUCCESS, // 得到roleid成功
	TOPFLOWER_GETROLEID_ERR_OLDNAME, // 使用的是老名字
	TOPFLOWER_ROLEID_NOT_EXIST,// roleid 不存在
	TOPFLOWER_ROLEID_EXIST, // roleid 存在
	TOPFLOWER_UPDATA_DATA_SUCCESS,// 更新鲜花成功
	TOPFLOWER_UPDATA_DATA_ERR, // 更新鲜花失败
};

enum EClientSrvFlowerErr
{
	S2C_TOPFLOWER_NAME_ERR = 0, // 名字不对
	S2C_TOPFLOWER_GENDER_ERR = 1, // 性别不对
	S2C_TOPFLOWER_TOP_NOTREADY = 2, // 榜单没有准备好
	S2C_TOPFLOWER_SUCESS = 3, // 成功
	S2C_TOPFLOWER_DB_ERR = 4, // 数据库执行过程中产生的错误,未知名
	S2C_TOPFLOWER_ROLEID_NOT_EXIST = 5, // roleid不存在
	S2C_TOPFLOWER_GIFT_TAKE_SUCESS = 6,// 提取礼物成功
	S2C_TOPFLOWER_GIFT_TAKE_ERR = 7, // 提取礼物失败
	S2C_TOPFLOWER_GIFT_NOT_IN_TOP = 8, // 未在排行榜中
	S2C_TOPFLOWER_GIFT_HAS_TAKE = 9, // 已经提取过了
	S2C_TOPFLOWER_GIFT_TIME_NOT = 10, // 领奖时间未到
};

enum EClientSrvOpenBanquetEnterNotify
{
	S2C_OPENBANQUET_ENTER_OK = 0, // 进入成功
	S2C_OPENBANQUET_ENTER = 1, // 失败
};

enum EClientSrvOpenBanquetJoinNotify
{
	S2C_OPENBANQUET_JOIN = 0, // 直接进入
	S2C_OPENBANQUET_QUEUE = 1, // 需要排队
};


enum KINGDOM_TITLE
{
	KINGDOM_TITLE_NONE = 0,
	KINGDOM_TITLE_KING,
	KINGDOM_TITLE_QUEEN,
	KINGDOM_TITLE_GENERAL,
	KINGDOM_TITLE_MINISTER,
	KINGDOM_TITLE_MEMBER,
	KINGDOM_TITLE_GUARD,
};

enum
{
	KINGDOM_DEFENDER_WIN = 1,
	KINGDOM_ATTACKER_WIN,
};

enum
{
	KINGDOM_TASK_NONE = 0,		//未发布任务 task_type=0 issu_time不确定
	KINGDOM_TASK_OPEN,		//任务发布中 task_type!=0 issue_time!=0
	KINGDOM_TASK_WAIT_BALANCE,	//任务结束等待结算 task_type=0 issue_time!=0
};

enum
{
	KINGDOM_PRIVILEGE_ANNOUNCE = 0x01,  //宣旨	
	KINGDOM_PRIVILEGE_APPOINT  = 0x02,  //授予职位
	KINGDOM_PRIVILEGE_SET_GAMEATTR = 0x04, //开启三倍经验
	KINGDOM_PRIVILEGE_OPEN_BATHPOOL = 0x08, //开放澡堂

	KING_PRIVILEGE_INIT = KINGDOM_PRIVILEGE_ANNOUNCE|KINGDOM_PRIVILEGE_APPOINT|KINGDOM_PRIVILEGE_SET_GAMEATTR,
	QUEEN_PRIVILEGE_INIT = 0x0, //皇后
	FUNC_PRIVILEGE_INIT = 0x0, //将军 大臣 
};

enum //数据库 config 表中各个 key 表示含义
{
	//100 已占用 DBConfig
	CONFIG_KEY_TOUCH_ORDER = 101,  //记录 touch 兑换订单号
};

enum //特殊全局变量ID
{
	COMMON_DATA_KINGTASK_WINPOINTS = 30353,  //暂存国王发布的任务为国王赢得的国家积分 
	COMMON_DATA_KINGTASK_ISSUETIME = 10070, //记录国王发布任务的时刻
};

enum
{
	KINGDOM_TASK_POINTS_LIMIT = 10000,  //国王发布的任务可以为国王赢取的积分上限
	KINGDOM_REWARD_WIN_TIMES = 6, //每 6 次连任会获得奖励
};
enum
{
	WEEK_SECONDS = 604800,  //一周的秒数
	DAY_SECONDS =   86400,	//一天的秒数
};
enum //离线消息特殊格式化类型 channel=CHANNEL_SPECIAL_TYPE 情况
{
	OFFLINE_MSG_ACACCUSE_REP = 1, //玩家举报外挂反馈
};

enum ERR_OPENBANQUET
{
	ERR_OPB_NOT_OPEN				= 1,	// 战场未开启
	ERR_OPB_TEAM_INFO_FAIL				= 2,	// 队伍限制不正确(1-6人)
	ERR_OPB_MAP_NOTEXIST				= 3,	// 没有找到地图
	ERR_OPB_LEVEL_LIMIT				= 4,    // 用户等级不符合战场限制
	ERR_OPB_APPOINT_TARGETNOTFOUND 			= 5,	// 接受传位的人不在线
	ERR_OPB_ACCEPT_REQUESTERNOTFOUND		= 6,	// 报名的人不在线
	ERR_OPB_ENTER_MAXTIMES				= 7,    // 进入次数到达上限
	ERR_OPB_HAS_QUEUE				= 8,    // 已经在排队中
	
	ERR_OPB_TIMEOUT 				= 20, 	// GS处理超时
	ERR_OPB_ROLE_NOT_ONLINE				= 21,   // 角色未在线
	ERR_OPB_ROLE_NOT_FIND_WAIT			= 22,   // 进入战场时，角色未在等待队列中
	ERR_OPB_ROLE_NOT_IN_BATTLE			= 23,	// 玩家未在战场中
};

enum ERR_IWEB_CMD
{
	ERR_IWEBCMD_SUCCESS		= 0, 	//成功
	ERR_IWEBCMD_INVALID_OP_TYPE	= 1,	//错误的操作类型
	ERR_IWEBCMD_DATA_NOTREADY	= 2,	//数据加载未完成
	ERR_IWEBCMD_UNMARSHAL		= 3,    //操作码 unmarsal 失败
	ERR_IWEBCMD_CMDSIZE		= 4, 	//操作码长度错误
	ERR_IWEBCMD_INTEGRITY		= 5, 	//操作码不完整
	ERR_IWEBCMD_PARSE		= 6, 	//操作码解析失败
	ERR_IWEBCMD_TIMESTAMP		= 7,	//操作码已过期
	ERR_IWEBCMD_COMMUNICATION	= 8,	//服务期间通信错误
	ERR_IWEBCMD_DB			= 9, 	//数据库错误
};

enum SERVICE_FIRBID_TYPE
{
	SERVICE_FORBID_DELIVERY_PRO = 101, //gdeliveryd 屏蔽 glinkd 发来的协议
};
enum
{
	WEB_ORDER_UNPROCESSED = 0,
	WEB_ORDER_PROCESSED = 1,
};
enum
{
	WEB_ORDER_PAY_COUPON = 1, //点券支付 web 商城订单
	WEB_ORDER_PAY_CASH = 2, //元宝支付 web 商城订单
};
enum
{
	ERR_WEBORDER_ARG_INVALID = 101, //参数错误
	ERR_WEBORDER_CASH_NOTENGOUTH = 102, //元宝不足
	ERR_WEBORDER_COMMUNICATION = 103, //服务期间通信错误
	ERR_WEBORDER_INVALID_ROLE = 104, //角色不存在
	ERR_WEBORDER_REPEAT = 105, //游戏判断重复订单 roleid 一致
	ERR_WEBORDER_ORDERID = 106, //游戏判断订单号重复 roleid 不一致
	ERR_WEBORDER_USERID = 107, //角色与账号不匹配
	ERR_WEBORDER_MAXSIZE = 108, //订单数已达上限
	ERR_WEBORDER_ROLE_STATUS = 109, //角色状态错误
};
enum
{
	WEB_ORDER_MAXSIZE = 30, //最多允许存在 30 笔未领取订单
};

// User 中 data_group的枚举  Key 值一旦定义 不得修改
enum USER_DATA_GROUP
{
	USER_DATA_CASHADD2	= 1,
};
//deliveryd 使用;  GRoleBase 中 datagroup的枚举; Key 值一旦定义 不得修改
enum BASE_DATA_GROUP
{
	KEY_DATAGOURP_ROLE_CT_COOLTIME = 1, // 跨服战队离开战队冷却cd
	KEY_DATAGOURP_ROLE_CT_TEAMID = 2, // 跨服战队id
	KEY_DATAGOURP_ROLE_CT_SCORE = 3, // 跨服战队个人积分
	KEY_DATAGOURP_ROLE_CT_MAX_SCORE = 4, // 当前最高积分,不存在GRoleInfo
	KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE = 5, // 上周最高积分
	KEY_DATAGOURP_ROLE_CT_SCORE_UPDATETIME = 6, //当前积分更新时间,不存在GRoleInfo
	KEY_DATAGOURP_ROLE_CT_LAST_PUNISH_TIME = 7, // 最后的惩罚结算时间
	KEY_DATAGOURP_ROLE_CT_LAST_BATTLE_COUNT = 8, // 上周打得场次
};
//GFactionInfo.datagroup 中的枚举 Key 值一旦定义 不得修改
enum FAC_DATA_GROUP
{
	FAC_DATA_MULTI_EXP = 1,		//帮派多倍经验倍数
	FAC_DATA_MULTI_EXP_ENDTIME,	//帮派多倍经验结束时间
};
enum
{
	CT_DIRTY_ROLE_DB = 0x01, //标识战队玩家信息写回
	CT_DIRTY_TEAM_DB = 0x02,//标识战队队伍信息写回
	CT_DIRTY_SCORE_DB = 0x04,   //标识玩家当前积分写回
	CT_DIRTY_ROLE_CENTRAL = 0x08,   // 标识战队玩家同步天界
	CT_DIRTY_TEAM_CENTRAL = 0x10,   //标识战队队伍同步天界
	CT_DIRTY_SCORE_CENTRAL= 0x20,	// 标识玩家积分同步天界 
	CT_DIRTY_ROLE_ONLINE_CENTRAL = 0x40, // 跨服在线状态同步,特殊处理
};

/*
enum
{
	CT_DIRTY_ROLE_SCORE = 0x01,
	CT_DIRTY_ROLE_JOINBATTLE_TIME = 0x02,// 最近一次参与战斗的时间 
	CT_DIRTY_ROLE_BATTLE_TIMES = 0x04, // 打得场次

};
*/
//gs 使用; GRoleBase2 中 datagroup的枚举; Key 值一旦定义 不得修改
enum BASE2_DATA_GROUP
{
	BASE2_DATA_FAC_COUPON_ADD = 1,  //帮派金券累计值
	BASE2_DATA_FAC_COUPON,  //帮派金券
	BASE2_DATA_USED_TITLE,
};

enum
{
	// CrssvrTeams 300-340
	ERR_CRSSVRTEAMS_NOT_OPEN 		= 300, // 战队未开启
	ERR_CRSSVRTEAMS_TEAMID_INVAILD 		= 301, // teamid 错误 
	ERR_CRSSVRTEAMS_DATA_IN_LOADING 	= 302, // 战队数据在loading中 
	ERR_CRSSVRTEAMS_NOTEXIST 		= 303, // 战队不存在 
	ERR_CRSSVRTEAMS_CHANGENAME_CD		= 304, // 改名次数太频繁，cd中
	ERR_CRSSVRTEAMS_CREATE_DUP 		= 305, // 名字重复
	ERR_CRSSVRTEAMS_INVALIDNAME		= 306, // 名字不合法
	ERR_CRSSVRTEAMS_NOT_MASTER              = 307, // 不是队长，不能进行此操作
	ERR_CRSSVRTEAMS_FULL			= 308, // 战队人满
	ERR_CRSSVRTEAMS_CHECKCONDITION     	= 309, // 不满足操作条件，资金不够,在跨服等
	ERR_CRSSVRTEAMS_OFFLINE			= 310, // 玩家不在线
	ERR_CRSSVRTEAMS_LEAVE_COOLDOWN		= 311, // 玩家离开战队，1天后才能再加入
	ERR_CRSSVRTEAMS_ROLE_NOT_IN		= 312, // 玩家不存在战队中
	ERR_CRSSVRTEAMS_JOIN_REFUSE		= 313, // 拒绝加入战队
	ERR_CRSSVRTEAMS_DBFAILURE       	= 314, // 数据库IO错误
	ERR_CRSSVRTEAMS_DB_TIMEOUT              = 315, // 数据库执行超时
	ERR_CRSSVRTEAMS_DB_INTEAMS              = 316, // 玩家已经在战队中
	ERR_CRSSVRTEAMS_DB_NOTIN_TEAMS		= 317, // 玩家在战队数据库中未找到
	ERR_CRSSVRTEAMS_JOIN_INVITEE_IN 	= 318, // 被邀请人已经在队伍中
	ERR_CRSSVRTEAMS_JOIN_NOT_REBORN 	= 319, // 玩家等级没有达到飞升135
	ERR_CRSSVRTEAMS_JOIN_OCCUP_CONT		= 320, // 某个职业人数>2不能招募

	ERR_CRSSVRTEAMS_DISMISS_OK		= 321, // 您的队伍已解散
	ERR_CRSSVRTEAMS_CROSS_CREATE_FAIL	= 322, // 跨服同步数据，创建战队失败
	ERR_CRSSVRTEAMS_CROSS_UPDATE_ROLE_FAIL  = 323, // 跨服同步数据，更新战队玩家信息失败
	ERR_CRSSVRTEAMS_NAME_NOT_FIND		= 324, // 玩家不在线或名字输入有误
	ERR_CRSSVRTEAMS_CROSS_DISCONNECT	= 325, // 跨服未连接，不能进行此项操作
	ERR_CRSSVRTEAMS_INVITE_JOIN_OK		= 326, // 加入战队成功
	ERR_CRSSVRTEAMS_QUIT_TEAM		= 327, // XX退出战队
	ERR_CRSSVRTEAMS_KICK_TEAM		= 328, // 您已被队长踢出战队
	ERR_CRSSVRTEAMS_CHG_CAPTAIN		= 329, // **将队长转移给您
};

enum
{
	CRSSVRTEAMS_OFFLINE_STATUS = 0, // 不在线
	CRSSVRTEAMS_SRC_ONLINE_STATUS = 1, // 原服在线
	CRSSVRTEAMS_CROSS_ONLINE_STATUS = 2, // 跨服在线

};

enum
{
	CRSSVRTEAMS_DAILY_TOP_TYPE   = 1, // 日排行榜类型
	CRSSVRTEAMS_WEEKLY_TOP_TYPE  = 2, // 周排行榜类型
	CRSSVRTEAMS_SEASON_TOP_TYPE  = 3, // 赛季
};

enum
{
	RAID_ATTARCKER_WIN = 1, // 攻方胜
	RAID_DEFENDER_WIN = 2, // 守方胜
	RAID_DRAW = 3, // 平
};

enum
{
	FAC_DIRTY_NORMAL = 0x01, //标识普通字段的写回
	FAC_DIRTY_DYNAMIC = 0x02,//标识帮派动态信息的写回
//	FAC_DIRTY_MSGS = 0x04,   //标识帮派留言的写回
	FAC_DIRTY_ACT = 0x08,    //标识帮派活跃度的写回
};
enum
{
	FAC_ACT_START_LEV = 3, //4级帮开始计算活跃度
//	FAC_ACT_LIMIT = 100000,
};
enum
{
	ST_FAC_BASE_OPEN = 0x01,
	ST_FAC_BASE_START = 0x02, //这个状态不会出现在db中 表示基地已分配gs 可以进入
};

enum
{
	ERR_FAC_BASE_ALREADY_OPEN = 1, //帮派基地已经开启 不能重复开启
	ERR_FAC_BASE_CREATE_LEV = 2, //不足四级 不能创建基地
	ERR_FAC_BASE_CREATE_DB = 3, //创建基地 数据库错误
	ERR_FAC_BASE_START_STATUS = 4, //开启基地状态错误
	ERR_FAC_BASE_START_FULL = 5, //数量已达上限 不能开启
	ERR_FAC_BASE_ENTER_NOTOPEN = 6, //基地未开启 无法操作
	ERR_FAC_BASE_LOGIC = 7, //逻辑错误
	ERR_FAC_BASE_CREATE_DELETING = 8, //帮派为删除状态 不能创建基地
};

enum CHG_GS_REASON //与 gs player_imp.h 中的 CHANGE_GS_REASON 同步修改
{
	CHG_GS_REASON_NORMAL = 0, //普通切换gs
	CHG_GS_REASON_VOTE_LEAVE_RAID = 1, //投票踢出副本
	CHG_GS_REASON_COLLISION_RAID = 2, //进入碰撞副本
	CHG_GS_REASON_LEAVE_RAID = 3, //离开副本
	CHG_GS_REASON_ENTER_FBASE = 4, //进入帮派基地 给客户端用
};

enum TopicSiteType
{
	TS_ADD_FACTION_TYPE = 1,
	TS_ADD_FAMILY_TYPE = 2,
	TS_DELETE_FACTION_TYPE = 3,
	TS_DELETE_FAMILY_TYPE = 4,
	//TS_ADDFRIEND_TYPE = 5,
};

// 捉迷藏类副本
enum HideSeekType
{
	HST_HIDESEEK = 1, // 捉迷藏副本
	HST_FENGSHEN = 2, // 飞升150-160副本
};




//by 2022 add

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
	GP_MOVE_PULLED	= 10,		//	拉扯状态的移动 Added 2011-07-29.
	GP_MOVE_MASK	= 0xf,

	GP_MOVE_TURN	= 0x10,		//	Turnaround
	GP_MOVE_DEAD	= 0x20,

	GP_MOVE_AIR		= 0x40,
	GP_MOVE_WATER	= 0x80,
	GP_MOVE_ENVMASK	= 0xc0,
};

//	Leave team reason
enum
{
	GP_LTR_LEAVE = 0,		//	Normal leave
	GP_LTR_KICKEDOUT,		//	Was kicked out
	GP_LTR_LEADERCANCEL,	//	Leader cancel the team
};

//	Team member pickup flag
enum
{
	GP_TMPU_RANDOM = 0,
	GP_TMPU_FREEDOM,
};

//	NPC service type
enum
{
	GP_NPCSEV_BUY = 1,			//	1, NPC buy from player
	GP_NPCSEV_SELL,				//	NPC sell to player
	GP_NPCSEV_REPAIR,
	GP_NPCSEV_HEAL,
	GP_NPCSEV_TRANSMIT,			//	Transmit to somewhere
	
	GP_NPCSEV_TASK_RETURN,		//	6, Return task
	GP_NPCSEV_TASK_ACCEPT,		//	Accept task
	GP_NPCSEV_TASK_MATTER,		//	Task matter
	GP_NPCSEV_LEARN,			//	Learn skill
	GP_NPCSEV_EMBED,			//	Embed stone

	GP_NPCSEV_CLEAR_TESSERA,	//	11, Clear tessear
	GP_NPCSEV_MAKEITEM,
	GP_NPCSEV_RESET_PK_VALUE,
	GP_NPCSEV_TRASHPSW,			//	Change trash password
	GP_NPCSEV_OPENTRASH,		//	Open trash

	GP_NPCSEV_OPENFACTIONTRASH,	//	16
	GP_NPCSEV_IDENTIFY,			//	Identify item
	GP_NPCSEV_FACTION,			//	About faction
	GP_NPCSEV_BOOTHSELL,		//	Player booth sell		
	GP_NPCSEV_TRAVEL,			//	Travel

	GP_NPCSEV_BOOTHBUY,			//	21, Player booth buy
	GP_NPCSEV_WAYPOINT,
	GP_NPCSEV_FORGETSKILL,
	GP_NPCSEV_FACECHANGE,
	GP_NPCSEV_MAIL,

	GP_NPCSEV_VENDUE,			//	26
	GP_NPCSEV_DBLEXPTIME,
	GP_NPCSEV_HATCHPET,
	GP_NPCSEV_RESTOREPET,
	GP_NPCSEV_BATTLE,
	
	GP_NPCSEV_BUILDTOWER,		//	31
	GP_NPCSEV_LEAVEBATTLE,
	GP_NPCSEV_RETURNSTATUSPT,
	GP_NPCSEV_ACCOUNTPOINT,
	GP_NPCSEV_BINDITEM,

	GP_NPCSEV_DESTROYBINDITEM,	//	36
	GP_NPCSEV_CANCELDESTROY,
	GP_NPCSEV_STOCK_TRANSACTION,
	GP_NPCSEV_STOCK_OPERATION,
	GP_NPCSEV_TALISMAN_REFINE,

	GP_NPCSEV_TALISMAN_UPGRADE,	// 41
	GP_NPCSEV_TALISMAN_REMOVE_OWNER,
	GP_NPCSEV_TALISMAN_COMBINE,
	GP_NPCSEV_TALISMAN_ENCHANT,
	GP_NPCSEV_WAR_CHALLENGE,

	GP_NPCSEV_WAR_CONSTRUCTION, // 46
	GP_NPCSEV_WAR_ARCHER,
	GP_NPCSEV_ADOPT_PET,
	GP_NPCSEV_FREE_PET,
	GP_NPCSEV_COMBINE_PET,
	
	GP_NPCSEV_RENAME_PET,		// 51
	GP_NPCSEV_VEHICLE_UPGRADE,
	GP_NPCSEV_ITEM_TRADE,
	GP_NPCSEV_LOCK_ITEM,
	GP_NPCSEV_UNLOCK_ITEM,

	GP_NPCSEV_REPAIR_DAMAGED_ITEM,
	GP_NPCSEV_BLEED_SACRIFICE,		//装备血祭
	GP_NPCSEV_EMBED_SOUL,			//器魄镶嵌
	GP_NPCSEV_CLEAR_SOUL_TESSERA,	//器魄拆除
	GP_NPCSEV_REFINE_SOUL,			//恢复魂力

	GP_NPCSEV_BREAKDOWN_EQUIP,		//61      //装备拆解 
	GP_NPCSEV_MELD_SOUL,			//器魄融合
	GP_NPCSEV_PET_EQUIP_ENCHANT,	//宠物装备强化
	GP_NPCSEV_CHANGE_FACE,			//形象美容
	GP_NPCSEV_REFINE_SHAPE_CARD,	//炼化幻化石（变身卡）包括：归元，强化，契合，轮回

	GP_NPCSEV_RECOVER_CARD_WAKAN,	//66 恢复幻化石（变身卡）的灵力值
	GP_NPCSEV_ARENA_JOIN,			//竞技场报名，以前走的是协议battle_jion，发协议给delivery, 因为要在报名的时候扣报名券，改成服务发协议给GS
	GP_NPCSEV_TERRITORY_CHALLENGE,	//领土战宣战
	GP_NPCSEV_TERRITORY_ENTER,		//进入领土战战场

	GP_NPCSEV_TERRITORY_AWARD,		//70 领土战奖励
	GP_NPCSEV_TELEPORTATION_CHARGE,	//星盘充能服务
	GP_NPCSEV_REPAIR_DAMAGED_ITEM2,	//修复破损物品的特殊服务
	GP_NPCSEV_UPGRADE_EQUIP,		//装备升级服务
	GP_NPCSEV_ONLINE_TRADE,			//网上交易

	GP_NPCSEV_TRANSMIT_TO_CROSS,	//75 传送到跨服服务器
	GP_NPCSEV_TRANSMIT_TO_NORMAL,	//从跨服服务器传送回普通服务器
	GP_NPCSEV_IDENTIFY_GEM_SLOTS,	//宝石插槽鉴定服务
	GP_NPCSEV_REBUILD_GEM_SLOTS,	//宝石插槽重铸服务
	GP_NPCSEV_CUSTOMIZE_GEM_SLOTS,	//宝石插槽定制服务

	GP_NPCSEV_EMBED_GEMS,			//80 宝石镶嵌服务
	GP_NPCSEV_REMOVE_GEMS,			//宝石拆除服务
	GP_NPCSEV_UPGRADE_GEMS,			//宝石升品服务
	GP_NPCSEV_REFINE_GEMS,			//宝石精炼服务
	GP_NPCSEV_EXTRACT_GEMS,			//宝石萃取服务

	GP_NPCSEV_SMELT_GEMS,			//85 宝石熔炼服务
	GP_OPEN_RAID_ROOM,				//开启副本房间服务 Added 2011-07-19.
	GP_JOIN_RAID_ROOM,				//报名加入副本房间服务 Added 2011-07-19.
	GP_PET_UPGRADE_WITH_XIANDOU,	//直接用仙豆提升宠物
	GP_CHANGE_NAME,					//改名服务

	GP_NPCSEV_CHANGE_FAMILYGUILD_NAME,	//90 帮派、家族改名
	GP_NPCSEV_TALISMAN_HOLEYLEVELU,		//法宝飞升
	GP_NPCSEV_TALISMAN_EMBEDSKILL,		//法宝技能镶嵌
	GP_NPCSEV_TALISMAN_SKILLREFINE,		//法宝技能融合
	GP_NPCSEV_TALISMAN_SKILLREFINE_RESULT,	//法宝技能融合是否接收	弃用，改为协议

	GP_NPCSEV_SHOP_REPUTATION,     //95  声望商店服务
	GP_NPCSEV_EQUIP_UPDATE2,		//装备升级为封神装, 废弃
	GP_NPCSEV_EQUIP_SLOT2,			//封神装备打孔
	GP_NPCSEV_ASTROLOGY_INSTALL,	//星座镶嵌
	GP_NPCSEV_ASTROLOGY_UNINSTALL,	//星座摘除

	GP_NPCSEV_ASTROLOGY_IDENTIFY,	//100星座鉴定, 废弃
	GP_NPCSEV_ASTROLOGY_UPGRADE,	//星座升级, 废弃
	GP_NPCSEV_ASTROLOGY_DESTORY,	//星座粉碎, 废弃
	GP_NPCSEV_ACTIVE_UI_TRANSFER,   //传送阵激活
	GP_NPCSEV_USE_UI_TRANSER,		//使用传送阵传送

	GP_NPCSEV_TRANSFER_SERVE = 105,		// 传送服务
	GP_NPCSEV_TRANSFER_SERVE_BATTLE_LEAVE,	// 从战场离开服务

	GP_NPCSEV_KINGDOM_ENTER = 107, //进入国战
	GP_NPCSEV_PRODUCE_JINFASHEN,	// 金法身快速制作
	GP_NPCSEV_PET_FLY,				// 宠物飞升
};

//	Player and NPC state
enum
{
    GP_STATE_SHAPE				= 0x00000001,
	GP_STATE_EMOTE				= 0x00000002,
	GP_STATE_INVADER			= 0x00000004,
	GP_STATE_FLY				= 0x00000008,
	GP_STATE_FAMILY				= 0x00000010,
    GP_STATE_SITDOWN			= 0x00000020,
    GP_STATE_EXTEND_PROPERTY	= 0x00000040,
    GP_STATE_CORPSE				= 0x00000080,

	//	Used only by player
	GP_STATE_TEAM				= 0x00000100,
	GP_STATE_TEAMLEADER			= 0x00000200,
	GP_STATE_ADV_MODE			= 0x00000400,
	GP_STATE_FACTION			= 0x00000800,
	GP_STATE_BOOTH				= 0x00001000,
	GP_STATE_FASHION			= 0x00002000,
	GP_STATE_GMFLAG				= 0x00004000,
	GP_STATE_PVP_ENABLE			= 0x00008000,
	GP_STATE_EFFECT				= 0x00010000,
	GP_STATE_INPVPCOMBAT		= 0x00020000,
	GP_STATE_IN_DUEL			= 0x00040000,	//	是否正在决斗中
	GP_STATE_IN_MOUNT			= 0x00080000,	//	正在骑乘中
	GP_STATE_IN_BIND			= 0x00100000,	//	和别人绑在一起
	GP_STATE_BC_INVADER			= 0x00200000,	//	Battle camp: invader
	GP_STATE_BC_DEFENDER		= 0x00400000,	//	Battle camp: defender
	GP_STATE_TITLE				= 0x00800000,	//	后面跟一个当前称谓id, short类型
	GP_STATE_SPOUSE				= 0x01000000,
	GP_STATE_SECT_MASTER_ID		= 0x08000000,
	GP_STATE_IN_SJBATTLE		= 0x10000000,	//  在宋金战场中
	GP_STATE_IN_VIPSTATE		= 0x20000000,	//	使用了vip卡片
	GP_STATE_IS_OBSERVER		= 0x40000000,	//  跨服pk的观战者模式
	GP_STATE_TRANSFIGURE		= 0x80000000,	//  变身状态
	
	//	Used only by NPC
	GP_STATE_NPC_DELAYDEAD		= 0x00000008,
	GP_STATE_NPC_ADDON1			= 0x00000100,
	GP_STATE_NPC_ADDON2			= 0x00000200,
	GP_STATE_NPC_ADDON3			= 0x00000400,
	GP_STATE_NPC_ADDON4			= 0x00000800,
	GP_STATE_NPC_ALLADDON		= 0x00000F00,
	GP_STATE_NPC_PET			= 0x00001000,	//	Pet flag
	GP_STATE_NPC_NAME			= 0x00002000,
	GP_STATE_NPC_SUMMON			= 0x00004000,	//  属于召唤怪
	GP_STATE_NPC_OWNER			= 0x00008000,	//  有归属的怪物
	GP_STATE_NPC_CLONE			= 0x00010000,	//  NPC是分身，客户端暂时不用这个状态
	GP_STATE_NPC_DIM			= 0x00020000,	//  NPC朦胧
	GP_STATE_NPC_FROZEN			= 0x00040000,	//	NPC冰冻状态
	GP_STATE_NPC_DIR_VISIBLE	= 0x00080000,	//	十字方向技能效果状态
	GP_STATE_NPC_INVISBLE       = 0x00100000,   //  NPC隐身
	GP_STATE_NPC_TELEPORT1      = 0x00200000,   //  传送类NPC（祝福）
	GP_STATE_NPC_TELEPORT2      = 0x00400000,   //  传送类NPC（诅咒）
	GP_STATE_NPC_PHASE			= 0x00800000,	//  相位标记NPC
};

enum
{
	GP_STATE_MATTER_OWNER		= 0x00000004,	// 有归属的矿物
	GP_STATE_MATTER_PHASE       = 0x00000008,   // 有相位标记的MATTER
	GP_STATE_COMBINE_MINE       = 0x00000020,   // 复合矿
};

enum
{
	GP_EXTRA_STATE_CARRIER			= 0x00000001,	//  玩家在载体上面
	GP_EXTRA_STATE_ANONYMOUS		= 0x00000002,	//  玩家处于匿名地图
	GP_EXTRA_STATE_INVISIBLE		= 0x00000004,	//  玩家隐身（自己看自己是半透明的，如果别人能看见自己的话，别人看自己也是半透明的）
	GP_EXTRA_STATE_DIM				= 0x00000008,	//  进入朦胧（效果是半透明，自己看或者别人看效果都是半透明）
	GP_EXTRA_STATE_CLONE			= 0x00000010,	//  存在一个分身
	GP_EXTRA_STATE_CIRCLE			= 0x00000020,	//  圈子信息
	GP_EXTRA_STATE_CROSSZONE		= 0x00000040,	//  大跨服
	GP_EXTRA_STATE_IGNITE			= 0x00000080,	//  点燃状态
	GP_EXTRA_STATE_FROZEN			= 0x00000100,   //  冰封状态
	GP_EXTRA_STATE_FURY				= 0x00000200,	//  神圣之怒状态
	GP_EXTRA_STATE_DARK				= 0x00000400,	//  黑暗之术
	GP_EXTRA_STATE_COLDINJURE		= 0x00000800,	//  冻伤状态
	GP_EXTRA_STATE_CIRCLEOFDOOM		= 0x00001000,	//	阵法状态，只有在阵眼（阵法发起者）身上才有 Added 2011-06-22.
	GP_EXTRA_STATE_BE_DRAGGED		= 0x00010000,	//	被牵引状态 //Added 2011-08-26.上面几个不连续的分别是：逆转正面，逆转负面，嗜血状态
	GP_EXTRA_STATE_BE_PULLED		= 0x00020000,	//	被拉扯状态 // Added 2011-08-26.
	GP_EXTRA_STATE_VIP_AWARD		= 0x00040000,	//	VIP奖励状态 Added 2012-01-16.
	GP_EXTRA_STATE_COLLISION_RAID	= 0x00080000,	//	处于碰撞副本时的状态	Added 2012-08-01.
	GP_EXTRA_STATE_XY_DARK_LIGHT	= 0x00100000,	//	轩辕光暗状态	Added 2012-08-16.
	GP_EXTRA_STATE_XY_SPIRITS		= 0x00200000,	//	轩辕灵体
	GP_EXTRA_STATE_MIRROR_IMAGE		= 0x00400000,	//	镜像
	GP_EXTRA_STATE_CHANGE_WING_COLOR= 0x00800000,	//	飞剑颜色变化 Added 2012-08-20.
	GP_EXTRA_STATE_DIR_VISIBLE_STATE= 0x01000000,	//	十字方向技能效果状态
	GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT = 0X02000000,	//	装备额外光效
	GP_EXTRA_STATE_FLOW_BATTLE = 0X04000000,		//	流水席状态
	GP_EXTRA_STATE_PUPPET			= 0x08000000,   //  傀儡姿态
	GP_EXTRA_STATE_INTERACT			= 0x20000000,	//	互动状态
	GP_EXTRA_STATE_ACTIVE_EMOTE		= 0x40000000,	//	互动表情
	GP_EXTRA_STATE_TRAVEL			= 0x80000000,	//	自由飞行
};
//	Chat channel
enum
{
	//这个枚举定义如果改动的话，要通知所有人，包括服务器程序员
	//如非必要，尽可能在后面添加，避免删除和更改前面channel的顺序
	GP_CHAT_LOCAL = 0,
	GP_CHAT_FARCRY,
	GP_CHAT_TEAM,
	GP_CHAT_FACTION,
	GP_CHAT_WHISPER,
	GP_CHAT_DAMAGE,
	GP_CHAT_FIGHT,
	GP_CHAT_TRADE,
	GP_CHAT_SYSTEM,
	GP_CHAT_BROADCAST,
	GP_CHAT_MISC,
	GP_CHAT_FAMILY,
	GP_CHAT_CIRCLE,
	GP_CHAT_ZONE,
	GP_CHAT_SPEAKER,
	GP_CHAT_RUMOR,		// 匿名大喇叭捌档
	GP_CHAT_MAX,
};

//	The reason of droping item
enum
{
	GP_DROP_GM,
	GP_DROP_PLAYER,
	GP_DROP_TAKEOUT,
	GP_DROP_TASK,
	GP_DROP_RECHARGE,
	GP_DROP_DESTROY,
	GP_DROP_DEATH,
	GP_DROP_PRODUCE,
	GP_DROP_DECOMPOSE,
	GP_DROP_TRADEAWAY,
	GP_DROP_RESURRECT,
	GP_DROP_USED,
	GP_DROP_CONSUMERUNE,
	GP_DROP_EXPIRED,
	GP_DROP_BIND_DISAPPEAR,
	GP_DROP_DAMAGED,
};

//	Cool time index
enum
{
	GP_CT_NULL = 0,					//	空，保留
	GP_CT_EMOTE,                   	//	做表情的冷却时间 
	GP_CT_DROP_MONEY,              	//	往地上扔钱的冷却时间
	GP_CT_DROP_ITEM,               	//	往地上扔物品的冷却时间
	GP_CT_SWITCH_FASHION,          	//	切换时装模式的冷却时间
	// 5
	GP_CT_TRANSMIT_ROLL,			//	传送卷轴
	GP_CT_VIEWOTHEREQUIP,			//	查看别人的装备
	CP_CT_FIREWORKS,				//	施放烟花
	CP_CT_FARCRY,					//	far cry 频道说话
	CP_CT_SKILLMATTER,				//  技能物品
	// 10
	CP_CT_DOUBLEEXPMATTER,			//  双倍经验道具
	GP_CT_SKILLMATTER1,				//	技能物品冷却时间类型1
	GP_CT_SKILLMATTER2,				//	技能物品冷却时间类型2
	GP_CT_SKILLMATTER3,				//	技能物品冷却时间类型3
	GP_CT_RESERVED,					//	保留
	// 15
	GP_CT_VEHICLE,					//	召唤交通工具
	GP_CT_COUPLE_JUMPTO,			//	夫妻同心结
	GP_CT_LOTTERY,					//	彩票
	GP_CT_CAMRECORDER,				//	录像道具   
	GP_CT_CAMRECORDCHECK,			//	录像验证			
	// 20
	GP_CT_TEAM_RELATION,
	GP_CT_CD_ERASER,
	GP_CT_PET_COMBINE1,				//	宠物通灵			
	GP_CT_PET_COMBINE2,				//	宠物御宝			
	GP_CT_PET_PICKUP,				//	拾取
	// 25
	GP_CT_SET_FASHION_MASK,			//	
	GP_CT_GET_BATTLEINFO,			//	获取战场信息的冷却
	GP_CT_HP,						//  瞬回hp药品冷却时间
	GP_CT_MP,
	GP_CT_HP_MP,
	// 30
	GP_CT_SLOW_HP,					//  持续回hp药品冷却时间
	GP_CT_SLOW_MP,
	GP_CT_SLOW_HP_MP,
    GP_CT_QUERY_OTHER_ACHIEVMENT,	//  查看其他玩家成就冷却
    GP_CT_TRANSFIGURE,				//  变身冷却
	// 35
	GP_CT_NEWBIE_CHAT,				//  30级以下普通频道说话冷却   
	GP_CT_CHANGE_DS,				//  大跨服冷却
 	GP_CT_DP,						//  瞬回dp药品冷却时间
	GP_CT_SLOW_DP,					//	持续回dp药品冷却时间
	GP_CT_GET_MALL_PRICE,			//  取商城销售数据
	// 40
	GP_CT_GET_VIP_AWARD_INFO,		//	获取VIP奖励列表信息冷却时间 Added 2012-01-04.
	COOLDOWN_INDEX_RESET_SKILL_PROP,		// 90级以下技能重置冷却
	COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT,	// 90级以下天书重置冷却
	COOLDOWN_INDEX_BATTLE_FLAG,				// 战旗使用冷却
	COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON,	// 玩家首次退出游戏冷却
	// 45
	COOLDOWN_INDEX_BOTTLE_HP,
	COOLDOWN_INDEX_BOTTLE_MP,
	COOLDOWN_INDEX_BOTTLE_DP,
	GP_CT_SKILLMATTER4,
	GP_CT_SKILLMATTER5,
	//50
	COOLDOWN_INDEX_BATH,
	COOLDOWN_INDEX_KING_CALL_GUARD,
	COOLDOWN_INDEX_ARRANGE_INVENTORY,
	COOLDOWN_INDEX_ARRANGE_TRASHBOX,
	COOLDOWN_INDEX_TELEPORT1,      //祝福传送

	//55
	COOLDOWN_INDEX_TELEPORT2,		//诅咒传送
	COOLDOWN_INDEX_CHANGE_PUPPET_FIGHT_FORM,//傀儡战斗状态转换

	GP_CT_MAX,
	GP_CT_PRODUCE_START = 600,
	GP_CT_PRODUCE_END = 800,
	
	GP_CT_COMBINE_EDIT_START	= 900,
	GP_CT_COMBINE_EDIT_END		= 950,

	GP_CT_SKILL_START = 1024,
};

//	PVP mask
enum
{
	GP_PVPMASK_WHITE	= 0x0001,
	GP_PVPMASK_RED		= 0x0002,
	GP_PVPMASK_BLUE		= 0x0004,
	GP_PVPMASK_MAFIA	= 0x0008,
	GP_PVPMASK_ZONE		= 0x0010,	// 跨服后攻击同一个服务器的玩家
	GP_PVPMASK_ALL		= 0x001F,
};

enum
{
	GP_PET_SKILL_NUM	= 16
};

//	Pet type
enum
{   
	GP_PET_CLASS_INVALID = -1,
	GP_PET_CLASS_MOUNT = 0, //	骑宠
	GP_PET_CLASS_COMBAT,    //	战斗宠物
	GP_PET_CLASS_FOLLOW,    //	跟随宠物
	GP_PET_CLASS_MAX,
};

//	Player camp in battle
enum
{
	GP_BATTLE_CAMP_NONE = 0,
	GP_BATTLE_CAMP_INVADER,
	GP_BATTLE_CAMP_DEFENDER,
	GP_BATTLE_CAMP_OBSERVER,	// 观察者
	GP_BATTLE_CAMP_ARENA,		// 竞技场中无阵营，将阵营统一为4
};



enum
{
    BATTLE_TYPE_NORMAL         = 0, //普通战场
    BATTLE_TYPE_NAMED_CRSSVR   = 1, //非匿名的跨服战场
	BATTLE_TYPE_INSTANCING	   = 2, //剧情战场	
    BATTLE_TYPE_REBORN         = 3, //飞升战场
    BATTLE_TYPE_SONJIN         = 4, //宋金战场
    BATTLE_TYPE_SONJINREBORN   = 5, //宋金飞升战场
	BATTLE_TYPE_ARENA		   = 6, //竞技场
	BATTLE_TYPE_ARENAREBORN	   = 7, //飞升竞技场
	BATTLE_TYPE_CRSSVR		   = 8,	//匿名跨服战场
	BATTLE_TYPE_CHALLENGE	   = 9, //挑战战场
	BATTLE_TYPE_CRSSVRTEAM	   = 10,//跨服小队pk战场 Added 2011-05-19
	BATTLE_TYPE_FLPW_CRSSVR   = 11,	//跨服流水席		
};
enum
{
    BATH_TYPE_NORMAL = 0, //泡澡地图 2013-04-19
};
enum COLLISION_SKILL_TYPE				//碰撞技能类型
{
	COLLISION_SKILL_MAXSPEED	= 0,	//加速技能
	COLLISION_SKILL_HALFSPEED,			//减速技能
	COLLISION_SKILL_ASHILL,				//不动如山技能
	COLLISION_SKILL_ROOT,				//定身技能
	COLLISION_SKILL_CHAOS,				//混乱技能
	COLLISION_SKILL_VOID,				//虚无技能
};

enum									//碰撞副本中技能后状态
{
	COLLISION_STATE_ASHILL		= 0x00000001,	//不动如山
	COLLISION_STATE_ROOT		= 0x00000002,	//定身状态
	COLLISION_STATE_CHAOS		= 0x00000004,	//混乱状态
	COLLISION_STATE_VOID		= 0x00000008,	//虚无状态
};

enum
{
	MAX_WING_COLOR_COUNT		= 6,			//飞剑变色的最大种类数目
	COLLISION_MAX_SPEED			= 15,			//碰撞副本中最大移动速度
	COLLISION_PLAYER_MAX_MASS	= 1000000,		//碰撞副本中最大质量，用于不动如山技能
	MAX_MULTILINE_SKILL_GFX_NUM	= 6,			//多线性技能攻击时，最大线条数
};

enum RAID_FACTION
{
     RF_NONE,                         //无阵营
     RF_ATTACKER,                     //攻击方
     RF_DEFENDER,                     //防守方
	 RF_VISITOR,                      //观察者
};

enum ERR_PHONE_TOKEN
{
	ERR_PHONE_TOKEN_FAILURE = 29, //手机令牌验证码不正确或过期
};

enum STATUS_TOKEN
{
	_STATUS_DISCONNECT,
	_STATUS_TRY_RECONNECT,
};


}; //end of GNET

#endif
