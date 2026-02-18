#ifndef __CPPGEN_GNET_SKILL2362
#define __CPPGEN_GNET_SKILL2362
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2362:public Skill
    {
      public:
        enum
        { SKILL_ID = 2362 };
          Skill2362 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2362Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1200;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2362Stub ():SkillStub (2362)
        {
            occupation = 151;
            name = L"雪孩子的祝福";
            nativename = "雪孩子的祝福";
            icon = "雪球.dds";
            maxlevel = 2;
            maxlearn = 1;
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
            action[0] = "鬼道_百鬼夜行";
            action[1] = "青云门_五气朝元";
            action[2] = "鬼王宗_锐金咒";
            action[3] = "合欢派_热情";
            action[4] = "天音寺_慈航普度";
            action[5] = "鬼道_百鬼夜行";
            action[6] = "青云门_五气朝元";
            action[7] = "兽神_横行";
            action[8] = "狐妖_风行者";
            action[9] = "兽神_横行";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼道_骑乘_百鬼夜行";
            action[16] = "青云门_骑乘_五气朝元";
            action[17] = "鬼王宗_骑乘_锐金咒";
            action[18] = "合欢派_骑乘_热情";
            action[19] = "天音寺_骑乘_慈航普度";
            action[20] = "鬼道_骑乘_百鬼夜行";
            action[21] = "青云门_骑乘_五气朝元";
            action[22] = "兽神_骑乘_横行";
            action[23] = "狐妖_骑乘_风行者";
            action[24] = "兽神_骑乘_横行";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击2";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 32934;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 23;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2362Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
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
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
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
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (50 + 1.0 * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetAddattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 1 ? 30 : 99));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetAmount (skill->GetLevel () == 1 ? 1 : (1 + zrand (2)));
            skill->GetVictim ()->SetValue (32934);
            skill->GetVictim ()->SetCreateitem (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 1 ? 120 : 0));
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((0.2 * skill->GetPlayer ()->GetLevel () + 5) * 1800);
            skill->GetVictim ()->SetHpgen (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (3600000);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((0.2 * skill->GetPlayer ()->GetLevel () + 5) * 1800);
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
#endif
    };
}
#endif
