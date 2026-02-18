
#ifndef __GNET_FACMALLCHANGE_HPP
#define __GNET_FACMALLCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void facbase_mall_change( int fid, const std::set<int>& mall_indexes );
namespace GNET
{

class FacMallChange : public GNET::Protocol
{
	#include "facmallchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		facbase_mall_change( fid, mall_indexes );
	}
};

};

#endif
