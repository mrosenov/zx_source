#ifndef __ONLINEGAME_GS_PLAYER_CONSIGN_H__
#define __ONLINEGAME_GS_PLAYER_CONSIGN_H__

#include <amemobj.h>
class gplayer_imp;

/**
 *	ÃÃ¢Â¸Ã¶ÃÃ Â±Â£Â´Ã¦ÃÃÃÃ¦Â¼ÃÂ¼ÃÃÃÃÂ±ÃÃ¨ÃÂªÂ±Â£Â´Ã¦ÂµÃÃÂ»ÃÂ©ÃÂ´ÃÂ¬ÂºÃÂ´Â¦ÃÃ­ÃÃÃÃ
 */
class player_consign  : public abase::ASmallObject
{
	int 	  _time_out;
public:
	explicit player_consign():_time_out(-1) {}

	virtual ~player_consign(){}

	void SetTimeOut(int t)
	{
		_time_out = t;
	}

	//ÃÂ»ÃÃÃÂ¡ÃÃ»ÃÂ´ÃÂ¬Â²ÃÂ»Ã¡ÂµÃ·ÃÃÃÃÃÃ¸ÂºÂ¯ÃÃ½
	bool Heartbeat(gplayer_imp * pImp)
	{	
		return (--_time_out > 0);
	}
};

#endif
