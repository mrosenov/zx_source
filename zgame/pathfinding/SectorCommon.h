#ifndef _SECTOR_COMMON_H_
#define _SECTOR_COMMON_H_


#include <vector.h>
using namespace abase;

#include "MAStar.h"

//////////////////////////////////////////////////////////////////////////
//define&&const

#define  SECTOR_SIZE   16


class IFile;

namespace SvrPF
{

class CGGraph;

class SG_MExpandLimit:public IMExpandLimit
{
public:
	SG_MExpandLimit(const vector<int>& listSectors,int width);
	void    SetAllowSectors(const vector<int>& listSectors)
	{
		m_AllowSectors = listSectors;
	}
	virtual	bool Test(SvrPF::MapNode * n);
	virtual ~SG_MExpandLimit(){};
protected:
	vector<int>				 m_AllowSectors;
	unsigned int             m_xSectorNum;
};



class RegionMap
{
public:
	RegionMap(){};
	RegionMap(const RegionMap& rhs);
	~RegionMap(){};
	RegionMap * Clone() const;
	int  GetRegion(int index)
	{
		if (index < 0 || index >= SECTOR_SIZE * SECTOR_SIZE)
		{
			return -1;
		}

		return m_Region[index];
	}
	void SetRegion(int index, int region)
	{
		if (index < 0 || index >= SECTOR_SIZE * SECTOR_SIZE)
		{
			return;
		}
		
		m_Region[index] = region;
	}
	
	bool Load(IFile &iFile);
	bool Save(IFile &iFile);
	
	RegionMap& operator= (const RegionMap& rhs);
	
private:
	//todo: refine further if need.  [7/18/2008 kuiwu]
	char    m_Region[SECTOR_SIZE*SECTOR_SIZE]; // region index in the sector
};

class Sector
{
public:
	Sector();
	Sector(const Sector& rhs);
	~Sector();
	
	void Release();
	void AddRegion(int id)
	{
		m_RegionIds.push_back(id);
	}

	void SetRegion(int id, int index)
	{
		if (index >= 0 && index < (int)m_RegionIds.size())
		{
			m_RegionIds[index] = id;
		}
	}
	
	int GetRegion(int index) const
	{
		if (index < 0 || index >= (int)m_RegionIds.size())
		{
			return -1;
		}

		return m_RegionIds[index];
	}

	//only for passable pos
	int GetRegion(const APointI& ptMap) const
	{
		if (!m_pRegionMap)
		{
			return  (m_RegionIds.empty())? (-1):(m_RegionIds[0]);
		}

		APointI ptSec;
		Trans2Loacl(ptMap, ptSec);
		int rgnIndex = m_pRegionMap->GetRegion(ptSec.y * SECTOR_SIZE + ptSec.x);
		return (rgnIndex <0 )? (-1) : (m_RegionIds[rgnIndex]);
	}
	int  GetRegionCount() const 
	{
		return (int)m_RegionIds.size();
	}
	void SetRegionMap(const RegionMap * pRegionMap);

	bool Load(IFile& iFile);
	bool Save(IFile& iFile);

	Sector& operator=(const Sector& rhs);


	//return the index
	int     Find(int rgnId) const;
// 	int     Find(int x, int z, CGGraph * g) const;

	static  void Trans2Loacl(const APointI& ptMap, APointI& ptSec);
	static  void RetrieveBottomLeft(int iSec, int iColCount, APointI& bottomleft);
private:
	vector<int>  m_RegionIds;  //region(node) id in the graph
	RegionMap  * m_pRegionMap;
};

bool LoadSecGraph(CGGraph * g, IFile& iFile);
bool SaveSecGraph(CGGraph * g, IFile& iFile);


template<class InputIterator, class T> 
InputIterator linear_find(InputIterator first, InputIterator last,  T&  target)
{
	InputIterator it;
	for (it = first; it != last; ++it)
	{
		if (*it == target)
		{
			break;
		}
	}
	return it;
}


}//end of namespace



#endif

