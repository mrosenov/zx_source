#ifndef __CPPGEN_GNET_SKILL1548
#define __CPPGEN_GNET_SKILL1548
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1548:public Skill
    {
      public:
        enum
        { SKILL_ID = 1548 };
          Skill1548 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1548Stub:public SkillStub
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
      Skill1548Stub ():SkillStub (1548)
        {
            occupation = 149;
            name = L"¶Ý¼×";
            nativename = "¶Ý¼×";
            icon = "¶Ý¼×.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 2;
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
            preskillnum = 1;
            talent[0] = 1549;
            talent[1] = 1552;
            talent_size = 2;
            action[0] = "ÇàÔÆÃÅ_¶Ý¼×";
            action[1] = "ÇàÔÆÃÅ_¶Ý¼×";
            action[2] = "¹íÍõ×Ú_¶Ý¼×";
            action[3] = "ºÏ»¶ÅÉ_¶Ý¼×";
            action[4] = "ÌìÒôËÂ_¶Ý¼×";
            action[5] = "¹íµÀ_¶Ý¼×";
            action[6] = "·ÙÏã¹È_¶Ý¼×";
            action[7] = "ÊÞÉñ_¶Ý¼×";
            action[8] = "ºüÑý_¶Ý¼×";
            action[9] = "ÐùÔ¯_¶Ý¼×";
            action[10] = "»³¹â_¶Ý¼×";
            action[11] = "Ì«ê»_¶Ý¼×";
            action[12] = "Ìì»ª_¶Ý¼×";
            action[13] = "ÁéÙí_¶Ý¼×";
            action[14] = "Ó¢ÕÐ_¶Ý¼×";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¶Ý¼×";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¶Ý¼×";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¶Ý¼×";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¶Ý¼×";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¶Ý¼×";
            action[20] = "¹íµÀ_Æï³Ë_¶Ý¼×";
            action[21] = "·ÙÏã¹È_Æï³Ë_¶Ý¼×";
            action[22] = "ÊÞÉñ_Æï³Ë_¶Ý¼×";
            action[23] = "ºüÑý_Æï³Ë_¶Ý¼×";
            action[24] = "ÐùÔ¯_Æï³Ë_¶Ý¼×";
            action[25] = "»³¹â_Æï³Ë_¶Ý¼×";
            action[26] = "Ì«ê»_Æï³Ë_¶Ý¼×";
            action[27] = "Ìì»ª_Æï³Ë_¶Ý¼×";
            action[28] = "ÁéÙí_Æï³Ë_¶Ý¼×";
            action[29] = "Ó¢ÕÐ_Æï³Ë_¶Ý¼×";
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
        virtual ~ Skill1548Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1549 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (325 + 25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 720000 - 36000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
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
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 325 + 25 * skill->GetLevel (), 30, 15 * skill->GetLevel (), 30, skill->GetLevel (), 12);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000 + 3000 * skill->GetT0 () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (30 + 3 * skill->GetT0 ());
            skill->GetVictim ()->SetValue (15 * skill->GetLevel ());
            skill->GetVictim ()->SetBloodshield (1);
            skill->GetVictim ()->SetTime (30000 + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (1000000);
            skill->GetVictim ()->SetDechurt (1);
            return true;
        }
#endif
    };
}
#endif
