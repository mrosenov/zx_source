#ifndef __CPPGEN_GNET_SKILL2033
#define __CPPGEN_GNET_SKILL2033
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2033:public Skill
    {
      public:
        enum
        { SKILL_ID = 2033 };
          Skill2033 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2033Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2033Stub ():SkillStub (2033)
        {
            occupation = 129;
            name = L"飞龙在天";
            nativename = "飞龙在天";
            icon = "毁天灭地.dds";
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
            action[0] = "鬼王宗_毁天灭地";
            action[1] = "鬼王宗_毁天灭地";
            action[2] = "鬼王宗_毁天灭地";
            action[3] = "鬼王宗_毁天灭地";
            action[4] = "鬼王宗_毁天灭地";
            action[5] = "鬼王宗_毁天灭地";
            action[6] = "0";
            action[7] = "兽神_龙光";
            action[8] = "狐妖_安如山";
            action[9] = "轩辕_暗能环";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "灵夙_猛击";
            action[14] = "英招_猛击";
            action[15] = "鬼王宗_骑乘_毁天灭地";
            action[16] = "鬼王宗_骑乘_毁天灭地";
            action[17] = "鬼王宗_骑乘_毁天灭地";
            action[18] = "鬼王宗_骑乘_毁天灭地";
            action[19] = "鬼王宗_骑乘_毁天灭地";
            action[20] = "鬼王宗_骑乘_毁天灭地";
            action[21] = "0";
            action[22] = "兽神_骑乘_龙光";
            action[23] = "狐妖_骑乘_安如山";
            action[24] = "轩辕_骑乘_暗能环";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "灵夙_骑乘_猛击";
            action[29] = "英招_骑乘_猛击";
            action[30] = "0";
            rangetype = 2;
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
        virtual ~ Skill2033Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
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
            return 15 * 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
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
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1000);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    1 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    2 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    2 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () != 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetHp () <
                            skill->GetPlayer ()->GetVar1 () * 15) ? (skill->GetPlayer ()->GetHp () + 1) : (skill->GetPlayer ()->GetVar1 () * 15));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
