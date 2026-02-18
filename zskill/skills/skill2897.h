#ifndef __CPPGEN_GNET_SKILL2897
#define __CPPGEN_GNET_SKILL2897
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2897:public Skill
    {
      public:
        enum
        { SKILL_ID = 2897 };
          Skill2897 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2897Stub:public SkillStub
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
      Skill2897Stub ():SkillStub (2897)
        {
            occupation = 129;
            name = L"Óù¼ÝÇ×Õ÷¡¤ïþ";
            nativename = "Óù¼ÝÇ×Õ÷¡¤ïþ";
            icon = "Óù¼ÝÇ×Õ÷¡¤ïþ.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 5;
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
            action[0] = "¹íÍõ×Ú_¼á¶Ü";
            action[1] = "ÇàÔÆÃÅ_¼á¶Ü";
            action[2] = "¹íÍõ×Ú_¼á¶Ü";
            action[3] = "ºÏ»¶ÅÉ_¼á¶Ü";
            action[4] = "ÌìÒôËÂ_¼á¶Ü";
            action[5] = "¹íµÀ_¼á¶Ü";
            action[6] = "0";
            action[7] = "ÊÞÉñ_¼á¶Ü";
            action[8] = "ºüÑý_¼á¶Ü";
            action[9] = "0";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "0";
            action[12] = "Ìì»ª_´ºÀ×";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¼á¶Ü";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¼á¶Ü";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼á¶Ü";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼á¶Ü";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¼á¶Ü";
            action[20] = "¹íµÀ_Æï³Ë_¼á¶Ü";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_¼á¶Ü";
            action[23] = "ºüÑý_Æï³Ë_¼á¶Ü";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_´ºÀ×";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷2";
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
            skill_limit = 1;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2897Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (35);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (3000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 480000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (30);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (32);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (30);
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
            return _snwprintf (buffer, length, format, 3000);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () == 0 ? 100 : 0));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetType () == 0 ? 30000 : 0);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (12);
            skill->GetVictim ()->SetValue (150);
            skill->GetVictim ()->SetBloodshield (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (12);
            skill->GetVictim ()->SetValue (150);
            skill->GetVictim ()->SetBloodshield (1);
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
