
#ifndef __GNET_REPAIRROLELIST_HPP
#define __GNET_REPAIRROLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RepairRoleList : public GNET::Protocol
{
	#include "repairrolelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Marshal::OctetsStream key;

		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			StorageEnv::AtomTransaction txn;
			try
			{
				key << roleid;
				GRoleBase	base;
				Marshal::OctetsStream(pbase->find( key, txn )) >> base;
				if (userid == base.userid)
				{
					//userid 相同不需要修
					return;
				}
				User user;
				Marshal::OctetsStream(puser->find(Marshal::OctetsStream() << userid, txn)) >> user;
				if (user.logicuid != (unsigned int)LOGICUID(roleid))
				{
					//logicuid 不相同不需要修
					return;
				}
				RoleList rolelist(user.rolelist);
				rolelist.DelRole(roleid % MAX_ROLE_COUNT);
				user.rolelist = rolelist.GetRoleList();
				puser->insert(Marshal::OctetsStream() << userid, Marshal::OctetsStream() << user, txn);
				Log::formatlog("repair_rolelist", "userid=%d, roleid=%d.\n",userid, roleid);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log(LOG_ERR, "repairerolelist, roleid=%d, what=%s\n", roleid, e.what() );
		}

	}
};

};

#endif
