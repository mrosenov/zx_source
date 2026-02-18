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
#include <glog.h>

#include "template/itemdataman.h"
#include "template/npcgendata.h"
#include "global_manager.h"
#include "player_imp.h"
#include "playertemplate.h"
#include "pathfinding/pathfinding.h"
#include "commondata_if.h"



static void str2rect(rect & rt,const char * str)
{
	sscanf(str,"{%f,%f} , {%f,%f}",&rt.left,&rt.top,&rt.right,&rt.bottom);
}

int
global_world_manager::Init(const char * gmconf_file,const char *  servername, int tag, int index)
{
	_message_handler = new global_world_message_handler(this,&_plane);


	/*
	 *      开始初始化世界
	 */
	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	section += servername;

	if(int irst = world_manager::InitBase(section.c_str()))
	{
		//初始化基础出错
		return irst;
	}

	if(tag >= 0 && index >= 0)
	{
		SetWorldTag(tag);
		SetWorldIndex(index);
	}

	//得到根目录
	std::string root = conf->find("Template","Root");

	//得到基础目录
	std::string base_path;
	base_path = root + conf->find(section,"base_path");
	printf("%s资源根目录:'%s'\n",servername, base_path.c_str());


	//得到世界采用的cid
	if(!_cid.Init(conf->find(section,"cid").c_str()))
	{
		__PRINTF("错误的classid 在 'cid'项\n");
		return -1008;
	}


	_plane.Init(GetWorldIndex(), GetWorldTag());
	_plane.InitManager(this);
	_plane.w_activestate = 1;	//大世界是始终激活的

	/*
	 *      初始化网格数据
	 *
	 */
	std::string str = conf->find(section,"grid");
	int row=800,column=800;
	float xstart=0.f,ystart=0.f,step=12.5f;
	sscanf(str.c_str(),"{%d,%d,%f,%f,%f}",&column,&row,&step,&xstart,&ystart);

	if(!_plane.CreateGrid(row,column,step,xstart,ystart)){
		__PRINTF("Can not create world!\n");
		return -1;
	}

	
	rect local_rt;
	str2rect(local_rt,conf->find(section,"local_region").c_str());
	if(!_plane.GetGrid().SetRegion(local_rt,1.f))
	{
		printf("配置文件中的区域数据不正确(local_region)\n");
		return -2;
	}

	_plane.GetGrid().inner_region = local_rt;
	_plane.BuildSliceMask(15.f,_max_visible_range); 

	rect rt = _plane.GetGrid().grid_region;
	__PRINTF("Create grid: %d*%d step: %f sight range:%f slice in sight:%d\n", row,column,step, _max_visible_range,_plane.w_far_vision + 1);
	__PRINTF("Grid Region: {%.2f,%.2f} - {%.2f,%.2f}\n",rt.left,rt.top,rt.right,rt.bottom);

	/**
	 *	进行区域的简单检查
	 */
	rt = _plane.GetGrid().local_region;
	__PRINTF("Local Region: {%.2f,%.2f} - {%.2f,%.2f}\n",rt.left,rt.top,rt.right,rt.bottom);
	if(rt.left > rt.right - _max_visible_range || rt.top > rt.bottom - _max_visible_range)
	{
		__PRINTF("内部区域过小\n");
		return -5;
	}
	/*
	if(IsRaidWorld())
	{
		raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
		if(!rwt)
		{
			__PRINTF("初始化副本NPC通路图失败，找不到副本模板%d\n", GetRaidID());
			return -140;
		}
		_movemap = rwt->GetMoveMap();
	}
	else
	*/
	INIT_RES ret = OnInitMoveMap();
	if (ret == UNINITED)
	{
		/*
		 *  读入npc通路图
		 *
		 */
		std::string str1 = base_path + conf->find("MoveMap","Path");
		std::string str2 = base_path + conf->find("MoveMap","WaterPath");
		std::string str3 = base_path + conf->find("MoveMap","AirPath");
		if(!InitMoveMap(str1.c_str(),str2.c_str(),str3.c_str()))
		{
			__PRINTF("无法读入NPC通路图或者无法读入水域图㘎n");
			return -123;
		}

		/*
		 *       读取地形数据
		 */
		str = "Terrain_";
		str += servername;
		if(!InitTerrain(base_path, str ,_plane.GetLocalWorld()))
		{
			return -2;
		}
	}
	else if (ret != INIT_SUCCESS)
		return -140;
	std::string  regionfile = base_path + conf->find("Template","RegionFile");
	std::string  regionfile2 = base_path + conf->find("Template","RegionFile2");

	//装载城市区域
	/*
	if(IsRaidWorld())
	{
		raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
		if(!rwt)
		{
			__PRINTF("初始化城市区域失败，找不到副本模板%d\n", GetRaidID());
			return -142;
		}
		_region = rwt->GetCityRegion();
	}
	else
	*/
	ret = OnInitCityRegion();
	if (ret == UNINITED)
	{
		if(!InitRegionData(regionfile.c_str(),regionfile2.c_str()))
		{
			__PRINTF("can not load city region data from file '%s'\n",regionfile.c_str());
			return -7;
		}
	}
	else if (ret != INIT_SUCCESS)
		return -142;

	_region->GetRegionTime(_region_file_tag,_precinct_file_tag);

	/*
	if(IsRaidWorld())
	{
		raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
		if(!rwt)
		{
			__PRINTF("初始化BUFF区域失败，找不到副本模板%d\n", GetRaidID());
			return -143;
		}
		_buffarea = rwt->GetBuffArea();
	}
	else
	*/
	ret = OnInitBuffArea();
	if (ret == UNINITED)
	{
		_buffarea = new buff_area;
		_buffarea->Init(rt.left, rt.top, rt.right, rt.bottom, 1.0f, _region);
	}
	else if (ret != INIT_SUCCESS)
		return -143;
	/*
	if(IsRaidWorld())
	{
		raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
		if(!rwt)
		{
			__PRINTF("初始化NPC线路失败，找不到副本模板%d\n", GetRaidID());
			return -141;
		}
		_pathman = rwt->GetPathMan();
	}
	else
	*/
	ret = OnInitPathMan();
	if (ret == UNINITED)
	{
		//读取路线文件
		std::string pathfile = base_path + conf->find("Template","PathFile");
		_pathman = new path_manager;
		if(!_pathman->Init(pathfile.c_str()))
		{
			__PRINTF("无法打开路线文件\n");
			return -9;
		}
	}
	else if (ret != INIT_SUCCESS)
		return -141;

	CommonDataSpaceCreate(MAP_COMMON_DATA_TYPE_BEGIN + _world_index, CMN_DATA_LOCAL);

	std::string  npcgenfile = base_path + conf->find("Template","NPCGenFile");
	CNPCGenMan npcgen;
	if(!npcgen.Load(npcgenfile.c_str()))
	{
		__PRINTF("无法打开npc分布文件\n");
		return -8;
	}
	__PRINTF("一共有%d个怪物区域\n", npcgen.GetGenAreaNum());
	_plane.InitNPCGenerator(npcgen);
	
	return 0;
}

void global_world_manager::Release()
{
	printf("global_world_manager::Release(%d)\n", GetWorldTag());
	if(_message_handler) 
	{
		delete _message_handler;
		_message_handler = 0;
	}
	CommonDataSpaceDestroy(MAP_COMMON_DATA_TYPE_BEGIN + _world_index);
	_plane.Release();
	world_manager::Release();
	ReleasePathMan();
	ReleaseMoveMap();
	ReleaseRegionData();
	ReleaseBuffArea();
}

bool 
global_world_manager::InitNetIO(const char * servername)
{
	return true;
}

void 
global_world_manager::Heartbeat()
{
	world_manager::Heartbeat();
	_plane.RunTick();
}

void 
global_world_manager::GetPlayerCid(player_cid & cid)
{
	cid = _cid;
}

world * 
global_world_manager::GetWorldByIndex(size_t index)
{
	ASSERT(index == 0);
	return &_plane;
}

size_t
global_world_manager::GetWorldCapacity()
{
	return 1;
}

void 
global_world_manager::HandleSwitchStart(int uid, int source_tag, const A3DVECTOR & pos, const instance_key & key)
{
	gplayer * pPlayer = _plane.GetPlayerByID(uid);
	if(pPlayer)
	{
		GLog::log(GLOG_WARNING,"接受到切换请求时用户%d已存在(%d)", uid, pPlayer->login_state);
		//这个用户不应该存在的
		return;
	}

	//检查坐标是否和符合规则
	if(!_plane.PosInWorld(pos))
	{
		//这种情况不应出现
		ASSERT(false);
		return;
	}

	//发出通知玩家进入的消息
	MSG msg;
	BuildMessage(msg,GM_MSG_SWITCH_GET,XID(GM_TYPE_MANAGER,source_tag),XID(GM_TYPE_MANAGER,GetWorldTag()),
			pos, uid,&key, sizeof(key));
	gmatrix::SendWorldMessage(source_tag,msg);
}

int
global_world_manager::GetOnlineUserNumber() 
{ 
	return _plane.GetPlayerCount();
}

void 
global_world_manager::GetLogoutPos(gplayer_imp * pImp, int & world_tag,A3DVECTOR & pos)
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

void 
global_world_manager::SwitchServerCancel(int link_id,int user_id, int localsid)
{
	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("%d切换服务器被取消:%u.%u\n",user_id,(size_t)tv.tv_sec,(size_t)tv.tv_usec);
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

bool 
global_world_manager::IsUniqueWorld()
{
	return true;
}

int 
global_world_manager::SwitchPlayerFrom(world_manager *from, gplayer * pPlayer, const A3DVECTOR & pos, const instance_key & ins_key)
{
	world * pPlane = &_plane;
	ASSERT(pPlane->PosInWorld(pos));

	// 保存旧的tag pos
	int oldtag = from->GetWorldTag();
	A3DVECTOR oldpos = pPlayer->pos;
	//重新确定player的坐标
	pPlayer->pos = pos; 
	float height = GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pPlayer->pos.y < height) pPlayer->pos.y = height;

	if(pPlane->InsertPlayer(pPlayer) <0)
	{       
		//插入对象失败
		GLog::log(GLOG_ERR,"insert player error while switching %d",pPlayer->ID.id);
		ASSERT(false);
		return -1;
	}

	//重新设置世界指针
	pPlayer->plane = pPlane;
	pPlayer->imp->_plane = pPlane;
	
	//进行可能的数据交换操作
	TrySwapPlayerData(pPlane,_cid.cid,pPlayer);

	//重新发送一下数据
	gplayer_imp *pImp = (gplayer_imp*)pPlayer->imp;

	pImp->PlayerEnterMap( oldtag, oldpos );

	/*
	if(IsRaidWorld())
	{	
		pImp->_runner->notify_raid_pos(pPlayer->pos);
	}
	else
	{
		pImp->_runner->notify_pos(pPlayer->pos);
	}
	*/
	//liuyue-facbase
	pImp->_runner->notify_pos(pPlayer->pos);

	pImp->_runner->begin_transfer();
	pImp->_runner->enter_world();
	pImp->_runner->end_transfer();
	pImp->_runner->server_config_data();

	GLog::log(GLOG_INFO,"用户%d(%d,%d)转移到%d",pPlayer->ID.id, pPlayer->cs_index,pPlayer->cs_sid,pPlane->GetTag());
	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("%d转移服务器完成:%u.%u\n",pPlayer->ID.id,(size_t)tv.tv_sec,(size_t)tv.tv_usec);
	return 0;
}


world * 
global_world_manager::GetWorldLogin(int id, const GDB::base_info * pInfo, const GDB::vecdata * data)
{
	return &_plane;
}

