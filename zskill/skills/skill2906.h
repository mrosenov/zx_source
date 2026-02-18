#ifndef __CPPGEN_GNET_SKILL2906
#define __CPPGEN_GNET_SKILL2906
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2906:public Skill
    {
      public:
        enum
        { SKILL_ID = 2906 };
          Skill2906 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2906Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2906Stub ():SkillStub (2906)
        {
            occupation = 151;
            name = L"ÔË³ïá¡á¢";
            nativename = "ÔË³ïá¡á¢";
            icon = "ÔË³ïá¡á¢.dds";
            maxlevel = 12;
            maxlearn = 12;
            type = 3;
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
            action[0] = "¹íÍõ×Ú_É¢¾¡";
            action[1] = "ÇàÔÆÃÅ_É¢¾¡";
            action[2] = "¹íÍõ×Ú_É¢¾¡";
            action[3] = "ºÏ»¶ÅÉ_É¢¾¡";
            action[4] = "ÌìÒôËÂ_É¢¾¡";
            action[5] = "¹íµÀ_É¢¾¡";
            action[6] = "¹íÍõ×Ú_É¢¾¡";
            action[7] = "ÊÞÉñ_É¢¾¡";
            action[8] = "ºüÑý_É¢¾¡";
            action[9] = "ÊÞÉñ_É¢¾¡";
            action[10] = "»³¹â_É¢¾¡";
            action[11] = "0";
            action[12] = "Ìì»ª_É¢¾¡";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_É¢¾¡";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_É¢¾¡";
            action[17] = "¹íÍõ×Ú_Æï³Ë_É¢¾¡";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_É¢¾¡";
            action[19] = "ÌìÒôËÂ_Æï³Ë_É¢¾¡";
            action[20] = "¹íµÀ_Æï³Ë_É¢¾¡";
            action[21] = "¹íÍõ×Ú_Æï³Ë_É¢¾¡";
            action[22] = "ÊÞÉñ_Æï³Ë_É¢¾¡";
            action[23] = "ºüÑý_Æï³Ë_É¢¾¡";
            action[24] = "ÊÞÉñ_Æï³Ë_É¢¾¡";
            action[25] = "»³¹â_Æï³Ë_É¢¾¡";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_É¢¾¡";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill2906Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 9);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               (skill->GetLevel () == 1 ? 40 : 0) + (skill->GetLevel () == 2 ? 50 : 0) + (skill->GetLevel () ==
                                                                                                          3 ? 60 : 0) + (skill->GetLevel () ==
                                                                                                                         4 ? 70 : 0) +
                               (skill->GetLevel () == 5 ? 80 : 0) + (skill->GetLevel () == 6 ? 100 : 0) + (skill->GetLevel () ==
                                                                                                           7 ? 110 : 0) + (skill->GetLevel () ==
                                                                                                                           8 ? 120 : 0) +
                               (skill->GetLevel () == 9 ? 130 : 0) + (skill->GetLevel () == 10 ? 140 : 0) + (skill->GetLevel () ==
                                                                                                             11 ? 180 : 0) + (skill->GetLevel () ==
                                                                                                                              12 ? 220 : 0));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), 2000);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 1 ? 40 : 0) + (skill->GetLevel () == 2 ? 50 : 0) +
                                           (skill->GetLevel () == 3 ? 60 : 0) + (skill->GetLevel () == 4 ? 70 : 0) + (skill->GetLevel () ==
                                                                                                                      5 ? 80 : 0) +
                                           (skill->GetLevel () == 6 ? 100 : 0) + (skill->GetLevel () == 7 ? 110 : 0) + (skill->GetLevel () ==
                                                                                                                        8 ? 120 : 0) +
                                           (skill->GetLevel () == 9 ? 130 : 0) + (skill->GetLevel () == 10 ? 140 : 0) + (skill->GetLevel () ==
                                                                                                                         11 ? 180 : 0) +
                                           (skill->GetLevel () == 12 ? 220 : 0));
            skill->GetVictim ()->SetSubantidizzy (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 1 ? 40 : 0) + (skill->GetLevel () == 2 ? 50 : 0) +
                                           (skill->GetLevel () == 3 ? 60 : 0) + (skill->GetLevel () == 4 ? 70 : 0) + (skill->GetLevel () ==
                                                                                                                      5 ? 80 : 0) +
                                           (skill->GetLevel () == 6 ? 100 : 0) + (skill->GetLevel () == 7 ? 110 : 0) + (skill->GetLevel () ==
                                                                                                                        8 ? 120 : 0) +
                                           (skill->GetLevel () == 9 ? 130 : 0) + (skill->GetLevel () == 10 ? 140 : 0) + (skill->GetLevel () ==
                                                                                                                         11 ? 180 : 0) +
                                           (skill->GetLevel () == 12 ? 220 : 0));
            skill->GetVictim ()->SetSubantisilent (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 1 ? 40 : 0) + (skill->GetLevel () == 2 ? 50 : 0) +
                                           (skill->GetLevel () == 3 ? 60 : 0) + (skill->GetLevel () == 4 ? 70 : 0) + (skill->GetLevel () ==
                                                                                                                      5 ? 80 : 0) +
                                           (skill->GetLevel () == 6 ? 100 : 0) + (skill->GetLevel () == 7 ? 110 : 0) + (skill->GetLevel () ==
                                                                                                                        8 ? 120 : 0) +
                                           (skill->GetLevel () == 9 ? 130 : 0) + (skill->GetLevel () == 10 ? 140 : 0) + (skill->GetLevel () ==
                                                                                                                         11 ? 180 : 0) +
                                           (skill->GetLevel () == 12 ? 220 : 0));
            skill->GetVictim ()->SetSubantiwrap (1);
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetValue ((skill->GetLevel () == 1 ? 40 : 0) + (skill->GetLevel () == 2 ? 50 : 0) +
                                           (skill->GetLevel () == 3 ? 60 : 0) + (skill->GetLevel () == 4 ? 70 : 0) + (skill->GetLevel () ==
                                                                                                                      5 ? 80 : 0) +
                                           (skill->GetLevel () == 6 ? 100 : 0) + (skill->GetLevel () == 7 ? 110 : 0) + (skill->GetLevel () ==
                                                                                                                        8 ? 120 : 0) +
                                           (skill->GetLevel () == 9 ? 130 : 0) + (skill->GetLevel () == 10 ? 140 : 0) + (skill->GetLevel () ==
                                                                                                                         11 ? 180 : 0) +
                                           (skill->GetLevel () == 12 ? 220 : 0));
            skill->GetVictim ()->SetSubantisleep (1);
            return true;
        }
#endif
    };
}
#endif
