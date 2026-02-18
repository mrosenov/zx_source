#ifndef _HOMETOWN_HOMETOWN_PROTO_IMP_H_
#define _HOMETOWN_HOMETOWN_PROTO_IMP_H_

#include "hometownprotocol.h"
#include "farmcompomanager.h"
#include "hometowndef.h"
#include "hometownitem.h"
#include <assert.h>
#include <algorithm>

namespace HOMETOWN
{
	namespace S2C
	{
		using namespace GNET;
		template <typename CMD>
		struct Make;

		template <>
		struct Make<crop_info>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, const FarmCrop &crop)
			{
				wrapper << crop._id;
				wrapper << crop._seed_time;
				int grow_time_left = crop._grow_period-crop._grow_time;
				if (grow_time_left < 0) grow_time_left = 0;
				wrapper << grow_time_left;
				wrapper << *std::max_element(&crop._protect_period[0], &crop._protect_period[sizeof(crop._protect_period)/sizeof(crop._protect_period[0])]);
				std::set<int> grow_states;
				crop.GetCurrentStates(grow_states);
				std::set<int>::iterator it, ie = grow_states.end();
				int state = 0;
				for (it = grow_states.begin(); it != ie; ++it)
				{
					int st = *it-HTF_STATE_CROP_BEGIN;
					assert(st>=0 && st<32);
					state |= (1<<st);
				}
				wrapper << state;
				wrapper << crop._fruit_left;
				wrapper << crop._fruit_total;
				return wrapper;
			}
		};

		template <>
		struct Make<field_info>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, const FarmField &field)
			{
				wrapper << field._id;
				wrapper << field._type;
				std::set<int> grow_states;
				field.GetCurrentStates(grow_states);
				std::set<int>::iterator it, ie = grow_states.end();
				int state = 0;
				for (it = grow_states.begin(); it != ie; ++it)
				{
					int st = *it-HTF_STATE_FIELD_BEGIN;
					assert(st>=0 && st<32);
					state |= (1<<st);
				}
				wrapper << state;
				wrapper << field._friend;
				char name_len = (char)field._friend_name.size();
				if (name_len<0) name_len = 0;
				if (name_len>20) name_len = 20;
				char name[20] = {};
				memcpy(name, field._friend_name.begin(), name_len);
				wrapper << name_len;
				wrapper.push_back(name, 20);
				Make<crop_info>::From(wrapper, field._crop);
				return wrapper;
			}
		};

		template <>
		struct Make<farm_player>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, const FarmPlayer &role)
			{
				wrapper << role._id;
				wrapper << role._charm;
				wrapper << role._money;
				wrapper << role._level;
				std::set<int> role_states;
				role.GetCurrentStates(role_states);
				std::set<int>::iterator it, ie = role_states.end();
				int state = 0;
				for (it = role_states.begin(); it != ie; ++it)
				{
					int st = *it-HTF_STATE_PLAYER_BEGIN;
					assert(st>=0 && st<32);
					state |= (1<<st);
				}
				wrapper << state;
				return wrapper;
			}
		};

		template <>
		struct Make<item_data>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, const HometownItemData &data)
			{
				wrapper << data._id;
				wrapper << data._pos;
				wrapper << data._count;
				wrapper << data._max_count;
				wrapper << data._data;
				return wrapper;
			}
		};
		
		template <>
		struct Make<pocket_info>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, const HometownItemMan &pocket)
			{
				wrapper << pocket.GetCapacity();

				std::vector<HometownItemData> idatas;
				pocket.GetData(idatas);
				wrapper << idatas.size();

				std::vector<HometownItemData>::iterator it, ie = idatas.end();
				for (it=idatas.begin(); it != ie; ++it)
					Make<item_data>::From(wrapper, *it);
				return wrapper;
			}
		};
		
		template <>
		struct Make<farm_info>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, const FarmManager &fm)
			{
				std::set<int> farm_states;
				fm._farm.GetCurrentStates(farm_states);
				std::set<int>::iterator sit, sie = farm_states.end();
				int state = 0;
				for (sit = farm_states.begin(); sit != sie; ++sit)
				{
					int st = *sit-HTF_STATE_FARM_BEGIN;
					assert(st>=0 && st<32);
					state |= (1<<st);
				}
				wrapper << state;
				wrapper << *std::max_element(&fm._farm._protect_period[0], &fm._farm._protect_period[sizeof(fm._farm._protect_period)/sizeof(fm._farm._protect_period[0])]);
				Make<farm_player>::From(wrapper, fm._owner);
				wrapper << fm._farm._fields.size();
				Farm::FieldMap::const_iterator it, ie = fm._farm._fields.end();
				for (it = fm._farm._fields.begin(); it != ie; ++it)
					Make<field_info>::From(wrapper, it->second);
				wrapper << fm._farm_dynamic.GetDynamics();
				wrapper << fm._action_dynamic.GetDynamics();
				return wrapper;
			}
		};

		template <>
		struct Make<hometown_s2c_cmd_header>
		{
			template <typename WRAPPER>
			static WRAPPER &From(WRAPPER &wrapper, int retcode, int cmd_type, int compo_id)
			{
				wrapper << retcode;
				wrapper << cmd_type;
				wrapper << compo_id;
				return wrapper;
			}
		};
	};
};
#endif
