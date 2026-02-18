#include "PfSec.h"
#include "PfCommon.h"
#include "MAStar.h"
#include "MoveMap.h"
#include "bitimage.h"
#include "MultiSecGraph.h"
#include "LayerMap.h"
#include "Graph.h"
#include "GAStar.h"
#include "AssertLowPriority.h"

namespace  SvrPF
{

enum
{
	PFSEC_SETUP,
	PFSEC_START2RGN,
	PFSEC_RGN2RGN,
	PFSEC_RGN2GOAL,
	PFSEC_DONE,
};

CPfSec::CPfSec()
:m_pMoveMap(NULL)
{
	m_State = PF_STATE_UNKNOWN;
	m_ptGoal.x = PF_MAP_INVALID_NODE;
	m_ptGoal.y = PF_MAP_INVALID_NODE;
	m_iLayerGoal = -1;
}

CPfSec::~CPfSec()
{
	m_Path.Release();	
}

int CPfSec::_RgnSearch(CBitImage * pRMap, int iSector, const APointI& ptStart, const APointI& ptGoal, vector<APointI>& path, int * nodeExpand)
{
	int wid, len;
	pRMap->GetImageSize(wid, len);
	vector<int> allowSec;
	allowSec.push_back(iSector);
	SG_MExpandLimit   expandLmt(allowSec, wid);
	CMAStar mAStar;
	float cost = mAStar.Search(pRMap, ptStart, ptGoal, path, &expandLmt);
	
	if (nodeExpand)
	{
		*nodeExpand = mAStar.GetNodeExpanded();
	}

	if (cost < 0.0f)
	{	
		return PF_STATE_NOPATH;		
	}

	return PF_STATE_FOUND;
}

int CPfSec::_AbsSearch(CMultiSecGraph * pMultiSecGraph, CGNode * pNodeStart, CGNode * pNodeGoal, CPathTrack& path, int * nodeExpand)
{
	path.Release();
	CGAStar  gAStar;
	vector<CGEdge*> absPath;
	float cost = gAStar.Search(pMultiSecGraph->GetGraph(), pNodeStart, pNodeGoal, absPath);
	if (nodeExpand)
	{
		*nodeExpand = gAStar.GetNodeExpanded();
	}
	if (cost < 0.0f || absPath.empty())
	{
		return PF_STATE_NOPATH;
	}
	
	int i;
	vector<APointI> startPath;
	startPath.push_back(pNodeStart->GetMapCoord());
	path.Init(startPath, pNodeStart->GetLabelL(CGNode::kLayer));

	for (i = 0; i < (int)absPath.size(); ++i)
	{
		CGEdge * e = absPath[i];
		int edgeIndex = e->GetLabelL(CGEdge::kEdgePath);
		CPathTrack * pEdgePath = pMultiSecGraph->GetPath(edgeIndex);
		if (pEdgePath)
		{
			bool bResult = g_PF_AppendPath(path, *pEdgePath);
			if (!bResult)
			{
				ASSERT_LOW_PRIORITY(0);
				path.Release();
				return PF_STATE_UNKNOWN;
			}
		}else
		{
			ASSERT_LOW_PRIORITY(0);
			path.Release();
			return PF_STATE_NOPATH;
		}
	}

	return PF_STATE_FOUND;

}


int CPfSec::SetupSearch(CMoveMap * pMoveMap, int iLayerStart, const APointI& ptStart, int iLayerGoal, const APointI& ptGoal)
{
	m_Path.Release();
	m_NodesExpanded = 0;
	m_iLayerStart = iLayerStart;
	m_ptStart = ptStart;
	m_iLayerGoal = iLayerGoal;
	m_ptGoal = ptGoal;
	m_pMoveMap = pMoveMap;
	m_InternalState = PFSEC_SETUP;
	m_State = PF_STATE_SEARCHING;

	if (!m_pMoveMap->GetLayer(iLayerGoal)->GetRMap()->GetPixel(ptGoal.x, ptGoal.y))
	{
		m_InternalState = PFSEC_DONE;
		m_State = PF_STATE_INVALIDEND;
	}

	return m_State;

}

void CPfSec::CleanupSearch()
{
	m_Path.Release();
	m_State = PF_STATE_UNKNOWN;
	
}

int CPfSec::StepSearch(int nExpands)
{
	CMultiSecGraph * pMultiSecGraph = m_pMoveMap->GetMultiSecGraph();
	int iSectorStart, iSectorGoal;

	Sector * pSectorStart = pMultiSecGraph->GetSector(m_iLayerStart, m_ptStart, &iSectorStart);
	Sector * pSectorGoal = pMultiSecGraph->GetSector(m_iLayerGoal, m_ptGoal, &iSectorGoal);
	if (!pSectorStart || !pSectorGoal)
	{
		ASSERT_LOW_PRIORITY(0);
		m_State = PF_STATE_NOPATH;
		return m_State;
	}

	int  rgnIdStart = pSectorStart->GetRegion(m_ptStart);
	int  rgnIdGoal = pSectorGoal->GetRegion(m_ptGoal);
	CGNode * pNodeStart = pMultiSecGraph->GetGraph()->GetNode(rgnIdStart);
	CGNode * pNodeGoal = pMultiSecGraph->GetGraph()->GetNode(rgnIdGoal);
	if (!pNodeStart || !pNodeGoal)
	{
		ASSERT_LOW_PRIORITY(0);
		m_State = PF_STATE_NOPATH;
		return m_State;
	}

	int curExpands = 0;
	while (curExpands  < nExpands && m_State == PF_STATE_SEARCHING)
	{
		switch(m_InternalState)
		{
		case PFSEC_SETUP:
			{
				if (m_iLayerStart == m_iLayerGoal 
					&& m_pMoveMap->GetLayer(m_iLayerStart)->LineTo(MAP_POINTI_2_POINTF(m_ptStart), MAP_POINTI_2_POINTF(m_ptGoal)))
				{
					vector<PathNode> pathLine;
					PathNode node;
					node.ptMap = MAP_POINTI_2_POINTF(m_ptStart);
					node.layer = m_iLayerStart;
					pathLine.push_back(node);
					node.ptMap = MAP_POINTI_2_POINTF(m_ptGoal);
					pathLine.push_back(node);
					m_Path.Init(pathLine);

					m_State = PF_STATE_FOUND;
					return m_State;
				}
				
				if (rgnIdStart== rgnIdGoal)
				{
					vector<APointI> pathSec;
					m_State = _RgnSearch(m_pMoveMap->GetLayer(m_iLayerStart)->GetRMap(), iSectorStart, m_ptStart, m_ptGoal, pathSec, &m_NodesExpanded);
					if (m_State == PF_STATE_FOUND)
					{
						m_Path.Init(pathSec, m_iLayerStart);
					}
					return m_State;
				}
				m_InternalState = PFSEC_START2RGN;
			}
			break;
		case PFSEC_START2RGN:
			{
				CPathTrack  pathStart;
				int nodeExpand;
				if (pNodeStart->GetMapCoord() != m_ptStart)
				{
					vector<APointI> pathSec;
					int	state = _RgnSearch(m_pMoveMap->GetLayer(m_iLayerStart)->GetRMap(), iSectorStart, m_ptStart, pNodeStart->GetMapCoord(), pathSec, &nodeExpand);
					m_NodesExpanded += nodeExpand;
					curExpands += nodeExpand;
					if (state != PF_STATE_FOUND)
					{
						m_State = state;
						return m_State;
					}
					pathStart.Init(pathSec, m_iLayerStart);
					bool bResult = g_PF_AppendPath(m_Path, pathStart);
					if (!bResult)
					{
						ASSERT_LOW_PRIORITY(0);
						m_Path.Release();
						return PF_STATE_UNKNOWN ;
					}
				}
				m_InternalState = PFSEC_RGN2RGN;
			}
			break;
		case PFSEC_RGN2RGN:
			{
				CPathTrack pathAbs;
				int nodeExpand;
				int state = _AbsSearch(pMultiSecGraph, pNodeStart, pNodeGoal, pathAbs, &nodeExpand);
				m_NodesExpanded += nodeExpand;
				curExpands += nodeExpand;
				if (state != PF_STATE_FOUND)
				{
					m_State = state;
					return m_State;
				}
				bool bResult = g_PF_AppendPath(m_Path, pathAbs);
				if (!bResult)
				{
					ASSERT_LOW_PRIORITY(0);
					m_Path.Release();
					return PF_STATE_UNKNOWN ;
				}
	        
				m_InternalState = PFSEC_RGN2GOAL;
			}
			break;
		case PFSEC_RGN2GOAL:
			{
				CPathTrack pathGoal;
				int nodeExpand;
				if (pNodeGoal->GetMapCoord() != m_ptGoal)
				{
					vector<APointI> pathSec;
					int state = _RgnSearch(m_pMoveMap->GetLayer(m_iLayerGoal)->GetRMap(), iSectorGoal, pNodeGoal->GetMapCoord(), m_ptGoal, pathSec, &nodeExpand);
					m_NodesExpanded += nodeExpand;
					curExpands += nodeExpand;
					if (state != PF_STATE_FOUND)
					{
						m_State = state;
						return state;
					}
					pathGoal.Init(pathSec, m_iLayerGoal);
					bool bResult = g_PF_AppendPath(m_Path, pathGoal);
	                if (!bResult)
					{
						ASSERT_LOW_PRIORITY(0);
						m_Path.Release();
						return PF_STATE_UNKNOWN ;
					}
				}
				m_InternalState = PFSEC_DONE;				
				m_State = PF_STATE_FOUND;
			}
			break;
		default :
			break;
		}

	}

	return m_State;
}


int CPfSec::Search()
{
	m_State = PF_STATE_SEARCHING;
	while ((m_State = StepSearch(100))== PF_STATE_SEARCHING )
	{
	}


	return m_State;
}


void CPfSec::RetrievePath(CPathTrack& path)
{
	path = m_Path;
}

}


