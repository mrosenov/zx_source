#include "achievement.h"
#include "obj_interface.h"
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <common/packetwrapper.h>
#include <glog.h>
#include <timer.h>
#include "topic_site.h"

extern abase::timer g_timer;

Achievement::ACHIEVEMENT_MAP Achievement::s_stubs;
Achievement::HeaderSet Achievement::s_headers;
unsigned short Achievement::total_achievement_count = 0;  // Youshuang add
PremissTemplate::TEMPLATE_MAP PremissTemplate::s_stubs;

namespace AchievementImp
{
struct Condition
{
	Condition(unsigned int _size, void * _data) : size(_size), data(_data) {}
	unsigned int  size;
	void * data;
};

enum
{
	EVENT_RET_SUCC	= 0,
	EVENT_RET_NOCHG	= 1,
	EVENT_RET_CHG	= 2,
};

template <unsigned short TEMPLATE_ID, unsigned int COND_ID>
class TemplateSimple : public PremissTemplate
{
public:
	TemplateSimple() : PremissTemplate(TEMPLATE_ID) {} 
	virtual ~TemplateSimple() {}

	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.b_finished == 1; 
	}

	virtual bool Load(PremissData & data, archive & ar) 
	{
		ar >> data.b_finished;
		return true;
	}
	virtual bool Save(PremissData & data, archive & ar)
	{ 
		ar << data.b_finished;
		return true;
	}

	virtual char GetParaSize() const { return 1;}
	virtual unsigned int GetCondMask() const { return COND_ID; }
};

//人物等级
typedef TemplateSimple<TEMPL_LEVEL, COND_LEVEL> _TemplateLevel;
class TemplateLevel : public _TemplateLevel 
{
public:
	struct Foo 
	{
		unsigned int rebornCnt;
		unsigned int level;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition) 
	{ 
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.rebornCnt && premiss->parameter2 <= foo.level)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//金钱
typedef TemplateSimple<TEMPL_MONEY, COND_MONEY> _TemplateMoney;
class TemplateMoney : public _TemplateMoney 
{
public:
	struct Foo
	{
		unsigned int money;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 <= foo.money)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//声望
typedef TemplateSimple<TEMPL_REPUTATION, COND_REPUTATION> _TemplateReputation;
class TemplateReputation : public _TemplateReputation 
{
public:
	struct Foo
	{
		unsigned int reputation_id;
		unsigned int reputation;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.reputation_id && premiss->parameter2 <= foo.reputation)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//杀怪
typedef TemplateSimple<TEMPL_KILLMONSTER, COND_KILLMONSTER> _TemplateKillMonster;
class TemplateKillMonster : public _TemplateKillMonster 
{
public:
	struct Foo 
	{
		unsigned int monsterid;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.monsterid)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


//PK值
typedef TemplateSimple<TEMPL_PK, COND_PK> _TemplatePK;
class TemplatePK : public _TemplatePK
{
public:
	struct Foo
	{
		unsigned int pkval;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 <= foo.pkval)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


//完成任务
typedef TemplateSimple<TEMPL_TASK, COND_TASK> _TemplateTask;
class TemplateTask : public _TemplateTask
{
public:
	struct Foo
	{
		unsigned int taskid;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.taskid)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//精炼
typedef TemplateSimple<TEMPL_REFINE, COND_REFINE> _TemplateRefine;
class TemplateRefine : public _TemplateRefine
{
public:
	struct Foo
	{ 
		unsigned int level_before; 
		unsigned int level_result; 
		unsigned int spirit;
	};

	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.level_before && premiss->parameter2 == foo.level_result 
				&& (premiss->parameter3 == foo.spirit || premiss->parameter3 == 0))
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


//宠物修炼
typedef TemplateSimple<TEMPL_PETREFINE, COND_PETREFINE> _TemplatePetRefine;
class TemplatePetRefine : public _TemplatePetRefine
{
public:
	struct Foo
	{ 
		unsigned int star_level; 
	};

	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.star_level) 
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


//法宝血炼
typedef TemplateSimple<TEMPL_TALIREFINE, COND_TALIREFINE> _TemplateTaliRefine;
class TemplateTaliRefine : public _TemplateTaliRefine
{
public:
	struct Foo
	{ 
		unsigned int combine_value; 
	};

	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.combine_value) 
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//生产物品
typedef TemplateSimple<TEMPL_PRODUCE, COND_PRODUCE> _TemplateProduce;
class TemplateProduce : public _TemplateProduce 
{
public:
	struct Foo
	{ 
		unsigned int recipe_id; 
		unsigned int item_id; 
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.recipe_id&& premiss->parameter2 == foo.item_id)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


//装备物品
typedef TemplateSimple<TEMPL_EQUIPMENT, COND_EQUIPMENT> _TemplateEquipment;
class TemplateEquipment : public _TemplateEquipment
{
public:
	struct Foo 
	{
		unsigned int equipid;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.equipid)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//装备特定物品
typedef TemplateSimple<TEMPL_EQUIPMENT, COND_SPECEQUIP> _TemplateSpecEquip;
class TemplateSpecEquip: public _TemplateSpecEquip
{
public:
	struct Foo 
	{
		unsigned int index;
		unsigned int spirit;
		unsigned int level;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.index && premiss->parameter2 == foo.spirit && premiss->parameter3 == foo.level)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


//达成某成就
typedef TemplateSimple<TEMPL_ACHIEVEMENT, COND_ACHIEVEMENT> _TemplateAchievement;
class TemplateAchievement : public _TemplateAchievement
{
public:
	struct Foo
	{
		unsigned int achieve_id;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.achieve_id)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//获得称谓
typedef TemplateSimple<TEMPL_TITLE, COND_TITLE> _TemplateTitle;
class TemplateTitle: public _TemplateTitle
{
public:
	struct Foo
	{
		unsigned int title_id;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.title_id)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//获得分数
//
// 1 -- 新科试炼
//
//
typedef TemplateSimple<TEMPL_SCORE, COND_SCORE> _TemplateScore;
class TemplateScore: public _TemplateScore
{
public:
	struct Foo
	{
		unsigned int type;
		unsigned int score;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.type && premiss->parameter1 <= foo.score)
		{
			data.b_finished = 1;
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};


// 7天连续活跃度100分
typedef TemplateSimple<TEMPL_LIVENESS_100P, COND_LIVENESS_100P> _TemplateLiveness100P;
class TemplateLiveness100P: public _TemplateLiveness100P
{
public:
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		data.b_finished = 1;
		return EVENT_RET_SUCC;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

class PremissCountTemplate : public PremissTemplate
{
public:
	PremissCountTemplate(unsigned short templ_id) : PremissTemplate(templ_id) {}
	~PremissCountTemplate(){}

	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
	virtual char GetParaSize() const {return 2;}
	virtual bool Load(PremissData & data, archive & ar) 
	{
		ar >> data.count;
		return true;
	}
	virtual bool Save(PremissData & data, archive & ar)
	{
		ar << data.count; 
		return true;
	}	
		
};

//完成任务次数
class TemplateCountTask : public PremissCountTemplate
{
public:
	struct Foo
	{
		unsigned int taskid;
	};
	TemplateCountTask() : PremissCountTemplate(TEMPL_COUNT_TASK) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.taskid)
		{
			if (++data.count >= premiss->parameter2)
				return EVENT_RET_SUCC;
			else
				return EVENT_RET_CHG;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter2; 
	}
	virtual unsigned int GetCondMask() const { return COND_TASK; }
};

//杀死怪物次数
class TemplateCountKillMonster : public PremissCountTemplate
{
public:
	struct Foo
	{
		unsigned int monsterid;
	};
	TemplateCountKillMonster() : PremissCountTemplate(TEMPL_COUNT_KILLMONSTER) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.monsterid)
		{
			if (++data.count >= premiss->parameter2)
				return EVENT_RET_SUCC;
			else
				return EVENT_RET_CHG;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter2; 
	}
	virtual unsigned int GetCondMask() const { return COND_KILLMONSTER; }
};

//生产配方的某物品次数
class TemplateCountProduce : public PremissCountTemplate
{
public:
	struct Foo
	{
		unsigned int recipe_id;
		unsigned int item_id;
	};
	TemplateCountProduce() : PremissCountTemplate(TEMPL_COUNT_PRODUCE) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.recipe_id&& premiss->parameter2 == foo.item_id)
		{
			if (++data.count >= premiss->parameter3)
				return EVENT_RET_SUCC;
			else
				return EVENT_RET_CHG;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter3; 
	}
	virtual unsigned int GetCondMask() const { return COND_PRODUCE; }
};


//加入战场次数
class TemplateCountBattleJoin : public PremissCountTemplate
{
public:
	struct Foo
	{
		unsigned int battleid;
	};
	TemplateCountBattleJoin() : PremissCountTemplate(TEMPL_COUNT_BATTLEJOIN) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.battleid)
		{
			if (++data.count >= premiss->parameter2)
				return EVENT_RET_SUCC;
			else
				return EVENT_RET_CHG;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter2; 
	}
	virtual unsigned int GetCondMask() const { return COND_BATTLEJOIN; }
};

//死亡次数
class TemplateCountDeath : public PremissCountTemplate
{
public:
	struct Foo {};
	TemplateCountDeath() : PremissCountTemplate(TEMPL_COUNT_DEATH) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		if (++data.count >= premiss->parameter1)
			return EVENT_RET_SUCC;
		else
			return EVENT_RET_CHG;
	}
	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter1; 
	}
	virtual unsigned int GetCondMask() const { return COND_DEATH; }
};

//精炼某等级装备次数
class TemplateCountRefine: public PremissCountTemplate
{
public:
	struct Foo 
	{
		unsigned int spirit;
		unsigned int level_result; 
	};

	TemplateCountRefine() : PremissCountTemplate(TEMPL_COUNT_REFINE) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{	
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if ((premiss->parameter1 == foo.spirit || premiss->parameter1 == 0) && premiss->parameter2 == foo.level_result)
		{
			if (++data.count >= premiss->parameter3)
				return EVENT_RET_SUCC;
			else
				return EVENT_RET_CHG;
		}
		return EVENT_RET_NOCHG;
	}

	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter3; 
	}
	
	virtual unsigned int GetCondMask() const { return COND_REFINECNT;}
};

//连续一周活跃度100分次数
class TemplateCount7Day100P: public PremissCountTemplate
{
public:
	TemplateCount7Day100P() : PremissCountTemplate(TEMPL_COUNT_7DAY_100P) {}
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{	
		if (++data.count >= premiss->parameter1)
			return EVENT_RET_SUCC;
		else
			return EVENT_RET_CHG;
	}

	virtual bool CheckFinish(PremissData & data, Premiss * premiss) 
	{ 
		return data.count >= premiss->parameter1; 
	}
	
	virtual unsigned int GetCondMask() const { return COND_LIVENESS_7DAY_100P;}
};

//流水席战场连杀
typedef TemplateSimple<TEMPL_FLOWKILL, COND_FLOWKILL> _TemplateFlowKill;
class TemplateFlowKill : public _TemplateFlowKill
{
public:
	struct Foo
	{
		unsigned int type;	//1：连杀; 2: 多杀
		int killCnt;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.type)
		{
			if (foo.killCnt >= (int)premiss->parameter2) 
				return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//流水席战场杀死多杀玩家
typedef TemplateSimple<TEMPL_FLOWKILLPLAYER, COND_FLOWKILLPLAYER> _TemplateFlowKillPlayer;
class TemplateFlowKillPlayer : public _TemplateFlowKillPlayer
{
public:
	struct Foo
	{
		int killCnt;
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if ((int)premiss->parameter1 <= foo.killCnt)
		{
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

//流水席战场完成
typedef TemplateSimple<TEMPL_FLOWDONE, COND_FLOWDONE> _TemplateFlowDone;
class TemplateFlowDone : public _TemplateFlowDone
{
public:
	struct Foo
	{
		unsigned int type;	//1：done by kill player only; 2: done by kill monster only; 3: done no death
	};
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition)
	{
		ASSERT(condition.size >= sizeof(Foo));
		Foo & foo = *(Foo*)condition.data;
		if (premiss->parameter1 == foo.type)
		{
			return EVENT_RET_SUCC;
		}
		return EVENT_RET_NOCHG;
	}
	virtual bool CanActive(object_interface player, Premiss * premiss)
	{
		return true;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static TemplateLevel stubTemplateLevel;
static TemplateMoney stubTemplateMoney;
static TemplateReputation stubTemplateReputation;
static TemplateKillMonster stubTemplateKillMonster;
static TemplatePK stubTemplatePK;
static TemplateTask stubTemplateTask;
static TemplateRefine stubTemplateRefine;
static TemplatePetRefine stubTemplatePetRefine;
static TemplateTaliRefine stubTemplateTaliRefine;
static TemplateProduce stubTemplateProduce;
static TemplateEquipment stubTemplateEquipment;
static TemplateAchievement stubTemplateAchievement;
static TemplateLiveness100P stubTemplateLiveness100P;

static TemplateCountTask stubTemplateCountTask;
static TemplateCountKillMonster stubTemplateCountKillMonster;
static TemplateCountProduce stubTemplateCountProduce;
static TemplateCountBattleJoin stubTemplateCountBattleJoin;
static TemplateCountDeath stubTemplateCountDeath;
static TemplateCountRefine stubTemplateCountRefine;
static TemplateCount7Day100P stubTemplateCount7Day100P;
static TemplateFlowKill stubTemplateFlowKill;
static TemplateFlowKillPlayer stubTemplateFlowKillPlayer;
static TemplateFlowDone stubTemplateFlowDone;

}; // namespace AchievementImp


bool player_achieve_man::OnEvent(object_interface player, unsigned int cond_id, const AchievementImp::Condition & condition) 
{
	__PRINTF("发生事件了，cond=%d, trigger_mask=%d\n", cond_id, trigger_mask);
	if (cond_id & trigger_mask)
	{
		__PRINTF("发生关心的事件了，cond=%d, trigger_mask=%d\n", cond_id, trigger_mask);
		abase::vector<unsigned short> finish_list;
		unsigned int new_trigger = 0x0;
		AchievementVector::iterator it = active_achievements.begin();
		for ( ; it != active_achievements.end();)
		{

			// 有前提未完成 
			if (it->unfinished_premiss_count > 0)
			{
				int cond_mask = 0x0;
				if (it->GetConditionMask() & cond_id == 0)
				{
					// 成就和事件不相关
					new_trigger |= it->GetConditionMask();
					++it;
					continue;
				}

				//成就和事件相关
				for (size_t i = 0; i < it->GetTempl()->premiss_count; i++)
				{
					// 检查前提是否完成
					if (it->finished_premiss_mask & (1 << i))
					{
						continue;
					}	

					Premiss * premiss = it->GetTempl()->GetPremiss(i);
					ASSERT(premiss);

					// 前提是否和事件相关
					if (premiss->GetCondMask() == cond_id)
					{
						PremissData & data = it->premiss_data[i];
						int ret = premiss->GetTemplate()->OnEvent(player, data, premiss, condition);
						if (ret == AchievementImp::EVENT_RET_SUCC)
						{
							__PRINTF("完成成就 %d 的前提 %d 了\n", it->GetID(), i);
							player.SendClientPremissFinish(it->GetID(), i);
							it->finished_premiss_mask |= 1 << i;
							it->unfinished_premiss_count --;
							if (it->unfinished_premiss_count == 0) break;
						}
						else if (ret == AchievementImp::EVENT_RET_CHG)
						{
							player.SendClientPremissData(it->GetID(), i,  sizeof(data.count), &data.count); 
						}
					}
					cond_mask |= premiss->GetCondMask();
				}

				if (it->unfinished_premiss_count == 0) 
				{
					finish_list.push_back(it->GetID());

					// 设置成就标志位
					achievement_map.set(0xFFFF & it->GetID(), true);

					// 当所有前提完成,删除该成就
					it = active_achievements.erase(it);
				}
				else
				{
					// 设置新的条件掩码
					it->SetConditionMask(cond_mask);
					++it;
					// 更新新的成就掩码
					new_trigger |= cond_mask;
				}

			}
		}

		//完成了某些成就
		if (finish_list.size())
		{
			for (abase::vector<unsigned short>::iterator it = finish_list.begin(); it != finish_list.end(); ++it)
			{
				// 检查是否可以激活新的成就
				Achievement * achievement = Achievement::GetStub(*it);
				TryActivePostAchievement(player, achievement, new_trigger);

				achieve_point += achievement->bonus;

				__PRINTF("完成成就 %d 了,获得了 %d个成就点\n", *it, achievement->bonus);
				GLog::log(GLOG_INFO,"用户%d完成成就%d",player.GetSelfID().id,*it);

				/*
				if(achievement->reward_item_id > 0)
				{
					player.CreateItem(achievement->reward_item_id, 1, 0);
				}	
				*/
				int finish_time = 0;
				//if(achievement->type > 0)  Youshuang add
				{
					finish_time = g_timer.get_systime();
					achieve_spec_info info;
					info.id = achievement->id;
					info.finish_time = finish_time;
					_special_info.push_back(info);
				}

				if (achievement->broad_type != BROADTYPE_NONE)
				{
					player.SendAchievementMessage(*it, achievement->broad_type, 0, finish_time);
				}
				if(achievement->reward_money > 0)
				{
					player.AddMoney(achievement->reward_money);	
				}
				
				player.DeliverTopicSite( new TOPIC_SITE::achievement( *it ) );  // Youshuang add
				player.SendClientAchieveFinish(*it, achievement->bonus, finish_time);

				// 调用成就完成事件
				OnFinishAchievement(player, *it);
			}
		}
		//设置新的成就掩码
		trigger_mask = new_trigger;
	}

	return true;
}

void split(const char * line, std::vector<std::string> & sub_strings, char div = ',')
{
	int len = strlen(line);
	char * s = const_cast<char*>(line);
	while (len > 0)
	{
		int i = 0;
		for (; i < len && s[i] != div; ++i) {}

		if (i > 0)
		{
			sub_strings.push_back(std::string(s, i));
		}

		++i;
		len -= i;
		s += i;
	}
}

bool Achievement::LoadConfig(const char * file)
{
	if (file && access(file, R_OK) == 0)
	{
		std::ifstream ifs(file);
		std::string line;
		int count = 0;
		while (std::getline(ifs, line))
		{
			count ++;
			if (line.size() == 0 || line[0] == ';' || line[0] == '#' || line[0] == '/')
				continue;
			std::vector<std::string> subs;
			split(line.c_str(), subs);
			/*
				每行中的数据项应该有 
					1 	成就id
				+ 	6 	前置成就数量
				+	10*4 	条件的个数
				+	1	成就点
				+	1	成就完成的广播范围 
				+	1	奖励的金钱
				+	1	奖励的物品ID
				+	1	是否是特殊成就
				= 	52	
			*/
			if (subs.size() != 52) 
			{
				__PRINTF("成就配置文件错误, 行号=%d\n", count);
				ASSERT(false);
			}

			int idx = 0;
			unsigned short id = atoi(subs[idx++].c_str());
			if (GetStub(id)) 
			{
				__PRINTF("成就id重复, id=%d 行号=%d\n", id, count);
				ASSERT(false);
			}
			Achievement * achievement = new Achievement(id); 
			for (int i = 0; i < MAX_PRE_ACHIEVEMENT; i++)
			{
				id = atoi(subs[idx++].c_str());
				if (id) achievement->AddPreAchievement(id);
			}
			for (int i = 0; i < MAX_PREMISS; i++)
			{
				id = atoi(subs[idx++].c_str());
				if (id) 
				{
					PremissTemplate * templ = PremissTemplate::GetTemplate(id);
					if (!templ)
					{
						__PRINTF("没有找到前提模板, 模板=%d 行号=%d\n", id, count);
						ASSERT(false);
					}
					Premiss * premiss = new Premiss(templ, i);
					premiss->SetParameter1(atoi(subs[idx++].c_str()));
					premiss->SetParameter2(atoi(subs[idx++].c_str()));
					premiss->SetParameter3(atoi(subs[idx++].c_str()));
					achievement->AddPremiss(premiss);
				}
				else
				{
					idx += 3;
				}
			}
			achievement->bonus = atoi(subs[idx++].c_str());
			achievement->broad_type = atoi(subs[idx++].c_str());
			achievement->reward_money = atoi(subs[idx++].c_str());
			achievement->reward_item_id = atoi(subs[idx++].c_str());
			achievement->type = atoi(subs[idx++].c_str());
		}
		// 搜集无前提成就和检查成就前提是否正确
		total_achievement_count = count;  // Youshuang add
		BuildHeaders();
		// 检查是否存在循环前提
		return Check();
	}
	return false;
}

using namespace AchievementImp;
bool player_achieve_man::OnLevelUp(object_interface player, int reborn_cnt, int level)
{
	AchievementImp::TemplateLevel::Foo foo = {reborn_cnt, level};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_LEVEL, cond);
}
bool player_achieve_man::OnMoneyInc(object_interface player, int money)
{
	Condition cond(sizeof(int), &money);
	return OnEvent(player, COND_MONEY, cond);
}
bool player_achieve_man::OnReputationInc(object_interface player, int reputation_id, int reputation)
{
	AchievementImp::TemplateReputation::Foo foo = {reputation_id, reputation};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_REPUTATION, cond);
}
bool player_achieve_man::OnKillMonster(object_interface player, int monster_id)
{
	Condition cond(sizeof(int), &monster_id);
	return OnEvent(player, COND_KILLMONSTER, cond);
}
bool player_achieve_man::OnPKValueChange(object_interface player, int pk_val)
{
	Condition cond(sizeof(int), &pk_val);
	return OnEvent(player, COND_PK, cond);
}
bool player_achieve_man::OnFinishTask(object_interface player, int task_id)
{
	Condition cond(sizeof(int), &task_id);
	return OnEvent(player, COND_TASK, cond);
}
bool player_achieve_man::OnRefine(object_interface player, int level_before, int level_result, int spirit)
{
	AchievementImp::TemplateRefine::Foo foo = {level_before, level_result, spirit};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_REFINE, cond);
}
bool player_achieve_man::OnRefine(object_interface player, int spirit, int level_result)
{
	AchievementImp::TemplateRefine::Foo foo = {spirit, level_result};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_REFINECNT, cond);
}
bool player_achieve_man::OnRefinePet(object_interface player, int star_level) 
{
	Condition cond(sizeof(int), &star_level);
	return OnEvent(player, COND_PETREFINE, cond);
}
bool player_achieve_man::OnRefineTali(object_interface player, int combine_value) 
{
	Condition cond(sizeof(int), &combine_value);
	return OnEvent(player, COND_TALIREFINE, cond);
}
bool player_achieve_man::OnProduce(object_interface player, int recipe_id, int item_id )
{
	AchievementImp::TemplateProduce::Foo foo = {recipe_id, item_id};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_PRODUCE, cond);
}
bool player_achieve_man::OnEquip(object_interface player, int itemid)
{
	Condition cond(sizeof(int), &itemid);
	return OnEvent(player, COND_EQUIPMENT, cond);
}
bool player_achieve_man::OnEquip(object_interface player, int index, int spirit, int level)
{
	AchievementImp::TemplateSpecEquip::Foo foo = {index, spirit, level};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_EQUIPMENT, cond);
}
bool player_achieve_man::OnFinishAchievement(object_interface player, int achieve_id)
{
	Condition cond(sizeof(int), &achieve_id);
	return OnEvent(player, COND_ACHIEVEMENT, cond);
}
bool player_achieve_man::OnBattleEnter(object_interface player, int battle_id)
{
	Condition cond(sizeof(int), &battle_id);
	return OnEvent(player, COND_BATTLEJOIN, cond);
}
bool player_achieve_man::OnDeath(object_interface player)
{
	Condition cond(0, 0);
	return OnEvent(player, COND_DEATH, cond);
}

bool player_achieve_man::OnGainTitle(object_interface player, int title_id)
{
	Condition cond(sizeof(int), &title_id);
	return OnEvent(player, COND_TITLE, cond);
}

bool player_achieve_man::OnGainScore(object_interface player, int type, int score)
{
	AchievementImp::TemplateScore::Foo foo = {type, score};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_SCORE, cond);
}

bool player_achieve_man::OnLiveness100P(object_interface player)
{
	int dummy = 0;
	Condition cond(sizeof(dummy), &dummy);
	return OnEvent(player, COND_LIVENESS_100P, cond);
}

bool player_achieve_man::OnLiveness7Day100P(object_interface player)
{
	int dummy = 0;
	Condition cond(sizeof(dummy), &dummy);
	return OnEvent(player, COND_LIVENESS_7DAY_100P, cond);
}

bool player_achieve_man::OnFlowKill(object_interface player, int type, int killcnt)
{
	AchievementImp::TemplateFlowKill::Foo foo = {type, killcnt};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_FLOWKILL, cond);
}

bool player_achieve_man::OnFlowDone(object_interface player, int type)
{
	AchievementImp::TemplateFlowDone::Foo foo = {type};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_FLOWDONE, cond);
}

bool player_achieve_man::OnFlowKillPlayer(object_interface player, int ckillcnt)
{
	AchievementImp::TemplateFlowKillPlayer::Foo foo = {ckillcnt};
	Condition cond(sizeof(foo), &foo);
	return OnEvent(player, COND_FLOWKILLPLAYER, cond);
}


void player_achieve_man::swap(player_achieve_man & rhs)
{
	abase::swap(trigger_mask, rhs.trigger_mask);
	achievement_map.swap(rhs.achievement_map);
	active_achievements.swap(rhs.active_achievements);
	_special_info.swap(rhs._special_info);
	std::swap(achieve_point, rhs.achieve_point);
	std::swap(spend_achieve_point, rhs.spend_achieve_point);
	// Youshuang add
	get_award_map.swap(rhs.get_award_map);
	std::swap( max_fetched, rhs.max_fetched );
	// end
}

void player_achieve_man::TryActivePostAchievement(object_interface player, Achievement * achievement, unsigned int & mask)
{
	for (Achievement::VertexSet::iterator vit = achievement->post_achievements.begin(); vit != achievement->post_achievements.end(); ++vit)
	{
		Achievement * to_active = Achievement::GetStub(*vit);
		if (TryActive(to_active) && to_active->CanActive(player))
		{
			__PRINTF("激活成就 %d 了\n", to_active->GetID());
			active_achievements.push_back(AchievementData());
			AchievementData & data = active_achievements.back();
			data.Init(to_active);
			mask |= to_active->GetCondMask();

			raw_wrapper wrapper(1024);
			wrapper << data.GetID();
			data.GetClientData(wrapper);
			player.SendClientAchieveData(wrapper.size(), wrapper.data());
		}
	}
}

// Youshuang add
void player_achieve_man::InitAwardMap( const void* data, size_t size )
{
	get_award_map.reserve( Achievement::GetTotalAchievementCount() / 8 + ( Achievement::GetTotalAchievementCount() % 8 )? 1 : 0 );
	for( unsigned short i = 0; i < Achievement::GetTotalAchievementCount(); ++i )
	{
		get_award_map.set( i,  false );
	}
	if( data && size != 0 )
	{
		get_award_map.init( (unsigned char*)data, size );
	}
	max_fetched = size;
}

void* player_achieve_man::SaveAchievementAward( size_t& size )
{
	size = max_fetched;
	size_t tmp_size = 0;
	return (void*)get_award_map.data( tmp_size );
}

bool player_achieve_man::GetAchievementAward( unsigned short achieve_id, unsigned int award_id )
{
	if( !GetMap().get( achieve_id ) )
	{ 
		return false; 
	}
	Achievement* pTmp = Achievement::GetStub( achieve_id );
	if( !pTmp || pTmp->reward_item_id != award_id )
	{ 
		return false; 
	}
	if( get_award_map.get( achieve_id ) )
	{ 
		return false; 
	}
	get_award_map.set( achieve_id, true );
	size_t new_size = (size_t)( achieve_id / 8 ) + 1;
	if( max_fetched < new_size )
	{
		max_fetched = new_size;
	}
	return true;
}

int player_achieve_man::GetAchivementFinishedTime( unsigned short achieve_id )
{
	if( !IsFinished( achieve_id ) ){ return 0; }
	for( abase::vector<achieve_spec_info>::const_iterator iter = _special_info.begin(); iter != _special_info.end(); ++iter )
	{
		if( achieve_id == iter->id )
		{
			return iter->finish_time;
		}
	}
	return -1;
}
// end
