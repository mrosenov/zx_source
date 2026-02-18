#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <wchar.h>
#include "ASSERT.h"
#include "topic_site.h"
#include <gsp_if.h>
#include "template/itemdataman.h"
#include "gmatrix.h"

namespace TOPIC_SITE
{
	std::set<int> level_up::levels;
	std::set<int> kill_monster::monsters;
	std::set<int> task_in::tasks_in;
	std::set<unsigned int> got_item::got_items;
	
	bool PackEquipName( packet_wrapper& tbuf, unsigned int itemid )
	{
		DATA_TYPE dt;
		const EQUIPMENT_ESSENCE& data= *(const EQUIPMENT_ESSENCE*)gmatrix::GetDataMan().get_data_ptr( itemid, ID_SPACE_ESSENCE, dt );
		if( &data && (dt == DT_EQUIPMENT_ESSENCE) )
		{
			size_t name_len = sizeof( wchar_t ) * wcslen( (const wchar_t*)data.name );
			name_len = ( ( name_len < 64 ) ? name_len : 64);
			tbuf << name_len;
			tbuf.push_back( (void*)data.name, name_len );
			return true;
		}
		return false;
	}
	
	
	void topic_data::DeliverTopic()
	{
		GMSV::DeliverTopicSite( _player, _type, _tbuf.data(), _tbuf.size() );
	}

	void level_up::DeliverTopic()
	{
		if( _reborn ){ _level += 1000; }
		if( levels.find( _level ) == levels.end() ){ return; }
		_tbuf << _level;
		topic_data::DeliverTopic();
	}
	
	void kill_monster::DeliverTopic()
	{
		if( monsters.find( _monster_id ) == monsters.end() ){ return; }
		_tbuf << _monster_id;
		topic_data::DeliverTopic();
	}
	
	void task_in::DeliverTopic()
	{
		if( tasks_in.find( _taskid ) == tasks_in.end() ){ return; }
		_tbuf << _taskid;
		topic_data::DeliverTopic();
	}
	
	void got_item::DeliverTopic()
	{
		if( got_items.find( _itemid ) == got_items.end() ){ return; }
		_tbuf << _itemid << _cnt;
		topic_data::DeliverTopic();
	}
	
	void achievement::DeliverTopic()
	{
		_tbuf << _achievement_id;
		topic_data::DeliverTopic();
	}
	
	void reinforce::DeliverTopic()
	{
		_tbuf << _level;
		if( !PackEquipName( _tbuf, _itemid ) ){ return; }
		topic_data::DeliverTopic();
	}
	
	void soul::DeliverTopic()
	{
		_tbuf << _level;
		if( !PackEquipName( _tbuf, _itemid ) ){ return; }
		topic_data::DeliverTopic();
	}
	
	void spirit::DeliverTopic()
	{
		if( !PackEquipName( _tbuf, _itemid ) ){ return; }
		//_tbuf << ( ( _len < 64 ) ? _len : 64 ) << _addon_name;
		_tbuf << _len;
		_tbuf.push_back( _addon_name, ( _len < 64 ) ? _len : 64 );
		topic_data::DeliverTopic();
	}
	
	static void split(const char * line, std::vector<std::string> & sub_strings, char div = ',')
	{
		int len = strlen(line);
		char * s = const_cast<char*>(line);
		while (len > 0)
		{
			int i = 0;
			for (; i < len && s[i] != div; ++i) {}
	
			if (i > 0)
			{
				sub_strings.push_back(std::string(s, i));
			}
	
			++i;
			len -= i;
			s += i;
		}
	}
	
	bool LoadConfig( const char* topic_file )
	{
		if( !topic_file || access(topic_file, R_OK) != 0 )
		{
			return false;
		}
		std::ifstream ifs( topic_file );
		std::string line;
		while( std::getline(ifs, line) )
		{
			if (line.size() == 0 || line[0] == ';' || line[0] == '#' || line[0] == '/')
			{
				continue;
			}
			std::vector<std::string> subs;
			split( line.c_str(), subs );
			if( subs.size() < 2 ){ continue; }
			int idx = 0;
			int type = atoi( subs[idx++].c_str() );
			int tid = atoi( subs[idx++].c_str() );
			switch( type )
			{
				case TOPIC_LEVELUP:
				{
					level_up::levels.insert( tid );
					break;
				}
				case TOPIC_KILL_MONSTER:
				{
					kill_monster::monsters.insert( tid );
					break;
				}
				case TOPIC_FINISH_TASK:
				{
					task_in::tasks_in.insert( tid );
					break;
				}
				case TOPIC_GOT_ITEM:
				{
					got_item::got_items.insert( tid );
					break;
				}
				default:
					break;
			}
		}
		return true;
	}
};
