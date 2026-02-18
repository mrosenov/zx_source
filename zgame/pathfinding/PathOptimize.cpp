/********************************************************************
	created:	2008/7/30
	author:		liudong
	
	purpose:	使路径的轨迹平滑自然; 精简路径长度.
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#include "PathOptimize.h"
#include "LayerMap.h"
#include "PathTrack.h"

namespace SvrPF
{
	CPathOptimize::CPathOptimize()
	{
		
	}

	CPathOptimize::~CPathOptimize()
	{
		
	}
	
	bool CPathOptimize::GetPathByStep(abase::vector<CPathOptimize::CPathTrack_PosByStep>& pathByStep, CPathTrack& pathTrack, float stepLen, float maxLen)
	{
		pathByStep.clear();
		abase::vector<PathNode>&path = pathTrack.m_Path;
		if (path.size()<3)
		{
			return false;
		}

		bool     bMeetAnotherLayer = false;
		float    passedLen = stepLen;//passed length in current line
		int i;
		for(i=1; i<(int)path.size(); ++i)
		{
			if (path[i].layer!=path[1].layer)
			{
				bMeetAnotherLayer = true;
				break;
			}
			APointF  dir(path[i].ptMap - path[i-1].ptMap);
			float    lenLine = sqrtf(dir.x *dir.x + dir.y *dir.y);
			APointF  dirNormal(dir.x/lenLine,dir.y/lenLine);
			float    lenTarget = maxLen>lenLine?lenLine:maxLen;
            while(passedLen<=lenTarget)
            {
				if (1!=i)
				{
					CPathOptimize::CPathTrack_PosByStep posStep;
					posStep.node         = path[i];//to copy layer. may be some more members in future.
					posStep.node.ptMap.x = path[i-1].ptMap.x + dirNormal.x*passedLen;
					posStep.node.ptMap.y = path[i-1].ptMap.y + dirNormal.y*passedLen;
					posStep.iPrevBasePos = i-1;
					pathByStep.push_back(posStep);
				}
				passedLen+=stepLen;
            }
			
			if (maxLen>lenLine)
			{
				passedLen-= lenLine;
				maxLen   -= lenLine;
			}else
			{
				maxLen = -1.0f;//下面没有可以用来优化的点了。
				break;
			}
		}
		
		if (!bMeetAnotherLayer && fgreater(maxLen,0.0f))
		{
			CPathOptimize::CPathTrack_PosByStep posStep;
			posStep.node = path[path.size()-1];
			posStep.iPrevBasePos = path.size()-1;
			pathByStep.push_back(posStep);
 		}

		if (pathByStep.size()<2)
		{
			pathByStep.clear();
			return false;
		}else
		{
		    return true;	
		}		
	}

	bool CPathOptimize::AddLine(CPathTrack& pathTrack, CPathTrack_PosByStep& pos)
	{
		if (pos.iPrevBasePos<0 ||
			pos.iPrevBasePos>=(int)pathTrack.m_Path.size())
		{
			ASSERT_LOW_PRIORITY(0);
			return false;
		}
		
		abase::vector<PathNode>path;
		path.push_back(pathTrack.m_Path[0]);

		if (pos.iPrevBasePos == (int)pathTrack.m_Path.size()-1)
		{
			path.push_back(pathTrack.m_Path[pathTrack.m_Path.size()-1]);
		}else
		{
			PathNode& nodeNext = pathTrack.m_Path[pos.iPrevBasePos+1];
			APointF dir(pos.node.ptMap.x-nodeNext.ptMap.x, pos.node.ptMap.y-nodeNext.ptMap.y);
			if (!fequal(dir.x,0) || !fequal(dir.y,0))
			{
				path.push_back(pos.node);	
			}
			for (unsigned int i=pos.iPrevBasePos+1; i<pathTrack.m_Path.size(); ++i )
			{
				path.push_back(pathTrack.m_Path[i]);
			}
		}
		pathTrack.m_Path = path;
		return true;
	}

	bool CPathOptimize::OptimizePath(CPathTrack& pathTrack, CLayerMap* pMap)
	{
		if (pathTrack.m_Path.size()<3)
		{
			return true;
		}
		if (!pMap)
		{
			ASSERT_LOW_PRIORITY(0);
			return false;
		}

		abase::vector<CPathOptimize::CPathTrack_PosByStep> pathByStep;
		bool bResult = GetPathByStep(pathByStep, pathTrack, 1.0f, 30.0f);
		if (!bResult)
		{
			return true;
		}
		for (int i=pathByStep.size()-1; i>=0; --i)
		{
			CPathOptimize::CPathTrack_PosByStep posByStep = pathByStep[i];
			if (pMap->LineTo(pathTrack.m_Path[0].ptMap ,posByStep.node.ptMap))
			{
				AddLine(pathTrack, posByStep);
				break;
			}
 		}
		
		return true;
	}
}


