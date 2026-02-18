#ifndef __CPPGEN_GNET_SKILL1227
#define __CPPGEN_GNET_SKILL1227
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1227:public Skill
    {
      public:
        enum
        { SKILL_ID = 1227 };
          Skill1227 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1227Stub:public SkillStub
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
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.06 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (215 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1227Stub ():SkillStub (1227)
        {
            occupation = 130;
            name = L"»ªÕÃÐÇÔÉ";
            nativename = "»ªÕÃÐÇÔÉ";
            icon = "»ªÕÃÐÇÔÉ.dds";
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
            talent[0] = 721;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íµÀ_·¨±¦_ÆßÐÇÓ¡";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_ÆßÐÇÓ¡";
            action[2] = "¹íÍõ×Ú_·¨±¦_ÆßÐÇÓ¡";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_ÆßÐÇÓ¡";
            action[4] = "ÌìÒôËÂ_·¨±¦_ÆßÐÇÓ¡";
            action[5] = "¹íµÀ_·¨±¦_ÆßÐÇÓ¡";
            action[6] = "·ÙÏã¹È_·¨±¦_ÆßÐÇÓ¡";
            action[7] = "ÊÞÉñ_·¨±¦_ÆßÐÇÓ¡";
            action[8] = "ºüÑý_·¨±¦_ÆßÐÇÓ¡";
            action[9] = "ÐùÔ¯_·¨±¦_ÆßÐÇÓ¡";
            action[10] = "»³¹â_·¨±¦_ÆßÐÇÓ¡";
            action[11] = "Ì«ê»_·¨±¦_ÆßÐÇÓ¡";
            action[12] = "Ìì»ª_·¨±¦_ÆßÐÇÓ¡";
            action[13] = "ÁéÙí_·¨±¦_ÆßÐÇÓ¡";
            action[14] = "Ó¢ÕÐ_·¨±¦_ÆßÐÇÓ¡";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[22] = "ÊÞÉñ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[23] = "ºüÑý_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[24] = "ÐùÔ¯_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[28] = "ÁéÙí_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
            action[29] = "Ó¢ÕÐ_Æï³Ë_·¨±¦_ÆßÐÇÓ¡";
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
        virtual ~ Skill1227Stub ()
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
        float GetRadius (Skill * skill) const
        {
            return (float) (4);
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
                               6 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               215 * skill->GetLevel (),
                               15 + skill->GetLevel () * skill->GetLevel (),
                               10,
                               skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               15 + skill->GetLevel () * skill->GetLevel (),
                               skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               10,
                               INT (2 + skill->GetLevel () * skill->GetLevel ()),
                               2 * skill->GetLevel (), 25 + skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15 + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (10000 + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.01 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetInchurt (1);
            skill->GetVictim ()->SetProbability (1.0 * (15 + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (10000 + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.01 * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetDecaccuracy (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 2 - skill->GetLevel () * skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * (0.25 + 0.01 * skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (2);
        }
#endif
    };
}
#endif
