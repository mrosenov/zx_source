#ifndef __GNET_QUERYAGENT_H
#define __GNET_QUERYAGENT_H
#include <map>
#include "taskqueue.h"
namespace GNET
{

/*
//specification for all class TOperation
class QueryOperation
{
public:
	template<class T1> int DBQuery1(const T1&);
	template<class TKey2> int DBQuery2(const Tkey2&,const T1&);
	template<class TKey2> int GetNextStep2Key();
	template<class T1>static int TryQuery(const T1& key1);
 	template<class TResult>int BuildResult(TResult & roles,const T2&info);
};
*/
	
template<class T1,class T1Result,class TList,class TKey2,class T2,class TResult,class TOperation>
class QueryAgent
{
private:
	T1 step1data;
	T1Result step1result;
	const TOperation& op;
	bool query_all;//query all step2 nodes at the same time
	
	class RData
	{
		public:
			RData():valid(false){}
			void SetData(const T2& r){info=r;valid=true;}
			T2 info;
			bool valid;
			//short repeat_count;
	};
	enum QueryStatusType{
		st_zero=0,
		st_one,
		st_two,
		st_three
	};
	class QueryStatus
	{
		public:
			QueryStatus():istatus(st_zero){}
			int istatus;
			bool IsStatus(int s){return istatus ==s;}
			bool Status_Move(int target){
				//may add check here
				istatus = target;
				return true;
			}
	};
	TList allstep2node;
	typedef std::map<TKey2,RData> T2Map;// roleid->RData
	T2Map step2map;
	QueryStatus status;//one,two
	int ticks;
	const static int interval=60;
	public:
//	QueryAgent(const T1 & t1,const QueryOperation& oper):step1data(t1),op(oper){}
	QueryAgent(const T1 & t1,bool queryall):step1data(t1),op(TOperation()),query_all(queryall){}
	~QueryAgent(){}
	int Query_One()
	{
//		DEBUG_PRINT("Query_One");
		if(!status.IsStatus(st_zero))
			return -1;
		op.DBQuery1(step1data);	
		if(!status.Status_Move(st_one))
			return -1;
		return 0;
	}

	int Query_Two_All()
	{
		TKey2 k2;
		while(1)
		{
			k2=op.GetNextStep2Key(allstep2node);
	//		DEBUG_PRINT("Query_Two_All k2=%d",k2);
			if(k2>=0)
			{
				step2map.insert(std::make_pair(k2,RData()));
			}
			else
				break;
		}
		typename T2Map::iterator it,ite=step2map.end();
		DEBUG_PRINT("Query_Two_All step2map size=%d",step2map.size());
		for(it=step2map.begin();it!=ite;++it)
		{
			Query_Two(it->first);
		}
		return TryFinish();
	}

	int Query_Two(const TKey2 key)
	{
		if(!status.IsStatus(st_two))	
			return -1;
		op.DBQuery2(key,step1data);
		return 0;
	}

	int Query_Two()
	{
		TKey2 k2=op.GetNextStep2Key(allstep2node);
		if(k2>=0)
		{
			DEBUG_PRINT("Query_Two k2=%d",k2);
			step2map.insert(std::make_pair(k2,RData()));
			return Query_Two(k2);
		}
		else//no other step2 nodes
		{
			if(!status.Status_Move(st_three))
				return -1;
		}
		return 0;
	}

	int On_Resp_One(const TList& s2list,const T1Result& t1res)
	{
		DEBUG_PRINT("On_Resp_One membersize=%d",s2list.size());
		if(!status.IsStatus(st_one))
			return -1;
		allstep2node=s2list;
		step1result = t1res;
		if(!status.Status_Move(st_two))
			return -1;
		if(query_all)
			return Query_Two_All();
		else
			return Query_Two();
	}
	
	int On_Resp_Two_Err(TKey2 k2)
	{
		if(!status.IsStatus(st_two))
			return -1;
		typename T2Map::iterator it;
		it = step2map.find(k2);
		if(it == step2map.end())
			return -1;
		step2map.erase(it);
		if(query_all)
			return TryFinish();
		else
			return Query_Two();
	}

	int On_Resp_Two(TKey2 k2,const T2& resp2)
	{
		if(!status.IsStatus(st_two))
			return -1;
		typename T2Map::iterator it;
		it = step2map.find(k2);
		if(it == step2map.end())
			return -1;
		it->second.SetData(resp2);
		if(query_all)
			return TryFinish();
		else
			return Query_Two();
	}
/*	int Check_Timeout(int delta){
	ticks+=delta;
	if(ticks>=interval){
	if(status==1){
	Query_One();
	}
	else if(status==2)
	Query_Two();
	}

	}*/
	int TryFinish()
	{
		if(!status.IsStatus(st_two))
			return 0;
		typename T2Map::iterator it,ite=step2map.end();
		for(it=step2map.begin();it!=ite;++it)
		{
			if(it->second.valid==false)
			{
				return 0;
			}
		}
		if(!status.Status_Move(st_three))
			return -1;
		return 0;
	}
	static int TryQuery(const T1& key1)
	{
		//return GTManager::Instance()->TryUserQuery(uid);
		// -> TryQuery
		//QueryOperation::TryQuery(key1);
		return TOperation::TryQuery(key1);
	}
/*	template<class T1other>
	static int TryQuery(const T1other& key1)
	{
		//return GTManager::Instance()->TryUserQuery(uid);
		// -> TryQuery
		QueryOperation::TryQuery(key1);
	}*/
	bool QueryFinish(){return status.IsStatus(st_three);}
	template<class TFunc>
	int SpecialBuild(TResult & result);
	template<class TFunc>
	int SpecialBuildStep2(TResult & result,const T2& node);
	int BuildResult(TResult & result)
	{
		DEBUG_PRINT("BuildResult");
	        if(!status.IsStatus(st_three))
        	        return -1;		
		SpecialBuild<TOperation>(result);//faction decleration
		typename T2Map::iterator it,ite=step2map.end();
		for(it=step2map.begin();it!=ite;++it)
		{
			if(it->second.valid==true)
			{
				SpecialBuildStep2<TOperation>(result,it->second.info);
			}
			else
				return -1;
		}
		return 0;
	
	}

};

};
#endif

