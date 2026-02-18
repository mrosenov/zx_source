#ifndef _COMMON_DATA_AUTO_H_
#define _COMMON_DATA_AUTO_H_

#include <vector>
#include <cassert>
#include <time.h>
#include <itimer.h>

namespace commondataauto
{
	class policy;
	class condition
	{
	public:
		std::vector<int> minute;
		std::vector<int> hour;
		std::vector<int> dayofmonth;
		std::vector<int> month;
		std::vector<int> dayofweek;

		condition(const std::vector<int>& min, const std::vector<int>& hr,
			  const std::vector<int>& mday, const std::vector<int>& mon,
			  const std::vector<int>& wday) : minute(min),hour(hr),dayofmonth(mday),
			   month(mon), dayofweek(wday) {}
		condition(const condition& rhs): minute(rhs.minute),hour(rhs.hour),
			dayofmonth(rhs.dayofmonth), dayofweek(rhs.dayofweek) { }
                bool Check(policy * self, int param = 0);
	};

	class target
	{
	public:
		std::vector<int> data_type;
		std::vector<int> key;
		target (const std::vector<int>& dtype, const std::vector<int>& k):
			data_type(dtype), key(k) {}
		target(const target& rhs):data_type(rhs.data_type),key(rhs.key) {}
		~target () {}
	};

	class expr
	{
	  public:
		expr (){}
                virtual ~expr () {}
                virtual int GetValue() =0;
	};

	class operation
	{
		operation(const operation & rhs);
	protected:
		target *        _target;
		operation *     _next;
		expr * _expr;
	public:
		enum {
			OP_TYPE_PLUS,
			OP_TYPE_MINUS,
			OP_TYPE_SET,
			OP_TYPE_REMOVE,
			OP_TYPE_NUM,
		};
		int _op_type;	//操作模式

		operation():_target(0),_next(0),_expr(0){}
		virtual ~operation()
                {
                        if(_target) delete _target;
			delete _expr;
                }
		void SetTarget(target * __tar)
                {
                        _target = __tar;
                }
		void SetOpType (int op)
		{
			_op_type = op;
		}
		void SetExpr (expr *expr)
		{
			_expr = expr;
		}
                void SetNext(operation * next)
                {
                        assert(next == 0 || _next == 0);
                        _next = next;
                }
                operation * GetNext()
                {
                        return _next;
                }
		target * GetTarget() {return _target;}
		expr * GetExpr() {return _expr;}

                bool DoSomething(policy * self);
	};

	class Rule
	{
		Rule(const Rule &rhs);
	protected:
                int             _id;
                condition *     _cond;
                operation *     _ops;
                bool            _enable;
		int		_type;
	public:
		enum {RULE_TYPE_BOOT, RULE_TYPE_TIMER};

		Rule(): _id(-1), _cond(0), _ops(0),_enable(false),_type(RULE_TYPE_BOOT){}
		~Rule ()
		{
			delete _cond;
			operation * tmp = _ops;
                        while(tmp)
                        {
                                operation *tmp2 = tmp->GetNext();
                                delete tmp;
                                tmp = tmp2;
                        }
		}
		void SetRuleType (int rtype) { _type = rtype;}
		int GetRuleType () {return _type;}

		inline void SetData(int id , condition * cond)
                {
                        _id = id;
                        _cond = cond;
                }
                inline void AddOp(operation * op)
                {
                        op->SetNext(0);
                        if(_ops)
                        {
                                operation * tmp = _ops;
                                while(tmp->GetNext())
                                {
                                        tmp = tmp->GetNext();
                                }
                                tmp->SetNext(op);

                        }
                        else
                        {
                                _ops = op;
                        }
                }
		operation * GetOperation() {return _ops;}
		inline bool IsEnable()
                {       
                        return _enable;
                }
		inline void EnableRule(bool enable)
                {       
                        _enable = enable;
		}
		inline bool TestRule(policy * self, int param = 0)
		{
			bool bRst = true;
			bRst = _cond->Check(self, param);
			if (bRst)
			{
				operation * tmp = _ops;
				while(tmp)
				{
					if(!tmp->DoSomething(self))
					{
						bRst = false;
						break;
					}
					tmp = tmp->GetNext();
				}
				return bRst;
			}
			return true;
		}
		void Init (std::vector<std::vector<int> >& info);
	};

	class policy
	{
		typedef std::vector<Rule*> RULER_LIST;
		RULER_LIST _rule_list;
		RULER_LIST _time_list;
		RULER_LIST _boot_list;

		int _seq;
	public:
		struct tm _t;

		policy ():_seq(-1) { }
		~policy ()
		{
			for (size_t i=0; i< _rule_list.size(); i++)
			{
				delete (_rule_list[i]);
			}
		}
		void AddRule(Rule * pRule)
		{
			if (pRule->GetRuleType() == Rule::RULE_TYPE_BOOT)
			{
			    _boot_list.push_back (pRule);
			}
			else if (pRule->GetRuleType() == Rule::RULE_TYPE_TIMER)
			{
			    _time_list.push_back (pRule);
			}
			_rule_list.push_back (pRule);
		}


		void Heartbeat ()
		{
			time_t now = time(0);
			localtime_r(&now, &_t);
			if (_seq == _t.tm_min)
			{
				return;
			}
			_seq = _t.tm_min;
			CheckTimerRules();
		}
		void CheckBootRules ()
		{
			time_t now = time(0);
			localtime_r(&now, &_t);

			RULER_LIST& list = _boot_list;
			for(size_t i = 0; i < list.size(); i++)
                        {
				if (!list[i]->IsEnable())
					continue;
                                if(!list[i]->TestRule(this))
                                {
                                        return ;
                                }
                        }
		}
		void CheckTimerRules()
		{
			RULER_LIST& list = _time_list;
			for(size_t i = 0; i < list.size(); i++)
                        {
				if (!list[i]->IsEnable())
					continue;
                                if(!list[i]->TestRule(this))
                                {
                                        return ;
                                }
                        }
		}
		void ValidateDataType();
		int load (const char *file);
	};

	enum {
		VAL_TYPE_CONSTANT,
		VAL_TYPE_COMMONDATA,
	};

	class expr_constant: public expr
	{
		int _constant;
	public:
		expr_constant (const int &v): _constant (v) {}
		~expr_constant () {}
		int GetValue () {return _constant;}
	};

	class expr_common_data : public expr 
	{
		int _data_type;
		int _key;
	public:
		expr_common_data(const int& data_type, const int & key):
			_data_type(data_type), _key(key) {}

		~expr_common_data() {}
		int GetValue ();
		int GetDataType() {return _data_type;}
	};
}

class CommonDataAuto : public GNET::IntervalTimer::Observer
{
	commondataauto::policy _policy;
	static CommonDataAuto _ins;
public:
	static CommonDataAuto* Instance() {return &_ins;}

	int LoadPolicy(const char *file) {return _policy.load(file);}
	//当从数据库里收到公用数据时，才可以让规则起作用
	void Start() 
	{
	    _policy.ValidateDataType();
	    _policy.CheckBootRules();
	    GNET::IntervalTimer::Attach(this,500000/GNET::IntervalTimer::Resolution());
	}
        bool Update() 
	{
	    _policy.Heartbeat();
	    return true;
	}
};
#endif
