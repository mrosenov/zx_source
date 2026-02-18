#include "general_indexer.h"
#include "general_indexer_cfg.h"
#include "gmatrix.h"
#include "world.h"
#include "worldmanager.h"
#include "commondata_if.h"

bool IsGlobalIndex(int idx) { return idx >= GENERAL_INDEX_GLOBAL_DESIGNER_BEGIN && idx <= GENERAL_INDEX_GLOBAL_DESIGNER_END; }
bool IsLineIndex(int idx) { return idx >= GENERAL_INDEX_LINE_DESIGNER_BEGIN && idx <= GENERAL_INDEX_LINE_DESIGNER_END; }
bool IsMapIndex(int idx) { return idx >= GENERAL_INDEX_MAP_DESIGNER_BEGIN && idx <= GENERAL_INDEX_MAP_DESIGNER_END; }

bool GeneralIndexer::Set(int index, int value)
{
	if (_check_perm && !_IsAccessible(index))
	{
		ASSERT(false && "该全局索引不允许访问");
		return false;
	}
	if (index >= GENERAL_INDEX_GLOBAL_BEGIN && index <= GENERAL_INDEX_GLOBAL_END) //全局数据
	{
		CommonDataInterface cif(0);
		if (!cif.IsConsistent())
		{
			return false;
		}
		cif.SetData (index, value);
		return true;
	}
	else if (index >= GENERAL_INDEX_LINE_BEGIN && index <= GENERAL_INDEX_LINE_END) //本线数据
	{
		ASSERT(_world_tag >=0 && "请先SetParam");
		world_manager * world_man = gmatrix::FindWorld(_world_tag);
		if(!world_man || world_man->IsIdle())
		{
			return false;
		}
		/*world *plane = gmatrix::FindWorld (_world_tag)->GetWorldByIndex(0);
		world_manager * world_man = plane->GetWorldManager();*/
		world_man->SetCommonData(index, value);
		return true;
	}
	else if (index >= GENERAL_INDEX_MAP_BEGIN && index <= GENERAL_INDEX_MAP_END)  //本地图数据
	{
		ASSERT(_world_tag >=0 && "请先SetParam");
		world_manager * world_man = gmatrix::FindWorld(_world_tag);
		if(!world_man || world_man->IsIdle())
		{
			return false;
		}
		/*world *plane = gmatrix::FindWorld (_world_tag)->GetWorldByIndex(0);
		world_manager * world_man = plane->GetWorldManager();*/
		world_man->SetCommonData(index, value);
		return true;
	}
	return false;
}
bool GeneralIndexer::Modify(int index, int offset)
{
	if (_check_perm && !_IsAccessible(index))
	{
		ASSERT(false && "该全局索引不允许访问");
		return false;
	}
	if (index >= GENERAL_INDEX_GLOBAL_BEGIN && index <= GENERAL_INDEX_GLOBAL_END) //全局数据
	{
		CommonDataInterface cif(0);
		if (!cif.IsConsistent())
		{
			return false;
		}
		cif.AddData (index, offset);
		return true;
	}
	else if (index >= GENERAL_INDEX_LINE_BEGIN && index <= GENERAL_INDEX_LINE_END) //本线数据
	{
		ASSERT(_world_tag >=0 && "请先SetParam");
		world_manager * world_man = gmatrix::FindWorld(_world_tag);
		if(!world_man || world_man->IsIdle())
		{
			return false;
		}
		/*world *plane = gmatrix::FindWorld (_world_tag)->GetWorldByIndex(0);
		world_manager * world_man = plane->GetWorldManager();*/
		world_man->ModifyCommonData(index, offset);
		return true;
	}
	else if (index >= GENERAL_INDEX_MAP_BEGIN && index <= GENERAL_INDEX_MAP_END)  //本地图数据
	{
		ASSERT(_world_tag >=0 && "请先SetParam");
		world_manager * world_man = gmatrix::FindWorld(_world_tag);
		if(!world_man || world_man->IsIdle())
		{
			return false;
		}
		/*world *plane = gmatrix::FindWorld (_world_tag)->GetWorldByIndex(0);
		world_manager * world_man = plane->GetWorldManager();*/
		world_man->ModifyCommonData(index, offset);
		return true;
	}
	return false;
}
bool GeneralIndexer::Get(int index, int &value)
{
	if (_check_perm && !_IsAccessible(index))
	{
		ASSERT(false && "该全局索引不允许访问");
		return false;
	}
	if (index >= GENERAL_INDEX_GLOBAL_BEGIN && index <= GENERAL_INDEX_GLOBAL_END) //全局数据
	{
		CommonDataInterface cif(0);
		if (!cif.IsConsistent())
		{
			return false;
		}
		value =0;
		cif.GetData (index,value);
		return true;
	}
	else if (index >= GENERAL_INDEX_LINE_BEGIN && index <= GENERAL_INDEX_LINE_END) //本线数据
	{
		ASSERT(_world_tag >=0 && "请先SetParam");
		world_manager * world_man = gmatrix::FindWorld(_world_tag);
		if(!world_man || world_man->IsIdle())
		{
			return false;
		}
		/*world *plane = gmatrix::FindWorld (_world_tag)->GetWorldByIndex(0);
		world_manager * world_man = plane->GetWorldManager();*/
		return world_man->GetCommonData(index, value);
	}
	else if (index >= GENERAL_INDEX_MAP_BEGIN && index <= GENERAL_INDEX_MAP_END)  //本地图数据
	{
		ASSERT(_world_tag >=0 && "请先SetParam");
		world_manager * world_man = gmatrix::FindWorld(_world_tag);
		if(!world_man || world_man->IsIdle())
		{
			return false;
		}
		/*world *plane = gmatrix::FindWorld (_world_tag)->GetWorldByIndex(0);
		world_manager * world_man = plane->GetWorldManager();*/
		return world_man->GetCommonData(index, value);
	}
	return false;
}

bool GeneralIndexer::_IsAccessible(int index)
{
	for (size_t i = 0; i< sizeof(access_table) / sizeof(access_table[0]); i++)
	{
		if (index >= access_table[i].range_low && index <= access_table[i].range_high)
		{
			return true;
		}
	}
	return false;
}
