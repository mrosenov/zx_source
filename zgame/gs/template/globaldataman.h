/*
 * FILE: globaldataman.h
 *
 * DESCRIPTION: global data loader and manager
 *
 * CREATED BY: Hedi, 2005/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef _GLOBALDATAMAN_H_
#define _GLOBALDATAMAN_H_

#include <vector.h>
#include <a3dvector.h>

typedef struct _TRANS_TARGET_SERV
{
	int		id;
	int 		world_tag;
	A3DVECTOR3	vecPos;
	int		domain_id;
} TRANS_TARGET_SERV;

typedef struct _MALL_ITEM_SERV
{
	int	goods_id;
	int	goods_count;
	int 	bonus;
	int 	props;
	int 	discount;
	bool 	has_present;
	bool 	present_bind;
	unsigned int present_id;
	unsigned int present_count;
	unsigned int present_time;

	//-------------------商城限时销售物品-------------------------
	int	type;				//有效时间的类别 0:永久有效，默认类别；1：按时间段；2：每周；3：每月
	int	start_time;			//开始时间：按时间段时：存储UTC的秒数，按每月和每周时：存储（小时+分+秒）的秒数
	int	end_time;			//终止时间：存储方案同上
	int	param;				//按时间段时：1：表示开启开始时间；2：表示开启终止时间
							//按每周时：（掩码）0-6位分别表示：周日 - 周六
							//按每月时：（掩码）1-31位分别表示：1-31日
	
	struct __entry
	{
		int 	expire_time;
		int 	cash_need;
	}list[4];
} MALL_ITEM_SERV;

//VIP奖励
typedef struct _VIP_AWARD_ITEM_SERV
{
	int		award_id;			//编辑器里每个奖励物品对应的id，此id是唯一的，单增的
	short	award_type;			//奖品类型：0是普通，1是vip

	int		award_item_id;		//每个奖励的具体物品id，用于进包裹时声称物品
								//可能重复(送5个飞天符和送10个飞天符是不同的奖励)
	int		award_item_count;	//物品数量

	short	obtain_level;		//获取奖品所需的档次，vip档次从1开始，普通玩家档次根据等级分级，从1开始
	short	obtain_type;		//获取奖品的类型：0是每日领取，1是特殊领取

	int		expire_time;		//时限，奖品有单独的时限
}VIP_AWARD_ITEM_SERV;


bool globaldata_loadserver(const char * trans_data,const char * mall_data, const char * bonusmall_data, const char * zonemall_data, const char * vipaward_data);
bool globaldata_releaseserver();

abase::vector<TRANS_TARGET_SERV> & globaldata_gettranstargetsserver();
abase::vector<MALL_ITEM_SERV> & globaldata_getmallitemservice();
int globaldata_getmalltimestamp();

abase::vector<MALL_ITEM_SERV> & globaldata_getbonusmallitemservice();
int globaldata_getbonusmalltimestamp();

abase::vector<MALL_ITEM_SERV> & globaldata_getzonemallitemservice();
int globaldata_getzonemalltimestamp();

abase::vector<VIP_AWARD_ITEM_SERV> & globaldata_getvipawardservice();
int globaldata_getvipawardtimestamp();

#endif//_GLOBALDATAMAN_H_

