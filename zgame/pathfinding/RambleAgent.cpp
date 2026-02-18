/********************************************************************
	created:	2008/08/21
	author:		liudong
	
	purpose:	NPC巡逻逻辑。
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#include "RambleAgent.h"
#include "MoveMap.h"
#include "LayerMap.h"
#include "bitimage.h"
#include "PathOptimize.h"

#include <time.h>

CRambleAgent::CRambleAgent(SvrPF::CMoveMap * pMoveMap):m_pMoveMap(pMoveMap)
{
	m_fRange      = -1.0f;
	m_fMoveStep   = -1.0f;
	m_ptCurPos    = APointF(0,0);   
	m_ptGoalPos   = APointF(0,0);   
	m_iLayerCur   = -1; 
	m_iDirLast    = 0;

	m_iAgentState = PF_AGENT_STATE_UNKNOWN;
}

CRambleAgent::~CRambleAgent()
{
}


bool CRambleAgent::Init( const A3DVECTOR3& vCurPos)
{
	m_iLayerCur= -1;
	m_Path.Release();
	m_vCurPos     = vCurPos;

	m_ptCurPos = m_pMoveMap->TransWld2Map(APointF(m_vCurPos.x, m_vCurPos.z));
	m_ptGoalPos   = m_ptCurPos;
	float dH      = m_vCurPos.y - m_pMoveMap->GetTerrainHeight(m_vCurPos.x, m_vCurPos.z);
	m_iLayerCur   = m_pMoveMap->WhichLayer(POINTF_2_POINTI(m_ptCurPos), dH);
	
	if (!m_pMoveMap->GetLayer(m_iLayerCur)->GetRMap()->GetPixel((int)m_ptCurPos.x,(int)m_ptCurPos.y))
	{
		m_iAgentState = PF_AGENT_STATE_INVALID_START ;
	}else
	{
		m_iAgentState = PF_AGENT_STATE_READY ;
	}

	return true;
	
}

bool CRambleAgent::SetRambleInfo(const A3DVECTOR3& vCenter, float fRange)
{
	if (fRange<=0.0f)
	{
		return false;
	}

	m_fRange = fRange;
	m_vCenter = vCenter;
	APointF ptCenter = m_pMoveMap->TransWld2Map(APointF(vCenter.x, vCenter.z));
	m_ptCenter  = POINTF_2_POINTI(ptCenter);
	return true;
}

void CRambleAgent::_GeneratePath()
{
	if (IsStopped())
	{
		return;
	}

	//防止NPC最终都站到整数点上，随机浮点数位置。
	const int RANDOM_RANGE   = 100;
	float fRandom = (rand()%RANDOM_RANGE)/(float)RANDOM_RANGE;
	m_ptGoalPos.x += fRandom;
	fRandom = (rand()%RANDOM_RANGE)/(float)RANDOM_RANGE;
	m_ptGoalPos.y += fRandom;

	abase::vector<PathNode> nodeList;
	PathNode nodeStart,nodeEnd;
	nodeStart.ptMap = m_ptCurPos;
	nodeEnd.ptMap = m_ptGoalPos;
	nodeList.push_back(nodeStart);
	nodeList.push_back(nodeEnd);
	m_Path.Init(nodeList);
	m_Path.SetLayer(m_iLayerCur);
}

bool CRambleAgent::StartRamble()
{
	if(PF_AGENT_STATE_READY != m_iAgentState)
	{
		return false;
	}

	m_Path.Release();
	m_ptGoalPos = m_ptCurPos;
	if (!m_pMoveMap ||
		m_fRange<=0.0f||
		!m_pMoveMap->GetLayer(m_iLayerCur)->GetRMap()->GetPixel((int)m_ptCurPos.x,(int)m_ptCurPos.y))
	{
		return false;
	}

	APointI ptMin((int)(m_ptCenter.x-m_fRange),(int)(m_ptCenter.y-m_fRange));
	APointI ptMax((int)(m_ptCenter.x+m_fRange),(int)(m_ptCenter.y+m_fRange));
	if (m_fRange<=1.0f)
	{
		if (m_pMoveMap->GetLayer(m_iLayerCur)->GetRMap()->GetPixel(m_ptCenter.x,m_ptCenter.y) && 
			m_ptCenter!= POINTF_2_POINTI(m_ptCurPos) &&
			m_pMoveMap->GetLayer(m_iLayerCur)->LineTo(m_ptCurPos,MAP_POINTI_2_POINTF(m_ptCenter)))
		{
			//find it!
			m_ptGoalPos  = MAP_POINTI_2_POINTF(m_ptCenter);
			_GeneratePath();
			return true;
		}
	}else
	{
		const int RANDOM_RANGE_X = ptMax.x-ptMin.x+1;
		const int RANDOM_RANGE_Y = ptMax.y-ptMin.y+1;
		const int RANDOM_RANGE   = RANDOM_RANGE_X * RANDOM_RANGE_Y;
		const int RANDOM_TIMES   = 50;
		for (int i=0;i<RANDOM_TIMES;++i)
		{	
			int randomIndex = rand()%(RANDOM_RANGE+1);
			APointI   randomPos;
			randomPos.x = ptMin.x + randomIndex%RANDOM_RANGE_X;
			randomPos.y = ptMin.y + randomIndex/RANDOM_RANGE_X;

			if (m_pMoveMap->GetLayer(m_iLayerCur)->GetRMap()->GetPixel(randomPos.x,randomPos.y) && 
				randomPos!=POINTF_2_POINTI(m_ptCurPos) &&
				m_pMoveMap->GetLayer(m_iLayerCur)->LineTo(m_ptCurPos,MAP_POINTI_2_POINTF(randomPos)))
			{
				//find it!
				m_ptGoalPos  = MAP_POINTI_2_POINTF(randomPos);
				_GeneratePath();
				return true;
			}
		}	
	}

	const int DIR_COUNT = 8;
	const int pixelOffset[DIR_COUNT*2]=
	{
		-1, 0,//left
		-1, 1,//up left
		 0, 1,//up
		 1, 1,//up right
		 1, 0,//right
		 1,-1,//down right
		 0,-1,//down
		 -1,-1,//down left
	};
	for (int i=0;i<DIR_COUNT;++i)
	{
		int iDir = (m_iDirLast+i)%DIR_COUNT;
		APointI   neighborPos = POINTF_2_POINTI(m_ptCurPos) ;
		neighborPos.x += pixelOffset[2*iDir];
		neighborPos.y += pixelOffset[2*iDir+1];
		if (neighborPos.x>=ptMin.x &&
			neighborPos.y>=ptMin.y &&
			neighborPos.x<=ptMax.x &&
			neighborPos.y<=ptMax.y &&
			m_pMoveMap->GetLayer(m_iLayerCur)->GetRMap()->GetPixel(neighborPos.x,neighborPos.y))
		{
			//find it!
			m_iDirLast   = iDir;
			m_ptGoalPos  = MAP_POINTI_2_POINTF(neighborPos);
			_GeneratePath();
			return true;
		}
	}
    return false;	
}

void CRambleAgent::SetMoveStep(float fStep)
{
	if (fStep<=0.0f)
	{
		return;
	}

	m_fMoveStep = fStep;
}

bool CRambleAgent::IsStopped()
{
	if (PF_AGENT_STATE_READY!=m_iAgentState)
	{
		return true;
	}
	float offsetX = fabs(m_ptCurPos.x - m_ptGoalPos.x);
	float offsetY = fabs(m_ptCurPos.y - m_ptGoalPos.y);
	return (offsetX<=REPULSION_DISTANCE && offsetY<=REPULSION_DISTANCE);
}

void CRambleAgent::MoveOn()
{
	if (m_fMoveStep<=0.0f || IsStopped())
	{
		return;
	}
	
	if (m_Path.GetNodeCount() <= 1)
	{
		return;
	}

	float dist = m_fMoveStep ;
	PathNode node;
	if(m_Path.GoAcrossPath(node, dist)<=0.0f)
	{
		return;
	}
	m_ptCurPos = node.ptMap;
	m_vCurPos    = m_pMoveMap->TransMap2Wld(m_ptCurPos, m_iLayerCur, true);
}

A3DVECTOR3 CRambleAgent::GetCurPos() const
{
	return m_vCurPos ;
}

#ifdef SVRPF_EDITOR
void  CRambleAgent::GetPath(CPathTrack& path)
{
	path = m_Path;
}
#endif
	
