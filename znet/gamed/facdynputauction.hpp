
#ifndef __GNET_FACDYNPUTAUCTION_HPP
#define __GNET_FACDYNPUTAUCTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynPutAuction : public GNET::Protocol
{
	#include "facdynputauction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
