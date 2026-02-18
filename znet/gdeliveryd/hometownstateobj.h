#ifndef _GNET_HOMETOWN_STATEOBJ_H_
#define _GNET_HOMETOWN_STATEOBJ_H_
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include "substance.h"
#include "timer.h"
#include "hometowndef.h"
#include "hometownactionpolicy.h"
#include "log.h"

namespace GNET
{
	class HometownStateObj;
	class State : public Substance
	{
	public:
		int _start_time;			//状态加上的时间
		int _expire_time;			//该状态终止时间
		int _save_policy_time;		//上次计算状态变迁概率的时间
		bool _deleted;
	public:
		DECLARE_SUBSTANCE_ABSTRACT(State)
		State(int start_time=0, int expire_time=0):_start_time(start_time),_expire_time(expire_time),_save_policy_time(expire_time),_deleted(false){}
		OctetsStream& marshal(OctetsStream &os) const
		{
			os << _start_time;
			os << _expire_time;
			os << _save_policy_time;
			os << _deleted;
			return os;
		}
		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> _start_time;
			os >> _expire_time;
			os >> _save_policy_time;
			os >> _deleted;
			return os;
		}
		void SetDelete() { _deleted = true; }
		bool IsDelete() { return _deleted; }
		void SetDuration(int duration)
		{
			_expire_time = _start_time+duration;
			_save_policy_time = _expire_time;
		}
		void Destroy() { delete this; }
		virtual int GetStateID() = 0;
		virtual bool TryTransit(HometownStateObj *man, int action, const void *action_param, State *&next) = 0;
		virtual void OnLeave(State *next, int action, const void *action_param, HometownStateObj *man) = 0;
		virtual void OnEnter(State *prev, int action, const void *action_param, HometownStateObj *man) = 0;
		virtual void OnHeartbeat(HometownStateObj *man) = 0;
	};

	class StatePolicy
	{
	public:
		virtual bool operator ()(HometownStateObj *man, State *cur, int action, const void *action_param, State *&next) = 0;
	};

	template <typename STATE_MAN, typename CUR_STATE, typename NEXT_STATE>
	class ParamPolicy
	{
	public:
		bool operator ()(HometownStateObj *man, State *cur, int action, const void *action_param, State *&next) 
		{
			return (*this)((STATE_MAN *)man, (CUR_STATE *)cur, action, action_param, (NEXT_STATE *&)next);
		}
	protected:
		virtual bool operator ()(STATE_MAN *man, CUR_STATE *cur, int action, const void *action_param, NEXT_STATE *&next) = 0;
	};

	template <typename STATE_MAN>
	class ParamState : public State
	{
		typedef std::vector<StatePolicy *> PolicyVec;
		PolicyVec _policies;
	public:
		ParamState(int start_time, int duration) : State(start_time, start_time+duration){}
		void AddPolicy(StatePolicy *s)
		{
			_policies.push_back(s);
		}
		bool TryTransit(HometownStateObj *man, int action, const void *action_param, State *&next)
		{
			PolicyVec::iterator it, ie = _policies.end();
			for (it = _policies.begin(); it != ie; ++it)
			{
				StatePolicy &policy = *(*it);
				if (policy(man, this, action, action_param, next))		//判断到达变迁条件
					return true;
			}
			return false;
		}
		void OnLeave(State *next, int action, const void *action_param, HometownStateObj *man)
		{
			OnLeave(next, action, action_param, (STATE_MAN *)man);
		}
		void OnEnter(State *prev, int action, const void *action_param, HometownStateObj *man)
		{
			OnEnter(prev, action, action_param, (STATE_MAN *)man);
		}
		void OnHeartbeat(HometownStateObj *man)
		{
			OnHeartbeat((STATE_MAN *)man);
		}
	protected:
		virtual void OnLeave(State *next, int action, const void *action_param, STATE_MAN *man) {}
		virtual void OnEnter(State *prev, int action, const void *action_param, STATE_MAN *man)
		{
			LOG_TRACE("StateChange %s[%d-%d-%d-%s]--->%s[%d-%d-%d-%s]\n", prev->GetRunTimeClsName(), prev->_start_time, prev->_expire_time, prev->_save_policy_time, prev->_deleted?"deleted":"active", GetRunTimeClsName(), _start_time, _expire_time, _save_policy_time, _deleted?"deleted":"active");
		}
		virtual void OnHeartbeat(STATE_MAN *man) {}
	};

	class HometownStateObj
	{
		class TriggerTransit
		{
			int _action;
			const void *_action_param;
			HometownStateObj *_man;
			bool &_state_changed;
		public:
			TriggerTransit(HometownStateObj *man, int action, const void *action_param, bool &state_changed):_action(action), _action_param(action_param), _man(man), _state_changed(state_changed){}
			void operator() (State *s)
			{
				if (s->IsDelete()) return;

				State *prev = s;
				State *next = NULL;
				while (prev->TryTransit(_man, _action, _action_param, next))
				{
					_state_changed = true;
					if (next == NULL)
					{
						prev->OnLeave(next, _action, _action_param, _man);
						_man->MarkDeleteState(prev);
						break;
					}
					prev->OnLeave(next, _action, _action_param, _man);
					_man->__MarkChangeState(prev, next);
					next->OnEnter(prev, _action, _action_param, _man);
					prev = next;
				}
			}
		};

		class TriggerHeartbeat
		{
			HometownStateObj *_man;
		public:
			TriggerHeartbeat(HometownStateObj *man):_man(man) {}
			void operator() (State *s)
			{
				if (s->IsDelete()) return;
				s->OnHeartbeat(_man);
			}
		};

		friend class TriggerTransit;
		friend class TriggerHeartbeat;

	protected:
		class StateCmp
		{
		public:
			bool operator ()(State *const &lhs, State *const &rhs) const
			{
				return lhs->GetStateID()<rhs->GetStateID();
			}
		};
		typedef std::set<State *, StateCmp> STATE_SET;
		STATE_SET _state_set;
		STATE_SET _insert_set;

		//操作状态表的函数分为Mark和Raw两大类，两大类之间不能互相调用
		//Mark类用于CheckTransit引起的函数调用如OnEnter,OnLeave,OnHeartbeat
		//Raw类用于不是CheckTransit引起的函数调用，如道具功能
		void __RawAddState(State *s) 
		{
			if (!_state_set.insert(s).second)
			{
				Log::log(LOG_ERR, "HometownStateObj, __RawAddState fail to add state %d\n", s->GetStateID());
				Dump();
				delete s;
			}
		}

		State *__RawGetState(int state_id)
		{
			STATE_SET::iterator it = _state_set.begin(), ie = _state_set.end();
			for (; it!=ie && (*it)->GetStateID()<state_id; ++it);
			if (it!=ie && (*it)->GetStateID()==state_id && !(*it)->IsDelete())
				return *it;

			it = _insert_set.begin(), ie = _insert_set.end();
			for (; it!=ie && (*it)->GetStateID()<state_id; ++it);
			if (it!=ie && (*it)->GetStateID()==state_id && !(*it)->IsDelete())
				return *it;
			return NULL;
		}

		void __RawClearStates()
		{
			std::for_each(_state_set.begin(), _state_set.end(), std::mem_fun<void, State>(&State::Destroy));
			_state_set.clear();
			std::for_each(_insert_set.begin(), _insert_set.end(), std::mem_fun<void, State>(&State::Destroy));
			_insert_set.clear();
		}

		void __MarkAddState(State *s)
		{
			if (!_insert_set.insert(s).second)
			{
				Log::log(LOG_ERR, "HometownStateObj, __MarkAddState fail to add state %d\n", s->GetStateID());
				Dump();
				delete s;
			}
		}

		void __MarkChangeState(State *sold, State *snew)
		{
			sold->SetDelete();
			__MarkAddState(snew);
		}
		void __MarkClearStates()
		{
			std::for_each(_insert_set.begin(), _insert_set.end(), std::mem_fun<void, State>(&State::SetDelete));
		}

		void CleanupStates()
		{
			STATE_SET::iterator it = _state_set.begin(), ie = _state_set.end();
			while (it != ie)
			{
				if ((*it)->IsDelete())
				{
					delete *it;
					_state_set.erase(it++);
				}
				else
					++it;
			}

			ie = _insert_set.end();
			for (it = _insert_set.begin(); it != ie; ++it)
			{
				if ((*it)->IsDelete())
					delete *it;
				else
					__RawAddState(*it);
			}
			_insert_set.clear();

		}

		void CheckTransit(int action, const void *action_param, bool &state_changed)
		{
			assert(_insert_set.empty());
			state_changed = false;
			std::for_each(_state_set.begin(), _state_set.end(), TriggerTransit(this, action, action_param, state_changed));
			CleanupStates();
			std::for_each(_state_set.begin(), _state_set.end(), TriggerHeartbeat(this));
			CleanupStates();
		}

	public:
		virtual ~HometownStateObj()
		{
			__RawClearStates();
		}

		bool Init(const STATE_SET &initstate)
		{
			__RawClearStates();
			STATE_SET::const_iterator it, ie = initstate.end();
			for (it = initstate.begin(); it != ie; ++it)
				__RawAddState(*it);
			return true;
		}

		void RawAddState(State *s)
		{
			__RawAddState(s);
		}
		void MarkAddState(State *s)
		{
			__MarkAddState(s);
		}
		void MarkDeleteState(State *s)
		{
			s->SetDelete();
		}

		void MarkClearOtherStates(State *left)
		{
			STATE_SET::iterator it, ie = _state_set.end();
			for (it = _state_set.begin(); it != ie; ++it)
			{
				if (*it != left) (*it)->SetDelete();
			}

			ie = _insert_set.end();
			for (it = _insert_set.begin(); it != ie; ++it)
			{
				if (*it != left) (*it)->SetDelete();
			}
		}

		void RawClearStates()
		{
			__RawClearStates();
		}

		void AddStateExpireTime(int state_id, int time)
		{
			State *s = __RawGetState(state_id);
			if (s != NULL) s->_expire_time += time;
		}

		//m: state_id--->start_time
		void GetCurrentStates(std::map<int, int> &m) const
		{
			STATE_SET::const_iterator it, ie = _state_set.end();
			for (it = _state_set.begin(); it != ie; ++it)
			{
				State &s = *(*it);
				if (!s.IsDelete())
					m[s.GetStateID()] = s._start_time;
			}
			ie = _insert_set.end();
			for (it = _insert_set.begin(); it != ie; ++it)
			{
				State &s = *(*it);
				if (!s.IsDelete())
					m[s.GetStateID()] = s._start_time;
			}
		}
		
		void GetCurrentStates(std::set<int> &s) const
		{
			STATE_SET::const_iterator it, ie = _state_set.end();
			for (it = _state_set.begin(); it != ie; ++it)
			{
				State &st = *(*it);
				if (!st.IsDelete())
					s.insert(st.GetStateID());
			}

			ie = _insert_set.end();
			for (it = _insert_set.begin(); it != ie; ++it)
			{
				State &st = *(*it);
				if (!st.IsDelete())
					s.insert(st.GetStateID());
			}
		}

		bool IsInState(int state_id) const
		{
			std::set<int> m;
			GetCurrentStates(m);
			return m.find(state_id)!=m.end();
		}

		bool IsInAllStates(const std::set<int> &ss) const
		{
			std::set<int> m;
			GetCurrentStates(m);
			std::set<int>::const_iterator it, ie = ss.end();
			for (it = ss.begin(); it != ie; ++it)
			{
				if (m.find(*it) == m.end())
					return false;
			}
			return true;
		}
		
		bool IsInAnyState(const std::set<int> &ss) const
		{
			std::set<int> m;
			GetCurrentStates(m);
			std::set<int>::const_iterator it, ie = ss.end();
			for (it = ss.begin(); it != ie; ++it)
			{
				if (m.find(*it) != m.end())
					return true;
			}
			return false;
		}

		Marshal::OctetsStream &SaveStates(Marshal::OctetsStream &os)
		{
			os << _state_set.size();
			STATE_SET::iterator it, ie = _state_set.end();
			for (it = _state_set.begin(); it != ie; ++it)
				(*it)->SaveSubstance(os);
			return os;
		}

		const Marshal::OctetsStream &LoadStates(const Marshal::OctetsStream &os)
		{
			__RawClearStates();
			size_t sz;
			os >> sz;
			while (sz-- > 0)
			{
				State *s = Substance::DynamicCast<State>(Substance::LoadSubstance(os));
				assert(s != NULL);
				__RawAddState(s);
			}
			return os;
		}

		virtual void SetDirty(bool dirty) {}
		virtual bool IsDirty() const { return false;}

		int CanAction(int action, bool must_accept=true) const
		{
			const HometownActionPolicy *act = HometownActionPolicy::GetActionPolicy(action);
			if (act == NULL) return HT_ERR_SUCCESS;
			return act->CanPerform(this, must_accept);
		}

		void PreAction(int action, bool &changed)
		{
			PreAction(action, NULL, changed);
		}

		void PreAction(int action, void *action_param, bool &changed)
		{
			bool changed1 = false, changed2 = false;
			CheckTransit(HTF_ACTION_NONE, NULL, changed1);	//优先时间过期的状态变迁
			if (action != HTF_ACTION_NONE)
				CheckTransit(action, action_param, changed2);
			changed = changed1||changed2;
		}

		void Dump() const
		{
			LOG_TRACE("---state_set---\n");
			for (STATE_SET::iterator it = _state_set.begin(); it != _state_set.end(); ++it)
			{
				LOG_TRACE("%s[%d-%d-%d-%s]\n", (*it)->GetRunTimeClsName(), (*it)->_start_time, (*it)->_expire_time, (*it)->_save_policy_time, (*it)->_deleted?"deleted":"active");
			}
			LOG_TRACE("---insert_set---\n");
			for (STATE_SET::iterator it = _insert_set.begin(); it != _insert_set.end(); ++it)
			{
				LOG_TRACE("%s[%d-%d-%d-%s]\n", (*it)->GetRunTimeClsName(), (*it)->_start_time, (*it)->_expire_time, (*it)->_save_policy_time, (*it)->_deleted?"deleted":"active");
			}
		}
	};

	//能够被道具或动作作用,但不能主动作用别人
	class HometownTargetObj : public HometownStateObj
	{
	protected:
		typedef std::map<int, std::map<int, int> > SPONSOR_COOLDOWN_MAP;		//受到某个玩家某类操作以后，要记录玩家的冷却时间
		SPONSOR_COOLDOWN_MAP _sponsor_cooldown_map;

	public:
		void SetSponsorCoolDown(int roleid, int type, int duration)
		{
			int new_expire = Timer::GetTime()+duration;
			int &old_expire = _sponsor_cooldown_map[type][roleid];
			if (old_expire < new_expire)
				old_expire = new_expire;
		}

		bool IsSponsorInCoolDown(int roleid, int type) const
		{
			SPONSOR_COOLDOWN_MAP::const_iterator it = _sponsor_cooldown_map.find(type);
			if (it == _sponsor_cooldown_map.end()) return false;
			std::map<int, int>::const_iterator mit = it->second.find(roleid);
			if (mit == it->second.end()) return false;
			if (mit->second <= Timer::GetTime()) return false;
			return true;
		}

		void ClearCoolDown()
		{
			_sponsor_cooldown_map.clear();
		}

		void CleanupCoolDown()
		{
			int now = Timer::GetTime();
			SPONSOR_COOLDOWN_MAP::iterator it = _sponsor_cooldown_map.begin();
			SPONSOR_COOLDOWN_MAP::iterator ie = _sponsor_cooldown_map.end();

			while (it != ie)
			{
				std::map<int, int>::iterator mit = it->second.begin();
				std::map<int, int>::iterator mie = it->second.end();
				while (mit != mie)
				{
					if (mit->second <= now)
						it->second.erase(mit++);
					else
						++mit;
				}
				if (it->second.empty())
					_sponsor_cooldown_map.erase(it++);
				else
					++it;
			}
		}

		Marshal::OctetsStream &SaveCoolDown(Marshal::OctetsStream &os) const
		{
			os << _sponsor_cooldown_map;
			return os;
		}

		const Marshal::OctetsStream &LoadCoolDown(const Marshal::OctetsStream &os)
		{
			_sponsor_cooldown_map.clear();
			os >> _sponsor_cooldown_map;
			return os;
		}

	};

	//能够使用道具或行动作用,也能被作用
	class HometownPlayerObj : public HometownTargetObj
	{
	protected:
		typedef std::map<int, int> SELF_COOLDOWN_MAP;				//按照功能类别来冷却, key: class, value: deadline,比如道具使用
		SELF_COOLDOWN_MAP _self_cooldown_map;

	public:
		virtual int GetID() const = 0;
		void SetSelfCoolDown(int type, int duration)
		{
			int new_expire = Timer::GetTime()+duration;
			int &old_expire = _self_cooldown_map[type];
			if (old_expire < new_expire)
				old_expire = new_expire;
		}

		bool IsSelfInCoolDown(int type) const
		{
			SELF_COOLDOWN_MAP::const_iterator it = _self_cooldown_map.find(type);
			if (it == _self_cooldown_map.end()) return false;
			if (it->second <= Timer::GetTime()) return false;
			return true;
		}

		void ClearCoolDown()
		{
			HometownTargetObj::ClearCoolDown();
			_self_cooldown_map.clear();
		}

		void CleanupCoolDown()
		{
			HometownTargetObj::CleanupCoolDown();

			int now = Timer::GetTime();
			SELF_COOLDOWN_MAP::iterator it = _self_cooldown_map.begin();
			SELF_COOLDOWN_MAP::iterator ie = _self_cooldown_map.end();
			while (it != ie)
			{
				if (it->second <= now)
					_self_cooldown_map.erase(it++);
				else
					++it;
			}
		}

		Marshal::OctetsStream &SaveCoolDown(Marshal::OctetsStream &os) const
		{
			HometownTargetObj::SaveCoolDown(os);
			os << _self_cooldown_map;
			return os;
		}

		const Marshal::OctetsStream &LoadCoolDown(const Marshal::OctetsStream &os)
		{
			HometownTargetObj::LoadCoolDown(os);
			_self_cooldown_map.clear();
			os >> _self_cooldown_map;
			return os;
		}
	};
};
#endif
