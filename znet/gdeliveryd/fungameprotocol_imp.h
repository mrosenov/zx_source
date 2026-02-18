#ifndef __GNET_FUNGAME_PROTOCOL_IMP_H
#define __GNET_FUNGAME_PROTOCOL_IMP_H

#include "fungamepacket.h"
#include "fungameprotocol.h"

namespace FUNGAME
{

namespace C2S
{
	namespace CMD
	{
		template <typename CMD>
		struct Make;
		template <>
		struct Make<fun_game_c2s_cmd_header>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, unsigned char cmd)
			{
				return wrapper << gametype << cmd;
			}
		};

		template <>
		struct Make<INFO::normal_card_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, std::vector<char> &cards)
			{
				wrapper << static_cast<char>(cards.size());
				std::vector<char>::iterator it = cards.begin(), ie = cards.end();
				for(; it != ie; ++it)
					wrapper << *it;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_throw_normal_card>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, std::vector<char> &cards)
			{
				Make<fun_game_c2s_cmd_header>::From(wrapper, gametype, PLAYER_THROW_NORMAL_CARD);
				wrapper << roomid;
				Make<INFO::normal_card_info>::From(wrapper, cards);
				return wrapper;
			}
		};
	};
};

namespace S2C
{
	namespace CMD
	{

		template <typename CMD>
		struct Make;

		template <>
		struct Make<fun_game_s2c_cmd_header>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, unsigned char command)
			{
				return wrapper << gametype << command;
			}
		};

		template <>
		struct Make<INFO::player_base_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,const GameActor & Player)
			{
				wrapper << Player.roleid;
				wrapper << Player.direction;
				wrapper << Player.state;
				wrapper << Player.gender;
				wrapper << Player.score;
				wrapper << Player.win_count;
				wrapper << Player.lose_count;
				wrapper << Player.draw_count;
				wrapper << Player.escape_count;

				char size = Player.rolename.size();
				if(size > 20) size = 20;
				wrapper << size;
				wrapper.push_back(Player.rolename.begin(), size);
				return wrapper;
			}
		};

		template <>
		struct Make<INFO::player_state>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,const GameActor & Player)
			{
				wrapper << Player.roleid;
				wrapper << Player.direction;
				wrapper << Player.state;
				return wrapper;
			}
		};

		template <>
		struct Make<INFO::room_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, Room* room)
			{
				wrapper <<  room->_roomid;
				char player_size = room->_actors.size();
				wrapper <<  player_size;
				std::map<int, GameActor *>::iterator  it = room->_actors.begin();
				for(    ; it!=room->_actors.end(); ++it)
				{
					CMD::Make<INFO::player_base_info>::From(wrapper, *(it->second));
				} 		 
				return wrapper; 
			}
		};

		template <>
		struct Make<INFO::normal_card_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,char* cards, char card_size)
			{
				wrapper << card_size;
				for(int i=0; i<card_size; ++i)
					wrapper << cards[i];
				return wrapper;
			}

			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, std::vector<char> &cards)
			{
				wrapper << static_cast<char>(cards.size());
				std::vector<char>::iterator it = cards.begin(), ie = cards.end();
				for(; it != ie; ++it)
					wrapper << *it;
				return wrapper;
			}
		};



		/********************************
		 * CMD wrapper 
		 *********************************/

		template <>
		struct Make<CMD::player_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, const GameActor & Player)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_INFO);
				Make<INFO::player_base_info>::From(wrapper, Player);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_enter_hall>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, const GameActor & player)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_ENTER_HALL);
				Make<INFO::player_base_info>::From(wrapper, player);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_leave_hall>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, int roleid)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_LEAVE_HALL);
				wrapper << roleid;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_enter_room>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype,short roomid, const GameActor & player)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_ENTER_ROOM);
				wrapper << roomid;
				Make<INFO::player_base_info>::From(wrapper, player);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_leave_room>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, int roleid, short roomid)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_LEAVE_ROOM);
				return wrapper << roleid << roomid;
			}
		};

		template <>
		struct Make<CMD::player_state_change>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, short roomid,const GameActor & Player)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_STATE_CHANGE);
				wrapper << roomid;
				Make<INFO::player_state>::From(wrapper, Player);
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::player_reenter_invite>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, short roomid)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_REENTER_INVITE);
				wrapper << roomid;
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::room_game_is_ready>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, short roomid)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, ROOM_GAME_IS_READY);
				wrapper << roomid;
				return wrapper; 
			}
		};


		template <>
		struct Make<CMD::notify_room_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, Room* room)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, NOTIFY_ROOM_INFO);
				CMD::Make<INFO::room_info>::From(wrapper, room);
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::get_room_list>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, Room** room, char room_size)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, GET_ROOM_LIST);
				wrapper << room_size;
				for(int i=0; i<room_size; ++i)
				{
					CMD::Make<INFO::room_info>::From(wrapper, *room);
					room += 1; //注意这里的遍历顺序 
				}
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::notify_hall_members>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, unsigned char gametype, BaseGameManager::HallMembers & members)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, NOTIFY_HALL_MEMBERS);
				BaseGameManager::HallMembers onlyinhall;
				BaseGameManager::HallMemberIt it = members.begin(), ite = members.end();
				for (; it != ite; ++it)
				{
					if (it->second.state == GameActor::ACTOR_STATE_HALL)
						onlyinhall.insert(*it);
				}
				wrapper << onlyinhall.size();
				it = onlyinhall.begin(), ite = onlyinhall.end();
				for(; it != ite; ++it)
				{
					CMD::Make<INFO::player_base_info>::From(wrapper, it->second);
				}
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::room_dispatch_normal_card>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, int next_roleid, char* cards, char card_size)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, ROOM_DISPATCH_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << next_roleid;
				Make<INFO::normal_card_info>::From(wrapper, cards, card_size);
				return wrapper;
			}

			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, int next_roleid, std::vector<char> &cards)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, ROOM_DISPATCH_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << next_roleid;
				Make<INFO::normal_card_info>::From(wrapper, cards);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_inturn_normal_card>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_INTURN_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_throw_normal_card>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, int next_roleid, char* cards, char card_size)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_THROW_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << next_roleid;
				Make<INFO::normal_card_info>::From(wrapper, cards, card_size);
				return wrapper;
			}

			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, int next_roleid, std::vector<char> &cards)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_THROW_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << next_roleid;
				Make<INFO::normal_card_info>::From(wrapper, cards);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_pickup_normal_card>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, char* cards, char card_size)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_PICKUP_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				Make<INFO::normal_card_info>::From(wrapper, cards, card_size);
				return wrapper;
			}

			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, std::vector<char> &cards)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_PICKUP_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				Make<INFO::normal_card_info>::From(wrapper, cards);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_giveup_normal_card>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, int next_roleid)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_GIVEUP_NORMAL_CARD);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << next_roleid;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_auction>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, int next_role, char score)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, PLAYER_AUCTION);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << next_role;
				wrapper << score;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::room_game_end>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, int roleid, char reason)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, ROOM_GAME_END);
				wrapper << roomid;
				wrapper << roleid;
				wrapper << reason;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::landlord_result_broadcast>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, short roomid, char size, const std::vector<std::pair<int, int> > &results)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, LANDLORD_RESULT_BROADCAST);
				wrapper << roomid;
				wrapper << static_cast<char>(results.size());
				for (size_t i = 0; i < results.size(); ++i)
				{
					wrapper << results[i].first;
					wrapper << results[i].second;
				}

				return wrapper;
			}
		};

		template <>
		struct Make<CMD::landlord_reonline_datasend>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,unsigned char gametype, Room *_proom, landlord_reonline_info & info)
			{
				Make<fun_game_s2c_cmd_header>::From(wrapper, gametype, LANDLORD_REONLINE_DATASEND);
				Make<INFO::room_info>::From(wrapper, _proom);
				wrapper << info.score;
				wrapper << info.lord_dir;
				wrapper << info.desk_dir;
				wrapper << info.next_dir;
				for (int i = 0; i <= 1; ++i)
				{
					wrapper << info.cards[i].direct;
					wrapper << info.cards[i].card_size;
				}
				Make<INFO::normal_card_info>::From(wrapper, info.selfcards);
				Make<INFO::normal_card_info>::From(wrapper, info.deskcards);

				return wrapper;
			}
		};
	};

}; // end namespace S2C


}; //end namespace FUNGAME 

#endif
