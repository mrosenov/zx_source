#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <threadpool.h>
#include <conf.h>
#include <io/pollio.h>
#include <io/passiveio.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <meminfo.h>
#include <strtok.h>
#include <glog.h>
#include <verbose.h>

#include "../template/itemdataman.h"
#include "../template/npcgendata.h"
#include "../world.h"
#include "../player_imp.h"
#include "../npc.h"
#include "../matter.h"
#include "../playertemplate.h"
#include "instance_config.h"
#include "instance_manager.h"
#include "../pathfinding/pathfinding.h"
#include "../template/globaldataman.h"


static void str2rect(rect & rt,const char * str)
{
	sscanf(str,"{%f,%f} , {%f,%f}",&rt.left,&rt.top,&rt.right,&rt.bottom);
}

world * 
instance_world_manager::CreateWorldTemplate()
{
	world * pPlane  = new world;
	pPlane->Init(_world_index, _world_tag);
	pPlane->InitManager(this);
	return pPlane;
}

world_message_handler * 
instance_world_manager::CreateMessageHandler()
{
	return new instance_world_message_handler(this);
}

int
instance_world_manager::Init(const char * gmconf_file,const char *  servername)
{
	_message_handler = CreateMessageHandler();
	//_manager_instance = this;


	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_BLOCK, &set, NULL);

	/*
	 *      开始初始化世界
	 */
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "Instance_";
	section += servername;

	PreInit(servername);

	//取得可能存在的存盘点
	std::string str = conf->find(section,"save_point");
	{
		int tag = -1;
		A3DVECTOR pos;
		sscanf(str.c_str(),"%d,%f,%f,%f",&tag,&pos.x,&pos.y,&pos.z);
		if(tag <=0) tag = -1;
		_save_point.tag = tag;
		_save_point.pos = pos;
	}

	//得到根目录
	std::string root = conf->find("Template","Root");

	//得到基础目录
	std::string base_path;
	base_path = root + conf->find(section,"base_path");
	printf("资源根目录:'%s'\n", base_path.c_str());
	
	//得到重新启动的操作
	_restart_shell  = base_path + conf->find("Template","RestartShell");


	//得到副本需求的cid
	if(!_cid.Init(conf->find(section,"cid").c_str()))
	{
		printf("错误的classid 在 'cid'项\n");
		return -1008;
	}

	_player_limit_per_instance = atoi(conf->find(section,"player_per_instance").c_str());
	if(_player_limit_per_instance <= 0) 
	{
		printf("每副本中容纳玩家数必须高于0\n");
		return -1010;
	}

	
	size_t instance_count = atoi(conf->find(section,"instance_capacity").c_str());
	if(instance_count > MAX_INSTANCE_PER_SVR) instance_count = MAX_INSTANCE_PER_SVR;
	_planes_capacity = instance_count;

	_pool_threshold_low  =  atoi(conf->find(section,"pool_threshold_low").c_str());
	_pool_threshold_high =  atoi(conf->find(section,"pool_threshold_high").c_str());
	int pool_threadhold_init = atoi(conf->find(section,"pool_threshold_init").c_str());

	if(_pool_threshold_low <= 0 ||  _pool_threshold_high <=0
			|| _pool_threshold_low > _pool_threshold_high)
	{
		printf("世界缓冲池阈值错误\n");
		return -1011;
	}

	if(pool_threadhold_init > _pool_threshold_high) pool_threadhold_init = _pool_threshold_high;
	if(pool_threadhold_init <= 0) pool_threadhold_init = (_pool_threshold_low + _pool_threshold_high)/2;
	

	_idle_time = atoi(conf->find(section,"idle_time").c_str());
	if(_idle_time < 30) _idle_time = 20*60;

	//初始化所有的管理器
	//world_manager::Init();
	if(int irst = world_manager::InitBase(section.c_str()))
	{
		//初始化基础出错
		return irst;
	}
	
	printf("idle_time %d\n",_idle_time);
	glb_verbose = 200;
	time_t ct = time(NULL);
	printf("%s\n",ctime(&ct));

	//初始创建一个世界 
	_plane_template = CreateWorldTemplate();
	world & plane = *_plane_template;

	/*
	 *      初始化网格数据
	 *
	 */
	str = conf->find(section,"grid");
	int row=800,column=800;
	float xstart=0.f,ystart=0.f,step=12.5f;
	sscanf(str.c_str(),"{%d,%d,%f,%f,%f}",&column,&row,&step,&xstart,&ystart);

	if(!plane.CreateGrid(row,column,step,xstart,ystart)){
		printf("Can not create world grid!\n");
		return -1;
	}

	//这里统计一下内存占用，如果超过了必要值，则不能进行
	unsigned long long mem_need = row*column*sizeof(slice);

	mem_need *= _pool_threshold_high;
	//mem_need += _player_max_count * sizeof(gplayer);
	//mem_need += _npc_max_count * sizeof(gnpc);
	//mem_need += _matter_max_count * sizeof(gmatter);
	mem_need += instance_count * sizeof(world);

	unsigned long long mem_std = mem_need;
	//mem_std += _player_max_count * sizeof(gplayer_imp);
	//mem_std += (unsigned long long)(0.5*_npc_max_count *sizeof(gnpc_imp)); 
	//mem_std += _matter_max_count * sizeof(gmatter_imp);


	unsigned long long mem_max = mem_need;
	mem_max += (instance_count - _pool_threshold_high)*row*column*sizeof(slice);
	//mem_max += (unsigned long long)(0.5*_npc_max_count *sizeof(gnpc_imp)); 
	//mem_max += (_player_max_count + _npc_max_count)*1536;

	size_t mem_now = GetMemTotal();
	float mem_ratio1 = ((mem_need/1024.f)/mem_now)*100.f;
	float mem_ratio2 = ((mem_std/1024.f)/mem_now)*100.f;
	float mem_ratio3 = ((mem_max/1024.f)/mem_now)*100.f;
	printf("总计%d个副本空间\n",instance_count);
	printf("预计内存需求:\n");
	printf("空状态  \t %dkB(%.2f%%)\n",(int)(mem_need/1024), mem_ratio1);
	printf("标准状态\t %dKb(%.2f%%)\n",(int)(mem_std/1024), mem_ratio2);
	printf("最大极限\t %dKb(%.2f%%)\n",(int)(mem_max/1024), mem_ratio3);
	
	
	if(mem_now * 0.6 < mem_std/1024)
	{
		printf("标准状态需求的内存超出了现有物理内存的阈值(need:%dmB/threshold:%dmB/total:%dmB)\n",(int)(mem_std/(1024*1024)), (int)(mem_now*0.6/1024), mem_now/1024);
		return -1001;
	}

	/**
	 *      进行区域的简单检查
	 */
	rect local_rt;
	str2rect(local_rt,conf->find(section,"local_region").c_str());
	if(!plane.GetGrid().SetRegion(local_rt,1.f))
	{
		printf("配置文件中的区域数据不正确(local_region)\n");
		return -2;
	}

	plane.GetGrid().inner_region = local_rt;
	plane.BuildSliceMask(15.f,_max_visible_range);

	rect rt = plane.GetGrid().grid_region;
	__PRINTF("Create grid: %d*%d step: %f sight range:%f slice in sight:%d\n",
			row,column,step, _max_visible_range,plane.w_far_vision + 1);
	printf("Grid Region: {%.2f,%.2f} - {%.2f,%.2f}\n",rt.left,rt.top,rt.right,rt.bottom);
	

	rt = plane.GetGrid().local_region;
	printf("Local Region: {%.2f,%.2f} - {%.2f,%.2f}\n",rt.left,rt.top,rt.right,rt.bottom);
	if(rt.left > rt.right - 100.f || rt.top > rt.bottom - 100.f)
	{
		printf("内部区域过小\n");
		return -5;
	}

	/*
	 *       读取地形数据
	 */
	str = "Terrain_";
	str += servername;
	if(!InitTerrain(base_path, str, plane.GetLocalWorld()))
	{
		return -2;
	}

	
	/*
	 *  读入npc通路图
	 *
	 */
	std::string str1 = base_path + conf->find("MoveMap","Path");
	std::string str2 = base_path + conf->find("MoveMap","WaterPath");
	std::string str3 = base_path + conf->find("MoveMap","AirPath");
	if(!InitMoveMap(str1.c_str(),str2.c_str(),str3.c_str()))
	{
		printf("无法读入NPC通路图的配置文件\n");
		return -123;
	}

	
	std::string  regionfile= base_path + conf->find("Template","RegionFile");
	std::string  regionfile2= base_path + conf->find("Template","RegionFile2");

	//装载城市区域
	if(!InitRegionData(regionfile.c_str(),regionfile2.c_str()))
	{
		printf("can not load city region data from file '%s'\n",regionfile.c_str());
		return -7;
	}

	_region.GetRegionTime(_region_file_tag,_precinct_file_tag);

	//读取路线文件
	std::string pathfile = base_path + conf->find("Template","PathFile");
	if(!_pathman.Init(pathfile.c_str()))
	{
		printf("无法打开路线文件\n");
		return -9;
	}

	printf("开始创建副本世界\n");

	//初始化所有副本的NPC排布
	std::string  npcgenfile = base_path + conf->find("Template","NPCGenFile");
	_npcgen = new CNPCGenMan;
	if(!_npcgen->Load(npcgenfile.c_str()))
	{
		printf("无法打开npc分布文件\n");
		return -8;
	}
	printf("应用%d个怪物区域到%d个位面中\n", _npcgen->GetGenAreaNum(),_planes_capacity);

	//生成激活世界列表
	_cur_planes.insert(_cur_planes.end(),instance_count,0);
	_planes_state.insert(_planes_state.end(),instance_count,0);


	//生成标准世界池
	for(int i = 0; i < pool_threadhold_init; i ++)
	{
		_planes_pool.push_back(new world);
		_plane_template->DuplicateWorld(_planes_pool[i]);
	}

	for(int i = 0; i < pool_threadhold_init; i ++)
	{
		_planes_pool[i]->w_plane_index = -1;
		_planes_pool[i]->InitNPCGenerator(*_npcgen);
	}

	_max_active_index = 0;

	/*
	 *      初始化游戏服务器之间的连接池
	 */
	if(!InitNetIO(servername))
	{
		return -7;
	}

	FinalInit(servername);
	return 0;
}

bool 
instance_world_manager::InitNetIO(const char * servername)
{
	//_ioman.SetPlane(this);
	//从第一个世界取得网格
	//grid & g = _plane_template->GetGrid();
//	return _ioman.Init(servername,g.local_region,g.inner_region);
	return true;
}

void 
instance_world_manager::RestartProcess()
{
	//考虑让所有人都断线 
	gplayer * pPool = gmatrix::GetPlayerPool();
	for(size_t i = 0; i < gmatrix::GetMaxPlayerCount(); i ++)
	{
		if(pPool[i].IsEmpty()) continue;
		if(!pPool[i].imp) continue;
		int cs_index = pPool[i].cs_index;
		if(cs_index <=0) continue;
		GMSV::SendDisconnect(cs_index,pPool[i].ID.id,pPool[i].cs_sid,0);
	}

	if(!fork())
	{
		for(int i =3;i < getdtablesize(); i ++)
		{
			close(i);
		}
		sleep(1);
		system(_restart_shell.c_str());
	}
}

size_t
instance_world_manager::GetWorldCapacity()
{
	return _planes_capacity;
}

int 
instance_world_manager::GetOnlineUserNumber()
{
	return gmatrix::GetPlayerAlloced();
}

world * 
instance_world_manager::GetWorldByIndex(size_t index)
{
	return _cur_planes[index];
}

void 
instance_world_manager::GetPlayerCid(player_cid & cid)
{
	cid = _cid;
}

void 
instance_world_manager::FreeWorld(world * pPlane, int index)
{
	mutex_spinlock(&_key_lock);
	if(_planes_state[index] == 0 || _cur_planes[index] != pPlane 
			|| pPlane->w_player_count || pPlane->w_obsolete == 0 ) 
	{
		mutex_spinunlock(&_key_lock);
		return;
	}
	pPlane->w_obsolete = 0;
	pPlane->w_index_in_man = -1;
	pPlane->w_activestate = 2;
	_key_map.erase(pPlane->w_ins_key);
	pPlane->w_create_timestamp = -1;	//清除创建的时间戳
	_cur_planes[index] = 0;
	_planes_state[index] = 0;
	mutex_spinunlock(&_key_lock);

	mutex_spinlock(&_pool_lock);
	_active_plane_count --; 
	mutex_spinunlock(&_pool_lock);

	//重置世界
	pPlane->ResetWorld();
	//将世界放入冷却列表
	_planes_cooldown.push_back(pPlane);
			
	__PRINTF("将世界%d放入冷却池中\n",index);
}
void 
instance_world_manager::RegroupCoolDownWorld(bool recycle_world)
{
	if(size_t count = _planes_cooldown2.size())
	{
		//试着将冷却完毕的世界放入到世界池中
		if(recycle_world)
		{
			mutex_spinlock(&_pool_lock);
			if(_planes_capacity > _active_plane_count &&
				(size_t)_planes_capacity > _planes_pool.size() + _active_plane_count &&
				_planes_pool.size() < (size_t)_pool_threshold_high)
			{
				for(size_t i =0;i<count && _planes_pool.size()<(size_t)_pool_threshold_high; i ++)
				{
					world * plane = _planes_cooldown2.back();
					plane->w_activestate = 0;
					_planes_cooldown2.pop_back();
					_planes_pool.push_back(plane);
					__PRINTF("世界%p被放回到世界池中\n",plane);
				}
			}
			mutex_spinunlock(&_pool_lock);
		}

		//将可能剩余的冷却完毕的世界进行释放
		for(size_t i = 0; i < _planes_cooldown2.size(); i ++)
		{
			__PRINTF("释放了世界%p\n",_planes_cooldown2[i]);
			_planes_cooldown2[i]->Release();
			delete _planes_cooldown2[i];
		}
		_planes_cooldown2.clear();
	}
	_planes_cooldown2.swap(_planes_cooldown);
}

void
instance_world_manager::FillWorldPool()
{
	mutex_spinlock(&_pool_lock);
	int pool_count = _planes_pool.size();
	int r1 = _pool_threshold_low - pool_count;
	int r2 =  _planes_capacity - (1 + pool_count +  (int)_planes_cooldown.size() + _active_plane_count);
	mutex_spinunlock(&_pool_lock);

	if(r1 > r2) r1 = r2;
	if(r1 > 0)
	{
		abase::vector<world*> list;
		list.reserve(r1);
		for(int i = 0; i < r1; i ++)
		{
			world * pPlane = new world;
			_plane_template->DuplicateWorld(pPlane);
			pPlane->w_plane_index = -1;
			pPlane->InitNPCGenerator(*_npcgen);
			list.push_back(pPlane);
			__PRINTF("创建了世界%p\n",pPlane);
		}

		//将新产生的世界放入到世界池中
		mutex_spinlock(&_pool_lock);
		for(int i = 0; i < r1; i ++)
		{
			_planes_pool.push_back(list[i]);
		}
		mutex_spinunlock(&_pool_lock);

	}

}

void
instance_world_manager::Heartbeat()
{
	mutex_spinlock(&_heartbeat_lock);
	
	world_manager::Heartbeat();
	size_t ins_count = _max_active_index;
	for(size_t i = 0; i < ins_count ; i ++)
	{
		if(_planes_state[i] == 0)
		{
			continue;
		}
		world * pPlane = _cur_planes[i];
		if(!pPlane) continue;
		pPlane->RunTick();
	}

	if((++_heartbeat_counter) > 200*3)
	{
		//每30秒检验一次
		for(size_t i = 0; i < ins_count ; i ++)
		{
			if(_planes_state[i] == 0) continue;	//空世界
			world * pPlane = _cur_planes[i];
			if(!pPlane) continue;
			if(pPlane->w_obsolete)
			{
				//处于等待废除状态
				if(pPlane->w_player_count)
				{
					pPlane->w_obsolete = 0;
				}
				else
				{
					if((pPlane->w_obsolete += 30) > _idle_time)  // 20 * 60
					{
						//没有玩家并且时间超时了，则释放世界
						FreeWorld(pPlane,i);
					}
				}
			}
			else
			{
				if(!pPlane->w_player_count)
				{
					pPlane->w_obsolete = 1;
				}
			}
			
		}
		_heartbeat_counter = 0;

		//进行冷却列表的处理
		RegroupCoolDownWorld();

		//如果世界池的容量不足，则进行重新创建处理
		FillWorldPool();


	}

	mutex_spinunlock(&_heartbeat_lock);
}

bool 
instance_world_manager::CompareInsKey(const instance_key & key, const instance_hash_key & hkey)
{
	instance_hash_key key2;
	TransformInstanceKey(key.essence,key2);
	return key2 == hkey;
}

int
instance_world_manager::CheckPlayerSwitchRequest(const XID & who, const instance_key * ikey,const A3DVECTOR & pos,int ins_timer)
{
	int rst = 0;
	//首先检查key是否存在
	instance_hash_key key;
	TransformInstanceKey(ikey->target,key);
	world *pPlane = NULL;
	mutex_spinlock(&_key_lock);
	int * pTmp = _key_map.nGet(key);
	if(!pTmp)
	{
		//世界不存在， 判断是否有空闲的world等待分配，如果没有则通知玩家
		bool bRst = _planes_pool.size() >= 1;
		mutex_spinunlock(&_key_lock);
		if(!bRst || (_active_plane_count + 1) >= _planes_capacity)
		{
			rst = S2C::ERR_TOO_MANY_INSTANCE;
		}
		else
		{
			if(ins_timer == 0)
			{
				rst = S2C::ERR_CAN_NOT_RESET_INSTANCE;
			}
		}
		return rst;
	}
	pPlane = _cur_planes[*pTmp];
	mutex_spinunlock(&_key_lock);
	//检查玩家的人数是否匹配
	if(pPlane)
	{
		return (pPlane->w_player_count < _player_limit_per_instance)?0:S2C::ERR_TOO_MANY_PLAYER_IN_INSTANCE;
	}
	else
	{
		return S2C::ERR_CANNOT_ENTER_INSTANCE;
	}
}

world * 
instance_world_manager::AllocWorld(const instance_hash_key & key, int & world_index)
{
	spin_autolock keeper(_key_lock);
	return AllocWorldWithoutLock(key,world_index);
}

world * 
instance_world_manager::AllocWorldWithoutLock(const instance_hash_key &key, int & world_index)
{
	//虽然是without lock 但是本阶段必须保持加锁
	ASSERT(_key_lock);
	//首先查询或者分配一个世界
	world *pPlane = NULL;
	int * pTmp = _key_map.nGet(key);
	world_index = -1;
	if(!pTmp)
	{
		if(!_planes_pool.size()) 
		{
			return NULL;
		}
		mutex_spinlock(&_pool_lock);
		if(_planes_pool.size())
		{
			pPlane = _planes_pool.back();
			_planes_pool.pop_back();
			_active_plane_count ++;
		}
		mutex_spinunlock(&_pool_lock);
		if(pPlane)
		{
			pPlane->w_destroy_timestamp = g_timer.get_systime() + 120;//防止根据destroy_time错误的释放

			//寻找可以放置世界的空位
			size_t i = 1;	
			for(; i < (size_t)_planes_capacity; i ++)
			{
				if(_cur_planes[i]) continue;
				_cur_planes[i] = pPlane;
				_planes_state[i]  = 1; 
				pPlane->w_index_in_man = i;
				pPlane->w_plane_index = i;
				pPlane->w_activestate = 1;
				world_index = i;
				break;
			}
			if(i != (size_t)_planes_capacity)
			{
				__PRINTF("分配了世界%d %p\n",i,pPlane);
				_key_map.put(key,i);
				pPlane->w_ins_key = key;
				pPlane->w_obsolete = 0;
				pPlane->w_create_timestamp = time(NULL);
				if((size_t) _max_active_index < i +1)
				{
					_max_active_index = i + 1;
				}
			}
			else
			{
			//	ASSERT(false);

				//无法加入世界，重新将生成的世界放入到世界池中
				mutex_spinlock(&_pool_lock);
				_planes_pool.push_back(pPlane);
				_active_plane_count --; 
				mutex_spinunlock(&_pool_lock);
				return NULL;
			}
		}
	}
	else
	{
		//存在这样的世界 
		world_index = *pTmp;;
		pPlane = _cur_planes[world_index];
		ASSERT(pPlane);
		pPlane->w_obsolete = 0;
	}
	if(world_index < 0) return NULL;
	return pPlane;
}

world * 
instance_world_manager::GetWorldInSwitch(const instance_hash_key & ikey,int & world_index)
{
	return AllocWorld(ikey,world_index);
	
}

void 
instance_world_manager::HandleSwitchStart(int uid, int source_tag, const A3DVECTOR & pos, const instance_key & key)
{
	ASSERT(false);
}

void 
instance_world_manager::HandleSwitchRequest(int link_id,int user_id,int localsid,int source,const instance_key &ikey)
{
	instance_hash_key key;
	TransformInstanceKey(ikey.target,key);
	int world_index;
	world * pPlane = GetWorldInSwitch(key,world_index);
	if(!pPlane ) 
	{
		//无法创建世界，副本数目达到最大上限 
		MSG msg;
		BuildMessage(msg,GM_MSG_ERROR_MESSAGE,XID(GM_TYPE_PLAYER,user_id),XID(0,0),A3DVECTOR(0,0,0),S2C::ERR_TOO_MANY_INSTANCE);
		gmatrix::SendMessage(msg);
		return;
	}

	ASSERT(pPlane == _cur_planes[world_index]);
	int index1 = gmatrix::FindPlayer(user_id);
	if(index1 >= 0) 
	{
		//这个用户不应该存在的(但是由于时许似乎也有可能)
		//如果link和game断开，那么这种情况可能会发生
		return;
	}
	gplayer * pPlayer = pPlane->AllocPlayer();
	if(pPlayer == NULL)
	{
		//发送没有物理空间可以容纳Player的信息
		__PRINTF("用户达到物理最大限制值\n");
		GLog::log(GLOG_ERR,"副本%d用户数目达到最大上限，当用户%d转移服务器时",GetWorldTag(),user_id);
		//不发送转移消息，这样玩家会因为超时而断线
		return;
	}

	__PRINTF("player %d switch from %d\n",user_id,source );
	pPlayer->cs_sid = localsid;
	pPlayer->cs_index = link_id;
	pPlayer->ID.id = user_id;
	pPlayer->ID.type = GM_TYPE_PLAYER;
	pPlayer->login_state = gplayer::WAITING_SWITCH;
	pPlayer->pPiece = NULL;
	if(!pPlane->MapPlayer(user_id,pPlane->GetPlayerIndex(pPlayer)))
	{	
		__PRINTF("服务器转移时map player失败\n");
		GLog::log(GLOG_ERR,"副本%d在执行服务器转移时map player失败，用户%d",GetWorldTag(),user_id);

		//可能是由于时序使得后来的消息先处理，从而进行的重复的创建 
		pPlane->FreePlayer(pPlayer);
		pPlayer->Unlock();
		return;
	}

	//将这个用户设置为在这个位面
	SetPlayerWorldIdx(user_id,world_index);
	
	class switch_task : public ONET::Thread::Runnable, public abase::timer_task , public abase::ASmallObject
	{
		gplayer *_player;
		int _userid;
		world * _plane;
		instance_world_manager * _manager;
		public:
			switch_task(gplayer * pPlayer,world * pPlane,instance_world_manager * manager)
				:_player(pPlayer),_userid(pPlayer->ID.id),_plane(pPlane),_manager(manager)
			{
				SetTimer(g_timer,10*5,1);
				__PRINTF("timer %p %d\n",this,_timer_index);
			}
		public:
			virtual void OnTimer(int index,int rtimes)
			{
				ONET::Thread::Pool::AddTask(this);
			}

			virtual void Run()
			{
				spin_autolock keeper(_player->spinlock);
				if(_player->IsActived() && _player->ID.id == _userid && _player->login_state == gplayer::WAITING_SWITCH)
				{
					_plane->UnmapPlayer(_userid);
					_plane->FreePlayer(_player);

					//同时取消在这个位面的记录  （由于时序，这里可能会有些问题）$$$$$$$$
					//要再考虑一下
					_manager->RemovePlayerWorldIdx(_userid);
				}
				delete this;
			}
	};
	//发出等待消息
	MSG msg;
	BuildMessage(msg,GM_MSG_SWITCH_GET,pPlayer->ID,XID(GM_TYPE_MANAGER,_world_tag),A3DVECTOR(0,0,0),_world_tag, &ikey,sizeof(ikey));
	//pPlane->SendRemoteMessage(source,msg); $$$$$ CM

	//设置超时
	switch_task *pTask = new switch_task(pPlayer,pPlane,this);
	pPlayer->base_info.cls = (short)(abase::timer_task*)pTask;
	pPlayer->base_info.faction = pTask->GetTimerIndex();
	pPlayer->Unlock();
	
}


void 
instance_world_manager::PlayerLeaveThisWorld(int plane_index, int userid)
{
	RemovePlayerWorldIdx(userid,plane_index);
}

void 
instance_world_manager::GetLogoutPos(gplayer_imp * pImp, int & world_tag ,A3DVECTOR & pos)
{
	if(_world_limit.savepoint && _save_point.tag > 0)
	{
		world_tag = _save_point.tag;
		pos = _save_point.pos;
	}
	else
	{
		pos = pImp->GetLogoutPos(world_tag);
	}
}

void instance_world_manager::SwitchServerCancel(int link_id,int user_id, int localsid)
{
	int index1;
	gplayer * pPlayer = gmatrix::FindPlayer(user_id,index1);
	if(!pPlayer)
	{
		ASSERT(false);
		//没有找到 合适的用户
		//正常情况下，这个用户应该存在的
		return;
	}
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != user_id || !pPlayer->IsActived() || !pPlayer->imp)
	{
		ASSERT(false);
		return;
	}
	pPlayer->imp->CancelSwitch();
}

void	instance_user_login(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size);
void 
instance_world_manager::UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size, char flag)
{
	instance_user_login(cs_index,cs_sid,uid,auth_data,auth_size);
}

bool instance_world_manager::IsUniqueWorld()
{
	return false;
}

void 
instance_world_manager::SetIncomingPlayerPos(gplayer * pPlayer, const A3DVECTOR & pos)
{
	pPlayer->pos = pos;

	//对pos的高度进行修正
	float height = GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pPlayer->pos.y < height) pPlayer->pos.y = height;
}


