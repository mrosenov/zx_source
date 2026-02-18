#ifndef _PF_SEC_H_
#define _PF_SEC_H_


#include <vector.h>
using namespace abase;

#include "PathTrack.h"

class CBitImage;

namespace SvrPF
{

class CMoveMap;
class CGNode;
class CMultiSecGraph;

class CPfSec
{
public:
	CPfSec();
	virtual ~CPfSec();
	int  Search();
	int  GetNodeExpanded() const
	{
		return m_NodesExpanded;
	}
	int SetupSearch(CMoveMap * pMoveMap, int iLayerStart, const APointI& ptStart, int iLayerGoal, const APointI& ptGoal);
	int  StepSearch(int nExpands);
	void RetrievePath(CPathTrack& path);
	void CleanupSearch();
	int  GetState() const
	{
		return m_State;
	}
	APointI GetGoal(int& iLayerGoal)
	{
		iLayerGoal = m_iLayerGoal;
		return m_ptGoal;
	}
private:
	int _RgnSearch(CBitImage * pRMap, int iSector,  const APointI& ptStart, const APointI& ptGoal, vector<APointI>& path, int* nodeExpand = NULL);
	int _AbsSearch(CMultiSecGraph * pMultiSecGraph, CGNode * pNodeStart, CGNode * pNodeGoal, CPathTrack& path, int * nodeExpand = NULL);
private:
	int  m_NodesExpanded;
	int  m_iLayerStart;
	APointI m_ptStart;
	int  m_iLayerGoal;
	APointI m_ptGoal;
	CMoveMap * m_pMoveMap;
	CPathTrack m_Path;
	int		   m_InternalState;  //internal search state 
	int        m_State;          //normal state
};

} //end of namespace

#endif

