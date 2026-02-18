#include "global_drop.h"
#include <math.h>

abase::hashtab<drop_template::drop_entry*,int, abase::_hash_function > drop_template::_drop_table(1024);

bool 
drop_template::LoadDropList()
{
	vector<EXTRADROPTABLE>& list = get_extra_drop_table();
	bool error_flag = false;

	for(size_t i = 0; i < list.size(); i ++)
	{
		if(list[i].type != EDT_TYPE_REPLACE && list[i].type != EDT_TYPE_ADDON)
		{
			printf("掉落表类型错误\n");
			return false;
		}

		drop_entry * pEnt = new drop_entry;
		pEnt->type = list[i].type;
		memcpy(pEnt->drop_num_probability,list[i].drop_num_probability,sizeof(pEnt->drop_num_probability));

		float prop = 0.0f;
		for(size_t k = 0; k < 8; k++)
		{
			prop += pEnt->drop_num_probability[k];
		}
	
		if(fabs(1.0f - prop) > 1e-5)
		{
			printf("掉落表掉落个数概率值不对, 概率=%f index = %d\n", prop, i);
			error_flag = true;
		}

		prop = 0.0f;
		for(size_t j = 0; j < 256; j ++)
		{
			if(list[i].drop_items[j].id == 0) break;

			drop_node node(list[i].drop_items[j].id,  list[i].drop_items[j].probability);
			pEnt->drop_list.push_back(node);
			prop += list[i].drop_items[j].probability;
		}

		if(fabs(1.0f - prop) > 1e-5)
		{
			printf("掉落表掉落物品概率值不对, 概率=%f index = %d \n", prop, i);
			error_flag = true;
		}	
		
		for(size_t j = 0; j < list[i].id_monsters.size(); j ++)
		{
			int id = list[i].id_monsters[j];
			if(!_drop_table.put(id, pEnt))
			{
				printf("掉落表中发现重复的id%d\n",id);
				error_flag = true;
			}
		}
	}
	
	if(error_flag) return false;
	
	vector<EXTRADROPTABLE> list2;

	list.swap(list2);
	return true;
}

