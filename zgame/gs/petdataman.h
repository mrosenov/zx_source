#ifndef __NETGAME_GS_PET_DATA_MAN_H__
#define __NETGAME_GS_PET_DATA_MAN_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>
#include "petman.h"
#include "playertemplate.h"
#include "property.h"

class itemdataman;

//宠物牌数据模板
struct pet_bedge_data_temp
{
	int tid;
	float body_size;
	int pet_type;
	int immune_type;
	int init_level;			//初始等级,没用了,脚本里控制
	float sight_range;
	float attack_range;		//宠物攻击距离
	int honor_point_inc_speed;	//亲密度增长速度
	int hunger_point_dec_speed[2];	//饱食度增长速度 0 非战斗 1 战斗
	int hp_gen[2];
	int vp_gen[2];
	float walk_speed;
	float run_speed;
};

class pet_bedge_dataman
{
	typedef abase::hashtab<pet_bedge_data_temp,int,abase::_hash_function> MAP;
	MAP _pt_map;
	
	bool __InsertTemplate(const pet_bedge_data_temp& pt)
	{       
		return _pt_map.put(pt.tid,pt);
	}       

	const pet_bedge_data_temp* __GetTemplate(int tid)
	{       
		return _pt_map.nGet(tid);
	}

	static pet_bedge_dataman& __GetInstance()
	{
		static pet_bedge_dataman __Singleton;
		return __Singleton; 
	}
	
	pet_bedge_dataman():_pt_map(1024) {}
	
public:
	static bool Insert(const pet_bedge_data_temp& pt)
	{
		bool rst = __GetInstance().__InsertTemplate(pt);
		ASSERT(rst);
		return rst;
	}

	static const pet_bedge_data_temp* Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman& dataman);
	static bool GenerateBaseProp(int tid,int level,q_extend_prop& prop);
};

//宠物装备数据模板
struct pet_equip_data_temp
{
	int tid;
	unsigned int pet_type_mask;	//宠物类别限制
	int level_require;		//需要级别
	int astro_type;			//主星象限定
	int astro_level;		//星等限定
	int equip_location;		//装备位置限定
	struct
	{
		int maximum;	// 上限
		int minimum;	// 下限
	} int_props[12];
	struct
	{
		float maximum;	// 上限
		float minimum;	// 下限
	} float_props[2];
};

class pet_equip_dataman
{
	typedef abase::hashtab<pet_equip_data_temp,int,abase::_hash_function> MAP;
	MAP _pt_map;
	
	bool __InsertTemplate(const pet_equip_data_temp& pt)
	{       
		return _pt_map.put(pt.tid,pt);
	}       

	const pet_equip_data_temp* __GetTemplate(int tid)
	{       
		return _pt_map.nGet(tid);
	}

	static pet_equip_dataman& __GetInstance()
	{
		static pet_equip_dataman __Singleton;
		return __Singleton; 
	}
	
	pet_equip_dataman():_pt_map(1024) {}
	
public:
	static bool Insert(const pet_equip_data_temp& pt)
	{
		bool rst = __GetInstance().__InsertTemplate(pt);
		ASSERT(rst);
		return rst;
	}

	static const pet_equip_data_temp* Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman& dataman);
	static bool GenerateBaseProp(int tid,int level,q_extend_prop& prop);
};

#endif

