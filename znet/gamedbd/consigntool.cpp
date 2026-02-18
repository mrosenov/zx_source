#include "consigntool.h"
namespace GNET
{
int CONSIGN::UpdateState(GConsignDB& detail)
{
	if(detail.info.state != DSTATE_SELL)
		return 0;
	int now=Timer::GetTime();
	if(now > detail.info.sell_endtime)
	{
		detail.info.state = DSTATE_POST;
		return 1;
	}
	return 0;
}

int CONSIGN::CheckStateChange(const GConsignDB& detail,ConsignState target_state)
{
	if(detail.info.state == target_state)
		return CHANGE_REPEAT;
	
	static int init=0;
	static Thread::Mutex   lock;
	static int matrix[10][10];
	
	if(init == 0)
	{
	Thread::Mutex::Scoped l(lock);
	if(init == 0)
	{
	memset(matrix,CHANGE_FORBID,sizeof(matrix));
	//permit from->to
	matrix[DSTATE_POST][DSTATE_SELL]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_PRE_CANCEL_POST]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_PRE_CANCEL_POST]=CHANGE_PERMIT;

//	matrix[DSTATE_PRE_POST][DSTATE_POST]=CHANGE_PERMIT;
	matrix[DSTATE_PRE_POST][DSTATE_SELL]=CHANGE_PERMIT;
	matrix[DSTATE_PRE_POST][DSTATE_POST_FAIL]=CHANGE_PERMIT;
	matrix[DSTATE_PRE_CANCEL_POST][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;

	matrix[DSTATE_SELL][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_POST]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_SOLD]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;

	//tolerate web err
	matrix[DSTATE_POST_GAME_CANCEL][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_POST_GAME_CANCEL][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_GAME_CANCEL][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_WEB_CANCEL][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_FORCE_CANCEL][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_EXPIRE][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;

//  only permit in deliver	
//	matrix[DSTATE_POST_WEB_CANCEL][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
//	matrix[DSTATE_POST_FORCE_CANCEL][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
//	matrix[DSTATE_EXPIRE][DSTATE_EXPIRE]=CHANGE_PERMIT;
//	matrix[DSTATE_SELL][DSTATE_SELL]=CHANGE_PERMIT;
//	matrix[DSTATE_SOLD][DSTATE_SOLD]=CHANGE_PERMIT;

	//forced
	matrix[DSTATE_PRE_CANCEL_POST][DSTATE_POST]=CHANGE_FORCED;
	
	init=1;
	}
	}
//        int ret=matrix[state][target_state];	
//	if((ret != CHANGE_PERMIT && state == target_state)
//		ret = CHANGE_REPEAT;
//	return ret;
	return matrix[detail.info.state][target_state];	
}

};
