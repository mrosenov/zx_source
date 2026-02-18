
#ifndef __GNET_FACMALLSYNC_HPP
#define __GNET_FACMALLSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void facbase_mall_sync( const std::map<int, std::set<int> >& malls );
namespace GNET
{

class FacMallSync : public GNET::Protocol
{
	#include "facmallsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		facbase_mall_sync( malls );
	}
};

};

#endif
