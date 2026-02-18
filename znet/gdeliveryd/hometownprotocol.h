#ifndef HOMETOWN_HOMETOWNPROTOCOL_H
#define HOMETOWN_HOMETOWNPROTOCOL_H 
#include <set>
#include <vector>
#include "common/octets.h"

namespace HOMETOWN
{
typedef std::set<int> PACK_SET;
using GNET::Octets;
#pragma pack(1)
	enum										//CMD type
	{
		COMPO_LIST = 0			,				//列举自己已添加的组件,该命令忽略c2s_hometown_cmd_header中的compo_id字段
		COMPO_ADD   			,				//添加组件
		EXCHG_MONEY				,				//通用币和组件货币互换, compo_id指明了组件货币
		LEVEL_UP				,				//升级
		BUY_ITEM				,				//购买物品
		USE_ITEM				,				//使用物品
		SELL_ITEM				,				//卖物品
		PACKAGE_LIST			,				//包裹查看
		FARM_VIEW     			,				//查看
		FARM_VIEW_FIELD			,				//查看一块地
		FARM_PLOW_FIELD 		,				//犁地
		FARM_SOW_FIELD			,				//播种
		FARM_WATER_FIELD		,				//浇水
		FARM_PEST_FIELD			,				//杀虫
		FARM_WEED_FIELD			,				//除草
		FARM_HARVEST_FIELD		,				//收获
		FARM_STEAL_FIELD		,				//偷窃
		FARM_LIST_LOVE_FIELD	,				//列举哪些好友家中有我耕种的爱心地
		FARM_LIST_MATURE_FIELD	,				//列举哪些好友家中有成熟的地
	};

	enum FARM_TARGET_TYPE						//农场组件中USE_ITEM的目标类型
	{
		FARM_TARGET_SELF = 0,					//自身
		FARM_TARGET_PLAYER	,					//其他玩家
		FARM_TARGET_FARM,						//整个菜园
		FARM_TARGET_FIELD,						//单独一块菜地
		FARM_TARGET_CROP,						//单独一棵庄稼
	};

	struct crop_info							//庄稼信息
	{
		int id;									//庄稼模板id
		int seed_time;							//播种时间
		int ripe_time;							//还有多少时间成熟,只有在未成熟之前才有效,但都会发
		int protect_period;						//剩余防偷保护期
		int state;								//状态
		int fruit_left;							//当前剩余果实数 只在成熟状态下才有效,但都会发
		int fruit_total;						//被偷的果实数, 只在成熟或被偷光状态下才有效,但都会发
	};
	struct field_info							//地块信息
	{
		int field_id;
		char field_type;						//0:未开垦, 1:普通地，2：爱心地
		int state;								//地块当前的状态
		int friend_id;							//爱心地播种者角色id
		char friend_name_size;	
		char friend_name[20];
		struct crop_info crop;					//庄稼信息,不在耕种状态下才会发该字段
	};
	struct farm_player
	{
		int id;
		int64_t charm; 
		int64_t money; 
		int level; 
		int state;								//人当前的状态
	};
	struct farm_info
	{
		int state;								//整个农场的状态列表
		int protect_period;						//整个农场的剩余保护时间
		struct farm_player owner;
		std::vector<struct field_info> fields;
		std::vector<Octets> farm_dynamic;
		std::vector<Octets> action_dynamic;
	};
	struct item_data
	{
		unsigned int id;
		int pos;
		int count;
		int max_count;
		Octets content;
	};
	struct pocket_info
	{
		int capacity;					//包裹大小
		std::vector<item_data> items;	//物品列表
	};

	namespace DYNAMIC					//我的行动和菜园动态结构
	{
		struct dynamic_header
		{
			int time;
			int action;
		};

		//只提示自己
		struct exchg_money
		{
			char type;				//参见hometownprotocol.h
			int64_t local_money;	//兑换的本币数
			int64_t foreign_money;	//兑换的外币数
		};

		//只提示自己
		struct level_up
		{
			int level;				//升级后等级
		};

		//只提示自己
		struct buy_item
		{
			int item_id;
			int count;
			int64_t cost1;			//含义同S2C中的协议
			int64_t cost2;
		};

		//只提示自己
		struct sell_item
		{
			int item_id;
			int count;
			int64_t earning;
		};

		//对好友使用了物品,提示自己和提示好友
		struct use_item
		{
			int sponsor;				//使用者角色id
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//目标角色id
			char target_name_size;	
			char target_name[20];
			int item_id;
			int target_type;
		};

		//在好友的爱心地犁地，提示自己和好友
		struct plow_field
		{
			int sponsor;				//犁地人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被犁地的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//在好友的爱心地播种，提示自己和好友
		struct sow_field
		{
			int sponsor;				//播种人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被播种的人
			char target_name_size;	
			char target_name[20];
			int seed_id;
		};

		//在好友的地里浇水，提示自己和好友
		struct water_field
		{
			int sponsor;				//浇水人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被浇水的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//在好友的地里捉虫，提示自己和好友
		struct pest_field
		{
			int sponsor;				//捉虫人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被捉虫的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//在好友的地里除草，提示自己和好友
		struct weed_field
		{
			int sponsor;				//除草人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被除草的人
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//收获了自己的土地或好友的爱心地,只出现在我的行动中
		struct harvest_field
		{
			int sponsor;				//收获的人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被收获的人
			char target_name_size;	
			char target_name[20];
			int fruit_id;
			int count;
		};

		//自己的爱心地或者自己种的爱心地被好友收获，得到果实，只出现在菜园动态中
		struct harvest_lovefield
		{
			int sponsor;
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;
			char target_name_size;	
			char target_name[20];
			int fruit_id;
			int gain_count;				//实际获得的果实数
			int drop_count;				//因包裹满丢失的数目
		};

		//偷好友庄园，提示自己和好友
		struct steal_field
		{
			int sponsor;				//偷窃的人
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//被偷窃的人
			char target_name_size;	
			char target_name[20];
			int emotion;
			int fruit_id;
			int count;
		};
	};

	namespace S2C
	{
		struct hometown_s2c_cmd_header
		{
			int retcode;			
			int cmd_type;
			int compo_id;						//组件id
		};

		namespace CMD
		{
			struct compo_list
			{
				int roleid;
				int64_t money;
				PACK_SET compo_list;		//玩家添加的组件ID列表，只在retcode==HT_ERR_SUCCESS才存在
			};

			struct compo_add
			{
			};
			struct exchg_money
			{
				char type;						//同C2S
				int64_t money;					//同C2S
				//下面两项只在HT_ERR_SUCCESS才有
				int64_t hometown_money;			//兑换完成后通用币数目
				int64_t compo_money;			//兑换完成后组件币数目
			};
			struct level_up
			{
				//只在HT_ERR_SUCCESS才存在
				//对于不同的组件有不同的值
				//struct farm_info farm;		农场组件升级后的结果
			};
			struct buy_item
			{       
				int item_id;        			//购买物品ID
				int count;          			//购买数量

				//下面只在retcode==HT_ERR_SUCCESS才存在
				//花费，不同组件可能花费不同的货币
				//农场组件，cost1表示消耗的魅力值, cost2表示消耗的金钱
				int64_t cost1;					
				int64_t cost2;
			};      
			struct use_item
			{
				int pos;			//使用1个位于包裹栏pos的物品
				int item_id;		//用于校验
				int target_type;	//目标类型
									//对于农场组件如下：
									//FARM_TARGET_SELF = 0,	自身
									//FARM_TARGET_PLAYER,其他玩家
									//FARM_TARGET_FARM,整个菜园
									//FARM_TARGET_FIELD,单独一块菜地
									//FARM_TARGET_CROP,单独一棵庄稼

				int roleid;			//目标roleid, 在FARM_TARGET_SELF情况忽略
				int field_id;		//目标地块id, 只在FARM_TARGET_FIELD, FARM_TARGET_CROP有效
				char consumed;		//0:不消耗，1:消耗
			};
			struct sell_item
			{       
				int pos;            //卖位于包裹栏pos的物品
				int item_id;		//物品ID，用于检验
				int count;          //卖的数量
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int64_t earning;	//收入，不同组件可能获得不同的收入
									//对于农场组件是农场币
			};  
			struct package_list
			{
				//下面只在retcode==HT_ERR_SUCCESS才存在
				struct pocket_info pktinfo;		//玩家在组件里的包裹信息，只有HT_ERR_SUCCESS才存在
			};
			struct farm_view
			{
				int roleid;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				struct farm_info farm;			//只在HT_ERR_SUCCESS才存在
			};
			struct farm_view_field
			{
				int roleid;
				int field_id;
				//下面只在HT_ERR_SUCCESS才存在
				struct field_info field;
			};
			struct farm_plow_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_sow_field
			{
				int roleid;
				int field_id;
				int seed_id;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int cost;						//种子耗费的金钱数
			};
			struct farm_water_field
			{
				int roleid;
				int field_id;
				int emotion;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int charm_change;
			};
			struct farm_pest_field
			{
				int roleid;
				int field_id;
				int emotion;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int charm_change;
			};
			struct farm_weed_field
			{
				int roleid;
				int field_id;
				int emotion;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int charm_change;
			};
			struct farm_harvest_field
			{
				int roleid;
				int field_id;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int charm_add;
				int fruit_id;
				int fruit_count;
			};
			struct farm_steal_field
			{
				int roleid;
				int field_id;
				int emotion;
				//下面只在retcode==HT_ERR_SUCCESS才存在
				int charm_change;
				int fruit_id;
				int fruit_count;
			};
			struct farm_list_love_field
			{
				PACK_SET friends;
			};
			struct farm_list_mature_field
			{
				PACK_SET friends;
			};
		};
	};

	namespace C2S
	{
		struct hometown_c2s_cmd_header
		{
			int cmd_type;
			int compo_id;
		};

		namespace CMD
		{
			struct compo_list
			{
				int roleid;
			};
			struct compo_add
			{
				//没有数据
			};
			struct exchg_money
			{
				char type;		//0表示通用币换成组件币，1:相反
				int64_t money;	//兑换的本币数，type==0:通用货币数，type==1:组件币数
			};
			struct level_up
			{
			};
			struct buy_item
			{
				int item_id;		//购买物品ID
				int count;			//购买数量
			};
			struct use_item
			{
				int pos;			//使用1个位于包裹栏pos的物品
				int item_id;
				int target_type;	//目标类型
									//对于农场组件如下：
									//FARM_TARGET_SELF = 0,	自身
									//FARM_TARGET_PLAYER,其他玩家
									//FARM_TARGET_FARM,整个菜园
									//FARM_TARGET_FIELD,单独一块菜地
									//FARM_TARGET_CROP,单独一棵庄稼

				int roleid;
				int field_id;
			};
			struct sell_item
			{
				int pos;			//卖位于包裹栏pos的物品
				int item_id;		//物品ID，用于检验
				int count;			//卖的数量
			};
			struct package_list
			{
			};
			struct farm_view
			{
				int roleid;
			};
			struct farm_view_field
			{
				int roleid;
				int field_id;
			};
			struct farm_plow_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_sow_field
			{
				int roleid;
				int field_id;
				int seed_id;
			};
			struct farm_water_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_pest_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_weed_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_harvest_field
			{
				int roleid;
				int field_id;
			};
			struct farm_steal_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_list_love_field
			{
			};
			struct farm_list_mature_field
			{
			};
		};
	};
#pragma pack()
};
#endif
