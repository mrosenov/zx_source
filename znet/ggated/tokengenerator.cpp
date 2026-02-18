#include "tokengenerator.h"
#include "kdsclient.hpp"
#include <openssl/md5.h>
#include <sstream>
#include "io/security.h"
#include "base64.h"
#include "keysreq.hpp"
#include "kdskeepalive.hpp"

namespace GNET
{
//把tbcd编码数据转换为字符串（允许有字符,高四位为每字节第一个数字）
int trans_tbcd16_r(char *pstr,const unsigned char *pbcd, int len)
{
	int i,ii;
	int ntemp;
	if((pstr == NULL)||(pbcd == NULL))
		return -1;
	for(i = 0, ii = 0; i < len; i++)
	{
	  	ntemp = pbcd[i];
		ntemp = (ntemp>>4);

	  	if( ntemp <= 9)
		    	ntemp += 0x30;
	 	else
			ntemp += 87;
	
		pstr[ii] = ntemp;
		ii++;

		ntemp = pbcd[i];
		ntemp &= 0x0F;
          
	  	if( ntemp <= 9)
			ntemp += 0x30;
	 	else
			ntemp += 87;
                
		pstr[ii] = ntemp;
		ii++;
	}
	pstr[ii] = 0;
	return ii;
}
/*
void Char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'a' + byte[i] - 10;
	}
	szHex[2] = 0;
}

void CharStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}
*/
/*
bool AsciiHex2Binary(const unsigned char * str,unsigned char * out,int size)
{
	char tmp[3]={0};
	int byte;
	for(int i=0;i<size;i++)
	{
		memcpy(tmp,str+i*2,2);
		sscanf(tmp,"%x",&byte);
		*(out+i)=byte;
	}
	return true;

}*/

inline unsigned char get_num_value(unsigned char ascii)
{
	if(ascii>='0' && ascii <='9')
		return ascii-0x30;
	else if(ascii>='a' && ascii <='f')
		return ascii-87;
	DEBUG_PRINT("get_num_value from ascii chars error, ascii=%d",ascii);
	return 0;
}

int trans_tbcd16(const unsigned char *pstr, unsigned char *pbcd,int len)
{
	int i,ii;
	int ntemp;
	if((pstr == NULL)||(pbcd == NULL) )
		return -1;
	for(i = 0,ii=0; i < len*2; i+=2,ii++)
	{
	  	ntemp = 16*get_num_value(pstr[i]);
		ntemp += get_num_value(pstr[i+1]);
		pbcd[ii]=ntemp;
	}
	return ii;
}

void TokenGenerator::Openssl_encoder::set_key(const Key_128& newkey,int64_t kid)
{
	AES_set_encrypt_key(newkey.key_array, AESKEYBITS, &key);
	std::stringstream ss;
	kid_ = kid;
	ss<<kid<<"-";
	ss>>keyid_pre;
	inited=true;
}

void TokenGenerator::Openssl_decoder::set_key(const Key_128& newkey)
{
	AES_set_decrypt_key(newkey.key_array, AESKEYBITS, &key);
	inited=true;
}

void TokenGenerator::Initialize(const string& sharekey,const string& kcname,int kcactiveid)
{
	if(sharekey.size()!=AESKEYSIZE)
	{
		Log::log(LOG_ERR, "AES sharekey size = %d err",sharekey.size());
		return;
	}
	kc_name.insert(kc_name.begin(), kcname.c_str(),kcname.size());
	kc_activeId = kcactiveid;
	SetKCInit(true);
	Key_128 tmp(sharekey);
	KeyDecoderSetKey(tmp);
	IntervalTimer::Attach(this,UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());
	return;
}

int TokenGenerator::Decode_Key_64(const unsigned char* ciphertext,unsigned int length,KeyType& key)
{
	if(length!=64)
		return -1;
	unsigned char keystr[32];
//	unsigned char keystr1[32];
//	AsciiHex2Binary(ciphertext,keystr,32);

	trans_tbcd16(ciphertext,keystr,32);
//	int i=memcmp(keystr,keystr1,32);
//	DEBUG_PRINT("AsciiHex2Binary  trans_tbcd16 cmp=%d",i);

	if(key_decoder.Decrypt(keystr,key.key_array,AESBLOCKSIZE))
		return -1;

	char output[33];
//	CharStr2HexStr(key.key_array,output,16);
//	output[32]=0;
//      DEBUG_PRINT("encoded key =%s,Decode Key=%s",ciphertext, output);	
	trans_tbcd16_r(output,key.key_array,16);
        DEBUG_PRINT("encoded key =%s,Decode Key=%s cmp",ciphertext, output);	
	return 0;
}

int TokenGenerator::Decode_Key(const unsigned char* ciphertext,unsigned int length,KeyType& key)
{
	if(length!=ENCODED_KEY_SIZE)
		return -1;
	unsigned char keystr[ENCODED_KEY_SIZE];
	memcpy(keystr,ciphertext,ENCODED_KEY_SIZE);
	if(key_decoder.Decrypt(keystr,key.key_array,AESBLOCKSIZE))
		return -1;

	char output[33];
	char cipheroutput[65];
	trans_tbcd16_r(output,key.key_array,16);
	trans_tbcd16_r(cipheroutput,ciphertext,32);
        DEBUG_PRINT("encoded key =%s,Decode Key=%s",cipheroutput, output);	
	return 0;
}

int TokenGenerator::BuildToken(const string & profile,const Octets & rolename,const string & profile1,string& token)
{
	if(!IsEncoderInit())
		return -1;
	unsigned char *plaintext,*ciphertext;
       	int length = Padding(profile,rolename,profile1,plaintext);
	ciphertext = new unsigned char[length];

	if(encoder.Encrypt(plaintext,ciphertext,length))
		return -1;

	unsigned char md5_buffer[MD5SIZE];
	MD5(ciphertext,length,md5_buffer);

	//calc real base64
	Octets ct_base64,tmp;
	tmp.insert(tmp.end(), ciphertext, length);
	tmp.insert(tmp.end(), md5_buffer, MD5SIZE);
	Base64Encoder::Convert(ct_base64,tmp);
	UrlSafe(ct_base64);
	token.append(encoder.GetKeyid());
	token.append((char*)ct_base64.begin(),ct_base64.size());
//	DEBUG_PRINT("Token=%s", token.c_str()); 
	delete[] plaintext;
	delete[] ciphertext;
//	delete[] tokenstr;
	return 0;
}

int TokenGenerator::BuildToken(const string & profile,string& token)
{
	if(!IsEncoderInit())
		return -1;
	unsigned char *plaintext,*ciphertext;
       	int length = Padding(profile,plaintext);
	ciphertext = new unsigned char[length];

	if(encoder.Encrypt(plaintext,ciphertext,length))
		return -1;

	unsigned char md5_buffer[MD5SIZE];
	MD5(ciphertext,length,md5_buffer);
/*	Octets ctmd5,tmp;
	tmp.insert(tmp.end(), ciphertext, length);

	MD5Hash md5;
        md5.Update(tmp);
        md5.Final(ctmd5);
	tmp.insert(tmp.end(), ctmd5.begin(), ctmd5.size());*/
//        Octets ct_base64;
//        Base64Encoder::Convert(ct_base64,tmp);

/* fake base64 */
/*
	char* tokenstr =new char[(length+MD5SIZE)*2+1];
//calc fake base64, old func
//	CharStr2HexStr(ciphertext,tokenstr,length);
//	CharStr2HexStr(md5_buffer,tokenstr+length*2,16);
//	tokenstr[(length+MD5SIZE)*2]=0;
//	DEBUG_PRINT("tokenstr=%s", tokenstr);

//calc fake base64, new func
	trans_tbcd16_r(tokenstr,ciphertext,length);
	trans_tbcd16_r(tokenstr+length*2,md5_buffer,16);
//	DEBUG_PRINT("tokenstr=%s cmp", tokenstr);

//	token.append(encoder.GetKeyid());
//	token.append(tokenstr);
	DEBUG_PRINT("no base64 Token=%s", tokenstr); 
*/

	//calc real base64
	Octets ct_base64,tmp;
	tmp.insert(tmp.end(), ciphertext, length);
	tmp.insert(tmp.end(), md5_buffer, MD5SIZE);
	Base64Encoder::Convert(ct_base64,tmp);
	UrlSafe(ct_base64);
	token.append(encoder.GetKeyid());
	token.append((char*)ct_base64.begin(),ct_base64.size());
//	DEBUG_PRINT("Token=%s", token.c_str()); 
	delete[] plaintext;
	delete[] ciphertext;
//	delete[] tokenstr;
	return 0;
}

void TokenGenerator::UrlSafe(Octets & text)
{
	char* p = (char*)text.begin();
	for(unsigned int i=0;i<text.size();i++)
	{
		if(p[i]=='+')
			p[i]='-';
		else if(p[i]=='/')
			p[i]='_';
	}
	for(int i=0;i<2;i++)
	{
		int j=text.size();
		if(j>0 && p[j-1]=='=')
			text.resize(j-1);
	}
}

int TokenGenerator::Padding(const string & profile,unsigned char* &plaintext)
{
	int size = profile.size();
	if(size==0)
		return 0;

	int fullsize = (((size)/(AESBLOCKSIZE)) +1)*(AESBLOCKSIZE);//not use size-1
	plaintext = new unsigned char[fullsize];
	memcpy(plaintext,profile.c_str(),size);

	int padding_len = fullsize - size;
	if(padding_len>0)
		memset(plaintext+size,padding_len,padding_len);//PKCS5
	return fullsize;
}

int TokenGenerator::Padding(const string & profile,const Octets & rolename,const string & profile1,unsigned char* &plaintext)
{
	int size = profile.size() + rolename.size() + profile1.size();
	if(size==0)
		return 0;

	int fullsize = (((size)/(AESBLOCKSIZE)) +1)*(AESBLOCKSIZE);//not use size-1
	plaintext = new unsigned char[fullsize];
	memcpy(plaintext,profile.c_str(),profile.size());
	memcpy(plaintext+profile.size(),rolename.begin(),rolename.size());
	memcpy(plaintext+profile.size()+rolename.size(),profile1.c_str(),profile1.size());

	int padding_len = fullsize - size;
	if(padding_len>0)
		memset(plaintext+size,padding_len,padding_len);//PKCS5
	return fullsize;
}

void TokenGenerator::CheckOldkeys(std::map<int64_t,CustomKey> & keylist)
{
	std::map<int64_t,CustomKey>::iterator kit;
	KeyMap::iterator it,ite=keys.end();
	for(it=keys.begin();it!=ite;)
	{
		kit = keylist.find(it->first);
		if(keylist.end()!=kit)
		{
			if(!it->second.IsEncodedEqual(kit->second.encoded))
			{
				DEBUG_PRINT("TokenGenerator::CheckOldkeys same keyid with diff encodedkey");
				keys.erase(it++);//delete old key when get same keyid with diff encodedkey
			}
			else
			{
				++it;
				keylist.erase(kit);//delete repeat key in new key list
			}
		}
		else
		{
			++it;//do not delete old keys here, excessive keys will be deleted after add new key
		}
	}
	return;
}

int TokenGenerator::AddKey(int64_t keyid,Octets& encoded,int overdue_time)
{
	if(encoded.size()!=ENCODED_KEY_SIZE)
		return -1;
	KeyType key;
        if(Decode_Key((unsigned char*)encoded.begin(),ENCODED_KEY_SIZE,key))
		return -1;
        KeyNodeType newkey(encoded,key,overdue_time);
	if(keys.insert(std::make_pair(keyid,newkey)).second == false)
		return -1;
	DEBUG_PRINT("TokenGenerator::AddKey kid=%lld,timeout=%d",keyid,overdue_time);
	return 0;
}

int TokenGenerator::CheckKeyTimeout()
{
	KeyMap::iterator it,ite=keys.end();
	int now = Timer::GetTime();
	bool set_new_key = false;
	for(it=keys.begin();it!=ite;)
	{
		if(it->second.IsTimeout(now))
		{
			DEBUG_PRINT("TokenGenerator::CheckKeyTimeout erase kid=%lld,time_limit=%d,now=%d keylist=%d",it->first,it->second.GetTimeLimit(),now,keys.size());
			if(it->first == EncoderGetKeyid())
				set_new_key = true;
			keys.erase(it++);
		}
		else
			++it;
	}
	if(set_new_key && keys.size()>0)
		EncoderSetKey(keys.begin()->second.GetKey(),keys.begin()->first);

	return 0;
}

int TokenGenerator::OnGetNewKeys(std::map<int64_t,CustomKey> & keylist,int64_t kds_time)
{
	DEBUG_PRINT("TokenGenerator::OnGetNewKeys");
	if(!IsKeyDecoderInit())
		return -1;
	int tmp = kds_time/1000;
	int now = Timer::GetTime();
	int time_delta = now - tmp;
//	DEBUG_PRINT("TokenGenerator::SetKey old key size=%d,newkey list size=%d",keys.size(),keylist.size());
	CheckOldkeys(keylist);
//	DEBUG_PRINT("TokenGenerator::SetKey add newkey number=%d",keylist.size());
	std::map<int64_t,CustomKey>::iterator kit,kite=keylist.end();
	for(kit=keylist.begin();kit!=kite;++kit)
	{
		int t = kit->second.timeout/1000;
//		DEBUG_PRINT("TokenGenerator::AddKey kdsserver=%d,orig timeout=%d,delta=%d",tmp,t,time_delta);
		if(IsTimeout(now,t + time_delta))
			continue;
		if(AddKey(kit->first,kit->second.encoded,t + time_delta))
			DEBUG_PRINT("TokenGenerator::SetKey AddKey err");
	}
	CheckKeyTimeout();
	while(keys.size()>KEY_CACHE_SIZE)
	{
		DEBUG_PRINT("TokenGenerator::erase too much key kid=%lld,timeout=%d,now=%d",keys.begin()->first,keys.begin()->second.GetTimeLimit(),now);
		keys.erase(keys.begin());
	}
	if(keys.size()>0)
		EncoderSetKey(keys.begin()->second.GetKey(),keys.begin()->first);
	return 0;
}

int TokenGenerator::GetNewKey()
{
	if(!IsKCInit())
		return -1;
	KeysReq req;
	req.kc_name = kc_name;
	req.kc_activeId = kc_activeId;
	KdsClient::GetInstance()->SendProtocol(req);
	OnKDSKeepAlive();//set initial ttl
	return 0;
}

void TokenGenerator::CheckConnection()
{
	connection_ttl--;
//	DEBUG_PRINT("TokenGenerator::ttl=%d", connection_ttl);
	if(connection_ttl<=0)
	{
		KdsClient::GetInstance()->KdsClose();
//		DEBUG_PRINT("TokenGenerator::ttl=0 close connection");
	}
	ticks+=UPDATE_INTERVAL;
	if(ticks==KEEPALIVE_INTERVAL)
	{
		CheckKeyTimeout();
		ticks = 0;
		KDSKeepAlive msg((char)0);
		KdsClient::GetInstance()->SendProtocol(msg);
	}
}

};

