/********************************************************************
	created:	2008/7/30
	author:		liudong
	
	purpose:	使路径的轨迹平滑自然; 精简路径长度.
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _PF_PATH_OPTIMIZE_H_ 
#define _PF_PATH_OPTIMIZE_H_

#include <math.h>
#include <vector.h>
#include "PfCommon.h"

namespace SvrPF
{
class  CLayerMap;
class  CPathTrack;

class CPathOptimize
{
public:
	CPathOptimize();
	~CPathOptimize();
	
	//调用这个函数前，应该首先调用CPathTrack的 SetCurPos(APointF& pos) 函数，以从当前位置开始路径优化。
	bool OptimizePath(CPathTrack& pathTrack, CLayerMap* pMap);

protected:

	struct CPathTrack_PosByStep
	{
		PathNode  node;
		int       iPrevBasePos;
	};

	bool GetPathByStep(abase::vector<CPathTrack_PosByStep>& pathByStep, CPathTrack& pathTrack, float stepLen, float maxLen);

	bool AddLine(CPathTrack& pathTrack, CPathTrack_PosByStep& pos);
};

}

#endif//_PF_PATH_OPTIMIZE_H_


