#ifndef __ONLINE_GAME_GS_PATROL_AGENT_IMP_H__
#define __ONLINE_GAME_GS_PATROL_AGENT_IMP_H__

#include "config.h"
#include "aipolicy.h"
#include "worldmanager.h"

class base_patrol_agent : public patrol_agent
{
protected:
	int _end_flag;
	int _index;
	int _type;		// 0 ²»Ñ­»· 1 Íù¸´ 2 Ñ­»·
	int _path_id;
	path_manager::single_path * _path;
public:
	DECLARE_SUBSTANCE(base_patrol_agent);
	base_patrol_agent():_index(0),_type(2),_path(0){}
	virtual bool Init(int path_id, world_manager* manager,int path_type)
	{
		_path = manager->GetPathMan()->GetPath(path_id); 
		_type = path_type;
		_path_id = path_id;
		if(!_path) return false;
		_index = 0;
		_end_flag = false;
		return true;
	}

	virtual int GetPathID() 
	{
		return _path_id;
	}

	virtual bool IsPathmapAvail() const
	{
		return _path->IsPathmapAvail();
	}

	virtual bool Reset()
	{
		_index = 0;
		_end_flag = false;
		return true;
	}

	virtual bool GetFirstWayPoint(A3DVECTOR & pos) 
	{
		_path->GetFirstWayPoint(pos);
		return true;
	}

	virtual bool GetNextWayPoint(A3DVECTOR & pos,bool &first_end)
	{
		bool need_end = first_end;
		first_end = false;
		if(_index >= (int)_path->GetWayPointCount())
		{
			switch(_type)
			{
				case 0:
				_index --;
				first_end = !_end_flag;
				if(need_end) _end_flag = true;
				break;

				case 1:
				case 2:
				default:
				_index = 0;
			}
		}
		_path->GetWayPoint(_index,pos);
		_index ++;
		return true;
	}

	virtual bool GetCurWayPoint(A3DVECTOR & pos)
	{
		if(_index  > 0)
		{
			_path->GetWayPoint(_index - 1,pos);
		}
		else
		{
			_path->GetWayPoint(0,pos);
		}
		return true;
	}

	virtual bool Save(archive & ar)
	{
		ASSERT(false);
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ASSERT(false);
		return true;
	}
};

#endif
