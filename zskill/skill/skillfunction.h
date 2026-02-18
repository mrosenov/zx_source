#ifndef _SKILL_FUNCTIONS_
#define _SKILL_FUNCTIONS_

#include <stdlib.h>

namespace GNET
{

	//===========================================haokaixin add start
	unsigned short* GetExcludeMonsterTids();

	bool IsForbidSkill(int skillid);

	int GetOccupClass(int occup);

	bool IsDiffrentOccupClass(int player_occup, int skill_occup);

	bool IsSkillValidClass(int player_occup, int skill_class);

	template<typename T>
	int norm(T v)
	{
		return v > 0;
	}

	int zrand(int n);

	//===========================================haokaixin add end

};


#endif // !_SKILL_FUNCTIONS_
