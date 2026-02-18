#ifndef __GNET_CLSCONFIG_H
#define __GNET_CLSCONFIG_H

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "log.h"

#include "dbbuffer.h"

#include "roleid"
#include "grolebase"
#include "grolestatus"
#include "groleinventory"
#include "grolestorehouse"
#include "groledetail"
#include "groletableclsconfig"
#include "accessdb.h"

namespace GNET
{
void ImportClsConfig( )
{
	if( 0 != access("./clsconfig",R_OK) )
		return;
	/*
	std::string datadir = StorageEnv::get_datadir();
	StorageEnv::checkpoint();
	StorageEnv::Close();

	std::string clsconfigfile = datadir + "/clsconfig";
	system( ("/bin/cp -f ./clsconfig " + clsconfigfile).c_str() );

	StorageEnv::Open();

	if( 0 != access(clsconfigfile.c_str(),R_OK) )
		return;
	*/
	TableWrapper tab_clsconfig("./clsconfig");
	if (!tab_clsconfig.Open())
	{
		Log::log(LOG_ERR, "Open clsconfig failed");
		assert(false);
	}
	tab_clsconfig.SetCompressor(new StorageEnv::NullCoder(), new StorageEnv::NullCoder());
	unsigned int roleid;
	//system roleid 16-23 system2 roleid 32-35  roleid 24-31 没用使用
	for( roleid = 16; roleid<=48; roleid ++ )
	{
		try
		{
//			StorageEnv::Storage * pclsconfig = StorageEnv::GetStorage("clsconfig");
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
//			pclsconfig->SetCompressor(new StorageEnv::NullCoder(), new StorageEnv::NullCoder());
			StorageEnv::CommonTransaction txn;
			try
			{
				GRoleTableClsconfig	clsconfig;
				Marshal::OctetsStream key_temp, os_value;
				key_temp << roleid;
//				Marshal::OctetsStream(pclsconfig->find( key_temp, txn )) >> clsconfig;
				if (!tab_clsconfig.Find(key_temp, os_value))
				{
					Log::log(LOG_ERR, "Can not find clsconfig data role %d", roleid);
					continue;
				}
				os_value >> clsconfig;
				pbase->insert( key_temp, Marshal::OctetsStream() << clsconfig.base, txn );
				pstatus->insert( key_temp, Marshal::OctetsStream() << clsconfig.status, txn );
				pinventory->insert( key_temp, Marshal::OctetsStream() << clsconfig.inventory, txn );
				pstorehouse->insert( key_temp, Marshal::OctetsStream() << clsconfig.storehouse,txn);
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
			Log::log( LOG_ERR, "ImportClsConfig, roleid=%d, what=%s\n", roleid, e.what() );
		}
	}

	StorageEnv::checkpoint();
	StorageEnv::Close();
	StorageEnv::Open();
	Log::log( LOG_INFO, "ClsConfig Imported ." );
}
void ExportClsConfig( )
{
	std::string datadir = StorageEnv::get_datadir();
	std::string clsconfigfile = datadir + "/clsconfig";

	unsigned int roleid;
	for( roleid = 16; roleid<=48; roleid ++ )
	{
		Marshal::OctetsStream key, value_all, value_base, value_status,
							value_inventory, value_storehouse;
		GRoleTableClsconfig	clsconfig;

		try
		{
			StorageEnv::Storage * pclsconfig = StorageEnv::GetStorage("clsconfig");
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			pclsconfig->SetCompressor( new StorageEnv::NullCoder(), new StorageEnv::NullCoder() );
			StorageEnv::CommonTransaction txn;
			try
			{
				key << RoleId(roleid);
				if( pbase->find( key, value_base, txn ) )
					value_base >> clsconfig.base;

				if( pstatus->find( key, value_status, txn ) )
					value_status >> clsconfig.status;

				if( pinventory->find( key, value_inventory, txn ) )
					value_inventory >> clsconfig.inventory;

				if( pstorehouse->find( key, value_storehouse, txn ) )
					value_storehouse >> clsconfig.storehouse;

				value_all << clsconfig;
				pclsconfig->insert( key, value_all, txn );
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
			Log::log( LOG_ERR, "SaveClsConfig, roleid=%d, what=%s\n", roleid, e.what() );
		}
	}

	StorageEnv::checkpoint();
	system( ("/bin/cp -f " + clsconfigfile + " ./clsconfig").c_str() );
	Log::log( LOG_INFO, "ClsConfig Exported ." );
}

void ClearClsConfig( )
{
	unsigned int roleid;
	for( roleid = 38; roleid<64; roleid ++ )
	{
		Marshal::OctetsStream key;
		key << RoleId(roleid);

		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			StorageEnv::CommonTransaction txn;

			pbase->del( key, txn );
			pstatus->del( key, txn );
			pinventory->del( key, txn );
			pstorehouse->del( key, txn );
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ClearClsConfig, roleid=%d, what=%s\n", roleid, e.what() );
		}
	}

	StorageEnv::checkpoint();
}

}

#endif

