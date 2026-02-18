#ifndef __ONLINE_GAME_GS_CMD_PREPARE_H__
#define __ONLINE_GAME_GS_CMD_PREPARE_H__
#include "player_imp.h"
#include "forbid_manager.h"

class player_cmd_dispatcher
{
	class exec_cmd
	{
		public:
		virtual ~exec_cmd() {}
		virtual int Dispatch(gplayer_controller * pCtrl, int cmd_type,const void * buf,size_t size) const = 0;
	};
	class exec_cmd1: public exec_cmd
	{
		public:
		virtual int Dispatch(gplayer_controller * pCtrl, int cmd_type,const void * buf,size_t size) const
		{
			return pCtrl->UnLockInventoryHandler(cmd_type, buf, size);
		}
	};
	class exec_cmd2: public exec_cmd
	{
		public:
		virtual int Dispatch(gplayer_controller * pCtrl, int cmd_type,const void * buf,size_t size) const
		{
			return pCtrl->InvalidCommandHandler(cmd_type, buf, size);
		}
	};

	class exec_cmd3: public exec_cmd
	{
		public:
		virtual int Dispatch(gplayer_controller * pCtrl, int cmd_type,const void * buf,size_t size) const
		{
			return pCtrl->DebugCommandHandler(cmd_type, buf, size);
		}
	};
	
	struct cmd_define
	{
		unsigned int state_mask;
		unsigned int exclude_state_mask;
		unsigned int spec_state;
		exec_cmd * spec_handler;
	};

private:	
	abase::vector<cmd_define> _standard_cmd;
	bool InitCommandList();
public:
	player_cmd_dispatcher()
	{
		InitCommandList();
	}
	
	inline int DispatchCommand(gplayer_imp * pImp, gplayer_controller * pCtrl, size_t cmd_type, const void* buf, size_t size)
	{
		if(cmd_type >= _standard_cmd.size())
		{
			return pCtrl->GMCommandHandler(cmd_type, buf, size);
		}

		if(forbid_manager::IsForbidProtocol(cmd_type))
		{
			pImp->_runner->error_message(S2C::ERR_FORBID_PROTOCOL);
			return 0;
		}

		const cmd_define & cmd = _standard_cmd[cmd_type];
		unsigned cur_state = (1 << pImp->GetPlayerState());
		if(pImp->IsDead()) cur_state |= 1 << player_state::STATE_DEAD;
		cur_state |= pImp->GetIdlePlayerState();

		if(!(cur_state & cmd.exclude_state_mask))
		{
			if(cur_state & cmd.state_mask) 
			{
				return pCtrl->CommandHandler(cmd_type, buf, size);
			}
		}

		if(cur_state & cmd.spec_state)
		{
			return cmd.spec_handler->Dispatch(pCtrl, cmd_type, buf, size);
		}
		__PRINTF("目前不能执行命令 %d\n",cmd_type);
		return 0;
	}
};
	
#endif

