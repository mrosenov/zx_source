#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
char* get_user(int number,int base)
{
	static char username[9]="tj";
	username[2]=0;
	int n = strlen(username);
	snprintf(username+n,9-n,"%d",number+base);
	return username;
}
char* get_pass(int number)
{
	static char passwd[9]="123456";
	return passwd;
}

int client_create(int number,int base)
{
	char * para[7];
	for(int i=0;i<number;i++)
	{
		para[0]="gclient";
		para[1]="gclient.conf";
		para[2]=get_user(i,base);
		para[3]=get_pass(i);
		para[4]="1";
		para[5]="c";
		para[6]=0;//end
		pid_t pid=fork();
		if(pid>0)
		{
			execv("gclient",para);
		}
		else if(pid<0)
		{
			printf("fork err");
			exit(-1);
		}
	}
	return 0;	
}
int client_run(int number,int base,char* choice)
{
	char * para[7];
	for(int i=0;i<number;i++)
	{
		para[0]="gclient";
		para[1]="gclient.conf";
		para[2]=get_user(i,base);
		para[3]=get_pass(i);
		para[4]="1";
		para[5]=choice;
		para[6]=0;//end
		pid_t pid=fork();
		if(pid>0)
		{
			execv("gclient",para);
		}
		else if(pid<0)
		{
			printf("fork err");
			exit(-1);
		}
	}
	return 0;	
}

int main(int argc,char* argv[])
{
	printf("usage: windforce (number) (c/ac/aj) (usernamebase,e.g. tj50)\n");
	if(argc!=4)
	{
		printf("err argu num\n");
		return -1;
	}
	int num = atoi(argv[1]);
	if(num<1 || num>1000)
	{
		printf("err client num=%d\n",num);
		return -1;
	}
	int base = atoi(argv[3]+2);
	if(base<1 || base>100000)
	{
		printf("err base username=%s\n",argv[3]);
		return -1;
	}
	char choice=argv[2][0];
	switch(choice)
	{
	case 'c':
		client_create(num,base);
		break;
	case 'a':
		client_run(num,base,argv[2]);
		break;
	default:
		printf("err choice=%c\n",choice);
		break;
	}
	int status;
	while(wait(&status)>=0)
		;
	return 0;
}
