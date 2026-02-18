#ifndef __CPPGEN_GNET_SKILL3011
#define __CPPGEN_GNET_SKILL3011
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3011:public Skill
    {
      public:
        enum
        { SKILL_ID = 3011 };
          Skill3011 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3011Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
      Skill3011Stub ():SkillStub (3011)
        {
            occupation = 167;
            name = L"惊鸿梦";
            nativename = "惊鸿梦";
            icon = "时光倒流.dds";
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
            action[0] = "仙_神圣爆发";
            action[1] = "仙_神圣爆发";
            action[2] = "仙_神圣爆发";
            action[3] = "仙_神圣爆发";
            action[4] = "仙_神圣爆发";
            action[5] = "仙_神圣爆发";
            action[6] = "仙_神圣爆发";
            action[7] = "仙_神圣爆发";
            action[8] = "仙_神圣爆发";
            action[9] = "仙_神圣爆发";
            action[10] = "仙_神圣爆发";
            action[11] = "仙_神圣爆发";
            action[12] = "仙_神圣爆发";
            action[13] = "仙_神圣爆发";
            action[14] = "仙_神圣爆发";
            action[15] = "骑乘_仙_神圣爆发";
            action[16] = "骑乘_仙_神圣爆发";
            action[17] = "骑乘_仙_神圣爆发";
            action[18] = "骑乘_仙_神圣爆发";
            action[19] = "骑乘_仙_神圣爆发";
            action[20] = "骑乘_仙_神圣爆发";
            action[21] = "骑乘_仙_神圣爆发";
            action[22] = "骑乘_仙_神圣爆发";
            action[23] = "骑乘_仙_神圣爆发";
            action[24] = "骑乘_仙_神圣爆发";
            action[25] = "骑乘_仙_神圣爆发";
            action[26] = "骑乘_仙_神圣爆发";
            action[27] = "骑乘_仙_神圣爆发";
            action[28] = "骑乘_仙_神圣爆发";
            action[29] = "骑乘_仙_神圣爆发";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 6;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3011Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 360100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 4000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (14);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               120 - 4 * skill->GetLevel (),
                               INT (1 + skill->GetLevel () / 4.9), 20 + 4 * skill->GetLevel (), 20 + 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6050);
            skill->GetVictim ()->SetGoback (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (8050 + 4000 * norm (skill->GetLevel () / 19.9));
            skill->GetVictim ()->SetAmount (4);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetSilenttimer (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (1 + INT (skill->GetLevel () / 4.9));
        }
#endif
    };
}
#endif
