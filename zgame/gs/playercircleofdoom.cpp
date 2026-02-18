#include "playercircleofdoom.h"
#include "gmatrix.h"
#include "vector.h"
#include "common/message.h"



void 
player_circle_of_doom::BroadcastMemberMsg(const XID & source_id, A3DVECTOR & source_pos, int message)
{
	abase::static_set<int, abase::fast_alloc<> >::iterator iter;

	for( iter = _member_set.begin(); iter != _member_set.end(); iter++ )
	{
		MSG msg;
		BuildMessage(msg, message, XID(GM_TYPE_PLAYER, *iter), source_id, source_pos);
		gmatrix::SendMessage(msg);
	}
}

bool 
player_circle_of_doom::IsOverMaxMemberNum() 
{ 
	if( (int)_member_set.size() <=  _max_member_num )
	{
		return false;
	}

	return true;
}

bool 
player_circle_of_doom::GetSponsorID(int &sponsor_id)
{
	if(_member_set.size() >= 1)
	{
		return false;
	}

	abase::static_set<int, abase::fast_alloc<> >::iterator iter;
	iter = _member_set.begin();
	if(_member_set.end() == iter)
	{
		__PRINTF("取不到sponsorid，自己是sponsor? 但sponsor不会调用这个函数！\n");
		return false;
	}

	sponsor_id = *iter;
	return true;
}

int 
player_circle_of_doom::GetMemberXIDList( std::vector<XID> &member_list , const XID &self_xid, bool is_include_self )
	
{
	abase::static_set<int, abase::fast_alloc<> >::iterator iter;
	for(iter = _member_set.begin(); iter != _member_set.end(); iter++)
	{
		member_list.push_back(XID(GM_TYPE_PLAYER, *iter));
	}

	if( is_include_self )
	{
		member_list.push_back(self_xid);
	}

	return member_list.size();
}

int 
player_circle_of_doom::GetMemberIDList( abase::vector<int> &member_list , int self_id, bool is_include_self )
{
	abase::static_set<int, abase::fast_alloc<> >::iterator iter;
	for(iter = _member_set.begin(); iter != _member_set.end(); iter++)
	{
		member_list.push_back( *iter);
	}

	if(is_include_self)
	{
		member_list.push_back(self_id);
	}

	return member_list.size();
}

int 
player_circle_of_doom::GetMemberCnt()
{
	return _member_set.size() + 1;
}

bool 
player_circle_of_doom::IsNormalRunning() 
{
	if( STATE_STOP == _state || STATE_SINGLE == _state )
	{
		return false;
	}

	return true;
}

bool 
player_circle_of_doom::Insert(int id) 
{
	if(!_member_set.insert(id).second)
	{
		__PRINTF("阵法成员insert失败！\n");
		return false;
	}

	return true;
}

bool 
player_circle_of_doom::IsInCircleOfDoom() 
{ 
	if( STATE_STOP == _state )
	{
		return false;
	}

	return true;
}

void 
player_circle_of_doom::SetSponsorPrepare(int max_member_num, int skill_id)
{
	SetPrepare();
	SetSponsor();
	SetMaxMemberNum(max_member_num);
	_skill_id = skill_id;
}

void
player_circle_of_doom::SetSponsorStop(const XID & source_id, A3DVECTOR & source_pos)
{
	SetStop();
    BroadcastMemberMsg(source_id, source_pos, GM_MSG_CIRCLE_OF_DOOM_STOP);
	Clear();
}

void 
player_circle_of_doom::SetSponsorStartup(const XID & source_id, A3DVECTOR & source_pos)
{
	SetStartup();
	BroadcastMemberMsg(source_id, source_pos, GM_MSG_CIRCLE_OF_DOOM_STARTUP);
}
