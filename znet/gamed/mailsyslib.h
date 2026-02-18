#ifndef __GNET_MAILSYSLIB_H
#define __GNET_MAILSYSLIB_H
namespace GNET
{
	// Lib function for gameserver
	int ForwardMailSysOP( unsigned int type,const void* pParams,size_t param_len,object_interface obj_if );
}
#endif
