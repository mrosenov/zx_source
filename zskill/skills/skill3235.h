#ifndef __CPPGEN_GNET_SKILL3235
#define __CPPGEN_GNET_SKILL3235
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3235:public Skill
    {
      public:
        enum
        { SKILL_ID = 3235 };
          Skill3235 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3235Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3235Stub ():SkillStub (3235)
        {
            occupation = 129;
            name = L"雪韵淞情";
            nativename = "雪韵淞情";
            icon = "焚心厉雷.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
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
            action[0] = "青云门_破魔剑气";
            action[1] = "青云门_破魔剑气";
            action[2] = "青云门_破魔剑气";
            action[3] = "青云门_破魔剑气";
            action[4] = "青云门_破魔剑气";
            action[5] = "鬼道_玉真诀";
            action[6] = "青云门_破魔剑气";
            action[7] = "兽神_雪琪之冰霜";
            action[8] = "狐妖_雪琪之冰霜";
            action[9] = "轩辕_雪琪之冰霜";
            action[10] = "怀光_雪琪之冰霜";
            action[11] = "兽神_雪琪之冰霜";
            action[12] = "天华_雪琪之冰霜";
            action[13] = "";
            action[14] = "";
            action[15] = "青云门_骑乘_破魔剑气";
            action[16] = "青云门_骑乘_破魔剑气";
            action[17] = "青云门_骑乘_破魔剑气";
            action[18] = "青云门_骑乘_破魔剑气";
            action[19] = "青云门_骑乘_破魔剑气";
            action[20] = "鬼道_骑乘_玉真诀";
            action[21] = "青云门_骑乘_破魔剑气";
            action[22] = "兽神_骑乘_雪琪之冰霜";
            action[23] = "狐妖_骑乘_雪琪之冰霜";
            action[24] = "轩辕_骑乘_雪琪之冰霜";
            action[25] = "怀光_骑乘_雪琪之冰霜";
            action[26] = "兽神_骑乘_雪琪之冰霜";
            action[27] = "天华_骑乘_雪琪之冰霜";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 1;
            doenchant = true;
            dobless = true;
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
        virtual ~ Skill3235Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (35);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (2);
        }
        float GetAttackdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (25);
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
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDecskilldamage (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (106);
            skill->GetVictim ()->SetSubanti (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
