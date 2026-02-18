#ifndef __CPPGEN_GNET_SKILL1550
#define __CPPGEN_GNET_SKILL1550
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1550:public Skill
    {
      public:
        enum
        { SKILL_ID = 1550 };
          Skill1550 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1550Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1550Stub ():SkillStub (1550)
        {
            occupation = 149;
            name = L"狂神";
            nativename = "狂神";
            icon = "狂神.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 2;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 1549;
            talent[1] = 1552;
            talent_size = 2;
            action[0] = "青云门_狂神";
            action[1] = "青云门_狂神";
            action[2] = "鬼王宗_狂神";
            action[3] = "合欢派_狂神";
            action[4] = "天音寺_狂神";
            action[5] = "鬼道_狂神";
            action[6] = "焚香谷_狂神";
            action[7] = "兽神_狂神";
            action[8] = "狐妖_狂神";
            action[9] = "轩辕_狂神";
            action[10] = "怀光_狂神";
            action[11] = "太昊_狂神";
            action[12] = "天华_狂神";
            action[13] = "灵夙_狂神";
            action[14] = "英招_狂神";
            action[15] = "青云门_骑乘_狂神";
            action[16] = "青云门_骑乘_狂神";
            action[17] = "鬼王宗_骑乘_狂神";
            action[18] = "合欢派_骑乘_狂神";
            action[19] = "天音寺_骑乘_狂神";
            action[20] = "鬼道_骑乘_狂神";
            action[21] = "焚香谷_骑乘_狂神";
            action[22] = "兽神_骑乘_狂神";
            action[23] = "狐妖_骑乘_狂神";
            action[24] = "轩辕_骑乘_狂神";
            action[25] = "怀光_骑乘_狂神";
            action[26] = "太昊_骑乘_狂神";
            action[27] = "天华_骑乘_狂神";
            action[28] = "灵夙_骑乘_狂神";
            action[29] = "英招_骑乘_狂神";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1550Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1552 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (325 + 25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 720000 - 36000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 325 + 25 * skill->GetLevel (), 30, 15 * skill->GetLevel (), 30, skill->GetLevel (), 12);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000 + 3000 * skill->GetT0 () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (30 + 3 * skill->GetT0 ());
            skill->GetVictim ()->SetValue (15 * skill->GetLevel ());
            skill->GetVictim ()->SetFrenzied (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000 + 100);
            skill->GetVictim ()->SetRatio (0.0101 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncskillaccu (1);
            skill->GetVictim ()->SetTime ((30000 + 100) * int (skill->GetLevel () / 10));
              skill->GetVictim ()->SetBuffid (2);
              skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * 0.1 * int (skill->GetLevel () / 10));
              skill->GetVictim ()->SetAddhp (1);
              return true;
        }
#endif
    };
}
#endif
