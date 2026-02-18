#ifndef __CPPGEN_GNET_SKILL2375
#define __CPPGEN_GNET_SKILL2375
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2375:public Skill
    {
      public:
        enum
        { SKILL_ID = 2375 };
          Skill2375 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2375Stub:public SkillStub
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
      Skill2375Stub ():SkillStub (2375)
        {
            occupation = 151;
            name = L"九华淀魂曲";
            nativename = "九华淀魂曲";
            icon = "帝俊.dds";
            maxlevel = 2;
            maxlearn = 10;
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
            action[0] = "青云门_炼气还神";
            action[1] = "青云门_炼气还神";
            action[2] = "鬼王宗_枯木咒";
            action[3] = "合欢派_激励";
            action[4] = "天音寺_易筋经";
            action[5] = "凝神血咒";
            action[6] = "0";
            action[7] = "兽神_横行";
            action[8] = "狐妖_固元术";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "灵夙_坚盾";
            action[14] = "英招_坚盾";
            action[15] = "青云门_骑乘_炼气还神";
            action[16] = "青云门_骑乘_炼气还神";
            action[17] = "鬼王宗_骑乘_枯木咒";
            action[18] = "合欢派_骑乘_激励";
            action[19] = "天音寺_骑乘_易筋经";
            action[20] = "凝神血咒";
            action[21] = "0";
            action[22] = "兽神_骑乘_横行";
            action[23] = "狐妖_骑乘_固元术";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "灵夙_骑乘_坚盾";
            action[29] = "英招_骑乘_坚盾";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 33142;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2375Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 1000 * (840 - 240 * skill->GetLevel ());
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
            return _snwprintf (buffer, length, format, 840 - 240 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (20000);
            skill->GetVictim ()->SetRatio (1.2);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetInccrithurt (1);
            return true;
        }
#endif
    };
}
#endif
