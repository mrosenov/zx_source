#include "summonman.h"
#include "summonnpc.h"
#include "player_imp.h"


//玩家给所有召唤怪发送消息
void summon_manager::PlayerSendAllMessage(gplayer_imp *pImp, int msg_id, int param, const void * content,size_t content_length)
{
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		MSG msg;
		BuildMessage(msg, msg_id ,XID(GM_TYPE_NPC, _summon_list[i].id),
				pImp->GetParent()->ID, pImp->GetParent()->pos, param, content, content_length);
		gmatrix::SendMessage(msg);

	}
}

//给某个召唤怪发送消息
void summon_manager::PlayerSendMessageTo(gplayer_imp *pImp, int msg_id, int id, int param, const void * content,size_t content_length)
{
	ASSERT(msg_id);
	MSG msg;
	BuildMessage(msg,msg_id,XID(GM_TYPE_NPC, id),
			pImp->GetParent()->ID, pImp->GetParent()->pos, param, content, content_length);
	gmatrix::SendMessage(msg);
}

bool summon_manager::IsCloneExist()
{
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		if(_summon_list[i].type == SUMMON_TYPE_CLONE) return true;
	}
	
	return false;
}

bool summon_manager::IsSummonExist(gplayer_imp* pImp, int tid, XID& id)
{
	object_interface obj(pImp);
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		if(_summon_list[i].tid == tid)
		{
			XID who(GM_TYPE_NPC,_summon_list[i].id);
			world::object_info info;
			if(!pImp->_plane->QueryObject(who,info)) 
			{
				continue;
			}
			if(info.state &  world::QUERY_OBJECT_STATE_ZOMBIE)
			{
				continue;
			}
			id = who;
			return true;
		}
	}
	
	return false;
}

XID summon_manager::GetCloneID()
{
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		if(_summon_list[i].type == SUMMON_TYPE_CLONE) return XID(GM_TYPE_NPC, _summon_list[i].id);
	}
	
	return XID(-1,-1);
}

XID summon_manager::GetSummonID()
{
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		if(_summon_list[i].type == SUMMON_TYPE_ANIMAL) return XID(GM_TYPE_NPC, _summon_list[i].id);
	}
	
	return XID(-1,-1);
}

void summon_manager::UpdateCloneStatus(gplayer_imp *pImp)
{
	gplayer *pPlayer = pImp->GetParent();
	if(!_clone_exist_flag && IsCloneExist())
	{
		_clone_exist_flag = true;
		pPlayer->SetExtraState(gplayer::STATE_CLONE);
		pImp->_skill.EventCloneExist(pImp, 1);
		property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
	}
	else if(_clone_exist_flag && !IsCloneExist())
	{
		_clone_exist_flag = false;
		pPlayer->ClrExtraState(gplayer::STATE_CLONE);
		pImp->_skill.EventCloneExist(pImp, 0);
		property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
		RemoveCloneFilter(pImp);
	}
	
}

void summon_manager::RemoveCloneFilter(gplayer_imp *pImp)
{
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_SHARELIFEWITHCLONE))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_SHARELIFEWITHCLONE);
	}
	
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_SHARESTATEWITHCLONE))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_SHARESTATEWITHCLONE);
	}
}


//检查是否能够召唤怪
bool summon_manager::TestCanSummonNPC(gplayer_imp* pImp,A3DVECTOR& pos)
{
	if(!summon_manager::FindGroundPos(pImp->GetWorldManager(),pos))
	{
		//召唤失败,因为没有可以立足的地点
		return false;
	}

	return true;
}

void summon_manager::InsertSummonNPC(int id, char type, int tid, int timestamp, bool is_invisible, A3DVECTOR& pos, int world_id)
{
	summon_data data;
	data.id = id;
	data.type = type;
	data.tid = tid;
 	data.summon_stamp = timestamp;
	data.heartbeat_stamp = timestamp;
 	data.cur_notify_counter = 0;
	data.is_invisible = is_invisible;
	data.pos = pos;
	data.world_id = world_id;

	_summon_list.push_back(data);
}


//杀死前面已经召唤的
void summon_manager::ClearSummonNPCByType(gplayer_imp *pImp, char type)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;

	for(iter = _summon_list.begin(); iter != _summon_list.end();)
	{
		if(iter->type == type)
		{
			PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, iter->id);
			iter = _summon_list.erase(iter);
		}
		else
		{
			++iter;
		}

	}

	UpdateCloneStatus(pImp);
}

void summon_manager::PreSummonNPC(gplayer_imp * pImp, char type, int tid, int count, int max_count)
{
	if(type == SUMMON_TYPE_ANIMAL || type == SUMMON_TYPE_PLANT || type == SUMMON_TYPE_CLONE )
	{
		ClearSummonNPCByType(pImp, type);
	}
	else if(type == SUMMON_TYPE_MACHINE || type == SUMMON_TYPE_TRAP) 
	{
		int cur_count = GetSummonNPCCountByID(tid);
		if(cur_count + count > max_count)
		{
			ClearSummonNPCByID(pImp, tid, cur_count + count - max_count);
		}
	}
	else if(type == SUMMON_TYPE_CONTROL_TRAP)
	{
		ClearSummonNPCByType(pImp, type);
	}
	else if(type == SUMMON_TYPE_TELEPORT1 || type == SUMMON_TYPE_TELEPORT2)
	{
		int cur_count = GetSummonNPCCountByID(tid);
		if(cur_count >= 2)
		{
			ClearSummonNPCByID(pImp, tid, cur_count -1);
		}
	}
}

int summon_manager::GetSummonNPCCountByType(char type)
{
	int count = 0;
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		if(_summon_list[i].type == type) count++;
	}
	return count;
}

int summon_manager::GetSummonNPCCountByID(int tid)
{
	int count = 0;
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		if(_summon_list[i].tid == tid) count++;
	}
	return count;
}

void summon_manager::ClearSummonNPCByType(gplayer_imp * pImp, char type, int count)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	for(iter = _summon_list.begin(); iter != _summon_list.end();)
	{
		if(iter->type == type)
		{
			PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, iter->id);
			iter = _summon_list.erase(iter);
			if(--count <= 0) return;
		}
		else
		{
			++iter;
		}
	}
}

void summon_manager::ClearSummonNPCByID(gplayer_imp * pImp, int tid, int count)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	for(iter = _summon_list.begin(); iter != _summon_list.end();)
	{
		if(iter->tid == tid)
		{
			PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, iter->id);
			iter = _summon_list.erase(iter);
			if(--count <= 0) return;
		}
		else
		{
			++iter;
		}
	}
}

void summon_manager::ClearAllSummonNPC(gplayer_imp * pImp)
{
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, _summon_list[i].id);
	}
	_summon_list.clear();
}

void summon_manager::ControlTrap(gplayer_imp * pImp, int id, int tid)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	for(iter = _summon_list.begin(); iter != _summon_list.end();++iter)
	{
		if(iter->id == id && iter->tid == tid)
		{
			PlayerSendMessageTo(pImp, GM_MSG_CONTROL_TRAP, id);
			iter = _summon_list.erase(iter);
			return;
		}
	}
}


void summon_manager::RemoveCloneNPC(gplayer_imp *pImp)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	for(iter = _summon_list.begin(); iter != _summon_list.end();)
	{
		if(iter->type == SUMMON_TYPE_CLONE)
		{
			PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, iter->id);
			iter = _summon_list.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	UpdateCloneStatus(pImp);
}

void summon_manager::PostSummonNPC(gplayer_imp * pImp, char type, int id, int tid, int lifetime, int skill_id, int skill_cd, A3DVECTOR & pos)
{
	if(type == summon_manager::SUMMON_TYPE_CONTROL_TRAP)
	{
		pImp->_runner->control_trap_info(id, tid, lifetime); 
	}
	if(type == summon_manager::SUMMON_TYPE_TELEPORT1 || type == summon_manager::SUMMON_TYPE_TELEPORT2)
	{
		int count =  GetSummonNPCCountByID(tid);
		if(count == 2)
		{
			for(size_t i = 0; i < _summon_list.size(); ++i)
			{
				if(_summon_list[i].tid == tid && _summon_list[i].world_id == pImp->GetWorldTag())
				{
					//发送给第一个传送
					msg_npc_teleport_info mnti;
					mnti.npc_id = id;
					mnti.pos = pos;
					mnti.lifetime = lifetime;
					PlayerSendMessageTo(pImp, GM_MSG_NPC_TELEPORT_INFO, _summon_list[i].id, skill_id, &mnti, sizeof(mnti)); 


					//发送给当前传送
					mnti.npc_id = _summon_list[i].id;
					mnti.pos = _summon_list[i].pos;
					mnti.lifetime = lifetime;
					PlayerSendMessageTo(pImp, GM_MSG_NPC_TELEPORT_INFO, id, skill_id, &mnti, sizeof(mnti)); 

					pImp->SetCoolDown(SKILL_COOLINGID_BEGIN + skill_id, skill_cd);
					return;
				}
			}
		}
		else if(count == 1)
		{
			pImp->_runner->teleport_skill_info(pImp->GetParent()->ID.id, tid, skill_id, 1);
		}
	}
}


void summon_manager::Heartbeat(gplayer_imp *pImp)
{
	if(_summon_list.empty()) return;

	++_heartbeat_counter;
	if(_heartbeat_counter % 60 == 0)
	{
		CheckSummonStatus(pImp);
	}
	CheckNotifyCounter(pImp);

}


void summon_manager::CheckNotifyCounter(gplayer_imp *pImp)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;

	for(iter = _summon_list.begin(); iter != _summon_list.end(); ++iter)
	{
		++iter->cur_notify_counter;
		if(iter->cur_notify_counter > NOTIFY_MASTER_TIME)
		{
			leader_prop data;
			pImp->SetLeaderData(data);
			PlayerSendMessageTo(pImp, GM_MSG_MASTER_INFO, iter->id, iter->summon_stamp, &data, sizeof(leader_prop));
			iter->cur_notify_counter = 0;
		}
	}
}

bool summon_manager::SummonHeartbeat(gplayer_imp * pImp, const XID & who, int type)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	for(iter = _summon_list.begin(); iter != _summon_list.end(); ++iter)
	{
		if(iter->id == who.id && iter->type == type)
		{
			int timestamp = g_timer.get_systime();
			iter->heartbeat_stamp = timestamp; 
			return true;
		}
	}
	return false;
}

void summon_manager::CheckSummonStatus(gplayer_imp* pImp)
{
	int timestamp = g_timer.get_systime();
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;

	for(iter = _summon_list.begin(); iter != _summon_list.end();)
	{
		if(timestamp - iter->heartbeat_stamp > 60)
		{
			PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, iter->id);
			iter = _summon_list.erase(iter);

		}
		else
		{
			++iter;
		}
	}
	UpdateCloneStatus(pImp);
}


void summon_manager::PlayerBeAttacked(gplayer_imp* pImp,const XID& attacker)
{
	if(_summon_list.empty()) return;

	PlayerSendAllMessage(pImp, GM_MSG_MASTER_ASK_HELP, 0, &attacker, sizeof(attacker));
}

void summon_manager::NotifyStartAttack(gplayer_imp* pImp,const XID& target,char force_attack)
{
	if(_summon_list.empty()) return;

	PlayerSendAllMessage(pImp, GM_MSG_MASTER_START_ATTACK, force_attack, &target, sizeof(target));
}

void summon_manager::NotifyDuelStop(gplayer_imp* pImp)
{
	if(_summon_list.empty()) return;

	PlayerSendAllMessage(pImp, GM_MSG_MASTER_DUEL_STOP, 0); 
}

void summon_manager::NotifyMasterInfo(gplayer_imp* pImp, leader_prop & data)
{
	if(_summon_list.empty()) return;

	PlayerSendAllMessage(pImp, GM_MSG_MASTER_INFO, 0, &data, sizeof(data));
	UpdateNotifyCounter(0);
}

void summon_manager::UpdateNotifyCounter(int counter)
{
	for(size_t i = 0; i < _summon_list.size(); ++i)
	{
		_summon_list[i].cur_notify_counter = counter; 
	}
}

bool summon_manager::SummonNPCDeath(gplayer_imp* pImp,const XID& who, int type, int summon_stamp)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	for(iter = _summon_list.begin(); iter != _summon_list.end(); ++iter)
	{
		if(iter->id == who.id && iter->type == type && iter->summon_stamp == summon_stamp)
		{
			_summon_list.erase(iter);
			UpdateCloneStatus(pImp);
			PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, who.id);

			if(type == SUMMON_TYPE_TELEPORT1 || type == SUMMON_TYPE_TELEPORT2)
			{
				ClearSummonNPCByType(pImp, type);
			}
			else if(type == SUMMON_TYPE_CONTROL_TRAP)
			{
				pImp->_runner->control_trap_info(0, 0, 0);
			}	
			return true;
		}
	}

	return false;

}

bool 
summon_manager::SummonRelocatePos(gplayer_imp* pImp,const XID& who, int stamp,bool force_disappear)
{
	abase::vector<summon_data, abase::fast_alloc<> >::iterator iter;
	A3DVECTOR pos = pImp->_parent->pos;
	for(iter = _summon_list.begin(); iter != _summon_list.end(); ++iter)
	{
		if(iter->id == who.id && iter->summon_stamp == stamp)
		{
			if(force_disappear || !summon_manager::FindGroundPos(pImp->GetWorldManager(),pos))
			{
				_summon_list.erase(iter);
				UpdateCloneStatus(pImp);
				PlayerSendMessageTo(pImp, GM_MSG_NPC_DISAPPEAR, who.id);
				return true;
			}
			else
			{
				PlayerSendMessageTo(pImp, GM_MSG_SUMMON_CHANGE_POS, who.id, stamp, &pos, sizeof(pos));
				return true;
			}
		}
	}
	return false;
}


//初始化数据
bool summon_manager::InitFromCaster(gsummon_imp * sImp, gplayer_imp * pImp, int id, int skill_level, int index)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_SUMMON);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false;
	}
	ASSERT(sImp != NULL && pImp != NULL);
	
	lua_getfield(L, LUA_GLOBALSINDEX, "Summon_Init_Entrance");
	lua_pushinteger(L, id);
	lua_pushinteger(L, skill_level);
	lua_pushlightuserdata(L,(void*)pImp);
	lua_pushlightuserdata(L,(void*)sImp);
	lua_pushinteger(L, index);

	int ret = lua_pcall(L,5,1,0);
	
	if(ret)
	{
		lua_pop(L,1);
		return false;
	}

	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		lua_pop(L,1);
		return false;
	}

	lua_pop(L,1);
	return true;
}

bool summon_manager::FindGroundPos(world_manager* manager,A3DVECTOR& pos)
{
	size_t i = 0;
	for(i = 0;i < 10;++i)
	{
		A3DVECTOR t = pos;
		float offsetx = abase::Rand(1.f,4.f);
		float offsetz = abase::Rand(1.f,4.f);
		t.x += abase::Rand(0,1)?offsetx:-offsetx;
		t.z += abase::Rand(0,1)?offsetz:-offsetz;
		if(!path_finding::GetValidPos(manager->GetMoveMap(), t)) continue;
		t.y = manager->GetTerrain().GetHeightAt(t.x,t.z);
		if(fabs(t.y - pos.y) >= 6.8f)
		{
			//高度差过大，重新寻找
			continue;
		}
		pos = t;
		return true;
	}
	return false;
}

void summon_manager::OnLeaveMap(gplayer_imp * pImp)
{
	ClearAllSummonNPC(pImp);
}


void summon_manager::OnLeaveWorld(gplayer_imp * pImp)
{
	ClearAllSummonNPC(pImp);
}

