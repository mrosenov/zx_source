#ifndef __ONLINE_GAME_NETGAME_GS_PATH_MAN_H__
#define __ONLINE_GAME_NETGAME_GS_PATH_MAN_H__

#include <hashtab.h>
#include <common/types.h>
#include <a3dvector.h>
class path_manager
{
public:
	struct single_path
	{
		int id;
		abase::vector<A3DVECTOR> _point_list;
		bool _pathmap_avail;
	public:
		void Push(const A3DVECTOR3 & pos)
		{
			_point_list.push_back(A3DVECTOR(pos.x,pos.y,pos.z));
		}
		void GetFirstWayPoint(A3DVECTOR& pos)
		{
			pos = _point_list[0];
		}

		void GetWayPoint(int index, A3DVECTOR& pos)
		{
			pos = _point_list[index];
		}
		size_t GetWayPointCount() 
		{
			return _point_list.size();
		}
		bool IsPathmapAvail() const { return _pathmap_avail; }
	};
	typedef abase::hashtab<single_path *, int,abase::_hash_function> PATH_TABLE;
private:
	PATH_TABLE _path_tab;
public:
	bool Init(const char * filename);
	path_manager():_path_tab(500){}
	~path_manager();
	single_path * GetPath(int id)
	{
		single_path ** ppPath = _path_tab.nGet(id);
		if(ppPath) 
			return *ppPath;
		else
			return NULL;
	}
	void Release();
	
};
#endif

