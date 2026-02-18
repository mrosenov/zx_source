#include "world.h"
#include "mobactivedataman.h"
#include "template/itemdataman.h"

bool mob_active_mount_point_dataman::LoadTemplate(itemdataman& dataman)
{
	DATA_TYPE  dt; 
	size_t id = dataman.get_first_data_id(ID_SPACE_CONFIG,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_CONFIG,dt))
	{       
		if (dt == DT_INTERACTION_OBJECT_HOOK_CONFIG)
		{
			const INTERACTION_OBJECT_HOOK_CONFIG& ess = *(const INTERACTION_OBJECT_HOOK_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(&ess && dt == DT_INTERACTION_OBJECT_HOOK_CONFIG);
			mob_active_mount_point_template mampt;
			mampt.tid = ess.id;
			mampt.ready_task_out = ess.start_task_id;
			mampt.activate_ready_controller_id = ess.start_open_controller_id;
			mampt.deactivate_ready_controller_id = ess.start_close_controller_id;
			mampt.end_task_out = ess.end_task_id;
			mampt.activate_end_controller_id = ess.end_open_controller_id;
			mampt.deactivate_end_controller_id = ess.end_close_controller_id;
			mampt.player_can_be_finish = ess.showbutton;
			mampt.interrupt_condition = ess.accident;
			mampt.goal_pos[0].x = ess.absx;
			mampt.goal_pos[0].y = ess.absy;
			mampt.goal_pos[0].z = ess.absz;
			mampt.goal_pos[1].x = ess.relativex;
			mampt.goal_pos[1].y = ess.relativey;
			mampt.goal_pos[1].z = ess.relativez;

			for (size_t i = 0; i < 5; i++)
			{
				if (ess.item_info[i].prop < 0)
				{

					printf("掉落表%d发现负数概率\n",mampt.tid);
					continue;
				}
				if (ess.item_info[i].prop > 0 && ess.item_info[i].item_id < 0)
				{
					printf("掉落表%d发现无效物品\n",mampt.tid);
					continue;
				}

			}
			Insert(mampt);
		}
	}
	return true;
}
