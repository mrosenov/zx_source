#ifndef __GNET_MAPREWARDTYPE_H
#define __GNET_MAPREWARDTYPE_H
#include <map>
#include <mutex.h>
#include "macros.h" //for _REWARDTYPE_INVALID
namespace GNET
{
	class RewardType
	{
	public:	
		struct reward_t
		{
			int type;
			int params;
			reward_t() : type(_REWARDTYPE_INVALID),params(0) { }
			reward_t( int t,int p ) : type(t),params(p) { }
		};
	private:
		typedef std::map<int,reward_t> RewardMap;
		RewardType() : locker_("RewardType::m_mapReward_") { }
		static RewardType& _instance() {
			static RewardType instance;
			return instance;
		}
	public:	
		static void Insert(int userid,int rewardtype,int params=0) {
			Thread::Mutex::Scoped l(_instance().locker_);
			_instance().m_mapReward_[userid]= reward_t(rewardtype,params);
		}
		static reward_t GetRewardType( int userid ) {
			Thread::Mutex::Scoped l( _instance().locker_ );
			RewardMap::iterator it=_instance().m_mapReward_.find(userid);
			return ( it!=_instance().m_mapReward_.end() ) ? (*it).second : reward_t(_REWARDTYPE_INVALID,0);
		}
		static void Remove( int userid ) {
			Thread::Mutex::Scoped l(_instance().locker_);
			_instance().m_mapReward_.erase( userid );
		}
	private:
		Thread::Mutex locker_;
		RewardMap m_mapReward_;
	};
}
#endif
