#include "global_manager.h"
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include <glog.h>

int
global_world_message_handler::HandleMessage(const MSG & msg)
{
	//有些消息操作可能会比较费时间，是否可以考虑Task完成，不过用线程的话就要考虑msg的数据问题了。

	//考虑将一些内容移动到基类中
	switch(msg.message)
	{
		case GM_MSG_PLANE_SWITCH_REQUEST:
		//确定切换请求 
		//直接返回反馈 可能需要检查是否可以传送等
		//检查是否有剩余空间
		
		if((size_t)(gmatrix::GetPlayerAlloced() + 10) < gmatrix::GetMaxPlayerCount())
		{
			//检查坐标是否满足
			if(_plane->PosInWorld(msg.pos))
			{
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				gmatrix::SendMessage(nmsg);
			}
		}
		else
		{
			//通知客户端无法进入
			MSG nmsg;
			BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,S2C::ERR_TOO_MANY_PLAYER_IN_INSTANCE);
			gmatrix::SendMessage(nmsg);
		}
		break;

		default:
			return world_message_handler::HandleMessage(msg);
	}
	return 0;
}

