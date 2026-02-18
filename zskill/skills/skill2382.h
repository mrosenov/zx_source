#ifndef __CPPGEN_GNET_SKILL2382
#define __CPPGEN_GNET_SKILL2382
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2382:public Skill
    {
      public:
        enum
        { SKILL_ID = 2382 };
          Skill2382 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2382Stub:public SkillStub
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
      Skill2382Stub ():SkillStub (2382)
        {
            occupation = 128;
            name = L"À¦ÏÉ¸¿Áé";
            nativename = "À¦ÏÉ¸¿Áé";
            icon = "À¦ÏÉ¸¿Áé.dds";
            maxlevel = 99;
            maxlearn = 0;
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
            action[0] = "ÇàÔÆÃÅ_ÃÍ»÷";
            action[1] = "ÇàÔÆÃÅ_ÃÍ»÷";
            action[2] = "¹íÍõ×Ú_ÃÍ»÷";
            action[3] = "ºÏ»¶ÅÉ_ÃÍ»÷";
            action[4] = "ÌìÒôËÂ_ÃÍ»÷";
            action[5] = "¹íµÀ_ÃÍ»÷";
            action[6] = "ÇàÔÆÃÅ_ÃÍ»÷";
            action[7] = "ÊŞÉñ_ÃÍ»÷";
            action[8] = "ºüÑı_ÃÍ»÷";
            action[9] = "0";
            action[10] = "»³¹â_ÃÍ»÷";
            action[11] = "ÊŞÉñ_ÃÍ»÷";
            action[12] = "Ìì»ª_ÃÍ»÷";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÃÍ»÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÃÍ»÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»÷";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÍ»÷";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÃÍ»÷";
            action[20] = "¹íµÀ_Æï³Ë_ÃÍ»÷";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_ÃÍ»÷";
            action[22] = "ÊŞÉñ_Æï³Ë_ÃÍ»÷";
            action[23] = "ºüÑı_Æï³Ë_ÃÍ»÷";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_ÃÍ»÷";
            action[26] = "ÊŞÉñ_Æï³Ë_ÃÍ»÷";
            action[27] = "Ìì»ª_Æï³Ë_ÃÍ»÷";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 10;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2382Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (44);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (22);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 8000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (42);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (40);
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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((33294 <= skill->GetPlayer ()->GetTmplid ()
                                                   && skill->GetPlayer ()->GetTmplid () <=
                                                   33297) ? ((skill->GetPlayer ()->GetLevel () - 175) * 15 + 5) : 0));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () + zrand (3000));
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
