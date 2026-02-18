//#include "circleusage.h"
//#include "hashstring.h"
#include "localmacro.h"
//#include "thread.h"
#include "gcirclechat.hpp"
class PlayerInfo;
class CircleManager;
class CircleInfo;

#include "base64.h"
namespace GNET
{
void CircleChat(GCircleChat & chat)
{
        if(CircleManager::Instance()->IsCircleOpen()==false)	
		return;
	if(chat.channel == GP_CHAT_CIRCLE)
	{
	//	Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(chat.src);
		if (NULL==pinfo || chat.circleid==0 || pinfo->GetCircleID()!=chat.circleid) 
			return;
		chat.emotion = pinfo->emotion;
	
		CircleInfo * pcircle = CircleManager::Instance()->Find(chat.circleid);
		if (NULL == pcircle)
			return;
		chat.channel = GP_CHAT_CIRCLE;
		pcircle->Broadcast(chat);
			
		Octets out;
		Base64Encoder::Convert(out, chat.msg);
		Log::log(LOG_CHAT,"Circle: src=%d circleid=%d msg=%.*s",chat.src,chat.circleid,out.size(),(char*)out.begin());
	}
}

}
