#ifndef __LOG_EXCEPTION_HPP__
#define __LOG_EXCEPTION_HPP__

#include "util.hpp"
#include <string>

namespace LOG
{
	class Exception 
	{ 
	private:
		std::string msg;
	public:
		Exception(std::string _msg = ""):msg(_msg){}
		std::string what() { return msg; }
	};
}

#endif
