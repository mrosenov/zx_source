#ifndef __CPPGEN_GNET_SKILL1944
#define __CPPGEN_GNET_SKILL1944
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1944:public Skill
    {
      public:
        enum
        { SKILL_ID = 1944 };
          Skill1944 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1944Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1800;
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
                skill->SetRatio (2.2);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1944Stub ():SkillStub (1944)
        {
            occupation = 151;
            name = L"º¬ÕÂ";
            nativename = "º¬ÕÂ";
            icon = "Ñ©ç÷Ö®±ùËª.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 7;
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
            action[0] = "¹íÍõ×Ú_Ñ©ç÷Ö®±ùËª";
            action[1] = "ÇàÔÆÃÅ_Ñ©ç÷Ö®±ùËª";
            action[2] = "¹íÍõ×Ú_Ñ©ç÷Ö®±ùËª";
            action[3] = "ºÏ»¶ÅÉ_Ñ©ç÷Ö®±ùËª";
            action[4] = "ÌìÒôËÂ_Ñ©ç÷Ö®±ùËª";
            action[5] = "¹íµÀ_Ñ©ç÷Ö®±ùËª";
            action[6] = "·ÙÏã_Ñ©ç÷Ö®±ùËª";
            action[7] = "ÊŞÉñ_Ñ©ç÷Ö®±ùËª";
            action[8] = "ºüÑı_Ñ©ç÷Ö®±ùËª";
            action[9] = "ĞùÔ¯_Ñ©ç÷Ö®±ùËª";
            action[10] = "»³¹â_Ñ©ç÷Ö®±ùËª";
            action[11] = "Ì«ê»_Ñ©ç÷Ö®±ùËª";
            action[12] = "Ìì»ª_Ñ©ç÷Ö®±ùËª";
            action[13] = "ÁéÙí_Ñ©ç÷Ö®±ùËª";
            action[14] = "Ó¢ÕĞ_Ñ©ç÷Ö®±ùËª";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[20] = "¹íµÀ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[21] = "·ÙÏã_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[22] = "ÊŞÉñ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[23] = "ºüÑı_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[24] = "ĞùÔ¯_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[25] = "»³¹â_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[26] = "Ì«ê»_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[27] = "Ìì»ª_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[28] = "ÁéÙí_Æï³Ë_Ñ©ç÷Ö®±ùËª";
            action[29] = "Ó¢ÕĞ_Æï³Ë_Ñ©ç÷Ö®±ùËª";
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
            skill_limit = 32;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1944Stub ()
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
            return 90000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
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
            skill->GetVictim ()->SetProbability (1.0 * (600));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (15000);
            skill->GetVictim ()->SetRatio (0.99);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncskilldodge (1);
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
