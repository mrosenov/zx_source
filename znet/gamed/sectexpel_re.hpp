
#ifndef __GNET_SECTEXPEL_RE_HPP
#define __GNET_SECTEXPEL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void sect_del_member(int master, int disciple);
namespace GNET
{

class SectExpel_Re : public GNET::Protocol
{
	#include "sectexpel_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		//师傅赶走徒弟
		//sect_id 师傅 role_id  若 sect_id == role_id 则表明是师傅
		//disciple 赶走谁
		if(retcode == 0)
		{
			sect_del_member(sectid, disciple);
		}
	}
};

};

#endif
