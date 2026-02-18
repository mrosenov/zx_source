#ifndef __CPPGEN_GNET_SKILL1515
#define __CPPGEN_GNET_SKILL1515
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1515:public Skill
    {
      public:
        enum
        { SKILL_ID = 1515 };
          Skill1515 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1515Stub:public SkillStub
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
      Skill1515Stub ():SkillStub (1515)
        {
            occupation = 129;
            name = L"凡尘物华";
            nativename = "凡尘物华";
            icon = "回魂祭.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 3;
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
            action[0] = "鬼王宗_无间寂";
            action[1] = "鬼王宗_无间寂";
            action[2] = "鬼王宗_无间寂";
            action[3] = "鬼王宗_无间寂";
            action[4] = "鬼王宗_无间寂";
            action[5] = "鬼道_天外天";
            action[6] = "焚香谷_火狩护主";
            action[7] = "兽神_虎力";
            action[8] = "狐妖_安如山";
            action[9] = "轩辕_能环";
            action[10] = "怀光_偷袭";
            action[11] = "太昊_加速";
            action[12] = "天华_奔雷";
            action[13] = "灵夙_坚盾";
            action[14] = "英招_坚盾";
            action[15] = "鬼王宗_骑乘_无间寂";
            action[16] = "鬼王宗_骑乘_无间寂";
            action[17] = "鬼王宗_骑乘_无间寂";
            action[18] = "鬼王宗_骑乘_无间寂";
            action[19] = "鬼王宗_骑乘_无间寂";
            action[20] = "鬼道_骑乘_天外天";
            action[21] = "焚香谷_骑乘_火狩护主";
            action[22] = "兽神_骑乘_虎力";
            action[23] = "狐妖_骑乘_安如山";
            action[24] = "轩辕_骑乘_能环";
            action[25] = "怀光_骑乘_偷袭";
            action[26] = "太昊_骑乘_加速";
            action[27] = "天华_骑乘_奔雷";
            action[28] = "灵夙_骑乘_坚盾";
            action[29] = "英招_骑乘_坚盾";
            action[30] = "0";
            rangetype = 2;
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
        virtual ~ Skill1515Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
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
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (600), 25, 30, 30, 300);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDeccrithurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBarehanded (1);
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
