#ifndef __GNET_MAPPRIVILEGE_H
#define __GNET_MAPPRIVILEGE_H

#include <set>
#include <map>

#include "rpcdefs.h"
#include "thread.h"

namespace GNET
{

class UserPrivilege
{
	typedef std::set<unsigned char> Set;
	typedef std::map<int/*userid*/, Set> Map;
	Map map;
	Thread::Mutex locker;
	static UserPrivilege	instance;
public:
	UserPrivilege() : locker("UserPrivilege::locker") { }
	~UserPrivilege() { }
	static UserPrivilege & GetInstance() { return instance; }
	size_t Size() { Thread::Mutex::Scoped l(locker);	return map.size();	}

	bool PrivilegeCheck(int userid,unsigned char privilege)
	{
		Thread::Mutex::Scoped l(locker);
		Map::const_iterator it = map.find(userid);
		return ( it!=map.end() && (*it).second.find(privilege)!=(*it).second.end() );
	}

	bool GetPrivilege( int userid, ByteVector & auth )
	{
		Thread::Mutex::Scoped l(locker);
		Map::iterator it = map.find( userid );
		if (it != map.end() )
		{
			Set & pvset = (*it).second;
			auth.clear();
			for ( Set::iterator itp=pvset.begin(), ite=pvset.end(); itp!=ite; ++itp )
				auth.push_back((*itp));
			return true;
		}
		return false;
	}

	void SetPrivilege( int userid, ByteVector & auth )
	{
		Set pvset;
		for (size_t i=0;i<auth.size();i++)
			pvset.insert(auth[i]);

		Thread::Mutex::Scoped l(locker);
		map[userid].swap(pvset);
	}
};

};

#endif

