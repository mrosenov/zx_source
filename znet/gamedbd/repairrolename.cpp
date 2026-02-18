#include "conf.h"
#include "log.h"
#include "storage.h"
#include "storagetool.h"
#include "xmlversion.h"
#include "conv_charset.h"
#include "gfamily"
#include "gsect"
#include "gfriendlist"
#include "grolebase"
#include "message"
#include "gmailbox"
#include "gmail"

#include <map>
#include <set>
#include <string>
#include <iostream>
using std::string;
using std::map;
using std::set;
using namespace GNET;

struct lt_Octets
{
	bool operator() (const Octets & o1, const Octets & o2) const
	{
		if( o1.size() < o2.size() )
			return true;
		if( o1.size() > o2.size() )
			return false;
		return memcmp(o1.begin(),o2.begin(),o2.size()) < 0;
	}
};

static char conf_filename[256];
static	Octets		g_octRoleNamePrefix;
static	Octets		g_octRoleNameSuffix;
static map<int, Octets> g_mapIdToRolename;

void printhelp( const char * cmd )
{
	std::cerr << "repairrolename version " << XMLVERSION << std::endl << std::endl;
	std::cerr << "Usage: " << cmd << " conf-file" << std::endl;
}

void LoadConfig()
{
	std::cout << "LoadConfig: " << std::endl;
	string strRoleNamePrefix		=	Conf::GetInstance()->find( "gamedbd", "rolenameprefix" );
	string strRoleNameSuffix		=	Conf::GetInstance()->find( "gamedbd", "rolenamesuffix" );
	Octets      l_octRoleNamePrefix( strRoleNamePrefix.c_str(), strRoleNamePrefix.length() );
	Octets      l_octRoleNameSuffix( strRoleNameSuffix.c_str(), strRoleNameSuffix.length() );
	CharsetConverter::conv_charset_l2u( l_octRoleNamePrefix, g_octRoleNamePrefix );
	CharsetConverter::conv_charset_l2u( l_octRoleNameSuffix, g_octRoleNameSuffix );

	fprintf( stderr, "\tINFO:role name prefix(local):" );		l_octRoleNamePrefix.dump();
	fprintf( stderr, "\tINFO:role name suffix(local):" );		l_octRoleNameSuffix.dump();
	fprintf( stderr, "\tINFO:role name prefix(UTF-16LE):" );		g_octRoleNamePrefix.dump();
	fprintf( stderr, "\tINFO:role name suffix(UTF-16LE):" );		g_octRoleNameSuffix.dump();
}

class RepairBaseQuery : public StorageEnv::IQuery
{
	set<Octets,lt_Octets> m_setRolename;
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

			if (base.name.size() > 0)
			{
				if (m_setRolename.find(base.name) != m_setRolename.end())
				{
					printf("RepairBaseQuery, rolename dupliated, roleid=%d\n", roleid);
					int i = 1;
					while( true )
					{
						Octets testname = base.name;
						testname.insert( testname.begin(), g_octRoleNamePrefix.begin(), g_octRoleNamePrefix.size() );
						testname.insert( testname.end(), g_octRoleNameSuffix.begin(), g_octRoleNameSuffix.size() );
						if( i > 1 )
						{
							char buffer[64];
							sprintf( buffer, "%d", i );
							Octets oct_count_local(buffer,strlen(buffer)), oct_count;
							CharsetConverter::conv_charset_l2u( oct_count_local, oct_count );
							testname.insert( testname.end(), oct_count.begin(), oct_count.size() );
						}

						Octets value_tmp;
						if( m_setRolename.find(testname) == m_setRolename.end() )
						{
							base.name = testname;
							break;
						}
						i ++;
					}
					m_mapBase[roleid] = base;
				}
				g_mapIdToRolename[base.id] = base.name;
				m_setRolename.insert(base.name);
			}
			else
				fprintf( stderr, "RepairBaseQuery, rolename empty, roleid=%d\n", roleid);
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

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				map<int, GRoleBase>::iterator it, ie = mapBase.end();

				for (it = mapBase.begin(); it !=  ie; ++it)
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairBase, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void RepairRolename()
{
	printf( "\nRepairRolename:\n" );

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "rolename" );
		StorageEnv::CommonTransaction	txn;
		try
		{
			map<int, Octets>::iterator it, ie = g_mapIdToRolename.end();

			for (it = g_mapIdToRolename.begin(); it != ie; ++it)
			{
				pstorage->insert(it->second, Marshal::OctetsStream()<<it->first, txn);
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
		fprintf( stderr, "RepairRolename, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class RepairFriendsQuery : public StorageEnv::IQuery
{
	private:
		map<int, GFriendList> &m_mapFriendList;

	public:
		RepairFriendsQuery(map<int, GFriendList> &mapFriendList) : m_mapFriendList(mapFriendList) { } 

		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int roleid;
			GFriendList friendlist;

			try
			{
				Marshal::OctetsStream(key) >> roleid;
				Marshal::OctetsStream(value) >> friendlist;
				bool need_repair = false;

				for( GFriendInfoVector::iterator it = friendlist.friends.begin(), ite = friendlist.friends.end(); it != ite; ++it )
				{
					if (it->name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->rid) != g_mapIdToRolename.end())
						{
							it->name = g_mapIdToRolename[it->rid]; 
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair friend name, roleid=%d, friend=%d\n", roleid, it->rid);
						}
					}
				}

				for( GEnemyInfoVector::iterator it = friendlist.enemies.begin(), ite = friendlist.enemies.end(); it != ite; ++it )
				{
					if (it->name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->rid) != g_mapIdToRolename.end())
						{
							it->name = g_mapIdToRolename[it->rid];
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair enemy name, roleid=%d, enemy=%d\n", roleid, it->rid);
						}
					}
				}

				if (need_repair)
					m_mapFriendList[roleid] = friendlist;
			}
			catch ( ... )
			{
				fprintf( stderr, "RepairFriendsQuery, marshal exception, roleid=%d\n", roleid);
			}

			return true;
		}
};

void RepairFriends()
{
	printf( "\nRepair friends:\n" );

	map<int, GFriendList> mapFriendList;
	RepairFriendsQuery q(mapFriendList);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "friends" );

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk(q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				map<int, GFriendList>::iterator it, ie = mapFriendList.end();

				for (it = mapFriendList.begin(); it != ie; ++it)
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairFriends, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class RepairFamilyQuery : public StorageEnv::IQuery
{
	private:
		map<int, GFamily> &m_mapFamily;

	public:
		RepairFamilyQuery(map<int, GFamily> &mapFamily) : m_mapFamily(mapFamily) { } 

		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int familyid;
			GFamily family;

			try
			{
				Marshal::OctetsStream(key) >> familyid;
				Marshal::OctetsStream(value) >> family;
				bool need_repair = false;

				for( GFolkVector::iterator it = family.member.begin(), ite = family.member.end(); it != ite; ++it )
				{
					if (it->name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->rid) != g_mapIdToRolename.end())
						{
							it->name = g_mapIdToRolename[it->rid];
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair family member name, familyid=%d, memberid=%d\n", familyid, it->rid);
						}
					}
				}

				if (need_repair)
					m_mapFamily[familyid] = family;
			}
			catch ( ... )
			{
				fprintf( stderr, "RepairFamilyQuery, marshal exception, familyid=%d\n", familyid);
			}

			return true;
		}
};

void RepairFamily()
{
	printf( "\nRepair family:\n" );

	map<int, GFamily> mapFamily;
	RepairFamilyQuery q(mapFamily);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "family" );
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk(q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				map<int, GFamily>::iterator it, ie = mapFamily.end();

				for (it = mapFamily.begin(); it != ie; ++it)
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairFamily, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class RepairMailBoxQuery : public StorageEnv::IQuery
{
	private:
		map<int, GMailBox> &m_mapMailBox;

	public:
		RepairMailBoxQuery(map<int, GMailBox> &mapMailBox) : m_mapMailBox(mapMailBox) { } 

		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int roleid;
			GMailBox mailbox;

			try
			{
				Marshal::OctetsStream(key) >> roleid;
				Marshal::OctetsStream(value) >> mailbox;
				bool need_repair = false;

				for( GMailVector::iterator it = mailbox.mails.begin(), ite = mailbox.mails.end(); it != ite; ++it )
				{
					if (it->header.sender_name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->header.sender) != g_mapIdToRolename.end())
						{
							it->header.sender_name = g_mapIdToRolename[it->header.sender];
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair mailbox sender name, roleid=%d, sender=%d\n", roleid, it->header.sender);
						}
					}
				}

				if (need_repair)
					m_mapMailBox[roleid] = mailbox;
			}
			catch ( ... )
			{
				fprintf( stderr, "Repair mailbox, marshal exception, roleid=%d\n", roleid);
			}

			return true;
		}
};

void RepairMailBox()
{
	printf( "\nRepair mailbox:\n" );

	map<int, GMailBox> mapMailBox;
	RepairMailBoxQuery q(mapMailBox);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "mailbox" );
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk(q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				map<int, GMailBox>::iterator it, ie = mapMailBox.end();

				for (it = mapMailBox.begin(); it != ie; ++it)
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairMailBox, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class RepairMessageQuery : public StorageEnv::IQuery
{
	private:
		map<int, MessageVector> &m_mapMessage;

	public:
		RepairMessageQuery(map<int, MessageVector> &mapMessage) : m_mapMessage(mapMessage) { } 

		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int roleid;
			MessageVector msgs;

			try
			{
				Marshal::OctetsStream(key) >> roleid;
				Marshal::OctetsStream(value) >> msgs;
				bool need_repair = false;

				for( MessageVector::iterator it = msgs.begin(), ite = msgs.end(); it != ite; ++it )
				{
					if (it->src_name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->srcroleid) != g_mapIdToRolename.end())
						{
							it->src_name = g_mapIdToRolename[it->srcroleid];
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair message src_name, roleid=%d, src_roleid=%d\n", roleid, it->srcroleid);
						}
					}

					if (it->dst_name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->dstroleid) != g_mapIdToRolename.end())
						{
							it->dst_name = g_mapIdToRolename[it->dstroleid];
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair message dst_name, roleid=%d, dst_roleid=%d\n", roleid, it->dstroleid);
						}
					}
				}

				if (need_repair)
					m_mapMessage[roleid] = msgs;
			}
			catch ( ... )
			{
				fprintf( stderr, "Repair message, marshal exception, roleid=%d\n", roleid);
			}

			return true;
		}
};

void RepairMessage()
{
	printf( "\nRepair message:\n" );

	map<int, MessageVector> mapMessage;
	RepairMessageQuery q(mapMessage);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "messages" );
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk(q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				map<int, MessageVector>::iterator it, ie = mapMessage.end();

				for (it = mapMessage.begin(); it != ie; ++it)
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairMessage, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class RepairSectQuery : public StorageEnv::IQuery
{
	private:
		map<int, GSect> &m_mapSect;

	public:
		RepairSectQuery(map<int, GSect> &mapSect) : m_mapSect(mapSect) { } 

		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int roleid;
			GSect sect;

			try
			{
				Marshal::OctetsStream(key) >> roleid;
				Marshal::OctetsStream(value) >> sect;
				bool need_repair = false;

				if (sect.name.size() == 0)
				{
					if (g_mapIdToRolename.find(roleid) != g_mapIdToRolename.end())
					{
						sect.name = g_mapIdToRolename[roleid];
						need_repair = true;
					}
					else
					{
						fprintf( stderr, "Fail to repair sect name, sectid=%d\n", roleid);
					}
				}

				for( GDiscipleVector::iterator it = sect.disciples.begin(), ite = sect.disciples.end(); it != ite; ++it )
				{
					if (it->name.size() == 0)
					{
						if (g_mapIdToRolename.find(it->roleid) != g_mapIdToRolename.end())
						{
							it->name = g_mapIdToRolename[it->roleid];
							need_repair = true;
						}
						else
						{
							fprintf( stderr, "Fail to repair sect disciple name, sectid=%d, disciple id=%d\n", roleid, it->roleid);
						}
					}
				}

				if (need_repair)
					m_mapSect[roleid] = sect;
			}
			catch ( ... )
			{
				fprintf( stderr, "Repair sect, marshal exception, sectid=%d\n", roleid);
			}

			return true;
		}
};

void RepairSect()
{
	printf( "\nRepair sect:\n" );

	map<int, GSect> mapSect;
	RepairSectQuery q(mapSect);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "sect" );
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk(q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}

		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				map<int, GSect>::iterator it, ie = mapSect.end();

				for (it = mapSect.begin(); it != ie; ++it)
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "RepairSect, error what=%s\n", e.what() );
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
	strcpy(conf_filename,argv[1]);
	Log::setprogname("repairrolename");

	if(!StorageEnv::Open())
	{
		fprintf( stderr,"Initialize storage environment failed.\n");
		exit(-1);
	}

	LoadConfig();
	RepairBase();
	RepairRolename();
	RepairFriends();
	RepairFamily();
	RepairMailBox();
	RepairMessage();
	RepairSect();
	StorageEnv::checkpoint();
	StorageEnv::Close();
	return 0;
}
