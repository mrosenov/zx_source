#ifndef __CPPGEN_GNET_SKILL4530
#define __CPPGEN_GNET_SKILL4530
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4530:public Skill
    {
      public:
        enum
        { SKILL_ID = 4530 };
          Skill4530 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4530Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                return 330;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0.3 + 0.01 * skill->GetT0 () + 0.03 * skill->GetLevel () +
                                 0.003 * INT (skill->GetPlayer ()->GetLevel () / (8 - skill->GetLevel ())));
                skill->SetPlus (255 * skill->GetLevel () + 0.01 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
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
                return 330;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0.4 + 0.01 * skill->GetT0 () + 0.03 * skill->GetLevel () +
                                 0.003 * INT (skill->GetPlayer ()->GetLevel () / (8 - skill->GetLevel ())));
                skill->SetPlus (255 * skill->GetLevel () + 0.01 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 340;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (3);
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxatk ());
                skill->SetRatio (0.5 + 0.01 * skill->GetT0 () + 0.03 * skill->GetLevel () +
                                 0.003 * INT (skill->GetPlayer ()->GetLevel () / (8 - skill->GetLevel ())));
                skill->SetPlus (255 * skill->GetLevel () + 0.01 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4530Stub ():SkillStub (4530)
        {
            occupation = 130;
            name = L"承・天琊绝世";
            nativename = "承・天琊绝世";
            icon = "天琊绝世.dds";
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
            talent[0] = 687;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "鬼道_法宝_天琊";
            action[1] = "青云门_法宝_天琊";
            action[2] = "鬼王宗_法宝_天琊";
            action[3] = "合欢派_法宝_天琊";
            action[4] = "天音寺_法宝_天琊";
            action[5] = "鬼道_法宝_天琊";
            action[6] = "焚香谷_法宝_天琊";
            action[7] = "兽神_法宝_天琊";
            action[8] = "狐妖_法宝_天琊";
            action[9] = "轩辕_法宝_天琊";
            action[10] = "怀光_法宝_天琊";
            action[11] = "太昊_法宝_天琊";
            action[12] = "天华_法宝_天琊";
            action[13] = "灵夙_法宝_天琊";
            action[14] = "英招_法宝_天琊";
            action[15] = "鬼道_骑乘_法宝_天琊";
            action[16] = "青云门_骑乘_法宝_天琊";
            action[17] = "鬼王宗_骑乘_法宝_天琊";
            action[18] = "合欢派_骑乘_法宝_天琊";
            action[19] = "天音寺_骑乘_法宝_天琊";
            action[20] = "鬼道_骑乘_法宝_天琊";
            action[21] = "焚香谷_骑乘_法宝_天琊";
            action[22] = "兽神_骑乘_法宝_天琊";
            action[23] = "狐妖_骑乘_法宝_天琊";
            action[24] = "轩辕_骑乘_法宝_天琊";
            action[25] = "怀光_骑乘_法宝_天琊";
            action[26] = "太昊_骑乘_法宝_天琊";
            action[27] = "天华_骑乘_法宝_天琊";
            action[28] = "灵夙_骑乘_法宝_天琊";
            action[29] = "英招_骑乘_法宝_天琊";
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill4530Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.1 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (705 - 55 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
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
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.1 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.1 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               705 - 55 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (),
                               30 + 3 * skill->GetLevel (),
                               255 * skill->GetLevel (),
                               40 + 3 * skill->GetLevel (),
                               255 * skill->GetLevel (),
                               50 + 3 * skill->GetLevel (),
                               255 * skill->GetLevel (),
                               2 * (skill->GetLevel () + 1) * (skill->GetLevel () + 1),
                               skill->GetLevel () * 20,
                               10 + 4 * skill->GetLevel (),
                               (skill->GetLevel () == 5 ? 8 : (skill->GetLevel () == 4 ? 5 : skill->GetLevel ())),
                               7 * (skill->GetLevel () + 1),
                               0.5 * skill->GetLevel () * skill->GetLevel (),
                               6.3 - 0.3 * skill->GetLevel (),
                               8 - skill->GetLevel (),
                               skill->GetLevel () * 10,
                               10 + 4 * skill->GetLevel (),
                               skill->GetLevel () * 20,
                               skill->GetLevel () * 20,
                               20 * skill->GetLevel (), 3 * skill->GetLevel () + 1, 4 * skill->GetLevel (), 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? skill->GetLevel () * 20 : 0));
            skill->GetVictim ()->SetTime (10000 + 4000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetType () ==
                                           0 ? (2 * (skill->GetLevel () + 1) * (skill->GetLevel () + 1) * 0.01 +
                                                zrand (2) * (1 -
                                                             2 * (skill->GetLevel () + 1) * (skill->GetLevel () +
                                                                                             1) * 0.01)) : (skill->GetPlayer ()->GetDef () <
                                                                                                            25000 ? 2 * (skill->GetLevel () +
                                                                                                                         1) * (skill->GetLevel () +
                                                                                                                               1) * 0.01 +
                                                                                                            zrand (2) * (1 -
                                                                                                                         2 * (skill->GetLevel () +
                                                                                                                              1) *
                                                                                                                         (skill->GetLevel () +
                                                                                                                          1) * 0.01) : 0));
            skill->GetVictim ()->SetBuffid (6);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetTime ((zrand (100) < skill->GetLevel () * 20 ? 1 : 0) * 1000 * (3 * skill->GetLevel () + 1));
            skill->GetVictim ()->SetAmount (4 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar2 () * 0.04 * skill->GetLevel ());
            skill->GetVictim ()->SetCursed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar1 () ==
                                          3 ? (zrand (100) <=
                                               (34 + 5 * (skill->GetLevel () + 1) +
                                                (skill->GetLevel () + 1) * (skill->GetLevel () +
                                                                            1)) ? 1000 * (0.5 * skill->GetLevel () * skill->GetLevel ()) : 0) : 0);
            skill->GetVictim ()->SetRatio ((skill->GetLevel () == 5 ? 8 : (skill->GetLevel () == 4 ? 5 : skill->GetLevel ())) * 0.01001);
            skill->GetVictim ()->SetInccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () * 20));
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetAmount (2);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetExorcism (1);
            return true;
        }
#endif
    };
}
#endif
