#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "xmlconfigdb.h"
#include "stringcaster.h"
namespace LOG
{

void XmlDB::Save(const std::string &file) const
{
	CheckDir(file);
	std::ofstream ofs(GetFileName().c_str());
	ofs << (StringCaster<XmlObject>::ToString(WriteObject()));
}

XmlObject XmlDB::Load(const std::string &file)
{
	CheckDir(file);
//	std::ifstream ifs(GetFileName().c_str());
	std::ifstream ifs(file.c_str());
	std::string buffer;
	std::string res;
	
	if( ifs.is_open() ) 
		while( ! ifs.eof() ) 
		{ 
			std::getline(ifs, buffer); 
			res += buffer + XmlObject::nl; 
		}
	return  StringCaster<XmlObject>::FromString(res);
//	LoadObject(xmlobject);
}

void XmlDB::CheckDir(const std::string &fn) const
{
//	std::string fn = GetFileName();
	std::string::size_type pos = fn.find_first_of('/');
	if( pos == std::string::npos ) return;
	while( pos != std::string::npos )
	{
		struct stat buff;
		const char *pn = fn.substr(0, pos).c_str();
		if( -1 == stat(pn, &buff) )
		{
			//if( errno == ENOENT )
				mkdir(pn, 0775);
		//	else
		//		throw Exception();
		}
		else
		{
			if( !(S_ISDIR(buff.st_mode)) )
				throw Exception();
		}
		
		pos = fn.find_first_of('/', pos+1);
	}
}

};//end namespace
