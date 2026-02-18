#ifndef _HIT_CONDITION_H_
#define _HIT_CONDITION_H_

#include <vector>
#include <map>
#include "filter.h"

#pragma pack(1)

namespace GNET
{
enum {
	TIME_INSTANT 	= 0,
	TIME_FINITE  	= 1,
	TIME_INFINITE	= 2,
};

enum {
	ADD_NORMAL			=	0x00000001,	//正常加上
	ADD_HAS_ANY_FILTER	=	0x00000002,	//具有任一buff
	ADD_HAS_ALL_FILTER	=	0x00000004,	//具有所有buff
	ADD_NOT_ANY_FILTER	=	0x00000008,	//没有任何一个buff
};

enum {
	EFFECT_BEGIN        =   0x00000001, //最初生效
	EFFECT_DELAY        =   0x00000002, //延时生效
	EFFECT_HIT  	    =   0x00000004, //击中生效
	EFFECT_BEHIT        =   0x00000008, //被击中生效 
	EFFECT_BEINJURE     =   0x00000010, //被伤血生效  
	EFFECT_KILL     	=   0x00000020, //杀人生效 
	EFFECT_VALID_KILL   =   0x00000040, //有效杀人生效
	EFFECT_INVALID_KILL =   0x00000080, //无效杀人生效                                                                             
	EFFECT_BELOW_HP     =   0x00000100, //自身血量低于百分比时 生效                                                                
	EFFECT_ABOVE_HP     =   0x00000200, //自身血量高于百分比时 生效                                                                
	EFFECT_BELOW_MP     =   0x00000400, //自身魔力低于百分比时 生效                                                                
	EFFECT_ABOVE_MP     =   0x00000800, //自身魔力高于百分比时 生效                                                                
	EFFECT_BEFORE_DEATH =   0x00001000, //遭受 致死攻击时 生效                                                                     
	EFFECT_INJURE       =   0x00002000, //对方伤血生效                                                                             
	EFFECT_STATE		=	0x00008000,	//玩家身上有某状态
	EFFECT_POST_PERFORM =	0x00010000,
};       

#define EFFECT_THRESHOLD (EFFECT_BELOW_HP | EFFECT_ABOVE_HP | EFFECT_BELOW_MP | EFFECT_ABOVE_MP)

enum {                                                                                                                             
    RULE_WEAK  			=   1, //无论如何都失败                                                                                            
    RULE_UNIQUE			=   2, //无论如何都成功覆盖                                                                                        
    RULE_ISOLATE    	=   3, //独立                                                                                                  
    RULE_ATTACH 		=   4, //叠加                                                                                                      
    RULE_TIMERESET  	=   5, //重置时间                                                                                              
    RULE_TIMEPRIOR  	=   6, //(剩余)持续时间更长，则成功覆盖                                                                          
    RULE_SUM_TIMEPRIOR	= 	7, //总持续时间更长，则成功覆盖                                                                    
    RULE_RATIOPRIOR  	=   8, //比例更大，则成功覆盖                                                                    
    RULE_VALUEPRIOR  	=   9, //数值更大，则成功覆盖                                                                    
    RULE_AMOUNTPRIOR  	=   10, //总量更大，则成功覆盖                                                                    
};

class BuffCondition
{
	public:
		int				   _addcondition;
		std::map<int, std::vector<int> > _addparams;

		int                _begincondition; // 生效条件
		std::map<int, int> _beginparam; // 生效条件参数	
		std::map<int, int> _maxcounts;  // 生效最大次数

		std::map<int, int> _endparam;	// 结束条件参数
		int                _endcondition; // 结束条件

		char _polyrule;
		char _polymax;
		int _buffdesc;
		int _buffvisi;

		int _version;
		int FILTER_MASK;
		char _timetype;

	public:
		BuffCondition():_addcondition(0),_begincondition(0),_endcondition(0),_polyrule(0),_polymax(0),_buffdesc(0),_buffvisi(0),_version(0),FILTER_MASK(0),_timetype(0)
		{
		}
};

extern BuffCondition _stub_buffcondition_default;

}

#pragma pack()

#endif //
