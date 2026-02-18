#include "acforbidrule.h"
#include "log.h"
#include "common/mutex.h"
namespace GNET
{
	namespace FORBIDRULE
	{
		int GetConsumptionRank(int64_t consumption)
		{
			if (consumption < 1000000)
				return 0;
			else if (consumption < 10000000)
				return 1;
			else if (consumption < 30000000)
				return 2;
			else if (consumption < 100000000)
				return 3;
			else if (consumption < 200000000)
				return 4;
			else 
				return 5;
		}
		int GetScoreRank(int score)
		{
			if (score <= 2)
				return 0;
			else if (score <= 8)
				return 1;
			else if (score <= 11)
				return 2;
			else 
				return 3;
		}
		int GetForbidTime(int score, int64_t consumption)
		{
			int score_rank = GetScoreRank(score);
			int consumption_rank = GetConsumptionRank(consumption);

			static int init=0;
			static Thread::Mutex   lock;
			static std::map<int, std::map<int, int> > rulemap;
			if (init == 0)
			{
				Thread::Mutex::Scoped l(lock);
				if (init == 0)
				{
					LOG_TRACE("Init ACForbidRule");
					rulemap[0][0] = 10800;
					rulemap[0][1] = 1800;
					rulemap[0][2] = 600;
					rulemap[0][3] = 600;
					rulemap[0][4] = 600;
					rulemap[0][5] = 300;

					rulemap[1][0] = 86400;
					rulemap[1][1] = 10800;
					rulemap[1][2] = 3600;
					rulemap[1][3] = 1800;
					rulemap[1][4] = 600;
					rulemap[1][5] = 300;

					rulemap[2][0] = 604800;
					rulemap[2][1] = 86400;
					rulemap[2][2] = 86400;
					rulemap[2][3] = 10800;
					rulemap[2][4] = 600;
					rulemap[2][5] = 300;

					rulemap[3][0] = 2592000;
					rulemap[3][1] = 2592000;
					rulemap[3][2] = 259200;
					rulemap[3][3] = 10800;
					rulemap[3][4] = 600;
					rulemap[3][5] = 300;

					init = 1;
				}
			}
			LOG_TRACE("GetForbidTime score %d consumption %lld score_rank %d consumption_rank %d time %d",
					score, consumption, score_rank, consumption_rank, rulemap[score_rank][consumption_rank]);
			return rulemap[score_rank][consumption_rank];
		}
	};
};
