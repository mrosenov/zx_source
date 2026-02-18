#ifndef __CPPGEN_GNET_SKILL3907
#define __CPPGEN_GNET_SKILL3907
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3907:public Skill
    {
      public:
        enum
        { SKILL_ID = 3907 };
          Skill3907 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3907Stub:public SkillStub
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
      Skill3907Stub ():SkillStub (3907)
        {
            occupation = 151;
            name = L"ĞÇÓï¡¤ÕòÁé";
            nativename = "ĞÇÓï¡¤ÕòÁé";
            icon = "ÉñÍ¨_ÕòÁé.dds";
            maxlevel = 1;
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
            action[0] = "¹íÍõ×Ú_È¼ÉÕ";
            action[1] = "ÇàÔÆÃÅ_È¼ÉÕ";
            action[2] = "¹íÍõ×Ú_È¼ÉÕ";
            action[3] = "ºÏ»¶ÅÉ_È¼ÉÕ";
            action[4] = "ÌìÒôËÂ_È¼ÉÕ";
            action[5] = "¹íµÀ_È¼ÉÕ";
            action[6] = "·ÙÏã¹È_È¼ÉÕ";
            action[7] = "ÊŞÉñ_È¼ÉÕ";
            action[8] = "ºüÑı_È¼ÉÕ";
            action[9] = "ĞùÔ¯_È¼ÉÕ";
            action[10] = "»³¹â_È¼ÉÕ";
            action[11] = "Ì«ê»_È¼ÉÕ";
            action[12] = "Ìì»ª_È¼ÉÕ";
            action[13] = "ÁéÙí_È¼ÉÕ";
            action[14] = "Ó¢ÕĞ_È¼ÉÕ";
            action[15] = "¹íÍõ×Ú_Æï³Ë_È¼ÉÕ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_È¼ÉÕ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_È¼ÉÕ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_È¼ÉÕ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_È¼ÉÕ";
            action[20] = "¹íµÀ_Æï³Ë_È¼ÉÕ";
            action[21] = "·ÙÏã¹È_Æï³Ë_È¼ÉÕ";
            action[22] = "ÊŞÉñ_Æï³Ë_È¼ÉÕ";
            action[23] = "ºüÑı_Æï³Ë_È¼ÉÕ";
            action[24] = "ĞùÔ¯_Æï³Ë_È¼ÉÕ";
            action[25] = "»³¹â_Æï³Ë_È¼ÉÕ";
            action[26] = "Ì«ê»_Æï³Ë_È¼ÉÕ";
            action[27] = "Ìì»ª_Æï³Ë_È¼ÉÕ";
            action[28] = "ÁéÙí_Æï³Ë_È¼ÉÕ";
            action[29] = "Ó¢ÕĞ_Æï³Ë_È¼ÉÕ";
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
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3907Stub ()
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
            return 360 * 1000;
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
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (0.4);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetSlow (1);
            return true;
        }
#endif
    };
}
#endif
