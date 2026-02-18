/********************************************************************
	created:	2008/08/21
	author:		liudong
	
	purpose:	NPC巡逻逻辑。
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#ifndef _RAMBLE_AGENT_H_
#define _RAMBLE_AGENT_H_

#include "PathTrack.h"

#include <a3dvector.h>
#include <APoint.h>

using namespace SvrPF;
namespace SvrPF
{
	class CMoveMap;
}


class CRambleAgent
{
public:
	CRambleAgent(SvrPF::CMoveMap * pMoveMap);
	~CRambleAgent();

	//brief    : 初始化NPC巡逻.
	//pMoveMap : [in] 传入地图.
	//vCurPos  : [in] 传入NPC当前的位置.
	//return   : 初始化成功返回true, 否则返回false.
	bool Init(const A3DVECTOR3& vCurPos);

	//brief    : 设置NPC的巡逻范围.
	//vCenter  : [in] 传入NPC巡逻区域的中心点.
	//fRange   : [in] 传入NPC巡逻区域边长的一半. NPC巡逻区域是以2*fRange为边长的正方形。
	//return   : 设置成功返回true, 否则返回false.
	bool SetRambleInfo(const A3DVECTOR3& vCenter, float fRange);

	//brief    : 产生NPC巡逻的下一个目标点和巡逻路径.
	//return   : 产生一个目标点成功返回true，否则返回false.
	bool StartRamble();

	//brief    : 设置NPC行走的速度.
	//fMoveStep: [in] NPC每步行走的距离.
	void SetMoveStep(float fStep);

	//brief    : 判断NPC当前是否处于静止状态,非巡逻状态就是静止状态.
	//return   : 静止状态返回true，巡逻状态返回false.
	bool IsStopped();

	//brief    : 控制NPC行走一步.
	void MoveOn();

	//brief    : 返回NPC当前位置.
	A3DVECTOR3 GetCurPos() const;

#ifdef SVRPF_EDITOR
	//brief    : 返回NPC当前巡逻路径.
	void  GetPath(CPathTrack& path);
#endif

protected:
	void _GeneratePath();
private:
	SvrPF::CMoveMap*   m_pMoveMap;
	float              m_fRange;
	float              m_fMoveStep;

	A3DVECTOR3         m_vCenter;
	APointI            m_ptCenter;     //center pos in map
	A3DVECTOR3         m_vCurPos;
	APointF            m_ptCurPos;     //cur pos in map
	APointF            m_ptGoalPos;    //goal pos in map
	int                m_iLayerCur;    // cur layer
	SvrPF::CPathTrack  m_Path;
	int                m_iAgentState;
    int                m_iDirLast;//当没有合适的路可走的时候，随便选择一个方向走一步，该变量记录上次走的方向，以防NPC在几个点之间来回走死循环。
};

#endif

