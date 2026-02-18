#ifndef __PRINT_OFSTREAM_H
#define __PRINT_OFSTREAM_H

#include "mymarshal_i386.h"
#include <algorithm>
#include <vector>
//#include <list>
//#include <deque>
//#include <map>
//#include <set>
#include "common/byteorder.h"
#include "common/octets.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "common/boo.h"
#include "common/conv_charset.h"
#include "storage/storagetool.h"
namespace GNET
{



	class PrintOfstream: public GNET::Marshal::OctetsStream
	{
		std::string filename;
		std::ofstream  outf;
		int filenum;
		int tick;
		int filesize_limit;
		const static int OUTPUTFILE_SIZE_THRESHOLD = 100000000;
		const static int OUTPUTFILE_CHECK_INTERVAL = 100;
		template<class T>
		void print_wrapper(T t)
		{
		//	for(int i=0;i<depth;i++)
		//		outf<<"    ";
			outf<<t;
			outf<<",";
		}
		template<class T>
		void nofield_print_wrapper(T t)
		{
			outf<<t;
			for(int i=0;i<depth;i++)
				outf<<"    ";
	//		outf<<"\n";
		}
		PrintOfstream& push_byte(int t)
		{
			print_wrapper(t);
			return *this;
		}
		PrintOfstream& push_byte(unsigned int t)
		{
			print_wrapper(t);
			return *this;
		}
		PrintOfstream& push_byte(int64_t t)
		{
			print_wrapper(t);
			return *this;
		}
	public:
	//	PrintOfstream():outf("testof",std::ios_base::app),filenum(1){}
		PrintOfstream(const char* pc,int limit=OUTPUTFILE_SIZE_THRESHOLD) : filename(pc),outf(pc,std::ios_base::app),filenum(2),tick(0),filesize_limit(limit)
		{if(limit<=0)limit = OUTPUTFILE_SIZE_THRESHOLD;}
                PrintOfstream& OutputEntry(const Marshal &x)
		{
			change_file();
       			depth=0;
                        operator<<("\n:ENTRY:\n");
                        depth=1;
                        return operator<<(x);
		}
		void change_file(){
			if(tick++<OUTPUTFILE_CHECK_INTERVAL)
				return;
			tick=0;
       			int64_t pos=outf.tellp();
			if(pos>=filesize_limit)
			{
				outf.close();
				std::stringstream ss;
				ss<<filename<<filenum++;
				outf.open(ss.str().c_str(),std::ios_base::app);
			}
		}
		void insert( const void *x, size_t len) { 
			outf<<"onestring";
		}
		void insert( const void *x, const void *y) { 
			outf<<"onestring";
		}

		PrintOfstream& operator << (char x)               { return push_byte(x); }
		PrintOfstream& operator << (unsigned char x)      { return push_byte(x); }
		PrintOfstream& operator << (bool x)               { return push_byte(x); }
		PrintOfstream& operator << (short x)              {
			return push_byte(x);       	
		}
		PrintOfstream& operator << (unsigned short x)     { 
			return push_byte(x);
		}
		PrintOfstream& operator << (int x)                { 
			return push_byte(x);
		}
		PrintOfstream& operator << (unsigned int x)       { 
			return push_byte(x);	
		}
/*		PrintOfstream& operator << (long x)               { 
			return push_byte(x);
		}
		PrintOfstream& operator << (unsigned long x)      {
			return push_byte(x);
		}*/
		PrintOfstream& operator << (int64_t x)            {
			return push_byte(x);       	
		}
		PrintOfstream& operator << (const Marshal &x)     { x.marshal(*this);return *this; }
/*		PrintOfstream& operator << (const Octets &x)  
		{
			insert( x.begin(), x.end());
			outf<<",";
			return *this;
		}*/
		PrintOfstream& operator << (const Octets &x)  
		{
			if(x.size()>0){
                	        Octets name, name2,temp=x;
                	        CharsetConverter::conv_charset_u2l( temp, name );
                	        EscapeCSVString( name, name2 );
				if(name2.size()>0 && name2.size()<17){
					std::string s(static_cast<const char*>(name2.begin()),name2.size());
					print_wrapper(s);
				}
				else
					print_wrapper("");
			}
			else
				print_wrapper("");
			return *this;
		}
	/*	template<typename T>
		PrintOfstream& operator << (const std::basic_string<T> &x)
		{
			STATIC_ASSERT(sizeof(T) == 1); // 需要在服务器处理utf16,utf32时，开放其他sizeof

			size_t bytes = x.length()*sizeof(T); // count of bytes
	//		compact_uint32(bytes);
			insert((void*)x.c_str(), bytes );
			return *this;
		}*/
		PrintOfstream& operator << (const char *x)
		{
			if(*x == '\n')
			{
				nofield_print_wrapper(std::string("\n"));
				++x;
			}
			outf<<std::string(x);
			return *this;
		}
		PrintOfstream& push_byte(const char *x, size_t len)
		{
			insert( x, len);
			outf<<";";
			return *this;
		}
		template<typename T1, typename T2>
		PrintOfstream& operator << (const std::pair<T1, T2> &x)
		{
			return *this << x.first << x.second;
		}
		template<typename T>
	        PrintOfstream& operator << (const std::vector<T> &x) 
	        {
	            return *this <<( MarshalContainer(x));
	        }
	
	};
};

#endif
