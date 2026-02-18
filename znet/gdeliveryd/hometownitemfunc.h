#ifndef _GNET_HOMETOWN_ITEM_FUNC_H_
#define _GNET_HOMETOWN_ITEM_FUNC_H_

#include <map>
#include "hometowntemplate.h"
#include "hometowndef.h"

namespace GNET
{
	class HometownItem;
	class HometownPlayerObj;
	class HometownTargetObj;

	class HometownItemFunc
	{
	public:
		virtual int CanPerform(const HometownPlayerObj *, const HometownItem *) const { return HT_ERR_CANNOTUSE; }
		virtual int CanPerform(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const { return HT_ERR_CANNOTUSE; }
		virtual void Perform(HometownPlayerObj *, HometownItem *) const {}
		virtual void Perform(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const {}
	};

	template <typename FUNC>
	class HTParamItemFunc : public HometownItemFunc
	{
	public:
		void SetTemplate(const FUNC *func) { _func = func; _OnSetTemplate(); }

	protected:
		virtual void _OnSetTemplate() {}

	protected:
		const FUNC * _func;
	};
	
	class HometownToolFunc : public HTParamItemFunc<TOOL_FUNC> 
	{
	public:
		typedef std::map<int, HometownToolFunc *> TypeFuncMap;
		HometownToolFunc(int id)
		{
			_stubs[id] = this;
		}
		static HometownToolFunc *MakeToolFunc(int type)
		{
			TypeFuncMap::iterator it = _stubs.find(type);
			if (it!=_stubs.end() && it->second)
				return it->second->Clone();
			else
				return NULL;
		}
		virtual HometownToolFunc *Clone() = 0;
	private:
		static TypeFuncMap _stubs;
	};

	#define CLONE(CLASSNAME) CLASSNAME *Clone() { return new CLASSNAME(*this); }
	template <int TOOL_FUNC_TYPE>
	class HTTypeToolFunc : public HometownToolFunc
	{
	public:
		HTTypeToolFunc():HometownToolFunc(TOOL_FUNC_TYPE) {}
	};
	//下面类型应该与enum TOOL_FUNC_TYPE一致
	typedef HTTypeToolFunc<TFT_FARM_PROTECT> _FarmProtectToolFunc;
	typedef HTTypeToolFunc<TFT_FIELD_PROTECT> _FieldProtectToolFunc;
	typedef HTTypeToolFunc<TFT_FIELD_SPEEDGROW> _FieldSpeedGrowToolFunc;
	typedef HTTypeToolFunc<TFT_FARM_SPEEDGROW> _FarmSpeedGrowToolFunc;
	typedef HTTypeToolFunc<TFT_FARM_HIDE> _FarmHideToolFunc;

	class FarmProtectToolFunc : public _FarmProtectToolFunc
	{
	public:
		int CanPerform(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const;
		void Perform(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const;
		CLONE(FarmProtectToolFunc)
	};
	class FieldProtectToolFunc : public _FieldProtectToolFunc
	{
	public:
		int CanPerform(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const;
		void Perform(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const;
		CLONE(FieldProtectToolFunc)
	};
	class FieldSpeedGrowToolFunc : public _FieldSpeedGrowToolFunc
	{
	public:
		int CanPerform(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const;
		void Perform(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const;
		CLONE(FieldSpeedGrowToolFunc)
	};
	class FarmSpeedGrowToolFunc : public _FarmSpeedGrowToolFunc
	{
	public:
		int CanPerform(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const;
		void Perform(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const;
		CLONE(FarmSpeedGrowToolFunc)
	};
	class FarmHideToolFunc : public _FarmHideToolFunc
	{
	public:
		int CanPerform(const HometownPlayerObj *, const HometownItem *) const;
		void Perform(HometownPlayerObj *, HometownItem *) const;
		CLONE(FarmHideToolFunc)
	};

	class HometownItemFuncMan
	{
		typedef std::map<unsigned int, const HometownItemFunc *> HTIFMap;
		HTIFMap _map;
		HometownTemplate *_tpl;
	
		HometownItemFuncMan():_tpl(NULL) {}
		template <typename ITEMFUNC, typename FUNC_TPL>
			void MakeFunc(unsigned int id);
		void MakeToolFunc(unsigned int id);
	public:
		~HometownItemFuncMan()
		{
			HTIFMap::iterator it = _map.begin(), ie = _map.end();
			for (; it != ie; ++it)
				delete it->second;
		}
		static HometownItemFuncMan * GetInstance()
		{
			static HometownItemFuncMan inst;
			return &inst;
		}
		void Init();		
		const HometownItemFunc *GetFunc(unsigned int id);
	};
};

#endif
