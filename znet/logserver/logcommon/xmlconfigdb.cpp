#include <iostream>
#include "xmlconfigdb.h"
#include "xmlobject.h"

namespace LOG
{

/*
	std::string XmlConfigDB::GetFileName() const 
	{
		return "application.xml";
	}
*/
	void XmlConfigDB::LoadObject(const XmlObject &_obj)	
	{
		version = _obj.GetAttr("version");
	}
	
       	XmlObject XmlConfigDB::WriteObject() const 
	{
		return XmlObject("null"); 
	}  
};

