#include "certverify.hpp"
#include "wanmeica.h"

#include <string.h>
#include <sys/time.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/rand.h>

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
// ignore the error if the certification is expired
//
static int verify_callback(int ok, X509_STORE_CTX *ctx)
{
	if(X509_STORE_CTX_get_error(ctx) == X509_V_ERR_CERT_HAS_EXPIRED)
		return 1;
	return ok;
}

//
// verify the cert is issued by the root cert
//
int CertVerify::VerifyCertByCA()
{
	ERR_load_BIO_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	X509_STORE *cert_ctx = NULL;

	cert_ctx=X509_STORE_new();
	X509_STORE_add_cert(cert_ctx, x509_ca);
	X509_STORE_CTX *csc = X509_STORE_CTX_new();

	X509_STORE_set_default_paths(cert_ctx);    
	
	if(!X509_STORE_CTX_init(csc,cert_ctx ,x509_cert,NULL)){         
		ERR_print_errors_fp(stderr);
		X509_STORE_CTX_free(csc);
		X509_STORE_free(cert_ctx);
		return 0;
	}

	X509_STORE_CTX_set_verify_cb(csc, verify_callback);	

	if(X509_verify_cert(csc) != 1){ 
		ERR_print_errors_fp(stderr);
		X509_STORE_CTX_free(csc);    
		X509_STORE_free(cert_ctx);
		return 0;
	}
	X509_STORE_CTX_free(csc);
	X509_STORE_free(cert_ctx);
	return 1;
}

//
//Get the public key from the PEM file
//
EVP_PKEY * CertVerify::GetPublicKey(const char *filename)
{
    BIO *in = NULL;
    X509 *ca = NULL;

    if (filename == NULL){
        return NULL;
    }

    if((in = BIO_new_file(filename, "r"))==NULL){
        fprintf(stderr, "Unable to load the cert\n");
        return NULL;
    }

    if((ca= PEM_read_bio_X509(in, NULL, NULL, NULL)) == NULL){
        fprintf(stderr, "Unable to read the cert\n");
        BIO_free(in);
        return NULL;
    }

    EVP_PKEY *public_key =X509_get_pubkey(ca);
    return public_key;
}

//
//Get the rpivate key from the PEM file
//
EVP_PKEY * CertVerify::GetPrivateKey(const char *filename, const char *password)                                             
{                                                                                                                 
    BIO            *bp;                                                                                           
    EVP_PKEY      *pkey;                                                                                          
                                                                                                                  
    if ((bp = BIO_new(BIO_s_file())) == NULL)                                                                     
    {                                                                                                             
        ERR_print_errors_fp(stderr);                                                                              
        return (NULL);                                                                                            
    }                                                                                                             
                                                                                                                  
    BIO_read_filename(bp, filename);                                                                              
                                                                                                                  
    if ((pkey = PEM_read_bio_PrivateKey(bp, NULL, NULL, (char *)password))== NULL)                                
    {                                                                                                             
        ERR_print_errors_fp(stderr);                                                                              
        BIO_free(bp);                                                                                             
        return (NULL);                                                                                            
    }                                                                                                             
    BIO_free(bp);                                                                                                 
                                                                                                                  
    return (pkey);                                                                                                
}                             

//
//Generate the rand number by the openssl rand method
//
void CertVerify::GenerateRandNum(unsigned char *buf, int size)
{
	struct timeval tv;
	unsigned int counter = 0;
	
	while (RAND_status() != 1){
		if (gettimeofday(&tv, NULL) < 0){
			fprintf(stderr, "gettimeofday() failed\n" );
			break;
		}
		RAND_add(&tv, sizeof(tv), sizeof(tv) / 2);
		
		if(++counter > 100){
			break;
		}
	}
	RAND_bytes(buf, size);
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
