/********************************************************************
	created:	2006/11/16
	author:		kuiwu
	
	purpose:	little graph a*  (typically search space < 100*100)
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/
#ifndef _G_ASTAR_H_
#define _G_ASTAR_H_


#include <vector.h>
#include <hashtab.h>
#include <ABaseDef.h>
#include "AssertLowPriority.h"
using namespace abase;

class CBitImage;


namespace SvrPF
{

class CGGraph;
class CGNode;
class CGEdge;
class CGHeap;


class GSearchNode
{
public:
	GSearchNode(){};
	~GSearchNode(){};
	GSearchNode(const GSearchNode& rhs)
		:n(rhs.n), e(rhs.e), f(rhs.f)
	{

	}

	GSearchNode& operator=(const GSearchNode& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}
		f = rhs.f;
		n = rhs.n;
		e = rhs.e;
		return *this;
	}
	float GetCost() const
	{
		return f;
	}
	void SetCost(float cost)
	{
		f = cost;
	}
	CGNode * GetNode() const
	{
		return n;
	}
	void    SetNode(CGNode * node)
	{
		n = node;
	}
	CGEdge * GetEdge() const
	{
		return e;
	}
	void   SetEdge(CGEdge * edge)
	{
		 e = edge;
	}
private:
	CGNode * n;
	CGEdge * e;
	float f;   //g+h
};

class  GOpen
{
public:
	GOpen(){};
	virtual ~GOpen(){};
	virtual void Init() = 0;
	virtual void Clear() = 0;
	virtual bool Empty() const = 0;
	virtual void Push(const GSearchNode& node) = 0;
	virtual void PopMinCost(GSearchNode& node) = 0;
	virtual int  Find(CGNode * n) = 0;
	virtual int  GetSize() = 0;
	virtual	GSearchNode *   At(int index) = 0;
	virtual void  DecreaseKey(int index) = 0;
};

class GOpenArray:public GOpen
{
public:
	GOpenArray();
	virtual ~GOpenArray();
	virtual void Init();
	virtual void Clear();
	
	virtual bool Empty() const 
	{
		return (m_List.empty());
	}
	virtual	void Push(const GSearchNode& node);
	virtual void PopMinCost(GSearchNode& node);
	virtual void DecreaseKey(int index)
	{
		//do nothing
	}
	/*
	 *
	 * @desc :
	 * @param :     
	 * @return :  size of the list if not found
	 * @note:
	 * @todo:   
	 * @author: kuiwu [21/12/2005]
	 * @ref:
	 */
	virtual	int Find(CGNode * n);
	virtual int GetSize()
	{
		return m_List.size();
	}
	virtual GSearchNode * At(int index)
	{
		if(index >= 0 && index < (int)m_List.size())
		{
		    return &m_List[index];	
		}else
		{
			ASSERT_LOW_PRIORITY(0);
			return NULL;
		}
	}
private:
	vector<GSearchNode>    m_List;
};


class GClose
{
	typedef hashtab<GSearchNode, unsigned int, abase::_hash_function>  CloseList;
public:
	GClose()
		:m_List(128)
	{
	}
	~GClose()
	{
		Clear();
	}
	void Push(const GSearchNode& node);
	void        Clear()
	{
		m_List.clear();
	}
	
	GSearchNode * Find(CGNode * n);
	GSearchNode * Find(int key);
	void          Remove(CGNode * n);
private:
	CloseList      m_List;
};


class IGExpandLimit
{
public:
	virtual	bool Test(CGNode * n) = 0;
};


class CGAStar  
{
public:

	CGAStar();
	virtual ~CGAStar();

	/**
	 * \brief 
	 * \param[in]
	 * \param[out]
	 * \return  the path cost, -1.0 means no path
	 * \note
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 16/11/2006
	 * \see 
	 */
	//float Search(CGGraph * g, CGNode * from, CGNode * to, vector<CGNode*>& path);
	float Search(CGGraph * g, CGNode * from, CGNode * to, vector<CGEdge*>& path);
	//float Search(CBitImage * rmap, A3DPOINT2& from, A3DPOINT2& to, vector<A3DPOINT2>& path, IGExpandLimit * expandLimit = NULL);

	static float GetHeuristic(CGNode * start, CGNode * target);
	int GetStat()
	{
		return m_State;
	}
	void SetMaxExpand(int maxExpand)
	{
		m_MaxExpand = maxExpand;
	}
	int GetNodeExpanded() const
	{
		return m_NodesExpanded;
	}
private:
#if 0
	void	_RelaxEdge(CGHeap *nodeHeap, CGEdge *e, CGNode * source, CGNode * through, CGNode *dest);
	double  _GeneratePath(CGGraph * g, unsigned int dest, vector<CGNode*>& path);
#endif
	double  _GeneratePath(GClose& closelist, GSearchNode& dest, vector<CGEdge*>& path);


private:
	int   m_NodesExpanded;
	int   m_NodesTouched;
	int   m_State;
	int   m_MaxExpand;

};

}

#endif

