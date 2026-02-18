#ifndef __CPPGEN_GNET_SKILL4729
#define __CPPGEN_GNET_SKILL4729
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4729:public Skill
    {
      public:
        enum
        { SKILL_ID = 4729 };
          Skill4729 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4729Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 9800;
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
      Skill4729Stub ():SkillStub (4729)
        {
            occupation = 129;
            name = L"2013´º½ÚÑÌ»¨";
            nativename = "2013´º½ÚÑÌ»¨";
            icon = "";
            maxlevel = 1;
            maxlearn = 10;
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
            action[0] = "ÑÌ»¨_¿Õ";
            action[1] = "ÑÌ»¨_½£";
            action[2] = "ÑÌ»¨_µ¶";
            action[3] = "ÑÌ»¨_ÂÖ";
            action[4] = "ÑÌ»¨_ÕÈ";
            action[5] = "ÑÌ»¨_×¦";
            action[6] = "ÑÌ»¨_ÈÐ";
            action[7] = "ÑÌ»¨_¸«";
            action[8] = "ÑÌ»¨_¹­";
            action[9] = "ÑÌ»¨_Æ÷";
            action[10] = "ÑÌ»¨_Á­";
            action[11] = "ÑÌ»¨_È­";
            action[12] = "ÑÌ»¨_ÇÙ";
            action[13] = "";
            action[14] = "";
            action[15] = "ÑÌ»¨_Æï³Ë_¿Õ";
            action[16] = "ÑÌ»¨_Æï³Ë_½£";
            action[17] = "ÑÌ»¨_Æï³Ë_µ¶";
            action[18] = "ÑÌ»¨_Æï³Ë_ÂÖ";
            action[19] = "ÑÌ»¨_Æï³Ë_ÕÈ";
            action[20] = "ÑÌ»¨_Æï³Ë_×¦";
            action[21] = "ÑÌ»¨_Æï³Ë_ÈÐ";
            action[22] = "ÑÌ»¨_Æï³Ë_¸«";
            action[23] = "ÑÌ»¨_Æï³Ë_¹­";
            action[24] = "ÑÌ»¨_Æï³Ë_Æ÷";
            action[25] = "ÑÌ»¨_Æï³Ë_Á­";
            action[26] = "ÑÌ»¨_Æï³Ë_È­";
            action[27] = "ÑÌ»¨_Æï³Ë_ÇÙ";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
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
        virtual ~ Skill4729Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 10000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
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
#endif
    };
}
#endif
