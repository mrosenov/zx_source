#include "commonvalue.h"
#include <map>

static std::map<Value::Type,Value> __stub_map;

static char __c =0;
static int __int =0;
static unsigned short __short = 0;
static float __float =0.0f;
static double __double = 0.0f;
static Octets __octets;

static Value __stub_char (Value::TYPE_CHAR, &__c, sizeof(char));
static Value __stub_short (Value::TYPE_SHORT,&__short, sizeof(short));
static Value __stub_int (Value::TYPE_INT, &__int, sizeof(int));
static Value __stub_float (Value::TYPE_FLOAT,&__float, sizeof(float));
static Value __stub_double (Value::TYPE_DOUBLE,&__double, sizeof(double));
static Value __stub_octets (Value::TYPE_OCTETS, __octets.begin(), __octets.size());

Value::Value(Value::Type t, void *p, size_t len):type(t)
{
	data.insert (data.end(), p, len);

	if (__stub_map.find(t) == __stub_map.end())
	{
		__stub_map.insert (std::make_pair(t,*this));
	}
}
Value* Value::Create (Value::Type t)
{
	std::map<Type,Value>::iterator it = __stub_map.find(t);
	ASSERT (it != __stub_map.end());

	return it->second.Clone();
}

const Value& Value::GetStub (Value::Type t)
{
	std::map<Type,Value>::iterator it = __stub_map.find(t);
	ASSERT (it != __stub_map.end());

	return it->second;
}
void Value::ChangeType (Value::Type t)
{
	if (type == t) return;
	*this = GetStub (t);
}
void Value::Inc()
{
   switch (type)
   {
	case TYPE_CHAR:
		(ToChar())++;
		break;
	case TYPE_SHORT:
		(ToShort())++;
		break;
	case TYPE_INT:
		(ToInteger())++;
		break;
	case TYPE_FLOAT:
		(ToFloat()) += 1.0f;
		break;
	case TYPE_DOUBLE:
		(ToDouble()) += 1.0f;
		break;
	case TYPE_OCTETS:
		break;
   }
}
void Value::Dec()
{
   switch (type)
   {
	case TYPE_CHAR:
		(ToChar())--;
		break;
	case TYPE_SHORT:
		(ToShort())--;
		break;
	case TYPE_INT:
		(ToInteger())--;
		break;
	case TYPE_FLOAT:
		(ToFloat()) -= 1.0f;
		break;
	case TYPE_DOUBLE:
		(ToDouble()) -= 1.0f;
		break;
	case TYPE_OCTETS:
		break;
   }

}
void Value::Add(Value& d)
{
   switch (type)
   {
	case TYPE_CHAR:
		(ToChar()) += d.ToChar();
		break;
	case TYPE_SHORT:
		(ToShort()) += d.ToShort();
		break;
	case TYPE_INT:
		(ToInteger()) += d.ToInteger();
		break;
	case TYPE_FLOAT:
		(ToFloat()) += d.ToFloat();
		break;
	case TYPE_DOUBLE:
		(ToDouble()) += d.ToDouble();
		break;
	case TYPE_OCTETS:
		break;
   }

}
void Value::Sub(Value& d)
{
   switch (type)
   {
	case TYPE_CHAR:
		(ToChar()) -= d.ToChar();
		break;
	case TYPE_SHORT:
		(ToShort()) -= d.ToShort();
		break;
	case TYPE_INT:
		(ToInteger()) -= d.ToInteger();
		break;
	case TYPE_FLOAT:
		(ToFloat()) -= d.ToFloat();
		break;
	case TYPE_DOUBLE:
		(ToDouble()) -= d.ToDouble();
		break;
	case TYPE_OCTETS:
		break;
   }
}
void Value::Set(Value& d)
{
	*this =d;
}
bool Value::IsInRange (Value& left, Value& right)
{
   switch (type)
   {
	case TYPE_CHAR:
		return (ToChar() >= left.ToChar()) && (ToChar() <= right.ToChar());
	case TYPE_SHORT:
		return (ToShort() >= left.ToShort()) && (ToShort() <= right.ToShort());
	case TYPE_INT:
		return (ToInteger() >= left.ToInteger()) && (ToInteger() <= right.ToInteger());
	case TYPE_FLOAT:
		{
			const float e = 0.000001f;
			return (ToFloat() > left.ToFloat() - e) && (ToFloat() < right.ToFloat() + e);
		}
	case TYPE_DOUBLE:
		{
			const double e = 0.00000000001f;
			return (ToDouble() > left.ToDouble() - e) && (ToDouble() < right.ToDouble() + e );
		}
	case TYPE_OCTETS:
		break;
   }
   return false;
}
void Value::Operate (int op, Value& diff_value)
{
	ASSERT (GetType() == diff_value.GetType());

	switch (op)
	{
		case OP_MODE_INC:
			Inc();
			break;
		case OP_MODE_DEC:
			Dec();
			break;
		case OP_MODE_ADD:
			Add(diff_value);
			break;
		case OP_MODE_SUB:
			Sub(diff_value);
			break;
		case OP_MODE_SET:
			Set(diff_value);
			break;
		case OP_MODE_XCHG:
			swap(diff_value);
			break;
		default:
			break;
	}
}
