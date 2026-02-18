#ifndef __CPPGEN_GNET_SKILL3009
#define __CPPGEN_GNET_SKILL3009
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3009:public Skill
    {
      public:
        enum
        { SKILL_ID = 3009 };
          Skill3009 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3009Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3009Stub ():SkillStub (3009)
        {
            occupation = 167;
            name = L"逆水寒";
            nativename = "逆水寒";
            icon = "极度寒冷.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
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
            preskillnum = 0;
            action[0] = "仙_银河星爆";
            action[1] = "仙_银河星爆";
            action[2] = "仙_银河星爆";
            action[3] = "仙_银河星爆";
            action[4] = "仙_银河星爆";
            action[5] = "仙_银河星爆";
            action[6] = "仙_银河星爆";
            action[7] = "仙_银河星爆";
            action[8] = "仙_银河星爆";
            action[9] = "仙_银河星爆";
            action[10] = "仙_银河星爆";
            action[11] = "仙_银河星爆";
            action[12] = "仙_银河星爆";
            action[13] = "仙_银河星爆";
            action[14] = "仙_银河星爆";
            action[15] = "骑乘_仙_银河星爆";
            action[16] = "骑乘_仙_银河星爆";
            action[17] = "骑乘_仙_银河星爆";
            action[18] = "骑乘_仙_银河星爆";
            action[19] = "骑乘_仙_银河星爆";
            action[20] = "骑乘_仙_银河星爆";
            action[21] = "骑乘_仙_银河星爆";
            action[22] = "骑乘_仙_银河星爆";
            action[23] = "骑乘_仙_银河星爆";
            action[24] = "骑乘_仙_银河星爆";
            action[25] = "骑乘_仙_银河星爆";
            action[26] = "骑乘_仙_银河星爆";
            action[27] = "骑乘_仙_银河星爆";
            action[28] = "骑乘_仙_银河星爆";
            action[29] = "骑乘_仙_银河星爆";
            action[30] = "0";
            rangetype = 0;
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
        virtual ~ Skill3009Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 4000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               120 - 4 * skill->GetLevel (),
                               skill->GetLevel (), skill->GetLevel (), 2 * skill->GetLevel (), 2 + 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 4000);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetColdinjure (1);
            skill->GetVictim ()->SetProbability (1.0 * (900));
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (skill->GetLevel () / 19.9) * 70));
            skill->GetVictim ()->SetTime (3050);
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
#endif
    };
}
#endif
