#ifndef __ONLINE_GAME_GS_PLAYER_ACTIVE_EMOTE_H__
#define __ONLINE_GAME_GS_PLAYER_ACTIVE_EMOTE_H__

#include <common/types.h>

class gplayer_imp;

class player_active_emote
{
	XID _invite_target;

	XID _active_emote_target;

	int _mode;

	int _timeout;

	int _active_emote_type;

public:
	enum
	{
		HUG_EMOTE, //拥抱待机
		HAND_EMOTE, //牵手待机
		RAMBLE_EMOTE,//漫步待机
		CARRIER_EMOTE,//背
		PROPOSE_EMOTE,//求婚
		KISS_EMOTE,//亲吻
		CUIBEI_EMOTE, //捶背
	};

	enum
	{
		MODE_ACTIVE_EMOTE_EMPTY,
		MODE_ACTIVE_EMOTE_INVITE,
		MODE_ACTIVE_EMOTE_PREPARE,
		MODE_ACTIVE_EMOTE_LINKED,
	};

	enum
	{
		ACTIVE_EMOTE_INVITE_TIMEOUT = 30,
		ACTIVE_EMOTE_PREPARE_TIMEOUT = 5,
	};
	
	enum
	{
		ACTIVE_EMOTE_ACCEPT, //接收
		ACTIVE_EMOTE_REFUSE, //拒绝
		ACTIVE_EMOTE_CANNOT, //不能发起
	};

public:
	player_active_emote():_invite_target(-1,-1),
	_active_emote_target(-1,-1),_mode(0),_timeout(0),_active_emote_type(-1)
	{}

	void Swap(player_active_emote & rhs)
	{
		player_active_emote tmp = rhs;
		rhs = *this;
		*this = tmp;
	}

	void Reset()
	{
		_invite_target = XID(-1,-1);
		_active_emote_target = XID(-1,-1);
		_mode = MODE_ACTIVE_EMOTE_EMPTY;
		_timeout = 0;
		_active_emote_type = -1;
	}

	bool HasActiveEmote()
	{
		return _active_emote_target != XID(-1,-1);
	}

	bool CheckPlayerActiveEmoteInvite(gplayer_imp *pImp);
	void PlayerActiveEmoteInvite(gplayer_imp *pImp, const XID & target,int type);
	void PlayerActiveEmoteInvReply(gplayer_imp *pImp, const XID & target, int type,int param);
	void PlayerActiveEmoteCancel(gplayer_imp *pImp);

	void CliMsgInvite(gplayer_imp *pImp, const XID & target, int type);
	void CliMsgInviteReply(gplayer_imp *pImp, const XID & target, int type, int param);
	void CliMsgStartActiveEmote(gplayer_imp *pImp, const XID & target,int type, const A3DVECTOR & pos);
	void CliMsgStopActiveEmoteStop(gplayer_imp *pImp, const XID & target);
	
	void Heartbeat(gplayer_imp * pImp);
};

#endif
