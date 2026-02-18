#ifndef __CPPGEN_GNET_SKILL3975
#define __CPPGEN_GNET_SKILL3975
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3975:public Skill
    {
      public:
        enum
        { SKILL_ID = 3975 };
          Skill3975 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3975Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3975Stub ():SkillStub (3975)
        {
            occupation = 151;
            name = L"八门遁甲・化生";
            nativename = "八门遁甲・化生";
            icon = "爱情良药.dds";
            maxlevel = 1;
            maxlearn = 0;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
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
            action[0] = "鬼王宗_回春";
            action[1] = "青云门_回春";
            action[2] = "鬼王宗_回春";
            action[3] = "合欢派_回春";
            action[4] = "天音寺_回春";
            action[5] = "鬼道_回春";
            action[6] = "焚香谷_回春";
            action[7] = "兽神_回春";
            action[8] = "狐妖_回春";
            action[9] = "轩辕_回春";
            action[10] = "怀光_回春";
            action[11] = "太昊_回春";
            action[12] = "天华_回春";
            action[13] = "灵夙_回春";
            action[14] = "英招_回春";
            action[15] = "鬼王宗_骑乘_回春";
            action[16] = "青云门_骑乘_回春";
            action[17] = "鬼王宗_骑乘_回春";
            action[18] = "合欢派_骑乘_回春";
            action[19] = "天音寺_骑乘_回春";
            action[20] = "鬼道_骑乘_回春";
            action[21] = "焚香谷_骑乘_回春";
            action[22] = "兽神_骑乘_回春";
            action[23] = "狐妖_骑乘_回春";
            action[24] = "轩辕_骑乘_回春";
            action[25] = "怀光_骑乘_回春";
            action[26] = "太昊_骑乘_回春";
            action[27] = "天华_骑乘_回春";
            action[28] = "灵夙_骑乘_回春";
            action[29] = "英招_骑乘_回春";
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
        virtual ~ Skill3975Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
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
            return 10000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.6 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.6 * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetMpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.6 * skill->GetPlayer ()->GetMaxdp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetDivinityfury (1);
            return true;
        }
#endif
    };
}
#endif
