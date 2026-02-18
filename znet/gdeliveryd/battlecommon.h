#ifndef _BATTLE_COMMON_DEF_H_INCLUDE__
#define _BATTLE_COMMON_DEF_H_INCLUDE__

#include "gbattlerole"

namespace GNET
{
	enum TEAM
	{
		TEAM_ATTACKER	= 1,
		TEAM_DEFENDER 	= 2,
		TEAM_LOOKER 	= 3,
		TEAM_MELEE		= 4,

		TEAM_NONE       = 0,
		TEAM_RED        = 1,
		TEAM_BLUE       = 2,
	};
	enum
	{
		QUEUE_MAX = 1,
	};
	enum
	{
		ONE_MINUTE = 60,
		UPDATE_INTERVAL = 10, 
	};
	enum BATTLE_STATUS
	{
		BS_READY = 0,           //服务器准备完毕等待delivery命令
		BS_PRPARING =1,         //战斗准备状态
		BS_RUNNING = 2,         //战斗开始状态
		BS_CLOSING = 3,         //战斗关闭,清理怪物等操作进行中
		BS_SENDSTART= 4,
		BS_WAITRUNNING = 5,
		
	};

	enum ROLE_STATUS
	{
		ROLE_QUEUEING = 0,
		ROLE_FIGHTING = 1,
	};
	class BattleRole : public GBattleRole
	{
	public:
		BattleRole(int _roleid, Octets & _name, unsigned char _level,
			unsigned char _occupation, int _score, int _reborn_cnt, int _localsid, int _linksid, int _src_zoneid)
			: GBattleRole(_roleid, _name, _level, _reborn_cnt, _occupation, _score, 0, _src_zoneid)
			, localsid(_localsid), linksid(_linksid), role_status(ROLE_QUEUEING),is_reenter(false)
		{
			jointime = 0;
		}

		BattleRole()
			: localsid(0), linksid(0)
		{
			jointime = 0;
		}
		int localsid;
		int linksid;
		int jointime;
		char role_status;
		bool is_reenter;
	};	
	struct  TagInfo
	{
		TagInfo(int _tag)
			: tag(_tag), status(0), timeout(0)
		{
		}
		TagInfo() : tag(-1), status(0), timeout(0)
		{
		}
		void 	SetTimeout(time_t now, int _timeout) 
		{ 
			timeout = now + _timeout; 
		}
		bool 	IsTimeout(time_t now) const 
		{ 
			if (timeout)
				return now > timeout;
			return false;
		}
		void 	ClearTimeout() 
		{
			timeout = 0;
		}
		void 	SetStatus(char st) 
		{	
			status = st;
		}
		char 	GetStatus() 
		{ 
			return status; 
		}

		int tag;
		char status;
		time_t timeout;
	};



};
#endif // _BATTLE_COMMON_DEF_H_INCLUDE__
