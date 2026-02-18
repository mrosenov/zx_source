#ifndef __GNET_SIMPLEQUERY_H
#define __GNET_SIMPLEQUERY_H
#include <map>
//#include "ggateroleinfo"
//#include "roledetailbean"
#include "taskqueue.h"
namespace GNET
{
class SimpleQuery
{
public:
	virtual int TryQuery() const{
		DEBUG_PRINT("SimpleQuery TryQuery,err call");
		return 0;
	}
	virtual ~SimpleQuery(){}
	SimpleQuery(){}
};

class RoleRelationTask:public SimpleQuery
{
public:	
	RoleRelationTask(int uid,int rid, int _type):userid(uid),roleid(rid),pt_type(_type){}
	int userid;
	int roleid;
	int pt_type;
	virtual int TryQuery() const;
};
class FamilyInfoTask:public SimpleQuery
{
public:
	FamilyInfoTask(int64_t localuid_,int fid_):localuid(localuid_),fid(fid_){}
	int64_t localuid;
	int fid;
	virtual int TryQuery() const;
};
class RoleInfoTask:public SimpleQuery
{
public:
	RoleInfoTask(int64_t localuid_,int rid):localuid(localuid_),roleid(rid){}
	int64_t localuid;
	int roleid;
	virtual int TryQuery() const;
};
/*class RoleStatusUpdateTask:public SimpleQuery
{
public:	
	int roleid;
	unsigned char status;
	std::vector<int> friends;
};*/
typedef TaskQueue<RoleRelationTask,RoleRelationReqType>  GTRoleRelationReqQueue;
//typedef TaskQueue<FactionInfoTask,FactionInfoReqType>  GTFactionInfoReqQueue;
typedef TaskQueue<FamilyInfoTask,FamilyInfoReqType>  GTFamilyInfoReqQueue;
typedef TaskQueue<RoleInfoTask,RoleInfoReqType>  GTRoleInfoReqQueue;
//typedef TaskQueue<RoleStatusUpdateTask,RoleStatusUpdateType>  GTRoleStatusUpdateQueue;
//	typedef TaskQueue<RoleStatusUpdate,RoleStatusUpdateType>  GTRoleStatusUpdateQueue;
/*
template<> inline int GTRoleRelationReqQueue::RunTask<RoleRelationTask,RoleRelationReqType>(const RoleRelationTask& node)
{
	return node.TryQuery();
}
template<> inline int GTFactionInfoReqQueue::RunTask<FactionInfoTask,FactionInfoReqType>(const FactionInfoTask& node)
{
	return 0;
}
template<> inline int GTFamilyInfoReqQueue::RunTask<FactionInfoTask,FamilyInfoReqType>(const FactionInfoTask& node)
{
	return 0;
}
template<> inline int GTRoleInfoReqQueue::RunTask<RoleInfoTask,RoleInfoReqType>(const RoleInfoTask& node)
{
	return 0;
}
template<> inline int GTRoleStatusUpdateQueue::RunTask<RoleStatusUpdateTask,RoleStatusUpdateType>(const RoleStatusUpdateTask& node)
{
	return ;
}*/

};
#endif

