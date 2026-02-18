#ifndef __GNET_ACFORBIDRULE_H
#define __GNET_ACFORBIDRULE_H

#include "common/platform.h"

namespace GNET
{
	namespace FORBIDRULE
	{
		int GetConsumptionRank(int64_t consumption);
		int GetScoreRank(int score);
		int GetForbidTime(int score, int64_t consumption);
	};
};

#endif
