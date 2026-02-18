#ifndef  _CHASE_AGENT_H_
#define  _CHASE_AGENT_H_


#include "PfSec.h"
using namespace SvrPF;

#include <a3dvector.h>
#include <APoint.h>

class CChaseAgent
{
public:
	CChaseAgent(CMoveMap * pMoveMap);
	~CChaseAgent();
	bool Init(const A3DVECTOR3& vCurPos);
	/**
	 * \brief find a path
	 * \param[in] nExpands, nodes expanded.
	 * \param[out]
	 * \return  the search state 
	 * \note
	 * \warning 
	 * \todo   
	 * \author kuiwu 
	 * \date 21/8/2008
	 * \see 
	 */
	int  Search(int nExpands = 1000);
	/**
	 * \brief  set the goal
	 * \param[in]  bRandomGoal: true if simulate the block chasing
	 * \param[out] 
	 * \return  false if the given goal is not passable and 
				can not set a 'fake' goal.
	 * \note   set a 'fake' goal in the range dist if goal is not passable
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 21/8/2008
	 * \see 
	 */
	bool SetGoal(const A3DVECTOR3& goal, float dist, bool bRandomGoal = false);
	
	/**
	 * \brief check if the curpos(pathend) is touched the goal
	 * \param[in] bPath: additionally check the pathend if true
	 * \param[out]
	 * \return  true if touched
	 * \note
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 21/8/2008
	 * \see 
	 */
	bool TouchGoal(bool bPath);	
	
	void SetMoveStep(float fStep)
	{
		m_fMoveStep = fStep;
	}

	A3DVECTOR3 GetCurPos() const
	{
		return m_vCurPos;
	}
	/**
	 * \brief move one step
	 * \param[in]
	 * \param[out]
	 * \return
	 * \note
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 21/8/2008
	 * \see 
	 */
	void       MoveOn();
#ifdef SVRPF_EDITOR
	void  GetPath(CPathTrack& path);
#endif
private:
	bool _SetFakeGoal();
	bool _CheckNeighbor(APointI& pt, const A3DVECTOR3& vCenter, float dist2, int iLayer);
	int  _RestartSearch(int nExpands);
	int  _ContinueSearch(int nExpands);

	//set a random pos as goal, called after function SetGoal(...)
	//lastGoalOffset: last fake goal's offset from real goal.try this pos first .
	bool _RandomGoal(const APointI& lastFakeGoalOffset);
private:
	CMoveMap *		m_pMoveMap;
	CPfSec			m_PfAlg;
	CPathTrack		m_Path;
	A3DVECTOR3      m_vGoal;
	float           m_fGoalRange2;   //sqr (dist to goal) tolerance
	APointI       m_ptGoal;  //goal in the map
	APointI       m_ptFakeGoalOffset;
	int             m_iLayerGoal;
	
	A3DVECTOR3      m_vCurPos;
	APointF         m_ptCur;   //cur pos in map
	int             m_iLayerCur; // cur layer
	float           m_fMoveStep;
	bool            m_bRandomGoal;

	int             m_iAgentState;
};


#endif

