
#ifndef __GNET_LINELIST_HPP
#define __GNET_LINELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "lineinfo"

namespace GNET
{

class LineList : public GNET::Protocol
{
	#include "linelist"

	//客户端主动取各条线的状态信息
	void Process(Manager *manager, Manager::Session::ID sid);
};

};

#endif
