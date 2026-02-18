
#ifndef __GNET_CONTROLPROTOCOL_HPP
#define __GNET_CONTROLPROTOCOL_HPP

#include "rpcdefs.h"
//#include "callid.hxx"
//#include "state.hxx"
#define PROTOCOL_CONTROLPROTOCOL 12000
namespace GNET
{

class ControlProtocol : public GNET::Protocol
{
        public:
                std::string content;
                size_t commond;
                enum { PROTOCOL_TYPE = PROTOCOL_CONTROLPROTOCOL };
        public:
                ControlProtocol() { type = PROTOCOL_CONTROLPROTOCOL; }
                ControlProtocol(void*) : Protocol(PROTOCOL_CONTROLPROTOCOL) { }
                ControlProtocol (std::string l_content, size_t l_commond = 0)
                        : content(l_content),commond(l_commond)
                        {
                                type = PROTOCOL_CONTROLPROTOCOL;
                        }

                ControlProtocol(const ControlProtocol &rhs)
                        : Protocol(rhs),content(rhs.content),commond(rhs.commond) { }

                GNET::Protocol *Clone() const { return new ControlProtocol(*this); }

                OctetsStream& marshal(OctetsStream & os) const
                {
                        STAT_MIN5("ControlProtocol",os.size());
                        os << content;
                        os << commond;
                        return os;
                }

                const OctetsStream& unmarshal(const OctetsStream &os)
                {
                        STAT_MIN5("ControlProtocol",os.size());
                        os >> content;
                        os >> commond;
                        return os;
                }

                bool SizePolicy(size_t size) const { return size <= 102400; }
                void Process(Manager *manager, Manager::Session::ID sid)
                {
                        // TODO
                }
		std::ostream & trace(std::ostream &os)
		{
			os << "file = " << content << " commond =" << commond;
			return os;
		}
};

};

#endif
