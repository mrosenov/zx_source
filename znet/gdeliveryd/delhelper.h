
#ifndef __GNET_DELHELPER_HPP_INCLUDE
#define __GNET_DELHELPER_HPP_INCLUDE

#include "waitdelkey"
namespace GNET
{

class DeleteHelper
{
public:

	static DeleteHelper * Instance() 
	{ 
		static DeleteHelper inst; 
		return &inst;
	}

	void DeleteNext();
	void AddDeleteList(WaitDelKeyVector& _todel);
private:
	DeleteHelper() : cursor(0) {}

	std::vector<WaitDelKey> todel;
	unsigned int cursor;
};

};

#endif //__GNET_DELROLEANNOUNCE_HPP
