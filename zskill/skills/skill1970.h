#ifndef __CPPGEN_GNET_SKILL1970
#define __CPPGEN_GNET_SKILL1970
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1970:public Skill
    {
      public:
        enum
        { SKILL_ID = 1970 };
          Skill1970 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1970Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 4000;
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1970Stub ():SkillStub (1970)
        {
            occupation = 166;
            name = L"捕捉";
            nativename = "捕捉";
            icon = "捕捉.dds";
            maxlevel = 10;
            maxlearn = 10;
            type = 3;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 256;
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
            action[0] = "青云门_动物捕捉";
            action[1] = "青云门_动物捕捉";
            action[2] = "鬼王宗_动物捕捉";
            action[3] = "合欢派_动物捕捉";
            action[4] = "天音寺_动物捕捉";
            action[5] = "鬼道_动物捕捉";
            action[6] = "鬼道_动物捕捉";
            action[7] = "兽神_动物捕捉";
            action[8] = "狐妖_动物捕捉";
            action[9] = "轩辕_动物捕捉";
            action[10] = "怀光_动物捕捉";
            action[11] = "太昊_动物捕捉";
            action[12] = "天华_动物捕捉";
            action[13] = "灵夙_动物捕捉";
            action[14] = "英招_动物捕捉";
            action[15] = "青云门_骑乘_动物捕捉";
            action[16] = "青云门_骑乘_动物捕捉";
            action[17] = "鬼王宗_骑乘_动物捕捉";
            action[18] = "合欢派_骑乘_动物捕捉";
            action[19] = "天音寺_骑乘_动物捕捉";
            action[20] = "鬼道_骑乘_动物捕捉";
            action[21] = "青云门_骑乘_动物捕捉";
            action[22] = "兽神_骑乘_动物捕捉";
            action[23] = "狐妖_骑乘_动物捕捉";
            action[24] = "轩辕_骑乘_动物捕捉";
            action[25] = "怀光_骑乘_动物捕捉";
            action[26] = "太昊_骑乘_动物捕捉";
            action[27] = "天华_骑乘_动物捕捉";
            action[28] = "灵夙_骑乘_动物捕捉";
            action[29] = "英招_骑乘_动物捕捉";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 31754;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            use_proficiency = true;
            inc_proficiency = 0;
            skill_limit = 14;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1970Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 6000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 };
            return array[level - 1];
        }
        int GetRequiredProficiency (int level) const
        {
            static int array[10] = { 0, 6, 48, 144, 432, 864, 2592, 5184, 9216, 14256 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2000);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), skill->GetCurproficiency (), skill->GetNeedproficiency ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetCatchpet (1);
            return true;
        }
#endif
    };
}
#endif
