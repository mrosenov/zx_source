#ifndef _MULTI_SEC_GRAPH_H_
#define _MULTI_SEC_GRAPH_H_

#include "SectorCommon.h"
#include "PathTrack.h"

namespace SvrPF
{
class CGGraph;

class CMultiSecGraph
{
public:
	CMultiSecGraph();
	~CMultiSecGraph();
	void Release();
	
	void SetSectors(const vector<vector<Sector> >& aaSectors);
	void SetRgnPaths(const vector<CPathTrack>&   aPaths);
	void SetGraph(const CGGraph * pGraph);
	void SetParams(int rowCount, int colCount)
	{
		m_iRowCount = rowCount;
		m_iColCount = colCount;
	}
	
	int  GetRowCount() const
	{
		return m_iRowCount;
	}

	int  GetColCount() const
	{
		return m_iColCount;
	}
	
	Sector * GetSector(int iLayer, const APointI& ptMap, int * pIndex = NULL);
	CGGraph * GetGraph() const
	{
		return m_pGraph;
	}

	CPathTrack * GetPath(int index)
	{
		if (index >=0 && index < (int)m_RgnPaths.size())
		{
			return &m_RgnPaths[index];
		}
		return NULL;
	}
	
	bool Save(const char * szPath);
	bool Load(const char * szPath);

private:
	vector<vector<Sector> >      m_aaSectors;
	vector<CPathTrack>           m_RgnPaths;
	CGGraph						*m_pGraph;
	int							 m_iRowCount;
	int							 m_iColCount;

};

} //end of namespace
#endif
