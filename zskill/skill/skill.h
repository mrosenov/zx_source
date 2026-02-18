#ifndef __SKILL_SKILL_H
#define __SKILL_SKILL_H

#include "common/types.h"
#include <sys/stat.h>
//#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>

#include <string>
#include <vector>
#include <map>
#include <ASSERT.h>
//#include "types.h"
#include "attack.h"
#include "playerwrapper.h"
#include "skillwrapper.h"

#include "skillfunction.h"

#ifndef L
#define L
#endif

//
//#pragma pack(1)
//#define DEG2RAD(deg) ((deg) * 3.1415926535f / 180.0f)
//
namespace SKILL
{
	struct Data;


	//struct Data
	//{
	//	int id = 0;  //0
	//	char forceattack = 0;  //0x4
	//	bool skippable = 0;  //0x5
	//	int stateindex = -1;  //0x8
	//	int nextindex = -1;  //0xC
	//	int warmuptime = -1;  //0x10
	//	short level = 0; //0x14

	//	int cooltime = 0; //0x18
	//	int item_id = -1; //0x1C
	//	short item_index = -1;  //0x20
	//	bool consumable = 0;  //0x22
	//	char spirit_index = -1; //0x23
	//	A3DVECTOR pos;  //0x24
	//	char skillstate = 0; //0x30

	//	std::vector<short> talismanRefineSkillList; //0x34

	//	Data(int i)
	//	{
	//		id = i;
	//	}
	//};

};
//
//struct enchant_msg;
//struct attack_msg;
//struct XID;

#ifndef INT
#define INT int
#endif // !INT


namespace GNET
{
class PlayerWrapper;

class Rank
{
	Rank();
	static Rank instance;
	std::map<int, int> basemap;
public:
	static int GetBase(int rank);
	static bool SameBranch(int rank, int root);
};

class SkillStub
{
public:

	typedef unsigned int ID;
	typedef std::map<ID, const SkillStub*> Map;

	class State
	{
	public:
		State() { }
		virtual ~State() { } //+1

		virtual int  GetTime(Skill * skill) const = 0; //+2
		virtual void Calculate(Skill * skill) const = 0;  //+3
		virtual bool Interrupt(Skill*) const { return false; }//+4
		virtual bool Cancel(Skill*) const { return false; } //+5
		virtual bool Bypass(Skill*) const { return false; } //+6
		virtual bool Skip(Skill*) const { return false; }	//+7
		virtual bool Quit(Skill*) const { return false; }	//+8
		virtual bool Loop(Skill*) const { return false; }	//+9


	};

public:
	//static std::map<unsigned int, const GNET::SkillStub*> Map;

	int id;  //0x4
	int occupation;
	int maxlevel;
	int maxlearn;
	int eventflag;
	int spcost;
	int reborncount;
	char type;
	char timetype;
	char targettype;//0x22
	char rangetype;//0x23
	char doenchant;
	char dobless;
	char autoattack;
	int allowform; //0x27
	int credittype;  //0x2B
	int clearmask;
	int skill_limit;
	int skillelemsnum;//0x37
	short skillelemsfactor;
	int serialskill;
	char charge;
	int chargedist;
	int succeedskillcolor;//0x48
	int succeedskillcolornum;
	int comboskill;
	char need_item;//0x54
	int item1_id;
	int item1_num;  //0x59
	int item2_id;
	int item2_num;  //0x61
	int item_relation;
	int weapon_limit;//0x69
	const char* action[31];//0x6D
	std::string nativename; //0xE9
	std::string icon; // 0xED
	std::wstring name; //0xF1
	std::vector<SkillStub::State*> statestub; //0xF5
	short talent[8];//0x101
	short talent_size;  //0x111
	char talent_type;
	bool use_proficiency;
	int inc_proficiency;//0x115
	int preskillnum;
	char cycle;
	int cyclemode;
	std::string cyclegfx; //0x122
	int skill_class; //0x126
	char guide;
	int summon_id;
	char trigger;
	char castInPreSkill;
	int preCastSkillId;
	char darkLightType; //0x135
	int followskill;
	int targetcnt;
	int excludemonstercnt;//0x13E

public:
	virtual ~SkillStub();//+1
	virtual void Clear(void); //+2
	virtual int GetExecutetime(Skill*) const; //+3
	virtual int GetCooldowntime(Skill*) const;//+4
	virtual int GetEnmity(Skill*) const;//+5
	virtual int GetRequiredLevel(int) const;//+6
	virtual int GetRequiredProficiency(int) const;//+7
	virtual float GetRadius(Skill*) const;//+8
	virtual float GetAttackdistance(Skill*) const;  //+9
	virtual float GetAngle(Skill*) const; //+10
	virtual float GetPraydistance(Skill*) const; //+11
	virtual float GetCastdistance(Skill*) const;  //+12
	virtual float GetEffectdistance(Skill*) const;//+13
	virtual float GetMpcost(Skill*) const;  //+14
	virtual int GetCoverage(Skill*) const;//+15
	virtual bool StateAttack(Skill*) const;//+16
	virtual bool TakeEffect(Skill*) const;//+17
	virtual bool BlessMe(Skill*) const; //+18
	virtual int GetCreditCost(int) const;//+19
	virtual float GetDropRate(Skill*) const;//+20
	virtual int GetSkillElem(int) const; //+21
	virtual int GetComboSkillCapacity(int) const;//+22
	virtual bool CheckDarkLightValue(Skill*) const; //+23
	virtual int GetPreSkillID(int) const;  //+24
	virtual int GetPreSkillSP(int) const;  //+25
	virtual int GetDivinityExp(int) const;//+26
	virtual int GetDivinityLevel(int) const;//+27
	virtual int GetDpcost(Skill*) const;//+28
	virtual int GetHpcost(Skill*) const;  //+29
	virtual int GetInkcost(Skill*) const;  //+30
	virtual int GetExcludemonsterTid(int) const;//+31

	SkillStub(unsigned int);
	bool CheckSkillElem(int, int&) const; 
	int Condition(Skill*) const;
	bool DoBless(void) const;
	int GetEventFlag(void) const;
	static std::map<unsigned int, const GNET::SkillStub*> GetMap(void);
	int GetOccupation(void) const;
	int GetPreskillNum(void) const;
	State* GetState(int) const;
	size_t GetStateSize(void) const;
	static const SkillStub* GetStub(unsigned int);
	bool GetTargettype(void) const;
	char GetType(void) const;
	bool ItemCondition(Skill*) const;
	bool Learn(Skill*) const;
	bool LearnCondition(Skill*) const;

};

class Skill
{
public:
	typedef unsigned int ID;
	typedef std::map<ID, const Skill*> Map;

	SKILL::Data* pdata;

protected:
	int level;  //0x4
	float ratio;
	float plus;
	float plus2;
	float crit;
	float crithurt;
	int skillaccu;
	float forceattack;  //0x20
	int is_crit;
	unsigned short skillelems[25];  //0x25

	int saint;  //0x39
	int t0;
	int t1;
	int t2;
	int t3;
	int t4;
	int t5;
	int t6;
	int t7;
	int damage;   //0x5D
	int real_damage;
	PlayerWrapper *player;  //0x65

	attacker_info_t attacker;  //0x6D

	char attackermode;
	A3DVECTOR attackerpos;  //0xA2
	int attacker_faction; //0xAE

	int target_faction;//0xB2
	float attack_range;//0xB6
	XID chargeTarget;  //0xBA

	A3DVECTOR chargeDestPos;//0xC2

	const SkillStub* stub;  //0xCE
	int c0;	//0xD2
	int c1;
	int c2;
	int c3;
	int c4;
	int j0;
	int j1;
	int j2;
	int j3;
	int j4;
	int j5;
	int j6;
	int j7;
	int j8;
	int j9; //0x10A
	float tianhua1;  //0x10E

//	static std::map<unsigned int, const SkillStub*> Map;

public:
	virtual ~Skill();  //+1
	virtual void Clear(void);  //+2
	virtual Skill* Clone(void) const;  //+3

	Skill(Skill const&);
	Skill(unsigned int);
	bool BlessMe(void);
	bool CanAttack(void);
	bool Cancel(void);
	int Condition(void);
	static Skill* Create(unsigned int);
	void Destroy(void);
	bool DoEnchant(void);
	int FirstRun(int&, int);
	float GetAngle(void);
	float GetAttackRange(void);
	float GetAttackdistance(void);
	const attacker_info_t& GetAttacker(void);
	int GetAttackerFaction(void);
	char GetAttackerMode(void);
	XID& GetAttackerid(void);
	int GetAttackerlevel(void);
	const A3DVECTOR& GetAttackerpos(void);
	int GetC0(void);
	int GetC1(void);
	int GetC2(void);
	int GetC3(void);
	int GetC4(void);
	const A3DVECTOR& GetChargeDestTarget(void);
	float GetChargeDist(void);
	const XID& GetChargeTarget(void);
	char GetChargeType(void);
	unsigned int  GetCharging(void);
	int GetCooltime(void);
	int GetCoverage(void);
	float GetCrit(void);
	float GetCrithurt(void);
	int GetDamage(void);
	char GetDarkLightType(void);
	const SKILL::Data* GetData(void);
	float GetDpcost(void);
	float GetDropRate(void);
	float GetEffectdistance(void);
	int GetEnmity(void);
	int GetEventFlag(void);
	char GetForceattack(void);
	float GetHpcost(void);
	unsigned int  GetId(void);
	int GetIncProficiency(void);
	float GetInkcost(void);
	int GetIscrit(void);
	int GetItem1Id(void);
	int GetItem1Num(void);
	int GetItem2Id(void);
	int GetItem2Num(void);
	char GetItemRelation(void);
	int GetJ0(void);
	int GetJ1(void);
	int GetJ2(void);
	int GetJ3(void);
	int GetLevel(void);
	unsigned int  GetLimit(void);
	static std::map<unsigned int, const GNET::Skill*>& GetMap(void);
	int GetMaxLearn(void);
	int GetMaxlevel(void);
	float GetMpcost(void);
	int GetNextindex(void);
	int GetOccupation(void);
	PlayerWrapper* GetPlayer(void);
	float GetPlus(void);
	float GetPlus2(void);
	const A3DVECTOR& GetPosition(void);
	float GetPraydistance(void);
	float GetRadius(void);
	char GetRange(void);
	float GetRatio(void);
	int GetRealDamage(void);
	int GetSaint(void);
	int GetSerialSkill(void);
	const unsigned short* GetSkillElems(void);
	char GetSkillSpiritIndex(void);
	int GetSkillaccu(void);
	const SkillStub::State* GetState(int);
	int GetStateindex(void);
	static const Skill* GetStub(unsigned int);
	const SkillStub* GetStub(void);
	int GetT0(void);
	int GetT1(void);
	int GetT2(void);
	int GetT3(void);
	int GetT4(void);
	int GetT5(void);
	int GetT6(void);
	int GetT7(void);
	int GetTargetCnt(void);
	int GetTargetFaction(void);
	bool GetTargettype(void);
	float GetTianhua1(void);
	char GetType(void);
	PlayerWrapper* GetVictim(void);
	void IncSkillElemsNum(int, int);
	int InstantRun(void);
	bool Interrupt(void);
	bool InvalidState(void);
	bool IsCastSelf(void);
	bool IsCharge(void);
	bool IsComboSkill(void);
	bool IsCycle(void);
	bool IsFamily(void);
	bool IsInstant(void);
	bool IsNeedItem(void);
	bool IsRune(void);
	bool IsSpouseSkill(void);
	bool IsTalisman(void);
	bool IsTrigger(void);
	bool IsUseProficiency(void);
	bool IsWarmup(void);
	bool Learn(void);
	int NextState(int);
	void NpcRun(void);
	bool Run(SkillStub::State const*);
	int Run(int&);
	void SetAttackRange(float);
	void SetAttackerpos(A3DVECTOR const&);
	void SetCastSelf(PlayerWrapper*);
	void SetChargeDestPos(A3DVECTOR const&);
	void SetChargeTarget(XID const&);
	void SetCharging(int);
	void SetCrit(float);
	void SetCrithurt(float);
	void SetData(SKILL::Data*);
	void SetIscrit(int);
	void SetLevel(int);
	void SetMessage(attack_msg const*);
	void SetMessage(enchant_msg const*);
	void SetNextindex(int);
	void SetPlayer(PlayerWrapper*);
	void SetPlus(float);
	void SetPlus2(float);
	void SetRatio(float);
	void SetRealDamage(int);
	void SetSaint(int);
	void SetSkillElems(unsigned short*);
	void SetSkillState(int);
	void SetSkillaccu(int);
	void SetStateindex(int);
	void SetSucceedSkillColor(int, int);
	void SetTalent(int, int);
	void SetTianhua1(float);
	bool StateAttack(void);
	bool TakeEffect(PlayerWrapper*);
	bool UndoEffect(PlayerWrapper*);

};

};

#pragma pack()

#endif

