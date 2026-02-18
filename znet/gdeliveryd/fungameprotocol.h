#ifndef __GNET_FUNGAME_PROTOCOL_H
#define __GNET_FUNGAME_PROTOCOL_H

namespace FUNGAME
{
	
#pragma pack(1)
namespace S2C
{
	struct fun_game_s2c_cmd_header 
	{
		unsigned char game_type;
		unsigned char cmd;
	};

	namespace INFO
	{
		struct player_base_info
		{
			int roleid;
			char direction;
			char state;
			char gender;
			int64_t score;
			int win_count;
			int lose_count;
			int draw_count;    //平局数
			int escape_count;  //逃跑局数
			char name_size;
			char name[20];
		};

		struct room_info
		{
			short roomid;
			char player_size;
			player_base_info players[];
		};

		struct player_state
		{
			int roleid;
			char direction;
			char state;
		};

		struct normal_card_info
		{
			char card_size;
			char cards[];
		};

		struct landlord_result_info
		{
			int roleid;	
			int score;
		};
		struct landlord_card_info
		{
			char direct;	
			char card_size;
		};
		struct landlord_reonline_info
		{
			room_info _room_info;
			int score;
			char lord_dir;  // 地主位置
			char desk_dir;  // 桌面牌所有者
			char next_dir;  // 当前出牌人 
			landlord_card_info cards[2];// 其他玩家手中牌信息
			std::vector<char> selfcards; // 手中牌
			std::vector<char> deskcards; // 桌面牌
		};
	}; 

	enum  //CMD
	{
		GET_ROOM_LIST,		//玩家获取房间列表
		NOTIFY_ROOM_INFO,	//通知房间信息
		PLAYER_INFO,		//通知玩家信息
		PLAYER_ENTER_ROOM, 	//玩家进入房间
		PLAYER_LEAVE_ROOM, 	//玩家离开房间
		PLAYER_STATE_CHANGE,	//玩家状态变化通知 
		PLAYER_REENTER_INVITE,	//玩家断线后 邀请玩家继续游戏 

		ROOM_GAME_IS_READY, 	//游戏人数已够， 等待玩家开始；
		ROOM_DISPATCH_NORMAL_CARD,  //游戏开始 发牌
		PLAYER_PICKUP_NORMAL_CARD, //玩家拿底牌 
		PLAYER_INTURN_NORMAL_CARD, //轮到由该玩家出牌 
		PLAYER_THROW_NORMAL_CARD, //玩家出牌
		PLAYER_GIVEUP_NORMAL_CARD, //玩家跳过出牌 

		PLAYER_AUCTION,
		ROOM_GAME_END,
		LANDLORD_RESULT_BROADCAST, //广播游戏结果
		LANDLORD_REONLINE_DATASEND, //重上线后发送当前数据

		PLAYER_ENTER_HALL,	//进入大厅
		PLAYER_LEAVE_HALL,	//离开大厅
		NOTIFY_HALL_MEMBERS,	//返回大厅中的玩家列表
	};

	namespace CMD
	{
		using namespace INFO;

		struct get_room_list 
		{
			fun_game_s2c_cmd_header header;
			char room_size;
			room_info data[];
		};

		struct notify_room_info
		{
			fun_game_s2c_cmd_header header;
			room_info data;
		};

		struct notify_hall_members
		{
			fun_game_s2c_cmd_header header;
			int member_num;
			player_base_info players[];
		};

		struct player_info
		{
			fun_game_s2c_cmd_header header;
			player_base_info data;
		};

		struct player_enter_hall
		{
			fun_game_s2c_cmd_header header;
			player_base_info data;
		};

		struct player_leave_hall
		{
			fun_game_s2c_cmd_header header;
			int roleid;
		};

		struct player_enter_room
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			player_base_info data;
		};

		struct player_leave_room
		{
			fun_game_s2c_cmd_header header;
			int roleid;
			short roomid;
		};

		struct player_state_change
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			player_state data;
		};

		struct player_reenter_invite
		{
			fun_game_s2c_cmd_header header;
			short roomid;
		};

		struct room_game_is_ready 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
		};

		struct room_dispatch_normal_card
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
			int next_roleid;
			normal_card_info cards;
		};

		struct player_inturn_normal_card 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
		};

		struct player_throw_normal_card 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
			int next_roleid; //下一个出牌人 
			normal_card_info cards;
		};

		struct player_pickup_normal_card 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
			normal_card_info cards;
		};

		struct player_giveup_normal_card
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			int roleid;
			int next_roleid; //下一个出牌人 
		};
		struct player_auction
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			int roleid;
			int next_roleid; //下一个出牌人 
			char score; //分值
		};
		struct room_game_end
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			int roleid;
			char reason; //结束原因
		};
		struct landlord_result_broadcast
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			char size;
			landlord_result_info results[];
		};
		struct landlord_reonline_datasend
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			landlord_reonline_info info;
		};
	};

}; // end namespace S2C

namespace C2S
{
	struct fun_game_c2s_cmd_header
	{
		unsigned char game_type;
		unsigned char cmd_type;
	};

	namespace INFO
	{
		struct normal_card_info
		{
			char card_size;
			char cards[];
		};
	}; 

	enum  //CMD
	{
		GET_ROOM_LIST,   	//玩家获取房间列表
		PLAYER_ENTER_ROOM,	//玩家进入房间 
		PLAYER_LEAVE_ROOM, 	//玩家离开房间
		PLAYER_BEGIN_GAME,	//玩家开始游戏
		PLAYER_REENTER_ROOM,	//玩家断线后 重新进入房间

		PLAYER_PICKUP_NORMAL_CARD, //玩家拿底牌 
		PLAYER_THROW_NORMAL_CARD, //玩家出牌
		PLAYER_GIVEUP_NORMAL_CARD, //玩家跳过出牌 
		PLAYER_AUCTION,		//叫牌

		PLAYER_ENTER_HALL,	//玩家进入大厅
		PLAYER_LEAVE_HALL,	//玩家离开大厅
	};

	namespace CMD
	{
		using namespace INFO;

		struct player_enter_hall
		{
			fun_game_c2s_cmd_header header;
		};

		struct player_leave_hall
		{
			fun_game_c2s_cmd_header	header;
		};

		struct get_room_list 
		{
			fun_game_c2s_cmd_header	header;
			char index;
		};

		struct player_enter_room
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
			char direction;
		};

		struct player_begin_game
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_leave_room
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_reenter_room
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_throw_normal_card
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
			normal_card_info cards;
		};

		struct player_pickup_normal_card
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_giveup_normal_card
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};
		struct player_auction
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
			int roleid;
			char score; //分值
		};
	};


}; // end namespace C2S

#pragma pack()  
}; //end namespace FUNGAME 

#endif
