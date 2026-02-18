#ifndef __CPPGEN_GNET_SKILL2034
#define __CPPGEN_GNET_SKILL2034
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2034:public Skill
    {
      public:
        enum
        { SKILL_ID = 2034 };
          Skill2034 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2034Stub:public SkillStub
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
      Skill2034Stub ():SkillStub (2034)
        {
            occupation = 129;
            name = L"·ïÎè¾ÅÌì";
            nativename = "·ïÎè¾ÅÌì";
            icon = "Á¯Ó°.dds";
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
            action[0] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[1] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[2] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[3] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[4] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[5] = "ºÏ»¶ÅÉ_Á¯Ó°";
            action[6] = "0";
            action[7] = "ÊÞÉñ_Áú¹â";
            action[8] = "ºüÑý_°²ÈçÉ½";
            action[9] = "ÐùÔ¯_¹âÄÜ»·";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "ÁéÙí_ÃÍ»÷";
            action[14] = "Ó¢ÕÐ_ÃÍ»÷";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Á¯Ó°";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_Áú¹â";
            action[23] = "ºüÑý_Æï³Ë_°²ÈçÉ½";
            action[24] = "ÐùÔ¯_Æï³Ë_¹âÄÜ»·";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "ÁéÙí_Æï³Ë_ÃÍ»÷";
            action[29] = "Ó¢ÕÐ_Æï³Ë_ÃÍ»÷";
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
        virtual ~ Skill2034Stub ()
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
                                                                                                    2 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    1 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    1 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (2);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetIncskilldodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (600);
            skill->GetVictim ()->SetAdddodge (1);
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
