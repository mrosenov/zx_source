#ifndef __CPPGEN_GNET_SKILL2988
#define __CPPGEN_GNET_SKILL2988
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2988:public Skill
    {
      public:
        enum
        { SKILL_ID = 2988 };
          Skill2988 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2988Stub:public SkillStub
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
      Skill2988Stub ():SkillStub (2988)
        {
            occupation = 152;
            name = L"´ò»ØÔ­ÐÎ";
            nativename = "´ò»ØÔ­ÐÎ";
            icon = "";
            maxlevel = 4;
            maxlearn = 4;
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
            action[0] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[1] = "ÇàÔÆÃÅ_Ç¬À¤Õ¶";
            action[2] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[3] = "ºÏ»¶ÅÉ_Ç¬À¤Õ¶";
            action[4] = "ÌìÒôËÂ_Ç¬À¤Õ¶";
            action[5] = "0";
            action[6] = "¹íµÀ_Ç¬À¤Õ¶";
            action[7] = "ÊÞÉñ_ºáÐÐ";
            action[8] = "ºüÑý_¹ÌÔªÊõ";
            action[9] = "0";
            action[10] = "»³¹â_ÍµÏ®";
            action[11] = "ÊÞÉñ_ºáÐÐ";
            action[12] = "Ìì»ª_³õÔªÒôÈÐ";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_Ç¬À¤Õ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ç¬À¤Õ¶";
            action[19] = "ÌìÒôËÂ_Æï³Ë_Ç¬À¤Õ¶";
            action[20] = "0";
            action[21] = "¹íµÀ_Æï³Ë_Ç¬À¤Õ¶";
            action[22] = "ÊÞÉñ_Æï³Ë_ºáÐÐ";
            action[23] = "ºüÑý_Æï³Ë_¹ÌÔªÊõ";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_ÍµÏ®";
            action[26] = "ÊÞÉñ_Æï³Ë_ºáÐÐ";
            action[27] = "Ìì»ª_Æï³Ë_³õÔªÒôÈÐ";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 8;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 7;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2988Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
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
            return 0;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (4);
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
            skill->GetVictim ()->SetRatio (419);
            skill->GetVictim ()->SetAmount (420);
            skill->GetVictim ()->SetValue (419);
            skill->GetVictim ()->SetCleartransform (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetRatio (421);
            skill->GetVictim ()->SetAmount (422);
            skill->GetVictim ()->SetValue (421);
            skill->GetVictim ()->SetCleartransform (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetValue (418 + skill->GetLevel ());
            skill->GetVictim ()->SetTransform (1);
            return true;
        }
#endif
    };
}
#endif
