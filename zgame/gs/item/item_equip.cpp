#include "item_equip.h"
#include "../clstab.h"
#include "../actobject.h"
#include "../item_list.h"
#include "../item_manager.h"
#include "../player_imp.h"
#include "../gmatrix.h"
#include "../global_table.h"
#include "../topic_site.h"
#include <wchar.h>
        
DEFINE_SUBSTANCE(equip_item,item_body,CLS_ITEM_EQUIP)

float equip_item::gem_shape_prob[] = 
{
	0.0f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.083f,
	0.087f,
};

// Youshuang add, equipment version control
void equip_item::FillOldEquipEssence( const equip_essence& old_ess, equip_essence_new& new_ess ) const
{
	new_ess.rf_stone = old_ess.rf_stone;
	new_ess.rf_stone_level = old_ess.rf_stone_level;
	new_ess.pr_stone = old_ess.pr_stone;
	new_ess.pr_stone_level = old_ess.pr_stone_level;
	new_ess.sk_stone = old_ess.sk_stone;
	new_ess.sk_stone_level = old_ess.sk_stone_level;
}

void equip_item::FillSpiritEquipEssence( const equip_essence_spirit& spirit_ess, equip_essence_new& new_ess ) const
{
	FillOldEquipEssence( spirit_ess.old_essence, new_ess );
	new_ess.master_id = spirit_ess.master_id;
	memcpy( new_ess.master_name, spirit_ess.master_name, 20 );
	new_ess.cur_spirit_power = spirit_ess.cur_spirit_power;
	new_ess.max_spirit_power = spirit_ess.max_spirit_power;
	new_ess.soul_used_spirit = spirit_ess.soul_used_spirit;
	memcpy( new_ess.soul, spirit_ess.soul, MAX_ENCHANT_SLOT * sizeof( int ) );
	new_ess.soul_rune_word = spirit_ess.soul_rune_word;
}

void equip_item::FillGemEquipEssence( const equip_essence_gem& gem_ess, equip_essence_new& new_ess ) const
{
	FillSpiritEquipEssence( gem_ess.spirit_essence, new_ess );
	memcpy( new_ess.gem_slot_shape, gem_ess.gem_slot_shape, MAX_GEM_SLOT );
	memcpy( new_ess.gem_embeded, gem_ess.gem_embeded, MAX_GEM_SLOT * sizeof( int ) );
	new_ess.colorant_id = gem_ess.colorant_id;
}

void equip_item::FillAstrologyEquipEssence( const equip_essence_astrology& astrology_ess, equip_essence_new& new_ess ) const
{
	FillGemEquipEssence( astrology_ess.gem_essence, new_ess );
	new_ess.cur_hole = astrology_ess.cur_hole;
	memcpy( new_ess.hole, astrology_ess.hole, 3 * sizeof( astrology_slot ) );
}

void equip_item::ResetEquipEssence( item* parent ) const
{
	if( !parent ){ return; }
	size_t len;
	void* buf = parent->GetContent( len );
	if( len == sizeof( equip_essence_new ) ){ return; }
	
	equip_essence_new result_essence;
	memset( &result_essence, 0, sizeof(result_essence));
	if( len == sizeof( equip_essence ) )
	{
		result_essence.version = EQUIP_ESSENCE_VERSION_OLD;
		FillOldEquipEssence( *( (equip_essence*)buf ), result_essence );
	}
	else if( len == sizeof( equip_essence_spirit ) )
	{
		result_essence.version = EQUIP_ESSENCE_VERSION_SPIRIT;
		FillSpiritEquipEssence( *( (equip_essence_spirit*)buf ), result_essence );
	}
	else if( len == sizeof( equip_essence_gem ) )
	{
		result_essence.version = EQUIP_ESSENCE_VERSION_GEM;
		FillGemEquipEssence( *( (equip_essence_gem*)buf ), result_essence );
	}
	else if( len == sizeof( equip_essence_astrology ) )
	{
		result_essence.version = EQUIP_ESSENCE_VERSION_ASTROLOGY;
		FillAstrologyEquipEssence( *( (equip_essence_astrology*)buf ), result_essence );
	}
	else
	{
		return;
	}
	parent->SetContent( &result_essence, sizeof( result_essence ) );
}

void equip_item::Empty2OldEssence( item* parent ) const
{
        if( IsEmptyEssence(parent) )
        {       
                equip_essence_new ess; 
                memset(&ess, 0, sizeof(ess));
                ess.version = EQUIP_ESSENCE_VERSION_OLD;
		parent->SetContent( &ess, sizeof( ess ) );
	} 
}

equip_essence_new* equip_item::GetOldDataEssence( item * parent, int version ) const
{
	size_t len = 0;
	void* buf = parent->GetContent( len );
	if( len != sizeof( equip_essence_new ) ){ return NULL; }
	equip_essence_new* ess = (equip_essence_new*)buf;
	return ( ess->version >= version ) ? ess : NULL;
}

const equip_essence_new* equip_item::GetOldDataEssence( const item * parent, int version ) const
{
	size_t len = 0;
	const void* buf = parent->GetContent( len );
	if( len != sizeof( equip_essence_new ) ){ return NULL; }
	const equip_essence_new* ess = (const equip_essence_new*)buf;
	return ( ess->version >= version ) ? ess : NULL;
}

equip_essence_new* equip_item::GetSpiritDataEssence(item* parent) const
{
	return GetOldDataEssence( parent, EQUIP_ESSENCE_VERSION_SPIRIT );
}

const equip_essence_new* equip_item::GetSpiritDataEssence(const item* parent) const
{
	return GetOldDataEssence( parent, EQUIP_ESSENCE_VERSION_SPIRIT );
}

equip_essence_new* equip_item::GetGemDataEssence(item* parent) const
{
	return GetOldDataEssence( parent, EQUIP_ESSENCE_VERSION_GEM );
}

const equip_essence_new* equip_item::GetGemDataEssence(const item* parent) const
{
	return GetOldDataEssence( parent, EQUIP_ESSENCE_VERSION_GEM );
}

equip_essence_new* equip_item::GetAstrologyDataEssence(item* parent) const
{
	return GetOldDataEssence( parent, EQUIP_ESSENCE_VERSION_ASTROLOGY );
}

const equip_essence_new* equip_item::GetAstrologyDataEssence(const item* parent) const
{
	return GetOldDataEssence( parent, EQUIP_ESSENCE_VERSION_ASTROLOGY );
}

bool equip_item::IsEmptyEssence(const item * parent) const 
{
	size_t len = 0;
	const equip_essence_new* ess = ( const equip_essence_new* )parent->GetContent( len );
	return ( len == 0 ) || !ess;
}

bool equip_item::IsOldEssence(const item * parent) const 
{
	size_t len = 0;
	const equip_essence_new* ess = ( const equip_essence_new* )parent->GetContent( len );
	return ( (len == sizeof( equip_essence_new )) && ess && (ess->version == EQUIP_ESSENCE_VERSION_OLD) );
}

bool equip_item::IsSpiritEssence(const item * parent) const 
{
	size_t len = 0;
	const equip_essence_new* ess = ( const equip_essence_new* )parent->GetContent( len );
	return ( (len == sizeof( equip_essence_new )) && ess && (ess->version == EQUIP_ESSENCE_VERSION_SPIRIT) );
}

bool equip_item::IsGemEssence(const item * parent) const
{
	size_t len = 0;
	const equip_essence_new* ess = ( const equip_essence_new* )parent->GetContent( len );
	return ( (len == sizeof( equip_essence_new )) && ess && (ess->version == EQUIP_ESSENCE_VERSION_GEM) );
}

bool equip_item::IsAstrologyEssence(const item * parent) const
{
	size_t len = 0;
	const equip_essence_new* ess = ( const equip_essence_new* )parent->GetContent( len );
	return ( (len == sizeof( equip_essence_new )) && ess && (ess->version == EQUIP_ESSENCE_VERSION_ASTROLOGY) );
}

bool equip_item::IsBloodEnchanted(const item* parent) const
{
	const equip_essence_new* ess = GetOldDataEssence( parent );
	if( !ess ){ return false; }
	if( ess->version == EQUIP_ESSENCE_VERSION_SPIRIT )
	{
		return true;
	}
	else if( ( ess->version == EQUIP_ESSENCE_VERSION_GEM ) && ( ess->master_id > 0 ) )
	{
		return true;
	}
	else if( ( ess->version == EQUIP_ESSENCE_VERSION_ASTROLOGY ) && ( ess->master_id > 0 ) )
	{
		return true;
	}
	return false;
}

bool equip_item::IsIdentifyGem(const item * parent) const
{
	const equip_essence_new* ess = GetOldDataEssence( parent );
	if( !ess ){ return false; }
	if( (ess->version == EQUIP_ESSENCE_VERSION_GEM) || (ess->version == EQUIP_ESSENCE_VERSION_ASTROLOGY) )
	{
		for(size_t i = 0; i < MAX_GEM_SLOT; ++i)
		{
			if( ess->gem_slot_shape[i] != -1 ){ return true; }
		}
	}
	return false;
}

int equip_item::GetCurHole(item * parent)
{
	const equip_essence_new* ess = GetOldDataEssence( parent );
	if( !ess ){ return false; }
	if( ess->version == EQUIP_ESSENCE_VERSION_ASTROLOGY )
	{
		return ess->cur_hole;
	}
	return 0;
}
// end

void equip_item::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	ResetEquipEssence( parent );
	//ĞŞ¸´Á¶Æ÷µÈ¼¶Îª-1µÄbug	
	equip_essence_new* ess = GetOldDataEssence( parent );
	if( !ess ){ return; }
	if(ess->rf_stone_level < 0)
	{
		ess->rf_stone_level = 0;
		ess->rf_stone = 0;
	}
	if(ess->pr_stone_level < 0)
	{
		ess->pr_stone_level = 0;
		ess->pr_stone = 0;
	}
	
	//ĞŞ¸ÄÓÉÓÚ¸ÄÃû¿ÉÄÜµ¼ÖÂµÄ×°±¸Ãû×Ö²»¶Ô (±ÈÈçÍæ¼Ò×°±¸ÏÈ½»Ò×³öÈ¥ÔÙÄÃ»ØÀ´)
	gplayer_imp * pImp= (gplayer_imp *)obj;
	const  equip_essence_new* pNewEss = GetSpiritDataEssence(parent);
	if( !pNewEss ){ return; }
	size_t name_len;
	const void * name = pImp->GetPlayerName(name_len);
	if(pNewEss->master_id == obj->_parent->ID.id && memcmp(pNewEss->master_name, name, MAX_USERNAME_LENGTH) != 0)
	{
		OnChangeOwnerName(item::BODY, index, obj, parent);
	}
}

void equip_item::OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
			Deactivate(index,obj,parent);
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

bool equip_item::VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const
{
	if(list.GetLocation() == item::BODY)
	{
		//ÅĞ¶Ï×ªÉú
		gplayer_imp * pImp = (gplayer_imp *)obj;
		if(_data.require_reborn_count > 0 )
		{
			int reborn_count = pImp->GetRebornCount();
			if(_data.require_reborn_count > reborn_count ) return false;

			ASSERT(reborn_count <= MAX_REBORN_COUNT);
		}
		//ÅĞ¶ÏÉùÍû
		if(_data.require_rep_idx1 >= 0 && _data.require_rep_val1 > 0 && pImp->GetRegionReputation(_data.require_rep_idx1) < _data.require_rep_val1)
		{
			return false;
		}
		if(_data.require_rep_idx2 >= 0 && _data.require_rep_val2 > 0 && pImp->GetRegionReputation(_data.require_rep_idx2) < _data.require_rep_val2)
		{
			return false;
		}
		//ÅĞ¶Ï³ÆºÅ
		if(_data.require_title && !pImp->CheckPlayerTitle(_data.require_title))
		{
			return false;
		}
		//ÅĞ¶ÏÏÉÄ§·ğ
		if(_data.require_cultivation && !(pImp->GetCultivation() & _data.require_cultivation))
		{
			return false;
		}
		//ÅĞ¶ÏÁìÍÁ»ı·Ö
		if(_data.require_territory && gmatrix::GetTerritoryScore(pImp->OI_GetMafiaID()) < _data.require_territory)
		{
			return false;
		}
		//¿¼ÂÇ¹ú¼ÒÖ°Î»
		if(_data.require_kingdom_title && (pImp->GetKingdomTitle() <= 0 || !(1 << (pImp->GetKingdomTitle()-1) & _data.require_kingdom_title)))
		{
			return false;
		}	
		//ÅĞ¶ÏÊÇ·ñµÎÑªÈÏÖ÷
		if( const equip_essence_new* pNewEss = GetSpiritDataEssence(parent) )
		{
			int id = pNewEss->master_id;
			if(id != 0 && id != obj->_parent->ID.id) return false;
		}
		if( const equip_essence_new * ess = GetOldDataEssence(parent) )
		{
			if(ess->pr_stone)
			{
				//ÓĞ×·¼ÓÊ¯Í·£¬ÓÃ¸½¼ÓÊôĞÔÀïµÄÅĞ¶¨À´½øĞĞ
				item_body * pItem = item_manager::GetInstance().GetItemBody(ess->pr_stone);
				int level = obj->GetObjectLevel();
				int level_require = _data.level_required;
				int64_t cls = ((obj->GetObjectClass() <= 0x3F) ? ((int64_t)1 << (obj->GetObjectClass() & 0x7F)) : ((int64_t)1 << ((obj->GetObjectClass() & 0x7F) - 0x40)));
				int64_t cls_require = _data.class_required;
				int64_t cls_require1 = _data.class_required1;
				if(pItem)
				{
					pItem->ModifyRequirement(level,level_require,cls,cls_require,cls_require1, ess->pr_stone_level);
				}
				return  level >= level_require
					&& ( (obj->GetObjectClass() <= 0x3F) ? (cls & cls_require) : (cls & cls_require1))
					&& ( (obj->IsObjectFemale()?0x02:0x01) & _data.require_gender);
			}
		}
		return  obj->GetObjectLevel() >= _data.level_required
			&& object_base_info::CheckCls(obj->GetObjectClass(), _data.class_required, _data.class_required1)
			&& ( (obj->IsObjectFemale()?0x02:0x01) & _data.require_gender);
	
	}
	return false;
}

int equip_item::CollectSpiritEffect(const equip_essence_new & ess, int * pData, int dataCount) const
{
	if(dataCount <= 0) return 0;
	itemdataman & dataman = gmatrix::GetDataMan();
	abase::vector<int, abase::fast_alloc<> > list;
	list.reserve(MAX_ENCHANT_SLOT);
	//Ê×ÏÈÊÇÆ÷ÆÇ±¾ÉíĞ§¹û
	int cur_count = 0;

	int soul_eid = 0;
	int soul_type = 0;
	int soul_level_min = 0;
	bool scan_soul_add = true;

	for(size_t i =0; i < MAX_ENCHANT_SLOT; i ++)
	{
		int id = ess.soul[i] ;
		if(id <= 0) continue;
		DATA_TYPE dt;
		const EQUIP_SOUL_ESSENCE& eess = *(const EQUIP_SOUL_ESSENCE*)dataman.get_data_ptr(ess.soul[i],ID_SPACE_ESSENCE,dt);
		if(!&eess || dt != DT_EQUIP_SOUL_ESSENCE) continue;
		list.push_back(id);
		pData[cur_count] = eess.id_addon1;
		cur_count ++;
		if(cur_count >= dataCount) return cur_count;

		if (i == 0)
		{
			soul_eid = id;
			soul_type = eess.soul_type;
			soul_level_min = eess.soul_level;
		}
		else if (scan_soul_add)
		{
			if (soul_type == eess.soul_type)
			{
				if (soul_level_min > eess.soul_level)
				{
					soul_level_min = eess.soul_level;
					soul_eid = id;
				}
			}
			else
			{
				scan_soul_add = false;
			}
		}
	}
	if(list.size() != MAX_ENCHANT_SLOT || !scan_soul_add) return cur_count;
	
	//¿ªÊ¼Ñ°ÕÒaddon
	return cur_count + item_manager::GetSpiritAddonSet(ess.soul_rune_word, soul_eid, pData + cur_count, dataCount - cur_count);
}

void equip_item::OnActivate(size_t index,gactive_imp* obj,item * parent) const
{
	if( !IsFashionWeapon() )  // Youshuang add
	{
		obj->EquipItemEnhance2(_data);
		if(_data.equip_type == 0) obj->WeaponItemEnhance(_data);
	}
	for(size_t i = 0;i < _addon_list.size(); i ++)
	{
		const addon_data_spec & spec = _addon_list[i];
		spec.handler->Activate(spec.data,this,obj,parent);
	}

	if( IsFashionWeapon() ){ return; }  // Youshuang add

	if(const equip_essence_new * ess = GetOldDataEssence(parent))
	{
		class __foo //¼¤»î Ê¯Í·µÄÊôĞÔ
		{
		public:
			static inline void reinforce(int type, size_t index, const equip_item * eq,const equip_essence_new & value,gactive_imp * obj, const item* parent, float adjust_ratio = 0.f, int adjust_point =0)
			{
				if(type)
				{
					item_body * pItem = item_manager::GetInstance().GetItemBody(type);
					if(pItem) pItem->ReinforceActivate(index, eq, value, obj, adjust_ratio, adjust_point, parent);
				}
			}
		};

		float adjust_ratio = 0.f;
		int  adjust_point = 0;
		//Modified by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
		const equip_essence_new * newess = GetSpiritDataEssence(parent);
		if(newess && newess->master_id > 0 && newess->cur_spirit_power) 
		{
			CheckRefineEffect(*newess, adjust_ratio, adjust_point);

			//¼ÓÈëÆ÷ÆÇÊôĞÔ ²¢¿¼ÂÇÆ÷ÆÇ×éºÏ
			int addon[32];
			int count = CollectSpiritEffect(*newess, addon, sizeof(addon)/sizeof(int));
			if(count > 0)
			{
				addon_data_man::Instance().ActivateAddonList(addon, count, this, obj, parent);
			}
		}
		
		//Add by houjun 2011-03-18, ¼¤»î±¦Ê¯ÊôĞÔ
		const equip_essence_new* gemess = GetGemDataEssence(parent);
		if(gemess)
		{
			ActiveGemEffect(obj, *gemess, parent);
		}

		__foo::reinforce(ess->rf_stone,index,this,*ess,obj, parent, adjust_ratio, adjust_point);
		__foo::reinforce(ess->pr_stone,index,this,*ess,obj, parent);

		if (ess->sk_stone) ActivateAddonByID(ess->sk_stone, obj, true, parent);
		if (ess->sk_stone_level) ActivateAddonByID(ess->sk_stone_level, obj, true, parent);
		if (ess->rf_stone_level > 0) ActiveHiddenAddon(ess->rf_stone_level, obj, true, parent);

	}

	gplayer_imp * pImp= (gplayer_imp *)obj;
	if(IsAstrologyEssence(parent) && pImp->IsAstrologyActive())
	{
		ActiveAstrologyProp(parent, obj);
	}
}


void equip_item::ActiveAstrologyProp(item * parent, gactive_imp * obj) const
{
	equip_essence_new * pEss = GetAstrologyDataEssence(parent);
	if(!pEss) return;

	for(int i = 0; i < pEss->cur_hole; ++i)
	{
		for(int j = 0; j < MAX_ASTROLOGY_PROP_NUM; ++j)
		{
			int level = pEss->hole[i].level;
			int prop_id = pEss->hole[i].prop[j].id;
			int value = pEss->hole[i].prop[j].value;

			int total_value = value + (int)(value * 0.25 * level); 

			object_interface oif(obj);
			oif.EnhanceProp(prop_id, total_value);
		}

	}	
}


void equip_item::DeactiveAstrologyProp(item * parent, gactive_imp * obj) const
{
	equip_essence_new * pEss = GetAstrologyDataEssence(parent);
	if(!pEss) return;
	
	for(int i = 0; i < pEss->cur_hole; ++i)
	{
		for(int j = 0; j < MAX_ASTROLOGY_PROP_NUM; ++j)
		{
			int level = pEss->hole[i].level;
			int prop_id = pEss->hole[i].prop[j].id;
			int value = pEss->hole[i].prop[j].value;

			int total_value = value + int(value * 0.25 * level); 

			object_interface oif(obj);
			oif.ImpairProp(prop_id, total_value);
		}

	}	
}

void equip_item::OnDeactivate(size_t index,gactive_imp* obj, item * parent) const
{
	if( !IsFashionWeapon() ) // Youshuang
	{
		obj->EquipItemImpair2(_data);
		if(_data.equip_type == 0) obj->WeaponItemImpair(_data);
	}
	for(size_t i = 0;i < _addon_list.size(); i ++)
	{
		const addon_data_spec & spec = _addon_list[i];
		spec.handler->Deactivate(spec.data,this,obj,parent);
	}

	if( IsFashionWeapon() ){ return; }  // Youshuang add
	if(const equip_essence_new * ess = GetOldDataEssence(parent))
	{
		class __foo //È¡Ïû Ê¯Í·µÄÊôĞÔ
		{

		public:
			static inline void reinforce(int type, size_t index, const equip_item * eq,const equip_essence_new & value,gactive_imp * obj, const item* parent, float adjust_ratio = 0.f, int adjust_point = 0)
			{
				if(type)
				{
					item_body * pItem = item_manager::GetInstance().GetItemBody(type);
					if(pItem) pItem->ReinforceDeactivate(index, eq, value, obj, adjust_ratio, adjust_point, parent);
				}
			}
		};

		float adjust_ratio = 0.f;
		int adjust_point = 0;

		//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
		const equip_essence_new * newess = GetSpiritDataEssence(parent);
		if(newess && newess->master_id > 0 && newess->cur_spirit_power)  
		{
			CheckRefineEffect(*newess, adjust_ratio, adjust_point);

			//¼ÓÈëÆ÷ÆÇÊôĞÔ ²¢¿¼ÂÇÆ÷ÆÇ×éºÏ
			int addon[32];
			int count = CollectSpiritEffect(*newess, addon, sizeof(addon)/sizeof(int));
			if(count > 0)
			{
				addon_data_man::Instance().DeactivateAddonList(addon, count, this, obj, parent);
			}

		}

		//Add by houjun 2011-03-18, È¡Ïû¼¤»î±¦Ê¯ÊôĞÔ
		const equip_essence_new* gemess = GetGemDataEssence(parent);
		if(gemess)
		{
			DeactiveGemEffect(obj, *gemess, parent);
		}

		__foo::reinforce(ess->rf_stone,index,this,*ess,obj, parent, adjust_ratio,adjust_point);
		__foo::reinforce(ess->pr_stone,index,this,*ess,obj, parent);

		if (ess->sk_stone) ActivateAddonByID(ess->sk_stone, obj, false, parent);
		if (ess->sk_stone_level) ActivateAddonByID(ess->sk_stone_level, obj, false, parent);
		if (ess->rf_stone_level > 0) ActiveHiddenAddon(ess->rf_stone_level, obj, false, parent);
	}
	
	gplayer_imp * pImp= (gplayer_imp *)obj;
	if(IsAstrologyEssence(parent) && pImp->IsAstrologyActive())
	{
		DeactiveAstrologyProp(parent, obj);
	}
}

void equip_item::InsertAddon(const addon_data & addon)
{
	addon_handler * handler = addon_manager::QueryHandler(addon.id);
	if(!handler) return;
	
	addon_data_spec spec;
	spec.handler = handler;
	spec.data = addon;
	
	int rst = addon_manager::CheckAndUpdate(addon,&_data);
	switch(rst)
	{
		case addon_manager::ADDON_MASK_STATIC:
		//ÓÉÓÚÊÇ¾²Ì¬µÄ£¬ËùÒÔÎŞĞèÔÙ±£´æÁË
		break;
		case addon_manager::ADDON_MASK_ACTIVATE:
		//´ËaddonĞèÒª±£Áô£¬¶¯Ì¬ÉúĞ§
		_addon_list.push_back(spec);
		break;
		case addon_manager::ADDON_MASK_USE:
		_use_addon_list.push_back(spec);
		break;
	}

	ASSERT(_addon_list.size() < 128);
}

void equip_item::ActiveHiddenAddon(int reinforce_level, gactive_imp *obj, bool active, item* parent) const
{
	if(reinforce_level > MAX_REINFORCE_STONE_LEVEL || reinforce_level <= 0) return ; 

	for(int i = 0; i < 3; ++i)
	{
		if(_hidden_addon_data[i].level <= reinforce_level && _hidden_addon_data[i].addon_id > 0 && _hidden_addon_data[i].level > 0)
		{
			ActivateAddonByID(_hidden_addon_data[i].addon_id, obj, active, parent);
		}
	}
}

int equip_item::ReinforceEquipment(item * parent, gactive_imp * pImp, int stone_id, const float aoffset[4], int & level_result, int & fee, int & level_before,bool newmode, int require_level, const float adjust2[20]) const
{
	item_body * pStone = item_manager::GetInstance().GetItemBody(stone_id);
	if(!pStone) return item::REINFORCE_UNAVAILABLE;

	Empty2OldEssence( parent );
	
	if(equip_essence_new * ess = GetOldDataEssence(parent))
	{
		int rst = pStone->DoReinforce(this, *ess, pImp,aoffset, level_result,fee, level_before, newmode, require_level, adjust2);
		if (rst != item::REINFORCE_UNAVAILABLE)
		{
			//¿ÉÄÜĞèÒªË¢ĞÂÑª¼À»êÁ¦
			equip_essence_new * essdata = GetSpiritDataEssence(parent);		
			//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
			if(essdata && essdata->master_id > 0) 
			{
				RebuildEnchantEffect(*essdata, false);
			}
			object_interface oif(pImp);
			if (rst == item::REINFORCE_SUCCESS)
			{
				((gplayer_imp*)pImp)->GetAchievementManager().OnRefine(oif, level_before, level_result, _data.equip_enchant_level);
				((gplayer_imp*)pImp)->GetAchievementManager().OnRefine(oif, _data.equip_enchant_level, level_result); //ÓÃÓÚ¼ÆÊı
				if(level_result >= 10)
				{
					((gplayer_imp*)pImp)->SendRefineMessage(parent->type,level_result);
				}
			}
			else if (rst == item::REINFORCE_FAILED_LEVEL_0)
			{
				((gplayer_imp*)pImp)->GetAchievementManager().OnRefine(oif, level_before, level_result, _data.equip_enchant_level);
			}
			else 
			{
				((gplayer_imp*)pImp)->GetAchievementManager().OnRefine(oif, level_before, 0, _data.equip_enchant_level);
			}
		}
		return rst;
	}
	return item::REINFORCE_UNAVAILABLE;
}

int equip_item::RemoveReinforce(item * parent, gactive_imp * pImp , size_t & money ,bool rp) const
{
	equip_essence_new * ess = GetOldDataEssence(parent);
	if(ess == NULL)
	{
		money = 0;
		return item::REINFORCE_UNAVAILABLE;
	}

	short * pLevel = NULL;
	int* pStone = NULL;
	int base_fee = 0;
	if(rp)
	{
		pStone = &ess->pr_stone;
		pLevel = &ess->pr_stone_level;
		base_fee = _data.install_fee.fee_uninstall_pstone;
	}
	if(!*pLevel || !*pStone)
	{
		money = 0;
		return item::REINFORCE_UNAVAILABLE;
	}

	size_t fee = base_fee * (*pLevel);
	if(money < fee) 
	{
		money = fee;
		return item::REMOVE_REINFORCE_OUT_OF_FUND;
	}

	money = fee;
	*pLevel = 0;
	*pStone = 0;
	return item::REMOVE_REINFORCE_SUCCESS;
}

int64_t equip_item::GetIdModify(const item * parent) const
{
	const equip_essence_new* ess = GetOldDataEssence( parent );
	if( !ess ){ return 0; }
	if( (_data.equip_type == 3) && _data.fashion_can_add_color )
	{
		int fashion_colorant_id = ess->colorant_id;
		if( fashion_colorant_id > 0 )
		{
			return ((int64_t)fashion_colorant_id)<<32;
		}
	}
	else
	{
		return ((int64_t)ess->rf_stone_level)<<32;
	}
	return 0;
}


void
equip_item::ActivateReinforce(equip_essence_new & value, int level_before) const
{
	if(level_before != value.rf_stone_level)
	{
		//¾«Á¶µÈ¼¶·¢Éú±ä»¯£¬ ½øĞĞÉ¨Ãè¿ØÖÆ
		value.sk_stone = CheckReinforceAddonValue( value.sk_stone, 0, value.rf_stone_level, level_before );
		value.sk_stone_level = CheckReinforceAddonValue( value.sk_stone_level, 1, value.rf_stone_level, level_before );
	}
}

int equip_item::CheckReinforceAddonValue( int new_addon, int index, int new_level, int old_level) const
{
	//¼ì²â¼¶±ğ
	if(new_level < _reinforce_addon_data[index].level)
	{
		new_addon = 0;
		return new_addon;
	}
	if(new_level == _reinforce_addon_data[index].level)
	{
		//Éú³ÉÊôĞÔ
		if(_reinforce_addon_data[index].level > 0)
		{
			int rst = abase::RandSelect(&(_reinforce_addon_data[index].prop[0].probability), sizeof(_reinforce_addon_data[index].prop[0]) , 5);
			new_addon = _reinforce_addon_data[index].prop[rst].id;
		}
	}
	return new_addon;
}

void equip_item::ActivateAddonByID(int id, gactive_imp * obj, bool active, item* parent) const
{
	const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
	if(pSpec)
	{
		if(active)
			pSpec->handler->Activate(pSpec->data,this,obj,parent);
		else
			pSpec->handler->Deactivate(pSpec->data,this,obj,parent);
	}
}

//Modified by Houjun 2010-03-07
//ĞŞ¸ÄÒòÎª¼ÓÈë²å²Û¼ø¶¨Ñª¼ÀÂß¼­£¬´¦ÀíÒÑ¾­½øĞĞ²å²Û¼ø¶¨ÔÙ½øĞĞÑª¼ÀµÄÇé¿ö
bool equip_item::DoBloodEnchant(size_t index, gactive_imp * obj, item * parent) const
{
	if(!_data.can_be_enchanted) return false;
	if(IsBloodEnchanted(parent)) return false;

	Empty2OldEssence( parent );
	
	const equip_essence_new * pOldEss = GetOldDataEssence(parent);
	if(!pOldEss) return false;

	equip_essence_new * pNewEss = GetSpiritDataEssence(parent);
	gplayer_imp * pImp= (gplayer_imp *)obj;

	//¼´Ã»ÓĞ½øĞĞÑª¼À£¬Ò²Ã»ÓĞ½øĞĞ²å²Û¼ø¶¨
	size_t name_len;
	const void * name = pImp->GetPlayerName(name_len);
	if(name_len > 20) name_len = 20;
	if(!pNewEss)
	{
		equip_essence_new  newess;
		memset(&newess,0 , sizeof(newess));
		newess = *pOldEss;

		//±ä³ÉÖ÷ÈË
		newess.master_id = obj->_parent->ID.id;
		memcpy(newess.master_name, name, name_len);

		//¼ÆËãÑª¼ÀºóµÄÒ»Ğ©Êı¾İ  Ö÷ÒªÊÇ¼ÆËãµ±Ç°ÆÇÁ¦ºÍÆÇÁ¦ÉÏÏŞ
		RebuildEnchantEffect(newess, true);
		newess.version = EQUIP_ESSENCE_VERSION_SPIRIT;

		//ÉèÖÃÊôĞÔ
		parent->SetContent(&newess, sizeof(newess));
	}
	//ÏÈ½øĞĞÁË±¦Ê¯²å²Û¼ø¶¨£¬»¹Ã»ÓĞ½øĞĞÑª¼ÀµÄÇé¿ö
	else if(pNewEss->master_id <= 0)
	{
		pNewEss->master_id = obj->_parent->ID.id;
		memcpy(pNewEss->master_name, name, name_len);
		//¼ÆËãÑª¼ÀºóµÄÒ»Ğ©Êı¾İ  Ö÷ÒªÊÇ¼ÆËãµ±Ç°ÆÇÁ¦ºÍÆÇÁ¦ÉÏÏŞ
		RebuildEnchantEffect(*pNewEss, true);	
	}

	if (!parent->IsBind())
	{
		parent->Bind();
		((gplayer_imp*)obj)->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, index);
		GLog::log(LOG_INFO,"ÓÃ»§%d×°±¸ %d µÎÑªÈÏÖ÷ºó×Ô¶¯°ó¶¨,GUID(%d,%d)",((gplayer_imp*)obj)->_parent->ID.id,parent->type,parent->guid.guid1,parent->guid.guid2);
	}

	// ¸üĞÂ»îÔ¾¶È[×°±¸Ñª¼À]
	((gplayer_imp*)obj)->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_EQUIP_XUELIAN);

	return true;
}

bool equip_item::RebuildEnchantEffect(equip_essence_new & ess, bool is_initial) const
{
	double max_spirit;
	if (false)
	{
		//¸ù¾İ¾«Á¶ÖµºÍÁéĞÔµÈ¼¶À´¼ÆËã¿ÉÓÃµÄ»êÁ¦×î´óÖµ£
		double level = ess.rf_stone_level;
		double level3 = level*level*level;
		double level4 = level3*level;
		double elevel2 = _data.equip_enchant_level*_data.equip_enchant_level;
		max_spirit = (level4 + level3*elevel2) / 10000;
	}
	else
	{
		max_spirit = ::GetSpiritMax(_data.equip_enchant_level, ess.rf_stone_level);
	}
	if(max_spirit < 0) max_spirit = 0;
	if(max_spirit > 1e8) max_spirit = 1e8;


	//¼ÆËãÆ÷ÆÇ¶Ô×î´ó»êÁ¦µÄÕ¼ÓÃ
	//Ä¿Ç°»¹Ã»ÓĞÆ÷ÆÇ ÒÔºó¼ÓÉÏ¡£
	DATA_TYPE dt;
	int spirit_need = 0;
	itemdataman & dataman = gmatrix::GetDataMan();
	for(size_t i = 0; i < MAX_ENCHANT_SLOT; i ++)
	{
		if(ess.soul[i] == 0) continue;
		const EQUIP_SOUL_ESSENCE& eess = *(const EQUIP_SOUL_ESSENCE*)dataman.get_data_ptr(ess.soul[i],ID_SPACE_ESSENCE,dt);
		if(!&eess || dt != DT_EQUIP_SOUL_ESSENCE) continue;
		spirit_need += eess.cost_soul;
	}

	ess.soul_used_spirit = spirit_need;
	ess.max_spirit_power = (int) max_spirit;
	if(is_initial || ess.cur_spirit_power > (int) max_spirit) ess.cur_spirit_power = (int) max_spirit;

	return true;
}

void equip_item::CheckRefineEffect(const equip_essence_new & ess, float & effect, int & effect2) const
{
	effect = 0;
	effect2 = 0;
	if(_data.equip_mask & (item::EQUIP_MASK_WEAPON | item::EQUIP_MASK_BODY)) 
	{
		effect =  0.05f;
		effect2 = 25;
	}
	else if(_data.equip_mask & (item::EQUIP_MASK_HEAD | item::EQUIP_MASK_FOOT)) 
	{
		effect =  0.1f;
		effect2 = 100;
	}
}

int equip_item::InsertSpiritAddon(gactive_imp * obj, item * parent, int addon_id, size_t addon_slot) const
{
	//±ØĞëÑª¼ÀºóµÄ×°±¸
	equip_essence_new * essdata = GetSpiritDataEssence(parent);
	//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
	if(!essdata || essdata->master_id <= 0) return S2C::ERR_SPIRIT_ADDON_FAILED;

	//±ØĞëÊÇÖ÷ÈË²ÅÄÜ¹à×¢
	if(essdata->master_id != obj->_parent->ID.id) return S2C::ERR_SPIRIT_MASTER_NOT_MATCH;

	//±ØĞëÊÇÆ÷ÆÇ²ÅĞĞ
	DATA_TYPE dt;
	const EQUIP_SOUL_ESSENCE& ess = *(const EQUIP_SOUL_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(addon_id,ID_SPACE_ESSENCE,dt);
	if(!&ess || dt != DT_EQUIP_SOUL_ESSENCE) return S2C::ERR_SPIRIT_ADDON_FAILED;

	gplayer_imp * pImp= (gplayer_imp *)obj;
	//½øĞĞ¹à×¢¼ì²é
	//·ÑÓÃ¼ì²é
	if(pImp->GetMoney() < ess.fee_install) return S2C::ERR_OUT_OF_FUND;
	//ĞèÇóµÈ¼¶¼ì²é
	if(_data.level_required < ess.level_required) return S2C::ERR_LEVEL_NOT_MATCH;
	//ĞèÇóÖ°Òµ
	if(_data.class_required > 0 && !(_data.class_required & ess.character_combo_id)) return S2C::ERR_INVALID_PLAYER_CALSS;
	if(_data.class_required1 > 0 && !(_data.class_required1 & ess.character_combo_id2)) return S2C::ERR_INVALID_PLAYER_CALSS;
	//ĞèÇóÎ»ÖÃ
	if(!(_data.equip_mask & ess.equip_mask)) return S2C::ERR_SPIRIT_ADDON_FAILED;
	//¼ì²é¿×Î»ÊÇ·ñ·ûºÏ
	if(addon_slot >= MAX_ENCHANT_SLOT) return S2C::ERR_SPIRIT_ADDON_FAILED;
	if(essdata->soul[addon_slot]) return S2C::ERR_SPIRIT_ADDON_FAILED;
	if(ess.hole_type && ess.hole_type != (int)(addon_slot +1)) return S2C::ERR_SPIRIT_ADDON_FAILED;
	//¼ì²é»êÁ¦ÊÇ·ñ×ã¹»
	if(essdata->max_spirit_power < ess.cost_soul + essdata->soul_used_spirit) return S2C::ERR_NOT_ENOUGH_SPIRIT_POWER;

	//¹à×¢
	essdata->soul[addon_slot] = addon_id;

	//¼ì²éÆ÷ÆÇ×éºÏ
	RefreshSoulRuneWord(*essdata);

	//ÖØĞÂË¢ĞÂ×î´ó»êÁ¦
	RebuildEnchantEffect(*essdata, false);

	//¿Û³ı½ğÇ®
	pImp->SpendMoney((int)ess.fee_install);
	pImp->_runner->spend_money((int)ess.fee_install);

	// ¸üĞÂ»îÔ¾¶È[×°±¸Ñª¼À]
	pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_EQUIP_XUELIAN);

	obj->DeliverTopicSite( new TOPIC_SITE::spirit( parent->type, ( wchar_t* )ess.name, sizeof( wchar_t ) * wcslen( (wchar_t*)ess.name ) ) );  // Youshuang add
	return 0;
}

bool equip_item::RefreshSoulRuneWord(equip_essence_new& ess) const
{
	abase::vector<int, abase::fast_alloc<> > list;
	list.reserve(MAX_ENCHANT_SLOT);
	for(size_t i =0; i < MAX_ENCHANT_SLOT; i ++)
	{
		int soulid = ess.soul[i];
		if(soulid > 0) list.push_back(soulid);
	}
	if(list.size() != MAX_ENCHANT_SLOT)
	{
		ess.soul_rune_word = 0;
		return true;
	}

	std::sort(list.begin(),list.end());
	item_manager::spirit_set node(&list[0]);
	ess.soul_rune_word = node(node);
	
	return true;
}

int
equip_item::RemoveSpiritAddon(gactive_imp * obj, item * parent, size_t addon_slot, int *removed_soul) const
{
	//±ØĞëÑª¼ÀºóµÄ×°±¸
	equip_essence_new * essdata = GetSpiritDataEssence(parent);
	//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
	if(!essdata || essdata->master_id <= 0) return S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED;

	//±ØĞëÊÇÖ÷ÈË²ÅÄÜ¹à×¢
	if(essdata->master_id != obj->_parent->ID.id) return S2C::ERR_SPIRIT_MASTER_NOT_MATCH;

	//¼ì²é¿×Î»ÊÇ·ñ·ûºÏ
	if(addon_slot >= MAX_ENCHANT_SLOT) return S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED;
	if(!essdata->soul[addon_slot]) return S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED;
	int addon_id = essdata->soul[addon_slot];
	
	//±ØĞëÊÇÆ÷ÆÇ²ÅĞĞ
	DATA_TYPE dt;
	const EQUIP_SOUL_ESSENCE& ess = *(const EQUIP_SOUL_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(addon_id,ID_SPACE_ESSENCE,dt);
	if(!&ess || dt != DT_EQUIP_SOUL_ESSENCE) return S2C::ERR_SPIRIT_ADDON_REMOVE_FAILED;

	gplayer_imp * pImp= (gplayer_imp *)obj;
	//½øĞĞ¹à×¢¼ì²é
	//·ÑÓÃ¼ì²é
	if(pImp->GetMoney() < ess.fee_uninstall) return S2C::ERR_OUT_OF_FUND;

	//¼ì²é¿ÕÎ»
	if(!pImp->GetInventory().HasSlot(addon_id)) return S2C::ERR_INVENTORY_IS_FULL;

	//·µ»Ø²ğ³ıµÄÆ÷ÆÇ
	*removed_soul = addon_id;
	//Çå³ı¹à×¢
	essdata->soul[addon_slot] = 0;

	//¼ì²éÆ÷ÆÇ×éºÏ
	RefreshSoulRuneWord(*essdata);

	//ÖØĞÂË¢ĞÂ×î´ó»êÁ¦
	RebuildEnchantEffect(*essdata, false);

	//¿Û³ı½ğÇ®
	pImp->SpendMoney((int)ess.fee_uninstall);
	pImp->_runner->spend_money((int)ess.fee_uninstall);

	// ¸üĞÂ»îÔ¾¶È[×°±¸Ñª¼À]
	pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_EQUIP_XUELIAN);

	return 0;
}

int equip_item::DecSpiritPower(gactive_imp * obj, item* parent, int count, int index) const
{
	equip_essence_new * essdata = GetSpiritDataEssence(parent);
	//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
	if(!essdata || essdata->master_id <= 0) return -1;
	if(essdata->cur_spirit_power <= 0) return -1;
	if(essdata->cur_spirit_power <= count && parent->IsActive())
	{
		Deactivate(index,obj,parent);
	}
	
	essdata->cur_spirit_power -= count;
	if(essdata->cur_spirit_power <= 0) 
	{
		essdata->cur_spirit_power = 0;
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(CanActivate(pImp->GetEquipInventory(), obj, parent)) Activate(index, pImp->GetEquipInventory(), obj, parent);

	}
	return essdata->cur_spirit_power;
}

int equip_item::GetSpiritPower(item* parent) const
{
	equip_essence_new * essdata = GetSpiritDataEssence(parent);
	//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
	if(!essdata || essdata->master_id <= 0) return 0;
	return essdata->cur_spirit_power;
}


int equip_item::RestoreSpiritPower(gactive_imp * obj, item * parent) const
{
	//»Ö¸´»êÁ¦
	//±ØĞëÑª¼ÀºóµÄ×°±¸
	equip_essence_new * essdata = GetSpiritDataEssence(parent);
	//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
	if(!essdata || essdata->master_id <= 0) return S2C::ERR_RESTORE_SPIRIT_POWER;

	//±ØĞëÊÇÖ÷ÈË²ÅÄÜ²Ù×÷
	if(essdata->master_id != obj->_parent->ID.id) return S2C::ERR_SPIRIT_MASTER_NOT_MATCH;
	gplayer_imp * pImp= (gplayer_imp *)obj;

	int spirit_need = essdata->max_spirit_power - essdata->cur_spirit_power;
	int money_need = spirit_need * g_config.spirit_charge_fee;
	if(money_need <= 0) return S2C::ERR_RESTORE_SPIRIT_POWER;

	//·ÑÓÃ¼ì²é
	if(pImp->GetMoney() < (size_t)money_need) return S2C::ERR_OUT_OF_FUND;

	//»Ö¸´»êÁ¦
	essdata->cur_spirit_power = essdata->max_spirit_power;

	//¿Û³ı½ğÇ®
	pImp->SpendMoney(money_need);
	pImp->_runner->spend_money(money_need);

	// ¸üĞÂ»îÔ¾¶È[×°±¸Ñª¼À]
	pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_EQUIP_XUELIAN);

	return 0;
	
}

int equip_item::SpiritDecompose(gactive_imp * obj, item * parent, int *spirit_unit1, int * spirit_unit2) const
{
	//±ØĞëÑª¼ÀºóµÄ×°±¸
	equip_essence_new * essdata = GetSpiritDataEssence(parent);
	//Add by houjun 2011-03-07, ¼ÓÈë¶ÔµÎÑªÈÏÖ÷µÄÅĞ¶Ï£¬´æÔÚÏÈ½øĞĞ²å²Û¼ø¶¨µÄÇé¿ö
	if(!essdata || essdata->master_id <= 0) return S2C::ERR_SPIRIT_DECOMPOSE_FAILED;

	//±ØĞëÊÇÖ÷ÈË²ÅÄÜ²Ù×÷
	if(essdata->master_id != obj->_parent->ID.id) return S2C::ERR_SPIRIT_MASTER_NOT_MATCH;
	gplayer_imp * pImp= (gplayer_imp *)obj;

	//µ±Ç°µÄ»êÁ¦±ØĞëÊÇÂúµÄ
	if(essdata->max_spirit_power != essdata->cur_spirit_power || essdata->max_spirit_power <= 0) return S2C::ERR_SPIRIT_DECOMPOSE_FAILED;
	
	//µ±Ç°²»ÄÜÓĞÏâÇ¶µÄÆ÷ÆÇ
	for(size_t i =0; i < MAX_ENCHANT_SLOT; i ++)
	{
		if(essdata->soul[i]) return S2C::ERR_SPIRIT_DECOMPOSE_FAILED;
	}
	
	//·ÑÓÃ¼ì²é
	int money_need = _data.install_fee.fee_install_estone;
	if(pImp->GetMoney() < (size_t)money_need) return S2C::ERR_OUT_OF_FUND;

	//¼ÆËã²ú³öµÄÔª»êÖé
	int cur_spirit = essdata->cur_spirit_power;

	*spirit_unit1 =  (cur_spirit / 1000) * 10;
	*spirit_unit2 =  cur_spirit % 1000;

	//¿Û³ı½ğÇ®
	pImp->SpendMoney(money_need);
	pImp->_runner->spend_money(money_need);

	// ¸üĞÂ»îÔ¾¶È[×°±¸Ñª¼À]
	pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_EQUIP_XUELIAN);
	return 0;
}

int equip_item::GetReinforceLevel(const item * parent) const
{
	const equip_essence_new * ess = GetOldDataEssence(parent);
	if(!ess) return 0;
	if(ess->rf_stone == 0) return 0;
	if(_estone_id <= 0) return 0;

	int level = ess->rf_stone_level;
	if(level > MAX_REINFORCE_STONE_LEVEL) level = MAX_REINFORCE_STONE_LEVEL;
	if(level < 0) level = 0;

	return level;
}

bool equip_item::InitReinforceLevel(item * parent, int level)
{
	if(level > MAX_REINFORCE_STONE_LEVEL || level <= 0) return false; 
	Empty2OldEssence( parent );
	equip_essence_new * ess = GetOldDataEssence(parent);
	if(!ess) return false;
	if(_estone_id <= 0) return false;

	ess->rf_stone = _estone_id;
        ess->rf_stone_level = level;

	if(level >= _reinforce_addon_data[0].level)
	{
		//Éú³ÉÒş²ØÊôĞÔ1
		if(_reinforce_addon_data[0].level > 0)
		{
			int rst = abase::RandSelect(&(_reinforce_addon_data[0].prop[0].probability), sizeof(_reinforce_addon_data[0].prop[0]) , 5);
			ess->sk_stone = _reinforce_addon_data[0].prop[rst].id;
		}
	}
	if(level >= _reinforce_addon_data[1].level)
	{
		//Éú³ÉÒş²ØÊôĞÔ2
		if(_reinforce_addon_data[1].level > 0)
		{
			int rst = abase::RandSelect(&(_reinforce_addon_data[1].prop[0].probability), sizeof(_reinforce_addon_data[1].prop[0]) , 5);
			ess->sk_stone_level = _reinforce_addon_data[1].prop[rst].id;
		}
	}


	return true;
}

bool equip_item::ResetHiddenAddon(item * parent) const
{
	equip_essence_new * ess = GetOldDataEssence(parent);
	if(!ess) return false;
	int level = ess->rf_stone_level; 
	ess->sk_stone = 0;
	ess->sk_stone_level = 0;

	if(level > MAX_REINFORCE_STONE_LEVEL || level <= 0) return false; 
		
	if(level >= _reinforce_addon_data[0].level)
	{
		//Éú³ÉÒş²ØÊôĞÔ1
		if(_reinforce_addon_data[0].level > 0)
		{
			int rst = abase::RandSelect(&(_reinforce_addon_data[0].prop[0].probability), sizeof(_reinforce_addon_data[0].prop[0]) , 5);
			ess->sk_stone = _reinforce_addon_data[0].prop[rst].id;
		}
	}
	if(level >= _reinforce_addon_data[1].level)
	{
		//Éú³ÉÒş²ØÊôĞÔ2
		if(_reinforce_addon_data[1].level > 0)
		{
			int rst = abase::RandSelect(&(_reinforce_addon_data[1].prop[0].probability), sizeof(_reinforce_addon_data[1].prop[0]) , 5);
			ess->sk_stone_level = _reinforce_addon_data[1].prop[rst].id;
		}
	}
	return true;
}

int equip_item::GetSpiritLevel(item * parent) const
{
	return _data.equip_enchant_level;
}

// Youshuang add
bool equip_item::SetFashionContent( item* parent, int colorant_id ) const
{
	Empty2OldEssence( parent );
	equip_essence_new* ess = GetOldDataEssence(parent);
	if( !ess ){ return false; }
	ess->colorant_id = colorant_id;
	if( ess->version < EQUIP_ESSENCE_VERSION_GEM )
	{
		ess->version = EQUIP_ESSENCE_VERSION_GEM;
	}
	return true;
}

bool equip_item::AddColor( item* parent, int colorant_id, int quality )
{
	if( !_data.fashion_can_add_color ){ return false; }
	if( player_template::GetInstance().HasQuality( quality ) == QUALITY_WRONG )
	{
		return false;
	}
	return SetFashionContent( parent, colorant_id );
}

void equip_item::InitFromShop(gplayer_imp* pImp,item * parent, int value) const
{
	if( _data.fashion_can_add_color )
	{
		int colorant_id = _data.fixed_init_color ? _data.init_color_value : player_template::GetInstance().GetInitColor();
		if( colorant_id < 0 )
		{
			return;
		}
		SetFashionContent( parent, colorant_id );
	}
}
// end

float upgrade_prop[] = {1.0, 0.9, 0.7, 0.64, 0.32, 0.16, 0.08, 0.04, 0.02, 0.01, 0.004, 0.001, 0.0005, 0.0004, 0.0003, 0.0002, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001};

bool equip_item::UpgradeEquipment(item * parent, gactive_imp * imp, int item_index, int item_type,  int output_id, 
		bool is_special_rt, int special_level, int stone_index, int stone_id, int stone_num, int rt_index, int rt_id)
{
	gplayer_imp * pImp = (gplayer_imp *)imp;
	item_list &inv = pImp->GetInventory();
	int reinforce_level = GetReinforceLevel(parent);
	ASSERT((size_t)reinforce_level <= sizeof(upgrade_prop)/sizeof(float)-1);

	if(rt_index >= 0 || _data.equip_type == 2)
	{
		int success = false;
		//Èç¹ûÊÇÊÎÆ·²»ĞèÒª±£Áô·û
		if( _data.equip_type == 2)
		{
			success = true;
		}
		else
		{
			//10¼¶ÒÔÉÏµÄ»ìÔªÌì¾§·â¶¥, ¿ÉÒÔ³É¹¦µÄÉı¼¶Á¶Æ÷µÈ¼¶³¬¹ı10¼¶µÄÈÎºÎ×°±¸
			if(is_special_rt && (special_level >= reinforce_level || special_level >= 10)) 
			{
				success = true;
			}
			else if(is_special_rt && special_level < reinforce_level && special_level < 10)
			{
				return false;
			}
			else if(!is_special_rt)
			{
				float prop[2];
				prop[0] = upgrade_prop[reinforce_level];
				prop[1] = 1.0f - prop[0] + 1e-4;
				int rst = abase::RandSelect(prop, 5);
				if(rst == 0) success = true;
			}
		}

		if(success)
		{
			item outitem;
			//Éú³ÉĞÂ×°±¸
			if(output_id > 0)
			{
				const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
				if( !pItem ) return false;
				if( ( pItem->proc_type & item::ITEM_PROC_TYPE_GUID ) && pItem->pile_limit == 1 )
				{
					item_data * pItem2 = DupeItem(*pItem);
					generate_item_guid(pItem2);
					MakeItemEntry(outitem, *pItem2);
					FreeItem(pItem2);
				}
				else
				{
					MakeItemEntry(outitem, *pItem);
				}
			}	

			//¸´ÖÆÊôĞÔ
			if(IsOldEssence(parent))
			{
				equip_essence_new oldess;
				memset(&oldess,0 , sizeof(oldess));	
				const equip_essence_new * pOldEss = GetOldDataEssence(parent);
				oldess = *pOldEss;
				outitem.SetContent(&oldess, sizeof(oldess));	
			}
			else if(IsSpiritEssence(parent))
			{
				equip_essence_new newess;
				memset(&newess,0 , sizeof(newess));	
				const  equip_essence_new * pNewEss = GetSpiritDataEssence(parent);
				newess = *pNewEss;
				if(outitem.body)
				{
					((equip_item*)(outitem.body))->RebuildEnchantEffect(newess, false);
					((equip_item*)(outitem.body))->RefreshSoulRuneWord(newess);
				}
				outitem.SetContent(&newess, sizeof(newess));	
			}
			//Add by houjun 2011-03-07, ´¦Àíequip_essence_gemÇé¿ö
			else if(IsGemEssence(parent))
			{
				equip_essence_new gemess;
				memset(&gemess, 0, sizeof(gemess));
				const equip_essence_new *pGemEss = GetGemDataEssence(parent);
				gemess = *pGemEss;
				//Èç¹ûÒÑ¾­½øĞĞÁËµÎÑªÈÏÇ×£¬¼¤»îÏà¹ØµÄÊôĞÔ
				if(outitem.body && gemess.master_id > 0)
				{
					((equip_item*)(outitem.body))->RebuildEnchantEffect(gemess, false);
					((equip_item*)(outitem.body))->RefreshSoulRuneWord(gemess);
				}
				//RefreshGemEffect(pImp, gemess);
				outitem.SetContent(&gemess, sizeof(gemess));	
			}
			else if(IsAstrologyEssence(parent))
			{
				equip_essence_new astess;
				memset(&astess, 0, sizeof(astess));
				const equip_essence_new * pAsEss = GetAstrologyDataEssence(parent);
				astess = * pAsEss;

				if(outitem.body && astess.master_id > 0)
				{
					((equip_item*)(outitem.body))->RebuildEnchantEffect(astess, false);
					((equip_item*)(outitem.body))->RefreshSoulRuneWord(astess);
				}

				outitem.SetContent(&astess, sizeof(astess));	
			}
			else
			{
				if(!outitem.body)
				{
					outitem.Release();
					return false;
				}
			}

			if(outitem.body)((equip_item*)(outitem.body))->ResetHiddenAddon(&outitem);
			if(parent->IsBind()) outitem.Bind();
			if(parent->IsLocked()) outitem.Lock();

			//×°±¸Éı¼¶µÄÊ±ºòÓĞ¿ÉÄÜµ¼ÖÂÁ¶Æ÷id¸Ä±ä,´æÅÌ½á¹¹ÀïÃæµÄrf_stoneÒ²ĞèÒªÏàÓ¦µÄĞŞÕı
			if(GetEStoneID() != ((equip_item*)(outitem.body))->GetEStoneID()) 
			{
				equip_essence_new * pess1 = GetOldDataEssence(parent); 
				if(pess1 && pess1->rf_stone > 0)
				{
					equip_essence_new * pess2 = GetOldDataEssence(&outitem);
					pess2->rf_stone = ((equip_item*)(outitem.body))->GetEStoneID();
				}
			}


			if(rt_index >= 0)
			{
				//Èç¹ûÊ¹ÓÃ°ó¶¨ĞÍ»ìÔªÌì¾§Éı¼¶×°±¸£¬Éı¼¶ºó×°±¸ÓëÈËÎï°ó¶¨
				item & rt_it = inv[rt_index];
				if(rt_it.proc_type & item::ITEM_PROC_TYPE_NOTRADE2) outitem.Bind();
			}

			outitem.expire_date = parent->expire_date;


			//×°±¸ÏûÊ§
			pImp->UseItemLog(inv[item_index], 1);
			pImp->GetInventory().DecAmount(item_index, 1);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item_index,item_type, 1 ,S2C::DROP_TYPE_USE);

			//Ä£¾ßÏûÊ§	
			pImp->UseItemLog(inv[stone_index], stone_num);
			pImp->GetInventory().DecAmount(stone_index, stone_num);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,stone_index,stone_id, stone_num ,S2C::DROP_TYPE_USE);

			//±£Áô·ûÏûÊ§	
			if(rt_index >= 0)
			{
				pImp->UseItemLog(inv[rt_index], 1);
				pImp->GetInventory().DecAmount(rt_index, 1);
				pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rt_index,rt_id, 1 ,S2C::DROP_TYPE_USE);
			}

			//·ÅÈëĞÂµÄ×°±¸
			int rst = inv.Push(outitem);
			int state = item::Proctype2State(outitem.proc_type);
			pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,gplayer_imp::IL_INVENTORY,rst,state);
			return true;

		}

		//Ê§°ÜÁËÒ²ĞèÒª¿Û³ıµÀ¾ß
		if(rt_index >= 0)
		{
			pImp->UseItemLog(inv[rt_index], 1);
			pImp->GetInventory().DecAmount(rt_index, 1);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rt_index,rt_id, 1 ,S2C::DROP_TYPE_USE);
		}
		return false;
	}
	else
	{
		item outitem;
		//Éú³ÉĞÂ×°±¸
		if(output_id > 0)
		{
			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
			if( !pItem ) return false;
			if( ( pItem->proc_type & item::ITEM_PROC_TYPE_GUID ) && pItem->pile_limit == 1 )
			{
				item_data * pItem2 = DupeItem(*pItem);
				generate_item_guid(pItem2);
				MakeItemEntry(outitem, *pItem2);
				FreeItem(pItem2);
			}
			else
			{
				MakeItemEntry(outitem, *pItem);
			}
		}	

		if(parent->IsBind()) outitem.Bind();
		if(parent->IsLocked()) outitem.Lock();
		outitem.expire_date = parent->expire_date;
	
		//×°±¸ÏûÊ§
		pImp->UseItemLog(inv[item_index], 1);
		pImp->GetInventory().DecAmount(item_index, 1);
		pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item_index,item_type, 1 ,S2C::DROP_TYPE_USE);

		//Ä£¾ßÏûÊ§	
		pImp->UseItemLog(inv[stone_index], stone_num);
		pImp->GetInventory().DecAmount(stone_index, stone_num);
		pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,stone_index,stone_id, stone_num ,S2C::DROP_TYPE_USE);

		//·ÅÈëĞÂµÄ×°±¸
		int rst = inv.Push(outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,gplayer_imp::IL_INVENTORY,rst,state);
		return true;
	}
}

bool repeat(char shape[3], int idx, int givenShape)
{
	for(int i = 0; i < 3; i ++)
	{
		if(idx == i) continue;
		if(shape[i] == givenShape) return true;
	}
	return false;
}


//±¦Ê¯²å²Û¼ø¶¨ºó³õÊ¼»¯
void equip_item::InitGemSlot(equip_essence_new& gemEss) const
{
	//Ëæ»úÉú³É×°±¸µÄ²å²Û
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		gemEss.gem_embeded[i] = -1;
		int rst = abase::RandSelect(gem_shape_prob, GEM_SHAPE_MAX_NUM);
		while(repeat(gemEss.gem_slot_shape, i, rst))
		{
			rst = abase::RandSelect(gem_shape_prob, GEM_SHAPE_MAX_NUM);
		}
		gemEss.gem_slot_shape[i] = rst;
	}
}

//±¦Ê¯½á¹¹³õÊ¼»¯
void equip_item::InitGemEssence(equip_essence_new & ess) const
{
	for(size_t i = 0; i < MAX_GEM_SLOT; ++i)
	{
		ess.gem_slot_shape[i] = -1;
		ess.gem_embeded[i] = -1;
	}	
}


//±¦Ê¯²å²Û¼ø¶¨
bool equip_item::IdentifyGemSlots(size_t index, gactive_imp * obj, item * parent) const
{
	gplayer_imp * pImp= (gplayer_imp *)obj;
	//×°±¸²»ÄÜ¿ªÆô²å²Û
	if(_data.equip_type != 2 || !_data.can_gem_slot_identify) 
	{
		pImp->_runner->error_message(S2C::ERR_ITEM_CAN_NOT_GEM_SLOT_IDENTIFY);
		return false;
	}

	Empty2OldEssence( parent );
	if(IsOldEssence(parent))
	{
		equip_essence_new gemEss;
		memset(&gemEss, 0, sizeof(gemEss));
		equip_essence_new* pOldEss = GetOldDataEssence(parent);
		if(!pOldEss) return false;
		gemEss = *pOldEss;
		InitGemSlot(gemEss);
		gemEss.version = EQUIP_ESSENCE_VERSION_GEM;
		parent->SetContent(&gemEss, sizeof(gemEss));
	}
	else if(IsSpiritEssence(parent))
	{
		equip_essence_new gemEss;
		memset(&gemEss, 0, sizeof(gemEss));
		equip_essence_new * pNewEss = GetSpiritDataEssence(parent);
		if(!pNewEss) return false;

		//½øĞĞ¹ıµÎÑªÈÏÇ×
		gemEss = *pNewEss;
		RebuildEnchantEffect(gemEss, true);
		InitGemSlot(gemEss);
		gemEss.version = EQUIP_ESSENCE_VERSION_GEM;
		parent->SetContent(&gemEss, sizeof(gemEss));
	}
	//ÒÑ¾­¿ªÆô²å²Û
	else if(IsGemEssence(parent))
	{
		return false;
	}
	else if(IsAstrologyEssence(parent))
	{
		equip_essence_new * pAstrologyEss = GetAstrologyDataEssence(parent);

		//¼ì²éÊÇ·ñÒÑ¾­¼ø¶¨
		bool flag = false;
		for(size_t i = 0; i < MAX_GEM_SLOT; ++i)
		{
			if(pAstrologyEss->gem_slot_shape[i] != -1)
			{
				flag = true;
				break;
			}
		}
		if(flag) return false;

		InitGemSlot(*pAstrologyEss);
	}
	else
	{
		return false;
	}


	//ÉèÖÃ×°±¸°ó¶¨
	if (!parent->IsBind())
	{
		parent->BindOnEquip();
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, index);
		GLog::log(LOG_INFO,"ÓÃ»§%d×°±¸ %d ±¦Ê¯²å²Û¼ø¶¨ºó×Ô¶¯°ó¶¨,GUID(%d,%d)",pImp->_parent->ID.id,parent->type,parent->guid.guid1,parent->guid.guid2);
	}
	return true;
}

struct cost_item
{
	int id;
	int idx;
	int cnt;
};

//ÖØÖı±¦Ê¯²å²Û
bool equip_item::RebuildGemSlots(gactive_imp* obj, item* parent, bool locked[3]) const
{
	equip_essence_new* gemEss = GetGemDataEssence(parent);	
	if(!gemEss) return false;
	if(!IsIdentifyGem(parent)) return false;

	gplayer_imp* pImp = (gplayer_imp*)obj;
	item_list& inv = pImp->GetInventory();

	//»ñÈ¡¼ÏËø²å²ÛÊıÁ¿£¬È«²¿¼ÏËø²»ÄÜ½øĞĞÖØÖı
	int lockItemCnt = 0;
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(locked[i]) lockItemCnt ++;	
	}
	if(lockItemCnt == 3)
	{
		pImp->_runner->error_message(S2C::ERR_PLAYER_KEEP_ALL_SLOTS_WHEN_REBUILD);
		return false;
	}
	
	//É¨ÃèÍæ¼Ò°ü¹ü£¬»ñµÃÖØÖıÏûºÄÎïÆ·µÄÎ»ÖÃºÍÊıÁ¿ĞÅÏ¢, Ë÷Òı0¡¢1¡¢2µÄÏûºÄÎïÆ·Ò»´Î¿Û¼õ
	abase::vector<cost_item> cost_item_vec[3];
	int cost_item_cnt[3] = {0, 0, 0};
	for(size_t i = 0; i < inv.Size(); i ++)
	{
		item& it = inv[i];
		if(it.type == -1) continue;
		for(int j = 0; j < 3; j ++)
		{
			//ÖØÖıµÀ¾ß
			if(g_config.id_gem_slot_rebuild_article[j] > 0 && it.type == g_config.id_gem_slot_rebuild_article[j])
			{
				cost_item ci;
				ci.id = it.type;
				ci.idx = i;
				ci.cnt = it.count;
				cost_item_cnt[j] += it.count;
				cost_item_vec[j].push_back(ci);
				break;
			}
		}
		//Èç¹ûÎïÆ·0ÒÑ¾­Âú×ãÌõ¼ş£¬Ö±½Ó·µ»Ø
		if(cost_item_cnt[0] >= 1) break;
	}

	if(cost_item_cnt[0] + cost_item_cnt[1] + cost_item_cnt[2] < 1)
	{
		pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return false;
	}

	//É¨ÃèÍæ¼Ò°ü¹üÀ¸£¬»ñµÃÏûºÄËø¶¨ÎïÆ·µÄÎ»ÖÃºÍÊıÁ¿ĞÅÏ¢£¬Ë÷Òı0¡¢1¡¢2µÄÏûºÄÎïÆ·ÒÀ´Î¿Û¼õ
	abase::vector<cost_item> lock_cost_item_vec[3];	
	if(lockItemCnt > 0)
	{
		int lock_cost_item_cnt[3] = {0, 0, 0};
		for(size_t i = 0; i < inv.Size(); i ++)
		{
			item& it = inv[i];
			if(it.type == -1) continue;
			for(int j = 0; j < 3; j ++)
			{
				//ÖØÖıËø¶¨µÀ¾ß
				if(g_config.id_gem_slot_lock_article[j] > 0 && it.type == g_config.id_gem_slot_lock_article[j])
				{
					cost_item ci;
					ci.id = it.type;
					ci.idx = i;
					ci.cnt = it.count;
					lock_cost_item_cnt[j] += it.count;
					lock_cost_item_vec[j].push_back(ci);
					break;
				}
			}
			//Èç¹ûÎïÆ·0ÒÑ¾­Âú×ãÌõ¼ş£¬Ö±½Ó·µ»Ø
			if(lock_cost_item_cnt[0] >= lockItemCnt) break;
		}

		//ÏûºÄÎïÆ·ÊıÁ¿²»Âú×ã¿Û¼õÒªÇó
		if(lock_cost_item_cnt[0] + lock_cost_item_cnt[1] + lock_cost_item_cnt[2] < lockItemCnt)
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
	}	

	//ÖØÖı²å²ÛĞÎ×´
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(locked[i]) continue;
		int rst = abase::RandSelect(gem_shape_prob, GEM_SHAPE_MAX_NUM);
		while(repeat(gemEss->gem_slot_shape, i, rst))
		{
			rst = abase::RandSelect(gem_shape_prob, GEM_SHAPE_MAX_NUM);
		}
		gemEss->gem_slot_shape[i] = rst;
	}

	//¿Û³ıÖØÖıµÀ¾ß 
	int cnt = 1;
	for(int i = 0; i < 3; i ++)
	{
		for(size_t j = 0; j < cost_item_vec[i].size(); j ++)
		{
			cost_item& ci = cost_item_vec[i][j];
			int decCnt = (ci.cnt <= cnt ? ci.cnt : cnt);
			//¼ì²éÊÇ·ñĞèÒª¼ÇÂ¼Ïû·ÑÖµ
			pImp->CheckSpecialConsumption(ci.id, decCnt);
			pImp->UseItemLog(inv[ci.idx], decCnt);
			pImp->GetInventory().DecAmount(ci.idx, decCnt);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,ci.idx,ci.id, decCnt ,S2C::DROP_TYPE_USE);
			cnt -= decCnt;
			if(cnt <= 0) break;
		}
		if(cnt <= 0) break;
	}

	//¿Û³ı¼ÓËøµÀ¾ß
	if(lockItemCnt > 0)
	{
		cnt = lockItemCnt;
		for(int i = 0; i < 3; i ++)
		{
			for(size_t j = 0; j < lock_cost_item_vec[i].size(); j ++)
			{
				cost_item& ci = lock_cost_item_vec[i][j];
				int decCnt = (ci.cnt <= cnt ? ci.cnt : cnt);
				//¼ì²éÊÇ·ñĞèÒª¼ÇÂ¼Ïû·ÑÖµ
				pImp->CheckSpecialConsumption(ci.id, decCnt);
				pImp->UseItemLog(inv[ci.idx], decCnt);
				pImp->GetInventory().DecAmount(ci.idx, decCnt);
				pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,ci.idx,ci.id, decCnt ,S2C::DROP_TYPE_USE);
				cnt -= decCnt;
				if(cnt <= 0) return true;
			}
		}
		
	}
	return true;
}

//¶¨ÖÆ±¦Ê¯²å²Û
bool equip_item::CustomizeGemSlots(gactive_imp * obj, item* parent, int gem_seal_id[3], int gem_seal_index[3]) const
{
	equip_essence_new* gemEss = GetGemDataEssence(parent);
	if(!gemEss) return false;
	if(!IsIdentifyGem(parent)) return false;

	gplayer_imp* pImp = (gplayer_imp*)obj;
	item_list& inv = pImp->GetInventory();

	//¼ÆËã¶¨ÖÆºóµÄ²å²ÛĞÎ×´
	char shape[3];	
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		shape[i] = -1;
		//¸Ä²å²Û²»½øĞĞ¶¨ÖÆ£¬±£³ÖÔ­ÓĞĞÎ×´
		if(gem_seal_id[i] <= 0)
		{
			shape[i] = gemEss->gem_slot_shape[i];
			continue;
		}

		//²å²ÛÎïÆ·ÊÇ·ñ´æÔÚ
		item& seal = inv[gem_seal_index[i]];
		if(seal.type == -1 || seal.type != gem_seal_id[i])
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		DATA_TYPE dt;
		const GEM_SEAL_ESSENCE& gemSealEss = *(const GEM_SEAL_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(gem_seal_id[i], ID_SPACE_ESSENCE, dt);
		if(!&gemSealEss || dt != DT_GEM_SEAL_ESSENCE) 
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		shape[i] = gemSealEss.shape_type;
	}

	//ÅĞ¶Ï±¦Ê¯Ä§Ó¡µÄĞÎ×´ÊÇ·ñÓĞÖØ¸´
	for(int i = 1; i < MAX_GEM_SLOT; i ++)
	{		
		for(int j = i - 1; j >= 0; j --)
		{
			if(shape[i] != -1 && shape[j] != -1 && shape[i] == shape[j])
			{
				pImp->_runner->error_message(S2C::ERR_SAME_GEM_SLOT_SHAPE);
				return false;
			}
		}		
	}
	
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(shape[i] >= GEM_SHAPE_ALL && shape[i] < GEM_SHAPE_MAX_NUM)
		{
			gemEss->gem_slot_shape[i] = shape[i];
			
			//ÏûºÄ±¦Ê¯Ä§Ó¡
			if(gem_seal_id[i] > 0 && gem_seal_index[i] > -1)
			{
				pImp->UseItemLog(inv, gem_seal_index[i],1);
				inv.DecAmount(gem_seal_index[i],1);
				pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY, gem_seal_index[i], gem_seal_id[i], 1 ,S2C::DROP_TYPE_USE);
			}
		}
	}
	return true;
}

//±¦Ê¯ÏâÇ¶
bool equip_item::EmbededGems(gactive_imp * obj, item* parent, int gem_id[3], int gem_index[3]) const
{
	equip_essence_new* gemEss = GetGemDataEssence(parent);
	if(!gemEss) return false;
	if(!IsIdentifyGem(parent)) return false;

	gplayer_imp* pImp = (gplayer_imp*)obj;
	item_list& inv = pImp->GetInventory();
	
	//¼ÆËãÒÑ¾­ÏâÇ¶µÄ±¦Ê¯ÊıÁ¿
	size_t slotCnt = 0;
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(gem_id[i] > 0 && gemEss->gem_embeded[i] > 0)
		{
			slotCnt ++;
		}
	}

	//¼ì²é°ü¹üÀ¸ÊıÁ¿
	if(inv.GetEmptySlotCount() < slotCnt)
	{
		pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	//ÏâÇ¶±¦Ê¯
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(gem_id[i] <= 0) continue;
		item& gem = inv[gem_index[i]];
		if(gem.type == -1 || gem.type != gem_id[i])
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}	
		if(gem.IsLocked())
		{
			return false;
		}
		int oldGemId = gemEss->gem_embeded[i];
		gemEss->gem_embeded[i] = gem_id[i];		

		//ÀÏ±¦Ê¯·µ»Ø°ü¹ü		
		if(oldGemId > 0)
		{
			pImp->CreateItem(oldGemId, 1, true);
		}
		
		//ÏâÇ¶ÉÏµÄ±¦Ê¯´Ó°ü¹üÏûÊ§
		pImp->UseItemLog(inv, gem_index[i],1);
		inv.DecAmount(gem_index[i],1);
		pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY, gem_index[i], gem_id[i], 1 ,S2C::DROP_TYPE_USE);
	}
	return true;
}

//±¦Ê¯²ğ³ı
bool equip_item::RemoveGems(gactive_imp * obj, item* parent, bool remove[3]) const
{
	equip_essence_new* gemEss = GetGemDataEssence(parent);
	if(!gemEss) return false;
	if(!IsIdentifyGem(parent)) return false;

	gplayer_imp* pImp = (gplayer_imp*)obj;
	item_list& inv = pImp->GetInventory();

	//Òª²ğ³ıµÄ±¦Ê¯ÊıÁ¿
	size_t gem_cnt = 0;
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(gemEss->gem_embeded[i] > 0 && remove[i])
		{
			gem_cnt ++;
		}
	}

	//¼ì²é¿Õ°ü¹üÀ¸µÄÊıÁ¿ÊÇ·ñ×ã¹»·ÅÖÃ²ğĞ¶ÏÂÀ´µÄ±¦Ê¯
	if(gem_cnt == 0 || inv.GetEmptySlotCount() < gem_cnt) 
	{
		return false;
	}
	
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{
		if(!remove[i]) continue;
		int gem_id = gemEss->gem_embeded[i];
		gemEss->gem_embeded[i] = -1;			
		//±¦Ê¯ÍË»¹µ½°ü¹ü
		if(gem_id > 0 && pImp->CreateItem(gem_id, 1, true))
		{
			GLog::log(GLOG_INFO,"ÓÃ»§%d²ğ³ıÁË%d¸ö±¦Ê¯%d", pImp->GetParent()->ID.id, 1, gem_id);
		}
	}
	return true;
}

int equip_item::CollectGemAddons(gactive_imp* obj, const equip_essence_new& ess, int* addons, int maxAddonCnt) const
{
	gplayer_imp* pImp = (gplayer_imp*)obj;

	int cnt = 0;
	for(int i = 0; i < MAX_GEM_SLOT; i ++)
	{		
		if(ess.gem_embeded[i] <= 0) continue;
		DATA_TYPE dt;
		const GEM_ESSENCE& gemEss = *(const GEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(ess.gem_embeded[i], ID_SPACE_ESSENCE, dt);
		if(!&gemEss || dt != DT_GEM_ESSENCE) 
		{
			pImp->_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return -1;
		}

		for(int j = 0; j < 6; j ++)
		{
			if(cnt >= maxAddonCnt) 
			{
				return cnt;
			}
			if(gemEss.addon_property[j].addon_type == 0)
			{
				if(gemEss.addon_property[j].addon_id > 0)
				{
					addons[cnt ++] = gemEss.addon_property[j].addon_id;
				}
			}
			else if(gemEss.addon_property[j].addon_type == 1)
			{
				if(ess.gem_slot_shape[i] == gemEss.shape_type || ess.gem_slot_shape[i] == 0)
				{
					if(gemEss.addon_property[j].addon_id > 0)
					{
						addons[cnt ++] = gemEss.addon_property[j].addon_id;
					}
				}	
			}
		}
	}
	return cnt;
}

void equip_item::ActiveGemEffect(gactive_imp* obj, const equip_essence_new& ess, item* parent) const
{
	int addon[18];
	int cnt = CollectGemAddons(obj, ess, addon, 18);
	if(cnt > 0)
	{
		addon_data_man::Instance().ActivateAddonList(addon, cnt, this, obj, parent);
	}
}

void equip_item::DeactiveGemEffect(gactive_imp* obj, const equip_essence_new& ess, item* parent) const
{
	int addon[18];
	int cnt = CollectGemAddons(obj, ess, addon, 18);
	if(cnt > 0)
	{
		addon_data_man::Instance().DeactivateAddonList(addon, cnt, this, obj, parent);
	}
}

void equip_item::OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{	
	equip_essence_new * pNewEss = GetSpiritDataEssence(parent);
	if( !pNewEss ){ return; }
	gplayer_imp * pImp= (gplayer_imp *)obj;
	if(pNewEss->master_id == pImp->_parent->ID.id)
	{
		memset(pNewEss->master_name, 0, sizeof(pNewEss->master_name));
		size_t name_len;
		const void * name = pImp->GetPlayerName(name_len);
		if(name_len > MAX_USERNAME_LENGTH) name_len = MAX_USERNAME_LENGTH;
		memcpy(pNewEss->master_name, name, name_len);
		pImp->PlayerGetItemInfo(l,index);
	}
}

void equip_item::OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{	
	equip_essence_new * pNewEss = GetSpiritDataEssence(parent);
	if( !pNewEss ){ return; }
	gplayer_imp * pImp= (gplayer_imp *)obj;
	pNewEss->master_id = pImp->_parent->ID.id;
	pImp->PlayerGetItemInfo(l,index);
}

//×°±¸´ò¿×
bool equip_item::OpenSlot(item * parent, gactive_imp *obj, size_t item_index, size_t stone_index, int stone_id)
{
	gplayer_imp * pImp= (gplayer_imp *)obj;
	item_list& inv = pImp->GetInventory();

//	ÏÖÔÚÕâ¸öÅäÖÃÏî×÷·Ï£¬Ä¬ÈÏËùÓĞ×°±¸¿ÉÒÔ´ò¿×£¬´ò¿×ºó×°±¸×Ô¶¯°ó¶¨
//	if(!_data.can_open_astrology_slot) return false;

	int cur_hole = GetCurHole(parent);
	if(cur_hole >= MAX_ASTROLOGY_HOLE) return false;
	if(stone_id != g_config.item_equip_hole_id[cur_hole]) return false;
	//60¼¶ÒÔÉÏ×°±¸²ÅÄÜ´ò¿×
	if(_data.level_required < 60) return false;

	Empty2OldEssence( parent );
	if(IsOldEssence(parent))
	{
		equip_essence_new astess;
		memset(&astess,0 , sizeof(astess));	
		const equip_essence_new * pOldEss = GetOldDataEssence(parent);
		astess = *pOldEss; 
		astess.cur_hole = 1;
		InitGemEssence(astess);
		astess.version = EQUIP_ESSENCE_VERSION_ASTROLOGY;
		parent->SetContent(&astess, sizeof(astess));	
	}
	else if(IsSpiritEssence(parent))
	{
		equip_essence_new astess;
		memset(&astess,0 , sizeof(astess));	
		const  equip_essence_new * pNewEss = GetSpiritDataEssence(parent);
		astess= *pNewEss; 
		astess.cur_hole = 1;
		InitGemEssence(astess);
		astess.version = EQUIP_ESSENCE_VERSION_ASTROLOGY;
		parent->SetContent(&astess, sizeof(astess));	

	}
	else if(IsGemEssence(parent))
	{
		equip_essence_new astess;
		memset(&astess,0 , sizeof(astess));	
		const equip_essence_new *pGemEss = GetGemDataEssence(parent);
		astess = *pGemEss; 
		astess.cur_hole = 1;
		astess.version = EQUIP_ESSENCE_VERSION_ASTROLOGY;
		parent->SetContent(&astess, sizeof(astess));	
	}
	else if(IsAstrologyEssence(parent))
	{
		equip_essence_new *pAstEss = GetAstrologyDataEssence(parent);
		if(pAstEss->cur_hole >= MAX_ASTROLOGY_HOLE) return false;
		pAstEss->cur_hole++;
	}
	else
	{
		return false;
	}
	
	//µÀ¾ßÏûÊ§	
	pImp->UseItemLog(inv[stone_index], 1);
	pImp->GetInventory().DecAmount(stone_index, 1);
	pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);
	if (!parent->IsBind())
	{
		parent->Bind();
		GLog::log(LOG_INFO,"ÓÃ»§%d×°±¸ %d ×°±¸´ò¿×ºó×Ô¶¯°ó¶¨,GUID(%d,%d)",((gplayer_imp*)obj)->_parent->ID.id,parent->type,parent->guid.guid1,parent->guid.guid2);
	}
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,item_index);

	GLog::log(LOG_INFO, "ÓÃ»§%d¶Ô×°±¸%dÖ´ĞĞÁË´ò¿×²Ù×÷, stone_id=%d, stone_index=%d", ((gplayer_imp*)obj)->_parent->ID.id,parent->type, stone_id, stone_index);
	return true;
}

static int astrology_level_require[] = {0, 60, 60, 75, 75, 90, 90, 105, 105, 120, 135};
//ÏâÇ¶ĞÇÅÌ
bool equip_item::InstallAstrology(gactive_imp * obj, item * parent1, item * parent2, size_t item_index, int item_id, size_t stone_index, int stone_id, size_t slot_index)
{
	if(!IsAstrologyEssence(parent1)) return false;
	
	equip_essence_new *pAstEss = GetAstrologyDataEssence(parent1);
	if(!pAstEss) return false;

	if(slot_index >= (size_t)pAstEss->cur_hole || slot_index > MAX_ASTROLOGY_HOLE) return false;
	if(pAstEss->hole[slot_index].astrology_id > 0) return false;

	astrology_essence * pAst = ((item_astrology*)parent2->body)->GetDataEssence(parent2);
	if(!pAst) return false;
	
	gplayer_imp * pImp= (gplayer_imp *)obj;
	item & it2 = pImp->GetInventory()[stone_index];
	int reborn_cnt = ((item_astrology*)(it2.body))->GetRebornCount();
	int equip_mask = it2.body->GetEquipMask(); 
	int level = pAst->level;
	ASSERT(level >= 0 && level <= 10);
	int req_level = astrology_level_require[level]; 
	if(_data.level_required < req_level) return false;
	if(_data.require_reborn_count < reborn_cnt) return false;
	if(!(_data.equip_mask & equip_mask)) return false;

	pAstEss->hole[slot_index].astrology_id = stone_id;
	pAstEss->hole[slot_index].level = pAst->level;
	pAstEss->hole[slot_index].score = pAst->score;
	pAstEss->hole[slot_index].energy_consume = pAst->energy_consume;
	memcpy(&pAstEss->hole[slot_index].prop[0], &pAst->prop[0], sizeof(astrology_prop)*6);
	if(it2.IsLocked()) pAstEss->hole[slot_index].is_lock = true;

	//ĞÇ×ùÏûÊ§	
	pImp->UseItemLog(pImp->GetInventory()[stone_index], 1);
	pImp->GetInventory().DecAmount(stone_index, 1);
	pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);
	
	size_t need_money = g_config.fee_astrology_add; 
	if(need_money > 0)
	{
		pImp->SpendMoney(need_money);
		pImp->_runner->spend_money(need_money);
	}

	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,item_index);
	GLog::log(LOG_INFO, "ÓÃ»§%d¶Ô×°±¸%dÖ´ĞĞÁËĞÇ×ùÏâÇ¶²Ù×÷, stone_id=%d, stone_index=%d, slot_index=%d", pImp->_parent->ID.id, item_id, stone_id, stone_index, slot_index);
	return true;
}

//Õª³ıĞÇÅÌ
bool equip_item::UninstallAstrology(gactive_imp * obj, item * parent, size_t item_index, int item_id, size_t slot_index)
{
	if(!IsAstrologyEssence(parent)) return false;
	
	equip_essence_new *pAstEss = GetAstrologyDataEssence(parent);
	if(!pAstEss) return false;

	if(slot_index >= (size_t)pAstEss->cur_hole || slot_index > MAX_ASTROLOGY_HOLE) return false;
	if(pAstEss->hole[slot_index].astrology_id <= 0) return false;

	astrology_essence ess;
	int astrology_id = pAstEss->hole[slot_index].astrology_id;
	bool need_lock = pAstEss->hole[slot_index].is_lock;
	ess.level = pAstEss->hole[slot_index].level;
	ess.score = pAstEss->hole[slot_index].score;
	ess.energy_consume = pAstEss->hole[slot_index].energy_consume;
	memcpy(&ess.prop[0], &pAstEss->hole[slot_index].prop[0], sizeof(ess.prop));
	memset(&pAstEss->hole[slot_index], 0, sizeof(astrology_slot));

	
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * pItem = gmatrix::GetDataMan().generate_item(astrology_id,&tag,sizeof(tag));
	if(!pItem) return false;
	
	gplayer_imp * pImp= (gplayer_imp *)obj;
	
	size_t need_money = g_config.fee_astrology_remove; 
	if(need_money > 0)
	{
		pImp->SpendMoney(need_money);
		pImp->_runner->spend_money(need_money);
	}
	
	int item_count = 1;
	int rst = pImp->GetInventory().Push(*pItem, item_count, 0);
	item & pAstItem = pImp->GetInventory()[rst];
	pAstItem.SetContent(&ess, sizeof(ess));
	if(need_lock) pAstItem.Lock();
	
	int state = item::Proctype2State(pItem->proc_type);
	pImp->_runner->obtain_item(astrology_id,0,1,pImp->GetInventory()[rst].count, 0,rst,state);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,item_index);
	GLog::log(LOG_INFO, "ÓÃ»§%d¶Ô×°±¸%dÖ´ĞĞÁËĞÇ×ùÕª³ı²Ù×÷, stone_id=%d, slot_index=%d", ((gplayer_imp*)obj)->_parent->ID.id,parent->type, astrology_id, slot_index);
	return true;
}

int equip_item::GetAstrologyEnergyConsume(item * parent) const
{
	if(IsAstrologyEssence(parent))
	{
		equip_essence_new *pAstEss = GetAstrologyDataEssence(parent);
		int energy_consume = 0;
		for(size_t i = 0; i < 3; ++i)
		{
			if(pAstEss->hole[i].astrology_id > 0 && pAstEss->hole[i].energy_consume > 0)
			{
				energy_consume += pAstEss->hole[i].energy_consume;
			}
		}
		return energy_consume;
	}
	return 0;
}
