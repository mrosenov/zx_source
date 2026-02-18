#include "conf.h"
#include "log.h"
#include "gamedbmanager.h"
#include "storage.h"
#include "storagetool.h"
#include "xmlversion.h"
#include "conv_charset.h"
#include "user"
#include "grolebase"

#include <set>
#include <iostream>
using std::map;
using namespace GNET;

static map<int, int> g_mapRoleId2UserId;

void printhelp( const char * cmd )
{
	std::cerr << "repairuserid version " << XMLVERSION << std::endl << std::endl;
	std::cerr << "Usage: " << cmd << " conf-file" << std::endl;
}

class PrepareUserIdQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		User user;
		int userid;

		try
		{
			Marshal::OctetsStream(key) >> userid;
			Marshal::OctetsStream(value) >> user;

			if (user.logicuid && (user.rolelist&0xFFFF))
			{
				RoleList r(user.rolelist);
				int roleid = -1;
				while ((roleid=r.GetNextRole()) >= 0)
				{
					g_mapRoleId2UserId[user.logicuid+roleid] = userid;
				}
			}
		}
		catch ( ... )
		{
			fprintf( stderr, "PrepareUserIdQuery, marshal exception, userid=%d\n", userid);
		}

		return true;
	}
};

void PrepareUserId()
{
	printf( "\nPrepareUserId:\n" );

	PrepareUserIdQuery  q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		{
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			printf("mapRoleid2Userid size=%d\n", g_mapRoleId2UserId.size());
		}
	}
	catch ( DbException e )
	{
		fprintf( stderr, "PrepareUserId, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class RepairBaseQuery : public StorageEnv::IQuery
{
	map<int, GRoleBase> &m_mapBase;

public:
	RepairBaseQuery(map<int, GRoleBase> &mapBase) : m_mapBase(mapBase) { } 

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		GRoleBase base;
		int roleid;

		try
		{
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> base;
			
			if (base.userid == 0)
			{
				base.userid = g_mapRoleId2UserId[roleid];
				if (base.userid == 0)
				{
					fprintf( stderr, "RepairBaseQuery, fail to repair role, roleid=%d\n", roleid);
				}
				m_mapBase[roleid] = base;
			}
		}
		catch ( ... )
		{
			fprintf( stderr, "RepairBaseQuery, marshal exception, roleid=%d\n", roleid);
		}

		return true;
	}
};

void RepairBase()
{
	printf( "\nRepairBase:\n" );

	map<int, GRoleBase> mapBase;
	RepairBaseQuery  q(mapBase);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		{
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		printf("Number of roles that will be repaired: %d\n", mapBase.size());

		{
			map<int, GRoleBase>::iterator it = mapBase.begin(), ie = mapBase.end();
			int count = 0;
			int i;
			while (it != ie)
			{
				{
					StorageEnv::CommonTransaction	txn;
					try
					{
						for (i = 0; i<50000 && it!=ie; ++i, ++it)
						{
							pstorage->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second, txn);
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

				StorageEnv::checkpoint();
				count += i;
				printf("Repair %d roles\n", count);
			}
		}
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairBase, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

int main(int argc, char *argv[])
{
	if (argc < 2 || access(argv[1], R_OK) == -1 )
	{
		printhelp( argv[0] );
		exit(-1);
	}
	Conf *conf = Conf::GetInstance(argv[1]);
	Log::setprogname("repairuserid");

	if(!StorageEnv::Open())
	{
		fprintf( stderr,"Initialize storage environment failed.\n");
		exit(-1);
	}

	PrepareUserId();
	RepairBase();
	StorageEnv::checkpoint();
	StorageEnv::Close();
	return 0;
}
