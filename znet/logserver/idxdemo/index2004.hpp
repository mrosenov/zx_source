#include "logcommon/indexmaker.hpp"
#include "logcommon/index.hpp"
#include "logcommon/util.hpp"
#include "alltype.hpp"

struct Index2004 : public LOG::Index
{
        virtual GNET::Protocol * create(const GNET::Marshal::OctetsStream &data) const
        {
                GNET::Alltype *protocol = new Alltype();
		data  >> *protocol;
                return protocol;
        }
        virtual LOG::keyvalue_t extractkeyvalue(GNET::Protocol *p) const
        {
                GNET::Alltype *protocol = dynamic_cast< GNET::Alltype * >(p);
                if (protocol)
                {
                        GNET::Marshal::OctetsStream os;
                        os << protocol->race;
                        //protocol->dump();
                        return os;
                }
                throw LOG::Exception("dynamic_cast error");
        }
        virtual LOG::type_t get_type() const { return 2004; }
        virtual std::string get_keyname() const { return "race"; }
        virtual bool is_rebuild() const { return true; }
};
