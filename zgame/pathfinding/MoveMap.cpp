/********************************************************************
	created:	2006/05/14
	author:		kuiwu
	
	purpose:	map for pathfinding
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

//#include <A3DMacros.h>

#include "MoveMap.h"
#include "LayerMap.h"
#include "bitimage.h"
#include "MultiSecGraph.h"
#include "waterareamap.h"
#include "PfCommon.h"

namespace SvrPF
{
//////////////////////////////////////////////////////////////////////////
//define&const

const     float      SAME_LAYER_DH   = 1.0f;

//////////////////////////////////////////////////////////////////////////
//local vars


//////////////////////////////////////////////////////////////////////////
//local funcs


//small helper func used to replace _splitpath in *nix
//@todo: maybe strrchr is good enough?
void l_RetrieveDir(const char * path, char * dir)
{
#ifdef SVRPF_EDITOR
#define  IS_SLASH(c)    (*(c) == '\\' || *(c) == '/')
#else
#define  IS_SLASH(c)    (*(c) == '/')
#endif	

	const char * pLastSlash = NULL;

	const char * c;
	for (c = path; (*c) != '\0'; ++c)
	{
		if (IS_SLASH(c))
		{
			pLastSlash = c;
		}
	}

	if (!dir)
	{
		ASSERT_LOW_PRIORITY(0);
		return;
	}
	if (pLastSlash)
	{
		memcpy(dir, path, pLastSlash - path + 1);
		dir[pLastSlash-path +1] = '\0';
	}
	else
	{
		dir[0] = '\0';
	}
}
	
//////////////////////////////////////////////////////////////////////////
//static

//////////////////////////////////////////////////////////////////////////
//class CMoveMap
CMoveMap::CMoveMap()
:m_pMultiSecGraph(NULL),m_pWaterAreaMap(NULL)
{
}

CMoveMap::~CMoveMap()
{
	Release();
}

void CMoveMap::Release()
{
	int i;
	for (i = 0; i < (int)m_aLayers.size(); ++i)
	{
		if (m_aLayers[i])
		{
			m_aLayers[i]->Release();
			delete m_aLayers[i];
			m_aLayers[i] = NULL;
		}
	}
	m_aLayers.clear();
	
	ReleaseMultiSecGraph();

	if (m_pWaterAreaMap)
	{
		delete m_pWaterAreaMap;
		m_pWaterAreaMap = NULL;
	}
}


void CMoveMap::ReleaseMultiSecGraph()
{
	if (m_pMultiSecGraph)
	{
		m_pMultiSecGraph->Release();
		delete m_pMultiSecGraph;
		m_pMultiSecGraph = NULL;
	}
}




bool CMoveMap::Load(const char * szPath)
{
	Release();

	StdFile sFile;
	if (!sFile.Open(szPath, IFILE_BIN|IFILE_READ))
	{
		return false;
	}

	unsigned int flag;
	sFile.Read(&flag, sizeof(unsigned int));
	if (flag != PFMAP_CFG_FILE_MAGIC)
	{
		return false;
	}
	sFile.Read(&flag, sizeof(unsigned int));  
	if (flag != PFMAP_CFG_FILE_VERSION)
	{
		return false;
	}
	
	int count;
	sFile.Read(&count, sizeof(int));
	int i;
	//char buf[MAX_PATH];
	char name[1024];
	char dir[1024];
	char fullPath[1024];

	l_RetrieveDir(szPath, dir);
	
	int  len;
	for (i = 0; i < count; ++i)
	{
		CLayerMap * pLayerMap = new CLayerMap;
		m_aLayers.push_back(pLayerMap);

		sFile.Read(&len, sizeof(int));
		sFile.Read(name, sizeof(char)*len);
		name[len] ='\0';
		
		//rmap
		sprintf(fullPath, "%s%s", dir, name);
		if (!pLayerMap->LoadRMap(fullPath))
		{
			return false;
		}

		//dhmap
		sFile.Read(&flag, sizeof(unsigned int));
		if (flag != 0)
		{
			sFile.Read(&len, sizeof(int));
			sFile.Read(name, sizeof(char)*len);
			name[len] ='\0';
			sprintf(fullPath, "%s%s", dir, name);
			if (!pLayerMap->LoadDHMap(fullPath))
			{
				return false;
			}
		}
	}
	//mec
	sFile.Read(&len, sizeof(int));
	sFile.Read(name, sizeof(char)*len);
	name[len] ='\0';
	sprintf(fullPath, "%s%s", dir, name);

	m_pMultiSecGraph = new CMultiSecGraph;
	
	if (!m_pMultiSecGraph->Load(fullPath))
	{
		return false;
	}

	CalcOrigin();
	
	return true;
}

// bool CMoveMap::Save(const char * szPath, const vector<AString>& layerNames, const AString& mluName)
// {
// 
// 	if (!m_pMultiCluGraph || m_aLayers.empty())
// 	{
// 		return false;
// 	}
// 
// 	AFile   fileimg;
// 	if (!fileimg.Open(szPath, AFILE_BINARY|AFILE_CREATENEW))
// 	{
// 		return false;
// 	}
// 	DWORD  writelen;
// 
// 	DWORD  flag = PFMAP_CFG_FILE_MAGIC;
// 	fileimg.Write(&flag, sizeof(DWORD), &writelen);
// 	flag = PFMAP_CFG_FILE_VERSION;
// 	fileimg.Write(&flag, sizeof(DWORD), &writelen);
// 
// 	int count = (int) m_aLayers.size();
// 	fileimg.Write(&count, sizeof(int), &writelen);
// 
// 	int i;
// 	char buf[1024];
// 	int  len;
// 	for (i = 0; i < count; ++i)
// 	{
// 		strcpy(buf, layerNames[i]);
// 		len = strlen(buf);
// 		fileimg.Write(&len, sizeof(int), &writelen);
// 		fileimg.Write(buf, sizeof(char)*len, &writelen);
// 		
// 		flag = (m_aLayers[i]->GetRMap() != NULL)? (1):(0);
// 		fileimg.Write(&flag, sizeof(DWORD), &writelen);
// 
// 		flag = (m_aLayers[i]->GetDHMap() != NULL)? (1):(0);
// 		fileimg.Write(&flag, sizeof(DWORD), &writelen);
// 		
// 		flag = (m_aLayers[i]->GetIslandList() != NULL)? (1):(0);
// 		fileimg.Write(&flag, sizeof(DWORD), &writelen);
// 	}
// 
// 	//mlu
// 	strcpy(buf, mluName);
// 	len = strlen(buf);
// 	fileimg.Write(&len, sizeof(int), &writelen);
// 	fileimg.Write(buf, sizeof(char)*len, &writelen);
// 
// 
// 	return true;
// }


void CMoveMap::_SetMapCenterAsOrigin()
{
	m_vOrigin.Clear();
	m_vOrigin.x = m_iMapWidth * m_fPixelSize * 0.5f;
	m_vOrigin.z = m_iMapLength * m_fPixelSize * 0.5f;
}

CLayerMap * CMoveMap::CreateLayer()
{
	CLayerMap * pLayerMap = new CLayerMap;
	m_aLayers.push_back(pLayerMap);
	return pLayerMap;
}

void CMoveMap::CalcOrigin()
{
	CLayerMap * pLayerMap;
	if (!m_aLayers.empty())
	{
		pLayerMap = m_aLayers[0];
	}
	else
	{
		ASSERT_LOW_PRIORITY(0);
		return;
	}

	pLayerMap->GetRMap()->GetImageSize(m_iMapWidth, m_iMapLength);
	m_fPixelSize = pLayerMap->GetRMap()->GetPixelSize();
	_SetMapCenterAsOrigin();
}


APointF CMoveMap::TransMap2Wld(const APointI& ptMap) const
{

	return TransMap2Wld(MAP_POINTI_2_POINTF(ptMap));
}

APointF CMoveMap::TransMap2Wld(const APointF& ptMap) const
{
	float x, y;
	x  =-m_vOrigin.x + ptMap.x  * m_fPixelSize;
	y = -m_vOrigin.z + ptMap.y  * m_fPixelSize;
	return APointF(x, y);
}

APointF CMoveMap::TransWld2Map(const APointF& ptWld)const
{
// 	int x, y;
// 	x = (int)((ptWld.x + m_vOrigin.x) / m_fPixelSize );
//  y = (int)((ptWld.y + m_vOrigin.z) / m_fPixelSize );
//  return APointI(x,y);
    return APointF((ptWld.x + m_vOrigin.x)/m_fPixelSize, (ptWld.y + m_vOrigin.z)/m_fPixelSize) ;
}



int CMoveMap::WhichLayer(const APointI& ptMap, float dH) const
{
	int i; 
	
	int which = 0;
	float min_layer_dh = 100000.0f;
	for (i = 0; i < (int)m_aLayers.size(); ++i)
	{
		float layer_dh = GetDH(i, ptMap);
		float diff_dh = (float)fabs(layer_dh - dH);
		
		if (diff_dh < SAME_LAYER_DH)
		{
			return i;
		}
		
		if (diff_dh < min_layer_dh)
		{
			min_layer_dh = diff_dh;
			which = i;
		}

	}

	return which;
}


float CMoveMap::GetDH(int iLayer, const APointI& ptMap) const
{
	if (!m_aLayers[iLayer] || !(m_aLayers[iLayer]->GetDHMap()))
	{
		float defValue = 0.0f;
		if (m_aLayers[0] && m_aLayers[0]->GetDHMap())
		{
			FIX16  fixDh = m_aLayers[0]->GetDHMap()->GetDefaultValue();
			defValue = AM_FIX16TOFLOAT(fixDh);
		}
		return defValue; //this should not happen.
	}

	FIX16  fixDh = m_aLayers[iLayer]->GetDHMap()->GetPixel(ptMap.x, ptMap.y);
	return AM_FIX16TOFLOAT(fixDh);
}

CMultiSecGraph * CMoveMap::CreateMultiSecGraph()
{
	m_pMultiSecGraph = new CMultiSecGraph;
	return m_pMultiSecGraph;
}

// APointF CMoveMap::GetPixelCenter(const APointF& ptMap) const
// {
// 	float x, y;
// 	x = (int)ptMap.x + 0.5f;
// 	y = (int)ptMap.y + 0.5f;
// 
// 	return APointF(x, y);
// }


bool CMoveMap::IsPassable(const APointI& ptMap, int iLayer) const
{
	CLayerMap* pLayer = GetLayer(iLayer);
	if (!pLayer)
	{
		return false;
	}
	CBitImage* pRMap = pLayer->GetRMap();
	if (!pRMap)
	{
		return false;
	}
	
	return  pRMap->GetPixel(ptMap.x, ptMap.y);
}


bool CMoveMap::IsPassable(const A3DVECTOR3& vWld) const
{
	APointF ptMap = TransWld2Map(APointF(vWld.x, vWld.z));
	float dH = vWld.y - GetTerrainHeight(vWld.x, vWld.z);
	APointI ptMapI = POINTF_2_POINTI(ptMap);

	int layer = WhichLayer(ptMapI, dH);
	if (layer < 0)
	{
		return false;
	}

	return IsPassable(ptMapI, layer);
	
}

A3DVECTOR3 CMoveMap::GetSurfacePos(const A3DVECTOR3& vWld) const
{
	APointF ptMap = TransWld2Map(APointF(vWld.x, vWld.z));
	float dH = vWld.y - GetTerrainHeight(vWld.x, vWld.z);
	
	int layer = WhichLayer(POINTF_2_POINTI(ptMap), dH);
	if (layer < 0)
	{
		return vWld;
	}

	return TransMap2Wld(ptMap, layer);
}




A3DVECTOR3   CMoveMap::TransMap2Wld(const APointF& ptMap, int iLayer, bool bFixDH) const
{
	const float tor = 0.1f;

	APointF ptWld = TransMap2Wld(ptMap);
	APointI ptMapI = POINTF_2_POINTI(ptMap);
	float dH;
	if (bFixDH)
	{
		bool bPassable = IsPassable(ptMapI,iLayer);
		if (bPassable)
		{
			dH= GetDH(iLayer, ptMapI);
		}else
		{
			APointI ptTemp;
			GetPassablePosNearby(ptTemp,ptMap,iLayer,1);
			dH= GetDH(iLayer, ptTemp);
		}
	}else
	{
		dH= GetDH(iLayer, ptMapI);
	}
	
	float y;
	if (dH < tor)
	{//terrain
		y =	GetTerrainHeight(ptWld.x, ptWld.y);	
	}
	else
	{//building
		APointF ptMapCenter = MAP_POINTI_2_POINTF(ptMapI);
		APointF ptWldCenter = TransMap2Wld(ptMapCenter);
		y =	GetTerrainHeight(ptWldCenter.x, ptWldCenter.y);	
		y += dH;
	}

	return A3DVECTOR3(ptWld.x, y, ptWld.y);
}


bool CMoveMap::LoadWaterAreaMap(const char * szPath)
{
	if (m_pWaterAreaMap)
	{
		delete m_pWaterAreaMap;
		m_pWaterAreaMap = NULL;
	}

	m_pWaterAreaMap = new CWaterAreaMap;
	return m_pWaterAreaMap->Load(szPath);
}


float CMoveMap::GetWaterHeight(const A3DVECTOR3& vPos) const
{
	if (m_pWaterAreaMap)
	{
		return  m_pWaterAreaMap->GetWaterHeight(vPos);
	}

	return 0.0f;
}

bool CMoveMap::GetPassablePosNearby(APointI& ptPassable, const APointF& ptMap, const int iLayer, const int maxDistH) const
{
	for (int dist=1; dist<=maxDistH; ++dist)
	{
		const int DIST_1 = dist*2+1;
		const int DIST_2 = dist*2-1;
		const int MIN_X  = (int)(ptMap.x-dist);
		const int MAX_X  = (int)(ptMap.x+dist);
		const int MIN_Y  = (int)(ptMap.y-dist);
		const int MAX_Y  = (int)(ptMap.y+dist);

		int i;
		//left ; right
		for (i=0;i<DIST_1;++i)
		{
			ptPassable = APointI(MIN_X,MIN_Y+i);
			if (IsPassable(ptPassable,iLayer)){	return true;}

			ptPassable = APointI(MAX_X,MIN_Y+i);
			if (IsPassable(ptPassable,iLayer)){	return true;}
		}

		//bottom ; top
		for (i=0;i<DIST_2;++i)
		{
			ptPassable = APointI(MIN_X+1+i,MIN_Y);
			if (IsPassable(ptPassable,iLayer)){	return true;}
			
			ptPassable = APointI(MIN_X+1+i,MAX_Y);
			if (IsPassable(ptPassable,iLayer)){	return true;}
		}
	}

	ptPassable = POINTF_2_POINTI(ptMap);;
	return false;
}

}	// end of the namespace


