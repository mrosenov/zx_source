#ifndef _GNET_SYSMAILCONTAINER_H
#define _GNET_SYSMAILCONTAINER_H

#include <string>
#include <map>
#include "gmail"

namespace GNET
{
using std::string;
class GMail;

struct SysMailInfo
{
public:
	enum
	{
		SENDER_NAME,
		TITLE,
		CONTEXT,
		MAX_MAILSECTION,
	};
	struct stringcasecmp
	{
		bool operator() (const string &x, const string &y) const { return strcasecmp(x.c_str(), y.c_str()) < 0; }
	};
	std::map<string, string, stringcasecmp> values;
	void Clear()
	{
		values.clear();
	}
	bool IsExist(const string &key) const
	{
		return values.find(key) != values.end();
	}
	void SetInfo(const string &key, const string &value, bool append = false)
	{
		string str_val = append ? ("\r\n" + value) : value;
		values[key] += str_val;
	}

	const string &GetInfo(const string &key)
	{
		return values[key];
	}
};

class SysMailContainer
{
public:
	static bool Init(const char *file = NULL);
	static bool IsMailExist(const string &mail_type);
	static SysMailInfo &GetMailInfo(const string &mail_type);

	const static int MAX_MAILTYPE = 3;
private:
	static bool Reload(const char *file = NULL);
	static std::map<string, SysMailInfo> mail_container;
};

};
#endif // _GNET_SYSMAILCONTAINER_H
