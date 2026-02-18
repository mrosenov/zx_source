#ifndef __CPPGEN_GNET_SKILL1111
#define __CPPGEN_GNET_SKILL1111
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1111:public Skill
    {
      public:
        enum
        { SKILL_ID = 1111 };
          Skill1111 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1111Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0.48 + 0.02 * (skill->GetLevel () > 13 ? (skill->GetLevel () - 13) : skill->GetLevel ()));
                skill->SetPlus (0.02 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1111Stub ():SkillStub (1111)
        {
            occupation = 26;
            name = L"ÄýÑªÑý¹Æ";
            nativename = "ÄýÑªÑý¹Æ";
            icon = "ÄýÑªÑý¹Æ.dds";
            maxlevel = 30;
            maxlearn = 12;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 4;
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
            skill_class = 4;
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
            preskillnum = 1;
            talent[0] = 1179;
            talent[1] = 1185;
            talent[2] = 1189;
            talent[3] = 1169;
            talent_size = 4;
            action[0] = "ÄýÑªÑý¹Æ";
            action[1] = "ÄýÑªÑý¹Æ";
            action[2] = "ÄýÑªÑý¹Æ";
            action[3] = "ÄýÑªÑý¹Æ";
            action[4] = "ÄýÑªÑý¹Æ";
            action[5] = "ÄýÑªÑý¹Æ";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÄýÑªÑý¹Æ";
            action[16] = "ÄýÑªÑý¹Æ";
            action[17] = "ÄýÑªÑý¹Æ";
            action[18] = "ÄýÑªÑý¹Æ";
            action[19] = "ÄýÑªÑý¹Æ";
            action[20] = "ÄýÑªÑý¹Æ";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
        virtual ~ Skill1111Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1108 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetLevel () >
                            13 ? 0.25 * (5.31 * (skill->GetLevel () - 13) * (skill->GetLevel () - 13) + 7.04 * (skill->GetLevel () - 13) +
                                         1030) : 0.25 * (5.31 * skill->GetLevel () * skill->GetLevel () + 7.04 * skill->GetLevel () + 1030));
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 7000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[12] = { 70, 72, 74, 76, 78, 80, 82, 84, 87, 90, 93, 96 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (22);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.25 * (5.31 * skill->GetLevel () * skill->GetLevel () + 7.04 * skill->GetLevel () + 1030)),
                               48 + 2 * skill->GetLevel (),
                               20,
                               24,
                               INT (5.31 * skill->GetLevel () * skill->GetLevel () + 7.04 * skill->GetLevel () + 1030),
                               8 + 2 * skill->GetLevel (), 3 * skill->GetLevel (), 4);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (3 * (skill->GetLevel () > 13 ? (skill->GetLevel () - 13) : skill->GetLevel ()) *
                                                  norm ((0.08 + 0.02 * (skill->GetLevel () > 13 ? (skill->GetLevel () - 13) : skill->GetLevel ()) +
                                                         0.05 * skill->GetT0 ()) * skill->GetPlayer ()->GetMaxhp () / (skill->GetPlayer ()->GetHp () +
                                                                                                                       1.0))));
            skill->GetVictim ()->SetTime (4000 + 100 + 1000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.3);
            skill->GetVictim ()->SetValue (0.3);
            skill->GetVictim ()->SetRandcurse (1);
            skill->GetVictim ()->SetProbability (1.0 * (100 * (1 - 0.002 * skill->GetPlayer ()->GetLevel ()) * (1 + 0.1 * skill->GetT2 ())));
            skill->GetVictim ()->SetTime (24000 - 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetLevel () >
                                            13 ? (1 +
                                                  0.4 * skill->GetT3 ()) * ((5.31 * (skill->GetLevel () - 13) * (skill->GetLevel () - 13) +
                                                                             7.04 * (skill->GetLevel () - 13) + 1030) * (1 + 0.1 * skill->GetT1 ()) +
                                                                            0.005 * skill->GetT1 () * (skill->GetLevel () -
                                                                                                       13) * skill->GetPlayer ()->GetHp ()) : (5.31 *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () +
                                                                                                                                               7.04 *
                                                                                                                                               skill->
                                                                                                                                               GetLevel
                                                                                                                                               () +
                                                                                                                                               1030) *
                                            (1 + 0.1 * skill->GetT1 ()) +
                                            0.005 * skill->GetT1 () * skill->GetLevel () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (skill->GetLevel () >
                                           13 ? (5.31 * (skill->GetLevel () - 13) * (skill->GetLevel () - 13) + 7.04 * (skill->GetLevel () - 13) +
                                                 1030) * 0.1 * (1 +
                                                                0.4 * skill->GetT3 ()) * skill->GetT0 () : (5.31 * skill->GetLevel () *
                                                                                                            skill->GetLevel () +
                                                                                                            7.04 * skill->GetLevel () +
                                                                                                            1030) * 0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetHpleak1 (1);
            return true;
        }
#endif
    };
}
#endif
