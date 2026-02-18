#include <stdlib.h>
#include <common/types.h>
#include <common/message.h>
#include "filter_man.h"
#include "actobject.h"
#include "actsession.h"
#include "playertemplate.h"
#include "obj_interface.h"
#include "player_imp.h"
#include "npc.h"
#include "item.h"
#include "invincible_filter.h"
#include "npcgenerator.h"
#include "clstab.h"
#include "ainpc.h"
#include <gsp_if.h>
#include "petnpc.h"
#include "item/item_petbedge.h"
#include "summonnpc.h"
#include "servicenpc.h"
#include "protectednpc.h"
#include "sfilterdef.h"
#include "mobactivenpc.h"
#include "mobactivedataman.h"
#include "topic_site.h"
#include "raid/raid_world_manager.h"

void 
object_interface::BeHurt(const XID & who,const attacker_info_t & info, int damage,char attacker_mode)
{
	return BeHurt(who,info,damage,false,attacker_mode);
}

void 
object_interface::BeHurt(const XID & who,const attacker_info_t & info, int damage, bool invader,char attacker_mode)
{
//	ASSERT(damage >= 0);
	if(damage <= 0) return;
	_imp->BeHurt(who,info,damage,invader,attacker_mode);
}

void 
object_interface::DecHP(int hp)
{
	ASSERT(hp >= 0);
	_imp->DecHP(hp);
}

void 
object_interface::DecHP(const XID & who, int hp)
{
	ASSERT(hp >= 0);
	_imp->DecHP(who, hp);
}

void 
object_interface::Heal(const XID & healer,size_t life, bool can_heal_bloodpool)
{
	_imp->Heal(healer,life, can_heal_bloodpool);
}

void 
object_interface::Heal(size_t life, bool is_potion, bool can_heal_bloodpool)
{
	__PRINTF("------------玩家回复%d血量\n",life);
	_imp->Heal(life, is_potion, can_heal_bloodpool);
}

bool 
object_interface::DrainMana(int mana)
{
	return _imp->DrainMana(mana);
}

void
object_interface::InjectMana(int mana, bool is_potion)
{
	__PRINTF("------------玩家回复%d魔法\n",mana);
	_imp->InjectMana(mana, is_potion);
}

void
object_interface::InjectDeity(int deity, bool is_potion)
{
	_imp->InjectDeity(deity, is_potion);
}

bool
object_interface::DecDeity(int deity)
{
	return _imp->DecDeity(deity);
}


bool
object_interface::DecInk(int ink)
{
	return _imp->DecInk(ink);
}

void 
object_interface::EnhanceScaleHPGen(int hpgen)
{
	_imp->_en_percent.hp_gen += hpgen;
}

void 
object_interface::ImpairScaleHPGen(int hpgen)
{
	_imp->_en_percent.hp_gen -= hpgen;
}

void 
object_interface::EnhanceScaleMPGen(int mpgen)
{
	_imp->_en_percent.mp_gen += mpgen;
}

void 
object_interface::ImpairScaleMPGen(int mpgen)
{
	_imp->_en_percent.mp_gen -= mpgen;
}

void 
object_interface::EnhanceScaleDPGen(int dpgen)
{
	_imp->_en_percent.dp_gen += dpgen;
}

void 
object_interface::ImpairScaleDPGen(int dpgen)
{
	_imp->_en_percent.dp_gen -= dpgen;
}


void 
object_interface::UpdateHPMPGen()
{
	property_policy::UpdateMPHPGen(_imp);
}

void 
object_interface::UpdateHPMP()
{
	property_policy::UpdateLife(_imp);
	property_policy::UpdateMana(_imp);
	_imp->SetRefreshState();
}

void 
object_interface::UpdateDPGen()
{
	property_policy::UpdateDPGen(_imp);
}

void 
object_interface::UpdateDP()
{
	property_policy::UpdateDeity(_imp);
	_imp->SetRefreshState();
}


void 
object_interface::EnhanceMaxHP(int hp)
{
	_imp->_en_point.max_hp += hp;
}

void 
object_interface::ImpairMaxHP(int hp)
{
	_imp->_en_point.max_hp -= hp;
}
void 
object_interface::EnhanceMaxMP(int mp)
{
	_imp->_en_point.max_mp += mp;
}

void 
object_interface::ImpairMaxMP(int mp)
{
	_imp->_en_point.max_mp -= mp;
}

void 
object_interface::EnhanceMaxDP(int dp)
{
	_imp->_en_point.max_dp += dp;
}

void 
object_interface::ImpairMaxDP(int dp)
{
	_imp->_en_point.max_dp -= dp;
}

void 
object_interface::EnhanceScaleMaxHP(int hp)
{
	_imp->_en_percent.max_hp += hp;
}
void 
object_interface::ImpairScaleMaxHP(int hp)
{
	_imp->_en_percent.max_hp -= hp;
}
void 
object_interface::EnhanceScaleMaxMP(int mp)
{
	_imp->_en_percent.max_mp += mp;
//	property_policy::UpdateMana(_imp);
//	_imp->SetRefreshState();
}

void 
object_interface::ImpairScaleMaxMP(int mp)
{
	_imp->_en_percent.max_mp -= mp;
	property_policy::UpdateMana(_imp);
	_imp->SetRefreshState();
}

void 
object_interface::EnhanceScaleMaxDP(int dp)
{
	_imp->_en_percent.max_dp += dp;
}
void 
object_interface::ImpairScaleMaxDP(int dp)
{
	_imp->_en_percent.max_dp -= dp;
}

void 
object_interface::EnhanceDefense(int def)
{
	_imp->_en_point.defense += def;
}

void 
object_interface::EnhanceScaleAttack(int attack)
{
	_imp->_en_percent.attack += attack;
}

void 
object_interface::ImpairScaleAttack(int attack)
{
	_imp->_en_percent.attack -= attack;
}

void 
object_interface::EnhanceAttack(int attack)
{
	_imp->_en_point.attack += attack;
}

void 
object_interface::ImpairAttack(int attack)
{
	_imp->_en_point.attack -= attack;
}

void 
object_interface::object_interface::ImpairDefense(int def)
{
	_imp->_en_point.defense -= def;
}

void 
object_interface::EnhanceScaleDefense(int def)
{
	_imp->_en_percent.defense += def;
}

void 
object_interface::ImpairScaleDefense(int def)
{
	_imp->_en_percent.defense -= def;
}

void
object_interface::EnhanceArmor(int ac)
{
	_imp->_en_point.armor += ac;
}

void 
object_interface::ImpairArmor(int ac)
{
	_imp->_en_point.armor -= ac;
}

void 
object_interface::EnhanceScaleArmor(int ac)
{
	_imp->_en_percent.armor += ac;
}

void 
object_interface::ImpairScaleArmor(int ac)
{
	_imp->_en_percent.armor -= ac;
}

void 
object_interface::EnhanceResistance(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_point.resistance[cls] += res;
}

void 
object_interface::ImpairResistance(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_point.resistance[cls] -= res;
}

void 
object_interface::EnhanceScaleResistance(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_percent.resistance[cls] += res;
}

void 
object_interface::ImpairScaleResistance(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_percent.resistance[cls] -= res;
}

void 
object_interface::EnhanceResistanceTenaciy(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_point.resistance_tenaciy[cls] += res;
}

void 
object_interface::ImpairResistanceTenaciy(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_point.resistance_tenaciy[cls] -= res;
}

void 
object_interface::EnhanceScaleResistanceTenaciy(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_percent.resistance_tenaciy[cls] += res;
}

void 
object_interface::ImpairScaleResistanceTenaciy(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_percent.resistance_tenaciy[cls] -= res;
}

void 
object_interface::EnhanceResistanceProficiency(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_point.resistance_proficiency[cls] += res;
}

void 
object_interface::ImpairResistanceProficiency(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_point.resistance_proficiency[cls] -= res;
}

void 
object_interface::EnhanceScaleResistanceProficiency(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_percent.resistance_proficiency[cls] += res;
}

void 
object_interface::ImpairScaleResistanceProficiency(size_t cls, int res)
{
	ASSERT(cls < 6);
	_imp->_en_percent.resistance_proficiency[cls] -= res;
}

void 
object_interface::EnhanceDamage(int dmg)
{
	_imp->_en_point.damage += dmg;
}

void 
object_interface::ImpairDamage(int dmg)
{
	_imp->_en_point.damage -= dmg;
}

void 
object_interface::EnhanceScaleDamage(int dmg)
{
	_imp->_en_percent.damage += dmg;
}

void 
object_interface::ImpairScaleDamage(int dmg)
{
	_imp->_en_percent.damage -= dmg;
}

void 
object_interface::EnhanceDmgReduce(int dmg_reduce)
{
	_imp->_en_point.dmg_reduce += dmg_reduce;
}

void 
object_interface::ImpairDmgReduce(int dmg_reduce)
{
	_imp->_en_point.dmg_reduce -= dmg_reduce;
}

void 
object_interface::EnhanceScaleDmgReduce(int dmg_reduce)
{
	_imp->_en_percent.dmg_reduce += dmg_reduce;
}

void 
object_interface::ImpairScaleDmgReduce(int dmg_reduce)
{
	_imp->_en_percent.dmg_reduce -= dmg_reduce;
}

void 
object_interface::EnhanceScaleIgnDmgReduce(int ign_dmg_reduce)
{
	_imp->_en_percent.ign_dmg_reduce += ign_dmg_reduce;
}

void 
object_interface::ImpairScaleIgnDmgReduce(int ign_dmg_reduce)
{
	_imp->_en_percent.ign_dmg_reduce -= ign_dmg_reduce;
}

void 
object_interface::EnhanceScaleDmgChange(int dmg_change)
{
	_imp->_en_percent.dmg_change += dmg_change;
}

void 
object_interface::ImpairScaleDmgChange(int dmg_change)
{
	_imp->_en_percent.dmg_change -= dmg_change;
}


void 
object_interface::EnhanceSpeed(float speedup)
{
	_imp->_en_point.run_speed  += speedup;
}

void 
object_interface::ImpairSpeed(float speedup)
{
	_imp->_en_point.run_speed  -= speedup;
}

void 
object_interface::EnhanceScaleSpeed(int inc)
{
	_imp->_en_percent.speed  += inc;
}

void 
object_interface::ImpairScaleSpeed(int inc)
{
	_imp->_en_percent.speed  -= inc;
}

void 
object_interface::EnhanceOverrideSpeed(float speed)
{
	_imp->_en_point.override_speed += speed;
}

void 
object_interface::ImpairOverrideSpeed(float speed)
{
	if((_imp->_en_point.override_speed -= speed) <= 1e-3)
	{
		_imp->_en_point.override_speed  = 0;
	}
}

void object_interface::EnhanceMountSpeed(float inc)
{
	_imp->_en_point.mount_speed += inc;
}

void object_interface::ImpairMountSpeed(float dec)
{
	_imp->_en_point.mount_speed -= dec;
}

//属性
const basic_prop & 
object_interface::GetBasicProp()
{
	return _imp->_basic;
}

const q_extend_prop & 
object_interface::GetExtendProp()
{
	return _imp->_cur_prop;
}

const q_enhanced_param & 
object_interface::GetEnhancedParam()
{
	return _imp->_en_point;
}

const q_scale_enhanced_param & 	
object_interface::GetScaleEnhanecdParam()
{
	return _imp->_en_percent;
}

const q_item_prop &
object_interface::GetCurWeapon()
{
	return _imp->_cur_item;
}

const int
object_interface::GetCurWeaponType()
{
	return _imp->_cur_item.weapon_class;
}

//filter
void 
object_interface::AddFilter(filter* fp)
{
	ASSERT(fp);
	if((fp->GetMask() & filter::FILTER_MASK_BUFF) > 0 && _imp->IsTurnBuffState())
	{
		return;
	}
	
	if((fp->GetMask() & filter::FILTER_MASK_DEBUFF) > 0 && _imp->IsTurnDebuffState())
	{
		return;
	}
	if( GetBattlegroundFlag() )
	{
		fp->MergeMask( filter::FILTER_MASK_BATTLE_BUFF );
	}
	if(_imp->GetWorldManager()->GetRaidType() == RT_CAPTURE)
	{
		fp->MergeMask( filter::FILTER_MASK_BATTLE_BUFF );
	}
	if(fp->CanOverlay() && fp->GetMaxOverlayCnt() > 1)
	{
		printf("1. Add Filter id=%d overlay_cnt=%d, max_overlay_cnt=%d\n", fp->GetFilterID(), fp->GetOverlayCnt(), fp->GetMaxOverlayCnt());
		if(!fp->Overlay())
		{
			printf("2. Add Filter id=%d overlay_cnt=%d, max_overlay_cnt=%d\n", fp->GetFilterID(), fp->GetOverlayCnt(), fp->GetMaxOverlayCnt());
			delete fp;
			fp = 0;
			return;
		}
		printf("3. Add Filter id=%d overlay_cnt=%d, max_overlay_cnt=%d\n", fp->GetFilterID(), fp->GetOverlayCnt(), fp->GetMaxOverlayCnt());
	}
	_imp->_filters.AddFilter(fp);
}

void 
object_interface::RemoveFilter(int filter_id)
{
	_imp->_filters.RemoveFilter(filter_id);
}

bool 
object_interface::IsFilterExist(int filter_id)
{
	return _imp->_filters.IsFilterExist(filter_id);
}

bool object_interface::IsMultiFilterExist(int id)
{
	for(int i = 0; i < 10; i ++)
	{
		if (IsFilterExist(MULTI_FILTER_BEGIN + id * 10 + i))
			return true;
	}
	return false;
}

int object_interface::FilterCnt(int buff_type)
{
	return _imp->_filters.FilterCnt(buff_type);
}

void object_interface::OnFilterAdd(int filter_id, const XID& caster)
{
	_imp->_filters.EF_OnFilterAdd(filter_id, caster);
}

void object_interface::AdjustSkill(int skill_id, short& level)
{
	_imp->_filters.EF_AdjustSkill(skill_id, level);
}

bool object_interface::ModifyFilter(int filterid, int ctrlname, void * ctrlval, size_t ctrllen)
{
	return _imp->_filters.ModifyFilter(filterid, ctrlname, ctrlval, ctrllen);
}

bool
object_interface::Resurrect(float exp_reduce, float hp_recover)
{
	_imp->EnterResurrectReadyState(exp_reduce, hp_recover);
	return true;
}

bool 
object_interface::Resurrect(float exp_reduce)
{
	return Resurrect(exp_reduce, 0.1f);
}

//查询 
bool
object_interface::IsDead()
{
	return _imp->_parent->IsZombie();
}

bool 
object_interface::IsMember(const XID & member)
{
	return _imp->OI_IsMember(member);
}

bool
object_interface::IsInTeam()
{
	return _imp->OI_IsInTeam();
}

bool
object_interface::IsTeamLeader()
{
	return _imp->OI_IsTeamLeader();
}

bool object_interface::GetLeader(XID & leader)
{       
	return _imp->OI_GetLeader(leader);
}

int
object_interface::GetTeamLeader()
{
	return _imp->OI_GetTeamLeader();
}

bool 
object_interface::IsPVPEnable()
{
	return _imp->OI_IsPVPEnable();
}

bool 
object_interface::IsMount()
{
	return _imp->OI_IsMount();
}

bool
object_interface::IsFlying()
{
	return _imp->OI_IsFlying();

}

bool
object_interface::IsGathering()
{
	return _imp->OI_IsGathering();
}

bool 
object_interface::IsTransformState()
{
	return _imp->IsTransformState();
}

bool 
object_interface::IsBindState()
{
	return _imp->IsBindState();
}


int
object_interface::GetFaction()
{
	return _imp->GetFaction();
}

bool 
object_interface::IsFamilyMember()
{
	return _imp->OI_GetFamilyID() > 0;
}

int 
object_interface::GetFamilyID()
{
	return _imp->OI_GetFamilyID();
}

void
object_interface::UpdateFamilySkill(int skill ,int ability)
{
	int fid = _imp->OI_GetFamilyID();
	if(fid > 0)
	{
		GMSV::SendUpdateFamilySkill(_imp->_parent->ID.id, fid, skill, ability);
	}
}


bool 
object_interface::IsMafiaMember()
{
	return _imp->OI_IsMafiaMember();
}

int
object_interface::GetMafiaID()
{
	return _imp->OI_GetMafiaID();
}

int
object_interface::GetClass()
{
	return ((gactive_object*)_imp->_parent)->GetClass();
}

int
object_interface::GetEnemyFaction()
{
	return _imp->GetEnemyFaction();
}

const A3DVECTOR &
object_interface::GetPos()
{
	return _imp->_parent->pos;
}

const int &
object_interface::GetTag()
{
	return _imp->_parent->tag;
}

const XID &
object_interface::GetSelfID()
{
	return _imp->_parent->ID;
}

const XID 
object_interface::GetCloneID()
{
	return _imp->GetCloneID();
}

const XID 
object_interface::GetSummonID()
{
	return _imp->GetSummonID();
}
const XID &
object_interface::GetCurTargetID()
{
	return _imp->GetCurTarget();
}

float
object_interface::GetBodySize()
{
	return _imp->_parent->body_size;
}

int
object_interface::QueryObject(const XID & who, A3DVECTOR & pos ,float & body_size)
{
	world::object_info info;
	if(!_imp->_plane->QueryObject(who,info)) return 0;
	pos = info.pos;
	body_size = info.body_size;
	if(info.state &  world::QUERY_OBJECT_STATE_ZOMBIE) return 2;
	return 1;
}

void
object_interface::SendClientMsgSkillCasting(int target_cnt, const XID* targets, int skill, unsigned short time,unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index)
{
	_imp->_runner->cast_skill(target_cnt,targets,skill,time,level, state, cast_speed_rate, pos, spirit_index);
}

void
object_interface::SendClientMsgSkillInterrupted(char reason, char spirit_index)
{
	_imp->_runner->skill_interrupt(reason, spirit_index);
}

void
object_interface::SendClientMsgSkillContinue(int skill_id, char spirit_index)
{
	_imp->_runner->skill_continue(skill_id, spirit_index);
}

void 
object_interface::SendClientRushMode(unsigned char is_active)
{
	_imp->_runner->active_rush_mode(is_active);
}

void 
object_interface::SendClientSkillAbility(int id, int ability)
{
	_imp->_runner->skill_ability_notify(id, ability);
}

void 
object_interface::SendClientInstantSkill(int target_cnt, const XID* targets, int skill, unsigned char level, const A3DVECTOR& pos, char spirit_index)
{
	_imp->_runner->cast_instant_skill(target_cnt,targets,skill,level,pos,spirit_index);
}

void 
object_interface::SendClientCastPosSkill(const A3DVECTOR & pos, int skill,unsigned short time, unsigned char level)
{
	_imp->_runner->cast_pos_skill(pos,skill,time,level);
}

void
object_interface::SendClientMsgSkillPerform(char spirit_index)
{
	_imp->_runner->skill_perform(spirit_index);
}

void 
object_interface::SendClientCurSpeed()
{
	_imp->_runner->get_extprop_move();
}


bool 
object_interface::CanAttack(const XID & target)
{
	return _imp->CanAttack(target);
}

void object_interface::FillAttackMsg(const XID & target, attack_msg & attack,int use_projectile)
{
	_imp->FillAttackMsg(target,attack,use_projectile);
}

//攻击
void object_interface::Attack(const XID & target, attack_msg & attack, int use_projectile)
{
	_imp->SendAttackMsg(target,attack);
	_imp->OnAttack();
}

void object_interface::SendRegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,int count,std::vector<exclude_target>& target_exclude)
{
	if(count <= 0) return;
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1), attack.ainfo.attacker, pObj->pos, 0, &attack, sizeof(attack));
	_imp->_plane->BroadcastSphereMessage(msg,pos,radius,count,target_exclude);
}

void object_interface::RegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,int max_count, std::vector<exclude_target>& target_exclude, bool trans_attack)
{
	if(max_count <= 0) return;
	//球型
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&attack,sizeof(attack));

	if(trans_attack)
	{
		_imp->TranslateAttack(XID(-1,-1),attack, 1, radius, max_count);
	}
	_imp->OnAttack();
	//进行广播活动
	_imp->_plane->BroadcastSphereMessage(msg,pos,radius,max_count,target_exclude);
}

void object_interface::RegionAttack2(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,float range,int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//圆柱
	//填充一下必要的数据
	//_imp->FillAttackMsg(XID(-1,-1),attack,use_projectile);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1),attack, 2, radius, max_count);
	_imp->OnAttack();

	//进行广播活动 
	A3DVECTOR offset= pos;
	offset -= msg.pos;
	offset.normalize();
	offset *= range;
	offset += msg.pos;

	_imp->_plane->BroadcastCylinderMessage(msg,offset,radius,max_count,target_exclude);
}

void object_interface::RegionAttack3(const A3DVECTOR& pos, float cos_half_angle,attack_msg & attack,int use_projectile, float range,int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//圆锥
	//填充一下必要的数据
	//_imp->FillAttackMsg(XID(-1,-1),attack,use_projectile);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos, 0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1),attack, 3, cos_half_angle, max_count);
	_imp->OnAttack();

	//进行广播活动
	_imp->_plane->BroadcastTaperMessage(msg,pos,range,cos_half_angle,max_count, target_exclude);
}

void object_interface::RegionAttackFaceLine(float radius,attack_msg & attack,int up,float range,int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1),attack, 2, radius, max_count);
	_imp->OnAttack();

	//进行广播活动 
	unsigned char dir = GetDirection();
	const static float deg = 3.1415926f / 180;
	float distAngle = (dir * 1.0f / 256 * 360) * deg;
	A3DVECTOR offset= pObj->pos;
	offset.x += (cosf(distAngle) * range);
	offset.z += (sinf(distAngle) * range);
	_imp->_plane->BroadcastCylinderMessage(msg,offset,radius,max_count,target_exclude);
}

void object_interface::RegionAttackFaceSector(float cos_half_angle,attack_msg & attack,int up,float range,int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//圆锥

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos, 0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1),attack, 3, cos_half_angle, max_count);
	_imp->OnAttack();

	//进行广播活动
	unsigned char dir = GetDirection();
	const static float deg = 3.1415926f / 180;
	float distAngle = (dir * 1.0f / 256 * 360) * deg;
	A3DVECTOR offset= pObj->pos;
	offset.x += (cosf(distAngle) * range);
	offset.z += (sinf(distAngle) * range);

	_imp->_plane->BroadcastTaperMessage(msg,offset,range,cos_half_angle,max_count, target_exclude);
}

void object_interface::RegionAttackCross(float radius, float range, float angle_to_north, attack_msg& attack, int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1), attack, 2, radius, max_count);
	_imp->OnAttack();

	//进行广播活动 
	const static float deg = 3.1415926f / 180;
	for(int i = 0; i < 4; i ++)
	{
		float distAngle = (angle_to_north + i * 90.0f) * deg;
		A3DVECTOR offset= pObj->pos;
		offset.x += (cosf(distAngle) * range);
		offset.z += (sinf(distAngle) * range);
		_imp->_plane->BroadcastCylinderMessage(msg,offset,radius,max_count, target_exclude);
	}
}

void object_interface::SendEnchantMsg(const XID& target, enchant_msg & enchant)
{
	enchant.attack_stamp = _imp->GetAttackStamp();
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT, target, enchant.ainfo.attacker, pObj->pos,
			0,&enchant,sizeof(enchant));
	gmatrix::SendMessage(msg);
}

void object_interface::MultiAttack(const XID * target,size_t size, attack_msg & attack,int use_projectile)
{
	//多播
	//填充一下必要的数据
	//_imp->FillAttackMsg(XID(-1,-1),attack,use_projectile);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(-1,-1),pObj->ID,pObj->pos,
	                  0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1),attack, 4);
	_imp->OnAttack();

	//进行广播活动
	gmatrix::SendMessage(target, target + size, msg);
}

void object_interface::MultiAttack(const XID * target,size_t size, attack_msg & attack,MsgAdjust & adj,int use_projectile)
{
	//多播
	//填充一下必要的数据
	//_imp->FillAttackMsg(*target,attack,use_projectile);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,XID(-1,-1),pObj->ID,pObj->pos,
	                  0,&attack,sizeof(attack));
	_imp->TranslateAttack(XID(-1,-1),attack, 4);
	_imp->OnAttack();
	
	for(size_t i = 0; i < size; i ++)
	{
		msg.target = target[i];
		gmatrix::SendMessage(msg);
		adj.AdjustAttack(attack);
	}
}

void object_interface::SendRegionEnchant1Msg(const A3DVECTOR& pos, float radius,enchant_msg & enchant, size_t max_count, std::vector<exclude_target>& target_exclude)
{
	//球形
	//填充一下必要的数据
	//_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),enchant.ainfo.attacker, pObj->pos,
	                  0,&enchant,sizeof(enchant));

	//_imp->TranslateEnchant(XID(-1,-1),enchant);
	//进行广播活动
	_imp->_plane->BroadcastSphereMessage(msg,pos,radius,max_count, target_exclude);
}

//技能
void object_interface::Enchant(const XID & target, enchant_msg & msg)
{
	_imp->FillEnchantMsg(target,msg);
	_imp->TranslateEnchant(target,msg);
	_imp->SendEnchantMsg(GM_MSG_ENCHANT,target,msg);
}

void object_interface::EnchantZombie(const XID & target, enchant_msg & msg)
{
	_imp->FillEnchantMsg(target,msg);
	_imp->TranslateEnchant(target,msg);
	_imp->SendEnchantMsg(GM_MSG_ENCHANT_ZOMBIE,target,msg);
}

void object_interface::RegionEnchant1(A3DVECTOR const& pos, float radius, enchant_msg& enchant, size_t max_count, std::vector<exclude_target, std::allocator<exclude_target> >& target_exclude)
//void object_interface::RegionEnchant1(const A3DVECTOR& pos, float radius,enchant_msg & enchant, int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//球形
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,
	                  0,&enchant,sizeof(enchant));

	_imp->TranslateEnchant(XID(-1,-1),enchant);
	//进行广播活动
	_imp->_plane->BroadcastSphereMessage(msg,pos,radius,max_count, target_exclude);
}

void object_interface::RegionEnchant2(A3DVECTOR const& pos, float radius, enchant_msg& enchant, size_t max_count, std::vector<exclude_target, std::allocator<exclude_target> >& target_exclude)
//void object_interface::RegionEnchant2(const A3DVECTOR& pos, float radius,enchant_msg & enchant, int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//圆柱
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,
	                  0,&enchant,sizeof(enchant));

	//进行广播活动
	A3DVECTOR offset= pos;
	offset -= msg.pos;
	offset.normalize();
	offset *= enchant.attack_range;
	offset += msg.pos;

	_imp->TranslateEnchant(XID(-1,-1),enchant);

	_imp->_plane->BroadcastCylinderMessage(msg,offset,radius, max_count, target_exclude);
}

void object_interface::RegionEnchant3(A3DVECTOR const& pos, float cos_half_angle, enchant_msg& enchant, size_t max_count, std::vector<exclude_target, std::allocator<exclude_target> >& target_exclude)
//void object_interface::RegionEnchant3(const A3DVECTOR& pos, float cos_half_angle,enchant_msg & enchant,int max_count, std::vector<exclude_target>& target_exclude)
{
	if(max_count <= 0) return;
	//圆球
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,
	                  0,&enchant,sizeof(enchant));

	//进行广播活动
	_imp->TranslateEnchant(XID(-1,-1),enchant);
	_imp->_plane->BroadcastTaperMessage(msg,pos,enchant.attack_range,cos_half_angle,max_count,target_exclude);
}

void object_interface::MultiEnchant(const XID * target,size_t size, enchant_msg & enchant)
{
	//多播
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(-1,-1),pObj->ID,pObj->pos,
	                  0,&enchant,sizeof(enchant));

	//进行广播活动
	_imp->TranslateEnchant(XID(-1,-1),enchant);
	gmatrix::SendMessage(target, target + size, msg);
}

void object_interface::TeamEnchant(enchant_msg & enchant, bool exclude_self, bool norangelimit)
{
	ASSERT(IsInTeam());
	//多播
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(-1,-1),pObj->ID,pObj->pos,0,&enchant,sizeof(enchant));

	_imp->TranslateEnchant(XID(-1,-1),enchant);
	//进行广播活动
	_imp->SendMsgToTeam(msg,enchant.attack_range,exclude_self, norangelimit, -1);
}

void object_interface::TeamEnchantZombie(enchant_msg & enchant,bool exclude_self, bool norangelimit, int max_count)
{
	ASSERT(IsInTeam());
	//多播
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);
	
	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT_ZOMBIE,XID(-1,-1),pObj->ID,pObj->pos,0,&enchant,sizeof(enchant));

	_imp->TranslateEnchant(XID(-1,-1),enchant);
	//进行广播活动
	_imp->SendMsgToTeam(msg,enchant.attack_range,exclude_self, norangelimit, max_count);
}

void object_interface::RegionEnchantFaceLine(float radius, enchant_msg& enchant, int max_count, std::vector<exclude_target>& target_exclude)
{
	//圆柱
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&enchant,sizeof(enchant));

	_imp->TranslateEnchant(XID(-1,-1),enchant);

	//进行广播活动
	unsigned char dir = GetDirection();
	const static float deg = 3.1415926f / 180;
	float distAngle = (dir * 1.0f / 256 * 360) * deg;
	A3DVECTOR offset= pObj->pos;
	offset.x += (cosf(distAngle) * enchant.attack_range);
	offset.z += (sinf(distAngle) * enchant.attack_range);

	_imp->_plane->BroadcastCylinderMessage(msg,offset,radius,max_count,target_exclude);
}

void object_interface::RegionEnchantFaceSector(float cos_half_angle, enchant_msg & enchant, int max_count,std::vector<exclude_target>& target_exclude)
{
	//扇形
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&enchant,sizeof(enchant));

	_imp->TranslateEnchant(XID(-1,-1),enchant);

	//进行广播活动
	unsigned char dir = GetDirection();
	const static float deg = 3.1415926f / 180;
	float distAngle = (dir * 1.0f / 256 * 360) * deg;
	A3DVECTOR offset= pObj->pos;
	offset.x += (cosf(distAngle) * enchant.attack_range);
	offset.z += (sinf(distAngle) * enchant.attack_range);

	_imp->_plane->BroadcastTaperMessage(msg,offset,enchant.attack_range,cos_half_angle,max_count,target_exclude);
}

void object_interface::RegionEnchantCross(float radius, float angle_to_north, enchant_msg& enchant, int max_count, std::vector<exclude_target>& target_exclude)
{
	//圆柱
	//填充一下必要的数据
	_imp->FillEnchantMsg(XID(-1,-1),enchant);

	//生成消息
	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg,GM_MSG_ENCHANT,XID(GM_TYPE_ACTIVE,-1),pObj->ID,pObj->pos,0,&enchant,sizeof(enchant));

	_imp->TranslateEnchant(XID(-1,-1),enchant);

	//进行广播活动
	const static float deg = 3.1415926f / 180;
	for(int i = 0; i < 4; i ++)
	{
		float distAngle = (angle_to_north + i * 90.0f) * deg;
		A3DVECTOR offset= pObj->pos;
		offset.x += (cosf(distAngle) * enchant.attack_range);
		offset.z += (sinf(distAngle) * enchant.attack_range);
		_imp->_plane->BroadcastCylinderMessage(msg,offset,radius,max_count,target_exclude);
	}
}

//物品
int object_interface::TakeOutItem(int item_id)
{
	return _imp->TakeOutItem(item_id, 1);
}

int object_interface::TakeOutItem(int item_id, int count)
{
	return _imp->TakeOutItem(item_id, count);
}

int object_interface::TakeOutItem(int inv_index, int item_id, int count)
{
	return _imp->TakeOutItem(inv_index, item_id, count);
}

bool object_interface::CheckItem(int item_id,size_t count)
{
	return _imp->CheckItemExist(item_id,count);
}

bool object_interface::CheckItem(int inv_index, int item_id, size_t count)
{
	return _imp->CheckItemExist(inv_index,item_id,count);
}

size_t object_interface::QueryItemPrice(int inv_index,int item_id)
{
	return _imp->CheckItemPrice(inv_index, item_id);
}

size_t 
object_interface::QueryItemPrice(int item_id)
{
	return gmatrix::GetDataMan().get_item_sell_price(item_id);
}

size_t object_interface::GetMoney()
{
	return _imp->GetMoneyAmount();
}

void object_interface::AddMoney(size_t inc)
{
	return _imp->AddMoneyAmount(inc);
}

void
object_interface::DecMoney(size_t money)
{
	return _imp->DecMoneyAmount(money);
}


bool object_interface::UseProjectile(size_t count)
{
	return false;
}

bool object_interface::SessionOnAttacked(int session_id)
{
	act_session * pSession;
	if(_imp->GetCurSessionID() != session_id || !(pSession = _imp->GetCurSession()))
	{
		return true;
	}
	return pSession->OnAttacked();
}

bool object_interface::IsEquipWing()
{
	return false;
}


void object_interface::IncVisibleState(unsigned short state)
{
	ASSERT(state < 64);
	_imp->_buff.IncVisibleState(state);
}

void object_interface::DecVisibleState(unsigned short state)
{
	ASSERT(state < 64);
	_imp->_buff.DecVisibleState(state);
}

void object_interface::ClearVisibleState(unsigned short state)
{
	ASSERT(state < 64);
	_imp->_buff.ClearVisibleState(state);
}

void object_interface::IncDirVisibleState(unsigned short state, short dir)
{
	ASSERT(state < 32);
	gactive_object *pObj = (gactive_object*)_imp->_parent;
	pObj->extend_dir_state |= state;
	pObj->extend_dir_state_dirs[state] = dir;
	_imp->_runner->dir_visible_state(state, dir, true);
}

void object_interface::DecDirVisibleState(unsigned short state, short dir)
{
	ASSERT(state < 32);
	gactive_object *pObj = (gactive_object*)_imp->_parent;
	pObj->extend_dir_state &= ~state;
	pObj->extend_dir_state_dirs[state] = 0;
	_imp->_runner->dir_visible_state(state, dir, false);
}

void object_interface::ClearDirVisibleState(unsigned short state, short dir)
{
	ASSERT(state < 32);
	gactive_object *pObj = (gactive_object*)_imp->_parent;
	pObj->extend_dir_state = 0;
	memset(pObj->extend_dir_state_dirs, 0, sizeof(pObj->extend_dir_state_dirs));
	pObj->extend_dir_state_dirs[state] = 0;
}

void object_interface::UpdateBuff(short buff_id, short buff_level, int end_time, int overlay_cnt)
{
	_imp->UpdateBuff(buff_id, buff_level, end_time, overlay_cnt); 
}

void object_interface::RemoveBuff(short buff_id, short buff_level)
{
	_imp->RemoveBuff(buff_id, buff_level);
}

void object_interface::ChangeShape(int shape)
{
	_imp->ChangeShape(shape);
	_imp->_runner->change_shape(shape);
}

bool 
object_interface::IsSilentSeal()
{
	return _imp->GetSilentSealMode();
}

bool 
object_interface::IsDietSeal()
{
	return _imp->GetDietSealMode();
}

bool 
object_interface::IsMeleeSeal()
{
	return _imp->GetMeleeSealMode();
}

bool 
object_interface::IsRootSeal()
{
	return _imp->GetRootMode();
}


bool
object_interface::IsCrit()
{
	return _imp->IsCrit();
}

void 
object_interface::SetSilentSeal(bool isSeal)
{
	_imp->SetSilentSealMode(isSeal);
	if(isSeal)
	{
		_imp->OI_OnSilentSeal();
	}
}

void 
object_interface::SetSelfSilentSeal(bool isSeal)
{
	_imp->SetSilentSealMode(isSeal);
}

void 
object_interface::SetDietSeal(bool isSeal)
{
	return _imp->SetDietSealMode(isSeal);
}

void 
object_interface::SetMeleeSeal(bool isSeal)
{
	return _imp->SetMeleeSealMode(isSeal);
}

void 
object_interface::SetRootSeal(bool isSeal)
{
	return _imp->SetRootMode(isSeal);
}

void
object_interface::ForbidAttack()
{
	_imp->OI_DenyAttack();
}

void 
object_interface::AllowAttack()
{
	_imp->OI_AllowAttack();
}

int object_interface::GetShape()
{
	return _imp->GetShape();
}

bool object_interface::IsAggressive()
{
	return _imp->_parent->msg_mask & gobject::MSG_MASK_PLAYER_MOVE;
}

void object_interface::SetAggressive(bool isActive)
{
	if(isActive)
	{
		_imp->_parent->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}
	else
	{
		_imp->_parent->msg_mask &= ~gobject::MSG_MASK_PLAYER_MOVE;
	}
}

void object_interface::UpdateDefenseData()
{
	property_policy::UpdateDefense(_imp->GetObjectClass(),_imp);
}

void object_interface::UpdateAttackData()
{
	_imp->OI_UpdataAttackData();
}

void object_interface::UpdateMagicData()
{
	property_policy::UpdateMagic(_imp);
}

void object_interface::UpdateSpeedData()
{
	property_policy::UpdateSpeed(_imp);
}

void object_interface::EnterCombatState()
{
	_imp->SetCombatState();
}

void object_interface::AddAggro(const XID & who, int rage)
{
	if(rage <0) return;
	_imp->AddNPCAggro(who,rage);
}

void object_interface::AddAggroToEnemy(const XID & helper, int rage)
{
	if(rage <0) return;
	_imp->AddAggroToEnemy(helper,rage);
}
	
void object_interface::KnockBack(const XID & attacker, const A3DVECTOR &source,float distance)
{
	_imp->KnockBack(attacker,source, distance);
}

void object_interface::Flee(const XID & attacker, float distance)
{
	_imp->Flee(attacker, distance);
}

void object_interface::SendClientNotifyRoot(unsigned char type)
{
	_imp->_runner->notify_root(type);
}

void object_interface::SendClientSelfNotifyRoot(unsigned char type)
{
	_imp->_runner->self_notify_root(type);
}

void object_interface::SendClientDispelRoot(unsigned char type)
{
	_imp->_runner->dispel_root(type);
}


void object_interface::SetATDefenseState(char state)
{
	_imp->_at_defense_state |= state;
}

GNET::SkillWrapper & object_interface::GetSkillWrapper()
{
	return _imp->_skill;
}

void 
object_interface::SetRetortState()
{
	_imp->SetRetortState();
}

void 
object_interface::SetNextAttackState(char state)
{
	_imp->SetNextAttackState(state);
}


bool
object_interface::GetRetortState()
{
	return _imp->GetRetortState();
}

void 
object_interface::SendHealMsg(const XID & target, int hp) 
{
	_imp->SendTo<0>(GM_MSG_EXTERN_HEAL,target,hp);
}

void 
object_interface::SendAddManaMsg(const XID & target, int mp) 
{
	_imp->SendTo<0>(GM_MSG_EXTERN_ADD_MANA,target,mp);
}


void
object_interface::Reclaim()
{
	_imp->SendTo<0>(GM_MSG_OBJ_ZOMBIE_END,_imp->_parent->ID,0);
}

void
object_interface::SendClientEnchantResult(const XID & caster, int skill, char level , bool invader,char at_state, char stamp,int value)
{
	_imp->_runner->enchant_result(caster, skill,level, invader,at_state, stamp,value);
}

void 
object_interface::SetInvincibleFilter(bool is_vin,int timeout,bool immune)
{
	if(is_vin)
	{
		AddFilter(new invincible_filter(_imp,FILTER_INVINCIBLE,timeout,immune));
	}
	else
	{
		RemoveFilter(FILTER_INVINCIBLE);
	}
}

void 
object_interface::LockEquipment(bool is_lock)
{
	_imp->LockEquipment(is_lock);
}

void 
object_interface::BindToGound(bool is_bind)
{
	_imp->BindToGound(is_bind);
}

void 
object_interface::ClearSpecFilter(int mask, int count)
{
	_imp->_filters.ClearSpecFilter(mask,count);
}

void 
object_interface::ClearRandomSpecFilter(int mask, int count)
{
	_imp->_filters.ClearRandomSpecFilter(mask,count);
}

void 
object_interface::BeTaunted(const XID & who, int time)
{
	if(time <=0) time = 1;
	_imp->BeTaunted(who,time);
}

void 
object_interface::BeTaunted2(const XID & who, int time)
{
	if(time <=0) time = 1;
	_imp->BeTaunted2(who,time);
}

void 
object_interface::CreateMinors(const A3DVECTOR & pos, const  object_interface::minor_param & param)
{
	npc_template * pTemplate = npc_stubs_manager::Get(param.mob_id);
	if(!pTemplate) return;
	
	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = param.mob_id;
	if(param.use_parent_faction)
	{
		//只有使用parent的faction时才会设置，否则是默认值 
		ent.accept_ask_for_help = true;
		ent.monster_faction_accept_for_help = _imp->GetMonsterFaction();
	}
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;

	int aggro_policy = param.policy_aggro;
	int pcid = param.policy_classid;
	if(pcid == 0)
	{
		pcid = CLS_NPC_AI_POLICY_MINOR;
	}

	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = npc_spawner::CreateMobBase(NULL,_imp->_plane,ent,-1,0,pos,cid,abase::Rand(0,255),pcid,aggro_policy,NULL,param.remain_time, param.script_data, param.script_size);
	if(pNPC)
	{
		gnpc_imp * pImp = (gnpc_imp*)pNPC->imp;
		if(param.parent_is_leader)
		{
			pImp->_leader_id = _imp->_parent->ID;
		}
		else
		{
			pImp->_leader_id = param.spec_leader_id;
		}
		pNPC->npc_idle_hearbeat = 1;

		if(param.use_parent_faction)
		{
			pImp->_faction = _imp->_faction;
			pImp->_enemy_faction = _imp->GetEnemyFaction();
			pNPC->monster_faction |= _imp->GetFactionAskHelp();
		}
		pImp->_basic.exp =(int)(pImp->_basic.exp*param.exp_factor);
		if(pImp->_basic.exp < 0) pImp->_basic.exp = 0;

		pImp->_money_scale = param.money_scale;
		pImp->_drop_rate = param.drop_rate;
		
		pImp->SetLifeTime(param.remain_time);
		pImp->SetDieWithLeader(param.die_with_leader);

		if(param.vis_id)
		{
			pNPC->vis_tid = param.vis_id;
		}

		if(param.mob_name_size)
		{
			ASSERT(param.mob_name_size <= sizeof(param.mob_name)); 
			memcpy(pNPC->npc_name, param.mob_name,param.mob_name_size);
			pNPC->name_size = param.mob_name_size;
			pNPC->object_state |= gactive_object::STATE_NPC_NAME;
		}

		_imp->_plane->InsertNPC(pNPC);
		pImp->_runner->enter_world();
		pImp->OnCreate();
		pNPC->Unlock();
	}
}

void 
object_interface::CreateMob(world *pPlane,const A3DVECTOR & pos, const  object_interface::minor_param & param)
{
	npc_template * pTemplate = npc_stubs_manager::Get(param.mob_id);
	if(!pTemplate) return;
	
	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = param.mob_id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;

	int aggro_policy = param.policy_aggro;
	int pcid = param.policy_classid;
	if(pcid == 0)
	{
		pcid = CLS_NPC_AI_POLICY_MINOR;
	}
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = npc_spawner::CreateMobBase(NULL,pPlane,ent,-1,0,pos,cid,abase::Rand(0,255),pcid,aggro_policy,NULL,param.remain_time, param.script_data, param.script_size);
	if(pNPC)
	{
		gnpc_imp * pImp = (gnpc_imp*)pNPC->imp;

		//调整经验参数等
		pImp->_basic.exp =(int)(pImp->_basic.exp*param.exp_factor);
		if(pImp->_basic.exp < 0) pImp->_basic.exp = 0;

		pImp->_money_scale = param.money_scale;
		pImp->_drop_rate = param.drop_rate;
		
		pImp->SetLifeTime(param.remain_time); 	//设两次寿命也无所谓了
		if(param.vis_id)
		{
			pNPC->vis_tid = param.vis_id;
		}

		if(param.mob_name_size)
		{
			ASSERT(param.mob_name_size <= sizeof(param.mob_name)); 
			memcpy(pNPC->npc_name, param.mob_name,param.mob_name_size);
			pNPC->name_size = param.mob_name_size;
			pNPC->object_state |= gactive_object::STATE_NPC_NAME;
		}

		pPlane->InsertNPC(pNPC);
		pImp->_runner->enter_world();
		pImp->OnCreate();
		pNPC->Unlock();
	}
}

void 
object_interface::CreateMinors(const  object_interface::minor_param & param,float radius)
{
	A3DVECTOR pos = _imp->_parent->pos;
	pos.x += abase::Rand(-radius,radius);
	pos.z += abase::Rand(-radius,radius);
	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height) pos.y = height;

	CreateMinors(pos,param);
}

void 
object_interface::CreateMonster(int id, int tag, A3DVECTOR & pos, int lifetime, int count, char owner_type, float radius)
{
	//副本中不用
	if(gmatrix::IsRaidServer())
	{
		return;
	}
	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;

	world_manager* manager = gmatrix::FindWorld(tag);
	if(!manager || manager->IsIdle())
	{
		return;
	}
	world *plane = manager->GetWorldByIndex(0);
	if(!manager || !plane)
	{
		ASSERT(false);
	}

	int owner_id1 = 0;
	int owner_id2 = 0;
	GenOwnerIDByType(owner_type, owner_id1, owner_id2);
	
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	for(int i = 0; i < count; ++i)
	{
		pos.x += abase::Rand(-radius,radius);
		pos.z += abase::Rand(-radius,radius);
		float height = manager->GetTerrain().GetHeightAt(pos.x,pos.z);
		if(pos.y < height) pos.y = height;

		gnpc * pNPC = npc_spawner::CreateMobBase(NULL,plane,ent,-1,0,pos,cid,abase::Rand(0,255),CLS_NPC_AI_POLICY_BASE,0,NULL,lifetime, 0, 0);
		if(pNPC)
		{
			pNPC->SetOwnerType(owner_type);
			pNPC->SetOwnerID(owner_id1, owner_id2);
			pNPC->SetObjectState(gactive_object::STATE_NPC_OWNER);
			gnpc_imp * pImp = (gnpc_imp*)pNPC->imp;

			pImp->SetDisappearLifeExpire(true);

			plane->InsertNPC(pNPC);
			pImp->_runner->enter_world();
			pImp->OnCreate();
			pNPC->Unlock();
		}
	}
}

void 
object_interface::CreateNPC(const  object_interface::minor_param & param,float radius)
{
	A3DVECTOR pos = _imp->_parent->pos;
	pos.x += abase::Rand(-radius,radius);
	pos.z += abase::Rand(-radius,radius);
	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height) pos.y = height;

	CreateNPC(pos,param);
}

void 
object_interface::CreateNPC(const A3DVECTOR & pos, const  object_interface::minor_param & param)
{
	npc_template * pTemplate = npc_stubs_manager::Get(param.mob_id);
	if(!pTemplate) return; 
	if(!pTemplate->npc_data) return;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = param.mob_id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;
	const int cid[3] = {CLS_SERVICE_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int aipolicy = 0;
	int aggro_policy = AGGRO_POLICY_BOSS;
	switch(pTemplate->npc_data->npc_type)
	{
		case npc_template::npc_statement::NPC_TYPE_GUARD:
			aipolicy = CLS_NPC_AI_POLICY_GUARD;
			break;
		case npc_template::npc_statement::NPC_TYPE_NORMAL:
			aipolicy = CLS_NPC_AI_POLICY_SERVICE;
			break;
		default:
			//普通的不做任何处理即可
			break;
	}
	gnpc * pNPC = npc_spawner::CreateMobBase(NULL,_imp->_plane,ent, -1,0, pos,cid, abase::Rand(0,255), aipolicy,aggro_policy, NULL, param.remain_time);
	
	if(pNPC)
	{
		ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(service_npc)));
		service_npc * pImp = (service_npc *)pNPC->imp;
		pImp->SetTaxRate(pTemplate->npc_data->tax_rate);
		pImp->SetNeedDomain(pTemplate->npc_data->need_domain);
		pImp->SetAttackRule(pTemplate->npc_data->attack_rule);
		pImp->SetTerritoryID(pTemplate->npc_data->id_territory);

		server_spawner::AddNPCServices(pImp, pTemplate);

		_imp->_plane->InsertNPC(pNPC);
		pImp->_runner->enter_world();
		pImp->OnCreate();
		pNPC->Unlock();
	}
}

void 
object_interface::CreateMobActiveDebug(const A3DVECTOR & pos, const  minor_param & param)
{
	npc_template * pTemplate = npc_stubs_manager::Get(param.mob_id);
	if(!pTemplate) return; 
	if(!pTemplate->mob_active_data) return;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = param.mob_id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;
	ent.path_id = 0;

	const int cid[3] = {CLS_MOBACTIVE_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int aipolicy = CLS_NPC_AI_POLICY_BASE;
	gnpc * pNPC = npc_spawner::CreateMobActiveBase(NULL,_imp->_plane,ent, -1, pos,cid, abase::Rand(0,255), aipolicy, 0, NULL, param.remain_time);
	
	if(pNPC)
	{
		mob_active_imp* pImp = (mob_active_imp *)pNPC->imp;
		mobactive_spawner::GenerateMobActiveParam(pImp,pTemplate);
		_imp->_plane->InsertNPC(pNPC);
		pImp->_runner->enter_world();
		pImp->OnCreate();
		pNPC->Unlock();
	}
}

void
object_interface::CreateProtectedNPC(int id, int lifetime, int taskid)
{
	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return; 
	if(!pTemplate->npc_data) return;

	A3DVECTOR pos = _imp->_parent->pos;
	pos.x += abase::Rand(-5,5);
	pos.z += abase::Rand(-5,5);
	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height) pos.y = height;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = id; 
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;
	const int cid[3] = {CLS_PROTECTED_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int aipolicy = 0;
	int aggro_policy = AGGRO_POLICY_BOSS;
	switch(pTemplate->npc_data->npc_type)
	{
		case npc_template::npc_statement::NPC_TYPE_GUARD:
			aipolicy = CLS_NPC_AI_POLICY_GUARD;
			break;
		case npc_template::npc_statement::NPC_TYPE_NORMAL:
			aipolicy = CLS_NPC_AI_POLICY_SERVICE;
			break;
		default:
			//普通的不做任何处理即可
			break;
	}

	gnpc * pNPC = npc_spawner::CreateMobBase(NULL,_imp->_plane,ent, -1, 0,pos,cid, abase::Rand(0,255), aipolicy,aggro_policy, NULL, lifetime); 
	if(pNPC)
	{
		ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(service_npc)));
		protected_npc * pImp = (protected_npc *)pNPC->imp;
		pImp->SetTaxRate(pTemplate->npc_data->tax_rate);
		pImp->SetNeedDomain(pTemplate->npc_data->need_domain);
		pImp->SetAttackRule(pTemplate->npc_data->attack_rule);
		pImp->SetTerritoryID(pTemplate->npc_data->id_territory);
		pImp->SetProtectedID(_imp->_parent->ID.id);
		pImp->SetTaskID(taskid);

		server_spawner::AddNPCServices(pImp, pTemplate);

		_imp->_plane->InsertNPC(pNPC);
		pImp->_runner->enter_world();
		pImp->OnCreate();
		pNPC->Unlock();
	}
}

void 
object_interface::CreateMine(const A3DVECTOR & pos , const mine_param & param)
{
	mine_template * pTemplate = mine_stubs_manager::Get(param.mine_id);
	if(!pTemplate) return;
	
	mine_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.mid = param.mine_id;
	ent.mine_count = 1;
	ent.reborn_time = 10000;

	gmatter * pMatter = mine_spawner::CreateMine(NULL,pos,_imp->_plane,0,ent,0,1,0);
	if(pMatter)
	{
		_imp->_plane->InsertMatter(pMatter);
		//pMatter->imp->_runner->enter_world();
		((gmatter_mine_imp*)pMatter->imp)->MineEnterWorld();
		pMatter->Unlock();
	}

}

void
object_interface::CreateMines(int mid, int count, int lifetime, float range)
{
	ASSERT(count > 0 && count < 100);
	ASSERT(range >= 0 && range <= 20);

	mine_template * pTemplate = mine_stubs_manager::Get(mid);
	if(!pTemplate) return;

	mine_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.mid = mid; 
	ent.mine_count = 1;

	A3DVECTOR pos = _imp->_parent->pos;

	for(int i = 0; i < count; ++i)
	{
		A3DVECTOR curPos = pos;
		curPos.x += abase::Rand(-range,range);
		curPos.z += abase::Rand(-range,range);
		float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(curPos.x,curPos.z);
		if(curPos.y < height) curPos.y = height;
		gmatter * pMatter = mine_spawner::CreateMine(NULL,curPos,_imp->_plane,0,ent,0,1,0,lifetime);
		if(pMatter)
		{
			_imp->_plane->InsertMatter(pMatter);
			pMatter->imp->_runner->enter_world();
			pMatter->Unlock();
		}
	}
}


void 
object_interface::CreateMines(int mid, int tag, A3DVECTOR & pos, int lifetime, int count, char owner_type, float radius)
{
	//副本中不用
	if(gmatrix::IsRaidServer())
	{
		return;
	}

	mine_template * pTemplate = mine_stubs_manager::Get(mid);
	if(!pTemplate) return;

	mine_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.mid = mid; 
	ent.mine_count = 1;

	world_manager *manager = gmatrix::FindWorld(tag);
	if(!manager || manager->IsIdle())
	{
		return;
	}
	world *plane = manager->GetWorldByIndex(0);
	if(!manager || !plane)
	{
		ASSERT(false);
	}

	int owner_id1 = 0;
	int owner_id2 = 0;
	GenOwnerIDByType(owner_type, owner_id1, owner_id2);
	for(int i = 0; i < count; ++i)
	{
		A3DVECTOR curPos = pos;
		curPos.x += abase::Rand(-radius,radius);
		curPos.z += abase::Rand(-radius,radius);
		float height = manager->GetTerrain().GetHeightAt(curPos.x,curPos.z);
		if(curPos.y < height) curPos.y = height;

		gmatter * pMatter = mine_spawner::CreateMine(NULL,curPos,plane,0,ent,0,1,0,lifetime);
		if(pMatter)
		{
			pMatter->SetOwnerType(owner_type);
			pMatter->SetOwnerID(owner_id1, owner_id2);
			pMatter->matter_state |= gmatter::STATE_MASK_OWNER_MATTER;

			plane->InsertMatter(pMatter);
			pMatter->imp->_runner->enter_world();
			pMatter->Unlock();
		}
	}
}

void
object_interface::GenOwnerIDByType(char type, int & owner_id1, int & owner_id2)
{
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	switch(type)
	{
		case gowner::OWNER_SELF:
		{
			owner_id1 = pImp->GetParent()->ID.id; 
		}
		break; 

		case gowner::OWNER_MASTER:
		{
			owner_id1 = (pImp->GetSectID() == pImp->GetParent()->ID.id ? 0 : pImp->GetSectID());
		}
		break;

		case gowner::OWNER_APPRENTICE:
		{
			owner_id2 = pImp->GetParent()->ID.id; 
		}	
		break;

		case gowner::OWNER_SPOUSE:
		{
			owner_id1 = pImp->GetSpouse();
		}
		break;

		case gowner::OWNER_TEAM:
		case gowner::OWNER_TEAM_OR_SELF:
		case gowner::OWNER_TEAM_AND_SELF:
		{
			owner_id1 = pImp->GetParent()->ID.id;
			if(pImp->IsInTeam())
			{
				owner_id2 = pImp->GetTeamID();
			}
			else
			{
				owner_id2 = 0;
			}
		}
		break;

		case gowner::OWNER_FAMILY:
		{
			owner_id2 = _imp->OI_GetFamilyID();
		}
		break;
		
		case gowner::OWNER_MAFIA:
		{
			owner_id2 = _imp->OI_GetMafiaID();
		}
		break;
	}
}

bool 
object_interface::TestCoolDown(unsigned short id)
{
	return _imp->CheckCoolDown((int)id);
}

void 
object_interface::SetCoolDown(unsigned short id, int ms)
{
	return _imp->SetCoolDown((int)id,ms);
}

void 
object_interface::ClrCoolDown(unsigned short id)
{
	return _imp->ClrCoolDown((int)id);
}

void 
object_interface::ReturnToTown()
{
	_imp->ObjReturnToTown();
}

bool
object_interface::CanReturnToTown()
{
	return true;
}

void 
object_interface::AddPlayerEffect(short effect)
{
	_imp->AddEffectData(effect);
}

void 
object_interface::RemovePlayerEffect(short effect)
{
	_imp->RemoveEffectData(effect);
	_imp->_runner->player_disable_effect(effect);
}

int 
object_interface::GetImmuneMask()
{
	return _imp->_immune_state | _imp->_immune_state_adj;
}

void 
object_interface::SetImmuneMask(int mask)
{
	_imp->_immune_state_adj |= mask;
}
	
void 
object_interface::ClearImmuneMask(int mask)
{
	_imp->_immune_state_adj &= ~mask;
}

void 
object_interface::SkillMove(const A3DVECTOR & pos)
{
	world_manager * pManager = _imp->GetWorldManager();
	if(!pManager) return;
	float h = pManager->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(h > pos.y + 0.2) return ;
	//$$$$$$ 碰撞检测做了没有...


	A3DVECTOR offset = pos;
	offset -= _imp->_parent->pos;
	_imp->StepMove(offset);
	_imp->ClearNextSession();
	int seq = _imp->_commander->GetCurMoveSeq() + 200;
	_imp->_runner->change_move_seq(seq & 0xFFFF);
	_imp->_commander->SetNextMoveSeq(seq);
}

int 
object_interface::CalcPhysicDamage(int raw_damage, int attacker_level)
{
	float reduce = player_template::GetDamageReduce(_imp->_cur_prop.defense,attacker_level);
	return (int)(raw_damage * (1 - reduce));
}

int 
object_interface::CalcMagicDamage(int dmg_class,int raw_damage, int attacker_level)
{
	float reduce = player_template::GetDamageReduce(_imp->_cur_prop.resistance[dmg_class],attacker_level);
	return (int)(raw_damage * (1 - reduce));
}

float 
object_interface::CalcLevelDamagePunish(int atk_level , int def_level)
{
	float adj = 1.0f;
	int delta = atk_level - def_level;
	player_template::GetAttackLevelPunishment(delta,adj);
	return adj;
}

bool 
object_interface::IsPlayerClass()
{
	return _imp->IsPlayerClass();
}

int
object_interface::GetObjectType()
{
	return _imp->GetObjectType();
}

void 
object_interface::BreakCurAction()
{
	_imp->TryStopCurSession();
}

bool object_interface::CreatePet(const A3DVECTOR& pos,const pet_data* pData,const pet_bedge_essence* pEss,int tid,size_t pet_index,XID& who, int reborn_cnt, void * cd_buf, size_t cd_len)
{
	const int cid[3] = {CLS_PET_IMP,CLS_PET_DISPATCHER,CLS_NPC_CONTROLLER};
	unsigned char dir = _imp->_parent->dir;
	gnpc* pNPC = npc_spawner::CreatePetBase((gplayer_imp*)_imp,pos,pData,pEss,cid,dir,CLS_NPC_AI_POLICY_PET,AGGRO_POLICY_PET,tid);
	if(pNPC)
	{
		gpet_imp* pImp = (gpet_imp*)pNPC->imp;
		pImp->_leader_id = _imp->_parent->ID;
		pImp->SetHonorLevel(pet_manager::GetHonorLevel(pEss->GetCurHonorPoint()));
		pImp->SetHungerLevel(pet_manager::GetHungerLevel(pEss->GetCurHungerPoint(),pEss->GetMaxHungerPoint()));
		pImp->SetPetStamp(pData->summon_stamp);
		pImp->SetPetIndex(pet_index);
		pImp->SetAggroState(gpet_imp::PET_AGGRO_AUTO);
		pImp->SetStayState(0);
		pImp->_money_scale = 0;
		pImp->_drop_rate = 0;	
		pImp->SetRebornCnt(reborn_cnt);
		pImp->SetDieWithLeader(true);
		pNPC->master_id = _imp->_parent->ID.id;
		pNPC->pet_star = pEss->GetStar();
		pNPC->pet_shape = pEss->GetShape();
		pNPC->pet_face = pEss->GetFace();
		pNPC->SetObjectState(gactive_object::STATE_NPC_PET);
		pImp->InitFromMaster((gplayer_imp*)_imp);
		pImp->LoadCoolDown(cd_buf, cd_len);
		
		size_t name_size;
		const void* name_buf = pEss->GetName(name_size);
		if(name_size)
		{
			pNPC->name_size = name_size;
			memcpy(pNPC->npc_name,name_buf,pNPC->name_size);
			pNPC->SetObjectState(gactive_object::STATE_NPC_NAME);
		}
		_imp->_plane->InsertNPC(pNPC);
		pImp->_runner->enter_world();
		//这里返回生成的npc id
		who = pNPC->ID;
		pNPC->Unlock(); //???
		return true;
	}
	return false;
}

void object_interface::CreateMinors(float dist, float angle, const minor_param& p)
{
	A3DVECTOR pos = GetPos();
	unsigned char dir = GetDirection();
	const static float deg = 3.1415926f / 180;
	float distAngle = (dir * 1.0f / 256 * 360 - angle) * deg;
	pos.x += (cosf(distAngle) * dist);
	pos.z += (sinf(distAngle) * dist);
	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height) pos.y = height;
	CreateMinors(pos, p);
}

bool object_interface::CreateSkillObject(int id, int count, int lifetime, int skill_level, float dist, float angle, int move_state)
{
	A3DVECTOR pos = GetPos();
	unsigned char dir = GetDirection();
	const static float deg = 3.1415926f / 180;
	float distAngle = (dir * 1.0f / 256 * 360 - angle) * deg;
	pos.x += (cosf(distAngle) * dist);
	pos.z += (sinf(distAngle) * dist);
	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height) pos.y = height;

	return CreateSkillObject(id, count, lifetime, skill_level, pos, move_state);
}

bool object_interface::CreateSkillObject(int id, int count, int lifetime, int skill_level, A3DVECTOR& pos, int move_state)
{
	ASSERT(lifetime > 0 && lifetime <= summon_manager::MAX_SUMMON_LIFETIME);

	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return false;
	if(IsFlying()) return false;

	for(int i = 0; i < count; ++i)
	{
		unsigned char dir = GetDirection();
		float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x,pos.z);
		if(pos.y < height) pos.y = height;

		summon_param param;
		memset(&param, 0, sizeof(param));

		param.id = id;
		param.type = summon_manager::SUMMON_TYPE_SKILLOBJECT;
		param.index = i+1;
		param.pos = pos;
		param.dir = dir;
		param.lifetime  = lifetime;
		param.skill_level = skill_level;
		param.exchange = false;
		param.move_type = move_state;
		param.aggro_type = gsummon_imp::AGGRO_STATE_DEFENSE;

		if(!SummonMonster(param)) return false;
	}
	return true;
}

//创建动物和植物
bool object_interface::CreateSummon(int id, int count, int lifetime, char type, int skill_level, const XID & target, bool use_target_pos) 
{
	ASSERT(lifetime > 0 && lifetime <= summon_manager::MAX_SUMMON_LIFETIME);

	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return false;
	if(IsFlying()) return false;

	gplayer_imp *pImp = (gplayer_imp*)_imp;
	pImp->GetSummonMan().PreSummonNPC(pImp, type, id, count, 0);

	if(use_target_pos && target.id != 0)
	{
		if(target.id == pImp->GetParent()->ID.id)
		{
			use_target_pos = false;
		}	
		else if(pImp->IsInTeam() && pImp->OI_IsMember(target))
		{
			use_target_pos = false;
		}	
	}

	A3DVECTOR targetPos;
	if(target.id != 0 && use_target_pos)
	{

		float bodysize = 0.0f;
		use_target_pos = QueryObject(target, targetPos, bodysize);
		if(_imp->_parent->pos.squared_distance(targetPos) > 30 * 30)
		{
			use_target_pos = false;
		}
	}
	else
	{
		use_target_pos = false;
	}


	for(int i = 0; i < count; ++i)
	{
		summon_param param;
		memset(&param, 0, sizeof(param));

		A3DVECTOR pos = _imp->_parent->pos;
		unsigned char dir = _imp->_parent->dir; 
		//动物
		if(type == summon_manager::SUMMON_TYPE_ANIMAL)
		{
			float radius = 5.0;
			pos.x += abase::Rand(-radius,radius);
			pos.z += abase::Rand(-radius,radius);

			param.move_type = gsummon_imp::MOVE_STATE_FOLLOW; 
			param.aggro_type = gsummon_imp::AGGRO_STATE_DEFENSE;
		}
		//植物
		else if(type == summon_manager::SUMMON_TYPE_PLANT)
		{
			pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
			pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

			param.move_type = gsummon_imp::MOVE_STATE_STAY; 
			param.aggro_type = gsummon_imp::AGGRO_STATE_OFFENSE;
		}

		param.id = id;
		param.type = type;
		param.index = i+1;
		if(use_target_pos) 
		{
			param.pos = targetPos;
		}
		else 
		{
			param.pos = pos;
		}
		param.dir = dir;
		param.lifetime  = lifetime;
		param.skill_level = skill_level;

		if(!SummonMonster(param)) return false;

		if(use_target_pos) 
		{
			pImp->GetSummonMan().NotifyStartAttack(pImp, target, 0);  
		}
	}
	return true;
}

//创建分身
bool object_interface::CreateClone(int id, int count, int lifetime, char type, int skill_level, bool exchange) 
{
	ASSERT(lifetime > 0 && lifetime <= summon_manager::MAX_SUMMON_LIFETIME);
	
	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return false;
	if(IsFlying()) return false;
	
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	for(int i = 0; i < count; ++i)
	{
		summon_param param;
		memset(&param, 0, sizeof(param));

		A3DVECTOR pos = _imp->_parent->pos;
		unsigned char dir = _imp->_parent->dir; 
		param.is_clone = true;
		param.move_type = gsummon_imp::MOVE_STATE_FOLLOW; 
		param.aggro_type = gsummon_imp::AGGRO_STATE_DEFENSE;

		pImp->GetSummonMan().PreSummonNPC(pImp, type, id, count, 0);

		param.id = id;
		param.type = summon_manager::SUMMON_TYPE_CLONE;
		param.index = i+1;
		param.pos = pos;
		param.dir = dir;
		param.lifetime  = lifetime;
		param.skill_level = skill_level;
		param.exchange = exchange;

		if(!SummonMonster(param)) return false;
	}
	return true;
}

//创建机关,陷阱
bool object_interface::CreateTrap(int id, int lifetime, char type, int skill_level, bool is_invisible, int max_count) 
{
	ASSERT(lifetime > 0 && lifetime <= summon_manager::MAX_SUMMON_LIFETIME);

	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return false;
	if(IsFlying()) return false;

	gplayer_imp *pImp = (gplayer_imp*)_imp;
	summon_param param;
	memset(&param, 0, sizeof(param));

	A3DVECTOR pos = _imp->_parent->pos;
	unsigned char dir = _imp->_parent->dir; 
	if(type == summon_manager::SUMMON_TYPE_MACHINE)
	{
		pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
		pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

		param.move_type = gsummon_imp::MOVE_STATE_STAY; 
		param.aggro_type = gsummon_imp::AGGRO_STATE_OFFENSE;
	}
	//陷阱
	else if(type == summon_manager::SUMMON_TYPE_TRAP)
	{
		pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
		pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

		param.move_type = gsummon_imp::MOVE_STATE_STAY; 
		param.aggro_type = gsummon_imp::AGGRO_STATE_OFFENSE;
	}
	//遥控陷阱
	else if(type == summon_manager::SUMMON_TYPE_CONTROL_TRAP)
	{
		pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
		pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

		param.move_type = gsummon_imp::MOVE_STATE_STAY; 
		param.aggro_type = gsummon_imp::AGGRO_STATE_OFFENSE;
	}
	pImp->GetSummonMan().PreSummonNPC(pImp, type, id, 1, max_count);

	param.id = id;
	param.type = type;
	param.index = 1;
	param.pos = pos;
	param.dir = dir;
	param.lifetime  = lifetime;
	param.skill_level = skill_level;
	param.is_invisible = is_invisible;

	return SummonMonster(param);
}

//传送怪物
bool object_interface::CreateTelePort(int id, int lifetime, char type, int skill_level, bool is_invisible, int skill_id, int skill_cd, int teleport_count, int teleport_prob) 
{
	ASSERT(lifetime > 0 && lifetime <= summon_manager::MAX_SUMMON_LIFETIME);

	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return false;
	if(IsFlying()) return false;

	gplayer_imp *pImp = (gplayer_imp*)_imp;
	summon_param param;
	memset(&param, 0, sizeof(param));

	A3DVECTOR pos = _imp->_parent->pos;
	unsigned char dir = _imp->_parent->dir; 
	//祝福传送
	if(type == summon_manager::SUMMON_TYPE_TELEPORT1)
	{
		pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
		pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

		param.move_type = gsummon_imp::MOVE_STATE_FIX; 
		param.aggro_type = gsummon_imp::AGGRO_STATE_PEACE;
	}
	//诅咒传送
	else if(type == summon_manager::SUMMON_TYPE_TELEPORT2)
	{
		pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
		pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

		param.move_type = gsummon_imp::MOVE_STATE_FIX; 
		param.aggro_type = gsummon_imp::AGGRO_STATE_PEACE;
	}
	pImp->GetSummonMan().PreSummonNPC(pImp, type, id, 1, 2);

	param.id = id;
	param.type = type;
	param.index = 1;
	param.pos = pos;
	param.dir = dir;
	param.lifetime  = lifetime;
	param.skill_level = skill_level;
	param.is_invisible = is_invisible;
	param.skill_id = skill_id;
	param.skill_cd = skill_cd;
	param.teleport_count = teleport_count;
	param.teleport_prob = teleport_prob;

	return SummonMonster(param);
}

//指向性召唤
bool object_interface::CreatePosSummon(int id, int count, int lifetime, char type, int skill_level, const XID & target) 
{
	ASSERT(lifetime > 0 && lifetime <= summon_manager::MAX_SUMMON_LIFETIME);
	if(count <= 0) return false;
	
	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) return false;
	if(IsFlying()) return false;

	A3DVECTOR targetPos;
	bool use_target_pos = false;
	if(target.id != 0)
	{
		float bodysize = 0.0f;
		use_target_pos = QueryObject(target, targetPos, bodysize);

		float radius = 5.0;
		targetPos.x += abase::Rand(-radius,radius);
		targetPos.z += abase::Rand(-radius,radius);
	}
	
	gplayer_imp *pImp = (gplayer_imp*)_imp;
//	pImp->GetSummonMan().PreSummonNPC(pImp, type, id, count, 0);
	for(int i = 0; i < count; ++i)
	{
		summon_param param;
		memset(&param, 0, sizeof(param));

		A3DVECTOR pos = _imp->_parent->pos;
		unsigned char dir = _imp->_parent->dir; 

		//动物
		if(type == summon_manager::SUMMON_TYPE_ANIMAL)
		{
			float radius = 5.0;
			pos.x += abase::Rand(-radius,radius);
			pos.z += abase::Rand(-radius,radius);

			param.move_type = gsummon_imp::MOVE_STATE_FOLLOW; 
			param.aggro_type = gsummon_imp::AGGRO_STATE_DEFENSE;
		}
		//植物
		else if(type == summon_manager::SUMMON_TYPE_PLANT)
		{
			pos.x += 0.4*cos((float)(dir)*3.1415*2.0/(float)255);
			pos.z += 0.4*sin((float)(dir)*3.1415*2.0/(float)255);

			param.move_type = gsummon_imp::MOVE_STATE_STAY; 
			param.aggro_type = gsummon_imp::AGGRO_STATE_OFFENSE;
		}

		param.id = id;
		param.type = type;
		param.index = i+1;
		if(use_target_pos) 
		{
			param.pos = targetPos;
		}
		else 
		{
			param.pos = pos;
		}
		param.dir = dir;
		param.lifetime  = lifetime;
		param.skill_level = skill_level;

		if(!SummonMonster(param)) return false;
		pImp->GetSummonMan().NotifyStartAttack(pImp, target, 0);  
		//去除隐遁buff
		if(pImp->_filters.IsFilterExist(FILTER_SHADOWHIDE))
		{
			pImp->_filters.RemoveFilter(FILTER_SHADOWHIDE);
		}
	}
	return true;
}

bool object_interface::SummonMonster(summon_param & param)
{
	//让前面召唤出来的消失掉
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	const int cid[3] = {CLS_SUMMON_IMP,CLS_SUMMON_DISPATCHER,CLS_NPC_CONTROLLER};

	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(param.pos.x,param.pos.z);
	if(param.pos.y < height) param.pos.y = height;

	gnpc *pNPC = npc_spawner::CreateSummonBase((gplayer_imp*)_imp,param.pos, cid, param.dir, CLS_NPC_AI_POLICY_SUMMON, AGGRO_POLICY_SUMMON, param.id, param.lifetime, param.is_clone);
	if(pNPC)
	{
		gsummon_imp *sImp = (gsummon_imp*)pNPC->imp;
		//利用脚本初始化怪物的数据
		if(!summon_manager::InitFromCaster(sImp, pImp, param.id, param.skill_level, param.index))
		{
			sImp->_commander->Release();	
			return false;
		}

		sImp->_leader_id = _imp->_parent->ID;
		sImp->SetDieWithLeader(true);
		sImp->_money_scale = 0;
		sImp->_drop_rate = 0;	
		sImp->_cur_prop = sImp->_base_prop;
		sImp->InitFromMaster((gplayer_imp*)_imp);
		sImp->SetSummonType(param.type);
		sImp->SetAggroState(param.aggro_type);
		sImp->SetMoveState(param.move_type);

		int timestamp = g_timer.get_systime();
		sImp->SetSummonStamp(timestamp);

		pNPC->master_id = _imp->_parent->ID.id;
		pNPC->SetObjectState(gactive_object::STATE_NPC_SUMMON);

		pImp->GetSummonMan().InsertSummonNPC(pNPC->ID.id, param.type, param.id, timestamp, param.is_invisible, param.pos, _imp->GetWorldTag());

		if(param.type == summon_manager::SUMMON_TYPE_ANIMAL)
		{
			sImp->_cur_item.attack_cycle = SECOND_TO_TICK(1.0f);
			sImp->_cur_item.attack_point = SECOND_TO_TICK(0.5f);
		}
		//分身的召唤兽
		else if(param.type == summon_manager::SUMMON_TYPE_CLONE)
		{
			sImp->SetCloneState(1);
			pNPC->SetObjectState(gactive_object::STATE_NPC_CLONE);
			pImp->SetCloneInfo(sImp, param.exchange);
			sImp->_cur_item.attack_cycle = SECOND_TO_TICK(1.0f);
			sImp->_cur_item.attack_point = SECOND_TO_TICK(0.5f);
		}
		else if(param.type == summon_manager::SUMMON_TYPE_CONTROL_TRAP)
		{
		}
		else if(param.type == summon_manager::SUMMON_TYPE_TELEPORT1)
		{
			pNPC->SetObjectState(gactive_object::STATE_NPC_TELEPORT1);
			sImp->SetTelePortInfo(param.teleport_count, param.teleport_prob);
			sImp->SetLifeTime(180);//初始3分钟寿命 

			if(pImp->GetSummonMan().GetSummonNPCCountByID(param.id) == 1)
			{
				pNPC->teleport_count = 1;
			}
			else if(pImp->GetSummonMan().GetSummonNPCCountByID(param.id) == 2)
			{
				pNPC->teleport_count = 2;
			}
		}
		else if(param.type == summon_manager::SUMMON_TYPE_TELEPORT2)
		{
			pNPC->SetObjectState(gactive_object::STATE_NPC_TELEPORT2);
			sImp->SetTelePortInfo(param.teleport_count, param.teleport_prob);
			sImp->SetLifeTime(180);	//初始3分钟寿命 

			if(pImp->GetSummonMan().GetSummonNPCCountByID(param.id) == 1)
			{
				pNPC->teleport_count = 1;
			}
			else if(pImp->GetSummonMan().GetSummonNPCCountByID(param.id) == 2)
			{
				pNPC->teleport_count = 2;
			}
		}
			
		if(param.is_invisible)
		{
			pNPC->SetObjectState(gactive_object::STATE_NPC_INVISIBLE);
			sImp->SetSummonInvisible();
		}

		_imp->_plane->InsertNPC(pNPC);
		sImp->_runner->enter_world();
		sImp->OnCreate();

		pImp->GetSummonMan().PostSummonNPC(pImp, param.type, pNPC->ID.id, param.id, param.lifetime, param.skill_id, param.skill_cd, param.pos);
		pNPC->Unlock();
	}
	return true;
}


void object_interface::UnSummonMonster(char type)
{
	_imp->OI_UnSummonMonster(type); 
}

int object_interface::GetLinkIndex()
{
	return _imp->GetLinkIndex();
}

int object_interface::GetLinkSID()
{
	return _imp->GetLinkSID();
}

size_t object_interface::GetMallOrdersCount()
{
	return _imp->OI_GetMallOrdersCount();
}

int object_interface::GetMallOrders(GDB::shoplog * list, size_t size)
{
	return _imp->OI_GetMallOrders(list, size);
}


size_t object_interface::GetInventorySize()
{
	return _imp->OI_GetInventorySize();
}

int object_interface::GetInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _imp->OI_GetInventoryDetail(list,size);
}

size_t object_interface::GetPetBedgeInventorySize()
{
	return _imp->OI_GetPetBedgeInventorySize();
}

int object_interface::GetPetBedgeInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _imp->OI_GetPetBedgeInventoryDetail(list,size);
}

size_t object_interface::GetPetEquipInventorySize()
{
	return _imp->OI_GetPetEquipInventorySize();
}

int object_interface::GetPetEquipInventoryDetail(GDB::itemdata * list, size_t size)
{
	return _imp->OI_GetPetEquipInventoryDetail(list,size);
}

size_t object_interface::GetPocketInventorySize()
{
	return _imp->OI_GetPocketInventorySize();
}

int object_interface::GetPocketInventoryDetail(GDB::pocket_item* list, size_t size)
{
	return _imp->OI_GetPocketInventoryDetail(list, size);
}

size_t object_interface::GetFashionInventorySize()
{
	return _imp->OI_GetFashionInventorySize();
}

int object_interface::GetFashionInventoryDetail(GDB::itemdata* list, size_t size)
{
	return _imp->OI_GetFashionInventoryDetail(list, size);
}

size_t object_interface::GetMountWingInventorySize()
{
	return _imp->OI_GetMountWingInventorySize();
}

int object_interface::GetMountWingInventoryDetail(GDB::itemdata* list, size_t size)
{
	return _imp->OI_GetMountWingInventoryDetail(list, size);
}

size_t object_interface::GetGiftInventorySize()
{
	return _imp->OI_GetGiftInventorySize();
}

int object_interface::GetGiftInventoryDetail(GDB::itemdata* list, size_t size)
{
	return _imp->OI_GetGiftInventoryDetail(list, size);
}

size_t object_interface::GetFuwenInventorySize()
{
	return _imp->OI_GetFuwenInventorySize();
}

int object_interface::GetFuwenInventoryDetail(GDB::itemdata* list, size_t size)
{
	return _imp->OI_GetFuwenInventoryDetail(list, size);
}

int object_interface::GetTrashBoxDetail(GDB::itemdata * list, size_t size)
{
	return _imp->OI_GetTrashBoxDetail(list,size);
}

int object_interface::GetMafiaTrashBoxDetail(GDB::itemdata * list, size_t size)
{
	return _imp->OI_GetMafiaTrashBoxDetail(list,size);
}

int object_interface::GetEquipmentDetail(GDB::itemdata * list, size_t size)
{
	return _imp->OI_GetEquipmentDetail(list,size);
}

size_t object_interface::GetEquipmentSize()
{
	return _imp->OI_GetEquipmentSize();
}

size_t object_interface::GetTrashBoxCapacity()
{
	return _imp->OI_GetTrashBoxCapacity();
}

size_t object_interface::GetMafiaTrashBoxCapacity()
{
	return _imp->OI_GetMafiaTrashBoxCapacity();
}

size_t object_interface::GetTrashBoxMoney()
{
	return _imp->OI_GetTrashBoxMoney();
}

bool object_interface::IsTrashBoxModified()
{
	return _imp->OI_IsTrashBoxModified();
}

bool object_interface::IsEquipmentModified()
{
	return _imp->OI_IsEquipmentModified();
}

int object_interface::TradeLockPlayer(int get_mask, int put_mask)
{
	return _imp->TradeLockPlayer(get_mask, put_mask);
}

int object_interface::TradeUnLockPlayer()
{
	return _imp->TradeUnLockPlayer();
}


void 
object_interface::SendClientAttackData()
{
	_imp->_runner->get_extprop_attack();
}

void 
object_interface::SendClientDefenseData()
{
	_imp->_runner->get_extprop_defense();
}

void 
object_interface::DuelStart(const XID & target)
{
	_imp->OnDuelStart(target);
}

void 
object_interface::DuelStop()
{
	_imp->OnDuelStop();
}

int
object_interface::GetDuelTargetID()
{
	return _imp->GetDuelTargetID();
}

void 
object_interface::SendClientDuelStart(const XID & target)
{
	_imp->_runner->duel_start(target);
	__PRINTF("决斗开始\n");
}

void 
object_interface::SendClientDuelStop(const XID & target)
{
	_imp->_runner->duel_stop();
	__PRINTF("决斗结束\n");
}

void object_interface::SendClientSkillAddon(int skill, int addon)
{
	_imp->_runner->player_skill_addon(skill, addon);
}

void object_interface::SendClientSkillCommonAddon(int addon)
{
	_imp->_runner->player_skill_common_addon(addon);
}

void object_interface::SendClientExtraSkill(int skill, int level)
{
	_imp->_runner->player_extra_skill(skill, level);
}

void
object_interface::SendClientLearnSkill(int id, int level)
{
	_imp->_runner->learn_skill(id, level);
}

void
object_interface::SendClientSkillProficiency(int id, int proficiency)
{
	_imp->_runner->skill_proficiency(id, proficiency);
}

void 
object_interface::ActiveMountState(int mount_id, int mount_lvl, bool no_notify, char mount_type)
{
	_imp->ActiveMountState(mount_id, mount_lvl, no_notify, mount_type);
}

void 
object_interface::DeactiveMountState()
{
	_imp->DeactiveMountState();
}

bool 
object_interface::CheckGMPrivilege()
{
	return _imp->CheckGMPrivilege();
}

int 
object_interface::GetDBTimeStamp()
{
	return _imp->OI_GetDBTimeStamp();
}

int 
object_interface::InceaseDBTimeStamp()
{
	return _imp->OI_InceaseDBTimeStamp();
}
	
void 
object_interface::EnhanceCrit(int val)
{
	_imp->_crit_rate += val;
	property_policy::UpdateCrit(_imp);
}


void 
object_interface::ImpairCrit(int val)
{
	_imp->_crit_rate -= val;
	property_policy::UpdateCrit(_imp);
}

void object_interface::EnhanceCritDmg(float val)
{
	_imp->_en_point.crit_damage += val;
	property_policy::UpdateCrit(_imp);
}

void object_interface::ImpairCritDmg(float val)
{
	_imp->_en_point.crit_damage -= val;
	property_policy::UpdateCrit(_imp);
}

void object_interface::EnhanceAntiTransform(int val)
{
	_imp->_en_point.anti_transform += val;
	property_policy::UpdateTransform(_imp);
}

void object_interface::ImpairAntiTransform(int val)
{
	_imp->_en_point.anti_transform -= val;
	property_policy::UpdateTransform(_imp);
}

void object_interface::EnhanceScaleAntiTransform(int val)
{
	_imp->_en_percent.anti_transform += val;
	property_policy::UpdateTransform(_imp);
}

void object_interface::ImpairScaleAntiTransform(int val)
{
	_imp->_en_percent.anti_transform -= val;
	property_policy::UpdateTransform(_imp);
}

void object_interface::EnhanceAntiDiet(int val)
{
	_imp->_en_point.anti_diet += val;
	property_policy::UpdateMagic(_imp);
}

void object_interface::ImpairAntiDiet(int val)
{
	_imp->_en_point.anti_diet -= val;
	property_policy::UpdateMagic(_imp);
}

bool 
object_interface::CheckWaypoint(int point_index, int & point_domain)
{
	return _imp->CheckWaypoint(point_index, point_domain);
}

void object_interface::ReturnWaypoint(int point)
{
	_imp->ReturnWaypoint(point);
}

int object_interface::GetCityOwner(int city_id)
{
	return GMSV::GetCityOwner(city_id);
}

bool
object_interface::ModifySkillPoint(int offset)
{
	if(_imp->_basic.status_point + offset < 0) return false;
	_imp->_basic.status_point += offset;
	_imp->_runner->self_get_property(_imp->_basic.status_point,_imp->_cur_prop);
	return true;
}

bool
object_interface::ModifyTalentPoint(int offset)
{
	return _imp->ModifyTalentPoint(offset);
}

bool 
object_interface::GetMallInfo(int & cash_used, int & cash, int &cash_delta,  int &order_id)
{
	return _imp->OI_GetMallInfo(cash_used, cash,cash_delta, order_id);
}

bool 
object_interface::IsCashModified()
{
	return _imp->OI_IsCashModified();
}

unsigned char 
object_interface::GetAttackStamp()
{
	return _imp->GetAttackStamp();
}

void 
object_interface::IncAttackStamp()
{
	return _imp->IncAttackStamp();
}

void 
object_interface::SetOverSpeed(float sp)
{
	_imp->_en_point.over_speed_square = sp*sp;
}

void 
object_interface::UpdateAllProp()
{
	_imp->OI_UpdateAllProp();
}

void object_interface::Die()
{
	_imp->OI_Die();
}

void
object_interface::Disappear()
{
	_imp->OI_Disappear();
	
}

void
object_interface::EnterSanctuary()
{
	_imp->OI_ToggleSanctuary(true);
}


void
object_interface::LeaveSanctuary()
{
	_imp->OI_ToggleSanctuary(false);
}

void 
object_interface::TalismanGainExp(int exp, bool is_aircraft)
{
	_imp->OI_TalismanGainExp(exp, is_aircraft);
}


void 
object_interface::ReceiveTaskExp(int exp)
{
	_imp->OI_ReceiveTaskExp(exp);
}


bool object_interface::TestSafeLock()
{
	return _imp->OI_TestSafeLock();
}

void 
object_interface::MagicGainExp(int exp)
{
	_imp->OI_MagicGainExp(exp);
}

int object_interface::RebornCount()
{
	return _imp->OI_RebornCount();
}

void object_interface::Teleport(int tag, const A3DVECTOR & pos)
{
	//liuyue-facbase
	if(tag > 0 && (_imp->GetWorldManager()->IsRaidWorld() || _imp->GetWorldManager()->IsFacBase()))
	{
		return;
	}

	if(tag == 0)
	{
		tag = _imp->GetWorldManager()->GetWorldTag();
	}
	_imp->LongJump(pos,tag);
}

int object_interface::SpendTalismanStamina(float cost)
{
	return _imp->OI_SpendTalismanStamina(cost);
}

unsigned int 
object_interface::GetDBMagicNumber()
{
	return _imp->GetDBMagicNumber();
}

int
object_interface::CreateItem(int item_id,int count,int period)
{
	return _imp->SkillCreateItem(item_id,count, period);
}

void 
object_interface::EnhanceSkillAttack(int skill_attack)
{
	_imp->_en_point.skill_attack_rate += skill_attack;
}

void 
object_interface::ImpairSkillAttack(int skill_attack)
{
	_imp->_en_point.skill_attack_rate -= skill_attack;
}

void 
object_interface::EnhanceSkillArmor(int skill_armor)
{
	_imp->_en_point.skill_armor_rate += skill_armor;
}

void 
object_interface::ImpairSkillArmor(int skill_armor)
{
	_imp->_en_point.skill_armor_rate -= skill_armor;
}

void 
object_interface::EnhanceAntiCritRate(int crit_rate)
{
	_imp->_en_point.anti_crit += crit_rate;
}

void 
object_interface::ImpairAntiCritRate(int crit_rate)
{
	_imp->_en_point.anti_crit -= crit_rate;
}


void 
object_interface::EnhanceAntiCritDamage(float crit_damage)
{
	_imp->_en_point.anti_crit_damage += crit_damage;
}

void 
object_interface::ImpairAntiCritDamage(float crit_damage)
{
	_imp->_en_point.anti_crit_damage -= crit_damage;
}

void
object_interface::EnhanceDeityPower(int power)
{
	_imp->_en_point.deity_power += power;
}

void
object_interface::ImpairDeityPower(int power)
{
	_imp->_en_point.deity_power -= power;
}


void
object_interface::EnhanceScaleDeityPower(int power)
{
	_imp->_en_percent.deity_power += power;
}


void 
object_interface::EnhanceCultDefense(int index, int cult_defense)
{
	ASSERT(index >= 0 && index < CULT_DEF_NUMBER);
	_imp->_en_point.cult_defense[index] += cult_defense;
}

void 
object_interface::ImpairCultDefense(int index, int cult_defense)
{
	ASSERT(index >= 0 && index < CULT_DEF_NUMBER);
	_imp->_en_point.cult_defense[index] -= cult_defense;
}

void 
object_interface::EnhanceCultAttack(int index, int cult_attack)
{
	ASSERT(index >= 0 && index < CULT_DEF_NUMBER);
	_imp->_en_point.cult_attack[index] += cult_attack;
}

void 
object_interface::ImpairCultAttack(int index, int cult_attack)
{
	ASSERT(index >= 0 && index < CULT_DEF_NUMBER);
	_imp->_en_point.cult_attack[index] -= cult_attack;
}


void 
object_interface::EnhancePetDamage(int adjust)
{
	_imp->_en_point.pet_attack_adjust += adjust;
}

void 
object_interface::EnhancePetAttackDefense(int adjust)
{
	_imp->_en_point.pet_attack_defense += adjust;
}

int 
object_interface::GetRegionReputation(int index)
{
	return _imp->OI_GetRegionReputation(index);
}

void 
object_interface::ModifyRegionReputation(int index, int rep)
{
	_imp->OI_ModifyRegionReputation(index,rep);
}


int object_interface::GetCultivation()
{
	return _imp->OI_GetCultivation();
}

void object_interface::AdjustDropRate(float rate)
{
	_imp->OI_SetDropRate(rate);
}

bool 
object_interface::IsMarried()
{
	return _imp->OI_IsMarried();
}

bool
object_interface::IsFemale()
{
	return ((gactive_object*)_imp->_parent)->IsFemale();
}

bool 
object_interface::IsSpouse(const XID & target)
{
	return _imp->OI_IsSpouse(target);
}

void 
object_interface::JumpToSpouse()
{
	_imp->OI_JumpToSpouse();
}

long
object_interface::GetGlobalValue(long lKey)
{
	return _imp->OI_GetGlobalValue(lKey);	
}

void
object_interface::PutGlobalValue(long lKey, long lValue)
{
	_imp->OI_PutGlobalValue(lKey, lValue);
}

void
object_interface::ModifyGlobalValue(long lKey, long lValue)
{
	_imp->OI_ModifyGlobalValue(lKey, lValue);	
}

void 
object_interface::AssignGlobalValue(long srcKey, long destKey)
{
	_imp->OI_AssignGlobalValue(srcKey, destKey);
}

void
object_interface::PlayerCatchPet(const XID& catcher)
{
	_imp->PlayerCatchPet(catcher);
}

void
object_interface::PlayerDiscover(int discover_type)
{
	_imp->PlayerDiscover(discover_type);
}

void 
object_interface::SendClientAchieveData(size_t size, const void * data)
{
	_imp->_runner->player_achievement_data(size, data);
}

void 
object_interface::SendClientAchieveFinish(unsigned short achieve_id, int achieve_point, int finish_time)
{
	_imp->_runner->player_achievement_finish(achieve_id, achieve_point, finish_time);
}

void 
object_interface::SendClientPremissData(unsigned short achieve_id, char premiss_id,  size_t size, const void * data)
{
	_imp->_runner->player_premiss_data(achieve_id, premiss_id, size, data);
}

void 
object_interface::SendClientPremissFinish(unsigned short achieve_id, char premiss_id)
{
	_imp->_runner->player_premiss_finish(achieve_id, premiss_id);
}

void 
object_interface::SendAchievementMessage(unsigned short achieve_id, int broad_type, int param, int finish_time)
{
	((gplayer_imp*)_imp)->SendAchievementMessage(achieve_id, broad_type, param, finish_time);
}


bool 
object_interface::CanTransform(int template_id)
{
	return _imp->CanTransform(template_id);
}

void
object_interface::TransformMonster(int template_id, int lifetime)
{
	_imp->TransformMonster(template_id, lifetime);
}


int
object_interface::GetTemplateID()
{
	return _imp->OI_GetTemplateID();
}

bool
object_interface::StartTransform(int template_id, int level, int exp_level, int timeout, char type)
{
	return _imp->OI_StartTransform(template_id, level, exp_level, timeout,  type);
}

void
object_interface::StopTransform(int template_id, char type)
{
	_imp->OI_StopTransform(template_id, type);
}	

int
object_interface::GetTransformID()
{
	return _imp->OI_GetTransformID();
}

int
object_interface::GetTransformLevel()
{
	return _imp->OI_GetTransformLevel();
}

int
object_interface::GetTransformExpLevel()
{
	return _imp->OI_GetTransformExpLevel();
}

void
object_interface::DropItem(unsigned int item_id, unsigned int item_num, unsigned int expire_date)
{
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * data = gmatrix::GetDataMan().generate_item(item_id,&tag,sizeof(tag));

	if(data)
	{
		if(item_num > data->pile_limit) item_num = data->pile_limit;
		data->count = item_num;
		if(expire_date > 0 && data->pile_limit == 1) data->expire_date = g_timer.get_systime() + expire_date;
		DropItemData(_imp->_plane,_imp->_parent->pos,data,XID(0,0),0,0);
	}
}


void
object_interface::GetComboColor(int & c1, int & c2, int & c3, int & c4, int & c5)
{
	_imp->GetComboColor(c1, c2, c3, c4, c5);
}

void
object_interface::ClearComboSkill()
{
	_imp->ClearComboSkill();
}

bool 
object_interface::CanSetInvisible()
{
	return _imp->CanSetInvisible();
}

bool
object_interface::IsInvisible()
{
	return	((gplayer*)(_imp->_parent))->IsInvisible(); 
}

void 
object_interface::SetInvisible(int invisible_rate)
{
	_imp->SetInvisible(invisible_rate);
}

void
object_interface::ClearInvisible(int invisible_rate)
{
	_imp->ClearInvisible(invisible_rate);
}

void
object_interface::EnhanceInvisible(int value)
{
	ASSERT(value > 0);
	_imp->_en_point.invisible_rate += value;
	property_policy::UpdateInvisible(_imp);

	if(IsInvisible())
	{
		int pre = ((gactive_object*)_imp->_parent)->invisible_rate;
	       	int cur = _imp->GetInvisibleRate();	
		((gactive_object*)_imp->_parent)->invisible_rate = cur;
		if(pre < cur) _imp->_runner->inc_invisible(pre, cur);
	}
}

void
object_interface::ImpairInvisible(int value)
{
	ASSERT(value > 0);
	_imp->_en_point.invisible_rate -= value;
	property_policy::UpdateInvisible(_imp);

	if(IsInvisible())
	{
		int pre = ((gactive_object*)_imp->_parent)->invisible_rate;
	       	int cur = _imp->GetInvisibleRate();	
		((gactive_object*)_imp->_parent)->invisible_rate = cur;
		if(pre > cur) _imp->_runner->dec_invisible(pre, cur);
	}
}

void
object_interface::EnhanceAntiInvisible(int value)
{
	ASSERT(value > 0);
	int pre = ((gactive_object*)_imp->_parent)->anti_invisible_rate;

	_imp->_en_point.anti_invisible_rate += value;
	property_policy::UpdateInvisible(_imp);
	int cur = _imp->GetAntiInvisibleRate();	
	((gactive_object*)_imp->_parent)->anti_invisible_rate = cur;

	if(pre < cur)_imp->_runner->inc_anti_invisible(pre, cur);
}

void
object_interface::ImpairAntiInvisible(int value)
{
	ASSERT(value > 0);
	int pre = ((gactive_object*)_imp->_parent)->anti_invisible_rate;

	_imp->_en_point.anti_invisible_rate -= value;
	property_policy::UpdateInvisible(_imp);
	int cur = _imp->GetAntiInvisibleRate();	
	((gactive_object*)_imp->_parent)->anti_invisible_rate = cur;

	if(pre > cur)_imp->_runner->dec_anti_invisible(pre, cur);
}

void
object_interface::SetDimState(bool is_dim)
{
	_imp->SetDimState(is_dim);
}

bool
object_interface::IsCloneExist()
{
	return _imp->OI_IsCloneExist();
}

void
object_interface::ExchangePos(const XID & who)
{
	_imp->ExchangePos(who);
}

void
object_interface::ExchangeStatus(const XID & who)
{
	_imp->ExchangeStatus(who);
}

A3DVECTOR 
object_interface::GetPosBetween(const A3DVECTOR& start, const A3DVECTOR& end, float dist_from_start)
{	
	float dist = sqrt(end.squared_distance(start));
	float ratio = (dist_from_start) / dist;	
	float x = start.x + (end.x - start.x) * ratio;
	float z = start.z + (end.z - start.z) * ratio;
	float y = start.y + (end.y - start.y) * ratio;
	return A3DVECTOR(x, y, z);
}

bool object_interface::CheckTargetPosReachable(const A3DVECTOR& targetPos)
{
	return _imp->GetWorldManager()->GetTerrain().GetHeightAt(targetPos.x, targetPos.z) < targetPos.y + 0.5f;
}

/**
 * 做玩家位置和目标位置之间直线的碰撞检测
 * return true, 玩家能够到达目标点; false, 玩家不能到达目标点
 * nearPos返回碰撞检测最远能够到达的地方

 * 目前只检测目标点是否在建筑内, 冲锋类技能客户端是沿着地面走的，不能简单的用两个点的直线距离采样
 */
bool object_interface::CollisionDetect(const A3DVECTOR& targetPos, A3DVECTOR& nearPos, float sampledist)
{
	bool is_solid;
	float ratio;
	bool bRst;
	trace_manager& man = _imp->GetWorldManager()->GetTraceMan();

	A3DVECTOR samplePos;
	A3DVECTOR startPos = GetPos();
	nearPos = GetPos();
	if(!man.Valid()) return true;
	float dist = sqrt(targetPos.squared_distance(startPos));
	if(dist < 1e-3) return false;
	int sampleCnt = (int)(dist/sampledist);
	for(int i = 1; i <= sampleCnt; ++i)
	{
		samplePos = GetPosBetween(startPos, targetPos, i* sampledist);
		A3DVECTOR temp(samplePos.x, samplePos.y + 0.3375, samplePos.z);
		bRst = man.AABBTrace(temp, A3DVECTOR(0,-10,0), A3DVECTOR(0.075, 0.1125, 0.075), is_solid,ratio);
		if(bRst && is_solid) 
		{
			return false;
		}
		nearPos = samplePos;
	}

	//检查目标点是否可达
	A3DVECTOR temp(targetPos.x, targetPos.y + 0.3375, targetPos.z);
	bRst = man.AABBTrace(temp, A3DVECTOR(0,-10,0), A3DVECTOR(0.075, 0.1125, 0.075), is_solid,ratio);
	if(bRst && is_solid) return false;
	nearPos = targetPos;
	return true;	
}

bool object_interface::CanCharge(const XID& target_id, const A3DVECTOR& destPos, char chargeType, float distance)
{  
	A3DVECTOR startPos = GetPos();
	A3DVECTOR targetPos, nearPos; 
	
	if(chargeType == 3)
	{
		if(startPos.squared_distance(destPos) > (distance + 2) * (distance + 2))
		{
			return false;
		}
	}	
	else
	{
		float bodysize = 0.0f;
		int ret = QueryObject(target_id, targetPos, bodysize);
		if(!ret) return false;

		if(destPos.squared_distance(targetPos) > 64.0f)
		{
			return false;
		}
	}

	float dist = (destPos.x - startPos.x) * (destPos.x - startPos.x) + (destPos.z - startPos.z) * (destPos.z - startPos.z);
	float dist2 = (destPos.y - startPos.y) * (destPos.y - startPos.y);
	if(dist < dist2)
	{
		return false;
	}

	if(!CheckTargetPosReachable(destPos)) return false;
	return true; 
}

void 
object_interface::TryCharge(int type, const A3DVECTOR & dest_pos, int target_id)
{
	return _imp->TryCharge(type, dest_pos, target_id);
}


void
object_interface::Charge(const XID& target_id, const A3DVECTOR& destPos, char chargeType)
{
	A3DVECTOR start = GetPos();
	A3DVECTOR offset(destPos.x - start.x, destPos.y-start.y, destPos.z - start.z);
	_imp->StepMove(offset);
	_imp->_runner->object_charge(target_id.id, chargeType, destPos);
}

void object_interface::ChargeToTarget(const XID& target_id, const A3DVECTOR& destPos)
{
	A3DVECTOR start = GetPos();
	A3DVECTOR offset(destPos.x - start.x, destPos.y-start.y, destPos.z - start.z);
	_imp->StepMove(offset);
	_imp->_runner->object_charge(target_id.id, 1, destPos);
}

void
object_interface::SetCloneMirror(bool is_mirror, int rate)
{
	_imp->SetCloneMirror(is_mirror, rate);
}

void
object_interface::SetSkillMirror(bool is_mirror, int rate)
{
	_imp->SetSkillMirror(is_mirror, rate);
}

void
object_interface::SetSkillReflect(bool is_reflect, int rate)
{
	_imp->SetSkillReflect(is_reflect, rate);
}

void
object_interface::SetAbsoluteZone(int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state)
{
	if(skill_id < 0 || skill_level < 0) return;
	_imp->SetAbsoluteZone(skill_id, skill_level, radis, count, mp_cost, force, is_helpful, var1, var2, visible_state);
}

void
object_interface::SetHealEffect(int value)
{
	_imp->SetHealEffect(value);
}

void
object_interface::SetSkillTalent(int skill_id, int skill_talent[8])
{
	_imp->SetSkillTalent(skill_id, skill_talent);
}

void
object_interface::SetBreakCasting()
{
	_imp->SetBreakCasting();
}

bool
object_interface::IsIgniteState() 
{
	return _imp->IsIgniteState();
}

bool
object_interface::IsFrozenState()
{
	return _imp->IsFrozenState();
}

bool
object_interface::IsColdInjureState()
{
	return _imp->IsColdInjureState();
}

bool
object_interface::IsFuryState() 
{
	return _imp->IsFuryState();
}

void
object_interface::SetIgniteState(bool on)
{
	_imp->SetIgniteState(on);
}

void
object_interface::SetFrozenState(bool on)
{
	_imp->SetFrozenState(on);
}

void
object_interface::SetColdInjureState(bool on)
{
	_imp->SetColdInjureState(on);
}

void
object_interface::SetFuryState(bool on)
{
	_imp->SetFuryState(on);
}

void
object_interface::SetDarkState(bool on)
{
	_imp->SetDarkState(on);
}

void
object_interface::SetCycleState(bool on)
{
	_imp->SetCycleState(on);
}

int
object_interface::GetRegionPlayerCount(float radius, std::vector<exclude_target>& target_exclude)
{
	return _imp->_plane->GetRegionPlayerCount(_imp->_parent->ID.id, _imp->_parent->pos, radius, target_exclude);
}

bool
object_interface::DecDeityExp(int exp)
{
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	return pImp->DecDeityExp(exp);
}

void
object_interface::TransferAttack(XID & target, char force_attack,  int dmg_plus, int radius, int total)
{
	return _imp->TransferAttack(target, force_attack, dmg_plus, radius, total);

}

unsigned char object_interface::GetDirection()
{
	return _imp->_parent->dir;
}

void object_interface::SetTurnBuffState(bool on)
{
	_imp->SetTurnBuffState(on);
}

void object_interface::SetTurnDebuffState(bool on)
{
	_imp->SetTurnDebuffState(on);
}

void object_interface::SetBloodThirstyState(bool on)
{
	_imp->SetBloodThirstyState(on);
}

bool object_interface::IsBloodThirstyState()
{
	return _imp->IsBloodThirstyState();
	
}

//阵法相关
bool
object_interface::CircleOfDoomPrepare( float radius, int faction, int max_member_num, int skill_id)
{
	//max_member_num等于0是允许的，因为max_member_num不包含阵法发起者自己
	if( radius >= 0 && max_member_num >= 0)
	{
		return _imp->SetCircleOfDoomPrepare(radius, faction, max_member_num, skill_id);
	}
	else
	{
		__PRINTF("阵法参数有误object_interface::CircleOfDoomPrepare\n");
		return false;
	}
	return false;
}

void 
object_interface::CircleOfDoomStartup()
{
	_imp->SetCircleOfDoomStartup();
}

void 
object_interface::CircleOfDoomStop()
{
	_imp->SetCircleOfDoomStop();
}

int  
object_interface::GetPlayerInCircleOfDoom( std::vector<XID> &playerlist )
{
	return _imp->GetPlayerInCircleOfDoom(playerlist);
}

bool
object_interface::IsInCircleOfDoom()
{
	return _imp->IsInCircleOfDoom();
}

int object_interface::GetCircleMemberCnt()
{
	return _imp->GetCircleMemberCnt();
}

void object_interface::ScalePlayer(char on, int scale_ratio)
{
	return _imp->_runner->player_scale(on, scale_ratio);
}

void object_interface::AddLimitCycleArea(int owner, const A3DVECTOR& center, float radius)
{
	return _imp->AddLimitCycleArea(owner, center, radius);
}

void object_interface::AddPermitCycleArea(int owner, const A3DVECTOR& center, float radius)
{
	return _imp->AddPermitCycleArea(owner, center, radius);
}

void object_interface::RemoveLimitCycleArea(int owner)
{
	_imp->RemoveLimitCycleArea(owner);
}

void object_interface::RemovePermitCycleArea(int owner)
{
	_imp->RemovePermitCycleArea(owner);
}

void object_interface::ClearLimitCycleArea()
{
	_imp->ClearLimitCycleArea();
}

void object_interface::ClearPermitCycleArea()
{
	_imp->ClearPermitCycleArea();
}

void object_interface::GetPlayerInJail(std::vector<XID>& list, const A3DVECTOR& target, float radius, float height)
{
	gobject *pObj = _imp->_parent;
	_imp->_plane->GetColumnPlayers(list, pObj->pos, target, pObj->ID.id, radius, height);
}

void object_interface::SendRemovePermitCycleArea(const XID& target)
{	
	gobject *pObj = _imp->_parent;
	const XID& id = pObj->ID;
	if(id.type == GM_TYPE_NPC)
	{
		gnpc_imp* npc = (gnpc_imp*)_imp;
		MSG msg;
		BuildMessage(msg, GM_MSG_REMOVE_PERMIT_CYCLE_AREA, target, npc->_leader_id, pObj->pos);
		gmatrix::SendMessage(msg);
	}
}

bool object_interface::IsLimitCycleAreaExist(int owner)
{
	return _imp->IsLimitCycleAreaExist(owner);
}
bool object_interface::IsPermitCycleAreaExist(int owner)
{
	return _imp->IsPermitCycleAreaExist(owner);
}

bool object_interface::PlayerFollowTarget(const XID& target, float speed, float stop_dist)
{
	_imp->FollowTarget(target, speed, stop_dist);
	return true;
}

bool object_interface::StopPlayerFollowTarget()
{
	_imp->StopFollowTarget();
	return true;
}

void object_interface::PlayerPulling(bool on)
{
	_imp->_runner->player_pulling(on);
}

void object_interface::PlayerBeSpiritDraged(const XID& id_spirit_drag_me, bool on)
{
	_imp->SetIdSpiritDragMe(on ? id_spirit_drag_me.id : 0, on);
	_imp->_runner->player_be_pulled(id_spirit_drag_me.id, on, 1);
}

void object_interface::PlayerBePulled(const XID& pulling_id, bool on)
{
	_imp->SetIdPullingMe(on ? pulling_id.id : 0, on);
	_imp->_runner->player_be_pulled(pulling_id.id, on, 0);
}

void object_interface::NotifyCancelPull(const XID& target)
{
	_imp->SendTo<0>(GM_MSG_CANCEL_BE_PULLED,target,0);
}

void object_interface::SendClientTriggerSkillTime(short skillid, short time)
{
	_imp->_runner->trigger_skill_time(skillid, time);
}

void object_interface::SendClientAddonSkillPermanent(int id, int level)
{
	_imp->_runner->addon_skill_permanent(id, level);
}

void object_interface::pz_maxspeed()
{
	_imp->SetCollisionMaxSpeed();
}

void object_interface::pz_halfspeed()
{
	_imp->SetCollisionHalfSpeed();
}

void object_interface::pz_ashill(bool on)
{
	_imp->SetCollisionAshill(on);
}

void object_interface::pz_nomove(bool on)
{
	_imp->SetCollisionNomove(on);
}

void object_interface::pz_chaos(bool on)
{
	_imp->SetCollisionChaos(on);
}

void object_interface::pz_void(bool on)
{
	_imp->SetCollisionVoid(on);
}

//轩辕光暗值
void object_interface::SetDarkLight(int v)
{
	_imp->SetDarkLight(v);
	_imp->_runner->player_darklight();
}

int object_interface::GetDarkLight()
{
	return _imp->GetDarkLight();
}

void object_interface::IncDarkLight(int inc)
{
	_imp->IncDarkLight(inc);
	_imp->_runner->player_darklight();
}

void object_interface::DecDarkLight(int dec)
{
	_imp->DecDarkLight(dec);
	_imp->_runner->player_darklight();
}

void object_interface::SetDarkLightForm(char s)
{
	_imp->SetDarkLightForm(s);
}

void object_interface::ClearDarkLightForm()
{
	_imp->ClearDarkLightForm();
}

char object_interface::GetDarkLightForm()
{
	return _imp->GetDarkLightForm();
}

bool object_interface::IsRenMa()
{
	return _imp->IsRenMa();
}

bool object_interface::IsXuanYuan()
{
	return _imp->IsXuanYuan();
}
	
//轩辕灵体
void object_interface::SetDarkLightSpirit(int idx, char type)
{
	_imp->SetDarkLightSpirit(idx, type);
}

char object_interface::GetDarkLightSpirit(int idx)
{
	return _imp->GetDarkLightSpirit(idx);	
}

void object_interface::AddDarkLightSpirit(char type)
{
	_imp->AddDarkLightSpirit(type);
}  
 
bool object_interface::CheckCanFly()
{
	return _imp->CheckCanFly();
}

void object_interface::ActiveFlyState(char type)
{
	_imp->ActiveFlyState(type);
}

void object_interface::DeactiveFlyState(char type)
{
	_imp->DeactiveFlyState(type);
}

void object_interface::PreFly()
{
	_imp->PreFly();
}

void object_interface::SummonCastSkill(int summonId, const XID& target, int skillid, int skilllevel)
{
	_imp->SummonCastSkill(summonId, target, skillid, skilllevel);
}

void object_interface::SendMirrorImageCnt(int cnt)
{
	_imp->_runner->mirror_image_cnt(cnt);
}

bool object_interface::QueryTalismanEffects(int& level, std::vector<short>& skills)
{
	return _imp->GetTalismanRefineSkills(level, skills);
}
	
void object_interface::SetTalismanEmbedSkillCooltime(int skill_id, int cooltime)
{
	_imp->SetTalismanEmbedSkillCooltime(skill_id, cooltime);
}

void object_interface::SendTalismanSkillEffects(const XID& target, int level, float range, char force, int skill_var[16], const std::vector<short>& skills)
{
	msg_talisman_skill mts;
	mts.level = level;
	mts.range = range;
	mts.force = force;
	memcpy(mts.skill_var, skill_var, sizeof(mts.skill_var));
	size_t cnt = sizeof(mts.skillList) / sizeof(mts.skillList[0]);
	if(cnt > skills.size())
	{
		cnt = skills.size();
	}
	for(size_t i = 0; i < cnt; i ++)
	{
		mts.skillList[i] = skills[i];
	}

	gobject *pObj = _imp->_parent;
	MSG msg;
	BuildMessage(msg, GM_MSG_TALISMAN_SKILL, target, pObj->ID, pObj->pos, 0, &mts, sizeof(mts));
	gmatrix::SendMessage(msg);
}

void object_interface::EnhanceProp(int prop_id, int value)
{
	int t_value = value;
	int r_value = (int)(t_value/100.f);		//实际使用的值，除以100得到实际的精度
	if(prop_id != 11 && prop_id != 13 && r_value == 0) return;
		
	switch(prop_id)
	{
		case 1:  EnhanceMaxHP(r_value); break;	//气血
		case 2:  EnhanceMaxMP(r_value); break;	//真气
		case 3:  EnhanceDamage(r_value); break;	//攻击
		case 4:  EnhanceDefense(r_value); break; //防御
		case 5:  EnhanceAttack(r_value); break;  //命中
		case 6:  EnhanceArmor(r_value); break;	 //躲闪
		case 7:  EnhanceSkillAttack(r_value); break;  //技能命中
		case 8:  EnhanceSkillArmor(r_value); break;   //技能躲闪
		case 9:  EnhanceMaxDP(r_value); break;      //元力值
		case 10: EnhanceCrit(r_value); break;  	  //致命一击率
		case 11: EnhanceCritDmg(t_value/100000.f); break; //致命一击伤害
		case 12: EnhanceAntiCritRate(r_value); break;	//减免致命一击率
		case 13: EnhanceAntiCritDamage(t_value/100000.0f); break;	//减免致命一击伤害
		case 14: EnhanceCultDefense(0, r_value); break;	//御仙
		case 15: EnhanceCultDefense(1, r_value); break;	//御佛
		case 16: EnhanceCultDefense(2, r_value); break;	//御魔 
		case 17: EnhanceCultAttack(0, r_value); break;	//克仙
		case 18: EnhanceCultAttack(1, r_value); break;	//克佛
		case 19: EnhanceCultAttack(2, r_value); break;	//克魔
		case 20: EnhanceDmgReduce(r_value); break;	//减免固定伤害
		case 21: EnhanceScaleDmgReduce(r_value); break;	//减免伤害百分比
		case 22: EnhanceResistance(0, r_value); break;	//眩晕
		case 23: EnhanceResistance(2, r_value); break;	//定神
		case 24: EnhanceResistance(3, r_value); break;	//魅惑
		case 25: EnhanceResistance(1, r_value); break;	//虚弱
		case 26: EnhanceResistance(4, r_value); break;	//昏睡
		case 27: EnhanceResistance(5, r_value); break;	//减速
		case 28: EnhanceScaleIgnDmgReduce(r_value); break; //无视伤害减免
		case 29: EnhanceScaleMaxHP(r_value); break;	//气血增加百分比
		case 30: EnhanceScaleMaxMP(r_value); break;	//真气增加百分比
		case 31: EnhanceScaleDamage(r_value); break;	//攻击增加百分比
		case 32: EnhanceScaleDefense(r_value); break;	//防御增加百分比
		case 33: EnhanceDeityPower(r_value); break;	//元神威能
		default: break;
	}
}

void object_interface::ImpairProp(int prop_id, int value)
{
	int t_value = value;
	int r_value = (int)(t_value/100.f);		//实际使用的值，除以100得到实际的精度
	if(prop_id != 11 && prop_id != 13 && r_value == 0) return;

	switch(prop_id)
	{
		case 1:  ImpairMaxHP(r_value); break;	//气血
		case 2:  ImpairMaxMP(r_value); break;	//真气
		case 3:  ImpairDamage(r_value); break;	//攻击
		case 4:  ImpairDefense(r_value); break; //防御
		case 5:  ImpairAttack(r_value); break;  //命中
		case 6:  ImpairArmor(r_value); break;	 //躲闪
		case 7:  ImpairSkillAttack(r_value); break;  //技能命中
		case 8:  ImpairSkillArmor(r_value); break;   //技能躲闪
		case 9:  ImpairMaxDP(r_value); break;      //元力值
		case 10: ImpairCrit(r_value); break;  	  //致命一击率
		case 11: ImpairCritDmg(t_value/100000.f); break; //致命一击伤害
		case 12: ImpairAntiCritRate(r_value); break;	//减免致命一击率
		case 13: ImpairAntiCritDamage(t_value/100000.0f); break;	//减免致命一击伤害
		case 14: ImpairCultDefense(0, r_value); break;	//御仙
		case 15: ImpairCultDefense(1, r_value); break;	//御佛
		case 16: ImpairCultDefense(2, r_value); break;	//御魔 
		case 17: ImpairCultAttack(0, r_value); break;	//克仙
		case 18: ImpairCultAttack(1, r_value); break;	//克佛
		case 19: ImpairCultAttack(2, r_value); break;	//克魔
		case 20: ImpairDmgReduce(r_value); break;	//减免固定伤害
		case 21: ImpairScaleDmgReduce(r_value); break;	//减免伤害百分比
		case 22: ImpairResistance(0, r_value); break;	//眩晕
		case 23: ImpairResistance(2, r_value); break;	//定神
		case 24: ImpairResistance(3, r_value); break;	//魅惑
		case 25: ImpairResistance(1, r_value); break;	//虚弱
		case 26: ImpairResistance(4, r_value); break;	//昏睡
		case 27: ImpairResistance(5, r_value); break;	//减速
		case 28: ImpairScaleIgnDmgReduce(r_value); break; //无视伤害减免
		case 29: ImpairScaleMaxHP(r_value); break;	//气血增加百分比
		case 30: ImpairScaleMaxMP(r_value); break;	//真气增加百分比
		case 31: ImpairScaleDamage(r_value); break;	//攻击增加百分比
		case 32: ImpairScaleDefense(r_value); break;	//防御增加百分比
		case 33: ImpairDeityPower(r_value); break;	//元神威能
		default: break;
	}
}


void object_interface::SetExtraEquipEffectState(bool on, char weapon_effect_level)
{
	_imp->SetExtraEquipEffectState(on, weapon_effect_level);
}

bool object_interface::IsMaster()
{
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	int id = pImp->GetSectID();
	return id && id == _imp->_parent->ID.id;
}

int object_interface::GetXPSkill()
{
	if(IsPlayerClass())
	{
		return player_template::GetXPSkill(GetClass());
	}
	return 0;
}

void object_interface::CurseTeleport(const XID & who)
{
	_imp->CurseTeleport(who);
}

void object_interface::SpyTrap()
{
	_imp->SpyTrap();
}

void object_interface::SetPuppetForm(bool on, int num)
{
	_imp->SetPuppetForm(on, num);
}

void object_interface::SetPuppetSkill(int skill_id, int immune_prob)
{
	_imp->SetPuppetSkill(skill_id, immune_prob);
}

void object_interface::BounceTo(const A3DVECTOR & target_pos, float dist)
{
	_imp->BounceTo(target_pos, dist);
}

void object_interface::DrawTo(const XID & target, const A3DVECTOR & pos)
{
	_imp->DrawTo(target, pos);
}

void object_interface::Repel2(const A3DVECTOR & target_pos, float dist)
{
	_imp->Repel2(target_pos, dist);
}

void object_interface::Blowoff()
{
	_imp->Blowoff();
}

/*
void object_interface::Throwback(const A3DVECTOR & target_pos, float dist)
{
	printf("throw back target pos: x,y,z=%f, %f, %f", target_pos.x, target_pos.y , target_pos.z); 
	A3DVECTOR dest_pos = target_pos;
	dest_pos -= _imp->_parent->pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= dist/sqrt(sq);
	dest_pos += target_pos; 

	A3DVECTOR near_pos;
	if(!CollisionDetect(dest_pos, near_pos, 0.2f))
	{
		return;
	}

	float height = _imp->GetWorldManager()->GetTerrain().GetHeightAt(near_pos.x,near_pos.z);
	if(near_pos.y < height) near_pos.y = height;

	A3DVECTOR start = _imp->_parent->pos; 
	A3DVECTOR offset(near_pos.x - start.x, near_pos.y-start.y, near_pos.z - start.z);

	_imp->StepMove(offset);
	_imp->_runner->object_charge(0, 6, near_pos);
	printf("throw back dest pos: x,y,z=%f, %f, %f", dest_pos.x, dest_pos.y , dest_pos.z); 
	printf("throw back: x,y,z=%f, %f, %f", _imp->_parent->pos.x, _imp->_parent->pos.y, _imp->_parent->pos.z);
}
*/

void object_interface::Throwback(const A3DVECTOR & target_pos, float dist)
{
	_imp->Throwback(target_pos, dist);
}

void object_interface::CastCycleSkill(int skill_id, int skill_level, char skill_type,  float radius, int coverage, int mpcost, int hpcost, 
		int dpcost, int inkcost, char force, float ratio, int plus, int period, int var[16], int crit_rate) 
{
	_imp->CastCycleSkill(skill_id, skill_level, skill_type, radius, coverage, mpcost, hpcost, dpcost, inkcost, force, ratio, plus, period, var, crit_rate);
}

void object_interface::SetQilinForm(bool on)
{
	_imp->SetQilinForm(on);
}

void object_interface::SetRage(int rage)
{
	_imp->SetRage(rage);
}

void object_interface::AddRage(int rage)
{
	_imp->AddRage(rage);
}

int object_interface::GetPuppetid()
{
	return _imp->GetPuppetid();
}

void object_interface::ReduceSkillCooldown(int id, int cooldown_msec)
{
	return _imp->ReduceSkillCooldown(id, cooldown_msec);
}

// Youshuang add
void object_interface::DeliverTopicSite( TOPIC_SITE::topic_data* data )
{
	_imp->DeliverTopicSite( data );
}
// end
void object_interface::NotifyBloodPoolStatus(bool on, int cur_hp, int max_hp)
{
	return _imp->NotifyBloodPoolStatus(on, cur_hp, max_hp);
}

const XID
object_interface::GetQilinLeaderID()
{
	return _imp->GetQilinLeaderID();
}

bool
object_interface::IsQilinLeader(const XID & target)
{
	return GetQilinLeaderID() == target;
}

void
object_interface::BeHurtOnSeekAndHideRaid(const XID & who)
{
	_imp->BeHurtOnSeekAndHideRaid(who);
}

void
object_interface::SendClientHiderTaunted(bool isStart)
{
	_imp->SendClientHiderTaunted(isStart);
}

void
object_interface::GetSeekAndHiderTauntedScore()
{
	_imp->GetSeekAndHiderTauntedScore();
}
