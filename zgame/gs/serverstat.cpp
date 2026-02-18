#include "worldmanager.h"
#include <glog.h>

/*
 *	整个服务器参数的设置
 */

namespace GNET
{
//多倍经验，上限是6倍，0或1表示没有开启多倍经验
void SetMultipleExp(unsigned char multiple)
{
	GLog::log(GLOG_INFO,"GM:服务器%d的多倍经验开关为(%s), 所开倍数为:%d",
			gmatrix::GetServerIndex(),
			(1 != multiple || 0 != multiple)?"On":"Off",
			multiple);

	int temp_multiple = multiple;
	if(temp_multiple >= 0 && temp_multiple <= 6)
	{
		gmatrix::GetWorldParam().set_multiple_exp(multiple);	
	}
	else
	{
		GLog::log(GLOG_ERR, "GM:服务器%d的多倍经验数据有误，倍数为%d", 
				gmatrix::GetServerIndex(), multiple);
	}
}

void SetNoTrade(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d交易开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_trade = blOn;	
}

void SetNoMail(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d邮件开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_mail = blOn;	
}

void SetNoAuction(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d拍卖开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_auction = blOn;	
}

void SetNoFaction(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d帮派开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_faction = blOn;	
}

void SetDoubleMoney(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d双倍金钱开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"On":"Off");
	gmatrix::GetWorldParam().double_money = blOn;	
}

void SetDoubleObject(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d双倍掉率开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"On":"Off");
	gmatrix::GetWorldParam().double_drop = blOn;	
}


void SetDoubleSP(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d双倍元神开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"On":"Off");
	gmatrix::GetWorldParam().double_sp = blOn;	
}

void SetNoSellPoint(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:服务器%d禁止点卡交易开关为(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_cash_trade = blOn;	

}
}

