#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

struct SClientBaseData
{
	std::string addr;
	std::string port;
	std::string name;
	std::string pwd;
};

static std::vector<SClientBaseData> g_IpVec;

int client_run()
{
	char* para[10];

	for(unsigned int i=0;i<g_IpVec.size();i++)
	{
		para[0]="gclient";
		para[1]="gclient.conf";
		para[2]=(char*)g_IpVec[i].name.c_str();
		para[3]=(char*)g_IpVec[i].pwd.c_str();
		para[4]="1";
		para[5]="a";
		para[6]=(char*)g_IpVec[i].addr.c_str();
		para[7]=(char*)g_IpVec[i].port.c_str();
		para[8]="server.log&";//end
		para[9]=0;
		pid_t pid=fork();
		if(pid>0)
		{
			execv("loginner_gclient",para);
		}
		else if(pid<0)
		{
			printf("fork err");
			exit(-1);
		}
	}
	return 0;	
}

bool PreLoadTable(const char* filepath)
{      
	FILE * file = fopen(filepath, "rb");
	if(file == NULL) return false;
	char buf[256];
	while(fgets(buf,sizeof(buf), file))
	{
		char addr[50];
		char port[10];
		char name[100];
		char pwd[20];
		memset(addr, 0, sizeof(addr));
		memset(port, 0, sizeof(port));
		memset(name, 0, sizeof(name));
		memset(pwd, 0, sizeof(pwd));
		sscanf(buf, "%s %s %s %s", addr, port, name, pwd);
		if(addr[0] == '#') continue;
		SClientBaseData data;
		data.addr = std::string(addr);
		data.port = std::string(port);
		data.name = std::string(name);
		data.pwd = std::string(pwd);
		if(data.addr.empty() || data.port.empty())
		{
			continue;
		}

		g_IpVec.push_back(data);
	}
	fclose(file);

	return true;
}

int main(int argc,char* argv[])
{
	char* path = "serverlist.txt";
	if(!PreLoadTable(path))
	{
		printf("can not read file :%s\n", path);
		exit(-1);
	}
	client_run();
	return 0;
}
