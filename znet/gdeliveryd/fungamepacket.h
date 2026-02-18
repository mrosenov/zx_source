#ifndef __GNET_FUNGAME_PACKET_H
#define __GNET_FUNGAME_PACKET_H

#include "common/octets.h"

namespace FUNGAME
{

using namespace GNET;
class fun_game_packet_wrapper
{
	Octets _buf;
	template <typename T>
	void push_byte(T t)
	{
		_buf.insert(_buf.end(), &t,sizeof(t));
	}
	void push_byte(const void * buf,size_t size)
	{
		_buf.insert(_buf.end(), buf,size);
	}
public:
	int _counter;
	fun_game_packet_wrapper():_buf(),_counter(0){}
	explicit fun_game_packet_wrapper(size_t size):_buf(size),_counter(0){}
	void swap(fun_game_packet_wrapper &rhs)
	{
		_buf.swap(rhs._buf);
		std::swap(_counter,rhs._counter);
	}

	fun_game_packet_wrapper & operator<<(int val)                {push_byte(val);return *this;}
	fun_game_packet_wrapper & operator<<(unsigned int val)       {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(int64_t val)   	     {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(short val)              {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(unsigned short val)     {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(char val)               {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(unsigned char val)      {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(bool val)               {return operator<<((char)val);}
	fun_game_packet_wrapper & operator<<(float val)              {push_byte(val);return *this;} 
	fun_game_packet_wrapper & operator<<(const char * str) {push_byte(str,strlen(str)+1);return *this;} 
	fun_game_packet_wrapper & operator<<(const Octets& buf)
	{
		push_byte(buf.size());
		push_byte(buf.begin(),buf.size());
		return *this;
	}

	template <typename T, template <typename> class CONTAINER>
	fun_game_packet_wrapper & operator<<(const CONTAINER<T> &cont)
	{
		size_t n = cont.size();
		*this << n;
		typename CONTAINER<T>::const_iterator it, ie = cont.end();
		for (it = cont.begin(); it != ie; ++it)
			*this << *it;
		return *this;
	}

	fun_game_packet_wrapper & operator<<(const fun_game_packet_wrapper& wrapper)
	{
		push_byte(wrapper._buf.begin(),wrapper._buf.size());
		return *this;
	}
	fun_game_packet_wrapper & push_back(const void * buf,size_t size)
	{
		push_byte(buf,size);
		return *this;
	}
	Octets & get_buf() { return _buf;}
	void * data() { return _buf.begin();}
	size_t size() { return _buf.size();}
	size_t raw_size() { return _buf.size();}
	void clear() {
		_buf.clear();
		_counter = 0;
	}
	int get_counter() { return _counter;}
};
	
typedef fun_game_packet_wrapper WRAPPER;
}; //end namespace FUNGAME 

#endif
