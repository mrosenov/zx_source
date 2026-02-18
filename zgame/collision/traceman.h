#ifndef __ONLINE_GAME_GS_COLLISION_MAN_H__
#define __ONLINE_GAME_GS_COLLISION_MAN_H__

#include <common/types.h>

namespace SvrCD {
	class CBrushMan;
	class CNmdTree;
	class CSMTree;
	class CNmdChd;
}
class trace_manager
{
//	SvrCD::CBrushMan * _brush_man;
//	SvrCD::CBrushMan * _brush_man2;
//	CNmdTree * _nmd_tree;
	SvrCD::CBrushMan * _brush_man;
	SvrCD::CSMTree * _sm_tree;
	int _element_count;
	int _sm_ref;

	static SvrCD::CNmdChd *_nmd_element;

	bool attach;

public:
	trace_manager();
	~trace_manager();

	void Attach(trace_manager * rhs);

	bool Load(const char * filename);
	bool Valid() const { return _brush_man;}
	bool AABBTrace(const A3DVECTOR & start, const A3DVECTOR & offset, const A3DVECTOR & ext, bool & in_solid, float &ratio);

	
	void Release();
	static bool LoadElement(const char * filename);	//装载单独文件
	static void ReleaseElement();
	int RegisterElement(int tid, int mid, const A3DVECTOR & pos, float dir0, float dir, float up);
	void EnableElement(int cid, bool active);
	void Compare();

};
#endif

