#ifndef __CPPGEN_GNET_SKILL3908
#define __CPPGEN_GNET_SKILL3908
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3908:public Skill
    {
      public:
        enum
        { SKILL_ID = 3908 };
          Skill3908 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3908Stub:public SkillStub
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
      Skill3908Stub ():SkillStub (3908)
        {
            occupation = 151;
            name = L"星语・腾云";
            nativename = "星语・腾云";
            icon = "神通_腾云.dds";
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
            action[0] = "鬼王宗_狂风咒";
            action[1] = "青云门_逍遥游";
            action[2] = "鬼王宗_狂风咒";
            action[3] = "合欢派_浮光";
            action[4] = "天音寺_易筋经";
            action[5] = "鬼道_百鬼夜行";
            action[6] = "青云门_逍遥游";
            action[7] = "兽神_横行";
            action[8] = "狐妖_风行者";
            action[9] = "轩辕_加速";
            action[10] = "怀光_加速";
            action[11] = "太昊_加速";
            action[12] = "天华_奔雷";
            action[13] = "灵夙_加速";
            action[14] = "英招_加速";
            action[15] = "鬼王宗_骑乘_狂风咒";
            action[16] = "青云门_骑乘_逍遥游";
            action[17] = "鬼王宗_骑乘_狂风咒";
            action[18] = "合欢派_骑乘_浮光";
            action[19] = "天音寺_骑乘_易筋经";
            action[20] = "鬼道_骑乘_百鬼夜行";
            action[21] = "青云门_骑乘_逍遥游";
            action[22] = "兽神_骑乘_横行";
            action[23] = "狐妖_骑乘_风行者";
            action[24] = "轩辕_骑乘_加速";
            action[25] = "怀光_骑乘_加速";
            action[26] = "太昊_骑乘_加速";
            action[27] = "天华_骑乘_奔雷";
            action[28] = "灵夙_骑乘_加速";
            action[29] = "英招_骑乘_加速";
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
        virtual ~ Skill3908Stub ()
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
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300 * 1000;
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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (5);
            skill->GetVictim ()->SetAddspeed (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (4);
            skill->GetVictim ()->SetIncmountspeed (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetClearslow (1);
            return true;
        }
#endif
    };
}
#endif
