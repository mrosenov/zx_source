
#ifndef __GNET_CREATEROLE_HPP
#define __GNET_CREATEROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "createrole_re.hpp"
#include "gdeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "uniquenameclient.hpp"

#include "dbcreaterole.hrp"
#include "precreaterole.hrp"
#include "matcher.h"
#include "mapuser.h"
#include "refspreadcode.h"

namespace GNET
{
	class CreateRole : public GNET::Protocol
	{
#include "createrole"

		int DecodeReferid(Octets &refer_id, int &player_suggest_referrer)
		{
			int userid, district_id, roleid;
			if (RefSpreadCode::Decode(refer_id, userid, district_id, roleid))
			{
				if (district_id == GDeliveryServer::GetInstance()->district_id)
				{
					player_suggest_referrer = roleid;
					return REF_ERR_SUCCESS;
				}
				else
					return REF_ERR_REFERRERNOTINDISTRICT;
			}
			else
			{
				return REF_ERR_INVALIDSPREADCODE;
			}
		}

		void Send2UniqueNameSrv(UserInfo *user, GDeliveryServer* dsm,Manager::Session::ID sid)
		{
			if( !UniqueNameClient::GetInstance()->IsConnect() )
			{
				dsm->Send(sid,CreateRole_Re(ERR_COMMUNICATION,_ROLE_INVALID,localsid));
				return;
			}
			
			//如果用户没有在网页上绑定过上线并且是创建第一个角色才接受用户输入的推广号
			int player_suggest_referrer = 0;
			if (referid.size()>0 && user->au_suggest_referrer==0 && user->rolelist.GetRoleCount()==0)
			{
				Octets temp;
				temp.swap(referid);
				CharsetConverter::conv_charset_u2t(temp, referid);
				int res = DecodeReferid(referid, player_suggest_referrer);
				if (res != REF_ERR_SUCCESS)
				{
					dsm->Send(sid,CreateRole_Re(res,_ROLE_INVALID,localsid));
					return;
				}
			}

			PreCreateRole* rpc=(PreCreateRole*) Rpc::Call( RPC_PRECREATEROLE, PreCreateRoleArg(dsm->zoneid,userid,0,roleinfo.name));
			rpc->save_linksid=sid;
			rpc->save_localsid=localsid;
			rpc->roleinfo=roleinfo;
			rpc->player_suggest_referrer = player_suggest_referrer;		//将推荐号缓存到precreaterole中

			DEBUG_PRINT("createrole::send to Uniqueserver. userid=%d,zoneid=%d,name(sz:%d)\n", userid,dsm->zoneid,roleinfo.name.size());
			if (!UniqueNameClient::GetInstance()->SendProtocol(rpc))
			{
				dsm->Send(sid,CreateRole_Re(ERR_CREATEROLE,_ROLE_INVALID,localsid));
			}
		}
		static bool ValidRolename(const Octets& rolename)
		{
			int maxlen = atoi(Conf::GetInstance()->find(GDeliveryServer::GetInstance()->Identification(), "max_name_len").c_str());
			if (maxlen <= 0) maxlen = 16;
			return rolename.size() >= 2 && rolename.size()<=(size_t)maxlen && Matcher::GetInstance()->Match((char*)rolename.begin(),rolename.size())==0;
		}

		void Process(Manager *manager, Manager::Session::ID sid)
		{
			LOG_TRACE("createrole occupation %d, gender %d", roleinfo.occupation, roleinfo.gender);
			GDeliveryServer* dsm=GDeliveryServer::GetInstance();
			if (dsm->IsCentralDS())
			{
				Log::log(LOG_ERR, "Userid %d try to create role on Central Delivery Server, refuse him!", userid);
				return;
			}
			if(!UniqueNameClient::GetInstance()->IsConnect() || !GameDBClient::GetInstance()->IsConnect())
			{
				dsm->Send(sid, CreateRole_Re(ERR_COMMUNICATION,_ROLE_INVALID,localsid));
				return;
			}
																				//注意！ 同步修改 dbconsignsoldrole.hrp seller_cls 的逻辑
			if((roleinfo.occupation!=0&&roleinfo.occupation!=33&&roleinfo.occupation!=39&&roleinfo.occupation!=45&&roleinfo.occupation!=51&&roleinfo.occupation!=56&&roleinfo.occupation!=96&&roleinfo.occupation!=102&&roleinfo.occupation!=108 ) || roleinfo.gender>1)
				return;
			UserInfo* userinfo = UserContainer::GetInstance().FindUser(userid);
			if (NULL==userinfo) 
				return;
			if(userinfo->rolelist.GetRoleCount()>=8)
			{
				dsm->Send(sid,CreateRole_Re(ERR_NOFREESPACE,_ROLE_INVALID,localsid));
				return;
			}
			/* valid name */
			if (!ValidRolename(roleinfo.name))
			{
				dsm->Send(sid,CreateRole_Re(ERR_INVALIDCHAR,_ROLE_INVALID,localsid));
				return;
			}
			Send2UniqueNameSrv(userinfo, dsm,sid);	
			return;
		}
	};

};

#endif
