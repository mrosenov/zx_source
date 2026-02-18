#include "sysmailcontainer.h"
#include <fstream>


namespace GNET
{

std::map<string, SysMailInfo> SysMailContainer::mail_container;

bool SysMailContainer::Init(const char *file)
{
	return (file && access(file, R_OK) == 0) ? Reload(file) : false;
}

bool SysMailContainer::IsMailExist(const string &mail_type)
{
	return mail_container.find(mail_type) != mail_container.end() ? true : false;
}

SysMailInfo &SysMailContainer::GetMailInfo(const string &mail_type)
{
	return mail_container[mail_type];
}

bool SysMailContainer::Reload(const char *file)
{
	std::ifstream ifs(file);
	string line;
	string section;
	SysMailInfo  mailinfo;
	if(!mail_container.empty()) mail_container.clear();
	while(std::getline(ifs, line))
	{
		const char c = line[0];
		if(c == '#' || c == ';') continue;
		if(c == '[')
		{
			string::size_type start = line.find_first_not_of(" \t", 1);
			if(start == string::npos) continue;
			string::size_type end = line.find_first_of(" \t]", start);
			if(end == string::npos) continue;
			if(!section.empty()) mail_container[section] = mailinfo;
			section = string(line, start, end - start);
			mailinfo.Clear();
		}
		else
		{
			string::size_type key_start = line.find_first_not_of(" \t");
			if(key_start == string::npos) continue;
			string::size_type key_end = line.find_first_of(" \t=", key_start);
			if(key_end == string::npos) continue;
			string::size_type val_start = line.find_first_of("=", key_end);
			if(val_start == string::npos) continue;
			val_start = line.find_first_of("{", val_start + 1) + 1;
			if(val_start == string::npos) continue;
			string::size_type val_end = line.find_last_not_of("}\r\n");
			if(val_end == string::npos) continue;
			if(val_end < val_start) continue;
			string key(line, key_start, key_end - key_start);
			string value(line, val_start, val_end - val_start + 1);
			bool append = mailinfo.IsExist(key);
			mailinfo.SetInfo(key, value, append);
		}
	}
	if(!section.empty()) mail_container[section] = mailinfo;
	return true;
}

};
