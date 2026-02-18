#ifndef __CPPGEN_GNET_SKILL1810
#define __CPPGEN_GNET_SKILL1810
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1810:public Skill
    {
      public:
        enum
        { SKILL_ID = 1810 };
          Skill1810 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1810Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 9750;
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
                return 250;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1810Stub ():SkillStub (1810)
        {
            occupation = 152;
            name = L"天涯比翼";
            nativename = "天涯比翼";
            icon = "天涯比翼.dds";
            maxlevel = 10;
            maxlearn = 3;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 7;
            credittype = 6;
            clearmask = 8;
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
            action[0] = "鬼王宗_夫妻技能_天涯比翼";
            action[1] = "青云门_夫妻技能_天涯比翼";
            action[2] = "鬼王宗_夫妻技能_天涯比翼";
            action[3] = "合欢派_夫妻技能_天涯比翼";
            action[4] = "天音寺_夫妻技能_天涯比翼";
            action[5] = "鬼道_夫妻技能_天涯比翼";
            action[6] = "焚香谷_夫妻技能_天涯比翼";
            action[7] = "兽神_夫妻技能_天涯比翼";
            action[8] = "狐妖_夫妻技能_天涯比翼";
            action[9] = "轩辕_夫妻技能_天涯比翼";
            action[10] = "怀光_夫妻技能_天涯比翼";
            action[11] = "太昊_夫妻技能_天涯比翼";
            action[12] = "天华_夫妻技能_天涯比翼";
            action[13] = "灵夙_夫妻技能_天涯比翼";
            action[14] = "英招_夫妻技能_天涯比翼";
            action[15] = "鬼王宗_骑乘_夫妻技能_天涯比翼";
            action[16] = "青云门_骑乘_夫妻技能_天涯比翼";
            action[17] = "鬼王宗_骑乘_夫妻技能_天涯比翼";
            action[18] = "合欢派_骑乘_夫妻技能_天涯比翼";
            action[19] = "天音寺_骑乘_夫妻技能_天涯比翼";
            action[20] = "鬼道_骑乘_夫妻技能_天涯比翼";
            action[21] = "焚香谷_骑乘_夫妻技能_天涯比翼";
            action[22] = "兽神_骑乘_夫妻技能_天涯比翼";
            action[23] = "狐妖_骑乘_夫妻技能_天涯比翼";
            action[24] = "轩辕_骑乘_夫妻技能_天涯比翼";
            action[25] = "怀光_骑乘_夫妻技能_天涯比翼";
            action[26] = "太昊_骑乘_夫妻技能_天涯比翼";
            action[27] = "天华_骑乘_夫妻技能_天涯比翼";
            action[28] = "灵夙_骑乘_夫妻技能_天涯比翼";
            action[29] = "英招_骑乘_夫妻技能_天涯比翼";
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
        virtual ~ Skill1810Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return skill->GetLevel () == 4 ? 3 * 3600000 : (33 - 9 * skill->GetLevel ()) * 3600000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[3] = { 30, 30, 30 };
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
            static int array[3] = { 1200, 3600, 7200 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel () == 4 ? 3 : (33 - 9 * skill->GetLevel ()));

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
            skill->GetVictim ()->SetJumptospouse (1);
            return true;
        }
#endif
    };
}
#endif
