#ifndef __GNET_LANDLORDGAME_H
#define __GNET_LANDLORDGAME_H

#include <set>
#include <map>
#include "fungamebase.h"
#include "fungameprotocol.h"
#include "fungameprotocol_imp.h"
#include "fungamepacket.h"


namespace GNET
{

extern char lord_card_values[55];
extern char *lord_typestr[];
enum CARD_SLICE_TYPE
{
	SLICE_NIL,
	SLICE_SINGLE,
	SLICE_SINGLE_SEQ,
	SLICE_DOUBLE,
	SLICE_DOUBLE_SEQ,
	SLICE_TRIPLE,
	SLICE_TRIPLE_SINGLE,
	SLICE_TRIPLE_DOUBLE,
	SLICE_TRIPLE_SEQ,
	SLICE_PLANE_WING,//3顺+同数量单牌
	SLICE_PLANE_WING2,//3顺+同数量对牌
	SLICE_BOMB,
	SLICE_QUAD_2,  //四张带2张单 或一对
	SLICE_QUAD_4,	//四张带2对
};

class CardSlice
{
public:
	CardSlice(char type, char value, char seq_len) 
		: _type(type), _value(value), _seq_len(seq_len) 
	{ }
	CardSlice(const CardSlice &rhs) 
		: _type(rhs._type), _value(rhs._value), _seq_len(rhs._seq_len) 
	{ }

	void SetNil() { _type = SLICE_NIL; }
	int GetType() const { return _type; }
	int SeqLength() const { return _seq_len; }
	virtual bool operator<(CardSlice *rhs) const 
	{
		if (_type == SLICE_NIL)
			return true;

		if (rhs->_type == SLICE_BOMB)
			return (_type == SLICE_BOMB) ? _value < rhs->_value : true;
		
		if (_type != rhs->_type)
			return false;

		if (_seq_len > 1 && _seq_len != rhs->_seq_len)
			return false;

		return _value < rhs->_value; 
	}
	virtual void Dump(std::string prefix) const 
	{ 
		std::cout << prefix << ":" 
			<< "-type:" << lord_typestr[(size_t)_type] 
			<< "-value:" << (int)_value 
			<< "-seqlen:" << (int)_seq_len << std::endl;
	}
	CardSlice & SetType(char type, char value, char seq_len)
	{
		_type = type;
		_value = value;
		_seq_len = seq_len;
		return *this;
	}

private:
	char _type;
	char _value;
	char _seq_len;
};

typedef std::vector<char> CARD_VEC;
class LordHelper
{
public:
	template<typename Iter>
	static bool IsSequence(Iter begin, Iter end)
	{
		Iter oldend = end;
		if (begin == end || *(--end) >= 13) // 合法的顺子不应该包括"2"和大小王
			return false;
		end = oldend;

		Iter pre = begin++;	
		while(begin != end)
		{
			if (*begin - *pre != 1)	
				return false;
			pre = begin++;
		}
		return true;
	}
	static bool ValidIndex(char index) { return index >=1 && index <=54; }

	static const CARD_VEC & ShuffleCards()
	{
		static CARD_VEC total;	
		if (total.empty())
		{
			for (char i = 1; i <= 54; ++i)
				total.push_back(i);
			srand((int)time(0));
		}
		random_shuffle(total.begin(), total.end());
		return total;
	}

	static void Sort(CARD_VEC & cards)
	{
		std::sort(cards.begin(), cards.end());
	}
	static void AddCards(CARD_VEC &to, const CARD_VEC &from)
	{
		CARD_VEC::const_iterator it = from.begin(), ie = from.end();
		for (; it != ie; ++it)
			to.push_back(*it);

		LordHelper::Sort(to);
	}
	static void IndexToValues(const CARD_VEC &index, CARD_VEC &values)
	{
		values.clear();
		for (CARD_VEC::const_iterator it = index.begin(), ie = index.end(); it != ie; ++it)
		{
			char value = lord_card_values[(size_t)(*it)];
			if(value & 0x80)
				values.push_back(15);
			else if(value & 0x40)
				values.push_back(14);
			else 
				//为了方便判断顺子的合法性，将"3,4,5...J,Q,K,A,2" 调整为"1,2,3...9,10,11,12,13"
				values.push_back( ((value & 0x0f) + 10) % 13 + 1);
		}
	}
	static void ValuesToPairs(const CARD_VEC &values, std::map<char, std::set<char> > &card_pairs)
	{
		card_pairs.clear();
		CARD_VEC::const_iterator it = values.begin(), ie = values.end();
		while(it != ie)
		{
			CARD_VEC::const_iterator tmpe = std::upper_bound(it, ie, *it); 
			card_pairs[std::distance(it, tmpe)].insert(*it);
			it = tmpe;
		}
	}
	static void SliceInput(CARD_VEC & slice)
	{
		std::istream_iterator<int> inpos(std::cin);
		std::istream_iterator<int> endpos;
		while(inpos != endpos)
		{
			int value = *inpos;
			if (value == 100)
				break;
			slice.push_back(value);
			++inpos;
		}

		std::cout << std::endl;
	}
	static bool RemoveSliceCards(CARD_VEC &src, const CARD_VEC &dst)
	{
		CARD_VEC::iterator sit = src.begin(), sie = src.end();
		CARD_VEC::const_iterator dit = dst.begin(), die = dst.end();

		while(sit != sie)
		{
			if (*dit < *sit)	
				return false;
			else if (*dit > *sit)
				++sit;
			else
			{
				*sit = 0; //标记删除
				++sit;
				++dit;
			}

			if (dit == die)
			{
				src.erase(std::remove(src.begin(), src.end(), 0), src.end());
				return true;
			}
		}
		return false;
	}
	static bool IsSubSeq(const CARD_VEC &src, const CARD_VEC &dst)
	{
		CARD_VEC::const_iterator sit = src.begin(), sie = src.end();
		CARD_VEC::const_iterator dit = dst.begin(), die = dst.end();
		while(sit != sie)
		{
			if (*dit < *sit)
				return false;
			else if (*dit > *sit)
				++sit;
			else
			{
				++sit;
				++dit;
			}

			if (dit == die)	
				return true;	
		}
		return false;
	}
	static void SeqDump(const CARD_VEC &seq, std::string prefix)
	{
		std::cout << std::endl << prefix << ":";

		std::ostream_iterator<int> oit(std::cout, " ");
		std::copy(seq.begin(), seq.end(), oit);

		std::cout << std::endl;
	}
	static bool IsSequenceValid(const CARD_VEC & cards) 
	{
		return cards.front() >=1 && cards.back() <= 54;
	}
	static void PrintCards(char dir, const CARD_VEC & cards)
	{
		fprintf(stderr, "print cards dir[%d]size[%d] ", dir, cards.size());
		CARD_VEC::const_iterator it = cards.begin(), ite = cards.end();
		for (;it != ite; ++it)
			fprintf(stderr, " %d", *it);
		fprintf(stderr, "\n");
	}
};

struct SeatInfo
{
	SeatInfo() 
		: roleid(-1), pocket(0), throw_times(0)
	{}
	SeatInfo(int _roleid, int _pocket) 
		: roleid(_roleid), pocket(_pocket), throw_times(0)
	{}
	SeatInfo(const SeatInfo &rhs) 
		: roleid(rhs.roleid), pocket(rhs.pocket), throw_times(rhs.throw_times), cards(rhs.cards)
	{}

	SeatInfo & operator=(const SeatInfo &rhs)
	{
		roleid = rhs.roleid;
		pocket = rhs.pocket;
		throw_times = rhs.throw_times;
		cards  = rhs.cards;
		return *this;
	}

	int roleid;
	int pocket; 	   //筹码
	int throw_times;   //自游戏开始来的出牌次数
	CARD_VEC cards;
};

class LandLordRoom : public Room
{
	friend class LandLordRoomHelper;
private:
	int FindRoleidDirection(int roleid);
	int FindDirectionRoleid(char direct);
	GameActor * FindDirectionActor(char direct);
	int TurnToSeat(char direct)
	{
		int nextrole = _seats[direct].roleid;
		SetCurActionRoleid(nextrole);
		SetTimeOut(TIMEOUT_TYPE_ACT, Timer::GetTime()+TIMEOUT_ACT, nextrole);

		if (direct == _deskseat)
		{
			_desktop.SetNil();
			//?? need to clear ??
			//_desktop_cards.clear(); 
		}
		return nextrole;
	}
public:
	//需要覆盖的基类接口
	virtual unsigned char GetGameType() { return GAME_TYPE_LANDLORD; }
	virtual unsigned char GetRoomPlayerLimit() { return LANDLORD_ROOM_PLAYERS; }
	virtual bool CheckDirection(char direction) 
	{ 
		if (direction >= 0 && direction <= 2)
			return true;
		else
		{
			LOG_TRACE("Warning: Invalid direction, direct=%d", direction);
			return false;
		}
	}

	virtual bool OnGameReady() { DeskReset(); return true; }
	virtual bool OnGameBegin();
	virtual bool OnGameEnd();
	virtual bool OnGameAction(int roleid, char action, char* data, unsigned int size);

	virtual bool OnPlayerOnline(int roleid);
	virtual bool OnPlayerOffline(int roleid) {return true; }

	bool OnPlayerActTimeout(int roleid, FUNGAME::fun_game_packet_wrapper & cmd);
	virtual bool OnPlayerOfflineTimeout(int roleid);

	virtual void OnAddPlayer(int roleid, char direction);
	virtual void OnDeletePlayer(int roleid);
	virtual void PreDeletePlayer(int roleid);
public:
	enum PARAS
	{
		LANDLORD_GAME_TYPE = 1,
		LANDLORD_ROOM_PLAYERS = 3,	
		LANDLORD_ROOM_SCORE_MAX = 3,
		TIMEOUT_ACT = 30,

		PROCESS_STAGE_INVALID = 0,
		PROCESS_STAGE_AUCTION = 1,
		PROCESS_STAGE_PLAY = 2,

		SEAT_DIRECTION_FIRST = 1,
		SEAT_DIRECTION_SECOND = 2,
		SEAT_DIRECTION_THIRD = 3,

		END_WINNER  = 0,
		END_OFFLINE = 1,
	};


	LandLordRoom(short roomid) : Room(roomid), _init_lordseat(-1), _lordseat(-1), _deskseat(-1), _score(0), 
		_process_stage(PROCESS_STAGE_INVALID), _winner(-1), _offliner(-1), _desktop(SLICE_NIL, 0, 0) 
	{ }

	bool PlayerAuction(char lordseat, char fightscore);
	char BeginCardPlay();
	void GameWin(char direct);
	void CountResult();
	void DeskReset();
	bool PlayerThrowCard(char direct, const CARD_VEC &cards);
	bool IsPlayerHasCards(char direct, const CARD_VEC &cards);

	void InitCards();

	// 确定牌型：需要先对cards进行排序
	static CardSlice * CreateSlice(const CARD_VEC &cards);
private:
	char _init_lordseat;//最初随机选择的地主位
	char _lordseat;  //地主位
	char _deskseat;  //桌面牌拥有者
	char _score; 	 //低分 
	char _process_stage; //游戏阶段
	char _winner; 	 
	char _offliner;

	typedef std::map<char/*direction*/, SeatInfo > DeskSeat;
	DeskSeat _seats; 
	CardSlice _desktop; //桌面牌的牌型
	CARD_VEC  _desktop_cards; //桌面牌内容
	CARD_VEC  _bottom_cards;  //底牌内容
};


};
#endif

