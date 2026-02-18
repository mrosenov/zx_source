// GAStar.cpp: implementation of the CGAStar class.
//
//////////////////////////////////////////////////////////////////////

#include "GAStar.h"
#include "Graph.h"
#include "PfCommon.h"

//#include <AAssist.h>

//#include <windows.h>

namespace SvrPF
{

//////////////////////////////////////////////////////////////////////////
//class GOpenArray
GOpenArray::GOpenArray()
{

}

GOpenArray::~GOpenArray()
{
	Clear();
}

void GOpenArray::Clear()
{
	m_List.clear();
}

void  GOpenArray::Init()
{
	m_List.reserve(15);
}

void GOpenArray::Push(const GSearchNode& node)
{
	m_List.push_back(node);
}

int  GOpenArray::Find(CGNode * n)
{
	int i = 0;
	while (i < (int)m_List.size())
	{
		if (m_List[i].GetNode() == n)
		{
			break;
		}
		++i;
	}

	return i;
}

void GOpenArray::PopMinCost(GSearchNode& node)
{
	int iMin = 0;
	int i;
	for (i = 1; i < (int)m_List.size(); ++i)
	{
		//eliminate branches
		iMin = (m_List[i].GetCost() < m_List[iMin].GetCost())? (i):(iMin);
	}

	node = m_List[iMin];
	m_List[iMin] = m_List[m_List.size()-1];
	m_List.pop_back();
}

//////////////////////////////////////////////////////////////////////////
//GClose
void GClose::Push(const GSearchNode& node)
{
	int key = node.GetNode()->GetNum();
	m_List.put(key, node);
}

GSearchNode * GClose::Find(CGNode * n)
{
	return Find(n->GetNum());
}

GSearchNode * GClose::Find(int key)
{
	CloseList::pair_type pair = m_List.get(key);
	return (pair.second)? (pair.first) : (NULL);
}

void GClose::Remove(CGNode * n)
{
	m_List.erase(n->GetNum());
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGAStar::CGAStar()
:m_MaxExpand(3000)
{

}

CGAStar::~CGAStar()
{

}

#if 0
float CGAStar::Search(CBitImage * rmap, APointI& from, APointI& to, vector<APointI>& path, IGExpandLimit * expandLimit /* = NULL */)
{
	m_State = PF_STATE_SEARCHING;
	m_NodesExpanded = 0;
	m_NodesTouched = 0;
	path.clear();
	double cost = -1.0;

	if (!rmap)
	{
		ASSERT_LOW_PRIORITY(rmap);
		return cost;
	}

	if (from == to)
	{
		m_State = PF_STATE_FOUND;
		path.push_back(to);
		cost = 0.0;
		return (float)cost;
	}

	//reset map graph
	CMapGraph * mg = g_AutoMoveAgent.GetMoveMap()->GetMapGraph();
	mg->SetRMap(rmap);
	CGNode * node_from;
	CGNode * node_to;

	node_from = mg->GetNode(from);
	node_to   = mg->GetNode(to);


	CGHeap * openlist = new CGHeap(30);
	vector<CGNode*> closelist;
	CGNode *n;

	// label start node cost 0
	n = node_from;
	n->SetLabelF(CGNode::kTemporaryLabel, GetHeuristic(n, node_to));  // f= g+h, save f
	n->MarkEdge(NULL);

	while (m_State == PF_STATE_SEARCHING)
	{
		m_NodesExpanded++;
		if (m_MaxExpand > 0 && m_NodesExpanded > m_MaxExpand)
		{
			m_State = PF_STATE_EXCEED;
			break;
		}


		// move current node onto closed list
		// mark node with its location in the closed list
		closelist.push_back(n);
		n->key = closelist.size()-1;
		
		// iterate over all the neighbors
		int i;
		for (i = 0; i < PF_NEIGHBOR_COUNT; ++i)
		{
			APointI neighbor(n->GetMapCoord());
			neighbor.x += PF2D_NeighborD[i*2];
			neighbor.y += PF2D_NeighborD[i*2+1];

			if (!rmap->GetPixel(neighbor.x, neighbor.y))
			{
				continue;
			}

			CGNode * node_neighbor = mg->GetNode(neighbor);
			//check to expand
			if (expandLimit && !expandLimit->Test(node_neighbor))
			{
				continue;
			}
			
			CGEdge * e = mg->GetEdge(n->GetMapCoord(), neighbor, PF2D_NeighborCost[i]);
			// if it's on the open list, try to update the weight
			if (openlist->IsIn(node_neighbor))
			{
				_RelaxEdge(openlist, e, n, node_neighbor, node_to);
			}
			// if it's not on the open list, then add it to the open list
			else if ((node_neighbor->key >= closelist.size()) 
					|| (closelist[node_neighbor->key] != node_neighbor))
			{
				node_neighbor->SetLabelF(CGNode::kTemporaryLabel, GRP_MAXINT);
				node_neighbor->SetKeyLabel(CGNode::kTemporaryLabel);
				node_neighbor->MarkEdge(NULL);
				openlist->Add(node_neighbor);
				_RelaxEdge(openlist, e, n, node_neighbor, node_to);				
			}
		}

		// get the next (the best) node off the open list
		n = openlist->Remove();
		
		// this means we have expanded all reachable nodes and there is no path
		if (!n)
		{
			m_State = PF_STATE_NOPATH;
		}
				
		if (n == node_to) 
		{
			m_State = PF_STATE_FOUND;	 // we found the goal
		}
	}

	delete openlist;
	
	if (m_State == PF_STATE_FOUND)
	{
		vector<CGNode*> node_path;
		cost = _GeneratePath(mg->GetGraph(), n->GetNum(), node_path);
		int i;
		for (i = 0; i < (int)node_path.size(); ++i)
		{
			path.push_back(node_path[i]->GetMapCoord());
		}
	}

	return (float)cost;
}


float CGAStar::Search(CGGraph * g, CGNode * from, CGNode * to, vector<CGNode*>& path)
{
	m_State = PF_STATE_SEARCHING;
	m_NodesExpanded = 0;
	m_NodesTouched = 0;
	path.clear();
	double cost = -1.0;

	if (!g || !from || !to)
	{
		ASSERT_LOW_PRIORITY(0);
		return cost;
	}
	if (from == to)
	{
		m_State = PF_STATE_FOUND;
		path.push_back(to);
		cost = 0.0;
		return (float)cost;
	}

	CGHeap * openlist = new CGHeap(30);
	vector<CGNode*> closelist;
	CGNode *n;


	// label start node cost 0
	n = from;
	n->SetLabelF(CGNode::kTemporaryLabel, GetHeuristic(n, to));  // f= g+h, save f
	n->MarkEdge(NULL);

	while (m_State == PF_STATE_SEARCHING)
	{
		m_NodesExpanded++;
		if (m_MaxExpand > 0 && m_NodesExpanded > m_MaxExpand)
		{
			m_State = PF_STATE_EXCEED;
			break;
		}

		// move current node onto closed list
		// mark node with its location in the closed list
		closelist.push_back(n);
		n->key = closelist.size()-1;
		
		vector<CGEdge*>* edges = n->GetAllEdges();
		vector<CGEdge*>::iterator eit;		
		// iterate over all the children
		for (eit = edges->begin(); eit != edges->end(); ++eit)
		{
			CGEdge * e = (*eit);
			m_NodesTouched++;
			unsigned int which;
			which =  (e->GetFrom() == n->GetNum())? (e->GetTo()) : (e->GetFrom());
			CGNode * nextChild = g->GetNode(which);	

			// if it's on the open list, try to update the weight
			if (openlist->IsIn(nextChild))
			{
				_RelaxEdge(openlist, e, n, nextChild, to);
			}
			// if it's not on the open list, then add it to the open list
			else if ((nextChild->key >= closelist.size()) 
					|| (closelist[nextChild->key] != nextChild))
			{
				nextChild->SetLabelF(CGNode::kTemporaryLabel, GRP_MAXINT);
				nextChild->SetKeyLabel(CGNode::kTemporaryLabel);
				nextChild->MarkEdge(NULL);
				openlist->Add(nextChild);
				_RelaxEdge(openlist, e, n, nextChild, to);				
			}
		}
		
		// get the next (the best) node off the open list
		n = openlist->Remove();
		
		// this means we have expanded all reachable nodes and there is no path
		if (!n)
		{
			m_State = PF_STATE_NOPATH;
		}
				
		if (n == to) 
		{
			m_State = PF_STATE_FOUND;	 // we found the goal
		}
	}

	delete openlist;
	
	if (m_State == PF_STATE_FOUND)
	{
		cost = _GeneratePath(g, n->GetNum(), path);
	}

	return (float)cost;

}
#endif

float CGAStar::Search(CGGraph * g, CGNode * from, CGNode * to, vector<CGEdge*>& path)
{
	m_State = PF_STATE_SEARCHING;
	m_NodesExpanded = 0;
	m_NodesTouched = 0;
	path.clear();
	double cost = -1.0;

	if (!g || !from || !to)
	{
		ASSERT_LOW_PRIORITY(0);
		return cost;
	}
	if (from == to)
	{
		m_State = PF_STATE_FOUND;
		cost = 0.0;
		return (float)cost;
	}

	GOpen * openlist = new GOpenArray();
	GClose  closelist;

	openlist->Init();
	GSearchNode curNode;
	curNode.SetNode(from);
	curNode.SetEdge(NULL);
	curNode.SetCost(GetHeuristic(from, to));
	openlist->Push(curNode);

	GSearchNode * pTmpNode;
	GSearchNode   nextNode;

	
	while ((m_State == PF_STATE_SEARCHING) && (!openlist->Empty()))
	{
		m_NodesExpanded++;
		if (m_MaxExpand > 0 && m_NodesExpanded > m_MaxExpand)
		{
			m_State = PF_STATE_EXCEED;
			break;
		}
		openlist->PopMinCost(curNode);
		

		if (curNode.GetNode() == to)
		{
			m_State = PF_STATE_FOUND;
			closelist.Push(curNode);
			break;
		}
			
		CGNode * n = curNode.GetNode();
		vector<CGEdge*>* edges = n->GetAllEdges();
		vector<CGEdge*>::iterator eit;		
		// iterate over all the children
		for (eit = edges->begin(); eit != edges->end(); ++eit)
		{
			CGEdge * e = (*eit);
			m_NodesTouched++;
			int which;
			which =  (e->GetFrom() == n->GetNum())? (e->GetTo()) : (e->GetFrom());
			CGNode * nextChild = g->GetNode(which);	

			nextNode.SetNode(nextChild);
			nextNode.SetEdge(e);
			nextNode.SetCost(curNode.GetCost() + e->GetWeight() + GetHeuristic(nextChild, to));
			// if it's on the open list, try to update the weight
			int open_index = openlist->Find(nextChild);
			if (open_index != openlist->GetSize())
			{
				//in open
				//see if update the node in open list
				pTmpNode = openlist->At(open_index);
				if (!pTmpNode)
				{
					//this should not happen.
					return cost;
				}
				if (pTmpNode->GetCost() > nextNode.GetCost())
				{

					*pTmpNode = nextNode;
					openlist->DecreaseKey(open_index);

				}
			}
			else if ((pTmpNode = closelist.Find(nextChild)))
			{
				//in close
				if (pTmpNode->GetCost() > nextNode.GetCost())
				{

					//move the node from close to open
					closelist.Remove(nextChild);
					openlist->Push(nextNode);

				}
			}
			else
			{
				//not in open nor close
				openlist->Push(nextNode);

			}
		}

		
		//add to close
		closelist.Push(curNode);
	}

	if (m_State == PF_STATE_SEARCHING && openlist->Empty())
	{
		m_State = PF_STATE_NOPATH;
	}

	delete openlist;
	
	if (m_State == PF_STATE_FOUND)
	{
		cost = _GeneratePath(closelist, curNode, path);
	}

//	printf("m_NodesExpanded = %d\n", m_NodesExpanded);
	return (float)cost;

}


float CGAStar::GetHeuristic(CGNode * start, CGNode * target)
{
	APointI ptStart(start->GetMapCoord());
	APointI ptTarget(target->GetMapCoord());
	int diffX = abs(ptStart.x - ptTarget.x);
	int diffZ = abs(ptStart.y - ptTarget.y);
	int diagonal = (diffX > diffZ)? (diffZ):(diffX);
	int straight = (diffX > diffZ)? (diffX):(diffZ);
	straight -= diagonal;
	//int diagonal = a_Min(diffX, diffZ);
	//int straight = a_Max(diffX, diffZ) - diagonal;

	return (float)((float)straight + diagonal * PF_ROOT_TWO);

}

#if 0
void CGAStar::_RelaxEdge(CGHeap *nodeHeap, CGEdge *e, CGNode * source, CGNode * through, CGNode *dest)
{
	double weight;
	
	weight = source->GetLabelF(CGNode::kTemporaryLabel) - GetHeuristic(source, dest)  + 
		      GetHeuristic(through, dest) + e->GetWeight();
	//if (weight < through->GetLabelF(CGNode::kTemporaryLabel))
	if (fless(weight, through->GetLabelF(CGNode::kTemporaryLabel)))
	{
#ifdef LOCAL_DEBUG
// 		AString str;
// 		str.Format("update %d parent %d, weight %5.4f from %5.4f\n", through->GetNum(), source->GetNum(), weight, through->GetLabelF(CGNode::kTemporaryLabel));
// 		OutputDebugStringA(str);
#endif

		through->SetLabelF(CGNode::kTemporaryLabel, weight);
		nodeHeap->DecreaseKey(through);
		// this is the edge used to get to this node in the min. path tree
		through->MarkEdge(e);
	}
}

double CGAStar::_GeneratePath(CGGraph * g, unsigned int dest, vector<CGNode*>& path)
{
	vector<CGNode*> rPath;
	
	unsigned int current = dest;
	double cost = 0.0;
	CGEdge *e;
	// extract best path from graph -- each node has a single parent in the graph which is the marked edge


	//////////////////////////////////////////////////////////////////////////
	//buggy
	while ((e = g->GetNode(current)->GetMarkedEdge()))
	{
		cost += e->GetWeight();
		rPath.push_back(g->GetNode(current));
		current = (e->GetFrom() == current)? (e->GetTo()):(e->GetFrom());	
	}
	rPath.push_back(g->GetNode(current));

	//reverse
	path.clear();
	int i = (int)rPath.size()-1;
	while (i >= 0)
	{
		path.push_back(rPath[i]);
		i--;
	}

	return cost;
}
#endif

double CGAStar::_GeneratePath(GClose& closelist, GSearchNode& dest, vector<CGEdge*>& path)
{
	vector<CGEdge*> rPath;
	
	GSearchNode * pCur = &dest;
	double cost = 0.0;
	CGEdge *e;
	// extract best path from graph -- each node has a single parent in the graph which is the marked edge

	//////////////////////////////////////////////////////////////////////////
	//buggy
	while (pCur && (e = pCur->GetEdge()))
	{
		cost += e->GetWeight();
		rPath.push_back(e);
		int current  = (e->GetFrom() == pCur->GetNode()->GetNum())? (e->GetTo()):(e->GetFrom());	
		pCur = closelist.Find(current);
	}


	//reverse
	path.clear();
	int i = (int)rPath.size()-1;
	while (i >= 0)
	{
		path.push_back(rPath[i]);
		i--;
	}

	return cost;
}

}

#undef  LOCAL_DEBUG
