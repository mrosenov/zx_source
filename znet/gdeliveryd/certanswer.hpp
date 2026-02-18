
#ifndef __GNET_CERTANSWER_HPP
#define __GNET_CERTANSWER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "certverify.hpp"
#include "certkey.hpp"

namespace GNET
{

class CertAnswer : public GNET::Protocol
{
	#include "certanswer"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("CertAnswer received");
		CertVerify cert((unsigned char*)authd_cert.begin(), authd_cert.size());

		// if(authd_cert.size() == 0)
		// {
			// DEBUG_PRINT("Error to receieve the au cert");
			// Log::log(LOG_ERR,"Error to receieve the au cert");
			// exit(-1);
		// }
		// if (cert.VerifyCert() == 0){
			// DEBUG_PRINT("Error to verify the cert");
			// Log::log(LOG_ERR,"Error to verify the cert");
			// exit(-1);
		// }

		// if (cert.VerifyCertByCA() == 0){
			// DEBUG_PRINT("Error to verify the cert by CA");
			// Log::log(LOG_ERR,"Error to verify the cert by CA");
			// exit(-1);
		// }

		int key_size = 8;
		Octets d_key1, d_key2;
		d_key1.resize(key_size);
		d_key2.resize(key_size);
	
		Security *random = Security::Create(RANDOM);
		random->Update(d_key1);
		random->Update(d_key2);
		random->Destroy();

		int buf_size = cert.GetBufferSize();
		Octets en_key1, en_key2;
		en_key1.resize(buf_size);
		en_key2.resize(buf_size);

		cert.Encrypt(key_size, (unsigned char*)d_key1.begin(), (unsigned char*)en_key1.begin());
		cert.Encrypt(key_size, (unsigned char*)d_key2.begin(), (unsigned char*)en_key2.begin());

		CertKey cert_key(en_key1, en_key2);		
		
		GAuthClient *au_client = GAuthClient::GetInstance();
		au_client->SetISecurity(sid, ARCFOURSECURITY, d_key2);
		au_client->SendProtocol(cert_key);
		au_client->osec_key = d_key1; 
		au_client->authd_cert = authd_cert;
	}
};

};

#endif
