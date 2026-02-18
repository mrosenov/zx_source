/********************************************************************
	created:	2008/7/30
	author:		liudong
	
	purpose:	Â·¾¶µÄ¹ì¼£.
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#include "PathTrack.h"
#include "PfCommon.h"
#include "IFile.h"
#include "AssertLowPriority.h"

#include <ABaseDef.h>
#include <math.h>

namespace SvrPF
{
	CPathTrack::CPathTrack()
	{
		
	}

	CPathTrack::~CPathTrack()
	{
		Release();
	}

	CPathTrack::CPathTrack(const CPathTrack& target)
	{
		*this = target;
	}
	
	CPathTrack& CPathTrack::operator=(const CPathTrack& target)
	{
		if (this==&target)
		{
			return *this;
		}

		m_Path = target.m_Path; 
		return *this;
	}
	
	CPathTrack* CPathTrack::Clone() const
	{
		return new CPathTrack(*this);
	}
	
	void CPathTrack::Release()
	{
		m_Path.clear();
	}

	bool CPathTrack::Init(abase::vector<APointI>& pathOri, int iLayer)
 	{
		Release();
		if (pathOri.size()<1)
		{
			ASSERT_LOW_PRIORITY(0);
			return false;
		}else if (1==pathOri.size())
		{
			PathNode pathNode;
			pathNode.ptMap = MAP_POINTI_2_POINTF(pathOri[0]);
			pathNode.layer = iLayer;
			m_Path.push_back(pathNode);
			return true;
		}

		abase::vector<APointF> path;

		APointF firstPos = MAP_POINTI_2_POINTF(pathOri[0]);
		path.push_back(firstPos);//first position
		int xDiff = pathOri[1].x -pathOri[0].x;
		int yDiff = pathOri[1].y -pathOri[0].y;
		int i;
		for (i=2;i<(int)pathOri.size();++i)
		{
			int xDiffTemp = pathOri[i].x - pathOri[i-1].x;
			int yDiffTemp = pathOri[i].y - pathOri[i-1].y;
			if (xDiffTemp!=xDiff || yDiffTemp!=yDiff)
			{
				APointF keyPos = MAP_POINTI_2_POINTF(pathOri[i-1]);
				path.push_back(keyPos);//corner
				xDiff = xDiffTemp;
				yDiff = yDiffTemp;
			}
		}

		APointF lastPos = MAP_POINTI_2_POINTF(pathOri[pathOri.size()-1]);
		path.push_back(lastPos);//last position

		for (i=0; i<(int)path.size(); ++i)
		{
			PathNode pathNode;
			pathNode.ptMap = path[i];
			pathNode.layer = iLayer;
			m_Path.push_back(pathNode);
		}
  		return true;
	}
	
	bool CPathTrack::Init(abase::vector<PathNode>& path)
	{
		Release();
		m_Path = path;
		return true;
	}

	bool CPathTrack::GetNextPos(PathNode& node,float& fStep)
	{
		if (m_Path.size()<2)
		{
			ASSERT_LOW_PRIORITY(0);
			fStep = 0.0f;
			return false;
		}
		if (fStep<=0.0f)
		{
			node  = m_Path[1];
			APointF  dir(m_Path[1].ptMap - m_Path[0].ptMap);
			fStep = sqrtf(dir.x *dir.x + dir.y *dir.y);
		}else
		{
			APointF  dir(m_Path[1].ptMap - m_Path[0].ptMap);
			float    len = sqrtf(dir.x *dir.x + dir.y *dir.y);
			if (fStep<len)
			{
				float rate = fStep/len;
				dir.x= dir.x*rate;
				dir.y= dir.y*rate;
				node = m_Path[1];//to copy layer. may be some more members in future.
				node.ptMap = m_Path[0].ptMap+dir;
			}else
			{
				node = m_Path[1];
				fStep= len;
			}
		}
		return true;
	}

	bool CPathTrack::SetCurPos(PathNode& node)
	{
		if (m_Path.size()<2)
		{
			if (m_Path.size()<1)
			{
				ASSERT_LOW_PRIORITY(0);
				return false;
			}else
			{
                m_Path[0] = node;
				return true;
			}
		}

		APointF  dir(m_Path[1].ptMap - node.ptMap);
		float    offsetX   = fabs(dir.x);
		float    offsetY   = fabs(dir.y);
		if (offsetX<REPULSION_DISTANCE && offsetY<REPULSION_DISTANCE)
		{
			m_Path.erase(&m_Path[0]);
		}else
		{
			m_Path[0] = node;
		}
		return true;
	}
	
	bool CPathTrack::GetCurPos(PathNode& node)
	{
		if (m_Path.size()<1)
		{
			return false;
		}
		node = m_Path[0];
		return true;
	}

	bool CPathTrack::GetEndPos(PathNode& node)
	{
		if (m_Path.size()<1)
		{
			return false;
		}
		node = m_Path[m_Path.size()-1];
		return true;
	}

	bool CPathTrack::ModifyPos(const PathNode& node, const unsigned int index)
	{
		if (index>=m_Path.size()) 
		{
			return false;
		}
		m_Path[index] = node;
		return true;
	}

	bool CPathTrack::Save(IFile& iFile)
	{ 
		unsigned int num = m_Path.size();
	    iFile.Write(&num,sizeof(unsigned int));
		for (unsigned int i=0;i<num;++i)
		{
			PathNode& node = m_Path[i];
			iFile.Write(&node.ptMap.x,sizeof(float));
			iFile.Write(&node.ptMap.y,sizeof(float));
			iFile.Write(&node.layer ,sizeof(int));
		}
		return true;
	}

	bool CPathTrack::Load(IFile& iFile)
	{
		Release();
		unsigned int num;
		iFile.Read(&num,sizeof(unsigned int));
		for (unsigned int i=0;i<num;++i)
		{
			PathNode node;
			iFile.Read(&node.ptMap.x,sizeof(float));
			iFile.Read(&node.ptMap.y,sizeof(float));
			iFile.Read(&node.layer ,sizeof(int));
			m_Path.push_back(node);
		}
		return true;
	}
	
	bool CPathTrack::GetNode(PathNode& node,const unsigned int index)
	{
		if (index>=m_Path.size())
		{
			ASSERT_LOW_PRIORITY(0);
			return false;		
		}
		node = m_Path[index];
		return true;
	}

	unsigned int CPathTrack::GetNodeCount()
	{
		return m_Path.size();
	}

	void CPathTrack::SetLayer(int iLayer)
	{
		int i;
		for (i = 0; i < (int)m_Path.size(); ++i)
		{
			m_Path[i].layer = iLayer;
		}
	}

	float CPathTrack::GetPathLen()
	{
		float len = 0.0f;
		for (int i=0; i<((int)m_Path.size()-1); ++i)
		{
			APointF offset = m_Path[i+1].ptMap - m_Path[i].ptMap;
			len += sqrtf(offset.x*offset.x + offset.y*offset.y);
		}
		return len;
	}

	float CPathTrack::GoAcrossPath(PathNode& node,const float fStep)
	{
		if (m_Path.size()<2 || fStep<=0.0f)
		{
			ASSERT_LOW_PRIORITY(0);
			if (m_Path.size()>=1)
			{
				node = m_Path[0]; 
			}
			return -1.0f;
		}
		
		float lenTotal = 0.0f;
		for (int i=0; i<((int)m_Path.size()-1); ++i)
		{
			APointF  offset(m_Path[i+1].ptMap - m_Path[i].ptMap);
			float    len = sqrtf(offset.x *offset.x + offset.y *offset.y);
			if (fStep<=lenTotal+len)
			{
				float rate = (fStep-lenTotal)/len;
				offset.x= offset.x*rate;
				offset.y= offset.y*rate;
				node = m_Path[i+1];//to copy layer. may be some more members in future.
				node.ptMap = m_Path[i].ptMap + offset;
				if (0!=i)
				{
					m_Path.erase(&m_Path[0],&m_Path[i]);
				}
				SetCurPos(node);
				return fStep;
			}else
			{
				lenTotal += len;
			}
		}
		if (m_Path.size()>1)
		{
			m_Path.erase(&m_Path[0],&m_Path[(int)m_Path.size()-1]);
		}
		node = m_Path[0];
		return lenTotal;
	}

	bool g_PF_AppendPath(CPathTrack& pathDest,const CPathTrack& pathSource)
	{
		if (pathDest.m_Path.size()<1)
		{
			pathDest = pathSource;
		}else if (pathSource.m_Path.size()>1)
		{
			bool bReversePath = true;
			if (fequal(pathDest.m_Path.back().ptMap.x, pathSource.m_Path.front().ptMap.x) &&
				fequal(pathDest.m_Path.back().ptMap.y, pathSource.m_Path.front().ptMap.y) &&
				pathDest.m_Path.back().layer == pathSource.m_Path.front().layer)
			{
				bReversePath = false;
			}else if (!fequal(pathDest.m_Path.back().ptMap.x, pathSource.m_Path.back().ptMap.x) ||
				      !fequal(pathDest.m_Path.back().ptMap.y, pathSource.m_Path.back().ptMap.y) ||
					  pathDest.m_Path.back().layer != pathSource.m_Path.back().layer)
			{
				ASSERT_LOW_PRIORITY(0);
				return false;
			}

			if (bReversePath)
			{
				for (int i=pathSource.m_Path.size()-2; i>=0; --i)
				{
					pathDest.m_Path.push_back(pathSource.m_Path[i]);
				}
			}else
			{
			    for (unsigned int i=1; i<pathSource.m_Path.size(); ++i)
				{
					pathDest.m_Path.push_back(pathSource.m_Path[i]);
				}	
			}
		}

		return true;
	}
}

