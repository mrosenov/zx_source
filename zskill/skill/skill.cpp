#include "skill.h"
#include "playerwrapper.h"
#include "sfilterdef.h"

#ifdef INTEPRETED_EXPR 
#include "skillexpr.h"
#endif

namespace GNET
{
	//==================================Rank=======================start

	Rank Rank::instance;
	Rank::Rank()
	{
		basemap[0]  =  0;
		basemap[1]  =  0;
		basemap[2]  =  1;
		basemap[3]  =  2;
		basemap[4]  =  0;
		basemap[5]  =  4;
		basemap[6]  =  5;
		basemap[7]  =  0;
		basemap[8]  =  7;
		basemap[9]  =  8;
		basemap[10] =  0;
		basemap[11] =  10;
		basemap[12] =  11;
		basemap[13] =  3;
		basemap[14] =  13;

		basemap[16] =  6;
		basemap[17] =  16;

		basemap[19] =  9;
		basemap[20] =  19;

		basemap[22] =  12;
		basemap[23] =  22;

		basemap[25] =  0;
		basemap[26] =  25;
		basemap[27] =  26;
		basemap[28] =  27;
		basemap[29] =  28;

		basemap[33] =  -1;
		basemap[34] =  33;
		basemap[35] =  34;
		basemap[36] =  35;
		basemap[37] =  36;

		basemap[39] =  -1;
		basemap[40] =  39;
		basemap[41] =  40;
		basemap[42] = 41;
		basemap[43] = 42;

		basemap[45] = -1;
		basemap[46] = 45;
		basemap[47] = 46;
		basemap[48] = 47;
		basemap[49] = 48;

		basemap[51] = -1;
		basemap[52] = 51;
		basemap[53] = 52;
		basemap[54] = 53;
		basemap[55] = 54;
		basemap[56] = -1;
		basemap[57] = 56;
		basemap[58] = 57;
		basemap[59] = 58;
		basemap[60] = 59;

		basemap[64] = 0;
		basemap[65] = 64;
		basemap[66] = 65;
		basemap[67] = 6666;
		basemap[68] = 67;

		basemap[96] = -1;
		basemap[97] = 96;
		basemap[98] = 97;
		basemap[99] = 98;
		basemap[100] = 99;

		basemap[102] = -1;
		basemap[103] = 102;
		basemap[104] = 103;
		basemap[105] = 104;
		basemap[106] = 105;

		basemap[108] = -1;
		basemap[109] = 108;
		basemap[110] = 109;
		basemap[111] = 110;
		basemap[112] = 111;
	}
	int Rank::GetBase(int rank)
	{
		return instance.basemap[rank];
	}
	bool Rank::SameBranch(int rank, int root)
	{
		if(root<0)
			return false;
		while (rank >= root)
		{
			if (rank == root)
				return 1;
			rank = instance.basemap[rank];
		}
		return 0;
	}
	//==================================Rank=======================end

	//==================================SkillStub==================start
	SkillStub::SkillStub(uint i)
	{
		this->id = i;
		this->credittype = 0;
		this->clearmask = 0;
		this->talent_size = 0;
		this->talent_type = 0;
		this->use_proficiency = 0;
		this->inc_proficiency = 0;
		this->skill_class = 0;
		this->darkLightType = 0;
		this->excludemonstercnt = 0;

		if(!GetStub(id)) 
			GetMap().insert(std::make_pair(id, this)); 

	}

	SkillStub::~SkillStub()
	{

	}

	bool SkillStub::BlessMe(Skill*) const
	{
		return 0;
	}

	bool SkillStub::CheckDarkLightValue(Skill*) const
	{
		return 1;
	}

	bool SkillStub::CheckSkillElem(int elemID, int& index) const
	{
		if (!elemID)
			return 1;
		for (int i = 1; i <= this->skillelemsnum; ++i)
		{
			if (GetSkillElem(i) == elemID)
			{
				index = i;
				return 1;
			}
		}
		index = -1;
		return 0;
	}

	void SkillStub::Clear(void)
	{
		for (int i = 0; i < statestub.size(); ++i)
		{
			if (statestub[i])
				delete statestub[i];
		}
		statestub.clear();
	}

	int SkillStub::Condition(Skill* skill) const
	{
		if (GetType() > 12)   //wulin : TYPE_RUNE¡¢TYPE_BLESS ..
			return 2;
		PlayerWrapper* Player = skill->GetPlayer();
		if (skill->GetPlayer()->IsRenMa())
		{
			//if (this+29(skill) + (this+14(skill) > Player->GetHp())  
			if (GetHpcost(skill) + GetMpcost(skill) > Player->GetHp())
				return 17;
		}
		else
		{
			//if (skill->GetPlayer()->GetMp() < this +14(skill))
			if (skill->GetPlayer()->GetMp() < GetMpcost(skill))
				return 3;
			//if (skill->GetPlayer()->GetHp() < this +29(skill))
			if (skill->GetPlayer()->GetHp() < GetHpcost(skill))
				return 17;
		}
		if (this->allowform)
		{
			if (((this->allowform >> skill->GetPlayer()->GetForm()) & 1) == 0)
				return 4;
		}
		if (this->rangetype != 6
			&& this->rangetype != 2
			&& this->rangetype != 5
			&& this->rangetype != 9
			&& this->rangetype != 12
			&& this->rangetype != 13
			&& this->rangetype != 14
			&& this->rangetype != 15
			&& this->rangetype != 11)
		{
			//if (!skill->GetPlayer()->CheckTarget(this + 11(skill), targettype))
			if (!skill->GetPlayer()->CheckTarget(GetPraydistance(skill), targettype))
				return 6;
		}
		if (this->rangetype == 8)
		{
			if (!skill->GetPlayer()->TargetIsSpouse())
				return 7;
		}
		if (this->occupation == 152)
		{
			if (!skill->GetPlayer()->IsMarried())
				return 8;
		}
		char cultivation = skill->GetPlayer()->GetCultivation();
		if (this->occupation == 148 && (cultivation & 1) == 0
			|| this->occupation == 149 && (cultivation & 2) == 0
			|| this->occupation == 150 && (cultivation & 4) == 0)
		{
			return 8;
		}
		if (this->occupation == 167 && (cultivation & 1) == 0
			|| this->occupation == 168 && (cultivation & 2) == 0
			|| this->occupation == 169 && (cultivation & 4) == 0)
		{
			return 15;
		}
		if (!ItemCondition(skill))
			return 9;

		if (skill->GetPlayer()->IsFilterExist(FILTER_FOXSTATE) && this->id != 2188)
			return 10;
		if (this->id == 2188)
		{
			if (skill->GetPlayer()->IsMount())
				return 11;
			if (skill->GetPlayer()->IsFlying())
				return 11;
		}
		if (skill->IsCharge())
		{
			if (!skill->GetPlayer()->CanCharge(skill->GetChargeTarget(), skill->GetChargeDestTarget(), skill->GetChargeType(), skill->GetChargeDist()))
				return 12;
		}
		if (skill->GetPlayer()->IsFilterExist(FILTER_FOGSTATE))
			return 13;
		//if (skill->GetPlayer()->GetDp() < this + 28(skill))
		if (skill->GetPlayer()->GetDp() < GetDpcost(skill))
			return 14;
		if (skill->GetPlayer()->IsFilterExist(FILTER_YUANLING) && this->id != 3367)
			return 15;
		//if (skill->GetPlayer()->IsXuanYuan() && !this + 23(skill))
		if (skill->GetPlayer()->IsXuanYuan() && !CheckDarkLightValue(skill))
		{
			return 16;
		}
		//if (skill->GetPlayer()->GetInk() < this + 30(skill))
		if (skill->GetPlayer()->GetInk() < GetInkcost(skill))
			return 18;
		if (skill->GetPlayer()->GetForm() == 6
			&& !this->allowform
			&& this->id != 4783
			&& this->id != 4784
			&& this->id != 4785
			&& this->id != 4786
			&& this->id != 4787)
		{
			return 19;
		}
		if (skill->GetPlayer()->GetForm() == 7
			&& this->id != 4871
			&& this->id != 5243
			&& this->id != 5244
			&& this->id != 5245
			&& this->id != 5246)
		{
			return 20;
		}
		if (!skill->GetPlayer()->IsFilterExist(FILTER_CYCLESKILL)
			|| this->id == 4877
			|| this->id == 5266
			|| this->id == 5270
			|| this->id == 5274)
		{
			if (skill->GetPlayer()->IsFilterExist(FILTER_RMHUMANFORM))
			{
				if (this->id != 4934)
					return 10;
				if (skill->GetPlayer()->IsFlying())
					return 10;
				if (skill->GetPlayer()->IsBindState())
					return 10;
			}
			return 0;
		}
		return 15;

	}

	bool SkillStub::DoBless(void) const
	{
		return this->dobless;
	}

	float SkillStub::GetAngle(Skill*) const
	{
		return 0.0;
	}

	float SkillStub::GetAttackdistance(Skill*) const
	{
		return 0.0;
	}

	float SkillStub::GetCastdistance(Skill*) const
	{
		return 0.0;
	}

	int SkillStub::GetComboSkillCapacity(int) const
	{
		return 0;
	}

	int SkillStub::GetCooldowntime(Skill*) const
	{
		return 0;
	}

	int SkillStub::GetCoverage(Skill*) const
	{
		return 20;
	}

	int SkillStub::GetCreditCost(int) const
	{
		return 0;
	}

	int SkillStub::GetDivinityExp(int) const
	{
		return 0;
	}

	int SkillStub::GetDivinityLevel(int) const
	{
		return 0;
	}

	int SkillStub::GetDpcost(Skill*) const
	{
		return 0;
	}

	float SkillStub::GetDropRate(Skill*) const
	{
		return 0.0;
	}

	float SkillStub::GetEffectdistance(Skill*) const
	{
		return 0.0;
	}

	int SkillStub::GetEnmity(Skill*) const
	{
		return 0;
	}

	int SkillStub::GetEventFlag(void) const
	{
		return this->eventflag;
	}

	int SkillStub::GetExcludemonsterTid(int) const
	{
		return 0;
	}

	int SkillStub::GetExecutetime(Skill*) const
	{
		return 0;
	}

	int SkillStub::GetHpcost(Skill*) const
	{
		return 0;
	}

	int SkillStub::GetInkcost(Skill*) const
	{
		return 0;
	}

	std::map<unsigned int, const GNET::SkillStub*> SkillStub::GetMap(void)
	{
		static Map map; 
		return map;

	}

	float SkillStub::GetMpcost(Skill*) const
	{
		return 0.0;
	}

	int SkillStub::GetOccupation(void) const
	{
		return this->occupation;
	}

	float SkillStub::GetPraydistance(Skill*) const
	{
		return 0.0;
	}

	int SkillStub::GetPreSkillID(int) const
	{
		return 0;
	}

	int SkillStub::GetPreSkillSP(int) const
	{
		return 0;
	}

	int SkillStub::GetPreskillNum(void) const
	{
		return this->preskillnum;
	}

	float SkillStub::GetRadius(Skill*) const
	{
		return 0.0;
	}

	int SkillStub::GetRequiredLevel(int) const
	{
		return 0;
	}

	int SkillStub::GetRequiredProficiency(int) const
	{
		return 0;
	}

	int SkillStub::GetSkillElem(int) const
	{
		return 0;
	}

	SkillStub::State* SkillStub::GetState(int index) const
	{
		return statestub[index];

	}

	size_t SkillStub::GetStateSize(void) const
	{
		return statestub.size();
	}

	const SkillStub* SkillStub::GetStub(uint i)
	{
		Map::iterator it = GetMap().find(i);
		return it == GetMap().end() ? NULL : (*it).second;
	}

	bool SkillStub::GetTargettype(void) const
	{
		return this->targettype != 0;
	}

	char SkillStub::GetType(void) const
	{
		return this->type;
	}

	bool SkillStub::ItemCondition(Skill* skill) const
	{
		if (this->need_item)
		{
			int total_item1_num = this->item1_num;
			int total_item2_num = this->item2_num;
			int item_relation = this->item_relation;
			if (item_relation == 1)
			{
				if (skill->pdata->item_id != -1 && skill->pdata->item_index != -1 && skill->pdata->consumable)
				{
					if (skill->pdata->item_id == this->item1_id)
					{
						++total_item1_num;
					}
					else if (skill->pdata->item_id == this->item2_id)
					{
						++total_item2_num;
					}
				}

				if (!skill->GetPlayer()->CheckItem(item1_id, total_item1_num))
					return 0;

				if (!skill->GetPlayer()->CheckItem(item2_id, total_item2_num))
					return 0;
			}
			else if (item_relation > 1)
			{
				if (item_relation == 2)
				{
					if (skill->pdata->item_id != -1 && skill->pdata->item_index != -1 && skill->pdata->consumable)
					{
						if (skill->pdata->item_id == this->item1_id)
						{
							++total_item1_num;
						}
						else if (skill->pdata->item_id == this->item2_id)
						{
							++total_item2_num;
						}
					}

					if (!skill->GetPlayer()->CheckItem(item1_id, total_item1_num))
					{
						if (!skill->GetPlayer()->CheckItem(item2_id, total_item2_num))
							return 0;
					}
				}
			}
			else if (!item_relation)
			{
				if (skill->pdata->item_id != -1
					&& skill->pdata->item_index != -1
					&& skill->pdata->consumable
					&& this->item1_id == skill->pdata->item_id)
				{
					++total_item1_num;
				}

				if (!skill->GetPlayer()->CheckItem(item1_id, total_item1_num))
					return 0;
			}
		}
		if (skill->pdata->item_id > 0 && skill->pdata->item_index >= 0)
		{
			if (!skill->GetPlayer()->CheckItem(skill->pdata->item_index, skill->pdata->item_id, 1))
				return 0;
		}
		return 1;

	}

	bool SkillStub::Learn(Skill* skill) const
	{
		if (LearnCondition(skill))
		{
			if (this->talent_type)
			{
				if (!skill->GetPlayer()->SetDectp(1))
					return 0;
			}
			else if (this->credittype)
			{
				skill->GetPlayer()->ModifyRegionReputation(this->credittype, -GetCreditCost(skill->GetLevel()));
			}
			else if (this->use_proficiency)
			{
				if (!skill->GetPlayer()->SetDecProficiency(skill->GetId(), GetRequiredProficiency(skill->GetLevel())))
					return 0;
			}
			else if (this->occupation == 167 || this->occupation == 168 || this->occupation == 169)
			{
				if (!skill->GetPlayer()->SetDecDeityExp(GetDivinityExp(skill->GetLevel())))
					return 0;
			}
			else if (!skill->GetPlayer()->SetDecsp(1))
			{
				return 0;
			}
			return 1;
		}
		return 0;

	}

	bool SkillStub::LearnCondition(Skill* skill) const
	{
		int level = skill->GetLevel();
		if (level <= 0 || level > this->maxlearn)
			return 0;
		PlayerWrapper* player = skill->GetPlayer();
		for (int i = 0; i < this->preskillnum; ++i)
		{
			if (GetPreSkillID(i))
			{
				if (player->GetBaseLevel(GetPreSkillID(i)) < GetPreSkillSP(i))
					return 0;
			}
			else if (GetPreSkillSP(i))
			{
				if (player->GetLevelSum(GNET::Rank::GetBase(this->occupation)) < GetPreSkillSP(i))
					return 0;
			}
		}
		switch (this->occupation)
		{
		case 0x94:
			if ((player->GetCultivation() & 1) == 0)
				return 0;
			break;
		case 0x95:
			if ((player->GetCultivation() & 2) == 0)
				return 0;
			break;
		case 0x96:
			if ((player->GetCultivation() & 4) == 0)
				return 0;
			break;
		case 0x98:
			if (!player->IsMarried())
				return 0;
			break;
		case 0xA7:
			if ((player->GetCultivation() & 1) == 0)
				return 0;
			if (player->GetSglevel() <= 0)
				return 0;
			if (player->GetSglevel() < GetDivinityLevel(level))
				return 0;
			if (player->GetDTExp() < GetDivinityExp(level))
				return 0;
			break;
		case 0xA8:
			if ((player->GetCultivation() & 2) == 0)
				return 0;
			if (player->GetSglevel() <= 0)
				return 0;
			if (player->GetSglevel() < GetDivinityLevel(level))
				return 0;
			if (player->GetDTExp() < GetDivinityExp(level))
				return 0;
			break;
		case 0xA9:
			if ((player->GetCultivation() & 4) == 0)
				return 0;
			if (player->GetSglevel() <= 0)
				return 0;
			if (player->GetSglevel() < GetDivinityLevel(level))
				return 0;
			if (player->GetDTExp() < GetDivinityExp(level))
				return 0;
			break;
		default:
			if (this->occupation != 255 && this->occupation != 166 && this->occupation != 147)
			{
				if (!GNET::Rank::SameBranch(player->GetOccupation(), occupation))
					return 0;
			}
			break;
		}
		if (player->GetLevel() < GetRequiredLevel(level))
			return 0;
		if (this->reborncount && player->GetReborncount() < this->reborncount)
			return 0;
		if (this->credittype)
		{
			if (player->GetRegionReputation(this->credittype) < GetCreditCost(level))
				return 0;
		}
		return this->occupation != 147 || player->IsMaster();


	}

	bool SkillStub::StateAttack(Skill*) const
	{
		return 0;
	}

	bool SkillStub::TakeEffect(Skill*) const
	{
		return 0;
	}


	//==================================SkillStub==================start


	//==================================Skill=======================start
	Skill::Skill(Skill const& r )
	{
		this->level = r.level;
		this->ratio = r.ratio;
		this->plus = r.plus;
		this->plus2 = r.plus2;
		this->crit = r.crit;
		this->crithurt = r.crithurt;
		this->skillaccu = r.skillaccu;
		this->forceattack = r.forceattack;
		this->saint = r.saint;
		this->t0 = r.t0;
		this->t1 = r.t1;
		this->t2 = r.t2;
		this->t3 = r.t3;
		this->t4 = r.t4;
		this->t5 = r.t5;
		this->t6 = r.t6;
		this->t7 = r.t7;
		this->damage = r.damage;
		this->player = r.player;
		this->pdata = r.pdata;
		memcpy(&this->attacker, &r.attacker, sizeof(attacker));

		this->stub = r.stub;
		this->c0 = r.c0;
		this->c1 = r.c1;
		this->c2 = r.c2;
		this->c3 = r.c3;
		this->c4 = r.c4;
		this->j0 = r.j0;
		this->j1 = r.j1;
		this->j2 = r.j2;
		this->j3 = r.j3;
		this->j4 = r.j4;
		this->j5 = r.j5;
		this->j6 = r.j6;
		this->j7 = r.j7;
		this->j8 = r.j8;
		this->j9 = r.j9;
		memcpy(this->skillelems, r.skillelems, sizeof(this->skillelems));
	}

	Skill::Skill(uint i)
	{
		this->level = 0;
		this->ratio = 1.0;
		this->plus = 0.0;
		this->plus2 = 0.0;
		this->crit = 0.0;
		this->crithurt = 0.0;
		this->skillaccu = 0;
		this->forceattack = 0;
		this->saint = 0;
		this->t0 = 0;
		this->t1 = 0;
		this->t2 = 0;
		this->t3 = 0;
		this->t4 = 0;
		this->t5 = 0;
		this->t6 = 0;
		this->t7 = 0;
		this->damage = 0;
		this->real_damage = 0;
		this->player = 0;
		this->pdata = 0;
		this->attackermode = 0;

		this->c0 = 0;
		this->c1 = 0;
		this->c2 = 0;
		this->c3 = 0;
		this->c4 = 0;
		this->j0 = 0;
		this->j1 = 0;
		this->j2 = 0;
		this->j3 = 0;
		this->j4 = 0;
		this->j5 = 0;
		this->j6 = 0;
		this->j7 = 0;
		this->j8 = 0;
		this->j9 = 0;
		this->stub = SkillStub::GetStub(i);
		if(!GetStub(i)) 
			GetMap().insert(std::make_pair(i, this));	
		memset(&this->attacker, 0, sizeof(this->attacker));
		memset(this->skillelems, 0, sizeof(this->skillelems));

	}

	Skill::~Skill()
	{
		Clear();
	}

	bool Skill::BlessMe(void)
	{
		return stub->BlessMe(this);
	}

	bool Skill::CanAttack(void)
	{
		return this->stub->type != 1 || this->player->CanAttack();
	}

	bool Skill::Cancel(void)
	{
		if (InvalidState())
			return 0;
		return stub->GetState(GetStateindex())->Cancel(this);
	}

	void Skill::Clear(void)
	{
		//NOTHING
	}

	Skill* Skill::Clone(void) const
	{
		return new Skill(*this);
	}

	int Skill::Condition(void)
	{
		return this->stub->Condition(this);
	}

	Skill* Skill::Create(uint i)
	{
		const Skill* s = GNET::Skill::GetStub(i);
		if (s)
			return s->Clone();
		else
			return 0;
	}

	void Skill::Destroy(void)
	{
		delete this;   
	}

	bool Skill::DoEnchant(void)
	{
		return this->stub->doenchant;
	}

	int Skill::FirstRun(int& next_interval, int prayspeed)
	{
		next_interval = -1;
		int nextindex = NextState(-1);
		SetStateindex(nextindex);
		if (nextindex < 0)
			return -1;
		const SkillStub::State* state = stub->GetState(0);
		Run(state);
		int time = state->GetTime(this);
		if (prayspeed > 89)
			prayspeed = 90;
		time = (int)(time * 0.0099999998 * (100 - prayspeed) + 0.0099999998);
		int nextindexa = NextState(nextindex);
		if (nextindexa < 0)
		{
			next_interval = -1;
		}
		else
		{
			next_interval = stub->GetState(nextindexa)->GetTime(this);
		}
		SetNextindex(nextindexa);
		if (!time)
			return -1;
		return time;

	}

	float Skill::GetAngle(void)
	{
		return this->stub->GetAngle(this);
	}

	float Skill::GetAttackRange(void)
	{
		return this->attack_range;
	}

	float Skill::GetAttackdistance(void)
	{
		return stub->GetAttackdistance(this);
	}

	const attacker_info_t& Skill::GetAttacker(void)
	{
		return this->attacker;
	}

	int Skill::GetAttackerFaction(void)
	{
		return this->attacker_faction;
	}

	char Skill::GetAttackerMode(void)
	{
		return this->attackermode;
	}

	XID& Skill::GetAttackerid(void)
	{
		return this->attacker.attacker;
	}

	int Skill::GetAttackerlevel(void)
	{
		return this->attacker.level;
	}

	const A3DVECTOR& Skill::GetAttackerpos(void)
	{
		return this->attackerpos;
	}

	int Skill::GetC0(void)
	{
		return this->c0;
	}

	int Skill::GetC1(void)
	{
		return this->c1;
	}

	int Skill::GetC2(void)
	{
		return this->c2;
	}

	int Skill::GetC3(void)
	{
		return this->c3;
	}

	int Skill::GetC4(void)
	{
		return this->c4;
	}

	const A3DVECTOR& Skill::GetChargeDestTarget(void)
	{
		return this->chargeDestPos;
	}

	float Skill::GetChargeDist(void)
	{
		return this->stub->chargedist;
	}

	const XID& Skill::GetChargeTarget(void)
	{
		return this->chargeTarget;
	}

	char Skill::GetChargeType(void)
	{
		return this->stub->charge;
	}

	unsigned int  Skill::GetCharging(void)
	{
		return this->pdata->warmuptime;
	}

	int Skill::GetCooltime(void)
	{
		int cooltime = this->stub->GetCooldowntime(this);
		if (this->pdata)
			cooltime += this->pdata->cooltime;
		if (cooltime < 0)
			return 0;
		return cooltime;
	}

	int Skill::GetCoverage(void)
	{
		return this->stub->GetCoverage(this);
	}

	float Skill::GetCrit(void)
	{
		return this->crit;
	}

	float Skill::GetCrithurt(void)
	{
		return this->crithurt;
	}

	int Skill::GetDamage(void)
	{
		return this->damage;
	}

	char Skill::GetDarkLightType(void)
	{
		return this->stub->darkLightType;
	}

	const SKILL::Data* Skill::GetData(void)
	{
		return this->pdata;
	}

	float Skill::GetDpcost(void)
	{
		return this->stub->GetDpcost(this);
	}

	float Skill::GetDropRate(void)
	{
		return this->stub->GetDropRate(this);
	}

	float Skill::GetEffectdistance(void)
	{
		return this->stub->GetEffectdistance(this);
	}

	int Skill::GetEnmity(void)
	{
		return this->stub->GetEnmity(this);
	}

	int Skill::GetEventFlag(void)
	{
		return this->GetStub()->GetEventFlag();
	}

	char Skill::GetForceattack(void)
	{
		return this->forceattack;
	}

	float Skill::GetHpcost(void)
	{
		return this->stub->GetHpcost(this);
	}

	unsigned int  Skill::GetId(void)
	{
		return this->stub->id;
	}

	int Skill::GetIncProficiency(void)
	{
		return this->stub->inc_proficiency;
	}

	float Skill::GetInkcost(void)
	{
		return this->stub->GetInkcost(this);
	}

	int Skill::GetIscrit(void)
	{
		return this->is_crit;
	}

	int Skill::GetItem1Id(void)
	{
		return this->stub->item1_id;
	}

	int Skill::GetItem1Num(void)
	{
		return this->stub->item1_num;
	}

	int Skill::GetItem2Id(void)
	{
		return this->stub->item2_id;
	}

	int Skill::GetItem2Num(void)
	{
		return this->stub->item2_num;
	}

	char Skill::GetItemRelation(void)
	{
		return this->stub->item_relation;
	}

	int Skill::GetJ0(void)
	{
		return this->j0;
	}

	int Skill::GetJ1(void)
	{
		return this->j1;
	}

	int Skill::GetJ2(void)
	{
		return this->j2;
	}

	int Skill::GetJ3(void)
	{
		return this->j3;
	}

	int Skill::GetLevel(void)
	{
		return this->level;
	}

	unsigned int  Skill::GetLimit(void)
	{
		return this->stub->skill_limit;
	}

	std::map<unsigned int, const GNET::Skill*>& Skill::GetMap(void)
	{
		static std::map<unsigned int, const Skill*> map;
		return map;
	}

	int Skill::GetMaxLearn(void)
	{
		return this->stub->maxlearn;
	}

	int Skill::GetMaxlevel(void)
	{
		return this->stub->maxlevel;
	}

	float Skill::GetMpcost(void)
	{
		return this->stub->GetMpcost(this);
	}

	int Skill::GetNextindex(void)
	{
		return this->pdata->nextindex;
	}

	int Skill::GetOccupation(void)
	{
		return this->stub->occupation;
	}

	PlayerWrapper* Skill::GetPlayer(void)
	{
		return this->player;
	}

	float Skill::GetPlus(void)
	{
		return this->plus;
	}

	float Skill::GetPlus2(void)
	{
		return this->plus2;
	}

	const A3DVECTOR& Skill::GetPosition(void)
	{
		return this->pdata->pos;
	}

	float Skill::GetPraydistance(void)
	{
		return this->stub->GetPraydistance(this);
	}

	float Skill::GetRadius(void)
	{
		return this->stub->GetRadius(this);
	}

	char Skill::GetRange(void)
	{
		return this->stub->rangetype;
	}

	float Skill::GetRatio(void)
	{
		return this->ratio;
	}

	int Skill::GetRealDamage(void)
	{
		return this->real_damage;
	}

	int Skill::GetSaint(void)
	{
		return this->saint;
	}

	int Skill::GetSerialSkill(void)
	{
		return this->stub->serialskill;
	}

	const unsigned short* Skill::GetSkillElems(void)
	{
		return this->skillelems;
	}

	char Skill::GetSkillSpiritIndex(void)
	{
		if (this->pdata)
			return this->pdata->spirit_index;
		else
			return -1;
	}

	int Skill::GetSkillaccu(void)
	{
		return this->skillaccu;
	}

	const SkillStub::State* Skill::GetState(int index)
	{
		return this->stub->GetState(index);
	}

	int Skill::GetStateindex(void)
	{
		return this->pdata->stateindex;
	}

	const Skill* Skill::GetStub(uint i)
	{
		std::map<unsigned int, const Skill*>::iterator it = Skill::GetMap().find(i);
		if (it == Skill::GetMap().end())
			return 0;
		else
			return it->second;
	}

	const SkillStub* Skill::GetStub(void)
	{
		return this->stub;
	}

	int Skill::GetT0(void)
	{
		return this->t0;
	}

	int Skill::GetT1(void)
	{
		return this->t1;
	}

	int Skill::GetT2(void)
	{
		return this->t2;
	}

	int Skill::GetT3(void)
	{
		return this->t3;
	}

	int Skill::GetT4(void)
	{
		return this->t4;
	}

	int Skill::GetT5(void)
	{
		return this->t5;
	}

	int Skill::GetT6(void)
	{
		return this->t6;
	}

	int Skill::GetT7(void)
	{
		return this->t7;
	}

	int Skill::GetTargetCnt(void)
	{
		return this->stub->targetcnt;
	}

	int Skill::GetTargetFaction(void)
	{
		return this->target_faction;
	}

	bool Skill::GetTargettype(void)
	{
		return this->stub->targettype != 0;
	}

	float Skill::GetTianhua1(void)
	{
		return this->tianhua1;
	}

	char Skill::GetType(void)
	{
		return this->stub->type;
	}

	PlayerWrapper* Skill::GetVictim(void)
	{
		return this->player;
	}

	void Skill::IncSkillElemsNum(int index, int inc)
	{
		switch (index)
		{
		case 0:
			this->j0 += inc;
			break;
		case 1:
			this->j1 += inc;
			break;
		case 2:
			this->j2 += inc;
			break;
		case 3:
			this->j3 += inc;
			break;
		case 4:
			this->j4 += inc;
			break;
		case 5:
			this->j5 += inc;
			break;
		case 6:
			this->j6 += inc;
			break;
		case 7:
			this->j7 += inc;
			break;
		case 8:
			this->j8 += inc;
			break;
		case 9:
			this->j9 += inc;
			break;
		default:
			return;
		}
	}

	int Skill::InstantRun(void)
	{
		const SkillStub::State* state = this->stub->GetState(0);
		if (state)
			Run(state);
		return 1;
	}

	bool Skill::Interrupt(void)
	{
		if (InvalidState())
			return 0;
		return stub->GetState(GetStateindex())->Interrupt(this);

	}

	bool Skill::InvalidState(void)
	{
		if (this->pdata->stateindex < 0)
			return 1;
		pdata = this->pdata;
		if (pdata->stateindex >= this->stub->GetStateSize())
			return 1;
		return 0;
	}

	bool Skill::IsCastSelf(void)
	{
		return this->stub->rangetype == 5;
	}

	bool Skill::IsCharge(void)
	{
		return this->stub->charge == 1;
	}

	bool Skill::IsComboSkill(void)
	{
		return this->stub->comboskill == 1;
	}

	bool Skill::IsCycle(void)
	{
		return this->stub->cycle;
	}

	bool Skill::IsFamily(void)
	{
		return this->stub->occupation == 131;
	}

	bool Skill::IsInstant(void)
	{
		return this->stub->timetype == 1;
	}

	bool Skill::IsNeedItem(void)
	{
		return this->stub->need_item;
	}

	bool Skill::IsRune(void)
	{
		return this->stub->type == 13;
	}

	bool Skill::IsSpouseSkill(void)
	{
		return this->stub->occupation == 152;
	}

	bool Skill::IsTalisman(void)
	{
		return this->stub->occupation == 130;
	}

	bool Skill::IsTrigger(void)
	{
		return this->stub->trigger;
	}

	bool Skill::IsUseProficiency(void)
	{
		return this->stub->use_proficiency;
	}

	bool Skill::IsWarmup(void)
	{
		return this->stub->timetype == 3;
	}

	bool Skill::Learn(void)
	{
		return this->stub->Learn(this);
	}

	int Skill::NextState(int index)
	{
		int statesize = this->stub->GetStateSize();
		if (index >= statesize)
			return -1;
		if (index < 0)
			index = -1;
		if (index >= 0 && stub->GetState(index)->Loop(this) )
		{
			if (stub->GetState(index)->Quit(this))
				return -1;
			else
				return index;
		}
		else
		{
			while (++index < statesize)
			{
				if (stub->GetState(index)->Quit(this))
					return -1;
				if (!stub->GetState(index)->Bypass(this))
					return index;
			}
			return -1;
		}

	}

	void Skill::NpcRun(void)
	{
		Run(stub->GetState(1));
	}

	bool Skill::Run(SkillStub::State const* state)
	{
		state->Calculate(this);
		return 1;
	}

	int Skill::Run(int& next_interval)
	{
		next_interval = -1;
		int newindex = GetNextindex();
		if (newindex < 0 || newindex >= this->stub->GetStateSize())
			return -1;
		SetStateindex(newindex);
		const SkillStub::State* state = this->stub->GetState(newindex);
		Run(state);
		int nextindex =NextState(newindex);
		if (nextindex < 0)
		{
			next_interval = -1;
		}
		else
		{
			next_interval = stub->GetState(nextindex)->GetTime(this);
		}
		SetNextindex(nextindex);
		if (!state->GetTime(this))
			return -1;
		return state->GetTime(this);

	}

	void Skill::SetAttackRange(float r)
	{
		this->attack_range = r;
	}

	void Skill::SetAttackerpos(A3DVECTOR const& p)
	{
		this->attackerpos = p;
	}

	void Skill::SetCastSelf(PlayerWrapper* p)
	{
		this->attacker.attacker.type = p->GetXid().type;
		this->attacker.attacker.id = p->GetXid().id;
		this->attacker.level = p->GetLevel();
		this->player = p;
	}

	void Skill::SetChargeDestPos(A3DVECTOR const& pos)
	{
		this->chargeDestPos = pos;
	}

	void Skill::SetChargeTarget(XID const& target)
	{
		this->chargeTarget.type = target.type;
		this->chargeTarget.id = target.id;
	}

	void Skill::SetCharging(int w)
	{
		if (this->pdata->warmuptime < 0 || this->pdata->warmuptime > w)
			this->pdata->warmuptime = w;
	}

	void Skill::SetCrit(float c)
	{
		this->crit = c;
	}

	void Skill::SetCrithurt(float c)
	{
		this->crithurt = c;
	}

	void Skill::SetData(SKILL::Data* data)
	{
		this->pdata = data;
		if (data)
			this->forceattack = data->forceattack;
	}

	void Skill::SetIscrit(int iscrit)
	{
		this->is_crit = iscrit;
	}

	void Skill::SetLevel(int l)
	{
		this->level = l;
	}

	void Skill::SetMessage(const attack_msg * msg)
	{
		memcpy(&this->attacker, msg, sizeof(this->attacker));
		this->attackermode = msg->attacker_mode;
		this->damage = (msg->damage_low + msg->damage_high) / 2;
		this->attacker_faction = msg->attacker_faction;
		this->target_faction = msg->target_faction;
		this->forceattack = msg->force_attack;
	}

	void Skill::SetMessage(const enchant_msg * msg)
	{
		memcpy(&this->attacker, msg, sizeof(this->attacker));
		this->attackermode = msg->attacker_mode;
		this->damage = msg->skill_reserved1;
		this->forceattack = msg->force_attack;
		this->attacker_faction = msg->attacker_faction;
		this->target_faction = msg->target_faction;
	}

	void Skill::SetNextindex(int s)
	{
		this->pdata->nextindex = s;
	}

	void Skill::SetPlayer(PlayerWrapper* p)
	{
		this->player = p;
	}

	void Skill::SetPlus(float p)
	{
		this->plus = p;
	}

	void Skill::SetPlus2(float p)
	{
		this->plus2 = p;
	}

	void Skill::SetRatio(float r)
	{
		this->ratio = r;
	}

	void Skill::SetRealDamage(int dmg)
	{
		this->real_damage = dmg;
	}

	void Skill::SetSaint(int s)
	{
		this->saint = s;
	}

	void Skill::SetSkillElems(unsigned short * elems)
	{
		memcpy(this->skillelems, elems, sizeof(this->skillelems));
	}

	void Skill::SetSkillState(int state)
	{
		if (this->pdata)
			this->pdata->skillstate = state;
	}

	void Skill::SetSkillaccu(int c)
	{
		this->skillaccu = c;
	}

	void Skill::SetStateindex(int s)
	{
		this->pdata->stateindex = s;
	}

	void Skill::SetSucceedSkillColor(int index, int num)
	{
		switch (index)
		{
		case 0:
			this->c0 = num;
			break;
		case 1:
			this->c1 = num;
			break;
		case 2:
			this->c2 = num;
			break;
		case 3:
			this->c3 = num;
			break;
		case 4:
			this->c4 = num;
			break;
		default:
			return;
		}
	}

	void Skill::SetTalent(int index, int level)
	{
		switch (index)
		{
		case 0:
			this->t0 = level;
			break;
		case 1:
			this->t1 = level;
			break;
		case 2:
			this->t2 = level;
			break;
		case 3:
			this->t3 = level;
			break;
		case 4:
			this->t4 = level;
			break;
		case 5:
			this->t5 = level;
			break;
		case 6:
			this->t6 = level;
			break;
		case 7:
			this->t7 = level;
			break;
		default:
			return;
		}
	}

	void Skill::SetTianhua1(float v)
	{
		this->tianhua1 = v;
	}

	bool Skill::StateAttack(void)
	{
		return this->stub->StateAttack(this);
	}

	bool Skill::TakeEffect(PlayerWrapper*)
	{
		return GetStub()->TakeEffect(this);
	}

	bool Skill::UndoEffect(PlayerWrapper* player)
	{
		player->SetContext(3);
		return GetStub()->TakeEffect(this);

	}



	//==================================Skill=======================end



};

