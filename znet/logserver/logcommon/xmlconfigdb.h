#ifndef __GNET_XMLCONFIGDB
#define __GNET_XMLCONFIGDB

#include <vector>
#include "xmlobject.h"
namespace LOG
{
//class XmlObject;

class XmlDB
{
public:
        class Exception { };
protected:
        virtual std::string GetFileName() const = 0;
        virtual XmlObject WriteObject() const = 0;
        virtual void LoadObject(const XmlObject &_obj) = 0;
        virtual ~XmlDB() { }
public:
        XmlObject Load(const std::string &);
        void Save(const std::string &) const;
private:
        void CheckDir(const std::string &) const;
};

class XmlConfigDB : public XmlDB
{
	std::string version;
public:
	~XmlConfigDB() { }
	std::string GetFileName() const { return ""; }
	XmlObject WriteObject() const ;//{ return XmlObject("null"); }

	void LoadObject(const XmlObject &_obj);	
	std::string GetVersion() const { return version; }
};

};

#endif
