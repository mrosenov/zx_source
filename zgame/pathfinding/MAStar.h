#ifndef _MASTAR_H_
#define _MASTAR_H_


#include <APoint.h>

#include "MapNode.h"


class CBitImage;


namespace SvrPF
{
class IMExpandLimit
{
public:
	IMExpandLimit(){};
	virtual ~IMExpandLimit(){};
	virtual	bool Test(MapNode * n) = 0;
};

class MClose;
class MapNode;

class CMAStar
{
public:
	CMAStar();
	~CMAStar();
	void    SetOpenHeap(bool bOpenHeap)
	{
		m_bOpenHeap = bOpenHeap;
	}
	float   Search(CBitImage * pRMap, const APointI& ptStart, const APointI& ptGoal, vector<APointI>& path, IMExpandLimit * pExpandLmt = NULL);
	int     GetNodeExpanded() const
	{
		return m_NodesExpanded;
	}
private:
	double  _GeneratePath(MClose& closelist, MapNode& dest, vector<APointI>& path);
protected:
	bool   m_bOpenHeap;  // use heap as open list
	int   m_NodesExpanded;
	int   m_NodesTouched;
	int   m_State;
	int   m_MaxExpand;


};

}

#endif


