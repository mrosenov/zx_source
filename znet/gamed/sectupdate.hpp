
#ifndef __GNET_SECTUPDATE_HPP
#define __GNET_SECTUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gsectskill"

void sect_update_skill(int roleid, const int *skill_list, int count);
void sect_add_member(int master, int disciple);
namespace GNET
{

class SectUpdate : public GNET::Protocol
{
	#include "sectupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		sect_add_member(sectid, roleid);
		if(sectid == roleid) 
			return;
		std::vector<int> data;
		data.reserve(skills.size()* 2);
		for(size_t i = 0; i <  skills.size(); i ++)
		{
			data.push_back(skills[i].id);
			data.push_back(skills[i].level);
		}
		if(data.empty()) 
			return;
		sect_update_skill(roleid, &data[0], data.size());

	}
};

};

#endif
