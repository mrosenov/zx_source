#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>

#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include "userlogin.h"
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>
#include "trade.h"
#include "task/taskman.h"
#include "playerstall.h"
#include "pvplimit_filter.h"
#include <glog.h>
#include "pathfinding/pathfinding.h"
#include "player_mode.h"
#include "cooldowncfg.h"
#include "template/globaldataman.h"
#include "petnpc.h"
#include "item_manager.h"
#include "netmsg.h"
#include "playertransform.h"
#include "raid/raid_world_manager.h"
#include "facbase_mall.h"

DEFINE_SUBSTANCE(gplayer_dispatcher,dispatcher,CLS_PLAYER_DISPATCHER)
enum
{	//暂时的
	MIN_SEND_COUNT = 128
};

void 
gplayer_dispatcher::begin_transfer()
{
	ASSERT(_mw.size() == 0);
	ASSERT(_nw.size() == 0);
	ASSERT(_pw.size() == 0);
	ASSERT(_self.size() == 0);
	ASSERT(_leave_list.size() == 0);
}

void 
gplayer_dispatcher::end_transfer()
{
	if(!_self.empty()){
		send_ls_msg(_header,_self.data(),_self.size());
		_self.clear();
	}
	wrapper_test<0>(_pw,_header,S2C::PLAYER_INFO_1_LIST);
	wrapper_test<0>(_mw,_header,S2C::MATTER_INFO_LIST);
	wrapper_test<0>(_nw,_header,S2C::NPC_INFO_LIST);
	if(_leave_list.size())
	{
		_tbuf.clear();
		using namespace S2C;
		CMD::Make<CMD::OOS_list>::From(_tbuf,_leave_list.size(), _leave_list.begin());
		send_ls_msg(_header,_tbuf.data(),_tbuf.size());
		_leave_list.clear();
	}
	
}

void 
gplayer_dispatcher::enter_slice(slice *  pPiece,const A3DVECTOR &pos)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_enter_slice>::From(_tbuf,(gplayer*)_imp->_parent,pos);

	cs_user_map map;
	pPiece->Lock();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(pPlayer->IsInvisible())
		gather_slice_cs_user(pPiece, map,  pPlayer);
	else
		gather_slice_cs_user(pPiece,map);
	pPiece->Unlock();
	
	multi_send_ls_msg(map,_tbuf,_imp->_parent->ID.id);

	//取得新区域数据
	get_slice_info(pPiece,_nw,_mw,_pw,pPlayer->anti_invisible_rate, pPlayer->team_id);
}

void 
gplayer_dispatcher::leave_slice(slice * pPiece,const A3DVECTOR &pos)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::leave_slice>::From(_tbuf,_imp->_parent);
	//发送离开消息
	cs_user_map map;
	pPiece->Lock();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(pPlayer->IsInvisible())
		gather_slice_cs_user(pPiece, map,  pPlayer);
	else
		gather_slice_cs_user(pPiece,map);

	gather_slice_object(pPiece,_leave_list,pPlayer->anti_invisible_rate, -1, 0);
	pPiece->Unlock();
	
	multi_send_ls_msg(map,_tbuf,_imp->_parent->ID.id);
}

void 
gplayer_dispatcher::start_attack(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	gplayer * pObj = (gplayer*)pImp->_parent;
	CMD::Make<CMD::object_start_attack>::From(_tbuf,pObj,target.id,pImp->GetAttackStamp());
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf.data(),_tbuf.size(),-1);

}

void 
gplayer_dispatcher::attack_once(unsigned char state)
{
	return;
	_tbuf.clear();
	using namespace S2C;
	gplayer * pObj = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_attack_once>::From(_tbuf,state);
	send_ls_msg(pObj,_tbuf);
}

void 
gplayer_dispatcher::be_damaged(const XID & id, int skill_id ,const attacker_info_t & info,int damage, int dt_damage, int dura_index,char at_state,char stamp,bool orange)
{
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(skill_id)
	{
		_tbuf.clear();
		CMD::Make<CMD::object_skill_attack_result>::From(_tbuf,id,pPlayer->ID,skill_id,damage,dt_damage,at_state,stamp);
		AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
	}
	else
	{
		_tbuf.clear();
		CMD::Make<CMD::object_attack_result>::From(_tbuf,id,pPlayer->ID,damage,at_state ,stamp);
		AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
	}
}

void 
gplayer_dispatcher::be_hurt(const XID & id, const attacker_info_t & info,int damage,bool invader)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
	{
		//如果对方是玩家，则发送特定数据给施法者
		CMD::Make<CMD::hurt_result>::From(_tbuf,pPlayer->ID,damage);
		send_ls_msg(info.cs_index,id.id,info.sid,_tbuf);
		_tbuf.clear();
	}

	CMD::Make<CMD::be_hurt>::From(_tbuf,id,damage,invader);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::dodge_attack(const XID &attacker, int skill_id, const attacker_info_t & info, char at_state,char stamp,bool orange)
{
	using namespace S2C;
	gplayer *pObj = (gplayer*)_imp->_parent;
	if(skill_id)
	{
		_tbuf.clear();
		CMD::Make<CMD::object_skill_attack_result>::From(_tbuf,attacker,pObj->ID,skill_id,0,0,at_state,stamp);
		AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);
	}
	else
	{
		_tbuf.clear();
		CMD::Make<CMD::object_attack_result>::From(_tbuf,attacker,pObj->ID,0,at_state,stamp);
		AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);
	}
}

void
gplayer_dispatcher::equipment_damaged(int index)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::equipment_damaged>::From(_tbuf,index);
	send_ls_msg((gplayer*)_imp->_parent,_tbuf);
}

void 
gplayer_dispatcher::on_death(const XID &killer,bool reserver, int time)
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	
	CMD::Make<CMD::be_killed>::From(_tbuf,pObj,killer, time);
	send_ls_msg((gplayer*)_imp->_parent,_tbuf);
	__PRINTF("send be killed message\n");
	
	_tbuf.clear();
	CMD::Make<CMD::player_dead>::From(_tbuf,killer,pObj->ID);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,pObj->ID.id);
}

void 
gplayer_dispatcher::stop_attack(int flag)
{
	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("player stop attack %ld.%06ld\n",tv.tv_sec,tv.tv_usec);

	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::self_stop_attack>::From(_tbuf,flag);

	gobject * pObj = _imp->_parent;
	send_ls_msg((gplayer*)pObj,_tbuf);
}

void 
gplayer_dispatcher::resurrect(int level)
{	
//	timeval tv;
//	gettimeofday(&tv,NULL);
//	__PRINTF("%d.%06d player resurrect\n",tv.tv_sec,tv.tv_usec);
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::player_revival>::From(_tbuf,pObj,level);
	//复活要让其他人知道
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);//自己也可以收到这个消息
//	gettimeofday(&tv,NULL);
//	__PRINTF("%d.%06d player resurrect\n",tv.tv_sec,tv.tv_usec);
}

void 
gplayer_dispatcher::notify_pos(const A3DVECTOR & pos)
{	
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::notify_pos>::From(_tbuf,pos,_imp->GetClientTag());
	send_ls_msg((gplayer*)(_imp->_parent),_tbuf);
}

void 
gplayer_dispatcher::get_base_info()
{
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::self_info_1>::From(_self,(gplayer*)pImp->_parent,pImp->_basic.exp);

}

void 
gplayer_dispatcher::query_info00(const XID & target, int cs_index,int sid)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	//_back_up_hp[1]
	CMD::Make<CMD::player_info_00>::From(_tbuf,pImp->_parent->ID,pImp->_basic.hp,pImp->_basic,pImp->_cur_prop,pImp->IsCombatState()?1:0);
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::query_info00()
{
/*
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_info_00>::From(_tbuf,pImp->_parent->ID,pImp->_basic,pImp->_cur_prop);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
	*/
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::self_info_00>::From(_tbuf,pImp->_basic,pImp->_cur_prop,pImp->IsCombatState()?1:0,(int)pImp->_talisman_info.stamina,pImp->_cheat_punish);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}


void 
gplayer_dispatcher::move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_move>::From(_tbuf,pPlayer,target,cost_time,speed,move_mode);

	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer,pPlayer->ID.id);//自己也可以收到这个消息
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer->ID.id);//自己也可以收到这个消息
}

void
gplayer_dispatcher::stop_move(const A3DVECTOR & target, unsigned short speed ,unsigned char dir,unsigned char move_mode)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_stop_move>::From(_tbuf,pPlayer,target,speed,dir,move_mode);

	pPlayer->dir = dir;
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer->ID.id);
}


void
gplayer_dispatcher::leave_world()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)(_imp->_parent);
	world * pPlane = _imp->_plane;
	CMD::Make<CMD::player_leave_world>::From(_tbuf,pPlayer);
	slice *pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::enter_region()
{
	world * pPlane = _imp->_plane;
	slice * pPiece = _imp->_parent->pPiece;
	gplayer * pPlayer = (gplayer*)(_imp->_parent);
	get_slice_info(pPiece,_nw,_mw,_pw,pPlayer->anti_invisible_rate,  pPlayer->team_id);

	int i;
	int total = pPlane->w_far_vision;
	int index = pPlane->GetGrid().GetSliceIndex(pPiece);
	int slice_x,slice_z;
	pPlane->GetGrid().Index2Pos(index,slice_x,slice_z);
	for(i = 0; i <total; i ++)
	{
		world::off_node_t &node = pPlane->w_off_list[i]; 
		int nx = slice_x + node.x_off; 
		int nz = slice_z + node.z_off; 
		if(nx < 0 || nz < 0 || nx >= pPlane->GetGrid().reg_column || nz >= pPlane->GetGrid().reg_row) continue;
		slice * pNewPiece = pPiece + node.idx_off;
		if(i <= pPlane->w_true_vision)
		{
			get_slice_info(pNewPiece,_nw,_mw,_pw,pPlayer->anti_invisible_rate,  pPlayer->team_id);
		}
		else
		{
			get_slice_info(pNewPiece,_nw,_mw,_pw,pPlayer->anti_invisible_rate,  pPlayer->team_id);
		}
		wrapper_test<MIN_SEND_COUNT>(_pw,_header,S2C::PLAYER_INFO_1_LIST);
		wrapper_test<MIN_SEND_COUNT>(_mw,_header,S2C::MATTER_INFO_LIST);
		wrapper_test<MIN_SEND_COUNT>(_nw,_header,S2C::NPC_INFO_LIST);
	}
	return ;
}

void 
gplayer_dispatcher::enter_world()
{
	world * pPlane = _imp->_plane;
	//首先发送进入世界的消息
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_world>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer->ID.id);

	//取得进入一个区域应当取得数据
	enter_region();
}

void 
gplayer_dispatcher::appear()
{
	world * pPlane = _imp->_plane;
	//首先发送进入世界的消息
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_world>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::error_message(int msg)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::error_msg>::From(_tbuf,msg);
	//发送离开消息
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void
gplayer_dispatcher::disappear(char at_once)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(_tbuf,pPlayer, at_once);
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer->ID.id);

}

void 
gplayer_dispatcher::receive_exp(int64_t exp)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::receive_exp>::From(_tbuf,exp);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::receive_bonus_exp(int64_t exp)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::receive_bonus_exp>::From(_tbuf,exp);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}


void
gplayer_dispatcher::embed_item(size_t chip_idx, size_t equip_idx)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::embed_item>::From(_tbuf,chip_idx,equip_idx);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::level_up()
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::level_up>::From(_tbuf,pObj);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf);
}

void 
gplayer_dispatcher::deity_levelup(char success)
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::deity_levelup>::From(_tbuf,pObj, pImp->_basic.dt_level,success);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf);
}

void 
gplayer_dispatcher::unselect()
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::unselect>::From(_tbuf);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::player_select_target(int id)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_select_target>::From(_tbuf,id);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_extprop_base()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_base>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
	
}

void 
gplayer_dispatcher::get_extprop_move()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_move>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_extprop_attack()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_attack>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_extprop_defense()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_defense>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::player_reject_invite(const XID & member)
{
	__PRINTF("team::player_reject_invite %d\n",_imp->_parent->ID.id);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_reject_invite>::From(_tbuf,member);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::leader_invite(const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid)
{
	__PRINTF("team::leader_invite %d  seq %d\n",_imp->_parent->ID.id,seq);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_leader_invite>::From(_tbuf,leader,seq,pickup_flag, familyid, mafiaid, level, sectid, referid);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::join_team(const XID & leader,int pickup_flag)
{
	__PRINTF("team::join_team %d\n",_imp->_parent->ID.id);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_join_team>::From(_tbuf,leader,pickup_flag);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::member_leave(const XID & leader, const XID& member,int type)
{
	__PRINTF("team::member_leave %d\n",_imp->_parent->ID.id);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_member_leave>::From(_tbuf,leader,member,type);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::leave_party(const XID & leader, int type)
{
	__PRINTF("team::leave_party %d\n",_imp->_parent->ID.id);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_leave_party>::From(_tbuf,leader,type);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::new_member(const XID & member)
{
	__PRINTF("team::new_member %d\n",_imp->_parent->ID.id);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_new_member>::From(_tbuf,member);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::leader_cancel_party(const XID & leader)
{
	//此命令已经取消
	ASSERT(false && "此命令已经取消");
	__PRINTF("team::leader_cancel_party %d\n",_imp->_parent->ID.id);
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_leader_cancel_party>::From(_tbuf,leader);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::npc_greeting(const XID & provider)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::npc_greeting>::From(_tbuf,provider);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::repair_all(size_t cost)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::repair_all>::From(_tbuf,cost);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::repair(int where,int index, size_t cost)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::repair>::From(_tbuf,where,index,cost);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::renew()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::renew>::From(_tbuf);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::spend_money(size_t cost)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::spend_money>::From(_tbuf,cost);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_player_money(size_t money,size_t capacity)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::get_own_money>::From(_tbuf,money,capacity);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::cast_skill(int target_cnt, const XID* targets, int skill,unsigned short time, unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::object_cast_skill>::From(_tbuf,pPlayer->ID,target_cnt,targets,skill,time,level,pImp->GetAttackStamp(), state, cast_speed_rate, pos, spirit_index);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::skill_interrupt(char reason, char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::skill_interrupted>::From(_tbuf,pPlayer->ID, spirit_index);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);

	_tbuf.clear();
	CMD::Make<CMD::self_skill_interrupted>::From(_tbuf,reason,spirit_index);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::skill_continue(int skill_id, char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::skill_continue>::From(_tbuf,pPlayer->ID.id, skill_id, spirit_index);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void
gplayer_dispatcher::stop_skill(char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_stop_skill>::From(_tbuf, spirit_index);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::takeoff()
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_takeoff>::From(_tbuf,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::landing()
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_landing>::From(_tbuf,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::flysword_time_capacity(unsigned char where, unsigned char index, int cur_time)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::flysword_time_capacity>::From(_tbuf,where,index,cur_time);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::skill_perform(char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::skill_perform>::From(_tbuf, spirit_index);
//	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 目前没有广播
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::teammate_get_pos(const XID & target,const A3DVECTOR & pos,int tag) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::teammate_pos>::From(_tbuf,target,pos,tag);
	send_ls_msg((gplayer*)_imp->_parent,_tbuf);
}

void 
gplayer_dispatcher::send_equipment_info(const XID & target, int cs_index, int sid)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::send_equipment_info>::From(_tbuf,pPlayer,pImp->_equip_info.mask,pImp->_equip_info.data);
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::send_clone_equipment_info(const XID & target, int clone_id, int cs_index, int sid)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::send_clone_equipment_info>::From(_tbuf,pPlayer, clone_id, pImp->_equip_info.mask,pImp->_equip_info.data);
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::equipment_info_changed(unsigned int madd,unsigned int mdel,const void * buf, size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::equipment_info_changed>::From(_tbuf,pPlayer,madd,mdel,buf,size);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::team_member_pickup(const XID & member, int type, int count)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::team_member_pickup>::From(_tbuf,member, type, count);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::send_team_data(const XID & leader,size_t team_count,size_t data_count, const player_team::member_entry ** list)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_member_data>::From(_tbuf,leader,team_count,data_count,list);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::send_team_data(const XID & leader,size_t team_count,const player_team::member_entry * list)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_member_data>::From(_tbuf,leader,team_count,list);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::item_to_money(size_t index, int type, size_t count,size_t price)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::item_to_money>::From(_tbuf,index,type,count,price);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::pickup_money(int money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pickup_money>::From(_tbuf,money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(abase::Rand(0,0) == 1)
	{
		std::pair<int/*user*/,int/*sid*/> pp(pPlayer->ID.id,pPlayer->cs_sid);
		GMSV::MultiSendClientData(pPlayer->cs_index,&pp,&pp,_tbuf.data(),_tbuf.size(),0);

	}
	else
	{
		send_ls_msg(pPlayer, _tbuf);
	}
}

void 
gplayer_dispatcher::pickup_item(int type, int expire_date, int amount, int slot_amount, int where,int index,int state)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pickup_item>::From(_tbuf,type,expire_date, amount & 0xFFFF,slot_amount & 0xFFFF,where & 0xFF,index & 0xFF,state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::obtain_item(int type, int expire_date, int amount, int slot_amount, int where,int index,int state)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_obtain_item>::From(_tbuf,type,expire_date,amount & 0xFFFF,slot_amount & 0xFFFF,where & 0xFF,index & 0xFF,state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::produce_once(int type, int amount, int slot_amount, int where,int index,int state)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::produce_once>::From(_tbuf,type,amount&0xFFFF,slot_amount & 0xFFFF,where & 0xFF,index & 0xFF,state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::produce_start(int type, int use_time ,int count)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::produce_start>::From(_tbuf,type,use_time,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::produce_end()
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::produce_end>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::decompose_start(int use_time,int type)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::decompose_start>::From(_tbuf,use_time,type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::decompose_end()
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::decompose_end>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::pickup_money_in_trade(size_t money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pickup_money_in_trade>::From(_tbuf,money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::pickup_item_in_trade(int type, int amount)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pickup_item_in_trade>::From(_tbuf,type,amount & 0xFFFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::receive_money_after_trade(size_t money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pickup_money_after_trade>::From(_tbuf,money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::receive_item_after_trade(int type,int expire_date, int amount, int slot_amount,int index,int state)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pickup_item_after_trade>::From(_tbuf,type,expire_date, amount & 0xFFFF, slot_amount & 0xFFFF,index & 0xFFFF,state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


void 
gplayer_dispatcher::purchase_item(int type, size_t money,int amount, int slot_amount, int where,int index)
{
	ASSERT(false);
/*
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::From(_tbuf,type,money,amount & 0xFFFF,slot_amount & 0xFFFF,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
	*/
}

void 
gplayer_dispatcher::self_item_empty_info(int where,int index) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::self_item_empty_info>::From(_tbuf,where, index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_item_info(int where,int index,item_data_client & data,unsigned short crc) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::self_item_info>::From(_tbuf,where, index,data,crc, item::Proctype2State(data.proc_type));
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_inventory_data(int where,unsigned char inv_size, const void * data, size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::self_inventory_data>::From(_tbuf,where,inv_size,data,len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_inventory_detail_data(int where,unsigned char inv_size,const void * data, size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::self_inventory_detail_data>::From(_tbuf,where,inv_size,data,len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::exchange_item_list_info(unsigned char where,const void *data,size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::exchange_item_list_info>::From(_tbuf,where,data,len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::repurchase_inventory_data(unsigned char inv_size,const void * data, size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::repurchase_inventory_data>::From(_tbuf,inv_size,data,len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_inventory_item(size_t idx1,size_t idx2)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::exchange_inventory_item>::From(_tbuf,idx1,idx2);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::move_inventory_item(size_t src,size_t dest,size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::move_inventory_item>::From(_tbuf,src,dest,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::player_drop_item(size_t where, size_t index,int type,size_t count, unsigned char drop_type)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_drop_item>::From(_tbuf,where,index,type,count,drop_type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

// Youshuang add
void gplayer_dispatcher::send_talisman_enchant_addon( int id, int index )
{
	_tbuf.clear();
	using namespace S2C;

	_tbuf << (unsigned short)S2C::SEND_TALISMAN_ENCHANT_ADDON;
	_tbuf << id;
	_tbuf << index;
	
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::produce_colorant_item( int index, int color, int quality )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::fashion_colorant_produce>::From(_tbuf,index,color,quality);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::use_colorant_item( int where, int index, int color, int quality )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::fashion_colorant_use>::From(_tbuf,where,index,color,quality);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_achievement_award( unsigned short achiev_id )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::get_achieve_award>::From(_tbuf,achiev_id);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_raid_boss_info( const std::vector<raid_boss_info>& monsters_been_killed )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::raid_boss_been_killed>::From(_tbuf, monsters_been_killed);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_facbase_mall_info( facbase_mall* mall_info )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::facbase_mall_item_info>::From( _tbuf, mall_info->GetFacbaseMallItemInfo() );
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_faction_coupon( int faction_coupon )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::get_faction_coupon>::From(_tbuf, faction_coupon);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_facbase_cash_items_info( const std::map<int, int>& item_cooldown )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::get_facbase_cash_items_info>::From(_tbuf, item_cooldown);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_facbase_cash_item_success( int item_index )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::get_facbase_cash_item_success>::From(_tbuf, item_index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_newyear_award_info( int score, const std::vector<char>& status )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::get_newyear_award_info>::From(_tbuf, score, status);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}
// end

void 
gplayer_dispatcher::exchange_equipment_item(size_t index1,size_t index2)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::exchange_equipment_item>::From(_tbuf,index1,index2);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::equip_item(size_t index_inv,size_t index_equip,int count_inv,int count_eq)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::equip_item>::From(_tbuf,index_inv,index_equip,count_inv,count_eq);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::move_equipment_item(size_t index_inv,size_t index_equip, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::move_equipment_item>::From(_tbuf,index_inv,index_equip,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_get_property(size_t status_point, const q_extend_prop & prop )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::self_get_property>::From(_tbuf,status_point,prop);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::get_skill_data()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	raw_wrapper wrapper(256);
	pImp->_skill.StorePartial(wrapper);
	CMD::Make<CMD::skill_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::get_combine_skill_data()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	raw_wrapper wrapper(64);
	pImp->_skill.SaveSkillElemsClient(wrapper);
	CMD::Make<CMD::combine_skill_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);

}


void 
gplayer_dispatcher::get_task_data()
{
	//这里不用tbuf是因为太大了
	packet_wrapper  h1(8192);
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	PlayerTaskInterface  task_if(pImp);
	CMD::Make<CMD::task_data>::From(h1,task_if.GetActiveTaskList(),task_if.GetActLstDataSize(),task_if.GetFinishedTaskList(),task_if.GetFnshLstDataSize(), task_if.GetFinishedTimeList(), task_if.GetFnshTimeLstDataSize());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, h1);
}

void 
gplayer_dispatcher::send_task_var_data(const void * buf, size_t size)
{
	//这里不用tbuf是因为太大了
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	PlayerTaskInterface  task_if(pImp);
	CMD::Make<CMD::task_var_data>::From(_tbuf,buf,size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::clear_embedded_chip(unsigned short equip_idx,size_t use_money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::clear_embedded_chip>::From(_tbuf,equip_idx, use_money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::learn_skill(int skill ,int level)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::learn_skill>::From(_tbuf,skill,level);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::use_item(char where, unsigned char index , int item_type , unsigned short count)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_use_item>::From(_tbuf,where, index, item_type, count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::use_item(char where, unsigned char index , int item_type , unsigned short count,const char * arg, size_t arg_size)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_use_item_with_arg>::From(_tbuf,where, index, item_type, count,arg,arg_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::use_item(int item_type,const char * arg, size_t arg_size)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_use_item_with_arg>::From(_tbuf,pPlayer,item_type,arg,arg_size);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::start_use_item(int item_type, int use_time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_start_use>::From(_tbuf,pPlayer,item_type,use_time);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::start_use_item_with_target(int item_type, int use_time,const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_start_use_with_target>::From(_tbuf,pPlayer,item_type,use_time,target);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::cancel_use_item()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_cancel_use>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::use_item(int item_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_use_item>::From(_tbuf,pPlayer,item_type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::sit_down()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_sit_down>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::stand_up()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_stand_up>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::do_emote(unsigned short emotion)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_do_emote>::From(_tbuf,pPlayer,emotion);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::do_emote_restore(unsigned short emotion)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_do_emote_restore>::From(_tbuf,pPlayer,emotion);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::do_action(unsigned char action)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_do_action>::From(_tbuf,pPlayer,action);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::send_timestamp()
{
	_tbuf.clear();
	using namespace S2C;
	__PRINTF("%d %d\n",(int)g_timer.get_systime(),(int)time(NULL));
	time_t t = time(NULL);
	struct tm  tm1;
	localtime_r(&t, &tm1);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::server_timestamp>::From(_tbuf,t, -(tm1.tm_gmtoff / 60) , gmatrix::GetLuaVersion());
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::notify_root(unsigned char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pObject =(gplayer*) _imp->_parent;
	CMD::Make<CMD::notify_root>::From(_tbuf,pObject);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,_tbuf,pObject->ID.id);

	_tbuf.clear();
	CMD::Make<CMD::self_notify_root>::From(_tbuf,pObject,type&0x7F);
	send_ls_msg(pObject, _tbuf);

}

void gplayer_dispatcher::self_notify_root(unsigned char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pObject =(gplayer*) _imp->_parent;
	CMD::Make<CMD::self_notify_root>::From(_tbuf,pObject,type&0x7F);
	send_ls_msg(pObject, _tbuf);

}

void
gplayer_dispatcher::dispel_root(unsigned char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::dispel_root>::From(_tbuf,type);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::gather_start(int mine, unsigned char t)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_gather_start>::From(_tbuf,pPlayer->ID,mine,t);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::gather_stop()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_gather_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::trashbox_passwd_changed(bool has_passwd)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_passwd_changed>::From(_tbuf,has_passwd);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_open(unsigned char where)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	CMD::Make<CMD::trashbox_open>::From(_tbuf,where,pImp->_trashbox.GetCurTrashBoxSize());
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_close()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_close>::From(_tbuf);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_wealth(size_t money)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_wealth>::From(_tbuf,money);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_passwd_state(bool has_passwd)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_passwd_state>::From(_tbuf,has_passwd);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_trashbox_item(unsigned char where, size_t idx1, size_t idx2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_trashbox_item>::From(_tbuf,where,idx1,idx2);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::move_trashbox_item(unsigned char where, size_t src, size_t dest, size_t delta)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_trashbox_item>::From(_tbuf,where, src,dest,delta);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_trashbox_inventory(unsigned char where, size_t idx_tra,size_t idx_inv)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_trashbox_inventory>::From(_tbuf,where, idx_tra,idx_inv);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::trash_item_to_inventory(unsigned char where, size_t idx_tra, size_t idx_inv, size_t delta)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trash_item_to_inventory>::From(_tbuf,where, idx_tra,idx_inv,delta);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::inventory_item_to_trash(unsigned char where, size_t idx_inv, size_t idx_tra, size_t delta)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::inventory_item_to_trash>::From(_tbuf,where, idx_inv,idx_tra,delta);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_trash_money(int inv_money, int tra_money)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_trash_money>::From(_tbuf,inv_money,tra_money);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::set_adv_data(int data1,int data2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_set_adv_data>::From(_tbuf,pPlayer,data1,data2);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::clear_adv_data()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_clr_adv_data>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_in_team(unsigned char state)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_in_team>::From(_tbuf,pPlayer,state);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::send_party_apply(int id, int familyid, int mafiaid, int level, int sectid, int referid)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::team_apply_request>::From(_tbuf,id, familyid, mafiaid, level, sectid, referid);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::query_info_1(int uid,int cs_index, int cs_sid)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<single_data_header>::From(_tbuf,PLAYER_INFO_1);
	if(CMD::Make<INFO::player_info_1>::From(_tbuf,(gplayer*)_imp->_parent))
	{
		send_ls_msg(cs_index,uid,cs_sid,_tbuf);
	}
}

void 
gplayer_dispatcher::mafia_info_notify()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_info_notify>::From(_tbuf,pPlayer); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::mafia_trade_start()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_trade_start>::From(_tbuf); 
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::mafia_trade_end()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_trade_end>::From(_tbuf); 
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_item(int type,int expire_date, int state, int amount, int slot_amount, int where,int index)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::task_deliver_item>::From(_tbuf,type,expire_date, state, amount & 0xFFFF,slot_amount & 0xFFFF,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_reputaion(int offset,int cur_reputaion)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::task_deliver_reputaion>::From(_tbuf,offset,cur_reputaion);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_exp(int64_t exp, int sp, int dt_exp)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::task_deliver_exp>::From(_tbuf,exp,sp,dt_exp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_money(size_t amount, size_t money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::task_deliver_money>::From(_tbuf,amount,money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_level2(int level2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::task_deliver_level2>::From(_tbuf,pPlayer,level2);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_reputation(int reputation)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_reputation>::From(_tbuf,pPlayer, reputation);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::identify_result(char index, char result)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::identify_result>::From(_tbuf,index,result);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::change_shape(char shape)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_shape>::From(_tbuf,pPlayer, shape);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::enter_sanctuary()
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_enter_sanctuary>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::leave_sanctuary()
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_leave_sanctuary>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::begin_personal_market(int item_id, int market_id, const char * name, size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_open_market>::From(_tbuf, item_id, pPlayer, market_id & 0xFF, name, len);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::cancel_personal_market()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cancel_market>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::market_trade_success(int trader)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_market_trade_success>::From(_tbuf,trader);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::send_market_name(const XID & target, int cs_index, int sid,const char * name ,size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_market_name>::From(_tbuf,pPlayer,name,len);
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::player_start_travel(int  line_no,const A3DVECTOR & dest_pos,float speed,int vehicle)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_travel>::From(_tbuf,pPlayer,vehicle);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);

	_tbuf.clear();
	CMD::Make<CMD::self_start_travel>::From(_tbuf,speed,dest_pos,line_no,vehicle);
	send_ls_msg(pPlayer,_tbuf);

}

void 
gplayer_dispatcher::player_complete_travel(int vehicle)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_complete_travel>::From(_tbuf,pPlayer,vehicle);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::toggle_invisible(char tmp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::gm_toggle_invisible>::From(_tbuf,tmp);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::toggle_invincible(char tmp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::gm_toggle_invincible>::From(_tbuf,tmp);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::trace_cur_pos(unsigned short seq)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_trace_cur_pos>::From(_tbuf,pPlayer->pos,seq);
	send_ls_msg(pPlayer,_tbuf);

	stop_move(pPlayer->pos,0x0010,pPlayer->dir,0x01);
}

void 
gplayer_dispatcher::cast_instant_skill(int target_cnt, const XID* targets, int skill,unsigned char level, const A3DVECTOR& pos, char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_cast_instant_skill>::From(_tbuf,pPlayer,target_cnt,targets,skill,level,pos,spirit_index);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::activate_waypoint(unsigned short waypoint)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::activate_waypoint>::From(_tbuf,waypoint);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_waypoint_list(const unsigned short * list, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_waypoint_list>::From(_tbuf,list,count);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::unlock_inventory_slot(unsigned char where, unsigned short index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::unlock_inventory_slot>::From(_tbuf,where, index);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::team_invite_timeout(int who)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::team_invite_timeout>::From(_tbuf,who);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pvp_no_protect()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pvp_no_protect>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::send_cooldown_data()
{
	raw_wrapper rw;
	((gplayer_imp*)_imp)->GetCoolDownDataForClient(rw);
	
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cooldown_data>::From(_tbuf,rw.data(),rw.size());
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::update_cooldown_data(archive & ar)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cooldown_data>::From(_tbuf,ar.data(),ar.size());
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::skill_ability_notify(int id, int ability)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::skill_ability_notify>::From(_tbuf,id, ability);
	send_ls_msg(pPlayer,_tbuf);
}


void
gplayer_dispatcher::personal_market_available(int index, int item_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::personal_market_available>::From(_tbuf, index, item_id);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::breath_data(int breath, int breath_capacity)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::breath_data>::From(_tbuf,breath, breath_capacity);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::stop_dive()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_dive>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::trade_away_item(int buyer,short inv_idx,int type, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trade_away_item>::From(_tbuf,buyer, inv_idx,type, count);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_enable_fashion_mode(char is_enable)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enable_fashion_mode>::From(_tbuf,pPlayer, is_enable);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_enable_free_pvp(char enable_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enable_free_pvp_mode>::From(_tbuf,enable_type);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_enable_effect(short effect)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enable_effect>::From(_tbuf,pPlayer->ID.id, effect);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_disable_effect(short effect)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_disable_effect>::From(_tbuf,pPlayer->ID.id, effect);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::enable_resurrect_state(float exp_reduce)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enable_resurrect_state>::From(_tbuf,exp_reduce);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::object_is_invalid(int id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_is_invalid>::From(_tbuf,id);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::set_cooldown(int idx, int cooldown)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::set_cooldown>::From(_tbuf,idx, cooldown);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::change_team_leader(const XID & old_leader, const XID & new_leader)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::change_team_leader>::From(_tbuf,old_leader,new_leader);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::kickout_instance(int timeout)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::kickout_instance>::From(_tbuf,_imp->GetWorldTag(), timeout);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::kickout_raid(int raid_id, int timeout)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::kickout_instance>::From(_tbuf,raid_id, timeout);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::begin_cosmetic(unsigned short inv_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cosmetic_begin>::From(_tbuf,inv_index);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::end_cosmetic(unsigned short inv_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cosmetic_end>::From(_tbuf,inv_index);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::cosmetic_success(unsigned short crc)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cosmetic_success>::From(_tbuf,pPlayer->ID,crc);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::cast_pos_skill(const A3DVECTOR & pos, int skill,unsigned short time,unsigned char level)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_cast_pos_skill>::From(_tbuf,pPlayer->ID,pos,skill,time,level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::change_move_seq(unsigned short seq)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::change_move_seq>::From(_tbuf,seq);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::server_config_data()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::server_config_data>::From(_tbuf, _imp->GetWorldManager(),item_manager::GetMallTimestamp());
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::active_rush_mode(char is_active)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_rush_mode>::From(_tbuf,is_active);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::produce_null(int recipe_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::produce_null>::From(_tbuf,recipe_id);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::trashbox_capacity_notify(int cap)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_capacity_notify>::From(_tbuf,cap);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::mafia_trashbox_capacity_notify(int cap)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_trashbox_capacity_notify>::From(_tbuf,cap);
	send_ls_msg(pPlayer,_tbuf);
}


void
gplayer_dispatcher::active_pvp_combat_state(bool is_active)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::active_pvp_combat_state>::From(_tbuf,pPlayer,is_active);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::duel_recv_request(const XID &target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_recv_request>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_reject_request(const XID &target,int reason) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_reject_request>::From(_tbuf,target,reason);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_prepare(const XID & target, int delay)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_prepare>::From(_tbuf,target,delay);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_cancel(const XID & target) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_cancel>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_start(const XID & who) 
{
	_tbuf.clear();
	//通知自己决斗开始
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_start>::From(_tbuf,who);
	send_ls_msg(pPlayer,_tbuf);

	//通知周围决斗开始
	_tbuf.clear();
	CMD::Make<CMD::else_duel_start>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::duel_stop()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::duel_result(const XID & target, bool is_failed) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_result>::From(_tbuf,target, pPlayer->ID, is_failed);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_bind_request(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_request>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_invite(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_invite>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_request_reply(const XID & target, int param)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_request_reply>::From(_tbuf,target,param);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_invite_reply(const XID & target, int param)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_invite_reply>::From(_tbuf,target,param);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_start(const XID & target)
{
	//这个start始终是上面那个法出的
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_start>::From(_tbuf,target, pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_bind_stop()
{
	//这个start始终是骑在上面那个法出的
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_mounting(int mount_id, char mount_level, char mount_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_mounting>::From(_tbuf,pPlayer->ID, mount_id, mount_level, mount_type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::send_equip_detail(int cs_index, int cs_sid,int target, const void * data, size_t size,const void * data_skill, size_t size_skill,
	const void* data_pet,size_t size_pet,const void* data_pet_equip,size_t size_pet_equip)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_equip_detail>::From(_tbuf,pPlayer->ID, data,size,data_skill,size_skill,data_pet,size_pet,data_pet_equip,size_pet_equip);
	send_ls_msg(cs_index,target,cs_sid,_tbuf);
}

void 
gplayer_dispatcher::pk_level_notify(char level)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pk_level_notify>::From(_tbuf,pPlayer, level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::pk_value_notify(int value)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pk_value_notify>::From(_tbuf,pPlayer, value);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_change_class()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_class>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::enter_battleground( unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_battleground>::From( _tbuf, battle_faction, battle_type, battle_id, end_timestamp );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::leave_battleground()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_battleground>::From( _tbuf );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::enter_battlefield(int faction_id_attacker,int faction_id_defender,int faction_id_attacker_assistant,int battle_id,int end_timestamp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_battlefield>::From(_tbuf,faction_id_attacker,faction_id_defender,faction_id_attacker_assistant,battle_id,end_timestamp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::leave_battlefield()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_battlefield>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battlefield_info(unsigned char count,const S2C::CMD::battlefield_info::structure_info_t* pInfo)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::battlefield_info>::From(_tbuf,count,pInfo);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battle_result(int result, int attacker_score, int defender_score)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::battle_result>::From(_tbuf,result, attacker_score, defender_score);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::get_recipe_data(const unsigned short * buf, size_t count)
{
	packet_wrapper  h1(count * sizeof(unsigned short) + 64);
	using namespace S2C;
	CMD::Make<CMD::player_recipe_data>::From(h1,buf, count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,h1);
}

void gplayer_dispatcher::player_learn_recipe(unsigned short recipe_id)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_learn_recipe>::From(_tbuf,recipe_id);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_produce_skill_info(int level ,int exp)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_produce_skill_info>::From(_tbuf,level,exp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::start_general_operation(int op_id,int duration, bool broadcast)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_general_operation>::From(_tbuf,pPlayer,op_id, duration);
	if(broadcast)
	{
		AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
	}
	else
	{
		send_ls_msg(pPlayer,_tbuf);
	}

}

void 
gplayer_dispatcher::stop_general_operation(int op_id, bool broadcast)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_general_operation>::From(_tbuf,pPlayer,op_id);
	if(broadcast)
	{
		AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
	}
	else
	{
		send_ls_msg(pPlayer,_tbuf);
	}
}

void 
gplayer_dispatcher::refine_result(int item_index, int item_id, int result)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_refine_result>::From(_tbuf,item_index, item_id, result);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_cash(int cash, int used, int cash_add2)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_cash>::From(_tbuf,cash,used, cash_add2);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bonus(int bonus, int used)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_bonus>::From(_tbuf,bonus,used);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_skill_addon(int skill, int level)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_skill_addon>::From(_tbuf,skill,level);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_skill_common_addon(int level)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_skill_common_addon>::From(_tbuf,level);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_extra_skill(int skill, int level)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_extra_skill>::From(_tbuf,skill,level);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_title_list(const short *  title, int count ,const short * extra_title, size_t extra_count)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_title_list>::From(_tbuf,title,count, extra_title, extra_count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_add_title(short title)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_add_title>::From(_tbuf,title);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_del_title(short title)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_del_title>::From(_tbuf,title);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_change_title(short title)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_title>::From(_tbuf,pPlayer, title);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_use_title( short title )
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_use_title>::From(_tbuf,title);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_region_reputation(const int * rep, int count) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_region_reputation>::From(_tbuf,rep,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_change_region_reputation(int idx, int rep) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_change_region_reputation>::From(_tbuf,idx,rep);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_change_inventory_size(int new_size)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_change_inventory_size>::From(_tbuf,new_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_change_mountwing_inv_size(int new_size)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_change_mountwing_inv_size>::From(_tbuf,new_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_bind_success(size_t index, int id,int state)
{       
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_bind_success>::From(_tbuf,index, id,state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_change_spouse(int id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_spouse>::From(_tbuf,pPlayer, id);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_invader_state(bool inv)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_invader_state>::From(_tbuf,pPlayer, inv);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
	__PRINTF("玩家 %d %s\n", pPlayer->ID.id, inv?"变成粉名啦":"从粉名变回来啦");
}

void gplayer_dispatcher::player_mafia_contribution(int con, int fcon)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_mafia_contribution>::From(_tbuf,con, fcon);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::lottery_bonus(int lottery_id ,int bonus_level, int bonus_item, int bonus_count, int bonus_money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::lottery_bonus>::From(_tbuf,lottery_id, bonus_level,bonus_item,bonus_count,bonus_money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_recorder_start(int stamp)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::recorder_start>::From(_tbuf,stamp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_recorder_check(const char * buf , size_t len)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::recorder_check_result>::From(_tbuf,buf, len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::npc_service_result(int type)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::npc_service_result>::From(_tbuf,type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battle_player_info( int id, unsigned char battle_faction,
			unsigned int battle_score, unsigned short kill, unsigned short death )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::battle_player_info>::From(_tbuf, id, battle_faction, battle_score, kill, death );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battle_info( unsigned char attacker_building_left, unsigned char defender_building_left,
			const char* buf, size_t size )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	ASSERT( size % sizeof(CMD::battle_info::player_info_in_battle) == 0 );
	unsigned char player_count = size / sizeof(CMD::battle_info::player_info_in_battle);
	CMD::Make<CMD::battle_info>::From(_tbuf, attacker_building_left, defender_building_left, 
		player_count, (CMD::battle_info::player_info_in_battle*)buf );
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_wallow_info(int level, int msg, int ptime, int light_t, int heavy_t)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_wallow_info>::From(_tbuf, gmatrix::AntiWallow()?1:0,level, msg, ptime, light_t , heavy_t
			);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::talisman_exp_notify(int where, int index, int exp)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::talisman_exp_notify>::From(_tbuf, where, index, exp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::talisman_combine(int type1, int type2, int type3)
{
	_tbuf.clear();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	using namespace S2C;
	CMD::Make<CMD::talisman_combine>::From(_tbuf, pPlayer->ID.id,  type1,type2, type3);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::talisman_bot_begin( unsigned char active )
{
	_tbuf.clear();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	using namespace S2C;
	CMD::Make<CMD::auto_bot_begin>::From(_tbuf, active );
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::talisman_combine_value(int value)
{
	_tbuf.clear();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	using namespace S2C;
	CMD::Make<CMD::talisman_value>::From(_tbuf, value);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::talisman_refineskill(int index, int id)
{
	_tbuf.clear();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	using namespace S2C;
	CMD::Make<CMD::talisman_refineskill>::From(_tbuf, id, index);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::anti_cheat_bonus(int type)
{
	_tbuf.clear();
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	using namespace S2C;
	CMD::Make<CMD::anti_cheat_bonus>::From(_tbuf, type);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battle_score( size_t score )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::battle_score>::From( _tbuf, score );
	send_ls_msg( pPlayer, _tbuf );
}

void gplayer_dispatcher::gain_battle_score( size_t inc )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::gain_battle_score>::From( _tbuf, inc );
	send_ls_msg( pPlayer, _tbuf );
}

void gplayer_dispatcher::spend_battle_score( size_t dec )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::spend_battle_score>::From( _tbuf, dec );
	send_ls_msg( pPlayer, _tbuf );
}

void gplayer_dispatcher::player_talent_notify(int value) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::talent_notify>::From( _tbuf, value );
	send_ls_msg( pPlayer, _tbuf );
}

void gplayer_dispatcher::notify_safe_lock(char active, int time, int max_time)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::notify_safe_lock>::From( _tbuf, active, time + g_timer.get_systime(), max_time);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::battlefield_construction_info(int res_a,int res_b,const char* buf,size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	ASSERT(size % sizeof(CMD::battlefield_construction_info::node_t) == 0);
	unsigned char count = size / sizeof(CMD::battlefield_construction_info::node_t);
	CMD::Make<CMD::battlefield_construction_info>::From(_tbuf,res_a,res_b,count,(CMD::battlefield_construction_info::node_t*)buf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::battlefield_contribution_info(unsigned char page,unsigned char max_page,const char* buf,size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	ASSERT(size % sizeof(CMD::battlefield_contribution_info::node_t) == 0);
	unsigned char count = size / sizeof(CMD::battlefield_contribution_info::node_t);
	CMD::Make<CMD::battlefield_contribution_info>::From(_tbuf,page,max_page,count,(CMD::battlefield_contribution_info::node_t*)buf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_equip_pet_bedge(unsigned char inv_index,unsigned char pet_index)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_equip_pet_bedge>::From(_tbuf,inv_index,pet_index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_equip_pet_equip(unsigned char inv_index,unsigned char pet_index)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_equip_pet_equip>::From(_tbuf,inv_index,pet_index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_set_pet_status(unsigned char pet_index,int pet_tid,unsigned char main_status,unsigned char sub_status)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_set_pet_status>::From(_tbuf,pet_index,pet_tid,main_status,sub_status);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_set_pet_rank(unsigned char pet_index,int pet_tid,unsigned char rank)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_set_pet_rank>::From(_tbuf,pet_index,pet_tid,rank);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_summon_pet(unsigned char pet_index,int pet_tid,int pet_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_summon_pet>::From(_tbuf,pet_index,pet_tid,pet_id);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_recall_pet(unsigned char pet_index,int pet_tid,int pet_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_recall_pet>::From(_tbuf,pet_index,pet_tid,pet_id);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_combine_pet(unsigned char pet_index,int pet_tid,unsigned char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_combine_pet>::From(_tbuf,pet_index,pet_tid,type);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_uncombine_pet(unsigned char pet_index,int pet_tid)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_uncombine_pet>::From(_tbuf,pet_index,pet_tid);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_start_pet_operation(unsigned char pet_index,int pet_id,int delay,unsigned char operation)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_pet_op>::From(_tbuf,pet_index,pet_id,delay,operation);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_end_pet_operation(unsigned char opeartion)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_pet_op>::From(_tbuf,opeartion);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_recv_exp(unsigned char pet_index,int pet_id, int cur_exp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_recv_exp>::From(_tbuf,pet_index,pet_id,cur_exp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_level_up(unsigned char pet_index,int pet_id,int cur_level,int cur_exp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_levelup>::From(_tbuf,pet_index,pet_id,cur_level,cur_exp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_honor_point(unsigned char pet_index,int cur_honor_point,int max_honor_point)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_honor_point>::From(_tbuf,pet_index,cur_honor_point,max_honor_point);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_hunger_point(unsigned char pet_index,int cur_hunger_point,int max_hunger_point)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_hunger_point>::From(_tbuf,pet_index,cur_hunger_point,max_hunger_point);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_age_life(unsigned char pet_index,int age,int life)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_age_life>::From(_tbuf,pet_index,age,life);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_dead(unsigned char pet_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_dead>::From(_tbuf,pet_index);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_hp_notify(unsigned char pet_index,int cur_hp,int max_hp,int cur_vp,int max_vp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_hp_notify>::From(_tbuf,pet_index,cur_hp,max_hp,cur_vp,max_vp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_ai_state(unsigned char pet_index,unsigned char aggro_state,unsigned char stay_state)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_ai_state>::From(_tbuf,pet_index,aggro_state,stay_state);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_room_capacity(unsigned char capacity)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_room_capacity>::From(_tbuf,capacity);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_set_auto_skill(unsigned char pet_index,int pet_id,int skill_id,unsigned char set_flag)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_set_auto_skill>::From(_tbuf,pet_index,pet_id,skill_id,set_flag);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_set_skill_cooldown(unsigned char pet_index,int pet_id,int cooldown_index,int cooldown_time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_set_skill_cooldown>::From(_tbuf,pet_index,pet_id,cooldown_index,cooldown_time);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_reborn_info(const int * info, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_reborn_info>::From(_tbuf,info, count);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::script_message(int player_id,int pet_id,unsigned short channel_id,unsigned short msg_id,bool bBroadcast)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::script_message>::From(_tbuf,player_id,pet_id,channel_id,msg_id);
	if(bBroadcast)
	{
		AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
	}
	else
	{
		send_ls_msg(pPlayer,_tbuf);
	}
}

void gplayer_dispatcher::player_pet_civilization(int civilization)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_civilization>::From(_tbuf,civilization);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_pet_construction(int construction)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_construction>::From(_tbuf,construction);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_move_pet_bedge(unsigned char src_index,unsigned char dst_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_move_pet_bedge>::From(_tbuf,src_index,dst_index);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_start_fly(char active, char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_fly>::From(_tbuf,pPlayer, active, type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_pet_prop_added(unsigned char pet_index,unsigned short maxhp,unsigned short maxmp,unsigned short defence,
	unsigned short attack,unsigned short* resistance,unsigned short hit,unsigned short jouk,float crit_rate,float crit_damage,int equip_mask)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_prop_added>::From(_tbuf,pet_index,maxhp,maxmp,defence,attack,resistance,hit,jouk,crit_rate,crit_damage,equip_mask);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::sect_become_disciple(int master_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::sect_become_disciple>::From(_tbuf,pPlayer->ID.id, master_id);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}


void gplayer_dispatcher::cultivation_notify(char cult)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cultivation_notify>::From(_tbuf,pPlayer->ID.id, cult);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}


void gplayer_dispatcher::offline_agent_bonus(int time, int left_time, int64_t bonus_exp) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::offline_agent_bonus>::From(_tbuf,time, left_time, bonus_exp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::offline_agent_time(int time) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::offline_agent_time>::From(_tbuf,time);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::equip_spirit_decrease(short index, short amount ,int result)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::spirit_power_decrease>::From(_tbuf,index, amount, result);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_bind_ride_invite(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_ride_invite>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_bind_ride_invite_reply(const XID & target, int param)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_ride_invite_reply>::From(_tbuf, target, param);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_bind_ride_join(const XID & leader, const XID & member, char bind_seq)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_ride_join>::From(_tbuf, leader, member, bind_seq);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_bind_ride_leave(const XID & leader, const XID & member, char bind_seq)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_ride_leave>::From(_tbuf, leader, member, bind_seq);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_bind_ride_kick(const XID & leader, char bind_seq)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_ride_kick>::From(_tbuf, leader, bind_seq);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::exchange_pocket_item(size_t idx1, size_t idx2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_pocket_item>::From(_tbuf, idx1, idx2);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::move_pocket_item(size_t src, size_t dest, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_pocket_item>::From(_tbuf, src, dest, count);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_inventory_pocket_item(size_t idx_inv, size_t idx_poc)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_inventory_pocket_item>::From(_tbuf, idx_inv, idx_poc);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::move_pocket_item_to_inventory(size_t idx_poc, size_t idx_inv, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_pocket_item_to_inventory>::From(_tbuf, idx_poc, idx_inv, count);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::move_inventory_item_to_pocket(size_t idx_inv, size_t idx_poc, size_t count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_inventory_item_to_pocket>::From(_tbuf, idx_inv, idx_poc, count);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_change_pocket_size(int new_size)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_pocket_size>::From(_tbuf, new_size);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::move_all_pocket_item_to_inventory()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_all_pocket_item_to_inventory>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}


void gplayer_dispatcher::move_max_pocket_item_to_inventory()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_max_pocket_item_to_inventory>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::notify_fashion_hotkey(int count,const S2C::CMD::notify_fashion_hotkey::key_combine* key)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_fashion_hotkey>::From(_tbuf,count,key);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_fashion_item(size_t idx1, size_t idx2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_fashion_item>::From(_tbuf, idx1, idx2);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_mount_wing_item(size_t idx1, size_t idx2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_mount_wing_item>::From(_tbuf, idx1, idx2);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_inventory_fashion_item(size_t idx_inv, size_t idx_fas)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_inventory_fashion_item>::From(_tbuf, idx_inv, idx_fas);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_inventory_mountwing_item(size_t idx_inv, size_t idx_mw)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_inventory_mountwing_item>::From(_tbuf, idx_inv, idx_mw);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_equipment_fashion_item(size_t idx_equ, size_t idx_fas)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_equipment_fashion_item>::From(_tbuf, idx_equ, idx_fas);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_equipment_mountwing_item(size_t idx_equ, size_t idx_mw)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_equipment_mountwing_item>::From(_tbuf, idx_equ, idx_mw);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::exchange_hotkey_equipment_fashion(size_t idx_key, size_t idx_head, size_t idx_cloth, size_t idx_shoe)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_hotkey_equipment_fashion>::From(_tbuf, idx_key, idx_head, idx_cloth, idx_shoe);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::peep_info(int id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::peep_info>::From(_tbuf, id);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::self_killingfield_info(int score, int rank, int con, int pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_killingfield_info>::From(_tbuf, score, rank, con, pos);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::killingfield_info(int attacker_score, int defender_score, int attacker_count, int defender_count, int count, const char * info)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::killingfield_info>::From(_tbuf, attacker_score, defender_score, attacker_count, defender_count, count, info); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::arena_info(int kill_count, int score, int apply_count, int remain_count, int count, const char * info)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::arena_info>::From(_tbuf, kill_count, score, apply_count, remain_count, count, info); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::rank_change(char new_rank)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::rank_change>::From(_tbuf,pPlayer->ID.id, new_rank);  
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::change_killingfield_score(int oldValue, int newValue)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::change_killingfield_score>::From(_tbuf,oldValue, newValue);  
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_change_style(unsigned char faceid, unsigned char hairid, unsigned char earid,unsigned char tailid, unsigned char fashionid)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_style>::From(_tbuf,pPlayer->ID.id, faceid, hairid, earid, tailid, fashionid);  
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_change_vipstate(unsigned char new_state)
{
	_tbuf.clear(); 
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_vipstate>::From(_tbuf,pPlayer->ID.id, new_state);  
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::crossserver_battle_info(const S2C::CMD::crossserver_battle_info & info)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::crossserver_battle_info>::From(_tbuf,info); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::crossserver_off_line_score_info(const std::vector<S2C::CMD::cs_battle_off_line_info::player_off_line_in_cs_battle>& vec_info)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cs_battle_off_line_info>::From(_tbuf,&vec_info[0],vec_info.size());
	send_ls_msg(pPlayer,_tbuf);

}
void gplayer_dispatcher::crossserver_team_battle_info(S2C::CMD::crossserver_team_battle_info & info, const char *pbuf, size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	ASSERT(size % sizeof(CMD::crossserver_team_battle_info::history_score) == 0);
	info.history_round_count = size / sizeof(CMD::crossserver_team_battle_info::history_score);
	CMD::Make<CMD::crossserver_team_battle_info>::From(_tbuf, info, (CMD::crossserver_team_battle_info::history_score *)pbuf);

	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::notify_circleofdoom_start(int sponsor_id, char faction, int faction_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_circleofdoom_start>::From(_tbuf, sponsor_id, faction);

	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCODStartMsg(_imp->_plane,pPiece,_tbuf, faction, faction_id);
}

//这个协议由阵法发起者向外广播，不用判断自己是否隐身，因为隐身不能发起阵法。
void gplayer_dispatcher::notify_circleofdoom_info(int skill_id, int sponsor_id, int member_count, const int *pmember_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_circleofdoom_info>::From(_tbuf, skill_id, sponsor_id, member_count, pmember_id);
	
	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf, -1);
}

void gplayer_dispatcher::notify_circleofdoom_info_single(const XID & target, int cs_index, int sid, int skill_id, 
		int sponsor_id, int member_count, const int *pmember_id)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::notify_circleofdoom_info>::From(_tbuf, skill_id, sponsor_id, member_count, pmember_id);
  
	send_ls_msg(cs_index, target.id, sid,_tbuf);
}

void gplayer_dispatcher::notify_circleofdoom_stop(int sponsor_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_circleofdoom_stop>::From(_tbuf, sponsor_id);

	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf, -1);
}

void gplayer_dispatcher::vip_award_info(int vip_end_time, int recent_total_cash, std::vector<S2C::CMD::vip_award_info::award_data> & award_list)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::vip_award_info>::From(_tbuf, vip_end_time, recent_total_cash, award_list.size());

	for(size_t i = 0; i < award_list.size(); i++)
	{
		CMD::Make<CMD::vip_award_info>::FromData(_tbuf, award_list[i]);
	}

	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::vip_award_change(int roleid, unsigned char vipaward_level, char is_hide)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::vip_award_change>::From(_tbuf, roleid, vipaward_level, is_hide);

	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf, -1);
}

void gplayer_dispatcher::online_award_info(char force_flag, std::vector<S2C::CMD::online_award_info::award_data> & award_list)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::online_award_info>::From(_tbuf, force_flag, award_list.size());

	for(size_t i = 0; i < award_list.size(); i++)
	{
		CMD::Make<CMD::online_award_info>::FromData(_tbuf, award_list[i]);
	}

	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::online_award_permit(int awardid, int awardindex, char permitcode)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::online_award_permit>::From(_tbuf, awardid, awardindex, permitcode);

	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mall_item_price(packet_wrapper &goodslist)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	ASSERT( (goodslist.size() % sizeof(CMD::mall_item_price::saleitem_list)) == 0 );
	size_t count = goodslist.size() / sizeof(CMD::mall_item_price::saleitem_list);
	CMD::Make<CMD::mall_item_price>::From(_tbuf, count);
	if(count > 0)
	{
		_tbuf.push_back(goodslist.data(), goodslist.size());
	}

	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_achievement()
{
	_tbuf.clear();
	size_t map_size;
	const void * map_buf  = ((gplayer_imp*)_imp)->GetAchievementManager().GetMap().data(map_size);
	raw_wrapper wrapper(2048);
	((gplayer_imp*)_imp)->GetAchievementManager().GetClientActiveAchievement(wrapper);

	size_t spec_size;
	const void * spec_buf = ((gplayer_imp*)_imp)->GetAchievementManager().GetSpecialAchievementInfo(spec_size);

	// Youshuang add
	size_t award_map_size;
	const void* award_buf = ((gplayer_imp*)_imp)->GetAchievementManager().SaveAchievementAward( award_map_size );
	// end

	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_achievement>::From(_tbuf, map_size, map_buf, wrapper.size(), wrapper.data(), spec_size, spec_buf, award_map_size, ( const unsigned char* )award_buf );
	send_ls_msg(pPlayer,_tbuf);
}
void gplayer_dispatcher::player_achievement_data(size_t size, const void * data) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_achievement_active>::From(_tbuf, data, size);
	send_ls_msg(pPlayer,_tbuf);
}
void gplayer_dispatcher::player_achievement_finish(unsigned short achieve_id, int achieve_point, int finish_time) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_achievement_finish>::From(_tbuf, achieve_id, achieve_point, finish_time);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_premiss_data(unsigned short achieve_id, char premiss_id, size_t size, const void *data) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_premiss_data>::From(_tbuf, achieve_id, premiss_id, data, size);
	send_ls_msg(pPlayer,_tbuf);
} 
void gplayer_dispatcher::player_premiss_finish(unsigned short achieve_id, char premiss_id) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_premiss_finish>::From(_tbuf, achieve_id, premiss_id);
	send_ls_msg(pPlayer,_tbuf);
}
void gplayer_dispatcher::player_achievement_map(int cs_index, int cs_sid,int target,int achieve_point, const void * data, size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_achievement_map>::From(_tbuf,pPlayer->ID, achieve_point, data,size);
	send_ls_msg(cs_index,target,cs_sid,_tbuf);
}
void gplayer_dispatcher::achievement_message(unsigned short achieve_id, int param, int finish_time)
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::achievement_message>::From(_tbuf, ((gplayer_imp*)_imp)->_username, ((gplayer_imp*)_imp)->_username_len, achieve_id, param, finish_time);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::self_instance_info(int level_score, int monster_score, int time_score, int death_penalty_score)
{
	_tbuf.clear(); 
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_instance_info>::From(_tbuf,level_score, monster_score, time_score, death_penalty_score);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::instance_info(int cur_level, size_t size, const void *data)
{
	_tbuf.clear(); 
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::instance_info>::From(_tbuf, cur_level, data, size);  
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_start_transform(int template_id, char active, char type, char level, char expLevel)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_transform>::From(_tbuf,pPlayer, template_id, active, type, level, expLevel);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::magic_duration_decrease(short index, short amount ,int result) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::magic_duration_decrease>::From(_tbuf,index, amount, result);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::magic_exp_notify(int where, int index, int exp) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::magic_exp_notify>::From(_tbuf, where, index, exp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::transform_skill_data()
{
	_tbuf.clear();
	using namespace S2C;
	raw_wrapper wrapper(64);
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	if(!pImp->GetTransformSkillData(wrapper)) return;
	CMD::Make<CMD::transform_skill_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::player_enter_carrier(int carrier_id,const A3DVECTOR& rpos,unsigned char rdir, unsigned char success)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_carrier>::From(_tbuf, pPlayer, carrier_id, rpos, rdir, success);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_leave_carrier(int carrier_id,const A3DVECTOR& pos,unsigned char dir, unsigned char success)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_leave_carrier>::From(_tbuf,pPlayer, carrier_id, pos, dir, success);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_move_on_carrier(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::player_move_on_carrier>::From(_tbuf,pObj,target,cost_time,speed,move_mode);

	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pObj->ID.id);//自己也可以收到这个消息
}

void
gplayer_dispatcher::player_stop_move_on_carrier(const A3DVECTOR & target, unsigned short speed ,unsigned char dir,unsigned char move_mode)
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::player_stop_move_on_carrier>::From(_tbuf,pObj,target,speed,dir,move_mode);

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	pPlayer->dir = pPlayer->dir + dir - pPlayer->rdir -64;
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pObj->ID.id);
}

void
gplayer_dispatcher::skill_proficiency(int id, int proficiency)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::skill_proficiency>::From(_tbuf,id, proficiency);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::notify_mall_prop(int mall_prop)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_mall_prop>::From(_tbuf,mall_prop);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::send_pk_message(int killer, int deader)
{
	_tbuf.clear();
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::send_pk_message>::From(_tbuf,killer, deader);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::inc_invisible(int pre, int cur)
{
	ASSERT(cur > pre);
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer *)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(_tbuf,pPlayer,false);
	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,cur,pre,pPlayer->team_id,pPlayer->ID.id);
}

void 
gplayer_dispatcher::dec_invisible(int pre, int cur)
{
	ASSERT(cur < pre);
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_slice>::From(_tbuf,pPlayer,pPlayer->pos);
	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pre,cur,pPlayer->team_id,pPlayer->ID.id);
}

void 
gplayer_dispatcher::inc_anti_invisible(int pre, int cur)
{
	ASSERT(cur > pre);

	begin_transfer();
	world * pPlane = _imp->_plane;
	slice * pPiece = _imp->_parent->pPiece;
	get_slice_player_info(pPiece,_nw,_mw,_pw,cur, pre);

	int i;
	int total = pPlane->w_far_vision;
	int index = pPlane->GetGrid().GetSliceIndex(pPiece);
	int slice_x,slice_z;
	pPlane->GetGrid().Index2Pos(index,slice_x,slice_z);
	for(i = 0; i <total; i ++)
	{
		world::off_node_t &node = pPlane->w_off_list[i];
		int nx = slice_x + node.x_off;
		int nz = slice_z + node.z_off;
		if(nx < 0 || nz < 0 || nx >= pPlane->GetGrid().reg_column || nz >= pPlane->GetGrid().reg_row) continue;
		slice * pNewPiece = pPiece + node.idx_off;
		if(i <= pPlane->w_true_vision)
		{
			get_slice_player_info(pNewPiece,_nw,_mw,_pw,cur, pre);
		}
		else                                                                                                 
		{                                                                                                    
			get_slice_player_info(pNewPiece,_nw,_mw,_pw,cur, pre);
		}                                                                                                    
	}                                                                                                            

	end_transfer();                                                                                              
	return ;
}

void 
gplayer_dispatcher::dec_anti_invisible(int pre, int cur)
{
	ASSERT(cur < pre);

	begin_transfer();
	world * pPlane = _imp->_plane;
	slice * pPiece = _imp->_parent->pPiece;
	int team_id = ((gplayer*)_imp->_parent)->team_id;
	gather_slice_player(pPiece,_leave_list,pre,cur,team_id);

	int i;
	int total = pPlane->w_far_vision;
	int index = pPlane->GetGrid().GetSliceIndex(pPiece);
	int slice_x,slice_z;
	pPlane->GetGrid().Index2Pos(index,slice_x,slice_z);
	for(i = 0; i <total; i ++)
	{       
		world::off_node_t &node = pPlane->w_off_list[i];
		int nx = slice_x + node.x_off;
		int nz = slice_z + node.z_off; 
		if(nx < 0 || nz < 0 || nx >= pPlane->GetGrid().reg_column || nz >= pPlane->GetGrid().reg_row) continue;
		slice * pNewPiece = pPiece + node.idx_off;
		if(i <= pPlane->w_true_vision)
		{       
			gather_slice_player(pNewPiece,_leave_list,pre,cur,team_id);
		}
		else
		{       
			gather_slice_player(pNewPiece,_leave_list,pre,cur,team_id);
		}
	}

	end_transfer();
	return ;
}

void
gplayer_dispatcher::update_combo_state(char state, char * color)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::update_combo_state>::From(_tbuf,state, color);
	send_ls_msg(pPlayer,_tbuf);

}

void
gplayer_dispatcher::enter_dim_state(bool state)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_dim_state>::From(_tbuf,pPlayer, state); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::enter_invisible_state(bool state)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_invisible_state>::From(_tbuf,pPlayer, state); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::enter_territory( unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_territory>::From( _tbuf, battle_faction, battle_type, battle_id, end_timestamp );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::leave_territory()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_territory>::From( _tbuf );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::last_logout_time(int time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::last_logout_time>::From( _tbuf, time);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::circle_info_notify()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::circle_info_notify>::From(_tbuf,pPlayer); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::receive_deliveryd_exp(int64_t exp, unsigned int award_type) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::receive_deliveryd_exp>::From(_tbuf, exp, award_type); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::lottery2_bonus(int lottery_id , int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::lottery2_bonus>::From(_tbuf,lottery_id, bonus_index, bonus_level,bonus_item,bonus_count,bonus_money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::lottery3_bonus(int lottery_id , int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::lottery3_bonus>::From(_tbuf,lottery_id, bonus_index, bonus_level,bonus_item,bonus_count,bonus_money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::gem_notify(int gem_id, int msg)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::gem_notify>::From(_tbuf, gem_id, msg);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}


void gplayer_dispatcher::player_scale(char on, int scale_ratio)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::player_scale>::From(h1, pObject, on, scale_ratio);
	AutoBroadcastCSMsg(_imp->_plane, pObject->pPiece, h1 , -1);
}

void gplayer_dispatcher::player_move_cycle_area(char mode, char type, int idx, float radius, const A3DVECTOR& pos)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_move_cycle_area>::From(_tbuf, mode, type, idx, radius, pos);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::enter_raid( unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_raid>::From( _tbuf, raid_faction, raid_type, raid_id, end_timestamp );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::leave_raid()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_raid>::From( _tbuf );
	send_ls_msg(pPlayer,_tbuf);
}
/*liuyue-facbase
void 
gplayer_dispatcher::notify_raid_pos(const A3DVECTOR & pos)
{	
	_tbuf.clear();
	using namespace S2C;
	int raid_id = _imp->GetWorldManager() ? _imp->GetWorldManager()->GetRaidID() : -1;
	CMD::Make<CMD::notify_raid_pos>::From(_tbuf,pos,raid_id,_imp->GetWorldTag());
	send_ls_msg((gplayer*)(_imp->_parent),_tbuf);
}
*/
void gplayer_dispatcher::player_raid_counter()
{
	
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	raw_wrapper wrapper(256);

	pImp->SaveRaidCountersClient(wrapper);
	CMD::Make<CMD::player_raid_counter>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_single_raid_counter(int map_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_single_raid_counter>::From(_tbuf, map_id, pImp->GetRaidCounter(map_id));
	send_ls_msg((gplayer*)(_imp->_parent),_tbuf);
}

void gplayer_dispatcher::send_raid_global_vars(int cnt, const void* buf, size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::raid_global_var>::From(_tbuf, cnt, buf, size);
	send_ls_msg((gplayer*)(_imp->_parent),_tbuf);
}

void gplayer_dispatcher::be_moved(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_be_moved>::From(_tbuf,pPlayer,target,cost_time,speed,move_mode);
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_pulling(bool on) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_pulling>::From(_tbuf, on);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_be_pulled(int player_pulling_id, bool on, unsigned char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_be_pulled>::From(_tbuf, player_pulling_id, pPlayer->ID.id, on, type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_be_pulled(const XID & target, int cs_index, int sid, int player_pulling_id, bool on, unsigned char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_be_pulled>::From(_tbuf, player_pulling_id, pPlayer->ID.id, on, type);
	send_ls_msg(cs_index, target.id, sid,_tbuf);
}

void gplayer_dispatcher::raid_info(const char* buf, size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	ASSERT( size % sizeof(CMD::raid_info::player_info_in_raid) == 0 );
	unsigned char player_count = size / sizeof(CMD::raid_info::player_info_in_raid);
	CMD::Make<CMD::raid_info>::From(_tbuf, player_count, (CMD::raid_info::player_info_in_raid*)buf );
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::trigger_skill_time(short skill_id, short time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trigger_skill_time>::From(_tbuf, skill_id, time);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_stop_skill(char spirit_index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_skill>::From(_tbuf, pPlayer->ID.id, spirit_index);
	//改为广播，客户端需要清楚施法加速减速状态
	gactive_imp * pImp = (gactive_imp*)_imp;
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,-1);
}
	
void gplayer_dispatcher::addon_skill_permanent(int skill, int level)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::addon_skill_permanent>::From(_tbuf,skill,level);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::get_addon_skill_permanent_data()
{
	_tbuf.clear();
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	raw_wrapper wrapper(256);
	pImp->_skill.StorePartialAddonPermanent(wrapper);
	CMD::Make<CMD::addon_skill_permanent_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::zx_five_anni_data(char star, char state)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::zx_five_anni_data>::From(_tbuf, star, state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


void gplayer_dispatcher::treasure_info()
{
	_tbuf.clear();
	raw_wrapper wrapper(128);
	((gplayer_imp*)_imp)->GetClientTreasureInfo(wrapper);

	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::treasure_info>::From(_tbuf, wrapper.size(), wrapper.data());
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::treasure_region_upgrade(int region_id, int region_index, bool success, int new_level)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::treasure_region_upgrade>::From(_tbuf, region_id, region_index, success, new_level); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::treasure_region_unlock(int region_id, int region_index, bool success)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::treasure_region_unlock>::From(_tbuf, region_id, region_index, success); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::treasure_region_dig(int region_id, int region_index, bool success, char event_id, size_t award_item_num, void * buf) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::treasure_region_dig>::From(_tbuf, region_id, region_index, success, event_id, award_item_num, buf); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::inc_treasure_dig_count(int inc_count, int total_dig_count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::inc_treasure_dig_count>::From(_tbuf, inc_count, total_dig_count); 
	send_ls_msg(pPlayer,_tbuf);
}


void gplayer_dispatcher::littlepet_info(char level, char cur_award_lvl, int cur_exp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::littlepet_info>::From(_tbuf, level, cur_award_lvl, cur_exp); 
	send_ls_msg(pPlayer,_tbuf);
}


void gplayer_dispatcher::rune_info(bool active, int rune_score, int customize_info)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::rune_info>::From(_tbuf, active, rune_score, customize_info); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::rune_refine_result(char type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::rune_refine_result>::From(_tbuf, type); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::random_tower_monster(int level, char reenter)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::random_tower_monster>::From(_tbuf, level, reenter); 
	send_ls_msg(pPlayer,_tbuf);
	
}

void gplayer_dispatcher::random_tower_monster_result(int monster_tid, char client_idx)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::random_tower_monster_result>::From(_tbuf, monster_tid, client_idx); 
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_tower_data()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	raw_wrapper wrapper(256);
	pImp->SaveTowerDataClient(wrapper);
	CMD::Make<CMD::player_tower_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_tower_monster_list()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	raw_wrapper wrapper(256);
	pImp->SaveTowerMonsterListClient(wrapper);
	CMD::Make<CMD::player_tower_monster_list>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_tower_give_skills(const void* buf, size_t size)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::player_tower_give_skills>::From(_tbuf,buf, size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::set_tower_give_skill(int skill_id, int skill_level, bool used)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::set_tower_give_skill>::From(_tbuf, skill_id, skill_level, used);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::add_tower_monster(int monster_tid)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::add_tower_monster>::From(_tbuf, monster_tid);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::set_tower_reward_state(char type, int level, bool delivered)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::set_tower_reward_state>::From(_tbuf, type, level, delivered);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::tower_level_pass(int level, int best_time) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::tower_level_pass>::From(_tbuf, level, best_time);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::fill_platform_mask(int mask) 
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::fill_platform_mask>::From(_tbuf, mask); 
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_player_pk_bet_data()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	raw_wrapper wrapper(256);
	pImp->SavePlayerBetDataClient(wrapper);
	CMD::Make<CMD::pk_bet_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::put_item(int type, int index, int expire_date, int amount, int slot_amount, int where,int state)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::put_item>::From(_tbuf,type, expire_date, index & 0xFF, amount & 0xFFFF,slot_amount & 0xFFFF,where & 0xFF,state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::object_start_special_move(int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_start_special_move>::From(_tbuf,id, velocity, acceleration, cur_pos, collision_state, timestamp);

	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg2(_imp->_plane,pPiece,_tbuf.data(), _tbuf.size(),pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg2(_imp->_plane,pPiece,_tbuf.data(), _tbuf.size(),pPlayer->ID.id);
}


void
gplayer_dispatcher::object_special_move(int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_special_move>::From(_tbuf,id, velocity, acceleration, cur_pos, collision_state, timestamp);

	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg2(_imp->_plane,pPiece,_tbuf.data(), _tbuf.size(),pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg2(_imp->_plane,pPiece,_tbuf.data(), _tbuf.size(),pPlayer->ID.id);
}


void
gplayer_dispatcher::object_stop_special_move(int id, unsigned char dir, A3DVECTOR & cur_pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_stop_special_move>::From(_tbuf,id, dir, cur_pos); 

	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsg2(_imp->_plane,pPiece,_tbuf.data(), _tbuf.size(), pPlayer,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg2(_imp->_plane,pPiece,_tbuf.data(), _tbuf.size(), pPlayer->ID.id);
}


void
gplayer_dispatcher::player_change_name(int id, int name_len, char * name)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_name>::From(_tbuf,id, name_len, name); 

	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf);
}

void
gplayer_dispatcher::change_faction_name(int id, int fid, char type, int name_len, char * name)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::change_faction_name>::From(_tbuf,id, fid, type, name_len, name); 

	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf);
}


void
gplayer_dispatcher::enter_collision_raid(unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_collision_raid>::From(_tbuf, pPlayer, raid_faction, raid_type, raid_id, end_timestamp); 
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::collision_special_state(int type, char on) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::collision_special_state>::From(_tbuf,pPlayer, type, on);
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf , -1);

}

void
gplayer_dispatcher::collision_raid_result(char result, int score_a, int score_b) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::collision_raid_result>::From(_tbuf,result, score_a, score_b); 
	send_ls_msg(pPlayer, _tbuf);

}

void
gplayer_dispatcher::collision_raid_info(int score_a, int score_b, int win_num, int lost_num, int draw_num, int daily_award_timestamp, int last_collision_timestamp, int day_win,
		int cs_personal_award_timestamp, int cs_exchange_award_timestamp, int cs_team_award_timestamp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::collision_raid_info>::From(_tbuf, score_a, score_b, win_num, lost_num, draw_num, daily_award_timestamp, last_collision_timestamp, day_win,
			cs_personal_award_timestamp, cs_exchange_award_timestamp, cs_team_award_timestamp); 

	raw_wrapper wrapper(256);
	gplayer_imp * pImp = (gplayer_imp *)_imp;	
	pImp->SaveExchange6V6AwardInfoForClient(wrapper);
	_tbuf.push_back(wrapper.data(), wrapper.size());
	send_ls_msg(pPlayer, _tbuf);
}


void gplayer_dispatcher::player_darklight()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;	
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_darklight>::From(_tbuf, pPlayer->ID.id, pImp->_dark_light); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_darklight_state()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_darklight_state>::From(_tbuf, pPlayer->ID.id, pPlayer->darklight_state);
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf , -1);
}

void gplayer_dispatcher::player_darklight_spirit()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_darklight_spirit>::From(_tbuf, pPlayer->ID.id, pPlayer->darklight_spirits);
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf , -1);
}

void gplayer_dispatcher::mirror_image_cnt(int cnt)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mirror_image_cnt>::From(_tbuf, pPlayer->ID.id, cnt);
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf , -1);
}

void gplayer_dispatcher::player_wing_change_color(int id, char wing_color)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_wing_change_color>::From(_tbuf, id, wing_color); 
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf , -1);
}

void gplayer_dispatcher::notify_astrology_energy()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::notify_astrology_energy>::From(_tbuf, pImp->IsAstrologyClientActive(), pImp->GetAstrologyEnergy());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::liveness_notify()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp*)_imp;
	char is_taken[4];
	for (int i = 0; i < 4; i++) 
	{
		is_taken[i]	 = (pImp->_liveness_info.grade_timestamp[i]) ? 1 : 0;
	}
	std::vector<int> achieved_ids;
	for (std::multimap<int, int>::iterator it = pImp->_liveness_info.classifyid2ids.begin(); it != pImp->_liveness_info.classifyid2ids.end(); ++it) 
	{
		achieved_ids.push_back(it->second);	
	}
	CMD::Make<CMD::liveness_notify>::From(_tbuf, pImp->_liveness_info.cur_point, is_taken, achieved_ids); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::ui_transfer_opened_notify()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	raw_wrapper wrapper(256);
	wrapper << (unsigned short)((gplayer_imp*)_imp)->_ui_transfer_info.opened_indexes.size();	
	for (abase::static_set<int>::iterator it = ((gplayer_imp*)_imp)->_ui_transfer_info.opened_indexes.begin();
	   	it != ((gplayer_imp*)_imp)->_ui_transfer_info.opened_indexes.end(); it++) 
	{
		wrapper << *it;	
	}
	CMD::Make<CMD::ui_transfer_opened_notify>::From(_tbuf, wrapper.data(), wrapper.size()); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::astrology_upgrade_result(bool success, int new_level)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::astrology_upgrade_result>::From(_tbuf, success, new_level); 
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::astrology_destroy_result(int gain_energy)
{
	_tbuf.clear();
	using namespace S2C;
	CMD::Make<CMD::astrology_destroy_result>::From(_tbuf, gain_energy); 
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::dir_visible_state(int id, int dir, bool on)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::dir_visible_state>::From(_tbuf, pPlayer->ID.id, id, dir, on); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_remedy_metempsychosis_level()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_remedy_metempsychosis_level>::From(_tbuf); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::collision_player_pos(int roleid, A3DVECTOR & pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::collision_player_pos>::From(_tbuf, roleid, pos); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::enter_kingdom(unsigned char battle_faction, char kingdom_type, char cur_phase, int end_timestamp, int defender_mafia_id,  std::vector<int> & attacker_mafia_ids) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_kingdom>::From(_tbuf, battle_faction, kingdom_type, cur_phase, end_timestamp, defender_mafia_id, attacker_mafia_ids);
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::leave_kingdom(char kingdom_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_kingdom>::From(_tbuf, kingdom_type); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::kingdom_half_end(int win_faction, int end_timestamp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::kingdom_half_end>::From(_tbuf, win_faction, end_timestamp); 
	send_ls_msg(pPlayer, _tbuf);
}


void gplayer_dispatcher::kingdom_end_result(int win_mafia_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::kingdom_end_result>::From(_tbuf, win_mafia_id);
	send_ls_msg(pPlayer, _tbuf);
}


void gplayer_dispatcher::raid_level_start(char level)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::raid_level_start>::From(_tbuf, level); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::raid_level_end(char level, bool result)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::raid_level_end>::From(_tbuf, level, result); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::raid_level_result(bool final, bool result[10], bool final_award_deliveried, bool level_award_deliveried[10])
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::raid_level_result>::From(_tbuf, final, result, final_award_deliveried, level_award_deliveried); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::raid_level_info(char level, short matter_cnt, int start_time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::raid_level_info>::From(_tbuf, level, matter_cnt, start_time); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::sale_promotion_info(std::set<int> taskids)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::sale_promotion_info>::From(_tbuf, taskids); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::event_info()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;

	std::set<int> event_list;
	gmatrix::GetEventList(event_list);
	CMD::Make<CMD::event_info>::From(_tbuf, event_list); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::cash_gift_ids(int id1, int id2)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;

	CMD::Make<CMD::cash_gift_ids>::From(_tbuf, id1, id2); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::notify_cash_gift(int gift_id, int gift_index, int gift_score) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;

	CMD::Make<CMD::notify_cash_gift>::From(_tbuf, gift_id, gift_index, gift_score); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_propadd() 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_propadd>::From(_tbuf, pImp->_prop_add); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_get_propadd_item(int item_id, int item_idx) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_get_propadd_item>::From(_tbuf, item_id, item_idx); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_extra_equip_effect()
{

	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_extra_equip_effect>::From(_tbuf, pPlayer->ID.id, pPlayer->weapon_effect_level); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf.data(),_tbuf.size(),-1);
}

void gplayer_dispatcher::kingdom_key_npc_info(int cur_hp, int max_hp, int hp_add, int hp_dec)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::kingdom_key_npc_info>::From(_tbuf, cur_hp, max_hp, hp_add, hp_dec); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::notify_kingdom_info(int kingdom_title, int kingdom_point)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_kingdom_info>::From(_tbuf, kingdom_title, kingdom_point); 
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::cs_flow_player_info(int score, int c_kill_cnt, bool m_kill_change, int m_kill_cnt, short kill_cnt, short death_cnt, short max_ckill_cnt, short max_mkill_cnt)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cs_flow_player_info>::From(_tbuf, pPlayer->ID.id, score, c_kill_cnt, m_kill_change, m_kill_cnt, kill_cnt, death_cnt, max_ckill_cnt, max_mkill_cnt); 
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf, -1);
}

void 
gplayer_dispatcher::player_bath_invite(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bath_invite>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bath_invite_reply(const XID & target, int param)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bath_invite_reply>::From(_tbuf,target,param);
	send_ls_msg(pPlayer,_tbuf);
}

//被邀请人发出的
void 
gplayer_dispatcher::player_bath_start(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bath_start>::From(_tbuf,target, pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_bath_stop()
{
	//这个start始终是骑在上面那个法出的
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bath_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::enter_bath_world(int remain_bath_count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_bath_world>::From(_tbuf, remain_bath_count);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::leave_bath_world()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_bath_world>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::flow_battle_info( const char* buf, size_t size )
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	ASSERT( size % sizeof(CMD::flow_battle_info::player_info_in_flow_battle) == 0 );
	unsigned char player_count = size / sizeof(CMD::flow_battle_info::player_info_in_flow_battle);
	CMD::Make<CMD::flow_battle_info>::From(_tbuf, player_count, (CMD::flow_battle_info::player_info_in_flow_battle*)buf );
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::bath_gain_item(int item_id, int item_count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::bath_gain_item>::From(_tbuf, pPlayer->ID.id, item_id, item_count);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::fuwen_compose_info(int compose_count, int extra_compose_count) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fuwen_compose_info>::From(_tbuf, compose_count, extra_compose_count); 
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::fuwen_compose_end()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fuwen_compose_end>::From(_tbuf); 
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::fuwen_install_result(int src_index, int dst_index) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fuwen_install_result>::From(_tbuf, src_index, dst_index); 
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::fuwen_uninstall_result(int fuwen_index, int inv_index) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fuwen_uninstall_result>::From(_tbuf, fuwen_index, inv_index); 
	send_ls_msg(pPlayer,_tbuf);
}


void
gplayer_dispatcher::multi_exp_mode(int multi_exp_factor, int timestamp) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::multi_exp_mode>::From(_tbuf, multi_exp_factor, timestamp); 
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::web_order_info()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;

	abase::vector<web_order> & web_order = pImp->_mallinfo._web_order_list;
	if(web_order.empty()) return;

	int order_count = 0;
	for(size_t i = 0; i < web_order.size(); ++i)
	{
		if(web_order[i]._roleid == pImp->GetParent()->ID.id) 
		{
			order_count++;
		}
	}
	if(order_count <= 0) return;


	_tbuf << (unsigned short)S2C::WEB_ORDER_INFO;
	_tbuf << order_count; 
	for(size_t i = 0; i < web_order.size(); ++i)
	{
		if(web_order[i]._roleid != pImp->GetParent()->ID.id) continue;
		
		_tbuf << web_order[i]._order_id << web_order[i]._pay_type << web_order[i]._status << web_order[i]._timestamp;
		_tbuf << web_order[i]._order_package.size();
		
		for(size_t j = 0; j < web_order[i]._order_package.size(); ++j)
		{
			_tbuf << web_order[i]._order_package[j]._package_id << web_order[i]._order_package[j]._name_len; 
			_tbuf.push_back(web_order[i]._order_package[j]._package_name, web_order[i]._order_package[j]._name_len);
			_tbuf << web_order[i]._order_package[j]._count << web_order[i]._order_package[j]._price;
			_tbuf <<  web_order[i]._order_package[j]._order_goods.size();

			for(size_t k = 0; k < web_order[i]._order_package[j]._order_goods.size(); ++k)
			{
				_tbuf << web_order[i]._order_package[j]._order_goods[k]._goods_id
				      << web_order[i]._order_package[j]._order_goods[k]._goods_count
				      << web_order[i]._order_package[j]._order_goods[k]._goods_flag
				      << web_order[i]._order_package[j]._order_goods[k]._goods_time;
			}
		}
	}
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::control_trap_info(int id, int tid, int time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::control_trap_info>::From(_tbuf, id, tid, time); 
	send_ls_msg(pPlayer,_tbuf);
}	



void
gplayer_dispatcher::puppet_form(bool on, int player_id, int puppet_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::puppet_form>::From(_tbuf, on, player_id, puppet_id); 
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf, -1);
}	

void
gplayer_dispatcher::teleport_skill_info(int player_id, int npc_tid, int skill_id, int teleport_count)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::teleport_skill_info>::From(_tbuf, player_id, npc_tid, skill_id, teleport_count); 
	AutoBroadcastCSMsg(_imp->_plane, pPlayer->pPiece, _tbuf, -1);
}	

void gplayer_dispatcher::player_mobactive_state_start(const XID & playerid,const XID & mobactive, int pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mob_active_ready_start>::From(_tbuf,playerid, mobactive, pos);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_mobactive_state_run(const XID & playerid,const XID & mobactive, int pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mob_active_running_start>::From(_tbuf,playerid, mobactive, pos);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_mobactive_state_ending(const XID & playerid,const XID & mobactive, int pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mob_active_end_start>::From(_tbuf,playerid, mobactive, pos);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_mobactive_state_finish(const XID & playerid,const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mob_active_end_finish>::From(_tbuf,playerid, mobactive, pos, player_pos, mobactive_pos);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void gplayer_dispatcher::player_mobactive_state_cancel(const XID& playerid,const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mob_active_cancel>::From(_tbuf,playerid, mobactive, pos, player_pos, mobactive_pos);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); 
}

void 
gplayer_dispatcher::player_qilin_invite(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_qilin_invite>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_qilin_invite_reply(const XID & target, int param)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_qilin_invite_reply>::From(_tbuf,target,param);
	send_ls_msg(pPlayer,_tbuf);
}

//被邀请人发出的
void 
gplayer_dispatcher::player_qilin_start(const XID & target)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_qilin_start>::From(_tbuf,target, pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_qilin_stop()
{
	//这个start始终是骑在上面那个法出的
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_qilin_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_qilin_disconnect()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_qilin_disconnect>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::player_qilin_reconnect(const A3DVECTOR & newpos)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_qilin_reconnect>::From(_tbuf,pPlayer->ID, newpos);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}


void 
gplayer_dispatcher::phase_info()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	raw_wrapper wrapper(256);

	pImp->SavePhaseInfoForClient(wrapper);
	if(wrapper.size() == 0) return;

	CMD::Make<CMD::phase_info>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::phase_info_change(int phase_id, bool on)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::phase_info_change>::From(_tbuf,phase_id, on);
	send_ls_msg(pPlayer,_tbuf);
}
/*
void 
gplayer_dispatcher::fac_building_add(int index, int tid)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fac_building_add>::From(_tbuf, index, tid);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::fac_building_upgrade(int index, int tid, int lev)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fac_building_upgrade>::From(_tbuf, index, tid, lev);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::fac_building_complete(int index, int tid, int level)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fac_building_complete>::From(_tbuf, index, tid, level);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::fac_building_remove(int index)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::fac_building_remove>::From(_tbuf, index);
	send_ls_msg(pPlayer,_tbuf);

}
*/
void
gplayer_dispatcher::player_fac_base_info(const S2C::CMD::player_fac_base_info & base_info, const std::vector<S2C::INFO::player_fac_field> & field_info, const abase::octets & msg)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_fac_base_info>::From(_tbuf, base_info, field_info, msg.begin(), msg.size());
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_active_emote_invite(const XID& target, int active_emote_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_active_emote_invite>::From(_tbuf,target,active_emote_type);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_active_emote_invite_reply(const XID& target,int param)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_active_emote_invite_reply>::From(_tbuf,target,param);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_active_emote_start(const XID & target, int active_emote_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_active_emote_start>::From(_tbuf,target,pPlayer->ID,active_emote_type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_active_emote_stop(const XID & target, int active_emote_type)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_active_emote_stop>::From(_tbuf,target,pPlayer->ID,active_emote_type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::be_taunted2(const XID & who, int time)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::be_taunted2>::From(_tbuf, who, time);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_change_puppet_form()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_puppet_form>::From(_tbuf, pPlayer->ID.id);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::notify_bloodpool_status(bool on, int cur_hp, int max_hp)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::notify_bloodpool_status>::From(_tbuf, pPlayer->ID.id, on, cur_hp, max_hp);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::object_try_charge(char type, const A3DVECTOR & dest, int target_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_try_charge>::From(_tbuf, type, target_id, dest); 
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_start_travel_around(int travel_vehicle_id, float travel_speed, int travel_path_id)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_travel_around>::From(_tbuf, pPlayer->ID.id, travel_vehicle_id, travel_speed, travel_path_id); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_stop_travel_around()
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_travel_around>::From(_tbuf, pPlayer->ID.id); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::cs6v6_cheat_info(bool is_cheat, int cheat_counter) 
{
	_tbuf.clear();
	using namespace S2C;
	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cs6v6_cheat_info>::From(_tbuf, is_cheat, cheat_counter);
	send_ls_msg(pPlayer,_tbuf);
}


void
gplayer_dispatcher::hide_and_seek_raid_info(int last_hide_and_seek_timestamp, int day_has_get_award)
{
	_tbuf.clear();
	using namespace S2C;
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::hide_and_seek_raid_info>::From(_tbuf,last_hide_and_seek_timestamp, day_has_get_award);
	send_ls_msg(pPlayer,_tbuf);
}
