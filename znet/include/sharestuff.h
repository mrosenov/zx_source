#ifndef __GNET_SHARESTUFF_H
#define __GNET_SHARESTUFF_H

namespace GDB
{
	/*
struct ivec
{
	const void * data;
	unsigned int size;
};
struct webmallgoods
{
	int id;
	int count;
	int flagmask;
	int timelimit;
};
struct webmallfunc //礼包结构
{
	int id;
	ivec name;//礼包名称
	int count;
	int price;

	size_t goods_count;
	webmallgoods * goods;
};
struct weborder //订单结构
{
	int64_t orderid;
	int userid;
	int roleid;
	int paytype;
	int status;
	int timestamp;
	
	size_t func_count;
	webmallfunc * funcs;
};
struct weborderlist
{
	size_t count;
	weborder * list;
};

inline void get_buf(const GNET::Octets & os, ivec & vec)
{
	if((vec.size = os.size()))
	{
		vec.data = os.begin();
	}
	else
	{
		vec.data = NULL;
	}
}


inline void get_weborders(const WebOrderVector & db, weborderlist & list)
{
	assert(list.list == NULL && list.count == 0);
	if (db.size() == 0)
		return;
	list.count = db.size();
	list.list = (weborder *)malloc(sizeof(weborder) * list.count);
	memset(list.list, 0, sizeof(weborder)*list.count);
	weborder * porder = list.list;
	WebOrderVector::const_iterator it, ite = db.end();
	for (it = db.begin(); it != ite; ++it, porder++)
	{
		porder->orderid = it->orderid;
		porder->userid = it->userid;
		porder->roleid = it->roleid;
		porder->paytype = it->paytype;
		porder->status = it->status;
		porder->timestamp = it->timestamp;

		if (it->functions.size())
		{
			porder->func_count = it->functions.size();
			porder->funcs = (webmallfunc *)malloc(sizeof(webmallfunc)*porder->func_count);
			memset(porder->funcs, 0, sizeof(webmallfunc)*porder->func_count);
			webmallfunc * pfunc = porder->funcs;
			WebMallFunctionVector::const_iterator fit, fite = it->functions.end();
			for (fit = it->functions.begin(); fit != fite; ++fit, pfunc++)
			{
				pfunc->id = fit->function_id;
				get_buf(fit->name, pfunc->name);
				pfunc->count = fit->count;
				pfunc->price = fit->price;

				if (fit->goods.size())
				{
					pfunc->goods_count = fit->goods.size();
					pfunc->goods = (webmallgoods *)malloc(sizeof(webmallgoods)
							* pfunc->goods_count);
					memset(pfunc->goods, 0, sizeof(webmallgoods)*pfunc->goods_count);
					webmallgoods * pgoods = pfunc->goods;
					WebMallGoodsVector::const_iterator git, gite = fit->goods.end();
					for (git = fit->goods.begin(); git != gite; ++git, pgoods++)
					{
						pgoods->id = git->goods_id;
						pgoods->count = git->count;
						pgoods->flagmask = git->flagmask;
						pgoods->timelimit = git->timelimit;
					}
				}
			}
		}
	}
}
inline void release_weborders()
{
	int waring;
}

*/

}; //end of GNET

#endif
