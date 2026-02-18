#ifndef __ONLINEGAME_GS_ITEM_DATA_H__
#define __ONLINEGAME_GS_ITEM_DATA_H__
#include <common/packetwrapper.h>

struct item_data 
{
	int type;		//类型
	size_t count;		//数量
	size_t pile_limit;	//堆叠限制
	int proc_type;		//物品的处理方式
	struct 
	{
		int guid1;
		int guid2;
	}guid;			//GUID
	size_t price;		//单价 这个单价作为一个参考值,实际值以模板中的为准
	int expire_date;	//期限 等于0表示无限制
	size_t content_length;	//额外数据的大小
	char * item_content;	//额外数据 供item对象使用

	bool CanMoveToPocket() const;
};

struct item_data_client
{
	int type;		//类型
	size_t count;		//数量
	size_t pile_limit;	//堆叠限制
	int proc_type;		//物品的处理方式
	struct 
	{
		int guid1;
		int guid2;
	}guid;			//GUID
	size_t price;		//单价 这个单价作为一个参考值,实际值以模板中的为准
	int expire_date;	//期限 等于0表示无限制
	size_t content_length;	//额外数据的大小
	char * item_content;	//额外数据 供item对象使用
	bool use_wrapper;
	packet_wrapper ar;

	item_data_client()
	{
		use_wrapper = false;
	}
};

#endif

