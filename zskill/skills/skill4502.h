#ifndef __CPPGEN_GNET_SKILL4502
#define __CPPGEN_GNET_SKILL4502
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4502:public Skill
    {
      public:
        enum
        { SKILL_ID = 4502 };
          Skill4502 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4502Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.06 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (215 * skill->GetLevel () +
                                0.04 * (skill->GetPlayer ()->GetMp () + skill->GetPlayer ()->GetHp ()) * (1 + 0.1 * skill->GetT0 ()) +
                                0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4502Stub ():SkillStub (4502)
        {
            occupation = 130;
            name = L"³Ğ¡¤ÃÔ×ÙÖ®¼Å";
            nativename = "³Ğ¡¤ÃÔ×ÙÖ®¼Å";
            icon = "öïÅôÕñ³á.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[0] = 1174;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íµÀ_·¨±¦_¹ííà";
            action[1] = "¹íµÀ_·¨±¦_¹ííà";
            action[2] = "¹íµÀ_·¨±¦_¹ííà";
            action[3] = "¹íµÀ_·¨±¦_¹ííà";
            action[4] = "¹íµÀ_·¨±¦_¹ííà";
            action[5] = "¹íµÀ_·¨±¦_¹ííà";
            action[6] = "¹íµÀ_·¨±¦_¹ííà";
            action[7] = "ÊŞÉñ_·¨±¦_É¥ÃÅ½£";
            action[8] = "ºüÑı_·¨±¦_ÂåÊé";
            action[9] = "ĞùÔ¯_·¨±¦_³ĞÓ°";
            action[10] = "»³¹â_·¨±¦_¾øÓ°";
            action[11] = "Ì«ê»_·¨±¦_ôË»ÊÖÓ";
            action[12] = "Ìì»ª_·¨±¦_±ÌÂä·Éºè";
            action[13] = "1";
            action[14] = "1";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[16] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[17] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[18] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[19] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[21] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_É¥ÃÅ½£";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_ÂåÊé";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_³ĞÓ°";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_¾øÓ°";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_ôË»ÊÖÓ";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_±ÌÂä·Éºè";
            action[28] = "1";
            action[29] = "1";
            action[30] = "0";
            rangetype = 0;
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
        virtual ~ Skill4502Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (495 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               495 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               215 * skill->GetLevel (),
                               4 * skill->GetLevel (),
                               500 * skill->GetLevel () + 100 * skill->GetLevel () * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               5,
                               5 * skill->GetLevel (),
                               2 * skill->GetLevel (), 10 + 5 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 5)) * 600));
            skill->GetVictim ()->SetTime (5000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (4000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0.2 * skill->GetPlayer ()->GetMaxmp () + 1);
            skill->GetVictim ()->SetValue (500 * skill->GetLevel () + 100 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetSubmp (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar11 () ==
                                                  29 ? 100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ()) : 23));
            skill->GetVictim ()->SetTime (2000 + 1000 * INT (skill->GetPlayer ()->GetLevel () / 2 / skill->GetLevel ()));
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (0.01 * (10 + 5 * skill->GetLevel () + 5 * skill->GetLevel () * skill->GetLevel ()) *
                                            skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
