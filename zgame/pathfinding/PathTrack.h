/********************************************************************
	created:	2008/7/30
	author:		liudong
	
	purpose:	路径的轨迹。
	            路径中两个相邻点之间的附加数据都跟随后一个点。比如layer:点3的layer是0，点4的是1，则3和4之间路径的layer都是4.
				对于joint点，就是连接两个layer的同一个点，必须把两个同样位置不同layer的点都加到路径中。
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _PF_PATH_TRACK_H_ 
#define _PF_PATH_TRACK_H_

#include <vector.h>
#include <APoint.h>

#define REPULSION_DISTANCE 0.03f //小于这个距离的两个点可以合并为一个点.

class IFile;

namespace SvrPF
{
class CPathOptimize;
struct PathNode;
	
class CPathTrack
{
public:
	CPathTrack();
	~CPathTrack();
	CPathTrack(const CPathTrack& target);
	CPathTrack& operator=(const CPathTrack& target);
	CPathTrack * Clone() const;
	void Release();
	
	//根据原始路径中所有的点获取压缩路径，这里生成的压缩路径仅仅保存8个基本方向上的线段。
	//pathOri     : 原始路径中的所有点。
	bool Init(abase::vector<APointI>& pathOri, int iLayer);
	bool Init(abase::vector<PathNode>& path);
	void SetLayer(int iLayer);

	//获取路径中fStep长度后的一个点 .如果该点在第二个关键点之后，返回第二个关键点。
	//因此，获取下一个点到当前位置的距离可能小于 fStep。
	//fStep<=0: 获取下一个关键点。
	//fStep 返回下一个点到当前点的真实距离。
	bool GetNextPos(PathNode& node,float& fStep);

	//沿着路径从当前点向前走fStep长度，设置当前点为新到达的点，删除已经走过的路径。
	//如果路径的长度没有fStep那么长，走完整条路径即可,走完全程后，路径中保留最后一个点而不是一个点都没有了。
	//本函数相当于 GetNextPos 与 SetCurPos 两个函数的组合。
	//node  : [out] 返回新路径的起始点。
	//fStep : [in]  传入希望走的长度, >0.0f。
	//return: 返回实际走的长度,如果走路失败返回值<=0.0f。
	float GoAcrossPath(PathNode& node,const float fStep);

	//用pos代替当前路径中的第一个关键点。(用户走到了第一个关键点和第二个关键点之间)
	//如果pos与路径中第二个关键点非常近，删除第一个关键点即可。(用户走到了第二个关键点)
	bool SetCurPos(PathNode& node);
	bool GetCurPos(PathNode& node);
	bool GetEndPos(PathNode& node);
	
	bool ModifyPos(const PathNode& node, const unsigned int index);

	//node  : [out] 传出点。
	//index : [in]  传入点的索引。
	//return: 该点是否存在。
	bool GetNode(PathNode& node, const unsigned int index);

	//获取路径中拐点数量。
	unsigned int GetNodeCount();

	//获取路径长度.
	float GetPathLen();

	//about file.
	bool Save(IFile& iFile);
	bool Load(IFile& iFile);
protected:
	//可以改成数组，以节省内存。
	abase::vector<PathNode> m_Path;

	friend class CPathOptimize;
	friend bool g_PF_AppendPath(CPathTrack& pathDest,const CPathTrack& pathSource);
};

//将两条路径连接在一起。
bool g_PF_AppendPath(CPathTrack& pathDest,const CPathTrack& pathSource);

}//end of namespace

#endif//_PF_PATH_TRACK_H_



