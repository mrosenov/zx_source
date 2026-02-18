#ifndef __ONLINEGAME_GS_RECIPE_TASKDICE_H__
#define __ONLINEGAME_GS_RECIPE_TASKDICE_H__

#include "../item.h"
#include "../config.h"

class item_taskdice : public item_body
{
	//类型1是矩形区域触发，类型2是圆球区域触发
	enum {TRIGGER_TYPE_CUBE = 1, TRIGGER_TYPE_SPHERE = 2};

	struct
	{
		unsigned int	id;
		float		probability;
	} _task_lists[8];

	//类型1矩形区域触发
	struct 
	{
		unsigned int	map_id;			// 地图号
		float			pos_min[3];		// 左下角(x,y,z)
		float			pos_max[3];		// 右上角(x,y,z)
	} _trigger_cube_point[8];

	//类型2圆球区域触发
	struct
	{
		int			map_id;		// 触发地图id
		float		x;			// 触发点(x,y,z)
		float		y;
		float		z;
		float		radius;		// 触发半径
	} _trigger_sphere_point;

	//该物品是否是区域触发型
	int _area_trigger_type;


public:
	item_taskdice()
	{
		memset(_task_lists,0,sizeof(_task_lists));
		_area_trigger_type = false;
		memset(_trigger_cube_point, 0, sizeof(_trigger_cube_point));
		memset(&_trigger_sphere_point, 0, sizeof(_trigger_sphere_point));
	}

	bool SetTaskList(const void * value)
	{
		memcpy(_task_lists,value, sizeof(_task_lists));
		float prob = 0.f;
		for(size_t i = 0; i < 8; i ++)
		{
			prob += _task_lists[i].probability;
		}
		return fabs(prob - 1.0f) < 1e-5;
	}

	bool SetTriggerPointList(int area_trigger_type, const void * value, int type2_map_id, float x, float y, float z, float radius);
	
	
public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_RECIPE;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return 0;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;


private:
	bool IsInCubeArea(int world_tag, float x, float y, float z) const;
	bool IsInSphereArea(int world_tag, float x, float y, float z) const;
};
#endif


