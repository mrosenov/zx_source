#ifndef __CPPGEN_GNET_SKILL1572
#define __CPPGEN_GNET_SKILL1572
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1572:public Skill
    {
      public:
        enum
        { SKILL_ID = 1572 };
          Skill1572 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1572Stub:public SkillStub
    {
      public:
        Skill1572Stub ():SkillStub (1572)
        {
            occupation = 150;
            name = L"皈依真法";
            nativename = "皈依真法";
            icon = "皈依真法.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 11;
            eventflag = 1;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
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
            talent[0] = 1574;
            talent[1] = 1576;
            talent[2] = 1578;
            talent[3] = 1580;
            talent_size = 4;
            action[0] = "青云门_皈依真法";
            action[1] = "青云门_皈依真法";
            action[2] = "鬼王宗_皈依真法";
            action[3] = "合欢派_皈依真法";
            action[4] = "天音寺_皈依真法";
            action[5] = "鬼道_皈依真法";
            action[6] = "焚香_皈依真法";
            action[7] = "兽神_皈依真法";
            action[8] = "狐妖_皈依真法";
            action[9] = "轩辕_皈依真法";
            action[10] = "怀光_皈依真法";
            action[11] = "太昊_皈依真法";
            action[12] = "天华_皈依真法";
            action[13] = "灵夙_皈依真法";
            action[14] = "英招_皈依真法";
            action[15] = "青云门_骑乘_皈依真法";
            action[16] = "青云门_骑乘_皈依真法";
            action[17] = "鬼王宗_骑乘_皈依真法";
            action[18] = "合欢派_骑乘_皈依真法";
            action[19] = "天音寺_骑乘_皈依真法";
            action[20] = "鬼道_骑乘_皈依真法";
            action[21] = "焚香_骑乘_皈依真法";
            action[22] = "兽神_骑乘_皈依真法";
            action[23] = "狐妖_骑乘_皈依真法";
            action[24] = "轩辕_骑乘_皈依真法";
            action[25] = "怀光_骑乘_皈依真法";
            action[26] = "太昊_骑乘_皈依真法";
            action[27] = "天华_骑乘_皈依真法";
            action[28] = "灵夙_骑乘_皈依真法";
            action[29] = "英招_骑乘_皈依真法";
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
#endif
        }
        virtual ~ Skill1572Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 90, 90, 90, 95, 100, 105, 110, 115, 120, 125 };
            return array[level - 1];
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 10000, 12000, 14000, 16000, 18000, 20000, 22000, 24000, 26000, 30000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               90 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel (), 4 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool TakeEffect (Skill * skill) const
        {
            skill->GetPlayer ()->SetPasdecfatalratio (int (skill->GetLevel () / 10) *
                                                      (int (skill->GetT0 () / 10) + int (skill->GetT1 () / 10) + int (skill->GetT2 () / 10) +
                                                       int (skill->GetT3 () / 10)) *0.01);
              skill->GetPlayer ()->SetPasaddhp (90 * skill->GetLevel () + 15 * skill->GetLevel () * skill->GetLevel ());
              skill->GetPlayer ()->SetPasadddizzy (4 * skill->GetLevel ());
              skill->GetPlayer ()->SetPasincskilldodge (50 * norm (skill->GetLevel () / 9.9));
              return true;
        }
#endif
    };
}
#endif
