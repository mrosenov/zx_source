#ifndef __GNET_CONTEST_H
#define __GNET_CONTEST_H

#include <map>
#include <list>
#include "thread.h" 
#include "itimer.h" 
#include "questionlib.h" 
#include "contestquestion.hpp" 
#include "contestanswer_re.hpp" 

namespace GNET
{

class BroadcastQuestionTask: public Thread::Runnable
{
public:
	static BroadcastQuestionTask* Instance() { static BroadcastQuestionTask instance; return &instance; }
	virtual void Run();
};

class BroadcastPlaceListTask: public Thread::Runnable
{
public:
	static BroadcastPlaceListTask* Instance() { static BroadcastPlaceListTask instance; return &instance; }
	virtual void Run();
};

class BroadcastAnswerReTask: public Thread::Runnable
{
public:
	static BroadcastAnswerReTask* Instance() { static BroadcastAnswerReTask instance; return &instance; }
	virtual void Run();
};

struct PlaceInfo
{
	int roleid; 
	Octets rolename;
	int score;
};

#define MAX_STAR_COUNT 3
struct PlayerContestInfo
{
	int score;
	int place; //竞赛名次 
	int role_level;  //玩家级别 
	char star_count; //答题星星个数 
	char question_id; //上次回答问题的id, 用于答案的合法性检查 
	char right_amount; //回答对问题的 总数 
	char series_right; //连续回答对问题的个数 
	bool active;      //是否在线

	unsigned int linksid;
	unsigned int localsid;
	int gameid;
};

class ContestManager: public IntervalTimer::Observer 
{
public:
	enum {
		PLACE_LIST_MAX_COUNT = 10,
	};

	enum {
		ANSWER_RIGHT,
		ANSWER_ERROR,
		ANSWER_TIMEOUT,
		ANSWER_ILLEGAL,
	};

	enum {
		STATE_IDLE,
		STATE_INVITE_MSG_1,
		STATE_INVITE_MSG_2,
		STATE_INVITE_MSG_3,
		STATE_BEGIN,
		STATE_FORBID_JOIN,
		STATE_END,
	};

	char InsertPlayer(int roleid, int level, unsigned int linksid, unsigned int localsid, int gameid)
	{
		Thread::RWLock::WRScoped l(_map_locker);
		UidIterator it = _contest_map.find(roleid);
		if(it == _contest_map.end())
		{
			PlayerContestInfo pci;
			pci.score = 0;
			pci.place = 0;
			pci.role_level = level;
			pci.star_count = MAX_STAR_COUNT;
			pci.question_id = -1;
			pci.right_amount = 0;
			pci.series_right = 0;
			pci.active = true;

			pci.linksid = linksid;
			pci.localsid = localsid;
			pci.gameid = gameid;
			_contest_map[roleid] = pci;
			return pci.star_count;
		}
		else
		{
			it->second.linksid = linksid;
			it->second.localsid = localsid;
			it->second.gameid = gameid;
			it->second.active = true;
			return it->second.star_count;
		}
	}
	
	PlayerContestInfo* GetPlayerContestInfo(int roleid)
	{
		UidIterator it = _contest_map.find(roleid);
		if(it != _contest_map.end())
		{
			return &(it->second);
		}

		return NULL;
	}

	bool IsPlayerActive(int roleid)
	{
		Thread::RWLock::WRScoped l(_map_locker);
		UidIterator it = _contest_map.find(roleid);
		if(it != _contest_map.end())
		{
			return it->second.active;
		}

		return false;
	}

	void BroadcastContestMsg(int id, const Octets & msg);
	void BroadcastContestInvite();
	void BroadcastQuestion();
	void BroadcastPlaceList();
	void BroadcastAnswerReCache();
	void BroadcastEndContest();
	void BroadcastResult();
	
	void OnPlayerLogin(int roleid, unsigned int linksid, unsigned int localsid);
	void OnPlayerLogout(int roleid);
 	void OnPlayerExit(int roleid);  

	void OnPlayerAnswer(int roleid, int question_id, int answer, char use_star, Octets& rolename);
	int ComputePlayerScore(int roleid, int question_id, int answer, char& result);
	int ComputeAddonScore(int score, char series_right);
	//计算排名 
	void ChangePlaceList(const int& roleid, const Octets& rolename, const int& score);

	int GetPlayerScore(int roleid)
	{
		Thread::RWLock::RDScoped l(_map_locker);
		UidIterator it = _contest_map.find(roleid);
		if(it != _contest_map.end())
			return (it->second).score; 
		return 0;
	}

	bool Initialize(const std::string filename);
	int GetCurQuestionIndex() { return _cur_question; }
	int GetNextQuestionIndex() { return _next_question; }
	void AddAnswerReCache(unsigned int linksid, ContestAnswer_Re &cas);
	void DeleteAnswerReCache(unsigned int linksid, int roleid);

	bool Update();
	bool IsProcessState() { return (STATE_BEGIN==_state || STATE_FORBID_JOIN==_state) ; }
	bool IsPermitJoinState() { return STATE_BEGIN==_state; }
	bool IsForbidJoinState() { return STATE_FORBID_JOIN==_state; }
	bool IsIdleState() { return STATE_IDLE==_state; }
	void Clear();
	size_t Size() { return _contest_map.size(); }

	typedef std::vector<ContestAnswer_Re>  AnswerReList;
	typedef std::vector<ContestAnswer_Re>::iterator  AnswerReListIt;
	typedef std::map<unsigned int, AnswerReList>  AnswerReCache; //key: linksid
	typedef std::map<unsigned int, AnswerReList>::iterator  AnswerReCacheIt;
	typedef std::map<int, PlayerContestInfo>  UidMap; //key: roleid
	typedef std::map<int, PlayerContestInfo>::iterator  UidIterator;
	static ContestManager& GetInstance() { static ContestManager instance; return instance; }

	ContestManager():_add_task(false),_state(STATE_IDLE),_cur_day(0),_next_question(0),_cur_question(0),_cur_answer(0) { }
	~ContestManager() { Clear(); }

public:
	void DebugBegin(int roleid, int index);
	void DebugEnd(int roleid);

private:
	bool BuildQuestion(int day, int id, ContestQuestion& contestques);

private:
	Thread::RWLock _map_locker;	
	Thread::RWLock _list_locker;	
	UidMap _contest_map; //竞赛参与人
	std::list<PlaceInfo> _place_list; //竞赛排行榜 
	AnswerReCache _answer_re_cache;  //"答案回应"的缓存；为了统一在固定时间发 "答案回应"

	bool _add_task;
	char _state; //竞赛状态 
	int _start_time; //竞赛开始时间(以秒为单位)
	time_t _calander; //日历时间 
	unsigned short _cur_day; //当前是该年中的第几天 

	unsigned char _next_question; //下一个问题索引
	unsigned char _cur_question; //当前问题索引 
	unsigned char _cur_answer; //当前问题答案
	time_t _cur_question_time; //当前问题开始时间 
	CQuestionLib _questionlib;
};

};

#endif

