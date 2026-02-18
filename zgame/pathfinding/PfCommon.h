/********************************************************************
	created:	2006/11/16
	author:		kuiwu
	
	purpose:	
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#ifndef _PF_CONSTNANT_H_
#define _PF_CONSTNANT_H_

#include <math.h>
#include <APoint.h>

namespace SvrPF
{



//////////////////////////////////////////////////////////////////////////
//enum
enum  
{
	PF_STATE_UNKNOWN,
	PF_STATE_SEARCHING, 
	PF_STATE_FOUND    , 
	PF_STATE_NOPATH,
	PF_STATE_EXCEED,
	PF_STATE_INVALIDSTART,
	PF_STATE_INVALIDEND,
};

enum  
{
	PF_AGENT_STATE_UNKNOWN,
	PF_AGENT_STATE_READY,
	PF_AGENT_STATE_INVALID_START,
};

//path finding algorithm
enum
{
	PF_ALGORITHM_NONE = -1,
	PF_ALGORITHM_ASTAR = 0,
	PF_ALGORITHM_BFS,
	//PF_ALGORITHM_CLU,
	PF_ALGORITHM_COUNT
};

//dir
enum
{
   PF_NEIGHBOR_LEFT = 0,
   PF_NEIGHBOR_RIGHT,
   PF_NEIGHBOR_TOP,
   PF_NEIGHBOR_BOTTOM,
   PF_NEIGHBOR_TOPRIGHT,
   PF_NEIGHBOR_BOTTOMRIGHT,
   PF_NEIGHBOR_TOPLEFT,
   PF_NEIGHBOR_BOTTOMLEFT,
   PF_NEIGHBOR_COUNT
};

//////////////////////////////////////////////////////////////////////////
//defines
#define	 MAKE_DWORD(l, h)    ((DWORD)(((WORD)((l) & 0xffff)) | ((DWORD)((WORD)((h) & 0xffff))) << 16))
#define  GET_LOWORD(dw)		((WORD)((DWORD)(dw) & 0xffff))	
#define	 GET_HIWORD(dw)		((WORD)((DWORD)(dw) >> 16))

#define PF_MAP_INVALID_NODE    (-30000)

#define  MAP_POINTI_2_POINTF(ptI) APointF( (int)((ptI).x)+0.5f , (int)((ptI).y)+0.5f )
#define  POINTF_2_POINTI(ptF) APointI( (int)((ptF).x) , (int)((ptF).y))

typedef unsigned char UCHAR;



//////////////////////////////////////////////////////////////////////////
//types
struct PathNode 
{
	APointF ptMap;
	int       layer;


	APointI GetPtI()
	{
		return APointI((int)ptMap.x, (int)ptMap.y);
	}
	void SetI(const APointI& pt, int iLayer)
	{
		ptMap = MAP_POINTI_2_POINTF(pt);
		layer = iLayer;
	}
};

//////////////////////////////////////////////////////////////////////////
//const var
const double   PF_ROOT_TWO  = 1.414;
// Floating point comparisons 
const double ZERO_TOLERANCE = 0.0001;    // floating point tolerance

	
//////////////////////////////////////////////////////////////////////////
//inline funcs
inline bool fless(double a, double b) { return (a < b - ZERO_TOLERANCE); }
inline bool fgreater(double a, double b) { return (a > b + ZERO_TOLERANCE); }
bool fequal(double a, double b);


inline float GetManhDist(int x1, int z1, int x2, int z2)
{
	 return (float)(fabs(x1- x2) + fabs(z1 - z2));
}
inline float GetEuclDist(int x1, int z1, int x2, int z2)
{
	return (sqrtf((x1 - (float)x2)*(x1 -x2) + (z1 -z2)*(z1 -z2)));
}

//////////////////////////////////////////////////////////////////////////
//extern var
extern int  PF2D_NeighborD[];
extern float PF2D_NeighborCost[];

} //end of namespace


#endif

