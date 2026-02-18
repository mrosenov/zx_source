#ifndef __CPPGEN_GNET_SKILL3283
#define __CPPGEN_GNET_SKILL3283
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3283:public Skill
    {
      public:
        enum
        { SKILL_ID = 3283 };
          Skill3283 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3283Stub:public SkillStub
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
      Skill3283Stub ():SkillStub (3283)
        {
            occupation = 129;
            name = L"鬼厉的愤怒";
            nativename = "鬼厉的愤怒";
            icon = "鬼厉的愤怒.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 13;
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
            action[0] = "鬼王宗_枯木咒";
            action[1] = "青云门_炼气还神";
            action[2] = "鬼王宗_枯木咒";
            action[3] = "合欢派_激励";
            action[4] = "天音寺_易筋经";
            action[5] = "凝神血咒";
            action[6] = "0";
            action[7] = "兽神_横行";
            action[8] = "狐妖_固元术";
            action[9] = "0";
            action[10] = "怀光_偷袭";
            action[11] = "0";
            action[12] = "天华_春雷";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_枯木咒";
            action[16] = "青云门_骑乘_炼气还神";
            action[17] = "鬼王宗_骑乘_枯木咒";
            action[18] = "合欢派_骑乘_激励";
            action[19] = "天音寺_骑乘_易筋经";
            action[20] = "凝神血咒";
            action[21] = "0";
            action[22] = "兽神_骑乘_横行";
            action[23] = "狐妖_骑乘_固元术";
            action[24] = "0";
            action[25] = "怀光_骑乘_偷袭";
            action[26] = "0";
            action[27] = "天华_骑乘_春雷";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击2";
            rangetype = 5;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 7;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3283Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (27);
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
            return 6000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (3600000 * 8);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (3600000 * 8);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
#endif
    };
}
#endif
