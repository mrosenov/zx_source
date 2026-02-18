#include "friendcallbackmanager.hpp"
#include "friendcallbackinfo.hpp"
#include "dbfriendcallbackinfo.hrp"
#include "friendcallbackinfo_re.hpp"
#include "friendcallbackmail.hpp"
#include "dbfriendcallbackmail.hrp"
#include "friendcallbackmail_re.hpp"
#include "dbupdatecallbackinfo.hrp"
#include "dbcheckaward.hrp"
#include "friendcallbackaward_re.hpp"
#include "friendcallbacksubscribe.hpp"
#include "dbfriendcallbacksubscribe.hrp"
#include "friendcallbacksubscribe_re.hpp"
#include "dbgetmaillist.hrp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "conv_charset.h"

namespace GNET
{
int FriendCallbackManager::_zoneoff = 0;
bool FriendCallbackManager::_isopen = false;

void FriendCallbackManager::QueryCallbackInfo(const FriendCallbackInfo *msg, unsigned int sid)
{
	if(!_isopen)
		return;

	int roleid = msg->roleid;
	int localsid = msg->localsid;
	PlayerInfo *playerinfo = NULL;
	if(!IsPlayerOnLine(roleid, playerinfo))
		return;

	if(!ExistCallbackLimit(roleid))
	{
		DBFriendCallbackInfo *rpc = (DBFriendCallbackInfo *)Rpc::Call(RPC_DBFRIENDCALLBACKINFO, DBFriendCallbackInfoArg(roleid));
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	else
	{
		ReplyCallbackInfo(ERR_CALLBACK_SUCCESS, roleid, localsid, sid);
	}
}

void FriendCallbackManager::ReplyCallbackInfo(unsigned char retcode, int roleid, unsigned int localsid, unsigned int sid)
{
	FriendCallbackInfo_Re re;
	re.retcode = retcode;
	re.roleid = roleid;
	re.localsid = localsid;
	if(ERR_CALLBACK_SUCCESS == retcode)
	{
		GetCallbackLimit(roleid, re.callback_limit);
	}
	
	//LOG_TRACE("GDelivery::ReplyCallbackInfo, retcode=%d, roleid=%d, mailnum=%d, timestamp=%d, cooldown_list.size=%d\n", retcode, roleid, re.callback_limit.mailnum, re.callback_limit.timestamp, re.callback_limit.cooldown_list.size());
	GDeliveryServer::GetInstance()->Send(sid, re);
}

void FriendCallbackManager::GetCallbackLimit(int roleid, GCallbackInfo &callback_info)
{
	Thread::Mutex::Scoped locker(_locker);
	if(_callback_limit.find(roleid) != _callback_limit.end())
	{
		const GCallbackLimit &limit = _callback_limit[roleid];
		callback_info.mailnum = limit.mailnum;
		callback_info.timestamp = limit.timestamp;
		callback_info.is_subscribe = limit.is_subscribe;

		callback_info.cooldown_list.clear();
		GCooldown cooldown;
		const map<int, int> &cooldown_map = limit.cooldown_map;
		map<int, int>::const_iterator it = cooldown_map.begin(), ie = cooldown_map.end();
		for(; it != ie; ++it)
		{
			cooldown.roleid = it->first;
			cooldown.send_time = it->second;
			callback_info.cooldown_list.push_back(cooldown);
		}
	}
	else
	{
		callback_info.mailnum = 0;
		callback_info.timestamp= 0;
		callback_info.is_subscribe = 1;
		callback_info.cooldown_list.clear();
	}
}

void FriendCallbackManager::LoadCallbackInfo(int roleid, const GCallbackInfo &callback_info)
{
	Thread::Mutex::Scoped locker(_locker);
	GCallbackLimit &limit = _callback_limit[roleid];
	limit.timestamp = callback_info.timestamp;
	limit.is_subscribe = callback_info.is_subscribe;
	// 如果最后一次发送召回邮件的时间与现在不是同一天则将该角色当天可以发送的召回邮件数置为0
	limit.mailnum = IsNDaysBefore(limit.timestamp, RESETMAILNUM_TIME) ? 0 : callback_info.mailnum;

	limit.cooldown_map.clear();
	GCooldownVector::const_iterator it = callback_info.cooldown_list.begin(), ie = callback_info.cooldown_list.end();
	for(; it != ie; ++it)
	{
		//if(!IsNDaysBefore(it->send_time, COOLDOWN_TIME))
		if(IsCooldown(it->send_time, COOLDOWN_TIME * SECONDS_PERDAY))
		{
			limit.cooldown_map[it->roleid] = it->send_time;
		}
	}
}

void FriendCallbackManager::SendCallbackMail(const FriendCallbackMail *msg, unsigned int sid)
{
	if(!_isopen)
		return;

	int sender_userid = 0;
	unsigned char retcode = CanSendCallbackMail(msg, sender_userid);
	LOG_TRACE("GDelivery::SendCallbackMail, retcode=%d, roleid=%d, tempate_id=%d\n", retcode, msg->roleid, msg->template_id);
	if(ERR_CALLBACK_OFFLINE == retcode || ERR_CALLBACK_MSGCOOLDOWN == retcode || ERR_CALLBACK_UNVALIDTEMPID == retcode)
	{
		return;
	}
	else if(ERR_CALLBACK_SUCCESS != retcode)
	{
		ReplyCallbackMail(retcode, msg->roleid, msg->localsid, msg->receiver_roleid, sid);
	}
	else
	{
		// 更新Delivery中该角色的发送限制
		UpdateCallbackLimit(msg->roleid, msg->receiver_roleid);
		// 发送召回邮件
		DBFriendCallbackMailArg arg;
		MakeMail(arg.mail, msg->roleid, msg->receiver_roleid, msg->template_id);
		arg.sender_userid = sender_userid;
		DBFriendCallbackMail *rpc = (DBFriendCallbackMail *)Rpc::Call(RPC_DBFRIENDCALLBACKMAIL, arg);
		rpc->template_id = msg->template_id;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
}

unsigned char FriendCallbackManager::CanSendCallbackMail(const FriendCallbackMail *msg, int &userid)
{
	int sender = msg->roleid;
	int receiver = msg->receiver_roleid;
	PlayerInfo *sender_info = NULL, *receiver_info = NULL;

	// 如果发送者不在线则忽略该消息
	if(!IsPlayerOnLine(sender, sender_info))
		return ERR_CALLBACK_OFFLINE;
	userid = sender_info->userid;

	// 如果发送者的好友列表未初始化则不能发送召回邮件（因为无法判断接收者是否为其好友）
	if(sender_info->friend_ver < 0)
		return ERR_CALLBACK_FDLISTNOTINIT;

	// 如果发送者召回邮件的限制信息没有加载则不能发送
	if(!ExistCallbackLimit(sender))
		return ERR_CALLBACK_LIMITNOTINIT;

	int lastsend_time = _callback_limit[sender].timestamp;
	// 发送者两次发送召回邮件消息的时间间隔不能短于3秒，否则忽略该消息
	if(IsCooldown(lastsend_time, MSG_INTERVAL))
		return ERR_CALLBACK_MSGCOOLDOWN;

	// 如果当前时间跟上次发送时间间隔了一天，则需要重置当天发送召回邮件数mailnum为0
	if(IsNDaysBefore(lastsend_time, RESETMAILNUM_TIME))
	{
		_callback_limit[sender].mailnum = 0;
	}

	// 如果发送者当天发送的召回邮件数达到5封则不能继续发送
	if(_callback_limit[sender].mailnum >= MAX_CALLBACKMAIL_PERDAY)
		return ERR_CALLBACK_MAXMAILNUM;

	// 如果接收者在线不能发送召回邮件
	if(IsPlayerOnLine(receiver, receiver_info))
		return ERR_CALLBACK_FRIENDONLINE;

	// 如果接收者不是其好友则不能发送召回邮件
	if(!IsFriend(sender_info, receiver))
		return ERR_CALLBACK_NOTHISFRIEND;

	// 玩家给同一好友发送两封召回邮件的时间间隔应超过10天，少于10天的不能发送
	if(IsReceiverInCooldown(sender, receiver))
		return ERR_CALLBACK_COOLDOWN;

	return ERR_CALLBACK_SUCCESS;
}

bool FriendCallbackManager::IsFriend(const PlayerInfo *playerinfo, int roleid)
{
	const GFriendInfoVector &friend_list = playerinfo->friends;
	GFriendInfoVector::const_iterator it = friend_list.begin(), ie = friend_list.end();
	for(; it != ie; ++it)
	{
		if(roleid == it->rid)
			return true;
	}
	return false;
}

bool FriendCallbackManager::IsReceiverInCooldown(int sender, int receiver)
{
	map<int, int> &cooldown_map = _callback_limit[sender].cooldown_map;
	if(cooldown_map.find(receiver) == cooldown_map.end())
		return false;

	const int lastsend_time = cooldown_map[receiver];
	//return IsNDaysBefore(lastsend_time, COOLDOWN_TIME) ? false : true;
	return IsCooldown(lastsend_time, COOLDOWN_TIME * SECONDS_PERDAY);
}

void FriendCallbackManager::ReplyCallbackMail(unsigned char retcode, int sender, unsigned int localsid, int receiver, unsigned int sid)
{
	FriendCallbackMail_Re re;
	re.retcode = retcode;
	re.roleid = sender;
	re.localsid = localsid;
	re.receiver_roleid = receiver;

	GDeliveryServer::GetInstance()->Send(sid, re);
}

void FriendCallbackManager::UpdateCallbackLimit(int sender, int receiver)
{
	int now = Timer::GetTime();
	Thread::Mutex::Scoped locker(_locker);

	GCallbackLimit &limit = _callback_limit[sender];
	// 记录上次给该角色发送召回邮件的时间，以便出错时进行回滚
	_rollback[sender][receiver] = limit.cooldown_map[receiver];
	// 更新delivery中保存的限制信息，以使得在与数据库交互的过程中也能正确限制召回邮件的发送
	limit.mailnum++;
	limit.timestamp = now;
	limit.cooldown_map[receiver] = now;

	LOG_TRACE("GDelivery::UpdateCallbackLimit, sender=%d, receiver=%d, mailnum=%d, send_time=%d, cooldown_list.size()=%d\n", 
		  sender, receiver, limit.mailnum, now, limit.cooldown_map.size());
}

void FriendCallbackManager::MakeMail(GMail &mail, int sender, int receiver, unsigned char template_id)
{
	PlayerInfo *sender_info = NULL;
	if(IsPlayerOnLine(sender, sender_info))
	{
		mail.header.sender_name = sender_info->name;
	}

	mail.header.id = 0;
	mail.header.sender = sender;
	mail.header.sndr_type = _MST_CALLBACK;
	mail.header.receiver = receiver;
	mail.header.send_time = Timer::GetTime();
	mail.header.attribute = (1 << _MA_UNREAD);
}

void FriendCallbackManager::Rollback(int sender, int receiver)
{
	LOG_TRACE("GDelivery::Rollback, sender=%d, receiver=%d\n", sender, receiver);

	Thread::Mutex::Scoped locker(_locker);
	// 回滚信息里面没有记录则忽略本次回滚
	if(_rollback[sender].find(receiver) == _rollback[sender].end())
		return;

	_callback_limit[sender].mailnum--;
	if(0 == _rollback[sender][receiver])
	{
		_callback_limit[sender].cooldown_map.erase(receiver);
	}
	else
	{
		_callback_limit[sender].cooldown_map[receiver] = _rollback[sender][receiver];
	}
	_rollback[sender].erase(receiver);
}

void FriendCallbackManager::PlayerLogout(int roleid)
{
	if(!_isopen)
		return;

	LOG_TRACE("GDelivery::FriendCallback PlayerLogout, roleid=%d\n", roleid);
	// 检查是否需要将当前Delivery中该角色发送召回邮件的限制信息跟数据库同步
	if(NeedSync2DB(roleid))
	{
		GCallbackInfo callback;
		GetCallbackLimit(roleid, callback);
		DBUpdateCallbackInfo *rpc = (DBUpdateCallbackInfo *)Rpc::Call(RPC_DBUPDATECALLBACKINFO, DBUpdateCallbackInfoArg(roleid, callback));
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	// 无论是否需要同步都需要将Delivery中对应玩家的限制信息删除
	_callback_limit.erase(roleid);
}

bool FriendCallbackManager::NeedSync2DB(int roleid)
{
	Thread::Mutex::Scoped locker(_locker);

	// 如果玩家不在限制列表中则表明其未使用过鸿雁传书，无需同步
	if(_callback_limit.find(roleid) == _callback_limit.end())
		return false;

	// 如果玩家在限制列表中存在但时戳为0，则表明数据库中不存在该玩家的信息且其在线过程中未发送过召回邮件
	// 对于此种情况也不需要更新
	if(0 == _callback_limit[roleid].timestamp)
		return false;

	// 检查是否存在冷却期结束的玩家，如果存在将其从限制信息中删除，使得能够继续给其发送召回邮件
	map<int, int> &cooldown_map = _callback_limit[roleid].cooldown_map;
	map<int, int>::iterator it = cooldown_map.begin();
	for(; it != cooldown_map.end(); )
	{
		//if(IsNDaysBefore(it->second, COOLDOWN_TIME))
		if(!IsCooldown(it->second, COOLDOWN_TIME * SECONDS_PERDAY))
		{
			cooldown_map.erase(it++);
		}
		else
		{
			++it;
		}
	}
	return true;
}

void FriendCallbackManager::PlayerLogin(int roleid)
{
	if(!_isopen)
		return;

	//LOG_TRACE("GDelivery::FriendCallbackManager::PlayerLogin, roleid=%d\n", roleid);
	DBCheckAward *rpc = (DBCheckAward *)Rpc::Call(RPC_DBCHECKAWARD, DBCheckAwardArg(roleid));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FriendCallbackManager::NotifyAwardInfo(int returnplayer_roleid, int award_type, const vector<int> &recaller_list)
{
	Octets name;
	int roleid = 0;
	FriendCallbackAward_Re re;
	PlayerInfo *playerinfo = NULL;

	for(unsigned int i = 0; i < (recaller_list.size() + 1); ++i)
	{
		if(0 == i)
		{
			roleid = returnplayer_roleid;
			if(!IsPlayerOnLine(roleid, playerinfo))
				return;
			re.award_type = award_type;
			re.returnplayer_name = name;
			name = playerinfo->name;
		}
		else
		{
			roleid = recaller_list[i - 1];
			if(!IsPlayerOnLine(roleid, playerinfo))
				continue;
			re.award_type = RECALL_AWARD;
			re.returnplayer_name = name;
			ReloadMaillist(roleid, playerinfo->localsid, playerinfo->linksid);
		}
		re.roleid = roleid;
		re.localsid = playerinfo->localsid;

		GDeliveryServer::GetInstance()->Send(playerinfo->linksid, re);
	}
}

void FriendCallbackManager::ReloadMaillist(int roleid, int localsid, int sid)
{
	DBGetMailList *rpc = (DBGetMailList *)Rpc::Call(RPC_DBGETMAILLIST, RoleId(roleid));
	rpc->save_linksid = sid;
	rpc->save_localsid = localsid;
	rpc->need_send2client = false;
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FriendCallbackManager::Subscribe(const FriendCallbackSubscribe *msg, unsigned int sid)
{
	if(!_isopen)
		return;

	DBFriendCallbackSubscribe *rpc = (DBFriendCallbackSubscribe *)Rpc::Call(RPC_DBFRIENDCALLBACKSUBSCRIBE, DBFriendCallbackSubscribeArg(msg->roleid, msg->is_subscribe));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FriendCallbackManager::ReplySubscribeResult(unsigned char retcode, char is_subscribe, int roleid, unsigned int localsid, unsigned int sid)
{
	if(ERR_CALLBACK_SUCCESS == retcode)
	{
		_callback_limit[roleid].is_subscribe = is_subscribe;
	}
	FriendCallbackSubscribe_Re re;
	re.retcode = retcode;
	re.roleid = roleid;
	re.localsid = localsid;
	GDeliveryServer::GetInstance()->Send(sid, re);
}

};
