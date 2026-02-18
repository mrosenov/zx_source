#ifndef __ONLINEGAME_GS_CONFIG_H__ 
#define __ONLINEGAME_GS_CONFIG_H__ 

#include "dbgprt.h" 
#define GL_MAX_MATTER_COUNT 	164000		//不可超过65536*16
#define GL_MAX_NPC_COUNT 	164000		//不可超过65536*16
#define GL_MAX_PLAYER_COUNT 	8192
#define MAX_PLAYER_IN_WORLD	(4096*4)
#define MAX_GS_NUM		1024

#define TEAM_MAX_RACE_COUNT	8		//最大组队职业数， 这个数字要和elements_data中一致
#define ITEM_LIST_BASE_SIZE	24		//最初的包裹栏大小

//修改包裹大小的时候一定要修改gamed/gmailendsync.hpp中同步包裹的大小，不然状态会错误
#define ITEM_LIST_MAX_ACTUAL_SIZE	252	//实际最大的包裹栏大小
#define ITEM_LIST_MAX_SIZE	252		//最大的包裹栏大小
//#define EQUIP_LIST_SIZE		13	//每个槽位的定义看item.h
#define TASKITEM_LIST_SIZE	30		//任务包裹栏的大小
#define TRASHBOX_BASE_SIZE	24		//仓库的最初大小
#define TRASHBOX_MAX_SIZE	252		//最大的仓库大小
#define MAFIA_TRASHBOX_BASE_SIZE 6		//帮派仓库最初的大小
#define MAX_PET_BEDGE_LIST_SIZE	32		//最大宠物牌包裹栏大小
#define INIT_PET_BEDGE_LIST_SIZE 2		//初始化宠物牌包裹栏大小
#define PET_EQUIP_LIST_SIZE	12		//宠物装备包裹栏大小
#define POCKET_INVENTORY_BASE_SIZE 0		//随身包裹初始大小
#define POCKET_INVENTORY_MAX_SIZE 252		//最大随身包裹大小
#define FASHION_INVENTORY_SIZE 120 		//时装包裹大小
#define REPURCHASE_INVENTORY_SIZE 18	//物品回购包裹大小
#define MOUNT_WING_INVENTORY_BASE_SIZE 0    //坐骑飞剑包裹的初始大小
#define MOUNT_WING_INVENTORY_MAX_SIZE 18	//坐骑飞剑包裹最大size
#define GIFT_INVENTORY_SIZE	12		//元宝积分包裹大小
#define FUWEN_INVENTORY_SIZE	6		//符文包裹大小
#define HOTKEY_MAX_SIZE 10			//最多的热键组合数量
#define PET_SUMMON_ALLOWED_COUNT 1		//允许召唤的宠物数量
#define MAX_PET_LEVEL		160		//最大宠物级别
#define DEFAULT_GRID_SIGHT_RANGE 40.f		//默认视野距离

#define MAX_REBORN_COUNT	3		//最多转生次数
#define MIN_REBORN_LEVEL	135		//最小转生等级

#define MATTER_ITEM_LIFE	120
#define MATTER_ITEM_PROTECT	30
#define PICKUP_DISTANCE		10.f
#define MAGIC_CLASS		5
#define MAX_MAGIC_FACTION	50
#define MAX_MESSAGE_LATENCY	256*20
#define MAX_AGGRO_ENTRY		50
#define MAX_SOCKET_COUNT	4
#define MAX_SPAWN_COUNT		10240
#define MAX_MATTER_SPAWN_COUNT  10240	
#define MAX_PLAYER_SESSION	64
#define NORMAL_COMBAT_TIME	15		//通常战斗时间，使被人攻击后的时间
#define MAX_COMBAT_TIME		15		//最大战斗时间，是攻击别人所产生的时间	
#define MAX_HURT_ENTRY		512		//伤害列表中最多的条目数目
#define LOGOUT_TIME_IN_NORMAL   3		//3秒普通的登出时间
#define LOGOUT_TIME_IN_COMBAT   15		//15秒登出时间
#define LOGOUT_TIME_IN_RECONNECT 1		//1秒登出时间
#define LOGOUT_TIME_IN_TRAVEL   30		//30秒旅行登出时间
#define SELECT_IGNORE_RANGE	170.f		//选定目标的限制距离

#define NPC_IDLE_TIMER		20		//20秒进入IDLE状态
#define NPC_IDLE_HEARTBEAT	30		//每60次普通心跳进行一次idle状态的心跳
#define LEAVE_IDLE_MSG_LIMIT	40		//40个tick发送一次，即限制为2秒
#define TICK_PER_SEC		20		//一秒钟有多少个tick，这个值不能随意变动

#define GET_EQUIP_INFO_DIS	200.f		//取得装备信息的限制距离（水平方向）
#define TEAM_INVITE_DIS		100.f		//邀请的限制距离
#define TEAM_EXP_DISTANCE	60.f		//组队经验的限制距离
#define TEAM_ITEM_DISTANCE	60.f		//组队随机分配物品的距离
#define NORMAL_EXP_DISTANCE	60.f		//组队经验的限制距离
#define TEAM_MEMBER_CAPACITY	6		//组队的最大人数
#define TEAM_WAIT_TIMEOUT	5		//5秒，即5个心跳
#define TEAM_INVITE_TIMEOUT	30		//邀请超时时间 30 秒, 邀请的超时
#define TEAM_INVITE_TIMEOUT2	25		//邀请超时时间  被邀请方的超时,应该小于上面的邀请方超时
#define TEAM_LEADER_TIMEOUT	30		//三十秒member没有收到来自队长的消息，即超时
#define TEAM_MEMBER_TIMEOUT	15		//15秒没受到队员的消息，即超时
#define TEAM_LEADER_UPDATE_INTERVAL 20		//每隔20秒leader更新所有数据
#define TEAM_LEADER_NOTIFY_TIME	10		//每隔10秒队长会通知所有队员自己仍在的信息
#define MAX_PROVIDER_PER_NPC	48		//每个NPC最多提供服务的种类
#define MAX_SERVICE_DISTANCE	10.f		//最大的服务距离为 10.f 米
#define DURABILITY_UNIT_COUNT	100		//外部显示的一个耐久度单位对应内部的值
#define DURABILITY_DEC_PER_HIT 	25		//防具每次被击中减少的耐久值
#define DURABILITY_DEC_PER_ATTACK 2		//武器每次攻击减少的耐久值
#define TOSSMATTER_USE_TIME	40		//暗器的使用时间固定是1.5秒
#define MAX_TOWN_REGION		1024
#define LINK_NOTIFY_TIMER	33
#define MAX_EXTEND_OBJECT_STATE 32		//每个对象最多同时存在的扩展属性数目
#define UNARMED_ATTACK_DELAY	12		//空手的攻击延迟时间的是0.3秒
#define HELP_RANGE_FACTOR	1.0f		//求救距离和视野距离的因子
#define DEFAULT_AGGRESSIVE_RANGE 15.f		//标准的最大索敌范围，参数里可以重新设定
#define MAX_INVADER_ENTRY_COUNT 10		//最多纪录多少给橙名对象
#define MAX_PLAYER_ENEMY_COUNT  20		//最大的玩家敌人数目
#define PARIAH_TIME_PER_KILL    7200		//每次杀人变成的红名累计时间
#define PARIAH_TIME_REDUCE	72		//杀一只高级怪所减少pk值
#define MAX_PARIAH_TIME		(PARIAH_TIME_PER_KILL*100)	//最大的PK值
#define DEATH_PROTECT_LEVEL	15		//死亡保护的级别
#define PVP_PROTECT_LEVEL	30		//可以进行PK的级别
#define MATTER_HEARTBEAT_SEC	11		//物品每多少秒一次心跳
#define GATHER_RANGE_LIMIT	6.f		//采集矿物的距离限制
#define TRASHBOX_MONEY_CAPACITY	2000000000	//仓库里最大金钱数量
#define MONEY_CAPACITY_BASE	2000000000
#define MONEY_CAPACITY_PER_LVL	200000000
#define MAX_ITEM_DROP_COUNT	65535		//掉落物品时每次最多掉落的数目
#define MONEY_DROP_RATE		0.7f		//金钱掉落概率
#define MONEY_MATTER_ID		565		//金钱的掉落物品id
#define FLEE_SKILL_ID 		40		//怪物逃跑的技能ID
#define SUICIDE_ATTACK_SKILL_ID	147		//自爆攻击的技能ID
#define WORLD_SPEAKER_ID	2530		//千里传音所用物品ID
#define WORLD_SPEAKER_ID2	33056		//千里传音所用物品ID2
#define ITEM_DESTROYING_ID	2965		//绑定物品摧毁中的ID
#define STAYIN_BONUS		100		//打坐的加成
#define PLAYER_BODYSIZE		0.3f		//玩家的体型大小
#define MAX_MASTER_MINOR_RANGE	400.f		
#define BASE_REBORN_TIME	15		//至少5秒复生时间
#define NPC_FOLLOW_TARGET_TIME	0.5f		//现在试验一下NPC追击的最小时间
#define NPC_FLEE_TIME		0.5f		//现在试验一下NPC逃跑的最小时间
#define MAX_FLIGHT_SPEED	20.f		//最大飞行速度
#define MAX_RUN_SPEED		15.f		//最大跑动速度
#define MAX_WALK_SPEED		8.f		//最大行走速度
#define MIN_RUN_SPEED		0.1f		//最小跑动速度
#define MIN_WALK_SPEED		0.1f		//最小行走速度
#define MAX_JUMP_SPEED		10.f		//最大跳跃速度
#define PLAYER_DEATH_EXP_VANISH	0.03f		//死亡时需要减少的经验比例
#define NPC_PATROL_TIME		1.0f
#define PLAYER_MARKET_SLOT_CAP	36
#define PLAYER_MAERKET_NAME_LEN 28
#define PVP_DAMAGE_REDUCE	1.0f		//PVP攻击时乘的系数
#define MAX_PLAYER_LEVEL	200		//物理最大级别，不要随意调整
#define MAX_TALISMAN_LEVEL	100		//物理最大法宝级别
#define MAX_WAYPOINT_COUNT	1024		//最大的路点数目
#define NPC_REBORN_PASSIVE_TIME 5		//怪物重生后进入主动的等待时间
#define PVP_STATE_COOLDOWN	(10*3600)
#define WATER_BREATH_MARK	3.0f		//水深两米以下才启动呼吸
#define MAX_PLAYER_EFFECT_COUNT 32
#define PLAYER_REBORN_PROTECT	5		//玩家复活后获得几秒钟的放逐时间(不能操作，不能治疗，不能运动)
#define CRIT_DAMAGE_BONUS	2.0f		//重击产生的伤害加成
#define PLAYER_HP_GEN_FACTOR	5		//玩家回血的因子
#define PLAYER_MP_GEN_FACTOR	10		//玩家回魔的因子
#define MAX_USERNAME_LENGTH	20		//玩家名字的最大长度
#define PVP_COMBAT_HIGH_TH	60		//PVP格斗时间的设置
#define PVP_COMBAT_LOW_TH	45		//PVP格斗时间小于一定值才会进行反馈刷新
#define MAX_DOUBLE_EXP_TIME	(4000*3600)	//最大积累双倍时间
#define MAX_MULTI_EXP_TIME	(4000*3600)	//最大积累双倍时间
#define MIN_DUEL_LEVEL		10		//最低的决斗级别
#define MIN_PK_VALUE		-999		//最小PK值
#define MAX_PK_VALUE		999		//最大PK值
#define REPUTATION_REGION	32		//有声望的区域数目
#define MAX_REPUTATION		200000000	//最大理论声望和威望
#define	HELP_BONUS_LEVEL	75		//多少级以下有老人带新人的奖励
#define	HELP_LEVEL_LIMIT	90		//多少级以上的老人才能带新人
#define PKVALUE_PER_POINT	7200		//每点PK值的等效内部值
#define STD_MOVE_USE_TIME	500		//移动命令里的参照时间 毫秒单位
#define MIN_MOVE_USE_TIME	100		//移动命令里允许的最小时间 毫秒单位
#define MAX_MOVE_USE_TIME	1000		//移动命令里允许的最大时间 毫秒单位
#define FAST_MOVE_SKIP_TIME	150		//可以跳过多少时间
#define SESSION_TIME_OUT	30		//session如果不执行，其超时时间为多少
#define ID_LIANQIFU		685		//炼器符ID
#define MAX_ARCHER_COUNT	5
#define SECT_MASTER_REP_INDEX	8		//师徒系统的师德趋于友好度索引
#define SECT_MASTER_TITLE	11013
#define MAX_DEATH_EXP_PUNISH	250000000	//最大死亡损失经验
#define SPIRIT_POWER_DEC_PROB	0.005f		//每次攻击/被攻击魂力减少的概率
#define MAGIC_DURATION_DEC_PROB	0.005f		//每次攻击/被攻击幻灵石灵气减少的概率

#define TICK_PER_SECOND		20
#define TICK_PER_SECOND_A	TICK_PER_SECOND
#define SECOND_TO_TICK(f) 	((int)((f) * TICK_PER_SECOND_A + 0.5f))
#define TICK_TO_SECOND(t) 	((t) * (1.0f/TICK_PER_SECOND_A)
#define MILLISEC_TO_TICK(f) 	((int)((f) * (TICK_PER_SECOND_A/1000.f) + 0.5f))
#define TICK_MILLISEC		(1000.f/TICK_PER_SECOND_A)
#define TICK_TO_MILLISEC(t) 	(int)((t) * TICK_MILLISEC)

#define MIN_BATTLE_MAPID	101		//最小的战场地图ID
#define MAX_BATTLE_MAPID	199		//最大的战场地图ID

#define LINE_COMMON_DATA_TYPE_BEGIN  1024		//本线内部自己使用的全局数据空间的开始序列号
#define MAP_COMMON_DATA_TYPE_BEGIN  2048		//本地图内部自己使用的全局数据空间的开始序列号

#define MAX_DEITY_LEVEL		81		//封神最大等级
#define MAX_DEITY_CLEVEL	9		//封神每个阶段最大等级
#define MAX_DEITY_EXP		(int64_t)1000000000000ULL	//封神允许的最大经验(1万亿)
#define MAX_DEITY_POWER		10000
#define MAX_GEM_LEVEL		10

#define MAX_TREASURE_REGION	12	
#define MAX_TREASURE_NORMAL_REGION	9	
#define MAX_TREASURE_REGION_LEVEL	5
#define MAX_TREASURE_RESET_CYCLE	86400	

#define MAX_LITTLE_PET_LEVEL	5

#define MAX_TOWER_RAID_LEVEL	100
#define CONSUMPTION_SCALE (100000.0f)  //消费值配置表里的数据最小能支持到0.0001的精度

#define COLLISION_RAID_ID 535 
#define COLLISION_RAID_ID2 543 

#define HIDE_AND_SEEK_RAID_ID 549

#define CAPTURE_RAID_ID 550

#define MAFIA_BATTLE_RAID_ID 691

#define MAX_XUANYUAN_SPIRITS	3
#define MAX_ASTROLOGY_VALUE 100000

#define MAFIA_BATTLE_RAID_VISITOR_COST 500000 //帮战副本观战者收取的手续费，50金
#define SKILL_COOLINGID_BEGIN 1024
#define MAX_BOTTLE_CAPACITY 2000000000

#define BASE_COLLISION_SCORE	1000

#define REPU_XIAN 9
#define REPU_FO 10
#define REPU_MO 11

#define TASK_CHAT_MESSAGE_ID	24		//任务特殊聊天信息的指定ID
#define LOTTERY_TY_CHAT_MESSAGE_ID	25	//汤圆彩票世界喊话消息ID
#define KINGDOM_BUY_ITEM_CHAT_MESSAGE_ID 26	//国王购买积分物品世界喊话ID
#define FUWEN_COMPOSE_MESSAGE_ID 27		//符文碎片合成产生极品符文的时候世界喊话

#define WEB_PACKAGE_BASE_ID	1000000		//Web商城Package起始ID

enum
{
	FBASE_PROP_GRASS = 1,
	FBASE_PROP_MINE,
	FBASE_PROP_MONSTERFOOD,
	FBASE_PROP_MONSTERCORE,
	FBASE_PROP_CASH,
};

#define MAX_FAC_BASE_MSG_LEN	100

#define MAX_MAFIA_NAME_LENGTH 20
#endif

