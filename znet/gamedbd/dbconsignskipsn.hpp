
#ifndef __GNET_DBCONSIGNSKIPSN_HPP
#define __GNET_DBCONSIGNSKIPSN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class DBConsignSkipSN : public GNET::Protocol
{
	#include "dbconsignskipsn"
	static const int64_t CONSIGNID_FOR_SN_STORAGE=0;
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		try
		{
			StorageEnv::Storage * pconsign = StorageEnv::GetStorage("consign");
			StorageEnv::CommonTransaction txn;
			try
			{
				Marshal::OctetsStream ksn;
				ksn <<CONSIGNID_FOR_SN_STORAGE;
				int64_t sn;
				Marshal::OctetsStream stateos;
				if(skip_amount<=0)
				{
			       		Log::log( LOG_ERR, "DBConsignSkipSN skip_amount=%d",skip_amount);
					return;
				}
				if(pconsign->find(ksn, stateos,txn)==true)// use this find to avoid exception
				{
					stateos>>sn;
					sn += skip_amount;
					Log::formatlog("consign","db consign sn skip %d newsn=%lld",skip_amount,sn);
					pconsign->insert(ksn,Marshal::OctetsStream()<<sn, txn);
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
			Log::log( LOG_ERR, "DBConsignSkipSN, what=%s\n", e.what() );
		}
	}
};

};

#endif
