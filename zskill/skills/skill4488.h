#ifndef __CPPGEN_GNET_SKILL4488
#define __CPPGEN_GNET_SKILL4488
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4488:public Skill
    {
      public:
        enum
        { SKILL_ID = 4488 };
          Skill4488 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4488Stub:public SkillStub
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
                skill->SetRatio (1 + 0.1 * skill->GetT0 () + 0.05 * skill->GetLevel () +
                                 0.01 * INT (skill->GetPlayer ()->GetLevel () / (10 - skill->GetLevel ())));
                skill->SetPlus (120 * skill->GetLevel () + 0.03 * skill->GetT1 () * skill->GetPlayer ()->GetDef ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4488Stub ():SkillStub (4488)
        {
            occupation = 130;
            name = L"承・乾坤如意";
            nativename = "承・乾坤如意";
            icon = "乾坤如意.dds";
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
            talent[0] = 670;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "鬼王宗_法宝_如意乾坤袋";
            action[1] = "青云门_法宝_如意乾坤袋";
            action[2] = "鬼王宗_法宝_如意乾坤袋";
            action[3] = "合欢派_法宝_如意乾坤袋";
            action[4] = "天音寺_法宝_如意乾坤袋";
            action[5] = "鬼道_法宝_如意乾坤袋";
            action[6] = "鬼道_法宝_如意乾坤袋";
            action[7] = "兽神_法宝_罡岚";
            action[8] = "狐妖_法宝_浮影古镜";
            action[9] = "轩辕_法宝_落地金钱";
            action[10] = "怀光_法宝_贪狼";
            action[11] = "太昊_法宝_子母阴阳扣";
            action[12] = "天华_法宝_琉璃盏";
            action[13] = "01";
            action[14] = "01";
            action[15] = "鬼王宗_骑乘_法宝_如意乾坤袋";
            action[16] = "青云门_骑乘_法宝_如意乾坤袋";
            action[17] = "鬼王宗_骑乘_法宝_如意乾坤袋";
            action[18] = "合欢派_骑乘_法宝_如意乾坤袋";
            action[19] = "天音寺_骑乘_法宝_如意乾坤袋";
            action[20] = "鬼道_骑乘_法宝_如意乾坤袋";
            action[21] = "鬼道_骑乘_法宝_如意乾坤袋";
            action[22] = "兽神_骑乘_法宝_罡岚";
            action[23] = "狐妖_骑乘_法宝_浮影古镜";
            action[24] = "轩辕_骑乘_法宝_落地金钱";
            action[25] = "怀光_骑乘_法宝_贪狼";
            action[26] = "太昊_骑乘_法宝_子母阴阳扣";
            action[27] = "天华_骑乘_法宝_琉璃盏";
            action[28] = "01";
            action[29] = "01";
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
        virtual ~ Skill4488Stub ()
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
            return (float) (15);
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
                               120 * skill->GetLevel (),
                               25 * skill->GetLevel (),
                               3 * skill->GetLevel (),
                               10 - skill->GetLevel (), 24 * skill->GetLevel (), 4 + skill->GetLevel (), 3 + 2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (3000 * skill->GetLevel () + 100 + 1000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (25 * skill->GetLevel () + 15 * skill->GetT0 ());
            skill->GetVictim ()->SetSubattack (1);
            skill->GetVictim ()->SetProbability (1.0 * (norm (zrand (100) / (100 - 4 - skill->GetLevel ())) * 600));
            skill->GetVictim ()->SetTime (3000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (24 * skill->GetLevel () * (1 + skill->GetT0 ()));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (1 + INT (zrand (110) / 100) * 12);
        }
#endif
    };
}
#endif
