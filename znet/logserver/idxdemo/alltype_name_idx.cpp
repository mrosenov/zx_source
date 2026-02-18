#include "logcommon/indexmaker.hpp"

using namespace LOG;
class Alltype_name_idx : public IndexMaker
{
public:
	Alltype_name_idx(const std::string &src, const std::string &dest):IndexMaker(src,dest) {}
	virtual void make()
	{
		std::cout << "make" << std::endl;	
                std::string rec;
                IndexInfo ii;
                size_t type = 2004;
                while( tablechunk.orderReadRec(ii, rec, type))
                {
std::cout << "found " << std::endl;
                        Alltype protocol;
                        GNET::Octets oc(rec.c_str(), rec.length());
                        GNET::Marshal::OctetsStream os(oc);
                        os >> protocol;
                        std::ostringstream oss;
                        oss << protocol.name;
                        idb.insert(oss.str().c_str(), ii);
                        rec.clear();
                }
	}
};

int main()
{
	std::string src("/home/lijinhua/log/alltype");
	std::string dest("/home/lijinhua/logidx/alltype.idx.new");
	Alltype_name_idx ani(src, dest);

	float timeuse;
        struct timeval tpstart,tpend;
        gettimeofday(&tpstart,NULL);

	ani.make();

        gettimeofday(&tpend,NULL);
        timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
        tpend.tv_usec-tpstart.tv_usec;
        timeuse/=1000000;
        std::cout << "Used Time:" << timeuse << std::endl;

	return 0;
}
