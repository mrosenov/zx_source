#ifndef __CPPGEN_GNET_SKILL1529
#define __CPPGEN_GNET_SKILL1529
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1529:public Skill
    {
      public:
        enum
        { SKILL_ID = 1529 };
          Skill1529 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1529Stub:public SkillStub
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
      Skill1529Stub ():SkillStub (1529)
        {
            occupation = 148;
            name = L"ÀëºŞÌì";
            nativename = "ÀëºŞÌì";
            icon = "ÀëºŞÌì.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 9;
            clearmask = 1;
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
            talent[0] = 1545;
            talent_size = 1;
            action[0] = "ÇàÔÆÃÅ_ÀëºŞÌì";
            action[1] = "ÇàÔÆÃÅ_ÀëºŞÌì";
            action[2] = "¹íÍõ×Ú_ÀëºŞÌì";
            action[3] = "ºÏ»¶ÅÉ_ÀëºŞÌì";
            action[4] = "ÌìÒôËÂ_ÀëºŞÌì";
            action[5] = "¹íµÀ_ÀëºŞÌì";
            action[6] = "·ÙÏã¹È_ÀëºŞÌì";
            action[7] = "ÊŞÉñ_ÀëºŞÌì";
            action[8] = "ºüÑı_ÀëºŞÌì";
            action[9] = "ĞùÔ¯_ÀëºŞÌì";
            action[10] = "»³¹â_ÀëºŞÌì";
            action[11] = "Ì«ê»_ÀëºŞÌì";
            action[12] = "Ìì»ª_ÀëºŞÌì";
            action[13] = "ÁéÙí_ÀëºŞÌì";
            action[14] = "Ó¢ÕĞ_ÀëºŞÌì";
            action[15] = "ÇàÔÆÃÅ_ÀëºŞÌì";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÀëºŞÌì";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÀëºŞÌì";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÀëºŞÌì";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÀëºŞÌì";
            action[20] = "¹íµÀ_Æï³Ë_ÀëºŞÌì";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÀëºŞÌì";
            action[22] = "ÊŞÉñ_Æï³Ë_ÀëºŞÌì";
            action[23] = "ºüÑı_Æï³Ë_ÀëºŞÌì";
            action[24] = "ĞùÔ¯_Æï³Ë_ÀëºŞÌì";
            action[25] = "»³¹â_Æï³Ë_ÀëºŞÌì";
            action[26] = "Ì«ê»_Æï³Ë_ÀëºŞÌì";
            action[27] = "Ìì»ª_Æï³Ë_ÀëºŞÌì";
            action[28] = "ÁéÙí_Æï³Ë_ÀëºŞÌì";
            action[29] = "Ó¢ÕĞ_Æï³Ë_ÀëºŞÌì";
            action[30] = "0";
            rangetype = 4;
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
        virtual ~ Skill1529Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1528 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 6);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000 + 125 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 600000 - 18000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 95, 100, 105, 110, 115, 120, 125, 125, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (170));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
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
                               2000 + 125 * skill->GetLevel (),
                               18 + 2 * skill->GetLevel (), 18, 1, 8 * skill->GetLevel (), 2 + skill->GetLevel (), 24);

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
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetClearbuff (1);
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetType () == 0 ? 3100 : 0);
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetType () == 0 ? 0.08 * skill->GetLevel () : 0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetInchp (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ())));
            skill->GetVictim ()->SetTime (24000);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetAmount ((0.02 + 0.01 * skill->GetLevel ()) * 12 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak1 (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (18 + 2 * skill->GetLevel ());
        }
#endif
    };
}
#endif
