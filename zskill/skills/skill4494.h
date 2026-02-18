#ifndef __CPPGEN_GNET_SKILL4494
#define __CPPGEN_GNET_SKILL4494
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4494:public Skill
    {
      public:
        enum
        { SKILL_ID = 4494 };
          Skill4494 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4494Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (120 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4494Stub ():SkillStub (4494)
        {
            occupation = 130;
            name = L"³Ğ¡¤ÆÆ»êÇ§½Ù";
            nativename = "³Ğ¡¤ÆÆ»êÇ§½Ù";
            icon = "ÆÆ»êÇ§½Ù.dds";
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
            action[0] = "»³¹â_·¨±¦_Ì°ÀÇ";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_çúçêÖìç±";
            action[2] = "¹íÍõ×Ú_·¨±¦_Ç¬À¤Çà¹â½ä";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_Á«»¨";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[5] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[6] = "·ÙÏã¹È_·¨±¦_»ğÁáçç";
            action[7] = "ÊŞÉñ_·¨±¦_î¸á°";
            action[8] = "ºüÑı_·¨±¦_¸¡Ó°¹Å¾µ";
            action[9] = "ĞùÔ¯_·¨±¦_ÂäµØ½ğÇ®";
            action[10] = "»³¹â_·¨±¦_Ì°ÀÇ";
            action[11] = "Ì«ê»_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[12] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
            action[13] = "01";
            action[14] = "01";
            action[15] = "»³¹â_Æï³Ë_·¨±¦_Ì°ÀÇ";
            action[16] = "ÇàÔÆÃÅ_·¨±¦_çúçêÖìç±";
            action[17] = "¹íÍõ×Ú_·¨±¦_Ç¬À¤Çà¹â½ä";
            action[18] = "ºÏ»¶ÅÉ_·¨±¦_Á«»¨";
            action[19] = "ÌìÒôËÂ_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[20] = "¹íµÀ_·¨±¦_Ç§¶¾¹Æ";
            action[21] = "·ÙÏã¹È_·¨±¦_»ğÁáçç";
            action[22] = "ÊŞÉñ_·¨±¦_î¸á°";
            action[23] = "ºüÑı_·¨±¦_¸¡Ó°¹Å¾µ";
            action[24] = "ĞùÔ¯_·¨±¦_ÂäµØ½ğÇ®";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_Ì°ÀÇ";
            action[26] = "Ì«ê»_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[27] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
            action[28] = "01";
            action[29] = "01";
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
        virtual ~ Skill4494Stub ()
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
            return (float) (295 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
                               295 - 25 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               120 * skill->GetLevel (), 10 - skill->GetLevel (), 12, 20 * skill->GetLevel (), 1 + skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (20 * skill->GetLevel ());
            skill->GetVictim ()->SetSubantisilent (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetRes1 () + skill->GetPlayer ()->GetRes2 () +
                                                   skill->GetPlayer ()->GetRes3 () + skill->GetPlayer ()->GetRes4 () +
                                                   skill->GetPlayer ()->GetRes5 ()) / 5));
            skill->GetVictim ()->SetTime (1000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetSilent (1);
            return true;
        }
#endif
    };
}
#endif
