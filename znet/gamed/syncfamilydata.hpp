
#ifndef __GNET_SYNCFAMILYDATA_HPP
#define __GNET_SYNCFAMILYDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfamilyskill"
namespace  GMSV
{

void OnSyncFamilyData(int roleid, int familyid, const int * list,size_t count);
}
namespace GNET
{

class SyncFamilyData : public GNET::Protocol
{
	#include "syncfamilydata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		std::vector<int> list;
		list.reserve(skills.size() * 2);
		for(size_t i = 0; i < skills.size(); i ++)
		{
			list.push_back(skills[i].id);
			list.push_back(skills[i].level);
		}
		GMSV::OnSyncFamilyData(roleid, familyid, &list[0],list.size());
	}
};

};

#endif
