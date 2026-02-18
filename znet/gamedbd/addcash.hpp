#ifndef __GNET_ADDCASH_HPP
#define __GNET_ADDCASH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "user"
#include "addcash_re.hpp"
#include "localmacro.h"

namespace GNET
{

class AddCash : public GNET::Protocol
{
#include "addcash"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		AddCash_Re re(0, userid, zoneid, sn);
		try
		{
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			StorageEnv::CommonTransaction txn;
			Marshal::OctetsStream key;
			User user;
			try
			{
				re.zoneid = zoneid;
				re.sn = sn;

				key << (unsigned int)userid;
				Marshal::OctetsStream(puser->find(key,txn))>>user;
				if(user.add_serial<sn)
				{
					if((int)(user.cash_add+cash)<0)
					{
						Log::log( LOG_ERR, "AddCash, userid=%d, int overflow cash=%d delta=%d",
								userid, user.cash_add, cash);
						throw DbException(DB_VERIFY_BAD);
					}
					int total = user.cash_add + user.cash_buy - user.cash_used - user.cash_sell;
					if(cash<0 && total+cash<0)
						re.retcode = CASH_NOT_ENOUGH;
					else
					{
						user.add_serial = sn;
						user.cash_add += cash;
						user.data_group[USER_DATA_CASHADD2] += cash;
						if (cash < 0 && user.data_group[USER_DATA_CASHADD2] < 0)
							user.data_group[USER_DATA_CASHADD2] = 0;
						Log::formatlog("addcash","userid=%d:oldserial=%d:newserial=%d:cash_add=%d:"
								"delta=%d", userid,user.add_serial,sn,user.cash_add, cash);
						puser->insert( key, Marshal::OctetsStream()<<user, txn );
						re.retcode = ERR_SUCCESS;
					}
				}else
				{
					Log::formatlog("addok","userid=%d:oldserial=%d:newserial=%d:cash_add=%d:delta=%d",
							userid, user.add_serial, sn, user.cash_add, cash);
					re.retcode = ERR_SUCCESS;
				}
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
			Log::log( LOG_ERR, "AddCash, userid=%d, what=%s\n", userid, e.what() );
			re.retcode = CASH_ADD_FAILED;
		}
		manager->Send(sid, re);
	};

};
}

#endif
