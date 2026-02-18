
#ifndef __GNET_WORLDCHAT_HPP
#define __GNET_WORLDCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class WorldChat : public GNET::Protocol
{
	#include "worldchat"
	class DispatchData
	{
		const WorldChat* chatmsg;
		bool check_zone;
	public:
		DispatchData(const WorldChat* msg, bool check):chatmsg(msg),check_zone(check) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME && (!check_zone || pair.second.src_zoneid == chatmsg->sender_zoneid))
				GLinkServer::GetInstance()->Send(pair.second.sid,chatmsg);
		}	
	};
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm = GLinkServer::GetInstance();
		Thread::RWLock::RDScoped l(lsm->locker_map);
		/*this->sender_zoneid = 0;
		if (this->channel == GP_CHAT_WORLD || this->channel == GP_CHAT_ZONE)
		{
			RoleData * role = lsm->GetRoleInfo(roleid);
			if (role != NULL)
				this->sender_zoneid = role->src_zoneid;
		}
		*/
		bool check_zone = false;
		if (this->channel == GP_CHAT_ZONE)
		{
			if (this->sender_zoneid == 0)
				return;
			check_zone = true;
		}
		if ( lsm->roleinfomap.size() )
			std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this, check_zone));
	}
};

};

#endif
