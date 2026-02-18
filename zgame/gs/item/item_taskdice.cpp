#include "item_taskdice.h"
#include "../player_imp.h"
#include "../task/taskman.h"

int 
item_taskdice::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gplayer_imp)));
	if(obj->GetWorldManager()->IsRaidWorld() || obj->GetWorldManager()->IsFacBase())
	{
		__PRINTF("副本不能使用TaskDice\n");
		obj->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	//区域触发
	gplayer_imp *pImp = (gplayer_imp *)obj;
	if((TRIGGER_TYPE_CUBE == _area_trigger_type) && !IsInCubeArea(pImp->GetWorldManager()->GetWorldTag(), pImp->GetParent()->pos.x, pImp->GetParent()->pos.y, pImp->GetParent()->pos.z))
	{
		__PRINTF("不在指定区域不能使用TaskDice,TRIGGER_TYPE_CUBE,type=%d\n", _area_trigger_type);
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	if((TRIGGER_TYPE_SPHERE == _area_trigger_type) && !IsInSphereArea(pImp->GetWorldManager()->GetWorldTag(), pImp->GetParent()->pos.x, pImp->GetParent()->pos.y, pImp->GetParent()->pos.z))
	{
		__PRINTF("不在指定区域不能使用TaskDice,TRIGGER_TYPE_SPHERE,type=%d\n", _area_trigger_type);
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	//试图取得物品
	int task_idx = abase::RandSelect(&(_task_lists[0].probability),sizeof(_task_lists[0]), 8);
	int id = _task_lists[task_idx].id;
	PlayerTaskInterface  task_if((gplayer_imp*)obj);
	if(OnTaskCheckDeliver(&task_if,id,0))
	{
		__PRINTF("触发任务成功 %d\n",id);
		return 1;
	}
	obj->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

bool 
item_taskdice::SetTriggerPointList(int area_trigger_type, const void * value, int type2_map_id, float x, float y, float z, float radius)
{
	_area_trigger_type = area_trigger_type;

	if(TRIGGER_TYPE_CUBE == area_trigger_type)
	{
		ASSERT(NULL != value);
		memcpy(_trigger_cube_point, value, sizeof(_trigger_cube_point));	
	}
	else if(TRIGGER_TYPE_SPHERE == area_trigger_type)
	{
		_trigger_sphere_point.map_id	= type2_map_id;
		_trigger_sphere_point.x			= x;
		_trigger_sphere_point.y			= y;
		_trigger_sphere_point.z			= z;
		_trigger_sphere_point.radius	= radius;
	}

	return true;
}

bool 
item_taskdice::IsInCubeArea(int world_tag, float x, float y, float z) const
{
	for(int i = 0; i < 8; i++)
	{
		if( world_tag != (int)_trigger_cube_point[i].map_id ) 
		{
			continue;
		}

		float min_x = _trigger_cube_point[i].pos_min[0];
		float min_y = _trigger_cube_point[i].pos_min[1];
		float min_z = _trigger_cube_point[i].pos_min[2];

		float max_x = _trigger_cube_point[i].pos_max[0];
		float max_y = _trigger_cube_point[i].pos_max[1];
		float max_z = _trigger_cube_point[i].pos_max[2];

		if( (x > min_x && x < max_x)
		 && (y > min_y && y < max_y)
		 && (z > min_z && z < max_z) )
		{
			return true;
		}
	}

	return false;
}

bool 
item_taskdice::IsInSphereArea(int world_tag, float x, float y, float z) const
{
	//修改为判断点在圆内,由于编辑器上填的数据还是(x,y,z)，因此函数名暂时不改
	if(world_tag == _trigger_sphere_point.map_id)
	{
		float tmp_x = _trigger_sphere_point.x;
		float tmp_z = _trigger_sphere_point.z;
		float tmp_r = _trigger_sphere_point.radius;
		if( ((x-tmp_x)*(x-tmp_x) + (z-tmp_z)*(z-tmp_z)) < (tmp_r*tmp_r) )
		{
			return true;
		}
	}

	return false;
}
