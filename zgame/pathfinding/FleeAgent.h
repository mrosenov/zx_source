/********************************************************************
	created:	2008/08/13
	author:		liudong
	
	purpose:	NPC逃跑逻辑。
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _FLEE_AGENT_H_
#define _FLEE_AGENT_H_

#include "PfSec.h"


#include <a3dvector.h>

using namespace SvrPF;
namespace SvrPF
{
	class CMoveMap;
	class CLayerMap;
	class CPathTrack;
}

class CFleeAgent
{
public:
	CFleeAgent(SvrPF::CMoveMap * pMoveMap);
	~CFleeAgent();

    //brief    : 初始化NPC逃跑.
	//pMoveMap : [in] 传入地图.
	//vCurPos  : [in] 传入NPC当前的位置.
	//return   : 初始化成功返回true, 否则返回false.
	bool Init(const A3DVECTOR3& vCurPos);

	//brief    : 设置NPC要逃离的危险点.
	//vFleePos : [in] 传入NPC要远离的危险点.
	//fSafeDist: [in] 传入安全距离，NPC与危险点直线距离大于等于该数值时，NPC处于安全状态.
	//return   : 设置成功返回true, 否则返回false.
	bool SetFleePos(const A3DVECTOR3& vFleePos, float fSafeDist);

	//brief    : 生成NPC逃跑路径.
	//return   : 生成路径成功返回true, 否则返回false.
	bool StartFlee();

	//brief    : 判断NPC当前是否处于安全位置.
	//return   : NPC当前处于安全位置返回true，否则返回false.
	bool FleeSuccess();

	//brief    : 设置NPC行走的速度.
	//fMoveStep: [in] NPC每步行走的距离.
	void SetMoveStep(float fMoveStep);

	//brief    : 控制NPC行走一步.
	void MoveOn();

	//brief    : 返回NPC当前位置.
	A3DVECTOR3 GetCurPos() const;

#ifdef SVRPF_EDITOR
	//brief    : 返回NPC当前逃跑路径.
    void GetPath(CPathTrack& path);
#endif

protected:
	//goalPos: 传出逃跑的目标点
	//bCanGoStraight:目标点是否直线可达.
	//curPos : 传入NPC当前位置
	//curPos : 传入NPC期望逃离的位置
	//pMap   : 传入地图文件
	//fSafeDist: 传入安全距离。 
	//return : 返回本次寻找安全位置的操作是否成功。
	bool _GenerateGoalPos(APointF& goalPos,bool& bCanGoStraight,const APointF& curPos, const APointF& fleePos, SvrPF::CLayerMap* pMap, float fSafeDist);

	bool _IsVecZero(APointF& vec);
	
	//当前进行寻路等操作是否合法。
	bool _IsOperateLegal();


protected:
    SvrPF::CMoveMap * m_pMoveMap;
	float             m_fSafeDist;
	float             m_fMoveStep;

	A3DVECTOR3        m_vCurPos;
	APointF           m_ptCurPos;
	int               m_iLayerCur;
	A3DVECTOR3        m_vFleePos;
	APointF           m_ptFleePos;
	A3DVECTOR3        m_vGoalPos;
	APointF           m_ptGoalPos;
	CPathTrack        m_Path;

	int               m_iAgentState;
};


#endif


