
#include "common/types.h"
#include "obj_interface.h"

#include "filter.h"
#include "playerwrapper.h"
#include "skillwrapper.h"
#include "skill.h"
#include "skillfilter.h"

namespace GNET
{
	//========================PlayerWrapper==start

	PlayerWrapper::PlayerWrapper(
		object_interface _o,
		char _type,
		Skill* _s,
		const XID* _target,
		int _size) : object(_o), context(_type), skill(_s), target(_target), tsize(_size), probability(0), ratio(0), amount(0),
		value(0), time(0), buffid(0), health(0), result(0), atstate(0), invader(false)
	{
		if(tsize<=0)
			target = NULL;
			
		if(skill)
			skill->SetPlayer(this);

		memset(var, 0, sizeof(var));
	}

	PlayerWrapper::~PlayerWrapper()
	{
	}

	//1
	bool PlayerWrapper::SetDisperseanti(bool a2)
	{
		object.AddFilter(new filter_Disperseanti(object, ratio, time));
		return true;
	}

	//2
	int PlayerWrapper::GetDarklight()
	{
		return object.GetDarkLight();
	}

	//3
	bool PlayerWrapper::SetDiscover(bool v)
	{
		//反汇编发现参数未使用
		object.PlayerDiscover(value);
		return true;
	}

	//4
	bool PlayerWrapper::SetFrenzied(bool a2)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Frenzied(object, ratio * 1000.0, value, amount, probability, time));
		}
		return true;
	}

	//5
	void PlayerWrapper::SetVar2(int v)
	{
		SetVar(v, 1);
	}

	//6
	void PlayerWrapper::SetVar1(int v)
	{
		SetVar(v, 0);
	}

	//7
	bool PlayerWrapper::SetSecondattack(bool a2)
	{
		if (context == 1)
		{
			if (skill)
			{
				int dam = skill->GetDamage() * (ratio + 1.0) - object.GetExtendProp().defense * (1.0 - value);
				if (dam > 0)
				{
					if (amount && dam > amount)
					{
						dam = amount;
					}
					object.BeHurt(skill->GetAttackerid(), skill->GetAttacker(), dam, invader, skill->GetAttackerMode());
				}
			}
		}
		return true;
	}

	//8
	bool PlayerWrapper::SetQilinform(bool a2)
	{
		if (ThrowDice())
		{
			if (object.GetShape())
			{
				object.RemoveFilter(FILTER_QILINFORM);  //4268 = 0x10AC
			}
			else
			{
				object.AddFilter(new filter_Qilinform(object,value));
			}
		}
		return true;
	}

	//9
	bool PlayerWrapper::SetClearcooldown(bool a2)
	{
		object.ClrCoolDown(ratio + 1024);
		object.ClrCoolDown(amount + 1024);
		object.ClrCoolDown(value + 1024);
		return 1;
	}

	//10
	bool PlayerWrapper::SetInsanityform(bool a2)
	{
		if (!object.GetShape())
		{
			object.AddFilter(new filter_Insanityform(this->object,probability,buffid,value,ratio,amount));
			return true;
		}
		if (!object.IsFilterExist(FILTER_INSANITYFORM))
		{
			if (object.IsFilterExist(FILTER_GHOSTFORM))
			{
				object.RemoveFilter(FILTER_GHOSTFORM);
			}
			object.AddFilter(new filter_Insanityform(this->object, probability, buffid, value, ratio, amount));
			return true;
		}
		else
		{
			object.RemoveFilter(FILTER_INSANITYFORM);
			return false;
		}
	}

	//11
	bool PlayerWrapper::SetDizzytimer(bool a2)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Dizzytimer(object, time, value, amount));
		}
		return true;
	}

	//12
	bool PlayerWrapper::SetDsleep(bool b)
	{
		if (probability < 0.001)
			return 0;
		if (context == 1 && ThrowDice(4))
		{
			if ((object.GetImmuneMask() & IMMUNESLEEP) != 0)
			{
				atstate = atstate | IMMUNEWEAK;   //0x80 = 128
			}
			else
			{
				AddCommonDebuff( 6, time, GetMaxhp() * value, 0, false);
			}
		}
		else
		{
			atstate |= IMMUNEREPEL;
		}
		return true;
	}

	//13
	void PlayerWrapper::SetVar6(int v)
	{
		SetVar(v, 5);
	}

	//14
	bool PlayerWrapper::SetDrunk(bool a2)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Drunk(object, ratio, value, amount, time));
		}
		return true;
	}

	//15
	int PlayerWrapper::GetVar4()
	{
		return var[3];
	}

	//16
	bool PlayerWrapper::SetDamagemove(bool a2)
	{
		if (ThrowDice())
		{
			filter_Damagemove* fdam = new filter_Damagemove(object, value, time, buffid);
			fdam->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fdam);
		}
		return 1;
	}

	//17
	bool PlayerWrapper::SetPasaddattack(float inc)
	{
		if (context == 3)
		{
			object.ImpairDamage(inc);
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceDamage(inc);
			if (context == 4)
			{
				object.UpdateAttackData();
			}
		}
		return true;
	}

	//18
	int PlayerWrapper::GetAtkrate()
	{
		return object.GetExtendProp().attack;
	}

	//19
	bool PlayerWrapper::SetUniqprompt(bool a2)
	{
		if (buffid)
		{
			atstate |= HSTATE_ADDDEFENCE;
			object.SetNextAttackState(HSTATE_ADDDEFENCE);
		}
		return true;
	}

	//20
	void PlayerWrapper::SetVar10(int v)
	{
		SetVar(v, 9);
	}

	//21
	bool PlayerWrapper::SetTongxin(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Tongxin(object, ratio * 1000.0, value, time, buffid));
		}
		return true;
	}

	//22
	bool PlayerWrapper::SetPasaddsleep(float inc)
	{
		if (context == 3)
		{
			object.ImpairResistance(4, inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance(4,inc);
			if (context == 4)
				object.UpdateMagicData();
		}
		return true;
	}

	//23
	bool PlayerWrapper::SetSubantiwrap2(bool)
	{
		if (!object.IsPlayerClass())
		{
			return 0;
		}
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subantiwrap2(object, value, time, amount));
		}
		return true;
	}

	//24
	bool PlayerWrapper::SetSharestatewithclone(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Sharestatewithclone(object, time, ratio));
		}
		return true;
	}

	//25
	bool PlayerWrapper::SetDecskillaccu(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Decskillaccu(object, ratio * 1000.0, time, buffid));
		}
		return true;
	}

	//26
	bool PlayerWrapper::SetYuanling(bool)
	{
		if (ThrowDice())
		{
			if (object.IsFilterExist(FILTER_YUANLING))
			{
				object.RemoveFilter(FILTER_YUANLING);
			}
			else
			{
				object.AddFilter(new filter_Yuanling(object, value));
			}
		}
		return true;
	}

	//27
	bool PlayerWrapper::SetIceshield(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Iceshield(object, amount, value, time));
		}
		return true;
	}

	//28
	bool PlayerWrapper::SetSleep(bool)
	{
		int atstate; // edx
		long double v3; // fst7

		if (probability < 0.001)
		{
			return 0;
		}
		if (context == 1 && ThrowDice(4))
		{
			if ((object.GetImmuneMask() & IMMUNESLEEP) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				AddCommonDebuff( 1, time, GetMaxhp() * value, 0, false);
			}
		}
		else
		{
			atstate |= IMMUNEREPEL;
		}
		return true;
	}

	//29
	bool PlayerWrapper::SetCycsubattack(bool)
	{
		unsigned int v3; // [esp-Ch] [ebp-34h]
		filter_Cycsubattack* v4; // [esp+14h] [ebp-14h]

		if (ThrowDice())
		{
			object.AddFilter(new filter_Cycsubattack(object, amount, time, buffid));
		}
		return true;
	}

	//30
	bool PlayerWrapper::SetBaradddogeaccu(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_BarAdddogeaccu(object, amount, value, buffid));
		}
		return true;
	}

	//31
	bool PlayerWrapper::SetJumptospouse(bool)
	{
		if (!context)
		{
			object.JumpToSpouse();
		}
		return true;
	}

	//32
	bool PlayerWrapper::ThrowDiceDiet(void)
	{
		probability = (1.0 - object.GetExtendProp().anti_diet / 1000.0) * probability;
		return ThrowDice();
	}

	//33
	bool PlayerWrapper::SetBounceto(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEREPEL) != 0)
			{
				atstate  = atstate | IMMUNEWEAK;
			}
			else
			{
				object.BounceTo( skill->GetAttackerpos(), value);
			}
		}
		return true;
	}

	//34
	bool PlayerWrapper::SetPasincpetdamage(float dec)
	{
		if (context == 3)
			object.EnhancePetDamage( -(int)(dec * 1000.0));
		else
			object.EnhancePetDamage((int)(dec * 1000.0));
		return true;
	}

	//35
	int PlayerWrapper::TakeOutItem(int inv_index, int item_id, int count)
	{
		return object.TakeOutItem( inv_index, item_id, count);
	}

	//36
	bool PlayerWrapper::IsXuanYuan(void)
	{
		return object.IsXuanYuan();
	}

	//37
	bool PlayerWrapper::SetMp(float mp)
	{
		float tmp = object.GetBasicProp().mp - mp;
		if (tmp <= 0)
		{
			return 0;
		}
		else
		{
			return object.DrainMana(tmp);
		}
	}

	//38
	bool PlayerWrapper::SetEnmity(int n)
	{
		if (skill->GetType() == 2)
		{
			object.AddAggroToEnemy(skill->GetAttackerid(), n);
		}
		else
		{
			object.AddAggro(skill->GetAttackerid(), n);
		}
		return true;
	}

	//39
	void PlayerWrapper::SetVar16(int v)
	{
		SetVar( v, 15);
	}

	//40
	bool PlayerWrapper::SetMpdisperse(bool a2)
	{
		if (object.IsRenMa())
		{
			return 0;
		}
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEMPDISPERSE) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				int mp = object.GetBasicProp().mp;
				object.DrainMana(amount);
				if (amount > mp)
				{
					object.BeHurt(skill->GetAttackerid(), skill->GetAttacker(), amount - mp, invader, skill->GetAttackerMode());
				}
			}
		}
		return 1;
	}

	//41
	bool PlayerWrapper::SetDimming(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Dimming(object, ratio, value, time, amount, buffid));
		}
		return true;
	}

	//42
	bool PlayerWrapper::SetInchurt2(bool)
	{
		if (context == 1 && ThrowDice())
		{
			object.AddFilter(new filter_Inchurt(object, ratio, time));
		}
		return true;
	}

	//43
	void PlayerWrapper::DoPerform(void)
	{
		object.SendClientMsgSkillPerform(skill->GetSkillSpiritIndex());
		bool cleanCoolTime = 0;
		int occu = skill->GetOccupation();
		if (occu < 128 || occu == 146 || (occu > 147 && occu <= 150) || (occu > 166 && occu <= 169))
		{
			object.ModifyFilter(FILTER_INSTANTSKILL, 11, &cleanCoolTime, 1u);
		}
		int coolTime = 0;
		if (!cleanCoolTime)
		{
			int adjust = 0;
			if (skill->GetDarkLightType() == 1)
			{
				adjust = object.GetSkillWrapper().GetDarkSkillCDAdjust();
			}
			else if (skill->GetDarkLightType() == 2)
			{
				adjust = object.GetSkillWrapper().GetLightSkillCDAdjust();
			}
			coolTime = adjust + skill->GetCooltime();
			if (coolTime < 0)
				coolTime = 0;
		}
		object.SetCoolDown(skill->GetId() + 1024, coolTime);
		if (skill->IsFamily())
		{
			object.UpdateFamilySkill(skill->GetId(), 1);
		}
		if (skill->IsTalisman())
		{
			object.TalismanGainExp( 1, 0);
			object.SetTalismanEmbedSkillCooltime(skill->GetId(), coolTime);
		}
		ConsumeItem();
		if (skill->IsUseProficiency() && skill->GetIncProficiency() > 0)
		{
			object.GetSkillWrapper().AddProficiency(object, skill->GetId(), skill->GetIncProficiency());
		}
	}

	//44
	bool PlayerWrapper::SetBreakcasting(bool)
	{
		if (ThrowDice())
		{
			object.SetBreakCasting();
		}
		return true;
	}

	//45
	bool PlayerWrapper::IsFlying(void)
	{
		return object.IsFlying();
	}

	//46
	bool PlayerWrapper::SetCycsubdefence(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Cycsubdefence(object, amount, time, buffid));
		}
		return true;
	}

	//47
	int PlayerWrapper::GetAttackHigh(float ratio, float plus)
	{
		return object.GetExtendProp().damage_high * ratio + plus;
	}

	//48
	bool PlayerWrapper::SetDarkuniform(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Darkuniform(object, ratio, value, amount, time, buffid));
		}
		return true;
	}

	//49
	bool PlayerWrapper::SetAddantisilent(bool)
	{
		object.AddFilter(new filter_Addantisilent(object, value, time, buffid));
		return true;
	}

	//50
	bool PlayerWrapper::SetAddattack4(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addattack4(object, value, time, buffid));
		}
		return true;
	}

	//51
	bool PlayerWrapper::SetHpleak2(bool)
	{
		if (!ThrowDice())
		{
			return 0;
		}
		float Amount = GetAmount();
		float damage = Amount;
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | IMMUNEWEAK;
		}
		else if (damage > 1)
		{
			if (!object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish(skill->GetAttackerlevel(), GetLevel());
				damage = Amount * damage;
			}
			filter_Hpleak2* fhpl = new filter_Hpleak2(object, time, damage, value, buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return true;
	}

	//52
	bool PlayerWrapper::SetSetskilllevel(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Setskilllevel(object, time, value, amount, ratio, buffid));
		}
		return true;
	}

	//53
	bool PlayerWrapper::SetHpregain(bool)
	{
		object.AddFilter(new filter_Hpregain(object, ratio, value, amount, probability, time));
		return true;
	}

	//54
	int PlayerWrapper::GetForm(void)
	{
		return object.GetShape();
	}

	//55
	int PlayerWrapper::GetReborncount(void)
	{
		return object.RebornCount();
	}

	//56
	bool PlayerWrapper::SetDarkness(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Darkness(object, time));
		}
		return true;
	}

	//57
	bool PlayerWrapper::SetTriggerskill(bool)
	{
		if (ThrowDice())
		{
			if (object.IsFilterExist(FILTER_TRIGGERSKILL))
			{
				trigger_skill ts;
				ts.skillid = value;
				ts.skilllevel = amount;
				ts.time = time;
				object.ModifyFilter(FILTER_TRIGGERSKILL, 10, &ts, sizeof(ts)); //CTRL_BUFFLEVEL
			}
			else
			{
				object.AddFilter(new filter_Triggerskill(object, time, value, amount));
			}
		}
		return 1;
	}

	//58
	bool PlayerWrapper::SetExpboost(bool)
	{
		if (ThrowDice() && time > 0)
		{
			object.AddFilter(new filter_Expboost(object, amount, time));
		}
		return true;
	}

	//59
	bool PlayerWrapper::SetUnsummon(bool)
	{
		if (ThrowDice())
		{
			object.UnSummonMonster(ratio);
		}
		return true;
	}

	//60
	bool PlayerWrapper::SetNingjin(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Ningjin(object, ratio, time));
		}
		return true;
	}

	//61
	bool PlayerWrapper::SetAmount(float d)
	{
		amount = d;
		return true;
	}

	//62
	bool PlayerWrapper::SetClearbuff(bool)
	{
		if (ThrowDice())
		{
			object.ClearRandomSpecFilter(0x20000000, amount);  //  haokaixin const
		}
		return true;
	}

	//63
	bool PlayerWrapper::SetBarexpboost(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Barexpboost(object, value, amount, buffid));
		}
		return true;
	}

	//64
	float PlayerWrapper::GetCrithurt(void)
	{
		return object.GetExtendProp().crit_damage;
	}

	//65
	bool PlayerWrapper::SetBarmpgen(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Barmpgen(object, value, amount, buffid));
		}
		return true;
	}

	//66
	int PlayerWrapper::GetAttackLow(float _ratio, float _plus)
	{
		return object.GetExtendProp().damage_low * _ratio + _plus;
	}

	//67
	bool PlayerWrapper::SetShadowdance(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Shadowdance(object, ratio, probability, value, time, amount));
		}
		return true;
	}

	//68
	bool PlayerWrapper::SetPowerless(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Powerless(object, time));
		}
		return true;
	}

	//69
	bool PlayerWrapper::SetPasaddweaktenaciy(float inc)
	{
		if (context == 3)
		{
			object.ImpairResistanceTenaciy(1, inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceTenaciy(1, inc);
			if (context == 4)
				object.UpdateMagicData();
		}
		return true;
	}

	//70
	int PlayerWrapper::GetVar3(void)
	{
		return var[2];
	}

	//71
	bool PlayerWrapper::SetSubhp(bool)
	{
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | IMMUNEWEAK;
		}
		else if (ThrowDice())
		{
			object.AddFilter(new filter_Subhp(object, value, amount, time, buffid));
		}
		return true;
	}

	//72
	bool PlayerWrapper::SetTalismaneffects(bool)
	{
		if (skill->IsTalisman() && skill->GetData())
		{
			object.SendTalismanSkillEffects(
				skill->GetAttackerid(),
				skill->GetLevel(),
				skill->GetEffectdistance(),
				skill->GetForceattack(),
				var,
				skill->GetData()->talismanRefineSkillList);
		}
		return true;
	}

	//73
	int PlayerWrapper::GetVar8(void)
	{
		return var[7];
	}

	//74
	bool PlayerWrapper::SetSalvation(bool)
	{
		if (object.IsDead())
		{
			if (GetLevel() <= 14 || ratio < 0.0)
				ratio = 0.0;
			object.Resurrect(ratio, value);
		}
		return true;
	}

	//75
	bool PlayerWrapper::SetTeamenchant(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Teamenchant(object, value, ratio, amount, skill->GetForceattack(), buffid));
		}
		return true;
	}

	//76
	bool PlayerWrapper::SetImmunesleep(bool)
	{
		object.AddFilter(new filter_Immunesleep(object, time));
		return true;
	}

	//77
	void PlayerWrapper::SendResult(XID const& attacker, int atstate)
	{
		object.SendClientEnchantResult(attacker, skill->GetId(), skill->GetLevel(), invader, atstate, object.GetAttackStamp(), health);
		object.IncAttackStamp();
	}

	//78
	float PlayerWrapper::GetSaint(void)
	{
		return object.GetExtendProp().deity_power;
	}

	//79
	bool PlayerWrapper::CanCharge(XID const& target, A3DVECTOR const& destPos, char chargeType, float distance)
	{
		return object.CanCharge(target, destPos, chargeType, distance);
	}

	//80
	bool PlayerWrapper::SetDecaccuracy(bool)
	{
		unsigned int v3; // [esp-Ch] [ebp-34h]
		filter_Decaccuracy* v4; // [esp+14h] [ebp-14h]

		if (ThrowDice())
		{
			object.AddFilter(new filter_Decaccuracy(object, ratio * 1000.0, time, buffid));
		}
		return true;
	}

	//81
	int PlayerWrapper::GetSkilllevel(int id)
	{
		return object.GetSkillWrapper().GetSkillLevel(id);
	}

	//82
	int PlayerWrapper::GetCyclemembercnt(void)
	{
		return object.GetCircleMemberCnt();
	}

	//83
	bool PlayerWrapper::SetSkillreflect(bool)
	{
		unsigned int v3; // [esp-Ch] [ebp-34h]
		filter_Skillreflect* v4; // [esp+14h] [ebp-14h]

		if (ThrowDice())
		{
			object.AddFilter(new filter_Skillreflect(object,ratio, value, time));
		}
		return true;
	}

	//84
	bool PlayerWrapper::SetWraptimer(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Wraptimer(object, time, value, amount));
		}
		return true;
	}

	//85
	bool PlayerWrapper::SetDectp(int dec)
	{
		return object.ModifyTalentPoint(-dec);
	}

	//86
	bool PlayerWrapper::SetReturnpos(bool)
	{
		unsigned int v3; // [esp-Ch] [ebp-24h]
		filter_Returnpos* v4; // [esp+Ch] [ebp-Ch]

		if (ThrowDice())
		{
			object.AddFilter(new filter_Returnpos(object));
		}
		return true;
	}

	//87
	bool PlayerWrapper::SetPasadddizzytenaciy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceTenaciy(0, inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceTenaciy(0, inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return true;
	}

	//88
	bool PlayerWrapper::SetSummonteleport2(bool)
	{
		if (ThrowDice())
		{
			object.CreateTelePort(value, time, 9, skill->GetLevel(), 0, skill->GetId(), ratio, amount, 0);
		}
		return true;
	}

	//89
	bool PlayerWrapper::SetParalysis(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Paralysis(object, time, ratio * 100.0, value * 1000.0));
		}
		return true;
	}

	//90
	int PlayerWrapper::GetRes6(void)
	{
		return object.GetExtendProp().resistance[5];
	}

	//91
	bool PlayerWrapper::SetAutorescurrect(bool)
	{
		object.AddFilter(new filter_Autoresurrect(this->object, this->time, this->value, this->ratio));
		return true;
	}

	//92
	void PlayerWrapper::SetVar9(int v)
	{
		SetVar( v, 8);
	}

	//93
	bool PlayerWrapper::SetBlowoff(bool)
	{
		int atstate; // edx

		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEREPEL) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				object.Blowoff();
				AddCommonDebuff(0, 1, 0, 0, 0);
			}
		}
		return true;
	}

	//94
	bool PlayerWrapper::SetInchpgen(bool)
	{
		if (this->time > 1)
		{
			object.AddFilter(new filter_Inchpgen(object, ratio * 1000.0, time, buffid));
		}
		return true;
	}

	//95
	int PlayerWrapper::GetDebuffcnt(void)
	{
		return object.FilterCnt(0x10000000);  //FILTER_MASK_DEBUFF
	}

	//96
	bool PlayerWrapper::IsBindState(void)
	{
		return object.IsBindState();
	}

	//97
	bool PlayerWrapper::SetPasdecfatalhurt(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceAntiCritDamage( uint(dec) ^ 0x80000000);
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceAntiCritDamage(dec);
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//98
	int PlayerWrapper::IsMarried(void)
	{
		return object.IsMarried();
	}

	//99
	int PlayerWrapper::GetVar9(void)
	{
		return this->var[8];
	}

	//100
	bool PlayerWrapper::SetPasincskillaccu(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceSkillAttack( -dec);
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceSkillAttack(dec);
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//101
	bool PlayerWrapper::SetVipguilin(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Vipguilin(object, ratio * 1000.0, value, buffid));
		}
		return true;
	}

	//102
	bool PlayerWrapper::SetClearslow(bool)
	{
		if (ThrowDice())
			RemoveMultiFilter(2);
		return true;
	}

	//103
	bool PlayerWrapper::IsFilterExist(int id)
	{
		return object.IsFilterExist(id);
	}

	//104
	bool PlayerWrapper::SetDirecthurt(bool)
	{
		if (this->context == 1 && this->skill && ThrowDice())
		{
			object.BeHurt(skill->GetAttackerid(), skill->GetAttacker(), value, invader, skill->GetAttackerMode());
			object.AddAggro(skill->GetAttackerid(), value + 2);
		}
		return true;
	}

	//105
	bool PlayerWrapper::SetIncskilldamage(bool)
	{
		object.AddFilter(new filter_Incskilldamage(this->object, this->ratio, this->time, this->buffid));
		return true;
	}

	//106
	bool PlayerWrapper::SetMirrorimage(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Mirrorimage(this->object, this->time, (int)this->value, (int)this->amount));
		}
		return true;
	}

	//107
	bool PlayerWrapper::SetAddcharm(bool)
	{
		if (object.IsFilterExist(FILTER_ADDCHARM))
		{
			object.RemoveFilter(FILTER_ADDCHARM);
		}
		else
		{
			object.AddFilter(new filter_Addcharm(object, probability, value, amount, ratio));
		}
		return true;
	}

	//108
	bool PlayerWrapper::SetDeccrithurt(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Deccrithurt(this->object, this->ratio, this->time, this->buffid));
		}
		return true;
	}

	//109
	bool PlayerWrapper::SetDecdamage(bool)
	{
		object.AddFilter(new filter_Decdamage(this->object, this->ratio, this->probability, this->time));
		return true;
	}

	//110
	int PlayerWrapper::GetCultivation(void)
	{
		return object.GetCultivation();
	}

	//111
	int PlayerWrapper::GetDp(void)
	{
		return object.GetBasicProp().dp;
	}

	//112
	bool PlayerWrapper::SetMagicdoor(bool)
	{
		if (ThrowDice() && object.CanReturnToTown())
		{
			A3DVECTOR vec(value, amount, buffid);
			object.Teleport(this->time, vec);
		}
		return true;
	}

	//113
	bool PlayerWrapper::SetPasdechurt3(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultDefense(2, -dec * 1000.0);
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceCultDefense(2, dec * 1000.0);
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return true;
	}

	//114
	bool PlayerWrapper::SetFamilyincattack(bool)
	{
		object.AddFilter(new filter_Familyincattack(object, ratio * 1000.0, value, time));
		return true;
	}

	//115
	bool PlayerWrapper::SetIncfatalratio(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incfatalratio(this->object, this->ratio * 1000.0, this->time, this->buffid));
		}
		return 1;
	}

	//116
	bool PlayerWrapper::SetActiveonfilteradd(bool)
	{
		object.AddFilter(new filter_Activeonfilteradd(object, time, probability, ratio, value, amount, buffid,skill->GetForceattack()));
		return 1;
	}

	//117
	bool PlayerWrapper::SetCrazy(bool)
	{
		object.AddFilter(new filter_Crazy(object, ratio, amount, value, time));
		return 1;
	}

	//118
	bool PlayerWrapper::SetAdddefence(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Adddefence(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//119
	bool PlayerWrapper::SetHupo(bool)
	{
		if (!IsMultiFilterExist(MULTI_FILTER_VIPHUPO) && ThrowDice())
		{
			object.AddFilter(new filter_Hupo(object, ratio * 1000.0, value, time, buffid));
		}
		return 1;
	}

	//120
	bool PlayerWrapper::SetImmuneweak(bool)
	{
		object.AddFilter(new filter_Immuneweak(this->object, this->time));
		return 1;
	}

	//121
	bool PlayerWrapper::SetRepel2(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEREPEL) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				object.Repel2(skill->GetAttackerpos(), value);
			}
		}
		return 1;
	}

	//122
	bool PlayerWrapper::GetHasbuff(int filter_id)
	{
		return IsFilterExist(filter_id);
	}

	//123
	bool PlayerWrapper::SetGuishen2debuff(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Guishen2debuff(object, time, ratio * 100.0, amount * 1000.0, value * 100.0, buffid));
		}
		return 1;
	}

	//124
	bool PlayerWrapper::SetHpleak1(bool)
	{
		if (! ThrowDice())
			return 0;
		float Amount = GetAmount();
		int damage = (int)Amount;
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate =  atstate | IMMUNEWEAK;
		}
		else if (damage > 1)
		{
			if (! object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish(skill->GetAttackerlevel(), GetLevel());
				damage = (int)(Amount * (long double)damage);
			}
			filter_Hpleak1* fhpl = new filter_Hpleak1(this->object, this->time, damage, (int)this->value, this->buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return 1;
	}

	//125
	bool PlayerWrapper::SetSubantiweak(bool)
	{
		object.AddFilter(new filter_Subantiweak(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//126
	bool PlayerWrapper::SetPassubhurt(float dec)
	{
		if (this->context == 3)
			object.ImpairDmgReduce(dec);
		else
			object.EnhanceDmgReduce(dec);
		return 1;
	}

	//127
	bool PlayerWrapper::IsMaster(void)
	{
		return object.IsMaster();
	}

	//128
	void PlayerWrapper::SetVar8(int v)
	{
		SetVar( v, 7);
	}

	//129
	int PlayerWrapper::GetInk(void)
	{
		return object.GetBasicProp().ink;
	}

	//130
	void PlayerWrapper::SetRage(int v)
	{
		object.SetRage(v);
	}

	//131
	void PlayerWrapper::SetTalentData(int* list)
	{
		if (this->skill)
		{
			list[0] = skill->GetT0();
			list[1] = skill->GetT1();
			list[2] = skill->GetT2();
			list[3] = skill->GetT3();
			list[4] = skill->GetT4();
			list[5] = skill->GetT5();
			list[6] = skill->GetT6();
			list[7] = skill->GetT7();
		}
	}

	//132
	bool PlayerWrapper::SetPasinchpgen(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleHPGen(inc * 1000.0);
			object.UpdateHPMPGen();
		}
		else
		{
			object.EnhanceScaleHPGen(inc * 1000.0);
			if (this->context == 4)
				object.UpdateHPMPGen();
		}
		return 1;
	}

	//133
	bool PlayerWrapper::SetDechurt2(bool)
	{
		if (this->ratio > 0.001 && this->ratio <= 1.0)
		{
			object.AddFilter(new filter_Dechurt2(this->object, this->ratio, (int)this->amount, this->time));
		}
		return 1;
	}

	//134
	bool PlayerWrapper::SetIncskillaccu(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incskillaccu(object, ratio * 1000.0, time, buffid));
		}
		return 1;
	}

	//135
	bool PlayerWrapper::SetPasaddaccuracy(float addend)
	{
		if (this->context == 3)
		{
			object.ImpairAttack(addend);
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceAttack(addend);
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//136
	bool PlayerWrapper::SetSummonskill(bool)
	{
		if (this->tsize < 0 || !this->target)
			return 0;
		object.AddFilter(new filter_Summonskill(object, (int)value, (int)amount, (int)ratio, *target));
		return 1;
	}

	//137
	bool PlayerWrapper::SetImmunesilent(bool)
	{
		object.RemoveFilter( FILTER_SILENT);
		object.AddFilter(new filter_Immunesilent(object,time));
		return 1;
	}

	//138
	bool PlayerWrapper::GetHasmultbuff(int filter_id)
	{
		return IsMultiFilterExist(filter_id);
	}

	//139
	void PlayerWrapper::SetSkill(Skill* s)
	{
		this->skill = s;
	}

	//140
	bool PlayerWrapper::SetPasaddslow(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistance( 5, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance( 5, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//141
	bool PlayerWrapper::CheckItem(int inv_index, int item_id, uint count)
	{
		return object.CheckItem(inv_index, item_id, count);
	}

	//142
	float PlayerWrapper::GetAmount(void)
	{
		return this->amount;
	}

	//143
	bool PlayerWrapper::SetSummonclone(bool)
	{
		if (ThrowDice())
		{
			object.CreateClone(value,(int)this->amount,time,(int)this->ratio, skill->GetLevel(), buffid);
		}
		return 1;
	}

	//144
	bool PlayerWrapper::SetReduceskillcd(bool)
	{
		if (ThrowDice())
		{
			if ((int)this->ratio > 0)
				object.ReduceSkillCooldown((int)this->ratio, (int)this->value);
			if ((int)this->amount > 0)
				object.ReduceSkillCooldown((int)this->amount, (int)this->value);
		}
		return 1;
	}

	//145
	int PlayerWrapper::GetCrit(void)
	{
		return object.GetExtendProp().crit_rate;
	}

	//146
	bool PlayerWrapper::SetJuniordarkform(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Juniordarkform(this->object, this->ratio, (int)this->value));
		}
		return 1;
	}

	//147
	bool PlayerWrapper::SetCrazycurse(bool)
	{
		if (!ThrowDice())
			return 0;
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | IMMUNEWEAK;
		}
		else if ((int)this->ratio > 0 && (int)this->amount > 0)
		{
			filter_Crazycurse *fcc = new filter_Crazycurse(this->object, (int)this->ratio, (int)this->amount, this->time);
			fcc->SetUp( skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fcc);
		}
		return 1;
	}

	//148
	bool PlayerWrapper::SetDecProficiency(uint skillid, int dec)
	{
		return object.GetSkillWrapper().DecProficiency(object, skillid, dec);
	}

	//149
	bool PlayerWrapper::SetInvader(bool b)
	{
		this->invader = b;
		return 1;
	}

	//150
	bool PlayerWrapper::SetClearwrap(bool)
	{
		if (ThrowDice())
			object.RemoveFilter(FILTER_WRAP);
		return 1;
	}

	//151
	bool PlayerWrapper::SetPuppetform(bool)
	{
		if (object.GetShape())
		{
			object.RemoveFilter(FILTER_PUPPETFORM);
			return 0;
		}
		else if (object.GetBasicProp().mp >= (int)this->ratio)
		{
			object.AddFilter(new filter_Puppetform(this->object, 0, (int)this->value, (int)this->ratio));
			return 1;
		}
		else
		{
			return 0;
		}
	}

	//152
	bool PlayerWrapper::SetGohome(bool)
	{
		if (ThrowDice())
		{
			this->result |= 1u;
			if (object.CanReturnToTown())
			{
				object.ReturnToTown();
				this->result &= ~1u;
			}
		}
		return 1;
	}

	//153
	bool PlayerWrapper::SetSharelifewithsummon(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Sharelifewithsummon(this->object, this->ratio, this->time));
		}
		return 1;
	}

	//154
	int PlayerWrapper::GetRes5(void)
	{
		return object.GetExtendProp().resistance[4];
	}

	//155
	int PlayerWrapper::GetMaxmp(void)
	{
		return object.GetExtendProp().max_mp;
	}

	//156
	bool PlayerWrapper::SetGtmpgen(bool)
	{
		object.AddFilter(new filter_Gtmpgen(this->object, (int)this->value, (int)this->amount, this->buffid));
		return 1;
	}

	//157
	bool PlayerWrapper::SetSilenttimer(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Silenttimer(this->object, this->time, (int)this->value, (int)this->amount));
		}
		return 1;
	}

	//158
	bool PlayerWrapper::SetSubattack(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subattack(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//159
	float PlayerWrapper::GetProbability(void)
	{
		return this->probability;
	}

	//160
	bool PlayerWrapper::SetCrippleddebuff(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Crippleddebuff(this->object, this->time, (int)this->value));
		}
		return 1;
	}

	//171
	bool PlayerWrapper::SetSpytrap(bool)
	{
		if (ThrowDice())
			object.SpyTrap();
		return 1;
	}

	//172
	bool PlayerWrapper::SetFamilyincmp(bool)
	{
		object.AddFilter(new filter_Familyincmp(this->object, (int)(this->ratio * 1000.0), (int)this->value, this->time));
		return 1;
	}

	//173
	int PlayerWrapper::GetHp(void)
	{
		return object.GetBasicProp().hp;
	}

	//174
	bool PlayerWrapper::SetPasdecrestrainfo(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultAttack(2, -(int)(dec * 1000.0));
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceCultAttack(2, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//175
	bool PlayerWrapper::SetSubdefence(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subdefence(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//176
	int PlayerWrapper::GetPuppetid(void)
	{
		return object.GetPuppetid();
	}

	//177
	int PlayerWrapper::GetAtstate(void)
	{
		return this->atstate;
	}

	//178
	void PlayerWrapper::RemoveMultiFilter(int id)
	{
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+1);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+2);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+3);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+4);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+5);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+6);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+7);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+8);
		object.RemoveFilter( 10 * id + MULTI_FILTER_BEGIN+9);
	}

	//179
	bool PlayerWrapper::SetBuffid(int d)
	{
		this->buffid = d;
		return 1;
	}

	//180
	bool PlayerWrapper::SetMakecrit(bool)
	{
		object.AddFilter(new filter_Makecrit(this->object, this->time));
		return 1;
	}

	//181
	bool PlayerWrapper::SetAddattack3(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addattack3( this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//182
	bool PlayerWrapper::SetCursed(bool)
	{
		if (this->context == 1)
		{
			if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				object.AddFilter(new filter_Cursed(this->object, (int)this->value, (int)this->amount, this->time));
			}
		}
		return 1;
	}

	//183
	bool PlayerWrapper::SetRejectdebuff(bool)
	{
		object.AddFilter(new filter_Rejectdebuff(this->object, (int)this->probability, this->time));
		return 1;
	}

	//184
	void PlayerWrapper::CalcCost(void)
	{
		if (skill->GetMpcost() > 0)
		{
			UseMp(skill->GetMpcost());
		}
		if (skill->GetDpcost() > 0)
		{
			UseDp(skill->GetDpcost());
		}
		if (skill->GetHpcost() > 0)
		{
			UseHp(skill->GetHpcost());
		}
		if (skill->GetInkcost() > 0)
		{
			UseInk(skill->GetInkcost());
		}
	}

	//185
	void PlayerWrapper::DoExoricism(int id)
	{
		switch (id)
		{
		case 0:
			object.RemoveFilter(FILTER_DRUNK);
			break;
		case 1:
			object.RemoveFilter(FILTER_SPICY);
			break;
		case 2:
			RemoveMultiFilter(MULTI_FILTER_HPLEAK);
			break;
		case 3:
			RemoveMultiFilter(MULTI_FILTER_MPLEAK);
			break;
		case 4:
			object.RemoveFilter(FILTER_DIET);
			break;
		case 5:
			object.RemoveFilter(FILTER_RETORT);
			break;
		case 6:	  
			object.RemoveFilter(FILTER_ALOOF);
			break;
		case 7:
			RemoveMultiFilter(MULTI_FILTER_HPLEAK1);
			break;
		case 8:
			RemoveMultiFilter(MULTI_FILTER_HPLEAK2);
			break;
		case 9:
			RemoveMultiFilter(MULTI_FILTER_HPLEAK3);
			break;
		case 10:
			object.RemoveFilter(FILTER_DEATHSCATTER);
			break;
		case 11:
			object.RemoveFilter(FILTER_REJECTDEBUFF);
			break;
		case 12:
			object.RemoveFilter(FILTER_BLESSED);
			break;
		case 13:
			object.RemoveFilter(FILTER_MAGICSHIELD);
			break;
		case 14:
			object.RemoveFilter(FILTER_DECHURT);
			break;
		case 15:
			object.RemoveFilter(FILTER_BAREHANDED);
			break;
		case 16:
			RemoveMultiFilter(MULTI_FILTER_BLEEDING);
			break;
		case 17:
			RemoveMultiFilter(MULTI_FILTER_INCSKILLACCU);
			break;
		case 18:
			RemoveMultiFilter(MULTI_FILTER_DECSKILLACCU);
			break;
		case 19:
			RemoveMultiFilter(MULTI_FILTER_HPLEAK6);
			break;
		default:
			return;
		}
	}

	//186
	bool PlayerWrapper::UseMp(int dec)
	{
		if (IsRenMa())
		{
			UseHp(dec);
			return 1;
		}
		else
		{
			int mp = object.GetBasicProp().mp;
			if (mp < dec)
				return object.DrainMana(mp);
			else
				return object.DrainMana(dec);
		}
	}

	//187
	int PlayerWrapper::GetVar1(void)
	{
		return this->var[0];
	}

	//188
	bool PlayerWrapper::SetHuju(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Huju(object, amount * 1000.0, value * 1000.0));
		}
		return 1;
	}

	//189
	bool PlayerWrapper::SetCreateobject(bool b)
	{
		int move_state; // [esp+38h] [ebp-10h]

		if (this->buffid == 1)
			move_state = 1;
		else
			move_state = 2;
		if (ThrowDice())
		{
			if (skill->GetRange() == 5)
			{
				object.CreateSkillObject(amount,1,time, skill->GetLevel(),ratio,value,move_state);
			}
			else
			{
				A3DVECTOR pos;
				float bodysize = 0.0;
				if (object.QueryObject(object.GetCurTargetID(), pos, bodysize))
				{
					object.CreateSkillObject((int)this->amount, 1, this->time, skill->GetLevel(), pos, move_state);
				}
			}
		}
		return 1;
	}

	//190
	bool PlayerWrapper::SetSubantidizzy(bool)
	{
		object.AddFilter(new filter_Subantidizzy( this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//191 
	int64_t PlayerWrapper::GetDTExp(void)
	{
		return object.GetBasicProp().dt_exp;
	}

	//192
	bool PlayerWrapper::SetDie(bool b)
	{
		if (b)
			object.Die();
		return 1;
	}

	//193
	bool PlayerWrapper::SetAdddodge2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Adddodge2(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//194
	bool PlayerWrapper::SetDrawto(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEREPEL) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				object.DrawTo( skill->GetAttackerid(), skill->GetAttackerpos());
			}
		}
		return 1;
	}

	//195
	bool PlayerWrapper::SetSubantisleep(bool)
	{
		object.AddFilter(new filter_Subantisleep(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//196
	bool PlayerWrapper::SetActivebecrit(bool)
	{
		object.AddFilter(new filter_Activebecrit(object, time, probability, value, amount, buffid, skill->GetForceattack()));
		return 1;
	}

	//197
	bool PlayerWrapper::CanAttack(void)
	{
		XID xid(-1, -1);
		return object.CanAttack(xid);
	}

	//198
	void PlayerWrapper::SetVar3(int v)
	{
		SetVar( v, 2);
	}

	//199
	bool PlayerWrapper::SetSubantislow(bool)
	{
		object.AddFilter(new filter_Subantislow( this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//200
	bool PlayerWrapper::SetHeal(bool)
	{
		if (ThrowDice())
		{
			bool b = this->ratio > 0.001;
			if (this->context)
			{
				object.Heal(skill->GetAttackerid(), value, b);
			}
			else
			{
				object.Heal((int)this->value, 0, b);
			}
			this->health = (int)this->value;
		}
		return 1;
	}

	//201
	void PlayerWrapper::SetDarklight(int v)
	{
		object.SetDarkLight(v);
	}

	//202
	int PlayerWrapper::GetDmgreduce(void)
	{		
		return object.GetScaleEnhanecdParam().dmg_reduce;
	}

	//203
	int PlayerWrapper::GetSkillrate(void)
	{
		return object.GetExtendProp().skill_attack_rate;
	}

	//204
	int PlayerWrapper::GetMp(void)
	{
		return object.GetBasicProp().mp;
	}

	//205
	bool PlayerWrapper::SetBackorigin(bool)
	{
		if (ThrowDice()
			&& object.CanTransform(value))
		{
			object.TransformMonster(value, time);
		}
		return 1;
	}

	//206
	bool PlayerWrapper::SetCleartransform(bool)
	{
		if (ThrowDice())
		{
			int transid = object.GetTransformID();
			if (transid)
			{
				if (!(int)this->value && !(int)this->amount && !(int)this->ratio
					|| transid == (int)this->value
					|| transid == (int)this->amount
					|| transid == (int)this->ratio)
				{
					object.StopTransform(transid, 1);
				}
			}
		}
		return 1;
	}

	//207
	bool PlayerWrapper::SetSharelifewithclone(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Sharelifewithclone(this->object, this->ratio, this->time));
		}
		return 1;
	}

	//208
	bool PlayerWrapper::GetIsfrozen(void)
	{
		return object.IsColdInjureState();
	}

	//209
	bool PlayerWrapper::SetXinmo(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Xinmo( this->object, this->ratio, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//210
	bool PlayerWrapper::SetAddcommon(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addcommon(object, time, ratio, value, amount, buffid, var));
		}
		return 1;
	}

	//211
	bool PlayerWrapper::SetDelaycast(bool)
	{
		if (ThrowDice())
		{
			filter_Delaycast *fdel = new filter_Delaycast(object,time,value,ratio,buffid, skill->GetForceattack(),var);
			fdel->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader, 
				skill->GetAttackerFaction(), skill->GetTargetFaction());
			object.AddFilter(fdel);
		}
		return 1;
	}

	//212
	bool PlayerWrapper::SetAddspirit(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addspirit(this->object, (int)this->ratio, (int)this->amount));
		}
		return 1;
	}

	//213
	bool PlayerWrapper::SetLongxiang(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Longxiang(this->object, (int)(this->amount * 1000.0), (int)(this->value * 1000.0)));
		}
		return 1;
	}

	//214
	bool PlayerWrapper::SetSubhp2(bool)
	{
		if (!object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subhp2( this->object, (int)this->value, this->time, (int)this->amount));
		}
		return 1;
	}

	//215
	bool PlayerWrapper::SetInchp2(bool)
	{
		object.AddFilter(new filter_Inchp2(object, (int)(this->ratio * 1000.0), time, buffid));
		return 1;
	}

	//216
	bool PlayerWrapper::SetInstantskill(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Instantskill(this->object, this->time, (int)this->value));
		}
		return 1;
	}

	//217
	bool PlayerWrapper::SetHpleak6(bool)
	{
		if (! ThrowDice())
			return 0;
		float Amount = GetAmount();
		int damage = (int)Amount;
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | 0x80;
		}
		else if (damage > 1)
		{
			if (! object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish(skill->GetAttackerlevel(), GetLevel());
				damage = (int)(Amount * (long double)damage);
			}
			filter_Hpleak6 *fhpl = new filter_Hpleak6( this->object, this->time, damage, (int)this->value, this->buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return 1;
	}

	//218
	bool PlayerWrapper::SetGuishen1(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Guishen1(object, time, ratio * 100.0, amount * 1000.0, value * 100.0, buffid));
		}
		return 1;
	}

	//219
	bool PlayerWrapper::SetAdddebuff(bool)
	{
		AddCommonDebuff(skill->GetT0(), skill->GetT1(), skill->GetT2(), skill->GetT3(), 1);
		return 1;
	}

	//220
	bool PlayerWrapper::SetDrainmagic(bool)
	{
		if (this->context == 1 && ThrowDice())
		{
			UseMp(GetMaxmp() * this->ratio);
		}
		return 1;
	}

	//221
	bool PlayerWrapper::SetPasincmp(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleMaxMP((int)(inc * 1000.0));
			object.UpdateHPMP();
		}
		else
		{
			object.EnhanceScaleMaxMP((int)(inc * 1000.0));
			if (this->context == 4)
				object.UpdateHPMP();
		}
		return 1;
	}

	//222
	bool PlayerWrapper::SetBlessed(bool)
	{
		if ((int)this->value > 0)
		{
			object.AddFilter(new filter_Blessed(this->object, (int)this->value, this->time));
		}
		return 1;
	}

	//223
	bool PlayerWrapper::SetStateexchangewithclone(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Stateexchangewithclone(this->object));
		}
		return 1;
	}

	//224
	bool PlayerWrapper::SetPasincattack(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleDamage((int)(inc * 1000.0));
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceScaleDamage((int)(inc * 1000.0));
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//225
	bool PlayerWrapper::SetChihun(bool)
	{
		if (ThrowDice())
		{
			object.RemoveFilter(FILTER_SLEEP);
			object.RemoveFilter(FILTER_SILENT);
			object.RemoveFilter(FILTER_WRAP);
			RemoveMultiFilter(MULTI_FILTER_DECATTACK);
			object.RemoveFilter(FILTER_DIZZY);
			RemoveMultiFilter(MULTI_FILTER_SLOW);
			RemoveMultiFilter(MULTI_FILTER_SLOW2);
			DoExoricism(2);
			DoExoricism(7);
			DoExoricism(8);
			DoExoricism(9);
			DoExoricism(17);
			DoExoricism(18);
			DoExoricism(19);
			DoExoricism(3);
			DoExoricism(4);
			object.AddFilter(new filter_Chihun(object, time, (int)(value * 1000.0), (int)(amount * 1000.0), buffid));
		}
		return 1;
	}

	//226
	bool PlayerWrapper::SetTransform(bool)
	{
		int atstate; // edx
		object_interface* v4; // [esp-Ch] [ebp-24h]

		this->probability = this->probability - object.GetExtendProp().anti_transform;
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNETRANSFORM) != 0)
			{
				atstate =  atstate | IMMUNEWEAK;
			}
			else
			{
				object.StartTransform((int)value,(int)amount,(int)ratio,time,1);
			}
		}
		else
		{
			this->atstate |= IMMUNEREPEL;
		}
		return 1;
	}

	//227
	bool PlayerWrapper::SetPulling(bool)
	{
		if (!this->target)
			return 0;
		if ((object.GetImmuneMask() & IMMUNEPULL) != 0)
		{
			atstate = atstate | IMMUNEWEAK;
		}
		else
		{
			object.AddFilter(new filter_Pulling(this->object, this->time, *this->target));
		}
		return 1;
	}

	//228
	bool PlayerWrapper::SetPasincmpgen(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleMPGen((int)(inc * 1000.0));
			object.UpdateHPMPGen();
		}
		else
		{
			object.EnhanceScaleMPGen((int)(inc * 1000.0));
			if (this->context == 4)
				object.UpdateHPMPGen();
		}
		return 1;
	}

	//229
	bool PlayerWrapper::SetPasdecpethurt(float dec)
	{
		if (this->context == 3)
			object.EnhancePetAttackDefense(-(int)(dec * 1000.0));
		else
			object.EnhancePetAttackDefense((int)(dec * 1000.0));
		return 1;
	}

	//230
	float PlayerWrapper::GetSpeed(void)
	{
		return object.GetExtendProp().run_speed;
	}

	//231
	float PlayerWrapper::GetValue(void)
	{
		return this->value;
	}

	//232
	bool PlayerWrapper::GetInrange(float range, float& correction, bool corpseonly)
	{
		A3DVECTOR tpos;
		if (!this->target || !this->tsize)
			return 0;
		float tbody;
		int ret = object.QueryObject(*this->target, tpos, tbody);
		if (!ret)
			return 0;
		if (corpseonly && ret != 2)
			return 0;
		correction = object.GetBodySize() + tbody;
		return (object.GetBodySize() + range + tbody) * (object.GetBodySize() + range + tbody) > object.GetPos().squared_distance(tpos);
	}

	//233
	bool PlayerWrapper::SetCycle(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Cycle(object, time, value, amount, ratio,
				skill->GetRadius(), buffid,skill->GetForceattack()));
		}
		else
		{
			object.CircleOfDoomStop();
		}
		return 1;
	}

	//234
	bool PlayerWrapper::SetBarhpgen(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Barhpgen(this->object, (int)this->value, (int)this->amount, this->buffid));
		}
		return 1;
	}

	//235
	bool PlayerWrapper::SetIncskilllevel(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incskilllevel(object, time, value, amount, ratio, buffid));
		}
		return 1;
	}

	//236
	bool PlayerWrapper::SetAddwrap(bool)
	{
		if (object.IsFilterExist(4200))
		{
			object.RemoveFilter(4200);
		}
		else
		{
			object.AddFilter(new filter_Addwrap(object, probability, value, amount, ratio));
		}
		return 1;
	}

	//237
	bool PlayerWrapper::SetAddattack2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addattack2(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//238
	bool PlayerWrapper::SetDeccritrate(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Deccritrate(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//239
	bool PlayerWrapper::SetInchp(bool)
	{
		object.AddFilter(new filter_Inchp(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		return 1;
	}

	//240
	bool PlayerWrapper::SetDecskilldodge(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Decskilldodge(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//241
	int PlayerWrapper::GetRace(void)
	{
		if (object.GetObjectType())
			return 0;
		if (object.GetClass() > 31)
			return 2;
		else
			return 1;
	}

	//242
	bool PlayerWrapper::SetIncattack3(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incattack3(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//243
	bool PlayerWrapper::SetActivecrit(bool)
	{
		object.AddFilter(new filter_Activecrit(object, time, probability, value, amount, buffid,skill->GetForceattack()));
		return 1;
	}

	//244
	void PlayerWrapper::SetResisProf(int idx, int v)
	{
		this->resistance_proficiency[idx] = v;
	}

	//245
	bool PlayerWrapper::SetFly(bool)
	{
		if (object.CheckCanFly() && ThrowDice())
		{
			object.PreFly();
			object.AddFilter(new filter_Fly(this->object, this->time, this->value));
		}
		return 1;
	}

	//246
	bool PlayerWrapper::SetBepulled(bool)
	{
		if (ThrowDice()
			&& !object.IsFilterExist(4210)
			&& !object.IsFilterExist(4220)
			&& !object.IsFilterExist(4218))
		{
			filter_Pull *fpul = new filter_Pull(this->object, this->time, this->value, this->amount);
			fpul->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fpul);
		}
		return 1;
	}

	//247
	bool PlayerWrapper::SetAddhp(bool)
	{
		object.AddFilter(new filter_Addhp(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//248
	bool PlayerWrapper::SetPasaddmp(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairMaxMP((int)inc);
			object.UpdateHPMP();
		}
		else
		{
			object.EnhanceMaxMP((int)inc);
			if (this->context == 4)
				object.UpdateHPMP();
		}
		return 1;
	}

	//249
	bool PlayerWrapper::SetAddantiweak(bool)
	{
		object.AddFilter(new filter_Addantiweak( this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//250
	bool PlayerWrapper::SetIncanti(bool)
	{
		object.AddFilter(new filter_Incanti(this->object, (int)(this->ratio * 1000.0), this->time));
		return 1;
	}

	//251
	bool PlayerWrapper::SetPasaddwrap(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistance(2, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance(2, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//252
	bool PlayerWrapper::SetFamilyinchp(bool)
	{
		object.AddFilter(new filter_Familyinchp(this->object, (int)(this->ratio * 1000.0), (int)this->value, this->time));
		return 1;
	}

	//253
	int PlayerWrapper::GetBuffcnt(void)
	{
		return object.FilterCnt( 0x20000000);  //FILTER_MASK_BUFF
	}

	//254
	bool PlayerWrapper::SetSetcommondata(bool)
	{
		if (ThrowDice())
			object.PutGlobalValue((int)this->amount, (int)this->value);
		return 1;
	}

	//255
	bool PlayerWrapper::SetAccumdamage(bool)
	{
		if (!object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			filter_Accumdamage *facc = new filter_Accumdamage(this->object, this->ratio, (int)this->amount, this->time);
			facc->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(facc);
		}
		return 1;
	}

	//256
	bool PlayerWrapper::SetSpicy(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Spicy(this->object, (int)this->value, (int)this->amount, this->time));
		}
		return 1;
	}

	//257
	bool PlayerWrapper::SetAsfire(bool)
	{
		if (object.IsFilterExist(FILTER_ASFIRE))
		{
			object.RemoveFilter(FILTER_ASFIRE);
		}
		else
		{
			object.AddFilter(new filter_Asfire(this->object, (int)this->amount, (int)this->value));
		}
		return 1;
	}

	//258
	int PlayerWrapper::GetVar2(void)
	{
		return this->var[1];
	}

	//259
	const XID& PlayerWrapper::GetXid(void)
	{
		return object.GetSelfID();
	}

	//260
	bool PlayerWrapper::SetAddslow(bool)
	{
		if (object.IsFilterExist(FILTER_ADDSLOW))
		{
			object.RemoveFilter(FILTER_ADDSLOW);
		}
		else
		{
			object.AddFilter(new filter_Addslow(object, probability, value, amount, ratio));
		}
		return 1;
	}

	//261
	int PlayerWrapper::GetMaxhp(void)
	{
		return object.GetExtendProp().max_hp;
	}

	//262
	bool PlayerWrapper::SetPasaddhp(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairMaxHP((int)inc);
			object.UpdateHPMP();
		}
		else
		{
			object.EnhanceMaxHP((int)inc);
			if (this->context == 4)
				object.UpdateHPMP();
		}
		return 1;
	}

	//263
	bool PlayerWrapper::SetDechurt(bool)
	{
		if (this->ratio > 0.001 && this->ratio <= 1.001)
		{
			object.AddFilter(new filter_Dechurt( this->object, this->ratio, (int)this->amount, this->time));
		}
		return 1;
	}

	//264
	bool PlayerWrapper::SetDouhun(bool)
	{
		if (object.IsFilterExist(FILTER_DOUHUN))
		{		  
			object.RemoveFilter(FILTER_DOUHUN);
		}
		else
		{
			object.AddFilter(new filter_Douhun(object, ratio, (int)(amount * 1000.0), amount * 10.0, value));
		}
		return 1;
	}

	//265
	bool PlayerWrapper::SetAddantidizzy(bool)
	{
		object.AddFilter(new filter_Addantidizzy(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//266
	bool PlayerWrapper::SetJuqi(bool)
	{
		if (!this->context)
		{
			this->atstate |= 8;   //unknown state define    haokaixin
			object.SetNextAttackState(8);
			object.Heal((int)this->value, 0, 0);
			this->health = (int)this->value;
		}
		return 1;
	}

	//267
	int PlayerWrapper::GetSkilldodge(void)
	{
		return object.GetExtendProp().skill_armor_rate;
	}

	//268
	bool PlayerWrapper::SetAoshi(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Aoshi(object, ratio * 1000.0, amount, value, time, buffid));
		}
		return 1;
	}

	//269
	bool PlayerWrapper::SetPasdechurt2(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultDefense(1, -(int)(dec * 1000.0));
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceCultDefense(1, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//270
	bool PlayerWrapper::SetAdddodge(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Adddodge(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//271
	int PlayerWrapper::GetVar7(void)
	{
		return this->var[6];
	}

	//272
	bool PlayerWrapper::SetFocusanti(bool)
	{
		object.AddFilter(new filter_Focusanti(this->object, this->ratio, (int)this->amount, (int)this->value, this->time));
		return 1;
	}

	//273
	bool PlayerWrapper::SetAddanti2(bool)
	{
		object.AddFilter(new filter_Addanti2(this->object, (int)this->value, this->time));
		return 1;
	}

	//274
	bool PlayerWrapper::SetGuishen2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Guishen2(object, time, ratio * 100.0, amount * 1000.0, value * 100.0, buffid));
		}
		return 1;
	}

	//275
	bool PlayerWrapper::SetSpeedto(bool)
	{
		float speed = object.GetExtendProp().run_speed;
		if (speed < value && ThrowDice())
		{
			object.AddFilter(new filter_Speedto(this->object, this->time, value - speed, this->buffid));
		}
		return 1;
	}

	//276
	bool PlayerWrapper::SetYanyu(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Yanyu(object, ratio * 1000.0, value, time, buffid));
		}
		return 1;
	}

	//277
	bool PlayerWrapper::SetBanruo(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Banruo(object, ratio * 1000.0, amount, value, time, buffid));
		}
		return 1;
	}

	//278
	bool PlayerWrapper::SetSetcooldown(bool)
	{
		if (ThrowDice())
		{
			object.SetCoolDown((int)this->ratio + 1024, 1000 * this->time);
			object.SetCoolDown((int)this->amount + 1024, 1000 * this->time);
			object.SetCoolDown((int)this->value + 1024, 1000 * this->time);
		}
		return 1;
	}

	//279
	bool PlayerWrapper::SetPasdecrestrainxian(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultAttack( 0, -(int)(dec * 1000.0));
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceCultAttack(0, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//280
	bool PlayerWrapper::SetWeak(bool)
	{
		if (this->probability < 0.001)
			return 0;
		if (ThrowDice(1))
			AddCommonDebuff( 2, this->time, (int)(this->ratio * 1000.0), this->buffid, 0);
		else
			this->atstate |= 4;  //IMMUNEREPEL
		return 1;
	}

	//281
	bool PlayerWrapper::SetHp(float hp)
	{
		float h = object.GetBasicProp().hp - hp;
		if (h > 0)
			object.DecHP(h);
		return 1;
	}

	//282
	bool PlayerWrapper::SetTime(float t)
	{
		this->time = (int)(t * 0.001 + 0.5);
		if (!this->time)
			this->time = 1;
		return 1;
	}

	//283
	bool PlayerWrapper::SetCycdecdefence(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Cycdecdefence(object, (int)(this->ratio * 1000.0), time, buffid));
		}
		return 1;
	}

	//284
	bool PlayerWrapper::SetXiaoxin(bool)
	{
		if (this->context == 1 && ThrowDice())
		{
			this->atstate |= 8;  //IMMUNEDRAINMAGIC
			UseMp(GetMaxmp() * this->ratio);
		}
		return 1;
	}

	//285
	bool PlayerWrapper::ThrowDice(void)
	{
		if (this->probability > 99.0)
			return 1;
		if (this->probability < 0.001)
			return 0;
		float r = rand() % 100;
		float p;
		probability = this->probability;
		if (probability <= r)
			p = 0.0;
		else
			p = 100.0;
		this->probability = p;
		return probability > r;
	}

	//286
	int PlayerWrapper::GetRes3(void)
	{
		return object.GetExtendProp().resistance[2];
	}

	//287
	bool PlayerWrapper::SetAbsulotearea(bool)
	{
		if (!this->skill)
			return 0;
		this->result |= 2u;
		if (object.IsFilterExist(FILTER_HOLYAURA))
		{
			object.RemoveFilter(FILTER_HOLYAURA);
			return 1;
		}
		else
		{
			if (object.IsFilterExist(FILTER_EVILAURA))
			{
				object.RemoveFilter(FILTER_EVILAURA);
			}
			else
			{
				object.SetAbsoluteZone(skill->GetId(),skill->GetLevel(),skill->GetRadius(),
					skill->GetCoverage(),skill->GetMpcost(),skill->GetForceattack(),
					value,amount,probability,ratio);
			}
			return 1;
		}
	}

	//288
	bool PlayerWrapper::SetPasaddsilent(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistance(3, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance(3, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//289
	bool PlayerWrapper::SetClearsleep(bool)
	{
		if (ThrowDice())
			object.RemoveFilter(FILTER_SLEEP);
		return 1;
	}

	//290
	bool PlayerWrapper::SetCycleskill(bool)
	{
		this->result |= 2; //IMMUNEBLIND
		if (ThrowDice())
		{
			if (IsFilterExist(4270))
			{
				object.RemoveFilter(4270);
			}
			else
			{
				object.AddFilter(new filter_Cycleskill(object,skill->GetId(),skill->GetLevel(),
					skill->GetType(),skill->GetRadius(),skill->GetCoverage(),skill->GetMpcost(),
					skill->GetHpcost(),skill->GetDpcost(),skill->GetInkcost(),skill->GetForceattack(),
					ratio,value,time,var, amount * 1000.0));
			}
		}
		return 1;
	}

	//291
	bool PlayerWrapper::SetRandcurse(bool)
	{
		if (!ThrowDice())
			return 1;
		int type = rand() % 6;
		if (this->context == 1)
		{
			switch (type)
			{
			case 0:
				if ((object.GetImmuneMask() & IMMUNEDIZZY) != 0)
				{
					atstate = atstate | IMMUNEWEAK;
				}
				else
				{
					AddCommonDebuff(0, this->time, 0, 0, 0);
				}
				break;
			case 1:
				if ((object.GetImmuneMask() & IMMUNESLEEP) != 0)
				{
					this->atstate  = atstate | IMMUNEWEAK;
				}
				else
				{
					AddCommonDebuff(1, this->time, 0, 0, 0);
				}
				break;
			case 2:
				AddCommonDebuff(2, this->time, (int)(this->value * 1000.0), this->buffid, 0);
				break;
			case 3:
				if ((object.GetImmuneMask() & IMMUNEWRAP) != 0)
				{
					atstate = atstate | IMMUNEWEAK;
				}
				else
				{
					AddCommonDebuff(3, this->time, 0, 0, 0);
				}
				break;
			case 4:
				if ((object.GetImmuneMask() & IMMUNESLOW) != 0)
				{
					atstate = atstate | IMMUNEWEAK;
				}
				else
				{
					AddCommonDebuff(4, this->time, (int)(this->ratio * 1000.0), this->buffid, 0);
				}
				break;
			case 5:
				if ((object.GetImmuneMask() & IMMUNESILENT) != 0)
				{
					atstate = atstate | IMMUNEWEAK;
				}
				else
				{
					AddCommonDebuff( 5, this->time, 0, 0, 0);
				}
				break;
			default:
				return 1;
			}
			return 1;
		}
		return 0;
	}

	//292
	bool PlayerWrapper::CheckItem(int item_id, uint count)
	{
		return object.CheckItem(item_id, count);
	}

	//293
	bool PlayerWrapper::SetPasincskilldodge(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceSkillArmor( -(int)dec);
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceSkillArmor((int)dec);
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//294
	bool PlayerWrapper::SetHumanbomb(bool)
	{
		if (ThrowDice())
		{
			filter_Humanbomb *fhum = new filter_Humanbomb(
				this->object,
				this->ratio,
				(int)this->value,
				(int)this->amount,
				this->time,
				this->buffid,
				skill->GetForceattack());
			fhum->SetUp( skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhum);
		}
		return 1;
	}

	//295
	bool PlayerWrapper::SetThrowback(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEREPEL) != 0)
			{
				atstate = atstate | IMMUNEWEAK;
			}
			else
			{
				AddCommonDebuff(0, 1, 0, 0, 0);
				object.Throwback(skill->GetAttackerpos(), value);
			}
		}
		return 1;
	}

	//296
	bool PlayerWrapper::SetCreateitem(bool)
	{
		if (ThrowDice())
			object.CreateItem((int)this->value, (int)this->amount, (int)this->ratio);
		return 1;
	}

	//297
	bool PlayerWrapper::SetAloof(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Aloof(this->object, this->time));
		}
		return 1;
	}

	//298
	bool PlayerWrapper::SetJuniorlightform(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Juniorlightform(this->object, this->ratio, (int)this->value));
		}
		return 1;
	}

	//299
	int PlayerWrapper::GetGender(void)
	{
		if (object.GetObjectType())
			return 0;
		if (object.IsFemale())
			return 2;
		else
			return 1;
	}

	//300
	bool PlayerWrapper::SetSummonteleport1(bool)
	{
		object.CreateTelePort(value,time,8,skill->GetLevel(),0,
			skill->GetId(),ratio,amount,probability);
		return 1;
	}

	//301
	float PlayerWrapper::GetRange(void)
	{
		return object.GetExtendProp().attack_range;
	}

	//302
	bool PlayerWrapper::SetCibei(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Cibei(object,(int)(this->ratio * 1000.0),
				(int)this->value,this->time,this->buffid));
		}
		return 1;
	}

	//303
	bool PlayerWrapper::SetDecdrugeffect(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Decdrugeffect(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//304
	bool PlayerWrapper::SetIncmpgen(bool)
	{
		if (this->time > 1)
		{
			object.AddFilter(new filter_Incmpgen(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//305
	bool PlayerWrapper::SetTiansha(bool)
	{
		if (ThrowDice())
		{
			this->atstate |= 8;  //IMMUNEDRAINMAGIC
			object.AddFilter(new filter_Decdefence(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//306
	bool PlayerWrapper::SetGoback(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Goback( this->object, this->time));
		}
		return 1;
	}

	//307
	bool PlayerWrapper::SetBleeding(bool)
	{
		if (! ThrowDice())
			return 0;
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | IMMUNEWEAK; //SKILL_IMMUNEATTACK
		}
		else if (value > 1)
		{
			filter_Bleeding *fble = new filter_Bleeding(this->object, this->time, value, this->buffid);
			fble->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fble);
		}
		return 1;
	}

	//308
	bool PlayerWrapper::SetDivinityfury(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Divinityfury(object,amount,value,time,buffid));
		}
		return 1;
	}

	//309
	bool PlayerWrapper::SetEquipeffect(bool)
	{
		object.AddFilter(new filter_Equipeffect(object, time, (short)value, (short)amount));
		return 1;
	}

	//310
	bool PlayerWrapper::SetFrozen(bool)
	{
		if ((object.GetImmuneMask() & IMMUNEFROZE) != 0)
		{
			atstate = atstate | IMMUNEWEAK; //SKILL_IMMUNEATTACK
		}
		else if (ThrowDice())
		{
			object.AddFilter(new filter_Frozen(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//311
	bool PlayerWrapper::SetPasdechurt1(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultDefense( 0, -(int)(dec * 1000.0));
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceCultDefense(0, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//312
	bool PlayerWrapper::SetHpleak3(bool)
	{
		if (!ThrowDice())
			return 0;
		float Amount = GetAmount();
		float damage = Amount;
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | IMMUNEWEAK;
		}
		else if (damage > 1)
		{
			if (!object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish(skill->GetAttackerlevel(), GetLevel());
				damage = Amount * damage;
			}
			filter_Hpleak3 *fhpl = new filter_Hpleak3(object, time, damage, (int)value, buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return 1;
	}

	//313
	int PlayerWrapper::GetDodge(void)
	{
		return object.GetExtendProp().armor;
	}

	//314  这个函数后面来补   haokaixin
	bool PlayerWrapper::SetPerform(int inform)
	{
		if (!this->skill)
			return 0;
		skill->SetSkillState(2);
		this->result = 0;
		if (skill->GetSerialSkill() == 3)
			object.ClearComboSkill();
		if (inform == 2)
		{
			object.SendClientMsgSkillPerform(skill->GetSkillSpiritIndex());
			object.SetCoolDown( skill->GetId() + FMID_SKILLMAX, skill->GetCooltime());
			return 1;
		}
		else if (skill->GetRange() == 5)
		{
			if (skill->CanAttack())
			{
				skill->SetCastSelf(this);
				skill->GetPlayer()->SetCaster(object.GetSelfID());
				skill->SetAttackerpos(object.GetPos());
				skill->StateAttack();
				skill->BlessMe();
				CalcCost();
				SendResult(object.GetSelfID(), 0);
				if (inform)
				{
					if (!this->result)
						DoPerform();
				}
			}
			return 1;
		}
		else
		{
			if (skill->GetStub()->DoBless() && (skill->SetCastSelf(this), skill->SetAttackerpos(object.GetPos()), skill->BlessMe(), (this->result & 2) != 0))
			{
				DoPerform();
				return 1;
			}
			else
			{
				if (skill->GetStub()->GetType() == 1)
				{
					attack_msg msg;
					memset(&msg, 0, sizeof(msg));
					msg.damage_low = GetAttackLow(skill->GetRatio(), skill->GetPlus());
					msg.damage_high = GetAttackHigh(skill->GetRatio(), skill->GetPlus());
					msg.damage_no_crit = skill->GetPlus2();
					msg.force_attack = skill->GetForceattack();
					msg.skill_id = skill->GetId();
					msg.attack_rate = object.GetExtendProp().attack;
					msg.skill_limit = skill->GetLimit();
					memcpy(msg.skill_element, skill->GetSkillElems(), sizeof(msg.skill_element));
					msg.combo_color[0] = skill->GetC0();
					msg.combo_color[1] = skill->GetC1();
					msg.combo_color[2] = skill->GetC2();
					msg.combo_color[3] = skill->GetC3();
					msg.combo_color[4] = skill->GetC4();
					for (int size = 0; size <= 15; ++size)
						msg.skill_var[size] = this->var[size];
					if (skill->DoEnchant() || skill->IsComboSkill())
					{
						msg.attached_skill.skill = skill->GetId();
						msg.attached_skill.level = skill->GetLevel();
					}
					if (object.IsFuryState())
					{
						msg.deity_power = object.GetExtendProp().deity_power + skill->GetSaint();
					}
					XID xid( -1, -1);
					object.FillAttackMsg(xid, msg, 0);
					msg.crit_rate += (int)(skill->GetCrit() * 1000.0);
					msg.crit_factor = skill->GetCrithurt() + msg.crit_factor;
					msg.skill_attack_rate += skill->GetSkillaccu();
					SetTalentData( msg.skill_modify);
					if (skill->IsTalisman() && skill->GetData())
					{
						int size = 8;
						if (size > skill->GetData()->talismanRefineSkillList.size())
						{
							size = skill->GetData()->talismanRefineSkillList.size();
						}
						for (int i_0 = 0; i_0 < (unsigned int)size; ++i_0)
						{
							msg.talismanSkills[i_0] = skill->GetData()->talismanRefineSkillList[i_0];;
						}
					}
					float correction = 0;
					switch (skill->GetRange())
					{
					case 0:
					{
						if (this->target
							&& GetInrange(skill->GetEffectdistance(), correction, 0)
							&& skill->CanAttack())
						{
							object.Attack(*target, msg, 0);
						}
						else
						{
							this->result |= 1u;
						}
						break;
					}
					case 1:
					{
						A3DVECTOR targets_exclude_0;
						if (this->target
							&& object.QueryObject(*target,targets_exclude_0,correction) == 1
							&& skill->CanAttack())
						{
							bool attack_target = 0;
							std::vector<exclude_target> targets_exclude;
							float tbody;
							if (GetInrange(targets_exclude_0, correction, skill->GetEffectdistance(), tbody)
								&& skill->CanAttack())
							{
								exclude_target et;
								object.Attack(*target, msg, 0);
								et.type = 0;
								et.id = target->id;
								targets_exclude.push_back(et);
								attack_target = 1;
							}
							AddGroupAttackExcludeMonsterTids(targets_exclude);
							int Coverage;
							if (attack_target)
								Coverage = skill->GetCoverage() - 1;
							else
								Coverage = skill->GetCoverage();
							object.RegionAttack2(targets_exclude_0, skill->GetRadius(),
								msg,0, skill->GetAttackdistance(),
								Coverage,targets_exclude);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 2:
					{
						if (skill->CanAttack())
						{
							std::vector<exclude_target> targets_exclude_0;
							AddGroupAttackExcludeMonsterTids(targets_exclude_0);
							if (object.IsPlayerClass())
							{
								A3DVECTOR targets_exclude_1;
								float tbody = 0;
								object.GetCurTargetID();
								if (object.QueryObject(object.GetCurTargetID(),targets_exclude_1,tbody) == 1
									&& skill->CanAttack())
								{
									if (GetInrange(targets_exclude_1,tbody, skill->GetRadius(),correction))
									{
										object.Attack(object.GetCurTargetID(),msg, 0);
										exclude_target et_0;
										et_0.type = 0;
										et_0.id = object.GetCurTargetID().id;
										targets_exclude_0.push_back(et_0);
									}
								}
							}

							object.RegionAttack1(object.GetPos(), skill->GetRadius(), msg, 0, 
								skill->GetCoverage(), targets_exclude_0, 1);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 3:
					{
						A3DVECTOR targets_exclude_0;
						float tbody;
						if (this->target
							&& object.QueryObject(*target, targets_exclude_0, tbody) == 1)
						{
							if (GetInrange(targets_exclude_0, tbody, skill->GetEffectdistance(), correction)
								&& skill->CanAttack())
							{
								object.Attack(*target, msg, 0);
								std::vector<exclude_target> targets_exclude_1;
								exclude_target et_0;
								et_0.type = 0;
								et_0.id = this->target->id;
								targets_exclude_1.push_back(et_0);
								AddGroupAttackExcludeMonsterTids(targets_exclude_1);
								object.RegionAttack1(targets_exclude_0, skill->GetRadius(),
									msg,0, skill->GetCoverage() - 1,targets_exclude_1,1);
							}
							else
							{
								this->result |= 1;
							}
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 4:
					{
						A3DVECTOR targets_exclude_0;
						float tbody;
						if (this->target
							&& object.QueryObject(*target, targets_exclude_0, tbody) == 1
							&& skill->CanAttack())
						{
							bool attack_target = 0;
							std::vector<exclude_target> targets_exclude_1;
							if (GetInrange(targets_exclude_0, tbody, skill->GetEffectdistance(), correction)
								&& skill->CanAttack())
							{
								object.Attack(*target, msg, 0);
								exclude_target et_0;
								et_0.type = 0;
								et_0.id = this->target->id;
								targets_exclude_1.push_back(et_0);
								attack_target = 1;
							}
							AddGroupAttackExcludeMonsterTids(targets_exclude_1);
							int coverage;
							if (attack_target)
								coverage = skill->GetCoverage() - 1;
							else
								coverage = skill->GetCoverage();
							object.RegionAttack3(targets_exclude_0, skill->GetAngle(),msg,0,
								skill->GetRadius(), coverage,targets_exclude_1);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 6:
					{
						if (skill->GetEffectdistance() * skill->GetEffectdistance() < object.GetPos().squared_distance(skill->GetPosition()))
						{
							this->result |= 1;
						}
						else
						{
							std::vector<exclude_target> targets_exclude_1;
							AddGroupAttackExcludeMonsterTids(targets_exclude_1);
							object.RegionAttack1(skill->GetPosition(), skill->GetRadius(), msg, 
								0, skill->GetCoverage(), targets_exclude_1, 1);
						}
						break;
					}
					case 10:
					{
						if (this->tsize > 0 && this->target)
						{
							int i_0 = this->tsize;
							if (i_0 > skill->GetTargetCnt())
								i_0 = skill->GetTargetCnt();
							bool attack_target = 1;
							for (int size = 0; size < i_0; ++size)
							{
								float tbody;
								if (GetInrange(target[size], skill->GetCoverage() * skill->GetEffectdistance(), tbody, 0)
									&& skill->CanAttack())
								{
									object.Attack(target[size], msg, 0);
									attack_target = 0;
								}
							}
							if (attack_target)
								this->result |= 1;
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 11:
					{
						if (this->tsize > 0 && this->target)
						{
							int i_0 = this->tsize;
							if (i_0 > skill->GetTargetCnt())
								i_0 = skill->GetTargetCnt();
							bool attack_target = 1;
							for (int size = 0; size < i_0; ++size)
							{
								A3DVECTOR targets_exclude_0;
								float tbody;
								if (object.QueryObject(target[size],targets_exclude_0,tbody) == 1
									&& skill->CanAttack())
								{
									bool allOutRange_0 = 0;
									std::vector<exclude_target> targets_exclude_1;
									if (GetInrange(targets_exclude_0,tbody, skill->GetEffectdistance(),correction)
										&& skill->CanAttack())
									{
										object.Attack(target[size], msg, 0);
										exclude_target et_0;
										et_0.type = 0;
										et_0.id = this->target[size].id;
										targets_exclude_1.push_back(et_0);
										allOutRange_0 = 1;
									}
									AddGroupAttackExcludeMonsterTids(targets_exclude_1);
									int coverage;
									if (allOutRange_0)
										coverage = skill->GetCoverage() - 1;
									else
										coverage = skill->GetCoverage();
									object.RegionAttack2(targets_exclude_0,skill->GetRadius(),msg,
										0,skill->GetAttackdistance(),coverage,targets_exclude_1);
									attack_target = 0;
								}
							}
						}
						break;
					}
					case 12:
					{
						if (skill->CanAttack())
						{
							std::vector<exclude_target> targets_exclude_1;
							AddGroupAttackExcludeMonsterTids(targets_exclude_1);
							object.RegionAttackFaceLine(skill->GetRadius(),msg,0,
								skill->GetAttackdistance(),skill->GetCoverage(),targets_exclude_1);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 13:
					{
						if (skill->CanAttack())
						{
							std::vector<exclude_target> targets_exclude_1;
							AddGroupAttackExcludeMonsterTids(targets_exclude_1);
							object.RegionAttackFaceSector(skill->GetAngle(),msg,0,
								skill->GetRadius(),skill->GetCoverage(),targets_exclude_1);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 14:
					{
						if (skill->CanAttack())
						{
							std::vector<exclude_target> targets_exclude_1;
							AddGroupAttackExcludeMonsterTids(targets_exclude_1);
							object.RegionAttack1(object.GetPos(), skill->GetRadius(), msg, 0, 
								skill->GetCoverage(), targets_exclude_1, 1);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 15:
					{
						if (skill->CanAttack())
						{
							std::vector<exclude_target> targets_exclude_1;
							AddGroupAttackExcludeMonsterTids(targets_exclude_1);
							object.RegionAttackCross(skill->GetRadius(), skill->GetAttackdistance(), skill->GetAngle(),
								msg,skill->GetCoverage(),targets_exclude_1);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					default:
						break;
					}
				}
				else if (skill->DoEnchant())
				{
					enchant_msg m;
					memset(&m, 0, sizeof(m));
					m.skill = skill->GetId();
					m.skill_level = skill->GetLevel();
					m.force_attack = skill->GetForceattack();
					m.skill_reserved1 = object.GetExtendProp().damage_high;
					m.skill_limit = skill->GetLimit();
					for (int i_0 = 0; i_0 <= 15; ++i_0)
						m.skill_var[i_0] = this->var[i_0];
					char i_0 = skill->GetStub()->GetType();
					if (i_0 == 2)
					{
						m.helpful = 1;
					}
					else if (i_0 == 5)
					{
						m.helpful = 2;
					}
					SetTalentData( m.skill_modify);
					memcpy(&m.skill_element, skill->GetSkillElems(), sizeof(m.skill_element));
					m.combo_color[0] = skill->GetC0();
					m.combo_color[1] = skill->GetC1();
					m.combo_color[2] = skill->GetC2();
					m.combo_color[3] = skill->GetC3();
					m.combo_color[4] = skill->GetC4();
					if (skill->IsTalisman() && skill->GetData())
					{
						int size = 8;
						if (skill->GetData()->talismanRefineSkillList.size() < 8)
						{
							size = skill->GetData()->talismanRefineSkillList.size();
						}
						for (int i = 0; i < size; ++i)
						{
							m.talismanSkills[i] = skill->GetData()->talismanRefineSkillList[i];
						}
					}
					switch (skill->GetRange())
					{
					case 0:
					{
						if (this->target)
						{
							float tbody;
							if (GetInrange(skill->GetEffectdistance(), tbody, skill->GetTargettype())
								&& skill->CanAttack())
							{
								m.attack_range = skill->GetEffectdistance();
								if (m.attack_range <= 0.0)
									m.attack_range = object.GetExtendProp().attack_range;
								m.attack_range = m.attack_range + tbody;
								if (!skill->GetTargettype())
									object.EnchantZombie(*target, m);
								else
									object.Enchant(*target, m);
							}
						}
						this->result |= 1;
						break;
					}
					case 1:
					{
						A3DVECTOR targets_exclude_1;
						float tbody = 0;
						if (this->target
							&& object.QueryObject(*target, targets_exclude_1, tbody) == 1
							&& skill->CanAttack())
						{
							m.attack_range = skill->GetAttackdistance();
							if (m.attack_range <= 0.0)
								m.attack_range = object.GetExtendProp().attack_range;
							tbody = object.GetBodySize();
							m.attack_range = m.attack_range + tbody + m.attack_range;
							bool allOutRange_0a = 0;
							std::vector<exclude_target> targets_exclude_0;
							float correction = 0;
							if (GetInrange(targets_exclude_1, tbody, skill->GetEffectdistance(), correction)
								&& skill->CanAttack())
							{
								object.Enchant(*target, m);
								exclude_target et_0;
								et_0.type = 0;
								et_0.id = this->target->id;
								targets_exclude_0.push_back(et_0);
								allOutRange_0a = 1;
							}
							AddGroupAttackExcludeMonsterTids(targets_exclude_0);
							int coverage;
							if (allOutRange_0a)
								coverage = skill->GetCoverage() - 1;
							else
								coverage = skill->GetCoverage();
							object.RegionEnchant2(targets_exclude_1, skill->GetRadius(),
								m,coverage,targets_exclude_0);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 2:
					case 14:
					{
						if (skill->CanAttack())
						{
							if (skill->GetStub()->GetType() == 2)
							{
								if (GetInteam())
								{
									m.attack_range = skill->GetRadius();
									if (m.attack_range <= 0.0)
										m.attack_range = object.GetExtendProp().attack_range;
									if (skill->GetTargettype())
									{
										object.TeamEnchantZombie(m, 0, skill->IsFamily(), -1);
									}
									else
									{
										object.TeamEnchant(m, 0, skill->IsFamily());
									}
								}
								else if ((skill->GetStub()->skill_limit & 0x10) == 0)
								{
									skill->SetCastSelf(this);
									skill->GetPlayer()->SetCaster(object.GetSelfID());
									skill->SetAttackerpos(object.GetPos());
									skill->StateAttack();
									GNET::PlayerWrapper::SendResult(object.GetSelfID(), 0);
								}
							}
							else
							{
								m.attack_range = skill->GetRadius();
								if (m.attack_range <= 0.0)
									m.attack_range = object.GetExtendProp().attack_range;
								std::vector<exclude_target> targets_exclude_1;
								AddGroupAttackExcludeMonsterTids(targets_exclude_1);
								if (object.IsPlayerClass())
								{
									A3DVECTOR targets_exclude_0;
									XID xid = object.GetCurTargetID();
									float tbody;
									if (object.QueryObject(xid,targets_exclude_0,tbody) == 1
										&& skill->CanAttack())
									{
										float correction;
										if (GetInrange(targets_exclude_0,tbody,skill->GetRadius(),correction))
										{
											object.Enchant(xid, m);
											exclude_target et;
											et.type = 0;
											et.id = xid.id;
											targets_exclude_1.push_back(et);
										}
									}
								}
								object.RegionEnchant1(object.GetPos(), m.attack_range, m, skill->GetCoverage(), targets_exclude_1);
							}
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 3:
					{
						A3DVECTOR targets_exclude_1;
						float tbody;
						if (this->target
							&& object.QueryObject(*target, targets_exclude_1, tbody) == 1)
						{
							float correction;
							if (GetInrange(targets_exclude_1, tbody, skill->GetEffectdistance(), correction)
								&& skill->CanAttack())
							{
								m.attack_range = skill->GetEffectdistance();
								if (m.attack_range <= 0.0)
									m.attack_range = object.GetExtendProp().attack_range;
								m.attack_range = m.attack_range + correction;
								object.Enchant(*target, m);
								std::vector<exclude_target> targets_exclude_0;
								exclude_target et_0;
								et_0.type = 0;
								et_0.id = this->target->id;
								targets_exclude_0.push_back(et_0);
								AddGroupAttackExcludeMonsterTids(targets_exclude_0);
								object.RegionEnchant1(targets_exclude_1,skill->GetRadius(),m,
									skill->GetCoverage() - 1,targets_exclude_0);
							}
							else
							{
								this->result |= 1;
							}
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 4:
					{
						A3DVECTOR targets_exclude_1;
						float tbody;
						if (this->target
							&& object.QueryObject(*target, targets_exclude_1, tbody) == 1
							&& skill->CanAttack())
						{
							m.attack_range = skill->GetRadius();
							if (m.attack_range <= 0.0)
								m.attack_range = object.GetExtendProp().attack_range;
							tbody = object.GetBodySize();
							m.attack_range = m.attack_range + tbody + m.attack_range;
							float allOutRange_0b = 0;
							std::vector<exclude_target> targets_exclude_0;
							float correction;
							if (GetInrange(targets_exclude_1, tbody, skill->GetEffectdistance(), correction)
								&& skill->CanAttack())
							{
								object.Enchant(*target, m);
								exclude_target et_0;
								et_0.type = 0;
								et_0.id = this->target->id;
								targets_exclude_0.push_back(et_0);
								allOutRange_0b = 1;
							}
							AddGroupAttackExcludeMonsterTids(targets_exclude_0);
							int coverage;
							if (allOutRange_0b)
								coverage = skill->GetCoverage() - 1;
							else
								coverage = skill->GetCoverage();
							object.RegionEnchant3(targets_exclude_1,skill->GetAngle(),
								m,coverage,targets_exclude_0);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 6:
					{
						if (skill->GetEffectdistance() * skill->GetEffectdistance() < object.GetPos().squared_distance(skill->GetPosition()))
						{
							this->result |= 1;
						}
						else
						{
							std::vector<exclude_target> targets_exclude_0;
							AddGroupAttackExcludeMonsterTids(targets_exclude_0);
							object.RegionEnchant1(skill->GetPosition(), skill->GetRadius(), m, skill->GetCoverage(), targets_exclude_0);
						}
						break;
					}
					case 7:
					{
						XID et_0;
						if (object.GetLeader(et_0))
						{
							A3DVECTOR targets_exclude_1;
							float correction = 0;
							m.attack_range = skill->GetEffectdistance();
							float tbody;
							if (object.QueryObject(et_0,targets_exclude_1,tbody) == 1
								&& skill->CanAttack())
							{
								if (GetInrange(targets_exclude_1,tbody, skill->GetEffectdistance(),correction))
								{
									object.Enchant(et_0, m);
								}
							}
						}
						break;
					}
					case 8:
					{
						float tbody;
						if (this->target
							&& skill->IsSpouseSkill()
							&& skill->CanAttack()
							&& object.IsSpouse(*target)
							&& GetInrange(skill->GetEffectdistance(), tbody, skill->GetTargettype()))
						{
							m.attack_range = skill->GetEffectdistance();
							if (m.attack_range <= 0.0)
								m.attack_range = object.GetExtendProp().attack_range;
							m.attack_range = m.attack_range + tbody;
							if (skill->GetTargettype())
								object.EnchantZombie(*target, m);
							else
								object.Enchant(*target, m);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 9:
					{
						XID et_0 = object.GetCloneID();
						float tbody;
						if (et_0.id > 0
							&& skill->CanAttack()
							&& GetInrange(skill->GetEffectdistance(), tbody, skill->GetTargettype()))
						{
							m.attack_range = skill->GetEffectdistance();
							if (m.attack_range <= 0.0)
								m.attack_range = object.GetExtendProp().attack_range;
							m.attack_range = m.attack_range + tbody;
							if (skill->GetTargettype())
								object.EnchantZombie(et_0, m);
							else
								object.Enchant(et_0, m);
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 10:
					{
						if (this->tsize > 0 && this->target)
						{
							int size = this->tsize;
							if (size > skill->GetTargetCnt())
								size = skill->GetTargetCnt();
							bool allOutRange_0c = 1;
							for (int ia = 0; ia < size; ++ia)
							{
								float tbody;
								if (GetInrange(target[ia], skill->GetEffectdistance(), tbody, skill->GetTargettype())
									&& skill->CanAttack())
								{
									m.attack_range = skill->GetEffectdistance();
									if (m.attack_range <= 0.0)
										m.attack_range =object.GetExtendProp().attack_range;
									m.attack_range = m.attack_range + tbody;
									if (skill->GetTargettype())
										object.EnchantZombie(target[ia], m);
									else
										object.Enchant(target[ia], m);
									allOutRange_0c = 0;
								}
							}
							if (allOutRange_0c)
								this->result |= 1;
						}
						else
						{
							this->result |= 1;
						}
						break;
					}
					case 11:
					{
						int size = this->tsize;
						if (size > skill->GetTargetCnt())
							size = skill->GetTargetCnt();
						for (int ib = 0; ib < size; ++ib)
						{
							A3DVECTOR targets_exclude_1;
							float tbody;
							if (this->target
								&& object.QueryObject(*target, targets_exclude_1, tbody) == 1
								&& skill->CanAttack())
							{
								m.attack_range = skill->GetAttackdistance();
								if (m.attack_range <= 0.0)
									m.attack_range = object.GetExtendProp().attack_range;
								object.GetBodySize();
								m.attack_range = m.attack_range + tbody + m.attack_range;
								int attack_target = 0;
								std::vector<exclude_target> targets_exclude_0;
								float correction = 0;
								if (GetInrange(targets_exclude_1, tbody, skill->GetEffectdistance(), correction)
									&& skill->CanAttack())
								{
									object.Enchant(*target, m);
									exclude_target et_0;
									et_0.type = 0;
									et_0.id = this->target->id;
									targets_exclude_0.push_back(et_0);
									attack_target = 1;
								}
								AddGroupAttackExcludeMonsterTids(targets_exclude_0);
								int coverage;
								if (attack_target)
									coverage = skill->GetCoverage() - 1;
								else
									coverage = skill->GetCoverage();
								object.RegionEnchant2(targets_exclude_1, skill->GetRadius(),
									m, coverage, targets_exclude_0);
							}
						}
						break;
					}
					case 12:
					{
						m.attack_range = skill->GetAttackdistance();
						if (m.attack_range <= 0.0)
							m.attack_range = object.GetExtendProp().attack_range;
						std::vector<exclude_target> targets_exclude_1;
						AddGroupAttackExcludeMonsterTids(targets_exclude_1);
						object.RegionEnchantFaceLine(skill->GetRadius(), m, skill->GetCoverage(), targets_exclude_1);
						break;
					}
					case 13:
					{
						m.attack_range = skill->GetRadius();
						if (m.attack_range <= 0.0)
							m.attack_range = object.GetExtendProp().attack_range;
						std::vector<exclude_target> targets_exclude_1;
						AddGroupAttackExcludeMonsterTids(targets_exclude_1);
						object.RegionEnchantFaceSector(skill->GetAngle(), m, skill->GetCoverage(), targets_exclude_1);
						break;
					}
					case 15:
					{
						m.attack_range = skill->GetAttackdistance();
						if (m.attack_range <= 0.0)
							m.attack_range = object.GetExtendProp().attack_range;
						std::vector<exclude_target> targets_exclude_1;
						AddGroupAttackExcludeMonsterTids(targets_exclude_1);
						object.RegionEnchantCross(skill->GetRadius(),skill->GetAngle(),
							m,skill->GetCoverage(),targets_exclude_1);
						break;
					}
					default:
						break;
					}
				}
				if (inform && !this->result)
				{
					CalcCost();
					DoPerform();
				}
				return 0;
			}
		}
	}

	bool PlayerWrapper::SetAddantislow(bool)
	{
		object.AddFilter(new filter_Addantislow(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//315
	bool PlayerWrapper::SetAddweak(bool)
	{
		if (object.IsFilterExist(FILTER_ADDWEAK))
		{
			object.RemoveFilter(FILTER_ADDWEAK);
		}
		else
		{
			object.AddFilter(new filter_Addweak(object,probability,value,amount,ratio));
		}
		return 1;
	}

	//316
	bool PlayerWrapper::SetDecdodge(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Decdodge(object, (int)(ratio * 1000.0), time, buffid));
		}
		return 1;
	}

	//317
	bool PlayerWrapper::SetMpleak(bool)
	{
		if (!ThrowDice())
			return 0;
		if (!object.IsPlayerClass())
			return 0;
		if ((object.GetImmuneMask() & IMMUNEMPLEAK) != 0)
		{
			atstate = atstate | 0x80;
		}
		else
		{
			float Amount = GetAmount();
			if (Amount > 1)
			{
				object.AddFilter(new filter_Mpleak(object, time, (int)Amount, (int)value, buffid));
			}
		}
		return 1;
	}

	//318
	bool PlayerWrapper::SetAddspot(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addspot(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//319
	bool PlayerWrapper::SetZhaoqi(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Zhaoqi(object,ratio * 1000.0,value,time,buffid));
		}
		return 1;
	}

	//320
	int PlayerWrapper::GetVar16(void)
	{
		return this->var[15];
	}

	//321
	bool PlayerWrapper::SetSeed(bool)
	{
		if (!object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			object.AddFilter(new filter_Seed( this->object, (int)this->amount, this->time));
		}
		return 1;
	}

	//322
	bool PlayerWrapper::SetSubantisilent(bool)
	{
		object.AddFilter(new filter_Subantisilent(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//323
	bool PlayerWrapper::IsRenMa(void)
	{
		return object.IsRenMa();
	}

	//324
	bool PlayerWrapper::UseDp(int dec)
	{
		int dp = object.GetBasicProp().dp;
		if (dp <= dec)
			return object.DecDeity(dp);
		else
			return object.DecDeity(dec);
	}

	//325
	bool PlayerWrapper::SetTuoling2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Tuoling2(object,time,ratio * 100.0,amount * 1000.0,value * 100.0,buffid));
		}
		return 1;
	}

	//326
	bool PlayerWrapper::SetSleeptimer(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Sleeptimer(this->object, this->time, (int)this->value, (int)this->amount));
		}
		return 1;
	}

	//327
	bool PlayerWrapper::SetMiracleburstfo(bool)
	{
		object.AddFilter(new filter_Miracleburstfo(object,ratio * 1000.0,time,buffid,amount));
		return 1;
	}

	//328
	bool PlayerWrapper::SetHpgen(bool)
	{
		if (ThrowDice())
		{
			if (this->time > 0)
			{
				object.AddFilter(new filter_Hpgen(object,value,amount,time,buffid, ratio > 0.001));
			}
		}
		return 1;
	}

	//329
	bool PlayerWrapper::SetAssault(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNETAUNT) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.BeTaunted(skill->GetAttackerid(), time);
			}
		}
		return 1;
	}

	//330
	bool PlayerWrapper::SetSummontrap(bool)
	{
		object.CreateTrap(value, time, 6, skill->GetLevel(), ratio != 0, amount);
		return 1;
	}

	//331
	bool PlayerWrapper::SetAswind(bool)
	{
		if (object.IsFilterExist(FILTER_ASWIND))
		{
			object.RemoveFilter(FILTER_ASWIND);
		}
		else
		{
			object.AddFilter(new filter_Aswind(this->object, this->amount, (int)this->value));
		}
		return 1;
	}

	//332
	int PlayerWrapper::GetHide(void)
	{
		return object.IsInvisible();
	}

	//333
	bool PlayerWrapper::GetIsignite(void)
	{
		return object.IsIgniteState();
	}

	//334
	void PlayerWrapper::SetVar7(int v)
	{
		SetVar( v, 6);
	}

	//335
	bool PlayerWrapper::SetJaillimitcyclearea(bool a2)
	{
		if ((object.GetImmuneMask() & IMMUNEPULL) != 0)
		{
			atstate = atstate | 0x80;
			return 1;
		}
		else
		{
			if (object.IsPermitCycleAreaExist(skill->GetAttackerid().id))
			{
				return 0;
			}
			else
			{
				if (object.IsFilterExist(FILTER_LIMITCYCLEAREA))
				{
					cycle_area ca;
					ca.owner = skill->GetAttackerid().id;
					ca.center = skill->GetAttackerpos();
					ca.radius = skill->GetAttackRange();
					ca.time = this->time;
					object.ModifyFilter(FILTER_LIMITCYCLEAREA, 8, &ca, sizeof(ca));
				}
				else
				{
					filter_Limitcyclearea *flim = new filter_Limitcyclearea(this->object, this->time, skill->GetAttackerpos(), skill->GetAttackRange());
					flim->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
					object.AddFilter(flim);
				}
				return 1;
			}
		}
	}

	//336
	bool PlayerWrapper::SetCatchpet(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_CatchPet(this->object, this->time, skill->GetAttackerid()));
		}
		return 1;
	}

	//337
	bool PlayerWrapper::SetJailpermitcyclearea(bool a2)
	{
		if ((object.GetImmuneMask() & IMMUNEPULL) != 0)
		{
			atstate = atstate | 0x80;
		}
		else
		{
			XID Attackerid = skill->GetAttackerid();
			object.RemoveLimitCycleArea(Attackerid.id);
			if (object.IsFilterExist(FILTER_PERMITCYCLEAREA))
			{
				cycle_area ca;
				ca.owner = Attackerid.id;
				ca.center = skill->GetAttackerpos();
				ca.radius = skill->GetAttackRange();
				ca.time = this->time;
				object.ModifyFilter(FILTER_PERMITCYCLEAREA, 8, &ca, sizeof(ca));
			}
			else
			{
				filter_Permitcyclearea *fper = new filter_Permitcyclearea(object, time, skill->GetAttackerpos(), skill->GetAttackRange());
				fper->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
				object.AddFilter(fper);
			}
		}
		return 1;
	}

	//338
	bool PlayerWrapper::SetPossummon(bool)
	{
		object.CreatePosSummon(value,amount,time,1,skill->GetLevel(),object.GetCurTargetID());
		return 1;
	}

	//339
	bool PlayerWrapper::SetSubantidizzy2(bool)
	{
		if (!object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subantidizzy2(object, value, time, amount));
		}
		return 1;
	}

	//340
	int PlayerWrapper::TakeOutItem(int item_id, int count)
	{
		return object.TakeOutItem(item_id, count);
	}

	//341
	bool PlayerWrapper::SetFogstate(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Fogstate(this->object, value, this->time));
		}
		return 1;
	}

	//342
	bool PlayerWrapper::SetDodge(bool)
	{
		object.AddFilter(new filter_Dodge(this->object, this->time, this->value));
		return 1;
	}

	//343
	bool PlayerWrapper::SetHpleak4(bool)
	{
		if (!ThrowDice())
			return 0;
		float Amount = GetAmount();
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | 0x80;
		}
		else if (Amount > 1)
		{
			if (! object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish(skill->GetAttackerlevel(), GetLevel());
				Amount = Amount * Amount;
			}
			filter_Hpleak4 *fhpl = new filter_Hpleak4(object, time, Amount, value, buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return 1;
	}

	//344
	bool PlayerWrapper::SetAddantiwrap(bool)
	{
		object.AddFilter(new filter_Addantiwrap(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//345
	bool PlayerWrapper::SetRmhumanform(bool)
	{
		if (ThrowDice())
		{
			if (object.GetShape())
			{
				object.RemoveFilter(FILTER_RMHUMANFORM);
			}
			else
			{
				object.AddFilter(new filter_Rmhumanform(this->object));
			}
		}
		return 1;
	}

	//346
	bool PlayerWrapper::GetSuccess(void)
	{
		return (this->result ^ 1) & 1;
	}

	//347
	bool PlayerWrapper::SetIgnite(bool)
	{
		if (! ThrowDice())
			return 0;
		if ((object.GetImmuneMask() & IMMUNEIGNITE) != 0)
		{
			atstate = atstate | 0x80;
		}
		else
		{
			int Amount = GetAmount();
			if (Amount > 0)
			{
				if (! object.IsPlayerClass())
				{
					object.CalcLevelDamagePunish(skill->GetAttackerlevel(), skill->GetLevel());
					Amount = Amount * Amount;
				}
				filter_Ignite *fign = new filter_Ignite(this->object, this->time, Amount, (int)this->value, this->buffid);
				fign->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
				object.AddFilter(fign);
			}
		}
		return 1;
	}

	//348
	bool PlayerWrapper::SetDiet(bool)
	{
		if (ThrowDiceDiet())
		{
			if ((object.GetImmuneMask() & IMMUNEDIET) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.AddFilter(new filter_Diet( this->object, this->time));
			}
		}
		return 1;
	}

	//349
	void PlayerWrapper::SetVar(int v, uint i)
	{
		if (i <= 15)
			this->var[i] = v;
	}

	//350
	bool PlayerWrapper::SetRepel(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEREPEL) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.KnockBack(skill->GetAttackerid(), skill->GetAttackerpos(), GetValue());
			}
		}
		return 1;
	}

	//351
	bool PlayerWrapper::SetContext(char c)
	{
		this->context = c;
		return 1;
	}

	//352
	bool PlayerWrapper::SetPasadddefence(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairDefense(inc);
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceDefense(inc);
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//353
	bool PlayerWrapper::SetAttack2hp(bool)
	{
		object.AddFilter(new filter_Attack2hp(object, ratio, time, probability));
		return 1;
	}

	//354
	bool PlayerWrapper::CheckTarget(float range, bool corpseonly)
	{
		A3DVECTOR tpos;
		if (!this->target || !this->tsize)
			return 0;
		if (*target == object.GetSelfID())
		{
			if (skill->GetStub()->GetType() != 2)
				return 0;
		}
		float tbody;
		int ret = object.QueryObject(*target, tpos, tbody);
		if (!ret)
			return 0;
		if (corpseonly && ret != 2)
			return 0;
		if (!corpseonly && ret == 2)
			return 0;
		float body = object.GetBodySize();
		return (body + range + tbody) * (body + range + tbody) > object.GetPos().squared_distance(tpos);
	}

	//355
	bool PlayerWrapper::SetTuoling1(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Tuoling1(object,time,ratio * 100.0,amount * 1000.0,value * 100.0,buffid));
		}
		return 1;
	}

	//356
	bool PlayerWrapper::SetIncattack(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incattack(object,ratio * 1000.0, time, buffid));
		}
		return 1;
	}

	//357
	bool PlayerWrapper::SetPasadddizzy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistance( 0, inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance(0, inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//358
	bool PlayerWrapper::SetFoxstate(bool)
	{
		if (ThrowDice())
		{
			if ( object.IsFilterExist(FILTER_FOXSTATE))
			{
				object.RemoveFilter(FILTER_FOXSTATE);
			}
			else
			{
				object.AddFilter(new filter_Foxstate(this->object, this->value));
			}
		}
		return 1;
	}

	//359
	bool PlayerWrapper::SetHolyaura(bool)
	{
		if (!this->skill)
			return 0;
		this->result |= 2u;
		if ( object.IsFilterExist(FILTER_HOLYAURA))
		{
			object.RemoveFilter(FILTER_HOLYAURA);
			return 1;
		}
		else
		{
			if ( object.IsFilterExist(FILTER_EVILAURA))
			{
				object.RemoveFilter(FILTER_EVILAURA);
			}
			else
			{
				filter_Holyaura *fhol = new filter_Holyaura(
					object,
					skill->GetId(),
					skill->GetLevel(),
					skill->GetRadius(),
					skill->GetCoverage(),
					skill->GetMpcost(),
					time,
					this->var,
					skill->GetDpcost(),
					skill->GetHpcost());
				fhol->SetVisible((int)this->amount);
				object.AddFilter(fhol);
			}
			return 1;
		}
	}

	//360
	bool PlayerWrapper::SetPuppetskill(bool)
	{
		if (! object.IsFilterExist(4258))
			return 0;
		object.AddFilter(new filter_Puppetskill( object, time, probability, value));
		return 1;
	}

	//361
	bool PlayerWrapper::SetShadowhide(bool)
	{
		if (ThrowDice() && object.CanSetInvisible())
		{
			object.AddFilter(new filter_Shadowhide(this->object, this->ratio, (int)this->value, this->time, (int)this->amount));
		}
		return 1;
	}

	//362
	bool PlayerWrapper::SetInccritrate(bool)
	{
		object.AddFilter(new filter_Inccritrate(this->object, (int)(this->ratio * 1000.0), this->time));
		return 1;
	}

	//363
	bool PlayerWrapper::SetImmuneslow(bool)
	{
		RemoveMultiFilter(2);
		object.AddFilter(new filter_Immuneslow(this->object, this->time));
		return 1;
	}

	//364
	int PlayerWrapper::GetRegionReputation(int index)
	{
		return object.GetRegionReputation(index);
	}

	//365
	bool PlayerWrapper::SetDecsp(int dec)
	{
		return object.ModifySkillPoint(-dec);
	}

	//366
	int PlayerWrapper::GetRage(void)
	{
		return object.GetBasicProp().rage;
	}

	//367
	int PlayerWrapper::GetSglevel(void)
	{
		return object.GetBasicProp().dt_level;
	}

	//368
	bool PlayerWrapper::SetRatio(float d)
	{
		this->ratio = d;
		return 1;
	}

	//369
	bool PlayerWrapper::SetIncmp(bool)
	{
		object.AddFilter(new filter_Incmp(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		return 1;
	}

	//370
	bool PlayerWrapper::SetSilent(bool)
	{
		if (this->probability < 0.001)
			return 0;
		if (ThrowDice(3))
		{
			if ((object.GetImmuneMask() & IMMUNESILENT) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				AddCommonDebuff(5, this->time, 0, 0, 0);
			}
		}
		else
		{
			this->atstate |= 4u;
		}
		return 1;
	}

	//371
	bool PlayerWrapper::SetPasdecrestrainmo(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultAttack( 1, -(int)(dec * 1000.0));
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceCultAttack(1, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//372
	bool PlayerWrapper::SetBarehanded(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Barehanded(this->object, this->time));
		}
		return 1;
	}

	//373
	bool PlayerWrapper::SetDizzy(bool)
	{
		if (this->probability >= 0.001)
		{
			if (this->context == 1 && ThrowDice(0))
			{
				if ((object.GetImmuneMask() & IMMUNEDIZZY) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					AddCommonDebuff( 0, this->time, 0, 0, 0);
				}
			}
			else
			{
				this->atstate |= 4u;
			}
		}
		return 0;
	}

	//374
	bool PlayerWrapper::SetGloss(bool)
	{
		if (this->buffid > 1)
		{
			object.AddFilter(new filter_Icon(this->object, this->time, 0, 3, FILTER_ICON, 0));
		}
		else
		{
			object.AddFilter(new filter_Icon(this->object, this->time, 0, this->buffid + 15, FILTER_ICON, 0));
		}
		return 1;
	}

	//375
	void PlayerWrapper::SetVar5(int v)
	{
		SetVar(v, 4);
	}

	//376
	int PlayerWrapper::GetOccupation(void)
	{
		return object.GetClass();
	}

	//377
	bool PlayerWrapper::SetTurnbuff(bool)
	{
		object.AddFilter(new filter_Turnbuff( this->object, this->time, (int)this->amount, this->buffid));
		return 1;
	}

	//378
	bool PlayerWrapper::SetPray(int)
	{
		if (!this->skill)
			return 0;
		skill->SetSkillState(1);
		return 1;
	}

	//379
	bool PlayerWrapper::SetSeniordarkform(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Seniordarkform(object,ratio,value,amount,time,buffid));
		}
		return 1;
	}

	//380
	bool PlayerWrapper::SetDecfatalratio(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Decfatalratio(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//381
	bool PlayerWrapper::SetPasinchp(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleMaxHP((int)(inc * 1000.0));
			object.UpdateHPMP();
		}
		else
		{
			object.EnhanceScaleMaxHP((int)(inc * 1000.0));
			if (this->context == 4)
				object.UpdateHPMP();
		}
		return 1;
	}

	//382
	int PlayerWrapper::GetMaxdp(void)
	{
		return object.GetExtendProp().max_dp;
	}

	//383
	bool PlayerWrapper::SetTalismaneffectsbless(bool)
	{
		if (skill->IsTalisman() && skill->GetData())
		{
			object.AddFilter(new filter_Talismaneffectsbless(
				object,
				skill->GetLevel(),
				skill->GetForceattack(),
				this->var,
				skill->GetData()->talismanRefineSkillList));
		}
		return 1;
	}

	//384
	bool PlayerWrapper::SetPosexchangewithclone(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Posexchangewithclone(this->object));
		}
		return 1;
	}

	//385
	bool PlayerWrapper::SetGtaddattackdefense(bool)
	{
		object.AddFilter(new filter_Gtaddattackdefense(
			this->object,
			(int)(this->value * 1000.0 + 0.5),
			(int)(this->amount * 1000.0 + 0.5),
			this->buffid));
		return 1;
	}

	//386
	bool PlayerWrapper::SetSubantisilent2(bool)
	{
		if (! object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subantisilent2(object, value, time, amount));
		}
		return 1;
	}

	//387
	bool PlayerWrapper::SetScopedamage(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Scopedamage(
				this->object,
				(int)this->value,
				this->ratio,
				skill->GetRadius(),
				skill->GetForceattack(),
				skill->GetCoverage()));
		}
		return 1;
	}

	//388
	bool PlayerWrapper::SetSkillmirror(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Skillmirror(this->object, this->time, (int)this->probability));
		}
		return 1;
	}

	//389
	bool PlayerWrapper::SetSlow(bool)
	{
		if (ThrowDice( 5))
		{
			if ((object.GetImmuneMask() & IMMUNESLOW) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				AddCommonDebuff( 4, this->time, (int)(this->ratio * 1000.0), this->buffid, 0);
			}
		}
		return 1;
	}

	//390
	bool PlayerWrapper::SetAddantisleep(bool)
	{
		object.AddFilter(new filter_Addantisleep(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//391
	bool PlayerWrapper::SetDeathscatter(bool)
	{
		object.AddFilter(new filter_Deathscatter(
			this->object,
			this->value,
			(int)this->probability,
			this->ratio,
			(int)this->amount,
			this->buffid,
			skill->GetForceattack(),
			time));
		return 1;
	}

	//392
	bool PlayerWrapper::SetSpiritdrag(bool)
	{
		if ( ThrowDice()
			&& !object.IsFilterExist(4210)
			&& !object.IsFilterExist(4220)
			&& !object.IsFilterExist(4218))
		{
			if ((object.GetImmuneMask() & IMMUNEPULL) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				filter_Spiritdrag *fspi = new filter_Spiritdrag(this->object, this->time, this->ratio, this->value);
				fspi->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
				object.AddFilter(fspi);
			}
		}
		return 1;
	}

	//393
	int PlayerWrapper::GetTmplid(void)
	{
		return object.GetTemplateID();
	}

	//394
	bool PlayerWrapper::SetImmunewrap(bool)
	{
		object.RemoveFilter(FILTER_WRAP);
		object.AddFilter(new filter_Immunewrap(this->object, this->time));
		return 1;
	}

	//395
	bool PlayerWrapper::SetClearweak(bool)
	{
		if (ThrowDice())
			RemoveMultiFilter(5);
		return 1;
	}

	//396
	void PlayerWrapper::SetVar4(int v)
	{
		SetVar(v, 3);
	}

	//397
	bool PlayerWrapper::SetPasaddsmite(float ratio)
	{
		if (this->context == 3)
			object.ImpairCritDmg(ratio);
		else
			object.EnhanceCritDmg(ratio);
		return 1;
	}

	//398
	void PlayerWrapper::SetCaster(XID const& id)
	{
		caster.type = id.type;
		caster.id = id.id;
	}

	//399
	bool PlayerWrapper::SetTurndebuff(bool)
	{
		object.AddFilter(new filter_Turndebuff(this->object, this->time, (int)this->amount, this->buffid));
		return 1;
	}

	//400
	int PlayerWrapper::GetVar10(void)
	{
		return this->var[9];
	}

	//401
	bool PlayerWrapper::SetPasincantislow(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(5, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(5, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//402
	void PlayerWrapper::ConsumeItem(void)
	{
		const SKILL::Data* skill_data =skill->GetData();
		if (skill_data && skill_data->consumable && skill_data->item_id != -1 && skill_data->item_index != -1)
			TakeOutItem( skill_data->item_index, skill_data->item_id, 1);
		if (skill->IsNeedItem())
		{
			char ItemRelation = skill->GetItemRelation();
			if (ItemRelation == 1)
			{
				TakeOutItem(skill->GetItem1Id(), skill->GetItem1Num());
				TakeOutItem(skill->GetItem2Id(), skill->GetItem2Num());
			}
			else if (ItemRelation > 1)
			{
				if (ItemRelation == 2)
				{
					int num = 0;
					int id = 0;
					if ( CheckItem(skill->GetItem1Id(), skill->GetItem1Num()))
					{
						num = skill->GetItem1Num();
						id =  skill->GetItem1Id();
					}
					else
					{
						num = skill->GetItem2Num();
						id =  skill->GetItem2Id();
					}
					TakeOutItem(id, num);
				}
			}
			else if (!ItemRelation)
			{
				TakeOutItem(skill->GetItem1Id(), skill->GetItem1Num());
			}
		}
	}

	//403
	void PlayerWrapper::UseHp(int dec)
	{
		int hp = object.GetBasicProp().hp;
		if ( hp <= dec)
			object.DecHP(hp - 1);
		else
			object.DecHP(dec);
	}

	//404
	bool PlayerWrapper::SetRetort(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Retort(object,ratio,probability,time,
				skill->GetForceattack(),value,amount));
		}
		return 1;
	}

	//405
	bool PlayerWrapper::SetAsforest(bool)
	{
		if ( object.IsFilterExist(FILTER_ASFOREST))
		{
			object.RemoveFilter(FILTER_ASFOREST);
		}
		else
		{
			object.AddFilter(new filter_Asforest(this->object, (int)this->amount, (int)(this->value * 1000.0)));
		}
		return 1;
	}

	//406
	bool PlayerWrapper::SetAddspeed(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addspeed(this->object, this->value, this->time, this->buffid));
		}
		return 1;
	}

	//407
	bool PlayerWrapper::SetJail(bool)
	{
		if ((object.GetImmuneMask() & IMMUNEPULL) != 0)
		{
			atstate = atstate | 0x80;
		}
		else
		{
			object.AddFilter(new filter_Jail(object,time,probability,value,amount,ratio,buffid,skill->GetForceattack()));
		}
		return 1;
	}

	//408
	bool PlayerWrapper::SetAddrage2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addrage2(this->object, this->ratio));
		}
		return 1;
	}

	//409
	bool PlayerWrapper::SetAttack2mp(bool)
	{
		object.AddFilter(new filter_Attack2mp(this->object, this->ratio, this->time, (int)this->probability));
		return 1;
	}

	//410
	bool PlayerWrapper::SetImmunedizzy(bool)
	{
		object.RemoveFilter(FILTER_DIZZY);
		object.AddFilter(new filter_Immunedizzy(this->object, this->time));
		return 1;
	}

	//411
	bool PlayerWrapper::SetBuxiu(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Buxiu(this->object, (int)this->value, this->time));
		}
		return 1;
	}

	//412
	void PlayerWrapper::SetVar11(int v)
	{
		SetVar(v, 10);
	}

	//413
	bool PlayerWrapper::SetPasdechurt(float dec)
	{
		if (this->context == 3)
			object.ImpairScaleDmgReduce(dec * 1000.0);
		else
			object.EnhanceScaleDmgReduce(dec * 1000.0);
		return 1;
	}

	//414
	int PlayerWrapper::GetLevelSum(int cls)
	{
		return object.GetSkillWrapper().GetLevelSum(cls);
	}

	//415
	bool PlayerWrapper::SetTiangu(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Tiangu(object,ratio * 1000.0,value,time,buffid));
		}
		return 1;
	}

	//416
	bool PlayerWrapper::IsCloneExist(void)
	{
		return object.IsCloneExist();
	}

	//417
	bool PlayerWrapper::SetDodgeregain(bool)
	{
		object.AddFilter(new filter_Dodgeregain(object,ratio,value,amount,probability,time));
		return 1;
	}

	//418
	bool PlayerWrapper::SetCleardizzy(bool)
	{
		if (ThrowDice())
			object.RemoveFilter(FILTER_DIZZY);
		return 1;
	}

	//419
	bool PlayerWrapper::SetSubantiwrap(bool)
	{
		object.AddFilter(new filter_Subantiwrap(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//420
	bool PlayerWrapper::SetSwift(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Swift(this->object, this->time, (int)(this->ratio * 100.0), (int)(this->value * 1000.0)));
		}
		return 1;
	}

	//421
	int PlayerWrapper::GetMaxatk(void)
	{
		return object.GetExtendProp().damage_high;
	}

	//422
	bool PlayerWrapper::SetBloodthirsty(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Bloodthirsty( this->object, this->time, this->ratio, this->buffid));
		}
		return 1;
	}

	//423
	bool PlayerWrapper::SetAshill(bool)
	{
		if (object.IsFilterExist(FILTER_ASHILL))
		{
			object.RemoveFilter(FILTER_ASHILL);
		}
		else
		{
			object.AddFilter(new filter_Ashill(this->object, (int)this->amount, (int)(this->value * 1000.0)));
		}
		return 1;
	}

	//424
	bool PlayerWrapper::SetAddhp2(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Addhp2(this->object, (int)this->value, this->time, this->buffid, this->amount));
		}
		return 1;
	}

	//425
	int PlayerWrapper::GetVar11(void)
	{
		return this->var[10];
	}

	//426
	void PlayerWrapper::AddGroupAttackExcludeMonsterTids(std::vector<exclude_target>& vec)
	{
		exclude_target et;
		unsigned short* s = GetExcludeMonsterTids();
		for (int i = 0; i <= 3; ++i)
		{
			et.type = 1;
			et.id = s[i];
			vec.push_back(et);
		}
	}

	//427
	bool PlayerWrapper::SetIncmountspeed(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incmountspeed(this->object, this->time, this->value, this->buffid));
		}
		return 1;
	}

	//428
	bool PlayerWrapper::SetRecordpos(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Recordpos(this->object, this->time));
		}
		return 1;
	}

	//429
	bool PlayerWrapper::IsMount(void)
	{
		return object.IsMount();
	}

	//430
	bool PlayerWrapper::SetPasaddanti(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistance( 0, (int)inc);
			object.ImpairResistance( 1, (int)inc);
			object.ImpairResistance( 2, (int)inc);
			object.ImpairResistance( 3, (int)inc);
			object.ImpairResistance( 4, (int)inc);
			object.ImpairResistance( 5, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance(0, (int)inc);
			object.EnhanceResistance(1, (int)inc);
			object.EnhanceResistance(2, (int)inc);
			object.EnhanceResistance(3, (int)inc);
			object.EnhanceResistance(4, (int)inc);
			object.EnhanceResistance(5, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//431
	bool PlayerWrapper::SetAddmp(bool)
	{
		object.AddFilter(new filter_Addmp(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//432
	bool PlayerWrapper::SetSubanti(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subanti(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//433
	bool PlayerWrapper::SetCycdecattack(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Cycdecattack(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//434
	int PlayerWrapper::GetBaseLevel(int id)
	{
		return object.GetSkillWrapper().GetBaseLevel(id);
	}

	//435
	bool PlayerWrapper::SetGangdan(bool)
	{
		if (object.IsFilterExist(4170))
		{
			object.RemoveFilter(4170);
		}
		else
		{
			object.AddFilter(new filter_Gangdan(this->object, this->ratio, (int)(this->amount * 1000.0), amount * 10.0, this->value));
		}
		return 1;
	}

	//436
	bool PlayerWrapper::SetFashen(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Fashen(object,ratio * 1000.0,value,time,buffid));
		}
		return 1;
	}

	//437
	int PlayerWrapper::GetLevel(void)
	{
		return object.GetBasicProp().level;
	}

	//438
	void PlayerWrapper::SetProbability(float d)
	{
		if (d <= -0.001)
		{
			if (d < -1.5)
			{
				if (probability > 0)   //猜的    haokaixin
					probability = 100.0;
				else
					probability = 0.0;
			}
		}
		else
		{
			this->probability = d;
		}
	}

	//439
	bool PlayerWrapper::UseInk(int dec)
	{
		int ink = object.GetBasicProp().ink;
		if (ink <= dec)
			return object.DecInk(ink);
		else
			return object.DecInk(dec);
	}

	//440
	int PlayerWrapper::GetResisProf(int idx)
	{
		return this->resistance_proficiency[idx];
	}

	//441
	bool PlayerWrapper::SetCurseteleport(bool)
	{
		if (ThrowDice())
			object.CurseTeleport(this->caster);
		return 1;
	}

	//442
	int PlayerWrapper::GetVar6(void)
	{
		return this->var[5];
	}

	//443
	void PlayerWrapper::ModifyRegionReputation(int index, int rep)
	{
		object.ModifyRegionReputation(index, rep);
	}

	//444
	bool PlayerWrapper::SetBloodthirsty2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Bloodthirsty2(this->object, this->time, this->ratio));
		}
		return 1;
	}

	//445
	bool PlayerWrapper::SetUpdatecommondata(bool)
	{
		if (ThrowDice())
			object.ModifyGlobalValue(this->amount, this->value);
		return 1;
	}

	//446
	int PlayerWrapper::GetDarklightform(void)
	{
		return object.GetDarkLightForm();
	}

	//447
	bool PlayerWrapper::SetMiracleburstxian(bool)
	{
		object.AddFilter(new filter_Miracleburstxian(object,ratio * 1000.0,time,buffid,amount));
		return 1;
	}

	//448
	bool PlayerWrapper::SetMagicshield(bool)
	{
		if (this->ratio < 1.0 && this->amount > 1.0)
		{
			object.AddFilter(new filter_Magicshield(this->object, this->ratio, (int)this->amount, this->time));
		}
		return 1;
	}

	//449
	bool PlayerWrapper::SetAddanti(bool)
	{
		object.AddFilter(new filter_Addanti(this->object, (int)this->value, this->time));
		return 1;
	}

	//450
	bool PlayerWrapper::SetSummon(bool)
	{
		if (ThrowDice())
		{
			object.CreateSummon(value,amount,time,ratio,skill->GetLevel(), object.GetCurTargetID(), buffid != 0);
		}
		return 1;
	}

	//451
	bool PlayerWrapper::SetUnsummonclone(bool)
	{
		if (ThrowDice())
			object.UnSummonMonster(ratio);
		return 1;
	}

	//452
	bool PlayerWrapper::SetIncdefence(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incdefence(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//453
	bool PlayerWrapper::SetSummonmachine(bool)
	{
		object.CreateTrap(this->value, this->time, 5, skill->GetLevel(), ratio != 0, amount);
		return 1;
	}

	//454
	bool PlayerWrapper::SetPasinccrit(float inc)
	{
		if (this->context == 3)
			object.ImpairCrit((inc * 1000.0));
		else
			object.EnhanceCrit((inc * 1000.0));
		return 1;
	}

	//455
	bool PlayerWrapper::SetRecoverhp(bool)
	{
		object.AddFilter(new filter_Recoverhp(this->object, this->time, (int)this->value));
		return 1;
	}

	//456
	bool PlayerWrapper::SetExorcism(bool)
	{
		if (ThrowDice())
		{
			DoExoricism(this->buffid);
			DoExoricism((int)this->value);
			DoExoricism((int)this->amount);
		}
		return 1;
	}

	//457
	bool PlayerWrapper::GetInrange(A3DVECTOR& tpos, float tbody, float range, float& correction)
	{		
		float body = object.GetBodySize();
		correction = body + tbody;
		return (body + range + tbody) * (body + range + tbody) > object.GetPos().squared_distance(tpos);
	}

	//458
	bool PlayerWrapper::ThrowDice(int type)
	{
		this->probability = this->probability - object.GetExtendProp().resistance[type];
		return ThrowDice();
	}

	//459
	bool PlayerWrapper::SetDecDeityExp(int dec)
	{
		return object.DecDeityExp(dec);
	}

	//460
	bool PlayerWrapper::GetIscrit(void)
	{
		return object.IsCrit();
	}

	//461
	bool PlayerWrapper::IsMultiFilterExist(int id)
	{
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 1))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 2))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 3))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 4))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 5))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 6))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 7))
			return 1;
		if (object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 8))
			return 1;
		return object.IsFilterExist(10 * id + MULTI_FILTER_BEGIN + 9) != 0;
	}

	//462
	bool PlayerWrapper::SetPasadddodge(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairArmor((int)inc);
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceArmor((int)inc);
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//463
	bool PlayerWrapper::SetAddattack(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Addattack( this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//464
	bool PlayerWrapper::SetGuilin(bool)
	{
		if (! IsMultiFilterExist(MULTI_FILTER_VIPGUILIN) && ThrowDice())
		{
			object.AddFilter(new filter_Guilin(object,ratio * 1000.0,value,time,buffid));
		}
		return 1;
	}

	//465
	int PlayerWrapper::GetRes1(void)
	{
		return object.GetExtendProp().resistance[0];
	}

	//466
	int PlayerWrapper::GetVar5(void)
	{
		return this->var[4];
	}

	//467
	bool PlayerWrapper::SetDecfatalhurt(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Decfatalhurt(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//468
	int PlayerWrapper::GetRes4(void)
	{
		return object.GetExtendProp().resistance[3];
	}

	//469
	bool PlayerWrapper::SetFiring(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEFIRING) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				this->atstate |= 8u;
				filter_Firing *ffir = new filter_Firing(object,time,amount,value,ratio,buffid,skill->GetForceattack());
				ffir->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader,
					skill->GetAttackerFaction(), skill->GetTargetFaction());
				object.AddFilter(ffir);
			}
		}
		return 1;
	}

	//470
	bool PlayerWrapper::SetExtracthp(bool)
	{
		if ( ThrowDice())
		{
			object.SendHealMsg(skill->GetAttackerid(), skill->GetRealDamage() * this->ratio);
		}
		return 1;
	}

	//471
	bool PlayerWrapper::SetIncdrugeffect(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Incdrugeffect(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//472
	int PlayerWrapper::GetMinatk(void)
	{
		return object.GetExtendProp().damage_low;
	}

	//473
	bool PlayerWrapper::SetCleardebuff(bool)
	{
		if ( ThrowDice())
			object.ClearRandomSpecFilter( 0x10000000, this->amount);
		return 1;
	}

	//474
	bool PlayerWrapper::SetDecanti(bool)
	{
		object.AddFilter(new filter_Decanti(this->object, (int)(this->ratio * 1000.0), this->time));
		return 1;
	}

	//475
	bool PlayerWrapper::SetLightuniform(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Lightuniform(object,ratio,value,amount,time,buffid));
		}
		return 1;
	}

	//476
	bool PlayerWrapper::SetBloodshield(bool)
	{
		object.AddFilter(new filter_Bloodshield(object,ratio,value,amount,probability,time));
		return 1;
	}

	//477
	bool PlayerWrapper::SetIncfatalhurt(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Incfatalhurt(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//478
	int PlayerWrapper::GetType(void)
	{
		return object.GetObjectType();
	}

	//479
	bool PlayerWrapper::SetInccrithurt(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Inccrithurt(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//480
	void PlayerWrapper::AddCommonDebuff(int type, int time, int value, int buffid, bool rejected)
	{
		int rate = object.GetSkillWrapper().GetRejectRate();
		if (rejected || rate <= 0 || rand() % 100 >= rate)
		{
			q_extend_prop p = object.GetExtendProp();
			switch (type)
			{
			case 0:
				if ((object.GetImmuneMask() & IMMUNEDIZZY) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[0];
					if (pfilter - GetResisProf(0) < 0)
						pfilter = 0;
					filter_Dizzy* fdiz = new filter_Dizzy(object, time * 100.0 / (pfilter + 100.0) + 0.5);
					object.AddFilter(fdiz);
					object.OnFilterAdd( fdiz->GetFilterID(), this->caster);
				}
				break;
			case 1:
				if ((object.GetImmuneMask() & IMMUNESLEEP) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[4] - GetResisProf(4);
					if (pfilter < 0)
						pfilter = 0;
					filter_Sleep *fsle = new filter_Sleep(object,time * 100.0 / (pfilter + 100.0) + 0.5, value);
					object.AddFilter(fsle);
					object.OnFilterAdd(fsle->GetFilterID(), this->caster);
				}
				break;
			case 2:
				if ((object.GetImmuneMask() & IMMUNEWEAK) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[4] - GetResisProf(1);
					if (pfilter < 0)
						pfilter = 0;
					filter_Decattack *fdec = new filter_Decattack(object,value,time * 100.0 / (pfilter + 100.0) + 0.5,buffid);
					object.AddFilter(fdec);
					object.OnFilterAdd(fdec->GetFilterID(), this->caster);
				}
				break;
			case 3:
				if ((object.GetImmuneMask() & IMMUNEWRAP) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[4] - GetResisProf(2);
					if (pfilter < 0)
						pfilter = 0;
					filter_Wrap* fwra = new filter_Wrap(object, time * 100.0 / (pfilter + 100.0) + 0.5);
					object.AddFilter(fwra);
					object.OnFilterAdd(fwra->GetFilterID(), this->caster);
				}
				break;
			case 4:
				if ((object.GetImmuneMask() & IMMUNESLOW) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[4] - GetResisProf(5);
					if (pfilter < 0)
						pfilter = 0;
					filter_Slow* fslo = new filter_Slow(object, value, time * 100.0 / (pfilter + 100.0) + 0.5, buffid);
					object.AddFilter(fslo);
					object.OnFilterAdd(fslo->GetFilterID(), this->caster);
				}
				break;
			case 5:
				if ((object.GetImmuneMask() & IMMUNESILENT) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[4] - GetResisProf(3);
					if (pfilter < 0)
						pfilter = 0;
					filter_Silent* fsil = new filter_Silent(object, time * 100.0 / (pfilter + 100.0) + 0.5);
					object.AddFilter(fsil);
					object.OnFilterAdd(fsil->GetFilterID(), this->caster);
				}
				break;
			case 6:
				if ((object.GetImmuneMask() & IMMUNESLEEP) != 0)
				{
					atstate = atstate | 0x80;
				}
				else
				{
					int pfilter = p.resistance_tenaciy[4] - GetResisProf(4);
					if (pfilter < 0)
						pfilter = 0;
					filter_Dsleep* fdsl = new filter_Dsleep(object, time * 100.0 / (pfilter + 100.0) + 0.5, value);
					object.AddFilter(fdsl);
					object.OnFilterAdd(fdsl->GetFilterID(), this->caster);
				}
				break;
			default:
				return;
			}
		}
		else
		{
			atstate = atstate | 0x80;
			enchant_msg msg;
			memset(&msg, 0, sizeof(msg));
			msg.skill = 1711;
			msg.skill_level = 1;
			msg.force_attack = -1;
			msg.helpful = 0;
			msg.attack_range = 10.0;
			msg.skill_modify[0] = type;
			msg.skill_modify[1] = time;
			msg.skill_modify[2] = value;
			msg.skill_modify[3] = buffid;
			msg.is_mirror = 1;
			object.Enchant( skill->GetAttackerid(), msg);
		}
	}

	//481
	bool PlayerWrapper::SetPullback(bool)
	{
		if (ThrowDice())
		{
			if ( object.CanCharge(skill->GetAttackerid(), skill->GetAttackerpos(), skill->GetChargeType(), skill->GetChargeDist()))
			{
				object.TryCharge( skill->GetChargeType(), skill->GetAttackerpos(), skill->GetAttackerid().id);
			}
		}
		return 1;
	}

	//482
	bool PlayerWrapper::SetSubhide(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subhide(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//483
	bool PlayerWrapper::GetInrange(XID const& target, float range, float& correction, bool corpseonly)
	{
		A3DVECTOR tpos;
		float tbody = 0;
		int ret = object.QueryObject(target, tpos, tbody);
		if (!ret || corpseonly && ret != 2)
			return 0;
		float body = object.GetBodySize();
		correction = body + tbody;
		return (body + range + tbody) * (body + range + tbody) > object.GetPos().squared_distance(tpos);
	}

	//484
	bool PlayerWrapper::SetIncskilldodge(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Incskilldodge(object, ratio * 1000.0, time, buffid));
		}
		return 1;
	}

	//485
	bool PlayerWrapper::SetDivinityburst(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Divinityburst(object,ratio,value,time,buffid));
		}
		return 1;
	}

	//486
	bool PlayerWrapper::SetDeadlybless(bool)
	{
		object.AddFilter(new filter_Deadlybless(object,ratio,amount,probability,time));
		return 1;
	}

	//487
	bool PlayerWrapper::SetClearsilent(bool)
	{
		if ( ThrowDice())
			object.RemoveFilter(FILTER_SILENT);
		return 1;
	}

	//488
	bool PlayerWrapper::SetColdinjure(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Coldinjure(this->object, this->ratio, this->time, this->buffid));
		}
		return 1;
	}

	//489
	bool PlayerWrapper::SetEvilaura(bool)
	{
		if (!this->skill)
			return 0;
		this->result |= 2;
		if ( object.IsFilterExist(FILTER_HOLYAURA))
		{
			object.RemoveFilter(FILTER_HOLYAURA);
			return 1;
		}
		else
		{
			if ( object.IsFilterExist(FILTER_EVILAURA))
			{
				object.RemoveFilter(FILTER_EVILAURA);
			}
			else
			{
				filter_Evilaura *fevi = new filter_Evilaura(
					this->object,
					skill->GetId(),
					skill->GetLevel(),
					skill->GetRadius(),
					skill->GetCoverage(),
					skill->GetMpcost(),
					ratio,
					value,
					time,
					skill->GetForceattack(),
					this->var,
					skill->GetDpcost(),
					skill->GetHpcost());
				fevi->SetVisible(this->amount);
				object.AddFilter(fevi);
			}
			return 1;
		}
	}

	//490
	bool PlayerWrapper::SetSubmp(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Submp(this->object, (int)this->value, (int)this->amount, this->time, this->buffid));
		}
		return 1;
	}

	//491
	bool PlayerWrapper::SetWrap(bool)
	{
		if (this->probability < 0.001)
			return 0;
		if (ThrowDice(2))
		{
			if ((object.GetImmuneMask() & IMMUNEWRAP) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				AddCommonDebuff(3, this->time, 0, 0, 0);
			}
		}
		else
		{
			this->atstate |= 4;
		}
		return 1;
	}

	//492
	bool PlayerWrapper::SetPasdecfatalratio(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceAntiCritRate(-(int)(dec * 1000.0));
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceAntiCritRate((int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}

	//493
	bool PlayerWrapper::SetAddrage1(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Addrage1(this->object, this->ratio));
		}
		return 1;
	}

	//494
	bool PlayerWrapper::GetInteam(void)
	{
		return object.IsInTeam();
	}

	//495
	bool PlayerWrapper::SetMpgen(bool)
	{
		if (ThrowDice() && this->time > 0)
		{
			object.AddFilter(new filter_Mpgen( this->object, (int)this->value, (int)this->amount, this->time, this->buffid));
		}
		return 1;
	}

	//496
	bool PlayerWrapper::SetViphupo(bool)
	{
		if ( ThrowDice())
		{
			RemoveMultiFilter(MULTI_FILTER_HUPO);
			object.AddFilter(new filter_Viphupo(this->object, (int)(this->ratio * 1000.0), (int)this->value, this->buffid));
		}
		return 1;
	}

	//497
	bool PlayerWrapper::SetBefired(bool)
	{
		if ((object.GetImmuneMask() & IMMUNEFIRING) != 0)
		{
			atstate = atstate | 0x80;
		}
		else
		{
			filter_Befired *fbef = new filter_Befired(this->object, (int)this->value);
			fbef->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader, 
				skill->GetAttackerFaction(), skill->GetTargetFaction());
			object.AddFilter(fbef);
		}
		return 1;
	}

	//498
	bool PlayerWrapper::SetPasaddweak(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistance(1, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistance(1, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//499
	bool PlayerWrapper::SetFear(bool)
	{
		if ((object.GetImmuneMask() & 0x4000) != 0)
		{
			atstate = atstate | 0x80;
		}
		else if (ThrowDice())
		{
			object.AddFilter(new filter_Fear(this->object, this->time, skill->GetAttackerid(), value, ratio * 1000.0));
		}
		return 1;
	}

	//500
	bool PlayerWrapper::SetInchurt(bool)
	{
		if (this->context == 1 && ThrowDice())
		{
			object.AddFilter(new filter_Inchurt(this->object, this->ratio, this->time));
		}
		return 1;
	}

	//501
	bool PlayerWrapper::SetValue(float d)
	{
		this->value = d;
		return 1;
	}

	//502
	bool PlayerWrapper::SetDispel(bool)
	{
		if (ThrowDice())
		{
			int filterid = (int)this->ratio;
			if (filterid > 0)
			{
				if (filterid > 4095)
					object.RemoveFilter(ratio);
				else
					GNET::PlayerWrapper::RemoveMultiFilter(filterid);
			}
			filterid = (int)this->amount;
			if (filterid > 0)
			{
				if (filterid > 4095)
					object.RemoveFilter(filterid);
				else
					RemoveMultiFilter(filterid);
			}
			filterid = (int)this->value;
			if (filterid > 0)
			{
				if (filterid > 4095)
					object.RemoveFilter(filterid);
				else
					RemoveMultiFilter(filterid);
			}
		}
		return 1;
	}

	//503
	bool PlayerWrapper::SetAssault2(bool)
	{
		if (ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNETAUNT) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.AddFilter(new filter_Betaunted2(object, skill->GetAttackerid(), time));
			}
		}
		return 1;
	}

	//504
	bool PlayerWrapper::SetHpleak5(bool)
	{
		if (! ThrowDice())
			return 0;
		float Amount = GetAmount();
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | 0x80;
		}
		else if (Amount > 1)
		{
			if (! object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish(skill->GetAttackerlevel(), skill->GetLevel());
				Amount = Amount * Amount;
			}
			filter_Hpleak5 *fhpl = new filter_Hpleak5(this->object, this->time, Amount, (int)this->value, this->buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return 1;
	}

	//505
	bool PlayerWrapper::SetSubantisleep2(bool)
	{
		if (! object.IsPlayerClass())
			return 0;
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Subantisleep2(this->object, (int)this->value, this->time, (int)this->amount));
		}
		return 1;
	}

	//506
	bool PlayerWrapper::SetInvincible(bool)
	{
		object.SetInvincibleFilter(1, this->time, 1);
		object.AddFilter(new filter_Icon(this->object, this->time, 47, 10, 4124, 0));
		return 1;
	}

	//507
	int PlayerWrapper::GetRes2(void)
	{
		return object.GetExtendProp().resistance[1];
	}

	//508
	bool PlayerWrapper::SetGhostform(bool)
	{
		if (object.GetShape())
		{
			if (object.IsFilterExist(FILTER_GHOSTFORM))
			{
				object.RemoveFilter(FILTER_GHOSTFORM);
				return 0;
			}
			else
			{
				if (object.IsFilterExist(FILTER_INSANITYFORM))
					object.RemoveFilter(FILTER_INSANITYFORM);
			}
		}
		object.AddFilter(new filter_Ghostform(object, probability, buffid, value, ratio, amount));
		return 1;

	}

	//509
	bool PlayerWrapper::SetBloodpool(bool)
	{
		if ( ThrowDice())
		{
			int blood_capa = 0;
			if (! object.IsFilterExist(FILTER_BLOODPOOL) || (object.ModifyFilter(FILTER_BLOODPOOL, 15, &blood_capa, sizeof(blood_capa)), amount > blood_capa))   //15是取值
			{
				object.AddFilter(new filter_Bloodpool(this->object, this->time, (int)this->amount));
			}
		}
		return 1;
	}

	//510
	bool PlayerWrapper::SetMiracleburstmo(bool)
	{
		object.AddFilter(new filter_Miracleburstmo(object,ratio * 1000.0,time,buffid,amount));
		return 1;
	}

	//511
	bool PlayerWrapper::SetSeniorlightform(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Seniorlightform(object,ratio,value,amount,time,buffid));
		}
		return 1;
	}

	//512
	bool PlayerWrapper::SetHpleak(bool)
	{
		if (! ThrowDice())
			return 0;
		float Amount = GetAmount();
		if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
		{
			atstate = atstate | 0x80;
		}
		else if (Amount > 1)
		{
			if (! object.IsPlayerClass())
			{
				object.CalcLevelDamagePunish( skill->GetAttackerlevel(), skill->GetLevel());
				Amount = Amount * Amount;
			}
			filter_Hpleak *fhpl = new filter_Hpleak(this->object, this->time, Amount, (int)this->value, this->buffid);
			fhpl->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
			object.AddFilter(fhpl);
		}
		return 1;
	}

	//513
	bool PlayerWrapper::SetDecskilldamage(bool)
	{
		object.AddFilter(new filter_Decskilldamage(this->object, this->ratio, this->time, this->buffid));
		return 1;
	}

	//514
	bool PlayerWrapper::SetDecdefence(bool)
	{
		if ( ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEDECDEFSCALE) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.AddFilter(new filter_Decdefence(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
			}
		}
		return 1;
	}

	//515
	bool PlayerWrapper::TargetIsSpouse(void)
	{
		return this->target && this->tsize && object.IsSpouse(*this->target);
	}

	//516
	bool PlayerWrapper::SetGthpgen(bool)
	{
		object.AddFilter(new filter_Gthpgen(this->object, (int)this->value, (int)this->amount, this->buffid));
		return 1;
	}

	//517
	int PlayerWrapper::GetDef(void)
	{
		return object.GetExtendProp().defense;
	}

	//518
	int PlayerWrapper::GetBufflevel(int id)
	{
		int level = 0;
		object.ModifyFilter(id, 13, &level, sizeof(level));
		return level;
	}

	//519
	bool PlayerWrapper::SetDecmp(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Decmp(this->object, (int)(this->ratio * 1000.0), this->time));
		}
		return 1;
	}

	//520
	bool PlayerWrapper::SetResurrect(bool)
	{
		if ( object.IsDead())
		{
			if ( GetLevel() <= 14 || this->ratio < 0.0)
				this->ratio = 0.0;
			object.Resurrect(this->ratio);
		}
		return 1;
	}

	//521
	bool PlayerWrapper::SetInchitrate(float _ratio)
	{
		if (this->context == 3)
		{
			object.ImpairScaleAttack(_ratio * 1000.0);
			object.UpdateAttackData();
		}
		else
		{
			object.EnhanceScaleAttack(_ratio * 1000.0);
			if (this->context == 4)
				object.UpdateAttackData();
		}
		return 1;
	}
	//522
	bool PlayerWrapper::SetBreak(bool)
	{
		if ( ThrowDice())
			object.BreakCurAction();
		return 1;
	}

	//523
	bool PlayerWrapper::SetScaleinchp(bool)
	{
		if ( ThrowDice())
		{
			object.Heal(GetMaxhp() * this->ratio, 0, 0);
		}
		return 1;
	}

	//524
	bool PlayerWrapper::SetScaleincmp(bool)
	{
		if ( ThrowDice())
		{
			object.InjectMana(GetMaxmp() * this->ratio, 0);
		}
		return 1;
	}

	//525
	bool PlayerWrapper::SetScaledechp(bool)
	{
		if ( ThrowDice())
		{
			if ((object.GetImmuneMask() & IMMUNEHPLEAK) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.DecHP(GetMaxhp() * this->ratio);
			}
		}
		return 1;
	}

	//526
	bool PlayerWrapper::SetScaledecmp(bool)
	{
		if ( ThrowDice())
		{
			object.DrainMana(GetMaxmp() * this->ratio);
		}
		return 1;
	}

	//527
	bool PlayerWrapper::SetPasincefence(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleDefense( (int)inc);
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceScaleDefense((int)inc);
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//528
	bool PlayerWrapper::SetPasaddsilenttenaciy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceTenaciy(3, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceTenaciy(3, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//529
	bool PlayerWrapper::SetPasaddsleeptenaciy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceTenaciy(4, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceTenaciy(4, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//530
	bool PlayerWrapper::SetPasaddslowtenaciy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceTenaciy(5, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceTenaciy(5, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//531
	bool PlayerWrapper::SetPasaddwraptenaciy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceTenaciy(2, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceTenaciy(2, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//532
	bool PlayerWrapper::SetPasaddsilentproficiency(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceProficiency(3, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceProficiency(3, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//533
	bool PlayerWrapper::SetPasaddweakproficiency(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceProficiency(1, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceProficiency(1, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//534
	bool PlayerWrapper::SetPasadddizzyproficiency(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceProficiency(0, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceProficiency(0, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//535
	bool PlayerWrapper::SetPasaddsleepproficiency(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceProficiency(4, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceProficiency(4, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//536
	bool PlayerWrapper::SetPasaddslowproficiency(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceProficiency(5, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceProficiency(5, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//537
	bool PlayerWrapper::SetPasaddwrapproficiency(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairResistanceProficiency(2, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceResistanceProficiency(2, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//538
	bool PlayerWrapper::SetOverawe(bool)
	{
		if ( object.IsPlayerClass())
			return 0;
		if (this->context == 1 && ThrowDice( 0))
		{
			if ((object.GetImmuneMask() & IMMUNEDIZZY) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				AddCommonDebuff( 0, this->time, 0, 0, 0);
			}
		}
		else
		{
			this->atstate |= 4u;
		}
		return 1;
	}

	//539
	bool PlayerWrapper::SetPowerup(bool)
	{
		object.AddFilter(new filter_Powerup(object, ratio * 1000.0, value * 1000.0, time));
		return 1;
	}

	//540
	bool PlayerWrapper::SetShouyi(bool)
	{
		object.AddFilter(new filter_Shouyi(this->object, (int)this->amount, (int)this->value, this->time));
		return 1;
	}

	//541
	bool PlayerWrapper::SetPasincantidizzy(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(0, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(0, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//542
	bool PlayerWrapper::SetPasincantiweak(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(1, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(1, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//543
	bool PlayerWrapper::SetPasincantiwrap(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(2, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(2, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//544
	bool PlayerWrapper::SetPasincantisilent(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(3, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(3, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//545
	bool PlayerWrapper::SetPasincantisleep(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(4, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(4, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//546
	bool PlayerWrapper::SetPasincanti(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleResistance(0, (int)inc);
			object.ImpairScaleResistance(1, (int)inc);
			object.ImpairScaleResistance(2, (int)inc);
			object.ImpairScaleResistance(3, (int)inc);
			object.ImpairScaleResistance(4, (int)inc);
			object.ImpairScaleResistance(5, (int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleResistance(0, (int)inc);
			object.EnhanceScaleResistance(1, (int)inc);
			object.EnhanceScaleResistance(2, (int)inc);
			object.EnhanceScaleResistance(3, (int)inc);
			object.EnhanceScaleResistance(4, (int)inc);
			object.EnhanceScaleResistance(5, (int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//547
	bool PlayerWrapper::SetIncheal(bool)
	{
		object.AddFilter(new filter_Incheal(this->object, this->ratio, this->time, this->buffid));
		return 1;
	}

	//548
	bool PlayerWrapper::SetDirectattack(bool)
	{
		if (this->context == 1)
		{
			if (this->skill)
			{
				int damage = skill->GetDamage() -  GetDef();
				if (damage > 0)
				{
					object.BeHurt(skill->GetAttackerid(), skill->GetAttacker(), damage, invader, skill->GetAttackerMode());
				}
			}
		}
		return 1;
	}

	//549
	bool PlayerWrapper::SetPasdechurt4(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultDefense( 3, -(int)(dec * 1000.0));
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceCultDefense(3, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//550
	bool PlayerWrapper::SetPasdechurt5(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultDefense( 4, -(int)(dec * 1000.0));
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceCultDefense(4, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//551
	bool PlayerWrapper::SetPasdechurt6(float dec)
	{
		if (this->context == 3)
		{
			object.EnhanceCultDefense(5, -(int)(dec * 1000.0));
			object.UpdateDefenseData();
		}
		else
		{
			object.EnhanceCultDefense(5, (int)(dec * 1000.0));
			if (this->context == 4)
				object.UpdateDefenseData();
		}
		return 1;
	}

	//552
	bool PlayerWrapper::SetCreditboost(bool)
	{
		if ( ThrowDice() && this->time > 0)
		{
			object.AddFilter(new filter_Creditboost(object, value, amount, time));
		}
		return 1;
	}

	//553
	bool PlayerWrapper::SetHurtscatter(bool)
	{
		object.AddFilter(new filter_Hurtscatter(object,value,probability,ratio,amount,buffid,
			skill->GetForceattack(),time));
		return 1;
	}

	//554
	bool PlayerWrapper::SetIncattack2(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Incattack2(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//555
	bool PlayerWrapper::SetSlow2(bool)
	{
		if ( ThrowDice( 5))
		{
			if ((object.GetImmuneMask() & IMMUNESLOW) != 0)
			{
				atstate = atstate | 0x80;
			}
			else
			{
				object.AddFilter(new filter_Slow2(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
			}
		}
		return 1;
	}

	//556
	bool PlayerWrapper::SetIncdefence2(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Incdefence2(this->object, (int)(this->ratio * 1000.0), this->time, this->buffid));
		}
		return 1;
	}

	//557
	bool PlayerWrapper::SetExtractmp(bool)
	{
		if ( ThrowDice())
		{
			object.SendAddManaMsg(skill->GetAttackerid(), skill->GetRealDamage() * this->ratio);
		}
		return 1;
	}

	//558
	bool PlayerWrapper::SetImmunetransform(bool)
	{
		object.StopTransform(object.GetTransformID(), 1);
		object.AddFilter(new filter_Immunetransform(this->object, this->time));
		return 1;
	}

	//559
	bool PlayerWrapper::SetPasaddtransform(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairAntiTransform((int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceAntiTransform((int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//560
	bool PlayerWrapper::SetPasincantitransform(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleAntiTransform((int)inc);
			object.UpdateMagicData();
		}
		else
		{
			object.EnhanceScaleAntiTransform((int)inc);
			if (this->context == 4)
				object.UpdateMagicData();
		}
		return 1;
	}

	//561
	bool PlayerWrapper::SetAddantitransform(bool)
	{
		object.AddFilter(new filter_Addantitransform(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//562
	bool PlayerWrapper::SetSubantitransform(bool)
	{
		object.AddFilter(new filter_Subantitransform(this->object, (int)this->value, this->time, this->buffid));
		return 1;
	}

	//563
	bool PlayerWrapper::SetDrawclonelife(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Drawclonelife(this->object, (int)this->value, this->time));
		}
		return 1;
	}

	//564
	bool PlayerWrapper::SetBackstab(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Backstab( this->object, (int)this->value, this->time));
		}
		return 1;
	}

	//565
	bool PlayerWrapper::SetCritcurse(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Critcurse(this->object, this->time, (int)this->amount));
		}
		return 1;
	}

	//566
	bool PlayerWrapper::SetSubSpot(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Subspot(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//567
	bool PlayerWrapper::SetAddhide(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Addhide(this->object, (int)this->value, this->time, this->buffid));
		}
		return 1;
	}

	//568
	bool PlayerWrapper::GetIsfury(void)
	{
		return object.IsFuryState();
	}

	//569
	bool PlayerWrapper::SetPasincdpgen(float inc)
	{
		if (this->context == 3)
		{
			object.ImpairScaleDPGen((int)(inc * 1000.0));
			object.UpdateDP();
		}
		else
		{
			object.EnhanceScaleDPGen((int)(inc * 1000.0));
			if (this->context == 4)
				object.UpdateDP();
		}
		return 1;
	}

	//570
	bool PlayerWrapper::SetCrippled(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Crippled(this->object, this->time, (int)this->value));
		}
		return 1;
	}

	//571
	bool PlayerWrapper::SetPermitcyclearea(bool)
	{
		if ( ThrowDice())
		{
			if (object.IsFilterExist(FILTER_PERMITCYCLEAREA))
			{
				cycle_area ca;
				ca.owner = skill->GetAttackerid().id;
				ca.center = skill->GetAttackerpos();
				ca.radius = this->value;
				ca.time = this->time;
				object.ModifyFilter(FILTER_PERMITCYCLEAREA, 8, &ca, sizeof(ca));
			}
			else
			{
				filter_Permitcyclearea* fper = new filter_Permitcyclearea(this->object, this->time, skill->GetAttackerpos(), value);
				fper->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
				object.AddFilter(fper);
			}
		}
		return 1;
	}

	//572
	bool PlayerWrapper::SetLimitcyclearea(bool)
	{
		if ( ThrowDice())
		{
			if ( object.IsFilterExist(4217))
			{
				cycle_area ca;
				ca.owner = skill->GetAttackerid().id;
				ca.center = skill->GetAttackerpos();
				ca.radius = this->value;
				ca.time = this->time;
				object.ModifyFilter(4217, 8, &ca, sizeof(ca));
			}
			else
			{
				filter_Limitcyclearea* flim = new filter_Limitcyclearea(this->object, this->time, skill->GetAttackerpos(), value);
				flim->SetUp(skill->GetAttackerid(), skill->GetAttacker(), skill->GetAttackerMode(), invader);
				object.AddFilter(flim);
			}
		}
		return 1;
	}

	//573
	bool PlayerWrapper::SetGuishen1debuff(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Guishen1debuff(object,time,ratio * 100.0,amount * 1000.0,value * 100.0,buffid));
		}
		return 1;
	}

	//574
	bool PlayerWrapper::SetTuoling1debuff(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Tuoling1debuff(object,time,ratio * 100.0,amount * 1000.0,value * 100.0,buffid));
		}
		return 1;
	}

	//575
	bool PlayerWrapper::SetTuoling2debuff(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Tuoling2debuff(object,time,ratio * 100.0,amount * 1000.0,value * 100.0,buffid));
		}
		return 1;
	}

	//576
	bool PlayerWrapper::SetPzmaxspeed(bool)
	{
		object.AddFilter(new filter_Pzmaxspeed(this->object));
		return 1;
	}

	//577
	bool PlayerWrapper::SetPzhalfspeed(bool)
	{
		object.AddFilter(new filter_Pzhalfspeed(this->object));
		return 1;
	}

	//578
	bool PlayerWrapper::SetPzhill(bool)
	{
		object.AddFilter(new filter_Pzhill(this->object, this->time));
		return 1;
	}

	//579
	bool PlayerWrapper::SetPznomove(bool)
	{
		object.AddFilter(new filter_Pznomove( this->object, this->time));
		return 1;
	}

	//580
	bool PlayerWrapper::SetPzchaos(bool)
	{
		object.AddFilter(new filter_Pzchaos(this->object, this->time));
		return 1;
	}

	//581
	bool PlayerWrapper::SetPzvoid(bool)
	{
		object.AddFilter(new filter_Pzvoid(this->object, this->time));
		return 1;
	}

	//582
	bool PlayerWrapper::SetActivebeattacked(bool)
	{
		object.AddFilter(new filter_Activebeattacked(object,probability,time,value,ratio,buffid,skill->GetForceattack()));
		return 1;
	}

	//583
	BuffLevelWrapper* PlayerWrapper::GetBufflevel(void)
	{
		return &this->buffwrapper;
	}

	//584
	bool PlayerWrapper::SetDarklaser(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Darklaser(this->object, this->time, (int)this->value));
		}
		return 1;
	}

	//585
	bool PlayerWrapper::SetDirstate(bool)
	{
		object.AddFilter(new filter_Dirstate(this->object, this->time, (int)this->value, (int)this->amount));
		return 1;
	}

	//586
	bool PlayerWrapper::SetShowyb(bool)
	{
		object.AddFilter(new filter_Showyb(this->object, this->time));
		return 1;
	}

	//587
	bool PlayerWrapper::SetSummoncontroltrap(bool)
	{
		object.CreateTrap((int)this->value, this->time, 7, skill->GetLevel(), this->ratio != 0, amount);
		return 1;
	}

	//588
	bool PlayerWrapper::SetSubantiweak2(bool)
	{
		if (! object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subantiweak2(this->object, (int)this->value, this->time, (int)this->amount));
		}
		return 1;
	}

	//589
	bool PlayerWrapper::SetSubantislow2(bool)
	{
		if (! object.IsPlayerClass())
			return 0;
		if (ThrowDice())
		{
			object.AddFilter(new filter_Subantislow2(this->object, (int)this->value, this->time, (int)this->amount));
		}
		return 1;
	}

	//590
	bool PlayerWrapper::SetSubmp2(bool)
	{
		if (! object.IsPlayerClass())
			return 0;
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Submp2(this->object, (int)this->value, this->time, (int)this->amount));
		}
		return 1;
	}

	//591
	bool PlayerWrapper::SetRenma1(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Renma1(this->object));
		}
		return 1;
	}

	//592
	bool PlayerWrapper::SetRenma2(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Renma2(this->object));
		}
		return 1;
	}

	//593
	bool PlayerWrapper::SetRenma3(bool)
	{
		if (ThrowDice())
		{
			object.AddFilter(new filter_Renma3(this->object));
		}
		return 1;
	}

	bool PlayerWrapper::SetRenma4(bool)
	{
		if ( ThrowDice())
		{
			object.AddFilter(new filter_Renma4(this->object));
		}
		return 1;
	}

	//========================PlayerWrapper====end

}
// end namespace GNET
