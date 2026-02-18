#include "conf.h"
#include "log.h"
#include "storage.h"
#include "storagetool.h"
#include "xmlversion.h"
#include "user"

using namespace GNET;

void RepairUserCash(int userid)
{
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		StorageEnv::CommonTransaction	txn;
		Marshal::OctetsStream key, value;
		User user;
		try
		{
			key << userid;
			if (pstorage->find(key, value, txn))
			{
				value >> user;
				int old_cash = user.cash;
				if (user.cash < 0)
					user.cash = 0;
				pstorage->insert(key, Marshal::OctetsStream()<<user, txn);
				printf("Repair user %d cash from %d to %d\n", userid, old_cash, user.cash);
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
		fprintf( stderr, "Fail to repair user %d cash, what=%s\n", userid, e.what() );
	}
	StorageEnv::checkpoint();
}

int main(int argc, char *argv[])
{
	if (argc < 2 || access(argv[1], R_OK) == -1 )
	{
		exit(-1);
	}
	Conf *conf = Conf::GetInstance(argv[1]);
	Log::setprogname("repairusercash");

	if(!StorageEnv::Open())
	{
		fprintf( stderr,"Initialize storage environment failed.\n");
		exit(-1);
	}

	RepairUserCash(1122760640);
	
	StorageEnv::checkpoint();
	StorageEnv::Close();
	return 0;
}
