
#ifndef __GNET_UPDATEENEMY_HPP
#define __GNET_UPDATEENEMY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "genemyinfo"
#include "getenemies_re.hpp"
#include "localmacro.h"

namespace GNET
{

class UpdateEnemy : public GNET::Protocol
{
	#include "updateenemy"

	GEnemyInfoVector::iterator Find(GEnemyInfoVector& list, int enemy)
	{
		GEnemyInfoVector::iterator it=list.begin(),ie=list.end();
		for(;it!=ie;++it)
		{
			if(it->rid==enemy)
				break;
		}
		return it;
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		char ret = 0;
		LOG_TRACE("UpdateEnemy: roleid=%d, enemy=%d, oper=%d", roleid, enemy, oper);
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if( !pinfo || pinfo->friend_ver<0 || pinfo->roleid!=roleid)
			return;
		GEnemyInfoVector& list = pinfo->enemies;
		GEnemyInfoVector::iterator it=list.begin(),ie=list.end();
		if(oper==ENEMY_INSERT)
		{
			GEnemyInfo info;
			it = Find(list, enemy);
			if(it!=ie)
			{
				info = *it;
				list.erase(it);
			}
			else
			{
				PlayerInfo *user = UserContainer::GetInstance().FindRoleOnline(enemy);
				if (!user)
					return;
				info.rid = enemy;
				info.lineid = user->gameid;
				info.status = 0;
				info.name = user->name;
			}
			info.pktime = Timer::GetTime();
			list.push_back(info);
			if(list.size()>60)
			{
				for(it=list.begin(),ie=list.end();it!=ie;++it)
				{
					if(it->status==0)
					{
						list.erase(it);
						break;
					}
				}
			}
			pinfo->friend_ver++;
			GetEnemies_Re res;
			res.enemies.push_back(info);
			res.fulllist = ENEMY_NEW;
			res.localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, res);
			return;
		}
		else if(oper==ENEMY_REMOVE)
		{
			it = Find(list, enemy);
			if(it!=ie)
				list.erase(it);
		}
		else if(oper==ENEMY_FREEZE)
		{
			it = Find(list, enemy);
			if(it==ie)
				return;
			if(it->status)
				it->status = 0;
			else
			{
				int sum = 0;
				for(GEnemyInfoVector::iterator i=list.begin();i!=ie;++i)
					if(i->status) sum++;
				if(sum>=20)
					ret = ERR_FS_NOSPACE;
				else
					it->status = 1;
			}
		}
		else
			return;
		if(!ret)
			pinfo->friend_ver++;
		UpdateEnemy_Re re;
		re.retcode = ret;
		re.enemy = enemy;
		re.oper = oper;
		re.localsid = localsid;
		manager->Send(sid, re);
	}
};

};

#endif
