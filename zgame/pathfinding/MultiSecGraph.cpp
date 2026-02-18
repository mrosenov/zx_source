#include "MultiSecGraph.h"
#include "Graph.h"
#include "StdFile.h"

namespace SvrPF
{

//////////////////////////////////////////////////////////////////////////
//define&const
#define	   MEC_FILE_MAGIC      (unsigned int)(('m'<<24)| ('e'<<16)|('c'<<8)|('f'))
#define    MEC_FILE_VERSION     1


//////////////////////////////////////////////////////////////////////////
//class
CMultiSecGraph::CMultiSecGraph()
:m_pGraph(NULL)
{

}
CMultiSecGraph::~CMultiSecGraph()
{
	Release();
}

void CMultiSecGraph::Release()
{
	if (m_pGraph)
	{
		m_pGraph->Release();
		delete m_pGraph;
		m_pGraph = NULL;
	}

	m_aaSectors.clear();
	m_RgnPaths.clear();
}

void CMultiSecGraph::SetSectors(const vector<vector<Sector> >& aaSectors)
{
	m_aaSectors = aaSectors;
}

void CMultiSecGraph::SetRgnPaths(const vector<CPathTrack>& aPaths)
{
	m_RgnPaths = aPaths;
}

void CMultiSecGraph::SetGraph(const CGGraph * pGraph)
{
	m_pGraph = pGraph->Clone();
}

Sector * CMultiSecGraph::GetSector(int iLayer, const APointI& ptMap, int * pIndex)
{
	int index = -1;
	if (iLayer < 0 || iLayer >= (int)m_aaSectors.size())
	{
		if (pIndex)
		{
			*pIndex = index;
		}

		return NULL;
	}

	
	int row = ptMap.y / SECTOR_SIZE;
	int col = ptMap.x / SECTOR_SIZE;

	if (row >= 0 && row <m_iRowCount && col >= 0 && col < m_iColCount)
	{
		index = row * m_iColCount + col;
	}

	if (pIndex)
	{
		*pIndex = index;
	}

	if (index < 0)
	{
		return NULL;
	}

	return &(m_aaSectors[iLayer][index]);
}

bool CMultiSecGraph::Save(const char * szPath)
{
	if (!m_pGraph || m_aaSectors.empty())
	{
		return false;
	}
	StdFile sFile;
	if (!sFile.Open(szPath, IFILE_BIN|IFILE_WRITE))
	{
		return false;
	}
	unsigned int flag;
	flag = MEC_FILE_MAGIC;
	sFile.Write(&flag, sizeof(unsigned int));
	flag = MEC_FILE_VERSION;
	sFile.Write(&flag, sizeof(unsigned int));
	
	sFile.Write(&m_iColCount, sizeof(int));
	sFile.Write(&m_iRowCount, sizeof(int));

	//sector
	unsigned int count1, count2;
	unsigned int i, j;
	count1 = m_aaSectors.size();
	sFile.Write(&count1, sizeof(unsigned int));
	for (i = 0; i < count1; ++i)
	{
		count2 = m_aaSectors[i].size();
		sFile.Write(&count2, sizeof(unsigned int));
		for (j = 0; j < count2; ++j)
		{
			Sector& sec = m_aaSectors[i][j];
			sec.Save(sFile);
		}
	}
	//path
	count1 = m_RgnPaths.size();
	sFile.Write(&count1, sizeof(unsigned int));
	for (i = 0; i < count1; ++i)
	{
		CPathTrack& path = m_RgnPaths[i];
		path.Save(sFile);
	}


	//graph
	SaveSecGraph(m_pGraph, sFile);
	

	return true;	
}

bool CMultiSecGraph::Load(const char * szPath)
{
	Release();

	StdFile sFile;
	if (!sFile.Open(szPath, IFILE_BIN|IFILE_READ))
	{
		return false;
	}

	unsigned int flag;
	sFile.Read(&flag, sizeof(unsigned int));
	if (flag != MEC_FILE_MAGIC)
	{
		return false;
	}
	sFile.Read(&flag, sizeof(unsigned int));
	if (flag != MEC_FILE_VERSION)
	{
		return false;
	}

	sFile.Read(&m_iColCount, sizeof(int));
	sFile.Read(&m_iRowCount, sizeof(int));

	//sector
	unsigned int count1, count2;
	unsigned int i, j;

	sFile.Read(&count1, sizeof(unsigned int));
	for (i = 0; i < count1; ++i)
	{
		m_aaSectors.push_back(vector<Sector>());
		sFile.Read(&count2, sizeof(unsigned int));
		for (j = 0; j < count2; ++j)
		{
			Sector sec;
			sec.Load(sFile);
			m_aaSectors[i].push_back(sec);
		}
	}

	//path
	sFile.Read(&count1, sizeof(unsigned int));
	for (i = 0; i < count1; ++i)
	{
		CPathTrack path;
		path.Load(sFile);
		m_RgnPaths.push_back(path);
	}

	//graph
	m_pGraph = new CGGraph;
	LoadSecGraph(m_pGraph, sFile);

	return true;
}


}//end of namespace

