#include "commondataauto.h"
#include <algorithm>
#include "commondata.h"

namespace commondataauto
{
	bool condition::Check (policy *self, int param)
	{
		struct tm *t = &self->_t;

		if (dayofweek.size() && !std::binary_search (dayofweek.begin(), dayofweek.end(), t->tm_wday))
		{
			return false;
		}
		if (month.size() && !std::binary_search (month.begin(), month.end(), t->tm_mon))
		{
			return false;
		}
		if (dayofmonth.size() && !std::binary_search (dayofmonth.begin(), dayofmonth.end(), t->tm_mday +1))
		{
			return false;
		}
		if (hour.size() && !std::binary_search (hour.begin(), hour.end(), t->tm_hour))
		{
			return false;
		}
		if (minute.size() && !std::binary_search (minute.begin(), minute.end(), t->tm_min))
		{
			return false;
		}
		return true;
	}
	int expr_common_data::GetValue()
	{
		CommonData* pcd = CommonDataMan::Instance()->Get(_data_type);
		assert (pcd != NULL && "未找到数据空间，可能是规则文件错误");
		Value v;
		if (pcd->GetValue (_key, v))
		{
			DEBUG_PRINT ("CommonDataAuto: can't find data_type=%d, key=%d as control param\n",_data_type,_key);
			return 0;
		}
		return v.ToInteger();
	}
	bool operation::DoSomething(policy *)
	{
		int diff_value = _expr->GetValue();

		if (_op_type == OP_TYPE_REMOVE)
		{
			//删除需要特殊处理, 本身不属于游戏逻辑的需要。
			//而是用于冗余数据的清理
			return true;
		}
		const int op_table[OP_TYPE_NUM] = {OP_MODE_ADD, OP_MODE_SUB, OP_MODE_SET};
		int op_mode = op_table[_op_type];

		//对数据空间里的数据进行修改
		for (size_t i=0; i< _target->data_type.size(); i++)
		{
			if (_target->key.empty())
			{
				//对_target->data_type中的所有数据操作
				CommonData *pcd = CommonDataMan::Instance()->Get(_target->data_type[i]);
				Value _tmp_diff_value (diff_value);
				pcd->OperateAllValue(op_mode, _tmp_diff_value);
				continue;
			}
			SyncTransaction trans (_target->data_type[i]);
			trans.SetUpdateLocal (true);
			for (size_t j=0; j< _target->key.size(); j++)
			{
				trans.ValueOp (_target->key[j], op_mode, Value(diff_value));
			}
		}
		return true;
	}


	//用于从文本文件中读取规则
	enum {
		POS_COND_TYPE,
		POS_COND_MIN,
		POS_COND_HOUR,
		POS_COND_MDAY,
		POS_COND_MON,
		POS_COND_WDAY,

		POS_TARGET_DTYPE,
		POS_TARGET_KEY,

		POS_OP_TYPE,
		POS_OP_VAL_TYPE,
		POS_OP_VAL1,
		POS_OP_VAL2,

		POS_NUM  //12
	};

	const int range_table[POS_NUM][2] = 
	{
		{0, 1}, {0,59}, {0,23}, {1,31},{1,12}, {0,6},
		{-1,-1},{-1,-1},
		{0,3},{0,1},{-1,-1},{-1,-1}
	};
	const char range_string[POS_NUM][16] = 
	{
		"条件类型", "分钟", "小时","日期","月份","星期","","",
		"操作类型","数值类型","","",
	};

	bool IsDataValid (std::vector<std::vector<int> >& data)
	{
		for (int pos=0; pos< POS_NUM; pos++)
		{
			if (range_table[pos][0] <0) continue;

			for (size_t i=0; i < data[pos].size(); i++)
			{
				int val = data[pos][i];
				if (val < range_table[pos][0] || val > range_table[pos][1])
				{
					printf ("非法的%s%d。", range_string[pos], val);
					return false;
				}
			}
		}
		return true;
	}
	void strcpy_special (char *des, const char *src)
	{
		bool copy_space =false;
		while (*src)
		{
			if (*src == '(')
			{
				copy_space = true;
			}
			if (*src == ')')
			{
				copy_space = false;
			}
			if (*src == ' ' && !copy_space)
			{
				src ++;
				continue;
			}
			*des = *src;
			des ++;
			src ++;
		}
		*des =0;
	}

	bool ParserLine (const char *line, char s[][256])
	{
		char _tmp_line[256];
		strcpy_special (_tmp_line, line);

		char condition[256], target[256], operation[256];;
		int ret;
		ret = sscanf(_tmp_line, "(%[^)])" "(%[^)])" "(%[^)])",
				condition, target, operation);
		if (ret !=3)
		{
			printf ("格式语法错误，");
			return false;
		}

		char _unused[256];
		ret = sscanf (condition, "%s%s%s%s%s%s%s", s[0],s[1],s[2],s[3],s[4],s[5], _unused);
		if (ret != 6)// && ret !=1)
		{
			printf ("错误的条件，");
			return false;
		}
		if (strcmp (s[POS_COND_TYPE], "boot")==0)
		{
			strcpy (s[POS_COND_TYPE], "0");
		}
		else if (strcmp (s[POS_COND_TYPE],"timer")==0)
		{
			strcpy (s[POS_COND_TYPE], "1");
		}
		else
		{
			printf ("不能识别的条件类型%s，", s[POS_COND_TYPE]);
			return false;
		}
		ret = sscanf (target, "%s%s", s[POS_TARGET_DTYPE],s[POS_TARGET_KEY]);
		if (ret != 2)
		{
			printf ("操作目标格式错误，");
			return false;
		}
		if (strcmp (s[POS_TARGET_DTYPE],"general") ==0)
		{
			strcpy(s[POS_TARGET_DTYPE], "0");
		}
		else if (strcmp (s[POS_TARGET_DTYPE],"automation") ==0)
		{
			strcpy(s[POS_TARGET_DTYPE], "999");
		}
		else
		{
			printf ("未知的数据空间%s", s[POS_TARGET_DTYPE]);
		}
		ret = sscanf (operation, "%s%s%s%s", s[8],s[9],s[10],s[11]);
		if (ret != 3)// && ret != 4)
		{
			printf ("错误的操作方式，");
			return false;
		} 
		if (strcmp (s[POS_OP_TYPE], "add")==0)
		{
			strcpy(s[POS_OP_TYPE], "0");
		}
		else if (strcmp (s[POS_OP_TYPE],"del")==0)
		{
			strcpy (s[POS_OP_TYPE], "1");
		}
		else if (strcmp (s[POS_OP_TYPE],"set")==0)
		{
			strcpy (s[POS_OP_TYPE], "2");
		}
		else if (strcmp (s[POS_OP_TYPE],"remove")==0)
		{
			strcpy (s[POS_OP_TYPE], "3");
		}
		else
		{
			printf ("不能识别的操作类型%s，", s[POS_OP_TYPE]);
			return false;
		}

		if (strcmp (s[POS_OP_VAL_TYPE], "constant")==0)
		{
			strcpy (s[POS_OP_VAL_TYPE], "0");
		}
		else if (strcmp (s[POS_OP_VAL_TYPE],"automation")==0)
		{
			strcpy (s[POS_OP_VAL_TYPE], "1");
			strcpy (s[POS_OP_VAL2], s[POS_OP_VAL1]);
			strcpy (s[POS_OP_VAL1], "0");
		}
		else
		{
			printf ("不能识别的数值类型%s，", s[POS_OP_VAL_TYPE]);
			return false;
		}
		return true;
	}

	bool ParserRange (char *str, std::vector<int>& range)
	{
		if (str[0]=='*' || str[0] == 0)  return true;

		char *p = str;
		int ch;

		while ((ch =*p++))
		{
			if (!isdigit(ch) && ch != ',' && ch != '-')
			{
				printf ("不能识别字符'%c'。", ch);
				return false;
			}
		}
		char *ptr = NULL;
		bool first = true;

		while ((p = strtok_r (first?str:NULL, ",", &ptr)))
		{
			first = false;
			int start, end;
			if (sscanf (p, "%d-%d", &start, &end) == 2)
			{
				for (int i=start; i<=end; i++)
				{
					range.push_back (i);
				}
				continue;
			}
			range.push_back (atoi(p));
		}
		return true;
	}

	void Rule::Init (std::vector<std::vector<int> >& data)
	{
		//临时代码
		//从vector中读取所需要的数据，并生成条件，目标和操作
		//
		//条件
		_cond = new condition(data[POS_COND_MIN], data[POS_COND_HOUR],
				data[POS_COND_MDAY], data[POS_COND_MON],data[POS_COND_WDAY]);

		if (data[POS_COND_TYPE][0] ==0)
		{
			SetRuleType(Rule::RULE_TYPE_BOOT);
		}
		else if (data[POS_COND_TYPE][0] ==1)
		{
			SetRuleType(Rule::RULE_TYPE_TIMER);
		}

		//目标
		target * tar = new target (data[POS_TARGET_DTYPE], data[POS_TARGET_KEY]);
		//操作
		operation *op = new operation;
		op->_op_type = data[POS_OP_TYPE][0];
		expr *e;
		int val_type = data[POS_OP_VAL_TYPE][0];
		if (val_type ==0)
		{
			e = new expr_constant(data[POS_OP_VAL1][0]);
		}
		else
		{
			e = new expr_common_data(data[POS_OP_VAL1][0], data[POS_OP_VAL2][0]);
		}
		op->SetExpr (e);
		op->SetTarget (tar);

		AddOp (op);
		EnableRule (true);
	}

	int policy::load (const char *file)
	{
		//临时代码
		//从文本文件中读取policy所需要的相关信息。
		//
		//
		if (access (file, R_OK) ==-1)
		{
			return -1;
		}
		std::ifstream ifs(file);
		std::string line;
		int lineno =0;
		while (std::getline(ifs, line))
		{
			lineno ++;
			if (line.empty()) continue;
			const char c = line[0];
			if (c == '#' || c == ';'|| c == '\r' || c=='\n') continue;

			char s[POS_NUM][256];
			memset (s, 0, sizeof(s));
			if (!ParserLine(line.c_str(), s))
			{
				printf ("文件%s第%d行解析错误.\n", file, lineno);
				continue;
			}
			bool has_error = false;
			std::vector<std::vector<int> > range(POS_NUM);
			for (size_t i=0; i<range.size(); i++)
			{
				if (!ParserRange (s[i], range[i]))
				{
					has_error = true;
					continue;
				}
				std::unique (range[i].begin(), range[i].end());
				std::sort (range[i].begin(), range[i].end());
			}
			if (has_error || !IsDataValid (range))
			{
				printf ("文件%s第%d行规则加载失败。\n", file, lineno);
				continue;
			}
			Rule * rule = new Rule;
			rule->Init (range);
			AddRule (rule);	
		}
		return 0;
	}
	void policy::ValidateDataType()
	{
		for (RULER_LIST::iterator it = _rule_list.begin(); it != _rule_list.end(); ++it)
		{
			operation * op = (*it)->GetOperation();
			while (op)
			{
				target * t = op->GetTarget();
				for (size_t i =0; i< t->data_type.size(); i++)
				{
					CommonData *pcd ;
					pcd = CommonDataMan::Instance()->Get(t->data_type[i]);
					if (pcd == NULL)
					{
						assert (op->_op_type == operation::OP_TYPE_REMOVE);
					}
				}
				expr_common_data * e = dynamic_cast<expr_common_data*> (op->GetExpr());
				if (e != NULL)
				{
					CommonData *pcd ;
					pcd = CommonDataMan::Instance()->Get(e->GetDataType());
					if (pcd == NULL)
					{
						CommonDataMan::Instance()->Create(e->GetDataType());
						printf ("CommonDataAuto: dataspace %d not found, created.\n",
							e->GetDataType());
					}
				}
				op = op->GetNext();
			}
		}
	}
}
CommonDataAuto CommonDataAuto::_ins;
