#ifndef __CPPGEN_GNET_SKILL4508
#define __CPPGEN_GNET_SKILL4508
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4508:public Skill
    {
      public:
        enum
        { SKILL_ID = 4508 };
          Skill4508 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4508Stub:public SkillStub
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->SetCrit (0.01 * skill->GetLevel ());
                skill->SetCrithurt (0.05 * skill->GetLevel () + 0.01 * skill->GetLevel () * skill->GetLevel ());
                skill->SetRatio (1 + 0.07 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (215 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4508Stub ():SkillStub (4508)
        {
            occupation = 130;
            name = L"³Ğ¡¤ºÆÆøÔÆÌì";
            nativename = "³Ğ¡¤ºÆÆøÔÆÌì";
            icon = "ºÆÆøÔÆÌì.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
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
            action[0] = "Ì«ê»_·¨±¦_ôË»ÊÖÓ";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_Ç¬À¤Ëø";
            action[2] = "¹íÍõ×Ú_·¨±¦_ÓÄÚ¤¹íÊÖ";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_³à½ğç±";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÂÖ»ØÅÌ";
            action[5] = "¹íµÀ_·¨±¦_¹ííà";
            action[6] = "·ÙÏã¹È_·¨±¦_¾Åº®Äı±ù´Ì";
            action[7] = "ÊŞÉñ_·¨±¦_É¥ÃÅ½£";
            action[8] = "ºüÑı_·¨±¦_ÂåÊé";
            action[9] = "ĞùÔ¯_·¨±¦_³ĞÓ°";
            action[10] = "»³¹â_·¨±¦_¾øÓ°";
            action[11] = "Ì«ê»_·¨±¦_ôË»ÊÖÓ";
            action[12] = "Ìì»ª_·¨±¦_±ÌÂä·Éºè";
            action[13] = "1";
            action[14] = "1";
            action[15] = "Ì«ê»_Æï³Ë_·¨±¦_ôË»ÊÖÓ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_Ç¬À¤Ëø";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÓÄÚ¤¹íÊÖ";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_³à½ğç±";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÂÖ»ØÅÌ";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_¹ííà";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_¾Åº®Äı±ù´Ì";
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
            dobless = true;
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
        virtual ~ Skill4508Stub ()
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
            return (float) (445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               skill->GetLevel (),
                               5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               7 * skill->GetLevel (),
                               225 * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               20 + 8 * skill->GetLevel (),
                               skill->GetLevel (),
                               3 + 2 * skill->GetLevel (),
                               20 + 8 * skill->GetLevel (), 1, 10 * skill->GetLevel (), 2 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((3000 + 2000 * skill->GetLevel ()) * norm (zrand (100) / (80 - 8 * skill->GetLevel ())));
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecanti (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetVar11 () ==
                                                  100 ? (20 + 8 * skill->GetLevel ()) : (10 + 4 * skill->GetLevel ())));
            skill->GetVictim ()->SetAmount (skill->GetLevel () == 5 ? 3 : 1);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (10 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
#endif
    };
}
#endif
