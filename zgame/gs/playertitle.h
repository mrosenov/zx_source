#ifndef __ONLINEGAME_GS_PLAYER_TITLE_H__
#define __ONLINEGAME_GS_PLAYER_TITLE_H__

enum 
{
	PLAYER_DEFAULT_TITLE_REPUTATION = 1,
};
class gplayer_imp;
class playertitle
{
	int _titleid;
public:
	playertitle():_titleid(0)
	{}

	inline int GetTitle()
	{
		return _titleid;
	}

public:
	void SetTitle(gplayer_imp * imp, int title);

};
#endif

