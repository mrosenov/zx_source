#ifndef __ONLINEGAME_GS_PLAYERCIRCLEOFDOOM_H__
#define __ONLINEGAME_GS_PLAYERCIRCLEOFDOOM_H__

#include <common/types.h>
#include "amemory.h"
#include "staticmap.h"
#include <vector>


class player_circle_of_doom
{
	enum
	{
		STATE_PREPARE,
		STATE_STARTUP,
		STATE_STOP,
		STATE_SINGLE, //只能自己一个人起阵,不在队伍、帮派、家族中
	};

public:
	player_circle_of_doom()  { Clear(); }
	~player_circle_of_doom() { Clear(); }

	//对外接口
	void SetSponsorPrepare(int max_member_num, int skill_id);
	void SetSponsorStop(const XID & source_id, A3DVECTOR & source_pos);
	void SetSponsorStartup(const XID & source_id, A3DVECTOR & source_pos);

	void SetMemberPrepare() { SetPrepare(); }
	void SetMemberStop()    { Clear(); }
	void SetMemberStartup() { SetStartup(); }

	bool IsInCircleOfDoom(); 
	bool IsMember(int id) const { return _member_set.find(id) != _member_set.end(); }
	bool IsPrepare() const { return (STATE_PREPARE == _state); }
	bool IsStartup() const { return (STATE_STARTUP == _state); }
	bool IsStop()    const { return (STATE_STOP == _state); }
	bool IsSingle()  const { return (STATE_SINGLE == _state); }
	bool IsSponsor() const { return _is_sponsor; }
	bool IsNormalRunning(); 
	bool IsOverMaxMemberNum(); 

	//只有自己起阵自己也是sponsor
	void SetSingle(int skill_id)  
	{
		SetSponsor();
		_state = STATE_SINGLE; 
		_skill_id = skill_id;
	}

	bool Insert(int id); 
	void Remove(int id) { _member_set.erase(id); }
	void Leave() { Clear(); }
	void BroadcastMemberMsg(const XID & source_id, A3DVECTOR & source_pos, int message);
	void ClearMember() {  _member_set.clear(); }
	
	void Swap(player_circle_of_doom &rhs)
	{
		_state = rhs.GetSate();
		_max_member_num = rhs.GetMaxMember();
		_is_sponsor     = rhs.IsSponsor();
		_skill_id       = rhs.GetSkillid();
		_member_set.swap( rhs.GetMemberSet() );
	}

	bool GetSponsorID(int &sponsor_id);
	int  GetMemberXIDList( std::vector<XID> &member_list , const XID &self_xid, bool is_include_self = false );
	int  GetMemberIDList( abase::vector<int> &member_list , int self_id, bool is_include_self = false );
	int  GetMemberCnt();
	int  GetSkillid() const { return _skill_id; }

	//调试使用
	int GetCurState() const { return _state; }

private:
	//如果是阵法发起者用来保存阵法成员(不包含发起者自己)
	//如果是阵法成员，用来保存阵法发起者的id
	abase::static_set<int, abase::fast_alloc<> > _member_set;
	int  _state;          //阵法的当前状态
	int  _max_member_num; //阵法最大成员数(不包含自己)
	bool _is_sponsor;     //表示是否是阵眼，即阵法发起者
	int  _skill_id;       //阵法发起者所使用的阵法技能id，用于客户端显示

	//swap used function
	int GetSate() const { return _state; }
	int GetMaxMember() const { return _max_member_num; }
	abase::static_set<int, abase::fast_alloc<> > & GetMemberSet() {	return _member_set;	}

	void SetSponsor() {	_is_sponsor = true;}
	void SetMaxMemberNum(int max_num) {	_max_member_num = max_num; }
	void SetPrepare() {	_state = STATE_PREPARE; }
	void SetStartup() { _state = STATE_STARTUP; }
	void SetStop()    { _state = STATE_STOP; }
	
	void Clear() 
	{
		_member_set.clear();
		_state = STATE_STOP;
		_max_member_num = 0;
		_is_sponsor     = false;
		_skill_id       = -1;
	}
};

#endif
