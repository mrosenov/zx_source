#ifndef __GNET_GTCOMMON_H
#define __GNET_GTCOMMON_H
namespace GNET
{
class RoleInfoBean;
class GGateRoleInfo;
enum GTActivateOperation
{
	OP_ACTIVATE=0,
	OP_DEACTIVATE,
};	
enum GGatePlatformType
{
	GGT_GT_TYPE = 0, // GT
	GGT_SNS_TYPE = 1, // SNS
};
RoleInfoBean GGateRoleInfo2RoleInfoBean(const GGateRoleInfo& r);
RoleInfoBean GGateRoleInfo2RoleInfoBean2(const GGateRoleInfo& r, int userid);
};
#endif
