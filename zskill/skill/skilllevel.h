#ifndef __SKILL_SKILLLEVEL_H
#define __SKILL_SKILLLEVEL_H

namespace GNET
{

class SkillWrapper;
class SkillLevel
{
	SkillWrapper * skillwrapper;
public:
	SkillLevel(SkillWrapper * s=NULL) : skillwrapper(s) { }

	int GetValue(int index);
	void SetValue(int index, int value);
};

}

#endif

