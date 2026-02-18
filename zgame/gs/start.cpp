#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <threadpool.h>
#include "start.h"
#include <gsp_if.h>
#include <glog.h>
#include <amemory.h>
#include "worldmanager.h"
#include "arandomgen.h"
#include "world.h"
#include <malloc.h>
#include <hashmap.h>
#include "netmsg.h"

const size_t  CSIZE = 65536 ;
static int size_counter[CSIZE] = {0};
static int large_counter = 0;
static int total_mem_size = 0;

static void AllocSize(size_t n)
{
	interlocked_add(&total_mem_size,n);
	if(n <CSIZE)
	{
		interlocked_add(&size_counter[n],1);
	}
	else
	{
		interlocked_add(&large_counter,1);
	}
}

static void DeleteSize(size_t n)
{
	interlocked_add(&total_mem_size,-(int)n);
	if(n <CSIZE)
	{
		interlocked_add(&size_counter[n],-1);
	}
	else
	{
		interlocked_add(&large_counter,-1);
	}
}
#ifndef __DO_NOT_CHECK_MEM__
//static bool no_record = true;
void * operator new[](size_t n)
{
	void * rst = abase::fast_allocator::raw_alloc(n + 4);
	*(size_t*) rst = n;
	AllocSize(n);
	return ((char*)rst) + 4;
}

void * operator new(size_t n)
{
	void * rst = abase::fast_allocator::raw_alloc(n + 4);
	*(size_t*) rst = n;
	AllocSize(n);
	return ((char*)rst) + 4;
}

void operator delete(void * p, size_t n)
{
	if(!p) return;
	void * xp = ((char*)p) -4;
	size_t size = *(size_t*)xp;
	DeleteSize(size);
	abase::fast_allocator::raw_free(xp);
//	free(p);
//	if(!no_record) fprintf(stderr,"free_ptr   %p  caller %p\n",p,*(&n-1));
}

void operator delete(void * p)
{
	if(!p) return;
	void * xp = ((char*)p) -4;
	size_t size = *(size_t*)xp;
	DeleteSize(size);
	abase::fast_allocator::raw_free(xp);
//	free(p);
//	if(!no_record) fprintf(stderr,"free_ptr   %p  caller %p\n",p,*(&p-1));
}

void operator delete[](void * p)
{
	if(!p) return;
	void * xp = ((char*)p) -4;
	size_t size = *(size_t*)xp;
	DeleteSize(size);
	abase::fast_allocator::raw_free(xp);
//	free(p);
//	if(!no_record) fprintf(stderr,"free_ptr   %p  caller %p\n",p,*(&p-1));
}
#else
//static bool no_record = true;
void * operator new[](size_t n)
{
	return malloc(n);
}

void * operator new(size_t n)
{
	return malloc(n);
}

void operator delete(void * p, size_t n)
{
	free(p);
}

void operator delete(void * p)
{
	free(p);
}

void operator delete[](void * p)
{
	free(p);
}
#endif

//#include <mcheck.h>

int InitWorld(const char *, const char *,const char*);
namespace ONET{ extern int ONET::__thread_lock_count;}
int main(int argn , char ** argv)
{

	if(system("/bin/touch foo"))
	{
		printf("文件系统不可写，无法进行后继的初始化操作\n");
		return -1;
	}
	const char * conf_file = "gs.conf";
	const char * gmconf_file = "gmserver.conf";
	const char * alias_conf = "gsalias.conf";
	if(argn >1) conf_file = argv[1];
	if(argn >2) gmconf_file = argv[2];
	if(argn >3) alias_conf = argv[3];

//	setenv("MALLOC_TRACE", "/tmp/m.log", 1);
//	mtrace();
	if(int rst = InitWorld(conf_file,gmconf_file,alias_conf))
	{
		printf("初始化失败， 错误号：%d\n",rst);
		//停止定时器
		g_timer.stop_thread();
		usleep(100000);
		_exit(rst);
	}
	fflush(stdout);
	long header_offset = ftell(stdout);
	if(header_offset <=0 || header_offset > 0x500000) 
	{
		header_offset = 2048;
	}

	//启动thread
	ONET::Thread::Pool::CreatePool(4);
	ONET::Thread::Pool::CreateThread(GMSV::StartPollIO);
	int log_counter = 0;

//	InitHookTab();

	setlinebuf(stdout);
	sleep(1);
	while(1) 
	{
		GMSV::FetchWeeklyTop();
		GMSV::FetchDailyTop();
		sleep(40);
		log_counter += 40;
		if(log_counter > 240)
		{
			log_counter -= 240;
			GLog::log(GLOG_INFO,"gameserver %d , 用户已分配数目:%d/%d", 
					gmatrix::GetServerIndex(),gmatrix::GetPlayerAlloced(),total_login_player_number());
		}
		/*
		printf("gameserver %d , 用户已分配数目:%d\n", gmatrix::GetServerIndex(),gmatrix::GetPlayerAlloced());
		printf("gameserver %d , 用户历史下线次数:%d\n", gmatrix::GetServerIndex(),total_login_player_number());
		printf("gameserver %d , NPC已分配数目:%d\n", gmatrix::GetServerIndex(),gmatrix::GetNPCAlloced());
		printf("tick:%d, thread_lock_count(60sec):%d\n",g_timer.get_tick(),ONET::__thread_lock_count);
		printf("free timer count:%8d total alloced:%d\n",g_timer.get_free_timer_count(),g_timer.get_timer_total_alloced());
		printf("total threadpool task count:%d\n",ONET::Thread::Pool::GetTotalTaskCount());
		abase::fast_allocator::dump(stdout);
		printf("Large size memory block:\n");
		abase::fast_allocator::dump_large(stdout);
		printf("_________________________________________________________________________\n");
//		world * pPlane = world_manager::GetInstance()->GetWorldByIndex(0);
//		if(pPlane)
//		{
//			pPlane->DumpMessageCount();
//		}
		
//		printf("cur_player_count %d\n",world_manager::GetInstance()->GetOnlineUserNumber());
		printf("_________________________________________________________\n");
		printf("total_size %d\n",total_mem_size);
		printf("large counter:%d\n",large_counter);
		*/
		/*
		size_t index = 0;
		for(size_t i = 0; i < CSIZE; i ++)
		{
			if(size_counter[i])
			{
				index ++;
				printf("%5d:%8d  count %5d\n",index, i, size_counter[i]);
			}
		}*/
		long pos = ftell(stdout);
		if(pos >= 0)
		{
//			printf("cur_file_size %ld\n",pos);
			fflush(stdout);
			if(pos > 0x5FFFFFFF)
			{
				//考虑进行对stdout进行剪切
				//留出头部的日期数据等
				ftruncate(fileno(stdout),header_offset);
				fseek(stdout,header_offset,SEEK_SET);
			}
		}
		else
		{
			fflush(stdout);
		}

	}
	return 0;
}



