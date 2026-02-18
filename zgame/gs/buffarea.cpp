#include "buffarea.h"
#include "template/city_region.h"
#include <spinlock.h>
#include "dbgprt.h"

bool buff_area::Init(float map_left, float map_top, float map_right, float map_bottom, float s, city_region* region)
{
	map_region.left = map_left;
	map_region.top = map_top;
	map_region.right = map_right;
	map_region.bottom = map_bottom;
	step = s;
	map_column = (int)((map_region.right -map_region.left) / step);
	map_row = (int)((map_region.bottom -map_region.top) / step);

	size_t cnt = region->GetBuffAreaCnt();
	no_buffarea = (cnt == 0);
	__PRINTF("buffArea count:%d\n", cnt);
	for(size_t i = 0; i < cnt; i ++)
	{
		int area_tid;
		float left, top, right, bottom;
		if(region->GetBuffArea(i, area_tid, left, top, right, bottom))
		{
			int column_start = (int)((left - map_region.left) / step);
			int column_end = (int)((right - map_region.left) / step) + 1;

			int row_start = (int)((top - map_region.top) / step);
			int row_end = (int)((bottom - map_region.top) / step) + 1;

			__PRINTF("buffArea tid=%d, (left,right)=(%f,%f), (top,bottom)=(%f,%f), column(%d,%d), row(%d,%d), grid_cnt=%d\n", area_tid, left, top, right, bottom, column_start, column_end, row_start, row_end, (row_end - row_start + 1) * (column_end - column_start + 1));
			for(int j = row_start; j <= row_end; j ++)
			{
				for(int k = column_start; k <= column_end; k ++)
				{
					float x = map_region.left + k * step + 0.5f;
					float z = map_region.top + j * step + 0.5f;
					if(region->IsInBuffArea(x, z))
					{
						AddArea(j * map_column + k, area_tid);
					}
				}
			}
		}
	}
	return true;
}

void buff_area::GetBuffAreas(float x, float y, abase::vector<int>& areas)
{
	areas.clear();
	BufferAreaMap::iterator it = map.find(GetGridIndex(x, y));
	if(it != map.end())
	{
		for(size_t i = 0; i < it->second.area_tids.size(); i ++)
		{
			areas.push_back(it->second.area_tids[i]);
		}
	}
}

void buff_area::AddArea(int index, int area_tid)
{
	BufferAreaMap::iterator it = map.find(index);
	if(it == map.end())
	{
		buff_area_t b;
		map[index] = b;
	}
	map[index].area_tids.push_back(area_tid);
}

int buff_area::GetGridIndex(float x, float z)
{
	float tmp = (x - map_region.left) / step;
	int c = (int)tmp;
	if(tmp > c) c ++;
	tmp = (z - map_region.top) / step;
	int r = (int)tmp;
	if(tmp > r) r ++;	
	return r * map_column + c; 
}

