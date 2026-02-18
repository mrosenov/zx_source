
#ifndef __GNET_WEBORDERNOTICE_HPP
#define __GNET_WEBORDERNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "weborder"
#include "gsp_if.h"

void add_web_order(int roleid, std::vector<GMSV::weborder> & list);
namespace GNET
{

class WebOrderNotice : public GNET::Protocol
{
	#include "webordernotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		std::vector<GMSV::weborder> list;
		WebOrderVector & db = weborders; 

		if (db.size() == 0)
			return;
		WebOrderVector::const_iterator it, ite = db.end();
		for (it = db.begin(); it != ite; ++it)
		{
			GMSV::weborder order;
			order.orderid = it->orderid;
			order.userid = it->userid;
			order.roleid = it->roleid;
			order.paytype = it->paytype;
			order.status = it->status;
			order.timestamp = it->timestamp;

			WebMallFunctionVector::const_iterator fit, fite = it->functions.end();
			for (fit = it->functions.begin(); fit != fite; ++fit)
			{
				GMSV::webmallfunc func;
				func.id = fit->function_id;
				func.name = std::string((char*)fit->name.begin(), fit->name.size());
				func.count = fit->count;
				func.price = fit->price;

				WebMallGoodsVector::const_iterator git, gite = fit->goods.end();
				for (git = fit->goods.begin(); git != gite; ++git)
				{
					GMSV::webmallgoods goods;
					goods.id = git->goods_id;
					goods.count = git->count;
					goods.flagmask = git->flagmask;
					goods.timelimit = git->timelimit;
					func.goods.push_back(goods);
				}
				order.funcs.push_back(func);
			}
			list.push_back(order);
		}

		add_web_order(roleid, list);
	}
};

};

#endif
