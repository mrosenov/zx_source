/********************************************************************
	created:	2008/05/23
	author:		kuiwu
	
	purpose:	layer move map
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/


#include "LayerMap.h"
#include "bitimage.h"
#include <math.h>
#include "PfCommon.h"

#ifdef SVRPF_EDITOR
#include "SecGraph.h"
#include "SectorGenerator.h"
#endif

namespace SvrPF
{

//////////////////////////////////////////////////////////////////////////
//local funcs



//////////////////////////////////////////////////////////////////////////
//class CLayerMap
CLayerMap::CLayerMap()
:m_pRMap(NULL), m_pDHMap(NULL)
{
#ifdef SVRPF_EDITOR
	m_pSecGraph = NULL;
#endif

}

CLayerMap::~CLayerMap()
{
	
}

void CLayerMap::Release()
{
	//A3DRELEASE(m_pDHMap);
	//A3DRELEASE(m_pRMap);
	
	if (m_pDHMap)
	{
		m_pDHMap->Release();
		delete m_pDHMap;
		m_pDHMap = NULL;
	}

	if (m_pRMap)
	{
		m_pRMap->Release();
		delete m_pRMap;
		m_pRMap = NULL;
	}
#ifdef SVRPF_EDITOR
	if (m_pSecGraph)
	{
		m_pSecGraph->Release();
		delete m_pSecGraph;
		m_pSecGraph = NULL;
	}
#endif


}


bool CLayerMap::LoadRMap(const char * szPath)
{
	Release();

	m_pRMap = new CBitImage;
	if (!m_pRMap->Load(szPath))
	{
		//A3DRELEASE(m_pRMap);
		m_pRMap->Release();
		delete m_pRMap;
		m_pRMap = NULL;

		return false;
	}
		
	return true;
}

bool CLayerMap::LoadDHMap(const char * szPath)
{
	//A3DRELEASE(m_pDHMap);
	if (m_pDHMap)
	{
		m_pDHMap->Release();
		delete m_pDHMap;
		m_pDHMap = NULL;
	}

	m_pDHMap = new CBlockImage<FIX16>;
	if (!m_pDHMap->Load(szPath))
	{
		//A3DRELEASE(m_pDHMap);
		m_pDHMap->Release();
		delete m_pDHMap;
		m_pDHMap = NULL;
		return false;
	}

	return true;
}


#ifdef SVRPF_EDITOR
bool CLayerMap::LoadSec(const char * szPath)
{
	if (m_pSecGraph)
	{
		m_pSecGraph->Release();
		delete m_pSecGraph;
		m_pSecGraph = NULL;
	}

	m_pSecGraph = new CSecGraph;
	if (!m_pSecGraph->Load(szPath))
	{
		m_pSecGraph->Release();
		delete m_pSecGraph;
		m_pSecGraph = NULL;
		return false;
	}
	
	return true;
} 


bool CLayerMap::CreateSec()
{
	if (m_pSecGraph)
	{
		m_pSecGraph->Release();
		delete m_pSecGraph;
		m_pSecGraph = NULL;
	}
	if (!m_pRMap)
	{
		ASSERT_LOW_PRIORITY(0);
		return false;
	}
	SectorGenerator  secGen;
	if (!secGen.GenerateAllSectors(m_pRMap, SECTOR_SIZE, SECTOR_SIZE))
	{
		return false;

	}

	m_pSecGraph = new CSecGraph;
	secGen.ExportSector(m_pSecGraph);

	return true;
}
#endif


bool CLayerMap::LineTo(const APointF& ptFrom, const APointF& ptTo)
{
	APointI   iPtTo  = POINTF_2_POINTI(ptTo);
	APointI   iPtCur = POINTF_2_POINTI(ptFrom);

	if (iPtCur == iPtTo)
	{
		return true;
	}
	
	APointF  dir(ptTo - ptFrom);
	float    len = sqrtf(dir.x *dir.x + dir.y *dir.y);
	if (len == 0.0f)
	{
		ASSERT_LOW_PRIORITY(0);
		return false;
	}
	dir.x /= len;
	dir.y /= len;
	int count = 0;
	
	while (((float)count < len) && (iPtCur != iPtTo))
	{
		++count;  //sample 1.0f
		iPtCur.x = (int)(ptFrom.x + dir.x *count);
		iPtCur.y = (int)(ptFrom.y + dir.y *count);

		if (!m_pRMap->GetPixel(iPtCur.x, iPtCur.y))
		{
			return false;
		}
	}


	return true;
}


}

