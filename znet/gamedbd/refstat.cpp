#include "conf.h"
#include "log.h"
#include "storage.h"
#include "storagetool.h"
#include "xmlversion.h"
#include "user"
#include "gamedbmanager.h"
#include "localmacro.h"
#include "grolebase"
#include "grolebase2"
#include "greferral"
#include <iostream>
#include <set>
#include <time.h>

using std::set;
using namespace GNET;

void printhelp( const char * cmd )
{
	std::cerr << cmd << " version " << XMLVERSION << std::endl << std::endl;
	std::cerr << "Usage: " << cmd << " conf-file dbhomewdb" << std::endl;
}

static int64_t s_referrer_user_cnt = 0;
static int64_t s_referrer_role_cnt = 0;
static int64_t s_referral_user_cnt = 0;
static int64_t s_referral_role_cnt = 0;
static int64_t s_bonus_produced = 0;
static int64_t s_bonus_withdrawn = 0;
static int64_t s_bonus_reward = 0;
static set<int> s_referrer_roleids;
static set<int> s_referrer_userids;

class UserQuery : public StorageEnv::IQuery
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

			if (user.referrer > 0)
			{
				++s_referral_user_cnt;
				RoleList r(user.rolelist);
				s_referral_role_cnt += r.GetRoleCount();

				if (s_referrer_roleids.find(user.referrer) == s_referrer_roleids.end())
				{
					s_referrer_roleids.insert(user.referrer);
					++s_referrer_role_cnt;
				}
			}
		}
		catch ( ... )
		{
			fprintf( stderr, "UserQuery, marshal exception, userid=%d\n", userid);
		}

		return true;
	}
};

class Base2Query : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = 0;
		GRoleBase2 base2;

		try
		{
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> base2;

			s_bonus_reward += base2.bonus_reward;
			s_bonus_withdrawn += base2.bonus_withdraw;
		}
		catch ( ... )
		{
			fprintf( stderr, "Base2Query, marshal exception, roleid=%d\n", roleid);
		}

		return true;
	}
};

class ReferralQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = 0;
		GReferral referral;

		try
		{
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> referral;
			s_bonus_produced += referral.bonus_total1+referral.bonus_total2;
		}
		catch ( ... )
		{
			fprintf( stderr, "ReferralQuery, marshal exception, roleid=%d\n", roleid);
		}

		return true;
	}
};

void TravelUser()
{
	printf( "\nTravelUser:\n" );

	UserQuery q;
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
		}
	}
	catch ( DbException e )
	{
		fprintf( stderr, "TravelUser, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void CountReferrerUsers()
{
	printf("\nCountReferrerUsers\n");

	StorageEnv::Storage *pbase = StorageEnv::GetStorage("base");

	set<int>::iterator it, ie = s_referrer_roleids.end();
	for (it = s_referrer_roleids.begin(); it != ie; ++it)
	{
		try
		{
			StorageEnv::AtomTransaction txn;
			GRoleBase base;
			try
			{
				Marshal::OctetsStream(pbase->find(Marshal::OctetsStream()<<*it, txn)) >> base;
				int userid = base.userid;
				if (userid == 0)
					userid = LOGICUID(*it);

				if (userid > 0)
				{
					if (s_referrer_userids.find(userid) == s_referrer_userids.end())
					{
						++s_referrer_user_cnt;
						s_referrer_userids.insert(userid);
					}
				}
			}
			catch (DbException e) { throw; }
			catch ( ... )
			{
				DbException ee(DB_OLD_VERSION);
				txn.abort(ee);
				throw ee;
			}
		}
		catch (DbException e)
		{
			fprintf( stderr, "CountReferrrerUsers, error roleid=%d, what=%s\n", *it, e.what());
		}
	}

	StorageEnv::checkpoint();
}

void TravelBase2()
{
	printf( "\nTravelBase2:\n" );

	Base2Query q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base2" );
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "TravelBase2, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void TravelReferral()
{
	printf( "\nTravelReferral:\n" );

	ReferralQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "referral" );
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
	}
	catch ( DbException e )
	{
		fprintf( stderr, "TravelReferral, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

int main(int argc, char *argv[])
{
	if (argc!=3 || access(argv[1], R_OK) == -1 || access(argv[2], R_OK)==-1 )
	{
		printhelp( argv[0] );
		exit(-1);
	}
	Conf *conf = Conf::GetInstance(argv[1]);
	conf->put("storagewdb", "homedir", argv[2]);
	Log::setprogname("refstat");

	if(!StorageEnv::Open())
	{
		fprintf( stderr,"Initialize storage environment failed.\n");
		exit(-1);
	}

	TravelUser();
	CountReferrerUsers();
	TravelBase2();
	TravelReferral();
	StorageEnv::checkpoint();
	StorageEnv::Close();
	time_t now = time(NULL);
	struct tm tmnow;
	localtime_r(&now, &tmnow);
	printf("统计日期\t下线账号数\t下线角色数\t上线账号数\t上线角色数\t下线产生鸿利数\t上线提取鸿利数\t商城返鸿利数\n");
	printf("%d-%d-%d\t%lld\t\t%lld\t\t%lld\t\t%lld\t\t%lld\t\t%lld\t\t%lld\n", tmnow.tm_year+1900, tmnow.tm_mon+1, tmnow.tm_mday, s_referral_user_cnt,
			s_referral_role_cnt, s_referrer_user_cnt, s_referrer_role_cnt, s_bonus_produced, s_bonus_withdrawn, s_bonus_reward);
	return 0;
}
