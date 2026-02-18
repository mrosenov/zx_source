
#ifndef __GNET_RESPONSE_HPP
#define __GNET_RESPONSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "security.h"

namespace GNET
{

class Response : public GNET::Protocol
{

        #include "response"
        void toHexString(Octets& os)
        {
                unsigned char* p;
                for (size_t i=0;i<os.size();i++)
                {
                        p=((unsigned char*) os.begin()) + i;
                        printf("%x",*p);
                }
                printf("\n");
        }

        void Setup(Octets& name,Octets& passwd,Octets challenge)
        {
                HMAC_MD5Hash hash;
                MD5Hash md5;
                Octets digest;

                md5.Update(name);
                md5.Update(passwd);
                //DEBUG_PRINT("passwd is "); toHexString(passwd);
                md5.Final(digest);
                //DEBUG_PRINT("response digest passwd is "); toHexString(digest);

                hash.SetParameter(digest);
                hash.Update(challenge);
                hash.Final(response);
                //DEBUG_PRINT("response is "); toHexString(response);
                identity.replace(name.begin(),name.size());
                /*
                HMAC_MD5Hash hash;
                hash.SetParameter(passwd);
                hash.Update(challenge);
                hash.Final(response);
                identity.replace(name.begin(),name.size());
                */
        }
	        void Process(Manager *manager, Manager::Session::ID sid)
        {
                // TODO
        }	
};

};

#endif
