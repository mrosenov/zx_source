#ifndef __NETGAME_GS_PLAYER_TEMPLATE_H__
#define __NETGAME_GS_PLAYER_TEMPLATE_H__

#include "property.h"
#include "hashmap.h"
#include <common/types.h>
#include <vector.h>
#include <hashtab.h>
#include <map>
#include <vector>
#include "template/exptypes.h"  // Youshuang add
#include <set>
#include <exception>

enum
{
	QUSER_CLASS_NEWBIE,			//新手
	QUSER_CLASS_WARLOCK,			//术士
	QUSER_CLASS_WARRIOR1,			//武士
	QUSER_CLASS_WARRIOR2,			//战士
	QUSER_CLASS_SWORDSMAN,			//剑士
	QUSER_CLASS_PRIEST,			//牧师
	QUSER_CLASS_MAGE,			//法师
	QUSER_CLASS_GENERAL,			//将军
	QUSER_CLASS_KINGHT,			//重骑士
	QUSER_CLASS_ASSASSIN,			//刺客
	QUSER_CLASS_SWORDFIGHTER,		//剑圣
	QUSER_CLASS_SOLON,			//贤者
	QUSER_CLASS_ZEALOT,			//祭司
	QUSER_CLASS_ARCHMAGE,			//秘法师
	QUSER_CLASS_SUMMONER,			//召唤使
	QUSER_CLASS_COUNT			= 128,	//最多不能超过128个职业
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

struct instance_field_info
{
	int battle_id;
	int world_tag;
	int tid;
	int queuing_time;
	int fighting_time;
	int preparing_time;
	int max_player_limit;
	int manual_start_player_num;
	int auto_start_player_num;
	int reborn_limit;
	int min_level_limit;
	int max_level_limit;
	int max_prof_count_limit;
	int faction_limit;
	int required_item;
	int required_money;
};

struct instance_level_info
{
	int score;
	int time_limit;
	int score_per_sec;
	int open_ctrl_id;
	int finish_ctrl_id;
	int death_penlty_score;
	int status;
	int start_timestamp;
	int close_timestamp;
};

struct instance_monster_score
{
	int monster_id;
	int score;
};


struct instance_award_info
{
	int victory_score;
	int victory_task_id;
	int lost_score;
	int lost_task_id;
};

struct treasure_award_item
{
	int item_id;
	int item_count;
};

// 上古世界传送配置项，by sunjunbo 2012.8.20
struct ui_transfer_config_entry
{
	unsigned int        map_id;             // 目标地图号
	float               pos[3];             // 目标坐标
	unsigned int        fee;                // 费用 
};

struct liveness_type_info
{
	int score;
	std::set<int> indexes; 
};

struct liveness_cfg
{
	int						gifts[4];			// 四个礼包 0-25、26-50、51-75、76-100
	liveness_type_info				type_infos[15];	 	// 类型信息
	std::map<int, int> 				index2typeid;		// index  --> 类型id
	std::map<int, int> 				taskindexs;	 	 	// taskid --> index 
	std::map<int, int> 				mapindexs;	  	 	// mapid  --> index
	std::map<int, int> 		specialindexs;	  	// specialid  --> index [战场会有多个模板]
};

struct bath_award
{
	int period;

	struct award
	{
		int item_id;
		int item_count;
		float prob;
	}bath_award_info[10];	
};

typedef q_extend_prop  player_extend_prop;
typedef q_enhanced_param  player_enhanced_param;
typedef q_scale_enhanced_param  player_scale_enhanced_param;
typedef q_item_prop player_item_prop;

class gactive_imp;
class gplayer_imp;

class itemdataman;
// Youshuang add
struct facbase_mall_item_info
{
	int idx;
	int tid;
	int build_id;
	int build_level;
	int needed_faction_coupon;
};

struct facbase_auction_item_info
{
	int tid;
	int needed_faction_coupon;
	int aucitontime;
};

struct facbase_cash_item_info
{
	int idx;
	int needed_faction_cash;
	int cooldown_time;
	int build_id;
	int build_level;
	int award_type;
	int auition_item_id;
	int controller_id;
	int exp_multi;
	int multi_exp_time;
};

enum 
{
	QUALITY_OK = 0,
	QUALITY_WRONG = 1,
	QUALITY_SPECIAL = 2
};

// struct newyear_award_item
// {
	// int score_needed;
	// int fetch_count;
	// int award_item_id;
// };

// struct newyear_award_template
// {
	// int start;
	// int end_sign;
	// int end_fetch_award;
	// int sign_score;
	// int sign_award_item_id;
	// std::vector<newyear_award_item> items;
// };
// end

class player_template
{
public:
	// Youshuang add for fashion color
	struct fashion_colorant_probability
	{
		int id;
		int probability;
		fashion_colorant_probability()
		{
			id = -1;
			probability = 0;
		}
		fashion_colorant_probability( int colorant_id, int colorant_prob ):id( colorant_id ), probability( colorant_prob ){}
		fashion_colorant_probability( const fashion_colorant_probability& src )
		{
			id = src.id;
			probability = src.probability;
		}
	};
	typedef std::map< int, fashion_colorant_probability > COLOR_MAP;
	typedef std::map< int, COLOR_MAP > QUALITY_MAP;
	QUALITY_MAP _quality_table;
	
	int GetID( int color, int quality ) const;
	int GetBestQuality() const;
	int GetBetterQuality( int quality ) const;
	void AddColorProbabilityConfig( const COLORANT_CONFIG& cfg );
	int GetProbability( int color, int quality ) const;
	int GetTotalProbability() const;
	int GetTotalProbabilityByQuality( const COLOR_MAP& colors ) const;
	int GetResultColor( int quality, int orignial_quality, const std::set<int>& colors, int colorant_cnt ) const;
	int GetRandomGoodsID() const;
	int HasQuality( int quality ) const;
	bool HasColor( int color ) const;
	std::vector< int > init_colorant_items;
	void SetInitColor( const COLORANT_DEFAULT_CONFIG& cfg );
	int GetInitColor() const;
	// end
	
	// Youshuang add for facbase
	typedef std::map< int, facbase_mall_item_info > MALL_ITEM_INFO;  // index 2 mall item
	typedef std::map< int, facbase_auction_item_info > AUCTION_ITEM_INFO;  // itemid 2 auction item
	typedef std::map< int, facbase_cash_item_info > CASH_ITEM_INFO;  // index 2 money item
	MALL_ITEM_INFO mall_items;
	AUCTION_ITEM_INFO auction_items;
	CASH_ITEM_INFO cash_items;

	void InsertMallItems( const FACTION_SHOP_CONFIG& cfg );
	void InsertAuctionItems( const FACTION_AUCTION_CONFIG& cfg);
	void InsertCashItems( const FACTION_MONEY_SHOP_CONFIG& cfg);
	
	const facbase_mall_item_info* GetItemInfoByIndex( int idx ) const;
	const facbase_auction_item_info* GetAuctionItemInfoByID( int item_id ) const;
	const facbase_cash_item_info* GetCashItemInfoByIndex( int idx ) const;
	// end
	
	// Youshuang add for newyear award
	// newyear_award_template _newyear_award_template;
	// const newyear_award_template& GetAwardTemplate(){ return _newyear_award_template; }
	// void SetNewYearAward( const SIGN_IN_CONFIG& cfg );
	// end
	struct prop_data
	{
		float hp;
		float mp;
		float dmg;
		float defense;
		float attack;
		float armor;
		float crit_rate;
		float crit_damage;
		float anti_stunt;
		float anti_weak;
		float anti_slow;
		float anti_silence;
		float anti_sleep;
		float anti_twist;
	};

	struct deity_prop_data
	{
		int dp;
		int hp;
		int mp;
		int dmg;
		int defense;
		int deity_power;
		int anti_stunt;		
		int anti_weak;
		int anti_slow;
		int anti_silence;
		int anti_sleep;
		int anti_twist;
		int dpgen[4];
		int talent_point;
		int skill_point;
	};

	//战场死亡掉落物品, 掠夺掉落
	struct deprive_item 
	{
		unsigned int id;		//  掠夺物品的id
		unsigned int max_num;		//	掠夺该物品的数目上限
	};

	//在线倒计时奖励
	struct online_giftbag_config
	{
		int			            id;							//模板ID
		int						award_type;					//奖励的类型：0：活动奖励；1：每日奖励
		int						start_time_per_day;			//每日奖励时的开启时间
		int						end_time_per_day;			//每日奖励时的结束时间
		int						start_time_per_interval;	//活动奖励时的开启时间
		int						end_time_per_interval;		//活动奖励时的结束时间
		int						overdue_time;				//奖励领取的截止时间，超期的模板不读入内存

		int						renascence_count;			//转生次数要求
		int						require_min_level;			//等级下限
		int						require_max_level;			//等级上限
		unsigned int			require_gender;				//性别限制，0-男，1-女，2-男女都可
		int64_t					character_combo_id;			//职业限制，通过掩码实现
		int64_t					character_combo_id2;		//职业限制，扩展需求
		int						require_race;				//种族限制，0-人族，1-兽族， 2-都可以

		struct small_gift_bag_info
		{
			int		gift_bag_id;							//小礼包id
			int		deliver_interval;						//小礼包发放间隔，都是以上一个小礼包发放后开始计算，时间单位：秒
		};

		abase::vector<small_gift_bag_info> small_gift_bags;	//最多MAX_SMALL_ONLINE_GB_COUNT个小礼包
	};
	
	typedef abase::hash_map<int, online_giftbag_config> OnlineGiftbagConfigMap;
	enum { MAX_SMALL_ONLINE_GB_COUNT = 16, };

	struct count_drop_item
	{
		int template_id;
		int item_id;
		int item_num;
		int start_time;
		int time_of_duration;
		int speak_interval;
	};

	struct pk_bet_data
	{
		int						first_id;				// 冠军ID
		int						second_id;				// 亚军ID
		int						third_id;				// 季军ID
		int						guess_start_time;		// 竞猜开始时间
		int						guess_end_time;			// 竞猜截止时间
		int						accept_award_start_time;// 领奖开始时间
		int						accept_award_end_time;	// 领奖截止时间
		int						champion_guess_item;	// 冠军投注道具
		int 					champion_guess_itemNum; 
		int						champion_guess_award;	// 冠军竞猜礼包
		float					award_back_ratio;		// 返奖率
		int						guess_item;				// 竞猜道具
		int						guess_item_num;			// 竞猜道具消耗数量
		int						guess_award_item;		// 三强竞猜礼包
		int						guess_award_item3;		// 三强全中礼包
		int						guess_award_item2;		// 猜中两强礼包
		int						guess_award_item1;		// 猜中一强礼包
	};

private:
	enum { MAX_LEVEL_DIFF = 200 ,
		BASE_LEVEL_DIFF = -100,
		PRODUCE_LEVEL_MAX = 10,
	};
	struct q_class_data
	{

		int class_id;
		prop_data base;
		prop_data lvlup;
		int base_level;
		int faction;
		int enemy_faction;
		float walk_speed;
		float run_speed;
		int hp_gen[4];		//0 非战斗状态中HP恢复速度         对于npc 非战斗慢速回血
					//1 非战斗状态中打坐HP恢复速度     对于npc 非战斗快速回血 
					//2 战斗状态中HP恢复速度 
					//3 战斗状态中打坐HP恢复速度
		int mp_gen[4];  	//0 非战斗状态中MP恢复速度 
					//1 非战斗状态中打坐MP恢复速度 
					//2 战斗状态中MP恢复速度 
					//3 战斗状态中打坐MP恢复速度      
		int64_t character_combo_id;	//可转值的职业组合
		int64_t character_combo_id2;
		int xp_skill;
	};
	
	struct level_adjust
	{
		float exp_adjust;
		float money_adjust;	//amount adjust
		float item_adjust;	//prob adjust
		float attack_adjust;	//击时的等级惩罚
	};
	struct team_adjust
	{	
		float exp_adjust;
	};
	struct team_cls_adjust
	{
		float exp_adjust;
	};
	
	struct sns_message
	{
		int fee_press_msg;
		int id_press_msg_item;
		int id2_press_msg_item;
		int id_press_msg_task;
		int id_support_msg_item;
		int id_support_msg_task;
		int id_against_msg_item;
		int id_against_msg_task;
		int fee_apply_msg;
		int id_apply_msg_item;
		int id_apply_msg_task;
		int fee_response_msg;
		int id_response_msg_item;
		int id_accept_apply_task1;
		int id_accept_apply_task2;
		int id_accept_apply_task3;	
	};

	struct battle_rank
	{
		int score;
		int task_id;	
	};
	
	//战场死亡掉落物品, 系统掉落
	struct death_drop_item	
	{
		unsigned int	id;			// 掉落物品id
		float			probability;	// 掉落概率
	};
	

	//杀死怪物积分
	struct monster_score
	{
		unsigned int monster_id;
		unsigned int monster_score;
	};	

	
	struct original_shape 
	{
		int monster_id;
		struct 
		{
			int original_type;
			int original_id;
			float probability;
		}original_info[3];
	};

	//组合技编辑配置
	struct combine_edit_config
	{
		int skill_id;
		int cool_index;
		int cool_time;
	};

	struct equipment_upgrade_data
	{
		int equip_id;
		int stone_id;
		int stone_num;
		int output_equip_id;
	};

	struct deity_levelup_exp
	{
		int64_t deity_exp;	//升级所需要的经验
		int64_t failure_exp;	//升级失败扣除的经验
		float levelup_prob;	//升级成功的概率
	};

	struct treasure_region
	{
		int region_id;
		char region_index;

		float init_level_prob[6];		//初始等级分布的概率
		float levelup_prob[5];			//等级升级的概率
		int unlock_region_item;			//解锁区域需要的物品id(仅针对隐藏区域)	

		struct award
		{
			int item_id;
			int item_count;
			float prob;
			char event_id;
		} treasure_award[6][10];
	};

	//元魂符文之语
	struct rune_combo_data
	{
		unsigned int id;
		abase::vector<int> stone_id;
		abase::vector<int> addon_id;
	};

	struct rune_data
	{
		unsigned int id_rune_identify[5];
		unsigned int id_rune_refine[5];
		unsigned int id_rune_reset[5];
		unsigned int id_rune_change_slot[5];

		int exp_identify;
		int exp_return;
		int exp_refresh;
		int exp_decompose[100];
		int lvlup_exp[100];

		struct rune_equip_req
		{
			int lv_req;
			int reborn_req;
		}equip_req[100];

		struct fixed_prop_info
		{
			unsigned int level;
			unsigned int damage;
			unsigned int hp;
			unsigned int mp;
			unsigned int dp;
		}fixed_prop[60];
			
	};

	//诛小仙
	struct little_pet
	{
		struct feed_pet_info
		{
			int item_id;
			int exp;
		} feed_info[2];

		struct pet_level_info
		{
			int lvlup_exp;
			int award_item;
		}level_info[5];
	};

	struct collision_raid_award
	{
		int daily_award_item;

		struct collision_award_list
		{
			unsigned int item_id;
			unsigned int item_num;
			unsigned int need_score_a;
			unsigned int need_score_b;
			unsigned int win_num;
		}award_list[20];	
	};

	struct kingdom_data
	{
		int kingdom_continue_item;	//国王连任奖励
		int kingdom_continue_item_num;	//国王连任奖励数目

		struct 
		{
			int monster_id;
			int hp_delta;
		}monster[2];

		struct
		{
			int id;
			int num;
			int score;
		}kingdom_shop[20];

		struct
		{
			int id;
			int num;
			int score;
		}normal_shop[20];
	};

	struct fuwen_exp_config
	{
		int max_level;
		int lvlup_exp[4][10];
	};

	struct cs_6v6_personal_level_award_config
	{
		int level;
		int min_point;
		int max_point;
		int max_count;

		struct award_item_info
		{
			int item_id;
			int item_num;
			int expire_time;
		}award_item[5];
	};

	struct cs_6v6_team_award_config
	{
		int rank_start;
		int rank_end;

		struct award_item_info
		{
			int item_id;
			int item_num;
			int expire_time;
		}award_item[6];
	};

	struct cs_6v6_exchange_award_config
	{
		int rank_start;
		int rank_end;
		int award_id;
		int award_count;
		int max_count;
		int expire_time;
	};

	struct cs_6v6_item_exchange_config
	{
		int item_id;
		int item_num;
		int require_item_id;
		int require_item_count;
		int expire_time;
		int exchange_count;
	};
	
	q_class_data _class_list[QUSER_CLASS_COUNT];
	deity_prop_data _deity_prop_data[100];
	
	level_adjust _level_adjust_table[2][MAX_LEVEL_DIFF+1]; //修正表2用于对未飞升玩家的特殊经验惩罚
	team_adjust _team_adjust[TEAM_MEMBER_CAPACITY+1];
	team_cls_adjust _team_cls_adjust[TEAM_MAX_RACE_COUNT+1];
	abase::vector<abase::pair<A3DVECTOR,rect> > _region_list;
	abase::vector<int64_t> _exp_list[MAX_REBORN_COUNT + 1];
	abase::vector<int> _pet_exp_list;
	abase::vector<int> _talisman_exp_list;
	abase::vector<int> _talent_point_list[MAX_REBORN_COUNT+1];
	abase::vector<int> _skill_point_list[MAX_REBORN_COUNT+1];
	
	deity_levelup_exp _deity_exp_list[100];
		
	int  _produce_levelup_exp[PRODUCE_LEVEL_MAX];
	int  _produce_exp[PRODUCE_LEVEL_MAX][PRODUCE_LEVEL_MAX];
	bool _debug_mode;
	int  _max_player_level;
	int  _max_player_reborn_level;
	float _exp_bonus;
	float _money_bonus;
	sns_message _sns_config[5];

	//楚汉战场相关的配置	
	std::map<int, std::vector<battle_rank> > _rank_score; //积分与军衔的对应表
	std::map<int, int > _map_controller; //战场控制器ID
	typedef std::map<int, std::vector<death_drop_item>  > RANK_DROP_ITEM; 
	std::map<int, RANK_DROP_ITEM> _death_drop_item; //战场死亡系统掉落表
	std::map<int, std::vector<deprive_item> > _deprive_item; //战场死亡掠夺配置表 
	std::map<int, std::vector<unsigned int> > _kill_score; //杀死某军衔玩家获得的积分
	std::map<int, std::vector<unsigned int> > _conKill_score; //连续杀死玩家获得的额外积分
	std::map<int, std::vector<monster_score> > _monster_score; //杀死怪物获得的积分 
	std::map<int, unsigned int> _battle_win_score;	


	//剧情战场相关的配置
	std::map<int, instance_field_info> _instance_field_info;
	std::map<int, std::vector<instance_level_info> > _instance_level_info; //怪物关卡信息
	std::map<int, std::vector<instance_monster_score> > _instance_monster_score; //杀死怪物获得的积分 
	std::map<int, std::vector<instance_award_info> > _instance_award_info; //杀死怪物获得的积分 
	std::map<int, int> _instance_win_ctrlID; //胜利控制器ID

	//照妖镜对应的原型模板
	std::map<int, std::vector<original_shape> >_original_shape_info;

	//竞技场掉落物品ID
	std::map<int, std::set<int> > _arena_drop_item; 
	combine_edit_config _combine_edit_config[30];

	//装备升级对应模板
	abase::vector<equipment_upgrade_data> _equipment_upgrade_data;		//140装备

	//挖宝区域数据
	abase::vector<treasure_region> _treasure_region;

	//诛小仙数据
	little_pet _little_pet;

	//碰撞战场奖励数据
	collision_raid_award _collision_raid_award;

	//元魂相关配置
	rune_data _rune_data;
	abase::vector<rune_combo_data> _rune_combo_data;

	//在线倒计时奖励
	OnlineGiftbagConfigMap  _online_giftbag_config;

	//上古世界传送 by sunjunbo 2012-08-20
	abase::vector<ui_transfer_config_entry> _ui_transfer_config;

	//活跃度 by sunjunbo 2012-08-20
	liveness_cfg _liveness_cfg;
	
	int prop_add_limit[20];
	
	typedef std::map<int, std::vector<count_drop_item> > COUNT_DROP_MAP;	
	COUNT_DROP_MAP _count_drops;

	pk_bet_data _pk_bet_data;
	kingdom_data _kingdom_data;
	fuwen_exp_config _fuwen_exp_config;

	bath_award _bath_award;

	cs_6v6_personal_level_award_config _cs_6v6_personal_level_award_list[15];
	cs_6v6_team_award_config _cs_6v6_team_award_list[15];
	cs_6v6_exchange_award_config _cs_6v6_exchange_award_list[15]; 
	cs_6v6_item_exchange_config _cs_6v6_item_exchange_list[15];

	player_template();
	
	static player_template _instance;

	bool __LoadDataFromDataMan(itemdataman & dataman);
	bool __Load(const char * filename,itemdataman* pDataMan);

	bool __LevelUp(int cls, int oldlvl,player_extend_prop & data, gactive_imp * pImp) const;
	bool __DeityLevelUp(int cls, int dt_level, gactive_imp * pImp) const;
	bool __IncPropAdd(int cls, gactive_imp * pImp) const;
	bool __BuildPlayerData(int cls, int cur_lvl, int cur_dt_lvl, player_extend_prop & prop, gactive_imp * pImp) const;
	void __ActivateDeityProp(int dt_level, player_extend_prop & prop) const;
	void __ActivatePropAdd(player_extend_prop & prop, gplayer_imp* pImp) const;
	int  __Rollback(int cls, player_extend_prop & data) const;
	int  __Rollback(int cls, player_extend_prop & data,int str, int agi ,int vit, int eng) const;
	int64_t  __GetLvlupExp(size_t reborn_count, int cur_lvl) const;
	int64_t  __GetDeityLvlupExp(int cur_lvl) const;
	int64_t	 __GetDeityLvlupPunishExp(int cur_lvl) const;
	float __GetDeityLvlupProb(int cur_lvl) const;
	int  __GetPetLvlupExp(int cur_lvl) const;
	void __InitPlayerData(int cls,gactive_imp * pImp);
	bool __GetTownPosition(const A3DVECTOR & source, A3DVECTOR & target);
	float __GetResurrectExpReduce(size_t sec_level);
	bool __CheckData(int cls, int level, const player_extend_prop &data);
	bool __ProduceItem(int & produce_level, int & produce_exp, size_t recipe_level);
	bool __IncProduceExp(int & produce_level, int & produce_exp,int exp);


	int __GetTalismanLvlupExp(size_t cur_lvl)
	{
		if(cur_lvl >= MAX_TALISMAN_LEVEL) return 0x7FFFFFFF;
		return _talisman_exp_list[cur_lvl];
	}
	int __GetSNSRequiredMoney(char m_type, char o_type);
	int __GetSNSRequiredItemID(char m_type, char o_type);
	int __GetSNSRequiredItemID2(char m_type, char o_type);
	int __GetSNSRequiredTaskID(char m_type, char o_type, int charm);
	int __GetRankByScore(int map_id, int score);
	int __GetTaskIDByRank(int map_id, int rank);
	int __GetDeathDropItemID(int map_id, int rank);

	bool  __GetDeprivedItemList(int map_id, std::vector<deprive_item> & deprive_item_list )
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
	
		std::map<int, std::vector<deprive_item> >::iterator iter = _deprive_item.find(map_id);	
		if(iter == _deprive_item.end()) return false;
		deprive_item_list = iter->second;
		return true;	
	}
	
	int __GetKillScoreByRank(int map_id, int rank)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		if(rank < 0 || rank > 10) return 0;
		std::map<int, std::vector<unsigned int> >::iterator iter = _kill_score.find(map_id);
		if(iter == _kill_score.end()) return 0;
		return (iter->second)[rank-1];	
	}	

	int __GetConKillScore(int map_id, int count)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		if(count < 3) return 0; //连杀次数小于3没有额外奖励
		if(count > 10) count = 10;
		
		std::map<int, std::vector<unsigned int> >::iterator iter = _conKill_score.find(map_id);
		if(iter == _kill_score.end()) return 0;
		
		return (iter->second)[count -3]; //这个表是从3开始算的	
	}

	int __GetKillMonsterScore(int map_id, int monster_id)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		std::map<int, std::vector<monster_score> >::iterator iter = _monster_score.find(map_id);
		if(iter == _monster_score.end()) return 0;
		
		std::vector<monster_score>::iterator iter2 = (iter->second).begin();
		for(; iter2 != (iter->second).end(); ++iter2)
		{
			if(iter2->monster_id == (size_t) monster_id)
			{
				return iter2->monster_score;
			}
		}
		return 0;
	}	

	int __GetBattleWinScore(int map_id)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		std::map<int, unsigned int>::iterator iter = _battle_win_score.find(map_id);

		if(iter == _battle_win_score.end()) return 0;

		return iter->second;
	}

	int __GetMapControllerID(int map_id)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		std::map<int, int>::iterator iter = _map_controller.find(map_id);
		
		if(iter == _map_controller.end()) return 0;
		
		return iter->second;
	}
	
	int __GetInstanceFieldInfo(int map_id, instance_field_info & info)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
		std::map<int, instance_field_info>::iterator iter = _instance_field_info.find(map_id);
		
		if(iter == _instance_field_info.end()) return -1;
		info = iter->second;
		return 0;
	}
	
	int __GetInstanceKillMonsterScore(int map_id, int monster_id)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		std::map<int, std::vector<instance_monster_score> >::iterator iter = _instance_monster_score.find(map_id);
		if(iter == _instance_monster_score.end()) return 0;
		
		std::vector<instance_monster_score>::iterator iter2 = (iter->second).begin();
		for(; iter2 != (iter->second).end(); ++iter2)
		{
			if(iter2->monster_id == monster_id)
			{
				return iter2->score;
			}
		}
		return 0;
	}	
	
	int __GetInstanceWinCtrlID(int map_id)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return 0;
		std::map<int, int>::iterator iter = _instance_win_ctrlID.find(map_id);
		
		if(iter == _map_controller.end()) return 0;
		
		return iter->second;
	}
	
	int __GetInstanceLevelInfo(int map_id, std::vector<instance_level_info>& info)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
		std::map<int, std::vector<instance_level_info> >::iterator iter = _instance_level_info.find(map_id);
		
		if(iter == _instance_level_info.end()) return -1;
		info = iter->second;
		return 0;
	}
	int __GetInstanceAwardTaskID(int map_id, int total_score, bool b_win)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
		if(total_score <= 0) return -1;
		
		std::map<int, std::vector<instance_award_info> >::iterator iter = _instance_award_info.find(map_id); 
		if(iter == _instance_award_info.end()) return -1;
		std::vector<instance_award_info> _award_info = iter->second;

		for(size_t i = _award_info.size() -1; i >= 0; --i)
		{
			if(b_win)
			{
				if(total_score > _award_info[i].victory_score)
				{
					return _award_info[i].victory_task_id;
				}
			}
			else
			{
				if(total_score > _award_info[i].lost_score)
				{
					return _award_info[i].lost_task_id;
				}
			}
		}
		return 0;
	}

	bool __CheckOriginalInfo(int table_id, int monster_id);
	int __GetOriginalID(int table_id, int monster_id, int & target_type);
	bool __GetArenaDropList(int map_id, std::set<int>& drop_list)
	{
		if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return false;
	
		std::map<int, std::set<int> >::iterator iter = _arena_drop_item.find(map_id);	
		if(iter == _arena_drop_item.end()) return false;
		drop_list = iter->second;
		return true;	
	}
	
	bool __GetCombineEditCD(int skill_id, int & cool_index, int & cool_time)
	{
		for(size_t i = 0; i < 30; ++i)
		{
			if(_combine_edit_config[i].skill_id == skill_id)
			{
				cool_index = _combine_edit_config[i].cool_index;
				cool_time = _combine_edit_config[i].cool_time;
				return true;
			}
		}
		return false;
	}	


	bool __CanEquipUpgrade(int equip_id)
	{
		for(size_t i = 0; i < _equipment_upgrade_data.size(); ++i)
		{
			if(_equipment_upgrade_data[i].equip_id == equip_id) return true;
		}
		return false;
	}

	bool __IsUpgradeEquipment(int equip_id)
	{
		for(size_t i = 0; i < _equipment_upgrade_data.size(); ++i)
		{
			if(_equipment_upgrade_data[i].output_equip_id == equip_id) return true;
		}
		return false;
	}

	void __GetEquipUpgradeStoneInfo(int equip_id, int & stone_id, int & stone_num)
	{
		for(size_t i = 0; i < _equipment_upgrade_data.size(); ++i)
		{
			if(_equipment_upgrade_data[i].equip_id == equip_id) 
			{
				stone_id = _equipment_upgrade_data[i].stone_id;
				stone_num = _equipment_upgrade_data[i].stone_num;
				return;
			}
		}
		return;
	}

	int __GetEquipUpgradeOutputID(int equip_id)
	{
		int output_id = 0;
		for(size_t i = 0; i < _equipment_upgrade_data.size(); ++i)
		{
			if(_equipment_upgrade_data[i].equip_id == equip_id) 
			{
				output_id = _equipment_upgrade_data[i].output_equip_id;
			}
		}
		return output_id;
	}
	

	const OnlineGiftbagConfigMap & __GetOnlineGiftbagConfigMap() const
	{
		return _online_giftbag_config;
	}

	bool TrySetOnlineGiftBagInfo(online_giftbag_config & data);

	int __GetTreasureRegionLevel(int id)
	{
		int level = 0;
		for(size_t i = 0; i < _treasure_region.size(); ++i)
		{
			if(_treasure_region[i].region_id == id)
			{
				level = abase::RandSelect( _treasure_region[i].init_level_prob, 6);
				return level;
			}
		}
		return level;
	}
	
	bool __UpgradeTreasureRegion(int region_id, int region_index, int cur_lvl, int & new_lvl)
	{
		bool success = false;
		for(size_t i = 0; i < _treasure_region.size(); ++i)
		{
			if(_treasure_region[i].region_id == region_id && _treasure_region[i].region_index == region_index)
			{
				while(new_lvl < MAX_TREASURE_REGION_LEVEL && abase::Rand(0.f, 1.f) < _treasure_region[i].levelup_prob[new_lvl])
				{
					new_lvl ++;
					success = true;
					continue;
				}
				return success;
			}
		}
		return success;
	}
	
	int __GetUnlockTreasureRegionID(int region_id, int region_index)
	{
		for(size_t i = 0; i < _treasure_region.size(); ++i)
		{
			if(_treasure_region[i].region_id == region_id && _treasure_region[i].region_index == region_index)
			{
				return _treasure_region[i].unlock_region_item;
			}
		}
		return 0;
	}
	
	void __GetTreasureAward(int region_id, int region_index, int region_level, abase::vector<treasure_award_item> & award_list, char & event_id)
	{
		treasure_award_item award_item;
		for(size_t i = 0; i < _treasure_region.size(); ++i)
		{
			if(_treasure_region[i].region_id == region_id && _treasure_region[i].region_index == region_index)
			{
				for(size_t j = 0; j < 10; ++j)
				{
					if(_treasure_region[i].treasure_award[region_level][j].item_id > 0 &&
					  _treasure_region[i].treasure_award[region_level][j].item_count > 0 &&
					  abase::Rand(0.0f, 1.0f) < _treasure_region[i].treasure_award[region_level][j].prob)
					{
						award_item.item_id = _treasure_region[i].treasure_award[region_level][j].item_id;
						award_item.item_count = _treasure_region[i].treasure_award[region_level][j].item_count;
						award_list.push_back(award_item);

						if(_treasure_region[i].treasure_award[region_level][j].event_id > event_id)
						{
							event_id = _treasure_region[i].treasure_award[region_level][j].event_id;
						}
					}
				}
				return;
			}
		}
	}

	int __GetTreasureRegionID(int region_index)
	{
		for(size_t i = 0; i < _treasure_region.size(); ++i)
		{
			if(_treasure_region[i].region_index == region_index)
			{
				return _treasure_region[i].region_id;
			}
		}
		return 0;
	}
	
	int __GetLittlePetFeedExp(int feed_id)
	{
		for(size_t i = 0; i < 2; ++i)
		{
			if(feed_id == _little_pet.feed_info[i].item_id)
			{
				return _little_pet.feed_info[i].exp;
			}
		}
		return 0;
	}
	
	int __GetLittlePetLevelupExp(int cur_lvl)
	{
		if(cur_lvl <= 0 || cur_lvl >= 5) return 0;

		return _little_pet.level_info[cur_lvl].lvlup_exp;
	}

	int __GetLittlePetAwardItem(int cur_lvl)
	{
		if(cur_lvl <= 0 || cur_lvl > 5) return 0;

		return _little_pet.level_info[cur_lvl-1].award_item;
	}

	int __GetRuneRequireLevel(int rune_lvl)
	{
		if(rune_lvl < 2 || rune_lvl > 100) return 0;
	       return _rune_data.equip_req[rune_lvl-2].lv_req;
	
	}
	
	int __GetRuneRequireReborn(int rune_lvl)
	{
		if(rune_lvl < 2 || rune_lvl > 100) return 0;
	       return _rune_data.equip_req[rune_lvl-2].reborn_req;
	
	}
	
	int __GetRuneIdentifyExp()
	{
		return _rune_data.exp_identify;
	}
	
	int __GetRuneResetExp()
	{
		return _rune_data.exp_return;
	}
	
	int __GetRuneRefineExp()
	{
		return _rune_data.exp_refresh;
	}
	
	int __GetRuneLevelExp(int cur_lvl)
	{
		if(cur_lvl <= 1 || cur_lvl > 20) return 0;

		int exp = 0;
		for(int i = 0; i < cur_lvl -1; ++i)
		{
			exp += _rune_data.lvlup_exp[i];
		}
		return exp;
	}
	
	int __GetRuneLevelUpExp(int cur_lvl)
	{
		if(cur_lvl <= 0 || cur_lvl >= 20) return 0;

		int exp = _rune_data.lvlup_exp[cur_lvl-1];
		return exp;
	}
	
	unsigned int __GetRuneComboID(abase::vector<int> & stone_list)
	{
		for(size_t i = 0; i < _rune_combo_data.size(); ++i)
		{
			abase::vector<int> & stone_combo = _rune_combo_data[i].stone_id;
			if(stone_list.size() == stone_combo.size())
			{
				size_t match = 0;
				for(size_t j = 0; j < stone_list.size(); ++j)
				{
					int stone_id = stone_list[j];
					for(size_t k = 0; k < stone_combo.size(); ++k)
					{
						if(stone_id == stone_combo[k])
						{
							match++;
							continue;
						}
						
					}
				}

				if(match == stone_list.size())
				{
					return _rune_combo_data[i].id;
				}
			}
		}
		return 0;
	}
	
	void __GetRuneComboAddon(unsigned int combo_id, abase::vector<int> & addon_list)
	{
		for(size_t i = 0; i < _rune_combo_data.size(); ++i)
		{
			if(combo_id == _rune_combo_data[i].id)
			{
				for(size_t j = 0; j < _rune_combo_data[i].addon_id.size(); ++j)
				{
					if(_rune_combo_data[i].addon_id[j] > 0)
					{
						addon_list.push_back(_rune_combo_data[i].addon_id[j]);
					}
				}
			}
		}
	}
	
	bool __IsRuneIdentifyItemID(unsigned int id)
	{
		if(id == 0) return false;

		for(size_t i = 0; i < 5; ++i)
		{
			if(_rune_data.id_rune_identify[i] == id) return true;	
		}
		return false; 
	}

	bool __IsRuneRefineItemID(unsigned int id)
	{
		if(id == 0) return false;

		for(size_t i = 0; i < 5; ++i)
		{
			if(_rune_data.id_rune_refine[i] == id) return true;	
		}
		return false; 
	}
	
	bool __IsRuneResetItemID(unsigned int id)
	{
		if(id == 0) return false;

		for(size_t i = 0; i < 5; ++i)
		{
			if(_rune_data.id_rune_reset[i] == id) return true;	
		}
		return false; 
	}
	
	bool __IsRuneChangeSlotItemID(unsigned int id)
	{
		if(id == 0) return false;

		for(size_t i = 0; i < 5; ++i)
		{
			if(_rune_data.id_rune_change_slot[i] == id) return true;	
		}
		return false; 
	}

	unsigned int __GetRuneActiveLevel(int cur_level)
	{
		for(size_t i = 0; i < 60; ++i)
		{
			if(cur_level < (int)_rune_data.fixed_prop[i].level)
			{
				if(i > 0)
				{
					return  _rune_data.fixed_prop[i-1].level;
				}
				else
				{
					return 0;
				}
			}
		}
		return 0;
	}
	
	void __GetRuneActiveProp(int active_level, int & damage, int & hp, int & mp, int & dp)
	{
		if(active_level == 0) return;

		for(size_t i = 0; i < 60; ++i)
		{
			if(active_level == (int)_rune_data.fixed_prop[i].level)
			{
				damage = _rune_data.fixed_prop[i].damage;
				hp = _rune_data.fixed_prop[i].hp;
				mp = _rune_data.fixed_prop[i].mp;
				dp = _rune_data.fixed_prop[i].dp;
			}
		}
	}

	int __GetRuneDecomposeExp(int avg_grade)
	{
		if(avg_grade <= 0 || avg_grade > 100) return 0;
		return _rune_data.exp_decompose[avg_grade -1];
	}

	bool __GetCountDrop(int itemId, count_drop_item& item);
	void __GetAllCountDrops(std::vector<count_drop_item>& items);

	const pk_bet_data& __GetPKBetData() const { return _pk_bet_data; }
	
	int __GetCollisionDailyAwardItem() { return _collision_raid_award.daily_award_item;}
	
	bool __GetCollisionAwardItem(int index, unsigned int & item_id, unsigned int & item_count, unsigned int & need_score_a, unsigned int & need_score_b, unsigned int & win_num)
	{
		if(index < 0 || index >= 20) return false;

		item_id = _collision_raid_award.award_list[index].item_id;
		item_count = _collision_raid_award.award_list[index].item_num;
		need_score_a = _collision_raid_award.award_list[index].need_score_a;
		need_score_b = _collision_raid_award.award_list[index].need_score_b;
		win_num = _collision_raid_award.award_list[index].win_num;
		return true;
	}

	int __GetPropAddLimit(int idx);

	void __GetKingdomMonsterID(int & attacker_npc_id, int & defender_npc_id)
	{
		attacker_npc_id = _kingdom_data.monster[0].monster_id; 
		defender_npc_id = _kingdom_data.monster[1].monster_id;
	}

	int __GetKingdomChangeHP(int type)
	{
		ASSERT(type == 1 || type == 2);
		return  _kingdom_data.monster[type-1].hp_delta;
	}

	void __GetKingdomShop(int index, int & item_id, int & item_num, int & need_score)
	{
		if(index < 0 || index >= 20) return;

		item_id = _kingdom_data.kingdom_shop[index].id;
		item_num = _kingdom_data.kingdom_shop[index].num;
		need_score = _kingdom_data.kingdom_shop[index].score;	
	}

	int __GetXPSkill(int cls)
	{
		if(((size_t)cls) >= QUSER_CLASS_COUNT) 
		{
			ASSERT(false);
			return -1; 
		}
		return _class_list[cls].xp_skill;
	}
	
	int __GetBathAwardPeriod()
	{
		return _bath_award.period;
	}

	void __GetBathAwardItem(int & item_id, int & item_count)
	{
		if(_bath_award.bath_award_info[0].item_id <= 0) return;
		int index = abase::RandSelect( &(_bath_award.bath_award_info[0].prob), sizeof(_bath_award.bath_award_info[0]), 10);
		item_id = _bath_award.bath_award_info[index].item_id;
		item_count = _bath_award.bath_award_info[index].item_count;
	}
	
	int __GetFuwenLevel(int exp, int quality)
	{
		int max_level = -1;
		for(size_t i = 0; i < 10; ++i) 
		{
			if(exp >= _fuwen_exp_config.lvlup_exp[quality][i])
			{
				max_level = i;
				continue;
			}
		}
		return max_level +1;
	}

	int __GetFuwenMaxLevel()
	{
		return _fuwen_exp_config.max_level;
	}

	int __GetFuwenMaxExp(int quality)
	{
		return _fuwen_exp_config.lvlup_exp[quality][_fuwen_exp_config.max_level-1];
	}

	bool __GetCS6V6PersonalLevelAwardItem(int level, void * award_item)
	{
		for(size_t i = 0; i < 15; ++i)
		{
			if(_cs_6v6_personal_level_award_list[i].level == level)
			{
				memcpy(award_item, _cs_6v6_personal_level_award_list[i].award_item, sizeof(_cs_6v6_personal_level_award_list[i].award_item));
				return true;
			}
		}	
		return false;
	}

	bool __GetCS6V6PersonalLevelAwardPoint(int level, int & max_point, int & min_point)
	{
		for(size_t i = 0; i < 15; ++i)
		{
			if(_cs_6v6_personal_level_award_list[i].level == level)
			{
				max_point = _cs_6v6_personal_level_award_list[i].max_point;
				min_point = _cs_6v6_personal_level_award_list[i].min_point;
				return true;
			}
		}	
		return false;
	}

	bool __GetCS6V6TeamAwardItem(int team_rank, void * award_item)
	{
		for(size_t i = 0; i < 15; ++i)
		{
			if(_cs_6v6_team_award_list[i].rank_start <= team_rank && _cs_6v6_team_award_list[i].rank_end >= team_rank)
			{
				memcpy(award_item, _cs_6v6_team_award_list[i].award_item, sizeof(_cs_6v6_team_award_list[i].award_item));
				return true;
			}
		}	
		return false;
	}

	bool __GetCS6V6ExchangeAwardItem(int team_rank, int & item_id, int & item_count, int & expire_time)
	{
		for(size_t i = 0; i < 15; ++i)
		{
			if(_cs_6v6_exchange_award_list[i].rank_start <= team_rank && _cs_6v6_exchange_award_list[i].rank_end >= team_rank)
			{
				item_id = _cs_6v6_exchange_award_list[i].award_id;
				item_count =  _cs_6v6_exchange_award_list[i].award_count;
				expire_time =  _cs_6v6_exchange_award_list[i].expire_time;
				return true;
			}
		}	
		return false;
	}

	bool __GetCS6V6ExchangeItemInfo(int index, int & exchange_item_id, int & exchange_item_num, int & require_item_id, int & require_item_count, int & expire_time, int & exchange_max_count)
	{
		if(index < 0 || index >= 15) return false;

		exchange_item_id = _cs_6v6_item_exchange_list[index].item_id; 
		exchange_item_num = _cs_6v6_item_exchange_list[index].item_num; 
		require_item_id = _cs_6v6_item_exchange_list[index].require_item_id; 
		require_item_count = _cs_6v6_item_exchange_list[index].require_item_count; 
		expire_time = _cs_6v6_item_exchange_list[index].expire_time; 
		exchange_max_count = _cs_6v6_item_exchange_list[index].exchange_count;
		return true;
	}
	
public:
	static player_template & GetInstance()
	{
		return _instance;
	}

	static inline bool Load(const char * filename,itemdataman* pDataMan)
	{
		return GetInstance().__Load(filename,pDataMan);
	}

	static inline bool GetDebugMode()
	{
		return GetInstance()._debug_mode;
	}

	static inline void InitPlayerData(int cls,gactive_imp * pImp)	//初始化对象的一些基本数据
	{
		return GetInstance().__InitPlayerData(cls,pImp);
	}

	static inline bool LevelUp(int cls, int oldlvl,player_extend_prop & data, gactive_imp * pImp)
	{
		return GetInstance().__LevelUp(cls,oldlvl,data, pImp);
	}

	static inline bool DeityLevelUp(int cls, int dt_level, gactive_imp * pImp)
	{
		return GetInstance().__DeityLevelUp(cls, dt_level, pImp);
	}

	static inline bool IncPropAdd(int cls, gactive_imp * pImp)
	{
		return GetInstance().__IncPropAdd(cls, pImp);
	}

	static void BuildExProp(const prop_data &base, const prop_data &lvlup, int lvl, int base_level,player_extend_prop &data);

	static inline bool CheckData(int cls, int level, const player_extend_prop &data)
	{
		return GetInstance().__CheckData(cls,level,data);
	}

	static inline int  Rollback(int cls, player_extend_prop & data)
	{
		return GetInstance().__Rollback(cls,data);
	}

	static inline int  Rollback(int cls, player_extend_prop & data,int str, int agi ,int vit, int eng)
	{
		return GetInstance().__Rollback(cls,data,str,agi,vit,eng);
	}

	static inline int64_t  GetLvlupExp(size_t reborn_count, int cur_lvl)
	{
		return GetInstance().__GetLvlupExp(reborn_count, cur_lvl);
	}
	
	static inline int64_t  GetDeityLvlupExp(int cur_lvl)
	{
		return GetInstance().__GetDeityLvlupExp(cur_lvl);
	}
	
	static inline int64_t  GetDeityLvlupPunishExp(int cur_lvl)
	{
		return GetInstance().__GetDeityLvlupPunishExp(cur_lvl);
	}
	
	static inline float GetDeityLvlupProb(int cur_lvl)
	{
		return GetInstance().__GetDeityLvlupProb(cur_lvl);
	}

	static inline int  GetPetLvlupExp(int cur_lvl)
	{
		return GetInstance().__GetPetLvlupExp(cur_lvl);
	}

	static inline int GetTalismanLvlupExp(int cur_lvl)
	{
		return GetInstance().__GetTalismanLvlupExp(cur_lvl);
	}

	static inline bool BuildPlayerData(int cls, int cur_lvl, int cur_dt_lvl, player_extend_prop & prop, gactive_imp *pImp)
	{
		return GetInstance().__BuildPlayerData(cls,cur_lvl,cur_dt_lvl,prop,pImp);
	}

	static inline int GetMaxLevelLimit()
	{
		return MAX_PLAYER_LEVEL;
	}
	
	static inline int GetMaxLevel(int reborn_cnt)
	{
		if(reborn_cnt == 0)
		{
			return GetInstance()._max_player_level;
		}
		else 
		{
			return GetInstance()._max_player_reborn_level;
		}
	}

	static inline int GetMaxDeityLevel()
	{
		return MAX_DEITY_LEVEL; 
	}

	static inline int GetStatusPointPerLevel(int cur_level)
	{
		return 1;
	}

	static inline int GetMaxProduceLevel()
	{
		return 10;
	}

	static inline bool ProduceItem(int & produce_level, int & produce_exp, int recipe_level)
	{
		return GetInstance().__ProduceItem(produce_level, produce_exp,recipe_level);
	}

	static inline bool IncProduceExp(int & produce_level, int & produce_exp, int exp)
	{
		return GetInstance().__IncProduceExp(produce_level, produce_exp,exp);
	}

	static inline int GetTalentPoint(size_t reborn_count, size_t level, size_t dt_level)
	{
		player_template & pt = GetInstance();
		if(reborn_count >= MAX_REBORN_COUNT || level >= pt._talent_point_list[0].size()) return 0;
		if(dt_level < 0 || dt_level > MAX_DEITY_LEVEL) return 0; 

		int deity_talent_point = 0;
		if(dt_level >0) deity_talent_point = pt._deity_prop_data[dt_level-1].talent_point; 

		return pt._talent_point_list[reborn_count][level] + deity_talent_point;
	}

	static inline int GetSkillPoint(size_t reborn_count, size_t level, size_t dt_level)
	{
		player_template & pt = GetInstance();
		if(reborn_count >= MAX_REBORN_COUNT || level >= pt._skill_point_list[0].size()) return 0;
		if(dt_level < 0 || dt_level > MAX_DEITY_LEVEL) return 0; 
		
		int deity_skill_point = 0;
		if(dt_level >0) deity_skill_point = pt._deity_prop_data[dt_level-1].skill_point; 

		return pt._skill_point_list[reborn_count][level] + deity_skill_point;
	}
/*
	static inline int GetHPGenFactor(int level)
	{
		int rst;
		rst = level / 30 + 1;
		return rst;
	}

	static inline int GetMPGenFactor(int level)
	{
		int rst;
		rst = level / 50 + 1;
		return rst;
	}
	static inline bool GetTownPosition(const A3DVECTOR & source, A3DVECTOR & target,int & world_tag)
	{
		//return GetInstance().__GetTownPosition(source,target);
		return city_region::GetCityPos(source.x,source.z,target,world_tag);
	}

	static inline bool IsInSanctuary(const A3DVECTOR & source)
	{
		//return GetInstance().__GetTownPosition(source,target);
		return city_region::IsInSanctuary(source.x,source.z);
	}

*/
	static inline void SetTeamBonus(size_t team_count, size_t cls_count,float * exp)
	{
		//这个操作会在比例的基础值上进行调整 
		ASSERT(team_count < TEAM_MEMBER_CAPACITY + 1);
		ASSERT(cls_count < TEAM_MAX_RACE_COUNT  + 1);
		const player_template & pt = GetInstance();
		*exp *= pt._team_adjust[team_count].exp_adjust + pt._team_cls_adjust[cls_count].exp_adjust;
	}

	static inline void  AdjustGlobalExpSp(int64_t &exp)
	{
		return ;
		//const player_template & pt = GetInstance();
		//exp = (int)(exp * (1 + pt._exp_bonus));
	}

	static inline void GetGlobalExpBonus(float * exp_bonus)
	{
		return;
		//const player_template & pt = GetInstance();
		//*exp_bonus = pt._exp_bonus;
	}

	static inline void SetGlobalExpBonus(float exp_bonus)
	{
		if(exp_bonus <-1.0f || exp_bonus > 100.f) return;

		player_template & pt = GetInstance();
		pt._exp_bonus = exp_bonus;
	}

	static inline  void AdjustGlobalMoney(int & money)
	{
		//const player_template & pt = GetInstance();
		//money = (int)(money * (1 + pt._money_bonus));
	}

	static inline void SetGlobalMoneyBonus(float money_bonus)
	{
		if(money_bonus <-1.0f || money_bonus > 100.f) return;

		player_template & pt = GetInstance();
		pt._money_bonus = money_bonus;
	}
	
	static inline void GetGlobalMoneyBonus(float * money_bonus)
	{
		const player_template & pt = GetInstance();
		*money_bonus = pt._money_bonus; 
	}

	static inline float GetDamageReduce(int defense, int attacker_level)
	{
		float def = defense;
		def = def / (def + 40.f * attacker_level - 25.f);
		if(def > 0.90f) def = 0.90f;
		return def;
	}


	static inline void GetAttackLevelPunishment(int plevel_minus_mlevel, float & adjust)
	{
		const player_template & pt = GetInstance();
		if(plevel_minus_mlevel < BASE_LEVEL_DIFF)
		{
			adjust = pt._level_adjust_table[0][0].attack_adjust;
			return;
		}
		plevel_minus_mlevel = plevel_minus_mlevel - BASE_LEVEL_DIFF;
		if(plevel_minus_mlevel >= MAX_LEVEL_DIFF)
		{
			adjust = pt._level_adjust_table[0][MAX_LEVEL_DIFF-1].attack_adjust;
			return;
		}
		adjust = pt._level_adjust_table[0][plevel_minus_mlevel].attack_adjust;
	}

	static inline void GetExpPunishment(int plevel_minus_mlevel, float * exp, bool reborn = false, bool no_exp_punish = false)
	{
		int i = (reborn || no_exp_punish)  ? 0 :1;
		const player_template & pt = GetInstance();
		if(plevel_minus_mlevel < BASE_LEVEL_DIFF)
		{
			*exp = pt._level_adjust_table[i][0].exp_adjust;
			return;
		}
		plevel_minus_mlevel = plevel_minus_mlevel - BASE_LEVEL_DIFF;
		if(plevel_minus_mlevel >= MAX_LEVEL_DIFF)
		{
			*exp = pt._level_adjust_table[i][MAX_LEVEL_DIFF-1].exp_adjust;
			return;
		}
		*exp = pt._level_adjust_table[i][plevel_minus_mlevel].exp_adjust;
	}

	static inline void GetDropPunishment(int plevel_minus_mlevel,float * money ,float * drop)
	{
		const player_template & pt = GetInstance();
		if(plevel_minus_mlevel < BASE_LEVEL_DIFF)
		{
			*money = pt._level_adjust_table[0][0].money_adjust;
			*drop = pt._level_adjust_table[0][0].item_adjust;
			return;
		}
		plevel_minus_mlevel = plevel_minus_mlevel - BASE_LEVEL_DIFF;
		if(plevel_minus_mlevel >= MAX_LEVEL_DIFF)
		{
			*money = pt._level_adjust_table[0][MAX_LEVEL_DIFF-1].money_adjust;
			*drop = pt._level_adjust_table[0][MAX_LEVEL_DIFF-1].item_adjust;
			return;
		}
		*money = pt._level_adjust_table[0][plevel_minus_mlevel].money_adjust;
		*drop = pt._level_adjust_table[0][plevel_minus_mlevel].item_adjust;
	}

	static  inline float GetRepairCost(int offset, int max_durability,size_t base_price)
	{
		if(max_durability > 0 && offset > 0) 
		{
			float factor = offset / (float)max_durability;
			return base_price * factor;
		}
		else
		{
			return 0;
		}
	}
	/*
	static inline float GetNormalDropMoneyRate()
	{
		return 0.0f;
	}
	
	static inline float GetInvaderDropMoneyRate()
	{
		return 0.15f;
	}

	static inline float GetPariahDropMoneyRate()
	{
		return 0.15f;
	}
*/

	enum
	{       
		PK_VALUE_BLUE_MIN = -6,
		PK_VALUE_BLUE_5 = -6,            //兰
		PK_VALUE_BLUE_4 = -5,            //兰
		PK_VALUE_BLUE_3 = -4,            //兰
		PK_VALUE_BLUE_2 = -3,            //兰
		PK_VALUE_BLUE_1 = -2,            //兰
		PK_VALUE_BLUE_0 = -1,            //兰
		PK_VALUE_WHITE = 0,             //白
		PK_VALUE_RED_0 = 1,             //红
		PK_VALUE_RED_1 = 2,             //红
		PK_VALUE_RED_2 = 3,             //红
		PK_VALUE_RED_3 = 4,             //红
		PK_VALUE_RED_4 = 5,             //红
		PK_VALUE_RED_5 = 6,             //红
		PK_VALUE_RED_MAX ,
	};

	static inline int CalcPKLevel(int pk_value)
	{
		static const int table[] = {-100,-80,-60,-40,-20,-1,0,6,12,24,84,168};
		size_t i;
		for(i = 0; i < sizeof(table) / sizeof(int); i ++)
		{
			if(pk_value <= table[i]) 
			{
				break;
			}
		}
		int rst = i ;
		return rst + PK_VALUE_BLUE_MIN;
	}
	static inline void GetDeathDropRate(int pk_level,int & eq_drop, int & inv_drop)
	{
		static float e_drop_prop[][6] =
		{
			{1.1f,0,0}, {1.1f,0,0},
			{0.9f,0.11f}, {0.5f,0.51f},
			{0.4f,0.3f,0.31f},
			{0.25f,0.25f,0.25f,0.251f}, {0.2f,0.2f,0.2f,0.2f,0.21f},
			{0,0.2f,0.2f,0.2f,0.2f,0.21f}
		};

		static float i_drop_prop[][7] =
		{
			{1.1f,0.0f}, {1.1f,0.0f},
			{0.8f,0.21f}, {0.5f,0.25f,0.251f},
			{0.4f,0.1f,0.2f,0.31f}, {0.30f,0.10f,0.15f,0.20f,0.251f},
			{0.25f,0.05f,0.1f,0.15f,0.20f,0.251f},
			{0,0.05f,0.1f,0.15f,0.20f,0.25f,0.251f},
		};

		if(pk_level >=0) 
			pk_level += 1;
		else if(pk_level < 0 )
			pk_level = 0;
		if(pk_level > PK_VALUE_RED_MAX) pk_level = PK_VALUE_RED_MAX; 	//这其实不会发生的
		eq_drop = abase::RandSelect(e_drop_prop[pk_level],6); 
		inv_drop = abase::RandSelect(i_drop_prop[pk_level],7); 
	}

	static inline void GetHostileDuleDeathDropRate( int& inv_drop )
	{
		static float i_drop_prop[] = { 0, 0.5f, 0.25f, 0.251f };
		inv_drop = abase::RandSelect( i_drop_prop, 4 );
	}

	static inline int UpdatePariahState(int pariah_time)
	{
		return IncPariahState(0,pariah_time);
	}

	static inline int IncPariahState(int cur_state, int pariah_time)
	{	
		static int t[] = {PARIAH_TIME_PER_KILL*2, PARIAH_TIME_PER_KILL*10, PARIAH_TIME_PER_KILL *100};
		for(int i = cur_state ; i < (int)(sizeof(t) / sizeof(int)); i ++)
		{
			if(pariah_time < t[i]) return i;
		}
		return 0;
	}

	static inline int DecPariahState(int cur_state, int pariah_time)
	{
		return IncPariahState(0,pariah_time);
	}

	static inline float GetResurrectExpReduce(size_t sec_level)
	{
		return PLAYER_DEATH_EXP_VANISH;
	}

	static inline float GetAttackPunishment(int attacker_lvl,int defender_lvl)
	{
		int delta = defender_lvl - attacker_lvl;
		if(delta <=5) return 1.0f;
		return 1.5f/(delta - 4);
	}

	static inline bool VerifyClassInvalid(int cls)
	{
		return cls >= 0 && cls < QUSER_CLASS_COUNT;
	}
	static int CalcPKReputation(int repa, int repd);

	static inline int GetSNSRequiredMoney(char m_type, char o_type)
	{
		return GetInstance().__GetSNSRequiredMoney(m_type, o_type);
	}
	
	static inline int GetSNSRequiredItemID(char m_type, char o_type)
	{
		return GetInstance().__GetSNSRequiredItemID(m_type, o_type);
	}
	
	static inline int GetSNSRequiredItemID2(char m_type, char o_type)
	{
		return GetInstance().__GetSNSRequiredItemID2(m_type, o_type);
	}
		
	static inline int GetSNSRequiredTaskID(char m_type, char o_type, int charm)
	{
		return GetInstance().__GetSNSRequiredTaskID(m_type, o_type, charm);
	}

	static inline int GetRankByScore(int map_id, int score)
	{
		return GetInstance().__GetRankByScore(map_id, score);
	}
	
	static inline int GetTaskIDByRank(int map_id, int rank)
	{
		return GetInstance().__GetTaskIDByRank(map_id,rank);
	}

	static inline int GetDeathDropItemID(int map_id, int rank)
	{
		return GetInstance().__GetDeathDropItemID(map_id, rank);
	}

	static inline bool GetDeprivedItemList(int map_id, std::vector<deprive_item> & deprive_item_list)
	{
		return GetInstance().__GetDeprivedItemList(map_id, deprive_item_list);
	}

	static inline int GetKillScoreByRank(int map_id, int rank)
	{
		return GetInstance().__GetKillScoreByRank(map_id, rank);
	}	

	static inline int GetConKillScore(int map_id, int count)
	{
		return GetInstance().__GetConKillScore(map_id, count);	
	}

	static inline int GetKillMonsterScore(int map_id, int monster_id)
	{
		return GetInstance().__GetKillMonsterScore(map_id, monster_id);
	}	

	static inline int GetBattleWinScore(int map_id)
	{
		return GetInstance().__GetBattleWinScore(map_id);
	}

	static inline int GetMapControllerID(int map_id)
	{
		return GetInstance().__GetMapControllerID(map_id);
	}

	static inline int GetInstanceFieldInfo(int map_id, instance_field_info & info)
	{
		return GetInstance().__GetInstanceFieldInfo(map_id, info);
	}
	
	static inline int GetInstanceKillMonsterScore(int map_id, int monster_id)
	{
		return GetInstance().__GetInstanceKillMonsterScore(map_id, monster_id);
	}	

	static inline int GetInstanceWinCtrlID(int map_id)
	{
		return GetInstance().__GetInstanceWinCtrlID(map_id);
	}

	static inline int GetInstanceLevelInfo(int map_id, std::vector<instance_level_info>& info)
	{
		return GetInstance().__GetInstanceLevelInfo(map_id, info);
	}
	static inline int GetInstanceAwardTaskID(int map_id, int total_score, bool b_win)
	{
		return GetInstance().__GetInstanceAwardTaskID(map_id, total_score, b_win);
	}

	static inline bool CheckOriginalInfo(int table_id, int monster_id)
	{
		return GetInstance().__CheckOriginalInfo(table_id, monster_id);
	}
	
	static inline int GetOriginalID(int table_id, int monster_id, int & target_type)
	{
		return GetInstance().__GetOriginalID(table_id, monster_id, target_type);
	}

	static inline bool GetArenaDropList(int map_id, std::set<int>& drop_list)
	{
		return GetInstance().__GetArenaDropList(map_id, drop_list);
	}	     

	static inline bool GetCombineEditCD(int skill_id, int & cool_index, int & cool_time)
	{
		return GetInstance().__GetCombineEditCD(skill_id, cool_index, cool_time);
	}	

	//140装备
	static inline bool CanEquipUpgrade(int equip_id)
	{
		return GetInstance().__CanEquipUpgrade(equip_id);
	}	
	
	static inline bool IsUpgradeEquipment(int equip_id)
	{
		return GetInstance().__IsUpgradeEquipment(equip_id);
	}
	
	static inline void GetEquipUpgradeStoneInfo(int equip_id, int & stone_id, int & stone_num)
	{
		GetInstance().__GetEquipUpgradeStoneInfo(equip_id, stone_id, stone_num);
	}

	static inline int GetEquipUpgradeOutputID(int equip_id)
	{
		return GetInstance().__GetEquipUpgradeOutputID(equip_id);
	}
	
	static inline const OnlineGiftbagConfigMap & GetOnlineGiftbagConfigMap()
	{
		return GetInstance().__GetOnlineGiftbagConfigMap();
	}

	static inline int GetTreasureRegionLevel(int id)
	{
		return GetInstance().__GetTreasureRegionLevel(id);
	}

	static inline bool UpgradeTreasureRegion(int region_id, int region_index, int cur_lvl, int & new_lvl)
	{
		return GetInstance().__UpgradeTreasureRegion(region_id, region_index, cur_lvl, new_lvl);
	}	      

	static inline int GetUnlockTreasureRegionID(int region_id, int region_index)
	{
		return GetInstance().__GetUnlockTreasureRegionID(region_id, region_index);
	}

	static inline void GetTreasureAward(int region_id, int region_index, int region_level, abase::vector<treasure_award_item> & award_list, char & event_id)
	{
		GetInstance().__GetTreasureAward(region_id, region_index, region_level, award_list, event_id);
	}

	static inline int GetTreasureRegionID(int region_index)
	{
		return GetInstance().__GetTreasureRegionID(region_index);
	}

	static inline int GetLittlePetFeedExp(int feed_id)
	{
		return GetInstance().__GetLittlePetFeedExp(feed_id);
	}

	static inline int GetLittlePetLevelupExp(int cur_lvl)
	{
		return GetInstance().__GetLittlePetLevelupExp(cur_lvl);
	}
	
	static inline int GetLittlePetAwardItem(int cur_lvl)
	{
		return GetInstance().__GetLittlePetAwardItem(cur_lvl);
	}

	static inline int GetRuneRequireLevel(int rune_lvl)
	{
		return GetInstance().__GetRuneRequireLevel(rune_lvl);
	}
	
	static inline int GetRuneRequireReborn(int rune_lvl)
	{
		return GetInstance().__GetRuneRequireReborn(rune_lvl);
	}

	static inline int GetRuneIdentifyExp()
	{
		return GetInstance().__GetRuneIdentifyExp();
	}
	
	static inline int GetRuneRefineExp()
	{
		return GetInstance().__GetRuneRefineExp();
	}
	
	static inline int GetRuneResetExp()
	{
		return GetInstance().__GetRuneResetExp();
	}

	//到达当前等级需要的经验
	static inline int GetRuneLevelExp(int cur_lvl)
	{
		return GetInstance().__GetRuneLevelExp(cur_lvl);
	}
	
	//升级需要的经验
	static inline int GetRuneLevelUpExp(int cur_lvl)
	{
		return GetInstance().__GetRuneLevelUpExp(cur_lvl);
	}

	static inline unsigned int GetRuneComboID(abase::vector<int> & stone_list)
	{
		return GetInstance().__GetRuneComboID(stone_list);
	}
	
	static inline void GetRuneComboAddon(int combo_id, abase::vector<int> & addon_list)
	{
		return GetInstance().__GetRuneComboAddon(combo_id, addon_list);
	}

	static inline bool IsRuneIdentifyItemID(unsigned int id)
	{
		return GetInstance().__IsRuneIdentifyItemID(id);
	}

	static inline unsigned int IsRuneRefineItemID(unsigned int id)
	{
		return GetInstance().__IsRuneRefineItemID(id);
	}
	
	static inline unsigned int IsRuneResetItemID(unsigned int id)
	{
		return GetInstance().__IsRuneResetItemID(id);
	}
	
	static inline unsigned int IsRuneChangeSlotItemID(unsigned int id)
	{
		return GetInstance().__IsRuneChangeSlotItemID(id);
	}

	static inline unsigned int GetRuneActiveLevel(int cur_level)
	{
		return GetInstance().__GetRuneActiveLevel(cur_level);
	}

	static inline void GetRuneActiveProp(int active_level, int & damage, int & hp, int & mp, int & dp)
	{
		return GetInstance().__GetRuneActiveProp(active_level, damage, hp, mp, dp);
	}

	static inline int GetRuneDecomposeExp(int avg_grade)
	{
		return GetInstance().__GetRuneDecomposeExp(avg_grade);
	}

	static inline bool GetCountDrop(int itemId, count_drop_item& item)
	{
		return GetInstance().__GetCountDrop(itemId, item);
	}

	static inline void GetAllCountDrops(std::vector<count_drop_item>& items)
	{
		GetInstance().__GetAllCountDrops(items);
	}

	static inline const pk_bet_data GetPKBetData()
	{
		return GetInstance().__GetPKBetData();
	}

	// index [1,maxIndex]
	static inline const bool GetUITransferEntry(size_t index, ui_transfer_config_entry& ret) 
	{
		if (index == 0 || index > _instance._ui_transfer_config.size()) return false;
		ret = _instance._ui_transfer_config[index - 1];
		return true;
	}
	
	static inline liveness_cfg * GetLivenessCfg() 
	{
		return &(_instance._liveness_cfg);	
	}
	
	static inline int GetCollisionDailyAwardItem()
	{
		return GetInstance().__GetCollisionDailyAwardItem();
	}

	static inline bool GetCollisionAwardItem(int index, unsigned int & item_id, unsigned int & item_count, unsigned int & need_score_a, unsigned int & need_score_b, unsigned int & win_num)
	{
		return GetInstance().__GetCollisionAwardItem(index, item_id, item_count, need_score_a, need_score_b, win_num);
	}

	static inline int GetPropAddLimit(int idx)
	{
		return GetInstance().__GetPropAddLimit(idx);
	}

	static inline void GetKingdomMonsterID(int & attacker_npc_id, int & defender_npc_id)
	{
		GetInstance().__GetKingdomMonsterID(attacker_npc_id, defender_npc_id);
	}

	static inline int GetKingdomChangeHP(int type)
	{
		return GetInstance().__GetKingdomChangeHP(type);
	}

	static inline void GetKingdomShop(int index, int & item_id, int & item_num, int & need_score)
	{
		return GetInstance().__GetKingdomShop(index, item_id, item_num, need_score);
	}

	static inline int GetXPSkill(int cls)
	{
		return GetInstance().__GetXPSkill(cls);
	}

	static inline int GetBathAwardPeriod()
	{
		return GetInstance().__GetBathAwardPeriod();
	}

	static inline void GetBathAwardItem(int & item_id, int & item_count)
	{
		return GetInstance().__GetBathAwardItem(item_id, item_count);
	}

	static inline int GetFuwenLevel(int exp, int quality)
	{
		if(quality < 0 || quality > 3) return 0;
		return GetInstance().__GetFuwenLevel(exp, quality);
	}

	static inline int GetFuwenMaxLevel()
	{
		return GetInstance().__GetFuwenMaxLevel();
	}

	static inline int GetFuwenMaxExp(int quality)
	{
		return GetInstance().__GetFuwenMaxExp(quality);
	}

	static inline bool GetCS6V6PersonalLevelAwardItem(int level, void * award_item)
	{
		return GetInstance().__GetCS6V6PersonalLevelAwardItem(level, award_item);
	}

	static inline bool GetCS6V6PersonalLevelAwardPoint(int level, int & max_point, int & min_point)
	{
		return GetInstance().__GetCS6V6PersonalLevelAwardPoint(level, max_point, min_point);
	}

	static inline bool GetCS6V6TeamAwardItem(int team_rank, void * award_item)
	{
		return GetInstance().__GetCS6V6TeamAwardItem(team_rank, award_item);
	}
	
	
	static inline bool GetCS6V6ExchangeAwardItem(int team_rank, int & item_id, int & item_count, int & expire_time)
	{
		return GetInstance().__GetCS6V6ExchangeAwardItem(team_rank, item_id, item_count, expire_time);
	}

	static inline bool GetCS6V6ExchangeItemInfo(int index, int & exchange_item_id, int & exchange_item_num, int & require_item_id, int & require_item_count, int & expire_time, int & exchange_max_count)
	{
		return GetInstance().__GetCS6V6ExchangeItemInfo(index, exchange_item_id, exchange_item_num, require_item_id, require_item_count, expire_time, exchange_max_count);
	}	

};

class property_policy
{
	static inline int Result(int base, int base2, int percent_enhance)
	{
		int rst = (int)((base + base2)*0.001f*((float)( 1000 + percent_enhance)) + 0.5f);
		if(rst < 0) rst = 0;
		return rst;
	}

	static inline int Result2(int base, int percent_enhance, int offset)
	{
		int rst = (int)(base * 0.001f * (percent_enhance + 1000) + 0.5f);
		if(rst < 0) rst = 0;
		rst += offset;
		if(rst < 0) rst = 0;
		return rst;
	}
public:
	static void UpdatePlayer(int cls,gactive_imp * pImp);
	static void UpdateNPC(gactive_imp *pImp);
	static void UpdatePet(gactive_imp *pImp);
	static void UpdatePlayerBaseProp(int cls,gactive_imp * pImp);
	static void UpdateMPHPGen(gactive_imp * pImp);
	static void UpdateDPGen(gactive_imp * pImp);
	static void UpdateNPCBaseProp(gactive_imp * pImp);
	static void UpdateAttack(int cls,gactive_imp * pImp);
	static void UpdatePetAttack(int cls,gactive_imp * pImp);
	static void UpdateCrit(gactive_imp * pImp);
	static void UpdateTransform(gactive_imp * pImp);
	static void UpdateInvisible(gactive_imp * pImp);
	static void UpdateMagic(gactive_imp * pImp);
	static void UpdateSpeed(gactive_imp *pImp);
	static void UpdateDefense(int cls,gactive_imp *pImp);

	/*
	*	部分update
	*/
	static void UpdateLife(gactive_imp * pImp);
	static void UpdateMana(gactive_imp * pImp);
	static void UpdateDeity(gactive_imp * pImp);

};

#endif

