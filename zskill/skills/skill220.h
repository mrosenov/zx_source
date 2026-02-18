#ifndef __CPPGEN_GNET_SKILL220
#define __CPPGEN_GNET_SKILL220
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill220:public Skill
    {
      public:
        enum
        { SKILL_ID = 220 };
          Skill220 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill220Stub:public SkillStub
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
      Skill220Stub ():SkillStub (220)
        {
            occupation = 0;
            name = L"ÓÂÆø";
            nativename = "ÓÂÆø";
            icon = "ÓÂÆø.dds";
            maxlevel = 15;
            maxlearn = 6;
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
            skill_class = -1;
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
            preskillnum = 1;
            talent[0] = 636;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_»¢Ð¥";
            action[1] = "ÇàÔÆÃÅ_»¢Ð¥";
            action[2] = "¹íÍõ×Ú_»¢Ð¥";
            action[3] = "ºÏ»¶ÅÉ_»¢Ð¥";
            action[4] = "ÌìÒôËÂ_»¢Ð¥";
            action[5] = "¹íÍõ×Ú_»¢Ð¥";
            action[6] = "ÇàÔÆÃÅ_»¢Ð¥";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»¢Ð¥";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_»¢Ð¥";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»¢Ð¥";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_»¢Ð¥";
            action[19] = "ÌìÒôËÂ_Æï³Ë_»¢Ð¥";
            action[20] = "¹íÍõ×Ú_Æï³Ë_»¢Ð¥";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_»¢Ð¥";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill220Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 218 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.25 * skill->GetLevel () * skill->GetLevel () + 1.6 * skill->GetLevel () + 17.5);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 4, 7, 9, 11, 13, 15 };
            return array[level - 1];
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
            return _snwprintf (buffer, length, format,
                               INT (0.25 * skill->GetLevel () * skill->GetLevel () + 1.6 * skill->GetLevel () + 17.5), 1 + 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1800000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetValue (1 + 4 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetAddattack2 (1);
            return true;
        }
#endif
    };
}
#endif
