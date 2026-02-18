#include "city_region.h"
#include "el_precinct.h"
#include "el_region.h"
#include "../worldmanager.h"

city_region::~city_region()
{
}

bool city_region::InitRegionData(const char * precinct_path,const char * region_path)
{
	return __gl_ps.Load(precinct_path) && __gl_rs.Load(region_path);
}
void city_region::GetRegionTime(int &rtime, int &ptime)
{
	ptime = __gl_ps.GetTimeStamp();
	rtime = __gl_rs.GetTimeStamp();
}

bool city_region::GetCityPos(float x, float z, A3DVECTOR & pos,int & world_tag)
{
	CELPrecinct*  pPrecinct = __gl_ps. IsPointIn(x, z, _world_tag);
	if(pPrecinct) 
	{
		const CELPrecinct::VECTOR3&  vec = pPrecinct->GetCityPos();
		pos.x = vec.x;
		pos.y = vec.y;
		pos.z = vec.z;
		world_tag = pPrecinct->GetDstInstanceID();
		return true;
	}
	else
	{
		return false;
	}
}

bool city_region::GetRegionLimit(float x, float z, int & reborn_cnt, int & max_level, int & min_level, int & kick_city,  A3DVECTOR & kick_pos)
{
	CELPrecinct*  pPrecinct = __gl_ps. IsPointIn(x, z, _world_tag);
	if(pPrecinct) 
	{
		reborn_cnt = pPrecinct->GetFeishengCnt();
		max_level = pPrecinct->GetAllowLevelMax();
		min_level = pPrecinct->GetAllowLevelMin();
		kick_city = pPrecinct->GetKickCityID();
		const CELPrecinct::VECTOR3&  vec = pPrecinct->GetKickCityPos();
		kick_pos.x = vec.x;
		kick_pos.y = vec.y;
		kick_pos.z = vec.z;
		return true;
	}
	else
	{
		return false;
	}
}

int city_region::GetDomainID(float x, float z)
{
	CELPrecinct*  pPrecinct = __gl_ps. IsPointIn(x, z, _world_tag);
	if(pPrecinct) 
	{
		return pPrecinct->GetDomainID();
	}
	return -1;
}

bool city_region::IsInSanctuary(float x, float z)
{
	//CELRegion*  pRegion = __gl_rs.IsPointInSanctuary(x, z);
	CELRegion*  pRegion = __gl_rs.IsPointInRegion(x, z);
	if(pRegion) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool city_region::GetRegionTransport(const A3DVECTOR & cur_pos, int src_tag, int ridx,  A3DVECTOR & target_pos, int & target_tag, int & target_line)
{
	CELTransportBox*  pBox = __gl_rs.GetTransportBox(ridx);
	if(!pBox) return false;

	if(_world_tag != pBox->GetSrcInstanceID()) return false;

	if(!pBox->IsPointIn(cur_pos.x,cur_pos.y,cur_pos.z))
	{
//		printf("副本传送区域不正确 %f,%f,%f\n",cur_pos.x,cur_pos.y,cur_pos.z);
		//区域不正确
		return false;
	} 
	
	if((target_tag = pBox->GetInstanceID()) != src_tag)
	{
		//目标不正确
		return false;
	}

	const CELTransportBox::VECTOR3 &t = pBox->GetTargetPos();
	target_pos.x = t.x;
	target_pos.y = t.y;
	target_pos.z = t.z;

	target_line = pBox->GetTransportLine();
	return true;
}

bool city_region::QueryTransportExist(int source_tag)
{
	size_t n = __gl_rs.GetTransportBoxNum();
	for(size_t i = 0; i < n ; i ++)
	{
		CELTransportBox*  pBox = __gl_rs.GetTransportBox(i);
		if(!pBox) 
		{
			ASSERT(false);
		}
		if(source_tag != pBox->GetSrcInstanceID()) return false;
	}
	return true;
}

int city_region::GetLevelLimit(int ridx)
{
	CELTransportBox*  pBox = __gl_rs.GetTransportBox(ridx);
	if(!pBox) return 0;

	return pBox->GetLevelLimit();
}

int city_region::GetControllerID(int ridx)
{
	CELTransportBox*  pBox = __gl_rs.GetTransportBox(ridx);
	if(!pBox) return 0;

	return pBox->GetControllerID();
}

int city_region::GetBuffAreaCnt()
{
	return __gl_rs.GetBuffRegionNum();
}

bool city_region::GetBuffArea(int idx, int& area_tid, float& left, float& top, float& right, float& bottom)
{
	area_tid = 0;
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
	
	CELBuffRegion* region = __gl_rs.GetBuffRegion(idx);
	if(!region)
	{
		return false;
	}
	area_tid = region->GetRegionTemplID();	
	left = region->GetLeft();
	top = region->GetTop();
	right = region->GetRight();
	bottom = region->GetBottom();
	return true;
}

bool city_region::IsInBuffArea(float x, float z)
{
	return __gl_rs.IsPointInBuffRegion(x, z);
}
