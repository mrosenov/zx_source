#ifndef __LOG_INDEX_HPP__
#define __LOG_INDEX_HPP__
#include "logcommon/baseclass.hpp"
#include "protocol.h"
#include "marshal.h"

namespace LOG
{

struct Index
{
	GNET::Protocol * createProtocol(const GNET::Marshal::OctetsStream &data)
	{
                return create(data);
	}

	virtual bool is_rebuild() const { return false; }
	virtual void destroy() { delete this; }

	///////////////////////////////////////////////////
	//
	//构建协议对象, protocol 指针由indexmaker负责删除
	//
	///////////////////////////////////////////////////
	virtual GNET::Protocol * create(const GNET::Marshal::OctetsStream &data) const = 0;

	///////////////////////////////////////////////////
	//
	//获得keyvalue值, 用户填写
	//
	///////////////////////////////////////////////////
	virtual keyvalue_t extractkeyvalue(GNET::Protocol *p) const  = 0;
	virtual type_t get_type() const  = 0;	
	virtual std::string get_keyname() const  = 0;	
	virtual ~Index() {}
};

}//end namespace LOG

#endif
