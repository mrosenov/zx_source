
#ifndef __GNET_SELECTROLE_RE_HPP
#define __GNET_SELECTROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "roleinfo"
#include "glinkclient.h"
#include "enterworld.hpp"

#include "makechoice.h"
//#include "auctionchoice.h"
//#include "privilege.hxx"
//#include "getplayerfactioninfo.hpp"

namespace GNET
{
class SelectRole_Re : public GNET::Protocol
{
	#include "selectrole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		GLinkClient * cm=(GLinkClient*) manager;
		Log::trace("client recv selectrole_re msg");
                if (result==ERR_SUCCESS)
                {
                        if (auth.size() > 0)
                        {
                                for (size_t i=0;i<auth.size();i++)
                                        printf("user has privilege %d\n",auth[i]);
                        }
                        //Privilege p;
                        //p.Init(&(*auth.GetVector().begin()),auth.size());

                        cm->ChangeState(sid,&state_GDataExchgClient);
                        cm->Send(sid,EnterWorld(((GLinkClient*)manager)->roleid,_SID_INVALID));
                        //manager->Send(sid,GetPlayerFactionInfo(((GLinkClient*)manager)->roleid,_SID_INVALID));
			cm->SetClientState(ECS_ENTER_WORLD);
                        PollIO::WakeUp();
			
			Thread::HouseKeeper::AddTimerTask(new RandomWalkTask(10,cm),2);
                        Thread::HouseKeeper::AddTimerTask(new KeepAliveTask(15),15);

                }
                else
                {
                        if (result==ERR_ROLEFORBID)
                        {
                                DEBUG_PRINT("client::selectrole_re:角色被GM封禁。不能登陆.\n");
                                MakeChoice(((GLinkClient*)manager)->userid,manager,sid);
                        }
                        else
                                DEBUG_PRINT("client::selectrole_re:不能连接游戏服务器，或者游戏服务器拒绝玩家登陆.\n");
                }
		
	}
};

};

#endif
