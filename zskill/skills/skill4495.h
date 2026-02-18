#ifndef __CPPGEN_GNET_SKILL4495
#define __CPPGEN_GNET_SKILL4495
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4495:public Skill
    {
      public:
        enum
        { SKILL_ID = 4495 };
          Skill4495 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4495Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1225;
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
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (1);
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (50 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 475;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar3 (0);
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (50 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4495Stub ():SkillStub (4495)
        {
            occupation = 130;
            name = L"³Ğ¡¤Óî¹âÅîÀ³";
            nativename = "³Ğ¡¤Óî¹âÅîÀ³";
            icon = "Óî¹âÅîÀ³.dds";
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
            action[0] = "Ìì»ª_·¨±¦_ÁğÁ§Õµ";
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
            action[15] = "Ìì»ª_Æï³Ë_·¨±¦_ÁğÁ§Õµ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_çúçêÖìç±";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_Ç¬À¤Çà¹â½ä";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_Á«»¨";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÈçÒâÇ¬À¤´ü";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_Ç§¶¾¹Æ";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_»ğÁáçç";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_î¸á°";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_¸¡Ó°¹Å¾µ";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_ÂäµØ½ğÇ®";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_Ì°ÀÇ";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_×ÓÄ¸ÒõÑô¿Û";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÁğÁ§Õµ";
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
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill4495Stub ()
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
        float GetRadius (Skill * skill) const
        {
            return (float) (skill->GetLevel () == 5 ? 8 : (1 + skill->GetLevel ()));
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
                               2 * skill->GetLevel (),
                               50 * skill->GetLevel (),
                               21 * skill->GetLevel (),
                               5 + skill->GetLevel () * 0.5,
                               10 - skill->GetLevel (), 5 + 0.5 * skill->GetLevel (), 5 + skill->GetLevel () * 5, 2 + skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (5000 + skill->GetLevel () * 500);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (21 * skill->GetLevel ());
            skill->GetVictim ()->SetSubantiweak (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()));
            skill->GetVictim ()->SetTime (5000 + skill->GetLevel () * 500);
            skill->GetVictim ()->SetRatio (0.05 + skill->GetLevel () * 0.05);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar3 () * (INT (zrand (100)) < (skill->GetLevel () + 2) ? 100 : 0)));
            skill->GetVictim ()->
                SetValue ((skill->GetPlayer ()->GetVar2 () *
                           (1 + 0.03 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ()))) +
                           50 * skill->GetLevel ()) >
                          skill->GetPlayer ()->GetDef ()? (skill->GetPlayer ()->GetVar2 () *
                                                           (1 + 0.03 * skill->GetLevel () +
                                                            0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ()))) +
                                                           50 * skill->GetLevel () - skill->GetPlayer ()->GetDef ()) : 0);
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
#endif
    };
}
#endif
