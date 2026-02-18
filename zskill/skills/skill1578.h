#ifndef __CPPGEN_GNET_SKILL1578
#define __CPPGEN_GNET_SKILL1578
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1578:public Skill
    {
      public:
        enum
        { SKILL_ID = 1578 };
          Skill1578 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1578Stub:public SkillStub
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
      Skill1578Stub ():SkillStub (1578)
        {
            occupation = 150;
            name = L"¿İÈÙÖ®Äî";
            nativename = "¿İÈÙÖ®Äî";
            icon = "¿İÈÙÖ®Äî.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 4;
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
            action[0] = "ÇàÔÆÃÅ_¿İÈÙÖ®Äî";
            action[1] = "ÇàÔÆÃÅ_¿İÈÙÖ®Äî";
            action[2] = "¹íÍõ×Ú_¿İÈÙÖ®Äî";
            action[3] = "ºÏ»¶ÅÉ_¿İÈÙÖ®Äî";
            action[4] = "ÌìÒôËÂ_¿İÈÙÖ®Äî";
            action[5] = "¹íµÀ_¿İÈÙÖ®Äî";
            action[6] = "·ÙÏã¹È_¿İÈÙÖ®Äî";
            action[7] = "ÊŞÉñ_¿İÈÙÖ®Äî";
            action[8] = "ºüÑı_¿İÈÙÖ®Äî";
            action[9] = "ĞùÔ¯_¿İÈÙÖ®Äî";
            action[10] = "»³¹â_¿İÈÙÖ®Äî";
            action[11] = "Ì«ê»_¿İÈÙÖ®Äî";
            action[12] = "Ìì»ª_¿İÈÙÖ®Äî";
            action[13] = "ÁéÙí_¿İÈÙÖ®Äî";
            action[14] = "Ó¢ÕĞ_¿İÈÙÖ®Äî";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¿İÈÙÖ®Äî";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÈÙÖ®Äî";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿İÈÙÖ®Äî";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¿İÈÙÖ®Äî";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¿İÈÙÖ®Äî";
            action[20] = "¹íµÀ_Æï³Ë_¿İÈÙÖ®Äî";
            action[21] = "·ÙÏã¹È_Æï³Ë_¿İÈÙÖ®Äî";
            action[22] = "ÊŞÉñ_Æï³Ë_¿İÈÙÖ®Äî";
            action[23] = "ºüÑı_Æï³Ë_¿İÈÙÖ®Äî";
            action[24] = "ĞùÔ¯_Æï³Ë_¿İÈÙÖ®Äî";
            action[25] = "»³¹â_Æï³Ë_¿İÈÙÖ®Äî";
            action[26] = "Ì«ê»_Æï³Ë_¿İÈÙÖ®Äî";
            action[27] = "Ìì»ª_Æï³Ë_¿İÈÙÖ®Äî";
            action[28] = "ÁéÙí_Æï³Ë_¿İÈÙÖ®Äî";
            action[29] = "Ó¢ÕĞ_Æï³Ë_¿İÈÙÖ®Äî";
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
        virtual ~ Skill1578Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1572 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1335 + 35 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000;
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
            return _snwprintf (buffer, length, format,
                               1335 + 35 * skill->GetLevel (),
                               30 + 6 * skill->GetLevel (), 20 + 8 * skill->GetLevel (), 79 + skill->GetLevel (), 5 + 4 * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (79 + skill->GetLevel ()));
            skill->GetVictim ()->SetTime (30000 + 6000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio ((0.20 + 0.08 * skill->GetLevel ()) > 1 ? 1 : (0.20 + 0.08 * skill->GetLevel ()));
            skill->GetVictim ()->SetAmount (5 + 4 * skill->GetLevel ());
            skill->GetVictim ()->SetDeadlybless (1);
            return true;
        }
#endif
    };
}
#endif
