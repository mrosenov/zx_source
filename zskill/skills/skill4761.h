#ifndef __CPPGEN_GNET_SKILL4761
#define __CPPGEN_GNET_SKILL4761
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4761:public Skill
    {
      public:
        enum
        { SKILL_ID = 4761 };
          Skill4761 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4761Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4761Stub ():SkillStub (4761)
        {
            occupation = 128;
            name = L"Ó¢ÁÒÒ»»÷";
            nativename = "Ó¢ÁÒÒ»»÷";
            icon = "Ò»»÷.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            action[0] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[1] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[2] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[3] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[4] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[5] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[6] = "¹íÍõ×Ú_Ğé¿ÕÕ¶";
            action[7] = "ÊŞÉñ_ºáĞĞ";
            action[8] = "ºüÑı_¹ÌÔªÊõ";
            action[9] = "ĞùÔ¯_·Éµ¯";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "Ì«ê»_¼ÓËÙ";
            action[12] = "Ìì»ª_³õÔªÒôÈĞ";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[21] = "¹íÍõ×Ú_Æï³Ë_Ğé¿ÕÕ¶";
            action[22] = "ÊŞÉñ_Æï³Ë_ºáĞĞ";
            action[23] = "ºüÑı_Æï³Ë_¹ÌÔªÊõ";
            action[24] = "ĞùÔ¯_Æï³Ë_·Éµ¯";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "Ì«ê»_Æï³Ë_¼ÓËÙ";
            action[27] = "Ìì»ª_Æï³Ë_³õÔªÒôÈĞ";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 57887;
            item1_num = 5;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 14;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4761Stub ()
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
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 10000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetTmplid () == 56038 || skill->GetPlayer ()->GetTmplid () == 56041
                                                   || skill->GetPlayer ()->GetTmplid () == 56042) ? 100 : 0));
            skill->GetVictim ()->SetValue (30);
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
