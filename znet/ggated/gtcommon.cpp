#include "gtcommon.h"
#include "roleinfobean"
#include "ggateroleinfo"
#include "localmacro.h"
namespace GNET
{
RoleInfoBean GGateRoleInfo2RoleInfoBean(const GGateRoleInfo& r)
{
	Marshal::OctetsStream os;
//	os << r.reborntimes << r.cultivation;
//	return RoleInfoBean(r.roleid,r.rolename,r.gender,0,r.occupation,r.level,os);
	os << r.reborntimes;
	return RoleInfoBean(r.roleid,r.rolename,r.gender,r.cultivation,r.occupation,r.level,os);
}

RoleInfoBean GGateRoleInfo2RoleInfoBean2(const GGateRoleInfo& r, int userid)
{
	Marshal::OctetsStream os;
	os << (int64_t)userid;
	return RoleInfoBean(r.roleid,r.rolename,r.gender,r.cultivation,r.occupation,r.level,os);
}

};
