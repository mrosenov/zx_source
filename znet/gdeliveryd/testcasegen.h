#ifndef __GNET_TESTCASEGEN_H
#define __GNET_TESTCASEGEN_H

#include <map>
#include "log.h"

namespace GNET
{
enum
{
	EMPTY_TEST = 1,
};
class TestCaseBase
{
private:
	int id;


	typedef std::map<int/*id*/, TestCaseBase *> STUBMAP;
	static STUBMAP stubmap;
	static TestCaseBase * GetStub(unsigned char _type)
	{
		STUBMAP::iterator it = stubmap.find(_type);
		if (it == stubmap.end())
			return NULL;
		else
			return it->second;
	}
	virtual TestCaseBase * Clone() = 0;
protected:
	TestCaseBase(int _id) : id(_id)
	{
		if (GetStub(id) == NULL)
			stubmap[id] = this;
	}
public:
	static TestCaseBase * Create(int caseid)
	{
		TestCaseBase * stub = GetStub(caseid);
		return stub == NULL ? NULL : stub->Clone();
	}
	static void RunTest(int id, int roleid, int param1, int param2, int param3)
	{
		TestCaseBase * tester = Create(id);
		if (tester == NULL)
		{
			Log::log(LOG_ERR, "Can not generate a test case %d", id);
			return;
		}
		tester->Run(roleid, param1, param2, param3);
		delete tester;
	}
	virtual ~TestCaseBase() {}
	virtual void Run(int roleid, int param1, int param2, int param3) = 0;
};

class EmptyTest : public TestCaseBase
{
private:
	TestCaseBase * Clone() { return new EmptyTest(*this); }
	EmptyTest() : TestCaseBase(EMPTY_TEST) {}
	static EmptyTest stub;
public:
	void Run(int roleid, int param1, int param2, int param3)
	{
		LOG_TRACE("role %d run an empty test,pa1 %d pa2 %d pa3 %d", roleid, param1, param2, param3);
	}
};
};//end of GNET namespace
#endif

