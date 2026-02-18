#ifndef __ONLINE_GAME_GS_CITY_REGION_H__
#define __ONLINE_GAME_GS_CITY_REGION_H__

#include <common/types.h>
#include "el_precinct.h"
#include "el_region.h"

class  city_region {
	CELPrecinctSet __gl_ps;
	CELRegionSet __gl_rs;
	int  _world_tag;

public:
	city_region():_world_tag(-1){ }
	~city_region();

	void SetWorldTag(int tag) { _world_tag = tag; }
	bool InitRegionData(const char * precinct_path,const char * region_path);
	bool GetCityPos(float x, float z, A3DVECTOR & pos,int & world_tag);
	bool GetRegionLimit(float x, float z, int & reborn_cnt, int & max_level, int & min_level, int & kick_city,  A3DVECTOR & kick_pos);
	int GetDomainID(float x, float z);
	bool IsInSanctuary(float x, float z);
	bool GetRegionTransport(const A3DVECTOR & cur_pos, int src_tag, int ridx,  A3DVECTOR & target_pos, int & target_tag, int & target_line);
	void GetRegionTime(int &rtime, int &ptime);
	bool QueryTransportExist(int source_tag);
	int GetLevelLimit(int ridx);
	int GetControllerID(int ridx);

	int GetBuffAreaCnt();
	bool GetBuffArea(int idx, int& areaTid, float& x, float& y, float& width, float& height);
	bool IsInBuffArea(float x, float z);
};

#endif

