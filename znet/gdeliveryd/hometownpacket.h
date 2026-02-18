#ifndef _hometown_hometown_packet_h_
#define _hometown_hometown_packet_h_

#include "fungamepacket.h"

namespace HOMETOWN
{
	class hometown_packet_wrapper : public FUNGAME::fun_game_packet_wrapper
	{
	public:
		int &get_retcode() { return *(int *)data(); }
		void set_retcode(int retcode) { get_retcode() = retcode; }
	};
	
	typedef hometown_packet_wrapper WRAPPER;
};
#endif
