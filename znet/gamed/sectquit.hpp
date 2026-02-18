
#ifndef __GNET_SECTQUIT_HPP
#define __GNET_SECTQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


void sect_update_skill(int roleid, const int *skill_list, int count);
void sect_del_member(int master, int disciple);
namespace GNET
{

class SectQuit : public GNET::Protocol
{
	#include "sectquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		sect_del_member(sectid, disciple);
		sect_update_skill(disciple, NULL, 0);
	}
};

};

#endif
