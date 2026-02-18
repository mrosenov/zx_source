#ifndef __GNET_SERVERLOAD_H
#define __GNET_SERVERLOAD_H

#include "lineplayerlimit"
#include "localmacro.h"

namespace GNET
{

class ServerLoad
{
	int server_limit;
	int server_count;
	int CheckNumber(int cur_num, int max_num) const
	{
		if (max_num > 50)
			return (cur_num >= max_num-50) ? ERR_EXCEED_LINE_MAXNUM : ERR_SUCCESS;
		else
			return (cur_num >= max_num) ? ERR_EXCEED_LINE_MAXNUM : ERR_SUCCESS;
	}
protected:
	LinePlayerLimitVector line_loads;
public:
	ServerLoad():server_limit(0), server_count(0) {}
	void SetLoad(int srv_limit, int srv_count, const LinePlayerLimitVector & lines)
	{
		server_limit = srv_limit;
		server_count = srv_count;
		line_loads = lines;
	}
	void ClearLoad()
	{
		server_limit = 0;
		server_count = 0;
		line_loads.clear();
	}
	int CheckLimit(int line) const 
	{
		if (server_count >= server_limit)
			return ERR_SERVEROVERLOAD;
		LinePlayerLimitVector::const_iterator it, ite = line_loads.end();
		for (it = line_loads.begin(); it != ite; ++it)
		{
			if (it->line_id == line)
			{
				//LOG_TRACE("SelectLine cur_num=%d, max_num=%d", it->cur_num, it->max_num);
				if (it->attr&GS_ATTR_HIDE)
					return ERR_INVALID_LINEID;
				else
					return CheckNumber(it->cur_num, it->max_num);
			}
		}
		return ERR_INVALID_LINEID;
	}
	int SelectLine(int & line) const
	{
		//return CheckLimit(line);
		int ret = CheckLimit(line);
		if (ret == ERR_SUCCESS || ret == ERR_SERVEROVERLOAD)
			return ret;
		size_t try_count = 0;
		size_t line_count = line_loads.size();
		if (line_count == 0)
			return ERR_SERVEROVERLOAD;
		int start_index = rand()%line_count;
		LinePlayerLimitVector::const_iterator it = line_loads.begin()+start_index;
		while(try_count < line_count)
		{
			LOG_TRACE("SelectLine try line %d cur_num %d is_hide %d", it->line_id, it->cur_num, (it->attr&GS_ATTR_HIDE));
			if ((it->attr&GS_ATTR_HIDE)== 0 && CheckNumber(it->cur_num, it->max_num) == ERR_SUCCESS)
			{
				line = it->line_id;
				return ERR_SUCCESS;
			}
			try_count++;
			it++;
			if (it == line_loads.end())
				it = line_loads.begin();
		}
		return ERR_SERVEROVERLOAD;
	}
};
class LocalServerLoad:private ServerLoad
{
public:
	const LinePlayerLimitVector& GetLineLimit(){return line_loads;}
//	void SetLineLimit(LinePlayerLimitVector& v){line_loads = v;}
	void SetLoad(int srv_limit, int srv_count, const LinePlayerLimitVector & lines){
		ServerLoad::SetLoad(srv_limit,srv_count,lines);
	}
	int SelectLine(int & line) const{return ServerLoad::SelectLine(line);}
//	LocalServerLoad(){}	
};

};
#endif
