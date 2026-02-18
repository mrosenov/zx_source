#ifndef __ONLINEGAME_GS_FILTER_MANAGER_H__
#define __ONLINEGAME_GS_FILTER_MANAGER_H__

#include "filter.h"
#include <vector.h>
#include <algorithm>
#include <functional>
#include "staticmap.h"
#include <amemory.h>

#define FILTER_MAP_FUNC_DEFINE(Name, Declare, Call, IDX)  inline void EF_##Name Declare\
{ \
	OperationAux  op(*this);\
	list & __list = _filter_pool[IDX];\
	for(list::iterator it = __list.begin(); it != __list.end(); )\
	{\
		filter * obj = *it;\
		if(obj->IsActived())\
		{\
			obj->Name Call;\
			if(obj->_is_deleted) \
			{\
				obj->_mask &= ~(1 << IDX);/*手动清除MASK*/\
				it = __list.erase(it);\
				op.RemoveFilter(obj);/*试图删除filter本身*/\
				continue;\
			}\
		}\
		++it;\
	}\
	return ;\
}

#define XDEF(x) x

class gactive_imp;
class filter_man
{
	typedef abase::static_multimap<int,filter*, abase::fast_alloc<> >  FILTER_MAP;
	typedef abase::vector<filter*,abase::fast_alloc<> > list;
	list _filter_pool[FILTER_IDX_MAX];
	FILTER_MAP _filter_map;
	list _wait_for_insert;
	list _wait_for_delete;
	int _operation_count;
	int _min_timeout_time;

	class OperationAux
	{
		filter_man & _man;
		public:
		OperationAux(filter_man & man):_man(man)
		{
			_man._operation_count ++;
		}
		~OperationAux()
		{
			_man._operation_count --;
			if(_man._operation_count == 0)
			{
				//操作末尾
				_man.__DelayExecute();
			}
		}

		void AddFilter(filter * obj)
		{
			if(_man._operation_count == 1)
			{
				_man.__RawAddFilter(obj);
			}
			else
			{
				_man._wait_for_insert.push_back(obj);
			}
		}

		bool RemoveFilter(FILTER_MAP::iterator &it)
		{
			bool bRst = false;
			filter * old = it->second;
			if(_man._operation_count == 1)
			{
				if(old->IsActived())
				{
					_man.__RemoveFilterFromPool(old);
					old->Release();
					it = _man._filter_map.erase(it);
					return true;
				}
				//这里是已经在待删除列表里了 所以不能直接删除 这种情况也有可能存在
			}
			else
			{
				if(old->IsActived())
				{
					//只有激活的filter才会加入等待删除列表，这样是为了避免多次删除
					old->Detach();
					_man._wait_for_delete.push_back(old);
					bRst = true;
				}
			}
			++it;
			return bRst;
		}

		bool RemoveFilter(filter * old)
		{
			if(old->IsActived())
			{
				//直接放入等待删除列表，因为这里处理也并不简单多少
				old->Detach();
				_man._wait_for_delete.push_back(old);
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	void __RawAddFilter(filter * obj);
	void __RemoveFilterFromPool(filter * obj);
	void __DelayExecute();
	void __RecordNewTimeout(int t);
	void __ScanTimeoutFilter(int cur_t);
	void __RawDelFilter(filter * obj);

public:
	filter_man():_operation_count(0),_min_timeout_time(0)
	{}

	~filter_man()
	{
		Clear();
	}

	void Clear();
	void Init(gactive_imp * imp);
	void DBInit(gactive_imp * imp);
	void Swap(filter_man & rhs, gactive_imp * imp);

	inline bool IsFilterExist(int id) { return !(_filter_map.find(id) == _filter_map.end()); }

	bool AddFilter(filter * obj);
	void RemoveFilter(int id);
	int ClearSpecFilter(int mask = filter::FILTER_MASK_DEBUFF, int max_count = 0);
	int ClearRandomSpecFilter(int mask, int max_count);
	bool ModifyFilter(int filterid, int ctrlname, void * ctrlval, size_t ctrllen);
	int  FilterCnt(int buff_type); 

public:
	void Heartbeat(int tick);
	FILTER_MAP_FUNC_DEFINE(Heartbeat,(int tick),(tick),FILTER_IDX_HEARTBEAT)
	FILTER_MAP_FUNC_DEFINE(TranslateSendAttack,(const XID& target,attack_msg& arg,char type,float radius,int max_cnt),(target,arg,type,radius,max_cnt),FILTER_IDX_TRANSLATE_SEND_MSG)
	FILTER_MAP_FUNC_DEFINE(TranslateSendEnchant,(const XID& target,enchant_msg& arg),(target,arg),FILTER_IDX_TRANSLATE_SEND_ENCHANT)
	FILTER_MAP_FUNC_DEFINE(TranslateRecvAttack,(const XID& target,attack_msg& arg),(target,arg),FILTER_IDX_TRANSLATE_RECV_MSG)
	FILTER_MAP_FUNC_DEFINE(TranslateRecvEnchant,(const XID& target,enchant_msg& arg),(target,arg),FILTER_IDX_TRANSLATE_ENCHANT)
	FILTER_MAP_FUNC_DEFINE(AdjustDamage,(damage_entry& dmg,const XID& attacker,const attack_msg& msg),(dmg,attacker,msg),FILTER_IDX_ADJUST_DAMAGE)
	FILTER_MAP_FUNC_DEFINE(BeforeDamage,(int & damage),(damage),FILTER_IDX_BEFORE_DAMAGE)
	FILTER_MAP_FUNC_DEFINE(BeforeDeath,(bool is_duel),(is_duel),FILTER_IDX_BEFORE_DEATH)
	FILTER_MAP_FUNC_DEFINE(AdjustManaCost,(int &mana),(mana),FILTER_IDX_ADJUST_MANA_COST)
	FILTER_MAP_FUNC_DEFINE(BeAttacked,(const XID & target, int damage, bool crit),(target, damage, crit),FILTER_IDX_BE_ATTACKED)
	FILTER_MAP_FUNC_DEFINE(BeReflected,(const XID & target),(target),FILTER_IDX_BE_REFLECTED)
	FILTER_MAP_FUNC_DEFINE(OnFilterAdd,(int filter_id, const XID & caster),(filter_id, caster),FILTER_IDX_ON_FILTER_ADD)
	FILTER_MAP_FUNC_DEFINE(CritEnemy,(const XID& target),(target),FILTER_IDX_CRIT_ENEMY)
	FILTER_MAP_FUNC_DEFINE(DamageEnemy,(const XID& target, int damage),(target, damage),FILTER_IDX_DAMAGE_ENEMY)	
	FILTER_MAP_FUNC_DEFINE(AdjustSkill,(int skill_id, short& level),(skill_id, level),FILTER_IDX_ADJUST_SKILL)
	FILTER_MAP_FUNC_DEFINE(AfterDeath,(bool is_duel),(is_duel),FILTER_IDX_AFTER_DEATH)
	FILTER_MAP_FUNC_DEFINE(OnMoved,(),(),FILTER_IDX_ON_MOVED)
	
	bool Save(archive & ar);
	bool Load(archive & ar);
	bool SaveSpecFilters(archive & ar, int mask); //用于存盘，存储的方式和Save保持一致
};

enum
{	
	FILTER_INDEX_SKILL_SESSION 	= 13,
	FILTER_INVINCIBLE		= 14,
	FILTER_INDEX_PVPLIMIT		= 15,
	FILTER_INDEX_GATHER_SESSION 	= 16,
	FILTER_INDEX_PASSIVE_WAIT	= 17,
	FILTER_INDEX_SITDOWN		= 18,
	FILTER_INVINCIBLE_SPEC_ID	= 20,
	FILTER_INVINCIBLE_BANISH	= 21,
	FILTER_CHECK_INSTANCE_KEY	= 22,
	FILTER_INDEX_DUEL_FILTER	= 23,
	FILTER_INDEX_MOUNT_FILTER	= 24,
	FILTER_INDEX_TOWERBUILD		= 25,
	FILTER_INDEX_NO_USE		= 26,
	FITLER_INDEX_POTION_RESIST	= 27,
	FILTER_INDEX_HEALING		= 28,
	FILTER_INDEX_MANA		= 29,
	FILTER_INDEX_REJUVENATION	= 30,
	FILTER_INDEX_AIRCRAFT		= 31,
	FILTER_INDEX_PET_COMBINE1	= 32,
	FILTER_INDEX_PET_COMBINE2	= 33,
	FILTER_INDEX_KILL		= 34,
	FILTER_INDEX_ONLINE_AGENT	= 35,
	FILTER_BATTLEGROUND_INVINCIBLE	= 36,
	FILTER_INDEX_ITEM_TRANSFORM	= 37,
	FILTER_INDEX_SKILL_TRANSFORM	= 38,
	FILTER_INDEX_TASK_TRANSFORM	= 39,
	FILTER_INDEX_LOGON_INVINCIBLE	= 40,
	FILTER_INDEX_AZONE		= 41,
	FILTER_INDEX_DEITY		= 42,
	FILTER_INDEX_COD_SPONSOR = 43,
	FILTER_INDEX_COD_MEMBER  = 44,
	FILTER_INDEX_BATTLEFLAG  = 45,
	FILTER_INDEX_KINGDOM_BATTLEPREPARE = 46,
	FILTER_INDEX_BUFFAREA_TRANFORM	= 47,
	FILTER_INDEX_KINGDOM_EXP = 48,
	FILTER_INDEX_KINGDOM_BATH = 49,
	FILTER_INDEX_TELEPORT1 = 50,
	FILTER_INDEX_TELEPORT2 = 51,
	FILTER_INDEX_RAGE = 52,
	FILTER_INDEX_COD_COOLDOWN = 53,
	FILTER_INDEX_ACTIVE_EMOTE = 54,
	FILTER_INDEX_TRAVEL_AROUND = 55,
	FILTER_INDEX_TRY_CHARGE	= 56,


	FILTER_INDEX_SKILL_BASE = 4096,
	FILTER_INDEX_INVISIBLE	= 4178,
	FILTER_INDEX_SHARELIFEWITHCLONE = 4183,
	FILTER_INDEX_SHARESTATEWITHCLONE = 4184,
	FILTER_INDEX_SKILL_END  = 8191,
};

#endif

