#include <stdlib.h>
#include "substance.h"
#include <hashmap.h>

ClassInfo substance::m_classsubstance("substance",-1,NULL,substance::CreateObject,0);
ClassInfo * substance::GetRunTimeClass(){ return &substance::m_classsubstance;}
substance * substance::CreateObject() { return NULL;}

ClassInfo * glb_info_tail = NULL;
ClassInfo * glb_info_head = NULL;

typedef abase::hash_map<int,ClassInfo *> CLASSINFO_MAP;

static CLASSINFO_MAP & GetClsInfoMap()
{
	static CLASSINFO_MAP _map(1024);
	return _map;
}

#include <typeinfo>
using namespace std;

ClassInfo::ClassInfo (const char * name, int guid,ClassInfo*pBase,substance *(*pfCreate)(), size_t typesize)
			:_name(name),_guid(guid),_pfCreate(pfCreate),_base(pBase),_ref(1),_type_size(typesize)
{
//测试唯一性
	
	ClassInfo * tmp = glb_info_head;
	if(guid != -1){
		//guid是-1的话，代表是抽象类 可以重复
		//只有非抽象类才能用guid查询ClassInfo
		CLASSINFO_MAP::iterator it = GetClsInfoMap().find(guid);
		if(it != GetClsInfoMap().end())
		{
			printf("重复的类GUID, class '%s' 与 class '%s' ,guid:%d\n",
					it->second->_name,name,guid);
			ASSERT(false);
			return;
		}
		GetClsInfoMap()[guid] = this;
		while(tmp)
		{
			ASSERT(tmp->_guid != guid);
			if(strcmp(tmp->_name,name) == 0)
			{
				//命名重复
				tmp->_ref ++;
				_ref ++;
			}
			tmp = tmp->_next;
		}
	}
	
	_next = NULL;
	_prev = glb_info_tail;

	if(glb_info_tail)
	{
		glb_info_tail->_next  = this;
	}
	else
	{
		glb_info_head = this;
	}
	ASSERT(glb_info_head);
	glb_info_tail = this;
}

ClassInfo::~ClassInfo ()
{
	if(_prev)
	{
		_prev->_next = _next;
	}
	if(_next)
	{
		_next->_prev = _prev;
	}
	if(glb_info_head == this)
	{
		glb_info_head = _next;
	}
	if(glb_info_tail == this)
	{
		glb_info_tail = _prev;
	}
	//ASSERT(GetClsInfoMap().find(_guid) != GetClsInfoMap().end());
	GetClsInfoMap().erase(_guid);
}

ClassInfo *
ClassInfo::GetRunTimeClass(const char *name, int & ref)
{
	ClassInfo * tmp = glb_info_head;
	while(tmp)
	{
		if(strcmp(tmp->_name,name) == 0) 
		{
			ref = tmp->_ref;
			return tmp;
		}
		tmp = tmp->_next;
	}
	return NULL;
}

ClassInfo * 
ClassInfo::GetRunTimeClass(int guid)
{
	CLASSINFO_MAP &map = GetClsInfoMap();
	CLASSINFO_MAP::iterator it = map.find(guid);
	if(it == map.end()) return NULL;

	return it->second;
/*	ClassInfo * tmp = glb_info_head;
	while(tmp)
	{
		if(tmp->_guid == guid) return tmp;
		tmp = tmp->_next;
	}
	return NULL;*/
}

bool 
ClassInfo::IsDerivedFrom(const ClassInfo * base)
{
	ClassInfo * tmp = this;
	while(tmp)
	{
		if(tmp == base) //或者strcmp(tmp->GetName(),base->GetName()) == 0
		{
			return true;
		}
		tmp = tmp->GetBaseClass();
	}
	return false;
}

bool
substance::DumpContent(std::string & str)
{
	ClassInfo * pInfo = GetRunTimeClass();
	if(!pInfo) return false;
	str = pInfo->GetName();
	str += " dump: ";
	size_t size = pInfo->GetTypeSize();
	unsigned char * p = (unsigned char*) this;
	for(size_t i = 0; i < size; i += 4)
	{
		char buf[128];
		sprintf(buf, "%02X ",  p[i]);
		str = str + buf;
	}
	return true;
}

