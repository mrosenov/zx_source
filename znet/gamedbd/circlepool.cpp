#include "circlepool.h"
#include "storagewdb.h"
namespace GNET
{
using namespace WDB;	
void CirclePool::GetCurrentState(CirclePoolState& state)
{
	state.reserve(GRADE_FOR_CIRCLEID_STORAGE);
	CirclePoolMap2::iterator im = circlepoolmap.begin(),ie=circlepoolmap.end();
	for(;im!=ie;++im)
	{
		state.push_back(im->second);
	/*DEBUG_PRINT("CirclePool::GetCurrentState node racegrade=%d cid=%d cname=%d",im->second.racegrade,\
			im->second.cur_id, im->second.cur_name);
	*/
	}
	state[GRADE_FOR_CIRCLEID_STORAGE-1].cur_id = current_circleid;
	DEBUG_PRINT("CirclePool::GetCurrentState cidnode racegrade=%d cid=%d",state[GRADE_FOR_CIRCLEID_STORAGE-1].racegrade,\
			state[GRADE_FOR_CIRCLEID_STORAGE-1].cur_id);
}

const unsigned int MemberLimitArray[9]={200,175,150,125,100,80,60,40,30};
//const unsigned int MemberLimitArray[9]={5,5,5,5,5,5,5,5,5};
unsigned int CirclePool::MemberLimit(unsigned char grade)
{
	return MemberLimitArray[grade -1];
}
void CirclePool::Poolinit()
{
	current_circleid = 1;
	//CirclePoolStateNode node;
	for(int i=MINGRADE;i<=GRADE_FOR_CIRCLEID_STORAGE;i++)
	{
		circlepoolmap[i]=CirclePoolStateNode(i);
		DEBUG_PRINT("CirclePool::Poolinit circlepoolmap node racegrade=%d",circlepoolmap[i].racegrade);
	}
	circlepoolmap[GRADE_FOR_CIRCLEID_STORAGE].cur_id = current_circleid;
}
void CirclePool::InitCurrentState()
{
	Marshal::OctetsStream key;
	try
	{
		StorageEnv::Storage * pcircle = StorageEnv::GetStorage("circle");

		StorageEnv::CommonTransaction txn;
		try{
		//	GCircleDB circle;
			DEBUG_PRINT("CirclePool::InitCurrentState");
			key << CIRCLEID_FOR_STATE_STORAGE;
		//	Marshal::OctetsStream(pcircle->find(key, txn)) >> circle;
		//	Marshal::OctetsStream stateos = circle.circlepool_state;
			CirclePoolState state;
			Marshal::OctetsStream stateos;
			if(pcircle->find(key, stateos,txn)==true)//use this find to avoid exception
				stateos >> state;
			if(state.size()==0)
			{
				DEBUG_PRINT("CirclePool::InitCurrentState Poolinit");
				Poolinit();
			}	
			else
			{
				for(CirclePoolState::iterator it = state.begin(),ie=state.end();it!=ie;++it)
				{
					circlepoolmap[it->racegrade] = *it;
				}
				current_circleid = circlepoolmap[GRADE_FOR_CIRCLEID_STORAGE].cur_id;
			
				DEBUG_PRINT("CirclePool::InitCurrentState init from db,currentid=%d",current_circleid);
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException e( DB_OLD_VERSION );
			txn.abort( e );
			throw e;
		}
	}
	catch ( DbException e )
	{
//		Log::log( LOG_ERR, "DBCircleJoin, grade=%d, roleid=%d what=%s.",arg->newgrade,arg->roleid,e.what());
	}
 	
}
};

