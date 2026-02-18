#include "logcommon/indexmaker.hpp"
#include "logcommon/index.hpp"
#include "logcommon/util.hpp"
#include "alltype.hpp"

struct Index2004_name : public LOG::Index
{
        virtual GNET::Protocol * create(const GNET::Marshal::OctetsStream &data) const
        {
                GNET::Alltype *protocol = new GNET::Alltype();
		//size_t size;
                //data >> GNET::CompactUINT(size)>> *protocol;
		data >> *protocol;
                return protocol;
        }
        virtual LOG::keyvalue_t extractkeyvalue(GNET::Protocol *p) const
        {
                GNET::Alltype *protocol = dynamic_cast< GNET::Alltype * >(p);
                if(protocol)
                {
                        //protocol->dump();
                        GNET::Marshal::OctetsStream os;
                        os.push_byte( protocol->name, sizeof(protocol->name) );
                        return os;
                }
                throw LOG::Exception("dynamic_cast error");
        }
        virtual LOG::type_t get_type() const { return 2004; }
        virtual std::string get_keyname() const { return "name"; }
        virtual bool is_rebuild() const { return true; }
};
