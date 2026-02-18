#ifndef __CPPGEN_GNET_SKILL3885
#define __CPPGEN_GNET_SKILL3885
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3885:public Skill
    {
      public:
        enum
        { SKILL_ID = 3885 };
          Skill3885 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3885Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 650;
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 525;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (2);
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill3885Stub ():SkillStub (3885)
        {
            occupation = 128;
            name = L"¶ªË®Çò";
            nativename = "¶ªË®Çò";
            icon = "Ë®Çò.dds";
            maxlevel = 1;
            maxlearn = 0;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 3;
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
            action[0] = "¶ªÑ©Çò";
            action[1] = "¶ªÑ©Çò";
            action[2] = "¶ªÑ©Çò";
            action[3] = "¶ªÑ©Çò";
            action[4] = "¶ªÑ©Çò";
            action[5] = "¶ªÑ©Çò";
            action[6] = "¶ªÑ©Çò";
            action[7] = "¶ªÑ©Çò";
            action[8] = "¶ªÑ©Çò";
            action[9] = "¶ªÑ©Çò";
            action[10] = "¶ªÑ©Çò";
            action[11] = "¶ªÑ©Çò";
            action[12] = "¶ªÑ©Çò";
            action[13] = "¶ªÑ©Çò";
            action[14] = "¶ªÑ©Çò";
            action[15] = "¶ªÑ©Çò";
            action[16] = "¶ªÑ©Çò";
            action[17] = "¶ªÑ©Çò";
            action[18] = "¶ªÑ©Çò";
            action[19] = "¶ªÑ©Çò";
            action[20] = "¶ªÑ©Çò";
            action[21] = "¶ªÑ©Çò";
            action[22] = "¶ªÑ©Çò";
            action[23] = "¶ªÑ©Çò";
            action[24] = "¶ªÑ©Çò";
            action[25] = "¶ªÑ©Çò";
            action[26] = "¶ªÑ©Çò";
            action[27] = "¶ªÑ©Çò";
            action[28] = "¶ªÑ©Çò";
            action[29] = "¶ªÑ©Çò";
            action[30] = "¶ªÑ©Çò";
            rangetype = 5;
            doenchant = true;
            dobless = false;
            need_item = true;
            item_relation = 0;
            item1_id = 52236;
            item1_num = 1;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 1024;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill3885Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3575;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
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
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (1000);
            skill->GetVictim ()->SetRatio (5 + 10 * skill->GetCharging () / 2000);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (52206);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetCreateobject (1);
            return true;
        }
#endif
    };
}
#endif
