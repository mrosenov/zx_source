#include "types.h"
#include "obj_interface.h"
#include "sellpointlib.h"

#include "gproviderclient.hpp"

#define GDELIVERY_SERVER_ID  0
#define CASE_PROTO_HANDLE(_proto_name_)\
    case _proto_name_::PROTOCOL_TYPE:\
    {\
		_proto_name_ proto;\
		proto.unmarshal( os );\
		if ( proto.GetType()!=_proto_name_::PROTOCOL_TYPE || !proto.SizePolicy(os.size()) )\
			return false;\
		return Handle_##_proto_name_( proto,obj_if );\
	}

namespace GNET
{
	bool ForwardSellPointSysOP( unsigned int type,const void* pParams,size_t param_len,object_interface obj_if )
	{
		try
		{
			Marshal::OctetsStream os( Octets(pParams,param_len) );
			switch ( type )
			{
				default:
					return false;
			}
			return true;
		}
		catch ( Marshal::Exception e )
		{
			return false;
		}
	}
}
