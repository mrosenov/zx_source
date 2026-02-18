#ifndef __GNET_FACTIONQUERYAGENT_H
#define __GNET_FACTIONQUERYAGENT_H
#include "localmacro.h"
#include "queryagent.h"
#include "gtcommon.h"

#include "factioninforesp.hpp"
#include "factioninfobean"
#include "factionidbean"
#include "gfolk"

namespace GNET
{

class FamilyList
{
	std::set<int> familyidlist;
	int count;
	std::set<int>::iterator cur_fid;
public:
	FamilyList() : count(-1){cur_fid=familyidlist.end();}
	FamilyList(const FamilyList& rhs): familyidlist(rhs.familyidlist)
	{
	//	DEBUG_PRINT("FamilyList copy Constructor");
		count =familyidlist.size();
		if(rhs.cur_fid == rhs.familyidlist.end())
			cur_fid = familyidlist.end();
		else
			cur_fid = familyidlist.find(*rhs.cur_fid);
	}
	FamilyList(const std::vector<FamilyId>& idvec)
	{
		for(FamilyIdVector::const_iterator it=idvec.begin();it!=idvec.end();++it)
			familyidlist.insert(it->fid);
		count = familyidlist.size();
		cur_fid = familyidlist.begin();
       	}
	~FamilyList() { }
	void operator=(const FamilyList& rhs) 
	{
	//	DEBUG_PRINT("FamilyList operator =");	
		familyidlist=rhs.familyidlist;
		count =familyidlist.size();
		if(rhs.cur_fid == rhs.familyidlist.end())
			cur_fid = familyidlist.end();
		else
			cur_fid = familyidlist.find(*rhs.cur_fid);
	}
	size_t size() const {return count;}
	int GetNodeCount(){
		return count;
	}
	int GetNextNode(){
		if (cur_fid!=familyidlist.end())
			return *cur_fid++;
		else
			return -1;
	}
};
struct FactionReq
{
	FactionReq(int64_t localuid_,int fid_,int type_):localuid(localuid_),fid(fid_),pt_type(type_){}
	int64_t localuid;
	int fid;
	int pt_type;
};
class FactionQueryOperation
{
public:
        int DBQuery1(FactionReq req)const;
        int DBQuery2(int familyid,FactionReq req)const;
	int GetNextStep2Key(FamilyList& rlist)const{return rlist.GetNextNode();}
	static int TryQuery(const FactionReq& key1);
	int BuildResult(FactionInfoResp & resp,const std::vector<GNET::GFolk>& info)const;
};
typedef QueryAgent<FactionReq,FactionInfoBean,FamilyList,int,std::vector<GFolk>,FactionInfoResp,FactionQueryOperation> FactionQueryAgent;
template<> inline int FactionQueryAgent::SpecialBuild<FactionQueryOperation>(FactionInfoResp& result)
{
	result.factioninfo = step1result;
	result.factionid = FactionIDBean(FACTIONTYPE,step1data.fid);
	result.localuid = step1data.localuid;
	return 0;
}
template<> inline int FactionQueryAgent::SpecialBuildStep2<FactionQueryOperation>(FactionInfoResp& result,const std::vector<GFolk>& node)
{
	return op.BuildResult(result,node);
}
typedef TaskQueue<FactionReq,FactionInfoReqType>  GTFactionInfoReqQueue;
template<> inline int GTFactionInfoReqQueue::RunTask<FactionReq,FactionInfoReqType>(const FactionReq& node)
{
	return FactionQueryAgent::TryQuery(node);
}

FactionInfoBean GFactionInfo2FactionInfoBean(const GFactionInfo& faction);
};
#endif

