
#ifndef __GNET_ALLTYPE_HPP
#define __GNET_ALLTYPE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class Alltype : public GNET::Protocol
{
	public:
		char name[32];
		int race;
		int cls;
		bool gender;
		std::string msg;
		std::string hostname;
		std::string servicename;
		Octets nonce;
		enum { PROTOCOL_TYPE = 2004 };
	public:
		Alltype() { type = PROTOCOL_ALLTYPE; }
		Alltype(void*) : Protocol(PROTOCOL_ALLTYPE) { }
		Alltype (const char * l_name,int l_race = 0,int l_cls = 0,
			bool l_gender = false,std::string l_msg = std::string(),std::string l_hostname = std::string(),
			std::string l_servicename = std::string(),Octets l_nonce)
			 : race(l_race),cls(l_cls),
			gender(l_gender),msg(l_msg),hostname(l_hostname),
			servicename(l_servicename),nonce(l_nonce)
		{
			type = PROTOCOL_ALLTYPE;
			strncpy( name, l_name, sizeof(name)-1 );
			name [sizeof(name)-1] = 0;
		}

		Alltype(const Alltype &rhs)
			: Protocol(rhs),race(rhs.race),cls(rhs.cls),
			gender(rhs.gender),msg(rhs.msg),hostname(rhs.hostname),
			servicename(rhs.servicename),nonce(rhs.nonce)
		{
			memcpy( name, rhs.name, sizeof(name) );
		}

		GNET::Protocol *Clone() const { return new Alltype(*this); }

		OctetsStream& marshal(OctetsStream & os) const
		{
			STAT_MIN5("Alltype",os.size());
			os.push_byte(name,sizeof(name));
			os << race;
			os << cls;
			os << gender;
			os << msg;
			os << hostname;
			os << servicename;
			os << nonce;
			return os;
		}

		const OctetsStream& unmarshal(const OctetsStream &os)
		{
			STAT_MIN5("Alltype",os.size());
			os.pop_byte(name,sizeof(name));
			os >> race;
			os >> cls;
			os >> gender;
			os >> msg;
			os >> hostname;
			os >> servicename;
			os >> nonce;
			return os;
		}

		bool SizePolicy(size_t size) const { return size <= 102400; }
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
	void dump()
	{
		std::cout << "Protocl alltype 2004 dump:" << std::endl;
		std::cout << "name = " << name << std::endl;
		std::cout << "race = " << race << std::endl;
		std::cout << "cls = " << cls << std::endl;
		std::cout << "gender(bool) = " << gender << std::endl;
		std::cout << "msg = " << msg << std::endl;
		std::cout << "hostname = " << hostname << std::endl;
		std::cout << "servicename = " << servicename << std::endl;
		std::cout << "nonce(Octets).size() = " << nonce.size() << std::endl;
		std::cout << "---------alltype 2004 end -----" << std::endl;
	}
};

};

#endif
