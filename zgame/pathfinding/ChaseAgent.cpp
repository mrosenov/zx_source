#include "ChaseAgent.h"
#include "MoveMap.h"
#include "PfCommon.h"
#include "LayerMap.h"
#include "bitimage.h"
#include "PathOptimize.h"

#include <time.h>

//////////////////////////////////////////////////////////////////////////
//local




//////////////////////////////////////////////////////////////////////////
CChaseAgent::CChaseAgent(CMoveMap * pMoveMap)
:m_pMoveMap(pMoveMap),m_ptFakeGoalOffset(0,0),m_bRandomGoal(false)
{
	m_iAgentState = PF_AGENT_STATE_UNKNOWN;
}

CChaseAgent::~CChaseAgent()
{
	
}


bool CChaseAgent::Init( const A3DVECTOR3& vCurPos)
{
	m_Path.Release();
	m_PfAlg.CleanupSearch();
	m_vCurPos  = vCurPos;
	m_ptFakeGoalOffset =APointI(0,0);
	m_bRandomGoal = false;
	
	m_ptCur = m_pMoveMap->TransWld2Map(APointF(m_vCurPos.x, m_vCurPos.z));
	APointI ptStart = POINTF_2_POINTI(m_ptCur);

	float dH = m_vCurPos.y - m_pMoveMap->GetTerrainHeight(m_vCurPos.x, m_vCurPos.z);
	m_iLayerCur = m_pMoveMap->WhichLayer(ptStart, dH);
	
	if (!m_pMoveMap->IsPassable(ptStart, m_iLayerCur))
	{
		APointI ptNei = ptStart;
		if (!_CheckNeighbor(ptNei, m_vCurPos, 5.0f, m_iLayerCur))
		{//invalid curpos
			m_iAgentState = PF_AGENT_STATE_INVALID_START;
			return true;
		}
		vector<PathNode> fakePath;
		PathNode pathNode;
		pathNode.ptMap = m_ptCur;
		pathNode.layer = m_iLayerCur;
		fakePath.push_back(pathNode);

		pathNode.ptMap = MAP_POINTI_2_POINTF(ptNei);
		pathNode.layer = m_iLayerCur;
		fakePath.push_back(pathNode);

		m_Path.Init(fakePath);
	}
	
	m_iAgentState = PF_AGENT_STATE_READY;
	return true;
}

bool CChaseAgent::TouchGoal(bool bPath)
{
    if (PF_AGENT_STATE_READY!=m_iAgentState)
	{
		return true;
	}

	const float tor = 0.1f;
	if (bPath && m_Path.GetNodeCount()>1)
	{
		PathNode pathEnd;
		m_Path.GetEndPos(pathEnd);
		A3DVECTOR3  vEnd = m_pMoveMap->TransMap2Wld(pathEnd.ptMap, pathEnd.layer);
		A3DVECTOR3  diff = m_vGoal - vEnd;
		float       diff_dist = diff.SquaredMagnitude();
		return (diff_dist < m_fGoalRange2 + tor);
	}

	if (m_bRandomGoal && m_Path.GetNodeCount() > 1)
	{
	    return false;
	}

	A3DVECTOR3  diff = m_vGoal - m_vCurPos;
	float diff_dist = diff.SquaredMagnitude();
	if (diff_dist < m_fGoalRange2 + tor)
	{
	    return true;
	}

	return false;
}

bool CChaseAgent::_SetFakeGoal()
{
	APointI fake_goal= m_ptGoal;
	bool bRet = _CheckNeighbor(fake_goal, m_vGoal, m_fGoalRange2, m_iLayerGoal);
	if (bRet)
	{
		m_ptFakeGoalOffset = fake_goal - m_ptGoal;
	}

	return bRet;
}


bool CChaseAgent::_CheckNeighbor(APointI& pt, const A3DVECTOR3& vCenter, float dist2, int iLayer)
{
	float dist = sqrtf(dist2);
	int count = (int)dist*2 +1;
	bool * visited =  new bool [count * count];

	//init to unvisited
	memset(visited, 0, sizeof(bool)*count*count);

	int i;
	APointI origin;
	origin.x = pt.x - (int)dist;
	origin.y = pt.y - (int)dist;
	
	CBitImage * pRMap = m_pMoveMap->GetLayer(iLayer)->GetRMap();


#define LOCAL_SET_VISITED(pt)   visited[(pt.y - origin.y) * count + (pt.x - origin.x)] =  true
#define LOCAL_IS_VISITED(pt)     (visited[(pt.y - origin.y) * count + (pt.x - origin.x)])

	bool bOK = false;
	vector<APointI> todo;
	todo.push_back(pt);
	LOCAL_SET_VISITED(pt);
	unsigned int  current = 0;
	while (current != todo.size())
	{
		pt = todo[current];
		++current;
		

		if (pRMap->GetPixel(pt.x, pt.y))
		{
			A3DVECTOR3 vCur =m_pMoveMap->TransMap2Wld(MAP_POINTI_2_POINTF(pt), iLayer);
			A3DVECTOR3 vDiff= vCenter - vCur;
			float diff_dist = vDiff.SquaredMagnitude();
			if (diff_dist < dist2)
			{
				bOK = true;
				break;
			}
		}

		//the  neighbors
		for (i = 0; i < PF_NEIGHBOR_COUNT; i++)
		{
			APointI neighbor;
			neighbor.x = pt.x + PF2D_NeighborD[i*2];
			neighbor.y = pt.y + PF2D_NeighborD[i*2+1];
			
			if (neighbor.x - origin.x >= 0 && neighbor.x - origin.x < count &&
				neighbor.y - origin.y >= 0 && neighbor.y - origin.y < count &&
				!LOCAL_IS_VISITED(neighbor))
			{
				ASSERT((neighbor.y - origin.y) * count + (neighbor.x - origin.x) < count * count);
				LOCAL_SET_VISITED(neighbor);
				todo.push_back(neighbor);
			}
		}
	
	}

	delete[] visited;
#undef  LOCAL_IS_VISITED
#undef  LOCAL_SET_VISITED
	return bOK;
}

bool CChaseAgent::SetGoal(const A3DVECTOR3& goal, float dist, bool bRandomGoal/*= false*/)
{
	m_vGoal = goal;
	m_fGoalRange2 = dist * dist;
	m_bRandomGoal = bRandomGoal;
	APointI lastFakeGoalOffset= m_ptFakeGoalOffset;
	
	APointF ptTmp = m_pMoveMap->TransWld2Map(APointF(goal.x, goal.z));
	m_ptGoal = POINTF_2_POINTI(ptTmp);
	m_ptFakeGoalOffset = APointI(0,0);

	float dH = goal.y - m_pMoveMap->GetTerrainHeight(goal.x, goal.z);
	m_iLayerGoal = m_pMoveMap->WhichLayer(m_ptGoal, dH);

	if (bRandomGoal && _RandomGoal(lastFakeGoalOffset))
	{
		return true;
	}

	if (!m_pMoveMap->GetLayer(m_iLayerGoal)->GetRMap()->GetPixel(m_ptGoal.x, m_ptGoal.y))
	{
		return _SetFakeGoal();
	}
	
	return true;
}

bool CChaseAgent::_RandomGoal(const APointI& lastFakeGoalOffset)
{
	if (m_fGoalRange2<=0.0f)
	{
		return false;
	}

	if (lastFakeGoalOffset.x!=0 || lastFakeGoalOffset.y!=0)
	{
		//check whether current fake goal is reachable.
		if (m_pMoveMap->GetLayer(m_iLayerGoal)->GetRMap()->GetPixel(m_ptGoal.x+lastFakeGoalOffset.x,m_ptGoal.y+lastFakeGoalOffset.y))
		{
			A3DVECTOR3 vFakeGoal =m_pMoveMap->TransMap2Wld(MAP_POINTI_2_POINTF(m_ptGoal+lastFakeGoalOffset), m_iLayerGoal);
			A3DVECTOR3 vDiff= m_vGoal - vFakeGoal;
			float diff_dist = vDiff.SquaredMagnitude();
			if (diff_dist < m_fGoalRange2)
			{
				m_ptFakeGoalOffset  = lastFakeGoalOffset;
				return true;
			}
		}
	}

	float dist = sqrtf(m_fGoalRange2);
	APointF ptRealGoal = m_pMoveMap->TransWld2Map(APointF(m_vGoal.x, m_vGoal.z));

	APointI ptMin((int)(ptRealGoal.x-dist),(int)(ptRealGoal.y-dist));
	APointI ptMax((int)(ptRealGoal.x+dist),(int)(ptRealGoal.y+dist));
	const int RANDOM_RANGE_X = ptMax.x-ptMin.x+1;
	const int RANDOM_RANGE_Y = ptMax.y-ptMin.y+1;
	const int RANDOM_RANGE   = RANDOM_RANGE_X * RANDOM_RANGE_Y;
	const int RANDOM_TIMES = 50;
	for (int i=0;i<RANDOM_TIMES;++i)
	{	
		int randomIndex = rand()%(RANDOM_RANGE+1);
		APointI   randomPos;
		randomPos.x = ptMin.x + randomIndex%RANDOM_RANGE_X;
		randomPos.y = ptMin.y + randomIndex/RANDOM_RANGE_X;

		if (m_pMoveMap->GetLayer(m_iLayerGoal)->GetRMap()->GetPixel(randomPos.x,randomPos.y) && randomPos!=m_ptGoal)
		{
			A3DVECTOR3 vRandom =m_pMoveMap->TransMap2Wld(MAP_POINTI_2_POINTF(randomPos), m_iLayerGoal);
			A3DVECTOR3 vDiff= m_vGoal - vRandom;
			float diff_dist = vDiff.SquaredMagnitude();
			if (diff_dist < m_fGoalRange2)
			{
				m_ptFakeGoalOffset  = randomPos-m_ptGoal;
				return true;
			}
		}
	}

	return false;
}

#ifdef SVRPF_EDITOR
void  CChaseAgent::GetPath(CPathTrack& path)
{
	path = m_Path;
}
#endif

int CChaseAgent::_RestartSearch(int nExpands)
{
	int iLayerStart;
	APointI ptStart;
	if (m_Path.GetNodeCount() > 0)
	{
		PathNode  pathNode;
		m_Path.GetEndPos(pathNode);
		iLayerStart = pathNode.layer;
		ptStart     = pathNode.GetPtI();
	}
	else
	{
		ptStart = POINTF_2_POINTI(m_ptCur);
		iLayerStart = m_iLayerCur;
	}
	APointI ptEnd = m_ptGoal +m_ptFakeGoalOffset; 
	int state =  m_PfAlg.SetupSearch(m_pMoveMap, iLayerStart, ptStart, m_iLayerGoal, ptEnd);
	if (state != PF_STATE_SEARCHING)
	{
		return state;
	}


	return _ContinueSearch(nExpands);
}

int CChaseAgent::_ContinueSearch(int nExpands)
{
	int state = m_PfAlg.StepSearch(nExpands);

	if (state == PF_STATE_SEARCHING || state == PF_STATE_FOUND )
	{
		CPathTrack   path;
		m_PfAlg.RetrievePath(path);
		if (0==m_Path.GetNodeCount() && path.GetNodeCount()>0) 
		{
			//寻路开始后的第一个点是整数的，将之变成原始的浮点数。
			PathNode pathNode;
			pathNode.ptMap = m_ptCur;
			pathNode.layer = m_iLayerCur;
			path.ModifyPos(pathNode,0);
		}
		if(!g_PF_AppendPath(m_Path, path))
		{
			return PF_STATE_UNKNOWN;
		}
		return state;
	}
	else 
	{
		return state;
	}
}


int CChaseAgent::Search(int nExpands /* = 1000 */)
{
	if (PF_AGENT_STATE_READY!=m_iAgentState)
	{
		return PF_STATE_UNKNOWN;
	}

	if (TouchGoal(true))
	{
		return PF_STATE_FOUND;
	}

	int       old_goal_layer;
	APointI old_goal = m_PfAlg.GetGoal(old_goal_layer);
	APointI ptEnd = m_ptGoal +m_ptFakeGoalOffset; 

	bool goal_changed = (old_goal_layer != m_iLayerGoal) || ( old_goal != ptEnd);
	
	if (goal_changed || 
		m_PfAlg.GetState() == PF_STATE_UNKNOWN ||
		m_PfAlg.GetState() == PF_STATE_SEARCHING)
	{
		return _RestartSearch(nExpands);
	}

	return m_PfAlg.GetState();
}

void CChaseAgent::MoveOn()
{
	if (PF_AGENT_STATE_READY!=m_iAgentState)
	{
		return ;
	}

	if (TouchGoal(false))
	{
		return;
	}
	
	if (m_Path.GetNodeCount()<=1)
	{
		return;
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
		m_ptCur     = MAP_POINTI_2_POINTF(m_ptGoal + m_ptFakeGoalOffset) ;
		m_iLayerCur = m_iLayerGoal;
		m_vCurPos   = m_pMoveMap->TransMap2Wld(m_ptCur, m_iLayerCur, true);
	}else
	{
		m_ptCur = node.ptMap;
		m_iLayerCur = node.layer;
		m_vCurPos = m_pMoveMap->TransMap2Wld(m_ptCur, m_iLayerCur, true);
	}
}
