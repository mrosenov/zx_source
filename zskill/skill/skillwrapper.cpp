
#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>

/* GCC 3.3 / FC2 compatibility: explicit POSIX and C runtime includes */
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include <ASSERT.h>
#include "playerwrapper.h"
#include "skill.h"
#include "skillwrapper.h"
#include "skillfilter.h"

namespace GNET
{
    static FILE* g_skill_log = NULL;

    // Usage: skill_log("LoadDatabase: version=%d size=%lu", (int)version, (unsigned long)size);
    static void skill_log(const char* fmt, ...)
    {
        if (!g_skill_log)
        {
            g_skill_log = fopen("/root/1378/libskill.log", "a");
            if (!g_skill_log)
                return;
        }

        // timestamp
        time_t t = time(NULL);
        struct tm* tm_info = localtime(&t);

        fprintf(g_skill_log, "[%02d:%02d:%02d][pid=%d] ",
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec,
            (int)getpid());

        va_list args;
        va_start(args, fmt);
        vfprintf(g_skill_log, fmt, args);
        va_end(args);

        fprintf(g_skill_log, "\n");
        fflush(g_skill_log);
    }

	SkillWrapper::SkillWrapper()
	{
		this->prayspeed = 0;
		this->commonlevel = 0;
		this->rejectrate = 0;
		this->dec_skill_level = 0;
		this->curr_cast_speed_rate = 0;
		this->dark_skill_cd_adjust = 0;
		this->light_skill_cd_adjust = 0;
		this->record_map_id = -1;
	}

	char SkillWrapper::GetType(unsigned int id)
	{
		skill_log("ENTER: SkillWrapper::GetType id=%u", id);
		const SkillStub* stub = GNET::SkillStub::GetStub(id);
		if (stub)
			return stub->GetType();
		else
			return -1;
	}

	char SkillWrapper::RangeType(unsigned int id)
	{
		skill_log("ENTER: SkillWrapper::RangeType id=%u", id);
		const SkillStub*  stub = GNET::SkillStub::GetStub(id);
		if (stub)
			return stub->rangetype;
		else
			return -1;
	}

    int SkillWrapper::Learn(unsigned int id, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::Learn id=%u", id);
        Skill* skill = Skill::Create(id);
        if (!skill)
            return -1;
        if (GNET::IsForbidSkill(id))
            return -1;
        int level = 1;
        std::map<unsigned int, GNET::SkillWrapper::PersistentData>::iterator it = this->map.find(id);
        if (it !=map.end())
        {
            if ((it->second.mask & 2) != 0)
                level = 1;
            else
                level = it->second.baselevel
                + 1;
        }
        int max = skill->GetMaxlevel();
        if (level <= max)
        {
            skill->SetLevel(level);
            SetSkillTalent(skill, player, 0);
            PlayerWrapper w_player(player, 2, skill, 0, 0);
            if (skill->Learn())
            {
                if (it == this->map.end())
                {
                    // First time learning this skill — create the map entry
                    PersistentData data(0);
                    it = map.insert(std::make_pair(id, data)).first;
                    // fall through to finish learn
                }
                it->second.baselevel = level;
                it->second.mask = 0;
                int real = this->commonlevel + level + it->second.actilevel;
                if (real > max)
                    real = max;
                int oldlevel = it->second.reallevel;
                it->second.reallevel = real;
                player.SendClientLearnSkill(id, level);
                skill_log("Learn: id=%u level=%d real=%d oldlevel=%d", id, level, real, oldlevel);
                OnTalentChange(player, id, oldlevel, real);
                if (skill->GetLevel() == skill->GetMaxLearn())
                {
                    OnAddSkill(skill->GetId(), player);
                }
                skill->Destroy();
                return level;
            }
            else
            {
                skill->Destroy();
                return -1;
            }
        }
        else
        {
            skill->Destroy();
            return -1;
        }
    }

    int SkillWrapper::Forget(bool all, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::Forget all=%d", (int)all);
        int sp_0 = 0;
        std::map<unsigned int, GNET::SkillWrapper::PersistentData>::iterator it= map.begin();
        while (it != map.end())
        {
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if ((all || stub->occupation == player.GetClass())
                            && (!all || stub->occupation != 147)
                            && !stub->talent_type
                            && !stub->clearmask)
                        {
                            if (stub->type == 11)
                            {
                                UndoPassive(it->first, it->second.reallevel, player);
                            }
                            commonlevel = this->commonlevel;
                            int real = it->second.actilevel + commonlevel;
                            if (real < it->second.reallevel)
                                it->second.reallevel = real;
                            sp_0 += it->second.baselevel;
                            it->second.baselevel = 0;
                            OnRemoveSkill(it->first, player);
                        }
                    }
                }
            }
            ++it;
        }
        semap.clear();
        if (sp_0)
        {
            player.ModifySkillPoint(sp_0);
            player.UpdateAllProp();
        }
        return sp_0;
    }

    //5
    int SkillWrapper::ForgetTalent(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ForgetTalent");
        int tp = 0;
        std::map<unsigned int, GNET::SkillWrapper::PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (! it->second.mask)
            {
                if ( it->second.baselevel)
                {
                    const SkillStub* stub = GNET::SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if (stub->talent_type)
                        {
                            if (stub->type == 11)
                            {
                                UndoPassive(it->first, it->second.reallevel, player);
                            }
                            commonlevel = this->commonlevel;
                            int real = it->second.actilevel
                                + commonlevel;
                            int oldlevel = it->second.reallevel;
                            if (real < it->second.reallevel)
                                it->second.reallevel = real;
                            tp += it->second.baselevel;
                            it->second.baselevel = 0;
                            OnTalentChange(player, it->first, oldlevel, it->second.reallevel);
                        }
                    }
                }
            }
            ++it;
        }
        return tp;
    }

    void SkillWrapper::ForgetCulSkills(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ForgetCulSkills");
        std::map<unsigned int, GNET::SkillWrapper::PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (! it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if (stub->occupation == 148 || stub->occupation == 149 || stub->occupation == 150)
                        {
                            if (stub->type == 11)
                            {
                                UndoPassive(it->first, it->second.reallevel, player);
                            }
                            int real = it->second.actilevel + commonlevel;
                            if (real < it->second.reallevel)
                                it->second.reallevel = real;
                            it->second.baselevel = 0;
                        }
                    }
                }
            }
            ++it;
        }
    }

    void SkillWrapper::ForgetDeitySkills(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ForgetDeitySkills");
        std::map<unsigned int, GNET::SkillWrapper::PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (! it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = GNET::SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if (!stub->talent_type && (stub->occupation == 167 || stub->occupation == 168 || stub->occupation == 169))
                        {
                            if (stub->type == 11)
                            {
                                UndoPassive(it->first, it->second.reallevel, player);
                            }
                            int real = it->second.actilevel + commonlevel;
                            if (real < it->second.reallevel)
                                it->second.reallevel = real;
                            it->second.baselevel = 0;
                        }
                    }
                }
            }
            ++it;
        }

    }

    void SkillWrapper::OnAddSkill(ID skill_id, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::OnAddSkill skill_id=%u", (unsigned int)skill_id);
        switch (skill_id)
        {
        case 0xF9:
            player.AddFilter(new filter_guiwang_addanticrit(player));
            break;
        case 0xFB:
            player.AddFilter(new filter_guiwang_addmaxhp(player));
            break;
        case 0x102:
            player.AddFilter(new filter_guiwang_adddamage(player));
            break;
        case 0x104:
            player.AddFilter(new filter_guiwang_addskillattack(player));
            break;
        case 0x105:
            player.AddFilter(new filter_guiwang_addtenaciy(player));
            break;
        default:
            return;
        }
    }

    void SkillWrapper::OnRemoveSkill(ID skill_id, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::OnRemoveSkill skill_id=%u", (unsigned int)skill_id);
        switch (skill_id)
        {
        case 0xF9u:
            player.RemoveFilter(FILTER_GUIWANG_ADDANTICRIT);
            break;
        case 0xFBu:
            player.RemoveFilter(FILTER_GUIWANG_ADDMAXHP);
            break;
        case 0x102u:
            player.RemoveFilter(FILTER_GUIWANG_ADDDAMAGE);
            break;
        case 0x104u:
            player.RemoveFilter(FILTER_GUIWANG_ADDSKILLATTACK);
            break;
        case 0x105u:
            player.RemoveFilter(FILTER_GUIWANG_ADDTENACIY);
            break;
        default:
            return;
        }
    }

    //10
    int SkillWrapper::Upgrade(ID id, unsigned int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::Upgrade id=%u level=%u", (unsigned int)id, level);
        if (!id)
            return 0;

        std::map<unsigned int,SkillWrapper::PersistentData>::iterator it = map.find(id);
        if (it != map.end())
        {
            it->second.actilevel += level;
            const SkillStub *stub = SkillStub::GetStub(id);
            if (stub)
            {
                int real = 0;
                if (it->second.baselevel)
                {
                    commonlevel = this->commonlevel;
                    real = it->second.actilevel + it->second.baselevel + commonlevel;
                }
                if (real > stub->maxlevel)
                    real = stub->maxlevel;
                int oldlevel = it->second.reallevel;
                it->second.reallevel = real;
                OnTalentChange(player, id, oldlevel, real);
                player.SendClientSkillAddon(id, it->second.actilevel);
                return 0;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            PersistentData data(0);
            data.baselevel = 0;
            data.reallevel = 0;
            data.actilevel = level;
            data.mask = 2;

            map[id].baselevel = data.baselevel;
            map[id].cooltime = data.cooltime;
            player.SendClientSkillAddon(id, level);
            return 0;
        }
    }

    int SkillWrapper::Degrade(ID id, unsigned int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::Degrade id=%u level=%u", (unsigned int)id, level);
        if (id)
        {
            std::map<unsigned int,PersistentData>::iterator it = map.find(id);
            if (it != map.end() )
            {
                it->second.actilevel -= level;
                int real = 0;
                if (it->second.baselevel)
                {
                    commonlevel = this->commonlevel;
                    real = it->second.actilevel + it->second.baselevel + commonlevel;
                }
                int oldlevel = it->second.reallevel;
                if (real != it->second.reallevel)
                    it->second.reallevel = real;
                OnTalentChange(player, id, oldlevel, real);
                player.SendClientSkillAddon(id, it->second.actilevel);
            }
        }
        return 0;
    }

    int SkillWrapper::InsertSkill(ID id, unsigned int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::InsertSkill id=%u level=%u", (unsigned int)id, level);
        const SkillStub* stub = SkillStub::GetStub(id);
        if (!stub)
            return -1;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->first == id)
            {
                if (it->second.mask != 2 || it->second.baselevel)
                {
                    return -1;
                }
                it->second.baselevel = level;
                int real = it->second.actilevel + level + this->commonlevel;
                if (real > stub->maxlevel)
                    real = stub->maxlevel;
                it->second.reallevel = real;
                player.SendClientExtraSkill(id, level);
                return 0;
            }
            it++;
        }
        PersistentData data(0);
        data.baselevel = level;
        data.mask = 2;
        int real = level + this->commonlevel;
        if (real > stub->maxlevel)
            real = stub->maxlevel;
        data.reallevel = real;

        int cooltime = data.cooltime;
        map[id].baselevel = data.baselevel;
        map[id].cooltime = cooltime;
        player.SendClientExtraSkill(id, level);
        return 0;
    }

    int SkillWrapper::RemoveSkill(ID id, unsigned int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::RemoveSkill id=%u level=%u", (unsigned int)id, level);
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->first == id
                && (it->second.mask & 2) != 0)
            {
                it->second.baselevel = 0;
                it->second.reallevel = 0;
                player.SendClientExtraSkill( id, 0);
                return 0;
            }
            ++it;
        }
        return 0;

    }

    int SkillWrapper::InsertSkillPermament(ID id, unsigned int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::InsertSkillPermament id=%u level=%u", (unsigned int)id, level);
        const SkillStub *stub = SkillStub::GetStub(id);
        if (!stub)
            return -1;
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        if (it == map.end() )
        {
            if (it->second.mask == 32
                && level
                && level > it->second.baselevel
                || it->second.mask == 2)
            {
                it->second.mask = 32;
                it->second.baselevel = level;
                int real = it->second.actilevel + level + this->commonlevel;
                if (real > stub->maxlevel)
                    real = stub->maxlevel;
                it->second.reallevel = real;
                player.SendClientAddonSkillPermanent(id, level);
                return 0;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            PersistentData data(0);
            data.baselevel = level;
            data.mask = 32;
            int real = level + this->commonlevel;
            if (real > stub->maxlevel)
                real = stub->maxlevel;
            data.reallevel = real;
            int cooltime = data.cooltime;
            map[id].baselevel = data.baselevel;
            map[id].cooltime = cooltime;
            player.SendClientAddonSkillPermanent(id, level);
            return 0;
        }
    }

    //15
    int SkillWrapper::InsertTriggerSkill(ID id, unsigned int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::InsertTriggerSkill id=%u level=%u", (unsigned int)id, level);
        const SkillStub* stub = SkillStub::GetStub(id);
        if (!stub || !stub->trigger)
            return -1;
        if (stub->maxlearn > 0)
        {
            std::map<unsigned int, PersistentData>::iterator it = map.find(id);
            bool find = 0;
            if (it != map.end() && it->second.baselevel)
            {
                find = 1;
            }
            if (find)
                trigger_active_map[id] = 1;
            return 0;
        }
        else
        {
            std::map<unsigned int, PersistentData>::iterator it = map.find(id);
            if (it != map.end())
            {
                if (it->second.mask == 16
                    && !it->second.baselevel)
                {
                    it->second.baselevel = level;
                    int real = level + it->second.actilevel;
                    if (real > stub->maxlevel)
                        real = stub->maxlevel;
                    it->second.reallevel = real;
                    player.SendClientExtraSkill(id, level);
                    trigger_active_map[id] = 1;
                    return 0;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                PersistentData data(0);
                data.baselevel = level;
                data.mask = 16;
                int real = level;
                if (level > stub->maxlevel)
                    real = stub->maxlevel;
                data.reallevel = real;

                int cooltime = data.cooltime;
                map[id].baselevel = data.baselevel;
                map[id].cooltime = cooltime;
                player.SendClientExtraSkill(id, level);
                trigger_active_map[id] = 1;
                return 0;
            }
        }
    }

    int SkillWrapper::ClearTriggerSkill(ID id, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ClearTriggerSkill id=%u", (unsigned int)id);
        const SkillStub* stub = SkillStub::GetStub(id);
        if (!stub || !stub->trigger)
            return -1;
        if (stub->maxlearn > 0)
        {
            std::map<unsigned int, bool>::iterator it = trigger_active_map.find(id);
            if (it != trigger_active_map.end())
            {
                trigger_active_map.erase(it++);
            }
            return 0;
        }
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        bool find = 0;
        if (it != map.end() && (it->second.mask & 0x10) != 0)
        {
            find = 1;
        }
        if (!find)
            return 0;
        it->second.baselevel = 0;
        it->second.reallevel = 0;
        player.SendClientExtraSkill(id, 0);
        std::map<unsigned int, bool>::iterator mit = trigger_active_map.find(id);
        if (mit != trigger_active_map.end())
        {
            trigger_active_map.erase(mit++);
        }
        return 0;
    }

    int SkillWrapper::InsertSysDeliveredSkill(ID id, unsigned int level, object_interface player, bool sendClient)
    {
        skill_log("ENTER: SkillWrapper::InsertSysDeliveredSkill id=%u level=%u", (unsigned int)id, level);
        const SkillStub* stub = SkillStub::GetStub(id);
        if (!stub)
            return -1;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->first == id)
            {
                if (it->second.mask != 64 || it->second.baselevel)
                {
                    return -1;
                }
                it->second.baselevel = level;
                int real = level + it->second.actilevel;
                if (real > stub->maxlevel)
                    real = stub->maxlevel;
                it->second.reallevel = real;
                if (sendClient)
                    player.SendClientExtraSkill(id, level);
                return 0;
            }
            ++it;
        }
        PersistentData data(0);
        data.baselevel = level;
        data.mask = 64;
        int real = level;
        if (level > stub->maxlevel)
            real = stub->maxlevel;
        data.reallevel = real;

        int cooltime = data.cooltime;
        map[id].baselevel = data.baselevel;
        map[id].cooltime = cooltime;
        if (sendClient)
            player.SendClientExtraSkill(id, level);
        return 0;
    }

    int SkillWrapper::ClearSysDeliveredSkill(ID id, object_interface player, bool sendClient)
    {
        skill_log("ENTER: SkillWrapper::ClearSysDeliveredSkill id=%u", (unsigned int)id);
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->first == id
                && (it->second.mask & 0x40) != 0)
            {
                it->second.baselevel = 0;
                it->second.reallevel = 0;
                if (sendClient)
                    player.SendClientExtraSkill(id, 0);
                return 0;
            }
            ++it;
        }
        return 0;
    }

    int SkillWrapper::ClearSpouseSkill(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ClearSpouseSkill");
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            const SkillStub*stub = SkillStub::GetStub(it->first);
            if (stub && stub->occupation == 152)
            {
                if (stub->type == 11)
                {
                    int reallevel = it->second.reallevel;
                    UndoPassive(it->first, reallevel, player);
                }
                it->second.baselevel = 0;
                it->second.reallevel = 0;
            }
            ++it;
        }
        return 0;
    }

    //20
    int SkillWrapper::Condition(ID id, object_interface player, const XID* target, int size)
    {
        skill_log("ENTER: SkillWrapper::Condition id=%u", (unsigned int)id);
        int level = GetSkillLevel(id);
        if (level <= 0)
            return -1;
        Skill* skill = GNET::Skill::Create(id);
        if (!skill)
            return 1;
        PlayerWrapper w_player(player, 0, skill, target, size);
        skill->SetLevel(level);
        int ret = skill->Condition();
        if (!ret)
            ret = player.TestCoolDown(id + 1024) == 0;
        if (skill->IsCycle() && player.IsInCircleOfDoom())
        {
            return 1;
        }
        else
        {
            skill->Destroy();
            return ret;
        }
    }

    int SkillWrapper::StartSkill(SKILL::Data& skilldata, object_interface player, const XID* target, int size, int& next)
    {
        skill_log("ENTER: SkillWrapper::StartSkill(XID) id=%u", (unsigned int)skilldata.id);
        next = -1;
        int level = 0;
        if (skilldata.item_id < 0 || skilldata.item_index < 0)
            level = GetSkillData(skilldata.id, skilldata.level, skilldata.cooltime, player);
        else
            level = skilldata.level;
        skill_log("StartSkill: GetSkillData returned level=%d", level);
        if (level <= 0)
            return -1;
        skill_log("StartSkill: calling Skill::Create id=%u", (unsigned int)skilldata.id);
        Skill* skill = Skill::Create(skilldata.id);
        skill_log("StartSkill: Skill::Create returned %s", skill ? "ok" : "null");
        if (!skill)
            return -1;
        PlayerWrapper w_player(player, 0, skill, target, size);
        skill->SetLevel(level);
        skill->SetData(&skilldata);
        skill_log("StartSkill: calling SetSkillTalent, skill=%p", (void*)skill);
        SetSkillTalent( skill, player, 1);
        skill_log("StartSkill: SetSkillTalent returned");
        int Occupation = skill->GetOccupation();
        skill_log("StartSkill: Occupation=%d", Occupation);
        int playerOccup = w_player.GetOccupation();
        skill_log("StartSkill: playerOccup=%d skillOccup=%d IsDiffrent=%d", playerOccup, Occupation, (int)IsDiffrentOccupClass(playerOccup, Occupation));
        if (IsDiffrentOccupClass(playerOccup, Occupation))
        {
            skill_log("StartSkill: return -1 IsDiffrentOccupClass");
            skill->Destroy();
            return -1;
        }
        else
        {
            if (skill->IsTrigger())
            {
                std::map<unsigned int, bool>::iterator it = trigger_active_map.find(skilldata.id);
                if (it == trigger_active_map.end())
                {
                    skill_log("StartSkill: return -1 IsTrigger not in map");
                    return -1;
                }
            }
            int ret = skill->Condition();
            int coolOk = player.TestCoolDown(skilldata.id + 1024);
            skill_log("StartSkill: Condition=%d TestCoolDown=%d", ret, coolOk);
            if (ret || !coolOk)
            {
                skill_log("StartSkill: return -1 Condition or CoolDown");
                skill->Destroy();
                return -1;
            }
            skill_log("StartSkill: IsCycle=%d IsInCircleOfDoom=%d", (int)skill->IsCycle(), (int)player.IsInCircleOfDoom());
            if ( (skill->IsCycle() && player.IsInCircleOfDoom())
                || (skill->IsCycle() && player.CircleOfDoomPrepare( skill->GetRadius(), 1, skill->GetCoverage(), skilldata.id) == 0))
            {
                skill_log("StartSkill: return -1 IsCycle check");
                return -1;
            }
            SetSucceedSkillColor( skill, player);
            if (skill->IsComboSkill())
            {
                std::map<unsigned int, SkillElems>::iterator it = semap.find(skilldata.id);
                if (it != semap.end())
                {
                    skill->SetSkillElems(it->second.ids);
                    SetSkillElemsNum(skill, it->second.ids);
                }
            }
            this->curr_cast_speed_rate = this->prayspeed;
            skill_log("StartSkill: calling FirstRun");
            ret = skill->FirstRun(next, 0);
            skill_log("StartSkill: FirstRun returned ret=%d next=%d", ret, next);
            if (curr_cast_speed_rate > 70)
                curr_cast_speed_rate = 70;
            ret = ret * 0.01 * (100 - curr_cast_speed_rate) + 0.01;
            next = next * 0.01 * (100 - curr_cast_speed_rate) + 0.01;
            if (ret >= 0)
            {
                char stype = skill->GetType();
                if (stype == 1 || stype == 3)
                    player.EnterCombatState();
                if (skill->IsCastSelf())
                {
                    player.SendClientMsgSkillCasting(1, &player.GetSelfID(), skilldata.id, ret,
                        level, 0, curr_cast_speed_rate, skilldata.pos, skilldata.spirit_index);
                }
                else
                {
                    if (size > 0)
                        player.SendClientMsgSkillCasting(size, target, skilldata.id, ret, level, 0, curr_cast_speed_rate, skilldata.pos, skilldata.spirit_index);
                    else
                        player.SendClientMsgSkillCasting(size, new XID(0,0), skilldata.id, ret, level, 0, curr_cast_speed_rate, skilldata.pos, skilldata.spirit_index);
                }
                if (skill->IsWarmup())
                {
                    const SkillStub::State * state = skill->GetState( 1);
                    if (state)
                    {
                        int time = state->GetTime(skill) * 0.01 * (100 - curr_cast_speed_rate) + 0.01;
                        if (skill->IsCastSelf())
                        {
                            player.SendClientMsgSkillCasting(1, &player.GetSelfID(), skilldata.id, time, skilldata.level, 1, curr_cast_speed_rate, skilldata.pos, skilldata.spirit_index);
                        }
                        else
                        {
                            if (size > 0)
                                player.SendClientMsgSkillCasting(size, target, skilldata.id, time, skilldata.level, 1, curr_cast_speed_rate, skilldata.pos, skilldata.spirit_index);
                            else
                                player.SendClientMsgSkillCasting(size, new XID(0,0), skilldata.id, time, skilldata.level, 1, curr_cast_speed_rate, skilldata.pos, skilldata.spirit_index);
                        }
                    }
                }
            }
            skill->Destroy();
            return ret;
        }

    }

    int SkillWrapper::Run(SKILL::Data& skilldata, object_interface player, const XID* target, int size, int& next)
    {
        skill_log("ENTER: SkillWrapper::Run(XID) id=%u", (unsigned int)skilldata.id);
        Skill* skill = GNET::Skill::Create(skilldata.id);
        if (!skill)
            return -1;
        PlayerWrapper w_player(player, 0, skill, target, size);
        skill->SetLevel(skilldata.level);
        skill->SetData(&skilldata);
        SetSkillTalent(skill, player, 1);
        SetSucceedSkillColor(skill, player);
        if (skill->IsComboSkill())
        {
            std::map<unsigned int, SkillElems>::iterator it = semap.find(skilldata.id);
            if (it != semap.end())
            {
               skill->SetSkillElems(it->second.ids);
               SetSkillElemsNum(skill, it->second.ids);
            }
        }
        const SkillStub *Stub = skill->GetStub();
        if (!Stub || Stub->ItemCondition(skill))
        {
            int elems = skill->Run(next);
            curr_cast_speed_rate = this->curr_cast_speed_rate;
            if (curr_cast_speed_rate > 70)
                curr_cast_speed_rate = 70;
            elems = elems * 0.01 * (100 - curr_cast_speed_rate) + 0.01;
            next = next * 0.01 * (100 - curr_cast_speed_rate) + 0.01;
            if (skilldata.stateindex == 1)
            {
                skilldata.skippable = skill->IsWarmup();
                if (skilldata.skippable)
                    skill->SetCharging( elems);
            }
            else
            {
                skilldata.skippable = 0;
            }
            skill->Destroy();
            if (!w_player.GetSuccess())
            {
                elems = -1;
                player.SendClientMsgSkillInterrupted( 2, skilldata.spirit_index);
            }
            return elems;
        }
        else
        {
            skill->Destroy();
            return -1;
        }
    }

    int SkillWrapper::StartSkill(SKILL::Data& skilldata, object_interface player, const A3DVECTOR& target, int& next)
    {
        skill_log("ENTER: SkillWrapper::StartSkill(A3DVECTOR) id=%u", (unsigned int)skilldata.id);
        int level = 0;
        if (skilldata.item_id < 0 || skilldata.item_index < 0)
            level = GetSkillData(skilldata.id, skilldata.level, skilldata.cooltime, player);
        else
            level = skilldata.level;
        if (level <= 0)
            return -1;
        Skill* skill = GNET::Skill::Create(skilldata.id);
        if (!skill)
            return -1;
        next = -1;
        PlayerWrapper w_player(player, 0, skill, 0, 0);
        skill->SetLevel(level);
        skill->SetData(&skilldata);
        SetSkillTalent(skill, player, 1);
        if (skill->IsTrigger())
        {
            std::map<unsigned int, bool>::iterator it = trigger_active_map.find(skilldata.id);
            if (it == trigger_active_map.end())
            {
                return -1;
            }
        }
        if (skill->Condition() || !player.TestCoolDown(skilldata.id + 1024))
        {
            skill->Destroy();
            return -1;
        }
        if (skill->IsCycle() && player.IsInCircleOfDoom())
            return -1;
        if (skill->IsCycle())
        {
            player.CircleOfDoomPrepare(skill->GetRadius(), 1, skill->GetCoverage(), skilldata.id);
        }
        float range = skill->GetPraydistance();
        if (player.GetPos().squared_distance(target) > range * range)
        {
            skill->Destroy();
            return -1;
        }
        player.SkillMove(target);
        int ret = skill->FirstRun( next, this->prayspeed);
        if (ret >= 0)
            player.SendClientCastPosSkill(target, skilldata.id, ret, level);
        skill->Destroy();
        return ret;

    }

    int SkillWrapper::Run(SKILL::Data& skilldata, object_interface player, const A3DVECTOR& target, int& next)
    {
        skill_log("ENTER: SkillWrapper::Run(A3DVECTOR) id=%u", (unsigned int)skilldata.id);
        Skill* skill = GNET::Skill::Create(skilldata.id);
        if (!skill)
            return -1;
        PlayerWrapper w_player(player, 0, skill, 0, 0);
        skill->SetLevel(skilldata.level);
        skill->SetData(&skilldata);
        SetSkillTalent(skill, player, 1);
        SetSucceedSkillColor(skill, player);
        if (skill->IsComboSkill())
        {
            std::map<unsigned int, SkillElems>::iterator it = semap.find(skilldata.id);
            if (it!= semap.end())
            {
                skill->SetSkillElems(it->second.ids);
                SetSkillElemsNum(skill, it->second.ids);
            }
        }
        const SkillStub* Stub = skill->GetStub();
        if (!Stub || Stub->ItemCondition(skill))
        {
            int elems = skill->Run(next);
            skill->Destroy();
            if (!w_player.GetSuccess())
            {
                elems = -1;
                player.SendClientMsgSkillInterrupted(2, -1);
            }
            return elems;
        }
        else
        {
            skill->Destroy();
            return -1;
        }

    }

    //25
    bool SkillWrapper::Interrupt(SKILL::Data& skilldata, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::Interrupt id=%u", (unsigned int)skilldata.id);
        Skill* skill = Skill::Create(skilldata.id);
        if (!skill)
            return 1;
        PlayerWrapper w_player(player, 0, skill, 0, 0);  //����������ûʹ��  ��    haokaixin
        skill->SetLevel(skilldata.level);
        skill->SetData(&skilldata);
        bool ret = skill->Interrupt();
        skill->Destroy();
        if (ret)
            player.SendClientMsgSkillInterrupted( 1, skilldata.spirit_index);
        return ret;

    }

    bool SkillWrapper::Cancel(SKILL::Data& skilldata, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::Cancel id=%u", (unsigned int)skilldata.id);
        Skill* skill = Skill::Create(skilldata.id);
        if (!skill)
            return 1;
        PlayerWrapper w_player(player, 0, skill, 0, 0);  //
        skill->SetLevel(skilldata.level);
        skill->SetData(&skilldata);
        bool ret = skill->Cancel();
        if (skill->IsCycle())
            player.CircleOfDoomStop();
        skill->Destroy();
        if (ret)
            player.SendClientMsgSkillInterrupted(2, skilldata.spirit_index);
        return ret;
    }

    int SkillWrapper::Continue(SKILL::Data& skilldata, object_interface player, const XID* target, int size, int& next, int elapse)
    {
        skill_log("ENTER: SkillWrapper::Continue id=%u elapse=%d", (unsigned int)skilldata.id, elapse);
        Skill* skill = Skill::Create(skilldata.id);
        if (!skill)
            return 1;
        PlayerWrapper w_player(player, 0, skill, 0, 0);  //
        skill->SetLevel(skilldata.level);
        skill->SetData(&skilldata);
        skill->SetCharging(elapse);
        SetSkillTalent( skill, player, 1);
        SetSucceedSkillColor( skill, player);
        if (skill->IsComboSkill())
        {
            std::map<unsigned int, SkillElems>::iterator it = semap.find(skilldata.id);
            if (it != semap.end())
            {
                skill->SetSkillElems(it->second.ids);
                SetSkillElemsNum(skill, it->second.ids);
            }
        }
        next = -1;
        if (skill->GetStub()->ItemCondition(skill))
        {
            int elems = skill->Run(next);
            curr_cast_speed_rate = this->curr_cast_speed_rate;
            if (curr_cast_speed_rate > 70)
                curr_cast_speed_rate = 70;
            elems = (int)(elems * 0.01 * (100 - curr_cast_speed_rate) + 0.01);
            next = (int)(next * 0.01 * (100 - curr_cast_speed_rate) + 0.01);
            if (skilldata.stateindex == 1)
            {
                skilldata.skippable = skill->IsWarmup();
                if (skilldata.skippable)
                    skill->SetCharging(elems);
            }
            else
            {
                skilldata.skippable = 0;
            }
            skill->Destroy();
            player.SendClientMsgSkillContinue(skilldata.id, skilldata.spirit_index);
            return (int)elems;
        }
        else
        {
            skill->Destroy();
            return -1;
        }
    }

    int SkillWrapper::InstantSkill(SKILL::Data& skilldata, object_interface player, const XID* target, int size, const A3DVECTOR& chargeDestPos, const XID& chargeTarget)
    {
        skill_log("ENTER: SkillWrapper::InstantSkill id=%u", (unsigned int)skilldata.id);
        int level = 0;
        if (skilldata.item_id < 0 || skilldata.item_index < 0)
            level = GetSkillData(skilldata.id, skilldata.level, skilldata.cooltime, player);
        else
            level = skilldata.level;
        if (level <= 0)
            return -1;
        Skill* skill = GNET::Skill::Create(skilldata.id);
        if (skill && skill->IsInstant())
        {
            PlayerWrapper w_player(player, 0, skill, target, size);
            skill->SetLevel( level);
            skill->SetData(&skilldata);
            SetSkillTalent( skill, player, 1);
            skill->SetChargeTarget( chargeTarget);
            skill->SetChargeDestPos( chargeDestPos);
            if (skill->IsTrigger() && trigger_active_map.find(skilldata.id) == trigger_active_map.end())
            {
                return -1;
            }
            else
            {
                int ret = skill->Condition();
                if (!ret &&  player.TestCoolDown( skilldata.id + 1024))
                {
                    SetSucceedSkillColor(skill, player);
                    if (skill->IsComboSkill())
                    {
                        std::map<unsigned int, SkillElems>::iterator it = semap.find(skilldata.id);
                        if (it != semap.end())
                        {
                            skill->SetSkillElems(it->second.ids);
                            SetSkillElemsNum(skill, it->second.ids);
                        }
                    }
                    if (skill->InstantRun())
                    {
                        char stype = skill->GetType();
                        if (stype == 1 || stype == 3)
                            player.EnterCombatState();

                        if (size > 0)
                            player.SendClientInstantSkill(size, target, skilldata.id, level, skilldata.pos, skilldata.spirit_index);
                        else
                            player.SendClientInstantSkill(size, new XID(0,0), skilldata.id, level, skilldata.pos, skilldata.spirit_index);
    
                        if (player.CanCharge(chargeTarget,chargeDestPos, skill->GetChargeType(), skill->GetChargeDist()))
                        {
                            player.Charge(chargeTarget, chargeDestPos, skill->GetChargeType() - 1);
                        }
                    }
                    skill->Destroy();
                    return ret;
                }
                else
                {
                    skill->Destroy();
                    return -1;
                }
            }
        }
        else
        {
            if (skill)
                skill->Destroy();
            return -1;
        }
    }

    int SkillWrapper::CastRune(SKILL::Data& skilldata, object_interface player, int level, int coolid)
    {
        skill_log("ENTER: SkillWrapper::CastRune id=%u level=%d", (unsigned int)skilldata.id, level);
        Skill* skill = GNET::Skill::Create(skilldata.id);
        if (!skill || !skill->IsRune())
            return -1;
        PlayerWrapper w_player(player, 1, skill, 0, 0);
        skill->SetLevel(level);
        skill->SetData(&skilldata);
        if ((skilldata.item_id == -1 || skill->GetItem1Id() != skilldata.item_id && skill->GetItem2Id() != skilldata.item_id)
            && skill->GetStub()->ItemCondition(skill))
        {
            skill->StateAttack();
            w_player.ConsumeItem();
            player.SetCoolDown(coolid, skill->GetCooltime());
            skill->Destroy();
            return 0;
        }
        else
        {
            return -1;
        }
    }

    //30
    int SkillWrapper::CastExtraSkillToSelf(SKILL::Data& skilldata, object_interface player, int level, const attack_msg* original_attack)
    {
        skill_log("ENTER: SkillWrapper::CastExtraSkillToSelf id=%u level=%d", (unsigned int)skilldata.id, level);
        Skill* skill = Skill::Create(skilldata.id);
        if (!skill)
            return -1;
        PlayerWrapper w_player(player, 1, skill, 0, 0);
        skill->SetLevel(level);
        skill->SetData(&skilldata);
        skill->GetPlayer()->SetCaster(original_attack->ainfo.attacker);
        skill->SetMessage(original_attack);
        skill->StateAttack();
        int aggro = skill->GetEnmity();
        if (aggro > 0)
            w_player.SetEnmity(aggro);
        skill->Destroy();
        return 0;
    }

    void SkillWrapper::LoadDatabase(object_interface player, archive& ar)
    {
        skill_log("ENTER: SkillWrapper::LoadDatabase");
        char version = 0;
        unsigned int size = 0;
        unsigned int id;
        map.clear();
        ar >> version >> size;
		skill_log("Load skill data, version=%d, size=%u", version, size);
        int forbidSp = 0;
        while (size)
        {
            PersistentData data(0);
            ar >> id;
            skill_log("Load skill data, id: %d", id);
            // FIX: StoreDatabase writes only baselevel as char (1 byte).
            // The old pop_back(&data, sizeof(data)) read 8 bytes and overran into
            // adjacent skill records, corrupting every subsequent id.
            ar >> data.baselevel;
            data.reallevel = data.baselevel;
            if (GNET::IsForbidSkill(id))
            {
                skill_log("GNET::IsForbidSkill(%d)", id);
                forbidSp += data.baselevel;
            }
            else
            {
                skill_log("GNET::IsForbidSkill Else");
                const SkillStub *stub = SkillStub::GetStub(id);
                if (!stub || IsSkillValidClass(player.GetClass(), stub->skill_class))
                {
                    skill_log("IsSkillValidClass class: %d", player.GetClass());
                    if (stub)
                    {
                        skill_log("Stub is TRUE");
                        if (stub->maxlearn == data.baselevel)
                        {
                            skill_log("Stub MaxLearn");
                            OnAddSkill(id, player);
                        }
                    }
                    map[id].baselevel = data.baselevel;
                    map[id].reallevel = data.reallevel;
                    map[id].cooltime = data.cooltime;
                }
                else
                {
                    skill_log("Stub Else");
                    forbidSp += data.baselevel;
                }
            }
            --size;
            skill_log("size: %d", size);
        }

        if (forbidSp > 0)
        {
            player.ModifySkillPoint(forbidSp);
            player.UpdateAllProp();
        }
        skill_log("forbidSp > 0");

        PersistentData data(0);
        data.baselevel = 1;
        data.reallevel = 1;
        data.actilevel = 0;
        data.mask = 1;

        skill_log("Teleport Assign");
        map[389].baselevel = data.baselevel;
        map[389].reallevel = data.reallevel;
        map[389].cooltime = data.cooltime;
        map[389].mask = data.mask;

        map[2354].baselevel = data.baselevel;
        map[2354].reallevel = data.reallevel;
        map[2354].cooltime = data.cooltime;
        map[2354].mask = data.mask;

        int stub = player.GetClass();
        if ((int)stub > 38 && (int)stub <= 43)
        {
            map[2356].baselevel = data.baselevel;
            map[2356].reallevel = data.reallevel;
            map[2356].cooltime = data.cooltime;
            map[2356].mask = data.mask;
        }
        if ((int)stub > 107 && (int)stub <= 112)
        {
            map[4934].baselevel = data.baselevel;
            map[4934].reallevel = data.reallevel;
            map[4934].cooltime = data.cooltime;
            map[4934].mask = data.mask;
        }
        if (version == 1)
        {
            ar >> size;
            while (size)
            {
                ar >> id;
                ar >> prfmap[id];
                --size;
            }
        }
        else if (version == 2)
        {
            skill_log("Version 2");
            ar >> size;
            skill_log("Version 2 after size");
            while (size)
            {
                skill_log("Version 2 while(size)");
                ar >> id;
                skill_log("Version 2 while(size) id: %d", id);
                ar >> prfmap[id];
                --size;
            }
            skill_log("Sled pyrvi while");
            ar >> size;
            while (size)
            {
                PersistentData data(0);
                ar >> id;
                // FIX: same as above — StoreDatabase writes 1 byte (char baselevel).
                ar >> data.baselevel;
                data.reallevel = data.baselevel;
                data.mask = 32;
                map[id].baselevel = data.baselevel;
                map[id].cooltime = data.cooltime;
                --size;
            }
            skill_log("Sled vtori while");
        }
        skill_log("Before XPSet");
        SetXPSkill(player.GetXPSkill());
    }

    void SkillWrapper::StoreDatabase(archive& ar)
    {
        skill_log("ENTER: SkillWrapper::StoreDatabase");
        size_t size = 0;
        size_t pskillSize = 0;
        // FIX: write version as char (1 byte) to match "ar >> (char)version" in LoadDatabase.
        // Original "ar << 2" wrote an int (4 bytes), misaligning all subsequent reads.
        ar << (char)2;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    if (! IsForbidSkill(it->first))
                        ++size;
                }
            }
            if (it->second.mask == 32 && it->second.baselevel)
            {
                if (! IsForbidSkill(it->first))
                    ++pskillSize;
            }
            ++it;
        }
        ar << size;
        it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    if (!IsForbidSkill(it->first))
                    {
                        ar << it->first;
                        // Store only baselevel (1 byte char) — matches "ar >> data.baselevel"
                        // in LoadDatabase. Cooltime is runtime state, not persisted.
                        ar << it->second.baselevel;
                    }
                }
            }
            ++it;
        }
        size = 0;
        std::map<unsigned int, int>::iterator it1 = prfmap.begin();
        while (1)
        {
            if (it1 == prfmap.end())
                break;
            if (it1->second)
                ++size;
            ++it1; // FIX: was "++it" (wrong iterator → infinite loop when prfmap non-empty)
        }
        ar << size;
        it1 = prfmap.begin();
        while (1)
        {
            if (it1 == prfmap.end())
                break;
            if (it1->second)
            {
                // FIX: was "ar << it->first; ar.push_back(&it->second, ...)"
                // Wrong iterator (it vs it1) and wrong type (PersistentData vs int).
                // LoadDatabase reads: ar >> id; ar >> prfmap[id] (int).
                ar << it1->first;
                ar << it1->second;
            }
            ++it1; // FIX: was "++it"
        }
        ar << pskillSize;
        it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->second.mask == 32)
            {
                if (it->second.baselevel)
                {
                    if (!IsForbidSkill(it->first))
                    {
                        ar << it->first;
                        ar << it->second.baselevel; // same 1-byte char format
                    }
                }
            }
            ++it;
        }
    }

    void SkillWrapper::StorePartial(archive& ar)
    {
        skill_log("ENTER: SkillWrapper::StorePartial");
        size_t size = 0;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->second.baselevel && (!it->second.mask || it->second.mask == 1))
            {
                ++size;
            }
            ++it;
        }
        skill_log("StorePartial: writing skill count=%u", (unsigned)size);
        ar << size;

        it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->second.baselevel && (!it->second.mask || it->second.mask == 1))
            {
                skill_log("StorePartial: writing skill id=%u baselevel=%d mask=%d", it->first, (int)it->second.baselevel, (int)it->second.mask);
                ar << (unsigned short)it->first;
                ar << it->second.baselevel;
            }
            ++it;
        }
        size = 0;
        std::map<unsigned int, int>::iterator it1 = prfmap.begin();
        
        while (1)
        {
            if (it1 == prfmap.end())
                break;
            if (it1->second)
                ++size;
            ++it1;
        }
        ar << size;

        it1 = prfmap.begin();
        while (1)
        {
            if (it1 == prfmap.end())
                break;
            if (it1->second)
            {
                ar << it1->first;
                ar << it1->second;
            }
            ++it1;
        }

    }

    void SkillWrapper::StorePartialAddonPermanent(archive& ar)
    {
        skill_log("ENTER: SkillWrapper::StorePartialAddonPermanent");
        size_t size = 0;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->second.baselevel)
            {
                if (it->second.mask == 32)
                    ++size;
            }
            ++it;
        }
        ar << size;

        it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            if (it->second.baselevel && it->second.mask == 32)
            {
                ar << (unsigned short)it->first;
                ar << it->second.baselevel;
            }
            ++it;
        }
    }

    //35
    int SkillWrapper::GetBaseLevel(ID id)
    {
        skill_log("ENTER: SkillWrapper::GetBaseLevel id=%u", (unsigned int)id);
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        if (it != map.end())
        {
            return it->second.baselevel;
        }
        else
        {
            return 0;
        }
    }

    int SkillWrapper::GetLevelSum(int cls)
    {
        skill_log("ENTER: SkillWrapper::GetLevelSum cls=%d", cls);
        int sum = 0;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            const SkillStub* stub = SkillStub::GetStub(it->first);
            if (stub && stub->occupation == cls && !stub->talent_type)
            {
                sum += it->second.baselevel;
            }
            ++it;
        }
        return sum;
    }

    bool SkillWrapper::Attack(object_interface victim, const XID& attacker, const A3DVECTOR& src, attack_msg& msg, bool inv, int damage, int is_crit)
    {
        skill_log("ENTER: SkillWrapper::Attack(attack_msg) skill=%d", msg.attached_skill.skill);
        int id = msg.attached_skill.skill;
        Skill* skill = Skill::Create(id);
        if (!skill)
            return 0;
        PlayerWrapper w_victim(victim, 1, skill, 0, 0);
        w_victim.SetInvader(inv);
        for (int i = 0; i <= 15; ++i)
            w_victim.SetVar( msg.skill_var[i], i);
        for (int i = 0; i <= 5; ++i)
            w_victim.SetResisProf(i, msg.resistance_proficiency[i]);
        skill->SetLevel(msg.attached_skill.level);
        skill->SetMessage(&msg);
        skill->SetAttackerpos( src);
        skill->SetRealDamage( damage);
        skill->SetIscrit( is_crit);
        SetSkillTalent( skill, msg.skill_modify);
        SetSucceedSkillColor( skill, msg.combo_color);
        if (skill->IsComboSkill())
            SetSkillElemsNum( skill, msg.skill_element);
        SKILL::Data data(id);
        data.forceattack = msg.force_attack;
        if (skill->IsTalisman())
        {
            for (int i = 0; i <= 7; ++i)
            {
                if (msg.talismanSkills[i] > 0)
                    data.talismanRefineSkillList.push_back(msg.talismanSkills[i]);
            }
            skill->SetData(&data);
        }

        skill->GetPlayer()->SetCaster( attacker);
        bool ret = skill->StateAttack();
        int i = skill->GetEnmity();
        if (i > 0)
            w_victim.SetEnmity( i);
        if (skill->GetType() != 1 && (skill->GetType() != 12 || w_victim.GetProbability() > 99.0))
        {
            w_victim.SendResult(attacker, w_victim.GetAtstate());
        }
        msg.attack_state |= w_victim.GetAtstate();
        float DropRate = skill->GetDropRate();
        victim.AdjustDropRate(DropRate);
        if (skill->IsComboSkill())
        {
            int elemsCount[10];
            memset(elemsCount, 0, sizeof(elemsCount));
            for (int k = 0; k < MAX_SKILL_ELEM_NUM; ++k)
            {
                unsigned short elem = msg.skill_element[k];
                if (elem)
                {
                    const SkillStub* Stub = skill->GetStub();
                    int elem_idx;
                    if (Stub->CheckSkillElem(elem, elem_idx))
                    {
                        Skill* elemskill = Skill::Create(elem);
                        if (elemskill)
                        {
                            PlayerWrapper vic(victim, 1, elemskill, 0, 0);
                            vic.SetInvader(inv);
                            for (int j = 0; j <= 15; ++j)
                                vic.SetVar(msg.skill_var[j], j);
                            for (int j = 0; j <= 5; ++j)
                                w_victim.SetResisProf(j, msg.resistance_proficiency[j]);
                            elemskill->SetLevel(msg.attached_skill.level);
                            elemskill->SetMessage( &msg);
                            elemskill->SetAttackerpos( src);
                            elemskill->SetRealDamage( damage);
                            elemskill->SetIscrit( is_crit);
                            SetSkillTalent( elemskill, msg.skill_modify);
                            SetSkillElemsNum( elemskill, msg.skill_element);
                            elemskill->SetTianhua1(2.0 - powf(0.5, elemsCount[elem_idx - 1]++));
                            elemskill->GetPlayer()->SetCaster(attacker);
                            elemskill->StateAttack();
                            elemskill->Destroy();
                        }
                    }
                }
            }
        }
        skill->Destroy();
        return ret;
    }

    bool SkillWrapper::Attack(object_interface victim, const XID& attacker, const A3DVECTOR& src, enchant_msg& msg, bool inv)
    {
        skill_log("ENTER: SkillWrapper::Attack(enchant_msg) skill=%d", msg.skill);
        Skill* skill = Skill::Create(msg.skill);
        if (!skill)
            return 0;
        if (!skill->IsFamily() || victim.GetFamilyID() == msg.ainfo.family_id && msg.ainfo.family_id)
        {
            PlayerWrapper w_victim(victim, 1, skill, 0, 0);
            w_victim.SetInvader( inv);
            for (int i = 0; i <= 15; ++i)
                w_victim.SetVar( msg.skill_var[i], i);
            for (int i = 0; i <= 5; ++i)
                w_victim.SetResisProf(i, msg.resistance_proficiency[i]);
            skill->SetLevel(msg.skill_level);
            skill->SetMessage( &msg);
            skill->SetAttackerpos( src);
            skill->SetAttackRange( msg.attack_range);
            SetSkillTalent( skill, msg.skill_modify);
            SetSucceedSkillColor( skill, msg.combo_color);
            if (skill->IsComboSkill())
                SetSkillElemsNum( skill, msg.skill_element);
            SKILL::Data data(msg.skill);
            data.forceattack = msg.force_attack;
            if (skill->IsTalisman())
            {
                for (int i = 0; i <= 7; ++i)
                {
                    if (msg.talismanSkills[i] > 0)
                        data.talismanRefineSkillList.push_back(msg.talismanSkills[i]);
                }
                skill->SetData(&data);
            }
            skill->GetPlayer()->SetCaster(attacker);
            bool ret = skill->StateAttack();
            int i = skill->GetEnmity();
            if (i > 0)
                w_victim.SetEnmity( i);
            if (skill->GetType() != 1
                && (skill->GetType() != 12 || w_victim.GetProbability() > 99.0))
            {
                w_victim.SendResult( attacker, w_victim.GetAtstate());
            }
            msg.attack_state |= w_victim.GetAtstate();
            float DropRate = skill->GetDropRate();
            victim.AdjustDropRate( DropRate);
            if (skill->IsComboSkill())
            {
                int elemsCount[10];
                memset(elemsCount, 0, sizeof(elemsCount));
                for (int j = 0; j < MAX_SKILL_ELEM_NUM; ++j)
                {
                    unsigned short elem = msg.skill_element[j];
                    if (elem)
                    {
                        int elem_idx;
                        if (skill->GetStub()->CheckSkillElem(elem, elem_idx))
                        {
                            Skill* elemskill = GNET::Skill::Create(elem);
                            if (elemskill)
                            {
                                PlayerWrapper vic(victim, 1, elemskill, 0, 0);
                                vic.SetInvader( inv);
                                for (int k = 0; k <= 15; ++k)
                                    vic.SetVar( msg.skill_var[k], k);
                                for (int l = 0; l <= 5; ++l)
                                    w_victim.SetResisProf(l, msg.resistance_proficiency[l]);
                                elemskill->SetLevel( msg.skill_level);
                                elemskill->SetMessage( &msg);
                                elemskill->SetAttackerpos( src);
                                SetSkillTalent( elemskill, msg.skill_modify);
                                SetSkillElemsNum( elemskill, msg.skill_element);
                                elemskill->SetTianhua1(2.0 - powf(0.5, elemsCount[elem_idx - 1]++));
                                elemskill->GetPlayer()->SetCaster(attacker);
                                elemskill->StateAttack();
                                elemskill->Destroy();
                            }
                        }
                    }
                }
            }
            skill->Destroy();
            return ret;
        }
        else
        {
            skill->Destroy();
            return 0;
        }

    }

    bool SkillWrapper::EventReset(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::EventReset");
        PlayerWrapper w_player( player, 2, 0, 0, 0);
        int cul = player.GetCultivation();
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            const SkillStub* stub = SkillStub::GetStub(it->first);
            if (stub)
            {
                if (stub->type == 11 && stub->GetEventFlag() == 1)
                {
                    if (!GNET::IsDiffrentOccupClass(w_player.GetOccupation(), stub->GetOccupation())
                        && (stub->occupation != 148 || (((unsigned char)cul ^ 1) & 1) == 0)
                        && (stub->occupation != 149 || (cul & 2) != 0)
                        && (stub->occupation != 150 || (cul & 4) != 0)
                        && (stub->occupation != 167 || (((unsigned char)cul ^ 1) & 1) == 0)
                        && (stub->occupation != 168 || (cul & 2) != 0)
                        && (stub->occupation != 169 || (cul & 4) != 0))
                    {
                        Skill* skill = Skill::Create(it->first);
                        if (skill)
                        {
                            SetSkillTalent(skill, player, 0);
                            w_player.SetSkill( skill);
                            skill->SetLevel(it->second.reallevel);
                            skill->SetPlayer(&w_player);
                            skill->TakeEffect(&w_player);
                            skill->Destroy();
                        }
                    }
                }
            }
            ++it;
        }
        return 1;
    }

    //40
    bool SkillWrapper::EventChange(object_interface player, int form)
    {
        skill_log("ENTER: SkillWrapper::EventChange form=%d", form);
        PlayerWrapper w_player(player, 2, 0, 0, 0);
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            const SkillStub *stub = SkillStub::GetStub(it->first);
            if (stub)
            {
                if (stub->type == 11 && stub->GetEventFlag() == 4)
                {
                    Skill* skill = Skill::Create(it->first);
                    if (skill)
                    {
                        SetSkillTalent(skill, player, 0);
                        w_player.SetSkill( skill);
                        skill->SetLevel(it->second.reallevel);
                        skill->SetPlayer(&w_player);
                        if (form == 1)
                            skill->TakeEffect(&w_player);
                        else
                            skill->UndoEffect(&w_player);
                        skill->Destroy();
                    }
                }
            }
            ++it;
        }
        return 1;
    }

    bool SkillWrapper::EventCloneExist(object_interface player, bool isCloneExist)
    {
        skill_log("ENTER: SkillWrapper::EventCloneExist isCloneExist=%d", (int)isCloneExist);
        PlayerWrapper w_player(player, 2, 0, 0, 0);
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            const SkillStub *stub = GNET::SkillStub::GetStub(it->first);
            if (stub)
            {
                if (stub->type == 11 && stub->GetEventFlag() == 8)
                {
                    Skill* skill = GNET::Skill::Create(it->first);
                    if (skill)
                    {
                        SetSkillTalent(skill, player, 0);
                        w_player.SetSkill( skill);
                        skill->SetLevel( it->second.reallevel);
                        skill->SetPlayer(&w_player);
                        if (isCloneExist)
                            skill->TakeEffect(&w_player);
                        else
                            skill->UndoEffect(&w_player);
                        skill->Destroy();
                    }
                }
            }
            ++it;
        }
        return 1;

    }

    bool SkillWrapper::EventAfterEnterWorld(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::EventAfterEnterWorld");
        PlayerWrapper w_player(player, 4, 0, 0, 0);
        int cul = player.GetCultivation();
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (1)
        {
            if (it == map.end())
                break;
            const SkillStub* stub = SkillStub::GetStub(it->first);
            if (stub)
            {
                if (stub->type == 11 && stub->GetEventFlag() == 16)
                {
                    int Occupation = stub->GetOccupation();
                    if (! IsDiffrentOccupClass(w_player.GetOccupation(), Occupation)
                        && (stub->occupation != 148 || (((unsigned char)cul ^ 1) & 1) == 0)
                        && (stub->occupation != 149 || (cul & 2) != 0)
                        && (stub->occupation != 150 || (cul & 4) != 0)
                        && (stub->occupation != 167 || (((unsigned char)cul ^ 1) & 1) == 0)
                        && (stub->occupation != 168 || (cul & 2) != 0)
                        && (stub->occupation != 169 || (cul & 4) != 0))
                    {
                        Skill* skill = GNET::Skill::Create(it->first);
                        if (skill)
                        {
                            SetSkillTalent(skill, player, 0);
                            w_player.SetSkill( skill);
                            skill->SetLevel( it->second.reallevel);
                            skill->SetPlayer( &w_player);
                            skill->TakeEffect( &w_player);
                            skill->Destroy();
                        }
                    }
                }
            }
            ++it;
        }
        return 1;

    }

    void SkillWrapper::CultivationChange(object_interface player, int culold, int culnew)
    {
        skill_log("ENTER: SkillWrapper::CultivationChange culold=%d culnew=%d", culold, culnew);
        if ((culold & culnew) == 0)
        {
            PlayerWrapper w_player(player, 2, 0, 0, 0);
            std::map<unsigned int, PersistentData>::iterator it = map.begin();
            while (1)
            {
                if (it == map.end())
                {
                    return;
                }
                const SkillStub* stub = SkillStub::GetStub(it->first);
                if (stub)
                {
                    if (stub->type == 11 && stub->GetEventFlag() == 1)
                    {
                        Skill* skill = GNET::Skill::Create(it->first);
                        if (skill)
                        {
                            SetSkillTalent(skill, player, 0);
                            w_player.SetSkill(skill);
                            skill->SetLevel(it->second.reallevel);
                            skill->SetPlayer(&w_player);
                            switch (stub->occupation)
                            {
                            case 148:
                            {
                                if ((culold & 1) != 0)
                                {
                                    skill->UndoEffect(&w_player);
                                    break;
                                }
                                if ((culnew & 1) != 0)
                                {
                                    w_player.SetContext(4);
                                    skill->TakeEffect(&w_player);
                                }
                                break;
                            }
                            case 149:
                            {
                                if ((culold & 2) != 0)
                                {
                                    skill->UndoEffect(&w_player);
                                    break;
                                }
                                if ((culnew & 2) != 0)
                                {
                                    w_player.SetContext(4);
                                    skill->TakeEffect(&w_player);
                                }
                                break;
                            }
                            case 150:
                            {
                                if ((culold & 4) != 0)
                                {
                                    skill->UndoEffect(&w_player);
                                    break;
                                }
                                if ((culnew & 4) != 0)
                                {
                                    w_player.SetContext(4);
                                    skill->TakeEffect(&w_player);

                                }
                                break;
                            }
                            default:
                            {
                                if (stub->occupation != 167)
                                {
                                    if (stub->occupation == 168)
                                    {
                                        if ((culold & 2) == 0)
                                        {
                                            if ((culnew & 2) != 0)
                                            {
                                                w_player.SetContext(4);
                                                skill->TakeEffect(&w_player);
                                            }
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if (stub->occupation != 169)
                                            break;
                                        if ((culold & 4) == 0)
                                        {
                                            if ((culnew & 4) == 0)
                                                break;
                                            w_player.SetContext(4);
                                            skill->TakeEffect(&w_player);
                                            break;
                                        }
                                    }
                                    skill->UndoEffect(&w_player);
                                }
                                else if ((culold & 1) != 0)
                                {
                                    skill->UndoEffect(&w_player);
                                }
                                else if ((culnew & 1) != 0)
                                {
                                    w_player.SetContext(4);
                                    skill->TakeEffect(&w_player);
                                }
                                break;
                            }
                            }
                            OnTalentChange(player, it->first, it->second.reallevel, it->second.reallevel);
                            skill->Destroy();
                        }
                    }
                }
                ++it;
            }
            
        }
    }

    int  SkillWrapper::NpcStart(ID id, object_interface npc, const XID* target, int size, int& next)
    {
        skill_log("ENTER: SkillWrapper::NpcStart id=%u", (unsigned int)id);
        const SkillStub* stub = SkillStub::GetStub(id);
        if (!stub)
            return -1;
        if (stub->occupation == 130)
            return -1;
        SkillStub::State* State = stub->GetState( 0);
        int time = State->GetTime(0);
        next = stub->GetState(1)->GetTime(0);
        if (stub->type == 1 || stub->type == 3)
            npc.EnterCombatState();
        A3DVECTOR pos;
        if (stub->rangetype == 5)
        {
            npc.SendClientMsgSkillCasting(1, &npc.GetSelfID(), id, time, 1u, 0, 0, pos, -1);
        }
        else
        {
            XID empty(0, 0);
            if (size > 0)
                npc.SendClientMsgSkillCasting(size, target, id, time, 1u, 0, 0, pos, -1);
            else
                npc.SendClientMsgSkillCasting(size, &empty, id, time, 1u, 0, 0, pos, -1);
        }
        return time;
    }

    //45
    void SkillWrapper::NpcEnd(ID id, object_interface npc, int level, const XID* target, int size)
    {
        skill_log("ENTER: SkillWrapper::NpcEnd id=%u level=%d", (unsigned int)id, level);
        Skill* skill = Skill::Create(id);
        if (skill && !skill->IsTalisman())
        {
            PlayerWrapper w_npc(npc, 0, skill, target, size);
            skill->SetLevel(level);
            SKILL::Data data(id);
            skill->SetData(&data);
            skill->NpcRun();
            skill->Destroy();
            if (!w_npc.GetSuccess())
                npc.SendClientMsgSkillInterrupted( 2, -1);
        }
    }

    bool SkillWrapper::NpcInterrupt(ID id, object_interface npc, int level)
    {
        skill_log("ENTER: SkillWrapper::NpcInterrupt id=%u level=%d", (unsigned int)id, level);
        Skill* skill = Skill::Create(id);
        if (!skill)
            return 1;
        skill->SetLevel(level);
        const SkillStub* stub = SkillStub::GetStub(id);
        bool ret;
        if (stub)
        {
            ret = stub->GetState(0)->Interrupt(skill);
        }
        else
        {
            ret = 1;
        }
        skill->Destroy();
        if (ret)
            npc.SendClientMsgSkillInterrupted(1, -1);
        return ret;

    }

    float SkillWrapper::NpcCastRange(ID id, object_interface npc, int level)
    {
        skill_log("ENTER: SkillWrapper::NpcCastRange id=%u level=%d", (unsigned int)id, level);
        Skill* skill = Skill::Create(id);
        if (skill)
        {
            PlayerWrapper w_npc(npc, 0, skill, 0, 0);
            skill->SetLevel(level);
            float range = skill->GetPraydistance();
            skill->Destroy();
            return range;
        }
        else
        {
            return 0.0;
        }
    }

    int SkillWrapper::IncPrayTime(int inc)
    {
        skill_log("ENTER: SkillWrapper::IncPrayTime inc=%d", inc);
        this->prayspeed -= inc;
        return 0;
    }

    int SkillWrapper::DecPrayTime(int dec)
    {
        skill_log("ENTER: SkillWrapper::DecPrayTime dec=%d", dec);
        this->prayspeed += dec;
        return 0;
    }

    //50
    int SkillWrapper::IncCoolTime(ID id, int inc)
    {
        skill_log("ENTER: SkillWrapper::IncCoolTime id=%u inc=%d", (unsigned int)id, inc);
        if (inc)
        {
            std::map<unsigned int, PersistentData>::iterator it = map.begin();
            while (1)
            {
                if (it == map.end())
                    break;
                if (it->first == id)
                {
                    it->second.cooltime += inc;
                    return 0;
                }
                ++it;
            }
        }
        return 0;
    }

    int SkillWrapper::DecCoolTime(ID id, int dec)
    {
        skill_log("ENTER: SkillWrapper::DecCoolTime id=%u dec=%d", (unsigned int)id, dec);
        if (dec)
        {
            std::map<unsigned int, PersistentData>::iterator it = map.begin();
            while (1)
            {
                if (it == map.end())
                    break;
                if (it->first == id)
                {
                    it->second.cooltime -= dec;
                    return 0;
                }
                ++it;
            }
        }
        return 0;

    }

    int SkillWrapper::GetDarkSkillCDAdjust() const
    {
        return this->dark_skill_cd_adjust;
    }

    int SkillWrapper::GetLightSkillCDAdjust() const
    {
        return this->light_skill_cd_adjust;
    }

    int SkillWrapper::IncDarkSkillCoolTime(int inc)
    {
        this->dark_skill_cd_adjust += inc;
        return 0;
    }

    //55
    int SkillWrapper::DecDarkSkillCoolTime(int dec)
    {
        this->dark_skill_cd_adjust -= dec;
        return 0;
    }

    int SkillWrapper::IncLightSkillCoolTime(int inc)
    {
        this->light_skill_cd_adjust += inc;
        return 0;
    }

    int SkillWrapper::DecLightSkillCoolTime(int dec)
    {
        this->light_skill_cd_adjust -= dec;
        return 0;
    }

    void SkillWrapper::RemoveAura(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::RemoveAura");
        if (player.IsFilterExist(FILTER_HOLYAURA))
            player.RemoveFilter(FILTER_HOLYAURA);
        if (player.IsFilterExist(FILTER_EVILAURA))
            player.RemoveFilter(FILTER_EVILAURA);
    }

    void SkillWrapper::UndoPassive(ID id, int level, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::UndoPassive id=%u level=%d", (unsigned int)id, level);
        Skill* skill = GNET::Skill::Create(id);
        if (skill)
        {
            PlayerWrapper w_player(player, 2, 0, 0, 0);
            w_player.SetSkill(skill);
            SetSkillTalent(skill, player, 0);
            skill->SetLevel( level);
            skill->SetPlayer( &w_player);
            skill->UndoEffect( &w_player);
            skill->Destroy();
        }
    }

    //60
    int SkillWrapper::GetTalentSum()
    {
        skill_log("ENTER: SkillWrapper::GetTalentSum");
        int sum = 0;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if (stub->talent_type)
                        {
                            sum += it->second.baselevel;
                        }
                    }
                }
            }
            ++it;
        }
        return sum;
    }
    
    int SkillWrapper::GetSpSum()
    {
        skill_log("ENTER: SkillWrapper::GetSpSum");
        int sum = 0;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = GNET::SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if (!stub->talent_type
                            && !stub->credittype
                            && !stub->use_proficiency
                            && stub->occupation != 167
                            && stub->occupation != 168
                            && stub->occupation != 169)
                        {
                            sum += it->second.baselevel;
                        }
                    }
                }
            }
            ++it;
        }
        return sum;

    }

    void SkillWrapper::SetSkillTalent(Skill* skill)
    {
        const SkillStub* stub = skill->GetStub();
        if (!stub)
        {
            skill->SetTalent(6, 0);
            skill->SetTalent(7, 0);
            return;
        }
        int n = stub->talent_size;
        if (n > 6)
            n = 6;
        for (int i = 0; i < n; ++i)
        {
            std::map<unsigned int, PersistentData>::iterator it = map.find(stub->talent[i]);
            if (it != map.end())
            {
                skill->SetTalent(i, it->second.reallevel);
            }
        }
        std::map<unsigned int, WeaponAddonTalent>::iterator it1 = wamap.find(skill->GetId());
        
        if (it1 == wamap.end())
        {
            skill->SetTalent(6, 0);
            skill->SetTalent(7, 0);
        }
        else
        {
            skill->SetTalent(6, it1->second.value1);
            skill->SetTalent(7, it1->second.value2);
        }
    }

    void SkillWrapper::SetSkillTalent(Skill* skill, const int* list)
    {
        for (int i = 0; i <= 7; ++i)
            skill->SetTalent(i, list[i]);
    }

    void SkillWrapper::SetSkillTalent(Skill* skill, object_interface player, bool adjustLevel)
    {
        skill_log("SetSkillTalent(player): entered, skill=%p", (void*)skill);
        const SkillStub* stub = skill->GetStub();
        skill_log("SetSkillTalent(player): stub=%p (null=%d)", (void*)stub, (stub==NULL)?1:0);
        if (!stub)
        {
            skill_log("SetSkillTalent(player): stub null branch");
            skill_log("SetSkillTalent(player): calling SetTalent(6,0) on skill=%p", (void*)skill);
            skill->SetTalent(6, 0);
            skill_log("SetSkillTalent(player): SetTalent(6) done, calling SetTalent(7,0)");
            skill->SetTalent(7, 0);
            skill_log("SetSkillTalent(player): SetTalent(7) done, returning");
            return;
        }
        skill_log("SetSkillTalent(player): stub non-null, talent_size=%d", (int)stub->talent_size);
        int n = stub->talent_size;
        if (n > 6)
            n = 6;
        for (int i = 0; i < n; ++i)
        {
            const SkillStub* tstub = GNET::SkillStub::GetStub(stub->talent[i]);
            if (tstub && (tstub->clearmask & 7) != 0
                && (tstub->clearmask & player.GetCultivation() == 0))
            {
                skill->SetTalent(i, 0);
            }
            else
            {
                std::map<unsigned int, PersistentData>::iterator it = map.find(stub->talent[i]);
                if (it != map.end())
                {
                    short level = it->second.reallevel;
                    if (adjustLevel)
                    {
                        player.AdjustSkill( stub->talent[i], level);
                        if (level > tstub->maxlevel)
                            level = tstub->maxlevel;
                        if (level < 0)
                            level = 0;
                    }
                    skill->SetTalent(i, level);
                }
            }
        }

        std::map<unsigned int, WeaponAddonTalent>::iterator it1 = wamap.find(skill->GetId());
        if (it1 == wamap.end())
        {
            skill->SetTalent(6, 0);
            skill->SetTalent(7, 0);
        }
        else
        {
            skill->SetTalent( 6, it1->second.value1);
            skill->SetTalent( 7, it1->second.value2);
        }
    }

    //65
    void SkillWrapper::SetSkillTalent(ID id, int list[8], object_interface player)
    {
        memset(list, 0, sizeof(int));
        const SkillStub* stub = GNET::SkillStub::GetStub(id);
        if (stub)
        {
            short n = stub->talent_size;
            if (n > 6)
                n = 6;
            for (int i = 0; i < n; ++i)
            {
                const SkillStub* tstub = SkillStub::GetStub(stub->talent[i]);
                if (tstub
                    && (tstub->clearmask & 7) != 0
                    && (tstub->clearmask & player.GetCultivation() == 0))
                {
                    list[i] = 0;
                }
                else
                {
                    std::map<unsigned int, PersistentData>::iterator it = map.find(stub->talent[i]);
                    if (it != map.end())
                    {
                        list[i] = it->second.reallevel;
                    }
                }
            }
            std::map<unsigned int, WeaponAddonTalent>::iterator it1 = wamap.find(id);
            if (it1 == wamap.end())
            {
                list[6] = 0;
                list[7] = 0;
            }
            else
            {
                list[6] = it1->second.value1;
                list[7] = it1->second.value2;
            }
        }
    }

    void SkillWrapper::AddSkillElement(ID id, int elemID)
    {
        if (elemID >= 0)
        {
            const SkillStub* stub = GNET::SkillStub::GetStub(id);
            if (stub)
            {
                int elem_idx;
                if (stub->CheckSkillElem(elemID, elem_idx))
                {
                    std::map<unsigned int, SkillElems>::iterator it = semap.find(id);
                    if (it != semap.end())
                    {
                        if (it->second.curr_num < MAX_SKILL_ELEM_NUM)
                        {
                            it->second.ids[it->second.curr_num] = elemID;
                            ++it->second.curr_num;
                        }
                    }
                    else
                    {
                        SkillElems elems;
                        elems.ids[elems.curr_num++] = elemID;
                        semap[id]=elems;
                    }
                }
            }
        }

    }

    void SkillWrapper::SetSkillElement(ID id, int idx, int elemID)
    {
        if (idx < MAX_SKILL_ELEM_NUM && elemID >= 0)
        {
            const SkillStub* stub = SkillStub::GetStub(id);
            if (stub)
            {
                int elem_idx;
                if (stub->CheckSkillElem(elemID, elem_idx))
                {
                    char level = 0;
                    std::map<unsigned int, PersistentData>::iterator it = map.find(id);
                    if (it != map.end())
                    {
                        level = it->second.reallevel;
                    }
                    if (level > 0)
                    {
                        std::map<unsigned int, SkillElems>::iterator it1 = semap.find(id);
                        if (it1 != semap.end())
                        {
                            if (it1->second.curr_num <= stub->GetComboSkillCapacity(level))
                            {
                                it1->second.ids[idx] = elemID;
                                if (idx + 1 > it1->second.curr_num)
                                    it1->second.curr_num = idx + 1;
                            }
                        }
                        else
                        {
                            SkillElems elems;
                            elems.ids[idx] = elemID;
                            elems.curr_num = idx + 1;
                            semap[id]=elems;
                        }
                    }
                }
            }
        }
    }

    void SkillWrapper::DelSkillElement(ID id, int idx)
    {
        if (idx < MAX_SKILL_ELEM_NUM)
        {
            const SkillStub* stub = GNET::SkillStub::GetStub(id);
            if (stub)
            {
                std::map<unsigned int, SkillElems>::iterator it = semap.find(id);
                if (it != semap.end())
                {
                    if (idx >= 0 && idx < it->second.curr_num)
                    {
                        int elem_idx;
                        if (stub->CheckSkillElem(it->second.ids[idx], elem_idx))
                        {
                            for (int i = idx + 1; i < it->second.curr_num; ++i)
                                it->second.ids[i-1] = it->second.ids[i];
                            it->second.ids[it->second.curr_num] = 0;
                            --it->second.curr_num;
                        }
                    }
                }
            }
        }
    }

    void SkillWrapper::SetSkillElemsNum(Skill* skill, const SkillElems& elems)
    {
        for (int i = 0; i < MAX_SKILL_ELEM_NUM; ++i)
        {
            if (elems.ids[i])
            {
                int elem_idx;
                if (skill->GetStub()->CheckSkillElem(elems.ids[i], elem_idx))
                    skill->IncSkillElemsNum(elem_idx - 1, 1);
            }
        }

    }

    //70
    void SkillWrapper::SetSkillElemsNum(Skill* skill, unsigned short elems[10])
    {
        for (int i = 0; i <= 9; ++i)
        {
            if (elems[i])
            {
                int elem_idx;
                if (skill->GetStub()->CheckSkillElem(elems[i], elem_idx))
                    skill->IncSkillElemsNum(elem_idx - 1, 1);
            }
        }
    }

    void SkillWrapper::SaveSkillElems(archive& ar)
    {
        skill_log("ENTER: SkillWrapper::SaveSkillElems");
        ar << semap.size();
        std::map<unsigned int, SkillElems>::iterator it = semap.begin();
        while (1)
        {
            if (it == semap.end())
                break;
            ar << it->first;
            ar << it->second.curr_num;
            for (int i = 0; i < it->second.curr_num; ++i)
                ar << it->second.ids[i];
            ++it;
        }
    }

    void SkillWrapper::LoadSkillElems(archive& ar)
    {
        skill_log("ENTER: SkillWrapper::LoadSkillElems");
        SkillElems elems;
        size_t size;
        ar >> size;
        skill_log("ENTER: SkillWrapper::LoadSkillElems size: %d", size);
        for (int i = 0; i < size; ++i)
        {
            memset(&elems, 0, sizeof(elems));
            short skillid;
            ar >> skillid;
            ar >> elems.curr_num;
            for (int j = 0; j < elems.curr_num; ++j)
                ar >> elems.ids[j];
            semap[skillid]=elems;
        }

    }

    void SkillWrapper::SaveSkillElemsClient(archive& ar)
    {
        skill_log("ENTER: SkillWrapper::SaveSkillElemsClient");
        ar << semap.size();
        std::map<unsigned int, SkillElems>::iterator it = semap.begin();
        while (1)
        {
            if (it == semap.end())
                break;
            ar << it->first;
            ar.push_back(&it->second, sizeof(SkillElems));
            ++it;
        }
    }

    bool SkillWrapper::OnTalentChange(object_interface player, int talent_skill_id, int old_talent_skill_lvl, int new_talent_skill_lvl)
    {
        skill_log("ENTER: SkillWrapper::OnTalentChange talent_skill_id=%d old=%d new=%d", talent_skill_id, old_talent_skill_lvl, new_talent_skill_lvl);
        int cul = player.GetCultivation();
        const SkillStub* stub = SkillStub::GetStub(talent_skill_id);
        if (stub && stub->type == 11)
        {
            Skill* tskill = Skill::Create(talent_skill_id);
            if (tskill)
            {
                PlayerWrapper w_player(player, 4, 0, 0, 0);
                SetSkillTalent(tskill, player, 0);
                w_player.SetSkill( tskill);
                tskill->SetPlayer(&w_player);
                tskill->SetLevel(old_talent_skill_lvl);
                tskill->UndoEffect(&w_player);
                w_player.SetContext(4);
                tskill->SetLevel(new_talent_skill_lvl);
                tskill->TakeEffect(&w_player);
                tskill->Destroy();
            }
            PlayerWrapper w_player(player, 4, 0, 0, 0);
            std::map<unsigned int, PersistentData>::iterator it = map.begin();
            while (1)
            {
                if (it == map.end())
                {
                    return 1;
                }
                const SkillStub* s = SkillStub::GetStub(it->first);
                if (s)
                {
                    if (s->type == 11)
                    {
                        for (int i = 0; ; ++i)
                        {
                            if (i >= s->talent_size)
                                break;
                            if (s->talent[i] == talent_skill_id)
                            {
                                if (IsDiffrentOccupClass(w_player.GetOccupation(), s->GetOccupation()))
                                {
                                    Skill* ts = Skill::Create(it->first);
                                    if (ts && it->second.reallevel > 0)
                                    {
                                        w_player.SetSkill(ts);
                                        ts->SetPlayer(&w_player);
                                        ts->SetLevel(it->second.reallevel);
                                        SetSkillTalent(ts, player, 0);
                                        ts->SetTalent(i, old_talent_skill_lvl);
                                        ts->UndoEffect(&w_player);
                                        ts->Destroy();
                                        continue;
                                    }
                                }
                                else if (s->occupation == 148 && (((unsigned char)cul ^ 1) & 1) != 0
                                    || s->occupation == 149 && (cul & 2) == 0
                                    || s->occupation == 150 && (cul & 4) == 0)
                                {
                                    Skill* ts = GNET::Skill::Create(it->first);
                                    if (ts && it->second.reallevel > 0)
                                    {
                                        w_player.SetSkill(ts);
                                        ts->SetPlayer(&w_player);
                                        ts->SetLevel(it->second.reallevel);
                                        SetSkillTalent(ts, player, 0);
                                        ts->SetTalent(i, old_talent_skill_lvl);
                                        ts->UndoEffect(&w_player);
                                        ts->Destroy();
                                        continue;
                                    }
                                }
                                else if (s->occupation == 167 && (((unsigned char)cul ^ 1) & 1) != 0
                                    || s->occupation == 168 && (cul & 2) == 0
                                    || s->occupation == 169 && (cul & 4) == 0)
                                {
                                    Skill* ts = GNET::Skill::Create(it->first);
                                    if (ts && it->second.reallevel > 0)
                                    {
                                        w_player.SetSkill(ts);
                                        ts->SetPlayer(&w_player);
                                        ts->SetLevel(it->second.reallevel);
                                        SetSkillTalent(ts, player, 0);
                                        ts->SetTalent(i, old_talent_skill_lvl);
                                        ts->UndoEffect(&w_player);
                                        ts->Destroy();
                                        continue;
                                    }
                                }
                                else
                                {
                                    Skill* tsa = GNET::Skill::Create(it->first);
                                    if (tsa && it->second.reallevel > 0)
                                    {
                                        w_player.SetSkill(tsa);
                                        tsa->SetPlayer(&w_player);
                                        tsa->SetLevel(it->second.reallevel);
                                        if (tsa->GetEventFlag() != 8 || w_player.IsCloneExist())
                                        {
                                            SetSkillTalent(tsa, player, 0);
                                            tsa->SetTalent(i, old_talent_skill_lvl);
                                            tsa->UndoEffect(&w_player);
                                            w_player.SetContext(4);
                                            tsa->SetTalent(i, new_talent_skill_lvl);
                                            tsa->TakeEffect(&w_player);
                                            tsa->Destroy();
                                        }
                                        else
                                        {
                                            tsa->Destroy();
                                        }
                                    }
                                }
                            }
                        }

                    }
                }
                ++it;
            }
        }
        return 0;

    }

    //75
    bool SkillWrapper::CheckConsistency()
    {
        skill_log("ENTER: SkillWrapper::CheckConsistency");
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = GNET::SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        for (int i = 0; i <= stub->GetPreskillNum(); ++i)
                        {
                            if (!stub->GetPreSkillID(i))
                            {
                                if (stub->GetPreSkillSP(i))
                                {
                                    int Base = Rank::GetBase(stub->occupation);
                                    int LevelSum = GetLevelSum(Base);
                                    if (LevelSum < stub->GetPreSkillSP(i))
                                        return 0;
                                }
                            }
                        }
                    }
                }
            }
            ++it;
        }
        return 1;
    }

    int SkillWrapper::GetCooldownId(ID id)
    {
        skill_log("ENTER: SkillWrapper::GetCooldownId id=%u", (unsigned int)id);
        return id + 1024;
    }

    int SkillWrapper::GetMpCost(ID id, int level)
    {
        skill_log("ENTER: SkillWrapper::GetMpCost id=%u level=%d", (unsigned int)id, level);
        Skill* skill = Skill::Create(id);
        if (!skill)
            return 0;
        skill->SetLevel( level);
        float Mpcost = skill->GetMpcost();
        skill->Destroy();
        return (int)Mpcost;
    }

    int SkillWrapper::GetSerialSkillType(ID id)
    {
        const SkillStub* stub = GNET::SkillStub::GetStub(id);
        if (stub)
            return stub->serialskill;
        else
            return 0;
    }

    void SkillWrapper::GetSucceedSkillColor(ID id, int& color, int& num)
    {
        color = 0;
        num = 0;
        const SkillStub* stub = GNET::SkillStub::GetStub(id);
        if (stub)
        {
            color = stub->succeedskillcolor;
            num = stub->succeedskillcolornum;
        }
    }

    //80
    void SkillWrapper::SetSucceedSkillColor(Skill* skill, object_interface player)
    {
        int c[5];
        if (skill->GetSerialSkill() == 3)
        {
            memset(c, 0, sizeof(c));
            player.GetComboColor(c[0], c[1], c[2], c[3], c[4]);
            for (int i = 0; i <= 4; ++i)
                skill->SetSucceedSkillColor(i, c[i]);
        }
    }

    void SkillWrapper::SetSucceedSkillColor(Skill* skill, char colors[5])
    {
        if (skill->GetSerialSkill() == 3)
        {
            for (int i = 0; i <= 4; ++i)
                skill->SetSucceedSkillColor(i, colors[i]);
        }
    }

    void SkillWrapper::SetDecSkillLevel(int level)
    {
        skill_log("ENTER: SkillWrapper::SetDecSkillLevel level=%d", level);
        this->dec_skill_level = level;
    }

    void SkillWrapper::RecordPos(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::RecordPos");
        this->record_map_id = player.GetTag();
        this->record_pos = player.GetPos();
    }

    void SkillWrapper::ClearRecordPos()
    {
        skill_log("ENTER: SkillWrapper::ClearRecordPos");
        this->record_map_id = -1;
    }

    //85
    bool SkillWrapper::SetXPSkill(int id)
    {
        skill_log("setXPSkill Bool");
        skill_log("setXPSkill SkillID: %d", id);
        if (id <= 0)
            return 0;
        PersistentData data(0);
        data.baselevel = 1;
        data.reallevel = 1;
        data.actilevel = 0;
        data.mask = 1;
        map[id].baselevel = data.baselevel;
        map[id].reallevel = data.reallevel;
        map[id].actilevel = data.actilevel;
        map[id].mask = data.mask;
        map[id].cooltime = data.cooltime;
        return 1;

    }

    bool SkillWrapper::ClearXPSkill(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ClearXPSkill");
        int id = player.GetXPSkill();
        if (id <= 0)
            return 0;
        const SkillStub* stub = GNET::SkillStub::GetStub(id);
        if (!stub || stub->occupation != 187)
            return 0;
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        if (it == map.end())
        {
            return 0;
        }
        player.ClrCoolDown( id + 1024);
        return 1;
    }

    bool SkillWrapper::ClearXPSkillCoolTime(object_interface player)
    {
        skill_log("ENTER: SkillWrapper::ClearXPSkillCoolTime");
        int id = player.GetXPSkill();
        if (id <= 0)
            return 0;
        const SkillStub* stub = SkillStub::GetStub(id);
        if (!stub || stub->occupation != 187)
            return 0;
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        if (it == map.end())
        {
            return 0;
        }
        player.ClrCoolDown(id + 1024);
        return 1;
    }

    void SkillWrapper::Swap(SkillWrapper& sw)
    {
        skill_log("ENTER: SkillWrapper::Swap");
        sw.map.swap(map);
        sw.wamap.swap(wamap);
        sw.prfmap.swap(prfmap);
        std::swap<short>(this->prayspeed, sw.prayspeed);
        std::swap<int>(this->dec_skill_level, sw.dec_skill_level);
        std::swap<int>(this->curr_cast_speed_rate, sw.curr_cast_speed_rate);
        std::swap<int>(this->dark_skill_cd_adjust, sw.dark_skill_cd_adjust);
        std::swap<int>(this->light_skill_cd_adjust, sw.light_skill_cd_adjust);
        std::swap<int>(this->record_map_id, sw.record_map_id);
        std::swap<A3DVECTOR>(this->record_pos, sw.record_pos);
        sw.semap.swap(semap);

    }

    int SkillWrapper::GetSkillLevel(ID id)
    {
        skill_log("ENTER: SkillWrapper::GetSkillLevel id=%u", (unsigned int)id);
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        if (it == map.end())
            return 0;
        return it->second.reallevel;
    }

    //90
    short SkillWrapper::GetSkillData(ID id, short& level, int& cooltime, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::GetSkillData id=%u", (unsigned int)id);
        std::map<unsigned int, PersistentData>::iterator it = map.find(id);
        if (it == map.end())
        {
            skill_log("GetSkillData: skill not in map");
            return 0;
        }
        level = it->second.reallevel;
        skill_log("GetSkillData: reallevel=%d", (int)level);
        if (!level)
            return 0;
        level -= this->dec_skill_level;
        if (level <= 0)
            level = 1;
        cooltime = it->second.cooltime;
        skill_log("GetSkillData: calling AdjustSkill");
        player.AdjustSkill(id, level);
        skill_log("GetSkillData: calling GetStub");
        const SkillStub* stub = SkillStub::GetStub(id);
        skill_log("GetSkillData: stub=%s", stub ? "found" : "null");
        if (stub)
        {
            if (level > stub->maxlevel)
                level = stub->maxlevel;
            if (level < 0)
                level = 0;
        }
        skill_log("GetSkillData: returning level=%d", (int)level);
        return level;

    }

    void SkillWrapper::SetFamilySkill(int* pairs, int size, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::SetFamilySkill size=%d", size);
        if (pairs && size)
        {
            for (int i = 0; i < size - 1; i += 2)
            {
                int level = pairs[i + 1];
                PersistentData data(0);
                data.baselevel = level;
                data.mask = 4;
                data.reallevel = level;
                map[pairs[i]].baselevel = data.baselevel;
                map[pairs[i]].cooltime = data.cooltime;
                player.SendClientExtraSkill( pairs[i], level);
            }
        }
        else
        {
            std::map<unsigned int, PersistentData>::iterator it = map.begin();
            while (1)
            {
                if (it == map.end())
                    break;
                if ((it->second.mask & 4) != 0)
                {
                    it->second.baselevel = 0;
                    it->second.reallevel = 0;
                    player.SendClientExtraSkill(it->first, 0);
                }
                ++it;
            }
        }

    }

    void SkillWrapper::SetSectSkill(int* pairs, int size, object_interface player)
    {
        skill_log("ENTER: SkillWrapper::SetSectSkill size=%d", size);
        if (pairs && size)
        {
            for (int i = 0; i < size - 1; i += 2)
            {
                int level = pairs[i + 1];
                PersistentData data(0);
                data.baselevel = level;
                data.mask = 8;
                data.reallevel = level;
                map[pairs[i]].baselevel = data.baselevel;
                map[pairs[i]].cooltime = data.cooltime;
                player.SendClientExtraSkill( pairs[i], level);
            }
        }
        else
        {
            std::map<unsigned int, PersistentData>::iterator it = map.begin();
            while (1)
            {
                if (it == map.end())
                    break;
                if ((it->second.mask & 8) != 0)
                {
                    it->second.baselevel = 0;
                    it->second.reallevel = 0;
                    player.SendClientExtraSkill( it->first, 0);
                }
                ++it;
            }
        }
    }

    void SkillWrapper::GetSectSkill(int* pairs, int& size)
    {
        skill_log("ENTER: SkillWrapper::GetSectSkill");
        size = 0;
        int n = 0;
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = SkillStub::GetStub(it->first);
                    if (stub)
                    {
                        if (stub->occupation == 147)
                        {
                            pairs[n] = it->first;
                            pairs[n + 1] = it->second.baselevel;
                            n += 2;
                        }
                    }
                }
            }
            ++it;
        }
        size = n >> 1;
    }

    int  SkillWrapper::Forget(object_interface player, int mask)
    {
        skill_log("ENTER: SkillWrapper::Forget(mask) mask=%d", mask);
        std::map<unsigned int, PersistentData>::iterator it = map.begin();
        while (it != map.end())
        {
            if (!it->second.mask)
            {
                if (it->second.baselevel)
                {
                    const SkillStub* stub = GNET::SkillStub::GetStub(it->first);
                    if (!stub || (mask & stub->clearmask) != 0)
                    {
                        if (stub->type == 11)
                        {
                            UndoPassive(it->first, it->second.reallevel, player);
                        }
                        commonlevel = this->commonlevel;
                        int real = it->second.actilevel + commonlevel;
                        if (real < it->second.reallevel)
                            it->second.reallevel = real;
                        it->second.baselevel = 0;
                    }
                }
            }
            ++it;
        }
        return 0;
    }

    //95
    void SkillWrapper::WeaponAddon(ID id, int value1, int value2)
    {
        skill_log("ENTER: SkillWrapper::WeaponAddon id=%u v1=%d v2=%d", (unsigned int)id, value1, value2);
        std::map<unsigned int, WeaponAddonTalent>::iterator it = wamap.find(id);
        if (it == wamap.end())
        {
            WeaponAddonTalent wat(value1, value2);
            wamap[id].value1 = wat.value1;
            wamap[id].value2 = wat.value2;
        }
        else
        {
            it->second.value1 += value1;
            it->second.value2 += value2;
            if (!it->second.value1 && !it->second.value2)
            {
                wamap.erase(it);
            }
        }
    }

    void SkillWrapper::SetRejectRate(int rate)
    {
        this->rejectrate = rate;
    }

    int  SkillWrapper::GetRejectRate()
    {
        return this->rejectrate;
    }
    
    void SkillWrapper::AddProficiency(object_interface player, ID skillid, int add)
    {
        skill_log("ENTER: SkillWrapper::AddProficiency skillid=%u add=%d", (unsigned int)skillid, add);
        const SkillStub* stub = GNET::SkillStub::GetStub(skillid);
        if (stub)
        {
            if (stub->use_proficiency)
            {
                std::map<unsigned int, PersistentData>::iterator it = map.find(skillid);
                if (it != map.end())
                {
                    if (it->second.baselevel < stub->maxlearn)
                    {
                        prfmap[skillid] += add;
                        player.SendClientSkillProficiency(skillid, prfmap[skillid]);
                    }
                }
            }
        }

    }

    bool SkillWrapper::DecProficiency(object_interface player, ID skillid, int dec)
    {
        skill_log("ENTER: SkillWrapper::DecProficiency skillid=%u dec=%d", (unsigned int)skillid, dec);
        if (dec <= 0)
            return 1;
        std::map<unsigned int, int>::iterator it = prfmap.find(skillid);
        int ok = 0;
        if (it == prfmap.end() || it->second < dec)
        {
            ok = 1;
        }
        if (ok)
            return 0;
        it->second -= dec;
        player.SendClientSkillProficiency( skillid, it->second);
        return 1;
    }

    //100
    unsigned int SkillWrapper::GetSkillLimit(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub)
            return stub->skill_limit;
        else
            return 0;
    }

    bool SkillWrapper::IsMonsterLimitSkill(ID id)
    {
        return SkillWrapper::GetSkillLimit(id) & 1;
    }

    bool SkillWrapper::IsPetLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 1) & 1;
    }

    bool SkillWrapper::IsSummonLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 2) & 1;
    }

    bool SkillWrapper::IsPlayerLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 3) & 1;
    }

    //105
    bool SkillWrapper::IsSelfLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 4) & 1;
    }

    bool SkillWrapper::IsBattleLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 5) & 1;
    }

    bool SkillWrapper::IsCombatLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 6) & 1;
    }

    bool SkillWrapper::IsNonCombatLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 7) & 1;
    }

    bool SkillWrapper::IsNoSummonPetLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 8) & 1;
    }

    //110
    bool SkillWrapper::IsMountLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 9) & 1;
    }

    bool SkillWrapper::IsFlyLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 10) & 1;
    }

    bool SkillWrapper::IsNoSummonLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 11) & 1;
    }

    int  SkillWrapper::LimitSkillSummonID(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub)
            return stub->summon_id;
        else
            return -1;
    }

    bool SkillWrapper::IsBossLimitSkill(ID id)
    {
        return (SkillWrapper::GetSkillLimit(id) >> 12) & 1;
    }

    //115
    bool SkillWrapper::IsWarmup(ID id)
    {
        const SkillStub* stub = GNET::SkillStub::GetStub(id);
        return stub && stub->timetype == 3;
    }

    bool SkillWrapper::IsCycle(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub && stub->cycle)
            return 1;
        return 0;
    }

    bool SkillWrapper::IsTalisman(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub && stub->occupation == 130)
            return 1;
        return 0;
    }

    int SkillWrapper::GetMaxSkillLevel(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub)
            return stub->maxlevel;
        else
            return 0;
    }

    char SkillWrapper::GetDarkLightType(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub)
            return stub->darkLightType;
        else
            return -1;
    }

    //120
    bool SkillWrapper::IsCastInPreSkill(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        return stub && stub->castInPreSkill;
    }

    int  SkillWrapper::GetPreCastSkillId(ID id)
    {
        const SkillStub* stub = SkillStub::GetStub(id);
        if (stub)
            return stub->preCastSkillId;
        else
            return -1;
    }

    bool SkillWrapper::IsXPSkill(ID id)
    {
        const SkillStub* stub = GNET::SkillStub::GetStub(id);
        if (stub && stub->occupation == 187)
            return 1;
        return 0;
    }

} // namespace GNET




