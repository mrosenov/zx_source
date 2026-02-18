#ifndef __ONLINEGAME_GS_TOPICSITE_H__
#define __ONLINEGAME_GS_TOPICSITE_H__ 

#include <map>
#include <set>
#include <common/packetwrapper.h>
#include <wchar.h>
#include "template/exptypes.h"

namespace TOPIC_SITE
{
#pragma pack(1)
	enum TOPICS
	{
		TOPIC_LEVELUP = 1000,	// 升级
		TOPIC_KILL_MONSTER,	// 杀怪
		TOPIC_FINISH_TASK,	// 完成任务
		TOPIC_GOT_ITEM,		// 获得物品
		TOPIC_ACHIEVEMENT,	// 完成成就
		TOPIC_REINFORCE,	// 炼器
		TOPIC_SOUL,		// 灌注
		TOPIC_SPIRIT,		// 血祭
		TOPIC_END,
	};
	
	struct topic_data
	{
		topic_data(){}
		virtual ~topic_data(){}
		topic_data( int type ) : _type( type ) {}
		topic_data( int type, int player ) : _type( type ), _player( player ){}
		int _type;
		int _player;
	
		packet_wrapper _tbuf;
		virtual void DeliverTopic();
	};
	
	struct level_up : public topic_data
	{
		level_up( bool reborn, int level ) : topic_data( TOPIC_LEVELUP ), _reborn( reborn ), _level( level ){}
		virtual void DeliverTopic();
		bool _reborn;
		int _level;
		
		static std::set<int> levels;
	};
	
	struct kill_monster : public topic_data
	{
		kill_monster( int monster ) : topic_data( TOPIC_KILL_MONSTER ), _monster_id( monster ){}
		virtual void DeliverTopic();
		int _monster_id;

		static std::set<int> monsters;
	};

	struct task_in : public topic_data
	{
		task_in( int taskid ) : topic_data( TOPIC_FINISH_TASK ), _taskid( taskid ){}
		virtual void DeliverTopic();
		int _taskid;
	
		static std::set<int> tasks_in;
	};

	struct got_item : public topic_data
	{
		got_item( unsigned int item, int cnt ) : topic_data( TOPIC_GOT_ITEM ), _itemid( item ), _cnt( cnt ){}
		virtual void DeliverTopic();
		unsigned int _itemid;
		int _cnt;
		
		static std::set<unsigned int> got_items;
	};

	struct achievement : public topic_data
	{
		achievement( int achievement_id ) : topic_data( TOPIC_ACHIEVEMENT ), _achievement_id( achievement_id ){}
		virtual void DeliverTopic();
		int _achievement_id;
	};

	struct reinforce : public topic_data
	{
		reinforce( unsigned int item, int level ) : topic_data( TOPIC_REINFORCE ), _itemid( item ), _level( level ){}
		virtual void DeliverTopic();
		unsigned int _itemid;
		int _level;
	};

	struct soul : public topic_data
	{
		soul( unsigned int item, int level ) : topic_data( TOPIC_SOUL ), _itemid( item ), _level( level ){}
		virtual void DeliverTopic();
		unsigned int _itemid;
		int _level;
	};

	struct spirit : public topic_data
	{
		spirit( unsigned int item, wchar_t* addonname, size_t len ) : topic_data( TOPIC_SPIRIT ), _itemid( item ), _len( len ), _addon_name( addonname ){}
		virtual void DeliverTopic();
		unsigned int _itemid;
		size_t _len;
		wchar_t* _addon_name;
	};

#pragma pack()

	bool LoadConfig( const char* topic_file );
	bool PackEquipName( packet_wrapper& tbuf, unsigned int itemid );
};

#endif
