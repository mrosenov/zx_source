#ifndef __CPPGEN_GNET_SKILL3062
#define __CPPGEN_GNET_SKILL3062
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3062:public Skill
    {
      public:
        enum
        { SKILL_ID = 3062 };
          Skill3062 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3062Stub:public SkillStub
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
      Skill3062Stub ():SkillStub (3062)
        {
            occupation = 168;
            name = L"¼À»ê<É·>";
            nativename = "¼À»ê<É·>";
            icon = "Ä§¡¤¼À»ê.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
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
            action[0] = "ÇàÔÆÃÅ_¼À»ê";
            action[1] = "ÇàÔÆÃÅ_¼À»ê";
            action[2] = "¹íÍõ×Ú_¼À»ê";
            action[3] = "ºÏ»¶ÅÉ_¼À»ê";
            action[4] = "ÌìÒôËÂ_¼À»ê";
            action[5] = "¹íµÀ_¼À»ê";
            action[6] = "·ÙÏã¹È_¼À»ê";
            action[7] = "ÊÞÉñ_¼À»ê";
            action[8] = "ºüÑý_¼À»ê";
            action[9] = "ÐùÔ¯_¼À»ê";
            action[10] = "»³¹â_¼À»ê";
            action[11] = "Ì«ê»_¼À»ê";
            action[12] = "Ìì»ª_¼À»ê";
            action[13] = "ÁéÙí_¼À»ê";
            action[14] = "Ó¢ÕÐ_¼À»ê";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¼À»ê";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¼À»ê";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¼À»ê";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼À»ê";
            action[19] = "ÌìÒôËÂ_Æï³Ë_¼À»ê";
            action[20] = "¹íµÀ_Æï³Ë_¼À»ê";
            action[21] = "·ÙÏã¹È_Æï³Ë_¼À»ê";
            action[22] = "ÊÞÉñ_Æï³Ë_¼À»ê";
            action[23] = "ºüÑý_Æï³Ë_¼À»ê";
            action[24] = "ÐùÔ¯_Æï³Ë_¼À»ê";
            action[25] = "»³¹â_Æï³Ë_¼À»ê";
            action[26] = "Ì«ê»_Æï³Ë_¼À»ê";
            action[27] = "Ìì»ª_Æï³Ë_¼À»ê";
            action[28] = "ÁéÙí_Æï³Ë_¼À»ê";
            action[29] = "Ó¢ÕÐ_Æï³Ë_¼À»ê";
            action[30] = "0";
            rangetype = 2;
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
        virtual ~ Skill3062Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1546 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1525 + 25 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (30);
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
            return _snwprintf (buffer, length, format,
                               1525 + 25 * skill->GetLevel (),
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               600 - 20 * skill->GetLevel (),
                               INT (4 + INT (skill->GetLevel () / 4.9)),
                               2 + 0.5 * skill->GetLevel (), 20 + 3 * skill->GetLevel (), 15 + 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (zrand (100) < 20 + 3 * skill->GetLevel ()? 900 : 0));
            skill->GetVictim ()->SetTime (2000 + 500 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (0.15 + 0.04 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetTime (2000 + 500 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetDisperseanti (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (4 + INT (skill->GetLevel () / 4.9));
        }
#endif
    };
}
#endif
