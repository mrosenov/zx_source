#include "SectorCommon.h"
#include "IFile.h"
#include "Graph.h"


#include <memory.h>



namespace SvrPF
{
//////////////////////////////////////////////////////////////////////////
//class SG_MExpandLimit
SG_MExpandLimit::SG_MExpandLimit(const vector<int>& listSectors,int width)
:m_AllowSectors(listSectors), m_xSectorNum(width/SECTOR_SIZE)
{
}
bool SG_MExpandLimit::Test(MapNode * n)
{
	int index = n->z / SECTOR_SIZE * m_xSectorNum + n->x/SECTOR_SIZE;

	vector<int>::const_iterator it = linear_find(m_AllowSectors.begin(), m_AllowSectors.end(), index);
	return (it != m_AllowSectors.end());
}

//////////////////////////////////////////////////////////////////////////
//class RegionMap
RegionMap::RegionMap(const RegionMap& rhs)
{
	memcpy(m_Region, rhs.m_Region, sizeof(char)*SECTOR_SIZE*SECTOR_SIZE);
}

RegionMap * RegionMap::Clone() const
{
	return  new RegionMap(*this);
}


bool RegionMap::Load(IFile &iFile)
{
	iFile.Read(m_Region, sizeof(char)*SECTOR_SIZE*SECTOR_SIZE);
	return true;
}

bool RegionMap::Save(IFile &iFile)
{
	iFile.Write(m_Region, sizeof(char)*SECTOR_SIZE*SECTOR_SIZE);
	return true;
}

RegionMap& RegionMap::operator=(const RegionMap& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	memcpy(m_Region, rhs.m_Region, sizeof(char)*SECTOR_SIZE*SECTOR_SIZE);
	return *this;
}
//////////////////////////////////////////////////////////////////////////
//class Sector
Sector::Sector()
:m_pRegionMap(NULL)
{

}

Sector::Sector(const Sector& rhs)
{
	m_RegionIds = rhs.m_RegionIds;
	SetRegionMap(rhs.m_pRegionMap);	
}

Sector::~Sector()
{
	Release();
}

void Sector::Release()
{
	m_RegionIds.clear();
	if (m_pRegionMap)
	{
		delete m_pRegionMap;
		m_pRegionMap = NULL;
	}

}
void Sector::SetRegionMap(const RegionMap * pRegionMap)
{
	if (pRegionMap)
	{
		m_pRegionMap = pRegionMap->Clone();
	}
	else
	{
		m_pRegionMap = NULL;
	}
}

bool Sector::Save(IFile& iFile)
{
	int regionCount = GetRegionCount();
	iFile.Write(&regionCount, sizeof(int));
	int i;
	for (i = 0; i < regionCount; ++i)
	{
		int regionId = GetRegion(i);
		iFile.Write(&regionId, sizeof(int));
	}
	if (m_pRegionMap)
	{
		ASSERT_LOW_PRIORITY(GetRegionCount() > 1);
		m_pRegionMap->Save(iFile);
	}
	return true;
}

bool Sector::Load(IFile& iFile)
{
	Release();

	int regionCount;
	iFile.Read(&regionCount, sizeof(int));
	int i;
	for (i = 0; i < regionCount; ++i)
	{
		int regionId;
		iFile.Read(&regionId, sizeof(int));
		AddRegion(regionId);
	}

	if (GetRegionCount() > 1)
	{
		m_pRegionMap = new RegionMap;
		m_pRegionMap->Load(iFile);
	}

	return true;
}

Sector& Sector::operator=(const Sector& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_RegionIds = rhs.m_RegionIds;
	if (rhs.m_pRegionMap)
	{
		if (m_pRegionMap)
		{
			*m_pRegionMap = *(rhs.m_pRegionMap);
		}
		else
		{
			m_pRegionMap = rhs.m_pRegionMap->Clone();
		}
	}
	else
	{
		if (m_pRegionMap)
		{
			delete m_pRegionMap;
			m_pRegionMap = NULL;
		}
	}

	return *this;
}

int    Sector::Find(int rgnId) const 
{
	vector<int>::const_iterator it = linear_find(m_RegionIds.begin(), m_RegionIds.end(), rgnId);
	if (it != m_RegionIds.end())
	{
		return it - m_RegionIds.begin();
	}
	else
	{
		return -1;
	}
}

// int  Sector::Find(int x, int z, CGGraph * g) const
// {
// 	int i;
// 	for (i = 0; i < (int)m_RegionIds.size(); ++i)
// 	{
// 		CGNode * n = g->GetNode(m_RegionIds[i]);
// 		if (n && n->GetLabelL(CGNode::kXCoordinate) == x && n->GetLabelL(CGNode::kZCoordinate) == z)
// 		{
// 			return i;
// 		}
// 	}
// 
// 	return -1;
// }

void Sector::Trans2Loacl(const APointI& ptMap, APointI& ptSec)
{
	ptSec.x =  ptMap.x % SECTOR_SIZE;
	ptSec.y =  ptMap.y % SECTOR_SIZE;
}


void Sector::RetrieveBottomLeft(int iSec, int iColCount, APointI& bottomleft)
{
	int iCol = iSec % iColCount;
	int iRow = iSec / iColCount;

	bottomleft.x = iCol * SECTOR_SIZE;
	bottomleft.y = iRow * SECTOR_SIZE;
}

//////////////////////////////////////////////////////////////////////////
//global func
bool LoadSecGraph(CGGraph * g, IFile& iFile)
{
	int count, i;

	iFile.Read(&count, sizeof(int));
	for (i = 0; i < count; ++i)
	{
		CGNode * node = new CGNode;
		int layer;
		iFile.Read(&layer, sizeof(int));
		node->SetLabelL(CGNode::kLayer, layer);
		APointI pt;
		iFile.Read(&pt.x, sizeof(int));
		iFile.Read(&pt.y, sizeof(int));
		node->SetMapCoord(pt);
		g->AddNode(node);
	}

	iFile.Read(&count, sizeof(int));
	for (i = 0; i < count; ++i)
	{
		
		unsigned int from, to;
		iFile.Read(&from, sizeof(unsigned int));
		iFile.Read(&to, sizeof(unsigned int));
		double weight;
		iFile.Read(&weight, sizeof(double));
		int path;
		iFile.Read(&path, sizeof(int));
		CGEdge * edge = new CGEdge(from, to, weight, path);
		g->AddEdge(edge);
	}

	return true;
}

bool SaveSecGraph(CGGraph * g, IFile& iFile)
{
	int count;

	vector<CGNode*> * pAllNodes = g->GetAllNodes();
	count = (int)pAllNodes->size();
	iFile.Write(&count, sizeof(int));
	int i;

	for (i = 0; i < count; ++i)
	{
		CGNode * node = pAllNodes->at(i);
		int layer = node->GetLabelL(CGNode::kLayer);
		iFile.Write(&layer, sizeof(int));
		APointI pt = node->GetMapCoord();
		iFile.Write(&pt.x, sizeof(int));
		iFile.Write(&pt.y, sizeof(int));
	}

	vector<CGEdge*> * pAllEdges = g->GetAllEdges();
	count = (int)pAllEdges->size();
	iFile.Write(&count, sizeof(int));
	
	for (i = 0; i < count; ++i)
	{
		CGEdge * edge = pAllEdges->at(i);
		unsigned int which;
		which = edge->GetFrom();
		iFile.Write(&which, sizeof(unsigned int));
		which = edge->GetTo();
		iFile.Write(&which, sizeof(unsigned int));
		double weight;
		weight = edge->GetWeight();
		iFile.Write(&weight, sizeof(double));
		int  path;
		path = edge->GetLabelL(CGEdge::kEdgePath);
		iFile.Write(&path, sizeof(int));
	}
	
	
	return true;
}

}//end of namespace
