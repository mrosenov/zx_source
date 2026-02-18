
#ifndef __ONLINEGAME_GAME_GS_COMMON_VALUE_H_
#define __ONLINEGAME_GAME_GS_COMMON_VALUE_H_

#include "octets.h"
#include <vector>

//这个文件描述了用于公共数据的Value
//
#define DELIVERY 1

#ifdef DELIVERY
#include <cassert>
using GNET::Octets;
#define ASSERT assert
#elif GAMESERVER
#include <ASSERT.h>
using abase::octets;
typedef abase::octets Octets;
#else
#error "Please Define DELIVERY or GAMESERVER before include this file"
#endif

class Value
{
	public:
		enum TYPE {
			TYPE_NULL, TYPE_INT, TYPE_CHAR, TYPE_SHORT, TYPE_FLOAT, 
			TYPE_DOUBLE, TYPE_OCTETS
		};
		typedef char Type;

		//构造，创建相关的
		Value():type(TYPE_NULL){}
		Value(const Value& rhs):type(rhs.type),data(rhs.data)  {}
		explicit Value(const int &v) { data.insert(data.end(),&v, sizeof(int));type=TYPE_INT;}
		explicit Value(const Octets &v):data(v) {type=TYPE_OCTETS;}
		Value(void *p,size_t size) {data.insert(data.end(),p, size);type=TYPE_OCTETS;}
		Value(Type, void*, size_t);
		static Value* Create(Type t);		//创建一个类型的实例
		Value* Clone () {return new Value(*this);}


		//类型获取修改相关的
		Type GetType () const {return type;}
		//强行改变类型。　注意，修改后变成该类型的默认值0
		void ChangeType (Type t);

		bool IsNull() {return type == TYPE_NULL;}
		void RawSetType (Type t)	//only used in DataStream wrapper
		{
			ASSERT (GetType() == TYPE_NULL);
			type = t;
		}

		char& ToChar() {
			ASSERT(type == TYPE_CHAR && "Incorrect type");
			return *(char*)(data.begin());
		}
		short& ToShort() {
			ASSERT(type == TYPE_SHORT && "Incorrect type");
			return *(short*)(data.begin());
		}
		int& ToInteger() { 
			ASSERT(type == TYPE_INT && "Incorrect type");
			return *(int*)(data.begin());
		}
		float& ToFloat() {
			ASSERT(type == TYPE_FLOAT && "Incorrect type");
			return *(float*)(data.begin());
		}
		double& ToDouble() {
			ASSERT(type == TYPE_DOUBLE && "Incorrect type");
			return *(double*)(data.begin());
		}
		Octets& ToOctets(){
			ASSERT(type == TYPE_OCTETS && "Try to convert to octets value from integer");
			return data;
		}

		//操作相关的
		void Inc();
		void Dec();
		void Add(Value& d);
		void Sub(Value& d);
		void Set(Value& v);
		void Operate (int op, Value& diff_val);

		//获取内部数据相关的
		Octets& GetData() { return data;}
		const char* c_str() {
			data.reserve (data.size()+1);
			*(char*)(data.end()) = 0;
			return (const char*)data.begin();
		}

		Value& operator =(const Value& rhs) {
			if (this == &rhs) return *this;
			type = rhs.type;
			data = rhs.data;
			return *this;
		}
		bool operator == (const Value& rhs) {
			return type == rhs.type && data == rhs.data;
		}
		bool IsInRange (Value& left, Value& right);
		void swap (Value& v)
		{
			ASSERT (GetType() == v.GetType());
			data.swap (v.data);
		}
		virtual ~Value()  {}
		static const Value& GetStub (Type t);
	protected:
		Type type;
		Octets data;
};


////////////////////////以下为辅助类///////////////////////////////
//helper class

//一个原子操作
struct data_atomic_action
{
	int _key;
	int _op;
	Value _value;

	data_atomic_action (){}
	data_atomic_action (const data_atomic_action& rhs):_key(rhs._key),_op(rhs._op),_value(rhs._value){}
	data_atomic_action(const int& key, const int& op, const Value& value):_key(key),_op(op),_value(value){}
};

enum OP_MODE {
	OP_MODE_INC,    //自增
	OP_MODE_DEC,    //自减
	OP_MODE_SET,    //设置
	OP_MODE_ADD,    //加上某值
	OP_MODE_SUB,    //减去某值
	OP_MODE_XCHG,    //减去某值
};

// helper class
//
class DataStream
{
	unsigned int ofs;
	Octets data;
	public:
	DataStream():ofs(0) {}
	DataStream(const Octets&d):ofs(0), data(d) {}
	DataStream(void *d, size_t len):ofs(0), data(d,len) {}

	~DataStream() {}
	Octets& GetData () {return data;}

	class Exception {};

	bool eos() const {return ofs == data.size();}

	template <typename T>
		DataStream & operator << (const T & t)
		{
			data.insert (data.end(), &t, sizeof(T));
			return *this;
		}

	template <typename T>
		DataStream & operator >> (T & t)
		{
			if (ofs + sizeof(T) > data.size()) throw Exception();
			t = *(T*)((char*)data.begin() +ofs);
			//memcpy (&t, (char*)data.begin()+ofs, sizeof(T));
			ofs += sizeof(T);
			return *this;
		}
};
	template <>
inline DataStream & DataStream::operator << (const Value & _v)
{
	Octets& v = (const_cast<Value&>(_v)).GetData();
	size_t size= v.size();
	data.insert (data.end(), &size, sizeof(size));
	char type = _v.GetType();
	data.insert (data.end(), &type, sizeof(char));
	data.insert (data.end(), v.begin(), v.size());
	return *this;
}
	template <>
inline DataStream & DataStream::operator >> (Value & o)
{

	if (ofs + sizeof(size_t) > data.size()) throw Exception();

	size_t size = *(size_t*)((char*)data.begin() +ofs);
	ofs += sizeof(size_t);

	char type = *(char*)((char*)data.begin() + ofs);
	ofs += sizeof(char);


	if (ofs + size > data.size()) throw Exception();

	Octets& odata = o.GetData();
	odata.insert (odata.end(), (char*)data.begin() + ofs, size);
	ofs += size;

	o.RawSetType(type);

	return *this;
}

	template <>
inline DataStream & DataStream::operator << (const Octets & _o)
{
	size_t size= _o.size();
	data.insert (data.end(), &size, sizeof(size));
	data.insert (data.end(), _o.begin(), _o.size());
	return *this;
}
	template <>
inline DataStream & DataStream::operator >> (Octets &_o)
{
	if (ofs + sizeof(size_t) > data.size()) throw Exception();

	size_t size = *(size_t*)((char*)data.begin() +ofs);
	ofs += sizeof(size_t);

	if (ofs + size > data.size()) throw Exception();

	_o.insert (_o.end(), (char*)data.begin() + ofs, size);
	ofs += size;

	return *this;
}
#endif
