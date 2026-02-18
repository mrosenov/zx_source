#ifndef _GNET_HOMETOWN_DYNAMIC_H_
#define _GNET_HOMETOWN_DYNAMIC_H_

#include "marshal.h"
#include <vector>
#include "hometownpacket.h"
#include "hometownprotocol.h"
#include <time.h>

namespace GNET
{
	using namespace HOMETOWN;
	class HometownDynamic
	{
		const size_t _max_num;
		std::vector<Octets> _dynamics;

	private:
		//MERGEPOLICY必须返回下面3个值之一
		enum
		{
			RES_CONTINUE = 0,		//继续遍历
			RES_STOP,				//不能合并，停止遍历
			RES_MERGED,				//已经合并，停止遍历
		};

		template <typename PAYLOAD, typename SUBPOLICY>
		class SameDayMergePolicy
		{
			int _time;
			const PAYLOAD *_pld;
			const SUBPOLICY &_sub_policy;
		public:
			SameDayMergePolicy(int time, const PAYLOAD *pld, const SUBPOLICY &sub_policy)
			:_time(time),_pld(pld),_sub_policy(sub_policy) 
			{}

			char operator() (DYNAMIC::dynamic_header *hdr, PAYLOAD *pld) const
			{
				struct tm old, cur;
				localtime_r((const time_t *)&hdr->time, &old);
				localtime_r((const time_t *)&_time, &cur);
				if (old.tm_year!=cur.tm_year || old.tm_yday!=cur.tm_yday)
					return RES_STOP;

				if (_sub_policy(pld, _pld))
					return RES_MERGED;
				else
					return RES_CONTINUE;
			}
		};

	protected:

		//同一天内的动态可能进行合并
		//返回true表示已经合并，false表示未合并
		template <typename PAYLOAD, typename SUBPOLICY>
		bool MergeDynamic(int time, int action, const PAYLOAD &new_dynamic, const SUBPOLICY &sub_policy)
		{
			SameDayMergePolicy<PAYLOAD, SUBPOLICY> policy(time, &new_dynamic, sub_policy);

			//逆向从时间从大到小顺序遍历
			std::vector<Octets>::reverse_iterator it, ie = _dynamics.rend();
			for (it = _dynamics.rbegin(); it != ie; ++it)
			{
				DYNAMIC::dynamic_header *hdr = (DYNAMIC::dynamic_header *)it->begin();
				if (hdr->action == action)
				{
					char res = policy(hdr, (PAYLOAD *)(hdr+1));
					if (res == RES_STOP)
						return false;
					else if (res == RES_MERGED)
						return true;
				}
			}
			return false;
		}

		void Log(const Octets &dynamic)
		{
			_dynamics.push_back(dynamic);
			if (_dynamics.size() > _max_num)
				_dynamics.erase(_dynamics.begin());
		}

		void Log(int time, int action, const void *data, size_t size)
		{
			WRAPPER wrapper;
			wrapper << time;
			wrapper << action;
			wrapper.push_back(data, size);
			Log(wrapper.get_buf());
		}

		template <typename PAYLOAD, typename SUBPOLICY>
		void MergeLog(int time, int action, const PAYLOAD &dynamic, const SUBPOLICY &sub_policy)
		{
			if (!MergeDynamic(time, action, dynamic, sub_policy))
				Log(time, action, &dynamic, sizeof(PAYLOAD));
		}

	public:
		HometownDynamic(size_t max_num=20) : _max_num(max_num) {}

		const std::vector<Octets> &GetDynamics() const
		{
			return _dynamics;
		}

		Marshal::OctetsStream& Save(Marshal::OctetsStream &os)
		{
			os << _dynamics;
			return os;
		}

		const Marshal::OctetsStream& Load(const Marshal::OctetsStream &os)
		{
			_dynamics.clear();
			os >> _dynamics;
			return os;
		}
	};

	class FarmDynamic : public HometownDynamic
	{
		class MergeExchgMoney
		{
		public:
			bool operator() (DYNAMIC::exchg_money *older, const DYNAMIC::exchg_money *newer) const
			{
				if (older->type == newer->type)
				{
					older->local_money += newer->local_money;
					older->foreign_money += newer->foreign_money;
					return true;
				}
				return false;
			}
		};

		class MergeBuyItem
		{
		public:
			bool operator() (DYNAMIC::buy_item *older, const DYNAMIC::buy_item *newer) const
			{
				if (older->item_id == newer->item_id)
				{
					older->count += newer->count;
					older->cost1 += newer->cost1;
					older->cost2 += newer->cost2;
					return true;
				}
				return false;
			}
		};

		class MergeSellItem
		{
		public:
			bool operator() (DYNAMIC::sell_item *older, const DYNAMIC::sell_item *newer) const
			{
				if (older->item_id == newer->item_id)
				{
					older->count += newer->count;
					older->earning += newer->earning;
					return true;
				}
				return false;
			}
		};

		class MergeUseItem
		{
		public:
			bool operator() (DYNAMIC::use_item *older, const DYNAMIC::use_item *newer) const
			{
				if (older->sponsor==newer->sponsor 
					&& older->target==newer->target
					&& older->item_id==newer->item_id)
					return true;
				else
					return false;
			}
		};

		//犁地、浇水、除草、除虫4种合并规则一样
		template <typename T>
		class MergeFieldAction
		{
		public:
			bool operator() (T *older, const T *newer) const
			{
				if (older->sponsor==newer->sponsor
					&& older->target==newer->target
					&& older->emotion==newer->emotion)
					return true;
				else
					return false;
			}
		};

		class MergeSowField
		{
		public:
			bool operator() (DYNAMIC::sow_field *older, const DYNAMIC::sow_field *newer) const
			{
				if (older->sponsor==newer->sponsor
					&& older->target==newer->target
					&& older->seed_id==newer->seed_id)
					return true;
				else
					return  false;
			}
		};

		class MergeHarvestField
		{
		public:
			bool operator() (DYNAMIC::harvest_field *older, const DYNAMIC::harvest_field *newer) const
			{
				if (older->sponsor==newer->sponsor
					&& older->target==newer->target
					&& older->fruit_id==newer->fruit_id)
				{
					older->count += newer->count;
					return true;
				}
				else
					return  false;
			}
		};

		class MergeHarvestLoveField
		{
		public:
			bool operator() (DYNAMIC::harvest_lovefield *older, const DYNAMIC::harvest_lovefield *newer) const
			{
				if (older->sponsor==newer->sponsor
					&& older->target==newer->target
					&& older->fruit_id==newer->fruit_id)
				{
					older->gain_count += newer->gain_count;
					older->drop_count += newer->drop_count;
					return true;
				}
				else
					return  false;
			}
		};

		class MergeStealField
		{
		public:
			bool operator() (DYNAMIC::steal_field *older, const DYNAMIC::steal_field *newer) const
			{
				if (older->sponsor==newer->sponsor
					&& older->target==newer->target
					&& older->fruit_id==newer->fruit_id
					&& older->emotion==newer->emotion)
				{
					older->count += newer->count;
					return true;
				}
				else
					return  false;
			}
		};

	public:

		template <typename T>
		void MergeLog(int time, int action, const T &dynamic)
		{
			Log(time, action, &dynamic, sizeof(T));
		}

	#define MERGELOG(SUBPOLICY) \
	HometownDynamic::MergeLog(time, action, dynamic, SUBPOLICY())

		void MergeLog(int time, int action, const DYNAMIC::exchg_money &dynamic)
		{
			MERGELOG(MergeExchgMoney);
		}

		void MergeLog(int time, int action, const DYNAMIC::buy_item &dynamic)
		{
			MERGELOG(MergeBuyItem);
		}

		void MergeLog(int time, int action, const DYNAMIC::sell_item &dynamic)
		{
			MERGELOG(MergeSellItem);
		}
		
		void MergeLog(int time, int action, const DYNAMIC::use_item &dynamic)
		{
			MERGELOG(MergeUseItem);
		}

		void MergeLog(int time, int action, const DYNAMIC::plow_field &dynamic)
		{
			MERGELOG(MergeFieldAction<DYNAMIC::plow_field>);
		}

		void MergeLog(int time, int action, const DYNAMIC::water_field &dynamic)
		{
			MERGELOG(MergeFieldAction<DYNAMIC::water_field>);
		}

		void MergeLog(int time, int action, const DYNAMIC::pest_field &dynamic)
		{
			MERGELOG(MergeFieldAction<DYNAMIC::pest_field>);
		}

		void MergeLog(int time, int action, const DYNAMIC::weed_field &dynamic)
		{
			MERGELOG(MergeFieldAction<DYNAMIC::weed_field>);
		}

		void MergeLog(int time, int action, const DYNAMIC::sow_field &dynamic)
		{
			MERGELOG(MergeSowField);
		}

		void MergeLog(int time, int action, const DYNAMIC::harvest_field &dynamic)
		{
			MERGELOG(MergeHarvestField);
		}

		void MergeLog(int time, int action, const DYNAMIC::harvest_lovefield &dynamic)
		{
			MERGELOG(MergeHarvestLoveField);
		}

		void MergeLog(int time, int action, const DYNAMIC::steal_field &dynamic)
		{
			MERGELOG(MergeStealField);
		}

	#undef MERGELOG
	};
};

#endif
