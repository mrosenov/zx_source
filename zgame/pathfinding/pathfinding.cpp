#include "pathfinding.h"
#include "ChaseAgent.h"
#include "RambleAgent.h"
#include "FleeAgent.h"
#include "PfCommon.h"
#include <map>
#include <string>


namespace path_finding
{
bool InitTerrain(PathfindMap * path, const _TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax)
{
	return path->InitTerrain(config, xmin, zmin, xmax, zmax);
}

PathfindMap * InitMoveMap(const char * ground_map_path , const char * water_map_path, const char * air_map)
{
	PathfindMap * path = new PathfindMap();
	if(!path->Load(ground_map_path)) 
	{
		delete path;
		return NULL;
	}
	if(!path->LoadWaterAreaMap(water_map_path))
	{
		delete path;
		return NULL;
	}
	return path;
}

float GetWaterHeight(PathfindMap * map, float x, float z)
{
	//return map->GetWaterHeight(x,z);
	return map->GetWaterHeight(A3DVECTOR3(x,0.f,z));
	//	return 0;
}

bool GetValidPos(PathfindMap * map, A3DVECTOR & pos)
{
	A3DVECTOR3 vPos(pos.x,pos.y,pos.z);
	if(!map->IsPassable(vPos)) return false;
	A3DVECTOR3 newpos = map->GetSurfacePos(vPos);
	pos.y = newpos.y;
	return true;
	/*
	   if(manager->GetMoveMap()->GetGroundValid3DPos(vPos))
	   {
	   pos.y = vPos.y;
	   return true;
	   }
	   else
	   {
	   return false;
	   }
	 */
}

void GetKnockBackPos(PathfindMap * map, const A3DVECTOR & start, A3DVECTOR & end, int ienv)
{
	/*
	   A3DVECTOR3 vStart(start.x,start.y,start.z);
	   A3DVECTOR3 vEnd(end.x,end.y,end.z);
	   A3DVECTOR3 vStopPos;
	   switch(ienv)
	   {
	   case NPC_MOVE_ENV_ON_GROUND:
	   manager->GetMoveMap()->GetGroundLastReachablePos(vStart,vEnd,vStopPos);
	   end.x = vStopPos.x;
	   end.y = vStopPos.y;
	   end.z = vStopPos.z;
	   break;
	   }
	 */
}


follow_target::~follow_target()
{
	if(_agent) delete _agent;
}

void 
	follow_target::CreateAgent(PathfindMap * pMap, int iMoveEnv)
	{
		_agent = new CChaseAgent(pMap);
	}

void 
	follow_target::Start(const A3DVECTOR & source, const A3DVECTOR & target, float speed, float range,float cur_distance/*, chase_info * pInfo */)
	{
		_nodes_count = 0;
		_is_found = false;
		_target = target;
		_is_blocked = false;

		A3DVECTOR3 initPos(source.x,source.y,source.z);
		_agent->Init(initPos);           						// 当前位置
		if(_agent->SetGoal(A3DVECTOR3(target.x,target.y,target.z), range,false))
		{
			return ;
		}
		_is_blocked = true;
	}

void 
	follow_target::Search()
	{
		int rst = _agent->Search(500);
		if(rst != PF_STATE_SEARCHING && rst != PF_STATE_FOUND) 
		{
			_is_blocked = true;
			_is_found = false;
			return ;
		}
		if(rst == PF_STATE_FOUND) 
		{
			_is_found = true;
			return ;
		}
		_nodes_count += 500;
		if(_nodes_count > 5000) 
		{
			_is_blocked = true;
		}
	}

void 
	follow_target::GetCurPos(A3DVECTOR & pos)
	{
		A3DVECTOR3 cp = _agent->GetCurPos();
		pos.x = cp.x;
		pos.y = cp.y;
		pos.z = cp.z;
	}

bool follow_target::MoveOneStep(float speed)
{
	if(!IsFailed()) 
	{
		Search();
		_agent->SetMoveStep(speed);
		_agent->MoveOn();
	}

	if(!PathIsFound() && IsFailed())  return false;
	return true;
}

bool follow_target::GetToGoal() const { return _agent->TouchGoal(false); } 


// Youshuang add
static bool CanPass( PathfindMap* movemap, trace_manager* traceman, const A3DVECTOR3& apoint )
{
	if( !traceman || !traceman->Valid() ){ return false; }
	float dH = movemap->GetTerrainHeight( apoint.x, apoint.z );
	if( apoint.y < dH + 0.2 ){ return false; }
	
	bool is_solid;
	float ratio;
	bool bRst = traceman->AABBTrace( A3DVECTOR( apoint.x, apoint.y, apoint.z ), A3DVECTOR( 0,-10,0 ), A3DVECTOR( 0.3,0.9,0.3 ), is_solid, ratio );
	if( bRst && is_solid ){ return false; }
	//return movemap->IsPassable( i_inmap, layer );
	return true;
}

static void SearchNextPos( PathfindMap* movemap, trace_manager* traceman, const A3DVECTOR3& curpos, const A3DVECTOR3& goal, 
		float speed, float range2, A3DVECTOR3& nextpos, bool& is_found, bool& is_blocked, int& nodes_count )
{
        A3DVECTOR3 diff = goal - curpos;
        A3DVECTOR3 diff_normalized = diff;
	float magnitude = diff_normalized.Normalize();
	if( magnitude < 1e-6 && magnitude > -1e-6 )
	{
		return;
	}

	float dist = diff.Magnitude();
	if( speed < dist )
	{
		dist = speed;
	}
	A3DVECTOR3 predict = curpos;
	int i = 0;
	
	int interval_points_cnt = (int)( dist / 0.2f );
	for( ; i < interval_points_cnt; ++i  )
	{
		predict = curpos + diff_normalized * 0.2f * ( i + 1 );
		if( !CanPass( movemap, traceman, predict ) )
		{
			is_blocked = true;
			nextpos = curpos + diff_normalized * 0.2f * i;
			if( i == 0 )
			{
				is_found = false;
			}
			return;
		}
		is_found = true;
		nextpos = predict;
		diff = goal - nextpos;
		if( diff.SquaredMagnitude() < range2 ){ return; }
	}
	
	if( dist - interval_points_cnt * 0.2f > 1e-3 )
	{
		predict = curpos + diff_normalized * dist;
		if( CanPass( movemap, traceman, predict ) )
		{
			nextpos = predict;
			is_found = true;
		}
		else
		{
			is_found = ( i != 0 );
			is_blocked = true;
		}
	}
	
	nodes_count += 500;
	if(nodes_count > 5000) 
	{
		is_blocked = true;
	}
}

bool follow_target_without_map::IsPassable( const A3DVECTOR3& apoint ) const
{
	return CanPass( _movemap, _traceman, apoint );
}

void follow_target_without_map::Start(const A3DVECTOR & source, const A3DVECTOR & target, float speed, float range,float cur_distance )
{
	_nodes_count = 0;
	_is_found = false;  // true when has movement before it is blocked
	_is_blocked = false;  // true when blocked in someplace
	_target = target;
	_curpos = A3DVECTOR3( source.x, source.y, source.z );
	_nextpos = _curpos;
	_goal = A3DVECTOR3( target.x, target.y, target.z );
	_range2 = range * range;
	_speed = speed;
	
	if( !IsPassable( _curpos ) || !IsPassable( _goal ) )
	{
		_is_blocked = true;
	}
}

void follow_target_without_map::Search()
{
	SearchNextPos( _movemap, _traceman, _curpos, _goal, _speed, _range2, _nextpos, _is_found, _is_blocked, _nodes_count );
}

void follow_target_without_map::GetCurPos(A3DVECTOR & pos)
{
	pos.x = _curpos.x;
	pos.y = _curpos.y;
	pos.z = _curpos.z;
}

void follow_target_without_map::MoveOn()
{
	_curpos = _nextpos;
}

bool follow_target_without_map::MoveOneStep( float speed )
{
	_speed = speed;
	if( !IsFailed() ) 
	{
		Search();
		MoveOn();
	}

	if( !PathIsFound() && IsFailed() )  return false;
	return true;
}

bool follow_target_without_map::GetToGoal() const
{ 
	const float tor = 0.1f;
        A3DVECTOR3  diff = _goal - _curpos;
        float diff_dist = diff.SquaredMagnitude();
        return diff_dist < ( _range2 + tor );
} 
// end

cruise::~cruise()
{
	if(_agent) delete _agent;
}

void cruise::CreateAgent(PathfindMap * pMap, int iMoveEnv)
{
	_agent = new CRambleAgent(pMap);
}

void cruise::Start(const A3DVECTOR & source, const A3DVECTOR & center, float speed, float range)
{
	A3DVECTOR3 initPos(source.x,source.y,source.z);
	A3DVECTOR3 cenPos(center.x,center.y,center.z);
	_agent->Init(initPos);           // 当前位置
	_agent->SetRambleInfo(cenPos, range);
	_agent->SetMoveStep(speed);
	_agent->StartRamble();
}

bool cruise::MoveOneStep(float speed)
{
	_agent->MoveOn();
	return true;
}

void cruise::GetCurPos(A3DVECTOR & pos)
{
	A3DVECTOR3  cp = _agent->GetCurPos();
	pos.x = cp.x;
	pos.y = cp.y;
	pos.z = cp.z;
}

bool cruise::GetToGoal()
{
	return _agent ->IsStopped();
}

keep_out::~keep_out()
{
	if(_agent) delete _agent;
}

void 
	keep_out::CreateAgent(PathfindMap * pMap, int iMoveEnv)
	{
		_agent = new CFleeAgent(pMap);
	}

void 
	keep_out::Start(const A3DVECTOR & source, const A3DVECTOR & center, float speed, float range)
	{
		A3DVECTOR3 initPos(source.x,source.y,source.z);
		A3DVECTOR3 cenPos(center.x,center.y,center.z);
		_agent->Init(initPos);           // 当前位置
		_agent->SetFleePos(cenPos, range);
		_agent->SetMoveStep(speed);
		_agent->StartFlee();
	}

bool 
	keep_out::GetToGoal()
	{
		return _agent ->FleeSuccess();
	}

void 
	keep_out::GetCurPos(A3DVECTOR & pos)
	{
		A3DVECTOR3  cp = _agent->GetCurPos();
		pos.x = cp.x;
		pos.y = cp.y;
		pos.z = cp.z;
	}

void 
	keep_out::SetFleePos(const A3DVECTOR & target, float range)
	{
		A3DVECTOR3 fleePos(target.x,target.y,target.z);
		_agent->SetFleePos(fleePos, range);
	}

bool 
	keep_out::MoveOneStep(float speed)
	{
		_agent->MoveOn();
		return true;
	}

// Youshuang add
bool keep_out_without_map::IsPassable( const A3DVECTOR3& apoint ) const
{
	return CanPass( _movemap, _traceman, apoint );
}

void keep_out_without_map::CalcFleePos( A3DVECTOR3& goal, float range )
{
	float dir[4][2] = { {range,0},{-range,0},{0, range},{0,-range} };
	for( int i = 0; i < 4; ++i )
	{
		A3DVECTOR3 tmp = goal;
		tmp.x += dir[i][0];
		tmp.z += dir[i][1];
		if( IsPassable( tmp ) )
		{
			goal = tmp;
			break;
		}
	}
}

void keep_out_without_map::Start( const A3DVECTOR& source, const A3DVECTOR& target, float speed, float range )
{
	_curpos = A3DVECTOR3( source.x, source.y, source.z );
	_nextpos = _curpos;
	_goal = A3DVECTOR3( target.x, target.y, target.z );
	CalcFleePos( _goal, range );
	_range2 = range * range;
	_speed = speed;
}

bool keep_out_without_map::GetToGoal()
{
	const float tor = 0.1f;
        A3DVECTOR3  diff = _goal - _curpos;
        float diff_dist = diff.SquaredMagnitude();
        return diff_dist < ( _range2 + tor );
}

void keep_out_without_map::GetCurPos( A3DVECTOR& pos )
{
	pos.x = _curpos.x;
	pos.y = _curpos.y;
	pos.z = _curpos.z;
}

void keep_out_without_map::SetFleePos( const A3DVECTOR& target, float range )
{
	_goal = A3DVECTOR3( target.x, target.y, target.z );
	CalcFleePos( _goal, range );
	_range2 = range * range;
}

void keep_out_without_map::Search()
{
	SearchNextPos( _movemap, _traceman, _curpos, _goal, _speed, 0.1f, _nextpos, _is_found, _is_blocked, _nodes_count );
}

void keep_out_without_map::MoveOn()
{
	_curpos = _nextpos;
}

bool keep_out_without_map::MoveOneStep( float speed )
{
	Search();
	MoveOn();
	return true;
}
// end

}
