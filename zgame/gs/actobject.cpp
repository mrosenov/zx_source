#include <stdlib.h>
#include <arandomgen.h>
#include "gmatrix.h"
#include "actobject.h"
#include "object.h"
#include "world.h"
#include "usermsg.h"
#include "actsession.h"
#include "property_ar.h"
#include "playertemplate.h"
#include "pathfinding/pathfinding.h"
#include "general_indexer.h"

int ai_actobject::QueryTarget(const XID & id, target_info & info)
{
	world::object_info base_info;
	bool rst = _imp->_plane->QueryObject(id,base_info);
	if(!rst) return TARGET_STATE_NOT_FOUND;
	info.pos 	= base_info.pos;
	info.cls 	= base_info.cls;
	info.gender 	= base_info.gender;
	info.faction 	= base_info.faction;
	info.level 	= base_info.level;
	info.hp 	= base_info.hp;
	info.mp		= base_info.mp;
	info.body_size 	= base_info.body_size;
	if(base_info.state & world::QUERY_OBJECT_STATE_ZOMBIE) return TARGET_STATE_ZOMBIE;
	return TARGET_STATE_NORMAL;
}

void 
ai_actobject::SendMessage(const XID & id, int message)
{
	MSG msg;
	gobject * pObj = _imp->_parent;
	BuildMessage(msg,message,id,pObj->ID,pObj->pos);
	gmatrix::SendMessage(msg);
}

gactive_imp::gactive_imp():_session_id(0),_spirit_session_id(0),_switch_dest(-1),_direction(-1,0,0),_session_state(STATE_SESSION_IDLE),_cur_session(NULL),_hp_gen_counter(0),_mp_gen_counter(0),_dp_gen_counter(0)
{
	memset(&_basic,0,sizeof(_basic));
	memset(&_base_prop,0,sizeof(_base_prop));
	memset(&_cur_prop,0,sizeof(_cur_prop));
	memset(&_cur_item,0,sizeof(_cur_item));
	memset(&_en_point,0,sizeof(_en_point));
	memset(&_en_percent,0,sizeof(_en_percent));

	_crit_rate = 0;
	_base_crit_rate = 0;
	_exp_addon = 0;
	_money_addon = 0;
	_immune_state = 0;
	_immune_state_adj = 0;

	_faction = 0;
	_enemy_faction = 0;
	_combat_state = false;
	_refresh_state = false;
	_cur_shape = 0;
	_silent_seal_flag = _diet_seal_flag = _melee_seal_flag = _root_seal_flag = 0;
	_at_attack_state = 0;
	_at_defense_state = 0;
	_sitdown_state = 0;
	_layer_ctrl.Ground();
	_lock_equipment = false;
	_lock_pet_bedge = false;
	_lock_pet_equip = false;
	_lock_inventory = false;
	_skill_mirror = false;
	_clone_mirror = false;
	_skill_reflect = false;
	_skill_mirror_rate = 0;
	_clone_mirror_rate = 0;
	_skill_reflect_rate = 0;
	_heal_effect = 0;
	_bind_to_ground = 0;
	_deny_all_session = 0;
	_attack_stamp = 0;
	_expire_item_date = 0;
	_last_attack_target = XID(-1,-1);
	_at_feedback_state = 0;
}

gactive_imp::~gactive_imp()
{		
	_plane = 0;
	//清除session队列
	ClearSession();
}

	void
gactive_imp::MH_query_info00(const MSG & msg)
{
	if(msg.source.type == GM_TYPE_PLAYER && msg.content_length == sizeof(int))
	{
		_runner->query_info00(msg.source,*(int*)(msg.content), msg.param);
	}
	else
	{
		ASSERT(false);
	}
}

	bool
gactive_imp::CurSessionValid(int id)
{
	return _cur_session && _cur_session->_session_id == id;
}

	int
gactive_imp::MessageHandler(const MSG & msg)
{
	//	__PRINTF("active object recv message (%d,%d)%d\n",_parent->ID.type,_parent->ID.id,msg.message);
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
			{
				ASSERT(msg.content_length >= sizeof(attack_msg));
				//这里由于都要进行复制操作，有一定的耗费存在
				attack_msg ack_msg = *(attack_msg*)msg.content;
				//处理一下到来的攻击消息
				_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
				HandleAttackMsg(msg,&ack_msg);
				return 0;
			}
		case GM_MSG_ENCHANT:
			{
				//__PRINTF("recv enchant\n");
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				//这里由于都要进行复制操作，有一定的耗费存在
				_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
				HandleEnchantMsg(msg,&ech_msg);
				return 0;
			}

		case GM_MSG_OBJ_ZOMBIE_SESSION_END:
			if(!_parent->IsZombie()) return 0;
		case GM_MSG_OBJ_SESSION_END:
			if(!CurSessionValid(msg.param))
			{
				break;
			}
				EndCurSession();
				StartSession();
			return 0;

		case GM_MSG_OBJ_SESSION_REPEAT:
			if(!CurSessionValid(msg.param))
			{
				break;
			}
			if(HasNextSession())
			{
				EndCurSession();
				StartSession();
			}
			else
			{
				int guid = _cur_session->GetGUID();
				int p = _session_state;
				if(!_cur_session->RepeatSession())		
				{
					if(_cur_session == NULL)
					{
						GLog::log(GLOG_ERR,"FATALERROR错误的session1状态 classid:%d session_state%d",guid,p);
					}
					EndCurSession();
					StartSession();
				}
			}
			return 0;
		case GM_MSG_OBJ_SESSION_REP_FORCE:
			if(!CurSessionValid(msg.param))
			{
				break;
			}
			//__PRINTF("-----session repeat force\n");
			{
				int guid = _cur_session->GetGUID();
				int p = _session_state;
				if(!_cur_session->RepeatSession())
				{
					if(_cur_session == NULL)
					{
						GLog::log(GLOG_ERR,"FATALERROR错误的session2状态 classid:%d session_state%d",guid,p);
					}

					EndCurSession();
					StartSession();
				}
			}
			return 0;
		case GM_MSG_HEARTBEAT:
			DoHeartbeat(msg.param);
			return 0;

		case GM_MSG_PICKUP_MONEY:
		{
			ASSERT(msg.content && msg.content_length == sizeof(int));
			int id = 0;
			if(msg.content_length) id = *(int*)msg.content;
			OnPickupMoney(msg.param,id);
		}
		break;

		case GM_MSG_PICKUP_ITEM:
			ASSERT(msg.content && msg.content_length);
			OnPickupItem(msg.pos,msg.content,msg.content_length,msg.param&0x80000000,msg.param&0x7FFFFFFF);
			break;
		case GM_MSG_QUERY_OBJ_INFO00:
			MH_query_info00(msg);
			break;
		case GM_MSG_SUBSCIBE_TARGET:
			ASSERT(msg.source.type == GM_TYPE_PLAYER);
			_buff.InsertSubscibe(this, msg.source,*(link_sid*)msg.content);
			SendTo<0>(GM_MSG_SUBSCIBE_CONFIRM,msg.source,0);
			break;

		case GM_MSG_UNSUBSCIBE_TARGET:
			ASSERT(msg.source.type == GM_TYPE_PLAYER);
			_buff.RemoveSubscibe(msg.source);
			break;

		case GM_MSG_EXTERN_HEAL:
			if(!_parent->IsZombie() && msg.param > 0)
			{
				IncHP(msg.param);
			}
			break;

		case GM_MSG_EXTERN_ADD_MANA:
			if(!_parent->IsZombie() && msg.param > 0)
			{
				IncMP(msg.param);
			}
			break;
			
		case GM_MSG_QUERY_INFO_1:
			{
				ASSERT(msg.content_length == sizeof(int));
				_runner->query_info_1(msg.source.id, msg.param, *(int*)(msg.content));
			}
			break;

		case GM_MSG_HURT:
			{
				if(!_parent->IsZombie() && msg.content_length == sizeof(attacker_info_t) )
				{
					int damage = msg.param;
					bool invader = msg.param2 & 0xFF00;
					char attacker_mode = msg.param2 & 0x00FF;
					const attacker_info_t  & info = *(const attacker_info_t*)msg.content;

					int newdamage = DoDamage(damage);
					if(newdamage > damage) damage = newdamage;
					if(damage > 0)
					{
						OnHurt(msg.source,info,damage,invader);
						if(_basic.hp <=0)
						{
							_basic.hp = 0;
							Die(info.attacker,invader,attacker_mode);
						}
					}
				}
			}
			break;

		case GM_MSG_DUEL_HURT:
			{
				if(!_parent->IsZombie()) 
				{
					int state = ((gactive_object*)_parent)->object_state;
					if(state & gactive_object::STATE_IN_DUEL_MODE)
					{
						DoDamage(msg.param);
						if(_basic.hp == 0)
						{
							Die(msg.source,false,attack_msg::PVP_DUEL);
						}
					}
				}
			}
			break;

		case GM_MSG_DEATH:
			{
				if(!_parent->IsZombie()) 
				{
					DoDamage(_basic.hp + 1000);
					if(_basic.hp == 0)
					{
						Die(msg.source,false,0);
					}
				}
			}
			break;

		case GM_MSG_DEC_HP:
			{
				ASSERT(msg.param > 0);
				DecHP(msg.param);
			}
			break;

		case GM_MSG_EXCHANGE_SUBSCIBE:
			{
				ASSERT(msg.content_length % sizeof(link_sid) == 0);
				bool is_init = msg.param;
				if(is_init) _buff.ExchangeSubscibeTo(this, msg.source, false, 0);
				_buff.ExchangeSubscibeFrom(this, msg.content, msg.content_length); 
			}
			break;

		case GM_MSG_TRANSFER_ATTACK:
			{
				ASSERT(msg.content_length == sizeof(msg_transfer_attack));
				const msg_transfer_attack * pMsg = (const msg_transfer_attack*)msg.content;

				HandleTransferAttack(msg.source, msg.pos, pMsg->force_attack, pMsg->dmg_plus, pMsg->radius, pMsg->total);
			}
			break;

		case GM_MSG_ATTACK_FEED_BACK:
			{
				if(msg.param & GNET::FEEDBACK_CRIT)
				{
					_filters.EF_CritEnemy(msg.source);
				}
				if(msg.param & GNET::FEEDBACK_DAMAGE)
				{
					_filters.EF_DamageEnemy(msg.source, msg.param2);
				}
				_at_feedback_state = msg.param;
			}
			break;

			
		default:
			__PRINTF("遇到未处理的消息%d to %p\n",msg.message,this);
			break;
	}
	return 0;
}

bool 
gactive_imp::AttackJudgement(attack_msg * attack,damage_entry &dmg)
{
	ASSERT(dmg.damage == 0);
	//首先是物理攻击判定
//	ASSERT(attack->damage_low >=0 && attack->damage_high >= attack->damage_low);
	if(attack->normal_attack)	//物理攻击才进行攻击判定的判断
	{
		float attack_pb = netgame::GetAttackProbability(attack->attack_rate, _cur_prop);
		if(abase::RandUniform() > attack_pb)
		{
			//未命中
			return false;
		}
	}
	else
	{
		int rate = 1000;
		rate += attack->skill_attack_rate;
		rate -= _cur_prop.skill_armor_rate;
		if(abase::Rand(0,1000) > rate)
		{
			return false;
		}
	}

	int damage = abase::Rand(attack->damage_low, attack->damage_high) - _cur_prop.defense;
	if(damage <0) damage = 0;
	dmg.damage = damage;
	dmg.damage_no_crit = attack->damage_no_crit - _cur_prop.defense;
	if (dmg.damage_no_crit < 0) dmg.damage_no_crit = 0; 
	return true;
}

void 
gactive_imp::Die(const XID & attacker, bool is_pariah, char attacker_mode)
{
	//filter调用死亡前的处理
	_filters.EF_BeforeDeath(attacker_mode & attack_msg::PVP_DUEL);

	//清除必要的标志
	_silent_seal_flag = 0;
	_diet_seal_flag = 0;
	_melee_seal_flag = 0;
	_root_seal_flag = 0;

	//去除死亡时应该去掉的filter
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH);

	//死亡，调用死亡函数，进行处理
	OnDeath(attacker, is_pariah);

}

int 
gactive_imp::HandleEnchantMsg(const MSG & msg, enchant_msg * enchant)
{
	//检查限制条件
	if(enchant->skill_limit != 0 && !CheckRecvSkillAttack(enchant->skill_limit, msg.source.id)) return 0;

	if(!enchant->helpful && (!enchant->target_faction || enchant->attack_state & gactive_imp::AT_STATE_IMMUNE))
	{
	//$$$$$$$$$$$$$$$$$$
		//无目标攻击，被前面的攻击取消掉了
		//但是如果是因为免疫而取消，那么发送免疫数据
	//	if(enchant->attack_state & gactive_imp::AT_STATE_IMMUNE)
	//	{
			_runner->enchant_result(msg.source,enchant->skill,enchant->skill_level,false,enchant->attack_state,enchant->attack_stamp,0);
	//	}
		return 0;
	}
	bool fac_match = (GetFaction() & enchant->target_faction);
	bool orange_name;// = enchant->is_invader && !fac_match;
	if(!enchant->helpful)
	{
		if(!fac_match && !enchant->force_attack)
		{
			//可能需要返回错误，让对方停止攻击
			return 0;
		}
		

		//是否非法的攻击 
		orange_name = enchant->is_invader && !fac_match;
		if(orange_name && _pk_level <= 0 )
		{
		//发送粉名消息
			SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,enchant->ainfo.attacker,60);
		}
		
		if(OnEnchantMirror(msg,enchant)) return 0;
	}
	else
	{
		if(fac_match) return 0;

		//增益操作是否非法完全由外面判断
		orange_name = enchant->is_invader;
		if(orange_name)
		{
			//判断是否发送粉名消息
			//死亡会自动发出红名消息
			SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,enchant->ainfo.attacker,60);
		}
	}
	OnEnchant(msg, enchant);

	_skill.Attack(this,msg.source,msg.pos,*enchant,orange_name);
	return 0;
}


int 
gactive_imp::HandleAttackMsg(const MSG & msg, attack_msg * attack)
{
	//检查限制条件
	if(attack->skill_limit != 0 && !CheckRecvSkillAttack(attack->skill_limit, msg.source.id)) return 0;
	//现在攻击一旦发出则必定命中
//	__PRINTF("收到攻击消息， 从 %d 到 %d ， skillid:%d\n",msg.source.id, _parent->ID.id, attack->skill_id);

	//根据阵营判定攻击
	int fac_match = (GetFaction() & attack->target_faction);
	if(attack->target_faction == 0 || (attack->attack_state & AT_STATE_IMMUNE) 
			|| (!fac_match && !attack->force_attack))
	{
		//非强制攻击的不匹配攻击，自动忽略（不再进行攻击）
		//发送无效效果
	//	_runner->be_damaged(msg.source, attack->skill_id, attack->ainfo, 0,-1,
	//			attack->attack_state| AT_STATE_EVADE,attack->attack_stamp,0);
		return 0;
	}

	bool orange_name = attack->is_invader && !fac_match;

	damage_entry dmg(0,attack->damage_factor);
	bool is_hit = AttackJudgement(attack,dmg);
	//这里的attack_state可能会变化
	OnAttacked(msg,attack,dmg,is_hit);
	if(!is_hit)
	{
		//未命中，返回信息
		_runner->dodge_attack(msg.source, attack->skill_id,attack->ainfo
				,attack->attack_state,attack->attack_stamp,orange_name);
		return 0;
	}
	else
	{
		//连续技
		int combo_type = 0;
		int combo_color = 0;
		int combo_color_num = 0;
		if(attack->skill_id > 0)
		{
			combo_type = _skill.GetSerialSkillType(attack->skill_id);
			_skill.GetSucceedSkillColor(attack->skill_id, combo_color, combo_color_num);
		}
		if(combo_type == GNET::SERIAL_SKILL_START || combo_type == GNET::SERIAL_SKILL_SUCCEED) 
		{
			msg_combo data;
			data.combo_type = combo_type;
			data.combo_color = combo_color;
			data.combo_color_num = combo_color_num;
			SendTo<0>(GM_MSG_USE_COMBO_SKILL,msg.source,0, &data, sizeof(data));
		}	

		//技能反弹
		if(OnAttackMirror(msg,attack))
		{
			if(orange_name && _pk_level <= 0 )
			{
				//发送粉名消息
				SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,attack->ainfo.attacker,60);	//这个时间目前不能超过127
			}
			return 0;
		}

	}

	char at_state = attack->attack_state;

	//先进行伤害的调整
	AdjustDamage(msg,attack,dmg);


	int feedback_mask = 0;
	
	//进行重击的判定
	int is_crit = 0;
	if(attack->ainfo.attacker.IsPlayerClass() && attack->ainfo.invisible_rate > ((gactive_object*)_parent)->anti_invisible_rate)
	{
		attack->crit_rate += 500;  //被隐身看不见的人攻击则必然被暴击
	}
	if(abase::Rand(0,999) < attack->crit_rate - _cur_prop.anti_crit)
	{
		float factor1 = attack->crit_factor - _cur_prop.anti_crit_damage;
		if(factor1 < 1.0) factor1 = 1.0f;
		dmg.adjust *= factor1;
		at_state |= AT_STATE_ATTACK_CRIT;
		is_crit = 1;
		
		//发送给施法者产生暴击
		feedback_mask |= GNET::FEEDBACK_CRIT;
	}

	// Youshuang add
	if( attack->fashion_weapon_addon_id > 0)
	{
		const addon_data_spec * pSpec = addon_data_man::Instance().GetAddon( attack->fashion_weapon_addon_id );
		if( pSpec )
		{
			int fashion_weapon_skill_id = pSpec->data.arg[0];
			int fashion_weapon_skill_level = pSpec->data.arg[1];
			int fashion_weapon_rate = pSpec->data.arg[2];
			//printf( "the addon info id: %d, level: %d, rate: %d\n", fashion_weapon_skill_id, fashion_weapon_skill_level, fashion_weapon_rate );
			if( abase::Rand(0,999) < fashion_weapon_rate )
			{
				SKILL::Data data( fashion_weapon_skill_id );
				_skill.CastExtraSkillToSelf( data, this, fashion_weapon_skill_level, attack );
			}
		}
	}
	//

	// filter 处理一下damage
	_at_defense_state = 0;
	_filters.EF_AdjustDamage(dmg,msg.source,*attack);

	if(dmg.ignore_attack)
	{
		at_state |= AT_STATE_IMMUNE;
		_runner->dodge_attack(msg.source, attack->skill_id,attack->ainfo
				,at_state,attack->attack_stamp,orange_name);
		return 0;
	}
	float damage = dmg.damage * dmg.adjust + attack->spec_damage + dmg.damage_no_crit;

	//进行减少伤害的判定
	if(_en_percent.dmg_reduce)
	{
		int dmg_reduce = _en_percent.dmg_reduce;
		dmg_reduce -= attack->ign_dmg_reduce;
		if(dmg_reduce > 0) 
		{
			damage *= (1000 - dmg_reduce) * 0.001f;
		}
	}

	//进行人物减少增加伤害的判定
	if(msg.source.IsPlayerClass() && _parent->ID.IsPlayerClass())
	{
		int dmg_change = 0;
		dmg_change += attack->dmg_change;
		if(dmg_change > 1000) dmg_change = 1000;
		else if(dmg_change < -800) dmg_change = -800;

		damage *= (1000 + dmg_change) * 0.001f;	
	}


	if(attack->is_pet)
	{
		int pet_attack_adjust = attack->pet_attack_adjust - _en_point.pet_attack_defense;
		if(pet_attack_adjust < -1000) pet_attack_adjust = - 1000;
		if(pet_attack_adjust)
		{
			damage *= (1000  + pet_attack_adjust) * 0.001f;
		}
	}
	else
	{
		//进行造化的攻击修正
		int cult_reduce = 0;
		if(attack->cultivation == 0)
		{
			//攻击者为普通玩家
			for(size_t i = 0; i < CULT_DEF_NUMBER; i ++)
			{
				if(cult_reduce < _cur_prop.cult_defense[i]) cult_reduce = _cur_prop.cult_defense[i];
			}
			if(cult_reduce > 1000) cult_reduce = 1000;
		}
		else
		{
			//攻击者为非普通玩家
			cult_reduce = 1000;
			int a_cult = attack->cultivation;
			for(size_t i =0; i < CULT_DEF_NUMBER; i ++)
			{
				if( (1 << i) & a_cult)
				{
					//攻击者匹配造化，按照最小值来进行匹配
					if(cult_reduce > _cur_prop.cult_defense[i]) cult_reduce = _cur_prop.cult_defense[i];
				}
			}
		}

		int cultivation = OI_GetCultivation();
		if(cultivation != 0)
		{
			if(cultivation & 0x01) cult_reduce -= attack->cult_attack[0]; 
			if(cultivation & 0x02) cult_reduce -= attack->cult_attack[1]; 
			if(cultivation & 0x04) cult_reduce -= attack->cult_attack[2]; 
		}
		
		if(cult_reduce)
		{
			damage *= (1000 - cult_reduce) * 0.001f;
		}
	}
	

	//进行固定减少伤害的判定
	if(_en_point.dmg_reduce)
	{
		damage -= _en_point.dmg_reduce;
		if(damage <= 0.5f) damage = 1;
	}
	
	at_state |= _at_defense_state;
	int int_damage = (int)(damage + 0.5f) + attack->fixed_damage;
	if(int_damage == 0) int_damage = 1;
	int newdamage = 0;

	//神圣力伤害计算
	float dt_power1 = (float)(attack->deity_power);
	float dt_power2 = (float)GetDeityPower(); 
	int dt_damage = 0;

	if(msg.source.IsPlayerClass() && (_parent->ID.IsPlayerClass() || GetObjectType() == TYPE_PET || GetObjectType() == TYPE_SUMMON))
	{
		if(dt_power1 > 0 && dt_power2 >=0)
		{
			if(dt_power1 > MAX_DEITY_POWER) dt_power1 = MAX_DEITY_POWER;
			if(dt_power2 > MAX_DEITY_POWER) dt_power2 = MAX_DEITY_POWER;

			float dt_power = dt_power1 + dt_power2;
			if(dt_power1 >= dt_power2)
			{
				dt_damage = (int)(abase::Rand(0.0f,1.0f)*0.2*dt_power1+1+(dt_power)*(944*sqrt(dt_power1)-514*sqrt(dt_power2)+23597*(dt_power1-dt_power2)/(dt_power + 466))/(dt_power + 200));
			}
			else
			{
				float damage = (dt_power)*(944*sqrt(dt_power2)-514*sqrt(dt_power1)+23597*(dt_power2-dt_power1)/(dt_power + 466))/(dt_power + 200);
				dt_damage = (int)(abase::Rand(0.0f,1.0f)*0.2*dt_power2+1+ (dt_power1*dt_power1*dt_power2*430.0*430.0)/((dt_power1+100)*(dt_power1+100)*damage));
			}

			int dt_crit = (int)(dt_power1*5/100 + 150);
			if(abase::Rand(0,999) < dt_crit)
			{
				at_state |= AT_STATE_DT_ATTACK_CRIT;
				dt_damage *= 2;
			}

			if(dt_damage <=0) dt_damage = 0;
		}
	}

	if(_immune_state_adj & IMMUNEDAMAGE)
	{
		at_state |= AT_STATE_IMMUNE_DAMAGE;
		_runner->dodge_attack(msg.source, attack->skill_id,attack->ainfo, at_state,attack->attack_stamp,orange_name);
	}
	else
	{
		int damage1 = int_damage;	//保存起来为了显示
		int damage2 = dt_damage;	//保存起来为了显示
			
		if(int_damage > 0) newdamage = DoDamage(int_damage );
		if(dt_damage > 0)  DoDamage(dt_damage );
		if(newdamage > int_damage) int_damage = newdamage;

		_filters.EF_BeAttacked(msg.source, newdamage, is_crit == 1);

		//if(newdamage > 0 && (attack->feedback_mask & GNET::FEEDBACK_DAMAGE))
		if(newdamage > 0) 
		{
			feedback_mask |= GNET::FEEDBACK_DAMAGE;
		}

		SendAttackFeedBack(attack->ainfo.attacker, feedback_mask, newdamage);
		OnDamage(msg.source,attack->skill_id, attack->ainfo,damage1,damage2,at_state,attack->attack_stamp,orange_name);
	}


	if(_basic.hp != 0)
	{
		//调用技能的处理接口
		//先调用技能后调用OnDamage，这样可以让NPC的行为判定在技能效果之后
		if(attack->attached_skill.skill)
		{
			attack->attack_state = 0;	//技能可能会添加state
			_skill.Attack(this,msg.source,msg.pos,*attack,orange_name,int_damage, is_crit);
			at_state |= attack->attack_state;
		}
	}

	if(orange_name && _pk_level <= 0 )
	{
		//发送粉名消息
		SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,attack->ainfo.attacker,60);	//这个时间目前不能超过127
	}

	if(_basic.hp == 0)
	{
		//只有变橙名的攻击才会让对方变成红名
		//只有杀死白名的自己才会让对方变成红名
		Die(attack->ainfo.attacker,orange_name,attack->attacker_mode);
	}
	return 0;
}


static inline int normalrand(int low,int high)
{
	if(low == high) return low;
	return abase::RandNormal(low,high);
}

int 
gactive_imp::MakeAttackMsg(attack_msg & attack,char force_attack)
{
	memset(&attack,0,sizeof(attack));
	attack.ainfo.level = _basic.level;
	attack.ainfo.team_id = -1; 
	attack.normal_attack  = true;
	GenerateAttackDamage(attack.damage_low, attack.damage_high);
	attack.attack_rate = GetAttackRate();
	attack.attacker_faction = GetFaction();
	attack.target_faction = GetEnemyFaction();
	attack.force_attack  = force_attack;

	return 0;
}

int 
gactive_imp::DoAttack(const XID & target,char force_attack)
{
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack);

	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,0,&attack,sizeof(attack));
	//try attack
	TranslateAttack(target,attack, 0);
	gmatrix::SendMessage(msg);
	__PRINTF("send attack message,target(%d,%d)\n",target.type,target.id);
	return 0;
}

void gactive_imp::AddStartSession(act_session * ses)
{
	if(AddSession(ses)) StartSession();
}

bool gactive_imp::StartSession()
{
	//	ASSERT(_cur_session == NULL);
	if(_cur_session != NULL || _session_state != STATE_SESSION_IDLE) return false;
	bool rst = false;
	while(_cur_session == NULL && HasNextSession())  
	{
		_cur_session = _session_list[0];
		_session_list.erase(_session_list.begin());
		if(!(rst = _cur_session->StartSession(HasNextSession()?_session_list[0]:NULL)))
		{
			EndCurSession();
		}
		else
		{
			_cur_session->_start_tick = g_timer.get_tick();
		}
	}
	return rst;
}

bool gactive_imp::EndCurSession()
{
	//这里不校验_session_state的目的是有些session不会改state
	if(_cur_session == NULL) return false;
	_cur_session->EndSession();
	_session_state = STATE_SESSION_IDLE;
	delete _cur_session;
	_cur_session = NULL;
	return true;
}

void
gactive_imp::TerminateCurSession()
{
	if(_cur_session && (_cur_session->TerminateSession(false) || _cur_session->TerminateSession(true)) )
	{
		_session_state = STATE_SESSION_IDLE;
		delete _cur_session;
		_cur_session = NULL;
	}
	
}


void gactive_imp::ClearNextSession()
{
	abase::clear_ptr_vector(_session_list);
}

void gactive_imp::ClearSession()
{
	if(_cur_session)
	{
		_cur_session->TerminateSession();
		_session_state = STATE_SESSION_IDLE;
		delete _cur_session;
		_cur_session = NULL;
	}

	if(_session_list.size())
	{
		abase::clear_ptr_vector(_session_list);
	}
}
void 
gactive_imp::ResetSession()
{
	if(_cur_session)
	{
		//这里不应该调用TerminateSession了，因为会引发额外的消息发送
		//_cur_session->TerminateSession();
		_session_state = STATE_SESSION_IDLE;
		delete _cur_session;
		_cur_session = NULL;
	}

	if(_session_list.size())
	{
		abase::clear_ptr_vector(_session_list);
	}
}

void 
gactive_imp::TryStopCurSession()
{
	if(_cur_session && _cur_session->TerminateSession(false))
	{
		_session_state = STATE_SESSION_IDLE;
		delete _cur_session;
		_cur_session = NULL;
		StartSession();
	}
}

void gactive_imp::ClearSpecSession(int exclusive_mask)
{
	act_session ** it = _session_list.end();
	for(;it != _session_list.begin();)
	{
		--it;
		if((*(it))->GetMask() & exclusive_mask)
		{
			delete *it;
			_session_list.erase(it);
		}
	}
	ASSERT(it == _session_list.begin());
}

bool gactive_imp::AddSession(act_session * ses)
{
	if(_deny_all_session) 
	{
		delete ses;
		return false;
	}

	int exclusive_mask = ses->GetExclusiveMask();
	if(_session_list.size() >= MAX_PLAYER_SESSION)	//超出了最大队列上限
	{
		__PRINTF("session队列满，清空原有队列内容\n");
		abase::clear_ptr_vector(_session_list);

		if(_cur_session)
		{
			//如果当前session存在,检查当前session的执行时间,如果过长,则清除之

			if((size_t)(g_timer.get_tick() - _cur_session->_start_tick) > SESSION_TIME_OUT * TICK_PER_SECOND )
			{
				GLog::log(GLOG_ERR,"%d Session执行时间过长而超时 session classid%d", 
						_parent->ID.id, _cur_session->GetGUID());
				_cur_session->DoTimeoutLog();
				TerminateCurSession();
			}
		}
	}

	if(exclusive_mask) 
	{
		act_session ** it = _session_list.end();
		for(;it != _session_list.begin();)
		{
			--it;
			act_session * as = *it;
			if((as->GetMask() & exclusive_mask) || as->Mutable(NULL))
			{
				__PRINTF("删除了被排斥的session %x\n",(*(it))->GetMask());
				delete *it;
				_session_list.erase(it);
			}
		}
		ASSERT(it == _session_list.begin());
	}

	if(_session_list.empty() && _cur_session)
	{
		if(_cur_session->Mutable(ses))
		{
			EndCurSession();
		}
	}
	_session_list.push_back(ses);
//	__PRINTF("object addSession ..............%s %d %d\n", ses->GetRunTimeClass()->GetName(), _session_list.size(),_cur_session?1:0);
	return !_cur_session;
}

bool 
gactive_imp::CheckAttack(const XID & target,bool report_err)
{
	enum
	{
		ALIVE = world::QUERY_OBJECT_STATE_ACTIVE | world::QUERY_OBJECT_STATE_ZOMBIE
	};
	if(!CanAttack(target))
	{	
		if(report_err) _commander->error_cmd(S2C::ERR_CANNOT_ATTACK);
		return false;
	}

	world::object_info info;
	if(!_plane->QueryObject(target,info) ||
			(info.state & ALIVE) != world::QUERY_OBJECT_STATE_ACTIVE ||  
			((gactive_object*)_parent)->anti_invisible_rate < info.invisible_rate)   
	{
		//无法攻击到敌人，无效的目标
		if(report_err) _commander->error_cmd(S2C::ERR_INVALID_TARGET);
		return false;
	}
	float squared_range = GetAttackRange() + info.body_size;
	if(info.pos.squared_distance(_parent->pos) > squared_range * squared_range)
	{
		//无法攻击到敌人，距离过远
		if(report_err)	_commander->error_cmd(S2C::ERR_OUT_OF_RANGE);
		return false;
	}
	return true;
}

bool 
gactive_imp::CheckAttack(const XID & target,int * flag,float * pDis, A3DVECTOR & pos)
{
	enum
	{
		ALIVE = world::QUERY_OBJECT_STATE_ACTIVE | world::QUERY_OBJECT_STATE_ZOMBIE,

		CANNOT_ATTACK 	= 1,
		TARGET_INVALID 	= 2,
		OUT_OF_RANGE 	= 4,
	};
	*flag = 0;
	if(!CanAttack(target))
	{	
		*flag |= CANNOT_ATTACK;
		return false;
	}

	world::object_info info;
	if(!_plane->QueryObject(target,info) ||
			(info.state & ALIVE) != world::QUERY_OBJECT_STATE_ACTIVE ||
		        ((gactive_object*)_parent)->anti_invisible_rate < info.invisible_rate)	
	{
		//无法攻击到敌人，无效的目标
		*flag |= TARGET_INVALID;
		return false;
	}
	pos = info.pos;
	float squared_range = GetAttackRange() + info.body_size;
	float distance = info.pos.squared_distance(_parent->pos);
	if(distance > squared_range * squared_range)
	{
		//无法攻击到敌人，距离过远
		*flag |= OUT_OF_RANGE;
		return false;
	}
	if(pDis) *pDis = distance;
	return true;
}

bool
gactive_imp::CheckSendSkillAttack(int skill_id)
{
	if(GNET::SkillWrapper::IsBattleLimitSkill(skill_id) && !GetWorldManager()->IsBattleWorld()) return false;
	if(GNET::SkillWrapper::IsCombatLimitSkill(skill_id) && IsCombatState()) return false;
	if(GNET::SkillWrapper::IsNonCombatLimitSkill(skill_id) && !IsCombatState()) return false;
	if(GNET::SkillWrapper::IsNoSummonPetLimitSkill(skill_id) && !OI_IsCloneExist()) return false;
	if(GNET::SkillWrapper::IsMountLimitSkill(skill_id) && OI_IsMount()) return false;
	if(GNET::SkillWrapper::IsFlyLimitSkill(skill_id) && OI_IsFlying()) return false; 

	int limitSkillSummonID = GNET::SkillWrapper::LimitSkillSummonID(skill_id);
	XID id;
	if(GNET::SkillWrapper::IsNoSummonLimitSkill(skill_id) && limitSkillSummonID > 0 && !OI_SummonExist(limitSkillSummonID, id)) return false; 
	
	if(GetWorldManager()->IsLimitSkillInWorld(skill_id)) return false;
	if(!GetWorldManager()->CanCastSkillInWorld(skill_id, _parent->ID.id)) return false;

	return true;
}

bool
gactive_imp::CheckRecvSkillAttack(unsigned int skill_limit, int id)
{
	if((skill_limit & SKILL_LIMIT_MONSTER) &&  GetObjectType() == TYPE_NPC) return false;
	if((skill_limit & SKILL_LIMIT_PET) &&  GetObjectType() == TYPE_PET) return false;
	if((skill_limit & SKILL_LIMIT_SUMMON) &&  GetObjectType() == TYPE_SUMMON) return false;
	if((skill_limit & SKILL_LIMIT_PLAYER) &&  GetObjectType() == TYPE_PLAYER) return false;
	if((skill_limit & SKILL_LIMIT_SELF) &&  id == _parent->ID.id) return false;
	if((skill_limit & SKILL_LIMIT_BOSS) && IsMonsterBoss()) return false;
	if((skill_limit & SKILL_LIMIT_INVISIBLE_NPC) && !IsInvisibleNPC()) return false;

	return true;
}




void 
gactive_imp::DoHeartbeat(size_t tick)
{
	
	//检查一下session的状态是否正确
	ASSERT(_cur_session || (!_cur_session && ! _session_list.size()) );
	
	OnHeartbeat(tick);
	UpdateDataToParent();
	_commander->OnHeartbeat(tick);
	_buff.Update(this);
//	_backup_hp[0] = _backup_hp[1];
//	_backup_hp[1] = _basic.hp;


	//清空变化的标志
	ClearRefreshState();
}


void 
gactive_imp::SendAttackMsg(const XID & target, attack_msg & attack)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack, 0);
	gmatrix::SendMessage(msg);
			
}

void 
gactive_imp::SendEnchantMsg(int message,const XID & target, enchant_msg & enchant)
{
	MSG msg;
	BuildMessage(msg,message,target,_parent->ID,_parent->pos,
			0,&enchant,sizeof(enchant));
	gmatrix::SendMessage(msg);
}

void gactive_imp::ReInit()
{
	if(_cur_session)
	{
		ASSERT(_runner && _commander);
		//这种情况仅发生在服务器转移时
		_cur_session->Restore(this,GetNextSessionID());
		for(size_t i =0; i < _session_list.size(); i ++)
		{
			_session_list[i]->Restore(this,-1);
		}
	}
	else
	{
		ASSERT(_session_list.size() == 0);
	}
}

void 
gactive_imp::Init(world * pPlane,gobject*parent)
{
	gobject_imp::Init(pPlane,parent);
	_filters.Init(this);
}

void 
gactive_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	attack.ainfo.attacker = _parent->ID;
	attack.attack_state = _at_attack_state;
	attack.ainfo.level = _basic.level;
	attack.attacker_faction = GetFaction();
	attack.target_faction = GetEnemyFaction();
	attack.ainfo.team_id = attack.ainfo.team_seq = attack.ainfo.sid = 0;
	attack.damage_factor = 1.0f;
	attack.crit_rate = GetCritRate();
	attack.crit_factor = GetCritDamageFactor();
	attack.spec_damage += netgame::GetSpecDamage(_en_point);
	attack.attack_stamp = GetAttackStamp();
	attack.skill_attack_rate = _cur_prop.skill_attack_rate;
	attack.ign_dmg_reduce = GetIgnDmgReduce();
	attack.dmg_change = GetDmgChange();
	memcpy(attack.resistance_proficiency, _cur_prop.resistance_proficiency, sizeof(attack.resistance_proficiency));

	for(size_t i = 0; i < 3; ++i)
		attack.cult_attack[i] = _cur_prop.cult_attack[i];
	
	_at_attack_state = 0;

	if(IsBloodThirstyState())
	{
		attack.feedback_mask |= GNET::FEEDBACK_DAMAGE;
	}
}

void 
gactive_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	enchant.ainfo.attacker = _parent->ID;
	enchant.ainfo.level = _basic.level;
	enchant.attacker_faction = GetFaction();
	enchant.target_faction = GetEnemyFaction();
	enchant.ainfo.team_id = enchant.ainfo.team_seq = enchant.ainfo.sid = 0;
	enchant.attack_stamp = GetAttackStamp();
	memcpy(enchant.resistance_proficiency, _cur_prop.resistance_proficiency, sizeof(enchant.resistance_proficiency));
}


void gactive_imp::Swap(gactive_imp * rhs)
{
#define Set(var,cls) var = cls->var
	//置换gactive_imp的数据
	Set(_basic,rhs);
	Set(_base_prop,rhs); 
	Set(_cur_prop,rhs); 
	Set(_cur_item,rhs); 
	Set(_en_point,rhs); 
	Set(_en_percent,rhs);
	Set(_faction,rhs); 
	Set(_enemy_faction,rhs);
	Set(_crit_rate,rhs); 
	Set(_base_crit_rate,rhs); 
	Set(_exp_addon,rhs);
	Set(_money_addon,rhs);
	Set(_immune_state,rhs);
	Set(_immune_state_adj,rhs);
	Set(_base_crit_rate,rhs);
	Set(_layer_ctrl,rhs);  
	Set(_combat_state,rhs); 
	Set(_refresh_state,rhs);
	Set(_lock_equipment,rhs);
	Set(_lock_pet_bedge,rhs);
	Set(_lock_pet_equip,rhs);
	Set(_lock_inventory,rhs);
	Set(_skill_mirror,rhs);
	Set(_clone_mirror,rhs);
	Set(_skill_reflect,rhs);
	Set(_skill_mirror_rate,rhs);
	Set(_clone_mirror_rate,rhs);
	Set(_skill_reflect_rate,rhs);
	Set(_heal_effect,rhs);
	Set(_bind_to_ground,rhs);
	Set(_deny_all_session,rhs);
	Set(_session_state,rhs); 
	Set(_session_id,rhs); 
	Set(_spirit_session_id,rhs); 
	Set(_silent_seal_flag,rhs);
	Set(_diet_seal_flag,rhs);
	Set(_melee_seal_flag,rhs);
	Set(_root_seal_flag,rhs);
	Set(_sitdown_state,rhs);
	Set(_pk_level,rhs);
	Set(_attack_stamp,rhs);
	Set(_expire_item_date,rhs);
	Set(_pk_value,rhs);
	Set(_hp_gen_counter,rhs);
	Set(_mp_gen_counter,rhs);
	Set(_dp_gen_counter,rhs);
	Set(_last_attack_target,rhs);
	Set(_at_feedback_state,rhs);
	_skill.Swap(rhs->_skill);
	_set_addon_map.swap(rhs->_set_addon_map);

	_cur_session = rhs->_cur_session;
	rhs->_cur_session = NULL;
	_session_list.swap(rhs->_session_list);
	_filters.Swap(rhs->_filters,this);

	_buff.Swap(rhs->_buff);
	Set(_cur_shape,rhs); 

#undef Set
}

void 
gactive_imp::OnDuelStart(const XID & target)
{
	((gactive_object*)_parent)->object_state |= gactive_object::STATE_IN_DUEL_MODE;
}

void 
gactive_imp::OnDuelStop()
{
	((gactive_object*)_parent)->object_state &= ~gactive_object::STATE_IN_DUEL_MODE;
}

void 
gactive_imp::SetSilentSealMode(bool seal)
{
	_silent_seal_flag += seal?1:-1;
	OnSetSealMode();
	if(_silent_seal_flag)
	{
		ClearSpecSession(act_session::SS_MASK_SKILL | act_session::SS_MASK_SITDOWN);
		if(_cur_session)
		{
			AddSession(new session_empty());
			StartSession();
		}
	}
}

void 
gactive_imp::SetDietSealMode(bool seal)
{
	_diet_seal_flag += seal?1:-1;
	OnSetSealMode();
	if(_diet_seal_flag)
	{
		ClearSpecSession(act_session::SS_MASK_USE_ITEM | act_session::SS_MASK_SITDOWN);
		if(_cur_session)
		{
			AddSession(new session_empty());
			StartSession();
		}
	}
}

void 
gactive_imp::SetMeleeSealMode(bool seal)
{
	_melee_seal_flag += seal?1:-1;
	OnSetSealMode();
	if(_melee_seal_flag)
	{
		ClearSpecSession(act_session::SS_MASK_ATTACK | act_session::SS_MASK_SITDOWN);
		if(_cur_session)
		{
			AddSession(new session_empty());
			StartSession();
		}
	}
}

void 
gactive_imp::SetRootMode(bool seal)
{
	_root_seal_flag += seal?1:-1;
	if(_root_seal_flag)
	{
		ClearSpecSession(act_session::SS_MASK_MOVE | act_session::SS_MASK_SITDOWN);
		if(_cur_session)
		{
			AddSession(new session_empty());
			StartSession();
		}
	}
	OnSetSealMode();
}

int gactive_imp::GetPKValue() const
{
	//不能这样完成
	if(_pk_value == 0) return 0;
	if(_pk_value < 0)
	{
		return _pk_value / PKVALUE_PER_POINT + ((_pk_value % PKVALUE_PER_POINT)?-1:0);
	}
	else
	{
		return _pk_value / PKVALUE_PER_POINT + ((_pk_value % PKVALUE_PER_POINT)?1:0);
	}
}

void 
gactive_imp::Say(const char * msg)
{
	if(_parent->pPiece)
	{
		SaySomething(_plane,_parent->pPiece,msg,GMSV::CHAT_CHANNEL_LOCAL,_parent->ID.id);
	}
}

void 
gactive_imp::OI_Die()
{
	SendTo<0>(GM_MSG_DEATH,_parent->ID,0);
}

void 
gactive_imp::OI_Disappear()
{
	SendTo<0>(GM_MSG_NPC_DISAPPEAR,_parent->ID,0);
}

void 
gactive_imp::UpdateBuff(short buff_id, short buff_level, int end_time, char overlay_cnt)
{
	_buff.UpdateBuff(buff_id, buff_level, end_time?end_time + g_timer.get_systime():0, overlay_cnt);
}

void 
gactive_imp::RemoveBuff(short buff_id, short buff_level)
{
	_buff.RemoveBuff(buff_id, buff_level);
}

long
gactive_imp::OI_GetGlobalValue(long lKey)
{
	int world_tag = _parent->tag;
	int world_index = _parent->w_serial;

	GeneralIndexer indexer;
	indexer.SetParam(world_tag, world_index,0);

	int value = 0;
	indexer.Get(lKey, value); 
	return value;
	
}

void
gactive_imp::OI_PutGlobalValue(long lKey, long lValue)
{
	int world_tag = _parent->tag;
	int world_index = _parent->w_serial;

	GeneralIndexer indexer;
	indexer.SetParam(world_tag, world_index,0);

	if (!indexer.Set(lKey, lValue)) 
	{
		__PRINTF("设置全局数据失败\n");
	}
	else
	{
		GLog::log(GLOG_INFO,"全局变量操作[put]: key = %ld, value = %ld", lKey, lValue);
	}
}

void
gactive_imp::OI_ModifyGlobalValue(long lKey, long lValue)
{
	int world_tag = _parent->tag;
	int world_index = _parent->w_serial;

	GeneralIndexer indexer;
	indexer.SetParam(world_tag, world_index,0);

	if (!indexer.Modify(lKey, lValue)) 
	{
		__PRINTF("修改全局数据失败\n");
	}
	else
	{
		GLog::log(GLOG_INFO,"全局变量操作[modify]: key = %ld, value = %ld", lKey, lValue);
	}
	
}

void
gactive_imp::OI_AssignGlobalValue(long srcKey, long destKey)
{
	ASSERT(srcKey >= 0 && destKey >=0);
	long srcValue = OI_GetGlobalValue(srcKey);
	if(srcValue != 0)
	{
		OI_PutGlobalValue(destKey, srcValue);
	}
	
}

bool 
gactive_imp::OnAttackMirror(const MSG & msg, attack_msg * attack)
{
	if(!_skill_mirror && !_clone_mirror && !_skill_reflect) return false;
	if(!msg.source.IsPlayerClass()) return false;
	if(attack->is_mirror) return false;
	
	//优先技能反射然后技能反弹
	if(_skill_reflect)
	{
		if(abase::Rand(0,100) <= _skill_reflect_rate) 
		{
			attack_msg new_attack;
			new_attack = *attack;
			FillAttackMsg(msg.source, new_attack, 0);
			new_attack.force_attack = OI_GetPVPMask();
			new_attack.is_mirror = true;
			MSG new_msg;
			BuildMessage(new_msg,GM_MSG_ATTACK,msg.source,_parent->ID,_parent->pos,
					0,&new_attack,sizeof(new_attack));
			gmatrix::SendMessage(new_msg, 3); //延迟三个tick发送, 保证先处理红黄名逻辑
			_filters.EF_BeReflected(msg.source);
			return true;
		}
	}

	if(_skill_mirror)
	{
		if(abase::Rand(0,100) > _skill_mirror_rate) return false;
		attack_msg new_attack;
		new_attack = *attack;
		FillAttackMsg(msg.source, new_attack, 0);
		new_attack.force_attack = OI_GetPVPMask();
		new_attack.is_mirror = true;
		MSG new_msg;
		BuildMessage(new_msg,GM_MSG_ATTACK,msg.source,_parent->ID,_parent->pos,
				0,&new_attack,sizeof(new_attack));
		gmatrix::SendMessage(new_msg, 3); //延迟三个tick发送, 保证先处理红黄名逻辑
		return true;
	}
	else if(_clone_mirror)
	{
		if(abase::Rand(0,100) > _clone_mirror_rate) return false;
		if(!OI_IsCloneExist()) return false;
		XID clone_id = GetCloneID();
		if(clone_id.type == -1 || clone_id.id == -1) return false;

		attack->is_mirror = true;
		MSG new_msg;
		BuildMessage(new_msg,GM_MSG_ATTACK, clone_id, msg.source, msg.pos,
				0,attack,sizeof(*attack));
		gmatrix::SendMessage(new_msg);
		return true;
	}
	return false;
}

bool 
gactive_imp::OnEnchantMirror(const MSG & msg, enchant_msg * enchant)
{
	if(!_skill_mirror && !_clone_mirror && !_skill_reflect) return false;
	if(!msg.source.IsPlayerClass()) return false;
	if(enchant->is_mirror) return false;

	if(_skill_reflect)
	{
		if(abase::Rand(0,100) <= _skill_reflect_rate) 
		{
			enchant_msg new_enchant = *enchant;
			FillEnchantMsg(msg.source, new_enchant);
			new_enchant.force_attack = OI_GetPVPMask();
			new_enchant.is_mirror = true;

			MSG new_msg;
			BuildMessage(new_msg,GM_MSG_ENCHANT,msg.source,_parent->ID,_parent->pos,
					0,&new_enchant,sizeof(new_enchant));
			gmatrix::SendMessage(new_msg, 3); //延迟三个tick发送, 保证先处理红黄名逻辑
			_filters.EF_BeReflected(msg.source);
			return true;
		}
	}

	if(_skill_mirror)
	{
		if(abase::Rand(0,100) > _skill_mirror_rate) return false;

		enchant_msg new_enchant = *enchant;
		FillEnchantMsg(msg.source, new_enchant);
		new_enchant.force_attack = OI_GetPVPMask();
		new_enchant.is_mirror = true;

		MSG new_msg;
		BuildMessage(new_msg,GM_MSG_ENCHANT,msg.source,_parent->ID,_parent->pos,
				0,&new_enchant,sizeof(new_enchant));
		gmatrix::SendMessage(new_msg, 3); //延迟三个tick发送, 保证先处理红黄名逻辑
		return true;
	}
	else if(_clone_mirror)
	{
		if(abase::Rand(0,100) > _clone_mirror_rate) return false;
		if(!OI_IsCloneExist()) return false;
		XID clone_id = GetCloneID();
		if(clone_id.type == -1 || clone_id.id == -1) return false;

		enchant->is_mirror = true;
		MSG new_msg;
		BuildMessage(new_msg,GM_MSG_ENCHANT, clone_id, msg.source, msg.pos,
				0,enchant,sizeof(*enchant));
		gmatrix::SendMessage(new_msg);
		return true;
	}
	return false;
}

void 
gactive_imp::SetBreakCasting()
{
	if(_cur_session)
	{
		_cur_session->OnBreakCast();
	}
}

void
gactive_imp::TransferAttack(XID & target, char force_attack, int dmg_plus, int radius, int total)
{
	msg_transfer_attack data;
	data.force_attack = force_attack;
	data.dmg_plus = dmg_plus;
	data.radius = radius;
	data.total = total;
	SendTo<0>(GM_MSG_TRANSFER_ATTACK, target, 0, &data, sizeof(data));
}

void
gactive_imp::HandleTransferAttack(const XID & source, const A3DVECTOR &pos, char force_attack, int dmg_plus, int radius, int total)
{
	world::object_info base_info;
	bool rst = _plane->QueryObject(source,base_info);
	if(!rst) return;

	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(XID(-1,-1),attack,0);

	attack.damage_low += dmg_plus;
	attack.damage_high += dmg_plus;

	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),_parent->ID,_parent->pos,0,&attack,sizeof(attack));

	TranslateAttack(XID(-1,-1),attack,1, radius, total);
	OnAttack();
	std::vector<exclude_target> empty;
	_plane->BroadcastSphereMessage(msg,pos,radius,total,empty);
}

void gactive_imp::SendAttackFeedBack(const XID& target, int mask, int param2)
{
	SendTo2<0>(GM_MSG_ATTACK_FEED_BACK,target,  mask, param2);
}

void gactive_imp::Blowoff()
{
	_runner->blow_off(_parent->ID.id);
}

