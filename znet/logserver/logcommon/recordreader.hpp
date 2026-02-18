#ifndef __LOG_RECORDREADER_LOG__
#define __LOG_RECORDREADER_LOG__

#include "logcommon/pagebuffer.hpp"
#include "logcommon/pagefile.hpp"
#include <deque>
#include <algorithm>
namespace LOG
{

// _Category, _Tp, _Distance, _Pointer, _Reference
class xrange : public std::iterator<std::random_access_iterator_tag, int, int, int, int >
{
        int _x;
public:
        xrange() : _x(-1) { }
        explicit xrange(int __x) : _x(__x) { }
        xrange(const xrange & __rhs) : _x(__rhs._x) { }

        reference operator * () const             { return _x; }

        xrange & operator ++()                    { ++_x; return *this; }
        xrange   operator ++(int)                 { xrange tmp = *this; ++_x; return tmp; }
        xrange & operator --()                    { --_x; return *this; }
        xrange   operator --(int)                 { xrange tmp = *this; --_x; return tmp; }
        xrange & operator +=(difference_type __i) { _x += __i; return *this; }
        xrange & operator -=(difference_type __i) { _x -= __i; return *this; }
        xrange   operator + (difference_type __i) { xrange tmp = *this; return tmp += __i; }
        xrange   operator - (difference_type __i) { xrange tmp = *this; return tmp -= __i; }

        reference operator [](difference_type __i) { return *(*this + __i); }

        bool operator == (const xrange & __rhs) const { return _x == __rhs._x; }
        bool operator != (const xrange & __rhs) const { return _x != __rhs._x; }
        bool operator <  (const xrange & __rhs) const { return _x < __rhs._x; }
        bool operator >  (const xrange & __rhs) const { return _x > __rhs._x; }
        bool operator <= (const xrange & __rhs) const { return _x <= __rhs._x; }
        bool operator >= (const xrange & __rhs) const { return _x >= __rhs._x; }

        // distance
        difference_type operator - (const xrange & __rhs) const { return _x - __rhs._x; }
};

class RecordReader;

class Comp
{
public:
        Comp(time_t _b, RecordReader * _rr) : begin(_b), rr(_rr) { }

        time_t begin;
	RecordReader * rr;
        time_t getpagetime(int pageidx) const;
        bool operator ()(int i1, int i2) const
        {
                //printf("\tcomp(%d, %d)\n", i1, i2); //return i1 < i2;
                return getpagetime(i1) < getpagetime(i2);
        }
};
	
class RecordReader
{
public:
	/* RecordReader pos 的初始值为0, pos指向的绝对位置和PageBuffer的pos不同.  */

	RecordReader( const std::string &filename, bool w  )
		: pages(filename, true /* readonly */), wait(w), pos(0) { }

	void printptime() //for test
	{
		for (size_t xxxx = 1; xxxx  <= pages.count(); xxxx++)
		{
			std::cout << xxxx  << " =  " << getpagetime(xxxx) << std::endl;
		}
	}

	void printctime() //for test
	{
		std::cout << "current time = " << page.timestamp() << std::endl;
	}

	bool seektime( time_t begin_time ) //MYTODO
	{
		xrange first(1), last(pages.count() + 1);
		xrange ix = std::lower_bound( first, last, -1, Comp(begin_time, this));
		if (ix != last)
		{
			if (ix == first)
				return seekreset(*first);
			std::cout << "found " << *ix << std::endl;
			--ix;
			if ( !pages.seek(*ix) )        return false;
			if ( !pages.read(page, false) ) return false;
			pos = page.first_record();
			GNET::Marshal::OctetsStream dummy_data;
			RecordHead rh;
			IndexInfo ii;
			while( _read(dummy_data, rh, &ii, false) )
			{
				if ( rh.time >= begin_time )
					return seek(ii);
			}
			return false;
		}
		return seekreset(*last);
	}
	
	bool seektail(size_t nr = 10) //nr tail lines
	{
		IndexInfo *end = NULL, enddata ;
		int pc = pages.count();
		for (int p = pc;  p > 0 && nr > 0; --p)
		{
			if ( !pages.seek(p) )          return false;
			if ( !pages.read(page, false) ) return false;
			if ( page.ispiece() )          continue;

			std::deque<IndexInfo> iix;

			pos = page.first_record();
			IndexInfo ii;
			RecordHead rh;
			GNET::Marshal::OctetsStream dummy_data;
			while ( _read_nowait(dummy_data, rh, ii, end))
			{
				iix.push_back(ii);
				if (iix.size() > nr)
					iix.pop_front();
			}
			nr -= iix.size();
			if (!iix.empty())
			{
				enddata = iix.front();
				end = &enddata;
			}
		}
		if (end) return seek(enddata);

		return seekreset( pc + 1 );
	}

	bool seek( const IndexInfo & ii )
	{
		if (pages.seek( ii.offset / PageBuffer::SIZE ) && pages.read( page, false ))
		{
			pos = ii.offset % PageBuffer::SIZE;
			return true;
		}
		return false;
	}

	bool read( GNET::Marshal::OctetsStream &data, RecordHead &rh, IndexInfo * ii = NULL) 
	{
		return _read(data, rh, ii, wait);
	}

private:
	bool _read( GNET::Marshal::OctetsStream &data, RecordHead &rh, IndexInfo * ii, bool iswait ) 
	{
		if ( !pumppage(false, iswait) )  return false;
		if ( ii )
			ii->offset = page.getoffset() + pos;

		data.clear();
		if ( !pump( data, RecordHead::SIZE, iswait ) ) return false;
		data >> rh;
		if( ii )
			ii->rlen = rh.rlen;

		data.clear();
		return pump( data, rh.rlen - RecordHead::SIZE, iswait );
	}

	bool  _read_nowait( GNET::Marshal::OctetsStream &data, RecordHead &rh, IndexInfo & ii , const IndexInfo *end )
	{
		if ( !_read( data, rh, &ii, false) ) return false; 
		if ( end && *end == ii ) return false;
		return true;
	}

	bool seekreset( int pagenum )
	{
		if (!pages.seek( pagenum )) return false;
		page.reset(0);
		return true;
	}


	bool pump( GNET::Marshal::OctetsStream &data, size_t len, bool iswait)
	{
		len -= data.size();
		while (pumppage(true, iswait))
		{
			size_t clen = std::min( len - data.size(), page.dsize() - pos );
			data.insert( data.end(), page.data() + pos, clen );
			pos += clen;
			if( data.size() >= len ) return true;	
		}
		return false;
	}

	bool pumppage(bool cont, bool iswait)
	{
		if (page.dsize() <= pos)
		{
			if ( !pages.read(page, iswait) ) return false;
			pos = cont ? 0 : page.first_record();
		}
		return true;
	}

	friend class Comp;
	time_t getpagetime( size_t pagenum )
	{
		if ( ! pages.seek( pagenum ) )        throw LOG::Exception( "seek page error" );
		if ( ! pages.read( page, false ) )    throw LOG::Exception( "read page error" ); 
		return page.timestamp();
	}
	
	PageFile pages;
	PageBuffer page;
	bool wait;
	size_t pos;
};

inline time_t Comp::getpagetime(int pageidx) const
{
	if (pageidx > 0) return rr->getpagetime( pageidx );//(pageidx)->gettiem();
	return begin;
}

} //end namespace LOG

#endif
