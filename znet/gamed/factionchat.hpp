
#ifndef __GNET_FACTIONCHAT_HPP
#define __GNET_FACTIONCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

size_t handle_chatdata(int uid, const void * aux_data, size_t size, void * buffset);
namespace GNET
{

class FactionChat : public GNET::Protocol
{
	#include "factionchat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(data.size())
		{
			char buffer[1024];
			int size = handle_chatdata(src, data.begin(), data.size(), buffer , sizeof(buffer));
			data.clear();
			if(size > 0) data.insert(data.begin(), buffer, size);
		}
		GProviderClient::DispatchProtocol(0, *this);
	}
};

};

#endif
