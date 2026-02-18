#ifndef __ONLINEGAME_GS_ACHIEVEMENT_H__
#define __ONLINEGAME_GS_ACHIEVEMENT_H__ 

#include "octets.h"
#include "dbgprt.h"
#include <common/base_wrapper.h>
#include <hashmap.h>
#include "staticmap.h"
#include "obj_interface.h"

enum 
{ 
	MAX_PRE_ACHIEVEMENT = 6,
	MAX_PREMISS = 10, 
};

enum ACHIEVE_BROADTYPE
{
	BROADTYPE_NONE,
	BROADTYPE_LOCAL, //本地喊话
	BROADTYPE_ALL,	 //全服喊话
};

enum CONDTIONMASK
{
	COND_LEVEL 		= 0x00000001, 	// 人物等级
	COND_MONEY		= 0x00000002,	// 金钱
	COND_REPUTATION		= 0x00000004,	// 声望
	COND_KILLMONSTER	= 0x00000008,	// 杀死怪物
	COND_PK			= 0x00000010,	// pk
	COND_TASK		= 0x00000020,	// 完成任务
	COND_REFINE		= 0x00000040,	// 精炼物品
	COND_PETREFINE		= 0x00000080,	// 宠物修炼
	COND_TALIREFINE		= 0x00000100,	// 血炼
	COND_PRODUCE		= 0x00000200,	// 生产物品
	COND_EQUIPMENT		= 0x00000400,	// 装备物品
	COND_ACHIEVEMENT	= 0x00000800,	// 达成成就
	COND_BATTLEJOIN		= 0x00001000,	// 加入战场
	COND_DEATH		= 0x00002000,	// 死亡 
	COND_TITLE		= 0x00004000,	// 获得称谓 
	COND_SCORE		= 0x00008000,	// 获得分数
	COND_REFINECNT		= 0x00010000,	// 精炼，用于计数
	COND_SPECEQUIP		= 0x00020000,	// 特定装备 
	COND_LIVENESS_100P		= 0x00040000,	//  活跃度100分
	COND_LIVENESS_7DAY_100P	= 0x00080000,	//  活跃度连续7天100分
	COND_FLOWKILL     		= 0x00100000, 	//  流水席战场多杀和连杀
	COND_FLOWDONE 			= 0x00200000,	//  流水席战场达到目标
	COND_FLOWKILLPLAYER 	= 0x00400000,	//  流水席战场击杀连杀玩家
};

enum PREMISS_STATUS { PREMISS_UNSATISFIED, PREMISS_SATISFIED, };

enum PREMISS_TEMPLATE 
{
//	普通条件
	// 1-5
	TEMPL_LEVEL 		= 1, 	// 人物等级
	TEMPL_MONEY		,	// 金钱
	TEMPL_REPUTATION	,	// 声望
	TEMPL_KILLMONSTER	,	// 杀死怪物
	TEMPL_PK		,	// pk
	// 6-10
	TEMPL_TASK		,	// 完成任务
	TEMPL_REFINE		,	// 精炼物品
	TEMPL_PETREFINE		,	// 宠物修炼
	TEMPL_TALIREFINE	,	// 法宝血炼 
	TEMPL_PRODUCE		,	// 生产物品
	// 11-15
	TEMPL_EQUIPMENT		,	// 装备物品
	TEMPL_ACHIEVEMENT	,	// 达成成就
	TEMPL_TITLE		,	// 获得称谓 
	TEMPL_SCORE		,	// 获得分数 
	TEMPL_SPECEQUIP		,	// 装备道具
	// 16-20
	TEMPL_LIVENESS_100P	,	// 活跃度100分
	TEMPL_FLOWKILL,			// 流水席战场击杀玩家
	TEMPL_FLOWDONE,			// 流水席战场完成
	TEMPL_FLOWKILLPLAYER,	// 流水席战场杀死连杀玩家记录

//	计数条件
	// 1000 - 1004
	TEMPL_COUNT_TASK	= 1000,	// 完成任务次数
	TEMPL_COUNT_KILLMONSTER ,	// 杀死怪物次数
	TEMPL_COUNT_PRODUCE	,	// 生产配方的某物品次数
	TEMPL_COUNT_BATTLEJOIN	,	// 参加战场次数
	TEMPL_COUNT_DEATH	,	// 死亡次数 
	// 1005 - 1009
	TEMPL_COUNT_REFINE	,	// 精炼某个等级的次数 
	TEMPL_COUNT_7DAY_100P	,	// 活跃度连续7天100分
}; 

namespace AchievementImp { class Condition; };
class Premiss;
class PremissData;

class PremissTemplate
{
	typedef abase::hash_map<unsigned short, PremissTemplate* > TEMPLATE_MAP;
public:
	PremissTemplate(unsigned short templ_id) 
		: template_id(templ_id)
	{
		s_stubs[template_id] = this;
	}
	virtual ~PremissTemplate() {}

	static PremissTemplate * GetTemplate(unsigned short temp_id) 
	{ 
		TEMPLATE_MAP::iterator it = s_stubs.find(temp_id);
		if (it != s_stubs.end())
			return it->second;
		return 0;
	}
	static TEMPLATE_MAP s_stubs;

	/* 
		OnEvent 返回值: 

	   	0: 满足前提条件了
		1: 不满足条件，并且前提的数据没有变化
		2: 不满足条件，但是改变了前提的数据

	 */
	virtual int OnEvent(object_interface player, PremissData & data, Premiss * premiss, const AchievementImp::Condition & condition) = 0;
	virtual bool CheckFinish(PremissData & data, Premiss * premiss) = 0;
	virtual bool CanActive(object_interface player, Premiss * premiss) = 0;

	virtual char GetParaSize() const = 0;
	virtual unsigned int GetCondMask() const = 0;
	virtual bool Load(PremissData & data, archive & ar) = 0;
	virtual bool Save(PremissData & data, archive & ar) = 0;
 
	unsigned short GetID() const { return template_id; }
	unsigned short template_id;
};


class Premiss
{
	typedef abase::hash_map<unsigned short, Premiss*> PREMISS_MAP;
public:

	Premiss(PremissTemplate * p_temp, unsigned short ident) 
		: premiss_id(ident), parameter1(0), parameter2(0), parameter3(0), ptempl(p_temp)
	{
	}

	bool Load(PremissData & data, archive & ar) { return ptempl->Load(data, ar); }
	bool Save(PremissData & data, archive & ar) { return ptempl->Save(data, ar); }
	bool IsFinished(PremissData & data) { return ptempl->CheckFinish(data, this); }
	bool CanActive(object_interface player) { return ptempl->CanActive(player, this); }
	
	PremissTemplate * GetTemplate() { return ptempl; }
	unsigned int GetCondMask() const { return ptempl->GetCondMask(); }
	void SetParameter1(unsigned int para) { parameter1 = para; }
	void SetParameter2(unsigned int para) { parameter2 = para; }
	void SetParameter3(unsigned int para) { parameter3 = para; }
	unsigned char GetID() const { return premiss_id; }
	char GetParaSize() const { return ptempl->GetParaSize();}

	unsigned short premiss_id;
	unsigned int   parameter1;
	unsigned int   parameter2;
	unsigned int   parameter3;
	PremissTemplate * ptempl;

	friend class PremissTemplate;

};

struct PremissData 
{
	unsigned char premiss_id;
	unsigned char b_finished;
	unsigned short count;
	PremissData() : premiss_id(0), b_finished(0), count(0) { }
	PremissData(Premiss * premiss) : premiss_id(premiss->GetID()), b_finished(0), count(0) { }
	PremissData(const PremissData & o ) : premiss_id(o.premiss_id), b_finished(o.b_finished), count(o.count) {}
	PremissData & operator=(const PremissData & o)
	{
		if (&o != this)
		{
			premiss_id = o.premiss_id;
			b_finished = o.b_finished;
			count = o.count;
		}
		return *this;
	}
};
typedef abase::vector<PremissData, abase::fast_alloc<> > PREMISSDATA_VEC;
typedef abase::bitmap<> Bitmap;
class Achievement;
class AchievementQuery
{
public:
	AchievementQuery() {}

	virtual void Update(Achievement & achievement) = 0;
	virtual ~AchievementQuery() {}
};

class Achievement
{
	typedef abase::vector<unsigned short, abase::fast_alloc<> > VertexSet;
	typedef abase::hash_map<unsigned short, Achievement*> ACHIEVEMENT_MAP;
	typedef abase::static_set<unsigned short, abase::fast_alloc<> > HeaderSet;
	typedef abase::vector<Premiss*, abase::fast_alloc<> > PREMISS_VEC;
public:

	Achievement(unsigned short _id) : id(_id), premiss_mask(0), cond_mask(0), premiss_count(0), bonus(0)
	{
		if (NULL == GetStub(id)) 
		{
			s_stubs[id] = this;
			memset(premisses, 0, MAX_PREMISS*sizeof(Premiss*));
			memset(premiss_data, 0, MAX_PREMISS*sizeof(PremissData));
		}
	}

	static Achievement * GetStub(unsigned short id)
	{
		ACHIEVEMENT_MAP::iterator it = s_stubs.find(id);
		if (it != s_stubs.end())
			return it->second;
		return 0;
	}
	static ACHIEVEMENT_MAP s_stubs;
	static HeaderSet s_headers;
	
	static void Walk(AchievementQuery & query)
	{
		ACHIEVEMENT_MAP::iterator it = s_stubs.begin();
		for (; it != s_stubs.end(); ++it) query.Update(*it->second);
	}

	static bool LoadConfig(const char * file);

	void AddPremiss(Premiss * premiss) 
	{ 
		ASSERT(premiss->GetID() < MAX_PREMISS);
		premisses[premiss_count] = premiss;
		premiss_data[premiss_count] = PremissData(premiss);
		premiss_mask |= 1 << premiss_count; 
		cond_mask |= premiss->GetCondMask(); 
		premiss_count++;
	}
	bool CanActive(object_interface player)
	{
		for (size_t i = 0; i < premiss_count; i++)
		{
			if (!premisses[i]->CanActive(player)) return false;
		}
		return true;
	}
	void AddPreAchievement(unsigned short aid) { pre_achievements.push_back(aid);  }
	void AddPostAchievement(unsigned short aid) { post_achievements.push_back(aid); }

	unsigned int GetCondMask() const { return cond_mask; }
	unsigned short GetID() const { return id; }
	static unsigned short GetTotalAchievementCount(){ return total_achievement_count; }  // Youshuang add
private:
	static void BuildHeaders()
	{
		ACHIEVEMENT_MAP::iterator it = s_stubs.begin();
		for (; it != s_stubs.end(); ++it)
		{
			Achievement * a = it->second;
			if (a->pre_achievements.size() == 0) s_headers.insert(it->first);

			for (VertexSet::iterator pit = a->pre_achievements.begin(); pit != a->pre_achievements.end(); ++pit)
			{
				Achievement * pre = GetStub(*pit);
				if (!pre)
				{
					__PRINTF("没有找到前提成就, 成就=%d 前提成就=%d\n", a->GetID(), *pit);
					ASSERT(false);
				}
				pre->AddPostAchievement(a->GetID());
			}
		}
	}
	static bool Check()
	{
		HeaderSet leaf_achievements, root_achievements;

		ACHIEVEMENT_MAP::iterator it = s_stubs.begin();
		for (; it != s_stubs.end(); ++it)
		{
			if (it->second->post_achievements.size() == 0) 
				leaf_achievements.insert(it->first);
			else
				root_achievements.insert(it->first);
		}

		while (root_achievements.size())
		{
			if (!ClearEdgeToLeaf(root_achievements, leaf_achievements))
			{
				__PRINTF("成就中存在前置成就环!!!!!!\n");
				return false;
			}
		}
		__PRINTF("成就模板检查通过\n");
		return true;
	}
	static bool ClearEdgeToLeaf(HeaderSet & root, HeaderSet & leaf)
	{
		for (HeaderSet::iterator it = root.begin(); it != root.end();)
		{
			Achievement * a = GetStub(*it);
			VertexSet::iterator pit = a->post_achievements.begin();
			for (; pit != a->post_achievements.end(); ++pit) if (leaf.find(*pit) == leaf.end()) break;

			if (pit == a->post_achievements.end())
			{
				leaf.insert(*it);
				root.erase(it);
				return true;
			}
			++it;
		}
		return false;
	}
	Premiss * GetPremiss(int index)
	{
		ASSERT(index >= 0 && (unsigned char)index < premiss_count);
		return premisses[index];
	}
	unsigned short id;
	unsigned short premiss_mask;
	unsigned int cond_mask;
	unsigned char premiss_count;
	Premiss * premisses[MAX_PREMISS];
	PremissData premiss_data[MAX_PREMISS];
	VertexSet pre_achievements;
	VertexSet post_achievements;
	unsigned int  bonus;
	int	broad_type;
	unsigned int reward_money;
	unsigned int reward_item_id;
	unsigned char type;			//成就类型 0-默认  1-特殊
	static unsigned short total_achievement_count;  // Youshuang add

	friend class AchievementQuery;
	friend class player_achieve_man;
	friend class AchievementData;
};

class AchievementData
{
public:
	unsigned short finished_premiss_mask;
	unsigned char unfinished_premiss_count;
	unsigned int  condition_mask; 
	PremissData premiss_data[MAX_PREMISS];

	AchievementData() 
		: finished_premiss_mask(0x0000), unfinished_premiss_count(0), condition_mask(0x00000000), achievement(NULL)
	{
		memset(premiss_data, 0, MAX_PREMISS*sizeof(PremissData));
	}
	void Init(Achievement * _achievement) 
	{ 
		achievement = _achievement;
		finished_premiss_mask = 0x0000;
		unfinished_premiss_count = _achievement->premiss_count;
		condition_mask = _achievement->GetCondMask();
		for (size_t i = 0; i < _achievement->premiss_count; i++) 
			premiss_data[i] = _achievement->premiss_data[i];
	}

	AchievementData & operator=(const AchievementData & o)
	{
		if (&o != this)
		{
			//ASSERT(false && "到这里了，安全吧!");
			finished_premiss_mask = o.finished_premiss_mask;
			unfinished_premiss_count = o.unfinished_premiss_count;
			condition_mask = o.condition_mask;
			achievement = o.achievement;
			for (size_t i = 0; i < MAX_PREMISS; i++) premiss_data[i] = o.premiss_data[i];
		}
		return *this;	
	}

	bool Load(archive &ar)
	{
		unsigned char count;
		ar >> count;
		ASSERT(count <= achievement->premiss_count);
		for (unsigned char i = 0; i < count; i++)
		{
			unsigned char premiss_id;
			ar >> premiss_id;
			Premiss * premiss = achievement->GetPremiss(premiss_id);
			PremissData & data = premiss_data[premiss_id];
			premiss->Load(data, ar);
		}
		// 收集前提条件信息和完成数量
		condition_mask = 0;
		finished_premiss_mask = 0x0000;
		unfinished_premiss_count = achievement->premiss_count;
		for (size_t i = 0; i < achievement->premiss_count; i++)
		{
			Premiss * premiss = achievement->GetPremiss(i);
			if (premiss->IsFinished(premiss_data[i]))
			{
				finished_premiss_mask |= 1 << i;
				unfinished_premiss_count--;
			}
			else
			{
				condition_mask |= premiss->GetCondMask();
			}
		}
		return true;
	}

	bool Save(archive &ar)
	{
		ar << (unsigned char)achievement->premiss_count;
		for (size_t i = 0; i < achievement->premiss_count; i++)
		{
			Premiss * premiss = achievement->GetPremiss(i);
			if (NULL == premiss)
			{
				ASSERT(false && "前提没有找到");
				return false;
			}
			ar << (unsigned char)premiss->GetID();
			premiss->Save(premiss_data[i], ar);
		}
		return true;
	}
	bool GetClientData(archive & ar)
	{
		ar << (unsigned short)finished_premiss_mask;
		ar << (unsigned char)unfinished_premiss_count;
		for (size_t i = 0; i < achievement->premiss_count; i++)
		{
			if (finished_premiss_mask & (1 << i)) 
			{
				// 该前提已经完成，不需要告诉客户端前提数据
				continue;
			}
			Premiss * premiss = achievement->GetPremiss(i);
			if (NULL == premiss)
			{
				ASSERT(false && "前提没有找到");
				return false;
			}
			ar << (unsigned char)premiss->GetID();
			premiss->Save(premiss_data[i], ar);

		}
		return true;
	}

	unsigned short GetID() const { return achievement->GetID(); }
	unsigned int GetConditionMask() const { return condition_mask; }
	void SetConditionMask(unsigned int cond_mask) { condition_mask = cond_mask; }
	Achievement * GetTempl() { return achievement; }
private:
	Achievement * achievement;
};

#pragma pack(1)
struct achieve_spec_info 
{
	unsigned short id;
	int finish_time; 
};
#pragma pack()

class player_achieve_man
{
public:
	typedef abase::vector<AchievementData, abase::fast_alloc<> > AchievementVector;

	player_achieve_man (): trigger_mask(0), achieve_point(0), spend_achieve_point(0)
	{
	}
	void swap(player_achieve_man & rhs);

	// 外部调用接口开始

	bool OnLevelUp(object_interface player, int reborn_cnt, int level);
	bool OnMoneyInc(object_interface player, int money);
	bool OnReputationInc(object_interface player, int reputation_id, int reputation);
	bool OnKillMonster(object_interface player, int monster_id);
	bool OnPKValueChange(object_interface player, int pk_val);
	bool OnFinishTask(object_interface player, int task_id);
	bool OnRefine(object_interface player, int equip_level, int refine_result, int spirit);
	bool OnRefine(object_interface player, int spirit, int refine_result); //精炼用于计数
	bool OnRefinePet(object_interface player, int star_level); 
	bool OnRefineTali(object_interface player, int combine_value);
	bool OnProduce(object_interface player, int recipe_id, int item_id);
	bool OnEquip(object_interface player, int itemid);
	bool OnEquip(object_interface player, int index, int spirit, int level);
	bool OnFinishAchievement(object_interface player, int achieve_id);
	bool OnBattleEnter(object_interface player, int battle_id);
	bool OnDeath(object_interface player);
	bool OnGainTitle(object_interface player, int title_id);
	bool OnGainScore(object_interface player, int type, int score);
	bool OnLiveness100P(object_interface player);
	bool OnLiveness7Day100P(object_interface player);
	bool OnFlowKill(object_interface player, int type, int killcnt);
	bool OnFlowDone(object_interface player, int type);
	bool OnFlowKillPlayer(object_interface player, int ckillcnt);


	// 外部调用接口结束

	void InitMap(const void * data, size_t size) 
	{ 
		achievement_map.init((unsigned char*)data, size); 
	}

	Bitmap & GetMap() { return achievement_map; }

	// Youshuang add
	Bitmap & GetAwardMap() { return get_award_map; }	
	void InitAwardMap( const void* data, size_t size );
	void* SaveAchievementAward( size_t& size );
	bool GetAchievementAward( unsigned short achieve_id, unsigned int award_id );
	int GetAchivementFinishedTime( unsigned short achieve_id );
	// end
	
	bool LoadActiveAchievement(archive &ar)
	{
		try 
		{
			unsigned short achieve_count;
			ar >> achieve_count;
			active_achievements.reserve(achieve_count);
			for (unsigned short i = 0; i < achieve_count; i++)
			{
				unsigned short achieve_id;
				ar >> achieve_id;
				Achievement * achievement = Achievement::GetStub(achieve_id);
				if (NULL == achievement)
				{
					__PRINTF("没有找到对应的成就%d\n", achieve_id);
					//发生这种错误的话可能是成就的配置文件不一致造成的(achieve.txt)
					ASSERT(false);
					continue;
				}
				active_achievements.push_back(AchievementData());
				AchievementData & data = active_achievements.back();
				data.Init(achievement);
				data.Load(ar);
				trigger_mask |= data.GetConditionMask();
			}
			ar >> achieve_point;
			ar >> spend_achieve_point;
		} 
		catch (...)
		{
			__PRINTF("初始化激活成就失败\n");
		}
		return true;
	}
	bool GetClientActiveAchievement(archive &ar)
	{
		ar << (unsigned short)active_achievements.size();
		AchievementVector::iterator it = active_achievements.begin();
		for (; it != active_achievements.end(); ++it) 
		{
			ar << (unsigned short)it->GetID();
			it->GetClientData(ar);	
		}
		ar << achieve_point;
		ar << achieve_point - spend_achieve_point;
		return true;
	}
	bool SaveActiveAchievement(archive &ar)
	{
		ar << (unsigned short)active_achievements.size();
		AchievementVector::iterator it = active_achievements.begin();
		for (; it != active_achievements.end(); ++it) 
		{
			ar << (unsigned short)it->GetID();
			it->Save(ar);	
		}
		ar << achieve_point;
		ar << spend_achieve_point;
		return true;
	}
	
	bool TryActive(unsigned short id)
	{
		Achievement * achievement = Achievement::GetStub(id);
		return achievement ? TryActive(achievement) : false;
	}

	void CheckAchievements(object_interface player)
	{
		struct LoadQuery : public AchievementQuery
		{
		public:
			LoadQuery(player_achieve_man *acm, object_interface _player) : p_acm(acm), player(_player)
			{
				AchievementVector & active_vec = p_acm->GetActiveAchievement();
				for (AchievementVector::iterator it = active_vec.begin(); it != active_vec.end(); ++it) active_map.set(0xFFFF & it->GetID(), true);
			}
			virtual ~LoadQuery() {}
			virtual void Update(Achievement & achievement)
			{
				if (p_acm->GetMap().get(achievement.GetID()) || active_map.get(achievement.GetID())) return;			
				Achievement::VertexSet::iterator it = achievement.pre_achievements.begin(), ie = achievement.pre_achievements.end();
				for (; it != ie; ++it)
				{
					if (!p_acm->GetMap().get(*it)) break;
				}
				if (it == ie && achievement.CanActive(player))
				{
					to_active.push_back(&achievement);
				}
			}
			player_achieve_man * p_acm;
			object_interface player;
			Bitmap active_map;
			abase::vector<Achievement*, abase::fast_alloc<> > to_active;

		};
		bool b_loop;
		do 
		{
			b_loop = false;
			LoadQuery q(this, player);
			Achievement::Walk(q);
			for (abase::vector<Achievement*, abase::fast_alloc<> >::iterator it = q.to_active.begin(); it != q.to_active.end(); ++it)
			{
				if ((*it)->premiss_count == 0)
				{
					__PRINTF("完成天生成就 %d 了\n", (*it)->GetID());
					GetMap().set((*it)->GetID(), true);
					achieve_point += (*it)->bonus; 
					b_loop = true;
					continue;
				}
				active_achievements.push_back(AchievementData());
				active_achievements.back().Init(*it);
				trigger_mask |= (*it)->GetCondMask();
			}

		}
		while (b_loop);
	}

	AchievementVector  & GetActiveAchievement() { return active_achievements; }
	bool IsActive(unsigned short id) const 
	{ 
		for (AchievementVector::const_iterator it = active_achievements.begin(); it != active_achievements.end(); ++it)
		{
			if (it->GetID() == id) return true;
		}
		return false;
	}
	bool IsFinished(unsigned short id)
	{
		return Achievement::GetStub(id) && GetMap().get(id);

	}
	bool FinishAchievement(object_interface player, unsigned short id)
	{
		Achievement * achievement = Achievement::GetStub(id);
		if (NULL == achievement) return false;

		if (GetMap().get(id) ) return true;
		GetMap().set(id&0xFFFF, true);
		for (AchievementVector::iterator it = active_achievements.begin(); it != active_achievements.end(); ++it)
		{
			if (it->GetID() == id) 
			{
				active_achievements.erase(it);
				achieve_point += it->GetTempl()->bonus; 
				break;
			}
		}
		__PRINTF("直接完成成就 %d 了\n", id);
		TryActivePostAchievement(player, achievement, trigger_mask);
		return true;
	}

	bool UnFinishAchievement(object_interface player, unsigned short id)
	{
		Achievement * achievement = Achievement::GetStub(id);
		if (NULL == achievement) return false;

		if (!GetMap().get(id) ) return true;
		GetMap().set(id&0xFFFF, false);
		return true;
	}

	int GetAchievementPoint() const { return achieve_point - spend_achieve_point; }

	void * GetSpecialAchievementInfo(size_t & spec_size)
	{
		spec_size = _special_info.size() * sizeof(achieve_spec_info); 
		if(_special_info.empty()) return NULL;
		return _special_info.begin();
	}

	void LoadSpecialAchievementInfo(const void *buf, size_t size)
	{
		if(size == 0) return;
		ASSERT(size % sizeof(achieve_spec_info) == 0);

		const achieve_spec_info * info = (const achieve_spec_info *)buf;
		size_t count = size / sizeof(achieve_spec_info);
		for(size_t i = 0; i < count; ++i)
		{
			_special_info.push_back(info[i]);
		}
	}

	void DebugAddAchievementPoint(int inc) { achieve_point += inc;}
	
private:
	void TryActivePostAchievement(object_interface player, Achievement * achievement, unsigned int & mask);
	
	bool TryActive(Achievement * achievement)
	{
		if (GetMap().get(achievement->GetID()) || IsActive(achievement->GetID())) return false;			

		Achievement::VertexSet::iterator it = achievement->pre_achievements.begin();
		for (; it != achievement->pre_achievements.end(); ++it) 
			if (!GetMap().get(*it))  return false;

		return true;
	}

	bool OnEvent(object_interface player, unsigned int cond_id, const AchievementImp::Condition & condition);

	unsigned int trigger_mask;
	Bitmap achievement_map;
	// Youshuang add
	Bitmap get_award_map;
	size_t max_fetched;
	// end
	AchievementVector active_achievements;
	int 	achieve_point;
	int	spend_achieve_point;
	abase::vector<achieve_spec_info> _special_info;
	
};

#endif //__ACHIEVEMENT_GS_INCLUDE_HPP__
