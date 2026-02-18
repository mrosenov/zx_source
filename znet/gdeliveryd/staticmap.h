#ifndef __ONLINEGAME_GS_STATIC_MAP_TEMPLATE_H__
#define __ONLINEGAME_GS_STATIC_MAP_TEMPLATE_H__

#include <algorithm>
#include <vector>
using namespace std;
namespace GNET
{
template <class _Key, class _Value>
class static_multimap
{
public:
	typedef _Key	key_type;
	typedef _Value	data_type;
	typedef _Value	mapped_type;
	typedef pair<_Key,_Value> value_type;
	typedef size_t size_type;
	typedef vector<value_type> list_type;

	typedef typename list_type::iterator iterator;
	typedef typename list_type::const_iterator const_iterator;
private:
	vector<value_type> _data;
	inline static bool __KeyCompare(const value_type & lhs, const key_type & __key)
	{
		return lhs.first < __key;
	}
	inline iterator __Find(const key_type & __key) 
	{
		return std::lower_bound(begin(),end(),__key,__KeyCompare);
	}
public:
	static_multimap()
	{} 

	void swap( static_multimap<_Key,_Value> & rhs)
	{
		_data.swap(rhs._data);
	}

	void reserve(size_t capacity)
	{
		if(capacity > _data.capacity())
		{
			_data.reserve(capacity);
		}
	}
public:
	size_type size()  const { return _data.size();}
	bool empty() const { return _data.empty();}
	iterator begin() { return _data.begin();}
	iterator end() { return _data.end();}
	const_iterator begin() const { return _data.begin();}
	const_iterator end() const { return _data.end();}

	iterator find(const key_type& __key) 
	{
		iterator it = __Find(__key);
		if(it != end() && it->first == __key)
		{
			return it;
		}
		return end();
	}

	data_type & operator[](const key_type & __key)
	{
		iterator it = find(__key);
		if(it != end()) return it->second;
		return insert(value_type(__key,data_type())).first->second;
	}


	pair<iterator, bool> insert(const value_type& x)
	{
		iterator it = __Find(x.first);
		int index = it - _data.begin();
		_data.insert(it,x);
		return pair<iterator,bool>(_data.begin() + index,true);
	}

	size_type erase(const key_type& __key) 
	{
		iterator it = __Find(__key);
		if(it == end()) return 0;
		iterator it2 = it;
		while(it2 != end() && it2->first == __key)
		{
			++it2;
		}
		_data.erase(it,it2);
		return it2 - it;
	}

	iterator erase(iterator __it)
	{
		_data.erase(__it);
		return __it;
	}
	
	void clear()
	{
		_data.clear();
	}

};

} // namespace 
#endif

