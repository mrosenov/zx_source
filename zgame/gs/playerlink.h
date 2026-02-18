/*
 * FILE: PlayerLink.h
 *
 * DESCRIPTION: Link multi players together
 *
 * CREATED BY: lianshuming, 2008/12/29
 *
 * HISTORY:
 *
 * Copyright (c) 2008 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_PLAYER_LINK_H
#define __ONLINEGAME_GS_PLAYER_LINK_H

#include <map>
#include <vector>
#include <algorithm>
#include <common/types.h>
#include <common/protocol.h>
#include "spinlock.h"
#include "config.h"

#define MAX_LINK_COUPLE_CAPACITY 1		//相依相偎的最大人数
#define MAX_LINK_RIDE_CAPACITY 	 5 		//多人骑乘的最多人数
#define MAX_LINK_BATH_CAPACITY	1		//泡澡的最多人数
#define MAX_LINK_QILIN_CAPACITY	1		//麒麟的最多人数

#define LINK_INVITE_TIMEOUT 30
#define LINK_INVITE_TIMEOUT2 25

#define LINK_COUPLE_LEADER_TYPE 1
#define LINK_COUPLE_MEMBER_TYPE 2
#define LINK_RIDE_LEADER_TYPE 3
#define LINK_RIDE_MEMBER_TYPE 4
#define LINK_BATH_LEADER_TYPE 5 
#define LINK_BATH_MEMBER_TYPE 6
#define LINK_QILIN_LEADER_TYPE 7 
#define LINK_QILIN_MEMBER_TYPE 8 

class gplayer_imp;

class sequence
{
public:
	sequence(int cnt){
		vec.assign(cnt, false);
	}
	int GetMaxSize(){return vec.size();}
	int GetCount(){
		return std::count(vec.begin(), vec.end(), true);
	}

	bool GetPos(int pos){
		return vec[pos];
	}
	void SetPos(int pos, bool value){
		vec[pos] = value;
	}

	int FindPos(){
		for(size_t i=0; i< vec.size(); i++){
			if(! vec[i]){
				return i;
			}
		}
		return -1;
	}

	void ResetAll(){
		vec.assign(vec.size(), false);
	}

private:
	std::vector<bool> vec;
};


struct invite_t{
	long timestamp;
	XID id;
};

struct member_t{
	XID id;
	int cs_index;		//队员的cs index
	int cs_sid;			//队员的sid
	char sequence;
};

enum LINK_MESSAGE
{
	GM_MSG_PLAYER_LINK_INVITE,
	GM_MSG_PLAYER_LINK_REQUEST,
	GM_MSG_PLAYER_LINK_INV_REPLY,
	GM_MSG_PLAYER_LINK_REQ_REPLY,
	GM_MSG_PLAYER_LINK_PREPARE,
	GM_MSG_PLAYER_LINK_START,
	GM_MSG_PLAYER_LINK_FOLLOW,
	GM_MSG_PLAYER_LINK_STOP,
	GM_MSG_PLAYER_LINK_LEAVE_REQUEST,
	GM_MSG_PLAYER_LINK_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_LEADER_LEAVE,
	GM_MSG_PLAYER_LINK_KICK_MEMBER,
};

enum ERROR_MESSAGE
{
	ERR_LINK_INVALID_REQUEST,
	ERR_LINK_CANNOT_HERE,
	ERR_LINK_INVITE_FULL,
	ERR_LINK_ALREADY_INVITE,
	ERR_LINK_ALREADY_EXIST,
	ERR_LINK_REQUEST_FULL,
	ERR_LINK_ALREADY_REQUEST,
	ERR_LINK_INVITE_TIMEOUT,
	ERR_LINK_FAR_DISTANCE,
};

typedef std::map<int, invite_t> INVITE_MAP;
typedef std::map<int, member_t> MEMBER_MAP;
typedef std::map<int, int> MESSAGE_MAP;

class player_link
{
public:
	player_link(int max_invite):_mode(0), _timeout(0), _seq(0), _leader(XID(-1, -1)), 
		 team_seq(max_invite), last_invite_time(0), spinlock(0){}
	virtual ~player_link(){}

	virtual void PlayerLinkInvite(gplayer_imp * pImp, const XID & target);
	virtual void PlayerLinkRequest(gplayer_imp * pImp, const XID & target);
	virtual void PlayerLinkInvReply(gplayer_imp * pImp, const XID & target,int param);
	virtual void PlayerLinkReqReply(gplayer_imp * pImp, const XID & target,int param);
	virtual void PlayerLinkCancel(gplayer_imp *pImp);
	virtual void PlayerLinkKick(gplayer_imp *pImp, char pos);
	virtual void PlayerLinkDisconnect(gplayer_imp * pImp){}
	virtual void PlayerLinkReconnect(gplayer_imp * pImp){}

	virtual void MsgInvite(gplayer_imp * pImp, const XID & target);
	virtual void MsgRequest(gplayer_imp * pImp, const XID & target);
	virtual void MsgInviteReply(gplayer_imp * pImp, const XID & target, int param);
	virtual void MsgRequestReply(gplayer_imp * pImp, const XID & target, int param);

	virtual void MsgPrepare(gplayer_imp *pImp, const XID & target);
	virtual void MsgBeLinked(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos, const char pos);
	virtual void MsgFollowOther(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);

	virtual void MsgMemberLeaveRequest(gplayer_imp *pImp, const XID & target);
	virtual void MsgMemberLeave(gplayer_imp *pImp, const XID& leader, const XID & target);
	virtual void MsgLeaderLeave(gplayer_imp *pImp, const XID & target);
	virtual void MsgKickMember(gplayer_imp *pImp, const XID & target);
	virtual void MsgStopLinked(gplayer_imp * pImp, const XID & target);


	void SendGroupMessage(gplayer_imp *pImp, int message, int param = 0);
	virtual void Heartbeat(gplayer_imp * pImp);

	virtual const XID GetLinkedPlayer(){
		if(_mode == MODE_LEADER){
			if(members.empty()){
				return XID(-1, -1);
			}
			MEMBER_MAP::iterator iter = members.begin();
			return ((*iter).second.id);
		}
		else{
			return _leader;
		}
	}

	bool IsPlayerLinked() { return _mode == MODE_LEADER;}
/*
	template <typename WRAPPER> void Save(WRAPPER & wrapper)
	{
		wrapper.push_back(this, sizeof(*this));
	}

	template <typename WRAPPER> void Load(WRAPPER & wrapper)
	{
		wrapper.pop_back(this, sizeof(*this));
	}
*/

	void Swap(player_link & rhs)
	{
		player_link tmp = rhs;
		rhs = *this;
		*this = tmp;
	}

protected:
	void SendMessage(gplayer_imp *pImp, int message, const XID &target, int param = 0);
	virtual int GetMapMessage(int message){return -1;}
	virtual int GetMapErrorMessage(int message){return -1;}
	virtual size_t GetMaxInviteCount(){return 0;}
	virtual size_t GetMaxRequestCount(){return 0;}
	virtual char GetLinkLeaderType(){return 0;}
	virtual char GetLinkMemberType(){return 0;}

	virtual void CliMsgInvite(gplayer_imp * pImp, const XID & target){}
	virtual void CliMsgRequest(gplayer_imp * pImp, const XID & target){}
	virtual void CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param){}
	virtual void CliMsgRequestReply(gplayer_imp * pImp, const XID & target, int param){}

	virtual void CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos){}
	virtual void CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos){}
	virtual void CliMemberLeave(gplayer_imp *pImp, const XID & target){}
	virtual void CliLeaderLeave(gplayer_imp *pImp){}
	virtual void CliNotifyMemberLeave(gplayer_imp *pImp, const XID & target){}
	virtual void CliKickMember(gplayer_imp *pImp, const XID & leader, char pos){};
	
	virtual bool CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target);
	virtual bool CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target);

	virtual bool EnterBindLeaderState(gplayer_imp * pImp, const XID & target, char type); 
	virtual bool EnterBindMemberState(gplayer_imp * pImp, const XID & target, char type, const char seq); 
	virtual bool IsValidPartner(gplayer_imp * self, const XID & partner);

	
	void ClsInviteMap(){invite_map.clear();}
	void ClsAppliedMap(){applied_map.clear();}
	void ClsMemberMap(){members.clear();}

	void ClearAllState(gplayer_imp *pImp);
	void CheckInviteMap(INVITE_MAP& _map);


private:
	void SendErrorMessage(gplayer_imp *pImp, int message);
	int GetAvailSequence(){return team_seq.FindPos();}

	char AddMember(const XID& target){
		if(members.find(target.id) != members.end()){
			return 0;
		}
		if(members.size() >= GetMaxInviteCount()){
			return 0;
		}

		member_t _new;
		_new.id = target;
		if((_new.sequence = GetAvailSequence() +1) == 0){
			return 0;
		}
		members[target.id]= _new;
		team_seq.SetPos(_new.sequence-1, true);
		return (_new.sequence);
	}

	bool RemoveMember(const XID& target){
		MEMBER_MAP::iterator iter;
		if((iter = members.find(target.id)) == members.end()){
			return false;
		}

		team_seq.SetPos(members[target.id].sequence-1, false);

		members.erase(target.id);
		return true;
	}

	XID FindMemberByPos(char pos){
		MEMBER_MAP::iterator iter;
		for(iter = members.begin(); iter != members.end(); ++iter){
			if(iter->second.sequence == pos){
				return XID(GM_TYPE_PLAYER, iter->first);
			}
		}
		return XID(-1, -1);
	}
protected:
	char _mode;
	char _timeout;
	char _seq;
	XID _leader;
	
	enum
	{
		MODE_NORMAL,
		MODE_REQUEST,
		MODE_INVITE,
		MODE_PREPARE,
		MODE_MEMBER,
		MODE_LEADER,
	};

	enum
	{
		NORMAL_TIMEOUT = 30,
		PREPARE_TIMEOUT = 5,
	};

	enum
	{
		ERR_SUCCESS,
		ERR_REFUSE,
		ERR_CANNOT_BIND,
	};

private:
	sequence team_seq;
	long last_invite_time;
	int spinlock;

	INVITE_MAP invite_map; //邀请者的map
	INVITE_MAP applied_map; //申请者的map
	MEMBER_MAP members;
};



//
//实现相依相偎
//
class player_link_couple : public player_link
{
public:
	player_link_couple():player_link(MAX_LINK_COUPLE_CAPACITY){}
//	virtual void Heartbeat(gplayer_imp * pImp);


	void Swap(player_link_couple & rhs)
	{
		player_link_couple tmp = rhs;
		rhs = *this;
		*this = tmp;
	}
protected:
	virtual void CliMsgInvite(gplayer_imp * pImp, const XID & target);
	virtual void CliMsgRequest(gplayer_imp * pImp, const XID & target);
	virtual void CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param);
	virtual void CliMsgRequestReply(gplayer_imp * pImp, const XID & target, int param);
	
	virtual void CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	virtual void CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	virtual void CliMemberLeave(gplayer_imp *pImp, const XID & target);
	virtual void CliLeaderLeave(gplayer_imp *pImp);
	
	virtual bool CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target);
	virtual bool CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target);


	virtual int GetMapMessage(int message);
	virtual int GetMapErrorMessage(int message);

	virtual size_t GetMaxInviteCount(){return MAX_LINK_COUPLE_CAPACITY;}
	virtual size_t GetMaxRequestCount(){return MAX_LINK_COUPLE_CAPACITY;}

	virtual char GetLinkLeaderType(){return LINK_COUPLE_LEADER_TYPE;}
	virtual char GetLinkMemberType(){return LINK_COUPLE_MEMBER_TYPE;}
private:
	bool IsMale(gplayer_imp * pImp);
	bool IsFemale(gplayer_imp * pImp);
};


//
//实现多人骑乘
//
class player_link_ride : public player_link
{
public:
	player_link_ride ():player_link(MAX_LINK_RIDE_CAPACITY){}

	void Swap(player_link_ride & rhs)
	{
		player_link_ride tmp = rhs;
		rhs = *this;
		*this = tmp;
	}

	void OnMemberDeath(gplayer_imp *pImp);

protected:
	virtual void CliMsgInvite(gplayer_imp * pImp, const XID & target);
	virtual void CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param);
	
	virtual void CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	virtual void CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	virtual void CliMemberLeave(gplayer_imp *pImp, const XID & target);
	virtual void CliLeaderLeave(gplayer_imp *pImp);
	virtual void CliKickMember(gplayer_imp *pImp, const XID & leader, char pos);
	
	virtual bool CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target);

	virtual int GetMapMessage(int message);
	virtual int GetMapErrorMessage(int message);

	virtual size_t GetMaxInviteCount(){return MAX_LINK_RIDE_CAPACITY;}
	virtual size_t GetMaxRequestCount(){return MAX_LINK_RIDE_CAPACITY;}

	virtual char GetLinkLeaderType(){return LINK_RIDE_LEADER_TYPE;}
	virtual char GetLinkMemberType(){return LINK_RIDE_MEMBER_TYPE;}
private:

};


//
//实现泡澡功能
//
class player_link_bath : public player_link
{
public:
	player_link_bath() : player_link(MAX_LINK_BATH_CAPACITY){}

	void Swap(player_link_bath & rhs)
	{
		player_link_bath tmp = rhs;
		rhs = *this;
		*this = tmp;
	}	

protected:
	virtual void CliMsgInvite(gplayer_imp * pImp, const XID & target);
	virtual void CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param);
	
	virtual void CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	virtual void CliMemberLeave(gplayer_imp *pImp, const XID & target);
	virtual void CliLeaderLeave(gplayer_imp *pImp);
	
	virtual bool CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target);

	virtual bool EnterBindLeaderState(gplayer_imp * pImp, const XID & target, char type); 
	virtual bool EnterBindMemberState(gplayer_imp * pImp, const XID & target, char type, const char seq); 

	virtual int GetMapMessage(int message);
	virtual int GetMapErrorMessage(int message);

	virtual size_t GetMaxInviteCount(){return MAX_LINK_BATH_CAPACITY;}
	virtual size_t GetMaxRequestCount(){return MAX_LINK_BATH_CAPACITY;}

	virtual char GetLinkLeaderType(){return LINK_BATH_LEADER_TYPE;}
	virtual char GetLinkMemberType(){return LINK_BATH_MEMBER_TYPE;}
};

//
//实现麒麟功能
//
class player_link_qilin: public player_link
{
public:
	player_link_qilin() : player_link(MAX_LINK_QILIN_CAPACITY), _team_id(0){}

	void Swap(player_link_qilin & rhs)
	{
		player_link_qilin tmp = rhs;
		rhs = *this;
		*this = tmp;
	}	

	virtual void PlayerLinkDisconnect(gplayer_imp * pImp);
	virtual void PlayerLinkReconnect(gplayer_imp * pImp);

protected:
	virtual void CliMsgInvite(gplayer_imp * pImp, const XID & target);
	virtual void CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param);
	
	virtual void CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	virtual void CliMemberLeave(gplayer_imp *pImp, const XID & target);
	virtual void CliLeaderLeave(gplayer_imp *pImp);
	virtual void CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos);
	
	virtual bool CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target);
	virtual bool CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target);

	virtual int GetMapMessage(int message);
	virtual int GetMapErrorMessage(int message);

	virtual size_t GetMaxInviteCount(){return MAX_LINK_QILIN_CAPACITY;}
	virtual size_t GetMaxRequestCount(){return MAX_LINK_QILIN_CAPACITY;}

	virtual char GetLinkLeaderType(){return LINK_QILIN_LEADER_TYPE;}
	virtual char GetLinkMemberType(){return LINK_QILIN_MEMBER_TYPE;}
	virtual bool IsValidPartner(gplayer_imp * self, const XID & partner);

private:
	int _team_id;
};

#endif //__ONLINEGAME_GS_PLAYER_LINK_H

