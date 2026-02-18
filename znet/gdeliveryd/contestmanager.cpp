#include "contestmanager.h" 
#include "localmacro.h" 
#include "timer.h" 
#include "gdeliveryserver.hpp" 
#include "gproviderserver.hpp" 
#include "maplinkserver.h"
#include "gamedbclient.hpp" 
#include "mapuser.h" 
#include "contestinvitebroadcast.hpp" 
#include "contestinvite.hpp" 
#include "contestquestion.hpp" 
#include "contestanswer_re.hpp" 
#include "contestplacenotify.hpp" 
#include "contestend.hpp" 
#include "contestresult.hpp" 
#include "contestbegin.hpp" 
#include "chatbroadcast.hpp" 

namespace GNET
{

#define SECOND_OF_ONE_DAY 86400
//18:29发第一次广播 18:44发第二次广播 18:54发第三次广播 18:59邀请 19:00开始  19:11:30后禁止加入 19:12:40 结束 
#define BROADCAST_MSG1_INTERVAL 1800
#define BROADCAST_MSG2_INTERVAL 900 
#define BROADCAST_MSG3_INTERVAL 300 
#define START_TIME 68340 
#define INVITE_INTERVAL (60)
#define FORBID_JOIN_INTERVAL (750)
#define PERSIST_TIME	(820)

#define ANSWER_QUESTION_LIMIT 20 
#define UPDATE_INTERVAL	25 
#define MAX_SCORE	500

int GetLen(unsigned short* src)
{
	int len = 0;
	for( ; src[len]!=0; ++len);
	return sizeof(unsigned short)*len;
}

void BroadcastQuestionTask::Run()
{
	if(!ContestManager::GetInstance().IsProcessState())
		return;

	ContestManager::GetInstance().BroadcastQuestion();
	Thread::HouseKeeper::AddTimerTask(this, UPDATE_INTERVAL);
}

void BroadcastPlaceListTask::Run()
{
	if(!ContestManager::GetInstance().IsProcessState())
		return;

	ContestManager::GetInstance().BroadcastPlaceList();
	Thread::HouseKeeper::AddTimerTask(this, UPDATE_INTERVAL);	
}

void BroadcastAnswerReTask::Run()
{
	if(!ContestManager::GetInstance().IsProcessState())
		return;

	ContestManager::GetInstance().BroadcastAnswerReCache();
	Thread::HouseKeeper::AddTimerTask(this, UPDATE_INTERVAL);	
}

bool ContestManager::Initialize(const std::string filename) 
{
	struct tm dt;
	time_t now = Timer::GetTime();
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;  
	dt.tm_hour = 0;
	_cur_day = dt.tm_mday;
	_calander = mktime(&dt);
	_start_time = START_TIME;  

	bool init = _questionlib.Load(filename.c_str());
	if(!init)
		return false;
	
	IntervalTimer::Attach( this, 1*1000000/IntervalTimer::Resolution()); //1秒 
	LOG_TRACE( "ContestManager initialized");
	return true;
}

bool ContestManager::Update()
{
	time_t now = Timer::GetTime();
	if(now -_calander > SECOND_OF_ONE_DAY )
	{
	       	_calander += SECOND_OF_ONE_DAY;
		struct tm dt;
		localtime_r(&now, &dt);
		_cur_day = dt.tm_mday;
	}

	now -= _calander;
	int elapse = now - _start_time;
	switch(_state)
	{
		case STATE_IDLE:
		{
			int interval = _start_time - now;
			if(interval>0 && interval<=BROADCAST_MSG1_INTERVAL) 
			{
				interval = BROADCAST_MSG1_INTERVAL;
				Marshal::OctetsStream os;
				os << interval;
				BroadcastContestMsg(MSG_CONTESTTIME, os);
				_state = STATE_INVITE_MSG_1;
			}
		}
			break;
	
		case STATE_INVITE_MSG_1:
		{
			int interval = _start_time - now;
			if(interval<=BROADCAST_MSG2_INTERVAL) 
			{
				interval = BROADCAST_MSG2_INTERVAL;
				Marshal::OctetsStream os;
				os << interval;
				BroadcastContestMsg(MSG_CONTESTTIME, os);
				_state = STATE_INVITE_MSG_2;
			}
		}
			break;

		case STATE_INVITE_MSG_2:
		{
			int interval = _start_time - now;
			if(interval<=BROADCAST_MSG3_INTERVAL) 
			{
				interval = BROADCAST_MSG3_INTERVAL;
				Marshal::OctetsStream os;
				os << interval;
				BroadcastContestMsg(MSG_CONTESTTIME, os);
				_state = STATE_INVITE_MSG_3;
			}
		}
			break;

		case STATE_INVITE_MSG_3:
		{
			if(elapse >= 0 && elapse < INVITE_INTERVAL ) 
			{
				Clear();
				int interval = 60;
				Marshal::OctetsStream os;
				os << interval;
				BroadcastContestMsg(MSG_CONTESTTIME, os);
				BroadcastContestInvite();
				_state = STATE_BEGIN;
			}
		}
			break;
		case STATE_BEGIN:
		{
			if(elapse >= INVITE_INTERVAL && elapse < FORBID_JOIN_INTERVAL) 
			{
				if(!_add_task)	
				{
					_add_task = true;
					Thread::HouseKeeper::AddTimerTask(BroadcastQuestionTask::Instance(), 1);	
					Thread::HouseKeeper::AddTimerTask(BroadcastAnswerReTask::Instance(),1+ANSWER_QUESTION_LIMIT);
					Thread::HouseKeeper::AddTimerTask(BroadcastPlaceListTask::Instance(),1+UPDATE_INTERVAL);
				}

			}
			else if(elapse > FORBID_JOIN_INTERVAL)
			{
				_state = STATE_FORBID_JOIN;
			}
		}
			break;
	
		case STATE_FORBID_JOIN:
		{
			if(elapse > PERSIST_TIME)
			{
				BroadcastResult();
				BroadcastEndContest();
				_state = STATE_END;
			}
		}
			break;
		case STATE_END:
			if(elapse > PERSIST_TIME)
			{
				Marshal::OctetsStream os;
				BroadcastContestMsg(MSG_CONTESTEND, os);
				_state = STATE_IDLE;
				Clear();
			}
			break;
		default:
			break;
	}

	return true;
}

void ContestManager::ChangePlaceList(const int& roleid, const Octets& rolename, const int& score)
{
	Thread::RWLock::WRScoped l(_list_locker);
	std::list<PlaceInfo>::iterator it = _place_list.begin(); 
	for(;it!=_place_list.end();)
	{
		if(it->roleid==roleid)
		{
			it = _place_list.erase(it);
			continue;
		}
		if(score<=it->score)
			break;
		++it;
	}
	if(_place_list.size()<PLACE_LIST_MAX_COUNT || it!=_place_list.begin())
	{
		PlaceInfo pinfo;
		pinfo.roleid = roleid;
		pinfo.rolename = rolename;
		pinfo.score = score;
		_place_list.insert(it, pinfo);
		if(_place_list.size() > PLACE_LIST_MAX_COUNT)
			_place_list.erase(_place_list.begin());
	}

}
void ContestManager::OnPlayerLogout(int roleid)
{
	PlayerContestInfo *pinfo = GetPlayerContestInfo(roleid);
	if(pinfo)
	{
		DeleteAnswerReCache(pinfo->linksid, roleid);

		pinfo->linksid = 0;
		pinfo->localsid = 0;
		pinfo->gameid = 0;
		pinfo->active = false;
		pinfo->series_right = 0;
	}
}
void ContestManager::OnPlayerLogin(int roleid, unsigned int linksid, unsigned int localsid)
{
	if(!IsPermitJoinState())
		return;

	int rtime = PERSIST_TIME + (_start_time + _calander - Timer::GetTime());
	if(rtime < 0)
		return;
	ContestInvite invite(0, 0, 0);
	invite.remain_time = rtime;
	invite.localsid = localsid;
	GDeliveryServer::GetInstance()->Send(linksid, invite);
}

void ContestManager::OnPlayerExit(int roleid)
{
	Thread::RWLock::WRScoped l(_map_locker); 
	UidIterator it = _contest_map.find(roleid);
	if(it!=_contest_map.end())
	{
		(it->second).active = false;
		(it->second).series_right = 0;

		ContestEnd contestend;
		contestend.roleid = roleid;
		contestend.localsid = (it->second).localsid;
		GDeliveryServer::GetInstance()->Send((it->second).linksid, contestend);
	}

}

void ContestManager::OnPlayerAnswer(int roleid, int question_id, int answer, char use_star, Octets& rolename)
{
	Thread::RWLock::WRScoped l(_map_locker);
	PlayerContestInfo *pinfo = GetPlayerContestInfo(roleid);
	if(!pinfo || pinfo->question_id>=question_id)
		return;
	char result;
	int score = ComputePlayerScore(roleid, question_id, answer, result);
	if( result == ANSWER_ILLEGAL)
	{
		pinfo->series_right = 0;
		return;
	}

	pinfo->question_id = question_id;
	if(result == ANSWER_RIGHT)
	{
		pinfo->right_amount += 1;
		pinfo->series_right += 1;
	}
	else
		pinfo->series_right = 0;

	if(use_star && pinfo->star_count>0)
	{
		score *= 2;
		pinfo->star_count -= 1;
	}
	if(score>0)
	{
		score += ComputeAddonScore(score, pinfo->series_right);
		pinfo->score += score;
		ChangePlaceList(roleid, rolename, pinfo->score);
	}

	ContestAnswer_Re car(roleid, question_id, result, score, pinfo->score, 
			pinfo->star_count, pinfo->right_amount, pinfo->series_right, _cur_answer, pinfo->localsid);
	//GDeliveryServer::GetInstance()->Send(pinfo->linksid, car);
	AddAnswerReCache(pinfo->linksid, car);
}

int ContestManager::ComputePlayerScore(int roleid, int question_id, int answer, char& result)
{
	int score = 0;
	if(question_id != _cur_question)
	{
		result = ANSWER_ILLEGAL;
		return score;
	}
	if(answer != _cur_answer )
	{
		result = ANSWER_ERROR;
		return score;
	}

	int elapse = Timer::GetTime() - _cur_question_time;
	if(elapse < 0 || elapse > ANSWER_QUESTION_LIMIT)
	{
		result = ANSWER_ILLEGAL;
		return 0;
	}

	if(elapse < 2)
		score = 10;
	else if(elapse < 3)
		score = 9;
	else if(elapse < 4)
		score = 8;
	else if(elapse < 5)
		score = 7;
	else if(elapse < 6)
		score = 6;
	else if(elapse < 8)
		score = 5;
	else if(elapse < 10)
		score = 4;
	else if(elapse < 13)
		score = 3;
	else if(elapse < 17)
		score = 2;
	else if(elapse < 20)
		score = 1;
	else
		score = 0;

	result = ANSWER_RIGHT;
	return score;
}

int ContestManager::ComputeAddonScore(int score, char series_right)
{
	float ratio = 0.0f;
	if(series_right<3)
		ratio = 0;
	else if(series_right<5)
		ratio = 0.1;
	else if(series_right<10)
		ratio = 0.2;
	else if(series_right<15)
		ratio = 0.3;
	else if(series_right<20)
		ratio = 0.5;
	else if(series_right<25)
		ratio = 0.7;
	else if(series_right<30)
		ratio = 0.9;
	else if(series_right==30)
		ratio = 1.0;

	return (int)(score*ratio + 0.01);
}

void ContestManager::BroadcastContestMsg(int id, const Octets & msg)
{
	ChatBroadCast cbc;
	cbc.channel = GP_CHAT_SYSTEM;
	cbc.srcroleid = id;
	cbc.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(cbc);
}

void ContestManager::BroadcastContestInvite()
{
	Log::log(LOG_INFO,"gdelivery::ContestManager: BroadcastContestInvite at %d\n", Timer::GetTime());

	ContestInviteBroadcast invite( FORBID_JOIN_INTERVAL );
	LinkServer::GetInstance().BroadcastProtocol(invite);
}

void ContestManager::BroadcastEndContest()
{
	Log::log(LOG_INFO,"gdelivery:ContestManager: BroadcastEndContest, player sum=%d\n", _contest_map.size());

	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	ContestEnd contestend;
	{
		Thread::RWLock::RDScoped l(_map_locker);
		UidIterator it = _contest_map.begin();
		for( ; it!=_contest_map.end(); ++it)
		{
			if(it->second.active)
			{
				contestend.roleid = it->first;
				contestend.localsid = (it->second).localsid;
				dsm->Send((it->second).linksid, contestend);
			}
		}
	}

}

void ContestManager::BroadcastQuestion()
{
	Log::log(LOG_INFO,"gdelivery::ContestManager: BroadcastQuestion at %d, questionid=%d\n", Timer::GetTime(),_next_question);

	unsigned short index = _cur_day;
	unsigned short count = _questionlib.GetQtArrayCount();
	if(index >= count)
		index = index % count;
	if(_next_question>=QUESTION_SUM_OF_ARRAY)
	{
		Log::log(LOG_INFO,"gdelivery::ContestManager: BroadcastQuestion End at %d\n", Timer::GetTime());
		return;
	}

	const _QUESTION_ARRAY* qarray = _questionlib.GetQtArray(index);
	_QUESTION* question = (qarray->listQuestion)[_next_question];
	int len = GetLen(question->szQuestion) ;
	Octets ques((void*)question->szQuestion, len);
	std::vector<Octets> answerlist;
	for(int i=0; i<question->nAnswer; ++i)
	{
		len = GetLen(question->pAnswer[i]) ;
		answerlist.push_back( Octets(question->pAnswer[i], len) );
	}

	ContestQuestion contestques(_next_question, ques, answerlist, 0);
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	{
		Thread::RWLock::RDScoped l(_map_locker);
		UidIterator it = _contest_map.begin();
		for( ; it!=_contest_map.end(); ++it)
		{
			if(it->second.active)
			{
				contestques.localsid = (it->second).localsid;
				dsm->Send((it->second).linksid, contestques);
			}
		}
	}

	_cur_question = _next_question;
	_cur_answer = question->cAnswer;
	_cur_question_time = Timer::GetTime();
	++_next_question;
	_answer_re_cache.clear();
}

void ContestManager::BroadcastPlaceList()
{
	ContestPlaceNotify plist;
	{
		Thread::RWLock::RDScoped l(_list_locker);
		std::list<PlaceInfo>::reverse_iterator it = _place_list.rbegin(); 
		for( ; it!=_place_list.rend(); ++it)
		{
			plist.placelist.add(ContestPlace(it->rolename, it->score));
		}
	}

	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	{
		Thread::RWLock::RDScoped l(_map_locker);
		UidIterator it = _contest_map.begin();
		for( ; it!=_contest_map.end(); ++it)
		{
			if(it->second.active)
			{
				plist.roleid = it->first;
				plist.localsid = (it->second).localsid;
				dsm->Send((it->second).linksid, plist);
			}
		}
	}
}

void ContestManager::BroadcastAnswerReCache()
{
	GDeliveryServer* dsm=GDeliveryServer::GetInstance();
	AnswerReCacheIt it = _answer_re_cache.begin();
	for( ; it!=_answer_re_cache.end(); ++it)
	{
		int linksid = it->first;
		AnswerReList &arlist = it->second;
		AnswerReListIt ait = arlist.begin();
		for( ; ait!=arlist.end(); ++ait)
		{
			dsm->Send(linksid, *ait);
		}
	}

	_answer_re_cache.clear();
}

void ContestManager::BroadcastResult()
{
	ContestResult cr(0,0,0,0);
	GProviderServer *psm = GProviderServer::GetInstance();

	std::list<PlaceInfo>::iterator it = _place_list.begin(); 
	int place = _place_list.size();
	{
		Thread::RWLock::WRScoped l(_map_locker);
		for( ; it!=_place_list.end(); ++it, --place)
		{
			UidIterator uit = _contest_map.find(it->roleid);
			if(uit == _contest_map.end())
				continue;
			(uit->second).place = place;
		}
	}

	{
		Thread::RWLock::RDScoped l(_map_locker);
		UidIterator it = _contest_map.begin();
		for( ; it!=_contest_map.end(); ++it)
		{
			if(!it->second.active)
				continue;
			PlayerContestInfo& pci = it->second;
			cr.roleid = it->first;
			cr.score = pci.score; 
			if(cr.score > MAX_SCORE)
				cr.score = MAX_SCORE;
			cr.place = pci.place; 
			cr.right_amount = pci.right_amount; 
			psm->DispatchProtocol(pci.gameid, cr);
			LOG_TRACE("ContestManager::BroadcastResult: roleid=%d, level=%d, score=%d, place=%d\n", it->first, pci.role_level, pci.score, pci.place);

		}
	}
}

void ContestManager::AddAnswerReCache(unsigned int linksid, ContestAnswer_Re &cas)
{
	_answer_re_cache[linksid].push_back(cas);
}

void ContestManager::DeleteAnswerReCache(unsigned int linksid, int roleid)
{
	AnswerReCacheIt it = _answer_re_cache.find(linksid);
	
	if(it!=_answer_re_cache.end())
	{
		AnswerReList &arlist = it->second;
		AnswerReListIt ait = arlist.begin();
		for( ; ait!=arlist.end(); ++ait)
		{
			if(roleid==ait->roleid)
			{
				arlist.erase(ait);
				return;
			}
		}
	}
}

void ContestManager::Clear() 
{
	_add_task =false; 
	_next_question = 0;
	_cur_question = 0;
	_cur_answer = 0;

	_contest_map.clear(); 
	_place_list.clear();
	_answer_re_cache.clear();
}

void ContestManager::DebugBegin(int roleid, int index)
{
	if(_state != STATE_IDLE)
	{
		LOG_TRACE("DebugBegin: Contest start failed roleid=%d, Contest is processing\n", roleid);
		return;
	}

	if(index<0 || index>30)
		index = 0;

	LOG_TRACE("DebugBegin: Contest start roleid=%d, index=%d\n", roleid, index);
	_cur_day = index;
	_state = STATE_IDLE;
	_start_time = Timer::GetTime() - _calander + 20;
	_next_question = 0;
	_cur_question = 0;
}

void ContestManager::DebugEnd(int roleid)
{
	LOG_TRACE("DebugEnd: Contest end roleid=%d\n", roleid);
	Marshal::OctetsStream os;
	BroadcastContestMsg(MSG_CONTESTEND, os);
	BroadcastResult();
	BroadcastEndContest();

	_state = STATE_IDLE;
	_start_time = START_TIME;
	time_t now = Timer::GetTime();
	struct tm dt;
	localtime_r(&now, &dt);
	_cur_day = dt.tm_mday;

	Clear();
}

bool ContestManager::BuildQuestion(int day, int id, ContestQuestion& contestques)
{
	if(day>=_questionlib.GetQtArrayCount())
		day = 0;
	if(id>=QUESTION_SUM_OF_ARRAY)
	{
		return false;	
	}

	const _QUESTION_ARRAY* qarray = _questionlib.GetQtArray(day);
	_QUESTION* question = (qarray->listQuestion)[id];
	int len = GetLen(question->szQuestion) ;
	Octets ques((void*)question->szQuestion, len);
	std::vector<Octets> answerlist;
	for(int i=0; i<question->nAnswer; ++i)
	{
		len = GetLen(question->pAnswer[i]) ;
		answerlist.push_back( Octets(question->pAnswer[i], len) );
	}

	contestques.questionid = id;
	contestques.question = ques;
	contestques.answerlist = answerlist;
	return true;
}


};


