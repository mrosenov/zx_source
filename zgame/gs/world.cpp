#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "config.h"
#include "world.h"
#include "clstab.h"
#include "npc.h"
#include "matter.h"
#include "player_imp.h"

#include "usermsg.h"
#include "template/itemdataman.h"
#include "task/taskman.h"
#include <factionlib.h>

abase::timer 	g_timer(1000,300000);

world::world():w_player_map(10),w_pmap_lock(0),w_nmap_lock(0),w_ctrl(NULL)
{
	w_index = 0;
	w_world_man = NULL;
	_message_handle_count = 0;
	memset(w_message_counter,0,sizeof(w_message_counter));
	w_player_count = 0;
	w_obsolete = 0;
	w_activestate = 0;
	w_index_in_man = -1;
	w_create_timestamp = -1;
	w_ins_kick = 1;
	w_battle_result = 0;
	w_offense_goal	= 0;
	w_offense_cur_score = 0;
	w_defence_goal = 0;
	w_defence_cur_score = 0;
	w_end_timestamp = 0;
	w_near_vision  = 0;
	w_true_vision = 0;
}

world::~world()
{
	if(w_ctrl)
	{
		delete w_ctrl;
		w_ctrl = NULL;
	}
}

void 
world::SetWorldCtrl(world_data_ctrl * ctrl)
{
	ASSERT(w_ctrl == NULL);
	w_ctrl = ctrl;
}

bool
world::Init(int world_index, int world_tag)
{
	w_plane_index = 0;
	w_index = world_index;
	w_tag   = world_tag;
	return true;
}

void
world::RunTick()
{
	//第二套运行方式
	w_npc_gen.Run();
	if(w_ctrl)
	{
		w_ctrl->Tick(this);
	}
}


bool    
world::InitNPCGenerator(CNPCGenMan & npcgen)
{
	if(!w_npc_gen.LoadGenData(w_world_man, npcgen,GetGrid().local_region))
	{
		return false;
	}
	w_npc_gen.InitIncubator(this);
	w_npc_gen.StartHeartbeat();
	if(!w_npc_gen.BeginSpawn()) return false;
	return true;
}

bool 
world::TriggerSpawn(int condition)
{
	if(condition == 0) return false;
	return w_npc_gen.TriggerSpawn(condition);
}

bool 
world::ClearSpawn(int condition)
{
	if(condition == 0) return false;
	w_npc_gen.ClearSpawn(condition);
	return true;
}

bool 
world::CreateGrid(int row,int column,float step,float sx,float sy)
{
	return w_grid.Create(row,column,step,sx,sy);
}

static void 
insert_unique(abase::vector<world::off_node_t, abase::fast_alloc<> > &list,const world::off_node_t& node)
{
	if(std::find(list.begin(),list.end(),node) == list.end())
	{
		list.push_back(node);
	}
}

int 
world::BuildSliceMask(float near,float far)
{
	if(far < near) return -1;
	w_off_list.clear();
	float inv_step = w_grid.inv_step;
	int n1 = (int)(near * inv_step); 
	int f1 = (int)(far * inv_step); 
	int tf1 = (int)((far-w_grid.slice_step) * inv_step);
	if( fabs(near - n1 * w_grid.slice_step) > 1e-3) n1 ++;
	if( fabs(far - f1 * w_grid.slice_step) > 1e-3) f1 ++;
	for(int i = 1; i <= f1; i ++)
	{
		for(int j =-i; j < i; j ++)
		{
			insert_unique(w_off_list,off_node_t(w_grid,j,-i));
			insert_unique(w_off_list,off_node_t(w_grid,i,j));
			insert_unique(w_off_list,off_node_t(w_grid,-j,i));
			insert_unique(w_off_list,off_node_t(w_grid,-i,-j));
		}
		if(n1 == i) w_near_vision = w_off_list.size();
		if(tf1 == i) w_true_vision = w_off_list.size();
	}
	w_far_vision = w_off_list.size();
	w_vision = far;
	return 0;
}
	
void 
world::RemovePlayer(gplayer *pPlayer)
{
	slice * pPiece = pPlayer->pPiece;
	ASSERT(pPiece);
	ASSERT(pPlayer->tag == w_tag && pPlayer->w_serial == w_index_in_man);
	pPlayer->SetWorld(0,0);

	pPiece->Lock();
	pPiece->RemovePlayer(pPlayer);
	pPiece->Unlock();
}
void 
world::RemoveNPC(gnpc *pNPC)
{
	slice * pPiece = pNPC->pPiece;
	ASSERT(pPiece);
	ASSERT(pNPC->tag == w_tag && pNPC->w_serial == w_index_in_man);
	pNPC->SetWorld(0,0);
	RemoveNPCFromMan(pNPC);

	pPiece->Lock();
	pPiece->RemoveNPC(pNPC);
	pPiece->Unlock();
}
void 
world::RemoveMatter(gmatter *pMatter)
{
	slice * pPiece = pMatter->pPiece;
	ASSERT(pPiece);
	ASSERT(pMatter->tag == w_tag && pMatter->w_serial == w_index_in_man);
	pMatter->SetWorld(0,0);
	RemoveMatterFromMan(pMatter);

	pPiece->Lock();
	pPiece->RemoveMatter(pMatter);
	pPiece->Unlock();

}

int 	
world::InsertPlayer(gplayer * pPlayer)
{
	slice *pPiece  = w_grid.Locate(pPlayer->pos.x,pPlayer->pos.z);
	if(pPiece == NULL) return -1;

	pPlayer->SetWorld(w_index,w_index_in_man);
	pPiece->Lock();
	pPiece->InsertPlayer(pPlayer);
	pPiece->Unlock();
	return pPiece - w_grid.pTable;
}

int 	
world::InsertMatter(gmatter * pMatter)
{
	slice *pPiece  = w_grid.Locate(pMatter->pos.x,pMatter->pos.z);
	if(pPiece == NULL) return -1;
	pMatter->SetWorld(w_index,w_index_in_man);

	pPiece->Lock();
	pPiece->InsertMatter(pMatter);
	pPiece->Unlock();
	gmatrix::InsertMatterToMan(pMatter);
	return pPiece - w_grid.pTable;
}


int 	
world::InsertNPC(gnpc * pNPC)
{
	slice *pPiece  = w_grid.Locate(pNPC->pos.x,pNPC->pos.z);
	if(pPiece == NULL) return -1;
	pNPC->SetWorld(w_index,w_index_in_man);
	
	pPiece->Lock();
	pPiece->InsertNPC(pNPC);
	pPiece->Unlock();
	gmatrix::InsertNPCToMan(pNPC);
	return pPiece - w_grid.pTable;
}


namespace
{
template <typename T>
inline gobject * GetList(slice * pPiece)
{
	ASSERT(false && "");
	return NULL;
}

template<>
inline gobject * GetList<gnpc>(slice *pPiece)
{
	return pPiece->npc_list;
}

template<>
inline gobject * GetList<gplayer>(slice *pPiece)
{
	return pPiece->player_list;
}

template<>
inline gobject * GetList<gmatter>(slice *pPiece)
{
	return pPiece->matter_list;
}

template <typename T, typename FUNC>
inline static void ForEachObject(slice * pPiece, const A3DVECTOR & pos, FUNC & func)
{
	gobject * pObj = GetList<T>(pPiece);
	while(pObj)
	{
		func(pObj,pos);
		pObj = pObj->pNext;
	}
}

template <typename T1>
class Foreach1
{
public:
	template <typename FUNC>
	inline static void ForEachInPiece(slice * pPiece, const A3DVECTOR &pos, FUNC & func)
	{
		pPiece->Lock();
		ForEachObject<T1>(pPiece,pos,func);
		pPiece->Unlock();
	};
};

template <typename T1,typename T2>
class Foreach2
{
public:
	template <typename FUNC>
	inline static void ForEachInPiece(slice * pPiece, const A3DVECTOR &pos, FUNC & func)
	{
		pPiece->Lock();
		ForEachObject<T1>(pPiece,pos,func);
		ForEachObject<T2>(pPiece,pos,func);
		pPiece->Unlock();
	};
};

template <typename T1,typename T2,typename T3>
class Foreach3
{
public:
	
	template <typename FUNC>
	inline static void ForEachInPiece(slice * pPiece, const A3DVECTOR &pos, FUNC & func)
	{
		pPiece->Lock();
		ForEachObject<T1>(pPiece,pos,func);
		ForEachObject<T2>(pPiece,pos,func);
		ForEachObject<T3>(pPiece,pos,func);
		pPiece->Unlock();
	};
};

template <typename FOREACH>
struct object_collector
{
	world * _plane;
	int _mask;
	abase::vector<gobject *,abase::fast_alloc<> > &_list;
	float _squared_radius;
	object_collector(world * plane,int mask,abase::vector<gobject *,abase::fast_alloc<> > &list,float radius):_plane(plane),_mask(mask),_list(list),_squared_radius(radius*radius){}

	inline void operator()(gobject * pObj, const A3DVECTOR & pos)
	{
		if((pObj->msg_mask & _mask) && pos.squared_distance(pObj->pos) < _squared_radius)
		{
			_list.push_back(pObj);
		}
	}
	
	inline void operator()(slice *pPiece,const A3DVECTOR & pos)
	{
		FOREACH::ForEachInPiece(pPiece,pos,*this);
	}
};

template <typename FOREACH>
struct object_sphere_collector
{
	world * _plane;
	int _self;
	abase::vector<gobject *,abase::fast_alloc<> > &_list;
	float _squared_radius;
	std::vector<exclude_target>& _targets_exclude;
	object_sphere_collector(world * plane,int self,abase::vector<gobject *,abase::fast_alloc<> > &list,float radius, std::vector<exclude_target>& targets_exclude)
		:_plane(plane),_self(self),_list(list),_squared_radius(radius*radius),_targets_exclude(targets_exclude){}
	
	inline void operator()(gobject * pObj, const A3DVECTOR & pos)
	{
		if(pObj->ID.id == _self) return;
		for(size_t i = 0; i < _targets_exclude.size(); i ++)
		{
			exclude_target tar = _targets_exclude[i];
			if(tar.type == 0 && pObj->ID.id == tar.id)
			{
				return;
			}
			if(tar.type == 1 && pObj->ID.type == GM_TYPE_NPC)
			{
				gnpc* npc = (gnpc*)pObj;
				if(npc && tar.id == npc->tid)
				{
					return;
				}
			}
		}
		if(pos.squared_distance(pObj->pos) < _squared_radius)
		{
			_list.push_back(pObj);
		}
	}

	inline void operator()(slice *pPiece,const A3DVECTOR & pos)
	{
		FOREACH::ForEachInPiece(pPiece,pos,*this);
	}
};

template <typename FOREACH>
struct object_column_collector
{
	world * _plane;
	int _self;
	abase::vector<gobject *,abase::fast_alloc<> > &_list;
	float _squared_radius;
	float _height;
	
	object_column_collector(world * plane,int self,abase::vector<gobject *,abase::fast_alloc<> > &list,float radius, float height = -1)
		:_plane(plane),_self(self),_list(list),_squared_radius(radius*radius), _height(height){}
	
	inline void operator()(gobject * pObj, const A3DVECTOR & pos)
	{
		if(pObj->ID.id == _self) return;
		if(pos.horizontal_distance(pObj->pos) < _squared_radius)
		{
			if(_height > 0 && pObj->pos.y > _height)
			{
				return;
			}
			_list.push_back(pObj);
		}
	}

	inline void operator()(slice *pPiece,const A3DVECTOR & pos)
	{
		FOREACH::ForEachInPiece(pPiece,pos,*this);
	}
};

template <typename FOREACH>
struct object_cylinder_collector
{
	world * _plane;
	int _self;
	abase::vector<gobject *,abase::fast_alloc<> > &_list;
	float _squared_radius;
	float _squared_range;
	float _inv_range;
	A3DVECTOR _offset;
	std::vector<exclude_target>& _targets_exclude;
	object_cylinder_collector(world * plane,int self, abase::vector<gobject *,abase::fast_alloc<> > &list,
			const A3DVECTOR &start, const A3DVECTOR &target,
			float radius, std::vector<exclude_target>& targets_exclude):_plane(plane),_self(self),_list(list),_squared_radius(radius*radius), _offset(target), _targets_exclude(targets_exclude)
	{
		_offset -= start;
		_squared_range = _offset.squared_magnitude();
		_inv_range = 1.f/_squared_range;
	}
	
	inline void operator()(gobject * pObj, const A3DVECTOR & pos)
	{
		if(pObj->ID.id == _self) return;
		for(size_t i = 0; i < _targets_exclude.size(); i ++)
		{
			exclude_target tar = _targets_exclude[i];
			if(tar.type == 0 && pObj->ID.id == tar.id)
			{
				return;
			}
			if(tar.type == 1 && pObj->ID.type == GM_TYPE_NPC)
			{
				gnpc* npc = (gnpc*)pObj;
				if(npc && tar.id == npc->tid)
				{
					return;
				}
			}
		}
		A3DVECTOR vec= pObj->pos;
		vec -= pos;
		float dp = vec.dot_product(_offset);
		if(dp > 0 && dp < _squared_range)
		{
			if(vec.squared_magnitude() - dp*dp*_inv_range <= _squared_radius)
			{
				_list.push_back(pObj);
			}
		}
	}

	inline void operator()(slice *pPiece,const A3DVECTOR & pos)
	{
		FOREACH::ForEachInPiece(pPiece,pos,*this);
	}
};

template <typename FOREACH>
struct object_taper_collector
{
	world * _plane;
	int _self;
	abase::vector<gobject *,abase::fast_alloc<> > &_list;
	float _squared_radius;
	A3DVECTOR _offset;
	float _trans_value;	// cos^2(1/2*angle) * distance^2 
	std::vector<exclude_target>& _targets_exclude;
	object_taper_collector(world * plane,int self, abase::vector<gobject *,abase::fast_alloc<> > &list,
			const A3DVECTOR &start, const A3DVECTOR &target, float radius,
			float cos_halfangle, std::vector<exclude_target>& targets_exclude):_plane(plane),_self(self),_list(list),_squared_radius(radius*radius),_offset(target),_trans_value(cos_halfangle*cos_halfangle),_targets_exclude(targets_exclude)
	{
		_offset -= start;
		_trans_value *= _offset.squared_magnitude();
	}
	
	inline void operator()(gobject * pObj, const A3DVECTOR & pos)
	{
		if(pObj->ID.id == _self) return;
		for(size_t i = 0; i < _targets_exclude.size(); i ++)
		{
			exclude_target tar = _targets_exclude[i];
			if(tar.type == 0 && pObj->ID.id == tar.id)
			{
				return;
			}
			if(tar.type == 1 && pObj->ID.type == GM_TYPE_NPC)
			{
				gnpc* npc = (gnpc*)pObj;
				if(npc && tar.id == npc->tid)
				{
					return;
				}
			}
		}
		A3DVECTOR vec = pObj->pos;
		vec -= pos;
		float dis = vec.squared_magnitude(); 
		if(dis <= _squared_radius)
		{
			float dp = vec.dot_product(_offset);
			if(dp >=0  && dp * dp > _trans_value * dis)
			{
				//cos * cos == (cos^angle * distance^2 * new_distance^2
				_list.push_back(pObj);
			}
		}
	}

	inline void operator()(slice *pPiece,const A3DVECTOR & pos)
	{
		FOREACH::ForEachInPiece(pPiece,pos,*this);
	}
};

template <typename FOREACH>
struct object_box_collector
{
	world * _plane;
	abase::vector<gobject *,abase::fast_alloc<> > &_list;
	rect _rt;
	object_box_collector(world * plane,abase::vector<gobject *,abase::fast_alloc<> > &list,const rect &rt)
		:_plane(plane),_list(list),_rt(rt){}
	
	inline void operator()(gobject * pObj, const A3DVECTOR & pos)
	{
		if(_rt.IsIn(pos.x,pos.z))
		{
			_list.push_back(pObj);
		}
	}

	inline void operator()(slice *pPiece,const A3DVECTOR & pos)
	{
		FOREACH::ForEachInPiece(pPiece,pos,*this);
	}
};

}

int 
world::BroadcastLocalMessage(const MSG & msg,float fRadius,int mask)
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	switch(msg.target.type)
	{
		case GM_TYPE_PLAYER:
			{
				object_collector<Foreach1<gplayer> > worker(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker);
			}
			break;
		case GM_TYPE_NPC:
			{
				object_collector<Foreach1<gnpc> > worker(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker);
			}
			break;
		case GM_TYPE_MATTER:
			{
				object_collector<Foreach1<gmatter> > worker(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker);
			}
			break;
		case GM_TYPE_ACTIVE:
			{
				//这里可以想办法减少一些构造的负担
				object_collector<Foreach1<gplayer> > worker1(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker1);

				object_collector<Foreach1<gnpc> > worker2(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker2);
			}
			break;
		case -1:		//-1表示全部发送
			{
				object_collector<Foreach1<gplayer> > worker1(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker1);
				
				object_collector<Foreach1<gnpc> > worker2(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker2);
				
				object_collector<Foreach1<gmatter> > worker3(this,mask,list,fRadius);
				ForEachSlice(msg.pos,fRadius,worker3);
			}
			break;
		default:
			ASSERT(false);
			return 0;
	}
	if(!list.empty())
	{
		__PRINTF("broadcast count %d\n",list.size());
		gmatrix::SendMultiMessage(list,msg);
	}
	return 0;
}

int 
world::BroadcastMessage(const MSG & msg,float fRadius,int mask)
{
	return BroadcastLocalMessage(msg,fRadius,mask);
}


int
world::BroadcastSphereMessage(const MSG & msg,const A3DVECTOR & target, float fRadius, size_t mc, std::vector<exclude_target>& targets_exclude)
{
	return BroadcastLocalSphereMessage(msg,target,fRadius,mc,targets_exclude);
}

int 
world::BroadcastCylinderMessage(const MSG & msg,const A3DVECTOR & target, float fRadius, size_t mc, std::vector<exclude_target>& targets_exclude)
{
	return BroadcastLocalCylinderMessage(msg,target,fRadius,mc,targets_exclude);
}

int 
world::BroadcastTaperMessage(const MSG & msg,const A3DVECTOR& target,float fRadius,float cos_halfangle, size_t mc, std::vector<exclude_target>& targets_exclude)
{
	return BroadcastLocalTaperMessage(msg,target,fRadius,cos_halfangle,mc,targets_exclude);
}

int 
world::BroadcastLocalBoxMessage(const MSG & msg, const rect & rt)
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	switch(msg.target.type)
	{
		case GM_TYPE_PLAYER:
			{
				object_box_collector<Foreach1<gplayer> > worker(this,list,rt);
				ForEachSlice(msg.pos,rt,worker);
			}
			break;
		case GM_TYPE_NPC:
			{
				object_box_collector<Foreach1<gnpc> > worker(this,list,rt);
				ForEachSlice(msg.pos,rt,worker);
			}
			break;
		case GM_TYPE_MATTER:
			{
				object_box_collector<Foreach1<gmatter> > worker(this,list,rt);
				ForEachSlice(msg.pos,rt,worker);
			}
			break;
		case GM_TYPE_ACTIVE:
			{
				object_box_collector<Foreach2<gplayer,gnpc> > worker(this,list,rt);
				ForEachSlice(msg.pos,rt,worker);
			}
			break;
		case -1:		//-1表示全部发送
			{
				object_box_collector<Foreach3<gplayer,gnpc,gmatter> > worker(this,list,rt);
				ForEachSlice(msg.pos,rt,worker);
			}
			break;
		default:
			ASSERT(false);
			return 0;
	}
	if(!list.empty())
	{
		__PRINTF("broadcast count %d\n",list.size());
		gmatrix::SendMultiMessage(list,msg);
	}
	return 0;
}

int 
world::BroadcastLocalSphereMessage(const MSG & msg,const A3DVECTOR & target, float fRadius,size_t mc, std::vector<exclude_target>& targets_exclude)
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	switch(msg.target.type)
	{
		case GM_TYPE_PLAYER:
			{
				object_sphere_collector<Foreach1<gplayer> > worker(this,msg.source.id,list,fRadius, targets_exclude);
				ForEachSlice(target,fRadius,worker);
			}
			break;
		case GM_TYPE_NPC:
			{
				object_sphere_collector<Foreach1<gnpc> > worker(this,msg.source.id,list,fRadius, targets_exclude);
				ForEachSlice(target,fRadius,worker);
			}
			break;
		case GM_TYPE_MATTER:
			{
				object_sphere_collector<Foreach1<gmatter> > worker(this,msg.source.id,list,fRadius,targets_exclude);
				ForEachSlice(target,fRadius,worker);
			}
			break;
		case GM_TYPE_ACTIVE:
			{
				object_sphere_collector<Foreach2<gplayer,gnpc> > worker(this,msg.source.id,list,fRadius,targets_exclude);
				ForEachSlice(target,fRadius,worker);
			}
			break;
		case -1:		//-1表示全部发送
			{
				object_sphere_collector<Foreach3<gnpc,gplayer,gmatter> > worker(this,msg.source.id,list,fRadius,targets_exclude);
				ForEachSlice(target,fRadius,worker);
			}
			break;
		default:
			ASSERT(false);
			return 0;
	}
	if(!list.empty())
	{
		__PRINTF("broadcast count %d\n",list.size());
		gmatrix::SendMultiMessage(list,msg,mc);
	}
	return 0;
}
int 
world::BroadcastLocalCylinderMessage(const MSG & msg,const A3DVECTOR & target, float fRadius,size_t mc,std::vector<exclude_target>& targets_exclude)
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	switch(msg.target.type)
	{
		case GM_TYPE_PLAYER:
			{
				object_cylinder_collector<Foreach1<gplayer> > worker(this,msg.source.id,list,msg.pos,target,fRadius,targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case GM_TYPE_NPC:
			{
				object_cylinder_collector<Foreach1<gnpc> > worker(this,msg.source.id,list,msg.pos,target,fRadius,targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case GM_TYPE_MATTER:
			{
				object_cylinder_collector<Foreach1<gmatter> > worker(this,msg.source.id,list,msg.pos,target,fRadius,targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case GM_TYPE_ACTIVE:
			{
				object_cylinder_collector<Foreach2<gplayer,gnpc> > worker(this,msg.source.id,list,msg.pos,target,fRadius,targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case -1:		//-1表示全部发送
			{
				object_cylinder_collector<Foreach3<gnpc, gplayer,gmatter> > worker(this,msg.source.id,list,msg.pos,target,fRadius,targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		default:
			ASSERT(false);
			return 0;
	}
	if(!list.empty())
	{
		__PRINTF("broadcast count %d\n",list.size());
		gmatrix::SendMultiMessage(list,msg,mc);
	}
	return 0;
}

int 
world::BroadcastLocalTaperMessage(const MSG & msg,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t mc, std::vector<exclude_target>& targets_exclude)
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	switch(msg.target.type)
	{
		case GM_TYPE_PLAYER:
			{
				object_taper_collector<Foreach1<gplayer> > worker(this,msg.source.id,list,msg.pos,target,fRadius,cos_halfangle, targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case GM_TYPE_NPC:
			{
				object_taper_collector<Foreach1<gnpc> > worker(this,msg.source.id,list,msg.pos,target,fRadius,cos_halfangle, targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case GM_TYPE_MATTER:
			{
				object_taper_collector<Foreach1<gmatter> > worker(this,msg.source.id,list,msg.pos,target,fRadius,cos_halfangle, targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case GM_TYPE_ACTIVE:
			{
				object_taper_collector<Foreach2<gplayer,gnpc> > worker(this,msg.source.id,list,msg.pos,target,fRadius,cos_halfangle, targets_exclude);
				ForEachSlice(msg.pos,target,worker);
			}
			break;
		case -1:		//-1表示全部发送
			{
				object_taper_collector<Foreach3<gnpc, gplayer,gmatter> > worker2(this,msg.source.id,list,msg.pos,target,fRadius,cos_halfangle, targets_exclude);
				ForEachSlice(msg.pos,target,worker2);
				
			}
			break;
		default:
			ASSERT(false);
			return 0;
	}
	if(!list.empty())
	{
		__PRINTF("broadcast count %d\n",list.size());
		if(mc > 0)
		{
			gmatrix::SendMultiMessage(list,msg,mc);
		} else
		{
			gmatrix::SendMultiMessage(list,msg);
		}
	}
	return 0;
}


namespace
{
void MakeObjectInfo(gactive_object * pObj, world::object_info & info)
{
	if(pObj->IsZombie())
	{
		info.state = world::QUERY_OBJECT_STATE_ZOMBIE;
	}
	else
	{
		info.state = world::QUERY_OBJECT_STATE_ACTIVE;
	}
	if(pObj->b_disconnect)
	{
		info.state |= world::QUERY_OBJECT_STATE_DISCONNECT;
	}
	info.pos = pObj->pos;
	info.body_size = pObj->body_size;
	info.cls = pObj->base_info.cls;
	info.gender = pObj->base_info.gender;
	info.faction = pObj->base_info.faction;
	info.hp = pObj->base_info.hp;
	info.mp = pObj->base_info.mp;
	info.level = pObj->base_info.level;
	info.tag = pObj->tag;
	info.tid = 0;
	info.invisible_rate = pObj->invisible_rate;
	info.anti_invisible_rate = pObj->anti_invisible_rate;
}

void MakeObjectInfo(gnpc * pNPC, world::object_info & info)
{
	MakeObjectInfo((gactive_object *)pNPC, info);
	info.tid = pNPC->tid;
}

void MakeObjectInfo(gmatter * pObj, world::object_info & info)
{
	info.state = world::QUERY_OBJECT_STATE_ACTIVE;
	info.pos = pObj->pos;
	info.body_size = pObj->body_size;
	info.cls = pObj->matter_type & 0xFFFF;
	info.gender = (pObj->matter_type >> 16) & 0xFFFF;
	info.faction = 0;
	info.hp = 0;
	info.tag = pObj->tag;
	info.tid = pObj->matter_type;
        info.invisible_rate = 0;
        info.anti_invisible_rate = 0;
}
}


bool 
world::QueryObject(const XID & id,object_info & info, bool other_plane)
{	
	//对一个对象进行查询
	//查询策略是：
	//1.首先在本地寻找,
	//2.如果本地没有找到的话,在本地外部列表和全局列表中寻找
	//3.如果仍然没有找到,那么对该对象可能所在的服务器发出搜寻请求(仅限于NPC和物品)
	//由于绝大部分查询都应该在本地找到,因此要着重考虑本地的优化
	switch(id.type)
	{
		case GM_TYPE_NPC:
		{
			size_t index = ID2IDX(id.id);
			if(index >= gmatrix::GetMaxNPCCount()) return false;
			//是本地原生的,一定有记录
			gnpc * pNPC = GetNPCByIndex(index);
			if(!other_plane && pNPC->tag != w_tag) return false;
			if(pNPC->IsActived() && pNPC->w_serial == w_index_in_man)
			{
				MakeObjectInfo(pNPC,info);
				return true;
			}
			return false;
		}
		return false;
		break;
		case GM_TYPE_PLAYER:
		{
			int index = FindPlayerIndex(id.id);
			if(index >=0)
			{
				gplayer * pPlayer = GetPlayerByIndex(index);
				if(!other_plane && pPlayer->tag != w_tag) return false;
				if(pPlayer->IsActived() && pPlayer->w_serial == w_index_in_man)
				{
					MakeObjectInfo(pPlayer,info);
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		}
		return false;
		break;

		case GM_TYPE_MATTER:
		{
			size_t index = ID2IDX(id.id);
			if(index >= gmatrix::GetMaxMatterCount()) return false;
			//是本地原生的,一定在本地
			gmatter * pMatter= GetMatterByIndex(index);
			if(!other_plane && pMatter->tag != w_tag) return false;
			if(pMatter->IsActived() && pMatter->w_serial == w_index_in_man)
			{
				MakeObjectInfo(pMatter,info);
				return true;
			}
			return false;
		}
		return false;
		default:
		return false;
	}

}

void world::GetColumnPlayers(std::vector<XID>& player_list, const A3DVECTOR& src, const A3DVECTOR& target, int self, float fRadius, float height)
{
	player_list.clear();
	abase::vector<gobject*,abase::fast_alloc<> > list;
	object_column_collector<Foreach1<gplayer> > worker(this,self,list,fRadius,height);
	ForEachSlice(src,target,worker);

	if(!list.empty())
	{
		__PRINTF("broadcast count %d\n",list.size());
		abase::vector<gobject*, abase::fast_alloc<> >::iterator it = list.begin();
		for(; it != list.end(); ++ it)
		{
			player_list.push_back((*it)->ID);
		}
	}
}

void 	
world::DuplicateWorld(world * dest) const
{
	dest->Init(w_index, w_tag);
	dest->InitManager(w_world_man);
	
	dest->w_grid = w_grid;
	dest->w_off_list	= w_off_list;
	dest->w_near_vision	= w_near_vision;
	dest->w_far_vision	= w_far_vision;
	dest->w_true_vision	= w_true_vision;
	dest->w_vision		= w_vision;
	if(w_ctrl) dest->w_ctrl	= w_ctrl->Clone();
}

void world::DumpMessageCount()
{
	for(size_t i =0;i < GM_MSG_MAX;i ++)
	{
		size_t count = w_message_counter[i];
		if(count > 0)
		{
			__PRINTF("MESSAGE:%4d\tcounter:%8d\n",i,count);
		}
	}
}

int
world::GetRegionPlayerCount(int id, A3DVECTOR & target, float & fRadius, std::vector<exclude_target>& targets_exclude)
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	object_sphere_collector<Foreach1<gplayer> > worker(this,id,list,fRadius,targets_exclude);
	ForEachSlice(target,fRadius,worker);

	return list.size();
}

int
world::GetRegionPlayer(abase::vector<gobject*,abase::fast_alloc<> > &list, int id, const A3DVECTOR & target, float & fRadius, std::vector<exclude_target>& targets_exclude)
{
	object_sphere_collector<Foreach1<gplayer> > worker(this,id,list,fRadius, targets_exclude);
	ForEachSlice(target,fRadius,worker);

	return list.size();
}


void 
world::BattleFactionSay(int faction, const void * msg, size_t size)
{
	w_world_man->BattleFactionSay( faction, msg, size );
}

void 
world::BattleSay(const void * msg, size_t size)
{
	w_world_man->BattleSay( msg, size );
}

int 
world::HandlePlaneMessage(const MSG & msg)
{
	switch(msg.message)
	{
		/*
		case GM_MSG_PRODUCE_MONEY:
			return 0;
			//不再掉钱了
			{
				ASSERT(msg.content_length == sizeof(msg_gen_money));
				msg_gen_money * mgi = (msg_gen_money*)msg.content;
				A3DVECTOR pos(msg.pos);
				pos.x += abase::RandUniform() * 4.f - 2.f;
				pos.z += abase::RandUniform() * 4.f - 2.f;
				const grid * pGrid = &GetGrid();
				if(!pGrid->IsLocal(pos.x,pos.z))
				{
					pos.x = msg.pos.x;
					pos.z = msg.pos.z;
				}
				float h = GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
				if(pos.y < h) pos.y = h;
				DropMoneyItem(this,pos,msg.param,msg.source,mgi->team_id,mgi->team_seq);
			}
			return 0;
			*/

		case GM_MSG_PRODUCE_MONSTER_DROP:
			{
				ASSERT(msg.content_length > 3 * sizeof(int));
				A3DVECTOR pos(msg.pos);
				const int * list = (const int*)msg.content;
				ASSERT(list[3]*sizeof(int) + sizeof(int)*4 == msg.content_length);
				// list[0] team_id
				// list[1] team_seq
				// list[2] npc_id
				// list[3] count
				// msg.param name_id
				// msg.param2 battle_faction 
				
				struct 
				{
					char type;
					char size;
					short name[10];
				}tag;
				tag.type = element_data::IMT_DROP;
				tag.size = 0;
				const grid * pGrid = &GetGrid();
				int count = list[3];
				float offset1 = 2.f;
				float offset2 = 4.f;
				if(count > 20) 
				{ 
					if(count > 300)
					{
						offset1 = 10.f;
					} 
					else if(count > 250)
					{
						offset1 = 8.f;
					}
					else if(count > 200)
					{
						offset1 = 7.f;
					}
					else if(count > 160)
					{
						offset1 = 6.f;
					}
					else if(count > 120)
					{
						offset1 = 5.f;
					}
					else if(count > 80)
					{
						offset1 = 4.f;
					}
					else if(count > 40)
					{
						offset1 = 3.5;
					}
					else
					{
						offset1 = 3.f;
					}

					offset2 = offset1 * 2.0f;
				}
				

				player_template::count_drop_item cdi;
				for(int i = 0; i <count; i ++)
				{
					int id = list[4+i];
					memset(&cdi, 0, sizeof(cdi));
					bool rst = player_template::GetCountDrop(id, cdi);

					if(rst)
					{
						class CheckCountDrop : public GDB::CountDropResult, public abase::ASmallObject
						{
						private:
							XID source;
							int team_id;
							int team_seq;
							int name_id;
							int battle_faction;
							int item_id;
							A3DVECTOR pos;
							world* plane;
							float offset1;
							float offset2;

						public:
							CheckCountDrop(const XID& _source, int _team_id, int _team_seq, int _name_id, int _battle_faction, int _item_id, const A3DVECTOR& _pos, world* _plane, float _offset1, float _offset2)
								: source(_source), team_id(_team_id), team_seq(_team_seq), name_id(_name_id), battle_faction(_battle_faction), item_id(_item_id), pos(_pos), plane(_plane), offset1(_offset1), offset2(_offset2)
							{
							}

							virtual void OnCountDropResult(int ret)
							{
								if(ret != 0)
								{
									__PRINTF("Failed to count drop, item_id=%d\n", item_id);
									return;
								}
								struct 
								{
									char type;
									char size;
									short name[10];
								}tag;
								tag.type = element_data::IMT_DROP;
								tag.size = 0;
								const grid * pGrid = &plane->GetGrid();
								item_data * data = gmatrix::GetDataMan().generate_item(item_id,&tag,tag.size + 2,name_id);
								if(data)
								{
									int n = 0;
									A3DVECTOR cPos;
									do{
										cPos.x = pos.x + abase::RandUniform() * offset2 - offset1;
										cPos.z = pos.z + abase::RandUniform() * offset2 - offset1;
										if(!pGrid->IsLocal(cPos.x,cPos.z))
										{
											cPos.x = pos.x;
											cPos.z = pos.z;
										}
										if(path_finding::GetValidPos(plane->GetWorldManager()->GetMoveMap(), cPos))
										{
											break;
										}
										cPos.y = plane->GetWorldManager()->GetTerrain().GetHeightAt(cPos.x,cPos.z);
										n ++;
									}while(n < 5);
									DropItemData(plane,cPos,data,source,team_id,team_seq, name_id, battle_faction);
									GNET::SendGlobalDropBoardcast(source.id, item_id, 0);
								}       
								delete this;
							}
						};
						GDB::put_counter_drop(cdi.template_id, cdi.start_time, cdi.time_of_duration, cdi.item_num, new CheckCountDrop(msg.source, list[0], list[1], msg.param, msg.param2, id, pos, this, offset1, offset2));
						continue;
					}
						
					item_data * data = gmatrix::GetDataMan().generate_item(list[4+i],&tag,tag.size + 2, msg.param);
					if(data)
					{
						int n = 0;
						do{
							pos.x = msg.pos.x + abase::RandUniform() * offset2 - offset1;
							pos.z = msg.pos.z + abase::RandUniform() * offset2 - offset1;
							if(!pGrid->IsLocal(pos.x,pos.z))
							{
								pos.x = msg.pos.x;
								pos.z = msg.pos.z;
							}
							if(path_finding::GetValidPos(GetWorldManager()->GetMoveMap(), pos))
							{
								break;
							}
							pos.y = GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
							n ++;
						}while(n < 5);
						DropItemData(this,pos,data,msg.source,list[0],list[1], msg.param, msg.param2);
					}       
				}       
			}       
			return 0;


		default:
			__PRINTF("未处理的位面消息%d\n",msg.message);
	}
	return 0;
}

int 
world::DispatchPlaneMessage(const MSG & msg)
{
	switch(msg.target.type)
	{
		case GM_TYPE_PLANE:
			return HandlePlaneMessage(msg);
		default:
			ASSERT(false && "受到无法处理的位面消息类型" );
			break;
	}
	return 0;
}

void
world::ResetWorld()
{
	if(w_ctrl) w_ctrl->Reset();
	//现在Reset只做很少的事情 
	//现在发现这样是不对的，应该让所有的NPC回到正常的状态并且ClearSession才是
	//而现在直接将所有世界中存在的对象释放
	//这个世界的心跳将会停止，所以再也不会有重生和新怪物出现了
	int count = w_grid.reg_row * w_grid.reg_column;
	abase::vector<gnpc *, abase::fast_alloc<> > list1;
	abase::vector<gmatter *, abase::fast_alloc<> > list2;
	list1.reserve(1000);
	list2.reserve(1000);

	//锁住所有的slice先 先小后大， 所以不会有死锁
	for(int i = 0; i < count; i ++)
	{
		w_grid.pTable[i].Lock();
	}

	//收集所有的NPC和matter，用完就解开锁
	for(int i = 0; i < count; i ++)
	{
		slice & piece = w_grid.pTable[i];
		gnpc * pNPC = (gnpc*)piece.npc_list;
		while(pNPC)
		{
			list1.push_back(pNPC);
			pNPC = (gnpc*)pNPC->pNext;
		}

		gmatter * pMatter = (gmatter*)w_grid.pTable[i].matter_list;
		while(pMatter)
		{
			list2.push_back(pMatter);
			pMatter = (gmatter*)pMatter->pNext;
		}
		piece.Unlock();
	}

	//释放收集到的NPC
	for(size_t i =0; i < list1.size(); i ++)
	{
		gnpc * pNPC = list1[i];
		//理论上存在这时正好消失和死亡的npc
		pNPC->Lock();
		if(pNPC->imp && pNPC->IsActived())
		{
			pNPC->imp->_commander->Release();
		}
		else
		{
			//什么也不做
			//这个NPC应该已经释放了
			//或者被管理器管理了
		}
		pNPC->Unlock();
	}

	//释放收集到的Matter
	for(size_t i =0; i < list2.size(); i ++)
	{
		gmatter * pMatter = list2[i];
		//理论上存在这时正好消失和死亡的matter
		pMatter->Lock();
		if(pMatter->imp && pMatter->IsActived())
		{
			pMatter->imp->_commander->Release();
		}
		else
		{
			//什么也不做
			//这个NPC应该已经释放了
			//或者被管理器管理了
		}
		pMatter->Unlock();
	}
}

void world::Release()
{	
	w_npc_gen.Release();
	ResetWorld();
	//将管理器中的对象(待复生的和其它的)和内容释放
	w_world_man = NULL;
	if(w_ctrl)
	{
		delete w_ctrl;
		w_ctrl = NULL;
	}
	w_player_map.clear();
	w_grid.Release();

	w_battle_result = 0;
	w_pmap_lock = 0;
	w_nmap_lock = 0;
	w_index = 0;
	_message_handle_count = 0;
	memset(w_message_counter,0,sizeof(w_message_counter));
	w_player_count = 0;
	w_obsolete = 0;
	w_activestate = 0;
	w_index_in_man = -1;
	w_create_timestamp = -1;
	w_ins_kick = 1;
	w_battle_result = 0;
	w_offense_goal	= 0;
	w_offense_cur_score = 0;
	w_defence_goal = 0;
	w_defence_cur_score = 0;
	w_end_timestamp = 0;
	w_near_vision  = 0;
	w_true_vision = 0;

}

