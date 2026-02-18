#include"skillfunction.h"

namespace GNET
{


	//===========================================haokaixin add start


	unsigned short exclude_monster_tids[4] = { 0xB1C3 ,0xC477 ,0xC478 ,0xC479 };

	unsigned short* GetExcludeMonsterTids()
	{
		return exclude_monster_tids;
	}

	unsigned int __skillIds[30] = { 3554, 3555, 3556, 3557, 3558, 3628, 3629, 3630, 3631, 3632,
							3559, 3560, 3561, 3562, 3563, 3623, 3624, 3625, 3626, 3627,
							3564, 3565, 3566, 3567, 3568, 3618, 3619, 3620, 3621, 3622 };

	bool IsForbidSkill(int skillid)
	{
		//int skillIds[30];
		//memcpy(&skillIds, &unk_1BD98DC, sizeof(skillIds));
		for (int i = 0; i <= 29; ++i)
		{
			if (skillid == __skillIds[i])
				return 1;
		}
		return 0;
	}


	unsigned int occupations[14][5] = { 1, 2, 3, 13, 14, 4, 5, 6, 16, 17, 7, 8, 9, 19,
							20, 10, 11, 12, 22, 23, 25, 26, 27, 28, 29, 33, 34, 35,
							36, 37, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 51, 52,
							53, 54, 55, 64, 65, 66, 67, 68, 96, 97, 98, 99, 100, 56,
							57, 58, 59, 60, 102, 103, 104, 105, 106, 108, 109, 110, 111, 112 };

	int GetOccupClass(int occup)
	{
		for (int i = 0; i <= 13; ++i)
		{
			for (int j = 0; j <= 4; ++j)
			{
				if (occupations[i][j] == occup)
					return i;
			}
		}
		return -1;
	}

	bool IsDiffrentOccupClass(int player_occup, int skill_occup)
	{
		int player_class; // [esp+10h] [ebp-8h]
		int skill_class; // [esp+14h] [ebp-4h]

		skill_class = GetOccupClass(skill_occup);
		if (skill_class == -1)
			return 0;
		player_class = GetOccupClass(player_occup);
		return player_class != -1 && player_class != skill_class;
	}

	bool IsSkillValidClass(int player_occup, int skill_class)
	{
		if (skill_class == -2)
			return 1;
		unsigned int player_class = GNET::GetOccupClass(player_occup);
		if (skill_class == -1 && (!player_occup || player_class < 5 || player_class == 9))
			return 1;
		if (player_class != -1 && player_class == skill_class)
			return 1;
		return 0;
	}

	int zrand(int n)
	{
		if (n <= 0)
			return 0;
		else
			return rand() % n;
	}


};