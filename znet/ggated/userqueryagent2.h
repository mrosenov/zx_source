#ifndef __GNET_USERQUERYAGENT2_H
#define __GNET_USERQUERYAGENT2_H
#include "localmacro.h"
#include "queryagent.h"
#include "ggateroleinfo"
#include "roledetailbean"
#include "gtcommon.h"
//#include "rolelistresp.hpp"

namespace GNET
{

class RoleList
{
	static const int ROLELIST_DEFAULT=0x80000000;
	static const int MAX_ROLE_COUNT=16;
	unsigned int rolelist;
	//	int count;
	int cur_role;
	unsigned int logicuid;
public:
//	RoleList() : rolelist(0),count(0),cur_role(0) { }
//	RoleList(unsigned int r) : rolelist(r),count(0),cur_role(0) { }
	RoleList() : rolelist(0),cur_role(0),logicuid(0) { }
	RoleList(unsigned int r,unsigned int l) : rolelist(r),cur_role(0), logicuid(l){ }
	~RoleList() { }
//	void operator=(const RoleList& rhs) { rolelist=rhs.rolelist; cur_role=rhs.cur_role;}
	bool IsRoleListInitialed()const
	{
		return (rolelist & ROLELIST_DEFAULT) != 0;
	}
	bool IsRoleExist(int roleid)const
	{
		return (rolelist & (1<<(roleid % MAX_ROLE_COUNT))) != 0;
	}
/*	void InitialRoleList()
	{
		rolelist = ROLELIST_DEFAULT;
	}
	unsigned int GetRoleList() 
	{
		return rolelist;
	}*/
	int GetRoleCount()const
	{
		if (!IsRoleListInitialed()) return -1;  //rolelist is not initialized
		int count=0;
		for (int id=0;id<MAX_ROLE_COUNT;id++)
		{
			if (IsRoleExist(id)) count++;
		}
		return count;
	}
	size_t size() const {return GetRoleCount();}
	int GetNextRole()
	{
		while (cur_role<MAX_ROLE_COUNT)
			if (IsRoleExist(cur_role++)) return logicuid+cur_role-1;
		return -1;
	}
};
struct UserQuery
{
	UserQuery(int userid_, int type_):userid(userid_),pt_type(type_){}
	int userid;
	int pt_type;
};

class UserQueryOperation//:public QueryOperation
{
public:
        int DBQuery1(UserQuery req)const;
        int DBQuery2(int key,UserQuery req)const;
        int GetNextStep2Key(RoleList& rlist)const{return rlist.GetNextRole();}
	static int TryQuery(UserQuery key1);//userid
	int BuildResult(std::vector<RoleDetailBean> & roles,const GGateRoleInfo& info)const;
};
typedef QueryAgent<UserQuery,int/*nouse*/,RoleList,int,GGateRoleInfo,std::vector<RoleDetailBean>,UserQueryOperation> UserQueryAgent;
template<> inline int UserQueryAgent::SpecialBuild<UserQueryOperation>(std::vector<RoleDetailBean>& result)
{
	UserQueryAgent::T2Map::iterator it,ite=step2map.end();//example, nouse
	return 0;
}
template<> inline int UserQueryAgent::SpecialBuildStep2<UserQueryOperation>(std::vector<RoleDetailBean>& result,const GGateRoleInfo& node)
{
	return op.BuildResult(result,node);
}
typedef TaskQueue<UserQuery/*userid*/,RoleListReqType>  GTRoleListReqQueue;
template<> inline int GTRoleListReqQueue::RunTask<UserQuery,RoleListReqType>(const UserQuery& node)
{
//	UserQueryAgent agent(node);
//	return agent.TryQuery(node);
	return UserQueryAgent::TryQuery(node);
}

};
#endif

