#include "logcommon/indexmaker.hpp"
#include "logcommon/index.hpp"
#include "logcommon/util.hpp"
#include "response.hpp"

struct Index2001_id : public LOG::Index
{
        virtual GNET::Protocol * create(const GNET::Marshal::OctetsStream &data) const
        {
                GNET::Response *protocol = new GNET::Response();
		//size_t size;
                data >> *protocol;
                return protocol;
        }
        virtual LOG::keyvalue_t extractkeyvalue(GNET::Protocol *p) const
        {
                GNET::Response *protocol = dynamic_cast< GNET::Response * >(p);
                if(protocol)
                {
                        //protocol->dump();
                        GNET::Marshal::OctetsStream os;
                        os << protocol->id;
                        return os;
                }
                throw LOG::Exception("dynamic_cast error");
        }
        virtual LOG::type_t get_type() const { return 2001; }
        virtual std::string get_keyname() const { return "id"; }
        virtual bool is_rebuild() const { return true; }
};
