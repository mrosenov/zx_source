#ifndef __GNET_CIRCLEPOOL_H
#define __GNET_CIRCLEPOOL_H

#include <map>

#include "thread.h"
#include "gcircledb"
#include "circlepoolstatenode"
namespace GNET
{
typedef std::vector<CirclePoolStateNode> CirclePoolState;
class CirclePool
{
public:
	typedef std::map<unsigned char,CirclePoolStateNode>  CirclePoolMap2;

	static const int CIRCLEID_FOR_STATE_STORAGE = 0;
	
	CirclePool(){InitCurrentState();}
	
	~CirclePool() 
	{ 
	}
        static CirclePool* Instance()
        {
                static CirclePool instance;
		return &instance;
	}
	unsigned int GetOpenCircleid(unsigned char grade,unsigned char occu)
	{
		unsigned char racegrade = ShiftGrade(grade,occu);
		CirclePoolMap2::iterator im = circlepoolmap.find(racegrade);
		if(im!=circlepoolmap.end() && im->second.cur_id!=0)
			return im->second.cur_id;
		return NewOpenCircle(grade,occu);
	}
	unsigned int NewOpenCircle(unsigned char grade,unsigned char occu)
	{
		unsigned char racegrade = ShiftGrade(grade,occu);
             	unsigned int circleid = GetNewCircleid(racegrade);
	     	circlepoolmap[racegrade].cur_id=circleid;
		circlepoolmap[racegrade].cur_name++;
		LOG_TRACE("NewOpenCircle, racegrade=%d,circleid=%d,cname=%d",racegrade,circleid,circlepoolmap[racegrade].cur_name);
		return circleid;	       
	}
	unsigned int GetCircleCount(unsigned char grade,unsigned char occu)
	{
		unsigned char racegrade = ShiftGrade(grade,occu);
		return circlepoolmap[racegrade].cur_name;
	}
	void GetCurrentState(CirclePoolState& state);
	bool CheckOpenCircle(unsigned char grade,unsigned char occu, unsigned int size)
	{
		//no need to shiftgrade
		return (size < MemberLimit(grade));
	}
private:
	unsigned char ShiftGrade(unsigned char grade,unsigned char occu)
	{
		if(grade == MINGRADE && isDeity(occu))
			grade+=MAXGRADE;
		return grade;
	}
	bool isDeity(unsigned char occu){return occu>32;}	
	void Poolinit();
	void InitCurrentState();
	unsigned int GetNewCircleid(unsigned char grade){return current_circleid++;}
	unsigned int MemberLimit(unsigned char grade);
	static const int MINGRADE = 1;
	static const int MAXGRADE = 10;
	
	static const int GRADE_FOR_CIRCLEID_STORAGE = MINGRADE + MAXGRADE +1;
	CirclePoolMap2 circlepoolmap;
	unsigned int current_circleid;
};

};
#endif
