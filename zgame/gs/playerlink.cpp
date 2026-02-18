/*
 * FILE: PlayerLink.cpp
 *
 * DESCRIPTION: Link multi players together
 *
 * CREATED BY: lianshuming, 2008/12/29
 *
 * HISTORY:
 *
 * Copyright (c) 2008 Archosaur Studio, All Rights Reserved.
 */

#include "playerlink.h"
#include "player_imp.h"
#include "vector.h"
#include <common/message.h>
#include <common/message.h>


extern abase::timer	g_timer;

//
//检查Player当前状态是否可以进行Link
bool player_link::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	//检查Player状态
	if(!pImp->CheckPlayerBindInvite() || !target.IsPlayer())
	{
		SendErrorMessage(pImp, ERR_LINK_INVALID_REQUEST);
		return false;
	}

	//检查地图限制
	if(pImp->GetWorldManager()->GetWorldLimit().nobind)
	{
		SendErrorMessage(pImp, ERR_LINK_CANNOT_HERE);
		return false;
	}
	return true;
}

//
//检查Player当前状态是否可以进行Link
bool player_link::CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	//检查Player状态
	if(!pImp->CheckPlayerBindRequest() || !target.IsPlayer())
	{
		SendErrorMessage(pImp, ERR_LINK_INVALID_REQUEST);
		return false;
	}

	//检查地图限制
	if(pImp->GetWorldManager()->GetWorldLimit().nobind)
	{
		SendErrorMessage(pImp, ERR_LINK_CANNOT_HERE);
		return false;
	}
	return true;
}

//
//发送消息给gs
void player_link::SendMessage(gplayer_imp *pImp, int message, const XID &target, int param)
{
	int msg = -1;
	if ((msg = GetMapMessage(message)) == -1){
		return;
	}
	if (!target.IsPlayer()){
		GLog::log(GLOG_ERR,"发送消息给其他玩家时，id不正确，发送者=%d",pImp->_parent->ID.id);
		return;
	}

	pImp->SendTo<0>(msg,target,param);
}

//
//Leader发送消息给所有Member
void player_link::SendGroupMessage(gplayer_imp *pImp, int message, int param )
{
	int msg = -1;
	if ((msg = GetMapMessage(message)) == -1){
		return;
	}

	MEMBER_MAP::iterator iter = members.begin();
	XID target;
	for(; iter != members.end(); iter++){
		target = (*iter).second.id;
		if(!target.IsPlayer()) return;
		pImp->SendTo<0>(msg, target, param);
	}
}


//
//发送错误消息给Client
void player_link::SendErrorMessage(gplayer_imp *pImp, int message)
{
	int err_msg = -1;
	if ((err_msg = GetMapErrorMessage(message)) == -1){
		return;
	}
	pImp->_runner->error_message(err_msg);
}


void player_link::ClearAllState(gplayer_imp *pImp)
{
	ClsInviteMap();
	ClsAppliedMap();
	ClsMemberMap();
	team_seq.ResetAll();
	_leader = XID(-1, -1);
	_mode = MODE_NORMAL;
	_seq = 0;
	_timeout = 0;
	last_invite_time = 0;
	spinlock = 0;
	pImp->ReturnBindNormalState(2);
}

//
//发出绑定的邀请
void player_link::PlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	//检查Player状态和地图限制
	if(!CheckPlayerLinkInvite(pImp, target)){
		return;
	}

	//是否队列已满
	if(invite_map.size() >= (GetMaxInviteCount()) || members.size() == GetMaxInviteCount() ){
		SendErrorMessage(pImp, ERR_LINK_INVITE_FULL);
		return;
	}

	//是否已经邀请了
	if(invite_map.find(target.id) != invite_map.end()){
		SendErrorMessage(pImp, ERR_LINK_ALREADY_INVITE);		
		return;
	}

	if(members.find(target.id) != members.end()){
		SendErrorMessage(pImp, ERR_LINK_ALREADY_EXIST);		
		return;
	}


	if (_mode == MODE_NORMAL){
		_mode = MODE_INVITE;
	}
	_timeout = NORMAL_TIMEOUT;

	invite_t it;
	it.timestamp = g_timer.get_systime() + LINK_INVITE_TIMEOUT;
	it.id = target;
	invite_map[target.id] = it;

	SendMessage(pImp, GM_MSG_PLAYER_LINK_INVITE, target);
}

//
//发出绑定的请求
void player_link::PlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	//检查Player状态和地图限制
	if(!CheckPlayerLinkRequest(pImp, target)){
		return;
	}

	//检查mode
	if(_mode){
		SendErrorMessage(pImp, ERR_LINK_INVALID_REQUEST);
		return;
	}

	//是否队列已满
	if(applied_map.size() >= GetMaxRequestCount()){
		SendErrorMessage(pImp, ERR_LINK_REQUEST_FULL);		
		return;
	}

	//是否已经邀请了
	if(applied_map.find(target.id) != applied_map.end()){
		SendErrorMessage(pImp, ERR_LINK_ALREADY_REQUEST);				
		return;
	}

	//设置状态
	_mode = MODE_REQUEST;
	_timeout = NORMAL_TIMEOUT;

	invite_t it;
	it.timestamp = g_timer.get_systime() + LINK_INVITE_TIMEOUT;
	it.id = target;
	applied_map[target.id] = it;

	SendMessage(pImp, GM_MSG_PLAYER_LINK_REQUEST, target);
}

//
//处理客户端对邀请的回应
void player_link::PlayerLinkInvReply(gplayer_imp * pImp, const XID & target,int param)
{
	//检查Player状态和地图限制
	if(!CheckPlayerLinkRequest(pImp, target)){
		return;
	}

	//判断是否超时
	if(last_invite_time < g_timer.get_systime()){
		SendErrorMessage(pImp, ERR_LINK_INVITE_TIMEOUT);
		return;						
	}

	//同意邀请则设置状态
	if(param == ERR_SUCCESS && _mode != MODE_LEADER){
		_mode = MODE_PREPARE;
		_timeout = PREPARE_TIMEOUT;
		_leader = target;
	}

	SendMessage(pImp, GM_MSG_PLAYER_LINK_INV_REPLY, target, param);	
}

//
//处理客户端对请求的回应
void player_link::PlayerLinkReqReply(gplayer_imp * pImp, const XID & target,int param)
{
	//检查Player状态和地图限制
	if(!CheckPlayerLinkRequest(pImp, target)){
		return;
	}

	//同意请求则设置状态
	if(param == ERR_SUCCESS && _mode != MODE_LEADER){
		_mode = MODE_PREPARE;
		_timeout = PREPARE_TIMEOUT;
	}

	SendMessage(pImp, GM_MSG_PLAYER_LINK_REQ_REPLY, target, param);	
}

//
//处理离开或者解散请求
void player_link::PlayerLinkCancel(gplayer_imp *pImp)
{
	if(_mode != MODE_LEADER && _mode != MODE_MEMBER){
		return;
	}
	if(_mode == MODE_MEMBER && _leader != XID(-1, -1)){
		//如果是成员只先发送消息给leader
		SendMessage(pImp, GM_MSG_PLAYER_LINK_LEAVE_REQUEST, _leader);	
		CliMemberLeave(pImp, pImp->_parent->ID);
	}	

	else if(_mode == MODE_LEADER){
		SendGroupMessage(pImp, GM_MSG_PLAYER_LINK_LEADER_LEAVE);
		CliLeaderLeave(pImp);
	}
}

//
//处理踢人逻辑
void player_link::PlayerLinkKick(gplayer_imp *pImp, char pos)
{
	if(_mode != MODE_LEADER){
		return;
	}

	if(pos <= 0 || pos >5 ){
		return;
	}

	XID target;
	if((target=FindMemberByPos(pos)) == XID(-1, -1)){
		return;
	}

	if(!RemoveMember(target)){
		return;
	}

	SendMessage(pImp, GM_MSG_PLAYER_LINK_KICK_MEMBER, target);
	if(members.empty()){
		CliLeaderLeave(pImp);
	}
}
//
//处理邀请消息
void player_link::MsgInvite(gplayer_imp * pImp, const XID & target)
{
	//检查Player的mode
	if(_mode == MODE_PREPARE || _mode == MODE_LEADER || _mode == MODE_MEMBER)
	{
		SendMessage(pImp, GM_MSG_PLAYER_LINK_INV_REPLY, target, ERR_CANNOT_BIND);
		return;
	}

	last_invite_time = g_timer.get_systime() + LINK_INVITE_TIMEOUT2;

	//通知消息给客户端
	CliMsgInvite(pImp, target);
}


//
//处理请求信息
void player_link::MsgRequest(gplayer_imp * pImp, const XID & target)
{
	//检查Player的mode
	if(_mode == MODE_PREPARE || _mode == MODE_LEADER || _mode == MODE_MEMBER)
	{
		//直接返回一个拒绝消息即可
		SendMessage(pImp, GM_MSG_PLAYER_LINK_REQ_REPLY, target, ERR_CANNOT_BIND);		
		return;
	}

	//通知消息给客户端
	CliMsgRequest(pImp, target);

}

//
//处理回复的邀请信息
void player_link::MsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	INVITE_MAP::iterator it = invite_map.find(target.id);
	if (it == invite_map.end()){
		//回馈一个消息 让对方脱离等待状态
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);				
		return;
	}
	invite_map.erase(it);

	if(_mode == MODE_NORMAL || !CheckPlayerLinkInvite(pImp, target)){
		//回馈一个消息 让对方脱离等待状态
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}

	//如果传回来的是拒绝消息，则清除当前状态，进行必要的操作
	if(param)
	{
		//此操作要传回给客户端
		CliMsgInviteReply(pImp, target, param);
		return;
	}

	int seq;
	//添加成员
	if((seq =AddMember(target)) == 0){
		SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}
	if (_mode != MODE_LEADER){
		if(!EnterBindLeaderState(pImp, target, GetLinkLeaderType()))
		{
			//进入状态失败 则直接返回
			return ;
		}
		_mode = MODE_LEADER;
	}

	//通知对方开始绑定
	SendMessage(pImp, GM_MSG_PLAYER_LINK_START, target, seq);
}

//
//处理回复的请求信息
void player_link::MsgRequestReply(gplayer_imp * pImp, const XID & target, int param)
{
	INVITE_MAP::iterator it = applied_map.find(target.id);
	if(it == applied_map.end()){
		//回馈一个消息 让对方脱离等待状态
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}

	applied_map.erase(it);

	if(_mode == MODE_NORMAL || !CheckPlayerLinkRequest(pImp,target)){
		//回馈一个消息 让对方脱离等待状态
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}

	//如果传回来的是拒绝消息，则清除当前状态，进行必要的操作
	if(param)
	{
		//此操作要传回给客户端
		CliMsgRequestReply(pImp, target, param);
		_mode = MODE_NORMAL;
		return;
	}

	_mode = MODE_PREPARE;
	_timeout = PREPARE_TIMEOUT;
	_leader = target;

	//传回的是成功消息,则通告之,并且通知对方进入可以进入绑定状态
	SendMessage(pImp, GM_MSG_PLAYER_LINK_PREPARE, target);	
}

void player_link::MsgPrepare(gplayer_imp *pImp, const XID & target)
{

	if((_mode != MODE_PREPARE && _mode != MODE_LEADER) || !CheckPlayerLinkInvite(pImp,target))
	{
		//状态不匹配或者目标不匹配则忽略
		return;
	}

	char seq;
	//添加成员
	if((seq = AddMember(target)) == 0){
		return;
	}

	if(_mode != MODE_LEADER){
		//对方同意开始绑定了,直接进入绑定状态
		if(!EnterBindLeaderState(pImp, target, GetLinkLeaderType()))
		{
			//进入状态失败 则直接返回
			return ;
		}
		//状态一致,进入link的状态
		_mode = MODE_LEADER;
	}
	//由对方发送绑定开始的消息
	SendMessage(pImp, GM_MSG_PLAYER_LINK_START, target, seq);	

}

//
//接受了邀请的人开始绑定
void player_link::MsgBeLinked(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos, const char seq)
{
	if(_mode != MODE_PREPARE || target != _leader || !CheckPlayerLinkRequest(pImp,target))
	{
		//若是非预期的状态则不处理
		//考虑一下自己的状态是否符合 (是否处于可以进行移动的状态,这个状态和请求的状态一致)
		//是否返回一个消息?
		SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		ClearAllState(pImp);			
		return;
	}

	if(!EnterBindMemberState(pImp, target, GetLinkMemberType(), seq))
	{
		//进入状态失败则直接返回
		return;
	}

	_mode = MODE_MEMBER;
	_seq = seq;

	CliMemberJoin(pImp, target, pos);
}

//
//Member开始Follow Leader跑了
void player_link::MsgFollowOther(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	//其他玩家发来了要求跟随的消息
	if(_mode != MODE_MEMBER|| target != _leader)
	{
		//不予处理
		//或者考虑返回一个停止link
		if(target != _leader || _mode != MODE_PREPARE)
		{
			//这是担心消息顺序错误,不是所有的状态都返回错误消息
			SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);		
		}
		return ;
	}

	CliMemberFollow(pImp, target, pos);
}


//
//Leader处理Member离开的请求
void player_link::MsgMemberLeaveRequest(gplayer_imp *pImp, const XID & target)
{
	if(_mode != MODE_LEADER){
		return;
	}

	SendGroupMessage(pImp, GM_MSG_PLAYER_LINK_MEMBER_LEAVE, target.id);

	if(!RemoveMember(target)){
		return;
	}

	if(members.empty()){
		CliLeaderLeave(pImp);
	}
}

//
//Member处理从Leader那里收到的Member离开的消息
void player_link::MsgMemberLeave(gplayer_imp *pImp, const XID& leader, const XID & target)
{
	if(_mode != MODE_MEMBER || leader != _leader){
		return;
	}

	/*	if(!RemoveMember(target)){
		return;
		}
	 */
	//如果是自己发出的请求
	if(target == pImp->_parent->ID){
		CliMemberLeave(pImp, target);
		return;
	}
	else{
		CliNotifyMemberLeave(pImp, target);
		return;
	}
}

//
//Member收到Leader发来的要离开的消息，这个时候整个队伍解散
void player_link::MsgLeaderLeave(gplayer_imp *pImp, const XID & target)
{
	if(_mode != MODE_MEMBER || _leader != target){
		return;
	}

	//Leader离开了Member必然也需要离开
	CliMemberLeave(pImp, target);
}

//
//Member收到leader踢掉你的消息
void player_link::MsgKickMember(gplayer_imp *pImp, const XID & target)
{
	if(_mode != MODE_MEMBER || _leader != target){
		return;
	}

	CliKickMember(pImp, target, _seq);
}
//
//处理link stop消息
void player_link::MsgStopLinked(gplayer_imp * pImp, const XID & target)
{
	if(_mode == MODE_LEADER)
	{
		INVITE_MAP::iterator iit = invite_map.find(target.id);
		if (iit != invite_map.end()) {
			invite_map.erase(iit);	
		}

		MEMBER_MAP::iterator mit = members.find(target.id);
		if (mit != members.end()){
			members.erase(mit);
		}
		if(members.empty()){
			CliLeaderLeave(pImp);
		}		
		return ;
	}
	if (target == _leader)
	{
		if(_mode == MODE_MEMBER) 
			CliMemberLeave(pImp, target);
		else if (_mode == MODE_PREPARE)
			ClearAllState(pImp);
	}
}

void player_link::CheckInviteMap(INVITE_MAP& _map)
{
	INVITE_MAP::iterator iter;

	iter = _map.begin();
	for(; iter != _map.end();){
		if((iter->second).timestamp < g_timer.get_systime()){
			_map.erase(iter++);
		}
		else{
			++iter;
		}
	}
}

bool player_link::EnterBindLeaderState(gplayer_imp * pImp, const XID & target, char type)
{
	return pImp->EnterBindMoveState(target, type);
}

bool player_link::EnterBindMemberState(gplayer_imp * pImp, const XID & target, char type, const char seq)
{
	return pImp->EnterBindFollowState(target, type, seq);
}

bool player_link::IsValidPartner(gplayer_imp * self, const XID & partner)
{
	world::object_info info;
	return 	self->_plane->QueryObject(partner, info)
		&& (info.state & world::QUERY_OBJECT_STATE_ZOMBIE != 0) 
		&& (info.pos.squared_distance(self->_parent->pos) < 50.f*50.f);
}

//
//处理心跳
void player_link::Heartbeat(gplayer_imp * pImp)
{
	if(!_mode) return ;
	switch(_mode)
	{
		case MODE_REQUEST:
			{
				CheckInviteMap(applied_map);
				if(applied_map.empty()){
					ClearAllState(pImp);
				}
			}
			break;

		case MODE_INVITE:
			{
				CheckInviteMap(invite_map);
				if(invite_map.empty()){
					ClearAllState(pImp);
				}
			}
			break;

		case MODE_PREPARE:
			{
				_timeout --;
				if(_timeout <=0)
				{
					ClearAllState(pImp);
				}
			}
			break;

		case MODE_LEADER:
			{
				CheckInviteMap(applied_map);
				CheckInviteMap(invite_map);

				//没有超时，进行范围和位置的判断
				abase::vector<int, abase::fast_alloc<> > todel;
				MEMBER_MAP::iterator iter = members.begin();
				for(; iter != members.end(); ++iter)
				{
					if (!IsValidPartner(pImp, iter->second.id)) 
						todel.push_back(iter->second.id.id);
				}

				abase::vector<int,abase::fast_alloc<> >::iterator it = todel.begin();
				for(; it != todel.end(); ++it)
				{
					MsgMemberLeaveRequest(pImp,XID(GM_TYPE_PLAYER, *it));
				}
			}
			break;

		case MODE_MEMBER:
			{
				CheckInviteMap(applied_map);
				CheckInviteMap(invite_map);
				//没有超时，进行范围和位置的判断
				if (!IsValidPartner(pImp, _leader))
				{
					SendMessage(pImp, GM_MSG_PLAYER_LINK_LEAVE_REQUEST, _leader);
					CliMemberLeave(pImp, _leader);
				}
			}
			break;
	}	
}

//////////////////////////////////////////////////////////////////////////////////////

bool player_link_couple::IsMale(gplayer_imp * pImp)
{
	if(pImp->IsPlayerFemale())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_GENDER);
		return false;
	}
	return true;
}

bool player_link_couple::IsFemale(gplayer_imp * pImp){
	if(!pImp->IsPlayerFemale())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_GENDER);
		return false;
	}
	return true;
}


void player_link_couple::CliMsgInvite(gplayer_imp * pImp, const XID & target){
	if(!pImp->IsPlayerFemale()) return;
	pImp->_runner->player_bind_invite(target);
}

void player_link_couple::CliMsgRequest(gplayer_imp * pImp, const XID & target){
	if(pImp->IsPlayerFemale()) return;
	pImp->_runner->player_bind_request(target);
}

void player_link_couple::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param){
	pImp->_runner->player_bind_invite_reply(target,param);
}

void player_link_couple::CliMsgRequestReply(gplayer_imp * pImp, const XID & target, int param){
	pImp->_runner->player_bind_request_reply(target,param);
}

void player_link_couple::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_bind_start(target);
	//进入绑定状态
}

void player_link_couple::CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	//否则进行移动
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);
}

void player_link_couple::CliMemberLeave(gplayer_imp *pImp, const XID &target)
{
	pImp->_runner->player_bind_stop();
	ClearAllState(pImp);
}

void player_link_couple::CliLeaderLeave(gplayer_imp *pImp)
{
	pImp->_runner->player_bind_stop();
	ClearAllState(pImp);
}

bool player_link_couple::CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkRequest(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;
	if(!IsFemale(pImp)) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;
	
	return false;
}

bool player_link_couple::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;
	if(!IsMale(pImp)) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;

	return true;
}

int player_link_couple::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_BIND_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return GM_MSG_PLAYER_BIND_REQUEST;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_BIND_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return GM_MSG_PLAYER_BIND_REQ_REPLY;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return GM_MSG_PLAYER_BIND_PREPARE;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_BIND_LINK;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return GM_MSG_PLAYER_BIND_FOLLOW;
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_BIND_STOP;
			/*		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
					return GM_MSG_PLAYER_BIND_LEAVE_REQUEST;
					case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
					return GM_MSG_PLAYER_BIND_MEMBER_LEAVE;
					case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
					return GM_MSG_PLAYER_BIND_LEADER_LEAVE;		
			 */
	}
	return -1;
}

int player_link_couple::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_INVALID_BIND_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return S2C::ERR_CANNOT_BIND_HERE;
		case ERR_LINK_INVITE_FULL:
			return -1;
		case ERR_LINK_ALREADY_INVITE:
			return -1;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
void player_link_ride::CliMsgInvite(gplayer_imp * pImp, const XID & target)
{
	pImp->_runner->player_bind_ride_invite(target);
}



void player_link_ride::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	pImp->_runner->player_bind_ride_invite_reply(target,param);
}


//发送member join的广播消息给客户端	
void player_link_ride::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_bind_ride_join(target, pImp->_parent->ID, _seq);
}

//发送member follow
void player_link_ride::CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);
}

//发送member leave的广播消息给客户端
void player_link_ride::CliMemberLeave(gplayer_imp *pImp, const XID & target)
{
	pImp->_runner->player_bind_ride_leave(_leader, pImp->_parent->ID, _seq);
	ClearAllState(pImp);
}

//发送leader leave的广播消息给客户端
void player_link_ride::CliLeaderLeave(gplayer_imp *pImp)
{
	pImp->_runner->player_bind_ride_leave(_leader, _leader, _seq);	
	ClearAllState(pImp);
};


void player_link_ride::CliKickMember(gplayer_imp *pImp, const XID & leader, char pos)
{
	pImp->_runner->player_bind_ride_kick(leader, _seq);
	ClearAllState(pImp);
}

//Member死亡的时候自己先解绑， 再给leader发消息
//这样能保证动作的一致性
void player_link_ride::OnMemberDeath(gplayer_imp *pImp)
{
	//转移到正常状态
	pImp->_runner->player_bind_ride_leave(_leader, pImp->_parent->ID, _seq);
	SendMessage(pImp, GM_MSG_PLAYER_LINK_LEAVE_REQUEST, _leader);	
	ClearAllState(pImp);
}

bool player_link_ride::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	gplayer * pPlayer = pImp->GetParent();
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(!pPlayer->IsMountMode() || pImp->GetMountType() != 1) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;

	return true;
}

int player_link_ride::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_LINK_RIDE_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return -1;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_LINK_RIDE_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return -1;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return -1;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_LINK_RIDE_START;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return GM_MSG_PLAYER_LINK_RIDE_FOLLOW; 
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_LINK_RIDE_STOP;
		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
			return GM_MSG_PLAYER_LINK_RIDE_LEAVE_REQUEST;
		case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
			return GM_MSG_PLAYER_LINK_RIDE_MEMBER_LEAVE;
		case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
			return GM_MSG_PLAYER_LINK_RIDE_LEADER_LEAVE;
		case GM_MSG_PLAYER_LINK_KICK_MEMBER:
			return GM_MSG_PLAYER_LINK_RIDE_KICK_MEMBER;		
	}
	return -1;
}

int player_link_ride::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_INVALID_LINK_RIDE_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return S2C::ERR_CANNOT_LINK_RIDE_HERE;
		case ERR_LINK_INVITE_FULL:
			return S2C::ERR_LINK_RIDE_INVITE_FULL;
		case ERR_LINK_ALREADY_INVITE:
			return S2C::ERR_LINK_RIDE_ALREADY_INVITE;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}

void player_link_bath::CliMsgInvite(gplayer_imp * pImp, const XID & target){
	pImp->_runner->player_bath_invite(target);
}

void player_link_bath::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	pImp->_runner->player_bath_invite_reply(target,param);
}

void player_link_bath::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_bath_start(target);
	//进入绑定状态
}

void player_link_bath::CliLeaderLeave(gplayer_imp *pImp)
{
	pImp->_runner->player_bath_stop();
	ClearAllState(pImp);
}

void player_link_bath::CliMemberLeave(gplayer_imp *pImp, const XID &target)
{
	pImp->_runner->player_bath_stop();
	ClearAllState(pImp);
}


bool player_link_bath::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;

	world::object_info info;
	if(!pImp->_plane->QueryObject(target,info,false))
	{
		return false;
	}

	if(pImp->GetParent()->base_info.gender == info.gender)
	{
		return false;
	}
	
	return true;
}

bool player_link_bath::EnterBindLeaderState(gplayer_imp * pImp, const XID & target, char type)
{
	if(!pImp->EnterBindMoveState(target, type)) return false;

	pImp->_commander->DenyCmd(controller::CMD_MOVE);
	pImp->_commander->DenyCmd(controller::CMD_PET);
	pImp->_bind_to_ground ++;
	pImp->OnBathStart(true);

	return true;
}

bool player_link_bath::EnterBindMemberState(gplayer_imp * pImp, const XID & target, char type, const char seq)
{
	if(!pImp->EnterBindFollowState(target, type, seq)) return false;

	pImp->OnBathStart(false);
	return true;
}

int player_link_bath::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_LINK_BATH_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return -1;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_LINK_BATH_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return -1;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return -1;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_LINK_BATH_START;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return -1; 
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_LINK_BATH_STOP;
		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
			return GM_MSG_PLAYER_LINK_BATH_LEAVE_REQUEST;
		case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
			return GM_MSG_PLAYER_LINK_BATH_MEMBER_LEAVE;
		case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
			return GM_MSG_PLAYER_LINK_BATH_LEADER_LEAVE;
		case GM_MSG_PLAYER_LINK_KICK_MEMBER:
			return -1;		
	}
	return -1;
}


int player_link_bath::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_BATH_INVALID_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return -1; 
		case ERR_LINK_INVITE_FULL:
			return S2C::ERR_BATH_INVITE_FULL;
		case ERR_LINK_ALREADY_INVITE:
			return S2C::ERR_BATH_ALREADY_INVITE;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}

void player_link_qilin::CliMsgInvite(gplayer_imp * pImp, const XID & target){
	pImp->_runner->player_qilin_invite(target);
}

void player_link_qilin::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	pImp->_runner->player_qilin_invite_reply(target,param);
}

void player_link_qilin::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_qilin_start(target);
	//进入绑定状态
}

void player_link_qilin::CliLeaderLeave(gplayer_imp *pImp)
{
	gplayer* pPlayer = pImp->GetParent();
	pPlayer->bind_disconnect = false;
	pImp->_runner->player_qilin_stop();
	ClearAllState(pImp);
}

void player_link_qilin::CliMemberLeave(gplayer_imp *pImp, const XID &target)
{
	gplayer* pPlayer = pImp->GetParent();
	pPlayer->bind_disconnect = false;
	pImp->_runner->player_qilin_stop();
	ClearAllState(pImp);
}


bool player_link_qilin::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(!pImp->IsInTeam() || !pImp->IsMember(target)) return false;

	world::object_info info;
	if(!pImp->_plane->QueryObject(target,info,false))
	{
		return false;
	}

	gplayer* pPlayer = pImp->GetParent();
	if(!pImp->IsRenMa() || !pPlayer->IsQilinState()) return false;
	
	_team_id = pImp->GetTeamID(); 
	return true;
	
}

bool player_link_qilin::CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkRequest(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode())  return false;
	if(!pImp->IsInTeam() || !pImp->IsMember(target)) return false;
	if(pImp->IsRenMa()) return false;
	
	_team_id = pImp->GetTeamID(); 
	return true;
}

void player_link_qilin::PlayerLinkDisconnect(gplayer_imp * pImp)
{
	gplayer* pPlayer = pImp->GetParent();
	if(pPlayer->bind_disconnect) return;
	pImp->_commander->AllowCmd(controller::CMD_MOVE);
	pImp->_commander->AllowCmd(controller::CMD_PET);

	pPlayer->bind_disconnect = true;
	int index = gmatrix::FindPlayer(_leader.id);
	if(index >= 0)
	{
		gplayer * pPlayer = gmatrix::GetPlayerByIndex(index);
		pPlayer->bind_disconnect = true;
	}

	pImp->_runner->player_qilin_disconnect();
}

void player_link_qilin::PlayerLinkReconnect(gplayer_imp * pImp)
{
	gplayer* pPlayer = pImp->GetParent();
	if(!pPlayer->bind_disconnect) return;
	if (!IsValidPartner(pImp, _leader)) return;

	world::object_info info;
	pImp->_plane->QueryObject(_leader, info);

	if((info.state & world::QUERY_OBJECT_STATE_ZOMBIE) != 0 || (info.pos.squared_distance(pImp->_parent->pos)) > 50.f*50.f) return; 

	pImp->_commander->DenyCmd(controller::CMD_MOVE);
	pImp->_commander->DenyCmd(controller::CMD_PET);

	pPlayer->bind_disconnect = false;
	int index = gmatrix::FindPlayer(_leader.id);
	if(index >= 0)
	{
		gplayer * pPlayer = gmatrix::GetPlayerByIndex(index);
		pPlayer->bind_disconnect = false;
	}

	A3DVECTOR tmp = info.pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

//	pImp->_runner->notify_pos(info.pos);
	pImp->_runner->player_qilin_reconnect(info.pos);
}

void player_link_qilin::CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	gplayer* pPlayer = pImp->GetParent();
	if(pPlayer->bind_disconnect) return;
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);
}


int player_link_qilin::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_LINK_QILIN_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return -1;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_LINK_QILIN_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return -1;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return -1;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_LINK_QILIN_START;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return GM_MSG_PLAYER_LINK_QILIN_FOLLOW; 
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_LINK_QILIN_STOP;
		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
			return GM_MSG_PLAYER_LINK_QILIN_LEAVE_REQUEST;
		case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
			return GM_MSG_PLAYER_LINK_QILIN_MEMBER_LEAVE;
		case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
			return GM_MSG_PLAYER_LINK_QILIN_LEADER_LEAVE;
		case GM_MSG_PLAYER_LINK_KICK_MEMBER:
			return -1;		
	}
	return -1;
}

int player_link_qilin::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_QILIN_INVALID_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return -1; 
		case ERR_LINK_INVITE_FULL:
			return S2C::ERR_QILIN_INVITE_FULL;
		case ERR_LINK_ALREADY_INVITE:
			return S2C::ERR_QILIN_ALREADY_INVITE;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}


bool player_link_qilin::IsValidPartner(gplayer_imp * self, const XID & partner)
{
	if(!player_link::IsValidPartner(self, partner))  return false;
	if(!self->IsInTeam() || !self->IsMember(partner)) return false;
	return true;
}

