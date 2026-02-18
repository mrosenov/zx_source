
#ifndef __GNET_GETCOUPONSREQ_HPP
#define __GNET_GETCOUPONSREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include <vector>

void get_coupons_req(int userid, int toaid, int tozoneid, int roleid, std::vector<int> & task_ids);
		
namespace GNET
{

class GetCouponsReq : public GNET::Protocol
{
	#include "getcouponsreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		get_coupons_req(userid, toaid, tozoneid, roleid, itemids);
	}
};

};

#endif
