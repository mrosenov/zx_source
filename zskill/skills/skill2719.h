#ifndef __CPPGEN_GNET_SKILL2719
#define __CPPGEN_GNET_SKILL2719
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2719:public Skill
    {
      public:
        enum
        { SKILL_ID = 2719 };
          Skill2719 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2719Stub:public SkillStub
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
      Skill2719Stub ():SkillStub (2719)
        {
            occupation = 151;
            name = L"ºÆÔªÕòÆÇ";
            nativename = "ºÆÔªÕòÆÇ";
            icon = "Í¨ÓÃ03.dds";
            maxlevel = 5;
            maxlearn = 1;
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
            action[0] = "¹íÍõ×Ú_»¢Ð¥";
            action[1] = "ÇàÔÆÃÅ_»¢Ð¥";
            action[2] = "¹íÍõ×Ú_»¢Ð¥";
            action[3] = "ºÏ»¶ÅÉ_»¢Ð¥";
            action[4] = "ÌìÒôËÂ_»¢Ð¥";
            action[5] = "¹íµÀ_»¢Ð¥";
            action[6] = "¹íÍõ×Ú_»¢Ð¥";
            action[7] = "ÊÞÉñ_ºáÐÐ";
            action[8] = "ºüÑý_·çÐÐÕß";
            action[9] = "";
            action[10] = "0";
            action[11] = "ÊÞÉñ_ºáÐÐ";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»¢Ð¥";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_»¢Ð¥";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»¢Ð¥";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»¢Ð¥";
            action[19] = "ÌìÒôËÂ_Æï³Ë_»¢Ð¥";
            action[20] = "¹íµÀ_Æï³Ë_»¢Ð¥";
            action[21] = "¹íÍõ×Ú_Æï³Ë_»¢Ð¥";
            action[22] = "ÊÞÉñ_Æï³Ë_ºáÐÐ";
            action[23] = "ºüÑý_Æï³Ë_·çÐÐÕß";
            action[24] = "";
            action[25] = "0";
            action[26] = "ÊÞÉñ_Æï³Ë_ºáÐÐ";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill2719Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (300);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000;
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
            return _snwprintf (buffer, length, format, 300);

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
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (300);
            skill->GetVictim ()->SetAddantidizzy (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (300);
            skill->GetVictim ()->SetAddantisleep (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (300);
            skill->GetVictim ()->SetAddantisilent (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (300);
            skill->GetVictim ()->SetAddantiwrap (1);
            return true;
        }
#endif
    };
}
#endif
