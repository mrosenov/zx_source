#ifndef __CPPGEN_GNET_SKILL3060
#define __CPPGEN_GNET_SKILL3060
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3060:public Skill
    {
      public:
        enum
        { SKILL_ID = 3060 };
          Skill3060 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3060Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1250;
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
                return 750;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.1 * skill->GetLevel ());
                skill->SetSaint (skill->GetPlayer ()->GetSaint () * 0.2 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 64);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3060Stub ():SkillStub (3060)
        {
            occupation = 168;
            name = L"噬灵";
            nativename = "噬灵";
            icon = "邪恶攻击.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
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
            action[0] = "魔_神圣点燃";
            action[1] = "魔_神圣点燃";
            action[2] = "魔_神圣点燃";
            action[3] = "魔_神圣点燃";
            action[4] = "魔_神圣点燃";
            action[5] = "魔_神圣点燃";
            action[6] = "魔_神圣点燃";
            action[7] = "魔_神圣点燃";
            action[8] = "魔_神圣点燃";
            action[9] = "魔_神圣点燃";
            action[10] = "魔_神圣点燃";
            action[11] = "魔_神圣点燃";
            action[12] = "魔_神圣点燃";
            action[13] = "魔_神圣点燃";
            action[14] = "魔_神圣点燃";
            action[15] = "骑乘_魔_神圣点燃";
            action[16] = "骑乘_魔_神圣点燃";
            action[17] = "骑乘_魔_神圣点燃";
            action[18] = "骑乘_魔_神圣点燃";
            action[19] = "骑乘_魔_神圣点燃";
            action[20] = "骑乘_魔_神圣点燃";
            action[21] = "骑乘_魔_神圣点燃";
            action[22] = "骑乘_魔_神圣点燃";
            action[23] = "骑乘_魔_神圣点燃";
            action[24] = "骑乘_魔_神圣点燃";
            action[25] = "骑乘_魔_神圣点燃";
            action[26] = "骑乘_魔_神圣点燃";
            action[27] = "骑乘_魔_神圣点燃";
            action[28] = "骑乘_魔_神圣点燃";
            action[29] = "骑乘_魔_神圣点燃";
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
        virtual ~ Skill3060Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 4);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 2000 * skill->GetLevel ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 2);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               60 - 2 * skill->GetLevel (), skill->GetLevel () * 10, 0.2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetIsignite ()? 120 : 0));
            skill->GetVictim ()->SetTime (3050);
            skill->GetVictim ()->SetDarkness (1);
            return true;
        }
#endif
    };
}
#endif
