
#ifndef __GNET_PRIVATECHAT_HPP
#define __GNET_PRIVATECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

size_t handle_chatdata(int uid, const void * aux_data, size_t size, void * buffset, size_t len);
namespace GNET
{

class PrivateChat : public GNET::Protocol
{
	#include "privatechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(data.size())
		{ 
			char buffer[1024];
			int size = handle_chatdata(srcroleid, data.begin(), data.size(), buffer , sizeof(buffer));
			data.clear();
			if(size > 0) data.insert(data.begin(), buffer, size);
		}       
		GProviderClient::DispatchProtocol(0, *this);
	}
};

};

#endif
