#include "carriernpc.h"
#include "clstab.h"
#include "template_loader.h"
#include "world.h"

DEFINE_SUBSTANCE(carrier_npc,service_npc,CLS_CARRIER_NPC_IMP)

int carrier_npc::MessageHandler(const MSG& msg)
{
	switch(msg.message)
	{
		case GM_MSG_PLAYER_ENTER_CARRIER:
		{
			if(msg.content_length != sizeof(msg_player_enter_carrier)) return 0;
			if(_player_set.find(msg.source.id) != _player_set.end()) return 0;
			npc_template* pTemplate = npc_stubs_manager::Get(GetParent()->tid);
			if(!pTemplate) return 0;

			float distance = msg.pos.horizontal_distance(_parent->pos);
			if(distance > MAX_CARRIER_RANGE * MAX_CARRIER_RANGE)
			{
			       	return 0;
			}
				
			const msg_player_enter_carrier * pMsg = (const msg_player_enter_carrier*)msg.content;
			_player_set.insert(msg.source.id);

			gnpc* pNPC = GetParent();
			msg_enter_carrier data;
			data.rpos = pMsg->rpos;
			data.rdir = pMsg->rdir;
			data.carrier_pos = pNPC->pos;
			data.carrier_dir = pNPC->dir;
			SendTo<0>(GM_MSG_ENTER_CARRIER,msg.source,0,&data,sizeof(data));
		}
		return 0;

		case GM_MSG_PLAYER_LEAVE_CARRIER:
		{
			if(!msg.source.IsPlayerClass()) return 0;
			_player_set.erase(msg.source.id);

		}
		return 0;

		default:
			break;
	}
	return service_npc::MessageHandler(msg);
}

void carrier_npc::OnHeartbeat(size_t tick)
{
	npc_template* pTemplate = npc_stubs_manager::Get(GetParent()->tid);
	if(!pTemplate || !pTemplate->npc_data) return;

	//检测交通工具玩家的有效性
	world::object_info info;
	for(std::set<int>::iterator it = _player_set.begin();it != _player_set.end();)
	{
		bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER, *it),info);
		if(!rst)
		{
			SendTo<0>(GM_MSG_LEAVE_CARRIER,XID(GM_TYPE_PLAYER, *it),0);
			_player_set.erase(it++);
			continue;
		}
		A3DVECTOR rpos = info.pos;
	        rpos -= _parent->pos;
		rpos *= 0.5;
		if(rpos.x < pTemplate->npc_data->carrier_mins[0] || rpos.x > pTemplate->npc_data->carrier_maxs[0] ||
		   rpos.y < pTemplate->npc_data->carrier_mins[1] || rpos.y > pTemplate->npc_data->carrier_maxs[1] ||
		   rpos.z < pTemplate->npc_data->carrier_mins[2] || rpos.z > pTemplate->npc_data->carrier_maxs[2] )
		{
			SendTo<0>(GM_MSG_LEAVE_CARRIER,XID(GM_TYPE_PLAYER, *it),0);
			_player_set.erase(it++);
			continue;
		}
		++it;
	}

	gnpc_imp::OnHeartbeat(tick);
}

bool carrier_npc::StepMove(const A3DVECTOR& offset)
{
	//移动时需要通知所有的船上对象
	bool bRst = gnpc_imp::StepMove(offset);
	if(bRst)
	{
		if(_player_set.size() == 0) return true;
		for(std::set<int>::iterator it = _player_set.begin();it != _player_set.end();++it)
		{
			SendTo<0>(GM_MSG_CARRIER_SYNC_POS,XID(GM_TYPE_PLAYER, *it), _parent->dir, 0, 0);
		}
	}
	return bRst;
}

