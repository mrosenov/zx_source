/********************************************************************
	created:	2006/05/14
	author:		kuiwu
	
	purpose:	map for pathfinding
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _MOVE_MAP_H_
#define _MOVE_MAP_H_


#include <a3dvector.h>
#include <APoint.h>
#include <stdio.h>
#include <vector.h>
using namespace abase;

#ifdef SVRPF_EDITOR
#include <AString.h>
#endif

#define	   PFMAP_CFG_FILE_MAGIC      (unsigned int)(('c'<<24)| ('f'<<16)|('g'<<8)|('f'))
#define    PFMAP_CFG_FILE_VERSION     1


class CWaterAreaMap;

namespace SvrPF
{



class CLayerMap;
class CMultiSecGraph;

class CMoveMap
{
public:
	CMoveMap();
	virtual ~CMoveMap();
	void Release();
 	bool Load(const char * szPath);

// 	bool Save(const char * szPath, const vector<AString>& layerNames, const AString& mluName);
	CLayerMap * CreateLayer();


	int	 GetLayerCount() const
	{
		return (int) m_aLayers.size();
	}
	CLayerMap * GetLayer(int index) const
	{
		if (index >= 0 && index < GetLayerCount())
		{
			return m_aLayers[index];
		}

		return NULL;
	}


	
	int      WhichLayer(const APointI& ptMap, float dH) const;
	

	int GetMapWidth() const
	{
		return m_iMapWidth;
	}

	int GetMapLength() const
	{
		return m_iMapLength;
	}

	CMultiSecGraph * GetMultiSecGraph() const
	{
		return m_pMultiSecGraph;
	}

	APointF TransMap2Wld(const APointI& ptMap) const;
	APointF TransMap2Wld(const APointF& ptMap) const;
	APointF TransWld2Map(const APointF& ptWld) const;
	void CalcOrigin();

	float GetDH(int iLayer, const APointI& ptMap) const;
	CMultiSecGraph * CreateMultiSecGraph();
	void  ReleaseMultiSecGraph();
//	APointF GetPixelCenter(const APointF& ptMap) const;

	bool  IsPassable(const APointI& ptMap, int iLayer) const;
	bool  GetPassablePosNearby(APointI& ptPassable, const APointF& curPos, const int iLayer, const int maxDistH) const;
	
	virtual float GetTerrainHeight(float x, float z) const = 0;
	//get the surface pos 
	A3DVECTOR3    GetSurfacePos(const A3DVECTOR3& vWld) const;
	A3DVECTOR3	  TransMap2Wld(const APointF& ptMap, int iLayer, bool bFixDH = false) const;
	//test if passable given world pos
	bool		  IsPassable(const A3DVECTOR3& vWld) const;	

	bool          LoadWaterAreaMap(const char * szPath);
	float		  GetWaterHeight(const A3DVECTOR3& vPos) const;

private:
	void _SetMapCenterAsOrigin();
private:

	// ^     
	// | |----------| 
	// | |			| 	
	// | |			| 
	// | |----------| 
	// |_ _ _ _ _ _> 
	A3DVECTOR3  m_vOrigin;   // origin of the world in the map coordinate system

	vector<CLayerMap *>  m_aLayers;

	int					 m_iMapWidth;
	int					 m_iMapLength;
	float				 m_fPixelSize;
	CMultiSecGraph	*    m_pMultiSecGraph;
	CWaterAreaMap	*    m_pWaterAreaMap;
};



};	// end of the namespace

#endif




