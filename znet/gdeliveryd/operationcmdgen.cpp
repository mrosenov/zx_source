#include "../common/octets.h"
#include "../common/marshal.h"
#include <string>
#include <stdio.h>
#include "base64.h"
#include "security.h"

using namespace GNET;

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Usage:\t %s -e plain_text [timestamp]\n", argv[0]);
		printf("\t %s -d encrypt_text\n", argv[0]);
		return 1;
	}
	bool encrypt = false;
	if (strncmp(argv[1], "-e", 2) == 0)
		encrypt = true;
	else if (strncmp(argv[1], "-d", 2) == 0)
		encrypt = false;
	else
		return 1;

	std::string text(argv[2]);
	//printf("string %s size %d\n", text.c_str(), text.size());
	int timestamp = 0;
	Octets code(text.c_str(), text.size());
	Octets out;
	Marshal::OctetsStream os;
	if (encrypt)
	{
		if (argc == 4)
			timestamp = atoi(argv[3]);
		else
			timestamp = time(NULL);
		Octets digest;
		MD5Hash hash;
		hash.Update(code);
		hash.Final(digest);
		os << timestamp << code << digest;
		Base64Encoder::Convert(out, os);
	}
	else
	{
		Octets digest, digest2;
		Base64Decoder::Convert(os, code);
		printf("os.size %d\n", os.size());
		try 
		{
			os >> timestamp >> out >> digest;
		}
		catch(...)
		{
			printf("unmarshal error\n");
		}
		if (out.size()==0 || digest.size()==0)
			printf("err:out.size %d digest.size %d", out.size(), digest.size());
		MD5Hash hash;
		hash.Update(out);
		hash.Final(digest2);
		printf("out.size %d\n", out.size());
		printf("digest.size %d\n", digest.size());
		if (digest != digest2)
			printf("digest not equal\n");
	}
//	printf("timestamp:%d cmd:%.*s\n", timestamp, out.size(), (char *)out.begin());
	printf("%.*s", out.size(), (char*)out.begin());
}
//#endif
