#ifndef __ONLINE_GAME_GS_PATHFINDING_H__
#define __ONLINE_GAME_GS_PATHFINDING_H__

#include <common/types.h>
#include <substance.h>
#include "a3dvector.h"
#include "MoveMap.h"
#include "../gs/terrain.h"
#include "../collision/traceman.h"

class CChaseAgent;
class CRambleAgent;
class CFleeAgent;

namespace path_finding
{
	//class PathfindMap;
	class PathfindMap : public SvrPF::CMoveMap
	{
		CTerrain _terrain;
	public:
		PathfindMap() {}
		float GetTerrainHeight(float x, float z) const
		{ 
			return _terrain.GetHeightAt(x,z); 
		} 
		CTerrain * GetTerrain() { return &_terrain; }
		bool InitTerrain(const _TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax)
		{
			return _terrain.Init(config, xmin, zmin, xmax, zmax);
		}
	};
	//PathfindMap * InitMoveMap(const char * ground_map_path , const char * water_map_path, const char * air_map, CTerrain** t);
	PathfindMap * InitMoveMap(const char * ground_map_path , const char * water_map_path, const char * air_map);
	bool GetValidPos(PathfindMap * , A3DVECTOR & pos);	//传入地面上的坐标点
	void GetKnockBackPos(PathfindMap * ,  const A3DVECTOR & start, A3DVECTOR & end,int ienv);
	float GetWaterHeight(PathfindMap * ,float x, float z);
	bool InitTerrain(PathfindMap *, const _TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax);

//	typedef CChaseInfo chase_info;
	class follow_target: public substance 
	{
		protected:
			CChaseAgent* _agent;
			A3DVECTOR _target;
			int  _nodes_count;
			bool _is_blocked;
			bool _is_found;
			static int _detail_param[3][4];
		public:
			follow_target():_agent(0) {}

			// Youshuang change to virtual functions
			virtual ~follow_target();
			virtual void CreateAgent(PathfindMap * , int iMoveEnv);
			virtual void Start(const A3DVECTOR & source, const A3DVECTOR & target, float speed, float range,float cur_distance/*, chase_info * pInfo*/);
			virtual void Search();
			virtual bool GetToGoal() const;
			virtual void GetCurPos(A3DVECTOR & pos);
			virtual bool MoveOneStep(float speed);
			// end

			bool IsFailed() const { return _is_blocked;}
			bool IsSearching() {return !_is_found; }	//替换原来的IsBlocked
			const A3DVECTOR & GetTarget() { return _target; }
			bool PathIsFound() const { return _is_found; }
	};

	// Youshuang add
	class follow_target_without_map : public follow_target
	{
		private:
			PathfindMap* _movemap;
			trace_manager* _traceman;
			A3DVECTOR3 _curpos;
			A3DVECTOR3 _nextpos;
			A3DVECTOR3 _goal;
			float _range2;
			float _speed;
			
		public:
			follow_target_without_map( trace_manager* traceman )
			{
				_traceman = traceman;
			}

			virtual ~follow_target_without_map(){}
			virtual void CreateAgent(PathfindMap* move_map, int iMoveEnv)
			{  
				_movemap = move_map; 
			}
			virtual void Start(const A3DVECTOR & source, const A3DVECTOR & target, float speed, float range,float cur_distance);
			virtual void Search();
			virtual bool GetToGoal() const;
			virtual void GetCurPos(A3DVECTOR & pos);
			virtual bool MoveOneStep(float speed);

		private:
			bool IsPassable( const A3DVECTOR3& apoint ) const;
			void MoveOn();
	};
	// end

	class cruise : public substance 
	{
		protected:
			CRambleAgent * _agent;
		public:
			cruise():_agent(0)
			{}

			~cruise();
			void CreateAgent(PathfindMap * , int iMoveEnv);
			void Start(const A3DVECTOR & source, const A3DVECTOR & center, float speed, float range);
			bool GetToGoal();
			void GetCurPos(A3DVECTOR & pos);
			bool MoveOneStep(float speed);
	};

	class keep_out : public substance
	{
		protected:
			CFleeAgent * _agent;
		public:
			keep_out():_agent(0)
			{}

			virtual ~keep_out();
			virtual void CreateAgent(PathfindMap * , int iMoveEnv);
			virtual void Start(const A3DVECTOR & source, const A3DVECTOR & center, float speed, float range);
			virtual bool GetToGoal();
			virtual void GetCurPos(A3DVECTOR & pos);
			virtual void SetFleePos(const A3DVECTOR & target, float range);
			virtual bool MoveOneStep(float speed);
	};

	// Youshuang add
	class keep_out_without_map : public keep_out
	{
		private:
			PathfindMap* _movemap;
			trace_manager* _traceman;
			A3DVECTOR3 _curpos;
			A3DVECTOR3 _nextpos;
			A3DVECTOR3 _goal;
			float _range2;
			float _speed;
			int  _nodes_count;
			bool _is_blocked;
			bool _is_found;
			
		public:
			keep_out_without_map( trace_manager* traceman )
			{
				_traceman = traceman;
			}

			virtual ~keep_out_without_map(){};
			virtual void CreateAgent(PathfindMap* move_map, int iMoveEnv)
			{ 
				_movemap = move_map; 
			}
			virtual void Start(const A3DVECTOR & source, const A3DVECTOR & target, float speed, float range);
			virtual bool GetToGoal();
			virtual void GetCurPos(A3DVECTOR & pos);
			virtual void SetFleePos(const A3DVECTOR & target, float range);
			virtual bool MoveOneStep(float speed);

		private:
			bool IsPassable( const A3DVECTOR3& apoint ) const;
			void MoveOn();
			void Search();
			void CalcFleePos( A3DVECTOR3& goal, float range );
	};
	// end
}

#endif

