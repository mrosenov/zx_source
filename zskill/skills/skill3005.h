#ifndef __CPPGEN_GNET_SKILL3005
#define __CPPGEN_GNET_SKILL3005
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3005:public Skill
    {
      public:
        enum
        { SKILL_ID = 3005 };
          Skill3005 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3005Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1100;
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
      Skill3005Stub ():SkillStub (3005)
        {
            occupation = 167;
            name = L"ÉñÁ¦±¬·¢";
            nativename = "ÉñÁ¦±¬·¢";
            icon = "ÔªÆø±¬·¢.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
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
            preskillnum = 0;
            talent[0] = 3711;
            talent[1] = 4449;
            talent[2] = 4450;
            talent[3] = 4451;
            talent_size = 4;
            action[0] = "ÏÉ_ÐÇ³½±¬·¢";
            action[1] = "ÏÉ_ÐÇ³½±¬·¢";
            action[2] = "ÏÉ_ÐÇ³½±¬·¢";
            action[3] = "ÏÉ_ÐÇ³½±¬·¢";
            action[4] = "ÏÉ_ÐÇ³½±¬·¢";
            action[5] = "ÏÉ_ÐÇ³½±¬·¢";
            action[6] = "ÏÉ_ÐÇ³½±¬·¢";
            action[7] = "ÏÉ_ÐÇ³½±¬·¢";
            action[8] = "ÏÉ_ÐÇ³½±¬·¢";
            action[9] = "ÏÉ_ÐÇ³½±¬·¢";
            action[10] = "ÏÉ_ÐÇ³½±¬·¢";
            action[11] = "ÏÉ_ÐÇ³½±¬·¢";
            action[12] = "ÏÉ_ÐÇ³½±¬·¢";
            action[13] = "ÏÉ_ÐÇ³½±¬·¢";
            action[14] = "ÏÉ_ÐÇ³½±¬·¢";
            action[15] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[16] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[17] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[18] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[19] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[20] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[21] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[22] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[23] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[24] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[25] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[26] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[27] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[28] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[29] = "Æï³Ë_ÏÉ_ÐÇ³½±¬·¢";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill3005Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (184 + 10 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () -
                          skill->GetT0 () * (184 + 10 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()));
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 10300, 16900, 21800, 43300, 49900, 81300, 84500, 197600, 340200, 435500, 998600,
 1897100 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 23, 27, 31, 36, 41, 46, 51, 57, 63, 69, 75, 81 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000 - 5000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 150 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT ((184 + 10 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()) * 0.5),
                               INT (5 + 2 * skill->GetLevel () + norm (skill->GetLevel () - 15) * (skill->GetLevel () - 15)),
                               300 - 5 * skill->GetLevel (),
                               1 * skill->GetLevel (),
                               1 * skill->GetLevel (),
                               1 * skill->GetLevel (),
                               1 * skill->GetLevel (), 1 * skill->GetLevel (), skill->GetT0 () > 0 ? table.Find (30053) : table.Find (30054));

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
            skill->GetVictim ()->SetTime (5000 + 2000 * skill->GetLevel () + norm (skill->GetLevel () / 15) * (skill->GetLevel () - 15) * 1000 +
                                          8000 * skill->GetT0 () + 8000 * skill->GetT1 () + 5000 * skill->GetT2 () + 3000 * skill->GetT3 ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((184 + 10 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel ()) *
                                            (5 + 2 * skill->GetLevel () + norm (skill->GetLevel () / 15) * (skill->GetLevel () - 15)) * 0.25 -
                                            (0.25 * skill->GetT0 () + 0.25 * skill->GetT1 () + 0.1 * skill->GetT2 () +
                                             0.05 * skill->GetT3 ()) * (184 + 10 * skill->GetLevel () +
                                                                        2 * skill->GetLevel () * skill->GetLevel ()) * (5 + 2 * skill->GetLevel () +
                                                                                                                        norm (skill->GetLevel () /
                                                                                                                              15) *
                                                                                                                        (skill->GetLevel () - 15)));
            skill->GetVictim ()->SetMiracleburstxian (1);
            return true;
        }
#endif
    };
}
#endif
