
#ifndef __GNET_TRYCHANGEDS_HPP
#define __GNET_TRYCHANGEDS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "changeds_re.hpp"
#include "playerchangeds.hpp"
#include "blockedrole.h"
#include "gproviderserver.hpp"
#include "centraldeliveryserver.hpp"

namespace GNET
{

class TryChangeDS : public GNET::Protocol
{
	#include "trychangeds"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("trychangeds:roleid=%d, flag=%d",roleid, flag);
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL || BlockedRole::GetInstance()->IsRoleBlocked(roleid))
			return;
		int ret = ERR_SUCCESS;
		if (IsTransToCentralDS(flag))//原服->跨服
		{
			if (ret==ERR_SUCCESS && GDeliveryServer::GetInstance()->IsCentralDS())
				ret = -1;
			if (ret==ERR_SUCCESS && !CentralDeliveryClient::GetInstance()->IsConnect())
				ret = ERR_CDS_COMMUNICATION;
			if (ret==ERR_SUCCESS)
			{
				int dst_lineid = pinfo->gameid;
				if(flag == DS_TO_BATTLEMAP)
				{
					CrossBattleManager *crossbattles = CrossBattleManager::GetInstance();
					const FightingTag *ftag = crossbattles->GetFightingTag(roleid);
					if(ftag == NULL)
					{
						LOG_TRACE("roleid=%d no fightingtag",roleid);
						Log::log(LOG_ERR, "roleid=%d no fightingtag", roleid);
						ret = -1;
					}
					else
					{
						// 因为战场只在固定的GS开启，因此需要保证拉到跨服的对应GS上
						dst_lineid = ftag->_gsid;
						crossbattles->EraseFightingTag(roleid);
						LOG_TRACE("roleid=%d change to gsid=%d", roleid, dst_lineid);
						//Log::log(LOG_DEBUG, "roleid=%d change to gsid=%d", roleid, dst_lineid);
					}
					//flag = DS_TO_CENTRALDS;
				}
				else
				{
					ret = CentralDeliveryClient::GetInstance()->SelectLine(dst_lineid);
				}
				if (ret == ERR_SUCCESS)
					pinfo->user->chgds_lineid = dst_lineid;
			}
		}
		else if (IsTransToNormalDS(flag))//跨服->原服
		{
			if (ret==ERR_SUCCESS && !GDeliveryServer::GetInstance()->IsCentralDS())
				ret = -1;
			GRoleInfo * role = NULL;
			if (ret==ERR_SUCCESS && (role=RoleInfoCache::Instance().Get(roleid))==NULL)
				ret = -2;
			if (ret==ERR_SUCCESS && !CentralDeliveryServer::GetInstance()->IsConnect(role->src_zoneid))
				ret = ERR_COMMUNICATION;
			if (ret==ERR_SUCCESS)
			{
				int dst_lineid = (dst_gsid != 0) ? dst_gsid : pinfo->gameid;
				ret = CentralDeliveryServer::GetInstance()->SelectLine(role->src_zoneid, dst_lineid);
				if (ret == ERR_SUCCESS)
					pinfo->user->chgds_lineid = dst_lineid;
			}
		} 
		else
			ret = -1;
		if (ret != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, ChangeDS_Re(ret, pinfo->localsid));
		else
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeDS(roleid, flag));
		LOG_TRACE("trychangeds:roleid=%d, flag=%d retcode=%d",roleid, flag, ret);
	}
};

};

#endif
