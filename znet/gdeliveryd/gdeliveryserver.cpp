#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "gauthclient.hpp"
#include "gproviderserver.hpp"

#include "onlineannounce.hpp"
#include "kickoutuser.hpp"
#include "statusannounce.hpp"
#include "userlogout.hrp"
#include "playeroffline.hpp"
#include "tradediscard.hpp"
#include "trade.h"
#include <algorithm>
#include <functional>

#include "maperaser.h"
#include "protocolexecutor.h"

#include "mapforbid.h"
#include "maplinkserver.h"
#include "mapuser.h"
#include "timer.h"
#include "announceserverattribute.hpp"
#include "announcechallengealgo.hpp"

namespace GNET
{
using namespace std;

GDeliveryServer GDeliveryServer::instance;
const Protocol::Manager::Session::State* GDeliveryServer::GetInitState() const
{
	return &state_GDeliverServer;
}

void GDeliveryServer::OnAddSession(Session::ID sid)
{
	LinkServer::GetInstance().Insert( sid );
	Send(sid, AnnounceChallengeAlgo(challenge_algo, auth_version));
}

void GDeliveryServer::OnDelSession(Session::ID sid)
{
	LinkServer::GetInstance().Erase( sid );

	//erase all users that belongs to this linkserver. To prevent accounting error
	//这里只做简单的下线处理，即给Au发送下线信息，防止多计费，同时将用户从map里删除，
	//发送playeroffline给game，并把用户放入forbidset中(如果用户正在切换服务器，则不做这个操作)
	//取消交易

	if (sid != iweb_sid)
	{
		int online = UserContainer::GetInstance().DisconnectLinkUsers(sid);
		Log::log(LOG_ERR,"Disconnect from linkserver sid=%d, drop %d users",sid, online);
	}
	else
	{
		iweb_sid = _SID_INVALID;
	}
}

void GDeliveryServer::BroadcastStatus()
{
	static time_t timer = 0;
	time_t now = Timer::GetTime();

	if(now-timer<10)
		return;
	timer = now;
	UserContainer& container = UserContainer::GetInstance();
	unsigned int _load = (unsigned int)((double)(container.Size()*200)/(double)(container.GetFakePlayerLimit()));
	if ( _load>200 ) 
		_load=200;
	serverAttr.SetLoad((unsigned char)_load);
	LinkServer::GetInstance().BroadcastProtocol( AnnounceServerAttribute(serverAttr.GetAttr())  );
	DEBUG_PRINT("gdeliveryserver::statusannounce,online=%d,fakemax=%d,load=%d,attr=%d\n",
		container.Size(), container.GetFakePlayerLimit(),(unsigned char)_load,serverAttr.GetAttr());
}
void GDeliveryServer::OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer)
{
	Log::formatlog("addsession","sid=%d:ip=%s", sid, inet_ntoa(((const struct sockaddr_in*)peer)->sin_addr));
}

};
