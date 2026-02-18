#include "types.h"
#include "obj_interface.h"
#include "mailsyslib.h"

#include "libcommon.h"

#include "gmailsyncdata"
#include "gproviderclient.hpp"
#include "deletemail.hpp"
#include "getmailattachobj.hpp"
#include "getmail.hpp"
#include "getmaillist.hpp"
#include "playersendmail.hpp"
#include "preservemail.hpp"
#include "../gdbclient/db_if.h"

#define MAIL_FEE             500
#define GDELIVERY_SERVER_ID  0
#define CASE_PROTO_HANDLE(_proto_name_)\
	case _proto_name_::PROTOCOL_TYPE:\
	{\
		_proto_name_ proto;\
		proto.unmarshal( os );\
		if ( proto.GetType()!=_proto_name_::PROTOCOL_TYPE || !proto.SizePolicy(os.size()) )\
			return 1;\
		return Handle_##_proto_name_( proto,obj_if );\
	}

namespace GNET
{

	int Handle_DeleteMail( DeleteMail& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0 : 1;
	}
	int Handle_GetMailAttachObj( GetMailAttachObj& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return 1;

		GMailSyncData data;
		if ( !GetSyncData(data,obj_if) ) 
			return 1;

		proto.localsid          = obj_if.GetLinkSID();

		Marshal::OctetsStream os;
		os << data;
		proto.syncdata = os;
		if(obj_if.TradeLockPlayer(0,DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if ( GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) )
				return 0;
			obj_if.TradeUnLockPlayer();
		}
		return 1;
	}

	int Handle_GetMail( GetMail& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0:1;
	}
	int Handle_GetMailList( GetMailList& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0:1;
	}		
	int Handle_PlayerSendMail( PlayerSendMail& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id || obj_if.TestSafeLock() ) return 2;

		GMailSyncData data;
		if ( !GetSyncData(data,obj_if) ) 
			return 1;

		if ( (unsigned int)data.inventory.money < (unsigned int)proto.attach_money+MAIL_FEE || data.inventory.money<0 )
			 return 1;
		if ( proto.attach_obj_num!=0 && !obj_if.CheckItem(proto.attach_obj_pos,proto.attach_obj_id,proto.attach_obj_num) ) 
			return 1;

		proto.localsid = obj_if.GetLinkSID();
		Marshal::OctetsStream os;
		os << data;
		proto.syncdata = os;
		proto.localsid = obj_if.GetLinkSID();
		if(obj_if.TradeLockPlayer(0,DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ))
				return 0;
			obj_if.TradeUnLockPlayer();
		}
		return 1;
	}	
	int Handle_PreserveMail( PreserveMail& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0:1;
	}	

	int ForwardMailSysOP( unsigned int type,const void* pParams,size_t param_len,object_interface obj_if )
	{
		try {
			Marshal::OctetsStream os( Octets(pParams,param_len) );
			switch (type)
			{
				CASE_PROTO_HANDLE(DeleteMail)
				CASE_PROTO_HANDLE(GetMailAttachObj)
				CASE_PROTO_HANDLE(GetMail)
				CASE_PROTO_HANDLE(GetMailList)
				CASE_PROTO_HANDLE(PlayerSendMail)
				CASE_PROTO_HANDLE(PreserveMail)
				default:
					return 1;	
			}
		}
		catch ( Marshal::Exception )
		{
			return 1;
		}
	}
}
