#ifndef __ONLINEGAME_COMMON_PROTOCOL_IMP_H__
#define __ONLINEGAME_COMMON_PROTOCOL_IMP_H__

#include "protocol.h"
#include <ASSERT.h>
#include <vector>
#include <set>
#include <map>
struct gplayer;
struct gobject;
struct gmatter;
struct gnpc;
struct item_data;
class gactive_imp;
struct gactive_object;
class world_manager;
struct extend_prop;
struct q_extend_prop;
class item_manager;
class item;

namespace  S2C
{
	namespace CMD
	{
		template <typename T>
		struct Type2Type
		{
			typedef T O_Type;
		};
		
		template <typename CMD>
		struct Make;

		template <>
		struct Make<single_data_header>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,int command)
			{
				return wrapper <<(unsigned short)command;
			}
		};

		template <>
		struct Make<multi_data_header>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,int command,int count)
			{
				return wrapper <<(unsigned short)command << (unsigned short)count;
			}
		};
		
		template <>
		struct Make<INFO::self_info_1>
		{
			template <typename WRAPPER,typename GPLAYER>
			static WRAPPER & From(WRAPPER & wrapper,GPLAYER *pPlayer,int64_t exp)
			{
				unsigned int state = MakePlayerState(pPlayer) | pPlayer->object_state;
				unsigned int extra_state = pPlayer->extra_state;
				unsigned int extra_state2 = pPlayer->extra_state2;
				wrapper << (double)exp << pPlayer->ID.id << pPlayer->pos 
				<< (unsigned short)pPlayer->crc
				<< (unsigned char)pPlayer->dir  << (unsigned char)pPlayer->base_info.cls
				<< (unsigned char) pPlayer->level  << (unsigned char)pPlayer->level2 << (unsigned char)pPlayer->dt_level 
				<< (char)pPlayer->pk_level << (char)pPlayer->cultivation << pPlayer->reputation << pPlayer->fashion_mask << state << extra_state
				<< extra_state2; 
				return MakePlayerExtendState(wrapper,pPlayer,state,extra_state,extra_state2);
			}
		};


		template <>
		struct Make<INFO::player_info_1>
		{

			template <typename WRAPPER,typename GPLAYER>
			static bool From(WRAPPER & wrapper,GPLAYER * pObject)
			{
				if(pObject->invisible) return false;
				unsigned int state = MakePlayerState(pObject) | pObject->object_state;
				unsigned int extra_state = pObject->extra_state;
				unsigned int extra_state2 = pObject->extra_state2;
				//临时的数据改变
				wrapper << pObject->ID.id << pObject->pos 
				<< (unsigned short)pObject->crc 
				<< (unsigned char)pObject->dir 
				<< (unsigned char)pObject->base_info.cls
				<< (unsigned char) pObject->level << (unsigned char)pObject->level2 << (unsigned char)pObject->dt_level 
				<< (char)pObject->pk_level << (char) pObject->cultivation << pObject->reputation << pObject->fashion_mask << state << extra_state 
				<< extra_state2;
				MakePlayerExtendState(wrapper,pObject,state,extra_state,extra_state2);
				return true;
			}

			template <typename WRAPPER,typename GPLAYER>
			static bool From(WRAPPER & wrapper,GPLAYER * pObject,const A3DVECTOR &newpos)
			{
				if(pObject->invisible) return false;
				unsigned int state = MakePlayerState(pObject) | pObject->object_state;
				unsigned int extra_state = pObject->extra_state;
				unsigned int extra_state2 = pObject->extra_state2;
				//临时的数据改变
				wrapper << pObject->ID.id << newpos
				<< (unsigned short)pObject->crc 
				<< (unsigned char)pObject->dir 
				<< (unsigned char)pObject->base_info.cls
				<< (unsigned char) pObject->level << (unsigned char)pObject->level2 << (unsigned char)pObject->dt_level 
				<< (char)pObject->pk_level << (char) pObject->cultivation << pObject->reputation << pObject->fashion_mask << state << extra_state 
				<< extra_state2;
				MakePlayerExtendState(wrapper,pObject,state,extra_state,extra_state2);
				return true;
			}
		};

		template <>
		struct Make<INFO::npc_info>
		{
			template <typename WRAPPER, typename GNPC>
			static WRAPPER & From(WRAPPER & wrapper,GNPC * pObject)
			{
				unsigned int state = MakeObjectState(pObject) | pObject->object_state;
				wrapper << pObject->ID.id << pObject->vis_tid << pObject->pos 
					<< (unsigned short)pObject->crc << (unsigned char)pObject->dir << state; 
				return MakeNPCExtendState(wrapper,pObject,state);
			}

			template <typename WRAPPER, typename GNPC>
			static WRAPPER & From(WRAPPER & wrapper,GNPC * pObject,const A3DVECTOR &newpos)
			{
				unsigned int state = MakeObjectState(pObject) | pObject->object_state;
				wrapper << pObject->ID.id << pObject->vis_tid << newpos
					<< (unsigned short)pObject->crc << (unsigned char)pObject->dir << state; 
				return MakeNPCExtendState(wrapper,pObject,state);
			}
		};
		
		template<>
		struct Make<INFO::matter_info_1>
		{
			template <typename WRAPPER, typename GMATTER>
			static WRAPPER & From(WRAPPER & wrapper,GMATTER * pMatter)
			{
				wrapper << pMatter->ID.id << pMatter->matter_type << pMatter->name_id
					<< pMatter->pos << pMatter->dir << pMatter->dir1 << pMatter->rad;
				unsigned char state = pMatter->matter_state;
				wrapper << state;
				if(state == GMATTER::STATE_MASK_BATTLE_FLAG)
				{
					wrapper << (char)pMatter->battle_flag;
				}
				else
				{
					wrapper << (char) 0;
				}
				if(state & GMATTER::STATE_MASK_OWNER_MATTER)
				{
					wrapper << pMatter->owner.GetOwnerType();
					wrapper << pMatter->owner.GetNameID();
				}
				if(state & GMATTER::STATE_MASK_PHASE)
				{
					wrapper << pMatter->phase_id;
				}
				if(state & GMATTER::STATE_MASK_COMBINE_MINE )
				{
					wrapper << pMatter->combine_state;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<INFO::move_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int id,const A3DVECTOR & target, 
					unsigned short use_time,unsigned short speed,unsigned char move_mode)
			{
				return wrapper << id << target << use_time << speed << move_mode;
			}
		};

		template <>
		struct Make<CMD::self_info_1>
		{
			template <typename WRAPPER,typename GPLAYER>
			static WRAPPER & From(WRAPPER & wrapper,GPLAYER *pPlayer,int64_t exp)
			{
				Make<single_data_header>::From(wrapper,SELF_INFO_1);
				return Make<INFO::self_info_1>::From(wrapper,pPlayer,exp);
			}
		};

		template <>
		struct Make<CMD::player_enter_slice>
		{
			template <typename WRAPPER,typename GPLAYER>
			static WRAPPER & From(WRAPPER & wrapper,GPLAYER* pPlayer,const A3DVECTOR & pos)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ENTER_SLICE);
				Make<INFO::player_info_1>::From(wrapper,pPlayer,pos);
				//这里要注意，不能在隐身状态下使用
				ASSERT(pPlayer->invisible == false);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::npc_enter_slice>
		{
			template <typename WRAPPER, typename GNPC>
			static WRAPPER & From(WRAPPER & wrapper,GNPC * pNPC,const A3DVECTOR & pos)
			{
				Make<single_data_header>::From(wrapper,NPC_ENTER_SLICE);
				return Make<INFO::npc_info>::From(wrapper,pNPC,pos);
			}
		};

		template <>
		struct Make<CMD::npc_enter_world>
		{
			template <typename WRAPPER,typename GNPC>
			static WRAPPER & From(WRAPPER & wrapper,GNPC * pNPC)
			{
				Make<single_data_header>::From(wrapper,NPC_ENTER_WORLD);
				return Make<INFO::npc_info>::From(wrapper,pNPC);
			}
		};


		template <>
		struct Make<CMD::leave_slice>
		{
			template <typename WRAPPER, typename OBJECT>
			static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObject)
			{
				Make<single_data_header>::From(wrapper,OBJECT_LEAVE_SLICE);
				return wrapper << pObject->ID.id;
			}
		};

		template <>
		struct Make<CMD::notify_pos>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,const A3DVECTOR & pos, int tag)
			{
				Make<single_data_header>::From(wrapper,OBJECT_NOTIFY_POS);
				return wrapper <<  pos << tag;
			}
		};

		template<typename T>
		inline unsigned int MakeObjectState(T * pObject)
		{
			unsigned int state = 0;
			if(pObject->IsZombie()) state = T::STATE_ZOMBIE;
			if(pObject->extend_state) state |= T::STATE_EXTEND_PROPERTY;
			if(pObject->effect_count) state |= T::STATE_EFFECT;
			if(pObject->extend_dir_state) state |= T::STATE_DIR_EXT_STATE;
			return state;
		}

		template<typename T>
		inline unsigned int MakePlayerState(T * pObject)
		{
			unsigned int state = MakeObjectState(pObject);
			if(pObject->title_id) state |= T::STATE_TITLE;
			if(pObject->sect_flag) state |= T::STATE_SECT_MASTER_ID;
			return state;
		}


		template <typename WRAPPER,typename GNPC>
		inline WRAPPER & MakeNPCExtendState(WRAPPER & ar, GNPC * pObject,unsigned int state)
		{
			if(state & GNPC::STATE_EXTEND_PROPERTY)
			{
				ar << pObject->extend_state;
			}
			if(state & GNPC::STATE_NPC_PET)
			{
				ar << pObject->master_id;
				ar << pObject->pet_star;
				ar << pObject->pet_shape;
				ar << pObject->pet_face;
			}
			if(state & GNPC::STATE_NPC_NAME)
			{
				unsigned char name_size = pObject->name_size;
				if(name_size >= sizeof(pObject->npc_name)) name_size = sizeof(pObject->npc_name);
				ar << name_size;
				ar.push_back(pObject->npc_name,name_size);
			}
			if(state & GNPC::STATE_NPC_SUMMON)
			{
				ar << pObject->master_id;
			}
			if(state & GNPC::STATE_NPC_OWNER)
			{
				ar << pObject->owner.GetOwnerType();
				ar << pObject->owner.GetNameID();
			}
			if(state & GNPC::STATE_DIR_EXT_STATE)
			{
				ar << pObject->extend_dir_state;
				ar.push_back(pObject->extend_dir_state_dirs, sizeof(pObject->extend_dir_state_dirs));
			}
			if(state & GNPC::STATE_PHASE)
			{
				ar << pObject->phase_id;
			}

			if(state & GNPC::STATE_NPC_TELEPORT1 || state & GNPC::STATE_NPC_TELEPORT2)
			{
				ar << pObject->teleport_count;
			}
			return ar;
		}

		template <typename WRAPPER,typename GPLAYER>
		inline WRAPPER & MakePlayerExtendState(WRAPPER & ar, GPLAYER* pPlayer,unsigned int state, unsigned int extra_state, unsigned int extra_state2)
		{
			if(state & GPLAYER::STATE_ADV_MODE)
			{
				ar << pPlayer->adv_data1 << pPlayer->adv_data2;
			}

			if(state & GPLAYER::STATE_SHAPE)
			{
				ar << pPlayer->shape_form;
			}
			if(state & GPLAYER::STATE_EMOTE)
			{
				ar << pPlayer->emote_form;
			}
			if(state & GPLAYER::STATE_EXTEND_PROPERTY)
			{
				ar << pPlayer->extend_state;
			}
			if(state & GPLAYER::STATE_MAFIA)
			{
				ar << pPlayer->id_mafia;
			}

			if(state & GPLAYER::STATE_MARKET)
			{
				ar << pPlayer->market_id;
				ar << pPlayer->market_item_id;
			}

			if(state & GPLAYER::STATE_EFFECT)
			{
				unsigned char count = pPlayer->effect_count;
				ar << count;
				if(count)
				{
					ar.push_back(pPlayer->effect_list, sizeof(short)* count);
				}
			}

			if(state & GPLAYER::STATE_MOUNT)
			{
				ar << (char) pPlayer->mount_level;
				ar << (int) pPlayer->mount_id;
				ar << (char) pPlayer->mount_type;
			}

			if(state & GPLAYER::STATE_IN_BIND)
			{
				ar << (char) pPlayer->bind_type;
				ar << (int) pPlayer->bind_target;
				ar << (char) pPlayer->bind_pos;
				ar << (bool) pPlayer->bind_disconnect;
			}
			
			if(state & GPLAYER::STATE_TITLE)
			{
				ar << (short)pPlayer->title_id;
			}

			if(state & GPLAYER::STATE_SPOUSE)
			{
				ar << (int)pPlayer->spouse_id;
			}

			if(state & GPLAYER::STATE_FAMILY)
			{
				ar << (int)pPlayer->id_family << pPlayer->rank_mafia;
			}

			if(state & GPLAYER::STATE_SECT_MASTER_ID)
			{
				ar << (int)pPlayer->sect_id;
			}
			
			if(state & GPLAYER::STATE_IN_KILLINGFIELD)
			{
				ar << (unsigned char)pPlayer->rank;
			}
			if(state & GPLAYER::STATE_IN_VIP)
			{
				ar << (unsigned char)pPlayer->vip_type;
			}
			if(state & GPLAYER::STATE_TRANSFORM)
			{
				ar << (int)pPlayer->transform_id;
			}
			if(extra_state & GPLAYER::STATE_CARRIER)
			{
				ar << (int)pPlayer->carrier_id;
				ar << pPlayer->rpos;
				ar << pPlayer->rdir;
			}
			if(extra_state & GPLAYER::STATE_CIRCLE)
			{
				ar << pPlayer->circle_id;
				ar << pPlayer->circle_title;
			}
			if(extra_state & GPLAYER::STATE_CROSSZONE)
			{
				ar << pPlayer->id_zone;
			}

			if(extra_state & GPLAYER::STATE_VIPAWARD)
			{
				ar << (unsigned char)pPlayer->vipaward_level;
				ar << (char)pPlayer->is_hide_vip_level;
			}
			if(extra_state & GPLAYER::STATE_IN_COLLISION_RAID)
			{
			//	ar << pPlayer->character_id;
			//	ar << pPlayer->collision_state;
			}
			if(extra_state & GPLAYER::STATE_XY_DARKLIGHT_STATE)
			{
				ar << pPlayer->darklight_state;
			}
			if(extra_state & GPLAYER::STATE_XY_SPIRITS)
			{
				ar.push_back(pPlayer->darklight_spirits, sizeof(pPlayer->darklight_spirits));
			}
			if(extra_state & GPLAYER::STATE_MIRROR_IMAGE)
			{
				ar << pPlayer->mirror_image_cnt;
			}
			if(extra_state & GPLAYER::STATE_WING_COLOR)
			{
				ar << pPlayer->wing_color;
			}
			if(extra_state & GPLAYER::STATE_DIR_EXT_STATE)
			{
				ar << pPlayer->extend_dir_state;
				ar.push_back(pPlayer->extend_dir_state_dirs, sizeof(pPlayer->extend_dir_state_dirs));
			}
			if(state & GPLAYER::STATE_FLY)
			{
				ar << (short)pPlayer->start_fly_type;
			}
			if(extra_state & GPLAYER::STATE_EXTRA_EQUIP_EFFECT)
			{
				ar << pPlayer->weapon_effect_level;
			}
			if(extra_state & GPLAYER::STATE_FLOW_BATTLE)
			{
				ar << pPlayer->flow_battle_ckill;
			}
			if(extra_state & GPLAYER::STATE_PUPPET)
			{
				ar << pPlayer->puppet_id;
			}

			if (extra_state & GPLAYER::STATE_MOBACTIVE)
			{
				ar << pPlayer->mobactive_id;
				ar << (char)pPlayer->mobactive_pos;
				ar <<  pPlayer->mobactive_state;
			}

			if (extra_state & GPLAYER::STATE_ACTIVE_EMOTE)
			{
				ar << pPlayer->active_emote_first_player;
				ar << pPlayer->active_emote_id;
				ar << pPlayer->active_emote_type;
			}

			if (extra_state & GPLAYER::STATE_TRAVEL)
			{
				ar << pPlayer->travel_vehicle_id;
			}

			if (extra_state2 & GPLAYER::STATE_SEEKRAID_DIE)
			{
				int roletype = 2;
				ar << roletype;
			}
			if (extra_state2 & GPLAYER::STATE_CAPTURE_RAID_FLAG)
			{    
				int capture_id = 1;
				ar << capture_id;
			}  
			return ar;
		}
		
		template<>
		struct Make<CMD::object_move>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,
					const A3DVECTOR & target, unsigned short use_time,
					unsigned short speed,unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,OBJECT_MOVE);
				return Make<INFO::move_info>::From(wrapper,pObj->ID.id,target,use_time,speed,move_mode);
			}
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int id,
					const A3DVECTOR & target, unsigned short use_time,
					unsigned short speed,unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,OBJECT_MOVE);
				return Make<INFO::move_info>::From(wrapper,id,target,use_time,speed,move_mode);
			}
		};

		template<>
		struct Make<CMD::object_stop_move>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,
					const A3DVECTOR & target, unsigned short speed,
					unsigned char dir, unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,OBJECT_STOP_MOVE);
				return wrapper << pObj->ID.id << target << speed << dir << move_mode;
			}
		};

		template<>
		struct Make<CMD::player_enter_world>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ENTER_WORLD);
				Make<INFO::player_info_1>::From(wrapper,pPlayer);
				ASSERT(pPlayer->invisible == false);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::matter_enter_world>
		{
			template <typename WRAPPER,typename GMATTER>
			inline static WRAPPER & From(WRAPPER & wrapper, GMATTER * pMatter)
			{
				Make<single_data_header>::From(wrapper,MATTER_ENTER_WORLD);
				return Make<INFO::matter_info_1>::From(wrapper,pMatter);
			}
		};
		

		template <>
		struct Make<CMD::player_leave_world>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pPlayer)
			{
				Make<single_data_header>::From(wrapper,PLAYER_LEAVE_WORLD);
				return wrapper << pPlayer->ID.id;
			}
		};

		template <>
		struct Make<CMD::npc_dead>
		{
			template <typename WRAPPER, typename GNPC>
			inline static WRAPPER & From(WRAPPER & wrapper, GNPC * pNPC,const XID & killer)
			{
				Make<single_data_header>::From(wrapper,NPC_DEAD);
				return wrapper << pNPC->ID.id << killer.id;
			}
		};

		template <>
		struct Make<CMD::object_disappear>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, char at_once)
			{
				Make<single_data_header>::From(wrapper,OBJECT_DISAPPEAR);
				return wrapper << pObj->ID.id << at_once;
			}

			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & id, char at_once)
			{
				Make<single_data_header>::From(wrapper,OBJECT_DISAPPEAR);
				return wrapper << id.id << at_once;
			}
		};

		template <>
		struct Make<CMD::object_start_attack>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int target,unsigned char attack_stamp)
			{
				Make<single_data_header>::From(wrapper,OBJECT_START_ATTACK);
				return wrapper << pObj->ID.id << target << attack_stamp;
			}
		};

		template <>
		struct Make<CMD::self_stop_attack>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int flag)
			{
				Make<single_data_header>::From(wrapper,SELF_STOP_ATTACK);
				return wrapper << flag;
			}
		};



		template <>
		struct Make<CMD::object_attack_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & attacker, const XID & target, int damage, unsigned char attack_flag , unsigned char attack_stamp)
			{
				Make<single_data_header>::From(wrapper,OBJECT_ATTACK_RESULT);
				return wrapper << attacker.id << target.id << damage << attack_flag << attack_stamp;
			}
		};

		template <>
		struct Make<CMD::object_skill_attack_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & attacker, const XID & target,int skill_id, int damage, int dt_damage, unsigned char attack_flag , unsigned char attack_stamp)
			{
				Make<single_data_header>::From(wrapper,OBJECT_SKILL_ATTACK_RESULT);
				return wrapper << attacker.id << target.id << skill_id << damage << dt_damage << attack_flag << attack_stamp;
			}
		};

		template <>
		struct Make<CMD::hurt_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & id, int damage)
			{
				Make<single_data_header>::From(wrapper,HURT_RESULT);
				return wrapper << id.id << damage;
			}
		};

		template <>
		struct Make<CMD::error_msg>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper , int err_msg)
			{
				Make<single_data_header>::From(wrapper,ERROR_MESSAGE);
				return wrapper << err_msg;
			}
		};
		template <>
		struct Make<CMD::be_attacked>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & attacker,int damage,unsigned char eq_index,bool is_orange,unsigned char attack_flag, char speed)
			{
				Make<single_data_header>::From(wrapper,BE_ATTACKED);
				eq_index &= 0x7F;
				eq_index |= is_orange ? 0x80:0;
				return wrapper << attacker.id << damage << eq_index << attack_flag << speed;
			}
		};

		template <>
		struct Make<CMD::be_skill_attacked>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & attacker,int skill_id , int damage,unsigned char eq_index,bool is_orange,unsigned char attack_flag, char speed)
			{
				Make<single_data_header>::From(wrapper,BE_SKILL_ATTACKED);
				eq_index &= 0x7F;
				eq_index |= is_orange ? 0x80:0;
				return wrapper << attacker.id << skill_id << damage << eq_index << attack_flag << speed;
			}
		};

		template <>
		struct Make<CMD::be_hurt>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & attacker,int damage,bool invader)
			{
				Make<single_data_header>::From(wrapper,BE_HURT);
				return wrapper << attacker.id << damage << (unsigned char)invader;
			}
		};
		
		template <>
		struct Make<CMD::player_dead>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & killer,const XID & player)
			{
				Make<single_data_header>::From(wrapper,PLAYER_DEAD);
				return wrapper << killer.id << player.id ;
			}
		};

		template <>
		struct Make<CMD::be_killed>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj,const XID & killer, int time)
			{
				Make<single_data_header>::From(wrapper,BE_KILLED);
				return wrapper << killer.id  << pObj->pos << time;
			}
		};

		template <>
		struct Make<CMD::player_revival>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper , OBJECT * pObj, short type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_REVIVAL);
				return wrapper << pObj->ID.id << type << pObj->pos;
			}
		};

		template <>
		struct Make<CMD::player_pickup_money>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int amount)
			{
				Make<single_data_header>::From(wrapper,PICKUP_MONEY);
				return wrapper << amount;
			}
		};

		template <>
		struct Make<CMD::player_pickup_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type,int expire_date, unsigned short amount,unsigned short slot_amount,unsigned char where,unsigned char index,int state)
			{
				Make<single_data_header>::From(wrapper,PICKUP_ITEM);
				return wrapper << type << expire_date << amount << slot_amount << where << index << state;
			}
		};

		template<>
		struct Make<INFO::npc_info_00>
		{
			template <typename WRAPPER, typename BASIC_PROP, typename Q_EXTEND_PROP>
			inline static WRAPPER & From(WRAPPER & wrapper,int hp, const BASIC_PROP& bp, const Q_EXTEND_PROP& ep)
			{
				return wrapper << hp << ep.max_hp;
				//return wrapper << bp.level << bp.hp << ep.max_hp;
			}
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const INFO::npc_info_00 & info)
			{
				return wrapper << info.hp << info.max_hp;
			}
		};

		template <>
		struct Make<CMD::npc_info_00>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id,int hp,struct basic_prop & bp, struct q_extend_prop & ep)
			{
				Make<single_data_header>::From(wrapper,NPC_INFO_00);
				wrapper << id.id;
				return Make<INFO::npc_info_00>::From(wrapper,hp, bp,ep);
			}
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id,const INFO::npc_info_00 & info)
			{
				Make<single_data_header>::From(wrapper,NPC_INFO_00);
				wrapper << id.id;
				return Make<INFO::npc_info_00>::From(wrapper,info);
			}
		};


		template<>
		struct Make<INFO::player_info_00>
		{
			template <typename WRAPPER, typename BASIC_PROP, typename Q_EXTEND_PROP>
			inline static WRAPPER & From(WRAPPER & wrapper,int hp, const BASIC_PROP& bp, const Q_EXTEND_PROP& ep, char combat_state)
			{
				return wrapper <<  bp.level << bp.dt_level << combat_state << (unsigned char)bp.sec_level 
					<< hp << ep.max_hp << bp.mp << ep.max_mp << bp.dp << ep.max_dp << bp.rage;
			}

			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const INFO::player_info_00 & info)
			{
				wrapper.push_back(&info,sizeof(info));
				return wrapper;
			}
		};


		template <>
		struct Make<CMD::player_info_00>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id,int hp,struct basic_prop & bp,struct q_extend_prop & ep,char combat_state)
			{
				Make<single_data_header>::From(wrapper,PLAYER_INFO_00);
				wrapper << id.id;
				return Make<INFO::player_info_00>::From(wrapper, hp, bp,ep,combat_state);
			}

			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id,const INFO::player_info_00 & info)
			{
				Make<single_data_header>::From(wrapper,PLAYER_INFO_00);
				wrapper << id.id;
				return Make<INFO::player_info_00>::From(wrapper, info);
			}
		};

		template<>
		struct Make<INFO::self_info_00>
		{
			template <typename WRAPPER, typename BASIC_PROP, typename Q_EXTEND_PROP>
			inline static WRAPPER & From(WRAPPER & wrapper,const BASIC_PROP& bp, const Q_EXTEND_PROP& ep,char combat_state, int talisman_stamina, char cheat_mode)
			{
				if(cheat_mode)
				{
					return wrapper << bp.level << bp.dt_level << combat_state 
						<< (unsigned char) bp.sec_level 
						<< 1 << ep.max_hp << 1 << ep.max_mp << 1 << ep.max_dp << talisman_stamina 
						<< (double)bp.exp << (double)bp.dt_exp << bp.rage;
				}
				else
				{
					return wrapper << bp.level << bp.dt_level << combat_state 
						<< (unsigned char) bp.sec_level 
						<< bp.hp << ep.max_hp << bp.mp << ep.max_mp << bp.dp << ep.max_dp << talisman_stamina
						<< (double)bp.exp << (double)bp.dt_exp << bp.rage;
				}
			}
		};

		template<>
		struct Make<CMD::self_info_00>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,struct basic_prop & bp, struct q_extend_prop & ep,char combat_state,int talisman_stamina, char cheat_mode)
			{
				Make<single_data_header>::From(wrapper,SELF_INFO_00);
				return Make<INFO::self_info_00>::From(wrapper, bp,ep,combat_state, talisman_stamina,cheat_mode);
			}
		};

		template <>
		struct Make<CMD::OOS_list>
		{
			
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned int size,int id_list[])
			{
				Make<single_data_header>::From(wrapper,OUT_OF_SIGHT_LIST);
				wrapper << size;
				for(size_t i = 0; i < size; ++i)
				{
					wrapper << id_list[i];
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::receive_exp>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int64_t exp)
			{
				Make<single_data_header>::From(wrapper,RECEIVE_EXP);
				return wrapper << exp ;
			}
		};
		
		template<>
		struct Make<CMD::receive_bonus_exp>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int64_t exp)
			{
				Make<single_data_header>::From(wrapper,RECEIVE_BONUS_EXP);
				return wrapper << (double)exp ;
			}
		};

		template<>
		struct Make<CMD::level_up>
		{
			
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObject)
			{
				Make<single_data_header>::From(wrapper,LEVEL_UP);
				return wrapper << pObject->ID.id;
			}
		};

		template<>
		struct Make<CMD::unselect>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,UNSELECT);
			}
		};

		template<>
		struct Make<CMD::self_item_info>
		{
			template <typename WRAPPER,typename ITEM_DATA>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char index, ITEM_DATA & data,unsigned short crc, int state)
			{
				Make<single_data_header>::From(wrapper,SELF_ITEM_INFO);
				wrapper << where << index << data.type << data.expire_date << state << data.count ;
				wrapper << crc;
				if(data.use_wrapper)
				{
					wrapper << (unsigned short)(data.ar.size());
					wrapper.push_back(data.ar.data(),data.ar.size());
				}
				else
				{
					wrapper << (unsigned short)(data.content_length);
					wrapper.push_back(data.item_content,data.content_length);
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::self_item_empty_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char index)
			{
				Make<single_data_header>::From(wrapper,SELF_ITEM_EMPTY_INFO);
				return wrapper << where << index;
			}
		};

		template<>
		struct Make<CMD::self_inventory_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char where, unsigned char inv_size, const void * data,size_t len)
			{
				Make<single_data_header>::From(wrapper,SELF_INVENTORY_DATA);
				wrapper << where << inv_size << len;
				wrapper.push_back(data,len);
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::self_inventory_detail_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char where, unsigned char inv_size, const void * data,size_t len)
			{
				Make<single_data_header>::From(wrapper,SELF_INVENTORY_DETAIL_DATA);
				wrapper << where << inv_size << len;
				wrapper.push_back(data,len);
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::repurchase_inventory_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char inv_size, const void * data,size_t len)
			{
				Make<single_data_header>::From(wrapper,REPURCHASE_INV_DATA);
				wrapper << inv_size << len;
				wrapper.push_back(data,len);
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::exchange_inventory_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char idx1,unsigned char idx2)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_INVENTORY_ITEM);
				return wrapper << idx1 << idx2;
			}
		};

		template<>
		struct Make<CMD::move_inventory_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char src,unsigned char dest,unsigned short count)
			{
				Make<single_data_header>::From(wrapper,MOVE_INVENTORY_ITEM);
				return wrapper << src << dest << count;
			}
		};

		template<>
		struct Make<CMD::player_drop_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char index, int type, unsigned short count, const char drop_type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_DROP_ITEM);
				return wrapper << where << index << count << type << drop_type;
			}
		};

		// Youshuang add
		template<>
		struct Make<CMD::fashion_colorant_produce>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int index, int color, int quality)
			{
				Make<single_data_header>::From(wrapper,FASHION_COLORANT_PRODUCE);
				return wrapper << index << color << quality;
			}
		};

		template<>
		struct Make<CMD::fashion_colorant_use>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int where, int index, int color, int quality)
			{
				Make<single_data_header>::From(wrapper,FASHION_COLORANT_USE);
				return wrapper << where << index << color << quality;
			}
		};

		template<>
		struct Make<CMD::get_achieve_award>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short achiev_id)
			{
				Make<single_data_header>::From(wrapper,GET_ACHIEVEMENT_AWARD);
				return wrapper << achiev_id;
			}
		};

		template<>
		struct Make<CMD::raid_boss_been_killed>
		{
			template <typename WRAPPER, typename BOSSINFO>
			inline static WRAPPER & From(WRAPPER & wrapper, const std::vector<BOSSINFO>& monsters_been_killed )
			{
				Make<single_data_header>::From(wrapper,RAID_BOSS_BEEN_KILLED);
				unsigned short monster_cnt = monsters_been_killed.size();
				wrapper << monster_cnt;
				for( size_t i = 0; i < monsters_been_killed.size(); ++i )
				{
					wrapper << monsters_been_killed[i].is_killed << monsters_been_killed[i].achievement << monsters_been_killed[i].cur_num << monsters_been_killed[i].max_num;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::facbase_mall_item_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const std::set<int>& actived_items )
			{
				Make<single_data_header>::From(wrapper,GET_FACBASE_MALL_INFO);
				unsigned short items_cnt = actived_items.size();
				wrapper << items_cnt;
				for( std::set<int>::const_iterator iter = actived_items.begin(); iter != actived_items.end(); ++iter )
				{
					wrapper << *iter;
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::combine_mine_state_change>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj,int state)
			{
				Make<single_data_header>::From(wrapper,COMBINE_MINE_STATE_CHANGE);
				return wrapper << pObj->ID.id << state;
			}
		};
		
		template<>
		struct Make<CMD::get_faction_coupon>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int faction_coupon)
			{
				Make<single_data_header>::From(wrapper,GET_FACTION_COUPON);
				return wrapper << faction_coupon;
			}
		};
		
		template<>
		struct Make<CMD::get_facbase_cash_items_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const std::map<int, int>& items_cooldown )
			{
				Make<single_data_header>::From(wrapper,GET_FACBASE_CASH_ITEMS_INFO);
				unsigned short items_cnt = items_cooldown.size();
				wrapper << items_cnt;
				for( std::map<int, int>::const_iterator iter = items_cooldown.begin(); iter != items_cooldown.end(); ++iter )
				{
					wrapper << iter->first << iter->second;
				}
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::get_facbase_cash_item_success>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int item_index )
			{
				Make<single_data_header>::From(wrapper,GET_FACBASE_CASH_ITEM_SUCCESS);
				return wrapper << item_index;
			}
		};

		template<>
		struct Make<CMD::get_newyear_award_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int score, const std::vector<char>& status ) 
			{
				Make<single_data_header>::From(wrapper, GET_NEWYEAR_AWARD_INFO);
				wrapper << score << status.size();
				for( std::vector<char>::const_iterator iter = status.begin(); iter != status.end(); iter++ )
				{
					wrapper << *iter;
				}
				return wrapper;
			}
		};
		// end
		
		template<>
		struct Make<CMD::exchange_equipment_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char index1, unsigned char index2)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_EQUIPMENT_ITEM);
				return wrapper << index1 << index2;
			}
		};

		template<>
		struct Make<CMD::equip_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char idx_inv, unsigned char idx_eq,unsigned short count_inv,unsigned short count_eq )
			{
				Make<single_data_header>::From(wrapper,EQUIP_ITEM);
				return wrapper << idx_inv << idx_eq << count_inv << count_eq;
			}
		};

		template<>
		struct Make<CMD::move_equipment_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char idx_inv, unsigned char idx_eq,unsigned short amount)
			{
				Make<single_data_header>::From(wrapper,MOVE_EQUIPMENT_ITEM);
				return wrapper << idx_inv << idx_eq << amount;
			}
		};
		template<>
		struct Make<CMD::self_get_property>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,size_t status_point,const struct q_extend_prop & prop)
			{
				Make<single_data_header>::From(wrapper,SELF_GET_EXT_PROPERTY);
				/*extend_prop prop2;
				memset(&prop2, 0, sizeof(prop2));
				prop2.BuildFrom(prop);*/
				return wrapper << status_point << prop;
			}
		};

		template<>
		struct Make<CMD::player_select_target>
		{
			template <typename WRAPPER>
				inline static WRAPPER & From(WRAPPER & wrapper,int id)
				{
					Make<single_data_header>::From(wrapper,SELECT_TARGET);
					return wrapper << id;
				}
		};

		template<>
		struct Make<CMD::player_extprop_base>
		{
			template <typename WRAPPER, typename GACTIVE_IMP>
				inline static WRAPPER & From(WRAPPER & wrapper,GACTIVE_IMP * pObject)
				{
					Make<single_data_header>::From(wrapper,PLAYER_EXTPROP_BASE);
					return wrapper << pObject->_parent->ID.id
							<<pObject->_cur_prop.max_hp << pObject->_cur_prop.max_mp << pObject->_cur_prop.max_dp;
				}
		};
		
		template<>
		struct Make<CMD::player_extprop_move>
		{
			template <typename WRAPPER,typename GACTIVE_IMP>
				inline static WRAPPER & From(WRAPPER & wrapper,GACTIVE_IMP * pObject)
				{
					Make<single_data_header>::From(wrapper,PLAYER_EXTPROP_MOVE);
					return wrapper << pObject->_parent->ID.id
							<< pObject->_cur_prop.walk_speed << pObject->_cur_prop.run_speed;
				}
		};

		template<>
		struct Make<CMD::player_extprop_attack>
		{
			template <typename WRAPPER,typename GACTIVE_IMP>
				inline static WRAPPER & From(WRAPPER & wrapper,GACTIVE_IMP * pObject)
				{
					Make<single_data_header>::From(wrapper,PLAYER_EXTPROP_ATTACK);
					return wrapper 	<< pObject->_parent->ID.id
							<< pObject->_cur_prop.attack 
							<< pObject->_cur_prop.damage_low << pObject->_cur_prop.damage_high 
							<< pObject->GetAttackRange()
							<< pObject->_cur_prop.crit_rate << pObject->_cur_prop.crit_damage;
				}
		};

		template<>
		struct Make<CMD::player_extprop_defense>
		{
			template <typename WRAPPER,typename GACTIVE_IMP>
				inline static WRAPPER & From(WRAPPER & wrapper,GACTIVE_IMP * pObject)
				{
					Make<single_data_header>::From(wrapper,PLAYER_EXTPROP_DEFENSE);
					wrapper << pObject->_parent->ID.id;
					wrapper.push_back(pObject->_cur_prop.resistance, sizeof(int)* 6);
					return wrapper << pObject->_cur_prop.defense << pObject->_cur_prop.armor;
				}
		};

		template<>
		struct Make<CMD::team_leader_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader, int seq,short pickupflag, int familyid, int mafiaid, int level, int sectid, int referid)
			{
				Make<single_data_header>::From(wrapper,TEAM_LEADER_INVITE);
				return wrapper << leader.id << seq << pickupflag << familyid << mafiaid << level << sectid << referid;
			}
		};
		
		template<>
		struct Make<CMD::team_reject_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & member)
			{
				Make<single_data_header>::From(wrapper,TEAM_REJECT_INVITE);
				return wrapper << member.id;
			}
		};
		
		template<>
		struct Make<CMD::team_join_team>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader,short pickup_flag)
			{
				Make<single_data_header>::From(wrapper,TEAM_JOIN_TEAM);
				return wrapper << leader.id << pickup_flag;
			}
		};
		
		template<>
		struct Make<CMD::team_member_leave>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader,const XID & member,short type)
			{
				Make<single_data_header>::From(wrapper,TEAM_MEMBER_LEAVE);
				return wrapper << leader.id << member.id << type;
			}
		};
		
		template<>
		struct Make<CMD::team_leave_party>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader,short type)
			{
				Make<single_data_header>::From(wrapper,TEAM_LEAVE_PARTY);
				return wrapper << leader.id << type;
			}
		};

		template<>
		struct Make<CMD::team_new_member>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & member)
			{
				Make<single_data_header>::From(wrapper,TEAM_NEW_MEMBER);
				return wrapper << member.id;
			}
		};

		template<>
		struct Make<CMD::team_leader_cancel_party>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader)
			{
				Make<single_data_header>::From(wrapper,TEAM_LEADER_CANCEL_PARTY);
				return wrapper << leader.id;
			}
		};

		template<>
		struct Make<CMD::team_member_data>
		{
			template <typename WRAPPER,typename MEMBER_ENTRY>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader,unsigned char team_count, unsigned char data_count, const MEMBER_ENTRY ** list)
			{
				Make<single_data_header>::From(wrapper,TEAM_MEMBER_DATA);
				wrapper << team_count << data_count << leader.id;
				for(size_t i = 0; i < data_count; i ++)
				{
					const MEMBER_ENTRY *ent = list[i];
					wrapper << ent->id.id << ent->data.level << ent->data.dt_level 
						<< ent->data.combat_state 
						<< (char) ent->data.pk_level << (char) (ent->data.wallow_level - 1)
						<< ent->data.cls_type 
						<< ent->data.hp << ent->data.mp << ent->data.dp << ent->data.max_hp 
						<< ent->data.max_mp << ent->data.max_dp << ent->data.family_id << ent->data.sect_master_id << ent->data.reborn_cnt << ent->data.rage;
				}
				return wrapper;
			}
			template <typename WRAPPER,typename MEMBER_ENTRY>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader,unsigned char team_count, const MEMBER_ENTRY * list)
			{
				Make<single_data_header>::From(wrapper,TEAM_MEMBER_DATA);
				wrapper << team_count << team_count << leader.id;
				for(size_t i = 0; i < team_count; i ++)
				{
					const MEMBER_ENTRY &ent = list[i];
					wrapper << ent.id.id << ent.data.level << ent.data.dt_level 
						<< ent.data.combat_state 
						<< (char) ent.data.pk_level << (char) (ent.data.wallow_level - 1)
						<< ent.data.cls_type << ent.data.hp << ent.data.mp 
						<< ent.data.dp << ent.data.max_hp << ent.data.max_mp << ent.data.max_dp 
						<< ent.data.family_id << ent.data.sect_master_id << ent.data.reborn_cnt << ent.data.rage;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::teammate_pos>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & id,const A3DVECTOR & pos,int tag)
			{
				Make<single_data_header>::From(wrapper,TEAMMATE_POS);
				return wrapper << id.id << pos << tag;
			}
		};

		template<>
		struct Make<CMD::send_equipment_info>
		{
			template <typename WRAPPER, typename OCTETS, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER* pPlayer,unsigned int mask,OCTETS &data)
			{
				Make<single_data_header>::From(wrapper,EQUIPMENT_DATA);
				wrapper << (unsigned short)pPlayer->crc << pPlayer->ID.id << mask;
				if(size_t rst = data.size())
				{
					ASSERT(rst % 4 == 0);
					wrapper.push_back(data.begin(),rst);
				}
				return  wrapper ;
			}
		};
		
		template<>
		struct Make<CMD::send_clone_equipment_info>
		{
			template <typename WRAPPER, typename OCTETS, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER* pPlayer,int clone_id,unsigned int mask,OCTETS &data)
			{
				Make<single_data_header>::From(wrapper,SEND_CLONE_EQUIPMENT_INFO);
				wrapper << clone_id;
				Make<INFO::player_info_1>::From(wrapper,pPlayer);
				wrapper << (unsigned short)pPlayer->crc << mask;
				if(size_t rst = data.size())
				{
					ASSERT(rst % 4 == 0);
					wrapper.push_back(data.begin(),rst);
				}
				return  wrapper ;
			}
		};
		
		template<>
		struct Make<CMD::equipment_info_changed>
		{
			template <typename WRAPPER, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER* pPlayer,unsigned int mask_add,unsigned int mask_del,const void * buf, size_t size)
			{
				Make<single_data_header>::From(wrapper,EQUIPMENT_INFO_CHANGED);
				wrapper << (unsigned short)pPlayer->crc << pPlayer->ID.id << mask_add << mask_del;
				if(size)
				{
					ASSERT(size % 4 == 0);
					wrapper.push_back(buf,size);
				}
				return  wrapper ;
			}
		};

		template<>
		struct Make<CMD::equipment_damaged>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char index)
			{
				Make<single_data_header>::From(wrapper,EQUIPMENT_DAMAGED);
				return wrapper << index;
			}
		};

		template<>
		struct Make<CMD::team_member_pickup>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id, int type, int count)
			{
				Make<single_data_header>::From(wrapper,TEAM_MEMBER_PICKUP);
				return wrapper << id.id << type << count;
			}
		};

		template<>
		struct Make<CMD::npc_greeting>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & provider)
			{
				Make<single_data_header>::From(wrapper,NPC_GREETING);
				return wrapper << provider.id;
			}
		};

		template<>
		struct Make<CMD::npc_service_content>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & self,int type, const void * buf ,size_t size)
			{
				Make<single_data_header>::From(wrapper,NPC_SERVICE_CONTENT);
				wrapper << self.id << type << size;
				if(size) wrapper.push_back(buf,size);
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::player_purchase_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & FirstStep(WRAPPER & wrapper, int trade_id, size_t cost,unsigned short all_amount)
			{
				Make<single_data_header>::From(wrapper,PURCHASE_ITEM);
				return wrapper << trade_id << cost << (unsigned char)0 << all_amount ;
			}

			template <typename WRAPPER>
			inline static WRAPPER & FirstStep(WRAPPER & wrapper, int trade_id, size_t cost,unsigned short all_amount,bool)
			{
				Make<single_data_header>::From(wrapper,PURCHASE_ITEM);
				return wrapper << trade_id << cost << (unsigned char)1 << all_amount;
			}
			template <typename WRAPPER,typename ITEM_DATA>
			inline static WRAPPER & SecondStep(WRAPPER & wrapper, const ITEM_DATA *pData,unsigned short count, unsigned short inv_index,int state)
			{
				return wrapper << pData->type << pData->expire_date << count << inv_index << (unsigned char)0 << state;
			}

			template <typename WRAPPER>
			inline static WRAPPER & SecondStep(WRAPPER & wrapper,int type, int expire_date ,unsigned short count, unsigned short inv_index,int state)
			{
				return wrapper << type << expire_date << count << inv_index << (unsigned char)0 << state;
			}

			template <typename WRAPPER>
			inline static WRAPPER & SecondStep(WRAPPER & wrapper,int type, int expire_date,unsigned short count, unsigned short inv_index, unsigned char stall_index,int state)
			{
				return wrapper << type << expire_date << count << inv_index << stall_index << state;
			}
		};

		template<>
		struct Make<CMD::item_to_money>
		{
			template < typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short index, int type, size_t count,size_t money)
			{
				Make<single_data_header>::From(wrapper,ITEM_TO_MONEY);
				return wrapper << index << type << count << money;
			}
		};

		template<>
		struct Make<CMD::repair_all>
		{
			template < typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t cost)
			{
				Make<single_data_header>::From(wrapper,REPAIR_ALL);
				return wrapper << cost;
			}
		};

		template<>
		struct Make<CMD::repair>
		{
			template < typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char where,unsigned char index,size_t cost)
			{
				Make<single_data_header>::From(wrapper,REPAIR);
				return wrapper << where << index << cost;
			}
		};

		template<>
		struct Make<CMD::renew>
		{
			template < typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,RENEW);
			}
		};

		template<>
		struct Make<CMD::spend_money>
		{
			template < typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,size_t cost)
			{
				Make<single_data_header>::From(wrapper,SPEND_MONEY);
				return wrapper << cost;
			}
		};

		template <>
		struct Make<CMD::player_pickup_money_in_trade>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t amount)
			{
				Make<single_data_header>::From(wrapper,PICKUP_MONEY_IN_TRADE);
				return wrapper << amount;
			}
		};

		template <>
		struct Make<CMD::player_pickup_item_in_trade>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type, unsigned short amount)
			{
				Make<single_data_header>::From(wrapper,PICKUP_ITEM_IN_TRADE);
				return wrapper << type << amount;
			}
		};

		template <>
		struct Make<CMD::player_pickup_money_after_trade>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t amount)
			{
				Make<single_data_header>::From(wrapper,PICKUP_MONEY_AFTER_TRADE);
				return wrapper << amount;
			}
		};

		template <>
		struct Make<CMD::player_pickup_item_after_trade>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type,int expire_date, unsigned short amount,unsigned short slot_amount,unsigned short index,int state)
			{
				Make<single_data_header>::From(wrapper,PICKUP_ITEM_AFTER_TRADE);
				return wrapper << type << expire_date << amount << slot_amount << index << state;
			}
		};
		
		template <>
		struct Make<CMD::get_own_money>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t amount,size_t capacity)
			{
				Make<single_data_header>::From(wrapper,GET_OWN_MONEY);
				return wrapper << amount << capacity;
			}
		};

		template<>
		struct Make<CMD::object_attack_once>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char proj_amount)
			{
				Make<single_data_header>::From(wrapper,OBJECT_ATTACK_ONCE);
				return wrapper << proj_amount;
			}
		};

		template<>
		struct Make<CMD::object_cast_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & caster, int target_cnt, const XID* targets, int skill,unsigned short time, unsigned char level, unsigned char attack_stamp, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index)
			{
				Make<single_data_header>::From(wrapper,OBJECT_CAST_SKILL);
				wrapper << caster.id << skill << time << level << attack_stamp << state << cast_speed_rate << pos << spirit_index << target_cnt;
				for(int i = 0; i < target_cnt; i ++)
				{
					wrapper << targets[i].id;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::skill_interrupted>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & caster, char spirit_index)
			{
				Make<single_data_header>::From(wrapper,SKILL_INTERRUPTED);
				return wrapper << caster.id << spirit_index;
			}
		};
		
		template<>
		struct Make<CMD::self_skill_interrupted>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char reason, char spirit_index)
			{
				Make<single_data_header>::From(wrapper,SELF_SKILL_INTERRUPTED);
				return wrapper << reason << spirit_index;
			}
		};

		template<>
		struct Make<CMD::skill_perform>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char spirit_index)
			{
				Make<single_data_header>::From(wrapper,SKILL_PERFORM);
				return wrapper << spirit_index;
			}
		};

		template <>
		struct Make<object_be_attacked>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id)
			{
				Make<single_data_header>::From(wrapper,OBJECT_BE_ATTACKED);
				return wrapper << id.id;
			}
		};

		template <>
		struct Make<skill_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void *buf, size_t size)
			{
				Make<single_data_header>::From(wrapper,SKILL_DATA);
				wrapper.push_back(buf,size);
				return wrapper;
			}
		};

		template <>
		struct Make<player_use_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,char where, unsigned char index, int item_id, unsigned short use_count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_USE_ITEM);
				return wrapper << where << index << item_id << use_count;
			}
		};

		template <>
		struct Make<embed_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char chip, unsigned char equip)
			{
				Make<single_data_header>::From(wrapper,EMBED_ITEM);
				return wrapper << chip << equip;
			}
		};

		template <>
		struct Make<clear_embedded_chip>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned short equip_idx, size_t cost)
			{
				Make<single_data_header>::From(wrapper,CLEAR_EMBEDDED_CHIP);
				return wrapper << equip_idx << cost;
			}
		};

		template <>
		struct Make<cost_skill_point>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int sp)
			{
				Make<single_data_header>::From(wrapper,COST_SKILL_POINT);
				return wrapper << sp;
			}
		};

		template <>
		struct Make<learn_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int skill,int level)
			{
				Make<single_data_header>::From(wrapper,LEARN_SKILL);
				return wrapper << skill << level;
			}
		};

		template <>
		struct Make<object_takeoff>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObj)
			{
				Make<single_data_header>::From(wrapper,OBJECT_TAKEOFF);
				return wrapper << pObj->ID.id;
			}
		};

		template <>
		struct Make<object_landing>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObj)
			{
				Make<single_data_header>::From(wrapper,OBJECT_LANDING);
				return wrapper << pObj->ID.id;
			}
		};

		template <>
		struct Make<flysword_time_capacity>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char index, int cur_time)
			{
				Make<single_data_header>::From(wrapper,FLYSWORD_TIME_CAPACITY);
				return wrapper << where << index << cur_time;
			}
		};
		
		template <>
		struct Make<CMD::player_obtain_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type,int expire_date, unsigned short amount,unsigned short slot_amount,unsigned char where,unsigned char index, int state)
			{
				Make<single_data_header>::From(wrapper,OBTAIN_ITEM);
				return wrapper << type << expire_date << amount << slot_amount << where << index << state;
			}
		};
		
		template <>
		struct Make<CMD::produce_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type, unsigned short use_time,unsigned short count)
			{
				Make<single_data_header>::From(wrapper,PRODUCE_START);
				return wrapper << use_time << count << type ;
			}
		};
		
		template <>
		struct Make<CMD::produce_once>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type, unsigned short amount,unsigned short slot_amount,unsigned char where,unsigned char index,int state)
			{
				Make<single_data_header>::From(wrapper,PRODUCE_ONCE);
				return wrapper << type << amount << slot_amount << where << index << state;
			}
		};

		template <>
		struct Make<CMD::produce_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,PRODUCE_END);
			}
		};
		
		template <>
		struct Make<CMD::decompose_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned short use_time, int type)
			{
				Make<single_data_header>::From(wrapper,DECOMPOSE_START);
				return wrapper << use_time << type;
			}
		};

		template <>
		struct Make<CMD::decompose_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,DECOMPOSE_END);
			}
		};
		
		template <>
		struct Make<CMD::task_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void * active,size_t asize, const void * finished, size_t fsize, const void * finished_time, size_t tsize)
			{
				Make<single_data_header>::From(wrapper,TASK_DATA);
				wrapper << asize;
				wrapper.push_back(active,asize);
				wrapper << fsize;
				wrapper.push_back(finished,fsize);
				wrapper << tsize;
				wrapper.push_back(finished_time,tsize);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::task_var_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void * buf,size_t size)
			{
				Make<single_data_header>::From(wrapper,TASK_VAR_DATA);
				wrapper << size;
				wrapper.push_back(buf,size);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::object_start_use>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer,int type, unsigned short time)
			{
				Make<single_data_header>::From(wrapper,OBJECT_START_USE);
				return wrapper << pPlayer->ID.id << type << time;
			}
		};
		
		template <>
		struct Make<CMD::object_cancel_use>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer)
			{
				Make<single_data_header>::From(wrapper,OBJECT_CANCEL_USE);
				return wrapper << pPlayer->ID.id;
			}
		};

		template <>
		struct Make<CMD::object_use_item>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer,int type)
			{
				Make<single_data_header>::From(wrapper,OBJECT_USE_ITEM);
				return wrapper << pPlayer->ID.id  << type;
			}
		};

		template <>
		struct Make<CMD::object_start_use_with_target>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer,int type, unsigned short time,const XID & target)
			{
				Make<single_data_header>::From(wrapper,OBJECT_START_USE_WITH_TARGET);
				return wrapper << pPlayer->ID.id << target.id << type << time;
			}
		};
		
		template <>
		struct Make<CMD::object_sit_down>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj)
			{
				Make<single_data_header>::From(wrapper,OBJECT_SIT_DOWN);
				//return wrapper << pObj->ID.id << pObj->pos; $$$$$$$4
				return wrapper << pObj->ID.id;
			}
		};

		template <>
		struct Make<CMD::object_stand_up>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj)
			{
				Make<single_data_header>::From(wrapper,OBJECT_STAND_UP);
				return wrapper << pObj->ID.id;
			}
		};
		template <>
		struct Make<CMD::object_do_emote>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,unsigned short emotion)
			{
				Make<single_data_header>::From(wrapper, OBJECT_DO_EMOTE);
				return wrapper << pObj->ID.id << emotion ;
			}
		};

		template <>
		struct Make<CMD::server_timestamp>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int timestamp, int timezone_bias, int lua_version)
			{
				Make<single_data_header>::From(wrapper, SERVER_TIMESTAMP );
				return wrapper << timestamp  << timezone_bias << lua_version;
			}
		};

		template <>
		struct Make<CMD::notify_root>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObject)
			{
				Make<single_data_header>::From(wrapper, NOTIFY_ROOT);
				return wrapper << pObject->ID.id << pObject->pos;
			}
		};

		template <>
		struct Make<CMD::self_notify_root>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObject,unsigned char type)
			{
				Make<single_data_header>::From(wrapper, SELF_NOTIFY_ROOT);
				return wrapper << pObject->pos << type;
			}
		};

		template <>
		struct Make<CMD::dispel_root>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper , unsigned char type)
			{
				return Make<single_data_header>::From(wrapper, DISPEL_ROOT) << type;
			}
		};

		template <>
		struct Make<CMD::pk_level_notify>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, char level)
			{
				Make<single_data_header>::From(wrapper, PK_LEVEL_NOTIFY);
				return wrapper << pObj->ID.id << level;
			}
		};

		template <>
		struct Make<CMD::pk_value_notify>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int value)
			{
				Make<single_data_header>::From(wrapper, PK_VALUE_NOTIFY);
				return wrapper << pObj->ID.id << value;
			}
		};


		template <>
		struct Make<CMD::self_stop_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char spirit_index)
			{
				Make<single_data_header>::From(wrapper, SELF_STOP_SKILL);
				return wrapper << spirit_index;
			}
		};
		

		template <>
		struct Make<CMD::update_visible_state>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObj,int64_t state)
			{
				Make<single_data_header>::From(wrapper, UPDATE_VISIBLE_STATE);
				return wrapper << pObj->ID.id << state;
			}
		};

		template <>
		struct Make<CMD::object_buff_notify>
		{       
			template <typename WRAPPER, typename BUFF>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id,const BUFF *state_list,size_t count)
			{                               
				Make<single_data_header>::From(wrapper, OBJECT_BUFF_NOTIFY);
				unsigned short scount = count;
				wrapper << id.id  << scount;
				wrapper.push_back(state_list, ((int)scount)*sizeof(BUFF));
				return wrapper;
			}
		};      

		template <>
		struct Make<CMD::self_buff_notify>
		{
			template <typename WRAPPER, typename BUFF>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & id,const BUFF *state_list,size_t count)
			{
				Make<single_data_header>::From(wrapper, SELF_BUFF_NOTIFY);
				unsigned short scount = count;
				wrapper << id.id  << scount;
				wrapper.push_back(state_list, ((int)scount)*sizeof(BUFF));
				return wrapper;
			}
		};      
	
		template <>
		struct Make<CMD::player_gather_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & player, int mine,unsigned char use_time)
			{
				Make<single_data_header>::From(wrapper, PLAYER_GATHER_START);
				return wrapper << player.id << mine  << use_time;
			}
		};

		template <>
		struct Make<CMD::player_gather_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & player)
			{
				Make<single_data_header>::From(wrapper, PLAYER_GATHER_STOP);
				return wrapper << player.id;
			}
		};

		template <>
		struct Make<CMD::trashbox_passwd_changed>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char has_passwd)
			{
				Make<single_data_header>::From(wrapper, TRASHBOX_PASSWD_CHANGED);
				return wrapper << has_passwd;
			}
		};

		template <>
		struct Make<CMD::trashbox_passwd_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char has_passwd)
			{
				Make<single_data_header>::From(wrapper, TRASHBOX_PASSWD_STATE);
				return wrapper << has_passwd;
			}
		};

		template <>
		struct Make<CMD::trashbox_open>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where ,unsigned short size)
			{
				return Make<single_data_header>::From(wrapper, TRASHBOX_OPEN) << where << size;
			}
		};

		template <>
		struct Make<CMD::trashbox_close>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper, TRASHBOX_CLOSE);
			}
		};

		template <>
		struct Make<CMD::trashbox_wealth>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,size_t amount)
			{
				Make<single_data_header>::From(wrapper, TRASHBOX_WEALTH);
				return wrapper << amount;
			}
		};

		template <>
		struct Make<CMD::exchange_trashbox_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char idx1,unsigned char idx2)
			{
				Make<single_data_header>::From(wrapper, EXCHANGE_TRASHBOX_ITEM);
				return wrapper << where << idx1 << idx2;
			}
		};

		template <>
		struct Make<CMD::move_trashbox_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char src,unsigned char dest, size_t amount)
			{
				Make<single_data_header>::From(wrapper, MOVE_TRASHBOX_ITEM);
				return wrapper << where << src << dest << amount;
			}
		};

		template <>
		struct Make<CMD::exchange_trashbox_inventory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char idx_tra,unsigned char idx_inv)
			{
				Make<single_data_header>::From(wrapper, EXCHANGE_TRASHBOX_INVENTORY);
				return wrapper << where << idx_tra << idx_inv;
			}
		};

		template <>
		struct Make<CMD::inventory_item_to_trash>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char src,unsigned char dest, size_t amount)
			{
				Make<single_data_header>::From(wrapper, INVENTORY_ITEM_TO_TRASH);
				return wrapper << where << src << dest << amount;
			}
		};

		template <>
		struct Make<CMD::trash_item_to_inventory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, unsigned char src,unsigned char dest, size_t amount)
			{
				Make<single_data_header>::From(wrapper, TRASH_ITEM_TO_INVENTORY );
				return wrapper << where << src << dest << amount;
			}
		};

		template <>
		struct Make<CMD::exchange_trash_money>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int inv_money,int tra_money)
			{
				Make<single_data_header>::From(wrapper, EXCHANGE_TRASH_MONEY );
				return wrapper << inv_money << tra_money;
			}
		};

		template <>
		struct Make<CMD::enchant_result>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj, const XID & caster, int skill, char level, char orange_name,char at_state, unsigned char attack_stamp, int value)
			{
				Make<single_data_header>::From(wrapper, ENCHANT_RESULT);
				return wrapper << caster.id << pObj->ID.id << skill << level << orange_name << at_state << attack_stamp << value;
			}
		};

		template <>
		struct Make<CMD::object_do_action>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,unsigned char action)
			{
				Make<single_data_header>::From(wrapper, OBJECT_DO_ACTION);
				return wrapper << pObj->ID.id << action;
			}
		};

		template <>
		struct Make<CMD::player_set_adv_data>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,int data1,int data2)
			{
				Make<single_data_header>::From(wrapper, PLAYER_SET_ADV_DATA);
				return wrapper << pObj->ID.id << data1 << data2;
			}
		};

		template <>
		struct Make<CMD::player_clr_adv_data>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj)
			{
				Make<single_data_header>::From(wrapper, PLAYER_CLR_ADV_DATA);
				return wrapper << pObj->ID.id;
			}
		};

		template <>
		struct Make<CMD::player_in_team>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,unsigned char state)
			{
				Make<single_data_header>::From(wrapper, PLAYER_IN_TEAM);
				return wrapper << pObj->ID.id << state;
			}
		};
		
		template <>
		struct Make<CMD::team_apply_request>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int id, int familyid, int mafiaid, int level, int sectid, int referid)
			{
				Make<single_data_header>::From(wrapper, TEAM_APPLY_REQUEST);
				return wrapper << id << familyid << mafiaid << level << sectid << referid;
			}
		};

		template <>
		struct Make<CMD::object_do_emote_restore>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,unsigned short emotion)
			{
				Make<single_data_header>::From(wrapper, OBJECT_DO_EMOTE_RESTORE);
				return wrapper << pObj->ID.id << emotion ;
			}
		};
		
		template <>
		struct Make<CMD::concurrent_emote_request>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int id, unsigned short action)
			{
				Make<single_data_header>::From(wrapper, CONCURRENT_EMOTE_REQUEST);
				return wrapper << id << action;
			}
		};

		template <>
		struct Make<CMD::do_concurrent_emote>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int self_id, int id, unsigned short action)
			{
				Make<single_data_header>::From(wrapper, DO_CONCURRENT_EMOTE);
				return wrapper << self_id << id << action;
			}
		};

		template <>
		struct Make<CMD::matter_pickup>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj,int who)
			{
				Make<single_data_header>::From(wrapper,MATTER_PICKUP);
				return wrapper << pObj->ID.id << who;
			}
		};

		template <>
		struct Make<CMD::mafia_info_notify>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pObj)
			{
				Make<single_data_header>::From(wrapper,MAFIA_INFO_NOTIFY);
				return wrapper << pObj->ID.id << pObj->id_mafia << pObj->id_family << pObj->rank_mafia;
			}
		};

		template <>
		struct Make<CMD::mafia_trade_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,MAFIA_TRADE_START);
			}
		};

		template <>
		struct Make<CMD::mafia_trade_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,MAFIA_TRADE_END);
			}
		};

		template <>
		struct Make<CMD::task_deliver_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type, int expire_date, int state, unsigned short amount,unsigned short slot_amount,unsigned char where,unsigned char index)
			{
				Make<single_data_header>::From(wrapper,TASK_DELIVER_ITEM);
				return wrapper << type << expire_date << amount << slot_amount << where << index << state;
			}
		};

		template <>
		struct Make<CMD::task_deliver_reputaion>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int delta, int reputation) 
			{
				Make<single_data_header>::From(wrapper,TASK_DELIVER_REPUTATION);
				return wrapper << delta << reputation;
			}
		};

		template <>
		struct Make<CMD::task_deliver_exp>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int64_t exp, int sp, int dt_exp) 
			{
				Make<single_data_header>::From(wrapper,TASK_DELIVER_EXP);
				return wrapper << exp << sp << dt_exp;
			}
		};

		template <>
		struct Make<CMD::task_deliver_money>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,size_t delta, size_t cur_money)
			{
				Make<single_data_header>::From(wrapper,TASK_DELIVER_MONEY);
				return wrapper << delta << cur_money;
			}
		};

		template <>
		struct Make<CMD::task_deliver_level2>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer, int level2)
			{
				Make<single_data_header>::From(wrapper,TASK_DELIVER_LEVEL2);
				return wrapper << pPlayer->ID.id <<  level2;
			}
		};

		template <>
		struct Make<CMD::player_reputation>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj, int reputation)
			{
				Make<single_data_header>::From(wrapper,PLAYER_REPUTATION);
				return wrapper << pObj->ID.id << reputation;
			}
		};
		
		template <>
		struct Make<CMD::identify_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,char index, char result)
			{
				Make<single_data_header>::From(wrapper,IDENTIFY_RESULT);
				return wrapper << index << result;
			}
		};

		template <>
		struct Make<CMD::player_change_shape>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,char shape)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_SHAPE);
				return wrapper << pObj->ID.id << shape;
			}
		};

		template <>
		struct Make<CMD::player_enter_sanctuary>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,PLAYER_ENTER_SANCTUARY);
			}
		};
		
		template <>
		struct Make<CMD::player_leave_sanctuary>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,PLAYER_LEAVE_SANCTUARY);
			}
		};

		template <>
		struct Make<INFO::market_goods>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return wrapper << (int)0;
			}

			template <typename WRAPPER, typename ITEM_DATA>
			inline static WRAPPER & SellItem(WRAPPER & wrapper, int type, int count, size_t price, ITEM_DATA & data,int state)
			{
				ASSERT(type == data.type);
				wrapper << type  << count << price << data.expire_date << state;
				if(data.use_wrapper)
				{
					wrapper << (unsigned short)(data.ar.size());
					wrapper.push_back(data.ar.data(), data.ar.size());
				}
				else
				{
					wrapper << (unsigned short)(data.content_length);
					wrapper.push_back(data.item_content,data.content_length);
				}
				return wrapper;
			}

			template <typename WRAPPER>
			inline static WRAPPER & BuyItem(WRAPPER & wrapper, int type, int count, size_t price)
			{
				return wrapper << type << -count << price;
			}
		};

		template <>
		struct Make<CMD::player_market_info>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int market_id, size_t count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MARKET_INFO);
				return wrapper << pObj->ID.id << market_id << count;
			}
			
		};

		template <>
		struct Make<CMD::player_open_market>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, int item_id, OBJECT * pObj, unsigned char market_crc,const char * name, size_t len)
			{
				Make<single_data_header>::From(wrapper,PLAYER_OPEN_MARKET);
				wrapper << item_id << pObj->ID.id << market_crc; 
				if(len > 127) len = 127;
				wrapper << (unsigned char) len;
				wrapper.push_back(name,len);
				return wrapper;
			}
			
		};

		template <>
		struct Make<CMD::self_open_market>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int item_id, unsigned short count)
			{
				Make<single_data_header>::From(wrapper,SELF_OPEN_MARKET);
				return wrapper << index << item_id << count;
			}

			template <typename WRAPPER>
			inline static WRAPPER & AddGoods(WRAPPER & wrapper, int type, unsigned short index, unsigned short count ,size_t price)
			{
				return wrapper << type << index << count << price;
			}
		};

		template <>
		struct Make<CMD::player_cancel_market>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CANCEL_MARKET);
				return wrapper << pObj->ID.id;
			}
		};

		template <>
		struct Make<CMD::player_market_trade_success>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int trader)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MARKET_TRADE_SUCCESS);
				return wrapper << trader;
			}
		};
		template <>
		struct Make<CMD::player_market_name>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer,const char * name, size_t name_len)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MARKET_NAME);
				wrapper << pPlayer->ID.id << pPlayer->market_id;
				if(name_len > 127) name_len = 127;
				wrapper << (unsigned char) name_len;
				wrapper.push_back(name,name_len);
				return wrapper;
			};
		};

		template <>
		struct Make<CMD::player_start_travel>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer,unsigned char vehicle)
			{
				Make<single_data_header>::From(wrapper,PLAYER_START_TRAVEL);
				return wrapper << pPlayer->ID.id << vehicle;
			};
		};

		template <>
		struct Make<CMD::self_start_travel>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, float speed, const A3DVECTOR & dest, int line_no, unsigned char vehicle)
			{
				Make<single_data_header>::From(wrapper,SELF_START_TRAVEL);
				return wrapper << speed << dest << line_no << vehicle;
			};
		};

		template <>
		struct Make<CMD::player_complete_travel>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer, unsigned char vehicle)
			{
				Make<single_data_header>::From(wrapper,PLAYER_COMPLETE_TRAVEL);
				return wrapper <<  pPlayer->ID.id << vehicle; 
			};
		};
		
		template <>
		struct Make<CMD::gm_toggle_invincible>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char tmp)
			{
				Make<single_data_header>::From(wrapper,GM_TOGGLE_INVINCIBLE);
				return wrapper <<  tmp;
			};
		};

		template <>
		struct Make<CMD::gm_toggle_invisible>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char tmp)
			{
				Make<single_data_header>::From(wrapper,GM_TOGGLE_INVISIBLE);
				return wrapper <<  tmp;
			};
		};

		template <>
		struct Make<CMD::self_trace_cur_pos>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const A3DVECTOR & pos, unsigned short seq)
			{
				Make<single_data_header>::From(wrapper,SELF_TRACE_CUR_POS);
				return wrapper <<  pos << seq;
			};
		};

		template <>
		struct Make<CMD::object_cast_instant_skill>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int target_cnt, const XID * target, int skill, unsigned char level, const A3DVECTOR& pos, char spirit_index)
			{
				Make<single_data_header>::From(wrapper,OBJECT_CAST_INSTANT_SKILL);
				wrapper << pObj->ID.id << skill << level << pos << spirit_index << target_cnt;
				for(int i = 0; i < target_cnt; i ++)
				{
					wrapper << target[i].id;
				}
				return wrapper;
			};
		};

		template <>
		struct Make<CMD::activate_waypoint>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short waypoint)
			{
				Make<single_data_header>::From(wrapper,ACTIVATE_WAYPOINT);
				return wrapper << waypoint;
			};
		};

		template <>
		struct Make<CMD::player_waypoint_list>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const unsigned short * wlist,size_t count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_WAYPOINT_LIST);
				wrapper << count;
				if(count) wrapper.push_back(wlist,sizeof(unsigned short)* count);
				return wrapper;
			};
		};

		template <>
		struct Make<CMD::unlock_inventory_slot>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char where, unsigned short index)
			{
				Make<single_data_header>::From(wrapper,UNLOCK_INVENTORY_SLOT);
				return wrapper << where << index;
			};
		};

		template <>
		struct Make<CMD::team_invite_timeout>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int who)
			{
				Make<single_data_header>::From(wrapper,TEAM_INVITE_PLAYER_TIMEOUT);
				return wrapper << who;
			};
		};

		template <>
		struct Make<CMD::player_pvp_no_protect>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PVP_NO_PROTECT);
				return wrapper << pObj->ID.id;
			};
		};

		template <>
		struct Make<CMD::cooldown_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void * buf, size_t size)
			{
				Make<single_data_header>::From(wrapper,COOLDOWN_DATA);
				return wrapper.push_back(buf,size);
			};
		};

		template <>
		struct Make<CMD::skill_ability_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int ability)
			{
				Make<single_data_header>::From(wrapper, SKILL_ABILITY_NOTFIY);
				return wrapper << id << ability;
			};
		};

		template <>
		struct Make<CMD::personal_market_available>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int item_id)
			{
				Make<single_data_header>::From(wrapper, PERSONAL_MARKET_AVAILABLE);
				return wrapper << index << item_id;
			};
		};

		template <>
		struct Make<CMD::breath_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int breath, int breath_capacity)
			{
				Make<single_data_header>::From(wrapper, BREATH_DATA);
				return wrapper << breath << breath_capacity;
			};
		};

		template <>
		struct Make<CMD::player_stop_dive>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper, PLAYER_STOP_DIVE);
			};
		};

		template <>
		struct Make<CMD::trade_away_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int buyer,short inv_idx,int type, size_t count )
			{
				Make<single_data_header>::From(wrapper, TRADE_AWAY_ITEM);
				return wrapper << inv_idx << type << count << buyer;
			};
		};

		template <>
		struct Make<CMD::player_enable_fashion_mode>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj, unsigned char is_enable)
			{
				Make<single_data_header>::From(wrapper, PLAYER_ENABLE_FASHION_MODE);
				return wrapper << pObj->ID.id << (int)pObj->fashion_mask << is_enable ;
			};
		};

		template <>
		struct Make<CMD::enable_free_pvp_mode>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char is_enable)
			{
				Make<single_data_header>::From(wrapper, ENABLE_FREE_PVP_MODE);
				return wrapper << is_enable;
			}
		};

		template <>
		struct Make<CMD::object_is_invalid>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id)
			{
				Make<single_data_header>::From(wrapper, OBJECT_IS_INVALID);
				return wrapper << id;
			}
		};

		template <>
		struct Make<CMD::player_enable_effect>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, short effect)
			{
				Make<single_data_header>::From(wrapper, PLAYER_ENABLE_EFFECT);
				return wrapper << effect << id;
			}
		};

		template <>
		struct Make<CMD::player_disable_effect>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, short effect)
			{
				Make<single_data_header>::From(wrapper, PLAYER_DISABLE_EFFECT);
				return wrapper << effect << id;
			}
		};

		template <>
		struct Make<CMD::enable_resurrect_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, float exp_reduce)
			{
				Make<single_data_header>::From(wrapper, ENABLE_RESURRECT_STATE);
				return wrapper << exp_reduce;
			}
		};

		template <>
		struct Make<CMD::set_cooldown>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int time)
			{
				Make<single_data_header>::From(wrapper, SET_COOLDOWN);
				return wrapper << index << time;
			}
		};

		template <>
		struct Make<CMD::change_team_leader>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & old_leader, const XID & new_leader)
			{
				Make<single_data_header>::From(wrapper, CHANGE_TEAM_LEADER);
				return wrapper << old_leader.id << new_leader.id;
			}
		};

		template <>
		struct Make<CMD::kickout_instance>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int tag, int timeout)
			{
				Make<single_data_header>::From(wrapper, KICKOUT_INSTANCE);
				return wrapper << tag << timeout;
			}
		};

		template <>
		struct Make<CMD::player_cosmetic_begin>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned short inv_index)
			{
				return Make<single_data_header>::From(wrapper, PLAYER_COSMETIC_BEGIN) << inv_index;
			}
		};

		template <>
		struct Make<CMD::player_cosmetic_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned short inv_index)
			{
				return Make<single_data_header>::From(wrapper, PLAYER_COSMETIC_END) << inv_index;
			}
		};

		template <>
		struct Make<CMD::cosmetic_success>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & self, unsigned short crc)
			{
				Make<single_data_header>::From(wrapper, COSMETIC_SUCCESS);
				return wrapper << crc << self.id;
			}
		};

		template <>
		struct Make<CMD::object_cast_pos_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & self, const A3DVECTOR & pos, int skill, unsigned short time , unsigned char level)
			{
				Make<single_data_header>::From(wrapper, OBJECT_CAST_POS_SKILL);
				return wrapper << self.id << pos << skill << time <<  level;
			}
		};

		template <>
		struct Make<CMD::change_move_seq>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned short seq)
			{
				Make<single_data_header>::From(wrapper, CHANGE_MOVE_SEQ);
				return wrapper << seq;
			}
		};

		template <>
		struct Make<CMD::server_config_data>
		{
			template <typename WRAPPER,typename WORLD_MANAGER>
			inline static WRAPPER & From(WRAPPER & wrapper, WORLD_MANAGER* manager, int mall_stamp)
			{
				Make<single_data_header>::From(wrapper, SERVER_CONFIG_DATA);
				//liuyue-facbase
				return wrapper << manager->GetClientTag() << manager->GetRegionTag() << manager->GetPrecinctTag() << mall_stamp;
			}
		};

		template <>
		struct Make<CMD::player_rush_mode>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char is_active)
			{
				Make<single_data_header>::From(wrapper, PLAYER_RUSH_MODE );
				return wrapper << is_active;
			}
		};

		template <>
		struct Make<CMD::trashbox_capacity_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int cap)
			{
				Make<single_data_header>::From(wrapper, TRASHBOX_CAPACITY_NOTIFY );
				return wrapper << cap;
			}
		};

		template <>
		struct Make<CMD::npc_dead_2>
		{
			template <typename WRAPPER,typename GNPC>
			inline static WRAPPER & From(WRAPPER & wrapper, GNPC * pNPC,const XID & killer)
			{
				Make<single_data_header>::From(wrapper,NPC_DEAD_2);
				return wrapper << pNPC->ID.id << killer.id;
			}
		};

		template <>
		struct Make<CMD::produce_null>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int recipe_id)
			{
				Make<single_data_header>::From(wrapper,PRODUCE_NULL);
				return wrapper << recipe_id;
			}
		};

		template <>
		struct Make<CMD::active_pvp_combat_state>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer,char is_active)
			{
				Make<single_data_header>::From(wrapper,ACTIVE_PVP_COMBAT_STATE);
				return wrapper << pPlayer->ID.id << is_active;
			}
		};

		template <>
		struct Make<CMD::duel_recv_request>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & target)
			{
				Make<single_data_header>::From(wrapper,DUEL_RECV_REQUEST);
				return wrapper << target.id;
			}
		};

		template <>
		struct Make<CMD::duel_reject_request>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & target,int reason)
			{
				Make<single_data_header>::From(wrapper,DUEL_REJECT_REQUEST);
				return wrapper << target.id << reason;
			}
		};

		template <>
		struct Make<CMD::duel_prepare>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & target,int delay)
			{
				Make<single_data_header>::From(wrapper,DUEL_PREPARE);
				return wrapper << target.id << delay;
			}
		};

		template <>
		struct Make<CMD::duel_cancel>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & target)
			{
				Make<single_data_header>::From(wrapper,DUEL_CANCEL);
				return wrapper << target.id ;
			}
		};

		template <>
		struct Make<CMD::duel_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & target)
			{
				Make<single_data_header>::From(wrapper,DUEL_START);
				return wrapper << target.id ;
			}
		};

		template <>
		struct Make<CMD::duel_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const XID & who)
			{
				Make<single_data_header>::From(wrapper,DUEL_STOP);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::duel_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & id1, const XID & id2, bool result)
			{
				Make<single_data_header>::From(wrapper,DUEL_RESULT);
				return wrapper << id1.id << id2.id << (char)(result?1:0);
			}
		};

		template <>
		struct Make<CMD::player_bind_request>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_REQUEST);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_bind_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_INVITE);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_bind_request_reply>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,int param)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_REQUEST_REPLY);
				return wrapper << who.id << param;
			}
		};

		template <>
		struct Make<CMD::player_bind_invite_reply>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,int param)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_INVITE_REPLY);
				return wrapper << who.id << param;
			}
		};

		template <>
		struct Make<CMD::player_bind_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & mule, const XID & rider)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_START);
				return wrapper << mule.id << rider.id;
			}
		};

		template <>
		struct Make<CMD::player_bind_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_STOP);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_mounting>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who, int mount_id, char mount_level, char mount_type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MOUNTING);
				return wrapper << who.id << mount_id << mount_level << mount_type;
			}
		};

		template <>
		struct Make<CMD::player_equip_detail>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & self, const void * data, size_t size,const void * data_skill, size_t size_skill,
				const void* data_pet,size_t size_pet,const void* data_pet_equip,size_t size_pet_equip)
			{
				Make<single_data_header>::From(wrapper,PLAYER_EQUIP_DETAIL);
				wrapper << self.id << size;
				wrapper.push_back(data,size);
				wrapper << size_skill;
				wrapper.push_back(data_skill,size_skill);
				wrapper << size_pet;
				wrapper.push_back(data_pet,size_pet);
				wrapper << size_pet_equip;
				wrapper.push_back(data_pet_equip,size_pet_equip);
				return wrapper;
			}
		};
		
		template <>
		struct Make<CMD::else_duel_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & self)
			{
				Make<single_data_header>::From(wrapper,ELSE_DUEL_START);
				return wrapper << self.id ;
			}
		};

		template <>
		struct Make<CMD::enter_battleground>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,
				unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp )
			{
				Make<single_data_header>::From(wrapper,ENTER_BATTLEGROUND);
				wrapper << battle_faction << battle_type << battle_id << end_timestamp; 
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::leave_battleground>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper )
			{
				Make<single_data_header>::From(wrapper,LEAVE_BATTLEGROUND);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::enter_battlefield>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int faction_id_attacker,
				int faction_id_defender,int faction_id_attacker_assistant,int battle_id,int end_timestamp)
			{
				Make<single_data_header>::From(wrapper,ENTER_BATTLEFIELD);
				return wrapper << faction_id_attacker << faction_id_defender
					<< faction_id_attacker_assistant << battle_id << end_timestamp;
			}
		};

		template <>
		struct Make<CMD::leave_battlefield>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper )
			{
				Make<single_data_header>::From(wrapper,LEAVE_BATTLEFIELD);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::battlefield_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				Make<single_data_header>::From(wrapper,BATTLEFIELD_START);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::battlefield_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char result)
			{
				Make<single_data_header>::From(wrapper,BATTLEFIELD_END);
				return wrapper << result;
			}
		};

		template <>
		struct Make<CMD::battlefield_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char count,const battlefield_info::structure_info_t* p)
			{
				Make<single_data_header>::From(wrapper,BATTLEFIELD_INFO);
				wrapper << count;
				for(size_t i = 0;i < count;++i)
				{
					wrapper << p[i].key << p[i].status << p[i].pos <<p[i].hp_factor;
				}
				return wrapper;
			}
		};


		template <>
		struct Make<CMD::turret_leader_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & self, int tid, int leader)
			{
				Make<single_data_header>::From(wrapper,TURRET_LEADER_NOTIFY);
				return wrapper << self.id << tid << leader;
			}
		};

		template <>
		struct Make<CMD::battle_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int result, int attacker_score, int defender_score)
			{
				Make<single_data_header>::From(wrapper,BATTLE_RESULT);
				return wrapper << result << attacker_score << defender_score;
			}
		};

/*		template <>
		struct Make<CMD::battle_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int os, int og, int ds, int dg)
			{
				Make<single_data_header>::From(wrapper,BATTLE_SCORE);
				return wrapper << os << og << ds << dg;
			}
		};*/

		template <>
		struct Make<CMD::player_recipe_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const unsigned short * data, size_t count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_RECIPE_DATA);
				wrapper << (unsigned short)count;
				if(count) wrapper.push_back(data, count*2);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_learn_recipe>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short id)
			{
				Make<single_data_header>::From(wrapper,PLAYER_LEARN_RECIPE);
				return wrapper << id;
			}
		};

		template <>
		struct Make<CMD::player_produce_skill_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int level , int exp)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PRODUCE_SKILL_INFO);
				return wrapper << level << exp;
			}
		};

		template <>
		struct Make<CMD::player_refine_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int item_index, int item_id, int result)
			{
				Make<single_data_header>::From(wrapper,PLAYER_REFINE_RESULT);
				return wrapper << item_index << item_id << result;
			}
		};

		template <>
		struct Make<CMD::player_start_general_operation>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int op, int duration)
			{
				Make<single_data_header>::From(wrapper,PLAYER_START_GENERAL_OPERATION);
				return wrapper << pObj->ID.id << op << duration;
			}
		};

		template <>
		struct Make<CMD::player_stop_general_operation>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int op)
			{
				Make<single_data_header>::From(wrapper,PLAYER_STOP_GENERAL_OPERATION);
				return wrapper << pObj->ID.id << op;
			}
		};

		template <>
		struct Make<CMD::player_change_class>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_CLASS);
				return wrapper << pPlayer->ID.id << (unsigned char) pPlayer->base_info.cls;
			}
		};

		template <>
		struct Make<CMD::player_cash>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int cash, int cash_used, int cash_add2)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CASH);
				return wrapper << cash << cash_used << cash_add2;
			}
		};

		template <>
		struct Make<CMD::player_bonus>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int bonus, int bonus_used)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BONUS);
				return wrapper << bonus << bonus_used;
			}
		};


		template <>
		struct Make<CMD::player_skill_addon>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int skill,int addon)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SKILL_ADDON);
				return wrapper << skill << addon;
			}
		};

		template <>
		struct Make<CMD::player_skill_common_addon>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int addon)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SKILL_COMMON_ADDON);
				return wrapper << addon;
			}
		};

		template <>
		struct Make<CMD::player_extra_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int skill, int addon)
			{
				Make<single_data_header>::From(wrapper,PLAYER_EXTRA_SKILL);
				return wrapper << skill << addon;
			}
		};

		template <>
		struct Make<CMD::player_title_list>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const short * title, size_t count, const short * extra_title, size_t extra_count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_TITLE_LIST);
				wrapper << (count + extra_count);
				for(size_t i = 0; i < count ; i ++)
				{
					wrapper << (short)title[i];
				}
				for(size_t i = 0; i < extra_count ; i ++)
				{
					wrapper << (short)extra_title[i];
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_add_title>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, short title)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ADD_TITLE);
				return wrapper << title;
			}
		};

		template <>
		struct Make<CMD::player_del_title>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, short title)
			{
				Make<single_data_header>::From(wrapper,PLAYER_DEL_TITLE);
				return wrapper << title;
			}
		};

		template <>
		struct Make<CMD::player_region_reputation>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const int * reputation, size_t count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_REGION_REPUTATION);
				wrapper << count;
				return wrapper.push_back(reputation, count * sizeof(int));
			}
		};

		template <>
		struct Make<CMD::player_change_region_reputation>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int new_reputation)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_REGION_REPUTATION);
				return wrapper << index << new_reputation;
			}
		};

		template <>
		struct Make<CMD::player_change_title>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, short title)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_TITLE);
				return wrapper << pObj->ID.id << title;
			}
		};

		template <>
		struct Make<CMD::player_use_title>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, short title)
			{
				Make<single_data_header>::From(wrapper,PLAYER_USE_TITLE);
				return wrapper << title;
			}
		};

		template <>
		struct Make<CMD::player_change_inventory_size>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int new_size)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_INVENTORY_SIZE);
				return wrapper << new_size;
			}
		};

		template <>
		struct Make<CMD::player_change_mountwing_inv_size>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int new_size)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_MOUNTWING_INV_SIZE);
				return wrapper << new_size;
			}
		};

		template <>
		struct Make<CMD::player_bind_success>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short inv_index, int id,int state)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_SUCCESS);
				return wrapper << inv_index << id << state;
			}
		};

		template <>
		struct Make<CMD::player_change_spouse>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int id)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_SPOUSE);
				return wrapper << pObj->ID.id << id;
			}
		};

		template <>
		struct Make<CMD::player_invader_state>
		{
			template <typename WRAPPER,typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, bool bState)
			{
				Make<single_data_header>::From(wrapper,PLAYER_INVADER_STATE);
				return wrapper << pObj->ID.id << (char)(bState?1:0);
			}
		};

		template <>
		struct Make<CMD::player_mafia_contribution>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int con, int fcon)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MAFIA_CONTRIBUTION);
				return wrapper << con << fcon;
			}
		};

		template <>
		struct Make<CMD::lottery_bonus>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int lottery_id, int bonus_level, int bonus_item, int bonus_count, int bonus_money)
			{
				Make<single_data_header>::From(wrapper,LOTTERY_BONUS);
				return wrapper << lottery_id << bonus_level << bonus_item << bonus_count << bonus_money;
			}
		};

		template <>
		struct Make<CMD::recorder_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int stamp)
			{
				Make<single_data_header>::From(wrapper,RECORDER_START);
				return wrapper << stamp;
			}
		};

		template <>
		struct Make<CMD::recorder_check_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const char * data, size_t len )
			{
				Make<single_data_header>::From(wrapper,RECORDER_CHECK_RESULT);
				return wrapper.push_back(data, len);
			}
		};

		template <>
		struct Make<player_use_item_with_arg>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,char where, unsigned char index, int item_id, unsigned short use_count, const char * arg, size_t buf_size)
			{
				Make<single_data_header>::From(wrapper,PLAYER_USE_ITEM_WITH_ARG);
				wrapper << where << index << item_id << use_count;
				wrapper << (unsigned short) buf_size;
				wrapper.push_back(arg, buf_size);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::object_use_item_with_arg>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer,int type,const char * arg, size_t buf_size)
			{
				Make<single_data_header>::From(wrapper,OBJECT_USE_ITEM_WITH_ARG);
				wrapper << pPlayer->ID.id  << type;
				wrapper << (unsigned short) buf_size;
				wrapper.push_back(arg,buf_size);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::mafia_trashbox_capacity_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int cap)
			{
				Make<single_data_header>::From(wrapper,MAFIA_TRASHBOX_CAPACITY);
				return wrapper << cap;
			}
		};

		template <>
		struct Make<CMD::npc_service_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type)
			{
				return Make<single_data_header>::From(wrapper,NPC_SERVICE_RESULT);
			}
		};

		template <>
		struct Make<CMD::battle_flag_change>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, char origin_state, char new_state)
			{
				Make<single_data_header>::From(wrapper,BATTLE_FLAG_CHANGE);
				return wrapper << id << origin_state << new_state;
			}
		};

		template <>
		struct Make<CMD::battle_player_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, unsigned char battle_faction,
				unsigned int battle_score, unsigned short kill, unsigned short death )
			{
				Make<single_data_header>::From(wrapper,BATTLE_FLAG_CHANGE);
				return wrapper << id << battle_faction << battle_score << kill << death;
			}
		};

		template <>
		struct Make<CMD::battle_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, 
				unsigned char attacker_building_left, unsigned char defender_building_left, 
				unsigned char player_count, const battle_info::player_info_in_battle* pInfo )
			{
				Make<single_data_header>::From(wrapper,BATTLE_INFO);
				wrapper << attacker_building_left << defender_building_left << player_count;
				battle_info::player_info_in_battle* pTemp = (battle_info::player_info_in_battle*)pInfo;
				for( size_t i = 0; i < player_count; ++i )
				{
					wrapper << pTemp->id << pTemp->battle_faction
					<< pTemp->battle_score << pTemp->kill << pTemp->death;
					++pTemp;
				}
				return  wrapper ;
			}
		};

		template <>
		struct Make<CMD::battle_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				Make<single_data_header>::From(wrapper,BATTLE_START);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::battle_flag_change_status>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int flag_id, char origin_state, char new_state)
			{
				Make<single_data_header>::From(wrapper,BATTLE_FLAG_CHANGE_STATUS);
				return wrapper << flag_id << origin_state << new_state;
			}
		};

		template <>
		struct Make<CMD::player_wallow_info>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char active, unsigned char level, int msg
					, int play_time, int l_time, int h_time)
			{
				Make<single_data_header>::From(wrapper,PLAYER_WALLOW_INFO);
				return wrapper << active << level << msg << play_time << l_time << h_time;
			}
		};      
		
		template <>
		struct Make<CMD::talisman_exp_notify>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short where, unsigned short index, int new_exp)
			{
				Make<single_data_header>::From(wrapper,TALISMAN_EXP_NOTIFY);
				return wrapper << where << index << new_exp;
			}
		};      

		template <>
		struct Make<CMD::talisman_combine>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int id,  int type1, int type2, int type3)
			{
				Make<single_data_header>::From(wrapper,TALISMAN_COMBINE);
				return wrapper << id << type1 << type2 << type3;
			}
		};      

		template <>
		struct Make<CMD::talisman_value>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int value)
			{
				Make<single_data_header>::From(wrapper,TALISMAN_VALUE_NOTIFY);
				return wrapper << value;
			}
		}; 

		template <>
		struct Make<CMD::auto_bot_begin>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char active )
			{
				Make<single_data_header>::From(wrapper,AUTO_BOT_BEGIN);
				return wrapper << active;
			}
		};      

		template <>
		struct Make<CMD::anti_cheat_bonus>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short bonus_type)
			{
				Make<single_data_header>::From(wrapper,ANTI_CHEAT_BONUS);
				return wrapper << bonus_type;
			}
		};      
		
		template <>
		struct Make<CMD::battle_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t score )
			{
				Make<single_data_header>::From(wrapper,BATTLE_SCORE);
				return wrapper << score;
			}
		};
		
		template <>
		struct Make<CMD::gain_battle_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t inc )
			{
				Make<single_data_header>::From(wrapper,GAIN_BATTLE_SCORE);
				return wrapper << inc;
			}
		};
		
		template <>
		struct Make<CMD::spend_battle_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t dec )
			{
				Make<single_data_header>::From(wrapper,SPEND_BATTLE_SCORE);
				return wrapper << dec;
			}
		};

		template <>
		struct Make<CMD::talent_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int value)
			{
				Make<single_data_header>::From(wrapper,TALENT_NOTIFY);
				return wrapper << value;
			}
		};

		template <>
		struct Make<CMD::notify_safe_lock>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char active, int time, int max_time)
			{
				Make<single_data_header>::From(wrapper,NOTIFY_SAFE_LOCK);
				return wrapper << active << time << max_time;
			}
		};

		template <>
		struct Make<CMD::battlefield_construction_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int res_a,int res_b,unsigned char count,battlefield_construction_info::node_t* pInfo)
			{
				Make<single_data_header>::From(wrapper,BATTLEFIELD_CONSTRUCTION_INFO);
				wrapper << res_a << res_b << count;
				for(size_t i = 0;i < count;++i)
				{
					wrapper << pInfo[i].type << pInfo[i].key
						<< pInfo[i].attr.attack_level << pInfo[i].attr.defence_level
						<< pInfo[i].attr.range_level << pInfo[i].attr.ai_level
						<< pInfo[i].attr.maxhp_level;
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::battlefield_contribution_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char page,unsigned char max_page,unsigned char count,battlefield_contribution_info::node_t* pInfo)
			{
				Make<single_data_header>::From(wrapper,BATTLEFIELD_CONTRIBUTION_INFO);
				wrapper << page << max_page << count;
				for(size_t i = 0;i < count;++i)
				{
					wrapper << pInfo[i].player_id << pInfo[i].res_a << pInfo[i].res_b;
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_equip_pet_bedge>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char inv_index,unsigned char pet_index)
			{
				Make<single_data_header>::From(wrapper,PLAYER_EQUIP_PET_BEDGE);
				return wrapper << inv_index << pet_index;
			}
		};

		template <>
		struct Make<CMD::player_equip_pet_equip>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char inv_index,unsigned char pet_index)
			{
				Make<single_data_header>::From(wrapper,PLAYER_EQUIP_PET_EQUIP);
				return wrapper << inv_index << pet_index;
			}
		};

		template <>
		struct Make<CMD::player_set_pet_status>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_tid,unsigned char main_status,unsigned char sub_status)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SET_PET_STATUS);
				return wrapper << pet_index << pet_tid << main_status << sub_status;
			}
		};

		template <>
		struct Make<CMD::player_summon_pet>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_tid,int pet_id)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SUMMON_PET);
				return wrapper << pet_index << pet_tid << pet_id;
			}
		};

		template <>
		struct Make<CMD::player_recall_pet>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_tid,int pet_id)
			{
				Make<single_data_header>::From(wrapper,PLAYER_RECALL_PET);
				return wrapper << pet_index << pet_tid << pet_id;
			}
		};

		template <>
		struct Make<CMD::player_combine_pet>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index, int pet_tid,unsigned char type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_COMBINE_PET);
				return wrapper << pet_index << pet_tid << type;
			}
		};

		template <>
		struct Make<CMD::player_uncombine_pet>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_tid)
			{
				Make<single_data_header>::From(wrapper,PLAYER_UNCOMBINE_PET);
				return wrapper << pet_index << pet_tid;
			}
		};

		template <>
		struct Make<CMD::player_start_pet_op>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_id,int delay,unsigned char operation)
			{
				Make<single_data_header>::From(wrapper,PLAYER_START_PET_OP);
				return wrapper << pet_index << pet_id << delay << operation;
			}
		};

		template <>
		struct Make<CMD::player_stop_pet_op>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char operation)
			{
				Make<single_data_header>::From(wrapper,PLAYER_STOP_PET_OP);
				return wrapper;// << operation;
			}
		};

		template <>
		struct Make<CMD::player_pet_recv_exp>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index, int pet_id,int cur_exp)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_RECV_EXP);
				return wrapper << pet_index << pet_id << cur_exp;
			}
		};

		template <>
		struct Make<CMD::player_pet_levelup>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_id,int new_level,int cur_exp)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_LEVELUP);
				return wrapper << pet_index << pet_id << new_level << cur_exp;
			}
		};

		template <>
		struct Make<CMD::player_pet_honor_point>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int cur_honor_point,int max_honor_point)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_HONOR_POINT);
				return wrapper << pet_index << cur_honor_point << max_honor_point;
			}
		};

		template <>
		struct Make<CMD::player_pet_hunger_point>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int cur_hunger_point,int max_hunger_point)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_HUNGER_POINT);
				return wrapper << pet_index << cur_hunger_point << max_hunger_point;
			}
		};

		template <>
		struct Make<CMD::player_pet_age_life>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int age,int life)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_AGE_LIFE);
				return wrapper << pet_index << age << life;
			}
		};

		template <>
		struct Make<CMD::player_pet_hp_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int cur_hp,int max_hp,int cur_vp,int max_vp)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_HP_NOTIFY);
				return wrapper << pet_index << cur_hp << max_hp << cur_vp << max_vp;
			}
		};

		template <>
		struct Make<CMD::player_pet_dead>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_DEAD);
				return wrapper << pet_index;
			}
		};

		template <>
		struct Make<CMD::player_pet_ai_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,unsigned char aggro_state,unsigned char stay_state)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_AI_STATE);
				return wrapper << pet_index << aggro_state << stay_state;
			}
		};

		template <>
		struct Make<CMD::player_pet_room_capacity>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char capacity)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_ROOM_CAPACITY);
				return wrapper << capacity;
			}
		};

		template <>
		struct Make<CMD::player_pet_set_auto_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_id,int skill_id,unsigned char set_flag)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_SET_AUTO_SKILL);
				return wrapper << pet_index << pet_id << skill_id << set_flag;
			}
		};

		template <>
		struct Make<CMD::player_pet_set_skill_cooldown>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_id,int cooldown_index,int cooldown_time)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_SET_SKILL_COOLDOWN);
				return wrapper << pet_index << pet_id << cooldown_index << cooldown_time;
			}
		};

		template <>
		struct Make<CMD::battlefield_building_status_change>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char key,unsigned char status,const A3DVECTOR& pos,float hp_factor)
			{
				Make<single_data_header>::From(wrapper,BATTLEFIELD_BUILDING_STATUS_CHANGE);
				return wrapper << key << status << pos << hp_factor;
			}
		};

		template <>
		struct Make<CMD::player_set_pet_rank>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,int pet_tid,unsigned char rank)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SET_PET_RANK);
				return wrapper << pet_index << pet_tid << rank;
			}
		};

		template <>
		struct Make<CMD::player_reborn_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const int * pInfo, size_t count)
			{
				Make<single_data_header>::From(wrapper,PLAYER_REBORN_INFO);
				wrapper << (short)count;
				for(size_t i = 0 ;i < count; i ++)
				{
					wrapper << (short) ((pInfo[i] >> 16) & 0xFFFF) << (short)(pInfo[i] & 0xFFFF);
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::script_message>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int player_id,int pet_id,unsigned short channel_id,unsigned short msg_id)
			{
				Make<single_data_header>::From(wrapper,SCRIPT_MESSAGE);
				return wrapper << player_id << pet_id << channel_id << msg_id;
			}
		};

		template <>
		struct Make<CMD::player_pet_civilization>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int civilization)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_CIVILIZATION);
				return wrapper << civilization;
			}
		};

		template <>
		struct Make<CMD::player_pet_construction>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int construction)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_CONSTRUCTION);
				return wrapper << construction;
			}
		};

		template <>
		struct Make<CMD::player_move_pet_bedge>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char src_index,unsigned char dst_index)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MOVE_PET_BEDGE);
				return wrapper << src_index << dst_index;
			}
		};

		template <>
		struct Make<CMD::player_start_fly>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer, char active, char type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_START_FLY);
				return wrapper << pPlayer->ID.id  << active << type;
			}
		};
		
		template <>
		struct Make<CMD::player_pet_prop_added>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char pet_index,unsigned short maxhp,unsigned short maxmp,unsigned short defence,
					unsigned short attack,unsigned short* resistance,unsigned short hit,unsigned short jouk,float crit_rate,float crit_damage,int equip_mask)
			{
				Make<single_data_header>::From(wrapper,PLAYER_PET_PROP_ADDED);
				wrapper << pet_index << maxhp << maxmp << defence << attack;
				for(int i = 0;i < 6;++i)
				{
					wrapper << resistance[i];
				}
				wrapper << hit << jouk << crit_rate << crit_damage << equip_mask;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::sect_become_disciple>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int self, int master)
			{
				Make<single_data_header>::From(wrapper,SECT_BECOME_DISCIPLE);
				return wrapper << self << master;
			}
		};

		template <>
		struct Make<CMD::cultivation_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int self, char cult)
			{
				Make<single_data_header>::From(wrapper,CULTIVATION_NOTIFY);
				return wrapper << self << cult;
			}
		};

		template <>
		struct Make<CMD::offline_agent_bonus>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int time, int left_time, int64_t exp)
			{
				Make<single_data_header>::From(wrapper,OFFLINE_AGENT_BONUS);
				return wrapper << time << left_time << (double)exp;
			}
		};


		template <>
		struct Make<CMD::offline_agent_time>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int left_time)
			{
				Make<single_data_header>::From(wrapper,OFFLINE_AGENT_TIME);
				return wrapper << left_time; 
			}
		};

		template <>
		struct Make<CMD::spirit_power_decrease>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,short index, short dec_amount, int result)
			{
				Make<single_data_header>::From(wrapper,SPIRIT_POWER_DECREASE);
				return wrapper << index << dec_amount << result;
			}
		};

		template <>
		struct Make<CMD::player_bind_ride_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_RIDE_INVITE);
				return wrapper << who.id;
			}
		};
		
		template <>
		struct Make<CMD::player_bind_ride_invite_reply>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who, int param)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_RIDE_INVITE_REPLY);
				return wrapper << who.id << param;
			}
		};

		template <>
		struct Make<CMD::player_bind_ride_join>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader, const XID & member,  char pos)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_RIDE_JOIN);
				return wrapper << leader.id << member.id << pos;
			}
		};

		template <>
		struct Make<CMD::player_bind_ride_leave>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader, const XID & member, char pos)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_RIDE_LEAVE);
				return wrapper << leader.id << member.id << pos;
			}
		};
		
		template <>
		struct Make<CMD::player_bind_ride_kick>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & leader, char pos)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BIND_RIDE_KICK);
				return wrapper << leader.id << pos;
			}
		};
		
		template <>
		struct Make<CMD::exchange_pocket_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char index1, unsigned char index2)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_POCKET_ITEM);
				return wrapper << index1<< index2;
			}
		};
	
		template <>
		struct Make<CMD::move_pocket_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char index1, unsigned char index2, unsigned short amount)
			{
				Make<single_data_header>::From(wrapper,MOVE_POCKET_ITEM);
				return wrapper << index1 << index2 << amount; 
			}
		};
	
		template <>
		struct Make<CMD::exchange_inventory_pocket_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char idx_poc, unsigned char idx_inv) 
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_INVENTORY_POCKET_ITEM);
				return wrapper << idx_poc << idx_inv;  
			}
		};
	
		template <>
		struct Make<CMD::move_pocket_item_to_inventory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char idx_poc, unsigned char idx_inv, unsigned short amount) 
			{
				Make<single_data_header>::From(wrapper,MOVE_POCKET_ITEM_TO_INVENTORY);
				return wrapper << idx_poc << idx_inv << amount; 
			}
		};
	
		template <>
		struct Make<CMD::move_inventory_item_to_pocket>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char idx_inv, unsigned char idx_poc, unsigned short amount) 
			{
				Make<single_data_header>::From(wrapper,MOVE_INVENTORY_ITEM_TO_POCKET);
				return wrapper << idx_inv << idx_poc << amount; 
			}
		};
	
		template <>
		struct Make<CMD::player_change_pocket_size>
		{   
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int size)
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_POCKET_SIZE);
				return wrapper << size; 
			}
		};
	
		template <>
		struct Make<CMD::move_all_pocket_item_to_inventory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				Make<single_data_header>::From(wrapper,MOVE_ALL_POCKET_ITEM_TO_INVENTORY);
				return wrapper; 
			}
		};
		
		template <>
		struct Make<CMD::move_max_pocket_item_to_inventory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				Make<single_data_header>::From(wrapper,MOVE_MAX_POCKET_ITEM_TO_INVENTORY);
				return wrapper; 
			}
		};
		
		template <>
		struct Make<CMD::notify_fashion_hotkey>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int count, const notify_fashion_hotkey::key_combine *key)
			{
				Make<single_data_header>::From(wrapper, NOTIFY_FASHION_HOTKEY);
				wrapper << count;
				for(int i = 0; i < count; ++i){
					wrapper << key[i].index << key[i].id_head << key[i].id_cloth << key[i].id_shoe;
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::exchange_fashion_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char index1, unsigned char index2)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_FASHION_ITEM);
				return wrapper << index1<< index2;
			}
		};

		template <>
		struct Make<CMD::exchange_mount_wing_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char index1, unsigned char index2)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_MOUNT_WING_ITEM);
				return wrapper << index1<< index2;
			}
		};


		template <>
		struct Make<CMD::exchange_inventory_fashion_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char idx_inv, unsigned char idx_fas)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_INVENTORY_FASHION_ITEM);
				return wrapper << idx_inv << idx_fas;
			}
		};

		template <>
		struct Make<CMD::exchange_inventory_mountwing_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char idx_inv, unsigned char idx_mw)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_INVENTORY_MOUNTWING_ITEM);
				return wrapper << idx_inv << idx_mw;
			}
		};
		
		template <>
		struct Make<CMD::exchange_equipment_fashion_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char idx_equ, unsigned char idx_fas)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_EQUIPMENT_FASHION_ITEM);
				return wrapper << idx_equ << idx_fas;
			}
		};

		template <>
		struct Make<CMD::exchange_equipment_mountwing_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char idx_equ, unsigned char idx_mw)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_EQUIPMENT_MOUNTWING_ITEM);
				return wrapper << idx_equ << idx_mw;
			}
		};
		
		template <>
		struct Make<CMD::exchange_hotkey_equipment_fashion>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char idx_key, unsigned char idx_head, unsigned char idx_cloth, unsigned char idx_shoe)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_HOTKEY_EQUIPMENT_FASHION);
				return wrapper << idx_key << idx_head << idx_cloth << idx_shoe;
			}
		};
		
		template <>
		struct Make<CMD::peep_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id) 
			{
				Make<single_data_header>::From(wrapper,PEEP_INFO);
				return wrapper << id; 
			}
		};
		
		template <>
		struct Make<CMD::self_killingfield_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int score, int rank, int conkill, int pos) 
			{
				Make<single_data_header>::From(wrapper,SELF_KILLINGFIELD_INFO);
				return wrapper << score << rank << conkill << pos; 
			}
		};
		
		template <>
		struct Make<CMD::killingfield_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int attacker_score, int defender_score, int attacker_count, int defender_count, int count, const void *buff) 
			{
				Make<single_data_header>::From(wrapper,KILLINGFIELD_INFO);
				wrapper << attacker_score << defender_score << attacker_count << defender_count <<  count;
				return wrapper.push_back(buff, count*sizeof(killingfield_info::player_info_in_killingfield));	
			}
		};
		
		template <>
		struct Make<CMD::arena_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int kill_count, int score, int apply_count, int remain_count, int count, const void *buff) 
			{
				Make<single_data_header>::From(wrapper,ARENA_INFO);
				wrapper << kill_count << score << apply_count << remain_count << count; 
				return wrapper.push_back(buff, count*sizeof(arena_info::player_info_in_arena));	
			}
		};
		
		template <>
		struct Make<CMD::rank_change>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id,char new_rank) 
			{
				Make<single_data_header>::From(wrapper,RANK_CHANGE);
				return wrapper << id << new_rank; 
			}
		};
		
		template <>
		struct Make<CMD::change_killingfield_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int old_value, int new_value) 
			{
				Make<single_data_header>::From(wrapper, CHANGE_KILLINGFIELD_SCORE);
				return wrapper << old_value << new_value; 
			}
		};
		
		template <>
		struct Make<CMD::player_change_style>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, unsigned char faceid, unsigned char  hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid) 
			{
				Make<single_data_header>::From(wrapper, PLAYER_CHANGE_STYLE);
				return wrapper << id << faceid << hairid << earid << tailid << fashionid; 
			}
		};

		template <>
		struct Make<CMD::player_change_vipstate>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, unsigned char new_state) 
			{
				Make<single_data_header>::From(wrapper, PLAYER_CHANGE_VIPSTATE);
				return wrapper << id << new_state; 
			}
		};
		
		template <>
		struct Make<CMD::crossserver_battle_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const crossserver_battle_info & info) 
			{
				Make<single_data_header>::From(wrapper, CROSSSERVER_BATTLE_INFO);
				return wrapper << info.attacker_score<< info.defender_score<< info.kill_count << info.death_count << 
					info.attacker_off_line_score << info.defender_off_line_score << info.attacker_count << info.defender_count  << info.min_player_deduct_score << info.end_timestamp;
			}
		};

		template<>
		struct Make<CMD::crossserver_team_battle_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER &wrapper, const crossserver_team_battle_info & info, 
					const crossserver_team_battle_info::history_score *phistory_score)
			{
				Make<single_data_header>::From(wrapper, CRSSVR_TEAM_BATTLE_INFO);
				wrapper << info.battle_state << info.cur_state_end_time << info.attacker_score << info.defender_score << info.best_killer_id << info.history_round_count;
				crossserver_team_battle_info::history_score *pTemp = (crossserver_team_battle_info::history_score *)phistory_score;
				for(size_t i = 0; i < info.history_round_count; ++i)
				{
					wrapper << pTemp->attacker_score << pTemp->defender_score;
					++pTemp;
				}

				return wrapper;
			}
		};

		template<>
		struct Make<CMD::notify_circleofdoom_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int sponsor_id, char faction)
			{
				Make<single_data_header>::From(wrapper, NOTIFY_CIRCLE_OF_DOOM_START);
				wrapper << sponsor_id << faction;

				return wrapper;
			}
		};

		template<>
		struct Make<CMD::notify_circleofdoom_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int skill_id, int sponsor_id, int member_count, const int *pmember_id)
			{
				Make<single_data_header>::From(wrapper, NOTIFY_CIRCLE_OF_DOOM_INFO);
				wrapper << skill_id << sponsor_id << member_count;
				return wrapper.push_back(pmember_id, member_count * sizeof(int));
			}
		};

		template<>
		struct Make<CMD::notify_circleofdoom_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int sponsor_id)
			{
				Make<single_data_header>::From(wrapper, NOTIFY_CIRCLE_OF_DOOM_STOP);
				wrapper << sponsor_id;
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::vip_award_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int end_time, int recent_total_cash, int award_count)
			{
				Make<single_data_header>::From(wrapper, PLAYER_VIP_AWARD_INFO);
				wrapper << end_time << recent_total_cash << award_count;
				return wrapper;
			}

			template <typename WRAPPER>
			inline static WRAPPER & FromData(WRAPPER & wrapper, const CMD::vip_award_info::award_data & data)
			{
				wrapper << data.award_id << data.is_obtained;
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::vip_award_change>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int roleid, unsigned char vipaward_level, char is_hide)
			{
				Make<single_data_header>::From(wrapper, VIP_AWARD_CHANGE);
				wrapper << roleid << vipaward_level << is_hide;
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::online_award_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char force, int count)
			{
				Make<single_data_header>::From(wrapper, ONLINE_AWARD_INFO);
				wrapper << force << count;
				return wrapper;
			}

			template <typename WRAPPER>
			inline static WRAPPER & FromData(WRAPPER & wrapper, const CMD::online_award_info::award_data & data)
			{
				wrapper << data.award_id << data.index << data.small_giftbag_id << data.remaining_time;
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::online_award_permit>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int awardid, int awardindex, char permitcode)
			{
				Make<single_data_header>::From(wrapper, ONLINE_AWARD_PERMIT);
				wrapper << awardid << awardindex << permitcode;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::mall_item_price>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int count)
			{
				Make<single_data_header>::From(wrapper, MALL_ITEM_PRICE);
				return wrapper << count;
			}

			template <typename WRAPPER>
			inline static WRAPPER & AddGoods(WRAPPER & wrapper, int good_index, int good_id, int remaining_time)
			{
				return wrapper << good_index << good_id << remaining_time;
			}
		};

		template<>
		struct Make<CMD::player_achievement>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,size_t map_size, const void * map_data, size_t achieve_size, const void * achieve_data, size_t spec_size, const void * spec_buf, 
					size_t award_map_size, const unsigned char* award_map_data)
			{
				Make<single_data_header>::From(wrapper, PLAYER_ACHIEVEMENT);
				wrapper << map_size; 
				if (map_size)
				{
					wrapper.push_back(map_data, map_size);
				}
				wrapper.push_back(achieve_data, achieve_size);
				wrapper << (spec_size)/sizeof(achievement_spec_info);
				if(spec_size)
				{
					wrapper.push_back(spec_buf, spec_size);
				}
				// Youshuang add
				wrapper << award_map_size;
				wrapper.push_back( award_map_data, award_map_size );
				// end
				return wrapper;
			}
		};
		template<>
		struct Make<CMD::player_achievement_finish>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short achieve_id, int achieve_point, int finish_time)
			{
				Make<single_data_header>::From(wrapper, PLAYER_ACHIEVEMENT_FINISH);
				return wrapper << achieve_id << achieve_point << finish_time;  
			}
		};
		template<>
		struct Make<CMD::player_achievement_active>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void * achieve_data, size_t achieve_size)
			{
				Make<single_data_header>::From(wrapper, PLAYER_ACHIEVEMENT_ACTIVE);
				wrapper.push_back(achieve_data, achieve_size); 
				return wrapper;
			}
		};
		template<>
		struct Make<CMD::player_premiss_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short achieve_id, char premiss_id, const void * premiss_data, size_t size)
			{
				Make<single_data_header>::From(wrapper, PLAYER_PREMISS_DATA);
				wrapper << achieve_id << premiss_id; 
				wrapper.push_back(premiss_data, size);
				return wrapper;
			}
		};
		template<>
		struct Make<CMD::player_premiss_finish>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short achieve_id, char premiss_id)
			{
				Make<single_data_header>::From(wrapper, PLAYER_PREMISS_FINISH);
				return wrapper << achieve_id << premiss_id; 
			}
		};
		template<>
		struct Make<CMD::player_achievement_map>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & self, int achieve_point, const void * data, size_t size)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ACHIEVEMENT_MAP);
				wrapper << self.id << achieve_point << size;
				return wrapper.push_back(data,size);
			}
		};
		template<>
		struct Make<CMD::achievement_message>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void * name, char size, unsigned short achieve_id, int param, int finish_time)
			{
				Make<single_data_header>::From(wrapper, ACHIEVEMENT_MESSAGE);
				wrapper << achieve_id << param << size;
				wrapper.push_back(name,size);
				return wrapper << finish_time;
			}
		};
		template<>
		struct Make<CMD::self_instance_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int level_score, int monster_score, int time_score, int death_penalty_score) 
			{
				Make<single_data_header>::From(wrapper, SELF_INSTANCE_INFO);
				return wrapper << level_score << monster_score << time_score << death_penalty_score; 
			}
		};
		template<>
		struct Make<CMD::instance_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int cur_level, const void * data, size_t size)
			{
				Make<single_data_header>::From(wrapper, INSTANCE_INFO);
				wrapper << cur_level;
				wrapper << size;
				return wrapper.push_back(data,size*sizeof(char));
			}
		};
		
		template<>
		struct Make<CMD::skill_continue>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int caster_id, int skill_id, char spirit_index)
			{
				Make<single_data_header>::From(wrapper, SKILL_CONTINUE);
				return wrapper << caster_id << skill_id << spirit_index;
			}
		};
		
		template <>
		struct Make<CMD::player_start_transform>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER * pPlayer, int template_id, char active, char type, char level, char expLevel)
			{
				Make<single_data_header>::From(wrapper,PLAYER_START_TRANSFORM);
				return wrapper << pPlayer->ID.id  << template_id << active << type << level << expLevel;
			}
		};

		template <>
		struct Make<CMD::magic_duration_decrease>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,short index, short dec_amount, int result)
			{
				Make<single_data_header>::From(wrapper,MAGIC_DURATION_DECREASE);
				return wrapper << index << dec_amount << result;
			}
		};
		
		template <>
		struct Make<CMD::magic_exp_notify>
		{  
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned short where, unsigned short index, int new_exp)
			{
				Make<single_data_header>::From(wrapper,MAGIC_EXP_NOTIFY);
				return wrapper << where << index << new_exp;
			}
		};      
		
		template <>
		struct Make<transform_skill_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void *buf, size_t size)
			{
				Make<single_data_header>::From(wrapper,TRANSFORM_SKILL_DATA);
				wrapper.push_back(buf,size);
				return wrapper;
			}
		};
		
		
		template<>
		struct Make<CMD::skill_proficiency>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int proficiency)
			{
				Make<single_data_header>::From(wrapper, SKILL_PROFICIENCY);
				return wrapper << id << proficiency;
			}
		};
		
		template<>
		struct Make<CMD::player_enter_carrier>
		{
			template <typename WRAPPER, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER *pPlayer, int carrier_id,
					const A3DVECTOR & rpos, unsigned char rdir, unsigned char success)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ENTER_CARRIER);
				return wrapper << pPlayer->ID.id << carrier_id << rpos << rdir << success;
			}
		};

		template<>
		struct Make<CMD::player_leave_carrier>
		{
			template <typename WRAPPER, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper,GPLAYER *pPlayer, int carrier_id,
					const A3DVECTOR & pos, unsigned char dir, unsigned char success)
			{
				Make<single_data_header>::From(wrapper,PLAYER_LEAVE_CARRIER);
				return wrapper << pPlayer->ID.id << carrier_id << pos << dir << success;
			}
		};

		template<>
		struct Make<CMD::player_move_on_carrier>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,
					const A3DVECTOR & target, unsigned short use_time,
					unsigned short speed,unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MOVE_ON_CARRIER);
				return Make<INFO::move_info>::From(wrapper,pObj->ID.id,target,use_time,speed,move_mode);
			}
		};
		
		
		template<>
		struct Make<CMD::player_stop_move_on_carrier>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,
					const A3DVECTOR & target, unsigned short speed,
					unsigned char dir, unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,PLAYER_STOP_MOVE_ON_CARRIER);
				return wrapper << pObj->ID.id << target << speed << dir << move_mode;
			}
		};
		
		template<>
		struct Make<CMD::notify_mall_prop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int mall_prop)
			{
				Make<single_data_header>::From(wrapper,NOTIFY_MALL_PROP);
				return wrapper << mall_prop; 
			}
		};
		
		template<>
		struct Make<CMD::send_pk_message>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int killer, int deader)
			{
				Make<single_data_header>::From(wrapper,SEND_PK_MESSAGE);
				return wrapper << killer << deader; 
			}
		};
		
		template<>
		struct Make<CMD::update_combo_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char state, char * color) 
			{
				Make<single_data_header>::From(wrapper,UPDATE_COMBO_STATE);
				wrapper << state; 
				return wrapper.push_back(color, sizeof(char)*5);
			}
		};
		
		template<>
		struct Make<CMD::enter_dim_state>
		{
			template <typename WRAPPER, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer, bool state) 
			{
				Make<single_data_header>::From(wrapper,ENTER_DIM_STATE);
				return wrapper << pPlayer->ID.id << state; 
			}
		};
		
		template<>
		struct Make<CMD::enter_invisible_state>
		{
			template <typename WRAPPER, typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pPlayer, bool state) 
			{
				Make<single_data_header>::From(wrapper,ENTER_INVISIBLE_STATE);
				return wrapper << pPlayer->ID.id << state; 
			}
		};
		
		template<>
		struct Make<CMD::object_charge>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, int id, char type, const A3DVECTOR & targetPos) 
			{
				Make<single_data_header>::From(wrapper,OBJECT_CHARGE);
				return wrapper << pObj->ID.id << type << id << targetPos; 
			}
		};
		
		template<>
		struct Make<CMD::object_try_charge>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char type, int target_id, const A3DVECTOR & targetPos) 
			{
				Make<single_data_header>::From(wrapper,OBJECT_TRY_CHARGE);
				return wrapper  << type << target_id << targetPos; 
			}
		};
		
		template<>
		struct Make<CMD::object_be_charge_to>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT * pObj, char type, int target_id,  const A3DVECTOR & targetPos) 
			{
				Make<single_data_header>::From(wrapper,OBJECT_BE_CHARGE_TO);
				return wrapper  << pObj->ID.id << type << target_id << targetPos; 
			}
		};

		template <>
		struct Make<CMD::enter_territory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,
				unsigned char battle_faction, unsigned char battle_type, int battle_id, int end_timestamp )
			{
				Make<single_data_header>::From(wrapper,ENTER_TERRITORY);
				wrapper << battle_faction << battle_type << battle_id << end_timestamp; 
				return wrapper;
			}
		};
		
		template <>
		struct Make<CMD::leave_territory>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper )
			{
				Make<single_data_header>::From(wrapper,LEAVE_TERRITORY);
				return wrapper;
			}
		};
		
		template <>
		struct Make<CMD::last_logout_time>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int time)
			{
				Make<single_data_header>::From(wrapper,LAST_LOGOUT_TIME);
				return wrapper << time;
			}
		};
		
		template <>
		struct Make<combine_skill_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void *buf, size_t size)
			{
				Make<single_data_header>::From(wrapper,COMBINE_SKILL_DATA);
				wrapper.push_back(buf,size);
				return wrapper;
			}
		};
		
		template <>
		struct Make<CMD::circle_info_notify>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pObj)
			{
				Make<single_data_header>::From(wrapper,CIRCLE_INFO_NOTIFY);
				return wrapper << pObj->ID.id << pObj->circle_id << pObj->circle_title;
			}
		};
		
		template <>
		struct Make<CMD::receive_deliveryd_exp>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int64_t exp, unsigned int award_type)
			{
				Make<single_data_header>::From(wrapper, DELIVERY_EXP_NOTIFY);
				return wrapper << exp << award_type;
			}
		};
		
		template<>
		struct Make<CMD::deity_levelup>
		{
			
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObject, short cur_lvl, char success)
			{
				Make<single_data_header>::From(wrapper,DEITY_LEVELUP);
				return wrapper << pObject->ID.id << cur_lvl << success;
			}
		};
		
		template<>
		struct Make<CMD::object_special_state>
		{
			
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObject, int type, char on)
			{
				Make<single_data_header>::From(wrapper,OBJECT_SPECIAL_STATE);
				return wrapper << pObject->ID.id << type << on;
			}
		};
		
		template <>
		struct Make<CMD::lottery2_bonus>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int lottery_id, int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money)
			{
				Make<single_data_header>::From(wrapper,LOTTERY2_BONUS);
				return wrapper << lottery_id << bonus_index << bonus_level << bonus_item << bonus_count << bonus_money;
			}
		};


		template <>
		struct Make<CMD::gem_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int gem_id, int msg)
			{
				Make<single_data_header>::From(wrapper,GEM_NOTIFY);
				return wrapper << gem_id << msg; 
			}
		};

		template <>
		struct Make<CMD::player_scale>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper, OBJECT *pObject, char on, int scale_ratio)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SCALE);
				return wrapper << pObject->ID.id << on << scale_ratio; 
			}
		};

		template <>
		struct Make<CMD::player_move_cycle_area>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char mode, char type, int idx, float radius, const A3DVECTOR& center)
			{
				Make<single_data_header>::From(wrapper,PLAYER_MOVE_CYCLE_AREA);
				return wrapper << mode << type << idx << radius << center;
			}
		};	

		template <>
		struct Make<CMD::enter_raid>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp )
			{
				Make<single_data_header>::From(wrapper,ENTER_RAID);
				wrapper << raid_faction << raid_type << raid_id << end_timestamp; 
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::leave_raid>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper )
			{
				Make<single_data_header>::From(wrapper,LEAVE_RAID);
				return wrapper;
			}
		};
		/*liuyue-facbase
		template <>
		struct Make<CMD::notify_raid_pos>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper,const A3DVECTOR & pos, int raid_map_id, int tag)
			{
				Make<single_data_header>::From(wrapper,OBJECT_NOTIFY_RAID_POS);
				return wrapper <<  pos << raid_map_id << tag;
			}
		};
		*/
		template <>
		struct Make<CMD::player_raid_counter>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, const void * data,size_t len)
			{
				Make<single_data_header>::From(wrapper, PLAYER_RAID_COUNTER);
				wrapper.push_back(data, len);
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::object_be_moved>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT * pObj,const A3DVECTOR & target, unsigned short use_time, unsigned short speed,unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,OBJECT_BE_MOVED);
				return Make<INFO::move_info>::From(wrapper,pObj->ID.id,target,use_time,speed,move_mode);
			}

			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int id, const A3DVECTOR & target, unsigned short use_time,unsigned short speed,unsigned char move_mode)
			{
				Make<single_data_header>::From(wrapper,OBJECT_BE_MOVED);
				return Make<INFO::move_info>::From(wrapper,id,target,use_time,speed,move_mode);
			}
		};

		template <>
		struct Make<CMD::player_pulling>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, bool on)
			{
				Make<single_data_header>::From(wrapper, PLAYER_PULLING);
				return wrapper << on;
			}
		};

		template <>
		struct Make<CMD::player_be_pulled>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, int player_pulling_id, int player_pulled_id, bool on, unsigned char type)
			{
				Make<single_data_header>::From(wrapper, PLAYER_BE_PULLED);
				return wrapper << player_pulling_id << player_pulled_id << on << type;
			}
		};

		template <>
		struct Make<CMD::lottery3_bonus>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int lottery_id, int bonus_index, int bonus_level, int bonus_item, int bonus_count, int bonus_money)
			{
				Make<single_data_header>::From(wrapper,LOTTERY3_BONUS);
				return wrapper << lottery_id << bonus_index << bonus_level << bonus_item << bonus_count << bonus_money;
			}
		};

		template <>
		struct Make<CMD::raid_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char player_count, const raid_info::player_info_in_raid* pInfo )
			{
				Make<single_data_header>::From(wrapper,RAID_INFO);
				wrapper << player_count;
				raid_info::player_info_in_raid* pTemp = (raid_info::player_info_in_raid*)pInfo;
				for( size_t i = 0; i < player_count; ++i )
				{
					wrapper << pTemp->id << pTemp->cls << pTemp->level << pTemp->reborn_cnt;
					++pTemp;
				}
				return  wrapper ;
			}
		};

		template <>
		struct Make<CMD::trigger_skill_time>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, short skill_id, short time)
			{
				Make<single_data_header>::From(wrapper,TRIGGER_SKILL_TIME);
				return wrapper << skill_id << time;
			}
		};

		template <>
		struct Make<CMD::player_stop_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, char spirit_index)
			{
				Make<single_data_header>::From(wrapper,PLAYER_STOP_SKILL);
				return wrapper << player_id << spirit_index;
			}
		};

		template <>
		struct Make<CMD::player_single_raid_counter>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int map_id, short counter)
			{
				Make<single_data_header>::From(wrapper,PLAYER_SINGLE_RAID_COUNTER);
				return wrapper << map_id << counter;
			}
		};

		template <>
		struct Make<addon_skill_permanent>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int skill,int level)
			{
				Make<single_data_header>::From(wrapper,ADDON_SKILL_PERMANENT);
				return wrapper << skill << level;
			}
		};

		template <>
		struct Make<addon_skill_permanent_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void *buf, size_t size)
			{
				Make<single_data_header>::From(wrapper,ADDON_SKILL_PERMANENT_DATA);
				wrapper.push_back(buf,size);
				return wrapper;
			}
		};

		template <>
		struct Make<zx_five_anni_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,char star, char state)
			{
				Make<single_data_header>::From(wrapper, ZX_FIVE_ANNI_DATA);
				wrapper << star << state;
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::treasure_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,size_t treasure_size, const void * treasure_data) 
			{
				Make<single_data_header>::From(wrapper, TREASURE_INFO);
				if (treasure_size > 0)
				{
					wrapper.push_back(treasure_data, treasure_size);
				}
				return wrapper;
			}
		};
		
		template <>
		struct Make<treasure_region_upgrade>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int region_id, int region_index, bool success, int new_level)
			{
				Make<single_data_header>::From(wrapper,TREASURE_REGION_UPGRADE);
				return wrapper << region_id << region_index << success << new_level;
			}
		};
		
		template <>
		struct Make<treasure_region_unlock>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int region_id, int region_index, bool success)
			{
				Make<single_data_header>::From(wrapper,TREASURE_REGION_UNLOCK);
				return wrapper << region_id << region_index << success;
			}
		};
		
		template <>
		struct Make<treasure_region_dig>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int region_id, int region_index, bool success, char event_id, size_t item_num, void * pItem)
			{
				Make<single_data_header>::From(wrapper,TREASURE_REGION_DIG);
				wrapper << region_id << region_index << success << event_id << item_num;

				for(size_t i = 0; i < item_num; ++i)
				{
					wrapper << *(int*)pItem;
					((int*)pItem)++;
					wrapper << *(int*)pItem;
					((int*)pItem)++;
				}
				return wrapper;
			}
		};
		
		template <>
		struct Make<inc_treasure_dig_count>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int inc_count, int total_dig_count)
			{
				Make<single_data_header>::From(wrapper,INC_TREASURE_DIG_COUNT);
				return wrapper << inc_count << total_dig_count; 
			}
		};

		template <>
		struct Make<raid_global_var>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int var_cnt, const void* buf, size_t size)
			{
				Make<single_data_header>::From(wrapper, RAID_GLOBAL_VAR);
				wrapper << var_cnt;
				wrapper.push_back(buf, size);
				return wrapper;
			}
		};

		template <>
		struct Make<random_tower_monster>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int level, char reenter)
			{
				Make<single_data_header>::From(wrapper, RANDOM_TOWER_MONSTER);
				wrapper << level << reenter;
				return wrapper;
			}
		};

		template <>
		struct Make<random_tower_monster_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int monster_tid, char client_idx)
			{
				Make<single_data_header>::From(wrapper, RANDOM_TOWER_MONSTER_RESULT);
				wrapper << monster_tid << client_idx;
				return wrapper;
			}
		};

		template <>
		struct Make<player_tower_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void* buf, size_t size)
			{
				Make<single_data_header>::From(wrapper, PLAYER_TOWER_DATA);
				wrapper.push_back(buf, size);
				return wrapper;
			}
		};

		template <>
		struct Make<player_tower_monster_list>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void* buf, size_t size)
			{
				Make<single_data_header>::From(wrapper, PLAYER_TOWER_MONSTER_LIST);
				wrapper.push_back(buf, size);
				return wrapper;
			}
		};

		template <>
		struct Make<player_tower_give_skills>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void* buf, size_t size)
			{
				Make<single_data_header>::From(wrapper, PLAYER_TOWER_GIVE_SKILLS);
				wrapper.push_back(buf, size);
				return wrapper;
			}
		};

		template <>
		struct Make<set_tower_give_skill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int skill_id, int skill_level, bool used)
			{
				Make<single_data_header>::From(wrapper, SET_TOWER_GIVE_SKILL);
				wrapper << skill_id << skill_level << used;
				return wrapper;
			}
		};

		template <>
		struct Make<add_tower_monster>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int monster_tid)
			{
				Make<single_data_header>::From(wrapper, ADD_TOWER_MONSTER);
				wrapper << monster_tid; 
				return wrapper;
			}
		};

		template <>
		struct Make<set_tower_reward_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char type, int level, bool deliveried)
			{
				Make<single_data_header>::From(wrapper, SET_TOWER_REWARD_STATE);
				wrapper << type << level << deliveried; 
				return wrapper;
			}
		};
			
		template <>
		struct Make<littlepet_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char level, char cur_award_lvl, int cur_exp) 
			{
				Make<single_data_header>::From(wrapper, LITTLEPET_INFO);
				wrapper << level << cur_award_lvl << cur_exp; 
				return wrapper;
			}
		};
		
		template <>
		struct Make<rune_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, bool active, int rune_score, int customize_info)
			{
				Make<single_data_header>::From(wrapper, RUNE_INFO);
				return wrapper << active << rune_score << customize_info;
			}
		};
		
		template <>
		struct Make<rune_refine_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char type)
			{
				Make<single_data_header>::From(wrapper, RUNE_REFINE_RESULT);
				return wrapper << type;
			}
		};

		template <>
		struct Make<tower_level_pass>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int level, int best_time)
			{
				Make<single_data_header>::From(wrapper, TOWER_LEVEL_PASS);
				return wrapper << level << best_time;
			}
		};
		
		template <>
		struct Make<fill_platform_mask>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int mask) 
			{
				Make<single_data_header>::From(wrapper, FILL_PLATFORM_MASK);
				return wrapper << mask; 
			}
		};
		

		template <>
		struct Make<pk_bet_data>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,const void *buf, size_t size)
			{
				Make<single_data_header>::From(wrapper, PK_BET_DATA);
				wrapper.push_back(buf,size);
				return wrapper;
			}
		};
		
		template <>
		struct Make<CMD::put_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type,int expire_date, unsigned char index, unsigned short amount,unsigned short slot_amount,unsigned char where,int state)
			{
				Make<single_data_header>::From(wrapper,PUT_ITEM);
				return wrapper << type << expire_date << index << amount << slot_amount << where << state; 
			}
		};

		template<>
		struct Make<CMD::object_start_special_move>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp)
			{
				Make<single_data_header>::From(wrapper,OBJECT_START_SPECIAL_MOVE);
				return wrapper << id << velocity << acceleration << cur_pos << collision_state << timestamp; 
			}
		};
		
		template<>
		struct Make<CMD::object_special_move>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp)
			{
				Make<single_data_header>::From(wrapper,OBJECT_SPECIAL_MOVE);
				return wrapper << id << velocity << acceleration << cur_pos << collision_state << timestamp; 
			}
		};
		
		template<>
		struct Make<CMD::object_stop_special_move>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, unsigned char dir, A3DVECTOR & cur_pos) 
			{
				Make<single_data_header>::From(wrapper,OBJECT_STOP_SPECIAL_MOVE);
				return wrapper << id << dir << cur_pos; 
			}
		};
		
		template<>
		struct Make<CMD::player_change_name>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int name_len, char * name) 
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_NAME);
				wrapper << id << name_len; 
				wrapper.push_back(name, name_len);
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::enter_collision_raid>
		{
			template <typename WRAPPER,typename GPLAYER>
			inline static WRAPPER & From(WRAPPER & wrapper, GPLAYER * pObj, unsigned char raid_faction, unsigned char raid_type, int raid_id, int end_timestamp) 
			{
				Make<single_data_header>::From(wrapper,ENTER_COLLISION_RAID);
				return wrapper << pObj->ID.id << raid_faction << raid_type << raid_id << end_timestamp;
			}
		};
		
		template<>
		struct Make<CMD::collision_raid_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int attacker_score, int defender_score, int max_killer_id, size_t count, void * buf) 
			{
				Make<single_data_header>::From(wrapper,COLLISION_RAID_SCORE);
				wrapper << attacker_score << defender_score << max_killer_id << count;

				for(size_t i = 0; i < count; ++i)
				{
					wrapper << *(int*)buf;
					((int*)buf)++;
					wrapper << *(int*)buf;
					((int*)buf)++;
					wrapper << *(int*)buf;
					((int*)buf)++;
					wrapper << *(int*)buf;
					((int*)buf)++;
					((int*)buf)++;
					((int*)buf)++;
				}
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::collision_special_state>
		{
			template <typename WRAPPER, typename OBJECT>
			inline static WRAPPER & From(WRAPPER & wrapper,OBJECT *pObject, int type, char on)
			{
				Make<single_data_header>::From(wrapper,COLLISION_SPECIAL_STATE);
				return wrapper << pObject->ID.id << type << on;
			}
		};
		
		template<>
		struct Make<CMD::change_faction_name>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int fid, char type, int name_len, char * name) 
			{
				Make<single_data_header>::From(wrapper,CHANGE_FACTION_NAME);
				wrapper << id << fid << type << name_len; 
				wrapper.push_back(name, name_len);
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::player_darklight>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int dark_light_value)
			{
				Make<single_data_header>::From(wrapper,PLAYER_DARKLIGHT);
				return wrapper << id << dark_light_value;
			}
		};

		template<>
		struct Make<CMD::player_darklight_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, char state)
			{
				Make<single_data_header>::From(wrapper, PLAYER_DARKLIGHT_STATE);
				return wrapper << id << state;
			}
		};

		template<>
		struct Make<CMD::player_darklight_spirit>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, char darklight_spirits[3])
			{
				Make<single_data_header>::From(wrapper, PLAYER_DARKLIGHT_SPIRIT);
				return wrapper << id << darklight_spirits[0] << darklight_spirits[1] << darklight_spirits[2];
			}
		};

		template<>
		struct Make<CMD::mirror_image_cnt>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, char mirror_image_cnt)
			{
				Make<single_data_header>::From(wrapper, MIRROR_IMAGE_CNT);
				return wrapper << id << mirror_image_cnt;
			}
		};
		
		template<>
		struct Make<CMD::player_wing_change_color>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, unsigned char wing_color)
			{
				Make<single_data_header>::From(wrapper, PLAYER_WING_CHANGE_COLOR);
				return wrapper << id << wing_color;
			}
		};
		
		
		template<>
		struct Make<CMD::ui_transfer_opened_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void *buf, size_t size)
			{
				Make<single_data_header>::From(wrapper, UI_TRANSFER_OPENED_NOTIFY);
				wrapper.push_back(buf, size);
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::liveness_notify>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int cur_point, char is_taken[4], std::vector<int>& achieved_ids)
			{
				Make<single_data_header>::From(wrapper, LIVENESS_NOTIFY);
				wrapper << cur_point; 
				for (size_t i = 0; i < 4; i++) {
					wrapper << is_taken[i];	
				}
				wrapper << achieved_ids.size();
				for (std::vector<int>::iterator it = achieved_ids.begin(); it != achieved_ids.end(); ++it) {
					wrapper << *it;	
				}
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::notify_astrology_energy>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, bool active, int cur_energy)
			{
				Make<single_data_header>::From(wrapper, NOTIFY_ASTROLOGY_ENERGY);
				return wrapper << active << cur_energy; 
			}
		};
		
		template<>
		struct Make<CMD::astrology_upgrade_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, bool success, int new_level)
			{
				Make<single_data_header>::From(wrapper, ASTROLOGY_UPGRADE_RESULT);
				return wrapper << success << new_level; 
			}
		};
		
		template<>
		struct Make<CMD::astrology_destroy_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int gain_energy) 
			{
				Make<single_data_header>::From(wrapper, ASTROLOGY_DESTROY_RESULT);
				return wrapper << gain_energy; 
			}
		};

		template<>
		struct Make<CMD::talisman_refineskill>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int index) 
			{
				Make<single_data_header>::From(wrapper, TALISMAN_REFINESKILL);
				return wrapper << id << index; 
			}
		};
		
		template<>
		struct Make<CMD::collision_raid_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char result, int score_a, int score_b) 
			{
				Make<single_data_header>::From(wrapper, COLLISION_RAID_RESULT);
				return wrapper << result << score_a << score_b; 
			}
		};
		
		template<>
		struct Make<CMD::collision_raid_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int score_a, int score_b, int win_num, int lost_num, int draw_num, int daily_award_timestamp, int last_collision_timestamp, 
					int day_win, int cs_personal_award_timestamp, int cs_exchange_award_timestamp, int cs_team_award_timestamp) 
			{
				Make<single_data_header>::From(wrapper, COLLISION_RAID_INFO);
				return wrapper << score_a << score_b << win_num << lost_num << draw_num << daily_award_timestamp << last_collision_timestamp << day_win
					<<cs_personal_award_timestamp << cs_exchange_award_timestamp << cs_team_award_timestamp;
			}
		};

		template<>
		struct Make<CMD::dir_visible_state>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, int id, int dir, bool on) 
			{
				Make<single_data_header>::From(wrapper, DIR_VISIBLE_STATE);
				return wrapper << player_id << id << dir << on; 
			}
		};
		
		template<>
		struct Make<CMD::player_remedy_metempsychosis_level>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper) 
			{
				Make<single_data_header>::From(wrapper, PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL);
				return wrapper; 
			}
		};
		
		template<>
		struct Make<CMD::collision_player_pos>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int roleid, A3DVECTOR & pos) 
			{
				Make<single_data_header>::From(wrapper, COLLISION_PLAYER_POS);
				return wrapper << roleid << pos; 
			}
		};
		
		template<>
		struct Make<CMD::enter_kingdom>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char battle_faction, char kingdom_type, char cur_phase, 
					int end_timestamp, int defender_mafia_id, std::vector<int> & attacker_mafia_id) 
			{
				Make<single_data_header>::From(wrapper, ENTER_KINGDOM);
				wrapper << battle_faction << kingdom_type << cur_phase << end_timestamp << defender_mafia_id; 
				wrapper << attacker_mafia_id.size();

				for(size_t i = 0; i < attacker_mafia_id.size(); ++i)
				{
					wrapper << attacker_mafia_id[i]; 
				}
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::leave_kingdom>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char kingdom_type) 
			{
				Make<single_data_header>::From(wrapper, LEAVE_KINGDOM);
				return wrapper << kingdom_type; 
			}
		};
		
		template<>
		struct Make<CMD::kingdom_half_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int win_faction, int end_timestamp) 
			{
				Make<single_data_header>::From(wrapper, KINGDOM_HALF_END);
				return wrapper << win_faction << end_timestamp; 
			}
		};
		
		template<>
		struct Make<CMD::kingdom_end_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int win_mafia_id) 
			{
				Make<single_data_header>::From(wrapper, KINGDOM_END_RESULT);
				return wrapper << win_mafia_id; 
			}
		};
		
		template<>
		struct Make<CMD::kingdom_mafia_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, size_t player_count, const char * pInfo) 
			{
				Make<single_data_header>::From(wrapper, KINGDOM_MAFIA_INFO);
				wrapper << player_count;
				kingdom_mafia_info::player_info_in_kingdom * pTemp = (kingdom_mafia_info::player_info_in_kingdom*)pInfo;
				for(size_t i = 0; i < player_count; ++i)
				{
					wrapper << pTemp->roleid;
					wrapper << pTemp->mafia_id;
					wrapper << pTemp->kill_count;
					wrapper << pTemp->death_count;
					pTemp++;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::sale_promotion_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, std::set<int>& taskids) 
			{
				Make<single_data_header>::From(wrapper, SALE_PROMOTION_INFO);
				wrapper << taskids.size();

				for (std::set<int>::iterator it = taskids.begin(); it != taskids.end(); ++it) {
					wrapper << *it;	
				}
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::raid_level_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char level) 
			{
				Make<single_data_header>::From(wrapper, RAID_LEVEL_START);
				return wrapper << level; 
			}
		};

		template<>
		struct Make<CMD::raid_level_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char level, bool result) 
			{
				Make<single_data_header>::From(wrapper, RAID_LEVEL_END);
				return wrapper << level << result; 
			}
		};

		template<>
		struct Make<CMD::raid_level_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, bool final, bool result[10], bool final_award_deliveried, bool level_award_deliveried[10]) 
			{
				Make<single_data_header>::From(wrapper, RAID_LEVEL_RESULT);
				wrapper << final; 
				wrapper.push_back(result, sizeof(bool) * 10);
				wrapper << final_award_deliveried; 
				wrapper.push_back(level_award_deliveried, sizeof(bool) * 10);
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::raid_level_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char level, short matter_cnt, int start_time) 
			{
				Make<single_data_header>::From(wrapper, RAID_LEVEL_INFO);
				return wrapper << level << matter_cnt << start_time; 
			}
		};
		
		template<>
		struct Make<CMD::event_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int event_id) 
			{
				Make<single_data_header>::From(wrapper, EVENT_START);
				return wrapper << event_id; 
			}
		};
		
		template<>
		struct Make<CMD::event_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int event_id) 
			{
				Make<single_data_header>::From(wrapper, EVENT_STOP);
				return wrapper << event_id; 
			}
		};
		
		template<>
		struct Make<CMD::event_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, std::set<int> & event_info) 
			{
				Make<single_data_header>::From(wrapper, EVENT_INFO);
				wrapper << event_info.size(); 

				std::set<int>::iterator iter;
				for(iter = event_info.begin(); iter != event_info.end(); ++iter)
				{
					wrapper << *iter;
				}
				return wrapper;
			}
		};
		
		template<>
		struct Make<CMD::cash_gift_ids>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int gift_id1, int gift_id2) 
			{
				Make<single_data_header>::From(wrapper, CASH_GIFT_IDS);
				return wrapper << gift_id1 << gift_id2;
			}
		};
		
		template<>
		struct Make<CMD::notify_cash_gift>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int gift_id, int gift_index, int gift_score) 
			{
				Make<single_data_header>::From(wrapper, NOTIFY_CASH_GIFT);
				return wrapper << gift_id << gift_index << gift_score;
			}
		};

		template<>
		struct Make<CMD::player_propadd>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int add_prop[30]) 
			{
				Make<single_data_header>::From(wrapper, PLAYER_PROPADD);
				for(int i = 0; i < 30; i ++)
				{
					wrapper << add_prop[i];
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::player_get_propadd_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int item_id, int item_idx) 
			{
				Make<single_data_header>::From(wrapper, PLAYER_GET_PROPADD_ITEM);
				return wrapper << item_id << item_idx;
			}
		};

		template<>
		struct Make<CMD::player_extra_equip_effect>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, char weapon_effect_level)
			{
				Make<single_data_header>::From(wrapper, PLAYER_EXTRA_EQUIP_EFFECT);
				return wrapper << player_id << weapon_effect_level;
			}
		};

		template<>
		struct Make<CMD::kingdom_key_npc_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int cur_hp, int max_hp, int hp_add, int hp_dec) 
			{
				Make<single_data_header>::From(wrapper, KINGDOM_KEY_NPC_INFO);
				return wrapper << cur_hp << max_hp << hp_add << hp_dec; 
			}
		};

		template<>
		struct Make<CMD::notify_kingdom_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int kingdom_title, int kingdom_point) 
			{
				Make<single_data_header>::From(wrapper, NOTIFY_KINGDOM_INFO);
				return wrapper << kingdom_title << kingdom_point; 
			}
		};

		template<>
		struct Make<CMD::cs_flow_player_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, int score, int c_kill_cnt, bool m_kill_change, int m_kill_cnt, short kill_cnt, short death_cnt, short max_ckill_cnt, short max_mkill_cnt)
			{
				Make<single_data_header>::From(wrapper, CSFLOW_BATTLE_PLAYER_INFO);
				return wrapper << player_id << score << c_kill_cnt << m_kill_change << m_kill_cnt << kill_cnt << death_cnt << max_ckill_cnt << max_mkill_cnt; 
			}
		};

		template <>
		struct Make<CMD::player_bath_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BATH_INVITE);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_bath_invite_reply>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,int param)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BATH_INVITE_REPLY);
				return wrapper << who.id << param;
			}
		};

		template <>
		struct Make<CMD::player_bath_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & invitor, const XID & invitee)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BATH_START);
				return wrapper << invitor.id << invitee.id;
			}
		};

		template <>
		struct Make<CMD::player_bath_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_BATH_STOP);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::enter_bath_world>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int remain_bath_count) 
			{
				Make<single_data_header>::From(wrapper,ENTER_BATH_WORLD);
				return wrapper << remain_bath_count; 
			}
		};

		template <>
		struct Make<CMD::leave_bath_world>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper) 
			{
				Make<single_data_header>::From(wrapper,LEAVE_BATH_WORLD);
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::flow_battle_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, unsigned char player_count, const flow_battle_info::player_info_in_flow_battle* pInfo )
			{
				Make<single_data_header>::From(wrapper,FLOW_BATTLE_INFO);
				wrapper << player_count;
				flow_battle_info::player_info_in_flow_battle* pTemp = (flow_battle_info::player_info_in_flow_battle*)pInfo;
				for( size_t i = 0; i < player_count; ++i )
				{
					wrapper << pTemp->id << pTemp->level << pTemp->cls << pTemp->battle_faction << pTemp->kill << pTemp->death << pTemp->zone_id << pTemp->flow_score;
					++pTemp;
				}
				return  wrapper ;
			}
		};

		template <>
		struct Make<CMD::bath_gain_item>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int roleid, int item_id, int item_count) 
			{
				Make<single_data_header>::From(wrapper,BATH_GAIN_ITEM);
				return wrapper << roleid << item_id << item_count; 
			}
		};

		template <>
		struct Make<CMD::kingdom_gather_mine_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int roleid, int mafia_id) 
			{
				Make<single_data_header>::From(wrapper,KINGDOM_GATHER_MINE_START);
				return wrapper << roleid << mafia_id; 
			}
		};

		template <>
		struct Make<CMD::fuwen_compose_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int compose_count, int extra_compose_count) 
			{
				Make<single_data_header>::From(wrapper,FUWEN_COMPOSE_INFO);
				return wrapper << compose_count << extra_compose_count; 
			}
		};

		template <>
		struct Make<CMD::fuwen_compose_end>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper) 
			{
				Make<single_data_header>::From(wrapper,FUWEN_COMPOSE_END);
				return wrapper; 
			}
		};

		template <>
		struct Make<CMD::fuwen_install_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int src_index, int dst_index) 
			{
				Make<single_data_header>::From(wrapper,FUWEN_INSTALL_RESULT);
				return wrapper << src_index << dst_index; 
			}
		};

		template <>
		struct Make<CMD::fuwen_uninstall_result>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int fuwen_index, int inv_index) 
			{
				Make<single_data_header>::From(wrapper,FUWEN_UNINSTALL_RESULT);
				return wrapper << fuwen_index << inv_index; 
			}
		};

		template <>
		struct Make<CMD::multi_exp_mode>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int multi_exp_factor, int timestamp) 
			{
				Make<single_data_header>::From(wrapper, MULTI_EXP_MODE);
				return wrapper << multi_exp_factor << timestamp; 
			}
		};

		template <>
		struct Make<CMD::exchange_item_list_info>
		{	
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,unsigned char where, const void *data, size_t len)
			{
				Make<single_data_header>::From(wrapper,EXCHANGE_ITEM_LIST_INFO);
				wrapper << where << len;
				wrapper.push_back(data,len);
				return wrapper;
			}
		};

		template <>
       		struct Make<CMD::cs_battle_off_line_info>
       		{
               		template <typename WRAPPER>
               		inline static WRAPPER & From(WRAPPER & wrapper, const cs_battle_off_line_info::player_off_line_in_cs_battle* pInfo,unsigned char off_line_count)
               		{
                       		Make<single_data_header>::From(wrapper,CS_BATTLE_OFF_LINE_INFO);
                       		wrapper << off_line_count;
                       		cs_battle_off_line_info::player_off_line_in_cs_battle* pTemp = (cs_battle_off_line_info::player_off_line_in_cs_battle*)pInfo;
                       		for (size_t i = 0; i < off_line_count;i++)
                       		{
                               		wrapper << pTemp->battle_faction << pTemp->_timestamp << pTemp->off_line_count << pTemp->off_line_score;
                               		++pTemp;
                       		}
                       		return wrapper;
              		 }
      		};

		template <>
		struct Make<CMD::control_trap_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int id, int tid, int time) 
			{
				Make<single_data_header>::From(wrapper, CONTROL_TRAP_INFO);
				return wrapper << id << tid << time; 
			}
		};

		template <>
		struct Make<CMD::npc_invisible>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int npc_id, bool on) 
			{
				Make<single_data_header>::From(wrapper, NPC_INVISIBLE);
				return wrapper << npc_id << on; 
			}
		};

		template <>
		struct Make<CMD::puppet_form>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, bool on, int player_id, int num) 
			{
				Make<single_data_header>::From(wrapper, PUPPET_FORM);
				return wrapper << on << player_id << num; 
			}
		};

		template <>
		struct Make<CMD::teleport_skill_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, int npc_tid, int skill_id, int teleport_count) 
			{
				Make<single_data_header>::From(wrapper, TELEPORT_SKILL_INFO);
				return wrapper << player_id << npc_tid << skill_id << teleport_count; 
			}
		};

		template <>
		struct Make<CMD::blow_off>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int roleid) 
			{
				Make<single_data_header>::From(wrapper, BLOW_OFF);
				return wrapper << roleid; 
			}
		};

		template <>
		struct Make<CMD::mob_active_ready_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & player, const XID & mobactive, int pos) 
			{
				Make<single_data_header>::From(wrapper,MOB_ACTIVE_READY_START);
				return wrapper << player.id << mobactive.id << pos;
			}
		};

		template <>
		struct Make<CMD::mob_active_running_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & player, const XID & mobactive, int pos) 
			{
				Make<single_data_header>::From(wrapper,MOB_ACTIVE_RUNNING_START);
				return wrapper << player.id << mobactive.id << pos;
			}
		};

		template <>
		struct Make<CMD::mob_active_end_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & player, const XID & mobactive, int pos) 
			{
				Make<single_data_header>::From(wrapper,MOB_ACTIVE_END_START);
				return wrapper << player.id << mobactive.id << pos;
			}
		};

		template <>
		struct Make<CMD::mob_active_end_finish>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & player, const XID & mobactive, int pos, const A3DVECTOR& player_pos, const A3DVECTOR& mobactive_pos) 
			{
				Make<single_data_header>::From(wrapper,MOB_ACTIVE_END_FINISH);
				return wrapper << player.id << mobactive.id << pos << player_pos << mobactive_pos;
			}
		};

		template <>
		struct Make<CMD::mob_active_cancel>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & player, const XID & mobactive, int pos, const A3DVECTOR& player_pos, const A3DVECTOR& mobactive_pos) 
			{
				Make<single_data_header>::From(wrapper,MOB_ACTIVE_CANCEL);
				return wrapper << player.id << mobactive.id << pos << player_pos << mobactive_pos;
			}
		};

		template <>
		struct Make<CMD::player_qilin_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_QILIN_INVITE);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_qilin_invite_reply>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,int param)
			{
				Make<single_data_header>::From(wrapper,PLAYER_QILIN_INVITE_REPLY);
				return wrapper << who.id << param;
			}
		};

		template <>
		struct Make<CMD::player_qilin_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & invitor, const XID & invitee)
			{
				Make<single_data_header>::From(wrapper,PLAYER_QILIN_START);
				return wrapper << invitor.id << invitee.id;
			}
		};

		template <>
		struct Make<CMD::player_qilin_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_QILIN_STOP);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_qilin_disconnect>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who)
			{
				Make<single_data_header>::From(wrapper,PLAYER_QILIN_DISCONNECT);
				return wrapper << who.id;
			}
		};

		template <>
		struct Make<CMD::player_qilin_reconnect>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who, const A3DVECTOR &newpos)
			{
				Make<single_data_header>::From(wrapper,PLAYER_QILIN_RECONNECT);
				return wrapper << who.id << newpos;
			}
		};

		template <>
		struct Make<CMD::phase_info>
		{
			template <typename WRAPPER>
			static WRAPPER & From(WRAPPER & wrapper, const void * data,size_t len)
			{
				Make<single_data_header>::From(wrapper, PHASE_INFO);
				wrapper.push_back(data, len);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::phase_info_change>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int phase_id, bool on) 
			{
				Make<single_data_header>::From(wrapper,PHASE_INFO_CHANGE);
				return wrapper << phase_id << on; 
			}
		};

		template <>
		struct Make<CMD::fac_building_add>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int tid) 
			{
				Make<single_data_header>::From(wrapper,FAC_BUILDING_ADD);
				return wrapper << index << tid; 
			}
		};

		template <>
		struct Make<CMD::fac_building_upgrade>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int tid, int lev) 
			{
				Make<single_data_header>::From(wrapper,FAC_BUILDING_UPGRADE);
				return wrapper << index << tid << lev; 
			}
		};

		template <>
		struct Make<CMD::fac_building_complete>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index, int tid, int level) 
			{
				Make<single_data_header>::From(wrapper,FAC_BUILDING_COMPLETE);
				return wrapper << index << tid << level; 
			}
		};

		template <>
		struct Make<CMD::fac_building_remove>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int index) 
			{
				Make<single_data_header>::From(wrapper,FAC_BUILDING_REMOVE);
				return wrapper << index; 
			}
		};

		template <>
		struct Make<CMD::fac_base_prop_change>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const INFO::fac_base_prop & prop, int noti_roleid) 
			{
				Make<single_data_header>::From(wrapper,FAC_BASE_PROP_CHANGE);
				Make<fac_base_prop>::From(wrapper, prop);
				wrapper << noti_roleid;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_active_emote_invite>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,int active_emote_type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ACTIVE_EMOTE_INVITE);
				return wrapper << who.id << active_emote_type;
			}
		};

		template <>
		struct Make<CMD::player_active_emote_invite_reply>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,int param)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ACTIVE_EMOTE_INVITE_REPLY);
				return wrapper << who.id << param;
			}
		};

		template <>
		struct Make<CMD::player_active_emote_start>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who, const XID & who1, int type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ACTIVE_EMOTE_START);
				return wrapper << who.id << who1.id << type;
			}
		};

		template <>
		struct Make<CMD::player_active_emote_stop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who,const XID & who1, int type)
			{
				Make<single_data_header>::From(wrapper,PLAYER_ACTIVE_EMOTE_STOP);
				return wrapper << who.id << who1.id << type;
			}
		};

		template <>
		struct Make<INFO::fac_base_prop>
		{

			template <typename WRAPPER>
			static bool From(WRAPPER & wrapper, const INFO::fac_base_prop & prop)
			{
				wrapper << prop.grass << prop.mine << prop.monster_food
					<< prop.monster_core << prop.cash << prop.task_id << prop.task_count << prop.task_need;
				return true;
			}
		};

		template <>
		struct Make<CMD::player_fac_base_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const CMD::player_fac_base_info & info, const std::vector<INFO::player_fac_field> & field_info, const void * msg, int msg_len)
			{
				Make<single_data_header>::From(wrapper,PLAYER_FAC_BASE_INFO);
				Make<fac_base_prop>::From(wrapper, info.prop);
				wrapper << msg_len;
				wrapper.push_back(msg, msg_len);
				wrapper << (unsigned char)field_info.size();
				std::vector<INFO::player_fac_field>::const_iterator it, ite = field_info.end();
				for (it = field_info.begin(); it != ite; ++it)
				{
					wrapper << it->index << it->tid << it->level << it->status;
				}
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::facbase_msg_update>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const void * msg, int msg_len)
			{
				Make<single_data_header>::From(wrapper,FACBASE_MSG_UPDATE);
				wrapper << msg_len;
				wrapper.push_back(msg, msg_len);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::be_taunted2>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const XID & who, int time)
			{
				Make<single_data_header>::From(wrapper,BE_TAUNTED2);
				return wrapper << who.id << time;
			}
		};

		template <>
		struct Make<CMD::notify_bloodpool_status>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, bool on, int cur_hp, int max_hp) 
			{
				Make<single_data_header>::From(wrapper,NOTIFY_BLOODPOOL_STATUS);
				return wrapper << player_id << on << cur_hp << max_hp; 
			}
		};

		template <>
		struct Make<CMD::player_change_puppet_form>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id) 
			{
				Make<single_data_header>::From(wrapper,PLAYER_CHANGE_PUPPET_FORM);
				return wrapper << player_id; 
			}
		};

		template <>
		struct Make<CMD::step_raid_score_info>
		{    
                        template <typename WRAPPER>
                        inline static WRAPPER & From(WRAPPER & wrapper, char tower_index, char monster_index,char award_icon,int award_id,int score)
                        {    
                                Make<single_data_header>::From(wrapper,STEP_RAID_SCORE_INFO);
                                wrapper << tower_index << monster_index << award_icon << award_id << score;
                                return wrapper;
                        }    
		};   

		template <>
		struct Make<INFO::fac_base_auc_item>
		{
			template <typename WRAPPER>
			static bool From(WRAPPER & wrapper, const INFO::fac_base_auc_item & item)
			{
				wrapper << item.idx << item.itemid << item.winner
					<< item.cost << item.name_len;
				wrapper.push_back(item.playername, sizeof(item.playername));
				wrapper << item.status << item.end_time;
				return true;
			}
		};

		template <>
		struct Make<INFO::fac_base_auc_history>
		{
			template <typename WRAPPER>
			static bool From(WRAPPER & wrapper, const INFO::fac_base_auc_history & history)
			{
				wrapper << history.event_type << history.name_len;
				wrapper.push_back(history.playername, sizeof(history.playername));
				wrapper << history.cost << history.itemid;
				return true;
			}
		};

		template <>
		struct Make<CMD::player_facbase_auction>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int return_coupon, const std::vector<INFO::fac_base_auc_item> & list, const std::vector<INFO::fac_base_auc_history> & history_list)
			{
				Make<single_data_header>::From(wrapper, PLAYER_FACBASE_AUCTION);
				wrapper << return_coupon << list.size();
				std::vector<INFO::fac_base_auc_item>::const_iterator it, ite = list.end();
				for (it = list.begin(); it != ite; ++it)
					Make<fac_base_auc_item>::From(wrapper, *it);
				wrapper << history_list.size();
				std::vector<INFO::fac_base_auc_history>::const_iterator hit, hite = history_list.end();
				for (hit = history_list.begin(); hit != hite; ++hit)
					Make<fac_base_auc_history>::From(wrapper, *hit);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::facbase_auction_update>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int type, const INFO::fac_base_auc_item & item)
			{
				Make<single_data_header>::From(wrapper, FACBASE_AUCTION_UPDATE);
				wrapper << type;
				Make<fac_base_auc_item>::From(wrapper, item);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::fac_coupon_return>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int return_coupon)
			{
				Make<single_data_header>::From(wrapper, FAC_COUPON_RETURN);
				wrapper << return_coupon;
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::facbase_auction_add_history>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const INFO::fac_base_auc_history & entry)
			{
				Make<single_data_header>::From(wrapper, FACBASE_AUCTION_ADD_HISTORY);
				Make<fac_base_auc_history>::From(wrapper, entry);
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_start_travel_around>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, int travel_vehicle_id, float travel_speed, int travel_path_id)
			{
				Make<single_data_header>::From(wrapper, PLAYER_START_TRAVEL_AROUND);
				wrapper << player_id << travel_vehicle_id << travel_speed << travel_path_id; 
				return wrapper;
			}
		};

		template <>
		struct Make<CMD::player_stop_travel_around>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id) 
			{
				Make<single_data_header>::From(wrapper, PLAYER_STOP_TRAVEL_AROUND);
				return wrapper << player_id;
			}
		};

		template<>
		struct Make<CMD::player_get_summon_petprop>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int pet_index, const struct q_extend_prop & prop)
			{
				Make<single_data_header>::From(wrapper,PLAYER_GET_SUMMON_PETPROP);
				return wrapper <<  pet_index << prop;
			}
		};

		template<>
		struct Make<CMD::cs6v6_cheat_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, bool is_cheat, int cheat_counter)
			{
				Make<single_data_header>::From(wrapper,CS6V6_CHEAT_INFO);
				return wrapper <<  is_cheat << cheat_counter;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_raid_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int last_hide_and_seek_timestamp, int day_has_get_award)
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_RAID_INFO);
				return wrapper <<  last_hide_and_seek_timestamp << day_has_get_award;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_round_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char round, char round_status, int end_timestamp)
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_ROUND_INFO);
				return wrapper <<  round << round_status << end_timestamp;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_players_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, const hide_and_seek_players_info::seek_player_info* pInfo,size_t total_num) 
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_PLAYERS_INFO);
				int num = total_num;
				wrapper << num;
				hide_and_seek_players_info::seek_player_info* pTemp = (hide_and_seek_players_info::seek_player_info*)pInfo;
				for (size_t i = 0; i < total_num;++i)
				{
					wrapper << pTemp->playerid << pTemp->score << pTemp->status << pTemp->occupation;
					++pTemp;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_skill_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char skill_left_num, char skill_buy_num)
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_SKILL_INFO);
				return wrapper <<  skill_left_num << skill_buy_num;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_blood_num>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int seeker_id, int hider_id, char blood_num)
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_BLOOD_NUM);
				return wrapper << seeker_id << hider_id <<  blood_num;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_role>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int player_id, char role_type)
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_ROLE);
				return wrapper << player_id << role_type;
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_enter_raid>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_ENTER_RAID);
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_leave_raid>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper)
			{
				return Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_LEAVE_RAID);
			}
		};

		template<>
		struct Make<CMD::hide_and_seek_hider_taunted>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,int playerid, const A3DVECTOR pos, bool isstart)
			{
				Make<single_data_header>::From(wrapper,HIDE_AND_SEEK_HIDER_TAUNTED);
				return wrapper << playerid << pos << isstart;
			}
		};

		template<>
		struct Make<CMD::capture_raid_player_info>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, int atk_score, int def_score,char atk_flag_count,
					char def_flag_count,int max_atk_killer_id,int max_def_killer_id, short max_atk_kill_count,
				        short max_def_kill_count,const capture_raid_player_info::capture_score_info* pInfo, size_t total_num)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_RAID_PLAYER_INFO);
				wrapper << atk_score << def_score << atk_flag_count << def_flag_count << max_atk_killer_id << max_def_killer_id;
				wrapper << max_atk_kill_count << max_def_kill_count;
				int num = total_num;
				wrapper << num;
				capture_raid_player_info::capture_score_info* pTemp = (capture_raid_player_info::capture_score_info*)pInfo;
				for (size_t i = 0; i < total_num; ++i)
				{
					wrapper << pTemp->roleid << pTemp->kill_count << pTemp->death_count << pTemp->faction << pTemp->flag_count;
					++pTemp;
				}
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::capture_broadcast_score>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper,char faction, char score)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_BROADCAST_SCORE);
				return wrapper << faction << score;
			}
		};

		template<>
		struct Make<CMD::capture_submit_flag>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char faction, int player_id, const void* name, int size)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_SUBMIT_FLAG);
				wrapper << faction << player_id;
				wrapper << size;
				wrapper.push_back(name,size);
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::capture_reset_flag>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char faction)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_RESET_FLAG);
				return wrapper << faction;
			}
		};

		template<>
		struct Make<CMD::capture_gather_flag>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char faction, int player_id,const void * name, int size)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_GATHER_FLAG);
				wrapper << faction << player_id;
				wrapper << size;
				wrapper.push_back(name,size);
				return wrapper;
			}
		};

		template<>
		struct Make<CMD::capture_monster_flag>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char faction, int monster_id)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_MONSTER_FLAG);
				return wrapper << faction << monster_id;
			}
		};

		template<>
		struct Make<CMD::capture_broadcast_tip>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char tip)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_BROADCAST_TIP);
				return wrapper << tip;
			}
		};

		template<>
		struct Make<CMD::capture_broadcast_flag_move>
		{
			template <typename WRAPPER>
			inline static WRAPPER & From(WRAPPER & wrapper, char faction, const A3DVECTOR& pos)
			{
				Make<single_data_header>::From(wrapper,CAPTURE_BROADCAST_FLAG_MOVE);
				return wrapper << faction << pos;
			}
		};

	};
}

#endif
