#ifndef __CPPGEN_GNET_SKILL3911
#define __CPPGEN_GNET_SKILL3911
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3911:public Skill
    {
      public:
        enum
        { SKILL_ID = 3911 };
          Skill3911 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3911Stub:public SkillStub
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
      Skill3911Stub ():SkillStub (3911)
        {
            occupation = 151;
            name = L"星语・不动明王";
            nativename = "星语・不动明王";
            icon = "神通_不动明王.dds";
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
            action[0] = "鬼王宗_坚盾";
            action[1] = "青云门_坚盾";
            action[2] = "鬼王宗_坚盾";
            action[3] = "合欢派_坚盾";
            action[4] = "天音寺_坚盾";
            action[5] = "鬼道_坚盾";
            action[6] = "焚香谷_坚盾";
            action[7] = "兽神_坚盾";
            action[8] = "狐妖_坚盾";
            action[9] = "轩辕_坚盾";
            action[10] = "怀光_坚盾";
            action[11] = "太昊_坚盾";
            action[12] = "天华_坚盾";
            action[13] = "灵夙_坚盾";
            action[14] = "英招_坚盾";
            action[15] = "鬼王宗_骑乘_坚盾";
            action[16] = "青云门_骑乘_坚盾";
            action[17] = "鬼王宗_骑乘_坚盾";
            action[18] = "合欢派_骑乘_坚盾";
            action[19] = "天音寺_骑乘_坚盾";
            action[20] = "鬼道_骑乘_坚盾";
            action[21] = "焚香谷_骑乘_坚盾";
            action[22] = "兽神_骑乘_坚盾";
            action[23] = "狐妖_骑乘_坚盾";
            action[24] = "轩辕_骑乘_坚盾";
            action[25] = "怀光_骑乘_坚盾";
            action[26] = "太昊_骑乘_坚盾";
            action[27] = "天华_骑乘_坚盾";
            action[28] = "灵夙_骑乘_坚盾";
            action[29] = "英招_骑乘_坚盾";
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
        virtual ~ Skill3911Stub ()
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
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetInvincible (1);
            return true;
        }
#endif
    };
}
#endif
