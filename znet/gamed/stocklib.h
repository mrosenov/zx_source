#ifndef __GNET_STOCKLIB_H
#define __GNET_STOCKLIB_H
namespace GNET
{
	bool SendStockTransaction(object_interface player, char withdraw, int cash, int money);
	int ForwardStockCmd( unsigned int type,const void* pParams,size_t param_len,object_interface player);
	bool SendBillingBalance(int roleid);
	bool SendBillingRequest(int roleid, int itemid, int itemnum, int timeout, int amount, int count);
	bool SendBillingConfirm(int roleid, int itemid, int itemnum, int timeout, int amount, int count, const char* txno, size_t len);
	bool SendBillingCancel (int roleid, int itemid, int itemnum, int timeout, int amount, int count, const char* txno, size_t len);
}
#endif
