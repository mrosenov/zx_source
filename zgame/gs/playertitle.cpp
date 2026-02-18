#include "playertitle.h"
#include "player_imp.h"
#include "item_manager.h"

void
playertitle::SetTitle(gplayer_imp * imp, int title)
{
	bool bUpdate = false;
	if(_titleid) bUpdate = title_manager::DeactiveTitle(imp, _titleid);
	if(title)  bUpdate =  title_manager::ActiveTitle(imp, title) || bUpdate; //注意这里的顺序，小心短路现象
	_titleid = title;
	if(bUpdate) property_policy::UpdatePlayer(imp->GetPlayerClass(),imp);
}

