
#ifndef __GNET_ADDICTIONCONTROL_HPP
#define __GNET_ADDICTIONCONTROL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gpair"

void wallow_control(int userid, int rate, int msg, std::map<int,int> & data);
namespace GNET
{

class AddictionControl : public GNET::Protocol
{
	#include "addictioncontrol"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// rate:     0-正常状态, 1-半沉迷, 2-沉迷
		// data.key: 1-累计在线时间， 2-进入半沉迷状态时间点,  3-进入沉迷状态
		// msg:      0-注册帐号填了身份证号，且身份证号标识了是未成年人。
		//           1-注册帐号走的是快速注册，没有填身份证号。
		std::map<int,int> mm;
		for(size_t i = 0; i < data.size(); i ++)
		{
			mm[data[i].key] = data[i].value;
		}

		wallow_control(userid, rate, msg, mm);
	}
};

};

#endif
