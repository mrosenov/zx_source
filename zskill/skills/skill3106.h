#ifndef __CPPGEN_GNET_SKILL3106
#define __CPPGEN_GNET_SKILL3106
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3106:public Skill
    {
      public:
        enum
        { SKILL_ID = 3106 };
          Skill3106 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3106Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1700;
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
                return 300;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3106Stub ():SkillStub (3106)
        {
            occupation = 169;
            name = L"万法不侵";
            nativename = "万法不侵";
            icon = "神圣护体.dds";
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
            clearmask = 4;
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
            action[0] = "佛_神圣护体";
            action[1] = "佛_神圣护体";
            action[2] = "佛_神圣护体";
            action[3] = "佛_神圣护体";
            action[4] = "佛_神圣护体";
            action[5] = "佛_神圣护体";
            action[6] = "佛_神圣护体";
            action[7] = "佛_神圣护体";
            action[8] = "佛_神圣护体";
            action[9] = "佛_神圣护体";
            action[10] = "佛_神圣护体";
            action[11] = "佛_神圣护体";
            action[12] = "佛_神圣护体";
            action[13] = "佛_神圣护体";
            action[14] = "佛_神圣护体";
            action[15] = "骑乘_佛_神圣护体";
            action[16] = "骑乘_佛_神圣护体";
            action[17] = "骑乘_佛_神圣护体";
            action[18] = "骑乘_佛_神圣护体";
            action[19] = "骑乘_佛_神圣护体";
            action[20] = "骑乘_佛_神圣护体";
            action[21] = "骑乘_佛_神圣护体";
            action[22] = "骑乘_佛_神圣护体";
            action[23] = "骑乘_佛_神圣护体";
            action[24] = "骑乘_佛_神圣护体";
            action[25] = "骑乘_佛_神圣护体";
            action[26] = "骑乘_佛_神圣护体";
            action[27] = "骑乘_佛_神圣护体";
            action[28] = "骑乘_佛_神圣护体";
            action[29] = "骑乘_佛_神圣护体";
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
        virtual ~ Skill3106Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
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
            return 200000 - 5000 * skill->GetLevel ();
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
                               2 + 0.5 * skill->GetLevel (), 200 - 5 * skill->GetLevel (), 0.2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 4000);
            skill->GetVictim ()->SetImmunedizzy (1);
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 4000);
            skill->GetVictim ()->SetImmunewrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (2050 + 500 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 4000);
            skill->GetVictim ()->SetRatio (0.2 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 2);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncdefence (1);
            return true;
        }
#endif
    };
}
#endif
