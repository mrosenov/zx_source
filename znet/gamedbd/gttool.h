#ifndef __GNET_GTTOOL_H
#define __GNET_GTTOOL_H
namespace GNET
{
	void ClearActivate(int& mask,int operation){mask&=(~operation);}
	void SetActivate(int& mask,int operation){mask|=operation;}
	bool IsActivate(int mask,int operation){return mask&operation;}
	bool IsMaskClean(int mask){return mask==ACTIVATE_NONE;}
};
#endif
