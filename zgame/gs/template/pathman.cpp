#include "pathman.h"
#include "sevbezier.h"


path_manager::~path_manager()
{
	Release();
}

bool
path_manager::Init(const char * filename)
{
	CSevBezierMan CBM;
	if(!CBM.Load(filename)) return false;
	typedef CSevBezierMan::BezierTable TABLE; 
	const TABLE & tab = CBM.GetTable();

	TABLE::const_iterator it = tab.begin();
	for(;it != tab.end(); ++it)
	{
		int id = *it.key();
		CSevBezier * pBezier = *it.value();
		CSevBezierWalker walker;
		walker.BindBezier(pBezier);
		walker.SetSpeed(8.0f);
		walker.StartWalk(false,true);
		single_path * pPath = new single_path();
		pPath->id = id;
		pPath->_pathmap_avail = pBezier->IsPathmapAvail();
		do{
			A3DVECTOR3 pos = walker.GetPos();
			pPath->Push(pos);
			walker.Tick(1000);
			A3DVECTOR3 pos2 = walker.GetPos();
			if(!walker.IsWalking() || (pos2-pos).Normalize() < 1e-3) break;
		}while(1);
		bool bRst = _path_tab.put(id,pPath);
		if(!bRst){
			printf("重复的路线ID被发现\n");
			return false;
		}
	}
	return true;
}

void path_manager::Release()
{
	PATH_TABLE::iterator it = _path_tab.begin();
	for(; it != _path_tab.end(); ++it)
	{
		single_path** path = it.value();
		delete *path;
	}
	_path_tab.clear();
}

