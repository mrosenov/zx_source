#ifndef _GNET_SUBSTANCE_H_
#define _GNET_SUBSTANCE_H_
#include <map>
#include <functional>
#include <algorithm>
#include <marshal.h>
#include <assert.h>

namespace GNET
{
	class Substance;
	class ClassInfo
	{
		int _clsid;
		const ClassInfo *_base;
		Substance *(*_pCreateFunc)();
		const char *_name;
		typedef std::map<int, ClassInfo*> CLASSMAP;
		static CLASSMAP &GetClsMap()
		{
			static CLASSMAP clsmap;
			return clsmap;
		}
	public:
		ClassInfo(int clsid, const char *name, const ClassInfo *base, Substance *(*pCreateFunc)()):_clsid(clsid),_base(base), _pCreateFunc(pCreateFunc), _name(name)
		{
			assert(GetClsMap().find(_clsid)==GetClsMap().end());
			GetClsMap()[_clsid] = this;
		}

		Substance *Create() const { return _pCreateFunc(); }

		int GetClsID() { return _clsid; }
		bool IsDerivedFrom(const ClassInfo *base) const
		{
			const ClassInfo *cls = this;
			while (cls!=NULL && cls!=base)
				cls = cls->_base;
			return cls==base;
		}

		const char *GetClsName() { return _name; }

		static const ClassInfo *GetClassInfo(int clsid)
		{
			CLASSMAP::iterator it = GetClsMap().find(clsid);
			return it==GetClsMap().end() ? NULL : it->second;
		}
	};

	#define DECLARE_SUBSTANCE(classname) \
		static ClassInfo _class##classname;\
		static classname *CreateObject() { return new classname(); }\
		static ClassInfo *GetClass() { return &classname::_class##classname; }\
		static int GetClsID() { return GetClass()->GetClsID(); }\
		static const char *GetClsName() { return GetClass()->GetClsName(); }\
		virtual ClassInfo *GetRunTimeClass() const { return &classname::_class##classname; }
	
	#define DECLARE_SUBSTANCE_ABSTRACT(classname) \
		static ClassInfo _class##classname;\
		static classname *CreateObject() { return NULL; }\
		static ClassInfo *GetClass() { return &classname::_class##classname; }\
		static int GetClsID() { return GetClass()->GetClsID(); }\
		static const char *GetClsName() { return GetClass()->GetClsName(); }\
		virtual ClassInfo *GetRunTimeClass() const { return &classname::_class##classname; }

	#define DEFINE_SUBSTANCE(clsid, classname, basename) \
		ClassInfo classname::_class##classname(clsid, #classname, &basename::_class##basename, (Substance *(*)())classname::CreateObject);

	class Substance : public Marshal
	{

		static Substance *CreateSubstance(int clsid)
		{
			const ClassInfo *clsinfo = ClassInfo::GetClassInfo(clsid);
			assert(clsinfo != NULL);
			return clsinfo->Create();
		}
	public:
		DECLARE_SUBSTANCE_ABSTRACT(Substance)
		static Substance *LoadSubstance(const OctetsStream& os)
		{
			int clsid;
			os >> clsid;
			Substance *inst = CreateSubstance(clsid);
			assert(inst != NULL);
			os >> *inst;
			return inst;
		}

		OctetsStream &SaveSubstance(OctetsStream &os) const
		{
			os << GetRunTimeClass()->GetClsID();
			os << *this;
			return os;
		}

		template <typename T>
		bool IsInstanceOf() const
		{
			return GetRunTimeClass()==T::GetClass();
		}

		template <typename T>
		bool IsDerivedFrom() const
		{
			return GetRunTimeClass()->IsDerivedFrom(T::GetClass());
		}

		template <typename TO, typename FROM>
		static TO *DynamicCast(FROM *frm)
		{
			return frm->IsDerivedFrom<TO>() ? (TO *)frm : NULL;
		}

		template <typename TO, typename FROM>
		static const TO *DynamicCast(const FROM *frm)
		{
			return frm->IsDerivedFrom<TO>() ? (const TO *)frm : NULL;
		}

		int GetRunTimeClsID() const
		{
			return GetRunTimeClass()->GetClsID();
		}

		const char *GetRunTimeClsName() const
		{
			return GetRunTimeClass()->GetClsName();
		}
	};
};
#endif
