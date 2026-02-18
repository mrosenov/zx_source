
#ifndef __GNET_ROLELIST_RE_HPP
#define __GNET_ROLELIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "roleinfo"
#include "rolelist.hpp"
#include "conv_charset.h"
#include "makechoice.h"
#include "glinkclient.h"

//#include "groleinfo"

namespace GNET
{

class RoleList_Re : public GNET::Protocol
{
        #include "rolelist_re"
        void Process(Manager *manager, Manager::Session::ID sid)
        {
		GLinkClient * cm=(GLinkClient*) manager;
                Log::trace("glinkd::receive rolelist_re from client");
                
		//if (result==ERR_SUCCESS)
                {
                        if (handle!=_HANDLE_END)
                        {
#if 1
                                cm->Send(sid,RoleList(userid,localsid,handle));
				cm->SetClientState(ECS_PREPARE_SELECTROLE);
                                if (result==ERR_SUCCESS)
                                {
                                        DEBUG_PRINT("Rolelist:\n");
					cm->m_vRoleinfo.push_back(rolelist[0]);
                                        //Octets name_gbk;
                                        //CharsetConverter::conv_charset_u2g(roleinfo.name,name_gbk);
                                	Log::trace("id=%d,name=%.*s",rolelist[0].roleid,rolelist[0].name.size(),rolelist[0].name.begin());
				}
                                else
                                {
                                        DEBUG_PRINT("Rolelist:\n");
                                        printf("\tRole info error.\n");
                                }
#endif 
                       }
                        else
                        {
                                /* let player select role */
                                DEBUG_PRINT("client:: get all role info.\n");
#if 1
                                if(0 == cm->m_vRoleinfo.size())
                                {
					Log::trace("create role ,username=%s",cm->username.c_str());
                                        MakeCreateRole(userid,manager,sid,(char*)cm->username.c_str(),cm->m_occup);
                                }
                                else
                                {                      
					// 选择第一个角色                 
                                        Log::trace("select the first role, roleid=%d,pos(%f,%f,%f)",cm->m_vRoleinfo[0].roleid,cm->m_vRoleinfo[0].posx,cm->m_vRoleinfo[0].posy,cm->m_vRoleinfo[0].posz);
                                        MakeSelectRole(userid,manager,sid,cm->m_vRoleinfo[0].roleid,cm->m_lineid);               
					cm->SetPosition(cm->m_vRoleinfo[0].posx,cm->m_vRoleinfo[0].posy,cm->m_vRoleinfo[0].posz);
                                }
#else
                                MakeChoice(userid,manager,sid);

#endif
                        }
                }
                return;
        }
};

};

#endif
