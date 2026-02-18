#include "certverify.hpp"
#include "wanmeica.h"

#include <string.h>
#include <sys/time.h>
#include <openssl/x509.h>
#include <openssl/err.h>

namespace GNET
{

CertVerify::CertVerify(unsigned char* cert, int len)
{
#if (OPENSSL_VERSION_NUMBER>=0x0090800fL)    
	const unsigned char *ca_cert = wanmeica_certificate;                                                                     
	x509_ca = d2i_X509(NULL, &ca_cert, sizeof(wanmeica_certificate));        
	x509_cert = d2i_X509(NULL, (const unsigned char**) &cert, len);                                               
#else 
	unsigned char *ca_cert = wanmeica_certificate; 
	x509_ca = d2i_X509(NULL, &ca_cert, sizeof(wanmeica_certificate));               
	x509_cert = d2i_X509(NULL,  &cert, len);                                                                      
#endif 

	cert_pkey = X509_get_pubkey(x509_cert);	
	ca_pkey = X509_get_pubkey(x509_ca);	

	//X509_print_fp(stdout, x509_ca);
	//X509_print_fp(stdout, x509_cert);
}

CertVerify::~CertVerify()
{
	if (x509_ca != NULL){
		X509_free(x509_ca);
		x509_ca = NULL;
	}

	if (x509_cert != NULL){
		X509_free(x509_cert);
		x509_cert = NULL;
	}

	if (cert_pkey != NULL){
		EVP_PKEY_free(cert_pkey);
		cert_pkey = NULL;
	}

	if (ca_pkey != NULL){
		EVP_PKEY_free(ca_pkey);
		ca_pkey = NULL;
	}
}


//
// verify the certification's OU and CN name
//
int CertVerify::VerifyCert()
{
	char ou_name[32];
	char cn_name[64];

	//To do: need to enhance to use X509_NAME_entry_count to deal with multi entres 
	X509_NAME_get_text_by_NID(X509_get_subject_name(x509_cert), NID_organizationalUnitName, ou_name, 32);
	if (strcmp(ou_name, "authd") != 0){
		return 0;
	}

	X509_NAME_get_text_by_NID(X509_get_subject_name(x509_cert), NID_commonName, cn_name, 64);
	if (strstr(cn_name, "au.wanmei.com") == NULL ){
		return 0;
	}	
	
	return 1;
}

//
// verify the cert is issued by the root cert
//
int CertVerify::VerifyCertByCA()
{
	OpenSSL_add_all_algorithms();

	X509_STORE *cert_ctx = NULL;

	cert_ctx=X509_STORE_new();
	X509_STORE_add_cert(cert_ctx, x509_ca);
	X509_STORE_CTX *csc = X509_STORE_CTX_new();

	X509_STORE_set_default_paths(cert_ctx);    
	
	if(!X509_STORE_CTX_init(csc,cert_ctx ,x509_cert,NULL)){         
		X509_STORE_CTX_free(csc);
		return 0;
	}

	if(X509_verify_cert(csc) != 1){ 
		X509_STORE_CTX_free(csc);    
		return 0;
	}
	X509_STORE_CTX_free(csc);
	return 1;
}

//
//Encrypt the key with the RSA public key
//
int CertVerify::Encrypt(int key_len, unsigned char* key, unsigned char* buf)
{
	return RSA_public_encrypt(key_len, key, buf, cert_pkey->pkey.rsa, RSA_PKCS1_PADDING);
}

//
//Decrypt the key with the RSA public key
//
int CertVerify::Decrypt(int buf_len, unsigned char* buf, unsigned char* key)
{
	return RSA_public_decrypt(buf_len, buf, key, cert_pkey->pkey.rsa, RSA_PKCS1_PADDING);
}


}; //End namespace
