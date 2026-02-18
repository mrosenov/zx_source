/*
 * FILE: globaldataman.cpp
 *
 * DESCRIPTION: global data loader and manager
 *
 * CREATED BY: Hedi, 2005/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include <vector.h>
#include "globaldataman.h"
#include <string.h>

static abase::vector<TRANS_TARGET_SERV>	global_trans_targets_server;
static abase::vector<MALL_ITEM_SERV>  global_mall_item_service;
static abase::vector<MALL_ITEM_SERV>  global_bonusmall_item_service;
static abase::vector<MALL_ITEM_SERV>  global_zonemall_item_service;
static abase::vector<VIP_AWARD_ITEM_SERV> global_vip_award_service;
static int mall_timestamp = 0;
static int bonusmall_timestamp = 0;
static int zonemall_timestamp = 0;
static int vipaward_timestamp = 0;

abase::vector<TRANS_TARGET_SERV> & globaldata_gettranstargetsserver()
{
	return global_trans_targets_server;
}

static bool load_transdata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	int nCount;
	fread(&nCount, 1, sizeof(int), fpServer);

	// now output a trans target list for server usage
	for(int i=0; i<nCount; i++)
	{
		TRANS_TARGET_SERV target;
		fread(&target, 1, sizeof(target), fpServer);
		global_trans_targets_server.push_back(target);
	}

	fclose(fpServer);
	return true;
}

#pragma pack(push, GSHOP_ITEM_PACK, 1)
typedef struct _GSHOP_ITEM
{
	unsigned int			id;				// object id of this item
	unsigned int			num;				// number of objects in this item
	char					icon[128];		// icon file path

	struct {
		unsigned int			price;			// price of this item
		unsigned int			time;			// time of duration, 0 means forever
	} list[1];

	int				discount;			//打折比例
	int				bonus;
	unsigned int			props;				// mask of all props, currently from low bit to high bit: 新品、推荐品、促销品
									// 最高8位分别表示打折方案，从低到高分别是1-8种方案
									// 次高8位分别表示出售方案，从低到高分别是1-8种方案

	int					main_type;		// index into the main type array
	int						sub_type;	// index into the sub type arrray
	int						local_id;	// id of this shop item, used only for localization purpose

	unsigned short 			desc[512];			// simple description
	unsigned short			szName[32];			// name of this item to show

	//新加赠品
	bool		 		bHasPresent;				//是否有赠品	
	unsigned short 			szPresentName[32];			//赠品名字
	unsigned int 			Presentid;				//赠品ID
	unsigned int 			Presentcount;				//赠品个数
	unsigned int 			Presenttime;				//赠品存在的时限(秒)
	char		 		Present_file_icon[128];			//赠品图标路径
	bool		 		bPresentBind;				//赠品是否绑定
	unsigned short 			Presentdesc[512];			//赠品文字描述

	//商城物品的上架有效时间Added 2011-11-07.
	struct
	{
		int		type;				//有效时间的类别 0:永久有效，默认类别；1：按时间段；2：每周；3：每月
		int		start_time;			//开始时间：按时间段时：存储UTC的秒数，按每月和每周时：存储（小时+分+秒）的秒数
		int		end_time;			//终止时间：存储方案同上
		int		param;				//按时间段时：1：表示开启开始时间；2：表示开启终止时间
									//按每周时：（掩码）0-6位分别表示：周日 - 周六
									//按每月时：（掩码）1-31位分别表示：1-31日
	}valid_time;
	//Added end.

	unsigned short SearchKey[64];			//关键字，","分割
	
	_GSHOP_ITEM()
	{
		bHasPresent = false;
		memset(szPresentName,0,sizeof(short)*32);
		Presentid = 0;
		Presentcount = 0;
		Presenttime = 0;
		memset(Present_file_icon,0,128);
		bPresentBind = false;
		memset(Presentdesc,0,sizeof(short)*512);
		memset(&SearchKey, 0, sizeof(SearchKey));
	}
} GSHOP_ITEM;
#pragma pack(pop, GSHOP_ITEM_PACK)

//-----------------------------VIP奖励-----------------------------------------
#pragma pack(push, VIP_AWARD_ITEM_PACK, 1)
typedef struct _VIP_AWARD_ITEM
{
	unsigned int	tid;					//模板ID
	unsigned short	szName[64];				//所含物品的名称
	unsigned int	item_id;				//所含物品的模板id
	unsigned int	count;					//所含物品的个数
	unsigned int	award_type;				//奖励类型 0：普通奖励， 1：VIP奖励
	unsigned int	award_level;			//奖励级别 普通奖励的奖励级别根据玩家等级设定，VIP等级根据充值额度设定
	unsigned int	award_obtain_type;		//获取奖励的方式：0：每日领取，1：特殊领取
	int				expire_time;			//期限，奖品有单独的时限
}VIP_AWARD_ITEM;
#pragma pack(pop, VIP_AWARD_ITEM_PACK)
//-----------------------------------------------------------------------------


static bool load_vipaward_data(const char * pfile)
{
	FILE * fpServer = fopen(pfile, "rb");
	if(!fpServer) 
	{
		fprintf(stderr, "\n\n打开VIPAward.data文件失败！\n");
		return false;
	}

	bool bRst = true;

	try
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) 
		{
			throw -1;
		}

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) 
		{
			throw -2;
		}

		// now output a trans target list for server usage
		for(int i = 0; i < nCount; i++)
		{
			VIP_AWARD_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}

			VIP_AWARD_ITEM_SERV awards; 
			memset(&awards, 0, sizeof(awards));
			awards.award_id			= data.tid;
			awards.award_type		= data.award_type;
			awards.award_item_id	= data.item_id;
			awards.award_item_count	= data.count;
			awards.obtain_level		= data.award_level;
			awards.obtain_type		= data.award_obtain_type;
			awards.expire_time		= data.expire_time;

			global_vip_award_service.push_back(awards);
		}

		vipaward_timestamp = timestamp;
	}
	catch(...)
	{
		fprintf(stderr, "\n\nVIP奖励数据读入有误，检查gs.conf中的VipAwardData项！\n");
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}


static bool load_malldata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	bool bRst = true;
	try 
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) throw -1;

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) throw -2;

		// now output a trans target list for server usage
		for(int i=0; i<nCount; i++)
		{
			GSHOP_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}
			MALL_ITEM_SERV goods;
			memset(&goods, 0, sizeof(goods));
			goods.goods_id = data.id;
			goods.goods_count = data.num;
			goods.bonus = data.bonus;
			goods.props = data.props;
			goods.discount = data.discount;

			//赠品
			goods.has_present = data.bHasPresent;
			goods.present_bind = data.bPresentBind;
			goods.present_id = data.Presentid;
			goods.present_count = data.Presentcount;
			goods.present_time = data.Presenttime;

			//商城限时销售物品
			goods.type       = data.valid_time.type;
			goods.start_time = data.valid_time.start_time;
			goods.end_time   = data.valid_time.end_time;
			goods.param      = data.valid_time.param;

			for(size_t j = 0;j < 1; j ++)
			{
				goods.list[j].expire_time = data.list[j].time;
				goods.list[j].cash_need = data.list[j].price;
			}

			global_mall_item_service.push_back(goods);
		}

		mall_timestamp = timestamp;
	}
	catch(...)
	{
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}

static bool load_bonusmalldata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	bool bRst = true;
	try 
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) throw -1;

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) throw -2;

		// now output a trans target list for server usage
		for(int i=0; i<nCount; i++)
		{
			GSHOP_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}
			MALL_ITEM_SERV goods;
			memset(&goods, 0, sizeof(goods));
			goods.goods_id = data.id;
			goods.goods_count = data.num;
			goods.bonus = data.bonus;
			goods.props = data.props;
			goods.discount = data.discount;

			//赠品
			goods.has_present = data.bHasPresent;
			goods.present_bind = data.bPresentBind;
			goods.present_id = data.Presentid;
			goods.present_count = data.Presentcount;
			goods.present_time = data.Presenttime;

			for(size_t j = 0;j < 1; j ++)
			{
				goods.list[j].expire_time = data.list[j].time;
				goods.list[j].cash_need = data.list[j].price;
			}

			global_bonusmall_item_service.push_back(goods);
		}

		bonusmall_timestamp = timestamp;
	}
	catch(...)
	{
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}

static bool load_zonemalldata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	bool bRst = true;
	try 
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) throw -1;

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) throw -2;

		// now output a trans target list for server usage
		for(int i=0; i<nCount; i++)
		{
			GSHOP_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}
			MALL_ITEM_SERV goods;
			memset(&goods, 0, sizeof(goods));
			goods.goods_id = data.id;
			goods.goods_count = data.num;
			goods.bonus = data.bonus;
			goods.props = data.props;
			goods.discount = data.discount;

			//赠品
			goods.has_present = data.bHasPresent;
			goods.present_bind = data.bPresentBind;
			goods.present_id = data.Presentid;
			goods.present_count = data.Presentcount;
			goods.present_time = data.Presenttime;

			for(size_t j = 0;j < 1; j ++)
			{
				goods.list[j].expire_time = data.list[j].time;
				goods.list[j].cash_need = data.list[j].price;
			}

			global_zonemall_item_service.push_back(goods);
		}

		zonemall_timestamp = timestamp;
	}
	catch(...)
	{
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}


bool globaldata_loadserver(const char * file, const char * file2, const char * file3, const char * file4, const char * file5)
{
	if(!load_transdata(file)) return false;
	if(!load_malldata(file2)) return false;
	if(!load_bonusmalldata(file3)) return false;
	if(!load_zonemalldata(file4)) return false;
	return load_vipaward_data(file5);
}

abase::vector<VIP_AWARD_ITEM_SERV> & globaldata_getvipawardservice()
{
	return global_vip_award_service;
}

int globaldata_getvipawardtimestamp()
{
	return vipaward_timestamp;
}

abase::vector<MALL_ITEM_SERV> & globaldata_getmallitemservice()
{
	return global_mall_item_service;
}

int globaldata_getmalltimestamp()
{
	return mall_timestamp;
}


abase::vector<MALL_ITEM_SERV> & globaldata_getbonusmallitemservice()
{
	return global_bonusmall_item_service;
}

int globaldata_getbonusmalltimestamp()
{
	return bonusmall_timestamp;
}

abase::vector<MALL_ITEM_SERV> & globaldata_getzonemallitemservice()
{
	return global_zonemall_item_service;
}

int globaldata_getzonemalltimestamp()
{
	return zonemall_timestamp;
}


bool globaldata_releaseserver()
{
	global_trans_targets_server.erase(global_trans_targets_server.begin(), global_trans_targets_server.end());

	return true;
}

