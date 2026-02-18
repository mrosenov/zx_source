#include <stdio.h>
#include "conf.h"
#include "log.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "printofstream.h"

#include "localmacro.h"
#include "xmlversion.h"
#include "storage.h"

#include "roleid"
#include "user"
#include "userid"
#include "grolebase"
#include "grolebase2"
#include "grolestatus"
#include "dbconfig"
#include "roleimportbean"
#include "groupbean"
#include "friendimportbean"
#include "ggroupinfo"
#include "gfriendinfo"
#include "gfriendlist"
#include "gfamily"
#include "titlebean"
#include "factionimportbean"
#include "gfactioninfo"

namespace GNET
{

#define ROLEIDMIN(x) x
#define ROLEIDMAX(x) (x | 0x0000000f)

static char conf_filename[256];
static int filelimit = 100000000;

int FriendSetData(int roleid,std::vector<GroupBean> & ret,const GFriendInfoVector& friends,const GGroupInfoVector& group);
int FamilyMemberSetData(FactionImportBean & resp,const std::vector<GNET::GFolk>& info,int reserve_num);
int FamilyChangeTitle(FactionImportBean & resp);

struct ExportFactionQuery : public StorageEnv::IQuery
{
	StorageEnv::Storage *pfamily;
//	bool is_central;
	std::string filename;
	PrintOfstream of;

	ExportFactionQuery(StorageEnv::Storage *pfami,const std::string & fname):pfamily(pfami),filename(fname),of(filename.c_str(),filelimit)
	{}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid=0;
		Marshal::OctetsStream keyos,keyfami;
		Marshal::OctetsStream value_family,value_faction;
		try
		{
			try
			{
				keyos=key;
				keyos >> fid;
				
				GFactionInfo fac;
				value_faction = value;
				value_faction>>fac;

				FactionImportBean faction;
				faction.ftype = FACTIONTYPE;
				faction.factionid = fid;
				faction.factionname = fac.name;
				faction.announcement = fac.announce;

				std::vector<FamilyId>::iterator it,ite=fac.member.end();
				GFamily family;
				int member_num=0;
				for(it=fac.member.begin();it!=ite;++it)
				{
					keyfami.clear();
					keyfami << it->fid;
					Marshal::OctetsStream(pfamily->find( keyfami, txn ))>> family;
					member_num += family.member.size();
					FamilyMemberSetData(faction,family.member,210);
				}
		
				of.OutputEntry(faction);

				return true;
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
			Log::log( LOG_ERR, "ExportFactionQuery, fid=%d, what=%s\n", fid, e.what() );
			return true;
		}
		return true;
	}
};

void ExportFaction(const std::string & fname) 
{
	try
	{
		StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
		StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
		StorageEnv::AtomTransaction	txn;
		
		ExportFactionQuery q(pfamily,fname);
		
		try
		{
			StorageEnv::Storage::Cursor cursor = pfaction->cursor( txn );
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportFaction, error when walk, what=%s\n", e.what() );
	}
}

int FamilyChangeTitle(FactionImportBean & resp)
{
	std::vector<GNET::TitleBean>::iterator vit,vite;
	vit=resp.members.begin();
	vite=resp.members.end();
	for(;vit!=vite;++vit)
	{
		if(vit->titleid==TITLE_MASTER || vit->titleid==TITLE_VICEMASTER || vit->titleid==TITLE_CAPTAIN)
		{
			vit->titleid = TITLE_HEADER;
		}
	}
	return 0;
}

int FamilyMemberSetData(FactionImportBean & resp,const std::vector<GNET::GFolk>& info,int reserve_num)
{
	std::vector<GNET::GFolk>::const_iterator it,ite=info.end();
	std::vector<GNET::TitleBean>::iterator vit,vite;
	for(it=info.begin();it!=ite;++it)
	{
		vit=resp.members.begin();
		vite=resp.members.end();
		for(;vit!=vite;++vit)
		{
			if(vit->titleid == it->title)
			{
				vit->members.push_back(it->rid);
				break;
			}
		}
		if(vit==vite)
		{
			resp.members.push_back(TitleBean(it->title));
			resp.members.back().members.reserve(reserve_num);//only normal folk need
			resp.members.back().members.push_back(it->rid);		
		}
	}
	return 0;

}

struct ExportFamilyQuery : public StorageEnv::IQuery
{
	std::string filename;
	PrintOfstream of;
	ExportFamilyQuery(const std::string & fname):filename(fname),of(filename.c_str(),filelimit)
	{}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid=0;
		Marshal::OctetsStream keyos;
		Marshal::OctetsStream value_family;
		try
		{
			try
			{
				keyos=key;
				keyos >> fid;
				
				GFamily info;
				value_family = value;
				value_family>>info;

				FactionImportBean family;
				family.ftype = FAMILYTYPE;
				family.factionid = fid;
				family.factionname = info.name;
				family.announcement = info.announce;

				FamilyMemberSetData(family,info.member,15);		
				FamilyChangeTitle(family);
				of.OutputEntry(family);
				return true;
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
			Log::log( LOG_ERR, "ExportFamilyQuery, fid=%d, what=%s\n", fid, e.what() );
			return true;
		}
		return true;
	}
};

void ExportFamily(const std::string & fname) 
{
	try
	{
		StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
		StorageEnv::AtomTransaction	txn;
		
		ExportFamilyQuery q(fname);
		
		try
		{
			StorageEnv::Storage::Cursor cursor = pfamily->cursor( txn );
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportFamily, error when walk, what=%s\n", e.what() );
	}
}

int FriendSetData(int roleid,std::vector<GroupBean> & ret,const GFriendInfoVector& friends,const GGroupInfoVector& group)
{
	GGroupInfoVector::const_iterator git,gite=group.end();
	ret.push_back(GroupBean(FRIENDTYPE,0));//default group
	for(git=group.begin();git!=gite;++git)
		ret.push_back(GroupBean(FRIENDTYPE,git->gid,git->name));

	std::vector<GroupBean>::iterator vit,vite;
	GFriendInfoVector::const_iterator it,ite=friends.end();
	vite=ret.end();

	for(it=friends.begin();it!=ite;++it)
	{
		for(vit=ret.begin();vit!=vite;++vit)
		{
			if(static_cast<char>(vit->groupid) == it->gid)
			{
				vit->friendlist.push_back(it->rid);
				break;
			}
		}
		if(vit==vite)
		{
	//		ret.push_back(GroupBean(FRIENDTYPE,it->gid));
			LOG_TRACE("FriendSetData:role %d friend roleid=%d cannot find group, gid=%d\n",roleid,it->rid,it->gid);
		}
	}
	return 0;
}

struct ExportFriendQuery : public StorageEnv::IQuery
{
	std::string filename;
	PrintOfstream of;

	ExportFriendQuery(const std::string & fname):filename(fname),of(filename.c_str(),filelimit)
	{}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		GFriendList flist;
		int roleid=0;
		Marshal::OctetsStream keyos;
		Marshal::OctetsStream value_friend;
		try
		{
			try
			{
				keyos=key;
				keyos >> roleid;
				
				value_friend = value;
				value_friend>>flist;
				if(flist.friends.size()==0)
					return true;
				FriendImportBean rfriend;
				rfriend.roleid = roleid;
				FriendSetData(roleid,rfriend.friends,flist.friends,flist.groups);
				
				of.OutputEntry(rfriend);
				return true;
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
			Log::log( LOG_ERR, "ExportFriendQuery, roleid=%d, what=%s\n", roleid, e.what() );
			return true;
		}
		return true;
	}
};

void ExportFriend(const std::string & fname) 
{
	try
	{
		StorageEnv::Storage * pfriends = StorageEnv::GetStorage("friends");
		StorageEnv::AtomTransaction	txn;
		
		ExportFriendQuery q(fname);
		
		try
		{
			StorageEnv::Storage::Cursor cursor = pfriends->cursor( txn );
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportFriend, error when walk, what=%s\n", e.what() );
	}
}

struct ExportRoleQuery : public StorageEnv::IQuery
{
	StorageEnv::Storage *pstatus;
	std::string filename;
	PrintOfstream of;

	ExportRoleQuery(StorageEnv::Storage *pstat,const std::string & fname):pstatus(pstat),filename(fname),of(filename.c_str(),filelimit)
	{}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		GRoleBase base;	
		GRoleStatus status;
		int roleid=0;
		Marshal::OctetsStream keyos;
		Marshal::OctetsStream value_base,value_status;
		try
		{
			try
			{
				keyos=key;
				keyos >> roleid;
				if(roleid>0 && roleid<128)
					return true;
				
				value_base = value;
				value_base>>base;
				RoleImportBean	role;
				
				role.roleid = roleid;
				role.rolename = base.name;
				role.gender = base.gender;
	
				if( pstatus->find( keyos, value_status, txn ) )
				{
					value_status >> status;
					role.occupation = status.occupation;
					role.level = status.level;
					role.reborntimes = status.reborndata.size()/sizeof(int);
					role.cultivation = status.cultivation;
				}
				of.OutputEntry(role);

				return true;
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
			Log::log( LOG_ERR, "ExportRoleQuery, roleid=%d, what=%s\n", roleid, e.what() );
			return true;
		}
		return true;
	}
};

void ExportRole(const std::string & fname) 
{
	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage( "base" );
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage( "status" );
		StorageEnv::AtomTransaction	txn;
		
		ExportRoleQuery q(pstatus,fname);
		
		try
		{
			StorageEnv::Storage::Cursor cursor = pbase->cursor( txn );
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportRole, error when walk, what=%s\n", e.what() );
	}
}

}; // namespace GNET

int main(int argc, char *argv[])
{
	if (argc < 2 || access(argv[1], R_OK) == -1 )
	{
		exit(-1);
	}
	Conf *conf = Conf::GetInstance(argv[1]);
	int zoneid=atoi(conf->find("GameDBServer", "zoneid").c_str());//get zoneid from config file
	int aid=11;
	std::stringstream pres;
      	pres<<aid << "_"<< zoneid<<"_";

	strcpy(conf_filename,argv[1]);
	Log::setprogname("gtimport");
	if(!StorageEnv::Open())
	{
		Log::log(LOG_ERR,"Initialize storage environment failed.\n");
		exit(-1);
	}
	if( argc == 4)
		filelimit = atoi(argv[3]);
	if( (argc == 3 || argc ==4 ) && 0 == strcmp(argv[2],"exportrole") )
	{
		pres<<"rolebase";
		ExportRole(pres.str());
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( (argc == 3 || argc ==4 ) && 0 == strcmp(argv[2],"exportfriend") )
	{
		pres<<"rolefriends";
		ExportFriend(pres.str());
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( (argc == 3 || argc ==4 ) && 0 == strcmp(argv[2],"exportfaction") )
	{
		pres<<"rolefactions";
		ExportFaction(pres.str());
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	else if( (argc == 3 || argc ==4 ) && 0 == strcmp(argv[2],"exportfamily") )
	{
		pres<<"rolefamilies";
		ExportFamily(pres.str());
		StorageEnv::checkpoint();
		StorageEnv::Close();
		return 0;
	}
	
	return 0;
}

