#ifndef _GNET_HOMETOWN_ITEM_BODY_H_
#define _GNET_HOMETOWN_ITEM_BODY_H_

#include <map>
#include "hometowntemplate.h"
#include "hometowndef.h"

namespace GNET
{
	class HometownItem;
	class HometownItemData;
	class HometownTargetObj;
	class HometownPlayerObj;
	class HometownItemFunc;

	class HometownItemBody
	{
	public:
		virtual ~HometownItemBody(){}
	public:
		virtual int CanUse(const HometownPlayerObj *, const HometownItem *) const { return HT_ERR_CANNOTUSE; }
		virtual int CanUse(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const { return HT_ERR_CANNOTUSE; }
		virtual bool OnUse(HometownPlayerObj *, HometownItem *) const { return true; }
		virtual bool OnUse(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const { return true; }
		virtual void FillItemData(HometownItemData &idata) const {}
	};
	
	template <typename ESSENCE>
	class HTParamItemBody : public HometownItemBody
	{
	public:
		HTParamItemBody():_ess(NULL) {}
		void SetTemplate(const ESSENCE *ess) 
		{ 
			_ess = ess; 
			_OnSetTemplate();
		}
		const ESSENCE *GetTemplate() { return _ess; }
	protected:
		virtual void _OnSetTemplate() {}

	protected:
		int _id;
		const ESSENCE * _ess;
	};

	class CropBody : public HTParamItemBody<CROP_ESSENCE>
	{
	public:
		void FillItemData(HometownItemData &idata) const;
	protected:
		void _OnSetTemplate();	
	};

	class FruitBody : public HTParamItemBody<FRUIT_ESSENCE>
	{
	public:
		void FillItemData(HometownItemData &idata) const;
	protected:
		void _OnSetTemplate();
	};

	class ToolBody : public HTParamItemBody<TOOL_ESSENCE>
	{
	public:
		int CanUse(const HometownPlayerObj *, const HometownItem *) const;
		int CanUse(const HometownPlayerObj *, const HometownItem *, const HometownTargetObj *) const;
		bool OnUse(HometownPlayerObj *, HometownItem *) const;
		bool OnUse(HometownPlayerObj *, HometownItem *, HometownTargetObj *) const;
		void FillItemData(HometownItemData &idata) const;
	protected:
		void _OnSetTemplate();
	private:
		std::vector<const HometownItemFunc *> _funcs;
	};

	class HometownItemBodyMan
	{
		typedef std::map<unsigned int, const HometownItemBody *> HTIBMap;
		HTIBMap _map;
		HometownTemplate *_tpl;
	
		HometownItemBodyMan():_tpl(NULL) {}
		template <typename ITEMBODY, typename ESSENCE>
			void MakeBody(unsigned int id);
	public:
		~HometownItemBodyMan()
		{
			HTIBMap::iterator it = _map.begin(), ie = _map.end();
			for (; it != ie; ++it)
				delete it->second;
		}
		static HometownItemBodyMan * GetInstance()
		{
			static HometownItemBodyMan inst;
			return &inst;
		}
		void Init();		
		const HometownItemBody *GetBody(unsigned int id);
	};
};
#endif
