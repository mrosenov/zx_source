// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "Graph.h"
#include <ABaseDef.h>
#include "AssertLowPriority.h"

namespace SvrPF
{


/************************************************************************/
/* graph                                                                */
/************************************************************************/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGGraph::CGGraph()
:m_Nodes(),m_Edges()
{

}

CGGraph::~CGGraph()
{

	Release();
}

void CGGraph::Release()
{
	vector<CGNode*>::iterator nit;
	for (nit = m_Nodes.begin(); nit != m_Nodes.end(); ++nit)
	{
		if (*nit)
		{
			delete (*nit);
		}
	}
	vector<CGEdge*>::iterator eit;
	for (eit = m_Edges.begin(); eit != m_Edges.end(); ++eit)
	{
		if (*eit)
		{
			delete (*eit);
		}
	}

	m_Nodes.clear();
	m_Edges.clear();

}

int CGGraph::AddNode(CGNode *n)
{
  if (n)
  {
	  m_Nodes.push_back(n);
	  n->m_NodeNum = m_Nodes.size() -1;
	  return n->m_NodeNum;
  }
  else
  {
	  ASSERT_LOW_PRIORITY(0);
	  return -1;
  }

}

void CGGraph::AddEdge(CGEdge * e)
{
	if (!e)
	{
		ASSERT_LOW_PRIORITY(0);
		return;
	}
    m_Edges.push_back(e);
    e->m_EdgeNum = m_Edges.size()-1;
    
	if (e->GetFrom() < (int)m_Nodes.size())
      m_Nodes[e->GetFrom()]->AddEdge(e);
    else
      ASSERT_LOW_PRIORITY(0);

    if (e->GetTo() < (int)m_Nodes.size())
      m_Nodes[e->GetTo()]->AddEdge(e);
    else
      ASSERT_LOW_PRIORITY(0);
}

CGGraph * CGGraph::Clone() const
{
	CGGraph * g = new CGGraph();
	vector<CGNode *>::const_iterator nit;
	for (nit = m_Nodes.begin(); nit!= m_Nodes.end(); ++nit)
	{
		CGNode * n = (*nit)->ModerateClone();
		g->AddNode(n);
	}
	vector<CGEdge *>::const_iterator eit;
	for (eit = m_Edges.begin(); eit != m_Edges.end(); ++eit)
	{
		CGEdge *e = (*eit)->ModerateClone();
		g->AddEdge(e);
	}
    return g;
}





void CGGraph::RemoveEdge(CGEdge *e)
{
	if (!e)
	{
		ASSERT_LOW_PRIORITY(0);
		return;
	}
	GetNode(e->GetTo())->RemoveEdge(e);
	GetNode(e->GetFrom())->RemoveEdge(e);

	unsigned int oldLoc = e->m_EdgeNum;
	if(m_Edges[oldLoc] != e)
	{
		ASSERT_LOW_PRIORITY(0);
		return;
	}
	CGEdge  *replacement = m_Edges.back();
	m_Edges[oldLoc] = replacement;
	replacement->m_EdgeNum = oldLoc;
	m_Edges.pop_back();

	//free the memory
	delete e;
}


// returns the node that had it's node number changed, if any
CGNode * CGGraph::RemoveNode(CGNode * n, int& oldid)
{
	if (!n)
	{
		ASSERT_LOW_PRIORITY(0);
		return NULL;
	}
	while (!n->m_AllEdges.empty())
	{
		CGEdge * e = n->m_AllEdges.back();
		RemoveEdge(e);
	}

	CGNode * tmp = m_Nodes.back();
	m_Nodes.pop_back();
	if (n== tmp)
	{
		delete n;
		return NULL;
	}


	if (!m_Nodes.empty())
	{
		//set n to tmp
		m_Nodes[n->GetNum()] = tmp;
		oldid = tmp->GetNum();  //tmp's old id
		tmp->m_NodeNum = n->GetNum();
	}
    // repair edges to and from  for tmp
 	unsigned int i;
	for (i = 0; i < tmp->m_AllEdges.size(); ++i)
	{
		if (tmp->m_AllEdges[i]->m_To == oldid)
		{
			tmp->m_AllEdges[i]->m_To = tmp->m_NodeNum;
		}
		else if (tmp->m_AllEdges[i]->m_From == oldid)
		{
			tmp->m_AllEdges[i]->m_From = tmp->m_NodeNum;
		}
	}

// 	for (i = 0; i < tmp->m_EdgesIncoming.size(); i++)
// 	{
// 		tmp->m_EdgesIncoming[i]->m_To = tmp->m_NodeNum;
// 	}
// 	for (i = 0; i < tmp->m_EdgesOutgoing.size(); i++)
// 	{
// 		tmp->m_EdgesOutgoing[i]->m_From = tmp->m_NodeNum;
// 	}
	
	delete n;
	return tmp;
	
}

#if 0
void CGGraph::Save(AFile& fileimg)
{
	unsigned int count;
	DWORD writelen;
	unsigned int i;

	count  = m_Nodes.size();
	fileimg.Write(&count, sizeof(unsigned int), &writelen);
	for (i = 0; i < count; i++)
	{
		CGNode * n = GetNode(i);
		//////////////////////////////////////////////////////////////////////////
		//labels
		//double dval;
		long   lval;
		lval = n->GetLabelL(CGNode::kAbstractionLevel);
		fileimg.Write(&lval, sizeof(long), &writelen);
		lval = n->GetLabelL(CGNode::kNumAbstractedNodes);
		fileimg.Write(&lval, sizeof(long), &writelen);
		lval = n->GetLabelL(CGNode::kParent);
		fileimg.Write(&lval, sizeof(long), &writelen);
		//skip temp
		lval = n->GetLabelL(CGNode::kXCoordinate);
		fileimg.Write(&lval, sizeof(long), &writelen);
		lval = n->GetLabelL(CGNode::kZCoordinate);
		fileimg.Write(&lval, sizeof(long), &writelen);
		//skip data

		//////////////////////////////////////////////////////////////////////////
		//all edge
// 		unsigned int eCount;
// 		eCount = n->m_AllEdges.size();
// 		fileimg.Write(&eCount, sizeof(unsigned int), &writelen);
// 		for (j = 0; j < eCount; j++ )
// 		{
// 			unsigned int eNum = n->m_AllEdges[j]->GetNum();
// 			fileimg.Write(&eNum, sizeof(unsigned int), &writelen);
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 		//in edge
// 		eCount = n->m_EdgesIncoming.size();
// 		fileimg.Write(&eCount, sizeof(unsigned int), &writelen);
// 		for (j = 0; j < eCount; j++ )
// 		{
// 			unsigned int eNum = n->m_EdgesIncoming[j]->GetNum();
// 			fileimg.Write(&eNum, sizeof(unsigned int), &writelen);
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 		//out edge
// 		eCount = n->m_EdgesOutgoing.size();
// 		fileimg.Write(&eCount, sizeof(unsigned int), &writelen);
// 		for (j = 0; j < eCount; j++ )
// 		{
// 			unsigned int eNum = n->m_EdgesOutgoing[j]->GetNum();
// 			fileimg.Write(&eNum, sizeof(unsigned int), &writelen);
// 		}
		
		//skip others
	}


	//edges
	count = m_Edges.size();
	fileimg.Write(&count, sizeof(unsigned int), &writelen);
	for (i = 0; i < count; i++)
	{
		CGEdge *e = m_Edges[i];
		unsigned int which;
		which = e->GetFrom();
		fileimg.Write(&which, sizeof(unsigned int), &writelen);
		which = e->GetTo();
		fileimg.Write(&which, sizeof(unsigned int), &writelen);

		double weight;
		weight = e->GetWeight();
		fileimg.Write(&weight, sizeof(double), &writelen);
	}


}

void CGGraph::Load(AFile& fileimg)
{
	unsigned int count;
	DWORD readlen;
	unsigned int i;

	Release();
	fileimg.Read(&count, sizeof(unsigned int), &readlen);
	for (i = 0; i < count; i++)
	{
		CGNode * n = new CGNode;
		AddNode(n);

		//////////////////////////////////////////////////////////////////////////
		//labels
		long   lval;
		fileimg.Read(&lval, sizeof(long), &readlen);
		n->SetLabelL(CGNode::kAbstractionLevel, lval);
		fileimg.Read(&lval, sizeof(long), &readlen);
		n->SetLabelL(CGNode::kNumAbstractedNodes, lval);
		fileimg.Read(&lval, sizeof(long), &readlen);
		n->SetLabelL(CGNode::kParent, lval);
		//skip temp
		fileimg.Read(&lval, sizeof(long), &readlen);
		n->SetLabelL(CGNode::kXCoordinate, lval);
		fileimg.Read(&lval, sizeof(long), &readlen);
		n->SetLabelL(CGNode::kZCoordinate, lval);
		//skip data
	}

	fileimg.Read(&count, sizeof(unsigned int), &readlen);
	for (i = 0; i < count; i++)
	{
		unsigned int from, to;
		double weight;
		fileimg.Read(&from, sizeof(unsigned int), &readlen);
		fileimg.Read(&to, sizeof(unsigned int), &readlen);
		fileimg.Read(&weight, sizeof(double), &readlen);

		CGEdge * e = new CGEdge(from, to, weight);
		AddEdge(e);
	}



}
#endif

/************************************************************************/
/* node                                                                 */
/************************************************************************/

CGNode::CGNode()
:m_Labels(), m_AllEdges()
{
	m_KeyLabel = 0;
}


void CGNode::SetLabelF(unsigned int index, double val)
{
  if (index < m_Labels.size())
    m_Labels[index].fval = val;
  else 
  {
    while (index > m_Labels.size())
	{
		labelValue v; 
		v.fval = (double)GRP_MAXINT;
	    m_Labels.push_back(v);
    }
	labelValue v; 
	v.fval = val;
    m_Labels.push_back(v);
  }
}

void CGNode::SetLabelL(unsigned int index, long val)
{
  if (index < m_Labels.size())
    m_Labels[index].lval = val;
  else 
  {
    while (index > m_Labels.size())
	{
		labelValue v; 
		v.lval = GRP_MAXINT;
		m_Labels.push_back(v);
    }
	labelValue v; 
	v.lval = val;
    m_Labels.push_back(v);
  }
}

void CGNode::AddEdge(CGEdge * e)
{
   if (!e)
   {
	   ASSERT_LOW_PRIORITY(0);
	   return ;
   }
   m_AllEdges.push_back(e);
//    if (e->GetFrom() == m_NodeNum)
//    {
// 	   m_EdgesOutgoing.push_back(e);
//    }
//    else if (e->GetTo() == m_NodeNum)
//    {
// 	   m_EdgesIncoming.push_back(e);
//    }
//    else
//    {
// 	   ASSERT_LOW_PRIORITY(0);
//    }
}


void CGNode::RemoveEdge(CGEdge *e)
{
	if (!e || (e->GetTo()!=m_NodeNum && e->GetFrom()!=m_NodeNum))
	{
		ASSERT_LOW_PRIORITY(0);
		return;
	}
	unsigned int i;
// 	if (m_NodeNum == e->GetTo())
// 	{
// 		for (i = 0; i < m_EdgesIncoming.size(); i++)
// 		{
// 			if (m_EdgesIncoming[i] == e)
// 			{
// 				m_EdgesIncoming[i] = m_EdgesIncoming.back();
// 				m_EdgesIncoming.pop_back();
// 				break;
// 			}
// 		}
// 	}
// 	else 
// 	{
// 	   for (i = 0; i < m_EdgesOutgoing.size(); i++)
// 	   {
// 		   if (m_EdgesOutgoing[i] == e)
// 		   {
// 			   m_EdgesOutgoing[i] = m_EdgesOutgoing.back();
// 			   m_EdgesOutgoing.pop_back();
// 			   break;
// 		   }
// 	   }
// 	}

	for (i = 0; i < m_AllEdges.size(); i++)
	{
		if (m_AllEdges[i] == e)
		{
			m_AllEdges[i] = m_AllEdges.back();
			m_AllEdges.pop_back();
			break;
		}
	}
}


CGNode * CGNode::ModerateClone()
{
	CGNode * n = new CGNode;
	for (unsigned int x = 0; x < m_Labels.size(); x++) 
		n->m_Labels.push_back(m_Labels[x]);
	n->m_KeyLabel =  m_KeyLabel;
	return n;
}

/************************************************************************/
/* edge                                                                 */
/************************************************************************/
CGEdge::CGEdge(int from, int to, double weight, int pathIndex)
: m_From(from), m_To(to)
{

	SetLabelF(kEdgeWeight, weight);
	SetLabelL(kEdgePath, pathIndex);
}

void CGEdge::SetLabelF(unsigned int index, double val)
{
  if (index < m_Labels.size())
    m_Labels[index].fval = val;
  else {
    while (index > m_Labels.size())
		{
			labelValue v; v.fval = (double)GRP_MAXINT;
			m_Labels.push_back(v);
    }
	labelValue v; 
	v.fval = val;
    m_Labels.push_back(v);
  }
}

void CGEdge::SetLabelL(unsigned int index, long val)
{
  if (index < m_Labels.size())
  {
    m_Labels[index].lval = val;
  }
  else 
  {
    while (index > m_Labels.size())
	{
		labelValue v; 
		v.lval = GRP_MAXINT;
		m_Labels.push_back(v);
    }
	labelValue v; 
	v.lval = val;
    m_Labels.push_back(v);
  }
}


}


