#ifndef __LOG_APPCONFIG_HPP__
#define __LOG_APPCONFIG_HPP__

#include <string>
#include <iostream>
#include <stdlib.h>
#include <set>
#include <map>
#include "xmlconfigdb.h"
#include "util.hpp"
#include "config.hpp"

namespace LOG
{

class Config2
{		
	//////////////////////////////////
	// Ω‚ŒˆXML 
	/////////////////////////////////
	typedef std::list< XmlObject > Children;

public:

	static Config2 *GetInstance(const char *file = NULL)
        {                               
                if (file && access(file, R_OK) == 0)
                {                       
                        instance.xmlFile = file;
                        instance.LoadXML();
                }                       
                return &instance;       
        }        

	RotateFile GetRotateFile() { return rotatefile; }

	type_file_map_t get_type_file_map() { return type_file_map; }

	denyset_t GetDenySet() { return denySet; }

	std::string GetDefaultFile() {	return defaultFile; }

	std::string GetAnyFile() {	return anyFile; }

	std::string GetRecordDir() { return recordDir; }

	std::string GetRotateDir() { return rotateDir; }

	std::string GetParseDir() { return parseDir; }

	std::string GetApplicationFile() { return xmlFile;}//xmlconfigdb.GetFileName();

	size_t GetInterval() { return interval; }

	void Dump()
	{
		LogRec( "recordDir = %s" ,recordDir.c_str());
		LogRec( "rotatedir = %s" ,rotateDir.c_str());
		LogRec( "parseDir = %s" ,parseDir.c_str());
		LogRec("defaultFile = %s", defaultFile.c_str());
		LogRec( "interval = %d", interval);
		LogRec(" anyFile = %s", anyFile.c_str() );
		LogRec( "type_file_map size = %d", type_file_map.size());
		for(type_file_map_t::iterator it = type_file_map.begin(); it != type_file_map.end(); ++it)
		{
			LogRec("type = %d ", (*it).first );
			for( fileset_t::iterator iit = (*it).second.begin(); iit != (*it).second.end(); ++iit )
			{
				LogRec( "filename = %s", (*iit).c_str() );
			}
		}
		LogRec( " deny size = %d", denySet.size() );
		for(denyset_t::iterator it = denySet.begin(); it != denySet.end(); ++it)
			LogRec( " deny type = %d", (*it) );
		LogRec("rotate file set ...");
		for(RotateFile::iterator it = rotatefile.begin(); it != rotatefile.end(); ++it)
			LogRec(" rotatefile[%s] = [%d]", (*it).first.c_str(), (*it).second);
	}


private:
	enum { ROTATEDEFAULT = 604800 };
	void LoadXML( ) 
	{
		LogRec( "LoadXML in :%s", xmlFile.c_str() );
		xmlobject = xmlconfigdb.Load(xmlFile); 
		/////////////////////
		// get directory
		/////////////////////
		Children childrenDir = xmlobject.GetChildren("directory");
		Children::iterator itDir = childrenDir.begin();
		recordDir = (*itDir).GetAttr("recorddir");
		CheckDir(recordDir); 
		itDir++;
		rotateDir = (*itDir).GetAttr("rotatedir");
		CheckDir(rotateDir); 
		itDir++;
		parseDir = (*itDir).GetAttr("parsedir");
		CheckDir(parseDir); 

		/////////////////////
		// get default
		/////////////////////
		Children childrenDefault = xmlobject.GetChildren("default");
		Children::iterator itDef = childrenDefault.begin();
		defaultFile = (*itDef).GetAttr("file");
		std::string temp = (*itDef).GetAttr("interval");
		if(!temp.empty())
			interval = atoi( temp.c_str() );
		else
			interval = 60;

		/*
		if(defaultFile.empty())
			defaultFile = GetRecordDir() + "default.log";
		else	
			defaultFile = GetRecordDir() + defaultFile + ".log";
		*/
		if (!defaultFile.empty())
		{
			rotatefile[defaultFile] = atoi( (*itDef).GetAttr("rotate").c_str() );
			/////////////////////
			// get denySet in default
			/////////////////////
			Children denychildren = (*itDef).GetChildren("deny");
			for(Children::iterator it_deny = denychildren.begin(); it_deny != denychildren.end(); ++it_deny)
			{
				denySet.insert( atoi((*it_deny).GetAttr("id").c_str()) );
			}
		}
		/////////////////////
		// get any
		/////////////////////
		Children any = xmlobject.GetChildren("any");
		Children::iterator itAny = any.begin();
		anyFile = (*itAny).GetAttr("file");
		if( ! anyFile.empty() )
		{
			anyFile = GetRecordDir() + anyFile + ".log";
			rotatefile[anyFile] = atoi( (*itAny).GetAttr("rotate").c_str() );
		}
		/////////////////////
		// get target
		/////////////////////
		Children children = xmlobject.GetChildren("target");
		for(Children::iterator it_target = children.begin(); it_target != children.end(); ++it_target)
		{
			std::string cur_file = GetRecordDir() + (*it_target).GetAttr("file") + ".log";
			if ( cur_file.empty() ) continue;
			int rotate = atoi( (*it_target).GetAttr("rotate").c_str() );
			if ( rotate <= 0 ) rotate = ROTATEDEFAULT;
			rotatefile[cur_file] = rotate;
			Children logidchildren = (*it_target).GetChildren("logid");
			for(Children::iterator it_logid=logidchildren.begin(); it_logid != logidchildren.end(); ++ it_logid)
			{
				type_t type = atoi( (*it_logid).GetAttr("id").c_str() );
				type_file_map[type].insert( cur_file ); // ! cur_file
			}
		}
		LogRec( "LoadXML out");
	}

	XmlConfigDB xmlconfigdb;
	XmlObject xmlobject;
	RotateFile rotatefile;
	std::string xmlFile;
	denyset_t denySet; 
	std::string recordDir;
	std::string rotateDir;
	std::string parseDir;
	std::string defaultFile;
	std::string anyFile;
	type_file_map_t type_file_map;
	size_t interval;
	static Config2 instance;
	static bool isLoad;
};

}// end namespace LOG
#endif
