#include "playeractiveemote.h"
#include "world.h"
#include "player_imp.h"

bool
player_active_emote::CheckPlayerActiveEmoteInvite(gplayer_imp *pImp)
{
	if (!pImp->CheckPlayerActiveEmoteInvite()) return false;
	if (pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;

	return true;
}

void
player_active_emote::PlayerActiveEmoteInvite(gplayer_imp *pImp, const XID & target, int active_emote_type)
{
	if (pImp->GetShape() != 0)
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_ACTIVE_EMOTE_INVITE);
		return;
	}

	if (!CheckPlayerActiveEmoteInvite(pImp)) //
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_ACTIVE_EMOTE_INVITE);
		return;
	}

	//检查性别(禁止同性别)
	short invite_gender = pImp->GetParent()->base_info.gender;
	world::object_info info;
	bool rst = pImp->_plane->QueryObject(target,info);
	if (!rst || invite_gender == info.gender)
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_ACTIVE_EMOTE_INVITE);
		return;
	}

	if (_mode == MODE_ACTIVE_EMOTE_LINKED) //已经在互动中
	{
			return;
	}

	if (_mode == MODE_ACTIVE_EMOTE_INVITE)
	{
		if (target == _invite_target && _active_emote_type == active_emote_type)
			return;
	}

	_mode = MODE_ACTIVE_EMOTE_INVITE;
	_invite_target = target;
	_active_emote_type = active_emote_type;
	_timeout = ACTIVE_EMOTE_INVITE_TIMEOUT;

	pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_INVITE, target, _active_emote_type);
}

void
player_active_emote::PlayerActiveEmoteInvReply(gplayer_imp *pImp, const XID & target, int type, int param)
{
	if (pImp->GetShape() != 0)
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_ACTIVE_EMOTE_INVITE);
		return;
	}

	if (!CheckPlayerActiveEmoteInvite(pImp) || !target.IsPlayer())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_ACTIVE_EMOTE_INVITE);
		return;
	}
	
	if (_mode && _mode != MODE_ACTIVE_EMOTE_INVITE)
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_ACTIVE_EMOTE_INVITE);
		return;
	}

	_mode = MODE_ACTIVE_EMOTE_PREPARE;
	_active_emote_target = target;
	_timeout = ACTIVE_EMOTE_PREPARE_TIMEOUT;

	pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_INV_REPLY,target,param,&type,sizeof(type));
}

void
player_active_emote::CliMsgInvite(gplayer_imp *pImp, const XID & target, int active_emote_type)
{
	if (_mode == MODE_ACTIVE_EMOTE_LINKED)
	{
		pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_INV_REPLY,target,ACTIVE_EMOTE_CANNOT);
		return;
	}

	pImp->_runner->player_active_emote_invite(target,active_emote_type);
}

void
player_active_emote::CliMsgInviteReply(gplayer_imp *pImp, const XID &target,int type, int param)
{
	if (target != _invite_target || _mode != MODE_ACTIVE_EMOTE_INVITE || _active_emote_type != type)
	{
		if (!param) pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_STOP,target,0);
		return;
	}
	if (param != ACTIVE_EMOTE_ACCEPT)
	{
		pImp->_runner->player_active_emote_invite_reply(target,param);
		Reset();
		return;
	}

	_mode = MODE_ACTIVE_EMOTE_LINKED;
	_active_emote_target = target;
	_active_emote_type = type;

	pImp->SetActiveEmoteState(target,_active_emote_type,1);

	pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_LINK,target,_active_emote_type);
}

void 
player_active_emote::CliMsgStartActiveEmote(gplayer_imp *pImp, const XID & target, int type, const A3DVECTOR & pos)
{
	if (_mode != MODE_ACTIVE_EMOTE_PREPARE || _active_emote_target != target || !CheckPlayerActiveEmoteInvite(pImp))
	{
		pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_STOP,target,0);
		return;
	}
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	_active_emote_target = target;
	_active_emote_type = type;
	_mode = MODE_ACTIVE_EMOTE_LINKED;

	pImp->SetActiveEmoteState(target,type,0);
	//进入互动状态
	pImp->_runner->player_active_emote_start(target, _active_emote_type);
}

void 
player_active_emote::PlayerActiveEmoteCancel(gplayer_imp *pImp)
{

	if (_mode != MODE_ACTIVE_EMOTE_LINKED) return;
	//取消互动表情
	pImp->SendTo<0>(GM_MSG_PLAYER_ACTIVE_EMOTE_STOP, _active_emote_target,0);
	pImp->ClrActiveEmoteState();
	Reset();

}

void
player_active_emote::CliMsgStopActiveEmoteStop(gplayer_imp *pImp, const XID& target)
{
	if ((_mode == MODE_ACTIVE_EMOTE_LINKED) && target == _active_emote_target)
	{
		pImp->_runner->player_active_emote_stop(target,_active_emote_type);
		pImp->ClrActiveEmoteState();
		if (pImp->_cur_session)
		{
			pImp->EndCurSession();
			pImp->StartSession();
		}
	}
}

void
player_active_emote::Heartbeat(gplayer_imp * pImp)
{
	if (!_mode) return;
	switch(_mode)
	{
		case MODE_ACTIVE_EMOTE_INVITE:
		_timeout--;
		if (_timeout <= 0)
		{
			Reset();
		}
		break;

		case MODE_ACTIVE_EMOTE_PREPARE:
		_timeout--;
		if (_timeout <= 0)
		{
			Reset();
		}

		case MODE_ACTIVE_EMOTE_LINKED:
		{
			//没有超时，进行范围和位置的判断
			world::object_info info;
			if(!pImp->_plane->QueryObject(_active_emote_target,info)
					|| (info.state & world::QUERY_OBJECT_STATE_ZOMBIE)
					|| (info.pos.squared_distance(pImp->_parent->pos)) >= 50.f*50.f)
			{
				//如果查询不到对象,或者距离过远 则主动退出结束绑定
				pImp->ClrActiveEmoteState();
				pImp->_runner->player_active_emote_stop(_active_emote_target,_active_emote_type);
				Reset();
			}
		}
		break;

	}
}
