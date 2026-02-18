#ifndef __CPPGEN_GNET_SKILL4466
#define __CPPGEN_GNET_SKILL4466
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4466:public Skill
    {
      public:
        enum
        { SKILL_ID = 4466 };
          Skill4466 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4466Stub:public SkillStub
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
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.02 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (12 - skill->GetLevel ())));
                skill->SetPlus (17 * skill->GetLevel () +
                                0.01 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) * (1 + 0.1 * skill->GetT0 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4466Stub ():SkillStub (4466)
        {
            occupation = 130;
            name = L"³Ð¡¤ËéÓñ»ÃÌì";
            nativename = "³Ð¡¤ËéÓñ»ÃÌì";
            icon = "Ç§ÓðÀ×¹â.dds";
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
            talent_size = 1;
            action[0] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[1] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[2] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[3] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[4] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[5] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[6] = "¹íµÀ_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[7] = "ÊÞÉñ_·¨±¦_Å­ÊÞá¦";
            action[8] = "ºüÑý_·¨±¦_µ¤Öì";
            action[9] = "ÐùÔ¯_·¨±¦_ÂÊ¶µÆì";
            action[10] = "»³¹â_·¨±¦_ÆßÉ±";
            action[11] = "Ì«ê»_·¨±¦_À×¹«Ä«";
            action[12] = "Ìì»ª_·¨±¦_¾ÅÒôÂÝ";
            action[13] = "01";
            action[14] = "01";
            action[15] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[16] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[17] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[18] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[19] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[21] = "¹íµÀ_Æï³Ë_·¨±¦_ÑªÓñ¹ÇÆ¬";
            action[22] = "ÊÞÉñ_Æï³Ë_·¨±¦_Å­ÊÞá¦";
            action[23] = "ºüÑý_Æï³Ë_·¨±¦_µ¤Öì";
            action[24] = "ÐùÔ¯_Æï³Ë_·¨±¦_ÂÊ¶µÆì";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_ÆßÉ±";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_À×¹«Ä«";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_¾ÅÒôÂÝ";
            action[28] = "01";
            action[29] = "01";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
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
        virtual ~ Skill4466Stub ()
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
            return (float) (46 - 4 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6300 - 300 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
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
                               46 - 4 * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (), 3 * skill->GetLevel (), 17 * skill->GetLevel (), 12 - skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
