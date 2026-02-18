#include "landlordgame.h"
#include <list>

namespace GNET
{

char lord_card_values[55] = 
{
	//{黑, 红, 梅, 方}
	0x00,
	0x03, 0x13, 0x23, 0x33,
	0x04, 0x14, 0x24, 0x34,
	0x05, 0x15, 0x25, 0x35,
	0x06, 0x16, 0x26, 0x36,
	0x07, 0x17, 0x27, 0x37,
	0x08, 0x18, 0x28, 0x38,
	0x09, 0x19, 0x29, 0x39,
	0x0a, 0x1a, 0x2a, 0x3a,
	0x0b, 0x1b, 0x2b, 0x3b,
	0x0c, 0x1c, 0x2c, 0x3c,
	0x0d, 0x1d, 0x2d, 0x3d,
	0x01, 0x11, 0x21, 0x31,
	0x02, 0x12, 0x22, 0x32,
	0x40, 0x80
};

char *lord_typestr[] = 
{
	"SLICE_NIL",
	"SLICE_SINGLE",
	"SLICE_SINGLE_SEQ",
	"SLICE_DOUBLE",
	"SLICE_DOUBLE_SEQ",
	"SLICE_TRIPLE",
	"SLICE_TRIPLE_SINGLE",
	"SLICE_TRIPLE_COUPLE",
	"SLICE_TRIPLE_SEQ",
	"SLICE_PLANE_WING",
	"SLICE_PLANE_WING2",
	"SLICE_BOMB",
	"SLICE_QUAD_2",
	"SLICE_QUAD_4",
};

int LandLordRoom::FindRoleidDirection(int roleid)
{
	std::map<int, GameActor *>::iterator it = _actors.find(roleid);
	if (it == _actors.end())
	{
		LOG_TRACE("Warning: direction not exist. roleid=%d", roleid);
		return -1;
	}
	else
		return it->second->direction;
}

int LandLordRoom::FindDirectionRoleid(char direct)
{
	DeskSeat::const_iterator it = _seats.find(direct);
	if (it == _seats.end())
	{
		LOG_TRACE("Warning: direction not exist. direction=%d", direct);
		return -1;
	}
	else 
		return it->second.roleid;
}

GameActor * LandLordRoom::FindDirectionActor(char direct)
{
	DeskSeat::const_iterator it = _seats.find(direct);
	if (it == _seats.end())
	{
		LOG_TRACE("Warning: direction not exist. direction=%d", direct);
		return NULL;
	}
	else 
	{
		ActorMapIt ait = _actors.find(it->second.roleid);
		if (ait == _actors.end())
			return NULL;
		else
			return ait->second;
	}
}

CardSlice * LandLordRoom::CreateSlice(const CARD_VEC & cards)
{
	static CardSlice tmp(SLICE_NIL,0,0);

	CARD_VEC values;
	LordHelper::IndexToValues(cards, values);

	std::map<char, std::set<char> > card_pairs; // <key, value> == <number, card_values>
	LordHelper::ValuesToPairs(values, card_pairs);

	if (card_pairs.rbegin() == card_pairs.rend())
		return NULL;

	char typebegin = card_pairs.rbegin()->first;
	std::set<char> & cards4 = card_pairs[4];
	std::set<char> & cards3 = card_pairs[3];
	std::set<char> & cards2 = card_pairs[2];
	std::set<char> & cards1 = card_pairs[1];

	switch(typebegin)
	{
	case 4:
		{
			if (cards4.size() == 1)
			{
				if (cards1.empty() && cards2.empty() && cards3.empty())
					return &tmp.SetType(SLICE_BOMB, *cards4.begin(), 0);
				else if (cards.size()== 6)
					return &tmp.SetType(SLICE_QUAD_2, *cards4.begin(), 0);
				else if (cards.size()==8 && cards2.size()==2)
					return &tmp.SetType(SLICE_QUAD_4, *cards4.begin(), 0);
				else
				{
					//处理333444+3+8情况
					cards3.insert(*cards4.begin());
					cards1.insert(*cards4.begin());
					if (cards3.size() >= 2 && LordHelper::IsSequence(cards3.begin(), cards3.end()))
					{
						if (cards1.size()==cards3.size() && cards2.size()==0)
							return &tmp.SetType(SLICE_PLANE_WING, *cards3.begin(), cards3.size());
					}
				}
			}
			return NULL;
		}break;
	case 3:
		{
/*
			if (!cards1.empty())
				return NULL;
*/
			if (cards3.size() == 1)	
			{
				if (cards2.size()==0 && cards1.size()==0)
					return &tmp.SetType(SLICE_TRIPLE, *cards3.begin(), 0);
				else if (cards2.size()==0 && cards1.size()==1)
					return &tmp.SetType(SLICE_TRIPLE_SINGLE, *cards3.begin(), 0);
				else if (cards2.size()==1 && cards1.size()==0)
					return &tmp.SetType(SLICE_TRIPLE_DOUBLE, *cards3.begin(), 0);
				else
					return NULL;
/*
				switch(
				{
				case 0:
					return &tmp.SetType(SLICE_TRIPLE, *cards3.begin(), 0);
					break;
				case 1:
					return &tmp.SetType(SLICE_TRIPLE_DOUBLE, *cards3.begin(), 0);
					break;
				}
				return NULL;
*/
			}

			if (cards3.size() >= 2 && LordHelper::IsSequence(cards3.begin(), cards3.end()))
			{
				if (cards2.empty() && cards1.empty())
					return &tmp.SetType(SLICE_TRIPLE_SEQ, *cards3.begin(), cards3.size());
				else if (cards1.size()==cards3.size() && cards2.size()==0)
					return &tmp.SetType(SLICE_PLANE_WING, *cards3.begin(), cards3.size());
				else if (cards2.size()==cards3.size() && cards1.size()==0)
					return &tmp.SetType(SLICE_PLANE_WING2, *cards3.begin(), cards3.size());
				else
					return NULL;
			}
			return NULL;
		}break;
	case 2:
		{
			if (!cards1.empty())	
				return NULL;

			if (cards2.size() == 1)
				return &tmp.SetType(SLICE_DOUBLE, *cards2.begin(), 0);

			if ( cards2.size() >= 3 && LordHelper::IsSequence( cards2.begin(), cards2.end() ) )	
				return &tmp.SetType(SLICE_DOUBLE_SEQ, *cards2.begin(), cards2.size());

			return NULL;
		}break;
	case 1:
		{
			if (cards1.size() == 1)
				return &tmp.SetType(SLICE_SINGLE, *cards1.begin(), 0);

			if (cards1.size() == 2)
			{
				std::set<char>::iterator it = cards1.begin();
				if (14 == *it && 15 == *(++it))
					return &tmp.SetType(SLICE_BOMB, 15, 0);
			}

			if ( cards1.size() >= 5 && LordHelper::IsSequence( cards1.begin(), cards1.end() ) )
				return &tmp.SetType(SLICE_SINGLE_SEQ, *cards1.begin(), cards1.size());
		
			return NULL;
		}break;
	default:
		break;
	}

	return NULL;
}

bool LandLordRoom::PlayerAuction(char aucseat, char fightscore)
{
	if (fightscore <= _score || fightscore > LANDLORD_ROOM_SCORE_MAX)
		return false;

	/*
	if (aucseat == _lordseat) // 与自己挣地主
		return false;
		*/

	_lordseat = aucseat;
	_score = fightscore;

	LOG_TRACE("LandLordRoom: PlayerAuction done. roomid=%d,lordseat=%d,score=%d", _roomid, _lordseat, _score);
	return true;
}

//override interfaces of the base class
bool LandLordRoom::OnPlayerOnline(int roleid)
{
	using namespace FUNGAME;
	using namespace FUNGAME::S2C;
	FUNGAME::fun_game_packet_wrapper packet;
	{
		using namespace FUNGAME::S2C::INFO;
		landlord_reonline_info info;		
		{
			info.score = _score;
			info.lord_dir = _lordseat;
			info.desk_dir = _deskseat;

			int nextroleid = GetCurActionRoleid();
			char next_direction = FindRoleidDirection(nextroleid);
			info.next_dir = next_direction; 

			char self_dir = FindRoleidDirection(roleid);
			std::vector<char> other_dir;
			for (DeskSeat::const_iterator it = _seats.begin(), ie = _seats.end(); it != ie; ++it)
			{
				if (it->first == self_dir)
					continue;
				else
					other_dir.push_back(it->first);
			}

			//other_dir.resize(2); // 正常情况下不需，安全起见
			info.cards[0].direct = other_dir[0];
			info.cards[0].card_size = _seats[other_dir[0]].cards.size();
			info.cards[1].direct = other_dir[1];
			info.cards[1].card_size = _seats[other_dir[1]].cards.size();

			info.selfcards = _seats[self_dir].cards;
			info.deskcards = _desktop_cards;
		}

		CMD::Make<CMD::landlord_reonline_datasend>::From(packet, GetGameType(), this, info);
		SendPlayerData(roleid, (char*)packet.data(), packet.size());
	}
	return true;
}

bool LandLordRoom::OnGameBegin()
{
	InitCards();
	_process_stage = PROCESS_STAGE_AUCTION;

	LOG_TRACE("LandLordRoom: OnGameBegin done. roomid=%d,lordseat=%d,score=%d", _roomid, _lordseat, _score);
	return true;
}

bool LandLordRoom::OnGameEnd()
{
	using namespace FUNGAME;
	using namespace FUNGAME::S2C;
	FUNGAME::fun_game_packet_wrapper packet;
	{
		if (_winner == -1) //有人掉线或者强退
		{
			int roleid = FindDirectionRoleid(_offliner);
			CMD::Make<CMD::room_game_end>::From(packet, GetGameType(), _roomid, roleid, END_OFFLINE);
			MultiBroadCast((char*)packet.data(), packet.size());
		}
		else
		{
			int roleid = FindDirectionRoleid(_winner);
			CMD::Make<CMD::room_game_end>::From(packet, GetGameType(), _roomid, roleid, END_WINNER);
			MultiBroadCast((char*)packet.data(), packet.size());
		}
	}

	LOG_TRACE("LandLordRoom: OnGameEnd. roomeid=%d,lordseat=%d,score=%d", _roomid, _lordseat, _score);
	CountResult();
	DeskReset();
	return true;
}
void LandLordRoom::OnAddPlayer(int roleid, char direction)
{
	_seats[direction] = SeatInfo(roleid, 0);
	LOG_TRACE("LandLordRoom: AddPlayer. roomid=%d,roleid=%d,direction=%d", _roomid, roleid, direction);
}

void LandLordRoom::PreDeletePlayer(int roleid)
{
	char direct = FindRoleidDirection(roleid);
	LOG_TRACE("LandLordRoom: PreDeletePlayer. roomid=%d,roleid=%d,direction=%d", _roomid, roleid, direct);
	if (direct == -1)
		return;
	_offliner = direct;
}

void LandLordRoom::OnDeletePlayer(int roleid)
{
	char direct = FindRoleidDirection(roleid);
	LOG_TRACE("LandLordRoom: DeletePlayer. roomid=%d,roleid=%d,direction=%d", _roomid, roleid, direct);
	if (direct == -1)
		return;
//	if (gameing)
//		_offliner = direct;
//	else
		_seats.erase(direct);
}

bool LandLordRoom::OnPlayerActTimeout(int roleid, FUNGAME::fun_game_packet_wrapper & cmd)
{
	char direct = FindRoleidDirection(roleid);
	LOG_TRACE("LandLordRoom: PlayerActTimeout. roomid=%d,roleid=%d,direction=%d", _roomid, roleid, direct);

	char next = (direct + 1) % 3;

	if (_process_stage==PROCESS_STAGE_AUCTION)
	{
		if (next == _init_lordseat)
		{
			BeginCardPlay();
			TurnToSeat(_lordseat);
		}
		else
		{
			int nextrole = TurnToSeat(next);
			FUNGAME::fun_game_packet_wrapper packet;
			using namespace FUNGAME::S2C;
			CMD::Make<CMD::player_giveup_normal_card>::From(packet, GetGameType(), _roomid, roleid, nextrole);
			MultiBroadCast((char*)packet.data(), packet.size());
		}
	}
	else 
	{
		if (direct == _deskseat || _deskseat == -1)
		{
			//自动出牌
			using namespace FUNGAME::C2S;
			CARD_VEC auto_choose;
			if (_seats[direct].cards.size())
				auto_choose.push_back(_seats[direct].cards.front());
			CMD::Make<CMD::player_throw_normal_card>::From(cmd, GetGameType(), _roomid, auto_choose);
		}
		else
		{
			int nextrole = TurnToSeat(next);
			FUNGAME::fun_game_packet_wrapper packet;
			using namespace FUNGAME::S2C;
			CMD::Make<CMD::player_giveup_normal_card>::From(packet, GetGameType(), _roomid, roleid, nextrole);
			MultiBroadCast((char*)packet.data(), packet.size());
		}
	}
	return true;
}

bool LandLordRoom::OnPlayerOfflineTimeout(int roleid)
{
	char direct = FindRoleidDirection(roleid);
	LOG_TRACE("LandLordRoom: PlayerOfflineTimeout. roomid=%d,roleid=%d,direction=%d", _roomid, roleid, direct);
	if (direct == -1)
		return false;
	
	_offliner = direct;
	return true;
}

char LandLordRoom::BeginCardPlay()
{
	_process_stage = PROCESS_STAGE_PLAY; 	
	if (_score <= 0)
		_score = 1;

	// 地主拿底牌
	LordHelper::AddCards(_seats[_lordseat].cards, _bottom_cards);

	using namespace FUNGAME;
	FUNGAME::fun_game_packet_wrapper packet;
	{
		int roleid = FindDirectionRoleid(_lordseat);
		using namespace FUNGAME::S2C;
		CMD::Make<CMD::player_pickup_normal_card>::From(packet, GetGameType(), _roomid, roleid, _bottom_cards);
		MultiBroadCast((char*)packet.data(), packet.size());
	}

	LOG_TRACE("LandLorRoom: BeginCardPlay. roomid=%d,lord_seat=%d,score=%d", _roomid, _lordseat, _score);
	return _lordseat;
}

void LandLordRoom::DeskReset()
{
	_init_lordseat = -1;
	_lordseat = -1;
	_deskseat = -1;
	_score = 0;
	_process_stage = PROCESS_STAGE_INVALID;
	_winner = -1;
	_offliner = -1;
	_desktop.SetNil();
	_desktop_cards.clear();
	_bottom_cards.clear();

	DeskSeat::iterator it = _seats.begin(), ie = _seats.end();
	for (; it != ie; ++it)
		it->second.cards.clear();

	LOG_TRACE("LandLordRoom: DeskRest. roomid=%d", _roomid);
}

void LandLordRoom::CountResult()
{
//here
	std::vector<char> farmers;
	farmers.reserve(2);
	for (char i = 0; i<=2; ++i)
	{
		if(i != _lordseat)	
			farmers.push_back(i);
	}

	std::vector<std::pair<int, int> > results;
	if (_offliner != -1)
	{
		/* 玩家掉线或者强退*/	
		int lost = 0;
		if (_score == 0)
			lost = -2; //叫牌前逃跑扣2分
		else
		{
			lost = _score * -2;
			if (_offliner == _lordseat)
				lost *= 2;
		}
//		_seats[_offliner].pocket += lost;
		GameActor * act = FindDirectionActor(_offliner);
		if (act != NULL)
		{
			act->ChangeScore(lost);
			act->EscapeInc();
			results.push_back(std::make_pair(FindDirectionRoleid(_offliner), lost));
			LOG_TRACE("LandLordRoom: CountResult. roomid=%d,offliner=%d,lost=%d", _roomid, _offliner, lost);
		}
	}
	else
	{
		/* 游戏正常结束*/	
		//farmers.resize(2); // 正常情况下不需，安全起见
		bool perfect_win = true;
		int  bonus = _score;
		int  flag = 0;  //标记地主输赢: win:flag = 1;  lose:flag = -1;
		if (_winner == _lordseat)
		{
			flag = 1;
			if (_seats[farmers[0]].throw_times > 0 || _seats[farmers[1]].throw_times > 0)
				perfect_win = false;
		}
		else
		{
			flag = -1;
			if(_seats[_lordseat].throw_times > 1)
				perfect_win = false;
		}
		if (perfect_win) bonus *= 2;
/*
		_seats[farmers[0]].pocket = 0;
		_seats[farmers[1]].pocket = 0; 
		_seats[_lordseat].pocket  = 0;
		
		_seats[farmers[0]].pocket -= (flag)* bonus;
		_seats[farmers[1]].pocket -= (flag)* bonus;
		_seats[_lordseat].pocket  += (flag)* bonus * 2;
*/
		int delta = 0;
		GameActor * act = FindDirectionActor(farmers[0]);
		if (act != NULL)
		{
			delta = (-1)*flag*bonus;
			act->ChangeScore(delta);
			if (delta >= 0)
				act->WinInc();
			else
				act->LoseInc();
			results.push_back(std::make_pair(FindDirectionRoleid(farmers[0]), delta));
			LOG_TRACE("LandLordRoom: CountResult. roomid=%d,dir=%d,score=%d", _roomid, farmers[0], delta);
		}
		act = FindDirectionActor(farmers[1]);
		if (act != NULL)
		{
			delta = (-1)*flag*bonus;
			act->ChangeScore(delta);
			if (delta >= 0)
				act->WinInc();
			else
				act->LoseInc();
			results.push_back(std::make_pair(FindDirectionRoleid(farmers[1]), delta));
			LOG_TRACE("LandLordRoom: CountResult. roomid=%d,dir=%d,score=%d", _roomid, farmers[1], delta);
		}
		act = FindDirectionActor(_lordseat);
		if (act != NULL)
		{
			delta = flag*bonus*2;
			act->ChangeScore(delta);
			if (delta >= 0)
				act->WinInc();
			else
				act->LoseInc();
			results.push_back(std::make_pair(FindDirectionRoleid(_lordseat), delta));
			LOG_TRACE("LandLordRoom: CountResult. roomid=%d,dir=%d,score=%d", _roomid, _lordseat, delta);
		}
	}
/*
	for (char i = 0; i<=2; ++i)
		results.push_back(std::make_pair(_seats[i].roleid, _seats[i].pocket));
*/
	using namespace FUNGAME;
	FUNGAME::fun_game_packet_wrapper packet;
	{
		using namespace FUNGAME::S2C;
		CMD::Make<CMD::landlord_result_broadcast>::From(packet, GetGameType(), _roomid, results.size(), results);
/*
		LOG_TRACE("LandlordRoom:count result.role1=%d,count=%d",results[0].first,results[0].second);
		LOG_TRACE("LandlordRoom:count result.role2=%d,count=%d",results[1].first,results[1].second);
		LOG_TRACE("LandlordRoom:count result.role3=%d,count=%d",results[2].first,results[2].second);
*/
		MultiBroadCast((char*)packet.data(), packet.size());
	}

}

void LandLordRoom::GameWin(char direct)
{
	_winner = direct;
	
	SetGameEnd(true);
}

bool LandLordRoom::IsPlayerHasCards(char direct, const CARD_VEC &cards)
{
	if (!CheckDirection(direct))
		return false;

	return LordHelper::IsSubSeq(_seats[direct].cards, cards);
}

void LandLordRoom::InitCards()
{
	const CARD_VEC & total = LordHelper::ShuffleCards();

	_seats[0].cards.assign(total.begin(), total.begin() + 17);
	LordHelper::PrintCards(0, _seats[0].cards);
	_seats[1].cards.assign(total.begin() + 17, total.begin() + 34);
	LordHelper::PrintCards(1, _seats[1].cards);
	_seats[2].cards.assign(total.begin() + 34, total.begin() + 51);
	LordHelper::PrintCards(2, _seats[2].cards);
	_bottom_cards.assign(total.begin() + 51, total.end());

	LordHelper::Sort(_seats[0].cards);
	LordHelper::Sort(_seats[1].cards);
	LordHelper::Sort(_seats[2].cards);

	//底牌是否需要排序？
	LordHelper::Sort(_bottom_cards);

	_score = 0;
	_init_lordseat = _lordseat = Timer::GetTime() % 3;
	int nextrole = TurnToSeat(_lordseat);

	using namespace FUNGAME;
	for (size_t i = 0; i <= 2; ++i)
	{
		FUNGAME::fun_game_packet_wrapper packet;
		{
			int roleid = FindDirectionRoleid(i);
			using namespace FUNGAME::S2C;
			CMD::Make<CMD::room_dispatch_normal_card>::From(packet, GetGameType(), _roomid, roleid, nextrole, _seats[i].cards);
			SendPlayerData(roleid, (char*)packet.data(), packet.size());
		}
	}
}

bool LandLordRoom::PlayerThrowCard(char direct, const CARD_VEC &cards)
{
	if (!IsPlayerHasCards(direct, cards))
		return false;

	//确定玩家出牌的牌型
	CardSlice * slice = CreateSlice(cards);
	if (slice != NULL)
	{
		LOG_TRACE("LandLordRoom: PlayerThrowCard. roomid=%d, slice_type=%s", _roomid, lord_typestr[slice->GetType()]);
		LordHelper::PrintCards(direct, cards);
		if (!(_desktop < slice))	
			return false;

		_desktop = *slice;
		_desktop_cards = cards;
		_deskseat = direct;

		//炸弹, 积分翻倍
		if (slice->GetType() == SLICE_BOMB)
			_score *= 2;

		LordHelper::RemoveSliceCards(_seats[direct].cards, cards);
		LOG_TRACE("LandLordRoom: After throw");
		LordHelper::PrintCards(direct, _seats[direct].cards);
		_seats[direct].throw_times++;

		return true;
	}

	return false;;
}

bool LandLordRoom::OnGameAction(int roleid, char action, char* data, unsigned int size) 
{ 
	using namespace FUNGAME;
	unsigned char cmd_type = ((const C2S::fun_game_c2s_cmd_header*)data) -> cmd_type;

	char direct = FindRoleidDirection(roleid);
	if (!CheckDirection(direct))
		return false;

	LOG_TRACE("LandLordRoom: OnGameAction. roomid=%d, roleid=%d, direction=%d, cmd_type=%d", _roomid, roleid, direct, cmd_type);
	switch(_process_stage)
	{
	case PROCESS_STAGE_AUCTION:
		{
			if (cmd_type == C2S::PLAYER_AUCTION)
			{
				C2S::CMD::player_auction &cmd = *(C2S::CMD::player_auction *)data;
				if (size != sizeof(cmd)) return false;

				char next = (direct + 1) % 3;
				if (PlayerAuction(direct, cmd.score))
				{
					if (_score == LANDLORD_ROOM_SCORE_MAX || next == _init_lordseat)	
					{
						BeginCardPlay();
						TurnToSeat(_lordseat);
					}
					else
					{
						int nextrole = TurnToSeat(next);

						FUNGAME::fun_game_packet_wrapper packet;
						using namespace FUNGAME::S2C;
						CMD::Make<CMD::player_auction>::From(packet, GetGameType(), _roomid, roleid, nextrole, _score);
						MultiBroadCast((char*)packet.data(), packet.size());
					}

					return true;
				}
				return false;
			}
			else if (cmd_type == C2S::PLAYER_GIVEUP_NORMAL_CARD)
			{
				C2S::CMD::player_giveup_normal_card &cmd = *(C2S::CMD::player_giveup_normal_card *)data;
				if (size != sizeof(cmd)) return false;

				char next = (direct + 1) % 3;
				//其他玩家选择放弃，当前叫牌玩家当选地主
				if (next == _init_lordseat)
				{
					BeginCardPlay();
					TurnToSeat(_lordseat);
				}
				else
				{
					int nextrole = TurnToSeat(next);

					FUNGAME::fun_game_packet_wrapper packet;
					using namespace FUNGAME::S2C;
					CMD::Make<CMD::player_giveup_normal_card>::From(packet, GetGameType(), _roomid, roleid, nextrole);
					MultiBroadCast((char*)packet.data(), packet.size());
				}
				
				return true;
			}

			return false;	
		}break;
	case PROCESS_STAGE_PLAY:
		{
			if (cmd_type == C2S::PLAYER_THROW_NORMAL_CARD)
			{
				C2S::CMD::player_throw_normal_card &cmd = *(C2S::CMD::player_throw_normal_card *)data;
				char header = sizeof(C2S::fun_game_c2s_cmd_header) + sizeof(short);
				char *cardsize = (char*) data + header;
				if (*cardsize > 20) return false;
				if (*cardsize != (char)(size - header - sizeof(char))) return false;
				
				//必须对牌进行排序
				CARD_VEC cards(cmd.cards.cards, cmd.cards.cards + cmd.cards.card_size);
				LordHelper::Sort(cards);

				if (!LordHelper::IsSequenceValid(cards))
				{
					LOG_TRACE("WARNING: index out of range!");
					return false;
				}
				
				if (PlayerThrowCard(direct, cards))
				{
					char next = (direct + 1) % 3;
					int nextrole = TurnToSeat(next);

					FUNGAME::fun_game_packet_wrapper packet;
					using namespace FUNGAME::S2C;
					CMD::Make<CMD::player_throw_normal_card>::From(packet, GetGameType(), _roomid, roleid, nextrole, cards);
					MultiBroadCast((char*)packet.data(), packet.size());

					//牌出净
					if (_seats[direct].cards.empty())
					{
						GameWin(direct);
					}	
					return true;
				}
				return false;
			}
			else if (cmd_type == C2S::PLAYER_GIVEUP_NORMAL_CARD)
			{
				C2S::CMD::player_giveup_normal_card &cmd = *(C2S::CMD::player_giveup_normal_card *)data;
				if (size != sizeof(cmd)) return false;

				//某些情况下不能不出牌
				if (_deskseat == -1 || _deskseat == direct) return false;
				char next = (direct + 1) % 3;
				int nextrole = TurnToSeat(next);

				FUNGAME::fun_game_packet_wrapper packet;
				using namespace FUNGAME::S2C;
				CMD::Make<CMD::player_giveup_normal_card>::From(packet, GetGameType(), _roomid, roleid, nextrole);
				MultiBroadCast((char*)packet.data(), packet.size());

				return true;
			}
		
			return false;
		}break;
	default:
		break;
	}
	return false;
}

};


