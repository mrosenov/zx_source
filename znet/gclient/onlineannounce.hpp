
#ifndef __GNET_ONLINEANNOUNCE_HPP
#define __GNET_ONLINEANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkclient.hpp"

#ifdef _TESTCODE
//#include "selectrole.hpp"
#include "rolelist.hpp"
#endif

namespace GNET
{

class OnlineAnnounce : public GNET::Protocol
{
        #include "onlineannounce"

        void Process(Manager *manager, Manager::Session::ID sid)
        {
                // TODO
                ((GLinkClient*)manager)->userid=userid;
                DEBUG_PRINT("Login successfully.\n\n");
                manager->ChangeState(sid,&state_GSelectRoleClient);
#ifdef _TESTCODE        
                /*int roleid=userid+1;
                ((GLinkClient*)manager)->roleid=roleid;
                ((GLinkClient*)manager)->SendProtocol(SelectRole(roleid));
                DEBUG_PRINT("send select role(roleid=%d) to link server.\n",roleid);
                */
                ((GLinkClient*)manager)->SendProtocol(RoleList(userid,_SID_INVALID,_HANDLE_BEGIN));
                DEBUG_PRINT("send rolelist to link server.\n");
		((GLinkClient*)manager)->SetClientState(ECS_PREPARE_SEND_ROLELIST);
#endif
        }	
};

};

#endif
