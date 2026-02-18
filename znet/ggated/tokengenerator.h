#ifndef __GNET_TOKENGENERATOR_H
#define __GNET_TOKENGENERATOR_H

#include <map>
#include <string>
#include "localmacro.h"
#include <openssl/aes.h>
#include <cstdio>
#include "customkey"

namespace GNET
{
using std::string;

class CustomKey;
class TokenGenerator: public IntervalTimer::Observer
{
public:
	static const unsigned int AESKEYBITS =128;
	static const unsigned int AESKEYSIZE =16;
	static const int AESBLOCKSIZE =16;
	static const int MD5SIZE =16;
	static const unsigned int KEY_CACHE_SIZE=20;
	static const unsigned int ENCODED_KEY_SIZE=32;
	struct Key_128
	{
		unsigned char key_array[AESKEYSIZE];
		Key_128(){}
		Key_128(const Key_128& k){
			memcpy(key_array,k.key_array,AESKEYSIZE);
		}
		Key_128(const string& s){
			memcpy(key_array,s.c_str(),AESKEYSIZE);
		}	
	};
	class Openssl_encoder
	{
		AES_KEY key;//used by openssl
		bool inited;
		string keyid_pre;
		int64_t kid_;
	public:
		void set_key(const Key_128& newkey,int64_t kid);
		int Encrypt(unsigned char* plaintext,unsigned char* ciphertext,int length){
			if(!inited)
				return -1;
			for(int i=0;i<length;i+=AESBLOCKSIZE)
				AES_ecb_encrypt(plaintext+i, ciphertext+i, &key, AES_ENCRYPT);
			return 0;
		}
		string& GetKeyid(){return keyid_pre;}
		int64_t GetKid(){return kid_;}
		Openssl_encoder():inited(false),kid_(0){}
	};	
private:
	class Openssl_decoder
	{
		AES_KEY key;//used by openssl
		bool inited;
	public:
		void set_key(const Key_128& newkey);
		int Decrypt(unsigned char* ciphertext,unsigned char* plaintext,int length){
			if(!inited)
				return -1;
			for(int i=0;i<length;i+=AESBLOCKSIZE)
				AES_ecb_encrypt(ciphertext+i, plaintext+i, &key, AES_DECRYPT);
			return 0;
		}
		Openssl_decoder():inited(false){}
	};	
	class Openssl_Key
	{
//		unsigned char encoded_key[64];
		Octets encoded_key;
		Key_128 key;
		int time_limit;
	public:
	/*	Openssl_Key()
		{
			//unsigned char ckey[] = "f1e554af2f501bf7";
			unsigned char keystr[33] = "6051889a1e38810817fccb825588f77f";
			AsciiHex2Binary((char*)keystr,(char*)key.key_array,sizeof(key.key_array));
			memcpy(encoded_key,"dc5aab8b047b22f8c1a251d72bd7d89519f2a0100050eb6c3445224ad6d99d0b",64);
		}*/
		Openssl_Key(const unsigned char* encoded,const Key_128& k,int overdue_time):time_limit(overdue_time)
		{
			//memcpy(encoded_key,encoded,64);
			encoded_key.insert(encoded_key.begin(),encoded,ENCODED_KEY_SIZE);
			key=k;
		}
		Openssl_Key(Octets &encoded,const Key_128& k,int overdue_time):time_limit(overdue_time)
		{
			encoded_key.swap(encoded);
			key=k;
		}
		Openssl_Key(const Openssl_Key& rhs):time_limit(rhs.time_limit)
		{
			encoded_key = rhs.encoded_key;
			key=rhs.key;
		}	
		Key_128& GetKey(){return key;}
		bool IsEncodedEqual(const Octets& oc){return oc==encoded_key;}
		bool IsTimeout(int now){return now>(time_limit);/*2 minutes*/}
		int GetTimeLimit(){return time_limit;}
	};
	typedef Openssl_encoder Encoder;
	typedef Openssl_decoder Decoder;
	typedef	Openssl_Key KeyNodeType;
	typedef Key_128 KeyType;
	typedef std::map<int64_t, KeyNodeType> KeyMap;
	KeyMap keys;
	static const int UPDATE_INTERVAL=2;
	Encoder encoder;
	Decoder key_decoder;
	bool encoder_inited;
	bool key_decoder_inited;
	Octets kc_name;
	int kc_activeId;
	bool kc_inited;
	int connection_ttl;
	int ticks;
	static const int KEEPALIVE_INTERVAL=60;
	static const int CONN_TTL_MAX=50;
public:
	static TokenGenerator& GetInstance() { static TokenGenerator instance;return instance; }
	TokenGenerator():encoder_inited(false),key_decoder_inited(false),kc_inited(false),connection_ttl(0),ticks(0){}
	virtual ~TokenGenerator(){
		keys.clear();
	}
	void Initialize(const string& sharekey,const string& kcname,int kcactiveid);
	bool Update()
	{
		//GetNewKey();
		//FakeSetKey();
                CheckConnection();
		//FakeUser();
		return true;
	}
	int OnGetNewKeys(std::map<int64_t,CustomKey> & keylist,int64_t kds_time);
	int BuildToken(const string & profile,string& token);
	int BuildToken(const string & profile,const Octets & rolename, const string & profile1, string& token);
	bool FakeSetKey(){
		if(IsEncoderInit())
			return true;
		if(!IsKeyDecoderInit())
			return true;
		KeyType key;
		unsigned char encoded_key[]="8fc685ae16aab4c0a38bf37596470e8c80c6b1928f6c500b62942c698e61e8b1";
		Decode_Key_64(encoded_key,64,key);
		KeyNodeType newkey(encoded_key,key,Timer::GetTime()+3600);
		keys.insert(std::make_pair(1304573327112LL,newkey));
		EncoderSetKey(newkey.GetKey(),1304573327112LL );
		return true;
	}
	int GetNewKey();//send KeysReq to KDS
	void OnKDSKeepAlive(){connection_ttl = CONN_TTL_MAX;}
private:
	void CheckConnection();
	void FakeUser(){
		string profile("aid=19&uid=990064&zid=1999&rid=9272035");
		string token;
		if(BuildToken(profile,token))
			return;
//		DEBUG_PRINT("GetToken profile=%.*s tokensize=%d token=%.*s",profile.size(),profile.c_str(),token.size(),token.size(),token.c_str());
	}
	int64_t EncoderGetKeyid(){
		return encoder.GetKid();
	}
	void EncoderSetKey(KeyType& key,int64_t kid){
		encoder.set_key(key,kid);
		encoder_inited=true;
	}
	void KeyDecoderSetKey(KeyType& key){
		key_decoder.set_key(key);
		key_decoder_inited=true;
	}
	bool IsKCInit(){return kc_inited==true;}
	void SetKCInit(bool v){kc_inited = v;}
	//functions for build token 
	bool IsEncoderInit(){return encoder_inited==true;}
	int Padding(const string & profile,unsigned char* &plaintext);//get formatted plaintext which can be encoded
	int Padding(const string & profile,const Octets & rolename,const string & profile1,unsigned char* &plaintext);//get formatted plaintext which can be encoded
	//functions for add new keys
	bool IsKeyDecoderInit(){return key_decoder_inited==true;}
	void CheckOldkeys(std::map<int64_t,CustomKey> & keylist);
	int AddKey(int64_t keyid,Octets& encoded,int overdue_time);
	int Decode_Key(const unsigned char* ciphertext,unsigned int length,KeyType& key);
	int Decode_Key_64(const unsigned char* ciphertext,unsigned int length,KeyType& key);
	void UrlSafe(Octets & text);
	int CheckKeyTimeout();
	bool IsTimeout(int now,int t_limit){return now>(t_limit);/*2 minutes*/}
};

};

#endif
