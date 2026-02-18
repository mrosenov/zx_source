#ifndef __SKILL_PLAYERWRAPPER_H
#define __SKILL_PLAYERWRAPPER_H

#include "common/types.h"
#include "obj_interface.h"
//#include "statedef.h"
//#include "../header/attack.h"

//#ifdef _SKILL_SIMULATION 
//#define VIRTUALFUN virtual
//#else
//#define VIRTUALFUN
//#endif

#ifdef WIN32
#ifndef uint
#define uint unsigned int
#endif // !uint
#endif

#include "skillfunction.h"

namespace GNET
{
	// 
class Skill;
class SkillWrapper;
//
//#define  COOLINGID_BEGIN 1024

	class PlayerWrapper;

	struct BuffLevelWrapper
	{
		PlayerWrapper* mPlayer = 0;
	};

	class PlayerWrapper
	{
	//public:
	//	typedef unsigned int	SID;
	protected:
		object_interface object;
		char    context;   // 0:施法者 1:被施法者 2:被动技能生效 3:被动技能失效 4:被动技能学习
		Skill * skill;
		BuffLevelWrapper buffwrapper;
		const XID * target;
		int         tsize;
		
		//TargetInfoWrapper _targetinfo;
		
		// 临时参数
		float	probability;	// 状态攻击参数
		float	ratio;		// 状态攻击参数
		float	amount;		// 状态攻击参数
		float	value;		// 状态攻击参数
		int 	time;		// 持续时间
		//bool 	tiny_time;		 
		int     buffid;		// 是否显示状态图标
		
		XID caster;
		int     health;         // 治疗生命值
		//bool    success;
		//bool    immune;
		int result;
		int atstate;
		bool    invader;
		
		//bool	succaddfilter;
		int var[16];  //0x50  = 80
		int resistance_proficiency[6];  //0x90  = 144

	public:
		PlayerWrapper(
			object_interface,
			char _type,
			Skill* _s,
			const XID* _target,
			int _size);
		~PlayerWrapper();
		bool SetDisperseanti(bool a2);
		int GetDarklight();
		bool SetDiscover(bool a2);
		bool SetFrenzied(bool a2);
		void SetVar2(int v);
		void SetVar1(int v);
		bool SetSecondattack(bool a2);
		bool SetQilinform(bool a2);
		bool SetClearcooldown(bool a2);
		bool SetInsanityform(bool a2);
		bool SetDizzytimer(bool a2);
		bool SetDsleep(bool b);
		void SetVar6(int v);
		bool SetDrunk(bool a2);
		int GetVar4();
		bool SetDamagemove(bool a2);
		bool SetPasaddattack(float inc);
		int GetAtkrate();
		bool SetUniqprompt(bool a2);
		void SetVar10(int v);
		bool SetTongxin(bool);
		bool SetPasaddsleep(float);
		bool SetSubantiwrap2(bool);
		bool SetSharestatewithclone(bool);
		bool SetDecskillaccu(bool);
		bool SetYuanling(bool);
		bool SetIceshield(bool);
		bool SetSleep(bool);
		bool SetCycsubattack(bool);
		bool SetBaradddogeaccu(bool);
		bool SetJumptospouse(bool);
		bool ThrowDiceDiet(void);
		bool SetBounceto(bool);
		bool SetPasincpetdamage(float);
		int TakeOutItem(int, int, int);
		bool IsXuanYuan(void);
		bool SetMp(float);
		bool SetEnmity(int);
		void SetVar16(int);
		bool SetMpdisperse(bool);
		bool SetDimming(bool);
		bool SetInchurt2(bool);
		void DoPerform(void);
		bool SetBreakcasting(bool);
		bool IsFlying(void);
		bool SetCycsubdefence(bool);
		int GetAttackHigh(float, float);
		bool SetDarkuniform(bool);
		bool SetAddantisilent(bool);
		bool SetAddattack4(bool);
		bool SetHpleak2(bool);
		bool SetSetskilllevel(bool);
		bool SetHpregain(bool);
		int GetForm(void);
		int GetReborncount(void);
		bool SetDarkness(bool);
		bool SetTriggerskill(bool);
		bool SetExpboost(bool);
		bool SetUnsummon(bool);
		bool SetNingjin(bool);
		bool SetAmount(float);
		bool SetClearbuff(bool);
		bool SetBarexpboost(bool);
		float GetCrithurt(void);
		bool SetBarmpgen(bool);
		int GetAttackLow(float, float);
		bool SetShadowdance(bool);
		bool SetPowerless(bool);
		bool SetPasaddweaktenaciy(float);
		int GetVar3(void);
		bool SetSubhp(bool);
		bool SetTalismaneffects(bool);
		int GetVar8(void);
		bool SetSalvation(bool);
		bool SetTeamenchant(bool);
		bool SetImmunesleep(bool);
		void SendResult(XID const&, int);
		float GetSaint(void);
		bool CanCharge(XID const&, A3DVECTOR const&, char, float);
		bool SetDecaccuracy(bool);
		int GetSkilllevel(int);
		int GetCyclemembercnt(void);
		bool SetSkillreflect(bool);
		bool SetWraptimer(bool);
		bool SetDectp(int);
		bool SetReturnpos(bool);
		bool SetPasadddizzytenaciy(float);
		bool SetSummonteleport2(bool);
		bool SetParalysis(bool);
		int GetRes6(void);
		bool SetAutorescurrect(bool);
		void SetVar9(int);
		bool SetBlowoff(bool);
		bool SetInchpgen(bool);
		int GetDebuffcnt(void);
		bool IsBindState(void);
		bool SetPasdecfatalhurt(float);
		int IsMarried(void);
		int GetVar9(void);
		bool SetPasincskillaccu(float);
		bool SetVipguilin(bool);
		bool SetClearslow(bool);
		bool IsFilterExist(int);
		bool SetDirecthurt(bool);
		bool SetIncskilldamage(bool);
		bool SetMirrorimage(bool);
		bool SetAddcharm(bool);
		bool SetDeccrithurt(bool);
		bool SetDecdamage(bool);
		int GetCultivation(void);
		int GetDp(void);
		bool SetMagicdoor(bool);
		bool SetPasdechurt3(float);
		bool SetFamilyincattack(bool);
		bool SetIncfatalratio(bool);
		bool SetActiveonfilteradd(bool);
		bool SetCrazy(bool);
		bool SetAdddefence(bool);
		bool SetHupo(bool);
		bool SetImmuneweak(bool);
		bool SetRepel2(bool);
		bool GetHasbuff(int);
		bool SetGuishen2debuff(bool);
		bool SetHpleak1(bool);
		bool SetSubantiweak(bool);
		bool SetPassubhurt(float);
		bool IsMaster(void);
		void SetVar8(int);
		int GetInk(void);
		void SetRage(int);
		void SetTalentData(int*);
		bool SetPasinchpgen(float);
		bool SetDechurt2(bool);
		bool SetIncskillaccu(bool);
		bool SetPasaddaccuracy(float);
		bool SetSummonskill(bool);
		bool SetImmunesilent(bool);
		bool GetHasmultbuff(int);
		void SetSkill(Skill* s);
		bool SetPasaddslow(float);
		bool CheckItem(int, int, uint);
		float GetAmount(void);
		bool SetSummonclone(bool);
		bool SetReduceskillcd(bool);
		int GetCrit(void);
		bool SetJuniordarkform(bool);
		bool SetCrazycurse(bool);
		bool SetDecProficiency(uint, int);
		bool SetInvader(bool);
		bool SetClearwrap(bool);
		bool SetPuppetform(bool);
		bool SetGohome(bool);
		bool SetSharelifewithsummon(bool);
		int GetRes5(void);
		int GetMaxmp(void);
		bool SetGtmpgen(bool);
		bool SetSilenttimer(bool);
		bool SetSubattack(bool);
		float GetProbability(void);
		bool SetCrippleddebuff(bool);
		bool SetSpytrap(bool);
		bool SetFamilyincmp(bool);
		int GetHp(void);
		bool SetPasdecrestrainfo(float);
		bool SetSubdefence(bool);
		int GetPuppetid(void);
		int GetAtstate(void);
		void RemoveMultiFilter(int);
		bool SetBuffid(int);
		bool SetMakecrit(bool);
		bool SetAddattack3(bool);
		bool SetCursed(bool);
		bool SetRejectdebuff(bool);
		void CalcCost(void);
		void DoExoricism(int);
		bool UseMp(int);
		int GetVar1(void);
		bool SetHuju(bool);
		bool SetCreateobject(bool);
		bool SetSubantidizzy(bool);
		int64_t GetDTExp(void);
		bool SetDie(bool);
		bool SetAdddodge2(bool);
		bool SetDrawto(bool);
		bool SetSubantisleep(bool);
		bool SetActivebecrit(bool);
		bool CanAttack(void);
		void SetVar3(int);
		bool SetSubantislow(bool);
		bool SetHeal(bool);
		void SetDarklight(int);
		int GetDmgreduce(void);
		int GetSkillrate(void);
		int GetMp(void);
		bool SetBackorigin(bool);
		bool SetCleartransform(bool);
		bool SetSharelifewithclone(bool);
		bool GetIsfrozen(void);
		bool SetXinmo(bool);
		bool SetAddcommon(bool);
		bool SetDelaycast(bool);
		bool SetAddspirit(bool);
		bool SetLongxiang(bool);
		bool SetSubhp2(bool);
		bool SetInchp2(bool);
		bool SetInstantskill(bool);
		bool SetHpleak6(bool);
		bool SetGuishen1(bool);
		bool SetAdddebuff(bool);
		bool SetDrainmagic(bool);
		bool SetPasincmp(float);
		bool SetBlessed(bool);
		bool SetStateexchangewithclone(bool);
		bool SetPasincattack(float);
		bool SetChihun(bool);
		bool SetTransform(bool);
		bool SetPulling(bool);
		bool SetPasincmpgen(float);
		bool SetPasdecpethurt(float);
		float GetSpeed(void);
		float GetValue(void);
		bool GetInrange(float, float&, bool);
		bool SetCycle(bool);
		bool SetBarhpgen(bool);
		bool SetIncskilllevel(bool);
		bool SetAddwrap(bool);
		bool SetAddattack2(bool);
		bool SetDeccritrate(bool);
		bool SetInchp(bool);
		bool SetDecskilldodge(bool);
		int GetRace(void);
		bool SetIncattack3(bool);
		bool SetActivecrit(bool);
		void SetResisProf(int, int);
		bool SetFly(bool);
		bool SetBepulled(bool);
		bool SetAddhp(bool);
		bool SetPasaddmp(float);
		bool SetAddantiweak(bool);
		bool SetIncanti(bool);
		bool SetPasaddwrap(float);
		bool SetFamilyinchp(bool);
		int GetBuffcnt(void);
		bool SetSetcommondata(bool);
		bool SetAccumdamage(bool);
		bool SetSpicy(bool);
		bool SetAsfire(bool);
		int GetVar2(void);
		const XID& GetXid(void);
		bool SetAddslow(bool);
		int GetMaxhp(void);
		bool SetPasaddhp(float);
		bool SetDechurt(bool);
		bool SetDouhun(bool);
		bool SetAddantidizzy(bool);
		bool SetJuqi(bool);
		int GetSkilldodge(void);
		bool SetAoshi(bool);
		bool SetPasdechurt2(float);
		bool SetAdddodge(bool);
		int GetVar7(void);
		bool SetFocusanti(bool);
		bool SetAddanti2(bool);
		bool SetGuishen2(bool);
		bool SetSpeedto(bool);
		bool SetYanyu(bool);
		bool SetBanruo(bool);
		bool SetSetcooldown(bool);
		bool SetPasdecrestrainxian(float);
		bool SetWeak(bool);
		bool SetHp(float);
		bool SetTime(float);
		bool SetCycdecdefence(bool);
		bool SetXiaoxin(bool);
		bool ThrowDice(void);
		int GetRes3(void);
		bool SetAbsulotearea(bool);
		bool SetPasaddsilent(float);
		bool SetClearsleep(bool);
		bool SetCycleskill(bool);
		bool SetRandcurse(bool);
		bool CheckItem(int, uint);
		bool SetPasincskilldodge(float);
		bool SetHumanbomb(bool);
		bool SetThrowback(bool);
		bool SetCreateitem(bool);
		bool SetAloof(bool);
		bool SetJuniorlightform(bool);
		int GetGender(void);
		bool SetSummonteleport1(bool);
		float GetRange(void);
		bool SetCibei(bool);
		bool SetDecdrugeffect(bool);
		bool SetIncmpgen(bool);
		bool SetTiansha(bool);
		bool SetGoback(bool);
		bool SetBleeding(bool);
		bool SetDivinityfury(bool);
		bool SetEquipeffect(bool);
		bool SetFrozen(bool);
		bool SetPasdechurt1(float);
		bool SetHpleak3(bool);
		int GetDodge(void);
		bool SetPerform(int);
		bool SetAddantislow(bool);
		bool SetAddweak(bool);
		bool SetDecdodge(bool);
		bool SetMpleak(bool);
		bool SetAddspot(bool);
		bool SetZhaoqi(bool);
		int GetVar16(void);
		bool SetSeed(bool);
		bool SetSubantisilent(bool);
		bool IsRenMa(void);
		bool UseDp(int);
		bool SetTuoling2(bool);
		bool SetSleeptimer(bool);
		bool SetMiracleburstfo(bool);
		bool SetHpgen(bool);
		bool SetAssault(bool);
		bool SetSummontrap(bool);
		bool SetAswind(bool);
		int GetHide(void);
		bool GetIsignite(void);
		void SetVar7(int);
		bool SetJaillimitcyclearea(bool);
		bool SetCatchpet(bool);
		bool SetJailpermitcyclearea(bool);
		bool SetPossummon(bool);
		bool SetSubantidizzy2(bool);
		int TakeOutItem(int, int);
		bool SetFogstate(bool);
		bool SetDodge(bool);
		bool SetHpleak4(bool);
		bool SetAddantiwrap(bool);
		bool SetRmhumanform(bool);
		bool GetSuccess(void);
		bool SetIgnite(bool);
		bool SetDiet(bool);
		void SetVar(int, uint);
		bool SetRepel(bool);
		bool SetContext(char);
		bool SetPasadddefence(float);
		bool SetAttack2hp(bool);
		bool CheckTarget(float, bool);
		bool SetTuoling1(bool);
		bool SetIncattack(bool);
		bool SetPasadddizzy(float);
		bool SetFoxstate(bool);
		bool SetHolyaura(bool);
		bool SetPuppetskill(bool);
		bool SetShadowhide(bool);
		bool SetInccritrate(bool);
		bool SetImmuneslow(bool);
		int GetRegionReputation(int);
		bool SetDecsp(int);
		int GetRage(void);
		int GetSglevel(void);
		bool SetRatio(float);
		bool SetIncmp(bool);
		bool SetSilent(bool);
		bool SetPasdecrestrainmo(float);
		bool SetBarehanded(bool);
		bool SetDizzy(bool);
		bool SetGloss(bool);
		void SetVar5(int);
		int GetOccupation(void);
		bool SetTurnbuff(bool);
		bool SetPray(int);
		bool SetSeniordarkform(bool);
		bool SetDecfatalratio(bool);
		bool SetPasinchp(float);
		int GetMaxdp(void);
		bool SetTalismaneffectsbless(bool);
		bool SetPosexchangewithclone(bool);
		bool SetGtaddattackdefense(bool);
		bool SetSubantisilent2(bool);
		bool SetScopedamage(bool);
		bool SetSkillmirror(bool);
		bool SetSlow(bool);
		bool SetAddantisleep(bool);
		bool SetDeathscatter(bool);
		bool SetSpiritdrag(bool);
		int GetTmplid(void);
		bool SetImmunewrap(bool);
		bool SetClearweak(bool);
		void SetVar4(int);
		bool SetPasaddsmite(float);
		void SetCaster(XID const&);
		bool SetTurndebuff(bool);
		int GetVar10(void);
		bool SetPasincantislow(float);
		void ConsumeItem(void);
		void UseHp(int);
		bool SetRetort(bool);
		bool SetAsforest(bool);
		bool SetAddspeed(bool);
		bool SetJail(bool);
		bool SetAddrage2(bool);
		bool SetAttack2mp(bool);
		bool SetImmunedizzy(bool);
		bool SetBuxiu(bool);
		void SetVar11(int);
		bool SetPasdechurt(float);
		int GetLevelSum(int);
		bool SetTiangu(bool);
		bool IsCloneExist(void);
		bool SetDodgeregain(bool);
		bool SetCleardizzy(bool);
		bool SetSubantiwrap(bool);
		bool SetSwift(bool);
		int GetMaxatk(void);
		bool SetBloodthirsty(bool);
		bool SetAshill(bool);
		bool SetAddhp2(bool);
		int GetVar11(void);
		void AddGroupAttackExcludeMonsterTids(std::vector<exclude_target>& vec);
		bool SetIncmountspeed(bool);
		bool SetRecordpos(bool);
		bool IsMount(void);
		bool SetPasaddanti(float);
		bool SetAddmp(bool);
		bool SetSubanti(bool);
		bool SetCycdecattack(bool);
		int GetBaseLevel(int);
		bool SetGangdan(bool);
		bool SetFashen(bool);
		int GetLevel(void);
		void SetProbability(float);
		bool UseInk(int);
		int GetResisProf(int);
		bool SetCurseteleport(bool);
		int GetVar6(void);
		void ModifyRegionReputation(int, int);
		bool SetBloodthirsty2(bool);
		bool SetUpdatecommondata(bool);
		int GetDarklightform(void);
		bool SetMiracleburstxian(bool);
		bool SetMagicshield(bool);
		bool SetAddanti(bool);
		bool SetSummon(bool);
		bool SetUnsummonclone(bool);
		bool SetIncdefence(bool);
		bool SetSummonmachine(bool);
		bool SetPasinccrit(float);
		bool SetRecoverhp(bool);
		bool SetExorcism(bool);
		bool GetInrange(A3DVECTOR&, float, float, float&);
		bool ThrowDice(int);
		bool SetDecDeityExp(int);
		bool GetIscrit(void);
		bool IsMultiFilterExist(int);
		bool SetPasadddodge(float);
		bool SetAddattack(bool);
		bool SetGuilin(bool);
		int GetRes1(void);
		int GetVar5(void);
		bool SetDecfatalhurt(bool);
		int GetRes4(void);
		bool SetFiring(bool);
		bool SetExtracthp(bool);
		bool SetIncdrugeffect(bool);
		int GetMinatk(void);
		bool SetCleardebuff(bool);
		bool SetDecanti(bool);
		bool SetLightuniform(bool);
		bool SetBloodshield(bool);
		bool SetIncfatalhurt(bool);
		int GetType(void);
		bool SetInccrithurt(bool);
		void AddCommonDebuff(int, int, int, int, bool);
		bool SetPullback(bool);
		bool SetSubhide(bool);
		bool GetInrange(XID const&, float, float&, bool);
		bool SetIncskilldodge(bool);
		bool SetDivinityburst(bool);
		bool SetDeadlybless(bool);
		bool SetClearsilent(bool);
		bool SetColdinjure(bool);
		bool SetEvilaura(bool);
		bool SetSubmp(bool);
		bool SetWrap(bool);
		bool SetPasdecfatalratio(float);
		bool SetAddrage1(bool);
		bool GetInteam(void);
		bool SetMpgen(bool);
		bool SetViphupo(bool);
		bool SetBefired(bool);
		bool SetPasaddweak(float);
		bool SetFear(bool);
		bool SetInchurt(bool);
		bool SetValue(float);
		bool SetDispel(bool);
		bool SetAssault2(bool);
		bool SetHpleak5(bool);
		bool SetSubantisleep2(bool);
		bool SetInvincible(bool);
		int GetRes2(void);
		bool SetGhostform(bool);
		bool SetBloodpool(bool);
		bool SetMiracleburstmo(bool);
		bool SetSeniorlightform(bool);
		bool SetHpleak(bool);
		bool SetDecskilldamage(bool);
		bool SetDecdefence(bool);
		bool TargetIsSpouse(void);
		bool SetGthpgen(bool);
		int GetDef(void);
		int GetBufflevel(int);
		bool SetDecmp(bool);
		bool SetResurrect(bool);
		bool SetInchitrate(float);
		bool SetBreak(bool);
		bool SetScaleinchp(bool);
		bool SetScaleincmp(bool);
		bool SetScaledechp(bool);
		bool SetScaledecmp(bool);
		bool SetPasincefence(float);
		bool SetPasaddsilenttenaciy(float);
		bool SetPasaddsleeptenaciy(float);
		bool SetPasaddslowtenaciy(float);
		bool SetPasaddwraptenaciy(float);
		bool SetPasaddsilentproficiency(float);
		bool SetPasaddweakproficiency(float);
		bool SetPasadddizzyproficiency(float);
		bool SetPasaddsleepproficiency(float);
		bool SetPasaddslowproficiency(float);
		bool SetPasaddwrapproficiency(float);
		bool SetOverawe(bool);
		bool SetPowerup(bool);
		bool SetShouyi(bool);
		bool SetPasincantidizzy(float);
		bool SetPasincantiweak(float);
		bool SetPasincantiwrap(float);
		bool SetPasincantisilent(float);
		bool SetPasincantisleep(float);
		bool SetPasincanti(float);
		bool SetIncheal(bool);
		bool SetDirectattack(bool);
		bool SetPasdechurt4(float);
		bool SetPasdechurt5(float);
		bool SetPasdechurt6(float);
		bool SetCreditboost(bool);
		bool SetHurtscatter(bool);
		bool SetIncattack2(bool);
		bool SetSlow2(bool);
		bool SetIncdefence2(bool);
		bool SetExtractmp(bool);
		bool SetImmunetransform(bool);
		bool SetPasaddtransform(float);
		bool SetPasincantitransform(float);
		bool SetAddantitransform(bool);
		bool SetSubantitransform(bool);
		bool SetDrawclonelife(bool);
		bool SetBackstab(bool);
		bool SetCritcurse(bool);
		bool SetSubSpot(bool);
		bool SetAddhide(bool);
		bool GetIsfury(void);
		bool SetPasincdpgen(float);
		bool SetCrippled(bool);
		bool SetPermitcyclearea(bool);
		bool SetLimitcyclearea(bool);
		bool SetGuishen1debuff(bool);
		bool SetTuoling1debuff(bool);
		bool SetTuoling2debuff(bool);
		bool SetPzmaxspeed(bool);
		bool SetPzhalfspeed(bool);
		bool SetPzhill(bool);
		bool SetPznomove(bool);
		bool SetPzchaos(bool);
		bool SetPzvoid(bool);
		bool SetActivebeattacked(bool);
		BuffLevelWrapper* GetBufflevel(void);
		bool SetDarklaser(bool);
		bool SetDirstate(bool);
		bool SetShowyb(bool);
		bool SetSummoncontroltrap(bool);
		bool SetSubantiweak2(bool);
		bool SetSubantislow2(bool);
		bool SetSubmp2(bool);
		bool SetRenma1(bool);
		bool SetRenma2(bool);
		bool SetRenma3(bool);
		bool SetRenma4(bool);


	//public:
	//	typedef unsigned int	SID;
	//protected:
	//	object_interface object;
	//	char    context;   // 0:施法者 1:被施法者 2:被动技能生效 3:被动技能失效 4:被动技能学习
	//	Skill * skill;
	//	MasterWrapper _master;
	//	const XID * target;
	//	int         tsize;
	//
	//	TargetInfoWrapper _targetinfo;
	//
	//	// 临时参数
	//	float	probability;	// 状态攻击参数
	//	float	ratio;		// 状态攻击参数
	//	float	amount;		// 状态攻击参数
	//	float	value;		// 状态攻击参数
	//	int 	time;		// 持续时间
	//	bool 	tiny_time;		 
	//	int     buffid;		// 是否显示状态图标
	//
	//	int     health;         // 治疗生命值
	//	bool    success;
	//	bool    immune;
	//	bool    invader;
	//
	//	bool	succaddfilter;
	//public:
	//	PlayerWrapper():succaddfilter(false) {}
	//	virtual ~PlayerWrapper() {}
	//	PlayerWrapper(object_interface _o, char _type, Skill *_s=NULL, const XID* _target=NULL, int _size=0);
	//
	//	object_interface &GetObject() { return object; }
	//	void RemoveFilter(int filterid); 
	//	void SetClear(int) { probability = 0; ratio = 0; amount = 0; value = 0; time = 0; tiny_time = false; buffid = 0; }
	//	int GetMp()         { return object.GetBasicProp().mp; }
	//	int GetHp()         { return object.GetBasicProp().hp; }
	//	int GetOccupation() { return object.GetClass(); }
	//	unsigned char GetPlayerRace() { return object.GetRace(); }
	//	int GetMaxhp()      { return object.GetExtendProp().max_hp; }
	//	int GetMaxmp()      { return object.GetExtendProp().max_mp; }
	//	int GetLevel()      { return object.GetBasicProp().level; }
	//	bool GetInteam()    { return object.IsInTeam(); }
	//	bool CanAttack()    { return object.CanAttack(XID(-1,-1)); }
	//	float GetRange()    { return object.GetExtendProp().attack_range; }
	//	char GetSneakstate() { return object.IsSneakMode(); }
	//	const XID& GetXid() { return object.GetSelfID(); }
	//	const XID *GetTarget() { return target; }
	//	int GetAttackHigh(float ratio, float plus) { return (int)(ratio * object.GetExtendProp().damage_high + plus); }
	//	int GetAttackLow(float ratio, float plus)  { return (int)(ratio * object.GetExtendProp().damage_low + plus); }
	//	int GetSkillTransform() { return object.GetSkillTransform(); }
	//	int GetSummonSoul() { return object.GetSoulState(); }
	//	int QueryObject(const XID & who, A3DVECTOR & pos, float & body_size) {return object.QueryObject(who, pos, body_size);}
	//	int QueryObject(const XID & who, bool & is_pet_summon) {return object.QueryObject(who, is_pet_summon);}
	//	bool IsSneakVisible(const XID & targetid){return object.IsSneakVisible(targetid);}
	//	float GetBodySize() {return object.GetBodySize();}
	//	int GetCheckbuff(int filter_id);
	//	bool checkfaction(size_t index);
	//	bool checkenemyfaction(size_t index);
	//	
	//	//target info interface
	//	TargetInfoWrapper * GetTargetinfo(){ return &_targetinfo;}
	//	// 图腾
	//	MasterWrapper* GetMaster();
	//	    
	//	int GetSkilllevel(int id);
	//	
	//	int GetDefence()	{ return object.GetExtendProp().defense; }
	//	int GetDamagehigh()	{ return object.GetExtendProp().damage_high; }
	//	int GetDamagelow()	{ return object.GetExtendProp().damage_low; }
	//	float GetWalkspeed()	{ return object.GetExtendProp().walk_speed; }
	//	float GetRunspeed()	{ return object.GetExtendProp().run_speed; }
	//	int GetAccuracy()	{ return object.GetExtendProp().attack; }
	//	int GetArmor()		{ return object.GetExtendProp().armor; }
	//	int GetCrit()		{ return object.GetExtendProp().crit_rate; }
	//	float GetSmite()	{ return object.GetExtendProp().crit_damage; }
	//	int GetAnticrit()	{ return object.GetExtendProp().anti_crit; }
	//	float GetAntismite()	{ return object.GetExtendProp().anti_crit_damage; }
	//
	//	bool GetDizzystate()	{ return object.HasVisibleState(VSTATE_DIZZY); }
	//	bool GetWrapstate()	{ return object.IsRootSeal(); }
	//	bool GetSilentstate()	{ return object.IsSilentSeal(); }
	//	bool GetSleepstate()	{ return (object.IsSilentSeal() && object.IsMeleeSeal() && object.IsRootSeal()); }
	//	bool GetSlowstate()	{ return object.HasVisibleState(VSTATE_SLOW); }
	//	bool GetWeakstate()	{ return object.HasVisibleState(VSTATE_WEAK); }
	//
	//	int GetDizzyresist()	{ return object.GetExtendProp().resistance[0]; }
	//	int GetWeakresist()	{ return object.GetExtendProp().resistance[1]; }
	//	int GetSlowresist()	{ return object.GetExtendProp().resistance[2]; }
	//	int GetSilentresist()	{ return object.GetExtendProp().resistance[3]; }
	//	int GetSleepresist()	{ return object.GetExtendProp().resistance[4]; }
	//	int GetWrapresist()	{ return object.GetExtendProp().resistance[5]; }
	//
	//	int GetBaseLevel(int id);
	//	int GetLevelSum(int cls);
	//	bool GetInrange( float range, float& correction, bool corpseonly = false );
	//	bool GetInrange( A3DVECTOR& tpos, float tbody,  float range, float& correction );
	//
	//	VIRTUALFUN int GetRand() {  return rand()%100; }
	//
	//	void SetSkill(Skill* s) { skill = s; }
	//	void SetProbability(float d) 
	//	{ 
	//		if(d>0) 
	//			probability = d; 
	//		else if (fabs(d + 100) < 0.001) //约定策划填-100表示一定不成功
	//			probability = -1;
	//		else if (fabs(d + 1) < 0.001)
	//			probability = succaddfilter ? 100 : -1;
	//		else if (fabs(d + 2) < 0.001)
	//			probability = succaddfilter ? -1 : 100;
	//	}
	//	float GetProbability() { return probability; }
	//
	//	bool ThrowDice()
	//	{
	//		bool ret;
	//		if(probability>99)
	//			ret = true;
	//		else if(probability<0.001)
	//			ret = false;
	//		else
	//			ret = (rand()%100)<probability;
	//		succaddfilter = ret;
	//		return succaddfilter;
	//	}
	//	bool ThrowDice(int type);
	//
	//	float GetRatio()    { return ratio; }
	//	float GetValue()    { return value; }
	//	float GetAmount()   { return amount; }
	//	int GetBuffid() { return buffid; }
	//	bool GetSuccess()   { return success; }
	//	void SetSuccess(bool val)   { success = val; }
	//	bool GetImmune()    { return immune; }
	//	int GetTime()       { return time; }
	//	bool GetInvader() { return invader; }
	//	int GetBufflevel(int id);
	//
	//	bool SetTime(float t)     
	//	{ 
	//		tiny_time = false;
	//		if (t < 0)		//技能编辑器里填负数表示无限
	//			time = 0;
	//		else
	//		{
	//			time = (int)(0.001*t+0.5); 
	//			if(time==0)
	//			{
	//				time = 1;
	//				tiny_time = true;
	//			}
	//		}
	//		return true; 
	//	}
	//	bool SetBuffid(int d)     { buffid = d%10; return true; }
	//	bool SetRatio(float d)    { ratio = d; return true; }
	//	bool SetValue(float d)    { value = d; return true; }
	//	bool SetAmount(float d)   { amount = d; return true; }
	//	bool SetInvader(bool b){ invader = b; return true; }
	//	bool SetContext(char c){ context = c; return true; }
	//
	//	void SetTalentData(int* list);
	//
	//public:
	//	bool SetHp(float hp) 
	//	{
	//		int dec = (int)(object.GetBasicProp().hp - hp);
	//		if(dec>0)
	//			object.DecHP(dec);
	//		return true;
	//	}
	//
	//	bool SetMp(float mp) 
	//	{ 
	//		int dec = (int)(object.GetBasicProp().mp - mp);
	//		if(dec>0)
	//			return object.DrainMana((int)dec);
	//		return false;
	//	}
	//
	//	bool UseMp(int dec)
	//	{
	//		int mp = object.GetBasicProp().mp;
	//		if(mp>=dec)
	//			return object.DrainMana(dec);
	//		else
	//			return object.DrainMana(mp);
	//	}
	//
	//	bool UseItem(int itemid, size_t count)
	//	{
	//		return object.TakeOutItem(itemid, count);
	//	}
	//
	//	int GetSp() { return object.GetSkillPoint(); }
	//	void SetDecsp(int dec)   { object.ModifySkillPoint(-dec); }
	//	void SetCombat(bool)     { object.EnterCombatState(); }
	//
	//	bool SetPray(int) { return true; }
	//	bool SetInform(int) { return true; }
	//	bool SetPerform(int);
	//	bool SetHeal(bool);
	//	bool SetResurrect(bool);
	//
	//	bool SetInchp(bool);
	//	bool SetIncmp(bool);
	//	bool SetAddhp(bool);
	//	bool SetAddmp(bool);
	//	bool SetHpgen(bool);
	//	bool SetMpgen(bool);
	//	bool SetInchpgen(bool);
	//	bool SetIncmpgen(bool);
	//	bool SetDizzy(bool);
	//	bool SetSleep(bool);
	//	bool SetWeak(bool);
	//	bool SetSilent(bool);
	//	bool SetWrap(bool);
	//	bool SetSlow(bool);
	//	bool SetAddantidizzy(bool);
	//	bool SetAddantisleep(bool);
	//	bool SetAddantiweak(bool);
	//	bool SetAddantisilent(bool);
	//	bool SetAddantiwrap(bool);
	//	bool SetAddantislow(bool);
	//	bool SetSubantidizzy(bool);
	//	bool SetSubantisleep(bool);
	//	bool SetSubantiweak(bool);
	//	bool SetSubantisilent(bool);
	//	bool SetSubantiwrap(bool);
	//	bool SetSubantislow(bool);
	//	bool SetCleardizzy(bool);
	//	bool SetClearsleep(bool);
	//	bool SetClearweak(bool);
	//	bool SetClearsilent(bool);
	//	bool SetClearwrap(bool);
	//	bool SetClearslow(bool);
	//
	//	bool SetAddattack(bool);
	//	bool SetAdddefence(bool);
	//	bool SetIncattack(bool);
	//	bool SetIncdefence(bool);
	//	bool SetDecattack(bool);
	//	bool SetAdddodge(bool);
	//	bool SetCrazy(bool);
	//	bool SetAssault(bool m);
	//	bool SetMagicshield(bool);
	//	bool SetDechurt(bool);
	//	bool SetInvincible(bool);
	//	bool SetBreak(bool);
	//	bool SetGohome(bool m);
	//	int CheckTarget(float minrange, float maxrange, bool corpseonly);
	//	int CheckSummonTarget(float minrange, float maxrange);
	//	bool SetAddspeed(bool);
	//	bool SetBarehanded(bool);
	//	bool SetDodge(bool);
	//	bool SetInccritrate(bool);
	//
	//	// Obsolete
	//	bool SetScaleinchp(bool);
	//	bool SetScaleincmp(bool);
	//	bool SetScaledecmp(bool);
	//	bool SetScaledechp(bool);
	//	bool SetHpleak(bool);
	//
	//	template<typename ATTACK_MSG>
	//	bool SetEnmity(const XID &attacker, const ATTACK_MSG &msg, int n)
	//	{
	//		const XID *target = NULL;
	//		if (msg.attack_flag & ATTACK_FLAG_SUMMON_INCUR_AGGRO_SELF)
	//			target  = &attacker;
	//		else
	//			target = &msg.ainfo.attacker;
	//
	//		if(skill->GetType()==TYPE_BLESS)
	//			object.AddAggroToEnemy(*target,msg.attacker_faction,n);
	//		else
	//			object.AddAggro(*target,msg.attacker_faction,n);
	//		return true;
	//	}
	//
	//	bool SetCleardebuff(bool);
	//	// 被动技能接口
	//	bool SetInchitrate(float ratio);
	//
	//	bool SetMoney(int n)        { return true; }
	//	bool SetUseitem(int n)      { return (object.TakeOutItem(n)!=-1); }
	//	bool SetUsemoney(int n)     { object.DecMoney(n); return true; }
	//	bool SetCheckitem(int n)    { return object.CheckItem(n,1);}
	//	bool SetCheckmoney(int n)   { return object.GetMoney()>=(unsigned int)n; }
	//
	//	// 武器持续激活技能
	//	bool SetFrenetic(bool b);
	//	int GetWeapon()     { return object.GetCurWeapon().weapon_class; }
	//	int GetMoney()      { return object.GetMoney(); }
	//	int GetForm()       { return object.GetShape(); }
	//	int GetRank()       { return object.GetBasicProp().sec_level; }
	//	INT64_T GetFaction()    { return object.GetFaction(); }
	//	INT64_T GetEnemyfaction()    { return object.GetEnemyFaction(); }
	//	bool UseArrow(int count) { return object.UseProjectile(count); }
	//	bool IsUsingWeapon(int weapon) { return weapon == object.GetCurWeapon().weapon_class; }
	//
	//	bool SetPasaddattack(float);
	//	bool SetPasincattack(float);
	//	bool SetPasadddefence(float);
	//	bool SetPasincdefence(float);
	//	bool SetPasaddhp(float);
	//	bool SetPasinchp(float);
	//	bool SetPasincmp(float);
	//	bool SetPasaddmp(float);
	//	bool SetPasaddaccuracy(float);
	//	bool SetPasadddodge(float);
	//	bool SetPasaddsilent(float);
	//	bool SetPasaddweak(float);
	//	bool SetPasadddizzy(float);
	//	bool SetPasaddsleep(float);
	//	bool SetPasaddslow(float);
	//	bool SetPasaddwrap(float);
	//	bool SetPasaddsilentf(float);
	//	bool SetPasaddweakf(float);
	//	bool SetPasadddizzyf(float);
	//	bool SetPasaddsleepf(float);
	//	bool SetPasaddslowf(float);
	//	bool SetPasaddwrapf(float);
	//	bool SetPasaddanticrit(float);
	//	bool SetPasaddantismite(float);
	//	bool SetPasaddsummonbl(float);
	//	bool SetPasaddsummonal(float);
	//	bool SetPasaddsummoncl(float);
	//	bool SetPasaddskillattack(float);
	//	bool SetPasaddskillarmor(float);
	//	bool SetPasaddfactdmgrat(float);
	//	bool SetPasaddantifactdmgrat(float);
	//	bool SetPasaddfactdmg(float);
	//	bool SetPasaddantifactdmg(float);
	//	bool SetPasaddqinglong(float);
	//	bool SetPasaddbaihu(float);
	//	bool SetPasaddzhuque(float);
	//	bool SetPasaddxuanwu(float);
	//	bool SetPasaddqinglongdef(float);
	//	bool SetPasaddbaihudef(float);
	//	bool SetPasaddzhuquedef(float);
	//	bool SetPasaddxuanwudef(float);
	//	bool SetPasaddholyhitrat(float);
	//	bool SetPasaddholyhitdmg(float);
	//	bool SetPasaddsneakpoint(float);
	//	bool SetPasaddantisneakpoint(float);
	//	void SendResult(const XID& attacker, int atstate);
	//
	//	bool SetPasinchpgen(float);
	//	bool SetRepel(bool);
	//	bool SetOverawe(bool);
	//	bool SetSubdefence(bool);
	//	bool SetDecdefence(bool);
	//	bool SetDecdamage(bool);
	//	bool SetBloodshield(bool);
	//	bool SetImmunedizzy(bool);
	//	bool SetImmunesleep(bool);
	//	bool SetImmunesilent(bool);
	//	bool SetImmuneweak(bool);
	//	bool SetImmunewrap(bool);
	//	bool SetImmuneslow(bool);
	//	bool SetDrainmagic(bool);
	//	bool SetUsemagic(bool);
	//	bool SetPasaddsmite(float);
	//	bool SetCursed(bool);
	//	bool SetPasaddantisleep(float);
	//	bool SetPasaddantiwrap(float);
	//	bool SetPasaddantiweak(float);
	//	bool SetPasaddantidizzy(float);
	//	bool SetDiet(bool);
	//	bool SetSalvation(bool);
	//	bool SetBlessed(bool);
	//	bool SetPowerup(bool);
	//	bool SetInchurt(bool);
	//	bool SetRandcurse(bool);
	//	bool SetClearcooldown(bool);
	//	bool SetSecondattack(bool);
	//	bool SetShouyi(bool);
	//	bool SetNingjin(bool);
	//	bool SetDisappear(bool b);
	//
	//	bool SetDamage2hp(bool b);
	//	bool SetAttack2hp(bool b);
	//	bool SetSwift(bool b);
	//	bool SetRetort(bool b);
	//	bool SetClearbuff(bool b);
	//	bool SetSealed(bool b);
	//	bool SetDechp(bool b);
	//	bool SetDecmp(bool b);
	//	bool SetSubhp(bool b);
	//	bool SetSubmp(bool b);
	//	bool SetIncanti(bool b);
	//	bool SetDecanti(bool b);
	//	bool SetIncdodge(bool b);
	//	bool SetDecdodge(bool b);
	//	bool SetSmiteattack(bool b);
	//	bool SetAddhpgen(bool b);
	//	bool SetPasaddcrit(float inc);
	//	bool SetPasmaxvigour(float max);
	//	bool SetPasmaxvitality(float max);
	//
	//	bool SetPasaddantidmgrat(float);
	//	bool SetPasaddantiantidmgrat(float);
	//
	//
	//	//92号击中效果之后的新效果都调用这个方法
	//	bool CheckAddCondition(base_filter* pFilter);
	//	bool SetFilter(int filter_guid);
	//	void PreSetUpFilter(base_filter* pFilter);



	};


}

#endif

