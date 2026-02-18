#ifndef __CPPGEN_GNET_SKILL1555
#define __CPPGEN_GNET_SKILL1555
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1555:public Skill
    {
      public:
        enum
        { SKILL_ID = 1555 };
          Skill1555 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1555Stub:public SkillStub
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1555Stub ():SkillStub (1555)
        {
            occupation = 149;
            name = L"ÄæÈĞ";
            nativename = "ÄæÈĞ";
            icon = "ÄæÈĞ.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
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
            action[0] = "ÇàÔÆÃÅ_ÄæÈĞ";
            action[1] = "ÇàÔÆÃÅ_ÄæÈĞ";
            action[2] = "¹íÍõ×Ú_ÄæÈĞ";
            action[3] = "ºÏ»¶ÅÉ_ÄæÈĞ";
            action[4] = "ÌìÒôËÂ_ÄæÈĞ";
            action[5] = "¹íµÀ_ÄæÈĞ";
            action[6] = "·ÙÏã¹È_ÄæÈĞ";
            action[7] = "ÊŞÉñ_ÄæÈĞ";
            action[8] = "ºüÑı_ÄæÈĞ";
            action[9] = "ĞùÔ¯_ÄæÈĞ";
            action[10] = "»³¹â_ÄæÈĞ";
            action[11] = "Ì«ê»_ÄæÈĞ";
            action[12] = "Ìì»ª_ÄæÈĞ";
            action[13] = "ÁéÙí_ÄæÈĞ";
            action[14] = "Ó¢ÕĞ_ÄæÈĞ";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÄæÈĞ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÄæÈĞ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÄæÈĞ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÄæÈĞ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÄæÈĞ";
            action[20] = "¹íµÀ_Æï³Ë_ÄæÈĞ";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÄæÈĞ";
            action[22] = "ÊŞÉñ_Æï³Ë_ÄæÈĞ";
            action[23] = "ºüÑı_Æï³Ë_ÄæÈĞ";
            action[24] = "ĞùÔ¯_Æï³Ë_ÄæÈĞ";
            action[25] = "»³¹â_Æï³Ë_ÄæÈĞ";
            action[26] = "Ì«ê»_Æï³Ë_ÄæÈĞ";
            action[27] = "Ìì»ª_Æï³Ë_ÄæÈĞ";
            action[28] = "ÁéÙí_Æï³Ë_ÄæÈĞ";
            action[29] = "Ó¢ÕĞ_Æï³Ë_ÄæÈĞ";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill1555Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
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
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2525 + 35 * skill->GetLevel ());
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
            return (float) (12);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 2525 + 35 * skill->GetLevel (), 30, 2 * skill->GetLevel (), 2 * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetLevel () * 10);
            skill->GetVictim ()->SetValue (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetLevel () * 10);
            skill->GetVictim ()->SetValue (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetSubmp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (30);
        }
#endif
    };
}
#endif
