/********************************************************************
	created:	2008/08/13
	author:		liudong
	
	purpose:	NPC逃跑逻辑。
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#include "FleeAgent.h"
#include "MoveMap.h"
#include "PfCommon.h"
#include "LayerMap.h"
#include "bitimage.h"
#include "PathOptimize.h"

#define FLEE_AGENT_PI          3.1416f
#define FLEE_AGENT_RADIAN_STEP (FLEE_AGENT_PI/18)// 10 degree

CFleeAgent::CFleeAgent(SvrPF::CMoveMap * pMoveMap):m_pMoveMap(pMoveMap)
{
	m_iLayerCur= -1;
	m_fSafeDist= 0.0f;
	m_fMoveStep= 0.0f;
	m_iAgentState = PF_AGENT_STATE_UNKNOWN;
}

CFleeAgent::~CFleeAgent()
{
}

bool CFleeAgent::_IsOperateLegal()
{
	if (PF_AGENT_STATE_READY != m_iAgentState ||
	    m_fSafeDist<=0.0f ||
		m_fMoveStep<=0.0f ||
		m_iLayerCur<0)
	{
		return false;
	}
	return true;
}

bool CFleeAgent::Init(const A3DVECTOR3& vCurPos)
{
	m_iLayerCur= -1;
	m_Path.Release();

	m_vCurPos  = vCurPos;
	m_ptCurPos = m_pMoveMap->TransWld2Map(APointF(m_vCurPos.x, m_vCurPos.z));
	APointI ptCur = POINTF_2_POINTI(m_ptCurPos);
	float dH = m_vCurPos.y - m_pMoveMap->GetTerrainHeight(m_vCurPos.x, m_vCurPos.z);
	m_iLayerCur = m_pMoveMap->WhichLayer(ptCur, dH);

	if (!m_pMoveMap->GetLayer(m_iLayerCur)->GetRMap()->GetPixel(ptCur.x,ptCur.y))
	{
		m_iAgentState = PF_AGENT_STATE_INVALID_START ;
	}else
	{
		m_iAgentState = PF_AGENT_STATE_READY;
	}
	return true;
}

bool CFleeAgent::SetFleePos(const A3DVECTOR3& vFleePos, float fSafeDist)
{
	if (fSafeDist<=0.0f)
	{
		return false;
	}

	m_vFleePos  = vFleePos;
	m_ptFleePos = m_pMoveMap->TransWld2Map(APointF(m_vFleePos.x, m_vFleePos.z));
	m_fSafeDist = fSafeDist;
	return true;
}

bool CFleeAgent::StartFlee()
{
	m_Path.Release();
	if (!_IsOperateLegal())
	{
		return false;
	}
	APointF goalPos;
	bool    bCanGoStraight;
	bool bOK = _GenerateGoalPos(goalPos,bCanGoStraight,m_ptCurPos, m_ptFleePos, m_pMoveMap->GetLayer(m_iLayerCur) , m_fSafeDist);
	if (!bOK)
	{
		m_ptGoalPos = m_ptCurPos;
		m_vGoalPos = m_vCurPos;
		return false;
	}
	
	m_ptGoalPos = goalPos;
	m_vGoalPos  = m_pMoveMap->TransMap2Wld(m_ptGoalPos, m_iLayerCur);

	APointF diff= goalPos-m_ptCurPos;
	if (_IsVecZero(diff))
	{
		//当前位置就是安全位置，NPC不需要移动
	}else if(bCanGoStraight)
	{
		//NPC逃跑的目标点直线可达
		abase::vector<PathNode> vecNodes;
		vecNodes.push_back(PathNode());
		vecNodes.push_back(PathNode());
		vecNodes[0].ptMap = m_ptCurPos;
		vecNodes[0].layer = m_iLayerCur;
		vecNodes[1].ptMap = m_ptGoalPos;
		vecNodes[1].layer = m_iLayerCur;
		m_Path.Init(vecNodes);
	}else
	{
		//NPC逃跑的目标点直线不可达，需要通过寻径找到路径.
		CPfSec	pfAlg;
		APointI ptCur = POINTF_2_POINTI(m_ptCurPos);
		APointI ptGoal= POINTF_2_POINTI(m_ptGoalPos);
		int state =  pfAlg.SetupSearch(m_pMoveMap, m_iLayerCur, ptCur, m_iLayerCur, ptGoal);
		if (state != PF_STATE_SEARCHING)
		{
			m_ptGoalPos = m_ptCurPos;
			m_vGoalPos = m_vCurPos;
			return false;
		}
		state = pfAlg.StepSearch(5000);
		if (state != PF_STATE_FOUND)
		{
			m_ptGoalPos = m_ptCurPos;
			m_vGoalPos = m_vCurPos;
			return false;
		} 
		pfAlg.RetrievePath(m_Path);
		if (m_Path.GetNodeCount()>0) 
		{
			//寻路开始后的第一个点是整数的，将之变成原始的浮点数。
			PathNode pathNode;
			pathNode.ptMap = m_ptCurPos;
			pathNode.layer = m_iLayerCur;
			m_Path.ModifyPos(pathNode,0);
		}
	}
	return true;

}

bool CFleeAgent::FleeSuccess()
{
	if (PF_AGENT_STATE_READY != m_iAgentState || m_Path.GetNodeCount()<=1)
	{
		return true;
	}
	APointF diff = m_ptFleePos - m_ptCurPos;
	float dist2 = diff.x*diff.x + diff.y*diff.y;
	if (dist2>m_fSafeDist*m_fSafeDist-0.01)
	{
		return true;
	}
	return false;
}

void CFleeAgent::SetMoveStep(float fMoveStep)
{
	if (fMoveStep<=0.0f)
	{
		return ;
	}
	m_fMoveStep = fMoveStep;
}

A3DVECTOR3 CFleeAgent::GetCurPos() const
{
	return m_vCurPos;
}

#ifdef SVRPF_EDITOR
void CFleeAgent::GetPath(CPathTrack& path)
{
	path = m_Path;
}
#endif

bool CFleeAgent::_IsVecZero(APointF& vec)
{
	if (vec.x > 0.1f ||
		vec.x < -0.1f||
		vec.y > 0.1f ||
		vec.y < -0.1f)
	{
		return false;
	}
	return true;
}

bool CFleeAgent::_GenerateGoalPos(APointF& goalPos,bool& bCanGoStraight,const APointF& curPos, const APointF& fleePos, SvrPF::CLayerMap* pMap, float fSafeDist)
{
	bCanGoStraight = false;
	if (!pMap)
	{
		return false;
	}
	CBitImage* pRMap = pMap->GetRMap();
	if (!pRMap)
	{
		return false;
	}
	if (fSafeDist<0.1f)
	{
		return false;
	}
	
	float cosDir ;
	float sinDir ;
	APointF vDir = curPos - fleePos; 
	if (_IsVecZero(vDir))
	{
		vDir.x = 1.0f;
		vDir.y = 0.0f;
		cosDir = 1.0f;
		sinDir = 0.0f;
	}else
	{
		float dirLen = sqrtf((float)(vDir.x*vDir.x+vDir.y*vDir.y));
		if (dirLen>fSafeDist)
		{
			// cur pos is safe pos.
			goalPos = curPos;
			return true;
		}
		cosDir       = vDir.x/dirLen;
		sinDir       = vDir.y/dirLen;
	}
	
	APointF vPos,vFirstReachablePos(0.0f,0.0f);
	
	// First, we test the best pos
	vPos.x = fleePos.x + fSafeDist * cosDir;
	vPos.y = fleePos.y + fSafeDist * sinDir;
	
	if(pRMap->GetPixel((int)vPos.x,(int)vPos.y) && pMap->LineTo(curPos,vPos))
	{
		// Find it!
		goalPos = vPos;
		bCanGoStraight = true;
		return true;
	}else if (pRMap->GetPixel((int)vPos.x,(int)vPos.y))
	{
		vFirstReachablePos = vPos;
	}

	// Start the loop
	float cosStepRadian = cos(FLEE_AGENT_RADIAN_STEP);
	float sinStepRadian = sin(FLEE_AGENT_RADIAN_STEP);
	float cosClockwiseCur = cosDir;
	float sinClockwiseCur = sinDir;
	float cosAntiClockwiseCur = cosDir;
	float sinAntiClockwiseCur = sinDir;
	float fOffsetRadian = 0.0f;
	// Widen the range of real goal from flee pos.
	while (fOffsetRadian < FLEE_AGENT_PI /* PI/2 is old value! */)
	{
		float tmpCos,tmpSin;
		fOffsetRadian += FLEE_AGENT_RADIAN_STEP;
		
		// Anticlockwise Pos: (A+B)
		
		// cos(A+B) = cosAcosB - sinAsinB
		// sin(A+B) = sinAcosB + cosAsinB
		tmpCos = cosAntiClockwiseCur*cosStepRadian-sinAntiClockwiseCur*sinStepRadian;
		tmpSin = sinAntiClockwiseCur * cosStepRadian + cosAntiClockwiseCur * sinStepRadian;
		cosAntiClockwiseCur = tmpCos;
		sinAntiClockwiseCur = tmpSin;
		
		vPos.x = fleePos.x + fSafeDist * cosAntiClockwiseCur;
		vPos.y = fleePos.y + fSafeDist * sinAntiClockwiseCur;

		if(pRMap->GetPixel((int)vPos.x,(int)vPos.y) && pMap->LineTo(curPos,vPos))
		{
			// Find it!
			goalPos = vPos;
			bCanGoStraight = true;
			return true;
		}
		else if (pRMap->GetPixel((int)vPos.x,(int)vPos.y) && _IsVecZero(vFirstReachablePos))
		{
			vFirstReachablePos = vPos;
		}

		// Clockwise Pos: (A-B)
		// cos(A-B) = cosAcosB + sinAsinB
		// sin(A-B) = sinAcosB - cosAsinB
		tmpCos = cosClockwiseCur*cosStepRadian + sinClockwiseCur*sinStepRadian;
		tmpSin = sinClockwiseCur * cosStepRadian - cosClockwiseCur * sinStepRadian;
		cosClockwiseCur = tmpCos;
		sinClockwiseCur = tmpSin;
		
		vPos.x = fleePos.x + fSafeDist * cosClockwiseCur;
		vPos.y = fleePos.y + fSafeDist * sinClockwiseCur;

		if(pRMap->GetPixel((int)vPos.x,(int)vPos.y) && pMap->LineTo(curPos,vPos))
		{
			// Find it!
			goalPos = vPos;
			bCanGoStraight = true;
			return true;
		}
		else if (pRMap->GetPixel((int)vPos.x,(int)vPos.y) && _IsVecZero(vFirstReachablePos))
		{
			vFirstReachablePos = vPos;
		}
	}

	if(_IsVecZero(vFirstReachablePos))
	{
		// if the program goes here, it always means that we even find no reachable pos meeting the flee condition!
		// so we return curPos as the goalPos!
		goalPos = curPos;
		return false;
	}
	else
	{
		goalPos = vFirstReachablePos;
		return true;
	}
}

void CFleeAgent::MoveOn()
{
	if (!_IsOperateLegal() || FleeSuccess())

	{
		return ;
	}
	if (!m_pMoveMap)
	{
		return ;
	}
	
	CPathOptimize optimizer;
	optimizer.OptimizePath(m_Path, m_pMoveMap->GetLayer(m_iLayerCur));
	PathNode  node;
	if(m_Path.GoAcrossPath(node,m_fMoveStep)<=0.0f)
	{
		return;
	}
	
	if (1==m_Path.GetNodeCount())
	{
		//reach the target
		m_ptCurPos  = m_ptGoalPos;
		m_iLayerCur = node.layer;
		m_vCurPos   = m_vGoalPos;
	}else
	{
		m_ptCurPos  = node.ptMap;
		m_iLayerCur = node.layer;
		m_vCurPos = m_pMoveMap->TransMap2Wld(m_ptCurPos, m_iLayerCur, true);
	}
}
