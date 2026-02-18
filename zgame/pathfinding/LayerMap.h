/********************************************************************
	created:	2008/05/23
	author:		kuiwu
	
	purpose:	layer move map
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _LAYER_MAP_H_
#define _LAYER_MAP_H_

#include "blockimage.h"
#include <APoint.h>

class CBitImage;



namespace SvrPF
{

#ifdef SVRPF_EDITOR
class CSecGraph;
#endif

class CLayerMap
{
public:
	CLayerMap();
	~CLayerMap();
	
	bool LoadRMap(const char * szPath);
	bool LoadDHMap(const char * szPath);
	CBitImage * GetRMap() const
	{
		return m_pRMap;
	}
	CBlockImage<FIX16> * GetDHMap() const
	{
		return m_pDHMap;
	}
#ifdef SVRPF_EDITOR
	bool	LoadSec(const char * szPath);
    CSecGraph  * GetSecGraph() const
	{
		 return m_pSecGraph;
	}
	bool  CreateSec();
#endif

	bool LineTo(const APointF& ptFrom, const APointF& ptTo);
	void Release();
private:

private:
	CBitImage			* m_pRMap;
	CBlockImage<FIX16>  * m_pDHMap;
#ifdef SVRPF_EDITOR
     CSecGraph          * m_pSecGraph;
#endif

};

}

 
#endif

