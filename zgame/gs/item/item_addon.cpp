#include "item_addon.h"
#include "../item.h"
#include "item_equip.h"
#include "../actobject.h"
#include "../player_imp.h"

addon_manager addon_manager::__instance;
addon_data_man addon_data_man::__instance;
int addon_inserter::_counter = 0;

namespace
{
enum  ADDON_ARG_TYPE
{
	VALUE,
	SCALE,
};


template <typename DEST_TYPE, typename SRC_TYPE, ADDON_ARG_TYPE arg, int MAX_VALUE> class DataTransform ;

template <typename DEST_TYPE, typename SRC_TYPE, int MAX_VALUE> class DataTransform<DEST_TYPE,SRC_TYPE, VALUE,MAX_VALUE>
{
protected:
	bool Transform(addon_data & data)
	{
		DEST_TYPE m = ((DEST_TYPE)(MAX_VALUE+1))/1000;

		SRC_TYPE t1 = *(SRC_TYPE*)&data.arg[0];
		DEST_TYPE t2 = (DEST_TYPE) t1;
		if(t2 > m || t2 < -m ) 
		{
			t2 = 0;
			printf("一类发生附加属性越界%d\n",data.id);
			return false;
		}
		data.arg[0] = *(int*)&t2;
		return true;
	}
};

template <typename DEST_TYPE, typename SRC_TYPE,int MAX_VALUE> class DataTransform<DEST_TYPE,SRC_TYPE, SCALE,MAX_VALUE>
{
protected:
	static bool Transform(addon_data & data)
	{
		DEST_TYPE m = ((DEST_TYPE)(MAX_VALUE+1))/1000;

		SRC_TYPE t = *(SRC_TYPE*)&data.arg[0];
		DEST_TYPE t2 = (DEST_TYPE)(t * 1000.f + 0.0001f);
		if(t2 > m || t2 < -m ) 
		{
			t2 = 0;
			printf("二类发生附加属性越界%d\n",data.id);
			return false;
		}
		data.arg[0] = *(int*)&t2;
		return true;
	}
};

}

#define __XT(x) (int)(x*1000 + 0.001f)
template <typename PARAM_TYPE, int OFFSET_IN_IMP, typename DATATRANS = DataTransform<PARAM_TYPE,PARAM_TYPE,VALUE,__XT(600)> >
class EPSA_addon: public addon_handler, public DATATRANS 
{
	public:
		virtual bool TransformData(addon_data & data)
		{
			return DATATRANS::Transform(data);
		}
		
		virtual int UpdateItem(const addon_data & , equip_data *)
		{
			return addon_manager::ADDON_MASK_ACTIVATE;
		}

		virtual int Use(const addon_data & , const item_body *, gactive_imp *)
		{
			ASSERT(false);
			return 0;
		}

		virtual int Activate(const addon_data & data , const item_body * item, gactive_imp *pImp, const item* parent)
		{
			*(PARAM_TYPE*)((char*)pImp + OFFSET_IN_IMP) += *(PARAM_TYPE*)&(data.arg[0]);
			return 0;
		}

		virtual int Deactivate(const addon_data & data, const item_body *, gactive_imp *pImp, const item* parent)
		{
			*(PARAM_TYPE*)((char*)pImp + OFFSET_IN_IMP) -= *(PARAM_TYPE*)&(data.arg[0]);
			return 0;
		}

		virtual int ScaleActivate(const addon_data & data, const item_body *, gactive_imp *pImp, size_t l, const item* parent)
		{
			*(PARAM_TYPE*)((char*)pImp + OFFSET_IN_IMP) += *(PARAM_TYPE*)&(data.arg[0]) * l;
			return 0;
		}

		virtual int ScaleDeactivate(const addon_data & data, const item_body *, gactive_imp *pImp, size_t l, const item* parent)
		{
			*(PARAM_TYPE*)((char*)pImp + OFFSET_IN_IMP) -= *(PARAM_TYPE*)&(data.arg[0]) * l;
			return 0;
		}
};
namespace
{
/*	enum
	{
		ITEM_OFF = ((size_t)&(((gactive_imp*)100)->_cur_item)) - 100,
		POINT_OFF = ((size_t)&(((gactive_imp*)100)->_en_point)) - 100,
		PERCENT_OFF = ((size_t)&(((gactive_imp*)100)->_en_percent)) - 100,
	};
*/
	enum
	{
		ITEM_OFF = offsetof(gactive_imp , _cur_item),
		POINT_OFF = offsetof(gactive_imp , _en_point),
		PERCENT_OFF = offsetof(gactive_imp , _en_percent),
	};
}
//#define ACT_OFFSET(x) (((size_t)&(((gactive_imp*)100)->x)) - 100)

#define ACT_OFFSET(x) (offsetof(gactive_imp, x))

class enhance_skill_addon : public addon_handler
{
	public:
		virtual bool TransformData(addon_data & data)
		{
			return true;
		}

		virtual int UpdateItem(const addon_data & , equip_data *)
		{
			return addon_manager::ADDON_MASK_ACTIVATE;
		}

		virtual int Use(const addon_data & , const item_body *, gactive_imp *)
		{
			ASSERT(false);
			return 0;
		}

		virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.Upgrade(data.arg[0],data.arg[1],object_interface(pImp));
			return 0;
		}

		virtual int Deactivate(const addon_data & data, const item_body *, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.Degrade(data.arg[0],data.arg[1],object_interface(pImp));
			return 0;
		}

		virtual int ScaleActivate(const addon_data & data, const item_body * it, gactive_imp *pImp, size_t l, const item* parent)
		{
			return Activate(data , it, pImp, parent);
		}

		virtual int ScaleDeactivate(const addon_data & data, const item_body * it, gactive_imp *pImp, size_t l, const item* parent)
		{
			return Deactivate(data , it, pImp, parent);
		}
};

class enhance_skill_learn_addon : public addon_handler
{
public:
	virtual bool TransformData(addon_data & data)
	{
		return true;
	}
	virtual int UpdateItem(const addon_data & , equip_data *)
	{
		return addon_manager::ADDON_MASK_ACTIVATE;
	}

	virtual int Use(const addon_data & , const item_body *, gactive_imp *)
	{
		ASSERT(false);
		return 0;
	}

	virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
	{
		pImp->_skill.InsertSkill(data.arg[0],data.arg[1],object_interface(pImp));
		return 0;
	}

	virtual int Deactivate(const addon_data & data, const item_body *, gactive_imp *pImp, const item* parent)
	{
		pImp->_skill.RemoveSkill(data.arg[0],data.arg[1],object_interface(pImp));
		return 0;
	}

	virtual int ScaleActivate(const addon_data & data,const item_body *it,gactive_imp *pImp,size_t l, const item* parent)
	{
		return Activate(data , it, pImp, parent);
	}

	virtual int ScaleDeactivate(const addon_data & data,const item_body *it,gactive_imp *pImp, size_t l, const item* parent)
	{
		return Deactivate(data , it, pImp, parent);
	}
};

// Youshuang add
class trigger_skill_by_accident : public addon_handler
{
public:
	virtual bool TransformData(addon_data & data)
	{
		return true;
	}

	virtual int UpdateItem(const addon_data & , equip_data *)
	{
		return addon_manager::ADDON_MASK_ACTIVATE;
	}

	virtual int Use(const addon_data & , const item_body *, gactive_imp *)
	{
		ASSERT(false);
		return 0;
	}

	virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
	{
		gplayer_imp* pPlayerImp = dynamic_cast<gplayer_imp*>( pImp );
		const equip_item* pEquipment = dynamic_cast<const equip_item*>( it );
		if( pPlayerImp && pEquipment )
		{
			pPlayerImp->SetFashionWeaponAddon( pEquipment->GetFashionWeaponAddonID() );
		}
		return 0;
	}

	virtual int Deactivate(const addon_data & data, const item_body*, gactive_imp *pImp, const item* parent)
	{
		gplayer_imp* pPlayerImp = dynamic_cast<gplayer_imp*>( pImp );
		if( pPlayerImp )
		{
			pPlayerImp->SetFashionWeaponAddon( -1 );
		}
		return 0;
	}

	virtual int ScaleActivate(const addon_data & data,const item_body *it,gactive_imp *pImp,size_t l, const item* parent)
	{
		return Activate(data , it, pImp, parent);
	}

	virtual int ScaleDeactivate(const addon_data & data,const item_body *it,gactive_imp *pImp, size_t, const item* parent)
	{
		return Deactivate(data , it, pImp, parent);
	}
};
// end

class enhance_skill_stone_addon : public addon_handler
{
	public:
		virtual int UpdateItem(const addon_data & , equip_data *)
		{
			return addon_manager::ADDON_MASK_ACTIVATE;
		}

		virtual int Use(const addon_data & , const item_body *, gactive_imp *)
		{
			ASSERT(false);
			return 0;
		}

		virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.InsertSkill(data.arg[0],data.arg[1],object_interface(pImp));
			return 0;
		}

		virtual int Deactivate(const addon_data & data, const item_body*, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.RemoveSkill(data.arg[0],data.arg[1],object_interface(pImp));
			return 0;
		}

		virtual int ScaleActivate(const addon_data & data,const item_body *it,gactive_imp *pImp,size_t l, const item* parent)
		{
			return Activate(data , it, pImp, parent);
		}

		virtual int ScaleDeactivate(const addon_data & data,const item_body *it,gactive_imp *pImp, size_t, const item* parent)
		{
			return Deactivate(data , it, pImp, parent);
		}
};

class enhance_skill_permanent_addon : public addon_handler
{
public:
	virtual bool TransformData(addon_data & data)
	{
		return true;
	}

	virtual int UpdateItem(const addon_data & , equip_data *)
	{
		return addon_manager::ADDON_MASK_ACTIVATE;
	}

	virtual int Use(const addon_data & , const item_body *, gactive_imp *)
	{
		ASSERT(false);
		return 0;
	}

	virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
	{
		if(!parent) return 0;
		if(parent->expire_date > 0)
		{
			pImp->_skill.InsertSkill(data.arg[0],data.arg[1],object_interface(pImp));
		}
		else 
		{
			pImp->_skill.InsertSkillPermament(data.arg[0],data.arg[1],object_interface(pImp));
		}
		return 0;
	}

	virtual int Deactivate(const addon_data & data, const item_body*, gactive_imp *pImp, const item* parent)
	{
		if(!parent) return 0;
		if(parent->expire_date > 0)
		{
			pImp->_skill.RemoveSkill(data.arg[0],data.arg[1],object_interface(pImp));
		}
		return 0;
	}

	virtual int ScaleActivate(const addon_data & data,const item_body *it,gactive_imp *pImp,size_t l, const item* parent)
	{
		return Activate(data , it, pImp, parent);
	}

	virtual int ScaleDeactivate(const addon_data & data,const item_body *it,gactive_imp *pImp, size_t l, const item* parent)
	{
		return Deactivate(data , it, pImp, parent);
	}
};

class enhance_skill_cooldown_addon : public addon_handler
{
public:
		virtual bool TransformData(addon_data & data)
		{
			float t = *(float*)&data.arg[1];
			int t2 = (int)(t * 1000.f + 0.0001f);
			data.arg[1] = *(int*)&t2;
			return true;
		}
		
		virtual int UpdateItem(const addon_data & , equip_data *)
		{
			return addon_manager::ADDON_MASK_ACTIVATE;
		}

		virtual int Use(const addon_data & , const item_body *, gactive_imp *)
		{
			return 0;
		}

		virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.DecCoolTime(data.arg[0], data.arg[1]);
			return 0;
		}

		virtual int Deactivate(const addon_data & data, const item_body *, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.IncCoolTime(data.arg[0], data.arg[1]);
			return 0;
		}

		virtual int ScaleActivate(const addon_data & data, const item_body * it, gactive_imp *pImp, size_t l, const item* parent)
		{
			return Activate(data , it, pImp, parent);
		}

		virtual int ScaleDeactivate(const addon_data & data, const item_body * it, gactive_imp *pImp, size_t l, const item* parent)
		{
			return Deactivate(data , it, pImp, parent);
		}
};

class set_addon_handler : public addon_handler
{
	int _id;
	int _crit_value;
	addon_handler * _imp;
	
public:
	set_addon_handler(int id, int critical_value, addon_handler * handle)
		:_id(id), _crit_value(critical_value),_imp(handle)
	{
		ASSERT(critical_value >= 2);
	}

	virtual int UpdateItem(const addon_data & , equip_data *)
	{
		return addon_manager::ADDON_MASK_ACTIVATE;
	}

	virtual int Use(const addon_data & , const item_body *, gactive_imp *)
	{
		return 0;
	}
	virtual int Activate(const addon_data & a, const item_body *e, gactive_imp *pImp, const item* parent)
	{
		if(pImp->ActivateSetAddon(_id) == _crit_value)
		{
			return _imp->Activate(a, e, pImp, parent);
		}
		return 0;
	}

	virtual int Deactivate(const addon_data & a, const item_body *e, gactive_imp *pImp, const item* parent)
	{
		if(pImp->DeactivateSetAddon(_id) == (_crit_value - 1))
		{
			return _imp->Deactivate(a, e, pImp, parent);
		}
		return 0;
	}

	virtual int ScaleActivate(const addon_data & a, const item_body *e, gactive_imp *g, size_t l, const item* parent)
	{
		return Activate(a,e,g,parent);
	}

	virtual int ScaleDeactivate(const addon_data & a, const item_body *e, gactive_imp *g, size_t l, const item* parent)
	{
		return Deactivate(a,e,g,parent);
	}
	virtual bool TransformData(addon_data & data)
	{
		return _imp->TransformData(data);
	}
};

class talisman_autobot_addon: public addon_handler
{
	public:
		virtual int UpdateItem(const addon_data & , equip_data *)
		{
			// 在装备的时候生效记录激活bot时该挂的buff和addon
			return addon_manager::ADDON_MASK_ACTIVATE;
		}
		virtual int Use(const addon_data & , const item_body *, gactive_imp *)
		{
			ASSERT(false);
			return 0;
		}

		virtual bool TransformData(addon_data & data)
		{
			return true;
		}

		virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
		{
			// 这里记录激活bot时该挂的buff和addon 到imp中
			pImp->SetTalismanAutobotArg( data );
			return 0;
		}
		virtual int Deactivate(const addon_data & data, const item_body*, gactive_imp *pImp, const item* parent)
		{
			// 这里清理
			pImp->ClearTalismanAutobotArg( data );
			return 0;
		}
		bool IsTalismanAddonHandler() { return true; }
};

class enhance_skill_damage_addon : public addon_handler
{
public:
		virtual bool TransformData(addon_data & data)
		{
			return true;
		}
		
		virtual int UpdateItem(const addon_data & , equip_data *)
		{
			return addon_manager::ADDON_MASK_ACTIVATE;
		}

		virtual int Use(const addon_data & , const item_body *, gactive_imp *)
		{
			return 0;
		}

		virtual int Activate(const addon_data & data , const item_body * it, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.WeaponAddon(data.arg[0], data.arg[1], data.arg[2]);
			return 0;
		}

		virtual int Deactivate(const addon_data & data, const item_body *, gactive_imp *pImp, const item* parent)
		{
			pImp->_skill.WeaponAddon(data.arg[0], -data.arg[1], -data.arg[2]);
			return 0;
		}

		virtual int ScaleActivate(const addon_data & data, const item_body * it, gactive_imp *pImp, size_t l, const item* parent)
		{
			return Activate(data , it, pImp, parent);
		}

		virtual int ScaleDeactivate(const addon_data & data, const item_body * it, gactive_imp *pImp, size_t l, const item* parent)
		{
			return Deactivate(data , it, pImp, parent);
		}
};

typedef DataTransform<float,float,VALUE,__XT(1.0f)>  VA_DT1_0;
typedef DataTransform<float,float,VALUE,__XT(1.5f)>  VA_DT1_5;
typedef DataTransform<float,float,VALUE,__XT(2.0f)>  VA_DT2_0;
typedef DataTransform<float,float,VALUE,__XT(3.0f)>  VA_DT3_0;

typedef DataTransform<int,float,SCALE,__XT(100)>  SC_DT10;
typedef DataTransform<int,float,SCALE,__XT(150)>  SC_DT15;
typedef DataTransform<int,float,SCALE,__XT(200)>  SC_DT20;
typedef DataTransform<int,float,SCALE,__XT(300)>  SC_DT30;
typedef DataTransform<int,float,SCALE,__XT(1000)>  SC_DT100;
typedef DataTransform<int,float,SCALE,__XT(500)>  SC_DT50;

typedef DataTransform<int ,int ,VALUE,__XT(1000)> VA_DT1000;
typedef DataTransform<int ,int ,VALUE,__XT(1200)> VA_DT1200;
typedef DataTransform<int ,int ,VALUE,__XT(2000)> VA_DT2000;
typedef DataTransform<int ,int ,VALUE,__XT(5000)> VA_DT5000;
typedef DataTransform<int ,int ,VALUE,__XT(10000)> VA_DT10000;
typedef DataTransform<int ,int ,VALUE,__XT(30000)> VA_DT30000;

typedef EPSA_addon<int, ACT_OFFSET(_exp_addon), SC_DT15> enhance_exp_addon;
typedef EPSA_addon<int, ACT_OFFSET(_money_addon), SC_DT15> enhance_money_addon;

typedef EPSA_addon<int, ITEM_OFF+offsetof(q_item_prop,max_hp), VA_DT30000> enhance_hp_addon;
typedef EPSA_addon<int, ITEM_OFF+offsetof(q_item_prop,max_mp), VA_DT30000> enhance_mp_addon;
typedef EPSA_addon<int, ITEM_OFF+offsetof(q_item_prop,max_dp), VA_DT30000> enhance_dp_addon;
typedef EPSA_addon<int, ITEM_OFF+offsetof(q_item_prop,defense), VA_DT5000> enhance_defense_addon;
typedef EPSA_addon<int, ITEM_OFF+offsetof(q_item_prop,attack), VA_DT10000> enhance_attack_addon;
typedef EPSA_addon<int, ITEM_OFF+offsetof(q_item_prop,armor)> enhance_armor_addon;

typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,damage), VA_DT10000> enhance_damage_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,crit_rate), SC_DT30> enhance_crit_addon;
typedef EPSA_addon<float, POINT_OFF+offsetof(q_enhanced_param,crit_damage),VA_DT2_0> enhance_crit_damage_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance[0])> enhance_res_0_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance[1])> enhance_res_1_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance[2])> enhance_res_2_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance[3])> enhance_res_3_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance[4])> enhance_res_4_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance[5])> enhance_res_5_addon;
typedef EPSA_addon<float, POINT_OFF+offsetof(q_enhanced_param,run_speed),VA_DT1_0>   enhance_runspeed_addon;
typedef EPSA_addon<float, POINT_OFF+offsetof(q_enhanced_param,mount_speed),VA_DT2_0>   enhance_mountspeed_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,spec_damage), VA_DT2000> enhance_spec_damage_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,dmg_reduce)> enhance_dmg_reduce_addon;
typedef EPSA_addon<float, POINT_OFF+offsetof(q_enhanced_param,flight_speed), VA_DT3_0> enhance_fly_speed_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,anti_crit), SC_DT30> enhance_anti_crit_addon;
typedef EPSA_addon<float, POINT_OFF+offsetof(q_enhanced_param,anti_crit_damage), VA_DT2_0> enhance_anti_crit_dmg_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,skill_attack_rate), SC_DT100> enhance_skill_attack_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,skill_armor_rate), SC_DT100> enhance_skill_armor_addon;

typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,max_hp),SC_DT50> enhance_sacle_hp_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,max_mp),SC_DT50> enhance_sacle_mp_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,max_dp),SC_DT50> enhance_sacle_dp_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,attack),SC_DT50> enhance_sacle_attack_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,defense),SC_DT50> enhance_sacle_defense_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,armor),SC_DT30> enhance_sacle_armor_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,damage),SC_DT50> enhance_sacle_damage_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,resistance[0]),SC_DT20> enhance_scale_res_0_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,resistance[1]),SC_DT20> enhance_scale_res_1_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,resistance[2]),SC_DT20> enhance_scale_res_2_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,resistance[3]),SC_DT20> enhance_scale_res_3_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,resistance[4]),SC_DT20> enhance_scale_res_4_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,resistance[5]),SC_DT20> enhance_scale_res_5_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,hp_gen),SC_DT100> enhance_scale_hpgen_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,mp_gen),SC_DT100> enhance_scale_mpgen_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param,dmg_reduce),SC_DT50> enhance_scale_dmg_reduce_addon;

typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_defense[0]),SC_DT50> enhance_cult_defense_0_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_defense[1]),SC_DT50> enhance_cult_defense_1_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_defense[2]),SC_DT50> enhance_cult_defense_2_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_defense[3]),SC_DT50> enhance_cult_defense_3_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_defense[4]),SC_DT50> enhance_cult_defense_4_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_defense[5]),SC_DT50> enhance_cult_defense_5_addon;

typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,pet_attack_adjust),SC_DT50> enhance_pet_attack_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,pet_attack_defense),SC_DT50> enhance_pet_defense_addon;

typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,anti_diet),SC_DT50> enhance_anti_diet_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param, ign_dmg_reduce),SC_DT50> enhance_scale_ign_dmg_reduce_addon;
typedef EPSA_addon<int, PERCENT_OFF+offsetof(q_scale_enhanced_param, dmg_change),SC_DT50> enhance_scale_dmg_change_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_attack[0]),SC_DT50> enhance_cult_attack_0_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_attack[1]),SC_DT50> enhance_cult_attack_1_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,cult_attack[2]),SC_DT50> enhance_cult_attack_2_addon;

typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_proficiency[0])> enhance_res_pro_0_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_proficiency[1])> enhance_res_pro_1_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_proficiency[2])> enhance_res_pro_2_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_proficiency[3])> enhance_res_pro_3_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_proficiency[4])> enhance_res_pro_4_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_proficiency[5])> enhance_res_pro_5_addon;

typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_tenaciy[0])> enhance_res_ten_0_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_tenaciy[1])> enhance_res_ten_1_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_tenaciy[2])> enhance_res_ten_2_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_tenaciy[3])> enhance_res_ten_3_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_tenaciy[4])> enhance_res_ten_4_addon;
typedef EPSA_addon<int, POINT_OFF+offsetof(q_enhanced_param,resistance_tenaciy[5])> enhance_res_ten_5_addon;

bool InitAllAddon()
{
	INSERT_ADDON(1 , enhance_hp_addon);
	INSERT_ADDON(2 , enhance_mp_addon);
	INSERT_ADDON(3 , enhance_damage_addon);
	INSERT_ADDON(4 , enhance_defense_addon);
	INSERT_ADDON(5 , enhance_attack_addon);
	INSERT_ADDON(6 , enhance_armor_addon);
	INSERT_ADDON(7 , enhance_crit_addon);
	INSERT_ADDON(8 , enhance_crit_damage_addon);
	INSERT_ADDON(9 , enhance_res_0_addon);
	INSERT_ADDON(10, enhance_res_1_addon);
	INSERT_ADDON(11, enhance_res_2_addon);
	INSERT_ADDON(12, enhance_res_3_addon);
	INSERT_ADDON(13, enhance_res_4_addon);
	INSERT_ADDON(14, enhance_res_5_addon);
	INSERT_ADDON(15, enhance_sacle_hp_addon);
	INSERT_ADDON(16, enhance_sacle_mp_addon);
	INSERT_ADDON(17, enhance_sacle_damage_addon);
	INSERT_ADDON(18, enhance_sacle_defense_addon);
	INSERT_ADDON(19, enhance_scale_res_0_addon);
	INSERT_ADDON(20, enhance_scale_res_1_addon);
	INSERT_ADDON(21, enhance_scale_res_2_addon);
	INSERT_ADDON(22, enhance_scale_res_3_addon);
	INSERT_ADDON(23, enhance_scale_res_4_addon);
	INSERT_ADDON(24, enhance_scale_res_5_addon);
	INSERT_ADDON(25, enhance_runspeed_addon);
	INSERT_ADDON(26, enhance_scale_hpgen_addon);
	INSERT_ADDON(27, enhance_scale_mpgen_addon);

	INSERT_ADDON(29,enhance_exp_addon );	//exp 
	INSERT_ADDON(31,enhance_money_addon );	//money

	INSERT_ADDON(32, enhance_dmg_reduce_addon);
	INSERT_ADDON(33, enhance_scale_dmg_reduce_addon);
	INSERT_ADDON(34, enhance_skill_addon);
	INSERT_ADDON(35, enhance_skill_cooldown_addon);
	INSERT_ADDON(36, enhance_spec_damage_addon);

	INSERT_ADDON(37, enhance_mountspeed_addon);
	INSERT_ADDON(38, enhance_skill_learn_addon);

	INSERT_ADDON( 39, talisman_autobot_addon );

	INSERT_ADDON( 40, enhance_fly_speed_addon );

	INSERT_ADDON( 41, enhance_anti_crit_addon);
	INSERT_ADDON( 42, enhance_anti_crit_dmg_addon);
	INSERT_ADDON( 43, enhance_skill_armor_addon);
	INSERT_ADDON( 44, enhance_skill_attack_addon);
	INSERT_ADDON( 45, enhance_skill_damage_addon);

	INSERT_ADDON( 47, enhance_cult_defense_0_addon);
	INSERT_ADDON( 48, enhance_cult_defense_1_addon);
	INSERT_ADDON( 49, enhance_cult_defense_2_addon);
	INSERT_ADDON( 50, enhance_cult_defense_3_addon);
	INSERT_ADDON( 51, enhance_cult_defense_4_addon);
	INSERT_ADDON( 52, enhance_cult_defense_5_addon);
	INSERT_ADDON( 53, enhance_pet_attack_addon);
	INSERT_ADDON( 54, enhance_pet_defense_addon);
	INSERT_ADDON( 55, enhance_anti_diet_addon);
	INSERT_ADDON( 56, enhance_scale_ign_dmg_reduce_addon);
	INSERT_ADDON( 57, enhance_scale_dmg_change_addon);
	INSERT_ADDON( 58, enhance_cult_attack_0_addon);
	INSERT_ADDON( 59, enhance_cult_attack_1_addon);
	INSERT_ADDON( 60, enhance_cult_attack_2_addon);
	INSERT_ADDON( 61, enhance_skill_permanent_addon);
	INSERT_ADDON( 62, trigger_skill_by_accident );	// Youshuang add
	INSERT_ADDON( 63, enhance_dp_addon);
	INSERT_ADDON( 64, enhance_sacle_dp_addon);

	INSERT_ADDON(65, enhance_res_pro_0_addon);
	INSERT_ADDON(66, enhance_res_pro_1_addon);
	INSERT_ADDON(67, enhance_res_pro_2_addon);
	INSERT_ADDON(68, enhance_res_pro_3_addon);
	INSERT_ADDON(69, enhance_res_pro_4_addon);
	INSERT_ADDON(70, enhance_res_pro_5_addon);

	INSERT_ADDON(71, enhance_res_ten_0_addon);
	INSERT_ADDON(72, enhance_res_ten_1_addon);
	INSERT_ADDON(73, enhance_res_ten_2_addon);
	INSERT_ADDON(74, enhance_res_ten_3_addon);
	INSERT_ADDON(75, enhance_res_ten_4_addon);
	INSERT_ADDON(76, enhance_res_ten_5_addon);

	INSERT_ADDON(ITEM_ADDON_SKILL_STONE,enhance_skill_stone_addon );	//skill_stone
	return true;
}

bool InsertSetAddon(int id ,int critical_value, int origin_addon)
{
	addon_handler * handler = addon_manager::QueryHandler(origin_addon);
	if(handler == NULL) return false;
	INSERT_ADDON_INSTANCE(id,new set_addon_handler(id,critical_value, handler) );
	return true;
}

