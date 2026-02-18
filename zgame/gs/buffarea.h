#ifndef __BUFFAREA__
#define __BUFFAREA__

#include <hashmap.h>
#include <amemory.h>

class city_region;
class buff_area
{
protected:
	struct buff_area_t
	{
		abase::vector<int> area_tids;
	};
	typedef abase::hash_map<int, buff_area_t, abase::_hash_function, abase::fast_alloc<> > BufferAreaMap;
	BufferAreaMap map;
	
	struct region
	{
		float left;
		float top;
		float right;
		float bottom;
	};
	region map_region;	
	int map_column;
	int map_row;
	float step;
	bool no_buffarea;

public:
	buff_area() : step(1.0f) , no_buffarea(false){}
	~buff_area() {}
	bool Init(float map_left, float map_top, float map_right, float map_bottom, float step, city_region* region);
	void GetBuffAreas(float x, float y, abase::vector<int>& areas);

	bool NoBuffArea() const { return no_buffarea; }

protected:
	void AddArea(int index, int area_tid);
	int GetGridIndex(float x, float z);
};

#endif /*__BUFFAREA__*/
